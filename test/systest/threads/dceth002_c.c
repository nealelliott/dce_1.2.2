/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dceth002_c.c,v $
 * Revision 1.1.17.2  1996/02/17  23:29:07  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:45:53  marty]
 *
 * Revision 1.1.17.1  1995/12/11  22:49:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:23:11  root]
 * 
 * Revision 1.1.13.1  1994/04/01  20:08:25  jd
 * 	First drop clode cleanup
 * 	[1994/03/29  14:20:44  jd]
 * 
 * Revision 1.1.11.2  1993/07/28  19:36:54  cmckeen
 * 	HP's TET'ized version of the system tests
 * 	[1993/07/28  13:27:44  cmckeen]
 * 
 * Revision 1.1.7.3  1993/05/27  18:56:27  eheller
 * 	*** empty log message ***
 * 
 * Revision 1.1.7.2  1993/05/27  14:09:03  eheller
 * 	*** empty log message ***
 * 	[1993/05/26  17:58:19  eheller]
 * 
 * Revision 1.1.4.2  1993/04/23  13:52:45  eheller
 * 	Cleaned up warnings
 * 	[1993/04/23  13:29:00  eheller]
 * 
 * 	Embedded copyright notice
 * 	[1993/02/04  22:18:49  cjd]
 * 
 * Revision 1.1.1.7  1993/04/21  20:28:49  root
 * 	    Loading drop DCE1_0_2B23
 * 
 * Revision 1.1.5.2  1993/02/05  15:26:28  cjd
 * 	Embedded copyright notice
 * 	[1993/02/05  14:47:43  cjd]
 * 
 * Revision 1.1.2.2  1992/06/07  15:46:07  ramb
 * 	Changed code to obtain the thread_count from arg1 to program instead of getting
 * 	from the environment variable NUMBER_OF_THREADS.
 * 	[1992/04/06  14:55:55  ramb]
 * 
 * 	Created for systest from PRVB_THD_002.C
 * 	Program modified for systest to take input from environment variable.
 * 	[1992/04/02  18:14:35  ramb]
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
 *	DCETH002_C - Adapted from PRVB_THD_002
 *
 *  TEST TOPICS:
 *
 *	THD  -  Thread Operations
 *
 *  ABSTRACT:
 *
 *	Create a few threads which loop and compute to
 *	test time slicing.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	01 November 1989
 *
 *  MODIFICATION HISTORY:
 *  001	    Paul Curtin	    31 May 1990
 *	    Converted test to use pthread interface.
 *  002	    Paul Curtin	    5 September 1990
 *	    Fixed test name.
 *  003     Ram Balaraman   1 April 1992
 *          Modified code for system testing purposes.
 *          Filename changed to keep independent copy.
 *          Added code to get the number of threads from environment
 *          variable NUMBER_OF_THREADS. If variable does not exist or has
 *          an invalid value assume default specified by 'thread_count_default'
 *          Changed code to dynamically allocate thread array instead of
 *          statically allocating it.
 *  004     Ram Balaraman   6 April 1992
 *          Changed code to take the NUMBER_OF_THREADS as arg1 to program and
 *          not depend on environment variable NUMBER_OF_THREADS. The rest
 *          the logic remains the same (ie. if no arguments are specified
 *          default to 'thread_count_default' variable.
 */

/*
 *  INCLUDE FILES
 */

#include <pthread.h>
#include <cts_test.h>
#include <limits.h>

#define thread_count_default 	101

static pthread_mutex_t	print_mutex;

static pthread_addr_t
thread ( int);

static void
say ( int, int, int);


/* 
 * redefine pthread_equal from pthread.h to use -> instead of . -Ram Balaraman
 */
#undef pthread_equal
#define pthread_equal(thread1,thread2) \
    (((pthread_t *)(thread1)->field1 == (pthread_t *)(thread2)->field1) \
    && ((pthread_t *)(thread1)->field2 == (pthread_t *)(thread2)->field2))


main
        (
	int argc,
	char *argv[])
{
    pthread_t   *threads_p;
    int thread_count;
    int	i;
    int status;
    pthread_addr_t	result;
    int	exit;

    thread_count = 0;
    exit = 0;
    if (argc > 1) thread_count = atol (argv[1]);
    if (thread_count < 1) thread_count = thread_count_default;

    printf("DCETH002_C status: Testing with %d threads... \n",thread_count);
    threads_p = malloc(sizeof(pthread_t)*thread_count);
    if (threads_p == NULL)
        cts_failed ("Unable to allocate memory for threads");
     
    cts_test ("DCETH002_C", "Test time slicing");

    status = pthread_mutex_init (&print_mutex, pthread_mutexattr_default);
    check(status,"pthread_mutex_init");

    for (i = 1; i < thread_count; i++)
	status = pthread_create (
				 (pthread_t *)(threads_p+i),
				 pthread_attr_default,
				 (pthread_startroutine_t)thread,
				 (pthread_addr_t)i);
	check(status,"pthread_create");

    if (pthread_equal(threads_p+0,threads_p+1))
	cts_failed ("Two thread handles are identical.");

    say (0, (int)thread (0), 1);

    for (i = 1; i < thread_count; i++) {
	status = pthread_join (
			*(threads_p+i),
			&result);
	check(status,"pthread_join");
		    
	say (i, (int)result, (int)exit);
	status = pthread_detach (threads_p+i);
	check(status,"pthread_detach");
	}

    cts_result ();

  }

static pthread_addr_t
thread
	(
	int	number)
    {
#define interval	20000
    int	sum, counter;


    counter = interval;

    for (sum = 0; sum < 100000; sum++)
	if (--counter == 0) {
	    counter = interval;
	    say (number, sum, 1);
	    }

    return (pthread_addr_t)sum;
    }

static void
say
	(
	int	thread,
	int	count,
	int	status)
    {
    int astatus;

    astatus = pthread_mutex_lock (&print_mutex);
    check(status,"pthread_mutex_lock");
    printf ("DCETH002_C status: thread %d, count %d (%d)\n", thread, count, status);
    astatus = pthread_mutex_unlock (&print_mutex);
    check(status,"pthread_mutex_unlock");
    }














































