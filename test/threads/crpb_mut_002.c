/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crpb_mut_002.c,v $
 * Revision 1.1.11.2  1996/02/17  23:29:30  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:46:18  marty]
 *
 * Revision 1.1.11.1  1995/12/11  22:57:17  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:34:41  root]
 * 
 * Revision 1.1.9.1  1994/04/01  20:08:30  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:11:33  jd]
 * 
 * Revision 1.1.7.1  1993/10/12  19:34:14  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:29:27  rhw]
 * 
 * Revision 1.1.5.3  1992/12/30  16:27:41  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:04:11  zeliff]
 * 
 * Revision 1.1.5.2  1992/09/03  14:30:28  bolinger
 * 	Move current 1.0.1-backed file (modified to work with new
 * 	threads source base) from private branch into 1.0.2 tree.
 * 	(See OT defect 5300.)
 * 	[1992/09/02  13:43:29  bolinger]
 * 
 * Revision 1.1.3.2  1992/08/12  17:22:12  bolinger
 * 	Replace uses of struct timeb/ftime(3) with refs to new timing
 * 	interface specific to these tests (which is designed to hide
 * 	any further changes to the underlying system facilities used).
 * 
 * Revision 1.1.2.3  1992/06/30  13:46:47  bolinger
 * 	Fix OT defect 4202:  typo in non-prototyped version of
 * 	function declaration.
 * 	[1992/06/29  19:29:05  bolinger]
 * 
 * Revision 1.1.2.2  1992/03/05  20:38:02  bolinger
 * 	Avoid violating "proper" mutex semantics in implementing this test:
 * 	don't let a thread unlock a mutex it doesn't own; don't re-lock a
 * 	previously-locked (non-recursive) mutex.  To get desired (blocking)
 * 	behavior on each lock operation, yield after each lock or unlock, to
 * 	let the other thread in the test run.
 * 
 * 	To preserve the performance measurement aspect of the test, measure the
 * 	time consumed by the yield operations and subtract it from the time
 * 	recorded for the lock/unlock loops.
 * 	[1992/03/05  20:35:27  bolinger]
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
 *	RP_MUT_002
 *
 *  TEST TOPICS:
 *
 *	MUT  -  Mutex Operations
 *
 *  ABSTRACT:
 *
 *	This program measures the amount of time required to lock
 *	and unlock a mutex, where the thread blocks on each lock attempt.
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
#include <cma_mutex.h>
#include <dce/cts_test.h>

#ifdef	VMS
#include <timeb.h>
#endif


#define	iterations 100000


cma_t_mutex mutex_1;
cma_t_mutex mutex_ready;
cma_t_cond	cond_ready;
int		ready;

cma_t_address
doyield(int *count)
{
	int i;
	int reps = *count;

	for (i = 0; i < reps; ++i)
		cma_yield();
	return ((cma_t_address)0);
}

void
timedelta(cts_timebuf_t *delta, cts_timebuf_t *start, cts_timebuf_t *end)
{
	cts_timerecord(start, end, delta);
}

void
dobusyyield(cts_timebuf_t *delta, int count)
{
	cma_t_thread ythread;
	int i;
    cts_timebuf_t start;
    cts_timebuf_t end;

    cma_thread_create(
	    &ythread, 
	    &cma_c_null, 
	    doyield, 
	    (cma_t_address)&count);
	cts_gettime(&start);
	for (i = 0; i < count; ++i)
		cma_yield();
	cts_gettime(&end);
	timedelta(delta, &start, &end);
}

void
do_locking(
	cma_t_mutex	*mutex_a,
	cts_timebuf_t	*delta)
    {
    cts_timebuf_t start;
    cts_timebuf_t end;
    int		 i;


    cts_gettime(&start);

    for (i = 1; ; i++) {
		cma_mutex_lock (mutex_a);
		cma_yield();
		cma_mutex_unlock (mutex_a);
		cma_yield();
		if (iterations/2 <= i)
			break;
	}
    cts_gettime(&end);

	timedelta(delta, &start, &end);
    }


cma_t_address
thread2(
	cma_t_address	delta)
    {
	cma_mutex_lock(&mutex_ready);
	ready += 2;
	cma_cond_signal(&cond_ready);
	while (ready != 3)
		cma_cond_wait(&cond_ready, &mutex_ready);
	cma_mutex_unlock(&mutex_ready);
    do_locking (&mutex_1, (cts_timebuf_t *)delta);
    return delta;
    }

main ()
    {
    cma_t_thread t1_handle;
    cts_timebuf_t elapsed1, elapsed2;
    cma_t_exit_status status;
    cma_t_address dummy;
	cts_timebuf_t yield;


    cts_test ("CRPB_MUT_002", "Measure blocking mutex lock/unlock performance.");

    cma_init ();

    cma_mutex_create (&mutex_1, &cma_c_null);
    cma_mutex_create (&mutex_ready, &cma_c_null);
	cma_cond_create (&cond_ready, &cma_c_null);

    cma_thread_create (
	    &t1_handle, 
	    &cma_c_null, 
	    thread2, 
	    (cma_t_address)&elapsed2);

	cma_mutex_lock(&mutex_ready);
	ready += 1;
	cma_cond_signal(&cond_ready);
	while (ready != 3)
		cma_cond_wait(&cond_ready, &mutex_ready);
	cma_mutex_unlock(&mutex_ready);
    do_locking (&mutex_1, &elapsed1);

    cma_thread_join (&t1_handle, &status, &dummy);

    if (status != cma_c_term_normal) 
        cts_failed ("Second thread error.");

	dobusyyield(&yield, iterations);
	timedelta(&elapsed1, &yield, &elapsed1);
	timedelta(&elapsed2, &yield, &elapsed2);

    printf("\n%d iterations of lock/unlock (blocking) took \n", iterations);
    printf("\tInitial Thread:\t%5.3f seconds\n", cts_timecvt(&elapsed1));
    printf("\tSecond Thread:\t%5.3f seconds\n\n", cts_timecvt(&elapsed2));
    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRPB_MUT_002.C */
/*  *7     5-SEP-1990 09:38:36 BUTENHOF "Convert to new CMA interface" */
/*  *6     3-JUL-1990 12:01:51 BUTENHOF "Change copyright" */
/*  *5    21-JUN-1990 12:19:00 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *4    21-JUN-1990 11:47:04 BUTENHOF "Conditionalize prototypes for VAX ULTRIX" */
/*  *3    21-JUN-1990 08:23:16 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *2     7-JUN-1990 11:33:48 SCALES "Updates to CTS" */
/*  *1    17-MAY-1990 13:56:59 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRPB_MUT_002.C */
