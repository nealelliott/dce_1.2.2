/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnsread.c,v $
 * Revision 1.1.9.1  1996/05/10  13:07:37  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/04/18  19:24 UTC  bissen
 * 	Single-threaded changes
 *
 * 	HP revision /main/pare_mothra/3  1995/02/06  13:34 UTC  pare
 * 	Check for i == -1 and errno == EINTR in else clause of read. Defect CHFts14276
 *
 * 	HP revision /main/pare_mothra/2  1995/02/02  18:02 UTC  pare
 * 	Check errno after read for single threaded case
 *
 * 	HP revision /main/pare_mothra/1  1995/01/23  13:15 UTC  pare
 * 	Add single threaded client changes.
 * 	[1996/05/09  20:43:04  arvind]
 *
 * Revision 1.1.5.6  1994/08/03  19:03:08  mccann
 * 	includes file cleanup
 * 	[1994/07/19  18:10:00  mccann]
 * 
 * Revision 1.1.5.5  1994/06/30  19:17:25  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:02:24  mccann]
 * 
 * Revision 1.1.5.4  1994/06/09  18:42:01  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:14:14  devsrc]
 * 
 * Revision 1.1.5.3  1994/06/08  20:16:34  peckham
 * 	OT#3503: fix casting on pthread_cleanup_push().
 * 	Change definition of cds_reader_cleanup().
 * 	[1994/05/31  18:54:50  peckham]
 * 
 * Revision 1.1.5.2  1994/04/29  15:54:33  peckham
 * 	Change dns* to cds*
 * 	[1994/04/29  14:25:34  peckham]
 * 
 * Revision 1.1.5.1  1994/03/12  22:07:22  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:31:32  peckham]
 * 
 * Revision 1.1.3.3  1992/12/30  13:58:19  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:13:49  zeliff]
 * 
 * Revision 1.1.3.2  1992/10/26  20:59:47  peckham
 * 	OT#4770: CV was being signalled before dns_reader_Flags_p was modified.
 * 	Also protect modification.
 * 	[1992/10/26  20:58:31  peckham]
 * 
 * Revision 1.1  1992/01/19  15:18:39  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* MODULE dnsread.c
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

#include <dce/dce.h>
#include <dce/cdsclerk.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <pthread.h>
#include <dce/assert.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <uc_clerk.h>
#include <uc_parse.h>


extern int cds_clerk_socket;
#define	LOCAL_READ_SIZE	100


/*
 * Use mutex defined in dnssend.c
 */
extern pthread_mutex_t cds_send_mutex;	/* master mutex - from dnssend.c */

static pthread_t cds_reader_thread;	/* reader thread */
static volatile enum {			/* state of reader thread: */
    reader_init = 0,			/*  initialize condition variable */
    reader_none,			/*  thread not active */
    reader_active,			/*  thread processing responses */
    reader_restart,			/*  thread cancelled - restart */
    reader_cancelled,			/*  thread cancelled by cds_unbind() */
    reader_rundown			/*  comm error - fd rundown */
} cds_reader_state;
static pthread_cond_t cds_reader_state_changed;

typedef enum {			/* read state: */
    read_none = 0,		/*  not reading anything */
    read_header,		/*  reading response header */
    read_body,			/*  reading response body into local buffer */
    read_addr,			/*  reading response address/length */
    read_data,			/*  reading response data into address/length */
    read_flush,			/*  flushing resp to cancelled rqst */
    read_done,			/*  do response post-processing */
    read_broken			/*  fd dead or parse error */
} read_status_t;

static volatile read_status_t cds_reader_status;
static cdsFlagStat_t *cds_reader_Flags_p; /* current resp is for this rqst */
static volatile int cds_reader_resp_cnt;	/* bytes left in response */

#define DNS_LOCK	{ pthread_mutex_lock(&cds_send_mutex); }
#define DNS_END_LOCK	{ pthread_mutex_unlock(&cds_send_mutex); }
#define DNS_WAIT(cv)	{ \
    if (pthread_is_multithreaded_np()) { \
        pthread_cleanup_push((void (*)(void *))pthread_mutex_unlock, &cds_send_mutex); \
        pthread_cond_wait(cv, &cds_send_mutex); \
        pthread_cleanup_pop(0); \
        } \
    }

/* 
 * Prototypes.
 */
static pthread_addr_t
cds_reader (
	    void);

static void
cds_reader_cleanup (
		    void *);

static void
cds_pending_cleanup (
		     int );

