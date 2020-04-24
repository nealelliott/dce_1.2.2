/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: bankd.c,v $
 * Revision 1.1.20.2  1996/02/17  23:27:13  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:44:44  marty]
 *
 * Revision 1.1.20.1  1995/12/11  22:00:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:59:45  root]
 * 
 * Revision 1.1.17.2  1994/04/04  15:54:50  dassarma
 * 	Merged with changes from 1.1.17.1
 * 	[1994/04/04  15:54:44  dassarma]
 * 
 * 	Changes for OT10260
 * 	[1994/04/04  15:53:31  dassarma]
 * 
 * Revision 1.1.17.1  1994/02/23  21:43:49  pellis
 * 	Changed std.h to dce.h
 * 	[1994/02/23  21:38:40  pellis]
 * 
 * Revision 1.1.14.3  1993/10/14  17:37:06  cmckeen
 * 	For CR 7671, included <dce/std.h> for definition of TRUE and FALSE
 * 	[1993/10/14  17:35:21  cmckeen]
 * 
 * Revision 1.1.14.2  1993/09/22  19:17:47  pellis
 * 	Fixed problem with execution on a i486 machine.
 * 	[1993/09/22  19:17:04  pellis]
 * 
 * Revision 1.1.14.1  1993/09/15  14:20:06  pellis
 * 	Assorted fixes and enhancements made while debugging OSF/1 problem.
 * 	[1993/09/15  14:15:54  pellis]
 * 
 * Revision 1.1.10.2  1993/02/05  15:21:21  cjd
 * 	Embedded copyright notice
 * 	[1993/02/05  14:41:42  cjd]
 * 
 * Revision 1.1.8.2  1993/02/04  22:08:50  cjd
 * 	Embedded copyright notice
 * 
 * Revision 1.1.3.5  1992/07/17  21:59:33  root
 * 	  Changed order of operations when error is encountered to cleanly
 * 	  execute them.
 * 	[1992/07/17  21:03:18  root]
 * 
 * Revision 1.1.3.4  1992/05/21  18:08:17  gmd
 * 	Checking systest_sbox (shared sandbox) version.
 * 	[1992/05/21  16:48:08  gmd]
 * 
 * Revision 1.1.5.2  1992/05/15  21:43:12  gmd
 * 	Corrected test for INSUFFICIENT FUNDS - argument types are unsigned longs so
 * 	subtraction was never less than 0!
 * 	[1992/05/15  21:00:48  gmd]
 * 
 * Revision 1.1.3.3  1992/04/15  16:54:04  rps
 * 	Reorganize util routines
 * 	[1992/04/15  15:57:05  rps]
 * 
 * Revision 1.1.3.2  1992/03/18  01:19:58  rps
 * 	Fix idl parms
 * 	[1992/03/18  00:12:25  rps]
 * 
 * Revision 1.1  1992/01/19  14:37:29  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *
 *	FILE_NAME:  bankd.c
 *
 *	COMPONENT_NAME:  dce.systest
 *	
 *	FUNCTIONS:  show_usage()
 *		    process_command_line_arguments()
 *		    inquire_bindings()
 *		    register_ifs()
 *		    register_endpoint()
 *		    export_interface()
 *		    secure_the_rpc_calls()
 *		    admin_open_bank()
 *		    admin_write_bank()
 *		    admin_create_acct()
 *		    admin_delete_acct()
 *		    admin_inquire_acct()
 *		    res_deposit()
 *		    res_withdraw()
 *		    unres_deposit()
 *		    unres_withdraw()
 *		    find_acct()
 *
 *	ORIGINS:  2 27
 *
 *	(C) COPYRIGHT International Business Machines Corp. 1991
 *	All Rights Reserved
 *
 *	Copyright  1987 by Apollo Computer Inc., Chelmsford, Massachusetts
 *
 *	FOR_TEST:  dcerpbnk
 *
 *	USAGE:
 *		bankd -P<protocol> -B<bank name> -E <entry syntax> 
 *		      -S <admin entry name> -s <trans entry name> 
 *		      -V<auth service> -n<principal> -N 
 *
 *		where:
 *		  protocol is a valid protocol sequence  
 *			(default:  ncadg_ip_udp).
 *		  bank name is name of database file
 *		  entry syntax is the syntax to use when exporting to the
 *			namespace  (default:  3.  3 maps to rpc_c_ns_synax_dce).
 *		  admin entry name is the name you wish to export the admin
 *			interface as (bind and X.550 style names are allowed).
 *		  trans entry name is the name you wish to export the trans
 *			interface as (bind and X.550 style names are allowed).
 *		  auth service is the authentication service to use
 *			(default:  1.  1 maps to private key authentication).
 *		  principal is the registery user you wish to register 
 *			authentication info as (Note:  this user must exist
 *			in the security registery database).
 *		  -N specifies to not use authenticated RPC.  This flag will 
 *			override the -V and -n flags.
 *
 *		  (e.g. bankd -P ip -B baybanks -V 1 -n banker15)
 *
 *	Overview (the big picture):
 *
 *		This test case will test the Object Registry Table and
 *		the Interface Registry Table in the RPC library.  This
 *		test will contain two interfaces (admin and trans).
 *		These interfaces will act on four objects:  a checking
 *		account, a savings account, a cd account and an ira account.  
 *		The checking and savings account will have a type of unre-
 *		stricted.  The cd and ira account will have a type of re-
 *		stricted.  Each of these types will have different
 *		manager code to implement bank transactions.  The
 *		interfaces will be registered in such a manner as to
 *		force the RPC runtime library to query the internal
 *		tables in order to select the proper manager function.
 *
 *	Design:
 *		This process defines and initialized the entry point
 *		vectors for admin, restricted transactions and un-
 *		restricted transactions.  Command line arguments are
 *		processed and checked for correctness.  The bank name 
 *		object uuid is obtained based from the bank name the 
 *		tester passed.  All object and type uuid's are obtained.  
 *		The four account objects are are set with their respective 
 *		types (cd and ira get the restricted type, checking and 
 *		savings get the unrestricted type).  The interfaces are 
 *		registered with the proper type uuid and manager entry 
 *		point vector (this registering process places entries in 
 *		the two internal tables we are interested in testing).  
 *		Server bindings are obtained and this server registers 
 *		itself in the namespace, using the binding vector and the 
 *		bank name uuid.  Now the server begins listening on the ports.
 *		The rpc_server_listen() call is guarded by a TRY macro so as 
 *		to facilitate a graceful shutdown when a signal is received.  
 *		As request come in, the appropriate manager routines are 
 *		called.  When a signal is received the server unregisters 
 *		the interfaces from the RPC library, unexports the namespace 
 *		entry, unregisters the endpoints from the RPC database and 
 *		exits.
 *
 *	Change History
 *		4/16/91		Initial Design		Gerald Cantor
 *		4/17/91		Design Complete		Gerald Cantor
 *		5/01/91		Code Complete		Gerald Cantor
 *	       10/04/91		Added -S & -s flags	Gerald Cantor
 *	       10/04/91		Removed cma includes	Gerald Cantor
 *	       10/16/91		Added exc_handling.h	Gerald Cantor
 *	       10/18/91		Added sigset code to
 *				handle ctrl-C signal	Gerald Cantor
 *	       10/20/91		ANSIfied functions and
 *				updated comments and
 *				prologs.		Gerald Cantor
 *  	       10/20/91		Added account locking
 *				mutex.			Gerald Cantor
 *	       12/17/91		Added else if to 
 *				res_withdraw().		Gerald Cantor
 *	       12/30/91		Have admin_open_bank()
 *				return FAIL rather
 *				than exiting.  Create 
 *				file in /tmp to signal
 *				when the interfaces are
 *				exported.		Gerald Cantor
 */
