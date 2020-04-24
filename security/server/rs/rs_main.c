/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_main.c,v $
 * Revision 1.1.99.2  1996/10/15  20:55:07  arvind
 * 	CHFts20208: prototype check.
 * 	[1996/10/09  18:33 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_2/1]
 *
 * Revision 1.1.99.1  1996/10/03  15:09:40  arvind
 * 	Merge from sommerfeld_pk_kdc to aha_pk9
 * 	[1996/08/13  21:33 UTC  aha  /main/aha_pk9/1]
 * 
 * 	change svc_c_action_abort to svc_c_action_exit_bad
 * 	[1996/08/09  18:01 UTC  sommerfeld  /main/sommerfeld_pk_kdc/1]
 * 
 * Revision 1.1.96.3  1996/02/18  00:18:24  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:07:06  marty]
 * 
 * Revision 1.1.96.2  1995/12/08  17:52:42  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/8  1995/09/29  14:26 UTC  jrr
 * 	Register bypass for sec_id_* calls.
 * 
 * 	HP revision /main/HPDCE02/sommerfeld_dfsperf/1  1995/09/15  16:09 UTC  sommerfeld
 * 	Add idmap bypass.
 * 
 * 	HP revision /main/HPDCE02/7  1995/09/20  22:25 UTC  rose_l
 * 	CHFts16180:  Need usable DCEAUDITFILTERON approach for secd.
 * 	(merge to MAINLINE...)
 * 
 * 	HP revision /main/HPDCE02/rose_l_mothradev/1  1995/09/20  22:13 UTC  rose_l
 * 	CHFts16180:  Need usable DCEAUDITFILTERON approach for secd.
 * 
 * 	HP revision /main/HPDCE02/6  1995/05/17  23:49 UTC  rps
 * 	Merge WP changes
 * 
 * 	HP revision /main/DCE1_1_WP/1  1995/05/11  00:35 UTC  sommerfeld
 * 	Sanity check local realm..
 * 	[1995/05/11  00:32 UTC  sommerfeld  /main/sommerfeld_wp_idmap/1]
 * 
 * 	HP revision /main/HPDCE02/5  1995/05/10  15:40 UTC  burati
 * 	Merge fix adding -threads -lqlen and -restore_service from mb_mothra2
 * 
 * 	HP revision /main/HPDCE02/mb_mothra2/1  1995/05/09  15:27 UTC  burati
 * 	Configurable number of threads
 * 
 * 	HP revision /main/HPDCE02/4  1995/03/27  19:33 UTC  cuti
 * 	CHFts14466 (ot12799) bug fix
 * 
 * 	HP revision /main/HPDCE02/cuti_mothra_bug/3  1995/03/24  20:40 UTC  cuti
 * 	Change dummy_handle to unauth_handle for better naming
 * 
 * 	HP revision /main/HPDCE02/cuti_mothra_bug/2  1995/03/23  18:12 UTC  cuti
 * 	OT12799 (DTS 14466) Create dummy cred handle before listener thread started
 * 
 * 	HP revision /main/HPDCE02/cuti_mothra_bug/1  1995/03/14  18:47 UTC  cuti
 * 	CHFts14111: Add rs_start_sigcatch_task
 * 
 * 	HP revision /main/HPDCE02/3  1995/01/30  22:43 UTC  sommerfeld
 * 	Remove reference to hpdce_version.h or hpdfs_version.h
 * 	[1995/01/27  04:34 UTC  sommerfeld  /main/HPDCE02/sommerfeld_mothmk_2/1]
 * 
 * 	HP revision /main/HPDCE02/2  1994/11/17  18:19 UTC  mullan_s
 * 	Merge unintegrated
 * 	[1995/12/08  17:20:44  root]
 * 
 * Revision 1.1.91.1  1994/10/13  20:01:18  blazin
 * 	For CR 12493, now use dce_aud_set_local_cell_uuid, a new audit api which will
 * 	prevent the secd hang.
 * 	[1994/10/13  20:00:58  blazin]
 * 
 * 	HP revision /main/HPDCE02/1  1994/11/15  21:22 UTC  mullan_s
 * 	Merge.
 * 
 * Revision 1.1.87.12  1994/09/09  17:51:11  sekhar
 * 	[ OT 12084 ]  Log debug messages on startup and shutdown
 * 		      to create a debug log file for test purposes.
 * 	[1994/09/09  15:15:30  sekhar]
 * 
 * Revision 1.1.87.11  1994/08/25  23:18:20  sekhar
 * 	Put back SVC messages for fatal_st() and error_st() calls.
 * 	[1994/08/25  23:17:51  sekhar]
 * 
 * Revision 1.1.87.10  1994/08/25  18:59:37  sekhar
 * 	Merged with changes from 1.1.87.9
 * 	[1994/08/25  18:59:27  sekhar]
 * 
 * 	[OT 11751 ] - Sec ACL Manager for SVCIF
 * 		      ( dce_svc_register "sec" sub-component table ).
 * 	[1994/08/24  20:02:47  sekhar]
 * 
 * Revision 1.1.87.9  1994/08/25  18:39:20  kline_s
 * 	Fix OT 11700, where secd was dumping core due to out of sequence
 * 	audit calls being made.
 * 	[1994/08/25  18:35:10  kline_s]
 * 
 * Revision 1.1.87.8  1994/08/24  17:30:13  max
 * 	Fix for CR 10816.  Init (and launch) KDS before PS so that
 * 	PS finds encryption system already initialized.  Otherwise
 * 	we had race condition.  PS would at times use uninitialized
 * 	function fields causing seg fault.
 * 	[1994/08/24  17:22:04  max]
 * 
 * Revision 1.1.87.7  1994/08/12  17:37:49  mdf
 * 	     1994/08/10   sekhar
 * 	SVC: add -w command line switch for specifying routing .
 * 	     fixed incorrect sub-component name on a few messages.
 * 	[1994/08/12  14:15:48  mdf]
 * 
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:18:42  mdf]
 * 
 * 	Remove hp specific stuff.
 * 
 * 	hp_sec_to_osf_3 drop
 * 	[1994/05/18  11:55:58  mdf]
 * 
 * Revision 1.1.87.6  1994/08/04  16:13:33  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:11:31  mdf]
 * 
 * Revision 1.1.11.5  1993/12/17  21:39:05  sommerfeld
 * 	Set queue length to 300
 * 	[1993/12/17  21:38:45  sommerfeld]
 * 
 * Revision 1.1.11.4  1993/10/29  16:44:17  hanfei
 * 	work for change master : rs_login_setup_host_identity() is called for
 * 	master starup.
 * 	[1993/10/29  16:41:41  hanfei]
 * 
 * Revision 1.1.11.3  1993/10/15  18:45:58  sekhar
 * 	[CHFts08099] Make output to stdout and stderr unbuffered.
 * 	[OT 9110]
 * 
 * Revision 1.1.11.2  1993/07/08  12:48:04  root
 * 	Initial King Kong branch
 * 	[1993/07/08  12:47:27  root]
 * 
 * Revision 1.1.9.3  1993/06/17  21:24:17  rps
 * 	Included domestic vs. export in libdce rev_ctl.
 * 	[1993/06/17  17:03:19  rps]
 * 
 * Revision 1.1.9.2  1993/05/25  16:32:50  rps
 * 	standardize version string
 * 	[1993/05/25  16:22:40  rps]
 * 
 * Revision 1.1.1.12  1993/05/04  20:38:53  mgm
 * 	    Loading drop DCE1_0_2FNL
 * 
 * Revision 1.1.4.17  1993/04/21  20:58:23  sommerfeld
 * 	[7700] Add call to initialize lock over in rs_rep_auth module.
 * 	[1993/04/21  20:41:32  sommerfeld]
 * 
 * Revision 1.1.4.17  1993/04/21  20:58:23  sommerfeld
 * 	[7700] Add call to initialize lock over in rs_rep_auth module.
 * 	[1993/04/21  20:41:32  sommerfeld]
 * 
 * Revision 1.1.4.16  1993/03/19  22:20:51  emartin
 * 	correct usage message.
 * 	don't ever checkpt when catch an exception.
 * 	remove some unused code
 * 	[1993/03/19  18:57:23  emartin]
 * 
 * Revision 1.1.4.15  1993/03/08  14:38:09  ahop
 * 	7340: initialized slave calls rs_login_setup_host_identity
 * 	[1993/03/04  21:31:03  ahop]
 * 
 * Revision 1.1.4.14  1993/02/26  23:50:58  emartin
 * 	em_bl6f: do restore_master
 * 	[1993/02/22  23:02:56  emartin]
 * 
 * Revision 1.1.4.13  1993/02/19  17:13:42  ahop
 * 	dbcp: use and promote global lock in lock_mode_riw
 * 	      pass rep_twrs to db_initialize and rsdb_initialize
 * 	[1993/02/19  08:42:53  ahop]
 * 
 * Revision 1.1.4.12  1993/02/18  20:48:05  sommerfeld
 * 	[OT7276] Avoid confusing HP debugger with TRY as first statement of main()
 * 	[1993/02/18  20:05:21  sommerfeld]
 * 
 * Revision 1.1.4.11  1993/02/05  22:53:07  ahop
 * 	auth: change rs_bypass_auth to false
 * 	[1993/02/05  22:03:16  ahop]
 * 
 * Revision 1.1.4.10  1993/02/03  22:52:14  sommerfeld
 * 	[OT 6641] Add exception handler wrapper around main() so that
 * 	otherwised unhandled exceptions from fatal() don't cause a core
 * 	dump (which would be worthless since the evidence of the crash is
 * 	already gone by the time CMA induces the crash).
 * 	[1993/01/29  18:46:17  sommerfeld]
 * 
 * Revision 1.1.4.8  1993/01/26  23:07:21  emartin
 * 	decl rs_bypass_auth
 * 	[1993/01/13  16:29:08  emartin]
 * 
 * Revision 1.1.4.7  1993/01/08  22:18:06  tom
 * 	Bug 6413 - Call setlocale for correct i18n.
 * 	[1993/01/08  21:01:48  tom]
 * 
 * Revision 1.1.4.6  1992/12/29  16:35:37  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:24:20  zeliff]
 * 
 * Revision 1.1.4.5  1992/12/18  22:12:02  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:49:47  sekhar]
 * 
 * Revision 1.1.7.3  1992/12/11  21:42:04  emartin
 * 	move start_kerberos to rs_start_kerberos in rs.c
 * 	move rs_auth_init and rs_login_become_rgy to
 * 	 rs_setup_dce_rgy_identity in rs.c
 * 
 * Revision 1.1.7.2  1992/12/10  21:26:31  emartin
 * 	do rs_main_startup_after_init
 * 	start_kerberos does kdc_init and starts kerberos task
 * 
 * Revision 1.1.4.4  1992/10/07  20:33:56  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:17:16  sekhar]
 * 
 * Revision 1.1.5.2  1992/10/06  17:01:40  burati
 * 	Second replication drop: bl5
 * 	chkpt default interval set in rs_rsdb.c
 * 	move shutdown to rs_rep_mgmt.c
 * 	5037: fix crash with -rpc_debug switch
 * 
 * Revision 1.1.4.3  1992/09/29  20:12:51  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:37:14  sekhar]
 * 
 * Revision 1.1.4.2  1992/08/31  18:24:57  sekhar
 * 	First security replication code drop
 * 	[1992/08/31  15:11:43  sekhar]
 * 
 * Revision 1.1.2.4  1992/06/30  21:53:46  burati
 * 	CR4516 HP/UX porting change
 * 	[1992/06/30  20:50:21  burati]
 * 
 * Revision 1.1.2.3  1992/06/09  18:25:12  ahop
 * 	 3410: remove unavailable options from usage message
 * 	 2887: if bad usage, exit instead for returning from process_args
 * 	[1992/06/09  15:35:30  ahop]
 * 
 * Revision 1.1.2.2  1992/03/30  16:34:53  ahop
 * 	 support locksmith mode
 * 	[1992/03/28  00:00:12  ahop]
 * 
 * Revision 1.1  1992/01/19  14:44:24  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  
 * Copyright (c) Hewlett-Packard Company 1991, 1992, 1995
 * Unpublished work. All Rights Reserved.
 */
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
/* 
 *      Registry Server - mainline
 */

