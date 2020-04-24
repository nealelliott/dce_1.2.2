/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: cma_debug_client.h $Revision: 1.1.8.1 $ $Date: 1996/05/10 13:35:11 $
 */
/*
 * HISTORY
 * $Log: cma_debug_client.h,v $
 * Revision 1.1.8.1  1996/05/10  13:35:11  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:40 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:42 UTC  jss  /main/HPDCE02/1]
 *
 * 	CR10892 - fix copyright in file
 * 	[1994/06/09  13:27:40  devsrc]
 * 	 *
 *
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:16:54  jd]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  15:13  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/20  19:29:20  truitt]
 * 	 *
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:43:13  root  1.1.6.3]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:11:45  hopkins]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  18:00:00  root]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:32:49  hopkins]
 *
 * 	Insert copyright notices
 * 	[1992/12/07  19:37:29  alan]
 *
 * 	Loading drop DCE1_0_2FNL
 *
 * 	[1993/05/04  22:33:42  mgm  1.1.1.11]
 *
 * Revision 1.1.4.2  1994/06/09  13:37:43  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:27:40  devsrc]
 * 
 * Revision 1.1.4.1  1994/04/01  20:15:56  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:16:54  jd]
 * 
 * Revision 1.1.2.2  1992/12/15  22:27:21  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:37:29  alan]
 * 
 * Revision 1.1  1992/01/19  14:39:03  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  Copyright (c) 1990, 1991 by
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
 *	Header file providing access to CMA clients that implement
 * 	language run-times to the CMA debugger capabilities.
 *
 *	NOTE: the clients that are able to use this interface is
 *	very limited because clients needing task debugging must have 
 *	support in the system debugger as well as here (at present).
 *	The following are the only legitimate clients of this interface:
 *	ADA runtime, C++ tasking library, and CMA.
 *
 *FIX-ME* We shall endeavor to extend these capabilities so that the
 * 	all-platform CMA debugger CMA_DEBUG and any client can layer
 * 	on thread debugging.  But that is still an open design problem.
 *	The design here does not preclude that extension (for example,
 *	the identity of the debug-client is indicated in an "open"
 *	manner by using the CMA context key as the identifier.
 *
 *  AUTHORS:
 *
 *	Bob Conti
 *
 *  CREATION DATE:
 *
 *	11 September 1990
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Bob Conti	11 September 1990
 *		Create module
 *	002	Bob Conti	13 September 1990
 *		Add reserved field in prolog for future expandability.
 *		Revise the types so they will be more atomic and can
 *		be used in the CMA TCB.
 *	003	Dave Butenhof	11 February 1991
 *		Change name of alert state bits.
 *	004	Dave Butenhof	25 February 1991
 *		Change sentinel for TCB to something AIX CC can live with.
 *	005	Paul Curtin	06 March 1991
 *		Added cma_debug_get_sp_context proto
 */

#ifndef CMA_DEBUG_CLIENT
#define CMA_DEBUG_CLIENT

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
 * TYPEDEFS
 */

/*
 * Type describing constants for a valid TCB sentinel.
 * Exactly one value is valid, but we provide a symbolic name for
 * at least one invalid sentinel as a convenience.
 */
typedef enum CMA_T_TCB_SENTINEL {
	cma_c_tcb_sentinel_nogood = 0,	/* Invalid sentinel constant */
	cma_c_tcb_sentinel = 0x0ACEFACE /* Valid TCB sentinel */
	} cma_t_tcb_sentinel;

/*
 * Type describing pad fields needed to align the "standard prolog"
 * to the right byte at the front of each TCB.  These fields are
 * free to be put to any use by the client.
 *
 * This is 32 bytes long and is fixed at this size for all clients
 * and CMA, for all time.
 */
typedef struct CMA_T_TCB_PRIVATE {
    cma_t_integer	pad1;		
    cma_t_integer	pad2;	
    cma_t_integer	pad3;
    cma_t_integer	pad4;
    cma_t_integer	pad5;
    cma_t_integer	pad6;
    cma_t_integer	pad7;
    cma_t_integer	pad8;
    } cma_t_tcb_private;

/*
 * Type describing the "standard prolog" that clients should use 
 * within their task control blocks.  We assume that the client will
 * store their "task control block" as a per-thread context under
 * the context key specified here.
 */
typedef struct CMA_T_TCB_PROLOG {
    cma_t_tcb_sentinel	sentinel;	/* Validity sentinel */
    cma_t_thread	client_thread;	/* Thread corresonding to task */
    cma_t_key		client_key;	/* Context key this is stored under */
    cma_t_address	reserved1;	/* Must be zero, reserved to CMA */
    } cma_t_tcb_prolog;

/*
 * Type defining the layout of all TCBs and TASKS.  This format
 * ensures that tasks will be self-identifying to the debugger.
 * this layout must never change as the CMA DEBUG Clients cannot
 * be changed after CMA ships.
 */
