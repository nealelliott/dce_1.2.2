/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: server.c,v $
 * Revision 1.1.9.2  1996/03/09  20:51:39  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:29:23  marty]
 *
 * Revision 1.1.9.1  1995/12/13  21:55:58  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/13  21:45:26  root]
 * 
 * Revision 1.1.6.2  1993/07/09  13:24:36  root
 * 	Initial King Kong branch
 * 	[1993/07/09  13:24:08  root]
 * 
 * Revision 1.1.4.2  1993/03/31  12:38:51  mgm
 * 	Initial GAMERA branch
 * 
 * Revision 1.1.3.2  1992/06/11  19:58:54  mcfarland
 * 	changed files from test_latest
 * 	[1992/06/09  13:05:51  mcfarland]
 * 
 * 	Reorganize util routines
 * 	[1992/04/15  16:01:27  rps]
 * 
 * 	Fix error returns
 * 	[1992/03/18  00:13:03  rps]
 * 
 * Revision 1.1.1.2  1992/05/28  15:14:52  root
 * 	    Loading drop Dce1_0_1b15
 * 
 * Revision 1.1  1992/01/19  14:37:39  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *
 *
 *   FILE_NAME: server.c 
 *
 *   COMPONENT_NAME: dce.systest
 *
 *   FUNCTIONS: init_vars 
 *		get_command_line_parameters
 *		show_usage
 *              get_port_seq
 *		register_interface
 *              cds_register
 *              cds_unregister
 *              dummy_if_read
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
 *      server -P <protocol> -C <max_calls> -S <entry_name> -g <service_group> 
 *	       -p <config_profile> -E <namespace_syntax> -O <principal_name>
 *	       -V <authentication_service> -N
 *
 *      where:
 *         protocol is a valid protocol sequence (default:  ncadg_ip_udp).
 *	   max_calls is number of RPC listener threads to create (default:  10).
 *	   entry_name is the server entry to create in the namespace.
 *	   service_group is the service_group to create in the namespace.  The
 *		entry_name will be placed in this service group.
 *	   config_profile is the configuration profile to create int the 
 *		namespace.  The entry_name will be placed in this config 
 *		profile.
 *	   namespace_syntax is the syntax to use when exporting interfaces to
 *		the namespace (default:  3.  3 maps up to rpc_c_ns_syntax_dce).
 *	   principal_name is the registery user that the server will register
 *		authentication info with.
 *	   authentication_service is the service that the server will register
 *		authentication info with (default:  1.  1 maps to dce private
 *		key authentication).
 *	   -N specifies to not use authenticated RPC.  This flag overrides the
 *		-O and -V flags.
 *		
 *	   (e.g.  server -S /.:/recov_entry -g /.:/recov_group -O recov_user 
 *			 -V 1)
 *	   (e.g.  server -P ncacn_ip_tcp -S /.:/recov_entry -p /.:/recov_profile
 *		         -O recov_user)
 *
 *   DESCRIPTION:
 *	The server registers the interface with the RPC runtime library and
 *	exports the interface to the namespace, using the entry name entered
 *	by the user.  It will create a service group or configuration profile
 *	and place the server entry in it (if the user specified to create a
 *	service group or config profile).  If the user specified authenticated
 *	RPC to be used, the server register the authentication info using
 *	the principal and the authentication level as specified by the user.
 *	(Note:  OSF/DCE 1.0 will not support authenticated RPC over the
 *	ncacn_ip_tcp protocol sequence).
 *
 *	The server receives a file name, offset, number of bytes and a buffer
 *	to put bytes.  The server opens the file, lseeks to the offset passed
 *	and reads the required number of bytes into the buffer passed.  The 
 *	file is closed and the buffer is passed back to the client.
 *
 *	When server is terminated, the interface is unregistered from the
 *	RPC runtime library and unexported from the namespace.  Any service
 *	groups or configuration profiles are removed from the namespace.
 *
 *	Note:  sed is run on this file replacing dummy_if with the proper
 *	       inteface name.  The sed call is done from the testcases driver
 *	       dcerprec.  This sed'ing is done because server.c serves both 
 *	       as the server for dummy_ifery and stress testcases.  sed is used 
 *	       so as to not have a seperate server.c file for dummy_ifery and
 *	       stress where the only difference would be the interface being
 *	       registered and exported.  If you wish to compile this file
 *	       without running the entire testcase, replace all occurences
 * 	       of dummy_if with either dummy_if or str, depending on which
 *	       flavor you wish to run (dummy_ifery or stress).
 *
 *   HISTORY:
 *      11/10/90        Gerald Cantor           Initial design/code.
 *	10/18/91	Gerald Cantor		Added sigset code to handle
 *						ctrl-C signal.
 *	10/19/91	Gerald Cantor		ANSIfied functions and updated
 *						prologs and comments.
 *	01/03/92	Gerald Cantor		Updated prologs.  Added code
 *						to create /tmp file when entries
 *						are exported to namespace.
 */
