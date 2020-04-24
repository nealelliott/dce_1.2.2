/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: osi_user_newProc.c,v $
 * Revision 1.1.11.1  1996/10/02  18:11:59  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:45:18  damon]
 *
 * $EndLog$
 */
/*
 *   osi_user_newProc.c -- user-space implementation of routine to create a new thread
 * and start it executing a specified routine
 */

/*
 *	Copyright (C) 1996, 1992 Transarc Corporation
 *	All rights reserved.
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>

#include <pthread.h>

#include <osi.h>

#if !defined(KERNEL)
EXPORT int osi_NewProc(clientRoutine, clientArgs,
		       blockPreemptionBeforeClientRoutine,
		       restorePreemptionAfterClientRoutine,
		       threadName)
     void	(*clientRoutine)();
     void *	clientArgs;
     int	blockPreemptionBeforeClientRoutine;
     int	restorePreemptionAfterClientRoutine;
     char	threadName[4];
{
  int		rtnVal = 0;
  int		preemptionCode;
  pthread_t	newThread;

  if (blockPreemptionBeforeClientRoutine) {
    preemptionCode = osi_PreemptionOff();
  }
    
  rtnVal = pthread_create(&newThread, pthread_attr_default,
			  (pthread_startroutine_t)clientRoutine, clientArgs);
  if (!rtnVal) {
    pthread_detach(&newThread);
  }

  if (restorePreemptionAfterClientRoutine) {
    osi_RestorePreemption(preemptionCode);
  }
  
  return rtnVal;
}

/* this is RIOS user mode version */
osi_MakeInitChild ()
{
    return 0;
}

#endif /* !defined(KERNEL) */
