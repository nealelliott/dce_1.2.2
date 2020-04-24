/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: clerkenumerateattributes.c,v $
 * Revision 1.1.9.2  1996/02/18  19:26:22  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:09:23  marty]
 *
 * Revision 1.1.9.1  1995/12/08  15:04:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:38:28  root]
 * 
 * Revision 1.1.7.4  1994/08/03  18:59:39  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:22:16  mccann]
 * 
 * Revision 1.1.7.3  1994/06/23  18:29:06  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:55:17  mccann]
 * 
 * Revision 1.1.7.2  1994/06/09  16:07:53  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:52:37  devsrc]
 * 
 * Revision 1.1.7.1  1994/03/12  21:56:44  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:52:42  peckham]
 * 
 * Revision 1.1.4.3  1993/03/09  19:44:27  peckham
 * 	OT#7411: Request given to gda when other clearinghouse
 * 	available. Requests using ET_dirOrObj were not setting
 * 	cache_the_dir, causing WalkTree not to match the cellname.
 * 	[1993/03/09  19:37:55  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  12:42:07  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:27:55  zeliff]
 * 
 * Revision 1.1.2.2  1992/03/22  21:25:09  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:09:00  weisman]
 * 
 * Revision 1.1  1992/01/19  15:27:19  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module clerkenumerateattributes.c
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
 *	Clerk.EnumerateAttributes command processor.
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
 * EnumerateAttributes arguments (passed in the thread block)
 */
#define	ENTRY_P		clerkArgs[0]
#define	LOOKINGFOR_P	clerkArgs[1]
#define	CONTEXTVARIABLE_P clerkArgs[2]
#define	OUTATTRIBUTESET_P clerkArgs[3]
#define	RET_P		clerkArgs[4]

/*
 * Local Prototype declarations.
 */
                                                             
static int 
EnumerateAttributesRPC (thread_t  *,
                        Set_t     *,
                        ObjUID_t  *);

int 
buildEnumerateAttributesRequest (thread_t *);

static void 
parseEnumerateAttributesResponse (thread_t *);

void 
clerkEnumerateAttributes (thread_t *);




/*
 *
 * Routine to call RPC stub for EnumerateAttributes request
 *
 */
static int 
EnumerateAttributesRPC (thread_t  *th_p,
                        Set_t     *tower_p,
                        ObjUID_t  *uuid_p)
{
    rpc_binding_handle_t  h;
    unsigned long max_size = prm_out_length(th_p->OUTATTRIBUTESET_P);
    unsigned long max_len;
    int i;
    cds_Name_t attname;
    cds_Progress_t Progress;
    error_status_t rpcStatus,commStatus = rpc_s_no_bindings;
    cds_status_t  dnsStatus;
    cds_Set_t *valueset_p = (cds_Set_t *)NULL;
    byte_t wholeset;
    int fromcache, getout = 0;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " >EnumerateAttributesRPC"));

    /* Set up structures for calling RPC */
    Build_R_Progress((byte_t *)th_p->taProgress.flags,
		     (Timeout_t *)th_p->taProgress.Timeout,
		     th_p->taProgress.Unresolved,
		     th_p->taProgress.Resolved,
		     &Progress);
    L_to_R_AttributeName((AttributeName_t *)th_p->CONTEXTVARIABLE_P, &attname);

    /*
     * Determine maximum buffer size for clerk.
     * Adjust size by out parameter overhead of 6 bytes each +
     * size of output buffers
     */
    max_len = th_p->clerkRspBufLen - ENUM_READ_OVERHEAD;
    if (max_size > max_len)
      max_size = max_len;

    /*
     * First attempt to find a cached handle.  This
     * assumes their is one handle/clearinghouse.  There
     * is a use count associated with handles, so make
     * sure either free or release cached handle is called
     */
    if (fromcache = get_cached_handle(&h, uuid_p, th_p))
      commStatus = cds_EnumerateAttributes(h, &Progress,
				 EXT8(th_p->LOOKINGFOR_P), &attname, max_size,
				 &valueset_p, &wholeset, &dnsStatus);

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
	    commStatus = cds_EnumerateAttributes(h, &Progress,
				 EXT8(th_p->LOOKINGFOR_P), &attname, max_size,
				 &valueset_p, &wholeset, &dnsStatus);
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
		th_p->taRspMsgPtr_p = R_to_L_Set(valueset_p,
				 (Set_t *)th_p->taRspMsgPtr_p, FALSE);
		R_Set_Free(valueset_p);
		PUT8(th_p->taRspMsgPtr_p, wholeset);
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
            " <EnumerateAttributesRPC: return(DNS_SUCCESS)"));

	return(DNS_SUCCESS);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " <EnumerateAttributesRPC: return(DNS_NOCOMMUNICATION)"));

    return(DNS_NOCOMMUNICATION);
}


