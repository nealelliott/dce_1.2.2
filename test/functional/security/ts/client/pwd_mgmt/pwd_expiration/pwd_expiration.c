/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: pwd_expiration.c,v $
 * Revision 1.1.4.2  1996/02/17  23:11:31  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:33:15  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:52:53  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/07/28  16:03 UTC  mullan_s
 * 	Merge
 * 
 * 	HP revision /main/HPDCE02/mullan_chfts15637/1  1995/07/28  15:59 UTC  mullan_s
 * 	Bind to master for setting policy.
 * 
 * 	HP revision /main/HPDCE02/1  1995/04/12  20:40 UTC  mullan_s
 * 	HPDCE02 branch
 * 
 * 	HP revision /main/mullan_mothra_beta_fixes/1  1995/04/12  20:29 UTC  mullan_s
 * 	Allocate space for password.
 * 	[1995/12/11  19:27:00  root]
 * 
 * Revision 1.1.2.2  1994/10/06  20:34:12  agd
 * 	expand copyright
 * 	[1994/10/06  14:29:31  agd]
 * 
 * Revision 1.1.2.1  1994/08/04  16:15:35  mdf
 * 	Initial checkin
 * 	[1994/08/03  19:20:39  mdf]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1994
 * Unpublished work. All Rights Reserved.
 *
 * Provide the tests needed to verify that the local 
 * sec_pwd_mgmt_strength_chk_prvcy function is functioning correctly.
 *
 * This test case makes the following assumptions :
 *
 *    1) Machine is a DCE client
 *    2) The tester is logged in as 'cell_admin' and the password is '-dce-'.
 */

#include <tet_api.h>

#include <dce/dce_error.h>
#include <dce/policy.h>
#include <dce/sec_login.h>
#include <krb5/error_tables/krb5_err.h>
#include <macros.h>
#include <time.h>

static void pwd_expiration_startup(void);
static void pwd_expiration_cleanup(void);
static void tp1(void);

void (*tet_startup)() = pwd_expiration_startup;
void (*tet_cleanup)() = pwd_expiration_cleanup;

struct tet_testlist tet_testlist[] = {
    {tp1, 1},
    {NULL, 0}
};

/* global variables */
sec_rgy_plcy_t        policy_data;
sec_login_handle_t    login_h;
unsigned char         unexpected[dce_c_error_string_len];
unsigned char         expected[dce_c_error_string_len];
int                   dce_status;
sec_rgy_handle_t      write_context;
unsigned_char_t       *sec_master_site_name;

#define CHECK_STARTUP_RESULT(T, FUNC, RETURNED_ST_P, EXPECTED_ST)               \
    if (STATUS_EQUAL(RETURNED_ST_P, EXPECTED_ST)) {                             \
        if ((T)) mtet_infoline("PASSED %s()\n", FUNC);                          \
    } else {                                                                    \
        int test_no;                                                            \
        dce_error_inq_text(*(RETURNED_ST_P), unexpected, &dce_status);          \
        dce_error_inq_text(EXPECTED_ST, expected, &dce_status);                 \
        mtet_infoline("FAILED %s()\nresult: \t%s\nexpected: \t%s\n\n",          \
                FUNC, unexpected, expected);                                    \
        for (test_no=0; tet_testlist[test_no++].testfunc != NULL;)              \
            tet_delete(test_no,  "startup() has failed");                       \
        return;                                                                 \
    }

#define CHECK_CLEANUP_RESULT(T, FUNC, RETURNED_ST_P, EXPECTED_ST)               \
    if (STATUS_EQUAL(RETURNED_ST_P, EXPECTED_ST)) {                             \
        if ((T)) mtet_infoline("PASSED %s()\n", FUNC);                          \
    } else {                                                                    \
        int test_no;                                                            \
        dce_error_inq_text(*(RETURNED_ST_P), unexpected, &dce_status);          \
        dce_error_inq_text(EXPECTED_ST, expected, &dce_status);                 \
        mtet_infoline("WARNING %s()\nresult: \t%s\nexpected: \t%s\n\n",          \
                FUNC, unexpected, expected);                                    \
        return;                                                                 \
    }

