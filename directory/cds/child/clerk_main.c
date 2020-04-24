/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: clerk_main.c,v $
 * Revision 1.1.17.2  1996/02/18  19:25:44  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:09:04  marty]
 *
 * Revision 1.1.17.1  1995/12/08  15:04:05  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/jrr_1.2_mothra/1  1995/11/03  18:10 UTC  psn
 * 	Change #include sec_login_util.h to sec_login.h for DCE 1.2.1 bugfix drop to OSF
 * 	[1995/12/08  14:37:58  root]
 * 
 * Revision 1.1.12.7  1994/08/03  18:59:25  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:11:03  mccann]
 * 
 * Revision 1.1.12.6  1994/06/23  18:28:48  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:54:04  mccann]
 * 
 * Revision 1.1.12.5  1994/06/09  16:07:37  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:52:19  devsrc]
 * 
 * Revision 1.1.12.4  1994/06/08  20:17:52  peckham
 * 	I missed dce/assert.h
 * 	[1994/05/31  19:39:18  peckham]
 * 
 * 	Initialize serviceability and message catalogs.
 * 	[1994/05/31  18:56:10  peckham]
 * 
 * Revision 1.1.12.3  1994/04/29  15:49:45  peckham
 * 	Use dcelocal_path in filenames.
 * 	[1994/04/29  13:56:17  peckham]
 * 
 * Revision 1.1.12.2  1994/04/14  14:41:10  peckham
 * 	New module clerk_socket.c to isolate SVR4 named pipes.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	Remove msgblk.h, cds_event_init() and evp references.
 * 	Remove v1v2 code (v1 never used for cds).
 * 	Remove VMS and UNIX conditionals, VMS code.
 * 	Remove last vestige of clerk_trace().
 * 	[1994/04/11  19:36:07  peckham]
 * 
 * Revision 1.1.12.1  1994/03/12  21:56:20  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:50:42  peckham]
 * 
 * Revision 1.1.10.2  1993/07/30  14:56:52  blurie
 * 	Cosmetic fix:  Remove dead code between SNI_TEST defines.
 * 	[1993/07/30  13:39:41  blurie]
 * 
 * Revision 1.1.7.5  1992/12/30  12:40:49  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:26:31  zeliff]
 * 
 * Revision 1.1.7.4  1992/12/18  00:33:00  jd
 * 	Alan Peckham, 17-Dec-1992
 * 	Have a cookie for being so good.
 * 	With the new casys.c, use CASysFindMem() to get the shmid
 * 	if -m is not supplied. Makes stand-alone clerk startup easier.
 * 	[1992/12/18  00:27:47  jd]
 * 
 * Revision 1.1.7.3  1992/12/15  15:54:38  tom
 * 	Bug 6409 - add setlocale call for i18n.
 * 	[1992/12/15  15:16:57  tom]
 * 
 * Revision 1.1.7.2  1992/09/29  19:09:07  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:10:23  weisman]
 * 
 * Revision 1.1.2.3  1992/05/12  12:59:54  keegan
 * 	The call to 'get_init_clearinghouse'  is #ifdef out. It is no longer
 * 	needed and results in the use of hard-coded protocol sequences in tower
 * 	construction.
 * 	[1992/05/11  20:47:47  keegan]
 * 
 * Revision 1.1.2.2  1992/03/22  21:21:27  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:07:50  weisman]
 * 
 * Revision 1.1  1992/01/19  15:27:27  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: clerk_main.c
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
 */ 

#include <dce/dce.h>		/* needed by dce/dce_msg.h */
#include <dce/assert.h>
#include <dce/dcesvcmsg.h>
#include <dce/dce_msg.h>	/* must be before dcecdsmsg.h */
#include <dce/dcecdsmsg.h>
#include <dce/rpc.h>
#include <dce/dce_error.h>

#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <pthread.h>

#include <locale.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

#include <uc_clerk.h>
#include <uc_child.h>
#include <dns_cfg.h>
#include <cache.h>
#include <dns_strings.h>
#include <tower.h>

#if defined(DCE_SEC)
# include <dce/sec_login.h>
#endif
#include <dce/cds_clerkserver.h>

extern char *optarg;
extern int optind;

/*
 * Local Function Prototype Declarations.
 */

int
main (register int,
      register char **);

static int
clerk_state (
      clerk_t *);

static void
getargs (
      int	,
      char	 **,
      clerk_t 	*);

static void
cleanup (
      clerk_t 	*);

static void *
signal_catcher (
      clerk_t	*);

