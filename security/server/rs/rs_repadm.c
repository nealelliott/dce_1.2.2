/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_repadm.c,v $
 * Revision 1.1.11.3  1996/02/18  00:19:45  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:08:00  marty]
 *
 * Revision 1.1.11.2  1995/12/08  17:55:51  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/05/15  21:08 UTC  greg
 * 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
 * 	[1995/04/21  18:59 UTC  greg  /main/HPDCE02/SEC_migrate_merge/1]
 * 
 * 	HP revision /main/HPDCE02/1  1995/03/27  19:34 UTC  cuti
 * 	CHFts14111 bug fix
 * 
 * 	HP revision /main/cuti_mothra_bug/2  1995/03/23  18:42 UTC  cuti
 * 	Add comment for true in rs_rep_mgmt_shutdown() call.
 * 
 * 	HP revision /main/cuti_mothra_bug/1  1995/03/14  18:57 UTC  cuti
 * 	Add boolean to rsdb_shutdown()
 * 	[1995/12/08  17:21:28  root]
 * 
 * Revision 1.1.7.3  1994/07/15  15:02:13  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:21:52  mdf]
 * 
 * Revision 1.1.7.2  1994/06/02  21:23:25  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:19:55  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 	[1994/05/18  11:56:58  mdf]
 * 
 * Revision 1.1.4.4  1993/12/20  22:31:31  hanfei
 * 	rs_rep_mgmt_start_master - restore old args
 * 	[1993/12/20  22:21:37  hanfei]
 * 
 * Revision 1.1.4.3  1993/12/16  19:11:48  hanfei
 * 	become slave not supported
 * 	[1993/12/16  18:17:23  hanfei]
 * 
 * 	rework for become -master/-slave
 * 	[1993/12/15  22:21:27  hanfei]
 * 
 * 	rework for change master.
 * 	[1993/12/11  18:49:58  hanfei]
 * 
 * 	rs_rep_admin_change_master - move most of code to rs_rep_mgmt.c
 * 	[1993/12/10  13:55:32  hanfei]
 * 
 * 	*** empty log message ***
 * 	[1993/12/08  04:39:28  hanfei]
 * 
 * 	bl6c: do init_replica, info_full
 * 	master_change operations retunr not_implemented error
 * 	[1992/12/11  21:53:33  emartin]
 * 
 * Revision 1.1.4.2  1993/10/29  17:16:15  hanfei
 * 	add codes for change master (force become).
 * 	[1993/10/29  17:15:27  hanfei]
 * 
 * 	rs_rep_admin_init_replica, destroy - chk for different permissions
 * 	[1993/03/09  22:13:53  emartin]
 * 
 * 	7339: rs_rep_admin_maint - return ok if already in requested state.
 * 	[1993/03/01  21:52:14  ahop]
 * 
 * 	em_bl6f: destroy call rpc_mgmt_stop_server_listening
 * 	[1993/02/11  20:21:46  emartin]
 * 
 * 	Embedding copyright notice
 * 	[1992/12/28  21:26:18  zeliff]
 * 
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:53:39  sekhar]
 * 
 * 	Third replication drop: bl6a
 * 	out_of_maint invokes rs_rep_mgmt_start_master
 * 	[1992/11/03  18:47:38  ahop]
 * 
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:19:07  sekhar]
 * 
 * 	First replication code drop.
 * 	[1992/08/31  17:55:12  sekhar]
 * 
 * Revision 1.1.1.6  1993/05/04  20:39:39  mgm
 * 	    Loading drop DCE1_0_2FNL
 * 
 * Revision 1.1.3.2  1992/10/06  17:26:25  burati
 * 	Second replication drop: bl5
 * 	Do rs_rep_admin_stop, _mkey, _maint, _reset
 * 
 * $EndLog$
 */

/*
 * Copyright (c) Hewlett-Packard Company 1992
 * Unpublished work. All Rights Reserved.
 */
