/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: timop_svc_client.c,v $
 * Revision 1.1.4.2  1996/02/18  18:26:26  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:48:29  marty]
 *
 * Revision 1.1.4.1  1995/12/08  20:55:09  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  20:50:13  root]
 * 
 * Revision 1.1.2.1  1994/06/14  19:57:38  weir
 * 	DCE 1.1 Beta check-in
 * 	[1994/06/14  16:15:16  weir]
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
**	timop_svc_client.c
**
**	Client program for timop_svc interface.
*/

#include <dce/dce.h>
#include <pthread.h>
#include <time.h>
#include <dce/rpc.h>
#include <dce/dce_error.h>
#include <dce/utc.h>
#include <dce/service.h>
/*
#include <errno.h>
#include <stdio.h>
*/
#include "timop_svc.h"
#include "timop_svc_aux.h"
#include "timop_svc_client.h"


extern char *strcpy();
extern char *strcat();
extern char *strncat();


#ifdef _HPUX_SOURCE
#define srandom srand
#define random rand
#endif


long			Rand;		/* sum of random numbers */
int			Workers;	/* number of active worker threads */
pthread_mutex_t		Work_mutex;	/* guard access to Workers, Rand */
pthread_cond_t		Work_cond;	/* condition variable for Workers==0 */

pthread_mutex_t		report_mutex;
int			object_nr_G;

int routes_G[MAX_ROUTES];

int debug_routes_G[MAX_LEVELS];

int debug_levels_G[MAX_LEVELS];



/*
 *	main()
 *
 *	Get started, and main loop.
 */

