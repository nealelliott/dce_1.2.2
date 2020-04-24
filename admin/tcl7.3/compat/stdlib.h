/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: stdlib.h,v $
 * Revision 1.1.6.2  1996/02/18  23:32:20  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:49  marty]
 *
 * Revision 1.1.6.1  1995/12/08  21:28:40  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:00:32  root]
 * 
 * Revision 1.1.4.1  1994/06/09  16:03:29  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:48:58  devsrc]
 * 
 * Revision 1.1.2.1  1993/12/12  01:38:10  rousseau
 * 	Initial 7.3 version.
 * 	[1993/12/12  01:37:51  rousseau]
 * 
 * $EndLog$
 */
/*
 * stdlib.h --
 *
 *	Declares facilities exported by the "stdlib" portion of
 *	the C library.  This file isn't complete in the ANSI-C
 *	sense;  it only declares things that are needed by Tcl.
 *	This file is needed even on many systems with their own
 *	stdlib.h (e.g. SunOS) because not all stdlib.h files
 *	declare all the procedures needed here (such as strtod).
 *
 * Copyright (c) 1991 The Regents of the University of California.
 * All rights reserved.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 * 
 * IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
 * OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF
 * CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 * $Header: /u0/rcs_trees/dce/rcs/admin/tcl7.3/compat/stdlib.h,v 1.1.6.2 1996/02/18 23:32:20 marty Exp $ SPRITE (Berkeley)
 */

#ifndef _STDLIB
#define _STDLIB

#include <tcl.h>

extern void		abort _ANSI_ARGS_((void));
extern double		atof _ANSI_ARGS_((CONST char *string));
extern int		atoi _ANSI_ARGS_((CONST char *string));
extern long		atol _ANSI_ARGS_((CONST char *string));
extern char *		calloc _ANSI_ARGS_((unsigned int numElements,
			    unsigned int size));
extern void		exit _ANSI_ARGS_((int status));
extern int		free _ANSI_ARGS_((char *blockPtr));
extern char *		getenv _ANSI_ARGS_((CONST char *name));
extern char *		malloc _ANSI_ARGS_((unsigned int numBytes));
extern void		qsort _ANSI_ARGS_((VOID *base, int n, int size,
			    int (*compar)(CONST VOID *element1, CONST VOID
			    *element2)));
extern char *		realloc _ANSI_ARGS_((char *ptr, unsigned int numBytes));
extern double		strtod _ANSI_ARGS_((CONST char *string, char **endPtr));
extern long		strtol _ANSI_ARGS_((CONST char *string, char **endPtr,
			    int base));
extern unsigned long	strtoul _ANSI_ARGS_((CONST char *string,
			    char **endPtr, int base));

#endif /* _STDLIB */