/*    
 *      Registry replica administration - server manager routines.
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = 
    "$Id: rs_repadm.c,v 1.1.11.3 1996/02/18 00:19:45 marty Exp $";
    "$Id: rs_repadm.c,v 1.1.11.3 1996/02/18 00:19:45 marty Exp $";
#endif
    
#include <dce/rgynbase.h>

#include <dce/rplbase.h>
#include <rs_repadm.h>

#include <rs_base.h>
#include <rs_util.h>
#include <rs_audit_trail.h>
#include <rs_pvt.h>
#include <rs_state.h>
#include <rs_master.h>
#include <rs_mkey.h>
#include <rs_lock.h>
#include <rs_ch_lock.h>
#include <rs_rep_util.h>
#include <rs_rsdb.h>
#include <rs_rep_mgmt.h>
#include <rs_replist2.h>
#include <rs_log_replist.h>
#include <rs_prop_tasks.h>
#include <rs_master_change.h>
#include <rs_ver.h>
#include <rs_ver_pvt.h>

#include <dce/aclbase.h>
#include <rsdb_acl.h>
#include <rsdb.h>

#if defined(DCE_AUD_POINTS)
#include <dce/audit.h>
#include <sec_aud_evts.h>
#endif	/* defined(DCE_AUD_POINTS) */

#if defined(DCE_AUD_POINTS)
extern dce_aud_trail_t sec_aud_trail;
#endif	/* defined(DCE_AUD_POINTS) */


/*
 * Private routine declarations
 */

PRIVATE void rs_repadm_into_maintenance (
    error_status_t      *status
);

PRIVATE void rs_repadm_out_of_maintenance (
    error_status_t      *status
);


/*
 *  r s _ r e p _ a d m i n  _ s t o p 
 *
 *  Stop the replica identified by this handle.
 */
PUBLIC  void  rs_rep_admin_stop
  (
    handle_t            h,        /* [in] */
    error_status_t      *status
  )
{
    error_status_t  lst;
#if defined(DCE_AUD_POINTS)
    dce_aud_rec_t             ard;
    error_status_t            aud_status;
#endif	/* defined(DCE_AUD_POINTS) */

    CLEAR_STATUS(status);

    if (rs_state_exiting()) return;

#if defined(DCE_AUD_POINTS)
    dce_aud_start(REPADMIN_Stop, h, 0, aud_c_esl_cond_unknown, 
                  &ard, &aud_status);
#endif	/* defined(DCE_AUD_POINTS) */

    /* rs_shutdown checks the rs state
     */
    GLOBAL_RIW_LOCK {

        rs_audit_trail_log(h, 
            "Stop Server", (char *) NULL);

        rs_rep_util_check_caller(h, sec_acl_perm_Admin, status);

        if (GOOD_STATUS(status)) {

            rs_rep_mgmt_shutdown(true, status); /* true means checkpoint required */

            if (GOOD_STATUS(status)) {
                rpc_mgmt_stop_server_listening(NULL, &lst);
     
                rs_audit_trail_log(NULL, 
                    "Server stopped", (char *) NULL);
            }
        }

    } END_GLOBAL_RIW_LOCK;

#if defined(DCE_AUD_POINTS)
    if (ard) {    
        if (GOOD_STATUS(status)) {
            dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                           aud_c_esl_cond_success, &aud_status); 
        } else {
            if (rs_util_status_is_denial(*status)) {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                               aud_c_esl_cond_denial, &aud_status);
            } else {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                               aud_c_esl_cond_failure, &aud_status);
            } 
        }
    }
#endif	/* defined(DCE_AUD_POINTS) */

    /*
     * Release global lock so that pending tasks can run.  They should
     * notice that the rs state is closed and exit.
     */
}

/*
 *  r s _ r e p _ a d m i n _ m a i n t
 *
 *  Put replica in or out of maintenance mode.
 */
PUBLIC  void  rs_rep_admin_maint
  (
    handle_t            h,               /* [in] */
    boolean32           in_maintenance,  /* [in] */
    error_status_t      *status
  )
{
#if defined(DCE_AUD_POINTS)
    dce_aud_rec_t             ard;
    dce_aud_ev_info_t         ev_info;
    error_status_t            aud_status;
#endif	/* defined(DCE_AUD_POINTS) */

    CLEAR_STATUS(status);

    if (in_maintenance) {
        if (rs_state.rep_state == rs_c_state_in_maintenance)
            return;
    }
    else {
        if (rs_state.rep_state == rs_c_state_in_service)
            return;
    }

#if defined(DCE_AUD_POINTS)
    dce_aud_start(REPADMIN_Maint, h, 0, aud_c_esl_cond_unknown, 
                  &ard, &aud_status);
    if (ard) {
        ev_info.format = aud_c_evt_info_boolean;
        ev_info.data.boolean = in_maintenance;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);
    }
