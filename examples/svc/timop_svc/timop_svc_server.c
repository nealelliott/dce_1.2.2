/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: timop_svc_server.c,v $
 * Revision 1.1.4.2  1996/02/18  18:26:34  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:48:33  marty]
 *
 * Revision 1.1.4.1  1995/12/08  20:55:15  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  20:50:17  root]
 * 
 * Revision 1.1.2.1  1994/06/14  19:57:44  weir
 * 	DCE 1.1 Beta check-in
 * 	[1994/06/14  16:15:20  weir]
 * 
 * $EndLog$
 */
/*
 * The Example code is provided to you as "Pass-through Code".
 * Pass-through Code and its related Documentation are not warrented,
 * supported, or indemnified by OSF, and are provided "AS IS".
 * You can use, sublicense, or distribute the same at your own risk.
 */
/*
 * Copyright (c) 1990, 1991  
 * Open Software Foundation, Inc. 
 *  
 * Permission is hereby granted to use, copy, modify and freely distribute 
 * the software in this file and its documentation for any purpose without 
 * fee, provided that the above copyright notice appears in all copies and 
 * that both the copyright notice and this permission notice appear in 
 * supporting documentation.  Further, provided that the name of Open 
 * Software Foundation, Inc. ("OSF") not be used in advertising or 
 * publicity pertaining to distribution of the software without prior 
 * written permission from OSF.  OSF makes no representations about the 
 * suitability of this software for any purpose.  It is provided "as is" 
 * without express or implied warranty. 
 */
/*
 *  OSF DCE Version 1.0 
 */

/*
**	timop_svc_server.c
**
**	Server program for timop_svc interface.
*/

#define MY_ID
#define DO_SVC
#define DCE_DEBUG


#include <dce/dce.h>
#include <stdio.h>
#include <stddef.h>
#include <dce/dce_error.h>
#include <dce/dce_cf.h>
#include <dce/rpc.h>
#include <stdarg.h>
#include <dce/utctypes.h>
#include <pthread.h>
#include <dce/sec_login.h>

#include <dce/dcesvcmsg.h>
#include <dce/svcremote.h>
#include <dce/svcfilter.h>
#include <dce/svclog.h>
#include <dce/dce_msg.h>
#include <dce/dce_svc.h>
#include "dcetsvsvc.h"
#include "dcetsvmsg.h"
#include "dcetsvmac.h"

#include "timop_svc.h"
#include "timop_svc_aux.h"
#include "timop_svc_server.h"


/* extern char *malloc(); */
extern char *strcpy();
extern char *strcat();
extern char *strncat();

/* default routing information... */
#define TIMOP_SVC_FATAL_DEFAULT_ROUTE 0
#define TIMOP_SVC_ERROR_DEFAULT_ROUTE 1
#define TIMOP_SVC_WARNING_DEFAULT_ROUTE 2
#define TIMOP_SVC_NOTICE_DEFAULT_ROUTE 3
#define TIMOP_SVC_VERBOSE_NOTICE_DEFAULT_ROUTE 4
#define MAX_DEFAULT_ROUTES 5
unsigned_char_t *default_routes[MAX_DEFAULT_ROUTES] =
	{"FATAL:TEXTFILE:/tmp/timop_svc_%ld",
	 "ERROR:TEXTFILE:/tmp/timop_svc_%ld",
	 "WARNING:TEXTFILE:/tmp/timop_svc_%ld",
/*	 "NOTICE:TEXTFILE:/tmp/timop_svc_%ld", */
	 "NOTICE:STDERR:",
	 "NOTICE_VERBOSE:TEXTFILE:/tmp/timop_svc_%ld"};

unsigned_char_t default_debug_route[] = "tsv:*.1,*.2,*.3,*.4:STDERR:--";
/* unsigned_char_t default_debug_route[] = "tsv:*.1,*.2,*.3,*.4:TEXTFILE:/usr/users/weir/DEBUG/debugXX_%ld"; */


int routes_G[MAX_ROUTES];

int debug_routes_G[MAX_LEVELS];

int debug_levels_G[MAX_LEVELS];


/* Declare manager EPV.  This EPV could be bulk-initialized here,
   but we prefer to do it one operation at a time in main(). */
timop_svc_v1_0_epv_t		manager_epv;

serviceability_v1_0_epv_t       dce_svc_epv;

dce_svc_handle_t		tsv_svc_handle;

/* Global switch for turning the serviceability filter on or off.   */
/*  If this variable is FALSE, the filter will just return TRUE...  */
boolean32			filter_setting = FALSE;

/*
 *	main()
 *
 *	Get started -- set up server the way we want it, and call listen loop.
 */

