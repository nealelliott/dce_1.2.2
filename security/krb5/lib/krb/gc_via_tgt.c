/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: gc_via_tgt.c,v $
 * Revision 1.1.8.3  1996/02/18  00:13:55  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:03:36  marty]
 *
 * Revision 1.1.8.2  1995/12/08  17:59:10  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/3  1995/08/22  21:07 UTC  mullan_s
 * 	Merge fix for CHFts15637
 * 
 * 	HP revision /main/HPDCE02/mullan_chfts15637/4  1995/08/15  17:10 UTC  mullan_s
 * 	Update calls to priv_krb_util_{parse|construct}_v1_{stkt|ptgt}_authdata.
 * 
 * 	HP revision /main/HPDCE02/mullan_chfts15637/3  1995/08/11  15:27 UTC  mullan_s
 * 	Add more comments.
 * 
 * 	HP revision /main/HPDCE02/mullan_chfts15637/2  1995/08/09  14:24 UTC  mullan_s
 * 	asd
 * 
 * 	HP revision /main/HPDCE02/mullan_chfts15637/1  1995/08/02  14:47 UTC  mullan_s
 * 	Initial work to copy uuid to STKT authorization data.
 * 
 * 	HP revision /main/HPDCE02/2  1995/04/15  00:03 UTC  rps
 * 	merge up to nightly
 * 	[1995/04/10  17:46 UTC  rps  /main/MOTHRA_panic/2]
 * 
 * 	HP revision /main/HPDCE02/1  1995/04/03  22:12 UTC  mullan_s
 * 	Binary Compatibility Merge
 * 
 * 	HP revision /main/mullan_mothra_bin_compat2/1  1995/02/01  22:22 UTC  mullan_s
 * 	Change DCE authdata code to only copy a uuid to service ticket
 * 	authdata.
 * 	[1995/12/08  16:59:49  root]
 * 
 * Revision 1.1.4.2  1994/08/18  20:25:34  greg
 * 	Slam the window on race conditions in EPTGT and application
 * 	service ticket acquisition that might result in "mismatched
 * 	seal" errors.
 * 	[1994/08/16  19:20:51  greg]
 * 
 * Revision 1.1.5.2  94/08/14  15:01:05  greg
 * 	Still testing...
 * 
 * Revision 1.1.4.1  1994/05/11  19:22:15  ahop
 * 	hp_sec_to_osf_1 drop
 * 	include int-proto.h with <> instead of ""
 * 	[1994/05/03  19:32:59  ahop]
 * 
 * Revision 1.1.2.2  1992/12/29  15:55:47  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:14:11  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:51:47  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  gc_via_tgt.c V=5 10/24/91 //littl/prgy/krb5/lib/krb
**
** Copyright (c) Hewlett-Packard Company 1991, 1995
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/krb/gc_via_tgt.c,v $
 * $Author: marty $
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
 * Given a tgt, and a target cred, get it.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_gcvtgt_c[] =
"$Id: gc_via_tgt.c,v 1.1.8.3 1996/02/18 00:13:55 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>

#include <krb5/asn1.h>

#include <krb5/los-proto.h>
#include <krb5/ext-proto.h>
#include <int-proto.h>
#ifdef OSF_DCE
#include <dce/sec_base.h>  /* contains architectural names of security principals */
#include <priv_krb_util.h>
#endif

krb5_error_code
krb5_get_cred_via_tgt (DECLARG(krb5_creds *, tgt),
		       DECLARG(const krb5_flags, kdcoptions),
		       DECLARG(const krb5_enctype, etype),
		       DECLARG(const krb5_cksumtype, sumtype),
		       DECLARG(krb5_creds *, cred))
