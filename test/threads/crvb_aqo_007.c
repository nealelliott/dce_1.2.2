/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crvb_aqo_007.c,v $
 * Revision 1.1.6.2  1996/02/17  23:30:01  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:46:48  marty]
 *
 * Revision 1.1.6.1  1995/12/11  22:58:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:07  root]
 * 
 * Revision 1.1.4.1  1994/04/01  20:09:17  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:13:19  jd]
 * 
 * Revision 1.1.2.2  1992/12/30  16:29:41  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:06:21  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:20:43  devrcs
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
 *	CRVB_AQO_007
 *
 *  TEST TOPICS:
 *
 *	AQO  -  Atomic Queue Operations
 *
 *  ABSTRACT:
 *
 *	This test endeavors to show that queues can be used to 
 *	reliably pass data between various threads.
 *
 *	This test has multiple threads all reading and writing the same 
 *	queue in a tight loop.  And interrupt periodically inserts items
 *	into the queue.
 *
 *  AUTHORS:
 *
 *	Webb Scales
 *
 *  CREATION DATE:
 *
 *	4 September 1990
 *
 *  MODIFICATION HISTORY:
 *  001	    Paul Curtin	    21-March-1991
 *	    Added a cast to appease the mips cc
 *  002	    Paul Curtin	    27-March-1991
 *	    Added unix siglongjmp to get out of signal handler.
 */


/*
 *  INCLUDE FILES
 */

#include <cma.h>
#include <cma_library.h>
#include <cts_test.h>
#include <signal.h>
#include <setjmp.h>


#define ITEM_CNT    20
#define	THREAD_CNT  8


cma_lib_t_queue	queue;
cma_t_thread	thread[THREAD_CNT];
#ifdef unix
 sigjmp_buf      signal_hopper;
#endif 

#if _CMA_OS_ == _CMA__UNIX
#define	SIG_NUM	SIGUSR1

# define setup_interrupt()  \
	if ((int)(signal (SIG_NUM, interrupt)) == -1)	cts_failed ("Error in 'signal'")

# define cause_interrupt()  \
	if (kill (getpid (), SIG_NUM) == -1)	cts_failed ("Error in 'kill'")
#endif

#if _CMA_OS_ == _CMA__VMS
# define setup_interrupt()

# define cause_interrupt()  \
	if (!(sys$dclast (interrupt, 0, 0) & 1)) cts_failed ("Error in $DCLAST")
#endif


void
interrupt(
	int sig, 
	int code,
	struct sigcontext *scp)
    {
    if (!cma_lib_queue_try_enqueue_int (&queue, cma_c_null_ptr))
	cts_failed ("Enqueue at interrupt failed unexpectedly.");
#ifdef unix
    siglongjmp (signal_hopper, 1);
#endif
    }


cma_t_address
thread_routine_1	(cma_t_address	parm)
    {
    cma_t_address   item;
    cma_t_integer   i;
    cma_t_integer   count = 0;
    cma_t_boolean   spin;


    for (i = 0; i < ITEM_CNT; i++) {
	spin = cma_c_true;
	while (spin)  {
	    count++;

	    /*
	     * Work until we get an item.
	     */
	    while (!cma_lib_queue_try_dequeue (&queue, &item));

	    /*
	     * If it is marked for us, mark it for the next guy and
	     * exit this inner loop.
	     */
	    if (item == parm) {
		item = (cma_t_address)((cma_t_integer)item + 1);
		spin = cma_c_false;
		}

	    /*
	     * Put it back
	     */
	    while (!cma_lib_queue_try_enqueue (&queue, item));

	    } 

	/*
	 * We just stuck something into the queue, give its owner a chance to
	 * get it.
	 */
	cma_yield ();
	}

    return (cma_t_address)count;
    }


