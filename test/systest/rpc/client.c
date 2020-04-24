/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: client.c,v $
 * Revision 1.1.8.2  1996/03/09  20:50:57  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:28:56  marty]
 *
 * Revision 1.1.8.1  1995/12/13  21:55:35  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/13  21:45:10  root]
 * 
 * Revision 1.1.6.2  1993/07/09  12:32:26  root
 * 	Initial King Kong branch
 * 	[1993/07/09  12:31:35  root]
 * 
 * Revision 1.1.4.2  1993/03/31  12:25:54  mgm
 * 	Initial GAMERA branch
 * 
 * Revision 1.1.3.2  1992/06/11  19:54:32  mcfarland
 * 	changed files from test_latest
 * 	[1992/06/09  13:01:53  mcfarland]
 * 
 * Revision 1.1.1.2  1992/05/28  15:12:14  root
 * 	    Loading drop Dce1_0_1b15
 * 
 * Revision 1.1.3.2  1992/04/15  16:54:28  rps
 * 	Reorganize util routines
 * 	[1992/04/15  15:57:40  rps]
 * 
 * Revision 1.1  1992/01/19  14:37:43  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *
 *
 *   FILE_NAME: client.c 
 *
 *   COMPONENT_NAME: dce.systest
 *
 *   FUNCTIONS: show_usage
 *              init_vars
 *              get_command_line_parameters
 *              file_read
 *		file_prep
 *		get_buffers
 *		do_comp
 *		do_result
 *		tidy_up
 *		Get_bind
 *		import_interface
 *
 *   ORIGINS: 27
 *
 *
 *   (C) COPYRIGHT International Business Machines Corp. 1991
 *   All Rights Reserved
 *
 *
 *   FOR_TEST: dcerprec
 *
 *   USAGE:
 *
 *      client -f <file_name> -b <buffer_size> -E <namespace_syntax> 
 *	       -S <entry_name> -g <group_name> -p <config_profile> 
 *  	       -a <authentication_level> -v <authentication_service> 
 *	       -z <authorization_service> -o <principal_name> -n 
 *
 *      where:
 *
 *	   file_name is the name of file to read
 *	   buffer_size is the number of bytes to read per file read
 *		(default:  100).
 *         namespace_syntax is the syntax to use when importing interfaces from 
 *              the namespace (default:  3.  3 maps up to rpc_c_ns_syntax_dce).
 *         entry_name is the server entry to import from.
 *         service_group is the service_group that contains the server entry.
 *         config_profile is the configuration profile that contains the server
 *		entry. 
 *	   authentication_level is the level the client will use to set binding
 *		auth info (default:  0.  0 maps to the default authentication 
 *		level).
 *	   authentication_service is the service that the client will use to
 *		set binding auth info (default:  1.  1 maps to dce private key
 *		authentication).
 *	   authorization_level is the authorization type the client will use
 *		to set binding auth info (default:  2.  2 maps to dce PAC
 *		authorization).
 *         principal_name is the registery user that the client will set
 *              binding auth info with.
 *	   -n specifies to not use authenticated RPC.  This flag overrides the
 *		-o, -a, -v, and -z flags.
 *
 *	   (e.g.  client -f bigfile -S /.:/recov_entry -p /.:/recov_profile -n)
 *
 *   DESCRIPTION:
 *	The client imports the interface from the namespace (from a server
 *	entry, service group or configuration profile).  If the tester
 *	specified authenticated RPC, the binding handle is set with the
 *	principal, authentication level, authentication service and 
 *	authorization service.
 *
 *	The client reads buffer_size number of bytes remotely and locally. 
 *	These reads go into different buffers.  These buffers are compared
 *	to insure likeness.  The offset into the file is saved for passing
 *	to the server (to facilitate lseeking to the right place).
 * 
 *	After the file has been read, client performs some very rudimentary
 *	performance analysis by computing the total number of RPC calls,
 *	how many passed and how many failed.  These numbers are turned into
 *	percentages.  Afterwards, client cleans up by closing open files and
 *	freeing the RPC handle.
 *
 *   HISTORY:
 *      11/10/90        Gerald Cantor           Initial design/code
 *
 *	07/15/91	Gerald Cantor		Added prefix goodies for
 *						GDA Bind stuff.
 *	10/19/91	Gerald Cantor		ANSIfied functions, updated
 *						comments and prologs and removed
 *						globals.h.
 *	10/29/91	Gerald Cantor		Pass nb to do_comp rather than
 *						buffer_size.
 *	12/15/91	Gerald Cantor		Add TRY ... CATCH clause around
 *						RPC.
 *	01/03/92	Gerald Cantor		Update prologs.
 */
