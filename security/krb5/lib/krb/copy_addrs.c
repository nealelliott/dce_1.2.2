/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: copy_addrs.c,v $
 * Revision 1.1.6.2  1996/02/18  00:13:24  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:03:09  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:57:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:59:34  root]
 * 
 * Revision 1.1.2.2  1992/12/29  15:53:51  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:13:10  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:51:31  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  copy_addrs.c V=3 10/24/91 //littl/prgy/krb5/lib/krb
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/krb/copy_addrs.c,v $
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
 * krb5_copy_addresses()
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_copy_addrs_c[] =
"$Id: copy_addrs.c,v 1.1.6.2 1996/02/18 00:13:24 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>

#include <krb5/ext-proto.h>

static krb5_error_code
krb5_copy_addr(inad, outad)
const krb5_address *inad;
krb5_address **outad;
{
    krb5_address *tmpad;

    if (!(tmpad = (krb5_address *)malloc(sizeof(*tmpad))))
	return ENOMEM;
    *tmpad = *inad;
    if (!(tmpad->contents = (krb5_octet *)malloc(inad->length))) {
	xfree(tmpad);
	return ENOMEM;
    }
    memcpy((char *)tmpad->contents, (char *)inad->contents, inad->length);
    *outad = tmpad;
    return 0;
}

/*
 * Copy an address array, with fresh allocation.
 */
krb5_error_code
krb5_copy_addresses(inaddr, outaddr)
krb5_address * const * inaddr;
krb5_address ***outaddr;
{
    krb5_error_code retval;
    krb5_address ** tempaddr;
    register int nelems;

    if (!inaddr) {
        *outaddr = NULL;
        return 0;
    }

    for (nelems = 0; inaddr[nelems]; nelems++);

    /* one more for a null terminated list */
    if (!(tempaddr = (krb5_address **) calloc(nelems+1, sizeof(*tempaddr))))
	return ENOMEM;

    for (nelems = 0; inaddr[nelems]; nelems++)
	if (retval = krb5_copy_addr(inaddr[nelems], &tempaddr[nelems])) {
	    krb5_free_addresses(tempaddr);
	    return retval;
	}

    *outaddr = tempaddr;
    return 0;
}
