/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crvb_stk_002.c,v $
 * Revision 1.1.41.2  1996/02/17  23:30:28  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:10  marty]
 *
 * Revision 1.1.41.1  1995/12/11  22:59:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:29  root]
 * 
 * Revision 1.1.39.1  1994/04/01  20:09:57  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:14:52  jd]
 * 
 * Revision 1.1.37.1  1993/10/12  19:35:40  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:08  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:31:38  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:08:28  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:21:09  devrcs
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
 *	CRVB_STK_002
 *
 *  TEST TOPICS:
 *
 *	STK  -  Stacks
 *
 *  ABSTRACT:
 *
 *		This test uses cma_stack_check_limit_np to check
 *	if a thread has reached it's stack guard page.  The
 *	check routine survey's a number of bytes in from of 
 *	the current top of stack, and catches any accvios from
 *	touching a guard page.  The check routine then returns
 *	a failing value to the test which raises and overflow
 *	exception, which is carried out in the reserve area the
 *	check routine has left.
 *
 *  AUTHORS:
 *
 *	Paul Curtin
 *
 *  CREATION DATE:
 *
 *	02 July 1990
 *
 *  MODIFICATION HISTORY:
 *	001	Paul Curtin	18 July 1990
 *		Rearranged the way in which exception is caught.
 *	002	Paul Curtin	4 September 1990
 *		- Adjust test name in cts_test statement.
 *		- Convert to new CMA interface.
 *	003	Dave Butenhof	10 September 1990
 *		Fix use of cts calls, and fix function types to avoid
 *		complaints by strict C compilers.
 */

/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <cma_defs.h>
#include <dce/cts_test.h>

cma_t_thread	handle1;
cma_t_thread	handle2;


static cma_t_address
dummy        (
        cma_t_address   arg)
    {
    int x;

    x = 1;
    do {
	cma_yield();
	} while (x == 1);

    }

static cma_t_address
blow_up        (
        cma_t_address   arg)
    {
    cma_t_thread    now_handle;
    cma_t_natural   stacksize;
    cma_t_natural   guardsize;


    cma_attr_get_stacksize(&cma_c_null, &stacksize);
    cma_attr_get_guardsize(&cma_c_null, &guardsize);
    cma_thread_get_self(&now_handle);

    if (!(cma_handle_equal(&now_handle, &handle2)))  {
        printf("!!!!!  Error: We are now on another stack\n");
        printf("Original handle field1: %d   field2: %d \n", handle2.field1,
                                                          handle2.field2);
        printf("We are now field1: %d   field2: %d \n", now_handle.field1,
                                                   now_handle.field2);
        printf("Other threads handle field1: %d   field2: %d\n", handle1.field1,
                                                    handle1.field2);
	cts_failed ("sneaked past guard pages into another stack");
        }

    /*
     *	When stack_check_limit returns false raise and exception, even at 
     *	this point there is still the reserve area available for handlers.
     */

    if (!(cma_stack_check_limit_np(0)))
        RAISE (cma_e_stackovf);

    blow_up (cma_c_null_ptr);
    }

static cma_t_address
catcher        (
        cma_t_address   arg)
    {

    TRY {
	blow_up (arg);
        }
    CATCH (cma_e_stackovf) {
        cts_comment ("caught stack overflow exception");
        }
    CATCH_ALL {
        cts_failed ("received unexpected exception");
        }
    ENDTRY

    }

main ()
    {
    cma_t_integer       i, j;
    cma_t_exit_status   exit_status;
    cma_t_address       result;
    int                 pagesize;

    cts_test ("CRVB_STK_002", "Test stack guards");

    cma_init ();

    cma_thread_create (		    /* A null thread */
	&handle1,
        &cma_c_null,
        dummy,
        cma_c_null_ptr);

    cma_thread_create (		    /* Thread that runs to it's limit */
        &handle2,
        &cma_c_null,
        catcher,
        cma_c_null_ptr);

    cma_yield ();                   /* Let someone run a little */

    cma_thread_join (
        &handle2,
        &exit_status,
        &result);

    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_STK_002.C */
/*  *7    10-SEP-1990 13:27:06 BUTENHOF "Fix redef of blow_up" */
/*  *6    10-SEP-1990 13:16:37 BUTENHOF "Fix conversion errors" */
/*  *5     5-SEP-1990 11:20:17 CURTIN "Converted to use the new CMA interfaces" */
/*  *4    22-AUG-1990 11:04:20 BUTENHOF "Fix pcc errors" */
/*  *3    24-JUL-1990 13:48:24 CURTIN "Caught exception printf a PASS" */
/*  *2     3-JUL-1990 12:05:01 BUTENHOF "Change copyright" */
/*  *1     2-JUL-1990 10:51:37 CURTIN "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_STK_002.C */
