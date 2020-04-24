/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: exception_print.c,v $
 * Revision 1.1.12.1  1996/10/02  20:59:36  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:47:43  damon]
 *
 * Revision 1.1.7.2  1994/07/13  22:26:47  devsrc
 * 	merged with bl-10
 * 	[1994/06/28  21:14:11  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  16:01:51  mbs]
 * 
 * Revision 1.1.7.1  1994/06/09  14:19:58  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:31:41  annie]
 * 
 * Revision 1.1.3.3  1993/01/21  15:20:03  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  15:58:53  cjd]
 * 
 * Revision 1.1.3.2  1992/11/24  19:41:10  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:25:10  bolinger]
 * 
 * Revision 1.1  1992/01/19  02:53:30  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *	exception_print.c -- print out the appropriate exception string
 *
 * Copyright (C) 1991 Transarc Corporation
 * All rights reserved.
 */

#include <stdio.h>

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>

#include <dce/rpcexc.h>

#include <exception_print.h>		/* and pthread stuff */

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/security/exception_print.c,v 1.1.12.1 1996/10/02 20:59:36 damon Exp $")

EXPORT char * ExceptionToString(thisCatchP)
     EXCEPTION *	thisCatchP;
{
  if (exc_matches(thisCatchP, &pthread_cancel_e)) 
    return "Thread cancellation in progress";

#if !defined(KERNEL)
  else if (exc_matches(thisCatchP, &exc_uninitexc_e))
    return "Uninitialized exception raised";
  
  else if (exc_matches(thisCatchP, &exc_illaddr_e))
    return "(exc_illaddr_e) exception raised";
  
  else if (exc_matches(thisCatchP, &exc_exquota_e))
    return "Operation failed due to insufficient quota";
  
  else if (exc_matches(thisCatchP, &exc_insfmem_e))
    return "Insufficient virtual memory for requested operation";
  
  else if (exc_matches(thisCatchP, &exc_nopriv_e))
    return "Insufficient priviledge for requested operation";
  
  else if (exc_matches(thisCatchP, &exc_illinstr_e))
    return "(exc_illinstr_e) exception raised";
  
  else if (exc_matches(thisCatchP, &exc_resaddr_e))
    return "(exc_resaddr_e) exception raised";
  
  else if (exc_matches(thisCatchP, &exc_privinst_e))
    return "(exc_privinst_e) exception raised";
  
  else if (exc_matches(thisCatchP, &exc_resoper_e))
    return "(exc_resoper_e) exception raised";
  
  else if (exc_matches(thisCatchP, &exc_SIGTRAP_e))
    return "Unhandled trace or breakpoint trap signal";
  
  else if (exc_matches(thisCatchP, &exc_SIGIOT_e))
    return "Unhandled IOT trap signal";
  
  else if (exc_matches(thisCatchP, &exc_SIGEMT_e))
    return "Unhandled EMT trap signal";
  
  else if (exc_matches(thisCatchP, &exc_aritherr_e))
    return "(exc_aritherr_e) exception raised";
  
  else if (exc_matches(thisCatchP, &exc_SIGSYS_e))
    return "Unhandled bad system call signal";
  
  else if (exc_matches(thisCatchP, &exc_SIGPIPE_e))
    return "Unhandled broken pipe signal";
  
  else if (exc_matches(thisCatchP, &exc_excpu_e))
    return "(exc_excpu_e) exception raised";
  
  else if (exc_matches(thisCatchP, &exc_exfilsiz_e))
    return "(exc_exfilsiz_e) exception raised";
  
  else if (exc_matches(thisCatchP, &exc_intovf_e))
    return "Unhandled integer overflow trap exception";
  
  else if (exc_matches(thisCatchP, &exc_intdiv_e))
    return "Unhandled integer divide by zero trap exception";
  
  else if (exc_matches(thisCatchP, &exc_fltovf_e))
    return "Unhandled floating point overflow trap exception";
	   
  
  else if (exc_matches(thisCatchP, &exc_fltdiv_e))
    return "Unhandled floating point/decimal divide by zero trap exception";
  
  else if (exc_matches(thisCatchP, &exc_fltund_e))
    return "Unhandled floating point underflow trap exception";
  
  else if (exc_matches(thisCatchP, &exc_decovf_e))
    return "Unhandled decimal overflow trap exception";
  
  else if (exc_matches(thisCatchP, &exc_subrng_e))
    return "Unhandled subscript out of range trap exception";
  
  else if (exc_matches(thisCatchP, &pthread_assertion_e))
    return "An assertion check has failed";
  
  else if (exc_matches(thisCatchP, &pthread_badparam_e))
    return "Improper parameter";
  
  else if (exc_matches(thisCatchP, &pthread_bugcheck_e))
    return "Internal error detected";
  
  else if (exc_matches(thisCatchP, &pthread_exit_thread_e))
    return "Current thread has been requested to exit";
  
  else if (exc_matches(thisCatchP, &pthread_existence_e))
    return "Object referenced does not exist";
  
  else if (exc_matches(thisCatchP, &pthread_in_use_e))
    return "Object referenced is already in use";
  
  else if (exc_matches(thisCatchP, &pthread_use_error_e))
    return "Requested operation is improperly invoked";
  
  else if (exc_matches(thisCatchP, &pthread_wrongmutex_e))
    return "(pthread_wrongmutex_e) exception raised";
  
  else if (exc_matches(thisCatchP, &pthread_stackovf_e))
    return "Attempted stack overflow was detected";
  
  else if (exc_matches(thisCatchP, &pthread_nostackmem_e))
    return "No space is currently available to create a new stack";
	   
  else if (exc_matches(thisCatchP, &pthread_notstack_e))
    return "The current stack has not been allocated";
  
  else if (exc_matches(thisCatchP, &pthread_unimp_e))
    return "Unimplemented feature";
  
  else if (exc_matches(thisCatchP, &pthread_inialrpro_e))
    return "(pthread_inialrpro_e) exception raised";
  
  else if (exc_matches(thisCatchP, &pthread_defer_q_full_e))
    return "No space is currently available to process an interrupt request";
  
  else if (exc_matches(thisCatchP, &pthread_signal_q_full_e))
    return "(pthread_signal_q_full_e) exception raised";
  
  else if (exc_matches(thisCatchP, &exc_accvio_e))
    return "Data or object could not be referenced";
  
  else if (exc_matches(thisCatchP, &exc_SIGILL_e))
    return "Unhandled illegal instruction signal";
  
  else if (exc_matches(thisCatchP, &exc_SIGFPE_e))
    return "Unhandled floating point exception signal";
  
  else if (exc_matches(thisCatchP, &exc_SIGBUS_e))
    return "Unhandled bus error signal";
  
  else if (exc_matches(thisCatchP, &exc_SIGSEGV_e))
    return "Unhandled segmentation violation signal";
  
  else if (exc_matches(thisCatchP, &exc_SIGXCPU_e))
    return "Unhandled CPU-time limit exceeded signal";
  
  else if (exc_matches(thisCatchP, &exc_SIGXFSZ_e))
    return "Unhandled file-size limit exceeded signal";
    
#endif /* !defined(KERNEL) */
    
  /* RPC exceptions */
    
  /* DG and common errors */
  else if (exc_matches(thisCatchP, &rpc_x_assoc_grp_not_found))
    return "(rpc_x_assoc_grp_not_found) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_cancel_timeout))
    return "(rpc_x_cancel_timeout) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_coding_error))
    return "(rpc_x_coding_error) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_comm_failure))
    return "(rpc_x_comm_failure) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_context_id_not_found))
    return "(rpc_x_context_id_not_found) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_endpoint_not_found))
    return "(rpc_x_endpoint_not_found) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_in_args_too_big))
    return "(rpc_x_in_args_too_big) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_invalid_binding))
    return "(rpc_x_invalid_binding) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_invalid_bound))
    return "(rpc_x_invalid_bound) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_invalid_call_opt))
    return "(rpc_x_invalid_call_opt) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_invalid_naf_id))
    return "(rpc_x_invalid_naf_id) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_invalid_rpc_protseq))
    return "(rpc_x_invalid_rpc_protseq) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_invalid_tag))
    return "(rpc_x_invalid_tag) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_invalid_timeout))
    return "(rpc_x_invalid_timeout) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_manager_not_entered))
    return "(rpc_x_manager_not_entered) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_max_descs_exceeded))
    return "(rpc_x_max_descs_exceeded) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_no_fault))
    return "(rpc_x_no_fault) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_no_memory))
    return "(rpc_x_no_memory) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_not_rpc_tower))
    return "(rpc_x_not_rpc_tower) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_object_not_found))
    return "(rpc_x_object_not_found) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_op_rng_error))
    return "(rpc_x_op_rng_error) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_protocol_error))
    return "(rpc_x_protocol_error) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_protseq_not_supported))
    return "(rpc_x_protseq_not_supported) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_rpcd_comm_failure))
    return "(rpc_x_rpcd_comm_failure) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_server_too_busy))
    return "(rpc_x_server_too_busy) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_unknown_if))
    return "(rpc_x_unknown_if) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_unknown_error))
    return "(rpc_x_unknown_error) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_unknown_mgr_type))
    return "(rpc_x_unknown_mgr_type) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_unknown_reject))
    return "(rpc_x_unknown_reject) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_unknown_remote_fault))
    return "(rpc_x_unknown_remote_fault) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_unsupported_type))
    return "(rpc_x_unsupported_type) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_who_are_you_failed))
    return "(rpc_x_who_are_you_failed) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_wrong_boot_time))
    return "(rpc_x_wrong_boot_time) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_wrong_kind_of_binding))
    return "(rpc_x_wrong_kind_of_binding) exception raised";
  
  else if (exc_matches(thisCatchP, &uuid_x_getconf_failure))
    return "(uuid_x_getconf_failure) exception raised";
  
  else if (exc_matches(thisCatchP, &uuid_x_internal_error))
    return "(uuid_x_internal_error) exception raised";
  
  else if (exc_matches(thisCatchP, &uuid_x_no_address))
    return "(uuid_x_no_address) exception raised";
  
  else if (exc_matches(thisCatchP, &uuid_x_socket_failure))
    return "(uuid_x_socket_failure) exception raised";
  
  
  /* CN errors */
  else if (exc_matches(thisCatchP, &rpc_x_access_control_info_inv))
    return "(rpc_x_access_control_info_inv) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_assoc_grp_max_exceeded))
    return "(rpc_x_assoc_grp_max_exceeded) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_assoc_shutdown))
    return "(rpc_x_assoc_shutdown) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_cannot_accept))
    return "(rpc_x_cannot_accept) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_cannot_connect))
    return "(rpc_x_cannot_connect) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_cant_inq_socket))
    return "(rpc_x_cant_inq_socket) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_connect_closed_by_rem))
    return "(rpc_x_connect_closed_by_rem) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_connect_no_resources))
    return "(rpc_x_connect_no_resources) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_connect_rejected))
    return "(rpc_x_connect_rejected) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_connect_timed_out))
    return "(rpc_x_connect_timed_out) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_connection_aborted))
    return "(rpc_x_connection_aborted) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_connection_closed))
    return "(rpc_x_connection_closed) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_host_unreachable))
    return "(rpc_x_host_unreachable) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_invalid_endpoint_format))
    return "(rpc_x_invalid_endpoint_format) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_loc_connect_aborted))
    return "(rpc_x_loc_connect_aborted) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_network_unreachable))
    return "(rpc_x_network_unreachable) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_no_rem_endpoint))
    return "(rpc_x_no_rem_endpoint) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_rem_host_crashed))
    return "(rpc_x_rem_host_crashed) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_rem_host_down))
    return "(rpc_x_rem_host_down) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_rem_network_shutdown))
    return "(rpc_x_rem_network_shutdown) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_rpc_prot_version_mismatch))
    return "(rpc_x_rpc_prot_version_mismatch) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_string_too_long))
    return "(rpc_x_string_too_long) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_too_many_rem_connects))
    return "(rpc_x_too_many_rem_connects) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_tsyntaxes_unsupported))
    return "(rpc_x_tsyntaxes_unsupported) exception raised";
  
  
  /* NS import routine errors */
  else if (exc_matches(thisCatchP, &rpc_x_binding_vector_full))
    return "(rpc_x_binding_vector_full) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_entry_not_found))
    return "(rpc_x_entry_not_found) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_group_not_found))
    return "(rpc_x_group_not_found) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_incomplete_name))
    return "(rpc_x_incomplete_name) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_invalid_arg))
    return "(rpc_x_invalid_arg) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_invalid_import_context))
    return "(rpc_x_invalid_import_context) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_invalid_inquiry_context))
    return "(rpc_x_invalid_inquiry_context) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_invalid_inquiry_type))
    return "(rpc_x_invalid_inquiry_type) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_invalid_lookup_context))
    return "(rpc_x_invalid_lookup_context) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_invalid_name_syntax))
    return "(rpc_x_invalid_name_syntax) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_invalid_object))
    return "(rpc_x_invalid_object) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_invalid_vers_option))
    return "(rpc_x_invalid_vers_option) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_name_service_unavailable))
    return "(rpc_x_name_service_unavailable) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_no_env_setup))
    return "(rpc_x_no_env_setup) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_no_more_bindings))
    return "(rpc_x_no_more_bindings) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_no_more_elements))
    return "(rpc_x_no_more_elements) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_not_found))
    return "(rpc_x_not_found) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_not_rpc_entry))
    return "(rpc_x_not_rpc_entry) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_obj_uuid_not_found))
    return "(rpc_x_obj_uuid_not_found) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_profile_not_found))
    return "(rpc_x_profile_not_found) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_unsupported_name_syntax))
    return "(rpc_x_unsupported_name_syntax) exception raised";
  
  /* Stub support errors */
  else if (exc_matches(thisCatchP, &rpc_x_ss_char_trans_open_fail))
    return "(rpc_x_ss_char_trans_open_fail) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_ss_char_trans_short_file))
    return "(rpc_x_ss_char_trans_short_file) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_ss_context_damaged))
    return "(rpc_x_ss_context_damaged) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_ss_context_mismatch))
    return "(rpc_x_ss_context_mismatch) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_ss_in_null_context))
    return "(rpc_x_ss_in_null_context) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_ss_pipe_closed))
    return "(rpc_x_ss_pipe_closed) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_ss_pipe_comm_error))
    return "(rpc_x_ss_pipe_comm_error) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_ss_pipe_discipline_error))
    return "(rpc_x_ss_pipe_discipline_error) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_ss_pipe_empty))
    return "(rpc_x_ss_pipe_empty) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_ss_pipe_memory))
    return "(rpc_x_ss_pipe_memory) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_ss_pipe_order))
    return "(rpc_x_ss_pipe_order) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_ss_remote_comm_failure))
    return "(rpc_x_ss_remote_comm_failure) exception raised";
  
  else if (exc_matches(thisCatchP, &rpc_x_ss_remote_no_memory))
    return "(rpc_x_ss_remote_no_memory) exception raised";
    
#ifdef notdef
  else if (exc_matches(thisCatchP, &rpc_x_ss_report_status))
    return "(rpc_x_ss_report_status) exception raised";
    
#endif /* notdef */

  else
    return (char *)NULL;
}

EXPORT void PrintException(thisCatchP, routineNameP, tagP, filenameP, line)
     EXCEPTION *	thisCatchP;
     char *		routineNameP;
     char *		tagP;
     char *		filenameP;
     int		line;
{
  char *	thisCatchStringP = ExceptionToString(thisCatchP);
  
  osi_printf("%s: %s: %s\t%s, %d\n",
	     ((routineNameP) ? routineNameP : ""), ((tagP) ? tagP : ""),
	     (thisCatchStringP) ? thisCatchStringP : "unknown exception",
	     ((filenameP) ? filenameP : ""), line);
}

