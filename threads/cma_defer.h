/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: cma_defer.h $Revision: 1.1.10.1 $ $Date: 1996/05/10 13:35:45 $
 */
/*
 * HISTORY
 * $Log: cma_defer.h,v $
 * Revision 1.1.10.1  1996/05/10  13:35:45  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:40 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:42 UTC  jss  /main/HPDCE02/1]
 *
 * 	CR10892 - fix copyright in file
 * 	[1994/06/09  13:27:46  devsrc]
 * 	 *
 *
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:17:09  jd]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  15:19  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/20  19:46:25  truitt]
 * 	 *
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:43:20  root  1.1.6.3]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:11:56  hopkins]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  18:05:31  root]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:33:00  hopkins]
 *
 * 	Insert copyright notices
 * 	[1992/12/07  19:37:54  alan]
 *
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:02:48  bolinger]
 *
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:12:52  bolinger]
 *
 * 	Loading drop DCE1_0_2FNL
 * 	[1993/05/04  22:33:53  mgm  1.1.1.11]
 *
 * Revision 1.1.6.2  1994/06/09  13:37:49  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:27:46  devsrc]
 * 
 * Revision 1.1.6.1  1994/04/01  20:16:07  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:17:09  jd]
 * 
 * Revision 1.1.2.3  1992/12/15  22:27:45  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:37:54  alan]
 * 
 * Revision 1.1.2.2  1992/09/03  14:36:34  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:02:48  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/08  23:13:06  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:12:52  bolinger]
 * 
 * Revision 1.1  1992/01/19  14:39:02  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *	%W%	(DEC OSF/1)	%G%
 */
/*
 *  Copyright (c) 1990, 1991 by
 *  Digital Equipment Corporation, Maynard Massachusetts.
 *  All rights reserved.
 *
 * This software is furnished under a licence and may be used and
 * copied only in accordance of the terms of such license and
 * with the inclusion of the above copyright notice. This
 * software or any other copies thereof may be provided or
 * otherwise made available to any other person. No title to and
 * ownership of the software is herby transferred.
 *
 * The information in this software is subject to change without
 * notice and should not be construed as a commitment by DIGITAL
 * Equipment Corporation.
 *
 * DIGITAL assumes no responability for the use or reliability
 * of its software on equipment which is not supplied by DIGITAL.
 */

/*
 * FACILITY:
 *
 *	CMA services
 *
 * ABSTRACT:
 *
 *	These routines allow actions which cannot be performed immediately
 *	to be deferred until a time when they can be performed immediately.
 *
 * AUTHOR:
 *
 *	Webb Scales
 *
 * CREATION DATE:
 *
 *	25 July 1990
 *
 * MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	03 August 1990
 *		Change the names of the semaphore deferral constants to fit
 *		new terms (wake_one and wake_all).
 *	002	Dave Butenhof	09 April 1991
 *		Use new type for "atomic bit" operation target
 *	003	Dave Butenhof	02 May 1991
 *		Remove defer_alert code.
 *	004	Paul Curtin	04 June 1991
 *		Add prototype for reinit routine.
 *	005	Dave Butenhof	21 November 1991
 *		Add prototype for cma__int_signal_int.
 *	006	Dave Butenhof	22 November 1991
 *		Fix 005.
 */


#ifndef CMA_DEFER
#define CMA_DEFER

/*
 * INCLUDE FILES
 */

#include <cma.h>
#include <cma_defs.h>
#include <cma_cond_defs.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * CONSTANTS AND MACROS
 */


/*
 * Define constants for deferral actions
 */
#define cma__c_defer_wake_one	1
#define cma__c_defer_wake_all	2
#define cma__c_defer_io_avail	3

/*
 * TYPEDEFS
 */


/*
 * GLOBAL DATA
 */

extern cma__t_atomic_bit cma__g_defers_processed; /* Zero if some defers exist*/


/*
 * INTERNAL INTERFACES
 */

/*
 * Defer unblocking of a thread: If the kernel is in its critical section,
 * any unblocking operation is deferred using this call.
 */
extern void cma__defer  (cma_t_integer,cma_t_address,cma_t_address);

/*
 * Initialize the defer mechanisms
 */
extern void cma__init_defer  (void);

/*
 * Internal deferred signal
 */
extern void cma__int_signal_int  (cma__t_int_cv *);

extern void cma__int_signal2_int (cma__t_int_cv *);

/*
 * Re-initialize defer queue after fork
 */
extern void cma__reinit_defer  (cma_t_integer);

/*
 * Undefer unblocking of a thread: opon leaving the critical section,
 * all deferred operations are executed.
 */
extern cma_t_integer cma__undefer  (void);

#ifdef __cplusplus
}
#endif

#endif
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_DEFER.H */
/*  *9    22-NOV-1991 11:55:32 BUTENHOF "Fix prototype " */
/*  *8    21-NOV-1991 13:54:25 BUTENHOF "Add prototype for cma__int_signal_int()" */
/*  *7    10-JUN-1991 19:52:26 SCALES "Convert to stream format for ULTRIX build" */
/*  *6    10-JUN-1991 19:20:32 BUTENHOF "Fix the sccs headers" */
/*  *5    10-JUN-1991 18:21:22 SCALES "Add sccs headers for Ultrix" */
/*  *4     5-JUN-1991 16:16:55 CURTIN "fork work" */
/*  *3     2-MAY-1991 13:58:02 BUTENHOF "Remove defer_alert" */
/*  *2    12-APR-1991 23:35:23 BUTENHOF "Change type of internal locks" */
/*  *1    12-DEC-1990 21:44:21 BUTENHOF "Defer events while kernel locked" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_DEFER.H */
