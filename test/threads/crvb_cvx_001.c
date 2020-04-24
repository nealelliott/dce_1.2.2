/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crvb_cvx_001.c,v $
 * Revision 1.1.40.2  1996/02/17  23:30:06  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:46:51  marty]
 *
 * Revision 1.1.40.1  1995/12/11  22:58:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:10  root]
 * 
 * Revision 1.1.37.1  1994/04/01  20:09:21  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:13:28  jd]
 * 
 * Revision 1.1.35.1  1993/10/12  19:35:02  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:29:51  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:29:56  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:06:41  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:20:47  devrcs
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
 *	CMA Test Programs
 *
 *  FILENAME:
 *
 *	CRVB_CVX_001
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
 *	001	Dave Butenhof	4 September 1990
 *		Change to new CMA interfaces.
 */

/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <dce/cts_test.h>

#define THREADS	9

static cma_t_thread	thread[THREADS+1];

static struct {
    cma_t_mutex		mutex;
    cma_t_cond		cond;
    cma_t_integer	count;
    } barrier;

static void
thread_code	(int thd_id)
    {
    cma_t_status	status;
    cma_t_date_time	timeout;


    cma_mutex_lock (&barrier.mutex);
    cma_time_get_expiration (&timeout, 600.0);
    barrier.count--;

    if (barrier.count > 0) {

	while (barrier.count > 0) {
	    status = cma_cond_timed_wait (
		    &barrier.cond,
		    &barrier.mutex,
		    &timeout);

	    if (status == cma_s_timed_out)
		cts_failed (
			"thread %d was not awakened within 10 minutes.",
			thd_id);

	    }

	cma_mutex_unlock (&barrier.mutex);
	}
    else {
	cma_mutex_unlock (&barrier.mutex);
	cma_cond_broadcast (&barrier.cond);
	}	

    }

int
main	(int argc, char *argv[], char *envp[])
    {
    cma_t_integer	i;


    cts_test ("CRVB_CVX_001", "Test broadcast");
    cma_init ();

    cma_mutex_create (&barrier.mutex, &cma_c_null);
    cma_cond_create (&barrier.cond, &cma_c_null);
    barrier.count = THREADS;
    cma_thread_get_self (&thread[0]);

    for (i = 1; i <= THREADS; i++)
	cma_thread_create (
		&thread[i],
		&cma_c_null,
		(cma_t_start_routine)thread_code,
		(cma_t_address)i);

    thread_code (0);

    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_CVX_001.C */
/*  *2     5-SEP-1990 09:40:33 BUTENHOF "Convert to new CMA interfaces" */
/*  *1     6-AUG-1990 13:21:56 BUTENHOF "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_CVX_001.C */
