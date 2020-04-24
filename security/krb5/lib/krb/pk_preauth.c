/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: pk_preauth.c,v $
 * Revision 1.1.2.4  1996/11/13  18:08:20  arvind
 * 	CHFts20151: rip out #if 0 crud
 * 	CHFts20154: fix bogon error codes
 * 	NO DTS: clean up references to algorithm uuid's.
 * 	[1996/10/16  23:28 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_3/1]
 *
 * Revision 1.1.2.3  1996/10/15  20:52:26  arvind
 * 	OSF DCE 1.2.2 Drop 4.5
 * 	[1996/10/15  20:16:58  arvind]
 * 
 * Revision 1.1.2.2  1996/10/04  16:54:34  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/04  16:54:21  arvind]
 * 
 * Revision /main/DCE_1.2.2/2  1996/10/03  18:24 UTC  sommerfeld
 * 	Fix CHFts20134: krb5_pkinit_sign_as_req() passing uninit kvno
 * 	[1996/10/03  17:05 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_2/1]
 * 
 * Revision /main/DCE_1.2.2/1  1996/09/26  23:31 UTC  sommerfeld
 * 	Use "built-in" KDC key retrieval domain instead of rolling our own.
 * 	[1996/09/21  03:12 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/2]
 * 	 *
 * 	Implement AS_REP decode side.
 * 	[1996/07/10  21:15 UTC  sommerfeld  /main/sommerfeld_pk_kdc/4]
 * 	 *
 * 	First cut coding of preauth generation.
 * 	[1996/06/06  21:51 UTC  sommerfeld  /main/sommerfeld_pk_kdc/1]
 * 
 * $EndLog$
 */

#include <krb5/krb5.h>
#include <krb5/asn1.h>
#include <dce/sec_psm.h>
#include <dce/sec_pk.h>
#include <dce/sec_pubkey.h>
#include <dce/uuid.h>

error_status_t dce_cell_from_princ (krb5_principal princ,
				    sec_rgy_name_t cell)
{
    /* !!! this isn't quite correct.. */
    krb5_data *realm = krb5_princ_realm(princ);

    /* !!! should use snprintf */
    sprintf((char *)cell, "/.../%.*s", realm->length, realm->data);
    return error_status_ok;
}

/* !!! next two functions belong elsewhere.... */
/*
 * This routine takes a key and a krb5_data structure as input, and
 * outputs the encrypted data in a krb5_enc_data structure.  Note that
 * the krb5_enc_data structure is not allocated, and the kvno field is
 * not filled in.
 */
krb5_error_code
krb5_encrypt_data(context, key, ivec, data, enc_data)
    krb5_context	context;
    krb5_keyblock *	key;
    krb5_pointer	ivec;
    krb5_data *		data;
    krb5_enc_data *	enc_data;
{
    krb5_error_code	retval;
    krb5_encrypt_block	eblock;

    krb5_use_enctype(/*context, */&eblock, key->keytype);

#if 0
    enc_data->magic = KV5M_ENC_DATA;
#endif
    enc_data->kvno = 0;
    enc_data->etype = key->keytype;
    enc_data->ciphertext.length = krb5_encrypt_size(data->length,
						    eblock.crypto_entry);
    enc_data->ciphertext.data = malloc(enc_data->ciphertext.length);
    if (enc_data->ciphertext.data == 0)
	return ENOMEM;

    if ((retval = krb5_process_key(/*context, */&eblock, key)) != 0)
	goto cleanup;

    if ((retval = krb5_encrypt(/*context, */(krb5_pointer) data->data,
			       (krb5_pointer) enc_data->ciphertext.data,
			       data->length, &eblock, ivec))) {
    	krb5_finish_key(/*context, */&eblock);
        goto cleanup;
    }
    (void) krb5_finish_key(/*context, */&eblock);

    return 0;

cleanup:
    free(enc_data->ciphertext.data);
    return retval;
}

/*
 * This routine takes a key and a krb5_enc_data structure as input, and
 * outputs the decrypted data in a krb5_data structure.  Note that
 * the krb5_data structure is not allocated.
 */
krb5_error_code
krb5_decrypt_data(context, key, ivec, enc_data, data)
    krb5_context	context;
    krb5_keyblock *	key;
    krb5_pointer	ivec;
    krb5_enc_data *	enc_data;
    krb5_data *		data;
{
    krb5_error_code	retval;
    krb5_encrypt_block	eblock;

    krb5_use_enctype(/*context, */&eblock, key->keytype);
    data->length = enc_data->ciphertext.length;
    if (!(data->data = malloc(data->length)))
	return ENOMEM;

    if ((retval = krb5_process_key(/*context, */&eblock, key)) != 0)
	goto cleanup;

    if ((retval = krb5_decrypt(/*context, */
			       (krb5_pointer) enc_data->ciphertext.data,
			       (krb5_pointer) data->data,
			       enc_data->ciphertext.length, &eblock, ivec))) {
    	krb5_finish_key(/*context,*/ &eblock);
        goto cleanup;
    }
    (void) krb5_finish_key(/*context, */&eblock);

    return 0;

cleanup:
    if (data->data) {
	free(data->data);
	data->data = 0;
    }
    return retval;
}
/*
 * Generate and sign a PA-PK-AS-ROOT.
 *
 * This is a middle-level encode routine.
 */

