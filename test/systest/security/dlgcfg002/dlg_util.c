/*
 * @OSF_COPYRIGHT@
 * Copyright (c) 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: dlg_util.c,v $
 * Revision 1.1.4.2  1996/02/17  23:28:20  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:45:23  marty]
 *
 * Revision 1.1.4.1  1995/12/11  22:45:00  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:21:22  root]
 * 
 * Revision 1.1.2.2  1994/10/18  18:51:28  agd
 * 	expand copyright
 * 	[1994/10/18  17:39:31  agd]
 * 
 * Revision 1.1.2.1  1994/09/29  21:02:47  bhaim
 * 	submit dlgcfg002 test
 * 	[1994/09/29  21:01:25  bhaim]
 * 
 * $EndLog$
 */

#include <stdio.h>
#include <string.h>
#include <macros.h>
#include <dce/dce_error.h>
#include <dce/sec_login.h>
#include <dce/passwd.h>
#include "dts_macros.h"

void log_in(
    sec_login_handle_t *lcp,
    unsigned_char_p_t  user,
    unsigned_char_p_t  pbuf,
    error_status_t     *stp
)
{
    sec_passwd_rec_t     prec;
    sec_login_auth_src_t auth_src;
    boolean32            reset_passwd;
    CLEAR_STATUS(stp);

    if (sec_login_setup_identity(user, sec_login_no_flags, lcp, stp)) {
        prec.key.key_type = sec_passwd_plain;
        prec.key.tagged_union.plain = pbuf;
        prec.pepper = NULL;
        prec.version_number = sec_passwd_c_version_none;

        if (! sec_login_validate_identity(*lcp, &prec, &reset_passwd,
            &auth_src, stp)) {
            fprintf(stderr, "Error from validate_identity: %x\n", *stp);
	} else {
	    sec_login_set_context(*lcp, stp);
	}
    } else {
        fprintf(stderr, "Error from setup_identity: %x\n", *stp);
    }
}