typedef struct CMA_T_TCB_HEADER {
    cma_t_tcb_private	IGNORED; 	/* TCB fields private to the client */
    cma_t_tcb_prolog	prolog; 	/* The standard prolog goes here */
    } cma_t_tcb_header;


/*
 * Type describing the kinds of information that a CMA debug
 * client can GET about a thread.
 */
typedef enum CMA_T_DEBUG_GET {
	/* 
	 * All of the following items use a buffer whose size is
	 * four bytes.  (That is four must be passed as the buffer_size
	 * parameter to cma_debug_get.)
	 */
	cma_c_debget_guardsize = 1,	/* Current guard size (bytes) */
	cma_c_debget_is_held = 2, 	/* Is it on hold? */
	cma_c_debget_is_initial = 3, 	/* Is it the initial thread? */
	cma_c_debget_number = 4, 	/* Thread's number */
	cma_c_debget_stack_ptr = 5, 	/* Current stack pointer */
	cma_c_debget_stack_base = 6, 	/* Stack base address */
	cma_c_debget_stack_top 	= 7, 	/* Stack top address */
	cma_c_debget_sched_state = 8,	/* Scheduler state 
					 *	0 - run
					 *	1 - ready
					 * 	2 - blocked
					 * 	3 - terminated
					 */
	cma_c_debget_reserve_size = 9, 	/* Size of stack reserve (bytes) */
	cma_c_debget_base_prio = 10, 	/* Base priority */
	cma_c_debget_priority = 11, 	/* Current priority */
	cma_c_debget_regs = 12,		/* Register set (and proc. state) */
	cma_c_debget_alt_pending = 13,	/* Alert is pending */
	cma_c_debget_alt_a_enable = 14,	/* Asynch alert delivery enabled */
	cma_c_debget_alt_g_enable = 15,	/* General alert delivery enabled */
	cma_c_debget_substate = 16,	/* Substate (or wait state) */
	cma_c_debget_object_addr = 17,	/* Address of thread object */
	cma_c_debget_thkind = 18,	/* Kind of thread */
	cma_c_debget_detached	= 19,	/* Thread is detached */
	cma_c_debget_tcb_size	= 20,	/* TCB size */
	cma_c_debget_start_pc	= 21,	/* Start address */
	cma_c_debget_next_pc	= 22,	/* Next instruction */
	cma_c_debget_policy	= 23,	/* Sched policy */
	cma_c_debget_stack_yellow = 24,	/* Addr of start of guard area */
	cma_c_debget_stack_default = 25	/* True if on default stack */

	} cma_t_debug_get;

/*
 * Type describing the kinds of information that a CMA debug
 * client can SET (or change) about a thread using cma_debug_set.
 */
typedef enum CMA_T_DEBUG_SET {
	/* 
	 * All of the following items use a buffer whose size is
	 * four bytes.  (That is four must be passed as the buffer_size
	 * parameter to cma_debug_set.)
	 */
	cma_c_debset_priority	= 1, 	/* Set the priority */
	cma_c_debset_policy	= 2,	/* Set the sched policy */
	cma_c_debset_hold	= 3,	/* Put thread on hold */
	cma_c_debset_regs	= 4	/* Set the regs and proc. state */

	} cma_t_debug_set;


/*
 *  GLOBAL DATA
 * 
 * 	none
 */

/*
 * EXTERNAL INTERFACES
 */

/* 
 * Routine to register with the CMA debug dispatcher.
 */
extern void     cma_debug_register (cma_t_address,cma_t_key,cma_t_integer,cma_t_boolean);

/* 
 * Routine to get get thread state needed by the CMA debug client.
 */
extern void     cma_debug_get (cma_t_thread *,cma_t_debug_get,cma_t_address,cma_t_integer);

/* 
 * Get thread context given an sp and a key 
 */
extern void cma_debug_get_sp_context    (cma_t_address,cma_t_key,cma_t_address *);

/* 
 * Routine to set thread state as needed by the CMA debug client.
 */
extern void     cma_debug_set (cma_t_thread *,cma_t_debug_set,cma_t_address,cma_t_integer);

#ifdef __cplusplus
}
#endif

#endif

/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_DEBUG_CLIENT.H */
/*  *7    10-JUN-1991 19:51:46 SCALES "Convert to stream format for ULTRIX build" */
/*  *6    10-JUN-1991 19:20:22 BUTENHOF "Fix the sccs headers" */
/*  *5    10-JUN-1991 18:19:15 SCALES "Add sccs headers for Ultrix" */
/*  *4     7-MAR-1991 11:07:08 CURTIN "added proto for cma_debug_get_sp_context" */
/*  *3    25-FEB-1991 18:25:56 BUTENHOF "Change TCB sentinel" */
/*  *2    12-FEB-1991 01:28:55 BUTENHOF "Change alert bits" */
/*  *1    12-DEC-1990 21:43:50 BUTENHOF "Debug support" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_DEBUG_CLIENT.H */
