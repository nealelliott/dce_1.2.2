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
 * Revision 1.1.78.2  1996/02/18  00:00:32  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:53:42  marty]
 *
 * Revision 1.1.78.1  1995/12/08  00:14:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:56:11  root]
 * 
 * Revision 1.1.76.2  1994/05/19  21:14:13  hopkins
 * 	Serviceability work
 * 	[1994/05/19  02:17:57  hopkins]
 * 
 * 	Serviceability:
 * 	  Change refs to rpc_e_dbg_last_switch to
 * 	  rpc_e_dbg_threads.
 * 	[1994/05/18  21:33:28  hopkins]
 * 
 * Revision 1.1.76.1  1994/01/22  22:49:46  cbrooks
 * 	RPC Code Cleanup - CR 9797
 * 	[1994/01/22  22:46:00  cbrooks]
 * 
 * Revision 1.1.6.5  1993/03/04  16:46:56  delgado
 * 	New thread_boot code from IBM and Transarc to save state
 * 	when krpc gets exceptions.
 * 	[1993/03/04  16:44:20  delgado]
 * 
 * Revision 1.1.6.4  1993/01/03  22:36:01  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:52:01  bbelch]
 * 
 * Revision 1.1.6.3  1992/12/23  19:38:28  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  14:52:45  zeliff]
 * 
 * Revision 1.1.6.2  1992/12/17  16:33:07  markar
 * 	     OT CR 6304 fix: fix 100 year hang in pthrea_delay_np()
 * 	[1992/12/07  16:16:25  markar]
 * 
 * Revision 1.1.2.2  1992/03/10  18:26:58  paul
 * 	Changed names of listener and timer threads to "LSN" and "TMR" respecitively.
 * 	Other threads still have names of "thr." where "." changes with each call to
 * 	pthread_create().
 * 	[1992/03/10  17:21:30  paul]
 * 
 * 	Added return statement to thread_boot() after clrjmpx call so kproc
 * 	will exit rather than DIE.
 * 	[1992/02/27  21:48:22  paul]
 * 
 * Revision 1.1  1992/01/19  03:15:34  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
**  Copyright (c) 1989, 1990 by
**      Hewlett-Packard Company, Palo Alto, Ca.
**
**  %a%copyright(,**  )
**
**  NAME:
**
**      kpthread.c
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  See pthread.h
**
**  %a%private_begin
**
**  MODIFICATION HISTORY: 
** 
**  22-apr-91 labossiere    removed some extraneous includes; portabilize longjmp
**  16-apr-91 labossiere    cond_timedwait(): hzto() portability rework
**                                            pre-sleep bugfix (free table entry)
**                                            make thread/multicpu safe
**                          pthread_delay_np(): rework for per-thread cv's
**  02-apr-91 labossiere    exc.h -> exc_handling.h
**                          thread creation work: create "detached" threads;
**                              threads exit upon completion
**  31-jan-91 labossiere    add pthread_keycreate signature fix
**  28-jan-91 labossiere    pthread_exc_ rework for pthread_exc_ctxt_t
**  18-jan-91 labossiere    various fixes for per thread stg,
**                          add cond_timedwait() (rework delay_np)
**  07-jan-90 nacey         Add pthread_keycreate() call to pthread__init()
**  04-jan-90 nacey         - Change pthread_cancel() from DIE to NO-OP
**                          - Change pthread_testcancel() from NO-OP to DIE
**  18-dec-90 nacey         Add NO-OP versions of
**                           pthread_setasynccancel(), pthread_setcancel()
**                           and pthread_testcancel()
**  27-nov-90 nacey         Add a NO-OP version of pthread_cond_timedwait()
**  21-nov-90 nacey         P1003.4a/D4 Additions
**                            - Add pthread_attr_create/delete
**  04-oct-90 nacey         Add pthread_cond_broadcast()
**  31-jul-90 nacey         Change mutex and condition function names
**                          to conform to CMA/pthreads bl03.
**                          Fix various CMA bl03 incompatabilities.
**  25-jul-90 labossiere    Update for CMA bl03 level of Pthreads API
**  11-jun-90 nacey         Adapted from osc.OLD/cma.c
**  30-apr-90 labossiere    Initial version 
**
**  %a%private_end  
**
*/

#include <commonp.h>
#include <dce/ker/exc_handling.h>