static void
cds_flags_cleanup (
		   cdsFlagStat_t 	*,
		   int			);


/* The following are protected by cds_send_mutex */
static cdsFlagStat_t *cds_pending_p = NULL;

/*
 * Set up to receive response
 *
 * Requests that can potentially receive responses must be on a list
 * of pending requests even before the request is sent.  A reponse
 * demultiplexor thread must be running to catch a response which
 * could come back right after the last byte of the request is sent.
 *
 * The pending list is actually maintained as a ring, with the last
 * entry pointing to the first, and cds_pending_p pointing to any entry.
 * The link (OPQLINK) will remain non-zero while on the ring.
 *
 * The request is complete when the status is changed from CDS_PENDING
 * and the OPQCND condition variable is goosed.
 */
int 
cds_read_push (cdsFlagStat_t *Flags_p)
{
    int retsts=0;

    
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >cds_read_push"));

    /*
     * Make sure these things fit...
     */
    dce_assert(cds__svc_handle,
	       sizeof(pthread_cond_t) <= 2*sizeof(Flags_p->fsOpqCDS[OPQCND0]));
    dce_assert(cds__svc_handle,
	       OPQCND1 < OPQ_SIZ);
    dce_assert(cds__svc_handle,
	       sizeof(cdsFlagStat_t *) <= sizeof(Flags_p->fsOpqCDS[OPQLINK]));
    dce_assert(cds__svc_handle,OPQLINK < OPQ_SIZ);

    /*
     * cds_read_pop() should avoid us unless we get on the pending ring.
     */
    Flags_p->fsOpqCDS[OPQLINK] = 0;

    /*
     * Make sure we have a condition variable to wait for completion.
     */
    if (pthread_cond_init((pthread_cond_t *)&Flags_p->fsOpqCDS[OPQCND0],
			  pthread_condattr_default) < 0)
    {
	retsts=dns_status_clerk(Flags_p, CDS_NONSRESOURCES);

	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_library,
            svc_c_debug9,
            " <cds_read_push: status = %d",
	    retsts));

        return (retsts);
    }
    

    /*
     * If reader fell over, clear out the mess.
     */
    if (cds_reader_state == reader_rundown)
	cds_unbind();

    DNS_LOCK {
	/*
	 * Place this request on the pending ring
	 */
	if (cds_pending_p) {
	    Flags_p->fsOpqCDS[OPQLINK] = cds_pending_p->fsOpqCDS[OPQLINK];
	    cds_pending_p->fsOpqCDS[OPQLINK] = (unsigned long)Flags_p;
	} else {
	    Flags_p->fsOpqCDS[OPQLINK] = (unsigned long)Flags_p;
	    cds_pending_p = Flags_p;

	    /* make sure we have our condition variable */
	    if (cds_reader_state == reader_init) 
	    {
		if (pthread_cond_init(&cds_reader_state_changed,
				      pthread_condattr_default) < 0)
		{
		    dce_svc_printf(CDS_S_PTHREADCONDINIT_A_MSG, (long)errno);
		}
		cds_reader_state = reader_none;
	    }
	}
    } DNS_END_LOCK;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " <cds_read_push: return(CDS_SUCCESS)"));

    return(CDS_SUCCESS);
}

/*
 * Wait until request has been responded to
 */
void 
cds_read (cdsFlagStat_t *Flags_p)
{    
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >cds_read"));

    DNS_LOCK {
        /*
         * Wait for the request to complete or cancel.
         */
	while (Flags_p->fsLocStat == CDS_PENDING)
	    switch (cds_reader_state) {
	    case reader_none:
	        if (pthread_is_multithreaded_np())
	        {
		    if (0 <= pthread_create(&cds_reader_thread,
					pthread_attr_default, 
					(void * (*)(void *))cds_reader, 
					NULL))

		        cds_reader_state = reader_active;
		    else
		        cds_flags_cleanup(Flags_p, CDS_SOCKET);
                }
	        else
		{
		    DNS_END_LOCK
		    cds_reader();
		    DNS_LOCK
		}
	        break;
	    case reader_active:
		DNS_WAIT((pthread_cond_t *)&Flags_p->fsOpqCDS[OPQCND0]);
		break;
	    case reader_cancelled:
	    case reader_rundown:
	    case reader_restart:
		DNS_WAIT(&cds_reader_state_changed);
		break;
	    default:
		/*
		 * should not happen 
		 */
		dce_assert(cds__svc_handle, 1 == 2);
	    }
    } DNS_END_LOCK;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " <cds_read: @ end"));
}

