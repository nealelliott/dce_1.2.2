/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 *
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems
 * Burlington, MA, USA
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: cma.h $Revision: 1.1.16.1 $ $Date: 1996/05/14 19:19:30 $
 */
/*
 * HISTORY
 * $Log: cma.h,v $
 * Revision 1.1.16.1  1996/05/14  19:19:30  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 * 	[1996/05/14  18:32:45  arvind]
 *
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * Revision 1.1.9.2  1994/06/09  13:37:13  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:27:14  devsrc]
 * 
 * Revision 1.1.9.1  1994/04/01  20:15:17  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:15:30  jd]
 * 
 * Revision 1.1.5.6  1993/02/01  21:47:03  hinman
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  17:25:01  hinman]
 * 
 * 	[hinman] - Check in merged SNI version
 * 	[1993/01/11  16:18:33  hinman]
 * 
 * Revision 9.5.1.9  92/12/17  13:34:02  raj
 * 	<<<replace with log message for ./threads/cma.h>>>
 * 
 * Revision 9.5.2.2  92/11/06  10:58:59  raj
 * 	[raj] Changes for new naming scheme with reentrant libraries
 * 
 * Revision 9.5.1.8  92/12/09  17:09:28  root
 * 	Auto checkin of OSF rev 1.1.5.4
 * 
 * Revision 1.1.5.4  1992/12/07  13:21:14  zeliff
 * 	Removed duplicate OSF copyright markers
 * 	[1992/12/06  00:45:12  zeliff]
 * 
 * Revision 1.1.5.3  1992/09/29  20:14:44  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:40:36  sekhar]
 * 
 * Revision 1.1.5.2  1992/09/03  14:34:23  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:01:18  bolinger]
 * 
 * Revision 1.1.5.2  1992/08/28  20:44:53  bolinger
 * 	Move latest 1.0.1-based revision into 1.0.2.
 * 	[1992/08/28  20:41:12  bolinger]
 * 
 * Revision 1.1.3.2  1992/08/28  14:02:02  bolinger
 * 	Restore previous (dce1.0.1) state of interface to libc memory
 * 	allocation system.
 * 
 * Revision 1.1.2.2  1992/05/08  22:34:58  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  22:28:32  bolinger]
 * 
 * Revision 1.1.3.2  1992/05/15  15:26:04  bolinger
 * 	Fix OT defect 3225:  changes from Brian Keane to cma_t_handle type,
 * 	to distinguish handle class from handle sequence number.  This
 * 	permits a portable implementation of functions that return the
 * 	sequence number.
 * 	[1992/05/15  14:04:24  bolinger]
 * 
 * Revision 1.1  1992/01/19  14:41:19  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *	%W%	(DEC OSF/1)	%G%
 */
/*
 *  Copyright (c) 1988, 1992 by
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
 *	Digital's Proprietary Interface to DECthreads (CMA)
 *
 *  ABSTRACT:
 *
 *	External definitions for CMA services
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 * 
 *	2 November 1988
 *
 *  MODIFIED BY:
 *
 *	Dave Butenhof
 *	Bob Conti
 *	Paul Curtin
 *	Hans Oser
 *	Webb Scales
 */

#ifndef CMA_INCLUDE
#define CMA_INCLUDE

/*
 * Define a symbol which client code can test to determine whether the 
 * underlying threads package is DECthreads or not.  This is especially
 * important to clients of the Pthreads interface who may want to use
 * certain DECthreads extensions, such as the global lock and non-real-time
 * scheduling policies, on the DECthreads platform while maintaining 
 * portability to a "vanilla" pthreads implementation.
 */
#define _DECTHREADS_	1

/*
 * The implementation makes these basic decisions
 */

#if defined(vms) || defined(__vms) || defined (VMS) || defined(__VMS) || defined(__vms__)
# include <cma_config.h>		/* configuration symbols */
#else
# include <dce/cma_config.h>		/* configuration symbols */
#endif

/* For HP_UX, specifies which version of the operating system we are building on */
#if _CMA_VENDOR_ == _CMA__HP
# include <dce/hpdce_platform.h>
#endif

