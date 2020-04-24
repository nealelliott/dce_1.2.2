/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: sec_psm_P.c,v $
 * Revision 1.1.2.3  1996/11/14  16:14:46  arvind
 * 	OSF DCE 1.2.2 Drop 5
 * 	[1996/11/14  14:29:48  arvind]
 *
 * Revision /main/DCE_1.2.2/9  1996/10/24  17:35 UTC  cuti
 * 	Merge
 * 
 * Revision /main/DCE_1.2.2/cuti_pk_f/1  1996/10/24  17:14 UTC  cuti
 * 	more initialize in kvno
 * 
 * Revision /main/DCE_1.2.2/8  1996/09/30  15:44 UTC  arvind
 * 	Add OSF copyright
 * 
 * $EndLog$
 */
/* sec_psm_P.c 
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <macros.h>
#include <tet_api.h>
#include <ts-common.h>

#include <dce/binding.h>
#include <dce/dce_error.h>

#include <dce/rgybase.h>
#include <dce/uuid.h>
#include <dce/sec_login.h>

#include <sec_def_flags.h>

#include <dce/sec_pk_base.h>
#include <dce/sec_pvtkey.h>
#include <dce/sec_psm.h>
#include <dce/sec_psm_base.h>
#include <sec_bsafe.h>
#include <dce/sec_pk.h>

#include "../common.h"

#undef PK_USER
#define PK_USER "test_psm_pk_user"

#define PVTKEY_FILE_PREFIX "/opt/dcelocal/var/security/pk_file/"


static void tp1(void), tp2(void), tp3(void);
static void pk_startup(void), pk_cleanup(void);

unsigned32 pk_size = 1024;

sec_login_handle_t login_h;
sec_rgy_name_t     user;
sec_passwd_str_t   passwd;
sec_passwd_str_t   new_passwd;
int                dce_status;
sec_login_handle_t ilh = NULL;
sec_rgy_handle_t   write_context;
char rm_pk_file_com[512];

unsigned char unexpected[dce_c_error_string_len];
unsigned char expected[dce_c_error_string_len];


sec_pk_data_t tobesigned_data = {
    50, "The is a test about psm and pvtkey and bsafe stuff" };


/* List of tests to run that TET will be looking for */
struct tet_testlist tet_testlist[] = {
    {tp1, 1},
    {tp2, 2},
    {tp3, 3},
    {NULL, 0}
};

void (*tet_startup)() = pk_startup;
void (*tet_cleanup)() = pk_cleanup;


/* Initialization of data needed for each of the test cases */
static void pk_startup(void)
{
    error_status_t          st;
    char *cp;
    sec_rgy_bind_auth_info_t auth_info;
    sec_rgy_handle_t     rgy_handle;
    uuid_t               princ_id;
    unsigned_char_t      *princ_idstr;
    sec_rgy_pname_t      pvtkey_file;

    
    mtet_infoline("sec_psm test startup.\n");
    CLEAR_STATUS(&st);
    strcpy((char *)user, PK_USER);
    strcpy((char *)passwd, PK_PASSWD);
    strcpy((char *)new_passwd, NEW_PK_PASSWD);

    cp = getenv ("PK_SIZE");
    if (cp) {
	pk_size = atoi(cp);
	if (pk_size < 256)
	    pk_size = 256;
    }

    /* just in case cleanup didn't work before... */
    system("./cleanup.ksh >/dev/null 2>&1");

    /* add a generic pk_user */

    if (system("./setup.ksh") != 0) {
        int test_no;
        mtet_infoline("FAILED: system(./setup.ksh)\n" );
        for (test_no=0; tet_testlist[test_no++].testfunc != NULL;)
            tet_delete(test_no, "startup() has failed");
        return;
    }

    /* Need to form pvtkey file name by appending to PVTKEY_PREFIX
       the user id */

    auth_info.info_type = sec_rgy_bind_auth_none;
    auth_info.tagged_union.dce_info.authn_level= rpc_c_protect_level_pkt_integ;
    auth_info.tagged_union.dce_info.authn_svc = rpc_c_authn_dce_secret;
    auth_info.tagged_union.dce_info.authz_svc = rpc_c_authz_dce;
    auth_info.tagged_union.dce_info.identity  = NULL;
 
    sec_rgy_site_bind_query(NULL, &auth_info, &rgy_handle, &st);

    sec_rgy_pgo_name_to_id(rgy_handle, sec_rgy_domain_person, user, &princ_id, &st);

    uuid_to_string(&princ_id, &princ_idstr, &st);

    sec_rgy_site_close(rgy_handle, &st);

    strcpy(pvtkey_file, PVTKEY_FILE_PREFIX);
    strcat(pvtkey_file, princ_idstr);

    strcpy(rm_pk_file_com, "rm ");
    strcat(rm_pk_file_com, pvtkey_file);
    
    rpc_string_free(&princ_idstr, &st);

    return;
}


