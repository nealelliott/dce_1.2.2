/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crvb_thd_003.c,v $
 * Revision 1.1.36.2  1996/02/17  23:30:33  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:14  marty]
 *
 * Revision 1.1.36.1  1995/12/11  23:00:04  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:33  root]
 * 
 * Revision 1.1.34.1  1994/04/01  20:10:09  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:15:18  jd]
 * 
 * Revision 1.1.32.1  1993/10/12  19:35:48  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:12  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:32:04  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:21:43  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:21:13  devrcs
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
 *	CRVB_THD_003
 *
 *  TEST TOPICS:
 *
 *	THD  -  Thread Operations
 *
 *  ABSTRACT:
 *
 *	This test endeavors to show that timeslicing is working.
 *	It doesn't assess accuracy, just presence.
 *
 *  AUTHORS:
 *
 *	Webb Scales
 *
 *  CREATION DATE:
 *
 *	10 November 1989
 *
 *  MODIFICATION HISTORY:
 *
 *  001	    Paul Curtin	    4 September 1990
 *	    Adjust test name in cts_test statement.
 *	    Convert to new CMA interface.
 */

/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <dce/cts_test.h>


#define	thread_num  4


cma_t_boolean	running = cma_c_true;	    /* flags */


cma_t_address
kilroy	(
	cma_t_address	dummy)
    {
    return (cma_t_address)running;
    }


main ()
    {
    cma_t_thread	thread[thread_num];
    cma_t_exit_status	exit_status;
    cma_t_address	result;
    cma_t_integer	count;


    cts_test ("CRVB_THD_003", "Test timeslicing");

    cma_init ();

    for (count = 1; count < thread_num; count++)
	cma_thread_create (
		&thread[count],
		&cma_c_null,
		kilroy,
		cma_c_null_ptr);

    cma_delay (5.0);			/* Give timeslicing a chance */

    running = cma_c_false;		/* Time's up! */
    
    for (count = 1; count < thread_num; count++) {
	cma_thread_join (
		&thread[count],
		&exit_status,
		&result);

	printf("Thread #%d %s (exit status = %d).\n",
		count, 
		(result ? "ran" : "did not run"), 
		exit_status);

	if (!result) cts_failed ("Thread did not execute.");
	}

    cts_result ();

    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_THD_003.C */
/*  *5     5-SEP-1990 11:34:45 CURTIN "Converted to use new CMA interface" */
/*  *4     3-JUL-1990 12:05:31 BUTENHOF "Change copyright" */
/*  *3    21-JUN-1990 08:32:05 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *2     7-JUN-1990 11:38:43 SCALES "Updates to CTS" */
/*  *1    17-MAY-1990 13:58:48 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_THD_003.C */
