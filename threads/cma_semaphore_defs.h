/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: cma_semaphore_defs.h $Revision: 1.1.8.1 $ $Date: 1996/05/10 13:41:13 $
 */
/*
 * HISTORY
 * $Log: cma_semaphore_defs.h,v $
 * Revision 1.1.8.1  1996/05/10  13:41:13  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:44 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:43 UTC  jss  /main/HPDCE02/1]
 *
 * 	CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:28  devsrc]
 * 	 *
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  17:55  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/21  14:07:50  truitt]
 * 	 *
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:44:04  root  1.1.6.3]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:13:35  hopkins]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  18:41:00  root]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:34:38  hopkins]
 *
 * 	Insert copyright notices
 * 	[1992/12/07  19:41:03  alan]
 *
 * 	Loading drop DCE1_0_2FNL
 *
 * 	[1993/05/04  22:35:08  mgm  1.1.1.11]
 *
 * Revision 1.1.4.1  1994/06/09  13:38:40  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:28  devsrc]
 * 
 * Revision 1.1.2.2  1992/12/15  22:30:11  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:41:03  alan]
 * 
 * Revision 1.1  1992/01/19  14:38:52  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
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
 *	Header file for semaphore structure definition.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof and Webb Scales
 *
 *  CREATION DATE:
 *
 *	4 December 1989
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	03 August 1990
 *		Simplify from counting semaphores to "blocking objects with
 *		state".  It's almost a binary semaphore, since it has state
 *		to support pending-wakeup, however rather than just P and V
 *		operations, it supports "wait", "wake one", and "wake all"
 *		operations.
 *	002	Dave Butenhof	08 August 1990
 *		Change "pending" bit to "nopending" (to better describe the
 *		function now that it'll be "set" with an interlocked clear
 *		and "cleared and tested" with an interlocked test-and-set).
 *	003	Dave Butenhof	09 April 1991
 *		Use new type for "atomic bit" operation target
 */
#ifndef CMA_SEMAPHORE_DEFS
#define CMA_SEMAPHORE_DEFS

/*
 *  INCLUDE FILES
 */
#include <cma.h>
#include <cma_queue.h>
#include <cma_defs.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * CONSTANTS AND MACROS
 */

#define cma__c_semaphore_timeout 1
#define cma__c_semaphore_event	 0
#define cma__c_select_timeout	 2

/*
 * TYPEDEFS
 */

typedef struct CMA__T_SEMAPHORE {
    cma__t_queue	queue;
    cma__t_atomic_bit	nopending;
    } cma__t_semaphore;

#ifdef __cplusplus
}
#endif

#endif
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_SEMAPHORE_DEFS.H */
/*  *5    10-JUN-1991 19:55:49 SCALES "Convert to stream format for ULTRIX build" */
/*  *4    10-JUN-1991 19:21:31 BUTENHOF "Fix the sccs headers" */
/*  *3    10-JUN-1991 18:23:20 SCALES "Add sccs headers for Ultrix" */
/*  *2    12-APR-1991 23:36:43 BUTENHOF "Change type of internal locks" */
/*  *1    12-DEC-1990 21:52:20 BUTENHOF "Binary semaphores" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_SEMAPHORE_DEFS.H */
