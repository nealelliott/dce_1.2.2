/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cma_queue.c,v $
 * Revision 1.1.6.2  1996/02/17  23:34:13  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:50:22  marty]
 *
 * Revision 1.1.6.1  1995/12/07  22:04:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:06:40  root]
 * 
 * Revision 1.1.4.2  1994/06/09  13:38:31  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:22  devsrc]
 * 
 * Revision 1.1.4.1  1994/04/01  20:16:59  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:18:22  jd]
 * 
 * Revision 1.1.2.3  1992/12/15  22:29:40  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:40:35  alan]
 * 
 * Revision 1.1.2.2  1992/09/03  14:39:21  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:05:33  bolinger]
 * 
 * Revision 1.1.2.3  1992/08/14  20:22:05  bolinger
 * 	Merge up to BL10+ internal release from DEC.
 * 	[1992/08/11  20:59:46  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/08  23:42:35  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:42:20  bolinger]
 * 
 * Revision 1.1  1992/01/19  14:39:18  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *  Copyright (c) 1989, 1990 by
 *  Digital Equipment Corporation, Maynard Massachusetts.
 *  All rights reserved.
 *
 *  This software is furnished under a license and may be used and  copied
 *  only  in  accordance  with  the  terms  of  such  license and with the
 *  inclusion of the above copyright notice.  This software or  any  other
 *  copies  thereof may not be provided or otherwise made available to any
 *  other person.  No title to and ownership of  the  software  is  hereby
 *  transferred.
 *
 *  The information in this software is subject to change  without  notice
 *  and  should  not  be  construed  as  a commitment by DIGITAL Equipment
 *  Corporation.
 *
 *  DIGITAL assumes no responsibility for the use or  reliability  of  its
 *  software on equipment which is not supplied by DIGITAL.
 */

/*
 *  FACILITY:
 *
 *	CMA services
 *
 *  ABSTRACT:
 *
 *	CMA queuing primitives
 *
 *  AUTHORS:
 *
 *	Webb Scales
 *
 *  CREATION DATE:
 *
 *	20 October 1989
 *
 *  MODIFICATION HISTORY:
 *
 *  	001	Webb Scales	14 November 1989
 *	   	Added assertions to cma__queue_dequeue
 *	002	Dave Butenhof & Webb Scales	9 April 1990
 *		Make cma__queue_dequeue return cma_c_null_ptr if the queue is
 *		empty.
 *	003	Webb Scales	 8 May 1992
 *		Added queue element consistency checks (to augment existing
 *		queue header consistency checks); improved bugcheck messages;
 *		rearranged local variable initialization to make assertions 
 *		work properly.
 */

/*
 *  INCLUDE FILES
 */

#include <cma.h>
#include <cma_defs.h>
#include <cma_queue.h>

/*
 * LOCAL DATA
 */

/*
 * LOCAL FUNCTIONS
 */

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Remove the first element from a queue.  Return a pointer to the element.
 *
 *  FORMAL PARAMETERS:
 *
 *	head  -  a pointer to the head of the queue
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	Returns a pointer to the object which was removed from the queue.
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern cma__t_queue *
cma__queue_dequeue(
	cma__t_queue	*head)
    {
    cma__t_queue    *element;


    cma__assert_fail (
	    head != (cma__t_queue *)cma_c_null_ptr, 
	    "cma__queue_dequeue: attempting dequeue using a null queue head");

    cma__assert_fail (
	    ((head->flink != (cma__t_queue *)cma_c_null_ptr)
	    && (head->blink != (cma__t_queue *)cma_c_null_ptr)),
	    "cma__queue_dequeue: dequeue on uninitialized queue head");

    cma__assert_fail (
	    head->blink->flink == head,
	    "cma__queue_dequeue: queue corruption -- head->blink->flink != head");

    cma__assert_fail (
	    head->flink->blink == head,
	    "cma__queue_dequeue: queue corruption -- head->flink->blink != head");

    /*
     * If the queue is empty, return the null pointer; otherwise remove and
     * return the first element.
     */
    element = head->flink;
    if (head == element)
	return (cma__t_queue *)cma_c_null_ptr;
    else
	return cma__queue_remove (element);

    }

/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_QUEUE.C */
/*  *3    15-MAY-1992 15:03:37 SCALES "Add additional consistency checks" */
/*  *2    10-JUN-1991 18:23:00 SCALES "Add sccs headers for Ultrix" */
/*  *1    12-DEC-1990 21:49:01 BUTENHOF "Atomic queues" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_QUEUE.C */
