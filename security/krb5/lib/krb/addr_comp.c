/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: addr_comp.c,v $
 * Revision 1.1.6.2  1996/02/18  00:13:14  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:03:01  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:56:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:59:30  root]
 * 
 * Revision 1.1.2.2  1992/12/29  15:53:13  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:12:53  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:51:28  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  addr_comp.c V=3 10/24/91 //littl/prgy/krb5/lib/krb
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/krb/addr_comp.c,v $
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
 * krb5_address_compare()
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_addr_comp_c[] =
"$Id: addr_comp.c,v 1.1.6.2 1996/02/18 00:13:14 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>

/*
 * If the two addresses are the same, return TRUE, else return FALSE
 */
krb5_boolean
krb5_address_compare(addr1, addr2)
const krb5_address *addr1;
const krb5_address *addr2;
{
    if (addr1->addrtype != addr2->addrtype)
	return(FALSE);

    if (addr1->length != addr2->length)
	return(FALSE);
    if (memcmp((char *)addr1->contents, (char *)addr2->contents,
	       addr1->length))
	return FALSE;
    else
	return TRUE;
}
