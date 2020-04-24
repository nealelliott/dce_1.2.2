/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crvb_alt_001.c,v $
 * Revision 1.1.9.2  1996/02/17  23:29:49  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:46:36  marty]
 *
 * Revision 1.1.9.1  1995/12/11  22:58:07  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:34:57  root]
 * 
 * Revision 1.1.6.1  1994/04/01  20:08:50  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:12:20  jd]
 * 
 * Revision 1.1.4.1  1993/10/12  19:34:45  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:29:42  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:28:43  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:05:25  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:20:23  devrcs
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
 *	CRVB_ALT_001
 *
 *  TEST TOPICS:
 *
 *	ALT  -  Alerts
 *
 *  ABSTRACT:
 *
 *	Attempt to alert threads with and without exception handlers,
 *	verify correct thread exit or handling.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	12 December 1989
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	4 September 1990
 *		Convert to new CMA interfaces.
 *	002	Dave Butenhof	7 February 1991
 *		New alert control interfaces.
 *	003	Dave Butenhof	14 May 1991
 *		change name of 'signal' function to avoid conflicts (since
 *		cma.h is including cma_sigwait.h, which includes signal.h).
 */

/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <dce/cts_test.h>

cma_t_thread	thread;
cma_t_mutex	mutex;
cma_t_cond	cv;
cma_t_boolean	variable;

static void compute_bound  (cma_t_address);

static void delayer  (cma_t_address);

static void timed_waiter  (cma_t_address);

static void waiter  (cma_t_address);

static void suicide  (cma_t_address);

static void joiner  (cma_t_address);

static void do_signal  (cma_t_address);

static void dummy  (cma_t_address);

main ()
    {
    cma_t_exit_status	status;
    cma_t_address	result;


    cts_test ("CRVB_ALT_001", "Basic test of alerts");

    cma_init ();

    cma_mutex_create (&mutex, &cma_c_null);
    cma_cond_create (&cv, &cma_c_null);

    cts_comment ("part 1: alert compute-bound thread");
    cma_thread_create (
	    &thread,
	    &cma_c_null,
	    (cma_t_start_routine)compute_bound,
	    cma_c_null_ptr);
    cma_delay (2.0);			/* Let it get started */
    cma_thread_alert (&thread);
    cma_thread_join (&thread, &status, &result);
    cma_thread_detach (&thread);

    if (status != cma_c_term_alert)
	cts_failed ("didn't terminate with ALERTED exit status");

    cts_comment ("part 2: alert waiting thread (cma_delay)");
    cma_thread_create (
	    &thread,
	    &cma_c_null,
	    (cma_t_start_routine)delayer,
	    cma_c_null_ptr);
    cma_delay (2.0);
    cma_thread_alert (&thread);
    cma_thread_join (&thread, &status, &result);
    cma_thread_detach (&thread);

    if (status != cma_c_term_alert)
	cts_failed ("didn't terminate with ALERTED exit status");

    cts_comment ("part 3: alert waiting thread (cma_cond_timed_wait)");
    cma_thread_create (
	    &thread,
	    &cma_c_null,
	    (cma_t_start_routine)timed_waiter,
	    cma_c_null_ptr);
    cma_delay (2.0);
    cma_thread_alert (&thread);
    cma_thread_join (&thread, &status, &result);
    cma_thread_detach (&thread);

    if (status != cma_c_term_alert)
	cts_failed ("didn't terminate with ALERTED exit status");

    cts_comment ("part 4: alert waiting thread (cma_cond_wait)");
    cma_thread_create (
	    &thread,
	    &cma_c_null,
	    (cma_t_start_routine)waiter,
	    cma_c_null_ptr);
    cma_delay (2.0);
    cma_thread_alert (&thread);
    cma_thread_join (&thread, &status, &result);
    cma_thread_detach (&thread);

    if (status != cma_c_term_alert)
	cts_failed ("didn't terminate with ALERTED exit status");

    cts_comment ("part 5: alert self");
    cma_thread_create (
	    &thread,
	    &cma_c_null,
	    (cma_t_start_routine)suicide,
	    cma_c_null_ptr);
    cma_thread_join (&thread, &status, &result);
    cma_thread_detach (&thread);

    if (status != cma_c_term_alert)
	cts_failed ("didn't terminate with ALERTED exit status");

    cts_comment ("part 6: alert thread doing cma_thread_join");
    cma_thread_create (
	    &thread,
	    &cma_c_null,
	    (cma_t_start_routine)joiner,
	    cma_c_null_ptr);
    cma_delay (2.0);
    cma_thread_alert (&thread);
    cma_thread_join (&thread, &status, &result);
    cma_thread_detach (&thread);

    if (status != cma_c_term_alert)
	cts_failed ("didn't terminate with ALERTED exit status");

    cts_result ();

    }

