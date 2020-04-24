/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: clerk_miscel.c,v $
 * Revision 1.1.6.2  1996/02/18  19:25:47  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:09:06  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:04:10  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:38:00  root]
 * 
 * Revision 1.1.4.5  1994/08/03  18:59:27  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:11:07  mccann]
 * 
 * Revision 1.1.4.4  1994/06/23  18:28:50  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:54:08  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  16:07:38  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:52:21  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/14  14:41:11  peckham
 * 	New module clerk_socket.c to isolate SVR4 named pipes.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	Remove msgblk.h, cds_event_init() and evp references.
 * 	Remove v1v2 code (v1 never used for cds).
 * 	Remove VMS and UNIX conditionals, VMS code.
 * 	Remove last vestige of clerk_trace().
 * 	[1994/04/11  19:36:13  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  21:56:22  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:50:49  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  12:40:55  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:26:37  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:27:24  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: clerk_miscel.c
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
 */
#include <dce/dce.h>
#include <dce/dcecdsmsg.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <pwd.h>
#include <uc_clerk.h>
#include <uc_child.h>
#include <cache.h>
#include <dce/dnsclerk.h>
#include <dns_malloc.h>
#include <dce/attributes.h>

# include <dce/rpcsts.h>

/* ----------------------------------------------------------------------
 *  Remove the rightmost SimpleName from the given FullName
 * ----------------------------------------------------------------------
 */
int 
childStripRSimple (FullName_t *Fname_p)
{
    register SimpleName_t *end_p = (SimpleName_t *)Fname_p->fn_name;
    register SimpleName_t *last_p;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >childStripRSimple"));

    if ((EXT8(end_p->sn_flag)&SN_type) == SN_null)
    {
	DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_child,
          svc_c_debug9,
          " <childStripRSimple: return(DNS_ERROR)"));

	return(DNS_ERROR);
    }
    
    do {
	last_p = end_p;
	end_p = (SimpleName_t *)SKIP_SimpleName(last_p);
    } while ((EXT8(end_p->sn_flag)&SN_type) != SN_null);

    INS16(Fname_p->fn_length, EXT16(Fname_p->fn_length)-LEN_SimpleName(last_p));
    COPY_SimpleName(NullSimpleName, last_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <childStripRSimple: return(DNS_SUCCESS)"));

    return(DNS_SUCCESS);
}



/*
 * Allocation/deallocation routines for NIDL runtime on client side
 */
idl_void_p_t 
rpc_ss_allocate (idl_size_t size)
{

  return((idl_void_p_t)malloc(size));
}

void 
rpc_ss_free (idl_void_p_t addr_p)
{

    free(addr_p);
}

/*
 * Convert an rpc error status to DNS_NOCOMMUNICATION for those RPC runtime
 * errors indicating that the requested server has become unavailable. The
 * CDS Clerk will then attempt to return stale data from the cache, with a
 * status code of CDS_SUCCESS_STALE. For all other rpc errors, return the
 * actual rpc error status.
*/

int 
cds_convert_rpc_status (error_status_t commStatus)
{
    int status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug8,
        " >cds_convert_rpc_status"));

    switch ( commStatus )
    { 
	case ept_s_cant_access:
	case ept_s_server_unavailable:
	case uuid_s_socket_failure:
	case rpc_s_addr_in_use:
	case rpc_s_assoc_req_rejected:
	case rpc_s_assoc_shutdown:
	case rpc_s_authn_authz_mismatch:
	case rpc_s_bad_pkt:
	case rpc_s_binding_has_no_auth:
	case rpc_s_binding_vector_full:
	case rpc_s_call_cancelled:
	case rpc_s_call_faulted:
	case rpc_s_call_orphaned:
	case rpc_s_call_queued:
	case rpc_s_call_timeout:
	case rpc_s_cannot_accept:
	case rpc_s_cannot_connect:
	case rpc_s_cant_bind_sock:
	case rpc_s_cant_create_sock:
	case rpc_s_cant_get_if_id:
	case rpc_s_cant_getpeername:
	case rpc_s_cant_inq_socket:
	case rpc_s_cant_listen_sock:
	case rpc_s_cant_recv:
	case rpc_s_cant_recvmsg:
	case rpc_s_comm_failure:
	case rpc_s_connect_closed_by_rem:
	case rpc_s_connect_no_resources:
	case rpc_s_connect_rejected:
	case rpc_s_connect_timed_out:
	case rpc_s_connection_aborted:
	case rpc_s_connection_closed:
	case rpc_s_database_busy:
	case rpc_s_different_server_instance:
	case rpc_s_endpoint_not_found:
	case rpc_s_fault_pipe_comm_error:
	case rpc_s_fault_remote_comm_failure:
	case rpc_s_fault_remote_no_memory:
	case rpc_s_host_unreachable:
	case rpc_s_loc_connect_aborted:
	case rpc_s_network_unreachable:
	case rpc_s_rem_host_crashed:
	case rpc_s_rem_host_down:
	case rpc_s_rem_network_shutdown:
	case rpc_s_server_too_busy:
	case rpc_s_unknown_ns_error:
	case rpc_s_unknown_reject:
	case rpc_s_unknown_status_code:
	    status = DNS_NOCOMMUNICATION;
	    break;

	default:
	    status = (int)commStatus;
	    break;
	}

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug8,
        " <cds_convert_rpc_status: return(status = %d)",
	status));

    return ( status );
}
