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
 * $Log: bin_compat-tc2.c,v $
 * Revision 1.1.2.1  1996/06/05  14:18:22  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  13:39:50  arvind]
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
 * The purpose of this test is to import a login context
 * created by a 1.0 binary and check to see that 1.1
 * specific fields were initialized correctly.
 *
 * Assumes that the binary compatibility switch is on.
 */
#include <tet_api.h>
#include <ts-common.h>

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <dce/dce_error.h>
#include <dce/sec_login.h>

static void tc2_startup(void);
static void tc2_cleanup(void);
static void tp1(void);
static void child_1_0(void);
extern  char    **environ;

void (*tet_startup)() = tc2_startup;
void (*tet_cleanup)() = tc2_cleanup;

struct tet_testlist tet_testlist[] = {
    {tp1, 1},
    {NULL, 0}
};

#define TEST_USER "root"
#define TEST_PASSWD "-dce-"
#define LC_LEN 512
#define TEST_FILE   "/tmp/bin_compat"

unsigned_char_p_t  user = (unsigned_char_p_t)TEST_USER;
unsigned char unexpected[dce_c_error_string_len];
unsigned char expected[dce_c_error_string_len];
int                dce_status;
idl_byte           lc_buf[LC_LEN];

static void tc2_startup(void)
{
    error_status_t	st;

    mtet_infoline("Startup for 1.0-->1.1 binary compatibility tests\n");

    /* fork 1.0 process to setup context */
    st = tet_fork(child_1_0, NULL, 0, TET_PASS|TET_FAIL);
    CHECK_STARTUP_RESULT(false, "Bin compat setup: child process", &st, 0);
}

static void tc2_cleanup(void)
{
    (void) unlink(TEST_FILE);
}

static void child_1_0(void)
{
    char *args[2];

    args[0] = "./bin_compat-tc2_child";
    args[1] = NULL;
    tet_exec(args[0], args, environ);

    return;
}

static void tp1()
{
    sec_login_handle_t	lh;
    error_status_t		st;
    char			lc_buf[LC_LEN];
    FILE			*fp;
    sec_cred_pa_handle_t        pa_h;
    sec_id_delegation_type_t    dlg_type_ret;
    boolean32           reset_passwd;
    sec_passwd_rec_t    prec;
    sec_login_auth_src_t    auth_src;
    sec_passwd_str_t    passwd_buf;

    mtet_infoline("Bin compat tc2: test purpose 1\n");

    /* open temporary file */
    if ((fp = fopen(TEST_FILE, "r")) == (FILE *) NULL) {
        mtet_infoline("FAILED %s\nresult: \t%d\n\n", 
            "Bin compat: fopen", errno);
        tet_result(TET_FAIL);
        return;
    }

    fread(lc_buf, sizeof(lc_buf[0]), LC_LEN, fp);

    mtet_infoline("Bin compat tc2: filename is %s\n", lc_buf);

    (void) fclose(fp);

    /* import 1.0 context file */
    sec_login_import_context(strlen(lc_buf), (idl_byte *)lc_buf, &lh, &st);
    CHECK_RESULT(true, "Bin compat: sec_login_import_context", &st,
        error_status_ok);

    pa_h = sec_login_cred_get_initiator(lh, &st);
    CHECK_RESULT(true, "Bin compat: sec_login_cred_get_initiator", &st,
        error_status_ok);

    dlg_type_ret = sec_cred_get_delegation_type(pa_h, &st);
    CHECK_RESULT(true, "Bin compat: sec_cred_get_delegation_type", &st,
        error_status_ok);
    if (dlg_type_ret != sec_id_deleg_type_none) {
        switch(dlg_type_ret) {
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

    /* refresh context to force a write */
    sec_login_refresh_identity(lh, &st);
    CHECK_RESULT(true, "Bin compat: sec_login_refresh_identity", &st,
        error_status_ok);

    strcpy((char *)passwd_buf, TEST_PASSWD);
    prec.key.key_type = sec_passwd_plain;
    prec.key.tagged_union.plain = passwd_buf;
    prec.pepper = NULL;
    prec.version_number = sec_passwd_c_version_none;

    sec_login_validate_identity(lh, &prec, &reset_passwd, &auth_src, &st);
    CHECK_RESULT(true, "Bin compat: sec_login_validate_identity", &st,
        error_status_ok);

    tet_result(TET_PASS);
}
