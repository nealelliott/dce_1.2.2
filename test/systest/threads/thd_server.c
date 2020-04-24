/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: thd_server.c,v $
 * Revision 1.1.18.2  1996/02/17  23:29:12  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:45:58  marty]
 *
 * Revision 1.1.18.1  1995/12/11  22:50:05  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1994/07/05  18:28 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1995/12/11  22:23:22  root]
 * 
 * Revision 1.1.15.1  1994/02/23  21:44:07  pellis
 * 	Changed std.h to dce.h
 * 	[1994/02/23  21:41:10  pellis]
 * 
 * 	HP revision /main/HPDCE01/1  1994/01/31  20:30  bissen
 * 	merge kk and hpdce01
 * 
 * Revision 1.1.7.2  1993/07/09  15:19:41  root
 * 	Initial King Kong branch
 * 	[1993/07/09  15:19:13  root]
 * 
 * Revision 1.1.5.3  1993/04/26  12:37:02  eheller
 * 	Removed warning fixes as the casts possibly uncover a null pointer
 * 	situation.
 * 	[1993/04/26  12:33:01  eheller]
 * 
 * Revision 1.1.5.2  1993/04/23  13:53:34  eheller
 * 	Merged Hp_dce1_0_1 changes and
 * 	removed warnings via unsigned_char_p_t casts
 * 	[1993/04/16  17:17:14  eheller]
 * 
 * Revision 1.1.1.7  1993/03/11  22:15:14  osfrcs
 * 	    Loading drop DCE1_0_2B18
 * 
 * Revision 1.1.5.2  1993/02/05  15:27:08  cjd
 * 	Embedded copyright notice
 * 	[1993/02/05  14:48:17  cjd]
 * 
 * Revision 1.1.3.2  1993/02/05  13:15:23  cjd
 * 	Embedded copyright notice
 * 
 * Revision 1.1.3.2  1992/06/11  20:04:01  mcfarland
 * 	changed files from test_latest
 * 	[1992/06/09  13:10:35  mcfarland]
 * 	Revision 1.1.2.2  1992/03/18  01:21:09  rps
 * 	Fix thd_read_portion() retval
 * 	[1992/03/18  00:10:22  rps]
 * 
 * Revision 1.1.1.2  1992/05/28  15:17:57  root
 * 	    Loading drop Dce1_0_1b15
 * 
 * Revision 1.1  1992/01/19  14:35:46  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *
 *   FILE_NAME: thd_server.c 
 *
 *   COMPONENT_NAME: dce.systest
 *
 *   FUNCTIONS: show_usage
 *              process_command_line_arguments
 *              get_port
 *              get_and_show_bindings
 *              register_server
 *              register_auth_info
 *              thd_read_portion
 *              read_array_portion
 *
 *   ORIGINS: 27
 *
 *
 *   (C) COPYRIGHT International Business Machines Corp. 1990
 *   All Rights Reserved
 *
 *
 *   FOR_TEST: dcethrp1
 *
 *   USAGE:
 *
 *      thd_server -P <protocol> -C <max calls> -X <endpoint> -p <principal> 
 *		   -V <auth service> -N 
 *
 *      where:
 *          protocol_sequence is a valid protocol sequence
 *              (default:  ncadg_ip_udp).
 *          max_calls is the number of RPC listener threads to create.
 *              (default:  10).
 *	    endpoint is the port to obtain from the RPC runtime library.
 *              (default:  1062).
 *	    principal is the register principal use to register authentication
 *		info with.
 *	    auth service is the authentication server is use when registering
 *		auth info  (default:  1.  1 maps to private key authentication).
 *
 *   DESCRIPTION:
 *	The server receives the original array, the copy array and how many
 *	threads to create.  Each thread that is created calls the function
 *	read_array_portion.  This function is passed the original array,
 *	the copy array and the lower bound to begin copying from.  The por-
 *	tion of the original array starting from the lower bound up to the
 *	lower bound plus the chunk size is read into the copy array starting 
 *  	at the same lower bound index.  When all threads have completed, the 
 *	original array should have been copied into the copy array.  The copy 
 *	array is passed back to the client so this can be verified.
 *
 *   HISTORY:
 *      11/10/90        Gerald Cantor           Initial design/code.
 *      12/11/90        Gerald Cantor           Output endpoint info to
 *                                              file with write.
 *      12/11/90        Gerald Cantor           convert to use opaque port.
 *	05/08/91	Gerald Cantor		removed functions setsignals
 *						and cleanup.
 *	10/18/91	Gerald Cantor		Added sigset code to handle
 *						ctrl-C signal.
 *	10/18/91	Gerald Cantor		Fixed ANSI C incompatibilites.
 *	10/21/91	Gerald Cantor		Further ANSIfied functions.
 *	10/26/91	Gerald Cantor		Updated prologs.  Removed
 *						functions 
 *						create_and_join_threads.
 *	11/13/91	Gerald Cantor		Changed -M option to -C
 *						(max calls).
 *	11/13/91	Gerald Cantor 		Added -X option.
 *	12/18/91	Gerald Cantor 		Removed read/write to junk_file.
 *						Updated prologs.
 */

