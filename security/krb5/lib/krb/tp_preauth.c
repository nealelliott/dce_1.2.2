/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: tp_preauth.c,v $
 * Revision 1.1.2.1  1996/11/13  18:08:58  arvind
 * 	OSF DCE 1.2.2 Drop 5
 * 	[1996/11/13  17:49:29  arvind]
 *
 * Revision /main/DCE_1.2.2/4  1996/11/05  22:33 UTC  aha
 * 	CHFts20612: fix typo in test for matching salt
 * 	[1996/11/05  19:03 UTC  aha  /main/DCE_1.2.2/aha_pk11/2]
 * 
 * Revision /main/DCE_1.2.2/3  1996/11/01  22:07 UTC  aha
 * 	CHFts20612
 * 
 * Revision /main/DCE_1.2.2/aha_pk11/1  1996/11/01  22:03 UTC  aha
 * 	CHFts20612: in krb5_decode_dce_edata(), consider dce_err_data salt
 * 	(->contents) a match to the default_salt if the dce_err_data->length
 * 	is 0.
 * 
 * Revision /main/DCE_1.2.2/2  1996/10/31  19:36 UTC  mullan_s
 * 	Merge to DCE 1.2.2
 * 
 * Revision /main/DCE_1.2.2/mullan_dce12_tp_preauth2/1  1996/10/30  23:01 UTC  mullan_s
 * 	CHFts20531: Fix non-default salt error handling along with
 * 	various memory corruption fixes.
 * 
 * Revision /main/DCE_1.2.2/1  1996/10/28  20:45 UTC  mullan_s
 * 	Merge to DCE_1.2.2
 * 
 * Revision /main/mullan_dce12_tp_preauth/5  1996/10/24  18:13 UTC  mullan_s
 * 	Fix DCE e_data processing (1st pass).
 * 
 * Revision /main/mullan_dce12_tp_preauth/4  1996/10/23  16:12 UTC  mullan_s
 * 	Fix SEGV in krb5_decode_dce_edata
 * 
 * Revision /main/mullan_dce12_tp_preauth/3  1996/10/21  16:54 UTC  mullan_s
 * 	Add extern ref to make_preauth_list().
 * 	Add padding to DCE third party enc_data to be compatible with old code.
 * 
 * Revision /main/mullan_dce12_tp_preauth/2  1996/10/15  13:30 UTC  mullan_s
 * 	Add routine to decode DCE specific data in AS error data.
 * 
 * Revision /main/mullan_dce12_tp_preauth/1  1996/10/08  14:23 UTC  mullan_s
 * 	OSF DCE Third Party preauthentication construction code.
 * 
 * $EndLog$
 */
/*
**
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/

/*
 * This file contains routines for construction of OSF DCE Third Party
 * preauthentication data. Currently this includes the subtypes 
 * OSF_DCE_THIRD_PARTY and OSF_DCE_THIRD_PARTY_PLAINTEXT of the krb5
 * registered preauth type KRB5_PADATA_OSF_DCE.
 */

#include <krb5/krb5.h>
#include <krb5/preauth.h>
#include <sec_login_pvt.h>

/* XXX These should be defined in a header file somewhere */
typedef krb5_error_code (*git_key_proc) PROTOTYPE((krb5_context,
                                                   const krb5_enctype,
                                                   krb5_data *,
                                                   krb5_const_pointer,
                                                   krb5_keyblock **));
extern krb5_error_code pwd_keyproc(krb5_context ctxt,
                                   krb5_const krb5_enctype type,
                                   krb5_data *salt,
                                   krb5_const_pointer keyseed,
                                   krb5_keyblock **key);

extern krb5_error_code make_preauth_list(krb5_context context,
                                         krb5_preauthtype *ptypes,
                                         krb5_pa_data ***ret_list,
					 krb5_pa_misc_data ***pa_misc_data);

