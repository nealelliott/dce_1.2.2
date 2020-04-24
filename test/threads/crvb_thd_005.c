/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crvb_thd_005.c,v $
 * Revision 1.1.38.2  1996/02/17  23:30:34  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:15  marty]
 *
 * Revision 1.1.38.1  1995/12/11  23:00:09  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:35  root]
 * 
 * Revision 1.1.36.1  1994/04/01  20:10:14  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:15:27  jd]
 * 
 * Revision 1.1.34.1  1993/10/12  19:35:52  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:14  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:32:15  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:21:51  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:21:15  devrcs
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
 *	CMA Test Programs
 *
 *  FILENAME:
 *
 *	CRVB_THD_005
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
 *  AUTHOR:
 *
 *	Bob Conti
 *
 *  CREATION DATE:
 *
 *	13 December 1989
 *
 *  MODIFICATION HISTORY:
 *
 *  001	    Paul Curtin	    4 September 1990
 *	    Convert to new CMA interfaces.
 */

/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <dce/cts_test.h>

#define thread_count	10
#define iterations	20000

static cma_t_address thread_action  (cma_t_address);

static cma_t_address	input[thread_count];
static cma_t_address	output[thread_count];

typedef char	t_output_line[256];

main ()
    {

    cma_t_thread	threads[thread_count];
    cma_t_address	result[thread_count];
    cma_t_exit_status	status[thread_count];
    cma_t_integer	i;


    cts_test ("CRVB_THD_005", "This tests various flavors of thread_exit");

    cma_init ();

    for (i = 0; i < thread_count; i++) {

	cma_t_integer *ptr;

	input[i] = (cma_t_address)i;
	cma_thread_create (
		&threads[i],
		&cma_c_null,
		thread_action,
		(cma_t_address)&input[i]);
	cma_yield ();

	cma_thread_join (
		&threads[i],
		&status[i],
		(cma_t_address *)&ptr);

	/*
	 * FIX-ME: modify the CMA architecture so that the error and
	 * alert cases are guarnateed to null the result.  This
	 * would allow the assignment to be safe to perform whenever
	 * the pointer is non-null rather than making the pointer
	 * indeterminate.  OR: allow the error and alert cases to
	 * pass a result??
	 */
	if (status[i] == cma_c_term_normal) 
	    result[i] = (cma_t_address)*ptr;

	cma_thread_detach (&threads[i]);

	}

    /*
     * Check that even threads exit with error and odd threads
     * exit normally and pass a result of thread_number + 1.
     */
    for (i = 0; i < thread_count; i++) {
	switch (i%3) {
	    case 0: if (status[i] != cma_c_term_normal) 
			cts_failed ("Some thread did not exit normally");
		    else if (result[i] != (cma_t_address)(i + 1)) 
			cts_failed ("Some thread did not correct exit status");
		    break;

	    case 1: if (status[i] != cma_c_term_error) 
			cts_failed ("Some thread did not get error exit");
		    break;

	    case 2: if (status[i] != cma_c_term_alert) 
			cts_failed ("Some thread did not self-alert");
		    break;

	    default:	cts_failed ("Fell through case");
	    }
	}

    cts_result ();
    }

static cma_t_address
thread_subaction	(
	cma_t_integer	i)
    {

    /* 
     * Make every other thread get normal exit, error_exit, or
     * alert itself.
     */
    switch (i%3)
	{
	case 0:	    cma_delay (1.5);
		    output[i] = (cma_t_address)(i + 1);
	 	    cma_thread_exit_normal ((cma_t_address)&output[i]);
		    break;

	case 1:	    cma_delay (2.5);
		    cma_thread_exit_error ();
		    break;

	case 2:	    {
		    cma_t_thread	self;

		    cma_delay (2.5);
		    cma_thread_get_self (&self);
		    cma_thread_alert (&self);
		    cma_alert_test ();
		    break;
		    }

	default:    cts_failed ("Fell through case");
	}
    }

static cma_t_address
thread_action	(
	cma_t_address	input_ptr)
    {
    cma_t_integer i, *ptr;
    
    ptr = (cma_t_integer *) input_ptr;
    i = *ptr;


    TRY
	{
	t_output_line 	line;

	thread_subaction (i);

	/* 
	 * We should not get here since every thread should terminate
	 * with an exception.
	 */
	sprintf (line, "Thread %d failed to exit with an exception", i);

	cts_failed (line);

	}
    CATCH_ALL 
	printf ("Caught exit exception.\n");
	RERAISE;
    ENDTRY

    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_THD_005.C */
/*  *7     5-SEP-1990 11:37:14 CURTIN "Converted to use new CMA interface" */
/*  *6    18-JUL-1990 22:54:32 SCALES "fix typo" */
/*  *5    18-JUL-1990 16:14:04 SCALES "Reduce use of COMMNT" */
/*  *4     3-JUL-1990 12:05:49 BUTENHOF "Change copyright" */
/*  *3    21-JUN-1990 08:32:34 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *2     7-JUN-1990 11:38:59 SCALES "Updates to CTS" */
/*  *1    17-MAY-1990 13:58:54 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_THD_005.C */
