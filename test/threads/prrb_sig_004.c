/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: prrb_sig_004.c,v $
 * Revision 1.1.58.2  1996/02/17  23:31:37  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:48:06  marty]
 *
 * Revision 1.1.58.1  1995/12/11  23:02:52  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:22  root]
 * 
 * Revision 1.1.56.1  1993/10/12  19:37:23  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:55  rhw]
 * 
 * Revision 1.1.4.2  1992/12/30  16:53:33  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:11:50  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/15  16:54:38  bolinger
 * 	Fix OT defect 2919:  add testcase from defect 1673, which checks
 * 	sigwait() in a newly-created thread, when the signal involved
 * 	was generated before the thread was created.
 * 	[1992/05/15  13:56:31  bolinger]
 * 
 * $EndLog$
 */
/*
 * Copyright (C) 1991 Transarc Corp.
 * All Rights Reserved
 */
/*
 * prrb_sig_004.c: This program verifies that sigwait() and pthread_join()
 *    work together.
 */
#include <pthread.h>
#include <stdio.h>
#include <signal.h>
#include <dce/cts_test.h>

char *msg = "\
\n    We expect to return form the next sigwait() waiting on SIGINT since\
\n    a SIGINT signal is pending. If sigwait() does not return or exception\
\n    occurs, it is an error. If exception does not occur, use ^C to cause\
\n    a new SIGINT signal and the program will terminate normally.\n\n";

WaitThread()
{
    sigset_t waitUpon;
    int sig;

    (void) sigemptyset(&waitUpon);
    (void) sigaddset(&waitUpon, SIGHUP);
    (void) sigaddset(&waitUpon, SIGINT);
    cts_comment("sigwait: waits on signals SIGHUP(1), SIGINT(2)");
    if ((sig = sigwait(&waitUpon)) == -1)
		cts_failed("sigwait: error %d", errno);
    else
		cts_comment("sigwait: returned because of signal %d", sig);
    cts_comment(msg);
    (void) sigemptyset(&waitUpon);
    (void) sigaddset(&waitUpon, SIGINT);
    cts_comment("sigwait: waits on signal SIGINT");
    if ((sig = sigwait(&waitUpon)) == -1)
		cts_failed("sigwait: error %d", errno);
    else
		cts_comment("sigwait: returned because of signal %d", sig);
}

main()
{
    pthread_t tid;
    sigset_t sigMask;
    void *result;

    cts_test("PRRB_SIG_004", "Test sigwait()/pthread_join() interaction");

    (void) sigfillset(&sigMask);
    if (sigprocmask(SIG_SETMASK, &sigMask, NULL) == -1) {
		cts_failed("sigprocmask: error %d", errno);
		exit(1);
    }

    if (kill(getpid(), SIGHUP) == -1) {
		cts_failed("kill: error %d, signal SIGHUP", errno);
		exit(1);
	}
    else
		cts_comment(stderr, "kill: sent signal SIGHUP");

    if (kill(getpid(), SIGINT) == -1) {
		cts_failed("kill: error %d, signal SIGINT", errno);
		exit(1);
	}
    else
		cts_comment("kill: sent signal SIGINT");

    if (pthread_create(&tid, pthread_attr_default, WaitThread, NULL) == -1) {
		cts_failed("pthread_create: error %d", errno);
		exit(2);
    }

    if (pthread_join(tid, &result) == -1) {
		cts_failed("pthread_join: error %d", errno);
		exit(3);
    }
	cts_result();
    exit(0);
}
