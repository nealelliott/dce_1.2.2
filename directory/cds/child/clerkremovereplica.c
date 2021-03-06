/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: clerkremovereplica.c,v $
 * Revision 1.1.8.2  1996/02/18  19:26:45  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:09:34  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:05:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:38:45  root]
 * 
 * Revision 1.1.6.4  1994/08/03  18:59:49  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:17:42  mccann]
 * 
 * Revision 1.1.6.3  1994/06/23  18:29:15  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:56:02  mccann]
 * 
 * Revision 1.1.6.2  1994/06/09  16:08:02  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:52:47  devsrc]
 * 
 * Revision 1.1.6.1  1994/03/12  21:56:59  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:53:42  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  12:42:47  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:28:50  zeliff]
 * 
 * Revision 1.1.2.2  1992/03/22  21:29:59  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:10:20  weisman]
 * 
 * Revision 1.1  1992/01/19  15:27:05  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module clerkremovereplica.c
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
 *	Clerk.RemoveReplica command processor.
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
 * RemoveReplica arguments (passed in the thread block)
 */
#define	DIRECTORY_P	clerkArgs[0]
#define	CLEARINGHOUSE_P	clerkArgs[1]

/*
 * Local Prototype declarations.
 */

int
RemoveReplicaRPC (thread_t  *,
                  Set_t     *,
                  ObjUID_t  *);

int
buildRemoveReplicaRequest (thread_t *);

static void 
parseRemoveReplicaResponse (thread_t *);

void 
clerkRemoveReplica (thread_t *);


/*
 * Routine to call RPC stub for removereplica
 */
int
RemoveReplicaRPC (thread_t  *th_p,
                  Set_t     *tower_p,
                  ObjUID_t  *uuid_p)
{
    rpc_binding_handle_t  h;
    int i;
    cds_FullName_t directory;
    error_status_t rpcStatus,commStatus = rpc_s_no_bindings;
    cds_status_t  dnsStatus;
    int fromcache, getout=0;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " >RemoveReplicaRPC"));

    /* Set up structures for calling RPC */
    L_to_R_FullName((FullName_t *)th_p->DIRECTORY_P, &directory);

    /*
     * First attempt to find a cached handle. There
     * is a use count associated with handles, so make
     * sure either free or release cached handle is called
     */
    if (fromcache = get_cached_handle(&h, uuid_p, th_p))
      commStatus = cds_RemoveReplica(h, &directory, &dnsStatus);

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
	    commStatus = cds_RemoveReplica(h, &directory, &dnsStatus);
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
           " <RemoveReplicaRPC: return(DNS_SUCCESS)"));

	return(DNS_SUCCESS);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " <RemoveReplicaRPC: return(DNS_NOCOMMUNICATION)"));

    return(DNS_NOCOMMUNICATION);
}

/*
 *	b u i l d R e m o v e R e p l i c a R e q u e s t
 *
 * Routine to build an RemoveReplica request message and
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
buildRemoveReplicaRequest (thread_t *th_p)
{
    int length = 0;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >buildRemoveReplicaRequest"));

    /*
     * RemoveReplicaRequest record:
     *	directory: FullName
     */

    if (th_p->taReqMsgEnd_p) {
	COPY_FullName(th_p->DIRECTORY_P, th_p->taReqMsgEnd_p);
	th_p->taReqMsgEnd_p = SKIP_FullName(th_p->taReqMsgEnd_p);
    }
    length += LEN_FullName(th_p->DIRECTORY_P);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <buildRemoveReplicaRequest: return(length=%d)",
        length));

    return(length);
}

/*
 *	p a r s e R e m o v e R e p l i c a R e s p o n s e
 *
 * This local routine parses the response message.
 */
static void 
parseRemoveReplicaResponse (thread_t *th_p)
{
    static EntryType_t dirType = {ET_directory};
    static EntryType_t childType = {ET_childPointer};


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >parseRemoveReplicaResponse"));
    /*
     * RemoveReplicaResponse record:
     *	(*Nothing*)
     */
    ta_begin(th_p);
    ta_end(th_p);

    /* Remove dns$replicas from cache */
    CARemoveAttribute((FullName_t *)th_p->DIRECTORY_P, &dirType,
  		      dns_AttributeName(DNS_REPLICAS));

    CARemoveAttribute((FullName_t *)th_p->DIRECTORY_P, &childType,
  		      dns_AttributeName(DNS_REPLICAS));

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <parseRemoveReplicaResponse"));
}

/*
 *	c l e r k R e m o v e R e p l i c a
 *
 * This routine removes a replica.
 *
 * Inputs:
 *	DIRECTORY_P	FullName of replica to be removed
 *	CLEARINGHOUSE_P	FullName of clearinghouse of replica
 */
void 
clerkRemoveReplica (thread_t *th_p)
{
    DEB_ASCII_BUF_FullName(directoryBuf)
    DEB_ASCII_BUF_FullName(clearinghouseBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " >clerkRemoveReplica(%s,%s)",
        deb_ascii_FullName(directoryBuf,th_p->DIRECTORY_P),
        deb_ascii_FullName(clearinghouseBuf,th_p->CLEARINGHOUSE_P)));

    th_p->taBuildReq = buildRemoveReplicaRequest;
    th_p->taParseRsp = parseRemoveReplicaResponse;
    th_p->taRPC = RemoveReplicaRPC;

    sendToClearinghouse(th_p, th_p->CLEARINGHOUSE_P);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " <clerkRemoveReplica(%s,%s) status(%s)",
        deb_ascii_FullName(directoryBuf,th_p->DIRECTORY_P),
        deb_ascii_FullName(clearinghouseBuf,th_p->CLEARINGHOUSE_P),
        deb_ascii_dns_status(statusBuf,th_p->clerkStatus)));
}
