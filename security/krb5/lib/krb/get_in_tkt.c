/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: get_in_tkt.c,v $
 * Revision 1.1.16.3  1996/11/13  18:07:46  arvind
 * 	Merge to DCE 1.2.2
 * 	[1996/10/30  23:05 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce12_tp_preauth2/2]
 *
 * 	Check preauth error replies for clock skew.
 * 	[1996/10/30  19:19 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce12_tp_preauth2/1]
 *
 * 	Fix minor code review comments.
 * 	Fix duplicate free of err_reply.
 * 	[1996/10/28  20:44 UTC  mullan_s  /main/DCE_1.2.2/3]
 *
 * 	Merge to DCE_1.2.2
 * 	[1996/10/24  18:53 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce12_tp_preauth/7]
 *
 * 	Allow local rgy TGTs to pass verify_as_reply.
 * 	Fixes for decoding DCE edata (first pass)
 * 	[1996/10/21  16:53 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce12_tp_preauth/6]
 *
 * 	Implement pa specific data storage code.
 * 	[1996/10/17  13:43 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce12_tp_preauth/5]
 *
 * 	code cleanup
 * 	[1996/10/15  18:29 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce12_tp_preauth/4]
 *
 * 	Add reply key parameter to obtain_preauth for
 * 	DCE third party protocol.
 * 	[1996/10/15  13:26 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce12_tp_preauth/3]
 *
 * 	local rgy rework
 * 	[1996/10/14  15:52 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce12_tp_preauth/2]
 *
 * 	DCE Third Party Preauthentication rework: decode DCE error info in
 * 	krb5 e_data packets returned in a failed AS request.
 * 	[1996/10/11  14:56 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce12_tp_preauth/1]
 *
 * Revision 1.1.16.2  1996/10/15  20:52:08  arvind
 * 	Partial fix to CHFts20154 (real error codes)
 * 	[1996/10/04  22:15 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_2/1]
 * 
 * Revision 1.1.16.1  1996/10/03  15:02:30  arvind
 * 	Merge in code from krb5beta6.
 * 	[1996/09/16  21:14 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 * 
 * Revision 1.1.14.3  1996/02/18  00:14:02  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:03:41  marty]
 * 
 * Revision 1.1.14.2  1995/12/08  17:59:30  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/3  1995/05/08  21:34 UTC  jrr
 * 	Merge work to handle 1.0.3 kinit.
 * 
 * 	HP revision /main/HPDCE02/jrr_sec_chfts14997/1  1995/05/08  21:30 UTC  jrr
 * 	Merge fixes by mullan_s for correct operation with 1.0.3 kinit.
 * 
 * 	HP revision /main/HPDCE02/2  1995/04/06  17:54 UTC  mullan_s
 * 	Merge OT12769
 * 
 * 	HP revision /main/mullan_ot12769/1  1995/02/14  17:51 UTC  mullan_s
 * 	OT12769: Check for clock skew in error packets returned from KDC.
 * 
 * 	HP revision /main/HPDCE02/1  1995/03/31  13:36 UTC  aha
 * 	CHFts14690: fix krb5 pre-auth memory leak
 * 
 * 	HP revision /main/aha_mothra4/3  1995/03/29  15:14 UTC  aha
 * 	CHFts14690: put changes in #ifdef OSF_DCE, do not free padata input
 * 
 * 	HP revision /main/aha_mothra4/2  1995/03/29  13:49 UTC  aha
 * 	Use krb5_free_pa_data
 * 
 * 	HP revision /main/aha_mothra4/1  1995/03/28  20:45 UTC  aha
 * 	CHFts14690: need to free padata block and contents in krb5_do_as_request
 * 	[1995/12/08  16:59:52  root]
 * 
 * Revision 1.1.10.1  1994/10/25  22:15:05  mullan_s
 * 	[OT12495] Plaintext third party preauth fixes: modularize
 * 	code so that padata can be passed in from sec_login layer.
 * 	[1994/10/25  22:14:45  mullan_s]
 * 
 * Revision 1.1.6.2  1994/08/04  16:13:09  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:10:26  mdf]
 * 
 * Revision 1.1.6.1  1994/05/11  19:22:32  ahop
 * 	hp_sec_to_osf_2 drop
 * 	[1994/04/29  21:19:51  ahop]
 * 
 * Revision 1.1.4.2  1992/12/29  15:56:14  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:14:25  zeliff]
 * 
 * Revision 1.1.2.3  1992/05/08  17:00:47  sommerfeld
 * 	Null out ticket.data after freeing it.
 * 	[1992/05/06  21:05:57  sommerfeld]
 * 
 * Revision 1.1.2.2  1992/05/05  15:12:46  sommerfeld
 * 	Use distinct status code for clock skew problems.
 * 	[1992/05/04  19:58:25  sommerfeld]
 * 
 * Revision 1.1  1992/01/19  14:51:49  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  get_in_tkt.c V=4 10/24/91 //littl/prgy/krb5/lib/krb
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/krb/get_in_tkt.c,v $
 * $Author: arvind $
 *
 * Copyright 1990 by the Massachusetts Institute of Technology.
 *
 * For copying and distribution information, please see the file
 * <krb5/copyright.h>.
 *
 * krb5_get_in_tkt()
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_get_in_tkt_c[] =
"$Id: get_in_tkt.c,v 1.1.16.3 1996/11/13 18:07:46 arvind Exp $";
#endif	/* !lint & !SABER */

