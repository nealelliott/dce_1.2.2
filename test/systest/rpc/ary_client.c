/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ary_client.c,v $
 * Revision 1.1.10.2  1996/03/09  20:50:50  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:28:51  marty]
 *
 * Revision 1.1.10.1  1995/12/13  21:55:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/13  21:45:07  root]
 * 
 * Revision 1.1.6.2  1993/07/09  12:21:29  root
 * 	Initial King Kong branch
 * 	[1993/07/09  12:21:02  root]
 * 
 * Revision 1.1.4.2  1993/03/31  12:24:21  mgm
 * 	Initial GAMERA branch
 * 
 * Revision 1.1.3.2  1992/06/11  19:53:27  mcfarland
 * 	changed files from test_latest
 * 	[1992/06/09  13:00:55  mcfarland]
 * 
 * 	Reorganize util routines
 * 	[1992/04/15  15:55:57  rps]
 * 
 * Revision 1.1.1.2  1992/05/28  15:11:50  root
 * 	    Loading drop Dce1_0_1b15
 * 
 * Revision 1.1  1992/01/19  14:37:25  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *
 *   FILE_NAME: ary_client.c 
 *
 *   COMPONENT_NAME: dce.systest
 *
 *   FUNCTIONS: show_usage
 *              get_command_line_parameters
 *              alloc_space
 *              generate_text
 *              make_rpc_call
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
 *      ary_client -h hostname -P protocol -X endpoint -H
 *
 *      where:
 *         hostname is the name of the machine ary_server is running on.
 *         protocol is a valid protocol sequence.
 *		(default:  ncadg_ip_udp).
 *	   endpoint is the port number ary_server is listening on.
 *		(default:  5555).
 *	   -H show usage statement.
 *
 *	   (e.g.  ary_client -h kidney -P ncacn_ip_tcp -X 3008).
 *
 *   DESCRIPTION:
 *	ary_client allocs a variable which contains two members, an 
 *      array of arrays of characters and an array of pointers to characters.
 *      These fields are each initialized to contain random strings (the
 *      randomness is accomplished by calling rand() and converting the 
 *	number into an ascii character via toascii()).  The array of arrays 
 *	and the array of pointers will contain the same string.  This struc-
 *	ture is passed to compare_text() to ensure that they are identical.
 *      The structure is then passed to the server for comparison after it
 *      has been sent across a network.  If the structures match, a boolean
 *      value of true is returned from the server.  If not, a value of false
 *      is returned.
 *
 *   HISTORY:
 *      09/16/91        Gerald Cantor           Initial design/code
 *      10/18/91        Gerald Cantor           ANSIfied functions and updated
 *						prolog and comments.
 *	10/19/91	Gerald Cantor		Updated prologs and comments.
 *	11/13/91	Gerald Cantor		Added -X option.
 *	12/14/91	Gerald Cantor		Added -P option.
 *	12/20/91	Gerald Cantor		Added TRY ... CATCH blocks
 *						around ary_pass_array_kinds.
 */

#include <stdio.h>
#include <pthread.h>
#include <dce/nbase.h>
#include <dce/rpc.h>
#include <dce/rpcexc.h>
#include <dce/rpc.h>
#include <xx_lib.h>
#include <ary.h>
#include <ary_util.h>
#include <util.h>


/*
 * a few global functions and variables.
 */
extern char header[512], buf[128];
char *prog;

main(int argc, char **argv)
{
	/*
	 * main's functions and variables.
	 */
	void get_command_line_parameters(int, char **, char **, char **, 
					 char **); 
	void alloc_space(array_groups_t *), generate_text(array_groups_t *);
	void make_rpc_call(array_groups_t *, handle_t);

	array_groups_t array_groups;
	char *hostname, *protocol = "ncadg_ip_udp", *endpoint = "5555"; 
	char *address;
	unsigned_char_p_t bound_string;
	error_status_t status;
	handle_t h;
        int i, j;

	cma_init();
	prog = argv[0];
	make_header(argv[0]);
	get_command_line_parameters(argc, argv, &protocol, &hostname, 
				    &endpoint);
	address = addr_from_name(hostname);

	/*
    	 *  Compose the string binding.
   	 */ 
    	rpc_string_binding_compose(
       		(unsigned_char_p_t) "32964acc-7444-11ca-a107-10005a4f3428",
       		(unsigned_char_p_t)protocol, address, 
		(unsigned_char_p_t)endpoint, NULL, &bound_string, &status);
    	sprintf(buf, "(%s):  rpc_string_binding_compose failed", prog);
    	check_status(status, rpc_s_ok, NOT_EQUAL, buf, true, RPC);

	/*
	 * Convert to an RPC binding.
 	 */
    	rpc_binding_from_string_binding(bound_string, &h, &status);
    	sprintf(buf, "(%s):  rpc_string_binding_compose failed", prog);
    	check_status(status, rpc_s_ok, NOT_EQUAL, buf, true, RPC);

	alloc_space(&array_groups);
	generate_text(&array_groups);
        make_rpc_call(&array_groups, h);
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
	xx_log(" -h <hostname> the hostname where the server is running");
	xx_log(" -P <protocol> the protocol sequenc to use");
	xx_log("	(default:  ncadg_ip_udp)");
	xx_log(" -X <endpoint> the port the server is listening on");
	xx_log("	(default:  5555)");
}

