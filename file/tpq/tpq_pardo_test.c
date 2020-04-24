/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: tpq_pardo_test.c,v $
 * Revision 1.1.331.1  1996/10/02  18:49:17  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:49:26  damon]
 *
 * Revision 1.1.326.3  1994/07/13  22:30:09  devsrc
 * 	merged with bl-10
 * 	[1994/06/29  11:56:02  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  16:03:29  mbs]
 * 
 * Revision 1.1.326.2  1994/06/09  14:23:12  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:34:54  annie]
 * 
 * Revision 1.1.326.1  1994/02/04  20:33:28  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:19:22  devsrc]
 * 
 * Revision 1.1.324.1  1993/12/07  17:36:11  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  17:22:11  jaffe]
 * 
 * Revision 1.1.2.5  1993/01/21  15:56:21  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  17:40:16  cjd]
 * 
 * Revision 1.1.2.4  1992/11/24  19:48:34  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:33:11  bolinger]
 * 
 * Revision 1.1.2.3  1992/09/25  19:43:32  jaffe
 * 	Cleanup Minor header differences
 * 	[1992/09/24  15:46:41  jaffe]
 * 
 * Revision 1.1.2.2  1992/08/31  21:34:31  jaffe
 * 	Transarc delta: comer-ot3947-multithread-cm-daemon 1.25
 * 	[1992/08/30  13:18:08  jaffe]
 * 
 * $EndLog$
 */
/*
 * Copyright (C) 1992 Transarc Corporation
 * All rights reserved.
 */
/*
 *  tpq_pardo_test.c -- test program for parallel-do facility based
 * on the tpq package
 */

#include <stdio.h>
#include <pthread.h>

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>

#include <tpq.h>
#include <tpq_private.h>

#define BASE_DELAY	10

tpq_lock_data_t	io_lock;

char programName[BUFSIZ];

typedef struct forall_test_args {
  unsigned int		baseDelay;
  unsigned int		forallDepth;
  unsigned int		parentKey;
  unsigned int		fanout;
} forall_test_args_t;

forall_test_args_t	base_forall;

void ForallBase_TestOp(argP, ignoredTpqOpHandle)
     void *	argP;
     void *	ignoredTpqOpHandle;
{
  tpq_forall_arg_t *	myForallArgP = (tpq_forall_arg_t *)argP;
  int			myIndex = myForallArgP->opIndex;
  forall_test_args_t *	testArgsP = (forall_test_args_t *)(myForallArgP->clientArgP);
  unsigned int		myBaseDelayTime = testArgsP->baseDelay;
  unsigned int		forallDepth = testArgsP->forallDepth;
  unsigned int		myParent = testArgsP->parentKey;
  struct timespec	delayTime;
  static char		routineName[] = "ForallBase_TestOp";

  delayTime.tv_sec = myBaseDelayTime * myIndex;
  delayTime.tv_nsec = 0;

  tpqLock_ObtainWrite(&io_lock);
  fprintf(stdout, "%s: %d / %d (%d) about to delay for %d seconds\n",
	  routineName, myParent, myIndex, forallDepth, delayTime.tv_sec);
  fflush(stdout);
  tpqLock_ReleaseWrite(&io_lock);

  if (pthread_delay_np(&delayTime) == -1) {
    fprintf(stderr, "%s: %d / %d: (%d) error in pthread_delay_np; errno = %d\n",
	    routineName, myParent, myIndex, forallDepth, errno);
  }

  tpqLock_ObtainWrite(&io_lock);
  fprintf(stdout, "%s: %d / %d (%d) exiting\n",
	  routineName, myParent, myIndex, forallDepth);
  fflush(stdout);
  tpqLock_ReleaseWrite(&io_lock);
}

void ForallNest_TestOp(argP, ignoredTpqOpHandle)
     void *	argP;
     void *	ignoredTpqOpHandle;
{
  tpq_forall_arg_t *	myForallArgP = (tpq_forall_arg_t *)argP;
  int			myIndex = myForallArgP->opIndex;
  forall_test_args_t *	testArgsP = (forall_test_args_t *)(myForallArgP->clientArgP);
  unsigned int		myBaseDelayTime = testArgsP->baseDelay;
  unsigned int		forallDepth = testArgsP->forallDepth;
  unsigned int		myParent = testArgsP->parentKey;
  unsigned int		myFanout = testArgsP->fanout;
  struct timespec	delayTime;
  forall_test_args_t *	myChildrensArgsP;
  static char		routineName[] = "ForallNest_TestOp";

  myChildrensArgsP = (forall_test_args_t *)
      osi_Alloc(sizeof(forall_test_args_t));
  osi_assert(myChildrensArgsP != (forall_test_args_t *)NULL);
  myChildrensArgsP->parentKey = myIndex;
  myChildrensArgsP->baseDelay = myBaseDelayTime + myIndex;
  myChildrensArgsP->forallDepth = forallDepth + 1;

  tpqLock_ObtainWrite(&io_lock);
  fprintf(stdout, "%s: %d / %d (%d) about to start forall processes\n",
	  routineName, myParent, myIndex, forallDepth);
  fflush(stdout);
  tpqLock_ReleaseWrite(&io_lock);

  if (tpq_ForAll((void *)NULL, ForallBase_TestOp,
		 (void *)myChildrensArgsP, myFanout, TPQ_HIGHPRI, 0)) {
    tpqLock_ObtainWrite(&io_lock);
    fprintf(stderr, "%s: %d / %d (%d) something went wrong in forall test\n",
	    routineName, myParent, myIndex, forallDepth);
    fflush(stderr);
    tpqLock_ReleaseWrite(&io_lock);
  }
  tpqLock_ObtainWrite(&io_lock);
  fprintf(stdout, "%s: %d / %d (%d) returned from forall processes\n",
	  routineName, myParent, myIndex, forallDepth);
  fflush(stdout);
  tpqLock_ReleaseWrite(&io_lock);
}

int main (argc, argv)
     int	argc;
     char *	argv[];
{
  int		rtnVal = 0;
  int		numbForallInstances;
  static char	routineName[] = "main";

  (void)strncpy(programName, argv[0], sizeof(programName));
  if (argc == 2) {
    numbForallInstances = atoi(argv[1]);
  }
  else {
    fprintf(stderr, "%s: Usage %s <fanout>\n", programName, programName);
    exit(1);
  }

  tpqLock_Init(&io_lock);

  base_forall.baseDelay = BASE_DELAY;
  base_forall.forallDepth = 1;
  base_forall.parentKey = 0;
  base_forall.fanout = numbForallInstances;

  tpqLock_ObtainWrite(&io_lock);
  fprintf(stdout, "%s: about to start base forall processes\n", routineName);
  fflush(stdout);
  tpqLock_ReleaseWrite(&io_lock);

  if (tpq_ForAll((void *)NULL, ForallNest_TestOp,
		 (void *)&base_forall, numbForallInstances, TPQ_HIGHPRI, 0)) {
    tpqLock_ObtainWrite(&io_lock);
    fprintf(stderr, "%s: something went wrong in forall test\n", routineName);
    fflush(stderr);
    tpqLock_ReleaseWrite(&io_lock);
  }
  tpqLock_ObtainWrite(&io_lock);
  fprintf(stdout, "%s: returned from forall processes\n", routineName);
  fflush(stdout);
  tpqLock_ReleaseWrite(&io_lock);

  return rtnVal;
}
