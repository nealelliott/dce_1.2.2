/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY 
 * $Log: rs.c,v $
 * Revision 1.1.17.3  1996/02/18  00:17:20  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:06:18  marty]
 *
 * Revision 1.1.17.2  1995/12/08  17:50:28  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/3  1995/05/15  21:04 UTC  greg
 * 	Mergeout from HPDCE02 to SEC_migrate_merge.
 * 	[1995/05/13  02:41 UTC  greg  /main/HPDCE02/SEC_migrate_merge/2]
 * 
 * 	Merge CHFts13996.
 * 	[1995/05/03  19:28 UTC  jrr  /main/HPDCE02/2]
 * 
 * 	Fix secd's name string used for syslog entries (CHFts13996).
 * 	[1995/04/11  17:49 UTC  jrr  /main/HPDCE02/jrr_sec_chfts13996/1]
 * 
 * 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
 * 	[1995/04/21  18:57 UTC  greg  /main/HPDCE02/SEC_migrate_merge/1]
 * 
 * 	HP revision /main/HPDCE02/2  1995/05/03  19:28 UTC  jrr
 * 	Merge CHFts13996.
 * 
 * 	HP revision /main/HPDCE02/jrr_sec_chfts13996/1  1995/04/11  17:49 UTC  jrr
 * 	Fix secd's name string used for syslog entries (CHFts13996).
 * 
 * 	HP revision /main/HPDCE02/1  1995/03/27  19:33 UTC  cuti
 * 	CHFts14111 bug fix
 * 
 * 	HP revision /main/cuti_mothra_bug/2  1995/03/23  18:41 UTC  cuti
 * 	Add comment for true in rs_rep_mgmt_shutdown() call.
 * 
 * 	HP revision /main/cuti_mothra_bug/1  1995/03/14  18:41 UTC  cuti
 * 	Add sigcatch task using sigwait
 * 	[1995/12/08  17:20:08  root]
 * 
 * Revision 1.1.12.1  1994/10/26  20:00:53  hondo
 * 	changes to support propagation of login activity.
 * 	[1994/10/25  14:59:12  hondo]
 * 
 * 	changes for replication of login activity.
 * 
 * Revision 1.1.10.7  1994/08/25  23:29:57  sekhar
 * 	Put back SVC messages for fatal_st() and error_st() calls.
 * 	[1994/08/25  23:28:45  sekhar]
 * 
 * Revision 1.1.10.6  1994/08/25  19:07:07  sekhar
 * 	[OT 11751 ] - Sec ACL Manager for SVCIF
 * 		      ( Register Serviceability Interface with RPC ).
 * 	[1994/08/24  19:47:13  sekhar]
 * 
 * Revision 1.1.10.5  1994/08/04  16:13:23  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:10:55  mdf]
 * 
 * Revision 1.1.10.3  1994/06/02  21:24:52  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:18:00  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 	[1994/05/18  11:55:23  mdf]
 * 
 * Revision 1.1.6.2  1993/10/29  16:33:19  hanfei
 * 	modified for change master work : rs_server_register_prop_if for
 * 	both master and replica.
 * 	[1993/10/29  16:32:36  hanfei]
 * 
 * Revision 1.1.4.3  1993/09/15  15:49:41  root
 * 	    Loading drop DCE1_0_3b03
 * 
 * Revision 1.1.6.11  1993/03/29  22:19:58  emartin
 * 	CR 7593.  rs_prop_plcy_v1_0 => rs_prop_plcy_v1_1
 * 	[1993/03/25  21:24:25  emartin]
 * 
 * Revision 1.1.6.10  1993/02/26  23:50:09  emartin
 * 	em_bl6f: bump version of rs_repmgr, rs_bind
 * 	[1993/02/24  22:28:21  emartin]
 * 
 * Revision 1.1.6.9  1993/02/03  22:06:24  ahop
 * 	bl6e: remove rs_auth_init from rs_setup_dce_rgy_identity
 * 	[1993/01/22  22:34:22  ahop]
 * 
 * Revision 1.1.6.8  1993/01/26  23:04:35  emartin
 * 	remove comment out of rs_prop_ifspecs
 * 	[1993/01/13  16:28:11  emartin]
 * 
 * Revision 1.1.6.7  1992/12/29  16:34:33  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:22:59  zeliff]
 * 
 * Revision 1.1.6.6  1992/12/18  22:11:38  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:48:48  sekhar]
 * 
 * Revision 1.1.8.3  1992/12/11  21:40:52  emartin
 * 	do rs_start_kerberos, rs_setup_dce_rgy_identity
 * 
 * Revision 1.1.8.2  1992/12/10  21:27:45  emartin
 * 	bl6c: remove comment out of rs_prop_*.h
 * 
 * Revision 1.1.6.5  1992/11/23  22:33:03  ahop
 * 	6179: initialize rs_string_bindings->count in save_string_bindings
 * 	[1992/11/23  21:18:22  ahop]
 * 
 * Revision 1.1.6.4  1992/11/04  19:34:50  ahop
 * 	Third replication drop: bl6a
 * 	[1992/11/03  18:28:35  ahop]
 * 
 * Revision 1.1.6.3  1992/10/07  20:32:08  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:15:51  sekhar]
 * 
 * Revision 1.1.7.2  1992/10/06  16:07:06  burati
 * 	Second replication drop: bl5
 * 	Do rs_server_unregister...
 * 	Remove master_key stuff.
 * 
 * Revision 1.1.6.2  1992/08/31  18:23:45  sekhar
 * 	First security replication code drop
 * 	[1992/08/31  15:11:13  sekhar]
 * 
 * Revision 1.1.4.5  1992/07/13  20:01:23  ahop
 * 	 3903: update rsdb_acct_get_auth_key_by_name semantics
 * 	[1992/06/30  22:29:42  ahop]
 * 
 * Revision 1.1.4.4  1992/06/02  21:12:30  sekhar
 * 	Merged the Unix interface changes made by frisco to
 * 	the mainline
 * 	[1992/06/02  19:40:11  sekhar]
 * 
 * Revision 1.1.2.2  1992/05/22  19:31:33  frisco
 * 	Add Unix interface support
 * 
 * Revision 1.1.4.3  1992/05/27  19:17:38  rsalz
 * 	 Remove DCE_1_1_RPC #ifdef; it is always on now
 * 	[1992/05/27  19:16:12  rsalz]
 * 
 * Revision 1.1.4.2  1992/05/26  13:57:34  rsalz
 * 	Add #define DCE_1_1_RPC
 * 	[1992/05/26  13:57:11  rsalz]
 * 
 * Revision 1.1  1992/01/19  14:44:28  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
 * Copyright (c) Hewlett-Packard Company 1991, 1992, 1994
 * Unpublished work. All Rights Reserved.
 */
