/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: pthread_exc.h $Revision: 1.2.12.2 $ $Date: 1996/06/04 22:16:48 $
 */
/*
 * HISTORY
 * $Log: pthread_exc.h,v $
 * Revision 1.2.12.2  1996/06/04  22:16:48  arvind
 * 	Merge change to DCE_1.2
 * 	[1996/05/01  15:40 UTC  bissen  /main/DCE_1.2/2]
 *
 * 	Remove __hpux define from around pthread_is_multithreaded_np
 * 	[1996/05/01  15:34 UTC  bissen  /main/DCE_1.2/bissen_122_work/1]
 *
 * 	Update OSF copyright years
 * 	[1996/02/18  22:35:27  marty]
 *
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:08:16  root]
 *
 * Revision 1.2.12.1  1996/05/10  13:46:05  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 * 
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:48 UTC  bissen
 * 	Merge from branch jss_single_thread
 * 	[1995/01/03  22:36 UTC  jss  /main/HPDCE02/4]
 * 
 * 	Changes for single threaded client.  pthread_is_multithreaded_np()
 * 	was changed to be a function.
 * 	[1995/01/03  19:15 UTC  jss  /main/HPDCE02/jss_single_thread/2]
 * 
 * 	Added cma__g_init_done2.
 * 	[1994/12/22  21:28 UTC  tatsu_s  /main/HPDCE02/3]
 * 
 * 	Merge from branch jss_single_thread.
 * 	[1994/12/22  19:59 UTC  jss  /main/HPDCE02/2]
 * 
 * 	Changes for the single threaded client.  Added the routine
 * 	pthread_is_multithreaded_np().
 * 	[1994/12/15  15:42 UTC  jss  /main/HPDCE02/jss_single_thread/1]
 * 
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:46 UTC  jss  /main/HPDCE02/1]
 * 
 * 	merge kk and hpdce01
 * 	[1994/01/25  18:11  truitt  /main/HPDCE01/1]
 * 
 * 	Fix prototype for ptdexc_yield_noselect()
 * 	[1993/11/02  15:59:08  ajayb]
 * 	 *
 * 
 * 	Change polarity of ifdef for including cma performance fixes
 * 	[1993/10/29  18:55:34  ajayb]
 * 
 * 	Add proper ifdefs to include DEPT changes.
 * 	[1993/10/19  19:38:58  ajayb]
 * 
 * 	Merging DEPT changes
 * 	[1993/10/18  14:38:57  prepetit]
 * 
 * 	Merging DEPT changes
 * 	[1993/09/30  15:08:39  gaz]
 * 
 * 	merge kk and dce1_0_3
 * 	[1993/09/21  15:25:16  truitt]
 * 
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:45:13  root  1.1.11.3]
 * 
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:15:44  hopkins]
 * 
 * 	Initial King Kong branch
 * 	[1993/07/08  19:26:43  root]
 * 
 * 	Added revision control strings
 * 	[1993/06/04  18:36:49  hopkins]
 * 
 * 	Gamera merge
 * 	[1993/03/26  23:57:41  hopkins]
 * 
 * 	Loading drop DCE1_0_2B18
 * 	[1993/03/11  22:29:53  osfrcs  1.1.1.8]
 * 
 * 	DCE 1.0.1 Release
 * 	[1992/10/07  14:40:18  ana]
 * 
 * Revision 1.2.8.2  1994/06/09  13:39:22  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:29:08  devsrc]
 * 
 * Revision 1.2.8.1  1994/04/01  20:18:11  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:19:57  jd]
 * 
 * Revision 1.2.6.3  1992/12/15  22:33:35  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:45:08  alan]
 * 
 * Revision 1.2.6.2  1992/09/03  14:43:26  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:09:01  bolinger]
 * 
 * Revision 1.1.2.3  1992/08/14  20:24:08  bolinger
 * 	Bring forward 1.0.1 revision 1.2.4.2, revision 1.2.4.3 (fix for OT
 * 	defect 3225).
 * 	[1992/08/10  16:44:48  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/09  00:23:20  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/09  00:23:05  bolinger]
 * 
 * Revision 1.2.4.3  1992/05/15  15:28:31  bolinger
 * 	Fix OT defect 3225:  changes from Brian Keane to implement
 * 	pthread_getunique_np() portably (despite the _np suffix).
 * 	[1992/05/15  14:08:04  bolinger]
 * 
 * 	Fix OT defect 2994:  Change the prototypes of the entry points in
 * 	the exception-raising Pthreads interface to match those of their
 * 	error-returning kin.
 * 
 * 	The motivation behind this is to realize the claim in the App Dev Guide
 * 	that one can substitute either of these interfaces for the other just
 * 	by changing one #include line.
 * 	[1992/05/14  01:21:54  bolinger]
 * 
 * Revision 1.2.4.2  1992/05/13  22:21:53  keane
 * 	Removed typecasts from pthread_cleanup_push and pthread_cleanup_pop.
 * 	These macros now require that the arguments are properly typed.
 * 	[1992/05/11  17:26:01  keane]
 * 
 * Revision 1.2  1992/01/19  22:14:50  devrcs
 * 	Dropping DCE1.0 OSF1_misc port archive
 * 
 * $EndLog$
 */
