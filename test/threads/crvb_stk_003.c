/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crvb_stk_003.c,v $
 * Revision 1.1.102.2  1996/02/17  23:30:29  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:11  marty]
 *
 * Revision 1.1.102.1  1995/12/11  22:59:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:29  root]
 * 
 * Revision 1.1.100.1  1994/04/01  20:09:58  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:14:59  jd]
 * 
 * Revision 1.1.98.1  1993/10/12  19:35:41  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:09  rhw]
 * 
 * Revision 1.1.8.2  1993/06/14  20:16:33  sommerfeld
 * 	define exc_sigs_are_xlated macro
 * 	to always return 0 for __hpux.
 * 	[1993/05/06  20:07:12  hopkins]
 * 
 * Revision 1.1.4.2  1992/12/30  16:31:43  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:08:32  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/11  17:16:50  bolinger
 * 	Fix OT defect 2895:  add test for whether CMA was built with
 * 	_CMA_SYNC_KILL_ code enabled.  If so, this test is dummied
 * 	out.
 * 	[1992/05/11  13:49:55  bolinger]
 * 
 * Revision 1.1  1992/01/19  03:21:10  devrcs
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
 *	CRVB_STK_003
 *
 *  TEST TOPICS:
 *
 *	STK  -  Stacks
 *
 *  ABSTRACT:
 *
 *	This test, useing cma_check_stack_limit_np tests stack
 *	overflow in the main thread.  We expect to get a 
 *	bad status, eventually, from the check and then we
 *	raise an overflow exception.
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
 *  001	    Paul Curtin	    18 July 1990
 *	    Rearranged the way in which exception is caught.
 *  002	    Paul Curtin	    4 September 1990
 *	    Adjust test name in cts_test statement.
 *	    Convert to new CMA interface.
 *  003	    Paul Curtin	    30 November 1990
 *	    Adjust cts_* calls remove printf's.
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
                                                                          

catcher        (
        cma_t_address   arg)
    {

    TRY {
        blow_up(0);
        }
    CATCH (cma_e_stackovf) {
	/* If we get here than all is as expected. */
        }
    CATCH_ALL {
        cts_failed ("\nFailed: received unexpected exception\n");
        }
    ENDTRY

    }

blow_up        (
        cma_t_address   arg)
    {
    cma_t_thread    now_handle;
    cma_t_natural   stacksize;
    cma_t_natural   guardsize;


    cma_attr_get_stacksize(&cma_c_null, &stacksize);
    cma_attr_get_guardsize(&cma_c_null, &guardsize);

    if (!(cma_stack_check_limit_np(0)))
        RAISE (cma_e_stackovf);

    blow_up(cma_c_null_ptr);
    }




main ()
    {
    cma_t_integer       i, j;
    cma_t_exit_status   exit_status;
    cma_t_address       result;
    int                 pagesize;
	sigset_t			set;

    cts_test ("CRVB_STK_003", "Test stack guards");

    cma_init ();

	/*
	 * 1.0.1 _CMA_SYNC_KILL_ hack
	 */
    sigemptyset(&set);
    sigaddset(&set, SIGSEGV);
    sigaddset(&set, SIGBUS);

    cma_thread_create (
	    &handle1,
            &cma_c_null,
            dummy,
            cma_c_null_ptr);

    cma_thread_create (
            &handle2,
            &cma_c_null,
            dummy,
            cma_c_null_ptr);

    catcher(cma_c_null_ptr);

    cts_result ();

    }

/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_STK_003.C */
/*  *6    30-NOV-1990 13:47:59 CURTIN "fixed cts_* function misuse" */
/*  *5     5-SEP-1990 16:00:55 CURTIN "Convert to use new CMA interface" */
/*  *4    22-AUG-1990 11:14:53 BUTENHOF "Fix args for pcc" */
/*  *3    24-JUL-1990 13:48:53 CURTIN "Cuaght exception; printf a PASS" */
/*  *2     3-JUL-1990 12:05:09 BUTENHOF "Change copyright" */
/*  *1     2-JUL-1990 16:16:10 CURTIN "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_STK_003.C */
