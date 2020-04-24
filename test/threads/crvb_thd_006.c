/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems
 * Burlington, MA, USA
 * All Rights Reserved
 * (c) Copyright 1990, 1991, 1992
 */
/*
 * HISTORY
 * $Log: crvb_thd_006.c,v $
 * Revision 1.1.22.2  1996/02/17  23:30:35  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:16  marty]
 *
 * Revision 1.1.22.1  1995/12/11  23:00:11  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/jrr_1.2_mothra/1  1995/12/06  16:30 UTC  psn
 * 	Merge mothra fixes.
 * 	[1995/12/11  22:35:36  root]
 * 
 * Revision 1.1.20.1  1994/04/01  20:10:17  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:15:32  jd]
 * 
 * Revision 1.1.18.2  1993/10/12  21:24:57  rhw
 * 	Fix for OT8201 - increase min_iterations for i486
 * 	[1993/10/12  20:43:52  rhw]
 * 
 * Revision 1.1.18.1  1993/10/12  19:35:53  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:15  rhw]
 * 
 * Revision 1.1.16.3  1993/06/14  20:16:40  sommerfeld
 * 	#undef only gets one argument: a symbol.
 * 	Define min_iterations on HP-UX
 * 	[1993/06/10  23:22:25  sommerfeld]
 * 
 * Revision 1.1.16.2  1993/05/24  20:49:05  cjd
 * 	Submitting 102-dme port to 103i
 * 	[1993/05/24  20:16:27  cjd]
 * 
 * Revision 1.1.14.2  1993/05/12  18:12:45  jd
 * 	Initial 486 port
 * 	[1993/05/12  18:12:07  jd]
 * 
 * Revision 1.1.7.3  1992/12/30  16:32:17  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:21:56  zeliff]
 * 
 * Revision 1.1.7.2  1992/09/29  20:38:37  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:32:45  sekhar]
 * 
 * Revision 1.1.2.2  1992/05/22  20:33:09  bolinger
 * 	Workaround (not real fix) for OT defect 2281:  Increase length of test
 * 	on RIOS ref platform.  Whether the test succeeds depends on some
 * 	complex interaction between the state (idleness) of the machine, the
 * 	contents of the test, and the number of iterations each thread in the
 * 	test runs.  We'll work on this further after 1.0.1 -- but for now, this
 * 	patch makes it run very consistently on the RIOS.  (There's never been
 * 	a problem on the PMAX.)
 * 	[1992/05/22  20:31:47  bolinger]
 * 
 * Revision 1.1  1992/01/19  03:21:16  devrcs
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
 *	CRVB_THD_006
 *
 *  TEST TOPICS:
 *
 *	THD  -  Thread Operations
 *
 *  ABSTRACT:
 *
 *	This test endeavors to determine whether and implementation
 *	has farily equitable timeslicing.
 *
 *  AUTHORS:
 *
 *	Webb Scales
 *
 *  CREATION DATE:
 *
 *	08 February 1990
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Paul Curtin	4 September 1990
 *		Adjust test name in cts_test statement.
 *		Convert to new CMA interfaces.
 *	002	Dave Butenhof	3 January 1991
 *		Make main thread FIFO so that it has a chance of getting the
 *		"quit mutex" so that it can tell the other threads to stop
 *		running.
 *	003	Dave Butenhof	4 January 1991
 *		Give each thread its own "quit" mutex to reduce contention.
 *	004	Dave Butenhof & Webb Scales	29 January 1991
 *		Try to make the test more tolerant of loaded systems, by
 *		letting it run at least until one thread reaches <some large
 * 		number> of iterations.
 *	005	Dave Butenhof	23 May 1991
 *		Fix compilation errors under new MIPS C compiler.
 */

/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <dce/cts_test.h>
#include <stdio.h>

#define thread_count	10
#define threshold	10
#undef	scale_lim
#define	delay_lim	100
#define print_all	1
#if	_CMA_PLATFORM_ == _CMA__IBMR2_UNIX
#define min_iterations	500000
#else
#ifdef __hpux
#define min_iterations	500000
#else
#define min_iterations	100000
#endif
#endif


cma_t_boolean	start = cma_c_false, maybe_done = cma_c_false;
cma_t_boolean	quit[thread_count];
cma_t_mutex	smutex, maybe_done_mutex, qmutex[thread_count];
cma_t_cond	start_cv, maybe_done_cv;


