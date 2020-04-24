/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: clerknewepoch.c,v $
 * Revision 1.1.8.2  1996/02/18  19:26:37  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:09:30  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:05:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:38:39  root]
 * 
 * Revision 1.1.6.4  1994/08/03  18:59:46  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:17:57  mccann]
 * 
 * Revision 1.1.6.3  1994/06/23  18:29:11  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:55:42  mccann]
 * 
 * Revision 1.1.6.2  1994/06/09  16:07:59  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:52:44  devsrc]
 * 
 * Revision 1.1.6.1  1994/03/12  21:56:52  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:53:17  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  12:42:34  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:28:29  zeliff]
 * 
 * Revision 1.1.2.2  1992/03/22  21:28:10  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:09:50  weisman]
 * 
 * Revision 1.1  1992/01/19  15:27:02  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module clerknewepoch.c
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
 *	Clerk.NewEpoch command processor.
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
#include <dce/attributes.h>

/*
 * NewEpoch arguments (passed in the thread block)
 */
#define	DIRECTORY_P	clerkArgs[0]
#define	MASTERCH_P	clerkArgs[1]
#define	SECONDARYCHS_P	clerkArgs[2]
#define	READONLYCHS_P	clerkArgs[3]

/*
 * Local Prototype declarations.
 */

int 
NewEpochRPC (thread_t  *,
             Set_t     *,
             ObjUID_t  *);

int 
buildNewEpochRequest (thread_t *);

static void 
parseNewEpochResponse (thread_t *);

void 
clerkNewEpoch (thread_t *);



/*
 * Routine to call RPC stub for NewEpoch request
 */
int 
NewEpochRPC (thread_t  *th_p,
             Set_t     *tower_p,
             ObjUID_t  *uuid_p)
{
    rpc_binding_handle_t  h;
    int i;
    cds_FullName_t directory;
    cds_Set_t *secondaryCHs, *readOnlyCHs;
    error_status_t rpcStatus,commStatus = rpc_s_no_bindings;
    cds_status_t  dnsStatus;
    int fromcache, getout=0;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " >NewEpochRPC"));

    /* Set up structures for calling RPC */
    L_to_R_FullName((FullName_t *)th_p->DIRECTORY_P, &directory);
    if (L_to_R_Set((Set_t *)th_p->SECONDARYCHS_P, &secondaryCHs, &dnsStatus)
	== NULL)
    {
	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_child,
            svc_c_debug4,
            " <NewEpochRPC: (1) return(DNS_RESOURCEERROR)"));

	return(DNS_RESOURCEERROR);
    }
    

    if (L_to_R_Set((Set_t *)th_p->READONLYCHS_P, &readOnlyCHs, &dnsStatus)
	== NULL) 
    {
	R_Set_Free(secondaryCHs);

	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_child,
            svc_c_debug4,
            " <NewEpochRPC: (2) return(DNS_RESOURCEERROR)"));

	return(DNS_RESOURCEERROR);
    }

    /*
     * First attempt to find a cached handle. There
     * is a use count associated with handles, so make
     * sure either free or release cached handle is called
     */
    if (fromcache = get_cached_handle(&h, uuid_p, th_p))
      commStatus = cds_NewEpoch(h, &directory, readOnlyCHs,
				secondaryCHs, &dnsStatus);

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
	    commStatus = cds_NewEpoch(h, &directory, readOnlyCHs,
				      secondaryCHs, &dnsStatus);
	    if (commStatus == rpc_s_ok) break;
	    maximize_rpc_error(th_p, commStatus, &getout);
	    rpc_binding_free(&h, &rpcStatus);
	    if (getout) break;
	} /* End For loop */
    }

    R_Set_Free(secondaryCHs);
    R_Set_Free(readOnlyCHs);

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
            " <NewEpochRPC: return(DNS_SUCCESS)"));

	return(DNS_SUCCESS);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " <NewEpochRPC: return(DNS_RESOURCEERROR)"));

    return(DNS_NOCOMMUNICATION);
}

