/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rpriv_v1_1_mgrs.c,v $
 * Revision 1.1.15.1  1996/05/10  13:21:36  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/2  1996/03/27  17:49 UTC  hanfei
 * 	merge in global group work for DCE 1.2.2
 *
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/2  1996/03/01  18:30 UTC  hanfei
 * 	code cleanup
 *
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/1  1996/01/31  19:45 UTC  hanfei
 * 	multi-cell group work: add audit code in parse_delegation_chain() for foreign_groupset
 * 	and add codes in rpriv_get_eptgt() to update epac's foreign_groupset data.
 *
 * 	HP revision /main/DCE_1.2/1  1996/01/03  19:35 UTC  psn
 * 	merge (revert)
 * 	[1995/07/31  18:42 UTC  rps  /main/HPDCE02/3]
 *
 * 	revert foreign cell account EPTGT check
 * 	[1995/07/31  15:41 UTC  rps  /main/HPDCE02/rps_moth3/1]
 *
 * 	[CHFts14668] check cell account validity when doing intercell client auth
 * 	[1995/06/14  18:00 UTC  rps  /main/HPDCE02/rps_moth2/1]
 *
 * 	fix for CHFts13993 (and OSF CR# 12770).
 * 	[1995/01/10  16:35 UTC  greg  /main/HPDCE02/1]
 *
 * Revision 1.1.9.1  1994/10/25  20:23:44  kline_s
 * 	UHYP CR 12343: Enable Delegation audit points: utilize dce_aud_start_with_uuid()
 * 	to prevent audit from causing secd to go into an infinite loop.
 * 	[1994/10/25  20:21:37  kline_s]
 * 
 * Revision 1.1.2.7  1994/09/28  20:50:26  greg
 * 	CR11910: pass delegation chain in priv_attr_trig_query calls.
 * 	CR12168: make certain the requested compatibility mode is
 * 	permitted by previous delegates/initiator.
 * 	[1994/09/28  18:03:43  greg]
 * 
 * Revision 1.1.5.2  94/09/21  9:45:49  greg
 * 	Add delegate info to priv_attr_trig callouts.
 * 
 * Revision 1.1.2.6  1994/08/24  21:07:13  cuti
 * 	Add audit points
 * 	[1994/08/24  20:08:35  cuti]
 * 
 * Revision 1.1.2.5  1994/08/18  20:25:39  greg
 * 	Flush FAKE-EPAC authorization data scheme and use the
 * 	real 1.1 protocol.
 * 	[1994/08/16  19:41:23  greg]
 * 
 * Revision 1.1.2.4  1994/08/04  16:13:21  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/08/01  11:44:14  mdf]
 * 
 * Revision 1.1.2.3  1994/06/10  15:07:24  greg
 * 	intercell fixes for DCE1.1 beta
 * 	[1994/06/03  20:50:19  greg]
 * 
 * Revision 1.1.2.2  1994/06/02  21:27:18  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:17:57  mdf]
 * 
 * Revision 1.1.2.1  1994/05/11  19:28:27  ahop
 * 	hp_sec_to_osf_2 drop
 * 	first cut at a real delegation token.
 * 	add memory management function parameters to sec_encode interfaces
 * 	work on rpriv_become_delegate.
 * 	[1994/04/29  21:41:04  ahop]
 * 
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1994
** Unpublished work. All Rights Reserved.
*/

/* 
 *     Version 1.1 Privilege Server Manager Routines
 */

#include <assert.h>
#include <rpriv_util.h>

#include <krb5/copyright.h>
#include <krb5/krb5.h>
#include <krb5/kdb.h>
#include <krb5/func-proto.h>
#include <krb5/los-proto.h>
#include <krb5/asn1.h>

#include <rpriv.h>

#include <bstring.h>
#include <un_strng.h>
#include <un_maloc.h>

#include <rs_server.h>
#include <rs_lock.h>
#include <rs_ch_lock.h>
#include <rs_util.h>

#include <rsdb_acct.h>
#include <rsdb_policy.h>
#include <rsdb_pgo.h>
#include <rsdb_name_util.h>

#include <sec_krb_util.h>
#include <sec_pkl.h>
#include <sec_encode.h>
#include <sec_id_pac_util.h>
#include <restrictions.h>
#include <priv_krb_util.h>
#include <u_str.h>

#define PTHREAD_EXC        /* make one version of pthread_exc.h harmless */
#define PTHREAD_EXC_H      /* make another version of pthread_exc.h harmless */
#include <dce/stubbase.h>  /* includes pthread_exc.h */
#include <authz_util.h> 

#include <dce/audit.h>
#include <sec_aud_evts.h>

#if defined(DCE_AUD_POINTS)
extern dce_aud_trail_t sec_aud_trail;

#define LOCAL_STR       "LOCAL"
#define INTERCELL_STR   "INTERCELL"
#endif	/* defined(DCE_AUD_POINTS) */

PRIVATE void  auth_rpc_check (
    handle_t  h,        /* [in] */
    error_status_t *stp /* [out] */
)
{

    rpc_authz_cred_handle_t priv;
    unsigned_char_p_t       server_princ_name;
    unsigned_char_p_t       cp;
    unsigned32              protect_level;
    unsigned32              authn_svc;
    unsigned32              authz_svc;
    unsigned32              len1, len2;
    

    CLEAR_STATUS(stp);

    rpc_binding_inq_auth_caller((rpc_binding_handle_t)h, &priv, &server_princ_name, &protect_level, 
				&authn_svc, &authz_svc, stp);
    if (!STATUS_OK(stp)) {
	return;
    }

    len1 = u_strlen(server_princ_name);
    len2 = u_strlen(SEC_PRIV_SERVER_NAME);
    cp = server_princ_name + len1 - len2;

    if (u_strcmp(cp, SEC_PRIV_SERVER_NAME) != 0) {
        *stp = sec_priv_s_invalid_server_name;
    }

    if (protect_level != rpc_c_protect_level_pkt_integ) {
	*stp = sec_priv_s_invalid_protect_lvl;
    }

    if (authn_svc != rpc_c_authn_dce_secret) {
	*stp = sec_priv_s_invalid_authn_svc;
    }

    if (authz_svc != rpc_c_authz_name) {
	*stp = sec_priv_s_invalid_authz_svc;
    }

    rpc_string_free(&server_princ_name, stp);

}

/*
 * The v1.1 priv server only understands version 0 tokens
 * so we walk the token set looking for a v0 token.  If we find
 * one, we make sure that it hasn't expired. 
 */
