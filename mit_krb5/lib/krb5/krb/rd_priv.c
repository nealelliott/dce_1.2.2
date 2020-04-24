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
 * $Log: rd_priv.c,v $
 * Revision 1.1.2.1  1996/06/05  21:05:51  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  20:02:30  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rd_priv.c,v $
 * Revision 1.1.2.1  1996/06/05  21:05:51  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  20:02:30  arvind]
 *
 * Revision 1.1.1.2  1996/06/05  20:02:30  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 *
 * Revision /main/DCE_1.2.2/2  1996/05/30  21:18 UTC  mullan_s
 * 	Merge krb5 work to DCE1.2.2 mainline
 * 
 * Revision /main/DCE_1.2.2/mullan_dce1.2.2_krb5_drop/1  1996/05/29  20:42 UTC  mullan_s
 * 	Merge krb5 work from DCE_1.2 to DCE_1.2.2
 * 
 * Revision /main/DCE_1.2/mullan_dce_k5_bsd44_rutils/2  1996/05/17  14:18 UTC  mullan_s
 * 	Remove #ifdef OSF_DCE - just a bug fix, not an enhancement.
 * 
 * Revision /main/DCE_1.2/mullan_dce_k5_bsd44_rutils/1  1996/05/03  20:06 UTC  mullan_s
 * 	Add OSF/HP copyrights & RCS header.
 * 
 * $EndLog$
 */
/*
 * lib/krb5/krb/rd_priv.c
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
 * krb5_rd_priv()
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/

#include <k5-int.h>
#include "cleanup.h"
#include "auth_con.h"

extern krb5_deltat krb5_clockskew;   
#define in_clock_skew(date) (labs((date)-currenttime) < krb5_clockskew)

/*

Parses a KRB_PRIV message from inbuf, placing the confidential user
data in *outbuf.

key specifies the key to be used for decryption of the message.
 
remote_addr and local_addr specify the full
addresses (host and port) of the sender and receiver.

outbuf points to allocated storage which the caller should
free when finished.

i_vector is used as an initialization vector for the
encryption, and if non-NULL its contents are replaced with the last
block of the encrypted data upon exit.

Returns system errors, integrity errors.

*/

static krb5_error_code
krb5_rd_priv_basic(context, inbuf, keyblock, local_addr, remote_addr, 
	     	   i_vector, replaydata, outbuf)
    krb5_context 	  context;
    const krb5_data     * inbuf;
    const krb5_keyblock * keyblock;
    const krb5_address  * local_addr;
    const krb5_address  * remote_addr;
    krb5_pointer 	  i_vector;
    krb5_replay_data    * replaydata;
    krb5_data 		* outbuf;
{
    krb5_error_code 	  retval;
    krb5_priv 		* privmsg;
    krb5_data 		  scratch;
    krb5_encrypt_block 	  eblock;
    krb5_priv_enc_part  * privmsg_enc_part;

    if (!krb5_is_krb_priv(inbuf))
	return KRB5KRB_AP_ERR_MSG_TYPE;

    /* decode private message */
    if (retval = decode_krb5_priv(inbuf, &privmsg)) 
	return retval;
    
    if (!valid_etype(privmsg->enc_part.etype)) {
	retval = KRB5_PROG_ETYPE_NOSUPP;
	goto cleanup_privmsg;
    }
			   
    /* put together an eblock for this decryption */
    krb5_use_cstype(context, &eblock, privmsg->enc_part.etype);
    scratch.length = privmsg->enc_part.ciphertext.length;
    
    if (!(scratch.data = malloc(scratch.length))) {
	retval = ENOMEM;
	goto cleanup_privmsg;
    }

    /* do any necessary key pre-processing */
    if (retval = krb5_process_key(context, &eblock, keyblock)) 
	goto cleanup_scratch;

    /* call the decryption routine */
    if (retval = krb5_decrypt(context, 
			      (krb5_pointer) privmsg->enc_part.ciphertext.data,
			      (krb5_pointer) scratch.data,
			      scratch.length, &eblock, i_vector)) {
    	krb5_finish_key(context, &eblock);
        goto cleanup_scratch;
    }

    /* if i_vector is set, put last block into the i_vector */
    if (i_vector)
	memcpy(i_vector,
	       privmsg->enc_part.ciphertext.data +
	       (privmsg->enc_part.ciphertext.length -
	        eblock.crypto_entry->block_length),
	       eblock.crypto_entry->block_length);

    if (retval = krb5_finish_key(context, &eblock)) 
        goto cleanup_scratch;

    /*  now decode the decrypted stuff */
    if (retval = decode_krb5_enc_priv_part(&scratch, &privmsg_enc_part)) 
        goto cleanup_scratch;

    if (!krb5_address_compare(context,remote_addr,privmsg_enc_part->s_address)){
	retval = KRB5KRB_AP_ERR_BADADDR;
	goto cleanup_data;
    }
    
    if (privmsg_enc_part->r_address) {
	if (local_addr) {
	    if (!krb5_address_compare(context, local_addr,
				      privmsg_enc_part->r_address)) {
		retval = KRB5KRB_AP_ERR_BADADDR;
		goto cleanup_data;
	    }
	} else {
	    krb5_address **our_addrs;
	
	    if (retval = krb5_os_localaddr(&our_addrs)) {
		goto cleanup_data;
	    }
	    if (!krb5_address_search(context, privmsg_enc_part->r_address, 
				     our_addrs)) {
		krb5_free_addresses(context, our_addrs);
		retval =  KRB5KRB_AP_ERR_BADADDR;
		goto cleanup_data;
	    }
	    krb5_free_addresses(context, our_addrs);
	}
    }

    replaydata->timestamp = privmsg_enc_part->timestamp;
    replaydata->usec = privmsg_enc_part->usec;
    replaydata->seq = privmsg_enc_part->seq_number;

    /* everything is ok - return data to the user */
    *outbuf = privmsg_enc_part->user_data;
    retval = 0;

cleanup_data:;
    if (retval == 0)
	privmsg_enc_part->user_data.data = 0;
    krb5_free_priv_enc_part(context, privmsg_enc_part);

cleanup_scratch:;
    memset(scratch.data, 0, scratch.length); 
    krb5_xfree(scratch.data);

cleanup_privmsg:;
    krb5_xfree(privmsg->enc_part.ciphertext.data); 
    krb5_xfree(privmsg);

    return retval;
}

