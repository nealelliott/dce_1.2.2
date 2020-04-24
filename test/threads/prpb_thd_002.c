/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: prpb_thd_002.c,v $
 * Revision 1.1.51.2  1996/02/17  23:31:30  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:48:00  marty]
 *
 * Revision 1.1.51.1  1995/12/11  23:02:32  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:16  root]
 * 
 * Revision 1.1.49.1  1993/10/12  19:37:12  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:50  rhw]
 * 
 * Revision 1.1.3.4  1992/12/30  22:33:43  jd
 * 	Dropped the number of iterations to 500. 1000 iterations consumed about
 * 	23M of memory. The real question is how does it work on 1.0.4 with a
 * 	data limit of 16M.
 * 	[1992/12/30  20:47:17  jd]
 * 
 * Revision 1.1.3.3  1992/12/30  16:53:07  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:11:24  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/03  14:32:46  bolinger
 * 	Move current 1.0.1-backed file (modified to work with new
 * 	threads source base) from private branch into 1.0.2 tree.
 * 	(See OT defect 5300.)
 * 	[1992/09/02  13:45:37  bolinger]
 * 
 * Revision 1.1.1.2  1992/08/11  21:40:23  bolinger
 * 	Replace uses of struct timeb/ftime(3) with refs to new timing
 * 	interface specific to these tests (which is designed to hide
 * 	any further changes to the underlying system facilities used).
 * 
 * Revision 1.1  1992/01/19  03:21:57  devrcs
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
 *	PTHREAD Test Programs
 *
 *  FILENAME:
 *
 *	RP_THD_002.C
 *
 *  TEST TOPICS:
 *
 *	THD  -  Thread Operations
 *
 *  ABSTRACT:
 *
 *	This test measures thread creation peformance.  It forks a thread,
 *	and then yields the processor to it.  The child thread is not
 *	detached so that its stack cannot be cached,
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
 *	001	Webb Scales	9 November 1989
 *		Added include of timeb.h for VMS.  On Ultrix, it is included
 *		inside CMA.h already.
 *	002	Paul Curtin	29 May 1990
 *		Converted test to use pthreads interface.
 *	003	Dave Butenhof	7 September 1990
 *		Remove uses of CMA interface.
 */

/*
 *  INCLUDE FILES
 */

#include <pthread.h>
#include <dce/cts_test.h>

#ifdef VMS
#include <timeb.h>
#endif

#define	iterations 500

pthread_addr_t
null_thread ()
    {}	    /* A "null" thread-action routine */


main ()
    {
    pthread_t	     thread;
    pthread_attr_t   attr;
    cts_timebuf_t     start;
    cts_timebuf_t     end;
    int		     i;
    int		     status;

    cts_test ("PRPB_THD_002", "Thread forking (noncached stacks) performance");

    status = pthread_attr_create (&attr);
    check(status,"pthread_attr_create");

    status = pthread_attr_setstacksize (&attr, 1024);
    check(status,"pthread_attr_setstacksize");

    /*
     * Don't time the first creation
     */
    status = pthread_create (&thread, attr, null_thread, 0);
    check(status,"pthread_create");
    status = pthread_detach (&thread);
    check(status,"pthread_detach");
    pthread_yield ();

    cts_gettime(&start);

    for (i = 0; i < iterations; i++) {
	status = pthread_create (&thread, attr, null_thread, 0);
	check(status,"pthread_create");
	pthread_yield ();
	}

    cts_gettime(&end);

    printf("\n%d iterations of fork/yield (uncached stacks) took %5.3f seconds\n",
        iterations, cts_timediff(&start, &end));

    cts_result ();
    }
