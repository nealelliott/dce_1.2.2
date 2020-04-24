/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rwl.c,v $
 * Revision 1.1.8.2  1996/02/18  00:20:34  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:08:34  marty]
 *
 * Revision 1.1.8.1  1995/12/08  17:57:35  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/06/29  22:34 UTC  rps
 * 	function return cleanup
 * 
 * 	HP revision /main/rps_moop/1  1995/06/29  03:49 UTC  rps
 * 	function return cleanup
 * 	[1995/12/08  17:21:49  root]
 * 
 * Revision 1.1.6.3  1992/12/29  16:37:56  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:27:25  zeliff]
 * 
 * Revision 1.1.6.2  1992/09/29  20:13:21  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:38:32  sekhar]
 * 
 * Revision 1.1.3.2  1992/06/24  22:04:26  sommerfeld
 * 	CR4212: remove bogus parameter passed to vsem macro.
 * 	[1992/06/24  20:45:36  sommerfeld]
 * 
 * Revision 1.1  1992/01/19  14:44:03  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rwl.c V=9 10/18/91 //littl/prgy/src/server/rs
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
/*
 *      read/write lock manager
 * 
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)rwl.c	9 - 10/18/91";
#endif

#include <stdio.h>
#include <errno.h>
#include <un_maloc.h>
#include <un_strng.h>
#include <bstring.h>
#include <pthread.h>
#include <macros.h>
#include <rwl.h>

PUBLIC  boolean32 rwl_lock_debug = false;

PRIVATE rwl_module_initialized = false;
PRIVATE rwl_module_init_done = false;

extern int errno;  /* needed for pthread error returns, which we convert to 
		    * exceptions (after the cma pthread interlude has converted 
                    * from cma exceptions to pthread errnos - sigh)
		    */


typedef struct rwl_mutex_t {
    char               *info;  /* debugging info */
    boolean32          locked;
    pthread_mutex_t    mutex;
} rwl_mutex_t;

#ifndef SNI_SVR4
/****  SNI_SEC_MERGE: Check to see if the following code is not required
                      because pthread_equal_np is already defined to be
                      the equivalent of pthread_equal
****/

#ifdef pthread_equal
#define pthread_equal_np pthread_equal
#endif
#endif

/* primitive semaphore operations */
#define P(mutex) rwl_mutex_lock((&mutex))

#define V(mutex)  rwl_mutex_unlock((&mutex))


/* semaphore with exception handling - useful only if the semaphore is acquired
 * and freed within the same lexical scope (the VSEM macro takes no parameter to 
 * emphasize this contraint).  Can be nested within other exception
 * handling scopes, but may not "straddle" any other such scope.
 */
#define PSEM(mutex) { \
            rwl_mutex_t  this_mutex; \
            this_mutex = mutex; \
            P(this_mutex); \
            TRY {
		
#define VSEM \
	    } CATCH_ALL { \
		V(this_mutex); \
                RERAISE; \
	    } \
            ENDTRY \
            V(this_mutex); \
	} 

	        
rwl_mutex_t  rwl_kernel_mutex;  /* a guard around the entire lock kernel.
				 * must be created at lock module initialization
                                 * time.
                                 */

/* kernel entries and exits must be paired within a single scope */
#define ENTER_LOCK_KERNEL PSEM(rwl_kernel_mutex)
#define EXIT_LOCK_KERNEL  VSEM

rwl_mutex_t  rwl_lock_id_mutex; /* for serializing calls to gen_lock_id() */

/*
 * PRIVATE types
 */

typedef enum rwl_lock_state {

    rwl_lock_none,   /* no thread holds any lock of any kind */
    
    rwl_lock_read,   /* an arbitrary number of threads are holding  read
		      * locks, but no thread holds a read-intend-write or
		      * write lock.  An arbitrary number of threads may
		      * be blocked waiting for a write lock.
		      */
    
    rwl_lock_riw,    /* an arbitrary number of threads may be holding a read
		      * lock, and one thread holds a read-intend-write lock
		      * (the holder of the riw lock is identified by the 
		      * "thread" field of the lock info).  An arbitrary 
		      * number of threads may be blocked waiting for a
		      * read-intend-write or write lock.
		      */ 
    
    rwl_lock_write,  /* A single thread holds a write lock (the holder of the 
		      * write lock is identified by the  "thread" field of the
		      * lock info).  An arbitrary number of threads may be blocked
		      * waiting for any kind of lock.
		      */

    rwl_lock_deleted  /* A lock has been successfully deleted (at the time
		       * the deleting thread grabbed the mutex, the lock was 
                       * not held by any thread).  As the lock_info storage
                       * may still be referenced by other threads, we use the 
                       * the deleted state to cause all such references
                       * to raise an existence exception.  
		       */

    } rwl_lock_state_t;


