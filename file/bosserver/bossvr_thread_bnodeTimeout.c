/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: bossvr_thread_bnodeTimeout.c,v $
 * Revision 1.1.100.1  1996/10/02  17:05:05  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:05:07  damon]
 *
 * Revision 1.1.92.1  1994/10/20  19:29:51  sasala
 * 	Implement S12Y in bosserver subcomponent -UYHP: 12618
 * 	[1994/10/20  18:15:30  sasala]
 * 
 * Revision 1.1.90.3  1994/07/13  22:19:59  devsrc
 * 	merged with DCE1.1
 * 	[1994/07/11  15:17:41  devsrc]
 * 
 * 	merged with bl-10
 * 	[1994/06/28  19:58:39  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:54:08  mbs]
 * 
 * Revision 1.1.90.2  1994/06/09  13:52:37  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:26:26  annie]
 * 
 * Revision 1.1.90.2  1994/06/09  13:52:37  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:26:26  annie]
 * 
 * Revision 1.1.90.1  1994/02/04  20:06:34  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:06:48  devsrc]
 * 
 * Revision 1.1.88.1  1993/12/07  17:13:12  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  12:58:47  jaffe]
 * 
 * Revision 1.1.5.4  1993/01/18  19:57:14  cjd
 * 	Embedded copyright notice
 * 	[1993/01/18  19:33:38  cjd]
 * 
 * Revision 1.1.5.3  1992/11/24  15:41:46  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:09:49  bolinger]
 * 
 * Revision 1.1.5.2  1992/08/31  18:33:13  jaffe
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
 * 	[1992/08/28  20:50:56  jaffe]
 * 
 * Revision 1.1.3.3  1992/01/24  01:48:57  devsrc
 * 	Merging dfs6.3
 * 	[1992/01/24  00:03:49  devsrc]
 * 
 * Revision 1.1.3.2  1992/01/23  19:05:43  zeliff
 * 	Moving files onto the branch for dfs6.3 merge
 * 	[1992/01/23  18:21:00  zeliff]
 * 	Revision 1.1.1.2  1992/01/22  19:30:48  devsrc
 * 	Bring in dfs6.3 sources
 * 
 * Revision 1.1  1992/01/19  02:49:29  devrcs
 * 	Initial revision
 * 
 * $EndLog$
*/
/*
 *  bossvr_thread_bnodeTimeout.c -- the code executed by the CMA thread that controls
 * bnode timeouts
 *
 *
 *	Copyright (C) 1991, 1990 Transarc Corporation
 *	All rights reserved.
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>


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

/* the condition variable and mutex for changing bnode timeouts */
IMPORT pthread_cond_t	bnodeTimeoutChangeCond;
IMPORT pthread_cond_t	timeoutThreadReadyCond;

IMPORT int		timeoutThreadReady;

static cleanupArg_t	bnodeTimeoutThrCleanupArg;

#define BOSSVR_MAX_WAIT_TIME	(99)

/*
 *  I don't know why, but the AFS_OSF_ENV seems to need this, and we definitely
 * want to keep the definition as localized as possible.
 */
#if defined(AFS_OSF_ENV)
#define pthread_addr_t void *
#endif /* defined(AFS_OSF_ENV) */