#include <pthread.h>

#include <stdio.h>
#include <locale.h>
#include <un_strng.h>
#include <un_io.h>   /* prototypes for unix system calls */
#include <un_time.h>
#include <sys/resource.h>

#include <dce/dce.h>
#include <dce/nbase.h>
#include <dce/uuid.h>
#include <dce/rpc.h>
#include <dce/rgynbase.h>
#include <dce/rplbase.h>

#include <rs_pvt.h>
#include <rs_state.h>
#include <rs.h>
#include <rs_ver.h>
#include <rs_base.h>
#include <rs_dacl.h>
#include <rs_auth.h>
#include <rs_login.h>
#include <rs_ns.h>
#include <rs_lock.h>
#include <rs_task.h>
#include <rs_log.h>
#include <rs_util.h>
#include <rs_audit_trail.h>
#include <rs_rep_mgmt.h>
#include <rs_rep_auth.h>
#include <rs_secidmap.h>

#include <rs_rsdb.h>
#include <rsdb.h>
#include <rsdb_pvt.h>
#include <rsdb_file.h>
#include <rsdb_pgo.h>
#include <rsdb_acct.h>
#include <rsdb_misc.h>
#include <rsdb_policy.h>
#include <sec_def_flags.h>

#include <krb5/los-proto.h>