/* 
 * Private lock info data 
 */
typedef struct rwl_lock_info {
    unsigned long    magic;     /* corruption guard */
    unsigned long    lock_id;   /* must jive with handle lock_id */

 /* synchronization primitives */
    rwl_mutex_t      mutex;
    pthread_cond_t   cond;

/* lock invariants */
    int              num_readers;
    rwl_lock_state_t state;

    boolean32        riw_owner_set; /* initialized to false */ 
    pthread_t        riw_owner;     /* id of thread currently holding the riw
                                     * lock.
                                     */
    boolean32        owner_set; /* initialized to false */ 
    pthread_t        owner;     /* id of thread currently holding the write
                                 * lock.
                                 */

    char             *debug_info; /* holds a debugging string supplied by
                                   * rwl package users via the rwl_lock_create
				   * call
				   */
} rwl_lock_info_t;


/* valid lock "magic" */
#define RWL_HANDLE_MAGIC    0x87654321  
#define RWL_LOCK_INFO_MAGIC 0x87654321  
#define RWL_MAGIC_NONE      0           /* the magic has gone! */

/* something recognizable to store in the lock handle after freeing the
 * the lock_info pointer
 */
#define RWL_LOCK_INFO_NULL_PTR   0

/* something recognizable to store in the lock_id field of the lock handle after
 * freeing the lock_info pointer
 */
#define RWL_LOCK_ID_NONE         0

/*  lock op "types" */
#define LOCK_OP_UNLOCK   0
#define LOCK_OP_READ     1
#define LOCK_OP_RIW      2
#define LOCK_OP_PROMOTE  3
#define LOCK_OP_WRITE    4
#define LOCK_OP_DEMOTE   5
#define LOCK_OP_DELETE   6

/* text strings for lock ops */
PRIVATE char *op_text[] = { "UNLOCK", 
			    "READ-LOCK",
			    "RIW-LOCK",
			    "PROMOTE",
			    "WRITE-LOCK",
			    "DEMOTE",
			    "DELETE" };

/* LOCK_BEGIN_CRITICAL and LOCK_END_CRITICAL check the coherency
 * of the primitive lock data structures and verify that lock invariants
 * have not been violated.  In addition, LOCK_BEGIN_CRITICAL
 * retrieves the lock_info pointer from the lock handle, places it in 
 * the lock_info_p parameter and returns with the lock_info mutex locked.
 */
#define LOCK_BEGIN_CRITICAL(handle, lock_op, lock_info_p) { \
	    rwl_handle_t     this_lock; \
            unsigned short   this_op = (lock_op); \
	    rwl_lock_info_t  *this_info; \
            this_lock = (handle); \
            if (rwl_lock_debug) { \
                printf("ENTER LOCK CRITICAL SECTION - %s\n", op_text[this_op]); \
            } \
            this_info = check_lock(this_lock); \
            (lock_info_p) = this_info; \
            if (rwl_lock_debug) { \
                print_lock_info(op_text[this_op], this_info); \
            }
                
#define LOCK_END_CRITICAL   \
            /* can't check a delete op on the way out cause the lock \
             * no longer (logically) exists \
             */ \
            if (! (this_op == LOCK_OP_DELETE)) { \
                /* don't lock mutex */ \
                check_existence(this_lock, false, this_info); \
            } \
            V(this_info->mutex); \
	    if (rwl_lock_debug) { \
	        print_lock_info(op_text[this_op], this_info); \
                printf("EXIT LOCK CRITICAL SECTION - %s\n", op_text[this_op]); \
	    } \
	}
				   

/* IMPORTANT!! The following macros must be nested within LOCK_BEGIN_CRITICAL
 * and LOCK_END_CRITICAL calls.
 */

#define LOCK_TRY \
            TRY {

#define LOCK_WAIT if (rwl_lock_debug) { \
                      printf("LOCK CONFLICT: Entering wait state\n"); \
		      if (this_info->debug_info != NULL && \
                          *this_info->debug_info != NULL) { \
		          printf("   debug info : %s\n", this_info->debug_info); \
		      } \
                  } \
		  if (pthread_cond_wait(&(this_lock.lock_info->cond), \
                                &(this_lock.lock_info->mutex.mutex)) == -1) { \
		          RAISE(rwl_e_fatal); \
		  } \
		  /* maybe the lock went bad while we waited */ \
	          check_existence(this_lock, false, this_info); \
		  if (rwl_lock_debug) { \
		      printf("*** Exiting wait state ***\n"); \
	              print_lock_info(op_text[this_op], this_info); \
		  }


