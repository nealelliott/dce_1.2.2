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
 * $Log: dce_db_fetch.c,v $
 * Revision 1.1.3.2  1996/02/18  00:23:39  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:11:08  marty]
 *
 * Revision 1.1.3.1  1995/12/08  22:07:39  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:43:01  root]
 * 
 * Revision 1.1.1.4  1994/06/10  20:18:43  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  16:58:04  devsrc]
 * 
 * Revision 1.1.1.3  1994/04/07  19:22:12  baum
 * 	Checkin again in another set
 * 	[1994/04/07  19:21:54  baum]
 * 
 * Revision 1.1.1.2  1994/04/05  01:22:35  baum
 * 	Initial checkin
 * 
 * $EndLog$
 */

/*
 * Module: 	dce_db_fetch.c
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
static unsigned32	dce_status, inq_status, flags, control_flags;;
static char		*p_test;
static dce_db_handle_t	db_h;
static unsigned char	result_buf[dce_c_error_string_len + LONG_STRING_LEN];
static unsigned char	result_buf2[ sizeof(result_buf) * 2];
static unsigned char	out_buf[ sizeof(result_buf) * 2];

unsigned char		dce_error_buf [dce_c_error_string_len];
dce_db_header_t		null_header;

void 
setup ()
{
}

void
cleanup()
{
}

/*
 * Function: 	db_fetch_item
 *
 * Abstract:	Fetch an item from backing store.
 */
