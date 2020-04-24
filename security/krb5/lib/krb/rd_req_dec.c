/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rd_req_dec.c,v $
 * Revision 1.1.6.3  1996/02/18  00:14:39  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:04:07  marty]
 *
 * Revision 1.1.6.2  1995/12/08  18:01:25  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/04/15  00:04 UTC  rps
 * 	beta4 merge
 * 	[1995/03/08  21:43 UTC  rps  /main/rps_hc/1]
 * 
 * Revision 1.1.2.3  1992/12/29  15:58:20  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:15:36  zeliff]
 * 
 * Revision 1.1.2.2  1992/10/23  02:36:11  sommerfeld
 * 	[CR5418] avoid problems with shared structure under tktauthent by only
 * 	leaving tktauthent->ticket non-NULL for a very short time.
 * 	[1992/10/22  17:24:09  sommerfeld]
 * 
 * Revision 1.1  1992/01/19  14:52:18  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rd_req_dec.c
**
** Copyright (c) Hewlett-Packard Company 1995
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/krb/rd_req_dec.c,v $
 * $Author: marty $
 *
 * Copyright (c) 1994 CyberSAFE Corporation.
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
 * permission.  Neither M.I.T., the Open Computing Security Group, nor
 * CyberSAFE Corporation make any representations about the suitability of
 * this software for any purpose.  It is provided "as is" without express
 * or implied warranty.
 * 
 *
 * krb5_rd_req_decoded()
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_rd_req_dec_c[] =
"$Id: rd_req_dec.c,v 1.1.6.3 1996/02/18 00:14:39 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>
#include <krb5/los-proto.h>
#include <krb5/asn1.h>

/*
 * essentially the same as krb_rd_req, but uses a decoded AP_REQ as
 * the input rather than an encoded input.
 */
/*
 *  Parses a KRB_AP_REQ message, returning its contents.
 * 
 *  server specifies the expected server's name for the ticket; if NULL, then
 *  any server will be accepted if the key can be found, and the caller should
 *  verify that the principal is something it trusts.
 * 
 *  sender_addr specifies the address(es) expected to be present in the
 *  ticket.
 * 
 *  rcache specifies a replay detection cache used to store authenticators and
 *  server names
 * 
 *  keyproc specifies a procedure to generate a decryption key for the
 *  ticket.  If keyproc is non-NULL, keyprocarg is passed to it, and the result
 *  used as a decryption key. If keyproc is NULL, then fetchfrom is checked;
 *  if it is non-NULL, it specifies a parameter name from which to retrieve the
 *  decryption key.  If fetchfrom is NULL, then the default key store is
 *  consulted.
 * 
 *  authdat is set to point at allocated storage structures; the caller
 *  should free them when finished. 
 * 
 *  returns system errors, encryption errors, replay errors
 */

/* widen prototypes, if needed */
#include <krb5/widen.h>

static krb5_error_code decrypt_authenticator
	PROTOTYPE((const krb5_ap_req *, krb5_authenticator **));

/* and back to normal... */
#include <krb5/narrow.h>

extern krb5_deltat krb5_clockskew;
#define in_clock_skew(date) (abs((date)-currenttime) < krb5_clockskew)

krb5_error_code
krb5_rd_req_decoded(req, server, sender_addr, fetchfrom, keyproc, keyprocarg,
		    rcache, authdat)
