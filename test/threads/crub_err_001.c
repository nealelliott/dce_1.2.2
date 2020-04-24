/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crub_err_001.c,v $
 * Revision 1.1.7.2  1996/02/17  23:29:44  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:46:32  marty]
 *
 * Revision 1.1.7.1  1995/12/11  22:57:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:34:53  root]
 * 
 * Revision 1.1.4.1  1993/10/12  19:34:37  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:29:39  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:28:23  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:05:08  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:20:17  devrcs
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
 *	CRUB_ERR_001
 *
 *  TEST TOPICS:
 *
 *	ERR  -  Error Reporting
 *
 *  ABSTRACT:
 *
 *	This program checks that calling cma__error causes the
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
 *	001	Webb Scales	4 September 1990
 *		Changed (self) filename references.
 */

/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <cma_errors.h>
#include <cma_defs.h>

#include <dce/cts_test.h>

main ()
    {

    cts_test ("CRUB_ERR_001", "Check that cma__error works.");

    cma_init ();

    cma__error(cma_s_alerted);

    cts_failed ("Reporting error failed to terminate program");
    cts_result ();

    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRUB_ERR_001.C */
/*  *5     4-SEP-1990 17:15:06 SCALES "Convert to new CMA interfaces" */
/*  *4     3-JUL-1990 12:02:37 BUTENHOF "Change copyright" */
/*  *3    21-JUN-1990 08:25:23 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *2     7-JUN-1990 11:34:44 SCALES "Updates to CTS" */
/*  *1    17-MAY-1990 13:57:21 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRUB_ERR_001.C */
