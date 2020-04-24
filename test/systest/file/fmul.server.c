/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
*/
/*
 * HISTORY
 * $Log: fmul.server.c,v $
 * Revision 1.1.9.3  1996/02/17  23:27:09  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:44:40  marty]
 *
 * Revision 1.1.9.2  1995/12/11  22:00:17  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:59:26  root]
 * 
 * Revision 1.1.5.1  1994/10/24  19:22:08  rdl
 * 	Added threaded code.  Added improved debug mode.  Fixed minor bugs.
 * 	[1994/10/24  19:21:14  rdl]
 * 
 * Revision 1.1.2.2  1994/09/01  19:37:37  rdl
 * 	Changes to fix problems with RPC where multiple servers on one machine
 * 	[1994/09/01  19:36:36  rdl]
 * 
 * Revision 1.1.2.1  1994/08/24  19:45:01  rdl
 * 	Initial submission for dfs.fmul test.
 * 	[1994/08/24  19:44:26  rdl]
 * 
 * $EndLog$
 */

/* fmul.server.c */

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <dce/exc_handling.h>
#include "fmul.h"
#include "fmul_common.h"
#include <sys/param.h>

#define MAX_CONCURRENT_CALLS 5
#define DEFAULT_FAMILY "ncadg_ip_udp"

#define USAGE() fprintf(stderr,"Usage: %s [-f family] [-d]\n",progname);
extern fmul_v1_0_epv_t fmul_v1_0_manager_epv;
extern uuid_t uuid_nil;

int debug=0;
char *progname;

static void *
signal_catcher(void *arg)
{
	static int	catch_sigs[] = {
	SIGHUP, SIGINT, SIGQUIT, SIGTERM
	};
	sigset_t	catch_these;
	int		i;
	error_status_t	st;

	for ( ; ; ) {
	sigemptyset(&catch_these);
	for (i = 0; i < sizeof catch_sigs / sizeof catch_sigs[0]; i++)
		sigaddset(&catch_these, catch_sigs[i]);

	i = sigwait(&catch_these);

	printf("Exiting\n");
	rpc_mgmt_stop_server_listening(NULL, &st);
	}
	return NULL;
}



