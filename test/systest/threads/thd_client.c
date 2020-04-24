/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: thd_client.c,v $
 * Revision 1.1.19.2  1996/02/17  23:29:10  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:45:57  marty]
 *
 * Revision 1.1.19.1  1995/12/11  22:50:01  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1994/07/05  18:26 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1995/12/11  22:23:21  root]
 * 
 * Revision 1.1.16.1  1994/02/23  21:44:06  pellis
 * 	Changed std.h to dce.h
 * 	[1994/02/23  21:41:05  pellis]
 * 
 * 	HP revision /main/HPDCE01/1  1994/01/31  20:28  bissen
 * 	merge kk and hpdce01
 * 
 * Revision 1.1.7.2  1993/07/09  15:18:21  root
 * 	Initial King Kong branch
 * 	[1993/07/09  15:17:52  root]
 * 
 * Revision 1.1.5.3  1993/04/26  12:33:55  eheller
 * 	Remove casts which fixed warnings as they possibly cause a pointer to a
 * 	null pointer situation.
 * 	[1993/04/26  12:31:07  eheller]
 * 
 * 	Embedded copyright notice
 * 	[1993/02/05  14:48:12  cjd]
 * 
 * Revision 1.1.5.2  1993/04/23  13:53:27  eheller
 * 	Merged Hp_dce1_0_1 changes and
 * 	removed warnings via unsigned_char_p_t casts
 * 	[1993/04/16  17:16:31  eheller]
 * 
 * 	Embedded copyright notice
 * 	[1993/02/05  13:15:17  cjd]
 * 
 * Revision 1.1.1.7  1993/03/11  22:15:12  osfrcs
 * 	    Loading drop DCE1_0_2B18
 * 
 * Revision 1.1.3.3  1992/07/17  21:59:30  root
 * 	  Fixed bug in loading the endpoint on all the nodes.
 * 	[1992/07/17  21:09:59  root]
 * 
 * Revision 1.1.3.2  1992/06/11  20:03:47  mcfarland
 * 	changed files from test_latest
 * 	[1992/06/09  13:10:28  mcfarland]
 * 
 * Revision 1.1.3.2  1992/06/08  18:05:36  ramb
 * 	   Changed type of network address and endpoint from (char *) to
 * 	   (unsigned char *).
 * 	   Also fixed to point to the correct buffer for error messages.
 * 	[1992/06/08  14:34:41  ramb]
 * 
 * Revision 1.1.1.2  1992/05/28  15:17:54  root
 * 	    Loading drop Dce1_0_1b15
 * 
 * Revision 1.1  1992/01/19  14:35:47  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *
 *   FILE_NAME: thd_client.c 
 *
 *   COMPONENT_NAME: dce.systest
 *
 *   FUNCTIONS: show_usage
 *              get_command_line_parameters
 *              create_and_join_threads
 *		assign_array
 *              thread_routine
 *		allocate arrays
 *		validate_and_free_arrays
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
 *      thd_client -h <host> -P <protocol> -X <endpoint> -n <number_of_threads> 
 *		   -c <chunk_size> -p <principal> -A <auth service> 
 *		   -V <auth level> -Z <authorization service> -N
 *
 *      where:
 *         host is the hostname where the server is running.
 *         protocol_sequence is a valid protocol sequence
 *              (default:  ncadg_ip_udp).
 *	   endpoint is the port number the server is listening on
 *              (default:  1062).
 *	   number_of_threads is the number of threads to create on client side
 *		and server side.
 *	   chunk_size is the number of array elements to read in a thread
 *	   principal is the registery principal to use when registering auth
 *		info.
 *	   auth service is the authentication service to use when registering
 *		auth info (default:  1.  1 maps to private key authentication).
 *	   auth level is the level of authentication to use when setting auth
 *		info (default:  0.  0 maps to default authentication level).
 *	   authz service is the authorization service to use when setting
 *		auth info (default:  2.  2 map to dce PAC authorization).
 *	   -N specifies to use unauthenticated RPC.  This flag overrides the
 *		-p, -A, -V and -Z flags.
 *
 *	   the  following will create a 100 element (10*10) array and 10 
 *	   threads.  Each thread will read 10 elements of the array.
 *
 *	   (e.g.  thd_client -h gcantor -P ncacn_ip_tcp -X 1062 -n 10 -c 10 -N)
 *
 *   DESCRIPTION:
 *	The client mallocs space for two arrays of long integers.  One
 *      array is initialized to contain long integers corresponding to
 *      the index numbers (array[0] = 0, array[1] = 1, etc).  This is the
 *      original array.  The other array is left uninitialized.  This is
 *      the copy array.  The size of the arrays is equal to the number of
 *      threads the user wishes to create times a chunk size.  Once the 
 *	arrays are created and initialized, the "imput number" of threads 
 *	are created.  Each of these threads are responsible for binding to 
 *	the server process.  Once bound, the thread passes the original 
 *	array, the copy array and the number of threads the server should 
 *	create (same number of threads created here).  After the remote 
 *	procedure call, the copy array should be identical to the original 
 *	array.  These two arrays are compared to verify.  A message stating 
 *	the outcome of this comparison is logged.  All resources (arrays, 
 *	binding, etc) are then freed and the process exits.	
 *
 *   HISTORY:
 *      11/10/90        Gerald Cantor           Initial design/code.
 *      12/11/90        Gerald Cantor           convert to use opaque port.
 *	10/21/91	Gerald Cantor		ANSIfied functions and updated
 *						comments.
 *	10/26/91	Gerald Cantor		Updated comments and prologs.
 *	11/13/91	Gerald Cantor		Added protocol sequence to 
 *						thread argument structure.
 *	11/13/91	Gerald Cantor		Changed -e option to -X.
 *	12/18/91	Gerald Cantor		Updated prologs.
 *      05/12/92        Ram Balaraman           Fixed type of network address
 *                                              Also fixed to pass the correct
 *                                              buffer for check_status rtn.
 */

