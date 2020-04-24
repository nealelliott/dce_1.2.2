/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_rep_init.c,v $
 * Revision 1.1.9.3  1996/02/18  00:19:29  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:07:50  marty]
 *
 * Revision 1.1.9.2  1995/12/08  17:55:22  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/15  21:08 UTC  greg
 * 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
 * 	[1995/04/21  18:59 UTC  greg  /main/SEC_migrate_merge/1]
 * 
 * 	Add current software version to calls
 * 	to rs_resv_gen_uninit_db()
 * 	[1995/03/06  14:50 UTC  greg  /main/SEC_migrate/5]
 * 
 * 	Use rs_resv_create_uninit_db()  to recover necessary
 * 	reserved entries after re-initializing database.
 * 	[1995/03/03  21:19 UTC  greg  /main/SEC_migrate/4]
 * 
 * 	Recreate reserved attr schema entries after cleaning out
 * 	databases in preparation fro initialization.
 * 	[1995/02/27  18:48 UTC  greg  /main/SEC_migrate/3]
 * 
 * 	fix surrogate master selection to account for sw rev.
 * 	[1995/01/29  21:35 UTC  greg  /main/SEC_migrate/2]
 * 
 * 	Create initial SEC_migrate mainline.
 * 	Compiled but not tested.
 * 	[1995/01/27  21:49 UTC  greg  /main/SEC_migrate/1]
 * 
 * 	Use rs_master_get_global_sw_rev() to ascertain current
 * 	compat sw rev instead of the constant previously defined
 * 	in rs_pvt.h.
 * 	[1995/01/21  21:12 UTC  greg  /main/greg_migration2/1]
 * 
 * Revision 1.1.5.3  1994/08/19  17:59:30  mdf
 * 	1994/08/18 sekhar
 * 	Converted  messages logged when rs_verbose set to DCE_SVC_DEBUG() calls.
 * 	[1994/08/19  13:11:05  mdf]
 * 
 * Revision 1.1.5.2  1994/08/04  16:13:54  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:12:10  mdf]
 * 
 * Revision 1.1.5.1  1994/06/02  21:23:14  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:19:43  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 	[1994/05/18  11:56:49  mdf]
 * 
 * 	HP revision /main/KK/ODESSA_2/2  1994/05/03  21:15 UTC  hanfei
 * 	 merge
 * 
 * 	HP revision /main/KK/ODESSA_2/hanfei_ch_master/1  1994/05/03  20:41 UTC  hanfei
 * 	label change master work
 * 
 * 	HP revision /main/KK/ODESSA_2/1  1994/04/26  19:58 UTC  mdf
 * 	Support for ERA propagation.
 * 
 * 	HP revision /main/KK/mdf_odessa_era_bl33/1  1994/04/26  19:23 UTC  mdf
 * 	ERA propagation support.
 * 
 * Revision 1.1.3.3  1993/10/29  17:10:19  hanfei
 * 	work for change master : add  rs_rep_init_copy_all_to_new_master()
 * 	[1993/10/29  17:06:05  hanfei]
 * 
 * 	slave will retry if get sec_rgy_rep_bad_state from master ( master
 * 	may be busying doing something else).
 * 	[1993/09/10  18:21:57  hanfei]
 * 
 * 	reinit_db - save away properties for rsdb_create_database.
 * 	don't want realm name to be lost when properties are
 * 	inited.
 * 	[1993/02/19  15:53:51  emartin]
 * 
 * 	em_bl6f: a few fixes to rs_rep_init_reinit_db
 * 	 rs_rep_init - remove stub out of re-init
 * 	 calls to rsdb_checkpt have next_rep_state arg
 * 	[1993/02/11  20:22:36  emartin]
 * 
 * 	bl6e: locate_from_rep was missing init_id from non-ansi function decl.
 * 	      copy_all_from_me encrypt key shouldn't be unalloced pointer.
 * 	[1993/02/03  20:32:00  ahop]
 * 
 * 	copy_all_to_me  demote global lock to riw before calling
 * 	rs_rep_init_startup_after_init
 * 	[1993/01/18  22:07:19  emartin]
 * 
 * 	support rs_bypass_auth
 * 	[1993/01/13  16:31:46  emartin]
 * 
 * 	Embedding copyright notice
 * 	[1992/12/28  21:25:56  zeliff]
 * 
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:53:27  sekhar]
 * 
 * 	Remove client_ids from rs_rep_init_db signature.  Use nil rgy_creator.
 * 	[1992/11/16  16:39:01  ahop]
 * 
 * 	Initial revision
 * 	[1992/11/10  23:11:53  ahop]
 * 
 * Revision 1.1.1.6  1993/05/04  20:39:33  mgm
 * 	    Loading drop DCE1_0_2FNL
 * 
 * Revision 1.1.1.6  1993/05/04  20:39:33  mgm
 * 	    Loading drop DCE1_0_2FNL
 * 
 * Revision 1.1.3.4  1992/12/16  20:20:17  emartin
 * 	bl6c: add printfs
 * 
 * Revision 1.1.3.3  1992/12/11  21:53:02  emartin
 * 	do rs_rep_init_startup_after_init
 * 
 * Revision 1.1.3.2  1992/12/10  22:15:20  emartin
 * 	bl6c: do replica initialization routines
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1992, 1993
 * Unpublished work. All Rights Reserved.
 */
/*
 *     Replica initialization routines
 */


#include <dce/dce.h>
#include <dce/nbase.h>
#include <dce/uuid.h>
#include <dce/rplbase.h>

