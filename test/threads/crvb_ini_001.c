/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crvb_ini_001.c,v $
 * Revision 1.1.36.2  1996/02/17  23:30:13  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:46:57  marty]
 *
 * Revision 1.1.36.1  1995/12/11  22:59:08  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:16  root]
 * 
 * Revision 1.1.34.1  1994/04/01  20:09:31  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:13:51  jd]
 * 
 * Revision 1.1.32.1  1993/10/12  19:35:14  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:29:56  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:30:32  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:07:12  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:20:56  devrcs
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
 *	RV_INI_001
 *
 *  TEST TOPICS:
 *
 *	INI  -  One-time Initialization
 *
 *  ABSTRACT:
 *
 *	Create several threads; using cma_yield, attempt to cause
 *	all to contend for a single one-time initialization routine.
 *	Test that the routine is executed once and only once despite
 *	multiple calls from multiple threads.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	16 October 1989
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	5 September 1990
 *		Convert to new CMA interfaces.
 *	002	Webb Scales	30 October 1990
 *		Add casts for cma_once argument parameter.
 */

/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <dce/cts_test.h>

#define thread_count	20

cma_t_once	once = cma_once_init;
cma_t_integer	one_time = 0, init_arg = 12345;

static void thread (cma_t_integer);

static void init (cma_t_address);

main ()
    {
    cma_t_exit_status	exit;
    cma_t_address	result;
    cma_t_integer	i;
    cma_t_thread	threads[thread_count];

    cts_test ("CRVB_INI_001", "Test one-time init across multiple threads");

    cma_init ();

    for (i = 0; i < thread_count; i++) {
	cma_thread_create (
		&threads[i],
		&cma_c_null,
		(cma_t_start_routine)thread,
		(cma_t_address)i);
	cma_yield ();
	}

    cma_yield ();
    cma_once (&once, init, (cma_t_address)&init_arg);
    cma_yield ();

    for (i = 0; i < thread_count; i++) {
	cma_thread_join (
		&threads[i],
		&exit,
		&result);
	cma_thread_detach (&threads[i]);
	}

    if (one_time == 0)
	cts_failed ("Initialization was not called");
    else if (one_time > 1)
	cts_failed ("Initialization was called multiple times");

    cts_result ();
    }

static void
thread	(
	cma_t_integer	number)
    {
    cma_init ();			/* Test idempotency of cma_init */
    cma_yield ();
    cma_once (&once, init, (cma_t_address)&init_arg);
    cma_yield ();
    cma_once (&once, init, (cma_t_address)&init_arg);
    cma_yield ();
    cma_once (&once, init, (cma_t_address)&init_arg);
    cma_yield ();
    }

static void
init	(cma_t_address	arg)
    {

    if (*((cma_t_integer *)arg) != init_arg)
	cts_failed ("Incorrect argument to init routine");

    cma_init ();			/* Test idempotency of cma_init */
    cma_yield ();
    one_time++;
    cma_yield ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_INI_001.C */
/*  *7    30-OCT-1990 21:56:15 SCALES "fix type mismatch" */
/*  *6    11-OCT-1990 06:40:02 BUTENHOF "cma_once now takes an argument" */
/*  *5     5-SEP-1990 09:40:45 BUTENHOF "Convert to new CMA interfaces" */
/*  *4     3-JUL-1990 12:04:08 BUTENHOF "Change copyright" */
/*  *3    21-JUN-1990 08:29:45 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *2     7-JUN-1990 11:37:01 SCALES "Updates to CTS" */
/*  *1    17-MAY-1990 13:58:12 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_INI_001.C */
