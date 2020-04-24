/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: clerkcreatelink.c,v $
 * Revision 1.1.8.2  1996/02/18  19:26:08  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:09:17  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:04:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:38:17  root]
 * 
 * Revision 1.1.6.4  1994/08/03  18:59:33  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:14:05  mccann]
 * 
 * Revision 1.1.6.3  1994/06/23  18:29:00  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:54:54  mccann]
 * 
 * Revision 1.1.6.2  1994/06/09  16:07:47  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:52:30  devsrc]
 * 
 * Revision 1.1.6.1  1994/03/12  21:56:36  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:52:00  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  12:41:39  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:27:22  zeliff]
 * 
 * Revision 1.1.2.3  1992/05/11  21:02:44  grober
 * 	Modified routine CreateLinkRPC to call "continue" rather than
 * 	"break" if get_rpc_handle fails.  This was causing creation
 * 	of softlinks to fail when the first auth protocol tried was
 * 	tcp .
 * 	[1992/05/11  20:45:57  grober]
 * 
 * Revision 1.1.2.2  1992/03/22  21:24:02  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:08:41  weisman]
 * 
 * Revision 1.1  1992/01/19  15:27:34  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module clerkcreatelink.c
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
 *	Clerk.CreateLink command processor.
 *
 * Networks & Communications Software Engineering
 *
 */

#include <dce/dce.h>
#include <dce/dcecdsmsg.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <uc_child.h>
#include <uc_clerk.h>

/*
 * CreateLink arguments (passed in the thread block)
 */
#define	LINKNAME_P	clerkArgs[0]
#define	TARGETNAME_P	clerkArgs[1]
#define	TIMEOUT_P	clerkArgs[2]
#define	RET_P		clerkArgs[3]

/*
 * Local Prototype declarations.
 */

static int 
CreateLinkRPC (thread_t  *,
               Set_t     *,
               ObjUID_t  *);

int 
buildCreateLinkRequest (thread_t *);

static void 
parseCreateLinkResponse (thread_t *);

void 
clerkCreateLink (thread_t *);


/*
 * Routine to call RPC stub for CreateLink request
 */
static int 
CreateLinkRPC (thread_t  *th_p,
               Set_t     *tower_p,
               ObjUID_t  *uuid_p)
{
    rpc_binding_handle_t  h;
    int i;
    cds_Timestamp_t actual_id;
    cds_FullName_t targetname;
    cds_Progress_t Progress;
    cds_Timeout_t  *TIMEOUT_P;
    error_status_t rpcStatus,commStatus = rpc_s_no_bindings;
    cds_status_t  dnsStatus;
    int           fromcache, getout = 0;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " >CreateLinkRPC"));

    /* Set up structures for calling RPC */
    Build_R_Progress((byte_t *)th_p->taProgress.flags,
		     (Timeout_t *)th_p->taProgress.Timeout,
		     th_p->taProgress.Unresolved,
		     th_p->taProgress.Resolved,
		     &Progress);
    L_to_R_FullName((FullName_t *)th_p->TARGETNAME_P, &targetname);
    if (prm_in_length(th_p->TIMEOUT_P) == 0)
      TIMEOUT_P = (cds_Timeout_t *)NULL;
    else
      TIMEOUT_P = (cds_Timeout_t *)th_p->TIMEOUT_P;

    /*
     * First attempt to find a cached handle. There
     * is a use count associated with handles, so make
     * sure either free or release cached handle is called
     */
    if (fromcache = get_cached_handle(&h, uuid_p, th_p))
      commStatus = cds_CreateSoftLink(h, &Progress,
					&targetname, TIMEOUT_P, &actual_id,
					&dnsStatus);

    if (commStatus != rpc_s_ok) {
	if (fromcache) {
	    free_cached_handle(th_p, h);
	    fromcache = 0;
	}

	for (i=0; i<NUM_Set(tower_p); ++i) {
	    if (!get_rpc_handle(&h,
				(MemberValue_t *)INDEX_Set(tower_p, i),
				uuid_p, th_p, &getout))
		if (getout) break;
		else continue;
	    commStatus = cds_CreateSoftLink(h, &Progress,
					    &targetname, TIMEOUT_P,
					    &actual_id,  &dnsStatus);
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
	    if (FLAG_IS_SET(&Progress.pr_flags, PR_done))
	      th_p->taRspMsgPtr_p = R_to_L_Timestamp(&actual_id,
				     (Timestamp_t *)th_p->taRspMsgPtr_p, TRUE);
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
           " <CreateLinkRPC: return(DNS_SUCCESS)"));

	return(DNS_SUCCESS);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " <CreateLinkRPC: return(DNS_NOCOMMUNICATION)"));

    return(DNS_NOCOMMUNICATION);
}

