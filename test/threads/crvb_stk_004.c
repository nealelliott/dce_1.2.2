/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crvb_stk_004.c,v $
 * Revision 1.1.63.2  1996/02/17  23:30:30  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:12  marty]
 *
 * Revision 1.1.63.1  1995/12/11  22:59:54  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:30  root]
 * 
 * Revision 1.1.61.1  1994/04/01  20:10:01  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:15:04  jd]
 * 
 * Revision 1.1.59.1  1993/10/12  19:35:43  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:10  rhw]
 * 
 * Revision 1.1.4.2  1992/12/30  16:31:48  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:21:31  zeliff]
 * 
 * Revision 1.1.2.2  1992/04/24  17:56:35  bolinger
 * 	Correct call to cma_stack_check_limit_np() -- this routine checks to
 * 	see if stack is big enough to accomodate an allocation of the size
 * 	indicated by its argument.  The test was calling this routine as though
 * 	it checked the total size of the stack against its argument.
 * 	[1992/04/24  16:54:21  bolinger]
 * 
 * Revision 1.1  1992/01/19  03:22:35  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  Copyright (c) 1991 by
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
 *	CRVB_STK_004
 *
 *  TEST TOPICS:
 *
 *	STK  -  Stacks
 *
 *  ABSTRACT:
 *
 *	This test creates a stack(s) larger than the default
 *	cluster, in an attempt to test code designed to
 *	create special clusters for such stacks.
 *
 *	Also, test that the large stack and special cluster 
 *	get cached on the attributes object.  This is done by
 *	multiple creations and deletions of the large stack
 *	using the same attributes object.
 *
 *  AUTHORS:
 *
 *	Paul Curtin
 *
 *  CREATION DATE:
 *
 *	11 July 1991
 *
 *  MODIFICATION HISTORY:
 * 
 *	001	Dave Butenhof	07 November 1991
 *		Modify the thread start routine to verify the assumed size
 *		of the large stack, rather than just running a null function
 *		(which would succeed even if there's a logic error resulting
 *		in a one page stack).
 */

/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <dce/cts_test.h>

#define ITERATIONS	100
#define STACKSIZE 7700480
#define GUARDSIZE	10000

cma_t_address
check_stack ()
    {
    /*
     * Make sure the stack's (approximately) as big was we wanted it to be.
     * Since we don't know how much space is required to start up a thread on
     * all architectures, allow a generous 2Kb for startup overhead, which
     * still leaves over 7.5Mb on the stack.
     */
    if (!cma_stack_check_limit_np (0)) {
	cts_failed ("stack was too small");
	cma_thread_exit_error ();
	}

    }

int
main	(int argc, char *argv[], char *envp[])
    {
    cma_t_attr		attr;
    cma_t_thread	thread;
    cma_t_integer	index;
    cma_t_exit_status	status;
    cma_t_address	result;


    cts_test ("CRVB_STK_004", "Test the ability to create stacks `>' the default cluster");
    cma_init ();

    cma_attr_create (&attr, &cma_c_null);
    cma_attr_set_guardsize (&attr, GUARDSIZE);
    
    for (index = 1; index < ITERATIONS; index++) {
    cma_attr_set_stacksize (&attr, STACKSIZE);
	cma_thread_create (
		&thread,
		&attr,
		(cma_t_start_routine)check_stack,
		cma_c_null_ptr);
	cma_thread_join (&thread, &status, &result);
	cma_thread_detach (&thread);

	/*
	 * If the thread exits with an error, then it didn't get the amount
	 * of stack it expected: it already reported the error, so just
	 * exit.
	 */

	if (status == cma_c_term_error)
	    exit (-1);

	}

    cma_attr_delete (&attr);

    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_STK_004.C */
/*  *2     8-NOV-1991 00:37:38 BUTENHOF "Enhance to check size of large stacks" */
/*  *1    18-JUL-1991 12:15:07 CURTIN "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_STK_004.C */
