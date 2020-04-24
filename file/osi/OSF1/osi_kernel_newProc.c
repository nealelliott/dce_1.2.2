/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: osi_kernel_newProc.c,v $
 * Revision 1.1.11.1  1996/10/02  17:58:24  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:43:55  damon]
 *
 * Revision 1.1.6.2  1994/07/13  22:18:54  devsrc
 * 	merged with bl-10
 * 	[1994/06/28  17:41:22  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  16:00:25  mbs]
 * 
 * Revision 1.1.6.1  1994/06/09  14:15:38  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:28:07  annie]
 * 
 * Revision 1.1.2.6  1993/01/21  14:50:33  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  14:52:38  cjd]
 * 
 * Revision 1.1.2.5  1992/11/24  18:22:23  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:13:18  bolinger]
 * 
 * Revision 1.1.2.4  1992/09/25  18:32:28  jaffe
 * 	Cleanup Minor header differences
 * 	[1992/09/24  15:43:31  jaffe]
 * 
 * Revision 1.1.2.3  1992/09/15  13:15:39  jaffe
 * 	Transarc delta: bab-ot2604-tkm-gc-impl 1.27
 * 	Transarc delta: jaffe-ot4609-cleanup-lock-initialization 1.5
 * 	[1992/09/14  20:15:23  jaffe]
 * 
 * Revision 1.1.2.2  1992/08/31  20:24:54  jaffe
 * 	Transarc delta: bab-ot2604-tkm-gc-impl 1.27
 * 	Transarc delta: jaffe-ot4719-cleanup-gcc-Wall-in-osi 1.3
 * 	[1992/08/30  03:16:46  jaffe]
 * 
 * $EndLog$
 */
/*
 *   osi_kernel_newProc.c -- kernel-space implementation (on RIOS) of routine to create 
 * a new thread and start it executing a specified routine
 */

/*
 *	Copyright (C) 1992 Transarc Corporation
 *	All rights reserved.
 */

#if defined(KERNEL)
#if defined(MACH)

/*
 * The non-kernel version of this routine is system-independent, and resides in the
 * parent directory to this one.
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>

#include <mach/std_types.h>
#include <kern/task.h>
#include <kern/thread.h>

#include <osi.h>

IMPORT thread_t 	kernel_thread _TAKES((
					      task_t	task,
					      void	(*start)()
					    ));
IMPORT kern_return_t	thread_terminate _TAKES((thread_t	thread));
IMPORT void		thread_halt_self _TAKES((void));

IMPORT task_t		first_task;

/*
 *   current_thread() is a macro defined in <kern/thread.h> that takes no
 * arguments and yields a thread_t.
 *
 *   current_task() is a macro defined in <kern/thread.h> that takes no
 * arguments and yields a task_t.
 */

typedef struct client_call_spec {
  thread_t	threadId;
  void 		(*clientRoutine)();
  void *	clientArgs;
  int		blockPreemption;
  int		restorePreemption;
} client_call_spec_t;

PRIVATE short			newProc_initialized = 0;

#define OSI_NEWPROC_NUMBER_CALLSPECS	(10)
/* this is also the address (caddr_t) on which we will osi_Sleep */
PRIVATE client_call_spec_t	callSpecArray[OSI_NEWPROC_NUMBER_CALLSPECS];

/*
 * This lock protects both the arg array and the initialized flag.
 * Initialize it the ugly way, since we don't have an osi_Init routine that
 * we know will only be executed once.
 */
PRIVATE struct lock_data	newProc_callSpecListLock =
						    LOCK_DATA_INITIALIZER;

/* prototypes */
PRIVATE void osi_NewProc_Init _TAKES(());
PRIVATE client_call_spec_t * osi_NewProc_FindCallSpecCell _TAKES((
							  thread_t	callerThread,
							  int		claimCell
								));
PRIVATE void osi_NewProc_ReleaseCallSpecCell _TAKES((
						     client_call_spec_t *	cellP,
						     thread_t		threadHoldingCell
						   ));

/*
 * routine assumes that the lock has been grabbed, and does not release it
 */
PRIVATE void osi_NewProc_Init()
{
  int	i;

  if (newProc_initialized == 0) {
    for (i = 0; i < OSI_NEWPROC_NUMBER_CALLSPECS; i++) {
      callSpecArray[i].threadId = THREAD_NULL;	/* ensures that the cell is unused */
    }

    newProc_initialized = 1;
  }
}

/*
 * Routine claims a call spec cell on behalf of the named thread.
 * Routine assumes the lock is held on its behalf, and returns with it held.
 */