/* Fix ordering problem with "wait" definition in HP-UX 1010 header files */
#if defined(__hpux1010p)
#  define wait cma_wait
#endif

#if (_CMA_UNIX_TYPE ==  _CMA__SVR4) && (_CMA_REENTRANT_CLIB_)
# if defined _libcma_code_
# include <cma_libc_redefs.h>
# endif
#endif

#if _CMA_OS_ != _CMA__VMS
# include <sys/types.h>
# include <sys/time.h>
#endif

#if _CMA_OS_ == _CMA__VMS
# include <exc_handling.h>		/* Bring in exception services */
#else
# include <dce/exc_handling.h>		/* Bring in exception services */
#endif

#if (_CMA_OSIMPL_ == _CMA__OS_OSF) && _CMA_REENTRANT_CLIB_
# define _REENTRANT	1		/* Enable reentrant errno.h */
#endif

#if (_CMA_VENDOR_ == _CMA__HP) && _CMA_REENTRANT_CLIB_
# ifndef _REENTRANT
#  define _REENTRANT    1
# endif
#endif



#ifdef __cplusplus
extern "C" {
#endif

/*
 * Sample decisions for the environment types
 */


typedef int			cma_t_integer;

typedef unsigned int		cma_t_boolean;

typedef unsigned int		cma_t_natural;

typedef float			cma_t_interval;

typedef	int			cma_t_key;

typedef int			cma_t_status;

typedef	int			cma_t_priority;

#if _CMA_VOID_
typedef void			*cma_t_address;
#else
typedef char			*cma_t_address;
#endif

#define cma_c_false	(cma_t_boolean)0
#define cma_c_true	(cma_t_boolean)1
#define cma_c_null_ptr	(cma_t_address)0

#define cma_c_prio_fifo_min	16
#define cma_c_prio_fifo_mid	24
#define cma_c_prio_fifo_max	31
#define cma_c_prio_rr_min	16
#define cma_c_prio_rr_mid	24
#define cma_c_prio_rr_max	31
#define cma_c_prio_through_min	8
#define cma_c_prio_through_mid	12
#define cma_c_prio_through_max	15
#define cma_c_prio_back_min	1
#define cma_c_prio_back_mid	4
#define cma_c_prio_back_max	7
#define cma_c_prio_ada_low_min	0
#define cma_c_prio_ada_low_mid	4
#define cma_c_prio_ada_low_max	7

/*
 * The implementation of the cma_t_date_time type should match the "native
 * time" of the platform: that allows clients to use the full set of platform
 * time operations, rather than just "cma_get_expiration", to compute and
 * test timed waits.
 *
 * This section assumes the platform is either "VMS" or "UNIX-ish".  Others
 * will require changes.
 */
#if _CMA_OS_ == _CMA__VMS
typedef struct CMA_T_DATE_TIME {
    unsigned long int	low;
    unsigned long int	high;
    }				cma_t_date_time;
#else
typedef struct timeval		cma_t_date_time;
#endif

#ifdef __hpux
/*
 * Structure for C++ callbacks
 */
typedef struct CB_STRUCT {
	void (*start)();
	void (*end)();
	void (*out)();
	void (*in)();
} _ctxcb_spec_t;
#endif

/* 
 * Sample decisions for what handles shall be 
 */

typedef struct CMA_T_HANDLE {
    cma_t_address	field1;
    short int		field2;
    short int		field3;
    } cma_t_handle;

#define cma_thread_get_unique(handle) \
    ((unsigned int)((cma_t_thread *)handle)->field2)


#define cma_c_handle_size sizeof(cma_t_handle)

typedef	cma_t_handle	cma_t_mutex;	/* Needed for CMA_ONCE_BLOCK */

/*
 * Predefined null handle
 */
#ifndef _CMA_SUPPRESS_EXTERNALS_
_CMA_IMPORT_ cma_t_handle	cma_c_null;
#endif

/*
 * Sample decision for a one-time initialization control block and its
 * initialization macro.
 *
 * Declare a one time initialization control block as:
 *
 *	static cma_t_once	block = cma_once_init;
 */
typedef struct CMA_T_ONCE {
    cma_t_integer	field1;
    cma_t_integer	field2;
    cma_t_integer	field3;
    }				cma_t_once;

#define cma_once_init	{0, 0, 0}

/* 
 * Sample decision for a variable to save the current alert state.
 */
typedef struct CMA_T_ALERT_STATE {
    cma_t_integer	state1;
    cma_t_integer	state2;
    }				cma_t_alert_state;

/*
 * The following are the portable CMA definitions
 */

/*
 * Operations on Handles
 */

/*
 * The value of a handle can be assigned the value of another handle.	
 */

extern void cma_handle_assign (cma_t_handle *, cma_t_handle *);

/*
 * The value of a handle can be compared to the value of another handle.
 */

extern cma_t_boolean cma_handle_equal (	cma_t_handle *, cma_t_handle *);

/*
 * Operations on attributes objects
 */

typedef cma_t_handle	cma_t_attr;

/*
 * An attributes object is created to specify the attributes of other CMA
 * objects that will be created.
 */

extern void cma_attr_create (cma_t_attr	*,cma_t_attr *);

/*
 * An attributes object can be deleted when it is no longer needed.
 */

extern void cma_attr_delete (cma_t_attr	*);

/*
 * Operations on threads
 */

typedef cma_t_handle	cma_t_thread;
typedef cma_t_address	(*cma_t_start_routine) (cma_t_address);

typedef enum CMA_T_EXIT_STATUS {
    cma_c_term_error		= 0,
    cma_c_term_normal		= 1,
    cma_c_term_alert		= 2
    }				cma_t_exit_status;

typedef enum CMA_T_SCHED_INHERIT {
    cma_c_sched_inherit = 0,
    cma_c_sched_use_default = 1
    }				cma_t_sched_inherit;

typedef enum CMA_T_SCHED_POLICY {
    cma_c_sched_fifo = 0,
    cma_c_sched_rr = 1,
    cma_c_sched_throughput = 2,
    cma_c_sched_background = 3,
    cma_c_sched_ada_low = 4
    }				cma_t_sched_policy;

#define cma_c_sched_default	cma_c_sched_throughput
#define cma_c_sched_other	cma_c_sched_default

/*
 * The following procedures can be used to control thread creation,
 * termination and deletion.
 */

/*
 * To create a thread object and runnable thread, a routine must be specified
 * as the new thread's start routine.  An argument may be passed to this
 * routine, as an untyped address; an untyped address may also be returned as
 * the routine's value.  An attributes object may be used to specify details
 * about the kind of thread being created.
 */
extern void cma_thread_create (cma_t_thread *,cma_t_attr *,cma_t_start_routine,cma_t_address);

/*
 * A thread object may be "detached" to specify that the return value and
 * completion status will not be requested.
 */
extern void cma_thread_detach (cma_t_thread *);

/* 
 * A thread may terminate it's own execution.
 */
extern void cma_thread_exit_error (void);

extern void cma_thread_exit_normal (cma_t_address);

/* 
 * A thread can await termination of another thread and retrieve the return
 * value and completion status of the thread.
 */
extern void cma_thread_join (cma_t_thread *,cma_t_exit_status *,cma_t_address *);

/*
 * Operations to define thread creation attributes
 */

/*
 * Set or obtain the default thread priority.
 */
extern void cma_attr_set_priority (cma_t_attr *,cma_t_priority);

extern void cma_attr_get_priority (cma_t_attr *,cma_t_priority *);

/*
 * Set or obtain the default scheduling algorithm
 */
extern void cma_attr_set_sched (cma_t_attr *,cma_t_sched_policy	,cma_t_priority);

extern void cma_attr_get_sched (cma_t_attr *,cma_t_sched_policy *);

/*
 * Set or obtain whether a thread will use the default scheduling attributes,
 * or inherit them from the creating thread.
 */
extern void cma_attr_set_inherit_sched (cma_t_attr *,cma_t_sched_inherit);

extern void cma_attr_get_inherit_sched (cma_t_attr *,cma_t_sched_inherit *);

/*
 * Set or obtain the default stack size
 */
extern void cma_attr_set_stacksize (cma_t_attr *,cma_t_natural);

extern void cma_attr_get_stacksize (cma_t_attr *,cma_t_natural *);

/*
 * Set or obtain the default guard size
 */
extern void cma_attr_set_guardsize (cma_t_attr *,cma_t_natural);

extern void cma_attr_get_guardsize (cma_t_attr *,cma_t_natural *);

/*
 * Thread Scheduling Operations
 */

/*
 * The current user_assigned priority of a thread can be changed.
 */
extern void cma_thread_set_priority (cma_t_thread *,cma_t_priority);

/*
 * The current user_assigned scheduler algorithm of a thread can be changed.
 */
extern void cma_thread_set_sched (cma_t_thread *,cma_t_sched_policy,cma_t_priority);

/*
 * A thread may tell the scheduler that its processor can be made available.
 */
extern void cma_yield (void);

#ifndef _HP_CMA_PERF_
extern void cma_yield_noselect (void);
#endif /* _HP_CMA_PERF_ */

/*
 * A thread may enter a wait state for a speciifed period of time.
 */
extern void cma_delay (cma_t_interval);

/*
 * Thread Information Operations
 */

/*
 * A thread may obtain a copy of its own thread handle.
 */
extern void cma_thread_get_self (cma_t_thread *);

/*
 * The current user_assigned priority of a thread can be read.
 */
extern void cma_thread_get_priority (cma_t_thread *,cma_t_priority *);

/*
 * The current user_assigned scheduler algorithm of a thread can be read.
 */
extern void cma_thread_get_sched (cma_t_thread *,cma_t_sched_policy *);

/*
 * Operations on Mutexes
 */

typedef enum CMA_T_MUTEX_KIND {
    cma_c_mutex_fast = 0,
    cma_c_mutex_recursive = 1,
    cma_c_mutex_nonrecursive = 2
    }				cma_t_mutex_kind;

/*
 * Operations to define mutex creation attributes
 */

/*
 * Set or obtain whether mutex locks can nest.
 */
extern void cma_attr_set_mutex_kind (cma_t_attr *,cma_t_mutex_kind);

extern void cma_attr_get_mutex_kind (cma_t_attr*,cma_t_mutex_kind *);

/* 
 * The following routines create, delete, lock and unlock mutexes.
 */
extern void cma_mutex_create  (cma_t_mutex *,cma_t_attr *);

extern void cma_mutex_delete  (cma_t_mutex *);

extern void cma_mutex_lock  (cma_t_mutex *);

extern cma_t_boolean cma_mutex_try_lock  (cma_t_mutex *);

extern void cma_mutex_unlock  (cma_t_mutex *);

extern void cma_lock_global  (void);

extern void cma_unlock_global  (void);

/*
 * Operations on condition variables
 */

typedef cma_t_handle	cma_t_cond;

/*
 * A thread can create and delete condition variables.
 */
extern void cma_cond_create  (cma_t_cond *,cma_t_attr *);

extern void cma_cond_delete  (cma_t_cond *);

/*
 * A thread can signal to and broadcast on a condition variable.
 */
extern void cma_cond_broadcast  (cma_t_cond *);

extern void cma_cond_signal  (cma_t_cond *);

extern void cma_cond_signal_int  (cma_t_cond *);

/*
 * A thread can wait for a condition variable to be signalled or broadcast.
 */
extern void cma_cond_wait  (cma_t_cond *,cma_t_mutex *);

/*
 * Operations for timed waiting
 */

/*
 * A thread can perform a timed wait on a condition variable.
 */
extern cma_t_status cma_cond_timed_wait  (cma_t_cond *,cma_t_mutex *,cma_t_date_time *);

/*
 * A thread may perform some operations on absolute date-time and intervals.
 */

extern void cma_time_get_expiration  (cma_t_date_time *,cma_t_interval );

/*
 * Operations for CMA and client initialization.
 */

/*
 * Initialize the CMA facility.
 */
extern void
cma_init  (void);

/*
 * A thread can declare a one-time initialization routine.  The address of
 * the init block and routine are passed as parameters.
 */
typedef void	(*cma_t_init_routine)  (cma_t_address);

extern void 
cma_once  (cma_t_once *_block,cma_t_init_routine,cma_t_address);

/*
 * Operations for per-thread context
 */
typedef void	(*cma_t_destructor)  (cma_t_address);

/*
 * A unique per-thread context key can be obtained for the process
 */
extern void cma_key_create  (cma_t_key *,cma_t_attr *,cma_t_destructor);

/*
 * A thread can set a per-thread context value identified by a key.
 */
extern void cma_key_set_context  (cma_t_key,cma_t_address);

/*
 * A thread can retrieve a per-thread context value identified by a key.
 */
extern void cma_key_get_context  (cma_t_key,cma_t_address *_value);

/*
 * Operations for alerts.
 */

/*
 * The current thread can request that a thread terminate it's execution.
 */

extern void cma_thread_alert  (cma_t_thread *);

/*
 * The current thread can poll for alert delivery.
 */
extern void cma_alert_test  (void);

/*
 * The current thread can disable asynchronous alert delivery, restore the
 * previous state of asynchronous alert delivery, or enable asynchronous
 * alert delivery.
 */
extern void cma_alert_disable_asynch  (cma_t_alert_state *);

extern void cma_alert_disable_general  (cma_t_alert_state *);

extern void cma_alert_enable_asynch  (void);

extern void cma_alert_enable_general  (cma_t_alert_state *);

extern void cma_alert_restore  (cma_t_alert_state *);

/*
 * Operations on stacks
 */

typedef	cma_t_handle	cma_t_stack_np;
typedef void	(*cma_t_call_routine)  (void);

/*
 * Allocate stack space
 */
extern void cma_stack_allocate_np  (cma_t_integer,cma_t_address *);

/*
 * Assign a stack to a thread
 */
extern void cma_stack_assign_np  (cma_t_stack_np *,cma_t_thread *);

/*
 * Call a routine on a new stack
 */
extern void cma_stack_call_routine_np  (cma_t_stack_np *,cma_t_call_routine,cma_t_address,cma_t_address *);

/*
 * Check stack limit
 */
extern cma_t_boolean cma_stack_check_limit_np  (cma_t_integer);

/*
 * Create a new stack
 */
extern void cma_stack_create_np  (cma_t_stack_np *,cma_t_attr *);

/*
 * Delete a stack
 */
extern void cma_stack_delete_np  (cma_t_stack_np *);

/*
 * Debug threads
 */
extern void
cma_debug  (void);

extern void cma_debug_cmd  (char *, ...);

#ifndef _CMA_SUPPRESS_EXTERNALS_
_CMA_IMPORT_ void (*cma_g_debug) (void);
#endif

#ifdef __cplusplus
}
#endif

