/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: prrb_sig_001.c,v $
 * Revision 1.1.37.2  1996/02/17  23:31:34  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:48:03  marty]
 *
 * Revision 1.1.37.1  1995/12/11  23:02:43  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:19  root]
 * 
 * Revision 1.1.35.1  1994/04/01  20:11:38  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:18:34  jd]
 * 
 * Revision 1.1.33.1  1993/10/12  19:37:19  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:53  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:53:19  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:11:39  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:22:30  devrcs
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
 *	PRRB_SIG_001
 *
 *  TEST TOPICS:
 *
 *	SIG  -  U*IX signal handling
 *
 *  ABSTRACT:
 *
 *	In earlier baselevels of DECthreads, sigwait could become
 *	confused when multiple signals were pending, and fail to
 *	recognize any but the first. Assure that this bug remains
 *	fixed.
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
#include <stdio.h>
#include <signal.h>

int doneFlag = 0;
 
char *msg1 = "\
\n    We just received a pending SIGHUP signal.  The next sigwait() call\
\n    will wait on SIGINT, which should still be pending.  If sigwait()\
\n    does not return, something is wrong.  Press ^C to generate a new\
\n    SIGINT signal and the program will terminate normally.\n\n";
char *msg2 = "\
\n    We just received a pending SIGINT signal.  The next sigwait() call\
\n    will wait on SIGHUP, which should still be pending.  If sigwait()\
\n    does not return, something is wrong.  Press ^Z to pause the program\
\n    and remove the program with an explicit kill(1) call\n\n";
 
WaitThread()
{
    sigset_t waitUpon;
    int sig;
 
    (void) sigemptyset(&waitUpon);
    (void) sigaddset(&waitUpon, SIGHUP);
    (void) sigaddset(&waitUpon, SIGINT);
    cts_comment("sigwait: waits on signals SIGHUP(%d), SIGINT(%d)\n",
	    SIGHUP, SIGINT);
    if ((sig = sigwait(&waitUpon)) == -1) {
        cts_failed("sigwait: error (%d)\n", errno);
    } else {
        cts_comment("sigwait: returned because of signal %d (%s)\n",
		sig, sig == SIGHUP ? "SIGHUP" : "SIGINT");
    }
 
    (void) sigemptyset(&waitUpon);
    if (sig == SIGHUP) {
	fprintf(stderr, msg1);
	(void) sigaddset(&waitUpon, SIGINT);
    } else {
	fprintf(stderr, msg2);
	(void) sigaddset(&waitUpon, SIGINT);
    }
 
    cts_comment("sigwait: waiting for signal %s\n",
	    sig == SIGINT ? "SIGHUP" : "SIGINT");
    if ((sig = sigwait(&waitUpon)) == -1) {
        cts_failed ("sigwait: error (%d)\n", errno);
    } else {
        cts_comment ("sigwait: returned because of signal %d\n", sig);
    }
    doneFlag = 1;
}

int
main	(int argc, char *argv[], char *envp[])
    {
    pthread_t tid;
    sigset_t sigMask;

    cts_test ("PRRB_SIG_001", "Orbit #2190 (sigwait with multiple pending)");

    (void) sigemptyset(&sigMask);
    (void) sigaddset(&sigMask, SIGHUP);
    (void) sigaddset(&sigMask, SIGINT);

    if (sigprocmask(SIG_SETMASK, &sigMask, NULL) == -1) {
        cts_failed("sigprocmask: error (%d)\n", errno);
        exit(1);
	}

    fprintf(stderr, "sigprocmask: blocked SIGHUP and SIGINT signals\n");
 
    if (kill(getpid(), SIGHUP) == -1) {
        cts_failed("kill: error (%d), signal SIGHUP\n", errno);
    } else {
        cts_comment("kill: sent signal SIGHUP\n");
    }
    if (kill(getpid(), SIGINT) == -1) {
        cts_failed("kill: error (%d), signal SIGINT\n", errno);
    } else {
        cts_comment("kill: sent signal SIGINT\n");
    }
 
    if (pthread_create(
	    &tid,
	    pthread_attr_default,
	    (pthread_startroutine_t)WaitThread,
	    (pthread_addr_t)NULL) == -1) {
        cts_failed ("pthread_create: error (%d)\n", errno);
	exit(1);
    }
 
    while (doneFlag == 0)
        pthread_yield();

    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element PRRB_SIG_001.C */
/*  *1    10-SEP-1991 11:46:44 BUTENHOF "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element PRRB_SIG_001.C */

