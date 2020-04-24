/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_master_change.c,v $
 * Revision 1.1.10.1  1996/10/15  20:55:21  arvind
 * 	Final review changes.
 * 	[1996/06/12  17:47 UTC  jrr  /main/HPDCE02/jrr_davis_4/4]
 *
 * 	Add code review changes. Mostly malloc failure detection.
 * 	[1996/06/11  21:18 UTC  jrr  /main/HPDCE02/jrr_davis_4/3]
 *
 * 	More changes to prevent deadlock during master change.
 * 	[1996/06/11  18:12 UTC  jrr  /main/HPDCE02/jrr_davis_4/2]
 *
 * 	Reorganize the code to allow RPC's to old master to happen before promoting global lock.
 * 	[1996/06/11  17:16 UTC  jrr  /main/HPDCE02/jrr_davis_4/1]
 *
 * Revision 1.1.6.3  1996/02/18  23:03:48  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:21  marty]
 * 
 * Revision 1.1.6.2  1995/12/08  17:52:55  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/05/15  21:12 UTC  greg
 * 	Fix botched clearmerge.
 * 	[1995/04/22  21:13 UTC  greg  /main/HPDCE02/SEC_migrate_merge/2]
 * 
 * 	HP revision /main/HPDCE02/1  1995/02/03  15:53 UTC  emartin
 * 	merge in some replication fixes
 * 
 * 	HP revision /main/emartin_mothra2/2  1995/01/27  22:34 UTC  emartin
 * 	change_master - don't promote global riw lock when prop_updates_to_new_master
 * 
 * 	HP revision /main/emartin_mothra2/1  1995/01/26  21:46 UTC  emartin
 * 	change_master - some small fixes when error conditions
 * 	become_master - only restore_slave when bad status and have done
 * 	        some work to become the master
 * 	become_slave - some small fixes when error conditions
 * 	[1995/12/08  17:20:48  root]
 * 
 * Revision 1.1.2.5  1994/08/25  20:05:33  sekhar
 * 	Put back SVC messages for fatal_st() and error_st().
 * 	[1994/08/25  19:53:21  sekhar]
 * 
 * Revision 1.1.2.4  1994/08/04  16:13:37  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:11:37  mdf]
 * 
 * Revision 1.1.2.2  1994/06/09  19:32:03  annie
 * 	cr10871 - expand copyright
 * 	[1994/06/09  19:31:21  annie]
 * 
 * Revision 1.1.2.1  1994/06/02  21:25:30  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:18:50  mdf]
 * 
 * 	hp_sec_to_osf_3 drop, removed __STDC__ stuff.
 * 	[1994/05/19  19:49:33  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 	[1994/05/18  12:00:40  mdf]
 * 
 * $EndLog$
 */
/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */

/*
 * Copyright (c) Hewlett-Packard Company 1991, 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 *      Registry Server - master change.
 */

#include <stdio.h>

#include <dce/dce.h>
#include <dce/nbase.h>
#include <dce/dce_cf.h>
#include <dce/rplbase.h>

#include <rs_base.h>
#include <rs_ver.h>

#include <rs_pvt.h>
#include <rs_state.h>
#include <rs_master.h>
#include <rs_util.h>
#include <rs_rep_pvt.h>

#include <rs_replist2.h>
#include <rs_master_replist.h>
#include <rs_master_change.h>
#include <rs_rep_util.h>
#include <rs_rep_mgmt.h>
#include <rs.h>
#include <rs_ns.h>
#include <rs_lock.h>
#include <rs_ch_lock.h>
#include <rs_rsdb.h>
#include <rs_log.h>
#include <rs_log_replist.h>
#include <rs_prop_tasks.h>
#include <rs_rep_bind.h>
#include <rs_task.h>

#include <rsdb.h>
#include <rsdb_replica.h>

#include <rrs_repmgr.h>

#include <dce/aclbase.h>
#include <rsdb_acl.h>

#include <sec_svc.h>


/*
 * Local Routines
 */

PRIVATE void chk_bind_to_new_master (
    uuid_p_t            new_master_id,  /* [in] */
    rs_rep_binding_t    *rep_binding,   /* [out] */
    error_status_t      *st             /* [out] */
);

PRIVATE void rs_mc_setup_new_master (
    rpc_binding_handle_t        rpc_binding,
    error_status_t              *st
);

PRIVATE void rs_mc_start_new_master (
    rs_replica_master_info_t    *new_master_info,
    rs_update_seqno_t           base_prop_seqno,
    error_status_t              *st
);

PRIVATE void rs_mc_restore_slave (
    error_status_t              *st
);

PRIVATE void chk_master_dupe (
    error_status_t          *st
);

PRIVATE void rs_mc_change_master_to_slave (
    rs_replica_master_info_p_t new_master_info,
    error_status_t             *st
);


/************
 * Routines for graceful change master 
 ************/

/*
 * support for rs_rep_admin_change_master.
 *
 * old master side of change_master
 */
PUBLIC  void  rs_mc_change_master
  (
    handle_t            h,
    uuid_p_t            new_master_id,  /* [in] */
    error_status_t      *st
  )
{
    rs_rep_binding_t        nm_binding;
    rs_update_seqno_t       base_prop_seqno;
    rs_update_seqno_t       last_update_seqno;
    rs_replica_master_info_t   master_info;
    rs_replica_master_info_t   new_master_info;
    error_status_t          lst;

    CLEAR_STATUS(st);

    BEGIN_GLOBAL_RIW_EVENT(rs_c_event_change_master, st) {

        rs_audit_trail_log(h, "Attempt change master", 
               (char *) NULL);

        rs_rep_util_check_caller(h, sec_acl_perm_Admin, st);

        if (GOOD_STATUS(st)) {
	    chk_bind_to_new_master(new_master_id, &nm_binding, st);
        }

        /*
         * Set volatile state at end of this lock block.
         * If bad status will return; don't want to return
         * when in becoming_slave state.
         */
        if (GOOD_STATUS(st)) {
            GLOBAL_RIW_PROMOTE;
            rs_state_set_state_volatile(rs_c_state_becoming_slave, st);
        }

    } END_GLOBAL_RIW_EVENT

    if (BAD_STATUS(st)) return;

    /*
     * prop tasks may be waiting for a lock.
     * Since mutex_lock is not a cancellation 
     * point, must unlock before stopping
     * prop tasks.
     */
    if (GOOD_STATUS(st)) {
        rs_prop_tasks_stop(st);
    }

    GLOBAL_RIW_LOCK {

        if (GOOD_STATUS(st)) { 
            if (!rs_state_current_state(rs_c_state_becoming_slave))
                SET_STATUS(st, sec_rgy_rep_bad_state);
        }            

        /*
         * propagate outstanding updates to the
         * new master
         */
        if (GOOD_STATUS(st)) {
            rs_prop_updates_to_new_master(new_master_id, 
					  &nm_binding, st);
        }

        if (GOOD_STATUS(st)) {
            rs_prop_tasks_get_master_info(&master_info, st);

            rs_log_get_propq_info(&base_prop_seqno, 
                &last_update_seqno, st);
        }

        /*
         * tell new master to become the master
         */
        if (GOOD_STATUS(st)) {
            rrs_rep_mgr_become_master(nm_binding.rep_handle,
                base_prop_seqno, 
                &master_info, &new_master_info, st);
        }

        GLOBAL_RIW_PROMOTE;

        if (GOOD_STATUS(st)) {
            rs_mc_change_master_to_slave(
                &new_master_info, &lst);
        } 
        else {
            /* re-init volatile master replist
             * and restart prop tasks
             */
            rs_rep_mgmt_start_new_master(false, &lst);
            if (BAD_STATUS(&lst))
                SEC_SVC_PRINT_MSG_ST(SEC_RGY_REP_MST_RESTART_PROP_MSG, lst);
        }

        if (rs_state_current_state(rs_c_state_becoming_slave))
            rs_state_set_state(rs_c_state_in_service, &lst);

	if (GOOD_STATUS(st)) {
	    rs_audit_trail_log(h, "Change master succeeded", 
                (char *) NULL);
        }
	else {
	    rs_audit_trail_log(h, "Change master failed", 
	       (char *) NULL);
        }

    } END_GLOBAL_RIW_LOCK;
}

PRIVATE boolean32 chk_new_master_state_ok
  (
    rs_replica_info_t       *info,
    rs_replica_mgt_item_p_t rp,
    error_status_t          *status
  )
{
    unsigned32  global_sw_rev;

    rs_master_info_get_sw_rev(&global_sw_rev, status);

    if (BAD_STATUS(status)) {
	return false;
    }

    if ( info->compat_sw_rev != global_sw_rev)
    {
        SET_STATUS(status, sec_rgy_rep_slave_bad_sw_vers);
	return false;
    }
    if ( info->rep_state != rs_c_state_in_service )
    {
        SET_STATUS(status, sec_rgy_rep_bad_state);
	return false;
    }
    if ( rp->prop_type != rs_c_replica_prop_update )
    {
        SET_STATUS(status, sec_rgy_rep_bad_state);
	return false;
    }
    return true;
}

/*
 *  Setup authenticated binding to new master and
 *  check compatibility between old and new masters.
 */
PRIVATE void chk_bind_to_new_master
  (
    uuid_p_t            new_master_id,  /* [in] */
    rs_rep_binding_t    *rep_binding,   /* [out] */
    error_status_t      *st             /* [out] */
  )
{
    rs_replica_mgt_item_p_t rp = NULL;
    uuid_t                  cursor;
    error_status_t          lst;
    rs_replica_info_t       rep_info;

    CLEAR_STATUS(st);

    /* setup rpc binding for the new master */
    cursor = *new_master_id;
    rsdb_replica_lookup(&cursor, &rp, st);
    if (BAD_STATUS(st)) return;

    rs_rep_bind_init_auth_to_slave(rp->rep_twrs, rep_binding, st);
    if (BAD_STATUS(st)) {
        rsdb_replica_item_free(&rp, &lst);
        return;
    }

    if (GOOD_STATUS(st)) {
        rrs_rep_adm_info(rep_binding->rep_handle, &rep_info, st);
    }

    if (GOOD_STATUS(st)) {
        chk_new_master_state_ok(&rep_info, rp, st);
    }

    if (rp != NULL)
        rsdb_replica_item_free(&rp, &lst);

    if (BAD_STATUS(st)) {
        rs_rep_bind_free_binding(rep_binding);
    }
}

/*
 *  rs_mc_become_master - new master side of change 
 *  master.
 *
 *  The master tells a slave to become the new master.
 *  The base_prop_seqno is the sequence number of
 *  the earliest update currently on the prop queue.
 */
PUBLIC void rs_mc_become_master
  (
    rs_update_seqno_t           base_prop_seqno,   /* [in] */
    rs_replica_master_info_p_t  old_master_info,   /* [in] */
    rs_replica_master_info_t    *new_master_info,  /* [out] */
    error_status_t              *st
  )
{
    rs_rep_binding_t        rep_binding;
    error_status_t          lst;

    CLEAR_STATUS(st);

    BEGIN_GLOBAL_RIW_EVENT(rs_c_event_become_master, st) {

        rs_rep_mgmt_slave_chk_master(old_master_info, st);

        if (GOOD_STATUS(st)) {
            rs_rep_bind_init_auth_to_master(&rep_binding, st);
        }

        if (GOOD_STATUS(st)) {

            rs_mc_setup_new_master(rep_binding.rep_handle, st);

            if (GOOD_STATUS(st)) {
                rs_state_set_state_volatile(
                    rs_c_state_becoming_master, st);
	    }

            if (BAD_STATUS(st)) {
                rs_rep_bind_free_binding(&rep_binding);
            }
        }

    } END_GLOBAL_RIW_EVENT;

    if (BAD_STATUS(st)) return;

    /*
     * Release all locks so that propq can be
     * sent from old master
     */
    rrs_rep_mgr_copy_propq(rep_binding.rep_handle, st); 

    GLOBAL_RIW_LOCK {

        if (GOOD_STATUS(st)) { 
            if (!rs_state_current_state(rs_c_state_becoming_master))
                SET_STATUS(st, sec_rgy_rep_bad_state);
        }            

        GLOBAL_RIW_PROMOTE;

        if (GOOD_STATUS(st)) { 
            rs_mc_start_new_master(
                new_master_info, base_prop_seqno, st);
        }

        if (GOOD_STATUS(st)) { 
            rs_log_replist_master(
            &new_master_info->master_id, 
            &new_master_info->master_seqno, NULL);
        }

    } END_GLOBAL_RIW_LOCK;

    if (BAD_STATUS(st)) {
        /*
         * take global write lock after
         * stopping prop tasks.
         */
        rs_mc_restore_slave(&lst);
    }

    rs_rep_bind_free_binding(&rep_binding);
}


/*
 * Invoked with the global RIW lock.
 *
 * Read the old master's replica list into a list.
 * Pass the list back so that write_master_replist can
 * update the local DB. This is done in two steps so that
 * going remote in rrs_replist_read_full() can be done
 * before promoting the global lock.
 *
 */ 
PRIVATE void read_master_replist
  (
    rpc_binding_handle_t        rpc_binding,
    uuid_p_t                    old_master_id,
    rs_replica_mgt_item_list_t  **full_replist,
    error_status_t              *st          
  )
{
#define MAX_ENTS 25
    uuid_t                              marker;
    unsigned32                          n_ents;
    rs_replica_item_full_t              replist[MAX_ENTS], 
                                        *rfp;
    rs_replica_mgt_item_t               rep_item; 
    rs_replica_mgt_item_list_t          *nlp, 
                                        *list_head = NULL, 
                                        *list_tail = NULL;
    uuid_t                              init_id;
    int                                 i;

    CLEAR_STATUS(st);
    
    *full_replist = NULL;

    uuid_create(&init_id, st);
    if (BAD_STATUS(st)) return;

    /* Start replist_read at beginning of replist */
    uuid_create_nil(&marker, st);
    if (BAD_STATUS(st)) return;

    for (;;) {
        n_ents = 0;

        /* Get replica list from old master */
        rrs_replist_read_full(rpc_binding, &marker, MAX_ENTS,
                              &n_ents, replist, st);
        if (BAD_STATUS(st)) {
            if (STATUS_EQUAL(st, sec_rgy_object_not_found))
                CLEAR_STATUS(st);
            break;
        }

        /*
         * Pack the replist info into rs_replica_mgt_item_list_t's
         * for writing later.
         */
        for (i = 0, rfp = replist; i < n_ents; i++) {
            nlp = (rs_replica_mgt_item_list_t *)
                   malloc(sizeof(rs_replica_mgt_item_list_t));
            if (nlp == NULL) {
                SET_STATUS(st, sec_rgy_cant_allocate_memory);
                return;
            }

            /* Create the new list */
            if (list_tail == NULL) {
                list_head = nlp;
                list_tail = nlp;
            }
            list_tail->next = nlp;
            list_tail = list_tail->next;
            list_tail->next = NULL;

            /* Store each replica's info */
            if (uuid_equal(&rfp->rep_id, old_master_id, st)) {
                rs_rep_util_rep_item_copy(&rfp->rep_id, rfp->rep_name, 
                                          rfp->rep_twrs, rs_c_replica_prop_update,
                                          rs_c_replica_prop_unknown, 	  
                                          NULL, NULL, &nlp->item, st);
                if (BAD_STATUS(st)) {
                    return;
                }
            }
            else {
                rs_rep_util_rep_item_copy(&rfp->rep_id, rfp->rep_name, 
                                          rfp->rep_twrs, 
                                          rfp->prop_info.prop_type,
                                          rs_c_replica_prop_unknown,
                                          &init_id, NULL, &nlp->item, st);
                if (BAD_STATUS(st)) {
                    return;
                }
            }
    
            /*
             * free space allocated by client stubs
             * for embedded ptr fields.
             */
            rpc_ss_client_free(rfp->rep_name);
            rpc_ss_client_free(rfp->rep_twrs);

            rfp++;    
        }

        if (BAD_STATUS(st) || (n_ents < MAX_ENTS))
            break;
    }

    if (GOOD_STATUS(st)) {
        *full_replist = list_head;
    } 
}


/*
 * Invoked with the global write lock.
 *
 * Replace all entries on the new master's
 * replist with the old master's entries,
 * thus getting each replica's current
 * prop_type.
 */
PRIVATE void write_master_replist 
(
   rs_replica_mgt_item_list_t   **replist,
   error_status_t               *st
)
{
    rs_replica_mgt_item_list_t  *nlp, *temp;
    rs_replica_mgt_item_t       *item;
    boolean32                   do_replace = TRUE;

    CLEAR_STATUS(st);

    nlp = *replist;

    while (nlp != NULL) {
        item = nlp->item;

        /*
         * If the rsdb_replica_replace fails, keep looping
         * until we free all the memory.
         */
        if (do_replace) {
            rsdb_replica_replace(item, st);
            if (BAD_STATUS(st)) {
                do_replace = FALSE;
            }
        }

        free(item->rep_name);
        TOWER_VEC_FREE(free, item->rep_twrs);
        temp = nlp;
        nlp = nlp->next;
        free(temp->item);
        free(temp);
    }
}    



/* 
 *  Setup new master environment
 *  Invoked with global RIW lock.
 *
 *  Assume that the top level call can simply
 *  return to caller when this routine returns
 *  so don't do anything that would effect the
 *  normal running of the slave.
 */
PRIVATE void rs_mc_setup_new_master
  (
    rpc_binding_handle_t        rpc_binding,
    error_status_t              *st
  )
{
    boolean32                   master_known;
    rs_replica_master_info_t    old_master_info;
    rs_update_seqno_t           last_upd_seqno;
    sec_timeval_t               last_upd_ts;
    rs_replica_mgt_item_list_t  *full_replist;
    error_status_t              lst;

    CLEAR_STATUS(st);

    rs_master_info_get(&master_known,
		       &old_master_info.master_id, 
		       &old_master_info.master_seqno, st);
    if ((!master_known) || BAD_STATUS(st)) {
        SET_STATUS(st, sec_rgy_rep_master_not_found);
        return;
    }

    /* Read full replist from old_master */
    read_master_replist(rpc_binding, 
                        &old_master_info.master_id, 
                        &full_replist, 
                        st);
    if (BAD_STATUS(st)) return;

    /* Promote the lock before writing */
    GLOBAL_RIW_PROMOTE;

    write_master_replist(&full_replist, st);
    if (BAD_STATUS(st)) return;

    /*
     * if new master_seqno will not be last_upd_seqno + 1,
     * mark all reps for init.  If die in middle of change
     * master, reps could get updates from old master and
     * new master couldn't detect this.
     */
    rs_log_get_update_info(&last_upd_seqno, &last_upd_ts, st);
    if (BAD_STATUS(st)) return;

    if (rs_util_seqno_cmp(&old_master_info.master_seqno,
            &last_upd_seqno) > 0) {
        rs_rep_mgmt_mark_reps_for_init(true, st);
        if (BAD_STATUS(st)) return;
    }
        
    rsdb_checkpt(rs_c_state_in_service, lock_mode_write, st);
    if (BAD_STATUS(st))
        SEC_SVC_PRINT_MSG_ST(SEC_RGY_RSDB_CHECKPT_MSG, *st);
    
    /* clear in memory propq
     */
    rsdb_clear_mem_log(); 

}

/* 
 *   Start new master tasks
 *   This function is invoked with global_write_lock.
 */
PRIVATE void rs_mc_start_new_master
  (
    rs_replica_master_info_t    *new_master_info,
    rs_update_seqno_t           base_prop_seqno,
    error_status_t              *st
  )
{
    error_status_t              lst;
    boolean32                   master_known;

    CLEAR_STATUS(st);

    /*
     * clear seqno checking state used when
     * copying propq.
     * check that last upd on propq is same
     * as this replica's last_upd_seqno.
     */
    rs_rep_mgmt_copy_propq_done(st);
    if (BAD_STATUS(st)) return;

    /* update base propq seqno in log header */
    rs_log_write_base_prop_seqno(base_prop_seqno, st);
    if (BAD_STATUS(st)) return;

    /*
     * ready to be master now.
     *
     * set volatile rs_state.master_flag so
     * can init volatile master_replist and
     * start prop tasks.
     *
     * init volatile master_replist and start 
     * prop tasks.
     */
    rs_state_set_master_flag(true, &lst);

    rs_rep_mgmt_start_new_master(false, st);
    if (BAD_STATUS(st)) return;

    /*
     * commit to being master by writing new master
     * info to disk.
     */
    rs_master_info_become_master(st);
    if (BAD_STATUS(st)) return;

    /*
     * committed to being new master.
     * must return good status now.
     */

    /*
     * Set state last.  If cannot become 
     * master should return in initial state
     * (normally becoming_master state).
     */
    rs_state_set_state(rs_c_state_in_service, &lst);
    
    rs_master_info_get(&master_known,
        &new_master_info->master_id,
        &new_master_info->master_seqno, &lst);

    /* wakeup register server task remove this
     * replica from the sec.v1 group
     */
    rs_ns_server_register_wakeup();
}



/**********
 * routines to support force become master 
 **********/

/*
 * do the work to force become a master.
 */
PUBLIC  void  rs_mc_force_become_master
  (
    handle_t            h,
    error_status_t      *st
  )
{
    boolean32                   started_new_master;
    boolean32                   master_known;
    rs_replica_master_info_t    master_info;
    error_status_t              lst;

    CLEAR_STATUS(st);

    started_new_master = false;

    BEGIN_GLOBAL_RIW_EVENT(rs_c_event_become_master, st) {

        rs_audit_trail_log(h, "Attempt become master", 
			   (char *) NULL);

        rs_rep_util_check_caller(h, sec_acl_perm_Admin, st);

        /* check if another master is up */
        if (GOOD_STATUS(st)) {
            chk_master_dupe(st);
        }

        GLOBAL_RIW_PROMOTE;

	if (GOOD_STATUS(st)) {

            /*
             * ready to be master now.
             *
             * set volatile rs_state.master_flag so
             * can get master stuff going.
             *
             * init volatile master_replist and start 
             * prop tasks.
             */

            started_new_master = true;

            rs_state_set_master_flag(true, &lst);    

    	    rs_rep_mgmt_start_new_master(true, st);
        }
    
        if (GOOD_STATUS(st)) {
            rsdb_checkpt(rs_c_state_in_service, lock_mode_write, st);
        }

	if (GOOD_STATUS(st)) {
            /*
             * commit to being master by
             * writing new master info to disk.
             */
	    rs_master_info_become_master(st);
        }

	GLOBAL_RIW_DEMOTE;
    
        if (GOOD_STATUS(st)) {
            rs_master_info_get(&master_known,
                &master_info.master_id, &master_info.master_seqno, &lst);

	    rs_log_replist_master(&master_info.master_id, 
				  &master_info.master_seqno, NULL);

            /* fix master registration in nameservice.
             * only master should be in sec.v1 group
             */
            rs_ns_server_register_wakeup();
        }

	if (GOOD_STATUS(st)) {
	    rs_audit_trail_log(h, "Become master succeeded", 
                (char *) NULL);
        }
        else {
	    rs_audit_trail_log(h, "Become master failed", 
	       (char *) NULL);
        }

        if (BAD_STATUS(st) && started_new_master) {

            GLOBAL_RIW_PROMOTE;

            /*
             * will release locks for some cleanup
             * but don't want any updates to arrive
             * during cleanup
             */
            rs_state_set_state_volatile(rs_c_state_becoming_master, &lst);
	 }

    } END_GLOBAL_RIW_EVENT;

    if (BAD_STATUS(st) && started_new_master) {
        /*
         * Failed to become master.
         * Restore slave state.
         * will take global write lock after
         * stopping prop tasks.
         * will set state back to in_service.
         */
        rs_mc_restore_slave(&lst);
    }
}

/*
 * Try to reach the current master.  Return
 * an error if can communicate with the
 * current master replica and it is still
 * the master.
 */
PRIVATE void chk_master_dupe
  (
    error_status_t          *st
  )
{
    rs_rep_binding_t        rep_binding;
    rs_replica_info_t       old_master_info;
    unsigned32              i;

    CLEAR_STATUS(st);

    for (i = 0; i < 10; i++) {

        rs_rep_bind_init_auth_to_master(&rep_binding, st);
        if (BAD_STATUS(st)) break;
    
        rrs_rep_adm_info(rep_binding.rep_handle, &old_master_info, st);
    
        rs_rep_bind_free_binding(&rep_binding);
    
        if (BAD_STATUS(st)) break;

        if (old_master_info.master) {
            SET_STATUS(st, sec_rgy_rep_master_dup);
            return;
        }

        /*
         * Replica whose info is in old_master_info
         * is no longer the master.  Adopt the new
         * master if it's later than this replica's
         * current master.
         */
         if (!old_master_info.master_known) break;

         rs_master_info_check_master(
             &old_master_info.master_id,
             &old_master_info.master_seqno, st);

         /*
          * Not appropriate to adopt this other
          * master, so give up trying to reach the
          * master.
          */
         if (BAD_STATUS(st)) break;

         /* 
          * Successfully adopted a new master.
          * Try to communicate with it.
          */
    }

    /* 
     * cannot communicate with current master; assume
     * it is no longer the master.
     */
    CLEAR_STATUS(st);

}

/*************
 * routines to support become slave 
 *************/

/*
 * do the work to become a slave
 */
PUBLIC  void  rs_mc_become_slave
  (
    handle_t            h,
    error_status_t      *st
  )
{
    error_status_t             lst;

    CLEAR_STATUS(st);

    BEGIN_GLOBAL_RIW_EVENT(rs_c_event_become_slave, st) {

        rs_audit_trail_log(h, "Attempt become slave", 
			   (char *) NULL);

        rs_rep_util_check_caller(h, sec_acl_perm_Admin, st);

        /*
         * Set volatile state at end of this lock block.
         * If bad status will return; don't want to return
         * when in becoming_slave state.
         */
        if (GOOD_STATUS(st)) {
            GLOBAL_RIW_PROMOTE;
            rs_state_set_state_volatile(rs_c_state_becoming_slave, st);
        }

    } END_GLOBAL_RIW_EVENT;

    if (BAD_STATUS(st)) return;

    /*
     * prop tasks may be waiting for a lock.
     * Since mutex_lock is not a cancellation 
     * point, must unlock before stopping
     * prop tasks.
     */
    if (GOOD_STATUS(st)) {
	rs_prop_tasks_stop(st);
    }

    GLOBAL_RIW_LOCK {

        if (GOOD_STATUS(st)) { 
            if (!rs_state_current_state(rs_c_state_becoming_slave))
                SET_STATUS(st, sec_rgy_rep_bad_state);
        }            

	GLOBAL_RIW_PROMOTE;

	if (GOOD_STATUS(st)) {
	    rs_mc_change_master_to_slave(NULL, st);
	}

        if (BAD_STATUS(st)) {
            /* re-init volatile master replist
             * and restart prop tasks
             */
            rs_rep_mgmt_start_new_master(false, &lst);
            if (BAD_STATUS(&lst))
                SEC_SVC_PRINT_MSG_ST(SEC_RGY_REP_SLV_RESTART_PROP_MSG, lst);
        }

        if (rs_state_current_state(rs_c_state_becoming_slave))
            rs_state_set_state(rs_c_state_in_service, &lst);

	if (GOOD_STATUS(st)) {
	    rs_audit_trail_log(h, "Become slave succeeded", 
                (char *) NULL);
        }
	else {
	    rs_audit_trail_log(h, "Become slaver failed", 
	       (char *) NULL);
        }
	
    } END_GLOBAL_RIW_LOCK;

}


/**************
 * shared utility routines 
 **************/

/*
 * Stably save new master info (and thus commit to
 * NOT being the master).
 *
 * Free resources that only the master needs (eg.
 * volatile copy of replist that manages propagation,
 * propq).
 *
 * NB: prop tasks MUST not be running when this
 * routine is invoked.
 */

PRIVATE void rs_mc_change_master_to_slave
  (
    rs_replica_master_info_p_t new_master_info,
    error_status_t             *st
  )
{
    error_status_t              lst;

    /* Free master's volatile replist before setting
     * master info
     */
    rs_m_replist_free();

    /*
     * Save new master's info.
     * If master_info is NULL, don't have current
     * master info so clear master_info.
     */
    if (new_master_info != NULL) {
        rs_master_info_set(&new_master_info->master_id,
		       &new_master_info->master_seqno, st);
    } 
    else {
        /* set master as unknown 
         */
        rs_master_info_clear(st); 
    }
    
    if (BAD_STATUS(st)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_rep,
                       svc_c_sev_error,
                       *st);
	return;
    }

    /*
     * Delete replicas whose prop_type is
     * deleted from the replist.
     */
    rs_replist2_purge_deleted_reps(&lst);

    /* Free propq
     */
    rsdb_clear_mem_log();

    /* wakeup register server task remove this
     * replica from the sec.v1 group
     */
    rs_ns_server_register_wakeup();
}