OLDDECLARG(krb5_creds *, tgt)
OLDDECLARG(const krb5_flags, kdcoptions)
OLDDECLARG(const krb5_enctype, etype)
OLDDECLARG(const krb5_cksumtype, sumtype)
OLDDECLARG(krb5_creds *, cred)
{
    krb5_error_code retval;
    krb5_principal tempprinc;
    krb5_data *scratch;
    krb5_kdc_rep *dec_rep;
    krb5_error *err_reply;
    krb5_response tgsrep;

#ifdef OSF_DCE
    /*
     * If tgt->client is the architectural DCE privilege server
     * principal, we must remove any DCE authorization data that may exist
     * in the cred->authdata array. If we don't, it will get sealed
     * in the authorization data of the service ticket, which will
     * will be unparseable. If this routine is called via
     * krb5_get_credentials_dce, krb5_get_cred_from_kdc_dce, then there 
     * should be DCE authorization data.
     */
    char  *unparsed_client = NULL, *p = NULL;
    int dcepriv = false;

    retval = krb5_unparse_name(tgt->client, &unparsed_client);
    if ((retval == 0) && unparsed_client != NULL) {
        p = strchr(unparsed_client, '@');
        if (p) {
            *p = '\0';
            if (strcmp(unparsed_client, SEC_PRIV_SERVER_NAME) == 0) {
                krb5_authdata **tmp_authdata = NULL;

                dcepriv = true;
                priv_krb_util_delete_dce_authdata(cred->authdata, &tmp_authdata,
                                                  (error_status_t *) &retval);
                if (retval == 0 && cred->authdata) {
		    krb5_free_authdata(cred->authdata);
                    cred->authdata = tmp_authdata;
                }
            }
        }
        if (unparsed_client != NULL)
            free(unparsed_client);
    }
#endif /* OSF_DCE */
    

    /* tgt->client must be equal to cred->client */

    if (!krb5_principal_compare(tgt->client, cred->client))
	return KRB5_PRINC_NOMATCH;

    if (!tgt->ticket.length)
	return(KRB5_NO_TKT_SUPPLIED);

    /* check if we have the right TGT                    */
    /* tgt->server must be equal to                      */
    /* krbtgt/realmof(cred->server)@realmof(tgt->server) */

    if(retval = krb5_tgtname(krb5_princ_realm(cred->server),
			     krb5_princ_realm(tgt->server), &tempprinc))
	return(retval);
    if (!krb5_principal_compare(tempprinc, tgt->server)) {
	krb5_free_principal(tempprinc);
	return KRB5_PRINC_NOMATCH;
    }
    krb5_free_principal(tempprinc);


    if (retval = krb5_send_tgs(kdcoptions, &cred->times, etype, sumtype,
			       cred->server,
			       tgt->addresses,
			       cred->authdata,
			       0,		/* no padata */
			       0,		/* no second ticket */
			       tgt, &tgsrep))
	return retval;

#undef cleanup
#define cleanup() free(tgsrep.response.data)

    switch (tgsrep.message_type) {
    case KRB5_TGS_REP:
	break;
    case KRB5_ERROR:
    default:
	if (!krb5_is_krb_error(&tgsrep.response)) {
	    retval = KRB5KRB_AP_ERR_MSG_TYPE;
	} else
	    retval = decode_krb5_error(&tgsrep.response, &err_reply);
	if (retval) {
	    cleanup();
	    return retval;		/* neither proper reply nor error! */
	}

#if 0
	/* XXX need access to the actual assembled request...
	   need a change to send_tgs */
	if ((err_reply->ctime != request.ctime) ||
	    !krb5_principal_compare(err_reply->server, request.server) ||
	    !krb5_principal_compare(err_reply->client, request.client))
	    retval = KRB5_KDCREP_MODIFIED;
	else
#endif
	    retval = err_reply->error + ERROR_TABLE_BASE_krb5;

	krb5_free_error(err_reply);
	cleanup();
	return retval;
    }
    retval = krb5_decode_kdc_rep(&tgsrep.response,
				 &tgt->keyblock,
				 etype, /* enctype */
				 &dec_rep);
    cleanup();
    if (retval)
	return retval;
#undef cleanup
#define cleanup() {\
	memset((char *)dec_rep->enc_part2->session->contents, 0,\
	      dec_rep->enc_part2->session->length);\
		  krb5_free_kdc_rep(dec_rep); }

    if (dec_rep->msg_type != KRB5_TGS_REP) {
	retval = KRB5KRB_AP_ERR_MSG_TYPE;
	cleanup();
	return retval;
    }
    
    /* now it's decrypted and ready for prime time */

    if (!krb5_principal_compare(dec_rep->client, tgt->client)) {
	cleanup();
	return KRB5_KDCREP_MODIFIED;
    }
    /* put pieces into cred-> */
    if (cred->keyblock.contents) {
	memset(&cred->keyblock.contents, 0, cred->keyblock.length);
	krb5_xfree(cred->keyblock.contents);
    }
    if (retval = krb5_copy_keyblock_contents(dec_rep->enc_part2->session,
					     &cred->keyblock)) {
	cleanup();
	return retval;
    }
    memset((char *)dec_rep->enc_part2->session->contents, 0,
	  dec_rep->enc_part2->session->length);