/*
*/

/*
 *  OSF DCE Version 1.0 
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
 *	Common Multithread Architecture (CMA) services; POSIX 1003.4 interface
 *	
 *	Exception Generating Pthreads Interface (ptdexc)
 *
 *  ABSTRACT:
 *
 *	External definitions for CMA's pthreads exception (ptdexc) services
 *
 *  AUTHORS:
 *
 *	Paul Curtin
 *
 *  CREATION DATE:
 * 
 *	27 August 1990
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	10 October 1990
 *		Use _CMA_IMPORT_ config symbol for distinguished handles
 *	002	Paul Curtin	9 November 1990
 *		Removed _e from pthread_equal routines 
 *	003	Paul Curtin	20 November 1990
 *		Removed p from pthread in posix_pthread_attr_stacksize
 *		Added two new defines for; realtime scheduling, and signals.
 *	004	Paul Curtin	21 November 1990
 *		Swithed order of arguments to pthread_keycreate
 *	005	Paul Curtin	21 January 1991
 *		Changed *_illinst to *_illinstr to match cma.h
 *	006	Dave Butenhof	22 January 1991
 *		Fix exception names
 *	007	Paul Curtin	04 February 1991
 *		Adjusted a couple of names.
 *	008	Dave Butenhof	4 February 1991
 *		New function names
 *	009	Dave Butenhof	7 February 1991
 *		Fix name of delay function (for some reason, it was defined
 *		as "pthread_e_delay_e_np").
 *	010	Dave Butenhof	12 February 1991
 *		Change "friendly" mutex to "recursive", add "nonrecursive"
 *	011	Webb Scales	19 February 1991
 *		Add CMA scheduling symbols for Pthreads.
 *	012	Dave Butenhof	20 March 1991
 *		Remove _POSIX_PROCESS_SCHEDULING option, since we don't
 *		support it.
 *	013	Dave Butenhof	26 March 1991
 *		Move exception definitions to exc_handling.h
 *	014	Paul Curtin	24 April 1991
 *		Adding missing def for pthread_cond_wait
 *	015	Webb Scales	24 April 1991
 *		Corrected typedef for initroutine_t
 *	016	Dave Butenhof	29 May 1991
 *		Remove the realtime scheduling option under OSF/1 when
 *		using kernel threads, since it doesn't (yet) support POSIX
 *		scheduling for Mach threads.
 *	017	Dave Butenhof	05 June 1991
 *		Remove include of cma_px.h, which is already included by
 *		cma.h.
 *	018	Dave Butenhof	10 June 1991
 *		Create pthread_getunique_np
 *	019	Webb Scales	 2 July 1991
 *		Add typedefs for OSF/1 compatibility.
 *	020	Paul Curtin	22 August 1991
 *		Added define for pthread_get_expiration_np.
 *	021	Dave Butenhof	07 October 1991
 *		Add pthread_attr_setguardsize_np &
 *		pthread_attr_getguardsize_np functions.
 *	022	Dave Butenhof	29 October 1991
 *		OSF/1 unistd.h defines _POSIX_THREADS and
 *		_POSIX_THREAD_ATTR_STACKSIZE, and cc (with -std1) is
 *		complaining about the redefinition here. Don't redefine if
 *		they're already defined.
 *	023	Dave Butenhof	15 November 1991
 *		Add omitted prototype for pthread_signal_to_cancel_np().
 */

