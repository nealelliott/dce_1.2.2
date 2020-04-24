/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crrb_att_001.c,v $
 * Revision 1.1.8.2  1996/02/17  23:29:37  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:46:24  marty]
 *
 * Revision 1.1.8.1  1995/12/11  22:57:39  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:34:47  root]
 * 
 * Revision 1.1.6.1  1994/04/01  20:08:35  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:11:45  jd]
 * 
 * Revision 1.1.4.1  1993/10/12  19:34:26  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:29:33  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:27:58  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:04:38  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:20:09  devrcs
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
 *	RR_ATT_001
 *
 *  TEST TOPICS:
 *
 *	ATT  -  Attributes Objects
 *
 *  ABSTRACT:
 *
 *	Test for problem reported by Harold Siegel (Ada RTL); the
 *	embedded synch. objects in TCB were created using 
 *	cma__g_def_attr, but flushing of the attributes object TCB
 *	cache during deletion assumed they were created using the
 *	same attributes object as the TCB; this resulted in freeing
 *	the synch objects without proper locking (it was detected
 *	by assertions in cma__free_mutex_nolock).  Now embedded
 *	synch objects are created using the TCB's attributes object
 *	to avoid the problem.
 *	
 *	This test verifies that the assertions are no longer
 *	triggered.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	05 April 1990
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	15 June 1990
 *		Fix type of argument to cma_thread_join to get rid of error
 *		in cautious C compilers.
 *	002	Dave Butenhof	4 September 1990
 *		Change to new CMA interfaces.
 */

/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <dce/cts_test.h>

cma_t_address
thread(cma_t_address	arg)
    {
    return cma_c_null_ptr;
    }

main ()
    {
    cma_t_attr		att;
    cma_t_thread	th;
    cma_t_exit_status	status;
    cma_t_address	result;


    cts_test ("CRRB_ATT_001", "Check locking during deletion of attr. objects");

    cma_init ();

    /*
     * Create a new attributes object with a small stack size.  An
     * optimization prevents flushing of the cached TCB list if the
     * attributes object being freed has the default stack and guard size
     * attributes (since the object can still be reused when and if the
     * attributes object is reused).  To provoke the bug, we need to ensure
     * that the TCB cache is flushed.
     */
    cma_attr_create (&att, &cma_c_null);
    cma_attr_set_stacksize (&att, 200);

    /*
     * Create a thread using the attributes object, and wait for it to
     * complete.  Detach the handle to free the TCB onto att's cache list.
     */
    cma_thread_create (&th, &att, thread, cma_c_null_ptr);
    cma_thread_join (&th, &status, &result);
    cma_thread_detach (&th);

    /*
     * Now, delete the attributes object to flush the cache list.  If the bug
     * is present (and this is a debug version of the CMA reference
     * implementation), several WARNING assertion messages should occur.
     * Note that this condition cannot be tested for by the program; manual
     * review is required.
     */
    cma_attr_delete (&att);
    cma_attr_create (&att, &cma_c_null);

    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRRB_ATT_001.C */
/*  *7     5-SEP-1990 09:39:03 BUTENHOF "Convert to new CMA interfaces" */
/*  *6     3-JUL-1990 12:02:26 BUTENHOF "Change copyright" */
/*  *5    27-JUN-1990 11:05:45 BUTENHOF "Conditionalize prototypes for VAX ULTRIX" */
/*  *4    21-JUN-1990 08:24:56 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *3    15-JUN-1990 09:18:46 BUTENHOF "Fix type of argument to cma_thread_join" */
/*  *2     7-JUN-1990 11:34:37 SCALES "Updates to CTS" */
/*  *1    17-MAY-1990 13:57:18 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRRB_ATT_001.C */
