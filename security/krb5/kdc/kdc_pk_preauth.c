/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: kdc_pk_preauth.c,v $
 * Revision 1.1.2.3  1996/11/13  18:05:55  arvind
 * 	CHFts20610: free reply_key contents only if bad status; modify
 * 	reply_keyP only if good status.
 * 	[1996/11/04  21:50 UTC  aha  /main/DCE_1.2.2/aha_pk11/3]
 *
 * 	CHFts20610: Free reply_key and contents before bad-status return
 * 	from krb5_pkinit_sign_as_rep()
 * 	[1996/11/04  18:58 UTC  aha  /main/DCE_1.2.2/aha_pk11/1]
 *
 * 	CHFts20154: fix bogon error codes.
 * 	[1996/10/16  23:25 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_3/1]
 *
 * Revision 1.1.2.2  1996/10/15  20:50:58  arvind
 * 	plug memory leaks.
 * 	[1996/10/11  00:13 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_2/2]
 * 
 * 	make changes from review comments:
 * 	from cuti: cleanup externs, remove printf
 * 	CHFts20158: remove bogus free; deal with pointer sharing in checksum.
 * 	Still has memory leaks.
 * 	[1996/10/04  22:10 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_2/1]
 * 
 * Revision 1.1.2.1  1996/10/03  14:59:30  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/03  13:43:00  arvind]
 * 
 * Revision /main/DCE_1.2.2/1  1996/09/26  23:31 UTC  sommerfeld
 * 	Initial version.
 * 	[1996/09/17  00:23 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 * 
 * $EndLog$
 */

#include <krb5/krb5.h>
#include <krb5/asn1.h>
#include <dce/assert.h>
#include <dce/sec_psm.h>
#include <dce/sec_pk.h>
#include <dce/sec_psm_base.h>
#include <syslog.h>

static sec_pk_domain_t client_key_domain = {
    /* 0a3822c6-de84-11cf-99bd-0800097d5a1a */
    0x0a3822c6, 0xde84, 0x11cf, 0x99, 0xbd,
    { 0x08, 0x00, 0x09, 0x7d, 0x5a, 0x1a }
};

extern error_status_t krb5_kdc_pk_client_key_retriever (
    void *principal,
    sec_pk_domain_t *domain,
    sec_pk_usage_flags_t usage,
    unsigned32 *kvno,
    sec_pk_pubkey_t *pubkey_p
);				/* !!! belongs in header file */

void krb5_pkinit_init()
{
    error_status_t errcode;
    
    errcode = sec_pubkey_define_domain (&client_key_domain,
					krb5_kdc_pk_client_key_retriever,
					NULL);
    if (errcode) {
	syslog(LOG_ERR, "PKINIT: key domain undefinable: %s", error_message(errcode));
    }
}


/* !!! this has leaks */

krb5_error_code krb5_pkinit_decode_as_req (
    sec_psm_handle_t psm_h,
    krb5_pa_data *in,		/* [in] */
    krb5_principal kdc,		/* [in] */
    krb5_principal client,	/* [in] */    
    krb5_checksum *checksum,	/* [out] */
    krb5_int32 *cusec,		/* [out] */
    krb5_timestamp *ctime,	/* [out] */
    krb5_int32 *nonce		/* [out] */
)
{
    krb5_pa_pk_as_root *r;
    krb5_error_code st;
    krb5_data indata, *pkae;
    sec_pk_data_t pkd, insig;
    
    dce_assert(NULL, in->pa_type == KRB5_PADATA_ROOT_CERT);

    indata.data = (char *)in->contents;
    indata.length = in->length;

    st = decode_krb5_pa_pk_as_root (&indata, &r);

    if (st != 0)
	return st;
    
    /*
     * !!! should add mapping of sec_psm into kerberos encryption goop
     * to let us deal with sha_rsa, etc., etc., 
     */
    if (r->authent.authentsig.etype != ETYPE_SIGN_MD5RSA)
	return KRB5KDC_ERR_ETYPE_NOSUPP;
    
    insig.len = r->authent.authentsig.ciphertext.length;
    insig.data = (unsigned8 *)r->authent.authentsig.ciphertext.data;

    r->authent.authentsig.ciphertext.data = NULL;

    /* re-encode the signed subtree, so we can verify the sig */
    st = encode_krb5_pk_authenticator(&(r->authent.pk_authenticator), &pkae);

    if (st != 0)
	return st;
    
    pkd.len = pkae->length;
    pkd.data = (unsigned8 *)pkae->data;

    pkae->data = NULL;
    free(pkae);

    st = sec_psm_verify_data(psm_h,
			     &client_key_domain,
			     client,
			     r->authent.authentsig.kvno,
			     &md5_rsa,
			     sec_pk_usage_digitalSignature,
			     &pkd,
			     &insig);

    sec_pk_data_free(&pkd);
    sec_pk_data_free(&insig);
    
    if (st != 0)
	return st;

    /* Compare KDC principal name.. */
    if (!krb5_principal_compare(kdc, r->authent.pk_authenticator.kdcname))
	return KRB5_PREAUTH_FAILED;
    
    krb5_free_principal(r->authent.pk_authenticator.kdcname);

    /* Extract checksum */
    if (r->authent.pk_authenticator.cksum) {
	*checksum = *r->authent.pk_authenticator.cksum;
	r->authent.pk_authenticator.cksum->contents = NULL;
    } else {
	checksum->checksum_type = 0;
	checksum->length = 0;
	checksum->contents = NULL;
    }
    *cusec = r->authent.pk_authenticator.cusec;
    *ctime = r->authent.pk_authenticator.ctime;
    *nonce = r->authent.pk_authenticator.nonce;

    free(r->authent.pk_authenticator.cksum);
    free(r);
    return 0;
}

krb5_error_code krb5_pkinit_sign_as_rep (sec_psm_handle_t psm_h,
					 krb5_principal kdc,
					 krb5_principal client,
					 krb5_enctype useetype, /* sess key type */
					 krb5_keyblock **reply_keyP, /* out */
					 krb5_int32 nonce,
					 krb5_pa_data *out)
{
    krb5_enc_pa_pk_as_rep_part_shell ir;
    krb5_pa_pk_as_rep rep;
    krb5_keyblock *tmpkey=NULL;
    krb5_keyblock *reply_key=NULL;
    krb5_error_code st;

    krb5_data *irdata=NULL;
    krb5_data *enckey=NULL;
    krb5_data *repe=NULL;
    unsigned32 irsigvno = 0;
    unsigned32 enckvno;
    
    sec_pk_data_t ird, irsig, tmpkey_encoded, tmpkey_encrypted;
    
    /* generate two keys: envelope key, and reply key */

    st = ((*(krb5_csarray[useetype]->system->random_key))
	  (krb5_csarray[useetype]->random_sequence, &reply_key));

    if (st != 0)
	return st;
    
    ir.reply.enc_reply_key.keytype = reply_key->keytype;
    ir.reply.enc_reply_key.length = reply_key->length;
    ir.reply.enc_reply_key.contents = reply_key->contents; /* shared structure.. */
    ir.reply.nonce = nonce;

    /*
     * Encode it for signature.
     */

    st = encode_krb5_enc_pa_pk_as_rep_part(&ir.reply, &irdata);
    if (st != 0)
        goto cleanup;

    ird.len = irdata->length;
    ird.data = (unsigned8 *)irdata->data;
    free(irdata);
    irdata = NULL;

    /* Sign it with KDC's private key */

    st = sec_psm_sign_data (psm_h,
			    &md5_rsa,
			    sec_pk_usage_digitalSignature,
			    &ird,
			    &irsigvno,
			    &irsig);

    sec_pk_data_free(&ird);

    if (st != error_status_ok)
	goto cleanup;

    ir.replysig.kvno = irsigvno;
    ir.replysig.etype = ETYPE_SIGN_MD5RSA;
    ir.replysig.ciphertext.data = (char *)irsig.data;
    ir.replysig.ciphertext.length = irsig.len;

    irsig.data = NULL;
    /* encode this pile */

    st = encode_krb5_enc_pa_pk_as_rep_part_shell(&ir, &irdata);

    free(ir.replysig.ciphertext.data);

    if (st != 0)
	goto cleanup;

    /* generate a temporary key .. */
    
    st = ((*(krb5_csarray[useetype]->system->random_key))
	  (krb5_csarray[useetype]->random_sequence, &tmpkey));

    if (st != 0)
	goto cleanup;

    /* secret-key encrypt the shell using tmpkey */

    st = krb5_encrypt_data(NULL, tmpkey, NULL, irdata, &rep.enc_shell);

    if (st != 0)
	goto cleanup;
    
    /* encode the temporary key */

    st = encode_krb5_enc_pa_pk_tmp_key(tmpkey, &enckey);

    if (st != 0)
	goto cleanup;
    
    
    /* public-key encrypt the temporary key */
    tmpkey_encoded.data = (unsigned char *)enckey->data;
    tmpkey_encoded.len = enckey->length;
    free(enckey);

    enckvno = 0;

    st = sec_psm_encrypt_data (psm_h,
			       &client_key_domain,
			       client,
			       &enckvno,
			       &rsa_enc,
			       sec_pk_usage_keyEncipherment,
			       &tmpkey_encoded,
			       &tmpkey_encrypted);
    sec_pk_data_zero_and_free(&tmpkey_encoded);

    if (st != 0)
	goto cleanup;

    /* encrypt the shell */

    /* fill in the top-level request */

    rep.kdc_cert.data = NULL;
    rep.kdc_cert.length = 0;

    rep.enc_tmp_key.etype = ETYPE_ENCRYPT_RSA_PRIV;
    rep.enc_tmp_key.kvno = enckvno;
    rep.enc_tmp_key.ciphertext.data = (char *)tmpkey_encrypted.data;
    rep.enc_tmp_key.ciphertext.length = tmpkey_encrypted.len;

    st = encode_krb5_pa_pk_as_rep(&rep, &repe);

    sec_pk_data_free(&tmpkey_encrypted);
    free(rep.enc_shell.ciphertext.data);

    if (st != 0)
	goto cleanup;

    out->pa_type = KRB5_PADATA_PUBLIC_REP;
    out->length = repe->length;
    out->contents = (unsigned char *)(repe->data);

cleanup:
    if (st != 0) {
       if (reply_key)
           krb5_free_keyblock(reply_key);
    } else {
       *reply_keyP = reply_key;
       free(reply_key);
    }
    if (tmpkey)
       krb5_free_keyblock(tmpkey);
    if (irdata) {
        if (irdata->data)
            free(irdata->data);
        free(irdata);
    }
    if (repe)
        free(repe);
    return st;
}