static cma_t_address
thread	(
	cma_t_natural	number)
    {
    cma_t_integer 	count = 0;
    cma_t_natural	scale_ctr, delay_ctr;


    cma_mutex_lock (&smutex);
    while (!start)
	cma_cond_wait (&start_cv, &smutex);
    cma_mutex_unlock (&smutex);

    while (++count > 0) 
#ifdef scale_lim
	for (scale_ctr = 0; scale_ctr < scale_lim; scale_ctr++) 
#endif
	    {
	    cma_mutex_lock (&qmutex[number]);
	    if (quit[number]) {
		cma_mutex_unlock (&qmutex[number]);
		return (cma_t_address)count;
		}
	    cma_mutex_unlock (&qmutex[number]);

	    if (count == min_iterations) {
		cma_mutex_lock (&maybe_done_mutex);
		maybe_done = cma_c_true;
		cma_mutex_unlock (&maybe_done_mutex);
		cma_cond_signal (&maybe_done_cv);
		}

#ifdef delay_lim
	    for (delay_ctr = 0; delay_ctr < delay_lim; delay_ctr++);
#endif
	    }

    printf ("Thread #%d overflowed.\n", number);
    cma_thread_exit_error();	/* Counter overflow */

    return (cma_t_address)count; /* bogus */
    }


main ()
    {
    cma_t_thread	threads[thread_count];
    cma_t_thread	self;
    cma_t_attr		attr;
    cma_t_natural	i, dif, dev;
    cma_t_natural	max = 0, min = -1, ave = 0;
    cma_t_address	result;
    cma_t_exit_status	exit;
    cma_t_natural	begin, end;


    cts_test ("CRVB_THD_006", "Test for timeslicing fairness.");

    cma_init ();

    cma_thread_get_self (&self);
    cma_thread_set_sched (&self, cma_c_sched_fifo, cma_c_prio_fifo_max);
    cma_attr_create (&attr, &cma_c_null);
    cma_attr_set_inherit_sched (&attr, cma_c_sched_use_default);
    cma_mutex_create (&smutex, &cma_c_null);
    cma_mutex_create (&maybe_done_mutex, &cma_c_null);
    cma_cond_create (&maybe_done_cv, &cma_c_null);

    for (i = 0; i < thread_count; i++) {
	quit[i] = cma_c_false;
	cma_mutex_create (&qmutex[i],   &cma_c_null);
	}

    cma_cond_create  (&start_cv, &cma_c_null);

    for (i = 0; i < thread_count; i++)
	cma_thread_create (
		&threads[i],
		&attr,
		(cma_t_start_routine)thread,
		(cma_t_address)i);

    fprintf (stderr, "Starting...");
    cma_mutex_lock (&smutex);
    start = cma_c_true;
    cma_mutex_unlock (&smutex);
    cma_cond_broadcast (&start_cv);
    begin = time(0);

    cma_mutex_lock (&maybe_done_mutex);

    while (!maybe_done)
	cma_cond_wait (&maybe_done_cv, &maybe_done_mutex);

    cma_mutex_unlock (&maybe_done_mutex);

    for (i = 0; i < thread_count; i++) {
	cma_mutex_lock (&qmutex[i]);
	quit[i] = cma_c_true;
	cma_mutex_unlock (&qmutex[i]);
	}

    end = time(0);
    fprintf (stderr, "...done.\n\n");

#ifdef print_all
    printf ("     Thread\t     Iterations\n");
#endif
    for (i = 0; i < thread_count; i++) {
	cma_thread_join (
		&threads[i],
		&exit,
		&result);

	if (exit != cma_c_term_normal) {
	    cts_failed("Thread error.");
	    printf ("Thread #%d overflowed.\n", i);
	    }
#ifdef print_all
	else
	    printf ("\t%d\t\t%06d\n", i, (cma_t_natural)result);
#endif

        if (min > (cma_t_natural)result)   min = (cma_t_natural)result;
        if (max < (cma_t_natural)result)   max = (cma_t_natural)result;
	ave += ((cma_t_natural)result)/thread_count;

	cma_thread_detach (&threads[i]);
	}

    printf ("\nFor %d threads, running for %d seconds:\n", 
	    thread_count, end - begin);
    printf ("\tAverage: %d\n\tMaximum: %d\n\tMinimum: %d\n", ave, max, min);

    max = abs(max - ave);
    min = abs(ave - min);
    dif = (max > min ? max : min);
    dev = (100 * dif)/ave;

    printf ("Deviation: %d%%\t(Threshold: %d%%)\n\n", dev, threshold);

    if (dev > threshold) cts_failed("Unequal distribution of execution.");

    cts_result ();

    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_THD_006.C */
/*  *10   29-MAY-1991 18:02:37 BUTENHOF "Fix compilation errors" */
/*  *9    29-JAN-1991 17:58:07 BUTENHOF "Try to improve tests' tolerance to loaded systems" */
/*  *8     4-JAN-1991 19:34:41 BUTENHOF "Tweak the locking..." */
/*  *7     4-JAN-1991 01:50:18 BUTENHOF "Make main thread FIFO to avoid lockout" */
/*  *6     5-SEP-1990 13:30:07 CURTIN "Converted to use new CMA interface" */
/*  *5    16-JUL-1990 13:43:27 SCALES "test bug: omitted broadcast" */
/*  *4     3-JUL-1990 12:05:57 BUTENHOF "Change copyright" */
/*  *3    21-JUN-1990 08:32:49 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *2     7-JUN-1990 11:39:10 SCALES "Updates to CTS" */
/*  *1    17-MAY-1990 13:58:57 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_THD_006.C */