#include <sys/sleep.h>
#include <sys/param.h>
#include <sys/systm.h>
#include <sys/user.h>       /* need the u area for a thread handle */
#include <sys/proc.h>       /* need for u.u_procp.c_cptr */
                                               

lock_t tctx_lock = LOCK_AVAIL;
char thread_nam[4+1] = "thr ";

/*
 * pthread_nameseed -   seed an initial name to be used for threads
 */
pthread_nameseed(seed)
char *seed; {

    bcopy(seed, thread_nam, 4);
    thread_nam[3] = ' ';
}

/*
 * calcdiff -   calculate time difference from now
 */
static
calcdiff(tt)
struct timestruc_t  *tt;
{
    int opri;

    opri = i_disable(INTTIMER);
    tt->tv_sec  -= tod.tv_sec;
    tt->tv_nsec -= tod.tv_nsec;
    i_enable(opri);
    return (tt->tv_sec * 100 + tt->tv_nsec / 10000000);
}


/* ================================================================= */

EXCEPTION exc_e_alerted;

pthread_attr_t          pthread_attr_default;
pthread_mutexattr_t     pthread_mutexattr_default;
pthread_condattr_t      pthread_condattr_default;

/* ================================================================= */


#define PTHREADS_INIT \
 {\
    static lock_t monitor = LOCK_AVAIL; \
    int rv; \
    \
    rv = lockl(&monitor, LOCK_SHORT); \
    if (pthreads__initd == 0) \
        pthread__init(); \
    if (rv != LOCK_NEST) \
       unlockl(&monitor); \
}

INTERNAL short pthreads__initd = 0;

/* ----------------------------------------------------------------- */

/*
 * "once" stuff...
 */
#define PTHREAD_ONCE_INITD          0x01
#define PTHREAD_ONCE_INPROGRESS     0x02

/* =============================================================== */

/*
 * exception package grunge...
 */
INTERNAL pthread_key_t pthread_exc_ctxt_key;

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

/*----------------------------------------------------------------------------*/

INTERNAL void pthread__init
    _DCE_PROTOTYPE_((void));


INTERNAL void pthread__delay_wakeup
    _DCE_PROTOTYPE_((
        caddr_t arg
    ));

INTERNAL void pthread__ctx_init
    _DCE_PROTOTYPE_((void));

INTERNAL pthread_thread_ctx_t *pthread__thread_ctx_get
    _DCE_PROTOTYPE_((
        pthread_key_t               key
    ));

INTERNAL pthread_thread_ctx_t *pthread__thread_ctx_lookup
    _DCE_PROTOTYPE_((
        pthread_key_t               key
    ));

INTERNAL void pthread__thread_ctx_free
    _DCE_PROTOTYPE_((
        pthread_thread_ctx_t            *ctx
    ));

INTERNAL void pthread__delay_init
    _DCE_PROTOTYPE_((void));


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
    /*
     * No need to use a mutex in pthread_create routine
     * We are not using exception context key, instead
     * use setjmpx, longjmpx and clrjmpx services.
     */
    /*
    pthread_mutex_init(&create_m, pthread_mutexattr_default);
    pthread_keycreate(&pthread_exc_ctxt_key, pthread_exc_ctxt_free);
    */

    EXCEPTION_INIT(exc_e_alerted);

    /*
    pthread_mutex_init(&timedwait_m, pthread_mutexattr_default);
    pthread__delay_init();
    */
}
   
/* =============================================================== */

/*
 * "once" operations.
 *
 * This implementation works for preemptive thread
 * environment and multi cpu environment.
 */

PRIVATE int pthread_once(once_block, init_routine)
pthread_once_t              *once_block;
pthread_initroutine_t       init_routine;
{
    int rv;
    static lock_t lock_var = LOCK_AVAIL;

    PTHREADS_INIT;
    rv = lockl(&lock_var, LOCK_SHORT);

    if (*once_block & PTHREAD_ONCE_INITD)
    {
        if (rv != LOCK_NEST)
          unlockl(&lock_var);
        return (0);
    }

    /*
     * This test should really be:
     *      if one init is inprogress
     *          wait until the inprogress one completes
     *          return
     * Since this condition is very unlikely (at least initially)
     * this hack will at least protect us.
     */

    if (*once_block & PTHREAD_ONCE_INPROGRESS)
    {
        if (rv != LOCK_NEST)
           unlockl(&lock_var);
        DIE("(pthread_once)");
    }

    *once_block |= PTHREAD_ONCE_INPROGRESS;
    (*init_routine) ();
    *once_block &= ~PTHREAD_ONCE_INPROGRESS;
    *once_block |= PTHREAD_ONCE_INITD;

    if (rv != LOCK_NEST)
       unlockl(&lock_var);
    return (0);
}

