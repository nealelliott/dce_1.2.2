/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crvb_aqo_003.c,v $
 * Revision 1.1.6.2  1996/02/17  23:29:57  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:46:44  marty]
 *
 * Revision 1.1.6.1  1995/12/11  22:58:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:04  root]
 * 
 * Revision 1.1.4.1  1994/04/01  20:09:09  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:13:01  jd]
 * 
 * Revision 1.1.2.2  1992/12/30  16:29:21  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:06:06  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:20:37  devrcs
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
 *	CRVB_AQO_003
 *
 *  TEST TOPICS:
 *
 *	AQO  -  Atomic Queue Operations
 *
 *  ABSTRACT:
 *
 *	This test endeavors to show that queues can be used to 
 *	reliably pass data between (non-interrupt) threads.
 *
 *	This test has multiple threads all reading and writing the same queue.
 *
 *  AUTHORS:
 *
 *	Webb Scales
 *
 *  CREATION DATE:
 *
 *	30 August 1990
 *
 *  MODIFICATION HISTORY:
 *  001	    Paul Curtin	    21-March-1991
 *	    Added some casts to appease the mips cc.
 */


/*
 *  INCLUDE FILES
 */

#include <cma.h>
#include <cma_library.h>
#include <cts_test.h>


#define	THREAD_CNT  8


cma_lib_t_queue	input, output;
cma_t_thread	thread[THREAD_CNT];


cma_t_address
thread_routine	(cma_t_address	parm)
    {
    cma_t_address   item;
    cma_t_integer   count = 0;


    do	{
	cma_lib_queue_dequeue (&input, &item);
	cma_lib_queue_enqueue (&output, item);
	count++;
	} while (item != cma_c_null_ptr);

    return (cma_t_address)count;
    }


int
main	(int argc, char *argv[], char *envp[])
    {
    cma_t_integer   i;
    cma_t_integer   queue_size;
    cma_t_address   item;
    cma_t_exit_status	exit_status;
    cma_t_address   result;
    cma_t_integer   count, sum = 0;


    cts_test ("CRVB_AQO_003", "Test queues as synchronization primitives");
    cma_init ();

    cma_lib_queue_create (&input,  &cma_c_null);
    cma_lib_queue_create (&output, &cma_c_null);

    for (i = 0; i < THREAD_CNT; i++)
	cma_thread_create (
		&thread[i],
		&cma_c_null,
		thread_routine,
		(cma_t_address)i);

    /*
     * Let the threads block trying to remove from the input queue.
     */
    cma_yield ();

    /*
     * Pump in enough items to fill both queues exactly.  Make the last
     * THREAD_CNT items zero to tell the threads to quit.  Then pump out
     * all of the items.
     *
     * Because there are more items than will fit in the input queue, the
     * master thread should block once or twice, depending on where the
     * timeslices hit.
     */

    cma_lib_attr_get_queuesize ((cma_lib_t_attr  *)&cma_c_null, 
				(cma_t_natural   *)&queue_size);

    for (i = 1; i < 2*queue_size - THREAD_CNT; i++)
	cma_lib_queue_enqueue (&input, (cma_t_address)i);

    for (i = 0; i < THREAD_CNT; i++)
	cma_lib_queue_enqueue (&input, cma_c_null_ptr);

    for (i = 1; i < 2*queue_size; i++)
	cma_lib_queue_dequeue (&output, &item);

    for (i = 0; i < THREAD_CNT; i++) {
	cma_thread_join (
		&thread[i],
		&exit_status,
		(cma_t_address *)&count);

	if (exit_status != cma_c_term_normal)
	    cts_failed ("Bad exit status for thread #%d.", i);

	printf ("Thread #%d moved %d items.\n", i, (cma_t_integer)count);
	sum += (cma_t_integer)count;
	}

    if (sum != 2*queue_size - 1)
	cts_failed ("Enqueue/dequeue mismatch: %d/%d", 2*queue_size - 1, sum);
    if (cma_lib_queue_try_dequeue (&input, &item))
	cts_failed ("Input queue not empty, removed %d.", (cma_t_integer)item);
    if (cma_lib_queue_try_dequeue (&output, &item))
	cts_failed ("Output queue not empty, removed %d.", (cma_t_integer)item);

    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_AQO_003.C */
/*  *2    21-MAR-1991 17:35:07 CURTIN "added some casts to appease mips cc" */
/*  *1    30-AUG-1990 20:05:44 SCALES "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_AQO_003.C */
