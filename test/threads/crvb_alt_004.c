/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crvb_alt_004.c,v $
 * Revision 1.1.8.2  1996/02/17  23:29:51  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:46:38  marty]
 *
 * Revision 1.1.8.1  1995/12/11  22:58:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:34:59  root]
 * 
 * Revision 1.1.6.1  1994/04/01  20:08:57  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:12:33  jd]
 * 
 * Revision 1.1.4.1  1993/10/12  19:34:50  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:29:45  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:28:53  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:05:39  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:20:27  devrcs
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
 *	CRVB_ALT_004
 *
 *  TEST TOPICS:
 *
 *	ALT  -  Alerts
 *
 *  ABSTRACT:
 *
 *	Test ability to next alert enable scopes within disable
 *	scopes, and detection of nesting errors.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	12 February 1991
 *
 *  MODIFICATION HISTORY:
 *
 */

/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <dce/cts_test.h>

cma_t_thread	main_thread;

int
main	(int argc, char *argv[], char *envp[])
    {
    cma_t_alert_state	as1, as2, as3;


    cts_test ("CRVB_ALT_004", "Test nested alert control scopes");
    cma_init ();
    cma_thread_get_self (&main_thread);

    cts_comment ("test 1: alert with general delivery off");

    cma_alert_disable_general (&as1);	/* Disable general delivery */

    TRY {
	cma_thread_alert (&main_thread);
	cma_alert_test ();
	}
    CATCH (cma_e_alerted) {
	cts_failed ("Received an alert with general delivery disabled");
	}
    ENDTRY
    
    cts_comment ("test 2: alert with general delivery on");

    cma_alert_enable_general (&as2);	/* Enable general delivery */

    TRY {
	cma_alert_test ();		/* Catch the pending alert */
	cts_failed ("didn't receive alert in nested enable scope");
	}
    CATCH (cma_e_alerted) {
	cts_comment ("	...the alert was delivered on schedule");
	}
    ENDTRY

    cts_comment ("test 3: create an inner disabled scope");

    cma_alert_disable_general (&as3);

    TRY {
	cma_thread_alert (&main_thread);
	cma_alert_test ();
	}
    CATCH (cma_e_alerted) {
	cts_failed ("Received an alert with general delivery disabled");
	}
    ENDTRY

    cts_comment ("test 4: restore previous context");

    cma_alert_restore (&as3);

    TRY {
	cma_alert_test ();		/* Catch the pending alert */
	cts_failed ("didn't receive alert in nested enable scope");
	}
    CATCH (cma_e_alerted) {
	cts_comment ("	...the alert was delivered on schedule");
	}
    ENDTRY

    cts_comment ("test 5: test idempotency of restore");

    TRY {
	cma_alert_restore (&as3);
	}
    CATCH (cma_e_alert_nesting) {
	cts_failed ("Restoring already restored scope raised error");
	}
    ENDTRY

    cts_comment ("test 6: restore outer scope");

    cma_alert_disable_general (&as3);	/* Disable delivery */
    cma_alert_restore (&as1);		/* Restore original state */

    TRY {
	cma_thread_alert (&main_thread);
	cma_alert_test ();		/* Catch the pending alert */
	cts_failed ("didn't receive alert in nested enable scope");
	}
    CATCH (cma_e_alerted) {
	cts_comment ("	...the alert was delivered on schedule");
	}
    ENDTRY

    cts_comment ("test 7: restoring inner scope should raise error");

    TRY {
	cma_alert_restore (&as2);
	cts_failed ("restoring inner scope didn't raise error");
	}
    CATCH (cma_e_alert_nesting) {
	cts_comment ("	...got the exception");
	}
    ENDTRY

    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_ALT_004.C */
/*  *1    12-FEB-1991 01:20:47 BUTENHOF "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_ALT_004.C */
