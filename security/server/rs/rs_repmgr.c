/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_repmgr.c,v $
 * Revision 1.1.10.3  1996/02/18  00:19:54  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:08:07  marty]
 *
 * Revision 1.1.10.2  1995/12/08  17:56:09  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/15  21:09 UTC  greg
 * 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
 * 	[1995/04/21  18:59 UTC  greg  /main/SEC_migrate_merge/1]
 * 
 * 	Merge work from greg_sec_migrate_acls branch.
 * 	[1995/04/18  18:20 UTC  greg  /main/SEC_migrate/2]
 * 
 * 	We no longer store the global sw_rev when shutting
 * 	down because we can't support it, so remove the
 * 	software rev parameter from the stop_til_compat
 * 	call chain.
 * 
 * 	[1995/04/13  18:29 UTC  greg  /main/SEC_migrate/greg_sec_migrate_acls/1]
 * 
 * 	Create initial SEC_migrate mainline.
 * 	Compiled but not tested.
 * 	[1995/01/27  21:48 UTC  greg  /main/SEC_migrate/1]
 * 
 * 	Modify rs_rep_stop_until_sw_compat_ok()
 * 	routine to adopt the new software rev if
 * 	possible, instead of just shutting down no matter
 * 	what.
 * 
 * 	[1995/01/23  00:34 UTC  greg  /main/greg_migration2/1]
 * 
 * Revision 1.1.6.3  1994/08/19  17:59:34  mdf
 * 	1994/08/18  sekhar
 * 	Converted calls logged when rs_verbose set to NOTICE_VERBOSE
 * 	SVC calls.
 * 	[1994/08/19  13:11:12  mdf]
 * 
 * Revision 1.1.6.2  1994/07/15  15:02:18  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:22:01  mdf]
 * 
 * Revision 1.1.6.1  1994/06/02  21:23:31  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:20:05  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 	[1994/05/18  11:57:05  mdf]
 * 
 * 	HP revision /main/KK/ODESSA_2/1  1994/05/03  18:56 UTC  hanfei
 * 	 merge
 * 
 * 	HP revision /main/KK/hanfei_ch_master/1  1994/04/20  15:22 UTC  hanfei
 * 	merge
 * 
 * 	HP revision /main/HPDCE01/emartin_ch_master/1  1994/04/12  14:34 UTC  emartin
 * 	cleanup for change master - move many routines to rs_master_change.c
 * 
 * 	HP revision /main/HPDCE01/1  1994/01/26  20:04  mdf
 * 	merge
 * 
 * Revision 1.1.4.7  1993/12/20  22:31:35  hanfei
 * 	rs_rep_mgmt_start_master => rs_rep_mgmt_start_new_master
 * 	[1993/12/20  22:21:44  hanfei]
 * 
 * Revision 1.1.4.6  1993/12/17  22:24:10  hanfei
 * 	checks if master is obsolete.
 * 	[1993/12/17  22:05:07  hanfei]
 * 
 * Revision 1.1.4.5  1993/12/16  19:11:59  hanfei
 * 	move rs_ns_server_register_wakeup
 * 	[1993/12/16  18:17:30  hanfei]
 * 
 * 	add some comments
 * 	[1993/12/15  22:19:56  hanfei]
 * 
 * 	rework become_master
 * 	rework copy propq
 * 	[1993/12/11  18:50:06  hanfei]
 * 
 * 	redo rs_rep_mgr_i_am_master
 * 	[1993/12/08  04:46:07  hanfei]
 * 
 * Revision 1.1.4.4  1993/11/04  22:11:01  hanfei
 * 	fixed a bug introducd by rsdb_replica_lookup zeros out
 * 	first argument.
 * 	[1993/11/04  21:55:23  hanfei]
 * 
 * Revision 1.1.4.3  1993/11/03  18:35:38  hanfei
 * 	add master_info as argument to rs_rep_mgr_copy_propq.
 * 	[1993/11/03  18:35:13  hanfei]
 * 
 * Revision 1.1.4.2  1993/10/29  17:19:22  hanfei
 * 	change master work : add codes and functions for client's become master.
 * 	[1993/10/29  17:18:34  hanfei]
 * 
 * 	CR7585 rs_rep_mgr_get_info_and_creds wasn't initializing the [OUT]
 * 	pointer '*rep_auth_info' to NULL, in case something failed before
 * 	it was set properly (causes stub crash on error).
 * 	[1993/03/30  23:13:36  burati]
 * 
 * 	em_bl6f: stop_until_sw_compat - call
 * 	 rpc_mgmt_stop_server_listening
 * 	[1993/02/19  15:58:34  emartin]
 * 
 * 	em_bl6f: do rs_rep_mgr_stop_until_sw_compat
 * 	[1993/02/11  20:20:16  emartin]
 * 
 * 	rs_rep_mgr_init - diagnostic message
 * 	[1993/01/13  16:32:42  emartin]
 * 
 * 	Embedding copyright notice
 * 	[1992/12/28  21:26:32  zeliff]
 * 
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:53:55  sekhar]
 * 
 * 	Third replication drop: bl6a
 * 	 Add encryption_key as input to copy_all
 * 	[1992/11/03  18:48:59  ahop]
 * 
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:19:23  sekhar]
 * 
 * 	First replication code drop.
 * 	[1992/08/31  17:56:10  sekhar]
 * 
 * Revision 1.1.1.6  1993/05/04  20:39:44  mgm
 * 	    Loading drop DCE1_0_2FNL
 * 
 * Revision 1.1.4.2  1992/12/11  21:54:18  emartin
 * 	bl6c: do init, init_done, i_am_slave, copy_all
 * 
 * Revision 1.1.3.2  1992/10/06  17:27:32  burati
 * 	Second replication drop: bl5
 * 
 * $EndLog$
 */

