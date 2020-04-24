/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: adver_main.c,v $
 * Revision 1.1.15.2  1996/02/18  19:23:20  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:07:44  marty]
 *
 * Revision 1.1.15.1  1995/12/08  00:10:09  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/jrr_1.2_mothra/1  1995/11/16  21:21 UTC  psn
 * 	Fix compilation problem for OSF 1.2.1 drop
 * 	[1995/12/07  23:53:13  root]
 * 
 * Revision 1.1.12.8  1994/09/06  17:36:52  proulx
 * 	fix timestamps
 * 	[1994/09/06  17:28:20  proulx]
 * 
 * 	fix timestamps
 * 
 * 	fix timestamps
 * 
 * 	fix timestamps
 * 
 * Revision 1.1.12.7  1994/08/23  19:57:50  mccann
 * 	change when startup message is displayed ot:11245
 * 	[1994/08/23  19:56:14  mccann]
 * 
 * Revision 1.1.12.6  1994/06/30  19:15:07  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:35:11  mccann]
 * 
 * 	sams cleanup drop 2
 * 
 * Revision 1.1.12.5  1994/06/09  16:06:50  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:51:21  devsrc]
 * 
 * Revision 1.1.12.4  1994/06/08  20:17:46  peckham
 * 	Use cds_server_set_progname().
 * 	[1994/06/06  20:25:51  peckham]
 * 
 * 	Initialize serviceability and message catalogs.
 * 	[1994/05/31  18:55:50  peckham]
 * 
 * Revision 1.1.12.3  1994/05/12  21:10:13  peckham
 * 	Remove signal_waker().
 * 	[1994/05/12  19:11:52  peckham]
 * 
 * Revision 1.1.12.2  1994/04/19  17:05:13  mccann
 * 	More code cleanup
 * 	[1994/04/19  16:23:52  mccann]
 * 
 * Revision 1.1.12.1  1994/03/12  21:54:51  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:44:40  peckham]
 * 
 * Revision 1.1.9.5  1993/01/26  14:06:51  mfox
 * 	Fix ANSI C compiler warnings.
 * 	[1993/01/26  13:59:49  mfox]
 * 
 * Revision 1.1.9.4  1992/12/30  12:34:54  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:17:15  zeliff]
 * 
 * Revision 1.1.9.3  1992/12/15  15:54:22  tom
 * 	Bug 5019 - Add setlocale() for correct messaging.
 * 	[1992/12/15  15:16:35  tom]
 * 
 * Revision 1.1.9.2  1992/09/29  18:43:32  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:08:34  weisman]
 * 
 * Revision 1.1.2.5  1992/06/05  20:37:13  keegan
 * 	Exit daemon with message if cannot access catalog files.
 * 	[1992/06/05  14:28:41  keegan]
 * 
 * Revision 1.1.2.4  1992/06/01  15:55:32  mfox
 * 	Replace BSD code with POSIX equivalents.
 * 	[1992/06/01  15:53:13  mfox]
 * 
 * Revision 1.1.2.3  1992/04/09  20:04:47  jim
 * 	renamed map_t structure to ETmap_t to preven collision
 * 	with machine defined structure on AIX.
 * 	[1992/04/09  18:50:03  jim]
 * 
 * Revision 1.1.2.2  1992/03/22  21:09:35  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:03:42  weisman]
 * 
 * Revision 1.1  1992/01/19  15:26:35  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * 	Module: adver_main.c
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

#include <adver.h>
#include <locale.h>
#include <rpcserver.h> 
#include <cache.h>
#include <dns_cfg.h>
#include <adver_ncl_defs.h>
#if defined(DCE_SEC)
# include <dce/sec_login.h>
#endif
#include <unistd.h>

/*
 * Main control structure
 */
struct data {
    void	*cache_h;	/* cache_handler handle */
    void	*clerk_h;	/* clerk handler handle */
    void	*server_h;	/* server handler handle */
    void	*rpc_h;		/* rpc handler handle */
};