#define LOCK_END_TRY \
            } CATCH(pthread_cancel_e) { \
	        /* give up the lock without corrupting the invariants, unless \
                 * the lock_info was corrupted by some other thread while we \
                 * slept.  We catch possible existence exceptions because \
		 * we're not allowed to convert the cancel to anything else \
                 */ \
                TRY { \
		    check_existence(this_lock, false, this_info); \
		} CATCH_ALL { \
		    RAISE(pthread_cancel_e); \
		} ENDTRY; \
                V(this_info->mutex); \
	        RERAISE; \
            } \
            CATCH_ALL { \
	        /* we don't know what happened or when so we try to set things \
		 * up so subsequent operations on this lock will generate an \
                 * existence  exception, but all bets are basically off \
                 */ \
		 check_existence(this_lock, false, this_info); \
		 this_info->lock_id = RWL_LOCK_ID_NONE; \
                 /* we want to be certain the thread taking the exception \
		  * doesn't die holding the mutex, but we don't know whether \
		  * it is or not. We nest the mutex_unlock inside an exception \
		  * handler, to avoid blowing the original exception status \
		  * away \
                  */ \
                   TRY { \
                       V(this_info->mutex); \
                   } CATCH_ALL { \
                   /* no-op, just keep the cma_e exception from propagating */ \
                   } \
                   ENDTRY \
		   /* wake up anybody who might be blocked */ \
		   (void) pthread_cond_broadcast(&(this_info->cond)); \
	        RERAISE; \
            } \
            ENDTRY 


/* we could have insisted that SIGNAL and BROADCAST macros be nested as well
 * but sometimes we want to invoke them outside the critical section to avoid
 * unecessary context switching
 */
#define LOCK_BROADCAST(lock) \
            if (rwl_lock_debug) { \
	        print_lock_info("BROADCASTING", (lock).lock_info); \
            } \
            if (pthread_cond_broadcast(&((lock).lock_info->cond)) == -1) { \
	        RAISE(rwl_e_fatal); \
	    }

#define LOCK_SIGNAL(lock) \
            if (rwl_lock_debug) { \
	        print_lock_info("SIGNALLING", (lock).lock_info); \
            } \
            if (pthread_cond_signal(&((lock).lock_info->cond)) == -1) { \
	        RAISE(rwl_e_fatal); \
	    }

/*
 * End PRIVATE types
 */


/*
 *  PRIVATE prototypes
 */

PRIVATE rwl_lock_info_t *check_lock (
    rwl_handle_t  lock
);

PRIVATE void gen_lock_id ( 
    unsigned long  *lock_id_p
);

PRIVATE void print_lock_info (
    char             *msg,
    rwl_lock_info_t  *info_p
);

PRIVATE void check_existence (
    rwl_handle_t     lock,
    boolean32        do_lock,  /* lock the lock_info mutex? */
    rwl_lock_info_t  *lock_info_p
);

/*
 * The following routines check for legal lock state transitions.  For instance,
 * a lock_demote operation is only valid if the lock is currently in the 
 * rwl_lock_write state.  Each of these operations assumes the lock data and 
 * invariants are valid, and that the lock_info mutex is locked. They also
 * assume that lock operation that invokes them has not altered any of the lock
 * invariants - this leaves them free to raise an exception locally.  
 */

PRIVATE boolean32 check_owner ( 
    unsigned32       lock_op,
    rwl_lock_info_t  *info_p,
    boolean32        fatal
);

PRIVATE void check_promote ( 
    rwl_lock_info_t  *info_p
);


PRIVATE void check_demote ( 
    rwl_lock_info_t  *info_p
);
 
PRIVATE void check_delete ( 
    rwl_lock_info_t  *info_p
);

PRIVATE void check_unlock ( 
    rwl_lock_info_t  *info_p
);


/* 
 * End PRIVATE declarations
 */

PRIVATE boolean32 check_init (
    void
)
{
    if (!rwl_module_initialized) {
	EXCEPTION_INIT(rwl_e_init);
	return false;
    }

    return true;
}

/* rwl_mutex_init
 *
 * initializes a pthread_mutex_t and stores debugging info pointer (assumes
 * pointer info storage is already allocated and does not reallocate)
 * returns -1 if pthread_mutex_init call fails - caller should check errno
 */
