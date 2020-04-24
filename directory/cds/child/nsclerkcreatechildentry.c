/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: nsclerkcreatechildentry.c,v $
 * Revision 1.1.8.2  1996/02/18  19:27:06  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:09:44  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:05:46  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:38:59  root]
 * 
 * Revision 1.1.6.4  1994/08/03  18:59:56  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:15:58  mccann]
 * 
 * Revision 1.1.6.3  1994/06/23  18:29:21  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:56:35  mccann]
 * 
 * Revision 1.1.6.2  1994/06/09  16:08:09  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:52:55  devsrc]
 * 
 * Revision 1.1.6.1  1994/03/12  21:57:14  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:54:31  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  12:43:20  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:29:32  zeliff]
 * 
 * Revision 1.1.2.2  1992/03/22  21:34:45  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:10:49  weisman]
 * 
 * Revision 1.1  1992/01/19  15:27:25  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module nsclerkcreatechildentry.c
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
 *	NSClerk.CreateChildEntry command processor.
 *
 * Networks & Communications Software Engineering
 * 
 */

#include <dce/dce.h>
#include <dce/dcecdsmsg.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <string.h>
#include <uc_child.h>
#include <uc_clerk.h>

/*
 * CreateChildEntry arguments (passed in the thread block)
 */
#define	CHILDNAME_P	clerkArgs[0]
#define	ONBEHALFOF_P	clerkArgs[1]
#define	CHILDID_P	clerkArgs[2]
#define	CHILDREPLICAS_P	clerkArgs[3]
#define	OUTPARENTID_P	clerkArgs[4]

/*
 * Local Prototype declarations.
 */

int 
CreateChildEntryRPC (thread_t  *,
                     Set_t     *,
                     ObjUID_t  *);

int
buildCreateChildEntryRequest (thread_t *);

static void
parseCreateChildEntryResponse (thread_t *);

void 
nsclerkCreateChildEntry (thread_t *);



/*
 * Routine to call RPC stub for CreateChild request 
 */
int 
CreateChildEntryRPC (thread_t  *th_p,
                     Set_t     *tower_p,
                     ObjUID_t  *uuid_p)
{
    rpc_binding_handle_t  h;
    int i;
    ObjUID_t childID,parentID;
    cds_Progress_t Progress;
    cds_Set_t *rp_p;
    error_status_t rpcStatus,commStatus = rpc_s_no_bindings;
    cds_status_t  dnsStatus;
    byte_t *msg_p;
    int fromcache, getout=0;
    sec_id_foreign_t user;
    byte_t *user_p;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " >CreateChildEntryRPC"));

    /* Set up structures for calling RPC */
    Build_R_Progress((byte_t *)th_p->taProgress.flags, 
		     (Timeout_t *)th_p->taProgress.Timeout, 
		     th_p->taProgress.Unresolved, 
		     th_p->taProgress.Resolved,
		     &Progress);
    msg_p = L_to_R_Set((Set_t *)th_p->CHILDREPLICAS_P, &rp_p, &dnsStatus);
    if (!msg_p)
    {
	DCE_SVC_DEBUG((
           cds__svc_handle,
           cds_svc_child,
           svc_c_debug4,
           " <CreateChildEntryRPC: return(DNS_RESOURCEERROR)"));

	return(DNS_RESOURCEERROR);
    }
    

    /* Align child uid */
    COPY_ObjUID(th_p->CHILDID_P, &childID);

    /* 
     * First attempt to find a cached handle. There
     * is a use count associated with handles, so make
     * sure either free or release cached handle is called 
     */
    if (fromcache = get_cached_handle(&h, uuid_p, th_p)) {
	if (prm_in_length(th_p->ONBEHALFOF_P) == 0)
	  commStatus = cds_CreateChild(h, &Progress, 
				(sec_id_foreign_t *)NULL, &childID,
					 rp_p, &parentID, &dnsStatus);
	else {
	    /* Build user, unpack sec_id_foreign_t */
	    user_p = (byte_t *)th_p->ONBEHALFOF_P;
	    /* Set up principal name */
	    memcpy(&user.id.uuid, user_p, sizeof(user.id.uuid));
	    user_p += sizeof(user.id.uuid);
	    user.id.name = user_p;
	    user_p += strlen((char *)user_p) + 1;
	    /* Set up realm */
	    memcpy(&user.realm.uuid, user_p, sizeof(user.realm.uuid));
	    user_p += sizeof(user.realm.uuid);
	    user.realm.name = user_p;
	      
	    commStatus = cds_CreateChild(h, &Progress,
			    (sec_id_foreign_t *)&user, &childID, rp_p,
					 &parentID, &dnsStatus);
	}
    }

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
	    if (prm_in_length(th_p->ONBEHALFOF_P) == 0)
	      commStatus = cds_CreateChild(h, &Progress, 
				(sec_id_foreign_t *)NULL, &childID,
					 rp_p, &parentID, &dnsStatus);
	    else {
		/* Build user, unpack sec_id_foreign_t */
		user_p = (byte_t *)th_p->ONBEHALFOF_P;
		/* Set up principal name */
		memcpy(&user.id.uuid, user_p, sizeof(user.id.uuid));
		user_p += sizeof(user.id.uuid);
		user.id.name = user_p;
		user_p += strlen((char *)user_p) + 1;
		/* Set up realm */
		memcpy(&user.realm.uuid, user_p, sizeof(user.realm.uuid));
		user_p += sizeof(user.realm.uuid);
		user.realm.name = user_p;
		
		commStatus = cds_CreateChild(h, &Progress,
			    (sec_id_foreign_t *)&user, &childID, rp_p,
					 &parentID, &dnsStatus);
	    }
	    if (commStatus == rpc_s_ok) break;
	    maximize_rpc_error(th_p, commStatus, &getout);
	    rpc_binding_free(&h, &rpcStatus);
	    if (getout) break;
	} /* End For loop */
    }

    R_Set_Free(rp_p);

    if (commStatus == rpc_s_ok) {
	th_p->taRspMsgPtr_p = th_p->taRspMsg_p;
	if (dnsStatus.er_status == PDU_Success) {
	    th_p->taRspType = MT_Response;
	    th_p->taRspMsgPtr_p = R_to_L_Progress(&Progress,
				(Progress_t *)th_p->taRspMsgPtr_p, TRUE);
	    if (FLAG_IS_SET(&Progress.pr_flags, PR_done))
	      COPY_ObjUID(&parentID, th_p->taRspMsgPtr_p);
	    th_p->taRspMsgPtr_p = SKIP_ObjUID(th_p->taRspMsgPtr_p);
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
           " <CreateChildEntryRPC: return(DNS_SUCCESS)"));

	return(DNS_SUCCESS);
    }   

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " <CreateChildEntryRPC: return(DNS_NOCOMMUNICATION"));

    return(DNS_NOCOMMUNICATION);
}

