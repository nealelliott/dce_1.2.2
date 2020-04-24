/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: addr_srch.c,v $
 * Revision 1.1.6.2  1996/02/18  00:13:19  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:03:05  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:57:08  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:59:32  root]
 * 
 * Revision 1.1.2.2  1992/12/29  15:53:24  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:13:00  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:51:30  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  addr_srch.c V=2 10/24/91 //littl/prgy/krb5/lib/krb
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/krb/addr_srch.c,v $
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
 * krb5_address_search()
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_addr_srch_c[] =
"$Id: addr_srch.c,v 1.1.6.2 1996/02/18 00:13:19 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>

/*
 * if addr is listed in addrlist, or addrlist is null, return TRUE.
 * if not listed, return FALSE.
 * Currently avoid comparing ADDRTYPE_DCE addresses.
 */
krb5_boolean
krb5_address_search(addr, addrlist)
const krb5_address *addr;
krb5_address * const * addrlist;
{
    if (!addrlist)
	return TRUE;
#ifdef OSF_DCE
    if (addr->addrtype == ADDRTYPE_DCE)
        return TRUE;
#endif /* OSF_DCE */

    for (; *addrlist; addrlist++) {
	if (krb5_address_compare(addr, *addrlist))
	    return TRUE;
    }
    return FALSE;
}
