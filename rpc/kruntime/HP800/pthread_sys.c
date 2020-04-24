/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * @HP_COPYRIGHT@
 */
/*
 * HISTORY
 * $Log: pthread_sys.c,v $
 * Revision 1.1.80.2  1996/02/18  00:00:05  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:53:27  marty]
 *
 * Revision 1.1.80.1  1995/12/08  00:14:04  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/3  1995/09/14  20:04 UTC  maunsell_c
 * 	merge Walnut Creek changes to HPDCE02 mainline
 * 
 * 	HP revision /main/HPDCE02/maunsell_WC_port/12  1995/09/12  13:00 UTC  maunsell_c
 * 	use kthreadp not procp for pthread_self return
 * 
 * 	HP revision /main/HPDCE02/maunsell_WC_port/11  1995/08/22  17:48 UTC  maunsell_c
 * 	use krpc_ vs. osi_ calls
 * 
 * 	HP revision /main/HPDCE02/maunsell_WC_port/10  1995/08/16  15:27 UTC  maunsell_c
 * 	add osi.h include for non-Walnut Creek builds
 * 
 * 	HP revision /main/HPDCE02/maunsell_WC_port/9  1995/08/16  15:11 UTC  maunsell_c
 * 	kthread ct in pthread_create should be Walnut Creek only
 * 
 * 	HP revision /main/HPDCE02/maunsell_WC_port/8  1995/08/15  22:03 UTC  maunsell_c
 * 	use pstat_fork after newproc in pthread_create
 * 
 * 	HP revision /main/HPDCE02/maunsell_WC_port/7  1995/08/15  21:16 UTC  maunsell_c
 * 	findpregtype changed, now needs kthreadp arg also
 * 
 * 	HP revision /main/HPDCE02/maunsell_WC_port/6  1995/08/15  21:04 UTC  maunsell_c
 * 	prepare_newproc is Walnut Creek only
 * 
 * 	HP revision /main/HPDCE02/maunsell_WC_port/5  1995/08/15  20:37 UTC  maunsell_c
 * 	use prepare_newproc prior to newproc in pthread_create
 * 
 * 	HP revision /main/HPDCE02/maunsell_WC_port/4  1995/08/14  19:39 UTC  maunsell_c
 * 	use osi_xxxx macros vs. 1010p ifdef's
 * 
 * 	HP revision /main/HPDCE02/maunsell_WC_port/3  1995/08/07  12:48 UTC  maunsell_c
 * 	fix nested ifdef using 1010p
 * 
 * 	HP revision /main/HPDCE02/2  1994/08/09  18:33 UTC  kinney
 * 	In pthread_create(), dis-associate controlling tty.
 * 
 * 	HP revision /main/HPDCE02/1  1994/08/03  16:33 UTC  tatsu_s
 * 	Merged mothra up to dce 1.1.
 * 
 * 	HP revision /main/HPDCE01/4  1994/06/09  13:06 UTC  kissel
 * 	Merge in MP-safe bugfix changes.
 * 
 * 	HP revision /main/HPDCE01/kissel_mpsafe_bugs/2  1994/06/08  18:02 UTC  kissel
 * 	Fix the SPINLOCK calls because the _USAV versions are not available on the 700 at 9.x.
 * 
 * 	HP revision /main/HPDCE01/kissel_mpsafe_bugs/1  1994/06/07  23:06 UTC  kissel
 * 	Fix MP_PXSEMA and MP_VXSEMA since these are not defined for the 700 at 9.x.  The
 * 	ones that are defined, PXSEMA and VXSEMA, are obsolete for the 800 at 9.x and both
 * 	machines at 10.x.
 * 
 * 	HP revision /main/HPDCE01/3  1994/05/26  13:29 UTC  kissel
 * 	Merge from the kissel_mpsafe branch to add locking to make this code MP-safe.
 * 
 * 	HP revision /main/HPDCE01/2  1994/05/03  18:12 UTC  mort
 * 	Rodan: remove KPREEMPTPOINT
 * 	also, remove hp9000_s300 code snippit
 * 	[1995/12/07  23:55:44  root]
 * 
 * Revision 1.1.10.4  1993/11/19  18:51:19  robinson
 * 	Make the kernel "thread" processes true kernel processes
 * 	so that they won't be added to the load average.
 * 	[1993/11/19  18:47:02  robinson]
 * 
 * Revision 1.1.10.3  1993/11/17  16:51:42  robinson
 * 	Make update_duxref() call contingent on releases prior to HP/UX 10.
 * 	[1993/11/17  16:50:01  robinson]
 * 
 * Revision 1.1.8.3  1993/09/15  15:25:12  root
 * 	    Loading drop DCE1_0_3b03
 * 
 * Revision 1.1.78.4  1994/06/10  20:54:12  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:59:56  devsrc]
 * 
 * Revision 1.1.78.3  1994/05/19  21:14:10  hopkins
 * 	Serviceability work
 * 	[1994/05/19  02:17:22  hopkins]
 * 
 * 	Serviceability:
 * 	  Change refs to rpc_e_dbg_last_switch to
 * 	  rpc_e_dbg_threads.
 * 	[1994/05/18  21:33:02  hopkins]
 * 
 * Revision 1.1.78.2  1994/02/02  21:48:54  cbrooks
 * 	OT9855 code cleanup breaks KRPC
 * 	[1994/02/02  21:00:00  cbrooks]
 * 
 * Revision 1.1.78.1  1994/01/21  22:31:25  cbrooks
 * 	RPC Code Cleanup
 * 	[1994/01/21  20:33:02  cbrooks]
 * 
 * Revision 1.1.6.1  1993/10/15  21:57:00  kinney
 * 	pthread_create(): inherit command name in new process
 * 	[1993/10/15  21:48:24  kinney]
 * 
 * Revision 1.1.4.2  1993/06/10  19:24:48  sommerfeld
 * 	Initial HPUX RP version.
 * 	[1993/06/03  22:04:35  kissel]
 * 
 * 	Increase kernel stack size in child in pthread_create()
 * 	[1993/01/15  15:35:47  toml]
 * 
 * 	Change pthread_create for HPUX so that only one process is created.
 * 	[1992/10/27  19:37:01  kissel]
 * 
 * 	06/16/92   tmm  Created from COSL drop.
 * 	[1992/06/18  18:30:48  tmm]
 * 
 * $EndLog$
 */
