/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: pthread.h,v $
 * Revision 1.2.13.1  1996/05/10  13:45:53  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/2  1996/03/08  21:48 UTC  bissen
 * 	Merge to DCE1.2
 *
 * 	HP revision /main/DCE_1.2/1  1996/01/03  18:49 UTC  psn
 * 	Merge second XIDL drop for DCE 1.2.1
 * 	[1995/11/20  22:54 UTC  dat  /main/dat_xidl2/1]
 *
 * Revision 1.2.9.2  1994/06/09  13:39:20  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:29:07  devsrc]
 * 
 * Revision 1.2.9.1  1994/04/01  20:18:08  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:19:53  jd]
 * 
 * Revision 1.2.5.4  1993/02/01  22:32:55  hinman
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  17:34:37  hinman]
 * 
 * 	[hinman] - Check in merged SNI version
 * 	[1993/01/11  16:24:28  hinman]
 * 
 * Revision 9.5.1.6  92/12/17  14:15:09  raj
 * 	[raj] New naming + parallelized reentrant library changes.
 * 
 * Revision 9.5.3.2  92/11/06  11:31:29  raj
 * 	[raj] Changes for new naming scheme with reentrant libraries.
 * 
 * Revision 9.5.1.5  92/09/10  16:38:21  hinman
 * 	[hinman] - Fold forward from raj_om
 * 
 * Revision 9.5.2.2  92/09/10  16:14:29  raj
 * 	[raj] merging SVR4 changes into 1.0.2
 * 
 * Revision 9.9  92/09/02  14:13:29  hinman
 * 	Update checkin
 * 
 * Revision 1.1.2.3  1992/08/14  20:24:04  bolinger
 * 	Bring forward 1.0.1 revision 1.2.3.2.
 * 	[1992/08/10  16:43:15  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/09  00:22:02  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/09  00:21:48  bolinger]
 * 
 * $EndLog$
 */
/*
 *      %W%     (DEC OSF/1)    %G%
 */
/*
 *  Copyright (c) 1990, 1992 by
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
 *	Common Multithread Architecture (CMA) services; POSIX 1003.4 interface
 *
 *  ABSTRACT:
 *
 *	External definitions for CMA's pthreads services
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 * 
 *	20 February 1990
 *
 *  MODIFIED BY:
 *
 *	Dave Butenhof
 *	Paul Curtin
 *	Webb Scales
 */

#ifndef PTHREAD
#define PTHREAD

