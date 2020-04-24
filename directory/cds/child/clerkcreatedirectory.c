/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: clerkcreatedirectory.c,v $
 * Revision 1.1.8.2  1996/02/18  19:26:05  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:09:15  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:04:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:38:14  root]
 * 
 * Revision 1.1.6.4  1994/08/03  18:59:32  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:14:02  mccann]
 * 
 * Revision 1.1.6.3  1994/06/23  18:28:58  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:54:49  mccann]
 * 
 * Revision 1.1.6.2  1994/06/09  16:07:46  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:52:29  devsrc]
 * 
 * Revision 1.1.6.1  1994/03/12  21:56:33  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:51:53  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  12:41:34  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:27:17  zeliff]
 * 
 * Revision 1.1.2.2  1992/03/22  21:23:30  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:08:31  weisman]
 * 
 * Revision 1.1  1992/01/19  15:26:53  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module clerkcreatedirectory.c
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
 *	Clerk.CreateDirectory command processor.
 *
 * Networks & Communications Software Engineering
 */
#include <dce/dce.h>
#include <dce/dcecdsmsg.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <uc_child.h>
#include <uc_clerk.h>
#include <dce/attributes.h>

/*
 * CreateDirectory arguments (passed in the thread block)
 */
#define	DIRECTORY_P	clerkArgs[0]
#define	CLEARINGHOUSE_P	clerkArgs[1]
#define	RET_P		clerkArgs[2]

/*
 * Local Prototype declarations.
 */

int 
buildCreateDirectoryRequest (thread_t *);

static void 
parseCreateDirectoryResponse (thread_t *);

static int 
CreateDirectoryRPC (thread_t  *,
                    Set_t     *,
                    ObjUID_t  *);


/*
 *	b u i l d C r e a t e D i r e c t o r y R e q u e s t
 *
 * Routine to build a CreateDirectory request message and
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
buildCreateDirectoryRequest (thread_t *th_p)
{
    int length = 0;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >buildCreateDirectoryRequest"));

    /*
     * CreateDirectoryRequest message:
     *	newDirectory: FullName
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
        " <buildCreateDirectoryRequest: return(length=%d)",
        length));

    return(length);
}

/*
 *      p a r s e C r e a t e D i r e c t o r y R e s p o n s e
 *
 * This local routine parses the response message.
 */
static void 
parseCreateDirectoryResponse (thread_t *th_p)
{
    /*
     * CreateDirectoryResponse message:
     *	actualID: ObjUID
     */

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >parseCreateDirectoryResponse"));

    ta_begin(th_p);
    ta_ObjUID(th_p, th_p->RET_P);
    ta_end(th_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <parseCreateDirectoryResponse"));
}

/*
 * Routine to call RPC stub for CreateDirectory request
 */
static int 
CreateDirectoryRPC (thread_t  *th_p,
                    Set_t     *tower_p,
                    ObjUID_t  *uuid_p)
{
    rpc_binding_handle_t  h;
    int i;
    cds_Timestamp_t actual_ts;
    cds_Progress_t Progress;
    error_status_t rpcStatus,commStatus = rpc_s_no_bindings;
    cds_status_t  dnsStatus;
    byte_t        flags = 0;
    int           fromcache, getout = 0;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " >CreateDirectoryRPC"));

    /* Set up structures for calling RPC */
    Build_R_Progress((byte_t *)&flags,
		     (Timeout_t *)NullTimeout,
		     (FullName_t *)th_p->DIRECTORY_P,
		     (FullName_t *)NullFullName,
		     &Progress);

    /*
     * First attempt to find a cached handle. There
     * is a use count associated with handles, so make
     * sure either free or release cached handle is called
     */
    if (fromcache = get_cached_handle(&h, uuid_p, th_p))
      commStatus = cds_CreateDirectory(h, &Progress, &actual_ts,
				       &dnsStatus);

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
	    commStatus = cds_CreateDirectory(h, &Progress, &actual_ts,
					     &dnsStatus);
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
	    /* Progress record is ignored here */
	    if (FLAG_IS_SET(&Progress.pr_flags, PR_done)) {
		COPY_ObjUID(&Progress.pr_unresolved.fn_root,
			      th_p->taRspMsgPtr_p);
		if (FLAG_IS_SET(&Progress.pr_flags, PR_linked))
		  th_p->outLinked++;
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
           " <CreateDirectoryRPC: return(DNS_SUCCESS)"));

	return(DNS_SUCCESS);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " <CreateDirectoryRPC: return(DNS_NOCOMMUNICATION)"));

    return(DNS_NOCOMMUNICATION);
}

