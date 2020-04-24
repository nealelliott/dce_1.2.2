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
 * $Log: md5crypto.c,v $
 * Revision 1.1.2.1  1996/06/05  20:30:56  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  19:29:53  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
#include "k5-int.h"
#include "rsa-md5.h"
#include "des_int.h"	/* we cheat a bit and call it directly... */

krb5_error_code
md5_crypto_sum_func NPROTOTYPE((krb5_pointer in, size_t in_length,
    krb5_pointer seed, size_t seed_length, krb5_checksum *outcksum));

krb5_error_code
md5_crypto_sum_func(in, in_length, seed, seed_length, outcksum)
krb5_pointer in;
size_t in_length;
krb5_pointer seed;
size_t seed_length;
krb5_checksum FAR *outcksum;
{
    krb5_octet outtmp[RSA_MD5_DES_CKSUM_LENGTH];
    krb5_octet *input = (krb5_octet *)in;
    krb5_encrypt_block eblock;
    krb5_keyblock keyblock;
    krb5_error_code retval;

    MD5_CTX working;

    MD5Init(&working);
    MD5Update(&working, input, in_length);
    MD5Final(&working);

    outcksum->checksum_type = CKSUMTYPE_RSA_MD5_DES;
    outcksum->length = RSA_MD5_DES_CKSUM_LENGTH;

    memcpy((char *)outtmp, (char *)&working.digest[0], 16);

    memset((char *)&working, 0, sizeof(working));

    keyblock.length = seed_length;
    keyblock.contents = (krb5_octet *)seed;
    keyblock.keytype = KEYTYPE_DES;

    if (retval = mit_des_process_key(&eblock, &keyblock))
	return retval;
    /* now encrypt it */
    retval = mit_des_cbc_encrypt((mit_des_cblock *)&outtmp[0],
				 (mit_des_cblock *)outcksum->contents,
				 RSA_MD5_DES_CKSUM_LENGTH,
				 (struct mit_des_ks_struct *)eblock.priv,
				 keyblock.contents,
				 MIT_DES_ENCRYPT);
    if (retval) {
	(void) mit_des_finish_key(&eblock);
	return retval;
    }
    return mit_des_finish_key(&eblock);
}


krb5_checksum_entry rsa_md5_des_cksumtable_entry = {
    0,
    md5_crypto_sum_func,
    RSA_MD5_DES_CKSUM_LENGTH,
    1,					/* is collision proof */
    1,					/* uses key */
};