#include <dummy_if.h>
#include <stdio.h>
#include <fcntl.h>
#include <xx_lib.h>
#include <pthread.h>
#include <dce/rpc.h>
#include <util.h>

/*
 * Some global variables.
 */
extern char header[512], buf[128];

int entry_type;
boolean32 security;
char *prog; 
char *protocol; 
char *entry_name, *group_name, *profile_name; 
char *princ_name;
rpc_if_id_t if_id;
rpc_binding_vector_p_t bind_vec;
unsigned32 max_calls;
unsigned32 entry_syntax, group_syntax, member_syntax, profile_syntax; 
unsigned32 authn_svc;

main(int argc, char **argv)
{
	/*
 	 * Main's functions and variables.
	 */
	void init_vars(void), get_command_line_parameters(int, char **);
	void get_prot_seq(void), register_interface(void), cds_register(void);
	void cds_unregister(void);

	pthread_t this_thread = pthread_self();
	sigset_t sigset;
	char tmp_file[30];
	int file_status;
	error_status_t st, e_st;

	cma_init();
	prog = argv[0];
	make_header(prog);
	init_vars();
	get_command_line_parameters(argc, argv);
	get_prot_seq();
	register_interface();
	cds_register();

	switch(security)
	{
		case FALSE:
			break;
		
		case TRUE:
		default:
		{
			rpc_server_register_auth_info(princ_name,
						      authn_svc,
						      NULL,
						      NULL,
						      &st);
			sprintf(buf, "Cannot register auth info\n");
			check_status(st, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);
			break;
		}
	}

	xx_log("Listening for remote requests . . .");
	sigemptyset(&sigset);
        sigaddset(&sigset, SIGINT);
        if (pthread_signal_to_cancel_np(&sigset, &this_thread) != 0)
        {
                sprintf(buf, "%spthread_signal_to_cancel_np failed\n", header);
                xx_severe(buf);
                exit(1);
        }

	/* listen on the specified port for remote requests */
	TRY
	{
		sprintf(tmp_file, "/tmp/server.%d", getpid());
		file_status = creat(tmp_file, 0666);
		sprintf(buf, "Cannot create %s file\n", tmp_file);
                check_status(file_status, -1, EQUAL, buf, TRUE, NOT_RPC);
		rpc_server_listen(max_calls, &st);
	}
	CATCH_ALL
	{

		/*
                 * Remove the temp file.
                 */
                file_status = unlink(tmp_file);
                sprintf(buf, "Cannot remove %s file\n", tmp_file);
                check_status(file_status, -1, EQUAL, buf, FALSE, NOT_RPC);

		rpc_server_unregister_if(dummy_if_v0_0_s_ifspec, NULL, &st);
		if (st != rpc_s_ok)
		{
			xx_error ("Cannot unregister interface from RPC");	
			sprintf(buf, "%s", dce_error_inq_text(st, &e_st));
			xx_error(buf);
		}
		else
			xx_log ("Interface dummy_if unregistered from RPC");

		rpc_ep_unregister(dummy_if_v0_0_s_ifspec, bind_vec, NULL, &st);
		if (st != rpc_s_ok)
		{
			xx_error ("Cannot unregister interface from RPCD");	
			sprintf(buf, "%s", dce_error_inq_text(st, &e_st));
			xx_error(buf);
		}
		else
			xx_log ("Interface dummy_if unregistered from RPCD");

		cds_unregister();
	}
	ENDTRY
	exit(0);
}

/*
 * init_vars initialize some variables with default values.
 */