cma_t_address
thread_routine_2	(cma_t_address	parm)
    {
    cma_t_address   item;
    cma_t_integer   i;
    cma_t_integer   count = 0;
    cma_t_boolean   spin;


    for (i = 0; i < ITEM_CNT; i++) {
	spin = cma_c_true;
	while (spin)  {
	    count++;

	    /*
	     * Get an item.
	     */
	    cma_lib_queue_dequeue (&queue, &item);

	    /*
	     * If it is marked for us, mark it for the next guy and
	     * exit this inner loop.
	     */
	    if (item == parm) {
		item = (cma_t_address)((cma_t_integer)item + 1);
		spin = cma_c_false;
		}

	    /*
	     * Put it back on the front of the queue
	     */
	    cma_lib_queue_requeue (&queue, item);

	    /*
	     * We just stuck something into the queue, give its owner a chance 
	     * to get it.  (No point in our looking at it, its on the front!)
	     */
	    cma_yield ();

	    }
	}

    return (cma_t_address)count;
    }


int
main	(int argc, char *argv[], char *envp[])
    {
    cma_t_integer   i, j;
    cma_t_address   item;
    cma_t_exit_status	exit_status;
    cma_t_address   result;
    cma_t_integer   queue_length = 0;


    cts_test ("CRVB_AQO_007", "Stress test queues as synchronization primitives");
    cma_init ();

    cma_lib_queue_create (&queue,  &cma_c_null);
    setup_interrupt();

    for (i = 0; i < THREAD_CNT; i++)
	cma_thread_create (
		&thread[i],
		&cma_c_null,
		(i % 3 ? thread_routine_1 : thread_routine_2),
		(cma_t_address)i);

    /*
     * Let the threads get started trying to remove from an empty queue.
     */
    cma_yield ();


    for (i = 0; i < ITEM_CNT; i++) {
	/*
	 * Cause the interrupt routine to execute (ie, enqueue an item)
	 */

#ifdef unix
	/* Save the current location and signal mask */
	if (!sigsetjmp (signal_hopper, cma_c_true))
#endif
	cause_interrupt ();
	printf (
		"-> Queue length is now %d (%d of %d items enqueued).\n", 
		++queue_length,
		i + 1,
		ITEM_CNT);

	/*
	 * Get an item.
	 */
	cma_lib_queue_dequeue (&queue, &item);

	/*
	 * See if everyone has touched it.
	 * Put it back on the front of the queue if they haven't.
	 */
	if (item != (cma_t_address)THREAD_CNT)
	    cma_lib_queue_enqueue (&queue, item);
	else
	    printf (
		    "<- Queue length is now %d (%d of %d items enqueued).\n", 
		    --queue_length,
		    i + 1,
		    ITEM_CNT);

	/*
	 * We just stuck something into the queue, give its owner a chance 
	 * to get it.  (No point in our looking at it, its on the front!)
	 */
	for (j = 0; j < queue_length; j++)
	    cma_yield ();
	}

    while (queue_length > 0) {
	/*
	 * Get an item.
	 */
	cma_lib_queue_dequeue (&queue, &item);

	/*
	 * See if everyone has touched it.
	 * Put it back on the front of the queue if they haven't.
	 */
	if (item != (cma_t_address)THREAD_CNT)
	    cma_lib_queue_enqueue (&queue, item);
	else
	    printf (
		    "<= Queue length is now %d (draining).\n", 
		    --queue_length);

	/*
	 * We just stuck something into the queue, give its owner a chance 
	 * to get it.  (No point in our looking at it, its on the front!)
	 */
	for (j = 0; j < queue_length; j++)
	    cma_yield ();
	}

    /*
     * Now wait for them all to come home
     */
    printf ("Joining...\n");
    for (i = 0; i < THREAD_CNT; i++) {
	cma_thread_join (
		&thread[i],
		&exit_status,
		&result);

	if (exit_status != cma_c_term_normal)
	    cts_failed ("Bad exit status for thread #%d.", i);

	printf (
		"Thread #%d spun %d times per item.\n", 
		i,
		((cma_t_integer)result)/ITEM_CNT);
	}

    if (cma_lib_queue_try_dequeue (&queue, &item))
	cts_failed ("Output queue not empty, removed %d.", (cma_t_integer)item);

    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_AQO_007.C */
/*  *3    27-MAR-1991 11:36:30 CURTIN "added siglongjmp" */
/*  *2    21-MAR-1991 17:40:44 CURTIN "added some casts to appease mips cc" */
/*  *1     4-SEP-1990 13:21:54 SCALES "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_AQO_007.C */
