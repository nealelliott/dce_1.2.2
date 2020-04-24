/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_prop_acl.c,v $
 * Revision 1.1.10.3  1996/02/18  00:18:55  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:07:30  marty]
 *
 * Revision 1.1.10.2  1995/12/08  17:53:58  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/15  21:08 UTC  greg
 * 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
 * 	[1995/04/21  18:59 UTC  greg  /main/SEC_migrate_merge/1]
 * 
 * 	Merge work from greg_sec_migrate_acls branch.
 * 	[1995/04/18  18:19 UTC  greg  /main/SEC_migrate/1]
 * 
 * 	Remove acl manager configuration code.  The acl
 * 	manager must be configured at server boot time, and
 * 	is reconfigured when a new version is set, so it must
 * 	already be configured by the time any propagation
 * 	code is executed.
 * 	[1995/04/13  17:10 UTC  greg  /main/greg_sec_migrate_acls/1]
 * 
 * Revision 1.1.6.1  1994/07/15  15:01:52  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:20:33  mdf]
 * 
 * Revision 1.1.3.6  1993/01/26  23:09:28  emartin
 * 	bl6d: fix bug in replace to reinit user_obj and group_obj between calls to get_ids
 * 	[1993/01/07  18:41:57  emartin]
 * 
 * Revision 1.1.3.5  1992/12/29  16:36:19  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:25:16  zeliff]
 * 
 * Revision 1.1.3.4  1992/12/18  22:12:22  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:50:20  sekhar]
 * 
 * Revision 1.1.4.2  1992/12/11  21:44:45  emartin
 * 	bl6c: add chk_update calls
 * 
 * Revision 1.1.3.3  1992/11/04  19:37:50  ahop
 * 	Third replication drop: bl6a
 * 	[1992/11/03  18:40:59  ahop]
 * 
 * Revision 1.1.3.2  1992/10/07  20:35:25  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:18:12  sekhar]
 * 
 * Revision 1.1.1.2  1992/10/06  17:22:30  burati
 * 	Second replication drop: bl5
 * 
 * $EndLog$
 */

/*
 * Copyright (c) Hewlett-Packard Company 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 *      Registry Propagation server manager routines - ACLs
 */
    
#include <dce/sec_daclmgr.h>
#include <rs_server.h>
#include <rs_log_acl.h>
#include <rs_prop_acl.h>
#include <rs_master.h>
#include <rs_rep_auth.h>
#include <rs_rep_mgmt.h>
#include <rs_ch_lock.h>
#include <rsdb_acl.h>

/*
 * rs_prop_acl_replace 
 */
PUBLIC  void  rs_prop_acl_replace
#ifndef __STDC__
    ( h, num_acls, acls, master_info, propq_only, st ) 
    handle_t                  h;              /* [in] */
    unsigned32                num_acls;       /* [in] */
    rs_prop_acl_data_t        acls[];         /* [in, size_is(num_acls)] */
    rs_replica_master_info_t  * master_info;  /* [in, ref] */
    boolean32                 propq_only;     /* [in] */
    error_status_t            * st;           /* [out] */
#else
  (
    handle_t                  h,            
    unsigned32                num_acls,     
    rs_prop_acl_data_t        acls[],       
    rs_replica_master_info_t  * master_info,
    boolean32                 propq_only,   
    error_status_t            * st
  )
#endif
{
    unsigned_char_t             *p;
    sec_id_t                    user_obj, group_obj, *user_p, *group_p;
    rsdb_acl_ids_t              key_ids;
    unsigned32                  i;
    boolean32                   apply_update;

    CLEAR_STATUS(st);

    rs_rep_auth_check(h, st);
    if (BAD_STATUS(st)) {
        return;
    }
   
    if (! propq_only) {
        BEGIN_SLAVE_WRITE_EVENT(rs_c_event_slave_write, lock_db, master_info,
            st) { 

            rs_rep_mgmt_slave_chk_update(master_info, &apply_update, st);
            if (apply_update) {
                for(i = 0; i < num_acls && GOOD_STATUS(st); i++) {
                    user_p = &user_obj; group_p = &group_obj;
                    rsdb_acl_get_ids_from_name(acls[i].component_name,
                                           acls[i].acl_type,
                                           &acls[i].manager_type,
                                           &user_p, &group_p,
                                           &key_ids, st);
                    if (GOOD_STATUS(st)) {
                        sec_acl_mgr_replace((sec_acl_mgr_handle_t) NULL,
                                        (sec_acl_key_t)&key_ids,
                                        &acls[i].manager_type,
                                        acls[i].acl_type,
                                        acls[i].acl_list, st);
                    }
                    if(GOOD_STATUS(st) &&
                       (rs_state.rep_state != rs_c_state_initializing)) {
                        rs_log_acl_replace((sec_acl_mgr_handle_t) NULL,
                                           acls[i].component_name,
                                           &acls[i].manager_type,
                                           acls[i].acl_type,
                                           acls[i].acl_list, 
                                           master_info);
                    }
                }
            }
        } END_SLAVE_WRITE_EVENT;
    }
    else {
        /* just add updates to propq
         */
        BEGIN_RIW_EVENT(rs_c_event_propq_write, lock_db, st) { 
            rs_rep_mgmt_slave_chk_propq_upd(master_info, 
                                            &apply_update, st);
            if (apply_update) {
                if (GOOD_STATUS(st)) {
                    for (i = 0; i < num_acls; i++) {
                        rs_log_acl_replace((sec_acl_mgr_handle_t) NULL,
                                       acls[i].component_name,
                                       &acls[i].manager_type,
                                       acls[i].acl_type,
                                       acls[i].acl_list, 
                                       master_info);
                    }
                }
            }
        } END_RIW_EVENT;
    }
}

