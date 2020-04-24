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
 * $Log: dce_acl_perm_fcns.c,v $
 * Revision 1.1.4.2  1996/02/18  00:23:25  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:10:57  marty]
 *
 * Revision 1.1.4.1  1995/12/08  22:07:07  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:42:48  root]
 * 
 * Revision 1.1.2.3  1994/06/10  20:18:34  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  16:57:56  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/19  14:46:32  baum
 * 	Resync with DB & ACL API changes
 * 	[1994/05/19  14:46:04  baum]
 * 
 * Revision 1.1.2.1  1994/04/22  06:19:47  baum
 * 	Initial Checkin
 * 	[1994/04/22  06:18:55  baum]
 * 
 * $EndLog$
 */
/*
 * Module: 	dce_acl_perm_fcns.c
 *
 * Facility:	DCE ACL Permissions Fcns Test Implementations
 *
 * Abstract:    This module implements the tests for the DCE ACL Library
 */

/*
 * Include files
 */
#include <acl_test_utils.h>

FILE 		*g_svr_hnd;
handle_t	g_svr_binding_hnd;

void 
setup ()
{
  unsigned32	dce_status;
  int		ret;
  char		svr_output[LONG_STRING_LEN + 1];


  /*
   * DCE login as ACL_TEST_PRINC1
   */
  tst_dce_login ( ACL_TEST_PRINC1, ACL_TEST_PRINC1, &dce_status );
  if ( dce_status != error_status_ok )
  {
    int inq_status;						       
    unsigned char dce_error_buf [dce_c_error_string_len]; 		
    unsigned char big_buf [dce_c_error_string_len * 2]; 		
    
    dce_error_inq_text ( dce_status, dce_error_buf, &inq_status );
    sprintf ( big_buf, "Could not DCE login as %s (%s}\n", ACL_TEST_PRINC1,
	      dce_error_buf );
    OUTPUT( big_buf, TET_FAIL );
    return;
  }


  /*
   * Start up test server.
   */

  ret = start_acl_test_server ( &g_svr_hnd, ACL_TEST_PRINC1, ACL_TEST_PRINC1 );

  if ( ret != 0 )
    ret = get_binding_to_test_server ( &g_svr_binding_hnd );
}

void
cleanup()
{
  stop_acl_test_server ( g_svr_hnd );
}

/*
 * Function: 	acl_inq_from_header_ok
 *
 * Abstract:	Exatract the ACL uuid from the standard header of a DB object.
 */
void
acl_inq_from_header_ok()
{
  char	*p_test_name = "acl_inq_from_header_ok";
  int			ret = 1;
  dce_db_header_t	db_hdr;
  error_status_t	st;
  unsigned char		*u_string, *u_string2;
  uuid_t		id;
  uuid_t	acl_uuid = { /* 003e8a15-21a6-1d9b-9815-0000c0dc0d4b */
        0x003e8a15,
        0x21a6,
        0x1d9b,
        0x98,
        0x15,
        {0x00, 0x00, 0xc0, 0xdc, 0x0d, 0x4b}
      };

  db_hdr.tagged_union.h.acl_uuid = acl_uuid;

  dce_acl_inq_acl_from_header ( db_hdr, sec_acl_type_object, &id, &st );

  if ( uuid_equal ( &id, &acl_uuid, &st ) != TRUE )
  {
    ret = 0;
    uuid_to_string ( &acl_uuid, &u_string, &st );
    uuid_to_string ( &id, &u_string2, &st );
    
    sprintf ( outbuf, "UUID's not equal (%s/%s)", u_string, u_string2 );
    rpc_string_free ( &u_string, &st );
    rpc_string_free ( &u_string2, &st );
  }

 done:
  REPORT_RESULT2( ret );
}

/*
 * Function: 	acl_inq_client_creds_ok
 *
 * Abstract:	Extract the Client Creds and examine it.  This is the client
 *		side of the operation.  The actual call to 
 *		dce_acl_inq_client_creds() occurs in the 'acl_test_server'
 *		implementation of the 'acl_inq_client_creds_ok' RPC.  The
 *		results are passed back to this routine via the child_wait()
 *		mechanism.
 */