/* Cleanup of data needed for each of the test cases */
static void pk_cleanup(void)
{

    if (system("./cleanup.ksh") != 0) {
        mtet_infoline("FAILED: system(./cleanup.ksh)\n");
    }
    system( rm_pk_file_com );
    mtet_infoline("sec_psm test cleanup.\n");

    return;
}


/* TP1: 
   sec_psm_open(),
   sec_psm_gen_pub_key(),
   sec_psm_put_pub_key(),
   sec_psm_update_pub_key(),
   sec_psm_close()
*/
static void tp1()
{
    error_status_t   st;
    sec_psm_handle_t psm_h;
    sec_pk_data_t    pvtkey;
    sec_pk_data_t    pubkey;
    int              key_usage = 0;
    sec_pk_usage_flags_t   usages;
    sec_pk_algorithm_id_t  * alg_id;


    for (key_usage = 1; key_usage <= 3; key_usage=key_usage+2) {
        switch(key_usage) {
        case 1: usages = sec_pk_usage_digitalSignature;
            break;
        case 2: usages = sec_pk_usage_nonRepudiation;
            break;
        case 3: usages = sec_pk_usage_keyEncipherment;
            break;
        case 4: usages = sec_pk_usage_dataEncipherment;
            break;
        case 5: usages = sec_pk_usage_keyAgreement;
            break;
        case 6: usages = sec_pk_usage_keyCertSign;
            break;
        case 7: usages = sec_pk_usage_offLineCRLSign;
            break;
        case 8: usages = 255; /* all 1 */
        }

        mtet_infoline("(tp1) P1. sec_psm_open(): check successful call with key usage %d.\n", usages);
        /* start fresh */
        system( rm_pk_file_com );

        st = sec_psm_open( user,
                           passwd,
                           & sec_pk_domain_dce_general,
                           & psm_h);

        CHECK_RESULT(true, "P1. SEC_PSM: sec_psm_open", &st, error_status_ok);


        mtet_infoline("(tp1) P2. sec_psm_gen_pub_key(): check successful call,with key usage %d.\n", usages);
        mtet_infoline("          (generating public key; be patient...)\n");
	

        alg_id = RSA_PKCS;
        /* generate keys */
        st = sec_psm_gen_pub_key( alg_id,
                                  pk_size, /* unsigned32  key_length */
                                  NULL,  /* *seed sec_pk_gen_data_t */
                                  &pubkey,
                                  &pvtkey );

        CHECK_RESULT(true, "P2. SEC_PSM: sec_psm_gen_pub_key", &st, error_status_ok);

        mtet_infoline("(tp1) P3. sec_psm_put_pub_key(): check successful call.\n");
        /* then store keys */
        st = sec_psm_put_pub_key( psm_h, 
			          passwd, 
				  usages, 
			          &pubkey, 
		   	          &pvtkey );

        CHECK_RESULT(true, "P3. SEC_PSM: sec_psm_put_pub_key", &st, error_status_ok);

        mtet_infoline("(tp1) P4. sec_psm_update_pub_key(): check successful call, changing key pairs.\n");
        mtet_infoline("          (generating public key; be patient...)\n");
       /* generate new keys */

        sec_pk_data_free(&pubkey);
        sec_pk_data_zero_and_free(&pvtkey);

        st = sec_psm_gen_pub_key( alg_id,
                                  pk_size, 
                                  NULL,
                                  &pubkey,
                                  &pvtkey );
        CHECK_RESULT(true, "P4. SEC_PSM: sec_psm_gen_pub_key", &st, error_status_ok);

        st = sec_psm_update_pub_key( psm_h, 
			             passwd, 
				     NULL,
				     usages, 
				     &pubkey,
				     &pvtkey);

        CHECK_RESULT(true, "P4. SEC_PSM: sec_psm_update_pub_key key-pairs", &st, error_status_ok);


        mtet_infoline("(tp1) P5. sec_psm_update_pub_key(): check successful call, changing passphrase.\n");
        st = sec_psm_update_pub_key( psm_h, 
			             passwd, 
				     new_passwd,
				     usages, 
				     &pubkey,
				     &pvtkey );

        sec_pk_data_free(&pubkey);
        sec_pk_data_zero_and_free(&pvtkey);

        CHECK_RESULT(true, "P4. SEC_PSM: sec_psm_update_pub_key passphrase", &st, error_status_ok);


        mtet_infoline("(tp1) P5. sec_psm_close(): check successful call.\n");

        sec_psm_close( psm_h );

        CHECK_RESULT(true, "P5. SEC_PSM: sec_psm_close", &st, error_status_ok);

    }


    tet_result(TET_PASS);
}
/*-----------------------------------------------------------------------*/


