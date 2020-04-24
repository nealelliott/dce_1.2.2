/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crvb_han_001.c,v $
 * Revision 1.1.33.2  1996/02/17  23:30:12  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:46:57  marty]
 *
 * Revision 1.1.33.1  1995/12/11  22:59:06  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:15  root]
 * 
 * Revision 1.1.31.1  1993/10/12  19:35:13  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:29:56  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:30:27  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:07:07  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:20:55  devrcs
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
 *	CRVB_HAN_001
 *
 *  TEST TOPICS:
 *
 *	HAN  -  HANDLES
 *
 *  ABSTRACT:
 *
 *	This tests that CMA handles do not change in size after
 *	first release.  This is required in order to support existing
 *	object modules and executables, that is, not require them
 *	to recompile.
 *
 *  AUTHORS:
 *
 *	Bob Conti
 *
 *  CREATION DATE:
 *
 *	18 January 1990
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
#include <dce/cts_test.h>

#define size_at_first_release 8

main ()
    {

    cts_test ("CRVB_HAN_001", "Test that the size of a handle remains fixed.");

    cma_init ();

    if (cma_c_handle_size != size_at_first_release) 
	cts_failed("Handle size constant has changed");

    if (sizeof(cma_t_handle) != size_at_first_release) 
	cts_failed("Size of handle has changed");

    if (sizeof(cma_t_attr) != size_at_first_release) 
	cts_failed("Size of attr handle has changed");

    if (sizeof(cma_t_mutex) != size_at_first_release) 
	cts_failed("Size of mutex handle has changed");

    if (sizeof(cma_t_cond) != size_at_first_release) 
	cts_failed("Size of cond handle has changed");

    cts_result ();

    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_HAN_001.C */
/*  *5     4-SEP-1990 17:15:10 SCALES "Convert to new CMA interfaces" */
/*  *4     3-JUL-1990 12:04:03 BUTENHOF "Change copyright" */
/*  *3    21-JUN-1990 08:29:29 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *2     7-JUN-1990 11:36:54 SCALES "Updates to CTS" */
/*  *1    17-MAY-1990 13:58:09 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_HAN_001.C */
