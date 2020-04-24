/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: copy_auth.c,v $
 * Revision 1.1.6.2  1996/02/18  00:13:28  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:03:12  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:57:35  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:59:36  root]
 * 
 * Revision 1.1.2.2  1992/12/29  15:54:03  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:13:16  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:51:34  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  copy_auth.c V=3 10/24/91 //littl/prgy/krb5/lib/krb
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/krb/copy_auth.c,v $
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
 * krb5_copy_authdata()
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_copy_auth_c[] =
"$Id: copy_auth.c,v 1.1.6.2 1996/02/18 00:13:28 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>

#include <krb5/ext-proto.h>

static krb5_error_code
krb5_copy_authdatum(inad, outad)
const krb5_authdata *inad;
krb5_authdata **outad;
{
    krb5_authdata *tmpad;

    if (!(tmpad = (krb5_authdata *)malloc(sizeof(*tmpad))))
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
 * Copy an authdata array, with fresh allocation.
 */
krb5_error_code
krb5_copy_authdata(inauthdat, outauthdat)
krb5_authdata * const * inauthdat;
krb5_authdata ***outauthdat;
{
    krb5_error_code retval;
    krb5_authdata ** tempauthdat;
    register int nelems;

    for (nelems = 0; inauthdat[nelems]; nelems++);

    /* one more for a null terminated list */
    if (!(tempauthdat = (krb5_authdata **) calloc(nelems+1,
						  sizeof(*tempauthdat))))
	return ENOMEM;

    for (nelems = 0; inauthdat[nelems]; nelems++)
	if (retval = krb5_copy_authdatum(inauthdat[nelems],
					 &tempauthdat[nelems])) {
	    krb5_free_authdata(tempauthdat);
	    return retval;
	}

    *outauthdat = tempauthdat;
    return 0;
}
