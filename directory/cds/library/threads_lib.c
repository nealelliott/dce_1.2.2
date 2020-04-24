/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: threads_lib.c,v $
 * Revision 1.1.12.2  1996/02/18  19:35:07  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:15:15  marty]
 *
 * Revision 1.1.12.1  1995/12/08  15:27:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:47:37  root]
 * 
 * Revision 1.1.8.7  1994/08/03  19:03:16  mccann
 * 	includes file cleanup
 * 	[1994/07/19  18:10:44  mccann]
 * 
 * Revision 1.1.8.6  1994/06/30  19:17:56  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:04:57  mccann]
 * 
 * Revision 1.1.8.5  1994/06/09  18:42:32  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:14:45  devsrc]
 * 
 * Revision 1.1.8.4  1994/05/12  21:12:15  peckham
 * 	Use _CDS_DEFAULT_STACK_
 * 	Eliminate some (now) extraneous definitions.
 * 	[1994/05/12  19:17:51  peckham]
 * 
 * Revision 1.1.8.3  1994/04/29  15:55:42  peckham
 * 	Remove unnecessary conditionals.
 * 	Use dcecdsmac.h macros for dce_svc_printf().
 * 	[1994/04/29  14:35:27  peckham]
 * 
 * Revision 1.1.8.2  1994/03/22  20:00:13  griffin
 * 	Modified instrumentation events to Level 9 so the serviceability
 * 	logs aren't as cluttered.
 * 	[1994/03/22  16:43:17  griffin]
 * 
 * Revision 1.1.8.1  1994/03/12  22:08:57  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:35:35  peckham]
 * 
 * Revision 1.1.6.2  1993/07/30  14:57:59  blurie
 * 	Cosmetic change:  Remove dead code between SNI_TEST defines.
 * 	[1993/07/30  13:40:43  blurie]
 * 
 * Revision 1.1.4.5  1993/01/26  14:07:41  mfox
 * 	Fix ANSI C compiler warnings.
 * 	[1993/01/26  14:01:53  mfox]
 * 
 * Revision 1.1.4.4  1992/12/30  14:17:26  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:17:22  zeliff]
 * 
 * Revision 1.1.4.3  1992/10/26  20:54:54  peckham
 * 	OT#3881: make dthread_current_thread() thread-safe.
 * 	 The problem is that there is no locking while
 * 	 setting last_?thread. Shuffle things so the locking
 * 	 penalty is minimized.
 * 	OT#4001: remove reference to cma_import_fd(). This can only be done
 * 	 for kernel threads (detected by absence of _USER_THREADS_).
 * 	[1992/10/26  20:53:04  peckham]
 * 
 * Revision 1.1.4.2  1992/09/29  18:53:52  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:16:25  weisman]
 * 
 * Revision 1.1  1992/01/19  15:19:19  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: threads_lib.c
 *
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1990-1994. ALL RIGHTS RESERVED.
 *
 * THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
 * ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
 * INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
 * COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
 * OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
 * TRANSFERRED.
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
 * AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
 * CORPORATION.
 *
 * DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
 * SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
 *
 * ABSTRACT:
 *
 * This module implements a hardware-independent threads package based
 * on similar constructs found in the Concert Multithread Architecture
 * (CMA) and POSIX Std. 1003.4.
 *
 */

#include <dce/dce.h>
#include <dce/dcecdsmsg.h>
#include <dce/assert.h>
#include <dce/cds_basic_types.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <dce/dns_config.h>
#include <threads_lib.h>
#include <dce/dns_record.h>
#include <dns_cfg.h>
#include <dns_malloc.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <signal.h>

#define THREAD_LOCK(ptr)	{ (void)pthread_mutex_lock(&ptr->th_lock); }
#define THREAD_END_LOCK(ptr)	{ (void)pthread_mutex_unlock(&ptr->th_lock); }
#define THREAD_WAIT(ptr,cv)	{ (void)pthread_cond_wait(cv, &ptr->th_lock); }
#define DNS_LOCK		{ (void)pthread_mutex_lock(&threads_mutex); }
#define DNS_END_LOCK		{ (void)pthread_mutex_unlock(&threads_mutex); }

typedef struct cma_opaque {
    char	*cma_addr;
    short	cma_id;
    short	cma_other;
} cma_opaque_t;

/*
 * These structures support the threads logging facility - for diagnostics
 */

#define MAX_THREADS_LOGGED 50
#define MAX_TH_LOG_COMMENT_SIZE 50
static int th_log_index = -1;
static int th_log_limit = -1;
static struct {
    dthread_t		thread_id;
    time_local_t	time_logged;
    char		comment[MAX_TH_LOG_COMMENT_SIZE];
} th_log[MAX_THREADS_LOGGED];

#if defined(DTHREAD_WRAPPERS)

/*
 *          Macros, macros, macros
 */
#define POSIX_SUCCESS_STATUS 0
#define	POSIX_ERROR_STATUS -1
#define LOCAL_SUCCESS_STATUS POSIX_SUCCESS_STATUS

/*
 * This macro returns errors using POSIX conventions.
 */
#define RETURN_ERROR(error_code) { 		\
    private_thread *ptc_p = CURRENT_THREAD; 	\
	                                        \
    dce_svc_printf( 				\
        CDS_S_LIBRARY_THREADS_MSG,		\
	__LINE__,				\
	(long)(error_code)); 			\
	                                        \
    ptc_p->th_errno = error_code;    		\
	                                        \
    return (POSIX_ERROR_STATUS);  		\
}

#define RETURN_SUCCESS return (POSIX_SUCCESS_STATUS)

#define RETURN_IF_FAILURE(error_code) 			\
{							\
    if (error_code != POSIX_SUCCESS_STATUS) 		\
    {							\
        return (error_code);				\
    }							\
}

/*
 * These are our local mappings for standard errors.
 *   Internal      -> POSIX
 *   -------------    -------
 *   ERR_NO_MEMORY -> ENOMEM
 *   ERR_INVALID   -> EINVAL
 *   ERR_BUSY      -> EBUSY
 */
#define ERR_NO_MEMORY ENOMEM
#define ERR_INVALID   EINVAL
#define ERR_BUSY      EBUSY


/*
 * Bugcheck handling
 */

#define BUGCHECK(bugcheck_code) th_bugcheck(bugcheck_code, __LINE__)

#define BUG_SWITCH		-10
#define BUG_SELECT		-11
#define BUG_CONTEXT		-12
#define BUG_LIST_CORRUPT	-13
#define	BUG_RESOURCES		-14
#define	BUG_CLEANUP		-15
#define	BUG_PTHREAD		-16
#define	BUG_INIT		-17
#define BUG_ASSERTION           -18


/*
 *                 Thread Management Data Structures
 *
 * Thread states
 */
typedef enum {
    state_initialized = 0,
    state_running,
    state_runnable,
    state_delay,
    state_wait,
    state_timed_wait,
    state_sys_wait,
    state_read,
    state_write,
    state_read_write,
    state_join,
    state_exiting
} thread_state_t;

/*
 *  CMA per-thread context for this package
 *
 *  This package maintains a per-thread context entry which contains
 *  a pointer to the private thread control block - essentially our
 *  version of the "current thread" value.
 */

static pthread_key_t dthread_ptc_key;

/*
 * Internal (private) thread control block
 */

typedef struct PRIVATE_THREAD {
    struct {
        struct PRIVATE_THREAD *next;   /* Pool queue forward link */
        struct PRIVATE_THREAD *prev;   /* Pool queue backward link */
    } pool;
    pthread_mutex_t	th_lock;       /* Lock for this control block */
    pthread_t		th_pthread;    /* CMA thread handle */
    pthread_cond_t	th_resume;     /* Condition to resume stalled thread */
    dthread_cleanup_handler_t *th_ch;  /* Stack of cleanup handlers */
    dthread_date_time_t	th_timer;      /* Delay expiration time for this thread */
    int			th_fd;         /* Socket associated with this thread */
    char		*th_name_p;    /* name of thread */
    dthread_start_routine th_start_routine; /* Thread start routine */
    dthread_address_t	th_arg;        /* Argument to thread routine */
    dthread_address_t	th_exit;       /* Exit value from thread routine */
    thread_state_t	th_state;      /* Thread state */
    dthread_mutex_t	th_mutex;      /* Mutex this thread is waiting to lock */
    dthread_cond_t	th_condition;  /* CV that this thread is waiting on */
    long                th_stack_limit; /* Stack limit set on creation (diag) */
    int			th_fork_ret;   /* Fork return status */
    int			th_errno;      /* Per-thread errno */
    char		th_cancel;     /* Thread cancel-ability state */
    char		th_exited;     /* Thread is dying */
    char		th_detached;   /* Thread is detached */
    char		th_cancelled;  /* Thread has been cancelled */
} private_thread;


void *
dthread_current_thread (
    void		*);

#define	CURRENT_THREAD \
    ((private_thread *)dthread_current_thread(NULL))

static dthread_date_time_t zero_timer;       /* Initialized timer constant */

/*
 * Thread database interlock
 */

static pthread_mutex_t threads_mutex;
static pthread_cond_t no_threads;

/*
 * Thread statistics
 */

static int num_threads  = 0;     /* Current number of threads being managed */
static int peak_threads = 0;     /* Maximum number of threads ever managed  */


/*
 * Active Thread Pool
 */

static private_thread active_pool = {
       { (private_thread *)&active_pool,               /* pool    */
         (private_thread *)&active_pool  }
    };

#define EMPTY_ACTIVE_POOL \
  (active_pool.pool.next == (private_thread *)&active_pool)

#define LISTHEAD_ACTIVE_POOL ((private_thread *)&active_pool)

/*
 *              Mutex and Condition Variable Data Structures
 */

typedef struct PRIVATE_MUTEX {
    pthread_mutex_t mutex_id;
    private_thread *owner;	        /* Thread that owns this mutex */
} private_mutex;

static int num_mutexes = 0;
static int peak_mutexes = 0;
static int mutex_locks = 0;            /* How many times a lock requested */
static int mutex_collisions = 0;       /* How many times a lock blocked */
static int mutex_multi_collisions = 0; /* Even stranger blocking condition */



typedef struct PRIVATE_CONDITION {
    pthread_cond_t condition_id;
} private_condition;

static int num_conditions = 0;
static int peak_conditions = 0;




/*
 *        Miscellaneous Data Structures
 */


/*
 * Predefined null handle and handle manipulation routines
 */

dthread_handle dthread_null = {dthread_handle_init};

#define	alloc_handle(size) ((dthread_handle)dns_malloc(size))

#define	free_handle(ptr) dns_free((char *)ptr)

#define	handle_data(ptr) (ptr)

/*
 * Local function prototypes
 */

static private_thread *
th_birth (
    dthread_start_routine ,
    dthread_address_t	  ,
    dthread_attr_t	  *);

static void
th_death (
    private_thread	*);

static int
th_bug_output (char       *,
               void    *);

static unsigned long
a__num (
	char	*);
/*
 * This routine should be "static" since it is NOT used in any
 * modules, but rather it's only for use in a debugger such as dbx.
 * Because the gcc compiler warns of it being unused, it's
 * made non static.
 */
int
dthread_debug (
	       void);




/*
 * Bugchecking routines
 */


/*
 * ROUTINE - th_bug_output
 *
 * Prints "message_string" on stderr & to the user's "log" area. 
 *	   (eg: file, or the console, etc).
 */
