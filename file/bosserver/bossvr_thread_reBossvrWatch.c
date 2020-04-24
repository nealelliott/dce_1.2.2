/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: bossvr_thread_reBossvrWatch.c,v $
 * Revision 1.1.82.1  1996/10/02  17:05:10  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:05:09  damon]
 *
 * Revision 1.1.74.1  1994/10/20  19:29:54  sasala
 * 	Implement S12Y in bosserver subcomponent -UYHP: 12618
 * 	[1994/10/20  18:15:33  sasala]
 * 
 * Revision 1.1.72.3  1994/07/13  22:20:04  devsrc
 * 	merged with DCE1.1
 * 	[1994/07/11  15:17:43  devsrc]
 * 
 * 	merged with bl-10
 * 	[1994/06/28  19:58:42  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:54:10  mbs]
 * 
 * Revision 1.1.72.2  1994/06/09  13:52:40  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:26:31  annie]
 * 
 * Revision 1.1.72.2  1994/06/09  13:52:40  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:26:31  annie]
 * 
 * Revision 1.1.72.1  1994/02/04  20:06:39  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:06:50  devsrc]
 * 
 * Revision 1.1.70.1  1993/12/07  17:13:16  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  12:59:21  jaffe]
 * 
 * Revision 1.1.5.4  1993/01/18  19:57:24  cjd
 * 	Embedded copyright notice
 * 	[1993/01/18  19:33:54  cjd]
 * 
 * Revision 1.1.5.3  1992/11/24  15:41:53  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:10:00  bolinger]
 * 
 * Revision 1.1.5.2  1992/08/31  18:33:28  jaffe
 * 	Transarc delta: bab-ot3956-bosserver-helper-thread-lock-release-on-cancel 1.1
 * 	  Selected comments:
 * 	    The code for the bosserver helper threads (incorrectly) assumed, if
 * 	    a pthread_cond_timedwait was cancelled, the lock would come back not
 * 	    held.  In such a case, the cleanup routine would not release the lock,
 * 	    making the bosserver eventually deadlock.  This delta changes the code
 * 	    so that the lock is released in such a case.
 * 	    ot 3956
 * 	    Change the flag in the cleanup routine argument to release the global lock.
 * 	Transarc delta: bab-ot4386-bosserver-child-signals-not-rcvd 1.2
 * 	  Selected comments:
 * 	    This delta reverses part of delta dstokes-ot2171-remove-cma-calls,
 * 	    since that delta broke the bosserver.  The bosserver was no longer
 * 	    seeing any SIGCHLD signals when sigwait was used in place
 * 	    of cma_sigwait, so the call to cma_sigwait was restored.
 * 	    ot 4386
 * 	    It turns out that the name of this delta is more than a bit of a
 * 	    misnomer, but the symptoms were that killed children were not being
 * 	    restarted.  It turns out that there is some sort of optimizer bug that
 * 	    was tickled by the way the error handling structures were manipulated in
 * 	    bossvr_thread_bnodeTimeout.c.  To work around this, the cleanup structures
 * 	    were changed, as was the way they were used.
 * 	    I also cleaned up some of the copious ifdefs.
 * 	    See above.
 * 	Transarc delta: jaffe-ot3211-fix-prototypes 1.1
 * 	  Selected comments:
 * 	    Remove ANSI-C declaration files which contained prototypes for standard
 * 	    functions.  We expect these prototypes to be defined by the system include
 * 	    files.
 * 	    use stdio instead of stdioDecls.h
 * 	[1992/08/28  20:51:47  jaffe]
 * 
 * Revision 1.1.3.3  1992/01/24  01:49:00  devsrc
 * 	Merging dfs6.3
 * 	[1992/01/24  00:03:55  devsrc]
 * 
 * Revision 1.1.3.2  1992/01/23  19:05:53  zeliff
 * 	Moving files onto the branch for dfs6.3 merge
 * 	[1992/01/23  18:21:15  zeliff]
 * 	Revision 1.1.1.2  1992/01/22  19:30:55  devsrc
 * 	Bring in dfs6.3 sources
 * 
 * Revision 1.1  1992/01/19  02:49:37  devrcs
 * 	Initial revision
 * 
 * $EndLog$
*/
/*
 *  bossvr_thread_reBossvrWatch.c -- the code executed by the CMA thread that controls when
 * the bosserver (and its children) will be restarted
 *
 *
 *	Copyright (C) 1991, 1990 Transarc Corporation
 *	All rights reserved.
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/ktime.h>

#include <dce/rpc.h>

#include <pthread.h>

#include <bbos_bnode.h>
#include <bbos_util.h>
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsbssmac.h>
#include <dfsbsssvc.h>
#include <dfsbssmsg.h>

IMPORT void bossvr_ThreadExit _TAKES((
				    void * argP
				  ));

/* bosserver's global mutex to coordinate its threads */
IMPORT pthread_mutex_t	bosserverMutex;