/*
 * Copyright (c) Hewlett-Packard Company 1992
 * Unpublished work. All Rights Reserved.
 */
/*    
 *  Registry replica management server-to-server manager routines.
 */
    
    
#include <crypt.h>
#include <rs_server.h>
#include <rs_ver.h>

#include <rs_repmgr.h> 
#include <rs_rep_util.h>
#include <rs_lock.h>
#include <rs_ch_lock.h>
#include <rs_rep_auth.h>
#include <rs_rep_mgmt.h>
#include <rs_rep_init.h>
#include <rs_log.h>
#include <rs_replist2.h>
#include <rs_log_replist.h>
#include <rs_master_replist.h>
#include <rs_master_replist_pvt.h>
#include <rs_master_change.h>
#include <rs_task.h>


#include <dce/aclbase.h>
#include <rsdb_acl.h>

/*
 *  rs_rep_mgr_get_info_and_creds
 *
 *  get a replica's basic state information
 *  and credentials to authenticate to it
 */
PUBLIC  void  rs_rep_mgr_get_info_and_creds
  (
    handle_t             h,                /* [in] */
    rs_replica_info_t    *rep_info,        /* [out] */
    rs_replica_auth_p_t  *rep_auth_info,   /* [out] */
    error_status_t       *st
  )
{
    rs_replica_auth_p_t  auth_info;        
    unsigned32           size;
    error_status_t       lst;

    /* Init auth info to NULL so it's marshallable if something fails */
    *rep_auth_info = NULL;
    
    BEGIN_READ_EVENT(rs_c_event_primitive_read, lock_db, st) {

        rs_rep_mgmt_get_info(rep_info, st);

        if (GOOD_STATUS(st)) {
            rs_rep_auth_get_auth_info(&auth_info, st);  
        }

        if (GOOD_STATUS(st)) {
     
            /* put the auth info in storage that will go away 
             * on return 
             */
            size = sizeof(rs_replica_auth_t) + auth_info->info_len;
            *rep_auth_info  = (rs_replica_auth_p_t) rpc_ss_allocate(size);
            if (*rep_auth_info == NULL) {
                SET_STATUS(st, sec_s_no_memory);
            } 
            else {
                (*rep_auth_info)->info_len = auth_info->info_len;
                (*rep_auth_info)->info_type = auth_info->info_type;
                memcpy((char *)(*rep_auth_info)->info, (char *) auth_info->info, 
                       auth_info->info_len);
            }
     
            rs_rep_auth_free_auth_info(&auth_info, &lst); 
        }

    } END_READ_EVENT;
}

