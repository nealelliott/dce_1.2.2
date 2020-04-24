/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: bank.c,v $
 * Revision 1.1.21.2  1996/02/17  23:27:11  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:44:43  marty]
 *
 * Revision 1.1.21.1  1995/12/11  22:00:35  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1994/08/09  17:15 UTC  bissen
 * 	fix merge: include dce.h
 * 
 * 	HP revision /main/HPDCE02/1  1994/06/29  20:13 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 
 * 	HP revision /main/HPDCE01/1  1994/01/27  15:51  bissen
 * 	merge kk and hpdce01
 * 	[1995/12/11  20:59:43  root]
 * 
 * Revision 1.1.8.2  1993/07/09  12:26:28  root
 * 	Initial King Kong branch
 * 	[1993/07/09  12:25:45  root]
 * 
 * Revision 1.1.14.2  1993/07/28  19:32:39  cmckeen
 * 	HP's TET'ized version of the system tests
 * 	[1993/07/28  14:24:08  cmckeen]
 * 
 * Revision 1.1.10.3  1993/05/27  18:48:57  eheller
 * 	*** empty log message ***
 * 
 * Revision 1.1.10.2  1993/05/27  13:51:22  eheller
 * 	*** empty log message ***
 * 	[1993/05/26  15:40:38  eheller]
 * 
 * Revision 1.1.6.2  1993/04/23  14:42:08  giza
 * 	dce1.0.2 merge up and timing mods
 * 	[1993/04/23  13:42:24  giza]
 * 
 * Revision 1.1.3.2  1992/06/11  19:53:56  mcfarland
 * 	changed files from test_latest
 * 	[1992/06/09  13:01:25  mcfarland]
 * 
 * 	Checking systest_sbox (shared sandbox) version.
 * 	[1992/05/21  16:47:45  gmd]
 * 
 * 	Reorganize util routines
 * 	[1992/04/15  15:56:51  rps]
 * 
 * 	Fix includes
 * 	[1992/03/18  00:12:18  rps]
 * 
 * Revision 1.1.1.2  1992/05/28  15:12:02  root
 * 	    Loading drop Dce1_0_1b15
 * 
 * Revision 1.1.4.2  1992/05/15  21:42:54  gmd
 * 	Replaced non-thread safe buffer allocation (global message character pointer)
 * 	with local character arrays in main(), DoDeposit(), DoWithdraw(), DoInquire().
 * 
 * 	Added write_and_exit() routine for portable way to pass transaction status
 * 	to transactions executable.
 * 	[1992/05/15  20:58:32  gmd]
 * 
 * Revision 1.1.2.3  1992/04/15  16:53:58  rps
 * 	Reorganize util routines
 * 	[1992/04/15  15:56:51  rps]
 * 
 * Revision 1.1.2.2  1992/03/18  01:19:42  rps
 * 	Fix includes
 * 	[1992/03/18  00:12:18  rps]
 * 
 * Revision 1.1  1992/01/19  14:37:31  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *
 *      FILE_NAME:  bank.c
 *
 *      COMPONENT_NAME:  dce.systest
 *
 *      FUNCTIONS:  show_usage()
 *      	    process_command_line_arguments()
 *                  assign_account_type()
 *                  get_account_uuid()
 *                  import_interface()
 *		    secure_handles()
 *		    check_transaction()
 *                  DoDeposit()
 *                  DoWithdraw()
 *                  DoInquire()
 *
 *      ORIGINS:  2 27
 *
 *      (C) COPYRIGHT International Business Machines Corp. 1991
 *      All Rights Reserved
 *
 *	Copyright  1987 by Apollo Computer Inc., Chelmsford, Massachusetts
 *
 *      FOR_TEST:  dcerpbnk
 *
 *      USAGE:
 *              bank -T <transaction>
 *              -B <bank name> 
 *		-n <account name> 
 *		-a <account type> 
 *		-D <dollar amount>
 *		-p <principal>
 *		-S <admin server entry>
 *		-s <trans server entry>
 *		-A <authentication level>
 *		-V <authentication service>
 *		-Z <authorization service>
 *		-N
 *
 *              where:
 *		  transaction is one of the following:
 *			create, delete, inquire, deposit, withdraw.
 *		  bank name is the bank to conduct transaction
 *		  account name is whose account to transact.
 *		  dollar amount is amount to deposit or withdraw (valid
 *		  only with deposit or withdraw transaction).
 *		  principal is the security registery user to set binding
 *		  	info with.
 *		  admin server entry is the name of the server entry for the
 *			admin interface (entry can have bind or X.500 style
 *			format).
 *		  trans server entry is the name of the server entry for the
 *			trans interface (entry can have bind or X.500 style
 *			format).
 *		  authentication level is level to use in setting the binding
 *			info (default:  0.  0 maps to the default level of
 *			authentication).
 *		  authentication service is the type of service to authenticate
 *			with (default:  1.  1 maps to private key authenti-
 *			cation).
 *		  authorization service is the type of authorization service
 *			to use (default:  2.  2 maps to DCE PAC authorization).
 *		  -N specifies to not use authenticated RPC.  This flag
 *		  	overrides the flags -p, -A, -V and -Z.
 *
 *                (e.g. bank -T create -B baybanks -n Gerald -p sam -A 2 
 *		             -V 1 -Z 1)
 *		  (e.g. bank -T deposit -B baybanks -n Gerald -a checking 
 *			-D 100 -N) 
 *
 *      Overview (the big picture):
 *
 *              This test case will test the object registry table and
 *              the Interface registry table in the RPC library.  This
 *              test will contain two interfaces (admin and trans).
 *              These interfaces will act on four objects:  a checking
 *              account, a savings account, a cd account and an ira account.
 *		The checking and savings account will have a type of unre-
 *              stricted.  The cd and ira account will have a type of re-
 *              stricted.  Each of these types will have different
 *              manager code to implement bank transactions.  The
 *              interfaces will be registered in such a manner as to
 *              force the RPC runtime library to query the internal
 *              tables in order to select the proper manager function.
 *
 *      Design:
 *		This code functions as the bank client.  bank
 *		will call the functions exported by the bank server.
 *		A command table is initialized with procedure names.
 *		After the command line parameters are verified, the
 *		bank name object uuid and account type uuid are
 *		obtained.  The namespace is queried for the location
 *		of a suitable bank server.  This handle is returned
 *		sets a context handle which is used in the other admin
 *		interface calls e.g. admin_write_bank).  If the user
 *		specifies to create an account, that function is called
 *		outright and bank exits accordingly.  If any other
 *		command is requested, the binding handle's object field
 *		is set to the account type uuid, the command table is
 *		searched and the appropriate function is called with any
 *		coresponding parameters.
 *
 *	Change History
 *		4/16/91		Initial Design		Gerald Cantor
 *		4/17/91		Design Complete		Gerald Cantor
 *		5/01/91		Code Complete		Gerald Cantor
 *	       10/04/91		Added -S & -s flags	Gerald Cantor
 *	       10/04/91		Removed cma includes  	Gerald Cantor
 *	       10/16/91		Added exc_handling.h  	Gerald Cantor
 *	       10/20/91		ANSIfied functions and
 *				updated prologs and
 *				comments.		Gerald Cantor
 *	       12/18/91		Added TRY ... CATCH
 *				blocks around RPC's.	Gerald Cantor
 */
