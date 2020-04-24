/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * @HP_COPYRIGHT@
 * @HP_DCE_VERSION@
 * @(#)Module: pthread_int.h $Revision: 1.1.2.1 $ $Date: 1996/06/04 22:15:54 $
 */
/*
 * HISTORY
 * $Log: pthread_int.h,v $
 * Revision 1.1.2.1  1996/06/04  22:15:54  arvind
 * 	Merge pthread.h changes for WC BE.
 * 	[1995/07/11  17:27 UTC  jss  /main/HPDCE02/1]
 *
 * 	[1995/07/11  15:46 UTC  jss  /main/jss_mothra_cose/1]
 *
 * 	Initial Revision.
 *
 * 	[1995/06/27  22:36 UTC  jss  /main/HPDCE02/1]
 *
 * $EndLog$
 */
/*  This file is for internal HP use only. This contains the structures
 *  necessary to allow the Commands group to build the non-ANSI compliant
 *  libraries. 
 */

#ifndef PTHREAD_INT
#define PTHREAD_INT

#include <stdlib.h>
#include <dce/exc_handling.h>           /* DCE exception handling */
#include <dce/cma_config.h>		/* configuration symbols */

#if (_CMA_VENDOR_ == _CMA__HP) && _CMA_REENTRANT_CLIB_
# ifndef _REENTRANT
#  define _REENTRANT    1
# endif
#endif

#ifdef _CMA_PROTO_
# define _CMA_PROTOTYPE_(arg)	arg
#else
# define _CMA_PROTOTYPE_(arg)	()
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

/*
 * Structure for C++ callbacks
 */
typedef struct CB_STRUCT {
	void (*start)();
	void (*end)();
	void (*out)();
	void (*in)();
} _ctxcb_spec_t;

/* 
 * Sample decisions for what handles shall be 
 */

typedef struct CMA_T_HANDLE {
    cma_t_address	field1;
    short int		field2;
    short int		field3;
    } cma_t_handle;

typedef	cma_t_handle	cma_t_mutex;	/* Needed for CMA_ONCE_BLOCK */

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
 * Operations on attributes objects
 */

typedef cma_t_handle	cma_t_attr;

/*
 * Operations on threads
 */

typedef cma_t_handle	cma_t_thread;
typedef cma_t_address	(*cma_t_start_routine) ();

/* 
 * Sample decision for a variable to save the current alert state.
 */
typedef struct CMA_T_ALERT_STATE {
    cma_t_integer	state1;
    cma_t_integer	state2;
    }				cma_t_alert_state;

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
 * Operations on Mutexes
 */

typedef enum CMA_T_MUTEX_KIND {
    cma_c_mutex_fast = 0,
    cma_c_mutex_recursive = 1,
    cma_c_mutex_nonrecursive = 2
    }				cma_t_mutex_kind;

/*
 * Operations on condition variables
 */

typedef cma_t_handle	cma_t_cond;

/*
 * A thread can declare a one-time initialization routine.  The address of
 * the init block and routine are passed as parameters.
 */
typedef void	(*cma_t_init_routine)  ();

/*
 * Operations for per-thread context
 */
typedef void	(*cma_t_destructor)  ();

/*
 * Operations on stacks
 */

typedef	cma_t_handle	cma_t_stack_np;
typedef void	(*cma_t_call_routine)  ();

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
#if defined (_POSIX_THREADS_REALTIME_SCHEDULING)
# undef _POSIX_THREADS_REALTIME_SCHEDULING
#endif
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

/*
 * Sample decisions for the environment types
 */

typedef	cma_t_key		pthread_key_t;

typedef cma_t_address		pthread_addr_t;

/*
 * For compatibility with OSF/1 pthreads
 */
typedef pthread_addr_t		any_t;

typedef void (*pthread_cleanup_t) _CMA_PROTOTYPE_ ((pthread_addr_t arg));

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
 * Operations on Mutexes
 */

#define MUTEX_FAST_NP		0
#define MUTEX_RECURSIVE_NP	1
#define MUTEX_NONRECURSIVE_NP	2

typedef cma_t_attr	pthread_mutexattr_t;
typedef	cma_t_mutex	pthread_mutex_t;

/*
 * Operations on condition variables
 */

typedef cma_t_attr	pthread_condattr_t;
typedef cma_t_cond	pthread_cond_t;

extern pthread_attr_t		pthread_attr_default;
extern pthread_mutexattr_t	pthread_mutexattr_default;
extern pthread_condattr_t	pthread_condattr_default;

#ifdef __cplusplus
}
#endif

#endif /* PTHREAD_INT */