static int
th_bug_output (char       *message_string,
               void    *debug_level)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        (*(unsigned32 *)debug_level),
        "th_bug_output: %s",
	message_string));


    return (1);
}


/*
 * ROUTINE - th_bugcheck
 *
 * Bugcheck handling for the threads package.
 *
 * If the threads package encounters a situation that it cannot cope with
 * it bugchecks.  This routine attempts to provide as much information as
 * possible to the outside world regarding the bugcheck.
 *
 * Arguments:
 *	bugcheck_code (in) - Identification of the bugcheck.
 *      line_number (in)   - Line number where the bugcheck occurred.
 */
static void
th_bugcheck (int  bugcheck_code,
             int  line_number)
{
    int save_errno = errno;
    private_thread *th_p = CURRENT_THREAD;
    unsigned32 dlevel = svc_c_debug1;
    struct sigaction sigactn;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug2,
        " >th_bugcheck"));

    /*
     * Pretty simple stuff right now - print error on stderr and die.
     */
    dce_svc_printf(CDS_S_LIBRARY_BUGCHECKHDR_MSG);


    dce_svc_printf(CDS_S_LIBRARY_THREADSBUG_MSG, bugcheck_code, 
		   line_number, strerror(save_errno), th_p);

     (void)dthread_diag_threads(8, 
				1, 
				(dthread_t *)0, 
				(dthread_mutex_t *)0,
				(dthread_cond_t *)0, 
				th_bug_output, 
				&dlevel);   /* ATP */

    (void)dthread_diag_threads(2, 
			       1, 
			       (dthread_t *)0, 
			       (dthread_mutex_t *)0,
			       (dthread_cond_t *)0, 
			       th_bug_output, 
			       &dlevel);   /* Stats */

    /*
     *  Set up sigaction structure to ensure abort's signal isn't caught
     */
    sigactn.sa_handler = SIG_DFL;
    sigemptyset(&sigactn.sa_mask);
    sigactn.sa_flags   = 0;
    sigaction(SIGILL, &sigactn, NULL);

    abort();
    exit(-1);
}

/*
 * ROUTINE - th_init
 *
 * Initialize the threads package
 */
int 
th_init (void)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug2,
        " >th_init")); 

    if (pthread_keycreate(&dthread_ptc_key,(pthread_destructor_t)th_death) < 0)
	BUGCHECK(BUG_RESOURCES);

    if (pthread_mutex_init(&threads_mutex, pthread_mutexattr_default) < 0)
	BUGCHECK(BUG_RESOURCES);

    if (pthread_cond_init(&no_threads, pthread_condattr_default) < 0)
	BUGCHECK(BUG_RESOURCES);

    CURRENT_THREAD->th_name_p = "main";
  
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " <th_init")); 

    RETURN_SUCCESS;
}

/*
 * ROUTINE - th_term
 *
 * Do post-threads processing and display
 */
int 
th_term (void)
{
  DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >th_term")); 

 DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " <th_term")); 

    RETURN_SUCCESS;
}

/*
 * ROUTINE - dthread_current_thread
 *
 * Obtain the current private_thread address.
 * This routine also looks for context switches and reports them.
 * Call it with NULL on entry to any dthread_*() routine,
 * and call with the thread block after any blocking call.
 */
void *
dthread_current_thread (void *arg)
{
    static private_thread *last_dthread = NULL;
    static pthread_t last_pthread;
    private_thread *return_dthread = NULL;
    pthread_t this_pthread;
    register private_thread *cth_p;

    this_pthread = pthread_self();

    /*
     * Decide which dthread is running.
     */

    DNS_LOCK {
      if (pthread_equal_np(this_pthread, last_pthread)) 
	  {
	    return_dthread = last_dthread;
	    /*
	     * fast exit if still in same thread 
	     */
	  } 
      else 
	if (((cth_p = arg) == NULL)
	    || !pthread_equal_np(this_pthread, cth_p->th_pthread)) 
	{
	    /*
	     * hint didn't match 
	     */
	    if (0 <= pthread_getspecific(dthread_ptc_key,
					 (pthread_addr_t *)&arg)) 
	    {
		cth_p = arg;
	    } 
	    else 
	    {
		cth_p = NULL;
	    }
	}
    } DNS_END_LOCK;
    
    if (return_dthread)
	return(return_dthread);
    /*
     * new foreign pthread - create private dthread to track it 
     */

    if (cth_p == NULL) 
    {
        cth_p = (private_thread *)dns_malloc(sizeof(private_thread));
	if (cth_p == NULL) 
	{
	    BUGCHECK(ERR_NO_MEMORY);
	}
	
	ZERO_bytes(cth_p, sizeof(*cth_p));
	cth_p->th_state = state_runnable;
	cth_p->th_start_routine = 0;
	cth_p->th_name_p = "*foreign*";
	cth_p->th_arg = 0;
	cth_p->th_fd = -1;
	cth_p->th_errno = 0;
	cth_p->th_cancel = pthread_setcancel(CANCEL_OFF);
	(void)pthread_setcancel(cth_p->th_cancel);
	cth_p->th_pthread = this_pthread;

	if (pthread_mutex_init(&cth_p->th_lock, pthread_mutexattr_default) < 0)
	{
	    BUGCHECK(BUG_RESOURCES);
	}
	
	if (pthread_cond_init(&cth_p->th_resume, pthread_condattr_default) < 0)
	{
	    BUGCHECK(BUG_RESOURCES);
	}

	if (pthread_setspecific(dthread_ptc_key, (pthread_addr_t)cth_p) < 0)
	{
	    BUGCHECK(BUG_CONTEXT);
	}

        DNS_LOCK 
	{
	    num_threads++;	/* Bump up # of threads active */

	    /*
	     * Keep track of maximum # of threads 
	     */
	    if (num_threads > peak_threads)
	        peak_threads = num_threads;

	    /*
	     * Add this thread to the active threads pool
	     */

	    cth_p->pool.next = LISTHEAD_ACTIVE_POOL;
	    active_pool.pool.prev->pool.next = cth_p;
	    cth_p->pool.prev = active_pool.pool.prev;
	    active_pool.pool.prev = cth_p;

	    last_dthread = cth_p;
	    last_pthread = this_pthread;
	} DNS_END_LOCK;
    } 
    else 
    {
        DNS_LOCK 
	{
	    last_dthread = cth_p;
	    last_pthread = this_pthread;
	} DNS_END_LOCK;
    }

    /*	
     * announce new thread 
     */
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " <dthread_current_thread: switch to 0x%p->%s",
        cth_p,
        cth_p->th_name_p));

    return(cth_p);
}

/*
 * ROUTINE - th_death
 *
 * Release the thread's context.
 *
 * Arguments:
 *	th_p (in) - Pointer to thread to be buried.
 *
 * no outputs.
 */
static void 
th_death (private_thread *th_p)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >th_death(0x%p->%s)",
        th_p,
        th_p->th_name_p));

    if (th_p->th_ch) 
    {
	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_library,
            svc_c_debug9,
            " <th_death(): th_p->th_ch"));

	return;
    }
    
    if (!th_p->th_detached)
    {
	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_library,
            svc_c_debug9,
            " <th_death(): !th_p->th_detached"));

	return;
    }

    if (!th_p->th_exited) 
    {
	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_library,
            svc_c_debug9,
            " <th_death(): !th_p->th_exited"));

	return;
    }

    DNS_LOCK 
    {
	/*
	 * Remove thread from the active pool
	 */
	th_p->pool.prev->pool.next = th_p->pool.next;
	th_p->pool.next->pool.prev = th_p->pool.prev;

	/*
	 * Another thread bites the dust.
	 */

	if (--num_threads == 0)
	{
	    (void)pthread_cond_broadcast(&no_threads);
	}

    } DNS_END_LOCK;

    /*
     * Destroy thread's interlock 
     */
    if (pthread_mutex_destroy(&th_p->th_lock) < 0)
    {
	BUGCHECK(BUG_RESOURCES);
    }

    /*
     * Destroy thread's stall cv  
     */
    if (pthread_cond_destroy(&th_p->th_resume) < 0)
    {
	BUGCHECK(BUG_RESOURCES);
    }
    
    dns_free((char *)th_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " <th_death()"));
}

/*
 * ROUTINE - th_start
 *
 * Start a new thread, passing the given data to it.
 * This routine acts within the thread's context, and
 * insures that the thread is freed when the thread's
 * code exits.
 *
 * No inputs or outputs.
 */
static void
th_start (private_thread *th_p)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >th_start")); 

    if (pthread_setspecific(dthread_ptc_key, (pthread_addr_t)th_p) < 0)
    {
        BUGCHECK(BUG_CONTEXT);
    }

    /*
     * Call the thread's code.
     */
    dthread_exit((*th_p->th_start_routine)(th_p->th_arg));

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " <th_start")); 
}

/*
 * ROUTINE - th_birth
 *
 * Create a new thread control block with associated resources
 *
 * Arguments:
 *     start_routine (in) - routine to handle new thread.
 *     arg (in) - argument to pass to new thread.
 *     attr (in) - thread attributes
 *
 * Returns:
 *     New thread control block.
 */
static private_thread *
th_birth ( dthread_start_routine start_routine,
           dthread_address_t     arg,
           dthread_attr_t        *attr)
{
    private_thread *const cth_p = CURRENT_THREAD;
    private_thread *th_p;
    char *start_name = (attr && attr->name) ? attr->name : "";
    const long stack_req = (attr) ? attr->stacksize : 0;
#if defined(_DECTHREADS_) &&  (_CMA_OSIMPL_ == _CMA__OS_OSF) && defined(__alpha)
    const long stack_size = stack_req ? stack_req+8*1024 : _CDS_DEFAULT_STACK_;
#else
    const long stack_size = stack_req ? stack_req : _CDS_DEFAULT_STACK_;
#endif

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >th_birth(0x%p,0x%p,%d->(%s,%d))",
        start_routine,
        arg,
        attr,
        start_name,
        stack_req));

    /*
     * Allocate a thread.
     */
    if ((th_p = (private_thread *)dns_malloc(sizeof(private_thread)))
	!= NULL) 
    {
	/*
	 * Initialize the thread control block
	 */

	ZERO_bytes(th_p, sizeof(*th_p));
	th_p->th_state = state_initialized;
	th_p->th_name_p = start_name;
	th_p->th_start_routine = start_routine;
	th_p->th_arg = arg;
        th_p->th_stack_limit = stack_req;
	th_p->th_fd = -1;
	th_p->th_errno = 0;
	th_p->th_cancel = CANCEL_ON;

	if (pthread_mutex_init(&th_p->th_lock, pthread_mutexattr_default) < 0)
	{
	    BUGCHECK(BUG_RESOURCES);
	}
	
	if (pthread_cond_init(&th_p->th_resume, pthread_condattr_default) < 0)
	{
	    BUGCHECK(BUG_RESOURCES);
	}

	/*
	 * Update the private threads database 
	 */
	DNS_LOCK 
	{
	    num_threads++;                    /* Bump up # of threads active */
	    if (num_threads > peak_threads) 
	    {   
		/*
		 * Keep track of maximum # of threads 
		 */
		peak_threads = num_threads;
	    }

	    /*
	     * Add this thread to the active threads pool
	     */

	    th_p->pool.next = LISTHEAD_ACTIVE_POOL;
	    active_pool.pool.prev->pool.next = th_p;
	    th_p->pool.prev = active_pool.pool.prev;
	    active_pool.pool.prev = th_p;
	} DNS_END_LOCK;

	th_p->th_state = state_runnable;
	{
	pthread_attr_t pattr;

        /*
         * Set requested stack size, if not defaulted
         */
	if (_CDS_DEFAULT_STACK_ || stack_size)
	{
	    if (pthread_attr_create(&pattr) < 0)
		{
		BUGCHECK(BUG_RESOURCES);
		}
	    if (pthread_attr_setstacksize(&pattr, stack_size) < 0)
		{
		BUGCHECK(BUG_RESOURCES);
		}
	} else
	    pattr = pthread_attr_default;

	/*
	 * Ask CMA to create the thread
	 */
	(void)pthread_create(&th_p->th_pthread, pattr,
		(pthread_startroutine_t)th_start, (pthread_addr_t)th_p);

	if (_CDS_DEFAULT_STACK_ || stack_size)
	    (void)pthread_attr_delete(&pattr);
        }
	
	(void)dthread_current_thread(cth_p);
    }
    

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " <th_birth(0x%p,0x%p,0x%p->(%s,%d)) return(0x%p)",
        start_routine,
        arg,
        attr,
        start_name,
        stack_req,
        th_p));

    return(th_p);
}

