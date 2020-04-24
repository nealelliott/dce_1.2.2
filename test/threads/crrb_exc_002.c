/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crrb_exc_002.c,v $
 * Revision 1.1.8.2  1996/02/17  23:29:40  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:46:28  marty]
 *
 * Revision 1.1.8.1  1995/12/11  22:57:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:34:49  root]
 * 
 * Revision 1.1.6.1  1994/04/01  20:08:40  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:11:58  jd]
 * 
 * Revision 1.1.4.1  1993/10/12  19:34:32  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:29:35  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:28:08  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:04:51  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:22:34  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  Copyright (c) 1991 by
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
 *	CRRB_EXC_002
 *
 *  TEST TOPICS:
 *
 *	EXC  -  Exceptions
 *
 *  ABSTRACT:
 *
 *	Apparently, the BL8 changes to VMS exception unwind didn't
 *	work right with exception propagation through multiple
 *	frames. This program demonstrated the failure, and is now
 *	destined to become part of the test system.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	31 October 1991
 *
 *  MODIFICATION HISTORY:
 *
 */

/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <dce/cts_test.h>

static EXCEPTION anerror;
static long i = 0;

void boom	(long	k)
    {
    TRY {
	if( k == 0) RAISE(anerror);	/* if arg = 0, raise an error */
	}
    CATCH (anerror) {
	cts_comment ("boom: anerror caught.");
	RERAISE;
	}
    ENDTRY
    }

int
main	(int argc, char *argv[], char *envp[])
    {
    cts_test ("CRRB_EXC_002", "nested routine exception propagation");
    cma_init ();

    EXCEPTION_INIT (anerror);

    TRY {
	boom(i);
	}
    CATCH (anerror) {
	cts_comment ("main: anerror caught.");
	}
    ENDTRY

    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRRB_EXC_002.C */
/*  *1    31-OCT-1991 10:42:24 BUTENHOF "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRRB_EXC_002.C */
