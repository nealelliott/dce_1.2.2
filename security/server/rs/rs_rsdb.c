/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_rsdb.c,v $
 * Revision 1.1.14.1  1996/05/10  13:23:27  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/2  1996/03/27  17:47 UTC  hanfei
 * 	merge in global group work for DCE 1.2.2
 *
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/1  1996/03/15  15:47 UTC  hanfei
 * 	add handle for database conversion to DCE 1.2.2
 *
 * 	HP revision /main/DCE_1.2/1  1996/01/03  19:18 UTC  psn
 * 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
 * 	[1995/04/21  18:59 UTC  greg  /main/HPDCE02/SEC_migrate_merge/1]
 *
 * 	Fix global lock problem.
 * 	[1995/03/30  18:25 UTC  cuti  /main/HPDCE02/3]
 *
 * 	[1995/03/30  18:24 UTC  cuti  /main/HPDCE02/3]
 *
 * 	lock
 * 	[1995/03/30  18:22 UTC  cuti  /main/HPDCE02/cuti_mothra_bug/3]
 *
 * 	CHFts14111 bug fix
 * 	[1995/03/27  19:34 UTC  cuti  /main/HPDCE02/2]
 *
 * 	Change signature name of rsdb_shutdown.
 * 	[1995/03/23  18:42 UTC  cuti  /main/HPDCE02/cuti_mothra_bug/2]
 *
 * 	CHFts14111: Change to take boolean to decide if do checkpoint.
 * 	[1995/03/14  18:59 UTC  cuti  /main/HPDCE02/cuti_mothra_bug/1]
 *
 * 	merge
 * 	[1994/12/08  17:29 UTC  hanfei  /main/HPDCE02/1]
 *
 * 	merge
 * 	[1994/12/07  19:32 UTC  hanfei  /main/hanfei_cpi_bl2/1]
 *
 * 	add code to safe-check for an idl pointer
 * 	[1994/12/07  17:05 UTC  hanfei  /main/hanfei_cpi_bl1/2]
 *
 * 	work for rgy checkpoint to be configurable
 * 	[1994/12/05  14:49 UTC  hanfei  /main/hanfei_cpi_bl1/1]
 *
 * Revision 1.1.8.5  1994/08/25  20:47:42  sekhar
 * 	Put back SVC messages for fatal_st() and error_st() calls.
 * 	[1994/08/25  20:41:10  sekhar]
 * 
 * Revision 1.1.8.3  1994/08/04  16:14:04  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:12:32  mdf]
 * 
 * Revision 1.1.8.1  1994/06/02  21:20:34  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 
 * 	hp_sec_to_osf_3 drop
 * 	[1994/05/18  11:57:09  mdf]
 * 
 * 	HP revision /main/KK/ODESSA_2/1  1994/05/03  21:16 UTC  hanfei
 * 	 merge
 * 
 * 	HP revision /main/KK/hanfei_ch_master/1  1994/05/03  20:41 UTC  hanfei
 * 	label change master work
 * 
 * Revision 1.1.4.3  1993/12/17  22:23:58  hanfei
 * 	initialize - return error if state_dup_master
 * 	[1993/12/17  22:05:13  hanfei]
 * 
 * Revision 1.1.4.2  1993/12/16  19:12:07  hanfei
 * 	add action for state rs_c_state_becoming_slave.
 * 	[1993/12/10  13:56:58  hanfei]
 * 
 * 	Second replication drop: bl5
 * 	rsdb_shutdown takes lock_mode arg
 * 	rsdb_checkpt: small logic changes, mkey changes,
 * 	sleeps for shorter interval if can't do checkpt
 * 	[1992/10/06  17:28:20  burati]
 * 
 * Revision 1.1.2.3  1993/09/15  15:51:10  root
 * 	    Loading drop DCE1_0_3b03
 * 
 * Revision 1.1.2.8  1993/02/26  23:55:09  emartin
 * 	em_bl6f: rsdb_checkpt takes next_rep_state arg
 * 	 rsdb_initialize sets state to next_rep_state if
 * 	 in state renaming
 * 	[1993/02/22  23:03:45  emartin]
 * 
 * Revision 1.1.2.7  1993/02/19  17:14:01  ahop
 * 	dbcp: add backwards compatibility (database) code
 * 	      add rep_twrs to rsdb_initialize argument list
 * 	[1993/02/19  08:43:17  ahop]
 * 
 * Revision 1.1.2.6  1992/12/29  16:37:27  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:26:50  zeliff]
 * 
 * Revision 1.1.2.5  1992/11/20  15:53:35  ahop
 * 	set_base_path for mkey file.
 * 	[1992/11/13  20:59:21  ahop]
 * 
 * Revision 1.1.2.4  1992/10/07  20:37:13  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:19:34  sekhar]
 * 
 * Revision 1.1.2.3  1992/09/29  20:13:03  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:37:58  sekhar]
 * 
 * Revision 1.1.2.2  1992/08/31  18:27:23  sekhar
 * 	First security replication code drop
 * 	[1992/08/31  15:12:03  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:44:16  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  
 * Copyright (c) Hewlett-Packard Company 1991, 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
/*
 *      Registry Server - database interlude (server only code)
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = 
    "$Id: rs_rsdb.c,v 1.1.14.1 1996/05/10 13:23:27 arvind Exp $";
#endif

#include <sys/types.h>

#include <dce/dce.h>
#include <dce/nbase.h>
#include <dce/sec_daclmgr.h>

#include <rs_rsdb.h>
#include <rs_pvt.h>
#include <rs_state.h>
#include <rs_master.h>
#include <rs_ver.h>
#include <rs_log.h>
#include <rs_lock.h>
#include <rs_task.h>
#include <rs_mkey.h>
#include <rs_rep_util.h>

#include <rsdb_pvt.h>
#include <rsdb_compat_pvt.h>
#include <rsdb_compat.h>
#include <rsdb_file.h>
#include <rsdb.h>

#include <sec_svc.h>

PRIVATE void  *rsdb_checkpt_task (
    void    *arg
);

PRIVATE signed32    checkpt_interval = 60 * 60 * CHECKPT_INTERVAL_HRS;
PRIVATE signed32    short_checkpt_interval = 60 * 15;   /* 15 minutes */
PRIVATE signed32    checkpt_wait = 0;
PRIVATE signed32    checkpt_diff = 0;
PRIVATE boolean32   checkpt_now = false;


/*
 * r s d b _ i n i t i a l i z e
 */
PUBLIC void rsdb_initialize
  (
    rs_replica_twr_vec_p_t  rep_twrs,
    error_status_t          *st
  )
{
    uuid_t                rs_id;
    error_status_t        lst;
    unsigned32            db_vers;
    sec_acl_mgr_handle_t  sec_acl_mgr;

    rsdb_file_set_base_path(RGY_FILE_DIRECTORY);
    rsdb_file_mkey_set_base_path(MKEY_FILE_DIRECTORY);

    rs_state_init(st);

    db_vers = rs_state_get_version();

    if ( db_vers != rs_c_state_file_version ) {
    /* database is from earlier release with different 
     * database format. need to reformat it.
     * currently database formats had been changed from
     * dce 1.0.1 -> dce 1.0.2 and
     * dce 1.0.2 -> dce 1.1
     *
     * We infer the server software version from the 
     * database version and use it to configure the
     * security server acl manager.  We need to to do this
     * so that any acls created in the reformat process
     * will use the protection bits appropriate to
     * the software version.
     */
        if ( db_vers == rsdb_c_db_version_dce101 ) {
	    sec_acl_mgr_configure(sec_acl_mgr_config_stable, 
				  rs_c_software_vers_1_0_2,
				  (unsigned_char_p_t)NULL, 
				  &sec_acl_mgr, st);
            rsdb_compat_reformat_database(rep_twrs, st);
            if (BAD_STATUS(st)) return;
            rsdb_compat_reformat_database_for_dce11(rep_twrs, st);
            if (BAD_STATUS(st)) return;
	}

        if ( db_vers == rsdb_c_db_version_dce102 ||
	     db_vers == rsdb_c_db_version_pre_migrate_dce11 ) {
	    sec_acl_mgr_configure(sec_acl_mgr_config_stable, 
				  rs_c_software_vers_1_0_2,
				  (unsigned_char_p_t)NULL, 
				  &sec_acl_mgr, st);
            rsdb_compat_reformat_database_for_dce11(rep_twrs, st);
            if (BAD_STATUS(st)) return;
	}

	if ( db_vers <=  rsdb_c_db_version_dce11 ) {
	    rsdb_compat_reformat_database_for_dce122(rep_twrs, st);
	    if (BAD_STATUS(st)) return;
	}

        rs_state_init(st);
    }

    if (BAD_STATUS(st)) return;

    /* restore temporary states to their "real" states
     */
    switch(rs_state.rep_state) {
    case rs_c_state_initializing:
        rs_state_set_state(rs_c_state_uninitialized, st);
        if (BAD_STATUS(st)) return; 
        break;

    case rs_c_state_copying_dbase:
        rs_state_set_state(rs_c_state_in_service, st);
        if (BAD_STATUS(st)) return; 
        break;

    case rs_c_state_becoming_master:
    case rs_c_state_becoming_slave:
        rs_state_set_state(rs_c_state_in_service, st);
        if (BAD_STATUS(st)) return; 
        break;

    default:
        break;
    }

    switch(rs_state.rep_state) {
    case rs_c_state_unknown_to_master:
        dce_svc_printf(SEC_RGY_BAD_RGY_DB_MSG);
        SET_STATUS(st, sec_rgy_object_not_found);
        return;

    case rs_c_state_uninitialized:
    case rs_c_state_in_service:
    /* calling routine will check that
     * a new master_seqno was specified.
     */
    case rs_c_state_dup_master:
        rsdb_delete_files(".new", st);
        rsdb_load_init(rs_state.rep_state, st);
        if (BAD_STATUS(st)) return;
        break;

    case rs_c_state_renaming:
        rsdb_rename_files(".new", "", st);
        if (BAD_STATUS(st)) return;
        rs_state_set_state(rs_state.next_rep_state, st);
        if (BAD_STATUS(st)) return;
        rsdb_load_init(rs_state.rep_state, st);
        if (BAD_STATUS(st)) return;
        break;

    case rs_c_state_in_maintenance:
        rsdb_load_init(rs_state.rep_state, st);
        if (BAD_STATUS(st)) return;
        break;

    case rs_c_state_mkey_changing:
        rsdb_delete_files(".new", st);
        rsdb_load_init(rs_state.rep_state, st);
        if (BAD_STATUS(st)) return;
        rs_state_set_state(rs_c_state_in_service, st);
        if (BAD_STATUS(st)) return;
        /* secd crashed during an mkey_change attempt; retry it */
        lock_demote_global();
        rs_mkey_change(st);
        lock_promote_global();
        if (BAD_STATUS(st)) return;
        break;

    case rs_c_state_deleted:
        rsdb_clean_dir(st);
        rsdb_state_delete();
        SET_STATUS(st, sec_rgy_object_not_found);
        break;

    default:
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_rsdb,
                       svc_c_sev_error,
                       sec_rgy_rep_pgmerr);
        break;
    }

    return;
}