PRIVATE  void  parse_delegation_token (
    sec_encrypted_bytes_t  *delegation_token,  /* [in] */
    krb5_encrypt_block     *skey_eblock,       /* [in] */
    unsigned32             *token_expiration,  /* [out] */
    sec_id_seal_t          *seal,              /* [out] */
    error_status_t         *stp                /* [out] */
)
{

    krb5_enctype         etype;
    krb5_data            scratch;
    sec_dlg_token_set_t  token_set;
    boolean32            found;
    krb5_encrypt_block   ps_eblock;
    krb5_kvno            ps_kvno;
    sec_dlg_token_t      parsed_token;
    unsigned32           i;
    krb5_timestamp       now;
    extern krb5_deltat   krb5_clockskew;
    sec_authz_mp_t       ump;

    if (!delegation_token
    || !delegation_token->ebytes.num_bytes
    || !delegation_token->ebytes.bytes) {
	SET_STATUS(stp, sec_priv_s_invalid_dlg_token);
	return;
    }
    
    /* 
     * The delegation token is protected on the wire by the
     * session key for the priv server request.  Decrypt it.
     */
    scratch.length = delegation_token->ebytes.num_bytes;
    scratch.data = rpc_ss_allocate(scratch.length);
    *stp =  krb5_decrypt((krb5_pointer) delegation_token->ebytes.bytes,
			 (krb5_pointer) scratch.data,
			 scratch.length, skey_eblock, 0);
    if (!STATUS_OK(stp)) {
	return;
    }

    /*
     * The decrypted token bytes are an idl pickle of a
     * sec_dlg_token_set_t, so first we need to decode 
     * the token set.
     */
    sec_dlg_token_set_decode(NULL, NULL, scratch.length,
			     (idl_byte *) scratch.data, 
			     &token_set, stp);
    if (!STATUS_OK(stp)) {
	return;
    }

    if (!token_set.num_tokens || !token_set.tokens) {
	SET_STATUS(stp, sec_priv_s_invalid_dlg_token);
	return;
    }

    /*
     * loop through the token set presented by the caller searching
     * for a version 0 token, which is all the v1_1 priv server
     * understands.  We simply ignore any newer versions of tokens
     * that we find.  This way, newer priv servers that support
     * new delegation token protocols can remain compatible with us.
     */
    for (i = 0, found = false;  i < token_set.num_tokens && !found; i++) {
	/*
	 * Make sure the current token has some content
	 * before we try to make use of it
	 */
	if (!token_set.tokens[i].token_bytes.num_bytes
	    || !token_set.tokens[i].token_bytes.bytes) {
	    continue;
	}
	
	/*
	 * The version number is in the first byte of the
	 * delegation token.  Check to see if it's version 0.
	 */
	if (TOKEN_VNO(token_set.tokens[i].token_bytes.bytes) == 0) {
	    found = true;
	    parsed_token = token_set.tokens[i];
	}
    }
    
    if (!found) {
	SET_STATUS(stp, sec_priv_s_invalid_dlg_token);
	return;
    }
    
    /* 
     * decrypt the ciphertext portion of the token bytes 
     * using the priv server key version number in byte 2 of the
     * token bytes.  If successful, the token bytes will
     * contain a 4-byte big-endian expiration timestamp followed
     * by 16 bytes of md5 message digest.
     */
    rpriv_util_get_ps_eblock(ETYPE_DES_CBC_CRC, 
			     V0_TOKEN_KVNO(parsed_token.token_bytes.bytes),
			     &ps_eblock, &ps_kvno, stp);
    
    if (!STATUS_OK(stp)) {
	return;
    }
    
    /* 
     * Since we don't support confidential bytes at
     * 1.1, and we assume that all privilege servers
     * in a particular cell either support confidential
     * bytes, or do not, we use only the required portion
     * of the delegation token.  If, perchance, we were to
     * receive a delegation token containing optional 
     * confidential bytes info, the following decryption
     * will fail.  So be it.
     */
    scratch.length = V0_TOKEN_REQ_CIPHER_LEN;
    scratch.data = rpc_ss_allocate(scratch.length);
    *stp =  krb5_decrypt(
	        (krb5_pointer) V0_TOKEN_CIPHER(parsed_token.token_bytes.bytes),
		(krb5_pointer) scratch.data, scratch.length, &ps_eblock, 0);
    
    /* ENHANCEMENT
     * 
     * If we decide to cache priv server encryption blocks as
     * suggested in the comments for rpriv_util_get_ps_eblock,
     * then we don't want to "finish" the key, but, for now, we
     * must.
     */
    rpriv_util_finish_ps_eblock(&ps_eblock);
    if (!STATUS_OK(stp)) {
	return;
    }
    
    /* 
     * scratch now contains a 4-byte big-endian timestamp, followed
     * by a 16 byte md5 message digest.  If we're little-endian, we 
     * have to swap the bytes in the token expiration.
     */
    ump = (sec_authz_mp_t) &(scratch.data[0]);
    AUTHZ_UTIL_UNMARSH_BE_UNSIGNED32(ump, *token_expiration);
    AUTHZ_UTIL_ADVANCE_MP(ump, 4);

    *stp = krb5_timeofday(&now);
    if (STATUS_OK(stp)) {
	if (*token_expiration - now < krb5_clockskew) {
	    SET_STATUS(stp, sec_priv_s_deleg_token_exp);
	}
    }
    if (STATUS_OK(stp)) {
	seal->seal_type = sec_id_seal_type_md5;
	seal->seal_len = 16;
	seal->seal_data = rpc_ss_allocate(seal->seal_len);
	AUTHZ_UTIL_UNMARSH_BYTES(ump, seal->seal_data, seal->seal_len);
    }
}

				       
PRIVATE  void parse_delegation_chain (
    dce_aud_rec_t           ard,                /* [in] */
    sec_bytes_t             *delegation_chain,  /* [in] */
    sec_id_seal_t           *seal,              /* [in] */
    unsigned32              *num_delegates,     /* [out] */
    sec_id_epac_data_t      **epac_data_chain,  /* [out] */
    error_status_t          *stp                /* [out] */
)
{
    sec_id_epac_set_t     epac_set;
    sec_id_seal_t         my_seal;
#if defined(DCE_AUD_POINTS)
    dce_aud_ev_info_t       ev_info;
    unsigned32              j, k;
    error_status_t          aud_status;
#endif	/* defined(DCE_AUD_POINTS) */

    CLEAR_STATUS(stp);
    *epac_data_chain = NULL;

    /* 
     * There has to be something in the delegation chain
     * parameter.
     */
    if (!delegation_chain
    || !delegation_chain->num_bytes
    || !delegation_chain->bytes) {
	SET_STATUS(stp, sec_priv_s_corrupt_deleg_chain);
	return;
    }

    /*
     * We can't verify the delegation chain without a seal,
     * so make sure that there is something in the seal
     * parameter.  If there is, it has to be an md5 seal
     * cause that's all we understand.
     */
    if (!seal || !seal->seal_data || !seal->seal_len 
    || seal->seal_type != sec_id_seal_type_md5) {
	SET_STATUS(stp, sec_priv_s_corrupt_deleg_chain);
	return;
    }

    /*
     * Generate a seal over the delegation chain and
     * verify that it matches the seal passed in
     * If it doesn't, return sec_priv_s_corrupt_deleg_chain.
     */
    sec_authz_util_gen_seal(rpc_ss_allocate, rpc_ss_free,
			    delegation_chain, &my_seal);
    if (memcmp(my_seal.seal_data, seal->seal_data, my_seal.seal_len) != 0) {
	SET_STATUS(stp, sec_priv_s_corrupt_deleg_chain);
	return;
    }

    /* 
     * Allocate a vector of sec_id_epac_data_t and decode
     * the delegation chain into it.
     */
    sec_id_epac_set_decode(NULL, NULL, delegation_chain->num_bytes,
			   delegation_chain->bytes,
			   &epac_set, stp);

    if (STATUS_OK(stp) && epac_set.num_epacs > 0) {
	unsigned32 i;

	*epac_data_chain = (sec_id_epac_data_t *) 
	    rpc_ss_allocate(epac_set.num_epacs * sizeof(**epac_data_chain));
	memset(*epac_data_chain, 0, sizeof(**epac_data_chain));

#if defined(DCE_AUD_POINTS)
	if (ard) {
	    ev_info.format = aud_c_evt_info_ushort_int;
	    ev_info.data.ushort_int = epac_set.num_epacs;
	    dce_aud_put_ev_info(ard, ev_info, &aud_status);
	}
#endif	/* defined(DCE_AUD_POINTS) */
	
	for (i = 0; i < epac_set.num_epacs && STATUS_OK(stp); i++) {
	    sec_id_epac_data_decode(NULL, NULL,
		    epac_set.epacs[i].pickled_epac_data.num_bytes,
		    epac_set.epacs[i].pickled_epac_data.bytes,
		    &((*epac_data_chain)[i]),
		    stp);

#if defined(DCE_AUD_POINTS)
	    if (ard && STATUS_OK(stp)) {
		ev_info.format = aud_c_evt_info_uuid;
		ev_info.data.uuid = (*epac_data_chain)[i].pa.realm.uuid;
		dce_aud_put_ev_info(ard, ev_info, &aud_status);

		ev_info.format = aud_c_evt_info_uuid;
		ev_info.data.uuid = (*epac_data_chain)[i].pa.principal.uuid;
		dce_aud_put_ev_info(ard, ev_info, &aud_status);

		ev_info.format = aud_c_evt_info_uuid;
		ev_info.data.uuid = (*epac_data_chain)[i].pa.group.uuid;
		dce_aud_put_ev_info(ard, ev_info, &aud_status);

		ev_info.format = aud_c_evt_info_ushort_int;
		ev_info.data.ushort_int = (*epac_data_chain)[i].pa.num_groups;
		dce_aud_put_ev_info(ard, ev_info, &aud_status);

		for (j = 0; j < (*epac_data_chain)[i].pa.num_groups; j++) {
		    ev_info.format = aud_c_evt_info_uuid;
		    ev_info.data.uuid = (*epac_data_chain)[i].pa.groups[j].uuid;
		    dce_aud_put_ev_info(ard, ev_info, &aud_status);
		}

		ev_info.format = aud_c_evt_info_ushort_int;
		ev_info.data.ushort_int = (*epac_data_chain)[i].pa.num_foreign_groupsets;
		dce_aud_put_ev_info(ard, ev_info, &aud_status);

		for (j = 0; j < (*epac_data_chain)[i].pa.num_foreign_groupsets; j++) {
		    ev_info.format = aud_c_evt_info_uuid;
		    ev_info.data.uuid = (*epac_data_chain)[i].pa.foreign_groupsets[j].realm.uuid;
		    dce_aud_put_ev_info(ard, ev_info, &aud_status);
		    
		    ev_info.format = aud_c_evt_info_ushort_int;
		    ev_info.data.ushort_int = (*epac_data_chain)[i].pa.foreign_groupsets[j].num_groups;
		    dce_aud_put_ev_info(ard, ev_info, &aud_status);

		    for ( k = 0; k < (*epac_data_chain)[i].pa.foreign_groupsets[j].num_groups; k++ ) {
		        ev_info.format = aud_c_evt_info_uuid;
		        ev_info.data.uuid = (*epac_data_chain)[i].pa.foreign_groupsets[j].groups[k].uuid;
		        dce_aud_put_ev_info(ard, ev_info, &aud_status);
		    }
		}
    }
#endif	/* defined(DCE_AUD_POINTS) */
	}
    }

    if (STATUS_OK(stp)) {
	*num_delegates = epac_set.num_epacs;
    }
}

