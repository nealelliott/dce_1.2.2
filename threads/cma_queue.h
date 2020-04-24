/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems
 * Burlington, MA, USA
 * All Rights Reserved
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: cma_queue.h $Revision: 1.1.10.1 $ $Date: 1996/05/10 13:40:19 $
 */
/*
 * HISTORY
 * $Log: cma_queue.h,v $
 * Revision 1.1.10.1  1996/05/10  13:40:19  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:44 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:43 UTC  jss  /main/HPDCE02/1]
 *
 * 	CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:23  devsrc]
 * 	 *
 *
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:18:24  jd]
 *
 * 	Ajay's performance enhancements
 * 	[1994/05/03  18:20 UTC  jss  /main/HPDCE01/2]
 *
 * 	Define macro for cma__queue_dequeue_fast()
 * 	[1994/04/29  19:08 UTC  ajayb  /main/HPDCE01/ajayb_threads_perf/1]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  17:01  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/21  13:45:12  truitt]
 * 	 *
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:43:59  root  1.1.6.3]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:13:17  hopkins]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  18:35:03  root]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:34:19  hopkins]
 *
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  17:26:49  hinman]
 *
 * 	[hinman] - Check in merged SNI version
 * 	[1993/01/11  16:20:41  hinman]
 *
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:50:16  sekhar]
 *
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:05:38  bolinger]
 *
 * 	Merge up to BL10+ internal release from DEC.
 * 	[1992/08/11  21:00:10  bolinger]
 *
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:43:11  bolinger]
 *
 * 	Loading drop DCE1_0_2FNL
 * 	[1993/05/04  22:34:59  mgm  1.1.1.11]
 *
 * 	PTC - DCOSX specific changes.
 * 	[92/10/26  12:40:34  bein  9.5.1.7]
 *
 * 	PTC - added DCOSX ifdef ala SINIX since our compiler complains too.
 * 	[92/10/26  10:32:03  bein  9.5.3.2]
 *
 * 	Acceptance of OSF rev 1.1.2.3
 * 	[92/09/30  12:41:01  root  9.5.1.6]
 *
 * 	Auto checkin of OSF rev 1.1.2.3
 * 	[92/09/30  10:34:32  hinman  9.12.1.2]
 *
 * Revision 1.1.5.2  1994/06/09  13:38:33  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:23  devsrc]
 * 
 * Revision 1.1.5.1  1994/04/01  20:17:01  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:18:24  jd]
 * 
 * Revision 1.1.2.5  1993/02/01  22:18:32  hinman
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  17:26:49  hinman]
 * 
 * 	[hinman] - Check in merged SNI version
 * 	[1993/01/11  16:20:41  hinman]
 * 
 * Revision 9.5.1.7  92/10/26  12:40:34  bein
 * 	PTC - DCOSX specific changes.
 * 
 * Revision 9.5.3.2  92/10/26  10:32:03  bein
 * 	PTC - added DCOSX ifdef ala SINIX since our compiler complains too.
 * 
 * Revision 9.5.1.6  92/09/30  12:41:01  root
 * 	Acceptance of OSF rev 1.1.2.3
 * 
 * Revision 9.12.1.2  92/09/30  10:34:32  hinman
 * 	Auto checkin of OSF rev 1.1.2.3
 * 
 * Revision 1.1.2.3  1992/09/29  20:15:18  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:50:16  sekhar]
 * 
 * Revision 1.1.2.2  1992/09/03  14:39:27  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:05:38  bolinger]
 * 
 * Revision 1.1.2.3  1992/08/14  20:22:08  bolinger
 * 	Merge up to BL10+ internal release from DEC.
 * 	[1992/08/11  21:00:10  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/08  23:43:27  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:43:11  bolinger]
 * 
 * Revision 1.1  1992/01/19  14:39:19  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *	%W%	(ULTRIX/OSF)	%G%
 */