/* 
 *  Error occurred when becoming master.
 *  Restore slave state.
 *  Routine must be invoked when in
 *  state becoming master; don't want
 *  any activity when locks freed while
 *  stopping prop tasks.
 */
PRIVATE void rs_mc_restore_slave
  (
    error_status_t              *st
  )
{
    CLEAR_STATUS(st);

    if (!rs_state_current_state(rs_c_state_becoming_master)) {
        SET_STATUS(st, sec_rgy_rep_bad_state);
        return;
    }

    /*
     * can't have replist lock when stop prop tasks
     */
    rs_prop_tasks_stop(st);

    GLOBAL_RIW_LOCK {

        GLOBAL_RIW_PROMOTE;

        /*
         * go into good states for checkpt
         */
        rs_state_set_master_flag(false, st);
        rs_state_set_state_volatile(rs_c_state_in_service, st);

        /*
         * fix log after possible failure when copying
         * propq from old master.
         */
        rsdb_checkpt(rs_c_state_in_service, lock_mode_write, st);

        /* clear in memory propq
         */
        rsdb_clear_mem_log(); 

        /* free volatile master replist.
         * master flag must be true to
         * free the master replist.
         */
        rs_state_set_master_flag(true, st);
        rs_m_replist_free();
        rs_state_set_master_flag(false, st);

    } END_GLOBAL_RIW_LOCK;
}







