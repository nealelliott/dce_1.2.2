/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: copy_athctr.c,v $
 * Revision 1.1.6.2  1996/02/18  00:13:26  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:03:10  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:57:30  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/06/12  12:46 UTC  mdf
 * 	When krb5_copy_keyblock() fails, there is no cleanup required
 * 	pertaining to the possible target of the copy.
 * 
 * 	OT12884
 * 	[1995/05/23  18:42 UTC  mdf  /main/mdf_mothra_defects_6/1]
 * 
 * Revision 1.1.2.2  1992/12/29  15:53:56  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:13:13  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:51:33  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  copy_athctr.c V=2 10/24/91 //littl/prgy/krb5/lib/krb
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/krb/copy_athctr.c,v $
 * $Author: marty $
 *
 * Copyright 1990,1991 by the Massachusetts Institute of Technology.
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
 * krb5_copy_authenticator()
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_copy_authenticator_c[] =
"$Id: copy_athctr.c,v 1.1.6.2 1996/02/18 00:13:26 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>

#include <krb5/ext-proto.h>

krb5_error_code
krb5_copy_authenticator(authfrom, authto)
const krb5_authenticator *authfrom;
krb5_authenticator **authto;
{
    krb5_error_code retval;
    krb5_authenticator *tempto;

    if (!(tempto = (krb5_authenticator *)malloc(sizeof(*tempto))))
	return ENOMEM;
    *tempto = *authfrom;

    if (retval = krb5_copy_principal(authfrom->client, &tempto->client)) {
	xfree(tempto);
	return retval;
    }
    
    if (authfrom->checksum &&
	(retval = krb5_copy_checksum(authfrom->checksum, &tempto->checksum))) {
	    krb5_free_principal(tempto->client);    
	    xfree(tempto);
	    return retval;
    }
    
    if (authfrom->subkey) {
	    if (retval = krb5_copy_keyblock(authfrom->subkey,
					    &tempto->subkey)) {
		    krb5_free_checksum(tempto->checksum);
		    krb5_free_principal(tempto->client);    
		    xfree(tempto);
		    return retval;
	    }
    }
    
    *authto = tempto;
    return 0;
}
