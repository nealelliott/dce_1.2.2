/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_log_policy.h,v $
 * Revision 1.1.6.2  1996/02/18  22:58:51  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:17:35  marty]
 *
 * Revision 1.1.6.1  1995/12/13  16:25:19  root
 * 	Submit
 * 	[1995/12/11  15:14:46  root]
 * 
 * Revision 1.1.4.1  1994/07/15  14:59:23  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:16:38  mdf]
 * 
 * Revision 1.1.2.2  1992/12/29  13:08:10  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:14:54  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:42:06  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rs_log_policy.h V=8 04/29/91 //littl/prgy/src/server/rs
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 *      Registry Server - Policy/Properties info Log operations
 * 
 */

#ifndef rs_log_policy_h__included
#define rs_log_policy_h__included

#include <dce/nbase.h>
#include <dce/rgynbase.h>
#include <dce/rplbase.h>
#include <dce/rsbase.h>


void rs_log_policy_init ( 
#ifdef __STDC__
    error_status_t   *st
#endif
);

void rs_log_policy_props ( 
#ifdef __STDC__
    sec_rgy_properties_t       *(properties),
    rs_replica_master_info_t   *master_info
#endif
);


void rs_log_policy_policy ( 
#ifdef __STDC__
    sec_rgy_name_t             org_name,
    sec_rgy_plcy_t             *policy,
    rs_replica_master_info_t   *master_info
#endif
);

void rs_log_policy_auth_policy ( 
#ifdef __STDC__
    sec_rgy_login_name_t       *account,
    sec_rgy_plcy_auth_t        *auth_policy,
    rs_replica_master_info_t   *master_info
#endif
);

void rs_log_policy_override ( 
#ifdef __STDC__
    sec_rgy_name_t                 name,
    rs_policy_override_flags_t     p_d,
    rs_replica_master_info_t       *master_info
#endif
);


#endif /* rs_log_policy_h__included */
