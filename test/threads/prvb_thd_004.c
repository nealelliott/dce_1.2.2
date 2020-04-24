/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: prvb_thd_004.c,v $
 * Revision 1.1.35.2  1996/02/17  23:32:04  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:48:29  marty]
 *
 * Revision 1.1.35.1  1995/12/11  23:04:11  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:43  root]
 * 
 * Revision 1.1.33.1  1994/04/01  20:12:26  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:20:26  jd]
 * 
 * Revision 1.1.31.1  1993/10/12  19:38:08  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:31:18  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:55:28  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:13:30  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:19:45  devrcs
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
 *	PRVB_THD_004
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
 *	001	Paul Curtin	31 May 1990
 *		Converted test to use pthread interface.
 *	002	Paul Curtin	5 September 1990
 *		Fixed test name.
 *	003	Dave Butenhof & Webb Scales	29 January 1991
 *		Try to make the test more tolerant of loaded systems, by
 *		letting it run a little longer.
 */

/*
 *  INCLUDE FILES
 */

#include <pthread.h>
#include <dce/cts_test.h>


#define	thread_num	2
#define	iterations	1000000


unsigned int	running = 1;	    /* flags */
unsigned int	someone_ran = 0;
pthread_mutex_t	someranmu;


pthread_addr_t
kilroy	(
	pthread_addr_t	dummy)
    {
    int	status, we_ran;

    
    we_ran = running;
    status = pthread_mutex_lock (&someranmu);
    check (status, "pthread_mutex_lock");
    someone_ran = 1;
    status = pthread_mutex_unlock (&someranmu);
    check (status, "pthread_mutex_unlock");
    return (pthread_addr_t)we_ran;
    }


main ()
    {
    pthread_t		thread[thread_num];
    int			status;
    int			exit_status;
    pthread_addr_t	result;
    int			count, i;
    unsigned int	someflag = 0;


    cts_test ("PRVB_THD_004", "Test timeslicing");
    pthread_mutex_init (&someranmu, pthread_mutexattr_default);

    for (count = 0; count < thread_num; count++)
	status = pthread_create (
		&thread[count],
		pthread_attr_default,
		kilroy,
		(pthread_addr_t)0);
	check(status,"pthread_create");

    /*
     * This is a little weird, since technically trying to lock a mutex in
     * this wait loop has the potential of blocking the thread, which would
     * allow the test threads to run even if timeslicing doesn't work.
     * However, we know that nobody else is locking the mutex, so the only
     * way it can be locked is if at least one of the test threads HAS
     * started, in which case the test has really passed anyway.
     */
    for (i = 0; i < iterations && !someflag; i++) {
	pthread_mutex_lock (&someranmu);
	someflag = someone_ran;
	pthread_mutex_unlock (&someranmu);
	}

    running = (int) 0;		/* Time's up! */
    
    for (count = 0; count < thread_num; count++) {
	exit_status = pthread_join (
		thread[count],
		&result);  
	check (exit_status, "pthread_join");

	if (!result) cts_failed ("Thread %d did not execute.", count);
	}

    cts_result ();

    }