krb5_error_code
krb5_rd_priv(context, auth_context, inbuf, outbuf, outdata)
    krb5_context 	  context;
    krb5_auth_context 	* auth_context;
    const krb5_data   	* inbuf;
    krb5_data 	      	* outbuf;
    krb5_replay_data  	* outdata;
{
    krb5_error_code 	  retval;
    krb5_keyblock       * keyblock;
    krb5_replay_data	  replaydata;

    /* Get keyblock */
    if ((keyblock = auth_context->local_subkey) == NULL)
        if ((keyblock = auth_context->remote_subkey) == NULL)
            keyblock = auth_context->keyblock;

    if (((auth_context->auth_context_flags & KRB5_AUTH_CONTEXT_RET_TIME) ||
      (auth_context->auth_context_flags & KRB5_AUTH_CONTEXT_RET_SEQUENCE)) &&
      (outdata == NULL))
	/* Need a better error */
	return KRB5_RC_REQUIRED;

    if ((auth_context->auth_context_flags & KRB5_AUTH_CONTEXT_DO_TIME) &&
      (auth_context->rcache == NULL))
	return KRB5_RC_REQUIRED;

{
    krb5_address * premote_fulladdr = NULL;
    krb5_address * plocal_fulladdr = NULL;
    krb5_address remote_fulladdr;
    krb5_address local_fulladdr;
    CLEANUP_INIT(2);

    if (auth_context->local_addr) {
    	if (auth_context->local_port) {
            if (!(retval = krb5_make_fulladdr(context, auth_context->local_addr,
                                 	      auth_context->local_port, 
					      &local_fulladdr))){
                CLEANUP_PUSH(local_fulladdr.contents, free);
	        plocal_fulladdr = &local_fulladdr;
            } else {
	        return retval;
            }
	} else {
            plocal_fulladdr = auth_context->local_addr;
        }
    }

    if (auth_context->remote_addr) {
    	if (auth_context->remote_port) {
            if (!(retval = krb5_make_fulladdr(context,auth_context->remote_addr,
                                 	      auth_context->remote_port, 
					      &remote_fulladdr))){
                CLEANUP_PUSH(remote_fulladdr.contents, free);
	        premote_fulladdr = &remote_fulladdr;
            } else {
                CLEANUP_DONE();
	        return retval;
            }
	} else {
            premote_fulladdr = auth_context->remote_addr;
        }
    }

    if (retval = krb5_rd_priv_basic(context, inbuf, keyblock, plocal_fulladdr, 
				    premote_fulladdr, auth_context->i_vector, 
				    &replaydata, outbuf)) {
	CLEANUP_DONE();
	return retval;
    }

    CLEANUP_DONE();
}

    if (auth_context->auth_context_flags & KRB5_AUTH_CONTEXT_DO_TIME) {
	krb5_donot_replay replay;
    	krb5_timestamp currenttime;

	if (retval = krb5_timeofday(context, &currenttime)) 
	    goto error;

	if (!in_clock_skew(replaydata.timestamp)) {
	    retval =  KRB5KRB_AP_ERR_SKEW;
	    goto error;
	}

	if (retval = krb5_gen_replay_name(context, auth_context->remote_addr, 
					  "_priv", &replay.client)) 
	    goto error;

	replay.server = "";		/* XXX */
	replay.cusec = replaydata.usec;
	replay.ctime = replaydata.timestamp;
	if (retval = krb5_rc_store(context, auth_context->rcache, &replay)) {
	    krb5_xfree(replay.client);
	    goto error;
	}
	krb5_xfree(replay.client);
    }

    if (auth_context->auth_context_flags & KRB5_AUTH_CONTEXT_DO_SEQUENCE) {
	if (auth_context->remote_seq_number != replaydata.seq) {
	    retval =  KRB5KRB_AP_ERR_BADORDER;
	    goto error;
	}
	auth_context->remote_seq_number++;
    }

    if ((auth_context->auth_context_flags & KRB5_AUTH_CONTEXT_RET_TIME) ||
      (auth_context->auth_context_flags & KRB5_AUTH_CONTEXT_RET_SEQUENCE)) {
	outdata->timestamp = replaydata.timestamp;
	outdata->usec = replaydata.usec;
	outdata->seq = replaydata.seq;
    }
	
    /* everything is ok - return data to the user */
    return 0;

error:;
    krb5_xfree(outbuf->data);
    return retval;

}

