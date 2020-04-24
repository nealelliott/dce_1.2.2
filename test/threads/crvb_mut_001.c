/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crvb_mut_001.c,v $
 * Revision 1.1.38.2  1996/02/17  23:30:15  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:46:59  marty]
 *
 * Revision 1.1.38.1  1995/12/11  22:59:13  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:18  root]
 * 
 * Revision 1.1.36.1  1994/04/01  20:09:34  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:14:01  jd]
 * 
 * Revision 1.1.34.1  1993/10/12  19:35:18  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:29:58  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:30:43  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:07:22  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:20:58  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  Copyright (c) 1989, 1991 by
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
 *	RV_MUT_001
 *
 *  TEST TOPICS:
 *
 *	MUT  -  Mutex Operations
 *
 *  ABSTRACT:
 *
 *	This test creates a number of threads and then
 *	has each one lock and unlock the same mutex
 *	repeatedly.  The intent is to detect any flaws
 *	that is, races, in mutexes, time-slicing, or the
 *	scheduler.
 *
 *  AUTHORS:
 *
 *	Bob Conti
 *
 *  CREATION DATE:
 *
 *	06 November 1989
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	5 September 1990
 *		Convert to new CMA interfaces.
 *	002	Dave Butenhof	08 May 1991
 *		Cast thread function to proper type to satisfy pickier MIPS C
 *		compiler.
 */

/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <dce/cts_test.h>
#include <signal.h>
#include <stdio.h>

#define maxthread_count	100000
static int thread_count, iterations;

cma_t_mutex		the_mutex;

static cma_t_address thread (cma_t_integer);

int
main	(int argc, char *argv[], char *envp[])
    {
    cma_t_thread	threads[maxthread_count];
    cma_t_integer	i;
    cma_t_address	result;
    cma_t_exit_status	exit;


    if (argc < 2)
	thread_count = 10;
    else {
	thread_count = atoi (argv[1]);

	if (thread_count > maxthread_count) {
	    thread_count = maxthread_count;
	    fprintf (stderr, "too many threads; using %d\n", maxthread_count);
	    }

	}

    if (argc < 3)
	iterations = 20000;
    else
	iterations = atoi (argv[2]);

    cts_test ("CRVB_MUT_001", "This test bangs on mutexes from several threads");
    cma_init ();
#if 0
    signal (SIGUSR1, (void (*)())cma_debug);	/* Cute debug trick! */
#endif

    cma_mutex_create (&the_mutex, &cma_c_null);

    /*
     * Fork a bunch of threads to perform "simultaneous" testing of
     * mutexes.  The number of threads can be set by changing the
     * constant "thread_count".
     */
    for (i = 1; i < thread_count; i++) {
	cma_thread_create (
		&threads[i],
		&cma_c_null,
		(cma_t_start_routine)thread,
		(cma_t_address)i);
	cma_yield ();
	}

    thread (0);				/* Do stuff for main thread */

    /*
     * Now join with the threads to be sure they've all cleaned up and gone
     * away.  The return value and status aren't used.
     */
    for (i = 1; i < thread_count; i++) {
	cma_thread_join (
		&threads[i],
		&exit,
		&result);
	cma_thread_detach (&threads[i]);
	}

    cts_result ();

    }

static cma_t_address
thread	(
	cma_t_integer	number)
    {
    cma_t_thread	self;
    cma_t_integer	i;

    fprintf (stderr, "Thread %d starting\n", number);

    for (i = 1; i <= iterations; i++) {
	cma_mutex_lock (&the_mutex);

	/* 
	 * Every time each thread does 5000 interations, print
	 * a progress report.
	 */
	if (i%5000 == 0)
	    fprintf (stderr, "At iteration %d in thread %d\n", i, number);

	/* 
	 * Every 11 times, yield with the mutex locked.
	 */
	if ((i+number)%11 == 0)
	    cma_yield ();

	cma_mutex_unlock (&the_mutex);

	/* 
	 * Every 23 times, yield with the mutex unlocked.
	 */
	if ((i+number)%23 == 0)
	    cma_yield ();

        }

    fprintf (stderr, "Thread %d exiting\n", number);
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_MUT_001.C */
/*  *8    10-JUN-1991 14:28:09 BUTENHOF "Remove SIGUSR1 debug hook (doesn't build on VMS)" */
/*  *7     4-JUN-1991 12:57:08 BUTENHOF "Add arguments" */
/*  *6    15-MAY-1991 16:16:50 BUTENHOF "Cast thread function" */
/*  *5     5-SEP-1990 09:40:52 BUTENHOF "Convert to new CMA interfaces" */
/*  *4     3-JUL-1990 12:04:13 BUTENHOF "Change copyright" */
/*  *3    21-JUN-1990 08:30:00 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *2     7-JUN-1990 11:37:08 SCALES "Updates to CTS" */
/*  *1    17-MAY-1990 13:58:14 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_MUT_001.C */
