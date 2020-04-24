/* 
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_rep_mgmt.c,v $
 * Revision 1.1.12.2  1996/08/09  12:10:18  arvind
 * 	Merge setting secd1.2.2's change.
 * 	[1996/08/05  18:34 UTC  cuti  /main/DCE_1.2.2/cuti_pk_migration/3]
 *
 * 	Add error status for out of bound secd version
 * 	[1996/08/05  13:34 UTC  cuti  /main/DCE_1.2.2/cuti_pk_migration/2]
 *
 * 	Merge 1.2.2's version change with 1.1's
 * 	[1996/07/31  15:22 UTC  cuti  /main/DCE_1.2.2/cuti_pk_migration/1]
 *
 * 	Add pk schema when flipping to dce122
 * 	[1996/05/24  23:16 UTC  sommerfeld  /main/DCE_1.2.2/1]
 *
 * 	merge in global group work for DCE 1.2.2
 * 	[1996/03/27  17:47 UTC  hanfei  /main/DCE_1.2/2]
 *
 * 	1.2.2 migration fixes.
 * 	[1996/03/22  15:26 UTC  greg  /main/DCE_1.2/hanfei_dce1.2_b1/1]
 *
 * 	Update OSF copyright years
 * 	[1996/02/17  23:07:53  marty]
 *
 * 	Submit OSF/DCE 1.2.1
 * 	HP 	[1995/05/15  21:08 UTC  greg  /main/HPDCE02/4]
 *
 * 	CHFts14111 bug fix
 * 	HP 	[1995/03/23  18:41 UTC  cuti  /main/HPDCE02/cuti_mothra_bug/4]
 *
 * 	Change signature name of rs_rep_mgmt_shutdown.
 * 	HP 	[1995/03/14  18:53 UTC  cuti  /main/HPDCE02/cuti_mothra_bug/3]
 *
 * 	CHFts14111: Add boolean to rs_rep_mgmt_shutdown() and rsdb_shutdown()
 * 	HP 	[1995/03/07  22:26 UTC  cuti  /main/HPDCE02/2]
 *
 * 	CHFts14419 (OT12797): Demote write lock before rpc operation and promote it
 * 	to preserve the semantics after rpc operation.
 * 	HP 	[1995/03/03  22:12 UTC  cuti  /main/HPDCE02/cuti_mothra_bug/2]
 *
 * 	changed Make it to be as mininum change as possible.
 * 	HP 	[1995/03/03  21:52 UTC  cuti  /main/HPDCE02/cuti_mothra_bug/1]
 *
 * 	CHFts14419 (OT12797).
 * 	HP 	[1995/02/03  15:53 UTC  emartin  /main/HPDCE02/1]
 *
 * 	merge in some replication fixes
 * 	HP 	[1995/01/27  22:34 UTC  emartin  /main/emartin_mothra2/1]
 *
 * 	chk_i_am_slv - call rs_prop_tasks_slave_alive for all slave prop fixing
 * 	[1995/12/08  17:29:09  root]
 *
 * Revision 1.1.12.1  1996/05/10  13:22:58  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 * 
 * 	HP revision /main/DCE_1.2/2  1996/03/27  17:47 UTC  hanfei
 * 	merge in global group work for DCE 1.2.2
 * 
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/1  1996/03/22  15:26 UTC  greg
 * 	1.2.2 migration fixes.
 * 
 * 	HP revision /main/DCE_1.2/1  1996/01/03  19:18 UTC  psn
 * 	Fix botched merge,
 * 	[1995/04/24  17:36 UTC  greg  /main/HPDCE02/SEC_migrate_merge/2]
 * 
 * 	CHFts14111 bug fix
 * 	[1995/03/27  19:33 UTC  cuti  /main/HPDCE02/3]
 * 
 * 	Change signature name of rs_rep_mgmt_shutdown.
 * 	[1995/03/23  18:41 UTC  cuti  /main/HPDCE02/cuti_mothra_bug/4]
 * 
 * 	CHFts14111: Add boolean to rs_rep_mgmt_shutdown() and rsdb_shutdown()
 * 	[1995/03/14  18:53 UTC  cuti  /main/HPDCE02/cuti_mothra_bug/3]
 * 
 * 	CHFts14419 (OT12797): Demote write lock before rpc operation and promote it
 * 	to preserve the semantics after rpc operation.
 * 	[1995/03/07  22:26 UTC  cuti  /main/HPDCE02/2]
 * 
 * 	changed Make it to be as mininum change as possible.
 * 	[1995/03/03  22:12 UTC  cuti  /main/HPDCE02/cuti_mothra_bug/2]
 * 
 * 	CHFts14419 (OT12797).
 * 	[1995/03/03  21:52 UTC  cuti  /main/HPDCE02/cuti_mothra_bug/1]
 * 
 * 	merge in some replication fixes
 * 	[1995/02/03  15:53 UTC  emartin  /main/HPDCE02/1]
 * 
 * 	chk_i_am_slv - call rs_prop_tasks_slave_alive for all slave prop fixing
 * 	[1995/01/27  22:34 UTC  emartin  /main/emartin_mothra2/1]
 * 
 * Revision 1.1.6.4  1994/08/25  20:47:40  sekhar
 * 	Put back SVC messages for fatal_st() and error_st() calls.
 * 	[1994/08/25  20:30:50  sekhar]
 * 
 * Revision 1.1.6.3  1994/08/04  16:13:55  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:12:14  mdf]
 * 
 * Revision 1.1.6.1  1994/06/02  21:23:18  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:19:49  mdf]
 * 
 * 	hp_sec_to_osf_3 drop, removed __STDC__ stuff.
 * 	[1994/05/19  20:00:02  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 	[1994/05/18  11:56:53  mdf]
 * 
 * 	HP revision /main/KK/ODESSA_2/1  1994/05/03  18:54 UTC  hanfei
 * 	 merge
 * 
 * 	HP revision /main/KK/hanfei_ch_master/1  1994/04/20  15:29 UTC  hanfei
 * 	merge
 * 
 * 	HP revision /main/HPDCE01/emartin_ch_master/3  1994/04/15  18:47 UTC  emartin
 * 	support master_seqno flag
 * 
 * 	HP revision /main/HPDCE01/emartin_ch_master/2  1994/04/12  16:06 UTC  emartin
 * 	remove master_inited flag
 * 
 * 	HP revision /main/HPDCE01/emartin_ch_master/1  1994/04/12  14:35 UTC  emartin
 * 	cleanup change master - move many routines to rs_master_change.c
 * 
 * 	HP revision /main/HPDCE01/1  1994/01/26  20:04  mdf
 * 	merge
 * 
 * Revision 1.1.4.7  1993/12/28  21:17:21  hanfei
 * 	rs_rep_mgmt_become_master - checkpt after mark reps for init,
 * 	so delrep won't be replayed.
 * 	chk_bind_to_new_master - some fixes
 * 	[1993/12/21  20:58:14  hanfei]
 * 
 * Revision 1.1.4.6  1993/12/20  22:31:28  hanfei
 * 	restore gamera version of rs_rep_mgmt_start_master,
 * 	kk version of this routien renamed to
 * 	rs_rep_mgmt_start_new_master
 * 	[1993/12/20  22:21:31  hanfei]
 * 
 * Revision 1.1.4.5  1993/12/17  22:24:07  hanfei
 * 	add checks if master is obsolete.
 * 	[1993/12/17  22:04:13  hanfei]
 * 
 * Revision 1.1.4.4  1993/12/16  19:11:41  hanfei
 * 	add some comments
 * 	rework for become -master/-slave
 * 	[1993/12/16  18:17:14  hanfei]
 * 
 * 	init_slave and master_to_slave - call rs_replist2_purge_deleted_reps
 * 	[1993/12/13  21:49:56  hanfei]
 * 
 * 	rework for change-master
 * 	[1993/12/11  18:49:32  hanfei]
 * 
 * 	lots of changes for change_master
 * 	[1993/12/10  13:55:23  hanfei]
 * 
 * 	rs_rep_mgmt_mark_reps_for_init - dont_init_deleted_rep flag
 * 	[1993/12/08  04:45:26  hanfei]
 * 
 * Revision 1.1.4.3  1993/11/04  22:10:58  hanfei
 * 	fixed a bug introduced by rsdb_replica_lookup zeros out first
 * 	argument.
 * 	[1993/11/04  21:54:34  hanfei]
 * 
 * Revision 1.1.4.2  1993/10/29  17:28:01  hanfei
 * 	change master work : add codes and functions for server's side's
 * 	work of change master, force become master/slave.
 * 	[1993/10/29  17:26:41  hanfei]
 * 
 * 	Second replication drop: bl5
 * 	Do rs_rep_mgmt_shutdown, rs_rep_mgmt_delete
 * 	[1992/10/06  17:25:00  burati]
 * 
 * Revision 1.1.2.3  1993/09/15  15:50:56  root
 * 	    Loading drop DCE1_0_3b03
 * 
 * Revision 1.1.2.9  1993/04/21  19:10:33  emartin
 * 	CR 7751.  rs_rep_mgmt_i_am_slave task - sleep for 20 seconds
 * 	before first try to contact master.
 * 	[1993/04/21  17:35:22  emartin]
 * 
 * Revision 1.1.2.8  1993/03/25  16:18:51  emartin
 * 	CR 7521.  rs_rep_mgmt_master_chk_i_am_slv - call
 * 	rs_prop_tasks_refrsh_slv_creds to mark auth handle
 * 	and binding handle as invalid so they will be
 * 	refreshed when the next propagation to the replica
 * 	occurs.
 * 	[1993/03/24  20:40:16  emartin]
 * 
 * Revision 1.1.2.7  1993/02/26  23:54:18  emartin
 * 	em_bl6f: remove debug printf
 * 	[1993/02/24  22:26:13  emartin]
 * 
 * 	em_bl6f: rs_rep_mgmt_init - support restore_master
 * 	[1993/02/22  23:05:16  emartin]
 * 
 * 	em_bl6f: do rs_rep_mgmt_master_ck_sw_compat
 * 	 do rs_rep_mgmt_stop_til_sw_ok_task,
 * 	    rs_rep_mgmt_stop_til_sw_ok
 * 	[1993/02/19  15:53:04  emartin]
 * 
 * 	em_bl6f: rs_c_state_compat_software_version =>
 * 	 rs_c_state_compat_software_vers
 * 	 remove rs_rep_mgmt_slave_incompat_sw
 * 	 do rs_rep_mgmt_slave_stop_til_sw_ok
 * 	 rs_rep_mgmt_get_info - fill info sw_rev field
 * 	[1993/02/11  17:01:32  emartin]
 * 
 * Revision 1.1.2.6  1992/12/29  16:36:50  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:26:02  zeliff]
 * 
 * Revision 1.1.2.5  1992/12/18  22:12:50  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:53:33  sekhar]
 * 
 * Revision 1.1.4.2  1992/12/11  22:24:43  emartin
 * 	bl6c: slave update chks, slave mgmt (i am slave task)
 * 
 * Revision 1.1.2.4  1992/11/04  19:38:56  ahop
 * 	Third replication drop: bl6a
 * 	Do rs_rep_mgmt_start/init_master
 * 	[1992/11/03  18:45:54  ahop]
 * 
 * Revision 1.1.2.3  1992/10/07  20:36:21  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:18:49  sekhar]
 * 
 * Revision 1.1.2.2  1992/08/31  18:25:56  sekhar
 * 	 Call rsdb_replist_lookup with cursor, not rs_state.rep_id
 * 	[1992/08/31  17:54:35  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:44:28  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
 * Copyright (c) Hewlett-Packard Company 1991, 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 *      Registry Server - replica management mainly
 *          for startup, shutdown, and delete of slave 
 *          and master
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = 
    "$Id: rs_rep_mgmt.c,v 1.1.12.2 1996/08/09 12:10:18 arvind Exp $";
#endif

#include <stdio.h>

#include <dce/dce.h>
#include <dce/nbase.h>
#include <dce/dce_cf.h>
#include <dce/rplbase.h>
#include <dce/sec_daclmgr.h>

#include <rs_base.h>
#include <rs_ver.h>
#include <rs_ver_pvt.h>

#include <rs_pvt.h>
#include <rs_state.h>
#include <rs_master.h>
#include <rs_util.h>
#include <rs_rep_pvt.h>

#include <rs_replist2.h>
#include <rs_master_replist.h>
#include <rs_rep_util.h>
#include <rs_rep_mgmt.h>
#include <rs_rep_init.h>
#include <rs.h>
#include <rs_ns.h>
#include <rs_lock.h>
#include <rs_ch_lock.h>
#include <rs_rsdb.h>
#include <rs_mkey.h>
#include <rs_log.h>
#include <rs_log_replist.h>
#include <rs_prop_tasks.h>
#include <rs_rep_bind.h>
#include <rs_task.h>
#include <rs_log_sw_version.h>

#include <rsdb.h>
#include <rsdb_replica.h>
#include <rsdb_policy.h>

#include <dce/aclbase.h>
#include <rsdb_acl.h>

#include <rrs_repmgr.h>
#include <sec_svc.h>

/* Local variables
 */
