/* Generated from dhd.sams on 2020-04-26-00:42:48.000 */
/* Do not edit! */
#if	!defined(_DCE_DCEDHDMAC_)
#define _DCE_DCEDHDMAC_
#define DCED_S_BAD_BINDING_MSG \
	DCE_SVC(dhd_svc_handle, ""), dhd_s_general, \
	svc_c_sev_error, dced_s_bad_binding
#define DCED_S_NO_MEMORY_MSG \
	DCE_SVC(dhd_svc_handle, ""), dhd_s_general, \
	svc_c_sev_error, dced_s_no_memory
#define DCED_S_INVALID_ARG_MSG \
	DCE_SVC(dhd_svc_handle, ""), dhd_s_general, \
	svc_c_sev_error, dced_s_invalid_arg
#define DCED_S_NO_MORE_ENTRIES_MSG \
	DCE_SVC(dhd_svc_handle, ""), dhd_s_general, \
	svc_c_sev_error, dced_s_no_more_entries
#define DCED_S_NOT_FOUND_MSG \
	DCE_SVC(dhd_svc_handle, ""), dhd_s_general, \
	svc_c_sev_error, dced_s_not_found
#define DCED_S_NO_SUPPORT_MSG \
	DCE_SVC(dhd_svc_handle, ""), dhd_s_general, \
	svc_c_sev_error, dced_s_no_support
#define DCED_S_UNKNOWN_ATTR_TYPE_MSG \
	DCE_SVC(dhd_svc_handle, ""), dhd_s_general, \
	svc_c_sev_error, dced_s_unknown_attr_type
#define DCED_S_UNKNOWN_LIST_TYPE_MSG \
	DCE_SVC(dhd_svc_handle, ""), dhd_s_general, \
	svc_c_sev_error, dced_s_unknown_list_type
#define DCED_S_UNKNOWN_OBJECT_TYPE_MSG \
	DCE_SVC(dhd_svc_handle, ""), dhd_s_general, \
	svc_c_sev_error, dced_s_unknown_object_type
#define DCED_S_UNKNOWN_SERVICE_MSG \
	DCE_SVC(dhd_svc_handle, ""), dhd_s_general, \
	svc_c_sev_error, dced_s_unknown_service
#define DCED_S_NOT_STARTED_BY_DCED_MSG \
	DCE_SVC(dhd_svc_handle, ""), dhd_s_general, \
	svc_c_sev_error, dced_s_not_started_by_dced
#define DCED_S_NEED_ONE_SERVER_PRIN_MSG \
	DCE_SVC(dhd_svc_handle, ""), dhd_s_general, \
	svc_c_sev_error, dced_s_need_one_server_prin
#define DCED_S_NO_SERVER_KEYFILE_MSG \
	DCE_SVC(dhd_svc_handle, ""), dhd_s_general, \
	svc_c_sev_error, dced_s_no_server_keyfile
#define DCED_S_DATA_UNAVAILABLE_MSG \
	DCE_SVC(dhd_svc_handle, ""), dhd_s_general, \
	svc_c_sev_error, dced_s_data_unavailable
#define DCED_S_NAME_MISSING_MSG \
	DCE_SVC(dhd_svc_handle, ""), dhd_s_general, \
	svc_c_sev_error, dced_s_name_missing
#define DCED_S_CANNOT_CREATE_KEY_MGMT_THR_MSG \
	DCE_SVC(dhd_svc_handle, ""), dhd_s_general, \
	svc_c_sev_error, dced_s_cannot_create_key_mgmt_thr
#define DCED_S_CANNOT_DETACH_KEY_MGMT_THR_MSG \
	DCE_SVC(dhd_svc_handle, ""), dhd_s_general, \
	svc_c_sev_error, dced_s_cannot_detach_key_mgmt_thr
#define DCED_S_SERVER_ATTR_NOT_FOUND_MSG \
	DCE_SVC(dhd_svc_handle, ""), dhd_s_general, \
	svc_c_sev_warning, dced_s_server_attr_not_found
#define DCED_S_EMPTY_REPLIST_MSG \
	DCE_SVC(dhd_svc_handle, ""), dhd_s_general, \
	svc_c_sev_error, dced_s_empty_replist
#define DCED_S_CAUGHT_SIGNAL_MSG \
	DCE_SVC(dhd_svc_handle, "%d"), dhd_s_general, \
	svc_c_sev_notice, dced_s_caught_signal