int
main(
	int				argc,
	char				*argv[])
{
	unsigned_char_t			*server_name;
	rpc_binding_vector_t		*bind_vector_p;
	rpc_binding_vector_t		*svc_bind_vector_p;
	sec_login_handle_t              login_context;
	rpc_binding_vector_t            *bind_p;
	char                            *keytab = (char *)SERVER_KEYFILE;
	void                            *sec_key_p;
	unsigned_char_t                 *string_binding, *svc_string_binding;
	unsigned_char_t                 *str_p;
	boolean32                       reset_passwd;
	sec_login_auth_src_t            auth_src;
	unsigned_char_t			*my_msg;
	int				routes, debug_routes, debug_levels;
	boolean32			route_error, debug_error;

	int				entry_ix;
	int				entry_flag;  /* This server instance's entry object.  */
						     /* This is for allowing different server */
						     /*  instances on the same machine to ex- */
						     /*  port to different entries, and be    */
						     /*  reachable thereunder.                */

	unsigned_char_t *OBJ_UUIDS[OBJECT_AMOUNT + 1] = {"", OBJ_UUID_1, OBJ_UUID_2};

	int				setup;
	int				command_error;
	unsigned32			status;
	int				i;
	uuid_t				type_uuid, obj_uuid;

	struct {
	    unsigned32	count;
	    uuid_t	*uuid[NUM_OBJS];
	}				obj_uuid_vec;

	/*  = {NUM_OBJS, {&obj_uuid}} */

	obj_uuid_vec.count = NUM_OBJS;
	obj_uuid_vec.uuid[0] = &obj_uuid;
	
	/* Set all the command line flags...                       */
	if (command_error = process_args(argc, argv, FALSE))
	{
		fprintf(stderr, "  Usage: %s [-w<svc_route> [-w<svc_route> ... ]] [-d\"<dbg_route>\" [-d\"<dbg_route>\" ... ]] \\ \n",
		        argv[0]);
		fprintf(stderr, "                      [-D\"<dbg_level>\" [-D\"<dbg_level>\" ... ]] [-f] -e<nr> <entry_name>\n\n");
		fprintf(stderr, "  Where:\n");
		fprintf(stderr, "         -w<svc_route>      (optional, one or more) Specifies a serviceability routing.\n");
		fprintf(stderr, "         -d\"<dbg_route>\"    (optional, one or more) Specifies a serviceability debug\n");
		fprintf(stderr, "                            routing.\n");
		fprintf(stderr, "         -D\"<dbg_level>\"    (optional, one or more) Specifies a serviceability debug\n");
		fprintf(stderr, "                            level.\n");
		fprintf(stderr, "         -f                 (optional) Causes the serviceability filter to be installed.\n");
		fprintf(stderr, "         -e1 or -e2         Specifies the object entry this server instance is\n");
		fprintf(stderr, "                            using for export.\n");
		fprintf(stderr, "         <entry_name>       Specifies the name of the entry to which this server\n");
		fprintf(stderr, "                            instance should export.\n");
		exit(1);
	}

	/* This call sets the contents of a substring area at the beginning of each message,  */
	/*  identifying the program that is writing the message. If this call is omitted,     */
	/*  the program is identified by process ID in the substring...                       */
	/*                                                                                    */
	dce_svc_set_progname(argv[0], &status);
	if (status != svc_s_ok)
	{
		print_server_error("dce_svc_set_progname()", status);
	}

	/* The following calls set up default routing of serviceability messages. Note that   */
	/*  these must be called before dce_svc_register()...                                 */
	/*                                                                                    */
	for (i = 0, route_error = FALSE; (i < MAX_DEFAULT_ROUTES) && (!route_error); i++)
	{
		printf("          Setting default route %s ...\n", default_routes[i]);
		dce_svc_routing(default_routes[i], &status);
		if (status != svc_s_ok)
		{
			print_server_error("dce_svc_routing(default_routes[i])", status);
			status = svc_s_ok;
		}
	}

	/* If any routings were specified on the command line, set them now...  */
	routes = process_args(argc, argv, DO_ROUTES);
	route_error = FALSE;
	if (routes)
	{
		printf("          Number of routes specified is %d\n", routes);
		printf("          Route index numbers and strings are...\n");
		for (i = 0; (i < routes) && (!route_error); i++)
		{
			printf("            %d   ", routes_G[i]);
			printf("%s\n", (char *) (argv[routes_G[i]]+2));
			dce_svc_routing((unsigned_char_t *)(argv[routes_G[i]]+2), &status);
			if (status != svc_s_ok)
			{
				print_server_error("dce_svc_routing(argv[routes_G[i]])", status);
				status = svc_s_ok;
				route_error = TRUE;
			}
		}
	}

	/* If any debug routings were specified, set them...  */
	debug_routes = process_args(argc, argv, DO_DEBUG_ROUTES);
	debug_error = FALSE;
	if (debug_routes)
	{
		printf("          Number of debug routes specified is %d\n", debug_routes);
		printf("          Debug route index numbers and strings are...\n");
		for (i = 0; (i < debug_routes) && (!debug_error); i++)
		{
			printf("            %d   ", debug_routes_G[i]);
			printf("%s\n", (char *) (argv[debug_routes_G[i]]+2));
			dce_svc_routing((unsigned_char_t *)(argv[debug_routes_G[i]]+2), &status);
			if (status != svc_s_ok)
			{
				print_server_error("dce_svc_routing(argv[debug_routes_G[i]])", status);
				status = svc_s_ok;
				route_error = TRUE;
			}
		}
	}

	/* If debug levels were specified, set them... */
	debug_levels = process_args(argc, argv, DO_DEBUG_LEVELS);
	debug_error = FALSE;
	if (debug_levels)
	{
		printf("          Number of debug levels specified is %d\n", debug_levels);
		printf("          Debug level index numbers and strings are...\n");
		for (i = 0; (i < debug_levels) && (!debug_error); i++)
		{
			printf("            %d   ", debug_levels_G[i]);
			printf("%s\n", (char *) (argv[debug_levels_G[i]]+2));
			/* Set the debug level for all sub-components...                      */
/*
			dce_svc_debug_set_levels((unsigned_char_t *)(argv[debug_levels_G[i]]+2), &status);
			if (status != svc_s_ok)
			{
				print_server_error("dce_svc_debug_set_levels(default)", status);
				status = svc_s_ok;
			}
*/
		}
	}

	/* This call registers the message table. It apparently sets up various service-      */
	/*  ability data structures. You use the handle that is returned by this call in      */
	/*  all subsequent calls to print out serviceability messages, etc...                 */
	/* Note the resetting of "status"; this variable is not cleared on a subsequent suc-  */
	/*  cessful call to some other routine if it has been set to some error code...       */
	/*                                                                                    */

	tsv_svc_handle = dce_svc_register(tsv_svc_table, (idl_char*)"tsv", &status);
	if (status != svc_s_ok)
	{
		print_server_error("dce_svc_register()", status);
	}

	/* This call can be used to add a message table to the in-core table. Note that in    */
	/*  order for it to work properly, you have to #include <dce/dce_msg.h>. You also     */
	/*  have to link in dcetsvmsg.o and dcetsvsvc.o, which contain the code for the mes-  */
	/*  sages and the table, respectively. If you do this, and if you make the following  */
	/*  call, you won't be dependent on having the message catalog installed correctly,   */
	/*  nor (it follows) on having the locale or NLSPATH or whatever correctly set.       */
	/*                                                                                    */
	/* Note the different table name, taken from dcetsvmsg.c. The table name used here is */
	/*  the one specified by the "table" header in Part I of the sams file, if it is dif- */
	/*  ferent from the one specified in Part II by "serviceability table". If the Part I */
	/*  header is left to default, then the name used here should be the "xxx_msg_table"  */
	/*  default name as described in the sams.1dce manpage. If the two names are the same */
	/*  then of course you use the same name here. Note however that the two names have   */
	/*  different types, so you have to do some casting if you want to use the same name. */
	/*                                                                                    */
	dce_msg_define_msg_table(tsv__table, sizeof(tsv__table) / sizeof(tsv__table[0]), &status);
	if (status != svc_s_ok)
	{
		print_server_error("dce_msg_define_msg_table()", status);
	}


	/* The following call is to a subroutine that installs the filtering routines; see    */
	/*  the definitions below for more information...                                     */
	/*                                                                                    */

	if (process_args(argc, argv, DO_FILTER_FLAG))
		install_filters();

	/* The following call prints out a serviceability message. Note the structure of the  */
	/*  call:                                                                             */
	/*                                                                                    */
	/*       1. Nested call to DCE_SVC macro, with arguments:                             */
	/*                     table handle                                                   */
	/*                     format string                                                  */
	/*                                                                                    */
	/*       2. Table index (tsv_s_server)                                                */
	/*                                                                                    */
	/*       3. Message attributes (these are or'ed in together)                          */
	/*                                                                                    */
	/*       4. Message id (tsv_s_sign_on)                                                */
	/*                                                                                    */
	/*      [5. Any necessary arguments demanded by the format string given in the        */
	/*          DCE_SVC macro call above.]                                                */
	/*                                                                                    */
	/* Note that if the message has been defined (in the .sams file) with certain fields  */
	/*  (i.e., "attributes" and "sub-component"), then a convenience macro will be gener- */
	/*  ated by sams that will automatically take care of arguments 1-4.                  */
	/*                                                                                    */
	/* full version... */
	dce_svc_printf(DCE_SVC(tsv_svc_handle, ""), tsv_s_server, svc_c_sev_notice | svc_c_route_stderr, tsv_s_testmessage);


	/* Using the convenience macro to print the same message...                           */
	dce_svc_printf(TSV_S_TESTMESSAGE_MSG);
	if (status != svc_s_ok)
	{
		print_server_error("dce_svc_printf() with macro", status);
		status = svc_s_ok;
	}


	/* The following call is supposed (if I correctly understand its description) to re-  */
	/*  trieve a message from the in-core table, as opposed to extracting it from the     */
	/*  message catalog. You have to #include <dce/dce_msg.h> for this to work...         */
	/*                                                                                    */
	/*                                                                                    */
	my_msg = (unsigned_char_t *)dce_msg_get_default_msg(tsv_s_sign_on, &status);
	if (status != svc_s_ok)
	{
		print_server_error("dce_msg_get_default_msg()", status);
		status = svc_s_ok;
	}
	printf("%s\n", my_msg);


	/* Following is a second call to print out a serviceability message...                */
	/* Note that adding a second attribute such as "svc_c_route_stderr" will cause the    */
	/*  message to be printed twice if the notices are aleady being routed to stderr...   */
	/*                                                                                    */
	dce_svc_printf(DCE_SVC(tsv_svc_handle, ""), tsv_s_server, svc_c_route_stderr, tsv_s_sign_on);
	if (status != svc_s_ok)
	{
		print_server_error("dce_svc_printf()", status);
		status = svc_s_ok;
	}


	/* The following call extracts a debug message from the catalog and prints it. This   */
	/*  is in order to prove to myself that the messages can indeed be found in the cata- */
	/*  log (since at present I am unable to get them to print via DCE_SVC_LOG; see im-   */
	/*  mediately below...                                                                */
	/*                                                                                    */
	/* Note that there is apparently something un-kosher about this call. It is the only  */
	/*  one that doesn't work with an in-core table when the message catalog is not in-   */
	/*  stalled, and it also even when it works prints a message string with garbage in   */
	/*  its severity field.                                                               */
	/*                                                                                    */
	dce_svc_printf(DCE_SVC(tsv_svc_handle, ""), tsv_s_server, svc_c_route_stderr, tsv_s_debug1_test);
	if (status != svc_s_ok)
	{
		print_server_error("dce_svc_printf()", status);
		status = svc_s_ok;
	}


	/* The following works because I've set the SVC_TSV_DBG=tsv:*.1:STDERR:-- environment */
	/*  variable...                                                                       */
	/* I've also been able to make it work by calling dce_svc_routing() with an error     */
	/*  format...                                                                         */

	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_server, svc_c_debug1, "DCE_SVC_DEBUG level 1 message"));
	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_server, svc_c_debug2, "DCE_SVC_DEBUG level 2 message"));
	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_server, svc_c_debug3, "DCE_SVC_DEBUG level 3 message"));
	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_server, svc_c_debug4, "DCE_SVC_DEBUG level 4 message"));
	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_server, svc_c_debug5, "DCE_SVC_DEBUG level 5 message"));
	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_server, svc_c_debug6, "DCE_SVC_DEBUG level 6 message"));
	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_server, svc_c_debug7, "DCE_SVC_DEBUG level 7 message"));
	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_server, svc_c_debug8, "DCE_SVC_DEBUG level 8 message"));
	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_server, svc_c_debug9, "DCE_SVC_DEBUG level 9 message"));

	/* The following shows how you supply an argument to a macro-ized message...          */
	dce_svc_printf(TSV_S_INNOCUOUS_MESSAGE_MSG, "harmless");

	/* Verbose notice test... */
	dce_svc_printf(TSV_S_VERBOSE_TEST_MSG);

	dce_svc_printf(SVC_TEST_FINISHED_MSG);

	dce_svc_printf(DCE_SVC(tsv_svc_handle, ""), tsv_s_server, svc_c_route_stderr, tsv_s_sign_on);
	if (status != svc_s_ok)
	{
		print_server_error("dce_svc_printf()", status);
		status = svc_s_ok;
	}

