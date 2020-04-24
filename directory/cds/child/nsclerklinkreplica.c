/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: nsclerklinkreplica.c,v $
 * Revision 1.1.8.2  1996/02/18  19:27:13  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:09:48  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:06:00  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:39:05  root]
 * 
 * Revision 1.1.6.5  1994/08/03  19:00:02  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:15:48  mccann]
 * 
 * Revision 1.1.6.4  1994/06/23  18:29:25  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:56:48  mccann]
 * 
 * Revision 1.1.6.3  1994/06/09  16:08:12  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:52:58  devsrc]
 * 
 * Revision 1.1.6.2  1994/04/14  14:41:20  peckham
 * 	New module clerk_socket.c to isolate SVR4 named pipes.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	Remove msgblk.h, cds_event_init() and evp references.
 * 	Remove v1v2 code (v1 never used for cds).
 * 	Remove VMS and UNIX conditionals, VMS code.
 * 	Remove last vestige of clerk_trace().
 * 	[1994/04/11  19:37:00  peckham]
 * 
 * Revision 1.1.6.1  1994/03/12  21:57:19  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:54:51  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  12:43:35  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:29:47  zeliff]
 * 
 * Revision 1.1.2.2  1992/03/22  21:35:47  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:11:07  weisman]
 * 
 * Revision 1.1  1992/01/19  15:27:15  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module nsclerklinkreplica.c
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
 *	NSClerk.LinkReplica command processor.
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
#include <cache.h>
#include <dce/attributes.h>

/*
 * LinkReplica arguments (passed in the thread block)
 */
#define	ONBEHALFOF_P	clerkArgs[0]
#define	DIRECTORY_P	clerkArgs[1]
#define	DIRID_P 	clerkArgs[2]
#define	NEWREPLICA_P	clerkArgs[3]
#define	EPOCH_P		clerkArgs[4]
#define	NEXTRING_P	clerkArgs[5]
#define	REPLICAVERSION_P clerkArgs[6]

/*
 * Local Prototype declarations.
 */

int 
LinkReplicaRPC (thread_t  *,
                Set_t     *,
                ObjUID_t  *);

int 
buildLinkReplicaRequest (thread_t *);

static void 
parseLinkReplicaResponse (thread_t *);

void 
nsclerkLinkReplica (thread_t *);



/*
 * Routine to call RPC stub for Linkreplica request
 */
int 
LinkReplicaRPC (thread_t  *th_p,
                Set_t     *tower_p,
                ObjUID_t  *uuid_p)
{
    rpc_binding_handle_t  h;
    int i;
    cds_Progress_t Progress;
    ObjUID_t directory, epoch, ring;
    cds_Version_t rpversion;
    cds_Update_t  update;
    error_status_t rpcStatus,commStatus = rpc_s_no_bindings;
    cds_status_t  dnsStatus;
    int fromcache, getout=0;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " >LinkReplicaRPC"));

    /* Align uuid */
    COPY_ObjUID(th_p->DIRID_P, &directory);

    /* Set up structures for calling RPC */
    if (L_to_R_Update((Update_t *)th_p->NEWREPLICA_P, &update, &dnsStatus) ==
	NULL)
    {
	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_child,
            svc_c_debug4,
            " <LinkReplicaRPC: return(DNS_RESOURCEERROR)"));

	return(DNS_RESOURCEERROR);
    }
    
    Build_R_Progress((byte_t *)th_p->taProgress.flags, 
		     (Timeout_t *)th_p->taProgress.Timeout, 
		     th_p->taProgress.Unresolved, 
		     th_p->taProgress.Resolved,
		     &Progress);

    /* 
     * First attempt to find a cached handle. There
     * is a use count associated with handles, so make
     * sure either free or release cached handle is called 
     */
    if (fromcache = get_cached_handle(&h, uuid_p, th_p))
      commStatus = cds_LinkReplica(h, &Progress, &directory, &update, 
				   &epoch, &ring, &rpversion, &dnsStatus);

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
	    commStatus = cds_LinkReplica(h, &Progress, &directory, &update, 
				       &epoch, &ring, &rpversion, &dnsStatus);
	    if (commStatus == rpc_s_ok) break;
	    maximize_rpc_error(th_p, commStatus, &getout);
	    rpc_binding_free(&h, &rpcStatus);
	    if (getout) break;
	} /* End For loop */
    }

    R_AtomicValue_Free(&update.ud_value);

    if (commStatus == rpc_s_ok) {
	th_p->taRspMsgPtr_p = th_p->taRspMsg_p;
	if (dnsStatus.er_status == PDU_Success) {
	    th_p->taRspType = MT_Response;
	    th_p->taRspMsgPtr_p = R_to_L_Progress(&Progress, 
				      (Progress_t *)th_p->taRspMsgPtr_p, TRUE);
	    if (FLAG_IS_SET(&Progress.pr_flags, PR_done)) {
		COPY_ObjUID(&epoch, th_p->taRspMsgPtr_p);
		th_p->taRspMsgPtr_p =SKIP_ObjUID(th_p->taRspMsgPtr_p);
		COPY_ObjUID(&ring, th_p->taRspMsgPtr_p);
		th_p->taRspMsgPtr_p =SKIP_ObjUID(th_p->taRspMsgPtr_p);
		COPY_VersionNumber(&rpversion, th_p->taRspMsgPtr_p);
		th_p->taRspMsgPtr_p = SKIP_VersionNumber(th_p->taRspMsgPtr_p);
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
            " <LinkReplicaRPC: return(DNS_SUCCESS)"));

	return(DNS_SUCCESS);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " <LinkReplicaRPC: return(DNS_NOCOMMUNICATION)"));

    return(DNS_NOCOMMUNICATION);
}

