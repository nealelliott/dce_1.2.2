/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: nsclerkcombine.c,v $
 * Revision 1.1.8.2  1996/02/18  19:27:03  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:09:43  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:05:43  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:38:57  root]
 * 
 * Revision 1.1.6.4  1994/08/03  18:59:55  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:16:01  mccann]
 * 
 * Revision 1.1.6.3  1994/06/23  18:29:20  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:56:30  mccann]
 * 
 * Revision 1.1.6.2  1994/06/09  16:08:08  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:52:53  devsrc]
 * 
 * Revision 1.1.6.1  1994/03/12  21:57:12  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:54:24  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  12:43:15  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:29:26  zeliff]
 * 
 * Revision 1.1.2.2  1992/03/22  21:34:16  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:10:40  weisman]
 * 
 * Revision 1.1  1992/01/19  15:27:12  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module nsclerkcombine.c
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
 * MODULE DESCRIPTION:
 *
 *	NSClerk.Combine command processor.
 *
 * Networks & Communications Software Engineering
 */

#include <dce/dce.h>
#include <dce/dcecdsmsg.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <uc_child.h>
#include <uc_clerk.h>

/*
 * Combine arguments (passed in the thread block)
 */
#define	DIRECTORY_P	clerkArgs[0]
#define	FROM_P		clerkArgs[1]
#define	TO_P		clerkArgs[2]
#define	SKULKTIME_P	clerkArgs[3]
#define	ALLUPTO_P	clerkArgs[4]
#define	EPOCH_P		clerkArgs[5]
#define	RET_P		clerkArgs[6]

/*
 * Local Prototype declarations.
 */

int 
CombineRPC (thread_t  *,
            Set_t     *,
            ObjUID_t  *);

int 
buildCombineRequest (thread_t *);

static void 
parseCombineResponse (thread_t *);

void 
nsclerkCombine (thread_t *);



/*
 * Routine to call RPC stub for Combine request
 */
int 
CombineRPC (thread_t  *th_p,
            Set_t     *tower_p,
            ObjUID_t  *uuid_p)
{
    rpc_binding_handle_t  h;
    int i;
    ObjUID_t directory, epoch, ring;
    cds_Timestamp_t skulktime, allUpTo;
    cds_FullName_t to;
    error_status_t rpcStatus,commStatus = rpc_s_no_bindings;
    cds_status_t  dnsStatus;
    int fromcache, getout = 0;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " >CombineRPC"));

    /* Align uuid */
    COPY_ObjUID(th_p->DIRECTORY_P, &directory);
    COPY_ObjUID(th_p->EPOCH_P, &epoch);

    /* Set up structures for calling RPC */
    L_to_R_Timestamp((Timestamp_t *)th_p->SKULKTIME_P, &skulktime);
    L_to_R_Timestamp((Timestamp_t *)th_p->ALLUPTO_P, &allUpTo);
    L_to_R_FullName((FullName_t *)th_p->TO_P, &to);

    /* 
     * First attempt to find a cached handle. There
     * is a use count associated with handles, so make
     * sure either free or release cached handle is called 
     */
    if (fromcache = get_cached_handle(&h, uuid_p, th_p))
      commStatus = cds_Combine(h, &directory, &skulktime, 
			       &allUpTo, &epoch, &to, &ring,
			       &dnsStatus);
    
    if (commStatus != rpc_s_ok) {
	if (fromcache) {
	    free_cached_handle(th_p, h);
	    fromcache = 0;
	}
	
	for (i=0; i<NUM_Set(tower_p); ++i) {
	    if (!get_rpc_handle(&h, (MemberValue_t *)INDEX_Set(tower_p, i), 
				uuid_p, th_p, &getout))
		if (getout) break;
		else continue;
	    commStatus = cds_Combine(h, &directory, &skulktime, 
				     &allUpTo, &epoch, &to, &ring,
				     &dnsStatus);
	    if (commStatus == rpc_s_ok) break;
	    maximize_rpc_error(th_p, commStatus, &getout);
	    rpc_binding_free(&h, &rpcStatus);
	    if (getout) break;
	} /* End For loop */
    }

    if (commStatus == rpc_s_ok) {
	th_p->taRspMsgPtr_p = th_p->taRspMsg_p;
	if (dnsStatus.er_status == PDU_Success) {
	    th_p->taRspType = MT_Response;
	    COPY_ObjUID(&ring, th_p->taRspMsgPtr_p);
	    th_p->taRspMsgPtr_p = SKIP_ObjUID(th_p->taRspMsgPtr_p);
	} else {
	    th_p->taRspType = MT_Exception;
	    th_p->taRspMsgPtr_p = R_to_L_Exception(&dnsStatus, 
						   th_p->taRspMsgPtr_p, TRUE);
	}
	CACHE_HANDLE(fromcache, h, th_p);

	DCE_SVC_DEBUG((
           cds__svc_handle,
           cds_svc_child,
           svc_c_debug4,
           " <CombineRPC: return(DNS_SUCCESS)"));

	return(DNS_SUCCESS);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " <CombineRPC: return(DNS_NOCOMMUNICATION)"));

    return(DNS_NOCOMMUNICATION);
}

/*
 *	b u i l d C o m b i n e R e q u e s t
 *
 * Routine to build an Combine request message and
 * return length of data
 *
 * Inputs:
 *	th_p->taReqMsgEnd_p	buffer to receive message arguments
 *				if zero, only figure length.
 *
 * Returns:
 *	total length of message arguments.
 */
