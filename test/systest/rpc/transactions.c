/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: transactions.c,v $
 * Revision 1.1.14.2  1996/02/17  23:28:18  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:45:22  marty]
 *
 * Revision 1.1.14.1  1995/12/11  22:44:27  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1994/07/01  16:37 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 
 * 	HP revision /main/HPDCE01/1  1994/03/29  15:50 UTC  truitt
 * 
 * 	HP revision /main/truitt_junk/1  1994/03/29  15:45 UTC  truitt
 * 	Update test to latest tet version
 * 	[1995/12/11  22:21:03  root]
 * 
 * Revision 1.1.8.2  1993/02/05  15:23:46  cjd
 * 	Embedded copyright notice
 * 	[1993/02/05  14:45:14  cjd]
 * 
 * Revision 1.1.6.2  1993/02/04  22:14:53  cjd
 * 	Embedded copyright notice
 * 
 * Revision 1.1.4.2  1992/05/21  18:33:38  gmd
 * 	Checking systest_sbox (shared sandbox) version.
 * 	[1992/05/21  16:48:49  gmd]
 * 
 * Revision 1.1.2.2  1992/05/15  21:42:41  gmd
 * 	Replaced incorrect test of status returned by wait() with portable read
 * 	of transaction_exit_status file written by bank.
 * 	[1992/05/15  21:03:29  gmd]
 * 
 * Revision 1.1  1992/01/19  14:37:34  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *
 *
 *      FILE_NAME:  transactions.c
 *
 *      COMPONENT_NAME:  dce.systest
 *
 *      FUNCTIONS:  none
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
 *              transactions <full path to su.dce or bank> <bank program name> 
 *			     <bank name> <runtime> <entry syntax> <admin entry>
 *			     <trans entry> <transaction logfile> <principal>
 *			     <authentication service> <authentication level>
 *			     <authorization service> 
 *
 *              where:
 *	    	  full path is the full pathname to su.dce or the bank 
 *		  	executable.
 *                bank program name is name the name of the bank executable.
 *		  bank name is the name of the bank datafile.
 *		  runtime is the number of seconds to run.
 *		  entry syntax is the syntax to use when [ex|im]porting to/from
 *		  	the namespace.
 *		  admin entry is the name of the server entry for the admin    
 *		  	interface.
 *		  trans entry is the name of the server entry for the trans    
 *		  	interface.
 *		  transaction logfile is name of file to write transaction
 *		  	results to.
 *		  principal is the security principal to use for registering
 *			and setting authentication info.
 *		  authentication service is the service to use when authenti-
 *			cating RPC requests.
 *		  authentication level is the level of authentication to use
 *			on RPC requests.
 *		  authorization service is the service to use for authorizing
 *			RPC requests.
 *
 *		Note:  principal, authentication service, authentication level
 *		       and authorization service are optional parameters.
 *
 *                (e.g. transactions /u/stlib/systest/tools/su.dce 
 *		        /tmp/dcerpbnk.9.skull.0/bank baybanks baybanks 300 3 
 *			/.:/admin /.:/trans /u/status/trans.log banker15 
 *			1 0 2)
 *
 *	Description:
 *		This program uses random number generators to create 
 *		bank transactions.  Four arrays contain the 
 *		information need to build a bank transactions:
 *			
 *		   name		    array of account names
 *		   transactions	    types of transactions allowed
 *		   acct_type	    type of accounts allowed
 *		   amounts	    the different dollar amounts
 *		
 *		By generating random numbers in the range of the above
 *		arrays' upper bound, we can build a bank tranaction.
 *		After the transaction is built, a fork/exec is done to
 *		call the bank with the just built transaction.  The 
 *		parent waits for the child to complete.  If the 
 *		transaction completes without error, the array of 
 *		running totals is updated according to the transaction.  
 *		When time's up, the running totals array is printed out 
 *		so as to check if the bank database contains correct 
 *		totals.
 *
 *	Change History
 *		05/03/91	Code Complete		Gerald Cantor
 *		10/18/91	Added test to handle
 *				secure and non-secure
 *				flavors.		Gerald Cantor
 *		12/14/91	Update usage statement  Gerald Cantor
 */
#include <stdio.h>
#include <sys/wait.h>
#include <xx_lib.h>


static char *transactions[2] = { "deposit", "withdraw" }; 
static char *name[10] = { "Mary", "Jim", "Jake", "Baab", "Lois", 
			  "John", "Pete", "Tammy", "Hannah", "Mercedes" 
			};
static char *acct_type[4] = { "checking", "savings", "cd", "ira" };
static char *amounts[30] = { "10", "20", "25", "40", "50", "60", "70", 
			     "75", "80", "95", "100", "150", "200", 
			     "250", "300", "350", "400", "450", "500", 
			     "550", "600", "700", "800", "900", "1000", 
			     "1500", "2000", "3000", "4000", "5000"
			   };
