/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: sec_begin_s.c,v $
 * Revision 1.1.7.3  1996/02/18  00:31:58  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:17:37  marty]
 *
 * Revision 1.1.7.2  1995/12/11  15:20:35  root
 * 	Submit
 * 	[1995/12/11  14:33:55  root]
 * 
 * Revision 1.1.2.2  1994/10/27  20:07:28  rrizzo
 * 	expand copyright
 * 	[1994/10/27  19:39:47  rrizzo]
 * 
 * Revision 1.1.2.1  1994/10/18  20:56:09  max
 * 	Initial submission of FVTs for dced utility API
 * 	[1994/10/18  20:48:11  max]
 * 
 * $EndLog$
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <dce/dced.h>
#include <dce/dce_error.h>
#include <dce/dce_cf.h>
#include <dce/rpc.h>
#include <dce/dced.h>

#include <dce/sec_login.h>

#include <dce_util_test_server.h>


int dce_server_sec_begin_setup()
{
    return 0;
}


int dce_server_sec_begin_cleanup()
{
    return 0;
}

int dce_server_sec_begin_001()
{
    error_status_t		st;
    sec_login_handle_t          login_ctx;
    sec_login_net_info_t        net_info;
    sec_rgy_name_t              prin_name;
    unsigned char *             prin = prin_name;
    int                         errval = 0;


    dce_server_sec_begin( dce_server_c_manage_key  | dce_server_c_login , &st);
    if ( st != rpc_s_ok) {
	send_infoline( "Failed dce_server_sec_begin: " );
	send_infoerror_nl( st );
	errval = 4;
    }

    if ( ! errval ) {
	/* get our context and check it */
	sec_login_get_current_context ( &login_ctx, &st);
	if ( st != error_status_ok) {
	    send_infoline( "Failed to get current context: " );
	    send_infoerror_nl( st );
	    errval = 3;
	}
    }


    if ( ! errval ) {
	sec_login_inquire_net_info( login_ctx, &net_info, &st);
	if (st != error_status_ok && st != sec_login_s_not_certified) {
	    send_infoline( "Failed sec_login_inquire_net_info: " );
	    send_infoerror_nl( st );
	    errval = 2;
	}
    }


    if ( ! errval) {
	if ( net_info_to_principal (&net_info, (sec_rgy_name_t *)prin)) {
	    send_infoline_nl( "Failed to get principal name from net info" );
	    errval = 1;
	}

	if ( strncmp( prin, principal_g_str, 30)) {
	    send_infoline( "Wrong principal: " );
	    send_infoline_nl( prin );
	    send_infoline( "Current principal should be: ");
	    send_infoline_nl( principal_g_str);
	    errval = 1;
	}
    }

    /* cleanup and check success */
    if ( errval < 2 ) sec_login_free_net_info(&net_info);

    if ( errval < 3 ) sec_login_release_context(&login_ctx, &st);

    if ( errval > 0 ) {
	send_infoline( tet_fail_c_str );
	return -1;
    }

    /* success */
    send_infoline( tet_pass_c_str );
    return 0;
}
