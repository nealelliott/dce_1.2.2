/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_log_sw_version.h,v $
 * Revision 1.1.4.3  1996/02/18  22:58:56  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:17:38  marty]
 *
 * Revision 1.1.4.2  1995/12/08  17:27:24  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/15  21:35 UTC  greg
 * 	security migration merge
 * 	[1995/12/08  17:21:36  root]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1995
 * Unpublished work. All Rights Reserved.
 *
 */

#ifndef rs_log_sw_version__included
#define rs_log_sw_version__included
 
#include <dce/nbase.h>
#include <dce/rgynbase.h>
#include <dce/aclbase.h>
#include <dce/rplbase.h>
#include <dce/daclmgr.h>
#include <dce/rsbase.h>


extern void rs_log_sw_version_init (
    error_status_t   *st
);

extern void rs_log_sw_version_set (
    unsigned32                  old_sw_version,
    unsigned32                  new_sw_version,
    sec_acl_mgr_handle_t        sec_acl_mgr,
    sec_acl_component_name_t    component_name,
    uuid_t                      *manager_type,
    sec_acl_type_t              sec_acl_type,
    sec_acl_list_t              *sec_acl_list,
    rs_replica_master_info_t    *master_info
);

#endif