PRIVATE void parse_delegation_restriction(
    sec_id_pa_t                    *caller_pa,                /* [in] */
    unsigned32                     num_epacs,                      /* [in] */
    sec_id_epac_data_t             *epac_data_chain,          /* [in/out] */
    error_status_t                 *stp                       /* [out] */
)
{

    boolean32                      match;
    unsigned32                     i=0;
    sec_id_epac_data_t             *epac_data_p;

    CLEAR_STATUS(stp);

    for (i = 0, epac_data_p = epac_data_chain; i < num_epacs; i++, epac_data_p++) {  
	match = sec_id_check_restrictions(&epac_data_p->pa, 
                                   &epac_data_p->deleg_restrictions,
                                   caller_pa, stp);

	if (!match || !STATUS_OK(stp)) {
	    /* means caller is not in restriction */
	    /* replace epac_data with anonymous epac */
	    sec_id_get_anonymous_epac(free, epac_data_p, stp);
	}
    }

}
	    


PRIVATE  void 	verify_delegation_chain (
    sec_id_delegation_type_t  delegation_type_requested, /* [in] */
    sec_id_pa_t               *caller_pa,                /* [in] */
    unsigned32                num_epacs,                 /* [in] */
    sec_id_epac_data_t        *epac_data_chain,          /* [in/out] */
    error_status_t            *stp                       /* [out] */
)
{
    sec_id_delegation_type_t delegation_type_permitted;
    
    assert(num_epacs > 0 && epac_data_chain != NULL);
    CLEAR_STATUS(stp);
    
    /*
     * Check the delegation type in the last delegate in the
     * chain against the delegation type permitted (to be permitted
     * by the new delegate, actually).
     */
    delegation_type_permitted = epac_data_chain[num_epacs -1].deleg_type;
    switch (delegation_type_permitted) {
    case sec_id_deleg_type_none:
	SET_STATUS(stp, sec_priv_s_deleg_not_enabled);
	break;
    case sec_id_deleg_type_traced:
	/*
	 * new delegate can request "traced" or "none", but
	 * not "impersonation."
	 */
	if (delegation_type_requested == sec_id_deleg_type_impersonation) {
	    SET_STATUS(stp, sec_priv_s_deleg_not_enabled);
	}
	break;
    case sec_id_deleg_type_impersonation:
	/*
	 * new delegate can request "impersonation" or "none", but
	 * not "traced."
	 */
	if (delegation_type_requested == sec_id_deleg_type_traced) {
	    SET_STATUS(stp, sec_priv_s_deleg_not_enabled);
	}
	break;
    default:
	/*
	 * Shouldn't ever happen, but just in case...
	 */
	SET_STATUS(stp, sec_priv_s_bad_deleg_type);
	break;
    }     
 
    /*
     * check the delegate restrictions in each epac
     * in the delegation chain against the caller's credentials
     * and replace any that match with the anonymous EPAC.
     *
     * The replacement occurs "in place" in the epac_data_chain.
     * We don't worry about orphaning any storage 'cause we assume
     * it has been allocated using rpc_ss_allocate and will magically
     * disappear when the server stub returns.
     */
    
    if (STATUS_OK(stp)) {
	parse_delegation_restriction(caller_pa, num_epacs, epac_data_chain, stp);
    }

}

    


