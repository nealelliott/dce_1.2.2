/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * (c) Copyright International Business Machines Corp. 1994, 1996
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 *
 */
/*
 * HISTORY
 * $Log: dmn_main.c,v $
 * Revision 1.1.15.1  1996/07/08  18:25:37  arvind
 * 	Audit for 1.2.2
 * 	[1996/06/24  16:40 UTC  psn  /main/arvind_audit122/2]
 *
 * 	Remove SIGDANGER.
 * 	[1996/06/21  20:24 UTC  arvind  /main/arvind_audit122/1]
 *
 * 	Audit for 1.2.2
 * 	[1996/02/18  00:06:02  marty  1.1.12.3]
 *
 * Revision 1.1.12.3  1996/02/18  00:06:02  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:57:36  marty]
 * 
 * Revision 1.1.12.2  1995/12/08  17:38:21  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/jrr_1.2_mothra/2  1995/11/08  21:08 UTC  dat
 * 	Remove #ifdef _AIX from #include <sys/stat.h>
 * 
 * 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/08  20:33 UTC  dat
 * 	Include sys/stat.h on the AIX platform to provide permission bit definitions (S_IRUSR, etc)
 * 
 * 	HP revision /main/HPDCE02/4  1995/08/17  19:53 UTC  jrr
 * 	Register a well-known object UUID in the namespace.
 * 	[1995/08/16  14:39 UTC  jrr  /main/HPDCE02/jrr_dcecp_01/1]
 * 
 * 	HP revision /main/HPDCE02/3  1995/07/20  20:18 UTC  cuti
 * 	Initialize uninitized varaible unveil by  compiling with +O2
 * 
 * 	HP revision /main/HPDCE02/cuti_mothra10_fix/2  1995/07/19  17:34 UTC  cuti
 * 	CHFts15455
 * 
 * 	HP revision /main/HPDCE02/cuti_mothra10_fix/1  1995/07/19  13:41 UTC  cuti
 * 	CHFts15455: Initialize unintialized variables unveil by +O2
 * 
 * 	HP revision /main/HPDCE02/2  1995/05/19  16:43 UTC  jrr
 * 	Merge work for CHFts15152.
 * 
 * 	HP revision /main/HPDCE02/jrr_sec_chfts15152/1  1995/05/19  16:42 UTC  jrr
 * 	Create thread for client updates (CHFts15152).
 * 
 * 	HP revision /main/HPDCE02/1  1995/02/13  22:38 UTC  mob
 * 	CHFts14288 - make auditd behave like a unix daemon
 * 
 * 	HP revision /main/mob_mothra2/1  1995/02/11  22:17 UTC  mob
 * 	CHFts14288
 * 	Make auditd behave like a real unix daemon so that it can be started remotely.
 * 	[1995/12/08  17:17:34  root]
 * 
 * Revision 1.1.8.1  1994/10/20  14:17:30  weisz
 * 	CR 12677: if not in selfaudit mode, use correct flag in dce_aud_open.
 * 	[1994/10/20  14:16:46  weisz]
 * 
 * Revision 1.1.2.17  1994/09/29  21:33:01  weisz
 * 	CR 12304: check for disabled state if in selfaudit mode.
 * 	[1994/09/29  21:32:17  weisz]
 * 
 * Revision 1.1.2.16  1994/09/23  15:29:36  weisz
 * 	CR 10703: handling of trail full situation.
 * 	[1994/09/23  15:29:19  weisz]
 * 
 * Revision 1.1.2.15  1994/09/22  21:08:57  weisz
 * 	CR 10703: handling of trail full situation.
 * 	[1994/09/22  21:07:06  weisz]
 * 
 * Revision 1.1.2.14  1994/08/15  16:21:31  blazin
 * 	Second submit for Cr 11431.  Removed code that was only commented out in
 * 	first submit.  This was code that handled "-i" identity option and that
 * 	spawned a thread which would manage the creds.
 * 	[1994/08/15  16:21:04  blazin]
 * 
 * Revision 1.1.2.13  1994/08/12  19:14:36  blazin
 * 	Merged with changes from 1.1.2.12
 * 	[1994/08/12  19:14:28  blazin]
 * 
 * 	For CR 11431 made the auditd always run as self.  Also deleted "-i" identity
 * 	option and keymgt thread as they are no longer supported when auditd only
 * 	runs as self.
 * 	[1994/08/12  19:09:01  blazin]
 * 
 * Revision 1.1.2.12  1994/08/11  15:38:04  weisz
 * 	CR 11642: unregister auditd i/f from endpoint mapper.
 * 	[1994/08/11  15:37:30  weisz]
 * 
 * Revision 1.1.2.11  1994/07/28  21:00:52  rousseau
 * 	Merged with changes from 1.1.2.10
 * 	[1994/07/28  21:00:44  rousseau]
 * 
 * 	Fix registration order and swap wrong out of order error messages.
 * 	[1994/07/28  20:57:52  rousseau]
 * 
 * Revision 1.1.2.10  1994/07/26  17:27:04  weisz
 * 	CR 11425: remove own acl manager and use DCE acl mgr library.
 * 	[1994/07/26  17:25:00  weisz]
 * 
 * Revision 1.1.2.9  1994/07/18  15:57:58  rousseau
 * 	Merged with changes from 1.1.2.8
 * 	[1994/07/18  15:57:51  rousseau]
 * 
 * 	Export all three if's to one namespace entry (CR 11298).
 * 	[1994/07/18  15:51:07  rousseau]
 * 
 * Revision 1.1.2.8  1994/07/14  19:48:57  weisz
 * 	CR 11167: DCEAUDITOFF should not affect trail opened for read.
 * 	[1994/07/14  19:48:24  weisz]
 * 
 * Revision 1.1.2.7  1994/05/29  21:50:40  weisz
 * 	CR 10754: added unexport from CDS for cases of dce_svc_printf with exit_bad.
 * 	[1994/05/29  21:47:08  weisz]
 * 
 * Revision 1.1.2.6  1994/04/28  17:19:28  ohara
 * 	add stdio.h
 * 	[1994/04/28  17:14:17  ohara]
 * 
 * Revision 1.1.2.5  1994/04/13  14:29:54  weisz
 * 	CR 10336: typo - second unexport had serviceability instead of rdaclif
 * 	          added unexport calls to the other exit possibilities.
 * 	[1994/04/13  14:29:27  weisz]
 * 
 * Revision 1.1.2.4  1994/03/11  22:48:27  luan
 * 	CR 9775 9776: Changed to use dce_aud_svc_init() for svc initialization.
 * 	[1994/03/11  22:38:33  luan]
 * 
 * Revision 1.1.2.3  1994/02/18  19:42:55  luan
 * 	CR 9777: Changed to use <dce/audit.h> and <dce/dceaudmsg.h>
 * 	instead of <audit.h> and <dceaudmsg.h>.
 * 	[1994/02/11  02:55:12  luan]
 * 
 * Revision 1.1.2.2  1994/01/25  04:59:35  luan
 * 	CR 9745 9746: File name paths consolidated. Hard-coded file names replaced with macros which generate names dynamically.
 * 	[1994/01/25  04:01:21  luan]
 * 
 * Revision 1.1.2.1  1993/12/22  12:38:52  zeliff
 * 	First version of code
 * 	[1993/12/21  15:01:10  zeliff]
 * 
 * $EndLog$
 */

