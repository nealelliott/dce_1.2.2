/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: timop_client.c,v $
 * Revision 1.2.5.2  1996/02/17  23:24:07  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:42:29  marty]
 *
 * Revision 1.2.5.1  1995/12/11  20:15:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:48:46  root]
 * 
 * Revision 1.2.3.3  1993/10/21  19:44:46  weir
 * 	timop for DCE 1.0.3, with following fixes:
 * 	CR 4804: use DCE status codes
 * 	CR 4805: use relative principal names
 * 	CR 7460: initialize remote status parameter in server manager
 * 	CR 9215: port to HP platform.
 * 	[1993/10/21  19:38:25  weir]
 * 
 * Revision 1.2.2.3  1993/02/04  20:32:26  cjd
 * 	Embedded copyright notice
 * 	[1993/02/04  20:29:44  cjd]
 * 
 * Revision 1.2.2.2  1992/12/18  18:18:23  marcyw
 * 	Defect 4998: adding disclaimer comment
 * 	[1992/12/18  18:12:12  marcyw]
 * 
 * Revision 1.2  1992/01/19  22:14:29  devrcs
 * 	Dropping DCE1.0 OSF1_misc port archive
 * 
 * $EndLog$
 */
/*
 * The Example code is provided to you as "Pass-through Code".
 * Pass-through Code and its related Documentation are not warrented,
 * supported, or indemnified by OSF, and are provided "AS IS".
 * You can use, sublicense, or disribute the same at your own risk.
 */
/*
 * Copyright (c) 1990, 1991  
 * Open Software Foundation, Inc. 
 *  
 * Permission is hereby granted to use, copy, modify and freely distribute 
 * the software in this file and its documentation for any purpose without 
 * fee, provided that the above copyright notice appears in all copies and 
 * that both the copyright notice and this permission notice appear in 
 * supporting documentation.  Further, provided that the name of Open 
 * Software Foundation, Inc. ("OSF") not be used in advertising or 
 * publicity pertaining to distribution of the software without prior 
 * written permission from OSF.  OSF makes no representations about the 
 * suitability of this software for any purpose.  It is provided "as is" 
 * without express or implied warranty. 
 */
/*
 *  OSF DCE Version 1.0 
 */

/*
**	timop_client.c
**
**	Client program for timop interface.
*/
#include <pthread.h>
#include <time.h>
#include <dce/rpc.h>
#include <dce/dce_error.h>
#include <dce/utc.h>
#include "timop.h"
#include "timop_aux.h"
#include "timop_client.h"


long			Rand;		/* sum of random numbers */
int			Workers;	/* number of active worker threads */
pthread_mutex_t		Work_mutex;	/* guard access to Workers, Rand */
pthread_cond_t		Work_cond;	/* condition variable for Workers==0 */



/*
 *	main()
 *
 *	Get started, and main loop.
 */

