/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: prvb_can_001.c,v $
 * Revision 1.1.38.2  1996/02/17  23:31:47  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:48:15  marty]
 *
 * Revision 1.1.38.1  1995/12/11  23:03:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:30  root]
 * 
 * Revision 1.1.36.1  1994/04/01  20:11:51  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:19:06  jd]
 * 
 * Revision 1.1.34.1  1993/10/12  19:37:39  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:31:03  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:54:20  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:12:28  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:22:01  devrcs
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
 *	PTHREAD Test Programs
 *
 *  FILENAME:
 *
 *	RV_CAN_001
 *
 *  TEST TOPICS:
 *
 *	CAN  -  Cancellation
 *
 *  ABSTRACT:
 *
 *	Attempt to cancel threads with and without cleanup handlers,
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
 * 	001	Paul Curtin	29 May 1990
 *		Converted test to use pthreads interface.
 * 	002	Paul Curtin	15 June 1990
 *		Got rid of void * so as to work on Mips.
 *	003	Dave Butenhof	29 June 1990
 *		Fix pthread_get_expiration_np to new definition.
 *	004	Dave Butenhof	11 October 1990
 *		Use cma_errno.h
 *	005	Dave Butenhof	7 February 1991
 *		Cleanup: change alert terminology to cancellation.
 */

/*
 *  INCLUDE FILES
 */

#include <pthread.h>
#include <cma_errno.h>
#include <dce/cts_test.h>

pthread_t	thread;
pthread_mutex_t	mutex;
pthread_cond_t	cv;
unsigned int	variable;

static void compute_bound  (pthread_addr_t);

static void delayer  (pthread_addr_t);

static void timed_waiter  (pthread_addr_t);

static void waiter  (pthread_addr_t);

static void suicide  (pthread_addr_t);

static void joiner  (pthread_addr_t);

static void xsignal  (pthread_addr_t);

static void dummy  (pthread_addr_t);

main ()
    {
    struct timespec delay;
    int	status;
    int join_status;
    pthread_addr_t	result;


    cts_test ("PRVB_CAN_001", "Basic test of cancellation");

    status = pthread_mutex_init (&mutex, pthread_mutexattr_default);
    check(status,"pthread_mutex_init");
    status = pthread_cond_init (&cv, pthread_condattr_default);
    check(status,"pthread_cond_init");

    cts_comment ("part 1: cancel compute-bound thread");
    status = pthread_create (
		&thread,
		pthread_attr_default,
		(pthread_startroutine_t)compute_bound,
		(pthread_addr_t)0);
    check(status,"pthread_create");
    delay.tv_sec = 2;
    delay.tv_nsec = 0;
    pthread_delay_np (&delay);			/* Let it get started */
    status = pthread_cancel (thread);
    check(status,"pthread_cancel");
    status = pthread_join (thread, (pthread_addr_t *)&join_status);
    check(status,"pthread_join");
    status = pthread_detach (&thread);
    check(status,"pthread_detach");

    if (join_status != -1)
	cts_failed ("didn't terminate with CANCELED exit status");

    cts_comment ("part 2: cancel waiting thread (pthread_delay_np)");
    status = pthread_create (
		&thread,
		pthread_attr_default,
		(pthread_startroutine_t)delayer,
		(pthread_addr_t)0);
    check(status,"pthread_create");
    pthread_delay_np (&delay);
    status = pthread_cancel (thread);
    check(status,"pthread_cancel");
    status = pthread_join (thread, (pthread_addr_t *)&join_status);
    check(status,"pthread_join");
    status = pthread_detach (&thread);
    check(status,"pthread_detach");
    
    if (join_status != -1)
	cts_failed ("didn't terminate with CANCELED exit status");

    cts_comment ("part 3: cancel waiting thread (pthread_cond_timedwait)");
    status = pthread_create (
		&thread,
		pthread_attr_default,
		(pthread_startroutine_t)timed_waiter,
		(pthread_addr_t)0);
    check(status,"pthread_create");
    pthread_delay_np (&delay);
    status = pthread_cancel (thread);
    check(status,"pthread_cancel");
    status = pthread_join (thread, (pthread_addr_t *)&join_status);
    check(status,"pthread_join");
    status = pthread_detach (&thread);
    check(status,"pthread_detach");

    if (join_status != -1)
	cts_failed ("didn't terminate with CANCELED exit status");

    cts_comment ("part 4: cancel waiting thread (pthread_cond_wait)");
    status = pthread_create (
		&thread,
		pthread_attr_default,
		(pthread_startroutine_t)waiter,
		(pthread_addr_t)0);
    pthread_delay_np (&delay);
    status =pthread_cancel (thread);
    check(status,"pthread_cancel");
    status = pthread_join (thread, (pthread_addr_t *)&join_status);
    check(status,"pthread_join");
    status = pthread_detach (&thread);
    check(status,"pthread_detach");

    if (join_status != -1)
	cts_failed ("didn't terminate with CANCELED exit status");

    cts_comment ("part 5: cancel self");
    status = pthread_create (
		&thread,
		pthread_attr_default,
		(pthread_startroutine_t)suicide,
		(pthread_addr_t)0);
    check(status,"pthread_create");
    status = pthread_join (thread, (pthread_addr_t *)&join_status);
    check(status,"pthread_join");
    status = pthread_detach (&thread);
    check(status,"pthread_detach");

    if (join_status != -1)
	cts_failed ("didn't terminate with CANCELED exit status");

    cts_comment ("part 6: cancel thread doing pthread_thread_join");
    status = pthread_create (
		&thread,
		pthread_attr_default,
		(pthread_startroutine_t)joiner,
		(pthread_addr_t)0);
    delay.tv_sec = 3;
    delay.tv_nsec = 0;
    check(status,"pthread_create");
    pthread_delay_np (&delay);
    status = pthread_cancel (thread);
    check(status,"pthread_cancel");
    status = pthread_join (thread, (pthread_addr_t *)&join_status);
    check(status,"pthread_join");
    status = pthread_detach (&thread);
    check(status,"pthread_detach");

    if (join_status != -1)
	cts_failed ("didn't terminate with CANCELED exit status");

    cts_result ();

    }

