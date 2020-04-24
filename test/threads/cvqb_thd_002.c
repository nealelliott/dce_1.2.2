/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cvqb_thd_002.c,v $
 * Revision 1.1.38.2  1996/02/17  23:31:14  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:46  marty]
 *
 * Revision 1.1.38.1  1995/12/11  23:01:42  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:04  root]
 * 
 * Revision 1.1.36.1  1993/10/12  19:36:50  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:40  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:52:06  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:10:26  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:21:44  devrcs
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
 *	CVQB_THD_002.C
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
 *	    Convert to use new CMA interfaces.
 *	    Adjust test name in cma_test.
 */

/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <cma_defs.h>
#include <cma_dispatch.h>
#include <cma_semaphore.h>
#include <cma_stack.h>
#include <dce/cts_test.h>

#define	iterations 100000


cma__t_semaphore    sem;
cma_t_thread	    thread;


incr_decr ()
    {
    int i;
    cma__t_int_tcb  *tcb;


    tcb = cma__get_self_tcb();

    for (i = 0;  i < iterations/2;  i++)
	{
	cma__increment_semaphore (&sem, 1);
	cma__enter_kernel ();
	cma__decrement_semaphore (&sem, tcb, 1);
	cma__exit_kernel ();
	}
    }


decr_incr ()
    {
    int i;
    cma__t_int_tcb  *tcb;


    tcb = cma__get_self_tcb();

    for (i = 0;  i < iterations/2;  i++)
	{
	cma__enter_kernel ();
	cma__decrement_semaphore (&sem, tcb, 1);
	cma__increment_semaphore (&sem, 1);
	cma__exit_kernel ();
	}
    }


main ()
    {
    cts_test ("CVQB_THD_002", "Semaphore blocking performance measurement");

    cma_init ();

    cma__init_semaphore (&sem, 0);

    printf("\nStarting...\n\n");
    lib$init_timer (0);

    cma_thread_create (&thread, &cma_c_null, incr_decr, 0);

    decr_incr ();

    printf("For %d iterations of increment/decrement semaphore:\n", iterations);
    lib$show_timer (0, 0, 0, 0);

    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CVQB_THD_002.C */
/*  *5     5-SEP-1990 15:19:28 CURTIN "Converted to use new CMA interfaces" */
/*  *4     3-JUL-1990 12:07:55 BUTENHOF "Change copyright" */
/*  *3    21-JUN-1990 08:37:17 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *2     7-JUN-1990 11:41:30 SCALES "Updates to CTS" */
/*  *1    17-MAY-1990 14:00:02 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CVQB_THD_002.C */