#include <rs_ver.h>
#include <rs_ver_pvt.h>
#include <rs_pvt.h>
#include <rs_rep_pvt.h>
#include <rs_state.h>
#include <rs_reserved.h>
#include <rs_master.h> 
#include <rs.h>
#include <rs_log.h>
#include <rs_util.h>
#include <rs_rep_util.h>
#include <rs_rep_mgmt.h>
#include <rs_lock.h>
#include <rs_ch_lock.h>
#include <rs_rep_init.h>
#include <rs_rep_auth.h>
#include <rs_login.h>
#include <rs_rep_bind.h>
#include <rs_rsdb.h>
#include <rs_mkey.h>
#include <rs_task.h>

#include <rca_pwd.h>
#include <crypt.h>

#include <rsdb.h>
#include <rsdb_pvt.h>
#include <rsdb_create.h>
#include <rsdb_policy.h>
#include <rsdb_replica.h>

#include <rrs_rep_remote.h>
#include <rrs_repmgr.h>
#include <rs_pwd.h>
#include <rs_prop_pgo.h>
#include <rs_prop_acct.h>
#include <rs_prop_plcy.h>
#include <rs_prop_acl.h>
#include <rs_prop_attr_schema.h>
#include <rs_prop_attr.h>
#include <rrs_prop_pgo.h>
#include <rrs_prop_acct.h>
#include <rrs_prop_plcy.h>
#include <rrs_prop_acl.h>
#include <rrs_prop_replist.h>
#include <rrs_prop_attr_schema.h>
#include <rrs_prop_attr.h>

#include <sec_svc.h>

/*
 * Local constants and types
 */
#define rs_c_rep_init_send_done_tries   5
#define rs_c_rep_init_send_done_sleep   120 /* 2 mins */

/*
 * Local variables
 */
PRIVATE uuid_t                  rs_rep_init_id;
PRIVATE uuid_t                  rs_rep_init_from_rep_id;
PRIVATE rpc_binding_handle_t    rs_rep_init_from_rep_handle;
PRIVATE rs_update_seqno_t       rs_rep_init_last_upd_seqno;
PRIVATE sec_timeval_t           rs_rep_init_last_upd_ts;

/*
 * update seqno maintained during init session to
 * insure that we don't apply duplicate updates
 */
PRIVATE boolean32               init_session_last_upd_seq_initd = false;
PRIVATE rs_update_seqno_t       init_session_last_upd_seq;

/*
 * Local routine declarations
 */
#ifdef RS_REP_NAUTH
PRIVATE void copy_all_from_me_no_auth (
    handle_t                    h,  
    rs_replica_master_info_p_t  copy_master_info, 
    rs_replica_auth_p_t         rep_auth_info,   
    rs_acct_key_transmit_t      *encryption_key,
    rs_update_seqno_t           *last_upd_seqno,
    sec_timeval_t               *last_upd_ts,
    error_status_t              *st
);
#endif

PRIVATE void rs_rep_init_save_initing_info (
    uuid_p_t                    from_rep_id,
    rpc_binding_handle_t        from_rep_handle,
    uuid_p_t                    init_id,
    rs_update_seqno_t           *last_upd_seqno,
    sec_timeval_t               *last_upd_ts,
    error_status_t              *st
);

PRIVATE void rs_rep_init_locate_from_rep (
    uuid_p_t                    init_id,               /* [in] */
    unsigned32                  nreps,                 /* [in] */
    uuid_p_t                    init_from_rep_ids[],   /* [in, size_is(nreps)] */
    rs_replica_twr_vec_p_t      init_from_rep_twrs[],  /* [in, size_is(nreps)] */
    unsigned32                  slave_sw_rev,          /* [in] */
    uuid_t                      *from_rep_id,          /* [out] */
    rpc_binding_handle_t        *from_rep_handle,      /* [out] */
    rs_update_seqno_t           *last_upd_seqno,       /* [out] */
    sec_timeval_t               *last_upd_ts,          /* [out] */
    error_status_t              *st
);

PRIVATE void *rs_rep_init_copy_all_to_me (
    void    *arg
);

#ifdef RS_REP_NAUTH
PRIVATE void copy_all_to_me_no_auth (
    void
);
#endif

PRIVATE void rs_rep_init_startup_after_init (
    void
);

PRIVATE void rs_rep_init_send_done (
    error_status_t  *init_st
);

PRIVATE  void  rs_rep_init_recover_db (
    error_status_t   *st
);

PRIVATE void rs_rep_init_reinit_db (
    error_status_t   *st
);


/*
 * Copy this replica's database to another replica
 *
 * Invoked with global riw lock
 *
 * As of 1.1, the compat_sw_rev of copy_master_info
 * indicates the sw_rev at which the slave expects
 * the initialization to occur.  The surrogate master
 * will refuse to initialize the slave if the 
 * surrogate's actual sw_rev is higher than that
 * indicated by the slave.  The surrogate will
 * coninue with the initialization if the surrogate's sw_rev 
 * is lower than that expected by the slave, because
 * in that case, there is no danger that we will attempt
 * to propagate updates that the slave does not understand.
 */
PUBLIC void rs_rep_init_copy_all_from_me
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
    rpc_binding_handle_t    rpc_handle;
    rs_rep_binding_t        rep_binding;
    sec_passwd_rec_t        *session_key;
    sec_passwd_rec_t        encrypt_key;
    error_status_t          lst;

#ifdef RS_REP_NAUTH
    if (rs_bypass_auth) {
        copy_all_from_me_no_auth(
          h, copy_master_info, rep_auth_info, encryption_key, 
          last_upd_seqno, last_upd_ts, st );
        return;
    }
