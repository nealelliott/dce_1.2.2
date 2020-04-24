/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: prvb_tim_001.c,v $
 * Revision 1.1.65.1  1996/06/04  22:15:17  arvind
 * 	Fix for +/-5%
 * 	[1996/03/08  22:17 UTC  bissen  /main/DCE_1.2/1]
 *
 * Revision 1.1.63.2  1996/02/17  23:32:09  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:48:33  marty]
 * 
 * Revision 1.1.63.1  1995/12/11  23:04:21  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:47  root]
 * 
 * Revision 1.1.61.1  1994/04/01  20:12:34  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:20:46  jd]
 * 
 * Revision 1.1.59.1  1993/10/12  19:38:15  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:31:21  rhw]
 * 
 * Revision 1.1.3.4  1993/02/01  21:46:25  hinman
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  17:07:40  hinman]
 * 
 * Revision 1.1.3.3  1992/12/30  16:55:43  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:13:46  zeliff]
 * 	Revision 1.1.4.2  1993/01/11  16:17:29  hinman
 * 	[hinman] - Check in merged SNI version
 * 
 * Revision 1.1.3.2  1992/09/03  14:33:20  bolinger
 * 	Move current 1.0.1-backed file (modified to work with new
 * 	threads source base) from private branch into 1.0.2 tree.
 * 	(See OT defect 5300.)
 * 	[1992/09/02  13:46:07  bolinger]
 * 
 * Revision 1.1.1.2  1992/08/11  21:46:58  bolinger
 * 	Replace uses of struct timeb/ftime(3) with refs to new timing
 * 	interface specific to these tests (which is designed to hide
 * 	any further changes to the underlying system facilities used).
 * 
 * Revision 1.1  1992/01/19  03:22:20  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  Copyright (c) 1990 by
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
 *	PRVB_TIM_001
 *
 *  TEST TOPICS:
 *
 *	TIM  -  Timer Operations
 *
 *  ABSTRACT:
 *
 *	Use the basic PTHREAD time services (pthread_delay
 *	pthread_time_get_expiration, pthread_cond_timed_wait) to ensure that
 *	they function as they should.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	17 November 1989
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	29 June 1990
 *		Fix pthread_get_expiration_np to new definition.
 *	002	Dave Butenhof	07 August 1990
 *		Add printfs to report the actual time for delay & timedwait
 *		calls; and fix the name, which somehow got corrupted (to
 *		prvb_tim_001_001).
 *	003	Dave Butenhof	11 October 1990
 *		Use cma_errno.h
 */

/*
 *  INCLUDE FILES
 */

#include <pthread.h>
#include <cma_px.h>
#include <cma_errno.h>
#include <dce/cts_test.h>
#ifdef vms
#include <timeb.h>
#endif

static float dif_secs  (cts_timebuf_t *,cts_timebuf_t *);

main ()
    {
    pthread_mutex_t	mutex;
    pthread_cond_t	cv;
    cts_timebuf_t		time1, time2;
    int			interval;
    float		wait;
    struct timespec	delta, wait_time;
    int			status;


    cts_test ("PRVB_TIM_001", "Test delay, get_expiration, etc");

    status = pthread_mutex_init (&mutex, pthread_mutexattr_default);
    check (status, "pthread_mutex_init");
    status = pthread_cond_init (&cv, pthread_condattr_default);
    check (status, "pthread_cond_init");

    for (interval = 0; interval <= 5; interval++) {
	printf ("Testing %d seconds...\n", interval);
	delta.tv_sec = interval;
	delta.tv_nsec = 0;
	status = pthread_get_expiration_np (&delta, &wait_time);
	check (status, "pthread_get_expiration_np");
	status = pthread_mutex_lock (&mutex);
	check (status, "pthread_mutex_lock");
	cts_gettime (&time1);

	while (((status = pthread_cond_timedwait (
		&cv,
		&mutex,
		&wait_time)) != -1)
	    || (errno != EAGAIN));

	if (status == -1 && errno != EAGAIN)
	    perror ("pthread_cond_timedwait");

	cts_gettime (&time2);
	status = pthread_mutex_unlock (&mutex);
	check (status, "pthread_mutex_unlock");
	wait = dif_secs (&time1, &time2);
	printf (".. timedwait waited %f seconds\n", wait);

        if ((interval > 0) &&
            ((wait < (float)interval * 0.95) || (wait > (float)interval * 1.05)))
	    cts_failed (
		    "%d second timed wait waited %f seconds",
		    interval,
		    wait);

	cts_gettime (&time1);
	wait_time.tv_sec = interval;
	wait_time.tv_nsec = 0;
	status = pthread_delay_np (&wait_time);
	check (status, "pthread_delay_np");
	cts_gettime (&time2);
	wait = dif_secs (&time1, &time2);
	printf (".. delay waited %f seconds\n", wait);

        if ((interval > 0) &&
            ((wait < (float)interval * 0.95) || (wait > (float)interval * 1.05)))
	    cts_failed (
		    "%d second delay waited %f seconds",
		    interval,
		    wait);

#if _CMA_UNIX_TYPE == _CMA__SVR4
    cts_gettime(&time1);
    status = pthread_delay_unsigned_np (interval);
    check (status, "pthread_delay_np");
    cts_gettime(&time2);

    wait = dif_secs (&time1, &time2);
    printf (".. delay waited %f seconds\n", wait);

    if (wait < (float)interval)
        cts_failed (
            "%d second delay waited %f seconds",
            interval,
            wait);

#endif /* _CMA_SVR4 */

	}

    cts_result ();
    }

static float
dif_secs	(
	cts_timebuf_t	*t1,
	cts_timebuf_t	*t2)
    {
	return (cts_timediff(t1, t2));
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element PRVB_TIM_001.C */
/*  *7    11-OCT-1990 13:04:43 BUTENHOF "Fix use of errno" */
/*  *6     7-AUG-1990 10:56:47 BUTENHOF "Fix name, add info printfs" */
/*  *5     3-JUL-1990 12:10:42 BUTENHOF "Change copyright" */
/*  *4    29-JUN-1990 14:13:53 BUTENHOF "Reflect change to pthread_get_expiration_np" */
/*  *3    22-JUN-1990 11:22:27 BUTENHOF "Conform to 1003.4a/D3 synch. interface changes" */
/*  *2    21-JUN-1990 08:43:29 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *1    14-JUN-1990 07:47:03 BUTENHOF "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element PRVB_TIM_001.C */
