/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: prvb_alt_cvx_001.c,v $
 * Revision 1.1.35.2  1996/02/17  23:31:43  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:48:11  marty]
 *
 * Revision 1.1.35.1  1995/12/11  23:03:11  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:26  root]
 * 
 * Revision 1.1.33.1  1994/04/01  20:11:48  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:18:57  jd]
 * 
 * Revision 1.1.31.1  1993/10/12  19:37:32  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:59  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:54:03  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:12:12  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:19:38  devrcs
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
 *	PRVB_ALT_CVX_001
 *
 *  TEST TOPICS:
 *
 *	ALT  -  Alerts
 *	CVX  -  Condition Variable Operations
 *
 *  ABSTRACT:
 *
 *	Verify that synchronous alerts work; that the CMA_S_ALERT
 *	exception is raised by cma_test_alert and cma_wait.
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
 */

/*
 *  INCLUDE FILES
 */

#include <pthread.h>
#include <dce/cts_test.h>

pthread_mutex_t mutex, printf_lock;
pthread_cond_t	cv;
pthread_t	thread[3];
int	threadcount;

static unsigned int wait_cv  (int);

static unsigned int adelay  (int);

static void lock_printf  (char *,int,int,int);

main ()
    {
    struct timespec delay;
    int	join_status;
    int status;
    unsigned int	result;
    int	count;
  

    cts_test ("PRVB_ALT_CVX_001", "Test synchronous alerts");

    status = pthread_mutex_init (&mutex, pthread_mutexattr_default);
    check(status,"pthread_mutex_init");
    status = pthread_mutex_init (&printf_lock, pthread_mutexattr_default);
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
    status = pthread_cancel (thread[0]);	/* Alert thread 1 */
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
	lock_printf (
		"Thread %d termination: (%d, %d)\n",
		count,
		join_status,
		result);
	}

    if (threadcount != 0)
	cts_failed ("Threads did not terminate properly");

    cts_result ();

    }

static void
clean1	(
	pthread_addr_t	arg)
    {
    int status;

    *(int*)arg = 1;
    lock_printf ("Alert received on a thread %d\n", 0, 0, 0);
    threadcount--;
    status = pthread_mutex_unlock (&mutex);
    check(status,"pthread_mutex_unlock");
    }

static unsigned int
wait_cv	(
	int	arg)
    {
    unsigned int	alert_occurred = (int) 0;
    int status;

    status = pthread_mutex_lock (&mutex);
    check(status,"pthread_mutex_lock");
    threadcount++;

    pthread_cleanup_push(clean1, &alert_occurred); 
	while (1) {
	    status = pthread_cond_wait (&cv, &mutex);
	    check(status,"pthread_cond_wait");

	    /*
	     * Because of the "check" above, the following conditional is
	     * never true; it's only here to prevent some C compilers from
	     * complaining about "unreachable statements" in
	     * pthread_cleanup_pop, due to the "while(1)" loop.  The
	     * unreachable statements are just fine, since we intend to exit
	     * this by a cancel.
	     */
	    if (status == -1)
		break;

	    lock_printf ("Spurious wakeup on thread %d\n", 0, 0, 0);
	    }
    pthread_cleanup_pop(0); 

    status = pthread_mutex_unlock (&mutex);
    check(status,"pthread_mutex_unlock");

    return alert_occurred;
    }

static void
clean2	(
	pthread_addr_t	arg)
    {
    int status;

    *(int*)arg = 1;
    lock_printf ("Alert received on thread %d\n", 0, 0, 0);
    status = pthread_mutex_lock (&mutex);
    check(status,"pthread_mutex_lock");
    threadcount--;
    status = pthread_mutex_unlock (&mutex);
    check(status,"pthread_mutex_unlock");
    }

static unsigned int
adelay	(
	int	arg)
    {
    struct timespec delay;
    unsigned int	alert_occurred = (int) 0;
    int status;

    status = pthread_mutex_lock (&mutex);
    check(status,"pthread_mutex_lock");
    threadcount++;
    status = pthread_mutex_unlock (&mutex);
    check(status,"pthread_mutex_unlock");

    pthread_cleanup_push(clean2, &alert_occurred);
	delay.tv_sec = 10;
	delay.tv_nsec = 0;
	pthread_delay_np (&delay);
    pthread_cleanup_pop(0);

    return alert_occurred;
    }


static void
lock_printf	(
	char	*format,
	int	arg1,
	int	arg2,
	int	arg3)
    {
    int status;


    status = pthread_mutex_lock (&printf_lock);
    check(status,"pthread_mutex_lock");
    printf (format, arg1, arg2, arg3);
    status = pthread_mutex_unlock (&printf_lock);
    check(status,"pthread_mutex_unlock");
    }