int 
buildCombineRequest (thread_t *th_p)
{
    int length = 0;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >buildCombineRequest"));

    /*
     * CombineRequest record:
     *	directory: ObjUID
     *	skulktime: Timestamp
     *	allUpTo: Timestamp
     *	epoch: ObjUID
     *	to: FullName
     */
    if (th_p->taReqMsgEnd_p) {
	COPY_ObjUID(th_p->DIRECTORY_P, th_p->taReqMsgEnd_p);
	th_p->taReqMsgEnd_p = SKIP_ObjUID(th_p->taReqMsgEnd_p);
    }
    length += LEN_ObjUID(th_p->DIRECTORY_P);

    if (th_p->taReqMsgEnd_p) {
	COPY_Timestamp(th_p->SKULKTIME_P, th_p->taReqMsgEnd_p);
	th_p->taReqMsgEnd_p = SKIP_Timestamp(th_p->taReqMsgEnd_p);
    }
    length += LEN_Timestamp(th_p->SKULKTIME_P);

    if (th_p->taReqMsgEnd_p) {
	COPY_Timestamp(th_p->ALLUPTO_P, th_p->taReqMsgEnd_p);
	th_p->taReqMsgEnd_p = SKIP_Timestamp(th_p->taReqMsgEnd_p);
    }
    length += LEN_Timestamp(th_p->ALLUPTO_P);

    if (th_p->taReqMsgEnd_p) {
	COPY_ObjUID(th_p->EPOCH_P, th_p->taReqMsgEnd_p);
	th_p->taReqMsgEnd_p = SKIP_ObjUID(th_p->taReqMsgEnd_p);
    }
    length += LEN_ObjUID(th_p->EPOCH_P);

    if (th_p->taReqMsgEnd_p) {
	COPY_FullName(th_p->TO_P, th_p->taReqMsgEnd_p);
	th_p->taReqMsgEnd_p = SKIP_FullName(th_p->taReqMsgEnd_p);
    }
    length += LEN_FullName(th_p->TO_P);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <buildCombineRequest: return(length=%d)",
        length));        

    return(length);
}

/*
 *	p a r s e C o m b i n e R e s p o n s e
 *
 * This local routine parses the response message.
 */
static void 
parseCombineResponse (thread_t *th_p)
{ 
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >parseCombineResponse"));

    /*
     * CombineResponse record:
     *	nextRing: ObjUID
     */
    ta_begin(th_p);
    ta_ObjUID(th_p, th_p->RET_P);
    ta_end(th_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <parseCombineResponse"));
}

/*
 *	n s c l e r k C o m b i n e
 *
 * Combine the contents of one replica with another
 *
 * Inputs:
 *	DIRECTORY_P	ObjUID combine for this directory
 *	FROM_P		ReplicaPointer copy updated from this CH
 *	TO_P		FullName to this CH
 *	SKULKTIME_P	Timestamp of active skulk
 *	ALLUPTO_P	Timestamp of local replica
 *	EPOCH_P		ObjUID of current epoch
 *
 * Returns:
 *	RET_P		ObjUID DNS$RingPointer of copy combined
 */
void 
nsclerkCombine (thread_t *th_p)
{
    DEB_ASCII_BUF_ObjUID(directoryBuf)
    DEB_ASCII_BUF_ReplicaPointer(fromBuf)
    DEB_ASCII_BUF_FullName(toBuf)
    DEB_ASCII_BUF_Timestamp(skulktimeBuf)
    DEB_ASCII_BUF_Timestamp(allUpToBuf)
    DEB_ASCII_BUF_ObjUID(epochBuf)
    DEB_ASCII_BUF_ObjUID(retBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " >nsclerkCombine(%s,%s,%s,%s,%s,%s,)",
        deb_ascii_ObjUID(directoryBuf,th_p->DIRECTORY_P),
        deb_ascii_ReplicaPointer(fromBuf,th_p->FROM_P),
        deb_ascii_FullName(toBuf,th_p->TO_P),
        deb_ascii_Timestamp(skulktimeBuf,th_p->SKULKTIME_P),
        deb_ascii_Timestamp(allUpToBuf,th_p->ALLUPTO_P),
        deb_ascii_ObjUID(epochBuf,th_p->EPOCH_P)));

    th_p->taBuildReq = buildCombineRequest;
    th_p->taParseRsp = parseCombineResponse;
    th_p->taRPC = CombineRPC;

    sendToReplica(th_p, (ReplicaPointer_t *)th_p->FROM_P);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " <nsclerkCombine(%s,%s,%s,%s,%s,%s,%s) status(%s)",
        deb_ascii_ObjUID(directoryBuf,th_p->DIRECTORY_P),
        deb_ascii_ReplicaPointer(fromBuf,th_p->FROM_P),
        deb_ascii_FullName(toBuf,th_p->TO_P),
        deb_ascii_Timestamp(skulktimeBuf,th_p->SKULKTIME_P),
        deb_ascii_Timestamp(allUpToBuf,th_p->ALLUPTO_P),
        deb_ascii_ObjUID(epochBuf,th_p->EPOCH_P),
        deb_ascii_ObjUID(retBuf,th_p->RET_P),
        deb_ascii_dns_status(statusBuf,th_p->clerkStatus)));
}
