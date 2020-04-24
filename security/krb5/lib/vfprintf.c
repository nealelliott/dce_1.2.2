/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: vfprintf.c,v $
 * Revision 1.1.4.2  1996/02/18  00:16:23  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:05:30  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:45:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:19:05  root]
 * 
 * Revision 1.1.2.2  1992/12/29  16:33:16  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:21:07  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:50:42  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  vfprintf.c V=1 05/03/90 //littl/prgy/krb5/lib
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
static char sccsid[] = "@(#)vfprintf.c	5.2 (Berkeley) 6/27/88";
#endif /* LIBC_SCCS and not lint */

#include <stdio.h>
#include <varargs.h>

int
vfprintf(iop, fmt, ap)
	FILE *iop;
	char *fmt;
	va_list ap;
{
	int len;
	char localbuf[BUFSIZ];

	if (iop->_flag & _IONBF) {
		iop->_flag &= ~_IONBF;
		iop->_ptr = iop->_base = localbuf;
		len = _doprnt(fmt, ap, iop);
		(void) fflush(iop);
		iop->_flag |= _IONBF;
		iop->_base = NULL;
		iop->_bufsiz = 0;
		iop->_cnt = 0;
	} else
		len = _doprnt(fmt, ap, iop);

	return (ferror(iop) ? EOF : len);
}