/*
 *      Registry Server - some initialization,
 *          manage server bindings, server
 *          registration with rpc runtime and
 *          endpoint mapper
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = 
    "$Id: rs.c,v 1.1.17.3 1996/02/18 00:17:20 marty Exp $";
#endif


#include <un_time.h>
#include <un_strng.h>
#include <un_io.h>
#include <un_maloc.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/errno.h>

#include <dce/dce.h>
#include <dce/uuid.h>
#include <dce/rpc.h>
#include <dce/dce_error.h>
#include <dce/sec_login.h>
#include <dce/binding.h>

#include <rs_acct.h>
#include <rs_pgo.h>
#include <rs_plcy.h>
#include <rpriv.h>
#include <rpriv_1_0.h>
#include <rdaclif.h>
#include <rdaclifv0.h>
#include <rsecidmap.h>
#include <krb5rpc.h>
#include <rs_unix.h>
#include <rs_attr.h>
#include <rs_attr_schema.h>
#include <rs_attr_srch.h>
#include <rs_login_activity.h>
#include <rs_prop_login_reset.h>

#include <rs_prop_acct.h>
#include <rs_prop_pgo.h>
#include <rs_prop_plcy.h>
#include <rs_prop_acl.h>
#include <rs_prop_replist.h>
#include <rs_prop_attr.h>
#include <rs_prop_attr_schema.h>

#include <rs_misc.h>
#include <rs_repadm.h>
#include <rs_repmgr.h>
#include <rs_replist.h>
#include <rs_replist_1_0.h>
#include <rs_bind.h>

#include <rs_ver.h>
#include <rs_ver_pvt.h>
#include <rs.h>
#include <rs_pvt.h>
#include <rs_state.h>
#include <rs_base.h>
#include <rs_lock.h>
#include <rs_rsdb.h>
#include <rs_auth.h>
#include <rs_util.h>
#include <rs_audit_trail.h>
#include <rs_task.h>
#include <rs_pwd_mgmt.h>

#include <rsdb_pvt.h>
#include <rsdb.h>
#include <rsdb_file.h>
#include <rsdb_policy.h>
#include <rsdb_acct.h>

#include <sec_svc.h>
#include <dce/service.h>

/* Array of bindings this server supports 
 */
PRIVATE rpc_binding_vector_p_t  rs_bindings = NULL;

/* Server's string bindings
 */
typedef struct {
    unsigned_char_p_t   protseq;
    unsigned_char_p_t   network_addr;
} rs_string_binding_t;

typedef struct {
    unsigned32          count;
    rs_string_binding_t string_binding[VARYING];    /* size = count */
} rs_string_binding_vec_t, *rs_string_binding_vec_p_t;

PRIVATE rs_string_binding_vec_p_t   rs_string_bindings = NULL;


PRIVATE unsigned32  rs_concurrency_level = 5;

typedef struct {
	 rpc_if_handle_t *if_spec;
	 unsigned32      min_sw_rev;
         unsigned32      max_sw_rev;
         boolean32       registered;
         unsigned char   *annotation;
} rs_if_info_t;

/* 
 * Server's interfaces for runtime
 *
 * PLEASE NOTE!!: If you add a new entry (or delete an entry) 
 * to this array, please make sure you also add or delete the
 * corresponding entry in the rs_mgrepvs array below.
 */
