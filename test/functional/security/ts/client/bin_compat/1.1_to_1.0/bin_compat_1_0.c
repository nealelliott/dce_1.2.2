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
 * $Log: bin_compat_1_0.c,v $
 * Revision 1.1.2.1  1996/06/05  14:19:07  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  13:48:56  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * The purpose of this program is to import a login
 * context, refresh it, and write it to disk.
 *
 * Assumptions: this program has been built with a 1.0
 * archived libdce.
 */
#define u_strlen(parm) ( (char *) parm == 0 ? 0 : strlen((char *) parm) )
#include <tet_api.h>

#include <string.h>

#include <dce/keymgmt.h>
#include <dce/sec_login.h>
#include <dce/dce_error.h>

#define TEST_PASSWD "-dce-"
#define TEST_USER   "root"

unsigned char           unexpected[dce_c_error_string_len];
int                     dce_status;
unsigned_char_p_t  user = (unsigned_char_p_t)TEST_USER;

tet_main(int argc, char *argv[])
{
    sec_login_handle_t	lh;
    unsigned32		lc_len;
    error_status_t	st;
    boolean32           reset_passwd;
    sec_passwd_rec_t    prec;
    sec_login_auth_src_t    auth_src;
    sec_passwd_str_t    passwd_buf;
    sec_login_net_info_t net_info;
    sec_passwd_rec_t    keyinfo;

    if (argc != 3)
        return -1;

    mtet_infoline("Arguments: %s %s %s\n", argv[0], argv[1], argv[2]);

    sec_login_import_context(strlen(argv[1]), (idl_byte *) argv[1],
        &lh, &st);
    if (st != error_status_ok) {
        dce_error_inq_text(st, unexpected, &dce_status);
        mtet_infoline("FAILED %s\nresult: \t%s\n\n",
            "Bin compat 1.0 setup: sec_login_import_context", unexpected);
        return -1;
    }
    mtet_infoline("PASSED: bin compat 1.0 setup: sec_login_import_context\n");

    sec_login_inquire_net_info(lh, &net_info, &st);
    if ((st != sec_login_s_not_certified) && (st != error_status_ok)) {
        dce_error_inq_text(st, unexpected, &dce_status);
        mtet_infoline("FAILED %s\nresult: \t%s\n\n",
            "Bin compat 1.0 setup: sec_login_inquire_net_info", unexpected);
        return -1;
    }
    mtet_infoline("PASSED: Bin compat 1.0 setup: sec_login_inquire_net_info\n");

    /* refresh and validate  -this will force a write to disk */
    if (!sec_login_refresh_identity(lh, &st)) {
        dce_error_inq_text(st, unexpected, &dce_status);                                            
        mtet_infoline("FAILED %s\nresult: \t%s\n%d\n\n",
            "Bin compat 1.0 setup: sec_login_refresh_identity", unexpected, st);
        return -1;
    }
    mtet_infoline("PASSED: Bin compat 1.0 setup: sec_login_refresh_identity\n");

    strcpy((char *)passwd_buf, TEST_PASSWD);
    prec.key.key_type = sec_passwd_plain;
    prec.key.tagged_union.plain = passwd_buf;
    prec.pepper = NULL;
    prec.version_number = sec_passwd_c_version_none;

    if (!sec_login_validate_identity(lh, &prec, &reset_passwd, &auth_src, &st)) {
        dce_error_inq_text(st, unexpected, &dce_status);
        mtet_infoline("FAILED %s\nresult: \t%s\n\n",
            "Bin compat 1.0 setup: sec_login_validate_identity", unexpected);
        return -1;
    }
    mtet_infoline("PASSED: Bin compat 1.0 setup: sec_login_validate_identity\n");

    sec_key_mgmt_get_key(rpc_c_authn_dce_secret, (void *) argv[2], user,
	1, (void **) &keyinfo, &st);
    if (st != error_status_ok) {
        dce_error_inq_text(st, unexpected, &dce_status);
        mtet_infoline("FAILED %s\nresult: \t%s\n\n",
            "Bin compat 1.0 setup: sec_key_mgmt_get_key", unexpected);
        return -1;
    }
    mtet_infoline("PASSED: Bin compat 1.0 setup: sec_key_mgmt_get_key\n");

    prec.key.key_type = sec_passwd_plain;
    prec.key.tagged_union.plain = passwd_buf;
    prec.pepper = NULL;
    prec.version_number = sec_passwd_c_version_none;
    sec_key_mgmt_set_key(rpc_c_authn_dce_secret, (void *) argv[2], user,
	2, (void *) &prec, &st);
    if (st != error_status_ok) {
        dce_error_inq_text(st, unexpected, &dce_status);
        mtet_infoline("FAILED %s\nresult: \t%s\n\n",
            "Bin compat 1.0 setup: sec_key_mgmt_set_key", unexpected);
        return -1;
    }
    mtet_infoline("PASSED: Bin compat 1.0 setup: sec_key_mgmt_set_key\n");

    return 0;
}