krb5_error_code krb5_obtain_dce_tp_preauth (
    krb5_context	context,
    krb5_preauthtype    preauth_subtype,
    krb5_principal      client,
    krb5_keyblock       *user_key,
    krb5_const_pointer  key_seed,
    krb5_keyblock       *reply_random_key,
    krb5_pa_data        *pa_data
)
{
    krb5_error_code	retval = 0;
    krb5_keyblock       random_key;
    krb5_time_and_key   replyblock;
    krb5_data           *replyblock_data = NULL;
    krb5_enc_data       single_ebuf;
    krb5_data           *single_ebuf_data = NULL;
    krb5_enc_data       double_ebuf;
    krb5_pa_third_party third_party;
    krb5_data           *third_party_data = NULL;
    krb5_keyblock       *reply_key = NULL;
    krb5_sub_pa_data    dce_padata;
    krb5_data           *encoded_dce_padata = NULL;
    krb5_dce_pa_keys    dce_pa_keys;
    int			new_len = 0;
    krb5_encrypt_block  eblock;

    /* check simple error cases */
    if ((preauth_subtype != OSF_DCE_THIRD_PARTY_PLAINTEXT) &&
        (preauth_subtype != OSF_DCE_THIRD_PARTY))
        return KRB5_PREAUTH_FAILED;  

    if (((preauth_subtype == OSF_DCE_THIRD_PARTY_PLAINTEXT) &&
         (key_seed == NULL)) ||
        ((preauth_subtype == OSF_DCE_THIRD_PARTY) &&
         (user_key->keytype != KEYTYPE_DES)))
        return KRB5_PREAUTH_FAILED;

    /* initialize */
    memset(&random_key, 0, sizeof(random_key));
    memset(&third_party, 0, sizeof(third_party));
    memset(&replyblock, 0, sizeof(replyblock));
    memset(&dce_pa_keys, 0, sizeof(dce_pa_keys));
    memset(&single_ebuf, 0, sizeof(single_ebuf));
    memset(&double_ebuf, 0, sizeof(double_ebuf));
    memset(&dce_padata, 0, sizeof(dce_padata));

#define cleanup_krb5_data(dat) { \
    if (dat->data) { \
        memset((char *) (dat->data), 0, dat->length); \
        krb5_free_data(dat); \
    } \
}