PRIVATE rs_if_info_t rs_ifspecs_info[] = {
    {
	&rs_misc_v1_0_s_ifspec, 
	rs_c_default_min_sw_vers,
	rs_c_default_max_sw_vers, 
	false,
	"DCE user registry rs_misc_v1_0_s_ifspec"
    },
    {
	&rs_policy_v1_0_s_ifspec,
	rs_c_default_min_sw_vers,
	rs_c_default_max_sw_vers,
	false,
	"DCE user registry rs_policy_v1_0_s_ifspec"
    },
    {
	&rs_pgo_v1_0_s_ifspec,
	rs_c_default_min_sw_vers,
	rs_c_default_max_sw_vers,
	false,
	"DCE user registry rs_pgo_v1_0_s_ifspec"
    },
    {
	&rs_acct_v1_0_s_ifspec,
	rs_c_default_min_sw_vers,
	rs_c_default_max_sw_vers,
	false,
	"DCE user registry rs_acct_v1_0_s_ifspec"
    },
    {
	&rs_unix_v1_0_s_ifspec,
	rs_c_default_min_sw_vers,
	rs_c_default_max_sw_vers,
	false,
	"DCE user registry rs_unix_v1_0_s_ifspec"
    },
    {
	&rpriv_v1_0_s_ifspec,
	rs_c_software_vers_1_0_2,
	rs_c_software_vers_1_0_2,
	false,
	"DCE user registry rpriv_v1_0_s_ifspec"
    },
    {
	&rdaclif_v0_0_s_ifspec,
	rs_c_default_min_sw_vers,
	rs_c_default_max_sw_vers,
	false,
	"DCE user registry rdaclif_v0_0_s_ifspec"
    },
    {
	&secidmap_v1_0_s_ifspec,
	rs_c_default_min_sw_vers,
	rs_c_default_max_sw_vers,
	false,
	"DCE user registry secidmap_v1_0_s_ifspec"
    },
    {
	&krb5rpc_v1_0_s_ifspec,
	rs_c_default_min_sw_vers,
	rs_c_default_max_sw_vers,
	false,
	"DCE user registry - krb5rpc_v1_0_s_ifspec"
    },
    {
	&rs_repadm_v1_2_s_ifspec,
	rs_c_default_min_sw_vers,
	rs_c_default_max_sw_vers,
	false,
	"DCE user registry rs_repadm_v1_2_s_ifspec"
    },
    {
	&rs_repmgr_v2_0_s_ifspec,
	rs_c_default_min_sw_vers,
	rs_c_default_max_sw_vers,
	false,
	"DCE user registry rs_repmgr_v2_0_s_ifspec"
    },
    {
	&rs_replist_v1_0_s_ifspec,
	rs_c_default_min_sw_vers,
	rs_c_default_max_sw_vers,
	false,
	"DCE user registry rs_replist_v1_0_s_ifspec"
    },
    {
	&rs_bind_v2_0_s_ifspec,
	rs_c_default_min_sw_vers,
	rs_c_default_max_sw_vers,
	false,
	"DCE user registry rs_bind_v2_0_s_ifspec"
    },
    {
	&rs_attr_v0_0_s_ifspec,
	rs_c_software_vers_1_1,
	rs_c_default_max_sw_vers,
	false,
	"DCE user registry rs_attr_v0_0_s_ifspec"
    },
    {
	&rs_attr_schema_v0_0_s_ifspec,
	rs_c_software_vers_1_1,
	rs_c_default_max_sw_vers,
	false,
	"DCE user registry rs_attr_schema_v0_0_s_ifspec"
    },
    {
	&rs_attr_srch_v0_0_s_ifspec,
	rs_c_software_vers_1_1,
	rs_c_default_max_sw_vers,
	false,
	"DCE user registry rs_attr_srch_v0_0_s_ifspec"
    },
    {
	&rs_pwd_mgmt_v1_0_s_ifspec,
	rs_c_software_vers_1_1,
	rs_c_default_max_sw_vers,
	false,
	"DCE user registry rs_pwd_mgmt_v1_0_s_ifspec"
    },
    {
	&rs_login_activity_v1_0_s_ifspec,
	rs_c_software_vers_1_1,
	rs_c_default_max_sw_vers,
	false,
	"DCE user registry rs_login_activity_v1_0_s_ifspec"
    },
    {
	&rdaclif_v1_0_s_ifspec,
	rs_c_software_vers_1_1,
	rs_c_default_max_sw_vers,
	false,
	"DCE user registry rdaclif_v1_0_s_ifspec"
    },
    {
	/* 
	 * export serviceability even if we're supposedly
         * pre-1.1 server.  There's no reason to deprive
         * anyone of serviceability.
	 */
	&serviceability_v1_0_s_ifspec,
	rs_c_default_min_sw_vers,
	rs_c_default_max_sw_vers,
	false,
	"DCE user registry serviceability_v1_0_s_ifspec"
    },
    {
	&rpriv_v1_1_s_ifspec,
	rs_c_software_vers_1_1,
	rs_c_default_max_sw_vers,
	false,
	"DCE user registry rpriv_v1_1_s_ifspec"
    },
    {
	/*
	 * Always register the v2_0 replist interface
	 * even when we're operating in v1_0 mode.
	 * It doesn't hurt, since only a new client would
	 * ever use the new version, and it makes the
         * compatibility path a bit more efficient.
         */
	&rs_replist_v2_0_s_ifspec,
	rs_c_default_min_sw_vers,
	rs_c_default_max_sw_vers,
	false,
	"DCE user registry rs_replist_v2_0_s_ifspec"
    },
    {
	NULL,
	rs_c_default_min_sw_vers,
	rs_c_default_max_sw_vers,
	false
    }
};


