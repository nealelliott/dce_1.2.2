/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: clerkcreateobject.c,v $
 * Revision 1.1.8.2  1996/02/18  19:26:11  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:09:18  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:04:42  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:38:20  root]
 * 
 * Revision 1.1.6.4  1994/08/03  18:59:35  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:14:09  mccann]
 * 
 * Revision 1.1.6.3  1994/06/23  18:29:01  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:54:59  mccann]
 * 
 * Revision 1.1.6.2  1994/06/09  16:07:48  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:52:31  devsrc]
 * 
 * Revision 1.1.6.1  1994/03/12  21:56:38  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:52:07  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  12:41:44  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:27:32  zeliff]
 * 
 * Revision 1.1.2.2  1992/03/22  21:24:36  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:08:50  weisman]
 * 
 * Revision 1.1  1992/01/19  15:26:54  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module clerkcreateobject.c
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
 *	Clerk.CreateObject command processor.
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
 * CreateObject arguments (passed in the thread block)
 */
#define	OBJECTNAME_P	clerkArgs[0]
#define	CLASS_P		clerkArgs[1]
#define	VERSION_P	clerkArgs[2]

#define OBJECTID_P      clerkArgs[3]
#define	RET_P		clerkArgs[4]

/*
 * Local Prototype declarations.
 */

int 
buildCreateObjectRequest (thread_t *);

static void 
parseCreateObjectResponse (thread_t *);

int 
CreateObjectRPC (thread_t  *,
                 Set_t     *,
                 ObjUID_t  *);

void 
clerkCreateObject (thread_t *);


/*
 *      b u i l d C r e a t e O b j e c t R e q u e s t
 *
 * Routine to build a CreateObject request message and
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
buildCreateObjectRequest (thread_t *th_p)
{
    int length = 0;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >buildCreateObjectRequest"));

    /*
     * CreateObjectRequest message:
     *	progress: Progress
     *  class: ClassName
     *	version: ClassVersion
     */
    length += buildProgress(th_p);

    if (th_p->taReqMsgEnd_p) {
	COPY_ClassName(th_p->CLASS_P, th_p->taReqMsgEnd_p);
	th_p->taReqMsgEnd_p = SKIP_ClassName(th_p->taReqMsgEnd_p);
    }
    length += LEN_ClassName(th_p->CLASS_P);

    if (th_p->taReqMsgEnd_p) {
	COPY_ClassVersion(th_p->VERSION_P, th_p->taReqMsgEnd_p);
	th_p->taReqMsgEnd_p = SKIP_ClassVersion(th_p->taReqMsgEnd_p);
    }
    length += LEN_ClassVersion(th_p->VERSION_P);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <buildCreateObjectRequest: return(length=%d)",
	length));

    return(length);
}

/*
 *      p a r s e C r e a t e O b j e c t R e s p o n s e
 *
 * This local routine parses the response message.
 */
