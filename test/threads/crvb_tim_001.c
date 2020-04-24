/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crvb_tim_001.c,v $
 * Revision 1.1.49.2  1996/02/17  23:30:42  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:23  marty]
 *
 * Revision 1.1.49.1  1995/12/11  23:00:26  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:41  root]
 * 
 * Revision 1.1.46.1  1994/04/01  20:10:27  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:15:58  jd]
 * 
 * Revision 1.1.44.1  1993/10/12  19:36:08  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:20  rhw]
 * 
 * Revision 1.1.3.3  1992/12/30  16:32:48  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:22:19  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/03  14:31:32  bolinger
 * 	Move current 1.0.1-backed file (modified to work with new
 * 	threads source base) from private branch into 1.0.2 tree.
 * 	(See OT defect 5300.)
 * 	[1992/09/02  13:44:31  bolinger]
 * 
 * Revision 1.1.1.2  1992/08/12  21:04:09  bolinger
 * 	Replace uses of struct timeb/ftime(3) with refs to new timing
 * 	interface specific to these tests (which is designed to hide
 * 	any further changes to the underlying system facilities used).
 * 
 * Revision 1.1  1992/01/19  03:21:21  devrcs
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
 *	CMA Test Programs
 *
 *  FILENAME:
 *
 *	RV_TIM_001
 *
 *  TEST TOPICS:
 *
 *	INI  -  One-time Initialization TIM  -  Timer Operations
 *
 *  ABSTRACT:
 *
 *	Use the basic CMA time services (cma_delay cma_time_get_expiration,
 *	cma_cond_timed_wait) to ensure that they function as they should.
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
 *	001	Dave Butenhof	5 September 1990
 *		Convert to new CMA interfaces.
 */

/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <dce/cts_test.h>
#ifdef vms
#include <timeb.h>
#endif

static cma_t_interval dif_secs  (cts_timebuf_t *,cts_timebuf_t *);

main ()
    {
    cma_t_mutex		mutex;
    cma_t_cond	cv;
    cts_timebuf_t		time1, time2;
    cma_t_integer	interval;
    cma_t_date_time	wait_time;
    cma_t_interval	wait;

    cts_test ("CRVB_TIM_001", "Test delay, get_expiration, etc");

    cma_init ();

    cma_mutex_create (&mutex, &cma_c_null);
    cma_cond_create (&cv, &cma_c_null);

    for (interval = 0; interval <= 5; interval++) {
	printf ("Testing %d seconds...\n", interval);
	cma_time_get_expiration (&wait_time, (float)interval);
	cma_mutex_lock (&mutex);
	cts_gettime (&time1);

	while (cma_cond_timed_wait (&cv, &mutex, &wait_time) != cma_s_timed_out);

	cts_gettime (&time2);
	cma_mutex_unlock (&mutex);
	wait = dif_secs (&time1, &time2);

	if (wait < (float)interval)
	    cts_failed (
		    "%d second timed wait waited %f seconds",
		    interval,
		    wait);

	cts_gettime (&time1);
	cma_delay ((float)interval);
	cts_gettime (&time2);
	wait = dif_secs (&time1, &time2);

	if (wait < (float)interval)
	    cts_failed (
		    "%d second delay waited %f seconds",
		    interval,
		    wait);

	}

    cts_result ();
    }

static cma_t_interval
dif_secs	(
	cts_timebuf_t	*t1,
	cts_timebuf_t	*t2)
    {
	return (cts_timediff(t1, t2));
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_TIM_001.C */
/*  *5     5-SEP-1990 09:41:18 BUTENHOF "Convert to new CMA interfaces" */
/*  *4     3-JUL-1990 12:06:24 BUTENHOF "Change copyright" */
/*  *3    21-JUN-1990 08:33:06 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *2     7-JUN-1990 11:39:19 SCALES "Updates to CTS" */
/*  *1    17-MAY-1990 13:59:01 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_TIM_001.C */
