/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: decrypt_tk.c,v $
 * Revision 1.1.6.2  1996/02/18  00:13:43  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:03:26  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:58:29  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:59:43  root]
 * 
 * Revision 1.1.2.2  1992/12/29  15:55:03  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:13:47  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:51:42  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  decrypt_tk.c V=5 10/24/91 //littl/prgy/krb5/lib/krb
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/krb/decrypt_tk.c,v $
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
 * krb5_decrypt_tkt_part() function.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_decrypt_tk_c[] =
"$Id: decrypt_tk.c,v 1.1.6.2 1996/02/18 00:13:43 marty Exp $";
#endif	/* !lint & !SABER */


#include <krb5/krb5.h>
#include <krb5/asn1.h>

#include <krb5/ext-proto.h>

/*
 Decrypts dec_ticket->enc_part
 using *srv_key, and places result in dec_ticket->enc_part2.
 The storage of dec_ticket->enc_part2 will be allocated before return.

 returns errors from encryption routines, system errors

*/

krb5_error_code
krb5_decrypt_tkt_part(srv_key, ticket)
const krb5_keyblock *srv_key;
register krb5_ticket *ticket;
{
    krb5_enc_tkt_part *dec_tkt_part;
    krb5_encrypt_block eblock;
    krb5_data scratch;
    krb5_error_code retval;

    if (!valid_etype(ticket->enc_part.etype))
	return KRB5_PROG_ETYPE_NOSUPP;

    /* put together an eblock for this encryption */

    krb5_use_cstype(&eblock, ticket->enc_part.etype);

    scratch.length = ticket->enc_part.ciphertext.length;
    if (!(scratch.data = malloc(ticket->enc_part.ciphertext.length)))
	return(ENOMEM);

    /* do any necessary key pre-processing */
    if (retval = krb5_process_key(&eblock, srv_key)) {
	free(scratch.data);
	return(retval);
    }

    /* call the encryption routine */
    if (retval = krb5_decrypt((krb5_pointer) ticket->enc_part.ciphertext.data,
			      (krb5_pointer) scratch.data,
			      scratch.length, &eblock, 0)) {
	(void) krb5_finish_key(&eblock);
	free(scratch.data);
	return retval;
    }
#define clean_scratch() {memset(scratch.data, 0, scratch.length); \
free(scratch.data);}
    if (retval = krb5_finish_key(&eblock)) {

	clean_scratch();
	return retval;
    }
    /*  now decode the decrypted stuff */
    if (!(retval = decode_krb5_enc_tkt_part(&scratch, &dec_tkt_part))) {
	ticket->enc_part2 = dec_tkt_part;
    }
    clean_scratch();
    return retval;
}
