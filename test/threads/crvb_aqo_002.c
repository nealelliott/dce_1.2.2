/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crvb_aqo_002.c,v $
 * Revision 1.1.6.2  1996/02/17  23:29:56  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:46:43  marty]
 *
 * Revision 1.1.6.1  1995/12/11  22:58:27  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:03  root]
 * 
 * Revision 1.1.4.1  1994/04/01  20:09:07  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:12:57  jd]
 * 
 * Revision 1.1.2.2  1992/12/30  16:29:17  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:06:02  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:20:35  devrcs
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
 *	CRVB_AQO_002
 *
 *  TEST TOPICS:
 *
 *	AQO  -  Atomic Queue Operations
 *
 *  ABSTRACT:
 *
 *	This is a simple test to demonstrate that blocking queue services
 *	work as long as the thread never actually blocks.
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



int
main	(int argc, char *argv[], char *envp[])
    {
    cma_lib_t_queue queue;
    cma_t_integer   queue_size;
    cma_t_address   item;
    cma_t_integer   i, j, k;


    cts_test ("CRVB_AQO_002", "(non-blocking) blocking queue services test");
    cma_init ();

    cma_lib_queue_create (&queue, &cma_c_null);
    cma_lib_attr_get_queuesize ((cma_lib_t_attr *)&cma_c_null, 
				(cma_t_natural *)&queue_size);

    for (i = 1; i <= queue_size; i++) {
	printf ("Starting pass #%d of %d.\n", i, queue_size);

	for (j = 1; j <= 2*queue_size; j++) {
	    for (k = 1; k <= i; k++)
		cma_lib_queue_enqueue (&queue, (cma_t_address)k);

	    for (k = 1; k <= i; k++) {
		cma_lib_queue_dequeue (&queue, &item);
		if (item != (cma_t_address)k)
		    cts_failed ("Got %d when expecting %d", item, k);
		}

	    for (k = 1; k <= i; k++)
		cma_lib_queue_requeue (&queue, (cma_t_address)k);

	    for (k = i; k >= 1; k--) {
		cma_lib_queue_dequeue (&queue, &item);
		if (item != (cma_t_address)k)
		    cts_failed ("Got %d when expecting %d", item, k);
		}
	    }
	}

    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_AQO_002.C */
/*  *3    21-MAR-1991 17:21:34 CURTIN "added some casts to appease mips cc" */
/*  *2    21-MAR-1991 17:02:21 CURTIN "Convert to stream format for ULTRIX build" */
/*  *1    30-AUG-1990 18:27:01 SCALES "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_AQO_002.C */