void
acl_inq_client_creds_ok() 
{
  char			*p_test_name = "acl_inq_client_creds_ok";
  uuid_t		client_principal;
  handle_t		hnd;
  int			ret = 1;
  error_status_t	st, st2;
  sec_login_handle_t	login_context;
  sec_login_net_info_t	net_info;

  sec_login_get_current_context ( &login_context, &st );
  CHECK_DCE_ERROR ( "sec_login_get_curret_context", st );

  sec_login_inquire_net_info ( login_context, &net_info, &st );
  if ( st == sec_login_s_not_certified )
    st = error_status_ok;
  CHECK_DCE_ERROR ( "sec_login_inquire_net_info", st );
  
  client_principal = net_info.pac.principal.uuid;
  sec_login_free_net_info ( &net_info );

  TRY
    acl_inq_client_creds_ok_fvt( g_svr_binding_hnd, client_principal, &st );
  CATCH_ALL
    ret = 0;
  ENDTRY

  if ( ret == 0 )
  {
    sprintf ( outbuf, "Exception occured in RPC" );
    goto done;
  }

  ret = child_wait ( g_svr_hnd, "Success", actual_output );
  if ( ret == 0 )
  {
    sprintf ( outbuf, "[%s]", actual_output );
    goto done;
  }

 done:
  REPORT_RESULT2( ret );
}


/*
 * Function: 	acl_inq_client_permset_ok
 *
 * Abstract:	Examine the permset of the client to see if it matches what
 *		is expected.  Assume the client principal is the OWNER of
 *		the ACL.  This would be principal ACL_TEST_PRINC1.  This 
 *		routine is the client side of the operation.  The actual call 
 *		to dce_acl_inq_client_permset() occurs in the 'acl_test_server'
 *		implementation of the 'acl_inq_client_permset_ok' RPC.  The
 *		results are passed back to this routine via the child_wait()
 *		mechanism.
 */
void
acl_inq_client_permset_ok() 
{
  char			*p_test_name = "acl_inq_client_permset_ok";
  uuid_t		client_principal;
  handle_t		hnd;
  int			ret = 1;
  error_status_t	st, st2;
  sec_login_handle_t	login_context;
  sec_login_net_info_t	net_info;

  TRY
    acl_inq_client_permset_ok_fvt( g_svr_binding_hnd, &st );
  CATCH_ALL
    ret = 0;
  ENDTRY

  if ( ret == 0 )
  {
    sprintf ( outbuf, "Exception occured in RPC" );
    goto done;
  }

  ret = child_wait ( g_svr_hnd, "Success", actual_output );
  if ( ret == 0 )
  {
    sprintf ( outbuf, "[%s]", actual_output );
    goto done;
  }

 done:
  REPORT_RESULT2( ret );
}

/*
 * Function: 	acl_is_client_authorized_ok
 *
 * Abstract:	See if the client is authorized for a certain priviledge.
 *		Assume the client principal is the OWNER of the ACL and the
 *		OWNER has OWNER_PERMS.  This would be principal
 *		ACL_TEST_PRINC1.  This routine is the client side of the 
 *		operation.  The actual call to dce_acl_is_client_authorized() 
 *		occurs in the 'acl_test_server'
 *		implementation of the 'acl_is_client_authorized_ok' RPC.  The
 *		results are passed back to this routine via the child_wait()
 *		mechanism.
 */
void
acl_is_client_authorized_ok() 
{
  char			*p_test_name = "acl_is_client_authorized_ok";
  uuid_t		client_principal;
  handle_t		hnd;
  int			ret = 1;
  error_status_t	st, st2;
  sec_login_handle_t	login_context;
  sec_login_net_info_t	net_info;

  TRY
    acl_is_client_authorized_ok_fvt( g_svr_binding_hnd, &st );
  CATCH_ALL
    ret = 0;
  ENDTRY

  if ( ret == 0 )
  {
    sprintf ( outbuf, "Exception occured in RPC" );
    goto done;
  }

  ret = child_wait ( g_svr_hnd, "Success", actual_output );
  if ( ret == 0 )
  {
    sprintf ( outbuf, "[%s]", actual_output );
    goto done;
  }

 done:
  REPORT_RESULT2( ret );
}

