/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: prrb_sig_002.c,v $
 * Revision 1.1.38.2  1996/02/17  23:31:35  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:48:04  marty]
 *
 * Revision 1.1.38.1  1995/12/11  23:02:46  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:20  root]
 * 
 * Revision 1.1.36.1  1994/04/01  20:11:40  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:18:39  jd]
 * 
 * Revision 1.1.34.1  1993/10/12  19:37:20  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:54  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:53:24  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:11:43  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:22:31  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  Copyright (c) 1991 by
 *  Digital Equipment Corporation, Maynard Massachusetts.
 *  All rights reserved.
 *
 *  This software is furnished under a license and may be used and copied
 *  only in  accordance  with  the  terms  of  such  license and with the
 *  inclusion of the above copyright notice.  This software or  any other
 *  copies thereof may not be provided or otherwise made available to any
 *  other person.  No title to and ownership of  the  software  is hereby
 *  transferred.
 *
 *  The information in this software is subject to change  without notice
 *  and should  not  be  construed  as  a commitment by DIGITAL Equipment
 *  Corporation.
 *
 *  DIGITAL assumes no responsibility for the use or  reliability  of its
 *  software on equipment which is not supplied by DIGITAL.
 */

/*
 *  FACILITY:
 *
 *	CMA Test Programs
 *
 *  FILENAME:
 *
 *	PRRB_SIG_002
 *
 *  TEST TOPICS:
 *
 *	SIG  -  U*IX signal handling
 *
 *  ABSTRACT:
 *
 *	Repeating SIGALARMS appear to be missed by sigwait() unless
 *	the interval is long.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	09 September 1991
 *
 *  MODIFICATION HISTORY:
 *
 */

/*
 *  INCLUDE FILES
 */

#include <pthread.h>
#include <dce/cts_test.h>
#include <cma_stdio.h>
#include <signal.h>
#include <sys/time.h>

#define PrintUsage() \
    printf("Usage: alarmtest\n");
 
#define CHECK_ERROR(call, msg) \
        if ((call) == -1) {\
            cts_failed("pthread_%s: Error Code (%d)\n", msg, errno);\
            exit (1);\
        }
 
pthread_mutex_t waitLock;
pthread_cond_t  waitCV;
 
int alarmWaiterReady = 0;
int signalCount = 0;
 
void AlarmWaiter();
void AlarmSigHandler();
void SetTimer();
 
/*
 * This function runs as a separate thread waiting for a SIGALRM to occur.
 */
void AlarmWaiter()
{
    sigset_t waitingOn;
    int sigReceived;
 
    (void) sigemptyset(&waitingOn);
    (void) sigaddset(&waitingOn, SIGALRM);
    (void) sigprocmask(SIG_BLOCK, &waitingOn, NULL);
 
    CHECK_ERROR(pthread_mutex_lock(&waitLock), "mutex_lock");
    alarmWaiterReady = 1;
    CHECK_ERROR(pthread_cond_signal(&waitCV), "cond_signal");
    CHECK_ERROR(pthread_mutex_unlock(&waitLock), "mutex_unlock");
 
    while (1) {
        sigReceived = sigwait(&waitingOn);
        if (sigReceived == SIGALRM)
            AlarmSigHandler();
        else
            cts_comment("WaitForAlarmSig: spurious wake up by signal %d.\n",
                      sigReceived);
    }
}
 
void AlarmSigHandler()
{
    CHECK_ERROR(pthread_mutex_lock(&waitLock), "mutex_lock");
    if (++signalCount > 10)
        CHECK_ERROR(pthread_cond_signal(&waitCV), "cond_signal");
    CHECK_ERROR(pthread_mutex_unlock(&waitLock), "mutex_unlock");
}
 
void SetTimer()
{
    struct timeval interval;
    struct itimerval it,old;
 
    interval.tv_sec =  0;
    interval.tv_usec = 500000;  /* USEC_PER_INTERRUPT */
 
    it.it_interval = interval;
    it.it_value = interval;
 
    if (setitimer(ITIMER_REAL,&it,&old) == -1) {
        cts_failed("SetTimer: Cannot set itimer\n");
        exit(1);
    }
}

int
main	(int argc, char *argv[], char *envp[])
    {
    pthread_t alarmWaiter;
    int err;
    void *result;
 
    if ((argc > 1) && (argv[1][0] == '-') && (argv[1][1] == 'u')) {
        PrintUsage();
        exit(0);
    }

    cts_test ("PRRB_SIG_002", "Orbit #2266 (sigwait on SIGALARM)");

    signalCount = 0;
    CHECK_ERROR(pthread_mutex_init(&waitLock, pthread_mutexattr_default),
                      "mutex_init");
    CHECK_ERROR(pthread_cond_init(&waitCV, pthread_condattr_default),
                      "cond_init");
 
    CHECK_ERROR(pthread_create(
	    &alarmWaiter,
	    pthread_attr_default,
	    (pthread_startroutine_t)AlarmWaiter,
	    (pthread_addr_t)0), "create");
 
    CHECK_ERROR(pthread_mutex_lock(&waitLock), "mutex_lock");
    while (alarmWaiterReady == 0)
        CHECK_ERROR(pthread_cond_wait(&waitCV, &waitLock), "cond_wait");
    CHECK_ERROR(pthread_mutex_unlock(&waitLock), "mutex_unlock");
 
    SetTimer(); /* alarm goes off repeatedly */
 
#if 1
    CHECK_ERROR(pthread_mutex_lock(&waitLock), "mutex_lock");
    while (signalCount < 10)
        CHECK_ERROR(pthread_cond_wait(&waitCV, &waitLock), "cond_wait");
    CHECK_ERROR(pthread_mutex_unlock(&waitLock), "mutex_unlock");
#else  /* CMA_BUG */
    while (signalCount < 10)
        pthread_yield();
#endif /* CMA_BUG */

    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element PRRB_SIG_002.C */
/*  *1    10-SEP-1991 11:47:21 BUTENHOF "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element PRRB_SIG_002.C */

