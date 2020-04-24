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
 * $Log: rsec_pk_N.c,v $
 * Revision 1.1.2.2  1996/10/03  18:43:26  arvind
 * 	Add OSF copyright
 * 	[1996/09/30  15:44 UTC  arvind  /main/DCE_1.2.2/8]
 *
 * Revision 1.1.2.1  1996/08/09  20:12:40  arvind
 * 	DCE 1.2.2 intermediate drop (post bl3/pre bl4)
 * 	[1996/08/09  19:58:06  arvind]
 * 
 * $EndLog$
 */
/* rsec_pk_N.c  Error Condition testcases
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
#include <tet_api.h>
#include <ts-common.h>

#include <dce/binding.h>
#include <dce/dce_error.h>

#include <dce/rgynbase.h>
#include <dce/rgybase.h>
#include <dce/uuid.h>
#include <dce/sec_login.h>

#include <sec_def_flags.h>

#include <dce/sec_pk.h>
#include <dce/sec_pk_base.h>
#include <dce/sec_psm.h>
#include <dce/rsec_pk_cache.h>
#include <dce/dcedhdmsg.h>
#include <pthread.h>
#include "../common.h"

#undef PK_USER
#define PK_USER "test_rsec_pk_user"

static void tp1(void),tp2(void),tp3(void), tp4(void), tp5(void), tp6(void);
static void pk_startup(void), pk_cleanup(void);

                                  
unsigned char unexpected[dce_c_error_string_len];
unsigned char expected[dce_c_error_string_len];
sec_login_handle_t login_h;
sec_rgy_name_t     user;
sec_passwd_str_t   passwd;
int                dce_status;
sec_login_handle_t ilh = NULL;
sec_rgy_handle_t   write_context;
rpc_binding_handle_t  dced_handle;
sec_rgy_name_t cellname;
unsigned32         real_kvno;

/* List of tests to run that TET will be looking for */
struct tet_testlist tet_testlist[] = {
    {tp1, 1},
    {tp2, 2},
    {tp3, 3},
    {tp4, 4},
    {tp5, 5},
    {tp6, 6},
    {NULL, 0}
};

void (*tet_startup)() = pk_startup;
void (*tet_cleanup)() = pk_cleanup;


/* Initialization of data needed for each of the test cases */
static void pk_startup(void)
{
    error_status_t          st;
    char *     cellname_p;

    tet_infoline("rsec_pk test startup");
    CLEAR_STATUS(&st);
    strcpy((char *)user, PK_USER);
    strcpy((char *)passwd, PK_PASSWD);

    dce_cf_get_cell_name( & cellname_p, & st );
    strcpy(cellname, cellname_p);
    free(cellname_p);

    /* add a generic test_rsec_pk_user */

    if (system("./setup.ksh") != 0) {
        int test_no;
        mtet_infoline("FAILED: system(./setup.ksh)\n" );
        for (test_no=0; tet_testlist[test_no++].testfunc != NULL;)
            tet_delete(test_no, "startup() has failed");
        return;
    }

    sec_login_util_bind_to_sec_clientd( & dced_handle, &st );
    CHECK_STARTUP_RESULT(true, "N. RSEC_PK: rsec_pk_cache_kdc_key: sec_login_util_bind_to_sec_clientd", &st, error_status_ok);

    return;
}


/* Cleanup of data needed for each of the test cases */
static void pk_cleanup(void)
{
    error_status_t  st;

    rpc_binding_free(&dced_handle, &st);

    system("./cleanup.ksh");

    return;
}


static void tp1()
{
    error_status_t  st;
    sec_pk_pubkey_t out_pubkey;
    sec_rgy_name_t cellname;
    unsigned32     kvno;
    int            i;

    mtet_infoline("(tp1) N1. rsec_pk_cache_kdc_key(): check Unsuccessful call, with bad cell name.\n" );

    /* initialize cellname with a name that will be longer than a sec_rgy_name_t once
     * "krbtgt" is added to it
     */
    strcpy((char *)cellname, "/.../");
    for (i=strlen("/.../"); i<(sec_rgy_name_max_len); i++)
        cellname[i]='a';
    cellname[sec_rgy_name_t_size]='\0';

    kvno=0;
    st = (*rsec_pk_cache_v1_1_c_epv.rsec_pk_cache_kdc_key)
           ( dced_handle,
             cellname,
             sec_pk_usage_digitalSignature,
             & kvno,
             & out_pubkey );


    CHECK_RESULT(true, "(tp1) N1. RSEC_PK: rsec_pk_cache_kdc_key", &st, dced_s_pkcache_cell_invalid);


    tet_result(TET_PASS);
}
/*-----------------------------------------------------------------------*/


static void tp2()
{
    error_status_t  st;
    sec_pk_pubkey_t out_pubkey;
    unsigned32      kvno;

    mtet_infoline("(tp2) N1. rsec_pk_cache_kdc_key(): check Unsuccessful call, with bad key usage sec_pk_usage_keyEncipherment.\n" );

    kvno=0;
    st = (*rsec_pk_cache_v1_1_c_epv.rsec_pk_cache_kdc_key)
           ( dced_handle,
             cellname,
             sec_pk_usage_keyEncipherment, /* bad key usage */
             & kvno,
             & out_pubkey );

    CHECK_RESULT(true, "N1. RSEC_PK: rsec_pk_cache_kdc_key", &st, dced_s_pkcache_usage_unsupp);


    tet_result(TET_PASS);
}
/*-----------------------------------------------------------------------*/