/*
	fprintf(stderr, "\n");
*/
	entry_ix = process_args(argc, argv, DO_ENTRY_NAME);
	entry_flag = process_args(argc, argv, DO_ENTRY_FLAG);
	entry_flag_G = entry_flag;
	server_name = (unsigned_char_t *)argv[entry_ix];

	/* I don't know why it's necessary to allocate twice the amount of */
	/*  length in argv[entry_ix] to the global variable-- but it is    */
	/*  necessary, apparently.                                         */
	server_name_G = (unsigned_char_t *)     malloc(((strlen(argv[entry_ix]) * 2) + 1));
	strcpy(server_name_G, argv[entry_ix]);

#ifdef TYPE_MANAGER
	/* TYPE MANAGER STUFF...                                         */
	/* If this is the first invocation of the server, we have to     */
	/*  generate type UUIDs and associate them with the managers.	 */

	/* Note that this is more complicated than I at first realized,  */
	/*  since we have to have a way of saving the type UUIDs and re- */
	/*  reading them in when the server is re-invoked (and is not    */
	/*  being setup).                                                */
	/* The solution to this is simply to generate the UUIDs by hand, */
	/*  and put them into the header, just as was done with the      */
	/*  object UUIDs. A note should then be inserted somewhere       */
	/*  pointing out that this is not perhaps the way one would do   */
	/*  it in a real program (or maybe it is?); and what that way    */
	/*  would be.                                                    */

	if (setup = process_args(argc, argv, DO_SETUP))
		;