/* 
** dmn_main.c 
** Main program of auditd. 
*/
#include <fcntl.h>
#include <locale.h>
#include <sys/stat.h>

#include <pthread.h>
#include <dce/dce.h>
#include <dce/dce_svc.h>
#include <stdio.h>
#include <dce/dce_cf.h>
#include <dce/sec_login.h>
#include <dce/rpc.h>
#include <dce/rdaclif.h>   /* new ACL interface */
#include <dce/rdaclifv0.h>

#include <dce/audit.h>
#include <dce/dceaudmsg.h>
#include <dce/dceaudmac.h>
#include <dce/dceaudsvc.h>

#include <error_statusp.h>
#include <eslp.h>
#include <esl_filep.h>
#include <pathsp.h>

#include <dmn_aud_evtsp.h>

#define CONCURRENCY_LEVEL          8
#define ONE_MINUTE                 60
#define FIVE_MINUTES               300
#define TEN_MINUTES                600
#define NAME_BUF_SIZE	           256
#define TASK_STACK_SIZE            64*1024

#define ADMIN_DIR_SUBPATH          "/var/audit/adm"
#define DEFAULT_ACL_FILE_SUBPATH   "/var/audit/adm/acl"
#define AUD_DIRECTORY_NAME         "audit-server"

/*  When auditd is run in the background, stdout, stderr
 *  and stdin are first closed. Then stdout and stderr
 *  are redirected to a log file. This is so that
 *  auditd can be started remotely via remsh/rsh .
 *  The log file name is obtained by concatenating
 *  dcelocal_path with AUDITD_LOGFILE .
 *
 *  auditd must also change to a well known directory
 *  so that core dumps can be easily located . This
 *  directory is obtained by concatenating dcelocal_path
 *  with AUDITD_LOGFILE .
 *
 *  The definitions ensure that the log file and the
 *  core dump are in the same directory.
 *
 */

