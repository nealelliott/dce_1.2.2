/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crvb_alt_003.c,v $
 * Revision 1.1.9.2  1996/02/17  23:29:51  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:46:37  marty]
 *
 * Revision 1.1.9.1  1995/12/11  22:58:12  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:34:58  root]
 * 
 * Revision 1.1.7.1  1994/04/01  20:08:55  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:12:28  jd]
 * 
 * Revision 1.1.5.1  1993/10/12  19:34:49  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:29:44  rhw]
 * 
 * Revision 1.1.3.3  1992/12/30  16:28:49  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:05:34  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/03  14:31:10  bolinger
 * 	Move current 1.0.1-backed file (modified to work with new
 * 	threads source base) from private branch into 1.0.2 tree.
 * 	(See OT defect 5300.)
 * 	[1992/09/02  13:44:11  bolinger]
 * 
 * Revision 1.1.1.2  1992/08/12  20:58:43  bolinger
 * 	Replace uses of struct timeb/ftime(3) with refs to new timing
 * 	interface specific to these tests (which is designed to hide
 * 	any further changes to the underlying system facilities used).
 * 
 * Revision 1.1  1992/01/19  03:20:26  devrcs
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
 *	CRVB_ALT_003
 *
 *  TEST TOPICS:
 *
 *	ALT  -  Alerts
 *
 *  ABSTRACT:
 *
 *	Make sure than a thread with asynchronous alert delivery
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
 *	001	Dave Butenhof	4 September 1990
 *		Change to new CMA interfaces.
 *	002	Dave Butenhof	7 February 1991
 *		New alert control interfaces.
 */

/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <dce/cts_test.h>
#ifdef vms
#include <timeb.h>
#endif

#define LIMIT 20000000

cma_t_thread	thread1, thread2, thread3;

static cma_t_address alertable  (cma_t_address);

static cma_t_address nonalertable  (cma_t_address);

static cma_t_address bystander  (cma_t_address);

static cma_t_interval dif_secs  (_CMA_VOLATILE_ cts_timebuf_t *,
                                 _CMA_VOLATILE_ cts_timebuf_t *);

int
main	(int argc, char *argv[], char *envp[])
    {
    cma_t_exit_status	status;
    cma_t_address	result;


    cts_test ("CRVB_ALT_003", "Test asynchronous alert delivery");
    cma_init ();

    cma_thread_create (&thread1, &cma_c_null, alertable, cma_c_null_ptr);
    cma_thread_create (&thread2, &cma_c_null, nonalertable, cma_c_null_ptr);
    cma_thread_create (&thread3, &cma_c_null, bystander, cma_c_null_ptr);
    cma_delay (1.0);			/* Give it a chance to go */
    cma_thread_alert (&thread1);	/* Send it the alert */
    cma_thread_alert (&thread2);	/* Send it the alert */
    cma_thread_join (&thread1, &status, &result);
    cma_thread_detach (&thread1);

    if (status != cma_c_term_alert)
	cts_failed ("thread 1 didn't terminate with alert status");

    /*
     * We don't join with thread2 or thread3, since thread2 is in a long
     * compute-bound loop, and thread3 is in a long delay.  If they were
     * alerted, they'd have reported an error, and if they weren't, we really
     * don't care what happens to them.
     */

    cts_result ();
    }

static cma_t_address
alertable	(
	cma_t_address	arg)
    {
    cma_t_alert_state			state;
    _CMA_VOLATILE_ cma_t_integer	counter;
    _CMA_VOLATILE_ cts_timebuf_t		time1, time2;
    cma_t_interval			wait;


    /*
     * This test goes into a compute bound loop, with asynchronous alerts
     * enabled.  Note that it assumes that incrementing a count to LIMIT will
     * take longer than the 1 second that the main thread will delay before
     * sending the alerts (the counter is volatile, so register optimizations
     * can't be used). On an extremely fast machine (say, over 100 MIPS),
     * this loop might time out and report a spurious error.
     */
    cma_alert_disable_asynch (&state);
    cma_alert_enable_asynch ();

    TRY {
	cts_gettime (&time1);
	for (counter = 0; counter < LIMIT; counter++);
	cts_gettime (&time2);
	wait = dif_secs (&time1, &time2);
	cts_failed ("alert didn't occur in alertable within %f seconds", wait);
	}
    CATCH (cma_e_alerted) {
	cts_gettime (&time2);
	wait = dif_secs (&time1, &time2);
	printf (
		"Alert occurred after %f seconds, at count of %d\n",
		wait,
		counter);
	RERAISE;
	}
    ENDTRY

    cma_alert_restore (&state);
    return cma_c_null_ptr;
    }

static cma_t_address
nonalertable	(
	cma_t_address	arg)
    {
    _CMA_VOLATILE_ cma_t_integer	counter;


    /*
     * This thread goes into a compute-bound loop without enabling
     * asynchronous alerts; the main thread will alert it, but since there
     * are no alert points, the pending alert can't be delivered.  If it is
     * delivered, an error will be reported.
     */
    TRY {
	for (counter = 0; counter < LIMIT; counter++);
	}
    CATCH (cma_e_alerted) {
	cts_failed (
		"alert occurred in nonalertable at count of %d",
		counter);
	RERAISE;
	}
    ENDTRY

    return cma_c_null_ptr;
    }

static cma_t_address
bystander	(
	cma_t_address	arg)
    {

    /*
     * This thread is an "innocent bystander" at the alert test. It's never
     * alerted. If it accidentally gets hit by any flying debris, it'll
     * complain.
     */
    TRY {
	cma_delay (60.0);		/* Wait a long time */
	}
    CATCH (cma_e_alerted) {
	cts_failed ("alert occurred in bystander");
	RERAISE;
	}
    ENDTRY

    return cma_c_null_ptr;
    }

static cma_t_interval
dif_secs	(
	_CMA_VOLATILE_ cts_timebuf_t	*t1,
	_CMA_VOLATILE_ cts_timebuf_t	*t2)
    {
    cma_t_integer	sec, ms;

	return (cts_timediff(t1, t2));

    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_ALT_003.C */
/*  *4    12-FEB-1991 01:30:45 BUTENHOF "Changes to alert control" */
/*  *3     5-SEP-1990 09:39:41 BUTENHOF "Convert to new CMA interfaces" */
/*  *2    16-AUG-1990 12:17:46 BUTENHOF "Fix for MIPS C" */
/*  *1    16-AUG-1990 08:23:36 BUTENHOF "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_ALT_003.C */
