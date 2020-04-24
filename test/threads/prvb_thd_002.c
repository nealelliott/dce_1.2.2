/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems
 * Burlington, MA, USA
 * All Rights Reserved
 * (c) Copyright 1990, 1991, 1992
 */
/*
 * HISTORY
 * $Log: prvb_thd_002.c,v $
 * Revision 1.1.53.2  1996/02/17  23:32:02  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:48:28  marty]
 *
 * Revision 1.1.53.1  1995/12/11  23:04:05  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:42  root]
 * 
 * Revision 1.1.51.1  1994/04/01  20:12:22  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:20:15  jd]
 * 
 * Revision 1.1.49.1  1993/10/12  19:38:05  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:31:16  rhw]
 * 
 * Revision 1.1.3.3  1992/12/30  16:55:20  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:13:23  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/29  20:40:10  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:35:39  sekhar]
 * 
 * Revision 1.1  1992/01/19  03:22:17  devrcs
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
 *	PRVB_THD_002
 *
 *  TEST TOPICS:
 *
 *	THD  -  Thread Operations
 *
 *  ABSTRACT:
 *
 *	Create a few threads which loop and compute to
 *	test time slicing.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	01 November 1989
 *
 *  MODIFICATION HISTORY:
 *  001	    Paul Curtin	    31 May 1990
 *	    Converted test to use pthread interface.
 *  002	    Paul Curtin	    5 September 1990
 *	    Fixed test name.
 */

/*
 *  INCLUDE FILES
 */

#include <pthread.h>
#include <dce/cts_test.h>
#include <limits.h>

#define thread_count	101

static pthread_mutex_t	print_mutex;

static pthread_addr_t thread (int);

static void say  (int,int,int);

main ()
    {
    pthread_t	threads[thread_count];
    int	i;
    int status;
    pthread_addr_t	result;
    int	exit;

    cts_test ("PRVB_THD_002", "Test time slicing");

    status = pthread_mutex_init (&print_mutex, pthread_mutexattr_default);
    check(status,"pthread_mutex_init");

    for (i = 1; i < thread_count; i++)
	status = pthread_create (
			&threads[i],
			pthread_attr_default,
			thread,
		    (pthread_addr_t)i);
	check(status,"pthread_create");

    if (pthread_equal(threads[0], threads[1]))
	cts_failed ("Two thread handles are identical.");

    say (0, (int)thread (0), 1);

    for (i = 1; i < thread_count; i++) {
	status = pthread_join (
			threads[i],
			&result);
	check(status,"pthread_join");
		    
#if (_CMA_UNIX_TYPE == _CMA__SVR4) 
	say (i, (int)result, (int)status);
#else
	say (i, (int)result, (int)exit);
#endif
	status = pthread_detach (&threads[i]);
	check(status,"pthread_detach");
	}

    cts_result ();

    }

static pthread_addr_t
thread	(
	int	number)
    {
#define interval	20000
    int	sum, counter;


    counter = interval;

    for (sum = 0; sum < 100000; sum++)
	if (--counter == 0) {
	    counter = interval;
	    say (number, sum, 1);
	    }

    return (pthread_addr_t)sum;
    }

static void
say	(
	int	thread,
	int	count,
	int	status)
    {
    int astatus;

    astatus = pthread_mutex_lock (&print_mutex);
    check(status,"pthread_mutex_lock");
    printf ("PRVB_THD_002 status: thread %d, count %d (%d)\n", thread, count, status);
    astatus = pthread_mutex_unlock (&print_mutex);
    check(status,"pthread_mutex_unlock");
    }