PRIVATE rs_replica_twr_vec_p_t  rep_towers;

PRIVATE unsigned32              global_compat_sw_rev;

/*
 * update seqno maintained during propq copy session
 * to insure that we don't apply duplicate updates
 */
PRIVATE boolean32           propq_last_upd_seqno_inited = false;
PRIVATE rs_update_seqno_t   propq_last_upd_seqno;


/* Prototypes for local routines
 */
PRIVATE void rs_rep_mgmt_init_master (
    rs_replica_mgt_item_p_t rp,
    boolean32               restore_masterf,
    rs_update_seqno_t       *master_seqno,
    error_status_t          *st
);

PRIVATE void rs_rep_mgmt_init_slave (
    rs_replica_mgt_item_p_t rp,
    error_status_t          *st
);

PRIVATE boolean32 i_am_slave_retry_status (
    error_status_t          *st,
    rs_rep_binding_t        *rep_binding
);

PRIVATE void rs_rep_mgmt_slave_chk_sw_compat (
    unsigned32      master_compat_sw_rev,
    error_status_t  *st
);

PRIVATE void *stop_til_sw_ok_task (
    void
);

static void get_admin_id (
    rpc_binding_handle_t  auth_rpc_handle,  /* [in] */
    sec_id_foreign_t      *client_id,       /* [out] */
    error_status_t        *st_p             /* [out] */
);

static void log_sw_version_change (
    unsigned32       old_sw_rev,
    unsigned32       new_sw_rev,
    error_status_t   *st_p
);

PUBLIC void rs_rep_mgmt_init
  (
    rs_replica_twr_vec_p_t  rep_twrs,
    boolean32               restore_masterf,
    rs_update_seqno_t       *master_seqno,
    error_status_t          *st
  )
{
    uuid_t                  cursor;
    rs_replica_mgt_item_p_t rp;
    error_status_t          lst;
    
    CLEAR_STATUS(st);

    rep_towers = rep_twrs;

    rp = NULL;

    /*
     * check replica's state and startup flags
     */

    if (restore_masterf) {
        if ((!rs_state_master()) ||
            (rs_state.rep_state != rs_c_state_in_service)) {
            SET_STATUS(st, sec_rgy_rep_bad_arg);
            dce_svc_printf(SEC_REP_RM_NOT_IN_SERVICE_MSG);
            dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                           sec_s_rs_rep, svc_c_sev_error, sec_rgy_rep_bad_arg);
            return;
        }
    }

    if (master_seqno != NULL) {
        if ((!rs_state_master()) ||
            (rs_state.rep_state != rs_c_state_dup_master)) {
            SET_STATUS(st, sec_rgy_rep_bad_arg);
            dce_svc_printf(SEC_REP_MSEQ_NOT_DUP_MASTER_MSG);
            dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                           sec_s_rs_rep, svc_c_sev_error, sec_rgy_rep_bad_arg);
            return;
        }
    }

    switch(rs_state.rep_state) {
    case rs_c_state_uninitialized:
        break;
        
    case rs_c_state_in_service:
        break;

    case rs_c_state_in_maintenance:
        if (!rs_state_master()) {
            dce_svc_printf(SEC_REP_MAINT_NOT_MASTER_MSG);
            SET_STATUS(st, sec_rgy_rep_not_master);
        }

        break;

    case rs_c_state_dup_master:
        if (!rs_state_master()) {
            dce_svc_printf(SEC_REP_DUPE_NOT_MASTER_MSG);
            SET_STATUS(st, sec_s_pgmerr);
        }
        else 
        if (master_seqno == NULL) {
            dce_svc_printf(SEC_REP_DUPE_CANT_START_MSG);
            SET_STATUS(st, sec_rgy_rep_master_dup);
        }

        break;

    default:
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_rep,
                       svc_c_sev_error,
                       sec_rgy_rep_bad_state);
        SET_STATUS(st, sec_rgy_rep_bad_state);
    }
    if (BAD_STATUS(st)) return;

    /*
     * get replist info about this replica
     */
    cursor = rs_state.rep_id;
    rsdb_replica_lookup(&cursor, &rp, st);
    if (BAD_STATUS(st)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_rep,
                       svc_c_sev_error,
                       *st);
        dce_svc_printf(SEC_REP_NOT_ON_REPLIST_MSG);
        return;
    }
        
    if (rs_state_master()) {
        rs_rep_mgmt_init_master(rp, 
            restore_masterf, master_seqno, st);
    }
    else {
        rs_rep_mgmt_init_slave(rp, st);
    }

    if (rp != NULL)
        rsdb_replica_item_free(&rp, &lst);

    if (BAD_STATUS(st)) return;
}

