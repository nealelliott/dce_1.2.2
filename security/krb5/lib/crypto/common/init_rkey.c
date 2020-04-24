/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: init_rkey.c,v $
 * Revision 1.1.6.2  1996/02/18  00:10:04  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:01:37  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:48:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:58:12  root]
 * 
 * Revision 1.1.4.2  1992/12/29  14:22:16  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:04:17  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/15  20:58:58  pato
 * 	Initial revision
 * 	[1992/05/14  15:24:16  pato]
 * 
 * $EndLog$
 */

/*
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
*/

/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/crypto/common/init_rkey.c,v $
 * $Author: marty $
 *
 * Derived from:
 * Source: /afs/athena.mit.edu/astaff/project/krb5/src/lib/des/RCS/init_rkey.c,v
 * Author: jtkohl
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
 */

#ifdef MODULE_VERSION_ID
static char rcsid_init_rkey_c[] =
"$Id: init_rkey.c,v 1.1.6.2 1996/02/18 00:10:04 marty Exp $";
#endif

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>

#include <krb5/des_int.h>

/*
        initialize the random key generator using the encryption key,
        "seedblock", and allocating private sequence information, filling
        in "seed" with the address of such information.
        "seed" is later passed to the random_key() function to provide
        sequence information.
 */

krb5_error_code mit_des_init_random_key (DECLARG(const krb5_keyblock *,seedblock),
					 DECLARG(krb5_pointer *,seed))
OLDDECLARG(const krb5_keyblock *,seedblock)
OLDDECLARG(krb5_pointer *,seed)
{
    mit_des_random_key_seed * p_seed;
    if (seedblock->keytype != KEYTYPE_DES)
	return KRB5_BAD_KEYTYPE;
    if ( !(p_seed = (mit_des_random_key_seed *) 
	   malloc(sizeof(mit_des_random_key_seed))) ) 
	return ENOMEM;
    memset((char *)p_seed, 0, sizeof(mit_des_random_key_seed) );
    mit_des_init_random_number_generator(seedblock->contents, p_seed);
    *seed = (krb5_pointer) p_seed;
    return 0;
}
