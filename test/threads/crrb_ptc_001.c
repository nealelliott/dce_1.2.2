/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crrb_ptc_001.c,v $
 * Revision 1.1.10.2  1996/02/17  23:29:41  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:46:29  marty]
 *
 * Revision 1.1.10.1  1995/12/11  22:57:47  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:34:50  root]
 * 
 * Revision 1.1.7.1  1994/04/01  20:08:42  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:12:02  jd]
 * 
 * Revision 1.1.5.1  1993/10/12  19:34:34  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:29:36  rhw]
 * 
 * Revision 1.1.3.3  1992/12/30  16:28:10  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:04:55  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/03  14:31:05  bolinger
 * 	Move current 1.0.1-backed file (modified to work with new
 * 	threads source base) from private branch into 1.0.2 tree.
 * 	(See OT defect 5300.)
 * 	[1992/09/02  13:44:06  bolinger]
 * 
 * Revision 1.1.1.2  1992/08/12  20:56:44  bolinger
 * 	Change to reflect code change in thread rundown (thread-specific data
 * 	destructor interface) -- data is now cleared *after* destructor is run,
 * 	so anything put there by destructor isn't visible afterward.
 * 
 * Revision 1.1  1992/01/19  03:20:13  devrcs
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
 *	CRRB_PTC_001
 *
 *  TEST TOPICS:
 *
 *	PTC  -  Per Thread Context
 *
 *  ABSTRACT:
 *
 *	Having decided that a ptc destructor routine ought to be
 *	able to make use of CMA functions which lock the TCB (such
 *	as cma_key_get_context), this test will verify that a
 *	destructor can do so without causing a hang, or anything
 *	else nasty.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	19 June 1990
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	4 September 1990
 *		Change to new CMA interfaces.
 *	002	Dave Butenhof	27 September 1990
 *		Destructors are no longer called by 'set' function, so don't
 *		expect them to be.
 *	003	Dave Butenhof	23 May 1991
 *		Increase timeouts to avoid spurious errors on initial
 *		OSF/1-based DECthreads (where kernel threads need to use slow
 *		kernel operations for synchronization).
 */

/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <dce/cts_test.h>

static cma_t_key	ctx;
static cma_t_boolean	destructor_called;
static cma_t_integer	data1, data2;
static cma_t_address	expected_data, known_data;
static cma_t_mutex	dest_mut;
static cma_t_cond	dest_wait;

static cma_t_address thread  (cma_t_address);

static void destructor  (cma_t_address);

main ()
    {
    cma_t_thread	thd;
    cma_t_exit_status	status;
    cma_t_address	result;
    cma_t_date_time	time;


    cts_test ("CRRB_PTC_001", "Test use of TCB lock from destructor");
    cma_init ();

    cma_mutex_create (&dest_mut, &cma_c_null);
    cma_cond_create (&dest_wait, &cma_c_null);
    cma_key_create (&ctx, &cma_c_null, destructor);

    cma_thread_create (&thd, &cma_c_null, thread, cma_c_null_ptr);

    cma_mutex_lock (&dest_mut);
    cma_time_get_expiration (&time, 5.0*60.0);

    if (cma_cond_timed_wait (&dest_wait, &dest_mut, &time) == cma_s_timed_out) {
	/*
	 * Assume that the worker thread is hung on the TCB mutex; report an
	 * error and alert it.
	 */
	cts_failed ("worker thread failed to complete in 5 minutes");
	}
    else {
	cma_thread_join (&thd, &status, &result);

	if (!destructor_called)
	    cts_failed ("destructor not called on thread rundown");
	}

    cma_mutex_unlock (&dest_mut);

    cts_result ();
    }

static cma_t_address
thread    (cma_t_address	arg)
    {
    destructor_called = cma_c_false;
    known_data = cma_c_null_ptr;
    expected_data = (cma_t_address)&data1;
    cma_key_set_context (ctx, (cma_t_address)&data1);
    known_data = (cma_t_address)&data1;

    if (destructor_called)
	cts_failed ("destructor called when it shouldn't have been");

    expected_data = cma_c_null_ptr;
    destructor_called = cma_c_false;
    }

static void
destructor    (cma_t_address	val)
    {
    cma_t_address	data;


    destructor_called = cma_c_true;
    cma_key_get_context (ctx, &data);

    if (val != known_data)
	cts_failed (
		"old context value %08X doesn't match expected %08X",
		val,
		known_data);

    if (data != known_data)
	cts_failed (
		"new context value %08X doesn't match expected %08X",
		data,
		expected_data);

    /*
     * If this is thread rundown, signal the main thread that we're
     * finished.
     */
    if (expected_data == cma_c_null_ptr)
	cma_cond_signal (&dest_wait);

    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRRB_PTC_001.C */
/*  *8    29-MAY-1991 18:01:47 BUTENHOF "Increase timeouts on slower OSF/1 systems" */
/*  *7    27-SEP-1990 14:09:27 BUTENHOF "Don't expect destructor call on 'set'" */
/*  *6     5-SEP-1990 09:39:10 BUTENHOF "Convert to new CMA interfaces" */
/*  *5     3-JUL-1990 12:02:32 BUTENHOF "Change copyright" */
/*  *4    27-JUN-1990 10:27:29 BUTENHOF "Fix bugs found by MIPS compiler" */
/*  *3    21-JUN-1990 11:47:16 BUTENHOF "Fix pointer types for MIPS cc" */
/*  *2    21-JUN-1990 08:25:09 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *1    19-JUN-1990 08:09:40 BUTENHOF "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRRB_PTC_001.C */
