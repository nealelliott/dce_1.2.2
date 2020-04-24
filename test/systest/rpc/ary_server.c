/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ary_server.c,v $
 * Revision 1.1.11.2  1996/03/09  20:50:52  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:28:52  marty]
 *
 * Revision 1.1.11.1  1995/12/13  21:55:31  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/13  21:45:08  root]
 * 
 * Revision 1.1.6.2  1993/07/09  12:22:35  root
 * 	Initial King Kong branch
 * 	[1993/07/09  12:22:09  root]
 * 
 * Revision 1.1.4.2  1993/03/31  12:24:45  mgm
 * 	Initial GAMERA branch
 * 
 * Revision 1.1.3.2  1992/06/11  19:53:34  mcfarland
 * 	changed files from test_latest
 * 	[1992/06/09  13:01:05  mcfarland]
 * 
 * Revision 1.1.1.2  1992/05/28  15:11:52  root
 * 	    Loading drop Dce1_0_1b15
 * 
 * Revision 1.1.3.3  1992/04/15  16:53:32  rps
 * 	Reorganize util routines
 * 	[1992/04/15  15:56:10  rps]
 * 
 * Revision 1.1.3.2  1992/03/18  01:19:30  rps
 * 	Fix error returns
 * 	[1992/03/18  00:12:01  rps]
 * 
 * Revision 1.1  1992/01/19  14:37:24  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *
 *
 *   FILE_NAME: ary_server.c 
 *
 *   COMPONENT_NAME: dce.systest
 *
 *   FUNCTIONS: show_usage
 *		process_command_line_arguments
 *              get_port
 *              get_and_show_bindings
 *              register_server
 *              ary_pass_array_kinds
 *
 *   ORIGINS: 27
 *
 *
 *   (C) COPYRIGHT International Business Machines Corp. 1990
 *   All Rights Reserved
 *
 *
 *   FOR_TEST: dcerpary
 *
 *   USAGE:
 *
 *      ary_server -P protocol_sequence -C max_calls -X endpoint -h|H 
 *
 *      where:
 *          protocol_sequence is a valid protocol sequence 
 *		(default:  ncadg_ip_udp).
 *	    max_calls is the number of RPC listener threads to create.
 *		(default:  10).
 *	    endpoint is the port number to obtain from RPC runtime library.
 *		(default:  5555).
 *	    -h|H shows detailed usage statement.
 *	    (e.g.  ary_server -P ncadg_ip_udp -C 5 -X 1044)
 *
 *   DESCRIPTION:
 *	This is the general purpose RPC server.  It takes care of 
 *	RPC/non-RPC specifics such as parsing command line options, 
 *	registering interfaces with the RPC runtime library, getting 
 *	binding information, ports, etc.  It containes the manager 
 *	function ary_pass_array_kinds, which simply takes the passed 
 *	structure of array of character arrays and an array of pointers
 *	to character strings and passes it to compare_text to verify that 
 *	the structure was properly sent across the network.
 *
 *   HISTORY:
 *      09/16/91        Gerald Cantor           Initial design/code.
 *	10/18/91	Gerald Cantor		Added sigset code to handle
 *						ctrl-C signal.
 *	10/18/91	Gerald Cantor		ANSIfied functions.  Updated 
 *						prologs and comments.
 *	10/19/91	Gerald Cantor		Updated prologs.
 *	11/13/91	Gerald Cantor		Converted rpc_server_use_...if
 *						to rpc_server_use_..._ep.
 *	12/20/91	Gerald Cantor		Changed -M option to -C.  Have
 *						ary_pass_array_kinds return
 *						void rather than error_status_t.
 */

#include <stdio.h>
#include <fcntl.h>
#include <pthread.h>
#include <dce/nbase.h>
#include <dce/rpcbase.h>
#include <dce/exc_handling.h>
#include <xx_lib.h>
#include <ary.h>
#include <ary_util.h>
#include <util.h>

/*
 * a few global variables.
 */
extern char header[512], buf[128];
char *protocol = "ncadg_ip_udp", *endpoint = "5555", *prog;
unsigned32 max_calls = 10;

