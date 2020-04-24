/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: prpb_thd_003.c,v $
 * Revision 1.1.54.2  1996/02/17  23:31:31  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:48:01  marty]
 *
 * Revision 1.1.54.1  1995/12/11  23:02:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:17  root]
 * 
 * Revision 1.1.52.1  1994/04/01  20:11:34  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:18:25  jd]
 * 
 * Revision 1.1.50.1  1993/10/12  19:37:14  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:51  rhw]
 * 
 * Revision 1.1.3.3  1992/12/30  16:53:08  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:11:27  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/03  14:32:53  bolinger
 * 	Move current 1.0.1-backed file (modified to work with new
 * 	threads source base) from private branch into 1.0.2 tree.
 * 	(See OT defect 5300.)
 * 	[1992/09/02  13:45:43  bolinger]
 * 
 * Revision 1.1.1.2  1992/08/11  21:43:12  bolinger
 * 	Replace uses of struct timeb/ftime(3) with refs to new timing
 * 	interface specific to these tests (which is designed to hide
 * 	any further changes to the underlying system facilities used).
 * 
 * Revision 1.1  1992/01/19  03:21:58  devrcs
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
 *	001	Paul Curtin	29 May 1990
 *		Converted test to use pthreads interface.
 *	002	Paul Curtin	05 July 1990
 *		Added ifdef protos to a couple of routines.
 *	003	Dave Butenhof	29 May 1991
 *		Fix name printout.
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


pthread_mutex_t mutex_1;
pthread_mutex_t mutex_2;


void
do_yielding 	(
	pthread_mutex_t mutex_a, 
	pthread_mutex_t mutex_b,
	cts_timebuf_t *delta)
    {
    cts_timebuf_t start;
    cts_timebuf_t end;
    int		 i;


    cts_gettime(&start);

    for (i = 0; i < iterations/2; i++) {
	pthread_yield ();
	}

    cts_gettime(&end);
	cts_timerecord(&start, &end, delta);
    }


pthread_addr_t
thread2	(
	pthread_addr_t delta)
    {
    do_yielding (mutex_1, mutex_2, (cts_timebuf_t *)delta);
    return delta;
    }

main ()
    {
    pthread_t t1_handle;
    cts_timebuf_t elapsed1, elapsed2;
    int status;
    pthread_addr_t dummy;


    cts_test ("PRPB_THD_003", "Measure YIELD time.");

    status = pthread_mutex_init (&mutex_1, pthread_mutexattr_default);
    check(status,"pthread_mutex_init");
    status = pthread_mutex_init (&mutex_2, pthread_mutexattr_default);
    check(status,"pthread_mutex_init");

    status = pthread_mutex_lock (&mutex_1);	/* Initialize; to block other thread */
    check(status,"pthread_mutex_lock");

    status = pthread_create (
		&t1_handle, 
		pthread_attr_default, 
		thread2, 
		(pthread_addr_t)&elapsed2);
    check(status,"pthread_create");

    printf("Starting...\n");

    do_yielding (mutex_2, mutex_1, &elapsed1);

    status = pthread_join (t1_handle, &dummy); /*  &dummy); */

    if (status != 0) 
        cts_failed ("Second thread error.");

    printf("\n%d iterations of yield took \n", iterations);
    printf("\tInitial Thread:\t%5.3f seconds\n", cts_timecvt(&elapsed1));
    printf("\tSecond Thread:\t%5.3f seconds\n\n", cts_timecvt(&elapsed2));

    cts_result ();
    }
