/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: pwd_gen-tc.c,v $
 * Revision 1.1.4.2  1996/02/17  23:11:37  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:33:16  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:52:58  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/01/23  13:52 UTC  mullan_s
 * 
 * 	HP revision /main/mullan_rep_fix/1  1995/01/23  13:48 UTC  mullan_s
 * 	Bind to update site for setup operations.
 * 	[1995/12/11  19:27:02  root]
 * 
 * Revision 1.1.2.3  1994/10/06  20:34:40  agd
 * 	expand copyright
 * 	[1994/10/06  14:29:40  agd]
 * 
 * Revision 1.1.2.2  1994/08/08  14:23:51  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/08/04  20:04:27  mdf]
 * 
 * Revision 1.1.2.1  1994/08/04  16:15:39  mdf
 * 	Initial checkin
 * 	[1994/08/03  19:21:12  mdf]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1994
 * Unpublished work. All Rights Reserved.
 *
 * Provide the tests needed to verify that the sec_pwd_mgmt & 
 * rsec_pwd_mgmt APIs are working correctly for password generation.
 *
 * This test case makes the following assumptions :
 *
 *    1) Machine is a DCE client
 *    2) The Password Strength Server is running and exporting the
 *       rsec_pwd_mgmt_* operations.
 *    3) The Password Strength Server running is the sample server
 *       (pwd_mgmtd) provided with DCE 1.1
 *    4) An account named "pwd_gen" has been created with password
 *       "pwd_gen".
 *    5) The tester is dce logged in as 'cell_admin'.
 *    6) The PWD_MGMTD_STRING_BINDING TET configuration variable
 *       has been set correctly.
 */

#include <tet_api.h>

#include <stdio.h>
#include <stdlib.h>

#include <macros.h>

#include <dce/acct.h>
#include <dce/binding.h>
#include <dce/passwd.h>
#include <dce/pgo.h>
#include <dce/rgynbase.h>
#include <dce/rpc.h>
#include <dce/dce_error.h>
#include <dce/sec_login.h>
#include <dce/sec_pwd_mgmt.h>
#include <dce/sec_rgy_attr.h>

enum     pwd_val_type { none, user_select, user_can_select,
                        generation_required };

static void pwd_gen_startup(void);
static void pwd_gen_cleanup(void);
static void tp1(void);
static void tp2(void);

void (*tet_startup)() = pwd_gen_startup;
void (*tet_cleanup)() = pwd_gen_cleanup;

struct tet_testlist tet_testlist[] = {
    {tp1, 1},
    {tp2, 2},
    {NULL, 0}
};

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

#define CHECK_CLEANUP_RESULT(T, FUNC, RETURNED_ST_P, EXPECTED_ST)               \
    if (STATUS_EQUAL(RETURNED_ST_P, EXPECTED_ST)) {                             \
        if ((T)) mtet_infoline("PASSED %s()\n", FUNC);                          \
    } else {                                                                    \
        dce_error_inq_text(*(RETURNED_ST_P), unexpected, &dce_status);          \
        dce_error_inq_text(EXPECTED_ST, expected, &dce_status);                 \
        mtet_infoline("WARNING %s()\nresult: \t%s\nexpected: \t%s\n\n",          \
                FUNC, unexpected, expected);                                    \
        return;                                                                 \
    }

/* Global variables */
sec_pwd_mgmt_handle_t           pwd_mgmt_h;
unsigned char                   unexpected[dce_c_error_string_len];
unsigned char                   expected[dce_c_error_string_len];
int                             dce_status;
sec_rgy_handle_t                context;
sec_rgy_login_name_t            login_name = { "pwd_gen", "none", "none" };
sec_attr_schema_entry_t         schema_info;
sec_rgy_name_t                  test_pname = "pwd_gen";

