/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: pi_strsep.c,v $
 * Revision 1.1.6.2  1996/03/11  13:26:31  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:14:31  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:57:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:29:19  root]
 * 
 * Revision 1.1.4.2  1992/12/29  12:38:31  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:34:35  zeliff]
 * 
 * Revision 1.1.2.2  1992/04/23  14:17:07  burati
 * 	Initial version, from original 4.4 source.
 * 	[1992/04/23  01:46:18  burati]
 * 
 * $EndLog$
 */

/*-
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that: (1) source distributions retain this entire copyright
 * notice and comment, and (2) distributions including binaries display
 * the following acknowledgement:  ``This product includes software
 * developed by the University of California, Berkeley and its contributors''
 * in the documentation or other materials provided with the distribution
 * and in all advertising materials mentioning features or use of this
 * software. Neither the name of the University nor the names of its
 * contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#include <string.h>
#include <stdio.h>

#if defined(LIBC_SCCS) && !defined(lint)
static const char sccsid[] = "@(#)strsep.c	5.3 (Berkeley) 5/18/90";
#endif /* LIBC_SCCS and not lint */

/*
 * Get next token from string *stringp, where tokens are nonempty
 * strings separated by characters from delim.  
 *
 * Writes NULs into the string at *stringp to end tokens.
 * delim need not remain constant from call to call.
 * On return, *stringp points past the last NUL written (if there might
 * be further tokens), or is NULL (if there are definitely no more tokens).
 *
 * If *stringp is NULL, strtoken returns NULL.
 */
char *strsep
#ifndef __STDC__
    (stringp, delim)
	register char **stringp;
	register const char *delim;
#else
  (
	register char **stringp,
	register const char *delim
  )
#endif
{
	register char *s;
	register const char *spanp;
	register int c, sc;
	char *tok;

	if ((s = *stringp) == NULL)
		return (NULL);
	for (tok = s;;) {
		c = *s++;
		spanp = delim;
		do {
			if ((sc = *spanp++) == c) {
				if (c == 0)
					s = NULL;
				else
					s[-1] = 0;
				*stringp = s;
				return (tok);
			}
		} while (sc != 0);
	}
	/* NOTREACHED */
}