/*
 * data 
 */

dthread_mutex_t dns_clerk_mutex;
int             clerk_def_stk = 0;		/* defunct */

/*
 * Stack sizes for clerk threads
 *
 * clerk_client.c
 */
dthread_attr_t clerk_client_attr = {"clerk_client", 0};
#if defined(_DECTHREADS_) && (_CMA_OSIMPL_ == _CMA__OS_OSF) && defined(__alpha)
dthread_attr_t clerk_request_attr = {"clerk_request", 100000};
#else
dthread_attr_t clerk_request_attr = {"clerk_request", 60000};
#endif


/*
 * clerk_listener.c
 */
dthread_attr_t clerk_listener_attr = {"clerk_listener", 0};

/*
 * Configuration parameter table
 */
static dthread_attr_t *attrp[] = {
    &clerk_client_attr,
    &clerk_request_attr,
    &clerk_listener_attr,
    0
};


#ifdef DCE_CDS_DEBUG

/*
 * for use in dbx ONLY !!!
 */
pid_t mypid;

#endif /* DCE_CDS_DEBUG */

static int loiter_interval = 1*60;	/* poll every minute */

static int loiter_polls = 20;		/* altogether 20 minutes */

static dthread_attr_t signal_catcher_attr = {"signal_catcher", 0};

static dthread_t signal_thread;

/* 
 * debugging aids..
 */
static int debugger_start = 0; 	/* set to 1 (TRUE) if -D set at cmd line */
static int no_adv = 0;  	/* set to 1 (TRUE) if -n set at cmd line */

static int Closem = 0;	/* set with -F to force recreation of shm */

static int CacheId = -1; /* Shared memory id set with -m */

static int localCacheId = -1;



static int catch[] = { SIGHUP, SIGINT, SIGTERM, 0 }, 
           dump[] = { SIGQUIT, SIGILL, SIGTRAP, SIGIOT, SIGEMT, SIGFPE,
		     SIGBUS, SIGSEGV, SIGSYS, 0 },
           ignore[] = {
#if defined(SIGXCPU)
                       SIGXCPU, SIGXFSZ,
#endif

#if defined(SIGLOST)
                       SIGLOST,
#endif
                       SIGPIPE, SIGALRM, SIGPROF, SIGUSR1, SIGUSR2, 0 };


static char *myname_p;


/*
 *		m a i n
 *
 * Does all of that described above.
 *
 * Returns:
 *	 errors from lower level routines
 */

