/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rrs_prop_plcy.h,v $
 * Revision 1.1.6.2  1996/02/18  22:58:22  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:17:14  marty]
 *
 * Revision 1.1.6.1  1995/12/13  16:24:49  root
 * 	Submit
 * 	[1995/12/11  15:14:30  root]
 * 
 * Revision 1.1.3.4  1992/12/29  13:07:19  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:43:42  zeliff]
 * 
 * Revision 1.1.3.3  1992/12/18  22:10:01  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:45:17  sekhar]
 * 
 * Revision 1.1.4.2  1992/12/11  21:04:29  emartin
 * 	bl6c: replication initialization updates
 * 
 * Revision 1.1.3.2  1992/10/07  20:24:55  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:10:48  sekhar]
 * 
 * Revision 1.1.1.2  1992/10/06  15:32:21  burati
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
#ifndef __rrs_prop_plcy_h__included__
#define __rrs_prop_plcy_h__included__

#include <dce/rgynbase.h>
#include <dce/rplbase.h>
#include <rs_rep_auth.h>
#include <rs_rep_pvt.h>

void rrs_prop_policy_all ( 
#ifdef __STDC__
    rs_rep_binding_t           *rep_binding,   /* [in] */
    rs_replica_master_info_t   *master_info,   /* [in] */
    error_status_t             *status
#endif
);


/*
 * rrs_prop_properties_set_info 
 */
void  rrs_prop_properties_set_info (
#ifdef __STDC__
    handle_t                  h,
    sec_rgy_properties_t      * properties,
    rs_replica_master_info_t  * master_info,
    boolean32                 propq_only,
    error_status_t            *status
#endif
);

/*
 * rrs_prop_plcy_set_info 
 */
void  rrs_prop_plcy_set_info (
#ifdef __STDC__
    handle_t                  h,
    sec_rgy_name_t            organization,
    sec_rgy_plcy_t            * policy_data,
    rs_replica_master_info_t  * master_info,
    boolean32                 propq_only,
    error_status_t            *status
#endif
);


/*
 * rrs_prop_auth_plcy_set_info  
 */
void  rrs_prop_auth_plcy_set_info (
#ifdef __STDC__
    handle_t                  h,
    sec_rgy_login_name_t      * account,
    sec_rgy_plcy_auth_t       * auth_policy,
    rs_replica_master_info_t  * master_info,
    boolean32                 propq_only,
    error_status_t            * status
#endif
);


#endif /*  __rrs_prop_plcy_h__included__ */
