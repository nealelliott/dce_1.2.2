/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rrs_prop_plcy.c,v $
 * Revision 1.1.6.2  1996/02/18  00:16:58  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:05:58  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:48:26  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:19:38  root]
 * 
 * Revision 1.1.3.5  1993/03/29  22:19:51  emartin
 * 	CR 7593.  rrs_prop_policy_all - send cache_info for all
 * 	domains.
 * 	[1993/03/25  21:23:18  emartin]
 * 
 * Revision 1.1.3.4  1992/12/29  16:33:51  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:21:53  zeliff]
 * 
 * Revision 1.1.3.3  1992/12/18  22:11:20  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:48:02  sekhar]
 * 
 * Revision 1.1.4.2  1992/12/11  21:38:12  emartin
 * 	bl6c: replication initialization updates
 * 
 * Revision 1.1.3.2  1992/10/07  20:30:38  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:14:34  sekhar]
 * 
 * Revision 1.1.1.2  1992/10/06  15:54:50  burati
 * 	 Second replication code drop: bl5
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 *      Registry Server Propagation Client - Properties and Policies
 */

#include <rs_server.h>
#include <rs_prop_plcy.h>
#include <rrs_prop_util.h>
#include <rrs_prop_plcy.h>
#include <rsdb_policy.h>

/*
 * declare PRIVATE routines
 */
PRIVATE  void  rrs_prop_plcy_set_dom_cache_info (
#ifdef __STDC__
    handle_t                  h,              /* [in] */
    rs_cache_data_t           * cache_info,   /* [in, ref] */
    rs_replica_master_info_t  * master_info,  /* [in, ref] */
    boolean32                 propq_only,     /* [in] */
    error_status_t            *status
#endif
);

PUBLIC void rrs_prop_policy_all
#ifndef __STDC__
    (rep_binding, master_info, status) 
    rs_rep_binding_t           *rep_binding;   /* [in] */
    rs_replica_master_info_t   *master_info;   /* [in] */
    error_status_t             *(status);      /* [out] */
#else
  (
    rs_rep_binding_t           *rep_binding,
    rs_replica_master_info_t   *master_info,
    error_status_t             *(status)
  )
#endif
{
    sec_rgy_properties_t       properties;
    sec_rgy_plcy_t             policy;
    sec_rgy_plcy_auth_t        auth_policy;
    sec_rgy_name_t             rgy_policy_key;
    sec_rgy_login_name_t       rgy_auth_policy_key;
    rs_cache_data_t            domain_cache_info;
    boolean32                  propq_only = false;

    rsdb_properties_get_info(&properties, status);
    if (BAD_STATUS(status)) return;

    master_info->previous_update_seqno = master_info->update_seqno;
    rs_util_seqno_next(&master_info->update_seqno);
    PROP_SETUP_RETRY {
        rrs_prop_properties_set_info(rep_binding->rep_handle, 
                                     &properties, master_info, 
                                     propq_only, status);
        PROP_CHECK_RETRY(rrs_prop_util_retry_handler, 
                         rep_binding, status)
    } PROP_END_RETRY

    if (BAD_STATUS(status)) return;

    /* propagate registry policy information.  organization policy is
     * is handled by pgo item propagation routines
     */
    MAKE_WILD(rgy_policy_key);
    rsdb_policy_get_info(rgy_policy_key, &policy, status);
    if (BAD_STATUS(status)) return;

    master_info->previous_update_seqno = master_info->update_seqno;
    rs_util_seqno_next(&master_info->update_seqno);
    PROP_SETUP_RETRY {
        rrs_prop_plcy_set_info(rep_binding->rep_handle, 
                               rgy_policy_key, &policy,
                               master_info, propq_only, status);
        PROP_CHECK_RETRY(rrs_prop_util_retry_handler, 
                         rep_binding, status)
    } PROP_END_RETRY

    if (BAD_STATUS(status)) return;


    /* propagate registry authentication policy.  account auth policy is
     * is handled by account propagation routines
     */
    CLEAR_LOGIN_NAME(&rgy_auth_policy_key);
    rsdb_auth_policy_get_info(&rgy_auth_policy_key,  &auth_policy, status);
    if (BAD_STATUS(status)) return;

    master_info->previous_update_seqno = master_info->update_seqno;
    rs_util_seqno_next(&master_info->update_seqno);
    PROP_SETUP_RETRY {
        rrs_prop_auth_plcy_set_info (rep_binding->rep_handle, 
                                     &rgy_auth_policy_key, 
                                     &auth_policy,  master_info, 
                                     propq_only, status);
        PROP_CHECK_RETRY(rrs_prop_util_retry_handler, 
                         rep_binding, status)
    } PROP_END_RETRY
                                
    if (BAD_STATUS(status)) return;

    /*
     * Send cache info for all the domains
     */
    rs_util_get_cache_data(&domain_cache_info);

    master_info->previous_update_seqno = master_info->update_seqno;
    rs_util_seqno_next(&master_info->update_seqno);
    PROP_SETUP_RETRY {
        rrs_prop_plcy_set_dom_cache_info(rep_binding->rep_handle, 
                               &domain_cache_info,
                               master_info, propq_only, status);
        PROP_CHECK_RETRY(rrs_prop_util_retry_handler, 
                         rep_binding, status)
    } PROP_END_RETRY

    if (BAD_STATUS(status)) return;

    CLEAR_STATUS(status);
}