int
main(
	int				argc,
	char				*argv[])
{

	utc_t				start_utc, stop_utc, span_utc;
	struct tm			time_tm;

	rpc_ns_handle_t			import_context;
	int				server_num, nservers, ret;
	unsigned32			status;
	uuid_t				obj_uuid;
	unsigned_char_t			*string_binding, *protseq;
	rpc_binding_handle_t		bind_handle[MAX_SERVERS];
	work_arg_t			work_arg[MAX_SERVERS];
	unsigned_char_t			*server_name[MAX_SERVERS]; 
	pthread_t			thread_id[MAX_SERVERS];


	/* Check usage and initialize. */
	if (argc < 2 || (nservers = argc-1) > MAX_SERVERS)
	{
		fprintf(stderr,
		    "Usage: %s server_name ...(up to %d server_name's)...\n",
		    argv[0], MAX_SERVERS);
		exit(1);
	}
	
	for (server_num = 0; server_num < nservers; server_num += 1)
	{
		server_name[server_num] = (unsigned_char_t *)argv[1+server_num];
	}

	/* Initialize object uuid. */
	uuid_from_string(OBJ_UUID, &obj_uuid, &status);
	if (status != uuid_s_ok)
	{
		print_error("uuid_from_string", status);
		exit(1);
	}

	/* Import binding info from namespace. */
	for (server_num = 0; server_num < nservers; server_num += 1)
	{
		/* Begin the binding import loop. */
		rpc_ns_binding_import_begin(rpc_c_ns_syntax_dce,
		    server_name[server_num], timop_v1_0_c_ifspec,
		    &obj_uuid, &import_context, &status);
		if (status != rpc_s_ok)
		{
			print_error("rpc_ns_binding_import_begin()", status);
			exit(1);
		}

		/* Import bindings one at a time. */
		while (1)
		{
			rpc_ns_binding_import_next(import_context,
			    &bind_handle[server_num], &status);
			if (status != rpc_s_ok) 
			{
				print_error("rpc_ns_binding_import", status);
				exit(1);
			}

			/* Select, say, the first binding over UDP. */
			rpc_binding_to_string_binding(bind_handle[server_num],
			    &string_binding, &status);
			if (status != rpc_s_ok)
			{
				print_error("rpc_binding_to_string_binding", status);
				exit(1);
			}

			rpc_string_binding_parse(string_binding, NULL,
			    &protseq, NULL, NULL, NULL, &status);
			if (status != rpc_s_ok)
			{
				print_error("rpc_string_binding_parse", status);
				exit(1);
			}

			rpc_string_free(&string_binding, &status);
			ret = strcmp((char *)protseq, "ncadg_ip_udp");
		        rpc_string_free(&protseq, &status);
			if (ret == 0)
			{
		                break;
			}
		}

		/* End the binding import loop. */
		rpc_ns_binding_import_done(&import_context, &status);
		if (status != rpc_s_ok)
		{
			print_error("rpc_ns_binding_import_done", status);
			exit(1);
		}

	}

	/* Annotate binding handles for security. */
	for (server_num = 0; server_num < nservers; server_num += 1)
	{
		rpc_binding_set_auth_info(bind_handle[server_num],
		    SERVER_PRINC_NAME, rpc_c_protect_level_pkt_integ,
		    rpc_c_authn_dce_secret, NULL /*default login context*/,
		    rpc_c_authz_name, &status);
		if (status != rpc_s_ok)
		{
			print_error("rpc_binding_set_auth_info", status);
			exit(1);
		}

	}

	/* Initialize mutex and condition variable. */
	ret = pthread_mutex_init(&Work_mutex, pthread_mutexattr_default);
	if (ret == -1)
	{
		print_error("pthread_mutex_init", status);
		exit(1);
	}

	ret = pthread_cond_init(&Work_cond, pthread_condattr_default);
	if (ret == -1)
	{
		print_error("pthread_cond_init", status);
		exit(1);
	}

	/* Initialize random number generator. */
#ifdef _HPUX_SOURCE
	srand(time(NULL));
#else
	srandom(time(NULL));
#endif
	/* Initialize work args that are constant throughout main loop. */
	for (server_num = 0; server_num < nservers; server_num += 1)
	{
		work_arg[server_num].server_num = server_num;
		work_arg[server_num].server_name = server_name[server_num];
		work_arg[server_num].bind_handle = bind_handle[server_num];
	}

	/* Print out the year and date, just once. */
	ret = utc_gettime(&start_utc);
	if (ret == -1)
	{
		print_error("utc_gettime", status);
		exit(1);
	}

	ret = utc_gmtime(&time_tm, NULL, NULL, NULL, &start_utc);
	if (ret == -1)
	{
		print_error("utc_gmtime", status);
		exit(1);
	}
	fprintf(stdout, "\n%24.24s UTC (Z time zone)\n\n", asctime(&time_tm));

	/* Main loop -- never exits -- interrupt to quit. */
	while (1)
	{
		/* Per-loop initialization.  We're single-threaded here, so
		   locks and reentrant random number generator unnecessary. */
		Rand = 0;
		Workers = nservers;
		for (server_num = 0; server_num < nservers; server_num += 1)
		{
#ifdef _HPUX_SOURCE
			work_arg[server_num].rand = rand()%MAX_RANDOM;
#else
			work_arg[server_num].rand = random()%MAX_RANDOM;
#endif
		}

		/* Get client's start timestamp. */
		ret = utc_gettime(&start_utc);
		if (ret == -1)
		{
			print_error("utc_gettime", status);
			exit(1);
		}


		/* Spawn a worker thread for each server. */
		for (server_num = 0; server_num < nservers; server_num += 1)
		{
			ret = pthread_create(&thread_id[server_num],
			    pthread_attr_default, (pthread_startroutine_t)do_work,
			    (void *)&work_arg[server_num]);
			if (ret == -1)
			{
				print_error("pthread_create", status);
				exit(1);
			}

		}

		/* Reap the worker threads; pthread_cond_wait() semantics
		   requires it to be coded this way. */
		ret = pthread_mutex_lock(&Work_mutex);
		if (ret == -1)
		{
			print_error("pthread_mutex_lock", status);
			exit(1);
		}


		while (Workers != 0)
		{
			ret = pthread_cond_wait(&Work_cond, &Work_mutex);
			if (ret == -1)
			{
				print_error("pthread_cond_wait", status);
				exit(1);
			}

		}
		ret = pthread_mutex_unlock(&Work_mutex);
		if (ret == -1)
		{
			print_error("pthread_mutex_unlock", status);
			exit(1);
		}

		/* Reclaim storage. */
		for (server_num = 0; server_num < nservers; server_num += 1)
		{
			ret = pthread_detach(&thread_id[server_num]);
			if (ret == -1)
			{
				print_error("pthread_detach", status);
				exit(1);
			}

		}

		/* Any failures? */
		for (server_num = 0; server_num < nservers; server_num += 1)
		{
			if (work_arg[server_num].status != DO_WORK_OK)
			{
				exit(1);
			}
		}

		/* Get client's stop timestamp. */
		ret = utc_gettime(&stop_utc);
		if (ret == -1)
		{
			print_error("utc_gettime", status);
			exit(1);
		}


		/* Calculate the span of client's start and stop timestamps. */
		ret = utc_spantime(&span_utc, &start_utc, &stop_utc);
		if (ret == -1)
		{
			print_error("utc_spantime", status);
			exit(1);
		}



		/* Print total results. */
		print_report((unsigned_char_t *)"(client)", CLIENT_NUM,
		    &span_utc, Rand);
	}

	/* Not reached. */
}