/*
 * Init master replist and start prop tasks
 * after master has been running for a while.
 * Invoked when master comes out of maintenance.
 */
PUBLIC void rs_rep_mgmt_start_master
  (
    error_status_t          *st
  )
{ 
    uuid_t                  cursor;
    rs_replica_mgt_item_p_t rp;
    error_status_t          lst;
    
    CLEAR_STATUS(st);

    cursor = rs_state.rep_id;

    rsdb_replica_lookup(&cursor, &rp, st);
    if (BAD_STATUS(st)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_rep,
                       svc_c_sev_error,
                       *st);
        dce_svc_printf(SEC_REP_NOT_ON_REPLIST_MSG);
        return;
    }
        
    rs_rep_mgmt_init_master(rp, false, NULL, st);

    rsdb_replica_item_free(&rp, &lst);

    if (BAD_STATUS(st)) return;
}

/*
*/
PRIVATE void rs_rep_mgmt_init_master
  (
    rs_replica_mgt_item_p_t rp,
    boolean32               restore_masterf,
    rs_update_seqno_t       *master_seqno,
    error_status_t          *st
  )
{ 
    rs_replica_twr_vec_p_t  current_rep_twrs;

/* comment out
    chk_another_master(st);
    if (BAD_STATUS(st)) return;
*/

    /*
     * If master_seqno isn't NULL, some "disaster recovery"
     * is being done to get this master's master_seqno to
     * be acceptable within the system.  
     *
     * The master with the highest master_seqno is considered 
     * the master. Slaves will reject updates from a master 
     * whose master_seqno is less than their cuurent master's
     * master_seqno; the error returned to the "obsolete"
     * master causes it to error exit.
     *
     * The calling routine has checked that the replica is the
     * master and is in state rs_c_state_dup_master.
     */
    if (master_seqno != NULL) {
        /*
         * save new master_seqno
         */
        rs_master_info_set(&rp->rep_id, master_seqno, st);
        if (BAD_STATUS(st)) return;

        rs_state_set_state(rs_c_state_in_service, st);
        if (BAD_STATUS(st)) return;

        rs_log_replist_master(
            &rp->rep_id, master_seqno, NULL);
    }

    /*
     * If the master's towers on the replist are
     * different from its current towers, 
     * update the replist and propagate the change
     */

    /* 
     * Get local pointer to master's current 
     * tower vector 
     */
    current_rep_twrs = rep_towers;  

    if (! rs_rep_util_twrs_equal(rp->rep_twrs, current_rep_twrs)) {
        rs_replist2_replace_rep(
            &rp->rep_id, rp->rep_name, current_rep_twrs, st);
        if (BAD_STATUS(st)) return;

        rs_log_replist_replace_replica(
            &rp->rep_id, rp->rep_name, current_rep_twrs, NULL);
    }

    /*
     * If restore_master flag is set or the master_seqno is
     * being changed, mark all replicas, except replicas marked 
     * for delete, for initialization
     */
    if (restore_masterf ||
        (master_seqno != NULL)) {
        rs_rep_mgmt_mark_reps_for_init(true, st);
        if (BAD_STATUS(st)) return;
    }

    rs_m_replist_init(st);
    if (BAD_STATUS(st)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_rep, svc_c_sev_error, *st);
        dce_svc_printf(SEC_REP_MSREPL_NOT_INITED_MSG);
        return;
    }

    rs_prop_tasks_start(st);
    if (BAD_STATUS(st)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_rep, svc_c_sev_error, *st);
        dce_svc_printf(SEC_REP_CANT_START_PROP_TASKS_MSG);
        return;
    }
}

/*
 * Invoked when a slave becomes the master.
 * All replicas are marked for init when a
 * force become master is done.
 */
PUBLIC void rs_rep_mgmt_start_new_master
  (
    boolean32                   force_become_master,
    error_status_t              *st
  )
{
    error_status_t              lst;

    CLEAR_STATUS(st);

    if (force_become_master) {
        /* mark all reps for init
         */
        rs_rep_mgmt_mark_reps_for_init(false, st);
        if (BAD_STATUS(st)) return;
    }

    rs_m_replist_init(st);
    if (BAD_STATUS(st)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_rep, svc_c_sev_error, *st);
	return;
    }

    rs_prop_tasks_start(st);
    if (BAD_STATUS(st)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_rep, svc_c_sev_error, *st);
	return;
    }
}

/*
 * Restoring master from an indeterminate state.  Mark 
 * all replicas to be initialized so that they'll be
 * consistent with the master.
 */
PUBLIC void rs_rep_mgmt_mark_reps_for_init 
  (
    unsigned32              dont_init_deleted_reps,
    error_status_t          *st
  )
{
    uuid_t                      cursor,
                                init_id;
    rs_replica_mgt_item_p_t     rp;
    rs_replica_packed_buff_t    *rep_buff;
    unsigned32                  rep_buff_len;
    error_status_t              lst;

    uuid_create_nil(&cursor, st);
    if (BAD_STATUS(st)) return;

    uuid_create(&init_id, st);
    if (BAD_STATUS(st)) return;
                                
    while (true) { 
        rsdb_replica_lookup_by_ref(&cursor, &rp,
            &rep_buff, &rep_buff_len, st);
        if (BAD_STATUS(st)) return;
                       
        if (dont_init_deleted_reps &&
            (rp->prop_type == rs_c_replica_prop_delete)) {
            /*
             * don't init deleted replica
             */
            ;  
        }
        else {
            /*
             * mark replica for init
             */
            rsdb_replica_set_prop_info(&rp->rep_id, rs_c_replica_prop_init,
                rs_c_replica_prop_unknown, &init_id, st);
            if (BAD_STATUS(st)) return;

            if (GOOD_STATUS(st)) {
            	rs_log_replist_init_replica(&rp->rep_id, &init_id);
	    }
        }

        /*
         * check for end of list
         */
        if (uuid_is_nil(&cursor, &lst)) 
            break;
    }
}

/*
 * Start task to send i_am_slave message to
 * the master
 * If replica's towers have changed make
 * a possibly temp change to the replist
 * (ie don't log the change, it can be 
 * redone when the slave comes up).
 * The change will be applied for real 
 * when the master propagates this change
 * to the slave.
*/
PRIVATE void rs_rep_mgmt_init_slave
  (
    rs_replica_mgt_item_p_t rp,
    error_status_t          *st
  )
{
    rs_replica_twr_vec_p_t  current_rep_twrs;
    error_status_t          lst;
    rs_task_handle_t        task_h;

    /* 
     * Get local pointer to slave's current 
     * tower vector 
     */
    current_rep_twrs = rep_towers;  

    if (! rs_rep_util_twrs_equal(rp->rep_twrs, current_rep_twrs)) {
        /* add/replace a replica to a slave's
         * copy of the replist
         */
        rs_replist2_slave_add_rep(
            &rp->rep_id, rp->rep_name, current_rep_twrs, st);
        if (BAD_STATUS(st)) return;
    }

    /*
     * delete replicas whose prop_type is deleted
     * from the replist.  This could happen if
     * this slave was once the master.
     */
    rs_replist2_purge_deleted_reps(&lst);

    if (! rs_task_create(&task_h,
                          (rs_task_func_ptr_t) rs_rep_mgmt_i_am_slave,
                          "SECD i_am_slave task", false, NULL) ) {
        SET_STATUS(st, sec_s_pgmerr);
        return;
    }

    CLEAR_STATUS(st);
}