# if defined(_HP_LIBC_R) && defined(__hpux1010p)
#  define sigaction _sigaction_sys
# endif

# if _CMA_OS_ == _CMA__VMS
#  include <cma_px.h>
# else
#  include <dce/cma_px.h>
# endif

#if _CMA_UNIPROCESSOR_
# if _CMA_OS_ == _CMA__VMS
#  include <cmalib_crtlx.h>
# else
#  include <dce/cmalib_crtlx.h>
# endif
#endif

# ifndef _CMA_NOWRAPPERS_
#  if !_CMA_REENTRANT_CLIB_
#   if _CMA_OS_ == _CMA__VMS
#    include <cmalib_crtlx.h>
#   else
#    include <dce/cma_errno.h>
#    include <dce/cmalib_crtlx.h>
#    include <dce/cma_stdio.h>
#   endif
#  elif (_CMA_UNIX_TYPE == _CMA__SVR4)
	/*
	*	SVR4 reentrant libraries do not have per-thread errno.
	*/
#   include <dce/cma_errno.h>
#  endif
#  if _CMA_OS_ == _CMA__UNIX
#   include <dce/cma_ux.h>
#  endif
#  if _CMA_OS_ == _CMA__VMS
#   include <cma_sigwait.h>
#  else
#   include <dce/cma_sigwait.h>
#  endif
# endif

# ifdef _HP_LIBC_R
#  undef wait
#  include <cma_libc_calls.h>
# endif


#endif