PRIVATE int rwl_mutex_init
  (
    rwl_mutex_t  *rwl_mutex,
    char         *info
  )
{
    int return_val = 0;

    return_val = pthread_mutex_init(&rwl_mutex->mutex, pthread_mutexattr_default);
    if (return_val == 0) {
	rwl_mutex->info = info;
	rwl_mutex->locked = false;
    }

    return return_val;
}

/* rwl_mutex_destroy
 *
 * destroys a pthread_mutex_t. Returns -1 if pthread_mutex_init call fails -
 * caller should check errno
 */
PRIVATE int rwl_mutex_destroy
  (
    rwl_mutex_t  *rwl_mutex
  )
{
    int return_val = 0;

    return_val = pthread_mutex_destroy(&rwl_mutex->mutex);

    return return_val;
}


PRIVATE void rwl_mutex_lock
  (
     rwl_mutex_t  *rwl_mutex
  )
{
    if (rwl_lock_debug && rwl_mutex->info != NULL) {
	printf("   attempt mutex lock: %s - state: %s\n", rwl_mutex->info,
	       rwl_mutex->locked ? "locked" : "unlocked");
    }

    if (pthread_mutex_lock(&rwl_mutex->mutex) == -1) { 
	RAISE(rwl_e_fatal);
    }

    rwl_mutex->locked = true;

    if (rwl_lock_debug && rwl_mutex->info != NULL) {
	printf("   acquired mutex lock : %s\n", rwl_mutex->info);
    }

}   

PRIVATE void rwl_mutex_unlock
  (
     rwl_mutex_t  *rwl_mutex
  )
{

    if (pthread_mutex_unlock(&rwl_mutex->mutex) == -1) { 
	RAISE(rwl_e_fatal);
    }
 
    rwl_mutex->locked = false;

    if (rwl_lock_debug && rwl_mutex->info != NULL) {
	printf("   mutex unlock: %s\n", rwl_mutex->info);
    }
}   


PRIVATE void set_lock_owner
  (
    unsigned32       lock_op,
    rwl_lock_info_t  *info_p
  )
{
    pthread_t  self;

    switch (lock_op) {
    case LOCK_OP_WRITE:
	info_p->owner = pthread_self();
	info_p->owner_set = true;
	break;

    case LOCK_OP_RIW:
	info_p->riw_owner = pthread_self();
	info_p->riw_owner_set = true;
	break;

    default:                    
       /*no-op */
	break;
    }
}

PRIVATE void clear_lock_owner
  (
    unsigned32       lock_op,
    rwl_lock_info_t  *info_p
  )
{
    switch (lock_op) {
    case LOCK_OP_WRITE:
	info_p->owner_set = false;
	break;

    case LOCK_OP_RIW:
	info_p->riw_owner = pthread_self();
	info_p->riw_owner_set = false;
	break;

    default:                    
       /*no-op */
	break;
    }
}



PUBLIC void rwl_module_init (
    void
)
{ 
    
    if ( rwl_module_initialized) {   
	if ( ! rwl_module_init_done) {
	    EXCEPTION_INIT(rwl_e_init_in_progress);
	    RAISE(rwl_e_init_in_progress);  
	}
	
    } else {
	
	rwl_module_initialized = true;   
	
	EXCEPTION_INIT (rwl_e_init);
	EXCEPTION_INIT (rwl_e_fatal);
	EXCEPTION_INIT (rwl_e_existence);
	EXCEPTION_INIT (rwl_e_quota);
	EXCEPTION_INIT (rwl_e_mem);
	EXCEPTION_INIT (rwl_e_use);
	EXCEPTION_INIT (rwl_e_owner);
	
	if (rwl_mutex_init(&rwl_kernel_mutex, "Kernel mutex") == -1) {
	    goto rwl_module_init_pthread_error;
	}

	if (rwl_mutex_init(&rwl_lock_id_mutex, "Lock id mutex") == -1) {	
	    goto rwl_module_init_pthread_error;
	}

	rwl_module_init_done = true; 
	EXCEPTION_INIT(rwl_e_init_in_progress);
    }
    
    return;  /* normal return */

rwl_module_init_pthread_error:

    switch (errno) {
    case EAGAIN: 
	RAISE(rwl_e_quota);
	break;
    case ENOMEM: 
	RAISE(rwl_e_mem);
	break;
    default:
	RAISE(rwl_e_fatal);
	break;
    }

}
   

PUBLIC void rwl_read_lock
  (
    rwl_handle_t lock
  )
{
    rwl_lock_info_t  *info_p;

    LOCK_BEGIN_CRITICAL(lock, LOCK_OP_READ, info_p) {

	LOCK_TRY {
	    /* must wait for writer */
	    while ( info_p->state == rwl_lock_write) {
		LOCK_WAIT;
	    }

	    info_p->num_readers += 1;
	    if (info_p->state == rwl_lock_none) {
		info_p->state = rwl_lock_read;
	    }

	} LOCK_END_TRY;
	
    } LOCK_END_CRITICAL;
}

