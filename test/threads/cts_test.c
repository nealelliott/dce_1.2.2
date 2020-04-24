/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cts_test.c,v $
 * Revision 1.1.51.2  1996/02/17  23:30:45  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:26  marty]
 *
 * Revision 1.1.51.1  1995/12/11  23:00:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:43  root]
 * 
 * Revision 1.1.49.1  1994/04/01  20:10:30  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:16:07  jd]
 * 
 * Revision 1.1.47.1  1993/10/12  19:36:14  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:22  rhw]
 * 
 * Revision 1.1.3.3  1992/12/30  16:33:00  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:08:42  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/03  14:31:43  bolinger
 * 	Move current 1.0.1-backed file (modified to work with new
 * 	threads source base) from private branch into 1.0.2 tree.
 * 	(See OT defect 5300.)
 * 	[1992/09/02  13:44:41  bolinger]
 * 
 * Revision 1.1.1.3  1992/08/31  20:16:21  bolinger
 * 	In cts_timecvt(), round off microseconds to nearest millisecond.
 * 	No point in pushing the accuracy limits of a float...
 * 
 * Revision 1.1.1.2  1992/08/12  21:05:51  bolinger
 * 	Add functions to implement elapsed time measurement, using
 * 	all of the interfaces required by existing functional tests.
 * 
 * Revision 1.1  1992/01/19  03:20:49  devrcs
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
 *	This file defines the CMA testing facility (CTS) routines.
 *
 *  AUTHORS:
 *
 *      Webb Scales
 *
 *  CREATION DATE:      31 July 1989
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	25 May 1990
 *		Make CMA => CTS changes more pervasive and consistent,
 *		correct use of naming conventions.
 *	002	Dave Butenhof	13 June 1990
 *		use varargs/stdargs for cts_failed & cts_commnt
 *	003	Webb Scales	 5 April 1991
 *		Modified result to return success/failure status.
 *	004	Dave Butenhof	29 May 1991
 *		Declare exit handler so that we can put out an end of test
 *		summary message (if we don't record successful termination).
 *	005	Dave Butenhof	03 October 1991
 *		Add cts_exit() function to declare exit from a process (and
 *		avoid the exit handler message) without declaring success or
 *		failure... used for child processes that are forked as part
 *		of a larger test.
 */


/*
 *  INCLUDE FILES
 */
#include <dce/cma.h>
#include <dce/cts_test.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
# include <stdarg.h>

/*
 *  LOCAL DATA
 */
static char *cts___g_test_name = (char *) cma_c_null_ptr;
static int   cts___g_test_success = cma_c_true;
static int   cts___g_test_completed = cma_c_false;

/*
 *  LOCAL FUNCTIONS
 */
static void cts___test_error  (char *,char *,char *);

static void cts___test_exit  (void);

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *      cts_test
 *
 *	This routine is called at the beginning of a test to initialize the CMA
 *	testing system (CTS) and log the start of a test.
 *
 *  FORMAL PARAMETERS:
 *
 *      test_name   -	name of test about to be executed
 *	test_des    -	brief description of the test
 *
 *  IMPLICIT INPUTS:
 *
 *      cts___g_test_name    -	previous test name (should be cma_c_null_ptr)
 *
 *  IMPLICIT OUTPUTS:
 *
 *      cts___g_test_name    -	current test name
 *      cts___g_test_success -	flag indicating test success (initialized)
 *
 *  FUNCTION VALUE:
 *
 *      none
 *
 *  SIDE EFFECTS:
 *
 *      none
 */
extern void
cts_test	(
	char	*test_name,
	char	*test_des)
    {
    if (cts___g_test_name != (char *) cma_c_null_ptr)
	cts___test_error (
	    "ERROR,\tTest \"%s\":\tPrevious test, \"%s\", was not concluded.\n",
	    test_name, cts___g_test_name);

    cts___g_test_name = test_name;
    cts___g_test_success = cma_c_true;	    /* Assume success until told otherwise */
    cts___g_test_completed = cma_c_false;

    if (atexit (cts___test_exit) == -1)
	cts___test_error (
		"ERROR,\tTest \"%s\":\tFailed to establish exit handler.\n",
		test_name, 0);

    printf ("\nSTART,\tTest \"%s\":\t%s\n", test_name, test_des);
    fflush (stdout);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *      cts_comment
 *
 *	This routine may be called at any time during a test to cause the
 *	specified string to be printed to the test log (stdout).
 *
 *  FORMAL PARAMETERS:
 *
 *	comment	-	character string to be logged
 *
 *  IMPLICIT INPUTS:
 *
 *      cts___g_test_name    -	current test name
 *
 *  IMPLICIT OUTPUTS:
 *
 *      none 
 *
 *  FUNCTION VALUE:
 *
 *      none
 *
 *  SIDE EFFECTS:
 *
 *      none
 */
extern void
cts_comment	(
	char	*comment,
	...)
    {
    char	buffer[512];
    va_list	ap;
    va_start (ap, comment);

    if (cts___g_test_name == (char *) cma_c_null_ptr)
	cts___test_error (
		"\nERROR:\tNo current test in call to cts_comment.\n\n",
		(char *) cma_c_null_ptr,
		(char *) cma_c_null_ptr);

    vsprintf (
	    buffer,
	    comment,
	    ap);
    printf ("COMMNT,\tTest \"%s\":\t%s\n", cts___g_test_name, buffer);
    fflush (stdout);
    va_end (ap);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cts_exit
 *
 *	This routine may be called at the end of a test process to indicate
 *	that the test is complete, without declaring "pass" or "fail". For
 *	example, inside a forked child process.
 *
 *  FORMAL PARAMETERS:
 *
 *	none
 *
 *  IMPLICIT INPUTS:
 *
 *	cts___g_test_name    -	current test name
 * 
 * 	cts___g_test_success -	flag indicating test suceess
 *
 *  IMPLICIT OUTPUTS:
 *
 *	cts___g_test_name    -	current test name (set to cma_c_null_ptr)
 *
 *  FUNCTION VALUE:
 *
 *	0: success; -1:  failure.
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
cts_exit ()
    {
    if (cts___g_test_name == (char *) cma_c_null_ptr)
	cts___test_error (
		"\nERROR:\tNo current test in call to cts_exit.\n\n",
		(char *) cma_c_null_ptr,
		(char *) cma_c_null_ptr);

    cts___g_test_completed = cma_c_true;	/* Mark completion */
    fflush (stdout);
    cts___g_test_name = (char *) cma_c_null_ptr;

    return (cts___g_test_success ? 0 : -1);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *      cts_failed
 *
 *	This routine may be called at any time during a test to indicate that
 *	the current test was failed and to print the reason for failure into the
 *	test log (stdout).
 *
 *	This routine may be called multiple times during the same test.
 *
 *  FORMAL PARAMETERS:
 *
 *	reason	-	character string to be logged
 *
 *  IMPLICIT INPUTS:
 *
 *      cts___g_test_name    -	current test name
 *
 *  IMPLICIT OUTPUTS:
 *
 *      cts___g_test_success -	flag indicating test success (set to false)
 *
 *  FUNCTION VALUE:
 *
 *      none
 *
 *  SIDE EFFECTS:
 *
 *      none
 */
extern void
cts_failed	(
	char	*reason,
	...)
    {
    va_list	ap;
    char	buffer[512];
    va_start (ap, reason);

    if (cts___g_test_name == (char *) cma_c_null_ptr)
	cts___test_error (
		"\nERROR:\tNo current test in call to cts_failed.\n\n",
		(char *) cma_c_null_ptr,
		(char *) cma_c_null_ptr);

    cts___g_test_success = cma_c_false;
    
    vsprintf (
	    buffer,
	    reason,
	    ap);
    printf ("FAILED,\tTest \"%s\":\t%s\n", cts___g_test_name, buffer);
    fflush (stdout);
    va_end (ap);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *      cts_result
 *
 *	This routine must be called at the end of a test to indicate that
 *	the test is complete.  If no failures were encountered during the test,
 *	a "passed" messages is placed in the test log (stdout).
 *
 *  FORMAL PARAMETERS:
 *
 *	none
 *
 *  IMPLICIT INPUTS:
 *
 *      cts___g_test_name    -	current test name
 *      cts___g_test_success -	flag indicating test suceess
 *
 *  IMPLICIT OUTPUTS:
 *
 *      cts___g_test_name    -	current test name (set to cma_c_null_ptr)
 *
 *  FUNCTION VALUE:
 *
 *      0: success; -1:  failure.
 *
 *  SIDE EFFECTS:
 *
 *      none
 */
extern int
cts_result ()
    {
    if (cts___g_test_name == (char *) cma_c_null_ptr)
	cts___test_error (
		"\nERROR:\tNo current test in call to cts_result.\n\n",
		(char *) cma_c_null_ptr,
		(char *) cma_c_null_ptr);

    if (cts___g_test_success)
	printf (
		"PASSED,\tTest \"%s\":\tTest ran successfully.\n", 
		cts___g_test_name);

    cts___g_test_completed = cma_c_true;	/* Mark completion */
    fflush (stdout);
    cts___g_test_name = (char *) cma_c_null_ptr;

    return (cts___g_test_success ? 0 : -1);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *      cts___test_error
 *
 *	This routine is called when one of the CMA test system routines is
 *	called erroniously.  It prints a message to the test log (stdout), and
 *	aborts test execution.
 *
 *  FORMAL PARAMETERS:
 *
 *	format	    -   The error message, as a printf format string for
 *			    optional inclusion of arguments.
 *	arg1, arg2  -   Optional arguments, suitable for passing to printf.
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *      none
 *
 *  FUNCTION VALUE:
 *
 *      none
 *
 *  SIDE EFFECTS:
 *
 *      NOTE:  This function does not return, it aborts execution.
 */
static void
cts___test_error	(
	char	*format,
	char	*arg1,
	char	*arg2)
    {
    printf (format, arg1, arg2);
    fflush (stdout);
    abort();
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *      cts___test_exit
 *
 *	This routine is called when the test program exits; if the test failed
 *	to call cts_result(), then print an end-of-test marker.  This doesn't
 *	necessarily mean that the test failed... a few tests are expected to
 *	terminate via unhandled exceptions, for example.
 *
 *  FORMAL PARAMETERS:
 *
 *	none
 *
 *  IMPLICIT INPUTS:
 *
 *	cts___g_test_completed	Set if cts_result() was called.
 *
 *  IMPLICIT OUTPUTS:
 *
 *      none
 *
 *  FUNCTION VALUE:
 *
 *      none
 *
 *  SIDE EFFECTS:
 *
 *      Print a termination message.
 */
static void
cts___test_exit	(void)
    {

    if (!cts___g_test_completed) {
	printf (
		"EXIT,\tTest \"%s\":\tTest exited without pass or fail.\n\n",
		cts___g_test_name);
	fflush (stdout);
	}

    }

/*
 * Function used to convert a time value from internal
 * format to a float.
 */
float
cts_timecvt	(cts_timebuf_t *buf)
{
	long ms;

	/*
	 * Round off microseconds to milliseconds.
	 */
	ms = (buf->tv_usec + 500) / 1000;
	return ((float)buf->tv_sec + ((float)ms / 1000));
}

/*
 * Function used to measure the elapsed time between two
 * time values.
 */
float
cts_timediff    (
	cts_timebuf_t *old,
    cts_timebuf_t *new)
{
	float secs;
	long usecs;

	secs = new->tv_sec - old->tv_sec;
	usecs = new->tv_usec - old->tv_usec;
	if (usecs < 0) {
		secs -= 1.0;
		usecs += 1000000;
	}
	secs += (float)usecs / 1000000;
	return (secs);
}

/*
 * Function used to output elapsed time between two time
 * values as a cts_timebuf_t.
 */
void
cts_timerecord    (
    cts_timebuf_t *old,
    cts_timebuf_t *new,
    cts_timebuf_t *diff)
{
	diff->tv_sec = new->tv_sec - old->tv_sec;
	diff->tv_usec = new->tv_usec - old->tv_usec;
	if (diff->tv_usec < 0) {
		--diff->tv_sec;
		diff->tv_usec += 1000000;
	}
}

/*  DEC/CMS REPLACEMENT HISTORY, Element CTS_TEST.C */
/*  *10    3-OCT-1991 11:42:00 BUTENHOF "Modify exit/signal handling" */
/*  *9    31-MAY-1991 13:35:11 BUTENHOF "Fix exit handler" */
/*  *8    30-MAY-1991 09:16:24 BUTENHOF "Fix typo" */
/*  *7    30-MAY-1991 08:56:53 BUTENHOF "Get proper header" */
/*  *6    29-MAY-1991 18:02:52 BUTENHOF "Add exit handler" */
/*  *5     9-APR-1991 20:09:57 SCALES "Modify for OSF auto-test" */
/*  *4     3-JUL-1990 12:06:35 BUTENHOF "Change copyright" */
/*  *3    21-JUN-1990 08:34:08 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *2    13-JUN-1990 12:59:35 BUTENHOF "Make cts_commnt and cts_fail take args" */
/*  *1     8-JUN-1990 15:45:35 CURTIN "Self-checking Test Routines" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CTS_TEST.C */