/*
 * ROUTINE - dthread_create
 *
 * Fork a new thread.
 *
 * Arguments:
 *     new_thread     - (out) address to receive handle to new thread.
 *     attr           - (in) pointer to threads attribute block
 *     start_routine  - (in) routine to handle new thread
 *     arg            - (in) argument to pass to new thread.
 *
 * Returns:
 *     0 - New thread has been successfully forked.
 *     ERR_NO_MEMORY - Unable to allocate space for the thread.
 */
int
dthread_create (dthread_t             *new_thread,
                dthread_attr_t        *attr,
                dthread_start_routine start_routine,
                dthread_address_t     arg)
{
    private_thread *th_p;

#if defined(DCE_ASSERT)
    private_thread *const cth_p = CURRENT_THREAD;
#endif


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >dthread_create(,0x%p,0x%p,0x%p)",
        attr,
        start_routine,
        arg));

    dce_assert(cds__svc_handle,(cth_p != NULL));

    /*
     * Allocate a thread.
     */
    if ((th_p = th_birth(start_routine, arg, attr)) != NULL) 
    {
	*new_thread = (dthread_t)th_p;

	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_library,
            svc_c_debug9,
            " <dthread_create(0x%p,0x%p,0x%p,0x%p) return(DNS_SUCCESS)",
            th_p,
            attr,
            start_routine,
            arg));

	RETURN_SUCCESS;
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " <dthread_create(0x%p,0x%p,0x%p,0x%p) return(ERR_NO_MEMORY)",
        th_p,
        attr,
        start_routine,
        arg));

    RETURN_ERROR(ERR_NO_MEMORY);
}

/*
 * ROUTINE - dthread_create_with_stack
 *
 * Fork a new thread with a given stack size.
 *
 * Arguments:
 *     new_thread (out) - address to receive handle to new thread.
 *     start_routine (in) - routine to handle new thread
 *     arg (in) - argument to pass to new thread.
 *     stack_size (in) - requested stack size or zero for default.
 *
 * Returns:
 *     0 - New thread has been successfully forked.
 *     ERR_NO_MEMORY - Unable to allocate space for the thread.
 */
int
dthread_create_with_stack (dthread_t             *new_thread,
                           dthread_start_routine start_routine,
                           dthread_address_t     arg,
                           unsigned int	         stack_size)
{
    dthread_attr_t attr;
    private_thread *th_p;

    private_thread *cth_p;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >dthread_create_with_stack(,0x%p,0x%p,%u)",
        start_routine,
        arg,
        stack_size));

    cth_p = CURRENT_THREAD;
    dce_assert(cds__svc_handle,(cth_p != NULL));

    attr.name = (char *)0;
    attr.stacksize = stack_size;

    /*
     * Allocate a thread.
     */
    if ((th_p = th_birth(start_routine, arg, &attr)) != NULL) 
    {
	*new_thread = (dthread_t)th_p;

	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_library,
            svc_c_debug9,
            " <dthread_create_with_stack(0x%p,0x%p,0x%p,%u) return(DNS_SUCCESS)",
            th_p,
            start_routine,
            arg,
            stack_size));

	RETURN_SUCCESS;
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " <dthread_create_with_stack(0x%p,0x%p,0x%p,%u) return(NO_RESOURCES)",
        th_p,
        start_routine,
        arg,
        stack_size));

    RETURN_ERROR(ERR_NO_MEMORY);
}

/*
 * ROUTINE - dthread_join
 *
 * Wait for a process to die, and return the exit status.
 *
 * Inputs:
 *	thread (in) - handle of thread to wait for.
 *	status (out) - to receive exit status.
 */
int
dthread_join (dthread_t	        thread,
              dthread_address_t	*status)
{
    private_thread *const cth_p = CURRENT_THREAD;
    private_thread *th_p = (private_thread *)thread;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >dthread_join(0x%p->%s,0x%p)",
        th_p,
        th_p->th_name_p,
        status));

    dce_assert(cds__svc_handle,cth_p != NULL);


    if (!th_p || th_p->th_detached) 
    {
        RETURN_ERROR(ERR_INVALID);
    }


    /*
     * Wait for thread to die.
     */
    cth_p->th_state = state_join;
    (void)pthread_join(th_p->th_pthread, (pthread_addr_t *)&th_p->th_exit);
    (void)dthread_current_thread(cth_p);
    cth_p->th_state = state_running;
    *status = th_p->th_exit;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " <dthread_join(0x%p->%s,0x%p->0x%p): return(success)",
        th_p,
        th_p->th_name_p,
        status,
        th_p->th_exit));

    RETURN_SUCCESS;
}

/*
 * ROUTINE - dthread_detach
 *
 * Wait for a process to die, and return the exit status.
 *
 * Inputs:
 *	thread (in) - handle of thread to wait for.
 */
int
dthread_detach (dthread_t   *thread)
{
    private_thread *const cth_p = CURRENT_THREAD;
    private_thread *th_p = (private_thread *)(*thread);
    int status = POSIX_SUCCESS_STATUS;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >dthread_detach(0x%p->%s)",
        th_p,
        th_p->th_name_p));

    dce_assert(cds__svc_handle,cth_p != NULL);


    if (!th_p || th_p->th_detached) 
    {
        RETURN_ERROR(ERR_INVALID);
    }


    /*
     * Set thread detached, and unplug life support.
     */
    THREAD_LOCK(th_p) 
    {
	pthread_t detach_thread;

	th_p->th_detached++;
	detach_thread = th_p->th_pthread;
	(void)pthread_detach(&detach_thread);
	(void)dthread_current_thread(cth_p);

    } THREAD_END_LOCK(th_p);

    if (th_p->th_exited)
    {
	th_death(th_p);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " <dthread_detach"));

    return(status);
}

/*
 * ROUTINE - dthread_exit
 *
 * Exit process.
 *
 * No inputs or outputs.
 */
void
dthread_exit (dthread_address_t	status)
{
    private_thread *const cth_p = CURRENT_THREAD;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >dthread_exit(0x%p) thread(0x%p->%s)",
        status,
        cth_p,
        cth_p->th_name_p));

    dce_assert(cds__svc_handle,cth_p != NULL);

    cth_p->th_exited++;

    /*
     * Another thread bites the dust.
     */
    THREAD_LOCK(cth_p) 
    {
	cth_p->th_exit = status;
	cth_p->th_state = state_exiting;
    } THREAD_END_LOCK(cth_p);

    pthread_exit((pthread_addr_t)status);

    /*
     * th_death() called through the key destructor 
     */

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " <dthread_exit()"));

}

/*
 * ROUTINE - dthread_self
 *
 * Returns the id for the current thread.
 *
 * Results:
 *    Thread pointer value.
 *
 */
dthread_t
dthread_self (void)
{
    private_thread *const cth_p = CURRENT_THREAD;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >dthread_self"));

    dce_assert(cds__svc_handle,cth_p != NULL);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " <dthread_self"));

    return((dthread_t)cth_p);
}

/*
 * ROUTINE - dthread_yield
 *
 * Allow other processes to run.
 *
 * No Inputs
 *
 * Returns:
 *    0 - Thread was yielded and has now resumed.
 *    ??? - Stack switcher failed - bugcheck time.
 */
int
dthread_yield (void)
{
    private_thread *const cth_p = CURRENT_THREAD;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >dthread_yield()"));

    dce_assert(cds__svc_handle,cth_p != NULL);

    pthread_yield();
    (void)dthread_current_thread(cth_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " <dthread_yield()"));

    RETURN_SUCCESS;
}



/*
 * ROUTINE - dthread_delay
 *
 *    Suspend the current thread for the amount of time specified.
 *
 * Arguments:
 *    interval - how long to wait (in seconds)
 *
 * Returns:
 *    0 - Thread was delayed and has now resumed.
 *    ??? - Stack switcher failed - bugcheck time.
 */
int
dthread_delay (dthread_interval_t	interval)
{
    private_thread *const cth_p = CURRENT_THREAD;
    struct timespec timeout;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >dthread_delay(%f)",
        interval));

    dce_assert(cds__svc_handle,cth_p != NULL);

    /*
     * Set the timer and insert in the timer_queue.
     */

    cth_p->th_state = state_delay;
    timeout.tv_sec = interval;
    timeout.tv_nsec = 0;

    (void)pthread_delay_np(&timeout);
    (void)dthread_current_thread(cth_p);
    cth_p->th_state = state_runnable;

   DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " <dthread_delay"));

    RETURN_SUCCESS;
}



/*
 * ROUTINE - dthread_get_errno
 *
 * Returns the errno for the current thread if in thread context, otherwise
 * it returns -1 (which is meant to imply that there is something wrong, since
 * you shouldn't be calling this out of thread context).
 *
 */
int
dthread_get_errno (void)
{
    private_thread *const cth_p = CURRENT_THREAD;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >dthread_get_errno"));

    dce_assert(cds__svc_handle,cth_p != NULL);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " <dthread_get_errno"));

    return (cth_p->th_errno);
}

/*
 * dthread_set_errno
 *
 * Sets the errno for the current thread if in thread context, otherwise
 * it bugchecks.
 *
 * Arguments:
 *    error_code (in) - Value for the per-thread errno.
 */
void
dthread_set_errno (int	error_code)
{
    private_thread *cth_p = CURRENT_THREAD;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >dthread_set_errno"));

    dce_assert(cds__svc_handle,cth_p != NULL);
    cth_p->th_errno = error_code;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " <dthread_set_errno"));
}



/*
 * ROUTINE - dthread_create_mutex
 *
 * Creates a mutex and returns the mutex handle.
 *
 * Arguments:
 *    new_mutex (out) - Address to store the mutex handle.
 *
 * Returns:
 *    0 - mutex created.
 *    ERR_NO_MEMORY - Unable to create mutex (outa memory)
 */
