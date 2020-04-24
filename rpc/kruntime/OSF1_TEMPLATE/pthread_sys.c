/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: pthread_sys.c,v $
 * Revision 1.2.13.2  1996/02/18  00:00:21  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:53:36  marty]
 *
 * Revision 1.2.13.1  1995/12/08  00:14:35  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:55:58  root]
 * 
 * Revision 1.2.11.1  1994/01/21  22:31:48  cbrooks
 * 	RPC Code Cleanup
 * 	[1994/01/21  20:41:24  cbrooks]
 * 
 * Revision 1.2.7.4  1993/01/03  22:35:33  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:51:13  bbelch]
 * 
 * Revision 1.2.7.3  1992/12/23  19:37:35  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  14:49:34  zeliff]
 * 
 * Revision 1.2.7.2  1992/10/13  17:24:43  marty
 * 	OSC1.1.1 upgrade.  Changes are osc1.0.4 compatible
 * 	[1992/10/13  17:14:15  marty]
 * 
 * Revision 1.2.5.2  1992/09/29  22:46:50  marty
 * 	OSC1.1 upgrade
 * 	[1992/09/29  22:38:54  marty]
 * 
 * Revision 1.2.3.2  1992/09/29  20:10:16  garyf
 * 	fix compilation warnings & errors for OSF/1 R1.1
 * 
 * Revision 1.2.2.2  1992/05/01  17:55:26  rsalz
 * 	     Remove non-Mach stuff.
 * 	[1992/05/01  17:50:22  rsalz]
 * 
 * Revision 1.2  1992/01/19  22:13:33  devrcs
 * 	Dropping DCE1.0 OSF1_misc port archive
 * 
 * $EndLog$
 */
/*
*/
/*
**  Copyright (c) 1989, 1990 by
**      Hewlett-Packard Company, Palo Alto, Ca.
**
**  NAME:
**
**      pthread_sys.c
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  See pthread.h.  Implements pthreads interface for OSF/1 kernel.
**
*/

#include <commonp.h>
#include <dce/ker/exc_handling.h>

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/user.h>       /* need the u area for a thread handle */
#include <sys/kernel.h>
#include <sys/proc.h>       /* need for u.u_procp.c_cptr */
                                               

/*
 * WARNING: this implementation is currently only designed to
 * operate properly in single CPU a non-preemptive thread environment.
 * For the most part, correcting this limitation involves making the
 * implementation of the mutex and cv primitives "safe".
 */


/* ================================================================= */

EXCEPTION exc_e_alerted;

/* ================================================================= */

pthread_attr_t          pthread_attr_default;
pthread_mutexattr_t     pthread_mutexattr_default;
pthread_condattr_t      pthread_condattr_default;

/* ================================================================= */

#define PTHREADS_INIT \
    if (pthreads__initd == 0) \
        pthread__init();

INTERNAL short pthreads__initd = 0;

/* ----------------------------------------------------------------- */

/*
 * "once" stuff...
 */
#define PTHREAD_ONCE_INITD          0x01
#define PTHREAD_ONCE_INPROGRESS     0x02

/* ----------------------------------------------------------------- */

/*
 * Thread creation grunge...
 */
/*
 * Internal junk to allow us to pass an argument to a thread upon creation...
 * This "junk" will be unnecessary once we come up to
 * a more recent OSF release that has kernel_thread_w_arg().
 */

typedef struct
{
    pthread_t thread;
    pthread_startroutine_t start_routine;
    pthread_addr_t arg;
    unsigned busy: 1;
} args_block_t, *args_block_p_t;

#define MAX_ARGS_BLOCKS 10
INTERNAL args_block_t   args_blocks[MAX_ARGS_BLOCKS];
INTERNAL pthread_mutex_t args_m;
INTERNAL pthread_cond_t args_cv;

/* ----------------------------------------------------------------- */

/*
 * exception package grunge...
 */
INTERNAL pthread_key_t pthread_exc_ctxt_key;

/* ----------------------------------------------------------------- */

/*
 * pthread_cond_timedwait() grunge...
 */

typedef struct {
    unsigned    timedout : 1;   /* if the timeout is no longer pending */
    pthread_t   thread;
    pthread_cond_t *cv;         /* cv the timedwait is for */
} pthread__timedwait_t;

INTERNAL pthread_mutex_t    timedwait_m;

#ifndef PTHREAD_TIMEDWAIT_TABLE_SIZE
#  define PTHREAD_TIMEDWAIT_TABLE_SIZE    5
#endif

INTERNAL pthread__timedwait_t timedwait_tbl[PTHREAD_TIMEDWAIT_TABLE_SIZE];

/* ----------------------------------------------------------------- */
/*
 * Per-thread context stuff.
 */