#include "dummy_if.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dce/rpc.h>
#include <dce/rpcexc.h>
#include <time.h>
#include <xx_lib.h>
#include <pthread.h>
#include <util.h>

/*
 * A few global variables.
 */
extern char header[512], buf[128]; 

boolean32 security;
char host[30], *prog;
char *file_name;
char *entry_name, *group_name, *profile_name;
char *princ_name;
unsigned32 entry_syntax, group_syntax, member_syntax, profile_syntax;
unsigned32 authn_svc, authz_svc, authn_level;
int pid_num, res_d, buffer_size, entry_type, cma_debug_available = 0;

main(int argc, char **argv)
{
	/*
	 * Main's functions and variables.
 	 */
	void init_vars(void), get_command_line_parameters(int, char **);
	void file_read(handle_t, char *, int);
	handle_t Get_bind(void), rh;
	error_status_t st;

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

	init_vars();
	get_command_line_parameters(argc, argv);
	rh = Get_bind();
	file_read(rh, file_name, buffer_size);
}

/*
 * print a usage statement for the tester.
 */
void show_usage()
{
	sprintf(buf, "Usage:  %s [ OPTIONS ]", prog);
	xx_log(buf);
	xx_log("Where [ OPTIONS ] are one or more of the following:");
	xx_log("-f <file name> name of the file to read");
	xx_log("-E <namespace syntax> syntax for importing from namespace");
	xx_log("        (default:  3.  3 maps to rpc_c_ns_syntax_dce)");
	xx_log("-b <bytes> number of bytes to read per file read");
	xx_log("        (default:  100)");
	xx_log("-S <entry name>	server entry to lookup in namespace");
	xx_log("-g <group name>	service group to lookup in namespace");
	xx_log("-p <config profile> config profile to lookup in namespace");
        xx_log("-a <auth level>     authentication level to use");
	xx_log("	(default:  0.  0 maps to default level)");
	xx_log("-v <auth service>   authentication service to use");
	xx_log("	(default:  1.  1 maps to dce private key service)");
        xx_log("-z <authz service>  authorization service to use");
	xx_log("	(default:  2.  2 maps to dce PAC service)");
        xx_log("-o <principal name> name to authenticate with");
	xx_log("-n use non-secure RPC (overrides -a, -v, -z and -o flags)");
	xx_log("Tester must specify a file name and an entry name");
}
	
/*
 * init_vars initialize some variables with default values.
 */
void init_vars()
{
	/* rpc_c_syntax_dce = 3 */
	entry_syntax = group_syntax = profile_syntax = member_syntax = 3;

	file_name = (char *)malloc(80*sizeof(char));
	file_name = (char *)NULL;
	buffer_size = 100;
	entry_type = SERVER_ENTRY;
	security = TRUE;
	authn_svc = 1;
	authz_svc = 2;
	authn_level = 0;
}

/*
 * get_command_line_arguments parses the parameters passed by the user
 * and assigns them to variables.
 */
void get_command_line_parameters(int argc, char **argv)
{
	void show_usage(void);
    	extern char *optarg;
    	int c;
	char *func = "get_command_line_parameters";

	
    while( ( c = getopt( argc, argv, "E:S:g:p:f:b:a:v:z:o:nhH" ) ) != EOF ) {
	switch( c ) {
	case 'E':
	    entry_syntax = atol(optarg);
	    group_syntax = profile_syntax = member_syntax = entry_syntax;
	    break;
	case 'S':
	    entry_type = SERVER_ENTRY;
	    entry_name = optarg;
	    break;
	case 'g':
	    entry_type = SERVICE_GROUP;
	    group_name = optarg;
	    break;
	case 'p':
	    entry_type = CONFIG_PROFILE;
	    profile_name = optarg;
	    break;
	case 'f':
	    file_name = optarg;
	    break;
	case 'b':
	    buffer_size = atoi(optarg);
	    break;
	case 'v':
            authn_svc = atol(optarg);
            break;
        case 'a':
            authn_level = atol(optarg);
            break;
        case 'z':
            authz_svc = atol(optarg);
            break;
	case 'o':
	    princ_name = optarg;
	    break;
	case 'n':
	    security = FALSE;
	    break;
	case 'h':
	case 'H':
	    show_usage();
	    exit( 0 );
        }
    }

    if (file_name == (char *)NULL)
    {
	sprintf(buf, "%s(%s):  Must specify a file name\n", header, func);
	xx_severe(buf); 
	show_usage();
	exit(1);
    }
}

/*
 * file_read conducts the remote and local file reads.
 */