int
dthread_create_mutex (dthread_mutex_t	*new_mutex)
{
    private_thread *const cth_p = CURRENT_THREAD;
    private_mutex *m_p;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >dthread_create_mutex()"));

    *new_mutex = (dthread_mutex_t)alloc_handle(sizeof(private_mutex));
    m_p = (private_mutex *)handle_data(*new_mutex);
    if (m_p == NULL) 
    {
        RETURN_ERROR(ERR_NO_MEMORY);
    }

    if (pthread_mutex_init(&m_p->mutex_id, pthread_mutexattr_default) < 0) 
    {
	cth_p->th_errno = errno;
	free_handle(m_p);

	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_library,
            svc_c_debug9,
            " <dthread_create_mutex: return(POSIX_ERROR_STATUS)"));

	return (POSIX_ERROR_STATUS);
    }

    DNS_LOCK 
    {
	num_mutexes++;
	if (num_mutexes > peak_mutexes) 
	{
	    peak_mutexes = num_mutexes;
	}
    } DNS_END_LOCK;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " <dthread_create_mutex: return(SUCCESS)"));

    RETURN_SUCCESS;
}



/*
 * ROUTINE - dthread_delete_mutex
 *
 * Deletes a mutex.
 *
 * Arguments:
 *    mutex (in out) - Pointer to a mutex handle.
 *
 * Returns:
 *    0 - Mutex deleted.
 *    ERR_INVALID - Pointer doesn't point to a mutex.
 *    ERR_BUSY - Mutex is in use.
 */
int
dthread_delete_mutex (dthread_mutex_t	*mutex)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >dthread_delete_mutex(0x%p)",
        mutex));

    /*
     * if not a valid mutex object, ERR_INVALID 
     */

    if (!dthread_equal(*mutex, dthread_null)) 
    {
	private_mutex *m_p = (private_mutex *)handle_data(*mutex);

	(void)pthread_mutex_destroy(&m_p->mutex_id);

	DNS_LOCK 
	{
	    --num_mutexes;
	} DNS_END_LOCK;

        free_handle(m_p);
	dthread_assign(dthread_null, mutex); /* Don't leave a dangling mutex */

    } 
    else 
    {
        RETURN_ERROR(ERR_INVALID);
    }

    RETURN_SUCCESS;
}



/*
 * ROUTINE - dthread_lock
 *
 * Forms a lock with a specified mutex.  If the mutex is already locked
 * then the current thread waits for the mutex to become available.
 *
 * Arguments:
 *    mutex (in) - Pointer to a mutex handle.
 *
 * Returns:
 *    0 - Lock granted.
 *    ERR_INVALID - Pointer doesn't point to a mutex.
 *    ERR_BUSY - Mutex is in use.
 */
int
dthread_lock (dthread_mutex_t	*mutex)
{
    private_thread *const cth_p = CURRENT_THREAD;
    private_mutex *m_p = (private_mutex *)handle_data(*mutex);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >dthread_lock(0x%p)",
        m_p));


    if (!m_p) 
    {
        RETURN_ERROR(ERR_INVALID);
    }


    mutex_locks++;

    cth_p->th_state = state_wait;
    cth_p->th_mutex = *mutex;

    (void)pthread_mutex_lock(&m_p->mutex_id);
    (void)dthread_current_thread(cth_p);

    cth_p->th_state = state_runnable;
    cth_p->th_mutex = (dthread_mutex_t)handle_data(dthread_null);

    m_p->owner = cth_p;

    RETURN_SUCCESS;
}



/*
 * ROUTINE - dthread_trylock
 *
 * Tests the mutex to see if it owned by anyone.  If it is not, then
 * the lock is granted to the caller (i.e., the caller now owns the mutex)
 * and the routine returns TRUE.  If someone else owns the lock, the routine
 * returns FALSE.
 *
 * Arguments:
 *    mutex (in) - Pointer to a mutex handle.
 *
 * Returns:
 *    1 - Lock granted.                  (Constants agree with POSIX)
 *    0 - Mutex is in use.
 *    ERR_INVALID - Mutex is invalid.
 */
int
dthread_trylock (dthread_mutex_t	*mutex)
{
    private_thread *const cth_p = CURRENT_THREAD;
    private_mutex *m_p = (private_mutex *)handle_data(*mutex);
    int lock_status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >dthread_trylock(0x%p)",
        m_p));


    if (m_p == NULL) 
    {
        RETURN_ERROR(ERR_INVALID);
    }


    cth_p->th_state = state_wait;
    cth_p->th_mutex = *mutex;

    if ((lock_status = pthread_mutex_trylock(&m_p->mutex_id)) < 0)
    {
	cth_p->th_errno = errno;
    }

    cth_p->th_mutex = (dthread_mutex_t)handle_data(dthread_null);
    cth_p->th_state = state_runnable;
    m_p->owner = cth_p;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " <dthread_trylock: return(lock_status)"));

    return (lock_status);
}



/*
 * ROUTINE - dthread_unlock
 *
 * Unlocks a mutex - if the caller owns it.
 *
 * Arguments:
 *    mutex (in) - Pointer to a mutex handle.
 *
 * Returns:
 *    ERR_INVALID - Mutex is invalid.
 *    ERR_BUSY - Mutex is locked, but not by the caller.
 *    0 - Mutex unlocked.
 */
int
dthread_unlock (dthread_mutex_t	*mutex)
{
    private_thread *const cth_p = CURRENT_THREAD;
    private_mutex *m_p = (private_mutex *)handle_data(*mutex);


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >dthread_unlock(0x%p)",
        m_p));


    if (m_p == NULL) 
    {
        RETURN_ERROR(ERR_INVALID);
    }


    m_p->owner = NULL;

    if (pthread_mutex_unlock(&m_p->mutex_id) < 0) 
    {
        cth_p->th_errno = errno;

	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_library,
            svc_c_debug9,
            " <dthread_unlock: return (POSIX_ERROR_STATUS)"));

	return (POSIX_ERROR_STATUS);
    }

    RETURN_SUCCESS;
}




/*
 * ROUTINE - dthread_create_condition
 *
 * Creates a condition variable.
 *
 * Arguments:
 *    new_condition (out) - Pointer to a condition variable handle.
 *
 * Returns:
 *    0 - Condition variable created.
 *    ERR_NO_MEMORY - No resources available to create a condition variable.
 */
int
dthread_create_condition (dthread_cond_t	*new_condition)
{
    private_thread *const cth_p = CURRENT_THREAD;
    private_condition *c_p;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >dthread_create_condition()"));

    *new_condition = (dthread_cond_t)alloc_handle(sizeof(private_condition));
    c_p = (private_condition *)handle_data(*new_condition);
    if (c_p == NULL) 
    {
        RETURN_ERROR(ERR_NO_MEMORY);
    }

    if (pthread_cond_init(&c_p->condition_id, pthread_condattr_default) < 0) 
    {
	cth_p->th_errno = errno;
	free_handle(c_p);

	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_library,
            svc_c_debug9,
            " <dthread_create_condition: return (POSIX_ERROR_STATUS)"));

	return (POSIX_ERROR_STATUS);
    }

    DNS_LOCK 
    {
	num_conditions++;
	if (num_conditions > peak_conditions) 
	{
	    peak_conditions = num_conditions;
	}
    } DNS_END_LOCK;

    RETURN_SUCCESS;
}



/*
 * ROUTINE - dthread_delete_condition
 *
 * Deletes a condition variable.
 *
 * Arguments:
 *    condition (in out) - Pointer to a condition variable handle.
 *
 * Returns:
 *    0 - Condition variable deleted.
 *    ERR_INVALID - Handle does not reference a valid condition variable.
 *    ERR_BUSY - Condition variable is in use.
 */
int
dthread_delete_condition (dthread_cond_t	*condition)
{
    private_condition *c_p = (private_condition *)handle_data(*condition);

    /*
     * if not a valid condition object, ERR_INVALID 
     */

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >dthread_delete_condition(0x%p)",
        condition));

    if (!dthread_equal(*condition, dthread_null)) 
    {
	private_condition *c_p = (private_condition *)handle_data(*condition);

	(void)pthread_cond_destroy(&c_p->condition_id);

	DNS_LOCK 
	{
	    --num_conditions;
	} DNS_END_LOCK;
    } 
    else 
    {
        RETURN_ERROR(ERR_INVALID);
    }

    free_handle(c_p);
    dthread_assign(dthread_null, condition);  /* Don't leave a dangling cv */

    RETURN_SUCCESS;
}



/*
 * ROUTINE - dthread_broadcast
 *
 * Wakes up all threads waiting on the condition variable.
 *
 * Arguments:
 *    condition (in) - Pointer to a condition variable handle.
 *
 * Returns:
 *    0 - Broadcast completed successfully.
 *    ERR_INVALID - Handle does not reference a valid condition variable.
 */
int
dthread_broadcast (dthread_cond_t	*condition)
{
		
    private_thread *const cth_p = CURRENT_THREAD;
    private_condition *c_p = (private_condition *)handle_data(*condition);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >dthread_broadcast(0x%p)",
        c_p));


    if (c_p == NULL) 
    {
        RETURN_ERROR(ERR_INVALID);
    }


    if (pthread_cond_broadcast(&c_p->condition_id) < 0) 
    {
        cth_p->th_errno = errno;

	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_library,
            svc_c_debug9,
            " <dthread_broadcast: return (POSIX_ERROR_STATUS)"));

	return (POSIX_ERROR_STATUS);
    }

    RETURN_SUCCESS;
}



/*
 * ROUTINE - dthread_signal
 *
 * Wakes up one thread waiting on the condition variable.
 *
 * Arguments:
 *    condition (in) - Pointer to a condition variable handle.
 *
 * Returns:
 *    0 - Signal completed successfully.
 *    ERR_INVALID - Handle does not reference a valid condition variable.
 */
int
dthread_signal (dthread_cond_t	*condition)
{
    private_thread *const cth_p = CURRENT_THREAD;
    private_condition *c_p = (private_condition *)handle_data(*condition);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >dthread_signal(0x%p)",
        c_p));


    if (pthread_cond_signal(&c_p->condition_id) < 0) 
    {
        cth_p->th_errno = errno;

	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_library,
            svc_c_debug9,
            " <dthread_signal: return (POSIX_ERROR_STATUS)"));

	return (POSIX_ERROR_STATUS);
    }

    RETURN_SUCCESS;
}



/*
 * ROUTINE - dthread_wait
 *
 * Should be called with the mutex lock.  This routine will unlock the
 * mutex and wait on the condition variable.  When the wait is satisfied,
 * the mutex is reacquired.
 *
 * Arguments:
 *    condition (in) - Pointer to a condition variable handle to wait on.
 *    mutex (in) - Pointer to mutex associated with the condition variable.
 *
 * Returns:
 *    0 - Wait completed successfully.
 *    ERR_INVALID - Handle does not reference a valid condition variable.
 *    Errors returned by dthread_unlock and dthread_lock
 */
int
dthread_wait (dthread_cond_t	*condition,
              dthread_mutex_t	*mutex)
{
    private_thread *const cth_p = CURRENT_THREAD;
    private_mutex *m_p = (private_mutex *)handle_data(*mutex);
    private_condition *c_p = (private_condition *)handle_data(*condition);
    int status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >dthread_wait(0x%p,0x%p)",
        c_p,
        m_p));


    if (c_p == NULL) 
    {
        RETURN_ERROR(ERR_INVALID);
    }


    cth_p->th_state = state_wait;
    cth_p->th_mutex = *mutex;
    cth_p->th_condition = *condition;

    if (pthread_cond_wait(&c_p->condition_id, &m_p->mutex_id) < 0) 
    {
	cth_p->th_errno = errno;
	status = POSIX_ERROR_STATUS;
    } 
    else
    {
	status = POSIX_SUCCESS_STATUS;
    }

    (void)dthread_current_thread(cth_p);
    cth_p->th_state = state_runnable;
    cth_p->th_condition = (dthread_cond_t)handle_data(dthread_null);
    cth_p->th_mutex = (dthread_mutex_t)handle_data(dthread_null);


    DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_library,
            svc_c_debug9,
            " <dthread_wait: return(status)"));

    return (status);
}



