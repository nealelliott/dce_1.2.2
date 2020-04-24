/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: prvb_ptc_002.c,v $
 * Revision 1.1.40.2  1996/02/17  23:31:59  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:48:25  marty]
 *
 * Revision 1.1.40.1  1995/12/11  23:03:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:39  root]
 * 
 * Revision 1.1.38.1  1994/04/01  20:12:15  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:20:01  jd]
 * 
 * Revision 1.1.36.1  1993/10/12  19:37:59  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:31:13  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:55:11  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:13:12  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:19:43  devrcs
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
 *	CMA Test Programs
 *
 *  FILENAME:
 *
 *	PRVB_PTC_002
 *
 *  TEST TOPICS:
 *
 *	PTC  -  Per Thread Context
 *
 *  ABSTRACT:
 *
 *	Create a batch of threads which each associate a large number of
 *	contexts with themselves; verify that it's maintained correctly over
 *	a loop which yields the processor.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	2 October 1990
 *
 *  MODIFICATION HISTORY:
 *	001	    Paul Curtin	    27 November 1990
 *		    Switched order of pthread_keycreate arguments.
 */

/*
 *  INCLUDE FILES
 */

#include <pthread.h>
#include <dce/cts_test.h>

#define key_count	200
#define thread_count	10
#define iterations	200

typedef struct CONTEXT_STRUCT {
    int			thread;
    pthread_key_t	key;
    } context_struct;

pthread_key_t	key[key_count];
pthread_once_t	once = pthread_once_init;

static pthread_addr_t thread  (int);

static void init (void);

static void wipe_it_out  (context_struct *);

main ()
    {
    pthread_t		threads[thread_count];
    int			i, status;
    pthread_addr_t	result;


    cts_test ("PRVB_PTC_002", "Test lots of per-thread context");

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
	check (status, "pthread_create");
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
	check (status, "pthread_join");
	status = pthread_detach (&threads[i]);
	check (status, "pthread_detach");
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
    check (status, "pthread_create");
    status = pthread_join (
	    threads[1],
	    &result);
    check (status, "pthread_join");
    status = pthread_detach (&threads[1]);
    check (status, "pthread_detach");
    cts_result ();
    }

static pthread_addr_t
thread	(
	int	number)
    {
    int			i, j, status;
    context_struct	*context_value;


    status = pthread_once (&once, init);	/* Create context keys */
    check (status, "pthread_once");
    pthread_yield ();

    /*
     * Verify that all the contexts are initially null, and complain if any
     * aren't.
     */
    for (i = 0; i < key_count; i++) {
	pthread_addr_t	value;


	status = pthread_getspecific (key[i], &value);
	check (status, "pthread_getspecific");

	if (value != (pthread_addr_t)0)
	    cts_failed (
		    "Thread %d: context %d is not initially null.",
		    number,
		    i);
		    
	}

    /*
     * Create a bunch of contexts for the thread.  Each consists of a
     * dynamically allocated structure which contains the thread's ID and the
     * number of the key.
     */
    for (i = 0; i < key_count; i++) {
	context_value = (context_struct *)malloc (sizeof (context_struct));
	context_value->thread = number;
	context_value->key = key[i];
	status = pthread_setspecific (key[i], (pthread_addr_t)context_value);
	check (status, "pthread_setspecific");
	pthread_yield ();
	}

    context_value = (context_struct *)0;	/* Init to null */

    /*
     * Verify that we get the correct values back.  Loop over all the
     * contexts several times, just to be sure that other threads don't
     * interfere (the use of a yield call in each iteration should guarantee
     * that they get their chance to try).
     */
    for (j = 0; j < iterations; j++) {

	for (i = 0; i < key_count; i++) {
	    pthread_yield ();
	    status = pthread_getspecific (
		    key[i],
		    (pthread_addr_t *)&context_value);
	    check (status, "pthread_getspecific");

	    if (
		    (context_value->thread != number)
		    || (context_value->key != key[i]))
		cts_failed (
			"Thread %d itr %d: context %d is incorrect.",
			number,
			j,
			i);

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
	check (status, "pthread_setspecific");
	status = pthread_getspecific (key[i], &value);
	check (status, "pthread_getspecific");

	if (value != (pthread_addr_t)0)
	    cts_failed (
		    "Thread %d: context %d was not correctly deleted.",
		    number,
		    i);

	}

    }

static void
init	(void)
    {
    int	i, status;


    /*
     * Define a bunch of context keys (this is executed via pthread_once).
     */
    for (i = 0; i < key_count; i++) {
	status = pthread_keycreate (&key[i], wipe_it_out);
	check (status, "pthread_keycreate");
	}

    }

static void
wipe_it_out	(
	context_struct	*context)
    {
    free ((pthread_addr_t)context);
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element PRVB_PTC_002.C */
/*  *2    27-NOV-1990 14:58:51 CURTIN "Switch order of pthread_keycreate arguments" */
/*  *1     3-OCT-1990 13:49:31 BUTENHOF "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element PRVB_PTC_002.C */
