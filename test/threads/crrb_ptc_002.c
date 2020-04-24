/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crrb_ptc_002.c,v $
 * Revision 1.1.8.2  1996/02/17  23:29:42  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:46:30  marty]
 *
 * Revision 1.1.8.1  1995/12/11  22:57:50  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:34:51  root]
 * 
 * Revision 1.1.6.1  1994/04/01  20:08:44  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:12:06  jd]
 * 
 * Revision 1.1.4.1  1993/10/12  19:34:35  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:29:37  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:28:14  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:04:59  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:20:15  devrcs
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
 *	CRRB_PTC_002
 *
 *  TEST TOPICS:
 *
 *	PTC  -  Per Thread Context
 *
 *  ABSTRACT:
 *
 *	CMA BL3 had a bug where multiple per-thread contexts could
 *	interfere with each other (setting a second cleared the
 *	first).  This test should ensure that the bug doesn't come
 *	back from the dead.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	06 September 1990
 *
 *  MODIFICATION HISTORY:
 *
 */

/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <dce/cts_test.h>

#define ITERATIONS	1000

cma_t_key	key1, key2;
cma_t_thread	thread;

static cma_t_address
thread_code	(cma_t_integer	thd)
    {
    cma_t_address	context_value;
    cma_t_integer	looper, dummy_var;


    for (looper = 0; looper < ITERATIONS; looper++) {
	cma_key_set_context (
		key1,
		(cma_t_address)&dummy_var);
	cma_key_get_context (
		key1,
		&context_value);

	if (context_value != (cma_t_address)&dummy_var)
	    cts_failed (
		    "thread %d context 1 is incorrect",
		    thd
		    );

	cma_key_set_context (
		key2,
		(cma_t_address)&dummy_var);
	cma_key_get_context (
		key2,
		&context_value);

	if (context_value != (cma_t_address)&dummy_var)
	    cts_failed (
		    "thread %d context 2 is incorrect",
		    thd
		    );

	cma_key_get_context (
		key1,
		&context_value);

	if (context_value != (cma_t_address)&dummy_var)
	    cts_failed (
		    "thread %d context 1 changed by setting context 2",
		    thd
		    );

	}

    }

int
main	(int argc, char *argv[], char *envp[])
    {
    cts_test ("CRRB_PTC_002", "Test use of multiple contexts");
    cma_init ();

    cma_key_create (&key1, &cma_c_null, (cma_t_destructor)cma_c_null_ptr);
    cma_key_create (&key2, &cma_c_null, (cma_t_destructor)cma_c_null_ptr);
    cma_thread_create (
	    &thread,
	    &cma_c_null,
	    (cma_t_start_routine)thread_code,
	    (cma_t_address)1);

    thread_code (0);
    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRRB_PTC_002.C */
/*  *1     7-SEP-1990 06:58:29 BUTENHOF "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRRB_PTC_002.C */
