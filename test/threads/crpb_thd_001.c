/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crpb_thd_001.c,v $
 * Revision 1.1.9.2  1996/02/17  23:29:31  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:46:19  marty]
 *
 * Revision 1.1.9.1  1995/12/11  22:57:21  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:34:42  root]
 * 
 * Revision 1.1.7.1  1994/04/01  20:08:32  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:11:36  jd]
 * 
 * Revision 1.1.5.1  1993/10/12  19:34:16  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:29:28  rhw]
 * 
 * Revision 1.1.3.3  1992/12/30  16:27:43  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:04:15  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/03  14:30:33  bolinger
 * 	Move current 1.0.1-backed file (modified to work with new
 * 	threads source base) from private branch into 1.0.2 tree.
 * 	(See OT defect 5300.)
 * 	[1992/09/02  13:43:34  bolinger]
 * 
 * Revision 1.1.1.2  1992/08/12  17:22:49  bolinger
 * 	Replace uses of struct timeb/ftime(3) with refs to new timing
 * 	interface specific to these tests (which is designed to hide
 * 	any further changes to the underlying system facilities used).
 * 
 * Revision 1.1  1992/01/19  03:20:00  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  Copyright (c) 1989, 1991 by
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
 *	RP_THD_001.C
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
 * 	001	Webb Scales	9 November 1989
 *		Added include of timeb.h for VMS.  On Ultrix, it is included
 * 		inside CMA.h already.
 *	002	Dave Butenhof	29 August 1990
 *		Convert to new cma interface
 *	003	Webb Scales	14 March 1991
 *		Removed spurious include of cma_defs.h
 *	004	Dave Butenhof	23 May 1991
 *		Fix compilation errors under new MIPS C compiler.
 *	005	Dave Butenhof	05 June 1991
 *		Add argument handling to specify iterations.
 */

/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <dce/cts_test.h>

#ifdef VMS
#include <timeb.h>
#endif

static int iterations;

cma_t_address
null_thread ()
   {}	    /* A "null" thread-action routine */

int
main(int argc, char *argv[], char *envp[])
    {
    cma_t_thread     thread;
    cma_t_attr attr;
    cts_timebuf_t start;
    cts_timebuf_t end;
    int		     i;


    if (argc < 2)
	iterations = 10000;
    else
	iterations = atoi (argv[1]);

    cts_test ("CRPB_THD_001", "Thread forking (cached stacks) performance");

    cma_init ();

    cma_attr_create (&attr, &cma_c_null);

    /* Don't time the first allocation */
    cma_thread_create (
	    &thread,
	    &attr,
	    (cma_t_start_routine)null_thread,
	    cma_c_null_ptr);
    cma_thread_detach (&thread);
    cma_yield ();

    cts_gettime(&start);

    for (i = 0; i < iterations; i++) {
	cma_thread_create (
		&thread,
		&attr,
		(cma_t_start_routine)null_thread,
		cma_c_null_ptr);
	cma_thread_detach (&thread);
	cma_yield ();
	}

    cts_gettime(&end);

    printf("\n%d iterations of fork/detach/yield (cached stacks) took %5.3f seconds\n",
        iterations, cts_timediff(&start, &end));

    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRPB_THD_001.C */
/*  *8     5-JUN-1991 13:05:48 BUTENHOF "Add argument parsing" */
/*  *7    29-MAY-1991 18:01:34 BUTENHOF "Fix compilation errors" */
/*  *6    14-MAR-1991 19:36:10 SCALES "Remove spurious include of cma_defs.h" */
/*  *5     5-SEP-1990 09:38:42 BUTENHOF "Convert to new CMA interface" */
/*  *4     3-JUL-1990 12:01:55 BUTENHOF "Change copyright" */
/*  *3    21-JUN-1990 08:23:30 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *2     7-JUN-1990 11:33:54 SCALES "Updates to CTS" */
/*  *1    17-MAY-1990 13:57:02 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRPB_THD_001.C */
