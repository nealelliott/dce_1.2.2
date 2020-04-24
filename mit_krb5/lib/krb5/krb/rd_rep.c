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
 * $Log: rd_rep.c,v $
 * Revision 1.1.2.1  1996/06/05  21:06:02  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  20:02:41  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * lib/krb5/krb/rd_rep.c
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
 * krb5_rd_rep()
 */

#include "k5-int.h"
#include "auth_con.h"

/*
 *  Parses a KRB_AP_REP message, returning its contents.
 * 
 *  repl is filled in with with a pointer to allocated memory containing
 * the fields from the encrypted response. 
 * 
 *  the key in kblock is used to decrypt the message.
 * 
 *  returns system errors, encryption errors, replay errors
 */

krb5_error_code INTERFACE
krb5_rd_rep(context, auth_context, inbuf, repl)
    krb5_context 	  context;
    krb5_auth_context	* auth_context;
    const krb5_data 	* inbuf;
    krb5_ap_rep_enc_part **repl;
{
    krb5_error_code 	  retval;
    krb5_ap_rep 	* reply;
    krb5_encrypt_block 	  eblock;
    krb5_data 	 	  scratch;

    if (!krb5_is_ap_rep(inbuf))
	return KRB5KRB_AP_ERR_MSG_TYPE;

    /* decode it */

    if (retval = decode_krb5_ap_rep(inbuf, &reply))
	return retval;

    /* put together an eblock for this encryption */

    if (!valid_etype(reply->enc_part.etype)) {
	krb5_free_ap_rep(context, reply);
	return KRB5_PROG_ETYPE_NOSUPP;
    }
    krb5_use_cstype(context, &eblock, reply->enc_part.etype);

    scratch.length = reply->enc_part.ciphertext.length;
    if (!(scratch.data = malloc(scratch.length))) {
	krb5_free_ap_rep(context, reply);
	return(ENOMEM);
    }

    /* do any necessary key pre-processing */
    if (retval = krb5_process_key(context, &eblock, auth_context->keyblock)) {
	goto errout;
    }

    /* call the encryption routine */
    if (retval = krb5_decrypt(context, 
			      (krb5_pointer) reply->enc_part.ciphertext.data,
			      (krb5_pointer) scratch.data,
			      scratch.length, &eblock, 0)) {
	(void) krb5_finish_key(context, &eblock);
	goto errout;
    }

    /* finished with the top-level encoding of the ap_rep */
    if (retval = krb5_finish_key(context, &eblock)) 
	goto clean_scratch;

    /* now decode the decrypted stuff */
    retval = decode_krb5_ap_rep_enc_part(&scratch, repl);

    /* Check reply fields */
    if (((*repl)->ctime != auth_context->authentp->ctime) ||
      ((*repl)->cusec != auth_context->authentp->cusec)) {
	retval = KRB5_SENDAUTH_MUTUAL_FAILED;
	goto clean_scratch;
    }

    /* Set auth subkey */
    if ((*repl)->subkey) {
	(*repl)->subkey->etype = reply->enc_part.etype;
	auth_context->remote_subkey = (*repl)->subkey;
    }

    /* Get remote sequence number */
    auth_context->remote_seq_number = (*repl)->seq_number;

clean_scratch:
    memset(scratch.data, 0, scratch.length); 
errout:
    krb5_free_ap_rep(context, reply);
    free(scratch.data);
    return retval;
}