PUBLIC void rwl_riw_lock
  (
    rwl_handle_t lock
  )
{
    rwl_lock_info_t  *info_p;

    LOCK_BEGIN_CRITICAL(lock, LOCK_OP_RIW, info_p) {

	LOCK_TRY {
	    
	    /* must wait for writer or read-intend-writer to complete */
	    while (info_p->state == rwl_lock_write ||
		   info_p->state == rwl_lock_riw ) {
		LOCK_WAIT;
	    }

	    set_lock_owner(LOCK_OP_RIW, info_p);
	    info_p->num_readers += 1;
	    info_p->state = rwl_lock_riw;

	} LOCK_END_TRY;
	
    } LOCK_END_CRITICAL;
}


PUBLIC void rwl_write_lock
  (
    rwl_handle_t lock
  )
{
    rwl_lock_info_t  *info_p;

    LOCK_BEGIN_CRITICAL(lock, LOCK_OP_WRITE, info_p) {
	
	LOCK_TRY {
	    
	    while ( ! (info_p->state == rwl_lock_none) ) {
		LOCK_WAIT;
	    }
	    
	    info_p->num_readers = -1;
	    info_p->state = rwl_lock_write;
	    set_lock_owner(LOCK_OP_WRITE, info_p);
	    
	} LOCK_END_TRY;
	
    } LOCK_END_CRITICAL;
}


/* only read-intend-write locks are promotable */
PUBLIC void rwl_promote_lock
  (
    rwl_handle_t lock
  )
{
    rwl_lock_info_t  *info_p;

    LOCK_BEGIN_CRITICAL(lock, LOCK_OP_PROMOTE, info_p) {
	
	check_promote(info_p);
	
	LOCK_TRY {
	    
	    /* wait until we're the only reader left */
	    while (info_p->num_readers !=  1 ) {
		LOCK_WAIT;
	    }
	    
	    info_p->num_readers =  (-1); 
	    info_p->state = rwl_lock_write;
	    clear_lock_owner(LOCK_OP_RIW, info_p);
	    set_lock_owner(LOCK_OP_WRITE, info_p);

	} LOCK_END_TRY;
	
    } LOCK_END_CRITICAL;
}


/* A write lock may be demoted to a read-intend-write lock, but only by the thread 
 * that owns the lock  
 */
PUBLIC void rwl_demote_lock
  (
    rwl_handle_t  lock
  )
{      
    rwl_lock_info_t  *info_p;

    LOCK_BEGIN_CRITICAL(lock, LOCK_OP_DEMOTE, info_p) {
	
	check_demote(info_p);  /* this will panic, if necessary */
	
	clear_lock_owner(LOCK_OP_WRITE, info_p);
	set_lock_owner(LOCK_OP_RIW, info_p);
	info_p->state = rwl_lock_riw;
	info_p->num_readers = 1;  
	
    } LOCK_END_CRITICAL;         /* unlock before broadcast */

    /* need to broadcast: any number of threads may be blocked and
     * we want to let any blocked readers proceed.
     */
    LOCK_BROADCAST(lock);

}

/* Note:  As there is no record of which threads are holding read locks,
 * any old thread can unlock a read lock whether or not it holds one.
 * The lock object invariants will be adjusted accordingly.  This is not
 * great, but at least the error will show up when the last legitimate holder
 * of the read lock attempts an unlock
 */ 