#define AUDIT_DIRECTORY            "/var/audit"
#define AUDITD_LOGFILE             DCE_CONCAT(AUDIT_DIRECTORY, "/auditd.log")


serviceability_v1_0_epv_t          dce_svc_ep;
dce_aud_trail_t                    central_trail;
pthread_attr_t                     attr;
pthread_t                          distinguished_task;

boolean32     non_dt_fatal_error = FALSE;
boolean32     got_login_context  = FALSE;
boolean32     audit              = FALSE;

static char * central_trail_name = NULL;
extern pthread_mutex_t aud_g_dmn_esl_mutex;
extern pthread_mutex_t aud_g_dmn_state_mutex;
extern pthread_mutex_t aud_g_dmn_trail_mutex;
extern pthread_mutex_t aud_g_update_list_lock;
extern pthread_cond_t  aud_g_update_list_cond;

extern time_t aud_esl_input_file(aud_esl_type_t, unsigned32,
				 unsigned32 *, void ***,
				 aud_esl_guides_p_t *, 
				 aud_esl_guides_p_t *);

extern time_t aud_esl_input_update_file(aud_esl_type_t, unsigned32,
				        unsigned32 *, void *** );

extern void *aud_update_client(void *);


static aud_esl_entry_p_t dmn_esl_princ[HASH_TABLE_SIZE];
static aud_esl_foreign_entry_p_t dmn_esl_foreign_princ[HASH_TABLE_SIZE];
static aud_esl_entry_p_t dmn_esl_group[HASH_TABLE_SIZE];
static aud_esl_foreign_entry_p_t dmn_esl_foreign_group[HASH_TABLE_SIZE];
static aud_esl_entry_p_t dmn_esl_cell[HASH_TABLE_SIZE];
static aud_esl_entry_p_t dmn_esl_cell_overridable[HASH_TABLE_SIZE];

void ** aud_g_dmn_esl[aud_e_esl_cell_overridable + 1] = {
					    (void **) dmn_esl_princ,
				    (void **) dmn_esl_foreign_princ,
					    (void **) dmn_esl_group,
				    (void **) dmn_esl_foreign_group,
					     (void **) dmn_esl_cell,
				  (void **) dmn_esl_cell_overridable
};

static uuid_t aud_g_object_uuid = { /* 86576546-d7d3-11ce-a9b3-08000992d971 */
                                    0x86576546, 0xd7d3, 0x11ce, 0xa9, 0xb3,
                                    { 0x08, 0x00, 0x09, 0x92, 0xd9, 0x71 }
};

aud_esl_guides_p_t aud_g_dmn_world_guides             = NULL;
aud_esl_guides_p_t aud_g_dmn_world_overridable_guides = NULL;

/*
 * Global (shared) variables
 */

char                   *server_name     = NULL;
char                   *auditd_identity = NULL;
char                   *daemon_acl_file = NULL;
boolean32               debug_mode      = FALSE;
rpc_binding_vector_p_t  bindings;
uuid_vector_t           ns_export_uuid_vector;
char                    audit_export_name[NAME_BUF_SIZE];

static void *listener
  (
    void    *arg
  )
{
    error_status_t  st;

    CLEAR_STATUS(&st);
    dce_svc_printf(AUD_S_DMN_READY_TO_LISTEN_MSG);
    rpc_server_listen(CONCURRENCY_LEVEL, &st);
    non_dt_fatal_error = TRUE;
    pthread_cancel(distinguished_task);
    pthread_exit((void *)-1);
}


/*
 * Register auth info
 */
static void register_auth_info
  (
    error_status_t  *st
  )
{
    rpc_server_register_auth_info((unsigned_char_t *)auditd_identity, rpc_c_authn_dce_private,
                                        (rpc_auth_key_retrieval_fn_t) NULL,
                                        (void *) NULL, st);
    dce_svc_printf(AUD_S_DMN_REGISTER_AUTH_INFO_MSG, auditd_identity);
}


/*
 * Register audit_log and audit_control interfaces with runtime, and
 * export the rpc bindings to a local file.  
 */