static void pwd_gen_startup(void)
{
    error_status_t              st;
    sec_rgy_bind_auth_info_t    auth_info;
    unsigned32                  space_avail = 0,
                                num_returned,
                                num_left;
    signed32                    failure_index;
    sec_attr_t                  attrs[1];
    sec_attr_t                  out_attrs[1];
    sec_login_handle_t          your_lc;
    unsigned_char_t		*sec_master_site_name = NULL;

    mtet_infoline("Startup for password generation tests\n");
    CLEAR_STATUS(&st);

    if (system("./setup.ksh") != 0) {
        int test_no;
        mtet_infoline("FAILED: system(./setup.ksh)\n");
        for (test_no=0; tet_testlist[test_no++].testfunc != NULL;)
            tet_delete(test_no, "startup() has failed");
        return;
    }

    /* get current context */
    sec_login_get_current_context(&your_lc, &st);
    CHECK_STARTUP_RESULT(false, "password generation startup: sec_login_get_current_context ",
        &st, error_status_ok);

    /* get registry server handle */
    auth_info.info_type = sec_rgy_bind_auth_dce;
    auth_info.tagged_union.dce_info.authn_level = rpc_c_protect_level_pkt_integ;
    auth_info.tagged_union.dce_info.authn_svc = rpc_c_authn_dce_secret;
    auth_info.tagged_union.dce_info.authz_svc = rpc_c_authz_dce;
    auth_info.tagged_union.dce_info.identity  = your_lc;
    sec_rgy_site_bind_update(sec_master_site_name, &auth_info, &context, &st);
    CHECK_STARTUP_RESULT(false, "password generation startup: sec_rgy_site_bind_update ",
        &st, error_status_ok);

    sec_rgy_attr_sch_lookup_by_name(context, "", "pwd_val_type", &schema_info, &st);
    CHECK_STARTUP_RESULT(false, "password generation startup: sec_rgy_attr_sch_lookup_by_name ",
        &st, error_status_ok);

    /* attach pwd_val_type ERA */
    attrs[0].attr_id = schema_info.attr_id;
    attrs[0].attr_value.attr_encoding = sec_attr_enc_integer;
    attrs[0].attr_value.tagged_union.signed_int = generation_required;
    sec_rgy_attr_update(context, sec_rgy_domain_person, test_pname, 1, space_avail,
        attrs, &num_returned, out_attrs, &num_left, &failure_index, &st);
    CHECK_STARTUP_RESULT(false, "password generation startup: sec_rgy_attr_update ",
        &st, error_status_ok);

    /* setup handle */
    sec_pwd_mgmt_setup(&pwd_mgmt_h, context, login_name, your_lc, NULL, &st);
    CHECK_STARTUP_RESULT(false, "password generation startup: sec_pwd_mgmt_setup ",
        &st, error_status_ok);

}

/*
 * the purpose of this test is to generate a password and strength check it
 */
static void tp1(void)
{
    error_status_t        st;
    sec_passwd_rec_t      gen_pwds[1],
                          caller_key;
    unsigned32            num_pwds = 1,
                          num_returned;
    sec_passwd_version_t  new_key_version;

    mtet_infoline("test purpose 1, generate and strength check password\n");

    sec_pwd_mgmt_gen_pwd(pwd_mgmt_h, num_pwds, &num_returned, gen_pwds, &st);
    CHECK_RESULT(false, "tp1: sec_pwd_mgmt_gen_pwd", &st, error_status_ok);

    caller_key.key.key_type = sec_passwd_plain;
    caller_key.key.tagged_union.plain = (unsigned_char_p_t)"-dce-";
    caller_key.pepper = NULL;
    caller_key.version_number = sec_passwd_c_version_none;
    sec_rgy_acct_passwd(context, &login_name, &caller_key, &gen_pwds[0], sec_passwd_des,
        &new_key_version, &st);
    CHECK_RESULT(false, "tp1: sec_rgy_acct_passwd", &st, error_status_ok);

    mtet_infoline("PASSED test purpose 1\n");
    tet_result(TET_PASS);
}

/*
 * the purpose of this test is to generate a password and send a different password
 * to the registry server.
 */
static void tp2(void)
{
    error_status_t        st;
    sec_passwd_rec_t      gen_pwds[1],
                          caller_key,
                          bogus_key;
    unsigned32            num_pwds = 1,
                          num_returned;
    sec_passwd_version_t  new_key_version;

    mtet_infoline("test purpose 2, generate and strength check bogus password\n");

    sec_pwd_mgmt_gen_pwd(pwd_mgmt_h, num_pwds, &num_returned, gen_pwds, &st);
    CHECK_RESULT(false, "tp2: sec_pwd_mgmt_gen_pwd", &st, error_status_ok);

    caller_key.key.key_type = bogus_key.key.key_type = sec_passwd_plain;
    caller_key.key.tagged_union.plain = (unsigned_char_p_t)"-dce-";
    bogus_key.key.tagged_union.plain = (unsigned_char_p_t)"bogus1";
    caller_key.pepper = bogus_key.pepper = NULL;
    caller_key.version_number = bogus_key.version_number = sec_passwd_c_version_none;
    sec_rgy_acct_passwd(context, &login_name, &caller_key, &bogus_key, sec_passwd_des,
        &new_key_version, &st);
    CHECK_RESULT(false, "tp2: sec_rgy_acct_passwd", &st, sec_pwd_mgmt_str_check_failed);

    mtet_infoline("PASSED test purpose 2\n");
    tet_result(TET_PASS);
}

static void pwd_gen_cleanup(void)
{
    error_status_t          lst;

    sec_pwd_mgmt_free_handle(&pwd_mgmt_h, &lst);

    /* delete pwd_gen account */
    sec_rgy_acct_delete(context, &login_name, &lst);
    CHECK_CLEANUP_RESULT(false, "password generation cleanup: sec_rgy_acct_delete", &lst,
        error_status_ok);

    /* delete pwd_gen principal */
    sec_rgy_pgo_delete(context, sec_rgy_domain_person, test_pname, &lst);
    CHECK_CLEANUP_RESULT(false, "password generation cleanup: sec_rgy_pgo_delete", &lst,
        error_status_ok);
}