#endif	/* defined(DCE_AUD_POINTS) */

    if (in_maintenance) {
    
        BEGIN_GLOBAL_RIW_EVENT(rs_c_event_into_maintenance, status) {
    
            rs_audit_trail_log(h, 
                "Going into maintenance state", (char *) NULL);
    
            rs_rep_util_check_caller(h, sec_acl_perm_Admin, status);

            if (GOOD_STATUS(status)) {
                rs_repadm_into_maintenance(status);
            }

            if (GOOD_STATUS(status)) {
                rs_audit_trail_log(h, 
                    "Succeeded in going into maintenance state", 
                    (char *) NULL);
            }
    
        } END_GLOBAL_RIW_EVENT
    }
    else {

        BEGIN_GLOBAL_RIW_EVENT(rs_c_event_out_of_maintenance, status) {
    
            rs_audit_trail_log(h, 
                "Going out of maintenance state", (char *) NULL);
    
            rs_rep_util_check_caller(h, sec_acl_perm_Admin, status);

            if (GOOD_STATUS(status)) { 
                rs_repadm_out_of_maintenance(status);
            }
    
            if (GOOD_STATUS(status)) {
                rs_audit_trail_log(h, 
                    "Succeeded in going out of maintenance state", 
                     (char *) NULL);
            }
    
        } END_GLOBAL_RIW_EVENT
    }
#if defined(DCE_AUD_POINTS)
    if (ard) {    
        if (GOOD_STATUS(status)) {
            dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                           aud_c_esl_cond_success, &aud_status); 
        } else {  
            if (rs_util_status_is_denial(*status)) {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                               aud_c_esl_cond_denial, &aud_status);
            } else {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                               aud_c_esl_cond_failure, &aud_status);
            } 
        }
    }
#endif	/* defined(DCE_AUD_POINTS) */
}

/*
 *  r s _ r e p _ a d m i n _ m k e y
 *
 *  Change the master key and re-encrypt the database
 */
PUBLIC  void  rs_rep_admin_mkey
  (
    handle_t            h,        /* [in] */
    error_status_t      *status
  )
{
#if defined(DCE_AUD_POINTS)
    dce_aud_rec_t             ard;
    error_status_t            aud_status;

    dce_aud_start(REPADMIN_Mkey, h, 0, aud_c_esl_cond_unknown, 
                  &ard, &aud_status);
#endif	/* defined(DCE_AUD_POINTS) */

    BEGIN_GLOBAL_RIW_EVENT(rs_c_event_mkey_change, status) {

        rs_audit_trail_log(h, 
            "Changing the master key", (char *) NULL);

        rs_rep_util_check_caller(h, sec_acl_perm_Admin, status);
        if (GOOD_STATUS(status)) {
            /* will promote lock when database is
             * being re-encrypted under new key and
             * when it's being checkpointed
             */
            rs_mkey_change(status);
            if (GOOD_STATUS(status)) {
                rs_audit_trail_log(h,
                    "Database re-encrypted under new master key",
                    (char *) NULL);
            }
        }

    } END_GLOBAL_RIW_EVENT

#if defined(DCE_AUD_POINTS)
    if (ard) {    
        if (GOOD_STATUS(status)) {
            dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                           aud_c_esl_cond_success, &aud_status); 
        } else {  
            if (rs_util_status_is_denial(*status)) {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                               aud_c_esl_cond_denial, &aud_status);
            } else {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                               aud_c_esl_cond_failure, &aud_status);
            } 
        }
    }
#endif	/* defined(DCE_AUD_POINTS) */

}


/*
 *  r s _ r e p _ a d m i n _ i n f o
 *
 *  Get basic information about a replica such
 *  as its state, uuid, latest update sequence
 *  number and timestamp, and whether it is the master.
 *  Also get the replica's information about the master's 
 *  uuid and the sequence number when the master was 
 *  designated.
 */
PUBLIC  void  rs_rep_admin_info
  (
    handle_t            h,          /* [in] */
    rs_replica_info_t   *rep_info,  /* [out] */
    error_status_t      *status
  )
{
    BEGIN_READ_EVENT(rs_c_event_primitive_read, lock_replist, status) {

        rs_rep_mgmt_get_info(rep_info, status);

    } END_READ_EVENT;
}

/*
 *  r s _ r e p _ a d m i n _ i n f o _ f u l l
 *
 *  Get complete information about a replica such
 *  as its state, uuid, protocol towers, latest
 *  update sequence number and timestamp, and whether 
 *  it is the master.  
 *  Also get the replica's information about the master's 
 *  uuid, protocol towers, and the sequence number when 
 *  the master was designated.
 */