#endif


	/* Initialize object uuid ...                                          */
	/* NOTE that he has the object UUID set up as a vector, although       */
	/*   the current number of elements in it (NUM_OBJS) is defined to     */
	/*   be one. The object vector is registered at the endpoint mapper    */
	/*   and exported all to the same entry, so they can't be intended     */
	/*   to be used to distinguish among server instances as such.         */

	if (entry_flag == 0)
	{
		uuid_from_string(OBJ_UUID_1, &obj_uuid, &status);
		if (status != uuid_s_ok)
		{
			print_server_error("uuid_from_string()", status);
			exit(1);
		}
		entry_flag_G = 1; /* to set the default object number */
	}
	else if (entry_flag <= OBJECT_AMOUNT)
	{
		uuid_from_string(OBJ_UUIDS[entry_flag], &obj_uuid, &status);
		if (status != uuid_s_ok)
		{
			print_server_error("uuid_from_string()", status);
			exit(1);
		}
	}
	else
	{
#ifdef DO_SVC
		dce_svc_printf(BAD_OBJECT_MSG);
#else
		fprintf(stderr, " Bad object number\n");
#endif
		exit(1);
	}


	/* Initialize manager EPV (just one entry point in this example). */
	manager_epv.timop_svc_getspan = getspan_ep;


	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_server, svc_c_debug5, "Calling uuid_create()"));
	/* Initialize type uuid (just one in this example). */
	uuid_create(&type_uuid, &status);
	if (status != uuid_s_ok)
	{
		print_server_error("uuid_create()", status);
		exit(1);
	}

	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_server, svc_c_debug5, "Calling rpc_object_set_type()"));
	/* Register object/type uuid associations with rpc runtime. */
	rpc_object_set_type(&obj_uuid, &type_uuid, &status);
	if (status != rpc_s_ok)
	{
		print_server_error("rpc_object_set_type()", status);
		exit(1);
	}

	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_server, svc_c_debug5, "Calling rpc_server_register_if()"));
	/* Register interface/type_uuid/epv associations with rpc runtime. */
	rpc_server_register_if(timop_svc_v1_0_s_ifspec, &type_uuid,
	    (rpc_mgr_epv_t)&manager_epv, &status);
	if (status != rpc_s_ok)
	{
		print_server_error("rpc_server_register_if()", status);
		exit(1);
	}


	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_server, svc_c_debug5, "Calling rpc_server_register_if(svc)"));
	/* Register serviceability remote interface...   */
	rpc_server_register_if(serviceability_v1_0_s_ifspec, &type_uuid,
	    (rpc_mgr_epv_t)&dce_svc_epv, &status);
	if (status != rpc_s_ok)
	{
		print_server_error("rpc_server_register_if(serviceability_v1_0_s_ifspec)", status);
		exit(1);
	}

	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_server, svc_c_debug5, "Calling rpc_server_use_all_protseqs()"));
	/* Tell rpc runtime we want to use all supported protocol sequences. */
	rpc_server_use_all_protseqs(MAX_CONC_CALLS_PROTSEQ, &status);
	if (status != rpc_s_ok)
	{
		print_server_error("rpc_server_use_all_protseqs()", status);
		exit(1);
	}

	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_server, svc_c_debug5, "Calling rpc_server_inq_bindings()"));
	/* Ask the runtime which binding handle(s) it's going to let us use. */
	rpc_server_inq_bindings(&bind_vector_p, &status);
	if (status != rpc_s_ok)
	{
		print_server_error("rpc_server_inq_bindings()", status);
		exit(1);
	}



	/********************************************************************************
	*
	* The following shows how to convert a vector of bindings into string bindings,
	*    and to print them out...
	*
	*
	*****/

