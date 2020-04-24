/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cupb_nbi_001.c,v $
 * Revision 1.1.63.2  1996/02/17  23:30:51  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:26  marty]
 *
 * Revision 1.1.63.1  1995/12/11  23:00:39  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:45  root]
 * 
 * Revision 1.1.60.1  1994/04/01  20:10:33  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:16:16  jd]
 * 
 * Revision 1.1.58.1  1993/10/12  19:36:15  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:23  rhw]
 * 
 * Revision 1.1.3.4  1993/02/01  21:34:08  hinman
 * 	[hinman] - SVR4 changes
 * 	[1993/02/01  21:06:03  hinman]
 * 
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  17:06:53  hinman]
 * 
 * Revision 1.1.3.3  1992/12/30  16:33:06  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:08:53  zeliff]
 * 	Revision 1.1.4.2  1993/01/11  16:16:33  hinman
 * 	[hinman] - Check in merged SNI version
 * 
 * Revision 1.1.3.2  1992/09/03  14:31:55  bolinger
 * 	Move current 1.0.1-backed file (modified to work with new
 * 	threads source base) from private branch into 1.0.2 tree.
 * 	(See OT defect 5300.)
 * 	[1992/09/02  13:44:51  bolinger]
 * 
 * Revision 1.1.1.2  1992/08/12  21:07:19  bolinger
 * 	Replace uses of struct timeb/ftime(3) with refs to new timing
 * 	interface specific to these tests (which is designed to hide
 * 	any further changes to the underlying system facilities used).
 * 
 * Revision 1.1  1992/01/19  03:21:24  devrcs
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
 *	CUPB_NBI_001
 *
 *  TEST TOPICS:
 *
 *	NBI  -  Non-Blocking (Unix) I/O
 *
 *  ABSTRACT:
 *
 *	This test will measure the performance of the CMA wrapper
 *	for the select system call, in a non-blocking call
 *
 *  AUTHORS:
 *
 *	Webb Scales
 *
 *  CREATION DATE:
 *
 *	27 September 1990
 *
 *  MODIFICATION HISTORY:
 *
 */


/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <dce/cts_test.h>

#define iterations  100000

int
main	(int argc, char *argv[], char *envp[])
    {
    cts_timebuf_t start;
    cts_timebuf_t end;
    int		 fds, i;


    cts_test ("CUPB_NBI_001", "Test select system call wrapper performance");
    cma_init ();

    cts_gettime(&start);

    for (i = 0; i < iterations; i++) {
	fds = 2;
#if (_CMA_UNIX_TYPE == _CMA__SVR4)
	__nr_select (2, 0, &fds, 0, 0);
#else
	select (2, 0, &fds, 0, 0);
#endif
	}

    cts_gettime(&end);

    printf("\n%d select calls (non-blocking) took %5.3f seconds\n",
        iterations, cts_timediff(&start, &end));

    cts_gettime(&start);

    for (i = 0; i < iterations; i++) {
	fds = 2;
#if (_CMA_UNIX_TYPE == _CMA__SVR4)
	select (2, 0, &fds, 0, 0);
#else
	cma_select (2, 0, &fds, 0, 0);
#endif
	}

    cts_gettime(&end);

    printf("\n%d cma_select calls (non-blocking) took %5.3f seconds\n",
        iterations, cts_timediff(&start, &end));

    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CUPB_NBI_001.C */
/*  *1    27-SEP-1990 19:03:43 SCALES "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CUPB_NBI_001.C */