/*
 * ROUTINE - dthread_timed_wait
 *
 * Should be called with the mutex lock.  This routine will unlock the
 * mutex and wait on the condition variable.  When the wait is satisfied,
 * the mutex is reacquired.  The wait is cancelled if the current time
 * exceeds the expiration time.
 *
 * Arguments:
 *    condition (in) - Pointer to a condition variable handle to wait on.
 *    mutex (in) - Pointer to mutex associated with the condition variable.
 *    expiration (in) - Pointer to expiration date and time.
 *
 * Returns:
 *    0 - Wait completed successfully.
 *    ERR_INVALID - Handle does not reference a valid condition variable.
 *    Errors returned by dthread_unlock and dthread_lock
 *    ??? - Stack switcher failed - bugcheck time.
 */
int
dthread_timed_wait (dthread_cond_t	*condition,
                    dthread_mutex_t	*mutex,
                    dthread_date_time_t	*expiration)
{
    DEB_ASCII_BUF_time_local(expirationBuf)
    struct timespec expiration_time;
    private_thread *const cth_p = CURRENT_THREAD;
    private_mutex *m_p = (private_mutex *)handle_data(*mutex);
    private_condition *c_p = (private_condition *)handle_data(*condition);
    int status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >dthread_timed_wait(0x%p,0x%p,%s)",
        c_p,
        m_p,
        deb_ascii_time_local(expirationBuf,expiration)));


    if (c_p == NULL) 
    {
        RETURN_ERROR(ERR_INVALID);
    }


    sys_time_to_timespec(expiration, &expiration_time);

    /*
     * Set the thread state to waiting, and then unlock the mutex 
     */

    cth_p->th_state = state_timed_wait;
    cth_p->th_mutex = *mutex;
    cth_p->th_condition = *condition;
    cth_p->th_timer = *expiration;

    if (pthread_cond_timedwait(&c_p->condition_id, &m_p->mutex_id, &expiration_time) < 0) 
    {
	cth_p->th_errno = errno;
	status = POSIX_ERROR_STATUS;
    } 
    else
    {
	status = POSIX_SUCCESS_STATUS;
    }

    (void)dthread_current_thread(cth_p);
    cth_p->th_state = state_runnable;
    cth_p->th_condition = (dthread_cond_t)handle_data(dthread_null);
    cth_p->th_mutex = (dthread_mutex_t)handle_data(dthread_null);
    cth_p->th_timer = zero_timer;

    DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_library,
            svc_c_debug9,
            " <dthread_timed_wait: return(cth_p->th_fork_ret)"));

    return(status);
}

/*
 * ROUTINE - dthread_create_key
 *
 * Generates a context key for per-thread context variables.
 * This is a simplification of the mechanism that is used by CMA.
 *
 * Arguments:
 *    destructor - pointer to procedure used to free the handle [typically
 *                 the procedure used is free()]
 *    key - address of dthread_key_t to receive context key handle.
 *
 * Returns:
 *    ERR_NO_MEMORY - if there was an error (or no more available keys)
 */
int
dthread_create_key (dthread_destructor	destructor,
                    dthread_key_t	*key)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >dthread_create_key()"));

    if (pthread_keycreate(key, (pthread_destructor_t)destructor) < 0) 
    {
        RETURN_ERROR(errno);
    }

    RETURN_SUCCESS;
}



/*
 * ROUTINE - dthread_set_context
 *
 * This sets the per-thread context for the associated key.  If there
 * already was a per-thread context for the key, then it is replaced
 * by the new value.
 *
 * Arguments:
 *    key - The context key associated with this context variable
 *    context_value - The pointer to the context variable
 *
 * Returns:
 *    0 - Set O.K.
 *    ERR_INVALID - Bad key.
 */
int
dthread_set_context (dthread_key_t	key,
                     dthread_address_t	context_value)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >dthread_set_context(%d, 0x%p)",
        key,
        context_value));

    if (pthread_setspecific((pthread_key_t)key, (pthread_addr_t)context_value) < 0) 
    {
	RETURN_ERROR(ERR_INVALID);
    }

    RETURN_SUCCESS;
}



/*
 * ROUTINE - dthread_get_context
 *
 * Retrieves a per-thread context variable based on a context key.
 *
 * Arguments:
 *    key - Context key to use to get the context variable.
 *    value - variable to receive context variable for this thread.
 *
 * Returns:
 *    ERR_INVALID if key is not defined.
 */
int
dthread_get_context (dthread_key_t	key,
                     dthread_address_t	*value)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >dthread_get_context(%d)",
        key));

    if (pthread_getspecific((pthread_key_t)key, (pthread_addr_t *)value) < 0)
    {
	RETURN_ERROR(errno);
    }

    RETURN_SUCCESS;
}



/*
 * ROUTINE - dthread_delete_context
 *
 * Deletes the per-thread context variable for the current thread based
 * on the key.  This procedure is implicitly run for all per-thread
 * contexts when the thread is terminating.
 *
 * Arguments:
 *    key - Context key to use to delete the context variable.
 *
 * Returns:
 *    0 - Done.
 *    ERR_INVALID - Bad key.
 */
int
dthread_delete_context (dthread_key_t	key)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >dthread_delete_context(%d)",
        key));

    /*
     * do nothing 
     */

    RETURN_SUCCESS;
}

/*
 * ROUTINE - dthread_cancel
 *
 * Schedules the specified thread to die at the next cancel point.
 *
 * Arguments:
 *    thread (out) - Thread to cancel.
 *
 */
int
dthread_cancel (dthread_t		thread)
{
    private_thread *const cth_p = CURRENT_THREAD;
    private_thread *th_p = (private_thread *)thread;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >dthread_cancel(0x%p->%s)",
        th_p,
        th_p->th_name_p));

    if (!th_p->th_cancelled) 
    {
	th_p->th_cancelled = 1;
	th_p->th_exited++;

	if (pthread_cancel(th_p->th_pthread) < 0) 
	{
	    cth_p->th_errno = errno;

	    DCE_SVC_DEBUG((
              cds__svc_handle,
              cds_svc_library,
              svc_c_debug9,
              " <dthread_cancel: return(POSIX_ERROR_STATUS)"));

	    return(POSIX_ERROR_STATUS);
	}
	(void)dthread_current_thread(cth_p);
    }

    RETURN_SUCCESS;
}

/*
 * ROUTINE - dthread_handler_push
 *
 * Push a cleanup handler on the thread's stack.
 *
 * Arguments:
 *    ch_p (in) - Address of cleanup handler data.
 *
 */
void
dthread_handler_push (dthread_cleanup_handler_t *ch_p)
{
    private_thread *const cth_p = CURRENT_THREAD;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >dthread_handler_push(0x%p->0x%p(0x%p))",
        ch_p,
        ch_p->ch_rtn,
        ch_p->ch_arg));

    ch_p->ch_lnk = cth_p->th_ch;
    cth_p->th_ch = ch_p;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " <dthread_handler_push(0x%p->0x%p(0x%p))",
        ch_p,
        ch_p->ch_rtn,
        ch_p->ch_arg));
}

/*
 * ROUTINE - dthread_handler_pop
 *
 * Pop cleanup data off thread's stack
 *
 * Arguments:
 *    ch_p (in) - Address of cleanup handler data.
 *
 */
void
dthread_handler_pop (dthread_cleanup_handler_t *ch_p)
{
    private_thread *const cth_p = CURRENT_THREAD;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >dthread_handler_pop(0x%p->0x%p(0x%p),%d)",
        ch_p,
        ch_p->ch_rtn,
        ch_p->ch_arg,
        ch_p->ch_ex));

    if (ch_p != cth_p->th_ch)
    {
	BUGCHECK(BUG_CLEANUP);
    }

    if (ch_p->ch_ex) (*ch_p->ch_rtn)(ch_p->ch_arg);
    cth_p->th_ch = ch_p->ch_lnk;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " <dthread_handler_pop(0x%p->0x%p(0x%p),%d)",
        ch_p,
        ch_p->ch_rtn,
        ch_p->ch_arg,
        ch_p->ch_ex));
}

/*
 * ROUTINE - dthread_setcancel
 *
 * Set the thread's general cancelability and return previous state.
 *
 * Arguments:
 *    state (in) - New general cancelability state
 *
 * Returns:
 *    Previous cancelability state
 */
int
dthread_setcancel (int			state)
{
    private_thread *const cth_p = CURRENT_THREAD;
    register int old_state;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >dthread_setcancel(%d)",
        state));

    old_state = pthread_setcancel(state);
    cth_p->th_cancel = state;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " <dthread_setcancel(%d) return(%d)",
        state,
        old_state));


    return(old_state);
}

/*
 * ROUTINE - dthread_testcancel
 *
 * Returns the id for the current thread.
 *
 * Arguments:
 *    thread (out) - Address to store the thread pointer value.
 *
 */
void
dthread_testcancel (void)
{
    private_thread *const cth_p = CURRENT_THREAD;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >dthread_testcancel()"));

    pthread_testcancel();
    (void)dthread_current_thread(cth_p);
}

/*
 * ROUTINE - dthread_accept ONLY COMPILED UNDER UNIX
 *
 * Pause thread until connection received on socket
 *
 * Arguments:
 *	s (in) - socket descriptor.
 *	addr (in) - address to receive source socket address
 *	addrlen (in) - pointer to length of socket address structure
 */
int
dthread_accept (int               s,
                struct sockaddr   *addr,
                int               *addrlen)
{
    private_thread *const cth_p = CURRENT_THREAD;
    int addrcc = (addrlen) ? *addrlen : 0;
    int ns;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >dthread_accept(%d,0x%p,0x%p->%d)",
        s,
        addr,
        addrlen,
        addrcc));

    cth_p->th_state = state_read;
    ns = accept(s, addr, addrlen);
    cth_p->th_state = state_runnable;
    (void)dthread_current_thread(cth_p);

    if (addrlen) 
    {
	addrcc = *addrlen;
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " <dthread_accept(%d,0x%p,0x%p->%d) return(%d)",
        s,
        addr,
        addrlen,
        addrcc,
        ns));

    return(ns);
}



/*
 * ROUTINE - dthread_close ONLY COMPILED UNDER UNIX
 *
 * Cancel any I/O on descriptor and close
 *
 * Arguments:
 *	fd (in) - file descriptor.
 */
int
dthread_close (int			fd)
{
    private_thread *const cth_p = CURRENT_THREAD;
    int sts;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >dthread_close(%d)",
        fd));

    sts = close(fd);
    (void)dthread_current_thread(cth_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " <dthread_close(%d) return(%d)",
        fd,
        sts));

    return (sts);
}



/*
 * ROUTINE - dthread_connect ONLY COMPILED UNDER UNIX
 *
 * Pause thread until connection is made on socket
 *
 * Arguments:
 *	s (in) - socket descriptor.
 *	name (in) - address of target socket address
 *	namelen (in) - length of socket address structure
 */
