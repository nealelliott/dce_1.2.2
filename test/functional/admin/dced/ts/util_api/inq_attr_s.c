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
 * $Log: inq_attr_s.c,v $
 * Revision 1.1.7.3  1996/02/18  00:31:41  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:17:26  marty]
 *
 * Revision 1.1.7.2  1995/12/11  15:20:21  root
 * 	Submit
 * 	[1995/12/11  14:33:45  root]
 * 
 * Revision 1.1.2.2  1994/10/27  20:07:20  rrizzo
 * 	expand copyright
 * 	[1994/10/27  19:39:38  rrizzo]
 * 
 * Revision 1.1.2.1  1994/10/18  20:55:58  max
 * 	Initial submission of FVTs for dced utility API
 * 	[1994/10/18  20:48:03  max]
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


int dce_server_inq_attr_setup()
{
    return 0;
}

int dce_server_inq_attr_cleanup()
{
    return 0;
}

int dce_server_inq_attr_001()
{

    sec_attr_t                  attr_values;
    sec_attr_enc_str_array_t   *str_arr_p;

    error_status_t		st;
    unsigned32                  uuid_st;
    uuid_t                      attr_uuid;

    

    uuid_from_string (attr_uuid_g_str, &attr_uuid,  &uuid_st);
    if (uuid_st !=  uuid_s_ok ) {
	send_infoline( "Failed uuids_from_string : " );
	send_infoerror( st );
	send_infoline( tet_unresolved_c_str );
	return -1;
    }
    
    dce_server_inq_attr( &attr_uuid, &attr_values, &st);
    if ( st != rpc_s_ok) {
	send_infoline( "Failed dce_server_inq_attr:" );
	send_infoerror_nl( st );
	send_infoline( tet_fail_c_str );
	return -1;
    }

    if ( attr_values.attr_value.attr_encoding != 
	    sec_attr_enc_printstring_array)	{
	send_infoline_nl( "Retrieved attributes have wrong encoding type" );
	send_infoline( tet_fail_c_str );
	return -1;
    }

    str_arr_p = attr_values.attr_value.tagged_union.string_array;
    if ( strcmp( str_arr_p->strings[0] , attr_value_0_g_str) ) {
	send_infoline_nl( "Retrieved attributes have wrong value" );
	send_infoline( tet_fail_c_str );
	return -1;
    }

    /* success */
    send_infoline( tet_pass_c_str );
    return 0;

}
