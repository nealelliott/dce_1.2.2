/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: prvb_cvx_001.c,v $
 * Revision 1.1.42.2  1996/02/17  23:31:51  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:48:18  marty]
 *
 * Revision 1.1.42.1  1995/12/11  23:03:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:33  root]
 * 
 * Revision 1.1.40.1  1994/04/01  20:11:59  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:19:24  jd]
 * 
 * Revision 1.1.38.1  1993/10/12  19:37:46  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:31:06  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:54:35  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:12:43  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:22:06  devrcs
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
 *	Pthread Test Programs
 *
 *  FILENAME:
 *
 *	PRVB_CVX_001
 *
 *  TEST TOPICS:
 *
 *	CVX  -  Condition Variable Operations
 *
 *  ABSTRACT:
 *
 *	Test condition broadcast by implementing a barrier (the last
 *	thread to reach the barrier releases all threads by doing a
 *	broadcast on a condition variable).
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	06 August 1990
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	11 October 1990
 *		Use cma_errno.h
 */

/*
 *  INCLUDE FILES
 */

#include <pthread.h>
#include <cma_errno.h>
#include <dce/cts_test.h>

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


    status = pthread_mutex_lock (&barrier.mutex);
    check (status, "pthread_mutex_lock");
    delta.tv_sec = 10;
    delta.tv_nsec = 0;
    status = pthread_get_expiration_np (&delta, &timeout);
    check (status, "pthread_get_expiration_np");
    barrier.count--;

    if (barrier.count > 0) {

	while (barrier.count > 0) {
	    status = pthread_cond_timedwait (&barrier.cond, &barrier.mutex, &timeout);

	    if (status == -1) {

		if (errno == EAGAIN)
		    cts_failed (
			    "thread %d was not awakened within 10 minutes.",
			    thd_id);
		else
		    check (status, "pthread_cond_timedwait");

		}

	    }

	status = pthread_mutex_unlock (&barrier.mutex);
	check (status, "pthread_mutex_unlock");
	}
    else {
	status = pthread_mutex_unlock (&barrier.mutex);
	check (status, "pthread_mutex_unlock");
	status = pthread_cond_broadcast (&barrier.cond);
	check (status, "pthread_cond_broadcast");
	}	

    }

int
main	(int argc, char *argv[], char *envp[])
    {
    int	i, status;


    cts_test ("PRVB_CVX_001", "Test broadcast");

    status = pthread_mutex_init (&barrier.mutex, pthread_mutexattr_default);
    check (status, "pthread_mutex_init");
    status = pthread_cond_init (&barrier.cond, pthread_condattr_default);
    check (status, "pthread_cond_init");
    barrier.count = THREADS;
    thread[0] = pthread_self ();

    for (i = 1; i <= THREADS; i++) {
	status = pthread_create (
		&thread[i],
		pthread_attr_default,
		(pthread_startroutine_t)thread_code,
		(pthread_addr_t)i);
	check (status, "pthread_create");
	}

    thread_code (0);

    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element PRVB_CVX_001.C */
/*  *2    11-OCT-1990 13:04:25 BUTENHOF "Fix use of errno" */
/*  *1     6-AUG-1990 13:46:29 BUTENHOF "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element PRVB_CVX_001.C */
