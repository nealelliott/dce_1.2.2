/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rrs_prop_login_reset.c,v $
 * Revision 1.1.7.3  1996/02/18  00:16:54  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:05:52  marty]
 *
 * Revision 1.1.7.2  1995/12/08  17:48:13  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:19:36  root]
 * 
 * Revision 1.1.2.2  1994/10/27  20:07:08  rrizzo
 * 	expand copyright
 * 	[1994/10/27  19:39:24  rrizzo]
 * 
 * Revision 1.1.2.1  1994/10/26  20:00:49  hondo
 * 	changes for replication of login activity.
 * 	[1994/10/24  20:06:12  hondo]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1994
 * Unpublished work. All Rights Reserved.
 */
/*
 *      Registry Server Propagation Client - Login Activity
 */
    
#include <rs_server.h>
#include <rs_util.h>
#include <rs_prop_login_reset.h> 
                          

/* the maximum number of login_activity records that 
 * can be propagated in a single call 
 */
#define RS_PROP_LOGIN_INIT_BULK_MAX 10
#define RS_PROP_LOGIN_BULK_MAX  15 


/*
 * rrs_prop_login_reset 
 * 
 */
PUBLIC  void rrs_prop_login_reset
  (
    handle_t                  h,              /* [in] */
    sec_rgy_login_name_t      * login_name,   /* [in] */
    sec_rgy_login_activity_t  * login_part,   /* [in] */
    uuid_t                    * client,       /* [in] */
    sec_timeval_sec_t         * master_time,  /* [in, ref] */
    rs_replica_master_info_t  * master_info,  /* [in, ref] */
    boolean32                 propq_only,     /* [in] */
    error_status_t            * st   
  )
{
    (*rs_prop_login_v1_0_c_epv.rs_prop_login_reset)(h, login_name, 
                login_part, client, master_time, master_info,
                propq_only, st);
}

