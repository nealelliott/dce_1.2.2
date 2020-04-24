/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, I
nc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems
 * Burlington, MA, USA
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: bin_compat-tc1.c,v $
 * Revision 1.1.2.1  1996/06/05  14:18:56  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  13:47:07  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * Binary Compatibility Tests
 *
 * The purpose of this test is to establish a login context
 * and store it on disk, export it to a 1.0 archived binary
 * which will import it and refresh it, and check to see
 * if the 1.0 binary did not destroy any 1.1 specific fields.
 *
 * Assumes that the binary compatibility switch is on.
 */
#include <tet_api.h>
#include <ts-common.h>

#include <string.h>

#include <dce/dce_error.h>
#include <dce/keymgmt.h>
#include <dce/sec_login.h>

static void tc1_startup(void);
static void tc1_cleanup(void);
static void tp1(void);
static void child_1_0(void);
extern  char    **environ;

void (*tet_startup)() = tc1_startup;
void (*tet_cleanup)() = tc1_cleanup;

struct tet_testlist tet_testlist[] = {
    {tp1, 1},
    {NULL, 0}
};

#define TEST_USER "root"
#define TEST_PASSWD "-dce-"
#define LC_LEN 512
#define KEY_TAB "./bc_keytab"

unsigned_char_p_t  user = (unsigned_char_p_t)TEST_USER;
unsigned char unexpected[dce_c_error_string_len];
unsigned char expected[dce_c_error_string_len];
int                dce_status;
idl_byte           lc_buf[LC_LEN];

static void tc1_cleanup(void)
{
    error_status_t st;

    sec_key_mgmt_delete_key(rpc_c_authn_dce_secret, (void *) KEY_TAB, user, 1, &st);
    sec_key_mgmt_delete_key(rpc_c_authn_dce_secret, (void *) KEY_TAB, user, 2, &st);
    (void) unlink((char *) KEY_TAB);
}

static void tc1_startup(void)
{
    sec_passwd_rec_t	prec;
    error_status_t	st;
    boolean32           reset_passwd;
    sec_login_handle_t  login_h, ilh;
    sec_passwd_str_t    passwd_buf;
    sec_login_auth_src_t    auth_src;
    unsigned32		lc_len = LC_LEN, lc_len_used, lc_len_needed = 0;

    mtet_infoline("Startup for 1.1-->1.0 binary compatibility tests\n");

    strcpy((char *)passwd_buf, TEST_PASSWD);
    /* Set up the initial login context */
    if (sec_login_setup_identity(user, sec_login_no_flags, &login_h, &st)) {
        prec.key.key_type = sec_passwd_plain;
        prec.key.tagged_union.plain = passwd_buf;
        prec.pepper = NULL;
        prec.version_number = sec_passwd_c_version_none;

        if ((!sec_login_valid_and_cert_ident(login_h, &prec, &reset_passwd,
            &auth_src, &st)) || (st != error_status_ok)) {
            CHECK_STARTUP_RESULT(false,
                "Bin compat setup: sec_login_valid_and_cert_ident ", &st,
                error_status_ok);
        }
    } else {
        CHECK_STARTUP_RESULT(false,
            "Bin compat setup: sec_login_setup_identity ", &st,
            error_status_ok);
    }

    ilh = sec_login_become_initiator(login_h, sec_id_deleg_type_traced, NULL,
        NULL, NULL, NULL, sec_id_compat_mode_initiator, &st);
    CHECK_STARTUP_RESULT(false, "Bin compat setup: sec_login_become_initiator", &st,
        error_status_ok);

    /* write this new context to disk */
    sec_login_set_context(ilh, &st);
    CHECK_STARTUP_RESULT(false, "Bin compat setup: sec_login_set_context", &st,
        error_status_ok);

    /* create exportable context */
    sec_login_export_context(ilh, lc_len, lc_buf, &lc_len_used, &lc_len_needed, &st);
    CHECK_STARTUP_RESULT(false, "Bin compat setup: sec_login_export_context", &st,
        error_status_ok);
    mtet_infoline("Bin compat setup, filename is %s\n", lc_buf);

    /* establish keytab file */
    prec.key.key_type = sec_passwd_plain;
    prec.key.tagged_union.plain = passwd_buf;
    prec.pepper = NULL;
    prec.version_number = sec_passwd_c_version_none;
    sec_key_mgmt_set_key(rpc_c_authn_dce_secret, (void *) KEY_TAB, user,
	1, (void *) &prec, &st);
    CHECK_STARTUP_RESULT(false, "Bin compat setup: sec_key_mgmt_set_key", &st,
	error_status_ok);

    /* fork 1.0 process to import context and refresh! */
    st =tet_fork(child_1_0, NULL, 0, TET_PASS|TET_FAIL);
    CHECK_STARTUP_RESULT(false, "Bin compat setup: child process", &st, 0);

    sec_login_release_context(&login_h, &st);
    sec_login_release_context(&ilh, &st);
}