/*
 * r s d b _ l o a d _ i n i t
 */
PUBLIC void rsdb_load_init
  (
    unsigned32          rep_state,
    error_status_t      *st
  )
{
    unsigned32            software_version;
    sec_acl_mgr_handle_t  acl_mgr_handle;

    rsdb_load(st);
    if (BAD_STATUS(st)) return;

    rs_master_info_init(st);
    if (BAD_STATUS(st)) return;

    /*
     * Configure ACL manager with current server
     * software version.
     */
    rs_master_info_get_sw_rev(&software_version, st);
    if (BAD_STATUS(st)) return;
    sec_acl_mgr_configure(sec_acl_mgr_config_stable, 
                          software_version,
			  (unsigned_char_p_t)NULL,
			  &acl_mgr_handle, st);
    if (BAD_STATUS(st)) return;


    rs_mkey_init(rep_state, st);
    if (BAD_STATUS(st)) return;

    rs_log_replay(st);
    if (BAD_STATUS(st)) return;
}

/*
 * r s d b _ s h u t d o w n
 */
PUBLIC void rsdb_shutdown
  (
    boolean32       checkpoint,
    lock_mode_t     lock_mode
  )
{
    error_status_t   status;

    if (lock_mode == lock_mode_riw) {
        lock_promote_global();
	lock_mode = lock_mode_write;
    }
	
    if (checkpoint)
        rsdb_checkpt(rs_state.rep_state, lock_mode, &status);

    rsdb_close_files();
}