/*
 *	b u i l d L i n k R e p l i c a R e q u e s t
 *
 * Routine to build an LinkReplica request message and
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
buildLinkReplicaRequest (thread_t *th_p)
{
    int length = 0;
    Update_t *upd_p;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >buildLinkReplicaRequest"));

    /*
     * LinkReplicaRequest record:
     *	progress: Progress
     *	directory: ObjUID
     *	onBehalfOf: AuthenticationInfo
     *	replica: Update
     */
    length += buildProgress(th_p);

    if (th_p->taReqMsgEnd_p) {
	COPY_ObjUID(th_p->DIRID_P, th_p->taReqMsgEnd_p);
	th_p->taReqMsgEnd_p = SKIP_ObjUID(th_p->taReqMsgEnd_p);
    }
    length += LEN_ObjUID(th_p->DIRID_P);

    if (th_p->taReqMsgEnd_p) {
	COPY_AuthenticationInfo(th_p->ONBEHALFOF_P, th_p->taReqMsgEnd_p);
	th_p->taReqMsgEnd_p = SKIP_AuthenticationInfo(th_p->taReqMsgEnd_p);
    }
    length += LEN_AuthenticationInfo(th_p->ONBEHALFOF_P);

    upd_p = (Update_t *)th_p->NEWREPLICA_P;

    if (th_p->taReqMsgEnd_p) {
	COPY_Update((byte_t *)upd_p, th_p->taReqMsgEnd_p);
	th_p->taReqMsgEnd_p = SKIP_Update((byte_t *)th_p->taReqMsgEnd_p);
    }
    length += LEN_Update((byte_t *)upd_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <buildLinkReplicaRequest: return(length=%d)",
	length));

    return(length);
}

/*
 *	p a r s e L i n k R e p l i c a R e s p o n s e
 *
 * This local routine parses the response message.
 */
static void 
parseLinkReplicaResponse (thread_t *th_p)
{
    static bytes_u replicaVersion[2] = {1,0};
    static EntryType_t dirType = {ET_directory};
    static EntryType_t childType = {ET_childPointer};


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >parseLinkReplicaResponse"));

    /*
     * LinkReplicaResponse record:
     *	progress: Progress
     *	epoch: ObjUID
     *	nextRing: ObjUID
     *	replicaVersion: VersionNumber
     */
    ta_begin(th_p);
    ta_Progress(th_p);
    ta_ObjUID(th_p, th_p->EPOCH_P);
    ta_ObjUID(th_p, th_p->NEXTRING_P);

    ta_VersionNumber(th_p, th_p->REPLICAVERSION_P);
    ta_end(th_p);

    if (th_p->taRspMsg_p) {
	CARemoveAttribute((FullName_t *)th_p->DIRECTORY_P, &dirType,
			  dns_AttributeName(DNS_REPLICAS));

	CARemoveAttribute((FullName_t *)th_p->DIRECTORY_P, &childType,
			  dns_AttributeName(DNS_REPLICAS));
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <parseLinkReplicaResponse"));
}

/*
 *	n s c l e r k L i n k R e p l i c a
 *
 * Link up a new replica with other replicas of a directory
 *
 * Inputs:
 *	ONBEHALFOF_P	AuthenticationInfo
 *	DIRECTORY_P	FullName of which directory
 *	DIRID_P 	ObjUID for sanity check
 *	NEWREPLICA_P	Update to add to replica set
 *
 * Outputs:
 *	EPOCH_P		ObjUID epoch of ring
 *	NEXTRING_P	ObjUID next replica in ring
 *	REPLICAVERSION_P VersionNumber of this replica
 */
void 
nsclerkLinkReplica (thread_t *th_p)
{
    DEB_ASCII_BUF_ObjUID(directoryBuf)
    DEB_ASCII_BUF_ObjUID(dirIDBuf)
    DEB_ASCII_BUF_Update(newReplicaBuf)
    DEB_ASCII_BUF_ObjUID(epochBuf)
    DEB_ASCII_BUF_ObjUID(nextRingBuf)
    DEB_ASCII_BUF_VersionNumber(replicaVersionBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " >nsclerkLinkReplica(,%s,%s,%s,,,)",
        deb_ascii_ObjUID(directoryBuf,th_p->DIRECTORY_P),
        deb_ascii_ObjUID(dirIDBuf,th_p->DIRID_P),
        deb_ascii_Update(newReplicaBuf,th_p->NEWREPLICA_P)));

    th_p->taBuildReq = buildLinkReplicaRequest;
    th_p->taParseRsp = parseLinkReplicaResponse;
    th_p->taRPC = LinkReplicaRPC;
    th_p->cache_the_dir = 1;

    sendToWalkTree(th_p, th_p->DIRECTORY_P);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " <nsclerkLinkReplica(,%s,%s,%s,%s,%s,%s) status(%s)",
        deb_ascii_ObjUID(directoryBuf,th_p->DIRECTORY_P),
        deb_ascii_ObjUID(dirIDBuf,th_p->DIRID_P),
        deb_ascii_Update(newReplicaBuf,th_p->NEWREPLICA_P),
        deb_ascii_ObjUID(epochBuf,th_p->EPOCH_P),
        deb_ascii_ObjUID(nextRingBuf,th_p->NEXTRING_P),
        deb_ascii_VersionNumber(replicaVersionBuf,th_p->REPLICAVERSION_P),
        deb_ascii_dns_status(statusBuf,th_p->clerkStatus)
        ));
}
