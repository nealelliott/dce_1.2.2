/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: gen_subkey.c,v $
 * Revision 1.1.6.2  1996/02/18  00:13:58  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:03:38  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:59:18  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/07/12  20:02 UTC  aha
 * 	CHFts15537: close memory leak in krb5_generate_subkey()
 * 
 * 	HP revision /main/aha_mothra7/1  1995/07/10  19:19 UTC  aha
 * 	CHFts15537: fix memory leak in krb5_generate_subkey()
 * 	[1995/12/08  16:59:50  root]
 * 
 * Revision 1.1.2.2  1992/12/29  15:56:01  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:14:18  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:51:26  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  gen_subkey.c V=1 06/06/91 //littl/prgy/krb5/lib/krb
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/krb/gen_subkey.c,v $
 * $Author: marty $
 *
 * Copyright 1991 by the Massachusetts Institute of Technology.
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
 * Routine to automatically generate a subsession key based on an input key.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_gen_subkey_c[] =
"$Id: gen_subkey.c,v 1.1.6.2 1996/02/18 00:13:58 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>

krb5_error_code
krb5_generate_subkey(key, subkey)
const krb5_keyblock *key;
krb5_keyblock **subkey;
{
    krb5_pointer random_state;
    krb5_encrypt_block eblock;
    krb5_error_code retval;

    if (!valid_keytype(key->keytype))
	return KRB5_PROG_KEYTYPE_NOSUPP;

    krb5_use_keytype(&eblock, key->keytype);

    if (retval = krb5_init_random_key(&eblock, key, &random_state))
	return(retval);

    /*
     * CHFts15537: remove malloc of memory for *subkey because
     * krb5_random_key() does its own malloc
     */

    if (retval = krb5_random_key(&eblock, random_state, subkey)) {
	(void) krb5_finish_random_key(&eblock, random_state);

        /*
         * CHFts15537: remove free of memory for *subkey because
         * krb5_random_key() does its own free in case of error
         */

	return retval;
    }	
    /* ignore the error if any, since we've already gotten the key out */
    (void) krb5_finish_random_key(&eblock, &random_state);
    return 0;
}

