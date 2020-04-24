/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: gda_main.c,v $
 * Revision 1.1.85.1  1996/10/03  14:34:06  arvind
 * 	better NO_GDS support.
 * 	[1996/09/17  06:09 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 *
 * Revision 1.1.83.3  1996/02/18  19:31:11  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:12:16  marty]
 * 
 * Revision 1.1.83.2  1995/12/08  15:12:53  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/DCE1_1_WP/1  1995/05/08  14:42 UTC  jrr
 * 	Handle new cds.conf file format.
 * 	[1995/12/08  14:41:29  root]
 * 
 * Revision 1.1.78.10  1994/08/23  19:57:34  mccann
 * 	change when startup message is displayed ot:11245
 * 	[1994/08/23  19:52:35  mccann]
 * 
 * Revision 1.1.78.9  1994/08/03  19:01:38  mccann
 * 	includes cleanup
 * 	[1994/08/03  18:43:52  mccann]
 * 
 * Revision 1.1.78.8  1994/07/25  15:13:04  proulx
 * 	Use common ACL manager for subcomponent mgmt ACLs
 * 	[1994/07/25  14:01:15  proulx]
 * 
 * Revision 1.1.78.7  1994/06/23  18:30:04  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  19:27:46  mccann]
 * 
 * Revision 1.1.78.6  1994/06/09  18:38:06  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:10:16  devsrc]
 * 
 * Revision 1.1.78.5  1994/06/08  20:17:56  peckham
 * 	Use cds_server_set_progname().
 * 	[1994/06/06  20:26:02  peckham]
 * 
 * 	Initialize serviceability and message catalogs.
 * 	[1994/05/31  18:56:31  peckham]
 * 
 * Revision 1.1.78.4  1994/05/20  22:03:15  sommerfeld
 * 	rdaclif.h -> dce/rdaclif.h
 * 	[1994/05/17  19:34:38  sommerfeld]
 * 
 * Revision 1.1.78.3  1994/04/29  15:49:57  peckham
 * 	Use dcecdsmac.h macros.
 * 	Use dcelocal_path in file names.
 * 	[1994/04/29  13:56:47  peckham]
 * 
 * Revision 1.1.78.2  1994/03/12  22:01:39  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:09:17  peckham]
 * 
 * Revision 1.1.78.1  1993/12/29  19:22:31  rsalz
 * 	Use dce_cf.h, not dce_cf_sts.h which was a private header file.
 * 	Part of move dce_cf_XXX from config to dce/utils/dcecf (OT CR 9663).
 * 	[1993/12/29  18:22:08  rsalz]
 * 
 * Revision 1.1.6.6  1992/12/30  13:27:16  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:43:58  zeliff]
 * 
 * Revision 1.1.6.5  1992/12/18  21:41:50  mfox
 * 	Update usage statement, hide some options with #ifdef DEBUG and
 * 	delete obsolete options.
 * 	[1992/12/18  21:41:02  mfox]
 * 
 * Revision 1.1.6.4  1992/12/15  15:54:55  tom
 * 	Bug 6409 - add setlocale call for i18n.
 * 	[1992/12/15  15:17:20  tom]
 * 
 * Revision 1.1.6.3  1992/10/26  18:37:07  peckham
 * 	OT#4001: Remove cma_*_global() references.
 * 	[1992/10/26  18:36:12  peckham]
 * 
 * Revision 1.1.6.2  1992/09/29  19:12:44  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:12:12  weisman]
 * 
 * Revision 1.1.2.6  1992/06/01  18:44:58  mfox
 * 	Don't copy alternate strings over defaults because the default literal
 * 	can be placed into read-only text segments by optimizing compilers. Use
 * 	pointers rather than arrays for a number of strings.
 * 	[1992/06/01  18:43:23  mfox]
 * 
 * Revision 1.1.2.5  1992/06/01  14:15:59  grober
 * 	Added hpux specific changes inside of #ifdef __hpux .
 * 	[1992/05/27  15:26:42  grober]
 * 
 * Revision 1.1.2.4  1992/05/22  14:01:00  mfox
 * 	Instead of aborting silently when BIND is not installed, reject DNS
 * 	names more gracefully.
 * 	[1992/05/21  23:43:14  mfox]
 * 
 * Revision 1.1.2.3  1992/03/22  21:56:51  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:24:14  weisman]
 * 
 * Revision 1.1.2.2  1992/01/22  23:01:27  melman
 * 	Adding changes from defect_fixes archive.
 * 	[1992/01/22  22:16:37  melman]
 * 
 * $EndLog$
 */
