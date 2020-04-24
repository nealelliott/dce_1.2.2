/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: panic.c,v $
 * Revision 1.1  1996/09/27  20:01:41  wwang
 * Initial revision
 *
 * Revision 1.1  1996/09/27  19:59:14  wwang
 * Initial revision
 *
 * Revision 1.1.5.1  1994/06/09  16:04:01  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:49:15  devsrc]
 *
 * Revision 1.1.2.1  1993/12/12  01:48:58  rousseau
 * 	Initial 7.3 version.
 * 	[1993/12/12  01:24:04  rousseau]
 * 
 * 	Initial 7.3 version.
 * 
 * $EndLog$
 */
/* 
 * panic.c --
 *
 *	Source code for the "panic" library procedure for Tcl;
 *	individual applications will probably override this with
 *	an application-specific panic procedure.
 *
 * Copyright (c) 1988-1993 The Regents of the University of California.
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
 */

#ifndef lint
static char rcsid[] = "$Header: /project/dce/mos/build/bld-1203/tcl_dce/RCS/panic.c,v 1.1 1996/09/27 20:01:41 wwang Exp $ SPRITE (Berkeley)";
#endif

#include <stdio.h>
#ifdef NO_STDLIB_H
#   include "compat/stdlib.h"
#else
#   include <stdlib.h>
#endif

/*
 *----------------------------------------------------------------------
 *
 * panic --
 *
 *	Print an error message and kill the process.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The process dies, entering the debugger if possible.
 *
 *----------------------------------------------------------------------
 */

	/* VARARGS ARGSUSED */
void
panic(format, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)
    char *format;		/* Format string, suitable for passing to
				 * fprintf. */
    char *arg1, *arg2, *arg3;	/* Additional arguments (variable in number)
				 * to pass to fprintf. */
    char *arg4, *arg5, *arg6, *arg7, *arg8;
{
    (void) fprintf(stderr, format, arg1, arg2, arg3, arg4, arg5, arg6,
	    arg7, arg8);
    (void) fflush(stderr);
    abort();
}