/*
 * Tell the master that a slave process has just 
 * restarted.  
 * The slave sends its tower set to the master.
 * If the slave's tower set has changed, the master will
 * update its copy of the replica list and propagate the
 * update to the other replicas.
 */
PUBLIC void rs_rep_mgmt_i_am_slave (
    void
)
{
#define rs_c_i_am_slave_max_sleep_mins    20

    rs_rep_binding_t        rep_binding;
    uuid_t                  cursor;
    rs_replica_mgt_item_p_t rp;
    unsigned32              sleep_mins;
    unsigned32              sw_rev;
    error_status_t          lst;

    BEGIN_READ_EVENT(rs_c_event_primitive_read, lock_replist, &lst) {

        rs_rep_bind_init_to_master(&rep_binding, &lst);

        if (GOOD_STATUS(&lst)) {
	    rs_master_info_get_sw_rev(&sw_rev, &lst);
	}

        if (GOOD_STATUS(&lst)) {
            cursor = rs_state.rep_id;
            rsdb_replica_lookup(&cursor, &rp, &lst);
        }

    } END_READ_EVENT;

    if (BAD_STATUS(&lst)) return;

    /* Let things settle down for 20 seconds.
     * Allow time for host context to be setup
     */                               
    rs_task_sleep(20);

    sleep_mins = 1;

    while ((! rs_state_exiting()) &&
           (! rs_state_master()) ) {

        if (!rep_binding.valid_handle) {
            rs_rep_bind_auth_to_master(&rep_binding, &lst);
        }

        if (GOOD_STATUS(&lst)) {
            rrs_rep_mgr_i_am_slave(rep_binding.rep_handle,
                &rp->rep_id, sw_rev, rp->rep_twrs, &lst);
        }

        if (! i_am_slave_retry_status(&lst, &rep_binding))
            break;

        if ((sleep_mins * 2) > rs_c_i_am_slave_max_sleep_mins)
            sleep_mins = rs_c_i_am_slave_max_sleep_mins;
        else
            sleep_mins = sleep_mins * 2;

        rs_task_sleep(sleep_mins*60);
    }

    rsdb_replica_item_free(&rp, &lst);
}

/*
 * Analyse the status returned from an i_am_slave call.
 * Return true if the call should be retried, return
 * false otherwise.
 */
PRIVATE boolean32 i_am_slave_retry_status
  (
    error_status_t          *st,
    rs_rep_binding_t        *rep_binding
  )
{
    boolean32   retval;

    switch (*st) {
        case error_status_ok:
            retval = false;
            break;

        case sec_rgy_rep_slave_bad_sw_vers:
            /* wait for master to tell slave correct
             * sw version
             */
            retval = false;
            break;

        case sec_rgy_rep_not_master:
            /* wait for i_am_master from new master which
             * will restart the i_am_slave task
             */
            retval = false;
            break;

        default: 
            /* rebind and try again
             */
            rep_binding->valid_handle = false;
            retval = true;
            break;
    }

    return retval;
}

/*
 * Invoked by the master when it receives an i_am_slave
 * message from a replica.
 * Check master's and slave's compat software version.
 * If the slave's tower set has changed, replace the
 * slave's info on the replist and log the change.
 *
 * Caller should have replist RIW lock.
 */
PUBLIC void rs_rep_mgmt_master_chk_i_am_slv
  (
    uuid_p_t                rep_id,
    unsigned32              compat_sw_rev,
    rs_replica_twr_vec_p_t  rep_twrs,
    error_status_t          *st     
  )
{
    uuid_t                      cursor;
    rs_replica_mgt_item_p_t     rp;
    error_status_t              lst;

    /*
     * This update should be from a slave
     */
    if (uuid_equal(&rs_state.rep_id, rep_id, &lst)) {
        SET_STATUS(st, sec_rgy_rep_master);
        return;
    }

    /*
     * Check master's and slave's compat sw versions.
     * Will crash if master's sw version is bad.
     */
    rs_rep_mgmt_master_ck_sw_compat(compat_sw_rev, st);
    if (BAD_STATUS(st)) return;

    /*
     * If the slave's tower set has changed, replace
     * the slave's info on the replist and log the
     * change.
     */
    cursor = *rep_id;
    rsdb_replica_lookup(&cursor, &rp, st);
    if (BAD_STATUS(st)) return;

    RIW_PROMOTE(lock_replist);

    /*
     * Mark slave's bindings as invalid so
     * they'll be refreshed next time the
     * master tries to propagate to the slave.
     * Mark this replica as communicado.
     */
    rs_prop_tasks_slave_alive(rp->prop_mgt);
     
    if (! rs_rep_util_twrs_equal(rp->rep_twrs, rep_twrs)) {

        rs_replist2_master_replace_rep(
            &rp->rep_id, rp->rep_name, rep_twrs, st);

        if (GOOD_STATUS(st)) {
            rs_log_replist_replace_replica(
                &rp->rep_id, rp->rep_name, rep_twrs, NULL);
        }
    } else {
	/* 
	 * If the slave's twrs changed, then the if portion
         * above will trigger a prop task wakeup.  If not, we need 
	 * to explicitly trigger a prop task wakeup to ensure that 
	 * software version negotiation occurs immediately, hence 
	 * this else clause.
	 */
	rs_prop_tasks_wakeup();
    }

    /* 
     * Inform the prop driver that an i_am_slave has arrived
     * so it can avoid any unnecessary sw rev negotiations with the
     * slave.
     */
    if (GOOD_STATUS(st)) {
	rs_prop_tasks_handle_i_am_slave(rp->prop_mgt, lock_mode_write, 
					compat_sw_rev, st);
    }

    rsdb_replica_item_free(&rp, &lst);
}

/*
 * Master's check that the slave's software
 * version is ok and that its software version is 
 * ok.  Master will crash if its sw version is
 * bad.
 */
PUBLIC void rs_rep_mgmt_master_ck_sw_compat
  (
    unsigned32              slave_compat_sw_rev,
    error_status_t          *st     
  )
{
    unsigned32  global_sw_rev;

    rs_master_info_get_sw_rev(&global_sw_rev, st);

    if (slave_compat_sw_rev == global_sw_rev) {
        CLEAR_STATUS(st);
    } else if (slave_compat_sw_rev > global_sw_rev) {
	/*
	 * Slave is operating at a higher version than the
	 * current operating version as undertsood by the master.
	 * That probably means that we've restored the master
	 * from an old backup, and that there are newer replicas
	 * around.  But why would anyone restore the master
	 * if there are newer functioning replicas available?
	 * If they really mean to "revert" to the backed up
	 * state of an old master, the new replicas are probably 
         * meaningless anyway, so they should be shut down before 
	 * restoring the master.  But we're not going to make that 
         * decision for the administrator.  Instead we shut down the 
         * master and let them examine the log to fgure out what's 
	 * going on.  If they really mean it, they're going to have
         * to shut down the replicas manually before they restore the 
	 * master.
	 */
        SET_STATUS(st, sec_rgy_rep_master_bad_sw_vers);
        rs_rep_mgmt_stop_til_sw_ok_task();
    } else {
	/*
	 * We know the slave is operating at a lower version than
	 * the current operating version as understood by the master.
	 * But we don't know whether or not the slave is capable
	 * of operating at the correct current operating revision
	 * if instructed to do so by the master.  We return the
	 * sec_rgy_rep_slave_bad_sw_vers status and let the caller
	 * decide what to do about it.
	 */
	SET_STATUS(st, sec_rgy_rep_slave_bad_sw_vers);
    }
}

/* 
 * Routines to check that an update which a slave has
 * just received is ok.
 * Before these routines are invoked, the slave's state
 * is checked to make sure it is ok for the
 * requested operation (see rs_state_check) and 
 * appropriate locks are taken.
 */

/*
 * Slave's check that the update it has just received
 * is from the master and is in correct order.
 * The slave also checks that its software version
 * matches the master's; the slave invokes a task
 * to kill itself if its sw rev is incorrect
 */