/*
 *  rs_rep_mgr_init 
 *
 *  master tells slave to initialize itself from
 *  one of the "init_from_reps".  "init_id" identifies
 *  the initialize event and prevents redundant
 *  initializations.
 *
 *  The slave returns the id of the replica it will 
 *  init from and the last update sequence number and 
 *  timestamp of the "init_from_rep". 
 */
PUBLIC  void  rs_rep_mgr_init
  (
    handle_t                    h,                     /* [in] */
    uuid_p_t                    init_id,               /* [in] */
    unsigned32                  nreps,                 /* [in] */
    uuid_p_t                    init_from_rep_ids[],   /* [in, size_is(nreps)] */
    rs_replica_twr_vec_p_t      init_from_rep_twrs[],  /* [in, size_is(nreps)] */
    rs_replica_master_info_p_t  master_info,           /* [in] */
    uuid_t                      *from_rep_id,          /* [out] */
    rs_update_seqno_t           *last_upd_seqno,       /* [out] */
    sec_timeval_t               *last_upd_ts,          /* [out] */
    error_status_t              *st
  )
{
    unsigned char  *msgp = NULL ;

    CLEAR_STATUS(st);

    dce_svc_printf(SEC_REP_RECV_INIT_SLAVE_MSG);

    rs_rep_auth_check(h, st);
    if (BAD_STATUS(st)) return; 
   
    BEGIN_GLOBAL_RIW_SLAVE_EVENT(rs_c_event_slave_initialize, master_info, st) { 

        rs_rep_mgmt_slave_chk_master(master_info, st);

        if (GOOD_STATUS(st)) {
            /* will promote global lock when changing
             * replica's state
             */
            rs_rep_init(init_id, nreps, 
                init_from_rep_ids, init_from_rep_twrs, 
                from_rep_id, last_upd_seqno, last_upd_ts, st);
        }

    } END_GLOBAL_RIW_SLAVE_EVENT;
    if (GOOD_STATUS(st))
        dce_svc_printf(SEC_REP_INIT_SLAVE_SUCC_MSG);
    else {
        dce_svc_printf(SEC_REP_INIT_SLAVE_FAIL_MSG,
                       msgp=dce_sprintf(*st));
        if (msgp)
            free(msgp);
    }
}


/*
 *  rs_rep_mgr_init_done
 *
 *  slave tells master that it is finished initializing
 *  itself from "from_rep_id"
 */
PUBLIC  void  rs_rep_mgr_init_done
  (
    handle_t                    h,               /* [in] */
    uuid_p_t                    rep_id,          /* [in] */
    uuid_p_t                    init_id,         /* [in] */
    uuid_p_t                    from_rep_id,     /* [in] */
    rs_update_seqno_t           *last_upd_seqno, /* [in] */
    sec_timeval_t               *last_upd_ts,    /* [in] */
    error_status_t              *init_st,        /* [in] */
    error_status_t              *st
  )
{
    unsigned_char_p_t   rep_id_str;
    error_status_t      lst;

    rep_id_str = NULL;
    uuid_to_string(rep_id, &rep_id_str, &lst);

    BEGIN_MASTER_RIW(lock_replist, st) {

        READ_LOCK(lock_db) {
            rs_audit_trail_log(h, "Attempt notify init replica done", 
                (char *) rep_id_str);
            rs_rep_util_check_caller(h, sec_acl_perm_Initialize, st);

            if (GOOD_STATUS(st)) {
                RIW_PROMOTE(lock_replist);
                rs_replist2_master_init_rep_don(
                    rep_id, init_id, from_rep_id, 
                    last_upd_seqno, last_upd_ts, init_st, st);
    
                /* If the slave reports that the init was successful
                 * (in init_st) and things have checked out with 
                 * the master, log the event.
                 */
                if (GOOD_STATUS(init_st) && GOOD_STATUS(st)) { 
                    rs_audit_trail_log(h, "Init replica succeeded", 
                        (char *) rep_id_str);
                    rs_log_replist_init_rep_done(rep_id, init_id);
                }
            }

        } END_READ_LOCK;

    } END_MASTER_RIW

    if (rep_id_str != NULL)
        rpc_string_free(&rep_id_str, &lst);
}


/*
 *  rs_rep_mgr_i_am_slave
 *
 *  message sent from a slave to the master when 
 *  the slave restarts.  The slave sends the master 
 *  its current tower set and software compatability
 *  version.
 */
