/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_u2u.c,v $
 * Revision 1.1.2.1  1996/06/04  22:11:23  arvind
 * 	Merge from mb_u2u
 * 	[1996/05/09  19:36 UTC  burati  /main/DCE_1.2/1]
 *
 * 	u2u server registration test
 * 	[1996/05/09  19:26 UTC  burati  /main/mb_u2u/1]
 *
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1996
 * Unpublished work. All Rights Reserved.
 */

#include <stdio.h>
#include <errno.h>
#include <macros.h>
#include <tet_api.h>
#include <ts-common.h>

#include <dce/binding.h>
#include <dce/dce_error.h>
#include <dce/sec_attr_tools.h>

#include <dce/rgybase.h>
#include <dce/uuid.h>
#include <dce/sec_login.h>
#include <dce/sec_cred.h>

#include <sec_def_flags.h>
#include <dce/pgo.h>
#include <dce/acct.h>


static void tp1(void), tp2(void);
static void u2u_startup(void), u2u_cleanup(void);

/* Simple existing account that shouldn't be allowed to be a u2u server */
#define U2U_BAD_USER "root"
#define U2U_BAD_PASSWD "-dce-"

/* Simple existing account that should be allowed to be a u2u server */
#define U2U_USER "u2usvr"
#define U2U_PASSWD "u2usvr"

/* Cell_admin info */
#define CA_USER "cell_admin"
#define CA_PASSWD "-dce-"

unsigned char unexpected[dce_c_error_string_len];
unsigned char expected[dce_c_error_string_len];
sec_login_handle_t login_h, ca_lh, u2u_lh;
unsigned_char_p_t  baduser = (unsigned_char_p_t)U2U_BAD_USER;
unsigned_char_p_t  user = (unsigned_char_p_t)U2U_USER;
unsigned_char_p_t  causer = (unsigned_char_p_t)CA_USER;
sec_passwd_str_t   passwd_buf, cbuf, nbuf;
int                dce_status;
sec_login_handle_t ilh = NULL;
sec_rgy_handle_t   write_context = NULL;

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
	return; \
    }

#undef CHECK_STARTUP_RESULT
#define CHECK_STARTUP_RESULT(T, FUNC, RETURNED_ST_P, EXPECTED_ST) \
    if (STATUS_EQUAL(RETURNED_ST_P, EXPECTED_ST)) { \
	if ((T)) mtet_infoline("PASSED %s()\n", FUNC); \
    } else { \
	int test_no; \
	dce_error_inq_text(*(RETURNED_ST_P), unexpected, &dce_status); \
	dce_error_inq_text(EXPECTED_ST, expected, &dce_status); \
	mtet_infoline("FAILED %s()\nresult: \t%s\nexpected: \t%s\n\n", \
		FUNC, unexpected, expected); \
	for (test_no=0; tet_testlist[test_no++].testfunc != NULL;) \
	    tet_delete(test_no,  "startup() has failed"); \
	return; \
    }

/* List of tests to run that TET will be looking for */
struct tet_testlist tet_testlist[] = {
    {tp1, 1}, /* rpc_server_register_auth_ident() negative */
    {tp2, 2}, /* ?... */
    {NULL, 0}
};

void (*tet_startup)() = u2u_startup;
void (*tet_cleanup)() = u2u_cleanup;