int
main (register int   argc,
      register char *argv[])
{
    clerk_t		thisClerk;
#if defined(DCE_SEC)
    sec_login_handle_t login_context;
#endif
    char		svcname[128];
    dthread_attr_t	**attr_pp = attrp, *attr_p;
    char		*myname_p, *handle;
    error_status_t	st;
    int			thstatus;

    /*
     * Get our filename, stripped of directories
     */
    if (myname_p = strrchr(argv[0], '/'))
	myname_p++;
    else
	myname_p = argv[0];

    setlocale(LC_ALL, "");

    (void)sprintf(svcname, "%s(%ld)", myname_p, (long)getpid());
    dce_svc_set_progname(svcname, &st);
    dce_assert(cds__svc_handle, (st == svc_s_ok));

    dce_msg_define_msg_table(cds_msg_table,
                             sizeof(cds_msg_table) / sizeof(cds_msg_table[0]),
                             &st);
    dce_assert(cds__svc_handle, (st == msg_s_ok));


    /*
     * Clerk startup message
     */
    dce_svc_printf(CDS_S_CHILD_STARTING_MSG);

#ifdef DCE_CDS_DEBUG
    mypid = getpid();
#endif

    cds_ifspec = cds_clerkserver_v1_0_c_ifspec;
    dns_cfginit(NULL, argv[0]);
    dthread_init();

#if defined(DCE_SEC)

    /* If security enabled, release any login context we may have
     * inherited.  The clerk imports the login context from the client,
     * and that context is used to: a) control access to the cache
     * and b) provide access control to the namespace.
     */
    sec_login_get_current_context (&login_context, &st);
    if (st == error_status_ok) 
    {
	sec_login_release_context (&login_context, &st);
    }

#endif

    {
        if ((thstatus = dthread_create_mutex(&dns_clerk_mutex)) < 0)
        {
	  dce_svc_printf(CDS_S_PTHREADMUTEXINIT_A_MSG, (long)thstatus);
        }
    }
    ZERO_bytes(&thisClerk, sizeof(thisClerk));
    {
        if ((thstatus = dthread_create_mutex(&thisClerk.mutex)) < 0)
        {
	  dce_svc_printf(CDS_S_PTHREADMUTEXINIT_A_MSG, (long)thstatus);
        }
    }
    thisClerk.listenerFd = -1;
    {
        if ((thstatus = dthread_create_condition(&thisClerk.state_changed)) < 0)
        {
	  dce_svc_printf(CDS_S_PTHREADCONDINIT_A_MSG, (long)thstatus);
        }
    }
    thisClerk.link_p = NULL;

    /* 
     * Get defaults from configuration file 
     */
    handle = dns_cfgopen(NULL, NULL);
    while (attr_p = *attr_pp++) 
    {
	char parm[256];

	strcpy(parm, attr_p->name);
	strcat(parm, ".stack");
	attr_p->stacksize = dns_cfgtol(handle, parm, attr_p->stacksize);
    }

    loiter_interval = dns_cfgtol(handle, "loiter_interval", loiter_interval);
    loiter_polls = dns_cfgtol(handle, "loiter_polls", loiter_polls);
    dns_cfgclose(handle);

    /*
     * Look at command line.  Map shared memory.
     */
    getargs(argc, argv, &thisClerk);

    if (CacheId != -1) 
    {
	if (CASysMap(CacheId) == DNS_NOTFND) 
        {
	    /*
	     * Tell user Shared memory id was invalid
	     */ 
            dce_svc_printf(CDS_S_CHILD_BADSHMEM_EB_MSG, CacheId, (long)errno);
	}
        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_child,
            svc_c_debug3,
            "  main: using cache(%d)",
            CacheId));
    } 
    else 
         if (debugger_start || no_adv) 
	 {
	     if ((localCacheId = CALoad(-1)) < 0) 
	     {
		 /*
		  *  Tell User unable to load cache
		  */
		 dce_svc_printf(CDS_S_CHILD_CACHELOAD_EB_MSG);
	      }

              DCE_SVC_DEBUG((
                  cds__svc_handle,
                  cds_svc_child,
                  svc_c_debug3,
                  "  main: created local cache(%d)",
                  localCacheId));    
         }
         else 
         {
	     /*
	      * Tell user that a shared memory id is required
	      */ 
             dce_svc_printf(CDS_S_CHILD_NOSHMEM_EB_MSG);
         }
 
    if (Closem)
    {
	unlink(thisClerk.socket_name);
    }

    if (no_adv) 
    {
        dns_daemon(myname_p);
    }

    dthread_create(&signal_thread, &signal_catcher_attr,
	(dthread_start_routine)signal_catcher, (dthread_address_t)&thisClerk);

    /* 
     * This call sets up the lock code so that only threads can
     * now aquire locks
     */
    CASetUp();

    /*
     * Monitor clerk state
     */
    clerk_state(&thisClerk);
    cleanup(&thisClerk);
    dthread_term();

    /*
     * Clerk normal succesfull exit
     */
    dce_svc_printf(CDS_S_CHILD_EXITING_EG_MSG);
}