/*
**  Copyright (c) 1989, 1990 by
**      Hewlett-Packard Company, Palo Alto, Ca.
**
**  NAME:
**
**      pthread.c
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  See pthread.h.  Implements pthreads interface for 4.4 bsd kernels.
**
*/

#include <commonp.h>
#include <dce/ker/exc_handling.h>
#include <dce/assert.h>

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/user.h>       /* need the u area for a thread handle */
#include <sys/kernel.h>
#if __hpux1010p
#include <sys/sem_alpha.h>
#include <sys/pregion.h>    /* used to include via proc.h */
#endif
#include <sys/proc.h>       /* need for u.u_procp.c_cptr */
#include <dce/ker/krpc_osi_mach.h>
#include <sys/vnode.h>
#include <sys/pstat.h>                                               

/*
 * This implementation is supposed to be MP safe.  It uses spinlocks
 * to protect data structure updates that can happen in interrupt
 * context (e.g.  pthread__timedwait_wakeup()).  The particular lock
 * we use is determined by looking at other HPUX code, see each use
 * for a description.  Note that <sys/spinlock.h> is included via
 * commonp.h (then sysconf.h then malloc.h, etc.).  The call side data
 * structues could be protected by the pm_sema alpha semaphore (DFS
 * uses the filesys_sema) because, again, the pthread stuff seems to
 * be related to process scheduling.  However, for simplicity, I
 * decided to use the filesys_sema for now.  I am using the semaphore
 * macros that save any previous semaphore at lock time and restore it
 * at unlock (they also do the right thing if you already hold the
 * semaphore you are now asking for).  Note that the saved semaphore
 * must be a local variable that is declared in each routine that uses
 * these macros.  Also note that all calls to
 * PTHREAD_MUTEX_LOCK_INTERNAL are surrounded by semaphore calls.  I
 * believe this makes pthread mutex locks MP safe despite the comments
 * in rpc/kruntime/pthread.h.
 *
 * There are also MP changes in kruntime/HP800/comsoc_sys.c.
 *
 * Note, the MP versions of the macros used here do nothing on a UP
 * system.  Also, the MP_ versions are the new ones, the others are
 * obsolete.  However, they are still defined at 9.x, so just test for
 * 10.x.
 */
#define PTHREAD_MP_LOCK_DECL	sv_sema_t saved_sema
#if __hpux1000p
#define PTHREAD_MP_LOCK		MP_PXSEMA(&filesys_sema, &saved_sema)
#define PTHREAD_MP_UNLOCK	MP_VXSEMA(&filesys_sema, &saved_sema)

#define PTHREAD_SPINLOCK_DECL	u_int context
#define PTHREAD_SPINLOCK	MP_SPINLOCK_USAV(sched_lock, context)
#define PTHREAD_SPINUNLOCK	MP_SPINUNLOCK_USAV(sched_lock, context)
#else
#define PTHREAD_MP_LOCK		PXSEMA(&filesys_sema, &saved_sema)
#define PTHREAD_MP_UNLOCK	VXSEMA(&filesys_sema, &saved_sema)

#define PTHREAD_SPINLOCK_DECL
#define PTHREAD_SPINLOCK	MP_SPINLOCK(sched_lock)
#define PTHREAD_SPINUNLOCK	MP_SPINUNLOCK(sched_lock)
#endif /* __hpux1000p */

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
/* Not used.  See pthread__init().
INTERNAL pthread_mutex_t create_m;
*/
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

