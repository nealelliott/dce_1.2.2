/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: prvb_thd_001.c,v $
 * Revision 1.1.42.2  1996/02/17  23:32:01  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:48:27  marty]
 *
 * Revision 1.1.42.1  1995/12/11  23:04:02  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:41  root]
 * 
 * Revision 1.1.40.1  1994/04/01  20:12:20  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:20:10  jd]
 * 
 * Revision 1.1.38.1  1993/10/12  19:38:03  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:31:15  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:55:18  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:13:19  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:22:16  devrcs
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
 *	PRVB_THD_001.C
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
 *	001	Paul Curtin	31 May 1990
 *		Converted test to use pthread interface.
 *	002	Paul Curtin	 5 September 1990
 *		Fixed test name.
 *	003	Webb Scales	 5 April 1990
 *		Set up for OSF auto-testing.
 */

/*
 *  INCLUDE FILES
 */

#include <pthread.h>
#include <dce/cts_test.h>
#include <stdio.h>

static pthread_addr_t hello_world  (pthread_addr_t);

main ()
    {
    pthread_t	thread;
    int status;
    int	result;


    cts_test ("PRVB_THD_001", "Test thread creation and stuff");

    status = pthread_create (
		    &thread,
		    pthread_attr_default,
		    hello_world,
		    (pthread_addr_t)42);
    check(status,"pthread_create");

    status = pthread_join (
		    thread,
		    (pthread_addr_t *)&result);

    /*
     * For auto-testing check
     */
    exit (cts_result ());
    }

static pthread_addr_t
hello_world	(
	pthread_addr_t	arg)
    {
    return (pthread_addr_t)printf ("Hello world (%d)\n", arg);
    }