#include <stdio.h>
#include <ctype.h>
#include <dce/rpc.h>
#include <dce/rpcexc.h>
#include <dce/dce.h>
#include <pthread.h>
#include <xx_lib.h>
#include <systest.h>
#include "thd.h"
#include "thd_util.h"

/*
 * header and buf are declared in thd_util.c.
 */
extern char header[512], buf[128];

/*
 * define structure to pass arguments to the function that the threads will
 * call.
 */
typedef struct {
	char *protocol;	  /* protocol sequence to use when making RPC */
	char *address;    /* network address of server to bind to */
	char *endpoint;   /* port server is listening on */
	int count;	  /* number of threads to create on server side */
	int chunks;	  /* chunk size of the array */
	char *server; 	  /* server thread is bound to */
} args;


/*
 * a few global variables.
 */
char *prog, *princ_name, *protocol = "ncadg_ip_udp", *endpoint = "1062";
int thread_count = 10, chunks = 100;
unsigned32 authn_svc = 1, authn_level = 0, authz_svc = 2;
boolean32 secure = true;

/*
 * structure to keep a list of server hosts and the ports they listen on.
 */
typedef struct host_and_port_t {
	char *host;
	char *port;
	struct host_and_port_t *next;
} host_and_port_t;

host_and_port_t *node, *head;
int cma_debug_available = 0;

main(int argc, char **argv)
{
	/*
	 * main's functions and variables.
	 */
	void get_command_line_parameters(int, char **);
	void create_and_join_threads(int, int);
	error_status_t st;

	cma_init();
	prog = argv[0];
	make_header(argv[0]);

#ifdef DEBUG
	rpc__dbg_set_switches("0-3.5", &st);
        if (st != rpc_s_ok)
        {
                printf("rpc__dbg_set_switches failed\n");
                exit(1);
        }
#endif

	get_command_line_parameters(argc, argv);
	create_and_join_threads(thread_count, chunks);
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
	xx_log(" -P <protocol> valid protocol sequence");
	xx_log(" 	(default:  ncadg_ip_udp)");
	xx_log(" -h <hostname> name of the host where server is running");
	xx_log(" -X <endpoint> port the server is listening on");
	xx_log(" 	(default:  1062)");
        xx_log(" -n <number of threads> number of threads to create");
	xx_log("	(default:  10)");
        xx_log(" -c <chunk size> number of elements to read from array"); 
	xx_log("	(default:  100)");
        xx_log(" -p <principal name> principal to set binding auth info"); 
        xx_log(" -A <authentication level> level of authentication");
	xx_log("	(default:  0.  0 maps to default level)"); 
        xx_log(" -V <authentication service> authentication service");
	xx_log("	(default:  1.  1 maps to private key authentication)"); 
        xx_log(" -Z <authorization service> authorization service to use");
	xx_log("	(default:  2.  2 maps to dce PAC authorization)"); 
        xx_log(" -N specifies unauthenticated RPC");
	xx_log(" -N overrides -p, -A, -V and -Z flags");
}