PUBLIC void rs_rep_mgmt_slave_chk_update
  (
    rs_replica_master_info_t    *master_info,
    boolean32                   *apply_update,
    error_status_t              *st
  )
{
    rs_update_seqno_t   last_update_seqno;
    sec_timeval_t       last_update_ts;

    /*
     * if any problems, don't apply the update
     */
    *apply_update = false;

    if (rs_state.rep_state == rs_c_state_initializing) {
        /*
         * when initing update must come from the surrogate
         * master
         */
        if (!rs_master_info_crnt_srgt_master(
                &master_info->master_id, &master_info->master_seqno, st)) {
            SET_STATUS(st, sec_rgy_rep_bad_state);
            return;
        }

        /*
         * when initing, special update seqno management/checking 
         * is done in rs_rep_init.c module
         */
        rs_rep_init_chk_update_seqno(master_info, apply_update, st);
        if (BAD_STATUS(st)) return;

	rs_rep_init_chk_master_sw_rev(master_info->master_compat_sw_rev, st);
        if (BAD_STATUS(st)) return;

    }
    else {
        rs_master_info_check_master(
            &master_info->master_id, &master_info->master_seqno, st);
        if (BAD_STATUS(st)) return;

        rs_rep_mgmt_slave_chk_sw_compat(
            master_info->master_compat_sw_rev, st);
        if (BAD_STATUS(st)) return;

        rs_log_get_update_info(&last_update_seqno, &last_update_ts, st);
        if (BAD_STATUS(st)) return;

        rs_rep_mgmt_slave_chk_upd_seqno(master_info, 
            &last_update_seqno, apply_update, st);
        if (BAD_STATUS(st)) return;
    }
}


/*
 * Slave's check that the update is from the
 * master and that its software version is
 * compatible with the master's.
 */
PUBLIC void rs_rep_mgmt_slave_chk_master
  (
    rs_replica_master_info_t    *master_info,
    error_status_t              *st
  )
{
    rs_master_info_check_master(
        &master_info->master_id, &master_info->master_seqno, st);
    if (BAD_STATUS(st)) return;

    rs_rep_mgmt_slave_chk_sw_compat(master_info->master_compat_sw_rev, st);
    if (BAD_STATUS(st)) return;
}

/* 
 * Compare the master's idea of the slave's last update seqno
 * (in master_info->previous_update_seqno) with the slave's 
 * last_update_seqno.  If they match, return good status and
 * apply the update.
 *
 * If the master's idea of the slave's last update seqno is ahead 
 * of the slave's, the slave probably lost some updates (perhaps 
 * because it was restored from backup); return seqno_high error.
 *
 * If the slave's last update is ahead of what the master thinks 
 * it is, check whether the seqno of the update from the master
 * matches the slave's last update seqno (perhaps the slave's
 * ack for this update was lost); if they match return good status 
 * but don't apply the update.  If not, return seqno_low error.
 */
PUBLIC void rs_rep_mgmt_slave_chk_upd_seqno
  (
    rs_replica_master_info_t    *master_info,
    rs_update_seqno_t           *last_upd_seqno,
    boolean32                   *apply_update,
    error_status_t              *st
  )
{
    signed32            seqno_cmp;

    CLEAR_STATUS(st);

    seqno_cmp = rs_util_seqno_cmp(&master_info->previous_update_seqno,
                        last_upd_seqno);

    if (seqno_cmp == 0) {
        *apply_update = true;
    }
    else
    if (seqno_cmp > 0) {
        /* master's update seqno is ahead of the slave's.
         * somehow the slave missed/lost some updates
         * (maybe it was restored from backup)
         */

        *apply_update = false;
        SET_STATUS(st, sec_rgy_rep_update_seqno_high);
    }
    else 
    if (seqno_cmp < 0) {
        /* master's idea of the slave's last update
         * seqno is lower than the slave's update 
         * seqno.  Perhaps the master just didn't
         * receive the slave's ack to this update.
         */

        *apply_update = false;

        if (rs_util_seqno_cmp(&master_info->update_seqno, 
                    last_upd_seqno) != 0) {
            SET_STATUS(st, sec_rgy_rep_update_seqno_low);
        }
    }
}

/*
 * Slave's check that the master's compat software
 * version is ok and that its compat software version
 * is ok
 */
PRIVATE void rs_rep_mgmt_slave_chk_sw_compat
  (
    unsigned32      master_compat_sw_rev,
    error_status_t  *st
  )
{
    unsigned32  my_global_sw_rev;

    rs_master_info_get_sw_rev(&my_global_sw_rev, st);
    if (BAD_STATUS(st)) {
	return;
    }

    if ((master_compat_sw_rev > rs_c_max_software_vers)
    || (master_compat_sw_rev > my_global_sw_rev)) {
	/*
	 * We either can't support, or are operating at less than,
	 * the current rev as understood by the master, so 
         * we fail.  The master will decide what to do about
	 * it *send us a stop_til_compat() operation which 
	 * will cause us to either adopt the new rev, if we can,
	 * or shut down completely.
	 */
        SET_STATUS(st, sec_rgy_rep_slave_bad_sw_vers);
    } else if (master_compat_sw_rev < my_global_sw_rev) {
	/* Shouldn't ever happen because a replica can't be
	 * created at a higher operating rev than the master.
	 * The master has rolled back for some reason, but we
	 * don't allow that to happen if there are newer
	 * replicas in the cell.  The admin will have to
	 * shut down all the replicas, delete them from the
	 * master's replist, and recreate them from scratch
	 * once the old master has been restored.
	 */
        SET_STATUS(st, sec_rgy_rep_master_bad_sw_vers);
    } 
}

/*
 * Check that an update which is directed to the
 * propq only is ok; ie. that the update is from 
 * the master and that it is in correct order.
 * The slave's state has already been verified
 * to be rs_c_state_becoming_master.
 *
 * A temporary update seqno counter is used when copying
 * the propq to a slave.  The slave maintains this 
 * update seqno in propq_last_upd_seqno.
 */
PUBLIC void rs_rep_mgmt_slave_chk_propq_upd
  (
    rs_replica_master_info_t    *master_info,
    boolean32                   *apply_update,
    error_status_t              *st
  )
{
    CLEAR_STATUS(st);

    if (!rs_master_info_current_master(
            &master_info->master_id, &master_info->master_seqno)) {
        SET_STATUS(st, sec_rgy_rep_not_from_master);
        return;
    }

    if (!propq_last_upd_seqno_inited) {
        propq_last_upd_seqno_inited = true;
        *apply_update = true;
    }
    else {
        rs_rep_mgmt_slave_chk_upd_seqno(master_info, 
            &propq_last_upd_seqno, apply_update, st);
        if (BAD_STATUS(st)) return;
    }

    if (*apply_update) {
        propq_last_upd_seqno = master_info->update_seqno;
    }
}

/*
 * copy propq has finished.
 * clear the last_upd_seqno_inited flag in
 * case another propq is done.
 * check that the last update that went on the
 * propq has the same seqno as the last
 * update that the replica received during
 * normal propagation.
 */
PUBLIC void rs_rep_mgmt_copy_propq_done
  (
    error_status_t              *st
  )
{
    rs_update_seqno_t   last_upd_seqno;
    sec_timeval_t       last_upd_ts;

    CLEAR_STATUS(st);

    if (!propq_last_upd_seqno_inited) return;

    propq_last_upd_seqno_inited = false;

    rs_log_get_update_info(&last_upd_seqno, &last_upd_ts, st);

    if (rs_util_seqno_cmp(
           &last_upd_seqno, &propq_last_upd_seqno) != 0) {
        SET_STATUS(st, sec_s_pgmerr);
    }
}

PUBLIC void rs_rep_mgmt_get_info
  (
    rs_replica_info_t   *rep_info,
    error_status_t      *st
  )
{ 
    unsigned32 slen;

    CLEAR_STATUS(st);

    rep_info->rep_state = rs_state.rep_state;
    rep_info->cell_sec_id = rs_state.cell_sec_id;
    rep_info->rep_id = rs_state.rep_id;
    rep_info->init_id = rs_state.init_id;

    rs_master_info_get_sw_rev(&rep_info->compat_sw_rev, st);
    if (BAD_STATUS(st)) return;

    u_strncpy(rep_info->sw_rev, rs_sw_vers_get_printstring(rep_info->compat_sw_rev),
	       sizeof(rs_sw_version_t));

    rs_log_get_update_info(&rep_info->last_upd_seqno, 
        &rep_info->last_upd_ts, st);
    if (BAD_STATUS(st)) return;

    rs_log_get_propq_info(&rep_info->base_propq_seqno,
        &rep_info->last_upd_seqno, st);
    if (BAD_STATUS(st)) return;

    rep_info->master = rs_state_master();

    rs_master_info_get(&rep_info->master_known,
        &rep_info->master_id, &rep_info->master_seqno, st);
}

