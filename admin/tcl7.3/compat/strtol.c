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
 * $Log: strtol.c,v $
 * Revision 1.1.6.2  1996/02/18  19:17:07  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:02:57  marty]
 *
 * Revision 1.1.6.1  1995/12/08  21:28:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:00:44  root]
 * 
 * Revision 1.1.4.1  1994/06/09  16:03:37  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:49:02  devsrc]
 * 
 * Revision 1.1.2.1  1993/12/12  01:38:15  rousseau
 * 	Initial 7.3 version.
 * 	[1993/12/12  01:37:54  rousseau]
 * 
 * $EndLog$
 */
/* 
 * strtol.c --
 *
 *	Source code for the "strtol" library procedure.
 *
 * Copyright (c) 1988 The Regents of the University of California.
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
static char rcsid[] = "$Header: /u0/rcs_trees/dce/rcs/admin/tcl7.3/compat/strtol.c,v 1.1.6.2 1996/02/18 19:17:07 marty Exp $ SPRITE (Berkeley)";
#endif /* not lint */

#include <ctype.h>


/*
 *----------------------------------------------------------------------
 *
 * strtol --
 *
 *	Convert an ASCII string into an integer.
 *
 * Results:
 *	The return value is the integer equivalent of string.  If endPtr
 *	is non-NULL, then *endPtr is filled in with the character
 *	after the last one that was part of the integer.  If string
 *	doesn't contain a valid integer value, then zero is returned
 *	and *endPtr is set to string.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

long int
strtol(string, endPtr, base)
    char *string;		/* String of ASCII digits, possibly
				 * preceded by white space.  For bases
				 * greater than 10, either lower- or
				 * upper-case digits may be used.
				 */
    char **endPtr;		/* Where to store address of terminating
				 * character, or NULL. */
    int base;			/* Base for conversion.  Must be less
				 * than 37.  If 0, then the base is chosen
				 * from the leading characters of string:
				 * "0x" means hex, "0" means octal, anything
				 * else means decimal.
				 */
{
    register char *p;
    int result;

    /*
     * Skip any leading blanks.
     */

    p = string;
    while (isspace(*p)) {
	p += 1;
    }

    /*
     * Check for a sign.
     */

    if (*p == '-') {
	p += 1;
	result = -(strtoul(p, endPtr, base));
    } else {
	if (*p == '+') {
	    p += 1;
	}
	result = strtoul(p, endPtr, base);
    }
    if ((result == 0) && (endPtr != 0) && (*endPtr == p)) {
	*endPtr = string;
    }
    return result;
}