#define DCED_S_CANT_REGISTER_SERVER_MSG \
	DCE_SVC(dhd_svc_handle, "%s"), dhd_s_general, \
	svc_c_sev_warning, dced_s_cant_register_server
#define DCED_S_CANT_NSI_UNEXPORT_MSG \
	DCE_SVC(dhd_svc_handle, "%s"), dhd_s_general, \
	svc_c_sev_warning, dced_s_cant_nsi_unexport
#define DCED_S_CANT_USE_PROTSEQ_MSG \
	DCE_SVC(dhd_svc_handle, "%s%s"), dhd_s_general, \
	svc_c_sev_fatal | svc_c_action_exit_bad, dced_s_cant_use_protseq
#define DCED_S_POSTPROCESSORS_NOT_SET_MSG \
	DCE_SVC(dhd_svc_handle, ""), dhd_s_general, \
	svc_c_sev_warning, dced_s_postprocessors_not_set
#define DCED_S_CANT_OPEN_DATABASE_MSG \
	DCE_SVC(dhd_svc_handle, "%s%d"), dhd_s_general, \
	svc_c_sev_fatal, dced_s_cant_open_database
#define DCED_S_IS_LISTENING_MSG \
	DCE_SVC(dhd_svc_handle, ""), dhd_s_general, \
	svc_c_sev_notice, dced_s_is_listening
#define DCED_S_BACKGROUND_EXIT_MSG \
	DCE_SVC(dhd_svc_handle, "%ld%o"), dhd_s_general, \
	svc_c_sev_error, dced_s_background_exit
#define DCED_S_BACKGROUND_WAIT_EXIT_MSG \
	DCE_SVC(dhd_svc_handle, "%d%ld%s"), dhd_s_general, \
	svc_c_sev_error, dced_s_background_wait_exit
#define DCED_S_INIT_CREATE_FILE_FAIL_MSG \
	DCE_SVC(dhd_svc_handle, "%s%s"), dhd_s_general, \
	svc_c_sev_notice, dced_s_init_create_file_fail
#define DCED_S_INIT_REMOVE_FILE_FAIL_MSG \
	DCE_SVC(dhd_svc_handle, "%s%s"), dhd_s_general, \
	svc_c_sev_fatal | svc_c_action_exit_bad, dced_s_init_remove_file_fail
#define DCED_S_INIT_FUNCTION_FAIL_MSG \
	DCE_SVC(dhd_svc_handle, "%s%lx"), dhd_s_general, \
	svc_c_sev_notice, dced_s_init_function_fail
#define DCED_S_CANT_FETCH_IACL_MSG \
	DCE_SVC(dhd_svc_handle, "%s%lx"), dhd_s_general, \
	svc_c_sev_error, dced_s_cant_fetch_iacl
#define DCED_S_CANT_STORE_ACL_MSG \
	DCE_SVC(dhd_svc_handle, "%s%lx"), dhd_s_general, \
	svc_c_sev_error, dced_s_cant_store_acl
#define DCED_S_CANT_DELETE_ACL_MSG \
	DCE_SVC(dhd_svc_handle, "%s%lx"), dhd_s_general, \
	svc_c_sev_error, dced_s_cant_delete_acl
#define DCED_S_CANT_STORE_OBJECT_MSG \
	DCE_SVC(dhd_svc_handle, "%s%lx"), dhd_s_general, \
	svc_c_sev_error, dced_s_cant_store_object
#define DCED_S_CANT_DELETE_OBJECT_MSG \
	DCE_SVC(dhd_svc_handle, "%s%lx"), dhd_s_general, \
	svc_c_sev_error, dced_s_cant_delete_object
#define DCED_S_ITER_FAIL_MSG \
	DCE_SVC(dhd_svc_handle, "%s%lx"), dhd_s_general, \
	svc_c_sev_error, dced_s_iter_fail
#define DCED_S_THREAD_ERROR_MSG \
	DCE_SVC(dhd_svc_handle, "%s"), dhd_s_general, \
	svc_c_sev_error, dced_s_thread_error
#define DCED_S_INIT_GLOBAL_DATA_FAIL_MSG \
	DCE_SVC(dhd_svc_handle, "%lx"), dhd_s_general, \
	svc_c_sev_error, dced_s_init_global_data_fail
#define DCED_S_INIT_ACL_FAIL_MSG \
	DCE_SVC(dhd_svc_handle, "%s%s%lx"), dhd_s_general, \
	svc_c_sev_error, dced_s_init_acl_fail
