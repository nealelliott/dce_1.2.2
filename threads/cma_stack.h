/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 *
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: cma_stack.h $Revision: 1.1.9.1 $ $Date: 1996/05/10 13:42:01 $
 */
/*
 * HISTORY
 * $Log: cma_stack.h,v $
 * Revision 1.1.9.1  1996/05/10  13:42:01  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:45 UTC  bissen
 * 	Merge singlestack code.
 * 	[1994/10/27  21:05 UTC  chris_p  /main/HPDCE02/2]
 *
 * 	Change symbol names with "carevue" to "singlestack"
 * 	[1994/10/17  20:30 UTC  chris_p  /main/HPDCE02/proven_threads_940803/2]
 *
 * 	Add stack growth code.
 * 	[1994/10/06  21:52 UTC  chris_p  /main/HPDCE02/proven_threads_940803/1]
 *
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:44 UTC  jss  /main/HPDCE02/1]
 *
 * 	CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:34  devsrc]
 * 	 *
 *
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:18:50  jd]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  18:17  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/21  14:49:55  truitt]
 * 	 *
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:44:18  root  1.1.6.3]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:13:57  hopkins]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  18:48:13  root]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:35:01  hopkins]
 *
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  17:27:28  hinman]
 *
 * 	[hinman] - Check in merged SNI version
 * 	[1993/01/11  16:21:22  hinman]
 *
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:54:16  bolinger]
 *
 * 	Loading drop DCE1_0_2FNL
 * 	[1993/05/04  22:35:27  mgm  1.1.1.11]
 *
 * 	[hinman] - Fold forward from raj_om
 * 	[92/09/10  16:35:24  hinman  9.5.1.2]
 *
 * 	[raj] merging SVR4 changes into 1.0.2
 * 	[92/09/10  16:09:21  raj  9.5.2.2]
 *
 * 	Update checkin
 * 	[92/09/02  14:07:55  hinman  9.9]
 *
 * Revision 1.1.5.2  1994/06/09  13:38:49  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:34  devsrc]
 * 
 * Revision 1.1.5.1  1994/04/01  20:17:19  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:18:50  jd]
 * 
 * Revision 1.1.2.4  1993/02/01  22:19:07  hinman
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  17:27:28  hinman]
 * 
 * 	[hinman] - Check in merged SNI version
 * 	[1993/01/11  16:21:22  hinman]
 * 
 * Revision 9.5.1.2  92/09/10  16:35:24  hinman
 * 	[hinman] - Fold forward from raj_om
 * 
 * Revision 9.5.2.2  92/09/10  16:09:21  raj
 * 	[raj] merging SVR4 changes into 1.0.2
 * 
 * Revision 9.9  92/09/02  14:07:55  hinman
 * 	Update checkin
 * 
 * Revision 1.1.2.2  1992/05/08  23:54:29  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:54:16  bolinger]
 * 
 * $EndLog$
 */
/*
 *	%W%	(DEC OSF/1)	%G%
 */
/*
 *  Copyright (c) 1989, 1992 by
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
 *	Header file for stack management
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	21 August 1989
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	29 September 1989
 *		Make cma__assign_chunks external, and add tcb backpointer to
 *		stack descriptor.
 *	002	Dave Butenhof	All Saints Day 1989
 *		Add prototype for get_tcb_kernel.
 *	003	Dave Butenhof	4 December 1989
 *		Include cma_tcb_defs.h instead of cma_tcb.h
 *	004	Dave Butenhof	10 April 1990
 *		If _CMA_UNIPROCESSOR_ is defined, declare cma__get_self_tcb
 *		as a macro returning the dispatcher's current thread pointer,
 *		instead of as a prototype for the stack cluster search
 *		function.
 *	005	Paul Curtin	27 June 1990
 *		Added cma__g_chunk_size
 *	006	Dave Butenhof	29 November 1990
 *		Add cma__get_sp_tcb to return a TCB for some specified SP
 *		value.
 *	007	Dave Butenhof	14 December 1990
 *		Change cma_attributes.h to cma_attr.h (shorten length)
 *	008	Dave Butenhof	04 April 1991
 *		Change _CMA_UNIPROCESSOR_ to 0/1 instead of ifdef/ifndef
 *	009	Paul Curtin	24 May 1991
 *		Added cma__reinit_stack prototype.
 *	010	Dave Butenhof	02 October 1991
 *		Integrate changes provided by Alan Peckham to unprotect guard
 *		pages on all stacks before aborting the process; this allows
 *		the UNIX core dump code (which makes the absurd assumption
 *		that the known universe stops just because a page is
 *		protected) to work on threaded processes.
 *	011	Dave Butenhof	13 March 1992
 *		Move cma__roundup_chunksize from cma_stack_int.h, since an
 *		"__" interface belongs here.
 */
