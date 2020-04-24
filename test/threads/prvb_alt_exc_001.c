/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: prvb_alt_exc_001.c,v $
 * Revision 1.1.56.2  1996/02/17  23:31:44  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:48:12  marty]
 *
 * Revision 1.1.56.1  1995/12/11  23:03:13  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:27  root]
 * 
 * Revision 1.1.54.1  1994/04/01  20:11:49  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:19:02  jd]
 * 
 * Revision 1.1.52.1  1993/10/12  19:37:34  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:31:00  rhw]
 * 
 * Revision 1.1.4.2  1992/12/30  16:54:07  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:12:16  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/11  17:08:23  bolinger
 * 	Fix OT defect 2161:  provide/correct missing/bad cts_test() banners.
 * 	[1992/05/11  13:46:33  bolinger]
 * 
 * Revision 1.1  1992/01/19  03:19:39  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  Copyright (c) 1990 by
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
 *	PRVB_ALT_EXC_001
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
 *  001	    Paul Curtin	    30 May 1990
 *	    Converted test to use pthread interface.
 *  002	    Paul Curtin	    15 June 1990
 *	    Added a cast so as to work on Mips
 */

/*
 *  INCLUDE FILES
 */

#include <pthread.h>
#include <dce/cts_test.h>

static pthread_mutex_t	mu;
static int		flag;

#define FIRST_ALERT	1
#define SECOND_ALERT	2
#define NORMAL_EXIT	3

static void
clean1	(
	pthread_addr_t	arg)
    {
    int status;

    status = pthread_mutex_lock (&mu);
    check(status,"pthread_mutex_lock");
    flag = FIRST_ALERT;
    status = pthread_mutex_unlock (&mu);
    check(status,"pthread_mutex_unlock");
    }



static pthread_addr_t
thread	(
	pthread_addr_t	arg)
    {
    struct timespec delay;
    int status;

    pthread_cleanup_push(clean1, 0);
	while (1)
	    pthread_testcancel ();
    pthread_cleanup_pop(0);

    /*
     * If the second alert wasn't delivered, then we'll fall through to here;
     * set "normal exit" status, and exit.
     */
    status = pthread_mutex_lock (&mu);
    check(status,"pthread_mutex_lock");
    flag = NORMAL_EXIT;
    status = pthread_mutex_unlock (&mu);
    check(status,"pthread_mutex_unlock");
    }

main ()
    {
    struct timespec delay;
    pthread_t	thd;
    int status;
    int	join_status;
    pthread_addr_t	result;

    
			/* Note - Pthreads does not allow you to continue. */
    cts_test ("PRVB_ALT_EXC_001", "Test ability to handle alerts and continue");

    status = pthread_mutex_init (&mu, pthread_mutexattr_default);
    check(status,"pthread_mutex_init");
    flag = 0;
    status = pthread_create (
		&thd,
		pthread_attr_default,
		(pthread_startroutine_t)thread,
		(pthread_addr_t)0);
    check(status,"pthread_create");
    delay.tv_sec = 2;
    delay.tv_nsec = 0;
    pthread_delay_np (&delay);			/* Delay a bit to let it run */
    status = pthread_cancel (thd);		/* Alert it */
    check(status,"pthread_cancel");
    status = pthread_join (thd, (pthread_addr_t *)&join_status);
    check(status,"pthread_join");

    if (join_status != -1)
	cts_failed ("didn't terminate with alert status");
    else {
	switch (flag) {
	    case 0 : {
		cts_failed ("didn't receive first alert");
		break;
		}
	    case FIRST_ALERT : {
		break;
		}
	    case NORMAL_EXIT : {
		cts_failed ("didn't receive second alert.");
		}
	    }
	}

    cts_result ();

    }
