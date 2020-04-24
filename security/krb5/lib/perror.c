/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: perror.c,v $
 * Revision 1.1.4.2  1996/02/18  00:15:56  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:05:11  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:44:27  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:18:46  root]
 * 
 * Revision 1.1.2.2  1992/12/29  16:31:43  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:19:15  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:49:18  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  perror.c V=2 07/03/90 //littl/prgy/krb5/lib
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * Copyright (c) 1988 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)perror.c	5.7 (Berkeley) 12/16/88";
#endif /* LIBC_SCCS and not lint */

#include <sys/types.h>
#include <stdio.h>			/* for sprintf() */

extern int sys_nerr;
extern char *sys_errlist[];

static char ebuf[20];

char *
strerror(errnum)
	int errnum;
{
	if ((u_int)errnum < sys_nerr)
		return(sys_errlist[errnum]);
	(void)sprintf(ebuf, "Unknown error: %d", errnum);
	return(ebuf);
}
