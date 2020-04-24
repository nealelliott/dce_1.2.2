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
 * $Log: tclMtherr.c,v $
 * Revision 1.1  1996/09/27  20:01:41  wwang
 * Initial revision
 *
 * Revision 1.1  1996/09/27  19:59:14  wwang
 * Initial revision
 *
 * Revision 1.1.4.1  1994/06/09  16:04:48  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:49:43  devsrc]
 *
 * Revision 1.1.2.1  1993/12/12  01:49:26  rousseau
 * 	Initial 7.3 version.
 * 	[1993/12/12  01:46:12  rousseau]
 * 
 * $EndLog$
 */
/* 
 * tclMatherr.c --
 *
 *	This function provides a default implementation of the
 *	"matherr" function, for SYS-V systems where it's needed.
 *
 * Copyright (c) 1993 The Regents of the University of California.
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
static char rcsid[] = "$Header: /project/dce/mos/build/bld-1203/tcl_dce/RCS/tclMtherr.c,v 1.1 1996/09/27 20:01:41 wwang Exp $ SPRITE (Berkeley)";
#endif /* not lint */

#include "tclInt.h"
#include <math.h>

#ifndef TCL_GENERIC_ONLY
#include "tclUnix.h"
#else
#define NO_ERRNO_H
#endif

#ifdef NO_ERRNO_H
extern int errno;			/* Use errno from tclExpr.c. */
#define EDOM 33
#define ERANGE 34
#endif

/*
 * The following variable is secretly shared with Tcl so we can
 * tell if expression evaluation is in progress.  If not, matherr
 * just emulates the default behavior, which includes printing
 * a message.
 */

extern int tcl_MathInProgress;


/*
 *----------------------------------------------------------------------
 *
 * matherr --
 *
 *	This procedure is invoked on Sys-V systems when certain
 *	errors occur in mathematical functions.  Type "man matherr"
 *	for more information on how this function works.
 *
 * Results:
 *	Returns 1 to indicate that we've handled the error
 *	locally.
 *
 * Side effects:
 *	Sets errno based on what's in xPtr.
 *
 *----------------------------------------------------------------------
 */

int
matherr(xPtr)
    struct exception *xPtr;	/* Describes error that occurred. */
{
    if (!tcl_MathInProgress) {
	return 0;
    }
    if ((xPtr->type == DOMAIN) || (xPtr->type == SING)) {
	errno = EDOM;
    } else {
	errno = ERANGE;
    }
    return 1;
}
