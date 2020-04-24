/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crub_stk_001.c,v $
 * Revision 1.1.8.2  1996/02/17  23:29:48  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:46:34  marty]
 *
 * Revision 1.1.8.1  1995/12/11  22:58:04  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:34:56  root]
 * 
 * Revision 1.1.6.1  1994/04/01  20:08:48  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:12:15  jd]
 * 
 * Revision 1.1.4.1  1993/10/12  19:34:43  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:29:41  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:28:38  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:05:20  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:20:21  devrcs
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
 *	RU_STK_001.C
 *
 *  TEST TOPICS:
 *
 *	STK  -  Stacks
 *
 *  ABSTRACT:
 *
 *	This module tests the CMA/REF services to manage stacks:
 *	it will create and delete stacks, reassign stacks between
 *	threads, and call routines on alternate stacks.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	14 September 1989
 *
 *  MODIFICATION HISTORY:
 * 
 *	001	Paul Curtin	5 July 1990
 *		Replaced cma__c_page_size w/cma__g_chunk_size in selected
 *		places.
 *	002	Dave Butenhof	4 September 1990
 *		Change to new CMA interfaces.
 *	003	Paul Curtin	24 September 1990
 *		Added a cma__roundup_chunksize to full cluster stack 
 *		allocation equations.
 *	004	Dave Butenhof	18 December 1990
 *		Remove redundant #include of cma_host.h
 */

/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <cma_defs.h>
#include <dce/cts_test.h>
#include <cma_stack.h>
#include <cma_stack_int.h>

typedef struct STACKSTRUCT {
    cma_t_integer	n;
    cma_t_natural	size;
    cma_t_address	sp;
    } stackstruct;

static cma_t_natural
get_fullsize  (cma_t_attr *);

static cma_t_natural get_stacksize  (cma_t_attr *);

static void validate_stack  (stackstruct *);

main ()
    {
    cma_t_attr		att[3];
    cma_t_stack_np	stacks[8];
    stackstruct		info[8];
    int			i;
    cma_t_address	first_sp, result;
    cma_t_natural	size, cluster_size, available_c1;


    cts_test ("CRUB_STK_001", "Test stack management");

    cma_init ();			/* Initialize CMA */

    /*
     * Calculate the size of a stack cluster
     */
    cluster_size = cma__g_chunk_size * cma__c_chunk_count;

    /*
     * Create a stack with default size.
     */
    cma_stack_create_np (&stacks[0], &cma_c_null);
    info[0].n = 0;
    info[0].size = get_stacksize (&cma_c_null);

    /*
     * Now, delete and re-create the first stack, and check whether we get
     * the same SP back from the call_on_stack (ensure that stack chunks are
     * properly released---or cached---and re-used).
     *
     * NOTE: This ASSUMES that released stack chunks are re-allocated in
     * order (whether cached or not).  CMA is not necessarily broken because
     * this test fails!  I've minimized the risk by doing this FIRST (so it's
     * the only freed stack), but that's not a guarantee.
     */
    cma_stack_call_routine_np (
	    &stacks[0],
	    (cma_t_call_routine)validate_stack,
	    (cma_t_address)(&info[0]),
	    (cma_t_address *)&result
	    );
    cma_stack_delete_np (&stacks[0]);	/* Try again (idempotency test) */
    cma_stack_create_np (&stacks[0], &cma_c_null);
    first_sp = info[0].sp;		/* Save SP from first call */
    cma_stack_call_routine_np (
	    &stacks[0],
	    (cma_t_call_routine)validate_stack,
	    (cma_t_address)(&info[0]),
	    (cma_t_address *)&result
	    );

    if (info[0].sp != first_sp)
	cts_failed (
		"Reallocation got different SP (%08x instead of %08x)",
		info[0].sp,
		first_sp
		);

    /*
     * Create an attributes object, modify the stack size, and then create
     * a 10-page stack using that attr object.
     */
    cma_attr_create (&att[0], &cma_c_null);
    cma_attr_set_stacksize (&att[0], 10 * cma__c_page_size);
    info[1].n = 1;
    info[1].size = get_stacksize (&att[0]);
    cma_stack_create_np (&stacks[1], &att[0]);

    /*
     * Now, create another attributes object.  Using our "back door"
     * knowledge of the implementation's stack cluster size, allocate enough
     * stacks to be sure we force allocation of another stack cluster.
     *
     * (Note, we create 5 stacks, each ¼ as large as the remaining space in
     * the original stack cluster... we make 5 rather than 2 or 3 just to get
     * more testing of the chunk free list manipulation).
     */
    available_c1 = cluster_size
	- (get_fullsize (&cma_c_null) + get_fullsize (&att[0]));
    cma_attr_create (&att[1], &cma_c_null);

    /*
     * We want to allocate the proper amount of space: so take into
     * consideration the fact that the stack size is rounded up to the next
     * page, and the guard and yellow zone pages are added to it (we want the
     * TOTAL to be ¼ of the available space!)
     */
    size = (available_c1 / 4) - cma__c_default_guard - cma__c_yellow_size;
    size = (size / cma__c_page_size) * cma__c_page_size;	/* Round down */
    cma_attr_set_stacksize (
	    &att[1],
	    size
	    );
    size = get_stacksize (&att[1]);	/* Get REAL size of stack */

    for (i = 2; i <= 6; i++)
	{
	info[i].n = i;
	info[i].size = size;
	cma_stack_create_np (&stacks[i], &att[1]);
	}

    /*
     * Now, one final attributes object... this time we're going to try to
     * allocate a complete stack cluster.
     *
     *	10752 = 21 pages * 512 = internal cma___c_reserve_size
     */
    cma_attr_create (&att[2], &cma_c_null);
    cma_attr_set_stacksize (
	    &att[2],
	    (cluster_size - cma__c_default_guard 
	    - cma__c_yellow_size - cma__roundup_chunksize(10752))
	    );
    info[7].n = 7;
    info[7].size = get_stacksize (&att[2]);
    cma_stack_create_np (&stacks[7], &att[2]);

    /*
     * To validate the stacks, try calling a test routine on each: the test
     * routine takes a parameter which is (the address of) the desired size
     * of the stack.
     */

    for (i = 0; i <= 7; i++)
	cma_stack_call_routine_np (
		&stacks[i],
		(cma_t_call_routine)validate_stack,
		(cma_t_address)(&info[i]),
		(cma_t_address *)&result);

    /*
     * The order in which stacks are freed is carefully chosen to test all
     * cases of chunk freelist insertion... don't fiddle unless the free list
     * algorithms change!
     *
     * (Of course, there's no easy way to verify in the test case that
     * insertion actually worked correctly... at least, it's useful during
     * debugging!)
     *
     * Arrangement:
     *
     *	    Stack 0:	C1	(default size)
     *	    Stack 1:	C1	(10 page)
     *	    Stack 2:	C1	(¼ of remaining C1)
     *	    Stack 3:	C1	(¼ of remaining C1)
     *	    Stack 4:	C1	(¼ of remaining C1)
     *	    Stack 5:	C1	(¼ of remaining C1)
     *		    C1 is FULL
     *	    Stack 6:	C2	(¼ of remaining C1)
     *		    C2 has free space
     *	    Stack 7:	C3	(full cluster size)
     *		    C3 is FULL
     *
     * We want to test the following cases:
     *
     * 1. Empty free list			(S7/C3)
     * 2. Empty free list			(S0/C1)
     * 3. At beginning, merge with next		(S6/C2)
     * 4. At end, no merge			(S4/C1)
     * 5. At end, merge with previous		(S5/C1)
     * 6. In middle of list, no merge		(S2/C1)
     * 7. In middle, merge with prev & next	(S1/C1)
     */
    cma_stack_delete_np (&stacks[7]);	/* Free to null list (C3) */
    cma_stack_delete_np (&stacks[0]);	/* Free (C1) */
    cma_stack_delete_np (&stacks[6]);	/* Free to head (merge) (C2) */
    cma_stack_delete_np (&stacks[4]);	/* Free to end of list (C1) */
    cma_stack_delete_np (&stacks[5]);	/* Free to end (merge) (C1) */
    cma_stack_delete_np (&stacks[2]);	/* Free to middle (no merge) (C1) */
    cma_stack_delete_np (&stacks[3]);	/* No test: just has to be done */
    cma_stack_delete_np (&stacks[1]);	/* Free to middle (merge both) (C1) */
    cma_stack_delete_np (&stacks[0]);	/* Delete again (idempotency test) */
    cma_attr_delete (&att[0]);
    cma_attr_delete (&att[1]);
    cma_attr_delete (&att[2]);

    cts_result ();
    }