/* =============================================================== */

/*
 * Generic Thread operations.
 */

static void thread_boot();
struct args {
    void (*func)();
    void *arg;
};

EXTERNAL *lthread();
EXTERNAL *timer_loop();

PRIVATE int pthread_create(new_thread, attr, start_routine, arg)
pthread_t                   *new_thread;
pthread_attr_t              attr;
pthread_startroutine_t      start_routine;
pthread_addr_t              arg;
{
    int pid;
    struct args args;

    PTHREADS_INIT

    /*
     * Create a new "detached" process (one that won't hang around
     * as a zombie forever when it exits).  You'd like to think that
     * there was an easier way to achieve this... if you can figure
     * out how (in a reasonably portable way) go for it!
     */

    args.func = (void (*)()) start_routine;
    args.arg = arg;

    if ((pid = creatp()) == (pid_t) -1)
        panic("creatp");
    if ((char *)start_routine == (char *)&lthread) {
      initp(pid, thread_boot, &args, sizeof (args), "LSN");
      *new_thread = "LSN";
    } else {
      if ((char *)start_routine == (char *)&timer_loop) {
        initp(pid, thread_boot, &args, sizeof (args), "TMR");
        *new_thread = "TMR";
      } else {
        initp(pid, thread_boot, &args, sizeof (args), thread_nam);
        *new_thread = thread_nam;
        if ((thread_nam[3] += 1) > 0x7e) {
          thread_nam[2] += 2; /* XXX: not expected to wrap!   */
          thread_nam[3] = ' ';
        } else
          thread_nam[3] += 1;
      }
    }
    return(0);
}


/*
 * this routine is the `_start' analogue for processes.  it is the first piece
 * of code to run in the new context.
 */

static void
thread_boot(flag, argp, argl)
struct args *argp;
{
TRCHKL5T(HKWD_USERE,0x00000001,argp, argp->func,argp->arg,NULL);
            (*argp->func)(argp->arg);   /* invoke the code to run       */
TRCHKL5T(HKWD_USERE,0x00000002,argp, argp->func,argp->arg,NULL);

    return;
}



PRIVATE pthread_t pthread_self()
{
    PTHREADS_INIT

    return((pthread_t) u.u_procp);
}
  
PRIVATE void pthread_yield()
{
    /*
     * !!! It isn't imperative that yield actually do something, however
     * it would probably be good (for performance reasons) if it did.
     */
}


PRIVATE int pthread_detach(thread)
pthread_t                   *thread;
{
    PTHREADS_INIT

    /* NO OP */
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
    long                    rv;

    assert((key >= 1 && key <= KEY_CTX_TABLE_SIZE));

    rv = lockl(&tctx_lock, LOCK_SHORT);
    thread = pthread_self();
    probe = thread_hash(thread);
    tctx_head = (pthread_thread_ctx_t *) &thread_ctx[probe];

    /*
     * Search for the thread specific key context using the
     * thread to compute a hash.
     */

    RPC_DBG_PRINTF(rpc_e_dbg_threads, 5,
        ("pthread__thread_ctx_get: key=%d thread=%x probe=%x\n",
        key, thread, probe));

    for (tctx = tctx_head->c.forw; tctx != tctx_head; tctx = tctx->c.forw)
    {
        RPC_DBG_PRINTF(rpc_e_dbg_threads, 10,
            ("pthread__thread_ctx_get: comparing tctx=%x\n", tctx));
        if (tctx->thread == thread && tctx->key == key)
        {
            RPC_DBG_PRINTF(rpc_e_dbg_threads, 5,
                ("pthread__thread_ctx_get: key=%d thread=%x found tctx=%x value=%x\n",
                key, thread, tctx, tctx->context_value));
            if (rv != LOCK_NEST)
               unlockl(&tctx_lock);
            return (tctx);
        }
    }

    /*
     * Not found; Create and initialize a thread context descriptor.
     * Insert at the head of the doubly linked list.
     */

    RPC_MEM_ALLOC(tctx, pthread_thread_ctx_t *, sizeof *tctx, RPC_C_MEM_PTHREAD_CTX, 
        RPC_C_MEM_WAITOK);
    if (tctx == NULL)
    {
       if (rv != LOCK_NEST)
          unlockl(&tctx_lock);
        DIE("(pthread__thread_ctx_get)");
    }

    RPC_DBG_PRINTF(rpc_e_dbg_threads, 5,
        ("pthread__thread_ctx_get: key=%d thread=%x creating tctx=%x\n",
        key, thread, tctx));

    tctx->thread    = thread;
    tctx->key       = key;
    tctx->context_value = NULL;

    tctx->c.forw    = tctx_head->c.forw;
    tctx->c.back    = tctx_head;
    tctx_head->c.forw->c.back   = tctx;
    tctx_head->c.forw           = tctx;

   if (rv != LOCK_NEST)
      unlockl(&tctx_lock);
    return (tctx);
}

