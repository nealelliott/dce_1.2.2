/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rd_safe.c,v $
 * Revision 1.1.6.2  1996/02/18  00:14:42  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:04:10  marty]
 *
 * Revision 1.1.6.1  1995/12/08  18:01:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:00:11  root]
 * 
 * Revision 1.1.2.2  1992/12/29  15:58:33  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:15:44  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:52:10  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rd_safe.c V=4 10/24/91 //littl/prgy/krb5/lib/krb
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/krb/rd_safe.c,v $
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
 * krb5_rd_safe()
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_rd_safe_c[] =
"$Id: rd_safe.c,v 1.1.6.2 1996/02/18 00:14:42 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/asn1.h>
#include <krb5/los-proto.h>
#include <krb5/ext-proto.h>

extern krb5_deltat krb5_clockskew;
#define in_clock_skew(date) (abs((date)-currenttime) < krb5_clockskew)

/*
 parses a KRB_SAFE message from inbuf, placing the integrity-protected user
 data in *outbuf.

 key specifies the key to be used for decryption of the message.
 
 sender_addr and recv_addr specify the full addresses (host and port) of
 the sender and receiver.

 outbuf points to allocated storage which the caller should free when finished.

 returns system errors, integrity errors
 */
krb5_error_code
krb5_rd_safe(inbuf, key, sender_addr, recv_addr, seq_number, safe_flags,
	     rcache, outbuf)
const krb5_data *inbuf;
const krb5_keyblock *key;
const krb5_address *sender_addr;
const krb5_address *recv_addr;
krb5_int32 seq_number;
krb5_int32 safe_flags;
krb5_rcache rcache;
krb5_data *outbuf;
{
    krb5_error_code retval;
    krb5_safe *message;
    krb5_checksum our_cksum, *his_cksum;
    krb5_octet zero_octet = 0;
    krb5_data *scratch;
    krb5_timestamp currenttime;

    if (!krb5_is_krb_safe(inbuf))
	return KRB5KRB_AP_ERR_MSG_TYPE;

    if (retval = decode_krb5_safe(inbuf, &message))
	return retval;

#define cleanup() krb5_free_safe(message)

    if (!valid_cksumtype(message->checksum->checksum_type))
	return KRB5_PROG_SUMTYPE_NOSUPP;
    if (!is_coll_proof_cksum(message->checksum->checksum_type) ||
	!is_keyed_cksum(message->checksum->checksum_type))
	return KRB5KRB_AP_ERR_INAPP_CKSUM;

    if (!(safe_flags & KRB5_SAFE_NOTIME)) {
	krb5_donot_replay replay;

	if (retval = krb5_timeofday(&currenttime)) {
	    cleanup();
	    return retval;
	}
	/* in_clock_skew #defined above */
	if (!in_clock_skew(message->timestamp)) {
	    cleanup();
	    return KRB5KRB_AP_ERR_SKEW;
	}
	if (!rcache) {
	    /* gotta provide an rcache in this case... */
	    cleanup();
	    return KRB5_RC_REQUIRED;
	}
	if (!krb5_address_compare(sender_addr, message->s_address)) {
	    cleanup();
	    return KRB5KRB_AP_ERR_BADADDR;
	}
	if (retval = krb5_gen_replay_name(sender_addr, "_safe",
					  &replay.client)) {
	    cleanup();
	    return retval;
	}
	replay.server = "";		/* XXX */
	replay.cusec = message->usec;
	replay.ctime = message->timestamp;
	if (retval = krb5_rc_store(rcache, &replay)) {
	    xfree(replay.client);
	    cleanup();
	    return retval;
	}
	xfree(replay.client);
    }

    if (safe_flags & KRB5_SAFE_DOSEQUENCE)
	if (message->seq_number != seq_number) {
	    cleanup();
	    return KRB5KRB_AP_ERR_BADORDER;
	}

    if (message->r_address) {
	krb5_address **our_addrs;
	
	if (retval = krb5_os_localaddr(&our_addrs)) {
	    cleanup();
	    return retval;
	}
	if (!krb5_address_search(message->r_address, our_addrs)) {
	    krb5_free_addresses(our_addrs);
	    cleanup();
	    return KRB5KRB_AP_ERR_BADADDR;
	}
	krb5_free_addresses(our_addrs);
    }

    /* verify the checksum */
    /* to do the checksum stuff, we need to re-encode the message with a
       zero-length zero-type checksum, then checksum the encoding, and verify.
     */
    his_cksum = message->checksum;

    our_cksum.checksum_type = 0;
    our_cksum.length = 0;
    our_cksum.contents = &zero_octet;

    message->checksum = &our_cksum;

    if (retval = encode_krb5_safe(message, &scratch)) {
	message->checksum = his_cksum;
	cleanup();
	return retval;
    }
    message->checksum = his_cksum;
			 
    if (!(our_cksum.contents = (krb5_octet *)
	  malloc(krb5_checksum_size(his_cksum->checksum_type)))) {
	cleanup();
	return ENOMEM;
    }

#undef cleanup
#define cleanup() {krb5_free_safe(message); xfree(our_cksum.contents);}

    retval = krb5_calculate_checksum(his_cksum->checksum_type,
				     scratch->data, scratch->length,
				     (krb5_pointer) key->contents,
				     key->length, &our_cksum);
    (void) memset((char *)scratch->data, 0, scratch->length);
    krb5_free_data(scratch);
    
    if (retval) {
	cleanup();
	return retval;
    }

    if (our_cksum.length != his_cksum->length ||
	memcmp((char *)our_cksum.contents, (char *)his_cksum->contents,
	       our_cksum.length)) {
	cleanup();
	return KRB5KRB_AP_ERR_MODIFIED;
    }

    *outbuf = message->user_data;

    xfree(our_cksum.contents);
    if (message->s_address)
	krb5_free_address(message->s_address);
    if (message->r_address)
	krb5_free_address(message->r_address);
    krb5_free_checksum(his_cksum);
    xfree(message);

    return 0;
}