/*
 * By default, PTHREAD_DESTRUCTOR_USEFUL is not defined since the
 * destructor is only used internally by pthreads during thread exit
 * processing...  in the kernel, this pthreads isn't wired-in to the
 * thread exit path, so it never gets a chance to run anyway, so why
 * use the storage...
 */

typedef struct pthread_key_ctx_t
{
    pthread_key_t           key;
#ifdef PTHREAD_DESTRUCTOR_USEFUL
    pthread_destructor_t    destructor;
#endif
} pthread_key_ctx_t;

typedef struct pthread_thread_ctx_chain_t
{
    struct pthread_thread_ctx_t *forw;
    struct pthread_thread_ctx_t *back;
} pthread_thread_ctx_chain_t;

typedef struct pthread_thread_ctx_t
{
    pthread_thread_ctx_chain_t  c;
    pthread_t                   thread;
    pthread_key_t               key;
    pthread_addr_t              context_value;
} pthread_thread_ctx_t;

/*
 * A hash table for the thread contexts.
 * Tune the hash list size as necessary.
 *
 * If you're willing to add something to the OS's standard
 * per thread data structures, all of this lookup stuff is
 * unnecessary (as well as the set / get operations becoming
 * pretty cheap).
 */

#define THREAD_CTX_TABLE_SIZE   37
INTERNAL pthread_thread_ctx_chain_t   thread_ctx[THREAD_CTX_TABLE_SIZE];

/*
 * A fixed size list of the different keys that can exist.
 */

#define KEY_CTX_TABLE_SIZE      4
INTERNAL pthread_key_ctx_t      key_ctx[KEY_CTX_TABLE_SIZE];

/*
 * Convert the thread ID into a index into the table.
 */
#define thread_hash(thread) \
    ((int)(((unsigned32) thread) % THREAD_CTX_TABLE_SIZE))


/* =============================================================== */

INTERNAL void pthread__init
    PROTOTYPE((void));

INTERNAL void pthread_exc_ctxt_free
    PROTOTYPE((
        pthread_addr_t context_value
    ));

INTERNAL void pthread__delay_wakeup
    PROTOTYPE((
        caddr_t arg
    ));

INTERNAL void pthread__ctx_init
    PROTOTYPE((void));

INTERNAL pthread_thread_ctx_t *pthread__thread_ctx_get
    PROTOTYPE((
        pthread_key_t               key
    ));

INTERNAL pthread_thread_ctx_t *pthread__thread_ctx_lookup
    PROTOTYPE((
        pthread_key_t               key
    ));

INTERNAL void pthread__thread_ctx_free
    PROTOTYPE((
        pthread_thread_ctx_t            *ctx
    ));

INTERNAL args_block_p_t find_free_args
    PROTOTYPE((void));

INTERNAL void free_args
    PROTOTYPE((
        args_block_p_t ap
    ));

INTERNAL args_block_p_t find_thread_args
    PROTOTYPE((
        pthread_t   t
    ));

INTERNAL void base_routine
    PROTOTYPE((void));

INTERNAL void pthread__delay_init
    PROTOTYPE((void));


/* =============================================================== */
/*
 * Package initialization
 */
 
INTERNAL void pthread__init()
{
    if (pthreads__initd == 1)
        return;

    pthread__ctx_init();
    pthreads__initd = 1;
    /*
     * do this after the __initd = 1 or loop forever :-)
     */
    pthread_mutex_init(&args_m, pthread_mutexattr_default);
    pthread_cond_init(&args_cv, pthread_condattr_default);
    pthread_keycreate(&pthread_exc_ctxt_key, pthread_exc_ctxt_free);

    /*
     * Initialize exception package defined exceptions (easier to
     * do as part of pthreads initialization than some exc specific
     * init).
     */
    EXCEPTION_INIT(exc_e_alerted);

    /*
     * Init other goodies...
     */
    pthread_mutex_init(&timedwait_m, pthread_mutexattr_default);
    pthread__delay_init();
}
   
/* =============================================================== */

/*
 * "once" operations.
 *
 * This implementation only works for a non-preemptive thread
 * environment *and* non-yielding initialization routines.
 */

PRIVATE int pthread_once(once_block, init_routine)
pthread_once_t              *once_block;
pthread_initroutine_t       init_routine;
{
    PTHREADS_INIT;

    if (*once_block & PTHREAD_ONCE_INITD)
        return (0);

    /*
     * This test should really be:
     *      if one init is inprogress
     *          wait until the inprogress one completes
     *          return
     * Since this condition is very unlikely (at least initially)
     * this hack will at least protect us.
     */
    if (*once_block & PTHREAD_ONCE_INPROGRESS)
        DIE("(pthread_once)");

    *once_block |= PTHREAD_ONCE_INPROGRESS;
    (*init_routine) ();
    *once_block &= ~PTHREAD_ONCE_INPROGRESS;
    *once_block |= PTHREAD_ONCE_INITD;

    return (0);
}

