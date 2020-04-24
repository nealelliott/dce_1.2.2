/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: faddr_ordr.c,v $
 * Revision 1.1.6.2  1996/02/18  00:13:48  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:03:30  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:58:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:59:45  root]
 * 
 * Revision 1.1.2.2  1992/12/29  15:55:26  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:13:56  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:51:45  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  faddr_ordr.c V=3 10/24/91 //littl/prgy/krb5/lib/krb
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/krb/faddr_ordr.c,v $
 * $Author: marty $
 *
 * Copyright 1990 by the Massachusetts Institute of Technology.
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
 *
 * krb5_fulladdr_order()
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_faddr_ordr_c[] =
"$Id: faddr_ordr.c,v 1.1.6.2 1996/02/18 00:13:48 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>

#ifndef min
#define min(a,b) ((a) > (b) ? (a) : (b))
#endif

/*
 * Return an ordering on the two full addresses:  0 if the same,
 * < 0 if first is less than 2nd, > 0 if first is greater than 2nd.
 */
int
krb5_fulladdr_order(addr1, addr2)
register const krb5_fulladdr *addr1;
register const krb5_fulladdr *addr2;
{
    int dir;
    const int minlen = min(addr1->address->length, addr2->address->length);
    register int i;

    dir = addr1->address->addrtype - addr2->address->addrtype;
    if (dir)
	return(dir);

    dir = addr1->address->length - addr2->address->length;
    if (dir)
	return(dir);

    for (i = 0; i < minlen; i++) {
	if ((unsigned char) addr1->address->contents[i] <
	    (unsigned char) addr2->address->contents[i])
	    return -1;
	else if ((unsigned char) addr1->address->contents[i] >
		 (unsigned char) addr2->address->contents[i])
	    return 1;
    }
    if (addr1->port > addr2->port)
	return(1);
    else if (addr1->port < addr2->port)
	return(-1);
    else
	return(0);
}
