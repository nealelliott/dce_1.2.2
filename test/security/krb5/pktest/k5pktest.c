/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: k5pktest.c,v $
 * Revision 1.1.2.1  1996/10/03  20:37:09  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/03  19:45:36  arvind]
 *
 * 	OSF DCE 1.2.2 Drop 4
 *
 * Revision /main/DCE_1.2.2/1  1996/09/26  23:32 UTC  sommerfeld
 * 	Quick & dirty pkinit internals test.
 * 
 * $EndLog$
 */
#include <stdio.h>
#include <stdlib.h>
#include <dce/sec_pk.h>
#include <dce/sec_psm.h>
#include <dce/sec_pubkey.h>
#include <dce/uuid.h> 
#include <krb5/krb5.h>
#include <krb5/kdb.h>
#include <com_err.h>

#include "kdc_util.h"

#include "pkcommon.h"

extern krb5_error_code krb5_pkinit_sign_as_req (
    sec_psm_handle_t psm_h,
    krb5_principal kdc,
    krb5_checksum *checksum,
    krb5_int32 cusec,
    krb5_timestamp ctime,
    krb5_int32 nonce,
    krb5_pa_data *out
);


krb5_error_code krb5_pkinit_decode_as_rep (sec_psm_handle_t psm_h,
					   krb5_pa_data *in,
					   krb5_principal kdc,
					   krb5_principal client,
					   krb5_keyblock *reply_key,
					   krb5_int32 *nonce);



extern sec_pk_algorithm_id_t rsadsi; /* !!! */

struct pubkey_table 
{
    void *principal;
    sec_pk_usage_flags_t usage;
    sec_pk_pubkey_t *key;
} pk_table[10];

error_status_t sec_pubkey_retrieve (
    void                   *principal,
    sec_pk_domain_t        *domain,
    sec_pk_usage_flags_t   usage,
    unsigned32             *kvno,
    sec_pk_pubkey_t        *pubkey_p
) 
{
    int i;
    
    for (i=0; i<10; i++) {
	if ((pk_table[i].usage & usage) &&
	    (pk_table[i].principal == principal)) {
	    printf("returned pubkey %d\n", i);
	    return sec_pk_data_copy(pk_table[i].key, pubkey_p);
	}
    }
    return 0xbeeff00d;
}

error_status_t sec_pubkey_store (
    void                  *principal,   /* [in] */
    sec_pk_domain_t       *domain,    /* [in] */
    sec_pk_usage_flags_t  usage,     /* [in] */
    unsigned32            *kvno,      /* [in] */
    sec_pk_pubkey_p_t     pubkey_p   /* [in] */
)
{
    int i;
    
    for (i=0; i<10; i++) {
	if (pk_table[i].principal == NULL) {
	    pk_table[i].principal = principal;
	    pk_table[i].usage = usage;
	    pk_table[i].key = pubkey_p;
	    return 0;
	}
    }
    return 0;
}

error_status_t
    sec_pubkey_retrieve_kdc_key(sec_rgy_name_t *cell,
				sec_pk_usage_flags_t usage,
				boolean32 refresh,
				sec_pk_pubkey_t *key)
{
    abort();
}

void init_random()
{
    int etype;
    krb5_keyblock master_keyblock;
    krb5_error_code retval;
    unsigned char dummykey[8] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};
    master_keyblock.keytype = 1;
    master_keyblock.length = 8;
    master_keyblock.contents = dummykey;

    for (etype = 0; (int) etype <= krb5_max_cryptosystem; etype++) {
	if (krb5_csarray[etype]) {
	    printf("init random sequence for etype %d\n", etype);
	    retval = (*krb5_csarray[etype]->system->
		      init_random_key)(&master_keyblock,
				       &krb5_csarray[etype]->random_sequence);
	    if (retval != 0) {
		com_err("k5pktest", retval, "while setting up random key generator for etype %d--etype disabled", etype);
		krb5_csarray[etype] = 0;
	    }
	}
    }
}

extern void cma_lib_malloc_bomb(int count);

