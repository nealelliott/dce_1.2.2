/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crqb_thd_001.c,v $
 * Revision 1.1.9.2  1996/02/17  23:29:34  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:46:22  marty]
 *
 * Revision 1.1.9.1  1995/12/11  22:57:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:34:44  root]
 * 
 * Revision 1.1.7.1  1993/10/12  19:34:21  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:29:30  rhw]
 * 
 * Revision 1.1.5.3  1992/12/30  16:27:49  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:04:26  zeliff]
 * 
 * Revision 1.1.5.2  1992/09/03  14:30:49  bolinger
 * 	Move current 1.0.1-backed file (modified to work with new
 * 	threads source base) from private branch into 1.0.2 tree.
 * 	(See OT defect 5300.)
 * 	[1992/09/02  13:43:51  bolinger]
 * 
 * Revision 1.1.3.2  1992/08/12  17:27:36  bolinger
 * 	Replace uses of struct timeb/ftime(3) with refs to new timing
 * 	interface specific to these tests (which is designed to hide
 * 	any further changes to the underlying system facilities used).
 * 
 * Revision 1.1.2.2  1992/06/30  13:43:55  bolinger
 * 	Fix OT defect 4084: use _CMA_VOLATILE_ instead of volatile alone.
 * 	[1992/06/29  19:32:23  bolinger]
 * 
 * Revision 1.1  1992/01/19  03:20:05  devrcs
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
 *	CRQB_THD_001.C
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
 *	001	Webb Scales	9 November 1989
 *		Added include of timeb.h for VMS.  On Ultrix, it is included 
 *		inside cma.h already.
 *	002	Webb Scales	19 November 1989
 *		Changed ctx from cma__t_int_ctx_buffer cma__t_static_ctx
 *	003	Webb Scales	17 July 1989
 *		Added include files (for Apollo) and corrected file names.
 *	004	Dave Butenhof	18 December 1990
 *		Remove redundant #include of cma_host.h
 */

/*
 *  INCLUDE FILES
 */

#include <dce/cts_test.h>

#include <cma_defs.h>
#include <cma_config.h>
#include <cma_assem.h>

#ifdef VMS
#include <timeb.h>
#endif


#define	iterations 1000000


_CMA_VOLATILE_ int count;
cma__t_static_ctx ctx;


main ()
{
    cts_timebuf_t start;
    cts_timebuf_t end;


    cts_test ("CRQB_THD_001", "cma__transfer_thread_ctx performance");
    
    cts_gettime(&start);

    for (count = 0; count < iterations; count++) 
	cma__transfer_thread_ctx(&ctx, &ctx);

    cts_gettime(&end);

    printf("\n%d iterations of cma__transfer_thread_ctx took %5.3f seconds\n",
        iterations, cts_timediff(&start, &end));

    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRQB_THD_001.C */
/*  *8    18-DEC-1990 23:41:11 BUTENHOF "Remove unnecessary #includes" */
/*  *7     4-SEP-1990 16:58:58 SCALES "Change filename references" */
/*  *6    21-AUG-1990 09:46:57 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *5    17-JUL-1990 19:50:03 SCALES "Apollo BL3 changes" */
/*  *4     3-JUL-1990 12:02:11 BUTENHOF "Change copyright" */
/*  *3    21-JUN-1990 08:24:16 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *2     7-JUN-1990 11:34:16 SCALES "Updates to CTS" */
/*  *1    17-MAY-1990 13:57:11 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRQB_THD_001.C */
