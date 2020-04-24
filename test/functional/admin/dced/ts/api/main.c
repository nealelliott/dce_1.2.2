/*
 * @OSF_COPYRIGHT@
 * Copyright (c) 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: main.c,v $
 * Revision 1.1.4.2  1996/02/18  00:30:34  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:16:44  marty]
 *
 * Revision 1.1.4.1  1995/12/11  15:18:55  root
 * 	Submit
 * 	[1995/12/11  14:33:18  root]
 * 
 * Revision 1.1.2.2  1994/10/18  18:43:01  agd
 * 	expand copyright
 * 	[1994/10/18  17:39:00  agd]
 * 
 * Revision 1.1.2.1  1994/09/30  21:15:45  baum
 * 	New secval tests plus the ability to run without TET (OT 11975)
 * 	[1994/09/30  21:14:46  baum]
 * 
 * $EndLog$
 */

/*
 * Module to emulate TET main loop for debugging purposes
 */
#include <dtapi_impl.h>

extern struct tet_testlist tet_testlist[];
extern void (*tet_startup)(void);
extern void (*tet_cleanup)(void);

main(int argc, char **argv)
{
  struct tet_testlist 	*tmp;
  int 			num_tests, j, tests_to_fire[1024];

  for ( num_tests = 1; num_tests < argc; num_tests++ )
    tests_to_fire[num_tests-1] = atoi (argv[num_tests]);
  num_tests--;

  if ( tet_startup )
    (*tet_startup)();
  if ( num_tests == 0 )
    for ( tmp = tet_testlist; tmp->testfunc != NULL; tmp++ )
      (*(tmp->testfunc))();
  else
    for ( j = 0; j < num_tests; j++ ) {
      for ( tmp = tet_testlist; tmp->testfunc != NULL; tmp++ ) {
	if ( tests_to_fire[j] == tmp->icref )
	  (*(tmp->testfunc))();
      }
    }

  if ( tet_cleanup )
    (*tet_cleanup)();
}
  