/*
 * Terminate request
 *
 * If the request terminated abnormally, the read of the response
 * may be in progress.  This requires care to ensure that the read
 * has been terminated before our return.  The only way guaranteed
 * by POSIX to terminate the read is to terminate the read thread.
 * We also must try to start the cds_reader again if the pending queue
 * is still non-empty after we have been removed.
 *
 * We also can't return the condition variable until we know that
 * the cds_reader thread cannot use it.  Thus it cannot be returned
 * if we were cds_reader_Flags_p until we have joined with cds_reader_thread.
 */
void 
cds_read_pop (cdsFlagStat_t *Flags_p)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >cds_read_pop"));

      DNS_LOCK {
	if (Flags_p->fsOpqCDS[OPQLINK]) {
	    /*
	     * Kill the cds_reader thread if it is working on this response
	     */
	    while (Flags_p == cds_reader_Flags_p) {
		if (cds_reader_state == reader_active) {
		    pthread_cancel(cds_reader_thread);
		    cds_reader_state = reader_restart;
		}
		DNS_WAIT((pthread_cond_t *)&Flags_p->fsOpqCDS[OPQCND0]);
	    }
	    /*
	     * Remove this request from the pending ring
	     */
	    if (Flags_p->fsOpqCDS[OPQLINK] == (unsigned long)Flags_p)
		cds_pending_p = NULL;
	    else {
		while (cds_pending_p->fsOpqCDS[OPQLINK] != (unsigned long)Flags_p)
		    cds_pending_p = (cdsFlagStat_t *)cds_pending_p->fsOpqCDS[OPQLINK];

		cds_pending_p->fsOpqCDS[OPQLINK] = Flags_p->fsOpqCDS[OPQLINK];
	    }
	    Flags_p->fsOpqCDS[OPQLINK] = 0;

	    pthread_cond_destroy((pthread_cond_t *)&Flags_p->fsOpqCDS[OPQCND0]);
	}
    } DNS_END_LOCK;

    if (cds_reader_state == reader_restart) {
	pthread_addr_t join_status;

	pthread_join(cds_reader_thread, &join_status);

	DNS_LOCK {
	    /*
	     * If state hasn't changed, we haven't been overridden
	     * by loss of the socket.
	     */
	    if (cds_reader_state == reader_restart) {
		pthread_detach(&cds_reader_thread);
		cds_reader_state = reader_none;
		pthread_cond_broadcast(&cds_reader_state_changed);

		if (0 <= pthread_create(&cds_reader_thread,
					pthread_attr_default, 
					(void * (*)(void *))cds_reader, 
					NULL))
		    cds_reader_state = reader_active;
	    }
	} DNS_END_LOCK;
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " <cds_read_pop"));
}

/*
 * Read requests and notify request threads
 *
 * Use a static state machine so that the thread can be stopped
 * and restarted without losing our parsing state.  If a requestor
 * thread dies, the thread can be killed to stop a read in progress,
 * and yet restarted to flush the response and tackle the next.
 * Otherwise we would have to toss the fd with ALL the outstanding
 * requests.
 *
 * There are two thread cancellation points: one at the common read(),
 * and the other in whatever is in the post-processing.
 * The post-processing could quit in name conversion, as files sometimes
 * have to be consulted.
 */
