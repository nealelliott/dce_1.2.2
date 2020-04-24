/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cvpb_thd_002.c,v $
 * Revision 1.1.57.2  1996/02/17  23:31:13  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:44  marty]
 *
 * Revision 1.1.57.1  1995/12/11  23:01:39  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:02  root]
 * 
 * Revision 1.1.55.1  1993/10/12  19:36:48  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:39  rhw]
 * 
 * Revision 1.1.4.2  1992/12/30  16:51:57  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:10:18  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/11  17:07:50  bolinger
 * 	Fix OT defect 2161:  provide/correct missing/bad cts_test() banners.
 * 	[1992/05/11  13:45:58  bolinger]
 * 
 * Revision 1.1  1992/01/19  03:21:42  devrcs
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
 *	CVPB_THD_002.C
 *
 *  TEST TOPICS:
 *
 *	THD  -  Thread Operations
 *
 *  ABSTRACT:
 *
 *	This test measures thread creation peformance.  It forks a thread, 
 *	and then yields the processor to it.  The child thread is not detached
 *	so that its stack cannot be cached, 
 *
 *  AUTHORS:
 *
 *	Webb Scales
 *
 *  CREATION DATE:
 *
 *	26 October 1989
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

#define	iterations 1000


null_thread () {}	    /* A "null" thread-action routine */


main ()
    {
    cma_t_thread     thread;
    cma_t_attr attr;
    int		     i;


    cts_test ("CVPB_THD_002", "Thread forking (non-cached stacks) performance");

    cma_init ();

    cma_attr_create (&attr, &cma_c_null);

    cma_attr_set_stacksize (&attr, 1);
    cma_attr_set_guardsize (&attr, 1);

    /* Don't time the first allocation */
    cma_thread_create (&thread, &attr, null_thread, cma_c_null_ptr);
    cma_thread_detach (&thread);
    cma_yield ();

    printf("Starting...\n");
    lib$init_timer (0);

    for (i = 0; i < iterations; i++) {
	cma_thread_create (&thread, &attr, null_thread, cma_c_null_ptr);
	cma_yield ();
	}

    printf("For %d iterations of fork/yield (uncached stacks):\n", iterations);
    lib$show_timer (0, 0, 0, 0);

    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CVPB_THD_002.C */
/*  *5     5-SEP-1990 13:45:25 CURTIN "Converted to use new CMA interface" */
/*  *4     3-JUL-1990 12:07:45 BUTENHOF "Change copyright" */
/*  *3    21-JUN-1990 08:36:54 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *2     7-JUN-1990 11:41:16 SCALES "Updates to CTS" */
/*  *1    17-MAY-1990 13:59:54 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CVPB_THD_002.C */
