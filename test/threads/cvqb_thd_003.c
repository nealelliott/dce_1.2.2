/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cvqb_thd_003.c,v $
 * Revision 1.1.4.2  1996/02/17  23:31:15  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:47  marty]
 *
 * Revision 1.1.4.1  1995/12/11  23:01:43  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:04  root]
 * 
 * Revision 1.1.2.2  1992/12/30  16:52:10  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:10:30  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:21:45  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  Copyright (c) 1989, 1990 by
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
 *	CVQB_THD_003.C
 *
 *  TEST TOPICS:
 *
 *	THD  -  Thread Operations
 *
 *  ABSTRACT:
 *
 *	This test measures thread context switch peformance.  Context switch is 
 *	caused by blocking on a semaphore.
 *
 *  AUTHORS:
 *
 *	Webb Scales
 *
 *  CREATION DATE:
 *
 *	3 October 1989
 *
 *  MODIFICATION HISTORY:
 *
 *  001	    Paul Curtin	    4 September 1990
 *	    Adjust test name in cts_test.
 */

/*
 *  INCLUDE FILES
 */

#include <cma.h>
#include <cma_defs.h>
#include <cma_dispatch.h>
#include <cma_semaphore.h>
#include <cma_stack.h>
#include <cts_test.h>

#define	iterations 100000


cma__t_semaphore    sem;
cma__t_int_tcb	    *tcb;


main ()
    {
    int i;


    cts_test ("CVQB_THD_003", "Semaphore nonblocking performance measurement");

    cma_init ();

    cma__init_semaphore (&sem, 0);

    tcb = cma__get_self_tcb ();

    printf("\nStarting...\n\n");
    lib$init_timer (0);

    for (i = 0;  i < iterations/2;  i++)
	{
	cma__increment_semaphore (&sem, 1);
	cma__enter_kernel ();
	cma__decrement_semaphore (&sem, tcb, 1);
	cma__exit_kernel ();
	}

    printf("For %d iterations of increment/decrement (nonblocking) semaphore:\n",
	    iterations);
    lib$show_timer (0, 0, 0, 0);

    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CVQB_THD_003.C */
/*  *5     5-SEP-1990 15:18:57 CURTIN "adjusted test name in cts_test" */
/*  *4     3-JUL-1990 12:08:01 BUTENHOF "Change copyright" */
/*  *3    21-JUN-1990 08:37:29 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *2     7-JUN-1990 11:41:37 SCALES "Updates to CTS" */
/*  *1    17-MAY-1990 14:00:06 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CVQB_THD_003.C */