void file_read(handle_t handle, char *readfile, int bufsize)
{
	void file_prep(char *, int *, struct stat *); 
	void get_buffers(char **, char **, int); 
	void do_result(int, long, long, long);
	void tidy_up(handle_t *, int);
	int do_comp(char *, char *, int);

	int fd, flag = 0, nb = 0;
	long os = 0, tries = 0, fails = 0;
	char *lbuffer, *rbuffer, *string_binding, *port, hostname[30];
	struct stat file_info;
	boolean32 exception_raised;
	error_status_t st, status;

	gethostname(host, 30);
	file_prep(readfile, &fd, &file_info);
	get_buffers(&lbuffer, &rbuffer, bufsize);
	rpc_binding_to_string_binding(handle, &string_binding, &st);
	string_binding_to_name(string_binding, hostname, &port, NULL);
	sprintf(buf, "Bound to server %s@%s\n", hostname, port);
	xx_log(buf);
	switch(security)
	{
		case FALSE:
			break;
		case TRUE:
		default:
		{
			rpc_binding_set_auth_info((rpc_binding_handle_t)handle,
						  (unsigned_char_p_t)princ_name,
						  authn_level,
						  authn_svc,
						  (rpc_auth_identity_handle_t)NULL,
						  authz_svc,
						  &st);
			sprintf(buf, "Cannot set binding auth info\n");
			check_status(st, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);

			break;
		}
	}
	sprintf(buf, "Performing file reads and compares.  Please wait.\n");
	xx_log(buf);
	do
	{
		do
		{
			TRY
			{
				exception_raised = false;
				dummy_if_read(handle, readfile, os, rbuffer, 
					      bufsize);
				tries++;
			}
			CATCH(rpc_x_comm_failure)
			{
				exception_raised = true;
				sprintf(buf, "RPC failure #%ld on %s\n", 
					++fails, host);
				xx_log(buf);
			}

			CATCH_ALL
			{
				exception_raised = true;
				xx_log("The following exception has occured");
				print_exception(THIS_CATCH);
			}
			ENDTRY;
		} while (exception_raised == true);

		nb = read(fd, lbuffer, bufsize);
		os += nb;
		flag = do_comp(lbuffer, rbuffer, nb); /* did reads match? */

		if ((os % 500) == 0) /* show bytes read in multiples of 500 */
		{
			sprintf(buf, "Client on host %s: %d of %d bytes read\n",
			 host, os, file_info.st_size);
			xx_log(buf);
		}

	} while (nb == bufsize && !flag);
	
	do_result(flag, os, tries, fails);
	tidy_up(&handle, fd); 	
}

/*
 * file_prep stats the file passed to obtain its size in bytes and
 * then opens the file.
 */
void file_prep(char *file, int *file_desc, struct stat *f_info)
{
	int rc;

	rc = stat(file, f_info);
	sprintf(buf, "Cannot stat %s\n", file);
	check_status(rc, -1, EQUAL, buf, TRUE, NOT_RPC); 

	*file_desc = open(file, O_RDONLY, 0);
	sprintf(buf, "Cannot open %s.  Exiting!\n", file);
	check_status(*file_desc, -1, EQUAL, buf, TRUE, NOT_RPC);
}

/*
 * get_buffers allocates space for the local and remote file buffers.
 * If any error occurs with malloc, bail out.
 */
void get_buffers(char **lb, char **rb, int size)
{
	if ((*lb = (char *) malloc(size)) == (char *)NULL)
	{
		sprintf(buf, "%smalloc for local buffer failed!\n", header);
		xx_severe(buf);
		exit(1);
	}

	if ((*rb = (char *) malloc(size)) == (char *)NULL)
	{
		sprintf(buf, "%smalloc for remote buffer failed!\n", header);
		xx_severe(buf);
		exit(1);
	}
}

/*
 * do_comp compares the bytes read by client and bytes read by server.
 * If they are alike, all is well.  If not, the two buffers are displayed
 * along with the location of where they did not match.
 */
int do_comp(char *buf1, char *buf2, int n_of_bytes)
{

        int i;

        for (i = 0; i < n_of_bytes; i++)
                if (buf1[i] != buf2[i])
		{
			sprintf(buf, "buffers do not match at location %d\n",
				i);
			xx_error(buf);
			xx_log("Local buffer ==>");
			strncpy(buf, buf1, n_of_bytes);
			xx_log(buf);
			xx_log("Remote buffer ==>");
			strncpy(buf, buf2, n_of_bytes);
			xx_log(buf);
                        return(1);
		}
        return(0);
}


/*
 * do_results checks the result of the buffer compares and logs an
 * according message.  Some basic percentages are computed on the number
 * of successful and unsuccessful RPC calls made.
 */
