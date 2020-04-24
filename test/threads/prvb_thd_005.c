/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: prvb_thd_005.c,v $
 * Revision 1.1.56.2  1996/02/17  23:32:05  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:48:30  marty]
 *
 * Revision 1.1.56.1  1995/12/11  23:04:13  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:44  root]
 * 
 * Revision 1.1.54.1  1994/04/01  20:12:28  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:20:31  jd]
 * 
 * Revision 1.1.52.1  1993/10/12  19:38:10  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:31:19  rhw]
 * 
 * Revision 1.1.4.2  1992/12/30  16:55:33  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:13:34  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/11  17:08:47  bolinger
 * 	Fix OT defect 2161:  provide/correct missing/bad cts_test() banners.
 * 	[1992/05/11  13:47:02  bolinger]
 * 
 * Revision 1.1  1992/01/19  03:19:46  devrcs
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
 *	PRVB_THD_005
 *
 *  TEST TOPICS:
 *
 *	THD  -  Thread Operations
 *
 *  ABSTRACT:
 *
 *	This test checks that the various thread_exit operations
 *	will terminate a thread and that exception clauses are triggered.
 *
 *	This test checks that when a thread exits normally,
 *	with an error, or with an alert, that:
 *	  o other threads are not affected
 *	  o an exception is raised and can be handled
 *	  o the result values are passed properly
 *
 *  AUTHORS:
 *
 *	Bob Conti
 *
 *  CREATION DATE:
 *
 *	13 December 1989
 *
 *  MODIFICATION HISTORY:
 *  001	    Paul Curtin	    31 May 1990
 *	    Converted test to use pthread interface.
 *  002	    Paul Curtin	    29 June 1990
 *	    Initialized timespecs in subordinate threads.
 */

/*
 *  INCLUDE FILES
 */

#include <pthread.h>
#include <dce/cts_test.h>

#define thread_count	10
#define iterations	20000

static pthread_addr_t thread_action  (pthread_addr_t);

static pthread_addr_t	input[thread_count];
static pthread_addr_t	output[thread_count];

typedef char	t_output_line[256];

main ()
    {

    pthread_t	threads[thread_count];
    pthread_addr_t	result[thread_count];
    int	status[thread_count];
    int pstatus;
    int	i;


    cts_test ("PRVB_THD_005", "This tests various flavors of thread_exit");

    for (i = 0; i < thread_count; i++) {

	int *ptr;

	input[i] = (pthread_addr_t)i;
	pstatus = pthread_create (
		&threads[i],
		pthread_attr_default,
		thread_action,
		(pthread_addr_t)&input[i]);
	check(pstatus,"pthread_create");
	pthread_yield ();

	status[i] = pthread_join (
		threads[i],
		(pthread_addr_t *)&ptr);    

	/*
	 * FIX-ME: modify the CMA architecture so that the error and
	 * alert cases are guarnateed to null the result.  This
	 * would allow the assignment to be safe to perform whenever
	 * the pointer is non-null rather than making the pointer
	 * indeterminate.  OR: allow the error and alert cases to
	 * pass a result??
	 */
	if (status[i] == 0) 
	    result[i] = (pthread_addr_t) ptr;

	pstatus = pthread_detach (&threads[i]);
	check(pstatus,"pthread_detach");

	}

    /*
     * Check that even threads exit with error and odd threads
     * exit normally and pass a result of thread_number + 1.
     */
    for (i = 0; i < thread_count; i++) {
	switch (i%3) {
	    case 0: if (status[i] != 0)
			cts_failed ("Some thread did not exit normally");
		    else if (*(pthread_addr_t *)result[i] != (pthread_addr_t)(i + 1)) 
			cts_failed ("Some thread did not correct exit status");
	
	    break;

	    case 1: if (result[i] != (pthread_addr_t)-2) 
			cts_failed ("Some thread did not get error exit");
		    break;

	    case 2: if (result[i] != (pthread_addr_t)-1) 
			cts_failed ("Some thread did not self-alert");
		    break;

	    default:	cts_failed ("Fell through case");
	    }
	}

    cts_result ();
    }

static pthread_addr_t
thread_subaction	(
	int	i)
    {
    struct timespec delay;
    int status;

    /* 
     * Make every other thread get normal exit, error_exit, or
     * alert itself.
     */
    switch (i%3)
	{
	case 0:	    delay.tv_sec = 1;
		    delay.tv_nsec = 50;
		    pthread_delay_np (&delay);
		    output[i] = (pthread_addr_t)(i+1);
	 	    pthread_exit ((pthread_addr_t)&output[i]);
		    break;

	case 1:	    delay.tv_sec = 2;
		    delay.tv_nsec = 50;
		    pthread_delay_np (&delay);
		    pthread_exit ((pthread_addr_t)-2);
		    break;

	case 2:	    {
		    pthread_t	self;

		    delay.tv_sec = 2;
		    delay.tv_nsec = 50;
		    pthread_delay_np (&delay);
		    self = pthread_self ();
		    status = pthread_cancel (self);
		    check(status,"pthread_cancel");
		    pthread_testcancel ();
		    break;
		    }

	default:    cts_failed ("Fell through case");
	}
    }

static pthread_addr_t
thread_action	(
	pthread_addr_t	input_ptr)
    {
    int i, *ptr;
    
    ptr = (int *) input_ptr;
    i = *ptr;


    TRY	{
	thread_subaction (i);

	/* 
	 * We should not get here since every thread should terminate
	 * with an exception.
	 */
	cts_failed ("Thread %d failed to exit with an exception", i);

	}
    CATCH_ALL 
	printf ("Caught exit exception.\n");
	RERAISE;
    ENDTRY

    }