#define DCED_S_CANT_FETCH_ACL_MSG \
	DCE_SVC(dhd_svc_handle, "%s%lx"), dhd_s_general, \
	svc_c_sev_error, dced_s_cant_fetch_acl
#define DCED_S_LOCK_FAIL_MSG \
	DCE_SVC(dhd_svc_handle, "%s%s%lx"), dhd_s_general, \
	svc_c_sev_error, dced_s_lock_fail
#define DCED_S_UNLOCK_FAIL_MSG \
	DCE_SVC(dhd_svc_handle, "%s%s%lx"), dhd_s_general, \
	svc_c_sev_error, dced_s_unlock_fail
#define DCED_S_CANT_INIT_SOMETHING_MSG \
	DCE_SVC(dhd_svc_handle, "%s%lx"), dhd_s_general, \
	svc_c_sev_fatal | svc_c_action_exit_bad, dced_s_cant_init_something
#define DCED_S_BAD_PROTSEQ_MSG \
	DCE_SVC(dhd_svc_handle, "%s"), dhd_s_general, \
	svc_c_sev_fatal | svc_c_action_exit_bad, dced_s_bad_protseq
#define DCED_S_CANT_STOP_SOMETHING_MSG \
	DCE_SVC(dhd_svc_handle, "%s%lx"), dhd_s_general, \
	svc_c_sev_fatal | svc_c_action_exit_bad, dced_s_cant_stop_something
#define DCED_S_CANT_EXPORT_YET_MSG \
	DCE_SVC(dhd_svc_handle, "%lx"), dhd_s_general, \
	svc_c_sev_notice_verbose, dced_s_cant_export_yet
#define DCED_S_WAITING_FOR_SOMETHING_MSG \
	DCE_SVC(dhd_svc_handle, "%s%lx"), dhd_s_general, \
	svc_c_sev_notice_verbose, dced_s_waiting_for_something
#define DCED_S_SVC_FAIL_MSG \
	DCE_SVC(dhd_svc_handle, "%s%lx"), dhd_s_general, \
	svc_c_sev_error, dced_s_svc_fail
#define DCED_S_BAD_KEYDIR_MSG \
	DCE_SVC(dhd_svc_handle, ""), dhd_s_general, \
	svc_c_sev_fatal, dced_s_bad_keydir
#define DCED_S_POSTPROCESSOR_OK_MSG \
	DCE_SVC(dhd_svc_handle, "%s"), dhd_s_hostdata, \
	svc_c_sev_notice_verbose, dced_s_postprocessor_ok
#define DCED_S_POSTPROCESSOR_BAD_MSG \
	DCE_SVC(dhd_svc_handle, "%s%d"), dhd_s_hostdata, \
	svc_c_sev_error, dced_s_postprocessor_bad
#define DCED_S_HD_POSTPROC_FAILED_MSG \
	DCE_SVC(dhd_svc_handle, "%s"), dhd_s_hostdata, \
	svc_c_sev_warning, dced_s_hd_postproc_failed
#define DCED_S_SC_OPEN_FILE_FAILED_MSG \
	DCE_SVC(dhd_svc_handle, "%d"), dhd_s_srvrconf, \
	svc_c_sev_error, dced_s_sc_open_file_failed
#define DCED_S_SC_SETID_FAILED_MSG \
	DCE_SVC(dhd_svc_handle, "%s%d"), dhd_s_srvrconf, \
	svc_c_sev_warning, dced_s_sc_setid_failed
#define DCED_S_SC_CHDIR_FAILED_MSG \
	DCE_SVC(dhd_svc_handle, "%s%d"), dhd_s_srvrconf, \
	svc_c_sev_warning, dced_s_sc_chdir_failed
#define DCED_S_SC_EXEC_FAILED_MSG \
	DCE_SVC(dhd_svc_handle, "%s%d"), dhd_s_srvrconf, \
	svc_c_sev_warning, dced_s_sc_exec_failed
#define DCED_S_SC_BOOT_START_FAILED_MSG \
	DCE_SVC(dhd_svc_handle, "%s%lx"), dhd_s_srvrconf, \
	svc_c_sev_warning, dced_s_sc_boot_start_failed
#define DCED_S_SX_PROSTPROC_EXIT_MSG \
	DCE_SVC(dhd_svc_handle, "%d"), dhd_s_srvrexec, \
	svc_c_sev_notice, dced_s_sx_prostproc_exit
