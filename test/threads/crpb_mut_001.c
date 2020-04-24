/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crpb_mut_001.c,v $
 * Revision 1.1.7.2  1996/02/17  23:29:29  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:46:17  marty]
 *
 * Revision 1.1.7.1  1995/12/11  22:57:15  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:34:40  root]
 * 
 * Revision 1.1.5.1  1993/10/12  19:34:13  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:29:26  rhw]
 * 
 * Revision 1.1.3.3  1992/12/30  16:27:39  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:04:08  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/03  14:30:22  bolinger
 * 	Move current 1.0.1-backed file (modified to work with new
 * 	threads source base) from private branch into 1.0.2 tree.
 * 	(See OT defect 5300.)
 * 	[1992/09/02  13:43:24  bolinger]
 * 
 * Revision 1.1.1.2  1992/08/12  17:20:37  bolinger
 * 	Replace uses of struct timeb/ftime(3) with refs to new timing
 * 	interface specific to these tests (which is designed to hide
 * 	any further changes to the underlying system facilities used).
 * 
 * Revision 1.1  1992/01/19  03:19:58  devrcs
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
 *	RP_MUT_001
 *
 *  TEST TOPICS:
 *
 *	MUT  -  Mutex Operations
 *
 *  ABSTRACT:
 *
 *	This program measures the amount of time required to lock
 *	and unlock a mutex, in the absence of contention or 
 *	interference from other threads.
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


main ()
    {
    cma_t_mutex	 mutex;
    cts_timebuf_t start;
    cts_timebuf_t end;
    int		 i;


    cts_test ("CRPB_MUT_001", "Measure non-blocking mutex lock/unlock performance.");

    cma_init ();

    cma_mutex_create (&mutex, &cma_c_null);

    cts_gettime(&start);

    for (i = 0; i < iterations; i++) {
	cma_mutex_lock (&mutex);
	cma_mutex_unlock (&mutex);
	}

    cts_gettime(&end);

    printf("\n%d iterations of lock/unlock (non-blocking) took %5.3f seconds\n",
        iterations, cts_timediff(&start, &end));

    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRPB_MUT_001.C */
/*  *5     5-SEP-1990 09:38:30 BUTENHOF "Convert to new CMA interface" */
/*  *4     3-JUL-1990 12:01:46 BUTENHOF "Change copyright" */
/*  *3    21-JUN-1990 08:23:04 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *2     7-JUN-1990 11:33:42 SCALES "Updates to CTS" */
/*  *1    17-MAY-1990 13:56:57 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRPB_MUT_001.C */
