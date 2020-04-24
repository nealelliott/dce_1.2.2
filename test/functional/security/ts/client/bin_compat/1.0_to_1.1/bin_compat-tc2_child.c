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
 * $Log: bin_compat-tc2_child.c,v $
 * Revision 1.1.2.1  1996/06/05  14:18:34  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  13:42:39  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 *
 * The purpose of this program is to establish a login
 * context and write it to disk.
 *
 * Assumptions: this program has been built with a 1.0
 * archived libdce.
 */
#include <tet_api.h>

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <dce/sec_login.h>
#include <dce/dce_error.h>

#define TEST_PASSWD "-dce-"
#define TEST_USER   "root"
#define LC_LEN      512

unsigned char           unexpected[dce_c_error_string_len];
int                     dce_status;

tet_main(int argc, char *argv[])
{
    unsigned_char_p_t  user = (unsigned_char_p_t)TEST_USER;
    sec_login_handle_t login_h;
    sec_passwd_rec_t   prec;
    error_status_t     st;
    sec_passwd_str_t   passwd_buf;
    boolean32          reset_passwd;
    sec_login_auth_src_t    auth_src;
    unsigned32         lc_len = LC_LEN, lc_len_used, lc_len_needed = 0;
    idl_byte           lc_buf[LC_LEN];
    FILE	       *fp;

    if (argc != 1)
        return -1;

    strcpy((char *)passwd_buf, TEST_PASSWD);
    /* Set up the initial login context */
    if (sec_login_setup_identity(user, sec_login_no_flags, &login_h, &st)) {
        prec.key.key_type = sec_passwd_plain;
        prec.key.tagged_union.plain = passwd_buf;
        prec.pepper = NULL;
        prec.version_number = sec_passwd_c_version_none;

        if (!sec_login_validate_identity(login_h, &prec, &reset_passwd,
            &auth_src, &st)) {
            dce_error_inq_text(st, unexpected, &dce_status);
            mtet_infoline("FAILED %s\nresult: \t%s\n%d\n\n",
                "Bin compat 1.0 setup: sec_login_validate_identity", unexpected, st);
            return -1;
        }
    } else {
            dce_error_inq_text(st, unexpected, &dce_status);
            mtet_infoline("FAILED %s\nresult: \t%s\n%d\n\n",
                "Bin compat 1.0 setup: sec_login_setup_identity", unexpected, st);
            return -1;
    }
    mtet_infoline("PASSED: Bin compat 1.0 setup: sec_login_setup/validate_identity\n");

    /* write this context to disk */
    sec_login_set_context(login_h, &st);
    if (st != error_status_ok) {
        dce_error_inq_text(st, unexpected, &dce_status);
        mtet_infoline("FAILED %s\nresult: \t%s\n%d\n\n",
            "Bin compat 1.0 setup: sec_login_set_context", unexpected, st);
        return -1;
    }
    mtet_infoline("PASSED: Bin compat 1.0 setup: sec_login_set_context\n");

    /* create exportable context */
    sec_login_export_context(login_h, lc_len, lc_buf, &lc_len_used, &lc_len_needed, &st);
    if (st != error_status_ok) {
        dce_error_inq_text(st, unexpected, &dce_status);
        mtet_infoline("FAILED %s\nresult: \t%s\n%d\n\n",
            "Bin compat 1.0 setup: sec_login_export_context", unexpected, st);
        return -1;
    }
    mtet_infoline("PASSED: Bin compat 1.0 setup: sec_login_export_context\n");

    /* write buffer to temporary file */
    if ((fp = fopen("/tmp/bin_compat", "w+")) == (FILE *) NULL) {
        mtet_infoline("FAILED %s\nresult: \t%d\n\n",
            "Bin compat 1.0 setup: fopen", errno);
        return -1;
    }

    if (fwrite(lc_buf, sizeof(lc_buf[0]), lc_len_used, fp) != lc_len_used) {
        mtet_infoline("FAILED %s\nresult: \t%d\n\n",
            "Bin compat 1.0 setup: fwrite", errno);
        (void) fclose(fp);
        return -1;
    }
    (void) fclose(fp);

    return 0;
}