#ifndef CMA_STACK
#define CMA_STACK

/*
 *  INCLUDE FILES
 */

#include <cma_tcb_defs.h>
#include <cma.h>
#include <cma_attr.h>
#include <cma_queue.h>
#include <cma_stack_int.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * CONSTANTS AND MACROS
 */

#if _CMA_UNIPROCESSOR_
# define cma__get_self_tcb()	(cma__g_current_thread)
#endif

/*
 * Round the given value (a) upto cma__g_chunk_size
 */
#define cma__roundup_chunksize(a)   (cma__roundup(a,cma__g_chunk_size))

/*
 * TYPEDEFS
 */

/*
 *  GLOBAL DATA
 */

extern cma__t_list	cma__g_stack_clusters;
extern cma__t_int_tcb	*cma__g_current_thread;
extern cma_t_integer	cma__g_chunk_size;

/*
 * INTERNAL INTERFACES
 */

extern void cma__assign_stack  (cma__t_int_stack *,cma__t_int_tcb *);

extern void cma__free_stack  (cma__t_int_stack *);

extern void cma__free_stack_list  (cma__t_queue *);

#if !_CMA_UNIPROCESSOR_
extern cma__t_int_tcb * cma__get_self_tcb  (void);
#endif

extern cma__t_int_tcb * cma__get_sp_tcb  (cma_t_address);

#ifdef HP_CMA_SINGLESTACK_OPT
extern cma__t_int_stack * cma__get_stack  (cma__t_int_attr *, cma__t_int_attr *);
#else
extern cma__t_int_stack * cma__get_stack  (cma__t_int_attr *);
#endif

extern void cma__init_stack  (void);

extern void cma__reinit_stack  (cma_t_integer);

#if _CMA_PROTECT_MEMORY_
extern void cma__remap_stack_holes  (void);
#endif

#ifdef __cplusplus
}
#endif

#endif
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_STACK.H */
/*  *10   13-MAR-1992 14:09:59 BUTENHOF "Rearrange stack/guard rounding" */
/*  *9    29-JAN-1992 14:37:24 BUTENHOF "Improve special cluster management" */
/*  *8    14-OCT-1991 13:40:22 BUTENHOF "Unprotect guard pages on abort" */
/*  *7    10-JUN-1991 19:56:38 SCALES "Convert to stream format for ULTRIX build" */
/*  *6    10-JUN-1991 19:21:44 BUTENHOF "Fix the sccs headers" */
/*  *5    10-JUN-1991 18:23:51 SCALES "Add sccs headers for Ultrix" */
/*  *4    24-MAY-1991 16:47:45 CURTIN "Added a new reinit routine" */
/*  *3    12-APR-1991 23:36:59 BUTENHOF "Change _CMA_UNIPROCESSOR_ to 0/1" */
/*  *2    14-DEC-1990 00:55:58 BUTENHOF "Change module names" */
/*  *1    12-DEC-1990 21:53:27 BUTENHOF "Thread stacks" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_STACK.H */
