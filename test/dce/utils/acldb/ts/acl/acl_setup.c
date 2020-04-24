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
 * $Log: acl_setup.c,v $
 * Revision 1.1.5.2  1996/02/18  00:23:17  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:10:50  marty]
 *
 * Revision 1.1.5.1  1995/12/08  22:06:40  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:42:36  root]
 * 
 * Revision 1.1.3.2  1994/06/17  18:45:33  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:34:48  devsrc]
 * 
 * Revision 1.1.3.1  1994/05/19  14:34:48  baum
 * 	Resync tests with ACL & DB API changes
 * 	[1994/05/19  14:34:11  baum]
 * 
 * $EndLog$
 */

/*
 * Module: 	setup.c
 *
 * Facility:	DCE ACL Setup Routine
 *
 * Abstract:    This module implements the setup functions for the DCE ACL 
 *		FVT's.
 */

/*
 * Include files
 */
#include <acl_test_utils.h>
#include <sys/utsname.h>


void 
setup ()
{
  unsigned32 	status;

  tst_dce_login ( CELL_ADMIN, CELL_ADMIN_PWD, &status );
  if ( status != error_status_ok )
  {
    OUTPUT ( "Could not dce_login as cell_admin\n", -1 );
  }
}

void
cleanup()
{
}

void
add_principals()
{
  int 	ret;
  char	add_princ_cmd[LONG_STRING_LEN];

  sprintf ( add_princ_cmd, "./add_principals.tcl %s %s %s",
	    ACL_TEST_PRINC1, ACL_TEST_PRINC1, CELL_ADMIN_PWD );
  ret = system ( add_princ_cmd );
  OUTPUT ( "add_principals: Done\n", TET_PASS );
}

void
create_keytab()
{
  int ret;
  char	keytab_cmd[LONG_STRING_LEN];

  sprintf ( keytab_cmd, "echo kta -p %s -pw %s -f %s | rgy_edit", 
	    ACL_TEST_PRINC1, ACL_TEST_PRINC1, KEYTAB_FILE );

  ret = system ( keytab_cmd );
  OUTPUT ( "create_keytab: Done\n", TET_PASS );
}

void
acl_cds_entries()
{
  int 		ret;
  char		acl_edit_cmd[LONG_STRING_LEN];
  struct	utsname  info;

  uname ( &info );
  sprintf ( acl_edit_cmd, "acl_edit /.:/hosts/%s -m any_other:rwdtcia",
	    info.nodename );
  
  ret = system ( acl_edit_cmd );
  OUTPUT ( "acl_cds_entries: Done\n", TET_PASS );
}

#ifndef _USE_TET
main ()
{
  setup ();
  add_principals();
  create_keytab();
  acl_cds_entries();
  cleanup();
}
#else
void (*tet_startup)() = setup;   /* Load the start routine into TET */
void (*tet_cleanup)() = cleanup; /* Load the clean routine into TET */

struct tet_testlist tet_testlist[] =
{
  { add_principals, 1 },
  { create_keytab, 1 },
  { acl_cds_entries, 1 },
  { (void (*)())NULL, 0 }
};
#endif