struct trans_amount {
	char *name;	/* account name */
	long ck_trans; 	/* total checking transactions */
	long sv_trans; 	/* total savings transactions */
	long cd_trans; 	/* total cd transactions */
	long ira_trans;	/* total ira transactions */
} T_amt[10] = {
			{ "Mary", 0, 0, 0 },
			{ "Jim", 0, 0, 0 },
			{ "Jake", 0, 0, 0 },
			{ "Baab", 0, 0, 0 },
			{ "Lois", 0, 0, 0 },
			{ "John", 0, 0, 0 },
			{ "Pete", 0, 0, 0 },
			{ "Tammy", 0, 0, 0 },
			{ "Hannah", 0, 0, 0 },
			{ "Mercedes", 0, 0, 0 }
		     };

main(argc, argv)
int argc;
char **argv;
{
	FILE *fp, *sfp;
	long amount, finish_time;
	int i, status, exit_status, pid;
	int name_index, amount_index, acct_index, trans_index;
	char *Name, *Amount, *Acct, *Transaction, *bank_name;
	char *bank_program, *princ_name, *full_path, *log_file;
	char *auth_svc, *auth_lvl, *authz_svc, *entry_syntax;
	char *admin_entry, *trans_entry;
	char buffer[512];

	full_path = argv[1];
	bank_program = argv[2];
	bank_name = argv[3];
	entry_syntax = argv[5];
	admin_entry = argv[6];
	trans_entry = argv[7];
	log_file = argv[8];
	princ_name = argv[9];
	auth_svc = argv[10];
	auth_lvl = argv[11];
	authz_svc = argv[12];

	fp = fopen(log_file, "w"); 
	if (fp == NULL )
	{
		xx_severe("Cannot open log file for writing");
		exit(1);
	}


	finish_time = time((long *)0) + atoi(argv[4]);
	while (time((long *)0) < finish_time)
	{
		srand((int) time((long *)0));
		name_index = rand() % 10;
		Name = name[name_index];
		sleep(1);

		srand((int) time((long *)0));
		amount_index = rand() % 30;
		Amount = amounts[amount_index];
		amount = atol(Amount);
		sleep(1);

		srand((int) time((long *)0));
		acct_index = rand() % 4;
		Acct = acct_type[acct_index];
		sleep(1);

		srand((int) time((long *)0));
		trans_index = rand() % 2;
		Transaction = transactions[trans_index];
		sleep(1);

		printf("Executing . . .\n");
		if (princ_name == NULL)
			sprintf(buffer, "-T %s -n %s -a %s -B %s -D %s -E %s -S %s -s %s -N",
				Transaction, Name, Acct, bank_name, Amount, 
				entry_syntax, admin_entry, trans_entry);
		else
			sprintf(buffer, "-T %s -n %s -a %s -B %s -D %s -E %s -V %s -A %s -Z %s -p %s -S %s -s %s", 
				Transaction, Name, Acct, bank_name, Amount, 
				entry_syntax, auth_svc, auth_lvl, authz_svc, 
				princ_name, admin_entry, trans_entry);
		printf("\tbank %s\n", buffer);

		if (princ_name == NULL)
		{
		        pid = fork();
			if (pid == 0)
				execl(full_path, "bank", "-T", Transaction, 
			      	      "-B", bank_name, "-n", Name, "-a", Acct, 
				      "-D", Amount, "-N", "-E", entry_syntax, 
				      "-S", admin_entry, "-s", trans_entry, 
				      (char *)0);

			else /* parent, wait for transaction to finish */
				wait(&status);
		}
		else
		{
		        pid = fork();
		        if (pid == 0) /* child process, call bank */
				execl(full_path, "bank", "-T", Transaction,
					"-n", Name, "-a", Acct, "-B", bank_name,
					"-D", Amount, "-E", entry_syntax, "-V", auth_svc,
					"-A", auth_lvl, "-Z", authz_svc, "-p", princ_name,
					"-S", admin_entry, "-s", trans_entry,
                                      (char *)0);
			else /* parent, wait for transaction to finish */
				wait(&status);
		}	

		sprintf(buffer,"transaction_exit_status.%d",pid);
		sfp = fopen(buffer,"r");
		if (sfp == NULL )
		{
			xx_severe("Cannot open status file for reading");
			exit(1);
		}
		else 
		{
		   fscanf(sfp,"%d",&exit_status);
		   fclose(sfp);
		   status = unlink(buffer);
                   if (status == -1)
                      perror("transactions:unlink error:");
    
                   printf("%s %s %s $%s  exited with %d\n",Name,Acct,Transaction,Amount,exit_status);
		   if (exit_status == 0)	/* transaction successful */
		   {
			if (strcmp(Transaction, "withdraw") == 0)
				amount = -amount;

			switch(acct_index)
			{
				case 0:
					T_amt[name_index].ck_trans += 
					amount;
					break;
				case 1:
					T_amt[name_index].sv_trans += 
					amount;
					break;
				case 2:
					T_amt[name_index].cd_trans += 
					amount;
					break;
				case 3:
					T_amt[name_index].ira_trans += 
					amount;
					break;
			} /* end switch */
		   } /* end if exit_status == 0 */
		} /* end if sfp != NULL */
        	   unlink(buffer);
	} /* end while */
	for (i = 0; i < 10; i++)
	{
		sprintf(buffer, "%s:%ld:%ld:%ld:%ld\n", T_amt[i].name,
			T_amt[i].ck_trans, T_amt[i].sv_trans,
			T_amt[i].cd_trans, T_amt[i].ira_trans);
		fprintf(fp, "%s", buffer);
	}
	close(fp);
}
