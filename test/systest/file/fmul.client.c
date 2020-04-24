/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
*/
/*
 * HISTORY
 * $Log: fmul.client.c,v $
 * Revision 1.1.9.3  1996/02/17  23:27:05  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:44:38  marty]
 *
 * Revision 1.1.9.2  1995/12/11  22:00:13  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:59:22  root]
 * 
 * Revision 1.1.5.1  1994/10/24  19:22:03  rdl
 * 	Added threaded code.  Added improved debug mode.  Fixed minor bugs.
 * 	[1994/10/24  19:20:48  rdl]
 * 
 * Revision 1.1.2.2  1994/09/01  19:37:35  rdl
 * 	Changes to fix problems with RPC where multiple servers on one machine
 * 	[1994/09/01  19:36:04  rdl]
 * 
 * Revision 1.1.2.1  1994/08/24  19:45:00  rdl
 * 	Initial submission for dfs.fmul test.
 * 	[1994/08/24  19:41:51  rdl]
 * 
 * $EndLog$
 */

/* fmul.client.c */

#include <dce/cma_stdio.h>
#include <dce/cma_ux.h>
#include "fmul.h"
#include "fmul_common.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>

#define USAGE() fprintf(stderr,"Usage: %s -f config_file -s number_of_servers -n filename -p path [-d]\n",progname)

#define UNLOCKED 0
#define LOCKED 1
#define BLOCKED 2

typedef struct {
rpc_binding_handle_t	*bh;
int			*state;
filename_t		filename;
idl_long_int		size;
idl_long_int		start;
idl_char		chartowrite;
} wfuarg_t;

char *progname;
int server_state[MAX_SERVERS];
wfuarg_t wfuarg[MAX_SERVERS];
pthread_t tid[MAX_SERVERS];

int passes=0;
int fails=0;
int terse=0;

int debug=0;




/* find_servers()
*/

find_servers( rpc_binding_handle_t bh[], int servers_needed )
{
	error_status_t		st, error_inq_st;
	idl_char		*string_binding;
	rpc_ns_handle_t		imp_contxt;
	rpc_ns_handle_t		grp_contxt;
	unsigned char		error_string[132];
	unsigned_char_t		*member_name;
	int			numfound=0;


	rpc_ns_group_mbr_inq_begin(rpc_c_ns_syntax_default, (unsigned_char_t *)NAMESERVER_GROUP_NAME, rpc_c_ns_syntax_default, &grp_contxt, &st);
	if (st != error_status_ok)
	{
		dce_error_inq_text(st, error_string, &error_inq_st);
		fprintf(stderr, "Cannot rpc_ns_group_mbr_inq_begin - %s\n", error_string);
	exit(1);
	}

	while (numfound < servers_needed)
	{
		rpc_ns_group_mbr_inq_next( grp_contxt , &member_name , &st );
		if ((st != error_status_ok) && (st != rpc_s_no_more_members))
		{
			dce_error_inq_text(st, error_string, &error_inq_st);
			fprintf(stderr, "Cannot rpc_ns_group_member_next - %s\n", error_string);
			exit(1);
		}
		if (st == rpc_s_no_more_members)
		{
			if (debug)
				puts("st == rpc_s_no_more_members");
			break;
		}

		rpc_ns_binding_import_begin(rpc_c_ns_syntax_default, member_name ,fmul_v1_0_c_ifspec,NULL,&imp_contxt,&st);
		if (st != error_status_ok)
		{
			dce_error_inq_text(st, error_string, &error_inq_st);
			fprintf(stderr, "Cannot rpc_ns_binding_import_begin - %s\n", error_string);
			exit(1);
		}

		while (numfound < servers_needed)
		{

			if (debug)
				printf("numfound=%d\n",numfound);

			rpc_ns_binding_import_next(imp_contxt,&bh[numfound],&st);
			if ((st != error_status_ok) && (st != rpc_s_no_more_bindings))
			{
				dce_error_inq_text(st, error_string, &error_inq_st);
				fprintf(stderr, "Cannot rpc_ns_binding_import_next - %s\n", error_string);
				exit(1);
			}
			if (st == rpc_s_no_more_bindings)
			{
				if (debug)
					puts("st == rpc_s_no_more_bindings");
				break;
			}

			if (debug)
			{
				rpc_binding_to_string_binding(bh[numfound],&string_binding,&st);
				printf("binding: %s\n",string_binding);
				rpc_string_free(&string_binding, &st);
			}

			/* convert partially bound handle to a fully bound handle
		 	* for the rpc_mgmt_is_server_listening call*/
			rpc_ep_resolve_binding(bh[numfound],fmul_v1_0_c_ifspec,&st);
			if (st != error_status_ok)
			{
				if (debug)
				{
					dce_error_inq_text(st, error_string, &error_inq_st);
					fprintf(stderr, "Cannot convert partially bound handle to a fully bound handle - %s\n", error_string);
					puts("Continuing");
				}
				continue;
			}

			if (debug)
			{
				rpc_binding_to_string_binding(bh[numfound],&string_binding,&st);
				printf("binding: %s\n",string_binding);
				rpc_string_free(&string_binding, &st);
			}

			/* is this a valid server? */
			if (rpc_mgmt_is_server_listening(bh[numfound],&st))
			{
				if (debug)
					printf("found server\n");
				numfound++;
			}
		}

		rpc_ns_binding_import_done(&imp_contxt,&st);
		if (st != error_status_ok)
		{
			dce_error_inq_text(st, error_string, &error_inq_st);
			fprintf(stderr, "Cannot rpc_ns_binding_import_done - %s\n", error_string);
			exit(1);
		}
	}

	rpc_ns_group_mbr_inq_done(&grp_contxt,&st);
	if (st != error_status_ok)
	{
		dce_error_inq_text(st, error_string, &error_inq_st);
		fprintf(stderr, "Cannot rpc_ns_group_mbr_inq_done - %s\n", error_string);
		exit(1);
	}

	return(numfound);
}




