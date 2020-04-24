/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: timop_svc_svc.c,v $
 * Revision 1.1.4.2  1996/02/18  18:26:39  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:48:34  marty]
 *
 * Revision 1.1.4.1  1995/12/08  20:55:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  20:50:20  root]
 * 
 * Revision 1.1.2.1  1994/06/14  19:57:49  weir
 * 	DCE 1.1 Beta check-in
 * 	[1994/06/14  16:15:23  weir]
 * 
 * $EndLog$
 */
/***************************************************************************************
* Serviceability Remote Routines.
* 
* 
*
* These are the implementations of the remote serviceability calls. The code below
* is built and linked into the server object; meanwhile the service.idl file is pro-
* cessed and the output of that is a set of client and server stubs for these imple-
* mentations. The server stub is generated with the -no_mepv option, which allows us
* to call our implementations by our own names, and explicitly initialize the entry
* point vector structure with them (see the end of this file for how that happens).
* The client of course calls the routines by their standard names, as generated in
* the client stub from service.idl.
* 
* Mainly the implementations consist of making an authorization check on the caller,
* and then (if everything is okay) calling the serviceability library routine that
* the caller is trying to get to.
*
* In order to make these calls remotely accessible, the server has to go through the
* steps of registering the serviceability interface (separately from all other inter-
* faces, of course) with the name service, and of registering its endpoints with the
* serviceability interface (and the "dce_svc_epv" vector) with the runtime. Then the
* client has to import bindings to the serviceability interface separately as well.
* How all this is done can be seen in timop_svc_client.c and timop_svc_server.c.
*
*
* 
* 
* 
***************************************************************************************/

#include <stdio.h>
#include <dce/utc.h>
#include "timop_svc.h"
#include "timop_svc_aux.h"
#include "timop_svc_server.h"

#include <dce/dce.h>
#include <stddef.h>
#include <dce/dce_error.h>
#include <dce/dce_cf.h>
#include <dce/rpc.h>
#include <dce/utctypes.h>
#include <pthread.h>
#include <dce/sec_login.h>
#include <dce/dcesvcmsg.h>
#include <dce/svclog.h>
#include <dce/dce_msg.h>
#include <dce/dce_svc.h>
#include "dcetsvsvc.h"
#include "dcetsvmsg.h"
#include "dcetsvmac.h"







/*
#include <dce/dce.h>
#include <dce/dce_svc.h> 
#include <dce/dcesvcmsg.h>
#include <dce/svcremote.h>



#include "timop_svc.h"
#include "timop_svc_aux.h"
#include "timop_svc_server.h"
*/


struct serviceability_v1_0_epv_t       dce_svc_epv;

/* test strings for dce_svc_inq_routings()...  */
dce_svc_string_t	my_routing_table_strings[] = {"Once upon a midnight dreary",
						      "As I pondered weak and weary",
						      "Over many a quaint and curious",
						      "Volume of forgotten lore..."};
/*****
*
* xxx_svc_set_route -- remote call-in to set routing.
*
*****/
static void
tsv_svc_set_route(
handle_t	h,
idl_byte	where[],
error_status_t	*st)
{
	int 	ret;

	printf("          tsv_svc_set_route() called!\n");

	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_remote, svc_c_debug7, "Entering tsv_svc_set_route()"));

	ret = ref_mon(h, SVC_REMOTE);
	if (ret == NOT_AUTHORIZED)
	{
		*st = svc_s_no_perm;
		return;
	}

	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_remote, svc_c_debug7, "Calling dce_svc_routing()"));
	
	dce_svc_routing(where, st);
}


/*****
*
* xxx_svc_set_dbg_route -- remote call-in to set debug routing.
*
*****/
static void
tsv_svc_set_dbg_route(
handle_t	h,
idl_byte	where[],
error_status_t	*st)
{
	int 	ret;

	printf("          tsv_svc_set_dbg_route() called!\n");

	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_remote, svc_c_debug7, "Entering tsv_svc_set_dbg_route()"));
	ret = ref_mon(h, SVC_REMOTE);
	if (ret == NOT_AUTHORIZED)
	{
		*st = svc_s_no_perm;
		return;
	}

	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_remote, svc_c_debug7, "Calling dce_svc_debug_routing()"));
	dce_svc_debug_routing(where, st);
}


/*****
*
* xxx_svc_set_dbg_levels -- remote call-in to set debug levels.
*
*****/
static void
tsv_svc_set_dbg_levels(
handle_t	h,
idl_byte	flags[],
error_status_t	*st)
{
	int 	ret;

	printf("          tsv_svc_set_dbg_levels() called!\n");
	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_remote, svc_c_debug7, "Entering tsv_svc_set_dbg_levels()"));
	ret = ref_mon(h, SVC_REMOTE);
	if (ret == NOT_AUTHORIZED)
	{
		*st = svc_s_no_perm;
		return;
	}

	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_remote, svc_c_debug7, "Calling dce_svc_debug_set_levels()"));
	dce_svc_debug_set_levels(flags, st);
}


/*****
*
* xxx_svc_inq_components -- remote request for list of all components registered
*                           by dce_svc_register().
*
*****/
static void
tsv_svc_inq_components(
handle_t		h,
dce_svc_stringarray_t	*table,
error_status_t		*st)
{
	int 		ret;

	printf("          tsv_svc_inq_components() called!\n");
	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_remote, svc_c_debug7, "Entering tsv_svc_inq_components()"));
	ret = ref_mon(h, SVC_REMOTE);
	if (ret == NOT_AUTHORIZED)
	{
		*st = svc_s_no_perm;
		return;
	}

	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_remote, svc_c_debug7, "Calling dce_svc_components()"));
	dce_svc_components(table, st);
}