#include <admin.h>
#include <trans.h>
#include <dce/rpc.h>
#include <dce/rpcexc.h>
#include <dce/dce_error.h>
#include <dce/dce.h>
#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <systest.h>
#include <bnk_util.h>
#include <util.h>

/*
 * Global functions and variables.
 */
extern char buf[128], header[512];
extern uuid_t BankID;

static void DoDeposit(handle_t, char *, long, error_status_t *); 
static void DoWithdraw(handle_t, char *, long, error_status_t *); 
static void DoInquire(handle_t, char *, long, error_status_t *);
void show_usage(void);
int check_transaction(error_status_t, char *);

static struct cmd_t
{
	char name[20];
	void (*proc)();
} CmdTable[] = {
	{"deposit", DoDeposit},
	{"withdraw", DoWithdraw},
	{"inquire", DoInquire},
	{"", NULL}
};


char *prog;
unsigned32 entry_syntax = 3; 
unsigned32 auth_svc = 1, auth_level = 0, authz_svc = 2;

int write_and_exit(status)
int status;
{
  FILE *sfp;
  char buffer[100];

  sprintf(buffer,"transaction_exit_status.%d",getpid());

  sfp = fopen(buffer,"a");
  if (sfp != NULL)
  {
    fprintf(sfp,"%d",status);
    fclose(sfp);
  }
  exit(status);
}

