/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnssend.c,v $
 * Revision 1.1.7.2  1996/02/18  19:34:42  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:14:46  marty]
 *
 * Revision 1.1.7.1  1995/12/08  15:26:02  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:46:44  root]
 * 
 * Revision 1.1.4.5  1994/06/30  19:17:35  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:03:00  mccann]
 * 
 * Revision 1.1.4.4  1994/06/09  18:42:15  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:14:25  devsrc]
 * 
 * Revision 1.1.4.3  1994/06/08  20:16:35  peckham
 * 	OT#3503: fix casting on pthread_cleanup_push().
 * 	Change definition of cds_cleanup_cdsFlagStat().
 * 	[1994/05/31  18:54:56  peckham]
 * 
 * Revision 1.1.4.2  1994/04/29  15:54:57  peckham
 * 	Use dcecdsmac.h macros for dce_svc_printf().
 * 	Rename dns*() -> cds*().
 * 	[1994/04/29  14:26:49  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  22:07:39  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:32:37  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:59:11  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:14:54  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:19:24  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* MODULE dnssend.c
 *
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1990-1994. ALL RIGHTS RESERVED.
 *
 * THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
 * ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
 * INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
 * COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
 * OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
 * TRANSFERRED.
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
 * AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
 * CORPORATION.
 *
 * DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
 * SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
 *
 */
  
#include <pthread.h>
#include <dce/dce.h>
#include <dce/utc.h>
#include <dce/cdsclerk.h>
#include <dcecdssvc.h>
#include <errno.h>

#include <sys/types.h>
#include <time.h>
#include <dce/assert.h>
#include <uc_clerk.h>     
#include <uc_parse.h>

#define	CDS_UNIX_EPIPE	(CDS_UNIXERRORS + EPIPE)

/* global data */

static pthread_once_t cds_send_once = pthread_once_init;
pthread_mutex_t cds_send_mutex;         /* master mutex - used by dns_read.c */
static cdsFlagStat_t *cds_writer_p;     /* current writer to dns_socket */
static pthread_cond_t cds_no_writer;    /* when current writer is finished */

#define DNS_INIT	{ pthread_once(&cds_send_once, cds_send_init); }
#define DNS_LOCK	{ pthread_mutex_lock(&cds_send_mutex); }
#define DNS_END_LOCK	{ pthread_mutex_unlock(&cds_send_mutex); }
#define	DNS_WAIT(cv)	{ \
    pthread_cleanup_push((void (*)(void *))pthread_mutex_unlock, &cds_send_mutex); \
    pthread_cond_wait(cv, &cds_send_mutex); \
    pthread_cleanup_pop(0); }

static void
cds_cleanup_cdsFlagStat (
    void		*);

/*
 * Initialize this module's statics
 */
static void
cds_send_init (void)
{
    int a1, a2;  /* assertions */
    
    /*
     * verify that op and version of the request packet are located
     * at bytes 12 and 13 of the data structure.
     */

    a1 = DIFF_bytes(&(((ucp_rqst_header *)NULL)->op), (ucp_rqst_header *)NULL);
    dce_assert(cds__svc_handle, a1 == 12);

    a2 = DIFF_bytes(&(((ucp_rqst_header *)NULL)->version),
		    (ucp_rqst_header *)NULL);
    dce_assert(cds__svc_handle, a2 == 13);

    pthread_mutex_init(&cds_send_mutex, pthread_mutexattr_default);
    pthread_cond_init(&cds_no_writer, pthread_condattr_default);
    cds_writer_p = NULL;
}


static unsigned long cds_send_seq = 31415;	/* send sequence number */

/* ----------------------------------------------------------------------
 *  Send the message to the child, and wait for response if synchronous
 * ----------------------------------------------------------------------
 */