PRIVATE client_call_spec_t * osi_NewProc_FindCallSpecCell(callerThread, claimCell)
     thread_t	callerThread;
     int	claimCell;
{
  client_call_spec_t *	rtnValP = (client_call_spec_t *)NULL;
  thread_t		probeThread = (claimCell) ? THREAD_NULL : callerThread;
  int			i;
  
  osi_NewProc_Init();
  
  while (rtnValP == (client_call_spec_t *)NULL) {
    for (i = 0;
	 (rtnValP == (client_call_spec_t *)NULL) && (i < OSI_NEWPROC_NUMBER_CALLSPECS);
	 i++) {
      if (callSpecArray[i].threadId == probeThread) {
	if (claimCell) {
	  callSpecArray[i].threadId = callerThread;
	}
	
	rtnValP = &(callSpecArray[i]);
      }
    }

    if (rtnValP == (client_call_spec_t *)NULL) {
      /* osi_SleepW atomically releases the lock and sleeps */
      osi_SleepW((opaque)callSpecArray, &newProc_callSpecListLock);

      /* re-obtain the lock and get ready to look through the list again */
      lock_ObtainWrite(&newProc_callSpecListLock);
    }
  }
  
  return rtnValP;
}

/*
 * Routine assumes the lock is held on entry, and returns with it locked.
 */
PRIVATE void osi_NewProc_ReleaseCallSpecCell(cellP, threadHoldingCell)
     client_call_spec_t *	cellP;
     thread_t			threadHoldingCell;
{
  osi_NewProc_Init();

  assert(cellP->threadId == threadHoldingCell);
  cellP->threadId = THREAD_NULL;

  osi_Wakeup((opaque)callSpecArray);
}

/*
 * this is the routine that sits at the bottom of the kernel thread stack
 */
PRIVATE void osi_ThreadBase (flags, argP, argSize)
     int		flags;
     void *		argP;
     unsigned int	argSize;
{
  client_call_spec_t *	argCellP = (client_call_spec_t *)NULL;
  thread_t		myThreadId = current_thread();
  void			(*routineToRun)();
  void *		argsToPass;
  int			blockPreemption;
  int			restorePreemption;
  int			preemptionCode;
  
  /* first, find and copy out the appropriate client info */
  lock_ObtainWrite(&newProc_callSpecListLock);
  argCellP = osi_NewProc_FindCallSpecCell(myThreadId, 0 /* => don't claim the cell */);
  routineToRun = argCellP->clientRoutine;
  argsToPass = argCellP->clientArgs;
  blockPreemption = argCellP->blockPreemption;
  restorePreemption = argCellP->restorePreemption;
  osi_NewProc_ReleaseCallSpecCell(argCellP, myThreadId);
  lock_ReleaseWrite(&newProc_callSpecListLock);  /* release the lock */

  if (blockPreemption) {
    preemptionCode = osi_PreemptionOff();
  }
    
  routineToRun(argsToPass);

  if (restorePreemption) {
    osi_RestorePreemption(preemptionCode);
  }

  thread_terminate(myThreadId);
  thread_halt_self();
}

/*
 * this is the routine that the client calls to get another kernel thread
 * (the user-space version really does have meaningful return value)
 */
EXPORT int osi_NewProc(clientRoutine, clientArgs,
		       blockPreemptionBeforeClientRoutine,
		       restorePreemptionAfterClientRoutine,
		       threadName)
     void	(*clientRoutine)();
     void *	clientArgs;
     int	blockPreemptionBeforeClientRoutine;
     int	restorePreemptionAfterClientRoutine;
     char	threadName[4];	/* arg ignored in this implementation */
{
  int			rtnVal = 0;
  thread_t		newThreadId;
  client_call_spec_t *	argCellP = (client_call_spec_t *)NULL;

  newThreadId = kernel_thread(first_task, osi_ThreadBase);

  lock_ObtainWrite(&newProc_callSpecListLock);
  argCellP = osi_NewProc_FindCallSpecCell(newThreadId, 1 /* => claim the found cell */);
  argCellP->clientRoutine = clientRoutine;
  argCellP->clientArgs = clientArgs;
  argCellP->blockPreemption = blockPreemptionBeforeClientRoutine;
  argCellP->restorePreemption = restorePreemptionAfterClientRoutine;

  osi_Wakeup((opaque)callSpecArray);  /* signal the state change */
  lock_ReleaseWrite(&newProc_callSpecListLock);  /* release the lock */

  return rtnVal;
}
#else /* defined(MACH) */
syntax error: we don't have a non-Mach implementation
#endif /* defined(MACH) */
#endif /* defined(KERNEL) */