/*
 * r s d b _ s e t _ c h e c k p t _ i n t e r v a l
 */
PUBLIC void rsdb_set_checkpt_interval
  (
    signed32    interval
  )
{
    checkpt_interval = interval;
}

/*
 * r s d b _ s t a r t _ c h e c k p t _ t a s k
 */
PUBLIC void rsdb_start_checkpt_task
  (
    error_status_t   *st
  )
{
    rs_task_handle_t  unused;

    if ( ! rs_task_create(&unused, rsdb_checkpt_task,
              RS_CHECKPT_THR, false, NULL)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_chkpt,
                       svc_c_sev_fatal | svc_c_action_abort,
                       sec_rs_thr_exit_creat_fail);
    }
}

/*
 * r s d b _ c h e c k p t _ r e s e t _ i n t e r v a l
 *
 */
PUBLIC void  rsdb_checkpt_reset_interval
  (
    boolean32   do_checkpt_now,
    signed32	new_checkpt_interval,
    char	*at_time_str,
    error_status_t    *status
  )
{
    boolean32	lock_taken=false;
    unsigned32	wait_interval=0;

    CLEAR_STATUS(status);

    if ( at_time_str && at_time_str[0] != '\0' ) {
        unsigned32      at_hour;
        unsigned32      at_minute;
        if ( ! rs_util_checkpt_parse_time(at_time_str, &at_hour, &at_minute)) {
            SET_STATUS(status, sec_rgy_checkpt_bad_at_time_str);
	    return;
        }
        if ( ! rs_util_checkpt_calc_interval(at_hour, at_minute,
                (unsigned32 *)&new_checkpt_interval, &wait_interval) ) {
            SET_STATUS(status, sec_rgy_checkpt_bad_at_time_str);
	    return;
        }
    }

    if (rs_state.rep_state == rs_c_state_in_service) {
        lock(lock_db, lock_mode_riw);
        lock_taken = true;
    }

    checkpt_now = do_checkpt_now;

    checkpt_wait = wait_interval;
    if ( new_checkpt_interval > 0 ) {
	/* 
	 * only when there is a new interval passed, we need to 
	 * calculate the interval difference
         */
    	checkpt_diff = new_checkpt_interval - checkpt_interval;
    } else /* case for do the checkpoint now */
	checkpt_diff = 0;
     

    if ( new_checkpt_interval > 0 ) {
    	checkpt_interval = new_checkpt_interval;
    }

    unlock(lock_db, lock_mode_riw);
    lock_taken = false;

    if ( ! rs_task_sleep_cancel() ) {
        SET_STATUS(status, sec_s_pgmerr);
    }
   
}

