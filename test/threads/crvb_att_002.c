/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crvb_att_002.c,v $
 * Revision 1.1.35.2  1996/02/17  23:30:04  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:46:50  marty]
 *
 * Revision 1.1.35.1  1995/12/11  22:58:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:09  root]
 * 
 * Revision 1.1.33.1  1993/10/12  19:34:57  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:29:49  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:29:48  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:06:32  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:20:44  devrcs
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
 *	RV_ATT_002
 *
 *  TEST TOPICS:
 *
 *	ATT  -  Attr Objects
 *
 *  ABSTRACT:
 *
 *	Test deferred delete for attr objects which have
 *	"live" objects attached to them.  This'll be tough to really
 *	validate automatically; it's really intended to go along with
 *	manual human observation through the debugger.  However, in
 *	an automatic test system, it will PROBABLY accvio/seg. fault
 *	if things aren't being done right.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	25 October 1989
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	4 September 1990
 *		Change to new CMA interfaces.
 */

/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <dce/cts_test.h>


main ()
    {
    cma_t_attr	att1, att2;
    cma_t_mutex	mutex1, mutex2;
    cma_t_cond	cv1, cv2;

    cts_test ("CRVB_ATT_002", "Validate cache sequencing and reclamation");

    cma_init ();

    /*
     * Create an attr object and a "child" attr object which
     * refers to the first.
     */
    cma_attr_create (&att1, &cma_c_null);
    cma_attr_create (&att2, &att1);

    /*
     * Create some objects off the second one
     */
    cma_mutex_create (&mutex1, &att2);
    cma_mutex_create (&mutex2, &att2);
    cma_cond_create (&cv1, &att2);
    cma_cond_create (&cv2, &att2);

    /*
     * Delete some to put them on the cache
     */
    cma_mutex_delete (&mutex1);
    cma_cond_delete (&cv1);

    /*
     * Now, delete the second attr object.  This should flush the cache
     * and free the attr object to it's parent's cache
     */
    cma_attr_delete (&att2);

    /*
     * Now, delete the first attr object and create a new one on the
     * default attr object..  This will reclaim the deleted one and
     * flush ITS cache, and attempt to destroy the second attr object:
     * which should result in setting delete-pending, since it still has live
     * "children".
     */
    cma_attr_delete (&att1);
    cma_attr_create (&att1, &cma_c_null);

    /*
     * Now, delete the other child objects... they should be destroyed since
     * delete-pending is set on their parent.  Deletion of the last object
     * should also result in the parent attr object being destroyed.
     */
    cma_mutex_delete (&mutex2);
    cma_cond_delete (&cv2);

    cts_result ();

    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_ATT_002.C */
/*  *5     5-SEP-1990 09:40:18 BUTENHOF "Convert to new CMA interfaces" */
/*  *4     3-JUL-1990 12:03:30 BUTENHOF "Change copyright" */
/*  *3    21-JUN-1990 08:27:30 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *2     7-JUN-1990 11:35:56 SCALES "Updates to CTS" */
/*  *1    17-MAY-1990 13:57:47 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_ATT_002.C */
