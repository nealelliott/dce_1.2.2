/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems
 * Burlington, MA, USA
 * All Rights Reserved
 * (c) Copyright 1990, 1991, 1992
 */
/*
 * HISTORY
 * $Log: prvb_sam_001.c,v $
 * Revision 1.1.91.2  1996/02/17  23:32:00  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:48:26  marty]
 *
 * Revision 1.1.91.1  1995/12/11  23:03:57  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:40  root]
 * 
 * Revision 1.1.89.1  1994/04/01  20:12:18  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:20:06  jd]
 * 
 * Revision 1.1.87.1  1993/10/12  19:38:01  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:31:14  rhw]
 * 
 * Revision 1.1.7.2  1993/06/14  20:18:45  sommerfeld
 * 	Removed SVR4 ifdefs around cts_ calls,
 * 	added include of cts_test.h, so that
 * 	cts_ interface is used for all platforms.
 * 	[1993/06/04  17:55:36  hopkins]
 * 
 * Revision 1.1.3.3  1992/12/30  16:55:13  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:13:15  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/29  20:40:06  devsrc
 * 	[OT 5373] SNI/SVR4 Merge UPDATE: Fixed function prototype headers to eliminate
 * 	          compiler errors on AIX3.2
 * 	[1992/09/24  20:04:12  sekhar]
 * 
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:34:48  sekhar]
 * 
 * Revision 1.1  1992/01/19  03:22:14  devrcs
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
 *	PTHREAD Test Programs
 *
 *  FILENAME:
 *
 *	PRVB_SAM_001
 *
 *  TEST TOPICS:
 *
 *	SAM  -  Sample (or example) programs
 *
 *  ABSTRACT:
 *
 *	This program illustrates the use of threads and serves as
 *	a simple example as well as a simple test.
 *
 *  AUTHORS:
 *
 *	Bob Conti
 *
 *  CREATION DATE:
 *
 *	19 December 1989
 *
 *  MODIFICATION HISTORY:
 *
 *	002	R. Conti	22 December 1989
 *		Modify to display when alert takes place.
 *	003	Paul Curtin	31 May 1990
 *		Converted test to use pthread interface.
 *	004	Paul Curtin	3 July 1990
 *		Converted file to stream format
 *	005	Paul Curtin	17 July 1990
 *		Rearranged handling of join statuses.
 *	006	Paul Curtin	5 September 1990
 *		Fixed test name in comments.
 *	007	Dave Butenhof	7 September 1990
 *		Remove CMA-isms.
 *	008	Dave Butenhof	7 February 1991
 *		Remove more CMA-isms (alert references).
 *	009	Webb Scales	7 March 1991
 *		Removed unneeded include of cts_test.h
 */

/*
 *  INCLUDE FILES
 */

#include <pthread.h>
#include <stdio.h>
#include <dce/cts_test.h>

#define check(status,string) \
    if (status == -1) perror (string);

#define thread_count	11	/* Number of threads to create */
#define iterations	2000	/* Number of iterations in each thread */

/* Declare routine to executed as each thread's start routine */
static pthread_addr_t     thread_action  (pthread_addr_t);

/* 
 * Declare arrays with a lifetime at least as long as each thread in order
 * to hold the inputs and outputs of the thread.  (Normally, this storage
 * would be objects allocated from the heap.  We are using static
 * storage only because this is a simple test.)
 */
static int 		input[thread_count];
static int		output[thread_count];

/* 
 * Declare a mutex for all the threads to use
 */
static pthread_mutex_t 	the_mutex;


