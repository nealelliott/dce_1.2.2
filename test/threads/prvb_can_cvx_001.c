/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: prvb_can_cvx_001.c,v $
 * Revision 1.1.41.2  1996/02/17  23:31:50  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:48:17  marty]
 *
 * Revision 1.1.41.1  1995/12/11  23:03:32  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:32  root]
 * 
 * Revision 1.1.39.1  1994/04/01  20:11:57  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:19:20  jd]
 * 
 * Revision 1.1.37.1  1993/10/12  19:37:45  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:31:06  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:54:31  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:12:40  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:22:04  devrcs
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
 *	PRVB_CAN_CVX_001
 *
 *  TEST TOPICS:
 *
 *	CAN  -  Cancel
 *	CVX  -  Condition Variable Operations
 *
 *  ABSTRACT:
 *
 *	Verify that synchronous alerts work; that cancellation occurs
 *	during pthread_testcancel and pthread_cond_wait.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	27 October 1989
 *
 *  MODIFICATION HISTORY:
 *	001	Paul Curtin	30 May 1990
 *		Converted test to use the pthread interface.
 *	002	Paul Curtin	15 June 1990
 *		Added a pthread_addr_t to suit the mips cc.
 *	003	Webb Scales	17 July 1990
 *		Incorporated Apollo BL3 changes & fixed filename references.
 *	004	Dave Butenhof	7 February 1991
 *		Clean up terminology (alert => cancel)
 *	005	Dave Butenhof	29 May 1991
 *		Clean up the test (removed explicitly locked printfs, for
 *		example; replace with cts_comment() calls).
 */

/*
 *  INCLUDE FILES
 */

#include <pthread.h>
#include <dce/cts_test.h>

typedef struct THDINFO {
    int	got_cancelled;
    int	thread;
    } thdinfo;

pthread_mutex_t mutex;
pthread_cond_t	cv;
pthread_t	thread[3];
int	threadcount;

static unsigned int wait_cv  (int);

static unsigned int adelay  (int);

main ()
    {
    struct timespec delay;
    int	join_status;
    int status;
    unsigned int	result;
    int	count;
  

    cts_test ("PRVB_CAN_CVX_001", "Test general cancellation");

    status = pthread_mutex_init (&mutex, pthread_mutexattr_default);
    check(status,"pthread_mutex_init");
    count = 0;
    status = pthread_cond_init (&cv, pthread_condattr_default);
    check(status,"pthread_cond_init");
    status = pthread_create (
		    &thread[0],
		    pthread_attr_default,
		    (pthread_startroutine_t)wait_cv,
		    (pthread_addr_t)0);
    check(status,"pthread_create");
    status = pthread_create (
		    &thread[1],
		    pthread_attr_default,
		    (pthread_startroutine_t)wait_cv,
		    (pthread_addr_t)1);
    check(status,"pthread_create");
    status = pthread_create (
		    &thread[2],
		    pthread_attr_default,
		    (pthread_startroutine_t)adelay,
		    (pthread_addr_t)2);
    check(status,"pthread_create");
    delay.tv_sec = 5;
    delay.tv_nsec = 0;
    pthread_delay_np (&delay);			/* Wait 5 seconds to let them wait */
    delay.tv_sec = 2;
    delay.tv_nsec = 0;
    status = pthread_cancel (thread[0]);	/* Cancel thread 1 */
    check(status,"pthread_cancel");
    pthread_delay_np (&delay);
    status = pthread_cancel (thread[1]);
    check(status,"pthread_cancel");
    pthread_delay_np (&delay);
    status = pthread_cancel (thread[2]);
    check(status,"pthread_cancel");
    pthread_delay_np (&delay);

    for (count = 0; count < 3; count++) {
	status = pthread_join (thread[count], (pthread_addr_t *)&join_status);
	check(status,"pthread_join");
	status = pthread_detach (&thread[count]);
	check(status,"pthread_detach");
	cts_comment (
		"Thread %d termination: (%d)",
		count,
		join_status);
	}

    if (threadcount != 0)
	cts_failed ("Threads did not terminate properly");

    cts_result ();

    }

static void
clean1
	(
	pthread_addr_t	arg)
    {
    int status;
    thdinfo	*ti = (thdinfo *)arg;


    ti->got_cancelled = 1;
    cts_comment ("Cancel received on thread %d", ti->thread);
    threadcount--;
    status = pthread_mutex_unlock (&mutex);
    check(status,"pthread_mutex_unlock");
    }

static unsigned int
wait_cv
	(
	int	arg)
    {
    thdinfo	inf;
    int status;


    inf.got_cancelled = 0;
    inf.thread = arg;
    status = pthread_mutex_lock (&mutex);
    check(status,"pthread_mutex_lock");
    threadcount++;

    pthread_cleanup_push(clean1, &inf); 
    while (1) {
	status = pthread_cond_wait (&cv, &mutex);
	check(status,"pthread_cond_wait");

	/*
	 * Because of the "check" above, the following conditional is never
	 * true; it's only here to prevent some C compilers from complaining
	 * about "unreachable statements" in pthread_cleanup_pop, due to the
	 * "while(1)" loop.  The unreachable statements are just fine, since
	 * we intend to exit this by a cancel.
	 */
	if (status == -1)
	    break;

	cts_comment ("Spurious wakeup on thread %d", inf.thread);
	}
    pthread_cleanup_pop(0); 

    status = pthread_mutex_unlock (&mutex);
    check(status,"pthread_mutex_unlock");

    return inf.got_cancelled;
    }

static void
clean2(
	pthread_addr_t	arg)
    {
    int status;
    thdinfo	*ti = (thdinfo *)arg;


    ti->got_cancelled = 1;
    cts_comment ("Cancel received on thread %d", ti->thread);
    status = pthread_mutex_lock (&mutex);
    check(status,"pthread_mutex_lock");
    threadcount--;
    status = pthread_mutex_unlock (&mutex);
    check(status,"pthread_mutex_unlock");
    }

static unsigned int
adelay(
	int	arg)
    {
    struct timespec delay;
    unsigned int	cancel_occurred = (int) 0;
    int status;
    thdinfo	inf;


    inf.got_cancelled = 0;
    inf.thread = arg;
    status = pthread_mutex_lock (&mutex);
    check(status,"pthread_mutex_lock");
    threadcount++;
    status = pthread_mutex_unlock (&mutex);
    check(status,"pthread_mutex_unlock");

    pthread_cleanup_push(clean2, &inf);
    delay.tv_sec = 60;
    delay.tv_nsec = 0;
    pthread_delay_np (&delay);
    pthread_cleanup_pop(0);

    return inf.got_cancelled;
    }