/* 
 * Server's manager epv's (corresponding to above interfaces) for runtime
 */
extern rpc_mgr_epv_t rdaclif_v0_0_manager_epv;
extern rpc_mgr_epv_t rdaclif_v1_0_manager_epv;
extern rpc_mgr_epv_t serviceability_v1_0_mgr_epv;
extern rpc_mgr_epv_t rs_replist_v1_0_manager_epv;
extern rpc_mgr_epv_t rs_replist_v2_0_manager_epv;

PRIVATE rpc_mgr_epv_t *rs_mgrepvs[] = {
    NULL,			/* &rs_misc_v1_0_s_ifspec        */
    NULL,			/* &rs_policy_v1_0_s_ifspec      */
    NULL,			/* &rs_pgo_v1_0_s_ifspec         */
    NULL,			/* &rs_acct_v1_0_s_ifspec        */
    NULL,			/* &rs_unix_v1_0_s_ifspec        */
    NULL,			/* &rpriv_v1_0_s_ifspec          */
    &rdaclif_v0_0_manager_epv,	/* &rdaclif_v0_0_s_ifspec        */
    NULL,			/* &secidmap_v1_0_s_ifspec       */
    NULL,			/* &krb5rpc_v1_0_s_ifspec        */
    NULL,			/* &rs_repadm_v1_1_s_ifspec      */
    NULL,			/* &rs_repmgr_v2_0_s_ifspec      */
    &rs_replist_v1_0_manager_epv,/* &rs_replist_v1_0_s_ifspec     */
    NULL,			/* &rs_bind_v2_0_s_ifspec        */
    NULL,			/* &rs_attr_v0_0_s_ifspec        */
    NULL,			/* &rs_attr_schema_v0_0_s_ifspec */
    NULL,			/* &rs_attr_srch_v0_0_s_ifspec   */
    NULL,                       /* &rs_pwd_mgmt_v1_0_s_ifspec    */
    NULL,                       /* &rs_login_activity_v1_0_s_ifspec    */
    &rdaclif_v1_0_manager_epv,	/* &rdaclif_v1_0_s_ifspec        */
    &serviceability_v1_0_mgr_epv, /* &serviceability_v1_0_s_ifspec */
    NULL,			/* &rpriv_v1_1_s_ifspec          */
    &rs_replist_v2_0_manager_epv /* &rs_replist_v2_0_s_ifspec     */
};


/* Server's interfaces to receive propagations
 */
PRIVATE rpc_if_handle_t *rs_prop_ifspecs[] = {
    &rs_prop_pgo_v1_0_s_ifspec,
    &rs_prop_acct_v1_0_s_ifspec,
    &rs_prop_plcy_v1_1_s_ifspec,
    &rs_prop_acl_v1_0_s_ifspec,
    &rs_prop_replist_v1_0_s_ifspec,
    &rs_prop_attr_v1_0_s_ifspec,
    &rs_prop_attr_sch_v1_0_s_ifspec,
    &rs_prop_login_v1_0_s_ifspec,
    NULL
};

PRIVATE boolean32   kdc_started = false;

PRIVATE char *kdc_argv[] = {
    rs_syslog_name,
    0
};

PRIVATE boolean32   dce_rgy_identity_inited = false;

/* PRIVATE routine prototypes
 */

PRIVATE void rs_server_bindings_save_string_bindings (
    rpc_binding_vector_p_t  bindings,
    error_status_t          *st
);

PRIVATE void rs_server_bindings_to_twr_vector (
    rpc_binding_vector_p_t  bindings,
    rpc_if_handle_t         if_spec,
    rs_replica_twr_vec_p_t  *rep_twrs,
    error_status_t          *st
);

PRIVATE void *start_kerberos_task (
    void    *arg
);

PRIVATE void *start_sigcatch_task (
    void    *arg
);
PRIVATE void rs_server_ep_register (
    error_status_t  *status
);

/*
 * Initialize the server's bindings to which
 * RPCs may be made.  Save bindings in rs_bindings
 * and string bindings in rs_string_bindings.
 *
 * Derive the server's protocol towers from
 * these bindings
 */