/*
 *	b u i l d E n u m e r a t e A t t r i b u t e s R e q u e s t
 *
 * Routine to build an EnumerateAttributes request message and
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
buildEnumerateAttributesRequest (thread_t *th_p)
{
    int length = 0;
    int max_size = prm_out_length(th_p->OUTATTRIBUTESET_P);
    int max_len;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >buildEnumerateAttributesRequest"));

    /*
     * Determine maximum buffer size for clerk.
     * Adjust size by out parmeter overhead of 6 bytes each +
     * size of outlinked and size of wholeentry
     */
    max_len = th_p->clerkRspBufLen - ENUM_READ_OVERHEAD;
    if (max_size > max_len)
      max_size = max_len;


    /*
     * EnumerateAttributesRequest message:
     *	progress: Progress
     *  entryType: EntryType
     *	context: SimpleName
     *	max_size: INTEGER
     */
    length += buildProgress(th_p);
    length += LEN_EntryType(th_p->LOOKINGFOR_P);
    length += LEN_SimpleName(th_p->CONTEXTVARIABLE_P);
    length += LEN_INTEGER(&max_size);

    if (th_p->taReqMsgEnd_p) {
        SimpleName_t *const sn_p = (SimpleName_t *)th_p->taReqMsgEnd_p;

	COPY_EntryType(th_p->LOOKINGFOR_P, th_p->taReqMsgEnd_p);
	th_p->taReqMsgEnd_p = SKIP_EntryType(th_p->taReqMsgEnd_p);

	COPY_SimpleName(th_p->CONTEXTVARIABLE_P, th_p->taReqMsgEnd_p);
	/*
	 * V1.0 parses binary before null so change nulls to binary
	 * to ensure all names are enumerated
	 */
	if (EXT8(sn_p->sn_flag)==SN_null)
	    INS8(sn_p->sn_flag, SN_binary);

	th_p->taReqMsgEnd_p = SKIP_SimpleName(th_p->taReqMsgEnd_p);

	COPY_INTEGER(&max_size, th_p->taReqMsgEnd_p);
	th_p->taReqMsgEnd_p = SKIP_INTEGER(th_p->taReqMsgEnd_p);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <buildEnumerateAttributesRequest: return(length=%d)",
        length));

    return(length);
}


/*
 *	p a r s e E n u m e r a t e A t t r i b u t e s R e s p o n s e
 *
 * This local routine parses the response message.
 */
static void 
parseEnumerateAttributesResponse (thread_t *th_p)
{
    byte_t	*ptr_p;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >parseEnumerateAttributesResponse"));

    /*
     * EnumerateAttributesResponse message:
     *	progress: Progress
     *	result: Set
     *	wholeSet: BOOLEAN
     */
    ta_begin(th_p);
    ta_Progress(th_p);
    ptr_p = th_p->clerkRspMsgEnd_p;
    ta_Set(th_p, th_p->OUTATTRIBUTESET_P);
    ta_BOOLEAN(th_p, th_p->RET_P);
    ta_end(th_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <parseEnumerateAttributesResponse"));
}


/*
 *	c l e r k E n u m e r a t e A t t r i b u t e s
 *
 * Enumerate attributes -- Read a set of attributes of an entry
 *
 * Inputs:
 *	ENTRY_P		FullName of entry which will be enumerated
 *	LOOKINGFOR_P	EntryType type of entry (directory, etc)
 *	CONTEXTVARIABLE_P SimpleName starting point for enumeration
 *
 * Outputs:
 *	OUTATTRIBUTESET_P Set of AttributeSpecifiers
 *
 * Returns:
 *	RET_P		BOOLEAN
 */
void 
clerkEnumerateAttributes (thread_t *th_p)
{
    DEB_ASCII_BUF_FullName(entryBuf)
    DEB_ASCII_BUF_EntryType(lookingForBuf)
    DEB_ASCII_BUF_SimpleName(contextVariableBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " >clerkEnumerateAttributes(%s,%s,%s,%p,%p)",
        deb_ascii_FullName(entryBuf,th_p->ENTRY_P),
        deb_ascii_EntryType(lookingForBuf,th_p->LOOKINGFOR_P),
        deb_ascii_SimpleName(contextVariableBuf,th_p->CONTEXTVARIABLE_P),
        th_p->OUTATTRIBUTESET_P,
        th_p->RET_P));

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

    th_p->taBuildReq = buildEnumerateAttributesRequest;
    th_p->taParseRsp = parseEnumerateAttributesResponse;
    th_p->taRPC = EnumerateAttributesRPC;

    sendToWalkTree(th_p, th_p->ENTRY_P);

    if (th_p->clerkStatus == DNS_TIMEOUTMAYBEDONE)
	th_p->clerkStatus = DNS_TIMEOUTNOTDONE;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " <clerkEnumerateAttributes(%s,%s,%s,%p,%p) status(%s)",
        deb_ascii_FullName(entryBuf,th_p->ENTRY_P),
        deb_ascii_EntryType(lookingForBuf,th_p->LOOKINGFOR_P),
        deb_ascii_SimpleName(contextVariableBuf,th_p->CONTEXTVARIABLE_P),
        th_p->OUTATTRIBUTESET_P,
        th_p->RET_P,
        deb_ascii_dns_status(statusBuf,th_p->clerkStatus)));
}