static void tp3()
{
    error_status_t  st;
    sec_pk_pubkey_t out_pubkey;
    unsigned32      kvno;


    mtet_infoline("(tp3) N1. rsec_pk_cache_kdc_key(): check Unsuccessful call, with more than one usage bit set sec_pk_usage_digitalSignature | sec_pk_usage_keyEncipherment .\n" );

    kvno=0;
    st = (*rsec_pk_cache_v1_1_c_epv.rsec_pk_cache_kdc_key)
           ( dced_handle,
             cellname,
             sec_pk_usage_digitalSignature | sec_pk_usage_keyEncipherment,
             & kvno,
             & out_pubkey );

    CHECK_RESULT(true, "(tp3) N1. RSEC_PK: rsec_pk_cache_kdc_key", &st, dced_s_pkcache_usage_unsupp);

    tet_result(TET_PASS);
}
/*-----------------------------------------------------------------------*/


static void tp4()
{
    error_status_t  st;
    sec_pk_pubkey_t out_pubkey;
    struct timespec interval;
    unsigned32      kvno;

    mtet_infoline("(tp4) N1. rsec_pk_cache_kdc_key(): check Unsuccessful call, with a too recent key cache operation.\n" );

    mtet_infoline("(tp4) N1. rsec_pk_cache_kdc_key(): 1st call.\n" );

    /* sleep for 5 mins, to refresh from pos test */
    mtet_infoline("(tp4) N1. waiting 5 mins after previous test to refresh...\n" );
    interval.tv_sec = 300;
    interval.tv_nsec = 0;
    pthread_delay_np(&interval);

    kvno=0;
    st = (*rsec_pk_cache_v1_1_c_epv.rsec_pk_cache_kdc_key)
           ( dced_handle,
             cellname,
             sec_pk_usage_digitalSignature,
             & kvno,
             & out_pubkey );

    CHECK_RESULT(true, "N1. RSEC_PK: rsec_pk_cache_kdc_key 1st try", &st, error_status_ok);

    /* Save actual key version for subsequent test */
    real_kvno=kvno;

    sec_pk_data_free( &out_pubkey );

    /* rely on previous call's cache,
       then try to cache a second time, in less then 5 mins */
    mtet_infoline("(tp4) N1. rsec_pk_cache_kdc_key(): 2nd call.\n" );
    kvno=0;
    st = (*rsec_pk_cache_v1_1_c_epv.rsec_pk_cache_kdc_key)
           ( dced_handle,
             cellname,
             sec_pk_usage_digitalSignature,
             & kvno,
             & out_pubkey );

    CHECK_RESULT(true, "N1. RSEC_PK: rsec_pk_cache_kdc_key 2nd try", &st, dced_s_pkcache_key_fresh);


    tet_result(TET_PASS);
}

/*-----------------------------------------------------------------------*/


static void tp5()
{
    error_status_t  st;
    sec_pk_pubkey_t out_pubkey;
    struct timespec interval;
    unsigned32      kvno;

    mtet_infoline("(tp5) N1. rsec_pk_cache_kdc_key(): check Unsuccessful call, with bad key version.\n" );

    /* sleep for 5 mins, to refresh from pos test */
    mtet_infoline("(tp5) N1. waiting 5 mins after previous test to refresh...\n" );
    interval.tv_sec = 300;
    interval.tv_nsec = 0;
    pthread_delay_np(&interval);

    kvno=256;
    st = (*rsec_pk_cache_v1_1_c_epv.rsec_pk_cache_kdc_key)
           ( dced_handle,
             cellname,
             sec_pk_usage_digitalSignature,
             & kvno,
             & out_pubkey );

    CHECK_RESULT(true, "N1. RSEC_PK: rsec_pk_cache_kdc_key", &st, sec_pk_e_key_vers_unavailable);

    tet_result(TET_PASS);
}

/*-----------------------------------------------------------------------*/

static void tp6()
{
    error_status_t  st;
    sec_pk_pubkey_t out_pubkey;
    struct timespec interval;
    unsigned32      kvno;

    mtet_infoline("(tp6) N1. rsec_pk_cache_kdc_key(): check Unsuccessful call, with non-existent key version.\n" );

    /* sleep for 5 mins, to refresh from pos test */
    mtet_infoline("(tp6) N1. waiting 5 mins after previous test to refresh...\n" );
    interval.tv_sec = 300;
    interval.tv_nsec = 0;
    pthread_delay_np(&interval);

    kvno=real_kvno+1;
    if (kvno>255)
        kvno=1;
    st = (*rsec_pk_cache_v1_1_c_epv.rsec_pk_cache_kdc_key)
           ( dced_handle,
             cellname,
             sec_pk_usage_digitalSignature,
             & kvno,
             & out_pubkey );

    CHECK_RESULT(true, "N1. RSEC_PK: rsec_pk_cache_kdc_key", &st, sec_pk_e_key_vers_unavailable);

    tet_result(TET_PASS);
}
/*-----------------------------------------------------------------------*/

