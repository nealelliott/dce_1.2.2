/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: clerktestattribute.c,v $
 * Revision 1.1.9.2  1996/02/18  19:26:54  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:09:37  marty]
 *
 * Revision 1.1.9.1  1995/12/08  15:05:33  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:38:51  root]
 * 
 * Revision 1.1.7.5  1994/08/03  18:59:53  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:17:33  mccann]
 * 
 * Revision 1.1.7.4  1994/06/23  18:29:17  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:56:16  mccann]
 * 
 * Revision 1.1.7.3  1994/06/09  16:08:05  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:52:50  devsrc]
 * 
 * Revision 1.1.7.2  1994/04/14  14:41:17  peckham
 * 	New module clerk_socket.c to isolate SVR4 named pipes.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	Remove msgblk.h, cds_event_init() and evp references.
 * 	Remove v1v2 code (v1 never used for cds).
 * 	Remove VMS and UNIX conditionals, VMS code.
 * 	Remove last vestige of clerk_trace().
 * 	[1994/04/11  19:36:44  peckham]
 * 
 * Revision 1.1.7.1  1994/03/12  21:57:05  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:54:03  peckham]
 * 
 * Revision 1.1.4.3  1993/03/09  19:44:57  peckham
 * 	OT#7411: Request given to gda when other clearinghouse
 * 	available. Requests using ET_dirOrObj were not setting
 * 	cache_the_dir, causing WalkTree not to match the cellname.
 * 	[1993/03/09  19:41:26  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  12:42:59  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:29:04  zeliff]
 * 
 * Revision 1.1.2.2  1992/06/17  18:31:19  keegan
 * 	Cache binding handle to server in 'TestAttribute' call.
 * 	[1992/06/17  17:44:14  keegan]
 * 
 * Revision 1.1  1992/01/19  15:27:23  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module clerktestattribute.c
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
 *	Clerk.TestAttribute command processor.
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
 * TestAttribute arguments (passed in the thread block)
 */
#define	ENTRY_P		clerkArgs[0]
#define	LOOKINGFOR_P	clerkArgs[1]
#define	ATTRIBUTE_P	clerkArgs[2]
#define	VALUE_P		clerkArgs[3]
#define	RET_P		clerkArgs[4]


/*
 * Local Prototype declarations.
 */

static int 
TestAttributeRPC (thread_t  *,
                  Set_t     *,
                  ObjUID_t  *);

int 
buildTestAttributeRequest (thread_t *);

static void 
parseTestAttributeResponse (thread_t *);



void 
clerkTestAttribute (thread_t *);



/*
 * Routine to call RPC stub for TestAttribute request
 */
static int 
TestAttributeRPC (thread_t  *th_p,
                  Set_t     *tower_p,
                  ObjUID_t  *uuid_p)
{
    rpc_binding_handle_t  h;
    int i;
    cds_Progress_t Progress;
    cds_AtomicValue_t value;
    cds_Name_t attrname;
    unsigned char result;
    error_status_t rpcStatus,commStatus = rpc_s_no_bindings;
    cds_status_t  dnsStatus;
    int fromcache, getout=0;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " >TestAttributeRPC"));

    /* Set up structures for calling RPC */
    Build_R_Progress((byte_t *)th_p->taProgress.flags,
		     (Timeout_t *)th_p->taProgress.Timeout,
		     th_p->taProgress.Unresolved,
		     th_p->taProgress.Resolved,
		     &Progress);
    /* Fails if no memory */
    if (!L_to_R_AtomicValue((AtomicValue_t *)th_p->VALUE_P, &value,&dnsStatus))
    {
	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_child,
            svc_c_debug4,
            " <TestAttributeRPC: return(DNS_RESOURCEERROR)"));

	return(DNS_RESOURCEERROR);
    }
    

    L_to_R_AttributeName((AttributeName_t *)th_p->ATTRIBUTE_P, &attrname);

    /*
     * First attempt to find a cached handle. There
     * is a use count associated with handles, so make
     * sure either free or release cached handle is called
     */
    if (fromcache = get_cached_handle(&h, uuid_p, th_p))
      commStatus = cds_TestAttribute(h, &Progress,
				     EXT8(th_p->LOOKINGFOR_P), &attrname,
				     &value, &result, &dnsStatus);

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
	    commStatus = cds_TestAttribute(h, &Progress,
					   EXT8(th_p->LOOKINGFOR_P),&attrname,
					   &value, &result, &dnsStatus);
	    if (commStatus == rpc_s_ok) break;
	    maximize_rpc_error(th_p, commStatus, &getout);
	    rpc_binding_free(&h, &rpcStatus);
	    if (getout) break;
	} /* End For loop */
    }

    R_AtomicValue_Free(&value);

    if (commStatus == rpc_s_ok) {
	th_p->taRspMsgPtr_p = th_p->taRspMsg_p;
	if (dnsStatus.er_status == PDU_Success) {
	    th_p->taRspType = MT_Response;
	    th_p->taRspMsgPtr_p = R_to_L_Progress(&Progress,
			(Progress_t *)th_p->taRspMsgPtr_p, TRUE);
	    if (FLAG_IS_SET(&Progress.pr_flags, PR_done))
	      PUT8(th_p->taRspMsgPtr_p, result);
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
           " <TestAttributeRPC: return (DNS_SUCCESS)"));

	return(DNS_SUCCESS);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " <TestAttributeRPC: return (DNS_NOCOMMUNICATION)"));

    return (DNS_NOCOMMUNICATION);
}



