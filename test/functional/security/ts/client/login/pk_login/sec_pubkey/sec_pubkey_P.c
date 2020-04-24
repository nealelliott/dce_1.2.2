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
 * $Log: sec_pubkey_P.c,v $
 * Revision 1.1.2.2  1996/10/03  18:45:50  arvind
 * 	Add OSF copyright
 * 	[1996/09/30  15:44 UTC  arvind  /main/DCE_1.2.2/8]
 *
 * Revision 1.1.2.1  1996/08/09  20:15:03  arvind
 * 	DCE 1.2.2 intermediate drop (post bl3/pre bl4)
 * 	[1996/08/09  20:00:55  arvind]
 * 
 * $EndLog$
 */
/* sec_pubkey_P.c 
*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <macros.h>
#include <tet_api.h>
#include <ts-common.h>

#include <dce/dce_cf.h>
#include <dce/binding.h>
#include <dce/dce_error.h>

#include <dce/rgybase.h>
#include <dce/uuid.h>
#include <dce/sec_login.h>

#include <sec_def_flags.h>

#include <dce/sec_pk.h>
#include <dce/sec_pubkey.h>

#include <../common.h>

#undef PK_USER
#define PK_USER "test_pubkey_pk_user"

static void tp1(void), tp2(void), tp3(void), tp4(void), tp5(void);
static void pk_startup(void), pk_cleanup(void);

                                  
unsigned char unexpected[dce_c_error_string_len];
unsigned char expected[dce_c_error_string_len];
sec_login_handle_t login_h;
sec_rgy_name_t     user;
sec_passwd_str_t   passwd_buf;
int                dce_status;
sec_login_handle_t ilh = NULL;
sec_rgy_handle_t   write_context;
char * cellname;


/* List of tests to run that TET will be looking for */
struct tet_testlist tet_testlist[] = {
    {tp1, 1},
    {tp2, 2},
    {tp3, 3},
    {tp4, 4},
    {NULL, 0}
};

void (*tet_startup)() = pk_startup;
void (*tet_cleanup)() = pk_cleanup;


/* Initialization of data needed for each of the test cases */
static void pk_startup(void)
{
    error_status_t          st;

    tet_infoline("sec_pubkey test startup");
    CLEAR_STATUS(&st);
    strcpy((char *)user, PK_USER);
    strcpy((char *)passwd_buf, PK_PASSWD);

    dce_cf_get_cell_name( & cellname, & st );

    /* add a generic pk_user */

    if (system("./setup.ksh") != 0) {
        int test_no;
        mtet_infoline("FAILED: system(./setup.ksh)\n" );
        for (test_no=0; tet_testlist[test_no++].testfunc != NULL;)
            tet_delete(test_no, "startup() has failed");
        return;
    }
    return;
}


/* Cleanup of data needed for each of the test cases */
static void pk_cleanup(void)
{
    if (system("./cleanup.ksh") != 0) {
        mtet_infoline("FAILED: system(./cleanup.ksh)\n");
    }

    return;
}