PUBLIC void rs_rep_admin_info_full 
    (
    handle_t                h,
    rs_replica_info_t       *rep_info,
    rs_replica_twr_vec_p_t  *rep_twrs,
    rs_replica_twr_vec_p_t  *master_twrs,
    error_status_t          *status
    )
{
    BEGIN_READ_EVENT(rs_c_event_primitive_read, lock_replist, status) {

        rs_rep_mgmt_get_info_full(rep_info, rep_twrs, master_twrs, status);

    } END_READ_EVENT;
}

/*
 *  r s _ r e p _ a d m i n _ d e s t r o y
 *
 *  a drastic operation which tells a replica
 *  to destroy its database and exit
 */
PUBLIC  void  rs_rep_admin_destroy
  (
    handle_t            h,        /* [in] */
    error_status_t      *status
  )
{
    error_status_t      lst;
#if defined(DCE_AUD_POINTS)
    dce_aud_rec_t             ard;
    error_status_t            aud_status;

    dce_aud_start(REPADMIN_Destroy, h, 0, aud_c_esl_cond_unknown, 
                  &ard, &aud_status);
#endif	/* defined(DCE_AUD_POINTS) */

    BEGIN_GLOBAL_RIW_EVENT(rs_c_event_delete, status) {

        rs_audit_trail_log(h, 
            "Destroying replica", (char *) NULL);

        rs_rep_util_check_caller(h, sec_acl_perm_delete, status);

        if (GOOD_STATUS(status)) {
            rs_rep_mgmt_delete(status);
        }

        if (GOOD_STATUS(status)) {
            rpc_mgmt_stop_server_listening(NULL, &lst);
     
            rs_audit_trail_log(h,
                "Replica destroyed", (char *) NULL);
        }

    } END_GLOBAL_RIW_EVENT

#if defined(DCE_AUD_POINTS)
    if (ard) {    
        if (GOOD_STATUS(status)) {
            dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                           aud_c_esl_cond_success, &aud_status); 
        } else {  
            if (rs_util_status_is_denial(*status)) {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                               aud_c_esl_cond_denial, &aud_status);
            } else {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                               aud_c_esl_cond_failure, &aud_status);
            } 
        }
    }
#endif	/* defined(DCE_AUD_POINTS) */
}

/*
 *  r s _ r e p _ a d m i n _ i n i t _ r e p l i c a
 *
 *  (re-)initialize the slave identified by "rep_id"
 *
 *  master-only operation.
 */
PUBLIC  void  rs_rep_admin_init_replica
  (
    handle_t            h,        /* [in] */
    uuid_p_t            rep_id,   /* [in] */
    error_status_t      *status
  )
{
    uuid_t              init_id;
    unsigned_char_p_t   rep_id_str;
    error_status_t      lst;
#if defined(DCE_AUD_POINTS)
    dce_aud_rec_t             ard;
    dce_aud_ev_info_t         ev_info;
    error_status_t            aud_status;
#endif	/* defined(DCE_AUD_POINTS) */

    uuid_create(&init_id, status);
    if (BAD_STATUS(status)) return;

    rep_id_str = NULL;
    uuid_to_string(rep_id, &rep_id_str, &lst);

#if defined(DCE_AUD_POINTS)
    dce_aud_start(REPADMIN_Init, h, 0, aud_c_esl_cond_unknown, 
                  &ard, &aud_status);
    if (ard) {
        ev_info.format = aud_c_evt_info_char_string;
        ev_info.data.char_string = (idl_char *) rep_id_str;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);
    }
#endif	/* defined(DCE_AUD_POINTS) */

    BEGIN_MASTER_RIW(lock_replist, status) {

        READ_LOCK(lock_db) {
            rs_audit_trail_log(h, "Attempt init replica", 
                (char *) rep_id_str);

            rs_rep_util_check_caller(h, sec_acl_perm_Admin, status);

            if (GOOD_STATUS(status)) {
                RIW_PROMOTE(lock_replist);
                rs_replist2_master_init_rep(rep_id, &init_id, status);
        
                if (GOOD_STATUS(status)) { 
                    rs_audit_trail_log(h, "Init replica succeeded", 
                                   (char *) rep_id_str);
                    rs_log_replist_init_replica(rep_id, &init_id);
                }
            }

        } END_READ_LOCK;

    } END_MASTER_RIW