/*
 * Free the thread context for this key.
 */

INTERNAL void pthread__thread_ctx_free(tctx)
pthread_thread_ctx_t            *tctx;
{
    int rv;

    rv = lockl(&tctx_lock, LOCK_SHORT);
    RPC_DBG_PRINTF(rpc_e_dbg_threads, 5,
        ("pthread__thread_ctx_free: key=%d thread=%x tctx=%x\n",
        tctx->key, tctx->thread, tctx));

    tctx->c.back->c.forw = tctx->c.forw;
    tctx->c.forw->c.back = tctx->c.back;

    RPC_MEM_FREE(tctx, RPC_C_MEM_PTHREAD_CTX);
    if (rv != LOCK_NEST)
       unlockl(&tctx_lock);
}

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
    int                     rv;

    assert((key >= 1 && key <= KEY_CTX_TABLE_SIZE));

    rv = lockl(&tctx_lock, LOCK_SHORT);
    thread = pthread_self();
    probe = thread_hash(thread);
    tctx_head = (pthread_thread_ctx_t *) &thread_ctx[probe];

    /*
     * Search for the thread specific key context using the
     * thread to compute a hash.
     */

    RPC_DBG_PRINTF(rpc_e_dbg_threads, 5,
        ("pthread__thread_ctx_lookup: key=%d thread=%x probe=%x\n",
        key, thread, probe));

    for (tctx = tctx_head->c.forw; tctx != tctx_head; tctx = tctx->c.forw)
    {
        RPC_DBG_PRINTF(rpc_e_dbg_threads, 10,
            ("pthread__thread_ctx_lookup: comparing tctx=%x\n", tctx));
        if (tctx->thread == thread && tctx->key == key)
        {
            RPC_DBG_PRINTF(rpc_e_dbg_threads, 5,
            ("pthread__thread_ctx_lookup: key=%d thread=%x found tctx=%x value=%x\n",
                key, thread, tctx, tctx->context_value));
            if (rv != LOCK_NEST)
               unlockl(&tctx_lock);
            return (tctx);
        }
    }

    RPC_DBG_PRINTF(rpc_e_dbg_threads, 5,
        ("pthread__thread_ctx_lookup: key=%d thread=%x no tctx\n",
        key, thread));
    if (rv != LOCK_NEST)
       unlockl(&tctx_lock);
    return (NULL);
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

    *new_mutex = LOCK_AVAIL;

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
    lockl(mutex, LOCK_SHORT);

    return (0);
}

PRIVATE int pthread_mutex_trylock(mutex)
pthread_mutex_t             *mutex;
{
    int rc;

    rc = lockl(mutex, LOCK_NDELAY);
    return (rc == LOCK_SUCC);
}

PRIVATE int pthread_mutex_unlock(mutex)
pthread_mutex_t             *mutex;
{
    unlockl(mutex);

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
    new_condition->queue = EVENT_NULL;
    new_condition->var = 0;

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
    e_wakeup((int *)&condition->queue);

    return (0);
}

PRIVATE int pthread_cond_broadcast(condition)
pthread_cond_t              *condition;
{
    e_wakeup((int *)&condition->queue);

    return (0);
}