/*
 *	do_work()
 *
 *	Do the work.  This is done in parallel threads, so we want it
 *	(and the subroutine print_report() it calls) to be reentrant.
 */

void
do_work(
	work_arg_t			*work_arg_p)
{
	int				server_num, *status_p, ret;
	unsigned_char_t			*server_name;
	rpc_binding_handle_t		bind_handle;
	idl_long_int			rand, status;
	error_status_t			remote_status = rpc_s_ok;
	timestamp_t			timestamp;


	/* Unpackage the args into local variables. */
	server_num = work_arg_p->server_num;
	server_name = work_arg_p->server_name;
	bind_handle = work_arg_p->bind_handle;
	rand = work_arg_p->rand;
	status_p = &work_arg_p->status;

	/* Do the RPC! */
	timop_getspan(bind_handle, rand, timestamp, &status, &remote_status);
	if (remote_status != rpc_s_ok)
	{
		print_error("timop_getspan remote error", status);
		*status_p = DO_WORK_ERR;
		pthread_exit(NULL);
		/* Not reached. */
	}

	if (status != rand)
	{
		print_error("timop_getspan error", status);
		*status_p = DO_WORK_ERR;
		pthread_exit(NULL);
		/* Not reached. */
	}

	/* Print report.  Not a critical section here because print_report()
	   is supposed to be implemented to be reentrant. */
	print_report(server_name, server_num, (utc_t *)timestamp, rand);

	/* Update Rand and decrement Workers.  As implemented, it is a
	   critical section, so must be locked. */
	ret = pthread_mutex_lock(&Work_mutex);
	if (ret == -1)
	{
		print_error("pthread_mutex_lock", status);
		exit(1);
	}
	Workers -= 1;
	if (Workers == 0)
	{
		/* Last worker signals main thread. */
		ret = pthread_cond_signal(&Work_cond);
		if (ret == -1)
		{
			print_error("pthread_cond_signal", status);
			exit(1);
		}
	}
	Rand += rand;
	ret = pthread_mutex_unlock(&Work_mutex);
	if (ret == -1)
	{
		print_error("pthread_mutex_unlock", status);
		exit(1);
	}

	/* Done. */
	*status_p = DO_WORK_OK;
	pthread_exit(NULL);
	/* Not reached. */
}