#define free_keyblock_contents(key) { \
    if (key.contents) { \
	memset((char *)key.contents, 0, key.length); \
	xfree(key.contents); \
    } \
}

    /*
     * get trusted portion of the padata. Both the machine
     * TGT and the encrypted data are in ASN.1 encoded form.
     */
    sec_login_pvt_get_trusted_preauth(client, &third_party.machine_tgt_data,
            &third_party.single_ebuf_data, &random_key, (error_status_t *) &retval);
    if (retval) {
        return KRB5_PREAUTH_FAILED;
    }

    /* generate the reply-random-key */
    if ((retval = krb5_generate_subkey(&random_key, &reply_key)))
	goto cleanup;

    if (preauth_subtype == OSF_DCE_THIRD_PARTY) {
        /* get timestamp */
        if ((retval = krb5_timeofday(&replyblock.timestamp))) 
	    goto cleanup;

        /* copy reply-random-key to replyblock */
        if ((retval = krb5_copy_keyblock_contents(reply_key, &replyblock.key))) 
	    goto cleanup;

        /* encode replyblock */
        retval = encode_krb5_time_and_key(&replyblock, &replyblock_data);
        /* free replyblock.key */
        free_keyblock_contents(replyblock.key);
        if (retval)
	    goto cleanup;

	krb5_use_enctype(&eblock, user_key->keytype);
	new_len = krb5_encrypt_size(replyblock_data->length, eblock.crypto_entry);
	if (!(replyblock_data->data = realloc(replyblock_data->data, new_len))) {
	    retval = ENOMEM;
	    goto cleanup;
	}
	memset(replyblock_data->data + replyblock_data->length, 0, new_len - replyblock_data->length);

        /* encrypt replyblock_data in user's key to get single_ebuf */
	if ((retval = krb5_encrypt_data(context, user_key, 0, replyblock_data, &single_ebuf)))
	    goto cleanup;

        /* encode single_ebuf */
        retval = encode_krb5_enc_data(&single_ebuf, &single_ebuf_data);
        free(single_ebuf.ciphertext.data);
        if (retval)
	    goto cleanup;
    } else {
        /* copy reply-random-key to dce_pa_keys */
        if ((retval = krb5_copy_keyblock_contents(reply_key, &dce_pa_keys.reply_key)))
	    goto cleanup;

        /* copy user's key to dce_pa_keys */
	/* XXX assumption : key_seed points to the password */
        dce_pa_keys.user_key.data = ((krb5_data*)(key_seed))->data;
        dce_pa_keys.user_key.length = ((krb5_data*)(key_seed))->length;

        /* encode dce_pa_keys */
        retval = encode_krb5_dce_pa_keys(&dce_pa_keys, &single_ebuf_data);
        /* free key */
        free_keyblock_contents(dce_pa_keys.reply_key);
        if (retval)
	    goto cleanup;
    }

    krb5_use_enctype(&eblock, random_key.keytype);
    new_len = krb5_encrypt_size(single_ebuf_data->length, eblock.crypto_entry);
    if (!(single_ebuf_data->data = realloc(single_ebuf_data->data, new_len))) {
        retval = ENOMEM;
        goto cleanup;
    }
    memset(single_ebuf_data->data + single_ebuf_data->length, 0, new_len - single_ebuf_data->length);

    /* encrypt single_ebuf_data in random key to get double_ebuf */
    if ((retval = krb5_encrypt_data(context, &random_key, 0, single_ebuf_data, &double_ebuf)))
	goto cleanup;

    /* encode double_ebuf */
    retval = encode_krb5_enc_data(&double_ebuf, &third_party.double_ebuf_data);
    free(double_ebuf.ciphertext.data);
    if (retval)
	goto cleanup;

    /* encode third_party */
    if ((retval = encode_krb5_pa_third_party(&third_party, &third_party_data)))
	goto cleanup;

    /* compose subtyped OSF DCE padata */
    dce_padata.pa_subtype = preauth_subtype;
    dce_padata.length = third_party_data->length;
    dce_padata.contents = (krb5_octet *) third_party_data->data;

    /* free third_party_data (only pointer, not contents!) */
    free(third_party_data);

    /* encode subtyped OSF DCE padata */
    retval = encode_krb5_sub_pa_data(&dce_padata, &encoded_dce_padata);
    /* free dce_padata */
    if (dce_padata.contents)
        free(dce_padata.contents);
    if (retval)
	goto cleanup;

    /* copy to padata */
    pa_data->pa_type = KRB5_PADATA_OSF_DCE;
    pa_data->length = encoded_dce_padata->length;
    pa_data->contents = (krb5_octet *) encoded_dce_padata->data;
    krb5_copy_keyblock_contents(reply_key, reply_random_key);

    free(encoded_dce_padata);

cleanup:

    free_keyblock_contents(random_key);
    if (third_party.machine_tgt_data)
	cleanup_krb5_data(third_party.machine_tgt_data);
    if (third_party.single_ebuf_data)
	cleanup_krb5_data(third_party.single_ebuf_data);
    if (third_party.double_ebuf_data)
	cleanup_krb5_data(third_party.double_ebuf_data);
    if (reply_key)
        krb5_free_keyblock(reply_key);
    if (replyblock_data)
        cleanup_krb5_data(replyblock_data);
    if (single_ebuf_data)
	cleanup_krb5_data(single_ebuf_data);

    return(retval);
}