/*
 * rrs_prop_properties_set_info 
 */
PUBLIC  void  rrs_prop_properties_set_info
#ifndef __STDC__
   ( h, properties, master_info, propq_only, status )
    handle_t                  h;              /* [in] */
    sec_rgy_properties_t      * properties;   /* [in, ref] */
    rs_replica_master_info_t  * master_info;  /* [in, ref] */
    boolean32                 propq_only;     /* [in] */
    error_status_t            *status;        /* [out] */
#else
  (
    handle_t                  h,              /* [in] */
    sec_rgy_properties_t      * properties,   /* [in, ref] */
    rs_replica_master_info_t  * master_info,  /* [in, ref] */
    boolean32                 propq_only,     /* [in] */
    error_status_t            *status
  )
#endif
{
    (*rs_prop_plcy_v1_1_c_epv.rs_prop_properties_set_info) (h, 
            properties, master_info, propq_only, status);
}

/*
 * rrs_prop_plcy_set_info 
 */
PUBLIC  void  rrs_prop_plcy_set_info
#ifndef __STDC__
    ( h, organization, policy_data, master_info, propq_only, status)
    handle_t                  h;              /* [in] */
    sec_rgy_name_t            organization;   /* [in, ref] */
    sec_rgy_plcy_t            * policy_data;  /* [in, ref] */
    rs_replica_master_info_t  * master_info;  /* [in, ref] */
    boolean32                 propq_only;     /* [in] */
    error_status_t            *status;        /* [out] */
#else
  (
    handle_t                  h,              /* [in] */
    sec_rgy_name_t            organization,   /* [in, ref] */
    sec_rgy_plcy_t            * policy_data,  /* [in, ref] */
    rs_replica_master_info_t  * master_info,  /* [in, ref] */
    boolean32                 propq_only,     /* [in] */
    error_status_t            *status
  )
#endif
{
    (*rs_prop_plcy_v1_1_c_epv.rs_prop_plcy_set_info) (h, 
            organization, policy_data, master_info, 
            propq_only, status);
                                                         
}


/*
 * rrs_prop_auth_plcy_set_info  
 */
PUBLIC   void  rrs_prop_auth_plcy_set_info
#ifndef __STDC__
    ( h, account, auth_policy, master_info, propq_only,status)
    handle_t                  h;              /* [in] */
    sec_rgy_login_name_t      * account;      /* [in, ref] */
    sec_rgy_plcy_auth_t       * auth_policy;  /* [in, ref] */
    rs_replica_master_info_t  * master_info;  /* [in, ref] */
    boolean32                 propq_only;     /* [in] */
    error_status_t            * status;       /* [out] */
#else
  (
    handle_t                  h,              /* [in] */
    sec_rgy_login_name_t      * account,      /* [in, ref] */
    sec_rgy_plcy_auth_t       * auth_policy,  /* [in, ref] */
    rs_replica_master_info_t  * master_info,  /* [in, ref] */
    boolean32                 propq_only,     /* [in] */
    error_status_t            * status
  )
#endif
{
    (*rs_prop_plcy_v1_1_c_epv.rs_prop_auth_plcy_set_info) (h, 
             account,  auth_policy,  master_info,
             propq_only, status);
}

/*
 * rrs_prop_plcy_set_dom_cache_info 
 *
 * used to send cache_info for all the domains during
 * slave initialization.
 */
PRIVATE  void  rrs_prop_plcy_set_dom_cache_info
#ifndef __STDC__
    ( h, cache_info, master_info, propq_only, status)
    handle_t                  h;              /* [in] */
    rs_cache_data_t           * cache_info;   /* [in, ref] */
    rs_replica_master_info_t  * master_info;  /* [in, ref] */
    boolean32                 propq_only;     /* [in] */
    error_status_t            *status;        /* [out] */
#else
  (
    handle_t                  h,              /* [in] */
    rs_cache_data_t           * cache_info,   /* [in, ref] */
    rs_replica_master_info_t  * master_info,  /* [in, ref] */
    boolean32                 propq_only,     /* [in] */
    error_status_t            *status
  )
#endif
{
    (*rs_prop_plcy_v1_1_c_epv.rs_prop_plcy_set_dom_cache_info) (h, 
            cache_info, master_info, 
            propq_only, status);
}