/*
 *	b u i l d T e s t A t t r i b u t e R e q u e s t
 *
 * Routine to build an TestAttribute request message and
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
buildTestAttributeRequest (thread_t *th_p)
{
    int length = 0;
    int valueLen;
    AtomicValue_u at;
    AtomicValue_t *in_p,*out_p;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >buildTestAttributeRequest"));

    in_p = (AtomicValue_t *)th_p->VALUE_P;
    /*
     * TestAttributeRequest message:
     *  progress: Progress
     *	entryType: EntryType
     *	attribute: AttributeName
     *	value: AtomicValue
     */
    length += buildProgress(th_p);
    length += LEN_EntryType(th_p->LOOKINGFOR_P);
    length += LEN_AttributeName(th_p->ATTRIBUTE_P);
    valueLen = LEN_AtomicValue(in_p);
    length += valueLen;

    if (th_p->taReqMsgEnd_p) {
	COPY_EntryType(th_p->LOOKINGFOR_P, th_p->taReqMsgEnd_p);
	th_p->taReqMsgEnd_p = SKIP_EntryType(th_p->taReqMsgEnd_p);

	COPY_AttributeName(th_p->ATTRIBUTE_P, th_p->taReqMsgEnd_p);
	th_p->taReqMsgEnd_p = SKIP_AttributeName(th_p->taReqMsgEnd_p);
	COPY_bytes(in_p, th_p->taReqMsgEnd_p, valueLen);
	th_p->taReqMsgEnd_p += valueLen;
      }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <buildTestAttributeRequest: return(length=%d)",
        length));

    return(length);
}


/*
 *	p a r s e T e s t A t t r i b u t e R e s p o n s e
 *
 * This local routine parses the response message.
 */
static void 
parseTestAttributeResponse (thread_t *th_p)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >parseTestAttributeResponse"));

    /*
     * TestAttributeResponse message:
     *  progress: Progress
     *	result: BOOLEAN
     */
    ta_begin(th_p);
    ta_Progress(th_p);
    if (th_p->taRspMsgPtr_p && (*th_p->taRspMsgPtr_p == 0)) {
      th_p->clerkStatus = DNS_NOTFND;
    } else {
      th_p->clerkStatus = DNS_SUCCESS;
    }
    ta_BOOLEAN(th_p, th_p->RET_P);
    ta_end(th_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <parseTestAttributeResponse"));
}



/*
 *	c l e r k T e s t A t t r i b u t e C a c h e
 *
 * This routine tests the value of the stored attribute for a match with
 * the specified value
 *
 * Inputs:
 *	ENTRY_P		FullName of entry to be accessed
 *	LOOKINGFOR_P	EntryType of entry to check attribute for
 *	ATTRIBUTE_P	AttributeName of attribute to check
 *	VALUE_P		AtomicValue of value to test against
 *
 * Outputs:
 *
 * Returns:
 *	RET_P		BOOLEAN
 */
/* not used with valuetypes, cause requires duplication of server code */



/*
 *	c l e r k T e s t A t t r i b u t e
 *
 * This routine tests the value of the stored attribute for a match with
 * the specified value
 *
 * Inputs:
 *	ENTRY_P		FullName of entry to be accessed
 *	LOOKINGFOR_P	EntryType of entry to check attribute for
 *	ATTRIBUTE_P	AttributeName of attribute to check
 *	VALUE_P		AtomicValue of value to test against
 *
 * Outputs:
 *
 * Returns:
 *	RET_P		BOOLEAN
 */
void 
clerkTestAttribute (thread_t *th_p)
{
    DEB_ASCII_BUF_FullName(entryBuf)
    DEB_ASCII_BUF_EntryType(lookingForBuf)
    DEB_ASCII_BUF_AttributeName(attributeBuf)
    DEB_ASCII_BUF_AtomicValue(valueBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " >clerkTestAttribute(%s,%s,%s,%s,%p)",
        deb_ascii_FullName(entryBuf,th_p->ENTRY_P),
        deb_ascii_EntryType(lookingForBuf,th_p->LOOKINGFOR_P),
        deb_ascii_AttributeName(attributeBuf,th_p->ATTRIBUTE_P),
        deb_ascii_AtomicValue(valueBuf,th_p->VALUE_P),
        th_p->RET_P));

      {
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

	th_p->taRPC = TestAttributeRPC;
        th_p->taBuildReq = buildTestAttributeRequest;
        th_p->taParseRsp = parseTestAttributeResponse;

        sendToWalkTree(th_p, th_p->ENTRY_P);

        if (th_p->clerkStatus == DNS_TIMEOUTMAYBEDONE)
	  th_p->clerkStatus = DNS_TIMEOUTNOTDONE;

      } /* End cache miss */

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " <clerkTestAttribute(%s,%s,%s,%s,%p) status(%s)",
        deb_ascii_FullName(entryBuf,th_p->ENTRY_P),
        deb_ascii_EntryType(lookingForBuf,th_p->LOOKINGFOR_P),
        deb_ascii_AttributeName(attributeBuf,th_p->ATTRIBUTE_P),
        deb_ascii_AtomicValue(valueBuf,th_p->VALUE_P),
        th_p->RET_P,
        deb_ascii_dns_status(statusBuf,th_p->clerkStatus)));
}
