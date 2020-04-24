/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ervb_cvx_001.c,v $
 * Revision 1.1.44.2  1996/02/17  23:31:21  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:52  marty]
 *
 * Revision 1.1.44.1  1995/12/11  23:01:59  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:08  root]
 * 
 * Revision 1.1.42.1  1994/04/01  20:11:15  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:17:53  jd]
 * 
 * Revision 1.1.40.1  1993/10/12  19:36:56  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:43  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:52:33  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:10:49  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:21:51  devrcs
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
 *	ERVB_CVX_001
 *
 *  TEST TOPICS:
 *
 *	CVX  -  Condition Variable Operations
 *
 *  ABSTRACT:
 *
 *	Test condition variable operations by implementing a barrier.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	04 February 1991
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	4 February 1991
 *		Include cma_errno.h for EAGAIN
 */

/*
 *  INCLUDE FILES
 */

#include <pthread_exc.h>
#include <dce/cts_test.h>
#include <cma_errno.h>

#define THREADS	9

static pthread_t	thread[THREADS+1];

static struct {
    pthread_mutex_t	mutex;
    pthread_cond_t	cond;
    int			count;
    } barrier;

static void
thread_code	(int thd_id)
    {
    int			status;
    struct timespec	timeout, delta;


    pthread_mutex_lock (&barrier.mutex);
    delta.tv_nsec = 0;
    delta.tv_sec = 600;
    pthread_get_expiration_np (&delta, &timeout);
    barrier.count--;

    if (barrier.count > 0) {

	while (barrier.count > 0) {
	    status = pthread_cond_timedwait (
		    &barrier.cond,
		    &barrier.mutex,
		    &timeout);

	    if (status == EAGAIN)
		cts_failed (
			"thread %d was not awakened within 10 minutes.",
			thd_id);

	    }

	pthread_mutex_unlock (&barrier.mutex);
	}
    else {
	pthread_mutex_unlock (&barrier.mutex);
	pthread_cond_broadcast (&barrier.cond);
	}

    }

int
main	(int argc, char *argv[], char *envp[])
    {
    int		i;


    cts_test ("ERVB_CVX_001", "Test condition variable operations");

    pthread_mutex_init (&barrier.mutex, pthread_mutexattr_default);
    pthread_cond_init (&barrier.cond, pthread_condattr_default);
    barrier.count = THREADS;
    thread[0] = pthread_self ();

    for (i = 1; i <= THREADS; i++)
	pthread_create (
		&thread[i],
		pthread_attr_default,
		(pthread_startroutine_t)thread_code,
		(pthread_addr_t)i);

    thread_code (0);

    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element ERVB_CVX_001.C */
/*  *2     5-FEB-1991 19:45:05 BUTENHOF "Include cma_errno.h" */
/*  *1     4-FEB-1991 22:26:30 BUTENHOF "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element ERVB_CVX_001.C */