#undef cleanup
#define cleanup() {\
	memset((char *)cred->keyblock.contents, 0, cred->keyblock.length);\
		  krb5_free_kdc_rep(dec_rep); }

    cred->times = dec_rep->enc_part2->times;

#if 0
    /* XXX probably need access to the request */
    /* check the contents for sanity: */
    if (!krb5_principal_compare(dec_rep->client, request.client)
	|| !krb5_principal_compare(dec_rep->enc_part2->server, request.server)
	|| !krb5_principal_compare(dec_rep->ticket->server, request.server)
	|| (request.nonce != dec_rep->enc_part2->nonce)
	/* XXX check for extraneous flags */
	/* XXX || (!krb5_addresses_compare(addrs, dec_rep->enc_part2->caddrs)) */
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

    if ((request.from == 0) &&
	!in_clock_skew(dec_rep->enc_part2->times.starttime))
	retval = KRB5_KDCREP_SKEW;
    
    if (retval) {
	cleanup();
	return retval;
    }
    
#endif

    cred->ticket_flags = dec_rep->enc_part2->flags;
    cred->is_skey = FALSE;
    if (cred->addresses) {
	krb5_free_addresses(cred->addresses);
    }
    if (dec_rep->enc_part2->caddrs) {
	if (retval = krb5_copy_addresses(dec_rep->enc_part2->caddrs,
					 &cred->addresses)) {
	    cleanup();
	    return retval;
	}
    } else {
	/* no addresses in the list means we got what we had */
	if (retval = krb5_copy_addresses(tgt->addresses,
					 &cred->addresses)) {
	    cleanup();
	    return retval;
	}
    }
    /*
     * Free cred->server before overwriting it.
     */
    if (cred->server)
	krb5_free_principal(cred->server);
    if (retval = krb5_copy_principal(dec_rep->enc_part2->server,
				     &cred->server)) {
	cleanup();
	return retval;
    }

    if (retval = encode_krb5_ticket(dec_rep->ticket, &scratch)) {
	cleanup();
	krb5_free_addresses(cred->addresses);
	return retval;
    }

    cred->ticket = *scratch;
    krb5_xfree(scratch);

#ifdef OSF_DCE
    /*
     * If tgt->client is the architectural DCE privilege server
     * principal, then copy the uuid, if any, contained in the
     * DCE version 1 specific authdata of tgt->authdata into the service
     * ticket authorization data. The uuid pair will prove that
     * this STKT and PTGT are in synchronization and will not
     * cause mismatched seal or who are you errors. Any additional
     * krb5 authdata contained in the service ticket prior to the
     * remote request is preserved. 
     *
     * It is an egregious violation of layering to be doing this
     * down in the bowels of the kerberos library, but the alternative
     * is to contrive some inter-process synchronization of DCE
     * credentials operations, which would be much costlier, in
     * development effort, and at runtime.
     *
     * NOTE WELL!
     * We are taking over creds->authdata.  Currently (DCE 1.1 development)
     * we know that it is always constructed with a version 1 format before calls to
     * krb5_get_credentials() from within the DCE runtime.  And since this
     * routine already mucks with other fields of the input cred block
     * for generic krb5 purposes, the caller *must* already be providing
     * concurrency control, since multiple, concurrent calls to any krb5 credential
     * acquisition routine with a single instance of a creds structure would
     * be courting disaster ...
     */
    if (dcepriv) {
        krb5_authdata **tmp_authdata = NULL;
        krb5_error_code lret;
	uuid_t	ptgt_uuid;

	/* Try to find a uuid */
	priv_krb_util_parse_v1_ptgt_authdata(tgt->authdata, 
		NULL, NULL, &ptgt_uuid, (error_status_t *)&lret);
	/* 
	 * ignore errors from above. Even if it is not the
	 * correct version or it does not have a uuid, we
	 * still want to try to construct a v1 stkt authdata.
	 */ 
        priv_krb_util_construct_v1_stkt_authdata(&ptgt_uuid, 
		cred->authdata, &tmp_authdata, (error_status_t *)&lret);

        if ((lret == 0) && tmp_authdata) {
            if (cred->authdata)
                krb5_free_authdata(cred->authdata);
	    cred->authdata = tmp_authdata;
        }
    }
#endif /* OSF_DCE */

    krb5_free_kdc_rep(dec_rep);
    return retval;
}
