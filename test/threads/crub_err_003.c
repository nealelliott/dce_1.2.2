/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crub_err_003.c,v $
 * Revision 1.1.6.2  1996/02/17  23:29:47  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:46:34  marty]
 *
 * Revision 1.1.6.1  1995/12/11  22:58:01  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:34:55  root]
 * 
 * Revision 1.1.4.1  1993/10/12  19:34:41  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:29:40  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:28:32  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:05:16  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:20:20  devrcs
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
 *	CRUB_ERR_003
 *
 *  TEST TOPICS:
 *
 *	ERR  -  Error Reporting
 *
 *  ABSTRACT:
 *
 *	This program checks that calling cma__bugcheck causes the
 *	program to terminate.
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
 *		Adjust cts_test statement to reflect CMA/Pthreads naming.
 *	002	Webb Scales	8 May 1991
 *		Add argument to bugcheck call.
 */

/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <cma_defs.h>
#include <dce/cts_test.h>
#include <cma_errors.h>


main ()
    {

    cts_test ("CRUB_ERR_003", "Check that cma__bugcheck works");

    cma_init ();

    cma__bugcheck("PASSED, Test \"CRUB_ERR_003\":	Test ran successfully.");

    cts_failed ("Bugcheck failed to terminate program");
    cts_result ();

    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRUB_ERR_003.C */
/*  *6     8-MAY-1991 17:45:58 SCALES "Add argument to bugcheck call" */
/*  *5     5-SEP-1990 14:06:47 CURTIN "Converted to new CMA interfaces" */
/*  *4     3-JUL-1990 12:02:48 BUTENHOF "Change copyright" */
/*  *3    21-JUN-1990 08:25:49 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *2     7-JUN-1990 11:35:05 SCALES "Updates to CTS" */
/*  *1    17-MAY-1990 13:57:27 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRUB_ERR_003.C */