#include <rs_proc.h>

#include <args.h>   

#include <sec_svc.h>

#if defined(DCE_AUD_POINTS)
#include <dce/audit.h>
#include <sec_aud_evts.h>

/* Define audit trail location */
static char *_sec_audit_path = NULL;
#define _SEC_AUDIT_TRAIL_PATH \
    STRING_CONCAT(_sec_audit_path, DCELOCAL_PATH, "/var/security/sec_audit_trail", "")
static char *_sec_audit_filter_envp = NULL;
#define _SEC_AUDIT_FILTER_ENV "DCEAUDITFILTERON=1"
#endif	/* defined(DCE_AUD_POINTS) */

/*
 * Prototypes for local routines
 */
PRIVATE process_args (
    int                 argc,
    char                *argv[],
    unsigned_char_p_t   *new_name_p,
    boolean32           *restore_masterf,
    boolean32           *restore_servicef,
    rs_update_seqno_t   **master_seqno_p,
    rs_update_seqno_t   *master_seqno,
    boolean32           *rs_start_kdc,
    rsdb_locksmith_info_t    
                        *locksmith_info_p,
    unsigned_char_p_t   *locksmith_name_p,
    boolean32           *prompt_for_locksmith_key,
    error_status_t      *st
);

PRIVATE void db_initialize (
    boolean32              restore_masterf,
    boolean32              restore_servicef,
    rs_update_seqno_t      *master_seqno,
    rs_replica_twr_vec_p_t rep_twrs,
    error_status_t         *st
);