/*
 * Module: gda_main.c
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
 *	gda_main.c contains all of the routines which handle tasks which are
 *	related to the workings of the GDA as a whole.  These tasks
 *	include initialization and the maintainance of the debugging
 *	subsystem.
 *
 */

#define GDA_MAIN

#include <gda.h>
#include <dce/dcesvcmsg.h>
#include <dce/dce_priv_proc.h>
#include <dce/assert.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/time.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <locale.h>
#include <gda_main.h>
#include <gda_debug.h>
#include <gda_xds.h>
#include <uc_clerk.h>
#include <threads_lib.h>
#include <dce/cds_clerkserver.h>
#include <tower.h>
#include <dce/attributes.h>
#include <dns_strings.h>
#include <rpcserver.h>
#if defined(DCE_SEC)
# include <dns_cfg.h>
# include <dce/dce_cf.h>
#endif

static char *myname_p;

/*
 * choose_conduit error code 
 */
#define CHOOSE_CONDUIT_FAIL             -4

/*
 * Entry name for the serviceabilty I/F of gda
 */ 
#define GDA_ENTRY     "/cds-gda"

/*
 * Local function prototypes.
 */

int 
main (int   ,
      char  **);

static void
usage(
      void);

static void 
init_conduits (
               void);

static void 
init_main (
           char  **);

static int 
sec_init (
	  void);

extern  conduit_el_t    bind_conduit;
#ifdef DO_X500
extern  conduit_el_t    X500_conduit;
#endif

#if defined(DCE_CDS_DEBUG)

/* 
 * mem_stat is used in gda_debug.h
 */
struct memory_stat      mem_stat; 
 
#endif

static pthread_once_t gda__once = pthread_once_init;
static pthread_mutex_t gda__mutex;

/*
 * Initialize this module's statics
 */
static void 
gda__init (void)
{
    pthread_mutex_init(&gda__mutex, pthread_mutexattr_default);
}

#if defined(DCE_SEC)
char gda_principal_name[256]; /* GDA's principal name */
/* thread attributes for server refresh and key mgmt threads */
extern dthread_attr_t refresh_attr;
extern dthread_attr_t manage_attr;

#endif

extern uuid_t gda_mgmt_dacl_uuid;

int exit_code;          /* exit code set by error_exit and read by main*/

/*
 * mutex and condtion on which main waits for the death of the GDA 
 */
pthread_mutex_t         death_mutex;
pthread_cond_t          death_cond;

int                     no_update = 0;
                                /* 1 = don't call update parent */

list_t                  question_list;
                                /* question is a thread protected list.  It
                                stores all of the question currently asked
                                of the GDA */

pthread_key_t           thread_num;
                                /* number of the thread currently running */

char                    *named_servers = NULL;
                                /* file which stores the names of BIND
                                root nameservers */
char                    *named_client_servers = "/etc/resolv.conf";
                                /* file which stores the names of BIND
                                servers when the gda asks as a client */
int                     nofork = 0;
                                /* if 0, the GDA should fork and kill the
                                parent. Otherwise, it should not fork. */

int                     enable_bind = 1;
                                /* if 1, bind code should be initialized */

#ifdef DO_X500
int                     enable_X500 = 1;
                                /* if 1, X500 code should be initialized */
#else
int                     enable_X500 = 0;
#endif

