/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: addr_order.c,v $
 * Revision 1.1.2.1  1996/06/05  20:56:17  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  19:54:26  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * lib/krb5/krb/addr_order.c
 *
 * Copyright 1990 by the Massachusetts Institute of Technology.
 * All Rights Reserved.
 *
 * Export of this software from the United States of America may
 *   require a specific license from the United States Government.
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
 * krb5_address_order()
 */

#include "k5-int.h"

#ifndef min
#define min(a,b) ((a) < (b) ? (a) : (b))
#endif

/*
 * Return an ordering on two addresses:  0 if the same,
 * < 0 if first is less than 2nd, > 0 if first is greater than 2nd.
 */
int
krb5_address_order(context, addr1, addr2)
    krb5_context context;
    register const krb5_address *addr1;
    register const krb5_address *addr2;
{
    int dir;
    register int i;
    const int minlen = min(addr1->length, addr2->length);

    if (addr1->addrtype != addr2->addrtype)
	return(FALSE);

    dir = addr1->length - addr2->length;

    
    for (i = 0; i < minlen; i++) {
	if ((unsigned char) addr1->contents[i] <
	    (unsigned char) addr2->contents[i])
	    return -1;
	else if ((unsigned char) addr1->contents[i] >
		 (unsigned char) addr2->contents[i])
	    return 1;
    }
    /* compared equal so far...which is longer? */
    return dir;
}
