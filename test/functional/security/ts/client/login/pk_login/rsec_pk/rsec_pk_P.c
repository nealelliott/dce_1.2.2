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
 * $Log: rsec_pk_P.c,v $
 * Revision 1.1.2.2  1996/10/03  18:43:39  arvind
 * 	Add OSF copyright
 * 	[1996/09/30  15:44 UTC  arvind  /main/DCE_1.2.2/9]
 *
 * Revision 1.1.2.1  1996/08/09  20:12:53  arvind
 * 	DCE 1.2.2 intermediate drop (post bl3/pre bl4)
 * 	[1996/08/09  19:58:14  arvind]
 * 
 * $EndLog$
 */
/* r s e c _ p k _ c a c h e _ k d c _ k e y
     *
     * rsec_pk_cache_kdc_key - this routine requests dced to
     * obtain and cache, or refresh cache, of public key of
     * the KDC of a specified cell.  The key is cached in
     * the file ${KDC_PUBKEY_CACHE_DIR}/<cell>/${KDC_PUBKEY_CACHE_FILE}
     * (KDC_PUBKEY_CACHE_DIR/FILE defined in security/h/rca_fileloc.h).
     *
     * Keys may be retrieved from this file via the
     * sec_pubkey_retrieve() interface with domain
     * sec_pk_domain_kdc_cache.  The sec_pubkey interface invokes
     * the rsec_pk_cache_kdc_key interface if the cache is empty or
     * a refresh request is being made.
     *
     * rsec_pk_cache_kdc_key refuses to refresh an existing
     * cache file if it has been written within the past five
     * minutes.  This is intended to stall denial-of-service
     * attacks.
     *
     * In Parameters
     *   cell           - canonical cell name (/.../cell_name)
     *   usage          - must be "sec_pk_usage_digitalSignature" in the
     *                    reference implementation.
     *   kvno           - pointer to key version desired (or NULL)
     *
     * Out Parameters
     *   kvno           - pointer to key version returned (or NULL)
     *   public_key     - DER-encoded public key value
     *
     * Returns:
     *   dced_s_pkcache_cellname_invalid - bad cellname
     *   dced_s_pkcache_usage_unsupp - usage value unsupported
     *   dced_s_pkcache_key_fresh - key was cached recently
     *            (within KEY_CACHE_STALE_SECS)
*/
 

#include <stdio.h>
#include <errno.h>
#include <macros.h>
#include <u_str.h>
#include <tet_api.h>
#include <ts-common.h>

#include <dce/binding.h>
#include <dce/dce_error.h>

#include <dce/rgybase.h>
#include <dce/uuid.h>
#include <dce/sec_login.h>

#include <sec_def_flags.h>
#include <pthread.h>

#include <dce/sec_pk.h>
#include <dce/sec_pk_base.h>
#include <dce/sec_psm.h>
#include <dce/rsec_pk_cache.h>

#include "../common.h"

#undef PK_USER
#define PK_USER "test_rsec_pk_user"

static void tp1(void), tp2(void);
static void pk_startup(void), pk_cleanup(void);

                                  
unsigned char unexpected[dce_c_error_string_len];
unsigned char expected[dce_c_error_string_len];
sec_login_handle_t login_h;
sec_rgy_name_t     user;
sec_passwd_str_t   passwd;
int                dce_status;
sec_login_handle_t ilh = NULL;
sec_rgy_handle_t   write_context;
char *     cellname;
sec_rgy_name_t     rgy_cell_name;


/* List of tests to run that TET will be looking for */
struct tet_testlist tet_testlist[] = {
    {tp1, 1},
    {tp2, 2},
    {NULL, 0}
};

void (*tet_startup)() = pk_startup;
void (*tet_cleanup)() = pk_cleanup;


/* Initialization of data needed for each of the test cases */
static void pk_startup(void)
{
    error_status_t          st;

    tet_infoline("rsec_pk test startup");
    CLEAR_STATUS(&st);
    strcpy((char *)user, PK_USER);
    strcpy((char *)passwd, PK_PASSWD);

    dce_cf_get_cell_name( & cellname, & st );

    u_strncpy(rgy_cell_name, cellname, sec_rgy_name_max_len);

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
    system("./cleanup.ksh");

    return;
}