/*
 * prototype declarations of procedures in this module
 */
int 
main (
    int              ,
    char            **);

/*
 * ..to extract command line arguments 
 */
static void
InitializeArgs (	
    int		     ,
    char	    **);

static void
signal_catcher (
    struct data *);

/*
 * Global Variables
 */
int permFD;				/* file descriptor to perm database */

char debug_mode = FALSE;                /* unix debug flag on? */

char stand_alone = FALSE;		/* non-zero -> drop advertisements */

char promiscuous = FALSE;               /* non-zero -> receive */
                                        /* advertisements from any cell */


/*
 * cds shared memory id 
 */
int sharedMemId;
int cache_size = -1;        /* cache size (in kbytes), -1 = use default,
			     *  changed via -c<kbytes> startup switch  */

/*
 * CDS CLERK structure 
 */

struct nsgbl_clerk cds_clerk;

/*
 * signal catching
 */

static int catch[] = {
    SIGCHLD,
# if defined(SIGCLD)
#  if SIGCHLD != SIGCLD
    SIGCLD,
#  endif
# endif
    SIGHUP, SIGINT, SIGTERM, SIGUSR1, 0
}, dump[] = {
    SIGQUIT, SIGILL, SIGTRAP, SIGIOT, SIGEMT, SIGFPE,
    SIGBUS, SIGSEGV, SIGSYS, 0
}, ignore[] = {
# if defined(SIGXCPU)
    SIGXCPU, SIGXFSZ,
# endif
    SIGIO, SIGPIPE, SIGALRM, SIGPROF, SIGUSR2, 0
};

/*
 *++
 *  main ()
 *
 *  Functional Description:
 *	Calls all the initialization routines and then
 *	goes into the top level loop of the advertiser.
 *	When the control program tells us to exit, this
 *	routines calls the cleanup and then exits.
 *
 *  Inputs:
 *	none
 *
 *  Implicit Inputs:
 *	none
 *
 *  Outputs:
 *	none
 *
 *  Implicit Outputs:
 *	status returned to DCL or other caller
 *
 *  Value Returned:
 *	none
 *
 *  Side Effects:
 *	runs the advertiser
 *--
 */

int 
main (int   argc,
      char  **argv)
{
    struct data		data;
    char 	       *myname_p;
    error_status_t 	st;
    int 		exitFlag;
    int			thstatus;

    memset(&data, 0, sizeof(data));

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

    dns_cfginit(NULL, argv[0]);
    dthread_init();

    InitializeArgs(argc, argv);

    /*
     * Advertiser start up message - wait for args to be processed for
     * correct logging
     */
    dce_svc_printf(CDS_S_ADVER_STARTUP_MSG);

    /* Must be root */
    if (dce_priv_proc_is_appropriate() != TRUE)
	dce_svc_printf(CDS_S_NOPRIV_EB_MSG);

    if (!debug_mode) 
	dns_coredefault(myname_p, FALSE);

    utc_gettime(&cds_clerk.cl_create_time);

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug1,
		   " Initializing RPC"));
    data.rpc_h = InitializeRPC (myname_p, debug_mode);

    if (!data.rpc_h)
    {
	dce_svc_printf(CDS_S_ADVER_EB_MSG);
    }

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug1,
		   " Loading cache"));
    data.cache_h = adver_cache_new(cache_size);	/* load the cache */

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug1,
		   " Setup server listener"));
    data.server_h = adver_server_new(data.cache_h, NULL); /* listen to cdsd */

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug1,
		   " Setup library listener"));
    data.clerk_h = adver_clerk_new(data.cache_h);	/* spawn clerks */

    /*
     * Start up a thread to listen for RPCs 
     */
    
    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug1,
		   " Start up listener"));
    adver_rpc_start(data.rpc_h, data.cache_h);

    /*
     * Detach from the user terminal (if -d not set)
     */
    if (!debug_mode)
        if((st = cds_server_detach (myname_p, NULL, NULL)) != 0) {
	  dce_svc_printf(CDS_S_ADVER_EB_MSG);
	}

    /* WAIT FOR TERMINATION */
    signal_catcher(&data);

    cds_solicit_kill(data.rpc_h);	/* don't receive advertisements */

    adver_clerk_delete(data.clerk_h);	/* don't spawn clerks */

    adver_server_delete(data.server_h);	/* don't talk to the server */

    TerminateRPC(data.rpc_h);		/* don't receive advertisements */

    adver_cache_delete(data.cache_h);	/* kill the cache */

    dthread_term();			/* good-bye dthreads */

    dce_svc_printf(CDS_S_ADVER_EG_MSG);
    
} /* End of routine main */

