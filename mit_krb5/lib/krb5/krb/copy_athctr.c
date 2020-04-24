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
 * $Log: copy_athctr.c,v $
 * Revision 1.1.2.1  1996/06/05  20:58:23  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  19:55:57  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * lib/krb5/krb/copy_athctr.c
 *
 * Copyright 1990,1991 by the Massachusetts Institute of Technology.
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
 * krb5_copy_authenticator()
 */

#include "k5-int.h"

krb5_error_code
krb5_copy_authenticator(context, authfrom, authto)
    krb5_context context;
const krb5_authenticator *authfrom;
krb5_authenticator **authto;
{
    krb5_error_code retval;
    krb5_authenticator *tempto;

    if (!(tempto = (krb5_authenticator *)malloc(sizeof(*tempto))))
	return ENOMEM;
#ifdef HAVE_C_STRUCTURE_ASSIGNMENT
    *tempto = *authfrom;
#else
    memcpy(tempto, authfrom, sizeof(krb5_authenticator));
#endif

    retval = krb5_copy_principal(context, authfrom->client, &tempto->client);
    if (retval) {
	krb5_xfree(tempto);
	return retval;
    }
    
    if (authfrom->checksum &&
	(retval = krb5_copy_checksum(context, authfrom->checksum, &tempto->checksum))) {
	    krb5_free_principal(context, tempto->client);    
	    krb5_xfree(tempto);
	    return retval;
    }
    
    if (authfrom->subkey) {
	    retval = krb5_copy_keyblock(context, authfrom->subkey, &tempto->subkey);
	    if (retval) {
		    krb5_xfree(tempto->subkey);
		    krb5_free_checksum(context, tempto->checksum);
		    krb5_free_principal(context, tempto->client);    
		    krb5_xfree(tempto);
		    return retval;
	    }
    }
    
    if (authfrom->authorization_data) {
		retval = krb5_copy_authdata(context, authfrom->authorization_data,
				    &tempto->authorization_data);
		if (retval) {
		    krb5_xfree(tempto->subkey);
		    krb5_free_checksum(context, tempto->checksum);
		    krb5_free_principal(context, tempto->client);    
		    krb5_free_authdata(context, tempto->authorization_data);
		    krb5_xfree(tempto);
		    return retval;
		}
    }

    *authto = tempto;
    return 0;
}