INTERNAL void pthread__init _DCE_PROTOTYPE_((void));

INTERNAL void pthread_exc_ctxt_free _DCE_PROTOTYPE_((
        pthread_addr_t  /*context_value*/
    ));

INTERNAL void pthread__delay_wakeup _DCE_PROTOTYPE_(( caddr_t  /*arg*/));


INTERNAL void pthread__ctx_init _DCE_PROTOTYPE_((void));

INTERNAL pthread_thread_ctx_t *pthread__thread_ctx_get _DCE_PROTOTYPE_((
        pthread_key_t                /*key*/
    ));

INTERNAL pthread_thread_ctx_t *pthread__thread_ctx_lookup _DCE_PROTOTYPE_((
        pthread_key_t                /*key*/
    ));

INTERNAL void pthread__thread_ctx_free _DCE_PROTOTYPE_((
        pthread_thread_ctx_t            * /*ctx*/
    ));

INTERNAL void pthread__delay_init _DCE_PROTOTYPE_((void));


/* =============================================================== */
/*
 * Package initialization
 *
 * We assume the MP lock is held around the call to this routine.
 */
 
INTERNAL void pthread__init(void)
{
    if (pthreads__initd == 1)
        return;

    pthread__ctx_init();
    pthreads__initd = 1;
    /*
     * do this after the __initd = 1 or loop forever :-)
     */
    /* No need to use a mutex in the pthread_create routine since no pthread
     * specific data is managed.
     */
    /*
    pthread_mutex_init(&create_m, pthread_mutexattr_default);
    */
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
    pthread__delay_init();
}
   
/* =============================================================== */

/*
 * "once" operations.
 *
 * This implementation works for preemptive thread
 * environment and multi cpu environment.
 */