void
db_fetch_item ()
{

  int		dummy_tet_return;
  dbtest_t	test_data, test_data_out;

  p_test = "db_fetch_item";

  /*
   * Setup: Create a DB file, indexed by name.  Init the test data.
   */
  flags = db_c_create | db_c_index_by_name | db_c_acl_uuid_header;
  control_flags = db_fvt_c_remove_before;
  fvt_db_open ( control_flags, DB_NAME, NULL, flags, dbtest_data_convert, 
	        &db_h, &dce_status, result_buf2, &tet_return );
  if ( tet_return != TET_PASS )
  {
    sprintf ( result_buf, "Setup Failed [%s]", result_buf2 );
    goto done;
  }

  test_data.h = null_header;
  test_data.i = DB_TEST_INT_VAL1;
  test_data.p_ptr = malloc ( strlen ( DB_TEST_CHAR_VAL1 ) + 1 );
  strcpy ( test_data.p_ptr, DB_TEST_CHAR_VAL1 );

  /*
   * Assertion: Add a record, read it back and compare successfully.
   */
  dce_db_store ( db_h, DB_KEY1, &test_data, &dce_status );
  CHECK_DCE_ERROR ( "dce_db_store", dce_status );

  /*
   * E: Read data back and compare
   */
  dce_db_fetch ( db_h, DB_KEY1, (void *)&test_data_out, &dce_status );
  CHECK_DCE_ERROR ( "dce_db_fetch", dce_status );

  if ( (test_data_out.i != DB_TEST_INT_VAL1 ) ||
       strcmp ( test_data_out.p_ptr, DB_TEST_CHAR_VAL1 ) )
  {
    sprintf ( result_buf, "Failed. Retrieved data does not match input" );
    tet_return = TET_FAIL;
    goto done;
  }
  
  sprintf ( result_buf, TEST_PASSED );
  tet_return = TET_PASS;

 done:

  /*
   * Cleanup, close and delete.
   */
  dce_db_close ( &db_h, &dce_status );
  fvt_db_open ( db_fvt_c_open_clean, DB_NAME, NULL, flags, 
	        dbtest_data_convert, &db_h, &dce_status, 
	        result_buf2, &dummy_tet_return );

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
 * Function: 	db_fetch_key_name_bad_type
 *
 * Abstract:	Attempt to fetch an item from a backing store with the
 *		wrong index type.
 */
void
db_fetch_key_name_bad_type ()
{

  int		dummy_tet_return;
  dbtest_t	test_data, test_data_out;
  unsigned32	expected_status;

  p_test = "db_fetch_key_name_bad_type";

  /*
   * Setup: Create a DB file, indexed by uuid.
   */
  flags = db_c_create | db_c_index_by_uuid | db_c_acl_uuid_header;
  control_flags = db_fvt_c_remove_before;
  fvt_db_open ( control_flags, DB_NAME, NULL, flags, dbtest_data_convert, 
	        &db_h, &dce_status, result_buf2, &tet_return );
  if ( tet_return != TET_PASS )
  {
    sprintf ( result_buf, "Setup Failed [%s]", result_buf2 );
    goto done;
  }

  /*
   * Assertion: Fetch a record with wrong index type.  Failure.
   */
  expected_status = db_s_bad_index_type;
  dce_db_fetch_by_name ( db_h, DB_KEY1, &test_data, &dce_status );
  CHECK_EXPECTED_STATUS( "dce_db_fetch_by_name", dce_status, 
			 db_s_bad_index_type );
  
  sprintf ( result_buf, TEST_PASSED );
  tet_return = TET_PASS;

 done:

  /*
   * Cleanup, close and delete.
   */
  dce_db_close ( &db_h, &dce_status );
  fvt_db_open ( db_fvt_c_open_clean, DB_NAME, NULL, flags, 
	        dbtest_data_convert, &db_h, &dce_status, 
	        result_buf2, &dummy_tet_return );

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
 * Function: 	db_fetch_key_uuid_bad_type
 *
 * Abstract:	Attemopt to fetch an item with wrong index type.
 */
void
db_fetch_key_uuid_bad_type ()
{

  int		dummy_tet_return;
  dbtest_t	test_data, test_data_out;
  unsigned32	expected_status;
  uuid_t	uuid    = { /* 005b0c8e-767d-1d97-be90-0000c0dc0d4b */
        0x005b0c8e,
        0x767d,
        0x1d97,
        0xbe,
        0x90,
        {0x00, 0x00, 0xc0, 0xdc, 0x0d, 0x4b}
      };

  p_test = "db_fetch_key_uuid_bad_type";

  /*
   * Setup: Create a DB file, indexed by name.
   */
  flags = db_c_create | db_c_index_by_name | db_c_acl_uuid_header;
  control_flags = db_fvt_c_remove_before;
  fvt_db_open ( control_flags, DB_NAME, NULL, flags, dbtest_data_convert, 
	        &db_h, &dce_status, result_buf2, &tet_return );
  if ( tet_return != TET_PASS )
  {
    sprintf ( result_buf, "Setup Failed [%s]", result_buf2 );
    goto done;
  }

  /*
   * Assertion: Fetch a record with wrong index type.  Failure.
   */
  expected_status = db_s_bad_index_type;
  dce_db_fetch_by_uuid ( db_h, &uuid, &test_data, &dce_status );
  CHECK_EXPECTED_STATUS( "dce_db_fetch_by_uuid", dce_status, 
			 db_s_bad_index_type );
  
  sprintf ( result_buf, TEST_PASSED );
  tet_return = TET_PASS;

 done:

  /*
   * Cleanup, close and delete.
   */
  dce_db_close ( &db_h, &dce_status );
  fvt_db_open ( db_fvt_c_open_clean, DB_NAME, NULL, flags, 
	        dbtest_data_convert, &db_h, &dce_status, 
	        result_buf2, &dummy_tet_return );

  sprintf ( out_buf, "Test %s: %s\n", p_test, result_buf );
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
  db_fetch_item();
  db_fetch_key_name_bad_type();
  db_fetch_key_uuid_bad_type();
}
#else
void (*tet_startup)() = setup;   /* Load the start routine into TET */
void (*tet_cleanup)() = cleanup; /* Load the clean routine into TET */

struct tet_testlist tet_testlist[] =
{
    { db_fetch_item,  1 },
    { db_fetch_key_name_bad_type,  1 },
    { db_fetch_key_uuid_bad_type,  1 },
    { (void (*)())NULL, 0 }
};
#endif