static void register_auditd_interfaces
  (
    error_status_t  *st
  )
{
    int                     i;
    int                     fd;
    FILE                   *F; 

    fd = open(DAEMON_IDENTITY_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd > -1) {
        fchown(fd, 0, 0);
        fchmod(fd, 0644);
	write(fd, auditd_identity, strlen(auditd_identity));
	write(fd, "\n", 1);
    } else {
	dce_svc_printf(AUD_S_DMN_IDENTITY_FILE_MSG, DAEMON_IDENTITY_FILE);
    }

    /* (Re)write the bindings file */
    fd = open(DAEMON_BINDING_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd > -1) {
        /*
         * Force proper file ownership and access modes
         */
        fchown(fd, 0, 0);
        fchmod(fd, 0644);

        /* write all bindings to daemon_binding file */

         if ((F = fdopen(fd, "w")) == NULL) {
             dce_svc_printf(AUD_S_DMN_BINDING_FILE_MSG, DAEMON_BINDING_FILE);
             close(fd);
             return;
         }
         dce_bindings_to_file(F, bindings, st);
         if (BAD_STATUS(st)) {

#ifdef DCE_DEBUG
	     DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
			  "cannot write daemon string binding to a file"));
#endif
         }
         fclose(F);
    } else {
	dce_svc_printf(AUD_S_DMN_BINDING_FILE_MSG, DAEMON_BINDING_FILE);
    }

    rpc_server_register_if(audit_log_v1_0_s_ifspec, 
                  (uuid_t *) NULL, (rpc_mgr_epv_t) NULL, st);

    rpc_server_register_if(audit_control_v1_0_s_ifspec, 
                  (uuid_t *) NULL, (rpc_mgr_epv_t) NULL, st);

    rpc_server_register_if(serviceability_v1_0_s_ifspec, NULL,
        (rpc_mgr_epv_t)&dce_svc_ep, st);

    dce_svc_printf(AUD_S_DMN_REGISTER_INTERFACE_MSG);
}

static void unregister_auditd_interfaces
   (
     unsigned_char_t     *entry_name, 
     uuid_vector_t       *uuid_vector
   )
{
  error_status_t      st;


           rpc_ns_binding_unexport(rpc_c_ns_syntax_dce,
                                    (unsigned_char_t *)entry_name,
                                    serviceability_v1_0_s_ifspec,
                                    uuid_vector, &st);
            rpc_ns_binding_unexport(rpc_c_ns_syntax_dce,
                                    (unsigned_char_t *)entry_name,
                                    rdaclif_v0_0_s_ifspec,
                                    uuid_vector, &st);
            rpc_ns_binding_unexport(rpc_c_ns_syntax_dce,
                                    (unsigned_char_t *)entry_name,
                                    rdaclif_v1_0_s_ifspec,
                                    uuid_vector, &st);
            rpc_ns_binding_unexport(rpc_c_ns_syntax_dce,
                                    (unsigned_char_t *)entry_name,
                                    audit_control_v1_0_s_ifspec,
                                    uuid_vector, &st);
            rpc_ep_unregister(audit_control_v1_0_s_ifspec,
                              bindings,
                              uuid_vector,
                              &st);
            rpc_ep_unregister(serviceability_v1_0_s_ifspec,
                              bindings,
                              uuid_vector,
                              &st);
            rpc_ep_unregister(rdaclif_v1_0_s_ifspec,
                              bindings,
                              uuid_vector,
                              &st);
            rpc_ep_unregister(rdaclif_v0_0_s_ifspec,
                              bindings,
                              uuid_vector,
                              &st);

}


/* Start a thread to perform client updates when filter changes occur */ 

static void 
create_client_update_thread(
    void
)
{
    pthread_t           tid;

    if (pthread_create(&tid, 
                       pthread_attr_default, 
                       aud_update_client, 
                       NULL) == -1) {
        dce_svc_printf(AUD_S_DMN_CREATE_UPDATE_THREAD_MSG, errno);
    }
    pthread_detach(&tid);
}

/* excepttion handler routine to handle the exception */

