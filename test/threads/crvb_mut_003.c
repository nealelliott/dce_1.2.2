/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crvb_mut_003.c,v $
 * Revision 1.1.40.2  1996/02/17  23:30:17  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:01  marty]
 *
 * Revision 1.1.40.1  1995/12/11  22:59:19  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:20  root]
 * 
 * Revision 1.1.38.1  1994/04/01  20:09:38  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:14:10  jd]
 * 
 * Revision 1.1.36.1  1993/10/12  19:35:21  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:00  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:30:53  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:07:33  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:21:00  devrcs
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
 *	CRVB_MUT_003
 *
 *  TEST TOPICS:
 *
 *	MUT  -  Mutex Operations
 *
 *  ABSTRACT:
 *
 *	Test that the CMA global lock guarantees exclusive access to
 *	a generic library routine.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	27 June 1990
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Webb Scales	1990
 *		Corrected filename references.
 *	002	Dave Butenhof	5 September 1990
 *		Convert to new CMA interfaces.
 */

/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <dce/cts_test.h>

#define THREADS	9
#define LOOP	20
#define INNER	100

cma_t_thread	worker[THREADS+1];
cma_t_integer	owner;

static cma_t_address bee (cma_t_integer);

int
main	(int argc, char *argv[], char *envp[])
    {
    cma_t_integer	thd;


    cts_test ("CRVB_MUT_003", "Test global lock primitive");
    cma_init ();

    owner = -1;

    for (thd = 1; thd <= THREADS; thd++)
	cma_thread_create (
		&worker[thd],
		&cma_c_null,
		(cma_t_start_routine)bee,
		(cma_t_address)thd);

    bee (0);

    cts_result ();
    }

static cma_t_address
bee	(cma_t_integer	thd_num)
    {
    cma_t_integer	i, j;


    for (i = 0; i < LOOP; i++) {
	cma_lock_global ();

	if (owner != -1)
	    cts_failed (
		    "thread %d entered protected region while %d owned it",
		    thd_num,
		    owner);

	owner = thd_num;
	cma_yield ();
	cma_lock_global ();

	for (j = 0; j < INNER; j++) {
	    cma_yield ();

	    if (owner != thd_num)
		cts_failed (
			"thread %d lost ownership unexpectedly to %d",
			thd_num,
			owner);

	    }

	cma_unlock_global ();
	cma_yield ();

	if (owner != thd_num)
	    cts_failed (
		    "thread %d entered protected region before %d released it",
		    owner,
		    thd_num);

	owner = -1;
	cma_unlock_global ();
	}

    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_MUT_003.C */
/*  *5     5-SEP-1990 09:41:06 BUTENHOF "Convert to new CMA interfaces" */
/*  *4    18-JUL-1990 16:28:00 SCALES "Fix filename references" */
/*  *3     3-JUL-1990 12:04:24 BUTENHOF "Change copyright" */
/*  *2     2-JUL-1990 15:35:00 BUTENHOF "Put in the element" */
/*  *1     2-JUL-1990 15:26:49 BUTENHOF "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_MUT_003.C */