PRIVATE  void assign_delegation_restrictions (
    sec_id_delegation_type_t       delegation_type_permitted, /* [in] */
    sec_id_restriction_set_t       *delegate_restrictions,    /* [in] */
    sec_id_restriction_set_t       *target_restrictions,      /* [in] */
    sec_id_opt_req_t               *optional_restrictions,    /* [in] */
    sec_id_opt_req_t               *required_restrictions,    /* [in] */
    sec_id_compatibility_mode_t    compatibility_mode,        /* [[in]*/
    sec_id_epac_data_t             *delegate                  /* [in/out] */
)
{
    /*
     * Just reassign pointers without fear of orphaning  
     * because the delegate epac was allocated
     * using rpc_ss_allocate().
     */
    delegate->deleg_type = delegation_type_permitted;
    delegate->opt_restrictions = *optional_restrictions;
    delegate->req_restrictions = *required_restrictions;
    delegate->deleg_restrictions = *delegate_restrictions;
    delegate->target_restrictions = *target_restrictions;
    delegate->compat_mode = compatibility_mode;
}


PRIVATE  unsigned32 get_expiration (
    unsigned32  token_exp,   /* delegation token expiration */
    unsigned32  tkt_exp      /* ticket expiration */
)
{
    /*
     * A token expiration value of 0 means ignore the
     * token expiration
     */
    if (token_exp == 0
    ||  tkt_exp < token_exp) {
	return tkt_exp;  /* ticket expires sooner */
    }

    return token_exp;  /* delegation token expires sooner */
}


/*
 * macro for common cleanup of storage not allocated using
 * rpc_ss_allocate
 */
#define eptgt_common_cleanup() \
	if (ptgt) krb5_free_ticket(ptgt); \
	if (req_info) rpriv_util_free_krb_req_info(&req_info); \
        rpriv_util_free_authz_data(&authz_data); 

extern void rpriv_become_delegate(
    handle_t                       handle,                    /* [in] */
    unsigned32                     authn_svc,                 /* [in] */
    unsigned32                     authz_svc,                 /* [in] */
    sec_id_delegation_type_t       delegation_type_permitted, /* [in] */
    sec_id_restriction_set_t       *delegate_restrictions,    /* [in] */
    sec_id_restriction_set_t       *target_restrictions,      /* [in] */
    sec_id_opt_req_t               *optional_restrictions,    /* [in] */
    sec_id_opt_req_t               *required_restrictions,    /* [in] */
    sec_id_compatibility_mode_t    compatibility_mode,        /* [[in]*/
    sec_bytes_t                    *delegation_chain,         /* [in] */
    sec_bytes_t                    *delegate,                 /* [in] */
    sec_encrypted_bytes_t          *delegation_token,         /* [in] */
    rpriv_pickle_t                 *ptgt_req,                 /* [in] */
    rpriv_pickle_t                 **ptgt_rep,                /* [out] */
    sec_bytes_t                    *new_delegation_chain,     /* [out] */
    rpriv_attr_request_t           *aux_attr_request,         /* [in, ptr] */
    rpriv_attr_result_t            *aux_attr_result,          /* [out] */
    unsigned char                  *app_tkt_request,          /* [in, ptr] */
    rpriv_app_tkt_result_t         *app_tkt_result,           /* [out] */
    error_status_t                 *status                    /* [out, ref] */
)
{
    krb5_data               *krb_rep;
    sec_id_epac_set_t       req_epac_set, new_dlg_epac_set;
    krb5_authdata           **authdata = NULL;
    rpriv_dce_authz_data_t  authz_data;
    rpriv_krb_req_info_t    *req_info = NULL;
    krb5_ticket             *ptgt = NULL;
    sec_id_epac_data_t      req_epac, 
                            granted_epac,
                            *deleg_chain_epac_data = NULL;
    unsigned32              num_delegates, 
                            token_expiration = 0,
                            ident_expiration;
    sec_id_seal_t           token_seal;

    dce_aud_rec_t           ard = NULL;
    error_status_t          aud_status;
       
    INIT_AUX_RETURN_PARMS(aux_attr_result,app_tkt_result);
    *ptgt_rep = NULL;
    new_delegation_chain->num_bytes = 0;
    new_delegation_chain->bytes = NULL;
    

    if (authz_svc != rpc_c_authz_dce) {
        SET_STATUS(status, sec_priv_s_invalid_authz_svc);
        return;
    }

    if (authn_svc != rpc_c_authn_dce_private) {
        SET_STATUS(status, sec_priv_s_invalid_authn_svc);
        return;
    }

    CHECK_DELEGATION_TYPE(delegation_type_permitted, status);

    if (STATUS_OK(status)) {
	auth_rpc_check(handle, status);
    }

    if (STATUS_OK(status)) {
	rpriv_util_parse_request(
	                         PRIV_BecomeDelegate,  /* audit event */
                                 handle,               /* - for audit's usage */
                                 ptgt_req, 
                                 &authz_data, 
                                 &req_info, 
                                 &ard,
                                 status);
    }

    if (!STATUS_OK(status)) {
#if defined(DCE_AUD_POINTS)
	if (ard) {
            if (rs_util_status_is_denial(*status)) {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                               aud_c_esl_cond_denial, &aud_status);
            } else {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                               aud_c_esl_cond_failure, &aud_status);
            } 
        }
#endif	/* defined(DCE_AUD_POINTS) */ 
	return;
    }

    TRY {
	/*
	 * All delegation requests must be "local"
	 */
	if (!authz_data.local) {
	    SET_STATUS(status, sec_priv_s_intercell_deleg_req);
	} else { 
	    rpriv_util_parse_req_privs(delegate, &req_epac, status);

	}

	if (STATUS_OK(status)) {

	    rpriv_util_verify_req_privs(ard,
                                        authz_data.client_princ,
					authz_data.client_cell,
					&req_epac, &granted_epac,
					status);
	}

	if (STATUS_OK(status)) {
	    parse_delegation_token(delegation_token, 
				   &req_info->eblock,
				   &token_expiration,
                                   &token_seal,
				   status);
	}

	if (STATUS_OK(status)) {
	    parse_delegation_chain(ard,
                                   delegation_chain, 
				   &token_seal,
				   &num_delegates,
				   &deleg_chain_epac_data,
				   status);
	}

	if (STATUS_OK(status)) {
	    verify_delegation_chain(delegation_type_permitted,
				    &granted_epac.pa, 
				    num_delegates,
				    deleg_chain_epac_data,
				    status);
	}

	if (STATUS_OK(status)) {
	    /*
             * Can't engage in traced delegation unless
	     * enable by the initiator/previous delegate
	     */
	    if (deleg_chain_epac_data[num_delegates-1].deleg_type != 
		    sec_id_deleg_type_traced)
		SET_STATUS(status, sec_priv_s_deleg_not_enabled);
	}
               
	if (STATUS_OK(status)) {
	    /*
	     * Verify the requested attribute set, if any.
	     * Do this after verifying the delegation 
	     * chain because the attribute lookup may
	     * result in a trigger callout to an
	     * authorization processor and it would be
	     * positively silly to incur that cost
	     * only to reject the priv server request 
	     * afterward because the delegation
	     * chain was bad.  Also, a stripped down
	     * version of the delegation chain is passed
	     * to the authorization processor and that
	     * should reflect any anonymity imposed
	     * as a result of delegate restrictions.
	     */
	    rpriv_util_verify_local_attrs(authz_data.client_princ,
					  &granted_epac,
					  num_delegates,
					  deleg_chain_epac_data,
					  req_epac.num_attrs,
					  req_epac.attrs,
					  &granted_epac.num_attrs,
					  &granted_epac.attrs,
					  status);
	}
	    
	/*
	 * The delegation_type_permitted, delegate_restrictions, 
	 * target_restrictions, optional_restrictions,
	 * required_restrictions, and compatibility_mode
	 * parameters in the request get assigned without
	 * any further processing to the new delegate being
	 * added to the chain.  
	 */
	assign_delegation_restrictions(delegation_type_permitted, 
				       delegate_restrictions, 
				       target_restrictions,
				       optional_restrictions,
				       required_restrictions,
				       compatibility_mode,
				       &granted_epac);

	ident_expiration = 
            get_expiration(token_expiration,
                req_info->authent->ticket->enc_part2->times.endtime);

	/*
	 * Everything checks out, so go ahead and construct
	 * a new delegation chain from the (possibly modifed)
	 * input delegation chain, and the new delegate's
	 * requested privileges
	 */
	if (STATUS_OK(status)) {
	    rpriv_util_gen_v1_1_privs(&granted_epac,
				      num_delegates,
				      deleg_chain_epac_data,
				      ident_expiration,
				      new_delegation_chain,
				      &authdata,
				      status);
	}

	if (STATUS_OK(status)) {
	    rpriv_util_build_ptgt(req_info, ident_expiration,
				  authdata, &ptgt, status);
	
	}	
    
	if (STATUS_OK(status)) {
	    rpriv_util_build_ptgt_rep(req_info, NULL, ptgt,
				      ptgt_rep, status);
	}

    } CATCH_ALL {
	/* 
	 * cleanup any memory *not* allocated using rpc_ss_allocate()
	 * and reraise the exception
         */
	eptgt_common_cleanup();
	RERAISE;
    } ENDTRY;