void init_vars()
{
	protocol = "ncadg_ip_udp";
	max_calls = 10;

	/* rpc_c_syntax_dce = 3 */
	entry_syntax = group_syntax = profile_syntax = member_syntax = 3;
	entry_type = SERVER_ENTRY;
	security = TRUE;
	authn_svc = 1;
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
	char *func = "get_command_line_parameters";

	
    	while( ( c = getopt( argc, argv, "P:C:S:g:p:E:O:V:NhH" ) ) != EOF )
	{
		switch( c )
		{
			case 'P':
	    			protocol = optarg;
	    			break;
			case 'C':
	    			max_calls = atol(optarg);
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
			case 'E':
     	    			entry_syntax = atol(optarg);
	    			group_syntax = profile_syntax = member_syntax = entry_syntax;
	    			break;
			case 'O':
	    			princ_name = optarg;
	    			break;
			case 'V':
	    			authn_svc = atol(optarg);
	    			break;
			case 'N':
	    			security = FALSE;
	    			break;
			case 'h':
			case 'H':
	    			show_usage();
	    			exit(0);
        	}
    	}
	
	if (entry_name == (char *)NULL)
	{
		sprintf(buf, "%s(%s):  An entry name must be specified\n", 
			header, func);
		xx_severe(buf);
		show_usage();
		exit(1);
	}
}

/*
 * print a usage statement for the tester.
 */
void show_usage()
{
	sprintf(buf, "Usage:  %s [ OPTIONS ]", prog);
	xx_log(buf);
	xx_log("Where [ OPTIONS ] are one or more of the following:");
	xx_log(" -P <protocol> protocol sequence to listen with");
	xx_log("	(default:  ncadg_ip_udp)");
	xx_log(" -C <max calls> number of concurrent listener threads");
	xx_log("	(default:  10)");
	xx_log(" -S <entry name> register interface in server entry");
	xx_log(" -g <service group> register interface in service group");
	xx_log(" -p <config profile> register interface in config profile");
	xx_log(" -E <namespace syntax> syntax to use when exporting");
	xx_log("	(default:  3.  3 maps to rpc_c_ns_syntax_dce)");
	xx_log(" -O <principal name> name to authenticate with");
	xx_log(" -V <auth service> authentication service to use");
	xx_log("	(default:  1.  1 maps to DCE private key auth)");
	xx_log(" -N use non-secure RPC (overrides -V flag)");
	xx_log(" tester must specify an entry name");
}

/*
 * get_prot_seq obtains a port to listen for remote requests on.
 */
void get_prot_seq()
{
	error_status_t st;

	rpc_server_use_protseq(protocol, max_calls, &st);
	sprintf(buf, "Cannot rpc_server_use_protseq.\n");
	check_status(st, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);
}

/*
 * register_interface registers the interface with the RPC runtime
 * library.
 */
void register_interface()
{
	error_status_t st;

	rpc_server_register_if(dummy_if_v0_0_s_ifspec, NULL, NULL, &st); 
	sprintf(buf, "Cannot register interface with RPC Library.\n");
	check_status(st, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);
}

/*
 * cds_register exports the inteface to the namespace.  If the tester specifies
 * that a service group or configuration profile is to be used, the group or
 * config profile is then created and the server entry is placed in it.  
 * Server entries, services groups and configuration profiles can be created
 * in foreign cells by specifying the name in /.../cellname/entry form.
 */
