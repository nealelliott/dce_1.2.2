/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: clerkmodifyattribute.c,v $
 * Revision 1.1.9.2  1996/02/18  19:26:34  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:09:28  marty]
 *
 * Revision 1.1.9.1  1995/12/08  15:05:10  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:38:37  root]
 * 
 * Revision 1.1.7.5  1994/08/03  18:59:45  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:18:01  mccann]
 * 
 * Revision 1.1.7.4  1994/06/23  18:29:10  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:55:37  mccann]
 * 
 * Revision 1.1.7.3  1994/06/09  16:07:57  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:52:43  devsrc]
 * 
 * Revision 1.1.7.2  1994/04/14  14:41:14  peckham
 * 	New module clerk_socket.c to isolate SVR4 named pipes.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	Remove msgblk.h, cds_event_init() and evp references.
 * 	Remove v1v2 code (v1 never used for cds).
 * 	Remove VMS and UNIX conditionals, VMS code.
 * 	Remove last vestige of clerk_trace().
 * 	[1994/04/11  19:36:28  peckham]
 * 
 * Revision 1.1.7.1  1994/03/12  21:56:50  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:53:10  peckham]
 * 
 * Revision 1.1.4.3  1993/03/09  19:44:34  peckham
 * 	OT#7411: Request given to gda when other clearinghouse
 * 	available. Requests using ET_dirOrObj were not setting
 * 	cache_the_dir, causing WalkTree not to match the cellname.
 * 	[1993/03/09  19:39:08  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  12:42:29  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:28:23  zeliff]
 * 
 * Revision 1.1.2.2  1992/03/22  21:27:35  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:09:39  weisman]
 * 
 * Revision 1.1  1992/01/19  15:27:03  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module clerkmodifyattribute.c
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
 *	Clerk.ModifyAttribute command processor.
 *
 * Networks & CommunicaIon Software Engineering
 *
 */

#include <dce/dce.h>
#include <dce/dcecdsmsg.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <uc_child.h>
#include <cache.h>

/*
 * ModifyAttribute arguments (passed in the thread block)
 */
#define	ENTRY_P		clerkArgs[0]
#define LOOKINGFOR_P	clerkArgs[1]
#define	UPDATE_P	clerkArgs[2]

/*
 * Local Prototype declarations.
 */

static int 
ModifyAttributeRPC (thread_t  *,
                    Set_t     *,
                    ObjUID_t  *);

int
buildModifyAttributeRequest (thread_t *);

static void
parseModifyAttributeResponse (thread_t *);

void 
clerkModifyAttribute (thread_t *);








/*
 * Routine to call RPC stub for ModifyAttribute request
 */
static int 
ModifyAttributeRPC (thread_t  *th_p,
                    Set_t     *tower_p,
                    ObjUID_t  *uuid_p)
{
    rpc_binding_handle_t  h;
    int i;
    cds_Progress_t Progress;
    cds_Update_t update;
    error_status_t rpcStatus, commStatus = rpc_s_no_bindings;
    cds_status_t  dnsStatus;
    int fromcache, getout;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " >ModifyAttributeRPC"));

    /* Set up structures for calling RPC */
    Build_R_Progress((byte_t *)th_p->taProgress.flags,
		     (Timeout_t *)th_p->taProgress.Timeout,
		     th_p->taProgress.Unresolved,
		     th_p->taProgress.Resolved,
		     &Progress);
    /* Fails if no memory */
    if (!L_to_R_Update((Update_t *)th_p->UPDATE_P, &update, &dnsStatus))
    {
	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_child,
            svc_c_debug4,
            " <ModifyAttributeRPC: return(DNS_RESOURCEERROR)"));

	return(DNS_RESOURCEERROR);
    }
    

    /*
     * First attempt to find a cached handle. There
     * is a use count associated with handles, so make
     * sure either free or release cached handle is called
     */
    if (fromcache = get_cached_handle(&h, uuid_p, th_p))
      commStatus = cds_ModifyAttribute(h, &Progress,
			    EXT8(th_p->LOOKINGFOR_P), &update, &dnsStatus);
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
	    commStatus = cds_ModifyAttribute(h, &Progress,
			      EXT8(th_p->LOOKINGFOR_P), &update, &dnsStatus);
	    if (commStatus == rpc_s_ok) break;
	    maximize_rpc_error(th_p, commStatus, &getout);
	    rpc_binding_free(&h, &rpcStatus);
	    if (getout) break;
	} /* End For loop */
    }

    if (update.ud_type != AT_null)
      R_AtomicValue_Free(&update.ud_value);

    if (commStatus == rpc_s_ok) {
	th_p->taRspMsgPtr_p = th_p->taRspMsg_p;
	if (dnsStatus.er_status == PDU_Success) {
	    th_p->taRspType = MT_Response;
	    th_p->taRspMsgPtr_p = R_to_L_Progress(&Progress,
				      (Progress_t *)th_p->taRspMsgPtr_p, TRUE);
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
            " <ModifyAttributeRPC: return(DNS_SUCCESS)"));

	return(DNS_SUCCESS);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " <ModifyAttributeRPC: return(DNS_NOCOMMUNICATION)"));

    return(DNS_NOCOMMUNICATION);
}

