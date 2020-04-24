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
 * @(#)Module: cma_tcb_defs.h $Revision: 1.1.13.1 $ $Date: 1996/05/10 13:42:47 $
 * @(#)Module: cma_tcb_defs.h $Revision: 1.1.13.1 $ $Date: 1996/05/10 13:42:47 $
 */
/*
 * HISTORY
 * $Log: cma_tcb_defs.h,v $
 * Revision 1.1.13.1  1996/05/10  13:42:47  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/2  1996/03/08  21:46 UTC  bissen
 * 	Merge singlestack code.
 * 	[1994/10/27  21:06 UTC  chris_p  /main/HPDCE02/3]
 *
 * 	Grab the latest DCE bits and make sure they work with the sinlestack hack
 * 	[1994/10/27  17:32 UTC  chris_p  /main/HPDCE02/proven_threads_940803/3]
 *
 * 	Merge from branch jss_close_fix.
 * 	[1994/09/08  19:06 UTC  jss  /main/HPDCE02/2]
 *
 * 	Fix for CHFts12442. Add select() functionality.
 * 	[1994/09/08  13:39 UTC  jss  /main/HPDCE02/jss_fix_close/1]
 *
 * 	Change symbol names with "carevue" to "singlestack"
 * 	[1994/10/17  20:30 UTC  chris_p  /main/HPDCE02/proven_threads_940803/2]
 *
 * 	Add sigwait_block structure to tcb
 * 	[1994/10/06  21:52 UTC  chris_p  /main/HPDCE02/proven_threads_940803/1]
 *
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:44 UTC  jss  /main/HPDCE02/1]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  19:06  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/21  18:02:07  truitt]
 * 	 *
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:44:27  root  1.1.10.3]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	redefine "class" and "new" keywords
 * 	[1993/08/03  18:14:21  hopkins]
 *
 * 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
 * 	[1993/06/17  14:46:00  hinman]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  18:54:54  root]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:35:25  hopkins]
 *
 * 	Gamera merge
 * 	[1993/03/26  23:56:03  hopkins]
 *
 * 	Loading drop DCE1_0_2B18
 * 	[1993/03/11  22:28:41  osfrcs  1.1.1.8]
 *
 * 	Add per thread h_errno and rpc_createrr
 * 	if _HP_LIBC_R
 * 	DCE 1.0.1 Release
 * 	[1992/11/10  23:49:57  hopkins]
 *
 * 	HP revision /main/DCE_1.2/1  1996/01/03  18:49 UTC  psn
 * 	Changes to allow valid AIX core dumps.
 * 	[1995/04/26  16:56 UTC  jrr  /main/DCE1_1_WP/1]
 *
 * Revision 1.1.6.3  1994/06/09  13:38:55  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:40  devsrc]
 * 
 * Revision 1.1.6.2  1994/04/25  19:57:50  ohara
 * 	code cleanup
 * 	[1994/04/25  18:17:56  ohara]
 * 
 * Revision 1.1.6.1  1994/04/01  20:17:28  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:19:02  jd]
 * 
 * Revision 1.1.4.2  1993/06/24  21:04:50  hinman
 * 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
 * 	[1993/06/17  14:46:00  hinman]
 * 
 * Revision 1.1.2.4  1992/12/15  22:31:28  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:42:20  alan]
 * 
 * Revision 1.1.2.3  1992/09/29  20:15:49  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:54:51  sekhar]
 * 
 * Revision 1.1.2.2  1992/09/03  14:40:54  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:06:45  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/09  00:00:16  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:59:58  bolinger]
 * 
 * Revision 1.1  1992/01/19  14:40:25  devrcs
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
 *	Concert Multithread Architecture (CMA) services
 *
 *  ABSTRACT:
 *
 *	TCB-related type definitions.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	4 December 1989
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	5 December 1989
 *		Remove initial_test attribute.
 *	002	Dave Butenhof	8 December 1989
 *		Add pointer to base of thread's exception stack.
 *	003	Webb Scales	11 January 1990
 *		Renamed the select field from nfds to nfound, as these names
 *		each have particular connotations, and the wrong one was
 * 		used.
 *	004	Dave Butenhof	12 February 1990
 *		Add field for per-thread errno value.
 *	005	Webb Scales	30 May 1990
 *		Add reference count of joiners to prevent zombies from being
 *              dispelled prematurely.
 *	006	Webb Scales	15 June 1990
 *		Added priority scheduling:  Created new tcb substructure for 
 *		scheduling and moved cpu_time, policy, and priority into it.
 *	007	Webb Scales	16 August 1990
 *		Replaced #ifdef's with #if's
 *	008	Bob Conti	9 September 1990
 *		Add debug context and standard TCB prolog so that all
 *		task control blocks (ours and clients as well) can be 
 *		identified while the debugger is active.  Also added, 
 *		for Webb, a condition variable tswait_cv to be used for all
 *		thread-synchronous waits (aka self-waits) done by the thread.
 *		New include files have been added so that more types can be 
 *		stored in the TCB; all of cma.h is now available.
 *	009	Webb Scales	13 September 1990
 *		Changed tswait to internal format.
 *	010	Webb Scales	20 September 1990
 *		Added a mutex for thread-synchronous wait CV.
 *	011	Webb Scales	25 September 1990
 *		Added a separate queue node for select.
 *	012	Bob Conti	28 September 1990
 *		Add cast to fix build bug.
 *	013	Paul Curtin	11 December 1990
 *		Added sigaction_data array to tcb
 *	014	Dave Butenhof	5 February 1991
 *		Rename "errno" field.
 *	015	Dave Butenhof	7 February 1991
 *		Redefine alert state fields.
 *	016	Webb Scales	18 March 1991
 *		Added sched.q_num to improve variable priority scheduling.
 *	017	Dave Butenhof	25 March 1991
 *		Change exception interface names
 *	018	Dave Butenhof	05 June 1991
 *		Integrate IBM changes
 *	019	Dave Butenhof and Webb Scales	05 June 1991
 *		Conditionalize vacuous (forward) structure defs, since MIPS C
 *		V2.1 doesn't like (or, apparently, need).
 *	020	Paul Curtin	05 June 1991
 *		Remove include of exc_handling.h, gets rid of a 
 *		circularity problem.  We get it from cma.h any way.
 *	021	Webb Scales and Dave Butenhof	    10 June 1991
 *		Conditionalize inclusion of I/O stuff.
 *	022	Dave Butenhof	01 July 1991
 *		Add thread start function and argument to TCB so that I can
 *		make cma__thread_base loop rather than creating new threads.
 *	023	Dave Butenhof	12 September 1991
 *		Shrink TCB by dynamically allocating file masks (which are
 *		now 4096 bits on ULTRIX V4.2).
 *	024	Dave Butenhof	04 October 1991
 *		Clean up use of _CMA_UNIPROCESSOR_
 *	025	Paul Curtin	20 November 1991
 *		Made exc_t_context volatile
 *	026	Dave Butenhof	27 November 1991
 *		Add thd_vmserrno field for DEC C and VAX C "vaxc$errno" (this
 *		is long overdue :-) ).
 *	027	Dave Butenhof	09 December 1991
 *		Align the count field of the alert structure to longword
 *		boundary.
 *	028	Dave Butenhof	13 December 1991
 *		Change name of exc_t_context to exc_context_t.
 *	029	Webb Scales	13 March 1992
 *		Parameterize scheduling policies.
 *	030	Dave Butenhof and Webb Scales	17 April 1992
 *		Remove "volatile" from exc_stack -- we don't think it's
 *		needed.
 */


#ifndef CMA_TCB_DEFS
#define CMA_TCB_DEFS

/*
 *  INCLUDE FILES
 */
# if !_CMA_THREAD_SYNC_IO_
#  include <cma_thread_io.h>
# endif
#include <cma.h>
#include <cma_debug_client.h>
#include <cma_attr.h>
#include <cma_defs.h>
#include <cma_handle.h>
#include <cma_queue.h>
#if _CMA_OS_ == _CMA__UNIX
# if defined(SNI_DCOSX)
#   include <sys/ucontext.h>
# endif
# include <signal.h>
#endif
#include <cma_sched.h>
#ifdef _HP_LIBC_R
# include <rpc/rpc.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
 * CONSTANTS AND MACROS
 */

#ifdef __cplusplus
#define	class	c_class
#endif

#if _CMA_PLATFORM_ == _CMA__IBMR2_UNIX
# define cma__c_ibmr2_ctx_stack_size	2048
# define cma__c_ibmr2_ctx_stack_top	(cma__c_ibmr2_ctx_stack_size - 1)
#endif

/*
 * TYPEDEFS
 */

#ifndef __STDC__
# if _CMA_HARDWARE_ != _CMA__HPPA
struct CMA__T_SEMAPHORE;
# endif
struct CMA__T_INT_CV;
struct CMA__T_INT_MUTEX;
struct CMA__T_INT_TCB;
#endif

typedef cma_t_address		*cma__t_context_list;

#ifdef HP_CMA_SINGLESTACK_OPT
#include <cma_cond_defs.h>

typedef struct CMA___T_SIGWAIT_BLOCK {
    cma__t_queue    queue;  /* Must be the first field */
    sigset_t        *set;   /* Signals which thread is awaiting */
    cma_t_integer   signal; /* Signal that the thread received */
    cma__t_int_cv   *cv;    /* Condition variable thread blocks on */
    /* cma__t_int_tcb  *tcb;   Just for debugging */
	struct CMA__T_INT_TCB *tcb;
    } cma___t_sigwait_block;
#endif 

typedef struct CMA__T_TCB_PAD {
#ifdef	AIX_CORE_FILE
    /*
     * header has been modified to be 24 bytes - pad 8 bytes
     */
    cma_t_integer	pad1;		/* pad bytes */
    cma_t_integer	pad2;		/* pad bytes */
#else
    /* 
     * Adjust to align the tcb prolog at byte 32.
     * 12 bytes are required as object header is currently 
     * 20 bytes long.
     */
    cma_t_integer	pad1;		/* pad bytes */
    cma_t_integer	pad2;		/* pad bytes */
    cma_t_integer	pad3;		/* pad bytes */
#endif	/* AIX_CORE_FILE */
    } cma__t_tcb_pad;

#if (_CMA_OS_ == _CMA__UNIX) && !_CMA_THREAD_SYNC_IO_
typedef struct CMA__T_TCB_SELECT {
    cma__t_queue	queue;
#if (_CMA_UNIX_TYPE !=  _CMA__SVR4)
	cma__t_file_mask    *rfds;
	cma__t_file_mask    *wfds;
	cma__t_file_mask    *efds;
#else
	cma__t_poll_info 	poll_info;
#endif /* (_CMA_UNIX_TYPE !=  _CMA__SVR4) */
    cma_t_integer	nfound;
    } cma__t_tcb_select;
#endif

typedef struct CMA__T_TCB_TIME {
    cma__t_queue	queue;		/* must be first entry! */
    cma_t_integer	mode;
    struct CMA__T_SEMAPHORE *semaphore;	/* used for timed semaphores */
    cma_t_date_time	wakeup_time;
    cma_t_integer	quanta_remaining;
    } cma__t_tcb_time;

typedef enum CMA__T_DEBEVT {
	cma__c_debevt_activating = 1,	/* First transition to running */
	cma__c_debevt_running = 2,	/* Any transition to running */
	cma__c_debevt_preempting = 3,	/* Preemted (replaced) another thread */
	cma__c_debevt_blocking = 4,	/* Any transition to blocked */
	cma__c_debevt_terminating = 5,	/* Final state transition */
	cma__c_debevt_term_alert = 6,	/* Terminated due to alert/cancel */
	cma__c_debevt_term_exc = 7,	/* Terminated due to exception */
	cma__c_debevt_exc_handled = 8	/* Exception is being handled */
	} cma__t_debevt;

#define cma__c_debevt__first ((cma_t_integer)cma__c_debevt_activating)
#define cma__c_debevt__last  ((cma_t_integer)cma__c_debevt_exc_handled)
#define cma__c_debevt__dim (cma__c_debevt__last + 1)

/* 
 * Type defining thread substate, which is used by the debugger.
 * If the state is blocked, substate indicates WHY the thread is blocked.
 */
typedef enum CMA__T_SUBSTATE {
    cma__c_substa_normal = 0,
    cma__c_substa_mutex = 1,
    cma__c_substa_cv = 2,
    cma__c_substa_timed_cv = 3,
    cma__c_substa_term_alt = 4,
    cma__c_substa_term_exc = 5,
    cma__c_substa_delay =6,
    cma__c_substa_not_yet_run = 7
    } cma__t_substate;
#define cma__c_substa__first ((cma_t_integer)cma__c_substa_normal)
#define cma__c_substa__last  ((cma_t_integer)cma__c_substa_not_yet_run)
#define cma__c_substa__dim (cma__c_substa__last + 1)


/*
 * Per-thread state for the debugger
 */
typedef struct CMA__T_TCB_DEBUG {
    cma_t_boolean	on_hold;	/* Thread was put on hold by debugger */
    cma_t_boolean	activated;	/* Activation event was reported */
    cma_t_boolean	did_preempt;	/* Thread preempted prior one */
    cma_t_address	start_pc;	/* Start routine address */
    cma_t_address	object_addr;	/* Addr of thread object */
    cma__t_substate	substate;	/* Reason blocked, terminated, etc.*/
    cma_t_boolean	notify_debugger;/* Notify debugger thread is running */
    cma_t_address	SPARE2;		/* SPARE */
    cma_t_address	SPARE3;		/* SPARE */
    struct CMA__T_INT_TCB	
			*preempted_tcb;	/* TCB of thread that got preempted */
    cma_t_boolean	flags[cma__c_debevt__dim]; 	
					/* Events enabled for this thread */
    } cma__t_tcb_debug;

typedef struct CMA__T_TCB_SCHED {
    cma_t_integer	adj_time;	/* Abs. time in ticks of last prio adj */
    cma_t_integer	tot_time;	/* Weighted ave in ticks (scaled) */
    cma_t_integer	time_stamp;	/* Abs. time in ticks of last update */
    cma_t_integer	cpu_time;	/* Weighted average in ticks */
    cma_t_integer	cpu_ticks;	/* # of ticks while comp. (scaled) */
    cma_t_integer	q_num;		/* Number of last ready queue on */
    cma_t_priority	priority;	/* Thread priority */
    cma_t_sched_policy  policy;         /* Scheduling policy of thread */
    cma_t_boolean	rtb;		/* "Run 'Till Block" scheduling */
    cma_t_boolean	spp;		/* "Strict Priority Preemption" sched */
    cma_t_boolean	fixed_prio;	/* Fixed priority */
    cma__t_sched_class	class;		/* Scheduling class */
    struct CMA__T_VP	*processor;	/* Current processor (if running) */
    } cma__t_tcb_sched;

typedef struct CMA__T_INT_ALERT {
    cma_t_boolean	pending : 1;	/* alert_pending bit */
    cma_t_boolean	g_enable : 1;	/* general delivery state */
    cma_t_boolean	a_enable : 1;	/* asynchronous delivery state */
    cma_t_integer	spare : 29;	/* Pad to longword */
    cma_t_natural	count;		/* Alert scope nesting count */
    } cma__t_int_alert;

typedef enum CMA__T_STATE {
    cma__c_state_running = 0,		/* For consistency with initial TCB */
    cma__c_state_ready	= 1,
    cma__c_state_blocked = 2,
    cma__c_state_terminated = 3
    } cma__t_state;
#define cma__c_state__first ((cma_t_integer)cma__c_state_running)
#define cma__c_state__last  ((cma_t_integer)cma__c_state_terminated)
#define cma__c_state__dim (cma__c_state__last + 1)

typedef enum CMA__T_THKIND {
    cma__c_thkind_initial = 0,		/* Initial thread */
    cma__c_thkind_normal = 1,		/* Normal thread */
    cma__c_thkind_null	= 2		/* A null thread */
    } cma__t_thkind;
#define cma__c_thkind__first ((cma_t_integer)cma__c_thkind_initial)
#define cma__c_thkind__last  ((cma_t_integer)cma__c_thkind_null)
#define cma__c_thkind__dim (cma__c_thkind__last + 1)

typedef enum CMA__T_SYSCALL_STATE {
	cma__c_syscall_ok = 1,                      /* syscall was not interrupted */
	cma__c_syscall_intintrpt = 1,       /* syscall was interrupted by VTALRM */
	cma__c_syscall_extintrpt = 2        /* syscall was interrupted by external signal */
	} cma__t_syscall_state;


typedef struct CMA__T_INT_TCB {
    /* 
     * Fixed part of TCB.
     *   Modifications to the following three fields must be coordinated.  
     *   The object header must always be first, and the prolog must always 
     *   remain at the same offset (32) for all time. Thus the object header
     *   must never grow beyond a maximum of 32 bytes.
     */
    cma__t_object	header;		/* Common object header */
    cma__t_tcb_pad	pad1;		/* Pad required to align prolog */
    cma_t_tcb_prolog	prolog;		/* Standard prolog for tasks, threads */

    /* 
     * Floating part of TCB (fields here on are free to be moved and resized).
     */
    cma__t_queue	threads;	/* List of all known threads */
    cma__t_int_attr	*attributes;	/* Backpointer to attr obj */
    cma__t_state	state;		/* Current state of thread */
    cma__t_thkind	kind;		/* Which kind of thread */
    struct CMA__T_INT_MUTEX 
			*mutex;		/* Mutex to control TCB access */
    struct CMA__T_INT_CV 
			*term_cv;	/* CV for join */
    struct CMA__T_INT_MUTEX 
			*tswait_mutex;	/* Mutex for thread-synchronous waits */
    struct CMA__T_INT_CV 
			*tswait_cv;	/* CV for thread-synchronous waits */
    cma_t_start_routine	start_code;	/* Address of start routine */
    cma_t_address	start_arg;	/* Argument to pass to start_code */
    cma__t_queue	stack;		/* Queue header for stack descr. */
    cma_t_natural	context_count;	/* Size of context array */
    cma__t_context_list	contexts;	/* Context value array pointer */
    cma_t_exit_status	exit_status;	/* Exit status of thread */
    cma_t_address	return_value;	/* Thread's return value */
    cma__t_async_ctx	async_ctx;	/* Asynchronous context switch info */
    cma__t_static_ctx	static_ctx;	/* Static context switch information */
    cma_t_integer	event_status;	/* Status of semaphore operation */
    cma__t_tcb_time	timer;		/* Time info for dispatcher */
    cma__t_tcb_sched	sched;		/* Scheduler info */
    cma__t_tcb_debug	debug;		/* Debugger info */
#if _CMA_OS_ == _CMA__UNIX
# if !_CMA_THREAD_SYNC_IO_
    cma__t_tcb_select	select;		/* Select info for timed selects */
# endif
    struct sigaction	sigaction_data[NSIG];
#endif
#ifndef	AIX_CORE_FILE
    cma_t_natural       syscall_state; /* set if one of the cma wrapped syscalls was interrupted. */
#endif	/* ! AIX_CORE_FILE */
    cma_t_boolean	detached;	/* Set if already detached */
    cma_t_boolean	terminated;	/* Set if terminated */
    cma_t_integer	joiners;	/* Count of joiners, for zombie frees */
    cma__t_int_alert	alert;		/* Current alert state info */
    struct CMA__T_INT_CV 
			*wait_cv;	/* CV thread is currently waiting on */
    struct CMA__T_INT_MUTEX 
			*wait_mutex;	/* Mutex thread is waiting on */
    struct EXC_CONTEXT_T
			*exc_stack;	/* Top of exception stack */
#ifdef	AIX_CORE_FILE
    cma_t_natural       syscall_state; /* set if one of the cma wrapped syscalls was interrupted. */
    cma_t_natural	_ibm_core_pad[3]; /* pad to make tcb correct size */
#endif	/* AIX_CORE_FILE */
#if _CMA_PLATFORM_ == _CMA__IBMR2_UNIX
    char		ctx_stack[cma__c_ibmr2_ctx_stack_size];
#endif
    cma_t_integer	thd_errno;	/* Per-thread errno value */
#ifdef _HP_LIBC_R
    cma_t_integer	thd_h_errno;	/* Per-thread h_errno value */
    struct rpc_createerr
			thd_rpcerr;	/* Per-thread rpc_createerr value */
#endif
#ifdef HP_CMA_SINGLESTACK_OPT
	cma___t_sigwait_block   wblk;
#endif
#if _CMA_OS_ == _CMA__VMS
    cma_t_integer	thd_vmserrno;	/* Per-thread VMS errno value */
#endif
#ifdef _HP_CMA_ASYNC_CLOSE_
    /*
     * These fields are used only by cma_select to keep track of the file objects
     * that are being selected on before and after waiting.  These fields should
     * appear in the cma__t_tcb_select structure. But placing them there would 
     * shift other fields in the tcb and blow up compatibility with DDE. Note
     * also that select_save_fo should be type (cma__t_fo_id **).  This couldn't
     * be accomplished due to cyclic dependencies in the include files. 
     */
    cma_t_address       select_save_fo; /* Per-thread array of pointers to cma__t_fo_id */
    cma_t_integer       select_save_nfds;  /* Per-thread number of pointers in array */
#endif
    } cma__t_int_tcb;

#ifdef __cplusplus
#undef	class
#endif

#ifdef __cplusplus
}
#endif

#endif

/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_TCB_DEFS.H */
/*  *21   17-APR-1992 11:11:44 BUTENHOF "Remove volatile on exc_stack" */
/*  *20   18-MAR-1992 19:01:47 SCALES "Parameterize scheduling policies" */
/*  *19   13-DEC-1991 10:12:45 BUTENHOF "Fix exc symbols" */
/*  *18   13-DEC-1991 09:53:49 BUTENHOF "Align nesting count in alert structure" */
/*  *17   27-NOV-1991 09:25:11 BUTENHOF "Add field for VMS errno on VAX C & DEC C" */
/*  *16   20-NOV-1991 14:45:36 CURTIN "Added a volatile" */
/*  *15   14-OCT-1991 13:41:00 BUTENHOF "Refine/fix use of config symbols" */
/*  *14   17-SEP-1991 13:21:30 BUTENHOF "Handle variable # of FDs more efficiently" */
/*  *13    2-JUL-1991 16:52:53 BUTENHOF "Add & use start func/arg fields in TCB" */
/*  *12   10-JUN-1991 19:57:11 SCALES "Convert to stream format for ULTRIX build" */
/*  *11   10-JUN-1991 19:21:59 BUTENHOF "Fix the sccs headers" */
/*  *10   10-JUN-1991 18:24:15 SCALES "Add sccs headers for Ultrix" */
/*  *9    10-JUN-1991 17:55:33 SCALES "Conditionalize inclusion of I/O stuff" */
/*  *8     5-JUN-1991 18:57:21 CURTIN "removed exc_handling.h include: axe circularity" */
/*  *7     5-JUN-1991 17:31:52 BUTENHOF "Integrate IBM changes" */
/*  *6     1-APR-1991 18:09:33 BUTENHOF "Integrate exception changes" */
/*  *5    28-MAR-1991 17:22:35 SCALES "Improve variable priority dispatch" */
/*  *4    12-FEB-1991 01:29:35 BUTENHOF "Redefine alert state" */
/*  *3     6-FEB-1991 01:33:22 BUTENHOF "Change errno field name" */
/*  *2    17-DEC-1990 14:35:16 CURTIN "added sigaction stuff" */
/*  *1    12-DEC-1990 21:53:54 BUTENHOF "Thread bookkeeping" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_TCB_DEFS.H */