/*
 * process_command_line_arguments parses the parameters passed by the user
 * and assigns them to variables.
 */
void get_command_line_parameters(int argc, char **argv, char **prot, 
				 char **host, char **ep)
{
	void show_usage(void);
    	extern char *optarg;
    	int c;

	while( ( c = getopt( argc, argv, "P:h:X:H" ) ) != EOF ) {
        switch( c ) {
		case 'P':
			*prot = optarg;
			break;
		case 'h':
			*host = optarg;
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

/*
 * Allocate the space needed for the array of array and array of pointers
 * structure.  If any errors occur, bail out.
 */
void alloc_space(array_groups_t *ary_grp)
{
	int i;
        char *func = "alloc_space";

        for (i = 0; i < MAX_ARRAY_ELEMENTS; i++)
	{
		ary_grp->ary_of_ptr[i] = (ptr_as_str_t *)malloc(sizeof(ptr_as_str_t));
		if (ary_grp->ary_of_ptr[i] == (ptr_as_str_t *)NULL)
		{
			sprintf(buf, "(%s):  Malloc for ary_ptr[%d] failed", 
				func, i);
			xx_severe(buf);
			exit(1);
		}
                ary_grp->ary_of_ptr[i]->the_strg = (char *)malloc(MAX_LINE_LEN);
		if (ary_grp->ary_of_ptr[i]->the_strg == (char *)NULL)
		{
			sprintf(buf, "(%s):  Malloc for ary_ptr[%d]->the_strg failed", func, i);
			xx_severe(buf);
			exit(1);
		}
	}
}

/*
 * Generate the strings that will go into the structure.  These strings will
 * be random due to the use of srand() and rand() to obtain random numbers
 * and converting them to ascii characters.
 */
void generate_text(array_groups_t *ary_grp)
{
	int i, j;
        char *strg = (char *)malloc(MAX_LINE_LEN), *func = "generate_text";

        srand((int) time((long *)0));
        for (i = 0; i < MAX_ARRAY_ELEMENTS; i++)
        {
        	for (j = 0; j < (MAX_LINE_LEN-1); j++)
                	strg[j] = toascii(rand() % 125);
		strg[j] = '\0';
                strcpy(ary_grp->ary_of_ary[i], strg);
                strcpy(ary_grp->ary_of_ptr[i]->the_strg, strg);
        }
}

/*
 * make_rpc_call first verifies that the array of arrays and the array of
 * pointers are identical, then passes the structure off to the server.
 * The boolean variable valid is checked and a success or failure is
 * logged.
 */
void make_rpc_call(array_groups_t *arg, handle_t handle)
{
	int i;
        boolean32 valid = false, exception_raised;

        if (!compare_text(arg))
        {
                sprintf(buf, "%sArray groups do not match before the RPC call",
                        header);
                xx_severe(buf);
                exit(1);
        }
        sprintf(buf, "%sArray groups match before the RPC call", header);
        xx_log(buf);

	do
	{
		TRY
		{
			exception_raised = false;
        		ary_pass_array_kinds(handle, arg, &valid);
		}
		CATCH(rpc_x_comm_failure)
		{
			exception_raised = true;
			xx_log("Communication failure.  Will retry");
		}
		CATCH_ALL
		{
			exception_raised = true;
			xx_error("The following exception has occured");
			print_exception(THIS_CATCH);
		}
		ENDTRY;
	} while (exception_raised == true);

        if (valid)
	{
                sprintf(buf, "%sArray types match at server side", header);
                xx_log(buf);
        }
        else
        {
                sprintf(buf, "%sArray types do not match at server side",
                        header);
                xx_error(buf);
        }
}