PUBLIC void rs_server_bindings_init
  (
    rpc_binding_vector_p_t  *svr_bindings,
    rs_replica_twr_vec_p_t  *svr_twrs,
    error_status_t          *st
  )
{
    CLEAR_STATUS(st);
    *svr_bindings = NULL;
    *svr_twrs = NULL;

    rpc_server_use_all_protseqs(rs_concurrency_level, st);
    if (BAD_STATUS(st)) {
        SEC_SVC_PRINT_MSG_ST(SEC_RS_RPC_USE_PROTSEQ_ERR_MSG, *st);
        return;
    }

    /* retrieve bindings from the runtime and store
     * them in rs_bindings
     */
    rpc_server_inq_bindings(&rs_bindings, st);
    if (BAD_STATUS(st)) {
        SEC_SVC_PRINT_MSG_ST(SEC_RS_RPC_INQ_BIND_ERR_MSG, *st);
        return;
    }

    if (rs_bindings->count == 0) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_rpc,
                       svc_c_sev_error,
                       sec_rgy_rep_bad_binding);
        SET_STATUS(st, sec_rgy_rep_bad_binding);
        return;
    }

    /* save server's network addresses as strings
     */
    rs_server_bindings_save_string_bindings(rs_bindings, st);
    if (BAD_STATUS(st)) {
        SEC_SVC_PRINT_MSG_ST(SEC_RS_RPC_SAVE_BIND_ERR_MSG, *st);
        return;
    }

    rs_server_bindings_to_twr_vector(rs_bindings,
        rs_repadm_v1_2_s_ifspec, svr_twrs, st);
    if (BAD_STATUS(st)) {
        SEC_SVC_PRINT_MSG_ST(SEC_RS_RPC_PROT_TWR_ERR_MSG, *st);
        return;
     }

    *svr_bindings = rs_bindings;
}

PUBLIC void rs_server_bindings_inquire
  (
    rpc_binding_vector_p_t  *svr_bindings,
    error_status_t          *st
  )
{
    *svr_bindings = rs_bindings;

    if (rs_bindings == NULL) {
        SET_STATUS(st, sec_rgy_rep_bad_binding);
    }
    else {
        CLEAR_STATUS(st);
    }
}

/* rs_server_bindings_naddr_equal
 *
 * return the number of network addresses in "bindings" 
 * which match the server's addresses (as saved in 
 * rs_string_bindings)
 */
PUBLIC void rs_server_bindings_naddr_equal
    (
    rpc_binding_vector_p_t  bindings,
    unsigned32              *nequal,
    error_status_t          *st
    )
{
    unsigned32  tmp_nequal;
    unsigned32  i;

    *nequal = 0;
    CLEAR_STATUS(st);

    if (bindings == NULL) return;

    tmp_nequal = 0;
    for (i = 0; i < bindings->count; i++) {
        if (rs_server_bindings_local_client(bindings->binding_h[i], st))
            tmp_nequal++;
        if (BAD_STATUS(st)) return;
    }

    *nequal = tmp_nequal;
}

/*  rs_server_bindings_local_client
 *
 *  Return true if the handle h identifies
 *  contains a local network address.
 *  Return false otherwise.
 */
PUBLIC boolean32 rs_server_bindings_local_client
    (
    handle_t                h,
    error_status_t          *st
    )
{
    unsigned_char_p_t       string_binding;
    unsigned_char_p_t       protseq;
    unsigned_char_p_t       network_addr;
    error_status_t          lst;
    unsigned32              i;

    CLEAR_STATUS(st);
    rpc_binding_to_string_binding(h, &string_binding, st);
    if (BAD_STATUS(st))
        return false;
    rpc_string_binding_parse(string_binding, NULL, &protseq,
                             &network_addr, NULL, NULL, st);
    rpc_string_free(&string_binding, &lst);
    if (BAD_STATUS(st)) {
        return false;
    }

    /* Address is local if its protseq and network_addr
     * match that of one of this server's bindings.
     */
    for (i = 0; i < rs_string_bindings->count; i++) {
        if (ustrcmp(protseq, rs_string_bindings->string_binding[i].protseq) == 0) {
            if (ustrcmp(network_addr, 
                    rs_string_bindings->string_binding[i].network_addr) == 0) {
                rpc_string_free(&protseq, &lst);
                rpc_string_free(&network_addr, &lst);
                return true;
            }
        }
    }
    rpc_string_free(&protseq, &lst);
    rpc_string_free(&network_addr, &lst);
    return false;
}

PRIVATE void rs_server_bindings_save_string_bindings 
  (
    rpc_binding_vector_p_t  bindings,
    error_status_t          *st
  )
{
    unsigned32          num_bindings;
    unsigned32          curr_binding;
    unsigned_char_p_t   string_binding;
    error_status_t      lst;

    num_bindings = rs_bindings->count;
    rs_string_bindings = (rs_string_binding_vec_p_t) malloc(
                            sizeof(rs_string_binding_vec_t) +
                            ((num_bindings - 1) * sizeof(rs_string_binding_t)));

    rs_string_bindings->count = 0; 
    for (curr_binding = 0; curr_binding < bindings->count; curr_binding++) {
        rpc_binding_to_string_binding(bindings->binding_h[curr_binding],
                                      &string_binding, st);
        if (BAD_STATUS(st)) return ;

        rpc_string_binding_parse(string_binding,
                 NULL,
                 &rs_string_bindings->string_binding[curr_binding].protseq,
                 &rs_string_bindings->string_binding[curr_binding].network_addr,
                 NULL,
                 NULL,
                 st);
        if (BAD_STATUS(st)) return ;

        (rs_string_bindings->count)++;

        rpc_string_free(&string_binding, &lst);
    }
}