/* TP1: P1. sec_pubkey_store(): check successful call, with sec_pk_usage_digitalSignature; sec_pubkey_retrieve(): check successful call, with sec_pk_usage_digitalSignature; sec_pk_data_free(): check successful call, data_p should be NULL  */
static void tp1()
{
    error_status_t    st;
    sec_pk_pubkey_t   in_publickey;
    sec_pk_pubkey_t   out_publickey;
    byte              *pkdata_p = (byte *)"ABCDKEY";
    unsigned32        kvno;

    in_publickey.len = 8;
    in_publickey.data = pkdata_p;


    mtet_infoline("(tp1) P1. sec_pubkey_store(), sec_pubkey_retrieve(), sec_pk_data_free() : check successful calls, with sec_pk_usage_digitalSignature\n");

    /* P1. sec_pubkey_store(): check successful call, with sec_pk_usage_digitalSignature */
    kvno=1;
    st = sec_pubkey_store( user,
                           &sec_pk_domain_dce_general,  /* sec_pk_domain_t domain */
                           sec_pk_usage_digitalSignature, /* sec_pk_usage_flags_t key_usages */
                           &kvno,  /* unsigned32 kvno */
                           &in_publickey );
    CHECK_RESULT(true, "P1. SEC_PUBKEY: sec_pubkey_store", &st, error_status_ok);

    /* P1. sec_pubkey_retrieve(): check successful call, with sec_pk_usage_digitalSignature */
    kvno=1;
    st = sec_pubkey_retrieve( user,
                              &sec_pk_domain_dce_general,  /* sec_pk_domain_t domain */
                              sec_pk_usage_digitalSignature, /* sec_pk_usage_flags_t key_usages */
                              &kvno, /* unsigned32 kvno */
                              &out_publickey );

    CHECK_RESULT(true, "SEC_PUBKEY: sec_pubkey_retrieve", &st, error_status_ok);
    /* verify that the value in *public_key is the same value _stored
       previously by doing a byte-by-byte comparison.
    */
    if (out_publickey.len != in_publickey.len) {
       mtet_infoline("P1. FAILED sec_pubkey_retrieve(): out_publickey len does not match what was stored\n");
       tet_result(TET_FAIL);
       return;
    }
    else if ( memcmp( out_publickey.data, in_publickey.data, in_publickey.len ) != 0 ) {
       mtet_infoline("P1. FAILED sec_pubkey_retrieve(): out_publickey data does not match what was stored\n");
       tet_result(TET_FAIL);
       return;
     }
     mtet_infoline("P1. PASSED sec_pubkey_retrieve(): data compare.\n");

    /* P1. sec_pk_data_free(): check successful call, data_p should be NULL */
    sec_pk_data_free( &out_publickey );

    CHECK_RESULT(true, "P1. SEC_PUBKEY: sec_pubkey_data_free", &st, error_status_ok);

    if ( out_publickey.data != NULL ) {
        mtet_infoline("P1. FAILED sec_pubkey_data_free(): out_publickey should be NULL, but is NOT!\n");
	tet_result(TET_FAIL);
	return;
    }

    tet_result(TET_PASS);

/*
    if (system("dcecp -c pri modify pk_user -remove {DCEPKAuthentication}" ) != 0) {
        mtet_infoline("FAILED: system(dcecp -c pri modify pk_user -remove {DCEPKAuthentication}\n");
    }
    if (system("dcecp -c pri modify pk_user -remove {DCEPKAuthentication}" ) != 0) {
        mtet_infoline("FAILED: system(dcecp -c pri modify pk_user -remove {DCEPKKeyEncipherment}\n");
    }
*/
}
/*-----------------------------------------------------------------------*/

/* TP2: Positive: sec_pubkey_store(): check successful call, with sec_pk_usage_keyEncipherment; sec_pubkey_retrieve(): check successful call, with sec_pk_usage_keyEncipherment; sec_pk_data_free(): check successful call, data_p should be NULL, address contents should be zero  */
static void tp2()
{
    error_status_t    st;
    sec_pk_pubkey_t   in_publickey;
    sec_pk_pubkey_t   out_publickey;
    byte              *pkdata_p = (byte *)"ABCDKEY";
    unsigned32        kvno;

    in_publickey.len = 8;
    in_publickey.data = pkdata_p;

    mtet_infoline("(tp2) P2. sec_pubkey_store(), sec_pubkey_retrieve(), sec_pk_data_free() : check successful calls, with sec_pk_usage_keyEncipherment\n");

    /* sec_pubkey_store(): check successful call, with sec_pk_usage_keyEncipherment */
    kvno=1;
    st = sec_pubkey_store( user,
                           &sec_pk_domain_dce_general,  /* sec_pk_domain_t domain */
                           sec_pk_usage_keyEncipherment, /* sec_pk_usage_flags_t key_usages */
                           &kvno,  /* unsigned32 kvno */
                           &in_publickey );
    CHECK_RESULT(true, "P2. SEC_PUBKEY: sec_pubkey_store", &st, error_status_ok);

    /* sec_pubkey_retrieve(): check successful call, with sec_pk_usage_keyEncipherment */
    kvno=1;
    st = sec_pubkey_retrieve( user,
                              &sec_pk_domain_dce_general,  /* sec_pk_domain_t domain */
                              sec_pk_usage_keyEncipherment, /* sec_pk_usage_flags_t key_usages */
                              &kvno, /* unsigned32 kvno */
                              &out_publickey );

    CHECK_RESULT(true, "P2. SEC_PUBKEY: sec_pubkey_retrieve", &st, error_status_ok);

    /* verify that the value in *public_key is the same value _stored
       previously by doing a byte-by-byte comparison. */
    if (out_publickey.len != in_publickey.len) {
       mtet_infoline("P2. FAILED sec_pubkey_retrieve(): out_publickey len does not match what was stored\n");
       tet_result(TET_FAIL);
       return;
    }
    else if ( memcmp( out_publickey.data, in_publickey.data, in_publickey.len ) != 0 ) {
       mtet_infoline("P2. FAILED sec_pubkey_retrieve(): out_publickey data does not match what was stored\n");
       tet_result(TET_FAIL);
       return;
    }
    mtet_infoline("P2. PASSED sec_pubkey_retrieve(): data compare.\n");

    /* sec_pk_data_free(): check successful call, data_p should be NULL */
    sec_pk_data_free( &out_publickey );

    CHECK_RESULT(true, "P2. SEC_PUBKEY: sec_pubkey_data_free", &st, error_status_ok);

    if ( out_publickey.data != NULL ) {
        mtet_infoline("P2. FAILED sec_pubkey_data_free(): out_publickey.data should be NULL, but is NOT!\n");
	tet_result(TET_FAIL);
	return;
    }

    tet_result(TET_PASS);

}
/*-----------------------------------------------------------------------*/


