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
 * $Log: db_test_utils.c,v $
 * Revision 1.1.3.2  1996/02/18  00:23:34  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:11:04  marty]
 *
 * Revision 1.1.3.1  1995/12/08  22:07:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:42:54  root]
 * 
 * Revision 1.1.1.6  1994/06/10  20:18:38  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  16:57:59  devsrc]
 * 
 * Revision 1.1.1.5  1994/06/07  14:44:06  bowe
 * 	Added rpc_sm_enable_allocate(). [CR 10757]
 * 	[1994/06/07  14:40:31  bowe]
 * 
 * Revision 1.1.1.4  1994/05/19  14:53:35  baum
 * 	Resync with recent DB & ACL API changes
 * 	[1994/05/19  14:53:10  baum]
 * 
 * Revision 1.1.1.3  1994/04/07  19:22:09  baum
 * 	Checkin again in another set
 * 	[1994/04/07  19:21:51  baum]
 * 
 * Revision 1.1.1.2  1994/04/05  01:22:33  baum
 * 	Initial checkin
 * 
 * $EndLog$
 */
/*
 * Module: 	db_test_utils.c
 *
 * Facility:	DCE Backing Store API
 *
 * Abstract:    This module implements utility routines for the DCE 
 *		Backing Store API FVT's.
 */

/*
 * Include files
 */
#include <db_test_utils.h>

/*
 * Function: 	db_open_create_name_ok
 *
 * Abstract:	Create a new backing store.  Indexed by name.
 */

void
fvt_db_open 
(
 unsigned32		control_flags,
 const char		*p_dbname,
 const char		*p_backend_type,
 unsigned32		create_flags,
 dce_db_convert_func_t	convert_fcn,
 dce_db_handle_t	*p_db_h,
 error_status_t		*p_dce_status,
 char			*p_result_buf,
 unsigned32		*p_tet_result
)
{

  int			ret;
  unsigned32		loc_status, inq_status;
  dce_db_handle_t	db_h;
  char			*p_error;
  boolean		opened = FALSE;

  *p_tet_result = TET_UNRESOLVED;

  /* Setup */
  if ( ( control_flags & db_fvt_c_remove_before ) == db_fvt_c_remove_before )
  {
    if ( !access ( p_dbname, F_OK ) )
    {
      ret = unlink ( p_dbname );
      if ( ret == -1 )
      {
	p_error = strerror ( errno );
	sprintf ( p_result_buf, "Failed setup (unlink:%s)", p_error );
	*p_tet_result = TET_UNRESOLVED;
	goto done;
      }
    }
  }

  /* Create */
  *p_db_h = NULL;
  dce_db_open ( p_dbname,  p_backend_type, create_flags, convert_fcn,
	        p_db_h, p_dce_status );
  if ( *p_dce_status != error_status_ok )
  {
    dce_error_inq_text ( *p_dce_status, dce_error_buf, &inq_status );
    sprintf ( p_result_buf, "API Failed (dce_db_open: %s)", dce_error_buf );
    *p_tet_result = TET_FAIL;
    goto done;
  }
  sprintf ( p_result_buf, "API Passed (dce_db_open)" );

  /* db_store uses rpc_sm_allocate and rpc_sm_free */
  rpc_sm_enable_allocate( p_dce_status );
  
  opened = TRUE;
  /* E. File p_dbname should be created */
  if ( access ( p_dbname, F_OK ) )
  {
    sprintf ( p_result_buf, 
	      "dce_db_open (CREATE) returns OK but DB file not created." );
    *p_tet_result = TET_FAIL;
    goto done;
  }

  *p_tet_result = TET_PASS;

 done:

  /* C. Cleanup */
  if ( opened == TRUE &&
      (control_flags & db_fvt_c_close_after ) == db_fvt_c_close_after ) 
  {
    dce_db_close ( p_db_h, &loc_status );
    *p_db_h = NULL;
  }

  if ( (control_flags & db_fvt_c_remove_after ) == db_fvt_c_remove_after )
  {
    ret = unlink ( p_dbname );
    if ( ret == -1 )
    {
      p_error = strerror ( errno );
      sprintf ( p_result_buf, "Cleanup failed (unlink:%s)\n", p_error );
      *p_tet_result = TET_UNRESOLVED;
    }
  }

  return;
}