#include <krb5/copyright.h>
#include <krb5/krb5.h>
#include <krb5/preauth.h>
#include <krb5/asn1.h>
#include <krb5/los-proto.h>
#include <krb5/ext-proto.h>

#ifdef OSF_DCE_FEATURE
#include <sec_lrgy.h>
#endif

/* some typedef's for the function args to make things look a bit cleaner */

typedef krb5_error_code (*git_key_proc) PROTOTYPE((krb5_context,
						   const krb5_enctype,
						   krb5_data *,
						   krb5_const_pointer,
						   krb5_keyblock **));

typedef krb5_error_code (*git_decrypt_proc) PROTOTYPE((krb5_context,
						       const krb5_keyblock *,
						       krb5_const_pointer,
						       krb5_kdc_rep * ));
/*
 * This function sends a request to the KDC, and gets back a response;
 * the response is parsed into ret_err_reply or ret_as_reply if the
 * reponse is a KRB_ERROR or a KRB_AS_REP packet.  If it is some other
 * unexpected response, an error is returned.
 */
static krb5_error_code
send_as_request(context, request, time_now, ret_err_reply, ret_as_reply)
    krb5_context 		context;
    krb5_kdc_req		*request;
    krb5_timestamp 		time_now;
    krb5_error ** 		ret_err_reply;
    krb5_kdc_rep ** 		ret_as_reply;
{
    krb5_kdc_rep *as_reply = 0;
    krb5_error_code retval;
    krb5_data *packet;
    krb5_data reply;
    char k4_version;		/* same type as *(krb5_data::data) */

    reply.data = 0;
    
    /* encode & send to KDC */
    if ((retval = encode_krb5_as_req(request, &packet)) != 0)
	goto cleanup;

    k4_version = packet->data[0];
    retval = krb5_sendto_kdc(/*context, */packet, 
			     krb5_princ_realm(/*context, */request->client), &reply);
    krb5_free_data(/*context, */packet);
    if (retval) {
#ifdef OSF_DCE_FEATURE
        /* Fetch TGT from local registry if KDC is down */
        if (retval == KRB5_KDC_UNREACH) {
            error_status_t lst;

	    /* XXX not on refresh - how can we tell? */

	    sec_lrgy_krbtgt_lookup(request->client, &reply, &lst);
	    if (lst != error_status_ok)
	        /* Preserve KRB5_KDC_UNREACH error return */
		goto cleanup;

	    /*
             * The nonce in the local rgy TGT will be different than
             * the request, so later verification will fail. We set the 
             * request.nonce to 0 to let the caller know that this is a 
             * local rgy TGT and it should ignore the nonce. Ugly, but 
             * it works.
	     */
	    request->nonce = 0;
	    retval = 0; /* we've got our TGT, now let's decode it */
        } else
            goto cleanup;
#else
	goto cleanup;
#endif
    }

    /* now decode the reply...could be error or as_rep */
    if (krb5_is_krb_error(&reply)) {
	krb5_error *err_reply;

	if ((retval = decode_krb5_error(&reply, &err_reply)))
	    /* some other error code--??? */	    
	    goto cleanup;
    
#ifdef OSF_DCE_FEATURE
	/*
	 * deal with error code in krb5_error so clock skew problems
	 * can be reported as such
	 */
	if (labs(err_reply->stime - time_now) > 5*60) {
            retval = KRB5KRB_AP_ERR_SKEW;
	    goto cleanup;
        }
#endif
	if (ret_err_reply)
	    *ret_err_reply = err_reply;
	else
	    krb5_free_error(/*context, */err_reply);
	goto cleanup;
    }

    /*
     * Check to make sure it isn't a V4 reply.
     */
    if (!krb5_is_as_rep(&reply)) {
/* these are in <kerberosIV/prot.h> as well but it isn't worth including. */
#define V4_KRB_PROT_VERSION     4
#define V4_AUTH_MSG_ERR_REPLY   (5<<1)
        /* check here for V4 reply */
        unsigned int t_switch;

        /* From v4 g_in_tkt.c: This used to be
           switch (pkt_msg_type(rpkt) & ~1) {
           but SCO 3.2v4 cc compiled that incorrectly.  */
        t_switch = reply.data[1];
        t_switch &= ~1;

        if (t_switch == V4_AUTH_MSG_ERR_REPLY
            && (reply.data[0] == V4_KRB_PROT_VERSION
                || reply.data[0] == k4_version)) {
            retval = KRB5KRB_AP_ERR_V4_REPLY;
        } else {
	    retval = KRB5KRB_AP_ERR_MSG_TYPE;
	    goto cleanup;
	}
    }

    /* It must be a KRB_AS_REP message, or an bad returned packet */
    if ((retval = decode_krb5_as_rep(&reply, &as_reply)))
	/* some other error code ??? */
	goto cleanup;

    if (as_reply->msg_type != KRB5_AS_REP) {
	retval = KRB5KRB_AP_ERR_MSG_TYPE;
	krb5_free_kdc_rep(/*context, */as_reply);
	goto cleanup;
    }

    if (ret_as_reply)
	*ret_as_reply = as_reply;
    else
	krb5_free_kdc_rep(/*context, */as_reply);

cleanup:
    if (reply.data)
	free(reply.data);
    return retval;
}