#include <admin.h>
#include <trans.h>
#include <stdio.h>
#include <dce/rpc.h>
#include <dce/dce.h>
#include <pthread.h>
#include <bnk_util.h>
#include <util.h>
#define MAX_CALLS 10

/*
 * A few external variables.
 */
extern char buf[128], header[512];
extern uuid_t BankID;

/*
 * Define the manager entry point vector functions.
 */
error_status_t res_deposit(handle_t, char *, long); 
error_status_t unres_deposit(handle_t, char *, long); 
error_status_t res_withdraw(handle_t, char *, long); 
error_status_t unres_withdraw(handle_t, char *, long);

/*
 * Fill in the manager entry point vector.
 */
globaldef admin_v1_0_epv_t admin_epv = { admin_open_bank,
					 admin_write_bank,
					 admin_create_acct,
					 admin_delete_acct,
					 admin_inquire_acct
				       };

globaldef trans_v0_0_epv_t res_trans_epv = { res_deposit,
				             res_withdraw
					   };

globaldef trans_v0_0_epv_t unres_trans_epv = { unres_deposit,
					       unres_withdraw
					     };

/*
 * Some global functions and variables
 */
int find_acct(char *, acct_t **);

char *bank_name, *prog;
long NumAccts, NextAcct;
acct_t *head_ptr = (acct_t *)NULL;
uuid_t cd_id, ck_id, sv_id, ira_id;
uuid_t res_trans_id, unres_trans_id;
unsigned32 entry_syntax = 3;
pthread_mutex_t mutex;
unsigned32 auth_svc;

