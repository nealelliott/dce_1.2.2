/* ______________________________________________________________________
 * Copyright (c) 1996 The Open Group, Inc., Cambridge, MA USA
 * All rights reserved.
 * ______________________________________________________________________
 *
 * Queue manager serviceability
 * ______________________________________________________________________
 *
 * $Log: svc.c,v $
 * Revision 1.1  1996/09/27  18:06:22  bowe
 * Initial revision
 *
 * $EndLog$
 * ______________________________________________________________________
 */

#include "mosdefs.h"

#include <dce/dce_svc.h>
#include <dce/svcremote.h>
#include <dce/dcesvcmsg.h>

static dce_svc_stringarray_t	s_nil_svc_stringarray;
static dce_svc_subcomparray_t	s_nil_svc_subcomparray;
static dce_svc_stats_t		s_nil_svc_stats;

/* ______________________________________________________________________ */

static void
mos_svc_set_route(
  /* in */
    handle_t			h,
    idl_char			*where,
  /* out */
    error_status_t		*st
)
{
    *st = svc_s_ok;

    if (!mos_authorized(h, &mgmt_type, &mgmt_acl, mos_perm_manage, st))
	return;
    dce_svc_routing(where, st);
    if (*st != svc_s_ok)
	dce_svc_printf(MOS_S_SVC_FAIL_MSG, "mos_svc_set_route", *st);
}

/* ______________________________________________________________________ */

static void
mos_svc_set_dbg_route(
  /* in */
    handle_t			h,
    idl_char			*where,
  /* out */
    error_status_t		*st
)
{
    *st = svc_s_ok;

    if (!mos_authorized(h, &mgmt_type, &mgmt_acl, mos_perm_manage, st))
	return;
    dce_svc_debug_routing(where, st);
    if (*st != svc_s_ok)
	dce_svc_printf(MOS_S_SVC_FAIL_MSG, "mos_svc_set_dbg_route", *st);
}

/* ______________________________________________________________________ */

static void
mos_svc_set_dbg_levels(
  /* in */
    handle_t			h,
    idl_char			*flags,
  /* out */
    error_status_t		*st
)
{
    *st = svc_s_ok;

    if (!mos_authorized(h, &mgmt_type, &mgmt_acl, mos_perm_manage, st))
	return;
    dce_svc_debug_set_levels(flags, st);
    if (*st != svc_s_ok)
	dce_svc_printf(MOS_S_SVC_FAIL_MSG, "mos_svc_set_dbg_levels", *st);
}

/* ______________________________________________________________________ */

static void
mos_svc_inq_components(
  /* in */
    handle_t			h,
  /* out */
    dce_svc_stringarray_t	*table,
    error_status_t		*st
)
{
    *st = svc_s_ok;
    *table = s_nil_svc_stringarray;
/*
    if (!mos_authorized(h, &mgmt_type, &mgmt_acl, mos_perm_read, st))
	return;
*/
    dce_svc_components(table, st);
    if (*st != svc_s_ok)
	dce_svc_printf(MOS_S_SVC_FAIL_MSG, "mos_svc_inq_components", *st);
}

/* ______________________________________________________________________ */

static void
mos_svc_inq_table(
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
/*
    if (!mos_authorized(h, &mgmt_type, &mgmt_acl, mos_perm_read, st)) {
	table->tab_size = 0;
	return;
    }
*/
    dce_svc_table(comp, table, st);
    if (*st != svc_s_ok)
	dce_svc_printf(MOS_S_SVC_FAIL_MSG, "mos_svc_inq_table", *st);
}

/* ______________________________________________________________________ */

static void
mos_svc_inq_routings(
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
/*
    if (!mos_authorized(h, &mgmt_type, &mgmt_acl, mos_perm_read, st))
	table->tab_size = 0;
	return;
    }
*/
    dce_svc_routings(table, flags, st);
    if (*st != svc_s_ok)
	dce_svc_printf(MOS_S_SVC_FAIL_MSG, "mos_svc_inq_routings", *st);
}

/* ______________________________________________________________________ */

static void
mos_svc_filter_control(
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

    if (!mos_authorized(h, &mgmt_type, &mgmt_acl, mos_perm_manage, st))
	return;
    dce_svc_filter(comp, arg_size, arg, st);
    if (*st != svc_s_ok)
	dce_svc_printf(MOS_S_SVC_FAIL_MSG, "mos_svc_filter_control", *st);
}

/* ______________________________________________________________________ */

static void
mos_svc_inq_stats(
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
	dce_svc_printf(MOS_S_SVC_FAIL_MSG, "mos_svc_filter_control", *st);
  */
  /* XXX - missing ... */
}

/* ______________________________________________________________________ */

serviceability_v1_0_epv_t my_svc_v1_0_epv = {
    mos_svc_set_route,
    mos_svc_set_dbg_route,
    mos_svc_set_dbg_levels,
    mos_svc_inq_components,
    mos_svc_inq_table,
    mos_svc_inq_routings,
    mos_svc_filter_control,
    mos_svc_inq_stats
};