int 
cds_send (cdsFlagStat_t  *Flags_p,
          byte_t         *msg_p)
{
    register ucp_rqst_header *hdr_p = (ucp_rqst_header *)msg_p;
    int msglen;
    register int status;
    int returnsts;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >cds_send"));

    DNS_INIT;

    dce_assert(cds__svc_handle,
	       sizeof(hdr_p->sequence) <= sizeof(Flags_p->fsOpqCDS[OPQSEQ]));
    dce_assert(cds__svc_handle,OPQSEQ < OPQ_SIZ);

    DNS_LOCK {
	Flags_p->fsOpqCDS[OPQSEQ] = cds_send_seq++;
    } DNS_END_LOCK;

    cds_status_clerk(Flags_p, CDS_PENDING);
    /*
     * Fill common information into the header.
     *
     * Fields user_uid and user_gid are set by cds_write().
     */
    UCP_INSlongword(hdr_p->sequence, Flags_p->fsOpqCDS[OPQSEQ]);
    UCP_INSbyte(hdr_p->version, UCP_VERSION);
    UCP_INSbyte(hdr_p->dontCache, Flags_p->fsDontCache);
    UCP_INSaddress(hdr_p->fromCache_p, &Flags_p->fsFromCache);
    UCP_INSaddress(hdr_p->outLink_p, &Flags_p->fsOutLink);
    UCP_INSlongword(hdr_p->cacheTimeout, Flags_p->fsCacheTimeout);
    UCP_INSbyte(hdr_p->conf, cds_validate_fsConf(Flags_p->fsConf));
    UCP_INSlongword(hdr_p->wait, cds_validate_fsWait(Flags_p->fsWait));
#if defined(DCE_SEC)
    UCP_INSbyte(hdr_p->UnAuth, Flags_p->fsUnAuth);
    UCP_INSbyte(hdr_p->TrustAll, Flags_p->fsTrustAll);
#else
    UCP_INSbyte(hdr_p->UnAuth, 1);
    UCP_INSbyte(hdr_p->TrustAll, 1);
#endif
    /*
     * The message length includes a header
     */
    msglen = sizeof(hdr_p->length) + UCP_EXTlongword(hdr_p->length);

    /*
     * From here on, the cma version must diverge considerably
     */

    dce_assert(cds__svc_handle,
	       sizeof(msg_p) <= sizeof(Flags_p->fsOpqCDS[OPQMSGP]));
    dce_assert(cds__svc_handle,OPQMSGP < OPQ_SIZ);
    Flags_p->fsOpqCDS[OPQMSGP] = (unsigned long)msg_p;

    /*
     * Place this request on the pending ring
     */
    if (cds_read_push(Flags_p) == CDS_SUCCESS) {
	cds_full_name_t resolved;
	unsigned char STR_temp[CDS_STR_FNAME_MAX];
	int resolvedLen;

	pthread_cleanup_push(cds_cleanup_cdsFlagStat, Flags_p);

	/* This must be synchronized with dns_validate_NameType() */
	switch (Flags_p->fsNameType) {
	case cdsOpqDNA:
	    UCP_INSaddress(hdr_p->partialResults, Flags_p->fsPartialRes);
	    UCP_INSlongword(hdr_p->resLen, Flags_p->fsResLength);
	    UCP_INSaddress(hdr_p->resLen_p, &Flags_p->fsResLength);
	    break;
	default:
	    UCP_INSaddress(hdr_p->partialResults, &resolved);
	    UCP_INSlongword(hdr_p->resLen, sizeof(resolved));
	    UCP_INSaddress(hdr_p->resLen_p, &resolvedLen);
	    break;
	}
	DNS_LOCK {
	    while (cds_writer_p)
		DNS_WAIT(&cds_no_writer);

	    cds_writer_p = Flags_p;
	} DNS_END_LOCK;

	/*
	 * If the pipe goes away, retry once.
	 */
	if (((status = cds_write(Flags_p, msg_p, msglen)) != CDS_SUCCESS) &&
		    (Flags_p->fsLocStat == CDS_UNIX_EPIPE)) {
	    cds_status_clerk(Flags_p, CDS_PENDING); /* reset fsLocStat */
	    status = cds_write(Flags_p, msg_p, msglen);
	}
	/* finished sending request */
	cds_writer_p = NULL;
	pthread_cond_signal(&cds_no_writer);

	/* request buffer is no longer needed */
	Flags_p->fsOpqCDS[OPQMSGP] = 0;
	free((char *)msg_p);

	/*
	 * Wait for the request to complete or cancel.
	 */
	if (status == CDS_SUCCESS)
	    cds_read(Flags_p);

	pthread_cleanup_pop(1);

	/* This must be synchronized with dns_validate_NameType() */
	if ((Flags_p->fsLocStat == CDS_UNKNOWNENTRY)
	    && (0 < resolvedLen) && Flags_p->fsPartialRes)
	{
	    switch (Flags_p->fsNameType) {
	    case 0:
	    case cdsStrDCE:
		if (cdsCvtFullToStr(&resolved,
			(unsigned char *)Flags_p->fsPartialRes,
			&Flags_p->fsResLength) == CDS_SUCCESS)
		    break;

		/* ERROR - return nothing for now */
		Flags_p->fsResLength = 0;
		break;

	    case cdsStrXDS:
		resolvedLen = sizeof(STR_temp);
		if (cdsCvtFullToStr(&resolved, STR_temp,
			&resolvedLen) == CDS_SUCCESS) {
		    resolvedLen = sizeof(resolved);
		    if (dns_CvtCDSFullToGDS(STR_temp, 1, 0, &resolved,
			    &resolvedLen) == CDS_SUCCESS)
			if (cdsCvtFullToStr(&resolved,
				    (unsigned char *)Flags_p->fsPartialRes,
				    &Flags_p->fsResLength) == CDS_SUCCESS)
			    break;
		}

		/* ERROR - return nothing for now */
		Flags_p->fsResLength = 0;
		break;

	    case cdsStrDNA:
		/*	
		 * I don't know what to do here 
		 */  
		dce_assert(cds__svc_handle, 1 == 2);

	    case cdsOpqDNA:
		break;
	    }
	}
    }

    returnsts = cds_status(Flags_p);
    
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " <cds_send: returning cds_status(Flags_p)=%d",
	returnsts));

    return (returnsts);
    
}

/*
 * Clean up use of cdsFlagStat structure if thread is cancelled.
 */
static void 
cds_cleanup_cdsFlagStat (void *arg)
{
    cdsFlagStat_t *const Flags_p = arg;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >cds_cleanup_cdsFlagStat(%p)", arg));

    DNS_LOCK {
	byte_t *msg_p;

	/*
	 * If she is the designated writer, stand aside...
	 */
	if (Flags_p == cds_writer_p) {
	    cds_writer_p = NULL;
	    pthread_cond_signal(&cds_no_writer);
	}
	/*
	 * Release the message buffer.
	 */
	if (msg_p = (byte_t *)Flags_p->fsOpqCDS[OPQMSGP])
	    free((char *)msg_p);
    } DNS_END_LOCK;

    cds_read_pop(Flags_p);

  DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " <cds_cleanup_cdsFlagStat(%p)", arg));
}