/* =============================================================== */

/*
 * Generic Thread operations.
 */

INTERNAL args_block_p_t find_free_args()
{
    int i;
    args_block_p_t ap;
    
    for (i = 0, ap = args_blocks; i < MAX_ARGS_BLOCKS; i++, ap++)
        if (ap->busy == 0) {
            ap->busy = 1;
            return (ap);
        }
    DIE("find_free_args() - pthread.c");
}

INTERNAL void free_args(ap)
args_block_p_t ap;
{
    ap->busy = 0;
}

INTERNAL args_block_p_t find_thread_args(t)
pthread_t t;
{
    int i;
    args_block_p_t ap;
    
    for (i = 0, ap = args_blocks; i < MAX_ARGS_BLOCKS; i++, ap++)
        if (ap->busy == 1 && ap->thread == t)
            return (ap);
    return (NULL);
}

INTERNAL void base_routine()
{
    pthread_t t = pthread_self();
    args_block_p_t ap;
    pthread_startroutine_t start_routine;
    pthread_addr_t arg;

    pthread_mutex_lock(&args_m);
    while ((ap = find_thread_args(t)) == NULL)
        pthread_cond_wait(&args_cv, &args_m);
    start_routine = ap->start_routine;
    arg = ap->arg;
    free_args(ap);
    pthread_mutex_unlock(&args_m);

    /*
     * The TRY / CATCH is setup to make it so threads created internally
     * (don't have to keep freeing their per thread exception context
     * when we know that they won't exit out from underneath us...).
     * This just lessens the cost of of TRY / CATCH for call executor
     * threads (at the cost of some stack space).
     */
    TRY {
        (*start_routine)(arg);
    } CATCH_ALL {
        RPC_DBG_GPRINTF(("(pthread_create) thread unwound...\n"));
    } ENDTRY

    thread_terminate(t);
    thread_halt_self();
}


/* ----------------------------------------------------------------- */

PRIVATE int pthread_create(new_thread, attr, start_routine, arg)
pthread_t                   *new_thread;
pthread_attr_t              attr;
pthread_startroutine_t      start_routine;
pthread_addr_t              arg;
{
    static pthread_t        new_thrd;

    PTHREADS_INIT

    /*
     * Maybe newproc and it's friends could be used, but it's
     * too confusing to figure that out right now.  It would be real
     * nice if the mach kernel_thread() routine allowed one to pass
     * an argument to a routine... This hack is probably sufficient.
     */

    {
        extern task_t first_task;
        args_block_t *ap;

        pthread_mutex_lock(&args_m);
        ap = find_free_args();
        *new_thread = 
            ap->thread = (pthread_t) kernel_thread(first_task, base_routine);
        ap->start_routine = start_routine;
        ap->arg = arg;
        pthread_mutex_unlock(&args_m);
        pthread_cond_signal(&args_cv);
    }

    return (0);
}

#ifdef NOTDEF
PRIVATE int pthread_join(thread, status) 
thread_t                   thread;
pthread_addr_t              *status;
{
    PTHREADS_INIT

    DIE("(pthread_join)");
}
#endif


PRIVATE int pthread_detach(thread)
pthread_t                   *thread;
{
    PTHREADS_INIT

    /* NO OP */
}


PRIVATE pthread_t pthread_self()
{
    PTHREADS_INIT

    return((pthread_t) current_thread());
}

PRIVATE void pthread_yield()
{
    PTHREADS_INIT

}

PRIVATE int pthread_attr_create(attr)
pthread_attr_t *attr;
{
    PTHREADS_INIT

    *attr = pthread_attr_default;
}

PRIVATE int pthread_attr_destroy(attr)
pthread_attr_t *attr;
{
    PTHREADS_INIT

    /* NO-OP */
}

/* =============================================================== */
/*
 * Per-thread context operations.
 *
 * This implementation is counting on the fact that there will be only
 * a small number of keys (e.g. one for stubs, one for exceptions).
 * Unfortunately, there may be numerous threads with active contexts
 * for these keys.
 *
 * There is a simply indexed small "Key Context" table that contains
 * the global per key information (i.e. the destructor).  Essentially,
 * this table is only used when a key is created (to assign a key value) and
 * when key's destructor function needs to be invoked (at thread exit).
 *
 * All the real work involves a thread_id hashed doubly linked "Thread
 * Context" table.  There is one Thread Context table entry for each
 * "active" key.
 * 
 * A context value of NULL has special meaning indicating that there
 * is no context value for the the key (see below)...
 */