/* TP3: Positive: sec_pubkey_store(): check successful call, with both 
sec_pk_usage_digitalSignature and sec_pk_usage_keyEncipherment; sec_pubkey_retrieve(): check successful call, with sec_pk_usage_keyEncipherment; sec_pk_data_free(): check successful call, data_p should be NULL, address contents should be zero  */
static void tp3()
{
    error_status_t    st;
    sec_pk_pubkey_t   in_publickey;
    sec_pk_pubkey_t   out_publickey;
    byte              *pkdata_p = (byte *)"ABCDKEY";
    unsigned32        kvno;

    in_publickey.len = 8;
    in_publickey.data = pkdata_p;

    mtet_infoline("(tp3) P3. sec_pubkey_store(), sec_pubkey_retrieve(), sec_pk_data_free() : check successful calls, with both sec_pk_usage_digitalSignature and sec_pk_usage_keyEncipherment\n");

    /* sec_pubkey_store(): check successful call, with both sec_pk_usage_digitalSignature and sec_pk_usage_keyEncipherment  */
    kvno=1;
    st = sec_pubkey_store( user,
                           &sec_pk_domain_dce_general,  /* sec_pk_domain_t domain */
                           sec_pk_usage_digitalSignature | sec_pk_usage_keyEncipherment, /* sec_pk_usage_flags_t key_usages */
                           &kvno,  /* unsigned32 kvno */
                           &in_publickey );
    CHECK_RESULT(true, "P3. SEC_PUBKEY: sec_pubkey_store", &st, error_status_ok);


    /* P3. sec_pubkey_retrieve(): check successful call, with sec_pk_usage_digitalSignature, after storing both */
    kvno=1;
    st = sec_pubkey_retrieve( user,
                              &sec_pk_domain_dce_general,  /* sec_pk_domain_t domain */
                              sec_pk_usage_digitalSignature, /* sec_pk_usage_flags_t key_usages */
                              &kvno, /* unsigned32 kvno */
                              &out_publickey );

    CHECK_RESULT(true, "SEC_PUBKEY: sec_pubkey_retrieve", &st, error_status_ok);
    /* verify that the value in *public_key is the same value _stored
       previously by doing a byte-by-byte comparison. */
    if (out_publickey.len != in_publickey.len) {
       mtet_infoline("P3. FAILED sec_pubkey_retrieve(): out_publickey.len does not match what was stored\n");
       tet_result(TET_FAIL);
       return;
    }
    else if ( memcmp( out_publickey.data, in_publickey.data, in_publickey.len ) != 0 ) {
       mtet_infoline("P3. FAILED sec_pubkey_retrieve(): out_publickey.data does not match what was stored\n");
       tet_result(TET_FAIL);
       return;
    }
    mtet_infoline("P3. PASSED sec_pubkey_retrieve(): data compare.\n");


    /* P3. sec_pk_data_free(): check successful call, data_p should be NULL */
    sec_pk_data_free( &out_publickey );

    CHECK_RESULT(true, "P3. SEC_PUBKEY: sec_pubkey_data_free", &st, error_status_ok);

    if ( out_publickey.data != NULL ) {
        mtet_infoline("P3. FAILED sec_pubkey_data_free(): out_publickey.data should be NULL, but is NOT!\n");
	tet_result(TET_FAIL);
	return;
    }

    /* sec_pubkey_retrieve(): check successful call, with sec_pk_usage_keyEncipherment,
        after storing both usages */
    kvno=1;
    st = sec_pubkey_retrieve( user,
                              &sec_pk_domain_dce_general,  /* sec_pk_domain_t domain */
                              sec_pk_usage_keyEncipherment, /* sec_pk_usage_flags_t key_usages */
                              &kvno, /* unsigned32 kvno */
                              &out_publickey );

    CHECK_RESULT(true, "P3. SEC_PUBKEY: sec_pubkey_retrieve", &st, error_status_ok);
    /* verify that the value in *public_key is the same value _stored
       previously by doing a byte-by-byte comparison. */
    if (out_publickey.len != in_publickey.len) {
       mtet_infoline("P3. FAILED sec_pubkey_retrieve(): out_publickey.len does not match what was stored\n");
       tet_result(TET_FAIL);
       return;
    }
    else if ( memcmp( out_publickey.data, in_publickey.data, in_publickey.len ) != 0 ) {
       mtet_infoline("P3. FAILED sec_pubkey_retrieve(): out_publickey.data does not match what was stored\n");
       tet_result(TET_FAIL);
       return;
    }
    mtet_infoline("P3. PASSED sec_pubkey_retrieve(): data compare.\n");


    /* sec_pk_data_free(): check successful call, data_p should be NULL */
    sec_pk_data_free( &out_publickey );

    CHECK_RESULT(true, "P3. SEC_PUBKEY: sec_pubkey_data_free", &st, error_status_ok);

    if ( out_publickey.data != NULL ) {
        mtet_infoline("P3. FAILED sec_pubkey_data_free(): out_publickey.data should be NULL, but is NOT!\n");
	tet_result(TET_FAIL);
	return;
    }

    tet_result(TET_PASS);

}
/*-----------------------------------------------------------------------*/

