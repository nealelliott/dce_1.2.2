/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crvb_thd_004.c,v $
 * Revision 1.1.37.2  1996/02/17  23:30:34  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:15  marty]
 *
 * Revision 1.1.37.1  1995/12/11  23:00:06  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:34  root]
 * 
 * Revision 1.1.35.1  1994/04/01  20:10:11  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:15:22  jd]
 * 
 * Revision 1.1.33.1  1993/10/12  19:35:50  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:13  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:32:10  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:21:47  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:21:14  devrcs
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
 *	CRVB_THD_004
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
 *	001	Paul Curtin	4 September 1990
 *		Adjust test name in cts_test statement.
 *		Convert to new CMA interfaces.
 *	002	Dave Butenhof & Webb Scales	29 January 1991
 *		Try to make the test more tolerant of loaded systems, by
 *		letting it run a little longer.
 */

/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <dce/cts_test.h>


#define	thread_num	2
#define	iterations	1000000


cma_t_boolean	running = cma_c_true;	    /* flags */
cma_t_boolean	someone_ran = cma_c_false;
cma_t_mutex	someranmu;

cma_t_address
kilroy	(
	cma_t_address	dummy)
    {
    cma_t_boolean	we_ran;


    we_ran = running;
    cma_mutex_lock (&someranmu);
    someone_ran = cma_c_true;
    cma_mutex_unlock (&someranmu);
    return (cma_t_address)we_ran;
    }

main ()
    {
    cma_t_thread	thread[thread_num];
    cma_t_exit_status	exit_status;
    cma_t_address	result;
    cma_t_integer	count, i;
    cma_t_boolean	someflag = cma_c_false;


    cts_test ("CRVB_THD_004", "Test timeslicing");

    cma_init ();
    cma_mutex_create (&someranmu, &cma_c_null);

    for (count = 0; count < thread_num; count++)
	cma_thread_create (
		&thread[count],
		&cma_c_null,
		kilroy,
		cma_c_null_ptr);

    /*
     * This is a little weird, since technically trying to lock a mutex in
     * this wait loop has the potential of blocking the thread, which would
     * allow the test threads to run even if timeslicing doesn't work.
     * However, we know that nobody else is locking the mutex, so the only
     * way it can be locked is if at least one of the test threads HAS
     * started, in which case the test has really passed anyway.
     */
    for (i = 0; i < iterations && !someflag; i++) {
	cma_mutex_lock (&someranmu);
	someflag = someone_ran;
	cma_mutex_unlock (&someranmu);
	}

    running = cma_c_false;		/* Time's up! */
    
    for (count = 0; count < thread_num; count++) {
	cma_thread_join (
		&thread[count],
		&exit_status,
		&result);

	if (!result) cts_failed ("Thread %d did not execute.", count);
	}

    cts_result ();

    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_THD_004.C */
/*  *6    29-JAN-1991 17:57:58 BUTENHOF "Try to improve tests' tolerance to loaded systems" */
/*  *5     5-SEP-1990 11:35:50 CURTIN "Converted to use new CMA interface" */
/*  *4     3-JUL-1990 12:05:40 BUTENHOF "Change copyright" */
/*  *3    21-JUN-1990 08:32:17 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *2     7-JUN-1990 11:38:50 SCALES "Updates to CTS" */
/*  *1    17-MAY-1990 13:58:51 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_THD_004.C */