INTERNAL void pthread__ctx_init()
{
    int     i;

    for (i = 0; i < KEY_CTX_TABLE_SIZE; i++)
        key_ctx[i].key = 0;

    for (i = 0; i < THREAD_CTX_TABLE_SIZE; i++)
        thread_ctx[i].forw = 
            thread_ctx[i].back = (pthread_thread_ctx_t *) &thread_ctx[i];
}

INTERNAL void pthread__tctx_dump()
{
    pthread_thread_ctx_t    *tctx_head;
    pthread_thread_ctx_t    *tctx;
    int     probe;

    for (probe = 0; probe < THREAD_CTX_TABLE_SIZE; probe++)
    {
        tctx_head = (pthread_thread_ctx_t *) &thread_ctx[probe];
        tctx = tctx_head->c.forw;

        printf("thread_ctx[%x] %x(b=%x):", probe, tctx_head, tctx_head->c.back);
        while (tctx != tctx_head)
        {
            printf("  %x(t=%x,k=%x,v=%x,b=%x)", tctx, tctx->thread, 
                tctx->key, tctx->context_value, tctx->c.back);
            tctx = tctx->c.forw;
        }
    }
}

/*
 * Lookup (create if necessary) the thread context for this key.
 */

INTERNAL pthread_thread_ctx_t *pthread__thread_ctx_get(key)
pthread_key_t               key;
{
    pthread_thread_ctx_t    *tctx;
    pthread_t               thread;
    int                     probe;
    pthread_thread_ctx_t    *tctx_head;

    assert((key >= 1 && key <= KEY_CTX_TABLE_SIZE));

    thread = pthread_self();
    probe = thread_hash(thread);
    tctx_head = (pthread_thread_ctx_t *) &thread_ctx[probe];

    /*
     * Search for the thread specific key context using the
     * thread to compute a hash.
     */

    RPC_DBG_PRINTF(rpc_e_dbg_last_switch, 5,
        ("pthread__thread_ctx_get: key=%d thread=%x probe=%x\n",
        key, thread, probe));

    for (tctx = tctx_head->c.forw; tctx != tctx_head; tctx = tctx->c.forw)
    {
        RPC_DBG_PRINTF(rpc_e_dbg_last_switch, 10,
            ("pthread__thread_ctx_get: comparing tctx=%x\n", tctx));
        if (tctx->thread == thread && tctx->key == key)
        {
            RPC_DBG_PRINTF(rpc_e_dbg_last_switch, 5,
                ("pthread__thread_ctx_get: key=%d thread=%x found tctx=%x value=%x\n",
                key, thread, tctx, tctx->context_value));
            return (tctx);
        }
    }

    /*
     * Not found; Create and initialize a thread context descriptor.
     * Insert at the head of the doubly linked list.
     */

    RPC_MEM_ALLOC(tctx, pthread_thread_ctx_t *, sizeof *tctx, rpc_c_mem_pthread_ctx, 
        rpc_c_mem_waitok);
    if (tctx == NULL)
        DIE("(pthread__thread_ctx_get)");

    RPC_DBG_PRINTF(rpc_e_dbg_last_switch, 5,
        ("pthread__thread_ctx_get: key=%d thread=%x creating tctx=%x\n",
        key, thread, tctx));

    tctx->thread    = thread;
    tctx->key       = key;
    tctx->context_value = NULL;

    tctx->c.forw    = tctx_head->c.forw;
    tctx->c.back    = tctx_head;
    tctx_head->c.forw->c.back   = tctx;
    tctx_head->c.forw           = tctx;

    return (tctx);
}


/*
 * Lookup the thread context for this key.
 */

INTERNAL pthread_thread_ctx_t *pthread__thread_ctx_lookup(key)
pthread_key_t               key;
{
    pthread_thread_ctx_t    *tctx;
    pthread_t               thread;
    int                     probe;
    pthread_thread_ctx_t    *tctx_head;

    assert((key >= 1 && key <= KEY_CTX_TABLE_SIZE));

    thread = pthread_self();
    probe = thread_hash(thread);
    tctx_head = (pthread_thread_ctx_t *) &thread_ctx[probe];

    /*
     * Search for the thread specific key context using the
     * thread to compute a hash.
     */

    RPC_DBG_PRINTF(rpc_e_dbg_last_switch, 5,
        ("pthread__thread_ctx_lookup: key=%d thread=%x probe=%x\n",
        key, thread, probe));

    for (tctx = tctx_head->c.forw; tctx != tctx_head; tctx = tctx->c.forw)
    {
        RPC_DBG_PRINTF(rpc_e_dbg_last_switch, 10,
            ("pthread__thread_ctx_lookup: comparing tctx=%x\n", tctx));
        if (tctx->thread == thread && tctx->key == key)
        {
            RPC_DBG_PRINTF(rpc_e_dbg_last_switch, 5,
            ("pthread__thread_ctx_lookup: key=%d thread=%x found tctx=%x value=%x\n",
                key, thread, tctx, tctx->context_value));
            return (tctx);
        }
    }

    RPC_DBG_PRINTF(rpc_e_dbg_last_switch, 5,
        ("pthread__thread_ctx_lookup: key=%d thread=%x no tctx\n",
        key, thread));
    return (NULL);
}


