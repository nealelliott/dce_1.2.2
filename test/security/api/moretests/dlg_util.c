/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dlg_util.c,v $
 * Revision 1.1.6.2  1996/02/17  23:25:22  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:43:33  marty]
 *
 * Revision 1.1.6.1  1995/12/11  21:48:21  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:52:15  root]
 * 
 * Revision 1.1.3.1  1994/06/02  21:14:41  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:21:39  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * 	HP revision /main/ODESSA_2/1  1994/01/31  20:53  burati
 * 	Merge from ODESSA_1
 * 
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1993
** Unpublished work. All Rights Reserved.
*/

#include <stdio.h>
#include <string.h>
#include <macros.h>
#include <dce/dce_error.h>
#include <dce/sec_login.h>
#include <dce/passwd.h>

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