krb5_error_code krb5_pkinit_sign_as_req (
    sec_psm_handle_t psm_h,
    krb5_principal kdc,
    krb5_checksum *checksum,
    krb5_int32 cusec,
    krb5_timestamp ctime,
    krb5_int32 nonce,
    krb5_pa_data *out
)
{
    krb5_pa_pk_as_root r;
    krb5_data *pkae;
    sec_pk_data_t pkd;
    sec_pk_signed_t outsig;
    error_status_t st;
    unsigned32 sigvno = 0;
    
    r.root_cert.data = NULL;
    r.root_cert.length = 0;

    /* construct the pk_authenticator message */

    r.authent.pk_authenticator.cksum = checksum;
    r.authent.pk_authenticator.ctime = ctime;
    r.authent.pk_authenticator.cusec = cusec;    
    r.authent.pk_authenticator.nonce = nonce;
    r.authent.pk_authenticator.kdcname = kdc;

    /* encode just this subtree, so we can sign it */
    st = encode_krb5_pk_authenticator(&r.authent.pk_authenticator, &pkae);

    if (st != error_status_ok)
	return st;
    
    pkd.len = pkae->length;
    pkd.data = (unsigned8 *)pkae->data;
    
    /* sign it */
    st = sec_psm_sign_data(psm_h,
			   &md5_rsa,
			   sec_pk_usage_digitalSignature,
			   &pkd,
			   &sigvno,
			   &outsig);
    free(pkae->data);
    free(pkae);
    if (st != error_status_ok)
	return st;

    /* Now, fill in the signature field, and encode it */

    r.authent.authentsig.kvno = sigvno;
    r.authent.authentsig.etype = ETYPE_SIGN_MD5RSA;
    r.authent.authentsig.ciphertext.data = (char *)outsig.data;
    r.authent.authentsig.ciphertext.length = outsig.len;

    st = encode_krb5_pa_pk_as_root(&r, &pkae);

    sec_pk_data_free(&outsig);

    if (st != error_status_ok)
	return st;

    /* and fill in the outer framing */
    out->pa_type = KRB5_PADATA_ROOT_CERT;
    out->length = pkae->length;
    out->contents = (krb5_octet *)pkae->data;
    free(pkae);

    return 0;
}

/* decode a PA-PK-AS-REP */

    
krb5_error_code krb5_pkinit_decode_as_rep (sec_psm_handle_t psm_h,
					   krb5_pa_data *in,
					   krb5_principal kdc,
					   krb5_principal client,
					   krb5_keyblock *reply_key,
					   krb5_int32 *nonce)
{
    krb5_enc_pa_pk_as_rep_part_shell *ir;
    krb5_pa_pk_as_rep *rep;
    krb5_keyblock *tmpkey;
    krb5_error_code st;
    krb5_data irdata, *irsigdata;
    krb5_data enckey;
    krb5_data irout;
    krb5_data repe;
    sec_rgy_name_t kdc_cell;
    
    sec_pk_data_t tmpkey_encoded, tmpkey_encrypted, insig, irmsg;

    repe.length = in->length;
    repe.data = (char *)in->contents;
    
    st = decode_krb5_pa_pk_as_rep(&repe, &rep);
    if (st != 0) return st;

    tmpkey_encrypted.data = (unsigned8 *)rep->enc_tmp_key.ciphertext.data;
    tmpkey_encrypted.len = rep->enc_tmp_key.ciphertext.length;    

    st = sec_psm_decrypt_data(psm_h,
			      rep->enc_tmp_key.kvno,
			      &rsa_enc,
			      sec_pk_usage_keyEncipherment,
			      &tmpkey_encrypted,
			      &tmpkey_encoded);
    if (st != 0) return st;

    enckey.data = (char *)tmpkey_encoded.data;
    enckey.length = tmpkey_encoded.len;
    
    st = decode_krb5_enc_pa_pk_tmp_key(&enckey, &tmpkey);
    if (st != 0) return st;

    st = krb5_decrypt_data(NULL, tmpkey, NULL, &rep->enc_shell, &irdata);
    if (st != 0) return st;

    st = decode_krb5_enc_pa_pk_as_rep_part_shell(&irdata, &ir);
    if (st != 0) return st;

    /*
     * !!! should add mapping of sec_psm into kerberos encryption goop
     * to let us deal with sha_rsa, etc., etc., 
     */
    if (ir->replysig.etype != ETYPE_SIGN_MD5RSA)
	return KRB5_PROG_ETYPE_NOSUPP;
    
    insig.data = (unsigned8 *)ir->replysig.ciphertext.data;
    insig.len = ir->replysig.ciphertext.length;    
    
    st = encode_krb5_enc_pa_pk_as_rep_part(&ir->reply, &irsigdata);
    if (st != 0) return st;

    irmsg.data = (unsigned8 *)irsigdata->data;
    irmsg.len = irsigdata->length;
    
    (void) dce_cell_from_princ(kdc, kdc_cell);

    st = sec_psm_verify_data(psm_h,
			     &sec_pk_domain_kdc_cache,
			     kdc_cell,
			     ir->replysig.kvno,
			     &md5_rsa, sec_pk_usage_digitalSignature,
			     &irmsg,
			     &insig);
    if (st != 0) {
	st = sec_psm_verify_data(psm_h,
				 &sec_pk_domain_kdc_cache_refresh,
				 kdc_cell,
				 ir->replysig.kvno,
				 &md5_rsa, sec_pk_usage_digitalSignature,
				 &irmsg,
				 &insig);
    }

    *nonce = ir->reply.nonce;
    reply_key->keytype = ir->reply.enc_reply_key.keytype;
    reply_key->length = ir->reply.enc_reply_key.length;
    reply_key->contents = ir->reply.enc_reply_key.contents;
    return st;
}