static krb5_error_code
decrypt_as_reply(context, request, as_reply, key_proc, keyseed, key,
		 decrypt_proc, decryptarg)
    krb5_context 		context;
    krb5_kdc_req		*request;
    krb5_kdc_rep		*as_reply;
    git_key_proc 		key_proc;
    krb5_const_pointer 		keyseed;
    krb5_keyblock *		key;	
    git_decrypt_proc 		decrypt_proc;
    krb5_const_pointer 		decryptarg;
{
    krb5_error_code		retval;
    krb5_keyblock *		decrypt_key = 0;
    krb5_data 			salt;
    
    if (as_reply->enc_part2)
	return 0;

    if (key)
	    decrypt_key = key;
    else {
	if ((retval = krb5_principal2salt(/*context, */request->client, &salt)))
	    return(retval);
    
	retval = (*key_proc)(context, as_reply->enc_part.etype,
			     &salt, keyseed, &decrypt_key);
	krb5_xfree(salt.data);
	if (retval)
	    goto cleanup;
    }
    
    if ((retval = (*decrypt_proc)(context, decrypt_key, decryptarg, as_reply)))
	goto cleanup;

cleanup:
    if (!key && decrypt_key)
	krb5_free_keyblock(/*context, */decrypt_key);
    return (retval);
}

