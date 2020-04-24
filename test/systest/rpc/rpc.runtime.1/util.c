/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: util.c,v $
 * Revision 1.1.9.2  1996/02/17  23:28:01  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:45:10  marty]
 *
 * Revision 1.1.9.1  1995/12/11  22:43:36  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:20:26  root]
 * 
 * Revision 1.1.7.1  1993/09/09  19:03:04  cmckeen
 * 	HP's TET'ized version
 * 	[1993/09/09  18:58:48  cmckeen]
 * 
 * Revision 1.1.5.2  1993/08/24  13:37:27  giza
 * 	.
 * 	[1993/08/23  20:36:26  giza]
 * 
 * Revision 1.1.1.5  1993/05/04  22:21:58  mgm
 * 	    Loading drop DCE1_0_2FNL
 * 
 * Revision 1.1.2.4  1993/03/26  18:20:19  eperkins
 * 	Corrected error reporting for RIOS build.
 * 	[1993/03/26  18:18:02  eperkins]
 * 
 * Revision 1.1.2.3  1993/02/05  15:22:02  cjd
 * 	Embedded copyright notice
 * 	[1993/02/05  14:42:40  cjd]
 * 
 * Revision 1.1.3.2  1993/02/04  22:11:12  cjd
 * 	Embedded copyright notice
 * 
 * Revision 1.1.2.2  1992/09/25  19:30:38  eperkins
 * 	Initial version
 * 	[1992/09/25  19:30:14  eperkins]
 * 
 * $EndLog$
 */
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <dce/cma.h>
#include <dce/rpc.h>
#include <dce/rpcexc.h>
#include <dce/pthread_exc.h>