PRIVATE void *start_listeners (
    void    *arg
);

/*
 * Global Variables
 */

#if defined(DCE_AUD_POINTS)
dce_aud_trail_t sec_aud_trail;
static boolean32  sec_aud_filter = true;
#endif	/* defined(DCE_AUD_POINTS) */

extern boolean32  priv_debug;

PUBLIC boolean32             rs_verbose     = false;
PUBLIC boolean32             rs_prop_debug  = false;

#ifdef RS_REP_NAUTH
PUBLIC boolean32             rs_bypass_auth = false;
#endif

static unsigned32 listener_qlen = 300;
static unsigned32 listener_threads = 5;

/*
 * Wrap main program with exception handler so we don't get unhandled
 * exception coredumps on mundane user errors.
 */
main(argc, argv)
    int argc;
    char **argv;
{
    error_status_t   st ;

    setlocale(LC_ALL, "");

    dce_svc_set_progname("secd", &st);
    /*  Register the sub-component table with svc library 
     *  but ignore the handle returned. Unless this is done
     *  the list of sub-component list cannot be obtained
     *  by for e.g. dcecp.
     *
     *  In all SVC calls, the handle sec_svc_handle ( defined
     *  statically elsewhere in the code) is used.
     */
    (void ) dce_svc_register(sec_svc_table, (idl_char *) "sec" ,
                             &st);

    TRY 
    {
	exit(rgy_main(argc, argv));
    }
    CATCH (pthread_cancel_e) 
    {
        dce_svc_printf(DCE_SVC(sec_svc_handle, "%s"),
                       sec_s_rs_main,
                       svc_c_sev_error | svc_c_action_exit_bad,
                       sec_thr_exit_cancel,
                       "main");
    } 
    CATCH (pthread_exit_thread_e) 
    {
        dce_svc_printf(DCE_SVC(sec_svc_handle, "%s"),
                       sec_s_rs_main,
                       svc_c_sev_error | svc_c_action_exit_bad,
                       sec_thr_exit_exc,
                       "main");
    }
    ENDTRY
}


/*
 * The *real* main program..
 */

int rgy_main
  (
    int     argc,
    char    *argv[]
  )
{
    error_status_t      status;
    boolean32           be_server;
    unsigned_char_p_t   new_myname;
    boolean32           restore_masterf, restore_servicef;
    rs_update_seqno_t   master_seqno,
                        *master_seqno_p;
    boolean32           rs_start_kdc;
    rsdb_locksmith_info_t    
                        locksmith_info;
    unsigned_char_p_t   locksmith_name_p;
    boolean32           prompt_for_locksmith_key;
    rpc_binding_vector_p_t  
                        rep_bindings;
    rs_replica_twr_vec_p_t  
                        rep_twrs;
    rs_task_handle_t    task_handle;
    pthread_addr_t      result;   
    char                **ap  ;    /* argument pointer */
    sec_rgy_properties_t  props;

    /*  process the -w switch - used to set the routing for SVC messages
     *  up front . This must be done before 
     *     a. any SVC messages are logged.
     *     b. before secd forks ( when run in the background ).
     *        Thus the -w switch will apply to both the parent and child
     *         process.
     */

    for ( ap = &argv[1] ; *ap ; ap++ ) {
        if (match_command("-w", *ap, 2)) {
            if ( ap[1] ) {  /* check for missing arg */
                dce_svc_routing((unsigned char *)ap[1], &status);
                if (BAD_STATUS(&status)) {
                    dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                                   sec_s_rs_main,
                                   svc_c_sev_error,
                                   status);
                    dce_svc_printf(SEC_SECD_CL_USAGE_MSG);
                }
            }
            else {
                dce_svc_printf(SEC_SECD_CL_MISSING_ARG_MSG, "-w");
                dce_svc_printf(SEC_SECD_CL_USAGE_MSG);
            }
        }
    }

    /*
     * check to see if we want the server to run in the background or if
     * we have debugging turned on and want it in the foreground for debugging.
     * This processing occurs before threads are initialized to avoid
     * problems with fork once the thread package is active.
     */
    be_server = rs_process_make_server(&argc, argv);

    rs_task_init_module();
    rs_task_init("Registry Distinguished task");

    if (setuid(0) != 0) {
        SET_STATUS(&status, sec_rgy_not_authorized);
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_main,
                       svc_c_sev_fatal | svc_c_action_exit_bad,
                       sec_rgy_not_root);
    }

    /*
     * Run the security services at a higher priority than normal processes
     */