PRIVATE int pthread_cond_wait(condition, mutex)
pthread_cond_t              *condition;
pthread_mutex_t             *mutex;
{

    e_sleepl(mutex, (int *)&condition->queue, EVENT_SHORT);
    return (0);
}


/*
 * This routine is called when you need to make an interruptible sleep.
 */

PRIVATE int pthread_cond_wait_intr_np(condition, mutex)
pthread_cond_t              *condition;
pthread_mutex_t             *mutex;
{
    int error;

    error = e_sleepl(mutex, (int *)&condition->queue, EVENT_SIGRET);
    if (error == EVENT_SUCC)
       return 0;
    else if (error == EVENT_SIG)
       return 1;
}



INTERNAL void pthread__timedwait_wakeup(trb)
register struct trb * trb;
{
   e_post(EVENT_SHARED, trb->pid);
}

PRIVATE int pthread_cond_timedwait(condition, mutex, abs_time)
pthread_cond_t              *condition;
pthread_mutex_t             *mutex;
struct timespec             *abs_time;
{
    int                     s;
    int                     tmo;
    struct timeval          atv;         /* absolute timeval */
    int opri;
    register struct trb *trb;
    struct timespec later = *abs_time;


    if ((tmo = calcdiff(&later)) <= 0) {
        return -1;
    }

    trb = talloc();
    if (trb == NULL)
       panic(" talloc returned a null trb");

    /*
     *  Ensure that this is treated as an incremental timer, not an
     *  absolute one.
     */
    trb->timeout.it_value = *(struct timestruc_t *) &later;
    trb->flags           &= ~(T_ABSOLUTE);
    trb->func             =  (void (*)()) pthread__timedwait_wakeup;
    trb->eventlist        =  EVENT_NULL;
    trb->pid              =  getpid();
    trb->func_data        =  (uint) trb;
    trb->ipri             =  INTTIMER;

    pin(mutex, sizeof(pthread_mutex_t));
    pin(condition, sizeof(pthread_cond_t));
    opri = i_disable(INTTIMER);
    tstart(trb);
    e_sleepl(mutex, (int *)&condition->queue, EVENT_SHORT);
    i_enable(opri);
    unpin(mutex, sizeof(pthread_mutex_t));
    unpin(condition, sizeof(pthread_cond_t));
    tstop(trb);
    tfree(trb);

    /*
     * If we timed out, tell em.
     */
    later = *abs_time;
    if ((tmo = calcdiff(&later)) <= 0) {
        return -1;
    }

    return 0;
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
 * pthread_*_np() stuff...
 */

PRIVATE int pthread_get_expiration_np(delta_ts, abs_ts)
struct timespec *delta_ts;
struct timespec *abs_ts;
{
    int s;
    int opri;

    PTHREADS_INIT

    /*
     * Compute an absolute timespec
     */
    opri = i_disable(INTTIMER);
    abs_ts->tv_nsec = tod.tv_nsec + delta_ts->tv_nsec;
    abs_ts->tv_sec = tod.tv_sec + delta_ts->tv_sec;
    i_enable(opri);
    if (abs_ts->tv_nsec >= 1000000000)
    {
        abs_ts->tv_nsec -= 1000000000;
        abs_ts->tv_sec++;
    }

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


INTERNAL pthread_mutex_t delay_m;
INTERNAL pthread_cond_t  delay_cv;
INTERNAL boolean  delay_setup = false;
INTERNAL int delayers = 0;

PRIVATE int pthread_delay_np(interval)
struct timespec *interval;
{
    struct timespec         abs_ts;     /* absolute timespec */

    PTHREADS_INIT

    /*
     * Convert to a useable absolute time.
     */
    pthread_get_expiration_np(interval, &abs_ts);

    /*
     * Ensure that the appropriate internal delay vars are setup
     */
    if (!delay_setup)
    {
        pthread_cond_init(&delay_cv, pthread_condattr_default);
        pthread_mutex_init(&delay_m, pthread_mutexattr_default);
        delay_setup = true;
    }

    /*
     * Block until the absolute time is reached...
     */
    pthread_mutex_lock(&delay_m);
    delayers++;
    while (pthread_cond_timedwait(&delay_cv, &delay_m, &abs_ts) == 0)
        ;
    delayers--;
    pthread_mutex_unlock(&delay_m);
}

