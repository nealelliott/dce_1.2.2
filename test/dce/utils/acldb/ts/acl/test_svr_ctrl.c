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
 * $Log: test_svr_ctrl.c,v $
 * Revision 1.1.4.2  1996/02/18  00:23:32  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:11:03  marty]
 *
 * Revision 1.1.4.1  1995/12/08  22:07:17  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:42:52  root]
 * 
 * Revision 1.1.2.5  1994/06/10  20:18:36  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  16:57:57  devsrc]
 * 
 * Revision 1.1.2.4  1994/05/25  15:51:02  baum
 * 	Changes to allow comp[ilation on HP
 * 	[1994/05/25  15:50:32  baum]
 * 
 * Revision 1.1.2.3  1994/05/19  14:46:31  baum
 * 	Resync with DB & ACL API changes
 * 	[1994/05/19  14:46:03  baum]
 * 
 * Revision 1.1.2.2  1994/04/22  19:34:42  baum
 * 	Changed OUTPUT macro to not always do tet_result
 * 	[1994/04/22  19:28:38  baum]
 * 
 * Revision 1.1.2.1  1994/04/22  06:20:01  baum
 * 	Initial Checkin
 * 	[1994/04/22  06:18:59  baum]
 * 
 * $EndLog$
 */
/*
 * Module: 	test_svr_ctrl.c
 *
 * Facility:	DCE ACL Tests.
 *
 * Abstract:    This module implements the routines to start/stop the 
 *		acl_test_server
 */

/*
 * Include files
 */
#include <acl_test_utils.h>
#include <acl_test.h>
#include <sys/utsname.h>
#include <dce/rpcsts.h>

int
get_binding_to_test_server
(
 handle_t	*p_hnd
)
{

  error_status_t		st = error_status_ok, st2;
  unsigned_char_t		name [LONG_STRING_LEN];
  rpc_ns_handle_t		import_context;
  int				i, ret = 0;
  char				*p_expected_output;
  struct utsname		info;

  uname ( &info );
  sprintf ( name, ACL_TEST_SERVER_CDS_ENTRY, info.nodename );

  rpc_ns_binding_import_begin ( rpc_c_ns_syntax_default, name, 
			        acl_test_v1_0_c_ifspec, NULL, &import_context, 
			        &st );
  CHECK_DCE_ERROR ( "binding_import_begin", st );
  while  ( 1 ) 
  {
    rpc_ns_binding_import_next ( import_context, p_hnd, &st );
    if  ( st == rpc_s_no_more_entries ) 
    {
      rpc_ns_binding_import_done ( &import_context, &st2 );
      CHECK_DCE_ERROR ( "import_import_done", st2 );
    }
    CHECK_DCE_ERROR ( "import_next", st );
    
    if  ( st == error_status_ok ) 
    {
      if  ( rpc_mgmt_is_server_listening ( *p_hnd, &st ) ) 
      {
	unsigned_char_t	*server_prin;
	
	rpc_mgmt_inq_server_princ_name ( *p_hnd, rpc_c_authn_default,
					&server_prin, &st );
	CHECK_DCE_ERROR ( "inq_server_princ_name", st );
	
	rpc_binding_set_auth_info ( *p_hnd, server_prin,
				   rpc_c_protect_level_connect,
				   rpc_c_authn_default, NULL,
				   rpc_c_authz_dce, &st );
	CHECK_DCE_ERROR ( "set_auth_info", st );

	rpc_ns_binding_import_done ( &import_context, &st );
	CHECK_DCE_ERROR ( "import_done", st );

	ret = 1;
	goto done;
      }
    }
  }

 done:
  if ( ret != 1 )
  {
    OUTPUT ( outbuf, -1 );
  }
  return ( ret );
}

int
start_acl_test_server
(
 FILE	**pp_svr_hnd,
 char	*principal,
 char	*pwd
)
{
  /*
   * Start up test server.
   */

  int		ret;
  char		svr_output[LONG_STRING_LEN + 1];
  char		svr_cmd[LONG_STRING_LEN];

  sprintf ( svr_cmd, "%s %s %s", TEST_SERVER, principal, pwd );
  ret = child_start ( pp_svr_hnd, svr_cmd, "r", "listening...", 
		      svr_output );
  if ( ret == 0 )
  {
    /* setup Failed */
    sprintf ( outbuf, "start_test_server:Failed to start server:%s\n", 
	      svr_output );
    OUTPUT ( outbuf, -1 );
  }
  else
  {
    sprintf ( outbuf, "start_test_server:Started test server:%s\n", 
	      svr_output );
    OUTPUT ( outbuf, -1 );
  }

  return ( ret );
}

int
stop_acl_test_server
(
 FILE	*p_svr_hnd
)
{
  /*
   * Stop server
   */
  char				*p_expected_output;
  int				ret = 0;
  rpc_binding_handle_t		bh;
  error_status_t		st;

  ret = get_binding_to_test_server ( &bh );
  if ( ret != 1 )
  {
    sprintf ( outbuf, "stop_server:Could not get binding to test_server" );
    OUTPUT ( outbuf, -1 );
    goto done;
  }

  rpc_mgmt_stop_server_listening ( bh, &st );
  CHECK_DCE_ERROR ( "stop_server_listening", st );

  p_expected_output = "SERVER DONE OK";
  ret = child_wait ( p_svr_hnd, p_expected_output, actual_output );
  if ( ret == 0 )
  {
    sprintf ( outbuf, "stop_test_server:Failed to stop test server:%s\n", 
	     actual_output );
    OUTPUT ( outbuf, -1 );
  }
  else
  {
    sprintf ( outbuf, "stop_test_server:%s\n", actual_output );
    OUTPUT ( outbuf, -1 );
  }

 done:
  return ( ret );
}


