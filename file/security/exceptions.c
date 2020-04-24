/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: exceptions.c,v $
 * Revision 1.1.9.1  1996/10/02  20:59:39  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:47:44  damon]
 *
 * Revision 1.1.4.1  1994/06/09  14:20:02  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:31:44  annie]
 * 
 * Revision 1.1.2.2  1993/01/21  15:20:14  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  15:59:06  cjd]
 * 
 * Revision 1.1  1992/01/19  02:53:23  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *	exceptions.c -- catch all the exceptions I could find & print out a message
 * describing which exception has been caught
 *
 * Copyright (C) 1991 - Transarc Corporation
 * All rights reserved.
 */



CATCH (pthread_cancel_e)
     fprintf(stderr, "%s: Thread cancellation in progress\n", routineName);
     rtnVal = 2;

#if !defined(KERNEL)
CATCH (exc_uninitexc_e)
     fprintf(stderr, "%s: Uninitialized exception raised\n", routineName);
     rtnVal = 2;
CATCH (exc_illaddr_e)
     fprintf(stderr, "%s: (exc_illaddr_e) exception raised\n", routineName);
     rtnVal = 2;
CATCH (exc_exquota_e)
     fprintf(stderr, "%s: Operation failed due to insufficient quota\n", routineName);
     rtnVal = 2;
CATCH (exc_insfmem_e)
     fprintf(stderr, "%s: Insufficient virtual memory for requested operation\n",
	     routineName);
     rtnVal = 2;
CATCH (exc_nopriv_e)
     fprintf(stderr, "%s: Insufficient priviledge for requested operation\n", routineName);
     rtnVal = 2;
CATCH (exc_illinstr_e)
     fprintf(stderr, "%s: (exc_illinstr_e) exception raised\n", routineName);
     rtnVal = 2;
CATCH (exc_resaddr_e)
     fprintf(stderr, "%s: (exc_resaddr_e) exception raised\n", routineName);
     rtnVal = 2;
CATCH (exc_privinst_e)
     fprintf(stderr, "%s: (exc_privinst_e) exception raised\n", routineName);
     rtnVal = 2;
CATCH (exc_resoper_e)
     fprintf(stderr, "%s: (exc_resoper_e) exception raised\n", routineName);
     rtnVal = 2;
CATCH (exc_SIGTRAP_e)
     fprintf(stderr, "%s: Unhandled trace or breakpoint trap signal\n", routineName);
     rtnVal = 2;
CATCH (exc_SIGIOT_e)
     fprintf(stderr, "%s: Unhandled IOT trap signal\n", routineName);
     rtnVal = 2;
CATCH (exc_SIGEMT_e)
     fprintf(stderr, "%s: Unhandled EMT trap signal\n", routineName);
     rtnVal = 2;
CATCH (exc_aritherr_e)
     fprintf(stderr, "%s: (exc_aritherr_e) exception raised\n", routineName);
     rtnVal = 2;
CATCH (exc_SIGSYS_e)
     fprintf(stderr, "%s: Unhandled bad system call signal\n", routineName);
     rtnVal = 2;
CATCH (exc_SIGPIPE_e)
     fprintf(stderr, "%s: Unhandled broken pipe signal\n", routineName);
     rtnVal = 2;
CATCH (exc_excpu_e)
     fprintf(stderr, "%s: (exc_excpu_e) exception raised\n", routineName);
     rtnVal = 2;
CATCH (exc_exfilsiz_e)
     fprintf(stderr, "%s: (exc_exfilsiz_e) exception raised\n", routineName);
     rtnVal = 2;
CATCH (exc_intovf_e)
     fprintf(stderr, "%s: Unhandled integer overflow trap exception\n", routineName);
     rtnVal = 2;
CATCH (exc_intdiv_e)
     fprintf(stderr, "%s: Unhandled integer divide by zero trap exception\n", routineName);
     rtnVal = 2;
