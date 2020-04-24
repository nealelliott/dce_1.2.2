/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_prop_pgo.c,v $
 * Revision 1.1.18.2  1996/11/13  18:11:02  arvind
 * 	Back out last fix.
 * 	[1996/11/06  19:32 UTC  jrr  /main/DCE_1.2.2/jrr_122_6/1]
 *
 * 	Merge
 * 	[1996/10/29  20:15 UTC  cuti  /main/DCE_1.2.2/cuti_pk_f/1]
 *
 * 	Check NILL_UUID, instead of NIL_PTR in rs_prop_pgo_add_member_global
 * 	[1996/05/24  23:16 UTC  sommerfeld  /main/DCE_1.2.2/1]
 *
 * 	merge in global group work for DCE 1.2.2
 * 	[1996/03/27  17:47 UTC  hanfei  /main/DCE_1.2/2]
 *
 * 	add rs_prop_pgo_add_member_global()
 * 	[1996/03/15  15:45 UTC  hanfei  /main/DCE_1.2/hanfei_dce1.2_b1/4]
 *
 * 	add cell_uuid and princ_uuid for log and prop data since
 * 	add_member need this information when creating foreign principal
 * 	locally.
 * 	[1996/02/29  18:53 UTC  hanfei  /main/DCE_1.2/hanfei_dce1.2_b1/3]
 *
 * 	rs_prop_pgo_add_member() : when in a replica, an add member operation
 * 	need to get cell and princ uuids for foreign principal before rsdb call.
 * 	[1996/02/20  15:15 UTC  hanfei  /main/DCE_1.2/hanfei_dce1.2_b1/2]
 *
 * 	multi-cell group work: add NULL_PTR as cell_uuid to rs_util_pgo_add() call.
 * 	[1996/02/01  16:30 UTC  hanfei  /main/DCE_1.2/hanfei_dce1.2_b1/1]
 *
 * 	Update OSF copyright years
 * 	[1996/02/17  23:07:34  marty]
 *
 * 	Submit OSF/DCE 1.2.1
 * 	HP 	[1995/07/13  21:37 UTC  burati  /main/HPDCE02/1]
 *
 * 	Merge ACL alias fix (CHFts15622) from mb_mothra5
 * 	HP 	[1995/07/13  20:48 UTC  burati  /main/mb_mothra5/1]
 *
 * 	Alias ACL fix
 * 	[1995/12/08  17:21:10  root]
 *
 * Revision 1.1.18.1  1996/05/10  13:22:30  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 * 
 * 	HP revision /main/DCE_1.2/2  1996/03/27  17:47 UTC  hanfei
 * 	merge in global group work for DCE 1.2.2
 * 
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/4  1996/03/15  15:45 UTC  hanfei
 * 	add rs_prop_pgo_add_member_global()
 * 
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/3  1996/02/29  18:53 UTC  hanfei
 * 	add cell_uuid and princ_uuid for log and prop data since
 * 	add_member need this information when creating foreign principal
 * 	locally.
 * 
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/2  1996/02/20  15:15 UTC  hanfei
 * 	rs_prop_pgo_add_member() : when in a replica, an add member operation
 * 	need to get cell and princ uuids for foreign principal before rsdb call.
 * 
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/1  1996/02/01  16:30 UTC  hanfei
 * 	multi-cell group work: add NULL_PTR as cell_uuid to rs_util_pgo_add() call.
 * 
 * 	HP revision /main/DCE_1.2/1  1996/01/03  19:17 UTC  psn
 * 	Merge ACL alias fix (CHFts15622) from mb_mothra5
 * 	[1995/07/13  21:37 UTC  burati  /main/HPDCE02/1]
 * 
 * 	Alias ACL fix
 * 	[1995/07/13  20:48 UTC  burati  /main/mb_mothra5/1]
 * 
 * Revision 1.1.11.2  1994/07/15  15:01:56  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:20:44  mdf]
 * 
 * Revision 1.1.11.1  1994/06/02  21:22:56  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:19:21  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 	[1994/05/18  11:56:27  mdf]
 * 
 * 	HP revision /main/GAMERA/KK/ODESSA_2/1  1994/05/03  21:15 UTC  hanfei
 * 	merge
 * 
 * 	HP revision /main/GAMERA/KK/hanfei_ch_master/1  1994/05/03  20:41 UTC  hanfei
 * 	label change master work
 * 
 * Revision 1.1.5.3  1993/09/02  20:48:18  hanfei
 * 	[OT 8397] fix incorrect replicated pgo scope name.
 * 	[1993/09/02  20:47:55  hanfei]
 * 
 * Revision 1.1.5.2  1993/07/08  12:49:24  root
 * 	Initial King Kong branch
 * 	[1993/07/08  12:48:53  root]
 * 
 * 	bl6c: add chk_update calls
 * 	[1992/12/11  21:45:21  emartin]
 * 
 * Revision 1.1.3.2  1993/05/25  15:52:33  emartin
 * 	CHFts07478  rs_prop_pgo_delete invoke rs_util_pgo_delete.
 * 	[1993/05/20  20:21:07  emartin]
 * 
 * Revision 1.1.1.6  1993/05/04  20:39:18  mgm
 * 	    Loading drop DCE1_0_2FNL
 * 
 * Revision 1.1.3.5  1993/03/19  22:21:03  emartin
 * 	CR 7472  rs_prop_pgo_rename - rsdb_acct_store_default_pepper
 * 	[1993/03/19  18:58:06  emartin]
 * 
 * Revision 1.1.3.4  1992/12/29  16:36:22  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:25:20  zeliff]
 * 
 * Revision 1.1.3.3  1992/12/18  22:12:25  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:52:54  sekhar]
 * 
 * Revision 1.1.3.2  1992/10/07  20:35:35  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:18:17  sekhar]
 * 
 * Revision 1.1.1.2  1992/10/06  17:22:52  burati
 * 	Second replication drop: bl5
 * 
 * $EndLog$
 */