#endif
    
    rs_master_info_get_sw_rev(&copy_master_info->master_compat_sw_rev, st);
    if (BAD_STATUS(st)) return;

    rpc_binding_server_from_client(h, &rpc_handle, st);
    if (BAD_STATUS(st)) return;

    rs_rep_bind_init_from_handle(
        rpc_handle, rep_auth_info, &rep_binding, st);
    if (BAD_STATUS(st)) {
        rpc_binding_free(&rpc_handle, &lst);
        return;
    }

    rs_util_gettime(&copy_master_info->update_ts);
    rs_util_seqno_create(&copy_master_info->update_seqno);

    GLOBAL_RIW_PROMOTE;

    rs_state_set_state_volatile(rs_c_state_copying_dbase, st);

    GLOBAL_RIW_DEMOTE;

    if (BAD_STATUS(st)) goto COPY_EXIT;

    rrs_prop_pgo_add_all(&rep_binding, copy_master_info, st);
    if (BAD_STATUS(st)) goto COPY_EXIT;

    /* Get the session key and use it to decrypt the encryption_key
     * for use in account propagation.
     */
    rs_rep_auth_get_rep_skey(rep_binding.auth_handle, &session_key, st);
    if (BAD_STATUS(st)) goto COPY_EXIT;

    rs_pwd_get_key_by_session_key(encryption_key, session_key, 
                                  &encrypt_key, st);
    if (BAD_STATUS(st)) goto COPY_EXIT;

    /* 
     * We're done with session_key, free it. 
     */
    sec_crypt_free_key(&session_key);

    rrs_prop_acct_add_all(&rep_binding, &encrypt_key, 
                          copy_master_info, st);
    if (BAD_STATUS(st)) goto COPY_EXIT;          

    rrs_prop_policy_all (&rep_binding, copy_master_info, st);
    if (BAD_STATUS(st)) goto COPY_EXIT;          
 
    rrs_prop_acl_replace_all(&rep_binding, copy_master_info, st);
    if (BAD_STATUS(st)) goto COPY_EXIT; 

    /* 
     * Don't bother propagating ERAs if the current software
     * version doesn't support them
     */
    if (rs_sw_feature_test_support(rs_c_sw_feature_era, &lst)) {
	rrs_prop_attr_schema_create_all(&rep_binding, copy_master_info, st);
	if (BAD_STATUS(st)) goto COPY_EXIT; 
	
	rrs_prop_attr_update_all(&rep_binding, copy_master_info, st);
	if (BAD_STATUS(st)) goto COPY_EXIT; 
    }

    rrs_prop_replist_add_all(&rep_binding, copy_master_info, st);
    if (BAD_STATUS(st)) goto COPY_EXIT; 

    rs_log_get_update_info(last_upd_seqno, last_upd_ts, st);
    if (BAD_STATUS(st)) goto COPY_EXIT; 

COPY_EXIT:
    rs_rep_bind_free_binding(&rep_binding);

    GLOBAL_RIW_PROMOTE;

    rs_state_set_state(rs_c_state_in_service, &lst);
    if (BAD_STATUS(&lst)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_rep,
                       svc_c_sev_fatal | svc_c_action_abort,
                       lst);
    }
}

#ifdef RS_REP_NAUTH
PRIVATE void copy_all_from_me_no_auth
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
    rpc_binding_handle_t    rpc_handle;
    rs_rep_binding_t        rep_binding;
    sec_passwd_rec_t        *session_key;
    sec_passwd_rec_t        *encrypt_key;
    error_status_t          lst;

    rs_master_info_get_sw_rev(&copy_master_info->master_compat_sw_rev, st);
    if (BAD_STATUS(st)) return;

    rpc_binding_server_from_client(h, &rpc_handle, st);
    if (BAD_STATUS(st)) return;

    rs_rep_bind_init_from_handle_na(
        rpc_handle, &rep_binding, st);
    if (BAD_STATUS(st)) {
        rpc_binding_free(&rpc_handle, &lst);
        return;
    }

    rs_util_gettime(&copy_master_info->update_ts);
    rs_util_seqno_create(&copy_master_info->update_seqno);

    GLOBAL_RIW_PROMOTE;

    rs_state_set_state_volatile(rs_c_state_copying_dbase, st);

    GLOBAL_RIW_DEMOTE;

    if (BAD_STATUS(st)) goto COPY_EXIT;

    rrs_prop_pgo_add_all(&rep_binding, copy_master_info, st);
    if (BAD_STATUS(st)) goto COPY_EXIT;

    /* gen up an encrypt key like the initee's
     */ 
    rs_rep_auth_gen_init_ekey(&encrypt_key, st);
    if (BAD_STATUS(st)) goto COPY_EXIT;          

    rrs_prop_acct_add_all(&rep_binding, encrypt_key, 
                          copy_master_info, st);
    if (BAD_STATUS(st)) goto COPY_EXIT;          

    rs_rep_auth_destroy_init_ekey(&lst);

    rrs_prop_policy_all (&rep_binding, copy_master_info, st);
    if (BAD_STATUS(st)) goto COPY_EXIT;          
 
    rrs_prop_acl_replace_all(&rep_binding, copy_master_info, st);
    if (BAD_STATUS(st)) goto COPY_EXIT; 

    rrs_prop_replist_add_all(&rep_binding, copy_master_info, st);
    if (BAD_STATUS(st)) goto COPY_EXIT; 

    rrs_prop_attr_schema_create_all(&rep_binding, copy_master_info, st);
    if (BAD_STATUS(st)) goto COPY_EXIT; 

    rrs_prop_attr_update_all(&rep_binding, copy_master_info, st);
    if (BAD_STATUS(st)) goto COPY_EXIT; 

    rs_log_get_update_info(last_upd_seqno, last_upd_ts, st);
    if (BAD_STATUS(st)) goto COPY_EXIT; 

COPY_EXIT:
    rs_rep_bind_free_binding(&rep_binding);

    GLOBAL_RIW_PROMOTE;

    rs_state_set_state(rs_c_state_in_service, &lst);
    if (BAD_STATUS(&lst)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_rep,
                       svc_c_sev_fatal | svc_c_action_abort,
                       lst);
    }
}
#endif

/*
 * Initialize this replica
 */