/* TP4: P1. sec_pubkey_retrieve(): check successful call, domain kdc_cache, with sec_pk_usage_digitalSignature; */
static void tp4()
{
    error_status_t    st;
    char * localcellname;
    char krbtgt_princ[256];
    sec_pk_pubkey_t   out_publickey;
    sec_pk_pubkey_t     in_publickey;
    byte                *pkdata_p=(byte *)"digS";
    sec_rgy_name_t      rgy_cell_name;
    unsigned32          kvno;

    mtet_infoline("(tp4) P1. sec_pubkey_retrieve() : check successful call, domain_kdc_cache, with sec_pk_usage_digitalSignature.\n");

    strcpy((char *)&rgy_cell_name[0], cellname);
    
    mtet_infoline("DEBUG: %s\n", cellname );
    localcellname = cellname + 4;
    sprintf(krbtgt_princ, "krbtgt%s", localcellname );
    mtet_infoline("DEBUG: %s\n", krbtgt_princ );

    in_publickey.len=5;
    in_publickey.data=pkdata_p;
    kvno=1;
    st = sec_pubkey_store( krbtgt_princ,
                           &sec_pk_domain_dce_general,
                           sec_pk_usage_digitalSignature,
                           &kvno,
                           &in_publickey );
    CHECK_RESULT(true, "P1. SEC_PUBKEY: sec_pubkey_store for sec_pubkey_retrieve", &st, error_status_ok);

    mtet_infoline("DEBUG: %s\n", cellname );
    kvno=1;
    st = sec_pubkey_retrieve( (void *)rgy_cell_name,
                              &sec_pk_domain_kdc_cache,
                              sec_pk_usage_digitalSignature, /* sec_pk_usage_flags_t key_usages */
                              &kvno, 
                              &out_publickey );

    free(cellname);
    CHECK_RESULT(true, "P1. SEC_PUBKEY: sec_pubkey_retrieve", &st, error_status_ok);
    /* possibly verify that the value in *public_key is the same value _stored in
       the kdc
       previously by doing a byte-by-byte comparison. */


    /* P1. sec_pk_data_free(): check successful call, data_p should be NULL */
    sec_pk_data_free( &out_publickey );

    CHECK_RESULT(true, "P1. SEC_PUBKEY: sec_pubkey_data_free", &st, error_status_ok);

    if ( out_publickey.data != NULL ) {
        mtet_infoline("P1. FAILED sec_pubkey_data_free(): out_publickey.data should be NULL, but is NOT!\n");
	tet_result(TET_FAIL);
	return;
    }

    tet_result(TET_PASS);

}
/*-----------------------------------------------------------------------*/


static void tp5()
{
    error_status_t    st;
    sec_rgy_name_t    foreign_cellname={NULL};  /* XXX-change to real foreign cell */
    sec_pk_pubkey_t   out_publickey;
    unsigned32        kvno;


    mtet_infoline("(tp5) P3. sec_pubkey_retrieve() : check successful call, domain kdc_cache_refresh, with sec_pk_usage_digitalSignature, using a foreign cell (XXX-non-functional test)\n");

    kvno=0;
    st = sec_pubkey_retrieve((void *)foreign_cellname,
                             &sec_pk_domain_kdc_cache_refresh,
                             sec_pk_usage_digitalSignature, /* sec_pk_usage_flags_t key_usages */
                             &kvno,
                             &out_publickey );

    CHECK_RESULT(true, "P3. SEC_PUBKEY: sec_pubkey_retrieve", &st, error_status_ok);
    /* possibly verify that the value in *public_key is the same value _stored in
       the kdc
       previously by doing a byte-by-byte comparison. */


    /* P3. sec_pk_data_free(): check successful call, data_p should be NULL */
    sec_pk_data_free( &out_publickey );

    CHECK_RESULT(true, "P3. SEC_PUBKEY: sec_pubkey_data_free", &st, error_status_ok);

    if ( out_publickey.data != NULL ) {
        mtet_infoline("P3. FAILED sec_pubkey_data_free(): out_publickey.data should be NULL, but is NOT!\n");
	tet_result(TET_FAIL);
	return;
    }

    tet_result(TET_PASS);

}
/*-----------------------------------------------------------------------*/

