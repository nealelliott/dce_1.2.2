/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_bind_util.c,v $
 * Revision 1.1.6.3  1996/02/18  00:22:52  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:10:28  marty]
 *
 * Revision 1.1.6.2  1995/12/08  18:02:50  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/09/19  20:11 UTC  bartf
 * 	merge sec_bind_retry_status fix incorrect messages, found in odss
 * 
 * 	HP revision /main/HPDCE02/bartf_sec_10/1  1995/09/08  16:50 UTC  bartf
 * 	fix sec_bind_retry_status incorrect msg catalog
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/15  21:14 UTC  greg
 * 	Merge client compatibility fixes from greg_sec_migrate2.
 * 	[1995/05/03  23:51 UTC  greg  /main/SEC_migrate_merge/1]
 * 
 * 	Fix check for NULL endpoint in sec_bind_util_resolve_binding().
 * 	[1995/05/03  21:55 UTC  greg  /main/greg_sec_migrate2/2]
 * 
 * 	Add sec_bind_util_binding_resolve().
 * 	[1995/05/02  20:30 UTC  greg  /main/greg_sec_migrate2/1]
 * 
 * Revision 1.1.2.2  1994/08/10  19:22:10  annie
 * 	expand copyright with OSF copyright text
 * 	[1994/08/10  17:16:29  annie]
 * 
 * Revision 1.1.2.1  1994/07/15  15:03:11  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  18:00:35  mdf]
 * 
 * $EndLog$
 */

/*  sec_bind_util.c
**
** Copyright (c) Hewlett-Packard Company 1994, 1995
** Unpublished work. All Rights Reserved.
**
*/
#include <stdlib.h>
#include <string.h>

#include <macros.h>
#include <rs_pwd_mgmt.h>
#include <sec_attr_tools.h>
#include <sec_pwd_mgmt.h>
#include <u_str.h>

#include <dce/rpc.h>
#include <dce/rsec_pwd_mgmt.h>
#include <dce/sec_attr_util.h>
#include <dce/sec_login.h>


/*
 * s e c _ b i n d _ r e t r y _ s t a t u s
 *
 * Return true if it is appropriate to retry an operation that
 * received status "op_st".
 * Return false if it is not appropriate to retry the operation.
 */
PUBLIC boolean32 sec_bind_retry_status (
    error_status_t *op_stp
)
{
    boolean32 retval;

    switch(*op_stp) {
        case error_status_ok:
            retval = false;
            break;

        case rpc_s_wrong_boot_time:
        case rpc_s_comm_failure:
        case rpc_s_endpoint_not_found:  /* ??? */
        case rpc_s_connection_closed:
        case rpc_s_connect_timed_out:
        case rpc_s_connect_rejected:
        case rpc_s_network_unreachable:
        case rpc_s_connect_no_resources:
        case rpc_s_rem_network_shutdown:
        case rpc_s_too_many_rem_connects:
        case rpc_s_no_rem_endpoint:
        case rpc_s_rem_host_down:
        case rpc_s_host_unreachable:
        case rpc_s_access_control_info_inv: /* ??? */
        case rpc_s_connect_closed_by_rem:
        case rpc_s_rem_host_crashed:
        case rpc_s_tsyntaxes_unsupported:
        case rpc_s_cant_getpeername:
        case rpc_s_unknown_reject:      /* ??? */
        case rpc_s_wrong_kind_of_binding:
        case rpc_s_call_timeout:
        case rpc_s_server_too_busy:
        case rpc_s_unknown_error:
        case rpc_s_socket_failure:
        case rpc_s_auth_skew:
        case rpc_s_not_listening:
        case rpc_s_rpcd_comm_failure:
        case rpc_s_object_not_found:
        case rpc_s_cthread_not_found:
        case rpc_s_invalid_rpc_protseq: /* ??? */
        case rpc_s_max_descs_exceeded:  /* ??? */
        case rpc_s_unknown_if:
        case rpc_s_unsupported_type:
        case rpc_s_loc_connect_aborted:
        case rpc_s_unknown_mgr_type:
        case ept_s_not_registered:

            retval = true;
            break;

        /* 
         * This probably means the password management server received
         * an internal error (hopefully, this case will be rare)
         */
        default:
            retval = false;
            break;
    }

    return retval;
}

/* sec_bind_util_resolve_binding 
 *
 * If the specifed rpc binding handle is not
 * fully resolved (has no endpoint) resolve it
 * using the specified if_handle.  If the which_protseq
 * parameter is NULL, the binding handle is resolved
 * regardless of the protocol sequence.  If the
 * which_protseq parameter is non-null, the
 * binding handle is resolved only if it is a
 * binding handle over the specifed protocol sequence.
 * Returns false if the rpc_handle requires resolution
 * and could not be resolved, true otherwise.
 */
boolean32 sec_bind_util_resolve_binding ( 
    rpc_binding_handle_t  rpc_handle,
    rpc_if_handle_t       if_handle,
    unsigned_char_p_t     which_protseq,  /* NULL is a wildcard */
    error_status_t        *status
)
{
    unsigned_char_p_t   str_obj             = NULL;
    unsigned_char_p_t   str_protseq         = NULL;
    unsigned_char_p_t   str_netaddr         = NULL;
    unsigned_char_p_t   str_endpoint        = NULL;
    unsigned_char_p_t   str_options         = NULL;
    unsigned_char_p_t   str_binding         = NULL;
    error_status_t      ignore_st;

    rpc_binding_to_string_binding(rpc_handle, &str_binding, status);
    if (BAD_STATUS(status)) {
	return false;
    }
    rpc_string_binding_parse(str_binding, &str_obj, &str_protseq,
			     &str_netaddr, &str_endpoint,
			     &str_options, status);
    if (STATUS_OK(status) 
    && (u_strlen(str_endpoint) == 0)
    && (!which_protseq || (u_strcmp(which_protseq, str_protseq) == 0))) {
	rpc_ep_resolve_binding(rpc_handle, if_handle, status);
    }	    
    
    /* cleanup */
    if (str_obj != NULL) {
        rpc_string_free(&str_obj, &ignore_st);
    }
    if (str_protseq != NULL) {
        rpc_string_free(&str_protseq, &ignore_st);
    }
    if (str_netaddr != NULL) {
        rpc_string_free(&str_netaddr, &ignore_st);
    }
    if (str_endpoint != NULL) {
        rpc_string_free(&str_endpoint, &ignore_st);
    }
    if (str_options != NULL) {
        rpc_string_free(&str_options, &ignore_st);
    }
    if (str_binding != NULL) {
        rpc_string_free(&str_binding, &ignore_st);
    }

    return STATUS_OK(status);
}
