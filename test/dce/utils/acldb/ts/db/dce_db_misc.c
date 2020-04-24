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
 * $Log: dce_db_misc.c,v $
 * Revision 1.1.3.2  1996/02/18  00:23:43  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:11:11  marty]
 *
 * Revision 1.1.3.1  1995/12/08  22:07:46  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:43:05  root]
 * 
 * Revision 1.1.1.6  1994/09/16  17:24:25  bowe
 * 	Adjust lock/unlock tests - should not try db_store. Instead repeat
 * 	lock/unlock and check for expected error.  [CR 10676]
 * 	[1994/09/16  17:24:00  bowe]
 * 
 * Revision 1.1.1.5  1994/06/10  20:18:45  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  16:58:06  devsrc]
 * 
 * Revision 1.1.1.4  1994/05/19  14:53:36  baum
 * 	Resync with recent DB & ACL API changes
 * 	[1994/05/19  14:53:11  baum]
 * 
 * Revision 1.1.1.3  1994/04/06  16:32:17  baum
 * 	Another checkin, different set
 * 	[1994/04/06  16:31:53  baum]
 * 
 * Revision 1.1.1.2  1994/04/05  01:22:37  baum
 * 	Initial checkin
 * 
 * $EndLog$
 */

/*
 * Module: 	dce_db_misc.c
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
 * Function: 	db_header_init
 *
 * Abstract:	Ensure the desired fields in the header are set.
 */
