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
 * $Log: encrypt_key.c,v $
 * Revision 1.1.2.1  1996/06/05  20:31:29  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  19:35:36  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * lib/kdb/encrypt_key.c
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
 * krb5_kdb_encrypt_key(), krb5_kdb_decrypt_key functions
 */

#include "k5-int.h"

/*
 * Encrypt a key for storage in the database.  "eblock" is used
 * to encrypt the key in "in" into "out"; the storage pointed to by "out"
 * is allocated before use.
 */

krb5_error_code
krb5_kdb_encrypt_key(context, eblock, in, out)
    krb5_context context;
    krb5_encrypt_block *eblock;
    const krb5_keyblock *in;
    register krb5_encrypted_keyblock *out;
{
    /* Encrypted rep has the real (unencrypted) key length stored
       along with the encrypted key.  The length is stored as a 4
       byte integer, MSB first.  */

    krb5_error_code retval;
    krb5_keyblock tmpin;
    unsigned int length;

    out->keytype = in->keytype;
    out->length = krb5_encrypt_size(in->length, eblock->crypto_entry);

    /* because of checksum space requirements imposed by the encryption
       interface, we need to copy the input key into a larger area. */
    tmpin.length = in->length;
    tmpin.contents = (krb5_octet *)malloc(out->length);
    if (!tmpin.contents) {
	out->length = 0;
	return ENOMEM;
    }
    memcpy((char *)tmpin.contents, (const char *)in->contents, tmpin.length);

    out->length += sizeof(out->length);
    out->contents = (krb5_octet *)malloc(out->length);
    if (!out->contents) {
	krb5_xfree(tmpin.contents);
	out->contents = 0;
	out->length = 0;
	return ENOMEM;
    }

    length = tmpin.length;
    ((char *)out->contents)[0] = length >> 24;
    ((char *)out->contents)[1] = length >> 16;
    ((char *)out->contents)[2] = length >> 8;
    ((char *)out->contents)[3] = length;
    
    retval = krb5_encrypt(context, (krb5_pointer) tmpin.contents,
			  (krb5_pointer) ((char *) out->contents + 4),
			  tmpin.length, eblock, 0);
    krb5_xfree(tmpin.contents);
    if (retval) {
	krb5_xfree(out->contents);
	out->contents = 0;
	out->length = 0;
    }

    return retval;
}
