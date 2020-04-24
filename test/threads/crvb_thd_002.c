/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crvb_thd_002.c,v $
 * Revision 1.1.35.2  1996/02/17  23:30:32  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:13  marty]
 *
 * Revision 1.1.35.1  1995/12/11  23:00:00  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:32  root]
 * 
 * Revision 1.1.33.1  1994/04/01  20:10:06  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:15:13  jd]
 * 
 * Revision 1.1.31.1  1993/10/12  19:35:46  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:11  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:31:58  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:21:39  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:21:12  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  Copyright (c) 1989, 1991 by
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
 *	CRVB_THD_002
 *
 *  TEST TOPICS:
 *
 *	THD  -  Thread Operations
 *
 *  ABSTRACT:
 *
 *	Create a few threads which loop and compute to
 *	test time slicing.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	01 November 1989
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Paul Curtin	4 September 1990
 *		Adjust test name in cts_test statement.
 *		Convert to new CMA interfaces.
 *	003	Dave Butenhof	08 May 1991
 *		Cast thread function parameter to proper type in thread
 *		creation (new MIPS C compiler is pickier).
 */

/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <dce/cts_test.h>
#include <limits.h>

#define thread_count	101

static cma_t_mutex	print_mutex;

static cma_t_address thread (cma_t_integer);

static void say (cma_t_integer,cma_t_integer,cma_t_integer);

main ()
    {
    cma_t_thread	threads[thread_count];
    cma_t_integer	i;
    cma_t_address	result;
    cma_t_exit_status	exit;

    cts_test ("CRVB_THD_002", "Test time slicing");

    cma_init ();

    cma_mutex_create (&print_mutex, &cma_c_null);

    for (i = 1; i < thread_count; i++)
	cma_thread_create (
		&threads[i],
		&cma_c_null,
		(cma_t_start_routine)thread,
		(cma_t_address)i);

    say (0, (int)thread (0), 1);

    for (i = 1; i < thread_count; i++) {
	cma_thread_join (
		&threads[i],
		&exit,
		&result);
	say (i, (int)result, (int)exit);
	cma_thread_detach (&threads[i]);
	}

    cts_result ();

    }

static cma_t_address
thread	(
	cma_t_integer	number)
    {
#define interval	20000
    cma_t_integer	sum, counter;


    counter = interval;

    for (sum = 0; sum < 100000; sum++)
	if (--counter == 0) {
	    counter = interval;
	    say (number, sum, 1);
	    }

    return (cma_t_address)sum;
    }

static void
say	(
	cma_t_integer	thread,
	cma_t_integer	count,
	cma_t_integer	status)
    {
    cma_mutex_lock (&print_mutex);
    printf ("CRVB_THD_002 status: thread %d, count %d (%d)\n", thread, count, status);
    cma_mutex_unlock (&print_mutex);
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_THD_002.C */
/*  *6    15-MAY-1991 16:16:54 BUTENHOF "Cast thread function" */
/*  *5     5-SEP-1990 11:33:41 CURTIN "Converted to use new CMA interface" */
/*  *4     3-JUL-1990 12:05:23 BUTENHOF "Change copyright" */
/*  *3    21-JUN-1990 08:31:50 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *2     7-JUN-1990 11:38:34 SCALES "Updates to CTS" */
/*  *1    17-MAY-1990 13:58:45 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_THD_002.C */