main(int argc, char **argv)
{
	/*
	 * main's functions and variables.
	 */
	void process_command_line_arguments(int, char **, char **, 
					    unsigned32 *, char **);
	void get_port(char *, char *); 
	void get_and_show_bindings(void), register_server(void);

	pthread_t this_thread = pthread_self();
	sigset_t sigset;
	error_status_t st;

	cma_init();
	prog = argv[0];
	make_header(prog);
	process_command_line_arguments(argc, argv, &protocol, &max_calls,
				       &endpoint);
	get_port(protocol, endpoint);
	get_and_show_bindings();
	register_server();	

#ifdef aix31
	sigemptyset(&sigset);
        sigaddset(&sigset, SIGINT);
        if (pthread_signal_to_cancel_np(&sigset, &this_thread) != 0)
        {
                sprintf(buf, "%spthread_signal_to_cancel_np failed\n", header);
                xx_severe(buf);
                exit(1);
        }
#endif

	TRY
	{
		rpc_server_listen(max_calls, &st);
	}
	CATCH_ALL
	{
		rpc_server_unregister_if(ary_v1_0_s_ifspec, NULL, &st);
		sprintf(buf, "Cannot unregister interface from RPC\n");
		check_status(st, rpc_s_ok, NOT_EQUAL, buf, false, RPC);
	}
	ENDTRY;
	exit(0);
}

/*
 * Print usage statement for tester.
 */
void show_usage()
{
        sprintf(buf, "Usage:  %s [ OPTIONS ]", prog);
        xx_log(buf);
        xx_log("Where [ OPTIONS ] are one or more of the following:");
	xx_log(" -P <protocol> a valid protocol sequence");
	xx_log("\t(default: ncadg_ip_udp)");
	xx_log(" -C <max calls> max executor threads");
	xx_log("\t(default: 10)");
	xx_log(" -X <endpoint> port number to obtain from RPC");
	xx_log("\t(default: 5555)");
}

/*
 * process_command_line_arguments parses the parameters passed by the user
 * and assigns them to variables.
 */
void process_command_line_arguments(int ac, char **av, char **proto,
				    unsigned32 *calls, char **port)
{
	void show_usage(void);
	extern char *optarg;
	int c;

	while( ( c = getopt( ac, av, "P:C:X:Hh" ) ) != EOF )
	{
		switch(c)
		{
			case 'P':
				*proto = optarg;
				break;
			case 'C':
				*calls = atol(optarg);
				break;
			case 'X':
				*port = optarg;
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
 * get_port obtains a port to listen for remote requests.  We try to obtain
 * the port number the user specified.
 */
void get_port(char *prot, char *port)
{
	error_status_t status;
	
	rpc_server_use_protseq_ep(prot, max_calls, port, &status);
	sprintf(buf, "Cannot use protocol sequence %s\n", prot);
	check_status(status, rpc_s_ok, NOT_EQUAL, buf, true, RPC);
}

/*
 * get_and_show_bindings inquires on the bindings the server supports.  For
 * each of the bindings, it is converted to a string representation and 
 * displayed.
 */
void get_and_show_bindings()
{
	error_status_t status;
	char *string_binding;
	rpc_binding_vector_t *bind_vec;
	int i;

	rpc_server_inq_bindings(&bind_vec, &status);
	sprintf(buf, "Cannot inquire on bindings\n");
	check_status(status, rpc_s_ok, NOT_EQUAL, buf, true, RPC);

	sprintf(buf, "%s%s supports the following binding(s)", header, prog);
        xx_log(buf);
	for (i = 0; i < bind_vec->count; i++)
	{
		rpc_binding_to_string_binding(bind_vec->binding_h[i],
					     &string_binding,
					     &status);
		sprintf(buf, "Cannot convert binding #%d to string\n", i);
		check_status(status, rpc_s_ok, NOT_EQUAL, buf, true, RPC);

		printf("%s\n", string_binding);
	}
}

/*
 * register_server registers the interface with the RPC runtime library.
 */
void register_server()
{
	error_status_t status;

	rpc_server_register_if(ary_v1_0_s_ifspec,
				NULL,
				NULL,
				&status);
	sprintf(buf, "Cannot register server with RPC\n");
	check_status(status, rpc_s_ok, NOT_EQUAL, buf, true, RPC);
}

/*
 * ary_pass_array_kinds simply takes the structure received and passes
 * it to compare_text to ensure that the array of arrays and array of 
 * pointers were properly transmitted across the network.
 */
error_status_t ary_pass_array_kinds(handle_t h, array_groups_t *array_groups, 
				    boolean32 *validated)
{
        *validated = compare_text(array_groups);
}

