/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crvb_alt_exc_001.c,v $
 * Revision 1.1.8.2  1996/02/17  23:29:54  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:46:41  marty]
 *
 * Revision 1.1.8.1  1995/12/11  22:58:21  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:01  root]
 * 
 * Revision 1.1.6.1  1994/04/01  20:09:03  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:12:47  jd]
 * 
 * Revision 1.1.4.1  1993/10/12  19:34:54  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:29:48  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:29:07  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:05:53  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:20:32  devrcs
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
 *	CRVB_ALT_EXC_001
 *
 *  TEST TOPICS:
 *
 *	ALT  -  Alerts
 *	EXC  -  Exceptions
 *
 *  ABSTRACT:
 *
 *	This program tests the ability to handle an alert, continue
 *	the program, and receive another alert.  This capability is
 *	required by RPC.  It performs the test by creating a thread
 *	which will execute a loop with a cma_alert_test call; after
 *	receiving an alert, the thread will resume the loop.  The
 *	main thread will create the thread, delay several seconds,
 *	send an alert, delay another few seconds, send a second
 *	alert, delay another few seconds, and check to see whether
 *	the alert thread is still there (note that a join operation
 *	isn't done, since it would hang if the second alert is never
 *	delivered).
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	01 May 1990
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	4 September 1990
 *		Change to new CMA interfaces.
 */

/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <dce/cts_test.h>

static cma_t_mutex	mu;
static cma_t_integer	flag;

#define FIRST_ALERT	1
#define SECOND_ALERT	2
#define NORMAL_EXIT	3

static cma_t_address
thread	(
	cma_t_address	arg)
    {
    TRY {
	while (cma_c_true)
	    cma_alert_test ();
	}
    CATCH (cma_e_alerted) {
	cma_mutex_lock (&mu);
	flag = FIRST_ALERT;
	cma_mutex_unlock (&mu);
	}
    ENDTRY

    /*
     * Now, delay for 10 seconds (delay is alertable); if we haven't gotten
     * an alert by then, something's not working, so exit.
     */
    TRY {
	cma_delay (10.0);
	}
    CATCH (cma_e_alerted) {
	cma_mutex_lock (&mu);
	flag = SECOND_ALERT;
	cma_mutex_unlock (&mu);
	RERAISE;
	}
    ENDTRY

    /*
     * If the second alert wasn't delivered, then we'll fall through to here;
     * set "normal exit" status, and exit.
     */
    cma_mutex_lock (&mu);
    flag = NORMAL_EXIT;
    cma_mutex_unlock (&mu);
    }

main ()
    {
    cma_t_thread	thd;
    cma_t_exit_status	status;
    cma_t_address	result;


    cts_test ("CRVB_ALT_EXC_001", "Test ability to handle alerts and continue");

    cma_init ();

    cma_mutex_create (&mu, &cma_c_null);
    flag = 0;
    cma_thread_create (
	    &thd,
	    &cma_c_null,
	    (cma_t_start_routine)thread,
	    cma_c_null_ptr);
    cma_delay (2.0);			/* Delay a bit to let it run */
    cma_thread_alert (&thd);		/* Alert it */
    cma_delay (2.0);			/* Give it time to go again */
    cma_thread_alert (&thd);		/* Alert it again */
    cma_thread_join (&thd, &status, &result);

    if (status != cma_c_term_alert)
	cts_failed ("didn't terminate with alert status");
    else {
	switch (flag) {
	    case 0 : {
		cts_failed ("didn't receive first alert");
		break;
		}
	    case FIRST_ALERT : {
		cts_failed ("received first alert, but not second");
		break;
		}
	    case SECOND_ALERT : {
		break;
		}
	    case NORMAL_EXIT : {
		cts_failed ("didn't receive second alert.");
		}
	    }
	}

    cts_result ();

    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_ALT_EXC_001.C */
/*  *6    12-FEB-1991 01:31:08 BUTENHOF "Changes to alert control" */
/*  *5     5-SEP-1990 09:39:56 BUTENHOF "Convert to new CMA interfaces" */
/*  *4     3-JUL-1990 12:03:20 BUTENHOF "Change copyright" */
/*  *3    21-JUN-1990 08:27:04 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *2     7-JUN-1990 11:35:42 SCALES "Updates to CTS" */
/*  *1    17-MAY-1990 13:57:42 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_ALT_EXC_001.C */
