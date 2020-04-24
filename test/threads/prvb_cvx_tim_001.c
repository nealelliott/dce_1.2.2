/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: prvb_cvx_tim_001.c,v $
 * Revision 1.1.55.1  1996/06/04  22:15:05  arvind
 * 	Get time before pthread_get_expiration_np() call
 * 	[1996/05/09  18:37 UTC  bissen  /main/DCE_1.2/bissen_122_work/1]
 *
 * Revision 1.1.53.2  1996/02/17  23:31:52  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:48:19  marty]
 * 
 * Revision 1.1.53.1  1995/12/11  23:03:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:34  root]
 * 
 * Revision 1.1.51.1  1994/04/01  20:12:01  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:19:29  jd]
 * 
 * Revision 1.1.49.1  1993/10/12  19:37:47  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:31:07  rhw]
 * 
 * Revision 1.1.3.3  1992/12/30  16:54:38  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:12:47  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/03  14:33:13  bolinger
 * 	Move current 1.0.1-backed file (modified to work with new
 * 	threads source base) from private branch into 1.0.2 tree.
 * 	(See OT defect 5300.)
 * 	[1992/09/02  13:46:02  bolinger]
 * 
 * Revision 1.1.1.2  1992/08/11  21:46:23  bolinger
 * 	Replace uses of struct timeb/ftime(3) with refs to new timing
 * 	interface specific to these tests (which is designed to hide
 * 	any further changes to the underlying system facilities used).
 * 
 * Revision 1.1  1992/01/19  03:22:07  devrcs
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
 *	PRVB_CVX_TIM_001
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
 *	001	Paul Curtin	30 May 1990
 *		Converted test to use pthread interface.
 *	002	Paul Curtin	15 June 1990
 *		Added a pthread_addr_t cast to join arg.
 *	003	Dave Butenhof	29 June 1990
 *		Fix pthread_get_expiration_np to new definition.
 *	004	Dave Butenhof	07 August 1990
 *		Fix checks for timeout on timedwait (EAGAIN is in errno, not
 *		in return value of function). Update cts_failed calls to use
 *		new stdargs interface for formatting instead of calling
 *		sprintf.
 *	005	Dave Butenhof	11 October 1990
 *		Use cma_errno.h
 */

/*
 *  INCLUDE FILES
 */

#include <pthread.h>
#include <cma_errno.h>
#include <dce/cts_test.h>
#include <stdio.h>
#ifdef vms
#include <timeb.h>
#endif

pthread_cond_t	cv;
pthread_mutex_t	    mutex;
int	variable;

static unsigned int test_it  (int,float *);

static float dif_secs  (cts_timebuf_t *,cts_timebuf_t *);

static pthread_addr_t do_signal (int);

main ()
    {
    float		wait;
    int			status;
    unsigned int	timed_out;


    cts_test ("PRVB_CVX_TIM_001", "Test pthread_cond_timedwait with timeout and signals");

    status = pthread_cond_init (&cv, pthread_condattr_default);
    check(status,"pthread_cond_init");
    status = pthread_mutex_init (&mutex, pthread_mutexattr_default);
    check(status,"pthread_mutex_init");

    cts_comment ("Part 1: signal after timeout interval");

    if (! test_it (15, &wait))
	cts_failed ("Wait didn't time out.");

    if (wait < 10.0 || wait > 14.0)
	cts_failed (
		"Expected 10 second interval: actual was %f seconds",
		wait);

    cts_comment ("Part 2: signal before timeout interval");

    if (test_it (5, &wait))
	cts_failed ("Wait timed out.");

    if (wait < 5.0 || wait > 9.0)
	cts_failed (
		"Expected 5 second interval: actual was %f seconds",
		wait);

    cts_result ();
    }

static unsigned int
test_it	(
	int	delay,
	float	*wait)
    {
    pthread_t		thread;
    int			status;    
    struct timespec	delta, expiration;
    cts_timebuf_t		time1, time2;
    float		result;
    int			exit;
    int			cond_status, timeout;


    timeout = 0;
    status = pthread_mutex_lock (&mutex);
    check(status,"pthread_mutex_lock");
    variable = 0;
    status = pthread_create (
	    &thread, 
	    pthread_attr_default, 
	    do_signal, 
	    (pthread_addr_t)delay);
    check(status,"pthread_create");
    delta.tv_sec = 10;
    delta.tv_nsec = 0;

    cts_gettime (&time1);

    status = pthread_get_expiration_np (&delta, &expiration);
    check(status,"pthread_get_expiration_np");

    while (variable == 0) {
	    
	    cond_status = pthread_cond_timedwait (
		    &cv,
		    &mutex,
		    &expiration);

	    if (cond_status == -1 && errno == EAGAIN) {
		timeout = !0;
		break;
		}

	    }

    cts_gettime (&time2);

    status = pthread_mutex_unlock (&mutex);  
    check(status,"pthread_mutex_unlock");
    *wait = dif_secs (&time1, &time2);
    status = pthread_join (thread, (pthread_addr_t *)&result);
    check(status,"pthread_join");
    status = pthread_detach (&thread);
    check(status,"pthread_detach");
    
    return timeout;
    }

static float
dif_secs	(
	cts_timebuf_t	*t1,
	cts_timebuf_t	*t2)
    {
	return (cts_timediff(t1, t2));
    }

static pthread_addr_t
do_signal	(
	int	delay)
    {
    struct timespec ddelay;
    int	    status;

    ddelay.tv_sec = delay;
    ddelay.tv_nsec = 0;
    pthread_delay_np (&ddelay);
    status = pthread_mutex_lock (&mutex);
    check(status,"pthread_mutex_lock");
    variable = 1;
    status = pthread_mutex_unlock (&mutex);
    check(status,"pthread_mutex_unlock");
    pthread_cond_signal (&cv);
    return (pthread_addr_t)0;
    }