/*
 * Copyright (c) Hewlett-Packard Company 1992, 1995
 * Unpublished work. All Rights Reserved.
 */
/*
 *     Registry Propagation server manager routines - PGO items
 */
    
#include <rs_server.h>
#include <rs_pvt.h>
#include <rs_ch_lock.h>
#include <rs_prop_pgo.h>
#include <rs_log_pgo.h>
#include <rs_util.h>
#include <rs_master.h>
#include <rs_rep_util.h>
#include <rs_rep_auth.h>
#include <rs_rep_mgmt.h>
#include <rsdb_pgo.h>
#include <rsdb_util.h>

/*
 * rs_prop_pgo_add
 */
PUBLIC  void  rs_prop_pgo_add
  (
    handle_t                  h,              /* [in] */
    sec_rgy_domain_t          domain,         /* [in] */
    unsigned32                num_pgo_items,  /* [in] */
    rs_prop_pgo_add_data_t    pgo_items[],    /* [in, size_is(num_pgo_items)] */
    rs_replica_master_info_t  * master_info,  /* [in, ref] */
    boolean32                 propq_only,     /* [in] */
    error_status_t            * status
  )
{
    error_status_t            xst;
    int                       i;
    boolean32                 apply_update, dummy;

    CLEAR_STATUS(status);

    rs_rep_auth_check(h, status);
    if (BAD_STATUS(status)) {
        return;
    }

    if (!propq_only) {

        BEGIN_SLAVE_WRITE_EVENT(rs_c_event_slave_write, lock_db, master_info, 
            status) {
            rs_rep_mgmt_slave_chk_update(master_info, 
                                         &apply_update, status);
            if (apply_update) {
                for (i = 0; i < num_pgo_items && GOOD_STATUS(status); 
                     i++) {
                    rs_util_pgo_add(domain, pgo_items[i].name, 
                        &pgo_items[i].item, NULL_PTR, 
			&pgo_items[i].client, false, &dummy, status);
                    if (GOOD_STATUS(status) &&
                        (rs_state.rep_state != rs_c_state_initializing)) {
                        rsdb_util_quota_decrement(
                                &pgo_items[i].client.principal,
                                &pgo_items[i].client.cell, &xst);
                        rs_log_pgo_add(domain, pgo_items[i].name, 
                                &pgo_items[i].item,
                                &pgo_items[i].client, master_info); 
                    }
                }
            }
        } END_SLAVE_WRITE_EVENT;
    } else {
        /* just add updates to propq
         */
        BEGIN_RIW_EVENT(rs_c_event_propq_write, lock_db, status) { 
            rs_rep_mgmt_slave_chk_propq_upd(master_info, 
                                            &apply_update, status);
            if (apply_update) {
                if (GOOD_STATUS(status)) {
                    for (i = 0; i < num_pgo_items; i++) {
                        rs_log_pgo_add(domain, pgo_items[i].name, 
                                   &pgo_items[i].item,
                                   &pgo_items[i].client, master_info); 
                    }
                }
            }
        } END_RIW_EVENT;
    }
}