PRIVATE int pthread_once
#ifdef _DCE_PROTO_
(
    pthread_once_t              *once_block,
    pthread_initroutine_t       init_routine
)
#else
(once_block, init_routine)
pthread_once_t              *once_block;
pthread_initroutine_t       init_routine;
#endif
{
    PTHREAD_MP_LOCK_DECL;

    PTHREAD_MP_LOCK;
    PTHREADS_INIT;

    if (*once_block & PTHREAD_ONCE_INITD) {
	PTHREAD_MP_UNLOCK;
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
    if (*once_block & PTHREAD_ONCE_INPROGRESS) {
	PTHREAD_MP_UNLOCK;
        DIE("(pthread_once)");
    }
    *once_block |= PTHREAD_ONCE_INPROGRESS;
    PTHREAD_MP_UNLOCK;
    /*
     * We don't need to hold the lock while the routine executes since we are protected by the
     * inprogress flag.
     */
    (*init_routine) ();

    PTHREAD_MP_LOCK;
    *once_block &= ~PTHREAD_ONCE_INPROGRESS;
    *once_block |= PTHREAD_ONCE_INITD;

    PTHREAD_MP_UNLOCK;
    return (0);
}

/* =============================================================== */

/*
 * Generic Thread operations.
 */



/* ----------------------------------------------------------------- */

/*
 * No locking is needed around this routine because it is just using kernel
 * data, not pthread data.
 */
void
pthread_close_file_desc(void)
{
   int i;

/* copied from the exit code in kern_bld/sys/kern_exit.c */
        for (i = 0; i <= krpc_osi_get_highestfd(); i++) {
		struct file *f;
		extern struct file *getf_no_ue();

		f = getf_no_ue(i);
		if (f != NULL) {
			/*
			 * Guard againt u.u_error being set by psig()
			 * u.u_error should be 0 if exit is called from
			 * syscall().  In the case of exit() being called
			 * by psig - u.u_error could be set to EINTR.  We
			 * need to clear this so that routines called by
			 * exit() would't return on non-zero u.u_error.
			 * Example: vno_lockrelease()
			 */
			u.u_error = 0;
			closef(f);
			/*
			 * Save flags until after closef() because flags
			 * are used in vno_lockrelease, called from closef().
			 */
			uffree(i);
#if ! __hpux1000p
			KPREEMPTPOINT();
#endif /* ! __hpux1000p */
		}
	}
#if __hpux1000p
	{
	        PTHREAD_SPINLOCK_DECL;
		PTHREAD_SPINLOCK;
                krpc_osi_set_highestfd(-1);
		PTHREAD_SPINUNLOCK;
	}
#else /* __hpux1000p */
        krpc_osi_set_highestfd(-1);	
#endif /* __hpux1000p */

	/*
	 * Release the current directory so that it can unmounted.
	 * Release the root directory so that it can unmounted.
	 * Then because of the way exit() operates, u.u_cdir cannot be
	 * NULL, so set it to u.u_rdir as we will never umount the root
	 * filesystem.
	 */
	release_cdir();
        if (krpc_osi_get_rdir()) {
#if ! __hpux1000p
                update_duxref(krpc_osi_get_rdir(),-1,0);
#endif /* ! __hpux1000p */
                VN_RELE(krpc_osi_get_rdir());
                krpc_osi_set_rdir(NULL);
        }

        krpc_osi_set_cdir(rootdir);
        if (krpc_osi_get_cdir()) {
                VN_HOLD(krpc_osi_get_cdir());
#if ! __hpux1000p
		update_duxref(krpc_osi_get_cdir(),1,0);
#endif /* ! __hpux1000p */
	}
}

/*
 * No locking is needed around this routine because it is just using kernel
 * data, not pthread data (except, note the lock around the PTHEADS_INIT call).
 */
PRIVATE int pthread_create
#ifdef _DCE_PROTO_
(
    pthread_t                   *new_thread,
    pthread_attr_t              attr,
    pthread_startroutine_t      start_routine,
    pthread_addr_t              arg
)
#else
(new_thread, attr, start_routine, arg)
pthread_t                   *new_thread;
pthread_attr_t              attr;
pthread_startroutine_t      start_routine;
pthread_addr_t              arg;
#endif
{
    pid_t       cpid;
    proc_t      *cp;
#if __hpux1010p
    kthread_t   *ct;
#endif
    preg_t     *prp;
    PTHREAD_MP_LOCK_DECL;

    PTHREAD_MP_LOCK;
    PTHREADS_INIT
    PTHREAD_MP_UNLOCK;

#if !__hpux1010p
    /*
     * Just do roughly what kern_bld/sys/kern_fork.c does.  A process created as FORK_DAEMON has
     * proc 0 for its parent, and when it exits it gets made a child of proc 1 (init), who will
     * take care of the zombie (see kern_bld/sys/kern_exit.c).
     */
    if ((cpid = getnewpid()) < 0) {
	return (EAGAIN);
    }
    if ((cp = allocproc(S_DONTCARE)) == (proc_t *)NULL) {
	return (EAGAIN);			/* Oops... */
    }
    proc_hash(cp, 0, cpid, PGID_NOT_SET, SID_NOT_SET); /* This cannot fail (see kern_fork.c). */
#else
    prepare_newproc(&cp, S_DONTCARE, (pid_t) -1, &ct, S_DONTCARE, (tid_t) -1, (uid_t) 0, PGID_NOT_SET, SID_NOT_SET);

#endif
    /*
     * Copy "our" command/u_comm strings to the new thread
     */
    pstat_fork(u.u_procp, cp);

#if __hpux1010p
    switch (newproc(FORK_DAEMON, cp, ct)) {
#else
    switch (newproc(FORK_DAEMON, cp)) {
#endif
      case FORKRTN_CHILD:
	/*
	 * Close all file descriptors and release the current directory.  We don't need the MP
	 * lock around this call since it only messes with OS data, no pthread data.
	 */
	pthread_close_file_desc();
	
	/*
	 * Increase kernel stack size.
	 */ 
#if __hpux1010p
        prp = findpregtype(p_vas(cp), PT_UAREA, ct);
#else
	prp = findpregtype(cp->p_vas, PT_UAREA);
#endif
	reglock(prp->p_reg);
	if (growpreg(prp, 1, 1, DBD_DFILL, ADJ_REG) < 0) 
		panic("pthread_create: could not grow user stack");
	/*
	 * make us a kernel process:
	 */
#if __hpux1010p
        set_system_proc(cp);
#else
        cp->p_flag |= SSYS;
#endif
	regrele(prp->p_reg);

	/*
	 * Lose our controlling tty ... so that uprintf() calls don't end up
	 * on some random user's window.
	 */
        krpc_osi_set_ttyp(NULL);
        krpc_osi_set_ttyd(NODEV);


	/*
	 * The TRY / CATCH is setup to make it so threads created internally
	 * don't have to keep freeing their per thread exception context
	 * when we know that they won't exit out from underneath us....
	 * This just lessens the cost of of TRY / CATCH for call executor
	 * threads (at the cost of some stack space).
	 */
	TRY {
	    (*start_routine)(arg);
	} CATCH_ALL {
	    RPC_DBG_GPRINTF(("(pthread_create) thread unwound...\n"));
	} ENDTRY
        exit(0);

      case FORKRTN_PARENT:
	*new_thread = cp;
	return (0);

      case FORKRTN_ERROR:
#if __hpux1010p
        cleanup_failed_newproc(cp, ct);
#endif
	return (ENOMEM);

      default:
	panic("pthread_create: Unknown return value of newproc");
    }
    return (0);
}

#ifdef NOTDEF
PRIVATE int pthread_join
#ifdef _DCE_PROTO_
(
    thread_t                   thread,
    pthread_addr_t              *status
)
#else
(thread, status) 
thread_t                   thread;
pthread_addr_t              *status;
#endif
{
    PTHREAD_MP_LOCK_DECL;

    PTHREAD_MP_LOCK;
    PTHREADS_INIT
    PTHREAD_MP_UNLOCK;

    DIE("(pthread_join)");
}
#endif /* NOTDEF */


PRIVATE int pthread_detach
#ifdef _DCE_PROTO_
(
    pthread_t                   *thread
)
#else
(thread)
pthread_t                   *thread;
#endif
{
    PTHREAD_MP_LOCK_DECL;

    PTHREAD_MP_LOCK;
    PTHREADS_INIT
    PTHREAD_MP_UNLOCK;

    /* NO OP */
    return (0);
}


PRIVATE pthread_t pthread_self(void)
{
    PTHREAD_MP_LOCK_DECL;

    PTHREAD_MP_LOCK;
    PTHREADS_INIT
    PTHREAD_MP_UNLOCK;
#if __hpux1010p
    return((pthread_t) u.u_kthreadp);
#else
    return((pthread_t) u.u_procp);
#endif
}

PRIVATE void pthread_yield(void)
{
    PTHREAD_MP_LOCK_DECL;

    PTHREAD_MP_LOCK;
    PTHREADS_INIT
    PTHREAD_MP_UNLOCK;
}

PRIVATE int pthread_attr_create
#ifdef _DCE_PROTO_
(
    pthread_attr_t *attr
)
#else
(attr)
pthread_attr_t *attr;
#endif
{
    PTHREAD_MP_LOCK_DECL;

    PTHREAD_MP_LOCK;
    PTHREADS_INIT

    *attr = pthread_attr_default;

    PTHREAD_MP_UNLOCK;
    return (0);
}

PRIVATE int pthread_attr_destroy
#ifdef _DCE_PROTO_
(
    pthread_attr_t *attr
)
#else
(attr)
pthread_attr_t *attr;
#endif
{
    PTHREAD_MP_LOCK_DECL;

    PTHREAD_MP_LOCK;
    PTHREADS_INIT
    PTHREAD_MP_UNLOCK;

    /* NO-OP */
    return (0);
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

/*
 * This routine is only called from pthread__init(), and it expects the MP lock
 * to be held.
 */

INTERNAL void pthread__ctx_init(void)
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
 *
 * This routine is only called from pthread_setspecific() and it expects the MP lock
 * to be held.
 */

INTERNAL pthread_thread_ctx_t *pthread__thread_ctx_get
#ifdef _DCE_PROTO_
(
    pthread_key_t               key
)
#else
(key)
pthread_key_t               key;
#endif
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
        DIE("(pthread__thread_ctx_get)");

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

    return (tctx);
}


/*
 * Lookup the thread context for this key.
 *
 * This routine is only called from pthread_getspecific() and it expects the MP lock
 * to be held.
 */

INTERNAL pthread_thread_ctx_t *pthread__thread_ctx_lookup
#ifdef _DCE_PROTO_
(
    pthread_key_t               key
)
#else
(key)
pthread_key_t               key;
#endif
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
            return (tctx);
        }
    }

    RPC_DBG_PRINTF(rpc_e_dbg_threads, 5,
        ("pthread__thread_ctx_lookup: key=%d thread=%x no tctx\n",
        key, thread));
    return (NULL);
}