int
dthread_connect (int			s,
                 struct sockaddr	*name,
                 int			namelen)
{
    private_thread *const cth_p = CURRENT_THREAD;
    int sts;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >dthread_connect(%d,0x%p,%d)",
        s,
        name,
        namelen));

    cth_p->th_state = state_read_write;
    sts = connect(s, name, namelen);
    cth_p->th_state = state_runnable;
    (void)dthread_current_thread(cth_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " <dthread_connect(%d,0x%p,%d) return(%d)",
        s,
        name,
        namelen,
        sts));

    return(sts);
}



/*
 * ROUTINE - dthread_creat ONLY COMPILED UNDER UNIX
 *
 * Create a file and make non-blocking
 *
 * Arguments:
 *	name (in) - address of file name
 *	mode (in) - file mode
 */
int
dthread_creat (char		*name,
               int		mode)
{
    int sts;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >dthread_creat(%s,%d)",
        name,
        mode));

    sts = creat(name, mode);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " <dthread_creat(%s,%d) return(%d)",
        name,
        mode,
        sts));

    return(sts);
}



/*
 * ROUTINE - dthread_open ONLY COMPILED UNDER UNIX
 *
 * Open a file and make non-blocking
 *
 * Arguments:
 *	path (in) - address of file name
 *	flags (in) - open flags
 *	mode (in) - file mode for (flags == O_CREAT)
 */
int
dthread_open (char     *path,
              int	flags,
              int	mode)
{
    int sts;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >dthread_open(%s,%d,%d)",
        path,
        flags,
        mode));

    sts = open(path, flags, mode);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " <dthread_open(%s,%d,%d) return(%d)",
        path,
        flags,
        mode,
        sts));

    return(sts);
}



/*
 * ROUTINE - dthread_recvfrom ONLY COMPILED UNDER UNIX
 *
 * Pause thread until message is received on descriptor
 *
 * Arguments:
 *	s (in) - socket descriptor.
 *	buf (in) - address of buffer to receive data
 *	len (in) - maximum data to read
 *	flags (in) - flags (MSG_OOB | MSG_PEEK)
 *	from (out) - received source address
 *	fromlen (in/out) - sizeof(*from) becomes actual length
 */
int
dthread_recvfrom (int			s,
                  char		        *buf,
                  int			len,
                  int			flags,
                  struct sockaddr	*from,
                  int			*fromlen)
{
    private_thread *const cth_p = CURRENT_THREAD;
    int cc, fromcc = (fromlen) ? *fromlen : 0;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >dthread_recvfrom(%d,0x%p,%d,%d,0x%p,0x%p->%d)",
        s,
        buf,
        len,
        flags,
        from,
        fromlen,
        fromcc));

    cth_p->th_state = state_read;
    cc = recvfrom(s, buf, len, flags, from, fromlen);
    cth_p->th_state = state_runnable;
    (void)dthread_current_thread(cth_p);

    if (fromlen) 
    {
	fromcc = *fromlen;
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " <dthread_recvfrom(%d,0x%p,%d,%d,0x%p,0x%p->%d) return(%d)",
        s,
        buf,
        len,
        flags,
        from,
        fromlen,
        fromcc,
        cc));

    return(cc);
}



/*
 * ROUTINE - dthread_read ONLY COMPILED UNDER UNIX
 *
 * Pause thread until data is read on descriptor
 *
 * Arguments:
 *	fd (in) - file descriptor.
 *	buf (in) - address of buffer to receive data
 *	nbytes (in) - maximum data to read
 */
int
dthread_read (int			fd,
              char		       *buf,
              int			nbytes)
{
    private_thread *const cth_p = CURRENT_THREAD;
    int cc;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >dthread_read(%d,0x%p,%d)",
        fd,
        buf,
        nbytes));

    cth_p->th_state = state_read;
    cc = read(fd, buf, nbytes);
    cth_p->th_state = state_runnable;
    (void)dthread_current_thread(cth_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " <dthread_read(%d,0x%p,%d) return(%d)",
        fd,
        buf,
        nbytes,
        cc));

    return(cc);
}



/*
 * ROUTINE - dthread_sendto ONLY COMPILED UNDER UNIX
 *
 * Pause thread until data is sent on descriptor
 *
 * Arguments:
 *	s (in) - file descriptor.
 *	msg (in) - address of buffer to receive data
 *	len (in) - maximum data to read
 *	flags (in) - flags (MSG_OOB | MSG_PEEK)
 *	to (out) - destination address
 *	tolen (in) - sizeof(*to)
 */
int
dthread_sendto (int			s,
                char		       *msg,
                int			len,
                int			flags,
                struct sockaddr	       *to,
                int			tolen)
{
    private_thread *const cth_p = CURRENT_THREAD;
    int cc;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >dthread_sendto(%d,0x%p,%d,%d,0x%p,%d)",
        s,
        msg,
        len,
        flags,
        to,
        tolen));

    cth_p->th_state = state_write;
    cc = sendto(s, msg, len, flags, to, tolen);
    cth_p->th_state = state_runnable;
    (void)dthread_current_thread(cth_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " <dthread_sendto(%d,0x%p,%d,%d,0x%p,%d) return(%d)",
        s,
        msg,
        len,
        flags,
        to,
        tolen,
        cc));

    return(cc);
}



/*
 * ROUTINE - dthread_socket ONLY COMPILED UNDER UNIX
 *
 * Open a socket and make non-blocking
 *
 * Arguments:
 *	af (in) - address family
 *	type (in) - socket type
 *	protocol (in) - protocol
 */
int
dthread_socket (int			af,
                int			type,
                int			protocol)
{
    int sts;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >dthread_socket(%d,%d,%d)",
        af,
        type,
        protocol));

    sts = socket(af, type, protocol);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " <dthread_socket(%d,%d,%d) return(%d)",
        af,
        type,
        protocol,
        sts));

    return(sts);
}



/*
 * ROUTINE - dthread_write ONLY COMPILED UNDER UNIX
 *
 * Pause thread until data is written to descriptor
 *
 * Arguments:
 *	fd (in) - file descriptor.
 *	buf (in) - address of data to write
 *	nbytes (in) - maximum data to write
 */
int
dthread_write (int		fd,
               char		*buf,
               int		nbytes)
{
    private_thread *const cth_p = CURRENT_THREAD;
    int cc;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >dthread_write(%d,0x%p,%d)",
        fd,
        buf,
        nbytes));

    cth_p->th_state = state_write;
    cc = write(fd, buf, nbytes);
    cth_p->th_state = state_runnable;
    (void)dthread_current_thread(cth_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " <dthread_write(%d,0x%p,%d) return(%d)",
        fd,
        buf,
        nbytes,
        cc));

    return(cc);
}

/*
 * ROUTINE - dthread_import_fd ONLY COMPILED UNDER UNIX
 *
 * Make file descriptor non-blocking
 *
 * Arguments:
 *	fd (in) - file descriptor to use with dthread calls
 */
void
dthread_import_fd (int			fd)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >dthread_import_fd(%d)",
        fd));

/* 
 * If we have DECthreads which doesn't support thread syncronous I/O
 * (e.g. user mode threads), then tell CMA about this file descriptor.
 */

#if defined(_DECTHREADS_) && !_CMA_THREAD_SYNC_IO_
    cma_import_fd(fd);
#endif

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " <dthread_import_fd(%d)",
        fd));
}


#else	/* !DTHREAD_WRAPPERS */
/*
 * ROUTINE - dthread_create
 *
 * Fork a new thread.
 *
 * Arguments:
 *     new_thread     - (out) address to receive handle to new thread.
 *     attr           - (in) pointer to threads attribute block
 *     start_routine  - (in) routine to handle new thread
 *     arg            - (in) argument to pass to new thread.
 *
 * Returns:
 *     0 - New thread has been successfully forked.
 *     ERR_NO_MEMORY - Unable to allocate space for the thread.
 */
int
dthread_create (dthread_t		*new_thread,
                dthread_attr_t	        *attr,
                dthread_start_routine   start_routine,
                dthread_address_t	arg)
{
    pthread_attr_t pattr;
    const long stack_req = attr ? attr->stacksize : 0;
#if defined(_DECTHREADS_) && (_CMA_OSIMPL_ == _CMA__OS_OSF) && defined(__alpha)
    const long stack_size = stack_req ? stack_req+8*1024 : _CDS_DEFAULT_STACK_;
#else
    const long stack_size = stack_req ? stack_req : _CDS_DEFAULT_STACK_;
#endif
    int status;

    
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >dthread_create"));

    /*
     * Set requested stack size, if not defaulted
     */
    if (_CDS_DEFAULT_STACK_ || stack_size) {
	if ((status = pthread_attr_create(&pattr)) < 0)
	{
	    DCE_SVC_DEBUG((
		  cds__svc_handle,
		  cds_svc_library,
		  svc_c_debug9,
		  " <dthread_create: (1) return(status)"));

	    return(status);
	}
	if ((status = pthread_attr_setstacksize(&pattr, stack_size)) < 0) {
	    {
	    pthread_attr_delete(&pattr);

	    DCE_SVC_DEBUG((
		  cds__svc_handle,
		  cds_svc_library,
		  svc_c_debug9,
		  " <dthread_create: (2) return(status)"));

	    return(status);
	    }
	}
     } else
	pattr = pthread_attr_default;

    /*
     * Ask CMA to create the thread
     */
    status = pthread_create(new_thread, pattr, start_routine, arg);

    if (_CDS_DEFAULT_STACK_ || stack_size)
	pthread_attr_delete(&pattr);

    DCE_SVC_DEBUG((
              cds__svc_handle,
              cds_svc_library,
              svc_c_debug9,
              " <dthread_create: (3) return(status)"));

    return(status);
}

/*
 * ROUTINE - dthread_create_with_stack
 *
 * Fork a new thread with a given stack size.
 *
 * Arguments:
 *     new_thread (out) - address to receive handle to new thread.
 *     start_routine (in) - routine to handle new thread
 *     arg (in) - argument to pass to new thread.
 *     stack_size (in) - requested stack size or zero for default.
 *
 * Returns:
 *     0 - New thread has been successfully forked.
 *     ERR_NO_MEMORY - Unable to allocate space for the thread.
 */
int
dthread_create_with_stack (dthread_t		*new_thread,
                           dthread_start_routine start_routine,
                           dthread_address_t	 arg,
                           unsigned int	         stack_size)
{
    pthread_attr_t pattr;
    int status;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >dthread_create_with_stack"));
    /*
     * Set requested stack size, if not defaulted
     */
    if (stack_size == 0)
    {
	stack_size = _CDS_DEFAULT_STACK_;
    }
    
#if defined(_DECTHREADS_) && (_CMA_OSIMPL_ == _CMA__OS_OSF) && defined(__alpha)

    else
    {
	stack_size += 8*1024;
    }

#endif

    if ((status = pthread_attr_create(&pattr)) < 0)
    {
	DCE_SVC_DEBUG((
              cds__svc_handle,
              cds_svc_library,
              svc_c_debug9,
              " <dthread_create_with_stack: (1) return(status)"));

	return(status);
    }

    if ((status = pthread_attr_setstacksize(&pattr, stack_size)) < 0) 
    {
	pthread_attr_delete(&pattr);
	DCE_SVC_DEBUG((
              cds__svc_handle,
              cds_svc_library,
              svc_c_debug9,
              " <dthread_create_with_stack: (2) return(status)"));

	return(status);
    }

    /*
     * Ask CMA to create the thread
     */
    status = pthread_create(new_thread, pattr, start_routine, arg);

    (void)pthread_attr_delete(&pattr);

    DCE_SVC_DEBUG((
              cds__svc_handle,
              cds_svc_library,
              svc_c_debug9,
              " <dthread_create_with_stack: (3) return(status)"));

    return(status);
}

