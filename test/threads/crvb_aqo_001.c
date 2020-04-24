/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crvb_aqo_001.c,v $
 * Revision 1.1.6.2  1996/02/17  23:29:55  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:46:42  marty]
 *
 * Revision 1.1.6.1  1995/12/11  22:58:25  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:02  root]
 * 
 * Revision 1.1.4.1  1994/04/01  20:09:06  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:12:53  jd]
 * 
 * Revision 1.1.2.2  1992/12/30  16:29:12  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:05:58  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:20:34  devrcs
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
 *	CRVB_AQO_001
 *
 *  TEST TOPICS:
 *
 *	AQO  -  Atomic Queue Operations
 *
 *  ABSTRACT:
 *
 *	This program tests atomic queue "try" operations, using a single
 *	thread and no interrupts.
 *
 *  AUTHORS:
 *
 *	Webb Scales
 *
 *  CREATION DATE:
 *
 *	28 August 1990
 *
 *  MODIFICATION HISTORY:
 *
 */


/*
 *  INCLUDE FILES
 */

#include <cma.h>
#include <cts_test.h>
#include <cma_library.h>

int
main	(int argc, char *argv[], char *envp[])
    {
    cma_lib_t_queue queue_1, queue_2;
    cma_lib_t_attr  attr;
    cma_t_natural   size_1, size_2;
    cma_t_address   item_1, item_2;
    cma_t_integer   i;


    cts_test ("CRVB_AQO_001", "Queue \"try\" operations:  single thread, no interrupts");
    cma_init ();

    /*
     * Create an attributes object.  Verify that we can get and set the queue
     * size attribute.
     */
    cma_lib_attr_create (&attr, &cma_c_null);

    cma_lib_attr_get_queuesize (&attr, &size_1);
    printf ("Default queue size is %d.\n", size_1);
    if ((cma_t_integer)size_1 <= 0)  cts_failed ("Queue size is too small.");

    cma_lib_attr_set_queuesize (&attr, 2*size_1);
    cma_lib_attr_get_queuesize (&attr, &size_2);
    if (size_2 != 2*size_1) 
	cts_failed (
		"Queue size 'set' failed: set to %d, got %d.", 
		2*size_1, 
		size_2);

    /*
     * Create queues with both default and new attributes objects.  Make sure
     * they're initially empty.
     */
    cma_lib_queue_create (&queue_1, &cma_c_null);
    cma_lib_queue_create (&queue_2, &attr);

    if (cma_lib_queue_try_dequeue (&queue_1, &item_1))
	cts_failed ("Successful try_dequeue on an initial (empty) queue (default-sized) .");
    if (cma_lib_queue_try_dequeue (&queue_2, &item_2))
	cts_failed ("Successful try_dequeue on an initial (empty) queue (specific-sized) .");

    /*
     * Fill them up enqueue.  Verify that the right number of items fit.
     */
    i = 0;
    while (cma_lib_queue_try_enqueue (&queue_1, (cma_t_address)i)) i++;
    if (i != size_1) 
	cts_failed (
		"Queue allocation (%d) doesn't match \"size\" (%d) [def/enq].", 
		i, 
		size_1);
    i = 0;
    while (cma_lib_queue_try_enqueue (&queue_2, (cma_t_address)i)) i++;
    if (i != size_2) 
	cts_failed (
		"Queue allocation (%d) doesn't match \"size\" (%d) [spec/enq].", 
		i, 
		size_2);

    /*
     * Verify that requeue on an enqueue'd-full queue doesn't work.
     */
    if (cma_lib_queue_try_requeue (&queue_1, item_1))
	cts_failed ("Successful try_requeue on an enqueue'd-full queue (default-sized) .");
    if (cma_lib_queue_try_requeue (&queue_2, item_2))
	cts_failed ("Successful try_requeue on an enqueue'd-full queue (specific-sized) .");

    /*
     * Empty them out.  Verify that the right number of items come out in the 
     * right order.
     */
    i = 0;
    while (cma_lib_queue_try_dequeue (&queue_1, &item_1))
	if (i++ != (cma_t_integer)item_1) {
	    cts_failed (
		    "Queue out of order:  got %d when expecting %d. [enq/deq/def]",
		    (cma_t_integer)item_1,
		    i);
	    break;
	    }
    if (i != size_1) 
	cts_failed (
		"Queue allocation (%d) doesn't match \"size\" (%d). [enq/deq/def]", 
		i, 
		size_1);

    i = 0;
    while (cma_lib_queue_try_dequeue (&queue_2, &item_2))
	if (i++ != (cma_t_integer)item_2) {
	    cts_failed (
		    "Queue out of order:  got %d when expecting %d. [enq/deq/spec]",
		    (cma_t_integer)item_2,
		    i);
	    break;
	    }
    if (i != size_2) 
	cts_failed (
		"Queue allocation (%d) doesn't match \"size\" (%d). [enq/deq/spec]", 
		i, 
		size_2);

    /*
     * Fill them up requeue.  Verify that the right number of items fit.
     */
    i = 0;
    while (cma_lib_queue_try_requeue (&queue_1, (cma_t_address)i)) i++;
    if (i != size_1) 
	cts_failed (
		"Queue allocation (%d) doesn't match \"size\" (%d) [def/req].", 
		i, 
		size_1);
    i = 0;
    while (cma_lib_queue_try_requeue (&queue_2, (cma_t_address)i)) i++;
    if (i != size_2) 
	cts_failed (
		"Queue allocation (%d) doesn't match \"size\" (%d) [spec/req].", 
		i, 
		size_2);

    /*
     * Verify that requeue on an requeue'd-full queue doesn't work.
     */
    if (cma_lib_queue_try_enqueue (&queue_1, item_1))
	cts_failed ("Successful try_enqueue on an requeue'd-full queue (default-sized) .");
    if (cma_lib_queue_try_enqueue (&queue_2, item_2))
	cts_failed ("Successful try_enqueue on an requeue'd-full queue (specific-sized) .");

    /*
     * Empty them out.  Verify that the right number of items come out in the 
     * right order.
     */
    i = size_1;
    while (cma_lib_queue_try_dequeue (&queue_1, &item_1))
	if (--i != (cma_t_integer)item_1) {
	    cts_failed (
		    "Queue out of order:  got %d when expecting %d. [req/deq/def]",
		    (cma_t_integer)item_1,
		    i);
	    break;
	    }
    if (i != 0) 
	cts_failed (
		"Queue allocation (%d) doesn't match \"size\" (%d). [req/deq/def]", 
		i, 
		size_1);

    i = size_2;
    while (cma_lib_queue_try_dequeue (&queue_2, &item_2))
	if (--i != (cma_t_integer)item_2) {
	    cts_failed (
		    "Queue out of order:  got %d when expecting %d. [req/deq/spec]",
		    (cma_t_integer)item_2,
		    i);
	    break;
	    }
    if (i != 0) 
	cts_failed (
		"Queue allocation (%d) doesn't match \"size\" (%d). [req/deq/spec]", 
		i, 
		size_2);

    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_AQO_001.C */
/*  *1    30-AUG-1990 16:20:30 SCALES "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_AQO_001.C */
