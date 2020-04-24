/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: server_main.c,v $
 * Revision 1.1.15.2  1996/02/18  19:36:40  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:17:19  marty]
 *
 * Revision 1.1.15.1  1995/12/08  15:32:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:51  root]
 * 
 * Revision 1.1.12.9  1994/09/15  15:21:45  mccann
 * 	add -v option OT12161
 * 	[1994/09/15  15:20:54  mccann]
 * 
 * Revision 1.1.12.8  1994/08/24  20:14:53  mccann
 * 	fix server shutdown, add annotation strings, start message delay:OT11239,11245 and 11766
 * 	[1994/08/24  20:09:20  mccann]
 * 
 * Revision 1.1.12.7  1994/08/03  19:04:37  mccann
 * 	fix assert
 * 	[1994/07/21  19:26:29  mccann]
 * 
 * 	final sams cleanup drop
 * 	[1994/07/19  18:28:04  mccann]
 * 
 * Revision 1.1.12.6  1994/06/30  19:18:18  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:49:04  mccann]
 * 
 * Revision 1.1.12.5  1994/06/23  18:30:53  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:51:29  mccann]
 * 
 * Revision 1.1.12.4  1994/06/09  18:43:50  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:16:23  devsrc]
 * 
 * Revision 1.1.12.3  1994/06/08  20:18:04  peckham
 * 	Use cds_server_set_progname().
 * 	[1994/06/06  20:26:24  peckham]
 * 
 * 	Initialize serviceability and message catalogs.
 * 	[1994/05/31  18:57:12  peckham]
 * 
 * Revision 1.1.12.2  1994/04/14  14:51:32  peckham
 * 	Remove CMA, VMS and UNIX conditionals.
 * 	Remove dnstables dependencies.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	[1994/04/11  20:22:09  peckham]
 * 
 * Revision 1.1.12.1  1994/03/12  22:12:26  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:45:22  peckham]
 * 
 * Revision 1.1.10.2  1993/06/24  19:47:57  hinman
 * 	[hinman] - Save merged-in SNI version (these files have been freed of nasty code)
 * 	[1993/06/17  14:22:23  hinman]
 * 
 * Revision 1.1.7.5  1993/02/01  20:26:55  hinman
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  15:58:41  hinman]
 * 
 * Revision 1.1.7.4  1992/12/30  14:23:21  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:23:54  zeliff]
 * 	Revision 1.1.8.2  1993/01/11  16:09:50  hinman
 * 	[hinman] - Check in merged SNI version
 * 	Revision 1.1.7.3  1992/12/15  15:55:18  tom
 * 	Bug 6409 - add setlocale call for i18n.
 * 	[1992/12/15  15:17:51  tom]
 * 
 * Revision 1.1.7.2  1992/09/29  19:16:18  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:17:25  weisman]
 * 
 * Revision 1.1.2.6  1992/06/08  15:17:18  mfox
 * 	Diagnostic interface to server didn't work unless cdsd was started
 * 	with -d command line option.
 * 	[1992/06/08  15:11:56  mfox]
 * 
 * Revision 1.1.2.5  1992/06/05  20:37:29  keegan
 * 	Exit server with message if cannot access catalog files.
 * 	[1992/06/05  14:26:08  keegan]
 * 
 * Revision 1.1.2.4  1992/06/01  14:17:58  grober
 * 	Placed calls to mallopt inside of #ifndef __hpux .
 * 	[1992/05/27  15:27:40  grober]
 * 
 * Revision 1.1.2.3  1992/03/22  22:33:31  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:36:02  weisman]
 * 
 * Revision 1.1.2.2  1992/01/22  23:02:21  melman
 * 	Adding changes from defect_fixes archive.
 * 	[1992/01/22  22:17:57  melman]
 * 
 * $EndLog$
 */
/*
 * MODULE: server_main.c
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
 *
 */

#include <server.h>
#include <dce/dcesvcmsg.h>
#include <dce/dce_priv_proc.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>

#if !defined(SOCK_STREAM)	/* socket.h not idempotent in BSD ULTRIX */
# include <sys/socket.h>
#endif