PUBLIC void rs_rep_init
  (
    uuid_p_t                    init_id,               /* [in] */
    unsigned32                  nreps,                 /* [in] */
    uuid_p_t                    init_from_rep_ids[],   /* [in, size_is(nreps)] */
    rs_replica_twr_vec_p_t      init_from_rep_twrs[],  /* [in, size_is(nreps)] */
    uuid_t                      *from_rep_id,          /* [out] */
    rs_update_seqno_t           *last_upd_seqno,       /* [out] */
    sec_timeval_t               *last_upd_ts,          /* [out] */
    error_status_t              *st
  )
{
    rpc_binding_handle_t    from_rep_handle; 
    rs_task_handle_t        task_h;
    uuid_t                  current_init_id;
    unsigned32              surrogate_sw_rev, slave_sw_rev;
    error_status_t          lst;

    rs_master_info_get_sw_rev(&slave_sw_rev, st);
    if (BAD_STATUS(st)) return;

    switch(rs_state.rep_state) {
        case rs_c_state_uninitialized:
            DCE_SVC_DEBUG((sec_svc_handle, sec_s_rs_rep, svc_c_debug1,
                           "rep_init: locating init_from replica\n"));

            rs_rep_init_locate_from_rep(init_id,
                nreps, init_from_rep_ids, init_from_rep_twrs, 
                slave_sw_rev, from_rep_id, &from_rep_handle, 
                last_upd_seqno, last_upd_ts, st);
            if (BAD_STATUS(st)) return;

            DCE_SVC_DEBUG((sec_svc_handle, sec_s_rs_rep, svc_c_debug1,
                           "rep_init  init_from replica located\n"));

            GLOBAL_RIW_PROMOTE;

            rs_rep_init_save_initing_info(
                from_rep_id, from_rep_handle,
                init_id, last_upd_seqno, last_upd_ts, st);
            if (BAD_STATUS(st)) return;

            if (! rs_task_create(&task_h,
                rs_rep_init_copy_all_to_me, 
                RS_INIT_SLAVE_THR, false, NULL)) {
                SET_STATUS(st, sec_s_pgmerr);
                return;
            }

            rs_state_set_state_volatile(rs_c_state_initializing, st);
            if (BAD_STATUS(st)) return;

            break;

        case rs_c_state_initializing:
            if (uuid_equal(init_id, &rs_rep_init_id, &lst)) {
                *from_rep_id = rs_rep_init_from_rep_id;
                *last_upd_seqno = rs_rep_init_last_upd_seqno;
                *last_upd_ts = rs_rep_init_last_upd_ts;
                CLEAR_STATUS(st);
            }
            else {
                SET_STATUS(st, sec_rgy_rep_bad_state);
            }
            break;

        case rs_c_state_in_service:
            rs_state_get_init_id( &current_init_id, st);
            if (BAD_STATUS(st)) return;

            if (uuid_equal(init_id, &current_init_id, &lst)) {
                /*
                 * This is a redundant request to initialize
                 * the replica.  Just fill output args and
                 * return a special status. 
                 */
                rs_log_get_update_info(last_upd_seqno, last_upd_ts, st);
                if (BAD_STATUS(st)) return;
                uuid_create_nil(from_rep_id, &lst);

                SET_STATUS(st, sec_rgy_rep_already_inited);
                return;
            }

            /*
             * This isn't a redundant request to init
             * so locate a communicado init_from replica,
             * wipeout the slave's database, save away
             * init state info, start the copy_all_to_me
             * task, and finally go into volatile
             * state initing
             */
            rs_rep_init_locate_from_rep(init_id,
                nreps, init_from_rep_ids, init_from_rep_twrs, 
                slave_sw_rev, from_rep_id, &from_rep_handle, 
                last_upd_seqno, last_upd_ts, st);
            if (BAD_STATUS(st)) return;

            GLOBAL_RIW_PROMOTE; 

            rs_rep_init_reinit_db(st);
            if (BAD_STATUS(st)) return;

            rs_rep_init_save_initing_info(
                from_rep_id, from_rep_handle,
                init_id, last_upd_seqno, last_upd_ts, st);
            if (BAD_STATUS(st)) return;

            if (! rs_task_create(&task_h,
                (rs_task_func_ptr_t) rs_rep_init_copy_all_to_me, 
                RS_INIT_SLAVE_THR, false, NULL)) {
                SET_STATUS(st, sec_s_pgmerr);
                return;
            }

            rs_state_set_state_volatile(rs_c_state_initializing, st);
            if (BAD_STATUS(st)) return;

            break;

        default:
            SET_STATUS(st, sec_rgy_rep_bad_state);
            break;
    }
}

PRIVATE void rs_rep_init_save_initing_info
  (
    uuid_p_t                    from_rep_id,
    rpc_binding_handle_t        from_rep_handle,
    uuid_p_t                    init_id,
    rs_update_seqno_t           *last_upd_seqno,
    sec_timeval_t               *last_upd_ts,
    error_status_t              *st
  )
{
    rs_update_seqno_t   surrogate_master_seqno;

    rs_util_seqno_create_random(&surrogate_master_seqno);
    rs_master_info_set_srgt_master(from_rep_id, 
        &surrogate_master_seqno, st);
    if (BAD_STATUS(st)) return;

    rs_rep_init_id = *init_id;
    rs_rep_init_from_rep_id = *from_rep_id;
    rs_rep_init_from_rep_handle = from_rep_handle;
    rs_rep_init_last_upd_seqno = *last_upd_seqno;
    rs_rep_init_last_upd_ts = *last_upd_ts;

    init_session_last_upd_seq_initd = false;
}

