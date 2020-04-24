/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: pthread.h,v $
 * Revision 1.1.65.2  1996/02/18  23:46:46  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:45:25  marty]
 *
 * Revision 1.1.65.1  1995/12/08  00:15:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:56:28  root]
 * 
 * Revision 1.1.63.1  1994/01/21  22:32:23  cbrooks
 * 	RPC Code Cleanup - Initial Submission
 * 	[1994/01/21  20:57:35  cbrooks]
 * 
 * Revision 1.1.5.3  1993/01/03  22:36:46  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:53:18  bbelch]
 * 
 * Revision 1.1.5.2  1992/12/23  19:40:02  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  14:54:03  zeliff]
 * 
 * Revision 1.1.3.4  1992/05/26  16:03:10  garyf
 * 	redo previous change
 * 	[1992/05/26  16:01:15  garyf]
 * 
 * Revision 1.1.3.3  1992/05/22  17:32:18  garyf
 * 	include <sys/utctime.h> for OSF/1
 * 	Revision 1.1.3.2  1992/05/01  17:56:17  rsalz
 * 	"Changed as part of rpc6 drop."
 * 	[1992/05/01  17:50:58  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:16:06  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**
**  NAME:
**
**      pthread.h
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  An implementation of the various pthread data types, functions, etc that
**  the runtime and stubs use.  This particular implementation is for
**  a OSF1 Operating System Component environment.  Depending on the
**  exact implementation, it may be useable for a "generic" UNIX kernel.
**
**  WARNING: this is NOT a generically useable implementation of pthreads for
**  a kernel environment.  This implementation supports the limited set
**  of operations that the runtime and stubs use.  It currently only works on
**  single CPU non-preemptive environments.
**
*/
#ifndef _PTHREAD_H
#define _PTHREAD_H 1

#include <dce/dce.h>

#if defined (_AIX)
#include <sys/types.h>
#include <sys/lockl.h>
#endif

#ifdef NOTDEF
/* enable these once we care about every cycle */
#define PTHREAD_MUTEX_MACROS_ENABLE 1
#define PTHREAD_COND_MACROS_ENABLE  1
#endif

#define _PTHREAD_NO_CANCEL_SUPPORT 1

typedef	int			pthread_key_t;

typedef void			*pthread_addr_t;

typedef int		        pthread_once_t;

#define pthread_once_init	0

#define CANCEL_ON       1
#define CANCEL_OFF	0


typedef pthread_addr_t	        pthread_attr_t;


/*
 * ===== Package INTERNAL Mutex and Condition Variable stuff =====
 */

/*
 * This sleep lock implementation operates *only*
 * in a single CPU non-preemptive thread environment.
 */

#define PTHREAD_MUTEX_SLEEP_PRI             ( PZERO - 2 )
#define PTHREAD_MUTEX_LOCKED                0x01
#define PTHREAD_MUTEX_WANTED                0x02

#define PTHREAD_MUTEX_LOCK_INTERNAL(mutex) \
{ \
    while (((mutex) & PTHREAD_MUTEX_LOCKED) != 0) \
    { \
        (mutex) |= PTHREAD_MUTEX_WANTED; \
        sleep(&(mutex), PTHREAD_MUTEX_SLEEP_PRI); \
    } \
    (mutex) |= PTHREAD_MUTEX_LOCKED; \
}

#define PTHREAD_MUTEX_TRY_LOCK_INTERNAL(mutex) \
( \
    ((((mutex) & PTHREAD_MUTEX_LOCKED) != 0) \
        ? 0 \
        : (((mutex) |= PTHREAD_MUTEX_LOCKED), \
          !0 )) \
)

#define PTHREAD_MUTEX_UNLOCK_INTERNAL(mutex) \
{ \
    boolean wanted = ((mutex) & PTHREAD_MUTEX_WANTED) != 0; \
    \
    (mutex) &= ~(PTHREAD_MUTEX_LOCKED | PTHREAD_MUTEX_WANTED); \
    if (wanted) \
        wakeup(&(mutex)); \
}


/*
 * Even in non-multicpu, non-preemptive environments, timed condition
 * waits have to cope with interaction with a timeout so spl's are
 * necessary.
 */

#  define PTHREAD_COND_WAITING              0x01
#  define PTHREAD_COND_TIMEOUT              0x02

#  define PTHREAD_COND_SIGNAL_INTERNAL(cond) \
{ \
    int s = splhigh(); \
    if (((cond) & PTHREAD_COND_WAITING) != 0) \
    { \
        (cond) &= ~PTHREAD_COND_WAITING; \
        wakeup(&(cond)); \
    } \
    splx(s); \
}


/*
 * ===== Operations on threads =====
 */

typedef pthread_addr_t	        pthread_t;
typedef pthread_addr_t		(*pthread_startroutine_t) (); /* ??? */

extern int
pthread_create _DCE_PROTOTYPE_ ((
	pthread_t		* /* thread */,
	pthread_attr_t		/* attr */,
	pthread_startroutine_t	/* start_routine */,
	pthread_addr_t		/* arg */));


extern int
pthread_detach _DCE_PROTOTYPE_ ((pthread_t * /* thread */));

extern int
pthread_join _DCE_PROTOTYPE_ ((	pthread_t /*thread*/, pthread_addr_t * /*status*/));

extern void
pthread_yield _DCE_PROTOTYPE_ ((void));

extern pthread_t
pthread_self _DCE_PROTOTYPE_ ((void));

extern int
pthread_attr_create _DCE_PROTOTYPE_ (( pthread_attr_t * /* attr */));

extern int
pthread_attr_delete _DCE_PROTOTYPE_ (( pthread_attr_t * /* attr */));


/*
 * ===== Operations on thread attributes =====
 */


#if defined(PTHREAD_NO_TIMESPEC) || defined(__OSF__)

struct timespec {
    unsigned long tv_sec;
    unsigned long tv_nsec;
};
#endif

/*
 * ===== Operations on Mutexes =====
 */

typedef pthread_addr_t	pthread_mutexattr_t;

#if ! (defined (_AIX) && defined (_KERNEL))
typedef	unsigned char	pthread_mutex_t;
#else
typedef	lock_t		pthread_mutex_t;
#endif /* ! _AIX && _KERNEL */

extern int
pthread_mutex_init _DCE_PROTOTYPE_ ((
	pthread_mutex_t * /* mutex */,
	pthread_mutexattr_t	/* attr */
     ));

extern int
pthread_mutex_destroy _DCE_PROTOTYPE_ ((pthread_mutex_t * /* mutex */));

#ifndef PTHREAD_MUTEX_MACROS_ENABLE
    extern int
    pthread_mutex_lock _DCE_PROTOTYPE_ ((pthread_mutex_t * /* mutex */));

    extern int
    pthread_mutex_trylock _DCE_PROTOTYPE_ (( pthread_mutex_t * /* mutex */));

    extern int
    pthread_mutex_unlock _DCE_PROTOTYPE_ ((pthread_mutex_t * /* mutex */));
#else

#define pthread_mutex_lock(mutex)       PTHREAD_MUTEX_LOCK_INTERNAL(*mutex)
#define pthread_mutex_try_lock(mutex)   PTHREAD_MUTEX_TRY_LOCK_INTERNAL(*mutex)
#define pthread_mutex_unlock(mutex)     PTHREAD_MUTEX_UNLOCK_INTERNAL(*mutex)

#endif /* PTHREAD_MUTEX_MACROS_ENABLE */


/*
 * ===== Operations on condition variables =====
 */

typedef pthread_addr_t	pthread_condattr_t;

#if ! (defined (_AIX) && (_KERNEL))
typedef unsigned char	pthread_cond_t;
#else
typedef struct {
        int queue;        /* head of queue of processes waiting */
        unsigned long var; /* condition variable */
        } pthread_cond_t;
#endif /* ! _AIX && _KERNEL */

extern int
pthread_cond_init _DCE_PROTOTYPE_ (( 
	pthread_cond_t * /* cond */,	
	pthread_condattr_t	/* attr */));

extern int
pthread_cond_destroy _DCE_PROTOTYPE_ ((	pthread_cond_t * /* cond */));

extern int
pthread_cond_wait _DCE_PROTOTYPE_ ((
	pthread_cond_t		* /* cond */,
	pthread_mutex_t		* /* mutex */));

#ifndef PTHREAD_COND_MACROS_ENABLE
    extern int
    pthread_cond_signal _DCE_PROTOTYPE_ (( pthread_cond_t * /* cond */));
#else

#define pthread_cond_signal(cond)       PTHREAD_COND_SIGNAL_INTERNAL(*cond)

#endif /* PTHREAD_COND_MACROS_ENABLE */

extern int
pthread_cond_broadcast _DCE_PROTOTYPE_ (( pthread_cond_t * /* cond */));


/*
 * ===== Operations for client initialization. =====
 */

typedef void	(*pthread_initroutine_t) (); /* ??? */

extern int
pthread_once _DCE_PROTOTYPE_ ((
	pthread_once_t		* /* once_block*/,
	pthread_initroutine_t	/* init_routine */
));

/*
 * ===== Operations for per-thread context =====
 */

typedef void (*pthread_destructor_t) _DCE_PROTOTYPE_((pthread_addr_t));

extern int
pthread_keycreate _DCE_PROTOTYPE_ ((
	pthread_key_t		* /* key */,
	pthread_destructor_t	/* destructor */
));

extern int
pthread_setspecific _DCE_PROTOTYPE_ ((
	pthread_key_t	/* key */,
	pthread_addr_t	/* value */));

extern int
pthread_getspecific _DCE_PROTOTYPE_ ((
	pthread_key_t	/* key */,
	pthread_addr_t	* /* value */));


/*
 * ===== Operations for alerts. =====
 */

extern int
pthread_cancel _DCE_PROTOTYPE_ ((pthread_t /* thread */));


extern int
pthread_setasynccancel _DCE_PROTOTYPE_ ((int /* mode */));

extern int
pthread_setcancel _DCE_PROTOTYPE_ ((int	/* mode */)); 

extern void
pthread_testcancel _DCE_PROTOTYPE_ ((void));

extern pthread_attr_t		pthread_attr_default;
extern pthread_mutexattr_t	pthread_mutexattr_default;
extern pthread_condattr_t	pthread_condattr_default;


#define pthread_equal(thread1,thread2) \
    ((thread1) == (thread2))

/*
 * ===== *_np stuff
 */

extern int
pthread_get_expiration_np _DCE_PROTOTYPE_ (( struct timespec *, struct timespec	*));

extern int
pthread_delay_np _DCE_PROTOTYPE_ (( struct timespec *));

#endif /* _DCE_H */
