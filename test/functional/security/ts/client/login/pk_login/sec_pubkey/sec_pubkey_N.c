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
 * $Log: sec_pubkey_N.c,v $
 * Revision 1.1.2.2  1996/10/03  18:45:38  arvind
 * 	Add OSF copyright
 * 	[1996/09/30  15:44 UTC  arvind  /main/DCE_1.2.2/6]
 *
 * Revision 1.1.2.1  1996/08/09  20:14:52  arvind
 * 	DCE 1.2.2 intermediate drop (post bl3/pre bl4)
 * 	[1996/08/09  20:00:22  arvind]
 * 
 * $EndLog$
 */
/* sec_pubkey_N.c  Error Condition testcases
*/
#include <stdio.h>
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

#include <dce/dce_cf.h>
#include <dce/sec_pubkey.h>
#include <dce/sec_pk.h>
#include <dce/dce_msg.h>
#include <dce/dcesadmsg.h>

#include <../common.h>

#undef PK_USER
#define PK_USER "test_pubkey_pk_user"

static void tp1(void), tp2(void), tp3(void), tp4(void), tp5(void), tp6(void), tp7(void), tp8(void), tp9(void), tp10(void);
static void pk_startup(void), pk_cleanup(void);

                                  
unsigned char unexpected[dce_c_error_string_len];
unsigned char expected[dce_c_error_string_len];
sec_login_handle_t login_h;
unsigned_char_p_t  user = (unsigned_char_p_t)PK_USER;
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
/*    {tp5, 5},  NOT DOING CHANGE LOGIN CONTEXT */
    {tp6, 6},
/*    {tp7, 7},  NOT DOING CHANGE LOGIN CONTEXT */
#if 0
    {tp8, 8},    SEC_PUBKEY_RETRIEVE_KDC_KEY NO LONGER EXISTS
#endif
/*    {tp9, 9},  NOT DOING CHANGE LOGIN CONTEXT */
    {tp10, 10},
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
    strcpy((char *)passwd_buf, PK_PASSWD);

    dce_cf_get_cell_name( & cellname, & st );

    /* add a generic pk_user */
    if (system("./setup.ksh") != 0) {
        int test_no;
        mtet_infoline("FAILED: system(./setup.ksh)\n");
        for (test_no=0; tet_testlist[test_no++].testfunc != NULL;)
            tet_delete(test_no, "startup() has failed");
        return;
    }
    return;
}


/* Cleanup of data needed for each of the test cases, deleting pk_user
   account  */
static void pk_cleanup(void)
{

    if (system("./cleanup.ksh") != 0) {
        mtet_infoline("FAILED: system(./cleanup.ksh)\n");
    }
    return;
}


/*
======================================
SEC_PUBKEY_STORE
======================================
*/

/* TP1: N1. sec_pubkey_store():  check Unsuccessful call, with unsupported domain */

static void tp1()
{
    error_status_t    st;
    sec_pk_pubkey_t   in_publickey;
    byte              *pkdata_p = (byte *)"ABCDKEY";
    sec_pk_domain_t   sec_pk_domain_bad;
    unsigned32        kvno=1;

    in_publickey.len = 8;
    in_publickey.data = pkdata_p;

    mtet_infoline("(tp1) N1. sec_pubkey_store():  check Unsuccessful call, with unsupported domain.\n");

    st = sec_pubkey_store( user,
                           &sec_pk_domain_bad,  /* sec_pk_domain_t domain */
                           sec_pk_usage_digitalSignature, /* sec_pk_usage_flags_t key_usages */
                           &kvno,  /* unsigned32 kvno */
                           &in_publickey );
    CHECK_RESULT(true, "N1. SEC_PUBKEY: sec_pubkey_store", &st, sec_pk_e_domain_unsupported);

    tet_result(TET_PASS);

}
/*-----------------------------------------------------------------------*/


/* TP2: N2. sec_pubkey_store(): check unsuccessful call, with sec_pk_usage_nonRepudiation */

static void tp2()
{
    error_status_t    st;
    sec_pk_pubkey_t   in_publickey;
    sec_pk_pubkey_p_t in_pubkey_p=&in_publickey;
    byte              *pkdata_p = (byte *)"ABCDKEY";
    unsigned32        kvno=1;

    in_publickey.len = 8;
    in_publickey.data = pkdata_p;

    mtet_infoline("(tp2) N2. sec_pubkey_store():check unsuccessful call, with sec_pk_usage_nonRepudiation.\n");

    st = sec_pubkey_store( user,
                           &sec_pk_domain_dce_general, /* sec_pk_domain_t domain */
                           sec_pk_usage_nonRepudiation, 
                           /* not supported sec_pk_usage_flags_t key_usages */
                           &kvno,  /* unsigned32 kvno */
                           in_pubkey_p );
    CHECK_RESULT(true, "N2. SEC_PUBKEY: sec_pubkey_store", &st, sec_pk_e_usage_unsupported);

    tet_result(TET_PASS);

}
/*-----------------------------------------------------------------------*/


