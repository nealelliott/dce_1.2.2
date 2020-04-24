/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cvri_exc_001.c,v $
 * Revision 1.1.59.2  1996/02/17  23:31:16  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:48  marty]
 *
 * Revision 1.1.59.1  1995/12/11  23:01:46  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:05  root]
 * 
 * Revision 1.1.57.1  1993/10/12  19:36:52  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:41  rhw]
 * 
 * Revision 1.1.4.2  1992/12/30  16:52:14  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:10:34  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/11  17:07:59  bolinger
 * 	Fix OT defect 2161:  provide/correct missing/bad cts_test() banners.
 * 	[1992/05/11  13:46:09  bolinger]
 * 
 * Revision 1.1  1992/01/19  03:21:46  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  Copyright (c) 1990 by
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
 *	CVRI_EXC_001
 *
 *  TEST TOPICS:
 *
 *	EXC  -  Exceptions
 *
 *  ABSTRACT:
 *
 *	This tests that the SS$_DEBUG condition that
 *	results from typing control Y DEBUG is not caught
 *	by CMA exception handlers.
 *	
 *	This test must be executed interactively.
 *
 *  AUTHORS:
 *
 *	Bob Conti
 *
 *  CREATION DATE:
 *
 *	06 April 1990
 *
 *  MODIFICATION HISTORY:
 *
 *  001	    Paul Curtin	    4 September 1990
 *	    Adjust test name in cts_test statement.
 */

/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <dce/cts_test.h>

dummy () {};

main ()
    {

    cts_test ("CVRI_EXC_001", "Test that control Y debug is not caught by CMA");

    cma_init ();

    cts_comment ("FIX-ME: this test requires you to manually type ^Y DEBUG");
    cts_comment (" and you must link/nodebug");

    cts_comment ("Type ^Y DEBUG. When the debugger comes up, type GO");
    TRY {

	TRY {

	    {int i; for (i = 0; i <=1000000; i++) dummy(); }
	  
	}
	CATCH_ALL {
	    cts_failed ("****Error - caught an unexpected exception");
	}
	ENDTRY
    }
    FINALLY {
	cts_comment ("Got to finally clause");
    }
    ENDTRY;

    cts_result ();

    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CVRI_EXC_001.C */
/*  *5     5-SEP-1990 13:54:05 CURTIN "Converted to use new CMA interface" */
/*  *4     3-JUL-1990 12:08:06 BUTENHOF "Change copyright" */
/*  *3    21-JUN-1990 08:37:43 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *2     7-JUN-1990 11:41:44 SCALES "Updates to CTS" */
/*  *1    17-MAY-1990 14:00:09 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CVRI_EXC_001.C */
