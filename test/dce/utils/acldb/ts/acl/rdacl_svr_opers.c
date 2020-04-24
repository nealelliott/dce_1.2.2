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
 * $Log: rdacl_svr_opers.c,v $
 * Revision 1.1.4.2  1996/02/18  00:23:28  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:10:58  marty]
 *
 * Revision 1.1.4.1  1995/12/08  22:07:10  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:42:49  root]
 * 
 * Revision 1.1.2.5  1994/06/10  20:18:35  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  16:57:57  devsrc]
 * 
 * Revision 1.1.2.4  1994/05/25  15:51:00  baum
 * 	Changes to allow comp[ilation on HP
 * 	[1994/05/25  15:50:31  baum]
 * 
 * Revision 1.1.2.3  1994/05/19  14:34:53  baum
 * 	Resync tests with ACL & DB API changes
 * 	[1994/05/19  14:34:18  baum]
 * 
 * Revision 1.1.2.2  1994/04/22  19:34:41  baum
 * 	Changed OUTPUT macro to not always do tet_result
 * 	[1994/04/22  19:28:37  baum]
 * 
 * Revision 1.1.2.1  1994/04/22  06:19:51  baum
 * 	Initial Checkin
 * 	[1994/04/22  06:18:56  baum]
 * 
 * $EndLog$
 */
/*
 * Module: 	dce_rdacl_svr_opers.c
 *
 * Facility:	DCE ACL rdacl Implementation
 *
 * Abstract:    This module implements the tests for the DCE ACL Library
 */

/*
 * Include files
 */
#include <acl_test_utils.h>
#include <sys/utsname.h>

FILE		*g_svr_hnd;
char		cds_target[LONG_STRING_LEN];
char		acl_edit_list[LONG_STRING_LEN];
char		acl_edit_modify[LONG_STRING_LEN];
char		acl_edit_delete[LONG_STRING_LEN];
char		acl_edit_grant[LONG_STRING_LEN];
char		acl_edit_test_read[LONG_STRING_LEN];
char		acl_edit_test_dump[LONG_STRING_LEN];
char		acl_edit_test_write[LONG_STRING_LEN];
char		princ1_perms_string[LONG_STRING_LEN];
char		princ2_perms_string[LONG_STRING_LEN];

void 
setup ()
{

  int			ret;
  char			svr_output[LONG_STRING_LEN + 1];
  struct utsname 	info;
  unsigned32		dce_status;

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

  uname ( &info );
  sprintf ( cds_target, ACL_TEST_OBJECT1, info.nodename );
  sprintf ( acl_edit_list, ACL_EDIT_LIST, cds_target );
  sprintf ( acl_edit_modify, ACL_EDIT_MODIFY, cds_target, ACL_TEST_PRINC2 );
  sprintf ( acl_edit_delete, ACL_EDIT_DELETE, cds_target, ACL_TEST_PRINC2);
  sprintf ( acl_edit_grant, ACL_EDIT_GRANT, cds_target );
  sprintf ( acl_edit_test_read, ACL_EDIT_TEST_READ, cds_target );
  sprintf ( acl_edit_test_dump, ACL_EDIT_TEST_DUMP, cds_target );
  sprintf ( acl_edit_test_write, ACL_EDIT_TEST_WRITE, cds_target );
  sprintf ( princ1_perms_string, PRINC1_PERMS_STRING, ACL_TEST_PRINC1 );
  sprintf ( princ2_perms_string, PRINC2_PERMS_STRING, ACL_TEST_PRINC2 );

}

void
cleanup()
{
  int ret;

  ret = stop_acl_test_server( g_svr_hnd );
  child_close ( g_svr_hnd );
}

/*
 * Function: 	acl_lookup_ok
 *
 * Abstract:	Lookup the ACL for an object.
 */

void
acl_lookup_ok()
{
  char	*p_test_name = "acl_lookup_ok";
  int	ret;
  FILE	*acl_edit_hnd;

  ret = child_start ( &acl_edit_hnd, acl_edit_list, "r", "", actual_output );
  CHECK_CHILD_ERROR ( ret );

  /* 
   * Next two lines are the header for ACL_EDIT and contain the cell_name
   * which we can just skip over
   */
  ret = child_wait ( acl_edit_hnd, ACCEPT_ANY_LINE, actual_output );
  CHECK_CHILD_ERROR ( ret );

  ret = child_wait ( acl_edit_hnd, ACCEPT_ANY_LINE, actual_output );
  CHECK_CHILD_ERROR ( ret );

  ret = child_wait ( acl_edit_hnd, "unauthenticated:r---t-",
		     actual_output );
  CHECK_CHILD_ERROR ( ret );

  ret = child_wait ( acl_edit_hnd, princ1_perms_string, actual_output );
  CHECK_CHILD_ERROR ( ret );

  ret = child_wait ( acl_edit_hnd, "any_other:r---t-", actual_output );
  CHECK_CHILD_ERROR ( ret );

 done:
  REPORT_RESULT( ret );

  child_close ( acl_edit_hnd );

}

/*
 * Function: 	acl_replace_ok
 *
 * Abstract:	Replace the ACL for an object.
 */

void
acl_replace_ok()
{
  char	*p_test_name = "acl_replace_ok";
  int	ret;
  FILE	*acl_edit_hnd;

  ret = child_start ( &acl_edit_hnd, acl_edit_modify, "r", NULL, 
		      actual_output );
  CHECK_CHILD_ERROR ( ret );

  child_close ( acl_edit_hnd );

  ret = child_start ( &acl_edit_hnd, acl_edit_list, "r", "", actual_output );
  CHECK_CHILD_ERROR ( ret );

  /* 
   * Next two lines are the header for ACL_EDIT and contain the cell_name
   * which we can just skip over
   */
  ret = child_wait ( acl_edit_hnd, ACCEPT_ANY_LINE, actual_output );
  CHECK_CHILD_ERROR ( ret );

  ret = child_wait ( acl_edit_hnd, ACCEPT_ANY_LINE, actual_output );
  CHECK_CHILD_ERROR ( ret );

  ret = child_wait ( acl_edit_hnd, "unauthenticated:r---t-",
		     actual_output );
  CHECK_CHILD_ERROR ( ret );

  ret = child_wait ( acl_edit_hnd, princ1_perms_string, actual_output );
  CHECK_CHILD_ERROR ( ret );

  ret = child_wait ( acl_edit_hnd, princ2_perms_string, actual_output );
  CHECK_CHILD_ERROR ( ret );

  ret = child_wait ( acl_edit_hnd, "any_other:r---t-", actual_output );
  CHECK_CHILD_ERROR ( ret );

 done:
  REPORT_RESULT( ret );

  /*
   * Remove the acl for principal ACL_TEST_PRINC1
   */
  child_close ( acl_edit_hnd );

  ret = child_start ( &acl_edit_hnd, acl_edit_delete, "r", NULL, 
		      actual_output );
  if ( ret != 1 )
  {
    sprintf ( outbuf, "%s: Could not remove ACL on cleanup!\n", p_test_name );
    OUTPUT ( outbuf, -1 );
  }
  child_close ( acl_edit_hnd );

}

/*
 * Function: 	acl_get_access_ok
 *
 * Abstract:	Get access of an ACL for an object.
 */

void
acl_get_access_ok()
{
  char	*p_test_name = "acl_get_access_ok";
  int	ret;
  FILE	*acl_edit_hnd;

  ret = child_start ( &acl_edit_hnd, acl_edit_grant,
		      "r", "Granted permissions: rwdct-", actual_output );
  REPORT_RESULT( ret );

  child_close ( acl_edit_hnd );

}

/*
 * Function: 	acl_test_access_allow
 *
 * Abstract:	Test for an operation we have privilege for.
 */
void
acl_test_access_allow()
{
  char	*p_test_name = "acl_test_access_allow";
  int	ret;
  FILE	*acl_edit_hnd;

  ret = child_start ( &acl_edit_hnd, acl_edit_test_read,
		      "r", "Access: GRANTED", actual_output );
  REPORT_RESULT( ret );

  child_close ( acl_edit_hnd );

}

/*
 * Function: 	acl_test_access_unauth_allow
 *
 * Abstract:	Test for an operation we have privilege for if we are 
 *		unauthenticated.
 */
void
acl_test_access_unauth_allow()
{
  char	*p_test_name = "acl_test_access_unauth_allow";
  int	ret;
  FILE	*acl_edit_hnd;
  char	*creds_cache;

  /*
   * DCE logout.
   */
  creds_cache = getenv ( "KRB5CCNAME" );
  if ( creds_cache == NULL )
  {
    unsigned char err_buf[LONG_STRING_LEN];

    sprintf ( err_buf, "Can't find KRB5CCNAME in environment to logout!\n" );
    OUTPUT ( err_buf, TET_FAIL );
    return;
  }

  unsetenv ( "KRB5CCNAME" );

  ret = child_start ( &acl_edit_hnd, acl_edit_test_read,
		      "r", "Access: GRANTED", actual_output );
  REPORT_RESULT( ret );

  child_close ( acl_edit_hnd );

  if ( setenv ( "KRB5CCNAME", creds_cache, 1 ) )
  {
    unsigned char err_buf[LONG_STRING_LEN];

    sprintf ( err_buf, "Can't reset KRB5CCNAME in environment to login!\n" );
    OUTPUT ( err_buf, -1 );
    return;
  }    
}

/*
 * Function: 	acl_test_access_unauth_denied
 *
 * Abstract:	Test for an operation we do not have privilege for if we are 
 *		unauthenticated but *do* if we are ACL_TEST_PRINC1
 */
void
acl_test_access_unauth_denied()
{
  char	*p_test_name = "acl_test_access_unauth_denied";
  int	ret;
  FILE	*acl_edit_hnd;
  char	*creds_cache;

  /*
   * DCE logout.
   */
  creds_cache = getenv ( "KRB5CCNAME" );
  if ( creds_cache == NULL )
  {
    unsigned char err_buf[LONG_STRING_LEN];

    sprintf ( err_buf, "Can't find KRB5CCNAME in environment to logout!\n" );
    OUTPUT ( err_buf, TET_FAIL );
    return;
  }

  unsetenv ( "KRB5CCNAME" );

  ret = child_start ( &acl_edit_hnd, acl_edit_test_write,
		      "r", "Access: GRANTED", actual_output );
  REPORT_RESULT( ret );

  child_close ( acl_edit_hnd );

  if ( setenv ( "KRB5CCNAME", creds_cache, 1 ) )
  {
    unsigned char err_buf[LONG_STRING_LEN];

    sprintf ( err_buf, "Can't reset KRB5CCNAME in environment to login!\n" );
    OUTPUT ( err_buf, -1 );
    return;
  }    
}

/*
 * Function: 	acl_test_access_denied
 *
 * Abstract:	Test for an operation we do not have privilege for.
 */
void
acl_test_access_denied()
{
  char	*p_test_name = "acl_test_access_denied";
  int	ret;
  FILE	*acl_edit_hnd;

  ret = child_start ( &acl_edit_hnd, acl_edit_test_dump,
		      "r", "Access: DENIED", actual_output );
  REPORT_RESULT( ret );

  child_close ( acl_edit_hnd );

}

/*
 * Function: 	acl_get_printstring_ok
 *
 * Abstract:	Test the printstring output.
 */

void
acl_get_printstring_ok()
{
  char	*p_test_name = "acl_get_printstring_ok";
  int	ret;
  FILE	*acl_edit_hnd;

  ret = child_start ( &acl_edit_hnd, acl_edit_list, "r", "", actual_output );
  CHECK_CHILD_ERROR ( ret );

  /* 
   * Next two lines are the header for ACL_EDIT and contain the cell_name
   * which we can just skip over
   */
  ret = child_wait ( acl_edit_hnd, ACCEPT_ANY_LINE, actual_output );
  CHECK_CHILD_ERROR ( ret );

  ret = child_wait ( acl_edit_hnd, ACCEPT_ANY_LINE, actual_output );
  CHECK_CHILD_ERROR ( ret );

  ret = child_wait ( acl_edit_hnd, "unauthenticated:r---t-",
		     actual_output );
  CHECK_CHILD_ERROR ( ret );

  ret = child_wait ( acl_edit_hnd, princ1_perms_string, actual_output );
  CHECK_CHILD_ERROR ( ret );

  ret = child_wait ( acl_edit_hnd, "any_other:r---t-", actual_output );
  CHECK_CHILD_ERROR ( ret );

 done:
  REPORT_RESULT( ret );

  child_close ( acl_edit_hnd );

}

/*
 * Function: 	acl_get_mgr_types_ok
 *
 * Abstract:    Retrieve the types of ACL's protecting an object.
 */
void
acl_get_mgr_types_ok()
{
  char			*p_test_name = "acl_get_mgr_types_ok";
  error_status_t	st;
  int			ret = 1, i;
  FILE			*acl_edit_hnd;
  unsigned char		*u_string, *acl_test_uuid_str;
  sec_acl_handle_t	hnd;
  unsigned32		num_mgr_types_used;
  unsigned32		num_mgr_types;
#define MAX_NUM_MGR_TYPES 64
  uuid_t		mgr_types[MAX_NUM_MGR_TYPES];
  uuid_t		loc_acl_test_uuid = INIT_ACL_TEST_UUID;

  sec_acl_bind ( cds_target, FALSE, &hnd, &st );
  CHECK_DCE_ERROR ( "sec_acl_bind", st );

  sec_acl_get_manager_types ( hnd, sec_acl_type_object, MAX_NUM_MGR_TYPES,
			      &num_mgr_types_used, &num_mgr_types, 
			      mgr_types, &st );
  CHECK_DCE_ERROR ( "sec_acl_get_mgr_types", st );

#ifdef VERBOSE
  for ( i = 0; i < num_mgr_types_used; i++ )
  {
    uuid_to_string ( &mgr_types[i], &u_string, &st );
    sprintf ( outbuf, "%s: Mgr Type #%d, %s\n", p_test_name, i, u_string );
    OUTPUT ( outbuf, -1 );
    rpc_string_free ( &u_string, &st );
  }
#endif
  if ( num_mgr_types == 1 )
  {
    uuid_to_string ( &mgr_types[0], &u_string, &st );
    uuid_to_string ( &loc_acl_test_uuid, &acl_test_uuid_str, &st );
    if ( !strcmp ( u_string, acl_test_uuid_str ) )
      ret = 1;
    else
    {
      sprintf ( outbuf, "Uuids don't match (%s/%s)", u_string, 
	        acl_test_uuid_str );
      ret = 0;
    }
    rpc_string_free ( &u_string, &st );
    rpc_string_free ( &acl_test_uuid_str, &st );
  }
  else
  {
    sprintf ( outbuf, "Number of mgr types != 1." );
    ret = 0;
  }

 done:
  REPORT_RESULT2( ret );

}

/*
 * Function: 	acl_get_mgr_types_sem_ok
 *
 * Abstract:    Retrieve the types of ACL's protecting an object.
 */
void
acl_get_mgr_types_sem_ok()
{
  char			*p_test_name = "acl_get_mgr_types_sem_ok";
  error_status_t	st;
  int			ret = 1, i;
  FILE			*acl_edit_hnd;
  unsigned char		*u_string, *acl_test_uuid_str;
  sec_acl_handle_t	hnd;
  unsigned32		num_mgr_types_used;
  unsigned32		num_mgr_types;
#define MAX_NUM_MGR_TYPES 64
  uuid_t		mgr_types[MAX_NUM_MGR_TYPES];
  uuid_t		loc_acl_test_uuid = INIT_ACL_TEST_UUID;
  sec_acl_posix_semantics_t	sems[MAX_NUM_MGR_TYPES];

  sec_acl_bind ( cds_target, FALSE, &hnd, &st );
  CHECK_DCE_ERROR ( "sec_acl_bind", st );

  sec_acl_get_mgr_types_semantics ( hnd, sec_acl_type_object, 
				    MAX_NUM_MGR_TYPES, &num_mgr_types_used,
				    &num_mgr_types, mgr_types, sems, &st );
  CHECK_DCE_ERROR ( "sec_acl_get_mgr_types_semantics", st );

  for ( i = 0; i < num_mgr_types_used; i++ )
  {
    if ( sems[i] != sec_acl_posix_no_semantics )
    {
      sprintf ( outbuf, "Mgr Semantics #%d not set to sec_acl_posix_no_semantics!", i );
      ret = 0;
      goto done;
    }
  }

  if ( num_mgr_types == 1 )
  {
    uuid_to_string ( &mgr_types[0], &u_string, &st );
    uuid_to_string ( &loc_acl_test_uuid, &acl_test_uuid_str, &st );
    if ( !strcmp ( u_string, acl_test_uuid_str ) )
      ret = 1;
    else
    {
      sprintf ( outbuf, "Uuids don't match (%s/%s)",u_string, acl_test_uuid_str );
      ret = 0;
    }
    rpc_string_free ( &u_string, &st );
    rpc_string_free ( &acl_test_uuid_str, &st );
  }
  else
  {
    sprintf ( outbuf, "Number of mgr types != 1." );
    ret = 0;
  }

 done:
  REPORT_RESULT2( ret );

}


#ifndef _USE_TET
main ()
{
  setup();
  acl_lookup_ok();
  acl_replace_ok();
  acl_get_access_ok();
  acl_test_access_allow();
  acl_test_access_denied();
/*  acl_test_access_unauth_allow();
  acl_test_access_unauth_denied(); */
  acl_get_printstring_ok();
  acl_get_mgr_types_ok();
  acl_get_mgr_types_sem_ok(); 
  cleanup();
}
#else
void (*tet_startup)() = setup;   /* Load the start routine into TET */
void (*tet_cleanup)() = cleanup; /* Load the clean routine into TET */

struct tet_testlist tet_testlist[] =
{
  { acl_lookup_ok, 1 },
  { acl_replace_ok, 1 },
  { acl_get_access_ok, 1 },
  { acl_test_access_allow, 1 },
  { acl_test_access_denied, 1 },
/*  { acl_test_access_unauth_allow, 1 },
  { acl_test_access_unauth_denied, 1 }, */
  { acl_get_printstring_ok, 1 },
  { acl_get_mgr_types_ok, 1 },
  { acl_get_mgr_types_sem_ok, 1 }, 
  { (void (*)())NULL, 0 }
};
#endif
