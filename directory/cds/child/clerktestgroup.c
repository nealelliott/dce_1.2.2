/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: clerktestgroup.c,v $
 * Revision 1.1.8.2  1996/02/18  19:26:58  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:09:39  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:05:36  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:38:52  root]
 * 
 * Revision 1.1.6.4  1994/08/03  18:59:54  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:16:06  mccann]
 * 
 * Revision 1.1.6.3  1994/06/23  18:29:18  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:56:22  mccann]
 * 
 * Revision 1.1.6.2  1994/06/09  16:08:06  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:52:52  devsrc]
 * 
 * Revision 1.1.6.1  1994/03/12  21:57:07  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:54:11  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  12:43:05  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:29:14  zeliff]
 * 
 * Revision 1.1.2.2  1992/03/22  21:33:42  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:10:30  weisman]
 * 
 * Revision 1.1  1992/01/19  15:27:09  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module clerktestgroup.c
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
 *	Clerk.TestGroup command processor.
 *
 * Networks & Communications Software Engineering
 */

#include <dce/dce.h>
#include <dce/dcecdsmsg.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <uc_child.h>
#include <uc_clerk.h>
#include <cache.h>


/*
 * TestGroup arguments (passed in the thread block)
 */
#define	GROUP_P		clerkArgs[0]
#define	MEMBER_P	clerkArgs[1]
#define	INOUTDIRECT_P	clerkArgs[2]
#define	OUTTIMEOUT_P	clerkArgs[3]
#define	LOOPDETECT_P	clerkArgs[4]
#define	RET_P		clerkArgs[5]

/*
 * Local Prototype declarations.
 */

static int 
TestGroupRPC (thread_t  *,
              Set_t     *,
              ObjUID_t  *);

int 
buildTestGroupRequest (thread_t *);

static void 
parseTestGroupResponse (thread_t *);

int
clerkTestGroupCache (thread_t  *,
                     int       *);

void 
clerkTestGroup (thread_t *);



/*
 *
 * Routine to call RPC stub for TestGroup Request
 *
 */
static int 
TestGroupRPC (thread_t  *th_p,
              Set_t     *tower_p,
              ObjUID_t  *uuid_p)
{
    rpc_binding_handle_t  h;
    int i;
    cds_Timeout_t *Timeout_p = (cds_Timeout_t *)NULL;
    ObjUID_t loopDetect;
    cds_FullName_t  member;
    cds_Progress_t Progress;
    error_status_t rpcStatus,commStatus = rpc_s_no_bindings;
    cds_status_t  dnsStatus;
    byte_t    result,direct;
    int fromcache, getout=0;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " >TestGroupRPC"));

    /* Set up structures for calling RPC */
    Build_R_Progress((byte_t *)th_p->taProgress.flags,
		     (Timeout_t *)th_p->taProgress.Timeout,
		     th_p->taProgress.Unresolved,
		     th_p->taProgress.Resolved,
		     &Progress);
    L_to_R_FullName((FullName_t *)th_p->MEMBER_P, &member);
    COPY_ObjUID(th_p->LOOPDETECT_P, &loopDetect);
    direct = EXT8(th_p->INOUTDIRECT_P);

    /*
     * First attempt to find a cached handle. There
     * is a use count associated with handles, so make
     * sure either free or release cached handle is called
     */
    if (fromcache = get_cached_handle(&h, uuid_p, th_p))
      commStatus = cds_TestGroup(h, &Progress,
				 &member, &loopDetect,
				 &direct, &result,
				 &Timeout_p, &dnsStatus);

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

	    commStatus = cds_TestGroup(h, &Progress,
				       &member, &loopDetect,
				       &direct, &result,
				       &Timeout_p, &dnsStatus);
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
	    th_p->taRspMsgPtr_p = R_to_L_Progress(&Progress,
				      (Progress_t *)th_p->taRspMsgPtr_p, TRUE);
	    if (FLAG_IS_SET(&Progress.pr_flags, PR_done)) {
		R_to_L_Timeout(Timeout_p,
			       (Timeout_t *)th_p->taRspMsgPtr_p);
		R_Timeout_Free(Timeout_p);
		th_p->taRspMsgPtr_p = SKIP_Timeout(th_p->taRspMsgPtr_p);
		
		PUT8(th_p->taRspMsgPtr_p, direct);
		COPY_ObjUID(&loopDetect, th_p->taRspMsgPtr_p);
		th_p->taRspMsgPtr_p = SKIP_ObjUID(th_p->taRspMsgPtr_p);
		PUT8(th_p->taRspMsgPtr_p, result);
	    }
	} else {
	    th_p->taRspType = MT_Exception;
	    th_p->taRspMsgPtr_p = R_to_L_Exception(&dnsStatus,
						   th_p->taRspMsgPtr_p, TRUE);
	}
	R_Set_Free(Progress.pr_replicas_p);
	CACHE_HANDLE(fromcache, h, th_p);

	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_child,
            svc_c_debug4,
            " <TestGroupRPC: return(DNS_SUCCESS)"));

	return(DNS_SUCCESS);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " <TestGroupRPC: return (DNS_NOCOMMUNICATION)"));

    return (DNS_NOCOMMUNICATION);
}

