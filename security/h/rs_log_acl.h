/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_log_acl.h,v $
 * Revision 1.1.6.2  1996/02/18  22:58:48  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:17:33  marty]
 *
 * Revision 1.1.6.1  1995/12/13  16:25:15  root
 * 	Submit
 * 	[1995/12/11  15:14:44  root]
 * 
 * Revision 1.1.4.1  1994/07/15  14:59:20  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:16:33  mdf]
 * 
 * Revision 1.1.2.3  1992/12/29  13:08:00  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:44:24  zeliff]
 * 
 * Revision 1.1.2.2  1992/11/04  19:31:48  ahop
 * 	Third replication drop: bl6a
 * 	[1992/11/03  18:09:45  ahop]
 * 
 * Revision 1.1  1992/01/19  14:43:02  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1991, 1992
 * Unpublished work. All Rights Reserved.
 *
 */

#ifndef rs_log_acl__included
#define rs_log_acl__included
 
#include <dce/nbase.h>
#include <dce/rgynbase.h>
#include <dce/aclbase.h>
#include <dce/rplbase.h>
#include <dce/daclmgr.h>
#include <dce/rsbase.h>


void rs_log_acl_init (
#ifdef __STDC__
    error_status_t   *st
#endif
);

void rs_log_acl_replace (
#ifdef __STDC__
    sec_acl_mgr_handle_t        sec_acl_mgr,
    sec_acl_component_name_t    component_name,
    uuid_t                      *manager_type,
    sec_acl_type_t              sec_acl_type,
    sec_acl_list_t              *sec_acl_list,
    rs_replica_master_info_t    *master_info
#endif
);


#endif