main(int argc, char **argv)
{

	void process_command_line_arguments(int, char **, char **, char **, 
					    char **, char **, long *, 
					    unsigned32 *, char **, char **,
					    unsigned32 *, unsigned32 *,
					    unsigned32 *, char **, boolean32 *);
	int assign_account_type(char *);
	void get_account_uuid(int, uuid_t *);
	handle_t import_interface(rpc_if_handle_t, char *, uuid_t *);
	void secure_handles(handle_t, unsigned32, unsigned32, unsigned32,
			    char *);

	uuid_t acct_uuid;
	rpc_binding_handle_t admin_h, trans_h;
	boolean32 secure = true, exception_raised;
	char *acct_name, *comm, *bank_name, *acct_type;
	char *princ_name, *prot_seq1, *prot_seq2, *port, host[30];
	char *admin_entry_name, *trans_entry_name;
	char message[dce_c_error_string_len];
        char buf[1024];
	unsigned_char_p_t string_binding;
	error_status_t st, trans_st;
	struct cmd_t *p;
	int account_type;
	long amount = -1; 

	acct_name = comm = bank_name = acct_type = NULL;
	princ_name = NULL;
	prog = argv[0];
	make_header(prog);
	process_command_line_arguments(argc, argv, &comm, &bank_name,
				       &acct_type, &acct_name, &amount,
				       &entry_syntax, &admin_entry_name, 
				       &trans_entry_name, &auth_svc, 
				       &auth_level, &authz_svc, &princ_name, 
				       &secure);

	if (acct_type != (char *)NULL)
	{
		account_type = assign_account_type(acct_type);
		get_account_uuid(account_type, &acct_uuid);
	}
	admin_h = import_interface(admin_v1_0_c_ifspec, admin_entry_name, 
			   	   &BankID);
	rpc_binding_to_string_binding(admin_h, &string_binding, &st);
        sprintf(buf, "Cannot convert string binding to handle\n");
        check_status(st, rpc_s_ok, NOT_EQUAL, buf, FALSE, RPC);

        string_binding_to_name(string_binding, host, &port, &prot_seq1);
	sprintf(buf, "%sBound to %s@%s[%s]\n", header, host, prot_seq1, port);
	xx_log(buf);

	trans_h = import_interface(trans_v0_0_c_ifspec, trans_entry_name, 
			   	   NULL);
	rpc_binding_to_string_binding(trans_h, &string_binding, &st);
        sprintf(buf, "Cannot convert string binding to handle\n");
        check_status(st, rpc_s_ok, NOT_EQUAL, buf, FALSE, RPC);

        string_binding_to_name(string_binding, host, &port, &prot_seq2);
	sprintf(buf, "%sBound to %s@%s[%s]\n", header, host, prot_seq2, port);
	xx_log(buf);

	if (secure)
	{
		secure_handles(admin_h, auth_svc, auth_level, authz_svc, 
			       princ_name);
		secure_handles(trans_h, auth_svc, auth_level, authz_svc,
			       princ_name);
	}
	
	do
	{
		TRY
		{
			exception_raised = false;
			st = admin_open_bank(admin_h);
		}
		CATCH(rpc_x_comm_failure)
		{
			exception_raised = true;
			xx_log("Communications failure occured.  Will retry");	
			rpc_binding_reset(admin_h, &st);
			sprintf(buf, "rpc_binding_reset failed");
			check_status(st, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);
		}
		CATCH_ALL
		{
			exception_raised = true;
			xx_severe("The following exception has occured");
			print_exception(THIS_CATCH);
			xx_severe("Cannot open bank");
			write_and_exit(1);
		}
		ENDTRY;
	} while (exception_raised == true);

	if (st == EMPTY_BANK)
	{
		sprintf(buf, "%sBank has no accounts", header);
		xx_log(buf);
	}

	if (strcmp(comm, "create") == 0)
	{
		do
		{
			TRY
			{
				exception_raised = false;
				st = admin_create_acct(admin_h, acct_name);
			}
			CATCH(rpc_x_comm_failure)
			{
				exception_raised = true;
				xx_log("Communications failure occured.");
				xx_log("Will retry");
				rpc_binding_reset(admin_h, &st);
				sprintf(buf, "rpc_binding_reset failed");
				check_status(st, rpc_s_ok, NOT_EQUAL, buf, TRUE,
					     RPC);
			}
			CATCH_ALL
			{
				exception_raised = true;
				xx_severe("The following exception occured");
				print_exception(THIS_CATCH);
				xx_severe("Cannot create account");
				write_and_exit(1);
			}
			ENDTRY;
		} while (exception_raised == true);

		if (check_transaction(st, message))
		{
			sprintf(buf, "%sAccount %s created\n", header, 
				acct_name);
			xx_log(buf);
		}
		else
		{
			sprintf(buf, "%s%s\n", header, message);
			xx_severe(buf);
			write_and_exit(st);
		}

		do
		{
			TRY
			{
				exception_raised = false;
				st = admin_write_bank(admin_h);
			}
			CATCH(rpc_x_comm_failure)
			{
				exception_raised = true;
				xx_log("Communications failure occured.");
				xx_log("Will retry");
				rpc_binding_reset(admin_h, &st);
				sprintf(buf, "rpc_binding_reset failed");
				check_status(st, rpc_s_ok, NOT_EQUAL, buf, TRUE,
					     RPC);
			}
			CATCH_ALL
			{
				exception_raised = true;
				xx_severe("The following exception occured");
				print_exception(THIS_CATCH);
				xx_severe("Cannot close bank.  Accounts lost");
			}
			ENDTRY;
		} while (exception_raised == true);
		write_and_exit(st);
	}

	if (strcmp(comm, "delete") == 0)
	{
		do
		{
			TRY
			{
				exception_raised = false;
				st = admin_delete_acct(admin_h, acct_name);
			}
			CATCH(rpc_x_comm_failure)
			{
				exception_raised = true;
				xx_log("Communications failure occured.");
				xx_log("Will retry");
				rpc_binding_reset(admin_h, &st);
				sprintf(buf, "rpc_binding_reset failed");
				check_status(st, rpc_s_ok, NOT_EQUAL, buf, TRUE,
					     RPC);
			}
			CATCH_ALL
			{
				exception_raised = true;
				xx_severe("The following exception occured");
				print_exception(THIS_CATCH);
				xx_severe("Cannot close bank.  Accounts lost");
			}
			ENDTRY;
		} while (exception_raised == true);

		if (check_transaction(st, message))
		{
			sprintf(buf, "%sAccount %s deleted\n", header, 
				acct_name);
			xx_log(buf);
		}
		else
		{
			sprintf(buf, "%sCannot delete account %s.  %s\n", 
				header, acct_name, message);
			xx_severe(buf);
		}
		do
		{
			TRY
			{
				exception_raised = false;
				st = admin_write_bank(admin_h);
			}
			CATCH(rpc_x_comm_failure)
			{
				exception_raised = true;
				xx_log("Communications failure occured.");
				xx_log("Will retry");
				rpc_binding_reset(admin_h, &st);
				sprintf(buf, "rpc_binding_reset failed");
				check_status(st, rpc_s_ok, NOT_EQUAL, buf, TRUE,
					     RPC);
			}
			CATCH_ALL
			{
				exception_raised = true;
				xx_severe("The following exception occured");
				print_exception(THIS_CATCH);
				xx_severe("Cannot close bank.  Accounts lost");
			}
			ENDTRY;
		} while (st == rpc_s_comm_failure);
		sprintf(buf, "Cannot close bank.  Accounts lost.\n");
		check_status(st, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);
		write_and_exit(st);
	}	

	if (strcmp(comm, "inquire") == 0)
	{
		DoInquire(admin_h, acct_name, amount, &trans_st);
		write_and_exit(trans_st);
	} 

	rpc_binding_set_object(trans_h, &acct_uuid, &st);
	sprintf(buf, "Cannot set handle object\n");
	check_status(st, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);

	for (p = CmdTable; p->proc != NULL; p++)
	{
		if (strcmp(comm, p->name) == 0)
		{
			(*p->proc)(trans_h, acct_name, amount, &trans_st);
			if (trans_st == rpc_s_ok)
			{
				do
				{
					st = admin_write_bank(admin_h);
				} while (st == rpc_s_comm_failure);
				sprintf(buf, 
					"Cannot close bank.  Accounts lost.\n");
				check_status(st, rpc_s_ok, NOT_EQUAL, buf, 
					     FALSE, RPC);
			}
			write_and_exit(trans_st);
		}
	}
	sprintf(buf, "%sInvalid bank command %s\n", header, comm);
	xx_severe(buf);
	show_usage();
	write_and_exit(1);
}