#define DCED_S_SX_SERVER_EXIT_MSG \
	DCE_SVC(dhd_svc_handle, "%s%d"), dhd_s_srvrexec, \
	svc_c_sev_warning, dced_s_sx_server_exit
#define DCED_S_SX_REAPING_SERVER_MSG \
	DCE_SVC(dhd_svc_handle, "%s"), dhd_s_srvrexec, \
	svc_c_sev_notice, dced_s_sx_reaping_server
#define DCED_S_SX_STARTING_SERVER_MSG \
	DCE_SVC(dhd_svc_handle, "%s"), dhd_s_srvrexec, \
	svc_c_sev_notice, dced_s_sx_starting_server
#define DCED_S_SX_WAITPID_FAILED_MSG \
	DCE_SVC(dhd_svc_handle, "%s"), dhd_s_srvrexec, \
	svc_c_sev_error, dced_s_sx_waitpid_failed
#define DCED_S_SX_KILL_FAILED_MSG \
	DCE_SVC(dhd_svc_handle, "%ld%d"), dhd_s_srvrexec, \
	svc_c_sev_error, dced_s_sx_kill_failed
#define DCED_S_SV_SEC_LOGIN_FAIL_MSG \
	DCE_SVC(dhd_svc_handle, "%lx"), dhd_s_secval, \
	svc_c_sev_error, dced_s_sv_sec_login_fail
#define DCED_S_SV_SEC_KEY_MGMT_FAIL_MSG \
	DCE_SVC(dhd_svc_handle, "%lx"), dhd_s_secval, \
	svc_c_sev_error, dced_s_sv_sec_key_mgmt_fail
#define DCED_S_SV_OPEN_BINDINGS_FAIL_MSG \
	DCE_SVC(dhd_svc_handle, "%s"), dhd_s_secval, \
	svc_c_sev_error, dced_s_sv_open_bindings_fail
#define DCED_S_SV_WRITE_BINDINGS_FAIL_MSG \
	DCE_SVC(dhd_svc_handle, "%lx"), dhd_s_secval, \
	svc_c_sev_error, dced_s_sv_write_bindings_fail
#define DCED_S_SV_LOAD_OVERRIDE_FAIL_MSG \
	DCE_SVC(dhd_svc_handle, "%lx"), dhd_s_secval, \
	svc_c_sev_error, dced_s_sv_load_override_fail
#define DCED_S_SV_BAD_OVERRIDE_ENTRY_MSG \
	DCE_SVC(dhd_svc_handle, "%s"), dhd_s_secval, \
	svc_c_sev_warning, dced_s_sv_bad_override_entry
#define DCED_S_SV_UPDATE_PE_SITE_FAIL_MSG \
	DCE_SVC(dhd_svc_handle, "%lx"), dhd_s_secval, \
	svc_c_sev_error, dced_s_sv_update_pe_site_fail
#define DCED_S_ATTR_BAD_NAME_MSG \
	DCE_SVC(dhd_svc_handle, ""), dhd_s_xattrschema, \
	svc_c_sev_error, dced_s_attr_bad_name
#define DCED_S_ATTR_BAD_SCH_NAME_MSG \
	DCE_SVC(dhd_svc_handle, ""), dhd_s_xattrschema, \
	svc_c_sev_error, dced_s_attr_bad_sch_name
#define DCED_S_ATTR_BAD_ENCODING_TYPE_MSG \
	DCE_SVC(dhd_svc_handle, ""), dhd_s_xattrschema, \
	svc_c_sev_error, dced_s_attr_bad_encoding_type
#define DCED_S_ATTR_BAD_ACL_MGR_SET_MSG \
	DCE_SVC(dhd_svc_handle, ""), dhd_s_xattrschema, \
	svc_c_sev_error, dced_s_attr_bad_acl_mgr_set
#define DCED_S_ATTR_INVALID_MANAGER_TYPE_MSG \
	DCE_SVC(dhd_svc_handle, ""), dhd_s_xattrschema, \
	svc_c_sev_error, dced_s_attr_invalid_manager_type
#define DCED_S_ATTR_ACL_BAD_PERMSET_MSG \
	DCE_SVC(dhd_svc_handle, ""), dhd_s_xattrschema, \
	svc_c_sev_error, dced_s_attr_acl_bad_permset