static krb5_error_code
verify_as_reply(context, time_now, request, as_reply)
    krb5_context 		context;
    krb5_timestamp 		time_now;
    krb5_kdc_req		*request;
    krb5_kdc_rep		*as_reply;
{
    krb5_error_code		retval;

#ifdef OSF_DCE_FEATURE
    int lrgy_flag = 0;
    /* 
     * A nonce of zero implies the TGT is from the local registry and we
     * should ignore the value when verifying.
     */
    if (!request->nonce) {
	request->nonce = as_reply->enc_part2->nonce;
	lrgy_flag = 1;
    }
#endif
    
    /* check the contents for sanity: */
    if (!as_reply->enc_part2->times.starttime)
	as_reply->enc_part2->times.starttime =
	    as_reply->enc_part2->times.authtime;
    
    if (!krb5_principal_compare(/*context, */as_reply->client, request->client)
	|| !krb5_principal_compare(/*context, */as_reply->enc_part2->server, request->server)
	|| !krb5_principal_compare(/*context, */as_reply->ticket->server, request->server)
	|| (request->nonce != as_reply->enc_part2->nonce)
	/* XXX check for extraneous flags */
	/* XXX || (!krb5_addresses_compare(context, addrs, as_reply->enc_part2->caddrs)) */
	|| ((request->from != 0) &&
	    (request->from != as_reply->enc_part2->times.starttime))
	|| ((request->till != 0) &&
	    (as_reply->enc_part2->times.endtime > request->till))
	|| ((request->kdc_options & KDC_OPT_RENEWABLE) &&
	    (request->rtime != 0) &&
	    (as_reply->enc_part2->times.renew_till > request->rtime))
	|| ((request->kdc_options & KDC_OPT_RENEWABLE_OK) &&
	    (as_reply->enc_part2->flags & KDC_OPT_RENEWABLE) &&
	    (request->till != 0) &&
	    (as_reply->enc_part2->times.renew_till > request->till))
	)
	return KRB5_KDCREP_MODIFIED;

#ifndef OSF_DCE_ARCHAISM
    if (context->library_options & KRB5_LIBOPT_SYNC_KDCTIME) {
	retval = krb5_set_real_time(context,
				    as_reply->enc_part2->times.authtime, 0);
	if (retval)
	    return retval;
    } else {
#endif
#ifdef OSF_DCE_FEATURE
    /* Don't check for clock skew if TGT is from local registry because
     * the starttime will probably be more than 5 minutes past.
     */
    if (!lrgy_flag)
#endif
	if ((request->from == 0) &&
	    (labs(as_reply->enc_part2->times.starttime - time_now)
	     > 5*60))
	    return (KRB5_KDCREP_SKEW);
#ifndef OSF_DCE_ARCHAISM
    }
#endif
    return 0;
}

static krb5_error_code
stash_as_reply(context, time_now, request, as_reply, creds, ccache)
    krb5_context 		context;
    krb5_timestamp 		time_now;
    krb5_kdc_req		*request;
    krb5_kdc_rep		*as_reply;
    krb5_creds * 		creds;
    krb5_ccache 		ccache;
{
    krb5_error_code 		retval;
    krb5_data *			packet;

    /* XXX issue warning if as_reply->enc_part2->key_exp is nearby */
	
    /* fill in the credentials */
    if ((retval = krb5_copy_keyblock_contents(/*context, */
					      as_reply->enc_part2->session,
					      &creds->keyblock)))
	return (retval);

    creds->times = as_reply->enc_part2->times;
    creds->is_skey = FALSE;		/* this is an AS_REQ, so cannot
					   be encrypted in skey */
    creds->ticket_flags = as_reply->enc_part2->flags;
    if ((retval = krb5_copy_addresses(/*context, */as_reply->enc_part2->caddrs,
				      &creds->addresses)))
	goto cleanup;

    creds->second_ticket.length = 0;
    creds->second_ticket.data = 0;

    if ((retval = encode_krb5_ticket(as_reply->ticket, &packet)))
	goto cleanup;

    creds->ticket = *packet;
    krb5_xfree(packet);

    /* store it in the ccache! */
    if (ccache) {
      if ((retval = krb5_cc_store_cred(/*context, */ccache, creds)))
	goto cleanup;
    }

cleanup:
    if (retval) {
	if (creds->keyblock.contents) {
	    memset((char *)creds->keyblock.contents, 0,
		   creds->keyblock.length);
	    krb5_xfree(creds->keyblock.contents);
	    creds->keyblock.contents = 0;
	    creds->keyblock.length = 0;
	}
	if (creds->ticket.data) {
	    krb5_xfree(creds->ticket.data);
	    creds->ticket.data = 0;
	}
	if (creds->addresses) {
	    krb5_free_addresses(/*context, */creds->addresses);
	    creds->addresses = 0;
	}
    }
    return (retval);
}