void add_u2u_acct(void)
{
    error_status_t st;
    sec_rgy_pgo_item_t pgo;
    sec_rgy_login_name_t lname;
    sec_rgy_acct_user_t  upart;
    sec_rgy_acct_admin_t apart;
    sec_passwd_rec_t     ckey, nkey;
    sec_passwd_version_t kvno;
    sec_rgy_acct_key_t   kp = sec_rgy_acct_key_person;

    uuid_create_nil(&pgo.id, &st);
    pgo.unix_num = sec_rgy_uxid_unknown;
    pgo.quota = pgo.flags = 0;
    pgo.fullname[0] = '\0';
    sec_rgy_pgo_add(write_context, sec_rgy_domain_person,
        (unsigned_char_p_t) U2U_USER, &pgo, &st);
    CHECK_STARTUP_RESULT(false, "SEC_U2U: sec_rgy_pgo_add ", &st,
                         error_status_ok);
    sec_rgy_pgo_add_member(write_context, sec_rgy_domain_group,
        (unsigned_char_p_t) "none", (unsigned_char_p_t) U2U_USER, &st);
    CHECK_STARTUP_RESULT(false, "SEC_U2U: sec_rgy_pgo_add_member ", &st,
                         error_status_ok);
    sec_rgy_pgo_add_member(write_context, sec_rgy_domain_org,
        (unsigned_char_p_t) "none", (unsigned_char_p_t) U2U_USER, &st);
    CHECK_STARTUP_RESULT(false, "SEC_U2U: sec_rgy_pgo_add_member ", &st,
                         error_status_ok);

    strcpy(lname.pname, U2U_USER);
    strcpy(lname.gname, "none");
    strcpy(lname.oname, "none");
    upart.gecos[0] = upart.homedir[0] = upart.shell[0] = '\0';
    upart.passwd_version_number = 0;
    upart.flags = sec_rgy_acct_user_passwd_valid;
    apart.good_since_date = apart.expiration_date = 0;
    apart.flags = sec_rgy_acct_admin_valid | sec_rgy_acct_admin_server |
	sec_rgy_acct_admin_client;
    apart.authentication_flags = sec_rgy_acct_auth_tgt |
	sec_rgy_acct_auth_dup_skey | sec_rgy_acct_auth_user_to_user;
    strcpy(cbuf, "-dce-");
    ckey.key.key_type = sec_passwd_plain;
    ckey.key.tagged_union.plain = cbuf;
    ckey.pepper = NULL;
    ckey.version_number = sec_passwd_c_version_none;
    strcpy(nbuf, U2U_PASSWD);
    nkey.key.key_type = sec_passwd_plain;
    nkey.key.tagged_union.plain = nbuf;
    nkey.pepper = NULL;
    nkey.version_number = sec_passwd_c_version_none;
    sec_rgy_acct_add(write_context, &lname, &kp, &upart,
        &apart, &ckey, &nkey, sec_passwd_des, &kvno, &st);
    CHECK_STARTUP_RESULT(false, "SEC_U2U: sec_rgy_acct_add ", &st,
                         error_status_ok);
}