int
main(
	int				argc,
	char				*argv[])
{
	int				server_num, nservers, ret, svc_ret;
	work_arg_t			work_arg[MAX_SERVERS];
	unsigned_char_t			*server_name[MAX_SERVERS],
	                                    *string_binding, *svc_string_binding,
					    *protseq, *svc_protseq;
	rpc_binding_handle_t		bind_handle[MAX_SERVERS];
	rpc_binding_handle_t		svc_bind_handle[MAX_SERVERS];
	unsigned32			status;
	utc_t				start_utc, stop_utc, span_utc;
	struct tm			time_tm;
	uuid_t				obj_uuid[OBJECT_AMOUNT];
	rpc_ns_handle_t			import_context;
	rpc_ns_handle_t			svc_import_context;
	pthread_t			thread_id[MAX_SERVERS];

	unsigned_char_t *OBJ_UUIDS[OBJECT_AMOUNT + 1] = {"", OBJ_UUID_1, OBJ_UUID_2};

	int				command_error = FALSE;
	int				string_ix;
	int				object_ix;
	int				one_object;
	int				server_ix;
	int				STRING_BINDING = FALSE;
	int				i, ix;

	int			routes, debug_levels;
	boolean32		route_error, debug_error;
	idl_byte		*where;
	unsigned_char_t		component_string[] = "tsv";
	dce_svc_string_t	component;
	idl_long_int		arg_size = 6; /* length of the following... */
	idl_byte		argument[] = "Toggle";
	error_status_t		remote_status = rpc_s_ok;
	dce_svc_string_t	inquire;
	dce_svc_subcomparray_t	subcomponents_table;
	dce_svc_stringarray_t	inq_table;
	boolean32		debug_values = 0; /* I don't really know what this means;     */
						  /*  setting it to 0 gets me the routes,     */
						  /*  setting it to 1 gets me an empty table. */


	/* Set all the command line flags...                       */
	if (command_error = process_args(argc, argv, FALSE, 0))
	{
		fprintf(stdout, "\n Usage:\n");
		fprintf(stdout, "       %s -o<nr> [ -o<nr> ... ] <server_entry> [ <server_entry> ... ] \\ \n", argv[0]);
		fprintf(stdout, "       [-D\"<dbg_level>\" [-D\"<dbg_level>\" ... ]] [-d\"<dbg_route>\" [-d\"<dbg_route>\" ... ]] \\ \n");
		fprintf(stdout, "       [-w<svc_route> [-w<svc_route> ... ]] [-l] [-C] [-R] [-f]\n");
		fprintf(stdout, "  Or\n");
		fprintf(stdout, "       %s -o<nr> -b\"<string_binding>\" \\ \n", argv[0]);
		fprintf(stdout, "       [-D\"<dbg_level>\" [-D\"<dbg_level>\" ... ]] [-d\"<dbg_route>\" [-d\"<dbg_route>\" ... ]] \\ \n");
		fprintf(stdout, "       [-w<svc_route> [-w<svc_route> ... ]] [-l] [-C] [-R] [-f]\n\n");
		fprintf(stdout, "  Where:\n");
		fprintf(stdout, "        -o1 or -o2         Specifies the server object to bind to. There can be\n");
		fprintf(stdout, "                           up to %d objects specified.\n", OBJECT_AMOUNT);
		fprintf(stdout, "        <server_entry>     Specifies the name of the entry (of the server) to bind to;\n");
		fprintf(stdout, "                           there can be up to %d <server_entry>s specified.\n", MAX_SERVERS);
		fprintf(stdout, "                           NOTE: If you specify multiple servers and objects, the list of\n");
		fprintf(stdout, "                                 servers and the list of objects must ordinally match.\n");
		fprintf(stdout, "        <string_binding>   Specifies a complete binding to use to make direct contact\n");
		fprintf(stdout, "                           with the server. Multiple servers cannot be specified with\n");
		fprintf(stdout, "                           this option, and specifying a <server_entry> with it is\n");
		fprintf(stdout, "                           an error.\n");
		fprintf(stdout, "        -D\"<dbg_level>\"    (optional, one or more) Specifies a serviceability debug\n");
		fprintf(stdout, "                           level. For example:\n");
		fprintf(stdout, "                               -D\"tsv:tsv_s_server.5,tsv_s_refmon.9\"\n");
		fprintf(stdout, "                           or:\n");
		fprintf(stdout, "                               -D\"tsv:*.9\"\n");
		fprintf(stdout, "        -d\"<dbg_route>\"    (optional, one or more) Specifies a serviceability debug\n");
		fprintf(stdout, "                           routing. For example:\n");
		fprintf(stdout, "                               -d\"tsv:tsv_s_server.5:TEXTFILE:<pathname>\"\n");
		fprintf(stdout, "                           or:\n");
		fprintf(stdout, "                               -d\"tsv:*.8:STDERR:\"\n");
		fprintf(stdout, "        -w<svc_route>      (optional, one or more) Specifies a remote serviceability routing.\n");
		fprintf(stdout, "        -l                 Specifies that the serviceability subcomponents be listed.\n");
		fprintf(stdout, "        -C                 Specifies that all registered serviceability components be listed.\n");
		fprintf(stdout, "        -R                 Specifies that the serviceability routings be listed.\n");
		fprintf(stdout, "        -f                 Specifies that the remote serviceability filter routine should\n");
		fprintf(stdout, "                           be toggled.\n");

		/* N.B. There should be a default mode whereby the same UUID is used for all servers, if no  */
		/*       objects are specified on the command line. This would be for multi-machine mode.    */
		
		exit(1);
	}

	fprintf(stdout, "\n");

	/* Initialize "well-known" timop_svc object UUID...            */
	/* We have to do this whether we're using NSI or a string  */
	/*  binding...                                             */

	/* Check for object binding option...                      */
	object_nr_G = process_args(argc, argv, DO_OBJECT_NR, 0);

	/* Check for number of servers specified...                */
	nservers = process_args(argc, argv, DO_EXTRA_SERVERS, 0);

	if (object_nr_G > OBJECT_AMOUNT)
	{
		fprintf(stdout, " Timop_SVC Client: Bad object number\n");
		exit(1);
	}

	else if (object_nr_G >= 1)
	/* If objects were specified, load them in...  */
	{
		for (i = 1; i <= object_nr_G; i++)
		{
			one_object = process_args(argc, argv, DO_ONE_OBJECT, i);
			uuid_from_string(OBJ_UUIDS[one_object], &obj_uuid[i-1], &status);
			if (status != uuid_s_ok)
			{
				print_error("uuid_from_string()", status);
				exit(1);
			}
			fprintf(stdout, " Timop_SVC Client: Binding to object %d...\n", one_object);

		}
	}				

	else
	/* Default object is number 1...  */
	{
		/* Check for number of servers. Maybe we want to call multiple */
		/*  servers with the same (default) object UUID; or maybe we   */
		/*  haven't specified any servers at all, because we specified */
		/*  a string binding instead...                                */
		ix = (nservers > 0) ? nservers : 1;
		for (i = 1; i <= ix; i++)
		{
			uuid_from_string(OBJ_UUID_1, &obj_uuid[i-1], &status);
			if (status != uuid_s_ok)
			{
				print_error("uuid_from_string()", status);
				exit(1);
			}
			fprintf(stdout, " Timop_SVC Client: Binding to object 1...\n");
		}
	}



	/******************************************************************************
	* String binding code.
	* 
	* The following section allows the client to bind directly to the server using
	*     a string binding typed on the command line immediately following a "-b"
	*     flag, e.g.:
	* 
	*           timop_svc_client -b"ncacn_ip_tcp:130.105.1.33[4727]"
	* 
	* (NOTE that client has to be invoked with quotes around the string binding,
	*      because of the brackets.)
	* 
	* Of course, how the client gets the string is its own affair. If a string
	*  binding is entered as described above, then the client will skip the NSI
	*  import steps entirely and call straight through with the binding. Note that
	*  timop_svc's object UUID must be inserted into the converted handle, because the
	*  server registers its endpoints using that object UUID; if it's missing from
	*  the binding, rpcd will reject the incoming RPC.
	* 
	******************************************************************************/


	if (string_ix = process_args(argc, argv, DO_STRING_BINDING, 0))
	{
		fprintf(stdout, " Timop_SVC Client: Recognized string binding option...\n");
		STRING_BINDING = TRUE;
		nservers = 1;

		/* Convert the string to a real binding...  */
		rpc_binding_from_string_binding((unsigned_char_t *)&argv[string_ix][2], &bind_handle[0], &status);
		if (status != rpc_s_ok)
		{
			print_error("rpc_binding_from_string_binding()", status);
			exit(1);
		}
		/* Use the same one for serviceability... */
		rpc_binding_from_string_binding((unsigned_char_t *)&argv[string_ix][2], &svc_bind_handle[0], &status);
		if (status != rpc_s_ok)
		{
			print_error("rpc_binding_from_string_binding(svc)", status);
			exit(1);
		}

		/* Insert timop_svc's "well-known" object UUID into the handles...  */
		rpc_binding_set_object(bind_handle[0], &obj_uuid[0], &status);
		if (status != rpc_s_ok)
		{
			print_error("rpc_binding_set_object()", status);
			exit(1);
		}
		rpc_binding_set_object(svc_bind_handle[0], &obj_uuid[0], &status);
		if (status != rpc_s_ok)
		{
			print_error("rpc_binding_set_object(svc)", status);
			exit(1);
		}

		/* Now convert the things back to a string and print them, to  */
		/*  show that we got them right...                             */
		rpc_binding_to_string_binding(bind_handle[0], &string_binding, &status);
		if (status != rpc_s_ok)
		{
			print_error("rpc_binding_to_string_binding()", status);
			exit(1);
		}
		fprintf(stdout, " Timop_SVC Client: Reconverted string binding == %s\n", string_binding);
		rpc_binding_to_string_binding(svc_bind_handle[0], &svc_string_binding, &status);
		if (status != rpc_s_ok)
		{
			print_error("rpc_binding_to_string_binding(svc)", status);
			exit(1);
		}
		fprintf(stdout, " Timop_SVC Client: Reconverted string binding == %s\n", svc_string_binding);

		/* Now free the string memory...  */
		rpc_string_free(&string_binding, &status);
		if (status != rpc_s_ok)
		{
			print_error("rpc_string_free()", status);
			exit(1);
		}
		rpc_string_free(&svc_string_binding, &status);
		if (status != rpc_s_ok)
		{
			print_error("rpc_string_free()", status);
			exit(1);
		}

		fprintf(stdout, " Timop_SVC Client: Skipping import step...\n");

		/* Insert a string (showing that we bound to a string binding rather  */
		/*  than to an entry name) into the array member that will be used    */
		/*  to identify the server in the report that will be printed below,  */
		/*  by print_report(). This routine is called both by do_work() and   */
		/*  from the while loop below in this routine; the latter prints the  */
		/*  client's value, the former prints the server's value (from the    */
		/*  RPC...                                                            */
		server_name[0] = malloc(20);
		strcpy(server_name[0], "(Bound by string)");
		goto skip_import;

	}


	else  /* i.e., no string binding... */
	/* Get the server entry names...    */
	{
		server_ix = process_args(argc, argv, DO_SERVER_IX, 0);
		for (server_num = 0; server_num < nservers; server_num += 1)
			server_name[server_num] = (unsigned_char_t *)argv[server_num+server_ix];

	}


	/* Import binding info from namespace... */
	for (server_num = 0; server_num < nservers; server_num += 1)
	{
		/* Begin the binding import loop. */
		rpc_ns_binding_import_begin(rpc_c_ns_syntax_dce,
		    server_name[server_num], timop_svc_v1_0_c_ifspec,
		    &obj_uuid[server_num], &import_context, &status);

		if (status != rpc_s_ok)
		{
			print_error("rpc_ns_binding_import_begin()", status);
			exit(1);
		}

		/* Begin the svc binding import loop. */
		rpc_ns_binding_import_begin(rpc_c_ns_syntax_dce,
		    server_name[server_num], NULL,
		    &obj_uuid[server_num], &svc_import_context, &status);
		if (status != rpc_s_ok)
		{
			print_error("rpc_ns_binding_import_begin(svc)", status);
			exit(1);
		}


		/* Import bindings one at a time... */
		ret = svc_ret = 1;
		while (1)
		{
                        fprintf(stdout, " Timop_SVC Client: Importing...\n");

			if (ret != 0)
			{
				rpc_ns_binding_import_next(import_context,
				    &bind_handle[server_num], &status);
				if (status != rpc_s_ok)
				{
					print_error("rpc_ns_binding_import_next()", status);
					exit(1);
				}
				/* Select, say, the first binding over UDP. */
				rpc_binding_to_string_binding(bind_handle[server_num],
				    &string_binding, &status);
				if (status != rpc_s_ok)
				{
					print_error("rpc_binding_to_string_binding()", status);
					exit(1);
				}

				fprintf(stdout, " Timop_SVC Client: Imported stringized partial binding     == %s\n", string_binding);
				rpc_string_binding_parse(string_binding, NULL,
				    &protseq, NULL, NULL, NULL, &status);
				if (status != rpc_s_ok)
				{
					print_error("rpc_string_binding_parse()", status);
					exit(1);
				}
				rpc_string_free(&string_binding, &status);
				ret = strcmp((char *)protseq, "ncadg_ip_udp");
		        	rpc_string_free(&protseq, &status);
			}

			if (svc_ret != 0)
			{
				rpc_ns_binding_import_next(svc_import_context,
				    &svc_bind_handle[server_num], &status);
				if (status != rpc_s_ok) 
				{
					print_error("rpc_ns_binding_import_next(svc)", status);
					exit(1);
				}
				rpc_binding_to_string_binding(svc_bind_handle[server_num],
				    &svc_string_binding, &status);
				if (status != rpc_s_ok)
				{
					print_error("rpc_binding_to_string_binding(svc)", status);
					exit(1);
				}

				fprintf(stdout, " Timop_SVC Client: Imported stringized partial svc binding == %s\n", svc_string_binding);
				rpc_string_binding_parse(svc_string_binding, NULL,
				    &svc_protseq, NULL, NULL, NULL, &status);
				if (status != rpc_s_ok)
				{
					print_error("rpc_string_binding_parse(svc)", status);
					exit(1);
				}

				rpc_string_free(&svc_string_binding, &status);
				svc_ret = strcmp((char *)svc_protseq, "ncadg_ip_udp");
		        	rpc_string_free(&svc_protseq, &status);
			}

			if ((ret == 0) && (svc_ret == 0))
			{
		                break;
			}
		}

		/* End the binding import loop. */
		rpc_ns_binding_import_done(&import_context, &status);
		if (status != rpc_s_ok)
		{
			print_error("rpc_ns_binding_import_done()", status);
			exit(1);
		}
		rpc_ns_binding_import_done(&svc_import_context, &status);
		if (status != rpc_s_ok)
		{
			print_error("rpc_ns_binding_import_done(svc)", status);
			exit(1);
		}

	}

skip_import:

	/* Annotate binding handles for security. */
	for (server_num = 0; server_num < nservers; server_num += 1)
	{
		rpc_binding_set_auth_info(bind_handle[server_num],
		    SERVER_PRINC_NAME, rpc_c_protect_level_pkt_integ,
		    rpc_c_authn_dce_secret, NULL /*default login context*/,
		    rpc_c_authz_name, &status);
		if (status != rpc_s_ok)
		{
			print_error("rpc_binding_set_auth_info()", status);
			exit(1);
		}
	}

	for (server_num = 0; server_num < nservers; server_num += 1)
	{
		rpc_binding_set_auth_info(svc_bind_handle[server_num],
		    SERVER_PRINC_NAME, rpc_c_protect_level_pkt_integ,
		    rpc_c_authn_dce_secret, NULL /*default login context*/,
		    rpc_c_authz_name, &status);
		if (status != rpc_s_ok)
		{
			print_error("rpc_binding_set_auth_info(svc)", status);
			exit(1);
		}

	}

	/* Toggle the serviceability filter, if specified...  */
	if (process_args(argc, argv, DO_FILTER_TOGGLE))
	{
		component = malloc(sizeof(component_string));
		strcpy(component, component_string);

		/* NOTE THE NAME... NOT "dce_svc_filter_ctl()"...    */
		/* Note that the 3rd arg is the length of the 4th... */
		dce_svc_filter_control(svc_bind_handle[0], component, arg_size, argument, &remote_status);
		if (remote_status != rpc_s_ok)
		{
			fprintf(stdout, " Timop_SVC Client: dce_svc_filter_control() failed\n");
			exit(1);
		}
	}

	/* Get routings, if specified... */
	if (process_args(argc, argv, GET_ROUTE_LIST))
	{
		inquire = malloc(80);
		inq_table.tab_size = 0; /* number of strings in following string array... */
		inq_table.table = &inquire;

		dce_svc_inq_routings(svc_bind_handle[0], debug_values, &inq_table, &remote_status);
		if (remote_status != rpc_s_ok)
		{
			fprintf(stdout, " Timop_SVC Client: dce_svc_inq_routings() failed\n");
			exit(1);
		}
		fprintf(stdout, "       Route table size received is: %d...\n", inq_table.tab_size);
		fprintf(stdout, "       Route table contents are:\n");
		for (i = 0; i < inq_table.tab_size; i++)
			fprintf(stdout, "          %s\n", inq_table.table[i]);
	}

	/* Get subcomponent list, if specified... */
	if (process_args(argc, argv, GET_SUBCOMPONENTS))
	{
		/* NOTE: see service.idl for a description of the structures  */
		/*  used. Basically the subcomponents table that's returned   */
		/*  contains an array of table structures; all of its fields  */
		/*  are printed in the code below...                          */

		component = malloc(sizeof(component_string));
		strcpy(component, component_string);

		dce_svc_inq_table(svc_bind_handle[0], component, &subcomponents_table, &remote_status);
		if (remote_status != rpc_s_ok)
		{
			fprintf(stdout, " Timop_SVC Client: dce_svc_inq_table() failed\n");
			exit(1);
		}
		fprintf(stdout, "       Subcomponent table size received is: %d...\n", subcomponents_table.tab_size);
		fprintf(stdout, "       Subcomponent table contents are:\n");
		for (i = 0; i < subcomponents_table.tab_size; i++)
		{
			fprintf(stdout, "          Name: %s\n", subcomponents_table.table[i].sc_name);
			fprintf(stdout, "          Desc: %s\n", subcomponents_table.table[i].sc_descr);
			fprintf(stdout, "          Msg Cat ID: %x\n", subcomponents_table.table[i].sc_descr_msgid);
			fprintf(stdout, "          Active debug level: %d\n\n", subcomponents_table.table[i].sc_level);
		}
	}

	/* Get component list, if specified... */
	if (process_args(argc, argv, GET_COMPONENTS))
	{
		dce_svc_inq_components(svc_bind_handle[0], &inq_table, &remote_status);
		if (remote_status != rpc_s_ok)
		{
			fprintf(stdout, " Timop_SVC Client: dce_svc_inq_components() failed\n");
			exit(1);
		}
		fprintf(stdout, "       Component table size received is: %d...\n", inq_table.tab_size);
		fprintf(stdout, "       Component table contents are:\n");
		for (i = 0; i < inq_table.tab_size; i++)
			fprintf(stdout, "          %s\n", inq_table.table[i]);
	}

	/* Set routings, if specified... */
	routes = process_args(argc, argv, SET_ROUTES);
	route_error = FALSE;
	if (routes)
	{
		where = malloc(80);
		fprintf(stdout, "       Setting routes...\n");
		fprintf(stdout, "          Number of routes specified is %d\n", routes);
		fprintf(stdout, "          Route index numbers and strings are...\n");

		for (i = 0; (i < routes) && (!route_error); i++)
		{
			fprintf(stdout, "            %d   ", routes_G[i]);
			fprintf(stdout, "%s\n", (char *) (argv[routes_G[i]]+2));
			dce_svc_set_route(svc_bind_handle[0], (idl_byte *) (argv[routes_G[i]]+2), &remote_status);
			if (remote_status != rpc_s_ok)
			{
				fprintf(stdout, " Timop_SVC Client: dce_svc_set_route() failed\n");
				route_error = TRUE;
			}
		}

		fprintf(stdout, "       Checking routes...\n");
		dce_svc_inq_routings(svc_bind_handle[0], debug_values, &inq_table, &remote_status);
		if (remote_status != rpc_s_ok)
		{
			fprintf(stdout, " Timop_SVC Client: dce_svc_inq_routings() failed\n");
			exit(1);
		}
		fprintf(stdout, "       Route table size received is: %d...\n", inq_table.tab_size);
		fprintf(stdout, "       Route table contents are:\n");
		for (i = 0; i < inq_table.tab_size; i++)
			fprintf(stdout, "          %s\n", inq_table.table[i]);
	}

	/* Set debug routings, if specified... */
	routes = process_args(argc, argv, SET_DEBUG_ROUTES);
	route_error = FALSE;
	if (routes)
	{
		where = malloc(80);
		fprintf(stdout, "       Setting debug routes...\n");
		fprintf(stdout, "          Number of debug routes specified is %d\n", routes);
		fprintf(stdout, "          Debug route index numbers and strings are...\n");

		for (i = 0; (i < routes) && (!route_error); i++)
		{
			fprintf(stdout, "            %d   ", debug_routes_G[i]);
			fprintf(stdout, "%s\n", (char *) (argv[debug_routes_G[i]]+2));
			dce_svc_set_dbg_route(svc_bind_handle[0], (idl_byte *) (argv[debug_routes_G[i]]+2), &remote_status);
			if (remote_status != rpc_s_ok)
			{
				fprintf(stdout, " Timop_SVC Client: dce_svc_set_dbg_route() failed\n");
				route_error = TRUE;
			}
		}

		fprintf(stdout, "       Checking debug routes...\n");
		dce_svc_inq_routings(svc_bind_handle[0], debug_values = TRUE, &inq_table, &remote_status);
		if (remote_status != rpc_s_ok)
		{
			fprintf(stdout, " Timop_SVC Client: dce_svc_inq_routings() failed\n");
			exit(1);
		}
		fprintf(stdout, "       Debug route table size received is: %d...\n", inq_table.tab_size);
		fprintf(stdout, "       Debug route table contents are:\n");
		for (i = 0; i < inq_table.tab_size; i++)
			fprintf(stdout, "          %s\n", inq_table.table[i]);
	}

	/* If debug levels were specified, set them... */
	debug_levels = process_args(argc, argv, SET_DEBUG_LEVELS);
	debug_error = FALSE;
	if (debug_levels)
	{
		fprintf(stdout, "          Number of debug levels specified is %d\n", debug_levels);
		fprintf(stdout, "          Debug level index numbers and strings are...\n");
		for (i = 0; (i < debug_levels) && (!debug_error); i++)
		{
			fprintf(stdout, "            %d   ", debug_levels_G[i]);
			fprintf(stdout, "%s\n", (char *) (argv[debug_levels_G[i]]+2));
			/* Set the debug level for all sub-components...                      */
/*
			dce_svc_set_dbg_levels((unsigned_char_t *)(argv[debug_levels_G[i]]+2), &status);
			if (remote_status != rpc_s_ok)
			{
				fprintf(stdout, " Timop_SVC Client: dce_svc_set_dbg_levels() failed\n");
				debug_error = TRUE;
			}
*/
		}
		fprintf(stdout, "       Checking debug levels and routes...\n");
		dce_svc_inq_routings(svc_bind_handle[0], debug_values = TRUE, &inq_table, &remote_status);
		if (remote_status != rpc_s_ok)
		{
			fprintf(stdout, " Timop_SVC Client: dce_svc_inq_routings() failed\n");
			exit(1);
		}
		fprintf(stdout, "       Debug route/level table size received is: %d...\n", inq_table.tab_size);
		fprintf(stdout, "       Debug route/level table contents are:\n");
		for (i = 0; i < inq_table.tab_size; i++)
			fprintf(stdout, "          %s\n", inq_table.table[i]);

	}

	/* Initialize mutex and condition variable. */
	ret = pthread_mutex_init(&Work_mutex, pthread_mutexattr_default);
	if (ret == -1)
	{
		fprintf(stdout, " Timop_SVC Client: pthread_mutex_init() failed\n");
		fprintf(stdout, "         errno==%d\n", errno);
		exit(1);
	}
	ret = pthread_cond_init(&Work_cond, pthread_condattr_default);

	if (ret == -1)
	{
		fprintf(stdout, " Timop_SVC Client: pthread_cond_init() failed\n");
		fprintf(stdout, "         errno==%d\n", errno);
		exit(1);
	}

	/* Initialize random number generator. */
#ifdef _HPUX_SOURCE
	srand(time(NULL));
#else
	srandom(time(NULL));
#endif

	/* Initialize work args that are constant throughout main loop. */
	for (server_num = 0; server_num < nservers; server_num += 1)
	{
		work_arg[server_num].server_num = server_num;
		work_arg[server_num].server_name = server_name[server_num];
		work_arg[server_num].bind_handle = bind_handle[server_num];
		work_arg[server_num].svc_bind_handle = svc_bind_handle[server_num];
		work_arg[server_num].object_ix = process_args(argc, argv, DO_ONE_OBJECT, server_num + 1);
	}

	/* Print out the year and date, just once. */
	ret = utc_gettime(&start_utc);

	if (ret == -1)
	{
		fprintf(stdout, " Timop_SVC Client: utc_gettime() failed\n");
		exit(1);
	}
	ret = utc_gmtime(&time_tm, NULL, NULL, NULL, &start_utc);

	if (ret == -1) {
		fprintf(stdout, " Timop_SVC Client: utc_gmtime() failed\n");
		exit(1);
	}
	fprintf(stdout, "\n %24.24s UTC (Z time zone)\n\n", asctime(&time_tm));

	ret = pthread_mutex_init(&report_mutex, pthread_mutexattr_default);
	if (ret == -1)
	{
		fprintf(stdout, " Timop_SVC Client: pthread_mutex_init() failed\n");
		fprintf(stdout, "         errno==%d\n", errno);
		exit(1);
	}

	/* Main loop -- never exits -- interrupt to quit. */
	while (1)
	{
		/* Per-loop initialization.  We're single-threaded here, so      */
		/*  locks and reentrant random number generator are unnecessary. */
		Rand = 0;
		Workers = nservers;
		for (server_num = 0; server_num < nservers; server_num += 1)
		{
#ifdef _HPUX_SOURCE
			work_arg[server_num].rand = rand()%MAX_RANDOM;
#else
			work_arg[server_num].rand = random()%MAX_RANDOM;
#endif
		}

		/* Get client's start timestamp. */
		ret = utc_gettime(&start_utc);
		if (ret == -1)
		{
			fprintf(stdout, " Timop_SVC Client: utc_gettime() failed\n");
			exit(1);
		}

		/* Spawn a worker thread for each server. */
		for (server_num = 0; server_num < nservers; server_num += 1)
		{
			ret = pthread_create(&thread_id[server_num],
			    pthread_attr_default, (pthread_startroutine_t)do_work,
			    (void *)&work_arg[server_num]);
			if (ret == -1)
			{
				fprintf(stdout, " Timop_SVC Client: pthread_create() %d failed\n",
					server_num);
				exit(1);
			}
		}

		/* Reap the worker threads; pthread_cond_wait() semantics  */
		/*   requires it to be coded this way.                     */
		ret = pthread_mutex_lock(&Work_mutex);
		if (ret == -1)
		{
			fprintf(stdout, " Timop_SVC Client: pthread_mutex_lock() failed\n");
			fprintf(stdout, "         errno==%d\n", errno);
			exit(1);
		}
		while (Workers != 0)
		{
			ret = pthread_cond_wait(&Work_cond, &Work_mutex);
			if (ret == -1)
			{
				fprintf(stdout, " Timop_SVC Client: pthread_cond_wait() failed\n");
				fprintf(stdout, "         errno==%d\n", errno);
				exit(1);
			}
		}
		ret = pthread_mutex_unlock(&Work_mutex);
		if (ret == -1)
		{
			fprintf(stdout, " Timop_SVC Client: pthread_mutex_unlock() failed\n");
			fprintf(stdout, "         errno==%d\n", errno);
			exit(1);
		}

		/* Reclaim storage. */
		for (server_num = 0; server_num < nservers; server_num += 1)
		{
			ret = pthread_detach(&thread_id[server_num]);
			if (ret == -1)
			{
				fprintf(stdout, " Timop_SVC Client: pthread_detach() failed\n");
				fprintf(stdout, "         errno==%d\n", errno);
				exit(1);
			}
		}

		/* Any failures? */
		for (server_num = 0; server_num < nservers; server_num += 1)
		{
			if (work_arg[server_num].status != DO_WORK_OK)
			{
				exit(1);
			}
		}


		/* Get client's stop timestamp. */
		ret = utc_gettime(&stop_utc);
		if (ret == -1)
		{
			fprintf(stdout, " Timop_SVC Client: utc_gettime() failed\n");
			exit(1);
		}

		/* Calculate the span of client's start and stop timestamps. */
		ret = utc_spantime(&span_utc, &start_utc, &stop_utc);
		if (ret == -1)
		{
			fprintf(stdout, " Timop_SVC Client: utc_spantime() failed\n");
			exit(1);
		}

		/* Print total results. */
		fprintf(stdout, "\n Timop_SVC Client: Final Report:\n");
		print_report((unsigned_char_t *)"(client)", CLIENT_NUM,
				&span_utc, Rand);


	}

	/* Not reached. */
}