/*
 * Function: 	acl_inq_client_prin_and_group_ok
 *
 * Abstract:	Extract the Client Creds and examine it.  This is the client
 *		side of the operation.  The actual call to 
 *		dce_acl_inq_client_prin_and_group() occurs in the 
 *		'acl_test_server' implementation of the 
 *		'acl_inq_client_prin_and_group_ok' RPC.  The
 *		results are passed back to this routine via the child_wait()
 *		mechanism.
 */
void
acl_inq_client_prin_and_group_ok() 
{
  char			*p_test_name = "acl_inq_client_prin_and_group_ok";
  uuid_t		client_principal, client_group;
  handle_t		hnd;
  int			ret = 1;
  error_status_t	st, st2;
  sec_login_handle_t	login_context;
  sec_login_net_info_t	net_info;

  sec_login_get_current_context ( &login_context, &st );
  CHECK_DCE_ERROR ( "sec_login_get_curret_context", st );

  sec_login_inquire_net_info ( login_context, &net_info, &st );
  if ( st == sec_login_s_not_certified )
    st = error_status_ok;
  CHECK_DCE_ERROR ( "sec_login_inquire_net_info", st );
  
  client_principal = net_info.pac.principal.uuid;
  client_group = net_info.pac.group.uuid;
  sec_login_free_net_info ( &net_info );

  TRY
    acl_inq_client_p_and_g_ok_fvt( g_svr_binding_hnd, client_principal,
				   client_group, &st );
  CATCH_ALL
    ret = 0;
  ENDTRY

  if ( ret == 0 )
  {
    sprintf ( outbuf, "Exception occured in RPC" );
    goto done;
  }

  ret = child_wait ( g_svr_hnd, "Success", actual_output );
  if ( ret == 0 )
  {
    sprintf ( outbuf, "[%s]", actual_output );
    goto done;
  }

 done:
  REPORT_RESULT2( ret );
}

/*
 * Function: 	acl_inq_permset_for_creds_ok
 *
 * Abstract:	Examine the permset of the client to see if it matches what
 *		is expected.  Assume the client principal is the OWNER of
 *		the ACL.  This would be principal ACL_TEST_PRINC1.  This 
 *		routine is the client side of the operation.  The actual call 
 *		to dce_acl_inq_permset_for_creds() occurs in the 'acl_test_server'
 *		implementation of the 'acl_inq_permset_for_creds_ok' RPC.  The
 *		results are passed back to this routine via the child_wait()
 *		mechanism.
 */
void
acl_inq_permset_for_creds_ok() 
{
  char			*p_test_name = "acl_inq_permset_for_creds_ok";
  uuid_t		client_principal;
  handle_t		hnd;
  int			ret = 1;
  error_status_t	st, st2;
  sec_login_handle_t	login_context;
  sec_login_net_info_t	net_info;

  TRY
    acl_inq_prmset_for_creds_ok_fvt( g_svr_binding_hnd, &st );
  CATCH_ALL
    ret = 0;
  ENDTRY

  if ( ret == 0 )
  {
    sprintf ( outbuf, "Exception occured in RPC" );
    goto done;
  }

  ret = child_wait ( g_svr_hnd, "Success", actual_output );
  if ( ret == 0 )
  {
    sprintf ( outbuf, "[%s]", actual_output );
    goto done;
  }

 done:
  REPORT_RESULT2( ret );
}

#ifndef _USE_TET
main ()
{
  setup();
  acl_inq_from_header_ok();
  acl_inq_client_creds_ok();
  acl_inq_client_permset_ok();
  acl_is_client_authorized_ok(); 
  acl_inq_client_prin_and_group_ok ();
  acl_inq_permset_for_creds_ok ();
  cleanup();
}
#else
void (*tet_startup)() = setup;   /* Load the start routine into TET */
void (*tet_cleanup)() = cleanup; /* Load the clean routine into TET */

struct tet_testlist tet_testlist[] =
{
  { acl_inq_from_header_ok, 1 },
  { acl_inq_client_creds_ok, 1 },
  { acl_inq_client_permset_ok, 1 },
  { acl_is_client_authorized_ok, 1 },
  { acl_inq_client_prin_and_group_ok, 1 },
  { acl_inq_permset_for_creds_ok, 1 },
  { (void (*)())NULL, 0 }
};
#endif