/*
 * Note on software version processing.
 *
 * We ignore a surrogate master whose software version
 * is greater than our own because we don't know
 * if we can handle all the database items that
 * might be sent to us by such a surrogate.  And
 * this shouldn't really be a problem anyway, since
 * the true master always attempts to bring an
 * initializing slave up to the current software
 * version for the cell as a whole before actually
 * propagating the "init" event, so unless something
 * is seriously out-of-whack, the situation should
 * never arise.
 */
PRIVATE void rs_rep_init_locate_from_rep 
  (
    uuid_p_t                    init_id,               /* [in] */
    unsigned32                  nreps,                 /* [in] */
    uuid_p_t                    init_from_rep_ids[],   /* [in, size_is(nreps)] */
    rs_replica_twr_vec_p_t      init_from_rep_twrs[],  /* [in, size_is(nreps)] */
    unsigned32                  slave_sw_rev,          /* [in] */
    uuid_t                      *from_rep_id,          /* [out] */
    rpc_binding_handle_t        *from_rep_handle,      /* [out] */
    rs_update_seqno_t           *last_upd_seqno,       /* [out] */
    sec_timeval_t               *last_upd_ts,          /* [out] */
    error_status_t              *st
  )
{
    unsigned32          nr;
    rs_replica_info_t   rep_info;
    boolean32           found = false;
    error_status_t      lst;

    for (nr = 0; nr < nreps; nr++) {
        rs_rep_bind_from_twrs(
            init_from_rep_twrs[nr], from_rep_handle, st);
        if (BAD_STATUS(st)) continue;

#ifdef RS_REP_NAUTH
        if (!rs_bypass_auth) 
#endif
        {
            rs_login_host_set_auth_info(*from_rep_handle, 
                (unsigned_char_p_t) SEC_RGY_SERVER_NAME, st);
            if (BAD_STATUS(st)) continue;
        }

        rrs_rep_adm_info(*from_rep_handle, &rep_info, st);

        if (GOOD_STATUS(st)
        && (rep_info.rep_state == rs_c_state_in_service) 
        && uuid_equal(&rep_info.rep_id, init_from_rep_ids[nr], &lst) 
        && uuid_equal(&rep_info.cell_sec_id, &rs_state.cell_sec_id, &lst)
        && (slave_sw_rev >= rep_info.compat_sw_rev)) {
	    *from_rep_id = rep_info.rep_id;
	    *last_upd_seqno = rep_info.last_upd_seqno;
	    *last_upd_ts = rep_info.last_upd_ts;
	    found = true;
	    break;
        }
    }
    
    if (!found) {
	SET_STATUS(st, sec_rgy_rep_no_suitable_srgt);
    }
}

PRIVATE void *rs_rep_init_copy_all_to_me
  (
    void    *arg
  )
{
    boolean32                   sm_known;
    rs_replica_master_info_t    surrogate_master_info;
    rs_replica_auth_p_t         auth_info;
    sec_passwd_rec_t            *skey;
    sec_passwd_rec_t            *ekey;
    boolean32                   ekey_created;
    rs_acct_key_transmit_t      *xmit_encrypt_key;
    boolean32                   xmit_encrypt_key_created;
    rs_update_seqno_t           last_upd_seqno;
    sec_timeval_t               last_upd_ts;
    error_status_t              lst;
    error_status_t              copy_st;

#ifdef RS_REP_NAUTH
    if (rs_bypass_auth) {
        copy_all_to_me_no_auth();
        return;
    }
#endif

    auth_info = NULL;
    skey = NULL;
    ekey_created = false;
    xmit_encrypt_key_created = false;

    dce_svc_printf(SEC_RS_THR_STARTED_MSG , RS_INIT_SLAVE_THR);

    BEGIN_GLOBAL_RIW_EVENT(rs_c_event_copy_dbase_to_me, &lst) { 

        rs_master_info_get_srgt_master(&sm_known,
            &surrogate_master_info.master_id,
            &surrogate_master_info.master_seqno, &lst);

        rs_rep_auth_get_auth_info(&auth_info, &lst);  
    
        if (GOOD_STATUS(&lst)) {
            rs_rep_auth_get_init_skey(&skey, &lst);
        }
    
        if (GOOD_STATUS(&lst)) {
            rs_rep_auth_gen_init_ekey(&ekey, &lst);
        }
        ekey_created = GOOD_STATUS(&lst);
    
        if (GOOD_STATUS(&lst)) {
            rca_pwd_gen_transmit_rep(NULL, skey, ekey, 
                &xmit_encrypt_key, &lst);
        } 
        xmit_encrypt_key_created = GOOD_STATUS(&lst);
    
        /*
         * wipeout copy of session key and ptr to data encryption
         * key so snoopers can't find them
         */
        if (skey != NULL)
            sec_crypt_free_key(&skey);
        ekey = NULL;

    } END_GLOBAL_RIW_EVENT;

    copy_st = lst;
    
    if (GOOD_STATUS(&lst)) {

        rrs_rep_mgr_copy_all(rs_rep_init_from_rep_handle,
            &surrogate_master_info, auth_info, 
            xmit_encrypt_key, 
            &last_upd_seqno, &last_upd_ts, &copy_st);
    }

    DCE_SVC_DEBUG((sec_svc_handle, sec_s_rs_rep, svc_c_debug1,
                   "rep_init  copy_all to me finished status %lx\n",copy_st));

    BEGIN_GLOBAL_RIW_EVENT(rs_c_event_copy_dbase_to_me, &lst) { 

        if (xmit_encrypt_key_created)
            rca_pwd_free_transmit_rep(&xmit_encrypt_key);
        if (ekey_created)
            rs_rep_auth_destroy_init_ekey(&lst);
        if (auth_info != NULL)
            rs_rep_auth_free_auth_info(&auth_info, &lst);  

        GLOBAL_RIW_PROMOTE;

        if (GOOD_STATUS(&copy_st)) {
            /* set update seqno and ts, then checkpt
             * new database.  checkpt will change state
             * to in_service.  Save new init_id.
             */
            rs_log_set_update_info(&last_upd_seqno, &last_upd_ts, &lst);
            rsdb_checkpt(rs_c_state_in_service, lock_mode_write, &lst);
            if (BAD_STATUS(&lst)) {
                dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                               sec_s_rs_rep,
                               svc_c_sev_fatal | svc_c_action_abort,
                               lst);
            }

            rs_state_set_init_id(&rs_rep_init_id, &lst);
            if (BAD_STATUS(&lst)) {
                dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                   sec_s_rs_rep,
                   svc_c_sev_fatal | svc_c_action_abort,
                   lst);
            }        

            /* allow readers
             */
            GLOBAL_RIW_DEMOTE;

            /*
             * Finish doing things that couldn't be done
             * at startup because slave had a stub database
             */
            rs_rep_init_startup_after_init();
        }
        else {
            /* copy all failed so clear out dbase
             * in memory and re-read uninitialized
             * stub database from disk
             */
            rs_state_set_state_volatile(rs_c_state_uninitialized, &lst);
            if (BAD_STATUS(&lst)) { /* bad status is never returned */
                                    /* but if it does abort         */
                dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                               sec_s_rs_rep,
                               svc_c_sev_fatal | svc_c_action_abort,
                               lst);
            }

            rs_rep_init_recover_db(&lst);
            if (BAD_STATUS(&lst)) {
                dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                               sec_s_rs_rep,
                               svc_c_sev_fatal | svc_c_action_abort,
                               lst);
            }
        }

        lst = copy_st;

    } END_GLOBAL_RIW_EVENT;

    rs_rep_init_send_done(&lst);
    dce_svc_printf(SEC_RS_THR_EXITING_MSG , RS_INIT_SLAVE_THR);
}