void sig_handle()
{
   sigset_t         sigmask;
   int              signal_num;
   int              i;
   dce_aud_rec_t    ard;
   error_status_t   stp;
   unsigned_char_t *addr;

   sigemptyset(&sigmask);
   sigaddset(&sigmask, SIGINT);
   sigaddset(&sigmask, SIGTERM);

      while (signal_num = sigwait(&sigmask))
        switch (signal_num) {
             case SIGINT:
             case SIGTERM:
                    /* record the event */
                    for (i = 0; i < bindings->count; i++) {
                       if (got_login_context) {
                         rpc_binding_set_auth_info(bindings->binding_h[i],
                                                  (unsigned char *) auditd_identity,
                                                   rpc_c_protect_level_pkt_integ,
                                                   rpc_c_authn_default,
                                                   NULL,
                                                   rpc_c_authz_dce,
                                                  &stp);
                         if (stp == rpc_s_ok)
                           dce_aud_start_with_server_binding(EVT_STOP,
                                                             bindings->binding_h[i],
                                                             aud_c_evt_all_info,
                                                             aud_c_esl_cond_success,
                                                             &ard,
                                                             &stp);
                       } else {
                         rpc_binding_set_auth_info(bindings->binding_h[i],
                                                  (unsigned char *) auditd_identity,
                                                   rpc_c_authn_level_none,
                                                   rpc_c_authn_none,
                                                   NULL,
                                                   rpc_c_authz_dce,
                                                  &stp);
                         rpc_binding_to_string_binding(bindings->binding_h[i], &addr, &stp);
                         dce_aud_start_with_name(EVT_STOP,
                                                (unsigned char *) auditd_identity,
                                                 addr,
                                                 aud_c_evt_all_info,
                                                 aud_c_esl_cond_success,
                                                &ard,
                                                &stp);
                       }

                       /* write to the trail file */
                       if (ard != NULL && stp == aud_s_ok) {
                         dce_aud_commit(central_trail, ard, 0, 0, aud_c_esl_cond_success, &stp);
                       } /* endif */
                    } /* endfor */

                    /* close the trail file and stop listening */
                    if (central_trail != NULL)
                      dce_aud_close(central_trail, &stp);

                    unregister_auditd_interfaces((unsigned_char_t *)audit_export_name, &ns_export_uuid_vector);
                    exit(1);
             default:        /* should not occur */
                 abort();
      }
}


