/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: md4crypto.c,v $
 * Revision 1.1.7.2  1996/02/18  00:16:09  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:05:20  marty]
 *
 * Revision 1.1.7.1  1995/12/08  17:45:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:18:55  root]
 * 
 * Revision 1.1.5.2  1992/12/29  16:32:33  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:20:22  zeliff]
 * 
 * Revision 1.1.3.2  1992/06/01  20:53:37  sekhar
 * 	Drop the des code restructuring changes made by Joe Pato.
 * 	[1992/06/01  20:03:08  sekhar]
 * 
 * Revision 1.1.1.2  1992/05/15  21:38:14  pato
 * 	OT#2330 des_int.h moved to krb5/
 * 
 * Revision 1.1  1992/01/19  14:53:41  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
** Copyright (c) Hewlett-Packard Company 1991, 1992
** Unpublished work. All Rights Reserved.
*/

/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/rsa-md4/md4crypto.c,v $
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
 * Kerberos glue for MD4 sample implementation.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_md4crypto_c[] =
"$Id: md4crypto.c,v 1.1.7.2 1996/02/18 00:16:09 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>
#include <krb5/rsa-md4.h>

#ifdef OSF_DCE
#include <krb5/des_int.h>
#else
#include "../des/des_int.h"	/* we cheat a bit and call it directly... */
#endif

static krb5_error_code
md4_crypto_sum_func(in, in_length, seed, seed_length, outcksum)
krb5_pointer in;
size_t in_length;
krb5_pointer seed;
size_t seed_length;
krb5_checksum *outcksum;
{
    krb5_octet outtmp[RSA_MD4_DES_CKSUM_LENGTH];
    krb5_octet *input = (krb5_octet *)in;
    krb5_encrypt_block eblock;
    krb5_keyblock keyblock;
    krb5_error_code retval;

    MD4_CTX working;

    MD4Init(&working);
    MD4Update(&working, input, in_length);
    MD4Final(&working);

    outcksum->checksum_type = CKSUMTYPE_RSA_MD4_DES;
    outcksum->length = RSA_MD4_DES_CKSUM_LENGTH;

    memcpy((char *)outtmp, (char *)&working.digest[0], 16);

    memset((char *)&working, 0, sizeof(working));

    keyblock.length = seed_length;
    keyblock.contents = (krb5_octet *)seed;
    keyblock.keytype = KEYTYPE_DES;

    if (retval = mit_des_process_key(&eblock, &keyblock))
	return retval;
    /* now encrypt it */
    retval = mit_des_cbc_encrypt(&outtmp[0],
				 outcksum->contents,
				 RSA_MD4_DES_CKSUM_LENGTH,
				 (struct mit_des_ks_struct *)eblock.priv,
				 keyblock.contents,
				 MIT_DES_ENCRYPT);
    if (retval) {
	(void) mit_des_finish_key(&eblock);
	return retval;
    }
    return mit_des_finish_key(&eblock);
}


krb5_checksum_entry rsa_md4_des_cksumtable_entry = {
    md4_crypto_sum_func,
    RSA_MD4_DES_CKSUM_LENGTH,
    1,					/* is collision proof */
    1,					/* uses key */
};
