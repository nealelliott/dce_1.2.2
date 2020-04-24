/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: cma_tcb.h $Revision: 1.1.8.1 $ $Date: 1996/05/10 13:42:36 $
 */
/*
 * HISTORY
 * $Log: cma_tcb.h,v $
 * Revision 1.1.8.1  1996/05/10  13:42:36  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:45 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:44 UTC  jss  /main/HPDCE02/1]
 *
 * 	CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:38  devsrc]
 * 	 *
 *
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:18:59  jd]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  19:05  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/21  17:59:14  truitt]
 * 	 *
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:44:26  root  1.1.6.3]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:14:14  hopkins]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  18:53:29  root]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:35:19  hopkins]
 *
 * 	Insert copyright notices
 * 	[1992/12/07  19:42:15  alan]
 *
 * 	Loading drop DCE1_0_2FNL
 *
 * 	[1993/05/04  22:35:39  mgm  1.1.1.11]
 *
 * Revision 1.1.4.2  1994/06/09  13:38:54  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:38  devsrc]
 * 
 * Revision 1.1.4.1  1994/04/01  20:17:26  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:18:59  jd]
 * 
 * Revision 1.1.2.2  1992/12/15  22:31:24  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:42:15  alan]
 * 
 * Revision 1.1  1992/01/19  14:39:27  devrcs
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
 *	Concert Multithread Architecture (CMA) services
 *
 *  ABSTRACT:
 *
 *	TCB-related definitions.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	24 July 1989
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	14 August 1989
 *		Extract context list into separate typedef for casts.
 *	002	Dave Butenhof	15 August 1989
 *		Add type field to object structure.
 *	003	Dave Butenhof	21 August 1989
 *		Add pointer to first stack descriptor for thread.
 *	004	Dave Butenhof	24 August 1989
 *		Modify TCB to use queues instead of lists
 *	005	Webb Scales	26 August 1989
 *		Change from jmp_buf context to private context type
 *	006	Dave Butenhof	29 August 1989
 *		Remove #include of cma_stack.h, since we don't need it any
 *		more.  Define context buffer locally to minimize dependencies
 *		and risk of circularities.
 *	007	Dave Butenhof	31 August 1989
 *		Rename cv field to term_cv.
 *	008	Dave Butenhof	14 September 1989
 *		Add alert state field to TCB.
 *	009	Hans Oser	22 Septemper 1989
 *		Add pc and psl field to TCB.
 *	010	Webb Scales	26 September 1989
 *		Change position of FP field in context buffer.
 *	011	Dave Butenhof	29 September 1989
 *		Add initial_test field to TCB (TCB creation will copy from
 *		attributes object so TCB is independent of changes to attr.
 *		obj between creation and execution).
 *	012	Dave Butenhof	11 October 1989
 *		Convert to use internal mutex operations.
 *	013	Dave Butenhof	16 October 1989
 *		Change TCB fields for Hans...
 *	014	Hans Oser	20 October 1989
 *		Add TCB fields for ultrix "select"
 *	015	Webb Scales	23 October 1989
 *		Changed "..._PSL" to "..._ps"
 *	016	Dave Butenhof	27 October 1989
 *		Add TCB field so alert can find condition variable to
 *		broadcast on (simplest way to break through wait).
 *	017	Dave Butenhof	1 November 1989
 *		Add backpointer for mutex.
 *	018	Dave Butenhof	16 November 1989
 *		Define tcb_time structure here to avoid circular
 *		dependencies.
 *	019	Webb Scales	18 November 1989
 *		Make thread context structure(s) host dependent
 *	020	Dave Butenhof	22 November 1989
 *		Make forward decl for semaphore structure to resolve circular
 *		references.
 *	021	Dave Butenhof	30 November 1989
 *		Add field for scheduling policy (not implemented yet).
 *	022	Dave Butenhof	4 December 1989
 *		Remove all the type definitions to cma_tcb_def.h to try to
 *		break circular dependencies.
 *	023	Paul Curtin	8 April 1991
 *		Added externs for use with quick kill
 *	024	Paul Curtin	24 May 1991
 *		Added a prototype for cma__reinit_tcb
 */
#ifndef CMA_TCB
#define CMA_TCB

/*
 *  INCLUDE FILES
 */

#include <cma_tcb_defs.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * CONSTANTS AND MACROS
 */

/*
 * TYPEDEFS
 */

/*
 *  GLOBAL DATA
 */

extern cma__t_int_tcb	    cma__g_def_tcb;	 /* Default thread's TCB */
extern cma__t_static_ctx   *cma__g_def_ctx_buf;	 /* Default static context */
extern cma_t_integer	    cma__g_abort_signal; /* Process abort signal */

/*
 * INTERNAL INTERFACES
 */

extern void cma__destroy_tcb  (cma__t_int_tcb *);

extern void cma__free_tcb  (cma__t_int_tcb *);

extern cma__t_int_tcb * cma__get_tcb  (cma__t_int_attr	*);

extern void cma__init_tcb  (void);

extern void cma__reinit_tcb  (cma_t_integer);

#ifdef __cplusplus
}
#endif

#endif
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_TCB.H */
/*  *6    10-JUN-1991 19:57:00 SCALES "Convert to stream format for ULTRIX build" */
/*  *5    10-JUN-1991 19:21:54 BUTENHOF "Fix the sccs headers" */
/*  *4    10-JUN-1991 18:24:12 SCALES "Add sccs headers for Ultrix" */
/*  *3    24-MAY-1991 16:48:36 CURTIN "Added a new reinit routine" */
/*  *2     8-APR-1991 20:31:17 CURTIN "Added some global defs for quick kill" */
/*  *1    12-DEC-1990 21:53:52 BUTENHOF "Thread bookkeeping" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_TCB.H */