/*
 * Free the thread context for this key.
 *
 * This routine is only called from pthread_setspecific() and it expects the MP lock
 * to be held.
 */

INTERNAL void pthread__thread_ctx_free
#ifdef _DCE_PROTO_
(
    pthread_thread_ctx_t            *tctx
)
#else
(tctx)
pthread_thread_ctx_t            *tctx;
#endif
{
    RPC_DBG_PRINTF(rpc_e_dbg_threads, 5,
        ("pthread__thread_ctx_free: key=%d thread=%x tctx=%x\n",
        tctx->key, tctx->thread, tctx));

    tctx->c.back->c.forw = tctx->c.forw;
    tctx->c.forw->c.back = tctx->c.back;

    RPC_MEM_FREE(tctx, RPC_C_MEM_PTHREAD_CTX);
}

/* ------------------------------------------------------------------ */

PRIVATE int pthread_keycreate(key, destructor)
pthread_key_t               *key;
pthread_destructor_t        destructor;
{
    pthread_key_ctx_t   *kctx;
    pthread_key_t   k;
    PTHREAD_MP_LOCK_DECL;

    PTHREAD_MP_LOCK;
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
	    PTHREAD_MP_UNLOCK;
            return (0);
        }

    DIE("pthread_key_create");
}


PRIVATE int pthread_setspecific(key, context_value)
pthread_key_t               key;
pthread_addr_t              context_value;
{
    pthread_thread_ctx_t   *tctx;
    PTHREAD_MP_LOCK_DECL;

    /*
     * Lookup (create if necessary) the thread's ctx block for this key.
     */

    PTHREAD_MP_LOCK;
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
	PTHREAD_MP_UNLOCK;
        return (0);
    }

    /*
     * Save the new context value.
     */

    tctx->context_value = context_value;

    PTHREAD_MP_UNLOCK;
    return (0);
}