main(int argc, char **argv)
{

	/*
 	 * main's functions and variables.
	 */
	void process_command_line_arguments(int, char **, char **, unsigned32 *,
					    char **, char **, char **, 
					    unsigned32 *, char **, boolean32 *);
	void inquire_bindings(rpc_binding_vector_p_t *); 
	void register_ifs(uuid_t, uuid_t, uuid_t); 
	void register_endpoint(rpc_binding_vector_p_t, uuid_vector_p_t);
	void export_interface(rpc_if_handle_t, rpc_binding_vector_p_t,
			      uuid_vector_p_t, char *); 
	void secure_the_rpc_calls(char *, unsigned32);

	pthread_t this_thread = pthread_self();
	sigset_t sigset;
	boolean32 secure = true;
	uuid_vector_p_t obj_vec; 
	error_status_t st;
	rpc_binding_vector_p_t bind_vec;
	char tmp_file[20];
	char *proto = "ncadg_ip_udp", *princ_name;
	char *admin_entry_name, *trans_entry_name;
	int i, mutex_status, file_status;

	cma_init();
	prog = argv[0];
	make_header(prog);

	/*
	 * Initialize our global locking mutext.
	 */ 
	mutex_status = pthread_mutex_init (&mutex, pthread_mutexattr_default);
	sprintf(buf, "pthread_mutex_init failed\n");
	check_status(mutex_status, -1, EQUAL, buf, TRUE, NOT_RPC);

	obj_vec = (uuid_vector_p_t)malloc(sizeof(uuid_vector_t));

	process_command_line_arguments(argc, argv, &proto, &entry_syntax,
				       &admin_entry_name, &trans_entry_name, 
				       &bank_name, &auth_svc, &princ_name, 
				       &secure);

	rpc_server_use_protseq_if(proto, MAX_CALLS, admin_v1_0_s_ifspec, &st);
	sprintf(buf, "Cannot use protocol sequence %s for admin", proto);
	check_status(st, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);

	rpc_server_use_protseq_if(proto, MAX_CALLS, trans_v0_0_s_ifspec, &st);
	sprintf(buf, "Cannot use protocol sequence %s for trans", proto);
	check_status(st, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);

	BankNameToID(bank_name, &BankID);
	get_uuids(&ck_id, &sv_id, &cd_id, &ira_id, &res_trans_id, 
		  &unres_trans_id);	
	set_objects(&ck_id, &sv_id, &cd_id, &ira_id, &res_trans_id, 
		    &unres_trans_id);	
	build_object_vector(obj_vec, &BankID);
	inquire_bindings(&bind_vec);
	register_ifs(BankID, res_trans_id, unres_trans_id);
	register_endpoint(bind_vec, obj_vec);
	export_interface(admin_v1_0_s_ifspec, bind_vec, obj_vec,
			 admin_entry_name);
	export_interface(trans_v0_0_s_ifspec, bind_vec, NULL, 
			 trans_entry_name);
	sprintf(buf, "Cannot free binding vector");
	check_status(st, rpc_s_ok, NOT_EQUAL, buf, FALSE, RPC);

	if (secure)
		secure_the_rpc_calls(princ_name, auth_svc);

	sigemptyset(&sigset);
        sigaddset(&sigset, SIGINT);
        if (pthread_signal_to_cancel_np(&sigset, &this_thread) != 0)
        {
                sprintf(buf, "%spthread_signal_to_cancel_np failed\n", header);
                xx_severe(buf);
                exit(1);
        }
	TRY
	{
		sprintf(buf, "%sListening for remote requests . . .", header);
		xx_log(buf);
		sprintf(tmp_file, "/tmp/bankd.%d", getpid());
		file_status = creat(tmp_file, 0666);
		sprintf(buf, "Cannot create /tmp/bankd.%d file\n", getpid());
		check_status(file_status, -1, EQUAL, buf, TRUE, NOT_RPC);
		rpc_server_listen(MAX_CALLS, &st);
	}
	CATCH_ALL
	{

		/*
		 * Remove the temp file.
		 */
		file_status = unlink(tmp_file);
		sprintf(buf, "Cannot remove /tmp/bankd.%d file\n", getpid());
		check_status(file_status, -1, EQUAL, buf, FALSE, NOT_RPC);

		/*
		 * Unexport the interfaces from the namespace
		 */
		rpc_ns_binding_unexport(entry_syntax, 
					trans_entry_name,
					trans_v0_0_s_ifspec, 
					(uuid_vector_p_t)NULL,
					&st);
		sprintf(buf, "Cannot unexport trans interface\n");
		check_status(st, rpc_s_ok, NOT_EQUAL, buf, FALSE, RPC);

		rpc_ns_binding_unexport(entry_syntax, 
					admin_entry_name, 
					admin_v1_0_s_ifspec, 
					obj_vec,
					&st);
		sprintf(buf, "Cannot unexport admin interface\n");
		check_status(st, rpc_s_ok, NOT_EQUAL, buf, FALSE, RPC);

		/*
		 * Unregister the endpoints from RPCD
		 */
		rpc_ep_unregister(trans_v0_0_s_ifspec,
				  bind_vec,
				  NULL,
				  &st);
		sprintf(buf, "Cannot unregister trans endpoint\n");
		check_status(st, rpc_s_ok, NOT_EQUAL, buf, FALSE, RPC);	

		rpc_ep_unregister(admin_v1_0_s_ifspec,
				  bind_vec,
				  obj_vec,
				  &st);
		sprintf(buf, "Cannot unregister admin endpoint\n");
		check_status(st, rpc_s_ok, NOT_EQUAL, buf, FALSE, RPC);	

		rpc_binding_vector_free(&bind_vec, &st);

		/*
		 * Unregister interfaces from RPC runtime library
		 */
		rpc_server_unregister_if(trans_v0_0_s_ifspec, 
					 &unres_trans_id, 
					 &st);
		sprintf(buf, 
			"Cannot unregister unrest. trans interface\n");
		check_status(st, rpc_s_ok, NOT_EQUAL, buf, FALSE, RPC);

		rpc_server_unregister_if(trans_v0_0_s_ifspec, 
					 &res_trans_id, 
					 &st);
		sprintf(buf, 
			"Cannot unregister rest. trans interface\n");
		check_status(st, rpc_s_ok, NOT_EQUAL, buf, FALSE, RPC);

		rpc_server_unregister_if(admin_v1_0_s_ifspec, 
					 (uuid_p_t)NULL, 
					 &st);
		sprintf(buf, "Cannot unregister admin interface\n");
		check_status(st, rpc_s_ok, NOT_EQUAL, buf, FALSE, RPC);

		mutex_status = pthread_mutex_destroy(&mutex);
		sprintf(buf, "pthread_mutex_destroy failed\n");
		check_status(mutex_status, -1, EQUAL, buf, FALSE, NOT_RPC);
        }
	ENDTRY;
	exit(0);
}

/*
 * Show usage to the tester.
 */
void show_usage()
{
	sprintf(buf, "Usage:  %s [ OPTIONS ]", prog);
	xx_log(buf);
	xx_log("Where [ OPTIONS ] are one or more of the following:");
	xx_log(" -P <protocol>	    protocol string (default:  ncadp_ip_udp)");
	xx_log(" -E <entry syntax>  Namespace entry syntax  (default:  3)");
	xx_log(" -S <admin entry>   Name of admin namespace entry");
	xx_log(" -s <trans entry>   Name of trans namespace entry");
	xx_log(" -B <bank name>	    name of bank database file");
	xx_log(" -V <auth service>  authentication service (default: 1)");
	xx_log(" -n <princl name>   name to authenticate with");
	xx_log(" -N 		    Use non-secure RPC.  Overrides -V flag");
	xx_log("Tester must specify values for bank name and princ name");
}

