/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: pvpb_thd_001.c,v $
 * Revision 1.1.34.2  1996/02/17  23:32:22  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:48:44  marty]
 *
 * Revision 1.1.34.1  1995/12/11  23:04:58  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:57  root]
 * 
 * Revision 1.1.32.1  1993/10/12  19:38:30  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:31:30  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:56:30  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:14:32  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:19:48  devrcs
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
 *	PVPB_THD_001.C
 *
 *  TEST TOPICS:
 *
 *	THD  -  Thread Operations
 *
 *  ABSTRACT:
 *
 *	This test measures thread creation peformance.  It forks a thread, 
 *	detaches it so that its stack will be cached, and then yields the 
 *	processor to it.
 *
 *  AUTHORS:
 *
 *	Webb Scales
 *
 *  CREATION DATE:
 *
 *	28 September 1989
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
#include <cma_defs.h>
#include <dce/cts_test.h>

#define	iterations 10000


null_thread () {}	    /* A "null" thread-action routine */


main ()
    {
    pthread_t	    thread;
    pthread_attr_t  attr;
    int		    status;
    int		    i;


    cts_test ("PVPB_THD_001", "Thread forking (cached stacks) performance");

    status = pthread_attr_create (&attr);
    check(status,"pthread_attr_create");

    /* Don't time the first allocation */
    status = pthread_create (&thread, attr, null_thread, (pthread_addr_t)0);
    check(status,"pthread_create");
    status = pthread_detach (&thread);
    check(status,"pthread_detach");
    pthread_yield ();

    printf("Starting...\n");
    lib$init_timer (0);

    for (i = 0; i < iterations; i++) {
	status = pthread_create (&thread, attr, null_thread, (pthread_addr_t)0);
	check(status,"pthread_create");
	status = pthread_detach (&thread);
	check(status,"pthread_detach");
	pthread_yield ();
	}

    printf("For %d iterations of fork/detach/yield (cached stacks):\n", 
	    iterations);
    lib$show_timer (0, 0, 0, 0);

    cts_result ();
    }
