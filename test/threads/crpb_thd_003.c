/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crpb_thd_003.c,v $
 * Revision 1.1.9.2  1996/02/17  23:29:33  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:46:21  marty]
 *
 * Revision 1.1.9.1  1995/12/11  22:57:26  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:34:43  root]
 * 
 * Revision 1.1.7.1  1994/04/01  20:08:33  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:11:40  jd]
 * 
 * Revision 1.1.5.1  1993/10/12  19:34:19  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:29:29  rhw]
 * 
 * Revision 1.1.3.3  1992/12/30  16:27:47  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:04:22  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/03  14:30:44  bolinger
 * 	Move current 1.0.1-backed file (modified to work with new
 * 	threads source base) from private branch into 1.0.2 tree.
 * 	(See OT defect 5300.)
 * 	[1992/09/02  13:43:42  bolinger]
 * 
 * Revision 1.1.1.2  1992/08/12  17:26:03  bolinger
 * 	Replace uses of struct timeb/ftime(3) with refs to new timing
 * 	interface specific to these tests (which is designed to hide
 * 	any further changes to the underlying system facilities used).
 * 
 * Revision 1.1  1992/01/19  03:20:03  devrcs
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
 *	RP_THD_003.C
 *
 *  TEST TOPICS:
 *
 *	THD  -  Thread Operations
 *
 *  ABSTRACT:
 *
 *	This program measures the time required to yield the processor.
 *
 *  AUTHORS:
 *
 *	Webb Scales
 *
 *  CREATION DATE:
 *
 *	12 March 1990
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	29 August 1990
 *		Convert to new cma interface
 */

/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <dce/cts_test.h>

#ifdef VMS
#include <timeb.h>
#endif

#define	iterations 100000

void
do_yielding(
	cts_timebuf_t	*delta)
{
    cts_timebuf_t start;
    cts_timebuf_t end;
    int		 i;


    cts_gettime(&start);

    for (i = 0; i < iterations/2; i++) {
	cma_yield ();
	}

    cts_gettime(&end);
	cts_timerecord(&start, &end, delta);
    }


cma_t_address
thread2(
	cma_t_address	delta)
{

    do_yielding ((cts_timebuf_t *)delta);
    return delta;
    }

main ()
    {
    cma_t_thread t1_handle;
    cts_timebuf_t elapsed1, elapsed2;
    cma_t_exit_status status;
    cma_t_address dummy;


    cts_test ("CRPB_THD_003", "Measure YIELD time.");

    cma_init ();

    cma_thread_create (
	    &t1_handle, 
	    &cma_c_null, 
	    thread2, 
	    (cma_t_address)&elapsed2);

    do_yielding (&elapsed1);

    cma_thread_join (&t1_handle, &status, &dummy);

    if (status != cma_c_term_normal) 
        cts_failed ("Second thread error.");

    printf("\n%d iterations of yield took \n", iterations);
    printf("\tInitial Thread:\t%5.3f seconds\n", cts_timecvt(&elapsed1));
    printf("\tSecond Thread:\t%5.3f seconds\n\n", cts_timecvt(&elapsed2));

    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRPB_THD_003.C */
/*  *7     5-SEP-1990 09:38:56 BUTENHOF "Convert to new CMA interface" */
/*  *6     3-JUL-1990 12:02:06 BUTENHOF "Change copyright" */
/*  *5    21-JUN-1990 12:19:15 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *4    21-JUN-1990 11:47:10 BUTENHOF "Conditionalize prototypes for VAX ULTRIX" */
/*  *3    21-JUN-1990 08:23:59 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *2     7-JUN-1990 11:34:09 SCALES "Updates to CTS" */
/*  *1    17-MAY-1990 13:57:08 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRPB_THD_003.C */