/*
 * process_command_line_arguments parses the parameters passed by the user
 * and assigns them to variables.
 */
void process_command_line_arguments(int ac, char **av, char **protocol, 
				    unsigned32 *ent_syntax, char **admin_entry,
				    char **trans_entry, char **bank_n, 
				    unsigned32 *auth_s, char **p_name, 
				    boolean32 *secure)
{
	void show_usage(void);
	extern char *optarg;
	int c;

	while ((c = getopt(ac, av, "P:E:S:s:B:V:n:NhH")) != EOF)
	{
		switch(c)
		{
			case 'P':
				*protocol = optarg;
				break;
			case 'E':
				*ent_syntax = atol(optarg);
				break;
			case 'S':
				*admin_entry = optarg;
				break;
			case 's':
				*trans_entry = optarg;
				break;
			case 'B':
				*bank_n = optarg;
				break;
			case 'V':
				*auth_s = atol(optarg);
				break;
			case 'n':
				*p_name = optarg;
				break;
			case 'N':
				*secure = false;
				break;
			case 'h':
			case 'H':
				show_usage();
				exit(0);
		}
	}
}

/*
 * inquire_bindings shows what bindings this server will support.
 */
void inquire_bindings(rpc_binding_vector_p_t *bvec)
{
	error_status_t st;
	char *string_binding;
	int i;

	rpc_server_inq_bindings(bvec, &st);
	sprintf(buf, "Cannot inquire bindings\n");
	check_status(st, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);

	sprintf(buf, "%sThe following bindings are supported by %s\n",
		header, prog);
	xx_log(buf);

	for (i = 0; i < (*bvec)->count; i++)
	{
        	rpc_binding_to_string_binding((*bvec)->binding_h[i],
                                              &string_binding,
                                              &st);
		sprintf(buf, "%s\n", string_binding);
		xx_log(buf);
		rpc_string_free(&string_binding, &st);
		sprintf(buf, "Cannot free binding %s", string_binding);
		check_status(st, rpc_s_ok, NOT_EQUAL, buf, FALSE, RPC);
	}
}

/*
 * register_ifs registers the admin and trans interface with the RPC
 * runtime library.  The types are registered along with the interfaces.
 * This registeration populates the Interface Registery Table and the
 * Object Registery Table, which is what this code tests.
 */
void register_ifs(uuid_t bank_id, uuid_t rest_type, uuid_t unrest_type)
{
	error_status_t st;

	rpc_server_register_if(admin_v1_0_s_ifspec, (uuid_p_t)NULL, 
			       (rpc_mgr_epv_t)&admin_epv, &st);
	sprintf(buf, "rpc_server_register for admin failed\n");
	check_status(st, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);

	rpc_server_register_if(trans_v0_0_s_ifspec, &rest_type,
			       (rpc_mgr_epv_t)&res_trans_epv, &st);
	sprintf(buf, "rpc_server_register for res_trans failed\n");
	check_status(st, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);

	rpc_server_register_if(trans_v0_0_s_ifspec, &unrest_type,
			       (rpc_mgr_epv_t)&unres_trans_epv, &st);
	sprintf(buf, "rpc_server_register for unres_trans failed\n");
	check_status(st, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);
}

/*
 * register_endpoint registers the ports in the RPC database.
 */
void register_endpoint(rpc_binding_vector_p_t bvec, uuid_vector_p_t object_vec)
{
	error_status_t st;

	rpc_ep_register(admin_v1_0_s_ifspec, bvec, object_vec, 
			"admin interface", &st);
	sprintf(buf, "Cannot register endpoint\n");
	check_status(st, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);

	rpc_ep_register(trans_v0_0_s_ifspec, bvec, NULL, 
			"trans interface", &st);
	sprintf(buf, "Cannot register endpoint for trans\n");
	check_status(st, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);
}

/*
 * export_interface exports the admin and trans interface to the namespace.
 * The interfaces can be exported to a foreign cell, based on the format
 * of the parameter ent_name.  ent_name could be in simple form (e.g. 
 * entry_name), bind form (e.g. /.../cellname/entry_name) or X.500 style (e.g. 
 * /C=country/O=org/OU=org_unit/CN=entry_name
 */
void export_interface(rpc_if_handle_t iface, rpc_binding_vector_p_t bvec, 
		      uuid_vector_p_t object_vec, char *ent_name)
{
	error_status_t st;

	rpc_ns_binding_export(entry_syntax, ent_name, 
			      iface, bvec, (uuid_vector_p_t)object_vec, &st);
	sprintf(buf, "Cannot export %s\n", ent_name);
	check_status(st, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);
}

/*
 * secure_the_rpc_calls register authentication information with the RPC
 * runtime library.  This function will be called only if the tester
 * specifies a principal and an authentication level.
 */
void secure_the_rpc_calls(principal_name, auth_service)
char *principal_name;
unsigned32 auth_service;
{
	error_status_t status;

	rpc_server_register_auth_info((unsigned_char_p_t)principal_name, 
				      auth_service, 
				      (rpc_auth_key_retrieval_fn_t)NULL,
				      (idl_void_p_t)NULL, &status);
	sprintf(buf, "Cannot register auth info\n");
	check_status(status, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);
}

/*
 * admin_open_bank reads in bank account records from the file specified by
 * the tester and constructs a double circular linked list with those accounts.
 * If the file has no accounts, the list is set to NULL.
 */
