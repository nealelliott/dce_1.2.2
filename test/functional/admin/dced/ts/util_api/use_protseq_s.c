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
 * $Log: use_protseq_s.c,v $
 * Revision 1.1.7.3  1996/02/18  00:32:09  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:17:45  marty]
 *
 * Revision 1.1.7.2  1995/12/11  15:20:47  root
 * 	Submit
 * 	[1995/12/11  14:34:02  root]
 * 
 * Revision 1.1.2.2  1994/10/27  20:07:32  rrizzo
 * 	expand copyright
 * 	[1994/10/27  19:39:53  rrizzo]
 * 
 * Revision 1.1.2.1  1994/10/18  20:56:16  max
 * 	Initial submission of FVTs for dced utility API
 * 	[1994/10/18  20:48:17  max]
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


int  dce_server_use_protseq_001()
{

    /****************************************************************
     *  dce_server_use_protseq  
     *
     *  dce_server_register and dce_server_use_protseq have been
     *  designed to be used with multiple servers in mind (multiple
     *  listen calls ).  At the time we can just hack it and call
     *  dce_server_use_protsep with a NULL handle.
     ****************************************************************/
    
    error_status_t st;
    unsigned32     rpc_st;
    int            ret;
    rpc_binding_handle_t binding;
    rpc_binding_vector_t *binding_vector;
    
    dce_server_handle_t   hdl = NULL;
    unsigned char * protseq_c_str = "ncadg_ip_udp";
    unsigned char *protseq_str, *binding_str;
    

    dce_server_use_protseq( hdl , protseq_c_str, &st);
    if ( st != error_status_ok) {
	send_infoline( "Failed dce_server_use_protseq\n" );
	send_infoerror_nl( st );
	send_infoline( tet_fail_c_str );
	return -1;
    }


    ret = do_our_registration ( &hdl);
    if ( ret != 0) {
	send_infoline_nl( "Failed registration");
	send_infoline( tet_unresolved_c_str );
	return -1;
    }


    /* now check that the endpoint data is correct */
    rpc_server_inq_bindings( &binding_vector, &rpc_st );
    if ( rpc_st != rpc_s_ok) {
	send_infoline_nl( "In check_our_binding_ep_map." );
	send_infoline( "Failed rpc_server_inq_bindings: " );
	send_infoerror_nl( rpc_st );
	return -1;
    }

    if (binding_vector->count < 1) {
	/* have no bindings */
	send_infoline_nl( "In check_our_binding_ep_map." );
	send_infoline( "Failed: rpc_server_inq_bindings returned " );
	send_infoline_nl( "a binding vector with no bindings." );
	send_infoline( tet_unresolved_c_str );
	return -1;
    }


    if (binding_vector->count > 1) {
	send_infoline_nl ( "Failed: got 2 bindings, should have been 1.");
	send_infoline( tet_fail_c_str );
	return -1;
    }

    binding = binding_vector->binding_h[0];

    rpc_binding_to_string_binding ( binding, &binding_str, &rpc_st);
    if ( rpc_st != rpc_s_ok) {
	send_infoline( "Failed rpc_binding_to_string_binding: ");
	send_infoerror_nl( st);
	send_infoline( tet_unresolved_c_str );
	return -1;
    }


    rpc_string_binding_parse (
			      binding_str,
			      NULL, 
			      &protseq_str,
			      NULL,
			      NULL,
			      NULL,
			      &st
			      );
    if ( rpc_st != rpc_s_ok) {
	send_infoline( "Failed rpc_string_binding_parse: ");
	send_infoerror_nl( st);
	send_infoline( tet_unresolved_c_str );
	return -1;
    }

    if ( strcmp ( protseq_str, protseq_c_str ) ){
	send_infoline_nl ( "Failed: Wrong protocol sequence used in our bindings.");
	send_infoline( tet_fail_c_str );
        return -1;
    }


    send_infoline( tet_pass_c_str );
    dce_server_unregister( &hdl, &st);
    return 0;

}

