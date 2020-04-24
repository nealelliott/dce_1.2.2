/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: union_server.c,v $
 * Revision 1.1.10.2  1996/03/09  20:51:45  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:29:27  marty]
 *
 * Revision 1.1.10.1  1995/12/13  21:56:05  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/13  21:45:30  root]
 * 
 * Revision 1.1.6.2  1993/07/09  13:45:24  root
 * 	Initial King Kong branch
 * 	[1993/07/09  13:44:57  root]
 * 
 * Revision 1.1.4.2  1993/03/31  12:42:52  mgm
 * 	Initial GAMERA branch
 * 
 * Revision 1.1.3.2  1992/06/11  20:00:16  mcfarland
 * 	changed files from test_latest
 * 	[1992/06/09  13:07:07  mcfarland]
 * 
 * 	Reorganize util routines
 * 	[1992/04/15  16:01:43  rps]
 * 
 * Revision 1.1.1.2  1992/05/28  15:15:16  root
 * 	    Loading drop Dce1_0_1b15
 * 
 * Revision 1.1  1992/01/19  14:37:05  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *
 *   FILE_NAME: union_server.c 
 *
 *   COMPONENT_NAME: dce.systest
 *
 *   FUNCTIONS: process_command_line_arguments
 *              show_usage
 *              get_port
 *              get_and_show_bindings
 *              register_server
 *
 *   ORIGINS: 27
 *
 *
 *   (C) COPYRIGHT International Business Machines Corp. 1990
 *   All Rights Reserved
 *
 *
 *   FOR_TEST: dcerpid4
 *
 *   USAGE:
 *
 *      union_server -P protocol_sequence -C max_calls -h|H 
 *
 *      where:
 *          protocol_sequence is a valid protocol sequence.
 *              (default:  ncadg_ip_udp).
 *          max_calls is the number of RPC listener threads to create.
 *              (default:  10).
 *          -h|H shows detailed usage statement.
 *	    (e.g.  union_server -P ip -M 5)
 *
 *   DESCRIPTION:
 *	This is the general purpose portion of union_server.  It basically
 *	takes care of RPC/non-RPC specifics such as parsing command line
 *	options, registering interfaces with the RPC runtime library, 
 *	getting binding information, ports, etc.  It is linked with the 
 *	manager code to make a complete RPC server.
 *
 *   HISTORY:
 *      08/06/91        Gerald Cantor           Initial design/code.  Modified
 *						DEC IDL test sp_union.
 *						prologs.
 *	12/19/91	Gerald Cantor		Updated prologs.
 */
#include <stdio.h>
#include <fcntl.h>
#include <dce/nbase.h>
#include <dce/rpcbase.h>
#include <xx_lib.h>
#include <sp_union.h>
#include <util.h>
#include <dce/exc_handling.h>

/*
 * a few global variables.
 */
extern char header[512], buf[128];
char *protocol = "ncadg_ip_udp", *prog;
unsigned32 max_calls = 10;

main(argc, argv)
int argc;
char **argv;
{
	/*
	 * main's functions and variables.
	 */
	void process_command_line_arguments(), get_port(); 
	void get_and_show_bindings(), register_server();
	error_status_t st;

	cma_init();
	prog = argv[0];
	make_header(prog);
	process_command_line_arguments(argc, argv, &protocol, &max_calls);
	get_port(protocol);
	get_and_show_bindings();
	register_server();	

	TRY
	{
		rpc_server_listen(max_calls, &st);
	}
	CATCH_ALL
	{
		rpc_server_unregister_if(sp_union_v0_0_s_ifspec, NULL, &st);
		sprintf(buf, "Cannot unregister interface from RPC\n");
		check_status(st, rpc_s_ok, NOT_EQUAL, buf, FALSE, RPC);
	}
	ENDTRY;
	exit(0);
}

/*
 * Print a usage statement to the tester.
 */
void show_usage()
{
        sprintf(buf, "Usage:  %s [ OPTIONS ]", prog);
        xx_log(buf);
        xx_log("Where [ OPTIONS ] are one or more of the following:");
        xx_log(" -P <protocol>  protocol sequence to use");
        xx_log("        (default:  ncadg_ip_udp)");
        xx_log(" -C <max calls> max executor threads");
        xx_log("        (default:  10)");
        xx_log(" -H | -h show usage");
}

/*
 * process_command_line_arguments parses the parameters passed by the user
 * and assigns them to variables.
 */
void process_command_line_arguments(ac, av, proto, calls)
int ac;
char **av;
char **proto;
unsigned32 *calls;
{
	int c;
	extern char *optarg;

	while( ( c = getopt( ac, av, "P:C:Hh" ) ) != EOF )
	{
		switch(c)
		{
			case 'P':
				*proto = optarg;
				break;
			case 'C':
				*calls = atol(optarg);
				break;
			case 'H':
			case 'h':
				show_usage();
				exit(0);
			default:
				xx_error("Invalid flag\n");
				show_usage();
				exit(1);
		}
	}
}

/*
 * get_port obtains a port to listen for remote requests.  We use a well
 * known port here (port #9009).
 */
void get_port(prot)
char *prot;
{
	error_status_t status;
	
	rpc_server_use_protseq_if(prot, max_calls, sp_union_v0_0_s_ifspec, 
				  &status);
	sprintf(buf, "Cannot use protocol sequence %s\n", prot);
	check_status(status, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);
}

/*
 * get_and_show_bindings inquires on the bindings the server supports.  For
 * each of the bindings, it is converted to a string and parsed to obtain
 * the network address and endpoint information.
 */
void get_and_show_bindings()
{
	error_status_t status;
	char *string_binding;
	rpc_binding_vector_t *bind_vec;
	int i;

	rpc_server_inq_bindings(&bind_vec, &status);
	sprintf(buf, "Cannot inquire on bindings\n");
	check_status(status, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);

	sprintf(buf, "%s%s supports the following binding(s)", header, prog);
        xx_log(buf);
	for (i = 0; i < bind_vec->count; i++)
	{
		rpc_binding_to_string_binding(bind_vec->binding_h[i],
					     &string_binding,
					     &status);
		sprintf(buf, "Cannot convert binding #%d to string\n", i);
		check_status(status, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);

		printf("%s\n", string_binding);
	}
}

/*
 * register_server registers the interface with the RPC runtime library.
 */
void register_server()
{
	error_status_t status;

	rpc_server_register_if(sp_union_v0_0_s_ifspec,
				NULL,
				NULL,
				&status);
	sprintf(buf, "Cannot register server with RPC\n");
	check_status(status, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);
}