/*
 *	do_work()
 *
 *	Do the work.  This is done in parallel threads, so we want it
 *	(and the subroutine print_report() that it calls) to be reentrant.
 */

void
do_work(
	work_arg_t			*work_arg_p)
{
	int				server_num, *status_p, object_ix, ret;
	unsigned_char_t			*server_name;
	rpc_binding_handle_t		bind_handle;
	rpc_binding_handle_t		svc_bind_handle;
	idl_long_int			rand, status;
	error_status_t			remote_status = rpc_s_ok;
	timestamp_t			timestamp;
	idl_char			server_msg[MESSAGE_SIZE], *s_p;
	unsigned_char_t			*string_binding, *svc_string_binding, *s;
	int				call_nr;
	char				call_nr_string[10];
	char				object_string[20];

	/* Unpackage the args into local variables. */
	server_num = work_arg_p->server_num;
	server_name = work_arg_p->server_name;
	bind_handle = work_arg_p->bind_handle;
	svc_bind_handle = work_arg_p->svc_bind_handle;
	rand = work_arg_p->rand;
	status_p = &work_arg_p->status;
	object_ix = work_arg_p->object_ix;
	work_arg_p->call_nr = work_arg_p->call_nr + 1;
	call_nr = work_arg_p->call_nr;

	rpc_binding_to_string_binding(bind_handle, &string_binding, (unsigned32 *)&status);
	if (status != rpc_s_ok)
	{
		print_error("rpc_binding_to_string_binding()", status);
		pthread_exit(NULL);
	}
	rpc_binding_to_string_binding(svc_bind_handle, &svc_string_binding, (unsigned32 *)&status);
	if (status != rpc_s_ok)
	{
		print_error("rpc_binding_to_string_binding(svc)", status);
		pthread_exit(NULL);
	}

	/* Check what kind of bindings we have. If we're using string binding,        */
	/*  then using_string will be TRUE and will remain so throughout. But if      */
	/*  we're using imported (partial) bindings, then we'll begin with partial    */
	/*  bindings but, as soon as we've made contact with timop_svc_server,        */
	/*  they will become full bindings. We can check which kind we have by        */
	/*  scanning the endpoint fields to see if they're empty or not. All of this  */
	/*  is done just so we can print an informative message...                    */
	/* NOTE: Now that we're making remote serviceability calls in the initial     */
	/*       part of the program, we may not have partial bindings sometimes      */
	/*       at this point when we think we ought to have them...                 */
	for (s = string_binding; (*s != '\0') && (*s != '['); s++)
		;
	if (*s == '\0') /* we never found an endpoint field  */
	{
		fprintf(stdout, " Timop_SVC Client: Bad string binding detected\n");
		pthread_exit(NULL);
	}
	else if (*++s != ']')  /* i.e., there is an endpoint, so this is a full binding */
	{
		fprintf(stdout, " Timop_SVC Client: Stringized full binding about to be used      == %s\n", string_binding);
	}
	else /* i.e., there is no endpoint  */
	{
		fprintf(stdout, " Timop_SVC Client: Stringized partial binding about to be used   == %s\n", string_binding);
	}

	for (s = svc_string_binding; (*s != '\0') && (*s != '['); s++)
		;
	if (*s == '\0') /* we never found an endpoint field  */
	{
		fprintf(stdout, " Timop_SVC Client: Bad SVC string binding detected\n");
		pthread_exit(NULL);
	}
	else if (*++s != ']')  /* i.e., there is an endpoint, so this is a full binding */
	{
		fprintf(stdout, " Timop_SVC Client: Stringized full svc binding                   == %s\n", svc_string_binding);
	}
	else /* i.e., there is no endpoint  */
	{
		fprintf(stdout, " Timop_SVC Client: Stringized partial svc binding                == %s\n", svc_string_binding);
	}

	rpc_string_free(&string_binding, (unsigned32 *)&status);
	if (status != rpc_s_ok)
	{
		print_error("rpc_string_free()", status);
		pthread_exit(NULL);
	}
	rpc_string_free(&svc_string_binding, (unsigned32 *)&status);
	if (status != rpc_s_ok)
	{
		print_error("rpc_string_free(svc)", status);
		pthread_exit(NULL);
	}

	s_p = (idl_char *)server_msg;
	strcpy(server_msg, " Timop_SVC Client: Calling ");
	sprintf(call_nr_string, "(%d) ", call_nr);
	strncat(s_p, call_nr_string, strlen(call_nr_string));

	sprintf(object_string, "object %d... ", object_ix);
	strncat(s_p, object_string, strlen(object_string));

	fprintf(stdout, "%s\n", server_msg);

	/* Do the RPC! */
	timop_svc_getspan(bind_handle, rand, server_msg, timestamp, &status, &remote_status);
	if (remote_status != rpc_s_ok)
	{
		fprintf(stdout, " Timop_SVC Client: timop_svc_getspan() failed\n");
		*status_p = DO_WORK_ERR;
		pthread_exit(NULL);
		/* Not reached. */
	}

	if (status != rand)
	{
		fprintf(stdout, " Timop_SVC Client: do_work() failed somehow\n");
		*status_p = DO_WORK_ERR;
		pthread_exit(NULL);
		/* Not reached. */
	}

	fprintf(stdout, " Timop_SVC Client: Received from: %s", server_msg);

	/* Print report.  Not a critical section here because print_report()  */
	/*   is supposed to be implemented to be reentrant.                   */
	print_report(server_name, server_num, (utc_t *)timestamp, rand);

	/* Update Rand and decrement Workers.  As implemented, it is a  */
	/*   critical section, so must be locked.                       */
	ret = pthread_mutex_lock(&Work_mutex);
	if (ret == -1)
	{
		fprintf(stdout, " Timop_SVC Client: pthread_mutex_lock() failed\n");
		fprintf(stdout, "         errno==%d\n", errno);
		exit(1);
	}
	Workers -= 1;

	if (Workers == 0)
	{
		/* Last worker signals main thread. */
		ret = pthread_cond_signal(&Work_cond);
		if (ret == -1)
		{
			fprintf(stdout, " Timop_SVC Client: pthread_cond_signal() failed\n");
			fprintf(stdout, "         errno==%d\n", errno);
			exit(1);
		}
	}
	Rand += rand;
	ret = pthread_mutex_unlock(&Work_mutex);
	if (ret == -1)
	{
		fprintf(stdout, " Timop_SVC Client: pthread_mutex_unlock() failed\n");
		fprintf(stdout, "         errno==%d\n", errno);
		exit(1);
	}

	/* Done. */
	*status_p = DO_WORK_OK;
	pthread_exit(NULL);
	/* Not reached. */
}