#ifdef __hpux
    nice(-10);
#else
    setpriority(PRIO_PROCESS, getpid(), -10);
#endif

    /*
     * Increase default stack size for rpc executor threads.
     */
    rpc_mgmt_set_server_stack_size(64 * 1024, &status);
    if (BAD_STATUS(&status))
        SEC_SVC_PRINT_MSG_ST(SEC_RGY_SET_STACK_SIZE_MSG , status);

    /* init optional command line values to default values
     */
    new_myname = NULL;
    restore_masterf = restore_servicef = false;
    master_seqno_p = NULL;
    rs_start_kdc   = true;

    /* Initialize locksmith_info */
    locksmith_info.locksmith_mode = false;
    locksmith_info.remote_ok = false;
    prompt_for_locksmith_key = false;
    
    process_args(argc, argv, &new_myname, &restore_masterf, &restore_servicef,
        &master_seqno_p, &master_seqno, &rs_start_kdc, &locksmith_info,
        &locksmith_name_p, &prompt_for_locksmith_key, &status);
    if (BAD_STATUS(&status)) {  /* process_args never returns on bad st */
                                /* but if it does, then abort           */
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_main,
                       svc_c_sev_fatal | svc_c_action_exit_bad,
                       status);
    }

    TRY { 
    
        /* will init threading
         */
        pthread_testcancel();

        /* will init locks
         */
	rs_rep_auth_init();
        lock_global(lock_mode_riw);
        lock_promote_global();
        
        /* Setup to receive messages via supported protocol
         * sequences.  Get replica's current protocol towers. 
         */
        rs_server_bindings_init(&rep_bindings, &rep_twrs, &status);
        if (BAD_STATUS(&status)) 
            SEC_SVC_PRINT_MSG_ST(SEC_RGY_INIT_RPC_BIND_MSG, status);
        
        /* Set up log epvs */
        rs_log_initialize(&status);
        if (BAD_STATUS(&status)) 
            SEC_SVC_PRINT_MSG_ST(SEC_RGY_LOG_INIT_MGR_MSG, status);
        
        /* read the database into memory
         */
        dce_svc_printf(SEC_RGY_DB_INIT_MSG);
        db_initialize(restore_masterf, restore_servicef, master_seqno_p,
            rep_twrs, &status);
        if (BAD_STATUS(&status)) 
            SEC_SVC_PRINT_MSG_ST(SEC_RGY_DB_INIT_ERR_MSG, status);
        
        rs_rep_mgmt_init(rep_twrs, 
            restore_masterf, master_seqno_p, &status);
        if (BAD_STATUS(&status)) 
            SEC_SVC_PRINT_MSG_ST(SEC_RGY_REP_INIT_REPLICA_MSG, status);
        
        /* Initialize sec_acl manager */
        rs_dacl_init(&status);
        if (BAD_STATUS(&status)) 
            SEC_SVC_PRINT_MSG_ST(SEC_RGY_ACL_INIT_MSG, status);

        /* If locksmith_mode, setup locksmith account */
        if(locksmith_info.locksmith_mode) {
            rs_auth_setup_locksmith(&locksmith_info, 
                locksmith_name_p, prompt_for_locksmith_key,
                &status);
            if (BAD_STATUS(&status))
                SEC_SVC_PRINT_MSG_ST(SEC_RGY_LOCKSMITH_INIT_MSG, status);
        }
    
        /* Register general interfaces with rpc runtime
         * and endpoint mapper
         */
        rs_server_register(&status);
        if (BAD_STATUS(&status)) 
            SEC_SVC_PRINT_MSG_ST(SEC_RGY_SVR_REGISTER_MSG, status);

        /*
         * Use read lock now.
         */
        lock_demote_global();

	rs_sec_id_install_bypass(&status);
	if (BAD_STATUS(&status))
	    abort();

	/* Initialize unauth_cred_handle(a global variable) 
	 * before multithread started.
	 */

	rs_auth_init_unauth_cred_handle(&status);

        if (BAD_STATUS(&status))
            SEC_SVC_PRINT_MSG_ST(SEC_RGY_UNAUTH_CRED_HANDLE_INIT_MSG, status);


	/*
	 * Start the KDS before the PS.  This is important as the KDS
	 * initialization also sets up the cryptographic system that the
	 * PS uses.
	 */

        if (rs_start_kdc && 
            (rs_state.rep_state != rs_c_state_uninitialized)) {
            /*
             * init kdc and start kdc task
             */
            rs_start_kerberos();
        }


        /*
         * Start the listener tasks
         * rs_login_become_rgy will make call to secd
         */

        if ( ! rs_task_create(&task_handle, start_listeners,
                              RS_LISTENER_THR, true, NULL) ) {
            dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                           sec_s_rs_main,
                           svc_c_sev_fatal | svc_c_action_exit_bad,
                           sec_rs_thr_exit_creat_fail);
        }
        
        /* Setup server's identities
         */
        if (!rs_state_master() && (rs_state.rep_state == rs_c_state_uninitialized)) {
            /* An uninitialized slave must inherit the local host principal's
             * login context that is setup and maintained by sec_clientd.
             */
            rs_login_inherit_host_identity(&status);
            if (BAD_STATUS(&status))
                SEC_SVC_PRINT_MSG_ST(SEC_RGY_HOST_IDENTITY_MSG, status);
        }