void do_result(int result_flag, long bytes, long attempts, long failures)
{
	float percent_passed, percent_failed;

	if (!result_flag) /* matched */
	{
		sprintf(buf, "%s%ld bytes read.  FILE READS MATCH.\n", header, 
			bytes);
		xx_log(buf);
	}
	else /* did not match */
	{
		sprintf(buf, "%s%ld bytes read.  FILE READS DO NOT MATCH.\n", 
			header, bytes);
		xx_error(buf);
	}
	percent_passed = ((float)(attempts - failures))/((float)attempts)*100.00;
	percent_failed = ((float)failures)/((float)attempts)*100.00;
	sprintf(buf, "Total rpc calls = %ld\n", attempts);
	xx_log(buf);
	sprintf(buf, "Total rpc calls that passed = %ld\n", 
		(attempts - failures));
	xx_log(buf);
	sprintf(buf, "Total rpc timeouts = %ld\n", failures);
	xx_log(buf);
	sprintf(buf, "Percent passed:  %4.2f%%\n", percent_passed);
	xx_log(buf);
	sprintf(buf, "Percent timeouts:  %4.2f%%\n", percent_failed);
	xx_log(buf);
}

/*
 * tidy_up frees the RPC handle and closes the read file and the log
 * file.
 */
void tidy_up(handle_t *rh, int file_desc)
{
	char *func = "tidy_up";
	error_status_t st;

	close(file_desc);
	rpc_binding_free(rh, &st);
	sprintf(buf, "(%s):  Cannot free handle\n", func);
	check_status(st, 0, NOT_EQUAL, buf, FALSE, RPC);

}

/*
 * Get_bind looks up the entry name a service group or a configuration
 * profile, depending on where the entry is stored.
 */
handle_t Get_bind()
{
	handle_t import_interface(unsigned32, char *), rh;
	rpc_ns_inq_handle_t profile_context;
	rpc_if_id_t if_id;
	char *member, *func = "Get_bind";
	unsigned32 prio, import_syntax;
	error_status_t st;

	switch(entry_type)
        {
                case SERVICE_GROUP:
                {
                        member = group_name;
			import_syntax = group_syntax;
                        break;
                }

                case CONFIG_PROFILE:
                {
			sprintf(buf, "(%s):  Beginning profile inquire", func);
			xx_log(buf);
                        rpc_ns_profile_elt_inq_begin(profile_syntax,
                                                     profile_name,
                                                     rpc_c_profile_all_elts,
                                                     (rpc_if_id_p_t)NULL, 
						     (unsigned32)NULL, 
						     member_syntax,
                                                     (unsigned_char_p_t)NULL,
                                                     &profile_context,
                                                     &st);
                        sprintf(buf, "(%s):  Cannot begin profile inquire",
                                func);
                        check_status(st, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);

                        rpc_ns_profile_elt_inq_next(profile_context, &if_id,
                                                    &member, &prio, NULL, &st);
                        sprintf(buf, "(%s): Cannot inquire next profile member",
                                func);
                        check_status(st, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);

                        rpc_ns_profile_elt_inq_done(&profile_context, &st);
                        sprintf(buf, "(%s):  profile_inq_elt_done failed",
                                func);
                        check_status(st, rpc_s_ok, NOT_EQUAL, buf, FALSE, RPC);
			sprintf(buf, "(%s):  Profile inquired successfully",
				func);
			xx_log(buf);

			import_syntax = entry_syntax;

                        break;
                }

                case SERVER_ENTRY:
                default:
                {
                        member = entry_name;
			import_syntax = entry_syntax;
                        break;
                }
        }
	sprintf(buf, "(%s):  Importing interface", func);
	xx_log(buf);
        rh = import_interface(import_syntax, member);
	sprintf(buf, "(%s):  Interface imported", func);
	xx_log(buf);
	return(rh);
}

/*
 * Imports the interface from the namespace.
 */
handle_t import_interface(unsigned32 syntax, char *ent_name)
{
	handle_t rh;
	rpc_ns_import_handle_t context;
	char *func = "import_interface";
	error_status_t st;
	
	sprintf(buf, "(%s):  Beginning import sequence", func);
	xx_log(buf);
	rpc_ns_binding_import_begin(syntax,
                                    ent_name,
                                    dummy_if_v0_0_c_ifspec,
                                    NULL,
                                    &context,
                                    &st);
       	sprintf(buf, "(%s):  rpc_ns_binding_import_begin failed\n", func);
       	check_status(st, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);

 	rpc_ns_binding_import_next(context, &rh, &st);
       	sprintf(buf, "(%s):  rpc_ns_binding_import_next\n", func);
        check_status(st, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);
		
        rpc_ns_binding_import_done(&context, &st);
        sprintf(buf, "(%s):  rpc_ns_binding_import_done failed\n", func);
        check_status(st, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);

	return(rh);
}