PRIVATE int pthread_getspecific(key, context_value)
pthread_key_t               key;
pthread_addr_t              *context_value;
{
    pthread_thread_ctx_t   *tctx;
    PTHREAD_MP_LOCK_DECL;

    /*
     * ensure that a getspecific does NOT create a internal
     * per-thread context block.
     */
    PTHREAD_MP_LOCK;
    tctx = pthread__thread_ctx_lookup(key);

    if (tctx == NULL)
        *context_value = NULL;
    else
        *context_value = tctx->context_value;

    PTHREAD_MP_UNLOCK;
    return (0);
}

/* =============================================================== */
/*
 * Generic Mutex operations.
 *
 * I believe that the PTHREAD_MUTEX macros that are used in the following can be made
 * MP safe by locking around them (they are defined in ../pthread.h).  In the lock
 * case, the macro calls sleep() so that almost forces PTHREAD_MP_LOCK to be an alpha
 * semaphore so it will be released around the sleep() call.  
 */

PRIVATE int pthread_mutex_init(new_mutex, attr)
pthread_mutex_t             *new_mutex;
pthread_mutexattr_t         attr;
{
    PTHREAD_MP_LOCK_DECL;

    PTHREAD_MP_LOCK;
    PTHREADS_INIT

    *new_mutex = 0;

    PTHREAD_MP_UNLOCK;
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
    PTHREAD_MP_LOCK_DECL;

    PTHREAD_MP_LOCK;
    PTHREAD_MUTEX_LOCK_INTERNAL(*mutex);
    PTHREAD_MP_UNLOCK;

    return (0);
}

PRIVATE int pthread_mutex_trylock(mutex)
pthread_mutex_t             *mutex;
{
    int return_value;
    PTHREAD_MP_LOCK_DECL;

    PTHREAD_MP_LOCK;
    return_value = PTHREAD_MUTEX_TRY_LOCK_INTERNAL(*mutex);
    PTHREAD_MP_UNLOCK;

    return (return_value);
}