/*
 * rs_prop_pgo_delete 
 */
PUBLIC  void  rs_prop_pgo_delete
  (
    handle_t                  h,              /* [in] */
    sec_rgy_domain_t          domain,         /* [in] */
    sec_rgy_name_t            name,           /* [in, ref] */
    sec_timeval_sec_t         cache_info,     /* [in] */
    rs_replica_master_info_t  * master_info,  /* [in, ref] */
    boolean32                 propq_only,     /* [in] */
    error_status_t            * status
  )
{
    boolean32                 apply_update;

    CLEAR_STATUS(status);

    rs_rep_auth_check(h, status);
    if (BAD_STATUS(status)) {
        return;
    }
    if (!propq_only) {
        BEGIN_SLAVE_WRITE_EVENT(rs_c_event_slave_write, lock_db, master_info, 
            status) {
            rs_rep_mgmt_slave_chk_update(master_info, 
                                         &apply_update, status);
            if (apply_update) {
                rs_util_pgo_delete(domain, name, status);
                if (STATUS_OK(status)) { 
                    rsdb_set_domain_cache_state(domain, &cache_info);
                    rs_log_pgo_delete(domain, name, master_info);
                }
            }
        } END_SLAVE_WRITE_EVENT;
    } else {
        /* just add updates to propq
         */
        BEGIN_RIW_EVENT(rs_c_event_propq_write, lock_db, status) { 
            rs_rep_mgmt_slave_chk_propq_upd(master_info, 
                                            &apply_update, status);
            if (apply_update) {
                if (GOOD_STATUS(status)) {
                    rs_log_pgo_delete(domain, name, master_info);
                }
            }
        } END_RIW_EVENT;
    }
}


/*
 * rs_prop_pgo_rename 
 */
PUBLIC  void  rs_prop_pgo_rename
  (
    handle_t                   h,              /* [in] */
    sec_rgy_domain_t           domain,         /* [in] */
    sec_rgy_name_t             old_name,       /* [in, ref] */
    sec_rgy_name_t             new_name,       /* [in, ref] */
    sec_timeval_sec_t          cache_info,     /* [in] */
    rs_replica_master_info_t   * master_info,  /* [in, ref] */
    boolean32                  propq_only,     /* [in] */
    error_status_t             * status
  )
{
    boolean32                   apply_update;
    sec_rgy_name_t          	old_par_name;
    sec_rgy_name_t          	new_par_name;
    sec_rgy_name_t              new_leaf;
    long                    	new_leaf_len;
    long                    	new_par_len;
    CLEAR_STATUS(status);

    rs_rep_auth_check(h, status);
    if (BAD_STATUS(status)) {
        return;
    }
    if (!propq_only) {
        BEGIN_SLAVE_WRITE_EVENT(rs_c_event_slave_write, lock_db, master_info, 
            status) {
            rs_rep_mgmt_slave_chk_update(master_info, 
                                         &apply_update, status);
            if (apply_update) {
                if (domain == sec_rgy_domain_person) {
                    rsdb_acct_store_default_pepper(old_name, status);
                }
                if (GOOD_STATUS(status)) { 
        	    new_par_name[0] = '\0';
        	    old_par_name[0] = '\0';
        	    rsdb_name_util_get_parent(old_name, old_par_name);
        	    ustrcpy(new_par_name, new_name);  
        	    new_par_len = ustrlen(new_par_name);
        	    rsdb_name_util_strip_leaf(new_par_name, &new_par_len,
                                  new_leaf, &new_leaf_len);
        	    if( ustrcmp(old_par_name, new_par_name) == 0 ) 
                   	rsdb_pgo_rename(domain, old_name, new_leaf, status);
		    else 
                    	rsdb_pgo_move(domain, old_name, new_name, status);
                }
                if (GOOD_STATUS(status)) { 
                    rsdb_set_domain_cache_state(domain, &cache_info);
                    rs_log_pgo_rename(domain, old_name, 
                                      new_name, master_info);
                }
            }
        } END_SLAVE_WRITE_EVENT;
    } else {
        /* just add updates to propq
         */
        BEGIN_RIW_EVENT(rs_c_event_propq_write, lock_db, status) { 
            rs_rep_mgmt_slave_chk_propq_upd(master_info, 
                                            &apply_update, status);
            if (apply_update) {
                if (GOOD_STATUS(status)) {
                    rs_log_pgo_rename(domain, old_name, 
                                      new_name, master_info);
                }
            }
        } END_RIW_EVENT;
    }
}


