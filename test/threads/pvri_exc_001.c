/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: pvri_exc_001.c,v $
 * Revision 1.1.56.2  1996/02/17  23:32:24  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:48:45  marty]
 *
 * Revision 1.1.56.1  1995/12/11  23:05:06  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:58  root]
 * 
 * Revision 1.1.54.1  1993/10/12  19:38:34  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:31:31  rhw]
 * 
 * Revision 1.1.4.2  1992/12/30  16:56:39  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:14:39  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/11  17:09:04  bolinger
 * 	Fix OT defect 2161:  provide/correct missing/bad cts_test() banners.
 * 	[1992/05/11  13:47:23  bolinger]
 * 
 * Revision 1.1  1992/01/19  03:19:50  devrcs
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
 *	PTHREAD Test Programs
 *
 *  FILENAME:
 *
 *	PVRI_EXC_001
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
 *  001	    Paul Curtin	    31 May 1990
 *	    Converted test to use pthread interface.
 *  002	    Paul Curtin	    5 September 1990
 *	    Fixed test name.
 */

/*
 *  INCLUDE FILES
 */

#include <pthread.h>
#include <dce/cts_test.h>

dummy () {};

main ()
    {

    cts_test ("PVRI_EXC_001", "Test that control Y debug is not caught by CMA");

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
