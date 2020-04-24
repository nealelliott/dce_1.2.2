/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crvb_cvx_tim_001.c,v $
 * Revision 1.1.52.2  1996/02/17  23:30:08  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:46:53  marty]
 *
 * Revision 1.1.52.1  1995/12/11  22:58:55  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/jrr_1.2_mothra/1  1995/12/06  17:16 UTC  psn
 * 	Fix to resolve timing problem.
 * 	[1995/12/11  22:35:12  root]
 * 
 * Revision 1.1.49.1  1994/04/01  20:09:25  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:13:37  jd]
 * 
 * Revision 1.1.47.1  1993/10/12  19:35:05  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:29:52  rhw]
 * 
 * Revision 1.1.3.3  1992/12/30  16:30:01  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:06:49  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/03  14:31:21  bolinger
 * 	Move current 1.0.1-backed file (modified to work with new
 * 	threads source base) from private branch into 1.0.2 tree.
 * 	(See OT defect 5300.)
 * 	[1992/09/02  13:44:21  bolinger]
 * 
 * Revision 1.1.1.2  1992/08/12  21:01:26  bolinger
 * 	Replace uses of struct timeb/ftime(3) with refs to new timing
 * 	interface specific to these tests (which is designed to hide
 * 	any further changes to the underlying system facilities used).
 * 
 * Revision 1.1  1992/01/19  03:20:50  devrcs
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
 *	RV_CVX_TIM_001
 *
 *  TEST TOPICS:
 *
 *	CVX  -  Cond Variable Operations
 *	TIM  -  Timer Operations
 *
 *  ABSTRACT:
 *
 *	Make sure that a timed wait works correctly regardless of
 *	whether the wait completes, or is timed out.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	20 November 1989
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	29 August 1990
 *		convert to use new interfaces.
 *	002	Dave Butenhof	08 May 1991
 *		Cast thread start function properly.
 */

/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <dce/cts_test.h>
#include <stdio.h>
#ifdef vms
#include <timeb.h>
#endif

cma_t_cond	cv;
cma_t_mutex	mutex;
cma_t_integer	variable;

static cma_t_boolean test_it (cma_t_integer,cma_t_interval *);

static cma_t_interval dif_secs (cts_timebuf_t *,cts_timebuf_t *);

static cma_t_address do_signal (cma_t_integer);

main ()
    {
    cma_t_interval	wait;
    cma_t_boolean	timed_out;


    cts_test ("CRVB_CVX_TIM_001", "Test cma_cond_timed_wait with timeout and signals");

    cma_init ();

    cma_cond_create (&cv, &cma_c_null);
    cma_mutex_create (&mutex, &cma_c_null);

    cts_comment ("Part 1: signal after timeout interval");

    if (! test_it (15, &wait))
	cts_failed ("Wait didn't time out.");

    if (wait < 10 || wait > 14)
	cts_failed (
		"Expected 10 second interval: actual was %f seconds",
		wait);

    cts_comment ("Part 2: signal before timeout interval");

    if (test_it (5, &wait))
	cts_failed ("Wait timed out.");

    if (wait < 5 || wait > 8)
	cts_failed (
		"Expected 5 second interval: actual was %f seconds",
		wait);

    cts_result ();
    }

static cma_t_boolean
test_it	(
	cma_t_integer	delay,
	cma_t_interval	*wait)
    {
    cma_t_thread	thread;
    cma_t_date_time	expiration;
    cts_timebuf_t		time1, time2;
    cma_t_address	result;
    cma_t_exit_status	exit;
    cma_t_status	status;


    cma_mutex_lock (&mutex);
    variable = 0;
    cma_thread_create (
	    &thread,
	    &cma_c_null,
	    (cma_t_start_routine)do_signal,
	    (cma_t_address)delay);
    cts_gettime (&time1);
    cma_time_get_expiration (&expiration, 10.0);

    while ((variable == 0)
	    && ((status
	    = cma_cond_timed_wait (&cv, &mutex, &expiration)) != cma_s_timed_out))
	;

    cts_gettime (&time2);

    cma_mutex_unlock (&mutex);
    *wait = dif_secs (&time1, &time2);
    cma_thread_join (&thread, &exit, &result);
    cma_thread_detach (&thread);
    
    return (status == cma_s_timed_out);
    }

static cma_t_interval
dif_secs	(
	cts_timebuf_t	*t1,
	cts_timebuf_t	*t2)
    {
	return (cts_timediff(t1, t2));
    }

static cma_t_address
do_signal	(
	cma_t_integer	delay)
    {
    cma_delay ((float)delay);
    cma_mutex_lock (&mutex);
    variable = 1;
    cma_mutex_unlock (&mutex);
    cma_cond_signal (&cv);
    return cma_c_null_ptr;
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_CVX_TIM_001.C */
/*  *6    15-MAY-1991 16:16:47 BUTENHOF "Cast thread function" */
/*  *5     5-SEP-1990 09:40:39 BUTENHOF "Convert to new interface" */
/*  *4     3-JUL-1990 12:03:40 BUTENHOF "Change copyright" */
/*  *3    21-JUN-1990 08:27:58 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *2     7-JUN-1990 11:36:03 SCALES "Updates to CTS" */
/*  *1    17-MAY-1990 13:57:50 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_CVX_TIM_001.C */
