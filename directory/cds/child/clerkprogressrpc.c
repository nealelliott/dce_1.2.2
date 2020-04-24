/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: clerkprogressrpc.c,v $
 * Revision 1.1.8.2  1996/02/18  19:26:40  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:09:31  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:05:17  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:38:40  root]
 * 
 * Revision 1.1.6.4  1994/08/03  18:59:47  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:17:54  mccann]
 * 
 * Revision 1.1.6.3  1994/06/23  18:29:12  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:55:46  mccann]
 * 
 * Revision 1.1.6.2  1994/06/09  16:08:00  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:52:45  devsrc]
 * 
 * Revision 1.1.6.1  1994/03/12  21:56:54  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:53:23  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  12:42:39  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:28:38  zeliff]
 * 
 * Revision 1.1.2.2  1992/03/22  21:28:45  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:09:59  weisman]
 * 
 * Revision 1.1  1992/01/19  15:27:04  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module clerkprogressrpc.c
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
 * Common routine to call RPC stub for routines which only pass and receive a
 * progress record.
 */
dns_status_t 
clerkProgressRPC (thread_t               *th_p,
                  Set_t                  *tower_p,
                  ObjUID_t               *uuid_p,
                  error_status_t (*func  )
		        (rpc_binding_handle_t  ,
                         cds_Progress_t        *,
                         cds_status_t          *))
{
    rpc_binding_handle_t  h;
    cds_Progress_t Progress;
    error_status_t rpcStatus, commStatus = rpc_s_no_bindings;
    cds_status_t  dnsStatus;
    int fromcache, getout=0;
    int i;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " >clerkProgressRPC(%lx,%lx,%lx,%lx)",
        th_p,
        tower_p,
        uuid_p,
        func));
   
    /* Set up structures for calling RPC */
    Build_R_Progress((byte_t *)th_p->taProgress.flags, 
		     (Timeout_t *)th_p->taProgress.Timeout, 
		     th_p->taProgress.Unresolved, 
		     th_p->taProgress.Resolved,
		     &Progress);
    /* 
     * First attempt to find a cached handle. There
     * is a use count associated with handles, so make
     * sure either free or release cached handle is called 
     */
    if ((fromcache = get_cached_handle(&h, uuid_p, th_p)) != 0)
    {
	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_child,
            svc_c_debug4,
            "  clerkProgressRPC: func = %lx",
            func));

	commStatus = (*func)(h, &Progress, &dnsStatus);
    }
    if (commStatus != rpc_s_ok) 
    {
	if (fromcache) 
	{
	    free_cached_handle(th_p, h);
	    fromcache = 0;
	}
	
	for (i = 0; i < NUM_Set(tower_p); i++) 
	{
	    if (!get_rpc_handle(&h, (MemberValue_t *)INDEX_Set(tower_p, i), 
				uuid_p, th_p, &getout)) 
		if (getout) 
		    break;
		else 
		    continue;
	    commStatus = (*func)(h, &Progress, &dnsStatus);
	    if (commStatus == rpc_s_ok) break;
	    maximize_rpc_error(th_p, commStatus, &getout);
	    rpc_binding_free(&h, &rpcStatus);

	    if (getout) 
		break;
	} /* End For loop */
    }

    if (commStatus == rpc_s_ok) 
    {
	th_p->taRspMsgPtr_p = th_p->taRspMsg_p;
	if (dnsStatus.er_status == PDU_Success) 
	{
	    th_p->taRspType = MT_Response;
	    th_p->taRspMsgPtr_p = R_to_L_Progress(&Progress, 
				      (Progress_t *)th_p->taRspMsgPtr_p, TRUE);
	}
	else 
	{
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
            " <clerkProgressRPC: return DNS_SUCCESS"));

	return(DNS_SUCCESS);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " <clerkProgressRPC: return DNS_NOCOMMUNICATION"));

    return(DNS_NOCOMMUNICATION);
}