error_status_t admin_open_bank(handle_t h)
{

	FILE *f;
	int mutex_status, acct_mutex_status; 
	int mutex_size = sizeof(pthread_mutex_t);
	acct_t *traverse_ptr = NULL, *new = NULL;
	char buff[100];
	long tot;

	mutex_status = pthread_mutex_lock(&mutex);	
	sprintf(buf, "pthread_mutex_lock failed\n");
	check_status(mutex_status, -1, EQUAL, buf, TRUE, NOT_RPC);

	if (head_ptr != (acct_t *)NULL)
	{
		/* Bank already opened */
		mutex_status = pthread_mutex_unlock(&mutex);
		sprintf(buf, "pthread_mutex_unlock failed\n");
		check_status(mutex_status, -1, EQUAL, buf, TRUE, NOT_RPC);
		return(rpc_s_ok);
	}

	f = fopen(bank_name, "r");
	if (f == NULL)
	{
		printf("Cannot open bank file baybanks\n");
		return(FAIL);
	}

	fscanf(f, "%ld\n", &NextAcct);

	tot = 0;

	new = (acct_t *)malloc(sizeof(acct_t));
	while (EOF != fscanf(f, "%s %d %d %d %d %d %d %d %d %d %d\n",
				buff,
				&(new->acct_info.number),
				&(new->acct_info.ck_balance),
				&(new->acct_info.ck_trans_time),
				&(new->acct_info.sv_balance),
				&(new->acct_info.sv_trans_time),
				&(new->acct_info.cd_balance),
				&(new->acct_info.cd_trans_time),
				&(new->acct_info.ira_balance),
				&(new->acct_info.ira_trans_time),
				&(new->acct_info.create_time)))
	{
		mutex_size = sizeof(pthread_mutex_t);
                new->acct_mutex = (idl_byte *)malloc(mutex_size);
                acct_mutex_status = pthread_mutex_init((pthread_mutex_t *)new->acct_mutex, pthread_mutexattr_default);
                sprintf(buf, "pthread_mutex_init for account failed\n");
                check_status(acct_mutex_status, -1, EQUAL, buf, TRUE, NOT_RPC);

		new->acct_info.name = SAlloc(buff);
		new->next = NULL;
		tot += new->acct_info.ck_balance;
		tot += new->acct_info.sv_balance;
		tot += new->acct_info.cd_balance;
		tot += new->acct_info.ira_balance;
		NumAccts++;
		if (head_ptr == NULL)
		{
			head_ptr = new;
			head_ptr->next = head_ptr;
			head_ptr->prev = head_ptr;
			traverse_ptr = head_ptr;
		}
		else
		{
			traverse_ptr->next = new;
			new->prev = traverse_ptr;
			new->next = head_ptr;
			head_ptr->prev = new;
			traverse_ptr = new; 
		}
		new = (acct_t *)malloc(sizeof(acct_t));
	}
	fclose(f);
	mutex_status = pthread_mutex_unlock(&mutex);
	sprintf(buf, "pthread_mutex_unlock failed\n");
	check_status(mutex_status, -1, EQUAL, buf, TRUE, NOT_RPC);
	if (head_ptr == (acct_t *)NULL)
		return(EMPTY_BANK);
	printf("Loaded %ld accounts, total balance is %ld\n", NumAccts, tot);
	return(rpc_s_ok);
}

/*
 * admin_write_bank traverses the bank linked list and writes the accounts
 * to the file specified by the user.
 */
error_status_t admin_write_bank(handle_t h)
{
	FILE *fp;
	acct_t *trav_ptr = head_ptr; 
	char new_bank[256];
	int Accounts = 0, mutex_status, acct_mutex_status;

	mutex_status = pthread_mutex_lock(&mutex);	
	sprintf(buf, "pthread_mutex_lock failed\n");
	check_status(mutex_status, -1, EQUAL, buf, TRUE, NOT_RPC);

	sprintf(buf, "%sDumping \"%s\"...\n", header, bank_name);
	xx_log(buf);

	sprintf(new_bank, "%s.new", bank_name);
	fp = fopen(new_bank, "w");
	if (fp == NULL)
	{
		printf("Cannot open %s for writing\n", new_bank);
		mutex_status = pthread_mutex_unlock(&mutex);
		sprintf(buf, "pthread_mutex_unlock failed\n");
		check_status(mutex_status, -1, EQUAL, buf, TRUE, NOT_RPC);
		return(FAIL);	
	}

	fprintf(fp, "%ld\n", NextAcct); 
	if (trav_ptr != (acct_t *)NULL)
	{
		do
		{
			fprintf(fp, "%s %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld\n",
				trav_ptr->acct_info.name,
				trav_ptr->acct_info.number,
				trav_ptr->acct_info.ck_balance,
				trav_ptr->acct_info.ck_trans_time,
				trav_ptr->acct_info.sv_balance,
				trav_ptr->acct_info.sv_trans_time,
				trav_ptr->acct_info.cd_balance,
				trav_ptr->acct_info.cd_trans_time,
				trav_ptr->acct_info.ira_balance,
				trav_ptr->acct_info.ira_trans_time,
				trav_ptr->acct_info.create_time);
				Accounts++;

				trav_ptr = trav_ptr->next;
		} while (trav_ptr != head_ptr);
	}
	fclose(fp);

	if (rename(new_bank, bank_name) < 0)
	{
        	sprintf(buf, "%s(%s)Can't rename %s to %s\n", header, 
			prog, new_bank, bank_name);
		xx_severe(buf);
		mutex_status = pthread_mutex_unlock(&mutex);
		sprintf(buf, "pthread_mutex_unlock failed\n");
		check_status(mutex_status, -1, EQUAL, buf, TRUE, NOT_RPC);
        	return(FAIL);
    	}

    	sprintf(buf, "%sDumped %d accounts\n", header, Accounts);
	xx_log(buf);
	mutex_status = pthread_mutex_unlock(&mutex);
	sprintf(buf, "pthread_mutex_unlock failed\n");
	check_status(mutex_status, -1, EQUAL, buf, TRUE, NOT_RPC);

	return(rpc_s_ok);	
}

