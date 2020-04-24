/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: cma_assem.h $Revision: 1.1.10.1 $ $Date: 1996/05/10 13:33:14 $
 */
/*
 * HISTORY
 * $Log: cma_assem.h,v $
 * Revision 1.1.10.1  1996/05/10  13:33:14  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:38 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:41 UTC  jss  /main/HPDCE02/1]
 *
 * 	CR10892 - fix copyright in file
 * 	[1994/06/09  13:27:19  devsrc]
 * 	 *
 *
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:15:45  jd]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  13:44  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/20  15:09:33  truitt]
 * 	 *
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:42:53  root  1.1.6.3]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:10:52  hopkins]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  17:48:15  root]
 *
 * 	Insert copyright notices
 * 	[1992/12/07  19:35:53  alan]
 *
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:01:25  bolinger]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:31:47  hopkins]
 *
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  22:43:00  bolinger]
 *
 * 	Remove unused parameters from assembly language routines.  Previously,
 * 	two parameters to a few assembly routines were rolled into the TCB, but
 * 	the obsolete parameters were never removed.
 * 	[1992/04/28  17:57:47  keane]
 *
 * 	Loading drop DCE1_0_2FNL
 * 	[1993/05/04  22:33:07  mgm  1.1.1.11]
 *
 * Revision 1.1.6.2  1994/06/09  13:37:18  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:27:19  devsrc]
 * 
 * Revision 1.1.6.1  1994/04/01  20:15:23  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:15:45  jd]
 * 
 * Revision 1.1.4.3  1992/12/15  22:25:55  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:35:53  alan]
 * 
 * Revision 1.1.4.2  1992/09/03  14:34:31  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:01:25  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/08  22:44:31  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  22:43:00  bolinger]
 * 
 * Revision 1.1.2.2  1992/04/28  18:58:54  keane
 * 	Remove unused parameters from assembly language routines.  Previously,
 * 	two parameters to a few assembly routines were rolled into the TCB, but
 * 	the obsolete parameters were never removed.
 * 	[1992/04/28  17:57:47  keane]
 * 
 * Revision 1.1  1992/01/19  14:38:48  devrcs
 * 	Initial revision
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
 *	Header file for non-portable (ie, non-C) thread services subroutines.
 *	Although the routines themselves are not portable, the interfaces are
 *	the same on each platform and, therefore, so is this file.
 *
 *  AUTHORS:
 *
 *	Webb Scales
 *
 *  CREATION DATE:
 *
 *	25 August 1989
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Webb Scales and Dave Butenhof	27 September 1989
 *		Add globalvalue for cma__c_default_ps value.
 *	002	Bob Conti	6 October 1989
 *		Move check that cma_s_bugcheck remains constant to 
 *		assertion in cma_thread.c
 *	003	Webb Scales	20 October 1989
 *		Created this file from cma_assem_vms.h
 *	004	Webb Scales	23 October 1989
 *		Changed definition of cma__force_dispatch to look like a routine
 *		Changed "..._PSL" to "..._ps"
 *	005	Webb Scales	 8 November 1989
 *		Add cma__init_assem for VMS
 *	006	Webb Scales	 18 November 1989
 *		Changed cma__t_int_ctx_buffer to cma__t_static_ctx
 *		Removed parent_ctx parameter from call to cma__create_thread
 *		Removed proto for defunct routine, cma__save_thread_ctx
 *	007	Dave Butenhof	4 December 1989
 *		Include cma_tcb_defs.h instead of cma_tcb.h
 *	008	Dave Butenhof	04 April 1991
 *		Minor touchups
 *	009	Paul Curtin	04 April 1991
 *		Added proto for cma__transfer_main_abort
 *	010	DECthreads team	    22 July 1991
 *		Added cma__save_exc_context, & cma__restore_exc_context
 *		in final effort to free DECthreads from crtl on VAX/VMS
 *	011	Dave Butenhof	21 November 1991
 *		Clean up the file.
 *	012	Webb Scales	30 January 1992
 *		Implement our own private $CLRAST
 *	013	Dave Butenhof	13 March 1992
 *		Some time ago, I dropped the start routine and arg arguments
 *		from cma__thread_base(), putting them in the TCB instead. I
 *		never got around to changing all the other calls that lead up
 *		to that, however. Some instructions will be saved by not
 *		passing those two arguments from cma__int_make_thread to
 *		cma__create_thread to cma__execute_thread and back up to
 *		cma__thread_base (which doesn't use them anyway). Gee, wish
 *		I'd thought of that! :-)
 */

#ifndef CMA_ASSEM
#define CMA_ASSEM

/*
 *  INCLUDE FILES
 */

