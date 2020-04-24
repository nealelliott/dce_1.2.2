/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crvb_aqo_006.c,v $
 * Revision 1.1.6.2  1996/02/17  23:30:00  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:46:47  marty]
 *
 * Revision 1.1.6.1  1995/12/11  22:58:39  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:06  root]
 * 
 * Revision 1.1.4.1  1994/04/01  20:09:15  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:13:14  jd]
 * 
 * Revision 1.1.2.2  1992/12/30  16:29:36  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:06:17  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:20:41  devrcs
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
 *	CRVB_AQO_006
 *
 *  TEST TOPICS:
 *
 *	AQO  -  Atomic Queue Operations
 *
 *  ABSTRACT:
 *
 *	This test endeavors to show that queues can be used to pass 
 *	data reliably between interrupt threads and non-interrupt threads.
 *
 *  AUTHORS:
 *
 *	Webb Scales
 *
 *  CREATION DATE:
 *
 *	31 August 1990
 *
 *  MODIFICATION HISTORY:
 *  001	    Paul Curtin	    21-March-1991
 *	    Added some casts to appease mips cc
 */


/*
 *  INCLUDE FILES
 */

#include <cma.h>
#include <cma_library.h>
#include <cts_test.h>
#if _CMA_OS_ == _CMA__UNIX
#include <signal.h>
#endif


cma_lib_t_queue	queue;
cma_t_integer	q_len = 0;
cma_t_integer	q_max;


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
    static cma_t_integer	item_no = 0;


    if (cma_lib_queue_try_enqueue_int (&queue, (cma_t_address)(++item_no)) ==
	    (q_len >= q_max)) {
	if (q_len >= q_max) {
	    cts_failed (
		    "Enqueue at interrupt succeeded unexpectedly, item #%d.",
		    item_no);
	    }
	else
	    cts_failed (
		    "Enqueue at interrupt failed unexpectedly, item #%d.",
		    item_no);

	printf ("\t\tqueue length: %d, queue size: %d.\n", q_len, q_max);
	}
    }


int
main	(int argc, char *argv[], char *envp[])
    {
    cma_t_integer   i;
    cma_t_address   item;


    cts_test ("CRVB_AQO_006", "Simple use of queues from interrupt level");
    cma_init ();

    cma_lib_attr_get_queuesize ((cma_lib_t_attr  *)&cma_c_null, 
				(cma_t_natural   *)&q_max);

    cma_lib_queue_create (&queue,  &cma_c_null);
    setup_interrupt();

    /*
     * Cause items to be inserted into the queue until it overflows.
     */
    for (q_len = 0; q_len <= q_max; q_len++) {
	/*
	 * Cause the interrupt routine to execute (ie, enqueue an item)
	 */
	cause_interrupt ();

	/*
	 * Cycle through the queue and see if all the items are still in the 
	 * right places.
	 */
	for (i = 1; (i <= q_len + 1) && (i <= q_max); i++) {
	    /*
	     * Grab the item off the front of the queue.
	     */
	    cma_lib_queue_dequeue (&queue, &item);

	    /*
	     * Check if it is the right item.
	     */
	    if ((cma_t_integer)item != i) {
		cts_failed ("Got item #%d when expecting #%d.", item, i);
		q_len = i = 2 * q_max;	/* Exit both loops now */
		}

	    /*
	     * Put this item back, at the rear of the queue
	     */
	    cma_lib_queue_enqueue (&queue, item);
	    }
	}

    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_AQO_006.C */
/*  *2    21-MAR-1991 17:39:48 CURTIN "added some casts to appease mips cc" */
/*  *1    31-AUG-1990 17:26:46 SCALES "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_AQO_006.C */




