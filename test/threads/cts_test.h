/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems
 * Burlington, MA, USA
 */
/*
 * HISTORY
 * $Log: cts_test.h,v $
 * Revision 1.1.7.2  1996/02/18  23:08:07  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:30:41  marty]
 *
 * Revision 1.1.7.1  1995/12/11  23:00:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:44  root]
 * 
 * Revision 1.1.5.1  1994/04/01  20:10:31  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:16:11  jd]
 * 
 * Revision 1.1.3.4  1992/12/30  16:33:04  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:08:47  zeliff]
 * 
 * Revision 1.1.3.3  1992/09/29  20:38:40  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:32:51  sekhar]
 * 
 * Revision 1.1.3.2  1992/09/03  14:31:48  bolinger
 * 	Move current 1.0.1-backed file (modified to work with new
 * 	threads source base) from private branch into 1.0.2 tree.
 * 	(See OT defect 5300.)
 * 	[1992/09/02  13:44:46  bolinger]
 * 
 * Revision 1.1.1.2  1992/08/12  21:06:41  bolinger
 * 	Add definitions needed to interface to new elapsed time
 * 	measurement package.  Goal is to insulate functional tests
 * 	from underlying system services used.
 * 
 * Revision 1.1  1992/01/19  03:22:43  devrcs
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

#ifndef	_CTS_TEST_H_
#define	_CTS_TEST_H_

#include <cma.h>
#include <sys/time.h>

#define check(status,string) \
    if (status == -1) perror (string); \

/*
 *  This routine is called at the beginning of a test to initialize the CMA
 *  testing system and log the start of a test.
 */
extern void cts_test  (char *,char *);

/*
 *  This routine may be called at any time during a test to cause the
 *  specified string to be printed to the test log (stdout).
 *
 */
extern void cts_comment  (char *,...);

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
extern void cts_failed  (char *,...);

/*
 *  This routine must be called at the end of a test to indicate that
 *  the test is complete.  If no failures were encountered during the test,
 *  a "passed" messages is placed in the test log (stdout).
 *
 *  Returns 0 on test success and -1 on test failure.
 */
extern int
cts_result ();

/*
 * Since so many tests need to measure elapsed time, centralize
 * here the data type and operations needed to do so.
 */
typedef struct timeval cts_timebuf_t;

/*
 * Function used to record current time value -- we assume
 * it takes a pointer to a cts_timebuf_t.
 */
#if _CMA_UNIX_TYPE == _CMA__SVR4
#define cts_gettime(pbuf)	gettimeofday(pbuf)
#else
#define cts_gettime(pbuf)	gettimeofday(pbuf, (struct timezone *)0)
#endif

/*
 * Function used to convert a time value from internal
 * format to a float.
 */
float cts_timecvt  (cts_timebuf_t *);

/*
 * Function used to measure the elapsed time between two
 * time values.
 */
float cts_timediff  (cts_timebuf_t *,cts_timebuf_t *);

/*
 * Function used to output elapsed time between two time
 * values as a cts_timebuf_t.
 */
void cts_timerecord  (cts_timebuf_t *,cts_timebuf_t *,cts_timebuf_t *);

/*  DEC/CMS REPLACEMENT HISTORY, Element CTS_TEST.H */
/*  *7     3-OCT-1991 11:42:07 BUTENHOF "Modify exit/signal handling" */
/*  *6     9-APR-1991 20:10:08 SCALES "Modify for OSF auto-test" */
/*  *5     3-JUL-1990 12:06:42 BUTENHOF "Change copyright" */
/*  *4    22-JUN-1990 11:20:26 BUTENHOF "Fix ""check"" macro" */
/*  *3    21-JUN-1990 08:34:22 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *2    13-JUN-1990 12:59:40 BUTENHOF "Make cts_commnt and cts_fail take args" */
/*  *1     8-JUN-1990 15:46:11 CURTIN "Self-checking Test Routines" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CTS_TEST.H */
#endif	/* _CTS_TEST_H_ */

