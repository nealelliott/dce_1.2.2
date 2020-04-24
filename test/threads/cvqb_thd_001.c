/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cvqb_thd_001.c,v $
 * Revision 1.1.6.2  1996/02/17  23:31:14  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:45  marty]
 *
 * Revision 1.1.6.1  1995/12/11  23:01:40  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:03  root]
 * 
 * Revision 1.1.4.2  1992/12/30  16:52:01  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:10:22  zeliff]
 * 
 * Revision 1.1.2.2  1992/06/30  13:44:05  bolinger
 * 	Fix OT defect 4084: use _CMA_VOLATILE_ instead of volatile alone.
 * 	[1992/06/29  19:33:32  bolinger]
 * 
 * Revision 1.1  1992/01/19  03:19:34  devrcs
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
 *	CVQB_THD_001.C
 *
 *  TEST TOPICS:
 *
 *	THD  -  Thread Operations
 *
 *  ABSTRACT:
 *
 *	This test measures thread context switch (register swap) peformance.
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
 *	    Adjust test name ini cts_test statement.
 */

/*
 *  INCLUDE FILES
 */

#include <cts_test.h>
#include <cma_assem.h>

#define	iterations 1000000

_CMA_VOLATILE_ int count;
cma__t_static_ctx ctx;

main ()
    {
    cts_test ("CVQB_THD_001", "cma__transfer_thread_ctx performance measurement");
    
    lib$init_timer (0);

    for (count = 0; count < iterations; count++) 
	cma__transfer_thread_ctx(&ctx, &ctx);

    printf("\nFor %d iterations of cma__transfer_thread_ctx :\n", iterations);
    lib$show_timer (0, 0, 0, 0);

    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CVQB_THD_001.C */
/*  *5     5-SEP-1990 15:16:17 CURTIN "Converted to use new CMA interfaces" */
/*  *4     3-JUL-1990 12:07:49 BUTENHOF "Change copyright" */
/*  *3    21-JUN-1990 08:37:06 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *2     7-JUN-1990 11:41:23 SCALES "Updates to CTS" */
/*  *1    17-MAY-1990 13:59:59 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CVQB_THD_001.C */