PUBLIC void rwl_unlock
  (
    rwl_handle_t lock
  )
{
    boolean32        must_broadcast = false,
                     must_signal    = false;
    rwl_lock_info_t  *info_p;


    LOCK_BEGIN_CRITICAL(lock, LOCK_OP_UNLOCK, info_p) {
    
	check_unlock(info_p);  /* will raise an exception, if necessary */
	
	switch (info_p->state) {
	    
	case rwl_lock_read:
	    
	    info_p->num_readers -= 1;
	    
	    if (info_p->num_readers == 0) {
		info_p->state = rwl_lock_none;
		
		/* a signal will suffice: there are no outstanding 
		 * read-intend-write locks, so at most one blocked writer 
		 * can proceed (although this blocked writer may lose out to 
		 * a thread requesting a new read-intend-write or write lock)
		 */
		must_signal = true;
	    }  
	    break;
	    
	case rwl_lock_riw:
	    info_p->num_readers--;
	    
	    if (info_p->num_readers == 0) {
		info_p->state = rwl_lock_none;
	    } else if (check_owner(LOCK_OP_RIW, info_p, false)) {
		info_p->state = rwl_lock_read;
                clear_lock_owner(LOCK_OP_RIW, info_p);
	    }
	    
	    /* need to broadcast: any number of threads may be blocked
	     * waiting for a write or read-intend-write lock and we want
	     * one of them to proceed.  If there are no concurrent readers,
	     * then blocked writers should be given a shot at the lock.  If
	     * there are concurrent readers then we need only give the next
	     * blocked read-intend-writer a shot.  The current implementation
	     * doesn't distinguish between the various reasons for blocking
	     * so we broadcast to cover them all.  Note that the ability
	     * to distinguish wait conditions will require a policy decision:
	     * namely, whether to favor blocked writers or blocked 
	     * read-intend-writers.
	     */
	    must_broadcast = true;
	    break;
	    
	case rwl_lock_write:
	    clear_lock_owner(LOCK_OP_WRITE, info_p);
	    info_p->num_readers = 0;	
	    info_p->state = rwl_lock_none;
	    
	    /* need to broadcast: all kinds of threads may be blocked, and all
	     * readers and one read-intend-writer should be allowed in unless
	     * a new or blocked writer grabs the lock first.
	     */
	    must_broadcast = true;
	    break;
	    
	}
	
    } LOCK_END_CRITICAL; 

    if (must_broadcast) {
	LOCK_BROADCAST(lock);
    } else if (must_signal) {
	LOCK_SIGNAL(lock);
    }

}



/*XXX haven't sorted out atomicity issues w.r.t. creation/deletion by concurrent
 * threads.  For now, these operations should only be performed by a "distinguished"
 * task.
 */
PUBLIC void rwl_create_lock
  (
     rwl_handle_t  *handle_p,
     char          *debug_info
  )
{
    rwl_lock_info_t  *info_p;
    boolean32        mutex_created = false,
                     cond_created   = false;
          
    if (! check_init()) {
	RAISE(rwl_e_init);
    }

    pthread_testcancel();  /* since we don't call any cancellable functions */

    info_p = (rwl_lock_info_t *) malloc(sizeof(rwl_lock_info_t));
    if (info_p == NULL) {
	RAISE(rwl_e_mem);
       /*NOTREACHED */
      return;
    }


    bzero(info_p, sizeof(rwl_lock_info_t));

    info_p->debug_info = NULL;
    if (debug_info != NULL && *debug_info != NULL) {
	if ((info_p->debug_info = malloc(strlen(debug_info)+1)) != NULL) {
	    strcpy(info_p->debug_info, debug_info);
	}
    }
 
   if (rwl_mutex_init(&info_p->mutex, info_p->debug_info) == -1) {	
	goto rwl_create_lock_pthread_error;
    }
    mutex_created = true;
    
    if (pthread_cond_init(&info_p->cond, 
			    pthread_condattr_default) == -1) {	
	(void) rwl_mutex_destroy(&info_p->mutex);
	goto rwl_create_lock_pthread_error;
    }
    cond_created = true;

    clear_lock_owner(LOCK_OP_WRITE, info_p);
    clear_lock_owner(LOCK_OP_RIW, info_p);

    info_p->state = rwl_lock_none;  
    
    info_p->magic = RWL_LOCK_INFO_MAGIC;
    
    gen_lock_id(&info_p->lock_id);
 
     
    handle_p->magic = RWL_HANDLE_MAGIC;
		    
    /* serialize  all lock handle read/writes */
    ENTER_LOCK_KERNEL {
	
	handle_p->lock_info = info_p;
	handle_p->lock_id = info_p->lock_id;
	handle_p->magic = RWL_HANDLE_MAGIC;
	
    } EXIT_LOCK_KERNEL;

    return;  /* normal return */


rwl_create_lock_pthread_error:

    free((char *) info_p);

    switch (errno) {
    case EAGAIN: 
	RAISE(rwl_e_quota);
	break;
    case ENOMEM: 
	RAISE(rwl_e_mem);
	break;
    default:
	RAISE(rwl_e_fatal);
	break;
    }

}

