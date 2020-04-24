/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crvb_alt_cvx_001.c,v $
 * Revision 1.1.8.2  1996/02/17  23:29:52  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:46:39  marty]
 *
 * Revision 1.1.8.1  1995/12/11  22:58:16  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:00  root]
 * 
 * Revision 1.1.6.1  1994/04/01  20:08:59  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:12:37  jd]
 * 
 * Revision 1.1.4.1  1993/10/12  19:34:51  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:29:46  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:28:59  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:05:44  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:20:29  devrcs
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
 *	CMA Test Programs
 *
 *  FILENAME:
 *
 *	CRVB_ALT_CVX_001
 *
 *  TEST TOPICS:
 *
 *	ALT  -  Alerts
 *	CVX  -  Condition Variable Operations
 *
 *  ABSTRACT:
 *
 *	Verify that synchronous alerts work; that the cma_e_alerted
 *	exception is raised by cma_alert_test and cma_cond_wait.
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
 *
 *	001	Webb Scales	17 July 1990
 *		Removed redundant mutex unlocks from CATCH clauses.
 *		Updated filename references.
 *	002	Dave Butenhof	29 August 1990
 *		convert to use new interfaces.
 *	003	Paul Curtin	07 January 1990
 *		Make arguments to cma_delay floats.
 *	004	Dave Butenhof	23 May 1991
 *		Increase timeouts to avoid spurious errors on initial
 *		OSF/1-based DECthreads (where kernel threads need to use slow
 *		kernel operations for synchronization).
 */


/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <dce/cts_test.h>

cma_t_mutex	mutex, printf_lock;
cma_t_cond	cv;
cma_t_thread	thread[3];
cma_t_integer	threadcount;

static cma_t_boolean wait_cv  (cma_t_integer);

static cma_t_boolean delay  (cma_t_integer);

static void lock_printf  (char *,int,int,int);

main ()
    {
    cma_t_exit_status	status;
    cma_t_boolean	result;
    cma_t_integer	count;

    cts_test ("CRVB_ALT_CVX_001", "Test synchronous alerts");

    cma_init ();

    cma_mutex_create (&mutex, &cma_c_null);
    cma_mutex_create (&printf_lock, &cma_c_null);
    count = 0;
    cma_cond_create (&cv, &cma_c_null);
    cma_thread_create (
	    &thread[0],
	    &cma_c_null,
	    (cma_t_start_routine)wait_cv,
	    (cma_t_address)0);
    cma_thread_create (
	    &thread[1],
	    &cma_c_null,
	    (cma_t_start_routine)wait_cv,
	    (cma_t_address)1);
    cma_thread_create (
	    &thread[2],
	    &cma_c_null,
	    (cma_t_start_routine)delay,
	    (cma_t_address)2);
    cma_delay (5.0);			/* Wait 5 seconds to let them wait */
    cma_thread_alert (&thread[0]);	/* Alert thread 1 */
    cma_delay (2.0);
    cma_thread_alert (&thread[1]);
    cma_delay (2.0);
    cma_thread_alert (&thread[2]);
    cma_delay (2.0);

    for (count = 0; count < 3; count++) {
	cma_thread_join (&thread[count], &status, (cma_t_address *)&result);
	cma_thread_detach (&thread[count]);
	lock_printf (
		"Thread %d termination: (%d, %d)\n",
		count,
		status,
		result);
	}

    if (threadcount != 0)
	cts_failed ("Threads did not terminate properly");

    cts_result ();

    }

static cma_t_boolean
wait_cv	(
	cma_t_integer	arg)
    {
    cma_t_boolean	alert_occurred = cma_c_false;


    cma_mutex_lock (&mutex);
    threadcount++;
    TRY {
	while (1) {
	    cma_cond_wait (&cv, &mutex);
	    lock_printf ("Spurious wakeup on thread %d\n", arg, 0, 0);
	    }
	}
    CATCH (cma_e_alerted) {
	alert_occurred = cma_c_true;
	lock_printf ("Alert received on thread %d\n", arg, 0, 0);
	threadcount--;
	}
    CATCH_ALL {
	lock_printf ("Unexpected alert on thread %d\n", arg, 0, 0);
	}
    ENDTRY

    cma_mutex_unlock (&mutex);

    return alert_occurred;
    }

static cma_t_boolean
delay	(
	cma_t_integer	arg)
    {
    cma_t_boolean	alert_occurred = cma_c_false;


    cma_mutex_lock (&mutex);
    threadcount++;
    cma_mutex_unlock (&mutex);

    TRY {
	cma_delay (5.0*60.0);
	lock_printf ("Delay completed (but shouldn't have)\n", 0, 0, 0);
	}
    CATCH (cma_e_alerted) {
	alert_occurred = cma_c_true;
	lock_printf ("Alert received on thread %d\n", arg, 0, 0);
	cma_mutex_lock (&mutex);
	threadcount--;
	cma_mutex_unlock (&mutex);
	}
    CATCH_ALL {
	lock_printf ("Unexpected alert on thread %d\n", arg, 0, 0);
	}
    ENDTRY

    return alert_occurred;
    }

static void
lock_printf	(
	char	*format,
	int	arg1,
	int	arg2,
	int	arg3)
    {
    cma_mutex_lock (&printf_lock);
    printf (format, arg1, arg2, arg3);
    cma_mutex_unlock (&printf_lock);
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_ALT_CVX_001.C */
/*  *9    29-MAY-1991 18:01:51 BUTENHOF "Increase timeouts on slower OSF/1 systems" */
/*  *8    12-FEB-1991 01:30:53 BUTENHOF "Changes to alert control" */
/*  *7     7-JAN-1991 11:29:56 CURTIN "Made changes to cma_delay arguments" */
/*  *6     5-SEP-1990 09:39:48 BUTENHOF "Convert to new interface" */
/*  *5    17-JUL-1990 18:19:39 SCALES "Remove redundant mutex unlocks from CATCH clauses" */
/*  *4     3-JUL-1990 12:03:15 BUTENHOF "Change copyright" */
/*  *3    21-JUN-1990 08:26:50 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *2     7-JUN-1990 11:35:36 SCALES "Updates to CTS" */
/*  *1    17-MAY-1990 13:57:39 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_ALT_CVX_001.C */