/* split_line()
*/
int split_line(char line[], char lineparts[][MAX_LINE_LENGTH])
{
	int	part=0;
	int	i=0;
	int	j=0;

	while (line[i] != '\0')
	{
		/* copy */
		while (! isspace(line[i]))
			lineparts[part][j++]=line[i++];
		lineparts[part][j]='\0';

		part++;
		j=0;

		/* run through blank space */
		while (isspace(line[i]))
			i++;
	}
	return(part);
}





/* report_pass()
*/
report_pass(int server,char *message)
{
	passes++;
	if (!terse)
	{
		/* client test */
		if (server==-1)
			printf("PASS - client: %s\n",message);
		else
			printf("PASS - server %d: %s\n",server,message);
	}
}





/* report_fail()
*/
report_fail(int server,char *message)
{
	fails++;
	if (!terse)
	{
		/* client test */
		if (server==-1)
			printf("FAIL - client: %s\n",message);
		else
			printf("FAIL - server %d: %s\n",server,message);
	}
}



/* process_client()
*/
process_client(char *line, rpc_binding_handle_t bh[], int numservers, char *file )
{
	char		lineparts[6][MAX_LINE_LENGTH];
	idl_long_int	rc_return, errno_return;
	char		*function;
	int		i, fd;
	filename_t	filename;

	split_line(line, lineparts);

	function=lineparts[1];

	if (debug)
		printf("processing a client command - %s\n", function);

	strcpy(filename, file);
	/* CLEAR_CHILDREN */
	if (strcmp(function,"CLEAR_CHILDREN") == 0)
	{
		if (debug)
			puts("CLEAR_CHILDREN");
		/* clear all locks on all servers */
		for (i=0 ; i < numservers ; i++)
			l_ulock( bh[i], filename , 0, 0, &rc_return, &errno_return);
	}

	/* CLEAR_LOCKFILE */
	if (strcmp(function,"CLEAR_LOCKFILE") == 0)
	{
		if (debug)
			puts("CLEAR_LOCKFILE");
		/* open file, clearing, and setting permissions */
		if ((fd=open( file, O_CREAT | O_WRONLY | O_TRUNC , S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH | S_ISGID)) == 0 )
			fprintf(stderr,"%s: cannot open %s to clear\n",progname, file);
		close(fd);
	}

	/* L_LOCK_GRANTED */
	if (strcmp(function,"L_LOCK_GRANTED") == 0)
	{
		if (debug)
			puts("L_LOCK_GRANTED");
		if (server_state[atoi(lineparts[2])-1]!=LOCKED)
			report_fail(-1,"L_LOCK_GRANTED failed.");
		else
			report_pass(-1,"L_LOCK_GRANTED passed.");
	}
}

static void *wait_for_unblock( void *arg)
{
	wfuarg_t		*wfuarg;
	idl_long_int		rc_return;
	idl_long_int		errno_return;
	idl_long_int		rc;


	wfuarg=(wfuarg_t *)arg;
	/* wait for unblock thread */
	if (debug)
	{
		puts("wait_for_unblock thread started");
		fflush(stdout);
	}

	rc=l_lock( *(wfuarg->bh), wfuarg->filename, wfuarg->start, wfuarg->size, wfuarg->chartowrite, &rc_return, &errno_return );
	if (debug)
		printf("wait_for_unblock thread ending, l_lock finished, rc=%d\n",rc);
	*(wfuarg->state)=LOCKED;
}










