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
 * $Log: mk_safe.c,v $
 * Revision 1.1.2.1  1996/06/05  21:04:32  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  20:01:24  arvind]
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
 * $Log: mk_safe.c,v $
 * Revision 1.1.2.1  1996/06/05  21:04:32  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  20:01:24  arvind]
 *
 * Revision 1.1.1.2  1996/06/05  20:01:24  arvind
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
 * lib/krb5/krb/mk_safe.c
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
 * krb5_mk_safe()
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/

#include <k5-int.h>
#include "cleanup.h"
#include "auth_con.h"

/*
 Formats a KRB_SAFE message into outbuf.

 userdata is formatted as the user data in the message.
 sumtype specifies the encryption type; key specifies the key which
 might be used to seed the checksum; sender_addr and recv_addr specify
 the full addresses (host and port) of the sender and receiver.
 The host portion of sender_addr is used to form the addresses used in the
 KRB_SAFE message.

 The outbuf buffer storage is allocated, and should be freed by the
 caller when finished.

 returns system errors
*/
static krb5_error_code
krb5_mk_safe_basic(context, userdata, keyblock, replaydata, local_addr,
		   remote_addr, sumtype, outbuf)
    krb5_context 	  context;
    const krb5_data 	* userdata;
    const krb5_keyblock * keyblock;
    krb5_replay_data    * replaydata;
    const krb5_address 	* local_addr;
    const krb5_address 	* remote_addr;
    const krb5_cksumtype  sumtype;
    krb5_data 		* outbuf;
{
    krb5_error_code retval;
    krb5_safe safemsg;
    krb5_octet zero_octet = 0;
    krb5_checksum safe_checksum;
    krb5_data *scratch1, *scratch2;

    if (!valid_cksumtype(sumtype))
	return KRB5_PROG_SUMTYPE_NOSUPP;
    if (!is_coll_proof_cksum(sumtype) || !is_keyed_cksum(sumtype))
	return KRB5KRB_AP_ERR_INAPP_CKSUM;

    safemsg.user_data = *userdata;
    safemsg.s_address = (krb5_address *) local_addr;
    safemsg.r_address = (krb5_address *) remote_addr;

    /* We should check too make sure one exists. */
    safemsg.timestamp  = replaydata->timestamp;
    safemsg.usec       = replaydata->usec;
    safemsg.seq_number = replaydata->seq;

    /* 
     * To do the checksum stuff, we need to encode the message with a
     * zero-length zero-type checksum, then checksum the encoding, then
     * re-encode with the checksum. 
     */

    safe_checksum.length = 0;
    safe_checksum.checksum_type = 0;
    safe_checksum.contents = &zero_octet;

    safemsg.checksum = &safe_checksum;

    if (retval = encode_krb5_safe(&safemsg, &scratch1))
	return retval;

    if (!(safe_checksum.contents =
	  (krb5_octet *) malloc(krb5_checksum_size(context, sumtype)))) {
	retval = ENOMEM;
	goto cleanup_scratch;
    }
    if (retval = krb5_calculate_checksum(context, sumtype, scratch1->data,
					 scratch1->length,
					 (krb5_pointer) keyblock->contents,
					 keyblock->length, &safe_checksum)) {
	goto cleanup_checksum;
    }
    safemsg.checksum = &safe_checksum;
    if (retval = encode_krb5_safe(&safemsg, &scratch2)) {
	goto cleanup_checksum;
    }
    *outbuf = *scratch2;
    krb5_xfree(scratch2);
    retval = 0;

cleanup_checksum:
    krb5_xfree(safe_checksum.contents);

cleanup_scratch:
    memset((char *)scratch1->data, 0, scratch1->length); 
    krb5_free_data(context, scratch1);
    return retval;
}

krb5_error_code
krb5_mk_safe(context, auth_context, userdata, outbuf, outdata)
    krb5_context 	  context;
    krb5_auth_context 	* auth_context;
    const krb5_data   	* userdata;
    krb5_data         	* outbuf;
    krb5_replay_data  	* outdata;
{
    krb5_error_code 	  retval;
    krb5_keyblock       * keyblock;
    krb5_replay_data      replaydata;

    /* Clear replaydata block */
    memset((char *) &replaydata, 0, sizeof(krb5_replay_data));

    /* Get keyblock */
    if ((keyblock = auth_context->local_subkey) == NULL)
        if ((keyblock = auth_context->remote_subkey) == NULL)
            keyblock = auth_context->keyblock;

    /* Get replay info */
    if ((auth_context->auth_context_flags & KRB5_AUTH_CONTEXT_DO_TIME) &&
      (auth_context->rcache == NULL))
	return KRB5_RC_REQUIRED;

    if (((auth_context->auth_context_flags & KRB5_AUTH_CONTEXT_RET_TIME) ||
      (auth_context->auth_context_flags & KRB5_AUTH_CONTEXT_RET_SEQUENCE)) &&
      (outdata == NULL))
	/* Need a better error */
	return KRB5_RC_REQUIRED;

    if ((auth_context->auth_context_flags & KRB5_AUTH_CONTEXT_DO_TIME) ||
	(auth_context->auth_context_flags & KRB5_AUTH_CONTEXT_RET_TIME)) {
	if (retval = krb5_us_timeofday(context, &replaydata.timestamp,
				       &replaydata.usec))
	    return retval;
	if (auth_context->auth_context_flags & KRB5_AUTH_CONTEXT_RET_TIME) {
    	    outdata->timestamp = replaydata.timestamp;
    	    outdata->usec = replaydata.usec;
	}
    }
    if ((auth_context->auth_context_flags & KRB5_AUTH_CONTEXT_DO_SEQUENCE) ||
	(auth_context->auth_context_flags & KRB5_AUTH_CONTEXT_RET_SEQUENCE)) {
	replaydata.seq = auth_context->local_seq_number++;
	if (auth_context->auth_context_flags & KRB5_AUTH_CONTEXT_RET_SEQUENCE) {
    	    outdata->seq = replaydata.seq;
	}
    } 

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
                goto error;
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
                goto error;
            }
	} else {
            premote_fulladdr = auth_context->remote_addr;
        }
    }

    if (retval = krb5_mk_safe_basic(context, userdata, keyblock, &replaydata, 
				    plocal_fulladdr, premote_fulladdr,
      				    auth_context->cksumtype, outbuf)) {
	CLEANUP_DONE();
	goto error;
    }

    CLEANUP_DONE();
}

    if (auth_context->auth_context_flags & KRB5_AUTH_CONTEXT_DO_TIME) {
	krb5_donot_replay replay;

	if (retval = krb5_gen_replay_name(context, auth_context->local_addr, 
					  "_safe", &replay.client)) {
    	    krb5_xfree(outbuf);
	    goto error;
	}

	replay.server = "";		/* XXX */
	replay.cusec = replaydata.usec;
	replay.ctime = replaydata.timestamp;
	if (retval = krb5_rc_store(context, auth_context->rcache, &replay)) {
	    /* should we really error out here? XXX */
    	    krb5_xfree(outbuf);
	    goto error;
	}
	krb5_xfree(replay.client);
    }

    return 0;

error:
    if ((auth_context->auth_context_flags & KRB5_AUTH_CONTEXT_DO_SEQUENCE) ||
      (auth_context->auth_context_flags & KRB5_AUTH_CONTEXT_RET_SEQUENCE)) 
	auth_context->local_seq_number--;

    return retval;
}