uuid_t                  ch_id;
                                /* my clearinghouse uuid */
/*
 * variables handy for debugging purposes 
 */
question_el_t           *q_debug;
rr_el_t                 *rr_debug;
element_t               *el_debug;

/*
 * thread numbers for the thread_num thread specific data 
 */
long    cds_createsoftlink_th_num       = CDS_CREATESOFTLINK_TH_NUM;
long    cds_createobject_th_num         = CDS_CREATEOBJECT_TH_NUM;
long    cds_deleteobject_th_num         = CDS_DELETE_OBJECT_TH_NUM;
long    cds_deletesoftlink_th_num       = CDS_DELETESOFTLINK_TH_NUM;
long    cds_deletedirectory_th_num      = CDS_DELETEDIRECTORY_TH_NUM;
long    cds_enumerateattributes_th_num  = CDS_ENUMERATEATTRIBUTES_TH_NUM;
long    cds_enumeratechildren_th_num    = CDS_ENUMERATECHILDREN_TH_NUM;
long    cds_enumerateobjects_th_num     = CDS_ENUMERATEOBJECTS_TH_NUM;
long    cds_enumeratesoftlinks_th_num   = CDS_ENUMERATESOFTLINKS_TH_NUM;
long    cds_modifyattribute_th_num      = CDS_MODIFYATTRIBUTE_TH_NUM;
long    cds_readattribute_th_num        = CDS_READ_ATTRIBUTE_TH_NUM;
long    cds_resolvename_th_num          = CDS_RESOLVENAME_TH_NUM;
long    cds_skulk_th_num                = CDS_SKULK_TH_NUM;
long    cds_testattribute_th_num        = CDS_TESTATTRIBUTE_TH_NUM;
long    cds_testgroup_th_num            = CDS_TESTGROUP_TH_NUM;
long    cds_allowclearinghouses_th_num  = CDS_ALLOWCLEARINGHOUSES_TH_NUM;
long    cds_disallowclearinghouses_th_num = CDS_DISALLOWCLEARINGHOUSES_TH_NUM;
long    recv_bind_th_num                = RECV_BIND_TH_NUM;
long    main_th_num                     = MAIN_TH_NUM;
long    rpc_listener_th_num             = RPC_LISTENER_TH_NUM;
long    memstat_th_num                  = MEMSTAT_TH_NUM;
long    cds_doupdate_th_num             = CDS_DOUPDATE_TH_NUM;
long    cds_combine_th_num              = CDS_COMBINE_TH_NUM;
long    cds_linkreplica_th_num          = CDS_LINKREPLICA_TH_NUM;
long    cds_modifyreplica_th_num        = CDS_MODIFYREPLICA_TH_NUM;
long    cds_addreplica_th_num           = CDS_ADDREPLICA_TH_NUM;
long    cds_removereplica_th_num        = CDS_REMOVEREPLICA_TH_NUM;
long    cds_newepoch_th_num             = CDS_NEWEPOCH_TH_NUM;
long    cds_deletechild_th_num          = CDS_DELETECHILD_TH_NUM;
long    cds_createchild_th_num          = CDS_CREATECHILD_TH_NUM;
long    cds_createdirectory_th_num      = CDS_CREATEDIRECTORY_TH_NUM;


/*
 * init_conduits initializes the conduits available to the GDA.
 *It returns nothing.
 */
static void 
init_conduits (void)
{
        conduit_el_t    *bind_c = &bind_conduit;
#ifdef DO_X500
        conduit_el_t    *X500_c = &X500_conduit;
#endif
        int             status;

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " >init_conduits"));

        if(enable_bind)
        {
	    /*
	     * since bind is enable, call its initializing routine 
	     */
	    rwl_init(&bind_c->rwl_c);
	    if (!bind_c->init_conduit())
		enable_bind = 0;        /* BIND not installed */
	  }
#ifdef DO_X500
        if (enable_X500)
        {
	    /*
	     * since X500 is enable, call its initializing routine 
	     */
	      rwl_init(&X500_c->rwl_c);
	      (void)X500_c->init_conduit();
        }