/*
 *	b u i l d T e s t G r o u p R e q u e s t
 *
 * Routine to build an TestGroup request message and
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
buildTestGroupRequest (thread_t *th_p)
{
    int length = 0;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >buildTestGroupRequest"));

    /*
     * TestGroupRequest message:
     *  progress: Progress
     *	member: FullName
     *	loopDetect: ObjUID
     *	direct: BOOLEAN
     */
    length += buildProgress(th_p);

    if (th_p->taReqMsgEnd_p) {
	COPY_FullName(th_p->MEMBER_P, th_p->taReqMsgEnd_p);
	th_p->taReqMsgEnd_p = SKIP_FullName(th_p->taReqMsgEnd_p);
    }
    length += LEN_FullName(th_p->MEMBER_P);

    if (th_p->taReqMsgEnd_p) {
	COPY_ObjUID(th_p->LOOPDETECT_P, th_p->taReqMsgEnd_p);
	th_p->taReqMsgEnd_p = SKIP_ObjUID(th_p->taReqMsgEnd_p);
    }
    length += LEN_ObjUID(th_p->LOOPDETECT_P);

    if (th_p->taReqMsgEnd_p) {
	COPY_BOOLEAN(th_p->INOUTDIRECT_P, th_p->taReqMsgEnd_p);
	th_p->taReqMsgEnd_p = SKIP_BOOLEAN(th_p->taReqMsgEnd_p);
    }
    length += LEN_BOOLEAN(th_p->INOUTDIRECT_P);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <buildTestGroupRequest: return(length=%d)",
	length));

    return(length);
}

/*
 *	p a r s e T e s t G r o u p R e s p o n s e
 *
 * This local routine parses the response message.
 */
static void 
parseTestGroupResponse (thread_t *th_p)
{
    FullName_t *groupname_p;
    Timeout_t *timeout_p;
    byte_t *inout_p;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >parseTestGroupResponse "));

    /*
     * TestGroupResponse message:
     *  progress: Progress
     *	timeout: Timeout
     *	direct: BOOLEAN
     *	loopDetect: ObjUID
     *	result: BOOLEAN
     */
    ta_begin(th_p);
    ta_Progress(th_p);
    ta_Timeout(th_p, th_p->OUTTIMEOUT_P);
    ta_BOOLEAN(th_p, th_p->INOUTDIRECT_P);
    ta_ObjUID(th_p, th_p->LOOPDETECT_P);
    if (th_p->taRspMsgPtr_p && (*th_p->taRspMsgPtr_p == 0)) {
      th_p->clerkStatus = DNS_NOTFND;
    } else {
      th_p->clerkStatus = DNS_SUCCESS;
    }
    ta_BOOLEAN(th_p, th_p->RET_P);
    ta_end(th_p);

    /*
     * If don't cache is false, and the result is TRUE, cache the results,.
     * If the result is FALSE, it doesn't make sense to cache the entry
     * since inoutdirect and grouprevoke are meaningless.  The server
     * doesn't return them filled in in this cache.
     * Progress record at taRspMsg_p.
     * Extract group name from progess record, this way we
     * know there are no softlinks in the name.
     */
    if ((th_p->clerkStatus == DNS_SUCCESS) &&
	(!th_p->clerkDontCache) && (th_p->taRspMsg_p)) {
      Progress_t *const pr_p = (Progress_t *)th_p->taRspMsg_p;

      groupname_p = (FullName_t *)SKIP_FullName(pr_p->pr_unresolvedName);
      timeout_p = (Timeout_t *)SKIP_Progress(th_p->taRspMsg_p);
      inout_p = SKIP_Timeout(timeout_p);
      CAInsertGroup(th_p->user, groupname_p,
		    (FullName_t *)th_p->MEMBER_P, *inout_p, timeout_p);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <parseTestGroupResponse"));
}

/*
 * Test the group/membership cache to see if this request can
 * be satisfied locally.
 *
 * Inputs:
 *	GROUP_P		FullName of group to be checked
 *	MEMBER_P	FullName of member to check for
 *	INOUTDIRECT_P	BOOLEAN
 *	LOOPDETECT_P	ObjUID (optional)
 *
 * Outputs:
 *	INOUTDIRECT_P	BOOLEAN
 *	OUTTIMEOUT_P	Timeout
 *	LOOPDETECT_P	ObjUID
 *
 * Returns:
 *	RET_P		BOOLEAN
 */
int
clerkTestGroupCache (thread_t  *th_p,
                     int       *stale_p)
{
    byte_t inoutdirect;
    byte_t result;
    Timeout_t timeout;
    ObjUID_t loopDetect;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug5,
        " >clerkTestGroupCache"));

    /*
     * Only query cache if confidence is low
     */
    if (th_p->clerkConf != CO_low) 
    {
	DCE_SVC_DEBUG((
           cds__svc_handle,
           cds_svc_child,
           svc_c_debug5,
           " <clerkTestGroupCache: (1) return(FALSE)"));

	return(FALSE);
    }
    

    /*
     * Query cache.   Returns false if cannot find group/member
     * in cache
     */
    if (CATestGroup(th_p->user, th_p->clerkCacheTimeout,
		    (FullName_t *)th_p->GROUP_P,
		    (FullName_t *)th_p->MEMBER_P, EXT8(th_p->INOUTDIRECT_P),
		    &inoutdirect, &timeout, stale_p, &th_p->outLinked) == FALSE)
    {
	DCE_SVC_DEBUG((
           cds__svc_handle,
           cds_svc_child,
           svc_c_debug5,
           " <clerkTestGroupCache: (2) return(FALSE)"));

	return(FALSE);
    }
    

    /*
     * Cache satisfied request.  Build return buffer
     *	timeout: Timeout
     *	direct: BOOLEAN
     *	loopDetect: ObjUID
     *	result: BOOLEAN
     */

    th_p->clerkFromCache++;
    prm_begin(th_p);
    prm_out_Timeout(th_p, th_p->OUTTIMEOUT_P, (byte_t *)&timeout);
    prm_out_BOOLEAN(th_p, th_p->INOUTDIRECT_P, &inoutdirect);


    ZERO_ObjUID(&loopDetect);
    prm_copy(th_p, th_p->LOOPDETECT_P, (byte_t *)&loopDetect,
	     sizeof(ObjUID_t));

    result = BL_true;
    prm_out_BOOLEAN(th_p, th_p->RET_P, &result);
    th_p->clerkStatus = DNS_SUCCESS;
    prm_end(th_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug5,
        " <clerkTestGroupCache: return(TRUE)"));

    return(TRUE);
}

