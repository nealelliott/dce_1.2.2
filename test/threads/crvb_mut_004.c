/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crvb_mut_004.c,v $
 * Revision 1.1.41.2  1996/02/17  23:30:18  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:02  marty]
 *
 * Revision 1.1.41.1  1995/12/11  22:59:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:20  root]
 * 
 * Revision 1.1.39.1  1994/04/01  20:09:40  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:14:14  jd]
 * 
 * Revision 1.1.37.1  1993/10/12  19:35:23  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:00  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:30:58  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:07:37  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:21:01  devrcs
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
 *	CRVB_MUT_004
 *
 *  TEST TOPICS:
 *
 *	MUT  -  Mutex Operations
 *
 *  ABSTRACT:
 *
 *	Ensure that attempting to relock a nonrecursive mutex raises the
 *	cma_e_in_use error, that attempting to unlock a mutex the thread
 *	doesn't own raises cma_e_in_use.  Otherwise, ensure that nonrecursive
 *	mutexes provide mutual exclusion.
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

cma_t_mutex	mu;
cma_t_thread	timeout, worker;
cma_t_attr	attr;
cma_t_boolean	blocker_flag;

typedef struct TIMEOUT_INFO {
    cma_t_thread	target;
    cma_t_interval	timeout;
    } timeout_info;

static cma_t_address
timer	(cma_t_address	arg)
    {
    timeout_info	*ti = (timeout_info *)arg;


    cma_delay (ti->timeout);
    cma_thread_alert (&ti->target);
    }

static cma_t_address
unlocker	(cma_t_address	arg)
    {
    TRY {
	cma_mutex_unlock (&mu);
	cts_failed ("unlocking mutex owned by another thread didn't raise error");
	}
    CATCH (cma_e_in_use) {
	cts_comment ("	...received expected exception");
	}
    ENDTRY
    }

static cma_t_address
blocker	(cma_t_address	arg)
    {
    cma_mutex_lock (&mu);

    if (blocker_flag)
	cts_failed ("locked mutex didn't block thread");

    cma_mutex_unlock (&mu);
    }

int
main	(int argc, char *argv[], char *envp[])
    {
    cma_t_exit_status	status;
    cma_t_address	result;
    timeout_info	tinf;


    cts_test ("CRVB_MUT_004", "Test nonrecursive mutex");
    cma_init ();
    cma_attr_create (&attr, &cma_c_null);
    cma_attr_set_mutex_kind (&attr, cma_c_mutex_nonrecursive);
    cma_mutex_create (&mu, &attr);

    /*
     * Test 1:
     *
     * 1. Lock the mutex
     * 2. Create a timer thread which will alert the main thread after 5
     *    seconds, in case the relock hangs.
     * 3. Attempt to lock the mutex again.
     * 4. If the attempt succeeds, report failure.
     * 5. If the cma_e_in_use error is received, report success.
     * 6. If alerted by the timer thread, report failure due to hang.
     * 7. Alert the timer thread and join with it.
     * 8. Unlock the mutex.
     */
    cts_comment ("test 1: relocking mutex");

    cma_thread_get_self (&tinf.target);
    tinf.timeout = 5.0;
    cma_mutex_lock (&mu);
    cma_thread_create (&timeout, &cma_c_null, timer, (cma_t_address)&tinf);
    TRY {
	TRY {
	    cma_mutex_lock (&mu);

	    /*
	     * (of course, it's more likely to simply hang here, which is
	     * what the timer thread is for).
	     */
	    cts_failed ("relocking nonrecursive mutex didn't raise error");
	    }
	CATCH (cma_e_in_use) {
	    cts_comment ("	...received expected exception");
	    }
	CATCH (cma_e_alerted) {		/* In case we time out */
	    cts_failed ("relocking nonrecursive mutex hung");
	    }
	ENDTRY
	cma_thread_alert (&timeout);	/* Kill the timer thread */
	}
    CATCH_ALL {
	}
    ENDTRY
    cma_thread_join (&timeout, &status, &result);
    cma_mutex_unlock (&mu);

    /*
     * Test 2:
     *
     * 1. Attempt to unlock the mutex (there is no owner).
     * 2. If an exception isn't raised, report an error.
     * 3. If the cma_e_use_error exception is reported, declare success with
     *    a comment.
     */
    cts_comment ("test 2: unlocking unowned mutex");

    TRY {
	cma_mutex_unlock (&mu);
	cts_failed ("unlocking unowned mutex didn't raise error");
	}
    CATCH (cma_e_use_error) {
	cts_comment ("	...received expected exception");
	}
    ENDTRY

    /*
     * Test 3:
     *
     * 1. Lock the mutex.
     * 2. Create a worker thread which will attempt to unlock the mutex; it
     *    will report an error if it doesn't receive the cma_e_in_use
     *    exception from the attempt.
     * 3. Join with the worker thread.
     * 4. Unlock the mutex.
     */
    cts_comment ("test 3: unlocking mutex owned by another thread");

    cma_mutex_lock (&mu);
    cma_thread_create (&worker, &cma_c_null, unlocker, cma_c_null_ptr);
    cma_thread_join (&worker, &status, &result);
    cma_mutex_unlock (&mu);

    /*
     * Test 4:
     *
     * 1. Lock the mutex
     * 2. Set the "blocker flag" to true; if the worker sees this set on
     * 	  return from cma_mutex_lock, it knows the block failed.
     * 3. Create a worker thread which will attempt to lock the mutex, then
     *    look at the blocker flag and report an error if it's set.
     * 4. Wait for 5 seconds to ensure that the worker has time to block.
     * 5. Clear the blocker flag and release the mutex to awaken the worker.
     * 6. Create a timer thread which will alert the worker after 5 seconds,
     *    in case the unlock fails to release the worker.
     * 7. Join with the worker thread.
     * 8. Alert the timer thread and join with it.
     */
    cts_comment ("test 4: make sure mutual exclusion works");

    cma_mutex_lock (&mu);
    blocker_flag = cma_c_true;
    cma_thread_create (&tinf.target, &cma_c_null, blocker, cma_c_null_ptr);
    tinf.timeout = 5.0;
    cma_delay (5.0);
    blocker_flag = cma_c_false;
    cma_mutex_unlock (&mu);
    cma_thread_create (&timeout, &cma_c_null, timer, (cma_t_address)&tinf);
    cma_thread_join (&worker, &status, &result);
    TRY {				/* Don't worry if it's gone already */
	cma_thread_alert (&timeout);
	}
    CATCH_ALL {
	}
    ENDTRY
    cma_thread_join (&timeout, &status, &result);

    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_MUT_004.C */
/*  *2    13-FEB-1991 18:00:39 BUTENHOF "Change names of mutex attributes" */
/*  *1    12-FEB-1991 22:55:35 BUTENHOF "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_MUT_004.C */