#endif

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " <init_conduits"));

}

/*
        choose_conduit takes as input a question and determines if the
        question being asked should be forwarded to BIND or X500.
        choose_conduit returns OK if no error occurs or CHOOSE_CONDUIT_FAIL
        if the conduit choosen has not been enabled.
*/
int 
choose_conduit (question_el_t *question)
{
       DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " >choose_conduits"));

        if(is_X500(&question->unresolved_in_q))
        {
#ifdef DO_X500
                /*
                 * the question should be resolved by X500 
                 */
                if(enable_X500)
                {
                        /*
                         * X500 is enabled, set conduit_q 
                         */
                        question->conduit_q = &X500_conduit;
                }
                else
#endif
                {
		    /*
		     * X500 is not enabled, return an error 
		     */
		    dce_svc_printf(CDS_S_GDA_NOX500_MSG);

		    question->conduit_q = NULL;

		    return(CHOOSE_CONDUIT_FAIL);
                }

        }
        else
        {
                /*
                 * the question should be resolved by BIND 
                 */
                if(enable_bind)
                {
		    /*
		     * bind is enabled, set conduit_q 
		     */
		    question->conduit_q = &bind_conduit;
                }
                else
                {
		    /*
		     * BIND is not enabled, return an error 
		     */

		    dce_svc_printf(CDS_S_GDA_NODNS_MSG);
	      
		    question->conduit_q = NULL;

		    return(CHOOSE_CONDUIT_FAIL);
                }
        }

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " <choose_conduit"));

      return(OK);
}

/*
        error_exit_th prints out an error message, then exits from the
        current thread.  When the main thread wakes it terminates the
        process by exiting.
*/

void
error_exit_th (int code)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_gda,
        svc_c_debug2,
        " >error_exit_th"));

    /*
     * exit from the thread 
     */
    pthread_exit((pthread_addr_t)&code);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_gda,
        svc_c_debug2,
        " <error_exit_th"));
}

/*
        error_exit prints out an error message, signals the main thread to wake
        up then exits from the current thread.  When the main thread wakes
        it terminates the process by exiting.
*/

void
error_exit (int   code)
{   
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_gda,
        svc_c_debug2,
        " >error_exit"));

    /*
     * set the exit_code used by main 
     */
    exit_code = code;

    /*
     * signal main wakeup 
     */
    if (pthread_cond_signal(&death_cond))
	dce_svc_printf(CDS_S_PTHREADCONDSIGNAL_MSG, (long)errno);

    /*
     * exit from the thread 
     */
    error_exit_th(code);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_gda,
        svc_c_debug2,
        " <error_exit"));
}

/*
 * init_main initializes the GDA code.  It kills the GDA if an error
 * occurs and returns nothing otherwise.
 */
