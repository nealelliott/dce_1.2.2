/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: encode_kdc.c,v $
 * Revision 1.1.8.1  1996/10/03  15:02:17  arvind
 * 	flush `register' to avoid type mismatch warning from compiler.
 * 	[1996/09/16  21:02 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 *
 * Revision 1.1.6.2  1996/02/18  00:13:45  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:03:27  marty]
 * 
 * Revision 1.1.6.1  1995/12/08  17:58:36  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:59:44  root]
 * 
 * Revision 1.1.2.2  1992/12/29  15:55:09  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:13:50  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:51:43  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  encode_kdc.c V=5 10/24/91 //littl/prgy/krb5/lib/krb
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/krb/encode_kdc.c,v $
 * $Author: arvind $
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
 * krb5_encode_kdc_rep() function.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_encode_kdc_c [] =
"$Id: encode_kdc.c,v 1.1.8.1 1996/10/03 15:02:17 arvind Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/asn1.h>

#include <krb5/ext-proto.h>

/*
 Takes KDC rep parts in *rep and *encpart, and formats it into *enc_rep,
 using message type type and encryption key client_key and encryption type
 etype.

 The string *enc_rep will be allocated before formatting; the caller should
 free when finished.

 returns system errors

 dec_rep->enc_part.ciphertext is allocated and filled in.
*/
/* due to argument promotion rules, we need to use the DECLARG/OLDDECLARG
   stuff... */
krb5_error_code
krb5_encode_kdc_rep(DECLARG(const krb5_msgtype, type),
		    DECLARG(const krb5_enc_kdc_rep_part *, encpart),
		    DECLARG(const krb5_keyblock *, client_key),
		    DECLARG(register krb5_kdc_rep *, dec_rep),
		    DECLARG(krb5_data **, enc_rep))
OLDDECLARG(const krb5_msgtype, type)
OLDDECLARG(const krb5_enc_kdc_rep_part *, encpart)
OLDDECLARG(const krb5_keyblock *, client_key)
OLDDECLARG(register krb5_kdc_rep *, dec_rep)
OLDDECLARG(krb5_data **, enc_rep)
{
    krb5_data *scratch;
    krb5_encrypt_block eblock;
    krb5_error_code retval;

    if (!valid_etype(dec_rep->enc_part.etype))
	return KRB5_PROG_ETYPE_NOSUPP;

    switch (type) {
    case KRB5_AS_REP:
    case KRB5_TGS_REP:
	break;
    default:
	return KRB5_BADMSGTYPE;
    }

    if (retval = encode_krb5_enc_kdc_rep_part(encpart, &scratch)) {
	return retval;
    }

#define cleanup_scratch() { (void) memset(scratch->data, 0, scratch->length); \
krb5_free_data(scratch); }

    /* put together an eblock for this encryption */

    krb5_use_cstype(&eblock, dec_rep->enc_part.etype);
    dec_rep->enc_part.ciphertext.length =
	krb5_encrypt_size(scratch->length, eblock.crypto_entry);
    /* add padding area, and zero it */
    if (!(scratch->data = realloc(scratch->data,
				  dec_rep->enc_part.ciphertext.length))) {
	/* may destroy scratch->data */
	xfree(scratch);
	return ENOMEM;
    }
    memset(scratch->data + scratch->length, 0,
	  dec_rep->enc_part.ciphertext.length - scratch->length);
    if (!(dec_rep->enc_part.ciphertext.data =
	  malloc(dec_rep->enc_part.ciphertext.length))) {
	retval = ENOMEM;
	goto clean_scratch;
    }

#define cleanup_encpart() { \
(void) memset(dec_rep->enc_part.ciphertext.data, 0, \
	     dec_rep->enc_part.ciphertext.length); \
free(dec_rep->enc_part.ciphertext.data); \
dec_rep->enc_part.ciphertext.length = 0; \
dec_rep->enc_part.ciphertext.data = 0;}

    if (retval = krb5_process_key(&eblock, client_key)) {
	goto clean_encpart;
    }

#define cleanup_prockey() {(void) krb5_finish_key(&eblock);}

    if (retval = krb5_encrypt((krb5_pointer) scratch->data,
			      (krb5_pointer) dec_rep->enc_part.ciphertext.data,
			      scratch->length, &eblock, 0)) {
	goto clean_prockey;
    }

    /* do some cleanup */
    cleanup_scratch();

    if (retval = krb5_finish_key(&eblock)) {
	cleanup_encpart();
	return retval;
    }

    /* now it's ready to be encoded for the wire! */

    switch (type) {
    case KRB5_AS_REP:
	retval = encode_krb5_as_rep(dec_rep, enc_rep);
	break;
    case KRB5_TGS_REP:
	retval = encode_krb5_tgs_rep(dec_rep, enc_rep);
	break;
    }
    if (retval)
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