/*
 * Show the usage
 */
static void show_usage()
{
        char buf[1024];
	sprintf(buf, "usage:  %s [OPTIONS]", prog);
        xx_log(buf);
	xx_log("Where OPTIONS are:");
	xx_log(" -T <deposit | withdraw | create | delete | inquire>");
 	xx_log(" -a <checking | savings | cd | ira>");
 	xx_log(" -B <bank name>		Name of bank database file");
 	xx_log(" -n <account name> 	Name of account to transact on");
 	xx_log(" -D <dollar amount>	Amount to deposit/withdraw");
 	xx_log(" -p <principal name>	Name to authenticate with");
	xx_log(" -E <entry syntax>	Namespace entry syntax to use");
	xx_log("	(default:  3.  3 maps to rpc_c_ns_syntax_dce)");
	xx_log(" -S <admin entry>	Admin namespace name");
	xx_log("	entry can be in bind or X.500 style");
	xx_log(" -s <trans syntax>	Trans namespace name");
	xx_log("	entry can be in bind or X.500 style");
 	xx_log(" -A <authentication level>  Authentication level to use");
	xx_log("	(default:  0.  0 maps to default auth level)");
 	xx_log(" -V <authentication service> Authentication service to use");
	xx_log("	(default:  1.  1 maps to private key authentication)");
 	xx_log(" -Z <authorization service>  Authorization service to use");
	xx_log("	(default:  2.  2 maps to DCE PAC authorization)");
 	xx_log(" -N no secure RPC.  Overrides flags -A -V -Z and -p");
}