#include <signal.h>
#include <tower.h>
#include <locale.h>
#include <dce/cds_clerkserver.h>

#if defined(DEC_LMF)
# include <lmf.h>
#endif

#include <rpcserver.h>
#include <dns_cfg.h>

#if defined(DCE_SEC)
# include <dce/binding.h>
#endif

#include <time.h>
#include <clerk.h>
#include <dns_strings.h>
#include <errno.h>
#include <dce/server_ncl_defs.h>

typedef struct {
    nsgbl_ns_t		*sd_ns_p;
    time_t		sd_begin;
    time_t		sd_end;
    int			sd_grain_size;
    char		sd_debug;
    char		sd_auto;
} data_t;

struct nsgbl_ns dns_nameserver;
/*
 * signal catching
 */

static int catch[] = {
    SIGHUP, SIGINT, SIGTERM, SIGUSR1, 0
}, dump[] = {
    SIGQUIT, SIGILL, SIGTRAP, SIGIOT, SIGEMT, SIGFPE,
    SIGBUS, SIGSEGV, SIGSYS, 0
}, ignore[] = {
# if defined(SIGXCPU)
    SIGXCPU, SIGXFSZ,
# endif
    SIGCHLD,
# if defined(SIGCLD)
#  if SIGCHLD != SIGCLD
    SIGCLD,
#  endif
# endif
    SIGIO, SIGPIPE, SIGALRM, SIGPROF, SIGUSR2, 0
};

/*
 * local prototypes
 */
int 
main (
    int   	  ,
    char 	**);

static void
signal_catcher (
    void);

static dns_status_t
server_auto (
	     data_t *);

static void
setup (
    data_t		*,
    int			,
    char		**);

int debug_mode = 0; /* =1 if -D switch set */

char * myname_p;  /* server's program name */
dthread_mutex_t dns_server_mutex = {0};


/* -------------------------------------------------------------------------- */

/*
 * ROUTINE - main 
 *
 * Main routine for server.  In charge of initialization of the various
 * server subsystems and libraries and their eventual rundown:
 *    - Configuration
 *    - Threads
 *    - Global Structures
 *    - Diagnostics
 *    - Instrumentation
 *    - Clerk Interface
 *    - Management
 */