#ifdef DO_SVC
	dce_svc_printf(TSV_S_BINDINGS_RECEIVED_MSG, "timop_svc", bind_vector_p->count);
#else
	fprintf(stderr, " Server: Number of bindings received == %d\n", bind_vector_p->count);
#endif
	for (i = 0; i < bind_vector_p->count; i++)
	{
		DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_server, svc_c_debug5, "Calling rpc_binding_to_string_binding()"));
		rpc_binding_to_string_binding(bind_vector_p->binding_h[i], &string_binding, &status);
		if (status != rpc_s_ok)
		{
			print_server_error("rpc_binding_to_string_binding()", status);
			exit(1);
		}
#ifdef DO_SVC
		dce_svc_printf(FULL_BINDING_MSG, "timop_svc", string_binding);
#else
		fprintf(stderr, "  Full binding in string form == %s\n", string_binding);
#endif
		DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_server, svc_c_debug5, "Calling rpc_string_free()"));
		rpc_string_free(&string_binding, &status);
		if (status != rpc_s_ok)
		{
			print_server_error("rpc_string_free()", status);
			exit(1);
		}


	}


	/********************************************************************************/

	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_server, svc_c_debug5, "Calling rpc_server_register_auth_info()"));
	/* Register authentication info with rpc runtime. */
	rpc_server_register_auth_info(SERVER_PRINC_NAME,
	    rpc_c_authn_dce_secret, NULL /*default key retrieval function*/,
	    KEYTAB /*server key table for this example*/, &status);
	if (status != rpc_s_ok)
	{
		print_server_error("rpc_server_register_auth_info()", status);
		exit(1);
	}

	/* Establish server's login context(s), if necessary.
	   In this example we just use the default login context,
	   so we do NOTHING here. */