/*
 * r s d b _ c h e c k p t _ n e e d _ w a i t
 *
 */
PRIVATE boolean32 rsdb_checkpt_need_wait
  (
    signed32	checkpt_interval,
    signed32	checkpt_wait,
    signed32	checkpt_diff,
    signed32	*sleep_time
  )
{
    boolean32	lock_taken=false;

    *sleep_time = 0;

    if ( checkpt_now ) {
	return false;
    }

    if (rs_state.rep_state == rs_c_state_in_service) {
        lock(lock_db, lock_mode_riw);
        lock_taken = true;
    }

    if ( checkpt_wait > 0 ) {
	/* 
	 * for the case checkpoint at a specific time 
	 * use pre-calculated wait time
	 */
        *sleep_time = checkpt_wait;
    }
    else
	*sleep_time += checkpt_diff;

    unlock(lock_db, lock_mode_riw);
    lock_taken = false;

    if ( *sleep_time <= 0 )
	return false;
    else
	return true;

}

/*
 * r s d b _ c h e c k p t _ t a s k
 *
 * Standard locking macros are not used in this 
 * routine so that the TRY/CATCH exception handling 
 * setup macros can be invoked once outside the loop
 */
PRIVATE void  *rsdb_checkpt_task
  (
    void    *arg
  )
{
    error_status_t  status;
    signed32        sleep_interval;
    signed32        sleep_residual=0;
    boolean32       lock_taken;

    dce_svc_printf(SEC_RS_THR_STARTED_MSG , RS_CHECKPT_THR);
    rs_task_init(arg);

    if ( ! rs_task_sleep_init() ) {
	dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
               sec_s_rs_chkpt,
               svc_c_sev_fatal | svc_c_action_abort,
               sec_thr_exit_exc);
    }

    sleep_interval = checkpt_interval;
    lock_taken = false;

    TRY {

        while (true) {

            if (rs_state_exiting()) break;
	
	    sleep_residual = 0;

            rs_task_sleep_interruptable(sleep_interval, &sleep_residual, 
			&status);
	    if ( BAD_STATUS(&status) ) {
		dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
               	sec_s_rs_chkpt,
               	svc_c_sev_fatal | svc_c_action_abort,
               	sec_thr_exit_exc);
    	    }

	    while ( sleep_residual > 0 ) {
	    	if ( rsdb_checkpt_need_wait(checkpt_interval, checkpt_wait, 
			checkpt_diff, &sleep_residual) ) {
		    sleep_interval = sleep_residual;
                    rs_task_sleep_interruptable(sleep_interval, 
			&sleep_residual, &status);
	    	    if ( BAD_STATUS(&status) ) {
			dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
               		sec_s_rs_chkpt,
               		svc_c_sev_fatal | svc_c_action_abort,
               		sec_thr_exit_exc);
		    }
	    	}
		else
		    break;
	    }
    
            if (rs_state.rep_state == rs_c_state_in_service) {
                lock_global(lock_mode_riw);
                lock_taken = true;
    
                rsdb_checkpt(rs_state.rep_state, lock_mode_riw, 
                               &status);
    
		checkpt_wait = 0;
		checkpt_now = false;
		checkpt_diff = 0;
                sleep_interval = checkpt_interval;

                unlock_global(lock_mode_riw);
                lock_taken = false;

            }
            else {
                /* not in good state for checkpt
                 * sleep for shorter interval
                 */
                sleep_interval = (short_checkpt_interval < checkpt_interval) ?
                    short_checkpt_interval:checkpt_interval;
            }
        }

    } CATCH (cma_e_alerted) {
        if (lock_taken)
            unlock_global(lock_mode_riw);
    
    } CATCH_ALL {
        if (lock_taken)
            unlock_global(lock_mode_riw);
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_chkpt,
                       svc_c_sev_fatal | svc_c_action_abort,
                       sec_thr_exit_exc);
    }

    ENDTRY;
    dce_svc_printf(SEC_RS_THR_EXITING_MSG , RS_CHECKPT_THR);
}