#if defined(DCE_AUD_POINTS)
    if (ard) {    
        if (GOOD_STATUS(status)) {
            dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1,  
                           aud_c_esl_cond_success, &aud_status); 
        } else {  
            if (rs_util_status_is_denial(*status)) {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                               aud_c_esl_cond_denial, &aud_status);
            } else {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                               aud_c_esl_cond_failure, &aud_status);
            } 
        }
    }
#endif	/* defined(DCE_AUD_POINTS) */

    /* 
     * cleanup any memory *not* allocated using rpc_ss_allocate()
     */
    eptgt_common_cleanup();
}

extern void rpriv_become_impersonator(
    handle_t                       handle,                    /* [in] */
    unsigned32                     authn_svc,                 /* [in] */
    unsigned32                     authz_svc,                 /* [in] */
    sec_id_delegation_type_t       delegation_type_permitted, /* [in] */
    sec_id_restriction_set_t       *delegate_restrictions,    /* [in] */
    sec_id_restriction_set_t       *target_restrictions,      /* [in] */
    sec_id_opt_req_t               *optional_restrictions,    /* [in] */
    sec_id_opt_req_t               *required_restrictions,    /* [in] */
    sec_bytes_t                    *delegation_chain,         /* [in] */
    sec_bytes_t                    *impersonator,             /* [in] */
    sec_encrypted_bytes_t          *delegation_token,         /* [in] */
    rpriv_pickle_t                 *ptgt_req,                 /* [in] */
    rpriv_pickle_t                 **ptgt_rep,                /* [out] */
    sec_bytes_t                    *new_delegation_chain,     /* [out] */
    rpriv_attr_request_t           *aux_attr_request,         /* [in, ptr] */
    rpriv_attr_result_t            *aux_attr_result,          /* [out, ref] */
    unsigned char                  *app_tkt_request,          /* [in, ptr] */
    rpriv_app_tkt_result_t         *app_tkt_result,           /* [out, ref] */
    error_status_t                 *status                    /* [out, ref] */
    )

{
    krb5_data               *krb_rep;
    krb5_authdata           **authdata = NULL;
    rpriv_dce_authz_data_t  authz_data;
    rpriv_krb_req_info_t    *req_info = NULL;
    krb5_ticket             *ptgt = NULL;
    sec_id_epac_data_t      req_impersonator_epac, 
                            impersonator_epac,
                            granted_epac,
                            *deleg_chain_epac_data = NULL;
    unsigned32              num_delegates, 
                            token_expiration = 0,
                            ident_expiration;
    sec_id_seal_t           token_seal;
    sec_id_restriction_set_t       *new_delegate_restrictions;
    sec_id_restriction_set_t       *new_target_restrictions;  
    sec_id_opt_req_t               *new_optional_restrictions;
    sec_id_opt_req_t               *new_required_restrictions;
       
    dce_aud_rec_t           ard = NULL;
    dce_aud_ev_info_t       ev_info;
    error_status_t          aud_status;

    INIT_AUX_RETURN_PARMS(aux_attr_result,app_tkt_result);
    *ptgt_rep = NULL;
    new_delegation_chain->num_bytes = 0;
    new_delegation_chain->bytes = NULL;

    
    if (authz_svc != rpc_c_authz_dce) {
        SET_STATUS(status, sec_priv_s_invalid_authz_svc);
        return;
    }

    if (authn_svc != rpc_c_authn_dce_private) {
        SET_STATUS(status, sec_priv_s_invalid_authn_svc);
        return;
    }

    CHECK_DELEGATION_TYPE(delegation_type_permitted, status);

    if (STATUS_OK(status)) {
        auth_rpc_check(handle, status);
    }

    if (STATUS_OK(status)) { 
	rpriv_util_parse_request(
	                         PRIV_BecomeImpersonator,  /* audit event */
                                 handle,                   /* - for audit's usage */
                                 ptgt_req, 
                                 &authz_data, 
                                 &req_info, 
                                 &ard,
                                 status);
    }

    if (!STATUS_OK(status)) {
#if defined(DCE_AUD_POINTS)
	if (ard) {
            if (rs_util_status_is_denial(*status)) {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                               aud_c_esl_cond_denial, &aud_status);
            } else {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                               aud_c_esl_cond_failure, &aud_status);
            } 
        }
