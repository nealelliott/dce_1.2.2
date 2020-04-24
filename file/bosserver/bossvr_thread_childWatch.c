/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: bossvr_thread_childWatch.c,v $
 * Revision 1.1.111.1  1996/10/02  17:05:07  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:05:08  damon]
 *
 * Revision 1.1.103.1  1994/10/20  19:29:52  sasala
 * 	Implement S12Y in bosserver subcomponent -UYHP: 12618
 * 	[1994/10/20  18:15:31  sasala]
 * 
 * Revision 1.1.101.3  1994/07/13  22:20:02  devsrc
 * 	merged with DCE1.1
 * 	[1994/07/11  15:17:42  devsrc]
 * 
 * 	merged with bl-10
 * 	[1994/06/28  19:58:40  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:54:09  mbs]
 * 
 * Revision 1.1.101.2  1994/06/09  13:52:39  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:26:28  annie]
 * 
 * Revision 1.1.101.2  1994/06/09  13:52:39  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:26:28  annie]
 * 
 * Revision 1.1.101.1  1994/02/04  20:06:36  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:06:49  devsrc]
 * 
 * Revision 1.1.99.1  1993/12/07  17:13:14  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  12:59:07  jaffe]
 * 
 * Revision 1.1.5.5  1993/01/18  19:57:19  cjd
 * 	Embedded copyright notice
 * 	[1993/01/18  19:33:47  cjd]
 * 
 * Revision 1.1.5.4  1992/11/24  15:41:50  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:09:54  bolinger]
 * 
 * Revision 1.1.5.3  1992/10/02  20:50:41  toml
 * 	Change sense of #ifdef to ease porting.
 * 	[1992/10/02  18:40:35  toml]
 * 
 * Revision 1.1.5.2  1992/08/31  18:33:22  jaffe
 * 	Transarc delta: bab-ot4386-bosserver-child-signals-not-rcvd 1.2
 * 	  Selected comments:
 * 	    This delta reverses part of delta dstokes-ot2171-remove-cma-calls,
 * 	    since that delta broke the bosserver.  The bosserver was no longer
 * 	    seeing any SIGCHLD signals when sigwait was used in place
 * 	    of cma_sigwait, so the call to cma_sigwait was restored.
 * 	    ot 4386
 * 	    See above.
 * 	    It turns out that the name of this delta is more than a bit of a
 * 	    misnomer, but the symptoms were that killed children were not being
 * 	    restarted.  It turns out that there is some sort of optimizer bug that
 * 	    was tickled by the way the error handling structures were manipulated in
 * 	    bossvr_thread_bnodeTimeout.c.  To work around this, the cleanup structures
 * 	    were changed, as was the way they were used.
 * 	    I also cleaned up some of the copious ifdefs.
 * 	    See above.
 * 	[1992/08/28  20:51:20  jaffe]
 * 
 * Revision 1.1.3.4  1992/05/20  19:44:20  mason
 * 	Transarc delta: dstokes-ot2171-remove-cma-calls 1.1
 * 	  Files modified:
 * 	    bosserver: bossvr_main.c, bossvr_thread_childWatch.c
 * 	    flserver: flc.c, flclient.c, flserver.c
 * 	    ncscompat: compat_serverdup.c
 * 	    ncsubik: ubikclient.c, ubikcmd.c, utst_client.c
 * 	    ncsubik: utst_server.c; rep: rep_main.c
 * 	    security/dacl: testacl.c
 * 	    security/dfsauth: auth_client_main.c, authtest_main.c
 * 	    security/dfsauth: dfsauth_client.c, dfsauth_server.c
 * 	    tkm: tkm_tokens.c; update: client.c
 * 	    userInt/fts: volc_vldbint.c
 * 	  Selected comments:
 * 	    Remove explicit CMA calls to allow other threading package styles.
 * 	    Dawn Stokes says this works fine for her.
 * 	    Add include of cma_sigwait.h and replace cma_sigwait call with sigwait().
 * 	[1992/05/19  22:20:42  mason]
 * 
 * Revision 1.1.3.3  1992/01/24  01:48:59  devsrc
 * 	Merging dfs6.3
 * 	[1992/01/24  00:03:52  devsrc]
 * 
 * Revision 1.1.3.2  1992/01/23  19:05:48  zeliff
 * 	Moving files onto the branch for dfs6.3 merge
 * 	[1992/01/23  18:21:11  zeliff]
 * 	Revision 1.1.1.2  1992/01/22  19:30:52  devsrc
 * 	Bring in dfs6.3 sources
 * 
 * Revision 1.1  1992/01/19  02:49:35  devrcs
 * 	Initial revision
 * 
 * $EndLog$