#if defined(DCE_AUD_POINTS)
    if (ard) {    
        if (GOOD_STATUS(status)) {
            dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                           aud_c_esl_cond_success, &aud_status); 
        } else {  
            if (rs_util_status_is_denial(*status)) {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                               aud_c_esl_cond_denial, &aud_status);
            } else {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                               aud_c_esl_cond_failure, &aud_status);
            } 
        }
    }
#endif	/* defined(DCE_AUD_POINTS) */

    if (rep_id_str != NULL)
        rpc_string_free(&rep_id_str, &lst);
}

/*
 *  r s _ r e p _ a d m i n _ c h a n g e _ m a s t e r
 *
 *  change the master to "new_master_id".
 *  The master gracefully passes its
 *  replica list state and propq to
 *  the new master.
 *
 *  master-only operation.
 */
PUBLIC  void  rs_rep_admin_change_master
  (
    handle_t            h,              /* [in] */
    uuid_p_t            new_master_id,  /* [in] */
    error_status_t      *status
  )
{
    CLEAR_STATUS(status);

    rs_mc_change_master(h, new_master_id, status);
}

/* 
 *  r s _ r e p _ a d m i n _ b e c o m e _ m a s t e r
 *
 *  a drastic operation to make a slave
 *  become the master because the master
 *  has died.  
 *  Normally the rs_rep_admin_change_master 
 *  operation is used to designate a new 
 *  master; this operation can cause updates 
 *  to be lost.
 */
PUBLIC  void  rs_rep_admin_become_master
  (
    handle_t            h,        /* [in] */
    error_status_t      *status
  )
{
    CLEAR_STATUS(status);

    rs_mc_force_become_master(h, status);

}

/*
 *  r s _ r e p _ a d m i n _ b e c o m e _ s l a v e
 *
 *  a drastic operation to make a replica
 *  which thinks it's the master become a
 *  slave.
 */
PUBLIC  void  rs_rep_admin_become_slave
  (
    handle_t            h,        /* [in] */
    error_status_t      *status
  )
{

    CLEAR_STATUS(status);

    rs_mc_become_slave(h, status);
}

PRIVATE void rs_repadm_into_maintenance
  (
    error_status_t      *status
  )
{
    /* checkpt database,
     * which promotes RIW lock,
     * and close files except state 
     * file.
     */
    rsdb_shutdown(true, lock_mode_riw);

    rs_state_set_state(rs_c_state_in_maintenance, status);
}

PRIVATE void rs_repadm_out_of_maintenance
  (
    error_status_t      *status
  )
{
    /*
     * Make sure database area is properly protected
     */
    rsdb_tighten_directory_protection();
    rsdb_protect_files(true);

    GLOBAL_RIW_PROMOTE;

    rsdb_open_files();

    rs_state_set_state(rs_c_state_in_service, status);

    if (rs_state_master()) {
        rs_rep_mgmt_start_master(status);
        if (BAD_STATUS(status)) return;
    }
}

/*
 *  r s _ r e p _ a d m i n _ s e t _ s w _ r e v
 *
 *  Set the operating software revision for the cell.
 *
 *  master-only operation.
 *
 * Alogrithm
 *
 * Check incoming sw rev. Is it within the
 * range supported by the master?  Is it 
 * >= the current operating rev?
 *
 * Auth check on caller.
 *
 * Update master sw rev.
 *
 * Take steps necessary to bring replica up
 * to the new rev.
 *    - register interfaces with runtime
 *    - register interfaces with endpoint map
 *      (may not be necessary if we decide to register
 *       all endpoints at server boot. In that case, 
 *       the replica will still be incapable of supporting
 *       the operation until the interfaces are registered
 *       with the local RPC runtime).
 *    - Wake up namespace thread to fix up cell
 *      namespace registrations if necessary. 
 */
PUBLIC  void  rs_rep_admin_set_sw_rev
  (
    handle_t            h,        /* [in] */
    unsigned32          sw_rev,
    error_status_t      *status
  )
{
    boolean32                set;
#if defined(DCE_AUD_POINTS)
    dce_aud_rec_t             ard;
    dce_aud_ev_info_t         ev_info;
    error_status_t            aud_status;
#endif	/* defined(DCE_AUD_POINTS) */

#if defined(DCE_AUD_POINTS)
    dce_aud_start(REPADMIN_SetSwRev, h, 0, aud_c_esl_cond_unknown, 
                  &ard, &aud_status);
    if (ard) {
        ev_info.format = aud_c_evt_info_ulong_int;
        ev_info.data.ulong_int = sw_rev;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);
    }
