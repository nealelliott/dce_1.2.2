/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: proc4.c,v $
 * Revision 1.1.8.1  1996/10/17  18:16:50  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:49:26  damon]
 *
 * Revision 1.1.2.3  1993/01/11  17:38:04  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  16:54:49  htf]
 * 
 * Revision 1.1.2.2  1992/11/24  21:44:48  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:44:23  bolinger]
 * 
 * Revision 1.1  1992/01/19  03:18:04  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  proc4.c -- simple procedure for the bosserver to manage during testing
 *
 *
 *	Copyright (C) 1991, 1990 Transarc Corporation
 *	All rights reserved.
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>

#define MAX_SLEEPS	0

RCSID("$Header: /u0/rcs_trees/dce/rcs/test/file/bos/tests/procs/proc4.c,v 1.1.8.1 1996/10/17 18:16:50 damon Exp $")

#include <stdio.h>

  /* This process sleeps indefinitely */

int main(argc, argv)
     int	argc;
     char *	argv[];
{
  int	numberSleeps = 0;
  int	sleepTime;

  printf("Starting up Proc4 <%d: %s, %s>, PID %d, at ",
     argc, (argc>=2)? argv[1]:"null", (argc>=3)? argv[2]:"null", getpid());
  fflush(stdout);
  system("/bin/date");
  fflush(stdout); sync(); sync();

  if (argc == 3) {
    sleepTime = atoi(argv[2]);
  }
  else {
    printf("Proc4 <%s> exiting (wrong number of arguments).\n",
       (argc>=2)? argv[1]:"null");
    exit (1);
  }

  while ((MAX_SLEEPS == 0) || (numberSleeps < MAX_SLEEPS)) {
    sleep(sleepTime);
    numberSleeps++;
  }
  
  printf("Proc4 <%s> exiting (end of program).\n",
    (argc>=2)? argv[1]:"null");
  exit (0);
  
}
