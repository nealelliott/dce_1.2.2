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
 * $Log: tmpnam.c,v $
 * Revision 1.1.6.2  1996/02/18  19:17:11  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:03:00  marty]
 *
 * Revision 1.1.6.1  1995/12/08  21:28:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:00:48  root]
 * 
 * Revision 1.1.4.1  1994/06/09  16:03:41  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:49:04  devsrc]
 * 
 * Revision 1.1.2.1  1993/12/12  01:38:18  rousseau
 * 	Initial 7.3 version.
 * 	[1993/12/12  01:37:56  rousseau]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) 1988 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that this notice is preserved and that due credit is given
 * to the University of California at Berkeley. The name of the University
 * may not be used to endorse or promote products derived from this
 * software without specific written prior permission. This software
 * is provided ``as is'' without express or implied warranty.
 */

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)tmpnam.c	4.4 (Berkeley) 6/8/88";
#endif /* LIBC_SCCS and not lint */

#include <sys/param.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <stdio.h>

/*
 * Use /tmp instead of /usr/tmp, because L_tmpname is only 14 chars
 * on some machines (like NeXT machines) and /usr/tmp will cause
 * buffer overflows.
 */

#define	P_tmpdir	"/tmp"

char *
tmpnam(s)
	char *s;
{
	static char name[50];
	char *mktemp();

	if (!s)
		s = name;
	(void)sprintf(s, "%s/XXXXXX", P_tmpdir);
	return(mktemp(s));
}