/*
 *	b u i l d M o d i f y A t t r i b u t e R e q u e s t
 *
 * Routine to build an ModifyAttribute request message and
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
buildModifyAttributeRequest (thread_t *th_p)
{
    int length = 0;
    Update_u upd;
    Update_t *upd_p;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >buildModifyAttributeRequest"));

    upd_p = (Update_t *)th_p->UPDATE_P;

    /*
     * ModifyAttributeRequest message:
     *  progress: Progress
     *	entryType: EntryType
     *	update: Update
     *
     */
    length += buildProgress(th_p);
    length += LEN_EntryType(th_p->LOOKINGFOR_P);
    length += LEN_Update((byte_t *)upd_p);

    if (th_p->taReqMsgEnd_p) {
	COPY_EntryType(th_p->LOOKINGFOR_P, th_p->taReqMsgEnd_p);
	th_p->taReqMsgEnd_p = SKIP_EntryType(th_p->taReqMsgEnd_p);

	COPY_bytes(upd_p, th_p->taReqMsgEnd_p, LEN_Update((byte_t *)upd_p));
	th_p->taReqMsgEnd_p = SKIP_Update(th_p->taReqMsgEnd_p);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <buildModifyAttributeRequest: return(length=%d)",
        length));

    return(length);
}

/*
 *	p a r s e M o d i f y A t t r i b u t e R e s p o n s e
 *
 * This local routine parses the response message.
 */
static void
parseModifyAttributeResponse (thread_t *th_p)
{
    AttributeSpecifier_t *att_p;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >parseModifyAttributeResponse"));

    /*
     * ModifyAttributeReqsponse message:
     *  progress: Progress
     */
    ta_begin(th_p);
    ta_Progress(th_p);
    ta_end(th_p);

    if (th_p->taRspMsg_p) {
      Progress_t *const pr_p = (Progress_t *)th_p->taRspMsg_p;

      att_p = (AttributeSpecifier_t *)((Update_t *)th_p->UPDATE_P)->ud_attribute;

      /* Use resolved name */
      CARemoveAttribute(
	     (FullName_t *)SKIP_FullName(pr_p->pr_unresolvedName),
	     (EntryType_t *)th_p->LOOKINGFOR_P,
	     (AttributeName_t *)att_p->as_name);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <parseModifyAttributeResponse"));
}

/*
 *	c l e r k M o d i f y A t t r i b u t e
 *
 * This routine modifies an attribute, according to the entry type supplied.
 *
 * Inputs:
 *	ENTRY_P		FullName of object, directory or child
 *	LOOKINGFOR_P	EntryType (object, directory, child)
 *	UPDATE_P	Update packet
 *
 * Outputs:
 */
void 
clerkModifyAttribute (thread_t *th_p)
{
    DEB_ASCII_BUF_FullName(entryBuf)
    DEB_ASCII_BUF_EntryType(lookingForBuf)
    DEB_ASCII_BUF_Update(updateBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " >clerkModifyAttribute(%s,%s,%s)",
        deb_ascii_FullName(entryBuf,th_p->ENTRY_P),
        deb_ascii_EntryType(lookingForBuf,th_p->LOOKINGFOR_P),
        deb_ascii_Update(updateBuf,th_p->UPDATE_P)));

    /*
     * If this is a directory, cache the final progress info.
     */
    if ((EXT8(th_p->LOOKINGFOR_P) == ET_directory)

#ifdef ET_dirOrObj

	|| (EXT8(th_p->LOOKINGFOR_P) == ET_dirOrObj)

#endif

	|| (EXT8(th_p->LOOKINGFOR_P) == ET_clearinghouse))
	th_p->cache_the_dir = 1;

    /*
     * If this is a clearinghouse, and confidence is high,
     * don't need to lookup replica set cause there is only one.
     */
    if (EXT8(th_p->LOOKINGFOR_P) == ET_clearinghouse)
      ++th_p->NoReplicas;
    else if (EXT8(th_p->LOOKINGFOR_P) == ET_softlink)
	++th_p->skipResolveNameCache;

    th_p->taBuildReq = buildModifyAttributeRequest;
    th_p->taParseRsp = parseModifyAttributeResponse;
    th_p->taRPC = ModifyAttributeRPC;

    sendToWalkTree(th_p, th_p->ENTRY_P);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " <clerkModifyAttribute(%s,%s,%s) status(%s)",
        deb_ascii_FullName(entryBuf,th_p->ENTRY_P),
        deb_ascii_EntryType(lookingForBuf,th_p->LOOKINGFOR_P),
        deb_ascii_Update(updateBuf,th_p->UPDATE_P),
        deb_ascii_dns_status(statusBuf,th_p->clerkStatus)));
}