#endif	/* defined(DCE_AUD_POINTS) */

    /*
     * We are going to update the database eventually, so we need
     * to take a global riw lock
     */

    BEGIN_GLOBAL_RIW_EVENT(rs_c_event_set_sw_rev, status) {
	rs_audit_trail_log(h, "Attempt set version", NULL );

	rs_rep_util_check_caller(h, sec_acl_perm_Admin, status);

	if (GOOD_STATUS(status)) {
	    
	    if (GOOD_STATUS(status)) {
		set = rs_rep_mgmt_master_set_sw_rev(sw_rev, h, status);
	    }
	    
	    if (GOOD_STATUS(status) && set) { 
		rs_audit_trail_log(h, "set version succeeded", NULL);
	    }
	}
    } END_GLOBAL_RIW_EVENT

#if defined(DCE_AUD_POINTS)
    if (ard) {    
        if (GOOD_STATUS(status)) {
            dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                           aud_c_esl_cond_success, &aud_status); 
        } else {  
            if (rs_util_status_is_denial(*status)) {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                               aud_c_esl_cond_denial, &aud_status);
            } else {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                               aud_c_esl_cond_failure, &aud_status);
            } 
        }
    }
#endif	/* defined(DCE_AUD_POINTS) */
}

/* 
 * See extensive comments in rs_repadm.idl ...
 *
 * Since we know that in reality our software versions
 * are monotonically increasing beginning at numeric version 1,
 * we can make a few simplfying assumptions that we forbid to our
 * clients. 
 */
void  rs_rep_admin_get_sw_vers_info(
    /* [in]  */   handle_t                    h,  
    /* [in]  */   unsigned32                  max_num_sw_vers,
    /* [out] */   unsigned32                 *num_sw_vers,             
    /* [out, size_is(max_num_sw_vers), length_is(*num_sw_vers)] */ 
                  rs_replica_sw_vers_info_t  sw_vers_info[],
    /* [out] */   unsigned32                 *min_sw_vers,  
    /* [out] */   unsigned32                 *max_sw_vers,
    /* [out] */   unsigned32                 *cur_sw_vers,
    /* [out] */   error_status_t             *status
)
{
    unsigned32 i, max, min;

    CLEAR_STATUS(status);
    *num_sw_vers = 0; 
    *min_sw_vers = rs_c_software_vers_unknown;

    if (max_num_sw_vers == 0) {
	/* Give back as good as we got, which isn't much ... */
        *max_sw_vers = rs_c_software_vers_unknown;
        *cur_sw_vers = rs_c_software_vers_unknown;
	return;
    }

    rs_master_info_get_sw_rev(cur_sw_vers, status);
    if (BAD_STATUS(status)) return;

    if ((rs_c_max_software_vers - rs_c_min_software_vers + 1) <= max_num_sw_vers) {

	/* 
	 * The caller's array will hold all our supported
         * software versions, so we're all set
         */
	max = rs_c_max_software_vers;
	min = rs_c_min_software_vers; 
	
    } else {

	/* 
	 * Our caller didn't provide a large enough array to
	 * hold all our software versions, so construct a window of
         * contiguous software versions that will fit in the caller's
         * buffer and that includes our current software version.  We
         * start with an upper bound equal to our max software
         * version and slide down as necessary until the window 
         * encompasses our current software version.
	 */
	max = rs_c_max_software_vers;
	do {

	    min = max - max_num_sw_vers + 1;
	    if (min <= *cur_sw_vers) {
		break;
	    }

	    max = max - 1;

	} while (max > 0);
    }
    
    if (max == 0) {
	/* 
	 * Shouldn't ever happen, since the current software
	 * version is always between the min and max software
         * versions, main and max are both non-zoer, 
	 * and we would never have entered the above loop
         * unless max_num_sw_vers was > 0 and < the number of
         * supported software revisions.
         */
        *min_sw_vers = rs_c_software_vers_unknown;
        *cur_sw_vers = rs_c_software_vers_unknown;
	SET_STATUS(status, sec_rgy_rep_pgmerr);
	return;
    }

    *num_sw_vers = max - min + 1;
    *max_sw_vers = rs_c_max_software_vers;
    *min_sw_vers = rs_c_min_software_vers; 

    /*
     * Fill in the caller's array.
     */
    for (i = 0; i < *num_sw_vers;  i++) {
	sw_vers_info[i].sw_vers = min + i;
        u_strcpy(&sw_vers_info[i].sw_vers_printstring[0],
		 rs_sw_vers_get_printstring(min + i));
    }
}    
