/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: prpb_cvx_001.c,v $
 * Revision 1.1.80.2  1996/02/17  23:31:27  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:58  marty]
 *
 * Revision 1.1.80.1  1995/12/11  23:02:21  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:14  root]
 * 
 * Revision 1.1.78.1  1994/04/01  20:11:32  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:18:22  jd]
 * 
 * Revision 1.1.76.1  1993/10/12  19:37:06  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:47  rhw]
 * 
 * Revision 1.1.6.2  1993/08/11  14:19:52  rhw
 * 	changed pthread_attr_t declaration to pthread_mutex_t
 * 	for "mutex" field in structure.
 * 	[1993/08/10  14:02:49  rhw]
 * 
 * Revision 1.1.3.3  1992/12/30  16:53:00  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:11:14  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/03  14:32:29  bolinger
 * 	Move current 1.0.1-backed file (modified to work with new
 * 	threads source base) from private branch into 1.0.2 tree.
 * 	(See OT defect 5300.)
 * 	[1992/09/02  13:45:20  bolinger]
 * 
 * Revision 1.1.1.2  1992/08/11  21:38:13  bolinger
 * 	Replace uses of struct timeb/ftime(3) with refs to new timing
 * 	interface specific to these tests (which is designed to hide
 * 	any further changes to the underlying system facilities used).
 * 
 * Revision 1.1  1992/01/19  03:21:53  devrcs
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
 *  DIGITAL assumes
 *  no responsibility for the use or  reliability  of its
 *  software on equipment which is not supplied by DIGITAL.
 */

/*
 *  FACILITY:
 *
 *	PTHREAD Test Programs
 *
 *  FILENAME:
 *
 *	RP_CVX_001.C
 *
 *  TEST TOPICS:
 *
 *	CVX  -  Condition Variable Operations
 *
 *  ABSTRACT:
 *
 *	This program measures the amount of time required to wait and be
 *	signalled on a condition variable.
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
 *	002	Paul Curtin	15 May 1990
 *		Made some pointer changes so as to work on Mips.
 *	003	Paul Curtin	5 July 1990
 *		Added some protos to suit Vax Ultrix.
 *	004	Dave Butenhof	29 May 1991
 *		Fix test name.
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

struct synch {
    pthread_mutex_t  mutex;
    pthread_cond_t     cv;
    unsigned int  ok;
    } synch1, synch2;


void
do_wait	(
	struct synch *synch_a,
	struct synch *synch_b,
	cts_timebuf_t *delta)
    {
    cts_timebuf_t start;
    cts_timebuf_t end;
    int		 i;
    int		 status;

    cts_gettime(&start);

    for (i = 0; i < iterations/2; i++) {
	status = pthread_mutex_lock (&synch_a->mutex);
	check(status,"pthread_mutex_lock");
	while (!synch_a->ok) pthread_cond_wait (&synch_a->cv, &synch_a->mutex);
	synch_a->ok = (int) 0;
	status = pthread_mutex_unlock (&synch_a->mutex);
	check(status,"pthread_mutex_unlock");
	
	status = pthread_mutex_lock (&synch_b->mutex);
	check(status,"pthread_mutex_lock");
	synch_b->ok = (int) 1;
	status = pthread_mutex_unlock (&synch_b->mutex);
	check(status,"pthread_mutex_unlock");

	status = pthread_cond_signal (&synch_b->cv);
	check(status,"pthread_cond_signal");
	}

    cts_gettime(&end);
	cts_timerecord(&start, &end, delta);
    }


pthread_addr_t
thread2	(
	pthread_addr_t delta)
    {
    int temp;
    do_wait (&synch1, &synch2, (cts_timebuf_t *)delta);
    temp = -1;
    pthread_exit ((pthread_addr_t) -1);   /* delta;  */
    }

main ()
    {
    pthread_t t1_handle;
    cts_timebuf_t elapsed1, elapsed2;
    int status;
    int join_status;
    pthread_addr_t dummy;


    cts_test ("PRPB_CVX_001", "Measure wait/signal performance.");

    status = pthread_mutex_init (&synch1.mutex, pthread_mutexattr_default);
    check(status,"pthread_mutex_init");
    status = pthread_cond_init  (&synch1.cv,    pthread_condattr_default);
    check(status,"pthread_cond_init");
    synch1.ok = (int) 1;

    status = pthread_mutex_init (&synch2.mutex, pthread_mutexattr_default);
    check(status,"pthread_mutex_init");
    status = pthread_cond_init  (&synch2.cv,    pthread_condattr_default);
    check(status,"pthread_cond_init");
    synch2.ok = (int) 0;

    status = pthread_create (
		&t1_handle, 
		pthread_attr_default, 
		thread2, 
		(pthread_addr_t)&elapsed2);
    check(status,"pthread_create");

    printf("Starting...\n");

    do_wait (&synch2, &synch1, &elapsed1);

    status = pthread_join (t1_handle,(pthread_addr_t *)&join_status);
    check(status,"pthread_join");

    if (join_status != -1) 
        cts_failed ("Second thread error.");

    printf("\n%d iterations of wait/signal took \n", iterations);
    printf("\tInitial Thread:\t%5.3f seconds\n", cts_timecvt(&elapsed1));
    printf("\tSecond Thread:\t%5.3f seconds\n\n", cts_timecvt(&elapsed2));

    cts_result ();
    }
