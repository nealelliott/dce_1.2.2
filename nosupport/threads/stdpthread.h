/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: stdpthread.h,v $
 * Revision 1.1.2.2  1996/12/05  08:53:58  rsalz
 * 	Final version
 * 	[1996/12/05  08:53:38  rsalz]
 *
 * Revision 1.1.2.1  1996/11/25  17:21:09  rsalz
 * 	Created.
 * 	[1996/11/25  17:00:54  rsalz]
 * 
 * $EndLog$
 */

#ifndef PTHREAD
#define PTHREAD

#ifdef __cplusplus
    extern "C" {
#endif

#include <dce/cma.h>
#include <signal.h>

/*
 * The implementation makes these basic decisions
 */

typedef	cma_t_key		pthread_key_t;
typedef	cma_t_mutex		pthread_mutex_t;
typedef cma_t_attr		pthread_attr_t;
typedef cma_t_attr		pthread_condattr_t;
typedef cma_t_attr		pthread_mutexattr_t;
typedef cma_t_cond		pthread_cond_t;
typedef cma_t_once		pthread_once_t;
typedef cma_t_thread		pthread_t;

#define PTHREAD_ONCE_INIT	cma_once_init

    /* Not supported, but here. */
#define PTHREAD_INHERIT_SCHED	((int)cma_c_sched_inherit)
#define PTHREAD_DEFAULT_SCHED	((int)cma_c_sched_use_default)
#define SCHED_FIFO		((int)cma_c_sched_fifo)
#define SCHED_RR		((int)cma_c_sched_rr)
#define SCHED_OTHER		((int)cma_c_sched_throughput)
#define PRI_FIFO_MIN		((int)cma_c_prio_fifo_min)
#define PRI_FIFO_MAX		((int)cma_c_prio_fifo_max)
#define PRI_RR_MIN		((int)cma_c_prio_rr_min)
#define PRI_RR_MAX		((int)cma_c_prio_rr_max)
#define PRI_OTHER_MIN		((int)cma_c_prio_through_min)
#define PRI_OTHER_MAX		((int)cma_c_prio_through_max)

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
typedef void (*__cleanup_t)(void*);
#define pthread_cleanup_push(routine,arg)	\
    { \
    __cleanup_t _XXX_proc = (__cleanup_t)(routine); \
    void* _XXX_arg = (arg); \
    int _XXX_completed = 0; \
    TRY {
#define pthread_cleanup_pop(execute)	\
    _XXX_completed = 1;} \
    FINALLY { \
	int _XXX_execute = execute; \
	if ((! _XXX_completed) || (_XXX_execute)) _XXX_proc (_XXX_arg);} \
    ENDTRY}

/*
 * Thread equality.
 */
#define pthread_equal(thread1,thread2) \
    (((thread1).field1 == (thread2).field1) \
    && ((thread1).field2 == (thread2).field2) \
    && ((thread1).field3 == (thread2).field3))


/*
 * An attributes object can be deleted when it is no longer needed.
 */
int pthread_attr_init(pthread_attr_t *);
int pthread_attr_destroy(pthread_attr_t *);

/*
 * Operations to define thread creation attributes
 */
#define pthread_attr_setinheritsched xpthread_attr_setinheritsched
int pthread_attr_setinheritsched(pthread_attr_t *,int);
#define pthread_attr_getinheritsched xpthread_attr_getinheritsched
int pthread_attr_getinheritsched(pthread_attr_t, int*);
#define pthread_attr_setstacksize xpthread_attr_setstacksize
int pthread_attr_setstacksize(pthread_attr_t *,long);
#define pthread_attr_getstacksize xpthread_attr_getstacksize
int pthread_attr_getstacksize(pthread_attr_t, long*);

/*
 * The following procedures can be used to control thread creation,
 * termination and deletion.
 */
#define pthread_create xpthread_create
int pthread_create(pthread_t *,pthread_attr_t*,void (*)(void*),void*);
#define pthread_detach xpthread_detach
int pthread_detach(pthread_t);
void pthread_exit(void*);
#define pthread_join xpthread_join
int pthread_join(pthread_t,void**);

/*
 * Thread Scheduling Operations
 */
#define sched_yield pthread_yield
void pthread_yield(void);
pthread_t pthread_self(void);

/*
 * Operations on Mutexes
 */
int pthread_mutexattr_init(pthread_mutexattr_t *);
int pthread_mutexattr_destroy(pthread_mutexattr_t *);
#define pthread_mutex_init xpthread_mutex_init
int pthread_mutex_init(pthread_mutex_t *,pthread_mutexattr_t*);
#define pthread_mutex_destroy xpthread_mutex_destroy
int pthread_mutex_destroy(pthread_mutex_t *);
#define pthread_mutex_lock xpthread_mutex_lock
int pthread_mutex_lock(pthread_mutex_t *);
#define pthread_mutex_trylock xpthread_mutex_trylock
int pthread_mutex_trylock(pthread_mutex_t *);
#define pthread_mutex_unlock xpthread_mutex_unlock
int pthread_mutex_unlock(pthread_mutex_t *);

/*
 * Operations on condition variables
 */
int pthread_condattr_init(pthread_condattr_t *);
int pthread_condattr_destroy(pthread_condattr_t *);
#define pthread_cond_init xpthread_cond_init
int pthread_cond_init(pthread_cond_t *,pthread_condattr_t);
#define pthread_cond_destroy xpthread_cond_destroy
int pthread_cond_destroy(pthread_cond_t *);
#define pthread_cond_broadcast xpthread_cond_broadcast
int pthread_cond_broadcast(pthread_cond_t *);
#define pthread_cond_signal xpthread_cond_signal
int pthread_cond_signal(pthread_cond_t *);
#define pthread_cond_wait xpthread_cond_wait
int pthread_cond_wait(pthread_cond_t *,pthread_mutex_t *);
#define pthread_cond_timedwait xpthread_cond_timedwait
int pthread_cond_timedwait(pthread_cond_t *,pthread_mutex_t *,struct timespec *);

/*
 * Operations for client initialization.
 */
#define pthread_once xpthread_once
int pthread_once(pthread_once_t *,void (*)(void));

/*
 * Operations for per-thread context
 */
#define pthread_keycreate xpthread_keycreate
int pthread_keycreate(pthread_key_t *,void (*)(void));
#define pthread_setspecific xpthread_setspecific
int pthread_setspecific(pthread_key_t,void*);
#define pthread_getspecific xpthread_getspecific
void *pthread_getspecific(pthread_key_t);

/*
 * Cancellation.
 */
#define CANCEL_ON	1
#define CANCEL_OFF	0
#define PTHREAD_CANCEL_ENABLE		CANCEL_ON
#define PTHREAD_CANCEL_DISABLE		CANCEL_OFF
#define PTHREAD_CANCEL_ASYNCHRONOUS	CANCEL_ON
#define PTHREAD_CANCEL_DEFERRED		CANCEL_OFF
#define pthread_cancel xpthread_cancel
int pthread_cancel(pthread_t);
void pthread_testcancel(void);
int pthread_setcancelstate(int, int*);
int pthread_setcanceltype(int, int*);

extern pthread_attr_t		pthread_attr_default;
extern pthread_mutexattr_t	pthread_mutexattr_default;
extern pthread_condattr_t	pthread_condattr_default;

#ifdef __cplusplus
    }
#endif

#endif