EXPORT pthread_addr_t * BnodeTimeoutWatchThread(ignoredArgP)
     pthread_addr_t *	ignoredArgP;
{
  pthread_addr_t *	rtnVal = (pthread_addr_t *)NULL;
  struct bnode *	bnodeProbeP;
  struct bnode *	nextBnodeCacheP;
  struct timespec	timeoutTime;
  int			condCode;
  int			waitTimedOut = 0;
  long			nowInSecs;
  int			signalCode;
  int			savedErrno;
  char			*dcesptr;   
  
  extern struct bnode *	allBnodes;
  
  strncpy(bnodeTimeoutThrCleanupArg.threadName, "bnodeTimeoutThread",
	  sizeof(bnodeTimeoutThrCleanupArg.threadName));
  bnodeTimeoutThrCleanupArg.msg[0] = '\0';
  bnodeTimeoutThrCleanupArg.releaseLockFlag = BOSSVR_NO_RELEASE_GLOBAL_LOCK;
  pthread_cleanup_push(bossvr_ThreadExit, &bnodeTimeoutThrCleanupArg);

  {
  dcesptr = dce_sprintf(bss_s_exception_in_mutex_lock2,  __FILE__, __LINE__);
  strcpy(bnodeTimeoutThrCleanupArg.msg, dcesptr);
  free(dcesptr);
  }
  if (pthread_mutex_lock(&bosserverMutex) != 0) {
    {
    dcesptr = dce_sprintf(bss_s_error_return_from_mutex_lock2, 
			 __FILE__, __LINE__, errno);
    strcpy(bnodeTimeoutThrCleanupArg.msg, dcesptr);
    free(dcesptr);
    }
    pthread_exit((pthread_addr_t)NULL);
    /* execution never reaches here */
  }

  /* we hold the global lock here, so we should release it if anything happens */
  {
  dcesptr = dce_sprintf(bss_s_excpn_or_cancel_in_pthread_cond_signal2, 
			__FILE__, __LINE__);
  (void)strcpy(bnodeTimeoutThrCleanupArg.msg, dcesptr);
  free(dcesptr);
  }
  bnodeTimeoutThrCleanupArg.releaseLockFlag = BOSSVR_RELEASE_GLOBAL_LOCK;
  timeoutThreadReady = 1;
  signalCode = pthread_cond_signal(&timeoutThreadReadyCond);
  savedErrno = errno;
  if (signalCode != 0) {
    {
    dcesptr = dce_sprintf(bss_s_error_return_from_pthread_cond_signal2,
			  __FILE__, __LINE__, savedErrno);
    (void)strcpy(bnodeTimeoutThrCleanupArg.msg, dcesptr);
    free(dcesptr);
    }
    pthread_exit((pthread_addr_t)NULL);
    /* execution never reaches here */
  }

  while (TRUE) {
    /* we hold the bosserver global mutex, no matter what route we took here */

    /* delay interval calculation */
    timeoutTime.tv_sec = (osi_Time() +
			  BOSSVR_MAX_WAIT_TIME); /* start by assuming max possible */
    timeoutTime.tv_nsec = 0;

    for (bnodeProbeP = allBnodes;
	 bnodeProbeP != (struct bnode *)NULL;
	 bnodeProbeP = bnodeProbeP->next) {
      if (((bnodeProbeP->flags & BNODE_NEEDTIMEOUT) != 0) &&
	  (bnodeProbeP->nextTimeout < timeoutTime.tv_sec)) {
	timeoutTime.tv_sec = bnodeProbeP->nextTimeout;
      }
    }

    /*
     * This thread will wake up from the following timed wait if either the timeout
     * expires or another thread adds the timeout property or changes the nextTimeout
     * of some bnode on the global list.  In the latter case, the timeout time will
     * be recalculated the next time around the loop.
     */

    /* make sure we know what to do if we are cancelled during this timed wait */
    {
    dcesptr = dce_sprintf(bss_s_excptn_or_cancel_during_timedwait,
			    __FILE__, __LINE__);
    strcpy(bnodeTimeoutThrCleanupArg.msg, dcesptr);
    free(dcesptr);
    }

    condCode = pthread_cond_timedwait(&bnodeTimeoutChangeCond,
				      &bosserverMutex,
				      &timeoutTime);
    savedErrno = errno;

    waitTimedOut = ((condCode == -1) && (savedErrno == EAGAIN));
    if ((condCode == -1) && (savedErrno != EAGAIN)) {
      {
      dcesptr = dce_sprintf(bss_s_error_return_from_timedwait,
			    __FILE__, __LINE__, savedErrno);
      (void)strcpy(bnodeTimeoutThrCleanupArg.msg, dcesptr);
      free(dcesptr);
      }
      pthread_exit((pthread_addr_t)NULL);
      /* execution never reaches here */
    }

    /* we hold the global bosserver mutex again, here */

    if (waitTimedOut == 0) {
       dcesptr = dce_sprintf(bss_s_processing_return_from_timed_wait);
       (void)strcpy(bnodeTimeoutThrCleanupArg.msg, dcesptr);
       free(dcesptr);
    }
    else {
       dcesptr = dce_sprintf(bss_s_processing_timeout_return_from_timed_wait);
       (void)strcpy(bnodeTimeoutThrCleanupArg.msg, dcesptr);
       free(dcesptr);
    }
    
    if (waitTimedOut != 0) {
      nowInSecs = osi_Time();
      
      /* bnode timeout processing goes here */
      for (bnodeProbeP = allBnodes;
	   bnodeProbeP != (struct bnode *)NULL;
	   bnodeProbeP = nextBnodeCacheP) {
	if (((bnodeProbeP->flags & BNODE_NEEDTIMEOUT) != 0) &&
	    (nowInSecs > bnodeProbeP->nextTimeout)) {
	  bnode_Hold(bnodeProbeP);
	  BOP_TIMEOUT(bnodeProbeP);
	  bnode_Check(bnodeProbeP);
	  if (bnodeProbeP->flags & BNODE_NEEDTIMEOUT) {
	    /* check again, BOP_TIMEOUT could change */
	    bnodeProbeP->nextTimeout = osi_Time() + bnodeProbeP->period;
	  }
	  nextBnodeCacheP = bnodeProbeP->next;
	  bnode_Release(bnodeProbeP);  /* delete may occur here */
	}	/* end if bnode needs timeout and is ready */
	else {
	  nextBnodeCacheP = bnodeProbeP->next;
	}
      }	/* end bnode list scan (for) */
      
    }	/* end if wait timed out */
  }	/* end while TRUE */
  
  /* execution should never reach here under normal operation */
  pthread_cleanup_pop(1);
  
  return rtnVal;
}
