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
 * $Log: disable_service_s.c,v $
 * Revision 1.1.7.3  1996/02/18  00:31:33  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:17:18  marty]
 *
 * Revision 1.1.7.2  1995/12/11  15:20:12  root
 * 	Submit
 * 	[1995/12/11  14:33:41  root]
 * 
 * Revision 1.1.2.2  1994/10/27  20:07:17  rrizzo
 * 	expand copyright
 * 	[1994/10/27  19:39:33  rrizzo]
 * 
 * Revision 1.1.2.1  1994/10/18  20:55:50  max
 * 	Initial submission of FVTs for dced utility API
 * 	[1994/10/18  20:47:59  max]
 * 
 * $EndLog$
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <dce/dce.h>
#include <dce/dce_error.h>
#include <dce/dce_cf.h>
#include <dce/rpc.h>
#include <dce/dced.h>

#include <dce_util_test_server.h>


int dce_server_disable_service_001()
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
    
    /* now let's disable our only service */
    dce_server_disable_service( hdl, if_handle_glob, &st);
    if ( st != rpc_s_ok) {
	send_infoline( "Failed dce_server_disable_service: " );
	send_infoerror_nl( st );
	send_infoline( tet_fail_c_str );
	errval = -1;
    }


    if (!errval) {
	ret = check_our_binding_ep_map();
	if( ret == 1) {
	    send_infoline_nl( "Error. Our endpoint is still in ep map");
	    send_infoline( tet_fail_c_str );
	    errval = -1;
	}
	else if ( ret != 0) {
	    send_infoline_nl( "Error. Failed check_our_binding_ep_map.");
	    send_infoline( tet_unresolved_c_str );
	    errval = -1;
	}
    }


    /* now we check the interface */
    if (!errval) {
	ret = check_our_if_registered();
	if ( ret == 1) {
	    send_infoline_nl( "Error. Our interface is still registered");
	    send_infoline( tet_fail_c_str );
	    errval = -1;
	}
	else if ( ret != 0) {
	    send_infoline_nl( "Error. Failed check_our_if_registered.");
	    send_infoline( tet_unresolved_c_str );
	    errval = -1;
	}
    }


    if ( ! errval ) { /* success */
	send_infoline( tet_pass_c_str );    
    }

    /* clean up and go */
    dce_server_unregister( &hdl, &st);

    return (errval == 0 ? 0 : -1);
	
}

