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
 * $Log: ntet_main.c,v $
 * Revision 1.1.7.3  1996/02/18  00:31:52  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:17:34  marty]
 *
 * Revision 1.1.7.2  1995/12/11  15:20:31  root
 * 	Submit
 * 	[1995/12/11  14:33:51  root]
 * 
 * Revision 1.1.2.2  1994/10/27  20:07:25  rrizzo
 * 	expand copyright
 * 	[1994/10/27  19:39:44  rrizzo]
 * 
 * Revision 1.1.2.1  1994/10/18  20:56:06  max
 * 	Initial submission of FVTs for dced utility API
 * 	[1994/10/18  20:48:09  max]
 * 
 * $EndLog$
 */
/*
 * @OSF_COPYRIGHT@
 * 
 */
/*
 * HISTORY
 * $Log: ntet_main.c,v $
 * Revision 1.1.7.3  1996/02/18  00:31:52  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:17:34  marty]
 *
 * Revision 1.1.8.2  1996/02/17  23:17:34  marty
 * 	Update OSF copyright years
 *
 * Revision 1.1.7.2  1995/12/11  15:20:31  root
 * 	Submit
 * 	[1995/12/11  14:33:51  root]
 *
 * Revision 1.1.8.2  1995/12/11  14:33:51  root
 * 	Submit
 *
 * Revision 1.1.2.2  1994/10/27  20:07:25  rrizzo
 * 	expand copyright
 * 	[1994/10/27  19:39:44  rrizzo]
 *
 * Revision 1.1.3.2  1994/10/27  19:39:44  rrizzo
 * 	expand copyright
 *
 * Revision 1.1.2.1  1994/10/18  20:56:06  max
 * 	Initial submission of FVTs for dced utility API
 * 	[1994/10/18  20:48:09  max]
 *
 * Revision 1.1.1.2  1994/10/18  20:48:09  max
 * 	Initial submission of FVTs for dced utility API
 *
 * $EndLog$
 */

/*
 * Rob Baum's module to emulate TET main loop for debugging purposes.
 * 
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
  




