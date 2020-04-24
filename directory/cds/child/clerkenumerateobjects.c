/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: clerkenumerateobjects.c,v $
 * Revision 1.1.8.2  1996/02/18  19:26:29  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:09:25  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:05:04  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:38:33  root]
 * 
 * Revision 1.1.6.4  1994/08/03  18:59:41  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:22:23  mccann]
 * 
 * Revision 1.1.6.3  1994/06/23  18:29:08  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:55:27  mccann]
 * 
 * Revision 1.1.6.2  1994/06/09  16:07:55  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:52:40  devsrc]
 * 
 * Revision 1.1.6.1  1994/03/12  21:56:47  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:52:56  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  12:42:18  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:28:14  zeliff]
 * 
 * Revision 1.1.2.2  1992/03/22  21:26:23  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:09:20  weisman]
 * 
 * Revision 1.1  1992/01/19  15:27:00  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module clerkenumerateobjects.c
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
 *	Clerk.EnumerateObjects command processor.
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
 * EnumerateObjects arguments (passed in the thread block)
 */
#define	DIRECTORY_P	clerkArgs[0]
#define	WILDCARD_P	clerkArgs[1]
#define	CLASSFILTER_P	clerkArgs[2]
#define	CONTEXTVARIABLE_P clerkArgs[3]
#define	INOUTRETURNCLASS_P clerkArgs[4]
#define	OUTOBJECTS_P	clerkArgs[5]
#define	RET_P		clerkArgs[6]

/*
 * Local Prototype declarations.
 */

static int 
EnumerateObjectsRPC (thread_t  *,
                     Set_t     *,
                     ObjUID_t  *);

int 
buildEnumerateObjectsRequest (thread_t *);

static void 
parseEnumerateObjectsResponse (thread_t *);

void 
clerkEnumerateObjects (thread_t *);




/*
 *
 * Routine to call RPC stub for EnumerateObjects request
 *
 */
static int 
EnumerateObjectsRPC (thread_t  *th_p,
                     Set_t     *tower_p,
                     ObjUID_t  *uuid_p)
{
    rpc_binding_handle_t  h;
    unsigned long max_size = prm_out_length(th_p->OUTOBJECTS_P);
    unsigned long max_len;
    int i;
    cds_Name_t classname;
    cds_Name_t wildcard,context;
    cds_Progress_t Progress;
    error_status_t rpcStatus,commStatus = rpc_s_no_bindings;
    cds_status_t  dnsStatus;
    cds_Set_t *valueset_p = (cds_Set_t *)NULL;
    byte_t wholeset,returnClass;
    int fromcache, getout=0;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " >EnumerateObjectsRPC"));

    /* Set up structures for calling RPC */
    Build_R_Progress((byte_t *)th_p->taProgress.flags,
		     (Timeout_t *)th_p->taProgress.Timeout,
		     th_p->taProgress.Unresolved,
		     th_p->taProgress.Resolved,
		     &Progress);
    L_to_R_ClassName((ClassName_t *)th_p->CLASSFILTER_P, &classname);
    L_to_R_SimpleName((SimpleName_t *)th_p->CONTEXTVARIABLE_P, &context);
    L_to_R_SimpleName((SimpleName_t *)th_p->WILDCARD_P, &wildcard);
    returnClass = EXT8(th_p->INOUTRETURNCLASS_P);

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
	commStatus = cds_EnumerateObjects(h, &Progress,
			  &wildcard, &context, &classname, max_size,
			  &valueset_p, &wholeset, &returnClass, &dnsStatus);

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
	    commStatus = cds_EnumerateObjects(h, &Progress,
			  &wildcard, &context, &classname, max_size,
			  &valueset_p, &wholeset, &returnClass, &dnsStatus);
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
		PUT8(th_p->taRspMsgPtr_p, returnClass);
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
            " <EnumerateObjectsRPC: return(DNS_SUCCESS)"));

	return(DNS_SUCCESS);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " <EnumerateObjectsRPC: return(DNS_NOCOMMUNICATION)"));

    return(DNS_NOCOMMUNICATION);
}

