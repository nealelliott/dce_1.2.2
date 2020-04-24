/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crvb_stk_001.c,v $
 * Revision 1.1.40.2  1996/02/17  23:30:27  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:09  marty]
 *
 * Revision 1.1.40.1  1995/12/11  22:59:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:28  root]
 * 
 * Revision 1.1.38.1  1994/04/01  20:09:55  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:14:48  jd]
 * 
 * Revision 1.1.36.1  1993/10/12  19:35:38  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:08  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:31:33  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:08:24  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:21:08  devrcs
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
 *	CRVB_STK_001.C
 *
 *  TEST TOPICS:
 *
 *	STK  -  Stacks
 *	THD  -  Thread Operations
 *
 *  ABSTRACT:
 *
 *	Test assigning stacks between threads, calling on stacks
 *	shared between threads, rundown of stacks during thread
 *	termination.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	29 September 1989
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	1 December 1989
 *		Change routine names to track changes in POSIX style.
 *	002	Paul Curtin	4 September 1990
 *		Adjust test name in cts_test statement.
 *		Convert to new CMA interface.
 *	003	Dave Butenhof	23 May 1991
 *		Fix compilation errors under new MIPS C compiler.
 */

/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <cma_defs.h>
#include <dce/cts_test.h>

#define thread_count	10
#define test_loops	5

cma_t_mutex	stack_mutex, print_mutex;
cma_t_cond	stack_synch, done_stack;
cma_t_integer	next_owner;
cma_t_stack_np	shared_stack;
cma_t_thread	threads[thread_count];

static cma_t_address thread_main (cma_t_integer);

static cma_t_address test_stack (cma_t_thread *);

static void say (cma_t_integer,cma_t_integer);

main ()
    {
    cma_t_integer	i, j;
    cma_t_exit_status	exit;
    cma_t_address	result;


    cts_test ("CRVB_STK_001", "Test multithreaded operations on stacks");

    cma_init ();

    cma_mutex_create (&stack_mutex, &cma_c_null);
    cma_mutex_create (&print_mutex, &cma_c_null);
    cma_mutex_lock (&stack_mutex);
    cma_cond_create (&stack_synch, &cma_c_null);
    cma_cond_create (&done_stack, &cma_c_null);
    cma_stack_create_np (&shared_stack, &cma_c_null);
    next_owner = -1;			/* Stack currently unused */
    cma_mutex_unlock (&stack_mutex);

    for (i = 0; i < thread_count; i++) {
	cma_thread_create (
		&threads[i],
		&cma_c_null,
		(cma_t_start_routine)thread_main,
		(cma_t_address)i);
	cma_yield ();			/* Let someone run a little */
	}

    /*
     * Tell each thread in sequence to run a test on the shared stack.  This
     * is done by a "next_owner" variable protected by stack_mutex and the
     * stack_synch cond variable.  When a thread returns from its test
     * call, it will set the new_owner to -1, and signal the done_stack
     * cond variable to wake the main thread.
     *
     * Repeat the entire loop 5 times.
     */
    for (j = 1; j < test_loops; j++) {

	for (i = 0; i < thread_count; i++) {
	    cma_mutex_lock (&stack_mutex);
	    next_owner = i;
	    cma_mutex_unlock (&stack_mutex);
	    cma_cond_broadcast (&stack_synch);

	    cma_mutex_lock (&stack_mutex);

	    while (next_owner != -1)
		cma_cond_wait (&done_stack, &stack_mutex);

	    cma_mutex_unlock (&stack_mutex);
	    }

	}

    for (i = 0; i < thread_count; i++) {
	cma_thread_join (
		&threads[i],
		&exit,
		&result);
	cma_thread_detach (&threads[i]);
	}

    cts_result ();
    }

static cma_t_address
thread_main	(
	cma_t_integer	number)
    {
    cma_t_thread	thread;
    cma_t_integer	i;
    cma_t_address	result;


    for (i = 1; i < test_loops; i++)
	{
#if 0
	say (number, i);
#endif
	cma_mutex_lock (&stack_mutex);
	cma_yield ();

	while (next_owner != number)
	    cma_cond_wait (&stack_synch, &stack_mutex);

	cma_mutex_unlock (&stack_mutex);

	cma_yield ();
	cma_stack_call_routine_np (
		&shared_stack,
		(cma_t_call_routine)test_stack,
		(cma_t_address)&thread,
		(cma_t_address *)&result);

	if (! cma_handle_equal (&thread, &threads[number]))
	    {
	    char	buffer[200];
	    sprintf (
		    buffer,
		    "Thread %d itr %d: shared stack ID (%08x,%08x) != real ID (%08x,%0x8)",
		    number, i,
		    thread.field1, thread.field2,
		    threads[number].field1, threads[number].field2
		    );
	    cts_failed (buffer);
	    }

	cma_yield ();
	cma_thread_get_self (&thread);

	if (! cma_handle_equal (&thread, &threads[number]))
	    {
	    char	buffer[200];
	    sprintf (
		    buffer,
		    "Thread %d itr %d: root stack ID (%08x,%08x) != real ID (%08x,%0x8)",
		    number, i,
		    thread.field1, thread.field2,
		    threads[number].field1, threads[number].field2
		    );
	    cts_failed (buffer);
	    }

	cma_yield ();
	cma_mutex_lock (&stack_mutex);
	next_owner = -1;
	cma_yield ();
	cma_mutex_unlock (&stack_mutex);
	cma_cond_signal (&done_stack);
	}

    }

static cma_t_address
test_stack	(
	cma_t_thread	*my_thread)
    {
    cma_thread_get_self (my_thread);
    }

static void
say	(
	cma_t_integer	thread,
	cma_t_integer	iteration)
    {
    cma_mutex_lock (&print_mutex);
    printf ("RV_STK_001_THD status: thread %d, iteration %d\n", thread, iteration);
    cma_mutex_unlock (&print_mutex);
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_STK_001.C */
/*  *6    29-MAY-1991 18:02:31 BUTENHOF "Fix compilation errors" */
/*  *5     5-SEP-1990 11:18:34 CURTIN "Converted to use new CMA interfaces" */
/*  *4     3-JUL-1990 12:04:53 BUTENHOF "Change copyright" */
/*  *3    21-JUN-1990 08:31:20 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *2     7-JUN-1990 11:38:14 SCALES "Updates to CTS" */
/*  *1    17-MAY-1990 13:58:39 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_STK_001.C */
