/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: proc1.c,v $
 * Revision 1.1.8.1  1996/10/17  18:16:39  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:49:19  damon]
 *
 * Revision 1.1.2.3  1993/01/11  17:37:40  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  16:54:11  htf]
 * 
 * Revision 1.1.2.2  1992/11/24  21:43:48  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:43:41  bolinger]
 * 
 * Revision 1.1  1992/01/19  03:18:02  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  bos_sleep_test.c -- simple procedure for the bosserver to manage during testing
 *
 *
 *	Copyright (C) 1991, 1990 Transarc Corporation
 *	All rights reserved.
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>

#define MAX_SLEEPS	1

RCSID("$Header: /u0/rcs_trees/dce/rcs/test/file/bos/tests/procs/proc1.c,v 1.1.8.1 1996/10/17 18:16:39 damon Exp $")

#include <stdio.h>

int main(argc, argv)
     int	argc;
     char *	argv[];
{
  int	sleepTime;
  int	numberSleeps = 0;

  if (argc == 2) {
    sleepTime = atoi(argv[1]);
  }
  else {
    exit (1);
  }
  fprintf(stdout, "Process proc1 PID %d, started at", getpid());
  fflush(stdout);
  system("date");
  fprintf(stdout, "\n");
  fflush(stdout); sync(); sync();

  while ((MAX_SLEEPS == 0) || (numberSleeps < MAX_SLEEPS)) {
    sleep(sleepTime);
    numberSleeps++;
  }
  
  exit (0);
  
}