#ifdef OSF_DCE_FEATURE
krb5_error_code
make_preauth_list(context, ptypes, ret_list, pa_misc_data)
    krb5_context	context;
    krb5_preauthtype *	ptypes;
    krb5_pa_data ***	ret_list;
    krb5_pa_misc_data	***pa_misc_data;
{
    krb5_preauthtype *		ptypep;
    krb5_pa_data **		preauthp;
    krb5_pa_data **		preauth_to_use;
    int				i;
    krb5_pa_misc_data		**misc, **miscp;

    for (i=1, ptypep = ptypes; *ptypep; ptypep++, i++)
	;
    preauth_to_use = malloc(i * sizeof(krb5_pa_data *));
    if (preauth_to_use == NULL)
	return (ENOMEM);
    misc = malloc(i * sizeof(krb5_pa_misc_data *));
    if (misc == NULL) {
	krb5_free_pa_data(/*context, */preauth_to_use);
	return (ENOMEM);
    }
    for (preauthp = preauth_to_use, ptypep = ptypes, miscp = misc;
	 *ptypep;
	 preauthp++, ptypep++, miscp++) {
	*preauthp = malloc(sizeof(krb5_pa_data));
	if (*preauthp == NULL) {
	    krb5_free_pa_data(/*context, */preauth_to_use);
	    krb5_free_pa_misc_data(misc);
	    return (ENOMEM);
	}
	*miscp = malloc(sizeof(krb5_pa_misc_data));
	if (*miscp == NULL) {
	    krb5_free_pa_data(/*context, */preauth_to_use);
	    krb5_free_pa_misc_data(misc);
	    return (ENOMEM);
	}
#ifndef OSF_DCE_ARCHAISM
	(*preauthp)->magic = KV5M_PA_DATA;
#endif
	(*preauthp)->pa_type = (*miscp)->pa_type = *ptypep;
	(*preauthp)->length = 0;
	(*preauthp)->contents = 0;
	(*miscp)->data = 0;
	(*miscp)->free = 0;
    }
    *preauthp = NULL;
    *miscp = NULL;
    *ret_list = preauth_to_use;
    *pa_misc_data = misc;
    return 0;
}
#else
static krb5_error_code
make_preauth_list(context, ptypes, ret_list)
    krb5_context        context;
    krb5_preauthtype *  ptypes;
    krb5_pa_data ***    ret_list;
{
    krb5_preauthtype *          ptypep;
    krb5_pa_data **             preauthp;
    krb5_pa_data **             preauth_to_use;
    int                         i;

    for (i=1, ptypep = ptypes; *ptypep; ptypep++, i++)
        ;
    preauth_to_use = malloc(i * sizeof(krb5_pa_data *));
    if (preauth_to_use == NULL)
        return (ENOMEM);
    for (preauthp = preauth_to_use, ptypep = ptypes;
         *ptypep;
         preauthp++, ptypep++) {
        *preauthp = malloc(sizeof(krb5_pa_data));
        if (*preauthp == NULL) {
            krb5_free_pa_data(/*context, */preauth_to_use);
            return (ENOMEM);
        }
#ifndef OSF_DCE_ARCHAISM
        (*preauthp)->magic = KV5M_PA_DATA;
#endif
        (*preauthp)->pa_type = *ptypep;
        (*preauthp)->length = 0;
        (*preauthp)->contents = 0;
    }
    *preauthp = NULL;
    *ret_list = preauth_to_use;
    return 0;
}
#endif