int 
main (int   argc,
      char  **argv)
{
    extern long data_limit;
    data_t sd;
    dns_status_t status;
    error_status_t st;
    int thstatus;
    void		*diag_handle;

    /*
     * Get our filename, stripped of directories
     */
    if (myname_p = strrchr(argv[0], '/'))
	myname_p++;
    else
	myname_p = argv[0];

    setlocale(LC_ALL, "");

    cds_server_set_progname(myname_p);

    dce_msg_define_msg_table(cds_msg_table,
			     sizeof(cds_msg_table) / sizeof(cds_msg_table[0]),
			     &st);
    dce_assert(cds__svc_handle, (st == msg_s_ok));

    cds_ifspec = cds_clerkserver_v1_0_s_ifspec;

    ZERO_bytes(&sd, sizeof(sd));
    sd.sd_ns_p = &dns_nameserver;
    sd.sd_grain_size = 64;		/* Mem struct plus common size */

    /*
     * Basic environment setup comes first.
     */

#if defined(M_MXFAST)
    mallopt(M_MXFAST, sd.sd_grain_size);	/* Get chunks for this size */
    mallopt(M_GRAIN, sd.sd_grain_size);		/* Round up for malloc  */
#endif

    /*
     * Initialize the configuration file facility.
     *
     */

    dns_cfginit(NULL, argv[0]);

    dthread_init();		       /* Threads environment */

    /* Initialize up front, setup may set locksmith field */
    mgmt_init(&dns_nameserver);		/* Server management */

    (void)setup(&sd, argc, argv);      /* Log file and instrumentation setup */

    /* Save startup message till after routing set up. */
    dce_svc_printf(CDS_S_SERVER_STARTUP_MSG);

    /*
     * must be a privileged user to start the server daemon
     */
    if (!dce_priv_proc_is_appropriate())
    {
	dce_svc_printf(CDS_S_NOPRIV_EB_MSG);
    }

    debug_mode = sd.sd_debug;

    init_global_structures();	       /* Globally-accessible structures */

    /*
     * Increase the data limit size to the configured value.
     */
    if (data_limit && !sys_set_data_limit(data_limit)) 
    {
	dce_svc_printf(CDS_S_SERVER_SETRLIMIT_MSG, strerror(errno));
    }


    /*
     * Common server initialization (enable the diagnostic processor and
     * the management transaction processor), and then start up the
     * threads package.
     */

    if ((thstatus = dthread_create_mutex(&dns_server_mutex)) < 0)
    {
	dce_svc_printf(CDS_S_PTHREADMUTEXINIT_A_MSG, (long)thstatus);
    }


    (void)clerk_init();			/* Enable server's clerk library */
    initloop();				/* Initialize group loop detection */

    /*
     * Automatically create, enable and init the server
     */
    if ((status = server_auto(&sd)) != DNS_SUCCESS)
    {
	dce_svc_printf(CDS_S_SERVER_EB_MSG);
    }

   dce_svc_printf(CDS_S_SERVER_INITCOMPLETE_MSG);
    
    /*
     * Detach from user terminal (if -d not set)
     */
    if (!debug_mode) {
      if ((st = cds_server_detach (myname_p, NULL, NULL)) != 0) {
	dce_svc_printf(CDS_S_SERVER_EB_MSG);
	exit(st);
      }
    }

#if defined(DNS_DIAG)
    /*
     * Enable diagnostics - wait until after fork if using pthreads 
     */
    if (!(diag_handle = server_diag_start(sd.sd_ns_p))) {
      /* Keep going */
      dce_svc_printf(CDS_S_SERVER_DIAG_MSG);
    }

#endif

    /* Wait for kill signal, then start cleanup */
    signal_catcher();

    server_shutdown(sd.sd_ns_p);

#if defined(DNS_DIAG)
    server_diag_stop(diag_handle);
#endif

    dthread_term();			/* We're done - shut down threads */

    /*
     * Normal exit 
     */
    dce_svc_printf(CDS_S_SERVER_EG_MSG);
    
    /*
     * NOTREACHED - the dce_svc_printf does the exit.. this is just
     * 		    here to stop the gcc(1) warnings for control 
     *		    reaching the end of a non-void function.
     */

    exit(0);
}
void
server_shutdown(
		nsgbl_ns_t *ns_p)
{
  nsgbl_ch_t *ch_p;
  dns_status_t status;
  int thstatus;

  /*
   * lock the memory section
   */
  SERVER_LOCK(ns_p->ns_mutex) 
  {
      
    /*
     * just return
     */
    if (ns_p->ns_state == dns_Off) {
      SERVER_END_LOCK(ns_p->ns_mutex);
      return;
    }
    
    /*
     * Disable all known clearinghouses
     */ 
    ch_p = NULL;
    while (TRUE) {
      ch_p = nsgbl_ch_next (ns_p, ch_p);
      if (!ch_p) break;
      if (ch_p->ch_state != dns_Off)
	  status = db_disable(&ch_p->ch_uid);
      
      if ((thstatus = dthread_lock(&(ch_p->ch_mutex))) < 0)
	  dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
      
      ch_p->ch_state = dns_Off;
      
      if ((thstatus = dthread_broadcast(&(ch_p->ch_state_changed))) < 0)
	  dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, (long)thstatus);
      
      if ((thstatus = dthread_unlock(&(ch_p->ch_mutex))) < 0) 
	  dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
      
    } /* end while */

    /*
     * Close down files
     */
    db_exit (ns_p);
    
    /*
     * Remove all clearinghouses from the list of known
     * clearinghouses
     */
    while (TRUE)
	{
	  ch_p = nsgbl_ch_next (ns_p, NULL);
	  if (!ch_p) break;
	  nsgbl_ch_free (ch_p);
	}
    
    ns_p->ns_state = dns_Off;

    /* Possibly wake up disable_server cmd thread */

    if ((thstatus = dthread_broadcast(&(ns_p->ns_state_changed))) < 0)
	dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, (long)thstatus);
      
      
  } 
  SERVER_END_LOCK(ns_p->ns_mutex)
      
  /*
  * Notify advertiser
  */
  adver_signal_new_info (ns_p->ns_advertiser);

  /*
   * Do this after broadcast so if shutdown is caused by
   * mgmt thread it will wake up and get out of here.
   * listener only exits when rpc threads are gone.
   * Stop receiving connection requests
   * Do this after the disable of id is zeroed.
   */
    if (ns_p->ns_ta_id) {
      (void)net_close (ns_p->ns_ta_id);
    }
  
  /*
   * Cause the advertiser to quit
   */
  adver_kill(ns_p->ns_advertiser);
}