#ifndef PTHREAD_EXC
#define PTHREAD_EXC

#ifdef vms
# include <cma.h>
#else
# include <dce/cma.h>
# include <signal.h>
#endif

#ifdef __cplusplus
extern "C" {
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
#else
# if defined (_POSIX_THREADS_REALTIME_SCHEDULING)
# undef _POSIX_THREADS_REALTIME_SCHEDULING
# endif
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
	int _XXX_execute = (execute); \
	if ((! _XXX_completed) || _XXX_execute) _XXX_proc (_XXX_arg);} \
    ENDTRY}


/*
 *  Macros used to convert normal pthread routine calls to exception 
 *  returning routines.  This is done by including this file, pthread_exc.h,
 *  in the place of pthread.h .
 */


#define pthread_equal_np(thread1,thread2) \
    (((thread1).field1 == (thread2).field1) \
    && ((thread1).field2 == (thread2).field2) \
    && ((thread1).field3 == (thread2).field3))

#define pthread_equal(thread1,thread2) \
    (((thread1).field1 == (thread2).field1) \
    && ((thread1).field2 == (thread2).field2) \
    && ((thread1).field3 == (thread2).field3))

#define pthread_getunique_np(handle) \
    (((unsigned int)((pthread_t *)handle)->field2))

#define pthread_attr_create(attr) \
    ptdexc_attr_create(attr)

#define pthread_attr_delete(attr) \
    ptdexc_attr_delete(attr)

#define pthread_attr_setprio(attr,priority) \
    ptdexc_attr_setprio(attr,priority)

#define pthread_attr_getprio(attr) \
    ptdexc_attr_getprio(attr)

#define pthread_attr_setsched(attr,scheduler) \
    ptdexc_attr_setsched(attr,scheduler)

#define pthread_attr_getsched(attr) \
    ptdexc_attr_getsched(attr)

#define pthread_attr_setinheritsched(attr,inherit) \
    ptdexc_attr_setinheritsched(attr,inherit)

#define pthread_attr_getinheritsched(attr) \
    ptdexc_attr_getinheritsched(attr)

#define pthread_attr_setstacksize(attr,stacksize) \
    ptdexc_attr_setstacksize(attr,stacksize)

#define pthread_attr_getstacksize(attr) \
    ptdexc_attr_getstacksize(attr)

#define pthread_attr_setguardsize_np(attr,guardsize) \
    ptdexc_attr_setguardsize_np(attr,guardsize)

#define pthread_attr_getguardsize_np(attr) \
    ptdexc_attr_getguardsize_np(attr)

#define pthread_create(thread,attr,start_routine,arg) \
    ptdexc_create(thread,attr,start_routine,arg)

#define pthread_detach(thread) \
    ptdexc_detach(thread)

#define pthread_exit(status) \
    ptdexc_exit(status)

#define pthread_get_expiration_np(delta,abstime) \
    ptdexc_get_expiration_np(delta,abstime)

#define pthread_join(thread,status) \
    ptdexc_join(thread,status)

#define pthread_setprio(thread,priority) \
    ptdexc_setprio(thread,priority)

#define pthread_setscheduler(thread,scheduler,priority) \
    ptdexc_setscheduler(thread,scheduler,priority)

#define pthread_yield() \
    ptdexc_yield()

#ifndef _HP_CMA_PERF_
# define pthread_yield_noselect() \
    ptdexc_yield_noselect()
#endif /* _HP_CMA_PERF_ */

#define pthread_self() \
    ptdexc_self()

#define pthread_getprio(thread) \
    ptdexc_getprio(thread)

#define pthread_getscheduler(thread) \
    ptdexc_getscheduler(thread)

#define pthread_mutexattr_create(attr) \
    ptdexc_mutexattr_create(attr)

#define pthread_mutexattr_delete(attr) \
    ptdexc_mutexattr_delete(attr)

#define pthread_mutexattr_setkind_np(attr,kind) \
    ptdexc_mutexattr_setkind_np(attr,kind)

#define pthread_mutexattr_getkind_np(attr) \
    ptdexc_mutexattr_getkind_np(attr)

#define pthread_mutex_init(mutex,attr) \
    ptdexc_mutex_init(mutex,attr)

#define pthread_mutex_destroy(mutex) \
    ptdexc_mutex_destroy(mutex)