static void 
init_main (char  **argv)
{
  unsigned int    status;
  char            logfile[256];
  error_status_t  rpc_status, 
  temp_status;
  int             inq_status;
  uuid_vector_t   uuids;
  dce_error_string_t err_buf;
  char             *hostName;
  char             svc_ns_entry[CDS_FNAME_MAX];

  DCE_SVC_DEBUG((
		 cds__svc_handle,
		 cds_svc_gda,
		 svc_c_debug8,
		 " >init_main"));
  /*
   * initialize the pthread data 
   */
  pthread_once(&gda__once, gda__init);

  /*
   * save away an old core and cd to our working directory 
   */
  dns_coredefault(myname_p, 1);

  /*
   * initialize the thread_num thread specific data 
   */
  if (pthread_keycreate(&thread_num, (pthread_destructor_t)NULL)) {
    dce_svc_printf(CDS_S_PTHREADKEYCREATE_MSG, (long)errno);
    dce_svc_printf(CDS_S_GDA_EB_MSG);
  }

  if (pthread_setspecific(thread_num, (pthread_addr_t)&main_th_num)) {
    dce_svc_printf(CDS_S_PTHREADSETSPECIFIC_MSG, (long)errno);
    dce_svc_printf(CDS_S_GDA_EB_MSG);
  }

#if defined(DCE_CDS_DEBUG)
  /*
   * Set up debug thread to monitor memory usage.
   */
  init_memstat();
#endif /* DCE_CDS_DEBUG */

  /*
   * init the debug code 
   */
  dns_cfginit(NULL, argv[0]);     /* Configuration file */
  /*
   * Initialize the RPC interface and check to see if there is a gdad
   * already running. Note: can't export bindings to namespace, yet,
   * 'cuz we haven't set up our DCE identity yet. We don't want to
   * do security initialization until we've forked (in InitializeRPC),
   * because the threads set up in 'sec_init' won't survive the fork.
   */
  if ((status = InitializeRPC (myname_p, nofork)) != CDS_SUCCESS) 
      {
	dce_svc_printf(CDS_S_GDA_INITRPC_EB_MSG);
      }
  /*
   * Initialize security (must be done before an outgoing or incoming
   * requests occur. Also, reads from config file, thus do cfginit above)
   */

#if defined(DCE_SEC)
  if (sec_init() == FALSE)
      {
	dce_svc_printf(CDS_S_GDA_INITSEC_EB_MSG);
      }
#endif

  /*
   * make sure that the /... entry is in the CDS database 
   */
  if (!no_update)
      {
	DCE_SVC_DEBUG((
		       cds__svc_handle,
		       cds_svc_gda,
		       svc_c_debug3,
		       "init_main: Updating parent pointer in CDS"));
	
	if ((status = update_parent(&ch_id)) != DNS_SUCCESS)
            {
	      dce_svc_printf(CDS_S_GDA_EB_MSG);
            }
      }
  l_init(&question_list);

  /* 
   * create a mutex and condition on which main will wait until the
   * GDA dies 
   */
  if (pthread_cond_init(&death_cond, pthread_condattr_default))
      {
	dce_svc_printf(CDS_S_PTHREADCONDINIT_A_MSG, (long)errno);
      }

  if (pthread_mutex_init(&death_mutex, pthread_mutexattr_default))
      {
	dce_svc_printf(CDS_S_PTHREADMUTEXINIT_A_MSG, (long)errno);
      }

  init_conduits();
                          
  /*
   * Get the host entry name
   */
  dce_cf_get_host_name (&hostName, &rpc_status);
  
  if (rpc_status != dce_cf_st_ok)
      {
	dce_svc_printf(CDS_S_CFGETHOSTNAME_MSG, (long)rpc_status);
	dce_svc_printf(CDS_S_GDA_EB_MSG);
      }

  strcpy (svc_ns_entry,CELL_DIR_ROOT);
  strcat (svc_ns_entry,hostName);
  strcat (svc_ns_entry,GDA_ENTRY);

  {
     char servername[]="cds-gda";

     /*
      * Correct the entry in the namespace if we have had our host
      * IP address changed since configuration.  export_ns_new will 
      * the binding vector, so don't free it here.  It doesn't 
      * take much space anyway.
      */
     export_ns_new (myname_p, servername, serviceability_v1_0_s_ifspec,
		    gdad_bind_vector, &gda_mgmt_dacl_uuid, &status);
     if (status != error_status_ok) {
	dce_svc_printf(CDS_S_GDA_EB_MSG);
     }
  }

  /*
   * No longer need the returned hostname
   */
  rpc_string_free ((unsigned_char_t **)&hostName, &temp_status);
  
  /*
   * Detach from user terminal (if -f not set) 
   */
  if (!nofork) 
      {
	(void)sprintf (logfile, "%s.log", myname_p);
	if ((status = cds_server_detach (myname_p, logfile, NULL)) != 0)
	    {
	      dce_svc_printf(CDS_S_GDA_EB_MSG);
	    }
	    
      }

  DCE_SVC_DEBUG((
		 cds__svc_handle,
		 cds_svc_gda,
		 svc_c_debug8,
		 " <init_main"));

}
#if defined(DCE_SEC)
 
