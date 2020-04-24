/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: timop_svc_manager.c,v $
 * Revision 1.1.4.2  1996/02/18  18:26:28  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:48:30  marty]
 *
 * Revision 1.1.4.1  1995/12/08  20:55:12  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  20:50:15  root]
 * 
 * Revision 1.1.2.1  1994/06/14  19:57:41  weir
 * 	DCE 1.1 Beta check-in
 * 	[1994/06/14  16:15:18  weir]
 * 
 * $EndLog$
 */
/*
 * The Example code is provided to you as "Pass-through Code".
 * Pass-through Code and its related Documentation are not warrented,
 * supported, or indemnified by OSF, and are provided "AS IS".
 * You can use, sublicense, or distribute the same at your own risk.
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
**	timop_svc_manager.c
**
**	Manager routines for timop_svc interface.
*/

#define DO_SVC

#include <dce/dce.h>
#include <stdio.h>
#include <dce/utc.h> 
#include <dce/dce_error.h>

#include <stddef.h>

#include <dce/dce_cf.h>
#include <dce/rpc.h>
#include <dce/utctypes.h>
#include <pthread.h>
#include <dce/sec_login.h>
#include <dce/dcesvcmsg.h>
#include <dce/svclog.h>
#include <dce/dce_msg.h>
#include <dce/dce_svc.h>
#include "dcetsvsvc.h"
#include "dcetsvmsg.h"
#include "dcetsvmac.h"

#include "timop_svc.h"
#include "timop_svc_aux.h"
#include "timop_svc_server.h"



/* extern char *malloc(); */
extern char *strcpy();
extern char *strcat();
extern char *strncat();


/*
 *	getspan_ep()
 *
 *	Entry point for timop_svc_getspan() operation.
 *	Note it is reentrant, so we can have a multi-threaded server.
 */

void
getspan_ep(
	rpc_binding_handle_t		bind_handle,
	idl_long_int			rand,
	idl_char			server_msg[MESSAGE_SIZE],
	timestamp_t			timestamp,
	idl_long_int			*status_p,
	error_status_t			*remote_status_p)
{
	int				ret;
	idl_char			*sp;
	static long int			call_nr = 0;
	int				entry_ix;

	call_nr++;
#ifdef DO_SVC
	dce_svc_printf(CALL_RECEIVED_MSG, call_nr);
#else
	fprintf(stderr, " Server: Received call %ld...\n", call_nr);
#endif

/* NEW CODE  */
	/* We have to explicitly initialize the remote status value;  */
	/*  otherwise, if no error occurs in the transmission (which  */
	/*  would cause the runtime to assign an error value to this  */
	/*  variable), its value will be whatever it happened to be   */
	/*  when the RPC was made by the client...                    */
	*remote_status_p = rpc_s_ok;
/* NEW CODE  */

	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_manager, svc_c_debug5, "Entering getspan_ep()"));
	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_manager, svc_c_debug5, "Calling ref_mon()"));
	/* Call reference monitor, to make authorization decision. */
	ret = ref_mon(bind_handle, GETSPAN_OP);
	if (ret == DENY_ACCESS)
	{
		*status_p = TIMOP_SVC_ERR;
		return;
	}

	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_manager, svc_c_debug5, "Calling do_getspan()"));
	/* Service the request, i.e., do the actual remote procedure. */
	ret = do_getspan(rand, timestamp);
	if (ret == GETSPAN_ERR)
	{
		*status_p = TIMOP_SVC_ERR;
		return;
	}

	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_manager, svc_c_debug5, "Preparing server message"));
	strcpy(server_msg, "Server ");
	strncat(server_msg, server_name_G, strlen(server_name_G));
	switch (entry_flag_G)
	{
		case 1:
			strncat(server_msg, " at object 1...\n", strlen(" at object 1...\n"));
			break;
		case 2:
			strncat(server_msg, " at object 2...\n", strlen(" at object 2...\n"));
			break;
		default:
			break;
	}

	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_manager, svc_c_debug5, "Exiting getspan_ep()"));
	/* Return the input random number as a status value (!= TIMOP_SVC_ERR). */
	*status_p = rand;
	/* Return all results to client, and resume listen loop. */
	return;
}


/*
 *	do_getspan()
 *
 *	Do the actual remote procedure.
 */

int
do_getspan(
	idl_long_int			rand,
	timestamp_t			timestamp)
{
	long				i;
	volatile long			n;
	int				ret;
	utc_t				start_utc, stop_utc;

	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_manager, svc_c_debug5, "Entering do_getspan()"));
	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_manager, svc_c_debug5, "Calling utc_gettime(start timestamp)"));
	/* Get server's start timestamp. */
	ret = utc_gettime(&start_utc);
	if (ret == -1)
	{
#ifdef DO_SVC
		dce_svc_printf(GETTIME_FAILED_MSG);
#else
		fprintf(stderr, " Server: utc_gettime() failed\n");
#endif
		return(GETSPAN_ERR);
	}

	/* Do service (here a random factorial, but could be anything). */
	for (n = i = 1; i <= rand; i += 1)
	{
		n *= i;		/* Burn cpu -- use your imagination. */
	}

	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_manager, svc_c_debug5, "Calling utc_gettime(stop timestamp)"));
	/* Get server's stop timestamp. */
	ret = utc_gettime(&stop_utc);
	if (ret == -1)
	{
#ifdef DO_SVC
		dce_svc_printf(GETTIME_FAILED_MSG);
#else
		fprintf(stderr, " Server: utc_gettime() failed\n");
#endif
		return(GETSPAN_ERR);
	}

	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_manager, svc_c_debug5, "Calling utc_spantime()"));
	/* Calculate the span of server's start and stop timestamps. */
	ret = utc_spantime((utc_t *)timestamp, &start_utc, &stop_utc);
	if (ret == -1)
	{
#ifdef DO_SVC
		dce_svc_printf(SPANTIME_FAILED_MSG);
#else
		fprintf(stderr, " Server: utc_spantime() failed\n");
#endif
		return(GETSPAN_ERR);
	}

	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_manager, svc_c_debug5, "Exiting do_getspan()"));
	/* Success. */
	return(GETSPAN_OK);
}


/*****
*
* print_manager_error-- Manager version. Prints text associated with bad status code.
*
*****/
void
print_manager_error(caller, status)
char *caller;		/* a string identifying the routine that received the error  */
error_status_t status;	/* the status we want to print the message for               */
{
	dce_error_string_t error_string;
	int print_status;

	dce_error_inq_text(status, error_string, &print_status);

#ifdef DO_SVC
	dce_svc_printf(MANAGER_ERROR_MSG, caller, error_string);
#else
	fprintf(stderr," Manager: %s: %s\n", caller, error_string);
#endif

/* The following shows how you can call dce_printf() to print out a message. This    */
/*  call will print out the simple error string, but not the "(dce / <comp>)" part.  */
/*  That's because those higher bits in which the latter info is contained are not   */
/*  part of the message id, I think.                                                 */
/* Thus, where dce_error_ing_text will give you back a string with something like:   */
/*  "Bad routing specification (dce / svc)", dce_printf will print only the simple   */
/*  "Bad routing specification" part...                                              */
/*                                                                                   */
/*
dce_printf(status);
*/

}