/*
 * rs_prop_pgo_replace 
 */
PUBLIC  void  rs_prop_pgo_replace
  (
    handle_t                  h,              /* [in] */
    sec_rgy_domain_t          domain,         /* [in] */
    sec_rgy_name_t            name,           /* [in, ref] */
    sec_rgy_pgo_item_t        * item,         /* [in, ref] */
    sec_timeval_sec_t         cache_info,     /* [in] */
    rs_replica_master_info_t  * master_info,  /* [in, ref] */
    boolean32                 propq_only,     /* [in] */
    error_status_t            * status
  )
{
    boolean32                 apply_update;

    CLEAR_STATUS(status);

    rs_rep_auth_check(h, status);
    if (BAD_STATUS(status)) {
        return;
    }
    if (!propq_only) {
        BEGIN_SLAVE_WRITE_EVENT(rs_c_event_slave_write, lock_db, master_info, 
            status) {
            rs_rep_mgmt_slave_chk_update(master_info, 
                                         &apply_update, status);
            if (apply_update) {
                rsdb_pgo_replace(domain, name, item, status);
                if (STATUS_OK(status)) { 
                    rsdb_set_domain_cache_state(domain, &cache_info);
                    rs_log_pgo_replace(domain, name, item, master_info);
                }
            }
        } END_SLAVE_WRITE_EVENT;
    } else {
        /* just add updates to propq
         */
        BEGIN_RIW_EVENT(rs_c_event_propq_write, lock_db, status) { 
            rs_rep_mgmt_slave_chk_propq_upd(master_info, 
                                            &apply_update, status);
            if (apply_update) {
                if (GOOD_STATUS(status)) {
                    rs_log_pgo_replace(domain, name, item, master_info);
                }
            }
        } END_RIW_EVENT;
    }
}


/*
 * rs_prop_pgo_add_member_global
 */
PUBLIC  void  rs_prop_pgo_add_member_global
    (
    handle_t                      h,
    sec_rgy_domain_t              domain,
    sec_rgy_name_t                go_name,
    unsigned32                    num_members,
    sec_rgy_member_t              members[],
    uuid_p_t			  cell_uuids[],
    uuid_p_t			  princ_uuids[],
    rs_replica_master_info_t      * master_info,
    boolean32                     propq_only,
    error_status_t                * status
    )
{
    int  i;
    boolean32                 apply_update;
    error_status_t            lst;

    CLEAR_STATUS(status);

    rs_rep_auth_check(h, status);
    if (BAD_STATUS(status)) {
        return;
    }

    if (!propq_only) {

        BEGIN_SLAVE_WRITE_EVENT(rs_c_event_slave_write, lock_db, master_info, 
            status) {
            rs_rep_mgmt_slave_chk_update(master_info, 
                                         &apply_update, status);
            if (apply_update) {
                for (i = 0; i < num_members && GOOD_STATUS(status);
                     i++) {
		    if ( cell_uuids[i] != NULL_PTR &&
			 GOOD_STATUS(status) ) {
			/* member is a foreign principal */
		        rs_util_pgo_add_foreign_principal(members[i],
				cell_uuids[i], princ_uuids[i], status);
		    }
                    if(GOOD_STATUS(status)) {
                    	rsdb_pgo_add_member(domain, go_name, 
                                        members[i], status);
		    }
                    if(GOOD_STATUS(status)) {
                        rs_log_pgo_add_member(domain, go_name, 
                                members[i], cell_uuids[i], princ_uuids[i],
				master_info);
                    }
                }
            }
        } END_SLAVE_WRITE_EVENT;
    } else {
        /* just add updates to propq
         */
        BEGIN_RIW_EVENT(rs_c_event_propq_write, lock_db, status) { 
            rs_rep_mgmt_slave_chk_propq_upd(master_info, 
                                            &apply_update, status);
            if (apply_update) {
                if (GOOD_STATUS(status)) {
                    for (i = 0; 
                         i < num_members && GOOD_STATUS(status); 
                         i++) {
                        
                        rs_log_pgo_add_member(domain, go_name, 
                                members[i], cell_uuids[i], princ_uuids[i],
				master_info);
                    }
                }
            }
        } END_RIW_EVENT;
    }
}