PUBLIC void rwl_delete_lock
  (
    rwl_handle_t  *lock_p
  )
{
    rwl_lock_info_t  *info_p;
    
    LOCK_BEGIN_CRITICAL(*lock_p, LOCK_OP_DELETE, info_p) {
	
	check_delete(info_p);
	
	/* make sure any future references to this lock_info storage take an 
	 * existence exception
	 */
	info_p->state = rwl_lock_deleted;
	
    } LOCK_END_CRITICAL;
    
    /* serialize lock_handle reads/writes */
    ENTER_LOCK_KERNEL {
	lock_p->lock_info = RWL_LOCK_INFO_NULL_PTR;
	lock_p->lock_id   = RWL_LOCK_ID_NONE;
	lock_p->magic     = RWL_MAGIC_NONE;
    } EXIT_LOCK_KERNEL;
    
    /* flush synchronization object storage */
    if (rwl_mutex_destroy(&info_p->mutex) == -1) {
	RAISE(rwl_e_fatal);
    }

    if (pthread_cond_destroy(&info_p->cond) == -1) {
	RAISE(rwl_e_fatal);
    }

    bzero(info_p, sizeof(rwl_lock_info_t));
    free( (char *) info_p);

}


/*XXX should eventually print more than the lock state and the number
 * of readers 
 */
PRIVATE void print_lock_info
  (
    char             *msg,
    rwl_lock_info_t  *info_p
  )
{
    char       *lock_state = "unknown";
    char       *debug_info = "";

    printf("   %s\n", msg);

    if (info_p->magic != RWL_LOCK_INFO_MAGIC) {
	printf("*** LOCK DATA IS CORRUPT ***\n");
	return;
    }

    switch (info_p->state) {
    case rwl_lock_none:
	lock_state = "none";
	break;

    case rwl_lock_deleted:
	lock_state = "deleted";
	break;

    case rwl_lock_read:
	lock_state = "read";
	break;

    case rwl_lock_riw:
	lock_state = "read-intend-write";
	break;

    case rwl_lock_write:
	lock_state = "write";
	break;
    }

    if (info_p->debug_info != NULL && *info_p->debug_info != NULL) {
	debug_info = info_p->debug_info;
    }
    printf("   info: %-20s - state: %-20s - num_readers: %4d\n",
	   debug_info, lock_state, info_p->num_readers);
}
 

/*
 * The following routines check for legal lock state transitions.  For instance,
 * a lock_demote operation is only valid if the lock is currently in the 
 * rwl_lock_write state.  Each of these operations assumes the lock data and 
 * invariants are valid, and that the lock_info mutex is locked. They also
 * assume that lock operation that invokes them has not altered any of the lock
 * invariants - this leaves them free to raise an exception locally.  
 */


/* if fatal is true and the calller doesn't have the specified ownership
 * raise a fatal exception otherwise return false.  Always return true if
 * caller is the owner.  Note that as no ownership info is kept for
 * read locks, a call to this function with LOCK_OP_READ and fatal
 * = true will cause a fatal exception.
 */
PRIVATE boolean32 check_owner
  (
    unsigned32      lock_op,
    rwl_lock_info_t *info_p,
    boolean32       fatal    /* raise an exception if caller doesn't own the lock? */
  )
{
    pthread_t       self;
    boolean32       owner = false;   /* assume caller doesn't own the lock */

    self = pthread_self();

    switch (lock_op) {
    case LOCK_OP_WRITE:
	if (info_p->owner_set && pthread_equal_np(self, info_p->owner)) {
	    owner = true;
	}
	break; 

    case LOCK_OP_RIW:
	if (info_p->riw_owner_set &&
	    pthread_equal_np(self, info_p->riw_owner)) {
	    owner = true;
	}
	break; 

    default:
	break;
    }

    if (fatal && !owner) {
	V(info_p->mutex);
	RAISE(rwl_e_owner);
    }

    return owner;
}
 

PRIVATE void check_promote
  (
    rwl_lock_info_t  *info_p
  )
{

    if (info_p->state != rwl_lock_riw) {

        V(info_p->mutex);
	RAISE(rwl_e_use);
        /*NOTREACHED*/
	return;
   }

   (void) check_owner(LOCK_OP_RIW, info_p, true);
    return;
}
    

PRIVATE void check_demote
  (
    rwl_lock_info_t  *info_p
  )
{

    if (info_p->state != rwl_lock_write) {
	V(info_p->mutex);
	RAISE(rwl_e_use);
        /*NOTREACHED*/
	return;
    }

    (void) check_owner(LOCK_OP_WRITE, info_p, true);

    return;
}
   

PRIVATE void check_delete
  (
    rwl_lock_info_t  *info_p
  )
{

    /* can't delete if anyone is using the lock */
    if (info_p->state != rwl_lock_none) {
	V(info_p->mutex);
	RAISE(rwl_e_use);
        /*NOTREACHED*/
	return;
   }

    return;
}