#include <stdio.h>
#include <fcntl.h>
#include <dce/rpc.h>
#include <dce/dce.h>
#include <pthread.h>
#include <xx_lib.h>
#include <systest.h> 
#include "thd.h"
#include "thd_util.h"

/*
 * header buffer and message buffer declared in thd_util.c.
 */
extern char header[512], buf[128];

/*
 * define structure to pass arguments to the function that the threads will
 * call.
 */
typedef struct {
	long *orig_ar;     /* address of the original array */
	long *sub_ar;      /* address of the copy array */
	int lower;         /* the lower bound of the array portion */
	int chunks;        /* chunk size of array */
	int which;         /* the current thread number */
} args;

/*
 * a few global variables.
 */
char *prog;
int cma_debug_available = 0;

main(int argc, char **argv)
{
	/*
	 * main's functions and variables.
	 */
	void process_command_line_arguments(int, char **, char **, unsigned32 *,
					    char **, char **, unsigned32 *,
					    boolean32 *);
	void get_port(char *, char *, unsigned32), get_and_show_bindings(void); 
	void register_server(void);
	void register_auth_info(char *, unsigned32);

	char *protocol = "ncadg_ip_udp", *endpoint = "1062", *princ_name;
	unsigned32 max_calls = 10, authn_svc = 1;
	boolean32 secure = true;
	error_status_t st;
#ifdef aix31
	pthread_t this_thread = pthread_self();
	sigset_t sigset;
#endif

	if (cma_debug_available)
                cma_debug();

	cma_init();
	prog = argv[0];
	make_header(prog);

#ifdef DEBUG
	rpc__dbg_set_switches("0-3.5", &st);
        if (st != rpc_s_ok)
        {
                printf("rpc__dbg_set_switches failed\n");
                exit(1);
        }
#endif

	process_command_line_arguments(argc, argv, &protocol, &max_calls, 
				       &endpoint, &princ_name, &authn_svc, 
				       &secure);
	get_port(protocol, endpoint, max_calls);
	get_and_show_bindings();
	register_server();	
	if (secure)
		register_auth_info(princ_name, authn_svc);

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
		sprintf(buf, "%s%s listening for remote requests . . .", header,
			prog);
		xx_log(buf);
		rpc_server_listen(max_calls, &st);
	}
	CATCH_ALL
	{
		rpc_server_unregister_if(thd_v0_0_s_ifspec, NULL, &st);
		sprintf(buf, "Cannot unregister interface from RPC\n");
		check_status(st, rpc_s_ok, NOT_EQUAL, buf, FALSE, RPC);
	}
	ENDTRY;
	exit(0);
}

/*
 * Print a usage statement for the tester.
 */
void show_usage()
{
        sprintf(buf, "Usage:  %s [ OPTIONS ]", prog);
        xx_log(buf);
        xx_log("Where [ OPTIONS ] are one or more of the following:");
	xx_log(" -P <protocol>  a valid protocol sequence");
	xx_log("	(default:  ncadg_ip_udp)");
	xx_log(" -C <max calls>	number of RPC listener threads to create");
	xx_log("	(default:  10)");
	xx_log(" -X <endpoint>	endpoint to obtain from RPC runtime");
	xx_log("	(default:  1062)");
	xx_log(" -p <principal name>	principal to authenticate with");
        xx_log(" -V <auth service>     	authentication service to use");
	xx_log("	(default:  1.  1 maps to private key authentication)");
        xx_log(" -N do not use authentication (overrides -V and -p flags)");
}

/*
 * process_command_line_arguments parses the parameters passed by the user
 * and assigns them to variables.
 */
