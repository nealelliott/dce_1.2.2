/*
 * @OSF_COPYRIGHT@
 */
/*
 * HISTORY
 * $Log: sec_krb_u2u.c,v $
 * Revision 1.1.2.2  1996/08/09  12:12:36  arvind
 * 	Merge code review cleanup from mb_u2u2
 * 	[1996/07/03  19:30 UTC  burati  /main/DCE_1.2.2/mb_u2u2/1]
 *
 * 	u2u cleanup (rtn for checking u2u status code)
 * 	[1996/05/24  23:28 UTC  sommerfeld  /main/DCE_1.2.2/1]
 *
 * Revision 1.1.2.1  1996/06/04  22:05:07  arvind
 * 	Merge code from mb_u2u
 * 	[1996/05/06  21:01 UTC  burati  /main/DCE_1.2/1]
 * 
 * 	New module for u2u code
 * 	[1996/05/02  19:22 UTC  burati  /main/mb_u2u/1]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1995, 1996
 * Unpublished work. All Rights Reserved.
 */

/* DCE security/Kerberos User to User authentication utilities
 * 
 * Includes source from: /mit/krb5/.cvsroot/src/appl/user_user/client.c
 * Copyright 1991 by the Massachusetts Institute of Technology.
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
 */

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>
#include <krb5/los-proto.h>
#include <com_err.h>
#include <sec_authnp.h>
#include <sec_login_pvt.h>

#define CLKSKW 300      /* KRB clock skew */

extern int sec__n_idents;
extern sec_krb_server_t *sec__serv_id_table;

/* sec_krb_u2u_keyproc
 * 
 * TGT based keyproc for passing to decoding routine (krb5_rd_req()) in place
 * of keytab based sec_krb_keyproc() (keytable based keyproc) when u2u in use.
 */
krb5_error_code sec_krb_u2u_keyproc(
    krb5_pointer    keyprocarg,
    krb5_principal  principal,
    krb5_keytype    keytype,
    krb5_kvno       vno,
    krb5_keyblock   **key
)
{
    krb5_creds *creds = (krb5_creds *)keyprocarg;

    return krb5_copy_keyblock(&creds->keyblock, key);
}


/* sec_krb_u2u_register_server
 * 
 * Similar to the ktab registration rtn sec_krb_register_server in sec_authn.c
 * 
 * Register a principal and information on where to get its TGT for u2u.
 * If we re-register an existing principal, just replace the existing
 * registration information.
 */
error_status_t sec_krb_u2u_register_server(
    unsigned_char_p_t   server_princ_name,
    sec_login_handle_t  lh
)
{
    unsigned_char_p_t server_global;
    error_status_t st;
    int i;
    sec_krb_server_t *nid;
    sec_login_context_t *lcp = (sec_login_context_t *) lh;

    /* Don't even attempt to register via u2u if svr princ isn't allowed to */
    if (!FLAG_SET(POLICY_INFO(lcp).pflags, policy_user_to_user)) {
        return sec_s_user_to_user_disabled;
    }

    CLEAR_STATUS(&st);
    LOCK_IDENTITY();

    /* We don't support multiple u2u server registrations */
    if (sec__n_idents > 0) {
        return sec_s_multiple_u2u_reg;
    }

    /* We don't support both keytab registration and u2u, so check first */
    if (sec__krb_server_type == sec_krb_svr_type_ktab) {
        UNLOCK_IDENTITY();
        return sec_s_svr_type_conflict;
    } else {
        sec__krb_server_type = sec_krb_svr_type_u2u;
    }

    server_global = (unsigned_char_p_t) malloc(strlen((char *) lcp->cell) +
        strlen((char *) lcp->principal) + 2);
    sprintf((char *)server_global, "%s/%s", lcp->cell, lcp->principal);
    if (st == rpc_s_ok) {

        /* Allocate table++ and copy old table to new before registering.
	 * We only support a single registration at this time, but the
	 * following code mirrors that of sec_krb_register_server (ktab)
	 * so that we could support multiple identities easier in the future
	 * if necessary.
	 */
        nid = (sec_krb_server_t *)
            malloc (sizeof(sec_krb_server_t) * (sec__n_idents + 1));
	memset(nid, 0, sizeof(sec_krb_server_t));
        memcpy (nid, sec__serv_id_table,
            sizeof(sec_krb_server_t) * sec__n_idents);
        free(sec__serv_id_table);

        /*
         * Resolve the keytab here if at all possible to save
         * ourselves significant effort later on.
         */
        sec__serv_id_table = nid;

        sec__serv_id_table[sec__n_idents].server_string =
            rpc_stralloc(server_princ_name);
        sec__serv_id_table[sec__n_idents].server_global = server_global;
        sec__serv_id_table[sec__n_idents].lh = lh;

        sec__n_idents++;

	/* Initialize sec__krb_key_seed from TGT */
        if (sec__krb_key_seed == NULL) { 
            sec__krb_key_seed = (mit_des_random_key_seed *)
                malloc (sizeof (mit_des_random_key_seed));
            if (sec__krb_key_seed == NULL)
            {
                st = sec_s_no_memory;
            } else {
                mit_des_cblock skey;
                st = sec_krb_u2u_get_tgt(lh, NULL, NULL, skey, NULL);
                if (GOOD_STATUS(&st)) {
                    mit_des_init_random_number_generator(skey,
                        sec__krb_key_seed);
		}
            }
        }
	
    }
    UNLOCK_IDENTITY();
    return st;
}