/*
 *      b u i l d C r e a t e L i n k R e q u e s t
 *
 * Routine to build a CreateLink request message and
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
buildCreateLinkRequest (thread_t *th_p)
{
    int length = 0;

    /*
     * CreateLinkRequest message:
     *	progress: Progress
     *  linkTarget: FullName
     *	linkTimeout: Timeout
     */


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >buildCreateLinkRequest"));

    length += buildProgress(th_p);

    if (th_p->taReqMsgEnd_p) {
	COPY_FullName(th_p->TARGETNAME_P, th_p->taReqMsgEnd_p);
	th_p->taReqMsgEnd_p = SKIP_FullName(th_p->taReqMsgEnd_p);
    }
    length += LEN_FullName(th_p->TARGETNAME_P);

    if (th_p->taReqMsgEnd_p) {
	COPY_Timeout(th_p->TIMEOUT_P, th_p->taReqMsgEnd_p);
	th_p->taReqMsgEnd_p = SKIP_Timeout(th_p->taReqMsgEnd_p);
    }
    length += LEN_Timeout(th_p->TIMEOUT_P);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <buildCreateLinkRequest: return(length=%d)",
	length));

    return(length);
}


/*
 *      p a r s e C r e a t e L i n k R e s p o n s e
 *
 * This local routine parses the response message.
 */
static void 
parseCreateLinkResponse (thread_t *th_p)
{
    /*
     * CreateLinkResponse message:
     *	progress: Progress
     *  actualID: Timestamp
     */

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >parseCreateLinkResponse"));

    ta_begin(th_p);
    ta_Progress(th_p);
    ta_Timestamp(th_p, th_p->RET_P);
    ta_end(th_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <parseCreateLinkResponse"));
}

/*
 *	c l e r k C r e a t e L i n k
 *
 * This routine creates a soft link
 *
 * Inputs:
 *	LINKNAME_P	FullName of the link to be created
 *	TARGETNAME_P	FullName of link target
 *	TIMEOUT_P	Timeout duration for link to exist
 *
 * Returns:
 *	RET_P		Timestamp of created link
 */
void 
clerkCreateLink (thread_t *th_p)
{
    DEB_ASCII_BUF_FullName(linkNameBuf)
    DEB_ASCII_BUF_FullName(targetNameBuf)
    DEB_ASCII_BUF_Timeout(timeoutBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " >clerkCreateLink(%s,%s,%s,%p)",
        deb_ascii_FullName(linkNameBuf,th_p->LINKNAME_P),
        deb_ascii_FullName(targetNameBuf,th_p->TARGETNAME_P),
        deb_ascii_Timeout(timeoutBuf,th_p->TIMEOUT_P),
        th_p->RET_P));

    th_p->taBuildReq = buildCreateLinkRequest;
    th_p->taParseRsp = parseCreateLinkResponse;
    th_p->taRPC = CreateLinkRPC;
    ++th_p->skipResolveNameCache;

    sendToWalkTree(th_p, th_p->LINKNAME_P);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " <clerkCreateLink(%s,%s,%s,%p) status(%s)",
        deb_ascii_FullName(linkNameBuf,th_p->LINKNAME_P),
        deb_ascii_FullName(targetNameBuf,th_p->TARGETNAME_P),
        deb_ascii_Timeout(timeoutBuf,th_p->TIMEOUT_P),
        th_p->RET_P,
        deb_ascii_dns_status(statusBuf,th_p->clerkStatus)));
}