void cds_register()
{
	char *string_binding, *port, hostname[30];
	char *anno = "config profile";
	unsigned32 priority = 0;
	error_status_t status;
	int i, fd;

	rpc_server_inq_bindings(&bind_vec, &status);
	sprintf(buf, "Cannot rpc_server_inq_bindings\n");
	check_status(status, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);
  
	rpc_binding_to_string_binding(bind_vec->binding_h[0],
				      &string_binding,
				      &status);
	string_binding_to_name(string_binding, hostname, &port, NULL);

	rpc_ep_register(dummy_if_v0_0_s_ifspec, bind_vec, NULL,
                        "dummy_if interface", &status);
        sprintf(buf, "Cannot register endpoint\n");
        check_status(status, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);
	xx_log("Interface registered with endpoint mapper");

	rpc_ns_binding_export(entry_syntax,
                              entry_name,
                              dummy_if_v0_0_s_ifspec,
                              bind_vec,
                              (uuid_vector_t *) NULL,
                              &status);
       	sprintf(buf, "rpc_ns_binding_export failed\n");
       	check_status(status, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);
	sprintf(buf, "Entry %s was successfully created in namespace", 
		entry_name);
	xx_log(buf);

	switch(entry_type)
	{
		case SERVICE_GROUP:
		{
			rpc_ns_group_mbr_add(group_syntax,
					     group_name,
					     entry_syntax,
					     entry_name,
					     &status);
			sprintf(buf, "Cannot add member to group\n");
			check_status(status, rpc_s_ok, NOT_EQUAL, buf, TRUE, 
				     RPC);
			sprintf(buf, 
				"Successfully created group %s in namespace", 
				group_name);
			xx_log(buf);	
			break;
		}

		case CONFIG_PROFILE:
		{
			uuid_create(&(if_id.uuid), &status);
			sprintf(buf, "uuid_create for config profile failed");
		        check_status(status, rpc_s_ok, NOT_EQUAL, buf, TRUE, 
				     RPC);

        		if_id.vers_major = 1;
        		if_id.vers_minor = 1;

			rpc_ns_profile_elt_add(profile_syntax,
					       profile_name,
					       &if_id,
					       entry_syntax,
					       entry_name,
					       priority,
					       anno,
					       &status);
			sprintf(buf, "Cannot add %s to config profile %s\n",
				entry_name, profile_name);
			check_status(status, rpc_s_ok, NOT_EQUAL, buf, TRUE, 
				     RPC);
			sprintf(buf, 
				"Successfully created config profile %s in namespace", profile_name);
			xx_log(buf);	
			
			break;
		}
	}

	sprintf(buf, "Host=%s, port=%s\n", hostname, port);
	xx_log(buf);
}

/*
 * cds_unregister unexports the interface from the namespace and deletes 
 * any service groups or configuration profiles that were created.
 */
void cds_unregister()
{
	error_status_t st;
	char *func = "cds_unregister";

	switch(entry_type)
	{
		case SERVICE_GROUP:
		{
			rpc_ns_group_delete(group_syntax,
					    group_name,
					    &st);
			sprintf(buf, "(%s):  Cannot delete group %s", func, 
				group_name);
                        check_status(st, rpc_s_ok, NOT_EQUAL, buf, FALSE, RPC); 
			sprintf(buf, "Group %s deleted from namespace", 
				group_name);
			xx_log(buf);
			break;
		}

		case CONFIG_PROFILE:
		{
			rpc_ns_profile_delete(profile_syntax,
					      profile_name,
					      &st);
			sprintf(buf, "(%s):  Cannot remove config profile %s",	
				func, profile_name);
			check_status(st, rpc_s_ok, NOT_EQUAL, buf, FALSE, RPC);

			sprintf(buf, "Config profile %s deleted from namespace",
				profile_name);
			xx_log(buf);
			break;
		}
	}

	rpc_ns_binding_unexport(entry_syntax,
				entry_name,
				dummy_if_v0_0_s_ifspec, 
				(uuid_vector_t *) NULL,
				&st);
	
	sprintf(buf, "Cannot unexport interface");
	check_status(st, rpc_s_ok, NOT_EQUAL, buf, FALSE, RPC);
	sprintf(buf, "Entry %s unexported from namespace", entry_name);
	xx_log(buf);
}

/*
 * dummy_if_read opens the file passed to it, lseeks to the proper location
 * in the file as specified by the offset parameter and reads a number of bytes
 * (based on the buffer size parameter passed) into the buffer that is passed.
 * After the read, the file is closed.
 */
error_status_t dummy_if_read(handle_t h, idl_char *fname, idl_long_int offset, idl_char *br, idl_long_int buf_s)
{
	error_status_t st;
	int fdl, nb;

	fdl = open(fname, O_RDONLY, 0);
	sprintf(buf, "Cannot open %s.\n", fname);
	check_status(fdl, -1, EQUAL, buf, TRUE, RPC);

	/* get to the right place */
	lseek(fdl, offset, 0);
	nb = read(fdl, br, buf_s);
	close(fdl);
	sprintf(buf, "File read on server side failed.\n");
	check_status(fdl, -1, EQUAL, buf, TRUE, RPC);
}
