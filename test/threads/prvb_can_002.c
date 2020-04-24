/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: prvb_can_002.c,v $
 * Revision 1.1.39.2  1996/02/17  23:31:48  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:48:16  marty]
 *
 * Revision 1.1.39.1  1995/12/11  23:03:26  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:31  root]
 * 
 * Revision 1.1.37.1  1994/04/01  20:11:53  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:19:11  jd]
 * 
 * Revision 1.1.35.1  1993/10/12  19:37:41  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:31:04  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:54:24  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:12:31  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:19:54  devrcs
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
 *	PTHREAD Test Programs
 *
 *  FILENAME:
 *
 *	PRVB_CAN_002
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
 *	004	Dave Butenhof	7 February 1991
 *		Cleanup: change cancel terminology to cancellation.
 *	005	Dave Butenhof	20 February 1991
 *		Change name of "fork" function
 */

/*
 *  INCLUDE FILES
 */

#include <pthread.h>
#include <dce/cts_test.h>

pthread_t	thread;

static pthread_addr_t
worker (pthread_addr_t);

main ()
    {
    int	join_status;
    int status;
    struct timespec delay;
    pthread_addr_t	result;


    cts_test ("PRVB_CAN_002", "Test general cancellation control");

    status = pthread_create (&thread, pthread_attr_default, worker, (pthread_addr_t)0);
    check(status,"pthread_create");
    delay.tv_nsec = 0;
    delay.tv_sec = 2;
    pthread_delay_np (&delay);			/* Give it a chance to go */
    status = pthread_cancel (thread);		/* Send it the cancel */
    check(status,"pthread_cancel");
    status = pthread_join (thread, (pthread_addr_t *)&join_status);
    check(status,"pthread_join");
    status = pthread_detach (&thread);
    check(status,"pthread_detach");

    if (join_status != -1)
	cts_failed ("Thread didn't terminate with cancel status");

    cts_result ();

    }

static void
cancel_during_wait	(pthread_addr_t	arg)
    {
    cts_failed ("Cancel was raised while general cancellation was off");
    }

static void
cancel_on_setcancel	(pthread_addr_t	arg)
    {
    cts_failed ("Pending cancel was raised by restoring cancel state");
    }

static pthread_addr_t
worker	(
	pthread_addr_t	arg)
    {
    int			state;
    struct timespec	delay;


    state = pthread_setcancel (CANCEL_OFF);

    pthread_cleanup_push (cancel_during_wait, (pthread_addr_t)0);
    delay.tv_sec = 5;
    delay.tv_nsec = 0;
    pthread_delay_np (&delay);		/* Delay for 5 seconds */
    pthread_cleanup_pop (0);

    pthread_cleanup_push (cancel_on_setcancel, (pthread_addr_t)0);
    (void)pthread_setcancel (state);
    pthread_cleanup_pop (0);

    pthread_testcancel ();
    cts_failed ("Pending cancel was not raised by testcancel");

    return (pthread_addr_t)0;
    }