main ()
    {

    /*
     * Declare arrays of threads and cells to hold their termination
     * status and result values.
     */
    pthread_t		threads[thread_count];
    pthread_addr_t	result[thread_count];
    int			the_status[thread_count];
    int			status;

    int	i;

	cts_test("PRVB_SAM_001","Threads example");

    /* Create the mutex all threads will use */
    status = pthread_mutex_init (&the_mutex, pthread_mutexattr_default);
    check(status,"pthread_mutex_init");

    printf ("Every odd thread should terminate due to an cancel\n");
    printf ("  Threads divisible by 3 take the cancel at a pthread_delay_np\n");
    printf ("  Threads not divisible by 3 take the cancel at an cancel_test\n");

    /* 
     * Set the input value and create each thread.  
     * Yield so that the thread is given a chance to run after its creation.
     */
    for (i = 0; i < thread_count; i++) {

	input[i] = i;
	status = pthread_create (
			&threads[i],
			pthread_attr_default,
			thread_action,
			(pthread_addr_t)&input[i]);
	pthread_yield ();
	check(status,"pthread_create");

	}

    /* 
     * Now that all threads have been created cancel every odd thread.
     * (We're assuming that the thread doesn't complete before it gets
     * this cancel.  In other words, this test assumes scheduling fairness
     * and needs to be changed if the default scheduling policy is not
     * fair.)
     */
    for (i = 1; i < thread_count; i = i + 2) {
	pthread_cancel (threads[i]);
	}

    /* 
     * Now, join with each thread and see if it was canceled
     * or produced a result.
     */
    for (i = 0; i < thread_count; i++) {
	int *ptr;

	the_status[i] = pthread_join (
			    threads[i],
			    (pthread_addr_t *)&ptr);

	/*
	 * If it terminated normally, then copy the result produced.
	 */
	if ((the_status[i] == 0) && (ptr != (int *)-1)) 
	    result[i] = (pthread_addr_t)*ptr;
	else
	    result[i] = (pthread_addr_t)-1;

	/*
	 * Give each thread permission to release its TCB
         */
	status = pthread_detach (&threads[i]);
	check(status,"pthread_detach");
	}

    /*
     * Check for correct results for each thread.  Some terminate
     * normally and produce a result; the others terminate with an
     * cancel.  None should terminate with an error.
     */
    for (i = 0; i < thread_count; i++) {
	if ((int)result[i] != -1) {
	    printf ("Thread %d exited normally\n", i);
	    if ((int)result[i] != i + 1) 
	        cts_failed ("**** Error: Thread %d exited with bad status", i);
	}
	else {
	    if (i%2 != 0) 
		printf ("Thread %d terminated prematurely (correctly).\n", i);
	    else 
	        cts_failed ("**** Error: Thread %d terminated unexpectedly.", i);
	    }

	if (the_status[i] == -1) 
	    cts_failed("*** Error: Thread join %d got error exit.", i);
	}
    cts_result();
    }



static void
clean1	(
	pthread_addr_t	arg)
    {
    printf ("Cancel received in cancel test\n");
    }



static void
clean2	(
	pthread_addr_t	arg)
    {
    int status;

    status = pthread_mutex_unlock (&the_mutex);
    check(status,"pthread_mutex_unlock");
    }


static void
clean3	(
	pthread_addr_t	arg)
    {
    printf("**** Error: cancel received in delay");
    }


static pthread_addr_t
thread_action	(
	pthread_addr_t	input_ptr)
    {
    int i, number, *ptr;
    int status;
    struct timespec delay;
    int	outer_cancel_state;
   
    ptr = (int *) input_ptr;
    number = *ptr;

    /*
     * Defer all cancels until enabled
     */
    outer_cancel_state = pthread_setcancel (CANCEL_OFF);

    /*
     * Lock a mutex and do some i/o using printf (which must be
     * presumed to be non-reentrant, therefore all threads must lock the
     * same mutex before calling it.
     */
    for (i = 1; i <= iterations; i++) {
	status = pthread_mutex_lock (&the_mutex);
	check(status,"pthread_mutex_lock");

	/* 
	 * Allow cancels at iteration 100.  (NOTE: we depend on the
	 * fact that the architecture guarantees that this operation is not 
	 * cancelable since we are calling it with a mutex locked and have 
	 * no exception handler.)
	 */
	if (i == 100) (void)pthread_setcancel (outer_cancel_state);

	/*
	 * Allow an cancel to take us out while we have the
	 * mutex locked, if an cancel is pending.  Demonstrate that
	 * the cancel can be taken at either an explicit test or
	 * implicitly at an cancelable routine (delay).  Demonstrate
	 * the use of exception handling to catch exceptions and to
	 * restore invariants using a FINALLY. 
	 */
	pthread_cleanup_push(clean2, 0);
	pthread_cleanup_push(clean1, 0);
	if (number%3 != 0) pthread_testcancel ();
	pthread_cleanup_pop(0);

	delay.tv_sec = 0;
	delay.tv_nsec = 500000;

	pthread_cleanup_push(clean1, 0);
	if (number%3 == 0 && i%11 == 0) pthread_delay_np (&delay);
	pthread_cleanup_pop(0);

	if (i%100 == 0)
	    printf (
		    "At iteration %d in thread %d\n",
		    i,
		    number);
	pthread_cleanup_pop(1);

	/*
	 * Yield every 10 iterations with the mutex unlocked
	 */
	if (i%10 == 0) pthread_yield ();

	/* 
	 * Delay every 50th iteration in odd threads. 
	 * Prevent cancels at this delay.  (This demonstrates that
	 * cancel deferral works.)
	 */ 
	pthread_cleanup_push(clean3, 0);

	if (number%2 != 0 && i%50 == 0) {
	    int	cancel_state;

	    cancel_state = pthread_setcancel (CANCEL_OFF);
	    delay.tv_sec = 0;
	    delay.tv_nsec = 500000;
	    pthread_delay_np (&delay);
	    (void)pthread_setcancel (cancel_state);
	    }

	pthread_cleanup_pop(0);
	}

    /*
     * Store our output results, which are just the number we were
     * passed plus one.  Return the address of our results.
     */
    output[number] = number + 1;
    return (pthread_addr_t)&(output[number]);
    }
