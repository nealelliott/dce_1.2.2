/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: epi_thread.c,v $
 * Revision 1.1.23.1  1996/10/02  17:28:59  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:34:49  damon]
 *
 * Revision 1.1.18.3  1994/07/13  22:22:36  devsrc
 * 	merged with bl-10
 * 	[1994/06/28  20:50:50  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:57:25  mbs]
 * 
 * Revision 1.1.18.2  1994/06/09  14:03:54  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:38:46  annie]
 * 
 * Revision 1.1.18.1  1994/02/04  20:15:34  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:11:29  devsrc]
 * 
 * Revision 1.1.16.1  1993/12/07  17:21:11  jaffe
 * 	New File from Transarc 1.0.3a
 * 	[1993/12/02  20:55:08  jaffe]
 * 
 * $EndLog$
 */
/* Copyright (C) 1995, 1993 Transarc Corporation - All rights reserved. */
/*
 * Some wrappers around pthread calls used by Episode test programs
 * and utilities; replaces the now-defunct LWP package.
 */
#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/sysincludes.h>

#include <epi_thread.h>

RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/episode/tools/epi_thread.c,v 1.1.23.1 1996/10/02 17:28:59 damon Exp $")

#ifdef OSI_TRACE_SYNC
/* This wouldn't really work if done in a build from scratch due to the
 * dependency loop between the osi and icl layers.  However, for dianostic
 * purposes this should be fine. */
 
#include <dcedfs/icl.h>
#include "ethr_trace.h"

extern struct icl_set *osi_lockInitSet;
extern struct icl_set *osi_lockSet;
#endif

void epi_Init (void)
{
#ifdef OSI_TRACE_SYNC
    osi_InitLockTrace_r();
#endif
}

#ifndef OSI_TRACE_SYNC
/* If tracing is off, just turn these procedures into straight through calls */
#undef icl_Trace1
#undef icl_Trace2
#define icl_Trace1(a, b, c1, d1) /* */
#define icl_Trace2(a, b, c1, d1, c2, d2) /* */
#endif

/* procedurize these for better tracing */
epi_MutexInit (epi_mutex_t *m, const char *s)
{
    icl_Trace2(osi_lockInitSet, TR_ETHR_MUTEX_INIT,
	       ICL_TYPE_STRING, s, ICL_TYPE_POINTER, m);
    osi_mutex_init(m);
}

epi_MutexLock (epi_mutex_t *m)
{
    afsl_Assert (*((long *)m) != 0);
    if (!osi_mutex_enter_no_wait(m)) {
	icl_Trace1(osi_lockSet, TR_ETHR_MUTEX_BLOCK,
		   ICL_TYPE_POINTER, m);
	epi_mutex_PreemptionOn();
	osi_mutex_enter(m);
	epi_mutex_NoPreemption();
	icl_Trace1(osi_lockSet, TR_ETHR_MUTEX_HOLD,
		   ICL_TYPE_POINTER, m);
    } else {
	icl_Trace1(osi_lockSet, TR_ETHR_MUTEX_GRAB,
		   ICL_TYPE_POINTER, m);
    }
}

epi_MutexUnlock (epi_mutex_t *m)
{
    icl_Trace1(osi_lockSet, TR_ETHR_MUTEX_RELE,
	       ICL_TYPE_POINTER, m);
    osi_mutex_exit(m);
}

epi_CondInit (epi_cond_t *cv, const char *s)
{
    icl_Trace2(osi_lockInitSet, TR_ETHR_COND_INIT,
	       ICL_TYPE_STRING, s, ICL_TYPE_POINTER, cv);
    osi_cv_init(cv);
}

epi_CondWait (epi_cond_t *cv, epi_mutex_t *m)
{
    icl_Trace1(osi_lockSet, TR_ETHR_MUTEX_RELE,
	       ICL_TYPE_POINTER, m);
    icl_Trace1(osi_lockSet, TR_ETHR_COND_WAIT,
	       ICL_TYPE_POINTER, cv);
    epi_mutex_PreemptionOn();
    osi_cv_wait(cv,m);
    epi_mutex_NoPreemption();
    icl_Trace1(osi_lockSet, TR_ETHR_COND_WAKE,
	       ICL_TYPE_POINTER, cv);
    icl_Trace1(osi_lockSet, TR_ETHR_MUTEX_GRAB,
	       ICL_TYPE_POINTER, m);
}

epi_CondSignal (epi_cond_t *cv)
{
    afsl_Assert (*((long *)cv) != 0);
    icl_Trace1(osi_lockSet, TR_ETHR_COND_SIGNAL,
	       ICL_TYPE_POINTER, cv);
    osi_cv_signal(cv);
}

epi_CondBroadcast (epi_cond_t *cv)
{
    afsl_Assert (*((long *)cv) != 0);
    icl_Trace1(osi_lockSet, TR_ETHR_COND_BROADCAST,
	       ICL_TYPE_POINTER, cv);
    osi_cv_broadcast(cv);
}

#ifndef KERNEL

int epi_threadScheduler = SCHED_OTHER;
int epi_threadQuantum = 0;

/*
 * Create a new thread with specified attributes.
 */
int
epi_ThreadCreate(
   pthread_startroutine_t start_fun,
   int stacksize, 
   int priority,
   pthread_addr_t arg,
   pthread_t *tid)
{
    int status;
    pthread_attr_t attr;
    pthread_t handle;
    int code;

    code = pthread_attr_create(&attr);
    assert(code == 0);
    if (stacksize != 0) {
	code = pthread_attr_setstacksize(&attr, stacksize);
	assert(code == 0);
    }
    code = pthread_attr_setinheritsched(&attr, PTHREAD_DEFAULT_SCHED);
    assert (!code || errno == ENOSYS);

    /* Set sched before prio since legal priorities depend on which scheduling
     * policy is in effect. */

    code = pthread_attr_setsched(&attr, epi_threadScheduler);
    assert (!code || errno == ENOSYS);
    code = pthread_attr_setprio(&attr, epi_ActualPriority(priority));
    assert (!code || errno == ENOSYS);

    status = pthread_create(tid, attr, start_fun, arg);
    code = pthread_attr_delete(&attr);
    assert(code == 0);

    if (epi_threadQuantum > 0) {
# if (_CMA_OS_ == _CMA__UNIX) && defined(AFS_AIX_ENV)

	/* This special has only been checked on AIX.  It definitely won't work
         * on Solaris. */

	struct timeval quantum;
	struct itimerval timer;
	static struct itimerval oldtimer;
	long code;

	extern struct timeval cma__g_one_tick;

	if (epi_threadQuantum < 10000) {
	    printf ("Can't set quantum below 10msec.\n");
	    epi_threadQuantum = 10000;
	}

	quantum.tv_sec = epi_threadQuantum / 1000000;
	quantum.tv_usec = epi_threadQuantum % 1000000;
	code = getitimer (ITIMER_VIRTUAL, &timer);
	if (code) perror ("getitimer");
	timer.it_value = quantum;
	timer.it_interval = quantum;
	code = setitimer (ITIMER_VIRTUAL, &timer, &oldtimer);
	if (code) perror ("setitimer");
	cma__g_one_tick = quantum;
#else
	printf ("Can't change non-CMA scheduler quantum\n");
#endif
    }
    return status;
}
#endif