#ifdef __cplusplus
    extern "C" {
#endif

#ifdef vms
# include <cma.h>
#else
# include <dce/cma.h>
# include <signal.h>
#endif

/*
 * The implementation makes these basic decisions
 */

#ifndef _POSIX_THREADS
# define _POSIX_THREADS
#endif
#ifndef _POSIX_THREAD_ATTR_STACKSIZE
# define _POSIX_THREAD_ATTR_STACKSIZE
#endif
#if _CMA_KTHREADS_ != _CMA__MACH
# define _POSIX_THREADS_REALTIME_SCHEDULING	1
#elif defined (_POSIX_THREADS_REALTIME_SCHEDULING)
# undef _POSIX_THREADS_REALTIME_SCHEDULING
#endif
#ifndef _POSIX_THREADS_PER_PROCESS_SIGNALS_1
# define _POSIX_THREADS_PER_PROCESS_SIGNALS_1	1
#endif

/*
 * Implement push and pop for cancellation handlers, using TRY and ENDTRY
 */

#define pthread_cleanup_push(routine,arg)	\
    { \
    pthread_cleanup_t _XXX_proc = (pthread_cleanup_t)(routine); \
    pthread_addr_t _XXX_arg = (arg); \
    int _XXX_completed = 0; \
    TRY {

#define pthread_cleanup_pop(execute)	\
    _XXX_completed = 1;} \
    FINALLY { \
	int _XXX_execute = execute; \
	if ((! _XXX_completed) || (_XXX_execute)) _XXX_proc (_XXX_arg);} \
    ENDTRY}

#define pthread_equal_np(thread1,thread2) \
    (((thread1).field1 == (thread2).field1) \
    && ((thread1).field2 == (thread2).field2) \
    && ((thread1).field3 == (thread2).field3))

#define pthread_equal(thread1,thread2) \
    (((thread1).field1 == (thread2).field1) \
    && ((thread1).field2 == (thread2).field2) \
    && ((thread1).field3 == (thread2).field3))

#define pthread_getunique_np(handle) \
    ((unsigned int)((pthread_t *)handle)->field2)

/*
 * Sample decisions for the environment types
 */

typedef	cma_t_key		pthread_key_t;

typedef cma_t_address		pthread_addr_t;

/*
 * For compatibility with OSF/1 pthreads
 */
typedef pthread_addr_t		any_t;
typedef void (*pthread_cleanup_t)  (pthread_addr_t);

/*
 * Sample decision for a one-time initialization control block and its
 * initialization macro.
 *
 * Declare a one time initialization control block as:
 *
 *	static pthread_once_t	block = pthread_once_init;
 */
typedef cma_t_once	pthread_once_t;

#define pthread_once_init	cma_once_init

#define CANCEL_ON	1
#define CANCEL_OFF	0

/*
 * The following are the portable pthread definitions
 */

/*
 * Operations on Handles
 */

/*
 * Operations on attributes objects
 */

typedef cma_t_attr	pthread_attr_t;

/*
 * An attributes object is created to specify the attributes of other CMA
 * objects that will be created.
 */

int pthread_attr_create  (pthread_attr_t *);

/*
 * An attributes object can be deleted when it is no longer needed.
 */

int pthread_attr_delete  (pthread_attr_t *);

/*
 * Operations on threads
 */

typedef cma_t_thread		pthread_t;
typedef cma_t_start_routine	pthread_startroutine_t;
/*
 * For compatibility with OSF/1 pthreads
 */
typedef pthread_startroutine_t	pthread_func_t;

#define PTHREAD_INHERIT_SCHED	((int)cma_c_sched_inherit)
#define PTHREAD_DEFAULT_SCHED	((int)cma_c_sched_use_default)

#define SCHED_FIFO		((int)cma_c_sched_fifo)
#define SCHED_RR		((int)cma_c_sched_rr)
#define SCHED_FG_NP		((int)cma_c_sched_throughput)
#define SCHED_BG_NP		((int)cma_c_sched_background)
#define SCHED_OTHER		((int)cma_c_sched_throughput)

#define PRI_FIFO_MIN		((int)cma_c_prio_fifo_min)
#define PRI_FIFO_MAX		((int)cma_c_prio_fifo_max)
#define PRI_RR_MIN		((int)cma_c_prio_rr_min)
#define PRI_RR_MAX		((int)cma_c_prio_rr_max)
#define PRI_FG_MIN_NP		((int)cma_c_prio_through_min)
#define PRI_FG_MAX_NP		((int)cma_c_prio_through_max)
#define PRI_BG_MIN_NP		((int)cma_c_prio_back_min)
#define PRI_BG_MAX_NP		((int)cma_c_prio_back_max)
#define PRI_OTHER_MIN		((int)cma_c_prio_through_min)
#define PRI_OTHER_MAX		((int)cma_c_prio_through_max)

/*
 * Operations to define thread creation attributes
 */

/*
 * Set or obtain the default thread priority.
 */
int pthread_attr_setprio  (pthread_attr_t *,int);

int pthread_attr_getprio  (pthread_attr_t);

/*
 * Set or obtain the default scheduling algorithm
 */
int pthread_attr_setsched  (pthread_attr_t	*,int);

int pthread_attr_getsched  (pthread_attr_t);

/*
 * Set or obtain whether a thread will use the default scheduling attributes,
 * or inherit them from the creating thread.
 */
int pthread_attr_setinheritsched  (pthread_attr_t *,int);

int pthread_attr_getinheritsched  (pthread_attr_t);

/*
 * Set or obtain the default stack size
 */
int pthread_attr_setstacksize  (pthread_attr_t *,long);

long pthread_attr_getstacksize  (pthread_attr_t);

/*
 * Set or obtain the default guard size
 */
int pthread_attr_setguardsize_np  (pthread_attr_t *,long);

long pthread_attr_getguardsize_np  (pthread_attr_t);

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
int pthread_create  (pthread_t *,pthread_attr_t,pthread_startroutine_t,pthread_addr_t);

/*
 * A thread object may be "detached" to specify that the return value and
 * completion status will not be requested.
 */
int pthread_detach  (pthread_t *);

/* 
 * A thread may terminate it's own execution.
 */
void pthread_exit  (pthread_addr_t);

/* 
 * A thread can await termination of another thread and retrieve the return
 * value of the thread.
 */
int pthread_join  (pthread_t,pthread_addr_t *);

/*
 * Thread Scheduling Operations
 */

/*
 * The current user_assigned priority of a thread can be changed.
 */
int pthread_setprio  (pthread_t,int);

/*
 * The current user_assigned scheduler algorithm of a thread can be changed.
 */
int pthread_setscheduler  (pthread_t,int,int);

/*
 * A thread may tell the scheduler that its processor can be made available.
 */
void pthread_yield  (void);

/*
 * Thread Information Operations
 */

/*
 * A thread may obtain a copy of its own thread handle.
 */
pthread_t pthread_self  (void);

/*
 * The current user_assigned priority of a thread can be read.
 */
int pthread_getprio  (pthread_t);

/*
 * The current user_assigned scheduler algorithm of a thread can be read.
 */
int pthread_getscheduler  (	pthread_t);

/*
 * Operations on Mutexes
 */

#define MUTEX_FAST_NP		0
#define MUTEX_RECURSIVE_NP	1
#define MUTEX_NONRECURSIVE_NP	2

typedef cma_t_attr	pthread_mutexattr_t;
typedef	cma_t_mutex	pthread_mutex_t;

int pthread_mutexattr_create  (pthread_mutexattr_t *);

int pthread_mutexattr_delete  (pthread_mutexattr_t *);

int pthread_mutexattr_setkind_np  (pthread_mutexattr_t *,int);

int pthread_mutexattr_getkind_np  (pthread_mutexattr_t);

/* 
 * The following routines create, delete, lock and unlock mutexes.
 */
int pthread_mutex_init  (pthread_mutex_t *,pthread_mutexattr_t);

int pthread_mutex_destroy  (pthread_mutex_t *);

int pthread_mutex_lock  (pthread_mutex_t *);

int pthread_mutex_trylock  (pthread_mutex_t *);

int pthread_mutex_unlock  (pthread_mutex_t *);

/*
 * Operations on condition variables
 */

typedef cma_t_attr	pthread_condattr_t;
typedef cma_t_cond	pthread_cond_t;

int pthread_condattr_create  (pthread_condattr_t *);

int pthread_condattr_delete  (pthread_condattr_t *);

/*
 * A thread can create and delete condition variables.
 */
int pthread_cond_init  (pthread_cond_t *,pthread_condattr_t);

int pthread_cond_destroy  (pthread_cond_t *);

/*
 * A thread can signal to and broadcast on a condition variable.
 */
int pthread_cond_broadcast  (pthread_cond_t *);

int pthread_cond_signal  (pthread_cond_t *);

int pthread_cond_signal_int_np  (pthread_cond_t *);

/*
 * A thread can wait for a condition variable to be signalled or broadcast.
 */
int pthread_cond_wait  (pthread_cond_t *,pthread_mutex_t *);

/*
 * Operations for timed waiting
 */

/*
 * A thread can perform a timed wait on a condition variable.
 */
int pthread_cond_timedwait  (pthread_cond_t *,pthread_mutex_t *,struct timespec *);

/*
 * Operations for client initialization.
 */
 typedef void (*pthread_initroutine_t)  (void);

int pthread_once  (pthread_once_t *,pthread_initroutine_t);

/*
 * Operations for per-thread context
 */

typedef cma_t_destructor	pthread_destructor_t;

/*
 * A unique per-thread context key can be obtained for the process
 */
int pthread_keycreate  (pthread_key_t *,pthread_destructor_t);

/*
 * A thread can set a per-thread context value identified by a key.
 */
int pthread_setspecific  (pthread_key_t,pthread_addr_t);

/*
 * A thread can retrieve a per-thread context value identified by a key.
 */
int pthread_getspecific  (pthread_key_t,pthread_addr_t *);

/*
 * Operations for alerts.
 */

/*
 * The current thread can request that a thread terminate it's execution.
 */

int pthread_cancel  (pthread_t);

/*
 * The current thread can poll for alert delivery.
 */
void pthread_testcancel  (void);

/*
 * The current thread can enable or disable alert delivery (PTHREAD
 * "cancels"); it can control "general cancelability" (CMA "defer") or
 * just "asynchronous cancelability" (CMA "asynch disable").
 */
int pthread_setasynccancel  (int);

int pthread_setcancel  (int);

_CMA_IMPORT_ pthread_attr_t		pthread_attr_default;
_CMA_IMPORT_ pthread_mutexattr_t	pthread_mutexattr_default;
_CMA_IMPORT_ pthread_condattr_t		pthread_condattr_default;

/*
 * Define nonportable extensions
 */

extern int pthread_get_expiration_np  (struct timespec *,struct timespec	*);

extern int pthread_delay_np  (struct timespec *);

#if (_CMA_UNIX_TYPE == _CMA__SVR4)
extern unsigned int pthread_delay_unsigned_np  (unsigned int );
#endif /* (_CMA_UNIX_TYPE == _CMA__SVR4) */

extern void pthread_lock_global_np  (void);

extern void pthread_unlock_global_np  (void);

# if _CMA_OS_ != _CMA__VMS
extern int pthread_signal_to_cancel_np  (sigset_t *,pthread_t *);
# endif

#ifdef __cplusplus
    }
#endif

#endif
