/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crvb_exc_002.c,v $
 * Revision 1.1.44.2  1996/02/17  23:30:11  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:46:56  marty]
 *
 * Revision 1.1.44.1  1995/12/11  22:59:04  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:14  root]
 * 
 * Revision 1.1.42.1  1994/04/01  20:09:29  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:13:46  jd]
 * 
 * Revision 1.1.40.1  1993/10/12  19:35:11  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:29:55  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:30:21  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:07:03  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:20:54  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  Copyright (c) 1989, 1991
 *  by Digital Equipment Corporation, Maynard Massachusetts.
 *  All rights reserved.
 *
 *  This software is furnished under a license and may be used and copied
 *  only in  accordance  with  the  terms  of  such  license and with the
 *  inclusion of the above copyright notice.  This software or  any other
 *  copies thereof may not be provided or otherwise made available to any
 *  other person.  No title to and ownership of  the  software  is hereby
 *  transferred.
 *
 *  The information in this software is subject to change  without notice and
 *  should  not  be  construed  as  a commitment by DIGITAL Equipment
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
 *	CRVB_EXC_002
 *
 *  TEST TOPICS:
 *
 *	EXC  -  Exceptions
 *
 *  ABSTRACT:
 *
 *	Ensure that an exception terminates the thread which caused
 *	it and no others, and that the proper information is reported
 *	and returned to the joining thread.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	15 December 1989
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Paul Curtin	4 September 1990
 *		Adjust test name in cts_test statement.
 *		Convert to new CMA interface.
 *	002	Dave Butenhof	01 April 1991
 *		Fix use of exception package
 *	003	Dave Butenhof	23 May 1991
 *		Fix compilation errors under new MIPS C compiler.
 */

/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <dce/cts_test.h>

cma_t_thread	thread;
EXCEPTION	junque, foobar;

static cma_t_address blow_up (EXCEPTION *);

main ()
    {
    cma_t_exit_status	status;
    cma_t_address	result;


    cts_test ("CRVB_EXC_002", "Test thread last-chance handler");

    cma_init ();

    EXCEPTION_INIT (junque);
    EXCEPTION_INIT (foobar);
    exc_set_status (&foobar, cma_s_timed_out);
    cts_comment ("Part 1: raise address exception in thread");
    cma_thread_create (
	    &thread,
	    &cma_c_null,
	    (cma_t_start_routine)blow_up,
	    (cma_t_address)&junque);
    cma_thread_join (&thread, &status, &result);
    cma_thread_detach (&thread);

    if (status != cma_c_term_error)
	cts_failed ("Thread didn't terminate with error");

    cts_comment ("Part 2: raise status exception in thread");
    cma_thread_create (
	    &thread,
	    &cma_c_null,
	    (cma_t_start_routine)blow_up,
	    (cma_t_address)&foobar);
    cma_thread_join (&thread, &status, &result);
    cma_thread_detach (&thread);

    if (status != cma_c_term_error)
	cts_failed ("Thread didn't terminate with error");

    cts_result ();

    }

static cma_t_address
blow_up	(
	EXCEPTION	*who)
    {
    RAISE (*who);
    cts_failed ("RAISE didn't terminate thread");
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_EXC_002.C */
/*  *7    29-MAY-1991 18:02:05 BUTENHOF "Fix compilation errors" */
/*  *6     1-APR-1991 17:17:52 BUTENHOF "Fix use of exception package" */
/*  *5     5-SEP-1990 10:53:56 CURTIN "Converted to new CMA interfaces" */
/*  *4     3-JUL-1990 12:03:58 BUTENHOF "Change copyright" */
/*  *3    21-JUN-1990 08:29:12 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *2     7-JUN-1990 11:36:46 SCALES "Updates to CTS" */
/*  *1    17-MAY-1990 13:58:06 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_EXC_002.C */
