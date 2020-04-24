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
 * $Log: decrypt_key.c,v $
 * Revision 1.1.2.1  1996/06/12  15:29:23  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/12  15:28:55  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * lib/kdb/decrypt_key.c
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
 * Decrypt a key from storage in the database.  "eblock" is used
 * to decrypt the key in "in" into "out"; the storage pointed to by "out"
 * is allocated before use.
 */

krb5_error_code
krb5_kdb_decrypt_key(context, eblock, in, out)
    krb5_context context;
    krb5_encrypt_block *eblock;
    const krb5_encrypted_keyblock *in;
    krb5_keyblock *out;
{
    krb5_error_code retval;

    /* the encrypted version is stored as the unencrypted key length
       (4 bytes, MSB first) followed by the encrypted key. */
    out->magic = KV5M_KEYBLOCK;
    out->etype = ETYPE_UNKNOWN;
    out->keytype = in->keytype;
    out->length = krb5_encrypt_size(in->length-sizeof(in->length),
				    eblock->crypto_entry);
    out->contents = (krb5_octet *)malloc(out->length);
    if (!out->contents) {
	out->contents = 0;
	out->length = 0;
	return ENOMEM;
    }

    /* copy out the real length count */
    out->length  = (((unsigned char *)in->contents)[0] << 24)
		 + (((unsigned char *)in->contents)[1] << 16)
		 + (((unsigned char *)in->contents)[2] << 8)
		 +  ((unsigned char *)in->contents)[3];

    /* remember the contents of the encrypted version has a 4 byte
       integer length of the real embedded key, followed by the
       encrypted key, so the offset here is needed */
    if (retval = krb5_decrypt(context, (krb5_pointer) (
			      (char *) in->contents + 4),
			      (krb5_pointer) out->contents,
			      in->length-sizeof(in->length), eblock, 0)) {
	krb5_xfree(out->contents);
	out->contents = 0;
	out->length = 0;
	return retval;
    }
    if (out->length < 0) {
	krb5_xfree(out->contents);
	out->contents = 0;
	out->length = 0;
	return KRB5_KDB_INVALIDKEYSIZE;
    }
    return retval;
}