PRIVATE void check_unlock
  (
    rwl_lock_info_t  *info_p
  )
{

    switch(info_p->state) {

    case rwl_lock_write:
	(void) check_owner(LOCK_OP_WRITE, info_p, true);
	break;
	
    case rwl_lock_riw:
	/* Can't check owner cause although the state is riw, the caller
	 * might be giving up concurrent read lock (and would therefore
         *not be the owner
	 */
	/*FALLTHROUGH*/
    case rwl_lock_read:
	/* nothing to check */
	break;
	
    case rwl_lock_none:
	V(info_p->mutex);
	RAISE(rwl_e_use);
	break;
	
    default:
	RAISE(rwl_e_fatal);
	break;
    }
}
      

/* 
 * This routine checks the basic coherency of the lock info, locking
 * the lock_info mutex, if requested.
 */
PRIVATE void check_existence
  (
    rwl_handle_t     lock,
    boolean32        do_lock,  /* lock the lock_info mutex? */
    rwl_lock_info_t  *lock_info_p
  )
{
    boolean32        exists = true; /* be optimistic */
    
    if (lock_info_p->magic != RWL_LOCK_INFO_MAGIC) {
	exists = false;
    }

    if (exists) {
	/* grab the lock_info mutex */
	if (do_lock) {
	    P(lock.lock_info->mutex);
	}
	
	if (lock.lock_info->state == rwl_lock_deleted) {
	    if (do_lock) {
		V(lock.lock_info->mutex);
	    }
	    exists = false;
	}
    }
    
    if (exists) {
    
	/* if the lock id in the handle doesn't match the lock_info, then
	 * the lock has been been deleted and possibly, if and when lock_info
	 * caching is implemented, the lock_info storage has been re-used.
	 */
	if (lock.lock_info->lock_id != lock.lock_id) {
	    if (do_lock) {
		V(lock.lock_info->mutex);
	}
	    exists = false;
	}
    }
    
    if (!exists) {
	RAISE(rwl_e_existence);
    }
}

/* Assumes basic coherency of the lock handle and lock_info, and that the
 * lock info mutex is locked.  returns true if lock invariants are correct,
 * false otherwise
 */
PRIVATE boolean32 check_invariance
  (
    rwl_lock_info_t  *info_p
  )
{

    boolean32 invariance = true;

    switch (info_p->state) {
	
    case rwl_lock_none:
	if (info_p->num_readers != 0) {
	    invariance = false;
	}
	break;

    case rwl_lock_deleted:
	if (info_p->num_readers != 0) {
	    invariance = false;
	}
	break;
	
    case rwl_lock_read:
	if (info_p->num_readers < 1) {
	    invariance = false;
	}
	break;
	
    case rwl_lock_riw:
	if (info_p->num_readers < 1) {
	    invariance = false;
	}
	break;
	
    case rwl_lock_write:
	if (info_p->num_readers != -1) {
	    invariance = false;
	}
	
	break;
	
    default:
	invariance = false;
	break;
	
    }
    
    if (! invariance ) {
	if (rwl_lock_debug) {
	    print_lock_info("*** LOCK INVARIANT VIOLATION ***", info_p);
	}
	
	V(info_p->mutex);
	/*  throb the next thread blocked on the lock's condition
	 * variable (if any). It should fail the invariance check as well.
	 */
	(void) pthread_cond_signal(&(info_p->cond));
    }

    return invariance;

}

/* an encapsulation of the previous three checks */
PRIVATE rwl_lock_info_t *check_lock
  (
    rwl_handle_t  lock
  )
{
    rwl_lock_info_t  *info_p;
    boolean32        exists = true;

    if (! check_init()) {
	RAISE(rwl_e_init);
    }
              
    /* don't let anyone interrupt us until we know that the lock_info
     * is coherent and we've grabbed it's mutex (if necessary).
     */
    ENTER_LOCK_KERNEL {
	
	if ( (lock.magic != RWL_HANDLE_MAGIC) ||
	    (lock.lock_id == RWL_LOCK_ID_NONE) ) {
	    exists = false;

	} else {
	    info_p = lock.lock_info;
	    check_existence(lock, true, info_p);
	}

    } EXIT_LOCK_KERNEL;

    if (!exists) {
	RAISE(rwl_e_existence);
    }

    if (! check_invariance(info_p)) {
	RAISE(rwl_e_fatal);
    }

    return info_p;
}  




PRIVATE void gen_lock_id
  (
    unsigned long  *lock_id_p
  )
{
    static unsigned long  next_id = 1;  /* ids start at one */

    if (! check_init()) {
	RAISE(rwl_e_init);
    }

    PSEM(rwl_lock_id_mutex) {
	*lock_id_p = next_id++;
    } VSEM;

}