/*
 *	print_report()
 *
 *	Print DTS timestamp interval, to millisecond granularity.
 *	Implemented this way so it is reentrent (assuming all the underlying
 *	OS subroutines it calls are reentrant).
 *	This kind of timestamp manipulation is always messy -- see the
 *	manual for the formats of structures and print-strings we use.
 */

void
print_report(
	unsigned_char_t		*server_name,
	int			server_num,
	utc_t			*utc_p,
	long			rand)
{
#define LINE_LEN		78
#define COL1			0
#define COL2			44
#define COL3a			47
#define COL3b			60
#define COL4			70
	char			asctime_buf[26], ascinacc_buf[26],
				    time_ns_buf[10], inacc_ns_buf[10],
				    report[LINE_LEN+3];
	int			inacc_sec, ret;
	long			time_ns, inacc_ns;
	struct tm		time_tm, inacc_tm;


	/* Print server_name into report.  Pad or truncate as necessary. */
	sprintf(report+COL1, "%*.*s  ", COL2-2, COL2-2, (char *)server_name);

	/* Print server_num into report. */
	if (server_num != CLIENT_NUM)
	{
		sprintf(report+COL2, "%1.1d  ", server_num);
	}
	else
	{
		sprintf(report+COL2, "%1.1s  ", "*");
	}

	/* Format utc_p and print it into report. */
	ret = utc_gmtime(&time_tm, &time_ns, &inacc_tm, &inacc_ns, utc_p);
	if (ret == -1)
	{
		fprintf(stderr, "utc_gmtime error");
		exit(1);
	}
	memcpy(asctime_buf, asctime(&time_tm), 26);	/* reentrancy bug! */
	memcpy(ascinacc_buf, asctime(&inacc_tm), 26);	/* reentrancy bug! */
	sprintf(time_ns_buf, "%9.9d", time_ns);
	sprintf(inacc_ns_buf, "%9.9d", inacc_ns);
	inacc_sec = inacc_tm.tm_yday*24*60*60 + inacc_tm.tm_hour*60*60 +
	    inacc_tm.tm_min*60 + inacc_tm.tm_sec;
	sprintf(report+COL3a, "%8.8s.%3.3sI", asctime_buf+11,
	    time_ns_buf);
	if (inacc_tm.tm_year != -1)
	{
		sprintf(report+COL3b, "%4.4d.%3.3s  ", inacc_sec,
		    inacc_ns_buf);
	}
	else
	{
		sprintf(report+COL3b, "%8.8s  ", "infinity");
	}

	/* Print rand into report. */
	if (server_num != CLIENT_NUM)
	{
		sprintf(report+COL4, "%8d\n", rand);
	}
	else
	{
		sprintf(report+COL4, "%8d\n\n", rand);
	}

	/* Output report. */
	fprintf(stdout, "%s", report);
	return;
}



/*****
*
* print_error-- Client version. Prints text associated with bad status code.
*
*****/
void
print_error(caller, status)
char *caller;		/* a string identifying the routine that received the error  */
error_status_t status;	/* the status we want to print the message for               */
{
	dce_error_string_t error_string;
	int print_status;

	dce_error_inq_text(status, error_string, &print_status);
	fprintf(stderr," Client: %s: %s\n", caller, error_string);
}