/*
 * Free the thread context for this key.
 */

INTERNAL void pthread__thread_ctx_free(tctx)
pthread_thread_ctx_t            *tctx;
{
    RPC_DBG_PRINTF(rpc_e_dbg_last_switch, 5,
        ("pthread__thread_ctx_free: key=%d thread=%x tctx=%x\n",
        tctx->key, tctx->thread, tctx));

    tctx->c.back->c.forw = tctx->c.forw;
    tctx->c.forw->c.back = tctx->c.back;

    RPC_MEM_FREE(tctx, rpc_c_mem_pthread_ctx);
}

/* ------------------------------------------------------------------ */

PRIVATE int pthread_keycreate(key, destructor)
pthread_key_t               *key;
pthread_destructor_t        destructor;
{
    pthread_key_ctx_t   *kctx;
    pthread_key_t   k;

    PTHREADS_INIT

    /*
     * Create and initialize a key context descriptor.
     * key '0' is a special "unused" key.
     * returned keys range from 1..KEY_CTX_TABLE_SIZE
     */

    for (k = 0, kctx = &key_ctx[k]; k < KEY_CTX_TABLE_SIZE; k++, kctx++)
        if (kctx->key == 0)
        {
            kctx->key = *key = k + 1;
#ifdef PTHREAD_DESTRUCTOR_USEFUL
            kctx->destructor = destructor;
#endif
            return (0);
        }

    DIE("pthread_key_create");
}


PRIVATE int pthread_setspecific(key, context_value)
pthread_key_t               key;
pthread_addr_t              context_value;
{
    pthread_thread_ctx_t   *tctx;

    /*
     * Lookup (create if necessary) the thread's ctx block for this key.
     */

    tctx = pthread__thread_ctx_get(key);

    /*
     * If a new context is NULL, we can free the internal per-thread 
     * context storage.
     *
     * Note: this is NOT a property of the pthreads spec!  However,
     * we need some way to determine that the per-thread context
     * block can be freed since we aren't wired into thread exit processing.
     * There are places in the runtime and libnidl that set the value to
     * NULL so this will happen.
     */

    if (context_value == NULL)
    {
        pthread__thread_ctx_free(tctx);
        return (0);
    }

    /*
     * Save the new context value.
     */

    tctx->context_value = context_value;

    return (0);
}

PRIVATE int pthread_getspecific(key, context_value)
pthread_key_t               key;
pthread_addr_t              *context_value;
{
    pthread_thread_ctx_t   *tctx;

    /*
     * ensure that a getspecific does NOT create a internal
     * per-thread context block.
     */
    tctx = pthread__thread_ctx_lookup(key);

    if (tctx == NULL)
        *context_value = NULL;
    else
        *context_value = tctx->context_value;

    return (0);
}

/* =============================================================== */
/*
 * Generic Mutex operations.
 */

PRIVATE int pthread_mutex_init(new_mutex, attr)
pthread_mutex_t             *new_mutex;
pthread_mutexattr_t         attr;
{
    PTHREADS_INIT

    *new_mutex = 0;

    return (0);
}

PRIVATE int pthread_mutex_destroy(mutex)
pthread_mutex_t             *mutex;
{
    /* NO-OP */

    return (0);
}

PRIVATE int pthread_mutex_lock(mutex)
pthread_mutex_t             *mutex;
{
    PTHREAD_MUTEX_LOCK_INTERNAL(*mutex);

    return (0);
}

PRIVATE int pthread_mutex_trylock(mutex)
pthread_mutex_t             *mutex;
{
    return (PTHREAD_MUTEX_TRY_LOCK_INTERNAL(*mutex));
}

PRIVATE int pthread_mutex_unlock(mutex)
pthread_mutex_t             *mutex;
{
    PTHREAD_MUTEX_UNLOCK_INTERNAL(*mutex);

    return (0);
}

/* =============================================================== */

/*
 * Generic Condition Variable operations.
 */

PRIVATE int pthread_cond_init(new_condition, attr)
pthread_cond_t              *new_condition;
pthread_condattr_t          attr;
{
    PTHREADS_INIT

    *new_condition = 0;

    return (0);
}

PRIVATE int pthread_cond_destroy(condition)
pthread_cond_t              *condition;
{
    /* NO-OP */

    return (0);
}

PRIVATE int pthread_cond_signal(condition)
pthread_cond_t              *condition;
{
    PTHREAD_COND_SIGNAL_INTERNAL(*condition);

    return (0);
}