/* TP2: 
   sec_psm_open(),
   sec_psm_gen_pub_key(),
   sec_psm_put_pub_key(),
   sec_psm_sign_data()
   sec_psm_verify_data()
   sec_psm_close()
*/
static void tp2()
{
    error_status_t   st;
    sec_psm_handle_t psm_h;
    sec_pk_data_t    pvtkey;
    sec_pk_data_t    pubkey;
    int              key_usage = 0;
    sec_pk_usage_flags_t   usages;
    sec_pk_algorithm_id_t * alg_id;
    sec_pk_signed_t  signature;
    unsigned32       kvno = 0;


    /* just test sec_pk_usage_digitalSignature and sec_pk_usage_keyEncipherment
       for now */
    for (key_usage = 1; key_usage <= 3; key_usage=key_usage+2) {
        switch(key_usage) {
        case 1: usages = sec_pk_usage_digitalSignature;
            break;
        case 2: usages = sec_pk_usage_nonRepudiation;
            break;
        case 3: usages = sec_pk_usage_keyEncipherment;
            break;
        case 4: usages = sec_pk_usage_dataEncipherment;
            break;
        case 5: usages = sec_pk_usage_keyAgreement;
            break;
        case 6: usages = sec_pk_usage_keyCertSign;
            break;
        case 7: usages = sec_pk_usage_offLineCRLSign;
            break;
        case 8: usages = 255; /* all 1 */
        }

        mtet_infoline("(tp2) P1. sec_psm_open(): check successful call, with key usage %d.\n", usages);

        system( rm_pk_file_com );

        st = sec_psm_open( user,
                       passwd,
                       & sec_pk_domain_dce_general,
                       & psm_h);

        CHECK_RESULT(true, "P1. SEC_PSM: sec_psm_open", &st, error_status_ok);


        mtet_infoline("(tp2) P2. sec_psm_gen_pub_key(): check successful call.\n");
        mtet_infoline("          (generating public key; be patient...)\n");

        /* generate keys */
        alg_id = RSA_PKCS;
        st = sec_psm_gen_pub_key( alg_id,
                                  pk_size, /* unsigned32  key_length */
                                  NULL,  /* *seed sec_pk_gen_data_t */
                                  &pubkey,
                                  &pvtkey );

        CHECK_RESULT(true, "P2. SEC_PSM: sec_psm_gen_pub_key", &st, error_status_ok);

        mtet_infoline("(tp2) P3. sec_psm_put_pub_key(): check successful call.\n");
        /* then store keys */
        st = sec_psm_put_pub_key( psm_h, 
			          passwd, 
				  usages, 
			          &pubkey, 
		   	          &pvtkey );

        sec_pk_data_free(&pubkey);
        sec_pk_data_zero_and_free(&pvtkey);

        CHECK_RESULT(true, "P3. SEC_PSM: sec_psm_put_pub_key", &st, error_status_ok);


        mtet_infoline("(tp2) P4. sec_psm_update_sign_data(): check successful call, \n");
        alg_id = MD5RSA;

	if (pk_size < 1024) {
	    tobesigned_data.len = (pk_size/8)-1;
	}
	
	kvno = 0;
        st = sec_psm_sign_data( psm_h, 
			        alg_id, 
				usages, 
				& tobesigned_data,
                                & kvno,
				& signature);

        CHECK_RESULT(true, "P4. SEC_PSM: sec_psm_sign_data", &st, error_status_ok);

        mtet_infoline("(tp2) P5. sec_psm_update_verify_data(): check successful call, \n");
        st = sec_psm_verify_data( psm_h,
                                  &sec_pk_domain_dce_general,
                                  user, 
                                  kvno,
			          alg_id, 
				  usages, 
				  & tobesigned_data,
				  &signature);

        CHECK_RESULT(true, "P5. SEC_PSM: sec_psm_verify_data", &st, error_status_ok);

        sec_pk_data_free( & signature );

        if ( signature.data != NULL ) {
           mtet_infoline("P6. FAILED sec_pubkey_data_free(): signature should be NULL, but is NOT!\n");
	   tet_result(TET_FAIL);
	   return;
        }
        mtet_infoline("(tp2) P5. sec_psm_close(): check successful call.\n");

        sec_psm_close( psm_h );

        CHECK_RESULT(true, "P6. SEC_PSM: sec_psm_update_pub_key passphrase", &st, error_status_ok);


    }


    tet_result(TET_PASS);
}
/*-----------------------------------------------------------------------*/


