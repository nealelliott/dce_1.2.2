/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: timop_manager.c,v $
 * Revision 1.2.5.2  1996/02/17  23:24:10  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:42:31  marty]
 *
 * Revision 1.2.5.1  1995/12/11  20:15:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:48:49  root]
 * 
 * Revision 1.2.3.3  1993/10/21  19:44:48  weir
 * 	timop for DCE 1.0.3, with following fixes:
 * 	CR 4804: use DCE status codes
 * 	CR 4805: use relative principal names
 * 	CR 7460: initialize remote status parameter in server manager
 * 	CR 9215: port to HP platform.
 * 	[1993/10/21  19:38:27  weir]
 * 
 * Revision 1.2.2.3  1993/02/04  20:32:31  cjd
 * 	Embedded copyright notice
 * 	[1993/02/04  20:29:54  cjd]
 * 
 * Revision 1.2.2.2  1992/12/18  18:18:36  marcyw
 * 	Defect 4998: adding disclaimer comment
 * 	[1992/12/18  18:12:23  marcyw]
 * 
 * Revision 1.2  1992/01/19  22:14:30  devrcs
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
**	timop_manager.c
**
**	Manager routines for timop interface.
*/

#include <stdio.h>
#include <dce/utc.h>
#include "timop.h"
#include "timop_aux.h"
#include "timop_server.h"


/*
 *	getspan_ep()
 *
 *	Entry point for timop_getspan() operation.
 *	Note it is reentrant, so we can have a multi-threaded server.
 */

void
getspan_ep(
	rpc_binding_handle_t		bind_handle,
	idl_long_int			rand,
	timestamp_t			timestamp,
	idl_long_int			*status_p,
	error_status_t			*remote_status_p)
{
	int				ret;

	/* We have to explicitly initialize the remote status value;  */
	/*  otherwise, if no error occurs in the transmission (which  */
	/*  would cause the runtime to assign an error value to this  */
	/*  variable), its value will be whatever it happened to be   */
	/*  when the RPC was made by the client...                    */
	*remote_status_p = rpc_s_ok;

	/* Call reference monitor, to make authorization decision. */
	ret = ref_mon(bind_handle, GETSPAN_OP);
	if (ret == DENY_ACCESS)
	{
		*status_p = TIMOP_ERR;
		return;
	}

	/* Service the request, i.e., do the actual remote procedure. */
	ret = do_getspan(rand, timestamp);
	if (ret == GETSPAN_ERR)
	{
		*status_p = TIMOP_ERR;
		return;
	}

	/* Return the input random number as a status value (!= TIMOP_ERR). */
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


	/* Get server's start timestamp. */
	ret = utc_gettime(&start_utc);
	if (ret == -1)
	{
		fprintf(stderr, " Server: utc_gettime() failed\n");
		return(GETSPAN_ERR);
	}

	/* Do service (here a random factorial, but could be anything). */
	for (n = i = 1; i <= rand; i += 1)
	{
		n *= i;		/* Burn cpu -- use your imagination. */
	}

	/* Get server's stop timestamp. */
	ret = utc_gettime(&stop_utc);
	if (ret == -1)
	{
		fprintf(stderr, " Server: utc_gettime() failed\n");
		return(GETSPAN_ERR);
	}

	/* Calculate the span of server's start and stop timestamps. */
	ret = utc_spantime((utc_t *)timestamp, &start_utc, &stop_utc);
	if (ret == -1)
	{
		fprintf(stderr, " Server: utc_spantime() failed\n");
		return(GETSPAN_ERR);
	}

	/* Success. */
	return(GETSPAN_OK);
}