#include <cma_tcb_defs.h>
#include <cma.h>

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

/*
 * INTERNAL INTERFACES
 */

#if _CMA_PLATFORM_ == _CMA__VAX_VMS
/*
 * Implement a private version of $CLRAST so that DECthreads doesn't have to
 * depend on SYS.STB (i.e., any particular version of the operating system).
 */
extern void cma__clrast  (void);
#endif

/*
 * Initialize a thread context and stack and prepare it to be scheduled
 */
extern void cma__create_thread  (cma__t_static_ctx *,cma_t_address,cma__t_int_tcb *);

/*
 * cma__do_async_alert is the address to jump to deliver an asynchronous
 * alert. The address of the instruction pointed to by this symbol is placed
 * in the  context area for a signal or AST and is used as the restart PC
 * when the  signal or AST ends.  This causes the execution to resume inside
 * the scheduler,  instead of at the point of interruption, and a context
 * switch is made.
 *
 * Note: this symbol does not necessarily point to the beginning of a
 * routine, and explicit calls should not be made to this address.
 */
extern void cma__do_async_alert  (void);

/*
 * Asynchronous interrupt launch code.  This isn't actually called; it's
 * simply put into the VP's state.  The arguments are
 *
 *	s0	address of C interrupt handler
 *	a0	argument to handler
 */
extern void cma__do_interrupt  (void);

/*
 * Breakpoint for use by cma_debug ()
 */
extern void cma__do_break  (void);

/*
 * Actually perform a call to a routine on a different stack
 */
extern cma_t_address cma__do_call_on_stack  (cma_t_address,cma_t_call_routine,cma_t_address);

/*
 * This is the base routine for a thread.  It's frame is at the bottom of the
 * stack and holds the cma catch-all condition handler.  It is responsibile for
 * calling the thread start routine and for calling the thread cleanup routines.
 * This function is never directly called, but the address & argument list is
 * placed into the context area when a new thread is created.
 */
extern void cma__execute_thread  (cma__t_int_tcb);

/* 
 * cma__force_dispatch is the address to jump to cause an involuntary dispatch.
 * The address of the instruction pointed to by this symbol is placed in the 
 * context area for a signal or AST and is used as the restart PC when the 
 * signal or AST ends.  This causes execution to resume inside the scheduler, 
 * instead of at the point of interruption, and a context switch is made.
 *
 * Note: this symbol does not necessarily point to the beginning of a routine,
 * and explicit calls should not be made to this address.
 */
extern void cma__force_dispatch  (void);

#if _CMA_OS_ == _CMA__VMS
/*
 * Initialize the assembler module.
 * Determine "base frame" for thread stacks.
 */
extern void cma__init_assem  (void);

/*
 *  Restore routine context after the $UNWIND during exception delivery.
 */
extern void cma__restore_exc_context  (void);

/*
 * Save registers so that they may be restored during exception delivery.
 */
extern cma_t_integer cma__save_exc_context  (cma_t_address);

#endif

/*
 * Restore current thread context (From either sync or asynch context switch)
 */
extern void cma__restore_thread_ctx  (cma__t_static_ctx *); /* Address of buffer to restore */

/*
 * cma__transfer_main_abort is used to abort the process by forcing a context
 * switch to the main thread and raising a signal. The signal number is
 * stored in cma__g_abort_signal.
 */
extern void cma__transfer_main_abort  (void);

/*
 * Transfer execution to a new thread 
 */
extern void cma__transfer_thread_ctx  (cma__t_static_ctx *,cma__t_static_ctx *); 

#ifdef __cplusplus
}
#endif

#endif
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_ASSEM.H */
/*  *10   13-MAR-1992 14:07:21 BUTENHOF "Remove excess thread_base arguments" */
/*  *9    30-JAN-1992 22:12:10 SCALES "Implement our own $CLRAST" */
/*  *8    21-NOV-1991 13:54:08 BUTENHOF "Update this file -- it's OLD" */
/*  *7    26-JUL-1991 15:52:16 CURTIN "added *_exc_context routines" */
/*  *6    10-JUN-1991 19:50:20 SCALES "Convert to stream format for ULTRIX build" */
/*  *5    10-JUN-1991 19:19:57 BUTENHOF "Fix the sccs headers" */
/*  *4    10-JUN-1991 18:16:44 SCALES "Add sccs headers for Ultrix" */
/*  *3    12-APR-1991 23:34:46 BUTENHOF "OSF/1 Mach thread support" */
/*  *2     8-APR-1991 20:30:23 CURTIN "added a proto for transfer_main_abort" */
/*  *1    12-DEC-1990 21:40:53 BUTENHOF "Assembly code header" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_ASSEM.H */
