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
 * $Log: sec_pvtkey_P.c,v $
 * Revision 1.1.2.3  1996/11/14  16:15:01  arvind
 * 	OSF DCE 1.2.2 Drop 5
 * 	[1996/11/14  14:29:58  arvind]
 *
 * Revision /main/DCE_1.2.2/7  1996/10/23  18:21 UTC  cuti
 * 	Merge of cuti_pk_fix
 * 
 * 	Add OSF copyright
 * 	[1996/09/30  15:45 UTC  arvind  /main/DCE_1.2.2/6]
 * 	Revision /main/DCE_1.2.2/cuti_pk_fix/1  1996/10/16  19:16 UTC  cuti
 * 	Initialize kvno to 0
 * 
 * $EndLog$
 */
/* sec_pvtkey_P.c 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

#include <dce/sec_pk.h>
#include <dce/sec_pk_base.h>
#include <dce/sec_pvtkey.h>
#include "sec_bsafe.h"

#include "../common.h"

#undef PK_USER
#define PK_USER "test_pvtkey_pk_user"

#define PVTKEY_FILE_PREFIX "/opt/dcelocal/var/security/pk_file/"

static void tp1(void), tp2(void);
static void pk_startup(void), pk_cleanup(void);

unsigned32 pk_size = 1024;

unsigned char unexpected[dce_c_error_string_len];
unsigned char expected[dce_c_error_string_len];
sec_login_handle_t login_h;
sec_rgy_name_t     user;
sec_rgy_name_t     passwd;
sec_rgy_name_t     new_pwd;
int                dce_status;
sec_login_handle_t ilh = NULL;
sec_rgy_handle_t   write_context;
boolean32          opened = false;
char rm_pk_file_com[512];


#undef CHECK_RESULT
#define CHECK_RESULT(T, FUNC, RETURNED_ST_P, EXPECTED_ST) \
    if (STATUS_EQUAL(RETURNED_ST_P, EXPECTED_ST)) { \
	if ((T)) mtet_infoline("PASSED %s()\n", FUNC); \
    } else { \
	error_status_t xst; \
	dce_error_inq_text(*(RETURNED_ST_P), unexpected, &dce_status); \
	dce_error_inq_text(EXPECTED_ST, expected, &dce_status); \
	mtet_infoline("FAILED %s()\nresult: \t%s\nexpected: \t%s\n\n", \
		FUNC, unexpected, expected); \
	tet_result(TET_FAIL); \
	if (ilh != NULL) sec_login_purge_context(ilh, &xst); \
        if ( opened ) \
           sec_pvtkey_close( handle ); \
        return; \
    }


/* List of tests to run that TET will be looking for */
struct tet_testlist tet_testlist[] = {
    {tp1, 1},
/*    {tp2, 2}, LOGIN CONTEXT CHANGE NOT RUN */
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
    
    tet_infoline("sec_pvtkey test startup");
    CLEAR_STATUS(&st);

    memset(user, 0, sizeof(sec_rgy_name_t));
    memset(passwd, 0, sizeof(sec_rgy_name_t));
    memset(new_pwd, 0, sizeof(sec_rgy_name_t)); 
 
    strcpy((char *)&user[0], PK_USER);
    strcpy((char *)&passwd[0], PK_PASSWD);
    strcpy((char *)&new_pwd[0], NEW_PK_PASSWD);

    cp = getenv ("PK_SIZE");
    if (cp) {
	pk_size = atoi(cp);
	if (pk_size < 256)
	    pk_size = 256;
    }
    /* just in case cleanup didn't work before... */

    system("./cleanup.ksh > /dev/null 2>&1");

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

    return;
}