#ifdef RS_REP_NAUTH
PRIVATE void copy_all_to_me_no_auth (
    void
)
{
    boolean32                   sm_known;
    rs_replica_master_info_t    surrogate_master_info;
    rs_replica_auth_p_t         auth_info;
    sec_passwd_rec_t            *ekey;
    boolean32                   ekey_created;
    rs_update_seqno_t           last_upd_seqno;
    sec_timeval_t               last_upd_ts;
    error_status_t              lst;
    error_status_t              copy_st;

    auth_info = NULL;
    ekey_created = false;

    DCE_SVC_DEBUG((sec_svc_handle, sec_s_rs_rep, svc_c_debug1,
                   "rep_init: copy_all to me (no auth) task started\n"));

    BEGIN_GLOBAL_RIW_EVENT(rs_c_event_copy_dbase_to_me, &lst) { 

        rs_master_info_get_srgt_master(&sm_known,
            &surrogate_master_info.master_id,
            &surrogate_master_info.master_seqno, &lst);

        rs_rep_auth_get_auth_info(&auth_info, &lst);  
    
        if (GOOD_STATUS(&lst)) {
            rs_rep_auth_gen_init_ekey(&ekey, &lst);
        }
        ekey_created = GOOD_STATUS(&lst);
    
        /*
         * wipeout copy of session key and ptr to data encryption
         * key so snoopers can't find them
         */
        ekey = NULL;

    } END_GLOBAL_RIW_EVENT;

    copy_st = lst;
    
    if (GOOD_STATUS(&lst)) {

        rrs_rep_mgr_copy_all(rs_rep_init_from_rep_handle,
            &surrogate_master_info, auth_info, 
            NULL,
            &last_upd_seqno, &last_upd_ts, &copy_st);
    }

    DCE_SVC_DEBUG((sec_svc_handle, sec_s_rs_rep, svc_c_debug1,
                   "rep_init: copy_all to me (no auth) finished status %lx\n",
                   copy_st));

    BEGIN_GLOBAL_RIW_EVENT(rs_c_event_copy_dbase_to_me, &lst) { 

        if (ekey_created)
            rs_rep_auth_destroy_init_ekey(&lst);
        if (auth_info != NULL)
            rs_rep_auth_free_auth_info(&auth_info, &lst);  

        GLOBAL_RIW_PROMOTE;

        if (GOOD_STATUS(&copy_st)) {
            /* set update seqno and ts, then checkpt
             * new database.  checkpt will change state
             * to in_service.  Save new init_id.
             */
            rs_log_set_update_info(&last_upd_seqno, &last_upd_ts, &lst);
            rsdb_checkpt(rs_c_state_in_service, lock_mode_write, &lst);
            if (BAD_STATUS(&lst)) {
                dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                               sec_s_rs_rep,
                               svc_c_sev_fatal | svc_c_action_abort,
                               lst);
            }

            rs_state_set_init_id(&rs_rep_init_id, &lst);
            if (BAD_STATUS(&lst)) {
                dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                               sec_s_rs_rep,
                               svc_c_sev_fatal | svc_c_action_abort,
                               lst);
            }

            /* allow readers
             */
            GLOBAL_RIW_DEMOTE;

            /*
             * Finish doing things that couldn't be done
             * at startup because slave had a stub database
             */
            rs_rep_init_startup_after_init();
        }
        else {
            /* copy all failed so clear out dbase
             * in memory and re-read uninitialized
             * stub database from disk
             */
            rs_state_set_state_volatile(rs_c_state_uninitialized, &lst);
            if (BAD_STATUS(&lst)) {
                dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                               sec_s_rs_rep,
                               svc_c_sev_fatal | svc_c_action_abort,
                               lst);
            }
            rs_rep_init_recover_db(&lst);
            if (BAD_STATUS(&lst)) {
                dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                               sec_s_rs_rep,
                               svc_c_sev_fatal | svc_c_action_abort,
                               lst);
            }
        }

        lst = copy_st;

    } END_GLOBAL_RIW_EVENT;

    rs_rep_init_send_done(&lst);
}
#endif