/*
 *	print_report()
 *
 *	Print DTS timestamp interval, to millisecond granularity.
 *	Implemented this way so it is reentrant (assuming all the underlying
 *	OS subroutines it calls are reentrant).
 *	This kind of timestamp manipulation is always messy -- see the
 *	manual for the formats of structures and print-strings we use.
 */

void
print_report(
	unsigned_char_t		*server_name,
	int			server_num,
	utc_t			*utc_p,
	long			rand)
{
#define LINE_LEN		78
#define COL1			0
#define COL2			44
#define COL3a			47
#define COL3b			60
#define COL4			70
	char			asctime_buf[26], ascinacc_buf[26],
				    time_ns_buf[10], inacc_ns_buf[10],
				    report[LINE_LEN+3];
	int			inacc_sec, ret;
	long			time_ns, inacc_ns;
	struct tm		time_tm, inacc_tm;


	/* Print server_name into report.  Pad or truncate as necessary. */
	sprintf(report+COL1, "%*.*s  ", COL2-2, COL2-2, (char *)server_name);

	/* Print server_num into report. */
	if (server_num != CLIENT_NUM)
	{
		sprintf(report+COL2, "%1.1d  ", server_num + 1);
	}
	else
	{
		sprintf(report+COL2, "%1.1s  ", "*");
	}

	/* Format utc_p and print it into report. */
	ret = utc_gmtime(&time_tm, &time_ns, &inacc_tm, &inacc_ns, utc_p);
	if (ret == -1)
	{
		fprintf(stdout, " Timop_SVC Client: utc_gmtime() failed\n");
		exit(1);
	}


	ret = pthread_mutex_lock(&report_mutex);
	if (ret == -1)
	{
		fprintf(stdout, " Timop_SVC Client: pthread_mutex_lock() failed\n");
		fprintf(stdout, "         errno==%d\n", errno);
		exit(1);
	}
	memcpy(asctime_buf, asctime(&time_tm), 26);	/* reentrancy bug! */
	ret = pthread_mutex_unlock(&report_mutex);
	if (ret == -1)
	{
		fprintf(stdout, " Timop_SVC Client: pthread_mutex_unlock() failed\n");
		fprintf(stdout, "         errno==%d\n", errno);
		exit(1);
	}

	ret = pthread_mutex_lock(&report_mutex);
	if (ret == -1)
	{
		fprintf(stdout, " Timop_SVC Client: pthread_mutex_lock() failed\n");
		fprintf(stdout, "         errno==%d\n", errno);
		exit(1);
	}
	memcpy(ascinacc_buf, asctime(&inacc_tm), 26);	/* reentrancy bug! */
	ret = pthread_mutex_unlock(&report_mutex);
	if (ret == -1)
	{
		fprintf(stdout, " Timop_SVC Client: pthread_mutex_unlock() failed\n");
		fprintf(stdout, "         errno==%d\n", errno);
		exit(1);
	}

	sprintf(time_ns_buf, "%9.9d", time_ns);
	sprintf(inacc_ns_buf, "%9.9d", inacc_ns);
	inacc_sec = inacc_tm.tm_yday*24*60*60 + inacc_tm.tm_hour*60*60 +
	    inacc_tm.tm_min*60 + inacc_tm.tm_sec;
	sprintf(report+COL3a, "%8.8s.%3.3sI", asctime_buf+11,
	    time_ns_buf);
	if (inacc_tm.tm_year != -1)
	{
		sprintf(report+COL3b, "%4.4d.%3.3s  ", inacc_sec,
		    inacc_ns_buf);
	}
	else
	{
		sprintf(report+COL3b, "%8.8s  ", "infinity");
	}

	/* Print rand into report. */
	if (server_num != CLIENT_NUM)
	{
		sprintf(report+COL4, "%8d\n", rand);
	}
	else
	{
		sprintf(report+COL4, "%8d\n\n", rand);
	}

	/* Output report. */
	fprintf(stdout, "%s", report);
	return;
}