PUBLIC void rs_rep_mgmt_get_info_full
    (
    rs_replica_info_t       *rep_info,
    rs_replica_twr_vec_p_t  *rep_twrs,
    rs_replica_twr_vec_p_t  *master_twrs,
    error_status_t          *st
    )
{ 
    uuid_t              cursor;
    rs_replica_item_t   rep_item;
    unsigned32          nreps;

    *rep_twrs = NULL;
    *master_twrs = NULL;

    rs_rep_mgmt_get_info(rep_info, st);
    if (BAD_STATUS(st)) return;

    cursor = rs_state.rep_id;
    rs_replist2_read(&cursor, 1, &nreps, &rep_item, st);
    if (BAD_STATUS(st)) return;

    *rep_twrs = rep_item.rep_twrs;

    if (rep_info->master_known) {
        cursor = rep_info->master_id;
        rs_replist2_read(&cursor, 1, &nreps, &rep_item, st);
        if (BAD_STATUS(st)) return;
        *master_twrs = rep_item.rep_twrs;
    }
}

/*STUB
*/
PUBLIC void rs_rep_mgmt_change_myname
  (
    unsigned_char_p_t       new_rep_name,
    rs_replica_twr_vec_p_t  rep_twrs,
    error_status_t          *st
  )
{
    /*  munge name ala sec_create_db process_name 
     */

/********* temp comment out
    process_name(new_rep_name, st);
    if (BAD_STATUS(st)) return;

    rs_ns_server_change_name(new_rep_name, st);
    if (BAD_STATUS(st)) return;

    rsdb_set_my_server_name(new_rep_name, st);
    if (BAD_STATUS(st)) return;

    if (rs_state.master) {
        rs_replist2_replace(&rs_state.rep_id, new_rep_name, rep_twrs, st);
        if (BAD_STATUS(st)) return;
    }
*********** end comment out */
}

/******* 
 * stop and delete replica routines 
 *******/

/*
 * Invoked by master when it notices that its
 * softwate version is bad.
 * Master finds out the it sw rev is bad when
 * it propagates to a slave (prop_driver, prop_analyse_status),
 * when it gets a slave's latest update seqno and compat sw rev
 * (prop_get_slave_seqno), and when it receives an i_am_slave
 * message from a slave.
 */
PUBLIC void rs_rep_mgmt_stop_til_sw_ok_task
  (
    void
  )
{
    rs_task_handle_t        task_h;

    rs_task_create(&task_h,
                   (rs_task_func_ptr_t) stop_til_sw_ok_task,
                   "SECD stop until software version ok task", 
                   false, NULL);
}

PRIVATE void *stop_til_sw_ok_task (
    void
)
{
    error_status_t  lst;

    BEGIN_GLOBAL_RIW_EVENT(rs_c_event_stop_til_compat_sw, &lst) { 

        rs_rep_mgmt_stop_til_sw_ok(&lst);

        rpc_mgmt_stop_server_listening(NULL, &lst);

    } END_GLOBAL_RIW_EVENT;
}

PUBLIC void rs_rep_mgmt_stop_til_sw_ok
  (
    error_status_t  *st
  )
{ 
    error_status_t  lst;

    dce_svc_printf(SEC_RS_REP_INCOMPAT_VERSION_MSG);

    /*
     * The pre-1.1 version of this function stored the
     * sw rev away before exiting.  We don't want to
     * do that anyomre.  Doing so would assume that
     * a server could effect a software version
     * upgrade solely when new bits are booted
     * over an old database.  That is not the 
     * model any more.  Slaves never adopt a 
     * new sw rev until told to do so dynamically
     * by the master.  A master that discovers
     * there are newer slaves in a cell passes through
     * this routine on the way to shutdown as well, 
     * and we would definitely not want to save the
     * new software version in that case.  A truly
     * old master (i.e., one erroneously restored
     * from an old backup) would see it's software
     * version bumped on each successive reboot, and
     * eventually believe that it was "current,"
     * even though it really wasn't.  Better to
     * to force a slave to become the master than
     * let an old master retain control of the 
     * cell under these circumstances.
     */  
    rs_rep_mgmt_shutdown(true, st); /* true means need to do checkpoint */ 
    if (BAD_STATUS(st)) return;
}

/* Insert GLOBAL_RIW_DEMOTE before rs_server_unregister() to avoid deadlock  and
   restore the write lock to have the same semantic as before */
PUBLIC void rs_rep_mgmt_shutdown
  (
    boolean32       checkpoint,
    error_status_t  *st
  )
{
    error_status_t  lst;
    CLEAR_STATUS(st);

    switch(rs_state.rep_state) {
    case rs_c_state_closed:
    case rs_c_state_deleted:
        return;

    default: break;
    }

    switch(rs_state.rep_state) {
    case rs_c_state_unknown_to_master:
        GLOBAL_RIW_PROMOTE;
        rsdb_close_files();
        rsdb_state_close();
        rsdb_delete_rgy_files();
        break;

    case rs_c_state_uninitialized:
        GLOBAL_RIW_PROMOTE;
        rsdb_close_files();
        rs_mkey_purge();
        rsdb_state_close();
        break;

    case rs_c_state_initializing:
        GLOBAL_RIW_PROMOTE;
        rsdb_close_files();
        rs_mkey_purge();
        rsdb_state_close();
        break;

    case rs_c_state_in_service:
        /* Invokes rsdb_checkpt which will 
         * promote global RIW lock 
         */
        rsdb_shutdown(checkpoint, lock_mode_riw);
        rs_mkey_purge();
        rsdb_state_close();
        break;

    case rs_c_state_renaming:
        /* shouldn't have been able to get the
         * global RIW lock when in this state.
         */
        /*** SVC_OPEN : Also log a debug statement :
         *
         *   error("shutdown got global riw lock when in state renaming");
         *
         *  -- Sekhar
         */
        
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_rep,
                       svc_c_sev_error,
                       sec_rgy_rep_pgmerr);
        SET_STATUS(st, sec_rgy_rep_pgmerr);
        break;

    case rs_c_state_copying_dbase:
        /* Promote global RIW lock when
         * copy finishes.
         */
        GLOBAL_RIW_PROMOTE;
        rsdb_shutdown(checkpoint, lock_mode_write);
        rs_mkey_purge();
        rsdb_state_close();
        break;

    case rs_c_state_in_maintenance:
        GLOBAL_RIW_PROMOTE;
        rs_mkey_purge();
        break;

    case rs_c_state_mkey_changing:
        /* shouldn't have been able to get the
         * global RIW lock when in this state.
         */

        /*** SVC_OPEN : Also log a debug statement :
         *
         *   error("shutdown got global riw lock when in state mkey changing");
         *  -- Sekhar
         */
        
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_rep,
                       svc_c_sev_error,
                       sec_rgy_rep_pgmerr);
        SET_STATUS(st, sec_rgy_rep_pgmerr);
        break;

    /* When becoming master, have global RIW lock except
     * when propq is being received and when recovering
     * from failed attempt to become master.  It's ok
     * to stop at both these times.
     * When becoming slave, have global RIW lock except
     * when stopping the prop tasks.  It's ok to stop
     * the process at this point in the master change
     * procedure; the old master has not sent the
     * message to the new master telling it to become
     * the master yet.
     */
    case rs_c_state_becoming_master:
    case rs_c_state_becoming_slave:
        GLOBAL_RIW_PROMOTE;
        rsdb_close_files();
        rs_mkey_purge();
        rsdb_state_close();
        break;

    default:
        /*** SVC_OPEN : Also log a debug statement :
         *
         *   error("shutdown bad replica state");
         *
         *  -- Sekhar
         */
        
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_rep,
                       svc_c_sev_error,
                       sec_rgy_rep_pgmerr);
        SET_STATUS(st, sec_rgy_rep_pgmerr);
        break;
    }

    if (GOOD_STATUS(st)) {
        /* should have global write lock now
         */
        rs_state_set_state_volatile(rs_c_state_closed, &lst);
        GLOBAL_RIW_DEMOTE; /* demote it to avoid deadlock */
        rs_server_unregister();
	GLOBAL_RIW_PROMOTE; /* restore the write lock */
    }

}

