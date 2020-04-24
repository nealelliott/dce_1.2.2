/*
 *  @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: svc.c,v $
 * Revision 1.1.4.2  1996/02/17  23:35:53  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:52:09  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:14:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:26:17  root]
 * 
 * Revision 1.1.2.3  1994/06/10  20:52:33  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  16:48:56  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/16  18:52:09  rhw
 * 	fix for CR 10642
 * 	[1994/05/16  03:34:45  rhw]
 * 
 * Revision 1.1.2.1  1994/01/21  21:58:23  rsalz
 * 	Fix history section
 * 	[1994/01/21  21:57:46  rsalz]
 * 
 * 	OT CR 9747:  Add SVC and AUD to dtsd
 * 	[1994/01/21  21:07:35  rsalz]
 * 
 * $EndLog$
 */

#include <dce/dce.h>

/* These get defined in a system header file included by <dtss_includes.h> */

#undef MIN
#undef MAX

#include <dce/dcesvcmsg.h>
#include <dts.h>
#include <dtss_includes.h>
#include <dtss_service.h>
#include <dce/svcremote.h>


static void
dts_svc_set_route(
    handle_t		h,
    idl_byte		where[],
    error_status_t	*st
)
{
    StatusBlk_t		statblock;

    if (!mgt_test_write_access(h, &statblock))
	*st = statblock.sysStatus;
    else
	dce_svc_routing(where, st);
}


static void
dts_svc_set_dbg_route(
    handle_t		h,
    idl_byte		where[],
    error_status_t	*st
)
{
    StatusBlk_t		statblock;

    if (!mgt_test_write_access(h, &statblock))
	*st = statblock.sysStatus;
    else
	dce_svc_debug_routing(where, st);
}


static void
dts_svc_set_dbg_levels(
    handle_t		h,
    idl_byte		flags[],
    error_status_t	*st
)
{
    StatusBlk_t		statblock;

    if (!mgt_test_write_access(h, &statblock))
	*st = statblock.sysStatus;
    else
	dce_svc_debug_set_levels(flags, st);
}


static void
dts_svc_inq_components(
    handle_t			h,
    dce_svc_stringarray_t	*table,
    error_status_t		*st
)
{
    StatusBlk_t		statblock;

    if (!mgt_test_read_access(h, &statblock))
	*st = statblock.sysStatus;
    else
	dce_svc_components(table, st);
}


static void
dts_svc_inq_table(
    handle_t			h,
    dce_svc_string_t		component,
    dce_svc_subcomparray_t	*table,
    error_status_t		*st
)
{
    StatusBlk_t		statblock;

    if (!mgt_test_read_access(h, &statblock))
	*st = statblock.sysStatus;
    else
	dce_svc_table(component, table, st);
}


static void
dts_svc_inq_routings(
    handle_t			h,
    boolean32			debug_flags,
    dce_svc_stringarray_t	*table,
    error_status_t		*st
)
{
    StatusBlk_t		statblock;

    if (!mgt_test_read_access(h, &statblock))
	*st = statblock.sysStatus;
    else
	dce_svc_routings(table, debug_flags, st);
}


static void
dts_svc_filter_ctl(
    handle_t			h,
    dce_svc_string_t		component,
    idl_long_int		arg_size,
    idl_byte			*argument,
    error_status_t		*st
)
{
    StatusBlk_t		statblock;

    if (!mgt_test_write_access(h, &statblock))
	*st = statblock.sysStatus;
    else
	dce_svc_filter(component, arg_size, argument, st);
}


static void
dts_svc_inq_stats(
    handle_t		h,
    dce_svc_stats_t	*stats,
    error_status_t	*st
)
{
    *st = svc_s_no_stats;
}


serviceability_v1_0_epv_t dts_svc_ep = {
    dts_svc_set_route,
    dts_svc_set_dbg_route,
    dts_svc_set_dbg_levels,
    dts_svc_inq_components,
    dts_svc_inq_table,
    dts_svc_inq_routings,
    dts_svc_filter_ctl,
    dts_svc_inq_stats
};
