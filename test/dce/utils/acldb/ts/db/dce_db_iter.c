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
 * $Log: dce_db_iter.c,v $
 * Revision 1.1.3.2  1996/02/18  00:23:41  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:11:10  marty]
 *
 * Revision 1.1.3.1  1995/12/08  22:07:42  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:43:04  root]
 * 
 * Revision 1.1.1.5  1994/06/22  20:56:56  baum
 * 	Added new test for iterate and fetch
 * 	[1994/06/22  20:56:29  baum]
 * 
 * Revision 1.1.1.4  1994/06/10  20:18:43  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  16:58:05  devsrc]
 * 
 * Revision 1.1.1.3  1994/04/07  19:22:13  baum
 * 	Checkin again in another set
 * 	[1994/04/07  19:21:55  baum]
 * 
 * Revision 1.1.1.2  1994/04/05  01:22:36  baum
 * 	Initial checkin
 * 
 * $EndLog$
 */

/*
 * Module: 	dce_db_iter.c
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

unsigned char	dce_error_buf [dce_c_error_string_len];
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
 * Function: 	db_iter_ok
 *
 * Abstract:	Be sure iter works OK.
 */
void
db_iter_ok ()
{
#define NUM_KEYS 100
  int		dummy_tet_return;
  dbtest_t	test_data, test_data_out;
  unsigned32	expected_result;
  char		*p_key[NUM_KEYS];
  char		*p_data[NUM_KEYS];
  int		found[NUM_KEYS];
  char		*p_key_out;
  int		i, j;

  p_test = "db_iter_ok";

  /*
   * Setup: Create a DB file, indexed by name, store NUM_KEYS keys in it.
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

  for ( i = 0; i < NUM_KEYS; i++ )
  {
    found[i] = 0;
    test_data.h = null_header;
    test_data.i = DB_TEST_INT_VAL1;

    p_data[i] = malloc ( LONG_STRING_LEN );
    sprintf ( p_data[i], "Data Item #%d", i );
    test_data.p_ptr = p_data[i];

    p_key[i] =  malloc ( LONG_STRING_LEN );
    sprintf ( p_key[i], "Key #%d", i );
    
    dce_db_store ( db_h, p_key[i], &test_data, &dce_status );
    CHECK_DCE_ERROR ( "dce_db_store", dce_status );
  }


  /*
   * ASSERTION: We can find all of the keys
   */
  dce_db_iter_start ( db_h, &dce_status );
  CHECK_DCE_ERROR ( "dce_db_iter_start", dce_status );

  for ( i = 0;; i++ )
  {
    dce_db_iter_next ( db_h, (void **)&p_key_out, &dce_status );
    if ( dce_status == db_s_no_more )
    {
      if ( i != NUM_KEYS )
      {
	sprintf ( result_buf, "Wrote %d keys, read back %d", 
		  NUM_KEYS, i );
	tet_return = TET_FAIL;
	goto done;
      }
      break;
    }
    CHECK_DCE_ERROR ( "dce_db_iter_next", dce_status );

    /*
     * Make sure key is valid
     */
    for ( j = 0; j < NUM_KEYS; j++ )
    {
      if ( !strcmp ( p_key_out, p_key[j] ) )
      {
	break;
      }
    }

    if ( found[j] != 0 )
    {
      sprintf ( result_buf, "Key read back twice (%s)", p_key_out );
      tet_return = TET_FAIL;
      goto done;
    }

    found[j] = 1;

    if ( j == NUM_KEYS )
    {
      sprintf ( result_buf, "Key read back which was not stored (%s)", p_key_out );
      tet_return = TET_FAIL;
      goto done;
    }

    dce_db_fetch ( db_h, p_key_out, (void *)&test_data_out, &dce_status );
    CHECK_DCE_ERROR ( "dce_db_fetch", dce_status );
    if ( strcmp ( p_data[j], test_data_out.p_ptr ) )
    {
      sprintf ( result_buf, "Key (%s) has incorrect data (%s)", p_key_out, 
	        test_data_out.p_ptr );
      tet_return = TET_FAIL;
      goto done;
    }

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
 * Function: 	db_iter_next_first
 *
 * Abstract:	Be sure function can get a key.
 */
void
db_iter_next_first ()
{

  int		dummy_tet_return;
  dbtest_t	test_data, test_data_out;
  unsigned32	expected_result;
  char		*p_key;

  p_test = "db_iter_next_first";

  /*
   * Setup: Create a DB file, indexed by name.  Init the test data and
   * successfully add a record.
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

  dce_db_store ( db_h, DB_KEY1, &test_data, &dce_status );
  CHECK_DCE_ERROR ( "dce_db_store", dce_status );
  
  dce_db_fetch ( db_h, DB_KEY1, (void *)&test_data_out, &dce_status );
  CHECK_DCE_ERROR ( "dce_db_fetch", dce_status );

  if ( (test_data_out.i != DB_TEST_INT_VAL1 ) ||
       strcmp ( test_data_out.p_ptr, DB_TEST_CHAR_VAL1 ) )
  {
    sprintf(result_buf, "Setup Failed. Retrieved data does not match input");
    tet_return = TET_FAIL;
    goto done;
  }

  /*
   * Assertion: We can get the same record back with an iterator.
   */
  dce_db_iter_start ( db_h, &dce_status );
  CHECK_DCE_ERROR ( "dce_db_iter_start", dce_status );

  dce_db_iter_next ( db_h, (void **)&p_key, &dce_status );
  CHECK_DCE_ERROR ( "dce_db_iter_next", dce_status );

  if ( strcmp ( p_key, DB_KEY1 ) )
  {
    sprintf ( result_buf,  "Failed.  Retrieved key (%s) does not match stored key (%s", p_key, DB_KEY1 );
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
 * Function: 	db_iter_next_end
 *
 * Abstract:	Be sure function can detect end of a list of items.
 */
void
db_iter_next_end ()
{

  int		dummy_tet_return;
  dbtest_t	test_data, test_data_out;
  unsigned32	expected_result;
  char		*p_key;

  p_test = "db_iter_next_end";

  /*
   * Setup: Create a DB file, indexed by name.  Init the test data and
   * successfully add a record.  Then next over it so that the next next
   * will return EOF.
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

  dce_db_store ( db_h, DB_KEY1, &test_data, &dce_status );
  CHECK_DCE_ERROR ( "dce_db_store", dce_status );
  
  dce_db_fetch ( db_h, DB_KEY1, (void *)&test_data_out, &dce_status );
  CHECK_DCE_ERROR ( "dce_db_fetch", dce_status );

  if ( (test_data_out.i != DB_TEST_INT_VAL1 ) ||
       strcmp ( test_data_out.p_ptr, DB_TEST_CHAR_VAL1 ) )
  {
    sprintf(result_buf, "Setup Failed. Retrieved data does not match input");
    tet_return = TET_FAIL;
    goto done;
  }

  dce_db_iter_start ( db_h, &dce_status );
  CHECK_DCE_ERROR ( "dce_db_iter_start", dce_status );

  dce_db_iter_next ( db_h, (void **)&p_key, &dce_status );
  CHECK_DCE_ERROR ( "dce_db_iter_next", dce_status );

  dce_db_iter_next ( db_h, (void **)&p_key, &dce_status );
  CHECK_EXPECTED_STATUS ( "dce_db_iter_next", dce_status, db_s_no_more );

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
 * Function: 	db_iter_next_empty
 *
 * Abstract:	Be sure function can detect end in empty DB.
 */
void
db_iter_next_empty ()
{

  int		dummy_tet_return;
  dbtest_t	test_data, test_data_out;
  unsigned32	expected_result;
  char		*p_key;

  p_test = "db_iter_next_empty";

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

  dce_db_iter_start ( db_h, &dce_status );
  CHECK_DCE_ERROR ( "dce_db_iter_start", dce_status );

  dce_db_iter_next ( db_h, (void **)&p_key, &dce_status );
  CHECK_EXPECTED_STATUS ( "dce_db_iter_next", dce_status, db_s_no_more );

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
 * Function: 	db_iter_next_end_again
 *
 * Abstract:	Be sure function can detect end again & again.
 */
void
db_iter_next_end_again ()
{

  int		dummy_tet_return;
  dbtest_t	test_data, test_data_out;
  unsigned32	expected_result;
  char		*p_key;
  int		i;

  p_test = "db_iter_next_end_again";

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

  dce_db_iter_start ( db_h, &dce_status );
  CHECK_DCE_ERROR ( "dce_db_iter_start", dce_status );

  for ( i = 0; i < 10; i++ )
  {
    dce_db_iter_next ( db_h, (void **)&p_key, &dce_status );
    CHECK_EXPECTED_STATUS ( "dce_db_iter_next", dce_status, db_s_no_more );
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
 * Function: 	db_iter_next_name_bad_index
 *
 * Abstract:	Iterate with wrong index type.  Returns error.
 */
 void
db_iter_next_name_bad_index ()
{

  int		dummy_tet_return;
  dbtest_t	test_data, test_data_out;
  unsigned32	expected_result;
  char		*p_key;
  int		i;

  p_test = "db_iter_next_name_bad_index";

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

  test_data.h = null_header;
  test_data.i = DB_TEST_INT_VAL1;
  test_data.p_ptr = malloc ( strlen ( DB_TEST_CHAR_VAL1 ) + 1 );
  strcpy ( test_data.p_ptr, DB_TEST_CHAR_VAL1 );

  dce_db_iter_start ( db_h, &dce_status );
  CHECK_DCE_ERROR ( "dce_db_iter_start", dce_status );

  dce_db_iter_next_by_name ( db_h, &p_key, &dce_status );
  CHECK_EXPECTED_STATUS ( "dce_db_iter_next_by_name", dce_status, 
			  db_s_bad_index_type);

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
 * Function: 	db_iter_next_uuid_bad_index
 *
 * Abstract:	Iterate with wrong index type.  Returns error.
 */
 void
db_iter_next_uuid_bad_index ()
{

  int		dummy_tet_return;
  dbtest_t	test_data, test_data_out;
  unsigned32	expected_result;
  uuid_t	*p_uuid;
  int		i;

  p_test = "db_iter_next_uuid_bad_index";

  /*
   * Setup: Create a DB file, indexed by name.  Add a key
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

  dce_db_store ( db_h, DB_KEY1, &test_data, &dce_status );
  CHECK_DCE_ERROR ( "dce_db_store", dce_status );
  
  dce_db_fetch ( db_h, DB_KEY1, (void *)&test_data_out, &dce_status );
  CHECK_DCE_ERROR ( "dce_db_fetch", dce_status );

  if ( (test_data_out.i != DB_TEST_INT_VAL1 ) ||
       strcmp ( test_data_out.p_ptr, DB_TEST_CHAR_VAL1 ) )
  {
    sprintf(result_buf, "Setup Failed. Retrieved data does not match input");
    tet_return = TET_FAIL;
    goto done;
  }

  dce_db_iter_start ( db_h, &dce_status );
  CHECK_DCE_ERROR ( "dce_db_iter_start", dce_status );

  dce_db_iter_next_by_uuid ( db_h, &p_uuid, &dce_status );
  CHECK_EXPECTED_STATUS ( "dce_db_iter_next_by_uuid", dce_status, 
			  db_s_bad_index_type);

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
  db_iter_ok();
  db_iter_next_first();
  db_iter_next_end();
  db_iter_next_empty();
  db_iter_next_end_again();
  db_iter_next_name_bad_index();
  db_iter_next_uuid_bad_index();
}
#else
void (*tet_startup)() = setup;   /* Load the start routine into TET */
void (*tet_cleanup)() = cleanup; /* Load the clean routine into TET */

struct tet_testlist tet_testlist[] =
{
    { db_iter_ok,  1 },
    { db_iter_next_first,  1 },
    { db_iter_next_end,  1 },
    { db_iter_next_empty,  1 },
    { db_iter_next_end_again,  1 },
    { db_iter_next_name_bad_index,  1 },
    { db_iter_next_uuid_bad_index,  1 },
    { (void (*)())NULL, 0 }
};
#endif
