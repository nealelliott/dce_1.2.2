/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: prvb_can_003.c,v $
 * Revision 1.1.70.2  1996/02/17  23:31:49  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:48:16  marty]
 *
 * Revision 1.1.70.1  1995/12/11  23:03:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:31  root]
 * 
 * Revision 1.1.68.1  1994/04/01  20:11:55  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:19:15  jd]
 * 
 * Revision 1.1.66.1  1993/10/12  19:37:43  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:31:05  rhw]
 * 
 * Revision 1.1.5.3  1992/12/30  16:54:26  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:12:36  zeliff]
 * 
 * Revision 1.1.5.2  1992/09/03  14:33:07  bolinger
 * 	Move current 1.0.1-backed file (modified to work with new
 * 	threads source base) from private branch into 1.0.2 tree.
 * 	(See OT defect 5300.)
 * 	[1992/09/02  13:45:57  bolinger]
 * 
 * Revision 1.1.3.2  1992/08/11  21:45:27  bolinger
 * 	Replace uses of struct timeb/ftime(3) with refs to new timing
 * 	interface specific to these tests (which is designed to hide
 * 	any further changes to the underlying system facilities used).
 * 
 * Revision 1.1.2.2  1992/05/11  17:08:31  bolinger
 * 	Fix OT defect 2161:  provide/correct missing/bad cts_test() banners.
 * 	[1992/05/11  13:46:49  bolinger]
 * 
 * Revision 1.1  1992/01/19  03:22:02  devrcs
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
 *	PRVB_CAN_003
 *
 *  TEST TOPICS:
 *
 *	CAN  -  Cancellation
 *
 *  ABSTRACT:
 *
 *	Make sure than a thread with asynchronous cancel delivery
 *	enabled will be broken out of a compute-bound loop, that
 *	enabling async delivery in one thread won't affect other
 *	threads, and so forth.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	14 August 1990
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	7 September 1990
 *		Remove CMA-ism (cma_c_null).
 *	002	Dave Butenhof	17 June 1991
 *		Fix some minor compilation warnings in new MIPS C
 */

/*
 *  INCLUDE FILES
 */

#include <pthread.h>
#include <dce/cts_test.h>
#ifdef vms
#include <timeb.h>
#endif

#define LIMIT 20000000
#ifdef NULL
# undef NULL
#endif
#define NULL (pthread_addr_t)0

pthread_t	thread1, thread2, thread3;

_CMA_VOLATILE_ int	counter1, counter2;
_CMA_VOLATILE_ cts_timebuf_t	time1, time2, time3, time4;

static pthread_addr_t cancelable  (pthread_addr_t);

static pthread_addr_t noncancelable  (pthread_addr_t);

static pthread_addr_t bystander  (pthread_addr_t);

static float dif_secs  (_CMA_VOLATILE_ cts_timebuf_t *,_CMA_VOLATILE_ cts_timebuf_t *);

int
main	(int argc, char *argv[], char *envp[])
    {
    pthread_addr_t	result;
    struct timespec	one_sec;


    cts_test ("PRVB_CAN_003", "Test asynchronous cancel delivery");

    pthread_create (&thread1, pthread_attr_default, cancelable, NULL);
    pthread_create (&thread2, pthread_attr_default, noncancelable, NULL);
    pthread_create (&thread3, pthread_attr_default, bystander, NULL);
    one_sec.tv_sec = 1;
    one_sec.tv_nsec = 0;
    pthread_delay_np (&one_sec);	/* Give it a chance to go */
    pthread_cancel (thread1);		/* Send it the cancel */
    pthread_cancel (thread2);		/* Send it the cancel */
    pthread_join (thread1, &result);
    pthread_detach (&thread1);

    if ((int)result != -1)
	cts_failed ("thread 1 didn't terminate with cancel status");

    /*
     * We don't join with thread2 or thread3, since thread2 is in a long
     * compute-bound loop, and thread3 is in a long delay.  If they were
     * canceled, they'd have reported an error, and if they weren't, we
     * really don't care what happens to them.
     */

    cts_result ();
    }

static void
cancel_during_loop	(pthread_addr_t	arg)
    {
    float	wait;


    cts_gettime (&time2);
    wait = dif_secs (&time1, &time2);
    printf (
	    "Cancel occurred after %f seconds, at count of %d\n",
	    wait,
	    counter1);
    }

static void
cancel_in_noncancel	(pthread_addr_t	arg)
    {
    cts_failed (
	    "cancel occurred in noncancelable at count of %d",
	    counter2);
    }

static void
cancel_in_bystander	(pthread_addr_t	arg)
    {
    cts_failed ("cancel occurred in bystander");
    }

static pthread_addr_t
cancelable	(
	pthread_addr_t	arg)
    {
    int	state;
    float	wait;


    /*
     * This test goes into a compute bound loop, with asynchronous cancels
     * enabled.  Note that it assumes that incrementing a count to LIMIT will
     * take longer than the 1 second that the main thread will delay before
     * sending the cancels (the counter is volatile, so register
     * optimizations can't be used). On an extremely fast machine (say, over
     * 100 MIPS), this loop might time out and report a spurious error.
     */
    state = pthread_setasynccancel (CANCEL_OFF);
    cts_gettime (&time1);
    (void) pthread_setasynccancel (CANCEL_ON);

    pthread_cleanup_push (cancel_during_loop, (pthread_addr_t)0);
    for (counter1 = 0; counter1 < LIMIT; counter1++);
    cts_gettime (&time2);
    wait = dif_secs (&time1, &time2);
    cts_failed ("cancel didn't occur in cancelable within %f seconds", wait);
    pthread_cleanup_pop (0);

    (void) pthread_setasynccancel (state);
    return NULL;
    }

static pthread_addr_t
noncancelable	(
	pthread_addr_t	arg)
    {
    /*
     * This thread goes into a compute-bound loop without enabling
     * asynchronous cancels; the main thread will cancel it, but since there
     * are no cancel points, the pending cancel can't be delivered.  If it is
     * delivered, an error will be reported.
     */
    pthread_cleanup_push (cancel_in_noncancel, (pthread_addr_t)0);
    for (counter2 = 0; counter2 < LIMIT; counter2++);
    pthread_cleanup_pop (0);

    return NULL;
    }

static pthread_addr_t
bystander	(
	pthread_addr_t	arg)
    {
    struct timespec	one_min;

    /*
     * This thread is an "innocent bystander" at the cancel test. It's never
     * canceled. If it accidentally gets hit by any flying debris, it'll
     * complain.
     */
    one_min.tv_sec = 60;
    one_min.tv_nsec = 0;

    pthread_cleanup_push (cancel_in_bystander, (pthread_addr_t)0);
    pthread_delay_np (&one_min);	/* Wait a long time */
    pthread_cleanup_pop (0);

    return NULL;
    }

static float
dif_secs	(
	_CMA_VOLATILE_ cts_timebuf_t	*t1,
	_CMA_VOLATILE_ cts_timebuf_t	*t2)
    {
	return (cts_timediff(t1, t2));
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element PRVB_CAN_003.C */
/*  *4    17-JUN-1991 11:41:03 BUTENHOF "Fix compilation warnings" */
/*  *3    12-FEB-1991 01:35:04 BUTENHOF "Changes to alert control" */
/*  *2     7-SEP-1990 09:06:41 BUTENHOF "Remove CMA-isms from pthread tests" */
/*  *1     5-SEP-1990 09:46:04 BUTENHOF "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element PRVB_CAN_003.C */