static cma_t_natural
get_fullsize	(
	cma_t_attr	*att)
    {
    cma_t_natural	stack, guard;

    cma_attr_get_stacksize (att, &stack);
    cma_attr_get_guardsize (att, &guard);
    return ((stack + guard + cma__c_yellow_size + cma__g_chunk_size - 1)
	    / cma__g_chunk_size) * cma__g_chunk_size;
    }

static cma_t_natural
get_stacksize	(
	cma_t_attr	*att)
    {
    cma_t_natural	stack;

    cma_attr_get_stacksize (att, &stack);
    return stack;
    }

static void
validate_stack	(
	stackstruct	*stack)		/* Allocated stacksize */
    {
    cma_t_integer	size = 0;
    cma_t_boolean	ok;


    stack->sp = cma__fetch_sp ();

    do {
	size += cma__c_page_size;
	ok = cma_stack_check_limit_np (size);
	} while (ok);

    /*
     * If the maximum stack allocation we could get was smaller than the
     * amount we asked for... then we didn't get enough.  Complain.
     *
     * Actually, this has one problem: it doesn't take into account the size
     * of the stack frame for this routine.  Well, you can't have everything:
     * where would you put it?  Anyway, the frame is PROBABLY small enough
     * that it doesn't matter (true on VAX).
     */
    if (size < stack->size)
	cts_failed (
		"Stack %d is too small: should be %d bytes, is %d",
		stack->n,
		stack->size,
		size
		);

    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRUB_STK_001.C */
/*  *8    18-DEC-1990 23:41:23 BUTENHOF "Remove unnecessary #includes" */
/*  *7    24-SEP-1990 14:09:04 CURTIN "added a roundup to an equation" */
/*  *6     5-SEP-1990 09:39:18 BUTENHOF "Convert to new CMA interfaces" */
/*  *5     5-JUL-1990 13:04:19 CURTIN "replaced selected occurrencs of cma__c_page_size" */
/*  *4     3-JUL-1990 12:02:54 BUTENHOF "Change copyright" */
/*  *3    21-JUN-1990 08:26:08 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *2     7-JUN-1990 11:35:12 SCALES "Updates to CTS" */
/*  *1    17-MAY-1990 13:57:30 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRUB_STK_001.C */