/* Initialization of data needed for each of the test cases */
static void u2u_startup(void)
{
    error_status_t          st;
    sec_passwd_rec_t        prec;
    sec_login_auth_src_t    auth_src;
    boolean32               reset_passwd;
    unsigned_char_t         *sec_master_site_name = (unsigned_char_p_t) "/.:";
    sec_rgy_bind_auth_info_t a_i;

    tet_infoline("sec_u2u test startup");
    CLEAR_STATUS(&st);

    /* Set up the initial login context for cell_admin */
    strcpy((char *)passwd_buf, CA_PASSWD);
    if (sec_login_setup_identity((unsigned_char_p_t)causer, sec_login_no_flags,
            &ca_lh, &st)) {
        prec.key.key_type = sec_passwd_plain;
        prec.key.tagged_union.plain = passwd_buf;
        prec.pepper = NULL;
        prec.version_number = sec_passwd_c_version_none;

        if (! sec_login_validate_identity(ca_lh, &prec, &reset_passwd,
            &auth_src, &st)) {
            CHECK_STARTUP_RESULT(false,
                "SEC_U2U setup: sec_login_validate_identity cell_admin", &st,
                error_status_ok);
        }
    } else {
        CHECK_STARTUP_RESULT(false,
            "SEC_U2U setup: sec_login_setup_identity cell_admin", &st,
            error_status_ok);
    }

    a_i.info_type = sec_rgy_bind_auth_dce;
    a_i.tagged_union.dce_info.authn_level = rpc_c_authn_level_pkt_integrity;
    a_i.tagged_union.dce_info.authn_svc = rpc_c_authn_dce_private;
    a_i.tagged_union.dce_info.authz_svc = rpc_c_authz_dce;
    a_i.tagged_union.dce_info.identity  = ca_lh;
    sec_rgy_site_bind_update(sec_master_site_name, &a_i, &write_context, &st);
    CHECK_STARTUP_RESULT(false, "SEC_U2U: sec_rgy_site_bind_update ", &st,
                         error_status_ok);
    
    add_u2u_acct();

    /* Set up the initial login context for non u2u acct*/
    strcpy((char *)passwd_buf, U2U_BAD_PASSWD);
    if (sec_login_setup_identity(baduser, sec_login_no_flags, &login_h, &st)) {
        prec.key.key_type = sec_passwd_plain;
        prec.key.tagged_union.plain = passwd_buf;
        prec.pepper = NULL;
        prec.version_number = sec_passwd_c_version_none;

        if (! sec_login_validate_identity(login_h, &prec, &reset_passwd,
            &auth_src, &st)) {
            CHECK_STARTUP_RESULT(false,
                "SEC_U2U setup: sec_login_validate_identity ", &st,
                error_status_ok);
        }
    } else {
        CHECK_STARTUP_RESULT(false,
            "SEC_U2U setup: sec_login_setup_identity ", &st,
            error_status_ok);
    }

    /* Set up the initial login context for u2u acct*/
    strcpy((char *)passwd_buf, U2U_PASSWD);
    if (sec_login_setup_identity(user, sec_login_no_flags, &u2u_lh, &st)) {
        prec.key.key_type = sec_passwd_plain;
        prec.key.tagged_union.plain = passwd_buf;
        prec.pepper = NULL;
        prec.version_number = sec_passwd_c_version_none;

        if (! sec_login_validate_identity(u2u_lh, &prec, &reset_passwd,
            &auth_src, &st)) {
            CHECK_STARTUP_RESULT(false,
                "SEC_U2U setup: sec_login_validate_identity ", &st,
                error_status_ok);
        }
    } else {
        CHECK_STARTUP_RESULT(false,
            "SEC_U2U setup: sec_login_setup_identity ", &st,
            error_status_ok);
    }
}


/* Cleanup of data needed for each of the test cases */
static void u2u_cleanup(void)
{
    error_status_t st;
    if (write_context) {
        sec_rgy_pgo_delete(write_context, sec_rgy_domain_person,
            (unsigned_char_p_t) U2U_USER, &st);
        CHECK_STARTUP_RESULT(false,
            "SEC_U2U cleanup: sec_rgy_pgo_delete ", &st,
            error_status_ok);
        sec_rgy_site_close(write_context, &st);
    }
    if (login_h != NULL) {
        sec_login_purge_context(&login_h, &st);
        CHECK_STARTUP_RESULT(false,"SEC_U2U cleanup: sec_login_purge_context ",
            &st, error_status_ok);
    }
}


/* Test purpose 1: Test */
static void tp1(void)
{
    error_status_t              st;

    tet_infoline("test case (tp1)");

    rpc_server_register_auth_ident(baduser, rpc_c_authn_default, login_h, &st);
    CHECK_RESULT(true, "SEC_U2U: rpc_server_register_auth_ident negative test",
        &st, sec_s_user_to_user_disabled);

    rpc_server_register_auth_ident((unsigned_char_p_t) U2U_USER,
        rpc_c_authn_default, u2u_lh, &st);
    CHECK_RESULT(true, "SEC_U2U: rpc_server_register_auth_ident positive test",
        &st, error_status_ok);

    tet_result(TET_PASS);
}


/* Test purpose 2: Test success of local side of attribute setup/lookup */
static void tp2()
{
    tet_result(TET_PASS);
}
