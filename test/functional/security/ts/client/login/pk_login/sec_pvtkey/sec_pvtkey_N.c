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
 * $Log: sec_pvtkey_N.c,v $
 * Revision 1.1.2.2  1996/10/03  18:46:39  arvind
 * 	Add OSF copyright
 * 	[1996/09/30  15:45 UTC  arvind  /main/DCE_1.2.2/7]
 *
 * Revision 1.1.2.1  1996/08/09  20:15:51  arvind
 * 	DCE 1.2.2 intermediate drop (post bl3/pre bl4)
 * 	[1996/08/09  20:02:00  arvind]
 * 
 * $EndLog$
 */
/* sec_pubkey_N.c  Error Condition testcases
*/
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <macros.h>
#include <tet_api.h>
#include <ts-common.h>

#include <dce/binding.h>
#include <dce/dce_cf.h>
#include <dce/dce_error.h>

#include <dce/rgybase.h>
#include <dce/uuid.h>
#include <dce/sec_login.h>

#include <sec_def_flags.h>

#include <dce/dce_msg.h>
#include <dce/dcesadmsg.h>

#include <dce/sec_pk.h>
#include <dce/sec_pk_base.h>
#include <dce/sec_pvtkey.h>
#include "sec_bsafe.h"

#include "../common.h"

#undef PK_USER
#define PK_USER "test_pvtkey_pk_user"

#define PVTKEY_FILE_PREFIX "/opt/dcelocal/var/security/pk_file/"

static void tp1(void), tp2(void), tp3(void), tp4(void), tp5(void);
static void pk_startup(void), pk_cleanup(void);

unsigned32 pk_size = 1024;
                                  
unsigned char unexpected[dce_c_error_string_len];
unsigned char expected[dce_c_error_string_len];
sec_login_handle_t login_h;
unsigned_char_p_t  user = (unsigned_char_p_t)PK_USER;
sec_passwd_str_t   passwd_buf;
int                dce_status;
sec_login_handle_t ilh = NULL;
sec_rgy_handle_t   write_context;
char * cellname;
sec_passwd_str_t   passwd;
sec_rgy_name_t     new_pwd;
sec_rgy_name_t     bad_pwd;
char rm_pk_file_com[512];


/* List of tests to run that TET will be looking for */
struct tet_testlist tet_testlist[] = {
    {tp1, 1},
    {tp2, 2},
    {tp3, 3},
    {tp4, 4},
/*    {tp5, 5},    LOGIN CONTEXT CHANGE NOT RUN */
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

    strcpy((char *)user, PK_USER);
    strcpy((char *)passwd, PK_PASSWD);
    strcpy((char *)new_pwd, NEW_PK_PASSWD);
    strcpy((char *)bad_pwd, "bogus_pwd");

