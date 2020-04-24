/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cvpb_thd_001.c,v $
 * Revision 1.1.36.2  1996/02/17  23:31:12  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:44  marty]
 *
 * Revision 1.1.36.1  1995/12/11  23:01:36  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:01  root]
 * 
 * Revision 1.1.34.1  1993/10/12  19:36:47  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:38  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:51:52  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:10:15  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:21:41  devrcs
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
 *	CVPB_THD_001.C
 *
 *  TEST TOPICS:
 *
 *	THD  -  Thread Operations
 *
 *  ABSTRACT:
 *
 *	This test measures thread creation peformance.  It forks a thread, 
 *	detaches it so that its stack will be cached, and then yields the 
 *	processor to it.
 *
 *  AUTHORS:
 *
 *	Webb Scales
 *
 *  CREATION DATE:
 *
 *	28 September 1989
 *
 *  MODIFICATION HISTORY:
 *
 *  001	    Paul Curtin	    4 September 1990
 *	    Convert to new CMA interfaces.
 *	    Adjust test name in cts_test statement.
 */

/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <cma_defs.h>
#include <dce/cts_test.h>

#define	iterations 10000


null_thread () {}	    /* A "null" thread-action routine */


main ()
    {
    cma_t_thread     thread;
    cma_t_attr attr;
    int		     i;


    cts_test ("CVPB_THD_001", "Thread forking (cached stacks) performance");

    cma_init ();

    cma_attr_create (&attr, &cma_c_null);

    /* Don't time the first allocation */
    cma_thread_create (&thread, &attr, null_thread, cma_c_null_ptr);
    cma_thread_detach (&thread);
    cma_yield ();

    printf("Starting...\n");
    lib$init_timer (0);

    for (i = 0; i < iterations; i++) {
	cma_thread_create (&thread, &attr, null_thread, cma_c_null_ptr);
	cma_thread_detach (&thread);
	cma_yield ();
	}

    printf("For %d iterations of fork/detach/yield (cached stacks):\n", 
	    iterations);
    lib$show_timer (0, 0, 0, 0);

    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CVPB_THD_001.C */
/*  *5     5-SEP-1990 13:41:58 CURTIN "Converted to use new CMA interface" */
/*  *4     3-JUL-1990 12:07:38 BUTENHOF "Change copyright" */
/*  *3    21-JUN-1990 08:36:42 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *2     7-JUN-1990 11:41:06 SCALES "Updates to CTS" */
/*  *1    17-MAY-1990 13:59:46 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CVPB_THD_001.C */