PRIVATE int pthread_cond_broadcast(condition)
pthread_cond_t              *condition;
{
    PTHREAD_COND_SIGNAL_INTERNAL(*condition);

    return (0);
}

/*
 * This routine gets a little tricky if cancel are supported since it
 * would have to tie in with cancels, signals and exception handlers.
 * Fortunately, this is not the case... In the kernel, we don't support
 * cancels the cancel mechanism so a cond wait blocks in a non-signallable.
 */

#define PTHREAD_COND_SLEEP_PRI              (PZERO-3)

PRIVATE int pthread_cond_wait(condition, mutex)
pthread_cond_t              *condition;
pthread_mutex_t             *mutex;
{
    *condition |= PTHREAD_COND_WAITING;
    PTHREAD_MUTEX_UNLOCK_INTERNAL(*mutex);

    sleep(condition, PTHREAD_COND_SLEEP_PRI);

    PTHREAD_MUTEX_LOCK_INTERNAL(*mutex);

    return (0);
}

/*
 * This routine is called when you need to make an interruptible sleep.
 */
#define PTHREAD_COND_INTR_SLEEP_PRI              ((PZERO+3)|PCATCH)

PRIVATE int pthread_cond_wait_intr_np(condition, mutex)
pthread_cond_t              *condition;
pthread_mutex_t             *mutex;
{
    int error;
    *condition |= PTHREAD_COND_WAITING;
    PTHREAD_MUTEX_UNLOCK_INTERNAL(*mutex);

#ifdef NO_TSLEEP
    error = sleep(condition, PTHREAD_COND_INTR_SLEEP_PRI);
#else
    error = tsleep(condition, PTHREAD_COND_INTR_SLEEP_PRI, "intr_wait", 0);
#endif

    PTHREAD_MUTEX_LOCK_INTERNAL(*mutex);

    return (error);
}


/*
 * This implementation of pthread_cond_timedwait() is skewed towards
 * *few* threads concurrently using this operation.  The problem is
 * that some form of per (waiting) thread storage is really necessary
 * inorder to properly manage the [un]timeouts.  We need to be certain to
 * untimeout only this thread's timer (which can only be accomplished
 * by a unique timeout_fn arg).  It would be convenient if we could just sleep
 * on the thread (proc) struct, but that's not easily possible because
 * we also have to be able to be awakened by a cond_signal(); hence the
 * per-thread storage requirement to allow us to sleep on the cv while
 * having a unique timeout_fn arg to untimeout with.
 *
 * Since real per-thread storage is just too costly in this implementation,
 * a hack solution is to provide a small table for waiters to use; which
 * should be sufficient for the runtime's needs.
 */

INTERNAL void pthread__timedwait_wakeup(ttbl)
pthread__timedwait_t *ttbl;
{
    ttbl->timedout = true;
    *ttbl->cv |= PTHREAD_COND_TIMEOUT;
    PTHREAD_COND_SIGNAL_INTERNAL(*ttbl->cv);
}

PRIVATE int pthread_cond_timedwait(condition, mutex, abs_time)
pthread_cond_t              *condition;
pthread_mutex_t             *mutex;
struct timespec             *abs_time;
{
    int                     s;
    int                     tmo;
    struct timeval          atv;         /* absolute timeval */
    pthread__timedwait_t    *ttbl;

    /* convert abs timespec to abs timeval */
    atv.tv_sec = abs_time->tv_sec;
    atv.tv_usec = abs_time->tv_nsec / 1000;

    PTHREAD_MUTEX_LOCK_INTERNAL(timedwait_m);

    /*
     * Locate a timedwait table entry to use.
     */
    for (ttbl = timedwait_tbl; 
        ttbl < &timedwait_tbl[PTHREAD_TIMEDWAIT_TABLE_SIZE]; ttbl++)
    {
        if (ttbl->thread == NULL)
        {
            ttbl->timedout = false;
            ttbl->thread = pthread_self();
            ttbl->cv = condition;
            break;
        }
    }
    if (ttbl >= &timedwait_tbl[PTHREAD_TIMEDWAIT_TABLE_SIZE])
    {
        DIE("timedwait table exhausted");
        /* 
         * make the table bigger, have "cheap" per-thread timedwait
         * storage, ...  vendors choice...
         */
    }

    PTHREAD_MUTEX_UNLOCK_INTERNAL(timedwait_m);

    s = splhigh();  /* accurate time and interaction with timeout */