static void 
getargs (int      argc,
         char     *argv[],
         clerk_t  *clerk_p)
{
    char *handle  = dns_cfgopen(NULL, NULL);
    int c;

    /*
     * Message routing variables
     */
    error_status_t routing_status;
    unsigned char  inq_error_string[dce_c_error_string_len];
    int 	   inq_sts;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >getargs"));

    /* 
     * Get our filename, stripped of directories 
     */
    if (myname_p = strrchr(argv[0], '/'))
    {
	++myname_p; 
    }
    else
    {
	myname_p = argv[0];
    }

    clerk_p->socket_name[0] = 0;

#if defined(DCE_CDS_DEBUG)
    while ((c = getopt(argc, argv, "C:FIm:ns:w:d:D")) != EOF)
#else
    while ((c = getopt(argc, argv, "Fm:w:d:")) != EOF)
#endif
    {
	switch (c) 
        {
            case 'C': /*
		       * Configuration.  add this parameter to the file 
		       */
                      dns_cfgfromarg(handle, optarg);
                      break;

            case 'F': /*
		       * Delete old socket on startup 
		       */
                      Closem = 1;
                      break;

            case 'I':
                      clerk_p->IPprefer++;
                      break;

            case 'm': /*
		       * Map to existing shared memory 
		       */
                      CacheId = atoi(optarg);
                      break;

            case 'n':
                      no_adv++;
                      break;

            case 's': /*
		       * Socket name for communication 
		       */
                      strcpy(clerk_p->socket_name, optarg);
                      break;

            case 'w': /*
		       * Startup Routing Specifications 
		       */
		      dce_svc_routing((unsigned char *)optarg,
				      &routing_status);

		      if (routing_status != svc_s_ok)
		      {
			dce_svc_printf(CDS_S_SVC_ROUTING_EB_MSG, 
				       (long)routing_status);
		      }
                      break;

            case 'd': /*
		       * Startup DEBUG-Routing Specifications 
		       */
		      dce_svc_debug_routing((unsigned char *)optarg,
					    &routing_status);

		      if (routing_status != svc_s_ok)
		      {
			dce_svc_printf(CDS_S_DEBUG_ROUTING_EB_MSG, 
				       (long)routing_status);
		      } 
                      break;
		  
	    case 'D':
		      /*
		       * Debugger start to stop fork 
		       */
		      debugger_start++;
		      break;

            default:

		      /*
		       * Give "usage" message.
		       */
                      dce_svc_printf(CDS_S_CHILD_USAGE_EB_MSG, myname_p);

	} /* end switch */
    } /* end while */

    /* 
     * if a configuration parameter was set, re-write and close the file 
     */
    dns_cfgwrite(handle);
    dns_cfgclose(handle);

    if ((no_adv || debugger_start) && !clerk_p->socket_name[0])
    {
        (void)sprintf(clerk_p->socket_name, CDS_DEF_CLERK_SOCK, dcelocal_path);
    }
    
    /*
     * Save core dumps (changes working directory) 
     */
    dns_coredefault(myname_p, 0);

    if (CacheId < 0)
    {
	CacheId = CASysFindMem();
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <getargs"));
}



/*
 * signal waker - hack for broken CMA
 *
 * A bug in CMA keeps asynchronous signals from being delivered
 * until a thread runs. this silent thread reschedules every second,
 * thus making sure that the desired signals get delivered.
 */
static pthread_addr_t 
signal_waker (pthread_addr_t arg)
{
    static struct timespec interval = {1,0};


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug7,
        " >signal_waker"));

    while (1==1)
    {
	pthread_delay_np(&interval);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug7,
        " <signal_waker"));

}


/*
 * signal_catcher - seperate thread waits for asynchronous signals
 */
static void 
*signal_catcher (clerk_t *clerk_p)
{
    pthread_t	waker_thread;
    sigset_t	signals;
    int		sig, *sig_p;
    struct sigaction sigactn;
    

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug7,
        " >signal_catcher"));
    /*
     * Start up the hack 
     */
    pthread_create(&waker_thread, pthread_attr_default,
		   signal_waker, (pthread_addr_t)0);

    sigemptyset(&signals);

    /*
     * These are the asynchronous signals we want to catch 
     */
    sig_p = catch;
    while (sig = *sig_p++)
    {
	sigaddset(&signals, sig);
    }

    /* 
     * Don't let CMA unwind the stack - override with the default dump 
     */
    sig_p = dump;
    while (sig = *sig_p++)
    {
	/*
	 *  Set up sigaction structure
	 */
	sigactn.sa_handler = SIG_DFL;
	sigemptyset(&sigactn.sa_mask);
	sigactn.sa_flags   = 0;

	sigaction(sig, &sigactn, NULL);
    }

    /* 
     * Ignore extraneous signals 	
     */
    sig_p = ignore;
    while (sig = *sig_p++)
    {
	/*
	 *  Set up sigaction structure
	 */
	sigactn.sa_handler = SIG_IGN;
	sigemptyset(&sigactn.sa_mask);
	sigactn.sa_flags   = 0;

	sigaction(sig, &sigactn, NULL);
    }

    sigprocmask(SIG_BLOCK, &signals, NULL);	/* block these while waiting */
    sig = sigwait(&signals);
    sigprocmask(SIG_UNBLOCK, &signals, NULL);

    cleanup(clerk_p);

    if (sig == -1)
    {
	/* 
	 *  Clerk exit due to signal flagging system error
	 */
	dce_svc_printf(CDS_S_CHILD_SIGWAIT_EB_MSG, (long)errno);
    }
    else
    {
	/*
	 * Clerk will now exit due to signal status.
	 */
	dce_svc_printf(CDS_S_CHILD_SIGNAL_EG_MSG, sig);
    }

}


