/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cts_test.h,v $
 * Revision 1.1.11.2  1996/02/18  23:08:01  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:30:24  marty]
 *
 * Revision 1.1.11.1  1995/12/11  22:49:15  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:23:09  root]
 * 
 * Revision 1.1.9.1  1994/04/01  20:08:24  jd
 * 	First drop clode cleanup
 * 	[1994/03/29  14:20:43  jd]
 * 
 * Revision 1.1.5.2  1993/02/05  15:26:17  cjd
 * 	Embedded copyright notice
 * 	[1993/02/05  14:47:30  cjd]
 * 
 * Revision 1.1.3.2  1993/02/04  22:18:37  cjd
 * 	Embedded copyright notice
 * 
 * Revision 1.1.2.2  1992/03/18  23:52:22  rps
 * 	Fix cma.h include
 * 	[1992/03/18  18:41:08  rps]
 * 
 * Revision 1.1  1992/01/19  14:35:52  devrcs
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
 *      CMA
 *
 *  ABSTRACT:
 *
 *      This header file defines the interfaces to the CMA testing facility
 *	(CTS) routines.
 *
 *  AUTHORS:
 *
 *      Webb Scales
 *
 *
 *  CREATION DATE:      31 July 1989
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	13 June 1990
 *		use varargs/stdargs for cts_failed & cts_commnt
 *	002	Dave Butenhof	22 June 1990
 *		Fix check macro.
 *	003	Webb Scales	 5 April 1991
 *		Modified result to return success/failure status.
 *	004	Dave Butenhof	03 October 1991
 *		Add cts_exit() function to declare exit from a process (and
 *		avoid the exit handler message) without declaring success or
 *		failure... used for child processes that are forked as part
 *		of a larger test.
 */

#include <dce/cma.h>

#define check(status,string) \
    if (status == -1) perror (string); \

/*
 *  This routine is called at the beginning of a test to initialize the CMA
 *  testing system and log the start of a test.
 */
extern void
cts_test ( char	*, char	*);

/*
 *  This routine may be called at any time during a test to cause the
 *  specified string to be printed to the test log (stdout).
 *
 */
extern void
cts_comment ( char *, ...);

/*
 *  This routine must be called at the end of a test to indicate that
 *  the test is complete (without declaring pass or fail). Used to avoid the
 *  exit handler message for child processes forked as part of a test.
 *
 *  Returns 0 on test success and -1 on test failure.
 */
extern int
cts_exit ();

/*
 *  This routine may be called at any time during a test to indicate that
 *  the current test was failed and to print the reason for failure into the
 *  test log (stdout).
 */
extern void
cts_failed ( char *, ...);

/*
 *  This routine must be called at the end of a test to indicate that
 *  the test is complete.  If no failures were encountered during the test,
 *  a "passed" messages is placed in the test log (stdout).
 *
 *  Returns 0 on test success and -1 on test failure.
 */
extern int
cts_result ();
/*  DEC/CMS REPLACEMENT HISTORY, Element CTS_TEST.H */
/*  *7     3-OCT-1991 11:42:07 BUTENHOF "Modify exit/signal handling" */
/*  *6     9-APR-1991 20:10:08 SCALES "Modify for OSF auto-test" */
/*  *5     3-JUL-1990 12:06:42 BUTENHOF "Change copyright" */
/*  *4    22-JUN-1990 11:20:26 BUTENHOF "Fix ""check"" macro" */
/*  *3    21-JUN-1990 08:34:22 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *2    13-JUN-1990 12:59:40 BUTENHOF "Make cts_commnt and cts_fail take args" */
/*  *1     8-JUN-1990 15:46:11 CURTIN "Self-checking Test Routines" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CTS_TEST.H */