/*
 *  Copyright (c) 1989, 1991 by
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
 *	Header file for generic queuing functions operating on circular
 *	double-linked lists.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	24 August 1989
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	18 October 1989
 *		Change queue header operations to operate on "cma__t_queue *"
 *		rather than generic "void *".  Change "void *" to
 *		"cma_t_address" (which is same if compiler supports it).
 *		Convert some routines to macros for compilers which don't
 *		support #inline pragma.
 *	002	Dave Butenhof	19 October 1989
 *		All queue operations should accept and return "cma__t_queue"
 *		type objects.  Require caller to use explicit casts where
 *		necessary, to support C compilers which don't correctly
 *		handle ANSI C's "void *" data type.
 *	003	Webb Scales	20 October 1989
 *		Move cma__queue_dequeue into cma_queue.c:  on MIPS, when the
 *		routine definition is included multiple time, we get competing
 *		definitions at link time.
 *	004	Dave Butenhof	23 October 1989
 *		Make cma__queue_dequeue prototype "extern" instead of
 *		"static", since definition was moved to .c...
 *	005	Webb Scales	15 November 1989
 *		Added assertions to queue operations
 *	006	Webb Scales	21 June 1990
 *		Added macro to insert after an element or at the head of queue.
 *	007	Dave Butenhof	11 June 1991
 *		Add trace array capability for queue operations; too
 *		frequent for cma__trace, but it's sometimes useful to be able
 *		to follow the sequences.
 *	008	Webb Scales	 8 May 1992
 *		Added queue element consistency checks (to augment existing
 *		queue header consistency checks); improved bugcheck messages;
 *		added a new macro which zero's an element's queue links.
 */


#ifndef CMA_QUEUE
#define CMA_QUEUE

/*
 *  INCLUDE FILES
 */

#include <cma.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * CONSTANTS AND MACROS
 */

/*
 * Test whether a queue is empty.  Return cma_c_true if so, else
 * cma_c_false.
 */
#define cma__queue_empty(head)	(			\
    cma__assert_fail (					\
	    (head)->blink->flink == (head),		\
	    "cma__queue_empty: Q corruption"), \
    cma__assert_fail (					\
	    (head)->flink->blink == (head),		\
	    "cma__queue_empty: Q corruption"), \
    (head)->flink == (head))

/*
 * Initialize a queue header to empty.  (Note that the comma operator is used
 * merely to avoid the necessity for a block, not because a return value is
 * actually useful).
 */
#define cma__queue_init(head)	((head)->flink = (head), (head)->blink = (head))

/*
 * Insert an element in a queue preceding the specified item (or at end of
 * queue if "queue" is the queue head).
 */
#define cma__queue_insert(element,q_ptr)    (		\
    cma__assert_fail (					\
	    (element)->flink == 0,			\
	    "cma__Queue_insert: Q corruption"), \
    cma__assert_fail (					\
	    (element)->blink == 0,			\
	    "cma_Queue_insert: Q corruption"), \
    cma__assert_fail (					\
	    (q_ptr)->blink->flink == (q_ptr),		\
	    "cma__Queue_insert: Q corruption"), \
    cma__assert_fail (					\
	    (q_ptr)->flink->blink == (q_ptr),		\
	    "cma__Queue_insert: Q corruption"), \
    (element)->blink		= (q_ptr)->blink,	\
    (element)->flink		= (q_ptr),		\
    (q_ptr)->blink->flink	= (element),		\
    (q_ptr)->blink		= (element))
/*
 * Insert an element in a queue following the specified item (or at head of
 * queue if "q_ptr" is the queue head).
 */
#define cma__queue_insert_after(element,q_ptr)    (		\
    cma__assert_fail (					\
	    (element)->flink == 0,			\
	    "cma__Queue_insert_after: Q corruption"), \
    cma__assert_fail (					\
	    (element)->blink == 0,			\
	    "cma__Queue_insert_after: Q corruption"), \
    cma__assert_fail (						\
	    (q_ptr)->blink->flink == (q_ptr),			\
	    "cma__Queue_insert_after: Q corruption"), \
    cma__assert_fail (						\
	    (q_ptr)->flink->blink == (q_ptr),			\
	    "cma__Queue_insert_after: Q corruption"), \
    (element)->flink		= (q_ptr)->flink,		\
    (element)->blink		= (q_ptr),			\
    (q_ptr)->flink->blink	= (element),			\
    (q_ptr)->flink		= (element))