PRIVATE void rs_server_bindings_to_twr_vector
  (
    rpc_binding_vector_p_t  bindings,
    rpc_if_handle_t         if_spec,
    rs_replica_twr_vec_p_t  *rep_twrs,
    error_status_t          *st
  )
{
    unsigned32              num_bindings;
    unsigned32              curr_binding;
    unsigned32              num_twrs;
    unsigned32              curr_twr;
    unsigned32              n; 
    rpc_binding_handle_t    tmp_binding_h;
    rpc_tower_vector_p_t    *twr_vec_ar;
    rs_replica_twr_vec_p_t  twr_vec;
    error_status_t          lst;

    *rep_twrs = NULL;

    if (bindings->count == 0) {
        SET_STATUS(st, sec_rgy_rep_bad_binding);
        return;
    }

    twr_vec = NULL;
    twr_vec_ar = NULL;
    tmp_binding_h = NULL;

    /* some bindings may yield more than one tower
     * so gather up ptrs to each binding's tower
     * vector in twr_vec_ar.  Also count the number
     * of towers.
     */
    num_bindings = bindings->count;
    twr_vec_ar = (rpc_tower_vector_p_t *) malloc(
        num_bindings * sizeof(rpc_tower_vector_p_t));

    for (curr_binding = 0; curr_binding < bindings->count; curr_binding++) {
        twr_vec_ar[curr_binding] = NULL;
    }

    num_twrs = 0;
    for (curr_binding = 0; curr_binding < bindings->count; curr_binding++) {

        /* copy the binding handle, clear its endpoint,
         * then generate the tower vector
         */
        rpc_binding_copy(bindings->binding_h[curr_binding], &tmp_binding_h, st);
        if (BAD_STATUS(st)) goto clean_err_return;
        rpc_binding_reset(tmp_binding_h, st);
        if (BAD_STATUS(st)) goto clean_err_return;

        rpc_tower_vector_from_binding(if_spec, tmp_binding_h, 
            &twr_vec_ar[curr_binding], st);
        if (BAD_STATUS(st)) goto clean_err_return;

        rpc_binding_free(&tmp_binding_h, &lst);

        num_twrs += twr_vec_ar[curr_binding]->count;
    }

    /* now allocate and fill the server's tower vector
     */
    twr_vec = (rs_replica_twr_vec_p_t) malloc(
                            sizeof(rs_replica_twr_vec_t) +
                            ((num_twrs - 1) * sizeof(twr_p_t)));

    curr_twr = 0;
    for (curr_binding = 0; curr_binding < bindings->count; curr_binding++) {

        if ((curr_twr + twr_vec_ar[curr_binding]->count) > num_twrs) {
            dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                           sec_s_rs_rpc,
                           svc_c_sev_error,
                           sec_s_pgmerr);
            SET_STATUS(st, sec_rgy_rep_bad_binding);
            goto clean_err_return;
        } 

        for (n = 0; n < twr_vec_ar[curr_binding]->count; n++, curr_twr++) {
            twr_vec->towers[curr_twr] = twr_vec_ar[curr_binding]->tower[n];
        }
    }

    /*  Small memory leak here.  Can't free the tower vectors
     *  that were returned by rpc_tower_vector_from_binding
     *  because twr_vec points to these towers.
     */
    free(twr_vec_ar);

    twr_vec->num_towers = num_twrs;
    *rep_twrs = twr_vec;
    return;


/* Problem
 * Free twr_vec_ar, the tower vectors to which it points,
 * twr_vec, and tmp_binding_h
 */

clean_err_return:

    if (twr_vec_ar != NULL) {
        for (n = 0; n < bindings->count; n++) {
            if (twr_vec_ar[n] != NULL)
                rpc_tower_vector_free(&twr_vec_ar[n], &lst);
        }
        free(twr_vec_ar);
    }

    if (twr_vec != NULL) {
        free(twr_vec);
    }

    if (tmp_binding_h != NULL) {
        rpc_binding_free(&tmp_binding_h, &lst);
    }

    return;
}


