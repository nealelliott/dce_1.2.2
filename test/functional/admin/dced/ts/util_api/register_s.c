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
 * $Log: register_s.c,v $
 * Revision 1.1.7.3  1996/02/18  00:31:54  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:17:35  marty]
 *
 * Revision 1.1.7.2  1995/12/11  15:20:32  root
 * 	Submit
 * 	[1995/12/11  14:33:52  root]
 * 
 * Revision 1.1.2.2  1994/10/27  20:07:26  rrizzo
 * 	expand copyright
 * 	[1994/10/27  19:39:45  rrizzo]
 * 
 * Revision 1.1.2.1  1994/10/18  20:56:07  max
 * 	Initial submission of FVTs for dced utility API
 * 	[1994/10/18  20:48:10  max]
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


int dce_server_register_001(void);


int dce_server_register_001()
{

    server_t                    *svr_info;
    dce_server_register_data_t  reg_data;
    dce_server_handle_t         hdl;
    error_status_t		st;
    unsigned32                  rpc_st;
    int                         ret;
    const unsigned char         *protseq = "ncadg_ip_udp";
    rpc_binding_vector_t        *binding_vector;
    unsigned32                  flags;

    flags = 0;

    dce_server_inq_server(&svr_info, &st);
    if ( st != rpc_s_ok) {
	send_infoline( "Failed dce_server_inq_server: " );
	send_infoerror_nl( st );
	send_infoline( tet_unresolved_c_str );
	return -1;
    }


    /* let's fix the registration data. Notice that we have
       it on the stack.  This is OK because we have register and
       unregister in the same function. Otherwise it would be
       a no-no */

    reg_data.ifhandle = if_handle_glob;
    reg_data.epv = NULL;
    reg_data.num_types = 0;
    reg_data.types = NULL;

    dce_server_register( /* trying with  some _no_ flags initially */
			flags,
			svr_info,
			&reg_data,
			&hdl,
			&st);
    if ( st != rpc_s_ok) {
	send_infoline( "Failed dce_server_register: " );
	send_infoerror_nl( st );
	send_infoline( tet_fail_c_str );
	return -1;
    }

    ret = check_our_binding_ep_map();
    if ( ret == -1 ) 
	send_infoline( tet_unresolved_c_str );
    else if ( ret == 0)
	send_infoline( tet_fail_c_str );

    /* this is just in case dced is not cleaning up quickly */
    dce_server_unregister( &hdl, &st);

    send_infoline ( tet_pass_c_str); 
    return 0;
}