#define pthread_mutex_lock(mutex) \
    ptdexc_mutex_lock(mutex)

#define pthread_mutex_trylock(mutex) \
    ptdexc_mutex_trylock(mutex)

#define pthread_mutex_unlock(mutex) \
    ptdexc_mutex_unlock(mutex)

#define pthread_condattr_create(attr) \
    ptdexc_condattr_create(attr)

#define pthread_condattr_delete(attr) \
    ptdexc_condattr_delete(attr)

#define pthread_cond_init(cond,attr) \
    ptdexc_cond_init(cond,attr)

#define pthread_cond_destroy(cond) \
    ptdexc_cond_destroy(cond)

#define pthread_cond_broadcast(cond) \
    ptdexc_cond_broadcast(cond)

#define pthread_cond_signal(cond) \
    ptdexc_cond_signal(cond)

#define pthread_cond_signal_int_np(cond) \
    ptdexc_cond_signal_int_np(cond)

#define pthread_cond_wait(cond,mutex) \
    ptdexc_cond_wait(cond,mutex)

#define pthread_cond_timedwait(cond,mutex,abstime) \
    ptdexc_cond_timedwait(cond,mutex,abstime)

#define pthread_once(once_block,init_routine) \
    ptdexc_once(once_block,init_routine)

#define pthread_keycreate(key,destructor) \
    ptdexc_keycreate(key,destructor)

#define pthread_setspecific(key,value) \
    ptdexc_setspecific(key,value)

#define pthread_getspecific(key,value) \
    ptdexc_getspecific(key,value)

#define pthread_mutexattr_setkind(attr,kind) \
    ptdexc_mutexattr_setkind_np(attr,kind)

#define pthread_mutexattr_getkind(attr) \
    ptdexc_mutexattr_getkind_np(attr)

#define pthread_cancel(thread) \
    ptdexc_cancel(thread)

#define pthread_testcancel() \
    ptdexc_testcancel()

#define pthread_setasynccancel(state) \
    ptdexc_setasynccancel(state)

#define pthread_setcancel(state) \
    ptdexc_setcancel(state)

#define pthread_delay_np(interval) \
    ptdexc_delay_np(interval)

#define pthread_lock_global_np() \
    ptdexc_lock_global_np()

#define pthread_unlock_global_np() \
    ptdexc_unlock_global_np()

#define pthread_signal_to_cancel_np(sigset,thread) \
    ptdexc_signal_to_cancel_np(sigset,thread)

/*
 * Sample decisions for the environment types
 */

typedef	cma_t_key		pthread_key_t;

typedef cma_t_address		pthread_addr_t;
/*
 * For compatibility with OSF/1 pthreads
 */
typedef pthread_addr_t		any_t;

typedef void (*pthread_cleanup_t) (pthread_addr_t arg);

/*
 * Sample decision for a one-time initialization control block and its
 * initialization macro.
 *
 * Declare a one time initialization control block as:
 *pp
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

int ptdexc_attr_create  (pthread_attr_t *);

/*
 * An attributes object can be deleted when it is no longer needed.
 */

int ptdexc_attr_delete  (pthread_attr_t *);

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
int ptdexc_attr_setprio  (pthread_attr_t *,int);

int ptdexc_attr_getprio  (pthread_attr_t);

/*
 * Set or obtain the default scheduling algorithm
 */
int ptdexc_attr_setsched  (pthread_attr_t *,int);

int ptdexc_attr_getsched  (pthread_attr_t);

/*
 * Set or obtain whether a thread will use the default scheduling attributes,
 * or inherit them from the creating thread.
 */
int ptdexc_attr_setinheritsched  (pthread_attr_t *,int);

int ptdexc_attr_getinheritsched  (pthread_attr_t);

/*
 * Set or obtain the default stack size
 */
int ptdexc_attr_setstacksize  (pthread_attr_t *,long);

long ptdexc_attr_getstacksize  (pthread_attr_t);

/*
 * Set or obtain the default guard size
 */
int ptdexc_attr_setguardsize_np  (pthread_attr_t *,long);

long ptdexc_attr_getguardsize_np  (pthread_attr_t);

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
int ptdexc_create  (pthread_t *,pthread_attr_t,pthread_startroutine_t,pthread_addr_t);

/*
 * A thread object may be "detached" to specify that the return value and
 * completion status will not be requested.
 */