int main(int argc, char **argv)
{
    uuid_t dummy;
    krb5_int32 cusec, nonce;
    krb5_timestamp ctime;
    char krbtgtstring[1024];
    int i;
    
    sec_pk_data_t user_sig_pubkey;
    sec_pk_data_t user_sig_privkey;

    sec_pk_data_t user_enc_pubkey;
    sec_pk_data_t user_enc_privkey;

    sec_pk_data_t kdc_sig_pubkey;
    sec_pk_data_t kdc_sig_privkey;

    sec_psm_handle_t psmh_user;
    sec_psm_handle_t psmh_kdc;    
    krb5_principal kdcname;
    krb5_principal username;
    krb5_keyblock *reply_key;
    krb5_keyblock other_key;
    krb5_int32 other_nonce;
#if 0
    krb5_checksum cksum;
#endif
    krb5_pa_data pa_out;
    sec_pk_data_t pa_fu_out;
    
    error_status_t status = 0;

    uuid_create_nil(&dummy, &status);
    
    init_random();
    
    if ((test_load_data(&user_sig_pubkey)) != 0) {
	printf("Bad input user_sig_pubkey.. giving up..\n");
	exit(1);
    }
    if ((test_load_data(&user_sig_privkey)) != 0) {
	printf("Bad input user_sig_privkey.. giving up..\n");
	exit(1);
    }

    if ((test_load_data(&user_enc_pubkey)) != 0) {
	printf("Bad input user_enc_pubkey.. giving up..\n");
	exit(1);
    }

    if ((test_load_data(&user_enc_privkey)) != 0) {
	printf("Bad input user_enc_privkey.. giving up..\n");
	exit(1);
    }

    if ((test_load_data(&kdc_sig_pubkey)) != 0) {
	printf("Bad input kdc_sig_pubkey.. giving up..\n");
	exit(1);
    }
    if ((test_load_data(&kdc_sig_privkey)) != 0) {
	printf("Bad input kdc_sig_privkey.. giving up..\n");
	exit(1);
    }

    sprintf(krbtgtstring, "krbtgt/%s", "dc.cell.ch.hp.com");
    status = krb5_parse_name(krbtgtstring, &kdcname);
    if (status != 0) {
	printf("krb5_parse_name failed; %lx\n", status);
	exit(1);
    }

    status = krb5_parse_name("root", &username);
    if (status != 0) {
	printf("krb5_parse_name failed; %lx\n", status);
	exit(1);
    }

    sec_pubkey_store (kdcname, &dummy, sec_pk_usage_digitalSignature,
		      0, &kdc_sig_pubkey);
    sec_pubkey_store (username, &dummy, sec_pk_usage_digitalSignature,
		      0, &user_sig_pubkey);
    sec_pubkey_store (username, &dummy, sec_pk_usage_keyEncipherment,
		      0, &user_enc_pubkey);    
    
    status = sec_psm_open("root", (unsigned char *)"userpwd",
			  &sec_pk_domain_dce_pk_login, &psmh_user);
    if (status != 0) {
	printf("psm_open (user) failed: %lx\n", status);
	exit(1);
    }
    status = sec_psm_put_pub_key(psmh_user, (unsigned char *)"userpwd",
				 sec_pk_usage_digitalSignature,
				 NULL /*user_sig_pubkey*/, &user_sig_privkey);
    if (status != 0) {
	printf("psm_put_pub_key (user sig key) failed: %lx\n", status);
	exit(1);
    }
    
    status = sec_psm_put_pub_key (psmh_user, (unsigned char *)"userpwd",
				  sec_pk_usage_keyEncipherment,
				  NULL /*user_enc_pubkey*/, &user_enc_privkey);
    if (status != 0) {
	printf("psm_put_pub_key (user enc key) failed: %lx\n", status);
	exit(1);
    }

    
    status = sec_psm_open(krbtgtstring, (unsigned char *)"kdcpwd",
			  &sec_pk_domain_dce_pk_login, &psmh_kdc);
    if (status != 0) {
	printf("psm_open (KDC) failed: %lx\n", status);
	exit(1);
    }

    status = sec_psm_put_pub_key(psmh_kdc, (unsigned char *)"kdcpwd",
				 sec_pk_usage_digitalSignature,
				 NULL /*user_sig_pubkey*/, &kdc_sig_privkey);
    if (status != 0) {
	printf("psm_put_pub_key (user sig key) failed: %lx\n", status);
	exit(1);
    }

#define BOMBCHECK() if (i) {printf("bomb-hit %d %x\n", i, status); continue; }
    
    for (i=0; i<1000; i++) {
	if (i>0) {
	    printf("bomb_after %d\n", i);
	    cma_lib_malloc_bomb(i);
	}
	
	status = krb5_pkinit_sign_as_req (psmh_user, kdcname,
					  NULL,
					  123456,
					  800000000,
					  0xdeadbeef,
					  &pa_out);

	if (status != 0) {
	    printf("pkinit_sign_as_req failed: %lx\n", status);
	    BOMBCHECK();
	    exit(1);
	}
    
	pa_fu_out.data = pa_out.contents;
	pa_fu_out.len = pa_out.length;
    
	test_dump_data(&pa_fu_out);


	status = krb5_pkinit_decode_as_req (psmh_kdc,
					    &pa_out,
					    kdcname,
					    username,
					    NULL, /* !!! */
					    &cusec,
					    &ctime,
					    &nonce);
	pa_out.contents = NULL;

	if (status != 0) {
	    BOMBCHECK();
	    printf("pkinit_decode_as_req failed: %lx\n", status);
	    exit(1);
	}

	if ((cusec != 123456) || (ctime != 800000000) || (nonce != 0xdeadbeef)) {
	    BOMBCHECK();
	    printf("guts mismatch: %d %d %x\n",
		   cusec, ctime, nonce);
	    printf("expected: %d %d %x\n",
		   123456, 800000000, 0xdeadbeef);

	    exit(1);
	}

	printf("pkinit_decode_as_req succeeded!\n");
    
	sec_pk_data_free(&pa_fu_out);

	status = krb5_pkinit_sign_as_rep (psmh_kdc,
					  kdcname,
					  username,				      
					  ETYPE_DES_CBC_CRC, /* !!! */
					  &reply_key,
					  0xdeadbeef,
					  &pa_out);

	if (status != 0) {
	    BOMBCHECK();
	    printf("pkinit_sign_as_rep failed: %lx\n", status);
	    exit(1);
	}
    
	pa_fu_out.data = pa_out.contents;
	pa_fu_out.len = pa_out.length;

	test_dump_data(&pa_fu_out);

	status = krb5_pkinit_decode_as_rep (psmh_user,
					    &pa_out,
					    kdcname,
					    username,
					    &other_key,
					    &other_nonce);

	if (status != 0) {
	    BOMBCHECK();	    
	    printf("pkinit_decode_as_rep failed: %lx\n", status);
	    exit(1);
	}
    
	printf("pkinit_decode_as_rep succeeded!\n");

	if ((other_nonce != 0xdeadbeef) ||
	    (other_key.length != reply_key->length) ||
	    (other_key.keytype != reply_key->keytype) ||
	    (memcmp(other_key.contents, reply_key->contents, reply_key->length))) {
	    BOMBCHECK();	    
	    printf("guts mismatch: %x %d %d\n",
		   other_nonce, other_key.length, other_key.keytype);
	    printf("expected: %x %d %d\n",
		   0xdeadbeef, reply_key->length, reply_key->keytype);	
	    exit(1);
	}
	sec_pk_data_free(&pa_fu_out);
	if (i>0) {
	    printf("phew.  made it to the end after %d bombs\n", i);
	    break;
	}
    }
    
    sec_pk_data_free(&user_sig_pubkey);
    sec_pk_data_free(&user_sig_privkey);
    sec_pk_data_free(&user_enc_pubkey);
    sec_pk_data_free(&user_enc_privkey);
    sec_pk_data_free(&kdc_sig_pubkey);
    sec_pk_data_free(&kdc_sig_privkey);

    krb5_free_keyblock(reply_key);
    sec_psm_close(psmh_kdc);
    sec_psm_close(psmh_user);
    
    return 0;
}