main
  (
    int     argc,
    char    *argv[]
  )
{
    error_status_t      st;
    pthread_addr_t      thread_status;
    int			i;
    pthread_t           listener_handle;
    aud_esl_type_t      esl_type;
    unsigned32		trail_size = 0;
    int                 pfd[2];
    int                 child_pid, pgid;
    char               *hostname;
    char               *locale;
    unsigned32		stostrategy = 0;
    char	       *working_dir;
    sec_login_handle_t  logincontext;
    int 		fd;
    char               *logfile ;
    char               *auditd_dir;
    sigset_t            sigmask;
    pthread_t           sig_thread_id;

    /* Set the process signal mask in the main thread. */

    sigemptyset(&sigmask);
    sigaddset(&sigmask, SIGINT);
    sigaddset(&sigmask, SIGTERM);
    sigaddset(&sigmask, SIGUSR1);
    sigaddset(&sigmask, SIGCHLD);
    sigprocmask(SIG_BLOCK, &sigmask, NULL);

    locale = setlocale(LC_ALL, "");
    dce_svc_set_progname("auditd", &st);
    dce_aud_init_svc();
    dce_svc_printf(AUD_S_DMN_LOCALE_MSG, locale);
    dce_svc_printf(AUD_S_DMN_STARTS_MSG);

    working_dir = (char *) malloc(strlen(dcelocal_path) + 
		strlen(ADMIN_DIR_SUBPATH) + 1);
    sprintf(working_dir, "%s%s", dcelocal_path, ADMIN_DIR_SUBPATH);
    if (chdir(working_dir) != 0) {
        dce_svc_printf(AUD_S_DMN_WORK_DIR_MSG, working_dir);
    }

    server_name = argv[0];

    for (i=1; i < argc; ++i) {
	if (strcmp(argv[i], "-w") == 0) {
	    dce_svc_routing((unsigned_char_t *)argv[++i], &st);
	} else if (strcmp(argv[i], "-d") == 0) {
	    dce_svc_debug_routing((unsigned_char_t *)argv[++i], &st);
	    if (st != 0) {
		printf("bad routespec\n");
		exit(1);
	    }
	} else if (strcmp(argv[i], "-a") == 0) {
	    audit = TRUE;
        } else if (strcmp(argv[i], "-t") == 0) {
            if ((i+1) == argc) {
		dce_svc_printf(AUD_S_DMN_TRAIL_NAME_MSG);
            } else {
		central_trail_name = argv[++i];
            }
        } else if (strcmp(argv[i], "-s") == 0) {
            if ((i+1) == argc) {
		dce_svc_printf(AUD_S_DMN_TRAIL_SIZE_MSG);
            } else {
		trail_size = strtoul(argv[++i], (char **)NULL, 10);
            }
        } else if (strcmp(argv[i], "-wrap") == 0) {
	    stostrategy = aud_c_trl_ss_wrap;
	} else if (strcmp(argv[i], "-D") == 0) {
	    debug_mode = TRUE;
        } else {
	    dce_svc_printf(AUD_S_DMN_USAGE_MSG, argv[i], argv[0]);
        }
    }

    if (auditd_identity == NULL) {
	if (setuid(0) == -1) {
	    dce_svc_printf(AUD_S_DMN_PRIVILEGE_MSG, argv[0]);
	}

        dce_cf_prin_name_from_host(NULL, &auditd_identity, &st);

        if (BAD_STATUS(&st)) {
	    dce_svc_printf(AUD_S_DMN_IDENTITY_MSG);
	}
    }

    /* The following code will insure that the auditd is running as machine
     * principal The current context is assumed to be the machine's
     * principal context.  dced or sec_clientd must already be running.
     * Also note that secval in dced (or sec_clientd) is responsible for
     * refreshing credentials for servers that run as machine principle so we
     * do not need to call sec_key_mmgmt_manage_key.
     */

    sec_login_get_current_context(&logincontext, &st);

    if (GOOD_STATUS(&st)) {
            /*
             * Make sure we get the real default machine context - so
             * release the context we just picked up.  (The current default
             * context may belong to an interactive user who has a DCE
             * identity established.)  By releasing the context and
             * re-obtaining the default context we are guaranteed to
             * fall back to getting the machine context (this is only
             * true because we are running as the local privileged user).
             */
            sec_login_release_context(&logincontext, &st);
    	    if (BAD_STATUS(&st))
    	    {
	        dce_svc_printf(AUD_S_DMN_CANNOT_RELEASE_CONTEXT_MSG, st);
    	    }
            sec_login_get_current_context(&logincontext, &st);
    	    if (BAD_STATUS(&st))
    	    {
	        dce_svc_printf(AUD_S_DMN_CANNOT_GET_MP_CONTEXT_MSG, st);
		exit(1);                
    	    }
    }
    else 
    {
	dce_svc_printf(AUD_S_DMN_CANNOT_GET_CONTEXT_MSG, st);
	exit(1);
    } 

    got_login_context = TRUE;

    /* initialize the esl hash tables, one for local principals and 
     * local groups, one for foreign principals and foreign groups, 
     * and one for cells. 
     */
    
    for (esl_type = aud_e_esl_princ; 
         esl_type < aud_e_esl_max;
         esl_type++) {
        aud_esl_input_file(esl_type, 0, NULL, 
                           aud_g_dmn_esl,
                           &aud_g_dmn_world_guides, 
                           &aud_g_dmn_world_overridable_guides); 
        if (esl_type <= aud_e_esl_cell_overridable)
            aud_esl_input_update_file(esl_type, 0, NULL,
                                      aud_g_dmn_esl); 
    }
    
    if (!debug_mode) {
        
	if (pipe(pfd) != 0) {
            printf("pipe failed");
            exit(1);
        }
        
        child_pid = fork();
        if (child_pid != 0) {
            char    buf;
            
            close(pfd[1]);
            read(pfd[0], &buf, sizeof(buf));
            
            exit(0);
        } else {
            
            close(pfd[0]);
            pgid = setsid();
	    close(pfd[1]);
	}
	    /* The following code is required to allow remote execution of auditd */

            /* check for malloc() before closing file descriptors
             * so malloc() failure will be logged */
            logfile  = (char *) malloc(   strlen(dcelocal_path)
                                       + strlen(AUDITD_LOGFILE) + 1);
            auditd_dir = (char *) malloc(   strlen(dcelocal_path)
                                       + strlen(AUDIT_DIRECTORY) + 1);
            if (logfile == NULL || auditd_dir == NULL)
		{
                printf("Memory could not be allocated for log file\n");
		exit(0);
		}

            if ((fd = open("/dev/null", O_RDWR,
                       S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)) != -1)
            {
                (void)dup2(fd,0);
                (void)dup2(fd,1);
                (void)dup2(fd,2);
                (void)close(fd);
            }

            /* redirect stdout, stderr to a log file */
            strcpy(logfile, dcelocal_path);
            strcat(logfile, AUDITD_LOGFILE);
            if ((fd = open(logfile,
                           O_WRONLY|O_CREAT|O_TRUNC,
                           S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)) != -1)
            {
                (void)dup2(fd,2);
                (void)dup2(fd,1);
            }
            close(fd);
            free(logfile);

            /* cd to a known location */
            strcpy(auditd_dir, dcelocal_path);
            strcat(auditd_dir, AUDIT_DIRECTORY);
            chdir(auditd_dir);
            free(auditd_dir);
    }
    
    
    if (pthread_mutex_init(&aud_g_dmn_esl_mutex, 
                           pthread_mutexattr_default)<0){
	dce_svc_printf(AUD_S_DMN_ESL_MUTEX_MSG);
    }
    if (pthread_mutex_init(&aud_g_dmn_state_mutex, 
                           pthread_mutexattr_default)<0){
	dce_svc_printf(AUD_S_DMN_STATE_MUTEX_MSG);
    }
    if (pthread_mutex_init(&aud_g_dmn_trail_mutex, 
                           pthread_mutexattr_default)<0){
	dce_svc_printf(AUD_S_DMN_TRAIL_MUTEX_MSG);
    }
    if (pthread_mutex_init(&aud_g_update_list_lock,
                           pthread_mutexattr_default) != 0) {
        dce_svc_printf(AUD_S_CLIENT_UPDT_MUTEX_INIT_FAIL_MSG);
    }
    
    if (pthread_cond_init(&aud_g_update_list_cond, 
                          pthread_condattr_default) != 0) {
        dce_svc_printf(AUD_S_CLIENT_UPDT_COND_INIT_FAIL_MSG);
    }

    distinguished_task = pthread_self();
    
    if (pthread_attr_create(&attr) == -1) {
        perror("Unable to create task attributes object.");
        return FALSE;
    }

    if (pthread_attr_setstacksize(&attr, TASK_STACK_SIZE) == -1) {
        perror("Unable to set task stack size attribute.");
        return FALSE;
    }

    /* set up our signal handling code */
    if (pthread_create(&sig_thread_id, pthread_attr_default,
                      (pthread_startroutine_t)sig_handle,
                      (pthread_addr_t)0) == -1) {
        perror("Unable to create the thread for signal handling.");
        exit(1);
    }
    
    TRY { 
        
        pthread_testcancel();
        
	/* Register all protseqs with the runtime */
        rpc_server_use_all_protseqs(CONCURRENCY_LEVEL, &st);

        if (BAD_STATUS(&st)) {
	    dce_svc_printf(AUD_S_DMN_PROTSEQ_MSG);
        }
        
        rpc_server_inq_bindings(&bindings, &st);
        if (BAD_STATUS(&st)) {
	    dce_svc_printf(AUD_S_DMN_BINDINGS_MSG);
        }
        
	/* We might want to allow the acl file name to be passed from
	 * command line.
	 */
        
        daemon_acl_file = (char *) malloc(strlen(dcelocal_path)
                                          + strlen(DEFAULT_ACL_FILE_SUBPATH) 
                                          + 1);
        sprintf(daemon_acl_file, "%s%s", dcelocal_path, 
                DEFAULT_ACL_FILE_SUBPATH);
	acl_init(daemon_acl_file, &st);
	if (BAD_STATUS(&st)) {
	    dce_svc_printf(AUD_S_DMN_ACL_MSG);
	}
        
        register_auditd_interfaces(&st);
        if (BAD_STATUS(&st)) {
	    dce_svc_printf(AUD_S_DMN_INTERFACES_MSG);
        }
        
        /* Export interfaces to the namespace */
	dce_cf_get_host_name(&hostname, &st);
        sprintf(audit_export_name, "/.:/%s/%s", hostname, AUD_DIRECTORY_NAME);

	ns_export_uuid_vector.count = 1;
        ns_export_uuid_vector.uuid[0] = &aud_g_object_uuid;
        
        /* Export acl manager interface */
	acl_register_if(audit_export_name, bindings, 
                        &ns_export_uuid_vector, &st);
	if (BAD_STATUS(&st)) {
	    dce_svc_printf(AUD_S_DMN_ACL_MGR_INTERFACE_MSG);
	}
    

        /* Register the control interface with the endpoint mapper */
	rpc_ep_register(audit_control_v1_0_s_ifspec, 
                        bindings,
                        &ns_export_uuid_vector, 
                        (unsigned_char_p_t)"Audit control interface", &st); 
	if (BAD_STATUS(&st)) {
	    dce_svc_printf(AUD_S_DMN_CONTROL_REGISTER_BINDINGS_MSG);
	}

        /* Export control interface */
        rpc_ns_binding_export(rpc_c_ns_syntax_dce, 
                              (unsigned_char_t *)audit_export_name,
                              audit_control_v1_0_s_ifspec, 
                              bindings, 
                              &ns_export_uuid_vector,
                              &st);
	if (BAD_STATUS(&st)) {
	    dce_svc_printf(AUD_S_DMN_CONTROL_EXPORT_INTERFACE_MSG);
        }


        /* Register the serviceability interface with the endpoint mapper */
	rpc_ep_register(serviceability_v1_0_s_ifspec, 
                        bindings,
                        &ns_export_uuid_vector, 
                        (unsigned_char_p_t)"AUDIT SVC", &st); 
	if (BAD_STATUS(&st)) {
	    dce_svc_printf(AUD_S_DMN_SVC_MGR_REGISTER_BINDINGS_MSG);
	}
    
	/* Export serviceability interface */
        rpc_ns_binding_export(rpc_c_ns_syntax_dce, 
                              (unsigned_char_t *)audit_export_name,
                              serviceability_v1_0_s_ifspec, 
                              bindings, 
                              &ns_export_uuid_vector,
                              &st);
	if (BAD_STATUS(&st)) {
	    dce_svc_printf(AUD_S_DMN_SVC_MGR_EXPORT_INTERFACE_MSG);
        }

    
        register_auth_info(&st);
        if (BAD_STATUS(&st)) {

           unregister_auditd_interfaces (
                             (unsigned_char_t *)audit_export_name,
                             &ns_export_uuid_vector);

	    dce_svc_printf(AUD_S_DMN_AUTH_INFO_MSG);
        }

        if (pthread_create(&listener_handle, attr, listener, NULL) == -1) {
	    dce_svc_printf(AUD_S_DMN_LISTENER_THREAD_MSG);
        } 

        /* Create a thread to handle the client updates */
        create_client_update_thread();

        if (audit)
        {
            dce_aud_open(aud_c_trl_open_write | stostrategy, 
                     (central_trail_name == NULL) ? 
                     "central_trail":central_trail_name, 
                     AUDITD_AUD_FIRST_EVT_NUMBER, AUDITD_AUD_NUM_OF_EVTS, 
                     &central_trail, &st);
        }
        else
        {
            dce_aud_open(aud_c_trl_open_write_no_filter | stostrategy, 
                     (central_trail_name == NULL) ? 
                     "central_trail":central_trail_name, 
                     AUDITD_AUD_FIRST_EVT_NUMBER, AUDITD_AUD_NUM_OF_EVTS, 
                     &central_trail, &st);
        }
	if (BAD_STATUS(&st)) {

            unregister_auditd_interfaces (
                             (unsigned_char_t *)audit_export_name,
                             &ns_export_uuid_vector);

	    dce_svc_printf(AUD_S_DMN_CANNOT_OPEN_CENTRAL_TRAIL_MSG, 
                           (central_trail_name == NULL) ? 
                           "central_trail" : central_trail_name);
	}
        if (central_trail != NULL && trail_size != 0)
                dce_aud_set_trail_size_limit(central_trail, trail_size, &st);
        
        /* Wait for thread to terminate */
        if (pthread_join(listener_handle, &thread_status) == -1) {
	    dce_svc_printf(AUD_S_DMN_THREAD_JOIN_LISTENER_MSG, server_name);
        }
        

        unregister_auditd_interfaces (
                             (unsigned_char_t *)audit_export_name,
                             &ns_export_uuid_vector);

    } 
    
    CATCH (pthread_cancel_e) {

        unregister_auditd_interfaces (
                             (unsigned_char_t *)audit_export_name,
                             &ns_export_uuid_vector);

        /* find out whether the alert is caused by a fatal error in a
         * non-distinguished thread, or by an asynchronous fault.
         */
        if ( ! non_dt_fatal_error ) {
            dce_svc_printf(AUD_S_DMN_DISTINGUISHED_THREAD_FAULT_MSG, 
                           server_name);
        } else {
            dce_svc_printf(AUD_S_DMN_NON_DISTINGUISHED_THREAD_FAULT_MSG, 
                           server_name);
        }
        
    } CATCH_ALL {

        unregister_auditd_interfaces (
                             (unsigned_char_t *)audit_export_name,
                             &ns_export_uuid_vector);

        dce_svc_printf(AUD_S_DMN_DISTINGUISHED_THREAD_EXIT_MSG, server_name);
    } ENDTRY;
    
    dce_svc_printf(AUD_S_DMN_NORMAL_EXIT_MSG, server_name);
}