    if ((tmo = hzto(&atv)) <= 0
        || atv.tv_sec < time.tv_sec
        || (atv.tv_sec == time.tv_sec && atv.tv_usec <= time.tv_usec))
    {
        RPC_DBG_PRINTF(rpc_e_dbg_last_switch, 1,
            ("pthread_cond_timedwait: timeout prior to sleep, ttbl=%x\n",
                ttbl));
        splx(s);

        PTHREAD_MUTEX_LOCK_INTERNAL(timedwait_m);
        ttbl->thread = NULL;
        PTHREAD_MUTEX_UNLOCK_INTERNAL(timedwait_m);

        return (-1);
    }

    RPC_DBG_PRINTF(rpc_e_dbg_last_switch, 3,
        ("pthread_cond_timedwait: scheduling %d sec timeout, ttbl=%x\n",
            tmo/hz, ttbl));

    timeout(pthread__timedwait_wakeup, (caddr_t) ttbl, tmo);

    *condition |= PTHREAD_COND_WAITING;
    *condition &= ~PTHREAD_COND_TIMEOUT;
    PTHREAD_MUTEX_UNLOCK_INTERNAL(*mutex);

    sleep(condition, PTHREAD_COND_SLEEP_PRI);

    PTHREAD_MUTEX_LOCK_INTERNAL(*mutex);

    PTHREAD_MUTEX_LOCK_INTERNAL(timedwait_m);

    /*
     * Free our timeout table slot.
     */
    ttbl->thread = NULL;

    /*
     * If we timed out, tell em.  Use our per thread info to
     * determine this so that we can correctly determine if we need
     * to clear our timeout.
     */
    if (ttbl->timedout)
    {
        PTHREAD_MUTEX_UNLOCK_INTERNAL(timedwait_m);
        RPC_DBG_PRINTF(rpc_e_dbg_last_switch, 1,
            ("pthread_cond_timedwait: timeout, ttbl=%x\n", ttbl));
        splx(s);
        return (-1);
    }

    PTHREAD_MUTEX_UNLOCK_INTERNAL(timedwait_m);

    /*
     * Otherwise, cancel the timeout and let the caller recheck
     * their predicates to see if the wakeup was "real".
     */
    untimeout(pthread__timedwait_wakeup, (caddr_t) ttbl);
    splx(s);
    RPC_DBG_PRINTF(rpc_e_dbg_last_switch, 3,
        ("pthread_cond_timedwait: no timeout, ttbl=%x\n", ttbl));
    return (0);
}

/* =============================================================== */
/*
 * pthread cancel stuff...
 */

PRIVATE int pthread_cancel(thread)
pthread_t                   thread;
{
    PTHREADS_INIT

    /* NO OP; no cancel behavour support in the kernel */
}

PRIVATE int pthread_setasynccancel(mode)
int mode;
{
    PTHREADS_INIT

    /* NO OP; no cancel behavour support in the kernel */
}

PRIVATE int pthread_setcancel(mode)
int mode;
{
    PTHREADS_INIT

    /* NO OP; no cancel behavour support in the kernel */
}


PRIVATE void pthread_testcancel()
{
    /*
     * Any code that depended on this working has to be re-coded.
     */
    DIE("(pthread_testcancel)");
}

/* =============================================================== */
/*
 * Support for the exception package (see exc.h for more info).
 */

/*
 * Raise the indicated exception (unwind to the innermost handler).
 */
PRIVATE void pthread_exc_raise(e)
volatile EXCEPTION *e;
{
    pthread_exc_ctxt_t **_exc_ctxt_head = NULL;

    pthread_getspecific(pthread_exc_ctxt_key, (pthread_addr_t *)&_exc_ctxt_head);

    /*
     * Ensure that attempts to unwind past the outermost handler
     * (a programming error) are detected in an obvous fashion.
     */
    if (_exc_ctxt_head == NULL || *_exc_ctxt_head == NULL)
        DIE("attempt to unwind above top cleanup handler; fix your application");

    /*
     * Can't raise 0 (typically indicates unitialized exception).
     */
    if (e->kind != _exc_kind_status || e->match.value == 0)
        DIE("unitialized exception");

    /*
     * stash the exception in the exc_ctxt that we're unwinding to
     * so they know what caused the unwind.
     */
    (*_exc_ctxt_head)->exc = *e;

    /*
     * Unwind to the thread's innermost exception context.
     *
     * Note kernel longjmp doesn't seem to take a "value" arg
     * (they always seem to induce a setjmp return value of '1').
     * We specify one anyway, just to be clear about this.
     */
    dce_exc_longjmp(&(*_exc_ctxt_head)->jmpbuf, 1);
}

/*
 * Get a pointer to the per-thread exc context storage
 * and initialize the prev pointer to its current value.
 * If the per-thread exc context hasn't yet been allocated,
 * now's the time to do it.
 */
