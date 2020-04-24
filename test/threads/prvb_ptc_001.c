/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: prvb_ptc_001.c,v $
 * Revision 1.1.39.2  1996/02/17  23:31:58  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:48:24  marty]
 *
 * Revision 1.1.39.1  1995/12/11  23:03:52  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:38  root]
 * 
 * Revision 1.1.37.1  1994/04/01  20:12:12  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:19:56  jd]
 * 
 * Revision 1.1.35.1  1993/10/12  19:37:57  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:31:12  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:55:06  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:13:08  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:22:13  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  Copyright (c) 1989, 1990 by
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
 *	PRVB_PTC_001
 *
 *  TEST TOPICS:
 *
 *	PTC  -  Per Thread Context
 *
 *  ABSTRACT:
 *
 *	Create a batch of threads which each associate some context
 *	with themselves... and then alternately yield and validate
 *	the context some pre-defined number of times.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	17 October 1989
 *
 *  MODIFICATION HISTORY:
 * 
 *	001	Paul Curtin	31 May 1990
 *		Converted test to use pthread interface.
 *	002	Dave Butenhof	14 June 1990
 *		Convert sprintfs to use of variable arg. cts calls.
 *	003	Webb Scales	17 September 1990
 *		Move C RTL functions to CMA Library layer.
 *	004	Paul Curtin	27 November 1990
 *		Switched order of pthread_keycreate arguments.
 */


/*
 *  INCLUDE FILES
 */

#include <pthread.h>
#include <dce/cts_test.h>
#include <cmalib_crtlx.h>

#define key_count	10
#define thread_count	10
#define iterations	20

typedef struct CONTEXT_STRUCT {
    int	odd;
    int	thread;
    int	key;
    } context_struct;

int	key[key_count];
pthread_once_t	once = pthread_once_init;

static pthread_addr_t thread  (int);

static void init  (void);

static void wipe_it_out_odd  (context_struct *);

static void wipe_it_out_even  (context_struct *);

static unsigned int odd  (int);

main ()
    {
    pthread_t	threads[thread_count];
    int	i;
    int status;
    pthread_addr_t	result;
    int	exit;


    cts_test ("PRVB_PTC_001", "Test use of per-thread context");

    /*
     * Fork a bunch of threads to perform "simultaneous" testing of
     * per-thread context.  The number of threads can be set by changing the
     * constant "thread_count".
     */
    for (i = 1; i < thread_count; i++) {
	status = pthread_create (
		    &threads[i],
		    pthread_attr_default,
		    thread,
		    (pthread_addr_t)i);
	check(status,"pthread_create");
	pthread_yield ();
	}

    thread (0);				/* Do stuff for main thread */

    /*
     * Now join with the threads to be sure they've all cleaned up and gone
     * away.  The return value and status aren't used.
     */
    for (i = 1; i < thread_count; i++) {
	status = pthread_join (
			threads[i],
			&result);
	check(status,"pthread_join");
	status = pthread_detach (&threads[i]);
	check(status,"pthread_detach");
	}

    /*
     * Now, run another thread, and do the same thing.  This should reclaim
     * an existing TCB from the cache... and verify that per-thread context
     * was handled correctly in the cached TCB.
     */
    status = pthread_create (
		    &threads[1],
		    pthread_attr_default,
		    thread,
		    (pthread_addr_t)1);
    check(status,"pthread_create");
    status = pthread_join (
		    threads[1],
		    &result);
    check(status,"pthread_join");
    status = pthread_detach (&threads[1]);
    check(status,"pthread_detach");
    cts_result ();

    }