#endif	/* defined(DCE_AUD_POINTS) */ 
	return;
    }

    TRY {
	/*
	 * All delegation requests must be "local"
	 */
	if (!authz_data.local) {
	    SET_STATUS(status, sec_priv_s_intercell_deleg_req);
	} else { 
	    rpriv_util_parse_req_privs(impersonator, 
				       &req_impersonator_epac, 
				       status);
	}

	if (STATUS_OK(status)) {
	    rpriv_util_verify_req_privs(
                                        ard,
                                        authz_data.client_princ,
					authz_data.client_cell,
					&req_impersonator_epac,
					&impersonator_epac, 
					status);
	}	
    
	if (STATUS_OK(status)) {
	    parse_delegation_token(delegation_token, 
				   &req_info->eblock,
				   &token_expiration,
                                   &token_seal,
				   status);
	}

	if (STATUS_OK(status)) {
	    parse_delegation_chain(
                                   ard,
                                   delegation_chain, 
				   &token_seal,
				   &num_delegates,
				   &deleg_chain_epac_data,
				   status);
	}

	if (STATUS_OK(status)) {
	    verify_delegation_chain(delegation_type_permitted,
				    &impersonator_epac.pa, 
				    num_delegates,
				    deleg_chain_epac_data,
				    status);
	}

	if (STATUS_OK(status)) {
	    /*
             * Can't engage in impersonation delegation unless
	     * enable by the initiator/previous impersonator
	     * enabled it.
	     */
	    if (deleg_chain_epac_data[num_delegates-1].deleg_type != 
		    sec_id_deleg_type_impersonation)
		SET_STATUS(status, sec_priv_s_deleg_not_enabled);
	}

	/* 
	 * No need to verify attributes--we just
	 * accept them as part of the impersonation
	 */

	/*
         * We know that a legitimate impersonation
	 * request will always contain a delegation
         * chain of length 1.  Why? Because once 
         * traced delegation is enabled, no subsequent
         * impersonation request will be honored, 
         * and vice versa.  That policy is enforced
         * in the verify_delegation_chain() logic.
         * So, from here on down we assume num_delegates = 1.
         * Since this is an impersonation request,
         * The granted privileges are based on the single 
         * delegate epac in the delegation chain.  We
         * modify that epac slightly based on the
         * delegation type permitted by the impersonator
         * (can be changed to none) and the restrictions 
         * requested by the impersonator.
	 */
	if (STATUS_OK(status)) {
	    granted_epac = deleg_chain_epac_data[0];

	    granted_epac.deleg_type = delegation_type_permitted;

	    ident_expiration = 
		get_expiration(token_expiration,
		    req_info->authent->ticket->enc_part2->times.endtime);

	    /* 
	     * Add the impersonator's requested optional and required 
	     * restrictions to the optional and required restrictions
	     * of the principal being impersonated: the union of two 
	     * optional or required restriction sets is "more" restrictive.
	     * Compute the intersection of the target and delegate 
	     * restrictions because the intersection of two target
	     * or delegate restrictions is "more" restrictive.  But
	     * there's a catch.  If the intersection of two non-empty
	     * terget (or delegate) restrictions produces an empty set,
	     * that means there are no legitmate targets (or delegates).
	     * We represent this with the entry type "no_other" because
	     * the empty set of target (or delegate) restrictions
	     * is already defined to mean all targets (or delegates)
	     * are legitimate. 
	     */

	    /*
	     * If an anonymous epac, don't bother checking delegate and
	     * target restrictions 
	     */

	    if (!sec_id_is_anonymous_epac(&granted_epac, status) 
            && STATUS_OK(status)  ) {

		sec_id_restriction_intersection(rpc_ss_allocate, rpc_ss_free, 
                                     &granted_epac.deleg_restrictions,
				     &granted_epac.pa.realm.uuid,
				     delegate_restrictions,
				     &impersonator_epac.pa.realm.uuid,
				     &new_delegate_restrictions,
				     status);

		if (STATUS_OK(status)) {
		    sec_id_restriction_intersection(rpc_ss_allocate, rpc_ss_free,
                                     &granted_epac.target_restrictions,
				     &granted_epac.pa.realm.uuid,
				     target_restrictions,
				     &impersonator_epac.pa.realm.uuid,
				     &new_target_restrictions,
				     status);
		}
	    }

	    if (STATUS_OK(status)) {
		sec_id_opt_req_restriction_union (rpc_ss_allocate, rpc_ss_free,
                               &granted_epac.opt_restrictions,
			       optional_restrictions,
			       &new_optional_restrictions,
			       status);
	    }

	    if (STATUS_OK(status)) {
		sec_id_opt_req_restriction_union (rpc_ss_allocate, rpc_ss_free,
                               &granted_epac.req_restrictions,
			       required_restrictions,
			       &new_required_restrictions,
			       status);
	    }

	    if (STATUS_OK(status)) {
		assign_delegation_restrictions(delegation_type_permitted, 
				       new_delegate_restrictions, 
				       new_target_restrictions,
				       new_optional_restrictions,
				       new_required_restrictions,
				       granted_epac.compat_mode, /* Made it ????? */
				       &granted_epac);
	    }
	   
				     
	}

	/*
	 * Everything checks out, so go ahead and construct authdata that
         * reflects the impersonation.
	 */
	if (STATUS_OK(status)) {
	    rpriv_util_gen_v1_1_privs(&granted_epac,
				      0, 
				      NULL,
				      ident_expiration,
				      new_delegation_chain,
				      &authdata,
				      status);
	}

	if (STATUS_OK(status)) {
	    rpriv_util_build_ptgt(req_info, ident_expiration,
				  authdata, &ptgt, status);
	
	}	
    
	if (STATUS_OK(status)) {
	    rpriv_util_build_ptgt_rep(req_info, NULL, ptgt,
				      ptgt_rep, status);
	}

    } CATCH_ALL {
	/* 
	 * cleanup any memory *not* allocated using rpc_ss_allocate()
	 * and reraise the exception
         */
	eptgt_common_cleanup();
	RERAISE;
    } ENDTRY;
    
#if defined(DCE_AUD_POINTS)
    if (ard) {    
        if (GOOD_STATUS(status)) {
            dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1,  
                           aud_c_esl_cond_success, &aud_status); 
        } else {  
            if (rs_util_status_is_denial(*status)) {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                               aud_c_esl_cond_denial, &aud_status);
            } else {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                               aud_c_esl_cond_failure, &aud_status);
            } 
        }
    }
#endif	/* defined(DCE_AUD_POINTS) */

    /* 
     * cleanup any memory *not* allocated using rpc_ss_allocate()
     */
    eptgt_common_cleanup();

}


/* r p r i v _ g e t _ e p t g t
 * 
 * Privilege server manager routine to construct and return a
 * an extended privilege certificate to the ticket-granting service.
 * The caller supplies the extended privilege attributes in the form
 * of an encoded Extended Privilege Attribute Certificate.
 * 
 * How the requested privilege attributes are verified depends on
 * how the call is authenticated and whether or not the request is 
 * "local" (i.e. is a request from a client in this privilege server's
 * cell, or is from a foreign privilege service). 
 *
 * Local request
 *
 * If the request is local, then the ticket to the privilege server
 * is based on a kerberos V5 TGT and the requested_privs consists
 * of a single encoded EPAC.  The privilege server decodes the 
 * requested_privs and verifies that the requested privileges are
 * valid by performing the necessary database queries.  The privilege
 * server then generates a new encoded EPAC containing the granted
 * privileges (it is not an error to request invalid privileges,
 * they simply will not appear in the EPAC generated by the
 * priv server), computes a checksum of the granted privileges and,
 * if delegation is enabled, generates a delegation token.
 * 
 * Intercell request
 *  
 * If the request is foreign, then the ticket to the privilege
 * service is based on a DCE EPTGT and the privilege server
 * retrieves the EPAC seal from the DCE authorization data
 * contained in the ticket, and uses it to verify that the 
 * requested privileges are valid.  In addition, if delegation
 * is enabled, the priv server generates a new delegation token
 * encrypted under its key (the existing delegation token present in
 * the incoming PTGT is useless since it is encrypted under the
 * key of a foreign priv server).
 *
 */