static void
compute_bound	(
	cma_t_address	arg)
    {
    cma_t_integer	cnt;

    for (cnt = 0; cnt < 100000000; cnt++)
	cma_alert_test ();

    cts_failed ("compute-bound loop completed without alert");
    }

static void
delayer	(
	cma_t_address	arg)
    {
    cma_delay (10.0);
    cts_failed ("10 second delay completed without alert");
    }

static void
timed_waiter	(
	cma_t_address	arg)
    {
    cma_t_date_time	timeout;


    cma_time_get_expiration (&timeout, 10.0);
    cma_mutex_lock (&mutex);

    TRY {
	while (cma_cond_timed_wait (&cv, &mutex, &timeout) != cma_s_timed_out);
	cts_failed ("10 second wait elapsed without alert");
	}
    CATCH (cma_e_alerted) {
	cma_mutex_unlock (&mutex);
	RERAISE;
	}
    ENDTRY

    cma_mutex_unlock (&mutex);
    }

static void
waiter	(
	cma_t_address	arg)
    {
    cma_t_exit_status	status;
    cma_t_address	result;
    cma_t_thread	dt;


    cma_thread_create (
	    &dt,
	    &cma_c_null,
	    (cma_t_start_routine)do_signal,
	    cma_c_null_ptr);
    cma_mutex_lock (&mutex);
    variable = cma_c_false;

    TRY {

	while (! variable)
	    cma_cond_wait (&cv, &mutex);

	cts_failed ("10 seconds elapsed without alert");
	}
    CATCH (cma_e_alerted) {
	cma_mutex_unlock (&mutex);
	cma_thread_alert (&dt);
	cma_thread_detach (&dt);
	RERAISE;
	}
    ENDTRY

    cma_mutex_unlock (&mutex);
    cma_thread_detach (&dt);
    }

static void
suicide	(
	cma_t_address	arg)
    {
    cma_t_thread	self;


    cma_thread_get_self (&self);
    cma_thread_alert (&self);
    cma_alert_test ();
    cts_failed ("continued after alerting self");
    }

static void
joiner	(
	cma_t_address	arg)
    {
    cma_t_exit_status	status;
    cma_t_address	result;
    cma_t_thread	dt;


    cma_thread_create (
	    &dt,
	    &cma_c_null,
	    (cma_t_start_routine)dummy,
	    cma_c_null_ptr);

    TRY {
	cma_thread_join (&dt, &status, &result);
	}
    CATCH (cma_e_alerted) {
	cma_thread_alert (&dt);
	cma_thread_detach (&dt);
	RERAISE;
	}
    ENDTRY

    cma_thread_detach (&dt);
    cts_failed ("cma_thread_join did not raise cma_e_alerted");
    }

static void
do_signal	(
	cma_t_address	arg)
    {
    cma_delay (10.0);
    cma_mutex_lock (&mutex);
    variable = cma_c_true;
    cma_mutex_unlock (&mutex);
    cma_cond_signal (&cv);
    }

static void
dummy	(
	cma_t_address	arg)
    {
    cma_delay (10.0);
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_ALT_001.C */
/*  *8    14-MAY-1991 16:34:49 BUTENHOF "Change name of 'signal' function" */
/*  *7    12-FEB-1991 01:30:26 BUTENHOF "Changes to alert control" */
/*  *6    10-SEP-1990 12:33:01 BUTENHOF "Fix conversion bug" */
/*  *5     5-SEP-1990 09:39:26 BUTENHOF "Convert to new CMA interfaces" */
/*  *4     3-JUL-1990 12:03:02 BUTENHOF "Change copyright" */
/*  *3    21-JUN-1990 08:26:23 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *2     7-JUN-1990 11:35:20 SCALES "Updates to CTS" */
/*  *1    17-MAY-1990 13:57:33 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_ALT_001.C */