/* TP3: N3. sec_pubkey_store(): check unsuccessful call for sec_rgy_object_not_found */

static void tp3()
{
    error_status_t    st;
    sec_pk_pubkey_t   in_publickey;
    byte              *pkdata_p = (byte *)"ABCDKEY";
    unsigned32        kvno=1;

    in_publickey.len = 8;
    in_publickey.data = pkdata_p;

    mtet_infoline("(tp3) N3. sec_pubkey_store(): check Unsuccessful call for sec_rgy_object_not_found.\n");

    st = sec_pubkey_store( "unknown_user_that_should_not_exist",
                           &sec_pk_domain_dce_general,  /* sec_pk_domain_t domain */
                           sec_pk_usage_digitalSignature, 
                           &kvno,  /* unsigned32 kvno */
                           &in_publickey );
    CHECK_RESULT(true, "N3. SEC_PUBKEY: sec_pubkey_store", &st, sec_attr_rgy_obj_not_found );

    tet_result(TET_PASS);

}
/*-----------------------------------------------------------------------*/

/* TP4: N4. sec_pubkey_store(): check unsuccessful call with kvno = 0 */
/* TP4: N5. sec_pubkey_store(): check unsuccessful call with kvno = 256 */

static void tp4()
{
    error_status_t    st;
    sec_pk_pubkey_t   in_publickey;
    byte              *pkdata_p = (byte *)"ABCDKEY";
    unsigned32        kvno=0;

    in_publickey.len = 8;
    in_publickey.data = pkdata_p;

    mtet_infoline("(tp4) N4. sec_pubkey_store(): check Unsuccessful call with kvno = 0.\n");

    st = sec_pubkey_store( user,
                           &sec_pk_domain_dce_general,  /* sec_pk_domain_t domain */
                           sec_pk_usage_digitalSignature, 
                           &kvno,  /* unsigned32 kvno */
                           &in_publickey );
    CHECK_RESULT(true, "N4. SEC_PUBKEY: sec_pubkey_store", &st, sec_pk_e_key_vers_unsupported);

    mtet_infoline("(tp4) N5. sec_pubkey_store(): check Unsuccessful call with kvno = 256.\n");

    kvno=256;
    st = sec_pubkey_store( user,
                           &sec_pk_domain_dce_general,  /* sec_pk_domain_t domain */
                           sec_pk_usage_digitalSignature, 
                           &kvno,  /* unsigned32 kvno */
                           &in_publickey );
    CHECK_RESULT(true, "N4. SEC_PUBKEY: sec_pubkey_store", &st, sec_pk_e_key_vers_unsupported);

    tet_result(TET_PASS);

}
/*-----------------------------------------------------------------------*/

/* TP5: N5. sec_pubkey_store():  check unsuccessful call for sec_attr_unauthorized  not as cell_admin */