/*
 * ROUTINE - dthread_delay_np
 *
 *    Suspend the current thread for the amount of time specified.
 *
 * Arguments:
 *    interval - how long to wait (in seconds)
 *
 * Returns:
 *    0 - Thread was delayed and has now resumed.
 *    ??? - Stack switcher failed - bugcheck time.
 */
int
dthread_delay_np (dthread_interval_t	interval)
{
    struct timespec timeout;
    int status;


    DCE_SVC_DEBUG((
	cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >dthread_delay_np"));

    timeout.tv_sec = interval;
    timeout.tv_nsec = 0;

    status = pthread_delay_np(&timeout);

    DCE_SVC_DEBUG((
	cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " <dthread_delay_np"));

    return(status);
}
#endif	/* !DTHREAD_WRAPPERS */


/*
 * ROUTINE - dthread_get_expiration
 *
 * Generates an absolute time from a (possibly) delta local time.
 */
int
dthread_get_expiration (time_local_t        *delta,
                        dthread_date_time_t *abstime)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >dthread_get_expiration"));

    sys_time_to_timespec(delta, abstime);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " <dthread_get_expiration"));

    return(1);
}

/*
 * ROUTINE - dthread_diag_threads
 *
 * Diagnostic dumper of threads data structures.
 *
 * This routine can provide diagnostic-level output of private threads
 * structures.  It creates formatted output, which must be handled by a
 * user-supplied routine (e.g., a routine that printf's the output string).
 *
 * Arguments:
 *    s_code (in)       - What structure to dump.
 *    detail_level (in) - What level of detail for the dump (s_code dependent)
 *    thread (in)       - Which thread to dump (use depends on s_code)
 *    mutex  (in)       - Which mutex to dump
 *    condition (in)    - Which condition to dump
 *    emit_routine (in) - Routine to call to emit output.
 *    emit_arg (in)     - Argument for emit_routine
 *
 * Supported s_codes:
 *    1 - Dump the thread control block for the specified thread.
 *    2 - Dump interesting thread statistics (e.g., # of active threads, etc.)
 *    3 - Dump the run queue
 *    4 - Dump the thread log
 *    5 - Dump a mutex
 *    6 - Dump a condition variable
 *    7 - Dump the timer queue
 *    8 - Dump the pool of active threads (all threads currently known)
 *
 * Returns:
 *
 *    0 - Dump completed.
 *   -1 - Error somewhere.
 */
int
dthread_diag_threads (int			s_code,
                      int			detail_level,
                      dthread_t		       *thread_p,
                      dthread_mutex_t	       *mutex_p,
                      dthread_cond_t	       *condition_p,
                      int		        (*emit_routine)(char *, void *),
                      void		        *emit_arg)
{
    char out[255];            /* Output buffer */
#if defined(DTHREAD_WRAPPERS)

    char time_string[50];     /* Work buffer for time conversions */
    private_thread *th_p;
    private_mutex *m_p;
    private_condition *c_p;
    dthread_cleanup_handler_t *ch_p;
    cma_opaque_t *cma_p;
    private_thread *this_p;   /* Iteration variable */
    int x;                    /* Iteration variable */

#define EMIT (void)(*emit_routine)(out, emit_arg)


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >dthread_diag_threads"));

    switch (s_code) 
    {

    case 1:
	th_p = (private_thread *)(thread_p ? handle_data(*thread_p) : 0);
        (void)sprintf(out, 
		      "___ Thread: 0x%p ________________________________", 
		      th_p);
        EMIT;

        if (th_p == NULL) 
	{
            (void)sprintf(out, "** Invalid thread referenced **"); 
	    EMIT;
            break;
        }

        (void)sprintf(out, "Start routine: 0x%p -- %s",
                th_p->th_start_routine, th_p->th_name_p); 
	EMIT;

        (void)sprintf(out, "Arg: 0x%p (%d)",
		th_p->th_arg, th_p->th_arg); 
	EMIT;

	if (cma_p = (cma_opaque_t *)&th_p->th_pthread, cma_p->cma_addr)
	{
	    (void)sprintf(out, "Thread cma thread id:	0x%p %d",
			  cma_p->cma_addr, 
			  cma_p->cma_id);
	}
	else
	{
	    (void)sprintf(out, "Thread cma thread id:	none");
	}
	EMIT;

	if (cma_p = (cma_opaque_t *)&th_p->th_lock, cma_p->cma_addr)
	{
	    (void)sprintf(out, "Thread cma mutex:	0x%p %d",
			  cma_p->cma_addr, 
			  cma_p->cma_id);
	}
	else
	{
	    (void)sprintf(out, "Thread cma mutex:	none");
	}
	EMIT;

	if (cma_p = (cma_opaque_t *)&th_p->th_resume, cma_p->cma_addr)
	{
	    (void)sprintf(out, "Thread cma resume cv:	0x%p %d",
			  cma_p->cma_addr, cma_p->cma_id);
	}
	else
	{
	    (void)sprintf(out, "Thread cma resume cv:	none");
	}
	EMIT;

        switch (th_p->th_state) 
	{
	    
        case state_initialized: 
	    (void)sprintf(out, "State:  Initialized"); 
	    break;

        case state_running:     
	    (void)sprintf(out, "State:  Running");     
	    break;

        case state_runnable:    
	    (void)sprintf(out, "State:  Runnable");    
	    break;

        case state_delay:       
	    (void)sprintf(out, "State:  Delay");       
	    break;

        case state_wait:        
	    (void)sprintf(out, "State:  Wait");        
	    break;

        case state_timed_wait:  
	    (void)sprintf(out, "State:  Timed Wait");  
	    break;

        case state_sys_wait:    
	    (void)sprintf(out, "State:  System Wait"); 
	    break;

        case state_read:        
	    (void)sprintf(out, "State:  Read Wait");   
	    break;

        case state_write:       
	    (void)sprintf(out, "State:  Write Wait");  
	    break;

        case state_read_write:  
	    (void)sprintf(out, "State:  R/W Wait");    
	    break;

        case state_join:        
	    (void)sprintf(out, "State:  Joining");     
	    break;

        case state_exiting:     
	    (void)sprintf(out, "State:  Exiting");     
	    break;

        default: 
	    (void)sprintf(out, "State:  *UNKNOWN* (%d)", 
			  th_p->th_state); 
	    break;

        }
        EMIT;

	switch (th_p->th_cancel) 
	{

	case CANCEL_OFF: 
	    (void)sprintf(out, "General cancelability: CANCEL_OFF"); 
	    break;

	case CANCEL_ON:	
	    (void)sprintf(out, "General cancelability: CANCEL_ON"); 
	    break;

	default:	
	    (void)sprintf(out, "General cancelability: %d", 
			  th_p->th_cancel);
	} 
	EMIT;

	(void)sprintf(out, "Detach state: %d   Cancel state: %d",
		      th_p->th_detached, 
		      th_p->th_cancelled); 
	EMIT;

	(void)sprintf(out, "Exit value: 0x%p     errno: %d",
		      th_p->th_exit, 
		      th_p->th_errno); 
	EMIT;

	if ((ch_p = th_p->th_ch) != NULL)
	{
	    do 
	    {
		(void)sprintf(out, "  cleanup handler 0x%p(0x%p) %d",
			      ch_p->ch_rtn, 
			      ch_p->ch_arg, 
			      ch_p->ch_ex);
		EMIT;
	    } while ((ch_p = ch_p->ch_lnk) != NULL);
	}

        (void)sprintf(out, "Mutex: 0x%p    Condition: 0x%p",
		      th_p->th_mutex, 
		      th_p->th_condition); 
	EMIT;

        (void)sprintf(out, "No thread log information");
        for (x = 0; x <= th_log_limit; x++) 
	{
            if (dthread_equal(th_log[x].thread_id, *thread_p)) 
	    {
                sys_time_to_string(&(th_log[x].time_logged),
				   time_string, 
				   sizeof(time_string));

                (void)sprintf(out, "Thread log: %-0.50s  -- logged at %s",
			      th_log[x].comment, 
			      time_string);
                break;
            }
        }
        EMIT;
        break;

    case 2:
        (void)sprintf(out, 
		      "___ Statistics __________________________________"); 
	EMIT;

        (void)sprintf(out, "Number of threads: Active: %4d  Peak: %4d",
		      num_threads, 
		      peak_threads); 
	EMIT;

        (void)sprintf(out, "Number of mutexes: Active: %4d  Peak: %4d",
		      num_mutexes, 
		      peak_mutexes); 
	EMIT;

        (void)sprintf(out, "Number of cv's:    Active: %4d  Peak: %4d",
		      num_conditions, 
		      peak_conditions); 
	EMIT;

        (void)sprintf(out, 
		      "Mutex usage:       Locks: %d  Collisions: %d (multi: %d)",
		      mutex_locks, 
		      mutex_collisions, 
		      mutex_multi_collisions); 
	EMIT;
        break;

    case 3:

	(void)sprintf(out, "Run queue is not supported for cma"); 
	EMIT;
        break;

    case 4:

        (void)sprintf(out, 
		      "___ Thread log __________________________________"); 
	EMIT;

        for (x = 0; x <= th_log_limit; x++) 
	{
            sys_time_to_string(&(th_log[x].time_logged), 
			       time_string, 
			       50);

            (void)sprintf(out, "%8x  %-0.50s  -- logged at %s",
			  th_log[x].thread_id, 
			  th_log[x].comment, 
			  time_string); 
	    EMIT;
        }
        break;

    case 5:
	m_p = (private_mutex *)(mutex_p ? handle_data(*mutex_p) : 0);
        (void)sprintf(out, 
		      "___ Mutex: 0x%p __________________________________", 
		      m_p);
        EMIT;

        if (m_p == NULL) 
	{
            (void)sprintf(out, "** Invalid mutex referenced **"); 
	    EMIT;
            break;
        }

        if (!m_p->owner) 
	{
            (void)sprintf(out, "No owner"); 	
	    EMIT;
        } 	
	else 
	{
            (void)sprintf(out, "Owned by thread: 0x%p", m_p->owner); 
	    EMIT;
        }

	if (cma_p = (cma_opaque_t *)&m_p->mutex_id, cma_p->cma_addr)
	{
	    (void)sprintf(out, "cma mutex:	0x%p %d",
			  cma_p->cma_addr, 
			  cma_p->cma_id);
	}
	else
	{
	    (void)sprintf(out, "cma mutex:	none");
	}
	EMIT;
        break;

    case 6:
	c_p = (private_condition *)(condition_p ? handle_data(*condition_p) : 0);
        (void)sprintf(out, 
		      "___ Condition variable: 0x%p _____________________", 
		      c_p);
        EMIT;

        if (c_p == NULL) 
	{
            (void)sprintf(out, "** Invalid condition variable referenced **"); EMIT;
            break;
        }

	if (cma_p = (cma_opaque_t *)&c_p->condition_id, cma_p->cma_addr)
	{
	    (void)sprintf(out, "cma condition:	0x%p %d",
			  cma_p->cma_addr, 
			  cma_p->cma_id);
	}
	else
	{
	    (void)sprintf(out, "cma condition:	none");
	}
	EMIT;
        break;

    case 7:

	(void)sprintf(out, "Timer queue is not supported for cma"); 
	EMIT;
        break;

    case 8:

        (void)sprintf(out, 
		      "___ Active Threads Pool _________________________"); 
	EMIT;

        if (EMPTY_ACTIVE_POOL) 
	{
            (void)sprintf(out, "** Active threads pool is empty **"); 
	    EMIT;
        } 
	else 
	{
            int thread_count = 0;
            this_p  = active_pool.pool.next;
            while (this_p != LISTHEAD_ACTIVE_POOL) 
	    {
                thread_count++; this_p = this_p->pool.next;
            }

            (void)sprintf(out, "There are %d active threads:", thread_count); 
	    EMIT;

            this_p  = active_pool.pool.next;
            while (this_p != LISTHEAD_ACTIVE_POOL) 
	    {
                if (detail_level == 0) 
		{
                    (void)sprintf(out, 
				  "  0x%p -- %s", this_p, this_p->th_name_p); 
		    EMIT;
                } 	
		else 
		{
                    (void)dthread_diag_threads(1, 
					       0, 
					       (dthread_t *)&this_p, 
					       0, 
					       0, 
					       emit_routine, 
					       emit_arg);

                    (void)sprintf(out, ""); 
		    EMIT;
                }

                this_p = this_p->pool.next;
            }
        }
        break;

    default:
	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_library,
            svc_c_debug9,
            " <dthread_diag_threads: return(-1)"));

        return (-1);
    }
