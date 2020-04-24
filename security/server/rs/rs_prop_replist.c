/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_prop_replist.c,v $
 * Revision 1.1.8.2  1996/02/18  00:19:08  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:07:38  marty]
 *
 * Revision 1.1.8.1  1995/12/08  17:54:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:21:12  root]
 * 
 * Revision 1.1.6.2  1994/08/04  16:13:47  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:11:55  mdf]
 * 
 * Revision 1.1.2.6  1993/02/26  23:53:03  emartin
 * 	em_bl6f: rs_prop_replist_delete_replica - call
 * 	   rpc_mgmt_server_stop_listening
 * 	[1993/02/11  20:18:31  emartin]
 * 
 * Revision 1.1.2.5  1992/12/29  16:36:26  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:25:30  zeliff]
 * 
 * Revision 1.1.2.4  1992/12/18  22:12:31  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:53:03  sekhar]
 * 
 * Revision 1.1.4.2  1992/12/11  21:46:44  emartin
 * 	bl6c: use slave_chk_update/propq_update routines
 * 
 * Revision 1.1.2.3  1992/10/07  20:35:52  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:18:26  sekhar]
 * 
 * Revision 1.1.3.2  1992/10/06  17:23:32  burati
 * 	Second replication drop: bl5
 * 
 * Revision 1.1.2.2  1992/08/31  18:25:37  sekhar
 * 	First replication code drop.
 * 	[1992/08/31  17:54:12  sekhar]
 * 
 * $EndLog$
 */

/*
 * Copyright (c) Hewlett-Packard Company 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 *      Registry Propagation - replica list server manager routines.
 */
    
#ifdef MODULE_VERSION_ID
static char *VersionID = 
    "$Id: rs_prop_replist.c,v 1.1.8.2 1996/02/18 00:19:08 marty Exp $";
#endif

#include <dce/dce.h>    
#include <dce/rplbase.h>

#include <rs_prop_replist.h>
#include <rs_base.h>
#include <rs_pvt.h>
#include <rs_lock.h>
#include <rs_ch_lock.h>
#include <rs_master.h>

#include <rs_replist2.h>
#include <rs_log_replist.h>
#include <rs_rep_auth.h>
#include <rs_rep_mgmt.h>

#include <sec_svc.h>

/*
 *  rs_prop_replist_add_replica
 *
 *  Add or replace a replica on the replist.
 *
 *  Slave-only operation
 *
 */
PUBLIC  void  rs_prop_replist_add_replica
#ifndef __STDC__
    ( h, rep_id, rep_name, rep_twrs, master_info, propq_only, st )
    handle_t                    h;            /* [in] */
    uuid_p_t                    rep_id;       /* [in] */
    rs_replica_name_p_t         rep_name;     /* [in] */
    rs_replica_twr_vec_p_t      rep_twrs;     /* [in] */
    rs_replica_master_info_p_t  master_info;  /* [in] */
    boolean32                   propq_only;   /* [in] */
    error_status_t              *st;          /* [out] */
#else
  (
    handle_t                    h,            /* [in] */
    uuid_p_t                    rep_id,       /* [in] */
    rs_replica_name_p_t         rep_name,     /* [in] */
    rs_replica_twr_vec_p_t      rep_twrs,     /* [in] */
    rs_replica_master_info_p_t  master_info,  /* [in] */
    boolean32                   propq_only,   /* [in] */
    error_status_t              *st
  )