PUBLIC  void  rs_rep_mgr_i_am_slave
  (
    handle_t                    h,              /* [in] */
    uuid_p_t                    rep_id,         /* [in] */
    unsigned32                  compat_sw_rev,  /* [in] */
    rs_replica_twr_vec_p_t      rep_twrs,       /* [in] */
    error_status_t              *st
  )
{
    unsigned_char_p_t   rep_id_str;
    error_status_t      lst;

    rep_id_str = NULL;
    uuid_to_string(rep_id, &rep_id_str, &lst);

    BEGIN_MASTER_RIW(lock_replist, st) {

        READ_LOCK(lock_db) {
            rs_audit_trail_log(h, "Attempt i am slave", 
                (char *) rep_id_str);
            rs_rep_util_check_caller(h, sec_acl_perm_mgmt_info, st);

            if (GOOD_STATUS(st)) {
                /*
                 * if slave's tower set has changed this
                 * routine will replace the slave's info
                 * on the replist and log the update
                 */
                rs_rep_mgmt_master_chk_i_am_slv(
                    rep_id, compat_sw_rev, rep_twrs, st);
        
                if (GOOD_STATUS(st)) {
                    rs_audit_trail_log(h, "i am slave succeeded", 
                                   (char *) rep_id_str);
                }
            }

        } END_READ_LOCK;

    } END_MASTER_RIW

    if (rep_id_str != NULL)
        rpc_string_free(&rep_id_str, &lst);
}

/*
 *  rs_rep_mgr_i_am_master
 *
 *  New master tells a slave that it is now
 *  the master.
 *
 */
PUBLIC  void  rs_rep_mgr_i_am_master
  (
    handle_t                    h,            /* [in] */
    rs_replica_master_info_p_t  master_info,  /* [in] */
    error_status_t              *st
  )
{
    boolean32          apply_update=true;
    error_status_t     lst; 

    CLEAR_STATUS(st);


    dce_svc_printf(SEC_REP_RECV_I_AM_MASTER_MSG);

    rs_rep_auth_check(h, st);
    if (BAD_STATUS(st)) return; 

    /* 
     * forgot propq_only flag for this call
     * so cheat and check that in state becoming
     * master instead.
     */

    if (! rs_state_current_state(rs_c_state_becoming_master)) {
   
        BEGIN_GLOBAL_RIW_SLAVE_EVENT(rs_c_event_i_am_master, master_info, st) { 

            rs_rep_mgmt_slave_chk_update(master_info, 
                &apply_update, st);

            if (apply_update) {
                if (GOOD_STATUS(st)) {
                    rs_log_replist_master(
                        &master_info->master_id, &master_info->master_seqno, 
                        master_info);
                }
            }

        } END_GLOBAL_RIW_SLAVE_EVENT;

    } else {

        /* just add update to propq.
         * must go on propq to keep this
         * update's seqno.
         */
        BEGIN_RIW_EVENT(rs_c_event_propq_write, lock_db, st) { 

            rs_rep_mgmt_slave_chk_propq_upd(master_info, 
                                            &apply_update, st);
            if (apply_update) {
                if (GOOD_STATUS(st)) {
                    rs_log_replist_master(
                        &master_info->master_id, &master_info->master_seqno, 
                        master_info);
                }
            }

        } END_RIW_EVENT;
    }
}

/*
 *  rs_rep_mgr_become_master
 *
 *  The master tells a slave to become the new master.
 *  The base_prop_seqno is the sequence number of
 *  the earliest update currently on the prop queue.
 */
PUBLIC void rs_rep_mgr_become_master
  (
    handle_t                    h,                 /* [in] */
    rs_update_seqno_t           base_prop_seqno,   /* [in] */
    rs_replica_master_info_p_t  old_master_info,   /* [in] */
    rs_replica_master_info_t    *new_master_info,  /* [out] */
    error_status_t              *st
  )
{

    CLEAR_STATUS(st);

    dce_svc_printf(SEC_REP_RECV_BECOME_MASTER_MSG);
    rs_rep_auth_check(h, st);
    if (BAD_STATUS(st)) return; 

    rs_mc_become_master(base_prop_seqno, old_master_info, 
                        new_master_info, st);
}