#define	BOSSVR_MINSKIP 3600	    /* minimum to advance clock */

IMPORT pthread_cond_t	newRestartTimeCond;
IMPORT pthread_cond_t	reBossvrThreadReadyCond;

IMPORT long	execOnRebossvr;
IMPORT int	reBossvrThreadReady;
IMPORT int	restartTimesValid;

static cleanupArg_t	reBossvrThrCleanupArg;
static long		reBossvrNoerrStatus = 0;

  char *dcesptr;  

/*
 *  I don't know why, but the AFS_OSF_ENV seems to need this, and we definitely
 * want to keep the definition as localized as possible.
 */
#if defined(AFS_OSF_ENV)
#define pthread_addr_t void *
#endif /* defined(AFS_OSF_ENV) */

static int bdrestart(abnode, arock)
     register struct bnode *	abnode;
     char *			arock; 
{
  register long	code;
  int		rtnVal = 0;
  
  if ((abnode->fileGoal == BSTAT_NORMAL) && (abnode->goal == BSTAT_NORMAL)) {
    bnode_Hold(abnode);
    code = bnode_RestartP(abnode);
    if (code) {
      /* restart the dude */
      bnode_SetStat(abnode, BSTAT_SHUTDOWN);
      bnode_WaitStatus(abnode, BSTAT_SHUTDOWN);
      bnode_SetStat(abnode, BSTAT_NORMAL);
    }
    bnode_Release(abnode);
  }
  
  return rtnVal;	/* keep trying all bnodes */
}