/*
 *	b u i l d N e w E p o c h R e q u e s t
 *
 * Routine to build an NewEpoch request message and
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
buildNewEpochRequest (thread_t *th_p)
{
    int length = 0;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >buildNewEpochRequest"));

    /*
     * NewEpochRequest record:
     *	directory: FullName
     *	secondaryCH: Set	(* of FullNames of clearinghouses *)
     *	readOnlyCH: Set		(* of FullNames of clearinghouses *)
     */
    if (th_p->taReqMsgEnd_p) {
	COPY_FullName(th_p->DIRECTORY_P, th_p->taReqMsgEnd_p);
	th_p->taReqMsgEnd_p = SKIP_FullName(th_p->taReqMsgEnd_p);
    }
    length += LEN_FullName(th_p->DIRECTORY_P);

    if (th_p->taReqMsgEnd_p) {
	COPY_Set(th_p->SECONDARYCHS_P, th_p->taReqMsgEnd_p);
	th_p->taReqMsgEnd_p = SKIP_Set(th_p->taReqMsgEnd_p);
    }
    length += LEN_Set(th_p->SECONDARYCHS_P);

    if (th_p->taReqMsgEnd_p) {
	COPY_Set(th_p->READONLYCHS_P, th_p->taReqMsgEnd_p);
	th_p->taReqMsgEnd_p = SKIP_Set(th_p->taReqMsgEnd_p);
    }
    length += LEN_Set(th_p->READONLYCHS_P);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <buildNewEpochRequest: return(length=%d)",
	length));

    return(length);
}

/*
 *	p a r s e N e w E p o c h R e s p o n s e
 *
 * This local routine parses the response message.
 */
static void 
parseNewEpochResponse (thread_t *th_p)
{
    static EntryType_t dirType = {ET_directory};
    static EntryType_t childType = {ET_childPointer};
    

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >parseNewEpochResponse"));
    /*
     * NewEpochResponse record:
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
        " <parseNewEpochResponse"));
}

/*
 *	c l e r k N e w E p o c h
 *
 * Recover clearinghouse
 *
 * Inputs:
 *	DIRECTORY_P	FullName of directory to reconstruct
 *	MASTERCH_P	FullName of master clearinghouse
 *	SECONDARYCHS_P	Set of FullNames of secondary clearinghouses
 *	READONLYCHS_P	Set of FullNames of read-only clearinghouses
 */
void 
clerkNewEpoch (thread_t *th_p)
{
    DEB_ASCII_BUF_FullName(directoryBuf)
    DEB_ASCII_BUF_FullName(masterCHBuf)
    DEB_ASCII_BUF_Set(secondaryCHsBuf)
    DEB_ASCII_BUF_Set(readOnlyCHsBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " >clerkNewEpoch(%s,%s,%s,%s)",
        deb_ascii_FullName(directoryBuf,th_p->DIRECTORY_P),
        deb_ascii_FullName(masterCHBuf,th_p->MASTERCH_P),
        deb_ascii_Set(secondaryCHsBuf,th_p->SECONDARYCHS_P),
        deb_ascii_Set(readOnlyCHsBuf,th_p->READONLYCHS_P)));

    th_p->taBuildReq = buildNewEpochRequest;
    th_p->taParseRsp = parseNewEpochResponse;
    th_p->taRPC = NewEpochRPC;

    sendToClearinghouse(th_p, th_p->MASTERCH_P);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " <clerkNewEpoch(%s,%s,%s,%s) status(%s)",
        deb_ascii_FullName(directoryBuf,th_p->DIRECTORY_P),
        deb_ascii_FullName(masterCHBuf,th_p->MASTERCH_P),
        deb_ascii_Set(secondaryCHsBuf,th_p->SECONDARYCHS_P),
        deb_ascii_Set(readOnlyCHsBuf,th_p->READONLYCHS_P),
        deb_ascii_dns_status(statusBuf,th_p->clerkStatus)));
}