CATCH (exc_fltovf_e)
     fprintf(stderr, "%s: Unhandled floating point overflow trap exception\n",
	     routineName);
     rtnVal = 2;
CATCH (exc_fltdiv_e)
     fprintf(stderr,
	     "%s: Unhandled floating point/decimal divide by zero trap exception\n",
	     routineName);
     rtnVal = 2;
CATCH (exc_fltund_e)
     fprintf(stderr, "%s: Unhandled floating point underflow trap exception\n",
	     routineName);
     rtnVal = 2;
CATCH (exc_decovf_e)
     fprintf(stderr, "%s: Unhandled decimal overflow trap exception\n", routineName);
     rtnVal = 2;
CATCH (exc_subrng_e)
     fprintf(stderr, "%s: Unhandled subscript out of range trap exception\n", routineName);
     rtnVal = 2;
CATCH (pthread_assertion_e)
     fprintf(stderr, "%s: An assertion check has failed\n", routineName);
     rtnVal = 2;
CATCH (pthread_badparam_e)
     fprintf(stderr, "%s: Improper parameter\n", routineName);
     rtnVal = 2;
CATCH (pthread_bugcheck_e)
     fprintf(stderr, "%s: Internal error detected\n", routineName);
     rtnVal = 2;
CATCH (pthread_exit_thread_e)
     fprintf(stderr, "%s: Current thread has been requested to exit\n", routineName);
     rtnVal = 2;
CATCH (pthread_existence_e)
     fprintf(stderr, "%s: Object referenced does not exist\n", routineName);
     rtnVal = 2;
CATCH (pthread_in_use_e)
     fprintf(stderr, "%s: Object referenced is already in use\n", routineName);
     rtnVal = 2;
CATCH (pthread_use_error_e)
     fprintf(stderr, "%s: Requested operation is improperly invoked\n", routineName);
     rtnVal = 2;
CATCH (pthread_wrongmutex_e)
     fprintf(stderr, "%s: (pthread_wrongmutex_e) exception raised\n", routineName);
     rtnVal = 2;
CATCH (pthread_stackovf_e)
     fprintf(stderr, "%s: Attempted stack overflow was detected\n", routineName);
     rtnVal = 2;
CATCH (pthread_nostackmem_e)
     fprintf(stderr, "%s: No space is currently available to create a new stack\n",
	     routineName);
     rtnVal = 2;
CATCH (pthread_notstack_e)
     fprintf(stderr, "%s: The current stack has not been allocated\n", routineName);
     rtnVal = 2;
CATCH (pthread_unimp_e)
     fprintf(stderr, "%s: Unimplemented feature\n", routineName);
     rtnVal = 2;
CATCH (pthread_inialrpro_e)
     fprintf(stderr, "%s: (pthread_inialrpro_e) exception raised\n", routineName);
     rtnVal = 2;
CATCH (pthread_defer_q_full_e)
     fprintf(stderr,
	     "%s: No space is currently available to process an interrupt request\n",
	     routineName);
     rtnVal = 2;
CATCH (pthread_signal_q_full_e)
     fprintf(stderr, "%s: (pthread_signal_q_full_e) exception raised\n", routineName);
     rtnVal = 2;

CATCH (exc_accvio_e)
     fprintf(stderr, "%s: Data or object could not be referenced\n", routineName);
     rtnVal = 2;
CATCH (exc_SIGILL_e)
     fprintf(stderr, "%s: Unhandled illegal instruction signal\n", routineName);
     rtnVal = 2;
CATCH (exc_SIGFPE_e)
     fprintf(stderr, "%s: Unhandled floating point exception signal\n", routineName);
     rtnVal = 2;
CATCH (exc_SIGBUS_e)
     fprintf(stderr, "%s: Unhandled bus error signal\n", routineName);
     rtnVal = 2;
CATCH (exc_SIGSEGV_e)
     fprintf(stderr, "%s: Unhandled segmentation violation signal\n", routineName);
     rtnVal = 2;
