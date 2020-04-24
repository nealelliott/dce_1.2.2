/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crvb_err_001.c,v $
 * Revision 1.1.42.2  1996/02/17  23:30:09  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:46:54  marty]
 *
 * Revision 1.1.42.1  1995/12/11  22:58:59  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:12  root]
 * 
 * Revision 1.1.40.1  1994/04/01  20:09:27  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:13:42  jd]
 * 
 * Revision 1.1.38.1  1993/10/12  19:35:06  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:29:53  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:30:08  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:06:53  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:20:52  devrcs
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
 *	CMA Test Programs
 *
 *  FILENAME:
 *
 *	CRVB_ERR_001
 *
 *  TEST TOPICS:
 *
 *	ERR  -  Error Reporting
 *
 *  ABSTRACT:
 *
 *	Cause multiple threads to set and read errno simultaneously, with
 *	liberal "yield"s to ensure context switches.  Ensure that value of
 *	errno is being maintained as per-thread.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	13 February 1990
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	14 June 1990
 *		Replace sprintfs with new parameterized cts calls.
 *	002	Paul Curtin	4 September 1990
 *		Convert to new CMA interface. Adjust cts_test statement to
 *		reflect CMA/Pthread naming.
 *	003	Paul Curtin	18 September 1990
 *		Fix reference to *_ctlx.h  (cma ==> cmalib)
 *	004	Dave Butenhof	11 October 1990
 *		errno is now in cma_errno.h
 *	005	Dave Butenhof	23 May 1991
 *		Fix compilation errors under new MIPS C compiler.
 */
/*	
/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <dce/cts_test.h>
#include <cma_errno.h>

#define thread_count	20

static cma_t_address thread (cma_t_integer);

main ()
    {
    cma_t_thread	threads[thread_count];
    cma_t_integer	i;
    cma_t_address	result;
    cma_t_exit_status	exit;


    cts_test ("CRVB_ERR_001", "Test per-thread errno");

    cma_init ();

    for (i = 1; i < thread_count; i++)
	cma_thread_create (
		&threads[i],
		&cma_c_null,
		(cma_t_start_routine)thread,
		(cma_t_address)i);

    thread (0);

    for (i = 1; i < thread_count; i++) {
	cma_thread_join (
		&threads[i],
		&exit,
		&result);
	cma_thread_detach (&threads[i]);
	}

    cts_result ();

    }

static cma_t_address
thread	(
	cma_t_integer	number)
    {
#define MAXIMUM		10000
    cma_t_integer	sum, value1, value2;


    for (sum = 0; sum < MAXIMUM; sum++) {
	value1 = number | (sum << 16);	/* compute value we're going to use */
	errno = value1;
	cma_yield ();
	value2 = errno;

	if (value1 != value2)
	    cts_failed (
		    "Thread %d: errno changed from %08X to %08X.",
		    number,
		    value1, value2);

	cma_yield ();
	}

    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_ERR_001.C */
/*  *10   29-MAY-1991 18:02:00 BUTENHOF "Fix compilation errors" */
/*  *9    11-OCT-1990 13:03:56 BUTENHOF "Fix use of errno" */
/*  *8    18-SEP-1990 15:19:19 CURTIN "Fixed reference to crtlx" */
/*  *7     5-SEP-1990 14:09:56 CURTIN "Converted to new CMA interfaces" */
/*  *6     3-JUL-1990 12:03:45 BUTENHOF "Change copyright" */
/*  *5    21-JUN-1990 08:28:25 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *4    14-JUN-1990 07:40:45 BUTENHOF "Get rid of sprintfs" */
/*  *3    11-JUN-1990 09:30:24 BUTENHOF "Fix MIPS compile warnings" */
/*  *2     7-JUN-1990 11:36:10 SCALES "Updates to CTS" */
/*  *1    17-MAY-1990 13:57:55 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_ERR_001.C */
