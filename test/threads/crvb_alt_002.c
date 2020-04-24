/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crvb_alt_002.c,v $
 * Revision 1.1.8.2  1996/02/17  23:29:50  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:46:36  marty]
 *
 * Revision 1.1.8.1  1995/12/11  22:58:10  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:34:57  root]
 * 
 * Revision 1.1.6.1  1994/04/01  20:08:52  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:12:24  jd]
 * 
 * Revision 1.1.4.1  1993/10/12  19:34:47  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:29:43  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:28:47  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:05:29  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:20:24  devrcs
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
 *	CRVB_ALT_002
 *
 *  TEST TOPICS:
 *
 *	ALT  -  Alerts
 *
 *  ABSTRACT:
 *
 *	Ensure that an alertable function does NOT raise the
 *	cma_e_alerted exception if general alert delivery is disabled (and
 *	that the alert WILL be raised when general delivery is reenabled).
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	13 December 1989
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	4 September 1990
 *		Change to new CMA interfaces.
 *	002	Dave Butenhof	7 February 1991
 *		New alert control interfaces
 *	003	Dave Butenhof	20 February 1991
 *		Change name of "fork" function
 */

/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <dce/cts_test.h>

cma_t_thread	thread;

static cma_t_address worker  (cma_t_address);

main ()
    {
    cma_t_exit_status	status;
    cma_t_address	result;


    cts_test ("CRVB_ALT_002", "Test general alert disable");

    cma_init ();

    cma_thread_create (&thread, &cma_c_null, worker, cma_c_null_ptr);
    cma_delay (2.0);			/* Give it a chance to go */
    cma_thread_alert (&thread);		/* Send it the alert */
    cma_thread_join (&thread, &status, &result);
    cma_thread_detach (&thread);

    if (status != cma_c_term_alert)
	cts_failed ("Thread didn't terminate with alert status");

    cts_result ();
    }

static cma_t_address
worker	(
	cma_t_address	arg)
    {
    cma_t_alert_state	state;

    cma_alert_disable_general (&state);

    TRY {
	cma_delay (5.0);		/* Delay for 5 seconds */
	}
    CATCH (cma_e_alerted) {
	cts_failed ("Alert was raised while general delivery was disabled");
	}
    ENDTRY

    TRY {
	cma_alert_restore (&state);
	}
    CATCH (cma_e_alerted) {
	cts_failed ("Pending alert was raised by restore");
	}
    ENDTRY;

    TRY {
	cma_alert_test ();
	cts_failed ("Pending alert was not raised by alert test");
	}
    ENDTRY

    return cma_c_null_ptr;
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_ALT_002.C */
/*  *7    20-FEB-1991 23:01:38 BUTENHOF "Change name of ""fork"" function" */
/*  *6    12-FEB-1991 01:30:36 BUTENHOF "Changes to alert control" */
/*  *5     5-SEP-1990 09:39:34 BUTENHOF "Convert to new CMA interfaces" */
/*  *4     3-JUL-1990 12:03:08 BUTENHOF "Change copyright" */
/*  *3    21-JUN-1990 08:26:37 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *2     7-JUN-1990 11:35:29 SCALES "Updates to CTS" */
/*  *1    17-MAY-1990 13:57:37 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_ALT_002.C */
