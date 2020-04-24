/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dcethmut_c.c,v $
 * Revision 1.1.15.2  1996/02/17  23:29:08  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:45:54  marty]
 *
 * Revision 1.1.15.1  1995/12/11  22:49:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:23:14  root]
 * 
 * Revision 1.1.13.1  1994/04/01  20:08:26  jd
 * 	First drop clode cleanup
 * 	[1994/03/29  14:20:45  jd]
 * 
 * Revision 1.1.11.2  1993/07/28  19:37:08  cmckeen
 * 	HP's TET'ized version of the system tests
 * 	[1993/07/28  13:28:06  cmckeen]
 * 
 * Revision 1.1.7.3  1993/05/27  18:56:50  eheller
 * 	*** empty log message ***
 * 
 * Revision 1.1.7.2  1993/05/27  14:09:50  eheller
 * 	*** empty log message ***
 * 	[1993/05/26  18:19:14  eheller]
 * 
 * Revision 1.1.4.2  1993/04/23  13:53:00  eheller
 * 	Cleaned up warnings
 * 	[1993/04/23  13:41:16  eheller]
 * 
 * 	Embedded copyright notice
 * 	[1993/02/04  22:19:02  cjd]
 * 
 * Revision 1.1.1.7  1993/04/21  20:28:53  root
 * 	    Loading drop DCE1_0_2B23
 * 
 * Revision 1.1.5.2  1993/02/05  15:26:39  cjd
 * 	Embedded copyright notice
 * 	[1993/02/05  14:47:52  cjd]
 * 
 * Revision 1.1.2.2  1992/06/07  15:47:19  ramb
 * 	Created for systest from PRVB_MUT_001.C
 * 	Bumped up max thread count also.
 * 	[1992/04/02  18:16:05  ramb]
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
 *	DCETHMUT_C - Adapted from PRVB_MUT_001
 *
 *  TEST TOPICS:
 *
 *	MUT  -  Mutex Operations
 *
 *  ABSTRACT:
 *
 *	This test creates a number of threads and then
 *	has each one lock and unlock the same mutex
 *	repeatedly.  The intent is to detect any flaws
 *	that is, races, in mutexes, time-slicing, or the
 *	scheduler.
 *
 *  AUTHORS:
 *
 *	Bob Conti
 *
 *  CREATION DATE:
 *
 *	06 November 1989
 *
 *  MODIFICATION HISTORY:
 *	001	Paul Curtin	31 May 1990
 *		Converted test to use pthread interface.
 *      002     Webb Scales     5 April 1991
 *              Set up for command line arguments for OSF auto-testing.
 *      003     Ram Balaraman   1 April 1992
 *              Modified code for system testing purposes.
 *              Filename changed to keep independent copy.
 *              Increased the max value of thread count to 10000

 */

/*
 *  INCLUDE FILES
 */

#include <pthread.h>
#include <cts_test.h>


#define MAX_THREAD_CNT	10000
#define THREAD_COUNT	10
#define ITERATIONS	20000
#define USAGE		"Usage:  %s [thread_count [iterations] ]\n"


pthread_mutex_t		the_mutex;
int			thread_count = THREAD_COUNT;
int			iterations = ITERATIONS;


static pthread_addr_t
thread (int);


main (
	int argc, 
	char *argv[])
    {
    pthread_t	threads[MAX_THREAD_CNT];
    int	i;
    int status;
    pthread_addr_t	result;


    if (argc > 1)  thread_count = atol (argv[1]);
    if (argc > 2)  iterations = atol (argv[2]);

    if ((argc > 1) && (*argv[1] == '-')) {
	printf (USAGE, argv[0]);
	exit (-1);
	}

    if (thread_count > MAX_THREAD_CNT) {
	printf ("Thread count is too large (maximum is %d).\n", MAX_THREAD_CNT);
	exit (-1);
	}

    cts_test ("DCETHMUT_C", "This test bangs on mutexes from several threads");

    status = pthread_mutex_init (&the_mutex, pthread_mutexattr_default);
    check(status,"pthread_mutex_init");

    /*
     * Fork a bunch of threads to perform "simultaneous" testing of
     * mutexes.  The number of threads can be set by changing the
     * constant "thread_count".
     */
    for (i = 1; i < thread_count; i++) {
	status = pthread_create (
			&threads[i],
			pthread_attr_default,
			(pthread_startroutine_t)thread,
			(pthread_addr_t)i);
	check(status,"pthread_create");
	pthread_yield ();
	}

    thread (0);				/* Do stuff for main thread */

    /*
     * Now join with the threads to be sure they've all cleaned up and gone
     * away.  The return value and status aren't used.
     */
    for (i = 1; i < thread_count; i++) {
	status = pthread_join (
			threads[i],
			&result);
	check(status,"pthread_join");
	pthread_detach (&threads[i]);
	}

    /*
     * For auto-testing check
     */
    exit (cts_result ());
    }


static pthread_addr_t
thread ( int	number)
    {
    pthread_t	self;
    int	status;
    int	i;

    for (i = 1; i <= iterations; i++) {

	status = pthread_mutex_lock (&the_mutex);
	check(status,"pthread_mutex_lock");
	/* 
	 * Every time each thread does 2000 interations, print
	 * a progress report.
	 */
	if (i%2000 == 0) {
	    printf ("At iteration %d in thread %d\n", i, number);
	    }
	/* 
	 * Every 11 times, yield with the mutex locked.
	 */
	if ((i+number)%11 == 0) pthread_yield ();
	status = pthread_mutex_unlock (&the_mutex);
	check(status,"pthread_mutex_unlock");
	/* 
	 * Every 23 times, yield with the mutex unlocked.
	 */
	if ((i+number)%23 == 0) pthread_yield ();
        }
    }