#ifdef MY_ID

	/********************************************************************************
	*
	* The following is adapted from:
	*    /project/dce/build/dce1.0.2/src/test/systest/rpc/rpc.cds.3/rpc.cds.3_smain.c
	*
	* It shows how to make a server assume its "own" identity...
	*
	*****/

	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_server, svc_c_debug5, "Calling rpc_mgmt_inq_server_princ_name()"));
	/* Get the current server principal...  */
	rpc_mgmt_inq_server_princ_name((rpc_binding_handle_t)NULL, 
                             rpc_c_authn_dce_secret, &str_p,  &status);
	if (status != rpc_s_ok)
	{
		print_server_error("rpc_mgmt_inq_server_princ_name()", status);
		exit(1);
	}

#ifdef DO_SVC
	dce_svc_printf(PRINC_NAME_MSG, str_p);
#else
	printf("\n Server: Principal name before the change is %s.\n",str_p);
#endif
	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_server, svc_c_debug5, "Calling rpc_string_free()"));
	rpc_string_free(&str_p, &status);


	/* Create a context and get the login context loaded...  */
#ifdef DO_SVC
	dce_svc_printf(CREATE_CONTEXT_MSG);
#else
	printf(" Server: creating login context...\n");
#endif
	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_server, svc_c_debug5, "Calling sec_login_setup_identity()"));
	sec_login_setup_identity(SERVER_PRINC_NAME,
                         (sec_login_flags_t)sec_login_no_flags,
                         &login_context, &status);
	if (status != error_status_ok)
	{
		print_server_error("sec_login_setup_identity()", status);
		exit(1);
	}


	/* Get our secret key from our keytab file...  */
#ifdef DO_SVC
	dce_svc_printf(KEY_ACCESS_MSG, keytab);
#else
	printf(" Server: getting secret key from %s...\n", keytab);
#endif
	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_server, svc_c_debug5, "Calling sec_key_mgmt_get_key()"));
	sec_key_mgmt_get_key(rpc_c_authn_dce_secret, (void *)keytab,
                     SERVER_PRINC_NAME, (unsigned32)0, 
                     &sec_key_p, &status);
	if (status != error_status_ok)
	{
		print_server_error("sec_key_mgmt_get_key()", status);
		exit(1);
	}

	/* Validate our login context...  */
#ifdef DO_SVC
	dce_svc_printf(CONTEXT_VALIDATION_MSG);
#else
	printf(" Server: validating login context...\n");
#endif
	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_server, svc_c_debug5, "Calling sec_login_validate_identity()"));
	sec_login_validate_identity(login_context, sec_key_p, &reset_passwd,
                                                        &auth_src, &status);
	if (status != error_status_ok)
	{
		print_server_error("sec_login_validate_identity()", status);
		exit(1);
	}

	/* If we did not get validated for network ops... */
	if (auth_src != sec_login_auth_src_network)
	{
		/* error and out */
#ifdef DO_SVC
		dce_svc_printf(SERVER_NOT_AUTHORIZED_MSG);
#else
		fprintf(stderr, " Server: Not authorized for network operations!\n");
#endif
		exit(2);
	}

	/* Certify identity information...  */
#ifdef DO_SVC
	dce_svc_printf(ID_CERTIFICATION_MSG);
#else
	fprintf(stderr, " Server: certifying identity info...\n");
#endif
	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_server, svc_c_debug5, "Calling sec_login_certify_identity()"));
	sec_login_certify_identity(login_context, &status);
	if (status != error_status_ok)
	{
		print_server_error("sec_login_certify_identity()", status);
		exit(1);
	}

	/* Finally, set our context...  */
#ifdef DO_SVC
	dce_svc_printf(CONTEXT_SET_MSG);
#else
	fprintf(stderr, " Server: setting own context...\n\n");
#endif
	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_server, svc_c_debug5, "Calling sec_login_set_context()"));
	sec_login_set_context(login_context, &status);
	if (status != error_status_ok)
	{
		print_server_error("sec_login_set_context()", status);
		exit(1);
	}

	/* END of assume-own-identity code  */
	/********************************************************************************/

