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
 * $Log: gc_via_tkt.c,v $
 * Revision 1.1.2.1  1996/06/05  21:01:23  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  19:58:43  arvind]
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
 * $Log: gc_via_tkt.c,v $
 * Revision 1.1.2.1  1996/06/05  21:01:23  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  19:58:43  arvind]
 *
 * Revision 1.1.1.2  1996/06/05  19:58:43  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 *
 * Revision /main/DCE_1.2.2/2  1996/05/30  21:19 UTC  mullan_s
 * 	Merge krb5 work to DCE1.2.2 mainline
 * 
 * Revision /main/DCE_1.2.2/mullan_dce1.2.2_krb5_drop/1  1996/05/29  20:44 UTC  mullan_s
 * 	Merge krb5 work from DCE_1.2 to DCE_1.2.2
 * 
 * Revision /main/DCE_1.2/mullan_dce_k5_bsd44_rutils/1  1996/05/03  20:07 UTC  mullan_s
 * 	Add OSF/HP copyrights & RCS header.
 * 
 * $EndLog$
 */
/*
 * lib/krb5/krb/gc_via_tgt.c
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
 * Given a tkt, and a target cred, get it.
 * Assumes that the kdc_rep has been decrypted.
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/

#include "k5-int.h"
#include "int-proto.h"

static krb5_error_code
krb5_kdcrep2creds(context, pkdcrep, address, psectkt, ppcreds)
    krb5_context          context;
    krb5_kdc_rep        * pkdcrep;
    krb5_address *const * address;
    krb5_data		* psectkt;
    krb5_creds         ** ppcreds;
{
    krb5_error_code retval;  
    krb5_data *pdata;
  
    if ((*ppcreds = (krb5_creds *)malloc(sizeof(krb5_creds))) == NULL) {
        return ENOMEM;
    }

    memset(*ppcreds, 0, sizeof(krb5_creds));

    if (retval = krb5_copy_principal(context, pkdcrep->client,
                                     &(*ppcreds)->client))
        goto cleanup;

    if (retval = krb5_copy_principal(context, pkdcrep->enc_part2->server,
                                     &(*ppcreds)->server))
        goto cleanup;

    if (retval = krb5_copy_keyblock_contents(context, 
					     pkdcrep->enc_part2->session,
                                             &(*ppcreds)->keyblock))
        goto cleanup;

    if (retval = krb5_copy_data(context, psectkt, &pdata))
	goto cleanup;
    (*ppcreds)->second_ticket = *pdata;
    krb5_xfree(pdata);

    (*ppcreds)->keyblock.etype = pkdcrep->ticket->enc_part.etype;
    (*ppcreds)->ticket_flags = pkdcrep->enc_part2->flags;
    (*ppcreds)->times = pkdcrep->enc_part2->times;
    (*ppcreds)->magic = KV5M_CREDS;

    (*ppcreds)->authdata = NULL;   			/* not used */
    (*ppcreds)->is_skey = 0;    			/* not used */

    if (pkdcrep->enc_part2->caddrs) {
	if (retval = krb5_copy_addresses(context, pkdcrep->enc_part2->caddrs,
					 &(*ppcreds)->addresses)) 
	    goto cleanup_keyblock;
    } else {
	/* no addresses in the list means we got what we had */
	if (retval = krb5_copy_addresses(context, address,
					 &(*ppcreds)->addresses)) 
	    goto cleanup_keyblock;
    }

    if (retval = encode_krb5_ticket(pkdcrep->ticket, &pdata))
	goto cleanup_keyblock;

    (*ppcreds)->ticket = *pdata;
    return 0;

cleanup_keyblock:
    memset((*ppcreds)->keyblock.contents, 0, (*ppcreds)->keyblock.length);

cleanup:
    free (*ppcreds);
    return retval;
}
 