/* TP1: check as cell_admin
   sec_pvtkey_open(),
   sec_pvtkey_store(),
   sec_pvtkey_update(),
   sec_pvtkey_get(),
   sec_pk_data_zero_and_free(),
   sec_pvtkey_close()
*/
static void tp1()
{
    error_status_t          st;
    sec_pk_mechanism_handle_t  handle;
    sec_pk_data_t    pvtkey;
    sec_pk_data_t    pubkey;
    sec_pk_data_t    out_pvtkey;
    sec_pk_data_t    out_pubkey;
    byte             *was_pvtkey_data;
    unsigned32       was_pvtkey_len, i;           
    unsigned32       kno;
    int              key_usage = 0;
    sec_pk_usage_flags_t   usages;

    system( rm_pk_file_com );

    for (key_usage = 1; key_usage <= 1; key_usage++) {

        usages = 0;
        switch(key_usage) {
        case 1: usages |= sec_pk_usage_digitalSignature;
            break;
        case 2: usages |= sec_pk_usage_nonRepudiation;
            break;
        case 3: usages |= sec_pk_usage_keyEncipherment;
            break;
        case 4: usages |= sec_pk_usage_dataEncipherment;
            break;
        case 5: usages |= sec_pk_usage_keyAgreement;
            break;
        case 6: usages |= sec_pk_usage_keyCertSign;
            break;
        case 7: usages |= sec_pk_usage_offLineCRLSign;
            break;
/*        case 8: usages |= 255; */ /* all 1 */
        }

        mtet_infoline("(tp1) P1. sec_pvtkey_open(): check successful call, with key usage %d.\n", key_usage);

        st = sec_pvtkey_open( user,
                              &sec_pk_domain_dce_general,
                              &handle);
        CHECK_RESULT(true, "P1. SEC_PVTKEY: sec_pvtkey_open", &st, error_status_ok);
   	opened = true;

        mtet_infoline("(tp1) P2. sec_bsafe_gen_pub_key(): check successful call with key usage %d, for _store.\n", key_usage);
	mtet_infoline("          (generating public key; be patient...)\n");
        st = sec_bsafe_gen_pub_key(pk_size, NULL, &pubkey, &pvtkey);
        CHECK_RESULT(true, "P2. SEC_PVTKEY: sec_bsafe_gen_pub_key", &st, error_status_ok);

        mtet_infoline("(tp1) P3. sec_pvtkey_store(): check successful call, with key usage %d.\n", key_usage);
        st = sec_pvtkey_store( handle,
                               passwd,
                               usages,  /* key usage */
                               1,     /* kvno */
                               &pvtkey,
                               &pubkey );
        CHECK_RESULT(true, "P3. SEC_PVTKEY: sec_pvtkey_store", &st, error_status_ok);

        mtet_infoline("(tp1) P4. sec_pvtkey_get(): check successful call, with key usage %d.\n", key_usage);
        kno = 0;
        st = sec_pvtkey_get( handle,
                             passwd,
                             usages,
                             & kno,
                             & out_pvtkey,
                             & out_pubkey );
        CHECK_RESULT(true, "P4. SEC_PVTKEY: sec_pvtkey_get status check", &st, error_status_ok);
        /* verify that the value in *pvtkey is the same value
           _stored previously by doing a byte-by-byte comparison. */

        if (out_pvtkey.len != pvtkey.len) {
           mtet_infoline("P4. FAILED sec_pvtkey_get(): out_pvtkey_p len does not match what was stored\n");
	   tet_result(TET_FAIL);
	   return;
        }
        else if ( memcmp( out_pvtkey.data, pvtkey.data, pvtkey.len ) != 0 ) {
           mtet_infoline("P4. FAILED sec_pvtkey_get(): out_pvtkey_p data does not match what was stored\n");
	   tet_result(TET_FAIL);
	   return;
        }

        sec_pk_data_free( &pubkey );
        sec_pk_data_zero_and_free( &pvtkey );
        sec_pk_data_zero_and_free( &out_pvtkey );


        mtet_infoline("(tp1) P5. sec_bsafe_gen_pub_key(): check successful call with key usage %d, for _update.\n", key_usage);
	mtet_infoline("          (generating public key; be patient...)\n"); 
        st = sec_bsafe_gen_pub_key(pk_size, NULL, &pubkey, &pvtkey);
        CHECK_RESULT(true, "P5. SEC_PVTKEY: sec_bsafe_gen_pub_key for _update", &st, error_status_ok);

        mtet_infoline("(tp1) P5. sec_pvtkey_update(): check successful call, using new pwd and keypairs, with key usage %d.\n", key_usage);
        st = sec_pvtkey_update( handle,
                                passwd,
                                new_pwd,
                                usages,
                                &kno,
                                &pvtkey,
                                &pubkey );
        CHECK_RESULT(true, "P5. SEC_PVTKEY: sec_pvtkey_update", &st, error_status_ok);


        mtet_infoline("(tp1) P6. sec_pvtkey_get(): check successful call, using new pwd, with key usage %d.\n", key_usage);
        st = sec_pvtkey_get( handle,
                             new_pwd,
                             usages,
                             & kno,
                             & out_pvtkey,
                             & out_pubkey );
        CHECK_RESULT(true, "P6. SEC_PVTKEY: sec_pvtkey_get", &st, error_status_ok);
        /* verify that the value in *pvtkey is the same value
           _stored previously by doing a byte-by-byte comparison. */

        if (out_pvtkey.len != pvtkey.len) {
           mtet_infoline("P6. FAILED sec_pvtkey_get(): out_pvtkey_p len does not match what was stored\n");
           sec_pk_data_free( & out_pubkey );
           sec_pk_data_zero_and_free( & out_pvtkey );
	   tet_result(TET_FAIL);
	   return;
        }
        else if ( memcmp( out_pvtkey.data, pvtkey.data, pvtkey.len ) != 0 ) {
           mtet_infoline("P6. FAILED sec_pvtkey_get(): out_pvtkey_p data does not match what was stored\n");
           sec_pk_data_free( & out_pubkey );
           sec_pk_data_zero_and_free( & out_pvtkey );
	   tet_result(TET_FAIL);
	   return;
        }

        sec_pk_data_free( &pubkey );
        sec_pk_data_zero_and_free( &pvtkey );


        mtet_infoline("(tp1) P7. sec_pk_data_zero_and_free(): check successful call.\n");
        sec_pk_data_free( & out_pubkey );

        was_pvtkey_len = out_pvtkey.len;
        was_pvtkey_data = out_pvtkey.data;
        sec_pk_data_zero_and_free( & out_pvtkey );
        CHECK_RESULT(true, "P7. SEC_PVTKEY: sec_pk_data_zero_and_free", &st, error_status_ok);

        if ( (out_pvtkey.data != NULL) ) {
           mtet_infoline("P7. FAILED sec_pk_data_zero_and_free(): out_pvtkey.data should be NULL, but is NOT!\n");
   	   tet_result(TET_FAIL);
        }
        for (i=0; i < was_pvtkey_len; i++) {
            if ( was_pvtkey_data[i] != 0 ) {
               mtet_infoline("P7. FAILED sec_pk_data_zero_and_free(): out_pvtkey.data[N] should be NULL, but is NOT!\n");
	       tet_result(TET_FAIL);
            }
        }


        mtet_infoline("(tp1) P8. sec_pvtkey_close(): check successful call, with key usage %d.\n", key_usage);
        sec_pvtkey_close( handle );
        CHECK_RESULT(true, "P8. SEC_PVTKEY: sec_pvtkey_close", &st, error_status_ok);
    }

    tet_result(TET_PASS);
}
/*-----------------------------------------------------------------------*/