/* sec_krb_u2u_check_creds()
 * 
 * Refresh the server table's copy of the current TGT and the previous
 * one (one expiring within now - clockskew timeframe) if necessary.
 * 
 * Used by rpc mgmt call to inquire the TGT and server principal name,
 * and by sec_krb_decode_msg_common_frags() before it attempts to decode/
 * decrypt an incoming request.  This routine updates the sec__serv_id_table[]
 * if necessary, so that the callers can just rely on the information in the
 * table to do their work...
 * 
 * The table (sec__serv_id_table[]) will contain a copy of the current_creds
 * (server's TGT data, obtained when registered) up until 5 minutes before
 * they are due to expire.  Clients will get this same copy of the creds via
 * rpc_mgmt_inq_svr_princ_name_tgt() throughout this time period, even if
 * the server has refreshed its TGT multiple times.
 *
 * The current TGT is refreshed (new lookup from cred cache) 5 minutes before
 * expiration time, because it's possible that we're up to 5 minutes slower
 * than the KDC, and we don't want to hand the client TGT data that's expiring
 * immediately.  Because the time could be correct and a client could be using
 * a previously returned TGT up until the exact expiration time, we save a
 * copy of the current_creds to prev_creds in the table, before updating the
 * creds (kept around until 5 minutes after it claims it's expired though,
 * in case we're almost 5 minutes faster than the client and KDC).
 *
 * The authn decode/decrypt will attempt to use the current creds first.
 * If that fails, it will assume that we could be faster or slower than the
 * client and/or KDC and it will try the prev_creds.  If it's just that
 * the prev_creds are still good for a few minutes, the decode will work.
 * If it's that the prev_creds have really expired and the client attempted
 * to use it anyway (expiration after client obtained ticket, but before
 * call started) then this will successfully get a TKT_EXPIRED error to
 * return to the client (as opposed to INTEGRITY_ERROR, which is all we could
 * know if we didn't keep the prev_creds around for the clock_skew limit).
 */
error_status_t sec_krb_u2u_check_creds(void)
{
    krb5_timestamp now;
    error_status_t st = error_status_ok;

    st = krb5_timeofday(&now);

    /* If there's no current creds, fetch them from the cred cache */
    if (sec__serv_id_table[0].curr_creds == NULL) {
        sec__serv_id_table[0].curr_creds =
            (krb5_creds *)malloc(sizeof(krb5_creds));
        st = sec_krb_u2u_get_tgt(sec__serv_id_table[0].lh, NULL, NULL,
                NULL, sec__serv_id_table[0].curr_creds);
        if (st != error_status_ok)
            free((char *)sec__serv_id_table[0].curr_creds);

        /* Else, if current creds will have expired within the next five
	 * minutes (KRB architectural clock-skew), try to get new ones...
	 */
    } else if (sec__serv_id_table[0].curr_creds->times.endtime < (now+CLKSKW)){

        /* Toss out existing previous creds, if they exist */
        if (sec__serv_id_table[0].prev_creds != NULL) {
            krb5_creds *tmpcreds;
            krb5_free_cred_contents(sec__serv_id_table[0].prev_creds);
            memset(sec__serv_id_table[0].prev_creds, 0, sizeof(krb5_creds));

	    /* only save curr creds to previous if not expired > clock_skew */
	    if (sec__serv_id_table[0].curr_creds->times.endtime < (now-CLKSKW)){
                krb5_free_cred_contents(sec__serv_id_table[0].curr_creds);
                free((char *)sec__serv_id_table[0].prev_creds);
		sec__serv_id_table[0].prev_creds = NULL;
	    } else {
                /* Swap mem locations, to reuse old prev creds memory */
                tmpcreds = sec__serv_id_table[0].prev_creds;
                sec__serv_id_table[0].prev_creds =
                    sec__serv_id_table[0].curr_creds;
                sec__serv_id_table[0].curr_creds = tmpcreds;
            }
        } else {
            /* or, there were no previous creds so save curr and alloc more */
            sec__serv_id_table[0].prev_creds =
                sec__serv_id_table[0].curr_creds;

            /* Allocate space for current creds if possible */
            if ((sec__serv_id_table[0].curr_creds =
                (krb5_creds *)malloc(sizeof(krb5_creds))) == NULL) {
                sec__serv_id_table[0].curr_creds =
                    sec__serv_id_table[0].prev_creds;
		sec__serv_id_table[0].prev_creds = NULL;
                return sec_s_no_memory;
            }
	}

        /* Finally, fill in the new creds */
        st = sec_krb_u2u_get_tgt(sec__serv_id_table[0].lh, NULL, NULL,
                NULL, sec__serv_id_table[0].curr_creds);

        /* If we couldn't do that, put the old ones back for now */
        if (st != error_status_ok) {
            free((char *)sec__serv_id_table[0].curr_creds);
            sec__serv_id_table[0].curr_creds =
                sec__serv_id_table[0].prev_creds;
	    sec__serv_id_table[0].prev_creds = NULL;
        }
    } else if ((sec__serv_id_table[0].prev_creds != NULL) &&
	(sec__serv_id_table[0].prev_creds->times.endtime < (now - CLKSKW)) ) {

        /* No new cred stuff to do above, but still need to make sure prev
	 * creds are worth keeping around.  Purge if expired more than clock
	 * skew limit ago, so they can't possibly be used after that point.
	 */
        krb5_free_cred_contents(sec__serv_id_table[0].prev_creds);
        free((char *)sec__serv_id_table[0].prev_creds);
	sec__serv_id_table[0].prev_creds = NULL;
    }

    /* If none of the above blocks were executed, then everything's
     * up to date and we'll just return with a good status.
     */
    return st;
}