krb5_error_code INTERFACE
krb5_get_cred_via_tkt (context, tkt, kdcoptions, address, in_cred, out_cred)
    krb5_context 	  context;
    krb5_creds 		* tkt;
    const krb5_flags 	  kdcoptions;
    krb5_address *const * address;
    krb5_creds 		* in_cred;
    krb5_creds 	       ** out_cred;
{
    krb5_error_code retval;
    krb5_kdc_rep *dec_rep;
    krb5_error *err_reply;
    krb5_response tgsrep;

    /* tkt->client must be equal to in_cred->client */
    if (!krb5_principal_compare(context, tkt->client, in_cred->client))
	return KRB5_PRINC_NOMATCH;

    if (!tkt->ticket.length)
	return KRB5_NO_TKT_SUPPLIED;

    if ((kdcoptions & KDC_OPT_ENC_TKT_IN_SKEY) && 
	(!in_cred->second_ticket.length))
        return(KRB5_NO_2ND_TKT);


    /* check if we have the right TGT                    */
    /* tkt->server must be equal to                      */
    /* krbtgt/realmof(cred->server)@realmof(tgt->server) */
/*
    {
    krb5_principal tempprinc;
        if (retval = krb5_tgtname(context, 
		     krb5_princ_realm(context, in_cred->server),
		     krb5_princ_realm(context, tkt->server), &tempprinc))
    	    return(retval);

        if (!krb5_principal_compare(context, tempprinc, tkt->server)) {
            krb5_free_principal(context, tempprinc);
	    return (KRB5_PRINC_NOMATCH);
        }
    krb5_free_principal(context, tempprinc);
    }
*/

    if (retval = krb5_send_tgs(context, kdcoptions, &in_cred->times, NULL, 
			       in_cred->server, address, in_cred->authdata,
			       0,		/* no padata */
			       (kdcoptions & KDC_OPT_ENC_TKT_IN_SKEY) ? 
#ifdef OSF_DCE
				&in_cred->second_ticket : (krb5_data *) NULL,
#else
				&in_cred->second_ticket : NULL,
#endif
			       tkt, &tgsrep))
	return retval;

    switch (tgsrep.message_type) {
    case KRB5_TGS_REP:
	break;
    case KRB5_ERROR:
    default:
	if (krb5_is_krb_error(&tgsrep.response))
	    retval = decode_krb5_error(&tgsrep.response, &err_reply);
	else
	    retval = KRB5KRB_AP_ERR_MSG_TYPE;

	if (retval) 			/* neither proper reply nor error! */
	    goto error_4;

#if 0
	/* XXX need access to the actual assembled request...
	   need a change to send_tgs */
	if ((err_reply->ctime != request.ctime) ||
	    !krb5_principal_compare(context,err_reply->server,request.server) ||
	    !krb5_principal_compare(context, err_reply->client, request.client))
	    retval = KRB5_KDCREP_MODIFIED;
	else
#endif
	    retval = err_reply->error + ERROR_TABLE_BASE_krb5;

	krb5_free_error(context, err_reply);
	goto error_4;
    }

    if (retval = krb5_decode_kdc_rep(context, &tgsrep.response, &tkt->keyblock,
				     tkt->keyblock.etype, &dec_rep))
	goto error_4;

    if (dec_rep->msg_type != KRB5_TGS_REP) {
	retval = KRB5KRB_AP_ERR_MSG_TYPE;
	goto error_3;
    }
    
    /* now it's decrypted and ready for prime time */
    if (!krb5_principal_compare(context, dec_rep->client, tkt->client)) {
	retval = KRB5_KDCREP_MODIFIED;
	goto error_3;
    }

#if 0
    /* XXX probably need access to the request */
    /* check the contents for sanity: */
    if (!krb5_principal_compare(context, dec_rep->client, request.client)
	|| !krb5_principal_compare(context, dec_rep->enc_part2->server, request.server)
	|| !krb5_principal_compare(context, dec_rep->ticket->server, request.server)
	|| (request.nonce != dec_rep->enc_part2->nonce)
	/* XXX check for extraneous flags */
	/* XXX || (!krb5_addresses_compare(context, addrs, dec_rep->enc_part2->caddrs)) */
	|| ((request.from != 0) &&
	    (request.from != dec_rep->enc_part2->times.starttime))
	|| ((request.till != 0) &&
	    (dec_rep->enc_part2->times.endtime > request.till))
	|| ((request.kdc_options & KDC_OPT_RENEWABLE) &&
	    (request.rtime != 0) &&
	    (dec_rep->enc_part2->times.renew_till > request.rtime))
	|| ((request.kdc_options & KDC_OPT_RENEWABLE_OK) &&
	    (dec_rep->enc_part2->flags & KDC_OPT_RENEWABLE) &&
	    (request.till != 0) &&
	    (dec_rep->enc_part2->times.renew_till > request.till))
	)
	retval = KRB5_KDCREP_MODIFIED;

    if (!request.from && !in_clock_skew(dec_rep->enc_part2->times.starttime)) {
	retval = KRB5_KDCREP_SKEW;
	goto error_3;
    }
    
#endif

    retval = krb5_kdcrep2creds(context, dec_rep, address, 
			       &in_cred->second_ticket,  out_cred);

error_3:;
    memset(dec_rep->enc_part2->session->contents, 0,
	   dec_rep->enc_part2->session->length);
    krb5_free_kdc_rep(context, dec_rep);

error_4:;
    free(tgsrep.response.data);
    return retval;
}