char *error_text(st)
error_status_t st; 
{
    char *buf;
    error_status_t rst=0;

    EXCEPTION e;

    buf = (char*)malloc(256);
#ifndef PRT
    TRY
       dce_error_inq_text(st,buf,&rst);
    CATCH_ALL
       fprintf(stderr,"FAILURE during call to dce_error_inq_text\n");
       rst = rpc_s_coding_error;
    ENDTRY    
    if (rst == error_status_ok) return buf;
#endif

    switch (st)
    {
        case rpc_s_cant_create_sock:
            return "rpc_s_cant_create_sock";
            break;
        case rpc_s_cant_bind_sock:
            return "rpc_s_cant_bind_sock";
            break;
        case rpc_s_not_in_call:
            return "rpc_s_not_in_call";
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
        /*case rpc_s_addr_in_use:
            return "rpc_s_addr_in_use";
            break;*/
        case rpc_s_in_args_too_big:
            return "rpc_s_in_args_too_big";
            break;
        case rpc_s_string_too_long:
            return "rpc_s_string_too_long";
            break;
        case rpc_s_too_many_objects:
            return "rpc_s_too_many_objects";
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
        case rpc_s_comm_failure:
            return "rpc_s_comm_failure";
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
        case rpc_s_entry_already_exists:
            return "rpc_s_entry_already_exists";
            break;
        case rpc_s_nsinit_failure:
            return "rpc_s_nsinit_failure";
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


void print_exception(e)
      EXCEPTION *e;
{
      char *t = NULL;

/* DG and common errors */
      if (exc_matches(e,&rpc_x_assoc_grp_not_found)) t = "rpc_x_assoc_grp_not_found";
      else if (exc_matches(e,&rpc_x_cancel_timeout)) t = "rpc_x_cancel_timeout";

      else if (exc_matches(e,&rpc_x_coding_error)) t = "rpc_x_coding_error";

      else if (exc_matches(e,&rpc_x_comm_failure)) t = "rpc_x_comm_failure";

      else if (exc_matches(e,&rpc_x_context_id_not_found)) t = "rpc_x_context_id_not_found";

      else if (exc_matches(e,&rpc_x_endpoint_not_found)) t = "rpc_x_endpoint_not_found";

      else if (exc_matches(e,&rpc_x_invalid_binding)) t = "rpc_x_invalid_binding";

      else if (exc_matches(e,&rpc_x_invalid_tag)) t = "rpc_x_invalid_tag";

      else if (exc_matches(e,&rpc_x_invalid_bound)) t = "rpc_x_invalid_bound";

      else if (exc_matches(e,&rpc_x_manager_not_entered)) t = "rpc_x_manager_not_entered";

      else if (exc_matches(e,&rpc_x_no_memory)) t = "rpc_x_no_memory";

      else if (exc_matches(e,&rpc_x_server_too_busy)) t = "rpc_x_server_too_busy";

      else if (exc_matches(e,&rpc_x_op_rng_error)) t = "rpc_x_op_rng_error";

      else if (exc_matches(e,&rpc_x_protocol_error)) t = "rpc_x_protocol_error";

      else if (exc_matches(e,&rpc_x_unknown_if)) t = "rpc_x_unknown_if";

      else if (exc_matches(e,&rpc_x_unknown_error)) t = "rpc_x_unknown_error";

      else if (exc_matches(e,&rpc_x_unknown_reject)) t = "rpc_x_unknown_reject";

      else if (exc_matches(e,&rpc_x_unknown_remote_fault)) t = "rpc_x_unknown_remote_fault";

      else if (exc_matches(e,&rpc_x_unsupported_type)) t = "rpc_x_unsupported_type";

      else if (exc_matches(e,&rpc_x_who_are_you_failed)) t = "rpc_x_who_are_you_failed";

      else if (exc_matches(e,&rpc_x_wrong_boot_time)) t = "rpc_x_wrong_boot_time";


/* CN errors */
      if (exc_matches(e,&rpc_x_access_control_info_inv)) t = "rpc_x_access_control_info_inv";


      else if (exc_matches(e,&rpc_x_assoc_shutdown)) t = "rpc_x_assoc_shutdown";

      else if (exc_matches(e,&rpc_x_cannot_accept)) t = "rpc_x_cannot_accept";

      else if (exc_matches(e,&rpc_x_connect_closed_by_rem)) t = "rpc_x_connect_closed_by_rem";

      else if (exc_matches(e,&rpc_x_connect_no_resources)) t = "rpc_x_connect_no_resources";

      else if (exc_matches(e,&rpc_x_connect_rejected)) t = "rpc_x_connect_rejected";

      else if (exc_matches(e,&rpc_x_connect_timed_out)) t = "rpc_x_connect_timed_out";

      else if (exc_matches(e,&rpc_x_connection_aborted)) t = "rpc_x_connection_aborted";

      else if (exc_matches(e,&rpc_x_connection_closed)) t = "rpc_x_connection_closed";

      else if (exc_matches(e,&rpc_x_host_unreachable)) t = "rpc_x_host_unreachable";

      else if (exc_matches(e,&rpc_x_invalid_call_opt)) t = "rpc_x_invalid_call_opt";

      else if (exc_matches(e,&rpc_x_invalid_endpoint_format)) t = "rpc_x_invalid_endpoint_format";

      else if (exc_matches(e,&rpc_x_loc_connect_aborted)) t = "rpc_x_loc_connect_aborted";

      else if (exc_matches(e,&rpc_x_network_unreachable)) t = "rpc_x_network_unreachable";

      else if (exc_matches(e,&rpc_x_no_rem_endpoint)) t = "rpc_x_no_rem_endpoint";

      else if (exc_matches(e,&rpc_x_no_fault)) t = "rpc_x_no_fault";

      else if (exc_matches(e,&rpc_x_rem_host_crashed)) t = "rpc_x_rem_host_crashed";

      else if (exc_matches(e,&rpc_x_rem_host_down)) t = "rpc_x_rem_host_down";

      else if (exc_matches(e,&rpc_x_rem_network_shutdown)) t = "rpc_x_rem_network_shutdown";

      else if (exc_matches(e,&rpc_x_too_many_rem_connects)) t = "rpc_x_too_many_rem_connects";

      else if (exc_matches(e,&rpc_x_tsyntaxes_unsupported)) t = "rpc_x_tsyntaxes_unsupported";


/* NS import routine errors */
      if (exc_matches(e,&rpc_x_binding_vector_full)) t = "rpc_x_binding_vector_full";

      else if (exc_matches(e,&rpc_x_entry_not_found)) t = "rpc_x_entry_not_found";

      else if (exc_matches(e,&rpc_x_group_not_found)) t = "rpc_x_group_not_found";

      else if (exc_matches(e,&rpc_x_incomplete_name)) t = "rpc_x_incomplete_name";

      else if (exc_matches(e,&rpc_x_invalid_import_context)) t = "rpc_x_invalid_import_context";

      else if (exc_matches(e,&rpc_x_invalid_inquiry_context)) t = "rpc_x_invalid_inquiry_context";

      else if (exc_matches(e,&rpc_x_invalid_inquiry_type)) t = "rpc_x_invalid_inquiry_type";

      else if (exc_matches(e,&rpc_x_invalid_lookup_context)) t = "rpc_x_invalid_lookup_context";

      else if (exc_matches(e,&rpc_x_invalid_name_syntax)) t = "rpc_x_invalid_name_syntax";

      else if (exc_matches(e,&rpc_x_invalid_object)) t = "rpc_x_invalid_object";

      else if (exc_matches(e,&rpc_x_invalid_rpc_protseq)) t = "rpc_x_invalid_rpc_protseq";

      else if (exc_matches(e,&rpc_x_invalid_vers_option)) t = "rpc_x_invalid_vers_option";

      else if (exc_matches(e,&rpc_x_no_more_bindings)) t = "rpc_x_no_more_bindings";

      else if (exc_matches(e,&rpc_x_no_more_elements)) t = "rpc_x_no_more_elements";

      else if (exc_matches(e,&rpc_x_not_found)) t = "rpc_x_not_found";

      else if (exc_matches(e,&rpc_x_not_rpc_entry)) t = "rpc_x_not_rpc_entry";

      else if (exc_matches(e,&rpc_x_profile_not_found)) t = "rpc_x_profile_not_found";

      else if (exc_matches(e,&rpc_x_unsupported_name_syntax)) t = "rpc_x_unsupported_name_syntax";


/* Stub support errors */
      if (exc_matches(e,&rpc_x_ss_char_trans_open_fail)) t = "rpc_x_ss_char_trans_open_fail";

      else if (exc_matches(e,&rpc_x_ss_char_trans_short_file)) t = "rpc_x_ss_char_trans_short_file";

      else if (exc_matches(e,&rpc_x_ss_context_damaged)) t = "rpc_x_ss_context_damaged";

      else if (exc_matches(e,&rpc_x_ss_context_mismatch)) t = "rpc_x_ss_context_mismatch";

      else if (exc_matches(e,&rpc_x_ss_in_null_context)) t = "rpc_x_ss_in_null_context";

      else if (exc_matches(e,&rpc_x_ss_pipe_closed)) t = "rpc_x_ss_pipe_closed";

      else if (exc_matches(e,&rpc_x_ss_pipe_comm_error)) t = "rpc_x_ss_pipe_comm_error";

      else if (exc_matches(e,&rpc_x_ss_pipe_discipline_error)) t = "rpc_x_ss_pipe_discipline_error";

      else if (exc_matches(e,&rpc_x_ss_pipe_empty)) t = "rpc_x_ss_pipe_empty";

      else if (exc_matches(e,&rpc_x_ss_pipe_memory)) t = "rpc_x_ss_pipe_memory";

      else if (exc_matches(e,&rpc_x_ss_pipe_order)) t = "rpc_x_ss_pipe_order";

      else if (exc_matches(e,&rpc_x_ss_remote_comm_failure)) t = "rpc_x_ss_remote_comm_failure";

      else if (exc_matches(e,&rpc_x_ss_remote_no_memory)) t = "rpc_x_ss_remote_no_memory";



      /* CMA exceptions */
      if (exc_matches(e,&exc_e_uninitexc)) t = "exc_e_uninitexc";
	else if (exc_matches(e,&exc_e_illaddr)) t = "exc_e_illaddr";
	else if (exc_matches(e,&exc_e_exquota)) t = "exc_e_exquota";
	else if (exc_matches(e,&exc_e_insfmem)) t = "exc_e_insfmem";
	else if (exc_matches(e,&exc_e_nopriv)) t = "exc_e_nopriv";
	else if (exc_matches(e,&exc_e_illinstr)) t = "exc_e_illinstr";
	else if (exc_matches(e,&exc_e_resaddr)) t = "exc_e_resaddr";
	else if (exc_matches(e,&exc_e_privinst)) t = "exc_e_privinst";
	else if (exc_matches(e,&exc_e_resoper)) t = "exc_e_resoper";
	else if (exc_matches(e,&exc_e_SIGTRAP)) t = "exc_e_SIGTRAP";
	else if (exc_matches(e,&exc_e_SIGIOT)) t = "exc_e_SIGIOT";
	else if (exc_matches(e,&exc_e_SIGEMT)) t = "exc_e_SIGEMT";
	else if (exc_matches(e,&exc_e_aritherr)) t = "exc_e_aritherr";
	else if (exc_matches(e,&exc_e_SIGSYS)) t = "exc_e_SIGSYS";
	else if (exc_matches(e,&exc_e_SIGPIPE)) t = "exc_e_SIGPIPE";
	else if (exc_matches(e,&exc_e_excpu)) t = "exc_e_excpu";
	else if (exc_matches(e,&exc_e_exfilsiz)) t = "exc_e_exfilsiz";
	else if (exc_matches(e,&exc_e_intovf)) t = "exc_e_intovf";
	else if (exc_matches(e,&exc_e_intdiv)) t = "exc_e_intdiv";
	else if (exc_matches(e,&exc_e_fltovf)) t = "exc_e_fltovf";
	else if (exc_matches(e,&exc_e_fltdiv)) t = "exc_e_fltdiv";
	else if (exc_matches(e,&exc_e_fltund)) t = "exc_e_fltund";
	else if (exc_matches(e,&exc_e_decovf)) t = "exc_e_decovf";
	else if (exc_matches(e,&exc_e_subrng)) t = "exc_e_subrng";
	else if (exc_matches(e,&cma_e_alerted)) t = "cma_e_alerted";
	else if (exc_matches(e,&cma_e_assertion)) t = "cma_e_assertion";
	else if (exc_matches(e,&cma_e_badparam)) t = "cma_e_badparam";
	else if (exc_matches(e,&cma_e_bugcheck)) t = "cma_e_bugcheck";
	else if (exc_matches(e,&cma_e_exit_thread)) t = "cma_e_exit_thread";
	else if (exc_matches(e,&cma_e_existence)) t = "cma_e_existence";
	else if (exc_matches(e,&cma_e_in_use)) t = "cma_e_in_use";
	else if (exc_matches(e,&cma_e_use_error)) t = "cma_e_use_error";
	else if (exc_matches(e,&cma_e_wrongmutex)) t = "cma_e_wrongmutex";
	else if (exc_matches(e,&cma_e_stackovf)) t = "cma_e_stackovf";
	else if (exc_matches(e,&cma_e_nostackmem)) t = "cma_e_nostackmem";
	else if (exc_matches(e,&cma_e_notcmastack)) t = "cma_e_notcmastack";
	else if (exc_matches(e,&cma_e_unimp)) t = "cma_e_unimp";
	else if (exc_matches(e,&cma_e_inialrpro)) t = "cma_e_inialrpro";
	else if (exc_matches(e,&cma_e_defer_q_full)) t = "cma_e_defer_q_full";
	else if (exc_matches(e,&cma_e_signal_q_full)) t = "cma_e_signal_q_full";
	else if (exc_matches(e,&cma_e_alert_nesting)) t = "cma_e_alert_nesting";


      /* 
       *  If we found a named exception that matches print it, otherwise check
       *  if it is a status and display that if possible.
       */
      if (t != NULL){
	  printf(" %s\n",error_text(e));
          printf("***FAILURE***: unexpected CMA exception %s\n",t);
      }
      else
      {
          if ( exc_set_status(e,t) == 0) 
              printf("***FAILURE***: CMA status exception %d (%s)\n", e, t);
          else if (e->kind == exc_kind_address_c)
              printf("***FAILURE***: unexpected user-defined CMA exception address 0x%x\n",(exc_address_t)&e);
          else
              printf("***FAILURE***: corrupted CMA exception\n",t);
      }
}