void process_command_line_arguments(int ac, char **av, char **proto, 
				    unsigned32 *calls, char **port, 
				    char **pname, unsigned32 *auth_srv, 
				    boolean32 *sec)
{
	void show_usage(void);
	extern char *optarg;
	int c;

	while( ( c = getopt( ac, av, "P:C:X:p:V:NHh" ) ) != EOF )
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
			case 'p':
				*pname = optarg;
				break;
			case 'V':
				*auth_srv = atol(optarg);
				break;
			case 'N':
				*sec = false;
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
 * get_port obtains a port to listen for remote requests.  We attempt to
 * obtain the port number that the tester entered.
 */
void get_port(char *prot, char *port, unsigned32 max)
{
	error_status_t status;
	
	rpc_server_use_protseq_ep(prot, max, port, &status);
	sprintf(buf, "Cannot use protocol sequence %s or endpoint %s\n", prot,
		port);
	check_status(status, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);
}

/*
 * get_and_show_bindings inquires on the bindings the server supports.  For
 * each of the bindings, it is converted to a string representation and
 * displayed.
 */
void get_and_show_bindings()
{
	error_status_t status;
	char *string_binding, *net_addr, *endpoint, *nil_string;
	rpc_binding_vector_t *bind_vec;
	int i;

	rpc_server_inq_bindings(&bind_vec, &status);
	sprintf(buf, "Cannot inquire on bindings\n");
	check_status(status, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);

	sprintf(buf, "%sThe following bindings are supported by %s\n", header, 
		prog);
	xx_log(buf);

	for (i = 0; i < bind_vec->count; i++)
	{
		rpc_binding_to_string_binding(bind_vec->binding_h[i],
					     &string_binding,
					     &status);
		sprintf(buf, "Cannot convert binding #%d to string\n", i);
		check_status(status, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);

		sprintf(buf, "%s\n", string_binding);
                xx_log(buf);
                rpc_string_free(&string_binding, &status);
                sprintf(buf, "Cannot free binding %s", string_binding);
                check_status(status, rpc_s_ok, NOT_EQUAL, buf, FALSE, RPC);
	}
}

/*
 * register_server registers the interface with the RPC runtime library.
 */
void register_server()
{
	error_status_t status;

	rpc_server_register_if(thd_v0_0_s_ifspec,
				(uuid_p_t)NULL,
				(rpc_mgr_epv_t)NULL,
				&status);
	sprintf(buf, "Cannot register server with RPC\n");
	check_status(status, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);
}

/*
 * register_auth_info registers the pricipal and the authentication level
 * with the RPC runtime library.  This function is called only if the tester
 * specifies to use authenticated RPC.
 */
void register_auth_info(char *principal, unsigned32 auth_service)
{
	error_status_t status;

	sprintf(buf, "Principal user --> %s", principal);
        xx_log(buf);
        sprintf(buf, "Auth service --> %ld", auth_service);
        xx_log(buf);
	
	rpc_server_register_auth_info((unsigned_char_p_t)principal, 
				      auth_service, 
				      (rpc_auth_key_retrieval_fn_t)NULL, 
				      (idl_void_p_t)NULL, 
				      &status);
	sprintf(buf, "Cannot register auth info\n");
	check_status(status, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);
}
	
/*
 * thd_read_portion allocates space for "thread number" of pthread structures 
 * and arguments structures, creates "thread number" of threads to call the 
 * function read_array_portion(the arguments structures are initialized before 
 * the call to pthread_create) and verifies the status of the thread create.  
 * The threads are then joined and the status of the join is verified.  After 
 * the join, the threads are detached.
 */
error_status_t thd_read_portion (handle_t h, long *orig_ar, long s, long *copy,
				 long mi, long count, long chunks)
{
	void read_array_portion(args *);

	args *arguments;
	pthread_t *thread_array;
	pthread_addr_t thd_st;
	int i, status;

	thread_array = (pthread_t *) malloc (count * sizeof(pthread_t));
	if (thread_array == (pthread_t *)NULL)
        {
                sprintf(buf, "%sMalloc for thread array failed\n", header);
                xx_severe(buf);
                exit(1);
        }

	arguments = (args *) malloc (count * sizeof(args));
	if (arguments == (args *)NULL)
        {
                sprintf(buf, "%sMalloc for arguments array failed\n", header);
                xx_severe(buf);
                exit(1);
        }

	for (i = 0; i < count; i++)
	{
		arguments[i].orig_ar = orig_ar;
		arguments[i].sub_ar = copy;
		arguments[i].chunks = chunks;
		arguments[i].lower = (i*chunks);
		arguments[i].which = i;
		status = pthread_create(&thread_array[i],
					pthread_attr_default,
					(pthread_startroutine_t)read_array_portion,
					(pthread_addr_t)&arguments[i]);
		sprintf(buf, "Creation of thread %d failed\n", i);
		check_status((error_status_t)status, 0, NOT_EQUAL, buf, FALSE, 
			     NOT_RPC);
	}

	for (i = 0; i < count; i++)
	{
		pthread_join(thread_array[i], (pthread_addr_t *)&thd_st);
		sprintf(buf, "Join of thread %d failed\n", i);
		check_status((error_status_t)thd_st, -1, EQUAL, buf, FALSE, 
			     NOT_RPC);
		pthread_detach(&thread_array[i]);
	}
	free(thread_array);
	free(arguments);

	return;
}


/*
 * read_array_portion reads a chunk of the original into the copy array by
 * starting at the lower bound (passed parameter) and continuing up CHUNK
 * indecies.
 */
void read_array_portion(args *a)
{
	int i, low, up;
	char thd_buf[512];

	low = a->lower;
	up = low + a->chunks; 
	for (i = low; i < up; i++)
		a->sub_ar[i] = a->orig_ar[i];

}