/*
 * admin_create_account mallocs a new list node, fills in the fields and 
 * incorperates the node into the list.
 */
error_status_t admin_create_acct(handle_t h, idl_char *name)
{
	int mutex_status, acct_mutex_status;
	int mutex_size = sizeof(pthread_mutex_t);
	error_status_t status;
	acct_t *acct_ptr, *new_acct;
	long current_time;
	char *func = "admin_create_acct";

	mutex_status = pthread_mutex_lock(&mutex);	
	sprintf(buf, "pthread_mutex_lock failed\n");
	check_status(mutex_status, -1, EQUAL, buf, TRUE, NOT_RPC);

	if (!find_acct(name, &acct_ptr))
	{
		new_acct = (acct_t *)malloc(sizeof(acct_t));
		new_acct->acct_mutex = (idl_byte *)malloc(mutex_size);
                acct_mutex_status = pthread_mutex_init((pthread_mutex_t *)new_acct->acct_mutex, pthread_mutexattr_default);
                sprintf(buf, "(%s):  pthread_mutex_init for account failed\n",
                        func);
                check_status(acct_mutex_status, -1, EQUAL, buf, TRUE, NOT_RPC);

		new_acct->acct_info.name = SAlloc(name);
		new_acct->acct_info.number = NextAcct++;
		new_acct->acct_info.ck_balance = 0;
		new_acct->acct_info.sv_balance = 0;
		new_acct->acct_info.cd_balance = 0;
		new_acct->acct_info.ira_balance = 0;
		current_time = time(NULL);
		new_acct->acct_info.ck_trans_time = current_time;
		new_acct->acct_info.sv_trans_time = current_time;
		new_acct->acct_info.cd_trans_time = current_time;
		new_acct->acct_info.ira_trans_time = current_time;
		new_acct->acct_info.create_time = current_time;
		if (head_ptr == (acct_t *)NULL)
		{
			head_ptr = new_acct;
			head_ptr->next = new_acct;
			head_ptr->prev = new_acct;
		}
		else
		{
			(head_ptr->prev)->next = new_acct;
			new_acct->next = head_ptr;
			new_acct->prev = head_ptr->prev;
			head_ptr->prev = new_acct;
		}
		status = rpc_s_ok;
	}
	else
		status = ACCOUNT_EXISTS;
	mutex_status = pthread_mutex_unlock(&mutex);
	sprintf(buf, "pthread_mutex_unlock failed\n");
	check_status(mutex_status, -1, EQUAL, buf, TRUE, NOT_RPC);
	return(status);
}

/*
 * admin_delete_acct removes a node from the list and frees that node.
 */
error_status_t admin_delete_acct(handle_t h, idl_char *name)
{
	int mutex_status;
	acct_t *acct_ptr;
	error_status_t status;

	mutex_status = pthread_mutex_lock(&mutex);	
	sprintf(buf, "pthread_mutex_lock failed\n");
	check_status(mutex_status, -1, EQUAL, buf, TRUE, NOT_RPC);

	if (find_acct(name, &acct_ptr))
	{
		/* special case of only one account to delete */
		if (acct_ptr == acct_ptr->next)
			head_ptr = (acct_t *)NULL;

		else
		{
			(acct_ptr->prev)->next = acct_ptr->next;
			(acct_ptr->next)->prev = acct_ptr->prev;
	
			/* may need to reset head_ptr */
			if (acct_ptr == head_ptr)
				head_ptr = head_ptr->next;	
		}

		free(acct_ptr);
		NextAcct--;
		status = rpc_s_ok;
	}
	else
		status = ACCT_NOT_FOUND;
	mutex_status = pthread_mutex_unlock(&mutex);
	sprintf(buf, "pthread_mutex_unlock failed\n");
	check_status(mutex_status, -1, EQUAL, buf, TRUE, NOT_RPC);
	return(status);
}

/*
 * res_deposit is the manager for the restricted deposit call.  This manager
 * routine only acts on the cd and ira objects.  Deposit amounts must be
 * in multiples of $500.00.  If they are not, this call will not be successful.
 */