/*
 * ROUTINE - setup
 *
 * This routine performs a number of miscellaneous server setup activities:
 *
 *    - Parses command line arguments
 *    - Establishes default and event log file names
 *    - Reassigns stdout/stderr if we're running detached
 *    - Changes the default directory so core files will end up in
 *      a known spot.
 *    - Reads in instrumentation events
 *    - Sets up debug/msg routing
 */

static void
setup (sd_p, argc, argv)
    data_t		*sd_p;
    int			argc;
    char		**argv;
{
    int			status;
    struct sigaction sigactn;
    long major,minor;
    char *endptr,*begin_ptr;

    /*
     * Variables for msg/debug routing set-up
     */
    dce_error_string_t  inq_error_string;
    int			inq_sts;
    error_status_t 	rout_err_st;
    
    {  /* non vms code */
    extern char		*optarg;
    register char	*handle = dns_cfgopen(NULL, NULL);


    while ((status = getopt(argc, argv, "aC:d:Dl:w:v:")) != EOF) 
    {
	switch (status) 
	{
	case 'a':
	    sd_p->sd_auto++;
	    break;

	case 'v':
	    if (optarg) {
	      begin_ptr = optarg;
	      major = strtol(begin_ptr, &endptr, 10);
	      if ((major == 3) || (major == 4)) {
		if (*endptr == '\0')
		  minor = 0;
	        else  {
		  begin_ptr = endptr;
		  if (*begin_ptr == '.')
		      ++begin_ptr;
		  minor = strtol(begin_ptr, &endptr, 10);
		}
		if (minor == 0) {
		  set_directory_version((byte_t)major, (byte_t)minor);
		  break;
		}
	      }
	    }

	    dce_svc_printf(CDS_S_SERVER_VERSION_EB_MSG, optarg);
	    break;

	case 'C':
	    /*
	     * add this parameter to the file 
	     */
	    (void)dns_cfgfromarg(handle, optarg);
	    break;

	case 'D':
	    /*
	     * for dbx debugger etc. to stop forking.
	     */
	    sd_p->sd_debug++;
	    break;

	case 'd':
	    /*
	     * Startup DEBUG Routing Specifications
	     */
	    dce_svc_debug_routing((unsigned char *)optarg,&rout_err_st);
	    if (rout_err_st != rpc_s_ok)
	    {
	      dce_svc_printf(CDS_S_DEBUG_ROUTING_EB_MSG, (long)rout_err_st);
	    }
	    break;

	case 'l':
	    strcpy((char *)sd_p->sd_ns_p->ns_Locksmith, optarg);
	    break;

	case 'w':
	    /*
	     * Startup Routing Specifications
	     */
	    dce_svc_routing((unsigned char *)optarg,&rout_err_st);
	    if (rout_err_st != rpc_s_ok)
	    {
	      dce_svc_printf(CDS_S_SVC_ROUTING_EB_MSG, (long)rout_err_st);
	    }
	    break;

	default:
	    dce_svc_printf(CDS_S_SERVER_USAGE_EB_MSG, myname_p);
	}
    } /* end while... one of them !! -> the appropriate  #if.. one !  */

    /* if configuration parameter was set, the file will be rewritten */
    (void)dns_cfgwrite(handle);
    dns_cfgclose(handle);

    if (!sd_p->sd_debug) {
        /*
	 * Save core dumps (changes working directory) 
	 */
	dns_coredefault(myname_p, 1);
    }

    /*
     * Make sure broken pipes don't stop us 
     */
    sigactn.sa_handler = SIG_IGN;
    sigemptyset(&sigactn.sa_mask);
    sigactn.sa_flags   = 0;
    sigaction(SIGPIPE, &sigactn, NULL);

    } /* end non vms code block */

    sd_p->sd_begin = time((time_t *)0);

} 
/*
 * ROUTINE - server_auto
 *
 * Automatically bring up the server database.
 * If no clearinghouse has been established,
 * create a new namespace based on the hostname.
 *
 */