#endif


	/* Decide what to do upon server termination.  It would be prudent
	   to handle signals and decide what to do if the listen loop returns
	   (e.g., clean exported info out of endpoint map and namespace,
	   something that is not usually done for a persistent server),
	   but since this is just an example we don't do those things here. */

	/* Register binding info with endpoint map.                       */
	/* NOTE that, even though we're possibly going to have multiple   */
	/*      (well, two) instances of this server active on the same   */
	/*      host, we don't have to use the "_no_replace" form of this */
	/*      call, because the fact that the different servers are al- */
        /*      ready distinguished by different object UUIDs in their    */
	/*      bindings ensures that the registrations will be separate  */
	/*      from rpcd's point of view, and as a consequence that nei- */
	/*      ther registration will overwrite the other. At least,     */
	/*      that's what the manpage for rpc_ep_register_no_replace()  */
	/*      seems to say, and doing it this way does work...          */

	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_server, svc_c_debug5, "Calling rpc_ep_register()"));
	rpc_ep_register(timop_svc_v1_0_s_ifspec, bind_vector_p,
            (uuid_vector_t *)&obj_uuid_vec,
	    (unsigned_char_t *)"timop_svc server, version 1.0", &status);
	if (status != rpc_s_ok)
	{
		print_server_error("rpc_ep_register()", status);
		exit(1);
	}

	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_server, svc_c_debug5, "Calling rpc_ep_register(svc)"));
	/* Register endpoints with serviceability interface...  */
	rpc_ep_register(serviceability_v1_0_s_ifspec, bind_vector_p,
	    (uuid_vector_t *)&obj_uuid_vec,
	    (unsigned_char_t *)"Timop_SVC SVC",
	    &status);
	if (status != rpc_s_ok)
	{
		print_server_error("rpc_ep_register(serviceability_v1_0_s_ifspec)", status);
		exit(1);
	}

	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_server, svc_c_debug5, "Calling rpc_ns_binding_export()"));
	/* Export binding info to the namespace. */
	rpc_ns_binding_export(rpc_c_ns_syntax_dce, server_name,
	    timop_svc_v1_0_s_ifspec, bind_vector_p,
	    (uuid_vector_t *)&obj_uuid_vec, &status);
	if (status != rpc_s_ok)
	{
		print_server_error("rpc_ns_binding_export()", status);
		exit(1);
	}

	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_server, svc_c_debug5, "Calling rpc_ns_binding_export(svc)"));
	rpc_ns_binding_export(rpc_c_ns_syntax_dce, server_name,
	    serviceability_v1_0_s_ifspec, bind_vector_p,
	    (uuid_vector_t *)&obj_uuid_vec, &status);
	if (status != rpc_s_ok)
	{
		print_server_error("rpc_ns_binding_export(serviceability_v1_0_s_ifspec)", status);
		exit(1);
	}


	/* Listen for service requests (semi-infinite loop). */
#ifdef DO_SVC
	dce_svc_printf(READY_MSG, server_name, entry_flag_G);
#else
	fprintf(stdout, " Server %s (object %d) ready...\n\n", server_name, entry_flag_G);
#endif
	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_server, svc_c_debug5, "Calling rpc_server_listen()"));
	rpc_server_listen(MAX_CONC_CALLS_TOTAL, &status);
	if (status != rpc_s_ok)
	{
		print_server_error("rpc_server_listen()", status);
		exit(1);
	}

	/* Returned from listen loop.  We haven't arranged for this. */
#ifdef DO_SVC
	dce_svc_printf(LISTEN_RETURN_MSG);
#else
	fprintf(stderr, " Server: Unexpectedly returned from server_listen()\n");
#endif
	exit(1);
}




/*****
*
* process_args-- Server version.
*
*****/
int
process_args(argc, argv, check)
int argc;
char *argv[];
int check;        /* if FALSE, simply scan the command line; */
{                 /*  if not FALSE, return the state of the  */
                  /*  flag/option specified by 'check'       */

	int i, j, o;
	int error = FALSE;
	static int entry_ix = 0;
	static int entry_flag = 0;
	static int filter_flag = FALSE;
	static int routing_flag = FALSE;
	static int routes = 0;
	static int debug_flag = FALSE;
	static int debug_routes = 0;
	static int debug_levels_flag = FALSE;
	static int debug_levels = 0;

	if (!check)
	{    
		if (argc)
		{
			/* one argument may or may not be present, so all we   */
			/*  can do here is check that we haven't exceeded the  */
			/*  maximum number...                                  */
			if (argc > MAX_SERVER_ARGS)
				error = TRUE;

			else for (i = 1; (i < argc) && (!error); i++)
			{
				/* entry name */
				if (argv[i][0] != '-')
					entry_ix = i;

				/* option flag */
				else if (argv[i][0] == '-')
				{
					switch (argv[i][1])
					{
						case 'e':
						/* return a number, depending on the  */
						/*  object we're supposed to be man-  */
						/*  aging...                          */
							o = 0;
							for (j = 2; isdigit(argv[i][j]); j++)
							{
								o = o + (10 * o + (argv[i][j] - '0'));
							}
							entry_flag = o;
							break;

						case 'f':
							filter_flag = TRUE;
							break;

						case 'w':
							if (!isalpha(argv[i][2]))
							{
								printf("No -w option value!\n");
								error = TRUE;
							}
							else
							{
								routing_flag = TRUE;
								routes_G[routes] = i;
								routes++;
							}
							break;

						case 'd':
							if (!isalpha(argv[i][2]))
							{
								printf("No -d option value!\n");
								error = TRUE;
							}
							else
							{
								debug_flag = TRUE;
								debug_routes_G[debug_routes] = i;
								debug_routes++;
							}
							break;

						case 'D':
							if (!isalpha(argv[i][2]))
							{
								printf("No -D option value!\n");
								error = TRUE;
							}
							else
							{
								debug_levels_flag = TRUE;
								debug_levels_G[debug_levels] = i;
								debug_levels++;
							}
							break;

						default:
							error = TRUE;
							break;
                    
					}
				}
			}
		}

	}

	else
	{

		switch (check)
		{
			case DO_ENTRY_NAME:
				return entry_ix;
				break;

			case DO_ENTRY_FLAG:
				return entry_flag;
				break;

			case DO_ROUTES:
				return routes;
				break;

			case DO_DEBUG_ROUTES:
				return debug_routes;
				break;

			case DO_DEBUG_LEVELS:
				return debug_levels;
				break;

			case DO_FILTER_FLAG:
				return filter_flag;
				break;

			default:
				break;

		}

	}

	if ((argc < 3) || (entry_ix == 0))
		error = TRUE;

	if (error)
		return error;
	else
		return NO_FLAG;

}


