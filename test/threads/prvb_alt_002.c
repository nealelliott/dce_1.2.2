/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: prvb_alt_002.c,v $
 * Revision 1.1.44.2  1996/02/17  23:31:42  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:48:10  marty]
 *
 * Revision 1.1.44.1  1995/12/11  23:03:08  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:26  root]
 * 
 * Revision 1.1.42.1  1994/04/01  20:11:46  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:18:53  jd]
 * 
 * Revision 1.1.40.1  1993/10/12  19:37:30  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:59  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:53:58  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:12:08  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:19:37  devrcs
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
 *	RV_ALT_002
 *
 *  TEST TOPICS:
 *
 *	ALT  -  Alerts
 *
 *  ABSTRACT:
 *
 *	Ensure that an alertable function does NOT raise the
 *	cma_e_alerted exception if alerts are deferred (and that
 *	the alert WILL be raised when deferral is turned off).
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
 *	001	Paul Curtin	29 May 1990
 *		Converted test to use pthreads interface.
 *	002	Paul Curtin	15 June 1990
 *		Added a cast to an arg in pthread_join to suit mips
 *	003	Dave Butenhof	7 September 1990
 *		Remove use of CMA interfaces.
 */

/*
 *  INCLUDE FILES
 */

#include <pthread.h>
#include <dce/cts_test.h>

pthread_t	thread;

static pthread_addr_t test_fork  (pthread_addr_t);

main ()
    {
    int	join_status;
    int status;
    struct timespec delay;
    pthread_addr_t	result;


    cts_test ("PRVB_ALT_002", "Test alert deferral");

    status = pthread_create (&thread, pthread_attr_default, test_fork, (pthread_addr_t)0);
    check(status,"pthread_create");
    delay.tv_nsec = 0;
    delay.tv_sec = 2;
    pthread_delay_np (&delay);			/* Give it a chance to go */
    status = pthread_cancel (thread);		/* Send it the alert */
    check(status,"pthread_cancel");
    status = pthread_join (thread, (pthread_addr_t *)&join_status);
    check(status,"pthread_join");
    status = pthread_detach (&thread);
    check(status,"pthread_detach");

    if (join_status != -1)
	cts_failed ("Thread didn't terminate with alert status");

    cts_result ();

    }

static pthread_addr_t
test_fork	(
	pthread_addr_t	arg)
    {
    int			state;
    struct timespec	delay;


    state = pthread_setcancel (CANCEL_OFF);

    TRY {
	delay.tv_sec = 5;
	delay.tv_nsec = 0;
	pthread_delay_np (&delay);	/* Delay for 5 seconds */
	}
    CATCH (cma_e_alerted) {
	cts_failed ("Alert was raised while in alert-deferred wait");
	}
    ENDTRY

    TRY {
	(void)pthread_setcancel (state);
	}
    CATCH (cma_e_alerted) {
	cts_failed ("Pending alert was raised by restoring cancel state");
	}
    ENDTRY;

    TRY {
	pthread_testcancel ();
	cts_failed ("Pending alert was not raised following deferral");
	}
    ENDTRY

    return (pthread_addr_t)0;
    }