const krb5_ap_req *req;
krb5_const_principal server;
const krb5_address *sender_addr;
krb5_const_pointer fetchfrom;
krb5_rdreq_key_proc keyproc;
krb5_pointer keyprocarg;
krb5_rcache rcache;
krb5_tkt_authent **authdat;
{
    krb5_error_code retval = 0;
    krb5_keyblock *tkt_key = NULL;
    krb5_timestamp currenttime, starttime;
    krb5_tkt_authent	*tktauthent = NULL;

    if (server && !krb5_principal_compare(server, req->ticket->server))
	return KRB5KRB_AP_WRONG_PRINC;

    /* if (req->ap_options & AP_OPTS_USE_SESSION_KEY)
       do we need special processing here ?	*/
    
    /* fetch a server key */
    if (keyproc) {
	retval = (*keyproc)(keyprocarg, req->ticket->server,
                 krb5_csarray[req->ticket->enc_part.etype]->system->proto_keytype,
		 req->ticket->enc_part.kvno, &tkt_key);
    } else {
	krb5_keytab keytabid;
	krb5_keytab_entry ktentry;

	if (fetchfrom) {
	    /* use the named keytab */
	    retval = krb5_kt_resolve(fetchfrom, &keytabid);
	} else {
	    /* use default keytab */
	    retval = krb5_kt_default(&keytabid);
	}
	if (!retval) {
	    retval = krb5_kt_get_entry(keytabid, req->ticket->server,
				       req->ticket->enc_part.kvno, &ktentry);
	    (void) krb5_kt_close(keytabid);
	    if (!retval) {
		retval = krb5_copy_keyblock(&ktentry.key, &tkt_key);
		(void) krb5_kt_free_entry(&ktentry);
	    }
	}
    }
    if (retval)
	return retval;			/* some error in getting the key */

    /* decrypt the ticket */
    if (retval = krb5_decrypt_tkt_part(tkt_key, req->ticket)) {
	krb5_free_keyblock(tkt_key);
	return(retval);
    }

    /*
     * Now we allocate space for the krb5_tkt_authent structure.  If
     * we ever have an error, we're responsible for freeing it.
     */
    if (!(tktauthent =
	  (krb5_tkt_authent *) malloc(sizeof(*tktauthent)))) {
	    retval = ENOMEM;
	    goto cleanup;
    }
    
    memset((char *)tktauthent, 0, sizeof(*tktauthent));
    tktauthent->ticket = 0;

    if (retval = decrypt_authenticator(req, &tktauthent->authenticator))
	goto cleanup;
    *authdat = NULL;		/* Set authdat to tktauthent when we finish */

    if (!krb5_principal_compare(tktauthent->authenticator->client,
				req->ticket->enc_part2->client)) {
	retval = KRB5KRB_AP_ERR_BADMATCH;
	goto cleanup;
    }
    if (sender_addr && !krb5_address_search(sender_addr, req->ticket->enc_part2->caddrs)) {
	retval = KRB5KRB_AP_ERR_BADADDR;
	goto cleanup;
    }

    if (retval = krb5_timeofday(&currenttime))
	goto cleanup;
    if (!in_clock_skew(tktauthent->authenticator->ctime)) {
	retval = KRB5KRB_AP_ERR_SKEW;
	goto cleanup;
    }

    /* okay, now check cross-realm policy */

    { int            i;
      krb5_data      lrealm;
      krb5_data      *realm = krb5_princ_realm(req->ticket->enc_part2->client);
      krb5_transited *trans = &(req->ticket->enc_part2->transited);
  
      /* If the transited list is not empty, then check that all realms */
      /* transited are within the hierarchy between the client's realm  */
      /* and the local realm.                                           */
  
#ifdef OSF_DCE
      if (trans->tr_contents.length && trans->tr_contents.data[0]) {
#else
      if (trans->tr_contents.data && trans->tr_contents.data[0]) {
#endif
        krb5_get_default_realm(&(lrealm.data));
        lrealm.length = strlen(lrealm.data);
        retval = krb5_check_transited_list(&(trans->tr_contents), realm,
                                           &lrealm);
        free(lrealm.data);
      }
    }

    if (retval)  goto cleanup;

    /* only check rcache if sender has provided one---some services
       may not be able to use replay caches (such as datagram servers) */
    if (rcache) {
	krb5_donot_replay rep;
	/* only temporarily...allocated below */
	tktauthent->ticket = req->ticket;	
	retval = krb5_auth_to_rep(tktauthent, &rep);
	tktauthent->ticket = 0;
	if (retval)
	    goto cleanup;
	retval = krb5_rc_store(rcache, &rep);
	xfree(rep.server);
	xfree(rep.client);
	if (retval) 
	    goto cleanup;
    }

    /* if starttime is not in ticket, then treat it as authtime */
    if (req->ticket->enc_part2->times.starttime != 0)
	starttime = req->ticket->enc_part2->times.starttime;
    else
	starttime = req->ticket->enc_part2->times.authtime;

    if (starttime - currenttime > krb5_clockskew) {
	retval = KRB5KRB_AP_ERR_TKT_NYV;	/* ticket not yet valid */
	goto cleanup;
    }	
    if (currenttime - req->ticket->enc_part2->times.endtime > krb5_clockskew) {
	retval = KRB5KRB_AP_ERR_TKT_EXPIRED;	/* ticket expired */
	goto cleanup;
    }	
    if (req->ticket->enc_part2->flags & TKT_FLG_INVALID) {
	retval = KRB5KRB_AP_ERR_TKT_INVALID;
	goto cleanup;
    }
    retval = krb5_copy_ticket(req->ticket, &tktauthent->ticket);
    
cleanup:
    if (tktauthent) {
        if (retval) {
	    krb5_free_tkt_authent(tktauthent);
        } else {
	    tktauthent->ap_options = req->ap_options;
	    *authdat = tktauthent;
	}
    }
    if (retval && req->ticket != NULL && req->ticket->enc_part2 != NULL) {
	/* only free if we're erroring out...otherwise some
	   applications will need the output. */
        krb5_free_enc_tkt_part(req->ticket->enc_part2);
	req->ticket->enc_part2 = NULL;
    }
    if (tkt_key)
	krb5_free_keyblock(tkt_key);
    return retval;
}

static krb5_error_code
decrypt_authenticator(request, authpp)
const krb5_ap_req *request;
krb5_authenticator **authpp;
{
    krb5_authenticator *local_auth;
    krb5_error_code retval;
    krb5_encrypt_block eblock;
    krb5_data scratch;
    krb5_keyblock *sesskey;

    sesskey = request->ticket->enc_part2->session;

    if (!valid_keytype(sesskey->keytype))
	return KRB5_PROG_KEYTYPE_NOSUPP;

    /* put together an eblock for this encryption */

    if (!valid_etype(request->authenticator.etype))
	return KRB5_PROG_ETYPE_NOSUPP;

    krb5_use_cstype(&eblock, request->authenticator.etype);

    scratch.length = request->authenticator.ciphertext.length;
    if (!(scratch.data = malloc(scratch.length)))
	return(ENOMEM);

    /* do any necessary key pre-processing */
    if (retval = krb5_process_key(&eblock, sesskey)) {
	free(scratch.data);
	return(retval);
    }

    /* call the encryption routine */
    if (retval = krb5_decrypt((krb5_pointer) request->authenticator.ciphertext.data,
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
    if (!(retval = decode_krb5_authenticator(&scratch, &local_auth))) {
	*authpp = local_auth;
    }
    clean_scratch();
    return retval;
}

