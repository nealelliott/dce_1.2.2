/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: prvb_err_001.c,v $
 * Revision 1.1.44.2  1996/02/17  23:31:53  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:48:20  marty]
 *
 * Revision 1.1.44.1  1995/12/11  23:03:39  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:34  root]
 * 
 * Revision 1.1.42.1  1994/04/01  20:12:03  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:19:33  jd]
 * 
 * Revision 1.1.40.1  1993/10/12  19:37:49  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:31:08  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:54:42  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:12:50  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:22:08  devrcs
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
 *	PTHREAD Test Programs
 *
 *  FILENAME:
 *
 *	PRVB_ERR_001
 *
 *  TEST TOPICS:
 *
 *	ERR  -  Error Reporting
 *
 *  ABSTRACT:
 *
 *	Cause multiple threads to set and read errno simultaneously,
 *	with liberal "yield"s to ensure context switches.  Ensure
 *	that value of errno is being maintained as per-thread.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	13 February 1990
 *
 *  MODIFICATION HISTORY:
 * 
 *  	001	Paul Curtin	30 May 1990
 *	    	Converted test to use pthread interface.
 *	002	Dave Butenhof	14 June 1990
 *		Replace sprintfs with new parameterized cts calls.
 *	003	Paul Curtin	15 June 1990
 *		Added a cast to make Mips happy
 *	004	Paul Curtin	18 September 1990
 *		Fixed reference to *_crtlx.h  (cma ==> cmalib)
 *	005	Dave Butenhof	11 October 1990
 *		Use cma_errno.h
 */

/*
 *  INCLUDE FILES
 */

#include <pthread.h>
#include <dce/cts_test.h>
#include <cma_errno.h>
#include <cmalib_crtlx.h>

#define thread_count	20

static void thread  (int);

main ()
    {
    pthread_t	threads[thread_count];
    int	i;
    pthread_addr_t	result;
    int status;
    int	exit;


    cts_test ("PRVB_ERR_001", "Test per-thread errno");

    for (i = 1; i < thread_count; i++)
	status = pthread_create (
		&threads[i],
		pthread_attr_default,
		(pthread_startroutine_t)thread,
		(pthread_addr_t)i);
    check(status,"pthread_create");

    thread (0);

    for (i = 1; i < thread_count; i++) {
	status = pthread_join (
		threads[i],
		&result);
	check(status,"pthread_join");
	status = pthread_detach (&threads[i]);
	check(status,"pthread_detach");
	}

    cts_result ();

    }

static void
thread	(
	int	number)
    {
#define MAXIMUM		10000
    int	sum, value1, value2;


    for (sum = 0; sum < MAXIMUM; sum++) {
	value1 = number | (sum << 16);	/* compute value we're going to use */
	errno = value1;
	pthread_yield ();
	value2 = errno;

	if (value1 != value2)
	    cts_failed (
		"Thread %d: errno changed from %08X to %08X.",
		number,
		value1, value2);

	pthread_yield ();
	}

    }