/* r s d b _ c h e c k p t
 *
 * Save the in-memory copy of the database to disk.
 * Set state to next_rep_state after database has
 * been saved.
 *
 * Invoking routine must lock_global_riw or lock_global_write 
 * if it needs a lock (ie. is multi-threaded or rpc_listening)
 */
PUBLIC void rsdb_checkpt
  (
    unsigned32      next_rep_state,
    lock_mode_t     lock_mode,
    error_status_t  *st
  )
{
    int new_log_len;

    CLEAR_STATUS(st);

    switch(rs_state.rep_state) {
        case rs_c_state_initializing: 
            break;
        case rs_c_state_in_service: 
            break;
        case rs_c_state_mkey_changing: 
            break;

        default:
            dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                           sec_s_rs_rsdb, 
                           svc_c_sev_error,
                           sec_rgy_rep_bad_state);
            SET_STATUS(st, sec_rgy_rep_bad_state);
            return;
    }

    rsdb_delete_files(".new", st);
    if (BAD_STATUS(st)) return;

    rsdb_save(".new", st);
    if (BAD_STATUS(st))
        SEC_SVC_PRINT_MSG_ST(SEC_RGY_RSDB_CHECKPT_MSG, *st);

    rs_log_truncate(".new", st);
    if (BAD_STATUS(st))
        SEC_SVC_PRINT_MSG_ST(SEC_RGY_CHECKPT_LOG_FILE_MSG, *st);

    if (lock_mode == lock_mode_riw)
        lock_promote_global();

    rs_state_set_state_renaming(next_rep_state, st);
    if (BAD_STATUS(st)) return;

    rsdb_close_files();

    rsdb_rename_files(".new", "", st);
    if (BAD_STATUS(st))
        SEC_SVC_PRINT_MSG_ST(SEC_RGY_CHECKPT_RENAME_FILES_MSG, *st);

    rsdb_open_files();

    rs_state_set_state(next_rep_state, st);
    if (BAD_STATUS(st))
        SEC_SVC_PRINT_MSG_ST(SEC_RGY_CHECKPT_SAVE_REP_STATE_MSG, *st);
}