EXPORT pthread_addr_t * ReBossvrWatchThread(ignoredArgP)
     pthread_addr_t *	ignoredArgP;
{
  pthread_addr_t *	rtnVal = (pthread_addr_t *)NULL;
  long			nowInSecs;
  long			nextRestart;
  long			nextDay;
  struct timespec	wakeupTime;
  int			wakeupTimedout;
  int			restartWaitCode;
  int			signalCode;
  int			savedErrno;
  error_status_t	ncs_error_status;
  static char		routineName[] = "ReBossvrWatchThread";
  
  IMPORT struct ktime	bossvr_nextRestartKT;
  IMPORT struct ktime	bossvr_nextDayKT;
  
  strncpy(reBossvrThrCleanupArg.threadName, "reBossvrWatchThread",
	  sizeof(reBossvrThrCleanupArg.threadName));
  reBossvrThrCleanupArg.msg[0] = '\0';
  pthread_cleanup_push(bossvr_ThreadExit, &reBossvrThrCleanupArg);
  
  /* intitialization code */
   
  {
  /*char *dcesptr;  */
  dcesptr = dce_sprintf(bss_s_exception_in_mutex_lock3,
			__FILE__, __LINE__, errno);
  (void)strcpy(reBossvrThrCleanupArg.msg, dcesptr);
  free(dcesptr);
  }
  reBossvrThrCleanupArg.releaseLockFlag = BOSSVR_NO_RELEASE_GLOBAL_LOCK;
  if (pthread_mutex_lock(&bosserverMutex) != 0) {
    {
    dcesptr = dce_sprintf(bss_s_error_return_from_mutex_lock3,
			  __FILE__, __LINE__, errno);
    (void)strcpy(reBossvrThrCleanupArg.msg, dcesptr);
    free(dcesptr);
    }
    pthread_exit(&reBossvrNoerrStatus);
    /* execution never reaches here */
  }

  /* we now have the global lock, so anybody who makes a mistake should release it */
  reBossvrThrCleanupArg.releaseLockFlag = BOSSVR_RELEASE_GLOBAL_LOCK;
  {
  dcesptr = dce_sprintf(bss_s_excpn_or_cancel_in_pthread_cond_signal3,
			__FILE__, __LINE__);
  (void)strcpy(reBossvrThrCleanupArg.msg, dcesptr);
  free(dcesptr);
  }
  reBossvrThreadReady = 1;
  signalCode = pthread_cond_signal(&reBossvrThreadReadyCond);
  savedErrno = errno;
  if (signalCode != 0) {
    {
    dcesptr = dce_sprintf(bss_s_error_return_from_pthread_cond_signal3,
			  __FILE__, __LINE__, savedErrno);
    (void)strcpy(reBossvrThrCleanupArg.msg, dcesptr);
    free(dcesptr);
    }
    pthread_exit(&reBossvrNoerrStatus);
    /* execution never reaches here */
  }
  
  /* wait for the main thread to signal this thread that the vars are init'd */
  {
  dcesptr = dce_sprintf(bss_s_excptn_or_cancel_in_cond_wait,
			__FILE__, __LINE__);
  (void)strcpy(reBossvrThrCleanupArg.msg, dcesptr);
  free(dcesptr);
  }
  while (restartTimesValid == 0) {
    restartWaitCode = pthread_cond_wait(&newRestartTimeCond, &bosserverMutex);
    savedErrno = errno;
    
    if (restartWaitCode != 0) {
      {
      dcesptr = dce_sprintf(bss_s_error_return_from_cond_wait,
			    __FILE__, __LINE__, savedErrno);
      (void)strcpy(reBossvrThrCleanupArg.msg, dcesptr);
      free(dcesptr);
      }
      pthread_exit(&reBossvrNoerrStatus);
      /* execution never reaches here */
    }
  }

  wakeupTime.tv_nsec = 0;
  nextRestart = ktime_next(&bossvr_nextRestartKT, BOSSVR_MINSKIP);
  nextDay = ktime_next(&bossvr_nextDayKT, BOSSVR_MINSKIP);
  wakeupTime.tv_sec = (nextRestart < nextDay) ? nextRestart : nextDay;
  wakeupTime.tv_nsec = 0;
  
  while (TRUE) {
    /* we hold the bosserverMutex here, no matter what route we took here */

    /* make sure we know what to do if we are cancelled during this timed wait */
    {
    dcesptr = dce_sprintf(bss_s_excptn_or_cancel_during_timedwait2,
			    __FILE__, __LINE__);
    strcpy(reBossvrThrCleanupArg.msg, dcesptr);
    free(dcesptr);
    }

    restartWaitCode = pthread_cond_timedwait(&newRestartTimeCond,
					     &bosserverMutex,
					     &wakeupTime);
    savedErrno = errno;
    nowInSecs = osi_Time();

    /* we have the global lock back again here */

    wakeupTimedout = ((restartWaitCode == -1) && (savedErrno == EAGAIN));
    if ((restartWaitCode == -1) && (savedErrno != EAGAIN)) {
      {
      dcesptr = dce_sprintf(bss_s_error_return_from_timedwait2,
			    __FILE__, __LINE__, savedErrno);
      (void)strcpy(reBossvrThrCleanupArg.msg, dcesptr);
      free(dcesptr);
      }
      pthread_exit(&reBossvrNoerrStatus);
      /* execution never reaches here */
    }

    if (wakeupTimedout == 0) {
      {
      dcesptr = dce_sprintf(bss_s_processing_return_from_timed_wait2);
      (void)strcpy(reBossvrThrCleanupArg.msg, dcesptr);
      free(dcesptr);
      }
    }
    else  {
      {
      dcesptr = dce_sprintf(bss_s_processing_timeout_return_from_timed_wait2);
      (void)strcpy(reBossvrThrCleanupArg.msg, dcesptr);
      free(dcesptr);
      }
    }

    if (restartWaitCode == 0) {
      /* we have a new wakeup time to register */
      nextRestart = ktime_next(&bossvr_nextRestartKT, BOSSVR_MINSKIP);
      nextDay = ktime_next(&bossvr_nextDayKT, BOSSVR_MINSKIP);
    }

    /* check for restarts in any case */
    if (nowInSecs >= nextRestart) {
      execOnRebossvr = 1;
      rpc_mgmt_stop_server_listening((rpc_binding_handle_t)NULL, &ncs_error_status);
      /* NB: RPC threads may not be shutdown here, but that doesn't really matter */
      if (ncs_error_status != rpc_s_ok) {
	/* log the error and go on; we are about to shutdown anyway */
	bossvr_SvcLog(bss_s_server_shutdown_error,
		      routineName, ncs_error_status);
      }

      /* the cleanup routine will release the lock */
      /* exit self */
      {
      dcesptr = dce_sprintf(bss_s_simple_restart_exit_no_error);
						 /* cleanup msg should look harmless */
      strcpy(reBossvrThrCleanupArg.msg, dcesptr);
      free(dcesptr);
      }
      pthread_exit(&reBossvrNoerrStatus);
    }
    /* see if we should restart a server, but only if we aren't restarting everything */
    else if (nowInSecs >= nextDay) {
      nextDay = ktime_next(&bossvr_nextDayKT, BOSSVR_MINSKIP);
      
      /* call the bnode restartp function, and restart all that require it */
      bnode_ApplyInstance(bdrestart, (char *)NULL);
    }

    /* calculate time for the next wakeup */
    wakeupTime.tv_sec = (nextRestart < nextDay) ? nextRestart : nextDay;
  }

  /* execution should never reach here */
  pthread_cleanup_pop(1);
  
  return rtnVal;
}