#endif
{
    boolean32           apply_update;

    CLEAR_STATUS(st);

    rs_rep_auth_check(h, st);
    if (BAD_STATUS(st)) return; 
   
    if (! propq_only) {
        
        BEGIN_SLAVE_WRITE_EVENT(rs_c_event_slave_write, lock_replist, 
            master_info, st) { 

            rs_rep_mgmt_slave_chk_update(master_info, &apply_update, st);

            if (apply_update) {
                rs_replist2_slave_add_rep(rep_id, rep_name, rep_twrs, st);

                if (GOOD_STATUS(st)) {
                    rs_log_replist_add_replica(rep_id, rep_name, rep_twrs,
                        NULL, master_info);
                }
            }

        } END_SLAVE_WRITE_EVENT;
        
    } 
    else {
        /* just add update to propq
         */
        BEGIN_RIW_EVENT(rs_c_event_propq_write, lock_replist, st) { 

            rs_rep_mgmt_slave_chk_propq_upd(master_info, &apply_update, st);

            if (apply_update) {
                rs_log_replist_add_replica(rep_id, rep_name, rep_twrs,
                    NULL, master_info);
            }

        } END_RIW_EVENT;
    }
}


/*
 *  rs_prop_replist_del_replica
 *
 *  Delete a replica on the replist.
 *  If the replica being deleted is
 *  this one, blow it away.
 *
 *  Slave-only operation
 *
 */
PUBLIC  void  rs_prop_replist_del_replica
#ifndef __STDC__
    ( h, rep_id, master_info, propq_only, st ) 
    handle_t                    h;            /* [in] */
    uuid_p_t                    rep_id;       /* [in] */
    rs_replica_master_info_p_t  master_info;  /* [in] */
    boolean32                   propq_only;   /* [in] */
    error_status_t              *st;          /* [out] */
#else
  (
    handle_t                    h,            /* [in] */
    uuid_p_t                    rep_id,       /* [in] */
    rs_replica_master_info_p_t  master_info,  /* [in] */
    boolean32                   propq_only,   /* [in] */
    error_status_t              *st
  )
#endif
{
    boolean32           apply_update;
    error_status_t      lst;

    CLEAR_STATUS(st);

    rs_rep_auth_check(h, st);
    if (BAD_STATUS(st)) return;
   
    if (uuid_equal(rep_id, &rs_state.rep_id, &lst)) {
        /* this replica is being deleted
         */
        if (propq_only) {
            /****  SVC_OPEN : Turn this into debug 
             * 
             *    error
             *        ("rs_prop_replist_del_replica received delete this replica for propq_only");
             */

            dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                           sec_s_rs_rep, 
                           svc_c_sev_error,
                           sec_rgy_rep_pgmerr);
            SET_STATUS(st, sec_rgy_rep_pgmerr);
            return;
        }

        BEGIN_GLOBAL_RIW_SLAVE_EVENT(rs_c_event_delete, master_info, st) {

            rs_rep_mgmt_slave_chk_master(master_info, st);

            if (GOOD_STATUS(st)) {
    
                /* will promote global lock when changing
                 * replica's state
                 */
                rs_rep_mgmt_delete(st);
            }

            if (GOOD_STATUS(st)) {
                rpc_mgmt_stop_server_listening(NULL, &lst);
            }

        } END_GLOBAL_RIW_SLAVE_EVENT;

        return;
    }

    if (!propq_only) {

        /* Not deleting this replica so just remove "rep_id"
         * from the replist
         */

        BEGIN_SLAVE_WRITE_EVENT(rs_c_event_slave_write, lock_replist, 
            master_info, st) { 

            rs_rep_mgmt_slave_chk_update(master_info, &apply_update, st);

            if (apply_update) {
                rs_replist2_slave_delete_rep(rep_id, st);

                if (GOOD_STATUS(st)) {
                    rs_log_replist_delete_replica(rep_id, false, master_info);
                }
            }

        } END_SLAVE_WRITE_EVENT;
    } 
    else {
        /* just add update to propq
         */
        BEGIN_RIW_EVENT(rs_c_event_propq_write, lock_replist, st) { 

            rs_rep_mgmt_slave_chk_propq_upd(master_info, &apply_update, st);

            if (apply_update) {
                rs_log_replist_delete_replica(rep_id, false, master_info);
            }

        } END_RIW_EVENT;
    }
}