static void 
cleanup (clerk_t *clerk_p)
{

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >cleanup"));

    /*
     * Delete unix domain socket
     */
    unlink(clerk_p->socket_name);

    /*
     * Unload locally-created cache
     */
    if (0 <= localCacheId)
    {
	CAUnLoad(localCacheId);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <cleanup"));
}


/*
 * This thread runs on a timer to check the clerk state and force
 * rundown when the state changes.  If there has been no
 * activity for 20 minutes forces a rundown.
 */
static int
clerk_state (clerk_t *clerk_p)
{
    dthread_date_time_t timeout;
    time_local_t interval;
    int count = 0;
    int thstatus;
  

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >clerk_state"));

    clerk_p->clerk_state = clerk_Init;
  
    /*
     * Create thread to monitor connection
     */
    if (!start_listener(clerk_p))
    {
	DCE_SVC_DEBUG((
           cds__svc_handle,
           cds_svc_child,
           svc_c_debug9,
           " <clerk_state: didn't start listener - return(0)"));

        return(0);
    }
  
    /*
     * Wake up every 1 minute.  If state has changed, force rundown.
     */
    sys_time_from_interval(&interval, 0, 0, loiter_interval);
    dthread_get_expiration(&interval, &timeout);
  
    if ((thstatus = dthread_lock(&(clerk_p->mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

     /* Beginning of the 'protected code' */

       while (clerk_p->clerk_state == clerk_Init)
       {
                if ((thstatus = dthread_wait(&clerk_p->state_changed,
                                             &clerk_p->mutex)) < 0)
                {
		  dce_svc_printf(CDS_S_PTHREADCONDWAIT_A_MSG, (long)thstatus);
                }
        }
  
        while (1) 
        {
	    if (clerk_p->clerk_state != clerk_On) 
	    {
		DCE_SVC_DEBUG((
                    cds__svc_handle,
                    cds_svc_child,
                    svc_c_debug9,
                    "  clerk_state state=%d",
                    clerk_p->clerk_state));
		
		break;
	    }
  
	    if ((CAfixed_p->ca_state != On) && 
		(CAfixed_p->ca_state != Initial)) 
	    {
		DCE_SVC_DEBUG((
                    cds__svc_handle,
                    cds_svc_child,
                    svc_c_debug9,
                    "  clerk_state ca_state=%d",
                    CAfixed_p->ca_state));
  
                break;
            }
  
            if (0 <= dthread_timed_wait(
  		      &clerk_p->state_changed, &clerk_p->mutex, &timeout)) 
  	    {
		continue;
  	    }
  
            if (dthread_get_errno() != EAGAIN) 
	    {
		break;
	    }

	    dthread_get_expiration(&interval, &timeout);
	    
	    /*
	     * Bump counter.  Every 20th interation, check that some activity 
	     * has gone on.  If not force rundown
	     */
	    if (loiter_polls <= ++count) 
	    {
		/* If reads and writers are sync. and the equal zero, no 
		 * activity in last 20 minutes
		 */

		DCE_SVC_DEBUG((
                    cds__svc_handle,
		    cds_svc_child,
		    svc_c_debug9,
                    "  clerk_state Reads=%d, Writes=%d",
                    clerk_p->Reads,
                    clerk_p->Writes));

	        if ((clerk_p->Reads == clerk_p->Writes) && !clerk_p->Reads)
	        {
		    break;
		}
		else 
		{
		    /*
		     * Reinitialize counters 
		     */
		    count = 0;

		    /* 
		     * Must be threads that haven't responded 
		     * yet => readers > writers
		     */
		    clerk_p->Reads = clerk_p->Reads - clerk_p->Writes;
		    clerk_p->Writes = 0;
		}
	    }

	} /* End while (1)... */


    /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(clerk_p->mutex))) < 0)
    {
	dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        "  clerk_state: killing listener"));

    kill_listener(clerk_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <clerk_state: @end : return(0)"));

    return(0);
}


#define	INFO_P	clerkArgs[1]  /* First parameter is local subcode */


/*
 *	c l e r k G e t I n f o
 *
 * This routine returns the cacheid.
 *
 * Outputs:
 *	INFO_P
 *
 * Returns:
 */
void
clerkGetInfo (thread_t *th_p)
{

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug5,
        " >clerkGetInfo"));
  
    prm_begin(th_p);
    if (CacheId != -1)
    {
	prm_copy(th_p, th_p->INFO_P, (byte_t *)&CacheId, sizeof(longword));
    }
    else
    {
	prm_copy(th_p, th_p->INFO_P, (byte_t *)&localCacheId, 
		 sizeof(longword));
    }
    
    prm_end(th_p);
  
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug5,
        " <clerkGetInfo"));
}
