/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: scal_client.c,v $
 * Revision 1.1.10.2  1996/03/09  20:51:32  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:29:18  marty]
 *
 * Revision 1.1.10.1  1995/12/13  21:55:52  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/13  21:45:22  root]
 * 
 * Revision 1.1.6.2  1993/07/09  13:17:07  root
 * 	Initial King Kong branch
 * 	[1993/07/09  13:16:39  root]
 * 
 * Revision 1.1.4.2  1993/03/31  12:36:05  mgm
 * 	Initial GAMERA branch
 * 
 * Revision 1.1.3.2  1992/06/11  19:58:10  mcfarland
 * 	changed files from test_latest
 * 	[1992/06/09  13:05:10  mcfarland]
 * 
 * 	Reorganize util routines
 * 	[1992/04/15  16:01:02  rps]
 * 
 * 	Adding changes from defect_fixes archive.
 * 	[1992/01/22  22:32:04  melman]
 * 
 * Revision 1.1.1.2  1992/05/28  15:14:36  root
 * 	    Loading drop Dce1_0_1b15
 * 
 * $EndLog$
 */
/*
 *   FILE_NAME: scal_client.c 
 *
 *   COMPONENT_NAME: dce.systest
 *
 *   FUNCTIONS: show_usage
 *              get_command_line_parameters
 *
 *   ORIGINS: 27
 *
 *
 *   (C) COPYRIGHT International Business Machines Corp. 1990
 *   All Rights Reserved
 *
 *
 *   FOR_TEST: dcerpid3
 *
 *   USAGE:
 *
 *      scal_client -h hostname -X endpoint -P protocol sequence -H
 *
 *      where:
 *         hostname is the name of the machine scal_server is running on.
 *         protocol sequence is the same sequence scal_server was passed.
 *              (default:  ncadg_ip_udp).
 *         endpoint is the port number scal_server is listening on.
 *              (default:  6678).
 *         -H show detailed usage statement.
 *
 *	   (e.g.  scal_client -h kidney -X 6678 -P ncacn_ip_tcp).
 *
 *   DESCRIPTION:
 *      This is the general purpose client driver for the idl scal.
 *      All this file does is call process_command_line_arguments and calls
 *      scalars_test.  Scalars_test is the DEC IDL scal code.
 *
 *   HISTORY:
 *      11/10/90        Gerald Cantor           Initial design/code.  Modified
 *						DEC IDL test scal.
 *	11/08/91	Gerald Cantor 		Added @OSF_COPYRIGHT@.  Updated
 *						prologs.
 *	12/19/91	Gerald Cantor		Updated prologs.
 */
#include <stdio.h>
#include <math.h>
#include <dce/nbase.h>
#include <dce/rpcbase.h>
#include <dce/rpcexc.h>
#include <dce/rpc.h>
#include <xx_lib.h>
#include <scal.h>
#include <util.h>


/*
 * a few global functions and variables.
 */
void show_usage();
extern char header[512], buf[128];
char *prog;
ndr_long_int *myarray;
ndr_long_int mycount;


main(argc, argv)
int argc;
char *argv[];
{
	/*
	 * main's functions and variables.
	 */
	void get_command_line_parameters(), scals_test();
	char *hostname, *protocol = "ncadg_ip_udp", *endpoint = "6678"; 
	char *address;
	unsigned_char_p_t bound_string;
	error_status_t status;
	handle_t h;

	cma_init();
	prog = argv[0];
	make_header(argv[0]);
	get_command_line_parameters(argc, argv, &hostname, &protocol, 
				    &endpoint);
	address = addr_from_name(hostname);

	/*
    	 *  Do the compose of the string, followed by the binding from string.
   	 */ 
    	rpc_string_binding_compose(
       		(unsigned_char_p_t) "3fb4f09c-b000-0000-0d00-018ed9000000",
       		(unsigned_char_p_t)protocol, address, 
		(unsigned_char_p_t)endpoint, NULL, &bound_string, &status);
    	sprintf(buf, "(%s):  rpc_string_binding_compose failed", prog);
    	check_status(status, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);

    	rpc_binding_from_string_binding(bound_string, &h, &status);
    	sprintf(buf, "(%s):  rpc_string_binding_compose failed", prog);
    	check_status(status, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);

    	(void) scalars_test(h);
}

/*
 * Print usage statement to the tester.
 */
void show_usage()
{
        sprintf(buf, "Usage:  %s [ OPTIONS ]", prog);
        xx_log(buf);
        xx_log("Where [ OPTIONS ] are one or more of the following:");
        xx_log(" -h <hostname> name of the host where server is running");
        xx_log(" -P <protocol> valid protocol sequence");
        xx_log("        (default:  ncadg_ip_udp)");
        xx_log(" -X <endpoint> port the server is listening on");
        xx_log("        (default:  6678)");
        xx_log(" -H show usage");
}

/*
 * get_command_line_arguments parses the parameters passed by the user
 * and assigns them to variables.
 */
void get_command_line_parameters( argc, argv, host, prot, ep )
int     argc;
char   **argv;
char **host;
char **prot;
char **ep;
{
    int         c;
    extern char *optarg;


	while( ( c = getopt( argc, argv, "h:P:X:H" ) ) != EOF ) {
        switch( c ) {
		case 'h':
			*host = optarg;
			break;
		case 'P':
			*prot = optarg;
			break;
		case 'X':
			*ep = optarg;
			break;
		case 'H':
			show_usage();
			exit(0);
		default:
			sprintf(buf, "%sInvalid flag\n", header);
			xx_severe(buf);
			show_usage();
			exit(1);
	        }
	}
}	
