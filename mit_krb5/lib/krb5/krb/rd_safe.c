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
 * $Log: rd_safe.c,v $
 * Revision 1.1.2.1  1996/06/05  21:06:36  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  20:03:25  arvind]
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
 * $Log: rd_safe.c,v $
 * Revision 1.1.2.1  1996/06/05  21:06:36  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  20:03:25  arvind]
 *
 * Revision 1.1.1.2  1996/06/05  20:03:25  arvind
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
 * lib/krb5/krb/rd_safe.c
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
 * krb5_rd_safe()
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
 parses a KRB_SAFE message from inbuf, placing the integrity-protected user
 data in *outbuf.

 key specifies the key to be used for decryption of the message.
 
 sender_addr and recv_addr specify the full addresses (host and port) of
 the sender and receiver.

 outbuf points to allocated storage which the caller should free when finished.

 returns system errors, integrity errors
 */
static krb5_error_code
krb5_rd_safe_basic(context, inbuf, keyblock, recv_addr, sender_addr, 
	     	   replaydata, outbuf)
    krb5_context          context;
    const krb5_data     * inbuf;
    const krb5_keyblock * keyblock;
    const krb5_address  * recv_addr;
    const krb5_address  * sender_addr;
    krb5_replay_data    * replaydata; 
    krb5_data           * outbuf;
{
    krb5_error_code 	  retval;
    krb5_safe 		* message;
    krb5_checksum our_cksum, *his_cksum;
    krb5_octet zero_octet = 0;
    krb5_data *scratch;

    if (!krb5_is_krb_safe(inbuf))
	return KRB5KRB_AP_ERR_MSG_TYPE;

    if (retval = decode_krb5_safe(inbuf, &message))
	return retval;

    if (!valid_cksumtype(message->checksum->checksum_type)) {
	retval = KRB5_PROG_SUMTYPE_NOSUPP;
	goto cleanup;
    }
    if (!is_coll_proof_cksum(message->checksum->checksum_type) ||
	!is_keyed_cksum(message->checksum->checksum_type)) {
	retval = KRB5KRB_AP_ERR_INAPP_CKSUM;
	goto cleanup;
    }

    if (!krb5_address_compare(context, sender_addr, message->s_address)) {
	retval = KRB5KRB_AP_ERR_BADADDR;
	goto cleanup;
    }

    if (message->r_address) {
	if (recv_addr) {
	    if (!krb5_address_compare(context, recv_addr, message->r_address)) {
		retval = KRB5KRB_AP_ERR_BADADDR;
		goto cleanup;
	    }
	} else {
	    krb5_address **our_addrs;
	
	    if (retval = krb5_os_localaddr( &our_addrs)) 
		goto cleanup;
	    
	    if (!krb5_address_search(context, message->r_address, our_addrs)) {
		krb5_free_addresses(context, our_addrs);
		retval = KRB5KRB_AP_ERR_BADADDR;
		goto cleanup;
	    }
	    krb5_free_addresses(context, our_addrs);
	}
    }

    /* verify the checksum */
    /* to do the checksum stuff, we need to re-encode the message with a
       zero-length zero-type checksum, then checksum the encoding, and verify.
     */
    his_cksum = message->checksum;

    our_cksum.length = 0;
    our_cksum.checksum_type = 0;
    our_cksum.contents = &zero_octet;

    message->checksum = &our_cksum;

    if (retval = encode_krb5_safe(message, &scratch)) 
	goto cleanup;

    message->checksum = his_cksum;
			 
    if (!(our_cksum.contents = (krb5_octet *)
	  malloc(krb5_checksum_size(context, his_cksum->checksum_type)))) {
	retval = ENOMEM;
	goto cleanup;
    }

    retval = krb5_calculate_checksum(context, his_cksum->checksum_type,
				     scratch->data, scratch->length,
				     (krb5_pointer) keyblock->contents,
				     keyblock->length, &our_cksum);
    (void) memset((char *)scratch->data, 0, scratch->length);
    krb5_free_data(context, scratch);
    
    if (retval) {
	goto cleanup_cksum;
    }

    if (our_cksum.length != his_cksum->length ||
	memcmp((char *)our_cksum.contents, (char *)his_cksum->contents,
	       our_cksum.length)) {
	retval = KRB5KRB_AP_ERR_MODIFIED;
	goto cleanup_cksum;
    }

    replaydata->timestamp = message->timestamp;
    replaydata->usec = message->usec;
    replaydata->seq = message->seq_number;

    *outbuf = message->user_data;
    message->user_data.data = NULL;

    krb5_free_checksum(context, his_cksum);
    return 0;

cleanup_cksum:
    krb5_xfree(our_cksum.contents);

cleanup:
    krb5_free_safe(context, message);
    return retval;
}

krb5_error_code
krb5_rd_safe(context, auth_context, inbuf, outbuf, outdata)
    krb5_context 	  context;
    krb5_auth_context 	* auth_context;
    const krb5_data   	* inbuf;
    krb5_data 	      	* outbuf;
    krb5_replay_data  	* outdata;
{
    krb5_error_code 	  retval;
    krb5_keyblock	* keyblock;
    krb5_replay_data	  replaydata;

    if (((auth_context->auth_context_flags & KRB5_AUTH_CONTEXT_RET_TIME) ||
      (auth_context->auth_context_flags & KRB5_AUTH_CONTEXT_RET_SEQUENCE)) &&
      (outdata == NULL)) 
	/* Need a better error */
	return KRB5_RC_REQUIRED;

    if ((auth_context->auth_context_flags & KRB5_AUTH_CONTEXT_DO_TIME) &&
      (auth_context->rcache == NULL)) 
	return KRB5_RC_REQUIRED;

    /* Get keyblock */
    if ((keyblock = auth_context->local_subkey) == NULL)
        if ((keyblock = auth_context->remote_subkey) == NULL)
            keyblock = auth_context->keyblock;

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
	        return retval;
            }
	} else {
            premote_fulladdr = auth_context->remote_addr;
        }
    }

    if (retval = krb5_rd_safe_basic(context, inbuf, keyblock, plocal_fulladdr, 
				    premote_fulladdr, &replaydata, outbuf)) {
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
					  "_safe", &replay.client)) 
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