int ptdexc_detach  (pthread_t *);

/* 
 * A thread may terminate it's own execution.
 */
void ptdexc_exit  (pthread_addr_t);

/* 
 * A thread can await termination of another thread and retrieve the return
 * value of the thread.
 */
int ptdexc_join  (pthread_t,pthread_addr_t *);

/*
 * Thread Scheduling Operations
 */

/*
 * The current user_assigned priority of a thread can be changed.
 */
int ptdexc_setprio  (pthread_t,int);

/*
 * The current user_assigned scheduler algorithm of a thread can be changed.
 */
int ptdexc_setscheduler  (pthread_t,int,int);

/*
 * A thread may tell the scheduler that its processor can be made available.
 */
void ptdexc_yield  (void);

#ifndef _HP_CMA_PERF_
void ptdexc_yield_noselect (void);
#endif /* _HP_CMA_PERF_ */
/*
 * Thread Information Operations
 */

/*
 * A thread may obtain a copy of its own thread handle.
 */
pthread_t ptdexc_self  (void);

/*
 * The current user_assigned priority of a thread can be read.
 */
int ptdexc_getprio  (pthread_t);

/*
 * The current user_assigned scheduler algorithm of a thread can be read.
 */
int ptdexc_getscheduler  (pthread_t);

/*
 * Operations on Mutexes
 */

#define MUTEX_FAST_NP		0
#define MUTEX_RECURSIVE_NP	1
#define MUTEX_NONRECURSIVE_NP	2

typedef cma_t_attr	pthread_mutexattr_t;
typedef	cma_t_mutex	pthread_mutex_t;

int ptdexc_mutexattr_create  (pthread_mutexattr_t *);

int ptdexc_mutexattr_delete  (pthread_mutexattr_t *);

int ptdexc_mutexattr_setkind_np  (pthread_mutexattr_t *,int);

int ptdexc_mutexattr_getkind_np  (pthread_mutexattr_t);

/* 
 * The following routines create, delete, lock and unlock mutexes.
 */
int ptdexc_mutex_init  (pthread_mutex_t *,pthread_mutexattr_t);

int ptdexc_mutex_destroy  (pthread_mutex_t	*);

int ptdexc_mutex_lock  (pthread_mutex_t *);

int ptdexc_mutex_trylock  (pthread_mutex_t *);

int ptdexc_mutex_unlock  (pthread_mutex_t	*);

/*
 * Operations on condition variables
 */

typedef cma_t_attr	pthread_condattr_t;
typedef cma_t_cond	pthread_cond_t;

int ptdexc_condattr_create  (pthread_condattr_t *);

int ptdexc_condattr_delete  (pthread_condattr_t *);

/*
 * A thread can create and delete condition variables.
 */
int ptdexc_cond_init  (pthread_cond_t *,pthread_condattr_t);

int ptdexc_cond_destroy  (pthread_cond_t *);

/*
 * A thread can signal to and broadcast on a condition variable.
 */
int ptdexc_cond_broadcast  (pthread_cond_t *);

int ptdexc_cond_signal  (pthread_cond_t *);

int ptdexc_cond_signal_int_np  (pthread_cond_t *);

/*
 * A thread can wait for a condition variable to be signalled or broadcast.
 */
int ptdexc_cond_wait  (pthread_cond_t *,pthread_mutex_t *);

/*
 * Operations for timed waiting
 */

/*
 * A thread can perform a timed wait on a condition variable.
 */
int ptdexc_cond_timedwait  (pthread_cond_t *,pthread_mutex_t *,struct timespec *);

/*
 * Operations for client initialization.
 */
 typedef void (*pthread_initroutine_t)  (void);

int ptdexc_once (pthread_once_t *,pthread_initroutine_t);

/*
 * Operations for per-thread context
 */

typedef cma_t_destructor	pthread_destructor_t;

/*
 * A unique per-thread context key can be obtained for the process
 */
int ptdexc_keycreate  (pthread_key_t *,pthread_destructor_t);

/*
 * A thread can set a per-thread context value identified by a key.
 */
int ptdexc_setspecific  (pthread_key_t,pthread_addr_t);

/*
 * A thread can retrieve a per-thread context value identified by a key.
 */
int ptdexc_getspecific  (pthread_key_t,pthread_addr_t *);

