/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: encrypt_tk.c,v $
 * Revision 1.1.6.2  1996/02/18  00:13:46  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:03:29  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:58:42  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:59:45  root]
 * 
 * Revision 1.1.2.2  1992/12/29  15:55:17  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:13:53  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:51:44  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  encrypt_tk.c V=5 10/24/91 //littl/prgy/krb5/lib/krb
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/krb/encrypt_tk.c,v $
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
 * krb5_encrypt_tkt_part() routine.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_encrypt_tk_c[] =
"$Id: encrypt_tk.c,v 1.1.6.2 1996/02/18 00:13:46 marty Exp $";
#endif	/* !lint & !SABER */


#include <krb5/krb5.h>
#include <krb5/asn1.h>

#include <krb5/ext-proto.h>

/*
 Takes unencrypted dec_ticket & dec_tkt_part, encrypts with
 dec_ticket->enc_part.etype
 using *srv_key, and places result in dec_ticket->enc_part.
 The string dec_ticket->enc_part.ciphertext will be allocated before
 formatting.

 returns errors from encryption routines, system errors

 enc_part->ciphertext.data allocated & filled in with encrypted stuff
*/

krb5_error_code
krb5_encrypt_tkt_part(srv_key, dec_ticket)
const krb5_keyblock *srv_key;
register krb5_ticket *dec_ticket;
{
    krb5_data *scratch;
    krb5_error_code retval;
    krb5_encrypt_block eblock;
    register krb5_enc_tkt_part *dec_tkt_part = dec_ticket->enc_part2;

    /* encrypt the encrypted part */

    if (!valid_etype(dec_ticket->enc_part.etype))
	return KRB5_PROG_ETYPE_NOSUPP;

    /*  start by encoding the to-be-encrypted part. */
    if (retval = encode_krb5_enc_tkt_part(dec_tkt_part, &scratch)) {
	return retval;
    }

#define cleanup_scratch() { (void) memset(scratch->data, 0, scratch->length); \
krb5_free_data(scratch); }

    /* put together an eblock for this encryption */

    krb5_use_cstype(&eblock, dec_ticket->enc_part.etype);
    dec_ticket->enc_part.ciphertext.length =
	krb5_encrypt_size(scratch->length,
			  eblock.crypto_entry);
    /* add padding area, and zero it */
    if (!(scratch->data = realloc(scratch->data,
				  dec_ticket->enc_part.ciphertext.length))) {
	/* may destroy scratch->data */
	xfree(scratch);
	return ENOMEM;
    }
    memset(scratch->data + scratch->length, 0,
	  dec_ticket->enc_part.ciphertext.length - scratch->length);
    if (!(dec_ticket->enc_part.ciphertext.data =
	  malloc(dec_ticket->enc_part.ciphertext.length))) {
	retval = ENOMEM;
	goto clean_scratch;
    }

#define cleanup_encpart() {\
(void) memset(dec_ticket->enc_part.ciphertext.data, 0,\
	     dec_ticket->enc_part.ciphertext.length); \
free(dec_ticket->enc_part.ciphertext.data); \
dec_ticket->enc_part.ciphertext.length = 0; \
dec_ticket->enc_part.ciphertext.data = 0;}

    /* do any necessary key pre-processing */
    if (retval = krb5_process_key(&eblock, srv_key)) {
	goto clean_encpart;
    }

#define cleanup_prockey() {(void) krb5_finish_key(&eblock);}

    /* call the encryption routine */
    if (retval = krb5_encrypt((krb5_pointer) scratch->data,
			      (krb5_pointer) dec_ticket->enc_part.ciphertext.data,
			      scratch->length, &eblock, 0)) {
	goto clean_prockey;
    }

    /* ticket is now assembled-- do some cleanup */
    cleanup_scratch();

    if (retval = krb5_finish_key(&eblock)) {
	cleanup_encpart();
	return retval;
    }

    return 0;

 clean_prockey:
    cleanup_prockey();
 clean_encpart:
    cleanup_encpart();
 clean_scratch:
    cleanup_scratch();

    return retval;
}