/*
 * process_command_line_arguments parses the parameters passed by the user
 * and assigns them to variables.
 */
void process_command_line_arguments(int ac, char **av, char **cmd, 
				    char ** bname, char **atype, char **aname, 
				    long *amt, unsigned32 *etry_syntax, 
				    char **admin_entry, char **trans_entry,
				    unsigned32 *auth_service, 
				    unsigned32 *auth_level, 
				    unsigned32 *authz_service, char **p_name, 
				    boolean32 *secure)
{
	extern char *optarg;
	int c;

	while ((c = getopt(ac, av, "T:B:Nn:p:a:D:E:S:s:A:V:Z:hH")) != EOF)
        {
                switch(c)
                {
			case 'T':
				*cmd = optarg;
				break;
			case 'B':
				*bname = optarg;
				break;
			case 'n':
				*aname = optarg;
				break;
			case 'a':
				*atype = optarg;
				break;
			case 'D':
				*amt = atol(optarg);
				break;
			case 'E':
				*etry_syntax = atol(optarg);
				break;
			case 'S':
				*admin_entry = optarg;
				break;
			case 's':
				*trans_entry = optarg;
				break;
			case 'A':
				*auth_level = atol(optarg);
				break;
			case 'V':
				*auth_service = atol(optarg);
				break;
			case 'Z':
				*authz_service = atol(optarg);
				break;
			case 'N':
				*secure = false; 
				break;
			case 'p':
				*p_name = optarg;
				break;
			case 'h':
			case 'H':
				show_usage();
				write_and_exit(0);

		}
	}
}

/*
 * assign_account_type returns the proper int for the type of account the
 * tester has requested to transact upon.
 */
int assign_account_type(char *type)
{
        char buf[1024];
	if (strcmp(type, "checking") == 0)
		return(CK);
	if (strcmp(type, "savings") == 0)
		return(SV);
	if (strcmp(type, "cd") == 0)
		return(CD);
	if (strcmp(type, "ira") == 0)
		return(IRA);
	sprintf(buf, "%sInvalid account type\n", header);
	xx_severe(buf);
	show_usage();
	write_and_exit(1);
}

/*
 * get_account_uuid calls uuid_from_string() to convert the string form
 * of the account object uuid to the binary form.
 */
void get_account_uuid(int type, uuid_t *account_uuid)
{
	uuid_t uuid;
	error_status_t st;
        char buf[1024];
	
	switch(type)
	{
		case CK:
			uuid_from_string(CK_UUID, account_uuid, &st);
			break;
		case SV:
			uuid_from_string(SV_UUID, account_uuid, &st);
			break;
		case CD:
			uuid_from_string(CD_UUID, account_uuid, &st);
			break;
		case IRA:
			uuid_from_string(IRA_UUID, account_uuid, &st);
			break;
	}
	sprintf(buf, "uuid_from_string failed\n");
	check_status(st, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);
}

