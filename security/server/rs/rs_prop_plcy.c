/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_prop_plcy.c,v $
 * Revision 1.1.8.2  1996/02/18  00:19:06  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:07:36  marty]
 *
 * Revision 1.1.8.1  1995/12/08  17:54:33  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:21:11  root]
 * 
 * Revision 1.1.6.1  1994/07/15  15:01:58  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:20:50  mdf]
 * 
 * Revision 1.1.3.5  1993/03/29  22:20:04  emartin
 * 	CR 7593.  do rs_prop_plcy_set_dom_cache_info
 * 	[1993/03/25  21:25:22  emartin]
 * 
 * Revision 1.1.3.4  1992/12/29  16:36:24  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:25:25  zeliff]
 * 
 * Revision 1.1.3.3  1992/12/18  22:12:28  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:52:58  sekhar]
 * 
 * Revision 1.1.4.2  1992/12/11  21:46:09  emartin
 * 	bl6c: add chk_update calls
 * 
 * Revision 1.1.3.2  1992/10/07  20:35:49  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:18:21  sekhar]
 * 
 * Revision 1.1.1.2  1992/10/06  17:23:13  burati
 * 	Second replication drop: bl5
 * 
 * $EndLog$
 */

/*
 * Copyright (c) Hewlett-Packard Company 1991, 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 *      Registry Propagation server manager routines - 
 *            Policies and properties
 */
    
#include <rs_server.h>
#include <rs_prop_plcy.h>
#include <rs_log_policy.h>
#include <rs_rep_auth.h>    
#include <rs_rep_mgmt.h>    
#include <rs_ch_lock.h>    
#include <rs_rep_util.h>    
#include <rs_master.h>    
#include <rsdb_policy.h>    


/*
 * rs_prop_properties_set_info 
 */
PUBLIC  void  rs_prop_properties_set_info
#ifndef __STDC__
    ( h, properties, master_info, propq_only, status )
    handle_t                  h;              /* [in] */
    sec_rgy_properties_t      * properties;   /* [in, ref] */
    rs_replica_master_info_t  * master_info;  /* [in, ref] */
    boolean32                 propq_only;     /* [in] */
    error_status_t            *status;        /* [out] */
#else
  (
    handle_t                  h,              
    sec_rgy_properties_t      * properties,   
    rs_replica_master_info_t  * master_info,  
    boolean32                 propq_only,     
    error_status_t            *status
  )
#endif
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
                rsdb_properties_set_info(properties, status);
                if (GOOD_STATUS(status)) {
                    rs_log_policy_props(properties, master_info);
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
                    rs_log_policy_props(properties, master_info);
                }
            }
        } END_RIW_EVENT;
    }   
}


/*
 * rs_prop_plcy_set_info 
 */
PUBLIC  void  rs_prop_plcy_set_info
#ifndef __STDC__
    ( h, organization, policy_data, master_info, propq_only, status )
    handle_t                  h;              /* [in] */
    sec_rgy_name_t            organization;   /* [in, ref] */
    sec_rgy_plcy_t            * policy_data;  /* [in, ref] */
    rs_replica_master_info_t  * master_info;  /* [in, ref] */
    boolean32                 propq_only;     /* [in] */
    error_status_t            *status;        /* [out] */
#else
  (
    handle_t                  h,            
    sec_rgy_name_t            organization, 
    sec_rgy_plcy_t            * policy_data,
    rs_replica_master_info_t  * master_info,
    boolean32                 propq_only,   
    error_status_t            *status
  )
#endif
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
                rsdb_policy_set_info(organization, policy_data, status);
                if (GOOD_STATUS(status)) {
                    rs_log_policy_policy(organization, policy_data, 
                                         master_info);
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
                    rs_log_policy_policy(organization, policy_data, 
                                         master_info);
                }
            }
        } END_RIW_EVENT;
    }   
}


/*
 * rs_prop_auth_plcy_set_info  
 */
PUBLIC  void  rs_prop_auth_plcy_set_info
#ifndef __STDC__
    ( h, account, auth_policy, master_info, propq_only, status )
    handle_t                  h;              /* [in] */
    sec_rgy_login_name_t      * account;      /* [in, ref] */
    sec_rgy_plcy_auth_t       * auth_policy;  /* [in, ref] */
    rs_replica_master_info_t  * master_info;  /* [in, ref] */
    boolean32                 propq_only;     /* [in] */
    error_status_t            * status;       /* [out] */
#else
  (
    handle_t                  h,            
    sec_rgy_login_name_t      * account,    
    sec_rgy_plcy_auth_t       * auth_policy,
    rs_replica_master_info_t  * master_info,
    boolean32                 propq_only,   
    error_status_t            * status
  )
#endif
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
                rsdb_auth_policy_set_info(account, auth_policy, status);
                if (GOOD_STATUS(status)) {
                    rs_log_policy_auth_policy(account, auth_policy, 
                                              master_info);
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
                    rs_log_policy_auth_policy(account, auth_policy, 
                                              master_info);
                }
            }
        } END_RIW_EVENT;
    }   
}

/*
 * rs_prop_plcy_set_dom_cache_info 
 *
 * NB:  This operation is only used to send the domain
 * cache_info to initialize a slave.  The update is not
 * logged; the slave will checkpoint its database to
 * disk when initialization completes.
 */
PUBLIC  void  rs_prop_plcy_set_dom_cache_info
#ifndef __STDC__
    ( h, cache_info, master_info, propq_only, status )
    handle_t                  h;              /* [in] */
    rs_cache_data_t           * cache_info;   /* [in, ref] */
    rs_replica_master_info_t  * master_info;  /* [in, ref] */
    boolean32                 propq_only;     /* [in] */
    error_status_t            *status;        /* [out] */
#else
  (
    handle_t                  h,            
    rs_cache_data_t           * cache_info,
    rs_replica_master_info_t  * master_info,
    boolean32                 propq_only,   
    error_status_t            *status
  )
#endif
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
                rsdb_set_domain_cache_state(
                    sec_rgy_domain_person, &cache_info->person_dtm);

                rsdb_set_domain_cache_state(
                    sec_rgy_domain_group, &cache_info->group_dtm);

                rsdb_set_domain_cache_state(
                    sec_rgy_domain_org, &cache_info->org_dtm);
            }

        } END_SLAVE_WRITE_EVENT; 

    } else {
        SET_STATUS(status, sec_s_not_implemented);
    }   
}


