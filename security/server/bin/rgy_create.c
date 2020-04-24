/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rgy_create.c,v $
 * Revision 1.1.86.3  1996/02/18  00:16:29  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:05:35  marty]
 *
 * Revision 1.1.86.2  1995/12/08  17:46:38  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/7  1995/08/24  17:09 UTC  sommerfeld
 * 	Use correct entry for master name.
 * 	[1995/08/11  20:17 UTC  sommerfeld  /main/HPDCE02/sommerfeld_mothpatch/2]
 * 
 * 	CHFts15379
 * 	Include replica name of server in the pe_site file line, just like
 * 	pe_update.c does.
 * 	Not completely there yet -- gets master wrong on replica creation..
 * 	[1995/08/07  19:34 UTC  sommerfeld  /main/HPDCE02/sommerfeld_mothpatch/1]
 * 
 * 	HP revision /main/HPDCE02/6  1995/05/15  21:04 UTC  greg
 * 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
 * 	[1995/04/22  13:37 UTC  greg  /main/HPDCE02/SEC_migrate_merge/1]
 * 
 * 	Remove reference to hpdce_version.h or hpdfs_version.h
 * 	[1995/01/27  04:34 UTC  sommerfeld  /main/HPDCE02/sommerfeld_mothmk_2/1]
 * 
 * 	Add fault_status.
 * 	[1995/01/19  15:41 UTC  cuti  /main/HPDCE02/4]
 * 
 * 	Use bad exit instead of abort exit.
 * 	[1994/12/09  22:03 UTC  cuti  /main/HPDCE02/3]
 * 
 * 	fix a bus error when a program area pointer ( keyseed) is modified.
 * 	[1994/12/09  16:02 UTC  hanfei  /main/HPDCE02/2]
 * 
 * 	Merge work from greg_sec_migrate_acls branch.
 * 	[1995/04/18  18:18 UTC  greg  /main/HPDCE02/SEC_migrate/6]
 * 
 * 	Configure acl manager version before invoking
 * 	databse code that creates deafult acls.
 * 	[1995/04/13  17:09 UTC  greg  /main/HPDCE02/SEC_migrate/greg_sec_migrate_acls/1]
 * 
 * 	Add current software version to calls
 * 	to rs_resv_gen_uninit_db()
 * 	[1995/03/06  14:50 UTC  greg  /main/HPDCE02/SEC_migrate/5]
 * 
 * 	Use rs_resv_create_uninit_db()  to create necessary
 * 	reserved entries when creating a slave database.
 * 	[1995/03/03  21:30 UTC  greg  /main/HPDCE02/SEC_migrate/4]
 * 
 * 	include rs_ver.h.
 * 	[1995/02/11  22:14 UTC  greg  /main/HPDCE02/SEC_migrate/3]
 * 
 * 	fix compilatin errors.
 * 	[1995/01/29  14:24 UTC  greg  /main/HPDCE02/SEC_migrate/2]
 * 
 * 	Create initial SEC_migrate mainline.
 * 	Compiled but not tested.
 * 	[1995/01/28  17:10 UTC  greg  /main/HPDCE02/SEC_migrate/1]
 * 
 * 	add global_compat_sw_rev to master info when creating a replica.
 * 	[1995/01/22  20:22 UTC  greg  /main/HPDCE02/greg_migration2/1]
 * 
 * 	HP revision /main/HPDCE02/5  1995/01/30  22:43 UTC  sommerfeld
 * 	Remove reference to hpdce_version.h or hpdfs_version.h
 * 	[1995/01/27  04:34 UTC  sommerfeld  /main/HPDCE02/sommerfeld_mothmk_2/1]
 * 
 * 	HP revision /main/HPDCE02/4  1995/01/19  15:41 UTC  cuti
 * 	Add fault_status.
 * 
 * 	HP revision /main/HPDCE02/3  1994/12/09  22:03 UTC  cuti
 * 	Use bad exit instead of abort exit.
 * 
 * 	HP revision /main/HPDCE02/2  1994/12/09  16:02 UTC  hanfei
 * 	fix a bus error when a program area pointer ( keyseed) is modified.
 * 
 * 	HP revision /main/HPDCE02/1  1994/11/15  21:22 UTC  mullan_s
 * 	Merge.
 * 	[1995/12/08  17:19:13  root]
 * 
 * Revision 1.1.79.5  1994/08/25  18:30:01  sekhar
 * 	Put back fatal_st() and error_st() messages.
 * 	[1994/08/25  18:29:33  sekhar]
 * 
 * Revision 1.1.79.4  1994/08/12  17:37:42  mdf
 * 	Hewlett Packard Security Drop
 * 	[1994/08/12  14:15:32  mdf]
 * 
 * Revision 1.1.79.3  1994/08/04  16:13:11  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:10:29  mdf]
 * 
 * Revision 1.1.79.1  1994/04/01  21:37:58  kline_s
 * 	[OT 10264] First audit code point drop for security.
 * 	[1994/04/01  21:36:44  kline_s]
 * 
 * Revision 1.1.77.1  1993/10/05  22:31:01  mccann
 * 	CR8651 64 bit porting changes
 * 	[1993/10/04  19:13:02  mccann]
 * 
 * Revision 1.1.5.16  1993/04/06  21:21:44  sommerfeld
 * 	[7655] fix typo in error message.
 * 	[1993/04/06  17:01:22  sommerfe]
 * 
 * 	[7653] Call rs_task_init_module
 * 	[1993/04/06  15:19:05  sommerfe]
 * 
 * 	[7653] Call rs_task_module_init
 * 	[1993/04/06  15:09:26  sommerfe]
 * 
 * Revision 1.1.5.15  1993/03/31  21:01:17  cuti
 * 	Add sec_salvage_db test's special switch and path_name.
 * 	[1993/03/25  22:17:39  cuti]
 * 
 * 	CR7340 Add slave's bindings to pe_site config file via new rtn
 * 	append_rgy_config_file called from create_slave(), and moved
 * 	common code out of create_rgy_config_file into write_rgy_config_file
 * 	so both create and append could use the same code...
 * 	[1993/03/16  18:40:54  burati]
 * 
 * Revision 1.1.5.13  1993/03/04  20:55:51  emartin
 * 	create_slave - register in nameservice after successfully add
 * 	replica to master's replist.
 * 	[1993/03/04  20:16:31  emartin]
 * 
 * Revision 1.1.5.12  1993/02/18  20:47:39  sommerfeld
 * 	[OT7276] avoid confusing HP debugger with TRY as first statment of main.
 * 	[1993/02/18  20:04:32  sommerfeld]
 * 
 * Revision 1.1.5.11  1993/02/10  20:34:32  emartin
 * 	bl6e: remove client agent workaround hacks
 * 	[1993/02/04  23:30:45  emartin]
 * 
 * 	bl6e: use standard client interfaces (eg. sec_rgy_site_bind_update,
 * 	 sec_rgy_properties_get_info for operations on master)
 * 	[1993/02/04  20:28:03  emartin]
 * 
 * Revision 1.1.5.10  1993/02/03  22:52:33  sommerfeld
 * 	Clean up error message in prev. fix.
 * 	Add exception handler wrapper around main() so that otherwised unhandled
 * 	exceptions from fatal() don't cause a core dump.
 * 	[1993/01/29  18:45:48  sommerfeld]
 * 
 * 	[OT6641] Reject `@' in cell name.
 * 	[1993/01/29  17:07:53  sommerfeld]
 * 
 * Revision 1.1.5.9  1993/01/26  23:01:28  emartin
 * 	bl6d: Slave must get local cell uuid from master before db_create
 * 	[1993/01/08  16:29:00  emartin]
 * 
 * Revision 1.1.5.8  1993/01/08  22:17:57  tom
 * 	Bug 6413 - Call setlocale for proper i18n.
 * 	Also unconditionally include unistd.h.
 * 	[1993/01/08  21:01:41  tom]
 * 
 * Revision 1.1.5.7  1992/12/29  16:33:36  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:21:30  zeliff]
 * 
 * Revision 1.1.5.6  1992/11/20  15:52:08  ahop
 * 	Slave's rgy_creator is a dummy; make it nil.
 * 	[1992/11/16  16:38:11  ahop]
 * 
 * 	Call rs_mkey_create after calling rsdb_create; set mkey_base_path.
 * 	[1992/11/10  23:07:34  ahop]
 * 
 * Revision 1.1.5.5  1992/11/04  19:33:46  ahop
 * 	Third replciation drop: bl6a
 * 	 clear argv's copy of keyseed, add master to a slave's replica list.
 * 	 Incorporate changed master_key operations
 * 	[1992/11/03  18:25:57  ahop]
 * 
 * Revision 1.1.5.4  1992/10/07  20:29:54  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:14:06  sekhar]
 * 
 * Revision 1.1.6.2  1992/10/06  15:52:08  burati
 * 	Second replication drop: bl5
 * 	Incorporate changed master_key operations
 * 
 * Revision 1.1.5.3  1992/09/29  20:12:28  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:36:15  sekhar]
 * 
 * Revision 1.1.5.2  1992/08/31  18:22:31  sekhar
 * 	First security replication code drop
 * 	[1992/08/31  15:10:50  sekhar]
 * 
 * Revision 1.1.3.3  1992/05/27  19:17:32  rsalz
 * 	 Remove DCE_1_1_RPC #ifdef; it is always on now
 * 	[1992/05/27  19:15:54  rsalz]
 * 
 * Revision 1.1.3.2  1992/05/22  19:15:03  wei_hu
 * 	Define DCE_1_1_RPC so that the code would use all available protocol
 * 	sequences instead of just datagram.
 * 	[1992/05/22  19:14:00  wei_hu]
 * 
 * Revision 1.1  1992/01/19  14:45:10  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
 * Copyright (c) Hewlett-Packard Company 1991, 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 *      Registry - Create an (almost) empty database
 */
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/param.h>
#include <locale.h>

#include <un_io.h>

#include <dce/dce.h>

#include <rgymacro.h>

#include <dce/rpc.h>
#include <dce/uuid.h>
#include <dce/rgybase.h>
#include <dce/dce_cf.h>
#include <dce/sec_login.h>

/* this stuff is here to get wrappers if necessary */
#include <pthread.h>
#ifdef CMA_INCLUDE
#   include <dce/cma_stdio.h>
#endif

#include <args.h>

#include <dce/rplbase.h>
#include <dce/policy.h>
#include <dce/repadm.h>
#include <dce/replist.h>

#include <rsecidmap.h>

#include <rss_errors.h>
#include <rs_pvt.h>
#include <rs_ver.h>
#include <rs_ver_pvt.h>
#include <rs_rep_pvt.h>
#include <rsdb.h>
#include <rsdb_create.h>
#include <rsdb_misc.h>
#include <rsdb_file.h>
#include <rsdb_replica.h>
#include <rca_fileloc.h>    /* must appear after rsdb.h */
#include <rsdb_pvt.h>       /* for rgy_file_directory */
#include <rs_reserved.h>

#include <rs.h>
#include <rs_state.h>
#include <rs_master.h>
#include <rs_log.h>
#include <rs_ns.h>
#include <rs_rep_bind.h>
#include <rs_rep_util.h>
#include <rs_login.h>
#include <rs_mkey.h>
#include <dce/sec_daclmgr.h>
#include <rs_ver.h>
#include <rs_ver_pvt.h>


#include <sec_svc.h>

#if defined(DCE_AUD_POINTS)
#include <dce/audit.h>
#include <sec_aud_evts.h>

/* Define audit trail location */
static char *_sec_audit_path = NULL;
#define _SEC_AUDIT_TRAIL_PATH \
    STRING_CONCAT(_sec_audit_path, DCELOCAL_PATH, "/var/security/sec_audit_trail", "")
#endif	/* defined(DCE_AUD_POINTS) */

#ifdef RS_REP_NAUTH
PUBLIC boolean32    rs_bypass_auth = false;
#endif

/* Private routines */

PRIVATE void create_master (
    int                     argc,
    char                    *argv[],
    sec_id_t                *rgy_local_cell,
    uuid_p_t                rep_id,
    rpc_binding_vector_p_t  rep_bindings,
    rs_replica_twr_vec_p_t  rep_twrs,
    error_status_t          *st
);

PRIVATE void create_slave (
    int                     argc,
    char                    *argv[],
    sec_id_t                *rgy_local_cell,
    uuid_p_t                rep_id,
    rpc_binding_vector_p_t  rep_bindings,
    rs_replica_twr_vec_p_t  rep_twrs,
    error_status_t          *st
);

PRIVATE void create_slave (
    int                     argc,
    char                    *argv[],
    sec_id_t                *rgy_local_cell,
    uuid_p_t                rep_id,
    rpc_binding_vector_p_t  rep_bindings,
    rs_replica_twr_vec_p_t  rep_twrs,
    error_status_t          *st
);

PRIVATE void create_master_db (
    uuid_p_t                rep_id,
    rs_replica_twr_vec_p_t  rep_twrs,
    unsigned_char_p_t       rep_name,
    sec_id_t                *rgy_creator,
    long                    rgy_creator_unix_id,
    sec_id_t                *rgy_local_cell,
    unsigned_char_p_t       rgy_hostname,
    unsigned_char_p_t       rgy_password,
    sec_rgy_properties_t    *default_properties,
    char                    *keyseed,
    unsigned32              master_compat_software_rev,
    error_status_t          *st
);

PRIVATE void create_slave_db (
    uuid_p_t                cell_sec_id,
    uuid_p_t                rep_id,
    rs_replica_twr_vec_p_t  rep_twrs,
    unsigned_char_p_t       rep_name,
    rs_replica_item_t       *master_item,
    rs_update_seqno_t       *master_seqno,
    sec_id_t                *rgy_creator,
    sec_id_t                *rgy_local_cell,
    sec_rgy_properties_t    *default_properties,
    char                    *keyseed,
    unsigned32              master_compat_software_rev,
    error_status_t          *st
);

PRIVATE void bind_to_master (
    unsigned_char_p_t       bind_site,
    sec_rgy_handle_t        *master_handle,
    error_status_t          *st
);

PRIVATE void get_master_info (
    sec_rgy_handle_t        master_handle,
    uuid_t                  *cell_sec_id,
    rs_update_seqno_t       *master_seqno,
    unsigned32              *master_compat_sw_rev,
    rs_replica_item_t       *master_item,
    error_status_t          *st
);

PRIVATE void process_args_master_or_slave (
    int             argc,
    char            *argv[],
    boolean32       *master
);

PRIVATE void process_args_master (
    int                     argc,
    char                    *argv[],
    unsigned_char_p_t       *rgy_myname,
    sec_id_t                *rgy_creator,
    long                    *rgy_creator_unix_id,
    char                    **keyseed,
    unsigned_char_p_t       *rgy_password,
    unsigned_char_p_t       *cell_uuid_str,
    sec_rgy_properties_t    *properties
);

PRIVATE void process_args_slave (
    int                     argc,
    char                    *argv[],
    unsigned_char_p_t       *rgy_myname,
    char                    **keyseed,
    unsigned_char_p_t       *master_str
);

PRIVATE boolean32 process_name (
    char    *cell_name,
    char    **name
);

PRIVATE void create_rgy_config_file (
    unsigned_char_p_t       cell_name,
    uuid_p_t                cell_sec_id,
    rpc_binding_vector_p_t  rep_bindings,
    unsigned_char_p_t       rep_name,
    boolean32               masterf,
    error_status_t          *st
);

PRIVATE void append_rgy_config_file (
    unsigned_char_p_t       cell_name,
    uuid_p_t                cell_sec_id,
    rpc_binding_vector_p_t  rep_bindings,
    unsigned_char_p_t       rep_name,
    error_status_t          *st
);

PRIVATE void write_rgy_config_file (
    int                     fd,
    unsigned_char_p_t       cell_name,
    uuid_p_t                cell_sec_id,
    rpc_binding_vector_p_t  bindings,
    unsigned_char_p_t       rep_name,    
    error_status_t          *st
);

PUBLIC boolean32    rs_verbose = false;

#if defined(DCE_AUD_POINTS)
dce_aud_trail_t sec_aud_trail;
#endif	/* defined(DCE_AUD_POINTS) */

/* Private variable for sec_salvage_db test */
char                            *alt_dir=NULL;
/* NULL indicates sec_create_db, other indicates sec_salvage_db test */
#define DEFAULT_PATH "/tmp/sec_salvage_db_test"
#define DEFAULT_KEYSEED "abcdefg1hijklmnop2qrstuv3wxyz"


main(argc, argv)
    int argc;
    char **argv;
{
    error_status_t     st ;

    setlocale(LC_ALL, "");

    dce_svc_set_progname("sec_create_db", &st);

    /* make sure threading is enabled in case invoke
     * something which is threaded (eg. cds)
     */
    pthread_testcancel();

    TRY 
    {
	return rgy_main(argc, argv);
    }
    CATCH (pthread_cancel_e) 
    {
        dce_svc_printf(DCE_SVC(sec_svc_handle, "%s"),
                       sec_s_rs_create_db,
                       svc_c_sev_error | svc_c_action_exit_bad,
                       sec_thr_exit_cancel,
                       "main");
    } 
    CATCH (pthread_exit_thread_e) 
    {
        dce_svc_printf(DCE_SVC(sec_svc_handle, "%s"),
                       sec_s_rs_create_db,
                       svc_c_sev_error | svc_c_action_exit_bad,
                       sec_thr_exit_exc,
                       "main");
    }
    ENDTRY;
    return 1;	
}


int rgy_main (
    int     argc,
    char   *argv[]
)
{
    error_status_t          st;
    sec_id_t                rgy_local_cell;
    uuid_t                  rep_id;
    rpc_binding_vector_p_t  rep_bindings;
    rs_replica_twr_vec_p_t  rep_twrs;
    boolean32               masterf;

    /*
     * initialize our threading wrappers (sigh).
     */
    rs_task_init_module();
    
    /* must run as root
     */
    if (setuid(0) == -1)
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                      sec_s_rs_create_db,
                      svc_c_sev_fatal | svc_c_action_exit_bad,
                      sec_rgy_not_root);

    /* creating master or slave?
     */
    process_args_master_or_slave(argc, argv, &masterf);

    /* Retrieve cell name; it will be needed to process myname and hostname 
     */
    dce_cf_get_cell_name((char **)&rgy_local_cell.name, &st);
    if(BAD_STATUS(&st)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_create_db,
                       svc_c_sev_fatal | svc_c_action_exit_bad,
                       st);
    }
    
    if (u_strchr (rgy_local_cell.name, '@') != NULL) 
    {
        dce_svc_printf(SEC_CRDB_AT_CHAR_IN_CELLNAME_MSG,
                       rgy_local_cell.name);
    }

    /* get new replica's instance uuid
     */
    uuid_create(&rep_id, &st);

    /* get replica's network addresses (in binding vector and tower vector)
     */
    rs_server_bindings_init(&rep_bindings, &rep_twrs, &st);
    if(BAD_STATUS(&st)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_create_db,
                       svc_c_sev_fatal | svc_c_action_exit_bad,
                       st);                       
    }

#if defined(DCE_AUD_POINTS)
    dce_aud_open(aud_c_trl_open_write, (char *)_SEC_AUDIT_TRAIL_PATH,
        SEC_AUD_FIRST_EVT_NUMBER, SEC_AUD_NUM_OF_EVTS, 
        &sec_aud_trail, &st);
    if (BAD_STATUS(&st)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_create_db,
                       svc_c_sev_fatal | svc_c_action_exit_bad,
                       st);
    }
#endif	/* defined(DCE_AUD_POINTS) */

    /* 
     * Do the real work of creating a master or slave 
     * database
     */
    if (masterf) {
        create_master(argc, argv, 
            &rgy_local_cell, &rep_id, rep_bindings, rep_twrs, &st);
    }
    else {
        create_slave(argc, argv, 
            &rgy_local_cell, &rep_id, rep_bindings, rep_twrs, &st);
    }
    if(BAD_STATUS(&st)) {  /* should never return if status is bad */
                           /* but if it does then abort            */
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_create_db,
                       svc_c_sev_fatal | svc_c_action_exit_bad,
                       st);
    }
    rsdb_create_shutdown();

    rs_mkey_purge();

    return 0;
}

PRIVATE void create_master (
    int                     argc,
    char                    *argv[],
    sec_id_t                *rgy_local_cell,
    uuid_p_t                rep_id,
    rpc_binding_vector_p_t  rep_bindings,
    rs_replica_twr_vec_p_t  rep_twrs,
    error_status_t          *st
)
{
    unsigned_char_p_t       rep_name;
    char                    *keyseed;
    sec_id_t                rgy_creator;
    long                    rgy_creator_unix_id;
    sec_rgy_properties_t    default_properties;
    unsigned_char_p_t       rgy_password;
    unsigned_char_p_t       cell_uuid_str;
    unsigned_char_p_t       rgy_hostname;
    unsigned_char_p_t       old_rgy_hostname;
    error_status_t          tmp_st;
    sec_acl_mgr_handle_t    sec_acl_mgr_handle;

    rep_name = NULL;
    keyseed = NULL;

    /* Set rgy_info creation values to defaults if not input 
     * by administrator 
     */
    rgy_creator.name = (unsigned_char_t *)rs_default_creator;
    rgy_creator_unix_id = rgyc_uxid_creator;
    rgy_password = (unsigned_char_t *)rs_default_password;
    cell_uuid_str = NULL;
    rsdb_create_set_default_properties(&default_properties);

    /* read and check command line arguments
     */
    process_args_master(argc, argv, &rep_name, 
        &rgy_creator, &rgy_creator_unix_id, &keyseed,
        &rgy_password, &cell_uuid_str, &default_properties);

    /* conj up replica's name
     */
    if (!process_name((char *)(rgy_local_cell->name), (char **)&rep_name)) {
        dce_svc_printf(SEC_CRDB_CL_BAD_NAME_MSG, rep_name);
    }

    /* Retrieve host name 
     */
    dce_cf_get_host_name((char **)&rgy_hostname, st);
    if(BAD_STATUS(st))
        SEC_SVC_PRINT_MSG_ST(SEC_CRDB_CANT_GET_HOSTNAME_MSG, *st);

    if(!process_name((char *)(rgy_local_cell->name), (char **)&rgy_hostname)) {
        dce_svc_printf(SEC_CRDB_CL_BAD_NAME_MSG, rgy_hostname);
    }

    /* Flesh out the sec_id_t uuid for local cell's TGS. 
     */
    if (cell_uuid_str != NULL) {
        uuid_from_string(cell_uuid_str, &rgy_local_cell->uuid, st);
    } 
    else {
        rsdb_uuid_create(sec_rgy_domain_person, false, rgyc_uxid_krbtgt,
                         &rgy_local_cell->uuid, st);
    }
    if (BAD_STATUS(st))
         SEC_SVC_PRINT_MSG_ST(SEC_CRDB_CANT_CREATE_CELLUUID_MSG, *st);

    dce_svc_printf(SEC_CRDB_CR_MASTER_DB_MSG, 
                   rgy_local_cell->name);

    rs_setup_creator(&rgy_creator, rgy_creator_unix_id, st);
    if (BAD_STATUS(st))
        SEC_SVC_PRINT_MSG_ST(SEC_CRDB_CANT_SETUP_RGYCREATOR_MSG, *st);

    old_rgy_hostname = rgy_hostname;
    dce_cf_prin_name_from_host((char *)old_rgy_hostname, (char **)&rgy_hostname, st);
    if (BAD_STATUS(st))
        SEC_SVC_PRINT_MSG_ST(SEC_CRDB_CANT_GET_HOST_PRNAME_MSG, *st);

    /*
     * Create master replica's database and
     * fill it with basic info.  We create the master at the
     * maximum supported software version.  We don't see any
     * reason to allow master creation at less than the maximum,
     * but that could be added without too much difficulty.
     * Need to configure the acl manager so the database creation
     * code will know what version of acls to create.
     */

    /* Our version of sec_acl_mgr_configure always returns error_status_ok */
    sec_acl_mgr_configure((sec_acl_mgr_config_t)NULL,
			  rs_c_max_software_vers,
			  (unsigned_char_p_t) NULL,
			  &sec_acl_mgr_handle,
			  st);

    create_master_db(rep_id, rep_twrs, rep_name,
        &rgy_creator, rgy_creator_unix_id, 
        rgy_local_cell, rgy_hostname, rgy_password,
        &default_properties, keyseed, rs_c_max_software_vers, st);
    if (BAD_STATUS(st))
        SEC_SVC_PRINT_MSG_ST(SEC_CRDB_DB_NOT_CREATED_MSG, *st)
    else
        dce_svc_printf(SEC_CRDB_CR_DB_SUCC_MSG) ;

    /*
     * Setup the rgy config file (ie. pe_site file)
     */
    if (alt_dir == NULL) { /* sec_create_db */
        create_rgy_config_file(rgy_local_cell->name, 
            &rs_state.cell_sec_id, rep_bindings, rep_name,
			       true, &tmp_st);
        
        if (BAD_STATUS(&tmp_st))
            dce_svc_printf(SEC_CRDB_SITE_FILE_CREATE_FAIL_MSG);
        else
            dce_svc_printf(SEC_CRDB_SITE_FILE_CREATE_SUCC_MSG);
    }
}

PRIVATE void create_slave (
    int                     argc,
    char                    *argv[],
    sec_id_t                *rgy_local_cell,
    uuid_p_t                rep_id,
    rpc_binding_vector_p_t  rep_bindings,
    rs_replica_twr_vec_p_t  rep_twrs,
    error_status_t          *st
)
{
    unsigned_char_p_t       rep_name;
    unsigned_char_p_t       full_rep_name;
    char                    *keyseed;
    unsigned_char_p_t       master_str;
    sec_id_t                rgy_creator;
    sec_rgy_properties_t    default_properties;
    sec_rgy_properties_t    dummy_properties;
    unsigned_char_p_t       bind_site;
    sec_rgy_handle_t        master_handle;
    uuid_t                  cell_sec_id;
    rs_replica_item_t       master_rep_item;
    rs_update_seqno_t       master_seqno;
    boolean32               rep_in_ns; 
    unsigned_char_p_t       string_binding;
    rpc_binding_vector_t    master_binding;
    unsigned32              master_compat_sw_rev;
    error_status_t          tmp_st;

    /* admin specified info
     */
    rep_name = NULL;
    keyseed = NULL;
    master_str = NULL;

    /* use default or nil info until real stuff is
     * propagated from master
     */
    uuid_create_nil(&rgy_creator.uuid, st);
    rgy_creator.name = 0;

    rsdb_create_set_default_properties(&default_properties);

    /* read and check command line arguments
     */
    process_args_slave(argc, argv, &rep_name, &keyseed, 
        &master_str);

    /* conj up replica's name
     */
    if (!process_name((char *)(rgy_local_cell->name), (char **)&rep_name))
        dce_svc_printf(SEC_CRDB_CL_BAD_NAME_MSG, rep_name);

    /*
     * get authenticated handle to master
     */ 
    bind_site = (master_str == NULL) ? rgy_local_cell->name : master_str;
    bind_to_master(bind_site, &master_handle, st); /* never returns if status is bad  */
    if(BAD_STATUS(st)) return;                     /* but if it ever does then return */

    get_master_info(master_handle,
        &cell_sec_id, &master_seqno, &master_compat_sw_rev,
	&master_rep_item, st); /* never returns if status is bad  */
    if(BAD_STATUS(st)) return;                              /* but if it ever does then return */

    /*
     * Check that the compat_sw_rev.  If we can't operate at the requested revision, 
     * then we just bail out.
     */
    if (master_compat_sw_rev < rs_c_min_software_vers
    || master_compat_sw_rev > rs_c_max_software_vers) {
	SET_STATUS(st, sec_rgy_rep_sw_rev_nosup);
        return;
    }

    /* Get the local cell uuid from the master */
    sec_rgy_properties_get_info(master_handle, 
                                &dummy_properties, st);
    if(BAD_STATUS(st)) { 
        dce_svc_printf(DCE_SVC(sec_svc_handle, "") ,
                       sec_s_rs_create_db , 
                       svc_c_sev_fatal | svc_c_action_exit_bad,
                       *st);
    }

    rgy_local_cell->uuid = dummy_properties.realm_uuid;

    dce_svc_printf(SEC_CRDB_CR_REP_DB_MSG , rgy_local_cell->name);

    /* create the replica's stub database
     */
    create_slave_db(&cell_sec_id, rep_id, rep_twrs, rep_name,
        &master_rep_item, &master_seqno,
        &rgy_creator, rgy_local_cell, 
        &default_properties, keyseed, master_compat_sw_rev, st);
    if (BAD_STATUS(st))
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_create_db,
                       svc_c_sev_fatal | svc_c_action_exit_bad,
                       *st);
    else
        dce_svc_printf(SEC_CRDB_CR_DB_SUCC_MSG) ;

    /* if not bypassing the nameservice,
     * check that it's ok to use the replica's name
     * in the nameservice.
     */
    if (master_str == NULL) {
        rs_ns_name_compose(rgy_local_cell->name, rep_name, &full_rep_name, st);
        if (BAD_STATUS(st)) {
            rsdb_delete_rgy_files();
            SEC_SVC_PRINT_MSG_ST(SEC_CRDB_CANT_REGISTER_NS_MSG, *st);
        }

        /*
         * check that it's ok to use rep_name for
         * this replica
         */ 
        rs_ns_entry_validate(full_rep_name, &cell_sec_id, rep_id, 
            rep_bindings, secidmap_v1_0_s_ifspec, st);
        if (BAD_STATUS(st)) {
            rsdb_delete_rgy_files();
            SEC_SVC_PRINT_MSG_ST(SEC_CRDB_CANT_REGISTER_NS_MSG, *st);
        } 
    }

    /*
     * add this replica to the master's replist
     */
    sec_rgy_replist_add_replica(master_handle, rep_id, rep_name, 
        rep_twrs, st);
    if (BAD_STATUS(st)) {
        rsdb_delete_rgy_files();
        SEC_SVC_PRINT_MSG_ST(SEC_CRDB_CANT_ADD_REPLICA_MSG, *st);
    } 

    /*
     * Go into uninited state
     * (now we're really committed to live)
     */
    rs_state_set_state(rs_c_state_uninitialized, st);
    if (BAD_STATUS(st)) {
        rsdb_delete_rgy_files();
        SEC_SVC_PRINT_MSG_ST(SEC_CRDB_CANT_UPD_RGYST_FILE_MSG, *st);
    } 

    /* 
     *  If not bypassing the nameservice, put the replica's name, 
     *  uuids, towers, and admin interface in the cell nameservice
     */
    if (master_str == NULL) {
        rs_ns_entry_add(full_rep_name, &cell_sec_id, rep_id, 
            secidmap_v1_0_s_ifspec, rep_bindings, &tmp_st);
        if (BAD_STATUS(&tmp_st)) {
            SEC_SVC_PRINT_MSG_ST(SEC_CRDB_REP_NOT_REGISTERED_MSG, tmp_st);
        } 
    }

    /*
     * Save master's location in the rgy config file (ie. pe_site file)
     */
    sec_rgy_site_binding_get_info(master_handle,
        NULL, NULL, &string_binding, NULL, &tmp_st);

    if (GOOD_STATUS(&tmp_st)) {
        master_binding.count = 1;
        rpc_binding_from_string_binding(string_binding, 
            &master_binding.binding_h[0], &tmp_st);
    }

    /* Create pe_site file with master's bindings, if it doesn't exist */
    if (GOOD_STATUS(&tmp_st)) {
        create_rgy_config_file(rgy_local_cell->name, 
			       &rs_state.cell_sec_id, &master_binding,
			       master_rep_item.rep_name,
			       false, &tmp_st);
    }

    if (BAD_STATUS(&tmp_st)) {
        dce_svc_printf(SEC_CRDB_SITE_FILE_CREATE_FAIL_MSG);
    } else {
        append_rgy_config_file(rgy_local_cell->name,
            &rs_state.cell_sec_id, rep_bindings, rep_name, &tmp_st);
        if (BAD_STATUS(&tmp_st))
            dce_svc_printf(SEC_CRDB_SITE_FILE_UPD_FAIL_MSG);
        else
            dce_svc_printf(SEC_CRDB_SITE_FILE_UPD_SUCC_MSG);
    }
}

/*
 * Create the master replica database
 */
PRIVATE void create_master_db (
    uuid_p_t                rep_id,
    rs_replica_twr_vec_p_t  rep_twrs,
    unsigned_char_p_t       rep_name,
    sec_id_t                *rgy_creator,
    long                    rgy_creator_unix_id,
    sec_id_t                *rgy_local_cell,
    unsigned_char_p_t       rgy_hostname,
    unsigned_char_p_t       rgy_password,
    sec_rgy_properties_t    *default_properties,
    char                    *keyseed,
    unsigned32              master_compat_software_rev,
    error_status_t          *st
)
{
    uuid_t                  cell_sec_id;
    rs_update_seqno_t       seqno;
    rs_replica_mgt_item_t   rep_mgt_item;
    error_status_t          lst;

    if (alt_dir == NULL){ /* sec_create_db */
        rsdb_file_set_base_path(RGY_FILE_DIRECTORY);
        rsdb_file_mkey_set_base_path(MKEY_FILE_DIRECTORY);
   }
    else { /* sec_salvage_db test */
        char base_path[MAXPATHLEN] = { '\0' };
        int      alt_dir_len;

        strcpy(base_path, alt_dir);
        alt_dir_len = strlen(base_path);
        strcpy (&base_path[alt_dir_len], "/");
        rsdb_file_mkey_set_base_path(base_path);
        strcpy(&base_path[alt_dir_len], "/rgy_data/");
        rsdb_file_set_base_path(base_path);
        rs_set_sec_salvage_db_test(true);
        if (keyseed == NULL) {
                keyseed = malloc(strlen(DEFAULT_KEYSEED)+1);
		strcpy(keyseed, DEFAULT_KEYSEED);
        }
    }


    /*
     * Check whether the database already exists
     * by trying to read the state file.
     * State file read should fail.
     */
    rs_state_create_chk(st);
    if (BAD_STATUS(st)) {
        SET_STATUS(st, sec_crdb_db_exists);
        return;
    }
    /*
     * create cell's security object uuid
     */
    uuid_create(&cell_sec_id, &lst);

    /*
     * create initial seqno
     */
    rs_util_seqno_create(&seqno);

    /*  
     * init in memory rs_state info.
     * will be saved to disk after other files
     * have been created.
     */
    rs_state_create_volatile(rs_c_state_in_service, 
        &cell_sec_id, rep_id, true);

    /*
     * create stub database and log files
     */
    rsdb_create(rep_name, rgy_creator, rgy_local_cell,
        default_properties, st);
    if (BAD_STATUS(st)) {
        rsdb_delete_rgy_files();
        return;
    }
    /* create and process the master_key, store it in disk file
     * and in database.
     */
    rs_mkey_create(rgy_local_cell, keyseed);

    /*
     * fill database with default info
     */
    db_create(rgy_local_cell, rgy_creator, rgy_creator_unix_id, 
      rgy_hostname, rgy_password, st);
    if (BAD_STATUS(st)) {
        rsdb_delete_rgy_files();
        return;
    }

    /* 
     * put this server on replist 
     */
    rs_rep_util_rep_item_pack(rep_id, rep_name, rep_twrs,
        rs_c_replica_prop_init, rs_c_replica_prop_unknown, 
        NULL, NULL, &rep_mgt_item);

    rsdb_replica_add(&rep_mgt_item, st);
    if (BAD_STATUS(st)) {
        rsdb_delete_rgy_files();
        return;
    }

    /*
     * save database to disk
     */
    rsdb_save("", st);
    if (BAD_STATUS(st)) {
        rsdb_delete_rgy_files();
        return;
    }

    /* create log and master_info files.
     * must be created after rsdb_create is called
     * because rsdb_create deletes all files in 
     * rgy_data directory
     */

    rs_log_create(&seqno, st);
    if (BAD_STATUS(st)) {
        rsdb_delete_rgy_files();
        return;
    }

    /*
     * Create the master at the maximum software rev
     * supported by the binary.
     */
    rs_master_info_create(rep_id, &seqno, rs_c_max_software_vers, st);
    if (BAD_STATUS(st)) {
        rsdb_delete_rgy_files();
        return;
    }

    /*
     * create rgy_state file on disk and fill
     * it with current rs_state info
     */
    rs_state_create(st);
    if (BAD_STATUS(st)) {
        rsdb_delete_rgy_files();
        return;
    }
}

PRIVATE void create_slave_db (
    uuid_p_t                cell_sec_id,
    uuid_p_t                rep_id,
    rs_replica_twr_vec_p_t  rep_twrs,
    unsigned_char_p_t       rep_name,
    rs_replica_item_t       *master_item,
    rs_update_seqno_t       *master_seqno,
    sec_id_t                *rgy_creator,
    sec_id_t                *rgy_local_cell,
    sec_rgy_properties_t    *default_properties,
    char                    *keyseed,
    unsigned32              master_compat_sw_rev,
    error_status_t          *st
)
{
    rs_update_seqno_t       seqno;
    rs_replica_mgt_item_t   rep_mgt_item,
                            master_mgt_item;
    sec_acl_mgr_handle_t    sec_acl_mgr_handle;

    rsdb_file_set_base_path(RGY_FILE_DIRECTORY);
    rsdb_file_mkey_set_base_path(MKEY_FILE_DIRECTORY);

    /*
     * Check whether the database already exists
     * by trying to read the state file.
     * State file read should fail.
     */
    rs_state_create_chk(st);
    if (BAD_STATUS(st)) {
        SET_STATUS(st, sec_crdb_db_exists);
        return;
    }

    /* 
     * Need to configure the acl manager so database creation code
     * knows which version of acls to create.  Our version of 
     * sec_acl_mgr_configure always returns error_status_ok
     */
    sec_acl_mgr_configure((sec_acl_mgr_config_t)NULL,
			  master_compat_sw_rev,
			  (unsigned_char_p_t)NULL,
			  &sec_acl_mgr_handle,
			  st);

    /*  
     * init in memory rs_state info.
     * will be saved to disk after other files
     * have been created.
     */
    rs_state_create_volatile(
        rs_c_state_unknown_to_master, 
        cell_sec_id, rep_id, false);

    /*
     * create stub database and log files
     */
    rsdb_create(rep_name, rgy_creator, rgy_local_cell,
        default_properties, st);
    if (BAD_STATUS(st)) {
        rsdb_delete_rgy_files();
        return;
    }
    /* create and process the master_key, store it in disk file
     * and in database.
     */
    rs_mkey_create(rgy_local_cell, keyseed);

    /* 
     * put this server on replist 
     */
    rs_rep_util_rep_item_pack(rep_id, rep_name, rep_twrs,
        rs_c_replica_prop_init,  rs_c_replica_prop_unknown, 
        NULL, NULL, &rep_mgt_item);
    if (BAD_STATUS(st)) {
        rsdb_delete_rgy_files();
        return;
    }

    rsdb_replica_add(&rep_mgt_item, st);
    if (BAD_STATUS(st)) {
        rsdb_delete_rgy_files();
        return;
    }

    /*
     * put the master to this slave's replist
     */
    rs_rep_util_rep_item_pack(&master_item->rep_id, 
        master_item->rep_name, master_item->rep_twrs,
        rs_c_replica_prop_init, rs_c_replica_prop_unknown,
        NULL, NULL, &master_mgt_item);
    if (BAD_STATUS(st)) {
        rsdb_delete_rgy_files();
        return;
    }

    rsdb_replica_add(&master_mgt_item, st);
    if (BAD_STATUS(st)) {
        rsdb_delete_rgy_files();
        return;
    }

    rs_resv_gen_uninit_db(master_compat_sw_rev, st);
    if (BAD_STATUS(st)) {
        rsdb_delete_rgy_files();
        return;
    }
    /*
     * save database to disk
     */
    rsdb_save("", st);
    if (BAD_STATUS(st)) {
        rsdb_delete_rgy_files();
        return;
    }

    /* 
     * create log and master_info files.
     * must be created after rsdb_create is called
     * because rsdb_create deletes all files in 
     * rgy_data directory
     */

    /*
     * Create initial bogus seqno for slave.
     * It will be set for real when the slave
     * is inited.
     */
    rs_util_seqno_create(&seqno);

    rs_log_create(&seqno, st);
    if (BAD_STATUS(st)) {
        rsdb_delete_rgy_files();
        return;
    }

    /* 
     * must create master info before adding necessary reserved entries to
     * stub database
     */
    rs_master_info_create(&master_item->rep_id, master_seqno, 
			  master_compat_sw_rev, st);
    if (BAD_STATUS(st)) {
        rsdb_delete_rgy_files();
        return;
    }

    /*
     * create rgy_state file on disk and fill
     * it with current rs_state info
     */
    rs_state_create(st);
    if (BAD_STATUS(st)) {
        rsdb_delete_rgy_files();
        return;
    }
}

PRIVATE void bind_to_master (
    unsigned_char_p_t       bind_site,
    sec_rgy_handle_t        *master_handle,
    error_status_t          *st
)
{ 
     sec_rgy_bind_auth_info_t   auth_info;

    /* slave must inherit the local host principal's identity 
     * and set_auth_info for some authorized calls that follow 
     */
    rs_login_inherit_host_identity(st);
    if(BAD_STATUS(st))
        SEC_SVC_PRINT_MSG_ST(SEC_CRDB_INHERIT_HOSTIDENT_ERR_MSG, *st);
 
    auth_info.info_type = sec_rgy_bind_auth_dce;
    auth_info.tagged_union.dce_info.authn_level = 
        rpc_c_protect_level_pkt_integ;
    auth_info.tagged_union.dce_info.authn_svc = 
        rpc_c_authn_dce_secret;
    auth_info.tagged_union.dce_info.authz_svc = 
        rpc_c_authz_dce;
    auth_info.tagged_union.dce_info.identity = NULL;

    sec_rgy_site_bind_update(bind_site, &auth_info,
            master_handle, st);
    if (BAD_STATUS(st))
        SEC_SVC_PRINT_MSG_ST(SEC_CRDB_CANT_BIND_UPDSITE_MSG, *st);
}

PRIVATE void get_master_info (
    sec_rgy_handle_t        master_handle,
    uuid_t                  *cell_sec_id,
    rs_update_seqno_t       *master_seqno,
    unsigned32              *master_compat_sw_rev,
    rs_replica_item_t       *master_item,
    error_status_t          *st
)
{
    rs_replica_info_t   info;
    uuid_t              cursor;
    unsigned32          nents;

    sec_rgy_rep_admin_info(master_handle, &info, st);
    if(BAD_STATUS(st))
        SEC_SVC_PRINT_MSG_ST(SEC_CRDB_CANT_COM_MASTER_MSG, *st);

    *cell_sec_id = info.cell_sec_id;
    *master_seqno = info.master_seqno;
    *master_compat_sw_rev = info.compat_sw_rev;

    cursor = info.master_id;
    sec_rgy_replist_read(master_handle, &cursor, 
        1, &nents, master_item, st);
    if(BAD_STATUS(st))
        SEC_SVC_PRINT_MSG_ST(SEC_CRDB_CANT_COM_MASTER_MSG, *st);
}

PRIVATE void process_args_master_or_slave (
    int             argc,
    char            *argv[],
    boolean32       *master
)
{
    unsigned32  i;
    boolean32   masterf,
                slavef;

    masterf = false;
    slavef = false;

    for (i = 1; i < argc; i++) {
        if (match_command("-master", argv[i], 7)) {
            masterf = true;
        } 
        
        else if (match_command("-slave", argv[i], 6)) {
            slavef = true;
        } 
    }

    if (slavef && masterf) {
        dce_svc_printf(SEC_CRDB_CL_DUP_OPTION_MSG, "-slave", "-master");
        dce_svc_printf(SEC_CRDB_CL_USAGE_MSG);
    }

/***** Temp comment out
    if (! (slavef || masterf)) {
        error("-slave or -master option must be specified");
        dce_svc_printf(SEC_CRDB_CL_USAGE_MSG);
    }
******/
    /* temp default to master if neither master or slave 
     * flag is speced
     */
    if (! (slavef || masterf)) {
        masterf = true;
    }

    *master = masterf;
}                     

PRIVATE void process_args_master (
    int                     argc,
    char                    *argv[],
    unsigned_char_p_t       *rgy_myname,
    sec_id_t                *rgy_creator,
    long                    *rgy_creator_unix_id,
    char                    **keyseed,
    unsigned_char_p_t       *rgy_password,
    unsigned_char_p_t       *cell_uuid_str,
    sec_rgy_properties_t    *properties
    )
{
    unsigned32  i;
    unsigned32  klen;
    char        error_buf[256];

    for (i = 1; i < argc; i++) {

        if (match_command("-myname", argv[i], 3)) {
            if (++i == argc) {
                dce_svc_printf(SEC_CRDB_CL_MISSING_ARG_MSG, "-my[name]");
                dce_svc_printf(SEC_CRDB_CL_USAGE_MSG);
            }
            if (strlen(argv[i]) > (sec_rgy_name_t_size - 1)) {
                dce_svc_printf(SEC_CRDB_CL_LONG_RGYNAME_MSG,
                               "-myname",
                               (sec_rgy_name_t_size - 1));
                dce_svc_printf(SEC_CRDB_CL_USAGE_MSG);
            }

            *rgy_myname = (unsigned_char_p_t)argv[i];
        } 

        else if (match_command("-creator", argv[i], 3)) {
            if (++i == argc) {
                dce_svc_printf(SEC_CRDB_CL_MISSING_ARG_MSG, "-cr[eator]");
                dce_svc_printf(SEC_CRDB_CL_USAGE_MSG);
            } 
            if (strlen(argv[i]) > (sec_rgy_name_t_size - 1)) {
                dce_svc_printf(SEC_CRDB_CL_LONG_RGYNAME_MSG,
                               "-myname",
                               (sec_rgy_name_t_size - 1));
                dce_svc_printf(SEC_CRDB_CL_USAGE_MSG);
            }

            rgy_creator->name = (unsigned_char_p_t)argv[i];
        } 

        else if (match_command("-cunix_id", argv[i], 3)) {
            if (++i == argc) {
                dce_svc_printf(SEC_CRDB_CL_MISSING_ARG_MSG, "-cu[nix_id]");
                dce_svc_printf(SEC_CRDB_CL_USAGE_MSG);
            } 

            *rgy_creator_unix_id = atol(argv[i]);
        } 

        else if (match_command("-uuid", argv[i], 2)) {
            if (++i == argc) {
                dce_svc_printf(SEC_CRDB_CL_MISSING_ARG_MSG, "-u[uid]");
                dce_svc_printf(SEC_CRDB_CL_USAGE_MSG);
            }
                
            *cell_uuid_str = (unsigned_char_p_t) argv[i];
        } 

        else if (match_command("-person_low_unix_id", argv[i], 3)) {
            if (++i == argc) {
                dce_svc_printf(SEC_CRDB_CL_MISSING_ARG_MSG,
                               "-pe[rson_low_unix_id]");
                dce_svc_printf(SEC_CRDB_CL_USAGE_MSG);
            } 

            properties->low_unix_id_person = atol(argv[i]);
        } 

        else if (match_command("-group_low_unix_id", argv[i], 2)) {
            if (++i == argc) {
                dce_svc_printf(SEC_CRDB_CL_MISSING_ARG_MSG,
                               "-g[roup_low_unix_id]");
                dce_svc_printf(SEC_CRDB_CL_USAGE_MSG);
            } 

            properties->low_unix_id_group = atol(argv[i]);
        } 

        else if (match_command("-org_low_unix_id", argv[i], 2)) {
            if (++i == argc) {
                dce_svc_printf(SEC_CRDB_CL_MISSING_ARG_MSG,
                               "-o[rg_low_unix_id]");
                dce_svc_printf(SEC_CRDB_CL_USAGE_MSG);
            }

            properties->low_unix_id_org = atol(argv[i]);
        } 

        else if (match_command("-max_unix_id", argv[i], 3)) {
            if (++i == argc) {
                dce_svc_printf(SEC_CRDB_CL_MISSING_ARG_MSG,
                               "-ma[x_unix_id]");
                dce_svc_printf(SEC_CRDB_CL_USAGE_MSG);
            } 

            properties->max_unix_id = atol(argv[i]);
        } 

        else if (match_command("-keyseed", argv[i], 2)) {
            if (++i == argc) {
                dce_svc_printf(SEC_CRDB_CL_MISSING_ARG_MSG,
                               "-k[eyseed]");
                dce_svc_printf(SEC_CRDB_CL_USAGE_MSG);
            }

            klen = strlen((char *) argv[i]);
            *keyseed = malloc(klen+1);
            u_strcpy(*keyseed, argv[i]);
            memset((char *) argv[i], 0, klen);
        } 

        else if (match_command("-alt_dir", argv[i], 2)) {
            if (++i == argc || argv[i][0] == '-') {
                dce_svc_printf(SEC_CRDB_CL_ALT_DIR_NO_ARG_MSG, DEFAULT_PATH);
                alt_dir = DEFAULT_PATH;
                if (argv[i][0] == '-')
                    i--;
            }
            else
                alt_dir = (char *) argv[i];
        }

        else if (match_command("-password", argv[i], 3)) {
            if (++i == argc) {
                dce_svc_printf(SEC_CRDB_CL_MISSING_ARG_MSG,
                               "-pa[ssword]");
                dce_svc_printf(SEC_CRDB_CL_USAGE_MSG);
            }
            if (strlen(argv[i]) > sec_passwd_str_max_len) {
                dce_svc_printf(SEC_CRDB_CL_LONG_PASSWD_MSG);
                dce_svc_printf(SEC_CRDB_CL_USAGE_MSG);
            }

            *rgy_password = (unsigned_char_p_t) argv[i];
        } 

        else if (match_command("-verbose", argv[i], 2)) {
            rs_verbose = true;
        } 

        else if (match_command("-master", argv[i], 7)) {
            ;
        } 

        else {
            dce_svc_printf(SEC_CRDB_CL_UNKNOWN_OPTION_MSG,
                           argv[i]);
            dce_svc_printf(SEC_CRDB_CL_USAGE_MSG);
        }
    }

    if (*rgy_myname == NULL) {
        dce_svc_printf(SEC_CRDB_CL_NULL_MYNAME_MSG);
        dce_svc_printf(SEC_CRDB_CL_USAGE_MSG);
    }
}

PRIVATE void process_args_slave (
    int                     argc,
    char                    *argv[],
    unsigned_char_p_t       *rgy_myname,
    char                    **keyseed,
    unsigned_char_p_t       *master_str
    )
{
    unsigned32  i;
    unsigned32  klen;
    char        error_buf[256];

    for (i = 1; i < argc; i++) {

        if (match_command("-myname", argv[i], 3)) {
            if (++i == argc) {
                dce_svc_printf(SEC_CRDB_CL_MISSING_ARG_MSG,
                               "-my[name]");
                dce_svc_printf(SEC_CRDB_CL_USAGE_MSG);
            }
            if (strlen(argv[i]) > (sec_rgy_name_t_size - 1)) {
                dce_svc_printf(SEC_CRDB_CL_LONG_RGYNAME_MSG,
                               "-myname",
                               (sec_rgy_name_t_size - 1));
                dce_svc_printf(SEC_CRDB_CL_USAGE_MSG);
            }

            *rgy_myname = (unsigned_char_p_t) argv[i];
        } 

        else if (match_command("-keyseed", argv[i], 2)) {
            if (++i == argc) {
                dce_svc_printf(SEC_CRDB_CL_MISSING_ARG_MSG,
                               "-k[eyseed]");
                dce_svc_printf(SEC_CRDB_CL_USAGE_MSG);
            }

            klen = strlen((char *) argv[i]);
            *keyseed = malloc(klen+1);
            u_strcpy(*keyseed, argv[i]);
            memset((char *) argv[i], 0, klen);
        } 

        else if (match_command("-nons", argv[i], 5)) {
            if (++i == argc) {
                dce_svc_printf(SEC_CRDB_CL_MISSING_ARG_MSG,
                               "-nons");
                dce_svc_printf(SEC_CRDB_CL_USAGE_MSG);
            }

            *master_str = (unsigned_char_p_t) argv[i];
        } 

        else if (match_command("-verbose", argv[i], 2)) {
            rs_verbose = true;
        } 

        else if (match_command("-slave", argv[i], 6)) {
            ;
        } 

        else {
            dce_svc_printf(SEC_CRDB_CL_UNKNOWN_OPTION_MSG,
                           argv[i]);
            dce_svc_printf(SEC_CRDB_CL_USAGE_MSG);
        }
    }

    if (*rgy_myname == NULL) {
        dce_svc_printf(SEC_CRDB_CL_NULL_MYNAME_MSG);
        dce_svc_printf(SEC_CRDB_CL_USAGE_MSG);
    }
}

PRIVATE boolean32 process_name (
    char    *cell_name,
    char    **name
    )
{
    long    name_len, cell_len;

    /* Legal syntax for name includes:
     *      path_name,        (no initial slash)
     *      /.:/path_name,
     *      <cell_name>/path_name.
     */
    cell_len = strlen(cell_name);

    if( **name == '/' ) {
        if(strncmp(*name, CELL_DIR_ROOT, CELL_DIR_ROOT_LEN) == 0) {
            (*name) += CELL_DIR_ROOT_LEN;
        } else if(strncmp(*name, cell_name, cell_len) == 0) {
            /* strip cell name and additional slash */
            (*name) += (cell_len + 1);
        } else {
            /* initial slash illegal without cell prefix */
            return false;
        }
    }
    name_len = strlen(*name);
    /* Weed out typos and bad input */
    if( name_len < 1                       ||
        name_len > sec_rgy_name_t_size - 1 ||
        **name == '/'                      ) {
        return false;
    }
    return true;
}


/*
 * Internal routine that adds string bindings to the config file
 */
PRIVATE void write_rgy_config_file (
    int                     fd,
    unsigned_char_p_t       cell_name,
    uuid_p_t                cell_sec_id,
    rpc_binding_vector_p_t  bindings,
    unsigned_char_p_t	    rep_name,
    error_status_t          *st
    )
{
    unsigned32              i;
    unsigned_char_p_t       string_binding          = NULL,
                            string_object_uuid      = NULL,
                            string_protseq          = NULL,
                            string_netaddr          = NULL,
                            string_endpoint         = NULL,
                            string_options          = NULL;
    char                    buf[1024];
    int                     buf_len, res;
    error_status_t          lst;

    uuid_to_string(cell_sec_id, &string_object_uuid, st);
    if (BAD_STATUS(st)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_create_db , svc_c_sev_error , *st);
        return ;
    }

    for (i = 0; (i < bindings->count) && GOOD_STATUS(st); i++) {
        rpc_binding_to_string_binding(bindings->binding_h[i],
            &string_binding, st);

        if (GOOD_STATUS(st)) {
            rpc_string_binding_parse(string_binding,
                             NULL,
                             &string_protseq,
                             &string_netaddr,
                             NULL,
                             &string_options,
                             st);
        }
        else {
            dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                           sec_s_rs_create_db , svc_c_sev_error , *st);
        }

        if (GOOD_STATUS(st)) {
            rpc_string_free(&string_binding, &lst);
    
            rpc_string_binding_compose(string_object_uuid,
                            string_protseq, string_netaddr,
                            NULL, string_options, &string_binding,
                            st);
        }
        else
            dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                           sec_s_rs_create_db , svc_c_sev_error , *st);
    
        if (GOOD_STATUS(st)) {
            sprintf(buf, "%s %s %s\n", (char *) cell_name,
                (char *) string_binding, (char *) rep_name);

            buf_len = strlen(buf);
        
            res = write(fd, buf, buf_len);
        
            if (res != buf_len) {
                dce_svc_printf(DCE_SVC(sec_svc_handle, "%d"),
                               sec_s_rs_create_db,
                               svc_c_sev_error,
                               sec_sys_file_write_error,
                               fd);
                SET_STATUS(st, sec_rgy_rep_cannot_create_db);
            }
        }

        if (string_binding) {
            rpc_string_free(&string_binding, &lst);
        }
        if (string_protseq) {
            rpc_string_free(&string_protseq, &lst);
        }
        if (string_netaddr) {
            rpc_string_free(&string_netaddr, &lst);
        }
        if (string_options) {
            rpc_string_free(&string_options, &lst);
        }
    }

    if (string_object_uuid) {
        rpc_string_free(&string_object_uuid, &lst);
    }
}


/* 
 * Create config file and store string bindings for rgy master
 */
PRIVATE void create_rgy_config_file (
    unsigned_char_p_t       cell_name,
    uuid_p_t                cell_sec_id,
    rpc_binding_vector_p_t  bindings,
    unsigned_char_p_t       rep_name,
    boolean32               masterf,
    error_status_t          *st
    )
{
    int                     fd;
    char                    bak_file[MAXPATHLEN];

    CLEAR_STATUS(st);

    fd = access(RGY_CONFIG_FILE, F_OK);
    if (fd == 0) {
        /* file exists, don't recreate it if this is a slave
         */
        if (!masterf) return;

        /* rename file with ".bak" */
        ustrcpy(bak_file, RGY_CONFIG_FILE);
        ustrcat(bak_file, ".bak");
        rename(RGY_CONFIG_FILE, bak_file);
    }
    fd = open(RGY_CONFIG_FILE, O_RDWR|O_CREAT, 0644);
    if (fd == -1) {
        dce_svc_printf(SEC_SYS_ERRNO_TEXT_MSG, "open", strerror(errno));
        dce_svc_printf(DCE_SVC(sec_svc_handle, "%s"),
                       sec_s_rs_create_db,
                       svc_c_sev_error, 
                       sec_sys_file_open_fail,
                       RGY_CONFIG_FILE);
        SET_STATUS(st, sec_rgy_rep_cannot_create_db);
        return;
    }
    write_rgy_config_file(fd, cell_name, cell_sec_id, bindings, rep_name, st);
    close(fd);
}


/* 
 * Append slave string bindings to rgy config file
 */
PRIVATE void append_rgy_config_file (
    unsigned_char_p_t       cell_name,
    uuid_p_t                cell_sec_id,
    rpc_binding_vector_p_t  bindings,
    unsigned_char_p_t       rep_name,
    error_status_t          *st
    )
{
    int                     fd;
    char                    bak_file[MAXPATHLEN];

    fd = open(RGY_CONFIG_FILE, O_RDWR);
    if (fd == -1) {
        dce_svc_printf(SEC_SYS_ERRNO_TEXT_MSG, "open", strerror(errno));
        dce_svc_printf(DCE_SVC(sec_svc_handle, "%s"),
                       sec_s_rs_create_db,
                       svc_c_sev_error, 
                       sec_sys_file_open_fail,
                       RGY_CONFIG_FILE);
        return;
    } else {
        /* get to end of file, so it will be appended to, not rewritten */
        if (lseek(fd, 0, SEEK_END) == -1) {
            dce_svc_printf(SEC_SYS_ERRNO_TEXT_MSG, "lseek", strerror(errno));
            dce_svc_printf(DCE_SVC(sec_svc_handle, "%d"),
                           sec_s_rs_create_db,
                           svc_c_sev_error, 
                           sec_sys_file_lseek_fail,
                           fd);
        } else {
            write_rgy_config_file(fd, cell_name, cell_sec_id, bindings,
				  rep_name, st);
        }
    }
    close(fd);
}