extern void rpriv_get_eptgt(
    handle_t                       handle,            /* [in] */
    unsigned32                     authn_svc,         /* [in] */
    unsigned32                     authz_svc,         /* [in] */
    sec_bytes_t                    *requested_privs,  /* [in] */
    rpriv_pickle_t                 *ptgt_req,         /* [in] */
    rpriv_pickle_t                 **ptgt_rep,        /* [out] */
    sec_bytes_t                    *granted_privs,    /* [out] */
    rpriv_attr_request_t           *aux_attr_request, /* [in] */
    rpriv_attr_result_t            *aux_attr_result,  /* [out] */ 
    unsigned char                  *app_tkt_request,  /* [in] */
    rpriv_app_tkt_result_t         *app_tkt_result,   /* [out] */
    error_status_t                 *status            /* [out] */
    )
{
    sec_id_epac_set_t       req_epac_set;
    krb5_authdata           **authdata = NULL;
    rpriv_dce_authz_data_t  authz_data;
    rpriv_krb_req_info_t    *req_info = NULL;
    krb5_ticket             *ptgt = NULL;
    unsigned32              token_expiration = 0, ident_expiration;

#if defined(DCE_AUD_POINTS)
    dce_aud_ev_info_t      ev_info;
    error_status_t         aud_status;
#endif	/* defined(DCE_AUD_POINTS) */
    dce_aud_rec_t           ard = NULL;  /* always needed */

    INIT_AUX_RETURN_PARMS(aux_attr_result,app_tkt_result);
    *ptgt_rep = NULL;
    granted_privs->num_bytes = 0;
    granted_privs->bytes = NULL;

    if (authz_svc != rpc_c_authz_dce) {
        SET_STATUS(status, sec_priv_s_invalid_authz_svc);
        return;
    }

    if (authn_svc != rpc_c_authn_dce_private) {
        SET_STATUS(status, sec_priv_s_invalid_authn_svc);
        return;
    }

    auth_rpc_check(handle, status);

    if (STATUS_OK(status)) {
	rpriv_util_parse_request(
	                         PRIV_GetEptgt,   /* audit event */
                                 handle,          /* - for audit's usage */
                                 ptgt_req, 
                                 &authz_data, 
                                 &req_info,
                                 &ard,
                                 status);
    }

    if (!STATUS_OK(status)) {

#if defined(DCE_AUD_POINTS)
	if (ard) {
            if (rs_util_status_is_denial(*status)) {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                               aud_c_esl_cond_denial, &aud_status);
            } else {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                               aud_c_esl_cond_failure, &aud_status);
            } 
        }
