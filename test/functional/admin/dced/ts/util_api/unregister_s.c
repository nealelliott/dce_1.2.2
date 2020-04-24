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
 * $Log: unregister_s.c,v $
 * Revision 1.1.7.3  1996/02/18  00:32:05  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:17:42  marty]
 *
 * Revision 1.1.7.2  1995/12/11  15:20:43  root
 * 	Submit
 * 	[1995/12/11  14:34:00  root]
 * 
 * Revision 1.1.2.2  1994/10/27  20:07:31  rrizzo
 * 	expand copyright
 * 	[1994/10/27  19:39:51  rrizzo]
 * 
 * Revision 1.1.2.1  1994/10/18  20:56:14  max
 * 	Initial submission of FVTs for dced utility API
 * 	[1994/10/18  20:48:15  max]
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

#include <dce_util_test_server.h>


int dce_server_unregister_001()
{
    dce_server_handle_t         hdl;
    error_status_t		st;
    unsigned32                  rpc_st;
    int                         errval = 0;
    int                         ret;

    ret = do_our_registration ( &hdl);
    if ( ret != 0) {
	send_infoline_nl( "Failed registration");
	send_infoline( tet_unresolved_c_str );
	return -1;
    }

    /* and now the tested call */
    dce_server_unregister( &hdl, &st);
    if ( st != rpc_s_ok) {
	send_infoline( "Failed dce_server_unregister: " );
	send_infoerror_nl( st );
	send_infoline( tet_fail_c_str );
	return -1;
    }

    ret = check_our_binding_ep_map();
    if ( ret == 1 ) {
	send_infoline_nl( "Failed to remove endpoints from the epmap." );
	send_infoline( tet_fail_c_str );
	return -1;
    }
    else if ( ret == -1) {
	send_infoline( tet_unresolved_c_str );
	return -1;
    }


    ret = check_our_if_registered();
    if ( ret == 1 ) {
	send_infoline_nl( "Failed to unregister the interfaces." );
	send_infoline( tet_fail_c_str );
	return -1;
    }
    else if ( ret == -1) {
	send_infoline( tet_unresolved_c_str );
	return -1;
    }


    /* success */
    send_infoline ( tet_pass_c_str); 
    return 0;
}

