/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * (c) Copyright International Business Machines Corp. 1994, 1996
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 *
 */
/*
 * HISTORY
 * $Log: dmn_svc.c,v $
 * Revision 1.1.6.1  1996/07/08  18:26:03  arvind
 * 	Audit for 1.2.2
 * 	[1996/06/21  20:24 UTC  arvind  /main/arvind_audit122/1]
 *
 * 	Audit for 1.2.2
 * 	[1996/02/18  00:06:08  marty  1.1.4.2]
 *
 * Revision 1.1.4.2  1996/02/18  00:06:08  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:57:39  marty]
 * 
 * Revision 1.1.4.1  1995/12/08  17:38:33  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:17:37  root]
 * 
 * Revision 1.1.2.2  1994/01/13  14:28:05  zeliff
 * 	OT9733:  argument change to dce_svc_routings
 * 	[1994/01/13  14:13:55  zeliff]
 * 
 * Revision 1.1.2.1  1993/12/22  12:38:56  zeliff
 * 	First version of code
 * 	[1993/12/21  15:01:12  zeliff]
 * 
 * $EndLog$
 */

/*
** dmn_svc.c
** Manager routines for audit daemon's svc network interface.
**
*/

#include <dce/dce.h>
#include <error_statusp.h>
#include <dce/audit.h>
#include <dce/dce_svc.h> 
#include <dce/dcesvcmsg.h>
#include <dce/svcremote.h>

struct serviceability_v1_0_epv_t dce_svc_ep; 
/*
static void aud_svc_set_route();
static void aud_svc_set_dbg_route();
static void aud_svc_set_dbg_levels();
static void aud_svc_inq_components();
static void aud_svc_inq_table();
static void aud_svc_inq_routings();
static void aud_svc_filter_ctl();
static void aud_svc_inq_stats();
 */


static void
aud_svc_set_route(
		  handle_t h,
		  idl_byte where[],
		  error_status_t * st
)
{
    boolean32       result;

    acl_test_write_access(h, &result, st);
    if (BAD_STATUS(st)) {
	return;
    } else if (result == FALSE) {
	*st = -1;		/* XXX */
    } else {
	dce_svc_routing(where, st);
    }
}


static void
aud_svc_set_dbg_route(
		      handle_t h,
		      idl_byte where[],
		      error_status_t * st
)
{
    boolean32       result;

    acl_test_write_access(h, &result, st);
    if (BAD_STATUS(st)) {
	return;
    } else if (result == FALSE) {
	*st = -1;		/* XXX */
    } else {
	dce_svc_debug_routing(where, st);
    }
}


static void
aud_svc_set_dbg_levels(
		       handle_t h,
		       idl_byte flags[],
		       error_status_t * st
)
{
    boolean32       result;

    acl_test_write_access(h, &result, st);
    if (BAD_STATUS(st)) {
	return;
    } else if (result == FALSE) {
	*st = -1;		/* XXX */
    } else {
	dce_svc_debug_set_levels(flags, st);
    }
}


static void
aud_svc_inq_components(
		       handle_t h,
		       dce_svc_stringarray_t * table,
		       error_status_t * st
)
{
    boolean32       result;

    acl_test_read_access(h, &result, st);
    if (BAD_STATUS(st)) {
	return;
    } else if (result == FALSE) {
	*st = -1;		/* XXX */
    } else {
	dce_svc_components(table, st);
    }
}


static void
aud_svc_inq_table(
		  handle_t h,
		  dce_svc_string_t component,
		  dce_svc_subcomparray_t * table,
		  error_status_t * st
)
{
    boolean32       result;

    acl_test_read_access(h, &result, st);
    if (BAD_STATUS(st)) {
	return;
    } else if (result == FALSE) {
	*st = -1;		/* XXX */
    } else {
	dce_svc_table(component, table, st);
    }
}


static void
aud_svc_inq_routings(
		     handle_t h,
		     boolean32 debug_values,
		     dce_svc_stringarray_t * table,
		     error_status_t * st
)
{
    boolean32       result;

    acl_test_read_access(h, &result, st);
    if (BAD_STATUS(st)) {
	return;
    } else if (result == FALSE) {
	*st = -1;		/* XXX */
    } else {
	dce_svc_routings(table, debug_values,  st);
    }
}


static void
aud_svc_filter_ctl(
		   handle_t h,
		   dce_svc_string_t component,
		   idl_long_int arg_size,
		   idl_byte * argument,
		   error_status_t * st
)
{
    boolean32       result;

    acl_test_write_access(h, &result, st);
    if (BAD_STATUS(st)) {
	return;
    } else if (result == FALSE) {
	*st = -1;		/* XXX */
    } else {
	dce_svc_filter(component, arg_size, argument, st);
    }
}


static void
aud_svc_inq_stats(
		  handle_t h,
		  dce_svc_stats_t * stats,
		  error_status_t * st
)
{
    *st = svc_s_no_stats;
}


struct serviceability_v1_0_epv_t dce_svc_ep = {
aud_svc_set_route,
aud_svc_set_dbg_route,
aud_svc_set_dbg_levels,
aud_svc_inq_components,
aud_svc_inq_table,
aud_svc_inq_routings,
aud_svc_filter_ctl,
aud_svc_inq_stats
};
