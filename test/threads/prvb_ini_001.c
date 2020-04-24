/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: prvb_ini_001.c,v $
 * Revision 1.1.36.2  1996/02/17  23:31:55  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:48:22  marty]
 *
 * Revision 1.1.36.1  1995/12/11  23:03:43  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:36  root]
 * 
 * Revision 1.1.34.1  1994/04/01  20:12:07  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:19:42  jd]
 * 
 * Revision 1.1.32.1  1993/10/12  19:37:52  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:31:09  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:54:51  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:12:57  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:19:42  devrcs
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
 *	PRVB_INI_001
 *
 *  TEST TOPICS:
 *
 *	INI  -  One-time Initialization
 *
 *  ABSTRACT:
 *
 *	Create several threads; using cma_yield, attempt to cause
 *	all to contend for a single one-time initialization routine.
 *	Test that the routine is executed once and only once despite
 *	multiple calls from multiple threads.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	16 October 1989
 *
 *  MODIFICATION HISTORY:
 * 
 *	001	Paul Curtin	31 May 1990
 *		Converted test to use pthread interface.
 */

/*
 *  INCLUDE FILES
 */

#include <pthread.h>
#include <dce/cts_test.h>

#define thread_count	20

pthread_once_t	once = pthread_once_init;
int	one_time = 0;

static void thread  (int);

static void
init ();

main ()
    {
    int	exit;
    pthread_addr_t	result;
    int	i;
    int	status;
    pthread_t	threads[thread_count];

    cts_test ("PRVB_INI_001", "Test one-time init across multiple threads");

    for (i = 0; i < thread_count; i++) {
	status = pthread_create (
			&threads[i],
			pthread_attr_default,
			(pthread_startroutine_t)thread,
			(pthread_addr_t)i);
	check(status,"pthread_create");
	pthread_yield ();
	}

    pthread_yield ();
    status = pthread_once (&once, init);
    check(status,"pthread_once");
    pthread_yield ();

    for (i = 0; i < thread_count; i++) {
	status = pthread_join (
			threads[i],
			&result);
	check(status,"pthread_join");
	status = pthread_detach (&threads[i]);
	check(status,"pthread_detach");
	}

    if (one_time != 1)
	cts_failed ("Initialization was called multiple times");

    cts_result ();
    }

static void
thread	(
	int	number)
    {
    int status;

    pthread_yield ();
    status = pthread_once (&once, init);
    check(status,"pthread_once");
    pthread_yield ();
    status = pthread_once (&once, init);
    check(status,"pthread_once");
    pthread_yield ();
    status = pthread_once (&once, init);
    check(status,"pthread_once");
    pthread_yield ();
    }

static void
init ()

{
    pthread_yield ();
    one_time++;
    pthread_yield ();
    }
