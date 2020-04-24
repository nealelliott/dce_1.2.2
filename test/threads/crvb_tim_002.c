/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crvb_tim_002.c,v $
 * Revision 1.1.69.2  1996/02/17  23:30:43  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:24  marty]
 *
 * Revision 1.1.69.1  1995/12/11  23:00:29  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:42  root]
 * 
 * Revision 1.1.67.1  1994/04/01  20:10:28  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:16:02  jd]
 * 
 * Revision 1.1.65.1  1993/10/12  19:36:10  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:21  rhw]
 * 
 * Revision 1.1.5.3  1992/12/30  16:32:50  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:22:23  zeliff]
 * 
 * Revision 1.1.5.2  1992/09/03  14:31:37  bolinger
 * 	Move current 1.0.1-backed file (modified to work with new
 * 	threads source base) from private branch into 1.0.2 tree.
 * 	(See OT defect 5300.)
 * 	[1992/09/02  13:44:36  bolinger]
 * 
 * Revision 1.1.3.2  1992/08/12  21:04:51  bolinger
 * 	Replace uses of struct timeb/ftime(3) with refs to new timing
 * 	interface specific to these tests (which is designed to hide
 * 	any further changes to the underlying system facilities used).
 * 
 * Revision 1.1.2.2  1992/06/30  13:43:59  bolinger
 * 	Fix OT defect 4084: use _CMA_VOLATILE_ instead of volatile alone.
 * 	[1992/06/29  19:33:07  bolinger]
 * 
 * Revision 1.1  1992/01/19  03:21:22  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  Copyright (c) 1991 by
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
 *	CRVB_TIM_002
 *
 *  TEST TOPICS:
 *
 *	TIM  -  Timer Operations
 *
 *  ABSTRACT:
 *
 *	Create a number of threads, each of which repeatedly wait
 *	for some fixed interval of time (each thread has a different
 *	interval). Each will report the actual elapsed time of each
 *	delay, and will report an error if any delay is too short
 *	or more than 20% too long.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	12 March 1991
 *
 *  MODIFICATION HISTORY:
 *
 */

/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <dce/cts_test.h>
#ifdef vms
#include <timeb.h>
#endif

#define NUM_THREADS	20

cma_t_thread	threads[NUM_THREADS];

static cma_t_interval
dif_secs	(
	cts_timebuf_t	*t1,
	cts_timebuf_t	*t2)
    {
	return (cts_timediff(t1, t2));
    }

static cma_t_address
worker	(cma_t_address	arg)
    {
    _CMA_VOLATILE_ cma_t_integer	thdnum = (cma_t_integer)arg;
    _CMA_VOLATILE_ cma_t_interval	wait, delay;
    _CMA_VOLATILE_ cma_t_integer	counter = 0, countunder = 0, countover = 0;
    _CMA_VOLATILE_ cma_t_interval	total_error, mindelay, maxdelay;
    cts_timebuf_t			time1, time2;


    delay = (float)thdnum;		/* Thread 1 waits 1.0 seconds, etc. */

    total_error = 0.0;
    mindelay = 100000.0;		/* Just a high number */
    maxdelay = 0.0;			/* A low number */

    TRY {
	while (1) {
	    cts_gettime (&time1);
	    cma_delay (delay);
	    cts_gettime (&time2);
	    wait = dif_secs (&time1, &time2);

	    if (wait < delay)
		countunder++;

	    if (wait > delay)
		countover++;

	    if (wait > maxdelay)
		maxdelay = wait;

	    if (wait < mindelay)
		mindelay = wait;

	    counter++;
	    total_error += (wait - delay);
	    }
	}
    CATCH (cma_e_alerted) {
	printf (
		"Thread %d: %d under, %d over; min %f, max %f, avg %f\n",
		thdnum,
		countunder,
		countover,
		mindelay,
		maxdelay,
		delay + (total_error/(float)counter));

	if (countunder > 0)
	    cts_failed ("Thread %d had %d short waits", thdnum, countunder);

	if (maxdelay > delay + (delay*0.2) && delay > 0.0)
	    cts_failed (
		    "Thread %d had waits more than 20%% too long",
		    thdnum);

	RERAISE;
	}
    ENDTRY

    }

int
main	(int argc, char *argv[], char *envp[])
    {
    cma_t_integer	i;
    cma_t_exit_status	status;
    cma_t_address	value;


    cts_test ("CRVB_TIM_002", "Test accuracy of repeated concurrent delays");
    cma_init ();

    for (i = 0; i < NUM_THREADS; i++)
	cma_thread_create (&threads[i], &cma_c_null, worker, (cma_t_address)i);

    cma_delay (60.0*3.0);		/* Delay 3 minutes */

    for (i = 0; i < NUM_THREADS; i++) {	/* Alert them all */
	cma_thread_alert (&threads[i]);
	cma_thread_join (
		&threads[i],
		&status,
		&value);
	}

    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_TIM_002.C */
/*  *1    13-MAR-1991 14:31:38 BUTENHOF "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_TIM_002.C */
