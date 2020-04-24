/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: cma_list.h $Revision: 1.1.8.1 $ $Date: 1996/05/10 13:38:40 $
 */
/*
 * HISTORY
 * $Log: cma_list.h,v $
 * Revision 1.1.8.1  1996/05/10  13:38:40  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:42 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:43 UTC  jss  /main/HPDCE02/1]
 *
 * 	CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:07  devsrc]
 * 	 *
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  16:10  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/21  13:09:27  truitt]
 * 	 *
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:43:42  root  1.1.6.3]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:12:49  hopkins]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  18:25:17  root]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:33:53  hopkins]
 *
 * 	Insert copyright notices
 * 	[1992/12/07  19:39:32  alan]
 *
 * 	Loading drop DCE1_0_2FNL
 *
 * 	[1993/05/04  22:34:32  mgm  1.1.1.11]
 *
 * Revision 1.1.4.1  1994/06/09  13:38:14  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:07  devsrc]
 * 
 * Revision 1.1.2.2  1992/12/15  22:28:49  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:39:32  alan]
 * 
 * Revision 1.1  1992/01/19  14:39:11  devrcs
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
 *	Header file for generic list functions operating on singly linked
 *	null-terminated lists.  Items may not be REMOVED from the list!  The
 *	intent is that the list can be traversed (for read-only operations)
 *	without locking, since insertion is "safe" (though not truely
 *	atomic).  THIS ASSUMES THAT THE HARDWARE MAKES WRITES VISIBLE TO READS
 *	IN THE ORDER IN WHICH THEY OCCURRED!  WITHOUT SUCH READ/WRITE
 *	ORDERING, IT MAY BE NECESSARY TO INSERT "BARRIERS" TO PRODUCE THE
 *	REQUIRED VISIBILITY!
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	26 January 1990
 *
 *  MODIFICATION HISTORY:
 *
 */

#ifndef CMA_LIST
#define CMA_LIST

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

#define cma__c_null_list	((cma__t_list *)cma_c_null_ptr)

/*
 * Test whether a list is empty.  Return cma_c_true if so, else
 * cma_c_false.
 */
#define cma__list_empty(head)	((head)->link == cma__c_null_list)

/*
 * Initialize a queue header to empty.
 */
#define cma__list_init(head)	(void)((head)->link = cma__c_null_list)

/*
 * Insert an element in a list following the specified item (or at the
 * beginning of the list if "list" is the list head).  NOTE: insertion
 * operations should be interlocked by the caller!
 */
#define cma__list_insert(element,list)    (void)(	\
    (element)->link		= (list)->link,		\
    (list)->link		= (element))

/*
 * Return the next item in a list (or the first, if the address is of the
 * list header)
 */
#define cma__list_next(element)    ((element)->link)

/*
 * TYPEDEFS
 */

typedef struct CMA__T_LIST {
    struct CMA__T_LIST	*link;		/* Forward link */
    } cma__t_list;

/*
 *  GLOBAL DATA
 */

/*
 * INTERNAL INTERFACES
 */

#ifdef __cplusplus
}
#endif

#endif
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_LIST.H */
/*  *4    10-JUN-1991 19:54:14 SCALES "Convert to stream format for ULTRIX build" */
/*  *3    10-JUN-1991 19:21:06 BUTENHOF "Fix the sccs headers" */
/*  *2    10-JUN-1991 18:22:20 SCALES "Add sccs headers for Ultrix" */
/*  *1    12-DEC-1990 21:47:12 BUTENHOF "Atomic lists" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_LIST.H */