/*
 *	b u i l d E n u m e r a t e O b j e c t s R e q u e s t
 *
 * Routine to build an EnumerateObjects request message and
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
buildEnumerateObjectsRequest (thread_t *th_p)
{
    int length = 0;
    int max_size = prm_out_length(th_p->OUTOBJECTS_P);
    int max_len;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >buildEnumerateObjectsRequest"));

    /*
     * Determine maximum buffer size for clerk.
     * Adjust size by out parmeter overhead of 6 bytes each +
     * size of outlinked and size of wholeentry
     */
    max_len = th_p->clerkRspBufLen - ENUM_READ_OVERHEAD;
    if (max_size > max_len)
      max_size = max_len;

    /*
     * EnumerateObjectsRequest record:
     *	progress: Progress
     *	wildcard: SimpleName
     *	class: ClassName
     *	context: SimpleName
     *	max_size: INTEGER
     *	returnClass: BOOLEAN
     */
    length += buildProgress(th_p);

    if (th_p->taReqMsgEnd_p) {
	COPY_SimpleName(th_p->WILDCARD_P, th_p->taReqMsgEnd_p);
	th_p->taReqMsgEnd_p = SKIP_SimpleName(th_p->taReqMsgEnd_p);
    }
    length += LEN_SimpleName(th_p->WILDCARD_P);

    if (th_p->taReqMsgEnd_p) {
	COPY_ClassName(th_p->CLASSFILTER_P, th_p->taReqMsgEnd_p);
	th_p->taReqMsgEnd_p = SKIP_ClassName(th_p->taReqMsgEnd_p);
    }
    length += LEN_ClassName(th_p->CLASSFILTER_P);

    if (th_p->taReqMsgEnd_p) {
	COPY_SimpleName(th_p->CONTEXTVARIABLE_P, th_p->taReqMsgEnd_p);
	th_p->taReqMsgEnd_p = SKIP_SimpleName(th_p->taReqMsgEnd_p);
    }
    length += LEN_SimpleName(th_p->CONTEXTVARIABLE_P);

    if (th_p->taReqMsgEnd_p) {
	COPY_INTEGER(&max_size, th_p->taReqMsgEnd_p);
	th_p->taReqMsgEnd_p = SKIP_INTEGER(th_p->taReqMsgEnd_p);
    }
    length += LEN_INTEGER(&max_size);

    if (th_p->taReqMsgEnd_p) {
      COPY_BOOLEAN(th_p->INOUTRETURNCLASS_P, th_p->taReqMsgEnd_p);
      th_p->taReqMsgEnd_p = SKIP_BOOLEAN(th_p->taReqMsgEnd_p);
    }
    length += LEN_BOOLEAN(th_p->INOUTRETURNCLASS_P);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >buildEnumerateObjectsRequest: return(length=%d)",
        length));

    return(length);
}

/*
 *	p a r s e E n u m e r a t e O b j e c t s R e s p o n s e
 *
 * This local routine parses the response message.
 */
static void 
parseEnumerateObjectsResponse (thread_t *th_p)
{
    static BOOLEAN_u noClass = {BL_false};


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >parseEnumerateObjectsResponse"));

    /*
     * EnumerateObjectsResponse record:
     *	progress: Progress
     *	result: Set
     *	wholeSet: BOOLEAN
     *	returnClass: BOOLEAN
     */
    ta_begin(th_p);
    ta_Progress(th_p);
    ta_Set(th_p, th_p->OUTOBJECTS_P);
    ta_BOOLEAN(th_p, th_p->RET_P);

    ta_BOOLEAN(th_p, th_p->INOUTRETURNCLASS_P);

    ta_end(th_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <parseEnumerateObjectsResponse"));
}

/*
 *	c l e r k E n u m e r a t e O b j e c t s
 *
 * Enumerate objects -- perhaps one of the most intricate calls
 *
 * Inputs:
 *	DIRECTORY_P	FullName of entry which will be enumerated
 *	WILDCARD_P	SimpleName
 *	CLASSFILTER_P	ClassName
 *	CONTEXTVARIABLE_P SimpleName starting point for enumeration
 *	INOUTRETURNCLASS_P BOOLEAN
 *
 * Outputs:
 *	INOUTRETURNCLASS_P BOOLEAN
 *	OUTOBJECTS_P	Set of SimpleName
 *
 * Returns:
 *	RET_P		BOOLEAN
 */
void 
clerkEnumerateObjects (thread_t *th_p)
{
    DEB_ASCII_BUF_FullName(directoryBuf)
    DEB_ASCII_BUF_SimpleName(wildcardBuf)
    DEB_ASCII_BUF_ClassName(classFilterBuf)
    DEB_ASCII_BUF_SimpleName(contextVariableBuf)
    DEB_ASCII_BUF_BOOLEAN(inoutReturnClassBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " >clerkEnumerateObjects(%s,%s,%s.%s,%s,%p,%p)",
        deb_ascii_FullName(directoryBuf,th_p->DIRECTORY_P),
        deb_ascii_SimpleName(wildcardBuf,th_p->WILDCARD_P),
        deb_ascii_ClassName(classFilterBuf,th_p->CLASSFILTER_P),
        deb_ascii_SimpleName(contextVariableBuf,th_p->CONTEXTVARIABLE_P),
        deb_ascii_BOOLEAN(inoutReturnClassBuf,th_p->INOUTRETURNCLASS_P),
        th_p->OUTOBJECTS_P,
        th_p->RET_P));

    /*
     * The final progress record will contain a directory name -- cache it.
     */
    th_p->cache_the_dir = 1;

    th_p->taBuildReq = buildEnumerateObjectsRequest;
    th_p->taParseRsp = parseEnumerateObjectsResponse;
    th_p->taRPC = EnumerateObjectsRPC;

    sendToWalkTree(th_p, th_p->DIRECTORY_P);

    if (th_p->clerkStatus == DNS_TIMEOUTMAYBEDONE)
	th_p->clerkStatus = DNS_TIMEOUTNOTDONE;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " <clerkEnumerateObjects(%s,%s,%s.%s,%s,%p,%p) status(%s)",
        deb_ascii_FullName(directoryBuf,th_p->DIRECTORY_P),
        deb_ascii_SimpleName(wildcardBuf,th_p->WILDCARD_P),
        deb_ascii_ClassName(classFilterBuf,th_p->CLASSFILTER_P),
        deb_ascii_SimpleName(contextVariableBuf,th_p->CONTEXTVARIABLE_P),
        deb_ascii_BOOLEAN(inoutReturnClassBuf,th_p->INOUTRETURNCLASS_P),
        th_p->OUTOBJECTS_P,
        th_p->RET_P,
        deb_ascii_dns_status(statusBuf,th_p->clerkStatus)));
}