/*
 * ROUTINE - sec_init
 *
 * Security initialization.
 * Perform a secure login using the principal name from the configuration
 * file, if present, otherwise, construct the well know server name from our
 * host name.
 * If present, use the password from the config file to validate the login,
 * otherwise, get the encrypted password record from the local key file.
 * Also, register authentication info with rpc, for the clerk-server
 * RPC interface.
 *
 * (when available, this function will also call the H.P provided
 *    function for managing keys (refreshing identity).)
 *
 * RETURNS: TRUE or FALSE
*/

static int 
sec_init (void)
{
  char passwd[100];
  char *handle, *p, *cellname, *hostname;
  error_status_t st;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_gda,
      svc_c_debug2,
      " >sec_init"));

  handle = dns_cfgopen(NULL, NULL);


  /* 
   * Check cds.conf for presence of principal name. If absent, generate:
   * <cellname>/hosts/<hostname>/gda
   *
   * Reject old server_princ_names containing cellname to avoid trouble 
   * with changed cellnames CHFts15155 
   */
   if ((p = dns_cfgtostr(handle, "security.server_princ_name", NULL)) == NULL || 
       !strncmp(p, "/.../", 5))
   {
      dce_cf_get_cell_name (&cellname, &st);
      if (st != dce_cf_st_ok) 
      {
	dce_svc_printf(CDS_S_CFGETCELLNAME_MSG, (long)st);
	DCE_SVC_DEBUG((
	      cds__svc_handle,
	      cds_svc_gda,
	      svc_c_debug2,
	      " <sec_init: dce_cf_get_cell_name failed -> return(FALSE)"));

	  return (FALSE);
      }
      
      dce_cf_get_host_name (&hostname, &st);
      if (st != dce_cf_st_ok) 
      {
	dce_svc_printf(CDS_S_CFGETHOSTNAME_MSG, (long)st);
	  DCE_SVC_DEBUG((
	      cds__svc_handle,
	      cds_svc_gda,
	      svc_c_debug2,
	      " <sec_init:  dce_cf_get_host_name failed -> return(FALSE)"));

	  return (FALSE);
      }

      (void)sprintf ((char *)gda_principal_name, "%s/%s/gda", cellname, hostname);
      free (cellname);
      free (hostname);

  }
  else strcpy (gda_principal_name,p);

  if ((p = dns_cfgtostr(handle, "security.server_passwd", NULL)) == NULL)
  {
      *passwd = '\0';
  }
  else
  {
      strcpy (passwd,p);
  }

  dns_cfgclose (handle);

  /*
   * Do secure login, fire off threads to manage server key and refresh
   * our identity.
   */
  if (!server_sec_init ((unsigned char *)gda_principal_name,
                        passwd,
                        0,
                        NULL,
                        NULL,
                        NULL,
                        NULL)) 
  {
      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " <sec_init:  server_sec_init failed -> return(FALSE)"));

      return (FALSE);
  }

  /*
   * Initialize the gda management DACL and its mutex 
   */
  if ( gda_mgmt_acl_init () != error_status_ok ) 
    return(FALSE);
  
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_gda,
      svc_c_debug2,
      " <sec_init:  return(TRUE)"));

  return(TRUE);
}

#endif /* DCE_SEC */


static void
usage(void)
{
    dce_svc_printf(CDS_S_GDA_USAGE_EG_MSG, myname_p);
}


/*
 *  main is the point of entry for the GDA.  It reads parameters from the
 *  input line, initializes the gda code and then hangs of a condition
 *  until the GDA is terminated.
 */
