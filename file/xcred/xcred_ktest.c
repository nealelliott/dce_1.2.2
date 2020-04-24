/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xcred_ktest.c,v $
 * Revision 1.1.491.1  1996/10/02  19:03:18  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:51:46  damon]
 *
 * Revision 1.1.486.3  1994/07/13  22:30:48  devsrc
 * 	merged with bl-10
 * 	[1994/06/29  12:12:07  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  16:04:55  mbs]
 * 
 * Revision 1.1.486.2  1994/06/09  14:25:47  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:37:45  annie]
 * 
 * Revision 1.1.486.1  1994/02/04  20:36:33  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:20:46  devsrc]
 * 
 * Revision 1.1.484.1  1993/12/07  17:38:34  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  18:17:21  jaffe]
 * 
 * Revision 1.1.3.3  1993/01/21  16:34:12  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  18:28:04  cjd]
 * 
 * Revision 1.1.3.2  1993/01/13  19:52:28  shl
 * 	Transarc delta: jaffe-cleanup-2.3-compiler-warnings 1.8
 * 	  Selected comments:
 * 	    Fix compiler warnings found when moving to OSF/1 1.1.1.
 * 	    remove extra ; after RCSID.
 * 	    Remove the function dacl_SetOwnerControl.  It is not used.
 * 	[1993/01/12  22:36:48  shl]
 * 
 * Revision 1.1  1992/01/19  02:45:49  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * (C) Copyright 1990 Transarc Corporation
 * Licensed Materials - Property of Transarc
 * All Rights Reserved.
 *
 * xcred_ktest.c
 *	Test program for the extended credential package.  This acts
 *	as a client via the xcred system call interface.
 */


#include <xcred.h>		/*Extended credential interface*/
#include <xcred_kint.h>		/*Exported kernel interface*/

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/xcred/xcred_ktest.c,v 1.1.491.1 1996/10/02 19:03:18 damon Exp $")

main(argc, argv)
    int argc;
    char **argv;

{ /*main*/

#ifdef  TESTING_XCRED_SYSCALL


  xcred_kint_opargs_t opArgs;			/*Args for chosen opcode*/
  xcred_kint_opargs_t *opArgsP = &opArgs;	/*Ptr to above*/
  xcred_kint_opargs_t *nullOpArgsP;		/*Null arguments*/
  long code;					/*Return code*/
  char userInput[10];				/*User input buffer*/
  long debugLevel;				/*Debugging level*/

  /*
   * Set up the null argument.
   */
  nullOpArgsP = (xcred_kint_opargs_t *)0;

  printf("\nTesting the extended credential package\n\n");

  /*
   * Set the debugging output level.  If none is provided on the input
   * line, then assume that none is desired.
   */
  if (argc > 1) {
    debugLevel = atol(argv[1]);
  }
  else
    debugLevel = 0;

  printf("Setting debugging level to %d\n", debugLevel);

  opArgs.adbLevel.anewDBLevel = debugLevel;
  code =
    afs_syscall(AFSCALL_XCRED,
	    XCRED_SYSCALL_SET_DEBUG_LEVEL,
	    opArgsP);
  if (code || errno)
    goto xcred_ktest_Exit;

  /*
   * Now, run the whole test suite.
   */
  printf("Hit go<CR> to run the full test suite: ");
  scanf("%s", userInput);

  code =
    afs_syscall(AFSCALL_XCRED, XCRED_SYSCALL_TEST_SUITE, nullOpArgsP);
  if (code || errno)
    goto xcred_ktest_Exit;
  else
    printf("Test suite completed successfully\n");

xcred_ktest_Exit:

  if (errno) {
    printf("** Syscall failed, errno is %d\n", errno);
    exit(-1);
  }
#endif  /* TESTING_XCRED_SYSCALL */

} /*main*/



