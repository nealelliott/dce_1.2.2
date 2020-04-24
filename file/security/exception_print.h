/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: exception_print.h,v $
 * Revision 1.1.317.1  1996/10/02  20:59:37  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:47:44  damon]
 *
 * $EndLog$
 */
/*
 *	exception_print.h -- print out the appropriate exception string
 *
 *	    Also include pthread and exception handling stuff.
 *
 * Copyright (C) 1994, 1991 Transarc Corporation
 * All rights reserved.
 */

#ifndef TRANSARC_DFSAUTH_EXCEPTION_PRINT_H
#define TRANSARC_DFSAUTH_EXCEPTION_PRINT_H	1

#include <stdio.h>

#include <dcedfs/param.h>
#include <dcedfs/stds.h>

#if !defined(KERNEL)
#include <pthread.h>
#else /* !defined(KERNEL) */
#define _PTHREAD_PROTOTYPE(x) ()
#include <dce/ker/pthread.h>
#include <dce/ker/exc_handling.h>
#endif /* !defined(KERNEL) */


IMPORT void PrintException _TAKES((
				   EXCEPTION *	thisCatchP,
				   char *	routineNameP,
				   char *	tagP,
				   char *	filenameP,
				   int		line
				 ));

IMPORT char * ExceptionToString _TAKES((
					EXCEPTION *	thisCatchP
				      ));

#endif /* TRANSARC_DFSAUTH_EXCEPTION_PRINT_H */


