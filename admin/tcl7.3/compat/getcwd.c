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
 * $Log: getcwd.c,v $
 * Revision 1.1.6.2  1996/02/18  19:16:58  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:02:51  marty]
 *
 * Revision 1.1.6.1  1995/12/08  21:28:33  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:00:27  root]
 * 
 * Revision 1.1.4.1  1994/06/09  16:03:25  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:48:55  devsrc]
 * 
 * Revision 1.1.2.1  1993/12/12  01:38:07  rousseau
 * 	Initial 7.3 version.
 * 	[1993/12/12  01:37:49  rousseau]
 * 
 * $EndLog$
 */
/* 
 * getcwd.c --
 *
 *	This file provides an implementation of the getcwd procedure
 *	that uses getwd, for systems with getwd but without getcwd.
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
static char rcsid[] = "$Header: /u0/rcs_trees/dce/rcs/admin/tcl7.3/compat/getcwd.c,v 1.1.6.2 1996/02/18 19:16:58 marty Exp $ SPRITE (Berkeley)";
#endif /* not lint */

#include "tclInt.h"
#include "tclUnix.h"

extern char *getwd _ANSI_ARGS_((char *pathname));

char *
getcwd(buf, size)
    char *buf;			/* Where to put path for current directory. */
    size_t size;		/* Number of bytes at buf. */
{
    char realBuffer[MAXPATHLEN+1];
    int length;

    if (getwd(realBuffer) == NULL) {
	/*
	 * There's not much we can do besides guess at an errno to
	 * use for the result (the error message in realBuffer isn't
	 * much use...).
	 */

	errno = EACCES;
	return NULL;
    }
    length = strlen(realBuffer);
    if (length >= size) {
	errno = ERANGE;
	return NULL;
    }
    strcpy(buf, realBuffer);
    return buf;
}