static void 
parseCreateObjectResponse (thread_t *th_p)
{
    /*
     * CreateObjectResponse message:
     *	progress: Progress
     *  actualTS: Timemstamp
     */

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >parseCreateObjectResponse"));

    ta_begin(th_p);
    ta_Progress(th_p);
    ta_Timestamp(th_p, th_p->RET_P);
    ta_end(th_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <parseCreateObjectResponse"));
}

/*
 * Routine to call RPC stub for CreateObject request
 */
int 
CreateObjectRPC (thread_t  *th_p,
                 Set_t     *tower_p,
                 ObjUID_t  *uuid_p)
{
    rpc_binding_handle_t  h;
    int                   i;
    cds_Timestamp_t       actual_id;
    cds_Name_t            classname,*cla_p;
    cds_Progress_t        Progress;
    cds_Version_t         version,*ver_p;
    error_status_t rpcStatus,commStatus = rpc_s_no_bindings;
    cds_status_t          dnsStatus;
    uuid_t                objectid,*Oid_p;
    int                   fromcache, getout = 0;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " >CreateObjectRPC"));

    /* Set up structures for calling RPC */
    Build_R_Progress((byte_t *)th_p->taProgress.flags,
		     (Timeout_t *)th_p->taProgress.Timeout,
		     th_p->taProgress.Unresolved,
		     th_p->taProgress.Resolved,
		     &Progress);
    /* optional arguments */
    if (prm_in_length(th_p->CLASS_P) == 0) {
      cla_p = (cds_Name_t *)NULL;
      ver_p = (cds_Version_t *)NULL;
    } else {
      L_to_R_ClassName((ClassName_t *)th_p->CLASS_P, &classname);
      cla_p = &classname;
      if (0 < prm_in_length(th_p->VERSION_P)) {
	version.ver_major = EXT8(th_p->VERSION_P);
	version.ver_minor = EXT8((th_p->VERSION_P+1));
	ver_p = &version;
      } else
	ver_p = (cds_Version_t *)NULL;
    }
    if (prm_in_length(th_p->OBJECTID_P) == 0)
      Oid_p = (uuid_t *)NULL;
    else {
      /* Align */
      COPY_ObjUID(th_p->OBJECTID_P, &objectid);
      Oid_p = &objectid;
    }

    /*
     * First attempt to find a cached handle. There
     * is a use count associated with handles, so make
     * sure either free or release cached handle is called
     */
    if (fromcache = get_cached_handle(&h, uuid_p, th_p))
      commStatus = cds_CreateObject(h, &Progress, cla_p, ver_p,
				    Oid_p, &actual_id, &dnsStatus);

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
	    commStatus = cds_CreateObject(h, &Progress, cla_p, ver_p,
					  Oid_p, &actual_id, &dnsStatus);
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
	    if (FLAG_IS_SET(&Progress.pr_flags, PR_done))
	      th_p->taRspMsgPtr_p = R_to_L_Timestamp(&actual_id,
				     (Timestamp_t *)th_p->taRspMsgPtr_p, TRUE);
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
           " <CreateObjectRPC: return(DNS_SUCCESS)"));

	return(DNS_SUCCESS);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " <CreateObjectRPC: return(DNS_NOCOMMUNICATION)"));

    return(DNS_NOCOMMUNICATION);
}

/*
 *	c l e r k C r e a t e O b j e c t
 *
 * This routine creates an object.
 *
 * Inputs:
 *	OBJECTNAME_P	FullName of object to be created
 *	CLASS_P		ClassName of class of object
 *	VERSION_P	ClassVersion of class of object
 *
 * Returns:
 *	RET_P		Timestamp of created object
 */
void 
clerkCreateObject (thread_t *th_p)
{
    DEB_ASCII_BUF_FullName(objectNameBuf)
    DEB_ASCII_BUF_ClassName(classBuf)
    DEB_ASCII_BUF_ClassVersion(versionBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " >clerkCreateObject(%s,%s,%s,%p)",
        deb_ascii_FullName(objectNameBuf,th_p->OBJECTNAME_P),
        deb_ascii_ClassName(classBuf,th_p->CLASS_P),
        deb_ascii_ClassVersion(versionBuf,th_p->VERSION_P),
        th_p->RET_P));

    th_p->taBuildReq = buildCreateObjectRequest;
    th_p->taParseRsp = parseCreateObjectResponse;
    th_p->taRPC = CreateObjectRPC;

    sendToWalkTree(th_p, th_p->OBJECTNAME_P);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " <clerkCreateObject(%s,%s,%s,%p) status(%s)",
        deb_ascii_FullName(objectNameBuf,th_p->OBJECTNAME_P),
        deb_ascii_ClassName(classBuf,th_p->CLASS_P),
        deb_ascii_ClassVersion(versionBuf,th_p->VERSION_P),
        th_p->RET_P,
        deb_ascii_dns_status(statusBuf,th_p->clerkStatus)));
}
