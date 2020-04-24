/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: prvb_exc_002.c,v $
 * Revision 1.1.35.2  1996/02/17  23:31:54  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:48:21  marty]
 *
 * Revision 1.1.35.1  1995/12/11  23:03:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:35  root]
 * 
 * Revision 1.1.33.1  1994/04/01  20:12:05  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:19:38  jd]
 * 
 * Revision 1.1.31.1  1993/10/12  19:37:50  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:31:09  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:54:47  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:12:54  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:22:10  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  Copyright (c) 1989, 1991 by
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
 *	PRVB_EXC_002
 *
 *  TEST TOPICS:
 *
 *	EXC  -  Exceptions
 *
 *  ABSTRACT:
 *
 *	Ensure that an exception terminates the thread which caused
 *	it and no others, and that the proper information is reported
 *	and returned to the joining thread.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	15 December 1989
 *
 *  MODIFICATION HISTORY:
 *	001	Paul Curtin	31 May 1990
 *		Converted test to use pthread interface.
 *	002	Paul Curtin	15 June 1990
 *		Added a couple of casts to join arg.
 *	003	Dave Butenhof	01 April 1991
 *		Fix use of exception package
 */

/*
 *  INCLUDE FILES
 */

#include <pthread.h>
#include <dce/cts_test.h>

pthread_t	thread;
EXCEPTION	junque, foobar;

static pthread_addr_t blow_up  (EXCEPTION *);

main ()
    {
    int			status;
    int			join_status;
    pthread_addr_t	result;


    cts_test ("PRVB_EXC_002", "Test thread last-chance handler");

    EXCEPTION_INIT (junque);
    EXCEPTION_INIT (foobar);
    exc_set_status (&foobar, cma_s_timed_out);
    cts_comment ("Part 1: raise address exception in thread");
    status = pthread_create (&thread, 
			    pthread_attr_default, 
			    blow_up, 
			    (pthread_addr_t)&junque);
    check(status,"pthread_create");
    status = pthread_join (thread, (pthread_addr_t *)&join_status);
    check(status,"pthread_join");
    status = pthread_detach (&thread);
    check(status,"pthread_detach");

    if (join_status != -1)
	cts_failed ("Thread didn't terminate with error");

    cts_comment ("Part 2: raise status exception in thread");
    status = pthread_create (&thread, 
			    pthread_attr_default,
			    blow_up, 
			    (pthread_addr_t)&foobar);
    check(status,"pthread_create");
    status = pthread_join (thread, (pthread_addr_t *)&join_status);
    check(status,"pthread_join");
    status = pthread_detach (&thread);
    check(status,"pthread_detach");

    if (join_status != -1)
	cts_failed ("Thread didn't terminate with error");

    cts_result ();

    }

static pthread_addr_t
blow_up	(
	EXCEPTION	*who)
    {
    RAISE (*who);
    cts_failed ("RAISE didn't terminate thread");
    }