static void tp5()
{
    error_status_t    st;
    sec_pk_pubkey_t   in_publickey;
    byte              *pkdata_p = (byte *)"ABCDKEY";
    sec_passwd_rec_t  pwdrec;
    sec_login_handle_t login_h;
    boolean32         reset_passwd;
    sec_login_auth_src_t    auth_src;
    unsigned32        kvno=1;

    in_publickey.len = 8;
    in_publickey.data = pkdata_p;

    mtet_infoline("(tp5) N5. sec_pubkey_store(): check unsuccessful call for sec_rgy_not_authorized  not as cell_admin.\n");

    /* login as pk_user */
    if (sec_login_setup_identity(user, sec_login_no_flags, &login_h, &st)) {
        pwdrec.key.key_type = sec_passwd_plain;
        pwdrec.key.tagged_union.plain = passwd_buf;
        pwdrec.pepper = NULL;
        pwdrec.version_number = sec_passwd_c_version_none;

        if ( !sec_login_validate_identity(login_h, &pwdrec, &reset_passwd, &auth_src, &st)) {
            CHECK_STARTUP_RESULT(false,
                "SEC_LOGIN_DELEG setup: sec_login_validate_identity ", &st,
                error_status_ok);
        }

        if ( !sec_login_certify_identity(login_h, &st)) {
            CHECK_STARTUP_RESULT(false,
                "SEC_LOGIN_DELEG setup: sec_login_certify_identity ", &st,
                error_status_ok);
        }
  
        sec_login_set_context(login_h, &st);
        CHECK_RESULT(false, "SEC_PUBKEY_STORE: sec_login_set_context ", &st,
                error_status_ok);

        if (auth_src != sec_login_auth_src_network)
	   printf("SEC_PUBKEY_STORE: no network creds\n");
          
    } else {
        CHECK_RESULT(false, "SEC_PUBKEY_STORE: sec_login_setup_identity ", &st,
               error_status_ok);
    }

    st = sec_pubkey_store( "cell_admin",
                           &sec_pk_domain_dce_general,  /* sec_pk_domain_t domain */
                           sec_pk_usage_digitalSignature, /* sec_pk_usage_flags_t key_usages */
                           &kvno,  /* unsigned32 kvno */
                           &in_publickey );
    CHECK_RESULT(true, "N5. SEC_PUBKEY: sec_pubkey_store", &st, sec_attr_unauthorized);

    if (login_h != NULL) sec_login_purge_context(&login_h, &st);

    tet_result(TET_PASS);

}
/*
======================================
SEC_PUBKEY_RETRIEVE
======================================
*/

/* TP6: N1. sec_pubkey_retrieve():  check Unsuccessful call, with unsupported domain */

static void tp6()
{
    error_status_t    st;
    sec_pk_pubkey_t   in_publickey;
    sec_pk_pubkey_t   out_publickey;
    byte              *pkdata_p = (byte *)"ABCDKEY";
    sec_pk_domain_t   sec_pk_domain_bad;
    unsigned32        kvno=1;

    in_publickey.len = 8;
    in_publickey.data = pkdata_p;


    mtet_infoline("(tp6) N1. sec_pubkey_retrieve():  check Unsuccessful call, with unsupported domain.\n");

    /* first store the pubkey for retrieve operations */
    st = sec_pubkey_store( user,
                           &sec_pk_domain_dce_general,  /* sec_pk_domain_t domain */
                           sec_pk_usage_digitalSignature, /* sec_pk_usage_flags_t key_usages */
                           &kvno,  /* unsigned32 kvno */
                           &in_publickey );

    CHECK_RESULT(true, "SEC_PUBKEY: sec_pubkey_store setup", &st, error_status_ok);


    kvno=0;
    st = sec_pubkey_retrieve( user,
                              &sec_pk_domain_bad,  /* sec_pk_domain_t domain */
                              sec_pk_usage_digitalSignature, /* sec_pk_usage_flags_t key_usages */
                              &kvno,  /* unsigned32 kvno */
                              &out_publickey );
    CHECK_RESULT(true, "N1. SEC_PUBKEY: sec_pubkey_retrieve", &st, sec_pk_e_domain_unsupported);

    mtet_infoline("(tp6) N2. sec_pubkey_retrieve():check unsuccessful call, with sec_pk_usage_nonRepudiation.\n");

    kvno=0;
    st = sec_pubkey_retrieve( user,
                              &sec_pk_domain_dce_general, /* sec_pk_domain_t domain */
                              sec_pk_usage_nonRepudiation,
                              /* not supported sec_pk_usage_flags_t key_usages */
                              &kvno,  /* unsigned32 kvno */
                              &out_publickey );
    CHECK_RESULT(true, "N2. SEC_PUBKEY: sec_pubkey_retrieve", &st, sec_pk_e_usage_unsupported);


/* TP6: N3. sec_pubkey_retrieve(): check unsuccessful call for sec_rgy_object_not_found */

    mtet_infoline("(tp6) N3. sec_pubkey_retrieve(): check Unsuccessful call for sec_rgy_object_not_found.\n");

    kvno=0;
    st = sec_pubkey_retrieve( "unknown_user_that_should_not_exist",
                              &sec_pk_domain_dce_general,  /* sec_pk_domain_t domain */
                              sec_pk_usage_nonRepudiation, 
                              &kvno,  /* unsigned32 kvno */
                              &out_publickey );
    CHECK_RESULT(true, "N3. SEC_PUBKEY: sec_pubkey_retrieve", &st, sec_pk_e_usage_unsupported);

/* TP6: N4. sec_pubkey_retrieve(): check unsuccessful call with kvno NOT 0 */

    mtet_infoline("(tp6) N4. sec_pubkey_retrieve(): check Unsuccessful call with kvno NOT match.\n");

    kvno=2;
    st = sec_pubkey_retrieve( user,
                              &sec_pk_domain_dce_general,  /* sec_pk_domain_t domain */
                              sec_pk_usage_digitalSignature, 
                              &kvno,  /* unsigned32 kvno */
                              &out_publickey );
    CHECK_RESULT(true, "N4. SEC_PUBKEY: sec_pubkey_retrieve", &st, sec_pk_e_key_vers_unavailable);

    tet_result(TET_PASS);
}