/*
 *	c l e r k T e s t G r o u p
 *
 * This routine tests the value of the stored attribute for a match with
 * the specified value
 *
 * Inputs:
 *	GROUP_P		FullName of group to be checked
 *	MEMBER_P	FullName of member to check for
 *	INOUTDIRECT_P	BOOLEAN
 *	LOOPDETECT_P	ObjUID (optional)
 *
 * Outputs:
 *	INOUTDIRECT_P	BOOLEAN
 *	OUTTIMEOUT_P	Timeout
 *	LOOPDETECT_P	ObjUID
 *
 * Returns:
 *	RET_P		BOOLEAN
 */
void 
clerkTestGroup (thread_t *th_p)
{
    DEB_ASCII_BUF_FullName(groupBuf)
    DEB_ASCII_BUF_FullName(memberBuf)
    DEB_ASCII_BUF_BOOLEAN(inoutDirectBuf)
    DEB_ASCII_BUF_ObjUID(loopDetectBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    int stale = 0;
    int status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " >clerkTestGroup(%s,%s,%s,%s,%p,%p)",
        deb_ascii_FullName(groupBuf,th_p->GROUP_P),
        deb_ascii_FullName(memberBuf,th_p->MEMBER_P),
        deb_ascii_BOOLEAN(inoutDirectBuf,th_p->INOUTDIRECT_P),
        deb_ascii_ObjUID(loopDetectBuf,th_p->LOOPDETECT_P),
        th_p->OUTTIMEOUT_P,
        th_p->RET_P));

    /*
     * Before calling the server, attempt to satisfy this request
     * locally in the user cache.
     */
    if (clerkTestGroupCache(th_p, &stale) == FALSE) {
      th_p->taBuildReq = buildTestGroupRequest;
      th_p->taParseRsp = parseTestGroupResponse;
      th_p->taRPC = TestGroupRPC;

      sendToWalkTree(th_p, th_p->GROUP_P);

      if (th_p->clerkStatus == DNS_TIMEOUTMAYBEDONE)
	th_p->clerkStatus = DNS_TIMEOUTNOTDONE;
      /*
       * If previously found data, but it was stale use it
       * now if there was a communication error
       */
      status = cds_convert_rpc_status(th_p->clerkStatus);
      if ((stale) && ((status == DNS_NOCOMMUNICATION) ||
		      (status == DNS_TIMEOUTNOTDONE) ||
		      (status == DNS_CLEARINGHOUSEDOWN) ||
		      (status == DNS_DATACORRUPTION) ||
		      (status == DNS_UNKNOWNCLEARINGHOUSE))) {
	/* check cache again an take whatever is there */
	th_p->clerkCacheTimeout = 0;
	if (clerkTestGroupCache(th_p, &stale) == TRUE) {
	  th_p->clerkStatus = DNS_SUCCESS_STALE;
	}
      }
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " <clerkTestGroup(%s,%s,%s,%s,%p,%p) status(%s)",
        deb_ascii_FullName(groupBuf,th_p->GROUP_P),
        deb_ascii_FullName(memberBuf,th_p->MEMBER_P),
        deb_ascii_BOOLEAN(inoutDirectBuf,th_p->INOUTDIRECT_P),
        deb_ascii_ObjUID(loopDetectBuf,th_p->LOOPDETECT_P),
        th_p->OUTTIMEOUT_P,
        th_p->RET_P,
        deb_ascii_dns_status(statusBuf,th_p->clerkStatus)));
}