/* TP1: P1. check successful call using digitalSignature, with refresh TRUE 
*/

static void tp1()
{
    error_status_t  st, rpcst, lst;
    handle_t        dced_handle;
    sec_pk_pubkey_t out_pubkey;
    sec_pk_pubkey_t in_pubkey;
    byte            *pkdata_p=(byte *)"digS";
    char * localcellname;
    char krbtgt_princ[256];
    unsigned32        kvno;
    struct timespec interval;
    unsigned32        real_kvno=1;


    mtet_infoline("(tp1) P1. rsec_pk_cache_kdc_key(): check successful call, with key usage digitalSignature.\n" );

    /* sleep for 5 mins, to refresh from sec_pubkey_retrieve_kdc_key test */
    mtet_infoline("P1. RSEC_PK: waiting 5 mins after previous test to refresh...\n");
    interval.tv_sec = 300;
    interval.tv_nsec = 0;
    pthread_delay_np(&interval);

    /* store the kdc key first for testing */
    localcellname = cellname + 4;
    sprintf(krbtgt_princ, "krbtgt%s", localcellname );

    in_pubkey.len=5;
    in_pubkey.data=pkdata_p;
    kvno=real_kvno;
    st = sec_pubkey_store( krbtgt_princ,
                           &sec_pk_domain_dce_general,
                           sec_pk_usage_digitalSignature,
                           &kvno,
                           &in_pubkey );
    CHECK_RESULT(true, "P1. RSEC_PK: sec_pubkey_store for rsec_pk_cache_kdc_key", &st, error_status_ok);

    /* now do the cache_kdc_key */
    sec_login_util_bind_to_sec_clientd( & dced_handle, &st );
    CHECK_RESULT(true, "P1. RSEC_PK: rsec_pk_cache_kdc_key: sec_login_util_bind_to_sec_clientd", &st, error_status_ok);

    kvno=0;
    st = (*rsec_pk_cache_v1_1_c_epv.rsec_pk_cache_kdc_key)
           ( dced_handle,
             rgy_cell_name,
             sec_pk_usage_digitalSignature,
             & kvno,
             & out_pubkey );

    CHECK_RESULT(true, "P1. RSEC_PK: rsec_pk_cache_kdc_key", &st, error_status_ok);

    rpc_binding_free(&dced_handle, &lst);
    CHECK_RESULT(true, "P1. RSEC_PK: rpc_binding_free", &lst, error_status_ok);

    /* verify that the value in *public_key is the same value _stored
       previously by doing a byte-by-byte comparison.
    */
    if (out_pubkey.len != in_pubkey.len) {
       mtet_infoline("P1. FAILED rsec_pk_cache_kdc_key(): out_pubkey len (%d) does not match what was stored (%d)\n", out_pubkey.len, in_pubkey.len);
       tet_result(TET_FAIL);
       return;
    }
    else if ( memcmp( out_pubkey.data, in_pubkey.data, in_pubkey.len ) != 0 ) {
       mtet_infoline("P1. FAILED rsec_pk_cache_kdc_key(): out_publickey data (%s) does not match what was stored (%s)\n", out_pubkey.data, in_pubkey.data);
       tet_result(TET_FAIL);
       return;
    }
    else if (kvno != real_kvno) {
       mtet_infoline("P1. FAILED rsec_pk_cache_kdc_key(): output key version (%d) does not match what was stored (%d)\n", kvno, real_kvno);
       tet_result(TET_FAIL);
       return;
    }
    mtet_infoline("PASSED P1. rsec_pk_cache_kdc_key(): data compare.\n");

    sec_pk_data_free( &out_pubkey );

    CHECK_RESULT(true, "P1. RSEC_PK: sec_pubkey_data_free", &st, error_status_ok);


    tet_result(TET_PASS);
}

/*-----------------------------------------------------------------------*/

/* TP2: P1. check successful call using digitalSignature, with specific key version
*/