/* process_server()
*/
process_server(char *line, rpc_binding_handle_t bh[], int numservers, char *file )
{
	char		lineparts[6][MAX_LINE_LENGTH];
	int		server;
	idl_long_int	rc, rc_return, errno_return;
	char		*function;
	filename_t	filename;

	strcpy(filename, file);

	split_line(line, lineparts);

	server=atoi(lineparts[0]);
	function=lineparts[1];

	if (debug)
		printf("processing a server command - %s on server %d\n", function, server);

	/* L_LOCK */
/* 
	- Needs to use a thread to bypass possible locking conditions.
	- The state is set to BLOCKED.
	- The thread is started, child thread makes the call, and possibly
	- blocks.  When the lock is granted, the child writes to a shared
	- variable and exits.
	- The parent process waits one second, then looks at the variable
	- to see if the child was successful, if so, the state is set
	- to LOCKED.  If the child has not completed, execution is then
	- continued.
*/
	if (strcmp(function,"L_LOCK") == 0)
	{

		/* Detour around timing problem (sometimes the RPC call itself
	   	blocks and the main thread can't tell the differece */
		rc=l_tlock( bh[server-1], filename, atoi(lineparts[2]), atoi(lineparts[3]), lineparts[4][0], &rc_return, &errno_return);
		if (rc_return==0)
		{
			/* well, since it is already locked */
			server_state[server-1]=LOCKED;
				if (lineparts[5][0]=='p')
				report_pass(server,"L_LOCK request granted.");
			else
				report_fail(server,"L_LOCK request locked, but shouldn't.");
		}
		else
		{

			/* the locking call will block */

			server_state[server-1]=BLOCKED;

			wfuarg[server-1].state= &server_state[server-1];
			wfuarg[server-1].bh= &bh[server-1];
			strcpy(wfuarg[server-1].filename,filename);
			wfuarg[server-1].start=atoi(lineparts[2]);
			wfuarg[server-1].size=atoi(lineparts[3]);
			wfuarg[server-1].chartowrite=lineparts[4][0];

			if (pthread_create(&tid[server-1], pthread_attr_default , wait_for_unblock, (pthread_addr_t) &wfuarg[server-1])!=0)
			{
				fprintf(stderr,"Cannot create thread\n");
				exit(1);
			}
			if (debug)
				printf("created thread\n");

			/* raise child thread's priority */
			if (debug)
				puts("raising priority of child thread");
			pthread_setprio(tid[server-1], PRI_OTHER_MAX);

			/* let child thread execute */
			if (debug)
				puts("yielding to child thread");
			pthread_yield();

			if (pthread_detach(&tid[server-1])!=0)
			{
				fprintf(stderr,"Cannot detach thread\n");
				exit(1);
			}
			if (debug)
				printf("detached thread\n");


			if (server_state[server-1]==BLOCKED)
			{
				if (lineparts[5][0]=='b')
					report_pass(server,"L_LOCK request successfully blocked.");
				else
					report_fail(server,"L_LOCK request blocked when expecting lock.");
			}
			else
			{
				if (lineparts[5][0]=='p')
					report_pass(server,"L_LOCK request successful.");
				else if (lineparts[5][0]=='b')
					report_fail(server,"L_LOCK request failed to block.");
				else
					report_fail(server,"L_LOCK request failed.");
			}
		}
	}

	/* WRITE_RANGE */
	if (strcmp(function,"WRITE_RANGE") == 0)
	{
		rc=writeRange( bh[server-1], filename, atoi(lineparts[2]), atoi(lineparts[3]), lineparts[4][0], &rc_return, &errno_return);

		if (rc_return==0)
		{
			if (lineparts[5][0]=='p')
				report_pass(server,"WRITE_RANGE successful.");
			else
				report_fail(server,"WRITE_RANGE didn't fail as expected.");
		}
		else
		{
			if (lineparts[5][0]=='f')
				report_pass(server,"WRITE_RANGE failed as expected.");
			else
				report_fail(server,"WRITE_RANGE failed.");
		}
	}

	/* L_TLOCK */
	if (strcmp(function,"L_TLOCK") == 0)
	{
		rc=l_tlock( bh[server-1], filename, atoi(lineparts[2]), atoi(lineparts[3]), lineparts[4][0], &rc_return, &errno_return);

		if (rc_return==0)
		{
			server_state[server-1]=LOCKED;
			if (lineparts[5][0]=='p')
				report_pass(server,"L_TLOCK request granted.");
			else
				report_fail(server,"L_TLOCK request didn't fail as expected.");
		}
		else
		{
			if (lineparts[5][0]=='f')
				report_pass(server,"L_TLOCK request not granted when already locked.");
			else
				report_fail(server,"L_TLOCK request failed.");
		}
	}

	/* L_TEST */
	if (strcmp(function,"L_TEST") == 0)
	{
		rc=l_test( bh[server-1], filename, atoi(lineparts[2]), atoi(lineparts[3]), &rc_return, &errno_return);

		if (rc == LOCK_DENIED)
		{
			server_state[server-1]=UNLOCKED;
			if (lineparts[4][0]=='f')
				report_pass(server,"L_TEST: file not locked.");
			else
				report_fail(server,"L_TEST: file not locked, but should be.");
		}
		else
		{
			server_state[server-1]=LOCKED;
			if (lineparts[4][0]=='p')
				report_pass(server,"L_TEST: file locked.");
			else
				report_fail(server,"L_TEST: file locked but shouldn't be.");
		}
	}

	/* L_ULOCK */
	if (strcmp(function,"L_ULOCK") == 0)
	{
		rc=l_ulock( bh[server-1], filename, atoi(lineparts[2]), atoi(lineparts[3]), &rc_return, &errno_return);

		if (rc_return==0)
		{
			server_state[server-1]=UNLOCKED;
			report_pass(server,"L_ULOCK successful");
		}
		else
			report_fail(server,"L_ULOCK failed.");
	}
}