*/
/*
 *  bossvr_thread_childWatch.c -- the code executed by the CMA thread that handles
 * the child death signals
 *
 *
 *	Copyright (C) 1991, 1990 Transarc Corporation
 *	All rights reserved.
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <pthread.h>

#include <dcedfs/osi_wait.h>

#include <bbos_bnode.h>
#include <bbos_util.h>
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsbssmac.h>
#include <dfsbsssvc.h>
#include <dfsbssmsg.h>

#if defined(AFS_OSF_ENV) || defined(AFS_AIX_ENV)
IMPORT int wait3 _TAKES((
			 osi_wait_t *	status,
			 int		options,
			 struct rusage *	rsrcUsage
		       ));
#endif /* defined(AFS_OSF_ENV) || defined(AFS_AIX_ENV) */

IMPORT struct bnode_proc * FindBnodeProcFromPid _TAKES((
						        int	thePid
						      ));

IMPORT void bossvr_ThreadExit _TAKES((
				    void * argP
				  ));

#ifdef POSIX_SUNOS5_SIGWAIT
static void     sigchld_handler(void);
#endif

/* bosserver's global mutex to coordinate its threads */
IMPORT pthread_mutex_t	bosserverMutex;
IMPORT bnode_stats_t 	bnode_stats;

IMPORT int		childWatchThreadReady;
IMPORT pthread_cond_t	childWatchThreadReadyCond;

static cleanupArg_t	childWatchThrCleanupArg;

/*
 *  I don't know why, but the AFS_OSF_ENV seems to need this, and we definitely
 * want to keep the definition as localized as possible.
 */
#if defined(AFS_OSF_ENV)
#define pthread_addr_t void *
#endif /* defined(AFS_OSF_ENV) */

/* move this into the bnode file */
struct bnode_proc * FindBnodeProcFromPid(thePid)
     int	thePid;
{
  struct bnode_proc *		rtnValP = (struct bnode_proc *)NULL;
  struct bnode_proc *		bnodeProcProbeP;
  
  extern struct bnode_proc *	allProcs;

  for (bnodeProcProbeP = allProcs;
       ((bnodeProcProbeP != (struct bnode_proc *)NULL) &&
	(rtnValP == (struct bnode_proc *)NULL));
       bnodeProcProbeP = bnodeProcProbeP->next) {
    if (bnodeProcProbeP->pid == thePid) {
      rtnValP = bnodeProcProbeP;
    }
  }

  return rtnValP;
}

static int GetNextExitedChildPid(childStatus)
     osi_wait_t *	childStatus;
{
  int		rtnVal = 0;
  int		wait3options = WNOHANG;

  extern int	bnode_waiting;
    
  bnode_waiting = wait3options | 0x800000;
  rtnVal = osi_wait3(childStatus, wait3options, (struct rusage *)NULL);
  bnode_waiting = 0;

  return rtnVal;
}
  
static void ProcessChildTermination(childBnodeP, childBnodeProcP, childStatusP)
     struct bnode *		childBnodeP;
     struct bnode_proc *	childBnodeProcP;
     osi_wait_t	*		childStatusP;
{
  long	nowInSecs;

  nowInSecs = osi_Time();

  /* count restarts in last 10 minutes */
  if (nowInSecs > (childBnodeP->rsTime + 10*60)) {
    /* it's been 10 minutes we've been counting */
    childBnodeP->rsTime = nowInSecs;
    childBnodeP->rsCount = 0;
  }
  
  if (childStatusP->w_termsig == 0) {
    /* exited, not signalled */
    childBnodeProcP->lastExit = childStatusP->w_retcode;
    if (childBnodeProcP->lastExit) {
      childBnodeP->errorCode = childBnodeProcP->lastExit;
      childBnodeP->lastErrorExit = osi_Time();
      childBnodeP->errorSignal = 0;
    }
  }
  else {
    /* Signal occurred, perhaps spurious due to shutdown request.
     * If due to a shutdown request, don't overwrite last error
     * information.
     */
    childBnodeProcP->lastSignal = childStatusP->w_termsig;
    if ((childBnodeProcP->lastSignal != SIGQUIT) &&
	(childBnodeProcP->lastSignal != SIGTERM) &&
	(childBnodeProcP->lastSignal != SIGKILL)) {
      childBnodeP->errorSignal = childBnodeProcP->lastSignal;
      childBnodeP->lastErrorExit = osi_Time();
    }
    if (childStatusP->w_coredump) {
      /* don't process core file unless one was really dumped */
      SaveCore(childBnodeP, childBnodeProcP);
    }
  }
  childBnodeP->lastAnyExit = osi_Time();
  
  BOP_PROCEXIT(childBnodeP, childBnodeProcP);
  bnode_Check(childBnodeP);
  if (childBnodeP->rsCount++ > 10) {
    /* 10 in 10 minutes */
    childBnodeP->flags |= BNODE_ERRORSTOP;
    bnode_SetGoal(childBnodeP, BSTAT_SHUTDOWN);
    bossvr_SvcLog(bss_s_childBnode_process_start_failure_repeatedly,
		  childBnodeP->name);
  }
}

     
  
