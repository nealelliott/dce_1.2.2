/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crpb_cvx_001.c,v $
 * Revision 1.1.9.2  1996/02/17  23:29:28  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:46:16  marty]
 *
 * Revision 1.1.9.1  1995/12/11  22:57:13  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:34:39  root]
 * 
 * Revision 1.1.7.1  1994/04/01  20:08:29  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:11:30  jd]
 * 
 * Revision 1.1.5.1  1993/10/12  19:34:11  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:29:25  rhw]
 * 
 * Revision 1.1.3.3  1992/12/30  16:27:37  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:04:05  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/03  14:30:16  bolinger
 * 	Move current 1.0.1-backed file (modified to work with new
 * 	threads source base) from private branch into 1.0.2 tree.
 * 	(See OT defect 5300.)
 * 	[1992/09/02  13:43:19  bolinger]
 * 
 * Revision 1.1.1.2  1992/08/12  17:18:20  bolinger
 * 	Replace uses of struct timeb/ftime(3) with refs to new timing
 * 	interface specific to these tests (which is designed to hide
 * 	any further changes to the underlying system facilities used).
 * 
 * Revision 1.1  1992/01/19  03:19:57  devrcs
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

struct synch {
    cma_t_mutex    mutex;
    cma_t_cond     cv;
    cma_t_boolean  ok;
    } synch1, synch2;


void
do_wait(
	struct synch	*synch_a,
	struct synch	*synch_b,
	cts_timebuf_t	*delta)
    {
    cts_timebuf_t start;
    cts_timebuf_t end;
    int		 i;


    cts_gettime(&start);

    for (i = 0; i < iterations/2; i++) {
	cma_mutex_lock (&synch_a->mutex);
	while (!synch_a->ok) cma_cond_wait (&synch_a->cv, &synch_a->mutex);
	synch_a->ok = cma_c_false;
	cma_mutex_unlock (&synch_a->mutex);
	
	cma_mutex_lock (&synch_b->mutex);
	synch_b->ok = cma_c_true;
	cma_mutex_unlock (&synch_b->mutex);

	cma_cond_signal (&synch_b->cv);
	}

    cts_gettime(&end);

	cts_timerecord(&start, &end, delta);
	}


cma_t_address
thread2	(
	cma_t_address	delta)
    {
    do_wait (&synch1, &synch2, (cts_timebuf_t *)delta);
    return delta;
    }

main ()
    {
    cma_t_thread t1_handle;
    cts_timebuf_t elapsed1, elapsed2;
    cma_t_exit_status status;
    cma_t_address dummy;


    cts_test ("CRPB_CVX_001", "Measure wait/signal performance.");

    cma_init ();

    cma_mutex_create (&synch1.mutex, &cma_c_null);
    cma_cond_create  (&synch1.cv,    &cma_c_null);
    synch1.ok = cma_c_true;

    cma_mutex_create (&synch2.mutex, &cma_c_null);
    cma_cond_create  (&synch2.cv,    &cma_c_null);
    synch2.ok = cma_c_false;

    cma_thread_create (
	    &t1_handle, 
	    &cma_c_null, 
	    thread2, 
	    (cma_t_address)&elapsed2);

    do_wait (&synch2, &synch1, &elapsed1);

    cma_thread_join (&t1_handle, &status, &dummy);

    if (status != cma_c_term_normal) 
        cts_failed ("Second thread error.");

    printf("\n%d iterations of wait/signal took \n", iterations);
    printf("\tInitial Thread:\t%5.3f seconds\n", cts_timecvt(&elapsed1));
    printf("\tSecond Thread:\t%5.3f seconds\n\n", cts_timecvt(&elapsed2));

    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRPB_CVX_001.C */
/*  *7     5-SEP-1990 09:38:22 BUTENHOF "Convert to new CMA interface" */
/*  *6     3-JUL-1990 12:01:39 BUTENHOF "Change copyright" */
/*  *5    21-JUN-1990 12:18:43 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *4    21-JUN-1990 11:46:51 BUTENHOF "Conditionalize prototypes for VAX ULTRIX" */
/*  *3    21-JUN-1990 08:22:49 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *2     7-JUN-1990 11:33:36 SCALES "Updates to CTS" */
/*  *1    17-MAY-1990 13:56:53 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRPB_CVX_001.C */