#define DCED_S_ATTR_BAD_ENTRY_FLAG_MSG \
	DCE_SVC(dhd_svc_handle, ""), dhd_s_xattrschema, \
	svc_c_sev_error, dced_s_attr_bad_entry_flag
#define DCED_S_ATTR_BAD_INTERCELL_ACTION_MSG \
	DCE_SVC(dhd_svc_handle, ""), dhd_s_xattrschema, \
	svc_c_sev_error, dced_s_attr_bad_intercell_action
#define DCED_S_ATTR_BAD_TRIG_TYPES_MSG \
	DCE_SVC(dhd_svc_handle, ""), dhd_s_xattrschema, \
	svc_c_sev_error, dced_s_attr_bad_trig_types
#define DCED_S_ATTR_BAD_UNIQ_QUERY_ACCEPT_MSG \
	DCE_SVC(dhd_svc_handle, ""), dhd_s_xattrschema, \
	svc_c_sev_error, dced_s_attr_bad_uniq_query_accept
#define DCED_S_ATTR_BAD_TRIG_BINDING_MSG \
	DCE_SVC(dhd_svc_handle, ""), dhd_s_xattrschema, \
	svc_c_sev_error, dced_s_attr_bad_trig_binding
#define DCED_S_ATTR_NOT_SUPPORTED_FOR_UPDATE_MSG \
	DCE_SVC(dhd_svc_handle, ""), dhd_s_xattrschema, \
	svc_c_sev_error, dced_s_attr_not_supported_for_update
#define DCED_S_MUTEX_INIT_FAIL_MSG \
	DCE_SVC(dhd_svc_handle, ""), dhd_s_general, \
	svc_c_sev_fatal | svc_c_action_exit_bad, dced_s_mutex_init_fail
#define DCED_S_COND_INIT_FAIL_MSG \
	DCE_SVC(dhd_svc_handle, ""), dhd_s_general, \
	svc_c_sev_fatal | svc_c_action_exit_bad, dced_s_cond_init_fail
#define DCED_S_MUTEX_LOCK_FAILED_MSG \
	DCE_SVC(dhd_svc_handle, ""), dhd_s_general, \
	svc_c_sev_fatal | svc_c_action_exit_bad, dced_s_mutex_lock_failed
#define DCED_S_MUTEX_UNLOCK_FAILED_MSG \
	DCE_SVC(dhd_svc_handle, ""), dhd_s_general, \
	svc_c_sev_fatal | svc_c_action_exit_bad, dced_s_mutex_unlock_failed
#define DCED_S_COND_WAIT_FAILED_MSG \
	DCE_SVC(dhd_svc_handle, ""), dhd_s_general, \
	svc_c_sev_fatal | svc_c_action_exit_bad, dced_s_cond_wait_failed
#define DCED_S_COND_SIGNAL_FAILED_MSG \
	DCE_SVC(dhd_svc_handle, ""), dhd_s_general, \
	svc_c_sev_fatal | svc_c_action_exit_bad, dced_s_cond_signal_failed
#define DCED_S_COND_BCAST_FAILED_MSG \
	DCE_SVC(dhd_svc_handle, ""), dhd_s_general, \
	svc_c_sev_fatal | svc_c_action_exit_bad, dced_s_cond_bcast_failed
#define DCED_S_PKCACHE_USAGE_UNSUPP_MSG \
	DCE_SVC(dhd_svc_handle, ""), dhd_s_pkcache, \
	svc_c_sev_error, dced_s_pkcache_usage_unsupp
#define DCED_S_PKCACHE_KEY_WRITE_FAILURE_MSG \
	DCE_SVC(dhd_svc_handle, ""), dhd_s_pkcache, \
	svc_c_sev_error, dced_s_pkcache_key_write_failure
#define DCED_S_PKCACHE_CELL_INVALID_MSG \
	DCE_SVC(dhd_svc_handle, ""), dhd_s_pkcache, \
	svc_c_sev_error, dced_s_pkcache_cell_invalid
#define DCED_S_PKCACHE_MKDIR_ERROR_MSG \
	DCE_SVC(dhd_svc_handle, ""), dhd_s_pkcache, \
	svc_c_sev_error, dced_s_pkcache_mkdir_error
#define DCED_S_PKCACHE_KEY_FRESH_MSG \
	DCE_SVC(dhd_svc_handle, ""), dhd_s_pkcache, \
	svc_c_sev_error, dced_s_pkcache_key_fresh
#endif	/* !defined(_DCE_DCEDHDMAC_) */
