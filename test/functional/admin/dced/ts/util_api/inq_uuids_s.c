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
 * $Log: inq_uuids_s.c,v $
 * Revision 1.1.7.3  1996/02/18  00:31:49  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:17:31  marty]
 *
 * Revision 1.1.7.2  1995/12/11  15:20:27  root
 * 	Submit
 * 	[1995/12/11  14:33:49  root]
 * 
 * Revision 1.1.2.2  1994/10/27  20:07:23  rrizzo
 * 	expand copyright
 * 	[1994/10/27  19:39:42  rrizzo]
 * 
 * Revision 1.1.2.1  1994/10/18  20:56:02  max
 * 	Initial submission of FVTs for dced utility API
 * 	[1994/10/18  20:48:07  max]
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




int dce_server_inq_uuid_setup()
{
    return 0;
}

int dce_server_inq_uuid_cleanup()
{
    return 0;
}

int dce_server_inq_uuid_001()
{

    uuid_t conf_uuid, exec_uuid;
    unsigned_char_t *conf_uuid_str, *exec_uuid_str;
    error_status_t		st;
    unsigned32                  uuid_st;
    
    st = rpc_s_ok ;
    dce_server_inq_uuids (&conf_uuid, &exec_uuid, &st);
    if ( st != rpc_s_ok) {
	send_infoline( "Failed dce_server_inq_uuids: " );
	send_infoerror_nl( st );
	send_infoline( tet_fail_c_str );
	return -1;
    }

    uuid_to_string(&conf_uuid, &conf_uuid_str, &uuid_st);
    if (uuid_st !=  uuid_s_ok ) {
	send_infoline( "Failed uuids_to_string on: " );
	send_infoerror( st );
	send_infoline( tet_fail_c_str );
	return -1;
    }
	
    uuid_to_string(&exec_uuid, &exec_uuid_str, &st);
    if (uuid_st !=  uuid_s_ok ) {
	send_infoline( "Failed uuids_to_string: " );
	send_infoerror_nl( st );
	send_infoline( tet_fail_c_str );
	return -1;
    }

    sprintf( snd_infoline_buf, "From inq_uuids: Conf uuid = %s", conf_uuid_str);
    send_infoline_nl( snd_infoline_buf);
    sprintf( snd_infoline_buf, "From inq_uuids: Exec uuid = %s", exec_uuid_str);
    send_infoline_nl( snd_infoline_buf);


    if ( ! uuid_equal ( &conf_uuid, &conf_uuid_glob, &uuid_st)) {
	send_infoline( "Wrong conf uuid returned. Should be " );
	sprintf( snd_infoline_buf, "From inq_uuids: Conf uuid = %s", conf_uuid_g_str);
	send_infoline_nl( snd_infoline_buf);
	send_infoline( tet_fail_c_str );
	return -1;
    }

    rpc_string_free(&conf_uuid_str, &st);
    rpc_string_free(&exec_uuid_str, &st);

    /* success */
    send_infoline( tet_pass_c_str );
    return 0;
}


