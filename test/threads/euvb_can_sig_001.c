/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: euvb_can_sig_001.c,v $
 * Revision 1.1.36.2  1996/02/17  23:31:22  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:54  marty]
 *
 * Revision 1.1.36.1  1995/12/11  23:02:05  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:10  root]
 * 
 * Revision 1.1.34.1  1994/04/01  20:11:19  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:18:01  jd]
 * 
 * Revision 1.1.32.1  1993/10/12  19:36:59  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:44  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:52:42  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:10:56  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:21:52  devrcs
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
 *	EUVB_CAN_SIG_001
 *
 *  TEST TOPICS:
 *
 *	CAN  -  Pthread cancel
 *	SIG  -  U*IX signal handling
 *
 *  ABSTRACT:
 *
 *	Make sure that the new pthread_signal_to_cancel() function
 *	works by creating a thread that uses it, raises a signal,
 *	and verifies that the target thread was canceled.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	02 April 1991
 *
 *  MODIFICATION HISTORY:
 *
 */

/*
 *  INCLUDE FILES
 */

#include <pthread_exc.h>
#include <dce/cts_test.h>
#include <signal.h>

pthread_addr_t
workcode	(pthread_addr_t	arg)
    {
    struct timespec timer;

    timer.tv_sec = 10;
    timer.tv_nsec = 0;
    pthread_delay_np (&timer);
    }

int
main	(int argc, char *argv[], char *envp[])
    {
    pthread_t		worker;
    pthread_addr_t	retval;
    sigset_t		sigset;
    struct timespec	timer;


    cts_test ("EUVB_CAN_SIG_001", "test pthread_signal_to_cancel()");

    sigemptyset (&sigset);
    sigaddset (&sigset, SIGINT);
    timer.tv_sec = 2;
    timer.tv_nsec = 0;
    pthread_create (&worker, pthread_attr_default, workcode, (pthread_addr_t)0);
    pthread_signal_to_cancel_np (&sigset, &worker);
    pthread_delay_np (&timer);
    kill (getpid (), SIGINT);
    pthread_join (worker, &retval);

    if (retval != (pthread_addr_t)-1)
	cts_failed ("worker wasn't canceled by signal");

    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element EUVB_CAN_SIG_001.C */
/*  *1     3-APR-1991 14:25:15 BUTENHOF "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element EUVB_CAN_SIG_001.C */