int 
main (int   argc,
      char  **argv)
{
  extern char       *optarg;      /* argument parameter */
  int                c;
  error_status_t     status;
  

  /*
   * Get our filename, stripped of directories 
   */
  if (myname_p = strrchr(argv[0], '/')) {
    myname_p++;
  } else {
    myname_p = argv[0];
  }

  setlocale(LC_ALL, "");

  cds_server_set_progname(myname_p);

  dce_msg_define_msg_table(cds_msg_table,
			   sizeof(cds_msg_table)/sizeof(cds_msg_table[0]),
			   &status);
  dce_assert(cds__svc_handle, (status == msg_s_ok));

  cds_ifspec = cds_clerkserver_v1_0_s_ifspec;
  
  while ((c = getopt(argc, argv, "bd:Dr:s:uvw:x")) != EOF) {
    switch(c) {
	  case 'b':
                /*
		 * disable the bind conduit, overides the default 
		 */
                 if(enable_X500 == 0)
		     {
		       usage();
		     }
		 else
		     enable_bind = 0;
		 
		 break;
		 
	  case 'd': 
		 /*
		  * Set the debug routing.
		  */
		 dce_svc_debug_routing((unsigned char *)optarg,&status);
		 if (status != svc_s_ok)
		     dce_svc_printf(CDS_S_DEBUG_ROUTING_EB_MSG, (long)status);
		 break;

	  case 'D':
		 /*
		  * do not fork, allows for debug, log to stdout 
		  */
		 nofork = 1;
		 break;

	  case 'r':
		 /*
		  * change the default resolv.conf file 
		  */
		 named_client_servers = optarg;
		 break;

	  case 's':
		 /*
		  * change the default bind server file 
		  */
		 named_servers = optarg;
		 break;
		 
	  case 'u':
		 /*
		  * don't update parent 
		  */
		 no_update = 1;
		 break;
		 
	  case 'w': 
		 /*
		  * Set the message routing.
		  */
		 dce_svc_routing((unsigned char *)optarg,&status);
		 if (status != svc_s_ok)
		     dce_svc_printf(CDS_S_SVC_ROUTING_EB_MSG, (long)status);
		 break;
			
	  case 'x':
		 /*
		  * disable the bind conduit, overides the default 
		  */
		 if(enable_bind == 0)
		     {
		       usage();
		     }
		 else
		     enable_X500 = 0;
		 
		 break;

	  default:
		 usage();
		 break;
	       }
  }

  /*
   * GDA start up message - save till now so routing setup
   */
  dce_svc_printf(CDS_S_GDA_STARTUP_MSG);
  
  /*
   * must be a privileged user to start the gda daemon
   */
  if (!dce_priv_proc_is_appropriate())
      {
	dce_svc_printf(CDS_S_NOPRIV_EB_MSG);
      }
  
  /*
   * if the 's' option is not supplied, then named_servers has to point to
   * default path for named.ca.
   */
  if (!named_servers) {
    static char cds_bind_svrs[_DNS_MAXPATHLEN+1];
    
    (void)sprintf(cds_bind_svrs, CDS_BIND_SVRS, dcelocal_path);
    named_servers = cds_bind_svrs;
  }

  /* 
   *Init dthreads for the 2 security threads.
   */

  dthread_init();
  /*
   * initialize the gda 
   */
  init_main(argv);

  /*
   * wait until the GDA dies 
   */
  if (pthread_mutex_lock(&death_mutex))
      {
	dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_MSG, (long)errno);
	dce_svc_printf(CDS_S_GDA_EB_MSG);
      }

  if (pthread_cond_wait(&death_cond, &death_mutex))
      { 
	dce_svc_printf(CDS_S_PTHREADCONDWAIT_MSG, (long)errno);
	dce_svc_printf(CDS_S_GDA_EB_MSG);
      }

  if(pthread_mutex_unlock(&death_mutex))
      { 
	dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_MSG, (long)errno);
	dce_svc_printf(CDS_S_GDA_EB_MSG);
      }

  if (exit_code)
      dce_svc_printf(CDS_S_GDA_EB_MSG);
  else
      dce_svc_printf(CDS_S_GDA_EG_MSG);


}
