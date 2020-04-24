/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: prvb_mut_001.c,v $
 * Revision 1.1.37.2  1996/02/17  23:31:56  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:48:23  marty]
 *
 * Revision 1.1.37.1  1995/12/11  23:03:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:37  root]
 * 
 * Revision 1.1.35.1  1994/04/01  20:12:09  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:19:47  jd]
 * 
 * Revision 1.1.33.1  1993/10/12  19:37:54  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:31:10  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:54:56  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:13:01  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:22:11  devrcs
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
 *	PRVB_MUT_001
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
 */

/*
 *  INCLUDE FILES
 */

#include <pthread.h>
#include <dce/cts_test.h>


#define MAX_THREAD_CNT	1000
#define THREAD_COUNT	10
#define ITERATIONS	20000
#define USAGE		"Usage:  %s [thread_count [iterations] ]\n"


pthread_mutex_t		the_mutex;
int			thread_count = THREAD_COUNT;
int			iterations = ITERATIONS;


static pthread_addr_t thread  (int);


main	(
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

    cts_test ("PRVB_MUT_001", "This test bangs on mutexes from several threads");

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
			thread,
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
thread	(
	int	number)
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