PRIVATE int pthread_mutex_unlock(mutex)
pthread_mutex_t             *mutex;
{
    PTHREAD_MP_LOCK_DECL;

    PTHREAD_MP_LOCK;
    PTHREAD_MUTEX_UNLOCK_INTERNAL(*mutex);
    PTHREAD_MP_UNLOCK;

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
    PTHREAD_MP_LOCK_DECL;

    PTHREAD_MP_LOCK;
    PTHREADS_INIT

    *new_condition = 0;

    PTHREAD_MP_UNLOCK;
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
/*
 * Since we are using an alpha semaphore for the MP lock, it will be released
 * when we sleep and re-acquired when we wake.
 */
#define PTHREAD_COND_SLEEP_PRI              (PZERO-3)

PRIVATE int pthread_cond_wait(condition, mutex)
pthread_cond_t              *condition;
pthread_mutex_t             *mutex;
{
    PTHREAD_MP_LOCK_DECL;

    PTHREAD_MP_LOCK;

    *condition |= PTHREAD_COND_WAITING;

    PTHREAD_MUTEX_UNLOCK_INTERNAL(*mutex);

    sleep(condition, PTHREAD_COND_SLEEP_PRI);

    PTHREAD_MUTEX_LOCK_INTERNAL(*mutex);

    PTHREAD_MP_UNLOCK;
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
    PTHREAD_MP_LOCK_DECL;

    PTHREAD_MP_LOCK;

    *condition |= PTHREAD_COND_WAITING;

    PTHREAD_MUTEX_UNLOCK_INTERNAL(*mutex);

#ifdef NO_TSLEEP
    error = sleep(condition, PTHREAD_COND_INTR_SLEEP_PRI);
#else
    error = tsleep(condition, PTHREAD_COND_INTR_SLEEP_PRI, "intr_wait", 0);
#endif

    PTHREAD_MUTEX_LOCK_INTERNAL(*mutex);

    PTHREAD_MP_UNLOCK;
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

/* These routines use the same locking strategy as softclock() in
 * kern_bld/sys.800/kern_clock.c.  We are protecting the callout to this routine
 * from the timer code against our call side manipulations.  This routine is
 * called at timer interrupt time and can't use the PTHREAD_COND_SIGNAL_INTERNAL
 * macro because it already does its own locking, and we need to lock around this
 * whole routine.
 */
INTERNAL void pthread__timedwait_wakeup
#ifdef _DCE_PROTO_
(
    pthread__timedwait_t *ttbl
)
#else
(ttbl)
pthread__timedwait_t *ttbl;
#endif
{
    int s;

    s = splhigh();

    ttbl->timedout = true;
    *ttbl->cv |= PTHREAD_COND_TIMEOUT;

    /* Copied from:  PTHREAD_COND_SIGNAL_INTERNAL(*ttbl->cv); */
    if ((*ttbl->cv & PTHREAD_COND_WAITING) != 0) {
        *ttbl->cv &= ~PTHREAD_COND_WAITING;
        wakeup(ttbl->cv);
    }
    splx(s);
}

/*
 * This routine protects against call-side interference with the global
 * semaphore and interrupt side interference using the same locking that is used
 * above.  We have to protect all access to our "per-thread" time elements and
 * we have to make timeout/untimeout operations "atomic" with changes to these
 * elements (hence the spinlock/spl).
 */
PRIVATE int pthread_cond_timedwait
#ifdef _DCE_PROTO_
(
    pthread_cond_t              *condition,
    pthread_mutex_t             *mutex,
    struct timespec             *abs_time
)
#else
(condition, mutex, abs_time)
pthread_cond_t              *condition;
pthread_mutex_t             *mutex;
struct timespec             *abs_time;
#endif
{
    int                     s;
    int                     tmo;
    struct timeval          atv;         /* absolute timeval */
    pthread__timedwait_t    *ttbl;
    PTHREAD_MP_LOCK_DECL;

    /* convert abs timespec to abs timeval */
    atv.tv_sec = abs_time->tv_sec;
    atv.tv_usec = abs_time->tv_nsec / 1000;

    PTHREAD_MP_LOCK;
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
	PTHREAD_MP_UNLOCK;
        DIE("timedwait table exhausted");
        /* 
         * make the table bigger, have "cheap" per-thread timedwait
         * storage, ...  vendors choice...
         */
    }

    /*
     * From here to the end we have to be "atomic" with respect to clock
     * interrupts and timer callouts to our pthread__timedwait_wakeup() above.
     * We use the same lock as the kern_clock.c routines since they are the ones
     * we are locking against (see comments above, too).  Note that we can't
     * hold a spinlock across the call to sleep().
     */
    s = splhigh();

    if ((tmo = hzto(&atv)) <= 0
        || atv.tv_sec < time.tv_sec
        || (atv.tv_sec == time.tv_sec && atv.tv_usec <= time.tv_usec))
    {
        ttbl->thread = NULL;			/* No longer in use. */

	splx(s);
	PTHREAD_MP_UNLOCK;

        RPC_DBG_PRINTF(rpc_e_dbg_threads, 1,
            ("pthread_cond_timedwait: timeout prior to sleep, ttbl=%x\n",
                ttbl));

        return (-1);
    }

    RPC_DBG_PRINTF(rpc_e_dbg_threads, 3,
        ("pthread_cond_timedwait: scheduling %d sec timeout, ttbl=%x\n",
            tmo/hz, ttbl));

    timeout(pthread__timedwait_wakeup, (caddr_t) ttbl, tmo);

    *condition |= PTHREAD_COND_WAITING;
    *condition &= ~PTHREAD_COND_TIMEOUT;
    PTHREAD_MUTEX_UNLOCK_INTERNAL(*mutex);

    sleep(condition, PTHREAD_COND_SLEEP_PRI); /* The PTHREAD_MP_LOCK is released and reacquired here. */

    PTHREAD_MUTEX_LOCK_INTERNAL(*mutex);

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
	splx(s);
	PTHREAD_MP_UNLOCK;

        RPC_DBG_PRINTF(rpc_e_dbg_threads, 1,
            ("pthread_cond_timedwait: timeout, ttbl=%x\n", ttbl));

        return (-1);
    }

    /*
     * Otherwise, cancel the timeout and let the caller recheck
     * their predicates to see if the wakeup was "real".
     */
    untimeout(pthread__timedwait_wakeup, (caddr_t) ttbl);

    splx(s);
    PTHREAD_MP_UNLOCK;

    RPC_DBG_PRINTF(rpc_e_dbg_threads, 3,
        ("pthread_cond_timedwait: no timeout, ttbl=%x\n", ttbl));
    return (0);
}

/* =============================================================== */
/*
 * pthread cancel stuff...
 */

PRIVATE int pthread_cancel
#ifdef _DCE_PROTO_
(
    pthread_t                   thread
)
#else
(thread)
pthread_t                   thread;
#endif
{
    PTHREAD_MP_LOCK_DECL;

    PTHREAD_MP_LOCK;
    PTHREADS_INIT
    PTHREAD_MP_UNLOCK;

    /* NO OP; no cancel behavour support in the kernel */
    return (0);
}

PRIVATE int pthread_setasynccancel
#ifdef _DCE_PROTO_
(
    int mode
)
#else
(mode)
int mode;
#endif
{
    PTHREAD_MP_LOCK_DECL;

    PTHREAD_MP_LOCK;
    PTHREADS_INIT
    PTHREAD_MP_UNLOCK;

    /* NO OP; no cancel behavour support in the kernel */
}