error_status_t res_deposit(handle_t h, char *acct_name, long amount)
{
	int mutex_status;
	acct_t *acct_ptr;
	error_status_t status;
	char *func = "res_deposit";
/*
	unsigned_char_p_t uuid_string = NULL;
	unsigned32 rpc_status;
 */

	if (find_acct(acct_name, &acct_ptr))
	{
		mutex_status = pthread_mutex_lock((pthread_mutex_t *)(acct_ptr->acct_mutex));
                sprintf(buf, "Error in locking account %s\n", func, acct_name);
                check_status(mutex_status, -1, EQUAL, buf, TRUE, NOT_RPC);

/*
		uuid_to_string (
				get_handle_obj(h),
				&uuid_string,
				&rpc_status);
		printf ("%s get_handle_obj = %s\n\n", acct_name, uuid_string);
		uuid_to_string (
				&cd_id,
				&uuid_string,
				&rpc_status);
		printf ("%s          cd_id = %s\n\n", acct_name, uuid_string);
		uuid_to_string (
				&ira_id,
				&uuid_string,
				&rpc_status);
		printf ("%s         ira_id = %s\n\n", acct_name, uuid_string);
 */

		if ((amount % 500) != 0)
			status = BAD_DEPOSIT_AMOUNT;
		else
		{
			if (!(bcmp(get_handle_obj(h), &cd_id, sizeof(uuid_t))))
			{
/*
				printf("res_deposit: %s CD, %d\n", acct_name, amount);
 */
				acct_ptr->acct_info.cd_balance += amount;
			}
			else
			{
/*
				printf("res_deposit: %s IRA, %d\n", acct_name, amount);
 */
				acct_ptr->acct_info.ira_balance += amount;
			}
			status = rpc_s_ok;
		}

		mutex_status = pthread_mutex_unlock((pthread_mutex_t *)(acct_ptr->acct_mutex));
                sprintf(buf, "Error in unlocking account %s\n", func,
                        acct_name);
                check_status(mutex_status, -1, EQUAL, buf, TRUE, NOT_RPC);

	}
	else
		status = ACCT_NOT_FOUND;
	return(status);
}

/*
 * res_withdraw is the manager for the restricted withdraw call.  This manager
 * routine only acts on the cd and ira objects.  Withdraws cannot be attempted 
 * until the accounts are 900 seconds old (a simulation of real cd's and ira's
 * time restrictions).  If the account is not "mature", this call will not be 
 * successful.
 */
error_status_t res_withdraw(handle_t h, char *acct_name, long amount)
{
	int mutex_status;
	acct_t *acct_ptr;
	error_status_t status;
	char *func = "res_withdraw";
/*
	unsigned_char_p_t uuid_string = NULL;
	unsigned32 rpc_status;
 */

	if (find_acct(acct_name, &acct_ptr))
	{
		mutex_status = pthread_mutex_lock((pthread_mutex_t *)(acct_ptr->acct_mutex));
                sprintf(buf, "Error in locking account %s\n", func, acct_name);
                check_status(mutex_status, -1, EQUAL, buf, TRUE, NOT_RPC);

/*
		uuid_to_string (
				get_handle_obj(h),
				&uuid_string,
				&rpc_status);
		printf ("%s get_handle_obj = %s\n\n", acct_name, uuid_string);
		uuid_to_string (
				&cd_id,
				&uuid_string,
				&rpc_status);
		printf ("%s          cd_id = %s\n\n", acct_name, uuid_string);
		uuid_to_string (
				&ira_id,
				&uuid_string,
				&rpc_status);
		printf ("%s         ira_id = %s\n\n", acct_name, uuid_string);
 */

		if (time(NULL) - acct_ptr->acct_info.create_time < 900)
			status = UNMATURE_ACCOUNT;
		else if (!(bcmp(get_handle_obj(h), &cd_id, sizeof(uuid_t))))
		{
			if ((acct_ptr->acct_info.cd_balance < amount))
				status = INSUFFICIENT_FUNDS;
			else
			{
/*
				printf("res_withdraw: %s CD, %d\n", acct_name, amount);
 */
				acct_ptr->acct_info.cd_balance -= amount;
				status = rpc_s_ok;
			}
		}
		else
		{
			if ((acct_ptr->acct_info.ira_balance < amount))
				status = INSUFFICIENT_FUNDS;
			else
			{
/*
				printf("res_withdraw: %s IRA, %d\n", acct_name, amount);
 */
				acct_ptr->acct_info.ira_balance -= amount;
				status = rpc_s_ok;
			}
		}

		mutex_status = pthread_mutex_unlock((pthread_mutex_t *)(acct_ptr->acct_mutex));
                sprintf(buf, "Error in unlocking account %s\n", func,
                        acct_name);
                check_status(mutex_status, -1, EQUAL, buf, TRUE, NOT_RPC);
	}
	else
		status = ACCT_NOT_FOUND;
	return(status);
}

/*
 * unres_deposit is the manager for the unrestricted deposit call.  This manager
 * routine only acts on the checking and savings objects.  Deposit amounts can 
 * be any amount.
 */
error_status_t unres_deposit(handle_t h, char *acct_name, long amount)
{
	int mutex_status;
	error_status_t status;
	acct_t *acct_ptr;
	char *func = "unres_deposit";
/*
	unsigned_char_p_t uuid_string = NULL;
	unsigned32 rpc_status;
 */

	if (find_acct(acct_name, &acct_ptr))
	{
		mutex_status = pthread_mutex_lock((pthread_mutex_t *)(acct_ptr->acct_mutex));
                sprintf(buf, "Error in locking account %s\n", func, acct_name);
                check_status(mutex_status, -1, EQUAL, buf, TRUE, NOT_RPC);

/*
		uuid_to_string (
				get_handle_obj(h),
				&uuid_string,
				&rpc_status);
		printf ("%s get_handle_obj = %s\n\n", acct_name, uuid_string);
		uuid_to_string (
				&ck_id,
				&uuid_string,
				&rpc_status);
		printf ("%s          ck_id = %s\n\n", acct_name, uuid_string);
		uuid_to_string (
				&sv_id,
				&uuid_string,
				&rpc_status);
		printf ("%s          sv_id = %s\n\n", acct_name, uuid_string);
 */

		if (!(bcmp(get_handle_obj(h), &ck_id, sizeof(uuid_t))))
		{
/*
			printf("unres_deposit: %s CK, %d\n", acct_name, amount);
 */
			acct_ptr->acct_info.ck_balance += amount;
		}
		else
		{
/*
			printf("unres_deposit: %s SV, %d\n", acct_name, amount);
 */
			acct_ptr->acct_info.sv_balance += amount;
		}

		mutex_status = pthread_mutex_unlock((pthread_mutex_t *)(acct_ptr->acct_mutex));
                sprintf(buf, "Error in unlocking account %s\n", func,
                        acct_name);
                check_status(mutex_status, -1, EQUAL, buf, TRUE, NOT_RPC);

		status = rpc_s_ok;
	}
	else
		status = ACCT_NOT_FOUND;
	return(status);
}
		