PRIVATE void rs_server_ep_register
  (
    error_status_t  *status
  )
{
    unsigned32              i;
    uuid_vector_t           secd_obj_uuids;
    unsigned_char_t         annotation[64];
    error_status_t          lst;

    CLEAR_STATUS(status);
    strcpy( (char *) annotation, "DCE user registry");

    secd_obj_uuids.count = 1;
    secd_obj_uuids.uuid[0] = &rs_state.cell_sec_id;

    /*
     * Register server's client interfaces with endpoint map
     */
    for (i = 0; rs_ifspecs_info[i].if_spec != NULL; i++) {
	/* 
         * the runtime won't let us register things
	 * in the endpoint map if they haven't already
	 * been registered locally.
	 */
	if (rs_ifspecs_info[i].registered) {
	    rpc_ep_register(*(rs_ifspecs_info[i].if_spec),
			    rs_bindings,
			    &secd_obj_uuids,
			    rs_ifspecs_info[i].annotation,
			    status);
	    if (BAD_STATUS(status)) return;
	} else {
	    /*
	     * if it isn't registered locally, it shouldn't
	     * be in the endpoint map either.
	     */
	    rpc_ep_unregister(*(rs_ifspecs_info[i].if_spec),
			      rs_bindings,
			      &secd_obj_uuids,
			      &lst);
	}
    }
}

/* NOTE WELL!
 *
 * Prior to the DCE 1.1 server migration work, server
 * if registration took place only at boot time while
 * secd was still single-threaded.  With the advent
 * of server migration, servers adjust their if
 * registrations dynamically when told
 * to move forward to a new software version.
 * Still, we have NOT added local concurrency
 * control around if registrations.  The caller
 * MUST ensure that only one thread is executing
 * the server if registration code.  Currently,
 * all calls to this routine are made under
 * (at least) a global read-intend-write lock.  
 */
PUBLIC void rs_server_register_if
  (
    unsigned32      compat_sw_rev,
    error_status_t  *status
  )
{
    int             i;
    error_status_t  lst;

    /* 
     * first check if compat software revision is valid
     * ( in bound ), if so, register the interface if
     * it had been not registered earlier.  If not in bound,
     * then unregister the interface if it had been registered.
     */
    for (i = 0; rs_ifspecs_info[i].if_spec != NULL; i++) {
	if ( compat_sw_rev >= rs_ifspecs_info[i].min_sw_rev
        && compat_sw_rev <= rs_ifspecs_info[i].max_sw_rev ) {
	    if ( ! rs_ifspecs_info[i].registered ) {
   	        rpc_server_register_if(*(rs_ifspecs_info[i].if_spec),
                               (uuid_t *) NULL,
                               (rpc_mgr_epv_t)(rs_mgrepvs[i]),
                               status);
        	if (BAD_STATUS(status)) return;
	    	rs_ifspecs_info[i].registered = true;
	    }
	} else if ( rs_ifspecs_info[i].registered ) {
	    rpc_server_unregister_if(*(rs_ifspecs_info[i].if_spec),
				     (uuid_t *) NULL,
				     &lst);
	    rs_ifspecs_info[i].registered = false;
	}
    }
    
    /*
     * Now that we've successfully registered with the
     * local runtime, register with the endpoint map as well.
     * The endpoint registration code keys on the value of
     * the "registered" boolean for each interface, which we set
     * to true or false in the local registration code above.
     */
    if (STATUS_OK(status)) {
	rs_server_ep_register(status);
    }
}

/* 
 * Called during startup to register server
 * interfaces with the rpc runtime and with
 * the endpoint mapper.  Also registers the
 * server's auth info.
 */
PUBLIC void rs_server_register
  (
    error_status_t  *status
  )
{
    unsigned32              i, compat_sw_rev;

    rs_master_info_get_sw_rev(&compat_sw_rev, status);
    if (BAD_STATUS(status)) return;

    rs_server_register_if(compat_sw_rev, status);

    /*
     * Register server's propagation interfaces 
     * with the rpc runtime and the endpoint mapper.
     */
    rs_server_register_prop_if(status);
    if (BAD_STATUS(status)) return;

    dce_svc_printf(SEC_RS_RPC_IF_REG_SUCC_MSG);
}


PUBLIC void rs_server_unregister(
    void
)
{
    uuid_vector_t       secd_obj_uuids;
    unsigned32          i;
    error_status_t      lst;

    secd_obj_uuids.count = 1;
    secd_obj_uuids.uuid[0] = &rs_state.cell_sec_id;

    /*  
     * Unregister interfaces that have been registered
     * with the local endpoint map.
     */
    for (i = 0; rs_ifspecs_info[i].if_spec != NULL; i++) {
        rpc_ep_unregister(*(rs_ifspecs_info[i].if_spec),
                        rs_bindings,
                        &secd_obj_uuids,
                        &lst);
    }

    /*
     * If server is a slave, unregister its propagation
     * interfaces from the local endpoint map.
     */
    if (!rs_state_master()) {
        rs_server_unregister_prop_if();
    }
    dce_svc_printf(SEC_RS_RPC_IF_UNREG_SUCC_MSG);
}