/*****
*
* xxx_svc_inq_table -- remote request for table registered with the
*                      specified component. (Apparently the subcom-
*                      ponent table.)
*
*****/
static void
tsv_svc_inq_table(
handle_t		h,
dce_svc_string_t	component,
dce_svc_subcomparray_t	*table,
error_status_t		*st)
{
	int 		ret;

	printf("          tsv_svc_inq_table() called!\n");
	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_remote, svc_c_debug7, "Entering tsv_svc_inq_table()"));
	ret = ref_mon(h, SVC_REMOTE);
	if (ret == NOT_AUTHORIZED)
	{
		*st = svc_s_no_perm;
		return;
	}

	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_remote, svc_c_debug7, "Calling dce_svc_table()"));
	dce_svc_table(component, table, st);
}


/*****
*
* xxx_svc_inq_routings -- remote request for routings in effect.
*
*****/
static void
tsv_svc_inq_routings(
handle_t		h,
boolean32		debug_values,
dce_svc_stringarray_t	*table,
	/*                                              */
	/* typedef struct dce_svc_stringarray_s_t {     */
	/*      idl_long_int      tab_size;             */
	/*      dce_svc_string_t  *table;               */
	/* } dce_svc_stringarray_t;                     */
	/*                                              */
error_status_t		*st)
{
	int 		ret;

	printf("          tsv_svc_inq_routings() called!\n");
	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_remote, svc_c_debug7, "Entering tsv_svc_inq_routings()"));
	ret = ref_mon(h, SVC_REMOTE);
	if (ret == NOT_AUTHORIZED)
	{
		*st = svc_s_no_perm;
		return;
	}

	/* Note the following carefully. It seems necessary to associate some */
	/*  real space with these arguments, one way or the other, to prevent */
	/*  the server core-dumping on the return, at least while I'm not     */
	/*  calling the library routine...                                    */

	/* "tab_size" is the number of strings we're sending... */
/*	table->tab_size = 4;
	table->table = (dce_svc_string_t *)&my_routing_table_strings;

	*st = rpc_s_ok;

	return;
*/
	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_remote, svc_c_debug7, "Calling dce_svc_routings()"));
	dce_svc_routings(table, debug_values, st);
}


/*****
*
* xxx_svc_filter_ctl -- remote call-in to adjust operation of serviceability
*                       filter routine. See my_filter_control() in timop_svc_server.c
*                       for an explanation of what's going on.
*
*****/
static void
tsv_svc_filter_ctl(
handle_t		h,
dce_svc_string_t	component,
idl_long_int		arg_size,    /* length of argument */
idl_byte		*argument,
error_status_t		*st)
{
	char		*temp_string;
	int 		ret;

	printf("          tsv_svc_filter_ctl() called!\n");
	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_remote, svc_c_debug7, "Entering tsv_svc_filter_ctl()"));
	*st = rpc_s_ok;

	/* Print out the arguments we got... */
	printf("            component passed is:       %s\n", component);
	printf("            arg size passed is:        %d\n", arg_size);

	temp_string = (char *)malloc(arg_size + 1);
	strncpy(temp_string, argument, arg_size);
	*(temp_string + arg_size) = '\0';
	printf("            argument string we got is: %s\n", temp_string);
	free(temp_string);

	ret = ref_mon(h, SVC_REMOTE);
	if (ret == NOT_AUTHORIZED)
	{
		*st = svc_s_no_perm;
		return;
	}

	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_remote, svc_c_debug7, "Calling dce_svc_filter()"));
	/* The following calls the installed filter control routine...          */
	dce_svc_filter(component, arg_size, argument, st);
}


/*****
*
* xxx_svc_inq_stats -- remote request for operating statistics.
*
*****/
static void
tsv_svc_inq_stats(
handle_t		h,
dce_svc_stats_t		*stats,
error_status_t		*st)
{
	int 		ret;

	printf("          tsv_svc_inq_stats() called!\n");
	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_remote, svc_c_debug7, "Entering tsv_svc_inq_stats()"));
	*st = svc_s_no_stats;

	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_remote, svc_c_debug7, "dce_svc_inq_stats() not implmented"));

	return;


}


/*                                                                          */
/* The table of slots is created by IDL from the source file                */
/*  src/dce/utils/svc/service.idl, the output of which is dce/service.h,    */
/*  included by dce/dce_svc.h. It's then the job of the application         */
/*  that wishes to offer the remote operations to fill in the table         */
/*  with the implementations' entry points. That's what's being done        */
/*  below. Typically the application simply interposes an appropriate       */
/*  ACL check between the entry into an implementation and the subsequent   */
/*  call to the "real" operation as implemented in the serviceability       */
/*  library. The ACL checks have been commented out in the code above.      */
/*                                                                          */


serviceability_v1_0_epv_t dce_svc_epv = {
	tsv_svc_set_route,
	tsv_svc_set_dbg_route,
	tsv_svc_set_dbg_levels,
	tsv_svc_inq_components,
	tsv_svc_inq_table,
	tsv_svc_inq_routings,
	tsv_svc_filter_ctl,
	tsv_svc_inq_stats
};


