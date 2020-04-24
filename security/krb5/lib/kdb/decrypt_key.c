/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: decrypt_key.c,v $
 * Revision 1.1.6.2  1996/02/18  00:12:19  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:02:28  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:53:46  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:59:07  root]
 * 
 * Revision 1.1.2.2  1992/12/29  14:29:20  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:09:51  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:50:47  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  decrypt_key.c V=4 10/24/91 //littl/prgy/krb5/lib/kdb
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/kdb/decrypt_key.c,v $
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
 * krb5_kdb_encrypt_key(), krb5_kdb_decrypt_key functions
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_decrypt_key_c [] =
"$Id: decrypt_key.c,v 1.1.6.2 1996/02/18 00:12:19 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>
#include <krb5/kdb.h>


/*
 * Decrypt a key from storage in the database.  "eblock" is used
 * to decrypt the key in "in" into "out"; the storage pointed to by "out"
 * is allocated before use.
 */

krb5_error_code
krb5_kdb_decrypt_key(eblock, in, out)
krb5_encrypt_block *eblock;
const krb5_encrypted_keyblock *in;
krb5_keyblock *out;
{
#ifndef OSF_DCE
    krb5_error_code retval;

    /* the encrypted version is stored as the unencrypted key length
       (in host byte order), followed by the encrypted key.
     */
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
    memcpy((char *)&out->length, (char *)in->contents, sizeof(out->length));

    /* remember the contents of the encrypted version has a sizeof(in->length)
       integer length of the real embedded key, followed by the
       encrypted key, so the offset here is needed */
    if (retval = krb5_decrypt((krb5_pointer) (((char *) in->contents) +
					      sizeof(in->length)),
			      (krb5_pointer) out->contents,
			      in->length-sizeof(in->length), eblock, 0)) {
	xfree(out->contents);
	out->contents = 0;
	out->length = 0;
	return retval;
    }
    if (out->length < 0) {
	xfree(out->contents);
	out->contents = 0;
	out->length = 0;
	return KRB5_KDB_INVALIDKEYSIZE;
    }
    return retval;
#else
    int len = in->length;
    out->keytype = in->keytype;
    out->contents = malloc (len);
    out->length = len;
    memcpy (out->contents, in->contents, len);
    return 0;
#endif
    
}