PRIVATE int pthread_setcancel
#ifdef _DCE_PROTO_
(
    int mode
)
#else
(mode)
int mode;
#endif
{
    PTHREAD_MP_LOCK_DECL;

    PTHREAD_MP_LOCK;
    PTHREADS_INIT
    PTHREAD_MP_UNLOCK;

    /* NO OP; no cancel behavour support in the kernel */
}


PRIVATE void pthread_testcancel(void)
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
 *
 * I don't believe we need to lock around this routine because the exception structure
 * argument is local to the caller (not shared).
 */
PRIVATE void pthread_exc_raise
#ifdef _DCE_PROTO_
(
    volatile EXCEPTION *e
)
#else
(e)
volatile EXCEPTION *e;
#endif
{
    pthread_exc_ctxt_t **_exc_ctxt_head = NULL;

    pthread_getspecific(pthread_exc_ctxt_key, (pthread_addr_t *)(&_exc_ctxt_head));

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

PRIVATE void pthread_exc_setup
#ifdef _DCE_PROTO_
(
    volatile pthread_exc_ctxt_t ***_exc_ctxt_head,
    pthread_exc_ctxt_t **prev_exc_ctxt
)
#else
(_exc_ctxt_head, prev_exc_ctxt)
volatile pthread_exc_ctxt_t ***_exc_ctxt_head;
pthread_exc_ctxt_t **prev_exc_ctxt;
#endif
{
    pthread_getspecific(pthread_exc_ctxt_key, (pthread_addr_t *)(_exc_ctxt_head));
    if (*_exc_ctxt_head == NULL) {
        RPC_MEM_ALLOC(*_exc_ctxt_head, pthread_exc_ctxt_t **, sizeof *_exc_ctxt_head, 
            RPC_C_MEM_JMPBUF_HEAD, RPC_C_MEM_WAITOK);
        if (*_exc_ctxt_head == NULL)
            DIE("(pthread_exc_setup)");
        **_exc_ctxt_head = NULL;
        pthread_setspecific(pthread_exc_ctxt_key, *_exc_ctxt_head);
    }

    *prev_exc_ctxt = (pthread_exc_ctxt_t *)**_exc_ctxt_head;
}

/*
 * Destroy the per-thread exception state context; i.e.
 * free the per-thread storage and cause the per-thread
 * key context storage to be freed.
 */

PRIVATE void pthread_exc_destroy
#ifdef _DCE_PROTO_
(
    volatile pthread_exc_ctxt_t **_exc_ctxt_head
)
#else
(_exc_ctxt_head)
volatile pthread_exc_ctxt_t **_exc_ctxt_head;
#endif
{
    RPC_MEM_FREE(_exc_ctxt_head, RPC_C_MEM_JMPBUF_HEAD);
    pthread_setspecific(pthread_exc_ctxt_key, NULL);
}

INTERNAL void pthread_exc_ctxt_free
#ifdef _DCE_PROTO_
(
    pthread_addr_t context_value
)
#else
(context_value)
pthread_addr_t context_value;
#endif
{
    RPC_MEM_FREE(context_value, RPC_C_MEM_JMPBUF_HEAD);
}

/* =============================================================== */
/*
 * pthread_*_np() stuff...
 */

/*
 * I don't believe we need to lock around this routine because it only changes its
 * arguments, which are probably local to its caller.
 */
PRIVATE int pthread_get_expiration_np
#ifdef _DCE_PROTO_
(
    struct timespec *delta_ts,
    struct timespec *abs_ts
)
#else
(delta_ts, abs_ts)
struct timespec *delta_ts;
struct timespec *abs_ts;
#endif
{
    int s;
    PTHREAD_MP_LOCK_DECL;

    PTHREAD_MP_LOCK;
    PTHREADS_INIT
    PTHREAD_MP_UNLOCK;

    /*
     * Compute an absolute timespec
     */
    s = splhigh();

    MICROTIME((struct timeval *)abs_ts);
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

/*
 * This routine is only called from pthread__init() and expects the MP lock to be held.
 */
INTERNAL void pthread__delay_init(void)
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

PRIVATE int pthread_delay_np
#ifdef _DCE_PROTO_
(
    struct timespec *interval
)
#else
(interval)
struct timespec *interval;
#endif
{
    pthread__delay_t        *dtbl;
    struct timespec         abs_ts;     /* absolute timespec */
    PTHREAD_MP_LOCK_DECL;

    PTHREAD_MP_LOCK;
    PTHREADS_INIT

    PTHREAD_MUTEX_LOCK_INTERNAL(delay_m);
    PTHREAD_MP_UNLOCK;				/* We are now protected by the delay_m mutex. */

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

    PTHREAD_MP_LOCK;
    PTHREAD_MUTEX_UNLOCK_INTERNAL(delay_m);
    PTHREAD_MP_UNLOCK;
    return (0);
}