/*
 * Return the next item in a queue (or the first, if the address is of the
 * queue header)
 */
#define cma__queue_next(element)    (			\
    cma__assert_fail (					\
	    (element)->flink != 0,			\
	    "cma__Queue_next: Q corruption"), \
    cma__assert_fail (					\
	    (element)->blink != 0,			\
	    "cma__Queue_next: Q corruption"), \
    cma__assert_fail (					\
	    (element)->blink->flink == (element),	\
	    "cma__Queue_next: Q corruption"), \
    cma__assert_fail (					\
	    (element)->flink->blink == (element),	\
	    "cma__Queue_next: Q corruption"), \
    (element)->flink)

/*
 * Return the previous item in a queue (or the last, if the address is of the
 * queue header)
 */
#define cma__queue_previous(element)    (		\
    cma__assert_fail (					\
	    (element)->flink != 0,			\
	    "cma__Queue_previous: Q corruption"), \
    cma__assert_fail (					\
	    (element)->blink != 0,			\
	    "cma__Queue_previous: Q corruption"), \
    cma__assert_fail (					\
	    (element)->blink->flink == (element),	\
	    "cma__Queue_previous: Q corruption"), \
    cma__assert_fail (					\
	    (element)->flink->blink == (element),	\
	    "cma__Queue_previous: Q corruption"), \
    (element)->blink)

/*
 * Remove the specified item from a queue.  Return a pointer to the element.
 */
#define cma__queue_remove(element) (			\
    cma__assert_fail (					\
	    (element)->flink != 0,			\
	    "cma__Queue_remove: Q corruption"), \
    cma__assert_fail (					\
	    (element)->blink != 0,			\
	    "cma__Queue_remove: Q corruption"), \
    cma__assert_fail (					\
	    (element)->blink->flink == (element),	\
	    "cma__Queue_remove: Q corruption"), \
    cma__assert_fail (					\
	    (element)->flink->blink == (element),	\
	    "cma__Queue_remove: Q corruption"), \
    (element)->blink->flink = (element)->flink,		\
    (element)->flink->blink = (element)->blink,		\
    cma__queue_zero(element),				\
    (element))

/*
 * Initialize a queue element to "not-on-queue".  (This is defined so as to
 * be able to be used inside of other macros.)
 */
#ifndef NDEBUG
# define cma__queue_zero(element)    ((element)->flink = (element)->blink = 0)
#else
# define cma__queue_zero(element)    0
#endif

#ifndef HP_CMA_QDEQUEUE_OPT
#define cma__queue_dequeue_fast(head,first) ((head)==(first)?(cma__t_queue *) cma_c_null_ptr: \
                                        (cma__queue_remove((first))))
#endif

/*
 * TYPEDEFS
 */

typedef struct CMA__T_QUEUE {
    struct CMA__T_QUEUE	*flink;		/* Forward link */
    struct CMA__T_QUEUE	*blink;		/* Backward link */
    } cma__t_queue;

/*
 *  GLOBAL DATA
 */

/*
 * INTERNAL INTERFACES
 */

/*
 * Remove the first element from a queue.  Return a pointer to the element.
 */
extern cma__t_queue * cma__queue_dequeue (cma__t_queue	*);

#ifdef __cplusplus
}
#endif

#endif
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_QUEUE.H */
/*  *6    15-MAY-1992 15:03:53 SCALES "Add additional consistency checks" */
/*  *5    11-JUN-1991 17:17:17 BUTENHOF "Add tracing" */
/*  *4    10-JUN-1991 19:55:11 SCALES "Convert to stream format for ULTRIX build" */
/*  *3    10-JUN-1991 19:21:20 BUTENHOF "Fix the sccs headers" */
/*  *2    10-JUN-1991 18:23:03 SCALES "Add sccs headers for Ultrix" */
/*  *1    12-DEC-1990 21:49:03 BUTENHOF "Atomic queues" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_QUEUE.H */