/*
 * process_command_line_arguments parses the parameters passed by the user
 * and assigns them to variables.
 */
void get_command_line_parameters(int argc, char **argv)
{
	void show_usage(void);
    	extern char *optarg;
    	int c;
    	host_and_port_t *trav;

	while( ( c = getopt( argc, argv, "P:h:X:n:c:p:A:V:Z:NH" ) ) != EOF ) {
        switch( c ) {
		case 'h':
			node = (host_and_port_t *)malloc(sizeof(host_and_port_t));
			node->host = optarg;
			node->port = endpoint;
			if (head == NULL)
				head = node;	
			else
			{
				trav = head;
				while (trav->next)
					trav = trav->next;
				trav->next = node;
			}
			break;
		case 'P':
			protocol = optarg;
			break;
		case 'X':
			node->port = optarg;
			break;
		case 'n':
			thread_count = atoi(optarg);
			break;
		case 'c':
			chunks = atoi(optarg);
			break;
		case 'p':
			princ_name = optarg;
			break;
		case 'A':
			authn_level = atol(optarg);
			break;
		case 'V':
			authn_svc = atol(optarg);
			break;
		case 'Z':
			authz_svc = atol(optarg);
			break;
		case 'N':
			secure = false;
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

	if (head == NULL)
	{
		sprintf(buf, "%sHost name must be specified\n", header);
		xx_severe(buf);
		exit(1);
	}
}	

/*
 * create_and_join_threads allocates space for "thread number" of pthread
 * structures and arguments structures, creates "thread number" of threads
 * to call the function thread_routine(the arguments structures are init-
 * ialized before the call to pthread_create) and verifies the status of
 * the thread create.  The threads are then joined and the status of the
 * join is verified.  After the join, the threads are detached.
 */
void create_and_join_threads(int t_count, int chks)
{
	void thread_routine(args *);
	args *arguments;
	pthread_addr_t thd_st;
	int i, status;
	pthread_t *thread_array;
	host_and_port_t *trav;

	thread_array = (pthread_t *) malloc (t_count * sizeof(pthread_t));
	if (thread_array == (pthread_t *)NULL)
        {
                sprintf(buf, "%sMalloc for thread array failed\n", header);
                xx_severe(buf);
                exit(1);
        }

	arguments = (args *) malloc (t_count * sizeof(args));
	if (arguments == (args *)NULL)
        {
                sprintf(buf, "%sMalloc for arguments array failed\n", header);
                xx_severe(buf);
                exit(1);
        }

	trav = head;
	i = 0;
	while (trav)
	{
		arguments[i].protocol = protocol; 
		arguments[i].address = addr_from_name(trav->host);
		arguments[i].endpoint = trav->port;
		arguments[i].count = t_count;
		arguments[i].chunks = chks;
		arguments[i].server = trav->host; 
		status = pthread_create(&thread_array[i],
				      pthread_attr_default,
				      (pthread_startroutine_t)&thread_routine,
			     	      (pthread_addr_t)&arguments[i]);
		sprintf(buf, "Creation of thread %d failed\n", i);
		check_status((error_status_t)status, 0, NOT_EQUAL, buf, FALSE, 
			     NOT_RPC);
		trav = trav->next;
		i++;
	}

	trav = head;
	i = 0;
	while (trav)
	{
		pthread_join(thread_array[i], &thd_st);
		sprintf(buf, "Join of thread %d failed\n", i);
		check_status((error_status_t)thd_st, -1, EQUAL, buf, FALSE, 
			     NOT_RPC);
		pthread_detach(&thread_array[i]);
		trav = trav->next;
		i++;
	}
	free(thread_array);
	free(arguments);
}

/*
 * assign_array initializes the original array with long integers corre-
 * sponding to the array indecies.
 */
void assign_array(long *a, int n)
{
	int i;
	
	for (i = 0; i < n; i++)
		a[i] = (long)i;
}

/*
 * thread_routine is responsible for binding to the proper server process
 * and calling the remote procedure.  A string binding is composed and that
 * string binding is converted to a binary binding.  Space for the arrays
 * are allocated and the original array is initialized.  The remote proce-
 * dure call is made and the arrays are compared to insure sameness.
 */
void thread_routine(args *a)
{
	void allocate_arrays(long **, long **, int), assign_arrays(long *, int);
	void validate_and_free_arrays(long *, long *, int, char *);

    	handle_t rh;
	boolean32 exception_raised; 
	char *nil_string = "";
	char *string_binding;
	char thd_buf[512];
	long *original, *copy;
	int size = ((a->count)*(a->chunks));
    	error_status_t st, status;
	
	rpc_string_binding_compose(nil_string,
				    a->protocol,
				    a->address,
				    a->endpoint,
				    nil_string,
				    &string_binding,
				    &status);  
    
	sprintf(thd_buf, "Cannot compose binding\n");
	check_status(status, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);
	
	rpc_binding_from_string_binding(string_binding, &rh, &status);
	sprintf(thd_buf, "Cannot create binding from string binding\n");
	check_status(status, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);

	if (secure)
	{
		sprintf(buf, "Principal user --> %s", princ_name);
		xx_log(buf);
		sprintf(buf, "Auth level --> %ld", authn_level);
		xx_log(buf);
		sprintf(buf, "Auth service --> %ld", authn_svc);
		xx_log(buf);
		sprintf(buf, "Authz service --> %ld", authz_svc);
		xx_log(buf);
	
		rpc_binding_set_auth_info((rpc_binding_handle_t)rh, 
				 	  (unsigned_char_p_t)princ_name, 
					  authn_level, 
					  authn_svc, 
					  (rpc_auth_identity_handle_t)NULL, 
					  authz_svc, &st);
		sprintf(buf, "Cannot set auth info on handle\n");
		check_status(st, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);
	}

	allocate_arrays(&original, &copy, size);
	assign_array(original, size);

	xx_log("Calling thd_read_portion with original and copy array\n");
	do
	{
		exception_raised = false;
		TRY
		{
			thd_read_portion(rh, original, size, copy, 
						  size, a->count, a->chunks);
		}
		CATCH(rpc_x_comm_failure)
		{
			exception_raised = true;
			sprintf(buf, "RPC failure in thread %d:  %s\n", 
				a->server, dce_error_inq_text(status, &st));
			xx_log(buf);
		}
		CATCH_ALL
		{
			exception_raised = true;
			sprintf(buf, "Following exception occured in thread %s\n", 
				a->server);
			xx_severe(buf);
			print_exception(THIS_CATCH);
			RERAISE;
		}
		ENDTRY;
	} while (exception_raised == true);

	rpc_binding_free(&rh, &status);
	sprintf(thd_buf, "Cannot free binding\n");
	check_status(status, rpc_s_ok, NOT_EQUAL, buf, FALSE, RPC);
	validate_and_free_arrays(original, copy, size, a->server);
}

/*
 * allocate_arrays mallocs space for the two arrays and verifies that the
 * malloc did not fail.
 */
void allocate_arrays(long **original_array, long **copy_array, int array_size)
{
	*original_array = (long *) malloc(array_size * sizeof(long));
	if (*original_array == (long *)NULL)
        {
                sprintf(buf, "%sMalloc for original array failed\n", header);
                xx_severe(buf);
                exit(1);
        }
		
	*copy_array = (long *) malloc(array_size * sizeof(long));
	if (*copy_array == (long *)NULL)
        {
                sprintf(buf, "%sMalloc for copy array failed\n", header);
                xx_severe(buf);
                exit(1);
        }
}

/*
 * validate_arrays compares byte for byte the two arrays.  If the bytes do
 * not match, the flag is set to 0 (false).  The space for the arrays is
 * freed and a message about the outcome of the compares is logged.
 */
void validate_and_free_arrays(long *original_array, long *copy_array, 
			      int array_size, char *server_name)
{
	int i, flag = 1;

	for (i = 0; i < array_size; i++)
	{
#ifdef DEBUG
		sprintf(buf, "Original ==> %ld\tCopy ==> %ld\n", 
			original_array[i], copy_array[i]);
		xx_log(buf);
#endif
		if (original_array[i] != copy_array[i])
		{ 
			sprintf(buf, "Array mismatch at location %d\n", i);
			xx_error(buf);
			sprintf(buf, "Original[%d] = %ld, Copy[%d] = %ld\n", i, 
				original_array[i], i, copy_array[i]);
			xx_error(buf);
			flag = 0; 
		}
	}
	free(original_array);
	free(copy_array);

	if (flag != 0)
	{
		sprintf(buf, "%sArray reads served by %s successful\n", header,
			server_name);
		xx_log(buf);
	}
	else
	{
		sprintf(buf, "%sArray reads served by %s not successful\n",
			header, server_name);
		xx_error(buf);
	}
}
