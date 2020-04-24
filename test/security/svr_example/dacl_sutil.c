/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: dacl_sutil.c,v $
 * Revision 1.1.4.2  1996/02/17  23:26:27  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:44:17  marty]
 *
 * Revision 1.1.4.1  1995/12/11  21:52:35  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:54:13  root]
 * 
 * Revision 1.1.2.2  1994/08/10  19:23:17  annie
 * 	expand copyright with OSF copyright text
 * 	[1994/08/10  17:17:33  annie]
 * 
 * Revision 1.1.2.1  1994/08/02  20:11:42  bhaim
 * 	move acl_server to test area
 * 	[1994/08/02  20:11:07  bhaim]
 * 
 * $EndLog$
 */
/*  dacl_sutil.c V=4 04/24/91 //littl/prgy/src/admin/dacl_edit/test
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
**  SEC_ACL dummy server utilities, for testing/debugging clients only
** 
*/

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)dacl_sutil.c	4 - 04/24/91";
#endif

#include <dce/rpc.h>

#include <stdio.h>

char *error_text
#ifndef __STDC__
    ( st ) 
    error_status_t st;
#else
  (
    error_status_t st
  )
#endif
{
    static char buf[100];
    switch (st) {
        case rpc_s_RESERVED_01:
            return "ncastat communications failure";
            break;
        case rpc_s_RESERVED_03:
            return "ncastat unknown interface";
            break;
        case rpc_s_cant_create_sock:
            return "rpc_s_cant_create_sock";
            break;
        case rpc_s_cant_bind_sock:
            return "rpc_s_cant_bind_sock";
            break;
        case rpc_s_RESERVED_04:
            return "rpc_s_RESERVED_04";
            break;
        case rpc_s_not_in_call:
            return "rpc_s_not_in_call";
            break;
        case rpc_s_RESERVED_05:
            return "rpc_s_RESERVED_05";
            break;
        case rpc_s_no_port:
            return "rpc_s_no_port";
            break;
        case rpc_s_too_many_sockets:
            return "rpc_s_too_many_sockets";
            break;
        case rpc_s_illegal_register:
            return "rpc_s_illegal_register";
            break;
        case rpc_s_cant_recv:
            return "rpc_s_cant_recv";
            break;
        case rpc_s_bad_pkt:
            return "rpc_s_bad_pkt";
            break;
        case rpc_s_unbound_handle:
            return "rpc_s_unbound_handle";
            break;
        case rpc_s_addr_in_use:
            return "rpc_s_addr_in_use";
            break;
        case rpc_s_in_args_too_big:
            return "rpc_s_in_args_too_big";
            break;
        case rpc_s_RESERVED_07:
            return "rpc_s_RESERVED_07";
            break;
        case rpc_s_RESERVED_08:
            return "rpc_s_RESERVED_08";
            break;
        case rpc_s_string_too_long:
            return "rpc_s_string_too_long";
            break;
        case rpc_s_too_many_objects:
            return "rpc_s_too_many_objects";
            break;
        case rpc_s_RESERVED_09:
            return "ncastat you crashed";
            break;
        case rpc_s_no_memory:
            return "rpc_s_no_memory";
            break;
        case rpc_s_cant_nmalloc:
            return "rpc_s_cant_nmalloc";
            break;
        case rpc_s_call_faulted:
            return "rpc_s_call_faulted";
            break;
        case rpc_s_call_failed:
            return "rpc_s_call_failed";
            break;
        case rpc_s_comm_failure:
            return "rpc_s_comm_failure";
            break;
        case rpc_s_unused2:
            return "rpc_s_unused2";
            break;
        case rpc_s_illegal_family_rebind:
            return "rpc_s_illegal_family_rebind";
            break;
        case rpc_s_invalid_handle:
            return "rpc_s_invalid_handle";
            break;
        case rpc_s_coding_error:
            return "rpc_s_coding_error";
            break;
        case rpc_s_object_not_found:
            return "rpc_s_object_not_found";
            break;
        case rpc_s_cthread_not_found:
            return "rpc_s_cthread_not_found";
            break;
        case rpc_s_invalid_binding:
            return "rpc_s_invalid_binding";
            break;
        case rpc_s_already_registered:
            return "rpc_s_already_registered";
            break;
        case rpc_s_endpoint_not_found:
            return "rpc_s_endpoint_not_found";
            break;
        case rpc_s_invalid_rpc_protseq:
            return "rpc_s_invalid_rpc_protseq";
            break;
        case rpc_s_desc_not_registered:
            return "rpc_s_desc_not_registered";
            break;
        case rpc_s_already_listening:
            return "rpc_s_already_listening";
            break;
        case rpc_s_no_protseqs:
            return "rpc_s_no_protseqs";
            break;
        case rpc_s_no_protseqs_registered:
            return "rpc_s_no_protseqs_registered";
            break;
        case rpc_s_no_bindings:
            return "rpc_s_no_bindings";
            break;
        case rpc_s_max_descs_exceeded:
            return "rpc_s_max_descs_exceeded";
            break;
        case rpc_s_no_interfaces:
            return "rpc_s_no_interfaces";
            break;
        case rpc_s_invalid_timeout:
            return "rpc_s_invalid_timeout";
            break;
        case rpc_s_cant_inq_socket:
            return "rpc_s_cant_inq_socket";
            break;
        case rpc_s_invalid_naf_id:
            return "rpc_s_invalid_naf_id";
            break;
        case rpc_s_inval_net_addr:
            return "rpc_s_inval_net_addr";
            break;
        case rpc_s_unknown_if:
            return "rpc_s_unknown_if";
            break;
        case rpc_s_unsupported_type:
            return "rpc_s_unsupported_type";
            break;
        case rpc_s_invalid_call_opt:
            return "rpc_s_invalid_call_opt";
            break;
        case rpc_s_no_fault:
            return "rpc_s_no_fault";
            break;
        case rpc_s_invalid_call_handle:
            return "rpc_s_invalid_call_handle";
            break;
        case rpc_s_cannot_alloc_assoc:
            return "rpc_s_cannot_alloc_assoc";
            break;
        case rpc_s_cannot_connect:
            return "rpc_s_cannot_connect";
            break;
        case rpc_s_connection_aborted:
            return "rpc_s_connection_aborted";
            break;
        case rpc_s_connection_closed:
            return "rpc_s_connection_closed";
            break;
        case rpc_s_cannot_accept:
            return "rpc_s_cannot_accept";
            break;
        case rpc_s_assoc_grp_not_found:
            return "rpc_s_assoc_grp_not_found";
            break;
        case rpc_s_stub_interface_error:
            return "rpc_s_stub_interface_error";
            break;
        case rpc_s_invalid_vers_option:
            return "rpc_s_invalid_vers_option";
            break;
        /*
            * Name service status codes
            */
        case rpc_s_import_already_done:
            return "rpc_s_import_already_done";
            break;
        case rpc_s_database_busy:
            return "rpc_s_database_busy";
            break;
        case rpc_s_invalid_import_context:
            return "rpc_s_invalid_import_context";
            break;
        case rpc_s_no_rpc_data:
            return "rpc_s_no_rpc_data";
            break;
        case rpc_s_no_more_entries:
            return "rpc_s_no_more_entries";
            break;
        case rpc_s_unknown_ns_error:
            return "rpc_s_unknown_name_service_error";
            break;
        case rpc_s_name_service_unavailable:
            return "rpc_s_name_service_unavailable";
            break;
        case rpc_s_incomplete_name:
            return "rpc_s_incomplete_name";
            break;
        case rpc_s_group_not_found:
            return "rpc_s_group_not_found";
            break;
        case rpc_s_invalid_name_syntax:
            return "rpc_s_invalid_name_syntax";
            break;
        case rpc_s_no_more_members:
            return "rpc_s_no_more_members";
            break;
        case rpc_s_no_more_interfaces:
            return "rpc_s_no_more_interfaces";
            break;
        case rpc_s_invalid_name_service:
            return "rpc_s_invalid_name_service";
            break;
        case rpc_s_no_name_mapping:
            return "rpc_s_no_name_mapping";
            break;
        case rpc_s_profile_not_found:
            return "rpc_s_profile_not_found";
            break;
        case rpc_s_not_found:
            return "rpc_s_not_found";
            break;
        case rpc_s_no_updates:
            return "rpc_s_no_updates";
            break;
        case rpc_s_update_failed:
            return "rpc_s_update_failed";
            break;
        case rpc_s_no_match_exported:
            return "rpc_s_no_match_exported";
            break;
        case rpc_s_entry_not_found:
            return "rpc_s_entry_not_found";
            break;
        case rpc_s_invalid_inquiry_context:
            return "rpc_s_invalid_inquiry_context";
            break;
        case rpc_s_interface_not_found:
            return "rpc_s_interface_not_found";
            break;
        case rpc_s_group_member_not_found:
            return "rpc_s_group_member_not_found";
            break;
        case rpc_s_entry_already_exists:
            return "rpc_s_entry_already_exists";
            break;
        case rpc_s_unsupported_name_syntax:
            return "rpc_s_unsupported_name_syntax";
            break;
        case rpc_s_uuid_set_not_found:
            return "rpc_s_uuid_set_not_found";
            break;
        case rpc_s_uuid_member_not_found:
            return "rpc_s_uuid_member_not_found";
            break;
        case rpc_s_tower_set_not_found:
            return "rpc_s_tower_set_not_found";
            break;
        case rpc_s_tower_member_not_found:
            return "rpc_s_tower_member_not_found";
            break;
        case rpc_s_obj_uuid_not_found:
            return "rpc_s_obj_uuid_not_found";
            break;
        case rpc_s_no_more_bindings:
            return "rpc_s_no_more_bindings";
            break;
        case rpc_s_invalid_priority:
            return "rpc_s_invalid_priority";
            break;
        case rpc_s_not_rpc_entry:
            return "rpc_s_not_rpc_entry";
            break;
        case rpc_s_invalid_lookup_context:
            return "rpc_s_invalid_lookup_context";
            break;
        case rpc_s_binding_vector_full:
            return "rpc_s_binding_vector_full";
            break;
        case rpc_s_cycle_detected:
            return "rpc_s_cycle_detected";
            break;

        default:
            sprintf(buf, "Unknown status %x", st);
            return buf;
    }
}