/* Insert GLOBAL_RIW_DEMOTE in front of rs_rserver operation to avoid deadlock,
   and restore the write lock afterwards to have the same semantic as before. */
PUBLIC void rs_rep_mgmt_delete
  (
    error_status_t  *st
  )
{
    unsigned_char_p_t   cell_name;
    sec_rgy_name_t      my_name;
    unsigned_char_p_t   full_name;

    CLEAR_STATUS(st);

    if (rs_state.rep_state == rs_c_state_deleted)
        return;

    GLOBAL_RIW_PROMOTE;

    rs_state_set_state(rs_c_state_deleted, st);
    if (BAD_STATUS(st)) return;

    rsdb_get_my_server_name(my_name);
    dce_cf_get_cell_name((char **) &cell_name, st);
    if (BAD_STATUS(st)) return;

    rs_ns_name_compose(cell_name, my_name, &full_name, st);
    if (BAD_STATUS(st)) return;

    GLOBAL_RIW_DEMOTE;
    rs_ns_server_delete_name(full_name, st);
    GLOBAL_RIW_PROMOTE;
    if (BAD_STATUS(st)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_rep , svc_c_sev_error , *st);
        dce_svc_printf(SEC_REP_NM_NOT_DELETED_MSG, full_name);
    }

    rsdb_close_files();
    rs_mkey_purge();
    rsdb_state_close();
    rsdb_delete_rgy_files();

    GLOBAL_RIW_DEMOTE;
    rs_server_unregister();
    GLOBAL_RIW_PROMOTE;

    CLEAR_STATUS(st);
}

PRIVATE void rsdb_add_pk_attr_sch_entries(
    error_status_t    *st_p)
{

    rs_resv_add_pk_attr_sch_entries(st_p);

}


/* 
 * Add pk schema and write atomic log with version change
 */

PRIVATE void rs_pk_schema_add_version_set(
    unsigned32     cur_sw_rev, 
    unsigned32     new_sw_rev, 
    error_status_t *st_p)
{

  /* create pk schema */
  rsdb_add_pk_attr_sch_entries (st_p);

  /* log */

  rs_log_122_sw_version_set(cur_sw_rev, new_sw_rev, (rs_replica_master_info_t *)NULL);


}



/* 
 * Master-only routine.
 *
 * Must be called with at a global
 * read-intend-write lock to ensure 
 * single-threadedness of software revision
 * upgrades.
 */
boolean32 rs_rep_mgmt_master_set_sw_rev (
    unsigned32      new_sw_rev,
    handle_t        auth_rpc_handle, 
    error_status_t  *st_p
)
{
    unsigned32            cur_sw_rev;
    boolean32             set = false;
    sec_id_foreign_t      admin_id;
    sec_acl_mgr_handle_t  acl_mgr_handle;

    rs_master_info_get_sw_rev(&cur_sw_rev, st_p);

    if (BAD_STATUS(st_p) || (cur_sw_rev == new_sw_rev)) {
	return false;
    }

    /*
     * Check that the specified sw rev is not out-of-bounds
     * with what we can support.
     */

    if ((new_sw_rev < rs_c_min_software_vers)
    || (new_sw_rev > rs_c_max_software_vers)) {
	SET_STATUS(st_p, sec_rgy_rep_sw_rev_nosup);
	return false;
    }

    /* 
     * Check that the specified sw rev is not earlier than
     * the current one.
     */
    if (new_sw_rev < cur_sw_rev) {
	SET_STATUS(st_p, sec_rgy_rep_sw_rev_no_setback);
	return false;
    }

    /*
     * Can't bump more than one revision at a time.
     * NOTE WELL:  The following check relies on the fact
     * that the software version constants are defined as 
     * monotonically increasing integers.
     */
    if (new_sw_rev - cur_sw_rev != 1) {
	SET_STATUS(st_p, sec_rgy_rep_sw_rev_multi);
	return false;
    }

    /*
     * !!! Need to check to be sure there are no previous 
     * sw rev updates outstanding for any slaves.
     * Otherwise, the master may try to bump a slave multiple
     * versions in a single set version operation, which would be
     * bad. 
     */
    if (GOOD_STATUS(st_p)) {
      switch (new_sw_rev) {
	case rs_c_software_vers_1_1: 
	    /*
	     * need to activate sec/xattrschema acl. at database
	     * conversion time, we created the acl with a single
	     * acl entry "any_other:----" which effectively
	     * disabled all acl operations (since no principal
	     * had the any rights at all).  if we are now 
	     * supposed to support 1.1 functionality, we create a
	     * a real xattrschema acl and give the administrator who
	     * effected the version change administrative rights
	     * to the xattr schema.
	     */
	    
	    /*
	     * this will construct the appropriate acl and
	     * store it in the volatile database relation.
	     */
	    get_admin_id(auth_rpc_handle, &admin_id, st_p);
	    
	    GLOBAL_RIW_PROMOTE;
	    /*
             * Configure the acl manager with the new software version.
             * Though sec_acl_mgr_configure returns a status parameter,
             * that parameter is always "ok"
             */
            sec_acl_mgr_configure((sec_acl_mgr_config_t) NULL, new_sw_rev,
				  (unsigned_char_p_t) NULL, &acl_mgr_handle, st_p);
	    if (GOOD_STATUS(st_p)) {
		rsdb_activate_xattrschema_acl_dce11(admin_id, st_p);
		if (GOOD_STATUS(st_p)) {
		    /*
		     * need to log any updates so that they will
		     * get replayed and/or propagated as necessary.
                     * This log operation commits the new sw version.
		     */
		    log_sw_version_change(cur_sw_rev, new_sw_rev, st_p);
		}
	    }
	    GLOBAL_RIW_DEMOTE;
	break;
        case rs_c_software_vers_1_2_2: 
	    /* PLACE HOLDER for 1.2.2-specific migration work
	     * 
	     * Nothing to do for global groups.  
             *
	     * Probably lots to do for public key work
	     */

	  /* Need to check if all of the replicas are in DCE1.1 /
	  if (!sec_replica_check_all_version(rs_c_software_vers_1_1)) {
	    *st_p = sec_rgy_rep_sw_rev_outstanding;
	    return false;
	  }   */

	    GLOBAL_RIW_PROMOTE;
	    /*
	     * Configure the acl manager with the new software version.
	     * Though sec_acl_mgr_configure returns a status parameter,
	     * that parameter is always "ok"
	     */
	    sec_acl_mgr_configure((sec_acl_mgr_config_t) NULL, new_sw_rev,
			      (unsigned_char_p_t) NULL, &acl_mgr_handle, st_p);
	    /* Add pk schema and write atomic log with version change*/
	    rs_pk_schema_add_version_set(cur_sw_rev, new_sw_rev, st_p);

	    GLOBAL_RIW_DEMOTE;
        break;

      default:
	SET_STATUS(st_p, sec_rgy_rep_bad_sw_vers);
      break;
      }
    } 
 
    
   if (GOOD_STATUS(st_p)) {
	rs_master_info_set_sw_rev(new_sw_rev, st_p);
    }
    
    /*
     * the following will update both the local rpc
     * runtime and the endpoint map with the interface
     * registrations appropriate to the new sw rev.
     */
    rs_server_register_if(new_sw_rev, st_p);
    
    /*
     * wake up the namespace management thread.
     * it will take whatever steps are necessary to
     * put the cell namespace into the state appropriate
     * for the new sw rev in the background.  we
     * don't want to hold up or abort the set version 
     * operation due to namespace problems.
     */
    if (GOOD_STATUS(st_p)) {
	rs_ns_server_register_wakeup();
    }

    if (BAD_STATUS(st_p)) {
	unsigned char *svc_msgp =  dce_sprintf(*st_p);
	
	dce_svc_printf(SEC_REP_ADOPT_SW_REV_FATAL_MSG, new_sw_rev, 
		       cur_sw_rev, svc_msgp);
    } else {
	set = true;
    }

    return set;
}