/*****
*
* process_args-- inspects command line arguments and sets internal flags
*                accordingly.
*
*                Must be called first with check==FALSE; this reads the
*                command line and sets the flags.
*                Thereafter, it can be called with check==<state_to_check>
*                (these are defined in timop_svc_aux.h). It will return either
*                a boolean or some sort of useable value, depending on
*                what's being checked.
*
*                Returns an error on first call if it detects anything
*                wrong with the command line arguments. Note that it
*                doesn't check validity of namespace entries. You'll
*                find out about those soon enough.
*
*****/
int
process_args(argc, argv, check, object_ix)
int argc;
char *argv[];
int check;        /* if FALSE, simply scan the command line; */
		  /*  if not FALSE, return the state of the  */
		  /*  flag/option specified by 'check'       */
int object_ix;
{                 
	int i, j, o;
	int error = FALSE;
	int string_requested = FALSE;
	static int server_ix = 1;    /* This will be used as an offset to an index */
                                     /*  into argv; the 1 will skip us over the    */
                                     /*  program name if no servers are specified  */
                                     /*  on the line. Otherwise, this variable     */
                                     /*  will point to the server name list.       */
	static int nservers = 0;
	static int string_bind_flag = FALSE;
	static int object_nr = 0;
	static int objects[OBJECT_AMOUNT + 1] = {0, 1, 0};  /* Note that this array starts at 1 */
	static int filter_toggle_flag = 0;
	static int routing_flag = FALSE;
	static int routes = 0;
	static int debug_flag = FALSE;
	static int debug_routes = 0;
	static int debug_levels_flag = FALSE;
	static int debug_levels = 0;
	static int get_route_list_flag = 0;
	static int components_flag = 0;
	static int subcomponents_flag = 0;

	if (!check)
	{
		if (argc)
		{
			if (argc < 2)
				error = TRUE;

			else for (i = 1; i < argc; i++)
			{
				/* server name */
				if (argv[i][0] != '-')
				{
					if (string_bind_flag)
						error = TRUE;
					else
					{
						if (!nservers)
							server_ix = i;
						nservers = nservers + 1;
					}
				}

				/* option flag */
				else if (argv[i][0] == '-')
				{
					switch (argv[i][1])
					{
						/* return index of parm string, so that */
						/*  caller can know where to extract    */
						/*  string binding from...              */
						/* Proper location will be argv[i][2]   */
						case 'b':
						/* Note that it is an error to specify  */
						/*  a string binding with a server en-  */
						/*  try...                              */
							if ((nservers) || (string_requested))
								error = TRUE;
							else							
								string_bind_flag = i;
							string_requested = TRUE;
							break;

						case 'o':
							object_nr++;
							o = 0;
							for (j = 2; isdigit(argv[i][j]); j++)
							{
								o = o + (10 * o + (argv[i][j] - '0'));
							}
							objects[object_nr] = o;
							break;

						case 'w':
							if (!isalpha(argv[i][2]))
{
fprintf(stdout, "No -w option value!\n");
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
fprintf(stdout, "No -d option value!\n");
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
fprintf(stdout, "No -D option value!\n");
								error = TRUE;
}
							else
							{
								debug_levels_flag = TRUE;
								debug_levels_G[debug_levels] = i;
								debug_levels++;
							}
							break;

						case 'f':
							filter_toggle_flag = TRUE;
							break;

						case 'C':
							components_flag = TRUE;
							break;

						case 'l':
							subcomponents_flag = TRUE;
							break;

						case 'R':
							get_route_list_flag = TRUE;
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
			case DO_STRING_BINDING:
				return string_bind_flag;
				break;

			case DO_OBJECT_NR:
				return object_nr;
				break;

			case DO_ONE_OBJECT:
				return objects[object_ix];
				break;

			case DO_FILTER_TOGGLE:
				return filter_toggle_flag;
				break;

			case DO_EXTRA_SERVERS:
				return nservers;
				break;

			case DO_SERVER_IX:
				return server_ix;
				break;

			case SET_ROUTES:
				return routes;
				break;

			case SET_DEBUG_ROUTES:
				return debug_routes;
				break;

			case SET_DEBUG_LEVELS:
				return debug_levels;
				break;

			case GET_COMPONENTS:
				return components_flag;
				break;

			case GET_SUBCOMPONENTS:
				return subcomponents_flag;
				break;

			case GET_ROUTE_LIST:
				return get_route_list_flag;
				break;

			default:
				break;

		}

	}

	if (nservers > MAX_SERVERS)
		error = TRUE;
	if (error)
		return error;
	else
		return NO_FLAG;

}


/*****
*
* print_error-- Client version. Prints text associated with bad status code.
*
*****/
void
print_error(caller, status)
char *caller;		/* a string identifying the routine that received the error  */
error_status_t status;	/* the status we want to print the message for               */
{
	dce_error_string_t error_string;
	int print_status;

	dce_error_inq_text(status, error_string, &print_status);
	fprintf(stdout," Timop_SVC Client: %s: %s\n", caller, error_string);
}