PUBLIC void rs_server_register_prop_if
  (
    error_status_t  *status
  )
{
    unsigned32      i;
    uuid_vector_t   secd_obj_uuids;
    unsigned_char_t annotation[64];

    for (i = 0; rs_prop_ifspecs[i] != NULL; i++) {
        rpc_server_register_if(*rs_prop_ifspecs[i],
                               (uuid_t *) NULL,
                               (rpc_mgr_epv_t) NULL,
                               status);
        if (BAD_STATUS(status)) return;
    }

    secd_obj_uuids.count = 1;
    secd_obj_uuids.uuid[0] = &rs_state.cell_sec_id;

    strcpy( (char *) annotation, "DCE user registry propagation interfaces");

    for (i = 0; rs_prop_ifspecs[i] != NULL; i++) {
        rpc_ep_register(*(rs_prop_ifspecs[i]),
                        rs_bindings,
                        &secd_obj_uuids,
                        annotation,
                        status);
        if (BAD_STATUS(status)) return;
    }
    dce_svc_printf(SEC_RS_RPC_PROPIF_REG_SUCC_MSG);
}

PUBLIC void rs_server_unregister_prop_if(
    void
)
{
    uuid_vector_t       secd_obj_uuids;
    unsigned32          i;
    error_status_t      lst;

    secd_obj_uuids.count = 1;
    secd_obj_uuids.uuid[0] = &rs_state.cell_sec_id;

    /*  
     * Unregister interfaces that have been registered
     * with the local endpoint map.
     */
    for (i = 0; rs_prop_ifspecs[i] != NULL; i++) {
        rpc_ep_unregister(*(rs_prop_ifspecs[i]),
                        rs_bindings,
                        &secd_obj_uuids,
                        &lst);
    }
    dce_svc_printf(SEC_RS_RPC_PROPIF_UNREG_SUCC_MSG);
}

PUBLIC void rs_start_sigcatch_task (
    void
)
{
    rs_task_handle_t    sigcatch_handle;

    if ( ! rs_task_create(&sigcatch_handle, start_sigcatch_task,
			  RS_SIGCATCH_THR, true, NULL) ) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_rpc,
                       svc_c_sev_error,
                       sec_rs_thr_exit_creat_fail);
    }

}

PRIVATE void *start_sigcatch_task (
    void  *arg
) 
{
    sigset_t            mask;                   /* signal values to wait for */
    int                 signo;
    error_status_t      lst, status;

    dce_svc_printf(SEC_RS_THR_STARTED_MSG, RS_SIGCATCH_THR);

    /*
     * Create the signal mask -- only the following signals will be caught.
     * See signal(5).
     */
    sigemptyset(&mask);
    sigaddset(&mask, SIGHUP);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGTERM);
#ifdef  _INCLUDE_POSIX_SOURCE
    sigaddset(&mask, SIGUSR1);
    sigaddset(&mask, SIGUSR2);
#endif  /* _INCLUDE_POSIX_SOURCE */
    sigprocmask(SIG_SETMASK, &mask, NULL);

    /*
     * Calling sigwait will cause this thread to block until the process
     * receives one of the signals in the mask.  If no threads were waiting
     * for these fatal asynchronous signals and such a signal were received,
     * the process would die immediately without giving the server a chance to
     * unregister its bindings with the endpoint mapper.  sigwait is the only
     * way to catch a fatal asynchronous signal and have the opportunity to
     * cleanup before exiting.
     */
    signo = sigwait(&mask);

    if (rs_state_exiting()) return;

    /* rs_shutdown checks the rs state
     */
    GLOBAL_RIW_LOCK {
	/* rs_audit_trail_log(h,
            "Stop Server", (char *) NULL);*/

        rs_rep_mgmt_shutdown(false, &status); /* false means checkpoint not required. */
        rpc_mgmt_stop_server_listening(NULL, &lst);
    } END_GLOBAL_RIW_LOCK;

}


PUBLIC void rs_start_kerberos (
    void
)
{ 
    rs_task_handle_t    kdc_handle;

    if (kdc_started) return;


    kdc_init((sizeof(kdc_argv)/sizeof(kdc_argv[0]))-1, kdc_argv);

    if( ! rs_task_create(&kdc_handle, start_kerberos_task,
                         RS_KDC_THR, false, NULL) ) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_rpc,
                       svc_c_sev_error,
                       sec_rs_thr_exit_creat_fail);
    }

    kdc_started = true;
}

PRIVATE void *start_kerberos_task
  (
    void    *arg
  )
{
    dce_svc_printf(SEC_RS_THR_STARTED_MSG,
                   RS_KDC_THR);

    /* no args */
    kdc_run((sizeof(kdc_argv)/sizeof(kdc_argv[0]))-1, kdc_argv);

    /* kdc_run never returns - on error it does an exit(errout).
       So a SVC messsage is not begin logged here */
}

PUBLIC void rs_setup_dce_rgy_identity
  (
    error_status_t      *status
  )
{
    CLEAR_STATUS(status);

    if (dce_rgy_identity_inited) return;

    /* Login as DCE rgy id for doing client operations
     * (eg. to nameservice)
     */
    rs_login_become_rgy(status);
    if (BAD_STATUS(status)) 
        return;

    dce_rgy_identity_inited = true;
}