CATCH (exc_SIGXCPU_e)
     fprintf(stderr, "%s: Unhandled CPU-time limit exceeded signal\n", routineName);
     rtnVal = 2;
CATCH (exc_SIGXFSZ_e)
     fprintf(stderr, "%s: Unhandled file-size limit exceeded signal\n", routineName);
     rtnVal = 2;
#endif /* !defined(KERNEL) */
     
/* RPC exceptions */

/* DG and common errors */
CATCH (rpc_x_assoc_grp_not_found)
     fprintf(stderr, "%s: (rpc_x_assoc_grp_not_found) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_call_failed)
     fprintf(stderr, "%s: (rpc_x_call_failed) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_cancel_timeout)
     fprintf(stderr, "%s: (rpc_x_cancel_timeout) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_coding_error)
     fprintf(stderr, "%s: (rpc_x_coding_error) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_comm_failure)
     fprintf(stderr, "%s: (rpc_x_comm_failure) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_context_id_not_found)
     fprintf(stderr, "%s: (rpc_x_context_id_not_found) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_endpoint_not_found)
     fprintf(stderr, "%s: (rpc_x_endpoint_not_found) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_in_args_too_big)
     fprintf(stderr, "%s: (rpc_x_in_args_too_big) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_invalid_binding)
     fprintf(stderr, "%s: (rpc_x_invalid_binding) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_invalid_bound)
     fprintf(stderr, "%s: (rpc_x_invalid_bound) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_invalid_call_opt)
     fprintf(stderr, "%s: (rpc_x_invalid_call_opt) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_invalid_naf_id)
     fprintf(stderr, "%s: (rpc_x_invalid_naf_id) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_invalid_rpc_protseq)
     fprintf(stderr, "%s: (rpc_x_invalid_rpc_protseq) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_invalid_tag)
     fprintf(stderr, "%s: (rpc_x_invalid_tag) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_invalid_timeout)
     fprintf(stderr, "%s: (rpc_x_invalid_timeout) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_manager_not_entered)
     fprintf(stderr, "%s: (rpc_x_manager_not_entered) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_max_descs_exceeded)
     fprintf(stderr, "%s: (rpc_x_max_descs_exceeded) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_no_fault)
     fprintf(stderr, "%s: (rpc_x_no_fault) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_no_memory)
     fprintf(stderr, "%s: (rpc_x_no_memory) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_not_rpc_tower)
     fprintf(stderr, "%s: (rpc_x_not_rpc_tower) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_object_not_found)
     fprintf(stderr, "%s: (rpc_x_object_not_found) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_op_rng_error)
     fprintf(stderr, "%s: (rpc_x_op_rng_error) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_protocol_error)
     fprintf(stderr, "%s: (rpc_x_protocol_error) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_protseq_not_supported)
     fprintf(stderr, "%s: (rpc_x_protseq_not_supported) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_rpcd_comm_failure)
     fprintf(stderr, "%s: (rpc_x_rpcd_comm_failure) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_server_too_busy)
     fprintf(stderr, "%s: (rpc_x_server_too_busy) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_unknown_if)
     fprintf(stderr, "%s: (rpc_x_unknown_if) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_unknown_error)
     fprintf(stderr, "%s: (rpc_x_unknown_error) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_unknown_mgr_type)
     fprintf(stderr, "%s: (rpc_x_unknown_mgr_type) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_unknown_reject)
     fprintf(stderr, "%s: (rpc_x_unknown_reject) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_unknown_remote_fault)
     fprintf(stderr, "%s: (rpc_x_unknown_remote_fault) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_unsupported_type)
     fprintf(stderr, "%s: (rpc_x_unsupported_type) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_who_are_you_failed)
     fprintf(stderr, "%s: (rpc_x_who_are_you_failed) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_wrong_boot_time)
     fprintf(stderr, "%s: (rpc_x_wrong_boot_time) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_wrong_kind_of_binding)
     fprintf(stderr, "%s: (rpc_x_wrong_kind_of_binding) exception raised\n", routineName);
     rtnVal = 3;
CATCH (uuid_x_getconf_failure)
     fprintf(stderr, "%s: (uuid_x_getconf_failure) exception raised\n", routineName);
     rtnVal = 3;
CATCH (uuid_x_internal_error)
     fprintf(stderr, "%s: (uuid_x_internal_error) exception raised\n", routineName);
     rtnVal = 3;
CATCH (uuid_x_no_address)
     fprintf(stderr, "%s: (uuid_x_no_address) exception raised\n", routineName);
     rtnVal = 3;
CATCH (uuid_x_socket_failure)
     fprintf(stderr, "%s: (uuid_x_socket_failure) exception raised\n", routineName);
     rtnVal = 3;

/* CN errors */
CATCH (rpc_x_access_control_info_inv)
     fprintf(stderr, "%s: (rpc_x_access_control_info_inv) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_assoc_grp_max_exceeded)
     fprintf(stderr, "%s: (rpc_x_assoc_grp_max_exceeded) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_assoc_shutdown)
     fprintf(stderr, "%s: (rpc_x_assoc_shutdown) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_cannot_accept)
     fprintf(stderr, "%s: (rpc_x_cannot_accept) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_cannot_connect)
     fprintf(stderr, "%s: (rpc_x_cannot_connect) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_cant_bind_sock)
     fprintf(stderr, "%s: (rpc_x_cant_bind_sock) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_cant_create_sock)
     fprintf(stderr, "%s: (rpc_x_cant_create_sock) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_cant_inq_socket)
     fprintf(stderr, "%s: (rpc_x_cant_inq_socket) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_cant_listen_sock)
     fprintf(stderr, "%s: (rpc_x_cant_listen_sock) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_connect_closed_by_rem)
     fprintf(stderr, "%s: (rpc_x_connect_closed_by_rem) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_connect_no_resources)
     fprintf(stderr, "%s: (rpc_x_connect_no_resources) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_connect_rejected)
     fprintf(stderr, "%s: (rpc_x_connect_rejected) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_connect_timed_out)
     fprintf(stderr, "%s: (rpc_x_connect_timed_out) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_connection_aborted)
     fprintf(stderr, "%s: (rpc_x_connection_aborted) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_connection_closed)
     fprintf(stderr, "%s: (rpc_x_connection_closed) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_host_unreachable)
     fprintf(stderr, "%s: (rpc_x_host_unreachable) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_invalid_endpoint_format)
     fprintf(stderr, "%s: (rpc_x_invalid_endpoint_format) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_loc_connect_aborted)
     fprintf(stderr, "%s: (rpc_x_loc_connect_aborted) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_network_unreachable)
     fprintf(stderr, "%s: (rpc_x_network_unreachable) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_no_rem_endpoint)
     fprintf(stderr, "%s: (rpc_x_no_rem_endpoint) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_rem_host_crashed)
     fprintf(stderr, "%s: (rpc_x_rem_host_crashed) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_rem_host_down)
     fprintf(stderr, "%s: (rpc_x_rem_host_down) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_rem_network_shutdown)
     fprintf(stderr, "%s: (rpc_x_rem_network_shutdown) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_rpc_prot_version_mismatch)
     fprintf(stderr, "%s: (rpc_x_rpc_prot_version_mismatch) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_string_too_long)
     fprintf(stderr, "%s: (rpc_x_string_too_long) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_too_many_rem_connects)
     fprintf(stderr, "%s: (rpc_x_too_many_rem_connects) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_tsyntaxes_unsupported)
     fprintf(stderr, "%s: (rpc_x_tsyntaxes_unsupported) exception raised\n", routineName);
     rtnVal = 3;

/* NS import routine errors */
CATCH (rpc_x_binding_vector_full)
     fprintf(stderr, "%s: (rpc_x_binding_vector_full) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_entry_not_found)
     fprintf(stderr, "%s: (rpc_x_entry_not_found) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_group_not_found)
     fprintf(stderr, "%s: (rpc_x_group_not_found) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_incomplete_name)
     fprintf(stderr, "%s: (rpc_x_incomplete_name) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_invalid_arg)
     fprintf(stderr, "%s: (rpc_x_invalid_arg) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_invalid_import_context)
     fprintf(stderr, "%s: (rpc_x_invalid_import_context) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_invalid_inquiry_context)
     fprintf(stderr, "%s: (rpc_x_invalid_inquiry_context) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_invalid_inquiry_type)
     fprintf(stderr, "%s: (rpc_x_invalid_inquiry_type) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_invalid_lookup_context)
     fprintf(stderr, "%s: (rpc_x_invalid_lookup_context) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_invalid_name_syntax)
     fprintf(stderr, "%s: (rpc_x_invalid_name_syntax) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_invalid_object)
     fprintf(stderr, "%s: (rpc_x_invalid_object) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_invalid_vers_option)
     fprintf(stderr, "%s: (rpc_x_invalid_vers_option) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_name_service_unavailable)
     fprintf(stderr, "%s: (rpc_x_name_service_unavailable) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_no_env_setup)
     fprintf(stderr, "%s: (rpc_x_no_env_setup) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_no_more_bindings)
     fprintf(stderr, "%s: (rpc_x_no_more_bindings) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_no_more_elements)
     fprintf(stderr, "%s: (rpc_x_no_more_elements) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_no_ns_privilege)
     fprintf(stderr, "%s: (rpc_x_no_ns_privilege) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_not_found)
     fprintf(stderr, "%s: (rpc_x_not_found) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_not_rpc_entry)
     fprintf(stderr, "%s: (rpc_x_not_rpc_entry) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_obj_uuid_not_found)
     fprintf(stderr, "%s: (rpc_x_obj_uuid_not_found) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_profile_not_found)
     fprintf(stderr, "%s: (rpc_x_profile_not_found) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_unsupported_name_syntax)
     fprintf(stderr, "%s: (rpc_x_unsupported_name_syntax) exception raised\n", routineName);
     rtnVal = 3;

/* Stub support errors */
CATCH (rpc_x_ss_char_trans_open_fail)
     fprintf(stderr, "%s: (rpc_x_ss_char_trans_open_fail) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_ss_char_trans_short_file)
     fprintf(stderr, "%s: (rpc_x_ss_char_trans_short_file) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_ss_context_damaged)
     fprintf(stderr, "%s: (rpc_x_ss_context_damaged) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_ss_context_mismatch)
     fprintf(stderr, "%s: (rpc_x_ss_context_mismatch) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_ss_in_null_context)
     fprintf(stderr, "%s: (rpc_x_ss_in_null_context) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_ss_pipe_closed)
     fprintf(stderr, "%s: (rpc_x_ss_pipe_closed) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_ss_pipe_comm_error)
     fprintf(stderr, "%s: (rpc_x_ss_pipe_comm_error) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_ss_pipe_discipline_error)
     fprintf(stderr, "%s: (rpc_x_ss_pipe_discipline_error) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_ss_pipe_empty)
     fprintf(stderr, "%s: (rpc_x_ss_pipe_empty) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_ss_pipe_memory)
     fprintf(stderr, "%s: (rpc_x_ss_pipe_memory) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_ss_pipe_order)
     fprintf(stderr, "%s: (rpc_x_ss_pipe_order) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_ss_remote_comm_failure)
     fprintf(stderr, "%s: (rpc_x_ss_remote_comm_failure) exception raised\n", routineName);
     rtnVal = 3;
CATCH (rpc_x_ss_remote_no_memory)
     fprintf(stderr, "%s: (rpc_x_ss_remote_no_memory) exception raised\n", routineName);
     rtnVal = 3;
#ifdef notdef
CATCH (rpc_x_ss_report_status)
     fprintf(stderr, "%s: (rpc_x_ss_report_status) exception raised\n", routineName);
     rtnVal = 3;
#endif /* notdef */

