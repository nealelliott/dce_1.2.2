/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: net_read.c,v $
 * Revision 1.1.4.2  1996/02/18  00:15:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:04:55  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:43:29  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:18:34  root]
 * 
 * Revision 1.1.2.2  1992/12/29  16:30:42  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:18:18  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:50:25  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  net_read.c V=3 10/24/91 //littl/prgy/krb5/lib/os
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/os/net_read.c,v $
 * $Author: marty $
 *
 * Copyright 1987, 1988, 1990 by the Massachusetts Institute of Technology.
 * All Rights Reserved.
 *
 * Export of this software from the United States of America is assumed
 *   to require a specific license from the United States Government.
 *   It is the responsibility of any person or organization contemplating
 *   export to obtain such a license before exporting.
 * 
 * WITHIN THAT CONSTRAINT, permission to use, copy, modify, and
 * distribute this software and its documentation for any purpose and
 * without fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright notice and
 * this permission notice appear in supporting documentation, and that
 * the name of M.I.T. not be used in advertising or publicity pertaining
 * to distribution of the software without specific, written prior
 * permission.  M.I.T. makes no representations about the suitability of
 * this software for any purpose.  It is provided "as is" without express
 * or implied warranty.
 * 
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_net_read_c[] =
"$Id: net_read.c,v 1.1.4.2 1996/02/18 00:15:36 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>

/*
 * krb5_net_read() reads from the file descriptor "fd" to the buffer
 * "buf", until either 1) "len" bytes have been read or 2) cannot
 * read anymore from "fd".  It returns the number of bytes read
 * or a read() error.  (The calling interface is identical to
 * read(2).)
 *
 * XXX must not use non-blocking I/O
 */

int
krb5_net_read(fd, buf, len)
int fd;
register char *buf;
register int len;
{
    int cc, len2 = 0;

    do {
	cc = read(fd, buf, len);
	if (cc < 0)
	    return(cc);		 /* errno is already set */
	else if (cc == 0) {
	    return(len2);
	} else {
	    buf += cc;
	    len2 += cc;
	    len -= cc;
	}
    } while (len > 0);
    return(len2);
}
