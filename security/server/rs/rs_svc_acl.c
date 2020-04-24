/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_svc_acl.c,v $
 * Revision 1.1.4.2  1996/02/18  00:20:16  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:08:21  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:56:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:21:42  root]
 * 
 * Revision 1.1.2.2  1994/10/06  20:29:45  agd
 * 	expand copyright
 * 	[1994/10/06  14:27:58  agd]
 * 
 * Revision 1.1.2.1  1994/08/25  21:26:02  sekhar
 * 	Initial version. ACL Manager for Serviceability interface.
 * 	                 [OT 11751 ]
 * 	[1994/08/24  20:06:28  sekhar]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1994
 * Unpublished work. All Rights Reserved.
 */

/*  Serviceability ACL manager 
 *
 *  As per the Serviceability requirements, every server must support
 *  a Remote Serviceability Interface ( referred to SVCIF ) which can
 *  used for e.g. to change or read the routing information.
 *
 *  A server must also support an ACL manager to protect the SVCIF
 *  There are no further requirements on the ACL manager ( the policy
 *  is left to the server ).
 *
 *  So, the SVCIF ACL manager in the security is implemented using
 *  Policy ACL Manager. One new permission bit 's' ( 's' stands for
 *  serviceability ) - has been added for the Policy ACL manager. 
 *  
 *  To retrieve Serviceability information , only the read permssion 
 *  is required. To modify serviceablity information the 's' permission 
 *  bit must be set.
 *
 *  By default, only cell_admin can modify serviceability information.
 *  The default can be changed by changing the Policy acl ( /.:/sec/policy ) .
 * 
 */
 
#include <dce/dce.h>

#include <dce/dcesvcmsg.h>
#include <dce/svcremote.h>
#include <rsdb_acl.h>
#include <rs_auth.h>

static uuid_t         *sec_svc_mgr_type  =  
                      &(rsdb_acl_mgr_uuids[rsdb_acl_mgr_type_rgy]);

static sec_rgy_name_t sec_svc_obj_name   = SEC_POLICY_OBJ_NAME ;

#define  TEST_READ_ACCESS(result,stp)                           \
         {                                                      \
             sec_rgy_foreign_id_t  client_ids ;                 \
                                                                \
             result = rs_is_authorized(h,                       \
                          sec_acl_perm_read,                    \
                          NULL ,  /* no parent_perms reqd */    \
                          sec_svc_obj_name,                     \
                          sec_svc_mgr_type,                     \
                          false,  /* no quota checking reqd */  \
                          &client_ids, stp);                    \
             if (!result && *stp == sec_rgy_not_authorized)     \
                 *stp = sec_svc_not_authorized ;                \
         }


#define  TEST_WRITE_ACCESS(result,stp)                          \
         {                                                      \
             sec_rgy_foreign_id_t  client_ids ;                 \
                                                                \
             result = rs_is_authorized(h,                       \
                          sec_acl_perm_svc,                     \
                          NULL ,  /* no parent_perms reqd */    \
                          sec_svc_obj_name,                     \
                          sec_svc_mgr_type,                     \
                          false,  /* no quota checking reqd */  \
                          &client_ids, stp);                    \
             if (!result && *stp == sec_rgy_not_authorized)     \
                 *stp = sec_svc_not_authorized ;                \
         }

static void
sec_svc_set_route(
    handle_t		h,
    idl_byte		where[],
    error_status_t	*st
)
{
    boolean32 result ;

    TEST_WRITE_ACCESS(result,st)

    if (result) 
        dce_svc_routing(where,st);
}
                        
static void
sec_svc_set_dbg_route(
    handle_t		h,
    idl_byte		where[],
    error_status_t	*st
)
{
    boolean32 result ;

    TEST_WRITE_ACCESS(result,st)

    if (result) 
        dce_svc_debug_routing(where,st);
}


static void
sec_svc_set_dbg_levels(
    handle_t		h,
    idl_byte		flags[],
    error_status_t	*st
)
{
    boolean32 result ;

    TEST_WRITE_ACCESS(result,st)

    if (result) 
	dce_svc_debug_set_levels(flags, st);
}


static void
sec_svc_inq_components(
    handle_t			h,
    dce_svc_stringarray_t	*table,
    error_status_t		*st
)
{
    boolean32 result ;

    TEST_READ_ACCESS(result,st)

    if (result) 
	dce_svc_components(table, st);
}


static void
sec_svc_inq_table(
    handle_t			h,
    dce_svc_string_t		component,
    dce_svc_subcomparray_t	*table,
    error_status_t		*st
)
{
    boolean32 result ;

    TEST_READ_ACCESS(result,st)

    if (result) 
	dce_svc_table(component, table, st);
}


static void
sec_svc_inq_routings(
    handle_t			h,
    boolean32			debug_flags,
    dce_svc_stringarray_t	*table,
    error_status_t		*st
)
{
    boolean32 result ;

    TEST_READ_ACCESS(result,st)

    if (result) 
	dce_svc_routings(table, debug_flags, st);
}


static void
sec_svc_filter_ctl(
    handle_t			h,
    dce_svc_string_t		component,
    idl_long_int		arg_size,
    idl_byte			*argument,
    error_status_t		*st
)
{
    boolean32 result ;

    TEST_WRITE_ACCESS(result,st)

    if (result) 
	dce_svc_filter(component, arg_size, argument, st);
}


static void
sec_svc_inq_stats(
    handle_t		h,
    dce_svc_stats_t	*stats,
    error_status_t	*st
)
{
    *st = svc_s_no_stats;
}


globaldef serviceability_v1_0_epv_t serviceability_v1_0_mgr_epv = {
    sec_svc_set_route,
    sec_svc_set_dbg_route,
    sec_svc_set_dbg_levels,
    sec_svc_inq_components,
    sec_svc_inq_table,
    sec_svc_inq_routings,
    sec_svc_filter_ctl,
    sec_svc_inq_stats
};