#endif	/* defined(DCE_AUD_POINTS) */ 
	return;
    }

    TRY {
	/* Is the client local or foreign? */
	if (authz_data.local) {	/* This is a local request */
	    sec_id_epac_data_t      req_epac, granted_epac;

#if defined(DCE_AUD_POINTS)
            if (ard) {
                ev_info.format = aud_c_evt_info_char_string;
                ev_info.data.char_string = (idl_char *)LOCAL_STR;
                dce_aud_put_ev_info(ard, ev_info, &aud_status);
            }
#endif	/* defined(DCE_AUD_POINTS) */
	
	    rpriv_util_parse_req_privs(requested_privs, &req_epac, status);
	
	    if (STATUS_OK(status)) {
		rpriv_util_verify_req_privs(
                                            ard,
                                            authz_data.client_princ,
					    authz_data.client_cell,
					    &req_epac, &granted_epac,
					    status);
	    }                   
	
	    if (STATUS_OK(status)) {
		/*
		 * Verify the requested attribute set, if any.
		 * No delegation chain involved in a local
		 * get_eptgt() call.
		 */
		rpriv_util_verify_local_attrs(authz_data.client_princ,
					      &granted_epac,
					      0, 
					      NULL,
					      req_epac.num_attrs,
					      req_epac.attrs,
					      &granted_epac.num_attrs,
					      &granted_epac.attrs,
					      status);
	    }
					  
	    if (STATUS_OK(status)) {
		assign_delegation_restrictions(req_epac.deleg_type, 
					       &req_epac.deleg_restrictions, 
					       &req_epac.target_restrictions,
					       &req_epac.opt_restrictions,
					       &req_epac.req_restrictions,
					       req_epac.compat_mode,
					       &granted_epac);
		ident_expiration = 
		    get_expiration(token_expiration,
		        req_info->authent->ticket->enc_part2->times.endtime);
		/* 
		 * generate a new encoded EPAC set containing the 
		 * granted_privs and the corresponding authdata needed
		 * for the authorization data field of the EPTGT
		 */
		rpriv_util_gen_v1_1_privs(&granted_epac,
					  0,
					  NULL,
					  ident_expiration,
					  granted_privs,
					  &authdata,
					  status);
	    }

	} else {		/* This is an intercell request */
	    sec_id_epac_data_t  *epac_data_chain = NULL; 
	    unsigned32          num_epacs = 0, i;      
	    sec_id_foreign_groupset_t *fg_ptr, *fg;
    	    sec_rgy_properties_t        properties;
	    uuid_t              fc_uuid;

#if defined(DCE_AUD_POINTS)
            if (ard) {
                ev_info.format = aud_c_evt_info_char_string;
                ev_info.data.char_string = (idl_char *)INTERCELL_STR;
                dce_aud_put_ev_info(ard, ev_info, &aud_status);
	    }
#endif	/* defined(DCE_AUD_POINTS) */
        
	    /* 
	     * In an intercell get_eptgt() request, the authdata field
	     * must contain a seal over the delegation chain presented
	     * in the requested_privs parameter.  Use that seal to
	     * verify the integrity of the requested_privs parameter.
	     * If the integrity check fails, or the seal is not 
	     * present at all, then bail out.
	     */
	    SET_STATUS(status, sec_priv_s_invalid_request);
	    if (FLAG_SET(authz_data.data_parts, priv_ad_part_v1_1)
	    && authz_data.v1_1_ad.seals
            && authz_data.v1_1_ad.seals->num_seals) {
		/* 
                 * There's something in the AD seal set, but
                 * unless it's an md5 seal, we can't use it, cause
                 * that's the only kind we understand
                 */
		unsigned32 i;

		for (i = 0; i < authz_data.v1_1_ad.seals->num_seals; i++) {
		    if (authz_data.v1_1_ad.seals->seals[i].seal_type
		            == sec_id_seal_type_md5) {
			/*
                         * we've got an md5 seal, so we can use it to
                         * verify the requested privs.  We also get a
                         * a decoded epac set of at least length 1 back
			 * if the requested privs check out okay (HooHah!)
                         */
			parse_delegation_chain(
                                       ard,
                                       requested_privs,
				       &authz_data.v1_1_ad.seals->seals[i],
                                       &num_epacs,
				       &epac_data_chain,
                                       status);
			break;
		    }
		}
	    }
	    
	    /*
	     * The requested_privs may consist of EPACS from
	     * multiple cells "upstream" in the chain.  We
	     * have to verify that the path through those
	     * privilege server's cells conforms to the rules for 
	     * hierarchical trust in DCE. This requires
	     * database lookups and therefore a READ LOCK.
             *
             * FAKE-EPAC2
             * 
             * None of the heirarchical trust path evaluation code
             * is in place, so we can only support point-to-point
             * trust for the moment.  That means  That all the 
             * cell uuids in all epacs in the chain must be the same
             * as the cell uuid of the initiator.
	     */
	    if (STATUS_OK(status)) {
		rsdb_pgo_name_to_uid(sec_rgy_domain_person, 
				     authz_data.client_tgt_princ,
				     &fc_uuid,
				     status);
	    }

	    rsdb_properties_get_info(&properties, status);

	    if (STATUS_OK(status)) {
		/*
		 * loop through EPACS in request and verify that
                 * every one represents a principal in the caller's
                 * cell
                 */
		for (i = 0; i < num_epacs && STATUS_OK(status); i++) {
		    error_status_t  xst;
		    int		    j;
		    int		    num_groupsets=0;

		    if (!uuid_equal(&fc_uuid, &
				    epac_data_chain[i].pa.realm.uuid, &xst)) {
			SET_STATUS(status, sec_priv_s_invalid_trust_path);
		    }
		    /* FAKE-EPAC2
                     * As long as we're looping through the 
                     * EPAC chain, we might as well take care of
                     * intercell attribute evaluation as well.
                     * See comment immediately following this
                     * code block for a description of how we
                     * *really* need to handle attributes.
                     * For now, we just nuke all attributes
                     * from the EPAC chain.
                     */
		    epac_data_chain[i].num_attrs = 0;
		    epac_data_chain[i].attrs = NULL;

		    /* Also updating foreign_groupsets data
		     * based on the following strategy :
		     * 	- If the groupsets entry realm is not
		     *	  the caller's realm, remove it from
		     *    groupsets.
		     *	- Add/update groupset entry if the caller
		     *	  principal, which is foreign to this local cell,
		     *	  is a member of local groups.
		     */
		    fg = fg_ptr = (sec_id_foreign_groupset_t *) 
			       rpc_ss_allocate(sizeof
			       (sec_id_foreign_groupset_t)*
			       (epac_data_chain[i].pa.num_foreign_groupsets+1));
	      	    for ( j = 0; j < 
			      epac_data_chain[i].pa.num_foreign_groupsets; 
			      j++ ) {
			if ( uuid_equal(&epac_data_chain[i].pa.foreign_groupsets[j].realm.uuid,
					&properties.realm_uuid, &xst) ) {
			    /* It is from my realm for the same principal, 
			     * we are going to update this information any how,
			     * discard this possible obsolete data.
			     */
			    continue;
			}
			if ( ! uuid_equal(&epac_data_chain[i].pa.foreign_groupsets[j].realm.uuid,
					&fc_uuid, &xst) ) {
			    /* Take out this entry since its realm
			     * is not the caller's realm
			     * In the future, we may enhance to accept
			     * foreign groupsets from third party cell.
			     */
			     continue;
			}
		    }
		    /* Presumably after this loop, we should have no 
		     * valid entry in the foreign_groupsets since
		     * we take out groupsets that does not belong
		     * to the initiator and we take out the groupset
		     * information for the principal in this local cell
		     * ( if any ).
		     */
		    epac_data_chain[i].pa.num_foreign_groupsets = num_groupsets;
  		    epac_data_chain[i].pa.foreign_groupsets = fg;
		    if ( rpriv_util_get_foreign_groupset(
				&epac_data_chain[i].pa.realm.uuid,
				&epac_data_chain[i].pa.principal.uuid,
				&epac_data_chain[i].pa.foreign_groupsets[num_groupsets], 
				status) ) {
			epac_data_chain[i].pa.num_foreign_groupsets++;
		    } else {
			if ( STATUS_EQUAL(status, sec_rgy_object_not_found) ) {
				CLEAR_STATUS(status);
			}
		    }
		}

	    } else {
		SET_STATUS(status, sec_priv_s_invalid_trust_path);
	    }
                        
	    /*
	     * Peform an intercell evaluation for the attributes in
	     * each EPAC of the delegation  chain. 
	     *
	     * NOTE: If the intercell action for any attribute is
	     *       "evaluate" this will result is a trigger
	     *       function invocation that, in all probability
	     *       will cause a remote operation.  We should 
	     *
	     *                 **** NOT ****
	     *
	     *       keep the database locked during this phase.
	     *
             * FAKE-EPAC2
             *
             * We're not supporting intercell attribute evaluation
             * for DCE 1.1 beta, so, in order not to set the
             * wrong expectations, we simply reject all attributes
             * in every foreign EPAC in the chain.  We can do this by 
             * resetting the relevant pointers/counters to NULL/0
             * becuase the EPAC chain was decoded using rpc_ss_allocate(),
             * meaning all the storage will be freed by the server stub
             * on return.
	     */
	    if (STATUS_OK(status)) {
		ident_expiration = 
		    get_expiration(token_expiration,
		        req_info->authent->ticket->enc_part2->times.endtime);


		/*
                 * This is a little funky, but it lets us take
                 * advantage of the rpriv_util_gen_v1_1_privs routine
                 * exactly as written.
                 */
		rpriv_util_gen_v1_1_privs(&epac_data_chain[num_epacs - 1],
					  num_epacs - 1,
					  epac_data_chain,
					  ident_expiration,
					  granted_privs,
					  &authdata,
					  status);
	    }
	}

	/* 
	 * if we are able to satisfy the EPTGT request, then we
	 * check for auxiliary attribute and application ticket
	 * requests.
	 */

	/* Auxiliary attributes.
	 * 
	 * These are meaningless in an intercell request, So only if
	 * the request is local, we look for attribute instances of
	 * the types specified in the aux_attr_request on the node of
	 * principal for whom the EPTGT is granted.
	 */

	/* Application service ticket.
	 * 
	 * If the caller requested a ticket to a service in 
	 * our cell, then go ahead and construct a second KRB5_TGS_REP
	 * containing a ticket and session key.  
	 * 
	 * Question: can we exploit that fact that we are using the same
	 * client-supplied subkey in both KRB5_TGS_REPs to "shrink" the amount
	 * data in the second REP?
	 */
    

	/* go ahead and construct the PTGT and the PTGS response */
	if (STATUS_OK(status)) {
	    rpriv_util_build_ptgt(req_info, ident_expiration,
				  authdata, &ptgt, status);
	
	}	
	if (STATUS_OK(status)) {
	    rpriv_util_build_ptgt_rep(req_info, NULL, ptgt,
				      ptgt_rep, status);
	}

    } CATCH_ALL {
	/* 
	 * cleanup any memory *not* allocated using rpc_ss_allocate()
	 * and reraise the exception
	 */
	eptgt_common_cleanup();
	RERAISE;
    } ENDTRY;
    
#if defined(DCE_AUD_POINTS)
    if (ard) {    
        if (GOOD_STATUS(status)) {
            dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1,  
                           aud_c_esl_cond_success, &aud_status); 
        } else {  
            if (rs_util_status_is_denial(*status)) {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                               aud_c_esl_cond_denial, &aud_status);
            } else {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                               aud_c_esl_cond_failure, &aud_status);
            } 
        }
    }
#endif	/* defined(DCE_AUD_POINTS) */

    /* 
     * cleanup any memory *not* allocated using rpc_ss_allocate()
     */
    eptgt_common_cleanup();
}