    dce_cf_get_cell_name( & cellname, & st );

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
        mtet_infoline("FAILED: system(./setup.ksh)\n");
        for (test_no=0; tet_testlist[test_no++].testfunc != NULL;)
            tet_delete(test_no, "startup() has failed");
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


/* Cleanup of data needed for each of the test cases, deleting pk_user
   account  */
static void pk_cleanup(void)
{

    if (system("./cleanup.ksh") != 0) {
        mtet_infoline("FAILED: system(./cleanup.ksh)\n");
    }
    system( rm_pk_file_com );

    return;
}


/* TP1: check Unsuccessful calls of sec_pvtkey_update
   1. with non existing pvt key
   2. invalid passwd */

static void tp1()
{
    error_status_t    st;

    sec_pk_mechanism_handle_t  pk_h = NULL;
    sec_pk_pubkey_t   pubkey;
    sec_pk_pubkey_t   pvtkey;
    boolean32 opened = false;
    unsigned32       kno;

    system( rm_pk_file_com );

    mtet_infoline("(tp1) N1. sec_pvtkey_get: check Unsuccessful call, with no match private key.\n");

    st = sec_pvtkey_open( user,
                          & sec_pk_domain_dce_general,
                          & pk_h);
    CHECK_RESULT_DONT_RETURN(true, "N. SEC_PVTKEY: sec_pvtkey_open", &st, error_status_ok);
    opened = true;

    mtet_infoline("          (generating public key; be patient...)\n");
    st = sec_bsafe_gen_pub_key(pk_size, NULL, &pubkey, &pvtkey);
    CHECK_RESULT_DONT_RETURN(true, "N. SEC_PVTKEY: sec_bsafe_gen_pub_key", &st, error_status_ok);


    mtet_infoline("(tp1) N2. sec_pvtkey_update: check Unsuccessful call, with no match private key.\n");

    st = sec_pvtkey_update( pk_h,
                            passwd,
                            new_pwd,
                            sec_pk_usage_digitalSignature,
                            & kno,
                            &pvtkey,
                            &pubkey );
    CHECK_RESULT_DONT_RETURN(true, "N2. SEC_PVTKEY: sec_pvtkey_update status check", &st, sec_pvtkey_no_matched_private_key );


    /* now really _store to test next _update */
    st = sec_pvtkey_store( pk_h,
                           passwd,
                           sec_pk_usage_digitalSignature,  /* key usage */
                           0,     /* kvno */
                           &pvtkey,
                           &pubkey );

    CHECK_RESULT_DONT_RETURN(true, "tp1() N. SEC_PVTKEY: sec_pvtkey_store", &st, error_status_ok );


    mtet_infoline("(tp1) N4. sec_pvtkey_update: check Unsuccessful call, with invalid password.\n");
    st = sec_pvtkey_update( pk_h,
                            bad_pwd,
                            new_pwd,
                            sec_pk_usage_digitalSignature,
                            & kno,
                            &pvtkey,
                            &pubkey );

    sec_pk_data_free( &pubkey );
    sec_pk_data_zero_and_free( &pvtkey );

    CHECK_RESULT_DONT_RETURN(true, "N4. SEC_PVTKEY: sec_pvtkey_update status check", &st, sec_s_bsafe_decryption_failure ); 
    if (opened) {
       st = sec_pvtkey_close( pk_h );
       CHECK_RESULT_DONT_RETURN(true, "tp1() N. SEC_PVTKEY: sec_pvtkey_close", &st, error_status_ok );
    }
 
    tet_result(TET_PASS);

}
/*-----------------------------------------------------------------------*/


/* TP2:  check Unsuccessful calls with invalid handle */

static void tp2()
{
    error_status_t    st;

    sec_pk_mechanism_handle_t  pk_h = NULL;
    sec_pk_pubkey_t   pubkey;
    sec_pk_pubkey_t   pvtkey;

    mtet_infoline("(tp2) N1. sec_pvtkey_store: check unsuccessful call, with bogus handle.\n");

    st = sec_pvtkey_store( pk_h,
                           passwd,
                           sec_pk_usage_digitalSignature,  /* key usage */
                           0,     /* kvno */
                           &pvtkey,
                           &pubkey );

    CHECK_RESULT_DONT_RETURN(true, "tp2() N1. SEC_PVTKEY: sec_pvtkey_store", &st, sec_pvtkey_mechanism_not_init );

    mtet_infoline("(tp2) N2. sec_pvtkey_get: check unsuccessful call, with bogus handle.\n");
    st = sec_pvtkey_get( pk_h,
                         passwd,
                         sec_pk_usage_keyEncipherment,  /* key usage */
                         0,     /* kvno */
                         & pvtkey,
                         & pubkey );

    CHECK_RESULT_DONT_RETURN(true, "tp2() N2. SEC_PVTKEY: sec_pvtkey_get", &st, sec_pvtkey_mechanism_not_init );


    mtet_infoline("(tp2) N3. sec_pvtkey_update: check unsuccessful call, with bogus handle.\n");
    st = sec_pvtkey_update( pk_h,
                            passwd,
                            new_pwd,
                            sec_pk_usage_digitalSignature,  /* key usage */
                            0,     /* kvno */
                            &pvtkey,
                            &pubkey );

    CHECK_RESULT_DONT_RETURN(true, "tp2() N3. SEC_PVTKEY: sec_pvtkey_update", &st, sec_pvtkey_mechanism_not_init);

    mtet_infoline("(tp2) N4. sec_pvtkey_close(): check unsuccessful call, with bogus handle.\n");

    st = sec_pvtkey_close( pk_h );

    CHECK_RESULT_DONT_RETURN(true, "tp2() N4. SEC_PVTKEY: sec_pvtkey_close", &st, sec_pvtkey_mechanism_not_init );

 
    tet_result(TET_PASS);

}
/*-----------------------------------------------------------------------*/


/* TP3: check Unsuccessful calls of sec_pvtkey_store with sec_pvtkey_same_domain_and_usage_key_already_exist */

static void tp3()
{
    error_status_t    st;

    sec_pk_mechanism_handle_t  pk_h = NULL;
    sec_pk_pubkey_t   pubkey;
    sec_pk_pubkey_t   pvtkey;
    boolean32 opened = false;


    mtet_infoline("(tp3) N1. sec_pvtkey_store: check unsuccessful call, with same usages and domain.\n");

    st = sec_pvtkey_open( user,
                          & sec_pk_domain_dce_general,
                          & pk_h);
    CHECK_RESULT_DONT_RETURN(true, "N1. SEC_PVTKEY: sec_pvtkey_open", &st, error_status_ok);
    opened = true;

    mtet_infoline("          (generating public key; be patient...)\n");
    st = sec_bsafe_gen_pub_key(pk_size, NULL, &pubkey, &pvtkey);

    CHECK_RESULT_DONT_RETURN(true, "N1. SEC_PVTKEY: sec_bsafe_gen_pub_key", &st, error_status_ok);


#if 0
    /* currently we don't need to store first for this test, because it is stored
     * in tp1(). If  _delete is implemented, then it will be used to cleanup
     */

    st = sec_pvtkey_store( pk_h,
                           passwd,
                           255,
                           0,  
                           &pvtkey,
                           &pubkey );

    CHECK_RESULT_DONT_RETURN(true, "tp3() N1. SEC_PVTKEY: sec_pvtkey_store 1st time", &st, error_status_ok );
#endif
    
    st = sec_pvtkey_store( pk_h,
                           passwd,
                           255,  /* key usage */
                           0,     /* kvno */
                           &pvtkey,
                           &pubkey );

    CHECK_RESULT_DONT_RETURN(true, "tp3() N1. SEC_PVTKEY: sec_pvtkey_store 2nd time", &st, sec_pvtkey_multiple_key_usages );

    sec_pk_data_free( &pubkey );
    sec_pk_data_zero_and_free( &pvtkey );

    if (opened) {
       st = sec_pvtkey_close( pk_h );
       CHECK_RESULT_DONT_RETURN(true, "tp3() N4. SEC_PVTKEY: sec_pvtkey_close", &st, error_status_ok );
    }
 
    tet_result(TET_PASS);

}
/*-----------------------------------------------------------------------*/


/* TP4: check Unsuccessful calls of sec_pvtkey_generate,
        sec_pvtkey_update_generate with unsupported */

static void tp4()
{
    error_status_t    st;

    sec_pk_mechanism_handle_t  pk_h = NULL;
    sec_pk_pubkey_t   pubkey;
    boolean32 opened = false;


    mtet_infoline("(tp4) N1. sec_pvtkey_generate: check unsuccessful call, Unsupported\n");

    st = sec_pvtkey_open( user,
                          & sec_pk_domain_dce_general,
                          & pk_h );
    CHECK_RESULT_DONT_RETURN(true, "N1. SEC_PVTKEY: sec_pvtkey_open", &st, error_status_ok);
    opened = true;

    st = sec_pvtkey_generate( pk_h,
                              passwd,
                              NULL,  /*  sec_pk_algorithm_id *alg_id  */
                              0,  /* key length */
                              sec_pk_usage_digitalSignature, /* key usage */
                              0,     /* kvno */
                              & pubkey );

    CHECK_RESULT_DONT_RETURN(true, "tp4() N1. SEC_PVTKEY: sec_pvtkey_generate", &st, sec_pvtkey_key_generation_not_supported );


    mtet_infoline("(tp4) N2. sec_pvtkey_update_generate: check unsuccessful call, Unsupported\n");
    st = sec_pvtkey_update_generate( pk_h,
                                     passwd,
                                     new_pwd,
                                     NULL,  /*  sec_pk_algorithm_id *alg_id  */
                                     1,  /* key length */
                                     sec_pk_usage_digitalSignature, /* key usage */
                                     0,     /* kvno */
                                     & pubkey );

    CHECK_RESULT_DONT_RETURN(true, "tp4() N1. SEC_PVTKEY: sec_pvtkey_generate", &st, sec_pvtkey_key_generation_not_supported );

    if (opened) {
       st = sec_pvtkey_close( pk_h );
       CHECK_RESULT_DONT_RETURN(true, "tp4() N1. SEC_PVTKEY: sec_pvtkey_close", &st, error_status_ok);
    }
 
    tet_result(TET_PASS);

}
/*-----------------------------------------------------------------------*/


/* TP5: N1. sec_pvtkey_open():  check Unsuccessful call, as unauth user */

static void tp5()
{
#if 0
    error_status_t    st;
    sec_passwd_rec_t  pwdrec;
    boolean32         reset_passwd;
    sec_login_auth_src_t    auth_src;
    sec_pk_mechanism_handle_t  pk_h;    
#endif

    mtet_infoline("(tp5) N1. sec_pvtkey_open(): check unsuccessful call for sec_rgy_not_authorized  not as cell_admin.\n");

    /* login as pk_user */
#if 0
    if (sec_login_setup_identity(user, sec_login_no_flags, &login_h, &st)) {
        pwdrec.key.key_type = sec_passwd_plain;
        pwdrec.key.tagged_union.plain = passwd_buf;
        pwdrec.pepper = NULL;
        pwdrec.version_number = sec_passwd_c_version_none;

        if ( !sec_login_validate_identity(login_h, &pwdrec, &reset_passwd, &auth_src, &st)) {
            CHECK_STARTUP_RESULT(false,
                "SEC_PVTKEY_OPEN login: sec_login_validate_identity ", &st,
                error_status_ok);
        }

        if ( !sec_login_certify_identity(login_h, &st)) {
            CHECK_STARTUP_RESULT(false,
                "SEC_LOGIN_DELEG setup: sec_login_certify_identity ", &st,
                error_status_ok);
        }
  
        sec_login_set_context(login_h, &st);
        CHECK_RESULT_DONT_RETURN(false, "SEC_PVTKEY_OPEN: sec_login_set_context ", &st,
                error_status_ok);

        if (auth_src != sec_login_auth_src_network)
	   printf("SEC_PVTKEY_OPEN: no network creds\n");
          
    } else {
        CHECK_RESULT_DONT_RETURN(false, "SEC_PVTKEY_OPEN: sec_login_setup_identity ", &st,
               error_status_ok);
    }

    st = sec_pvtkey_open( "pk_user",
                          & sec_pk_domain_dce_general,
                          & pk_h );
    CHECK_RESULT_DONT_RETURN(true, "N1. SEC_PVTKEY: sec_pvtkey_open", &st, sec_pvtkey_s_privileged );

    if (login_h != NULL) sec_login_purge_context(&login_h, &st);
#endif

    tet_result(TET_PASS);

}
/*-----------------------------------------------------------------------*/
