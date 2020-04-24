/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: pthread_hello.c,v $
 * Revision 1.1.6.2  1996/02/17  23:32:12  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:48:36  marty]
 *
 * Revision 1.1.6.1  1995/12/11  23:04:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:49  root]
 * 
 * Revision 1.1.4.1  1994/04/01  20:12:40  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:21:07  jd]
 * 
 * Revision 1.1.2.2  1992/12/30  16:55:56  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:13:56  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:19:47  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  Copyright © 1989, 1990
 *  by DIGITAL Equipment Corporation, Maynard, Mass.
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
 *	RV_THD_001.C
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
 *
 *	001	Dave Butenhof	7 September 1990
 *		Remove cma_init call.
 */

/*
 *  INCLUDE FILES
 */

#include <pthread.h>
#include <stdio.h>

static pthread_addr_t hello_world  (pthread_addr_t);

main ()
    {
    pthread_t		thread;
    pthread_addr_t	result;


    cts_test ("pthread_hello", "Test basic thread creation");

    pthread_create (
	    &thread,
	    pthread_attr_default,
	    hello_world,
	    (pthread_addr_t)42);

    pthread_join (
	    thread,
	    &result);

    cts_result ();
    }

static pthread_addr_t
hello_world	(
	pthread_addr_t	arg)
    {
    return (pthread_addr_t)printf ("Hello world (%d)\n", arg);
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element PTHREAD_HELLO.C */
/*  *5     7-SEP-1990 09:06:56 BUTENHOF "Remove CMA-isms from pthread tests" */
/*  *4     3-JUL-1990 12:10:56 BUTENHOF "Change copyright" */
/*  *3    21-JUN-1990 08:44:04 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *2     7-JUN-1990 11:33:27 SCALES "Updates to CTS" */
/*  *1    17-MAY-1990 13:56:50 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element PTHREAD_HELLO.C */
