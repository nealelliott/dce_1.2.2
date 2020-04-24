/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crvb_thd_001.c,v $
 * Revision 1.1.44.2  1996/02/17  23:30:31  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:12  marty]
 *
 * Revision 1.1.44.1  1995/12/11  22:59:57  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:31  root]
 * 
 * Revision 1.1.42.1  1994/04/01  20:10:04  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:15:09  jd]
 * 
 * Revision 1.1.40.1  1993/10/12  19:35:45  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:11  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:31:53  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:21:35  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:21:11  devrcs
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
 *	CRVB_THD_001.C
 *
 *  TEST TOPICS:
 *
 *	THD - Threads
 *
 *  ABSTRACT:
 *
 *	This module tests thread creation and stuff.
 *
 *  AUTHORS:
 *
 *	Webb Scales and Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	27 September 1989
 *
 *  MODIFICATION HISTORY:
 *
 *  001	    Paul Curtin	    4 September 1990
 *	    Adjust test name in cts_test statement.
 *	    Convert to new CMA interfaces.
 */

/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <stdio.h>

static cma_t_address hello_world (cma_t_address);

main ()
    {
    cma_t_thread	thread;
    cma_t_exit_status	exit;
    cma_t_integer	result;


    cts_test ("CRVB_THD_001", "Test thread creation and stuff");

    cma_init ();			/* Initialize CMA */

    cma_thread_create (
	    &thread,
	    &cma_c_null,
	    hello_world,
	    (cma_t_address)42);

    cma_thread_join (
	    &thread,
	    &exit,
	    (cma_t_address *)&result);

    cts_result ();
    }

static cma_t_address
hello_world	(
	cma_t_address	arg)
    {
    return (cma_t_address)printf ("Hello world (%d)\n", arg);
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_THD_001.C */
/*  *5     5-SEP-1990 11:30:58 CURTIN "Converted to use new CMA interface" */
/*  *4     3-JUL-1990 12:05:16 BUTENHOF "Change copyright" */
/*  *3    21-JUN-1990 08:31:36 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *2     7-JUN-1990 11:38:25 SCALES "Updates to CTS" */
/*  *1    17-MAY-1990 13:58:42 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_THD_001.C */