void
db_header_init ()
{

  int		dummy_tet_return;
  dbtest_t	test_data;
  unsigned32	expected_result;
  char		*p_key;
  unsigned32	ref_count = DB_TEST_REF_COUNT1;
  char		*name = DB_TEST_NAME1;

  uuid_t	uuid  = { /* 00403bca-2191-1d9b-8cfa-0000c0dc0d4b */
        0x00403bca, 0x2191, 0x1d9b, 0x8c, 0xfa,
        {0x00, 0x00, 0xc0, 0xdc, 0x0d, 0x4b}
      };

  uuid_t	acl_uuid = { /* 003e8a15-21a6-1d9b-9815-0000c0dc0d4b */
        0x003e8a15, 0x21a6, 0x1d9b, 0x98, 0x15,
        {0x00, 0x00, 0xc0, 0xdc, 0x0d, 0x4b}
      };

  uuid_t def_obj_acl_uuid = { /* 00704a2c-bec8-1dd3-a1e0-0000c0dc0d4b */
    0x00704a2c, 0xbec8, 0x1dd3, 0xa1, 0xe0,
    { 0x00, 0x00, 0xc0, 0xdc, 0x0d, 0x4b }
  };

  uuid_t def_cont_acl_uuid = { /* 00669893-bf0e-1dd3-8c3a-0000c0dc0d4b */
    0x00669893, 0xbf0e, 0x1dd3, 0x8c, 0x3a,
    { 0x00, 0x00, 0xc0, 0xdc, 0x0d, 0x4b }
  };

  p_test = "db_header_init";

  /*
   * Setup: Create a DB file, indexed by name.
   */
  flags = db_c_create | db_c_index_by_name | db_c_std_header;
  control_flags = db_fvt_c_remove_before;
  fvt_db_open ( control_flags, DB_NAME, NULL, flags, dbtest_data_convert, 
	        &db_h, &dce_status, result_buf2, &tet_return );
  if ( tet_return != TET_PASS )
  {
    sprintf ( result_buf, "Setup Failed [%s]", result_buf2 );
    goto done;
  }

  /*
   * Assertion: We can set the header
   */
  dce_db_std_header_init ( db_h, &(test_data.h), &uuid, &acl_uuid,
			   &def_obj_acl_uuid, &def_cont_acl_uuid, ref_count, 
			   &dce_status );
  CHECK_DCE_ERROR ( "dce_db_std_header_init", dce_status );
  
  if ( (memcmp ( &test_data.h.tagged_union.h.uuid, &uuid, sizeof(uuid_t))) ||
       (memcmp ( &test_data.h.tagged_union.h.acl_uuid, &acl_uuid, 
		 sizeof(uuid_t) )) ||
       (memcmp ( &test_data.h.tagged_union.h.def_object_acl, 
		 &def_obj_acl_uuid, sizeof(uuid_t) )) ||
       (memcmp ( &test_data.h.tagged_union.h.def_container_acl, 
		 &def_cont_acl_uuid, sizeof(uuid_t) )) ||
       test_data.h.tagged_union.h.ref_count != DB_TEST_REF_COUNT1 )
  {
    sprintf(result_buf, "Failed. Retrieved data does not match input");
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
 * Function: 	db_header_init_bad_type
 *
 * Abstract:	Ensure the header_init can not be done on a non standard
 *		header.
 */
void
db_header_init_bad_type ()
{

  int		dummy_tet_return;
  dbtest_t	test_data;
  unsigned32	expected_result;
  char		*p_key;
  unsigned32	ref_count = DB_TEST_REF_COUNT1;
  char		*name = DB_TEST_NAME1;

  uuid_t	uuid  = { /* 00403bca-2191-1d9b-8cfa-0000c0dc0d4b */
        0x00403bca, 0x2191, 0x1d9b, 0x8c, 0xfa,
        {0x00, 0x00, 0xc0, 0xdc, 0x0d, 0x4b}
      };

  uuid_t	acl_uuid = { /* 003e8a15-21a6-1d9b-9815-0000c0dc0d4b */
        0x003e8a15, 0x21a6, 0x1d9b, 0x98, 0x15,
        {0x00, 0x00, 0xc0, 0xdc, 0x0d, 0x4b}
      };

  uuid_t def_obj_acl_uuid = { /* 00704a2c-bec8-1dd3-a1e0-0000c0dc0d4b */
    0x00704a2c, 0xbec8, 0x1dd3, 0xa1, 0xe0,
    { 0x00, 0x00, 0xc0, 0xdc, 0x0d, 0x4b }
  };

  uuid_t def_cont_acl_uuid = { /* 00669893-bf0e-1dd3-8c3a-0000c0dc0d4b */
    0x00669893, 0xbf0e, 0x1dd3, 0x8c, 0x3a,
    { 0x00, 0x00, 0xc0, 0xdc, 0x0d, 0x4b }
  };


  p_test = "db_header_init_bad_type";

  /*
   * Setup: Create a DB file, indexed by name.
   */
  flags = db_c_create | db_c_index_by_name;
  control_flags = db_fvt_c_remove_before;
  fvt_db_open ( control_flags, DB_NAME, NULL, flags, dbtest_data_convert, 
	        &db_h, &dce_status, result_buf2, &tet_return );
  if ( tet_return != TET_PASS )
  {
    sprintf ( result_buf, "Setup Failed [%s]", result_buf2 );
    goto done;
  }
  
  /*
   * Assertion: We can't set the header on a non-standard DB
   */
  dce_db_std_header_init ( db_h, &(test_data.h), &uuid, &acl_uuid,
			   &def_obj_acl_uuid, &def_cont_acl_uuid, ref_count, 
			   &dce_status );
  CHECK_EXPECTED_STATUS ( "dce_db_std_header_init", dce_status,
			  db_s_bad_header_type );

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
 * Function: 	db_lock
 *
 * Abstract:	Lock DB, attempt to add a record and get back a 'lock'
 *		status.
 */
void
db_lock ()
{

  int		dummy_tet_return;
  dbtest_t	test_data;
  unsigned32	expected_result;
  char		*p_key;
  unsigned32	ref_count = DB_TEST_REF_COUNT1;
  char		*name = DB_TEST_NAME1;

  uuid_t	uuid  = { /* 00403bca-2191-1d9b-8cfa-0000c0dc0d4b */
        0x00403bca, 0x2191, 0x1d9b, 0x8c, 0xfa,
        {0x00, 0x00, 0xc0, 0xdc, 0x0d, 0x4b}
      };

  uuid_t	acl_uuid = { /* 003e8a15-21a6-1d9b-9815-0000c0dc0d4b */
        0x003e8a15, 0x21a6, 0x1d9b, 0x98, 0x15,
        {0x00, 0x00, 0xc0, 0xdc, 0x0d, 0x4b}
      };

  uuid_t def_obj_acl_uuid = { /* 00704a2c-bec8-1dd3-a1e0-0000c0dc0d4b */
    0x00704a2c, 0xbec8, 0x1dd3, 0xa1, 0xe0,
    { 0x00, 0x00, 0xc0, 0xdc, 0x0d, 0x4b }
  };

  uuid_t def_cont_acl_uuid = { /* 00669893-bf0e-1dd3-8c3a-0000c0dc0d4b */
    0x00669893, 0xbf0e, 0x1dd3, 0x8c, 0x3a,
    { 0x00, 0x00, 0xc0, 0xdc, 0x0d, 0x4b }
  };


  dce_db_header_t	header;

  p_test = "db_lock";

  /*
   * Setup: Create a DB file, indexed by name.  Init the test data including
   * the header, lock the DB.
   */
  flags = db_c_create | db_c_index_by_name | db_c_std_header;
  control_flags = db_fvt_c_remove_before;
  fvt_db_open ( control_flags, DB_NAME, NULL, flags, dbtest_data_convert, 
	        &db_h, &dce_status, result_buf2, &tet_return );
  if ( tet_return != TET_PASS )
  {
    sprintf ( result_buf, "Setup Failed [%s]", result_buf2 );
    goto done;
  }

  dce_db_lock ( db_h, &dce_status );
  CHECK_DCE_ERROR( "dce_db_lock", dce_status );

  dce_db_lock ( db_h, &dce_status );
  CHECK_EXPECTED_STATUS ( "dce_db_lock", dce_status, db_s_already_locked );

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
 * Function: 	db_unlock
 *
 * Abstract:	Lock DB, then unlock.  Add record successfully.
 */
void
db_unlock ()
{

  int		dummy_tet_return;
  dbtest_t	test_data;
  unsigned32	expected_result;
  char		*p_key;
  unsigned32	ref_count = DB_TEST_REF_COUNT1;
  char		*name = DB_TEST_NAME1;

  uuid_t	uuid  = { /* 00403bca-2191-1d9b-8cfa-0000c0dc0d4b */
        0x00403bca, 0x2191, 0x1d9b, 0x8c, 0xfa,
        {0x00, 0x00, 0xc0, 0xdc, 0x0d, 0x4b}
      };

  uuid_t	acl_uuid = { /* 003e8a15-21a6-1d9b-9815-0000c0dc0d4b */
        0x003e8a15, 0x21a6, 0x1d9b, 0x98, 0x15,
        {0x00, 0x00, 0xc0, 0xdc, 0x0d, 0x4b}
      };

  uuid_t def_obj_acl_uuid = { /* 00704a2c-bec8-1dd3-a1e0-0000c0dc0d4b */
    0x00704a2c, 0xbec8, 0x1dd3, 0xa1, 0xe0,
    { 0x00, 0x00, 0xc0, 0xdc, 0x0d, 0x4b }
  };

  uuid_t def_cont_acl_uuid = { /* 00669893-bf0e-1dd3-8c3a-0000c0dc0d4b */
    0x00669893, 0xbf0e, 0x1dd3, 0x8c, 0x3a,
    { 0x00, 0x00, 0xc0, 0xdc, 0x0d, 0x4b }
  };

  dce_db_header_t	header;

  p_test = "db_unlock";

  /*
   * Setup: Create a DB file, indexed by name.  Init the test data including
   * the header, lock the DB then unlock it.
   */
  flags = db_c_create | db_c_index_by_name | db_c_std_header;
  control_flags = db_fvt_c_remove_before;
  fvt_db_open ( control_flags, DB_NAME, NULL, flags, dbtest_data_convert, 
	        &db_h, &dce_status, result_buf2, &tet_return );
  if ( tet_return != TET_PASS )
  {
    sprintf ( result_buf, "Setup Failed [%s]", result_buf2 );
    goto done;
  }

  dce_db_lock ( db_h, &dce_status );
  CHECK_DCE_ERROR( "dce_db_lock", dce_status );

  dce_db_unlock ( db_h, &dce_status );
  CHECK_DCE_ERROR( "dce_db_lock", dce_status );

  dce_db_unlock ( db_h, &dce_status );
  CHECK_EXPECTED_STATUS ( "dce_db_unlock", dce_status, db_s_not_locked );

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
 * Function: 	db_header_fetch
 *
 * Abstract:	Retrieve header set by set_header.
 */
void
db_header_fetch ()
{

  int		dummy_tet_return;
  dbtest_t	test_data;
  unsigned32	expected_result;
  char		*p_key;
  unsigned32	ref_count = DB_TEST_REF_COUNT1;
  char		*name = DB_TEST_NAME1;

  uuid_t	uuid  = { /* 00403bca-2191-1d9b-8cfa-0000c0dc0d4b */
        0x00403bca,
        0x2191,
        0x1d9b,
        0x8c,
        0xfa,
        {0x00, 0x00, 0xc0, 0xdc, 0x0d, 0x4b}
      };

  uuid_t	acl_uuid = { /* 003e8a15-21a6-1d9b-9815-0000c0dc0d4b */
        0x003e8a15,
        0x21a6,
        0x1d9b,
        0x98,
        0x15,
        {0x00, 0x00, 0xc0, 0xdc, 0x0d, 0x4b}
      };

  uuid_t def_obj_acl_uuid = { /* 00704a2c-bec8-1dd3-a1e0-0000c0dc0d4b */
    0x00704a2c, 0xbec8, 0x1dd3, 0xa1, 0xe0,
    { 0x00, 0x00, 0xc0, 0xdc, 0x0d, 0x4b }
  };

  uuid_t def_cont_acl_uuid = { /* 00669893-bf0e-1dd3-8c3a-0000c0dc0d4b */
    0x00669893, 0xbf0e, 0x1dd3, 0x8c, 0x3a,
    { 0x00, 0x00, 0xc0, 0xdc, 0x0d, 0x4b }
  };

  dce_db_header_t	header;

  p_test = "db_header_fetch";

  /*
   * Setup: Create a DB file, indexed by name.  Init the test data including
   * the header and successfully add a record.
   */
  flags = db_c_create | db_c_index_by_name | db_c_std_header;
  control_flags = db_fvt_c_remove_before;
  fvt_db_open ( control_flags, DB_NAME, NULL, flags, dbtest_data_convert, 
	        &db_h, &dce_status, result_buf2, &tet_return );
  if ( tet_return != TET_PASS )
  {
    sprintf ( result_buf, "Setup Failed [%s]", result_buf2 );
    goto done;
  }

  test_data.i = DB_TEST_INT_VAL1;
  test_data.p_ptr = malloc ( strlen ( DB_TEST_CHAR_VAL1 ) + 1 );
  strcpy ( test_data.p_ptr, DB_TEST_CHAR_VAL1 );
  test_data.h = null_header;

  dce_db_std_header_init ( db_h, &(test_data.h), &uuid, &acl_uuid,
			   &def_obj_acl_uuid, &def_cont_acl_uuid, ref_count, 
			   &dce_status );
  CHECK_DCE_ERROR ( "dce_db_std_header_init", dce_status );

  dce_db_store ( db_h, DB_KEY1, &test_data, &dce_status );
  CHECK_DCE_ERROR ( "dce_db_store", dce_status );

  /*
   * Assertion: Retrieve header successfully
   */
  header = null_header;
  dce_db_header_fetch ( db_h, DB_KEY1, &header, &dce_status );
  CHECK_DCE_ERROR ( "dce_db_header_fetch", dce_status );

  if ( (memcmp ( &test_data.h.tagged_union.h.uuid, &uuid, sizeof(uuid_t))) ||
       (memcmp ( &test_data.h.tagged_union.h.acl_uuid, &acl_uuid, 
		 sizeof(uuid_t) )) ||
       (memcmp ( &test_data.h.tagged_union.h.def_object_acl, 
		 &def_obj_acl_uuid, sizeof(uuid_t) )) ||
       (memcmp ( &test_data.h.tagged_union.h.def_container_acl, 
		 &def_cont_acl_uuid, sizeof(uuid_t) )) ||
       test_data.h.tagged_union.h.ref_count != DB_TEST_REF_COUNT1 )
  {
    sprintf(result_buf, "Failed. Retrieved data does not match input");
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
 * Function: 	db_inq_count_zero
 *
 * Abstract:	Retrieve header set by set_header.
 */
void
db_inq_count_zero ()
{

  unsigned32 	count, dummy_tet_return;
  dbtest_t	test_data;
  int		i;

  p_test = "db_inq_count_zero";

  /*
   * Setup: Create a DB file, indexed by name.
   */
  flags = db_c_create | db_c_index_by_name | db_c_std_header;
  control_flags = db_fvt_c_remove_before;
  fvt_db_open ( control_flags, DB_NAME, NULL, flags, dbtest_data_convert, 
	        &db_h, &dce_status, result_buf2, &tet_return );
  if ( tet_return != TET_PASS )
  {
    sprintf ( result_buf, "Setup Failed [%s]", result_buf2 );
    goto done;
  }

  /*
   * Count, should return zero
   */
  dce_db_inq_count ( db_h, &count, &dce_status );
  CHECK_DCE_ERROR ( "dce_db_inq_count", dce_status );

  if ( count != 0 )
  {
    sprintf(result_buf, "Failed. Count (%d) not equal zero", count );
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
 * Function: 	db_inq_count_not_zero
 *
 * Abstract:	Retrieve header set by set_header.
 */
void
db_inq_count_not_zero ()
{

  unsigned32 	count, dummy_tet_return;
  dbtest_t	test_data;
  int		i;
  char		key[80];
  unsigned32	ref_count = DB_TEST_REF_COUNT1;
  char		*name = DB_TEST_NAME1;

  uuid_t	uuid  = { /* 00403bca-2191-1d9b-8cfa-0000c0dc0d4b */
        0x00403bca,
        0x2191,
        0x1d9b,
        0x8c,
        0xfa,
        {0x00, 0x00, 0xc0, 0xdc, 0x0d, 0x4b}
      };

  uuid_t	acl_uuid = { /* 003e8a15-21a6-1d9b-9815-0000c0dc0d4b */
        0x003e8a15,
        0x21a6,
        0x1d9b,
        0x98,
        0x15,
        {0x00, 0x00, 0xc0, 0xdc, 0x0d, 0x4b}
      };

  uuid_t def_obj_acl_uuid = { /* 00704a2c-bec8-1dd3-a1e0-0000c0dc0d4b */
    0x00704a2c, 0xbec8, 0x1dd3, 0xa1, 0xe0,
    { 0x00, 0x00, 0xc0, 0xdc, 0x0d, 0x4b }
  };

  uuid_t def_cont_acl_uuid = { /* 00669893-bf0e-1dd3-8c3a-0000c0dc0d4b */
    0x00669893, 0xbf0e, 0x1dd3, 0x8c, 0x3a,
    { 0x00, 0x00, 0xc0, 0xdc, 0x0d, 0x4b }
  };

  p_test = "db_inq_count_not_zero";

  /*
   * Setup: Create a DB file, indexed by name.  Add n records.
   */
  flags = db_c_create | db_c_index_by_name | db_c_std_header;
  control_flags = db_fvt_c_remove_before;
  fvt_db_open ( control_flags, DB_NAME, NULL, flags, dbtest_data_convert, 
	        &db_h, &dce_status, result_buf2, &tet_return );
  if ( tet_return != TET_PASS )
  {
    sprintf ( result_buf, "Setup Failed [%s]", result_buf2 );
    goto done;
  }

  test_data.i = DB_TEST_INT_VAL1;
  test_data.p_ptr = malloc ( strlen ( DB_TEST_CHAR_VAL1 ) + 1 );
  strcpy ( test_data.p_ptr, DB_TEST_CHAR_VAL1 );
  test_data.h = null_header;

  dce_db_std_header_init ( db_h, &(test_data.h), &uuid, &acl_uuid,
			   &def_obj_acl_uuid, &def_cont_acl_uuid, ref_count, 
			   &dce_status );
  CHECK_DCE_ERROR ( "dce_db_std_header_init", dce_status );

  for ( i = 0; i < DB_NUM_RECORDS1; i++ )
  {
    sprintf ( key, "key_name%d", i );
    dce_db_store ( db_h, key, &test_data, &dce_status );
    CHECK_DCE_ERROR ( "dce_db_store", dce_status );
  }

  /*
   * Count, should return DB_NUM_RECORDS1
   */
  dce_db_inq_count ( db_h, &count, &dce_status );
  CHECK_DCE_ERROR ( "dce_db_inq_count", dce_status );

  if ( count != DB_NUM_RECORDS1 )
  {
    sprintf( result_buf, "Failed. Count (%d) not equal %d", count,
	     DB_NUM_RECORDS1 );
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

#ifndef _USE_TET
main ()
{
  db_header_init();
  db_header_init_bad_type();
  db_header_fetch();
  db_inq_count_zero();
  db_inq_count_not_zero();
  db_unlock();
  db_lock();

}
#else
void (*tet_startup)() = setup;   /* Load the start routine into TET */
void (*tet_cleanup)() = cleanup; /* Load the clean routine into TET */

struct tet_testlist tet_testlist[] =
{
    { db_header_init,  1 },
    { db_header_init_bad_type,  1 },
    { db_header_fetch,  1 },
    { db_unlock,  1 },
    { db_lock,  1 },
    { db_inq_count_zero,  1 },
    { db_inq_count_not_zero,  1 },
    { (void (*)())NULL, 0 }
};
#endif
