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
 * $Log: dce_db_open.c,v $
 * Revision 1.1.3.2  1996/02/18  00:23:46  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:11:13  marty]
 *
 * Revision 1.1.3.1  1995/12/08  22:07:50  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:43:06  root]
 * 
 * Revision 1.1.1.6  1994/06/10  20:18:46  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  16:58:07  devsrc]
 * 
 * Revision 1.1.1.5  1994/05/25  15:40:43  baum
 * 	Removed include of sys/mode.h for HP
 * 	[1994/05/25  15:40:10  baum]
 * 
 * Revision 1.1.1.4  1994/04/09  20:43:15  bowe
 * 	Account for new error return: db_s_open_already_exists
 * 	Do not pass create flag to dce_db_open 2nd time in eaccess test.
 * 	[1994/04/09  20:42:39  bowe]
 * 
 * Revision 1.1.1.3  1994/04/07  19:22:14  baum
 * 	Checkin again in another set
 * 	[1994/04/07  19:21:56  baum]
 * 
 * Revision 1.1.1.2  1994/04/05  01:22:38  baum
 * 	Initial checkin
 * 
 * $EndLog$
 */

/*
 * Module: 	dce_db_open.c
 *
 * Facility:	DCE Backing Store API
 *
 * Abstract:    This module implements the tests for the DCE Backing Store
 *		API.
 */

/*
 * Include files
 */
#include <db_test_utils.h>

/*
 * Globals
 */
static int		tet_return, ret;
static unsigned32	dce_status, inq_status, flags, control_flags;
static char		*p_test;
static dce_db_handle_t	db_h;
static unsigned char	result_buf[dce_c_error_string_len + LONG_STRING_LEN];
static unsigned char	result_buf2[ sizeof(result_buf) * 2];
static unsigned char	out_buf[ sizeof(result_buf) * 2];

unsigned char	dce_error_buf [dce_c_error_string_len];

void 
setup ()
{
}

void
cleanup()
{
}

/*
 * Function: 	db_open_create_name_ok
 *
 * Abstract:	Create a new backing store.  Indexed by name.
 */
void
db_open_create_name_ok ()
{
  
  p_test = "db_open_create_name_ok";

  flags = db_c_create | db_c_index_by_name;
  control_flags = db_fvt_c_open_clean;
  fvt_db_open ( control_flags, DB_NAME, NULL, flags, dbtest_data_convert, 
	        &db_h, &dce_status, result_buf, &tet_return );
  if ( tet_return == TET_PASS )
    sprintf ( result_buf, TEST_PASSED );

  sprintf ( out_buf, "Test %s: %s\n", p_test, result_buf );
#ifndef _USE_TET
  fprintf ( stderr, out_buf );
#else
  tet_infoline ( out_buf );
  tet_result ( tet_return );
#endif
  return;
}

/*
 * Function: 	db_open_create_uuid_ok
 *
 * Abstract:	Create a new backing store.  Indexed by uuid.
 */
void
db_open_create_uuid_ok ()
{
  
  p_test = "db_open_create_uuid_ok";

  flags = db_c_create | db_c_index_by_uuid;
  control_flags = db_fvt_c_open_clean;
  fvt_db_open ( control_flags, DB_NAME, NULL, flags, dbtest_data_convert, 
	        &db_h, &dce_status, result_buf, &tet_return );
  if ( tet_return == TET_PASS )
    sprintf ( result_buf, TEST_PASSED );

  sprintf ( out_buf, "Test %s: %s\n", p_test, result_buf );
#ifndef _USE_TET
  fprintf ( stderr, out_buf );
#else
  tet_infoline ( out_buf );
  tet_result ( tet_return );
#endif
  return;
}

/*
 * Function: 	db_open_create_already_exists
 *
 * Abstract:	Try to create a new backing store, where one already exists.
 */
void
db_open_create_already_exists ()
{
  
  p_test = "db_open_create_already_exists";

  /*
   * Setup: Create DB
   */
  flags = db_c_create | db_c_index_by_name;
  control_flags  = db_fvt_c_remove_before | db_fvt_c_close_after;
  fvt_db_open ( control_flags, DB_NAME, NULL, flags, dbtest_data_convert, 
	        &db_h, &dce_status, result_buf, &tet_return );
  if ( tet_return != TET_PASS )
  {
    sprintf ( result_buf2, "Setup Failed [%s]", result_buf );
    goto done;
  }
    
  /*
   * Assertion: We expect 'open_failed' if we try to create with
   * same name.  (This call will clean up also).
   */
  control_flags = db_fvt_c_close_and_remove_after;
  fvt_db_open ( control_flags, DB_NAME, NULL, flags, dbtest_data_convert, 
	        &db_h, &dce_status, result_buf, &tet_return );
  CHECK_OPEN_STATUS( dce_status, db_s_open_already_exists );

  sprintf ( result_buf2, TEST_PASSED );
  tet_return = TET_PASS;
  
 done:

  sprintf ( out_buf, "Test %s: %s\n", p_test, result_buf2 );
#ifndef _USE_TET
  fprintf ( stderr, out_buf );
#else
  tet_infoline ( out_buf );
  tet_result ( tet_return );
#endif
  return;
}

/*
 * Function: 	db_open_create_eacces
 *
 * Abstract:	Try to create a new backing store where one currently exists
 *		to which you do not have priviledge.
 */

