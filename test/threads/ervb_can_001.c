/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ervb_can_001.c,v $
 * Revision 1.1.43.2  1996/02/17  23:31:20  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:51  marty]
 *
 * Revision 1.1.43.1  1995/12/11  23:01:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:08  root]
 * 
 * Revision 1.1.41.1  1994/04/01  20:11:13  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:17:49  jd]
 * 
 * Revision 1.1.39.1  1993/10/12  19:36:54  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:42  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:52:29  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:10:46  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:21:49  devrcs
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
 *	PTHREAD Test Programs
 *
 *  FILENAME:
 *
 *	ERVB_CAN_001
 *
 *  TEST TOPICS:
 *
 *	CAN  -  Cancellation
 *
 *  ABSTRACT:
 *
 *	Ensure that a cancellable function does NOT cause cancellation
 *	if general cancellability is disabled (and that
 *	the cancel WILL happen when general cancellability is enabled).
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	7 February 1991
 *
 *  MODIFICATION HISTORY:
 * 
 *	001	Dave Butenhof	21 February 1991
 *		Change name of "fork" procedure.
 */

/*
 *  INCLUDE FILES
 */

#include <pthread_exc.h>
#include <dce/cts_test.h>

pthread_t	thread;

static pthread_addr_t worker  (pthread_addr_t);

int
main	(int argc, char *argv[], char *envp[])
    {
    int	join_status;
    struct timespec delay;
    pthread_addr_t	result;


    cts_test ("ERVB_CAN_001", "Test general cancellation control");

    pthread_create (&thread, pthread_attr_default, worker, (pthread_addr_t)0);
    delay.tv_nsec = 0;
    delay.tv_sec = 2;
    pthread_delay_np (&delay);		/* Give it a chance to go */
    pthread_cancel (thread);		/* Send it the cancel */
    pthread_join (thread, (pthread_addr_t *)&join_status);
    pthread_detach (&thread);

    if (join_status != -1)
	cts_failed ("Thread didn't terminate with cancel status");

    cts_result ();
    }

static pthread_addr_t
worker	(
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
    CATCH (pthread_cancel_e) {
	cts_failed ("Cancel was raised while general cancellation was off");
	}
    ENDTRY

    TRY {
	(void)pthread_setcancel (state);
	}
    CATCH (pthread_cancel_e) {
	cts_failed ("Pending cancel was raised by restoring cancel state");
	}
    ENDTRY;

    TRY {
	pthread_testcancel ();
	cts_failed ("Pending cancel was not raised by testcancel");
	}
    ENDTRY

    return (pthread_addr_t)0;
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element ERVB_CAN_001.C */
/*  *2    21-FEB-1991 00:38:10 BUTENHOF "Change ""fork"" function" */
/*  *1     8-FEB-1991 00:58:18 BUTENHOF "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element ERVB_CAN_001.C */
