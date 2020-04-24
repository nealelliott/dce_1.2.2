/*
 * @OSF_COPYRIGHT@
 * Copyright (c) 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: secval_status.c,v $
 * Revision 1.1.6.1  1996/10/03  18:39:45  arvind
 * 	Submit fix for CHFts19700
 * 	[1996/08/14  19:46 UTC  mullan_s  /main/mullan_dce122_bugs_1/1]
 *
 * 	Merge in all changes to dced tests for new ACLs.
 * 	[1996/01/15  22:56 UTC  jss  /main/jss_dced_acl/1]
 *
 * 	Fix order problem in cleanup code.
 * 	[1994/10/18  18:44:18  agd  1.1.2.2]
 *
 * Revision 1.1.4.2  1996/02/18  00:31:01  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:17:01  marty]
 * 
 * Revision 1.1.4.1  1995/12/11  15:19:30  root
 * 	Submit
 * 	[1995/12/11  14:33:26  root]
 * 
 * Revision 1.1.2.2  1994/10/18  18:44:18  agd
 * 	expand copyright
 * 	[1994/10/18  17:39:12  agd]
 * 
 * Revision 1.1.2.1  1994/09/30  21:15:46  baum
 * 	New secval tests plus the ability to run without TET (OT 11975)
 * 	[1994/09/30  21:14:46  baum]
 * 
 * $EndLog$
 */

/*
** Module:	secval_status.c
**
** FACILITY:	DCE Host Daemon API
**
** ABSTRACT:	This module implements the tests for dced api 
**		dced_secval_status 
**
*/

#include <dtapi_impl.h>

/*
 *  Global Declarations.
 */
dced_binding_handle_t   binding_handle;
boolean32		g_original_sv_state, g_setup_failed;

void setup(void);
void cleanup(void);
void dced_secval_status_1(void);          /* Test purposes 1 */
void dced_secval_status_2(void);          /* Test purposes 1 */

void (*tet_startup)(void) = setup;
void (*tet_cleanup)(void) = cleanup;

struct tet_testlist tet_testlist[] =
{
    { dced_secval_status_1,  1 },
    { dced_secval_status_2,  2 },
    { NULL, 0 }
};

/*
 * setup() - Called by TET once beofre any tests in module have run.
 */
void
setup()
{
  /*
   * . Create a global dced binding for convenience.
   * . Record the state of secval, start it if it isn't started.
   */

  error_status_t	st;
  
  g_setup_failed = FALSE;

  dced_util_binding_create( SECVAL_SVC, &binding_handle, &st);
  CHECK_SETUP_CMD( "dced_util_binding_create", st );
  
  dced_secval_status( binding_handle, &g_original_sv_state, &st );
  CHECK_SETUP_CMD( "dced_secval_status", st );

  if ( !g_original_sv_state ) {
    dced_secval_start( binding_handle, &st);
    CHECK_SETUP_CMD( "dced_secval_status", st );
  }

 cleanup:
  return;
}

/*
 * cleanup() - Called by TET once after all tests in module have completed
 */
void cleanup()
{
  error_status_t          st;

  /*
   * . Free global dced binding
   * . Reset secval state to original
   */
  if ( g_setup_failed )
    return;

  if ( !g_original_sv_state ) {
    dced_secval_stop( binding_handle, &st);
  }

  dced_binding_free( binding_handle, &st);
  CHECK_STATUS( "cleanup:dced_binding_free", st );

 cleanup:
  return;
}

/*
 * dced_secval_status_1() - Verify that of secval id enabled, secval_status
 *			    returns TRUE.	
 */
void 
dced_secval_status_1()
{

  error_status_t	st;
  boolean32		sv_active;
	
  /*
   * . Verify that secval_status returns TRUE.
   * . Restart secval
   */

  tet_infoline("Assertion: dced_secval_status_1");
  tet_infoline("Verify that if secval is enabled, secval_status" );
  tet_infoline("returns TRUE" );

  if ( g_setup_failed ) {
    tet_infoline("Due to a setup failure, test can not be run" );
    tet_result(TET_UNRESOLVED);
    return;
  }
    
  dced_secval_status( binding_handle, &sv_active, &st );
  CHECK_STATUS( "dced_secval_status", st );

  if ( !sv_active ) {
    tet_infoline("API dced_secval_status() returns FALSE");
    tet_result(TET_FAIL);
  }
  else {
    tet_infoline("API dced_secval_status() returns TRUE");
    tet_result(TET_PASS);
  }

 cleanup:

  return;
}

/*
 * dced_secval_status_2() - Verify that of secval id disabled, secval_status
 *			    returns FALSE.	
 */
void 
dced_secval_status_2()
{

  error_status_t	st;
  boolean32		sv_active;
	
  /*
   * . Stop secval
   * . Verify that secval_status returns FALSE.
   * . Restart secval
   */

  tet_infoline("Assertion: dced_secval_status_2:" );
  tet_infoline("Verify that if secval is disabled, secval_status" );
  tet_infoline("returns FALSE" );

  if ( g_setup_failed ) {
    tet_infoline("Due to a setup failure, test can not be run" );
    tet_result(TET_UNRESOLVED);
    return;
  }

  dced_secval_stop( binding_handle, &st);
  CHECK_STATUS( "dced_secval_stop", st );

  dced_secval_status( binding_handle, &sv_active, &st );
  CHECK_STATUS( "dced_secval_status", st );

  if ( sv_active ) {
    tet_infoline("API dced_secval_status() returns TRUE");
    tet_result(TET_FAIL);
  }
  else {
    tet_infoline("API dced_secval_status() returns FALSE");
    tet_result(TET_PASS);
  }

 cleanup:
  dced_secval_start( binding_handle, &st);
  if ( st != error_status_ok ) {
    tet_infoline ("PROBLEM:Could not restart secval for subsequent tests");
  }

  return;
}