static pthread_addr_t 
cds_reader (void)
{
    ucp_resp_header header;	/* buffer for header */
    char bufb[LOCAL_READ_SIZE];	/* when data is small */
    ucp_prm_header bf;		/* buffer for address/length parameter stuff */
    register cdsFlagStat_t *Flags_p;
    register char *addr_p;
    register int resp_cnt;
    register int addr_cnt;
    register read_status_t status = cds_reader_status;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >cds_reader"));

    /* reader must have been cleaned up */
    dce_assert(cds__svc_handle,cds_reader_Flags_p == NULL);

    /* local buffer must be at least as large as address/length buffer */
    dce_assert(cds__svc_handle,sizeof(ucp_prm_header) <= sizeof(bufb));

    switch (status) {
    case read_body:
    case read_addr:
    case read_data:
	status = read_flush;
	resp_cnt = cds_reader_resp_cnt;
	break;
    case read_done:
	status = read_header;
	break;
    default:
	break;
    }

    pthread_cleanup_push(cds_reader_cleanup, NULL);

    while (TRUE) {
	register int i;

	switch (status) {
	case read_none:
	    status = read_header;
	case read_header:
	    /*
	     * Set up to read the header.
	     */
	    addr_p = (char *)&header;
	    addr_cnt = sizeof(header);
	    resp_cnt = sizeof(header);
	    cds_reader_resp_cnt = resp_cnt;
	    break;
	case read_body:
	    if (resp_cnt <= sizeof(bufb)) {
		/*
		 * Read body of message into local buffer.
		 */
		addr_p = bufb;
		addr_cnt = resp_cnt;
		break;
	    }
	    status = read_addr;
	case read_addr:
	    /*
	     * Read next address/length into buffer.
	     */
	    addr_p = (char *)&bf;
	    addr_cnt = sizeof(bf);
	    break;
	case read_data:
	    /*
	     * Read data of given length into given address.
	     */
	    addr_p = (char *)EXTaddress(bf.address);
	    addr_cnt = EXT16(bf.length);
	    break;
	case read_flush:
	    /*
	     * Toss as much of message as possible.
	     */
	    if (resp_cnt <= sizeof(bufb))
		addr_cnt = resp_cnt;
	    else
		addr_cnt = sizeof(bufb);

	    addr_p = bufb;
	    break;
	default:
	    addr_cnt = 0;
	    break;
	}

	/*
	 * If there is data to read, read it here.
	 */
	if (0 < addr_cnt) {
	    /* Must be room left in response packet */
	    /* The socket must be still open */
	    if ((addr_cnt <= resp_cnt) && (0 <= cds_clerk_socket)) {
		/* THREAD CANCELLATION POINT, synchronize */
		cds_reader_status = status;
		do {
		    i = read(cds_clerk_socket, addr_p, addr_cnt);
		    if (0 < i) {
			addr_p += i;
			addr_cnt -= i;
			resp_cnt -= i;
			cds_reader_resp_cnt = resp_cnt;
		    } else {
			if (i == -1 && errno == EINTR)
			{
			    continue;
			}
			/* READ FAILURE */
			status = read_broken;
			break;
		    }
		} while (addr_cnt);
	    } else
		status = read_broken;
	}

	switch (status) {
	case read_header:
	    /* length of parameter data to process */
	    resp_cnt = UCP_EXTlongword(header.length)
			- (sizeof(header) - sizeof(header.length));

	    if (0 < resp_cnt) {
		cds_reader_resp_cnt = resp_cnt;
		status = read_body;
	    } else if (resp_cnt == 0)
		status = read_done;
	    else {
		status = read_broken;
		break;
	    }

	    /*
	     * Find the request if still there
	     */
	    DNS_LOCK {
		if (Flags_p = (cdsFlagStat_t *)cds_pending_p)
		    while (TRUE) {
			if (Flags_p->fsOpqCDS[OPQSEQ]
				== UCP_EXTlongword(header.sequence)) {
			    cds_reader_Flags_p = Flags_p;
			    break;
			}

			Flags_p = (cdsFlagStat_t *)Flags_p->fsOpqCDS[OPQLINK];

			if (Flags_p != cds_pending_p)
			    continue;

			/* completed ring traversal and not found */
			status = read_flush;
			break;
		    }
		else
		    status = read_flush;

	    } DNS_END_LOCK;

	    continue;
	case read_body:
	    {
	    register char *buf_p = bufb;
	    /*
	     * Now parse the response into user buffers
	     */
	    while (buf_p < addr_p) {
		register char *adr_p;
		register int len;

		adr_p = (char *)GETaddress(buf_p);
		len = GET16(buf_p);
		COPY_bytes(buf_p, adr_p, len);
		buf_p += len;
	    }
	    }
	    status = read_done;
	    continue;
	case read_addr:
	    status = read_data;
	    continue;
	case read_data:
	    if (0 < resp_cnt)
		status = read_addr;
	    else
		status = read_done;

	    continue;
	case read_flush:
	    if (resp_cnt == 0)
		status = read_header;

	    continue;
	case read_done:
	    /*
	     * If remote operation was successful
	     * then do post-processing.
	     */
	    if (dns_status_clerk(Flags_p, UCP_EXTlongword(header.status))
			== CDS_SUCCESS) {
		dns_ReadHandle_t *const rh_p =
			    (dns_ReadHandle_t *)UCP_EXTaddress(header.handle);

		if (rh_p) {
		    /* Save values */
		    rh_p->dns_outLinked = Flags_p->fsOutLink;
		    rh_p->dns_fromCache = Flags_p->fsFromCache;
		    rh_p->dns_status = Flags_p->fsLocStat;

		    /* THREAD CANCELLATION POINT, synchronize */
		    cds_reader_status = status;

		    (*rh_p->dns_parse)(Flags_p, (cds_handle_t)rh_p);
		}
	    }

	    DNS_LOCK {
		cds_reader_Flags_p = NULL;
	    } DNS_END_LOCK;
            
	    if (pthread_is_multithreaded_np())
	    {
	        pthread_cond_signal((pthread_cond_t *)
				    &Flags_p->fsOpqCDS[OPQCND0]);
	        status = read_header;
	        continue;
            }
	    else
	    {
		cds_reader_status = read_done;
		goto reader_done;
	    }

	default:
	    /*
	     * Failure - close the socket.
	     * This code is duplicated in cds_unbind().
	     * What is the elegant way to avoid this duplication?
	     */
            
	    if (pthread_is_multithreaded_np())
		cds_reader_state = reader_rundown;
	    close(cds_clerk_socket);
	    cds_clerk_socket = -1;
	    cds_pending_cleanup(CDS_SOCKET);
	    status = read_none;
	    break;
	}
	break;
    }
reader_done:
    pthread_cleanup_pop(0);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " <cds_reader: return(NULL)"));

    return(NULL);
}

