/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: prvb_thd_003.c,v $
 * Revision 1.1.44.2  1996/02/17  23:32:03  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:48:29  marty]
 *
 * Revision 1.1.44.1  1995/12/11  23:04:08  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:43  root]
 * 
 * Revision 1.1.42.1  1994/04/01  20:12:24  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:20:20  jd]
 * 
 * Revision 1.1.40.1  1993/10/12  19:38:06  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:31:17  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:55:24  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:13:27  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:19:44  devrcs
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
 *	PTHREAD Test Programs
 *
 *  FILENAME:
 *
 *	PRVB_THD_003
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
 *	001	Paul Curtin	30 May 1990
 *		Converted test to use the Pthreads interface.
 *	002	Paul Curtin	5 September 1990
 *		Fixed test name.
 *	003	Dave Butenhof	7 September 1990
 *		Remove CMA-isms
 */

/*
 *  INCLUDE FILES
 */

#include <pthread.h>
#include <dce/cts_test.h>


#define	thread_num  4


unsigned int	running = (int) 1;	    /* flags */


pthread_addr_t
kilroy	(
	pthread_addr_t	dummy)
    {
    return (pthread_addr_t)running;
    }


main ()
    {
    pthread_t	thread[thread_num];
    int	exit_status;
    pthread_addr_t	result;
    int	count;
    struct timespec	five_seconds;


    cts_test ("PRVB_THD_003", "Test timeslicing");

    for (count = 1; count < thread_num; count++)
	pthread_create (
		&thread[count],
		pthread_attr_default,
		kilroy,
		(pthread_addr_t)0);

    five_seconds.tv_sec = 5;
    five_seconds.tv_nsec = 0;
    pthread_delay_np (&five_seconds);	/* Give timeslicing a chance */

    running = (int) 0;		/* Time's up! */
    
    for (count = 1; count < thread_num; count++) {
	exit_status = pthread_join (
		thread[count],
		&result);
	printf(
		"Thread #%d %s (exit status = %d).\n",
		count, 
		(result ? "ran" : "did not run"), 
		exit_status);

	if (!result) cts_failed ("Thread did not execute.");
	}

    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element PRVB_THD_003.C */
/*  *2     7-JUN-1990 11:38:43 SCALES "Updates to CTS" */
/*  *1    17-MAY-1990 13:58:48 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element RV_THD_003.C */