/*
 *  InitializeArgs ()
 */
static void
InitializeArgs (
		int argc,
		char **argv)
{

  extern char *optarg;
  register char *handle = dns_cfgopen(NULL, NULL);
  int  c;

  /*
   * Message routing variables
   */
  error_status_t w_err_st;
  dce_error_string_t inq_error_string;
  int 	   inq_sts;


# if defined(DCE_CDS_DEBUG)
    while ((c = getopt(argc, argv, "Dc:C:spd:w:")) != EOF) {
# else
    while ((c = getopt(argc, argv, "Dc:spd:w:")) != EOF) {
# endif

	switch (c) {
	    case 'c': /*
			 * cache size, in kbytes 
			 */
	                 cache_size = atoi(optarg);
	                  break;

	    case 'C': /*
		       * add this parameter to the file 
		       */
		      dns_cfgfromarg(handle, optarg);
		      break;

            case 'w': /*
                       * Startup Routing Specifications
                       */
                      dce_svc_routing((unsigned char *)optarg,&w_err_st);
		      if (w_err_st != rpc_s_ok) {
			dce_svc_printf(CDS_S_SVC_ROUTING_EB_MSG, 
				       (long)w_err_st);
		      }
		       break;

            case 'd': /*
                       * Startup DEBUG Routing Specifications
                       */
                      dce_svc_debug_routing((unsigned char *)optarg,&w_err_st);
		      if (w_err_st != rpc_s_ok) {
			dce_svc_printf(CDS_S_DEBUG_ROUTING_EB_MSG, (long)w_err_st);
		      }
                      break;

	    case 'D':
		      debug_mode = TRUE;
		      break;

	    case 'p': /*
		       * receive advertisements from any cell 
		       */
		      promiscuous = TRUE;  
		      break;
	    case 's': /*
		       * don't send or receive advertisements 
		       */
		      stand_alone = TRUE;
		      break;

	    default:
		      /*
                       * Give "usage" message.
                       */
                      dce_svc_printf(CDS_S_ADVER_USAGE_EB_MSG, argv[0]);

	 } /* end switch */

    } /* end while */

    /*
     * if a configuration parameter was set, re-write and close the file 
     */
    dns_cfgwrite(handle);
    dns_cfgclose(handle);
}

/*
 * signal_catcher - seperate thread waits for asynchronous signals
 */
static void
signal_catcher (
    struct data *data_p)
{
    dthread_t	waker_thread;
    sigset_t	signals;
    int		sig, *sig_p;
    struct sigaction sigactn;

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug8,
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
	switch (sig) {
            case SIGCHLD :
	    
#  if defined(SIGCLD)
#   if SIGCHLD != SIGCLD
	    case SIGCLD :
#   endif
#  endif
	      adver_clerk_signal(data_p->clerk_h, sig);
	      continue;

	    default:
	      break;
	} /* end of switch */

	break;
    } /* end of while */

    sigprocmask(SIG_UNBLOCK, &signals, NULL);

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug8,
		   " <signal_catcher()"));
}
