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
 * $Log: db_test_utils.h,v $
 * Revision 1.1.3.2  1996/02/18  23:03:51  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:28  marty]
 *
 * Revision 1.1.3.1  1995/12/08  22:07:28  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:42:55  root]
 * 
 * Revision 1.1.1.5  1994/06/10  20:18:39  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  16:58:00  devsrc]
 * 
 * Revision 1.1.1.4  1994/05/25  15:40:39  baum
 * 	Removed include of sys/mode.h for HP
 * 	[1994/05/25  15:40:07  baum]
 * 
 * Revision 1.1.1.3  1994/04/07  19:25:04  baum
 * 	Checkin in another set
 * 	[1994/04/07  19:24:51  baum]
 * 
 * Revision 1.1.1.2  1994/04/05  01:26:33  baum
 * 	Initial checkin
 * 
 * $EndLog$
 */
#ifndef __DB_TEST_UTILS_H__
#define __DB_TEST_UTILS_H__

#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <tet_api.h>
#include <dce/rpc.h>
#include <dce/dce_error.h>
#include <dce/dce.h>
#include <dce/dbif.h>
#include <dbtest.h>

#define LONG_STRING_LEN 	240
#define DB_NAME			"dbtest.db"
#define TEST_PASSED		"Passed"
#define DB_KEY1			"key1"
#define DB_TEST_INT_VAL1	98765
#define DB_TEST_CHAR_VAL1	"key1_data"
#define DB_TEST_NAME1		"name #1"
#define DB_TEST_REF_COUNT1	99
#define DB_NUM_RECORDS1		100

#define db_fvt_c_remove_before		1
#define db_fvt_c_close_after		2
#define db_fvt_c_remove_after		4
#define db_fvt_c_close_and_remove_after db_fvt_c_close_after | 		\
                                          db_fvt_c_remove_after

#define db_fvt_c_open_clean		db_fvt_c_remove_before |	\
                                          db_fvt_c_close_and_remove_after

extern unsigned char	dce_error_buf [dce_c_error_string_len];


#define CHECK_DCE_ERROR( m_api_name, m_dce_status )			\
  if ( m_dce_status != error_status_ok )				\
  {									\
    dce_error_inq_text ( m_dce_status, dce_error_buf, &inq_status );	\
    sprintf (result_buf, "API Failed (%s):%s", m_api_name, 		\
	     dce_error_buf);						\
    tet_return = TET_FAIL;						\
    goto done;								\
  }

#define CHECK_EXPECTED_STATUS( m_api_name, m_dce_status, m_exp_status ) \
  if ( m_dce_status != m_exp_status )					\
  {									\
    dce_error_inq_text ( m_dce_status, dce_error_buf, &inq_status );	\
    dce_error_inq_text ( m_exp_status, result_buf2, &inq_status );	\
    sprintf (result_buf, "Failed.  Expected: [%s:%s], received [%s:%s]",\
	     m_api_name, result_buf2, m_api_name, dce_error_buf );	\
    tet_return = TET_FAIL;						\
    goto done;								\
  }		

#define UNEXPECTED_OPEN_STATUS	 				    	\
    "Failed, Expected: [API Failed (dce_db_open: %s)], Received [%s]"

#define CHECK_OPEN_STATUS( m_dce_status, m_exp_status ) 		\
  if ( m_dce_status != m_exp_status )					\
  {									\
    dce_error_inq_text ( m_exp_status, dce_error_buf, &inq_status );	\
    sprintf (result_buf2, UNEXPECTED_OPEN_STATUS, dce_error_buf, 	\
	     result_buf);						\
    if ( tet_return == TET_PASS )					\
      tet_return = TET_FAIL;						\
    goto done;								\
  }

#endif
