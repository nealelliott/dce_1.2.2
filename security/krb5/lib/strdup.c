/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: strdup.c,v $
 * Revision 1.1.4.2  1996/02/18  00:16:19  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:05:29  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:45:49  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:19:03  root]
 * 
 * Revision 1.1.2.2  1992/12/29  16:33:09  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:21:00  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:50:55  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  strdup.c V=1 06/06/91 //littl/prgy/krb5/lib
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * Copyright (c) 1988 The Regents of the University of California.
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

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)strdup.c	5.3 (Berkeley) 6/1/90";
#endif /* LIBC_SCCS and not lint */

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>

char *
strdup(str)
	const char *str;
{
	int len;
	char *copy;

	len = strlen(str) + 1;
	if (!(copy = malloc((u_int)len)))
		return((char *)0);
	bcopy(str, copy, len);
	return(copy);
}
