
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
 * $Log: dtapi_srvr_utils.c,v $
 * Revision 1.1.4.2  1996/02/18  00:30:06  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:16:25  marty]
 *
 * Revision 1.1.4.1  1995/12/11  15:18:15  root
 * 	Submit
 * 	[1995/12/11  14:33:06  root]
 * 
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  23:18:19  root]
 * 
 * Revision 1.1.2.2  1994/09/30  22:55:36  baum
 * 	Fixed a number of test things (OT 12393)
 * 	[1994/09/30  22:54:24  baum]
 * 
 * Revision 1.1.2.1  1994/09/28  21:47:45  melman
 * 	moved admin tests to test/functional
 * 	[1994/09/28  21:15:24  melman]
 * 
 * Revision 1.1.2.3  1994/09/16  20:10:49  ganni
 * 	enhancements and cleanup of srvrexec tests
 * 	[1994/09/16  20:09:52  ganni]
 * 
 * Revision 1.1.2.2  1994/08/10  19:22:47  annie
 * 	expand copyright with OSF copyright text
 * 	[1994/08/10  17:17:07  annie]
 * 
 * Revision 1.1.2.1  1994/07/20  19:57:48  ganni
 * 	initial version
 * 	[1994/07/20  19:56:06  ganni]
 * 
 * $EndLog$
 */

/*
** Module:	dtapi_srvr_utils.c
**
** FACILITY:	DCE Host Daemon API
**
** ABSTRACT:	This module implements the dced api common functions
**
*/

#include <dtapi_impl.h>
#include <dtapi_1_srvr.h>


/*
** ROUTINE NAME:
**                    test_api10()
**
** DESCRIPTION:
**                    This routine verifies the result and the status
**                    from executing the interface dtapi_10
**
*/

boolean32
test_api_10( error_status_t expected_stat )
{
        unsigned_char_t         *string_binding;
        unsigned32              result,input;
        rpc_binding_handle_t    rpc_binding;
        error_status_t          st,tmp_st;
	int			i;

        rpc_string_binding_compose(DCED_SRVR_OBJ1,
                                   (unsigned_char_p_t)DCED_SRVR_PROTO,
                                   (unsigned_char_p_t)"",
                                   (unsigned_char_p_t)"",
                                   (unsigned_char_p_t)"",
                                   &string_binding,
                                   &st);
        if (st != error_status_ok)
        {
                tet_infoline("rpc_string_binding_compose failed");
#ifdef DEBUG
                dced_util_printerr(st);
#endif
                return false;
        }

        rpc_binding_from_string_binding(string_binding, &rpc_binding, &st);
        rpc_string_free(&string_binding, &tmp_st);
        if (st != error_status_ok)
        {
                tet_infoline("rpc_binding_from_string_binding failed");
#ifdef DEBUG
                dced_util_printerr(st);
#endif
                return false;
        }

	/*
	 * Wait for server to be started by dced and become ready
	 * (wait a minute at most)
	 */
#define NUM_TRIES 6
#define SLEEP_INTERVAL 10

	for ( i = 0; i < NUM_TRIES; i++ ) {
	  rpc_binding_reset(rpc_binding, &tmp_st);
	  rpc_mgmt_is_server_listening ( rpc_binding, &tmp_st );
	  if ( tmp_st == error_status_ok )
	    break;
	  sleep ( SLEEP_INTERVAL );
	}
	if ( i == NUM_TRIES ) {
	  
	  tet_infoline("dced_test_server never started listening" );
	  dced_util_printerr(tmp_st);
	  return false;
	}
	  
        input = 2;
        st = dtapi_10( rpc_binding, input, &result );
        if (st != expected_stat)
        {
#ifdef DEBUG
                tet_infoline("dtapi_10 failed:");
                tet_infoline("Expected status:");
                dced_util_printerr(expected_stat);
                tet_infoline("Received status:");
                dced_util_printerr(st);
#endif
                return false;
        }
	else if	(st == error_status_ok)
	{
        	if (result!=RESULT_1)
		    return false;
	}

	/* we don't need this where server can be stopped 
	   thru' server_stop()		*/
/*
        stop_server( rpc_binding, &st );
*/
        return true;
}