/*
 * Invoked when an uninitialized slave
 * becomes initialized to finish up startup
 * jobs that couldn't be done when the
 * slave had a stub database.
 *
 * Must be invoked with global read lock
 */
PRIVATE void rs_rep_init_startup_after_init (
    void
)
{
    boolean32           start_kdc;      /* for debugging */
    error_status_t      status;

    start_kdc = true;
    if (start_kdc)
        rs_start_kerberos();
    
    rs_setup_dce_rgy_identity(&status);
    if (BAD_STATUS(&status)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_rep,
                       svc_c_sev_fatal | svc_c_action_abort,
                       status);
    }

    /*
     * Finish registering with nameservice
     */
    rs_ns_server_register_wakeup();
}



/*
 * Check that this update is in correct order
 * for an initializing slave.  A temporary
 * update seqno counter is used for a initing
 * slave.  The slave maintains this update seqno
 * in init_session_last_upd_seq.
 */
PUBLIC void rs_rep_init_chk_update_seqno
  (
    rs_replica_master_info_t    *master_info,
    boolean32                   *apply_update,
    error_status_t              *st
  )
{
    if (!init_session_last_upd_seq_initd) {
        init_session_last_upd_seq_initd = true;
        *apply_update = true;
        CLEAR_STATUS(st);
    }
    else {
        rs_rep_mgmt_slave_chk_upd_seqno(master_info, 
            &init_session_last_upd_seq, apply_update, st);
        if (BAD_STATUS(st)) return;
    }

    if (*apply_update) {
        init_session_last_upd_seq = master_info->update_seqno;
    }
}

/*
 * Tell the master that a replica initialization is
 * finished, successfully or otherwise
 */
PRIVATE void rs_rep_init_send_done
  (
    error_status_t  *init_st
  )
{ 
    uuid_t                      rep_id;
    uuid_t                      init_id;
    uuid_t                      init_from_rep_id;
    rs_update_seqno_t           last_upd_seqno;
    sec_timeval_t               last_upd_ts;
    boolean32                   master_known;
    uuid_t                      master_id;
    rs_update_seqno_t           master_seqno;
    uuid_t                      cursor;
    rs_replica_mgt_item_t       *rp;
    rs_replica_packed_buff_t    *rep_buff;
    unsigned32                  rep_buff_len;
    rs_rep_binding_t            rep_binding;
    unsigned32                  i;
    boolean32                   done;
    error_status_t              lst;

    /* Get all info that is needed for the remote rs_rep_mgr_init_done
     * call on my local stack.  Initialize binding to master.
     */
    BEGIN_READ_EVENT(rs_c_event_primitive_read, lock_replist, &lst) {

        rep_id = rs_state.rep_id;
        init_id = rs_rep_init_id;
        init_from_rep_id = rs_rep_init_from_rep_id;

        rs_log_get_update_info(&last_upd_seqno, &last_upd_ts, &lst);

        rs_master_info_get(&master_known, &master_id, &master_seqno, &lst);

        if (master_known) {

            cursor = master_id;
            rsdb_replica_lookup_by_ref(&cursor, &rp,
                &rep_buff, &rep_buff_len, &lst);

            if (GOOD_STATUS(&lst)) {
                rs_rep_bind_init_binding(rp->rep_twrs, &rep_binding, &lst);
            }
        }
        else {
            SET_STATUS(&lst, sec_rgy_rep_master_not_found);
        }

    } END_READ_EVENT; 

    if (BAD_STATUS(&lst)) return;

    /*
     * Try the rs_rep_mgr_init_done call a few times.
     * It's not a disaster if we can't reach the master
     * because the master keeps checking the initing 
     * replica's state.
     */
    for (i = 0, done = false; 
         (i < rs_c_rep_init_send_done_tries) && 
         (!done) &&
         (!rs_state_exiting()); 
         i++) 
    {
        if (!rep_binding.valid_handle) {
                rs_rep_bind_auth_to_master(&rep_binding, &lst);
        }

        if (rep_binding.valid_handle) {
            DCE_SVC_DEBUG((sec_svc_handle, sec_s_rs_rep, svc_c_debug1,
                           "prop_init: sending init_done\n"));

            rrs_rep_mgr_init_done(rep_binding.rep_handle,
                &rep_id, &init_id, &init_from_rep_id,
                &last_upd_seqno, &last_upd_ts, init_st, &lst);
        }

        switch(lst) {
            case error_status_ok: 
            case sec_rgy_rep_bad_init_id:
            case sec_rgy_rep_not_master:
            case sec_rgy_rep_pgmerr:
                done = true;
                break;

            default:
                rep_binding.valid_handle = false;
                break;
        }

        rs_task_sleep(rs_c_rep_init_send_done_sleep);
    } 

    rs_rep_bind_free_binding(&rep_binding);
}

/*
 *  r s _ r e p _ i n i t _ r e c o v e r _ d b 
 *
 *  This operation is called by a newly created initializing
 *  slave whose initialization failed before completion
 *  (bad status returned by rs_rep_mgr_copy_all).
 *  This operation restores the in-memory database to the
 *  pre_initialization state (almost empty database)
 *  by clearing the database memory and reloading the
 *  pre-initialization database from disk.
 *  Locking: this operation should be called with the global lock.
 */
PRIVATE  void  rs_rep_init_recover_db
    (
    error_status_t   *st
    )
{
    CLEAR_STATUS(st);

    rsdb_clear_mem();
    rsdb_load(st);

    /* ??? init log info.  
     * We probably don't touch the log during init 
     */
}


