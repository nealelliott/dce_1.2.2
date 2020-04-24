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
 * Revision 1.1.11.1  1996/10/02  17:59:11  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:44:28  damon]
 *
 * Revision 1.1.6.2  1994/07/13  22:18:49  devsrc
 * 	merged with bl-10
 * 	[1994/06/28  17:38:15  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  16:00:31  mbs]
 * 
 * Revision 1.1.6.1  1994/06/09  14:16:21  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:28:37  annie]
 * 
 * Revision 1.1.2.7  1993/01/21  14:51:27  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  14:54:12  cjd]
 * 
 * Revision 1.1.2.6  1993/01/13  18:17:45  shl
 * 	Transarc delta: kazar-ot6437-make-init-parent 1.1
 * 	  Selected comments:
 * 	    exiting threads weren't being wait3'd, and turned into zombies
 * 	    define osi_MakeInitChild to make init its parent
 * 	[1993/01/12  21:43:34  shl]
 * 
 * Revision 1.1.2.5  1992/11/24  18:22:42  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:13:41  bolinger]
 * 
 * Revision 1.1.2.4  1992/10/27  21:24:47  jaffe
 * 	Transarc delta: tu-ot5660-dont-use-setjmp-in-kernel 1.1
 * 	  Selected comments:
 * 	    Remove the usage of 'setjmp' in kernel when starting a new thread. This would
 * 	    make the debugging nearly impossible when the system panic.
 * 	[1992/10/27  14:40:08  jaffe]
 * 
 * Revision 1.1.2.3  1992/09/25  18:51:09  jaffe
 * 	Cleanup Minor header differences
 * 	[1992/09/24  15:44:11  jaffe]
 * 
 * Revision 1.1.2.2  1992/08/31  20:33:32  jaffe
 * 	Transarc delta: bab-ot2604-tkm-gc-impl 1.27
 * 	  Selected comments:
 * 	    Had to make another checkpoint so I could import another delta.
 * 	    Checkpoint version that passes basic user-space (non-regression - no real
 * 	    exercise of the gc, yet) tests.
 * 	    Removed subset bug fix in order to put it into the re-org delta.
 * 	    NEVER IMPORT THIS VERSION OF THIS DELTA!
 * 	    Re-establish the token subset bug fix in this delta.
 * 	    Need to import another delta.
 * 	    Another check-point version, this one in preparation for the one that
 * 	    removes tkm_Alloc.  This version passes basic user-space tests of garbage
 * 	    collection and asynchronous revocation.
 * 	    Another check-point.  This one removes tkm_Alloc (and, hence,
 * 	    tkm_memory.[ch]) from the token manager, since it doesn't really gain us
 * 	    that much, especially now that token cells are managed as an internal list.
 * 	    Another buildable checkpoint that passes basic periodic gc tests.
 * 	    Implemented a routine to be used as a "try waiting tokens" thread,
 * 	    along the same lines as the garbage collection thread.
 * 	    Add exception catching (if activated) to periodic try waiting routine,
 * 	    and make tkm panic/exit if it can't get memory from osi_Alloc;
 * 	    Fix stupid compilation bug left in in debugging a couple of versions ago.
 * 	    Implementation of token manager control afs_syscall.
 * 	    (Untested check-point...probably doesn't even compile)
 * 	    Had to reopen to fix compilation errors that stopped the other testing.
 * 	    Removed the token manager syscall routine from user-space compilation.
 * 	    This version of this delta contains support for the token manager's
 * 	    periodic activites thread (not yet activated) and for the afs_syscall
 * 	    that will query and set various parameters that control the token manager.
 * 	    Put ifdefs around prototype for routine that's only needed if testing without
 * 	    other tsr deltas.
 * 	    Check-point so that I can import another delta.
 * 	    Support for token manager periodic activities thread, from within
 * 	    the core kernel extension.
 * 	    Kernel-space implementation for RIOS.
 * 	    Allow client routines to return (exit the thread) without
 * 	    getting error messages.  Also changed the type of the thread base
 * 	    routine to void.
 * 	    Removed message printed if client routine returns.  Also, release the
 * 	    kernel lock unconditionally (Kazar believes that the client routine should
 * 	    be holding the kernel lock on return to the thread base routine).
 * 	    Changed interface to osi_NewProc to support parameterization of
 * 	    grabbing of kernel lock (on RIOS....blocking & restoration of kernel
 * 	    preemption, in general).
 * 	    Changed implementation of osi_NewProc to support parameterization
 * 	    of preemption.
 * 	    Correct typo.
 * 	    Set up for debugging.
 * 	    Sometimes, the token manager helper thread would get an illegal trap
 * 	    instruction on a rios machine (which Carl tells me is what assert
 * 	    failures become when the code is shipped) on trying to pause (via
 * 	    osi_Pause/osi_Wait). The problem is that the timer blocks in the kernel
 * 	    had not been initialized by the time the token manager thread tried to run.
 * 	    This version of this delta fixes the problem by making sure the timer
 * 	    blocks are initialized in time.
 * 	    Removed debugging control from testing of gc/async grant thread.
 * 	    Debugged problems with tkm not identifying potential candidates if the
 * 	    testing goes too fast (the reference times get too close together),
 * 	    or tkm tries to gc a fid on which it already holds the lock.
 * 	    Changed name of TakeBackTokens routine to reflect its new placement.
 * 	[1992/08/30  03:21:30  jaffe]
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
/*
 * The non-kernel version of this routine is system-independent, and resides in the
 * parent directory to this one.
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>

#include <sys/types.h>

#include <osi.h>

IMPORT pid_t	creatp _TAKES((void));
IMPORT int	initp _TAKES((
			      pid_t	newPid,
			      int	(*routineToRun)(),
			      void *	argP,
			      int	sizeofArgPData,
			      char	threadName[4]
			    ));

typedef struct client_call_spec {
  void 		(*clientRoutine)();
  void *	clientArgs;
  int		blockPreemption;
  int		restorePreemption;
} client_call_spec_t;

/*
 * this is the routine that sits at the bottom of the kernel thread stack
 */
PRIVATE osi_ThreadBase (flags, argP, argSize)
     int		flags;
     void *		argP;
     unsigned int	argSize;
{

    int           preemptionCode;

    /* 
     * This function is used to do a "setjmp" for every new thread started. 
     * However, when the longjmp occurs, the system panics without providing 
     * any information. This makes  subsequent debugging work nearly 
     * impossible. Remove these setjmp/clrjmp and let the system die on the
     * spot with appropriate info when there is a wrong doing. 
     */

    if (((client_call_spec_t *)argP)->blockPreemption) {
        preemptionCode = osi_PreemptionOff();
    }

    /* call the client's routine */
    ((client_call_spec_t *)
     argP)->clientRoutine(((client_call_spec_t *)argP)->clientArgs);

    if (((client_call_spec_t *)argP)->restorePreemption) {
        osi_RestorePreemption(preemptionCode);
    }

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
     char	threadName[4];
{
  int			rtnVal = 0;
  pid_t			newThreadPid;
  client_call_spec_t	clientCallSpec;
  
  /* package up the client's routine and args to be passed to the new thread */
  clientCallSpec.clientRoutine = clientRoutine;
  clientCallSpec.clientArgs = clientArgs;
  clientCallSpec.blockPreemption = blockPreemptionBeforeClientRoutine;
  clientCallSpec.restorePreemption = restorePreemptionAfterClientRoutine;
  
  if ((newThreadPid = creatp()) != -1) {
    if (initp(newThreadPid, osi_ThreadBase,
	      (void *)&clientCallSpec, sizeof(client_call_spec_t),
	      threadName) != 0) {
      panic("osi_NewProc: initp");
    }
  }
  else {
    panic("osi_NewProc: creatp");
  }

  return rtnVal;
}

/* make the calling process a child of init.  Used so that tpq-generated
 * threads eventually get wait3 calls issued for them, leave the zombie
 * state and eventually go away after exiting.
 */
osi_MakeInitChild()
{
    setpinit();
    setsid();
    return 0;
}

#endif /* defined(KERNEL) */
