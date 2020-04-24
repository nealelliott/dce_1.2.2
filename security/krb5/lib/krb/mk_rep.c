/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: mk_rep.c,v $
 * Revision 1.1.6.2  1996/02/18  00:14:16  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:03:49  marty]
 *
 * Revision 1.1.6.1  1995/12/08  18:00:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:59:59  root]
 * 
 * Revision 1.1.2.2  1992/12/29  15:57:09  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:14:53  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:51:58  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  mk_rep.c V=3 10/24/91 //littl/prgy/krb5/lib/krb
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/krb/mk_rep.c,v $
 * $Author: marty $
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
 *
 * krb5_mk_rep()
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_mk_rep_c[] =
"$Id: mk_rep.c,v 1.1.6.2 1996/02/18 00:14:16 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/asn1.h>

#include <krb5/libos.h>
#include <krb5/los-proto.h>

#include <krb5/ext-proto.h>

/*
 Formats a KRB_AP_REP message into outbuf.

 The reply in repl is encrypted under the key in kblock, and the resulting
 message encoded and left in outbuf.

 The outbuf buffer storage is allocated, and should be freed by the
 caller when finished.

 returns system errors
*/

krb5_error_code
krb5_mk_rep(repl, kblock, outbuf)
const krb5_ap_rep_enc_part *repl;
const krb5_keyblock *kblock;
krb5_data *outbuf;
{
    krb5_error_code retval;
    krb5_data *scratch;
    krb5_ap_rep reply;
    krb5_enctype etype;
    krb5_encrypt_block eblock;
    krb5_data *toutbuf;

    /* verify a valid etype is available */
    if (!valid_keytype(kblock->keytype))
	return KRB5_PROG_KEYTYPE_NOSUPP;

    etype = krb5_keytype_array[kblock->keytype]->system->proto_enctype;

    if (!valid_etype(etype))
	return KRB5_PROG_ETYPE_NOSUPP;

    /* encode it before encrypting */
    if (retval = encode_krb5_ap_rep_enc_part(repl, &scratch))
	return retval;

#define cleanup_scratch() { (void) memset(scratch->data, 0, scratch->length); \
krb5_free_data(scratch); }

    /* put together an eblock for this encryption */

    krb5_use_cstype(&eblock, etype);
    reply.enc_part.etype = etype;
    reply.enc_part.kvno = 0;		/* XXX user set? */

    reply.enc_part.ciphertext.length = krb5_encrypt_size(scratch->length,
							 eblock.crypto_entry);
    /* add padding area, and zero it */
    if (!(scratch->data = realloc(scratch->data,
				  reply.enc_part.ciphertext.length))) {
	/* may destroy scratch->data */
	xfree(scratch);
	return ENOMEM;
    }
    memset(scratch->data + scratch->length, 0,
	  reply.enc_part.ciphertext.length - scratch->length);
    if (!(reply.enc_part.ciphertext.data =
	  malloc(reply.enc_part.ciphertext.length))) {
	retval = ENOMEM;
	goto clean_scratch;
    }

#define cleanup_encpart() {\
(void) memset(reply.enc_part.ciphertext.data, 0,\
	     reply.enc_part.ciphertext.length); \
free(reply.enc_part.ciphertext.data); \
reply.enc_part.ciphertext.length = 0; reply.enc_part.ciphertext.data = 0;}

    /* do any necessary key pre-processing */
    if (retval = krb5_process_key(&eblock, kblock)) {
	goto clean_encpart;
    }

#define cleanup_prockey() {(void) krb5_finish_key(&eblock);}

    /* call the encryption routine */
    if (retval = krb5_encrypt((krb5_pointer) scratch->data,
			      (krb5_pointer) reply.enc_part.ciphertext.data,
			      scratch->length, &eblock, 0)) {
	goto clean_prockey;
    }

    /* encrypted part now assembled-- do some cleanup */
    cleanup_scratch();

    if (retval = krb5_finish_key(&eblock)) {
	cleanup_encpart();
	return retval;
    }

    if (!(retval = encode_krb5_ap_rep(&reply, &toutbuf))) {
	*outbuf = *toutbuf;
	xfree(toutbuf);
    }
    cleanup_encpart();
    return retval;

 clean_prockey:
    cleanup_prockey();
 clean_encpart:
    cleanup_encpart();
 clean_scratch:
    cleanup_scratch();

    return retval;
}
