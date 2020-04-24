/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crvb_ini_002.c,v $
 * Revision 1.1.37.2  1996/02/17  23:30:14  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:46:58  marty]
 *
 * Revision 1.1.37.1  1995/12/11  22:59:10  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:16  root]
 * 
 * Revision 1.1.35.1  1994/04/01  20:09:32  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:13:56  jd]
 * 
 * Revision 1.1.33.1  1993/10/12  19:35:16  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:29:57  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:30:37  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:07:18  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:20:57  devrcs
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
 *	RV_INI
 *
 *  TEST TOPICS:
 *
 *	INI  -  One-time Initialization
 *
 *  ABSTRACT:
 *
 *	Test cma_once initialization with argument and separate init control
 *	blocks.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	8 October 1989
 *
 *  MODIFICATION HISTORY:
 *
 */

/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <dce/cts_test.h>

#define thread_count	20
#define iterations	100
#define control_blocks	3

/*
 * Create 3 separate init control blocks, and three separate flags; the
 * control blocks and flags are associated in pairs (of the same index), to
 * ensure that the same init routine can be called multiple times with
 * different information.
 */
cma_t_once	once[3] = {cma_once_init, cma_once_init, cma_once_init};
cma_t_integer	one_time[3] = {0, 0, 0};

/*
 * This is the one-time initialization action routine used throughout the
 * test program.  It yields to allow contention, and increments the init flag
 * corresponding to the control block that was used (the index number is
 * passed as the argument, and cast to an integer).
 */
static void
init	(cma_t_address	arg)
    {
    cma_t_integer	init_number = (cma_t_integer)arg;


    cma_yield ();
    one_time[init_number]++;
    cma_yield ();
    }

/*
 * Routine that performs one-time init on control block 2.
 */
static void
routine2	(void)
    {
    cma_yield ();
    cma_once (&once[2], init, (cma_t_address)2);
    }

/*
 * Routine that performs one-time init on control block 1.
 */
static void
routine1	(void)
    {
    cma_yield ();
    cma_once (&once[1], init, (cma_t_address)1);
    routine2 ();
    }

/*
 * Performs one-time init on control block 1, and calls "routine1".  It does
 * this in a loop to ensure that each thread can repeat a cma_once call
 * without effect.
 */
static void
thread	(
	cma_t_integer	number)
    {
    cma_t_integer	looper;


    for (looper = 0; looper < iterations; looper++) {
	cma_yield ();
	cma_once (&once[0], init, (cma_t_address)0);
	routine1 ();
	}

    }

main ()
    {
    cma_t_exit_status	exit;
    cma_t_address	result;
    cma_t_integer	i;
    cma_t_thread	threads[thread_count];

    cts_test ("CRVB_INI_002", "Test cma_once argument across multiple threads");

    cma_init ();

    for (i = 1; i < thread_count; i++) {
	cma_thread_create (
		&threads[i],
		&cma_c_null,
		(cma_t_start_routine)thread,
		(cma_t_address)i);
	cma_yield ();
	}

    thread (0);

    for (i = 1; i < thread_count; i++) {
	cma_thread_join (
		&threads[i],
		&exit,
		&result);
	cma_thread_detach (&threads[i]);
	}

    for (i = 0; i < control_blocks; i++) {

	if (one_time[i] == 0)
	    cts_failed ("Initialization %d was not called.", i);
	else if (one_time[i] > 1)
	    cts_failed ("Initialization %d was called multiple times", i);

	}

    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_INI_002_001.C */
/*  *5     5-SEP-1990 09:40:45 BUTENHOF "Convert to new CMA interfaces" */
/*  *4     3-JUL-1990 12:04:08 BUTENHOF "Change copyright" */
/*  *3    21-JUN-1990 08:29:45 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *2     7-JUN-1990 11:37:01 SCALES "Updates to CTS" */
/*  *1    17-MAY-1990 13:58:12 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_INI_002_001.C */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_INI_002.C */
/*  *1     9-OCT-1990 12:38:01 BUTENHOF "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_INI_002.C */