static pthread_addr_t
thread	(
	int	number)
    {
    pthread_t	self;
    int	status;
    int	i, j;
    context_struct	*context_value;


    status = pthread_once (&once, init);	/* Create context keys */
    check(status,"pthread_once");
    self = pthread_self ();
    pthread_yield ();

    /*
     * Verify that all the contexts are initially null, and complain if any
     * aren't.
     */
    for (i = 0; i < key_count; i++) {
	pthread_addr_t	value;


	status = pthread_getspecific (key[i], &value);
	check(status,"pthread_getspecific");

	if (value != (pthread_addr_t)0)
	    cts_failed (
		    "Thread %d: context %d is not initially null.",
		    number, i);

	}

    /*
     * Create a bunch of contexts for the thread.  Each consists of a
     * dynamically allocated structure which contains the thread's ID and the
     * number of the key.
     */
    for (i = 0; i < key_count; i++) {
	context_value = (context_struct *)malloc (sizeof (context_struct));
	context_value->odd = odd (i);
	context_value->thread = number;
	context_value->key = key[i];
	status = pthread_setspecific (key[i], (pthread_addr_t)context_value);
	check(status,"pthread_setspecific");
	pthread_yield ();
	}

    context_value = (context_struct *)(pthread_addr_t)0;	/* Init to null */

    /*
     * Verify that we get the correct values back.  Loop over all the
     * contexts several times, just to be sure that other threads don't
     * interfere (the use of a yield call in each iteration should guarantee
     * that they get their chance to try).
     */
    for (j = 0; j < iterations; j++) {

	for (i = 0; i < key_count; i++) {
	    pthread_yield ();
	    status = pthread_getspecific (key[i], 
					  (pthread_addr_t *)&context_value);
	    check(status,"pthread_getspecific");

	    if (
		    (context_value->thread != number)
		    || (context_value->key != key[i]))
		cts_failed (
			"Thread %d itr %d: context %d is incorrect.",
			number, j, i);

	    }

	}

    /*
     * Now delete half of the contexts for the thread... and verify that
     * they're really gone.  The other half will be destroyed by thread
     * rundown.
     */
    for (i = 0; i < key_count / 2; i++) {
	pthread_addr_t	value;

	status = pthread_setspecific (key[i], (pthread_addr_t)0);
	check(status,"pthread_setspecific");
	status = pthread_getspecific (key[i], &value);
	check(status,"pthread_getspecific");

	if (value != (pthread_addr_t)0)
	    cts_failed (
		    "Thread %d: context %d was not correctly deleted.",
		    number, i);

	}

    }

static void
init	(void)
    {
    int	i;
    int status;

    /*
     * Define a bunch of context keys (this is executed via cma_once).
     */
    for (i = 0; i < key_count; i++)
	if (odd (i)) {
	    status = pthread_keycreate (&key[i], wipe_it_out_odd);
	    check(status,"pthread_keycreate");
	    }
	else {
	    status = pthread_keycreate (&key[i], wipe_it_out_even);
	    check(status,"pthread_keycreate");
	    }

    }

/*
 * This routine is the context destructor for all odd numbered keys (1, 3,
 * 5, 7, etc.).  If the "odd" field of the context isn't cma_c_false, generate
 * an error.
 */
static void
wipe_it_out_odd	(
	context_struct	*context)
    {

    if (!context->odd)
	cts_failed (
		"Odd destructor called for even context (thread %d, ctx %d)",
		context->thread, context->key);

    free ((pthread_addr_t)context);
    }
/*
 * This routine is the context destructor for all even numbered keys (0, 2, 4,
 * 6, 8, etc.).  If the "odd" field of the context isn't cma_c_true, generate
 * an error.
 */
static void
wipe_it_out_even	(
	context_struct	*context)
    {

    if (context->odd)
	cts_failed (
		"Even destructor called for odd context (thread %d, ctx %d)",
		context->thread, context->key);

    free ((pthread_addr_t)context);
    }

/*
 * Routine to compute whether a number is odd...
 */
static unsigned int
odd	(
	int	number)
    {
    if (((number / 2) * 2) == number)
	return (int) 0;
    else
	return (int) 1;
    }
