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
 * $Log: crvb_sam_001.c,v $
 * Revision 1.1.15.2  1996/02/17  23:30:21  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:04  marty]
 *
 * Revision 1.1.15.1  1995/12/11  22:59:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:23  root]
 * 
 * Revision 1.1.13.1  1994/04/01  20:09:47  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:14:29  jd]
 * 
 * Revision 1.1.11.1  1993/10/12  19:35:28  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:03  rhw]
 * 
 * Revision 1.1.9.2  1993/06/14  20:15:12  sommerfeld
 * 	Removed ifdefs so that code fixed under
 * 	SVR4 ifdef is in force for all platforms.
 * 	[1993/05/06  20:07:03  hopkins]
 * 
 * Revision 1.1.5.4  1993/02/04  00:13:33  jd
 * 	Moved the include of cma.h to before check for
 * 	CMA_UNIX_TYPE == CMA__SVR4.
 * 	[1993/02/04  00:06:47  jd]
 * 
 * Revision 1.1.5.3  1992/12/30  16:31:14  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:07:55  zeliff]
 * 
 * Revision 1.1.5.2  1992/09/29  20:38:01  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:32:16  sekhar]
 * 
 * Revision 1.1.2.2  1992/05/11  17:07:07  bolinger
 * 	Fix OT defect 2161:  provide/correct missing/bad cts_test() banners.
 * 	[1992/05/11  13:45:01  bolinger]
 * 
 * Revision 1.1  1992/01/19  03:21:05  devrcs
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
 *	CRVB_SAM_001
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
 *	001 	R. Conti	22 December 1989
 *		Modify to display when alert takes place.
 *	002	Webb Scales	4 September 1990
 *		Changed CMA calls to pass structures by reference.
 *	003	Dave Butenhof	7 February 1991
 *		New alert control interfaces.
 */


/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <dce/cts_test.h>
#include <stdio.h>

#define thread_count	11	/* Number of threads to create */
#define iterations	2000	/* Number of iterations in each thread */

/* Declare routine to executed as each thread's start routine */
static cma_t_address     thread_action (cma_t_address);

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
static cma_t_mutex 	the_mutex;


main ()
    {

    /*
     * Declare arrays of threads and cells to hold their termination
     * status and result values.
     */
    cma_t_thread	threads[thread_count];
    cma_t_address	result[thread_count];
    cma_t_exit_status	status[thread_count];

    int	i;

	cts_test("CRVB_SAM_001", "CMA sample program");

    /* First, initialize CMA */
    cma_init ();

    /* Create the mutex all threads will use */
    cma_mutex_create (&the_mutex, &cma_c_null);

    printf ("Every odd thread should terminate due to an alert\n");
    printf ("  Threads divisible by 3 take the alert at a cma_delay\n");
    printf ("  Threads not divisible by 3 take the alert at an alert_test\n");

    /* 
     * Set the input value and create each thread.  
     * Yield so that the thread is given a chance to run after its creation.
     */
    for (i = 0; i < thread_count; i++) {

	input[i] = i;
	cma_thread_create (
		&threads[i],
		&cma_c_null,
		thread_action,
		(cma_t_address)&input[i]);
	cma_yield ();

	}

    /* 
     * Now that all threads have been created alert every odd thread.
     * (We're assuming that the thread doesn't complete before it gets
     * this alert.  In other words, this test assumes scheduling fairness
     * and needs to be changed if the default scheduling policy is not
     * fair.)
     */
    for (i = 1; i < thread_count; i = i + 2) cma_thread_alert (&threads[i]);

    /* 
     * Now, join with each thread and see if it was alerted
     * or produced a result.
     */
    for (i = 0; i < thread_count; i++) {
	int *ptr;

	cma_thread_join (
		&threads[i],
		&status[i],
		(cma_t_address *)&ptr);

	/*
	 * If it terminated normally, then copy the result produced.
	 */
	if (status[i] == cma_c_term_normal) 
	    result[i] = (cma_t_address)*ptr;

	/*
	 * Give each thread permission to release its TCB
         */
	cma_thread_detach (&threads[i]);
	}

    /*
     * Check for correct results for each thread.  Some terminate
     * normally and produce a result; the others terminate with an
     * alert.  None should terminate with an error.
     */
    for (i = 0; i < thread_count; i++) {
	if (status[i] == cma_c_term_normal) {
	    printf ("Thread %d exited normally\n", i);
	    if ((int)result[i] != i + 1) 
		cts_failed("Thread did not get correct exit status");
	}
	if (status[i] == cma_c_term_error) 
            cts_failed("Thread got error exit");
	if (status[i] == cma_c_term_alert) 
	    printf ("Thread %d got alerted\n", i);
	}
    cts_result();
    }