/*
 *	b u i l d C r e a t e C h i l d E n t r y R e q u e s t
 *
 * Routine to build an CreateChildEntry request message and
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
buildCreateChildEntryRequest (thread_t *th_p)
{
    int length = 0;
    Set_t *set_p;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >buildCreateChildEntryRequest"));

    /*
     * CreateChildEntryRequest record:
     *	progress: Progress
     *	onBehalfOf: AuthenticationInfo
     *	childID: ObjUID
     *	childReplicas: Set
     */
    length += buildProgress(th_p);

    if (th_p->taReqMsgEnd_p) {
	COPY_AuthenticationInfo(th_p->ONBEHALFOF_P, th_p->taReqMsgEnd_p);
	th_p->taReqMsgEnd_p = SKIP_AuthenticationInfo(th_p->taReqMsgEnd_p);
    }
    length += LEN_AuthenticationInfo(th_p->ONBEHALFOF_P);

    if (th_p->taReqMsgEnd_p) {
	COPY_ObjUID(th_p->CHILDID_P, th_p->taReqMsgEnd_p);
	th_p->taReqMsgEnd_p = SKIP_ObjUID(th_p->taReqMsgEnd_p);
    }
    length += LEN_ObjUID(th_p->CHILDID_P);

      set_p = (Set_t *)th_p->CHILDREPLICAS_P;

    if (th_p->taReqMsgEnd_p) {
      COPY_Set(set_p, th_p->taReqMsgEnd_p);
      th_p->taReqMsgEnd_p = SKIP_Set(th_p->taReqMsgEnd_p);
    }

    length += LEN_Set(set_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <buildCreateChildEntryRequest: return(length=%d)",
        length));

    return(length);
}

/*
 *	p a r s e C r e a t e C h i l d E n t r y R e s p o n s e
 *
 * This local routine parses the response message.
 */
static void
parseCreateChildEntryResponse (thread_t *th_p)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >parseCreateChildEntryResponse"));

    /*
     * CreateChildEntryResponse record:
     *	progress: Progress
     *	outParentID: ObjUID
     */
    ta_begin(th_p);
    ta_Progress(th_p);
    ta_ObjUID(th_p, th_p->OUTPARENTID_P);
    ta_end(th_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <parseCreateChildEntryResponse"));
}


/*
 *	n s c l e r k C r e a t e C h i l d E n t r y
 *
 * This routine creates a ChildPointer entry
 *
 * Inputs:
 *	CHILDNAME_P	FullName of ChildPointer to be created
 *	ONBEHALFOF_P	AuthenticationInfo
 *	CHILDID_P	ObjUID
 *	CHILDREPLICAS_P	Set
 *
 * Outputs:
 *	OUTPARENTID_P	ObjUID
 */
void 
nsclerkCreateChildEntry (thread_t *th_p)
{
    DEB_ASCII_BUF_FullName(childNameBuf)
    DEB_ASCII_BUF_ObjUID(childIDBuf)
    DEB_ASCII_BUF_ObjUID(outParentIDBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " >nsclerkCreateChildEntry(%s,,%s,)",
        deb_ascii_FullName(childNameBuf,th_p->CHILDNAME_P),
        deb_ascii_ObjUID(childIDBuf,th_p->CHILDID_P)));

    th_p->taBuildReq = buildCreateChildEntryRequest;
    th_p->taParseRsp = parseCreateChildEntryResponse;
    th_p->taRPC = CreateChildEntryRPC;

    sendToWalkTree(th_p, th_p->CHILDNAME_P);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " <nsclerkCreateChildEntry(%s,,%s,%s) status(%s)",
        deb_ascii_FullName(childNameBuf,th_p->CHILDNAME_P),
        deb_ascii_ObjUID(childIDBuf,th_p->CHILDID_P),
        deb_ascii_ObjUID(outParentIDBuf,th_p->OUTPARENTID_P),
        deb_ascii_dns_status(statusBuf,th_p->clerkStatus)));
}