#define CHECK_RESULT(T, MSG, RETURNED_ST_P, EXPECTED_ST)                        \
    if (STATUS_EQUAL(RETURNED_ST_P, EXPECTED_ST)) {                             \
        if ((T)) mtet_infoline("PASSED %s\n", MSG);                             \
    } else {                                                                    \
        dce_error_inq_text(*(RETURNED_ST_P), unexpected, &dce_status);          \
        dce_error_inq_text(EXPECTED_ST, expected, &dce_status);                 \
        mtet_infoline("FAILED %s\nresult: \t%s\nexpected: \t%s\n\n",            \
                MSG, unexpected, expected);                                     \
        tet_result(TET_FAIL);                                                   \
        return;                                                                 \
    }

/*
 * Startup will retrieve and save the current registry password policy. This
 * will be restored in the cleanup. It then sets the expiration time to now,
 * so that the password will be expired by the time tp1 is executed.
 */
static void pwd_expiration_startup(void)
{
    error_status_t    status;
    sec_rgy_name_t    null_name;

    mtet_infoline("Startup for password expiration tests\n");
    CLEAR_STATUS(&status);
    sec_rgy_site_open_update(sec_master_site_name, &write_context, &status);
    CHECK_STARTUP_RESULT(false, "pwd_expiration startup: sec_rgy_site_open_update ",
        &status, error_status_ok);
    null_name[0] = '\0';

    /* retrieve registry policy */
    sec_rgy_plcy_get_info(write_context, null_name, &policy_data, &status);
    CHECK_STARTUP_RESULT(false, "pwd_expiration startup: sec_rgy_plcy_get_info ",
        &status, error_status_ok);
}

/*
 * tp1 : sets a password expiration date in the past and checks to see
 * that cell_admin cannot log in.
 */
static void tp1(void)
{
    error_status_t        status;
    sec_rgy_plcy_t        plcy_data;
    time_t                now;
    unsigned_char_p_t     user = (unsigned_char_p_t)"cell_admin";
    sec_passwd_rec_t      prec;
    boolean32             reset_passwd;
    sec_login_auth_src_t  auth_src;
    sec_rgy_name_t        null_name;
    sec_passwd_str_t	  passwd_buf = "-dce-";

    mtet_infoline("pwd_expiration, test purpose 1, login attempt using expired password\n");
    CLEAR_STATUS(&status);
    null_name[0] = '\0';

    /* get current time */
    time(&now);

    /* set up policy data */
    plcy_data.passwd_min_len = policy_data.passwd_min_len;
    plcy_data.passwd_lifetime = policy_data.passwd_lifetime;
    plcy_data.passwd_exp_date = now - 120;
    plcy_data.acct_lifespan = policy_data.acct_lifespan;
    plcy_data.passwd_flags = policy_data.passwd_flags;

    /* set registry policy */
    sec_rgy_plcy_set_info(write_context, null_name, &plcy_data, &status);
    CHECK_RESULT(false, "tp1: sec_rgy_plcy_set_info", &status, error_status_ok);

    /* set up and validate cell_admin login context */
    if (sec_login_setup_identity(user, sec_login_no_flags, &login_h, &status)) {
        prec.key.key_type = sec_passwd_plain;
        prec.key.tagged_union.plain = passwd_buf;
        prec.pepper = NULL;
        prec.version_number = sec_passwd_c_version_none;

        if (! sec_login_validate_identity(login_h, &prec, &reset_passwd,
            &auth_src, &status)) {
            CHECK_RESULT(true, "password expiration: login with expired password",
                &status, KRB5KDC_ERR_KEY_EXP);
        }
    } else {
        CHECK_RESULT(false, "sec_login_validate_identity", &status, error_status_ok);
    }

    tet_result(TET_PASS);
}
    

/*
 * Cleanup will restore the saved registry password policy.
 */
static void pwd_expiration_cleanup(void)
{
    error_status_t    status;
    sec_rgy_name_t    null_name;

    mtet_infoline("Cleanup for password expiration tests\n");
    null_name[0] = '\0';

    /* restore registry policy */
    sec_rgy_plcy_set_info(write_context, null_name, &policy_data, &status);
    CHECK_CLEANUP_RESULT(false, "pwd_expiration cleanup: sec_rgy_plcy_set_info ",
        &status, error_status_ok);

    if (login_h != NULL) sec_login_purge_context(&login_h, &status);
    sec_rgy_site_close(write_context, &status);
}
