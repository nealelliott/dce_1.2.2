/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* Copyright (C) 1995, 1991 Transarc Corporation - All rights reserved. */
/* $Header: /u0/rcs_trees/dce/rcs/file/episode/tools/epi_thread.h,v 1.1.24.1 1996/10/02 17:29:01 damon Exp $ */
/*
 * HISTORY
 * $Log: epi_thread.h,v $
 * Revision 1.1.24.1  1996/10/02  17:29:01  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:34:49  damon]
 *
 * Revision 1.1.19.2  1994/06/09  14:03:55  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:38:47  annie]
 * 
 * Revision 1.1.19.1  1994/02/04  20:15:36  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:11:30  devsrc]
 * 
 * Revision 1.1.17.1  1993/12/07  17:21:12  jaffe
 * 	New File from Transarc 1.0.3a
 * 	[1993/12/02  20:55:13  jaffe]
 * 
 * $EndLog$
 */

#ifndef TRANSARC_EPI_THREAD_H
#define TRANSARC_EPI_THREAD_H 1

#include <dcedfs/lock.h>
#ifndef KERNEL
#include <pthread.h>
#endif /* !KERNEL */

extern void epi_Init (void);

/* In the kernel, on SUNOS osi_mutex_enter assumes preemption is on while on
 * AIX it assumes a non-preemptive environment. Go figure! */

/* Avoid preemption lock within Episode on SunOS5. */

#if defined(AFS_SUNOS5_ENV) && defined(KERNEL)
#define epi_mutex_NoPreemption() /* osi_PreemptionOff() */
#define epi_mutex_PreemptionOn() /* osi_RestorePreemption(0) */
#else
#define epi_mutex_NoPreemption()
#define epi_mutex_PreemptionOn()
#endif

#define epi_mutex_t osi_mutex_t
#define epi_cond_t osi_cv_t

#define epi_mutex_trylock(m) osi_mutex_enter_no_wait(m)
#define epi_mutex_held(m) osi_mutex_held(m)
#define epi_mutex_hasowner(m) osi_mutex_hasowner(m)
#define epi_mutex_destroy(m) osi_mutex_destroy(m)

#define epi_cond_destroy(cv) osi_cv_destroy(cv)

/* These functions are replaced with calls to wrapper functions if we are
 * requested to trace synchronization primitives. */

#ifndef OSI_TRACE_SYNC

#define epi_mutex_init(m,s) osi_mutex_init(m)
#define epi_mutex_lock(m) \
    MACRO_BEGIN \
        if (!epi_mutex_trylock(m)) { \
	    epi_mutex_PreemptionOn(); \
	    osi_mutex_enter(m); \
	    epi_mutex_NoPreemption(); \
	} \
    MACRO_END
#define epi_mutex_unlock(m) osi_mutex_exit(m)

#define epi_cond_init(cv,s) osi_cv_init(cv)
#define epi_cond_wait(cv,m) \
    MACRO_BEGIN \
        epi_mutex_PreemptionOn(); \
	osi_cv_wait(cv,m); \
	epi_mutex_NoPreemption(); \
    MACRO_END
#define epi_cond_broadcast(cv) osi_cv_broadcast(cv)
#define epi_cond_signal(cv) osi_cv_signal(cv)

#else /* not OSI_TRACE_SYNC */

#define epi_mutex_init(m,s) epi_MutexInit(m,s)
#define epi_mutex_lock(m) epi_MutexLock(m)
#define epi_mutex_unlock(m) epi_MutexUnlock(m)
#define epi_cond_init(cv,s) epi_CondInit(cv,s)
#define epi_cond_wait(cv,m) epi_CondWait(cv,m)
#define epi_cond_signal(cv) epi_CondSignal(cv)
#define epi_cond_broadcast(cv) epi_CondBroadcast(cv)
#endif

#ifndef KERNEL

/* Maximum priority permissible */
#define EPI_MAX_PRIORITY	PRI_OTHER_MAX

/* Usual priority used by user LWPs */
#define EPI_NORMAL_PRIORITY	(EPI_MAX_PRIORITY - 2)

/* We need to be able to correct specified priority for the current scheduling
 * policy.  However, only "OTHER" exists on Solaris. */
#ifdef PRI_RR_MAX
#define epi_CurrentMaxPriority() \
    (epi_threadScheduler == SCHED_OTHER ? PRI_OTHER_MAX : \
     (epi_threadScheduler == SCHED_FIFO ? PRI_FIFO_MAX : \
      (epi_threadScheduler == SCHED_RR ? PRI_RR_MAX : \
       0)))
#define epi_ActualPriority(p) (epi_CurrentMaxPriority()-(EPI_MAX_PRIORITY-(p)))
#else
#define epi_CurrentMaxPriority() EPI_MAX_PRIORITY
#define epi_ActualPriority(p) (p)
#endif

/* These are initialized and used by epi_ThreadCreate.  They can be altered by
 * runtest.c */

extern int epi_threadScheduler;
extern int epi_threadQuantum;

extern int epi_ThreadCreate(
   pthread_startroutine_t start_fun,
   int stacksize, 
   int priority,
   pthread_addr_t arg,
   pthread_t *tid);

#endif /* !KERNEL */
#endif /* !TRANSARC_EPI_THREAD_H */