#define MAX_IN_TKT_LOOPS 16

krb5_error_code
krb5_get_in_tkt(context, options, addrs, etypes, ptypes, key_proc, keyseed,
		decrypt_proc, decryptarg, creds, ccache, ret_as_reply)
    krb5_context context;
    const krb5_flags options;
    krb5_address * const * addrs;
    krb5_enctype * etypes;
    krb5_preauthtype * ptypes;
    git_key_proc key_proc;
    krb5_const_pointer keyseed;
    git_decrypt_proc decrypt_proc;
    krb5_const_pointer decryptarg;
    krb5_creds * creds;
    krb5_ccache ccache;
    krb5_kdc_rep ** ret_as_reply;
{
    krb5_error_code	retval = 0;
    krb5_timestamp	time_now;
    krb5_keyblock *	decrypt_key = 0;
    krb5_kdc_req	request;
    krb5_pa_data	**padata = 0;
    krb5_error *	err_reply;
    krb5_kdc_rep *	as_reply;
    krb5_pa_data  **	preauth_to_use = 0;
    int			loopcount = 0;
    krb5_int32		do_more = 0;
#ifdef OSF_DCE_FEATURE
    krb5_pa_misc_data	**preauth_misc_data = 0;
#endif

    if (! krb5_realm_compare(context, creds->client, creds->server))
	return KRB5_IN_TKT_REALM_MISMATCH;

    if (ret_as_reply)
	*ret_as_reply = 0;
    
    /*
     * Set up the basic request structure
     */
#ifndef OSF_DCE_ARCHAISM
    request.magic = KV5M_KDC_REQ;
#endif
    request.msg_type = KRB5_AS_REQ;
    request.addresses = 0;
    request.etype = 0;
    if (addrs)
	request.addresses = (krb5_address **) addrs;
    else
	if ((retval = krb5_os_localaddr(/*context, */&request.addresses)))
	    goto cleanup;
    request.padata = 0;
    request.kdc_options = options;
    request.client = creds->client;
    request.server = creds->server;
    request.from = creds->times.starttime;
    request.till = creds->times.endtime;
    request.rtime = creds->times.renew_till;
    request.etype = etypes;
    for (request.netypes = 0;request.etype[request.netypes];request.netypes++);
    request.authorization_data.ciphertext.length = 0;
    request.authorization_data.ciphertext.data = 0;
    request.unenc_authdata = 0;
    request.second_ticket = 0;
    
    /*
     * If a list of preauth types are passed in, convert it to a
     * preauth_to_use list.
     */
    if (ptypes) {
#ifdef OSF_DCE_FEATURE
	retval = make_preauth_list(context, ptypes, &preauth_to_use, &preauth_misc_data);
#else
	retval = make_preauth_list(context, ptypes, &preauth_to_use);
#endif
	if (retval)
	    goto cleanup;
    }
	    
    while (1) {
	if (loopcount++ > MAX_IN_TKT_LOOPS) {
	    retval = KRB5_GET_IN_TKT_LOOP;
	    goto cleanup;
	}

	err_reply = 0;
	as_reply = 0;

	if ((retval = krb5_timeofday(/*context, */&time_now)))
	    goto cleanup;

	/*
	 * XXX we know they are the same size... and we should do
	 * something better than just the current time
	 */
	request.nonce = (krb5_int32) time_now;

	/* free any padata from previous time thru loop */
        if (request.padata)
	    krb5_free_pa_data(/*context, */request.padata);
	request.padata = 0;
	if ((retval = krb5_obtain_padata(context, 
					 preauth_to_use, 
					 key_proc,
					 keyseed, 
					 creds, 
					 ccache, 
#ifdef OSF_DCE_FEATURE
					 preauth_misc_data,
#endif
					 &request)) != 0)
	    goto cleanup;
	if (preauth_to_use)
	    krb5_free_pa_data(/*context, */preauth_to_use);
	preauth_to_use = 0;
	
	if ((retval = send_as_request(context, &request, time_now, &err_reply,
				      &as_reply)))
	    goto cleanup;

	if (err_reply) {
#ifdef OSF_DCE_FEATURE
	    /*
             * Check for DCE specific errors
	     * XXX Should we just check on specific errors?
             * XXX Do non-default salts work with 1.03 based secd?
	     */
            if (preauth_misc_data)
	        krb5_free_pa_misc_data(preauth_misc_data);
	    preauth_misc_data = 0;
	    retval = krb5_decode_dce_edata(context, err_reply->e_data, retval,
				           ptypes, request, key_proc, keyseed,
					   &preauth_to_use, &preauth_misc_data);
	    if (retval)
		goto cleanup;
	    if (preauth_to_use) {
		krb5_free_error(/*context, */err_reply);
		err_reply = 0;
		continue;
            }
#endif
	    if (err_reply->error == KDC_ERR_PREAUTH_REQUIRED &&
		err_reply->e_data.length > 0) {
		retval = decode_krb5_padata_sequence(&err_reply->e_data,
						     &preauth_to_use);
		if (retval)
		    goto cleanup;
		krb5_free_error(/*context, */err_reply);
		err_reply = 0;
		continue;
	    } else {
		retval = err_reply->error + ERROR_TABLE_BASE_krb5;
		goto cleanup;
	    }
	} else if (!as_reply) {
	    retval = KRB5KRB_AP_ERR_MSG_TYPE;
	    goto cleanup;
	}
	if ((retval = krb5_process_padata(context, 
					  &request, 
					  as_reply,
					  key_proc, 
					  keyseed, 
					  decrypt_proc, 
					  &decrypt_key, 
					  creds, 
					  ccache,
#ifdef OSF_DCE_FEATURE
					  preauth_misc_data,
#endif
					  &do_more)) != 0)
	    goto cleanup;

	if (!do_more)
	    break;
    }
    
    if ((retval = decrypt_as_reply(context, &request, as_reply, key_proc,
				   keyseed, decrypt_key, decrypt_proc,
				   decryptarg))) {
#ifdef OSF_DCE_FEATURE
	/* 
	 * If we tried to decrypt the TGT using a decryption key obtained
         * from krb5_process_padata and failed, retry with the client's key. 
         * It's possible we are dealing with a pre-DCE 1.1 KDC and the TGT
         * is encrypted in the client's key. We could have fixed this
         * by having the server send back the padata type it processed in
         * the reply->padata array (and public key does do this), but it's 
         * much more important to maintain interoperability.
         */
	if (decrypt_key) {
            if ((retval = decrypt_as_reply(context, &request, as_reply, key_proc,
				           keyseed, NULL, decrypt_proc, decryptarg)))
		goto cleanup;
        } else
	    goto cleanup;
#else
	goto cleanup;
#endif
    }

    if ((retval = verify_as_reply(context, time_now, &request, as_reply)))
	goto cleanup;

    if ((retval = stash_as_reply(context, time_now, &request, as_reply,
				 creds, ccache)))
	goto cleanup;

cleanup:
    if (!etypes && request.etype)
	free(request.etype);
    if (!addrs && request.addresses)
	krb5_free_addresses(/*context, */request.addresses);
    if (request.padata)
	krb5_free_pa_data(/*context, */request.padata);
    if (padata)
	krb5_free_pa_data(/*context, */padata);
    if (preauth_to_use)
	krb5_free_pa_data(/*context, */preauth_to_use);
    if (decrypt_key)
	krb5_free_keyblock(/*context, */decrypt_key);
    if (as_reply) {
	if (ret_as_reply)
	    *ret_as_reply = as_reply;
	else
	    krb5_free_kdc_rep(/*context, */as_reply);
    }
    if (err_reply)
	krb5_free_error(/*context, */err_reply);
#ifdef OSF_DCE_FEATURE
    if (preauth_misc_data)
	krb5_free_pa_misc_data(preauth_misc_data);
#endif
    return (retval);
}
