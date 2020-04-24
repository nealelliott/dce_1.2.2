/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cma_test.c,v $
 * Revision 1.1.6.2  1996/02/17  23:29:27  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:46:14  marty]
 *
 * Revision 1.1.6.1  1995/12/11  22:57:09  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:34:38  root]
 * 
 * Revision 1.1.4.1  1994/04/01  20:08:27  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:23:34  jd]
 * 
 * Revision 1.1.2.2  1992/12/30  16:27:31  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:03:57  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:19:33  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*
 *  FACILITY:
 *
 *      CMA
 *
 *  ABSTRACT:
 *
 *	This file defines the CMA testing facility routines.
 *
 *  AUTHORS:
 *
 *      Webb Scales
 *
 *  CREATION DATE:      31 July 1989
 *
 *  MODIFICATION HISTORY:
 *
 */


/*
 *  INCLUDE FILES
 */
#include <dce/cma.h>
#include <cma_test.h>
#include <stdio.h>

/*
 *  LOCAL DATA
 */
static char *cma__g_test_name = (char *) cma_c_null_ptr;
static int   cma__g_test_success = cma_c_true;


/*
 *  LOCAL FUNCTIONS
 */
static void
cma___test_error (char	*,char	*,char	*);


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *      cma__test
 *
 *	This routine is called at the beginning of a test to initialize the CMA
 *	testing system and log the start of a test.
 *
 *  FORMAL PARAMETERS:
 *
 *      test_name   -	name of test about to be executed
 *	test_des    -	brief description of the test
 *
 *  IMPLICIT INPUTS:
 *
 *      cma__g_test_name    -	previous test name (should be cma_c_null_ptr)
 *
 *  IMPLICIT OUTPUTS:
 *
 *      cma__g_test_name    -	current test name
 *      cma__g_test_success -	flag indicating test success (initialized)
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
cma__test(
	char	*test_name,
	char	*test_des)
    {
    if (cma__g_test_name != (char *) cma_c_null_ptr)
	cma___test_error (
	    "ERROR,\tTest \"%s\":\tPrevious test, \"%s\", was not concluded.\n",
	    test_name, cma__g_test_name);

    cma__g_test_name = test_name;
    cma__g_test_success = cma_c_true;	    /* Assume success until told otherwise */

    printf ("\nSTART,\tTest \"%s\":\t%s\n", test_name, test_des);
    fflush (stdout);
}



/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *      cma__comment
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
 *      cma__g_test_name    -	current test name
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
cma__comment(
	char	*comment)
    {
    if (cma__g_test_name == (char *) cma_c_null_ptr)
	cma___test_error (
		"\nERROR:\tNo current test in call to cma__comment.\n\n",
		(char *) cma_c_null_ptr,
		(char *) cma_c_null_ptr);

    printf ("COMMNT,\tTest \"%s\":\t%s\n", cma__g_test_name, comment);
    fflush (stdout);
}



/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *      cma__failed
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
 *      cma__g_test_name    -	current test name
 *
 *  IMPLICIT OUTPUTS:
 *
 *      cma__g_test_success -	flag indicating test success (set to false)
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
cma__failed(
	char	*reason)
    {
    if (cma__g_test_name == (char *) cma_c_null_ptr)
	cma___test_error (
		"\nERROR:\tNo current test in call to cma__failed.\n\n",
		(char *) cma_c_null_ptr,
		(char *) cma_c_null_ptr);

    cma__g_test_success = cma_c_false;
    
    printf ("FAILED,\tTest \"%s\":\t%s\n", cma__g_test_name, reason);
    fflush (stdout);
}



/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *      cma__result
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
 *      cma__g_test_name    -	current test name
 *      cma__g_test_success -	flag indicating test suceess
 *
 *  IMPLICIT OUTPUTS:
 *
 *      cma__g_test_name    -	current test name (set to cma_c_null_ptr)
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
cma__result ()
{
    if (cma__g_test_name == (char *) cma_c_null_ptr)
	cma___test_error (
		"\nERROR:\tNo current test in call to cma__result.\n\n",
		(char *) cma_c_null_ptr,
		(char *) cma_c_null_ptr);

    if (cma__g_test_success)
	printf ("PASSED,\tTest \"%s\":\tTest ran successfully.\n", 
		cma__g_test_name);

    fflush (stdout);
    cma__g_test_name = (char *) cma_c_null_ptr;
}




/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *      cma___test_error
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
cma___test_error(
	char	*format,
	char	*arg1,
	char	*arg2)
    {
    printf (format, arg1, arg2);
    fflush (stdout);
    abort();
}
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_TEST.C */
/*  *3    21-JUN-1990 08:21:43 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *2     7-JUN-1990 11:33:03 SCALES "Updates to CTS" */
/*  *1    17-MAY-1990 13:56:40 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_TEST.C */