static dns_status_t
server_auto (data_t *sd_p)
{
    DEB_ASCII_BUF_nsgbl_ns(nsBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    register nsgbl_ns_t	*const ns_p = sd_p->sd_ns_p;
    register dns_status_t status;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " >server_auto(%s)",
        deb_ascii_nsgbl_ns(nsBuf,ns_p)));

    if ((status = dns_enable(ns_p)) == DNS_SUCCESS) 
	{
	    char name[32+4]; /* hostname maxes out at 31 chars. appending 3 */
	    SimpleName_u rootch;
	    int rootchlen = sizeof(rootch);
	    ObjUID_t nsuid;
	    register char *name_p;
	    

	    if (!sd_p->sd_auto || ns_p->ns_clearinghouse_count)
		goto auto_done;

	    if (gethostname(name, sizeof(name)) < 0)
	    {
	      dce_svc_printf(CDS_S_SERVER_GETHOSTNAME_MSG, strerror(errno));
	      status = errno;
	      goto auto_done;
	    }

	    /* use everything up to the first dot, if there is one */
	    if (!(name_p = strchr(name, '.')))
		name_p = strchr(name, '\0');

	    (void)strcpy(name_p, "_ch");
	    status = clerk_cvt_simple_to_opaque((char *)name, 0,
					   (char *)rootch, &rootchlen, NULL);
	    if (status != DNS_SUCCESS)
	    {
		dce_svc_printf(CDS_S_SERVER_CVTSIMPLE_MSG, (long)status);
		goto auto_done;
	    }

	    if ((status = dns_initialize(ns_p, (SimpleName_t *)rootch, &nsuid))
		== DNS_SUCCESS) {
	      goto auto_done;
	    }

	    dce_svc_printf(CDS_S_SERVER_AUTOFAIL_MSG);
	    server_shutdown(ns_p);
	} 
	else 
	{
	    dce_svc_printf(CDS_S_SERVER_ENABLEFAIL_MSG);
        }
auto_done:

#if defined(DCE_SEC)

/* Restore the security nsi binding state (formally 'BIND_PE_SITE')
 * to its original state.
*/
    (void)sec_rgy_enable_nsi(orig_sec_nsi_state);
#endif

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " <server_auto(%s) return(%s)",
        deb_ascii_nsgbl_ns(nsBuf,ns_p),
        deb_ascii_dns_status(statusBuf,status)));

    /* Release the memory */
    rpc_ss_disable_allocate();
    return(status);
}

/*
 * signal_catcher - seperate thread waits for asynchronous signals
 */
static void
signal_catcher (
    void)
{
    sigset_t	signals;
    int		sig, *sig_p;
    struct sigaction sigactn;

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_server, svc_c_debug8,
		   " >signal_catcher()"));


    sigemptyset(&signals);

    /*
     * These are the asynchronous signals we want to catch 
     */
    sig_p = catch;
    while (sig = *sig_p++)
	sigaddset(&signals, sig);

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
    while (1==1) 
    {
	sig = sigwait(&signals);
	break;
    } /* end of while */

    sigprocmask(SIG_UNBLOCK, &signals, NULL);

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_server, svc_c_debug8,
		   " <signal_catcher()"));
}