/* sec_krb_u2u_get_tgt
 * 
 * Lookup TGT data from the login handle registered in the sec__serv_id_table.
 * 
 * If caller asked for TGT data (as in a request from the client side), pass
 * that back as tgt_len and tgt_data.
 * 
 * If caller asked for TGT skey (above rtn sec_krb_u2u_register_server() needs
 * it to initialize the random number generator), then pass that back in
 * tgt_skey.
 */
error_status_t sec_krb_u2u_get_tgt(
    sec_login_handle_t  lh,
    unsigned32          *tgt_len,
    unsigned_char_p_t   *tgt_data_p,
    mit_des_cblock      tgt_skey,
    krb5_creds          *ret_creds_p
)
{
    error_status_t         st;
    krb5_creds             creds, *creds_p;
    sec_login_context_t    *lcp = (sec_login_context_t *) lh;

    /* Only send back the creds if the caller asked for them */
    if (ret_creds_p != NULL)
	creds_p = ret_creds_p;
    else
	creds_p = &creds;

    memset ((char*)creds_p, 0, sizeof(creds));

    /* Get principal name */
    if (st = krb5_cc_get_principal(KRB_INFO(lcp).cache, &creds_p->client)) {
        return st;
    }

    /* Get server name for creds */
    if (st = krb5_build_principal_ext(&creds_p->server,
                krb5_princ_realm(creds_p->client)->length,
                krb5_princ_realm(creds_p->client)->data, 6, "krbtgt",
                krb5_princ_realm(creds_p->client)->length,
                krb5_princ_realm(creds_p->client)->data, 0)) {
        if (creds_p != ret_creds_p) {
            krb5_free_cred_contents(creds_p);
        }
        return st;
    }

    /* Get TGT from cred cache */
    if (st = krb5_get_credentials_dce(KRB5_GC_CACHED, KRB_INFO(lcp).cache,
        creds_p, NULL)) {
        if (creds_p != ret_creds_p) {
            krb5_free_cred_contents(creds_p);
        }
        return st;
    }

    /* If caller asked for TGT data */
    if (tgt_data_p) {
        *tgt_len = creds.ticket.length;

        /* Pass back a copy of the TGT data */
        if ((*tgt_data_p = malloc(*tgt_len)) == NULL) {
            if (creds_p != ret_creds_p) {
                krb5_free_cred_contents(creds_p);
            }
            return sec_s_no_memory;
        }
        memcpy(*tgt_data_p, creds_p->ticket.data, *tgt_len);
    }

    /* If caller asked for TGT skey */
    if (tgt_skey) {
        memcpy(tgt_skey, creds_p->keyblock.contents, sizeof(mit_des_cblock));
    }

    /* Clean up local data */
    if (creds_p != ret_creds_p) {
        krb5_free_cred_contents(creds_p);
    }
    return st;
}


boolean32 sec_krb_must_use_u2u(
    error_status_t status
)
{
    return ( status == KRB5KDC_ERR_MUST_USE_USER2USER );
}


boolean32 sec_krb_never_valid(
    error_status_t status
)
{
    return ( status == KRB5KDC_ERR_NEVER_VALID );
}
