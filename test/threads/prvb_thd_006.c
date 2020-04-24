/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: prvb_thd_006.c,v $
 * Revision 1.1.21.2  1996/02/17  23:32:06  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:48:31  marty]
 *
 * Revision 1.1.21.1  1995/12/11  23:04:16  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/jrr_1.2_mothra/1  1995/12/06  16:43 UTC  psn
 * 	Merge mothra fixes.
 * 	[1995/12/11  22:36:45  root]
 * 
 * Revision 1.1.19.1  1994/04/01  20:12:30  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:20:36  jd]
 * 
 * Revision 1.1.17.2  1993/10/12  21:24:58  rhw
 * 	Fix for OT8201 - increase min_iterations for i486
 * 	[1993/10/12  20:43:53  rhw]
 * 
 * Revision 1.1.17.1  1993/10/12  19:38:12  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:31:20  rhw]
 * 
 * Revision 1.1.15.2  1993/06/14  20:19:07  sommerfeld
 * 	#undef only gets one argument: a symbol.
 * 	Define min_iterations on HP-UX
 * 	[1993/06/10  23:23:56  sommerfeld]
 * 
 * Revision 1.1.8.4  1993/02/01  21:46:19  hinman
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  17:07:32  hinman]
 * 
 * Revision 1.1.8.3  1992/12/30  16:55:35  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:13:38  zeliff]
 * 	Revision 1.1.11.2  1993/01/11  16:17:20  hinman
 * 	[hinman] - Check in merged SNI version
 * 
 * Revision 1.1.8.2  1992/10/14  20:45:06  marty
 * 	OSC1.1.1 upgrade. Changes are compatible with osc1.0.4.
 * 	[1992/10/14  20:03:50  marty]
 * 
 * Revision 1.1.6.2  1992/10/08  16:25:44  garyf
 * 	add 386 support
 * 	[1992/09/29  20:28:28  garyf]
 * 
 * Revision 9.5.3.2  92/10/27  12:57:54  bein
 * 	PTC - fixed typo - _CMA_SVR4 --> _CMA__SVR4 - was causing compile to fail.
 * 
 * Revision 9.5.1.4  92/10/20  16:40:23  raj
 * 	Merge in SVR4 changes
 * 
 * Revision 9.5.2.2  92/10/20  16:39:13  raj
 * 	Merge in SVR4 changes
 * 
 * Revision 9.5.1.3  92/08/05  13:56:20  hinman
 * 	[hinman] - Fold backward from DCE1TOP
 * 
 * Revision 9.5.2.2  92/08/05  11:27:55  blurie
 * 	Merge our changes into OSF files.
 * 
 * Revision 9.5.1.1  92/07/24  18:59:45  hinman
 * 	Create merge branch
 * 
 * Revision 9.5  92/07/24  18:59:43  hinman
 * 	Update checkin
 * 
 * Revision 1.1.3.2  1992/05/22  20:33:17  bolinger
 * 	Workaround (not real fix) for OT defect 2281:  Increase length of test
 * 	on RIOS ref platform.  Whether the test succeeds depends on some
 * 	complex interaction between the state (idleness) of the machine, the
 * 	contents of the test, and the number of iterations each thread in the
 * 	test runs.  We'll work on this further after 1.0.1 -- but for now, this
 * 	patch makes it run very consistently on the RIOS.  (There's never been
 * 	a problem on the PMAX.)
 * 	[1992/05/22  20:32:29  bolinger]
 * 
 * Revision 1.1  1992/01/19  03:22:18  devrcs
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
 *	PTHREAD Test Programs
 *
 *  FILENAME:
 *
 *	PRVB_THD_006
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
 *	001	Paul Curtin	5 September 1990
 *		Fixed test name.
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
 *	005	Paul Curtin	3 April 1991
 *		Added calls to _getscheduler, _getprio, and _setprio
 */

/*
 *  INCLUDE FILES
 */

#include <pthread.h>
#include <dce/cts_test.h>
#include <stdio.h>

#define thread_count	10
#define threshold	10
#define time_lapse	60
#undef	scale_lim
#define	delay_lim	100
#define print_all	1
#if _CMA_VENDOR_ == _CMA__HP
#define min_iterations 500000
#endif
#if	_CMA_PLATFORM_ == _CMA__IBMR2_UNIX
#define min_iterations	500000
#endif
#if _CMA_PLATFORM_ == _CMA__MIPS_UNIX
#define min_iterations	100000
#endif
#if _CMA_PLATFORM_ == _CMA__I386_UNIX

#define min_iterations	500000
#endif

#if _CMA_UNIX_TYPE == _CMA__SVR4
#define min_iterations	100000
#endif


unsigned int	start = 0, quit[thread_count], maybe_done = 0;
pthread_mutex_t	smutex, maybe_done_mutex, qmutex[thread_count];
pthread_cond_t	start_cv, maybe_done_cv;