/*
 * Clean up if the reader is cancelled
 */
static void 
cds_reader_cleanup (void *arg)
{
   DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >cds_reader_cleanup()"));

    DNS_LOCK {
	register cdsFlagStat_t *Flags_p;

	if (Flags_p = (cdsFlagStat_t *)cds_reader_Flags_p)
	    cds_flags_cleanup(Flags_p, CDS_SOCKET);

    } DNS_END_LOCK;

   DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " <cds_reader_cleanup()"));
}

/*
 * Terminate any requests waiting for responses
 */
static void 
cds_pending_cleanup (int status)
{
    register cdsFlagStat_t *Flags_p;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >cds_pending_cleanup"));

    DNS_LOCK {
	/*
	 * Mark all pending requests for death.
	 */
	if (Flags_p = (cdsFlagStat_t *)cds_pending_p)
	    do {
		cds_flags_cleanup(Flags_p, status);
		Flags_p = (cdsFlagStat_t *)Flags_p->fsOpqCDS[OPQLINK];
	    } while (Flags_p != cds_pending_p);

    } DNS_END_LOCK;

   DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " <cds_pending_cleanup"));
}

/*
 * Terminate a request. MUTEX must be locked.
 */
static void 
cds_flags_cleanup (cdsFlagStat_t  *Flags_p,
                   int            status)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >cds_flags_cleanup"));

    if (Flags_p == cds_reader_Flags_p)
	cds_reader_Flags_p = NULL;

    if (Flags_p->fsLocStat == CDS_PENDING) {
	dns_status_clerk(Flags_p, status);
	pthread_cond_signal(
	    (pthread_cond_t *)&Flags_p->fsOpqCDS[OPQCND0]);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " <cds_flags_cleanup"));
}

/*
 * Socket has died - reset our state machine
 */
int 
cds_unbind (void)
{
    pthread_addr_t join_status;
    register int old_state;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >cds_unbind"));

    DNS_LOCK {
	switch (old_state = cds_reader_state) {
	case reader_active:
	    pthread_cancel(cds_reader_thread);
	case reader_restart:
	case reader_rundown:
	    cds_reader_state = reader_cancelled;
	}
    } DNS_END_LOCK;

    switch (old_state) {
    case reader_active:
    case reader_restart:
    case reader_rundown:
	/*
	 * None of the following has to be done while locked
	 * as long as it is done in this order.
	 */
	pthread_join(cds_reader_thread, &join_status); /* The death watch */
	pthread_detach(&cds_reader_thread);	/* Give thread absolution */
	close(cds_clerk_socket);		/* Cause of death */
	cds_clerk_socket = -1;
	cds_pending_cleanup(CDS_SOCKET);	/* Died without estate */
	cds_reader_status = read_none;		/* Reset reader state */
	cds_reader_state = reader_none;		/* No reader */
	pthread_cond_broadcast(&cds_reader_state_changed); /* Post in obits */
	break;
    default:
	DNS_LOCK {
	    while (cds_reader_state == reader_cancelled)
		DNS_WAIT(&cds_reader_state_changed);
	} DNS_END_LOCK;
	break;
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " <cds_unbind: return(CDS_SUCCESS)"));

    return(CDS_SUCCESS);
}
