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
 * $Log: dce_db_close.c,v $
 * Revision 1.1.3.2  1996/02/18  00:23:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:11:06  marty]
 *
 * Revision 1.1.3.1  1995/12/08  22:07:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:42:58  root]
 * 
 * Revision 1.1.1.4  1994/06/10  20:18:41  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  16:58:03  devsrc]
 * 
 * Revision 1.1.1.3  1994/04/07  19:22:11  baum
 * 	Checkin again in another set
 * 	[1994/04/07  19:21:52  baum]
 * 
 * Revision 1.1.1.2  1994/04/05  01:22:34  baum
 * 	Initial checkin
 * 
 * $EndLog$
 */

/*
 * Module: 	dce_db_close.c
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

void 
setup ()
{
}


void
cleanup()
{
}

/*
 * Function: 	db_close
 *
 * Abstract:	Create a new backing store.  Indexed by name.
 */
void
db_close ()
{

  int	dummy_tet_return;
  
  p_test = "db_close";

  /*
   * Setup: Create a DB file
   */
  flags = db_c_create | db_c_index_by_name;
  control_flags = db_fvt_c_remove_before;
  fvt_db_open ( control_flags, DB_NAME, NULL, flags, dbtest_data_convert, 
	        &db_h, &dce_status, result_buf, &tet_return );
  if ( tet_return != TET_PASS )
  {
    sprintf ( result_buf2, "Setup Failed [%s]", result_buf );
    goto done;
  }

  /*
   * Assertion: Close it successfully
   */
  dce_db_close ( &db_h, &dce_status );
  if ( db_h != NULL )
  {
    tet_return = TET_FAIL;
    sprintf ( result_buf2, "Failed.  DB handle no NULL after close" );
  }
  else
  {
    tet_return = TET_PASS;
    sprintf ( result_buf2, TEST_PASSED );
  }

 done:

  /*
   * Cleanup
   */
  fvt_db_open ( db_fvt_c_open_clean, DB_NAME, NULL, flags, 
	        dbtest_data_convert, &db_h, &dce_status, 
	        result_buf, &dummy_tet_return );
 
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
  db_close();
}
#else
void (*tet_startup)() = setup;   /* Load the start routine into TET */
void (*tet_cleanup)() = cleanup; /* Load the clean routine into TET */

struct tet_testlist tet_testlist[] =
{
    { db_close,  1 },
    { (void (*)())NULL, 0 }
};
#endif