#endif	/* !DTHREAD_WRAPPERS */

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " <dthread_diag_threads: return(0)"));

    return (0);
}

/*
 * ROUTINE - dthread_diag_log_thread
 *
 * Logs the creation of a thread along with a comment.  The log kept is
 * a ring buffer, so we can track up to the last MAX_THREADS_LOGGED threads
 * that were created (and logged).
 *
 * Logging is typically done after forking a thread.  It is not manditory
 * to log a thread creation, but it could help identify the thread during
 * diagnostics.  Non-instrumented versions of the code can conditionalize
 * the calling of this routine to improve run-time performance.
 *
 * Arguments:
 *    thread_id (in) - ID of the thread to log.
 *    comment   (in) - String to associate with the thread (<50 characters)
 */
void
dthread_diag_log_thread (dthread_t		thread_id,
                         char		       *comment)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >dthread_diag__long_thread"));

    th_log_index++;

    if (th_log_index >= MAX_THREADS_LOGGED) 
    {
        th_log_index = 0;
    }

    if (th_log_limit < th_log_index) 
    {
        th_log_limit = th_log_index;
    }

    th_log[th_log_index].thread_id = thread_id;
    sys_time_from_utc(&(th_log[th_log_index].time_logged), NULL);
    (void)strncpy(th_log[th_log_index].comment, comment,
			MAX_TH_LOG_COMMENT_SIZE);
}

#if defined(DTHREAD_WRAPPERS)
/*
 * dthread_debug - debugging info in dbx
 *	start in mips by:
 *	(dbx) print dthread_debug()
 */

int
dthread_debug (void)
{
    unsigned32 dlevel = svc_c_debug1;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
	" >dthread_debug"));
    
    while (TRUE) 
    {
	static char seps[] = " \t";
	char line[256], *line_p = line;
	char *argv[30];
	int argc = 0;

	/*
	 * prompt and get a command line 
	 */
	dce_printf(
	    cds_s_library_threadsprompt);
	
	if (!gets(line_p)) 
	{
	    break;
	}

	/*
	 * Parse the command into argv[] (strtok() is non-reentrant) 
	 */
	ZERO_bytes(argv, sizeof(argv));

	while (TRUE) 
	{
	    line_p = &line_p[strspn(line_p, seps)];
	    if (!*line_p) 
	    {
		break;
	    }

	    argv[argc++] = line_p;
	    line_p = strpbrk(line_p, seps);
	    if (!line_p) 
	    {
		break;
	    }
	    
	    *line_p++ = '\0';
	}
	argv[argc] = NULL;

	if (argc <= 0) 
	{
	    continue;
	}

	if (strcasecmp(argv[0], "q") == 0)
	{
	    break;
	}

	if (strcasecmp(argv[0], "tcb") == 0) 
	{
	    dthread_t thread = (dthread_t)a__num(argv[1]);

	    (void)dthread_diag_threads(1, 
				       1, 
				       &thread,
				       (dthread_mutex_t *)0, 
				       (dthread_cond_t *)0, 
				       th_bug_output, 	
				       &dlevel); 
	    continue;
	}
	if (strcasecmp(argv[0], "thstats") == 0) 
	{
	    (void)dthread_diag_threads(2, 
				       0, 
				       (dthread_t *)0,
				       (dthread_mutex_t *)0, 
				       (dthread_cond_t *)0, 
				       th_bug_output, 	
				       &dlevel); 
	    continue;
	}

	if (strcasecmp(argv[0], "trq") == 0) 
	{
	    (void)dthread_diag_threads(3, 
				       1, 
				       (dthread_t *)0,
				       (dthread_mutex_t *)0, 
				       (dthread_cond_t *)0, 
				       th_bug_output, 	
				       &dlevel); 
	    continue;
	}

	if (strcasecmp(argv[0], "thlog") == 0) 
	{
	    (void)dthread_diag_threads(4, 
				       0, 
				       (dthread_t *)0,
				       (dthread_mutex_t *)0, 
				       (dthread_cond_t *)0, 
				       th_bug_output, 	
				       &dlevel); 
	    continue;
	}

	if (strcasecmp(argv[0], "mutex") == 0) 
	{
	    dthread_mutex_t mutex = (dthread_mutex_t)a__num(argv[1]);
	    (void)dthread_diag_threads(5, 
				       0, 
				       (dthread_t *)0,
				       &mutex, 
				       (dthread_cond_t *)0, 
				       th_bug_output, 	
				       &dlevel); 
	    continue;
	}

	if (strcasecmp(argv[0], "cv") == 0) 
	{
	    dthread_cond_t condition = (dthread_cond_t)a__num(argv[1]);
	    (void)dthread_diag_threads(6, 
				       0, 
				       (dthread_t *)0,
				       (dthread_mutex_t *)0, 
				       &condition, 
				       th_bug_output, 	
				       &dlevel); 
	    continue;
	}

	if (strcasecmp(argv[0], "ttq") == 0) 
	{
	    (void)dthread_diag_threads(7, 
				       1, 
				       (dthread_t *)0,
				       (dthread_mutex_t *)0, 
				       (dthread_cond_t *)0, 
				       th_bug_output, 	
				       &dlevel); 
	    continue;
	}

	if (strcasecmp(argv[0], "atp") == 0) 
	{
	    (void)dthread_diag_threads(8, 
				       1, 
				       (dthread_t *)0,
				       (dthread_mutex_t *)0, 
				       (dthread_cond_t *)0, 
				       th_bug_output, 	
				       &dlevel); 
	    continue;
	}

	DCE_SVC_DEBUG((
	    cds__svc_handle,
	    cds_svc_library,
	    (svc_c_debug1 |
	     svc_c_route_stderr | 
	     svc_c_route_nolog |	
	     svc_c_action_brief),
	     "commands:"));

	DCE_SVC_DEBUG((
	    cds__svc_handle,
	    cds_svc_library,
	    (svc_c_debug1 |
	     svc_c_route_stderr | 
	     svc_c_route_nolog |	
	     svc_c_action_brief),
	     "  atp    - Active Thread Pool"));

	DCE_SVC_DEBUG((
	    cds__svc_handle,
	    cds_svc_library,
	    (svc_c_debug1 |
	     svc_c_route_stderr | 
	     svc_c_route_nolog |	
	     svc_c_action_brief),
	     "  ttq    - Thread Timer Queue"));

	DCE_SVC_DEBUG((
	    cds__svc_handle,
	    cds_svc_library,
	    (svc_c_debug1 |
	     svc_c_route_stderr | 
	     svc_c_route_nolog |	
	     svc_c_action_brief),
	     "  ttq    - Thread Timer Queue"));

	DCE_SVC_DEBUG((
	    cds__svc_handle,
	    cds_svc_library,
	    (svc_c_debug1 |
	     svc_c_route_stderr | 
	     svc_c_route_nolog |	
	     svc_c_action_brief),
	     "  mutex  - Mutex"));

	DCE_SVC_DEBUG((
	    cds__svc_handle,
	    cds_svc_library,
	    (svc_c_debug1 |
	     svc_c_route_stderr | 
	     svc_c_route_nolog |	
	     svc_c_action_brief),
	     "  thlog  - Threads Log"));

	DCE_SVC_DEBUG((
	    cds__svc_handle,
	    cds_svc_library,
	    (svc_c_debug1 |
	     svc_c_route_stderr | 
	     svc_c_route_nolog |	
	     svc_c_action_brief),
	     " trq     - Thread Run Queue"));

	DCE_SVC_DEBUG((
	    cds__svc_handle,
	    cds_svc_library,
	    (svc_c_debug1 |
	     svc_c_route_stderr | 
	     svc_c_route_nolog |	
	     svc_c_action_brief),
	     " tcb     - Thread Control Block"));

	DCE_SVC_DEBUG((
	    cds__svc_handle,
	    cds_svc_library,
	    (svc_c_debug1 |
	     svc_c_route_stderr | 
	     svc_c_route_nolog |	
	     svc_c_action_brief),
	     " cv      - Condition Variable"));

	DCE_SVC_DEBUG((
	    cds__svc_handle,
	    cds_svc_library,
	    (svc_c_debug1 |
	     svc_c_route_stderr | 
	     svc_c_route_nolog |	
	     svc_c_action_brief),
	     " thstats - Thread Statistics"));
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " <dthread_debug: return(0)"));

    return(0);
}

static unsigned long
a__num (char	*ptr)
{
    unsigned long arg = 0;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
	" >a__num"));

    if (!*ptr)
    {
	goto done;
    }

    if (*ptr != '0') 
    {
	/*
	 * decimal 
	 */
	while (('0' <= *ptr) && (*ptr <= '9'))
	{
	    arg = (arg*10) + (*ptr++ - '0');
	}
	goto check_end;
    }
    ptr++;

    if ((*ptr != 'x') && (*ptr != 'X')) 
    {
	/*
	 * octal 
	 */
	while (('0' <= *ptr) && (*ptr <= '9'))
	{
	    arg = (arg*8) + (*ptr++ - '0');
	}
	goto check_end;
    }

    ptr++;

    /*
     * hexadecimal 
     */
    while (TRUE) 
    {
	char c = *ptr;

	if (('0' <= c) && (c <= '9'))
	{
	    c = c - '0';
	}
	else 
	    if (('a' <= c) && (c <= 'f'))
	    {
		c = c - 'a' + 10;
	    }
	    else 
		if (('A' <= c) && (c <= 'F'))
		{
		    c = c - 'A' + 10;
		}
		else
		{
		    break;
		}

	arg = (arg*16) + c;
	ptr++;
    }

check_end:

    if (*ptr) 
    {
	arg = 0;
    }
    
done:

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
	" <a__num"));

    return(arg);

}

#endif	/* DTHREAD_WRAPPERS */