void
db_open_create_eacces ()
{
  
  char		*p_error;
  int		ret;

  p_test = "db_open_create_eacces";
  sprintf ( result_buf2, TEST_PASSED );


  /*
   * Setup: Create DB and then change the perms so we can't access it
   */
  flags = db_c_create | db_c_index_by_name;
  control_flags = db_fvt_c_remove_before | db_fvt_c_close_after;
  fvt_db_open ( control_flags, DB_NAME, NULL, flags, dbtest_data_convert, 
	        &db_h, &dce_status, result_buf, &tet_return );

  if ( tet_return != TET_PASS )
  {
    sprintf ( result_buf2, "Setup Failed [%s]", result_buf );
    goto done;
  }

  ret = chmod ( DB_NAME, 0 );
  if ( ret == -1 )
  {
    p_error = strerror ( errno );
    sprintf ( result_buf2, "Couldn't change DB file perms (chmod: %s)\n", 
	      p_error );
    tet_return = TET_UNRESOLVED;
    goto done;
  }

  /*
   * Assertion; Attempt to create a DB of same name will now return
   * 'eacces'
   */
  flags = db_c_index_by_name;
  control_flags = db_fvt_c_close_after;
  fvt_db_open ( control_flags, DB_NAME, NULL, flags, dbtest_data_convert, 
	        &db_h, &dce_status, result_buf, &tet_return );
  CHECK_OPEN_STATUS( dce_status, db_s_open_failed_eacces );
  
  tet_return = TET_PASS;
  
 done:

  /* 
   * Cleanup: Flip back perms and open/remove DB.
   */
  ret = chmod ( DB_NAME, 0700 );
  if ( ret == -1 )
  {
    p_error = strerror ( errno );
    sprintf ( result_buf2, 
	      "Cleanup: Could not set DB file perms back (chmod:%s)\n", 
	      p_error );
    tet_return = TET_UNRESOLVED;
  }
  else
  {
    int	dummy_tet_status;

    flags = db_c_index_by_name;
    control_flags = db_fvt_c_close_and_remove_after;
    fvt_db_open ( control_flags, DB_NAME, NULL, flags, dbtest_data_convert, 
		 &db_h, &dce_status, result_buf, &dummy_tet_status );

  }

  sprintf ( out_buf, "Test %s: %s\n", p_test, result_buf2 );
#ifndef _USE_TET
  fprintf ( stderr, out_buf );
#else
  tet_infoline ( out_buf );
  tet_result ( tet_return );
#endif
  return;
}

/*
 * Function: 	db_open_create_bad_index_neither
 *
 * Abstract:	Try to open backing store, not indicating an index scheme.
 */
void
db_open_create_bad_index_neither ()
{

  p_test = "db_open_create_bad_index_neither";
  sprintf ( result_buf2, TEST_PASSED );

  flags = db_c_create;
  control_flags = db_fvt_c_open_clean;
  fvt_db_open ( control_flags, DB_NAME, NULL, flags, dbtest_data_convert, 
	        &db_h, &dce_status, result_buf, &tet_return );
  CHECK_OPEN_STATUS( dce_status, db_s_bad_index_type );

  tet_return = TET_PASS;

 done:

  sprintf ( out_buf, "Test %s: %s\n", p_test, result_buf2 );
#ifndef _USE_TET
  fprintf ( stderr, out_buf );
#else
  tet_infoline ( out_buf );
  tet_result ( tet_return );
#endif
  return;
}

/*
 * Function: 	db_open_create_bad_index_both
 *
 * Abstract:	Try to open backing store, not indicating an index scheme.
 */
void
db_open_create_bad_index_both ()
{

  p_test = "db_open_create_bad_index_both";
  sprintf ( result_buf2, TEST_PASSED );

  flags = db_c_create | db_c_index_by_name | db_c_index_by_uuid;
  control_flags = db_fvt_c_open_clean;
  fvt_db_open ( control_flags, DB_NAME, NULL, flags, dbtest_data_convert, 
	        &db_h, &dce_status, result_buf, &tet_return );
  CHECK_OPEN_STATUS( dce_status, db_s_bad_index_type );

  tet_return = TET_PASS;

 done:

  sprintf ( out_buf, "Test %s: %s\n", p_test, result_buf2 );
#ifndef _USE_TET
  fprintf ( stderr, out_buf );
#else
  tet_infoline ( out_buf );
  tet_result ( tet_return );
#endif
  return;
}

#ifndef _USE_TET
main ()
{
  db_open_create_name_ok();
  db_open_create_uuid_ok();
  db_open_create_already_exists ();
  db_open_create_eacces();
  db_open_create_bad_index_neither();
  db_open_create_bad_index_both();

}
#else
void (*tet_startup)() = setup;   /* Load the start routine into TET */
void (*tet_cleanup)() = cleanup; /* Load the clean routine into TET */

struct tet_testlist tet_testlist[] =
{
    { db_open_create_uuid_ok,  1 },
    { db_open_create_name_ok,  1 },
    { db_open_create_already_exists,  1 },
    { db_open_create_eacces,  1 },
    { db_open_create_bad_index_neither,  1 },
    { db_open_create_bad_index_both,  1 },
    { (void (*)())NULL, 0 }
};
#endif