PRIVATE void pthread_exc_setup(_exc_ctxt_head, prev_exc_ctxt)
volatile pthread_exc_ctxt_t ***_exc_ctxt_head;
pthread_exc_ctxt_t **prev_exc_ctxt;
{
    pthread_getspecific(pthread_exc_ctxt_key, _exc_ctxt_head);
    if (*_exc_ctxt_head == NULL) {
        RPC_MEM_ALLOC(*_exc_ctxt_head, pthread_exc_ctxt_t **, sizeof *_exc_ctxt_head, 
            rpc_c_mem_jmpbuf_head, rpc_c_mem_waitok);
        if (*_exc_ctxt_head == NULL)
            DIE("(pthread_exc_setup)");
        **_exc_ctxt_head = NULL;
        pthread_setspecific(pthread_exc_ctxt_key, *_exc_ctxt_head);
    }

    *prev_exc_ctxt = **_exc_ctxt_head;
}

/*
 * Destroy the per-thread exception state context; i.e.
 * free the per-thread storage and cause the per-thread
 * key context storage to be freed.
 */
PRIVATE void pthread_exc_destroy(_exc_ctxt_head)
volatile pthread_exc_ctxt_t **_exc_ctxt_head;
{
    RPC_MEM_FREE(_exc_ctxt_head, rpc_c_mem_jmpbuf_head);
    pthread_setspecific(pthread_exc_ctxt_key, NULL);
}

INTERNAL void pthread_exc_ctxt_free(context_value)
pthread_addr_t context_value;
{
    RPC_MEM_FREE(context_value, rpc_c_mem_jmpbuf_head);
}

/* =============================================================== */
/*
 * pthread_*_np() stuff...
 */

PRIVATE int pthread_get_expiration_np(delta_ts, abs_ts)
struct timespec *delta_ts;
struct timespec *abs_ts;
{
    int s;

    PTHREADS_INIT

    /*
     * Compute an absolute timespec
     */
    s = splhigh();

    microtime((struct timeval *)abs_ts);
    abs_ts->tv_nsec *= 1000;

    abs_ts->tv_nsec += delta_ts->tv_nsec;
    if (abs_ts->tv_nsec >= 1000000000)
    {
        abs_ts->tv_nsec -= 1000000000;
        abs_ts->tv_sec++;
    }
    abs_ts->tv_sec += delta_ts->tv_sec;

    splx(s);
}


/*
 * This implementation of pthread_delay_np() is skewed towards
 * a small number of threads concurrently delaying.  Threads
 * block on a per-thread cv (so that cuncurrent delayers aren't
 * prematurely awakened by other waiter's time expirations).
 * Since real per-thread storage is rather inefficient with
 * this implementation, we just use a small lightweight
 * table to achieve the same effect (a.k.a. hack).
 */

typedef struct {
    pthread_t   thread;
    pthread_cond_t cv;          /* cv to delay on */
} pthread__delay_t;

#ifndef PTHREAD_DELAY_TABLE_SIZE
#  define PTHREAD_DELAY_TABLE_SIZE    5
#endif

INTERNAL pthread__delay_t delay_tbl[PTHREAD_DELAY_TABLE_SIZE];
INTERNAL pthread_mutex_t delay_m;

INTERNAL void pthread__delay_init()
{
    pthread__delay_t        *dtbl;

    pthread_mutex_init(&delay_m, pthread_mutexattr_default);

    for (dtbl = delay_tbl; 
        dtbl < &delay_tbl[PTHREAD_DELAY_TABLE_SIZE]; dtbl++)
    {
        dtbl->thread = NULL;
        pthread_cond_init(&dtbl->cv, pthread_condattr_default);
    }
}

PRIVATE int pthread_delay_np(interval)
struct timespec *interval;
{
    pthread__delay_t        *dtbl;
    struct timespec         abs_ts;     /* absolute timespec */

    PTHREADS_INIT

    PTHREAD_MUTEX_LOCK_INTERNAL(delay_m);

    /*
     * Locate a delay table entry to use.
     */
    for (dtbl = delay_tbl;
        dtbl < &delay_tbl[PTHREAD_DELAY_TABLE_SIZE]; dtbl++)
    {
        if (dtbl->thread == NULL)
        {
            dtbl->thread = pthread_self();
            break;
        }
    }
    if (dtbl >= &delay_tbl[PTHREAD_DELAY_TABLE_SIZE])
    {
        DIE("delay table exhausted");
        /* 
         * make the table bigger, have "cheap" per-thread delay
         * storage, ...  vendors choice...
         */
    }

    /*
     * Convert to a useable absolute time.
     */
    pthread_get_expiration_np(interval, &abs_ts);

    /*
     * Block until the absolute time is reached...
     */
    while (pthread_cond_timedwait(&dtbl->cv, &delay_m, &abs_ts) == 0)
        ;

    /*
     * Free the table entry.
     */
    dtbl->thread = NULL;

    PTHREAD_MUTEX_UNLOCK_INTERNAL(delay_m);
}

