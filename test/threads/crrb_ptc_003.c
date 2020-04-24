/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crrb_ptc_003.c,v $
 * Revision 1.1.8.2  1996/02/17  23:29:43  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:46:31  marty]
 *
 * Revision 1.1.8.1  1995/12/11  22:57:52  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:34:52  root]
 * 
 * Revision 1.1.6.1  1994/04/01  20:08:46  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:12:11  jd]
 * 
 * Revision 1.1.4.1  1993/10/12  19:34:36  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:29:38  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:28:19  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:05:04  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:20:16  devrcs
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
 *	CRRB_PTC_003
 *
 *  TEST TOPICS:
 *
 *	PTC  -  Per Thread Context
 *
 *  ABSTRACT:
 *
 *	Snapshots of CMA BL4 blew up after 7 sets of cma_key_create,
 *	cma_key_set_context calls, due to a bug in clearing the
 *	unused elements of the new context array.  This test will
 *	verify that the bug does not recur.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	29 October 1990
 *
 *  MODIFICATION HISTORY:
 *
 */

/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <dce/cts_test.h>
#include <stdio.h>

#define ARRAY_SIZE 10

cma_t_key  key_array [ARRAY_SIZE];

int
main	(int argc, char *argv[], char *envp[])
    {
    int i;
    int *int_ptr;
    int dummy_val [ARRAY_SIZE];


    cts_test ("CRRB_PTC_003", "Test repetitive create/set context");
    cma_init ();

    for (i = 0; i < ARRAY_SIZE; i++) {
	printf ("Creating key %d\n", i);
	cma_key_create (
		&key_array[i],
		&cma_c_null,
		(cma_t_destructor)cma_c_null_ptr);
	printf ("Setting context for key %d\n", i);
        cma_key_set_context (key_array[i], (cma_t_address)(&dummy_val[i]));
	}

    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRRB_PTC_003.C */
/*  *2    30-OCT-1990 00:34:29 BUTENHOF "Fix MIPS C type mismatch" */
/*  *1    29-OCT-1990 21:40:57 BUTENHOF "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRRB_PTC_003.C */