/*
 * unres_withdraw is the manager for the unrestricted withdraw call.  This 
 * manager routine only acts on the checking and savings objects.  Withdraws 
 * can be attempted at any time.
 */
error_status_t unres_withdraw(handle_t h, char *acct_name, long amount)
{
	int mutex_status;
	error_status_t status;
	acct_t *acct_ptr;
	char *func = "unres_withdraw";
/*
	unsigned_char_p_t uuid_string = NULL;
	unsigned32 rpc_status;
 */
	
	if (find_acct(acct_name, &acct_ptr))
	{
		mutex_status = pthread_mutex_lock((pthread_mutex_t *)(acct_ptr->acct_mutex));
                sprintf(buf, "Error in locking account %s\n", func, acct_name);
                check_status(mutex_status, -1, EQUAL, buf, TRUE, NOT_RPC);

/*
		uuid_to_string (
				get_handle_obj(h),
				&uuid_string,
				&rpc_status);
		printf ("%s get_handle_obj = %s\n\n", acct_name, uuid_string);
		uuid_to_string (
				&ck_id,
				&uuid_string,
				&rpc_status);
		printf ("%s          ck_id = %s\n\n", acct_name, uuid_string);
		uuid_to_string (
				&sv_id,
				&uuid_string,
				&rpc_status);
		printf ("%s          sv_id = %s\n\n", acct_name, uuid_string);
 */

		if (!(bcmp(get_handle_obj(h), &ck_id, sizeof(uuid_t))))
		{
			if ((acct_ptr->acct_info.ck_balance < amount))
				status = INSUFFICIENT_FUNDS;
			else
			{
/*
				printf("unres_withdraw: %s CK, %d\n", acct_name, amount);
 */
				acct_ptr->acct_info.ck_balance -= amount;
				status = rpc_s_ok;
			}
		}
		else
		{
			if ((acct_ptr->acct_info.sv_balance < amount))
				status = INSUFFICIENT_FUNDS;
			else
			{
/*
				printf("unres_withdraw: %s SV, %d\n", acct_name, amount);
 */
				acct_ptr->acct_info.sv_balance -= amount;
				status = rpc_s_ok;
			}
		}

		mutex_status = pthread_mutex_unlock((pthread_mutex_t *)(acct_ptr->acct_mutex));
                sprintf(buf, "Error in unlocking account %s\n", func,
                        acct_name);
                check_status(mutex_status, -1, EQUAL, buf, TRUE, NOT_RPC);
	}
	else
		status = ACCT_NOT_FOUND;
	return(status);
}

/*
 * admin_inquire_acct pulls the account information out of the proper
 * node of the list and sends it back to the client.
 */
error_status_t admin_inquire_acct(handle_t h, idl_char *acct_name,
				  acct_info_t *acct)
{
	int mutex_status, length;
	error_status_t status;
	acct_t *acct_ptr;
	char *func = "admin_inquire_acct";

	printf("Finding account %s\n", acct_name);
	if (find_acct(acct_name, &acct_ptr))
	{
	/*	bcopy(acct, acct_ptr->acct_info, sizeof(acct_info_t));*/

		mutex_status = pthread_mutex_lock((pthread_mutex_t *)(acct_ptr->acct_mutex));
                sprintf(buf, "Error in locking account %s\n", func, acct_name);
                check_status(mutex_status, -1, EQUAL, buf, TRUE, NOT_RPC);

		acct->name = SAlloc(acct_ptr->acct_info.name);
		acct->number = acct_ptr->acct_info.number;
		acct->ck_balance = acct_ptr->acct_info.ck_balance;
		acct->ck_trans_time = acct_ptr->acct_info.ck_trans_time;
		acct->sv_balance = acct_ptr->acct_info.sv_balance;
		acct->sv_trans_time = acct_ptr->acct_info.sv_trans_time;
		acct->cd_balance = acct_ptr->acct_info.cd_balance;
		acct->cd_trans_time = acct_ptr->acct_info.cd_trans_time;
		acct->ira_balance = acct_ptr->acct_info.ira_balance;
		acct->ira_trans_time = acct_ptr->acct_info.ira_trans_time;
		acct->create_time = acct_ptr->acct_info.create_time;

		mutex_status = pthread_mutex_unlock((pthread_mutex_t *)(acct_ptr->acct_mutex));
                sprintf(buf, "Error in unlocking account %s\n", func,
                        acct_name);
                check_status(mutex_status, -1, EQUAL, buf, TRUE, NOT_RPC);

		status = rpc_s_ok;
	}
	else
		status = ACCT_NOT_FOUND;
	return(status);
}

/*
 * find_acct searches the list of accounts until it finds the account that
 * is being requested.  When located, the address of the node is returned.
 */
int find_acct(char *name, acct_t **account)
{
	acct_t *trav_ptr = head_ptr;

	if (head_ptr == (acct_t *)NULL)
		return (0);
	do
	{
		if (strcmp(name, trav_ptr->acct_info.name) == 0)
		{
			*account = trav_ptr;
			return (1);
		}
		trav_ptr = trav_ptr->next;
	} while (trav_ptr != head_ptr);
	return(0);
}