/* TP2: check as cell_admin
   sec_pvtkey_open(),
   sec_pvtkey_store(),
   sec_pvtkey_update(),
   sec_pvtkey_get(),
   sec_pvtkey_close()
*/
static void tp2()
{
    error_status_t          st;
    sec_pk_mechanism_handle_t  handle;
    sec_pk_data_t    pvtkey;
    sec_pk_data_t    pubkey;
    sec_pk_data_t    out_pvtkey;
    sec_pk_data_t    out_pubkey;
    unsigned32       kno;
    int              key_usage = 0;
    sec_pk_usage_flags_t   usages;



        mtet_infoline("(tp2) P1. sec_pvtkey_open(): check successful call, with key usage %d.\n", key_usage);

        usages = 255;

        st = sec_pvtkey_open( user,
                              &sec_pk_domain_dce_general,
                              &handle);
        CHECK_RESULT(true, "P1. SEC_PVTKEY: sec_pvtkey_open", &st, error_status_ok);
   	opened = true;

        mtet_infoline("(tp2) P2. sec_bsafe_gen_pub_key(): check successful call with key usage %d, for _store.\n", key_usage);
	mtet_infoline("          (generating public key; be patient...)\n"); 
        st = sec_bsafe_gen_pub_key(pk_size, NULL, &pubkey, &pvtkey);
        CHECK_RESULT(true, "P2. SEC_PVTKEY: sec_bsafe_gen_pub_key", &st, error_status_ok);

        mtet_infoline("(tp2) P3. sec_pvtkey_store(): check successful call, with key usage %d.\n", key_usage);
        st = sec_pvtkey_store( handle,
                               passwd,
                               usages,  /* key usage */
                               0,     /* kvno */
                               &pvtkey,
                               &pubkey );
        CHECK_RESULT(true, "P3. SEC_PVTKEY: sec_pvtkey_store", &st, error_status_ok);

	sec_pk_data_init(&pubkey);
	sec_pk_data_init(&pvtkey);
	sec_pk_data_init(&out_pvtkey);
	sec_pk_data_init(&out_pubkey);

        mtet_infoline("(tp2) P5. sec_bsafe_gen_pub_key(): check successful call with key usage %d, for _update.\n", key_usage);
	mtet_infoline("          (generating public key; be patient...)\n"); 
        st = sec_bsafe_gen_pub_key(pk_size, NULL, &pubkey, &pvtkey);
        CHECK_RESULT(true, "P5. SEC_PVTKEY: sec_bsafe_gen_pub_key for _update", &st, error_status_ok);


    for (key_usage = 1; key_usage <= 3; key_usage=key_usage+2) {
        usages = 0;
        switch(key_usage) {
        case 1: usages |= sec_pk_usage_digitalSignature;
            break;
        case 2: usages |= sec_pk_usage_nonRepudiation;
            break;
        case 3: usages |= sec_pk_usage_keyEncipherment;
            break;
        case 4: usages |= sec_pk_usage_dataEncipherment;
            break;
        case 5: usages |= sec_pk_usage_keyAgreement;
            break;
        case 6: usages |= sec_pk_usage_keyCertSign;
            break;
        case 7: usages |= sec_pk_usage_offLineCRLSign;
            break;
/*        case 8: usages |= 255; */ /* all 1 */
        }

        mtet_infoline("(tp2) P5. sec_pvtkey_update(): check successful call, using new pwd and keypairs, with key usage %d.\n", key_usage);
        st = sec_pvtkey_update( handle,
                                passwd,
                                new_pwd,
                                usages,
                                &kno,
                                &pvtkey,
                                &pubkey );
        CHECK_RESULT(true, "P5. SEC_PVTKEY: sec_pvtkey_update", &st, error_status_ok);

        mtet_infoline("(tp2) P6. sec_pvtkey_get(): check successful call, using new pwd, with key usage %d.\n", key_usage);
        st = sec_pvtkey_get( handle,
                             new_pwd,
                             usages,
                             & kno,
                             & out_pvtkey,
                             & out_pubkey );
        CHECK_RESULT(true, "P6. SEC_PVTKEY: sec_pvtkey_get", &st, error_status_ok);
        /* verify that the value in *pvtkey is the same value
           _stored previously by doing a byte-by-byte comparison. */

        if (out_pvtkey.len != pvtkey.len) {
           mtet_infoline("P6. FAILED sec_pvtkey_get(): out_pvtkey.len does not match what was stored\n");
	   tet_result(TET_FAIL);
	   return;
        }
        else if ( memcmp( out_pvtkey.data, pvtkey.data, pvtkey.len ) != 0 ) {
           mtet_infoline("P6. FAILED sec_pvtkey_get(): out_pvtkey.data does not match what was stored\n");
	   tet_result(TET_FAIL);
	   return;
        }

    /* P3. sec_pk_data_free(): check successful call, data_p should be NULL */
    mtet_infoline("(tp2) P8. sec_pvtkey_close(): check successful call, with key usage %d.\n", key_usage);
    sec_pk_data_free( & out_pubkey );

    CHECK_RESULT(true, "P3. SEC_PUBKEY: sec_pubkey_data_free", &st, error_status_ok);

    if ( out_pubkey.data != NULL ) {
        mtet_infoline("P3. FAILED sec_pubkey_data_free(): out_pubkey.data should be NULL, but is NOT!\n");
	tet_result(TET_FAIL);
	return; \
    }


        mtet_infoline("(tp2) P8. sec_pvtkey_close(): check successful call, with key usage %d.\n", key_usage);
	sec_pvtkey_close( handle );
        CHECK_RESULT(true, "P8. SEC_PVTKEY: sec_pvtkey_close", &st, error_status_ok);
    }

    tet_result(TET_PASS);
}
/*-----------------------------------------------------------------------*/