static void child_1_0(void)
{
    char *args[4];

    args[0] = "./bin_compat_1_0";
    args[1] = (char *) malloc(strlen((char *)lc_buf) + 1);
    strcpy(args[1], (char *) lc_buf);
    args[2] = (char *) malloc(strlen((char *)KEY_TAB) + 1);
    strcpy(args[2], (char *) KEY_TAB);
    args[3] = NULL;
    tet_exec(args[0], args, environ);

    return;
}

static void tp1()
{
    sec_login_handle_t	lh;
    sec_cred_pa_handle_t        pa_h;
    sec_id_pa_t                 *pa_dataP;
    sec_id_delegation_type_t    dlg_type_ret;
    sec_cred_cursor_t           cursor;
    error_status_t		st;
    sec_passwd_rec_t		keyinfo;

    mtet_infoline("Bin compat: test purpose 1\n");

    /* ensure that 1.0 app hasn't stomped on 1.1 info */
    sec_login_import_context(strlen((char *)lc_buf), lc_buf, &lh, &st);
    CHECK_RESULT(true, "Bin compat: sec_login_import_context", &st,
        error_status_ok);

    pa_h = sec_login_cred_get_initiator(lh, &st);
    CHECK_RESULT(true, "Bin compat: sec_login_cred_get_initiator", &st,
        error_status_ok);
    
    pa_dataP = sec_cred_get_pa_data(pa_h, &st);
    CHECK_RESULT(true, "Bin compat: sec_cred_get_pa_data", &st,
        error_status_ok);

    dlg_type_ret = sec_cred_get_delegation_type(pa_h, &st);
    CHECK_RESULT(true, "Bin compat: sec_cred_get_delegation_type", &st,
        error_status_ok);
    if (dlg_type_ret != sec_id_deleg_type_traced) {
        switch(dlg_type_ret) {
        case sec_id_deleg_type_none:
            mtet_infoline(
                "FAILED sec_cred_get_delegation_type returned: NONE");
            break;
        case sec_id_deleg_type_traced:
            mtet_infoline(
                "FAILED sec_cred_get_delegation_type returned: TRACED");
            break;
        case sec_id_deleg_type_impersonation:
            mtet_infoline(
                "FAILED sec_cred_get_delegation_type returned: IMPERSONATION");
            break;
        default:
            mtet_infoline("FAILED sec_cred_get_delegation_type returned: %d\n",
                dlg_type_ret);
        }
        tet_result(TET_FAIL);
        sec_login_purge_context(&lh, &st);
        return;
    }

    sec_login_cred_init_cursor(&cursor, &st);
    CHECK_RESULT(true, "Bin compat: sec_login_cred_init_cursor", &st,
        error_status_ok);

    pa_h = sec_login_cred_get_delegate(lh, &cursor, &st);
    CHECK_RESULT(true, "Bin compat: sec_login_cred_get_delegate", &st,
        sec_cred_s_no_more_entries);

    /* try retrieving both keys */
    sec_key_mgmt_get_key(rpc_c_authn_dce_secret, (void *) KEY_TAB, user,
	1, (void **) &keyinfo, &st);
    CHECK_RESULT(true, "Bin compat: sec_key_mgmt_get_key (vno:1)", &st,
	error_status_ok);

    sec_key_mgmt_free_key((void *) &keyinfo, &st);
    CHECK_RESULT(true, "Bin compat: sec_key_mgmt_free_key (vno:1)", &st,
	error_status_ok);

    sec_key_mgmt_get_key(rpc_c_authn_dce_secret, (void *) KEY_TAB, user,
	2, (void **) &keyinfo, &st);
    CHECK_RESULT(true, "Bin compat: sec_key_mgmt_get_key (vno:2)", &st,
	error_status_ok);

    sec_key_mgmt_free_key((void *) &keyinfo, &st);
    CHECK_RESULT(true, "Bin compat: sec_key_mgmt_free_key (vno:2)", &st,
	error_status_ok);

    tet_result(TET_PASS);
    sec_login_release_context(&lh, &st);
}