static cma_t_address
thread_action	(
	cma_t_address	input_ptr)
    {
    int i, number, *ptr;
    cma_t_alert_state	outer_alert_state;


    ptr = (int *) input_ptr;
    number = *ptr;

    /*
     * Disable general delivery
     */
    cma_alert_disable_general (&outer_alert_state);

    /*
     * Lock a mutex and do some i/o using printf (which must be
     * presumed to be non-reentrant, therefore all threads must lock the
     * same mutex before calling it.
     */
    for (i = 1; i <= iterations; i++) {
	cma_mutex_lock (&the_mutex);

	/* 
	 * Allow alerts at iteration 100.  (NOTE: we depend on the
	 * fact that the architecture guarantees that this operation is not 
	 * alertable since we are calling it with a mutex locked and have 
	 * no exception handler.)
	 */
	if (i == 100) cma_alert_restore (&outer_alert_state);

	/*
	 * Allow an alert to take us out while we have the
	 * mutex locked, if an alert is pending.  Demonstrate that
	 * the alert can be taken at either an explicit test or
	 * implicitly at an alertable routine (delay).  Demonstrate
	 * the use of exception handling to catch exceptions and to
	 * restore invariants using a FINALLY. 
	 */
	TRY 
	    TRY
		/* Threads not divisible by 3 take an alert here */
		if (number%3 != 0) cma_alert_test ();
	    CATCH (cma_e_alerted)
	        printf ("Alert received at alert_test, iter. %d, thread %d\n",
		    i, number);
		RERAISE;
	    ENDTRY

	    TRY
		/* Threads divisible by 3 take an alert at delay */
		if (number%3 == 0 && i%11 == 0) cma_delay (0.01);
	    CATCH (cma_e_alerted)
	        printf ("Alert received at delay, iter. %d, thread %d\n",
		    i, number);
		RERAISE;
	    ENDTRY

	    if (i%100 == 0) printf ("At iteration %d in thread %d\n",
		    i, number);

	FINALLY
	    /* 
	     * Restore invariants: unlock the mutex
	     * No outputs if we get blown away 
	     */
	    cma_mutex_unlock (&the_mutex);
	ENDTRY

	/* Yield every 10 iterations with the mutex unlocked */
	if (i%10 == 0) cma_yield ();

	/* 
	 * Delay every 50th iteration in odd threads. 
	 * Prevent alerts at this delay.  (This demonstrates that
	 * disabling general alert delivery works.)
	 */ 
	if (number%2 != 0 && i%50 == 0) {
	    TRY
		cma_t_alert_state	alert_state;


		cma_alert_disable_general (&alert_state);
	 	cma_delay (0.01);
		cma_alert_restore (&alert_state);
	    CATCH (cma_e_alerted)
		printf (
		  "**** Error: alert received at delay, iter. %d, thread %d\n",
		  i, number);
	    ENDTRY
	    }

        }

    /*
     * Store our output results, which are just the number we were
     * passed plus one.  Return the address of our results.
     */
    output[number]	= number + 1;
    return (cma_t_address)&(output[number]);
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_SAM_001.C */
/*  *6    12-FEB-1991 01:31:16 BUTENHOF "Changes to alert control" */
/*  *5     4-SEP-1990 17:15:13 SCALES "Convert to new CMA interfaces" */
/*  *4     3-JUL-1990 12:04:34 BUTENHOF "Change copyright" */
/*  *3    21-JUN-1990 08:30:33 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *2     7-JUN-1990 11:37:30 SCALES "Updates to CTS" */
/*  *1    17-MAY-1990 13:58:20 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_SAM_001.C */
