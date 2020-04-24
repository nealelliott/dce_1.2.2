/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: nsclerkdoupdate.c,v $
 * Revision 1.1.8.2  1996/02/18  19:27:10  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:09:46  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:05:57  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:39:03  root]
 * 
 * Revision 1.1.6.5  1994/08/03  18:59:59  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:15:51  mccann]
 * 
 * Revision 1.1.6.4  1994/06/23  18:29:23  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:56:43  mccann]
 * 
 * Revision 1.1.6.3  1994/06/09  16:08:11  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:52:57  devsrc]
 * 
 * Revision 1.1.6.2  1994/04/14  14:41:19  peckham
 * 	New module clerk_socket.c to isolate SVR4 named pipes.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	Remove msgblk.h, cds_event_init() and evp references.
 * 	Remove v1v2 code (v1 never used for cds).
 * 	Remove VMS and UNIX conditionals, VMS code.
 * 	Remove last vestige of clerk_trace().
 * 	[1994/04/11  19:36:54  peckham]
 * 
 * Revision 1.1.6.1  1994/03/12  21:57:18  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:54:44  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  12:43:30  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:29:42  zeliff]
 * 
 * Revision 1.1.2.2  1992/03/22  21:35:14  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:10:58  weisman]
 * 
 * Revision 1.1  1992/01/19  15:27:14  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module nsclerkdoupdate.c
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
 *	NSClerk.DoUpdate command processor.
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
 * DoUpdate arguments (passed in the thread block)
 */
#define	REPLICA_P	clerkArgs[0]
#define	EPOCH_P		clerkArgs[1]
#define UPDATES_P	clerkArgs[2]


/*
 * Local Prototype declarations.
 */

int
DoUpdateRPC (thread_t  *,
             Set_t     *,
             ObjUID_t  *);

int
buildDoUpdateRequest (thread_t *);

static void
parseDoUpdateResponse (thread_t *);

void 
nsclerkDoUpdate (thread_t *);



/*
 * Routine to call RPC stub for DoUpdate request
 */
int
DoUpdateRPC (thread_t  *th_p,
             Set_t     *tower_p,
             ObjUID_t  *uuid_p)
{
    rpc_binding_handle_t  h;
    int                   i;
    cds_UpdatePkt_t       *update_p;
    error_status_t        rpcStatus,commStatus = rpc_s_no_bindings;
    cds_status_t          dnsStatus;
    ObjUID_t              epoch;
    int fromcache, getout=0;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " >DoUpdateRPC"));

    /* Align uuid */
    COPY_ObjUID(th_p->EPOCH_P, &epoch);
   
    /* Set up structures for calling RPC */
    if (L_to_R_UpdatePkt((UpdatePacket_t *)th_p->UPDATES_P, &update_p, 
			 &dnsStatus) == 
	NULL)
    {
	DCE_SVC_DEBUG((
           cds__svc_handle,
           cds_svc_child,
           svc_c_debug4,
           " <DoUpdateRPC: return(DNS_RESOURCEERROR)"));

	return(DNS_RESOURCEERROR);
    }
    

    /* 
     * First attempt to find a cached handle. There
     * is a use count associated with handles, so make
     * sure either free or release cached handle is called 
     */
    if (fromcache = get_cached_handle(&h, uuid_p, th_p))
      commStatus = cds_DoUpdate(h, &epoch, update_p, &dnsStatus);

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
	    commStatus = cds_DoUpdate(h, &epoch, update_p, &dnsStatus);
	    if (commStatus == rpc_s_ok) break;
	    maximize_rpc_error(th_p, commStatus, &getout);
	    rpc_binding_free(&h, &rpcStatus);
	    if (getout) break;
	} /* End For loop */
    }

    R_UpdatePkt_Free(update_p);

    if (commStatus == rpc_s_ok) {
	th_p->taRspMsgPtr_p = th_p->taRspMsg_p;
	if (dnsStatus.er_status == PDU_Success) {
	    th_p->taRspType = MT_Response;
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
           " <DoUpdateRPC: return(DNS_SUCCESS)"));

	return(DNS_SUCCESS);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " <DoUpdateRPC: return(DNS_NOCOMMUNICATION)"));

    return(DNS_NOCOMMUNICATION);
}

/*
 *	b u i l d D o U p d a t e R e q u e s t
 *
 * Routine to build an DoUpdate request message and
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
buildDoUpdateRequest (thread_t *th_p)
{
    int length = 0;
    UpdatePacket_t *upd_p;
    /* 
     * On a conversion from v2 to v1 we may increase the size of the
     * update packet.   This buffer will hold it all.
     * This routine is called first to determine the length of the
     * message so overflow will be detected then.   
     */
    byte_t upd[NSMAXSIZE*2];


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >buildDoUpdateRequest"));

    upd_p = (UpdatePacket_t *)th_p->UPDATES_P;

    /*
     * DoUpdateRequest record:
     *	epoch: ObjUID
     *	updates: UpdatePacket
     */
    if (th_p->taReqMsgEnd_p) {
	COPY_ObjUID(th_p->EPOCH_P, th_p->taReqMsgEnd_p);
	th_p->taReqMsgEnd_p = SKIP_ObjUID(th_p->taReqMsgEnd_p);
    }
    length += LEN_ObjUID(th_p->EPOCH_P);

    if (th_p->taReqMsgEnd_p) {
      COPY_UpdatePacket((byte_t *)upd_p, th_p->taReqMsgEnd_p);
      th_p->taReqMsgEnd_p = SKIP_UpdatePacket((byte_t *)th_p->taReqMsgEnd_p);
    }
    length += LEN_UpdatePacket((byte_t *)upd_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <buildDoUpdateRequest: return(length=%d)",
        length));

    return(length);
}

/*
 *	p a r s e D o U p d a t e R e s p o n s e
 *
 * This local routine parses the response message.
 */
static void
parseDoUpdateResponse (thread_t *th_p)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >parseDoUpdateResponse"));

    /*
     * DoUpdateResponse record:
     *	(*Nothing*)
     */
    ta_begin(th_p);
    ta_end(th_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <parseDoUpdateResponse"));
}

/*
 *	n s c l e r k D o U p d a t e
 *
 * Copy updates from one replica to another
 *
 * Inputs:
 *	REPLICA_P	ReplicaPointer clearinghouse to receive update
 *	EPOCH_P		ObjUID epoch of directory
 *	UPDATES_P	UpdatePacket
 */
void 
nsclerkDoUpdate (thread_t *th_p)
{
    DEB_ASCII_BUF_ReplicaPointer(replicaBuf)
    DEB_ASCII_BUF_ObjUID(epochBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " >nsclerkDoUpdate(%s,%s,)",
        deb_ascii_ReplicaPointer(replicaBuf,th_p->REPLICA_P),
        deb_ascii_ObjUID(epochBuf,th_p->EPOCH_P)));

    th_p->taBuildReq = buildDoUpdateRequest;
    th_p->taParseRsp = parseDoUpdateResponse;
    th_p->taRPC = DoUpdateRPC;

    sendToReplica(th_p, (ReplicaPointer_t *)th_p->REPLICA_P);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " <nsclerkDoUpdate(%s,%s,) status(%s)",
        deb_ascii_ReplicaPointer(replicaBuf,th_p->REPLICA_P),
        deb_ascii_ObjUID(epochBuf,th_p->EPOCH_P),
        deb_ascii_dns_status(statusBuf,th_p->clerkStatus)));
}
