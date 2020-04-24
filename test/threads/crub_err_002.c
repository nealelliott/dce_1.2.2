/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crub_err_002.c,v $
 * Revision 1.1.7.2  1996/02/17  23:29:46  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:46:33  marty]
 *
 * Revision 1.1.7.1  1995/12/11  22:57:58  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:34:54  root]
 * 
 * Revision 1.1.4.1  1993/10/12  19:34:39  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:29:39  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:28:27  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:05:12  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:20:19  devrcs
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
 *	CMA Test Programs
 *
 *  FILENAME:
 *
 *	CRUB_ERR_002.C
 *
 *  TEST TOPICS:
 *
 *	ERR  -  Error Reporting
 *
 *  ABSTRACT:
 *
 *	This is a test of the assertions package.
 *	It checks that the definitions are made available by
 *	file cma_defs.h and CMA.H, and that both warning exceptions and 
 *	failure exceptions can be raised.
 *
 *  AUTHORS:
 *
 *	Bob Conti
 *
 *  CREATION DATE:
 *
 *	09 October 1989
 *
 *  MODIFICATION HISTORY:
 *  
 *	001	Paul Curtin	4 September 1990
 *		Adjust cts_test statement to reflect CMA/Pthreads naming
 *	002	Dave Butenhof	18 December 1990
 *		Remove redundant #include of cma_assert.h
 */

/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <cma_defs.h>
#include <dce/cts_test.h>


main ()
    {

    int j = 1;

    cts_test ("CRUB_ERR_002", "Test assertions");

    cma_init ();

    /* Examples of true assertion */
    cma__assert_warn (j==1, "J has unexpected count");

    /* 
     * More true assertions.
     * This set shows that the assertion is considered to be true
     * if the expression so long as the expression is not zero.
     */
    cma__assert_fail (j, "J has unexptected count");
    cma__assert_fail (j+2, "Attributes mutex not locked");
    cma__assert_fail (j+3, "Attributes mutex not locked");
    cma__assert_fail (j-2, "Attributes mutex not locked");
    cma__assert_fail (j-3, "Attributes mutex not locked");

    /* Examples of false warning assertions */
    /* Warnings simply report the error and continue execution */
    cma__assert_warn (0, "J has unexpected count");
    cma__assert_warn (0, "J has unexpected count");

    /* Examples of a failure assertion that is false */
    /* Failures cause immediate termination */
    cma__assert_fail (0, "Attributes mutex not locked");
    
    cts_failed ("Failure assertion did not terminate program");
    cts_result ();

    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRUB_ERR_002.C */
/*  *6    18-DEC-1990 23:41:17 BUTENHOF "Remove unnecessary #includes" */
/*  *5     5-SEP-1990 14:04:29 CURTIN "Converted to new CMA interfaces" */
/*  *4     3-JUL-1990 12:02:43 BUTENHOF "Change copyright" */
/*  *3    21-JUN-1990 08:25:36 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *2     7-JUN-1990 11:34:57 SCALES "Updates to CTS" */
/*  *1    17-MAY-1990 13:57:23 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRUB_ERR_002.C */
