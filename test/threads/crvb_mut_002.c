/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crvb_mut_002.c,v $
 * Revision 1.1.39.2  1996/02/17  23:30:16  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:00  marty]
 *
 * Revision 1.1.39.1  1995/12/11  22:59:15  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:19  root]
 * 
 * Revision 1.1.37.1  1994/04/01  20:09:36  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:14:06  jd]
 * 
 * Revision 1.1.35.1  1993/10/12  19:35:19  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:29:59  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:30:48  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:07:27  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:20:59  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  Copyright (c) 1990, 1991 by
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
 *	CRVB_MUT_002
 *
 *  TEST TOPICS:
 *
 *	MUT  -  Mutex Operations
 *
 *  ABSTRACT:
 *
 *	Test new "mutex kind" attribute; ensure that a mutex with a
 *	kind of "cma_c_friendly" will allow the same thread to nest
 *	multiple locks, and correctly maintain a reference count; and
 *	that it won't allow another thread to lock it (but that the
 *	other thread will be correctly unblocked when the owner
 *	releases all nested locks).
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	27 June 1990
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Webb Scales	1990
 *		Corrected filename references.
 *	002	Dave Butenhof	5 September 1990
 *		Convert to new CMA interfaces.
 *	003	Dave Butenhof	12 February 1991
 *		Now "friendly" mutex is "recursive"
 */

/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <dce/cts_test.h>

#define LOOP	10
#define NUMTHD	10

cma_t_thread	thd[NUMTHD], guard;
cma_t_mutex	recursive;
cma_t_attr	recursive_attr;
cma_t_integer	nest_count, mutex_owner;

static cma_t_address thread (cma_t_integer);

static cma_t_address guardian (cma_t_address);

int
main	(int argc, char *argv[], char *envp[])
    {
    cma_t_exit_status	status;
    cma_t_address	value;
    cma_t_integer	i;


    cts_test ("CRVB_MUT_002", "Test recursive mutexes");
    cma_init ();

    cma_thread_get_self (&thd[0]);	/* Store main thread ID for guardian */
    cma_attr_create (&recursive_attr, &cma_c_null);
    cma_attr_set_mutex_kind (&recursive_attr, cma_c_mutex_recursive);
    cma_mutex_create (&recursive, &recursive_attr);
    nest_count = 0;
    mutex_owner = -1;

    cma_thread_create (&guard, &cma_c_null, guardian, cma_c_null_ptr);

    for (i = 1; i < NUMTHD; i++)
	cma_thread_create (
		&thd[i],
		&cma_c_null,
		(cma_t_start_routine)thread,
		(cma_t_address)i);

    thread (0);				/* Call work routine for main thread */

    for (i = 1; i < NUMTHD; i++) {
	cma_thread_join (&thd[i], &status, &value);

	if (status != cma_c_term_normal)
	    cts_failed ("thread %d terminated with error", i);

	}

    cma_thread_alert (&guard);

    if (mutex_owner != -1)
	cts_failed ("final mutex owner is %d (should be -1)", mutex_owner);

    if (nest_count != 0)
	cts_failed ("final nest is %d (should be 0)", nest_count);

    cts_result ();
    }

static cma_t_address
thread	(cma_t_integer	thdnum)
    {
    cma_t_integer	i;


#if 0
    if (cma_mutex_try_lock (&recursive)) {
	cma_mutex_unlock (&recursive);
	printf ("%d starting with mutex unlocked\n", thdnum);
	}
    else
	printf ("%d starting with mutex locked\n", thdnum);
#endif

    cma_mutex_lock (&recursive);
#if 0
    printf ("%d locking mutex (%d)\n", thdnum, nest_count);
#endif

    if (mutex_owner != -1)
	cts_failed (
		"%d acquired lock with owner == %d",
		thdnum,
		mutex_owner);

    if (nest_count != 0)
	cts_failed (
		"%d acquired lock with nest count == %d",
		thdnum,
		nest_count);

    nest_count++;
    mutex_owner = thdnum;

    for (i = 1; i < LOOP; i++) {
	cma_mutex_lock (&recursive);
#if 0
	printf ("%d locking mutex (%d)\n", thdnum, nest_count);
#endif

	if (mutex_owner != thdnum)
	    cts_failed (
		    "lock stolen from %d by %d during lock",
		    thdnum,
		    mutex_owner);
	
	if (nest_count != i)
	    cts_failed (
		    "%d: nesting corrupted during lock (%d instead of %d)",
		    thdnum,
		    nest_count,
		    i);

	nest_count++;
	cma_yield ();
	}

    for (i = LOOP - 1; i >= 0; i--) {

	if (mutex_owner != thdnum)
	    cts_failed (
		    "lock stolen from %d by %d during unlock",
		    thdnum,
		    mutex_owner);
	
	if (--nest_count == 0)
	    mutex_owner = -1;

	if (nest_count != i)
	    cts_failed (
		    "%d: nesting corrupted during unlock (%d instead of %d)",
		    thdnum,
		    nest_count,
		    i);

#if 0
	printf ("%d unlocking mutex (%d)\n", thdnum, nest_count);
#endif
	cma_mutex_unlock (&recursive);
	cma_yield ();
	}

    }

static cma_t_address
guardian	(cma_t_address	arg)
    {
    cma_t_integer	i;


    /*
     * Just wait for a reasonable amount of time.  If we haven't been
     * terminated by then (by the completion of the test), then alert the
     * main thread to ensure that the test fails.
     */
    cma_delay (120.0);			/* Wait for 2 minutes */
    cts_failed ("test timed out");

    for (i = 1; i < NUMTHD; i++)
	cma_thread_alert (&thd[i]);

    cma_delay (120.0);			/* Let main thread join & report */
    cma_thread_alert (&thd[0]);
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_MUT_002.C */
/*  *7    13-FEB-1991 18:00:33 BUTENHOF "Change names of mutex attributes" */
/*  *6    12-FEB-1991 23:08:47 BUTENHOF "Change 'friendly' to 'recursive'" */
/*  *5     5-SEP-1990 09:40:59 BUTENHOF "Convert to new CMA interfaces" */
/*  *4    18-JUL-1990 16:27:51 SCALES "Fix filename references" */
/*  *3     3-JUL-1990 12:04:18 BUTENHOF "Change copyright" */
/*  *2     2-JUL-1990 15:34:51 BUTENHOF "Put the module in library (??)" */
/*  *1     2-JUL-1990 15:25:00 BUTENHOF "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_MUT_002.C */
