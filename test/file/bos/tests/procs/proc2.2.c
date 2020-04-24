/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: proc2.2.c,v $
 * Revision 1.1.437.1  1996/10/17  18:16:41  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:49:21  damon]
 *
 * Revision 1.1.432.1  1994/02/04  20:39:25  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:22:05  devsrc]
 * 
 * Revision 1.1.430.1  1993/12/07  17:40:55  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  18:35:23  jaffe]
 * 
 * Revision 1.1.2.3  1993/01/11  17:37:44  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  16:54:19  htf]
 * 
 * Revision 1.1.2.2  1992/11/24  21:44:05  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:43:53  bolinger]
 * 
 * Revision 1.1  1992/01/19  03:18:09  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  proc2.2.c -- simple procedure for the bosserver to manage during testing
 *
 *
 *	Copyright (C) 1991, 1990 Transarc Corporation
 *	All rights reserved.
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/test/file/bos/tests/procs/proc2.2.c,v 1.1.437.1 1996/10/17 18:16:41 damon Exp $")

#include <sys/types.h>
#include <sys/file.h>
#include <fcntl.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>

extern int errno;

char programName[BUFSIZ];

#define OWN_PID_FILENAME	"/tmp/test.2"
#define OTHER_PID_FILENAME	"/tmp/test.1"
#define LOCK_FILENAME		"/tmp/bossvr_test_lockfile"

/* Process proc2.2 kills process proc2.1 as soon as it starts up */

int main(argc, argv)
     int	argc;
     char *	argv[];
{
  int			other_pid;
  FILE *		fp = (FILE *)NULL;
  int			lockFd;
  int			haveFSLock;
  static char		routineName[] = "main";
  
  extern FILE *	fopen();

  (void)strncpy(programName, argv[0], sizeof(programName) - 1);

  fprintf(stdout, "%s: (%d) UP\n", programName, getpid());
  fflush(stdout);

  while (fp == (FILE *)NULL) {
    while (haveFSLock == 0) {
      if ((lockFd = open(LOCK_FILENAME, (O_CREAT | O_EXCL), 0644)) < 0) {
	if (errno != EEXIST) {
	  fprintf(stderr, "%s: %s: error opening lock file: %d\n",
		  programName, routineName, errno);
	  exit(1);
	}
      }
      else {
	haveFSLock = 1;
      }
    }

    /* at this point, the other file is either completely ready or not there */
    if ((fp = fopen(OTHER_PID_FILENAME, "r")) == (FILE *)NULL) {
      if (close(lockFd)) {
	fprintf(stderr, "%s: %s: error closing lock file (waiting): %d\n",
		programName, routineName, errno);
	exit(1);
      }
      
      if (unlink (LOCK_FILENAME)) {
	fprintf(stderr, "%s: %s: error unlinking lock file (waiting): %d\n",
		programName, routineName, errno);
	exit(1);
      }
      
      haveFSLock = 0;

      sleep(2);		/* give the other process a chance to do something */
    }
  }
  
  /* at this point, the other process' file is there and complete */

  if (fscanf(fp, "%d", &other_pid) != 1) {
    fprintf(stderr, "%s: %s: error reading other process' pid file: %d\n",
	    programName, routineName, errno);
    exit(1);
  }

  /* close and unlink the other process' pid file */
  if (fclose(fp)) {
    fprintf(stderr, "%s: %s: error closing other process' pid file: %s\n",
	    programName, routineName);
    exit(1);
  }
  
  if (unlink (OTHER_PID_FILENAME)) {
    fprintf(stderr, "%s: %s: error unlinking other process' pid file: %d\n",
	    programName, routineName, errno);
    exit(1);
  }
  
  /* close and unlink the lock file, since we are done with the other pid file */
  if (close(lockFd)) {
    fprintf(stderr, "%s: %s: error closing lock file: %d\n",
	    programName, routineName, errno);
    exit(1);
  }
  
  if (unlink (LOCK_FILENAME)) {
    fprintf(stderr, "%s: %s: error unlinking lock file: %d\n",
	    programName, routineName, errno);
    exit(1);
  }
  
  fp = fopen(OWN_PID_FILENAME, "w");
  if (fp != (FILE *)NULL) {
    fprintf(fp, "%d", getpid());

    if (fclose(fp)) {
      fprintf(stderr, "%s: %s: error closing own pid file: %d\n",
	      programName, routineName, errno);
      exit(1);
    }
    
    fprintf(stdout, "%s: %s: Sending Process proc2.1 (%d) SIGUSR1\n",
	    programName, routineName, other_pid);
    fflush(stdout);

    if (kill(other_pid, SIGUSR1)) {
      fprintf(stderr, "%s: %s: error sending other process SIGUSR1 signal: %d\n",
	      programName, routineName, errno);
      exit(1);
    }

    while(1) {
      /* intentionally left blank */
    }
  }  
}



