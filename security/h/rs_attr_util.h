/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_attr_util.h,v $
 * Revision 1.1.4.2  1996/02/18  22:58:33  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:17:22  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:27:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:53:26  root]
 * 
 * Revision 1.1.2.5  1994/06/17  18:42:02  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:08:39  devsrc]
 * 
 * Revision 1.1.2.4  1994/06/02  20:50:37  mdf
 * 	Merged with changes from 1.1.2.3
 * 	[1994/06/02  20:50:27  mdf]
 * 
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:14:45  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * Revision 1.1.2.3  1994/06/02  20:21:34  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:14:45  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * Revision 1.1.2.1  1994/03/02  17:54:54  hanfei
 * 	Initial ERA drop
 * 	[1994/02/18  19:08:50  hanfei]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1994
 * Unpublished work. All Rights Reserved.
 */
/*
 * rs_attr_util.h: utilities useful for rs_attr_*.c
 */

#ifndef rs_attr_util_h__included
#define rs_attr_util_h__included

#include <dce/aclbase.h>
#include <dce/sec_attr_base.h>

#include <rs_attr_pvt.h>
#include <rsdb_util.h>
#include <rsdb_attr_pvt.h>
#include <rsdb_acl.h>


void rs_attr_util_map_status
(
    error_status_t      *st_p   /* [in, out] */
);

boolean32 rs_attr_util_is_auth
(
    handle_t                    h,                      /* [in] */
    rsdb_util_rgy_obj_info_t    *rgy_obj_info,          /* [in] */
    sec_acl_permset_t           requested_perms,        /* [in] */
    sec_attr_component_name_t   object_name,            /* [in] */
    error_status_t              *st_p
);

void rs_attr_util_get_permset
(
    rsdb_util_rgy_obj_info_t    *rgy_obj_info,          /* [in] */
    sec_attr_schema_entry_t     *sec_sch_entry_p,       /* [in] */
    rs_attr_operation_t         operation,              /* [in] */
    sec_acl_permset_t           *permset,               /* [out] */
    error_status_t              *st_p
);

void rs_util_attr_pre_update (
    handle_t                    h,
    sec_attr_component_name_t   component_name,         /* [in] */
    unsigned32                  num_attrs,              /* [in] */
    sec_attr_t                  attrs[],                /* [in] */
    rsdb_util_rgy_obj_info_t    *rgy_obj_info,          /* [out] */
    rsdb_attr_sch_list_t        *sch_list,              /* [out] */
    rsdb_attr_sch_attr_xref_t   xref,                   /* [out] */
    signed32                    *failure_index,         /* [out] */
    error_status_t              *st_p
);



#endif /* rs_attr_util_h__included */