/*
 *  r s _ r e p _ i n i t _ r e i n i t _ d b
 *
 *  This operation is called by a re-initializing slave
 *  (a replica that had previously been in_service and
 *  has just received an rs_replica_mgr_init from the master).
 *  This operation creates for the slave a database 
 *  comparable to that of an uninitialized slave.
 *  This operation should be called before rs_rep_mgr_copy_all
 *  is called by a reinitializing slave.
 *  client_ids should represent the identity of the client
 *  who invoked the reinitialization.
 *  Locking: this operation should be called with the global write
 *  lock.
 */
PRIVATE void rs_rep_init_reinit_db
    (
    error_status_t   *st
    )
{
    sec_rgy_name_t              rep_name;
    sec_rgy_properties_t        properties,
                                cr_properties;
    sec_id_t                    local_cell;
    sec_id_t                    rgy_creator;
    rs_replica_mgt_item_p_t     my_rep;
    rs_replica_mgt_item_p_t     master_rep;
    uuid_t                      cursor;
    boolean32                   master_known;
    rs_update_seqno_t           master_seqno;
    unsigned32                  current_sw_rev;
    error_status_t              lst;
                                            
    CLEAR_STATUS(st);
    CLEAR_STATUS(&lst);

    /* Remember enough info to reconstruct the "uninitialized database" 
     */
    rs_master_info_get_sw_rev(&current_sw_rev, st);
    if(BAD_STATUS(st)) {
        return;
    }
    rsdb_get_my_server_name(rep_name);
    rsdb_properties_get_info(&properties, st);
    if(BAD_STATUS(st)) {
        return;
    }
    cr_properties = properties;

    local_cell.name = (unsigned_char_t *)properties.realm;
    local_cell.uuid = properties.realm_uuid;

    /* fill rgy_creator with null values
     */
    rgy_creator.name = NULL;
    uuid_create_nil(&rgy_creator.uuid, &lst);

    /* Remember my and master's replica info 
     */
    cursor = rs_state.rep_id;
    rsdb_replica_lookup(&cursor, &my_rep, st);
    if(BAD_STATUS(st)) {
        return;
    }
    rs_master_info_get(&master_known, &cursor, &master_seqno, st);
    if(GOOD_STATUS(st)) {
        rsdb_replica_lookup(&cursor, &master_rep, st);                
    }
    if(BAD_STATUS(st)) {
        rsdb_replica_item_free(&my_rep, st);
        return;
    }

    /* Clear memory 
     * this doesn't affect state, master_info 
     */
    rsdb_clear_mem();

    rsdb_create_database(&rgy_creator, &local_cell, 
                         (unsigned_char_p_t)rep_name, &cr_properties);

    /* 
     * If we're a 1.1 (or later) replica, but are posing as a pre-1.1
     * replica, then we just wiped out all the reserved Extended
     * Registry attribute schema entries, and the master is not going 
     * to propagate them to us (since it thinks we're a pre-1.1
     * replica, and therefore would not know what to do with them).
     * We need to recreate them now.  If we can't, die a horrible
     * death, cause something is *very* seriously wrong.
     */
    rs_resv_gen_uninit_db(current_sw_rev, st);
    if (BAD_STATUS(st)) {
	dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
		       sec_s_rs_rep,
		       svc_c_sev_fatal | svc_c_action_abort,
		       *st);
    }

    /* store the encrypted master_key in the database again 
     */
    rs_mkey_store_in_db(st);
    if (BAD_STATUS(st)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_rep,
                       svc_c_sev_fatal | svc_c_action_abort,
                       *st);
    }

    rsdb_replica_add(my_rep, st);
    if (BAD_STATUS(st)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_rep,
                       svc_c_sev_fatal | svc_c_action_abort,
                       *st);
    }

    rsdb_replica_add(master_rep, st);
    if (BAD_STATUS(st)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_rep,
                       svc_c_sev_fatal | svc_c_action_abort,
                       *st);
    }

    rsdb_replica_item_free(&my_rep, &lst);
    rsdb_replica_item_free(&master_rep, &lst);

    /* 
     * Just let checkpt truncate the log;
     * the real rs_log_last_upd_seqno will be 
     * set when the replica is inited.
     */
    rsdb_checkpt(rs_c_state_uninitialized, lock_mode_write, st);
    if (BAD_STATUS(st)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_rep,
                       svc_c_sev_fatal | svc_c_action_abort,
                       *st);
    }
}


PUBLIC void rs_rep_init_copy_all_to_new_master
    (
    uuid_t                      *copy_from_rep_id,
    rs_update_seqno_t           *copy_from_upd_seqno,
    error_status_t              *st
    )
{
    rs_task_handle_t        task_h;

    rs_state_set_init_id(copy_from_rep_id, st);
    if (BAD_STATUS(st)) return;

    rs_master_info_set_srgt_master(copy_from_rep_id,
				   copy_from_upd_seqno, st);
    
    if (! rs_task_create(&task_h,
			 (rs_task_func_ptr_t)rs_rep_init_copy_all_to_me,
			 RS_INIT_SLAVE_THR, false, NULL)) {
        SET_STATUS(st, sec_s_pgmerr);
	return;
    }

    rs_state_set_state_volatile(rs_c_state_initializing, st);
    if (BAD_STATUS(st)) return;

}

void  rs_rep_init_chk_master_sw_rev 
    (
    unsigned32      master_compat_sw_rev, 
    error_status_t  *st
    )
{
    unsigned32  my_sw_rev;

    rs_master_info_get_sw_rev(&my_sw_rev, st);

    /*
     * Surrogate master must not have a greater
     * sw rev then our own.  Less than is okay, since it's
     * possible, although not likely, that master got through
     * to us with a new sw rev before it could get through to the
     * the replica we choose to init from and an older version
     * of the software doesn't know anything that we don't.
     */
    if (GOOD_STATUS(st)
    && (master_compat_sw_rev > my_sw_rev)) {
	SET_STATUS(st, sec_rgy_rep_slave_bad_sw_vers);
    }
}