/* TP7: N5. sec_pubkey_retrieve():  check unsuccessful call for sec_attr_unauthorized  not as cell_admin */

static void tp7()
{
    error_status_t    st;
    sec_pk_pubkey_t   in_publickey;
    sec_pk_pubkey_p_t in_pubkey_p=&in_publickey;
    sec_pk_pubkey_t   out_publickey;
    byte              *pkdata_p = (byte *)"ABCDKEY";
    sec_passwd_rec_t  pwdrec;
    sec_login_handle_t login_h;
    boolean32         reset_passwd;
    sec_login_auth_src_t    auth_src;
    unsigned32        kvno=1;

    in_publickey.len = 8;
    in_publickey.data = pkdata_p;


    mtet_infoline("(tp7) N5. sec_pubkey_retrieve(): check unsuccessful call for sec_rgy_not_authorized  not as cell_admin.\n");

    /* first store the pubkey for retrieve operations */
    st = sec_pubkey_store( "cell_admin",
                           &sec_pk_domain_dce_general,  /* sec_pk_domain_t domain */
                           sec_pk_usage_digitalSignature, /* sec_pk_usage_flags_t key_usages */
                           &kvno,  /* unsigned32 kvno */
                           in_pubkey_p );
    CHECK_RESULT(true, "N1. SEC_PUBKEY: sec_pubkey_store setup", &st, error_status_ok);

    /* Set up the pk_user login context */
    if (sec_login_setup_identity(user, sec_login_no_flags, &login_h, &st)) {
        pwdrec.key.key_type = sec_passwd_plain;
        pwdrec.key.tagged_union.plain = passwd_buf;
        pwdrec.pepper = NULL;
        pwdrec.version_number = sec_passwd_c_version_none;

        if (! sec_login_validate_identity(login_h, &pwdrec, &reset_passwd,
            &auth_src, &st)) {
            CHECK_RESULT(false,
                "SEC_PUBKEY_RETRIEVE: sec_login_validate_identity ", &st,
                error_status_ok);
        }
    }


    kvno=0;
    st = sec_pubkey_retrieve( user,
                           &sec_pk_domain_dce_general,  /* sec_pk_domain_t domain */
                           sec_pk_usage_digitalSignature, /* sec_pk_usage_flags_t key_usages */
                           &kvno,  /* unsigned32 kvno */
                           &out_publickey );
    CHECK_RESULT(true, "N4. SEC_PUBKEY: sec_pubkey_retrieve", &st, sec_pk_e_domain_unsupported);

    if (login_h != NULL) sec_login_purge_context(&login_h, &st);

    tet_result(TET_PASS);

}
/*-----------------------------------------------------------------------*/


/*
======================================
SEC_PUBKEY_RETRIEVE with domain sec_pk_domain_kdc_cache and kdc_cache_refresh
======================================
*/

/*-----------------------------------------------------------------------*/

/* TP10: N1. sec_pubkey_store(): check unsuccessful call, with domain sec_pk_domain_kdc_cache  */
static void tp10()
{
    error_status_t    st;
    sec_pk_pubkey_t   in_publickey;
    sec_pk_pubkey_t   out_publickey;
    byte              *pkdata_p = (byte *)"ABCDKEY";
    unsigned32        kvno;

    in_publickey.len = 8;
    in_publickey.data = pkdata_p;


    mtet_infoline("(tp10) N1. sec_pubkey_store() check unsuccessful call, with domain sec_pk_domain_kdc_cache");

    /* N1. sec_pubkey_store() check unsuccessful call, with domain sec_pk_domain_kdc_cache  */
    kvno=1;
    st = sec_pubkey_store( user,
                           &sec_pk_domain_kdc_cache,  /* sec_pk_domain_t domain */
                           sec_pk_usage_digitalSignature, /* sec_pk_usage_flags_t key_usages */
                           &kvno,  /* unsigned32 kvno */
                           &in_publickey );
    CHECK_RESULT(true, "N1. SEC_PUBKEY: sec_pubkey_store", &st, sec_pk_e_domain_unsupported);

    tet_result(TET_PASS);
}
/*-----------------------------------------------------------------------*/

