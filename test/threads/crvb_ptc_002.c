/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crvb_ptc_002.c,v $
 * Revision 1.1.43.2  1996/02/17  23:30:20  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:03  marty]
 *
 * Revision 1.1.43.1  1995/12/11  22:59:28  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:22  root]
 * 
 * Revision 1.1.41.1  1994/04/01  20:09:45  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:14:24  jd]
 * 
 * Revision 1.1.39.1  1993/10/12  19:35:26  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:02  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:31:11  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:07:49  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:21:04  devrcs
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
 *	CRVB_PTC_002
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
 *
 *	001	Dave Butenhof	5 September 1990
 *		Convert to new CMA interfaces.
 *	002	Dave Butenhof	29 October 1990
 *		Convert to new 3-argument cma_once.
 *	003	Dave Butenhof	23 May 1991
 *		Fix compilation errors under new MIPS C compiler.
 */

/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <dce/cts_test.h>

#define key_count	200
#define thread_count	10
#define iterations	200

typedef struct CONTEXT_STRUCT {
    cma_t_integer	thread;
    cma_t_key		key;
    } context_struct;

cma_t_key	key[key_count];
cma_t_once	once = cma_once_init;

static cma_t_address thread (cma_t_integer);

static void
init (cma_t_address);

static void wipe_it_out (context_struct *);

main ()
    {
    cma_t_thread	threads[thread_count];
    cma_t_integer	i;
    cma_t_address	result;
    cma_t_exit_status	exit;


    cts_test ("CRVB_PTC_002", "Test lots of per-thread context");

    cma_init ();

    /*
     * Fork a bunch of threads to perform "simultaneous" testing of
     * per-thread context.  The number of threads can be set by changing the
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

    /*
     * Now, run another thread, and do the same thing.  This should reclaim
     * an existing TCB from the cache... and verify that per-thread context
     * was handled correctly in the cached TCB.
     */
    cma_thread_create (
	    &threads[1],
	    &cma_c_null,
	    (cma_t_start_routine)thread,
	    (cma_t_address)1);
    cma_thread_join (
	    &threads[1],
	    &exit,
	    &result);
    cma_thread_detach (&threads[1]);
    cts_result ();
    }

static cma_t_address
thread	(
	cma_t_integer	number)
    {
    cma_t_integer	i, j;
    context_struct	*context_value;


    cma_once (&once, init, cma_c_null_ptr);	/* Create context keys */
    cma_yield ();

    /*
     * Verify that all the contexts are initially null, and complain if any
     * aren't.
     */
    for (i = 0; i < key_count; i++) {
	cma_t_address	value;


	cma_key_get_context (key[i], &value);

	if (value != cma_c_null_ptr)
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
	cma_key_set_context (key[i], (cma_t_address)context_value);
	cma_yield ();
	}

    context_value = (context_struct *)cma_c_null_ptr;	/* Init to null */

    /*
     * Verify that we get the correct values back.  Loop over all the
     * contexts several times, just to be sure that other threads don't
     * interfere (the use of a yield call in each iteration should guarantee
     * that they get their chance to try).
     */
    for (j = 0; j < iterations; j++) {

	for (i = 0; i < key_count; i++) {
	    cma_yield ();
	    cma_key_get_context (key[i], (cma_t_address *)&context_value);

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
	cma_t_address	value;

	cma_key_set_context (key[i], cma_c_null_ptr);
	cma_key_get_context (key[i], &value);

	if (value != cma_c_null_ptr)
	    cts_failed (
		    "Thread %d: context %d was not correctly deleted.",
		    number,
		    i);

	}

    }

static void
init	(cma_t_address	arg)		/* arg is ignored */
    {
    cma_t_integer	i;


    /*
     * Define a bunch of context keys (this is executed via cma_once).
     */
    for (i = 0; i < key_count; i++)
	cma_key_create (&key[i], &cma_c_null, (cma_t_destructor)wipe_it_out);

    }

static void
wipe_it_out	(
	context_struct	*context)
    {
    free ((cma_t_address)context);
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_PTC_002.C */
/*  *3    29-MAY-1991 18:02:13 BUTENHOF "Fix compilation errors" */
/*  *2    29-OCT-1990 22:40:19 BUTENHOF "Fix cma_once calls" */
/*  *1     3-OCT-1990 10:13:36 BUTENHOF "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_PTC_002.C */