/*
 * Operations for alerts.
 */

/*
 * The current thread can request that a thread terminate it's execution.
 */

int ptdexc_cancel  (pthread_t);

/*
 * The current thread can poll for alert delivery.
 */
void ptdexc_testcancel  (void);

/*
 * The current thread can enable or disable alert delivery (PTHREAD
 * "cancels"); it can control "general cancelability" (CMA "defer") or
 * just "asynchronous cancelability" (CMA "asynch disable").
 */
int ptdexc_setasynccancel  (int);

int ptdexc_setcancel  (int);

_CMA_IMPORT_ pthread_attr_t		pthread_attr_default;
_CMA_IMPORT_ pthread_mutexattr_t	pthread_mutexattr_default;
_CMA_IMPORT_ pthread_condattr_t		pthread_condattr_default;

/*
 * Define nonportable extensions
 */

extern int ptdexc_get_expiration_np  (struct timespec *,struct timespec *);

extern int ptdexc_delay_np  (struct timespec *);

extern void ptdexc_lock_global_np  (void);

extern void ptdexc_unlock_global_np  (void);

# if _CMA_OS_ != _CMA__VMS
extern void ptdexc_signal_to_cancel_np  (sigset_t *,pthread_t *);
# endif

extern int pthread_is_multithreaded_np (void);

#ifdef __cplusplus
}
#endif

#endif
/*  DEC/CMS REPLACEMENT HISTORY, Element PTHREAD_EXC.H */
/*  *26   18-NOV-1991 14:39:01 BUTENHOF "Fix an oops" */
/*  *25   18-NOV-1991 11:22:59 BUTENHOF "Add prototype for signal_to_cancel_np" */
/*  *24   31-OCT-1991 12:40:54 BUTENHOF "Fix errors where OSF/1 has already defined POSIX feature macros" */
/*  *23   14-OCT-1991 13:43:06 BUTENHOF "Fix header file locations" */
/*  *22   22-AUG-1991 13:11:05 CURTIN "adding a definition for pthread_get_expiration_np" */
/*  *21    2-JUL-1991 21:02:08 SCALES "Add typedefs for OSF/1 compatibility" */
/*  *20   10-JUN-1991 19:59:27 SCALES "Convert to stream format for ULTRIX build" */
/*  *19   10-JUN-1991 19:22:48 BUTENHOF "Fix the sccs headers" */
/*  *18   10-JUN-1991 18:25:49 SCALES "Add sccs headers for Ultrix" */
/*  *17   10-JUN-1991 14:13:08 BUTENHOF "Add macro to extract thread sequence #" */
/*  *16    5-JUN-1991 17:32:23 BUTENHOF "On UNIX platforms, most headers are in dce/" */
/*  *15   29-MAY-1991 17:02:30 BUTENHOF "Currently on OSF/1 based system, no priority" */
/*  *14   24-APR-1991 18:43:52 SCALES "Correct definition of pthread_initroutine_t" */
/*  *13   24-APR-1991 12:21:53 CURTIN "added a missing definition" */
/*  *12    1-APR-1991 18:10:37 BUTENHOF "Fix up exceptions" */
/*  *11   21-MAR-1991 09:27:04 BUTENHOF "" */
/*  *10   19-FEB-1991 22:11:40 SCALES "Add scheduling symbols" */
/*  *9    13-FEB-1991 17:55:01 BUTENHOF "Change mutex attribute name symbols" */
/*  *8    12-FEB-1991 23:10:06 BUTENHOF "Recursive/nonrecursive mutexes" */
/*  *7    12-FEB-1991 01:29:47 BUTENHOF "Fix name of delay function" */
/*  *6     5-FEB-1991 01:00:13 BUTENHOF "Change pthread exc interface names" */
/*  *5     4-FEB-1991 13:56:15 CURTIN "Changed a couple of names" */
/*  *4    24-JAN-1991 00:35:44 BUTENHOF "Exception changes" */
/*  *3    21-JAN-1991 13:26:59 CURTIN "changed *_illinst to *_illinstr" */
/*  *2    28-DEC-1990 00:04:55 BUTENHOF "Change exception names" */
/*  *1    12-DEC-1990 21:48:47 BUTENHOF "P1003.4a support" */
/*  DEC/CMS REPLACEMENT HISTORY, Element PTHREAD_EXC.H */