EXPORT pthread_addr_t * ChildWatchThread(ignoredArgP)
     pthread_addr_t *	ignoredArgP;
{
  pthread_addr_t *	rtnVal = (pthread_addr_t *)NULL;
  int			exitedChildPid = 0;
  struct bnode *	exitedBnodeP = (struct bnode *)NULL;
  struct bnode_proc *	exitedBnodeProcP = (struct bnode_proc *)NULL;
  osi_wait_t		exitedChildStatus;
  int			signalCaught = 0;
  sigset_t		signalChildSet;
  int			pthreadRtn;
  int			savedErrno;
  unsigned char *dcesptr;    

  strncpy(childWatchThrCleanupArg.threadName, "childWatchThread",
	  sizeof(childWatchThrCleanupArg.threadName));
  childWatchThrCleanupArg.msg[0] = '\0';
  childWatchThrCleanupArg.releaseLockFlag = BOSSVR_NO_RELEASE_GLOBAL_LOCK;
  pthread_cleanup_push(bossvr_ThreadExit, &childWatchThrCleanupArg);

  /* initialize the signal set */
  (void)sigemptyset(&signalChildSet);
  (void)sigaddset(&signalChildSet, SIGCHLD);
#ifdef POSIX_SUNOS5_SIGWAIT
  {
      struct sigaction sigactn;

      sigactn.sa_handler = sigchld_handler;
      sigemptyset(&sigactn.sa_mask);
      sigactn.sa_flags   = 0;
      sigaction(SIGCHLD, &sigactn, NULL);
  }
#endif

  {
  dcesptr = dce_sprintf(bss_s_exception_in_mutex_lock, __FILE__, __LINE__);
  (void)strcpy(childWatchThrCleanupArg.msg, dcesptr);
  free(dcesptr);
  }
  pthreadRtn = pthread_mutex_lock(&bosserverMutex);
  savedErrno = errno;
  if (pthreadRtn != 0) {
    dcesptr = dce_sprintf(bss_s_error_return_from_mutex_lock,
		__FILE__, __LINE__, savedErrno);
    (void)strcpy(childWatchThrCleanupArg.msg, dcesptr);
    free(dcesptr);
    pthread_exit((pthread_addr_t)NULL);
    /* execution never reaches here */
  }
  
  dcesptr = dce_sprintf(bss_s_excpn_or_cancel_in_pthread_cond_signal, 
                      __FILE__, __LINE__, savedErrno);
  (void)strcpy(childWatchThrCleanupArg.msg, dcesptr);
  free(dcesptr);
  childWatchThrCleanupArg.releaseLockFlag = BOSSVR_RELEASE_GLOBAL_LOCK;
  childWatchThreadReady = 1;
  pthreadRtn = pthread_cond_signal(&childWatchThreadReadyCond);
  savedErrno = errno;
  if (pthreadRtn != 0) {
    dcesptr = dce_sprintf(bss_s_error_return_from_pthread_cond_signal,
                         __FILE__, __LINE__, savedErrno);
    (void)strcpy(childWatchThrCleanupArg.msg, dcesptr);
    free(dcesptr);
    pthread_exit((pthread_addr_t)NULL);
    /* execution never reaches here */
  }
  
  dcesptr = dce_sprintf(bss_s_excptn_in_mutex_unlock,
                        __FILE__, __LINE__, savedErrno);
  (void)strcpy(childWatchThrCleanupArg.msg, dcesptr);
  free(dcesptr);
  pthreadRtn = pthread_mutex_unlock(&bosserverMutex);
  savedErrno = errno;
  if (pthreadRtn != 0) {
    dcesptr = dce_sprintf(bss_s_error_return_from_mutex_unlock,
		           __FILE__, __LINE__, savedErrno);
    (void)strcpy(childWatchThrCleanupArg.msg, dcesptr);
    free(dcesptr);
    pthread_exit((pthread_addr_t)NULL);
    /* execution never reaches here */
  }
  
  childWatchThrCleanupArg.releaseLockFlag = BOSSVR_NO_RELEASE_GLOBAL_LOCK;
  while (TRUE) {
    /* wait for a SIGCHLD to show up */
    dcesptr = dce_sprintf(bss_s_excptn_or_cancel_in_cma_sigwait, __FILE__, __LINE__);
    strcpy(childWatchThrCleanupArg.msg, dcesptr);
    free(dcesptr);
    signalCaught = /*cma_*/sigwait(&signalChildSet);
    dcesptr = dce_sprintf(bss_s_processing_cma_sigwait_return, __FILE__, __LINE__);
    strcpy(childWatchThrCleanupArg.msg, dcesptr);
    free(dcesptr);

    if (signalCaught == SIGCHLD) {
      dcesptr = dce_sprintf(bss_s_excptn_in_mutex_lock,
		            __FILE__, __LINE__, errno);
      (void)strcpy(childWatchThrCleanupArg.msg, dcesptr);
      free(dcesptr);
      childWatchThrCleanupArg.releaseLockFlag =
	BOSSVR_NO_RELEASE_GLOBAL_LOCK;
      if (pthread_mutex_lock(&bosserverMutex) != 0) {
	  dcesptr = dce_sprintf(bss_s_error_return_from_mutex_lock1,
				__FILE__, __LINE__, errno);
	  (void)strcpy(childWatchThrCleanupArg.msg, dcesptr);
	  free(dcesptr);
	pthread_exit((pthread_addr_t)NULL);
	/* execution never reaches here */
      }

      /* we have the lock here, so exit should release it */
      childWatchThrCleanupArg.releaseLockFlag = BOSSVR_RELEASE_GLOBAL_LOCK;
      
      /* process each child that has exited */
      dcesptr = dce_sprintf(bss_s_error_processing_exited_child_pids);
      (void)strcpy(childWatchThrCleanupArg.msg, dcesptr);
      free(dcesptr);
      while ((exitedChildPid = GetNextExitedChildPid(&exitedChildStatus)) > 0) {

	exitedBnodeProcP = FindBnodeProcFromPid(exitedChildPid);
	if (exitedBnodeProcP != (struct bnode_proc *)NULL) {
	  /* do whatever is needed to process this bnode */
	  exitedBnodeP = exitedBnodeProcP->bnode;
	  
	  bnode_Hold(exitedBnodeP);
	  ProcessChildTermination(exitedBnodeP, exitedBnodeProcP, &exitedChildStatus);
	  bnode_Release(exitedBnodeP);
	  
	  DeleteProc(exitedBnodeProcP);
	}	/* end if we found the bnode proc */
	else {
	  bnode_stats.weirdPids++;
	  bossvr_SvcLog(bss_s_weird_Pids_processed_up_to_here,
			childWatchThrCleanupArg.threadName, 
			bnode_stats.weirdPids);
	}
	
      }	/* end while we are still collecting exited child pids */
      
      dcesptr = dce_sprintf(bss_s_pthread_mutex_unlock_exception,
		            __FILE__, __LINE__, errno);
      (void)strcpy(childWatchThrCleanupArg.msg, dcesptr);
      free(dcesptr);
      if (pthread_mutex_unlock(&bosserverMutex) != 0) {
	  dcesptr = dce_sprintf(bss_s_pthread_mutex_unlock_error_return,
				__FILE__, __LINE__, errno);
	  (void)strcpy(childWatchThrCleanupArg.msg, dcesptr);
	  free(dcesptr);
	  pthread_exit((pthread_addr_t)NULL);
	  /* execution never reaches here */
      }
    }	/* if caught SIGCHLD */
    else {
      /* HERE - can we even get here? */
    }
  }	/* end while TRUE */
  
  /* execution should never reach here */
  pthread_cleanup_pop(1);
  
  return rtnVal;
}

#ifdef POSIX_SUNOS5_SIGWAIT
static void sigchld_handler() {}
#endif