/*
 * import_interface imports the requested interface from the namespace.
 * The entry name can have bind or X.500 style names.
 */
handle_t import_interface(rpc_if_handle_t iface, char *entry_name, uuid_t *id)
{
	handle_t rh;
	rpc_ns_import_handle_t *context;
	error_status_t st;
        char buf[1024];


	rpc_ns_binding_import_begin(entry_syntax, entry_name, iface,
				    (uuid_p_t)id, (rpc_ns_handle_t *)&context, 
				    &st);
	sprintf(buf, "Cannot import_begin\n");
	check_status(st, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);
	
	rpc_ns_binding_import_next((rpc_ns_handle_t)context, &rh, &st);
	sprintf(buf, "Cannot import_next\n");
	check_status(st, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);

	rpc_ns_binding_import_done((rpc_ns_handle_t *)&context, &st);	
	sprintf(buf, "Cannot import_done\n");
	check_status(st, rpc_s_ok, NOT_EQUAL, buf, FALSE, RPC);

	return(rh);
}

/*
 * secure_handles sets authentication info on the binding handle.  This
 * function is called only if the tester has specified to use authenticated
 * RPC.
 */
void secure_handles(handle_t rh, unsigned32 auth_service, unsigned32 auth_lvl, 
		    unsigned32 authz_service, char *pname)
{
	error_status_t status;
        char buf[1024];


	rpc_binding_set_auth_info((rpc_binding_handle_t)rh, 
				  (unsigned_char_p_t)pname, 
				  auth_lvl, auth_service,
				  (rpc_auth_identity_handle_t)NULL, 
				  authz_service, &status);

	sprintf(buf, "Cannot set auth info\n");
	check_status(status, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);
}

/*
 * check_transaction examines the return code from bank transactions and
 * displays the appropriate message.
 */
int check_transaction(error_status_t status, char *mess)
{
	int st, rc = 1;
	error_status_t e_status;

	switch(status)
	{
		case 0:
			strcpy(mess, "OK\n");
			break;
		case ACCOUNT_EXISTS:
			strcpy(mess,"Account exists\n");
			break;
		case ACCT_NOT_FOUND:
			strcpy(mess,"Account not found\n");
			break;
		case BAD_DEPOSIT_AMOUNT:
			strcpy(mess,"Must deposit multiples of $500.00\n");
			break;
		case UNMATURE_ACCOUNT:
			strcpy(mess,"Account not mature\n");
			break;
		case INSUFFICIENT_FUNDS:
			strcpy(mess,"Insufficient funds\n");
			break;
		case FAIL:
			strcpy(mess,"Transaction attempt failed\n");
			rc = 0;
			break;
		default:
			dce_error_inq_text(status, mess, (int *)&e_status);
			if (e_status == rpc_s_unknown_status_code)
				sprintf(mess, "Unknow status code %ld", 
					status);
			rc = 0;
			break;
	}
	return(rc);
}

/*
 * DoDeposit sets up parameters and calls the remote procedure deposit.
 */
static void DoDeposit(handle_t h, char *acct_name, long amt, 
		      error_status_t *trans_status)
{
	char message[dce_c_error_string_len], *func = "DoDeposit";
	boolean32 exception_raised;
	error_status_t st;
        char buf[1024];

	
	if (amt == -1)
	{
		sprintf(buf, "%sExactly one number must follow deposit\n",
			header);
		xx_severe(buf);
		show_usage();
		write_and_exit(1);
	}
	do
	{	
		TRY
		{
			exception_raised = false;
			*trans_status = deposit(h, acct_name, amt);
		}
		CATCH(rpc_x_comm_failure)
		{
			exception_raised = true;
			sprintf(buf, "(%s):  Communications failure occured.",
				func);
			xx_log(buf);
			xx_log("Will retry");
			rpc_binding_reset(h, &st);
			sprintf(buf, "(%s):  rpc_binding_reset failed", func);
			check_status(st, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);
		}
		CATCH_ALL
		{
			exception_raised = true;
			sprintf(buf, "(%s):  The following exception occured");
			xx_severe(buf);
			print_exception(THIS_CATCH);
			write_and_exit(1);
		}
		ENDTRY;
	} while (exception_raised == true);

	if (check_transaction(*trans_status, message))
	{	
		sprintf(buf, "%s%s\n", header, message);
		xx_log(buf);
		return;
	}
	else
	{
		sprintf(buf, "%s%s\n", header, message);
		xx_error(buf);
		return;
	}
}
	