#if defined(DCE_AUD_POINTS)
	/* set audit filters in the environment, unless user invocation
	 * specified full auditing.
         */
	if (sec_aud_filter) {
	    _sec_audit_filter_envp = strdup(_SEC_AUDIT_FILTER_ENV);
	    if (putenv(_sec_audit_filter_envp)) {
		SET_STATUS(&status, sec_s_no_memory);
		dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
				       sec_s_rs_main,
				       svc_c_sev_fatal | svc_c_action_exit_bad,
				       sec_rgy_cant_allocate_memory);
	    }
	}

        /* Fix for OT 11700: Move aud open call to occur after secd's
         * server bindings have been established, but before secd
         * registers its auth info with RPC and itself to the nameservice.
         */
        dce_aud_open(aud_c_trl_open_write, (char *)_SEC_AUDIT_TRAIL_PATH,
            SEC_AUD_FIRST_EVT_NUMBER, SEC_AUD_NUM_OF_EVTS, 
            &sec_aud_trail, &status);
        if (BAD_STATUS(&status)) {
           dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                          sec_s_rs_main,
                          svc_c_sev_fatal | svc_c_action_exit_bad,
                          status);
        }

        /* Fix for OT 12493: Obtain the cell uuid from the registry and
         * provide this on the side to auditd.  This is to avoid bootstrap
         * problems in auditd causing secd to hang when DCEAUDITFILTERON
         * env var is set prior to config-ing.
         */

        rsdb_properties_get_info(&props, &status);
        dce_aud_set_local_cell_uuid(props.realm_uuid, &status);

#endif	/* defined(DCE_AUD_POINTS) */

        /* Register this server's auth information with RPC runtime */
        rs_auth_init(&status);
        if (BAD_STATUS(&status))
            SEC_SVC_PRINT_MSG_ST(SEC_RGY_AUTH_INIT_MSG, status);

        if (rs_state.rep_state != rs_c_state_uninitialized) {
            /* An initialized replica should not be dependent on sec_clientd or
             * other replicas to maintain the local host principal's context.
             * Set it up and start a thread to keep it refreshed.
             */
            rs_login_setup_host_identity(&status);
            if (BAD_STATUS(&status))
                SEC_SVC_PRINT_MSG_ST(SEC_RGY_REP_HOST_IDENTITY_MSG, status);

            /* Use DCE rgy id for servicing client requests
             * and for doing client operations
             */
            rs_setup_dce_rgy_identity(&status);
            if (BAD_STATUS(&status))
                SEC_SVC_PRINT_MSG_ST(SEC_RGY_DCE_RGY_IDENTITY_MSG, status);
        }

        /*
         * Unlock global read lock
         */

        unlock_global(lock_mode_read);

        /*
         * Start the task to register with nameservice
         */
        rs_ns_server_register_start_task(&status);
        if (BAD_STATUS(&status))
            SEC_SVC_PRINT_MSG_ST(SEC_RGY_NS_REGISTER_MSG, status);
        
        /* start checkpt task
         */
        rsdb_start_checkpt_task(&status);
        if (BAD_STATUS(&status))
            SEC_SVC_PRINT_MSG_ST(SEC_RGY_CHECKPT_START_TASK_MSG, status);

	/*
	 * start the task to catch asynchronous signal 
	 */
	rs_start_sigcatch_task(&status);
        if (BAD_STATUS(&status))
            SEC_SVC_PRINT_MSG_ST(SEC_RGY_SIGCATCH_START_TASK_MSG, status);

        rs_audit_trail_log( (handle_t) NULL,  "Server Boot", "");

        rs_process_ready_to_listen();

        /*
         * Wait for listeners to terminate
         */
        dce_svc_printf(SEC_RGY_STARTUP_DONE_MSG);
        /* Log a debug message to create a debug file which is needed
         * to test system test scripts
         */
        DCE_SVC_DEBUG((sec_svc_handle, sec_s_rs_main, svc_c_debug1,
                       "Startup completed\n"));

        if ( ! rs_task_join(task_handle, &result) ) {
            dce_svc_printf(SEC_RGY_THR_JOIN_MSG);
        }
        else  {/* normal exit */
            dce_svc_printf(SEC_RGY_SHUTDOWN_DONE_MSG);
            DCE_SVC_DEBUG((sec_svc_handle, sec_s_rs_main, svc_c_debug1,
                          "Shutdown completed\n"));    /* for use by systest */
        }
    } 

    CATCH (cma_e_alerted) {
        
        /* find out whether the alert is due to a fatal error taken by some
         * other task, or the result of an asynchronous fault.
         */
        if ( ! rs_task_test_fatal() ) {
            dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                           sec_s_rs_main , svc_c_sev_error, status);
            dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                           sec_s_rs_main,
                           svc_c_sev_error,
                           sec_thr_alert);
            /* I don't think it is correct to checkpt here
             *  rs_rep_mgmt_shutdown(&status);
             */
        } 
        else {
            /****   SVC_OPEN : Where and how is status set ? ****
             *      
             */
            dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                           sec_s_rs_main , svc_c_sev_error, status);
            dce_svc_printf(SEC_RGY_THR_EXIT_ALERT_MSG);
        }
    } 

    CATCH_ALL {
        /****   SVC_OPEN : Where and how is status set ? ****
         *      This was the case before SVC changes.
         *
         */
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_main , svc_c_sev_error, status);
        dce_svc_printf(SEC_RGY_THR_EXIT_EXC_MSG);
    } 

    ENDTRY;

    exit(0);
}