int main(int argc, char *argv[])
{
	rpc_binding_vector_p_t	bvec;
	error_status_t		st, error_inq_st;
	idl_boolean		validfamily;
	idl_char		*string_binding;
	int			i;
	unsigned char		error_string[132];
	uuid_t			objuid;
	uuid_vector_t		objuidvec;
	char			family[1024]="";
	int			c;
	extern char		*optarg;
	extern int		optind,opterr,optopt;
	pthread_t		tid;
	unsigned_char_t		member_name[1024]="";
	char			hostname[MAXHOSTNAMELEN];
/*
	uuid_t			objtype;
*/

	progname=argv[0];

	optarg = NULL;
	while ((c = getopt(argc, argv, "df:")) != -1)
		switch(c)
		{
			case 'd':
				debug=1;
				break;
			case 'f':
				if (family[0] != '\0')
				{
					/* family defined more than once */
					USAGE();
					exit(1);
				}
				strcpy(family,optarg);
				break;
			default:
					USAGE();
					exit(1);
		}

	if (family[0] == '\0')
		strcpy(family,DEFAULT_FAMILY);

	/* start signal catcher */
	pthread_create(&tid, pthread_attr_default, signal_catcher, (pthread_addr_t)NULL);
	pthread_detach(&tid);


	/* check that arg specifies a protocol sequence that is *
	 * supported on its host both by RPC runtime and OS.    */

	validfamily = rpc_network_is_protseq_valid((idl_char *)family, &st);
	if (st != error_status_ok)
	{
		dce_error_inq_text(st, error_string, &error_inq_st);
		fprintf(stderr, "Cannot check protocol sequence - %s\n", error_string);
		exit(1);
	}

	if (!validfamily)
	{
		fprintf(stderr, "Protocol sequence %s is not valid\n", family);
		exit(1);
	}

	/* find endpoint to listen on */

	if (debug)
		puts("Finding endpoint to listen on");
	rpc_server_use_protseq((idl_char *)family, MAX_CONCURRENT_CALLS, &st);
	if (st != error_status_ok)
	{
		dce_error_inq_text(st, error_string, &error_inq_st);
		fprintf(stderr, "Cannot use protocol sequence - %s\n", error_string);
		exit(1);
	}

	/* register interface */
	if (debug)
			puts("registering interface");
	rpc_server_register_if(fmul_v1_0_s_ifspec, &uuid_nil, NULL, &st);
	if (st != error_status_ok)
	{
		dce_error_inq_text(st, error_string, &error_inq_st);
		fprintf(stderr, "Cannot Register interface - %s\n", error_string);
		exit(1);
	}

	/* obtain vector of binding handles */
	if (debug)
		puts("obtaining a vector of binding handles");
	rpc_server_inq_bindings(&bvec, &st);
	if (st != error_status_ok)
	{
		dce_error_inq_text(st, error_string, &error_inq_st);
		fprintf(stderr, "Cannot inquire bindings - %s\n", error_string);
		exit(1);
	}

	/* obtain, print, free string binding */
	printf("Bindings:\n");
	for (i=0; i < bvec->count ; i++)
	{
		rpc_binding_to_string_binding(bvec->binding_h[i], &string_binding, &st);
		printf("%s\n", (char *)string_binding);
		rpc_string_free(&string_binding, &st);
	}

	uuid_create(&objuid,&st);
	objuidvec.count=1;
	objuidvec.uuid[0]=&objuid;
/*
	uuid_create(&objtype,&st);
	rpc_object_set_type(&objuid,&objtype,&st);
	if (st != error_status_ok)
	{
		dce_error_inq_text(st, error_string, &error_inq_st);
		fprintf(stderr, "Cannot set object type - %s\n", error_string);
		exit(1);
	}
*/

	/* register server endpoint */
	if (debug)
		puts("registering server endpoint");
	rpc_ep_register(fmul_v1_0_s_ifspec, bvec, &objuidvec, (unsigned_char_p_t) "fmul version 1.0 server", &st);
	if (st != error_status_ok)
	{
		dce_error_inq_text(st, error_string, &error_inq_st);
		fprintf(stderr, "Cannot register endpoint - %s\n", error_string);
		exit(1);
	}


	/* create member_name - use hostname and pid */
	gethostname(hostname, MAXHOSTNAMELEN);
	sprintf((char *)member_name,"%s%s%d",NAMESERVER_MEMBER_NAME_BEGIN, hostname , (int) getpid());

	/* Export binding info to CDS */
	if (debug)
		puts("exporting binding to CDS");
	rpc_ns_binding_export(rpc_c_ns_syntax_default, member_name ,fmul_v1_0_s_ifspec,bvec, &objuidvec ,&st);
	if (st != error_status_ok)
	{
		dce_error_inq_text(st, error_string, &error_inq_st);
		fprintf(stderr, "Cannot export binding info to CDS - %s\n", error_string);
		exit(1);
	}

	/* Add to group */
	rpc_ns_group_mbr_add( rpc_c_ns_syntax_default ,  (unsigned_char_t *)NAMESERVER_GROUP_NAME , rpc_c_ns_syntax_default , member_name , &st);
	if (st != error_status_ok)
	{
		dce_error_inq_text(st, error_string, &error_inq_st);
		fprintf(stderr, "Cannot add to group - %s\n", error_string);
		exit(1);
	}

	/* begin listening for RPC requests */

	TRY
	{
		printf("Listening...\n");
		rpc_server_listen(MAX_CONCURRENT_CALLS, &st);
		if (st != error_status_ok)
		{
			dce_error_inq_text(st, error_string, &error_inq_st);
			fprintf(stderr, "Error: %s\n", error_string);
		}
	}
	FINALLY
	{
		if (debug)
			puts("Remove from group");
		rpc_ns_group_mbr_remove( rpc_c_ns_syntax_default ,  (unsigned_char_t *)NAMESERVER_GROUP_NAME , rpc_c_ns_syntax_default , member_name , &st);
		if (st != error_status_ok)
		{
			dce_error_inq_text(st, error_string, &error_inq_st);
			fprintf(stderr, "Remove from group: %s\n", error_string);
		}

		if (debug)
			puts("Unexporting name binding");
		rpc_ns_binding_unexport(rpc_c_ns_syntax_default, member_name , fmul_v1_0_s_ifspec, &objuidvec, &st);
		if (st != error_status_ok)
		{
			dce_error_inq_text(st, error_string, &error_inq_st);
			fprintf(stderr, "Unexporting name binding: %s\n", error_string);
		}

		if (debug)
			puts("Unregistering endpoint");
		rpc_ep_unregister(fmul_v1_0_s_ifspec, bvec, &objuidvec, &st);
		if (st != error_status_ok)
		{
			dce_error_inq_text(st, error_string, &error_inq_st);
			fprintf(stderr, "Unregistering endpoint: %s\n", error_string);
		}
	}
	ENDTRY;
}