/*
 * DoWithdraw sets up parameters and calls the remote procedure withdraw.
 */
static void DoWithdraw(handle_t h, char *acct_name, long amt, 
		       error_status_t *trans_status)
{
	char message[dce_c_error_string_len], *func = "DoWithdraw";
	boolean32 exception_raised;
	error_status_t st;
        char buf[1024];


	if (amt == -1)
	{
		sprintf(buf, "%sExactly one number must follow withdraw\n",
			header);
		xx_severe(buf);
		show_usage();
		write_and_exit(1);
	}
	do
	{
		TRY
		{
			exception_raised = false;
			*trans_status = withdraw(h, acct_name, amt);
		}
		CATCH(rpc_x_comm_failure)
		{
			exception_raised = true;
			sprintf(buf, "(%s):  Communications failure occured.",
				func);
			xx_log(buf);
			xx_log("Will retry");
			rpc_binding_reset(h, &st);
			sprintf(buf, "(%s):  rpc_binding_reset failed", func);
			check_status(st, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);
		}
		CATCH_ALL
		{
			exception_raised = true;
			sprintf(buf, "(%s):  The following exception occured");
			xx_severe(buf);
			print_exception(THIS_CATCH);
			write_and_exit(1);
		}
		ENDTRY;
	} while (exception_raised == true);

	if (check_transaction(*trans_status, message))
	{	
		sprintf(buf, "%s%s\n", header, message);
		xx_log(buf);
	}
	else
	{
		sprintf(buf, "%s%s\n", header, message);
		xx_error(buf);
		return;
	}
}

/*
 * DoInquire sets up parameters and calls the remote procedure 
 * admin_inquire_acct.  Parameter amt is ignored.
 */
static void DoInquire(handle_t h, char *acct_name, long amt, 
		      error_status_t *trans_status)
{
	char message[dce_c_error_string_len], *func = "DoInquire";
	boolean32 exception_raised;
	error_status_t st;
	acct_info_t *acct = (acct_info_t *)malloc(sizeof(acct_info_t));
        char buf[1024];


	do
	{
		TRY
		{
			exception_raised = false;
			*trans_status = admin_inquire_acct(h, acct_name, acct);
		}
		CATCH(rpc_x_comm_failure)
		{
			exception_raised = true;
			sprintf(buf, "(%s):  Communications failure occured.",
				func);
			xx_log(buf);
			xx_log("Will retry");
			rpc_binding_reset(h, &st);
			sprintf(buf, "(%s):  rpc_binding_reset failed", func);
			check_status(st, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);
		}
		CATCH_ALL
		{
			exception_raised = true;
			sprintf(buf, "(%s):  The following exception occured");
			xx_severe(buf);
			print_exception(THIS_CATCH);
			write_and_exit(1);
		}
		ENDTRY;
	} while (exception_raised == true);

	if (check_transaction(*trans_status, message))
	{	
		sprintf(buf, "%s%s\n", header, message);
		xx_log(buf);
	}
	else
	{
		sprintf(buf, "%s%s\n", header, message);
		xx_error(buf);
		return;
	}
	
	sprintf(buf, "%sResults of inquire\n", header);
	xx_log(buf);
	printf("Name: %s\n", acct->name);
	printf("Number: %ld\n", acct->number);
	printf("Ck balance: %ld\n", acct->ck_balance);
	printf("Ck trans time: %s\n", ctime(&(acct->ck_trans_time)));
	printf("Sv balance: %ld\n", acct->sv_balance);
	printf("Sv trans time: %s\n", ctime(&(acct->sv_trans_time)));
	printf("Cd balance: %ld\n", acct->cd_balance);
	printf("CD trans time: %s\n", ctime(&(acct->cd_trans_time)));
	printf("IRA balance: %ld\n", acct->ira_balance);
	printf("IRA trans time: %s\n", ctime(&(acct->ira_trans_time)));
	printf("Account create time: %s\n", ctime(&(acct->create_time)));
}
