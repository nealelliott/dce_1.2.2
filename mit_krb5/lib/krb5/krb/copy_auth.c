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
 * $Log: copy_auth.c,v $
 * Revision 1.1.2.1  1996/06/05  20:58:35  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  19:56:04  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * lib/krb5/krb/copy_auth.c
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
 * krb5_copy_authdata()
 */

#include "k5-int.h"

static krb5_error_code
krb5_copy_authdatum(context, inad, outad)
    krb5_context context;
const krb5_authdata *inad;
krb5_authdata **outad;
{
    krb5_authdata *tmpad;

    if (!(tmpad = (krb5_authdata *)malloc(sizeof(*tmpad))))
	return ENOMEM;
#ifdef HAVE_C_STRUCTURE_ASSIGNMENT
    *tmpad = *inad;
#else
    memcpy(tmpad, inad, sizeof(krb5_authdata));
#endif
    if (!(tmpad->contents = (krb5_octet *)malloc(inad->length))) {
	krb5_xfree(tmpad);
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
krb5_copy_authdata(context, inauthdat, outauthdat)
    krb5_context context;
krb5_authdata * const * inauthdat;
krb5_authdata ***outauthdat;
{
    krb5_error_code retval;
    krb5_authdata ** tempauthdat;
    register int nelems = 0;

    if (!inauthdat) {
	    *outauthdat = 0;
	    return 0;
    }

    while (inauthdat[nelems]) nelems++;

    /* one more for a null terminated list */
    if (!(tempauthdat = (krb5_authdata **) calloc(nelems+1,
						  sizeof(*tempauthdat))))
	return ENOMEM;

    for (nelems = 0; inauthdat[nelems]; nelems++) {
	retval = krb5_copy_authdatum(context, inauthdat[nelems],
				     &tempauthdat[nelems]);
	if (retval) {
	    krb5_free_authdata(context, tempauthdat);
	    return retval;
	}
    }

    *outauthdat = tempauthdat;
    return 0;
}