static void
compute_bound	(
	pthread_addr_t	arg)
    {
    int	cnt;
    int status;

    for (cnt = 0; cnt < 100000000; cnt++) {
	pthread_testcancel ();
	check(status,"pthread_testcancel");
	}

    cts_failed ("compute-bound loop completed without cancel");
    }

static void
delayer	(
	pthread_addr_t	arg)
    {
    struct timespec delay;


    delay.tv_sec = 10;
    delay.tv_nsec = 0;
    pthread_delay_np (&delay);
    cts_failed ("10 second delay completed without cancel");
    }


static void
clean1	(
	pthread_addr_t	arg)
    {
    int	status;

    status = pthread_mutex_unlock (&mutex);
    check(status,"pthread_mutex_unlock");
    }

static void
timed_waiter	(
	pthread_addr_t	arg)
    {
    struct timespec	delta, timeout;
    int			status;


    delta.tv_sec = 10;
    delta.tv_nsec = 0;
    status = pthread_get_expiration_np (&delta, &timeout);
    check(status,"pthread_get_expiration_np");
    status = pthread_mutex_lock (&mutex);
    check(status,"pthread_mutex_lock");

    pthread_cleanup_push(clean1, 0);
	while (pthread_cond_timedwait (&cv, &mutex, &timeout) != EAGAIN);
	cts_failed ("10 second wait elapsed without cancel");
    pthread_cleanup_pop(1);

    status = pthread_mutex_unlock (&mutex);
    check(status,"pthread_mutex_unlock");
    }


static void
clean2	(
	pthread_addr_t	arg)
    {
    int	status;
    pthread_t thd;

    thd = *((pthread_t *) arg);
    status = pthread_mutex_unlock (&mutex);
    check(status,"pthread_mutex_unlock");
    status = pthread_cancel (thd);
    check(status,"pthread_cancel");
    }


static void
waiter	(
	pthread_addr_t	arg)
    {
    int	status;
    pthread_addr_t	result;
    pthread_t	dt;


    status = pthread_create (
			    &dt,
			    pthread_attr_default,
			    (pthread_startroutine_t)xsignal,
			    (pthread_addr_t)0);
    check(status,"pthread_create");
    status = pthread_mutex_lock (&mutex);
    check(status,"pthread_mutex_lock");
    variable = (int) 0;

    pthread_cleanup_push(clean2, &dt);
	while (! variable) {
	    status = pthread_cond_wait (&cv, &mutex);
	    check(status,"pthread_cond_wait");
	    }
    pthread_cleanup_pop(1);

    status = pthread_mutex_unlock (&mutex);
    check(status,"pthread_mutex_unlock");
    status = pthread_detach (&dt);
    check(status,"pthread_detach");
    }

static void
suicide	(
	pthread_addr_t	arg)
    {
    pthread_t	self;
    int		status;


    self = pthread_self ();
    status = pthread_cancel (self);
    check(status,"pthread_cancel");
    pthread_testcancel ();
    check(status,"pthread_testcancel");
    cts_failed ("continued after canceling self");
    }


static void
clean3        (
        pthread_addr_t    arg)
    {
    int status;
    pthread_t thd;

    thd = *((pthread_t *) arg);
    status = pthread_cancel (thd);
    check(status,"pthread_cancel");
    }
 

static void
joiner	(
	pthread_addr_t	arg)
    {
    int	status;
    int join_status;
    pthread_addr_t	result;
    pthread_t	dt;


    status = pthread_create (
			    &dt,
			    pthread_attr_default,
			    (pthread_startroutine_t)dummy,
			    (pthread_addr_t)0);
    check(status,"pthread_create");

    pthread_cleanup_push(clean3, &dt);
	status = pthread_join (dt, (pthread_addr_t *)&join_status);
	check(status,"pthread_join");
    pthread_cleanup_pop(1);

    status = pthread_detach (&dt);
    check(status,"pthread_detach");
    cts_failed ("pthread_join did not cancel");
    }

static void
xsignal	(
	pthread_addr_t	arg)
    {
    int status;
    struct timespec delay;


    delay.tv_sec = 10;
    delay.tv_nsec = 0;
    pthread_delay_np (&delay);
    status = pthread_mutex_lock (&mutex);
    check(status,"pthread_mutex_lock");
    variable = (int) 1;
    status = pthread_mutex_unlock (&mutex);
    check(status,"pthread_mutex_unlock");
    status = pthread_cond_signal (&cv);
    check(status,"pthread_cond_signal");
    }

static void
dummy	(
	pthread_addr_t	arg)
    {
    struct timespec delay;


    delay.tv_sec = 20;
    delay.tv_nsec = 0;
    pthread_delay_np (&delay);
    }