/* process()
*/
int process(char *c_file , rpc_binding_handle_t bh[] , int numservers , char *file )
{
	FILE	*fp;
	char 	line[MAX_LINE_LENGTH];
	int	i=0;
	char	ch;

	if ((fp=fopen(c_file,"r")) == NULL )
	{
		fprintf(stderr,"%s: Cannot open confinguration file %s for reading\n",progname,c_file);
		exit(1);
	}

	while (fgets(line,MAX_LINE_LENGTH,fp) != NULL )
	{
		i++;
		if (debug)
			printf("line %d\n",i);
		/* skip lines not beginning with 'p' or a digit */
		if ( (line[0] != 'p') && ! (isdigit((int)line[0])) )
			continue; 

		if (line[0] == 'p')
			process_client(line, bh, numservers, file);
		else
			process_server(line, bh, numservers, file);
	}
}






/* main()
*/
int main(int argc, char *argv[])
{
	rpc_binding_handle_t	bh[MAX_SERVERS];
	char			configfile[MAX_LINE_LENGTH], filename[MAX_LINE_LENGTH], path[MAX_LINE_LENGTH];
	int			numservers, servers_found,i,c;
	char			file[MAX_LINE_LENGTH];
	extern char		*optarg;
	extern int		optind,opterr,optopt;
	int			ffound=0,sfound=0,nfound=0,pfound=0;

	/* parse command line */
	progname=argv[0];

	optarg = NULL;
	while ((c = getopt(argc, argv, "f:s:n:p:d")) != -1)
		switch(c)
		{
			case 'd':
				debug=1;
				break;
			case 'f':
				ffound=1;
				strcpy(configfile,optarg);
				break;
			case 's':
				sfound=1;
				numservers=atoi(optarg);
				if (( numservers < 1 ) || ( numservers > MAX_SERVERS ))
				{
					USAGE();
					exit(1);
				}
				break;
			case 'n':
				nfound=1;
				strcpy(filename,optarg);
				break;
			case 'p':
				pfound=1;
				strcpy(path,optarg);
				break;
			default:
				USAGE();
				exit(1);
		}

	if (!(ffound && sfound && nfound && pfound))
	{
		USAGE();
		exit(1);
	}
				

	strcpy(file,path);
	strcat(file,"/");
	strcat(file,filename);

	/* clear blocked lock requests list */
	for (i=0 ; i < numservers ; i++)
		server_state[i]=UNLOCKED;

	/* initialize RPC */
	if (debug)
		puts("looking for servers");
	if ((servers_found=find_servers( bh , numservers )) < numservers)
	{
		fprintf(stderr,"%s: Need %d servers, found %d\n", progname, numservers, servers_found);
		exit(1);
	}

	/* lower priority of main thread */
	if (debug)
		puts("lowering priority of main thread");
	pthread_setprio(pthread_self() , PRI_OTHER_MIN );

	if (debug)
		puts("process config file");
	process(configfile, bh , numservers, file );
}