static void tp2()
{
    error_status_t  st, rpcst, lst;
    handle_t        dced_handle;
    sec_pk_pubkey_t out_pubkey;
    sec_pk_pubkey_t in_pubkey;
    byte            *pkdata_p=(byte *)"keyWithVersion";
    char * localcellname;
    char krbtgt_princ[256];
    unsigned32        kvno;
    struct timespec interval;
    unsigned32        real_kvno=8;


    mtet_infoline("(tp2) P1. rsec_pk_cache_kdc_key(): check successful call, with specified key version.\n" );

    /* now do the cache_kdc_key */
    sec_login_util_bind_to_sec_clientd( & dced_handle, &st );
    CHECK_RESULT(true, "P1. RSEC_PK: rsec_pk_cache_kdc_key: sec_login_util_bind_to_sec_clientd", &st, error_status_ok);

    /* sleep for 5 mins, to refresh from previous test */
    mtet_infoline("P1. RSEC_PK: waiting 5 mins after previous test to refresh...\n");
    interval.tv_sec = 300;
    interval.tv_nsec = 0;
    pthread_delay_np(&interval);

    /* store the kdc key first for testing */
    localcellname = cellname + 4;
    sprintf(krbtgt_princ, "krbtgt%s", localcellname );

    /* Write a new key value to be picked up by dced */
    in_pubkey.len=strlen((char *)pkdata_p);
    in_pubkey.data=pkdata_p;
    kvno=real_kvno;
    st = sec_pubkey_store( krbtgt_princ,
                           &sec_pk_domain_dce_general,
                           sec_pk_usage_digitalSignature,
                           &kvno,
                           &in_pubkey );
    CHECK_RESULT(true, "P1. RSEC_PK: sec_pubkey_store for rsec_pk_cache_kdc_key", &st, error_status_ok);

    kvno=real_kvno;
    st = (*rsec_pk_cache_v1_1_c_epv.rsec_pk_cache_kdc_key)
           ( dced_handle,
             rgy_cell_name,
             sec_pk_usage_digitalSignature,
             & kvno,
             & out_pubkey );

    CHECK_RESULT(true, "P1. RSEC_PK: rsec_pk_cache_kdc_key", &st, error_status_ok);

    rpc_binding_free(&dced_handle, &lst);
    CHECK_RESULT(true, "P1. RSEC_PK: rpc_binding_free", &lst, error_status_ok);

    /* verify that the value in *public_key is the same value _stored
       previously by doing a byte-by-byte comparison.
    */
    if (out_pubkey.len != in_pubkey.len) {
       printf("out_pubkey.len=%d\nin_pubkey.len=%d\n", out_pubkey.len, in_pubkey.len);
       printf("out_pubkey.data=%*s\nin_pubkey.data=%*s\n", 
               out_pubkey.len, out_pubkey.data, in_pubkey.len, in_pubkey.data);
       mtet_infoline("P1. FAILED rsec_pk_cache_kdc_key(): out_pubkey len (%d) does not match what was stored (%d)\n", out_pubkey.len, in_pubkey.len);
       tet_result(TET_FAIL);
       return;
    }
    else if ( memcmp( out_pubkey.data, in_pubkey.data, in_pubkey.len ) != 0 ) {
       mtet_infoline("P1. FAILED rsec_pk_cache_kdc_key(): out_publickey data (%s) does not match what was stored (%s)\n", out_pubkey.data, in_pubkey.data);
       tet_result(TET_FAIL);
       return;
    }
    else if (kvno != real_kvno) {
       mtet_infoline("P1. FAILED rsec_pk_cache_kdc_key(): output key version (%d) does not match what was stored (%d)\n", kvno, real_kvno);
       tet_result(TET_FAIL);
       return;
    }
    mtet_infoline("PASSED P1. rsec_pk_cache_kdc_key(): data compare.\n");

    sec_pk_data_free( &out_pubkey );

    CHECK_RESULT(true, "P1. RSEC_PK: sec_pubkey_data_free", &st, error_status_ok);


    tet_result(TET_PASS);
}
/*-----------------------------------------------------------------------*/
