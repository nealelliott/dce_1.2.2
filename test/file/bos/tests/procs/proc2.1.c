/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: proc2.1.c,v $
 * Revision 1.1.436.1  1996/10/17  18:16:40  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:49:20  damon]
 *
 * Revision 1.1.431.1  1994/02/04  20:39:23  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:22:04  devsrc]
 * 
 * Revision 1.1.429.1  1993/12/07  17:40:53  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  18:35:04  jaffe]
 * 
 * Revision 1.1.2.3  1993/01/11  17:37:42  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  16:54:15  htf]
 * 
 * Revision 1.1.2.2  1992/11/24  21:43:56  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:43:47  bolinger]
 * 
 * Revision 1.1  1992/01/19  03:18:08  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  proc2.1.c -- simple procedure for the bosserver to manage during testing
 *
 *
 *	Copyright (C) 1991, 1990 Transarc Corporation
 *	All rights reserved.
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/test/file/bos/tests/procs/proc2.1.c,v 1.1.436.1 1996/10/17 18:16:40 damon Exp $")

#include <sys/types.h>
#include <sys/file.h>
#include <fcntl.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>

  /* Process proc2.1 waits for a signal from proc2.2. Once it receives it,
     it kills both proc2.2 and proc2.1 */

extern int		errno;
extern FILE *		fopen();
     
char programName[BUFSIZ];

#define OWN_PID_FILENAME	"/tmp/test.1"
#define OTHER_PID_FILENAME	"/tmp/test.2"
#define LOCK_FILENAME		"/tmp/bossvr_test_lockfile"

static void test_handler(sig)
     int sig;
{
  FILE *		fp;
  int			other_pid;
  static char		routineName[] = "test_handler";
  
  fp = fopen(OTHER_PID_FILENAME, "r");
  if (fp == (FILE *)NULL) {
    fprintf(stderr, "%s: %s: error opening other process' pid file: %d\n",
	    programName, routineName, errno);
    exit(1);
  }
    
  fscanf(fp, "%d", &other_pid);
  if (fclose(fp)) {
    fprintf(stderr, "%s: %s: error closing other process' pid file: %d\n",
	    programName, routineName, errno);
    exit(1);
  }

  if (unlink(OTHER_PID_FILENAME)) {
    fprintf(stderr, "%s: %s: error unlinking other process' pid file: %d\n",
	    programName, routineName, errno);
    exit(1);
  }

  fprintf(stdout, "%s: %s: Killing Process proc2.2\n",
	  programName, routineName);
  fflush(stdout);

  if (kill(other_pid, SIGKILL)) {
    fprintf(stderr, "%s: %s: error killing other process: %d\n",
	    programName, routineName, errno);
    exit(1);
  }
      
  fprintf(stdout, "%s: %s: Killing Process proc2.1\n",
	  programName, routineName);
  fflush(stdout);

  if (kill(getpid(), SIGKILL)) {
    fprintf(stderr, "%s: %s: error killing self: %d\n",
	    programName, routineName, errno);
    exit(1);
  }
}

int main(argc, argv)
     int	argc;
     char *	argv[];
{
  FILE *		fp = (FILE *)NULL;
  int			haveFSLock = 0;
  int			lockFd = -1;
  static char		routineName[] = "main";
  
  (void)strncpy(programName, argv[0], sizeof(programName) - 1);

  fprintf(stdout, "%s: (%d) UP\n", programName, getpid());
  fflush(stdout);

  signal(SIGUSR1, test_handler);

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
  /* at this point, the other process cannot try to read this process' pid file */

  fp = fopen(OWN_PID_FILENAME, "w");
  if (fp == (FILE *)NULL) {
    fprintf(stderr, "%s: %s: error opening own pid file: %d\n",
	    programName, routineName, errno);
    exit(1);
  }
    
  fprintf(fp, "%d", getpid());

  if (fclose(fp)) {
    fprintf(stderr, "%s: %s: error closing own pid file: %d\n",
	    programName, routineName, errno);
    exit(1);
  }
  
  /* release the file lock */
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


  while(1) {
    /* intentionally left blank */
  }
  
}