PRIVATE process_args
  (
    int                 argc,
    char                *argv[],
    unsigned_char_p_t   *new_name_p,
    boolean32           *restore_masterf,
    boolean32           *restore_servicef,
    rs_update_seqno_t   **master_seqno_p,
    rs_update_seqno_t   *master_seqno,
    boolean32           *rs_start_kdc,
    rsdb_locksmith_info_t    
                        *locksmith_info_p,
    unsigned_char_p_t   *locksmith_name_p,
    boolean32           *prompt_for_locksmith_key,
    error_status_t      *st
  )
{
    int                         i; 
    int                         n;
    unsigned32                  sn1,
                                sn2;
    signed32                    chkpt_interval;
    char                        error_buf[128];
    
    for (i = 1; i < argc; i++) {
        if (match_command("-verbose", argv[i], 2)) {
            rs_verbose = true;
            rs_audit_trail_enable();
        }
        else if (match_command("-audit", argv[i], 2)) {
            rs_audit_trail_enable();
        }

        else if (match_command("-lock_debug", argv[i], 7)) {
            rs_lock_debug = true;
        }
        else if (match_command("-priv_debug", argv[i], 2)) {
            priv_debug = true;
        } 
        else if (match_command("-rpc_debug", argv[i], 3)) {
            if (++i == argc) {
                rpc__dbg_set_switches("0-4.3", st);
            } else {
                rpc__dbg_set_switches(argv[i], st);
            }
        }
        else if (match_command("-prop_debug", argv[i], 10)) {
            rs_prop_debug = true;
        }

        else if (match_command("-bootstrap", argv[i], 2)) {
            rs_ns_bootstrap_enable();
        }

        else if (match_command("-change_myname", argv[i], 14)) {
            if (++i == argc) {
                dce_svc_printf(SEC_SECD_CL_MISSING_ARG_MSG, "-change_myname");
                dce_svc_printf(SEC_SECD_CL_USAGE_MSG);
            } 

            *new_name_p = (unsigned_char_p_t)argv[i];
        }

        else if (match_command("-restore_master", argv[i], 15)) {
            *restore_masterf = true;
        }

        else if (match_command("-restore_service", argv[i], 16)) {
            *restore_servicef = true;
        }

        else if (match_command("-master_seqno", argv[i], 13)) {
            if (++i == argc) {
                dce_svc_printf(SEC_SECD_CL_MISSING_ARG_MSG, "-master_seqno");
                dce_svc_printf(SEC_SECD_CL_USAGE_MSG);
            } 

            n = sscanf(argv[i], "%lu.%lu", &sn1, &sn2);

            if (n == 1) {
                master_seqno->high = 0;
                master_seqno->low = sn1;
            }
            else 
            if (n == 2) {
                master_seqno->high = sn1;
                master_seqno->low = sn2;
            }
            else
                dce_svc_printf(SEC_SECD_CL_BAD_ARG_MSG, "-master_seqno"); /* exits */
                /* usage not reqd since this is not a usage error */

            /*
             * non-NULL master_seqno_p indicates a master_seqno
             * argument
             */
            *master_seqno_p = master_seqno;
        }

        else if (match_command("-cpi", argv[i], 4)) {
            if (++i == argc) {
                dce_svc_printf(SEC_SECD_CL_MISSING_ARG_MSG, "-cpi");
                dce_svc_printf(SEC_SECD_CL_USAGE_MSG);
            } 

            chkpt_interval = atol(argv[i]);
            if (chkpt_interval <= 0)
                dce_svc_printf(SEC_SECD_CL_BAD_CHKPT_INTERVAL_MSG); /* exits */
                /* usage not reqd since this is not a usage error */

            rsdb_set_checkpt_interval(chkpt_interval);
        }

        else if (match_command("-no_kdc", argv[i], 5)) {
            *rs_start_kdc = false;
        }

        else if (match_command("-locksmith", argv[i], 7)) {
            locksmith_info_p->locksmith_mode = true;
            if (++i == argc) {
                dce_svc_printf(SEC_SECD_CL_MISSING_ARG_MSG, "-locksmith");
                dce_svc_printf(SEC_SECD_CL_USAGE_MSG);
            }
            *locksmith_name_p = (unsigned_char_p_t)argv[i];
        }
        else if (match_command("-lockpw", argv[i], 7)) {
            if(!locksmith_info_p->locksmith_mode) {
                dce_svc_printf(SEC_SECD_CL_LOCKSMITH_OPT_MSG,argv[i]);
                dce_svc_printf(SEC_SECD_CL_USAGE_MSG);
            }
            *prompt_for_locksmith_key = true;
        }
        else if (match_command("-remote", argv[i], 4)) {
            if(!locksmith_info_p->locksmith_mode) {
                dce_svc_printf(SEC_SECD_CL_LOCKSMITH_OPT_MSG,argv[i]);
                dce_svc_printf(SEC_SECD_CL_USAGE_MSG);
            }
            locksmith_info_p->remote_ok = true;
        }

        else if (match_command("-w", argv[i], 2)) {
              ++i ; /* skip the next arg - guranteed to be
                     * there since this is never reached on error */
        }

        else if (match_command("-threads", argv[i], 8)) {
            if (++i == argc) {
                dce_svc_printf(SEC_SECD_CL_MISSING_ARG_MSG, "-threads");
                dce_svc_printf(SEC_SECD_CL_USAGE_MSG);
            } 

            listener_threads = atoi(argv[i]);
        }

        else if (match_command("-lqlen", argv[i], 6)) {
            if (++i == argc) {
                dce_svc_printf(SEC_SECD_CL_MISSING_ARG_MSG, "-lqlen");
                dce_svc_printf(SEC_SECD_CL_USAGE_MSG);
            } 

            listener_qlen = atoi(argv[i]);
        }

#if defined(DCE_AUD_POINTS)
        else if (match_command("-noaudfilter", argv[i], 12)) {
            sec_aud_filter = false;
        }
#endif

        else {
            dce_svc_printf(SEC_SECD_CL_UNKNOWN_OPT_MSG, argv[i]);
            dce_svc_printf(SEC_SECD_CL_USAGE_MSG);
        }
    }

    CLEAR_STATUS(st);
}


