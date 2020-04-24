/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crqb_thd_003.c,v $
 * Revision 1.1.8.2  1996/02/17  23:29:36  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:46:23  marty]
 *
 * Revision 1.1.8.1  1995/12/11  22:57:36  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:34:45  root]
 * 
 * Revision 1.1.5.1  1993/10/12  19:34:25  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:29:32  rhw]
 * 
 * Revision 1.1.3.3  1992/12/30  16:27:52  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:04:34  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/03  14:31:00  bolinger
 * 	Move current 1.0.1-backed file (modified to work with new
 * 	threads source base) from private branch into 1.0.2 tree.
 * 	(See OT defect 5300.)
 * 	[1992/09/02  13:44:01  bolinger]
 * 
 * Revision 1.1.1.2  1992/08/12  20:54:14  bolinger
 * 	Replace uses of struct timeb/ftime(3) with refs to new timing
 * 	interface specific to these tests (which is designed to hide
 * 	any further changes to the underlying system facilities used).
 * 
 * Revision 1.1  1992/01/19  03:20:08  devrcs
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
 *	CRQB_THD_003.C
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
 *  	001	Webb Scales	9 November 1989
 *		Added include of timeb.h for VMS.  On Ultrix, it is included
 *		inside CMA.h already.
 *	002	Dave Butenhof	8 August 1990
 *		Change semaphore operations to new form.
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

#ifdef VMS
#include <timeb.h>
#endif


#define	iterations 100000


cma__t_semaphore    sem;
cma__t_int_tcb	    *tcb;

main ()
    {
    cts_timebuf_t start;
    cts_timebuf_t end;
    int i;


    cts_test ("CRQB_THD_003", "Semaphore non-blocking performance measurement");

    cma_init ();

    cma__sem_init (&sem);

    tcb = cma__get_self_tcb();

    cts_gettime(&start);

    for (i = 0;  i < iterations;  i++)
	{
	cma__sem_wake_one (&sem);
	cma__enter_kernel ();
	cma__sem_wait (&sem, tcb);
	cma__exit_kernel ();
	}

    cts_gettime(&end);

    printf("\n%d iterations of increment/decrement semaphore (nonblocking) took %5.3f seconds\n",
        iterations, cts_timediff(&start, &end));

    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRQB_THD_003.C */
/*  *6     4-SEP-1990 16:59:17 SCALES "Change filename references" */
/*  *5     8-AUG-1990 14:00:40 BUTENHOF "Use new semaphore primitives" */
/*  *4     3-JUL-1990 12:02:21 BUTENHOF "Change copyright" */
/*  *3    21-JUN-1990 08:24:44 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *2     7-JUN-1990 11:34:30 SCALES "Updates to CTS" */
/*  *1    17-MAY-1990 13:57:16 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRQB_THD_003.C */