krb5_error_code krb5_decode_dce_edata(
    krb5_context	context,
    krb5_data		edata,
    krb5_error_code	as_req_retval,
    krb5_preauthtype	*prev_ptypes,
    krb5_kdc_req 	request,
    git_key_proc        key_proc,
    krb5_const_pointer  keyseed,
    krb5_pa_data	***preauth_to_retry,
    krb5_pa_misc_data   ***preauth_misc_data
)
{
    krb5_error_code	retval = 0;
    krb5_sub_err_data	*dce_err_data = NULL;
    krb5_pa_data	**pa = NULL, **pap;
    krb5_preauthtype	*patypes = NULL, *patypesp = NULL;
    krb5_data		default_salt, *entry_data = NULL;
    krb5_pa_misc_data   **pa_misc = NULL;
    krb5_etype_info_entry **entry = 0;
    int			size = 0;

    if (edata.length == 0)
        return 0;

    /* initialize */
    memset(&default_salt, 0, sizeof(default_salt));

    /*
     * KDC failed to decrypt the padata.
     * See what is contained in the edata packet.
     */
    if ((retval = decode_krb5_sub_err_data(&edata, &dce_err_data)))
        /*
         * This isn't fatal, because we could be getting non-DCE
         * specific error data.
         */
	return 0;

    switch(dce_err_data->err_subtype) {
	case OSF_DCE_ERR_PREAUTH:
	    /* XXX - Still need to do this */
	case OSF_DCE_ERR_PREAUTH_SALT:
	    /* recompute key, if necessary */
	    if (key_proc != pwd_keyproc) {
		/* salts do not apply to strong keys, so restore
		 * original return value from AS request and return.
                 */
		retval = as_req_retval;
		goto cleanup;
	    }
	    /* compare salt with what was used previously,
	     * if they are identical, there is no point in retrying
             * because the user's password is probably incorrect
	     */
	    if ((retval = krb5_principal2salt(request.client, 
					      &default_salt)))
		goto cleanup;
            /* CHFts20612: if client has default salt, dce_err_data->length
             * is set to 0 by kdc_preauth.c/prepare_enc_tp_error_edata(),
             * so consider it a match in this case
             */
	    if ((dce_err_data->length == 0) ||
               ((dce_err_data->length == default_salt.length) &&
                 (memcmp(default_salt.data, dce_err_data->contents, 
		        dce_err_data->length) == 0))) {
		retval = as_req_retval;
		goto cleanup;
	    }
	    if (!(entry = malloc (2*sizeof(krb5_etype_info_entry *)))) {
		retval = ENOMEM;
		goto cleanup;	
	    }

            entry[1] = NULL;
            if (!(entry[0] = malloc (sizeof (krb5_etype_info_entry)))) {
		retval = ENOMEM;
		goto cleanup;
	    }
            entry[0]->etype = ETYPE_DES_CBC_CRC;
	    entry[0]->salt = (unsigned char *) dce_err_data->contents;
	    entry[0]->length = dce_err_data->length;
	    free(dce_err_data);
	    dce_err_data = 0;
            if ((retval = encode_krb5_etype_info(entry, &entry_data)))
                goto cleanup;
	    /* Add ETYPE padata entry to previously requested preauth types */
	    for (patypesp = prev_ptypes, size=0; *patypesp; patypesp++, size++);
	    if (!(patypes = malloc(sizeof(krb5_preauthtype *)+(sizeof(krb5_preauthtype)*(size+2))))) {
		retval = ENOMEM;
		goto cleanup;
	    }
	    memcpy(&patypes[0], &prev_ptypes[0], (sizeof(krb5_preauthtype)*size)); 
	    patypes[size++] = KRB5_PADATA_ETYPE_INFO;
	    patypes[size] = 0;
	    break;
	case OSF_DCE_ERR_PREAUTH_INTERCELL:
	    if (!(patypes = malloc(sizeof(*patypes)*3))) {
		retval = ENOMEM;
		goto cleanup;
            }
	    patypes[0] = KRB5_PADATA_ENC_UNIX_TIME;
	    patypes[1] = KRB5_PADATA_ENC_TIMESTAMP; 
	    patypes[2] = 0;
	    break;
	case OSF_DCE_ERR_ADMIN_VALID:
	    retval = sec_login_s_acct_invalid;
	    goto cleanup;
	default:
	    retval = as_req_retval;
	    goto cleanup;
    }

    if ((retval = make_preauth_list(context, patypes, &pa, &pa_misc)))
	goto cleanup;

    /* Fill in ETYPE_INFO entry */
    if (entry_data) {
        for (pap = pa; *pap; pap++) {
            if ((*pap)->pa_type == KRB5_PADATA_ETYPE_INFO) {
	        (*pap)->length = entry_data->length;
		(*pap)->contents = (unsigned char *)entry_data->data;
		free(entry_data);
		entry_data = 0;
		break;
	    }
	}
    }

    *preauth_to_retry = pa;
    *preauth_misc_data = pa_misc;

cleanup:
    if (default_salt.data)
        krb5_xfree(default_salt.data);
    if (dce_err_data) {
	if (dce_err_data->contents)
	    free(dce_err_data->contents);
	free(dce_err_data);
    }
    if (entry)
        krb5_free_etype_info(context, entry);
    if (entry_data)
	krb5_free_data(entry_data);
    if (patypes)
	free(patypes);

    return(retval);
}