PRIVATE void db_initialize
  (
    boolean32               restore_masterf,
    boolean32               restore_servicef,
    rs_update_seqno_t       *master_seqno,
    rs_replica_twr_vec_p_t  rep_twrs,
    error_status_t          *st
  )
{
    CLEAR_STATUS(st);

    rsdb_initialize(rep_twrs, st);
    if (BAD_STATUS(st)) return;
    rsdb_util_check_cell_name(st);
    if (BAD_STATUS(st)) return;

    /* If restoring master and this is the master and it
     * is in maintenance state, put the server into
     * state in_service.  The database is probably being
     * restored from backup.
     * OR, if explicitly asked to set state svc, then do so.
     */
    if ((rs_state.rep_state == rs_c_state_in_maintenance) &&
	((restore_masterf && rs_state_master()) || (restore_servicef)) ) {

        rs_state_set_state(rs_c_state_in_service, st);
    }

    /*
     * Error exit if in dupe master state and a
     * new master seqno has not been specified on
     * the command line
     */
    if ((rs_state.rep_state == rs_c_state_dup_master) &&
        (master_seqno == NULL)) {
        SET_STATUS(st, sec_rgy_rep_master_dup);
        return;
    }

    if (rs_state.rep_state == rs_c_state_in_maintenance) {
        rsdb_close_files();
    }
}


PRIVATE void *start_listeners
  (
    void    *arg
  )
{
    error_status_t   status;
   
    dce_svc_printf(SEC_RS_THR_STARTED_MSG,
                   RS_LISTENER_THR);

    rpc_server_set_thread_pool_qlen(NULL, listener_qlen, &status);
    if (BAD_STATUS(&status))
        SEC_SVC_PRINT_MSG_ST(SEC_RGY_THR_SET_POOL_MSG, status);
    
    rpc_server_listen(listener_threads, &status);  
    if (BAD_STATUS(&status)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_main,
                       svc_c_sev_error,
                       status);
    }
    dce_svc_printf(SEC_RS_THR_EXITING_MSG,
                   RS_LISTENER_THR);
}
