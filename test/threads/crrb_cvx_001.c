/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crrb_cvx_001.c,v $
 * Revision 1.1.8.2  1996/02/17  23:29:38  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:46:25  marty]
 *
 * Revision 1.1.8.1  1995/12/11  22:57:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:34:48  root]
 * 
 * Revision 1.1.6.1  1994/04/01  20:08:36  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:11:49  jd]
 * 
 * Revision 1.1.4.1  1993/10/12  19:34:28  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:29:34  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:28:02  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:04:43  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:20:10  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  Copyright (c) 1990 by
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
 *	CRRB_CVX_001
 *
 *  TEST TOPICS:
 *
 *	CVX  -  Condition Variable Operations
 *
 *  ABSTRACT:
 *
 *	CMA BL4 developed a bug where a signal to a condition would
 *	mask a later broadcast (by setting the cv->event bit).  This
 *	test creates two threads which wait on a condition; the main
 *	thread waits sufficiently long to ensure both are waiting,
 *	then signals the condition (to wake one waiter), and then
 *	performs a broadcast; the test will fail if the remaining
 *	thread isn't awakened within a reasonable period of time.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	26 November 1990
 *
 *  MODIFICATION HISTORY:
 *
 */

/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <dce/cts_test.h>

cma_t_thread	thread1, thread2, watcher;
cma_t_cond	cv;
cma_t_mutex	m;
cma_t_boolean	p1, p2;

static cma_t_address
waiter1(cma_t_address arg)
    {
    cma_mutex_lock (&m);

    while (! p1)
	cma_cond_wait (&cv, &m);

    cma_mutex_unlock (&m);
    }

static cma_t_address
waiter2	(cma_t_address arg)
    {
    cma_mutex_lock (&m);

    while (! p2)
	cma_cond_wait (&cv, &m);

    cma_mutex_unlock (&m);
    }

static cma_t_address
watchdog	(cma_t_address arg)
    {
    cma_delay (20.0);
    cma_thread_alert (&thread1);
    cma_thread_alert (&thread2);
    }

int
main	(int argc, char *argv[], char *envp[])
    {
    cma_t_exit_status	status1, status2;
    cma_t_address	result;

    cts_test ("CRRB_CVX_001", "Signal shouldn't mask later broadcast");
    cma_init ();

    p1 = cma_c_false;
    p2 = cma_c_false;
    cma_mutex_create (&m, &cma_c_null);
    cma_cond_create (&cv, &cma_c_null);

    cma_thread_create (&thread1, &cma_c_null, waiter1, cma_c_null_ptr);
    cma_thread_create (&thread2, &cma_c_null, waiter2, cma_c_null_ptr);
    cma_thread_create (&watcher, &cma_c_null, watchdog, cma_c_null_ptr);

    cma_delay (2.0);			/* Let them both get to wait */

    p1 = cma_c_true;			/* Set first predicate */
    cma_cond_signal (&cv);

    p2 = cma_c_true;			/* Set second predicate */
    cma_cond_broadcast (&cv);

    cma_thread_join (&thread1, &status1, &result);
    cma_thread_join (&thread2, &status2, &result);

    if (status1 == cma_c_term_alert || status2 == cma_c_term_alert)
	cts_failed ("Test timed out; probably a hang.");

    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRRB_CVX_001.C */
/*  *1    26-NOV-1990 23:39:19 BUTENHOF "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRRB_CVX_001.C */
