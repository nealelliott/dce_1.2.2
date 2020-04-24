/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: gc_2tgt.c,v $
 * Revision 1.1.11.1  1996/06/04  22:03:36  arvind
 * 	Merge AUTH_DATA cleanup code from mb_u2u
 * 	[1996/05/06  20:47 UTC  burati  /main/DCE_1.2/1]
 *
 * 	merge u2u  work
 * 	[1996/04/29  21:34 UTC  burati  /main/mb_u2u/1]
 *
 * 	u2u work off Mothra, before 1.2 branch is available
 * 	[1996/04/29  21:32 UTC  burati  /main/mb_mothra8/1]
 *
 * Revision 1.1.9.2  1996/02/18  00:13:51  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:03:33  marty]
 * 
 * Revision 1.1.9.1  1995/12/08  17:59:00  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:59:47  root]
 * 
 * Revision 1.1.5.1  1994/05/11  19:21:26  ahop
 * 	hp_sec_to_osf_1 drop
 * 	include int-proto.h with <> instead of ""
 * 	[1994/05/03  19:31:40  ahop]
 * 
 * Revision 1.1.3.3  1992/12/29  15:55:34  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:14:03  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/29  21:19:42  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:33:52  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:51:24  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  gc_2tgt.c
**
** Copyright (c) Hewlett-Packard Company 1991, 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/krb/gc_2tgt.c,v $
 * $Author: arvind $
 *
 * Copyright 1991 by the Massachusetts Institute of Technology.
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
 * Given two tgts, get a ticket.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_gcv2tgt_c[] = "$Id: gc_2tgt.c,v 1.1.11.1 1996/06/04 22:03:36 arvind Exp $";
#endif

#include <krb5/krb5.h>
#include <krb5/asn1.h>		/* needed for some macros */

#include <krb5/los-proto.h>
#include <krb5/ext-proto.h>
#include <int-proto.h>
#ifdef OSF_DCE
#include <dce/sec_base.h>  /* contains architectural names of security principals */
#include <priv_krb_util.h>
#endif

krb5_error_code
#if (__STDC__ || defined(__STDC_ZERO_OK__)) && defined(NARROW_PROTOTYPES)
krb5_get_cred_via_2tgt (krb5_creds * tgt,
			const krb5_flags kdcoptions,
			const krb5_enctype etype,
			const krb5_cksumtype sumtype,
			register krb5_creds *cred)
#else
krb5_get_cred_via_2tgt (tgt, kdcoptions, etype, sumtype, cred)
krb5_creds *tgt;
const krb5_flags kdcoptions;
const krb5_enctype etype;
const krb5_cksumtype sumtype;
register krb5_creds * cred;
#endif
{
    krb5_error_code retval;
#if 0
    krb5_principal tempprinc;
#endif
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
    /* tgt->server must be equal to krbtgt/realmof(cred->client) */
    if (!krb5_principal_compare(tgt->client, cred->client))
	return KRB5_PRINC_NOMATCH;

    if (!tgt->ticket.length)
	return(KRB5_NO_TKT_SUPPLIED);

    if (!cred->second_ticket.length)
	return(KRB5_NO_2ND_TKT);

#if 0	/* What does this do? */
    if (retval = krb5_tgtname(krb5_princ_realm(cred->server),
			      krb5_princ_realm(cred->client), &tempprinc))
	return(retval);

    if (!krb5_principal_compare(tempprinc, tgt->server)) {
	krb5_free_principal(tempprinc);
	return KRB5_PRINC_NOMATCH;
    }
    krb5_free_principal(tempprinc);
#endif

    if (!(kdcoptions & KDC_OPT_ENC_TKT_IN_SKEY))
	return KRB5_INVALID_FLAGS;

    if (retval = krb5_send_tgs(kdcoptions, &cred->times, etype, sumtype,
			       cred->server,
			       tgt->addresses,
			       cred->authdata,
			       0,		/* no padata */
			       &cred->second_ticket,
			       tgt, &tgsrep))
	return retval;

    if (tgsrep.message_type != KRB5_TGS_REP)
      {
	if (!krb5_is_krb_error(&tgsrep.response)) {
	    free(tgsrep.response.data);
	    return KRB5KRB_AP_ERR_MSG_TYPE;
	}
	retval = decode_krb5_error(&tgsrep.response, &err_reply);
	if (retval) {
	    free(tgsrep.response.data);
	    return retval;
	}
	retval = err_reply->error + ERROR_TABLE_BASE_krb5;

	krb5_free_error(err_reply);
	free(tgsrep.response.data);
	return retval;
      }
    retval = krb5_decode_kdc_rep(&tgsrep.response, &tgt->keyblock,
				 etype, &dec_rep);
    free(tgsrep.response.data);
    if (retval)
	return retval;

#undef cleanup
#define cleanup() {\
	memset((char *)dec_rep->enc_part2->session->contents, 0,\
	      dec_rep->enc_part2->session->length);\
		  krb5_free_kdc_rep(dec_rep); }

    if (dec_rep->msg_type != KRB5_TGS_REP) {
	cleanup();
	return KRB5KRB_AP_ERR_MSG_TYPE;
    }
    
    /* now it's decrypted and ready for prime time */

    if (!krb5_principal_compare(dec_rep->client, tgt->client)) {
	cleanup();
	return KRB5_KDCREP_MODIFIED;
    }
    /* put pieces into cred-> */
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

    /* Should verify that the ticket is what we asked for. */
    cred->times = dec_rep->enc_part2->times;
    cred->ticket_flags = dec_rep->enc_part2->flags;
    cred->is_skey = TRUE;
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
    xfree(scratch);

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
    return 0;
}

/*
 * Local variables:
 * mode:c
 * eval: (make-local-variable (quote c-indent-level))
 * eval: (make-local-variable (quote c-continued-statement-offset))
 * eval: (setq c-indent-level 4 c-continued-statement-offset 4)
 * End:
 */