/* 
 * slave-only routine.
 *
 * Must be called with at global riw lock
 * single-threadedness of software revision
 * upgrades.
 *
 * Called by a slave on receipt of a 
 * stop_til_sw_compat() message from the
 * master. 
 *
 * The error status situation deserves a slight
 * bit of explanation ...
 *
 * Since the purpose of this routine is to determine
 * whether or not it is indeed proper to shut
 * down a replica due to software version
 * incompatibilities, an "error_status_ok"
 * return value means "yes, the slave cannot
 * support the requested software version and
 * should be shut down."  There are many possible
 * "bad" statuses, many of which indicate real
 * errors.  However, the sec_rgy_rep_sw_rev_adopted
 * status means that the slave is either already operating
 * at the specified software version or has successfully
 * adopted the specifed sofware version.  It's up to the
 * caller to decide whether or not this is truly a bad 
 * status. 
 *
 * Note that because this routine triggers the actual
 * software version adoption process as necessary, it
 * cannot be used as a simple "test" function.
 * 
 */
void rs_rep_mgmt_slave_chk_stop_til_compat (
    unsigned32      compat_sw_rev, 
    error_status_t  *st_p
)
{
    unsigned32            cur_sw_rev;
    error_status_t        lst;
    sec_acl_mgr_handle_t  acl_mgr_handle;

    rs_master_info_get_sw_rev(&cur_sw_rev, st_p);

    if (STATUS_OK(st_p)) {
	
	if (compat_sw_rev == cur_sw_rev) {
	    /* 
	     * We've already adopted the specified
	     * sw rev on a previous occasion, so set
	     * status to "sw_rev_adopted" so our caller
	     * will know *not* to shut us down.
	     */
	    SET_STATUS(st_p, sec_rgy_rep_sw_rev_adopted);
	} 
        else if (compat_sw_rev < cur_sw_rev) {
	    /*
             * Can't rollback sw rev ...
	     */
	    SET_STATUS(st_p, sec_rgy_rep_sw_rev_no_setback);
	} else if (compat_sw_rev > rs_c_max_software_vers) {
	    /* 
	     * Can't support specified software rev so return
	     * status_ok indicating that it is indeed proper
	     * to shut down.
	     */
	    CLEAR_STATUS(st_p);
	} else if (compat_sw_rev - cur_sw_rev != 1) {
	    /*
	     * Can't bump more than one revision at a time.
	     * This should never happen if the master is behaving
	     * sanely, so we don't shut down, but return
	     * the error to the master.
	     * NOTE WELL:  The following check relies on the fact
	     * that the software version constants are defined as 
	     * monotonically increasing integers.  
	     */
	    SET_STATUS(st_p, sec_rgy_rep_sw_rev_multi);
	    
	} else  {  /* (cur_sw_rev < compat_sw_rev <= rs_c_max_software_vers) */
	    /*
	     * Try to adopt specified sw rev
	     */

	    GLOBAL_RIW_PROMOTE;
	
	    /*
	     * Configure the acl manager with the new software version.
	     * sec_acl_mgr_configure() returns a status parameter, but
	     * it's essentially meaningless because it is always
	     * "ok"
	     */
	    sec_acl_mgr_configure((sec_acl_mgr_config_t) NULL, compat_sw_rev,
				  (unsigned_char_p_t) NULL, &acl_mgr_handle, st_p);
	    
	    if (GOOD_STATUS(st_p) && compat_sw_rev == rs_c_software_vers_1_2_2) {
	        /* Add pk schema and write atomic log with version change */
	        rs_pk_schema_add_version_set(cur_sw_rev, compat_sw_rev, st_p);
	    }
	

	    GLOBAL_RIW_DEMOTE;
	    
	    /* Per Greg's request, move this op outside RIW for performance */
	    if (GOOD_STATUS(st_p)) {
		/* Make the new sw rev permanent ... */
		rs_master_info_set_sw_rev(compat_sw_rev, st_p);
	    }


            /* MIGRATION-POINT
	     * 
	     * As of 1.1, there is no need to log the results of a
             * stop_until_compat operation at the slave, because
             *
             *  1) The write of the master info above
             *     made the new sw rev permanent.
             *
             *  2) All other information required to
             *     effect the version change will arrive
             *     in subsequent propagations from the master
             *     and be logged at that point.
             *
             * This may not be true of future versions of
             * security, in which case a new slave sw version
             * log operation may need to be defined.
             */

	    /*
	     * The following will update both the local RPC
	     * runtime and the endpoint map with the interface
	     * registrations appropriate to the new sw rev.
	     */
	    if (GOOD_STATUS(st_p)) {
		rs_server_register_if(compat_sw_rev, st_p);
	    }

	    /*
	     * Wake up the namespace management thread.
	     * It will take whatever steps are necessary to
	     * put the cell namespace into the state appropriate
	     * for the new sw rev
	     */
	    if (GOOD_STATUS(st_p)) {
		rs_ns_server_register_wakeup();
	    }

	    if (BAD_STATUS(st_p)) {
		unsigned char *svc_msgp =  dce_sprintf(*st_p);
		
		dce_svc_printf(SEC_REP_ADOPT_SW_REV_FATAL_MSG, compat_sw_rev, 
			       cur_sw_rev, svc_msgp);
	    }
	    
	    if (GOOD_STATUS(st_p)) {
		/*
		 * Succeeded in adopting new rev, so set
		 * status to "sw_rev_adopted" so our caller
		 * will know *not* to shut us down.
		 */
		SET_STATUS(st_p, sec_rgy_rep_sw_rev_adopted);
	    }
	}
    }
}

static void get_admin_id (
    rpc_binding_handle_t  auth_rpc_handle,  /* [in] */
    sec_id_foreign_t      *client_id,       /* [out] */
    error_status_t        *st_p             /* [out] */
)
{
    rpc_authz_handle_t privs;
    unsigned_char_p_t  sname = NULL;
    unsigned32         authn_level;
    unsigned32         authz_svc;
    unsigned32         authn_svc;
    sec_id_pac_t       *pac_p;

    rpc_binding_inq_auth_client(auth_rpc_handle, &privs, &sname, 
				&authn_level, &authn_svc, &authz_svc, 
				st_p);
    if (GOOD_STATUS(st_p)) {
	if (authz_svc != rpc_c_authz_dce) {
	    SET_STATUS(st_p, sec_rgy_not_authorized);
	} else {
	    pac_p = (sec_id_pac_t *) privs;
            client_id->realm = pac_p->realm;
            client_id->id = pac_p->principal;
	}
    }
}

static void log_sw_version_change (
    unsigned32       old_sw_rev,    
    unsigned32       new_sw_rev,
    error_status_t   *st_p
) 
{
    sec_acl_component_name_t    component_name;
    sec_id_t                    *ignored;
    sec_acl_type_t              sec_acl_type = sec_acl_type_object;
    uuid_t                      *acl_mgr_type_p;
    sec_acl_list_t              *sec_acl_list_p;
    rsdb_acl_ids_t              ids; 
    rs_replica_master_info_t    master_info;

    ids.acl_id = rsdb_get_attr_schema_acl_id();
    ids.obj_id = RSDB_ATTR_SCHEMA_OBJ_ID;
    acl_mgr_type_p = &rsdb_acl_mgr_uuids[rsdb_acl_mgr_type_attr_schema];
    component_name = (unsigned_char_p_t) SEC_ATTR_SCHEMA_OBJ_NAME;
    sec_acl_type = sec_acl_type_object;

    /* 
     * Acquire the xattrschema acl in a form appropriate for
     * logging.
     */

    /*
     * We know that argument 1 (the acl manager handle) is ignored in
     * our implementation, so we just pass NULL
     */
    sec_acl_mgr_lookup((sec_acl_mgr_handle_t) (NULL), (sec_acl_key_t)&ids,
		       acl_mgr_type_p, sec_acl_type, 
		       &sec_acl_list_p, st_p); 

    /*
     * log new sw version and any database updates (as of 1.1, just
     * the xattrschema ACL), required to implement the version upgrade.
     */
    if (GOOD_STATUS(st_p)) {
	rs_log_sw_version_set(old_sw_rev, new_sw_rev, (sec_acl_mgr_handle_t) (NULL), 
			      component_name, acl_mgr_type_p, sec_acl_type, 
			      sec_acl_list_p, (rs_replica_master_info_t *)NULL);
 
	 rsdb_acl_free_list(&sec_acl_list_p);
    }
}