/*
 *  rs_rep_mgr_copy_all
 *
 *  A request sent from one replica (A) to another (B) asking 
 *  replica B to push its entire database to A.
 *
 *  The replica B which is pushing its database 
 *  uses "copy_master_info" as the master_info
 *  which accompanies the updates it sends.
 *  "rep_auth_info" includes a session key which
 *  is used by the two replicas to authenticate 
 *  each other.   "encryption_key" is a key (pickled and
 *  encrypted with the session key) that B will use
 *  to encrypt the account auth_keys during propagation.
 */
PUBLIC  void  rs_rep_mgr_copy_all
  (
    handle_t                    h,  
    rs_replica_master_info_p_t  copy_master_info, 
    rs_replica_auth_p_t         rep_auth_info,   
    rs_acct_key_transmit_t      *encryption_key,
    rs_update_seqno_t           *last_upd_seqno,
    sec_timeval_t               *last_upd_ts,
    error_status_t              *st
  )
{
    BEGIN_GLOBAL_RIW_EVENT(rs_c_event_copy_dbase_from_me, st) {

        rs_audit_trail_log(h, "Attempt copy database", NULL);

        rs_rep_util_check_caller(h, sec_acl_perm_Initialize, st);

        if (GOOD_STATUS(st)) {
            /* will promote global lock when changing
             * replica's state
             */
            rs_rep_init_copy_all_from_me(
                h, copy_master_info, 
                rep_auth_info, encryption_key,
                last_upd_seqno, last_upd_ts, st);

            if (GOOD_STATUS(st)) { 
                rs_audit_trail_log(h, "Copy database  succeeded", NULL);
            }
        }

    } END_GLOBAL_RIW_EVENT;
}

/*
 *  rs_rep_mgr_copy_propq
 *
 *  request from a slave, which is becoming the master,
 *  to the old master to send the new master the
 *  propagation queue
 */
PUBLIC  void  rs_rep_mgr_copy_propq
  (
    handle_t                    h,    /* [in] */
    error_status_t              *st
  )
{

    CLEAR_STATUS(st);

    BEGIN_READ_EVENT(rs_c_event_copy_propq, lock_replist, st) { 
        rs_audit_trail_log(h, "Attempt copy propq", NULL);

        rs_rep_util_check_caller(h, sec_acl_perm_Initialize, st);

        if (GOOD_STATUS(st)) {
            rs_prop_copy_propq(h, st);
        }

        if (GOOD_STATUS(st)) {
            rs_audit_trail_log(h, "Copy propq  succeeded", NULL);
        }
        else {
            rs_audit_trail_log(h, "Copy propq  failed", NULL);
        }

    } END_READ_EVENT;
} 

/* 
 *  rs_rep_mgr_stop_until_sw_compat
 *
 *  Master tells slave to not run until its
 *  software has been updated to compat_sw_rev.
 */
PUBLIC void rs_rep_mgr_stop_until_compat_sw
  (
    handle_t                    h,              /* [in] */
    unsigned32                  compat_sw_rev,  /* [in] */
    rs_replica_master_info_p_t  master_info,    /* [in] */
    error_status_t              *st
  )
{
    error_status_t  lst;

    CLEAR_STATUS(st);

    dce_svc_printf(SEC_REP_RECV_STOP_SW_COMPAT_MSG);

    if (rs_state_exiting()) return;

    rs_rep_auth_check(h, st);
    if (BAD_STATUS(st)) return; 
   
    BEGIN_GLOBAL_RIW_EVENT(rs_c_event_stop_til_compat_sw, st) { 

        rs_master_info_check_master(
            &master_info->master_id, &master_info->master_seqno, st);

        if (GOOD_STATUS(st)) {
	    /*
	     * Will adopt new software rev, if possible.  If so,
	     * will return a sec_rgy_rep_sw_rev_adopted status
	     */
	    rs_rep_mgmt_slave_chk_stop_til_compat(compat_sw_rev, st);

	}

        if (GOOD_STATUS(st)) {
            rs_rep_mgmt_stop_til_sw_ok(st);

            if (GOOD_STATUS(st)) {
                rpc_mgmt_stop_server_listening(NULL, &lst);
            }
        }

    } END_GLOBAL_RIW_EVENT;
} 