static pthread_addr_t
thread	(
	unsigned int	number)
    {
    int 	count = 0;
    int		status;
    unsigned int	scale_ctr, delay_ctr;


    status = pthread_mutex_lock (&smutex);
    check(status,"pthread_mutex_lock");
    while (!start) {
	status = pthread_cond_wait (&start_cv, &smutex);
	check(status,"pthread_cond_wait");
	}
    status = pthread_mutex_unlock (&smutex);
    check(status,"pthread_mutex_unlock");

    while (++count > 0) 
#ifdef scale_lim
	for (scale_ctr = 0; scale_ctr < scale_lim; scale_ctr++) 
#endif
	{
	    status = pthread_mutex_lock (&qmutex[number]);
	    check(status,"pthread_mutex_lock");
	    if (quit[number]) {
		status = pthread_mutex_unlock (&qmutex[number]);
		check(status,"pthread_mutex_unlock");
		return (pthread_addr_t)count;
		}
	    status = pthread_mutex_unlock (&qmutex[number]);
	    check(status,"pthread_mutex_unlock");

	    if (count == min_iterations) {
		pthread_mutex_lock (&maybe_done_mutex);
		maybe_done = 1;
		pthread_mutex_unlock (&maybe_done_mutex);
		pthread_cond_signal (&maybe_done_cv);
		}

#ifdef delay_lim
	    for (delay_ctr = 0; delay_ctr < delay_lim; delay_ctr++);
#endif
	}

    printf ("Thread #%d overflowed.\n", number);
    pthread_exit((pthread_addr_t) &number);	/* Counter overflow */

    return (pthread_addr_t)count; /* bogus */
    }

main ()
    {
    pthread_t		threads[thread_count];
    pthread_attr_t	attr;
    unsigned int	i, dif, dev;
    unsigned int	max = 0, min = -1, ave = 0;
    pthread_addr_t	result;
    int			status;
    int			exit;
    int			sched_pol;
    int			prio;
    unsigned int	begin, end;


    cts_test ("PRVB_THD_006", "Test for timeslicing fairness.");

    status = pthread_setscheduler (pthread_self (), SCHED_FIFO, PRI_FIFO_MAX);
    check (status, "pthread_setscheduler");
    sched_pol = pthread_getscheduler (pthread_self ());
    check (sched_pol, "pthread_getscheduler");
    if (sched_pol != SCHED_FIFO)
	cts_failed ("pthread_getscheduler returned wrong policy.");
    prio = pthread_getprio(pthread_self ());
    check (sched_pol, "pthread_getprio");
    if (prio != PRI_FIFO_MAX)
	cts_failed ("pthread_getprio didn't get expected value.");
    status = pthread_setprio(pthread_self (), PRI_FIFO_MAX);
    check (status, "pthread_setprio");
    status = pthread_attr_create (&attr);
    check (status, "pthread_attr_create");
    status = pthread_attr_setinheritsched (&attr, PTHREAD_DEFAULT_SCHED);
    check (status, "pthread_attr_setinheritsched");
    status = pthread_mutex_init (&smutex, pthread_mutexattr_default);
    check(status,"pthread_mutex_init");
    status = pthread_mutex_init (&maybe_done_mutex, pthread_mutexattr_default);
    check(status,"pthread_mutex_init");
    status = pthread_cond_init (&maybe_done_cv, pthread_condattr_default);
    check(status,"pthread_cond_init");

    for (i = 0; i < thread_count; i++) {
	quit[i] = 0;
	status = pthread_mutex_init (&qmutex[i], pthread_mutexattr_default);
	check(status,"pthread_mutex_init");
	}

    status = pthread_cond_init  (&start_cv, pthread_condattr_default);
    check(status,"pthread_cond_init");

    for (i = 0; i < thread_count; i++)
	status = pthread_create (
		&threads[i],
		attr,
		thread,
		(pthread_addr_t)i);
	check(status,"pthread_create");

    fprintf (stderr, "Starting...");
    status = pthread_mutex_lock (&smutex);
    check(status,"pthread_mutex_loc");
    start = 1;
    status = pthread_mutex_unlock (&smutex);
    check(status,"pthread_mutex_unlock");

    status = pthread_cond_broadcast (&start_cv);
    check(status,"pthread_cond_broadcast");
    begin = time(0);

    pthread_mutex_lock (&maybe_done_mutex);

    while (!maybe_done)
	pthread_cond_wait (&maybe_done_cv, &maybe_done_mutex);

    pthread_mutex_unlock (&maybe_done_mutex);

    for (i = 0; i < thread_count; i++) {
	status = pthread_mutex_lock (&qmutex[i]);
	check(status,"pthread_mutex_lock");
	quit[i] = 1;
	status = pthread_mutex_unlock (&qmutex[i]);
	check(status,"pthread_mutex_unlock");
	}

    end = time(0);
    fprintf (stderr, "...done.\n\n");

#ifdef print_all
    printf ("     Thread\t     Iterations\n");
#endif
    for (i = 0; i < thread_count; i++) {
	exit = pthread_join (
		threads[i],
		&result);

	if (exit == -1) {
	    cts_failed("Thread error.");
	    printf ("Thread #%d overflowed.\n", i);
	    }
#ifdef print_all
	else
	    printf ("\t%d\t\t%06d\n", i, (unsigned int)result);
#endif

        if (min > (unsigned int)result)   min = (unsigned int)result;
        if (max < (unsigned int)result)   max = (unsigned int)result;
	ave += ((unsigned int)result)/thread_count;

	status = pthread_detach (&threads[i]);
	check(status,"pthread_detach");
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