/*
 * rs_prop_pgo_add_member
 */
PUBLIC  void  rs_prop_pgo_add_member
    (
    handle_t                      h,
    sec_rgy_domain_t              domain,
    sec_rgy_name_t                go_name,
    unsigned32                    num_members,
    sec_rgy_member_t              members[],
    rs_replica_master_info_t      * master_info,
    boolean32                     propq_only,
    error_status_t                * status
    )
{
    int  i;
    boolean32                 apply_update;

    CLEAR_STATUS(status);

    rs_rep_auth_check(h, status);
    if (BAD_STATUS(status)) {
        return;
    }

    if (!propq_only) {

        BEGIN_SLAVE_WRITE_EVENT(rs_c_event_slave_write, lock_db, master_info, 
            status) {
            rs_rep_mgmt_slave_chk_update(master_info, 
                                         &apply_update, status);
            if (apply_update) {
                for (i = 0; i < num_members && GOOD_STATUS(status);
                     i++) {
                    rsdb_pgo_add_member(domain, go_name, 
                                        members[i], status);
                    if(GOOD_STATUS(status)) {
                        rs_log_pgo_add_member(domain, go_name, 
                                members[i], NULL_PTR, NULL_PTR,
				master_info);
                    }
                }
            }
        } END_SLAVE_WRITE_EVENT;
    } else {
        /* just add updates to propq
         */
        BEGIN_RIW_EVENT(rs_c_event_propq_write, lock_db, status) { 
            rs_rep_mgmt_slave_chk_propq_upd(master_info, 
                                            &apply_update, status);
            if (apply_update) {
                if (GOOD_STATUS(status)) {
                    for (i = 0; 
                         i < num_members && GOOD_STATUS(status); 
                         i++) {
                        
                        rs_log_pgo_add_member(domain, go_name, 
                                members[i], NULL_PTR, NULL_PTR,
				master_info);
                    }
                }
            }
        } END_RIW_EVENT;
    }
}


/*
 * rs_prop_pgo_delete_member
 */
PUBLIC  void  rs_prop_pgo_delete_member
    (
    handle_t                  h,
    sec_rgy_domain_t          domain,
    sec_rgy_name_t            go_name,
    sec_rgy_name_t            person_name,
    rs_replica_master_info_t  * master_info,
    boolean32                 propq_only,
    error_status_t            * status
    )
{
    boolean32                 apply_update;

    CLEAR_STATUS(status);

    rs_rep_auth_check(h, status);
    if (BAD_STATUS(status)) {
        return;
    }

    if (!propq_only) {
        BEGIN_SLAVE_WRITE_EVENT(rs_c_event_slave_write, lock_db, master_info, 
            status) {
            rs_rep_mgmt_slave_chk_update(master_info, 
                                         &apply_update, status);
            if (apply_update) {
                rsdb_pgo_delete_member(domain, go_name, 
                                       person_name, status);
                if (GOOD_STATUS(status)) {
                    rs_log_pgo_delete_member(domain, go_name, 
                                      person_name, master_info);
                }
            }
        } END_SLAVE_WRITE_EVENT;
    } else {
        /* just add updates to propq
         */
        BEGIN_RIW_EVENT(rs_c_event_propq_write, lock_db, status) { 
            rs_rep_mgmt_slave_chk_propq_upd(master_info, 
                                            &apply_update, status);
            if (apply_update) {
                if (GOOD_STATUS(status)) {
                    rs_log_pgo_delete_member(domain, go_name, 
                                         person_name, master_info);
                }
            }
        } END_RIW_EVENT;
    }
}







