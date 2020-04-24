/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: random_key.c,v $
 * Revision 1.1.8.1  1996/06/04  22:02:58  arvind
 * 	Merge krb5 work to DCE1.2.2 mainline
 * 	[1996/05/29  20:31 UTC  mullan_s  /main/mullan_dce1.2.2_krb5_drop/1]
 *
 * 	If these sources are built with -DMIT_KRB5 compiler define, adjust
 * 	routines and structures to build with include files in
 * 	/dce/src/mit_krb5/include.
 * 	[1992/12/29  14:22:43  zeliff  1.1.4.2]
 *
 * Revision 1.1.6.2  1996/02/18  00:10:18  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:01:40  marty]
 * 
 * Revision 1.1.6.1  1995/12/08  17:49:02  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:58:17  root]
 * 
 * Revision 1.1.4.2  1992/12/29  14:22:43  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:04:40  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/15  21:00:47  pato
 * 	Initial revision
 * 	[1992/05/14  15:26:43  pato]
 * 
 * $EndLog$
 */

/*
** Copyright (c) Hewlett-Packard Company 1991, 1992
** Unpublished work. All Rights Reserved.
*/

/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/crypto/common/random_key.c,v $
 * $Author: arvind $
 *
 * Derived from:
 * Source: /afs/athena.mit.edu/astaff/project/krb5/src/lib/des/RCS/random_key.c,v
 * Author: jtkohl
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
 */

#ifdef MODULE_VERSION_ID
static char rcsid_random_key_c[] =
"$Id: random_key.c,v 1.1.8.1 1996/06/04 22:02:58 arvind Exp $";
#endif

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>

#include <krb5/des_int.h>
/*
        generate a random encryption key, allocating storage for it and
        filling in the keyblock address in *keyblock
 */

#ifdef MIT_KRB5
krb5_error_code mit_des_random_key (DECLARG(const krb5_encrypt_block *, eblock),
				    DECLARG(krb5_pointer, seed),
				    DECLARG(krb5_keyblock **, keyblock))
OLDDECLARG(const krb5_encrypt_block *, eblock)
OLDDECLARG(krb5_pointer, seed)
OLDDECLARG(krb5_keyblock **, keyblock)
#else
krb5_error_code mit_des_random_key (DECLARG(krb5_pointer, seed),
				    DECLARG(krb5_keyblock **, keyblock))
OLDDECLARG(krb5_pointer, seed)
OLDDECLARG(krb5_keyblock **, keyblock)
#endif
{
    krb5_keyblock *randkey;

    if (!(randkey = (krb5_keyblock *)malloc(sizeof(*randkey))))
	return ENOMEM;
    if (!(randkey->contents = (krb5_octet *)malloc(sizeof(mit_des_cblock)))) {
	xfree(randkey);
	return ENOMEM;
    }
#ifdef MIT_KRB5
    randkey->magic = KV5M_KEYBLOCK;
    randkey->etype = eblock->crypto_entry->proto_enctype;
#endif
    randkey->length = sizeof(mit_des_cblock);
    randkey->keytype = KEYTYPE_DES;
    mit_des_new_random_key(randkey->contents, (mit_des_random_key_seed *) seed);
    *keyblock = randkey;
    return 0;
}
