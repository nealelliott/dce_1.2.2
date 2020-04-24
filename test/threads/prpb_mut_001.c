/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: prpb_mut_001.c,v $
 * Revision 1.1.49.2  1996/02/17  23:31:28  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:59  marty]
 *
 * Revision 1.1.49.1  1995/12/11  23:02:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:14  root]
 * 
 * Revision 1.1.47.1  1993/10/12  19:37:08  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:48  rhw]
 * 
 * Revision 1.1.3.3  1992/12/30  16:53:02  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:11:17  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/03  14:32:34  bolinger
 * 	Move current 1.0.1-backed file (modified to work with new
 * 	threads source base) from private branch into 1.0.2 tree.
 * 	(See OT defect 5300.)
 * 	[1992/09/02  13:45:26  bolinger]
 * 
 * Revision 1.1.1.2  1992/08/11  21:39:21  bolinger
 * 	Replace uses of struct timeb/ftime(3) with refs to new timing
 * 	interface specific to these tests (which is designed to hide
 * 	any further changes to the underlying system facilities used).
 * 
 * Revision 1.1  1992/01/19  03:21:54  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  Copyright (c) 1990, 1991 by
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
 *	001	Paul Curtin	29 May 1990
 *		Converted test to use pthreads interface.
 *	002	Dave Butenhof	29 May 1991
 *		Change name of test printout.
 */

/*
 *  INCLUDE FILES
 */

#include <pthread.h>
#include <dce/cts_test.h>

#ifdef VMS
#include <timeb.h>
#endif


#define	iterations 100000


main ()
    {
    pthread_mutex_t mutex;
    cts_timebuf_t start;
    cts_timebuf_t end;
    int		 i;
    int		 status;

    cts_test ("PRPB_MUT_001", "Measure non-blocking mutex lock/unlock performance.");

    status = pthread_mutex_init (&mutex, pthread_mutexattr_default);
    check(status,"pthread_mutex_init");    

    printf("Starting...\n");
    cts_gettime(&start);

    for (i = 0; i < iterations; i++) {
	status = pthread_mutex_lock (&mutex);
	check(status,"pthread_mutex_lock");
	status = pthread_mutex_unlock (&mutex);
	check(status,"pthread_mutex_unlock");
	}

    cts_gettime(&end);

    printf("\n%d iterations of lock/unlock (non-blocking) took %5.3f seconds\n",
        iterations, cts_timediff(&start, &end));

    cts_result ();
    }
