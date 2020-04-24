/* ______________________________________________________________________ */
/*
 * @OSF_COPYRIGHT@
 * Copyright (c) 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dced_svc.c,v $
 * Revision 1.1.4.2  1996/02/18  19:16:26  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:02:24  marty]
 *
 * Revision 1.1.4.1  1995/12/08  20:53:46  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/09/29  22:39 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/tatsu_s.dced_local.b0/1  1995/09/19  15:31 UTC  tatsu_s
 * 	Initial revision: dced splitting (libdcedpvt).
 * 	[1995/12/08  20:49:08  root]
 * 
 * Revision 1.1.2.2  1994/09/22  22:00:30  bowe
 * 	Add some error reporting. [CR 11725]
 * 	[1994/09/22  21:42:40  bowe]
 * 
 * Revision 1.1.2.1  1994/08/26  20:15:12  bowe
 * 	Initial implementation of remote svc access. [CR 11725]
 * 	[1994/08/26  20:08:00  bowe]
 * 
 * $EndLog$
 */
/* ______________________________________________________________________ */

#include <dcedimpl.h>
#include <dce/svcremote.h>
#include <dce/dcesvcmsg.h>

static dce_svc_stringarray_t	s_nil_svc_stringarray;
static dce_svc_subcomparray_t	s_nil_svc_subcomparray;
static dce_svc_stats_t		s_nil_svc_stats;

/* ______________________________________________________________________ */

static void
dced_svc_set_route(
  /* in */
    handle_t			h,
    idl_char			*where,
  /* out */
    error_status_t		*st
)
{
    *st = svc_s_ok;

    if (!dced_authorized(h, &dced_config_type, &dced_config_acl, NULL,
		dced_perm_write, st))
	return;
    dce_svc_routing(where, st);
    if (*st != svc_s_ok)
	dce_svc_printf(DCED_S_SVC_FAIL_MSG, "dced_svc_set_route", *st);
}

/* ______________________________________________________________________ */

static void
dced_svc_set_dbg_route(
  /* in */
    handle_t			h,
    idl_char			*where,
  /* out */
    error_status_t		*st
)
{
    *st = svc_s_ok;

    if (!dced_authorized(h, &dced_config_type, &dced_config_acl, NULL,
		dced_perm_write, st))
	return;
    dce_svc_debug_routing(where, st);
    if (*st != svc_s_ok)
	dce_svc_printf(DCED_S_SVC_FAIL_MSG, "dced_svc_set_dbg_route", *st);
}

/* ______________________________________________________________________ */

static void
dced_svc_set_dbg_levels(
  /* in */
    handle_t			h,
    idl_char			*flags,
  /* out */
    error_status_t		*st
)
{
    *st = svc_s_ok;

    if (!dced_authorized(h, &dced_config_type, &dced_config_acl, NULL,
		dced_perm_write, st))
	return;
    dce_svc_debug_set_levels(flags, st);
    if (*st != svc_s_ok)
	dce_svc_printf(DCED_S_SVC_FAIL_MSG, "dced_svc_set_dbg_levels", *st);
}

/* ______________________________________________________________________ */

static void
dced_svc_inq_components(
  /* in */
    handle_t			h,
  /* out */
    dce_svc_stringarray_t	*table,
    error_status_t		*st
)
{
    *st = svc_s_ok;
    *table = s_nil_svc_stringarray;
    if (!dced_authorized(h, &dced_config_type, &dced_config_acl, NULL,
		dced_perm_read, st))
	return;
    dce_svc_components(table, st);
    if (*st != svc_s_ok)
	dce_svc_printf(DCED_S_SVC_FAIL_MSG, "dced_svc_inq_components", *st);
}

/* ______________________________________________________________________ */

static void
dced_svc_inq_table(
  /* in */
    handle_t			h,
    dce_svc_string_t		comp,
  /* out */
    dce_svc_subcomparray_t	*table,
    error_status_t		*st
)
{
    *st = svc_s_ok;
    *table = s_nil_svc_subcomparray;
    if (!dced_authorized(h, &dced_config_type, &dced_config_acl, NULL,
		dced_perm_read, st))
	return;
    dce_svc_table(comp, table, st);
    if (*st != svc_s_ok)
	dce_svc_printf(DCED_S_SVC_FAIL_MSG, "dced_svc_inq_table", *st);
}

/* ______________________________________________________________________ */

static void
dced_svc_inq_routings(
  /* in */
    handle_t			h,
    boolean32			flags,
  /* out */
    dce_svc_stringarray_t	*table,
    error_status_t		*st
)
{
    *st = svc_s_ok;
    *table = s_nil_svc_stringarray;
    if (!dced_authorized(h, &dced_config_type, &dced_config_acl, NULL,
		dced_perm_read, st))
	return;
    dce_svc_routings(table, flags, st);
    if (*st != svc_s_ok)
	dce_svc_printf(DCED_S_SVC_FAIL_MSG, "dced_svc_inq_routings", *st);
}

/* ______________________________________________________________________ */

static void
dced_svc_filter_control(
  /* in */
    handle_t			h,
    dce_svc_string_t		comp,
    idl_long_int		arg_size,
    idl_byte			*arg,
  /* out */
    error_status_t		*st
)
{
    *st = svc_s_ok;

    if (!dced_authorized(h, &dced_config_type, &dced_config_acl, NULL,
		dced_perm_write, st))
	return;
    dce_svc_filter(comp, arg_size, arg, st);
    if (*st != svc_s_ok)
	dce_svc_printf(DCED_S_SVC_FAIL_MSG, "dced_svc_filter_control", *st);
}

/* ______________________________________________________________________ */

static void
dced_svc_inq_stats(
  /* in */
    handle_t			h,
  /* out */
    dce_svc_stats_t		*stats,
    error_status_t		*st
)
{
    *stats = s_nil_svc_stats;
    *st = svc_s_no_stats;
  /*
    if (*st != svc_s_ok)
	dce_svc_printf(DCED_S_SVC_FAIL_MSG, "dced_svc_filter_control", *st);
  */
}

/* ______________________________________________________________________ */

serviceability_v1_0_epv_t dced_svc_v1_0_epv = {
    dced_svc_set_route,
    dced_svc_set_dbg_route,
    dced_svc_set_dbg_levels,
    dced_svc_inq_components,
    dced_svc_inq_table,
    dced_svc_inq_routings,
    dced_svc_filter_control,
    dced_svc_inq_stats
};