/*****
*
* print_server_error-- Server version. Prints text associated with bad status code.
*
*****/
void
print_server_error(caller, status)
char *caller;		/* a string identifying the routine that received the error  */
error_status_t status;	/* the status we want to print the message for               */
{
	dce_error_string_t error_string;
	int print_status;

	dce_error_inq_text(status, error_string, &print_status);

#ifdef DO_SVC
	dce_svc_printf(SERVER_ERROR_MSG, caller, error_string);
#else
	fprintf(stderr," Server: %s: %s\n", caller, error_string);
#endif

/* The following shows how you can call dce_printf() to print out a message. This    */
/*  call will print out the simple error string, but not the "(dce / <comp>)" part.  */
/*  That's because those higher bits in which the latter info is contained are not   */
/*  part of the message id, I think.                                                 */
/* Thus, where dce_error_ing_text will give you back a string with something like:   */
/*  "Bad routing specification (dce / svc)", dce_printf will print only the simple   */
/*  "Bad routing specification" part...                                              */
/*                                                                                   */
/*
dce_printf(status);
*/

}


/***************************************************************************************
* Serviceability Filter Routine.
* 
*
* typedef struct dce_svc_prolog_s_t {
*     dce_svc_handle_t   handle;
*     int                version;
*     utc_t              t;
*     const char         *argtypes;
*     unsigned32         table_index;
*     unsigned32         attributes;
*     unsigned32         message_index;
*     char               *format;
*     const char         *file;
*     char               progname[dce_svc_c_progname_buffsize];
*     int                line;
*     pthread_t          thread_id;
* } *dce_svc_prolog_t;
*
***************************************************************************************/


/*****
*
* Filter routine-- this is the routine that's hooked into the serviceability mechanism
*                  when you install it by calling dce_svc_define_filter(). It makes its
*                  own inspection of the circumstances of the message that the service-
*                  ability API is about to output, and returns TRUE if it decides it's
*                  okay to go ahead. If it wants the message suppressed, it returns
*                  FALSE.
*
*****/
boolean my_filter(prolog, args)
dce_svc_prolog_t prolog;
va_list args;
{
	if (filter_setting)
	{
		printf("          The value of filter_setting is TRUE\n");

		printf("          The progname is %s\n", prolog->progname);

		if (prolog->attributes | svc_c_sev_notice)
			printf("          This is a Notice-type message\n");


		switch (prolog->table_index)
		{
			case tsv_s_server:
				printf("     Server sub-component\n");
				break;
			case tsv_s_refmon:
				printf("     Refmon sub-component\n");
				break;
			case tsv_s_manager:
				printf("     Manager sub-component\n");
				break;
		}
	}

	return 1;

}

/*****
*
* Filter Control routine-- this is the entry point for the remote-control call
*                          to modify the filter routine's behavior. Note that
*                          it too is installed by the call to dce_svc_define_filter().
*                          The machinery works as follows. The filter routine
*                          should be coded to make its decision(s) based on the
*                          settings of various static variables, which are also
*                          accessible to this control routine. Then by calling
*                          the latter with the correct arguments one can change
*                          these settings and hence the filter's operation.
*
*****/
void my_filter_control(arg_size, arg, status)
idl_long_int arg_size;
idl_byte *arg;
error_status_t *status;
{

	printf("          my_filter_control() called!\n");

	if (strncmp(arg, "Toggle", arg_size))
		return;
	else
	{
		filter_setting = (filter_setting == FALSE) ? TRUE : FALSE;
		if (filter_setting)
			printf("     FILTER IS TURNED ON\n");
		else
			printf("     FILTER IS TURNED OFF\n");
	}

	return;

}


/*****
*
* install_filters-- calls dce_svc_define_filter() to install the above 2 routines.
*
*****/
void install_filters()
{
	unsigned32	status;

	filter_setting = TRUE;
	dce_svc_define_filter(tsv_svc_handle, my_filter, (dce_svc_filterctl_proc_t)my_filter_control, &status);
}