/* TP3:
   sec_psm_open(),
   sec_psm_gen_pub_key(),
   sec_psm_put_pub_key(),
   sec_psm_encrypt()
   sec_psm_decrypt()
   sec_psm_close()
*/
static void tp3()
{
    error_status_t   st;
    sec_psm_handle_t psm_h;
    sec_pk_data_t    pvtkey;
    sec_pk_data_t    pubkey;
    int              key_usage = 0;
    sec_pk_usage_flags_t   usages;
    sec_pk_algorithm_id_t * alg_id;
    sec_pk_encrypted_t  encrypted_data;
    sec_pk_data_t tobeencrypted_data = {
    60, "The is a test about psm and pvtkey and bsafe stuff0123456789" };
    unsigned32       kvno;

    sec_pk_data_t       wasdecrypted_data;


    /* just test sec_pk_usage_keyEncipherment for now */
    for (key_usage = 3; key_usage == 3; key_usage++) {
        switch(key_usage) {
        case 1: usages = sec_pk_usage_digitalSignature;
            break;
        case 2: usages = sec_pk_usage_nonRepudiation;
            break;
        case 3: usages = sec_pk_usage_keyEncipherment;
            break;
        case 4: usages = sec_pk_usage_dataEncipherment;
            break;
        case 5: usages = sec_pk_usage_keyAgreement;
            break;
        case 6: usages = sec_pk_usage_keyCertSign;
            break;
        case 7: usages = sec_pk_usage_offLineCRLSign;
            break;
        case 8: usages = 255; /* all 1 */
        }

        mtet_infoline("(tp3) P1. sec_psm_open(): check successful call, using keyEncipherment" );

        system( rm_pk_file_com );

        st = sec_psm_open( user,
                           passwd,
                           & sec_pk_domain_dce_general,
                           & psm_h );
        CHECK_RESULT(true, "P1. SEC_PSM: sec_psm_open", &st, error_status_ok);


        mtet_infoline("(tp3) P2. sec_psm_gen_pub_key(): check successful call.\n");
        mtet_infoline("          (generating public key; be patient...)\n");

        alg_id = RSA_PKCS;
        /* generate keys */
        st = sec_psm_gen_pub_key( alg_id,
                                  pk_size, /* unsigned32  key_length */
                                  NULL,  /* *seed sec_pk_gen_data_t */
                                  &pubkey,
                                  &pvtkey );

        CHECK_RESULT(true, "P2. SEC_PSM: sec_psm_gen_pub_key", &st, error_status_ok);

        mtet_infoline("(tp3) P3. sec_psm_put_pub_key(): check successful call.\n");
        /* then store keys */
        st = sec_psm_put_pub_key( psm_h,
			          passwd, 
				  usages,
			          &pubkey,
		   	          &pvtkey );

        sec_pk_data_free(&pubkey);
        sec_pk_data_zero_and_free(&pvtkey);

        CHECK_RESULT(true, "P3. SEC_PSM: sec_psm_put_pub_key", &st, error_status_ok);

        mtet_infoline("(tp3) P4. sec_psm_encrypt_data(): check successful call.\n");

        alg_id = RSAENC;
	if (pk_size < 1024) {
	    tobeencrypted_data.len = pk_size/16;
	}
	
        st = sec_psm_encrypt_data( psm_h,
                                   &sec_pk_domain_dce_general,
                                   user, 
                                   &kvno,
			           alg_id, 
				   usages, 
				   & tobeencrypted_data,
				   & encrypted_data );

        CHECK_RESULT(true, "P4. SEC_PSM: sec_psm_encrypt_data", &st, error_status_ok);

        mtet_infoline("(tp3) P5. sec_psm_decrypt_data(): check successful call, \n");
        /* kvno=0; */
        st = sec_psm_decrypt_data( psm_h, 
                                   kvno,
			           alg_id, 
				   usages, 
				   & encrypted_data,
				   & wasdecrypted_data );

        CHECK_RESULT(true, "P5. SEC_PSM: sec_psm_decrypt_data", &st, error_status_ok);

        /* compare tobeencrypted_data with wasdecrypted_data */
        if ( tobeencrypted_data.len != wasdecrypted_data.len ) {
           mtet_infoline("P5. SEC_PSM: FAILED sec_psm_decrypt_data(): wasdecrypted_data->len does NOT match what was stored.\n");
	   tet_result(TET_FAIL);
        }
        else if ( memcmp(tobeencrypted_data.data, wasdecrypted_data.data,
                        tobeencrypted_data.len) != 0 ) {
           mtet_infoline("P5. SEC_PSM: FAILED sec_psm_decrypt_data(): was decrypted_data->data does NOT match what was stored\n");
	   tet_result(TET_FAIL);
        } else 
           mtet_infoline("(tp3) P4. sec_psm_decrypt_data(): data compare passed.\n");

        sec_pk_data_free( & encrypted_data );
        sec_pk_data_free( & wasdecrypted_data );
        if ( wasdecrypted_data.data != NULL ) {
           mtet_infoline("P6. FAILED sec_pubkey_data_free(): wasdecrypted_data should be NULL, but is NOT!\n");
	   tet_result(TET_FAIL);
	   return;
        }


        mtet_infoline("(tp3) P5. sec_psm_close(): check successful call.\n");

        sec_psm_close( psm_h );

        CHECK_RESULT(true, "P6. SEC_PSM: sec_psm_update_pub_key passphrase", &st, error_status_ok);

    }

    tet_result(TET_PASS);
}
/*-----------------------------------------------------------------------*/