/*
 *        c l e r k C r e a t e D i r e c t o r y
 *
 * This routine creates a directory.
 *
 * Inputs:
 *	DIRECTORY_P	FullName of directory to be created
 *	CLEARINGHOUSE_P	FullName of clearinghouse for new directory
 *
 * Returns:
 *	RET_P		Timestamp of created directory
 */
void 
clerkCreateDirectory (thread_t *th_p)
{
    DEB_ASCII_BUF_FullName(directoryBuf)
    DEB_ASCII_BUF_FullName(clearinghouseBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " >clerkCreateDirectory(%s,%s,%p)",
        deb_ascii_FullName(directoryBuf,th_p->DIRECTORY_P),
        deb_ascii_FullName(clearinghouseBuf,th_p->CLEARINGHOUSE_P),
        th_p->RET_P));

    /*
     * If clearinghouse not supplied, find master of parent directory
     * and use that.
     */
    if (EQ_bytes(NullFullName, th_p->CLEARINGHOUSE_P, sizeof(NullFullName))) {
	FullName_u parBuf;
	Timestamp_u context;
	int parBufLen = sizeof(parBuf);
	int found = FALSE;
	const int snum = dnsCountSimple((dns_opq_fname *)th_p->DIRECTORY_P);

	COPY_Timestamp(NullTimestamp, context);

	/*
	 * Missing clearinghouse argument.  Must find clearinghouse of
	 * parent, and use that as the default.
	 */
	dnsScatter((dns_opq_fname *)th_p->DIRECTORY_P, snum - 1,
	    (dns_opq_fname *)parBuf, &parBufLen,
	    NULL_DNS_OPQ_SNAME, 0,
	    NULL_DNS_OPQ_FNAME, 0);

	/* Read parent directory replica set */
	while (!found) {
	  AttributeContents_t results;
	  Set_t *const set_p = (Set_t *)results.ac_set;
	  int status;
	  int i;

	  status  = read_att(th_p, (FullName_t *)parBuf, ET_directory,
		     dns_AttributeName(DNS_REPLICAS),
		     (Timestamp_t *)context, BL_false, sizeof(results),
		     &results, CO_low, 0);
	  th_p->clerkStatus = status;

	  if (status != DNS_SUCCESS)
	    goto leave_clerkCreateDirectory;

	  if (NUM_Set(set_p) == 0) {
	    th_p->clerkStatus = DNS_DATACORRUPTION;
	    goto leave_clerkCreateDirectory;
	  }

	  /* Search each member for a master replica  */
	  for (i=0; i<NUM_Set(set_p); ++i) {
	    MemberValue_t *const sm_p = (MemberValue_t *)INDEX_Set(set_p, i);
	    ReplicaPointer_t *const rp_p = (ReplicaPointer_t *)sm_p->mv_value;
	    ReplicaType_t *type_p;

	    if (EXT8(sm_p->mv_valuetype) != VT_ReplicaPointer) continue;
	    type_p = (ReplicaType_t *)SKIP_Set(rp_p->rp_addressHint);
	    if (EXT8(type_p) == RT_master){
	      found = TRUE;
	      th_p->CLEARINGHOUSE_P =(unsigned char *)SKIP_ReplicaType(type_p);
	      break;
	    }
	    COPY_Timestamp(sm_p->mv_timestamp, context);
	  } /* End for loop */
	} /* End found directory in cache */
      } /* End clearinghouse not found yet */

    th_p->taBuildReq = buildCreateDirectoryRequest;
    th_p->taParseRsp = parseCreateDirectoryResponse;
    th_p->taRPC = CreateDirectoryRPC;

    th_p->cache_the_dir = 1;

    sendToClearinghouse(th_p, th_p->CLEARINGHOUSE_P);

leave_clerkCreateDirectory:

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " <clerkCreateDirectory(%s,%s,%p) status(%s)",
        deb_ascii_FullName(directoryBuf,th_p->DIRECTORY_P),
        deb_ascii_FullName(clearinghouseBuf,th_p->CLEARINGHOUSE_P),
        th_p->RET_P,
        deb_ascii_dns_status(statusBuf,th_p->clerkStatus)));
}
