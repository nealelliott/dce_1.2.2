/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY 
 * $Log: rs_ns.c,v $
 * Revision 1.1.11.3  1996/02/18  00:18:40  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:07:18  marty]
 *
 * Revision 1.1.11.2  1995/12/08  17:53:25  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/07  20:05 UTC  psn
 * 	Fix parameter type for call to rs_task_join().
 * 
 * 	HP revision /main/HPDCE02/3  1995/08/21  15:33 UTC  burati
 * 	Merge status dereference fix from mb_mothra6 (CHFts15942/OT12980)
 * 
 * 	HP revision /main/HPDCE02/mb_mothra6/1  1995/08/15  22:41 UTC  burati
 * 	CHFts15942, 950802, [S:5,P:5] OT12980 secd chg_master to slave rtn not checking sts correctly
 * 
 * 	HP revision /main/HPDCE02/2  1995/06/15  19:36 UTC  rps
 * 	rs_ns_server_register: return a value
 * 	[1995/06/12  18:48 UTC  rps  /main/HPDCE02/rps_moth2/2]
 * 
 * 	[CHFts15428] rs_ns_calc_sleep_time: fix uninitialized variable
 * 	[1995/06/02  22:15 UTC  rps  /main/HPDCE02/rps_moth2/1]
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/15  21:08 UTC  greg
 * 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
 * 	[1995/04/21  18:58 UTC  greg  /main/SEC_migrate_merge/1]
 * 
 * 	Oops.
 * 	[1995/03/18  04:47 UTC  greg  /main/SEC_migrate/3]
 * 
 * 	Fixup namespace dynamically on software version changes.
 * 	[1995/03/18  01:25 UTC  greg  /main/SEC_migrate/2]
 * 
 * 	Merge .../greg_sec_migrate/LATEST --> .../SEC_migrate/LATEST.
 * 	[1995/02/11  22:14 UTC  greg  /main/SEC_migrate/1]
 * 
 * 	Incorportate  new software version model.  Register
 * 	version (1.2) of rs_repadm.idl in the namespace.
 * 	[1995/02/06  07:47 UTC  greg  /main/greg_sec_migrate/1]
 * 
 * Revision 1.1.6.6  1994/08/25  20:47:36  sekhar
 * 	Put back SVC messages for fatal_st() and error_st() calls.
 * 	[1994/08/25  20:20:02  sekhar]
 * 
 * Revision 1.1.6.5  1994/08/17  21:22:28  sekhar
 * 	[ OT 11712 ] illegal pointer usage in SVC message.
 * 	[1994/08/17  20:47:26  sekhar]
 * 
 * Revision 1.1.6.4  1994/08/12  17:37:51  mdf
 * 	     1994/08/11 sekhar
 * 	     [ OT 11608 ] until name service not configured, log messages
 * 	         as NOTICE_VERBOSE not ERROR .
 * 	[1994/08/12  14:15:53  mdf]
 * 
 * Revision 1.1.6.3  1994/08/04  16:13:43  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:11:48  mdf]
 * 
 * Revision 1.1.6.2  1994/06/02  21:22:43  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:19:02  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 	[1994/05/18  11:56:13  mdf]
 * 
 * Revision 1.1.2.14  1993/03/09  00:52:33  ahop
 * 	7446: remove no longer used sec-v1 group name defines
 * 	[1993/03/08  23:52:16  ahop]
 * 
 * 	compat: handle failure case of crash during update_namespace
 * 	[1993/03/08  23:30:06  ahop]
 * 
 * Revision 1.1.2.13  1993/02/26  23:52:35  emartin
 * 	em_bl6f: bump version of rs_bind
 * 	[1993/02/24  22:27:41  emartin]
 * 
 * Revision 1.1.2.12  1993/02/25  16:45:57  ahop
 * 	7184: support modification of namespace when 1.0.2 slave is
 * 	      started in what used to be a 1.0.1 cell
 * 	[1993/02/25  03:16:29  ahop]
 * 
 * Revision 1.1.2.11  1993/02/10  20:34:59  emartin
 * 	bl6e: group operations take group_name arg.
 * 	cell profile name and server's name are inited in rs_ns_server_names_init
 * 	group names (sec, sec-v1) are inited in rs_ns_server_group_names_init
 * 	only master adds itself to sec-v1 group
 * 	remove update interface registration by master
 * 	register secidmap interface first
 * 	bypass cache lookup when validating name
 * 	nuke rs_ns_group_lookup_ifspec
 * 	[1993/02/04  23:28:40  emartin]
 * 
 * Revision 1.1.2.10  1993/02/01  23:59:15  ahop
 * 	bl6e: in entry_lookup_rep_id, free returned attr_value
 * 	[1993/01/29  23:07:45  ahop]
 * 
 * Revision 1.1.2.9  1993/01/27  21:51:54  sommerfeld
 * 	Remove duplicate copy of rs_ns_server_register_wakeup.
 * 	[1993/01/27  21:25:03  sommerfeld]
 * 
 * Revision 1.1.2.8  1993/01/26  23:08:49  emartin
 * 	better printfs
 * 	[1993/01/18  22:04:21  emartin]
 * 
 * Revision 1.1.2.7  1992/12/29  16:36:01  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:24:50  zeliff]
 * 
 * Revision 1.1.2.6  1992/12/18  22:12:14  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:50:07  sekhar]
 * 
 * Revision 1.1.4.2  1992/12/11  22:35:16  emartin
 * 	bl6c: do rs_ns_server_register_wakeup
 * 
 * Revision 1.1.2.5  1992/11/04  19:36:55  ahop
 * 	Third replication drop: bl6a
 * 	rs_ns_entry_lookup_rep_id - don't free string returned by dns_attr_value_read
 * 	[1992/11/03  18:39:13  ahop]
 * 
 * Revision 1.1.2.4  1992/10/07  20:34:39  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:17:46  sekhar]
 * 
 * Revision 1.1.3.2  1992/10/06  17:20:38  burati
 * 	Second replication drop: bl5
 * 
 * Revision 1.1.2.3  1992/09/29  20:12:54  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:37:28  sekhar]
 * 
 * Revision 1.1.2.2  1992/08/31  18:25:25  sekhar
 * 	First replication code drop.
 * 	[1992/08/31  17:54:05  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:44:28  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
 * Copyright (c) Hewlett-Packard Company 1991, 1992, 1993, 1994
 * Unpublished work. All Rights Reserved.
 */
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
/*
 *      Registry Server - manage server's nameservice server entry
 *          and group entry
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = 
    "$Id: rs_ns.c,v 1.1.11.3 1996/02/18 00:18:40 marty Exp $";
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
#include <dce/binding.h>
#include <dce/rplbase.h>
#include <dce/dce_cf_const.h>
#include <rpriv.h>
#include <rpriv_1_0.h>
#include <rdaclif.h>
#include <rdaclifv0.h>
#include <rsecidmap.h>
#include <rs_bind.h>
#include <krb5rpc.h>

#include <rs_misc.h>
#include <rs_repadm.h>

#include <rs.h>
#include <rs_pvt.h>
#include <rs_ver.h>
#include <rs_ver_pvt.h>
#include <rs_state.h>
#include <rs_master.h>
#include <rs_base.h>
#include <rs_lock.h>
#include <rs_ch_lock.h>
#include <rs_util.h>
#include <rs_audit_trail.h>
#include <rs_task.h>

#include <rs_ns.h>
#include <rs_dns.h>

#include <rsdb_policy.h>
#include <rsdb_replica.h>

#include <sec_svc.h>

/* 
 * Local constants, typedefs, and variables
 */
#define rs_ns_c_one_minute          60
#define rs_ns_c_short_sleep_time    (rs_ns_c_one_minute * 5)
#define rs_ns_c_mid_sleep_time      (rs_ns_c_one_minute * 16)
#define rs_ns_c_long_sleep_time     (rs_ns_c_one_minute * 30)

/* define version-1 postfix for /.:/sec group name */
#define rs_ns_sec_group_postfix_v1  "-v1"


/*
 * rs_ns_server_register task's condition variable, handle,
 * and lock
 */
static pthread_cond_t      rs_ns_server_register_cv;
static rs_task_handle_t    rs_ns_server_register_task_handle;
static pthread_mutex_t     rs_ns_server_register_lock;

/* Security service profile and group names in the nameservice.
 * This server's name in the nameservice.
 */
static unsigned_char_p_t   rs_ns_sec_profile_name = NULL;
static unsigned_char_p_t   rs_ns_sec_group_name = NULL;
static unsigned_char_p_t   rs_ns_sec_group_name_v1 = NULL;
static unsigned_char_p_t   rs_ns_server_name = NULL;

/*
 * Attribute name (ie. Object ID) in ns of rep_id attribute 
 */
#define rs_ns_c_attrib_rep_id   ((unsigned_char_p_t) "1.3.22.1.5.1")

/* Interfaces to be registered with the nameservice.
 */
typedef struct rs_ns_ifspec_t {
    boolean32       registered;
    unsigned32      min_sw_rev;
    unsigned32      max_sw_rev;
    rpc_if_handle_t *ifspec;
} rs_ns_ifspec_t;

/*
 * offset of secidmap ifspec in rs_ns_ifspecs array
 */
#define rs_ns_c_ifspec_secidmap_offset    0
#define rs_ns_c_ifspec_rs_bind_offset     1
#define rs_ns_c_ifspec_rpriv_v1_0_offset  3
#define rs_ns_c_ifspec_rpriv_v1_1_offset  4

static rs_ns_ifspec_t  rs_ns_ifspecs[] = {
    { 
	false, 
	rs_c_default_min_sw_vers,
	rs_c_default_max_sw_vers,
	&secidmap_v1_0_s_ifspec 
    },
    { 
	false,
	rs_c_default_min_sw_vers,
	rs_c_default_max_sw_vers,
	&rs_bind_v2_0_s_ifspec
    },
    { 
	false,
	rs_c_default_min_sw_vers,
	rs_c_default_max_sw_vers,
	&rs_repadm_v1_2_s_ifspec
    },
    {
	false,
	rs_c_software_vers_1_0_2,
	rs_c_software_vers_1_0_2,
	&rpriv_v1_0_s_ifspec 
    },
    {
	false,
	rs_c_software_vers_1_1,
	rs_c_default_max_sw_vers,
	&rpriv_v1_1_s_ifspec 
    },
    {
	false,
	rs_c_default_min_sw_vers,
	rs_c_default_max_sw_vers,
	&rdaclif_v0_0_s_ifspec 
    },
    {
	false,
	rs_c_default_min_sw_vers,
	rs_c_default_max_sw_vers,
	&krb5rpc_v1_0_s_ifspec 
    },
    {
	false,
	rs_c_default_min_sw_vers,
	rs_c_default_max_sw_vers,
	&rdaclif_v1_0_s_ifspec 
    },
    {
	false,
	0,
	0,
	NULL 
    }
};

typedef struct rs_ns_uuid_vector_t { 
    unsigned32      count;
    uuid_t          uuid[1];
} rs_ns_uuid_vector_t, *rs_ns_uuid_vector_p_t;



/* 
 * static routine prototypes
 */

static void rs_ns_server_register_stop_task (
    void
);

static void *rs_ns_server_register (
    void
);

static void rs_ns_server_register_master (
    uuid_p_t                cell_sec_id,
    unsigned32              initial_sw_rev,
    rpc_binding_vector_p_t  svr_bindings
);

static void rs_ns_update_namespace (
    error_status_t      *st
);

static void rs_ns_server_init (
    error_status_t  *st
);

static void rs_ns_server_names_init (
    unsigned_char_p_t   cell_name,
    unsigned_char_p_t   server_name,
    error_status_t      *st
);

static void rs_ns_server_group_names_init (
    error_status_t  *st
);

static void read_group_name (
    unsigned_char_p_t   profile_name,
    rpc_if_id_t         *if_id,
    unsigned_char_p_t   *group_name,
    error_status_t      *st
);

static void rs_ns_register_sleep (
    signed32    sleep_secs
);

static void rs_ns_register_calc_and_sleep (
    signed32    *sleep_secs,
    signed32    max_sleep_secs
);

static boolean32 rs_ns_fatal_st (
    error_status_t  *st
);

static boolean32 rs_ns_server_dupe_found (
    uuid_p_t                cell_sec_id,
    uuid_p_t                rep_id,
    rpc_binding_vector_p_t  bindings,
    error_status_t          *st
);

static void rs_ns_entry_validate_and_init (
    unsigned_char_p_t       name,
    uuid_p_t                cell_sec_id,
    uuid_p_t                rep_id,
    rpc_binding_vector_p_t  svr_bindings,
    rpc_if_handle_t         ifspec,
    error_status_t          *st
);

static void rs_ns_entry_validate_is_mine (
    unsigned_char_p_t       name,
    uuid_p_t                cell_sec_id,
    uuid_p_t                rep_id,
    rs_ns_uuid_vector_p_t   entry_objs,
    uuid_p_t                entry_rep_id,
    rpc_binding_vector_p_t  entry_bindings,
    unsigned32              *nbindings_equal,
    error_status_t          *st
);

static void rs_ns_entry_lookup (
    unsigned_char_p_t       name,
    uuid_p_t                cell_sec_id,
    rpc_if_handle_t         ifspec,
    unsigned32              max_binding_count,
    uuid_t                  *entry_rep_id,
    rs_ns_uuid_vector_p_t   *entry_objs,
    rpc_binding_vector_p_t  *entry_bindings,
    error_status_t          *st
);

static void rs_ns_entry_add_ifspec (
    unsigned_char_p_t       name,
    uuid_p_t                obj_id,
    rpc_if_handle_t         ifspec,
    rpc_binding_vector_p_t  bindings,
    error_status_t          *st
);

static void rs_ns_entry_delete_ifspec (
    unsigned_char_p_t       name,
    rpc_if_handle_t         ifspec,
    error_status_t          *st
);

static void rs_ns_entry_delete_if_id (
    unsigned_char_p_t       name,
    rpc_if_id_p_t           if_id,
    error_status_t          *st
);

static void rs_ns_entry_lookup_ifspec (
    unsigned_char_p_t       name,
    rpc_if_handle_t         ifspec,
    uuid_p_t                obj_id,
    unsigned32              max_binding_count,
    rpc_binding_vector_p_t  *entry_bindings,
    error_status_t          *st
);

static void rs_ns_entry_chk_ifspecs (
    unsigned_char_p_t       name,
    error_status_t          *st
);

static void rs_ns_entry_lookup_objs (
    unsigned_char_p_t       name,
    rs_ns_uuid_vector_p_t   *entry_objs,
    error_status_t          *st
);

static void rs_ns_entry_add_rep_id (
    unsigned_char_p_t       name,
    uuid_p_t                rep_id,
    error_status_t          *st
);
 
static void rs_ns_entry_replace_rep_id (
    unsigned_char_p_t       name,
    uuid_p_t                rep_id,
    uuid_p_t                old_rep_id,
    error_status_t          *st
);

static void rs_ns_entry_lookup_rep_id (
    unsigned_char_p_t       name,
    uuid_t                  *rep_id,
    error_status_t          *st
);

static void rs_ns_group_add_member (
    unsigned_char_p_t       group_name,
    uuid_p_t                obj_id,
    unsigned_char_p_t       name,
    error_status_t          *st
);

static void rs_ns_group_delete_member (
    unsigned_char_p_t       group_name,
    unsigned_char_p_t       name,
    error_status_t          *st
);

static void rs_ns_chk_master_change (
    signed32       *sleep_time,        /* [in, out] */
    error_status_t *status             /* [out] */
);

static boolean32 rs_ns_register_if_all (
    rpc_binding_vector_p_t  svr_bindings,
    unsigned32              software_version
);

static void rs_ns_chk_sw_rev_change (
     unsigned32              initial_sw_rev,
     rpc_binding_vector_p_t  svr_bindings,
     signed32                *sleep_time,
     error_status_t          *st_p
);

static void rs_ns_calc_sleep_time (
     error_status_t  ns_op_status, /* [in] */
     signed32        *sleep_time  /* [in, out] */
);

/*
 * If rs_ns_bootstrap_enabled is true, don't backoff time
 * between retries to contact the nameservice.
 */
static boolean32       rs_ns_bootstrap_enabled = false;

void rs_ns_bootstrap_enable (
    void
)
{
    rs_ns_bootstrap_enabled = true;
}

void rs_ns_server_register_start_task
  (
    error_status_t  *st
  )
{
    rs_ns_server_init(st);
    if (BAD_STATUS(st)) return;

    if ( ! rs_task_create(&rs_ns_server_register_task_handle,
                          (rs_task_func_ptr_t) rs_ns_server_register,
                          RS_NSI_REGISTER_THR, true, NULL) ) {
        SET_STATUS(st, sec_s_pgmerr);
        return;
    }

    CLEAR_STATUS(st);
}

/*
 * send the rs_ns_server_register task a cancel
 * and then wait for the task to exit
 */
static void rs_ns_server_register_stop_task (
    void
)
{
    pthread_addr_t    join_st;

    if (pthread_cancel(rs_ns_server_register_task_handle) == -1)
        return;

    rs_task_join(rs_ns_server_register_task_handle, &join_st);
}


void rs_ns_server_register_wakeup (
    void
)
{
    pthread_cond_broadcast(&rs_ns_server_register_cv);
}

/*
 * Task to register the server with the nameservice 
 */
static void *rs_ns_server_register (
    void
)
{
    error_status_t          status;
    rpc_binding_vector_p_t  svr_bindings;
    signed32                sleep_time,
                            max_sleep_time;
    unsigned32              init_sw_rev;

    dce_svc_printf(SEC_RS_THR_STARTED_MSG , RS_NSI_REGISTER_THR);

    rs_master_info_get_sw_rev(&init_sw_rev, &status);
    if (BAD_STATUS(&status)) {
	/*
	 * If we can't determine our current sw rev,
	 * we're in deep, deep, trouble, so just
         * log a fatal exit through the serviceability
         * module.
	 */
	SEC_SVC_PRINT_MSG_ST(SEC_REP_CANT_GET_SW_REV_FATAL_MSG, status);
    }

    rs_server_bindings_inquire(&svr_bindings, &status);
    if (BAD_STATUS(&status))
        SEC_SVC_PRINT_MSG_ST(SEC_RGY_NS_SVR_GET_BINDING_MSG, status);

    sleep_time = rs_ns_c_one_minute;
    if (rs_ns_bootstrap_enabled)
        max_sleep_time = rs_ns_c_one_minute;
    else
        max_sleep_time = rs_ns_c_mid_sleep_time;


    /* task is single threaded so just need this
     * lock to keep pthread_cond_timedwait happy
     */
    pthread_mutex_lock(&rs_ns_server_register_lock);

TRY 

    /* 
     * Disable NS cache lookup while getting group name
     * from cell profile and validating the server's NS 
     * info.
     */
    rpc_ns_mgmt_set_exp_age(0, &status);

    /*
     * Initialize the server's copy of its security group names.
     * This requires a lookup in the cell profile.
     */
    while (true) {
        rs_ns_server_group_names_init(&status);
        if (GOOD_STATUS(&status)) break;
        if (rs_ns_fatal_st(&status))
            SEC_SVC_PRINT_MSG_ST(SEC_RGY_SVR_REGISTER_NS_MSG, status);

        rs_ns_register_calc_and_sleep(&sleep_time, max_sleep_time);
    }

    /* 
     * Validate and initialize this server's nameservice entry.
     *
     * Make sure that the nameservice entry registered under the server's
     * name is really this server's entry.
     * 
     * Create the server's ns entry if it doesn't exist.
     * Clean out the ns entry if too much has changed (eg.
     * the server's addresses or interfaces)
     *
     * Register the secidmap interface with the server's
     * current bindings.
     */
    while (true) {
        rs_ns_entry_validate_and_init(rs_ns_server_name, 
            &rs_state.cell_sec_id, &rs_state.rep_id, svr_bindings, 
            *rs_ns_ifspecs[rs_ns_c_ifspec_secidmap_offset].ifspec,
            &status);
        if (GOOD_STATUS(&status)) break;
        if (rs_ns_fatal_st(&status)) 
            SEC_SVC_PRINT_MSG_ST(SEC_RGY_SVR_REGISTER_NS_MSG, status);

        rs_ns_register_calc_and_sleep(&sleep_time, max_sleep_time);
    } 

    rs_ns_ifspecs[rs_ns_c_ifspec_secidmap_offset].registered = true;

    /*
     * Enable NS cache lookups
     */
    rpc_ns_mgmt_set_exp_age(rpc_c_ns_default_exp_age, &status);

    /* eg. if uninited, don't register any more interfaces yet
     */
    while (true) {
        rs_state_check(rs_c_event_read, true, &status);
        if (GOOD_STATUS(&status)) break;

        rs_ns_register_sleep(rs_ns_c_mid_sleep_time);
    }

    /* 
     * Register the server's generic interfaces and binding handles
     * in the nameservice
     */
    while (true) {
         if (rs_ns_register_if_all(svr_bindings, init_sw_rev)) {
	     /*
	      * All interfaces successfully registered
	      */
	     break;
	 }
        rs_ns_register_calc_and_sleep(&sleep_time, max_sleep_time);
    }

    /*
     * Add the server's name to the security service group name
     */
    if (rs_ns_sec_group_name != NULL) {

        while (true) {
            rs_ns_group_add_member(rs_ns_sec_group_name,
                &rs_state.cell_sec_id, rs_ns_server_name, 
                &status);
            if (GOOD_STATUS(&status)) break;
    
            rs_ns_register_calc_and_sleep(&sleep_time, max_sleep_time);
        }
    }

    /* 
     * If this is the master, add it to the rs_ns_sec_group_name_v1
     * group.  If this is a slave, remove it from this group.
     *
     * If this is the first 1.0.2 secd started in a 1.0.1 cell,
     * attempt to modify the namespace so that 1.0.1 clients
     * always bind to an update site and so that 1.0.2 client
     * binding is more efficient.
     *
     * Wait for a master change event when need to change
     * the server's registration
     */
    rs_ns_server_register_master(&rs_state.cell_sec_id, init_sw_rev, svr_bindings);


CATCH_ALL 

ENDTRY

    pthread_mutex_unlock(&rs_ns_server_register_lock);

    dce_svc_printf(SEC_RS_THR_EXITING_MSG , RS_NSI_REGISTER_THR);
    return(NULL);
}

static boolean32 rs_ns_register_if_all 
  (
    rpc_binding_vector_p_t  svr_bindings,
    unsigned32              software_version
  )
{
    boolean32       registered_all_ifs = true;
    unsigned32      i;
    error_status_t  status;

    for (i = 0; rs_ns_ifspecs[i].ifspec != NULL; i++) {
	if (rs_ns_ifspecs[i].registered) continue;

	if ((software_version < rs_ns_ifspecs[i].min_sw_rev)
        || (software_version > rs_ns_ifspecs[i].max_sw_rev)) {
	    continue;
	}

	rs_ns_entry_add_ifspec(rs_ns_server_name, &rs_state.cell_sec_id, 
			       *rs_ns_ifspecs[i].ifspec, svr_bindings, &status);
	
	if (GOOD_STATUS(&status)) {
	    rs_ns_ifspecs[i].registered = true;
	}
	else {
	    registered_all_ifs = false;
	} 
    }

    return registered_all_ifs;
}

/* 
 * If this is the master, add it to the rs_ns_sec_group_name_v1
 * group.  If this is a slave, remove it from this group.
 *
 * If this is the first 1.0.2 secd started in a 1.0.1 cell,
 * attempt to modify the namespace so that 1.0.1 clients
 * always bind to an update site and so that 1.0.2 client
 * binding is more efficient.
 *
 * Wait for a master change event when need to change
 * the server's registration
 */
static void rs_ns_server_register_master 
  (
    uuid_p_t                cell_sec_id,
    unsigned32              initial_sw_rev, 
    rpc_binding_vector_p_t  svr_bindings
  )
{
    signed32            sleep_time, namespace_sleep_time;
    unsigned32          current_sw_rev;
    error_status_t      status;
    
    CLEAR_STATUS(&status);

    namespace_sleep_time = rs_ns_c_long_sleep_time;

    while (true) {

        sleep_time = rs_ns_c_long_sleep_time;
	
        /* If required, modify the namespace (as inherited
         * from a 1.0.1 cell) for the 1.0.2 environment
         */
        if (rs_ns_sec_group_name == NULL) {
            rs_ns_update_namespace(&status);
        }
	
        /* 
         * Make sure that we're appropriately identified
	 * as a master or slave in the namespace.  We could
         * have woken up as result of a change_master
         * operation.
	 */
	if (STATUS_OK(&status)) {
	    rs_ns_chk_master_change(&sleep_time, &status);
	}

	/*
         * If we're the master and our software rev has changed since last
         * we awoke, update the namespace as necessary.
	 */
	if (STATUS_OK(&status)) {
	    rs_ns_chk_sw_rev_change(initial_sw_rev, svr_bindings, &sleep_time, &status);
	} 

	rs_ns_calc_sleep_time(status, &sleep_time);
	CLEAR_STATUS(&status);
        rs_ns_register_sleep(sleep_time);
    }
}

static void rs_ns_chk_sw_rev_change
  (
     unsigned32              initial_sw_rev,
     rpc_binding_vector_p_t  svr_bindings,     
     signed32                *sleep_time,
     error_status_t          *st_p
  )
{
    /*
     * First time into this routine, adopt the
     * initial software version as our base version.
     */
    static unsigned32  base_sw_rev = 0;
    unsigned32         current_sw_rev;
    boolean32          registered = true;
    rpc_if_id_t        rpriv_v1_0_if_id, rpriv_v1_1_if_id;

    if (base_sw_rev == 0) {
	base_sw_rev = initial_sw_rev;
    }

    rs_master_info_get_sw_rev(&current_sw_rev, st_p);
    if (BAD_STATUS(st_p)) {
	/*
	 * If we can't determine our current sw rev,
	 * we're in deep, deep, trouble, so just
         * log a fatal exit through the serviceability
         * module.
	 */
	SEC_SVC_PRINT_MSG_ST(SEC_REP_CANT_GET_SW_REV_FATAL_MSG, *st_p);
    }
    
    CLEAR_STATUS(st_p);

    if (current_sw_rev != base_sw_rev) {
	switch (current_sw_rev) {
	case rs_c_software_vers_1_1:

	    /* 
             * Need to ensure that
	     * 1. the 1.1 version of the rpriv interface is registered 
	     *    in the cell_profile and the 1.0 version is removed.
	     *    we do this only if we are the master. 
	     *
	     * 2. The correct versions of generic interfaces are registered in the
	     *     server entry.  Every replica must do this.
	     */
	    if (rs_state_master()) {

		/* 
		 * Lower priority of rpriv_v1_0 mapping in cell-profile 
		 */
		rpc_if_inq_id(*(rs_ns_ifspecs[rs_ns_c_ifspec_rpriv_v1_0_offset].ifspec), 
			      &rpriv_v1_0_if_id, st_p);
		rpc_ns_profile_elt_add(rpc_c_ns_syntax_dce, rs_ns_sec_profile_name,
				       &rpriv_v1_0_if_id, rpc_c_ns_syntax_dce,
				       rs_ns_sec_group_name, 1, 
				       (unsigned_char_p_t)"rpriv", st_p);
		if (BAD_STATUS(st_p)) {
		    dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
				   sec_s_rs_ns,
				   svc_c_sev_notice_verbose,
				   *st_p);
		    dce_svc_printf(SEC_RS_NS_PROF_ELT_ADD_FAIL_MSG,
				   rs_ns_sec_group_name);
		} else {
		    dce_svc_printf(SEC_RS_NS_PROF_ELT_ADD_SUCC_MSG,
				   rs_ns_sec_group_name);
		}
		
		if (STATUS_OK(st_p)) {
		    /* 
		     * Add rpriv_v1_1  mapping to cell-profile with high priority.
		     * duplicate profile_elt_add calls are successful
		     */
		    rpc_if_inq_id(*(rs_ns_ifspecs[rs_ns_c_ifspec_rpriv_v1_1_offset].ifspec), 
				  &rpriv_v1_1_if_id, st_p);
		    rpc_ns_profile_elt_add(rpc_c_ns_syntax_dce, rs_ns_sec_profile_name,
					   &rpriv_v1_1_if_id, rpc_c_ns_syntax_dce,
					   rs_ns_sec_group_name, 0, 
					   (unsigned_char_p_t)"rpriv", st_p);
		    if (BAD_STATUS(st_p)) {
			dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
				       sec_s_rs_ns,
				       svc_c_sev_notice_verbose,
				       *st_p);
			dce_svc_printf(SEC_RS_NS_PROF_ELT_ADD_FAIL_MSG,
				       rs_ns_sec_group_name_v1);
		    } else {
			dce_svc_printf(SEC_RS_NS_PROF_ELT_ADD_SUCC_MSG,
				       rs_ns_sec_group_name_v1);
		    }
		}
		if (STATUS_OK(st_p)) {
		    /* 
		     *  Remove rpriv_v1_0 mapping from cell_profile
		     */
		    rpc_ns_profile_elt_remove(rpc_c_ns_syntax_dce, rs_ns_sec_profile_name,
					      &rpriv_v1_0_if_id, rpc_c_ns_syntax_dce,
					      rs_ns_sec_group_name, st_p);
		    if (BAD_STATUS(st_p)) {
			dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
				       sec_s_rs_ns,
				       svc_c_sev_notice_verbose,
				       *st_p);
			dce_svc_printf(SEC_RS_NS_PROF_ELT_RM_FAIL_MSG,
				       rs_ns_sec_group_name);
		    } else {
			dce_svc_printf(SEC_RS_NS_PROF_ELT_RM_SUCC_MSG,
				       rs_ns_sec_group_name);
		    }
		}
	    } /* end if (rs_state_master()) */
	    
	    if (STATUS_OK(st_p)) {
		/* clean out any old, unsupported interfaces */
		rs_ns_entry_chk_ifspecs(rs_ns_server_name, st_p);
	    }

	    /* register any interfaces needed by the current software rev */
	    if (STATUS_OK(st_p)) {
		registered = rs_ns_register_if_all(svr_bindings, current_sw_rev);
	    }
	    break;
	    
	default:
	    break;
	}
    }

    if (STATUS_OK(st_p) && registered) {
	base_sw_rev = current_sw_rev;
    } else {
        /* 
	 * override caller's sleep time so we'll try again reasonably soon
	 */
	*sleep_time = rs_ns_c_short_sleep_time;
    }
}
	
static void rs_ns_calc_sleep_time 
  (
     error_status_t  ns_op_status, /* [in] */
     signed32        *sleep_time   /* [in, out] */
  )
{
    static   namespace_retries = 0;
    signed32 namespace_sleep_time = rs_ns_c_long_sleep_time;

    if (BAD_STATUS(&ns_op_status)) {
	if (namespace_retries < 3) {
	    namespace_sleep_time = rs_ns_c_short_sleep_time;
	}
	namespace_retries++;
    } else {
	namespace_retries = 0;
    }
    /*
     * Caller can override default namespace sleep
     * calculation by passing in a smaller interval
     * than what gets caclulated.
     */
    *sleep_time = MIN(*sleep_time, namespace_sleep_time);
}

/*
 * If we've changed from master to slave,
 * or vice versa, we need to tweak our namespace
 * registrations a bit.
 */
static void rs_ns_chk_master_change
  (
    signed32       *sleep_time,        /* [in, out] */
    error_status_t *status             /* [out] */
  )
{
    /*
     * First time into this routine, force an add
     * to the group if this is the master and
     * a remove from the group if this is a slave
     */
    static boolean32   first_entry = true;
    boolean32          server_registered;

    if (first_entry) {
	server_registered = rs_state_master() ? false:true;
    }

    if ((rs_state_master()) && (!server_registered)) {
	rs_ns_group_add_member(rs_ns_sec_group_name_v1,
			       &rs_state.cell_sec_id, rs_ns_server_name, 
			       status);
	if (GOOD_STATUS(status)) {
	    server_registered = true;
	} else {
	    *sleep_time = rs_ns_c_short_sleep_time;
	}
    } else if ((!rs_state_master()) && server_registered) {
	rs_ns_group_delete_member(rs_ns_sec_group_name_v1,
				  rs_ns_server_name, 
				  status);
	if (GOOD_STATUS(status)) {
	    server_registered = false;
	}
	else {
	    *sleep_time = rs_ns_c_short_sleep_time;
	}
    }
}

static void rs_ns_server_init
  (
    error_status_t  *st
  )
{
    CLEAR_STATUS(st);

    if (pthread_cond_init(&rs_ns_server_register_cv, 
            pthread_condattr_default) == -1) {
        SET_STATUS(st, sec_s_pgmerr);
        return;
    }

    if (pthread_mutex_init(&rs_ns_server_register_lock, 
            pthread_mutexattr_default) == -1) {
        SET_STATUS(st, sec_s_pgmerr);
        return;
    }
                                
    BEGIN_READ_EVENT(rs_c_event_primitive_read, lock_db, st) {

        rs_ns_server_names_init(NULL, NULL, st);

    } END_READ_EVENT;
}

/*
 * Init the local variables which contain the server's
 * nameservice entry name and the security service's
 * profile name.
 *
 * Caller should have a read lock.
 */
static void rs_ns_server_names_init
  (
    unsigned_char_p_t   cell_name,
    unsigned_char_p_t   server_name,
    error_status_t      *st
  )
{
    sec_rgy_properties_t    cell_properties;
    sec_rgy_name_t          my_name;
    unsigned_char_p_t       server_name_p,
                            cell_name_p;

    CLEAR_STATUS(st);

    cell_name_p = cell_name;
    server_name_p = server_name;

    if (cell_name_p == NULL) {
        rsdb_properties_get_info(&cell_properties, st);
        if (BAD_STATUS(st)) return;
        cell_name_p = (unsigned_char_p_t) cell_properties.realm;
    }

    if (rs_ns_sec_profile_name == NULL) {
        rs_ns_sec_profile_name = (unsigned_char_t *) malloc(sizeof(char) *
                                    (strlen((char *) cell_name_p) +
                                    CELL_PROFILE_LEN + 2));
        sprintf((char *)rs_ns_sec_profile_name, "%s/%s", cell_name_p,
                CELL_PROFILE_NAME);
    }

    if (rs_ns_server_name == NULL) {
        if (server_name_p == NULL) {
            rsdb_get_my_server_name(my_name);
            server_name_p = (unsigned_char_p_t) my_name;
        }

        rs_ns_server_name = (unsigned_char_t *) malloc(sizeof(char) *
                                    (strlen((char *) cell_name_p) +
                                     strlen((char *) server_name_p) + 2));
        sprintf((char *)rs_ns_server_name, "%s/%s", cell_name_p,
                (char *) server_name_p);
    }
}

/*
 *  r s _ n s _ u p d a t e _ n a m e s p a c e
 *
 * This should only be called by an initialized master or slave.
 *
 * Handle the special case of the first 1.0.2 secd
 * created in what used to be a 1.0.1 cell.
 * This case is detected by the absence of the
 * rs_bind -> sec group mapping in the cell-profile.
 * In this case:
 * 1. create the sec-v1 group
 * 2. put the master in the sec-v1 group
 * 3. lower the priority of the secidmap -> sec mapping in the cell-profile
 * 4. add the secidmap -> sec-v1 mapping with high priority
 * 5. remove the secidmap -> sec mapping from cell-profile
 * 6. add rs_bind -> sec mapping to cell-profile
 * 7. if I'm a slave, add me as member of /.:/sec group
 * 8. reinitialize the sec_group_name variables.
 * 
 */
static void rs_ns_update_namespace
  (
    error_status_t      *st_p
  )
{
    rpc_if_id_t             rs_bind_if_id, secidmap_if_id;
    dce_error_string_t      dce_error;
    error_status_t          lst;
    boolean32               master_known;
    uuid_t                  master_id;
    rs_update_seqno_t       master_seqno;
    rs_replica_name_p_t     master_name = 0;
    unsigned_char_p_t       global_master_name = 0;
    unsigned_char_p_t       master_name_p = 0;
    unsigned_char_p_t       save_sec_group_name = 0;
    unsigned_char_p_t       tmp_sec_group_name = 0;
    unsigned_char_p_t       tmp_sec_group_name_v1 = 0;
    signed32                name_len, v1_postfix_len;
    boolean32               sec_v1_exists = false;

    CLEAR_STATUS(st_p);

    if (rs_ns_sec_profile_name == NULL) {
        dce_svc_printf(SEC_RS_NS_NULL_PROFILE_MSG);
        SET_STATUS(st_p, sec_s_pgmerr);
        return;
    }
    if (rs_ns_sec_group_name_v1 == NULL) {
        dce_svc_printf(SEC_RS_NS_NULL_V1_GROUP_MSG);
        SET_STATUS(st_p, sec_s_pgmerr);
        return;
    }
    /* if rs_ns_sec_group_name already init-ed, work is done */
    if (rs_ns_sec_group_name != NULL) {
        return;
    }
    /*
     * Try to get the security group name associated w. the
     * rs_bind interface.  If no entry is found for this interface
     * modify the namespace to support pre-1.0.2 clients
     * who require update sites,
     */
    rpc_if_inq_id(*(rs_ns_ifspecs[rs_ns_c_ifspec_rs_bind_offset].ifspec), 
        &rs_bind_if_id, st_p);
    if (BAD_STATUS(st_p)) return;
    read_group_name(rs_ns_sec_profile_name, &rs_bind_if_id, 
                    &rs_ns_sec_group_name, st_p);
    if (GOOD_STATUS(st_p)) {
        return;
    } else if (BAD_STATUS(st_p) && !STATUS_EQUAL(st_p, rpc_s_no_more_elements)) {
        rs_ns_sec_group_name = 0;
        return;
    }
    /* We're here if the rs_bind mapping isn't stored in the cell-profile
     * indicating that the namespace changes haven't yet been made.
     */
    /* The security group name currently associated with the
     * secidmap i/f is stored in rs_ns_sec_group_name_v1.
     * Use it to initialize tmp group name variables. 
     */
    /* If we are here, in the normal case, we expect 
     * rs_ns_sec_group_name_v1 to be /.:/sec.
     * If secd crashed in this routine after adding the
     * secidmap->sec-v1 mapping, rs_ns_sec_group_name_v1 will be /.:/sec-v1.
     * Handle both cases.
     */
    v1_postfix_len = u_strlen(rs_ns_sec_group_postfix_v1);
    name_len = u_strlen(rs_ns_sec_group_name_v1);

    if(name_len > v1_postfix_len) {
        if(u_strcmp((rs_ns_sec_group_name_v1 + name_len - v1_postfix_len),
                     rs_ns_sec_group_postfix_v1) == 0) {
            sec_v1_exists = true;
        }
    }
    if(!sec_v1_exists) {
        name_len += v1_postfix_len;
    }
    if(!(tmp_sec_group_name = malloc(name_len + 1))) {
        SET_STATUS(st_p, sec_rgy_cant_allocate_memory);
        return;
    }
    if(!(tmp_sec_group_name_v1 = malloc(name_len + 1))) {
        SET_STATUS(st_p, sec_rgy_cant_allocate_memory);
        goto out;
    }
    u_strcpy(tmp_sec_group_name, rs_ns_sec_group_name_v1);
    u_strcpy(tmp_sec_group_name_v1, rs_ns_sec_group_name_v1);
    if(sec_v1_exists) {
        *(tmp_sec_group_name + name_len - v1_postfix_len) = '\0';
    } else {
        u_strcat(tmp_sec_group_name_v1, rs_ns_sec_group_postfix_v1);
    }
            

    /* Step 1. Create the /.:/sec-v1 group
     * Don't fail if it already exists.
     */
    rpc_ns_mgmt_entry_create(rpc_c_ns_syntax_dce, tmp_sec_group_name_v1, st_p);
                            
    if(STATUS_EQUAL(st_p, rpc_s_entry_already_exists)) {
        CLEAR_STATUS(st_p);
    }
    if(BAD_STATUS(st_p)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_ns,
                       svc_c_sev_notice_verbose,
                       *st_p);
        dce_svc_printf(SEC_RS_NS_GRP_ENT_CREATE_FAIL_MSG, 
                       tmp_sec_group_name_v1);
    } else {
        dce_svc_printf(SEC_RS_NS_GRP_ENT_CREATE_SUCC_MSG, 
                       tmp_sec_group_name_v1);
    }

    if(BAD_STATUS(st_p)) goto out;

    /*
     * Step 2. Add master secd as the only member of the sec-v1 group
     */
    if(rs_state_master()) {
        master_name_p = rs_ns_server_name;
    } else {
        /* I'm a slave, so lookup master name */
        BEGIN_READ(lock_replist, st_p) {
            rs_master_info_get(&master_known, &master_id, 
                               &master_seqno, st_p);
            if(GOOD_STATUS(st_p)) {
                /* don't forget to free master_name */
                rsdb_replica_get_name_by_uuid(&master_id, &master_name, st_p);
            }
        } END_READ
        if (BAD_STATUS(st_p)) goto out;
        if(!(global_master_name = malloc(CELL_DIR_ROOT_LEN + u_strlen(master_name) + 1))) {
            SET_STATUS(st_p, sec_rgy_cant_allocate_memory);
            goto out;
        }
        u_strcpy(global_master_name, CELL_DIR_ROOT);
        u_strcat(global_master_name+CELL_DIR_ROOT_LEN, master_name);
        master_name_p = global_master_name;
    }
    /* duplicate rpc_ns_group_mbr_add calls are successful */
    rs_ns_group_add_member(tmp_sec_group_name_v1, &rs_state.cell_sec_id,
                           master_name_p, st_p);
 
    if(BAD_STATUS(st_p)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_ns,
                       svc_c_sev_notice_verbose,
                       *st_p);
        dce_svc_printf(SEC_RS_NS_GRP_MBR_ADD_FAIL_MSG,
                       master_name_p,
                       tmp_sec_group_name_v1);
    } else {
        dce_svc_printf(SEC_RS_NS_GRP_MBR_ADD_SUCC_MSG,
                       master_name_p,
                       tmp_sec_group_name_v1);
    }

    if (BAD_STATUS(st_p)) goto out;
    /* 
     * Step 3. Lower priority of secidmap -> sec mapping in cell-profile 
     */
    rpc_if_inq_id(*(rs_ns_ifspecs[rs_ns_c_ifspec_secidmap_offset].ifspec), 
        &secidmap_if_id, st_p);
    if (BAD_STATUS(st_p)) goto out;
    rpc_ns_profile_elt_add(rpc_c_ns_syntax_dce, rs_ns_sec_profile_name,
                           &secidmap_if_id, rpc_c_ns_syntax_dce,
                           tmp_sec_group_name, 1, (unsigned_char_p_t)"secidmap", st_p);
    if(BAD_STATUS(st_p)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_ns,
                       svc_c_sev_notice_verbose,
                       *st_p);
        dce_svc_printf(SEC_RS_NS_PROF_ELT_ADD_FAIL_MSG,
                       tmp_sec_group_name);
    } else {
        dce_svc_printf(SEC_RS_NS_PROF_ELT_ADD_SUCC_MSG,
                       tmp_sec_group_name);
    }
    if (BAD_STATUS(st_p)) goto out;
    /* 
     * Step 4. Add secidmap -> sec-v1 mapping to cell-profile with high priority.
     * 1.0.1 clients will now bind to master-only sec-v1 group
     */
    /* duplicate profile_elt_add calls are successful */
    rpc_ns_profile_elt_add(rpc_c_ns_syntax_dce, rs_ns_sec_profile_name,
                           &secidmap_if_id, rpc_c_ns_syntax_dce,
                           tmp_sec_group_name_v1, 0, 
                           (unsigned_char_p_t)"secidmap", st_p);
    if(BAD_STATUS(st_p)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_ns,
                       svc_c_sev_notice_verbose,
                       *st_p);
        dce_svc_printf(SEC_RS_NS_PROF_ELT_ADD_FAIL_MSG,
                       tmp_sec_group_name_v1);
    } else {
        dce_svc_printf(SEC_RS_NS_PROF_ELT_ADD_SUCC_MSG,
                       tmp_sec_group_name_v1);
    }
    if (BAD_STATUS(st_p)) goto out;
    /* 
     * Step 5. Remove secidmap -> sec mapping from cell_profile
     */
    rpc_ns_profile_elt_remove(rpc_c_ns_syntax_dce, rs_ns_sec_profile_name,
                           &secidmap_if_id, rpc_c_ns_syntax_dce,
                           tmp_sec_group_name, st_p);
    if(BAD_STATUS(st_p)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_ns,
                       svc_c_sev_notice_verbose,
                       *st_p);
        dce_svc_printf(SEC_RS_NS_PROF_ELT_RM_FAIL_MSG,
                       tmp_sec_group_name);
    } else {
        dce_svc_printf(SEC_RS_NS_PROF_ELT_RM_SUCC_MSG,
                       tmp_sec_group_name);
    }
    if (BAD_STATUS(st_p)) goto out;
    /* 
     * Step 6. Add rs_bind -> sec mapping to cell-profile 
     */
    /* duplicate profile_elt_add calls are successful */
    rpc_ns_profile_elt_add(rpc_c_ns_syntax_dce, rs_ns_sec_profile_name,
                           &rs_bind_if_id, rpc_c_ns_syntax_dce,
                           tmp_sec_group_name, 0, (unsigned_char_p_t)"rs_bind", st_p);
    if(BAD_STATUS(st_p)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_ns,
                       svc_c_sev_notice_verbose,
                       *st_p);
        dce_svc_printf(SEC_RS_NS_PROF_ELT_ADD_FAIL_MSG,
                       tmp_sec_group_name);
    } else {
        dce_svc_printf(SEC_RS_NS_PROF_ELT_ADD_SUCC_MSG,
                       tmp_sec_group_name);
    }
    if (BAD_STATUS(st_p)) goto out;
    /*
     * Step 7. If I'm a slave, add me as member of /.:/sec group
     */
    if(!rs_state_master()) {
        rs_ns_group_add_member(tmp_sec_group_name, &rs_state.cell_sec_id,
                               rs_ns_server_name, st_p);
        if(BAD_STATUS(st_p)) {
            dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                           sec_s_rs_ns,
                           svc_c_sev_notice_verbose,
                           *st_p);
            dce_svc_printf(SEC_RS_NS_GRP_MBR_ADD_FAIL_MSG,
                           rs_ns_server_name,
                           tmp_sec_group_name);
        } else {
            dce_svc_printf(SEC_RS_NS_GRP_MBR_ADD_SUCC_MSG,
                           rs_ns_server_name,
                           tmp_sec_group_name);
        }
    }
    if (BAD_STATUS(st_p)) goto out;

    /*
     * Step 8. Initialize the sec_group_name variable
     */
    if(!(u_strcmp(rs_ns_sec_group_name_v1, tmp_sec_group_name_v1))) {
        /* rs_ns_sec_group_name_v1 already contains /.:/sec-v1.
         * Leave it alone and lookup rs_ns_sec_group_name.
         */
        read_group_name(rs_ns_sec_profile_name, &rs_bind_if_id, 
                        &rs_ns_sec_group_name, st_p);
        if (BAD_STATUS(st_p)) {
            rs_ns_sec_group_name = 0;
            goto out;
        }
    } else {
        /* rs_ns_sec_group_name_v1 contains /.:/sec.
         * - Copy the group_name_v1 to group_name.
         * - Read the new group_name_v1 from NSI.
         */
        save_sec_group_name = rs_ns_sec_group_name_v1;
        rs_ns_sec_group_name_v1 = 0;
        /* Attempt to read group_name_v1 */
        read_group_name(rs_ns_sec_profile_name, &secidmap_if_id, 
                        &rs_ns_sec_group_name_v1, st_p);
        if (BAD_STATUS(st_p)) {
            rs_ns_sec_group_name_v1 = save_sec_group_name;
            goto out;
        }
        rs_ns_sec_group_name = save_sec_group_name;
    }

out:
    if(tmp_sec_group_name)
        free(tmp_sec_group_name);
    if(tmp_sec_group_name_v1)
        free(tmp_sec_group_name_v1);
    if(master_name)
        free(master_name);
    if(global_master_name)
        free(global_master_name);
}


/*
 * Init the local variables which contain 
 * the security service's group names.
 * rs_ns_server_names_init must be called before
 * this routine to init rs_ns_sec_profile_name.
 */
static void rs_ns_server_group_names_init
  (
    error_status_t      *st
  )
{
    rpc_if_id_t             if_id;

    CLEAR_STATUS(st);

    if (rs_ns_sec_profile_name == NULL) {
        dce_svc_printf(SEC_RS_NS_NULL_PROFILE_MSG);
        SET_STATUS(st, sec_s_pgmerr);
        return;
    }

    if (rs_ns_sec_group_name_v1 == NULL) {
        rpc_if_inq_id(*(rs_ns_ifspecs[rs_ns_c_ifspec_secidmap_offset].ifspec), 
            &if_id, st);
        if (BAD_STATUS(st)) return;
    
        read_group_name(rs_ns_sec_profile_name, &if_id, 
            &rs_ns_sec_group_name_v1, st);
        if (BAD_STATUS(st)) return;
    }

    /*
     * Try to get the security group name associated w. the
     * rs_bind interface.  If no entry is found for this interface,
     * ignore the problem; it's expected in 1.0.2 sites 
     * which have evolved from 1.0.1 sites.
     */
    if (rs_ns_sec_group_name == NULL) {
        rpc_if_inq_id(*(rs_ns_ifspecs[rs_ns_c_ifspec_rs_bind_offset].ifspec), 
            &if_id, st);
        if (BAD_STATUS(st)) return;
    
        read_group_name(rs_ns_sec_profile_name, &if_id, 
            &rs_ns_sec_group_name, st);
        if (BAD_STATUS(st)) {
            if (STATUS_EQUAL(st, rpc_s_no_more_elements)) {
                CLEAR_STATUS(st);
            }
            return;
        }
    }
}

/* Open the Cell Profile and attempt to retrieve the catalog point
 * (general security service name) by matching the interface uuid. 
 * The catalog point for the security service must have been added
 * to the Cell Profile prior to this point (ie: at Big Bang).
 */
static void read_group_name
  (
    unsigned_char_p_t   profile_name,
    rpc_if_id_t         *if_id,
    unsigned_char_p_t   *group_name,
    error_status_t      *st
  )
{
    rpc_ns_inq_handle_t     profile_context;
    unsigned32              priority;
    dce_error_string_t      dce_error;
    error_status_t          lst;

    CLEAR_STATUS(st);

    rpc_ns_profile_elt_inq_begin(rpc_c_ns_syntax_dce,
                                    (unsigned_char_p_t) profile_name,
                                    rpc_c_profile_match_by_if,
                                    if_id,
                                    rpc_c_vers_exact,
                                    rpc_c_ns_syntax_dce,
                                    NULL,
                                    &profile_context,
                                    st);
        
    if (GOOD_STATUS(st)) {
        rpc_ns_profile_elt_inq_next(profile_context,
                                        if_id,
                                        group_name,
                                        &priority,
                                        NULL,
                                        st);
        rpc_ns_profile_elt_inq_done(&profile_context,
                                       &lst);
    }
        
    if (BAD_STATUS(st)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_ns,
                       svc_c_sev_notice_verbose,
                       *st);
        dce_svc_printf(SEC_RS_NS_PROF_ELT_INQ_FAIL_MSG);
    }
    else {
        dce_svc_printf(SEC_RS_NS_PROF_ELT_INQ_SUCC_MSG,
                       (char *) (*group_name),
                       (char *) profile_name);
    }
}

static void rs_ns_register_sleep
  (
    signed32    sleep_secs
  )
{
    struct timeval      time_tv;
    struct timezone     time_tz;
    struct timespec     time_ts;

#ifdef SNI_SVR4_POSIX
    gettimeofday(&time_tv);
#else
    gettimeofday(&time_tv, &time_tz);
#endif
    time_ts.tv_sec = time_tv.tv_sec + sleep_secs;
    time_ts.tv_nsec = 0;

    /* release bogus_mutex and wait for rs_ns event or timeout */
    pthread_cond_timedwait(&rs_ns_server_register_cv, 
        &rs_ns_server_register_lock, &time_ts);
}

static void rs_ns_register_calc_and_sleep
  (
    signed32    *sleep_secs,
    signed32    max_sleep_secs
  )
{
    if (*sleep_secs > max_sleep_secs)
        *sleep_secs = max_sleep_secs;

    rs_ns_register_sleep(*sleep_secs);

    if (*sleep_secs < max_sleep_secs)
        *sleep_secs = (*sleep_secs) * 2;
}

/* 
 * rs_ns_fatal_st
 *
 * return true if the error in "st" is a
 * fatal nameservice error for the security
 * server.
 */
static boolean32 rs_ns_fatal_st
  (
    error_status_t  *st
  )
{
    if (STATUS_EQUAL(st, sec_s_invalid_nameservice_entry))
        return true;
    else
        return false;
}


/*
 * Delete the server's server entry and
 * group entry from the nameservice.
 *
 * Stop the rs_ns_server_register task
 * before deleting the names from the
 * nameservice (it will be trying to add
 * the names).
 */
void rs_ns_server_delete_name
  (
    unsigned_char_p_t   name,
    error_status_t      *st
  )
{
    error_status_t  lst;

    rs_ns_server_register_stop_task();

    rs_ns_entry_delete(name, st);
    if (BAD_STATUS(st)) return;
    rs_ns_group_delete_member(rs_ns_sec_group_name, name, st);
    if (BAD_STATUS(st)) return;

    dce_svc_printf(SEC_RS_NS_NAME_DEL_SUCC_MSG,
                   (char *) name, 
                   rs_ns_sec_group_name);
}

/*
 * Change the server's name in the name service.
 *
 * Invoked at startup time before the rs_ns_server_register
 * task has started.
 */
void rs_ns_server_change_name
  (
    unsigned_char_p_t       old_name,
    unsigned_char_p_t       new_name,
    uuid_p_t                cell_sec_id,
    uuid_p_t                rep_id,
    rpc_binding_vector_p_t  svr_bindings,
    error_status_t          *st
  )
{
    uuid_t          entry_rep_id;
    error_status_t  lst;

    /* Check that it's ok to use new_name.
     * If it is ok, put this server's replica
     * instance uuid, secidmap interface and bindings
     * and the cell security object uuid in ns entry 
     * "new_name"
     */
    rs_ns_entry_validate_and_init(new_name,
        cell_sec_id, rep_id, svr_bindings, 
        *rs_ns_ifspecs[rs_ns_c_ifspec_secidmap_offset].ifspec,
        st);
    if (BAD_STATUS(st)) return;

    /* add new_name to the security service group
     */
    rs_ns_group_add_member(rs_ns_sec_group_name,
        cell_sec_id, new_name, st);
    if (BAD_STATUS(st)) return;

    /* remove old_name server entry and group entry
     * from nameservice (if it's this server's entry)
     */
    rs_ns_entry_lookup_rep_id(old_name, &entry_rep_id, &lst);
    if (BAD_STATUS(&lst)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_ns,
                       svc_c_sev_error,
                       lst);
        dce_svc_printf(SEC_RS_NS_CANT_RM_NAME_MSG, old_name);
        return;
    }

    if (!uuid_equal(rep_id, &entry_rep_id, &lst)) {
        dce_svc_printf(SEC_RS_NS_NAME_NOT_REMOVED_MSG);
        return;
    }

    rs_ns_entry_delete(old_name, &lst);
    if (BAD_STATUS(&lst)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_ns,
                       svc_c_sev_error,
                       lst);
        dce_svc_printf(SEC_RS_NS_CANT_RM_NAME_MSG, old_name);
        return;
    }

    rs_ns_group_delete_member(rs_ns_sec_group_name,
        old_name, &lst);
    if (BAD_STATUS(&lst)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_ns,
                       svc_c_sev_error,
                       lst);
        dce_svc_printf(SEC_RS_NS_CANT_RM_MEMBER_MSG,
                       old_name, rs_ns_sec_group_name);
        return;
    }
}


/*
 * Try to talk to the server at these
 * bindings 
 ************ temp stub routine **************
 */
static boolean32 rs_ns_server_dupe_found
  (
    uuid_p_t                cell_sec_id,
    uuid_p_t                rep_id,
    rpc_binding_vector_p_t  bindings,
    error_status_t          *st
  )
{
    CLEAR_STATUS(st);

    return false;
}

/*
 * Given a cell name and a cell relative name (w/out any
 * prefix) compose a full name.
 */
void rs_ns_name_compose
  (
    unsigned_char_p_t   cell_name,
    unsigned_char_p_t   name,
    unsigned_char_p_t   *full_name,
    error_status_t      *st
  )
{
    *full_name = (unsigned_char_t *) malloc(sizeof(char) *
                                (strlen((char *) cell_name) +
                                 strlen((char *) name) + 2));
    sprintf((char *)(*full_name), "%s/%s", cell_name,
            (char *) name);
}

/* 
 * Validate that it's ok for this server to use "name"
 * as its nameservice entry
 */
void rs_ns_entry_validate
  (
    unsigned_char_p_t       name,
    uuid_p_t                cell_sec_id,
    uuid_p_t                rep_id,
    rpc_binding_vector_p_t  svr_bindings,
    rpc_if_handle_t         ifspec,
    error_status_t          *st
  )
{
    rs_ns_uuid_vector_p_t   entry_objs;
    rpc_binding_vector_p_t  entry_bindings;
    uuid_t                  entry_rep_id;
    unsigned32              nbindings_equal;
    error_status_t          lst;

    entry_objs = NULL;
    entry_bindings = NULL;

    /* get the nameservice entry's data for this name
     */
    rs_ns_entry_lookup(name, cell_sec_id, ifspec, (svr_bindings->count + 30),
        &entry_rep_id, &entry_objs, &entry_bindings, st);

    if (STATUS_EQUAL(st, rpc_s_entry_not_found)) {
        /* Nameservice entry doesn't exist
         */
        CLEAR_STATUS(st);
        return;
    }
    if (BAD_STATUS(st)) return;

    /*
     * make sure this isn't someone else's entry
     */
    rs_ns_entry_validate_is_mine(name, cell_sec_id, rep_id, 
        entry_objs, &entry_rep_id, entry_bindings, &nbindings_equal, st);
    if (BAD_STATUS(st)) goto clean_return;

clean_return:
    if (entry_objs != NULL)
        free(entry_objs);
    if (entry_bindings != NULL) 
        rpc_binding_vector_free(&entry_bindings, &lst);
}

/*
 * Validate that nameservice entry "name" is this server's entry.
 *
 * If the ns entry is not this server's entry, error exit.
 *
 * If no entry exists for this server, create it.
 *
 * If the ns entry is this server's entry but there is some basic
 * problem (eg the rep_id is incorrect or the bindings in the 
 * ns entry are different from the server's current addresses) 
 * clean up the problem.
 *
 * Finally, add the "ifspec" interface and current server bindings
 * to the ns entry.
 */

static void rs_ns_entry_validate_and_init
  (
    unsigned_char_p_t       name,
    uuid_p_t                cell_sec_id,
    uuid_p_t                rep_id,
    rpc_binding_vector_p_t  svr_bindings,
    rpc_if_handle_t         ifspec,
    error_status_t          *st
  )
{
    rs_ns_uuid_vector_p_t   entry_objs;
    rpc_binding_vector_p_t  entry_bindings;
    uuid_t                  entry_rep_id;
    unsigned32              nbindings_equal;
    error_status_t          lst;

    entry_objs = NULL;
    entry_bindings = NULL;

    /* get the nameservice entry's data for this name
     */
    rs_ns_entry_lookup(name, cell_sec_id, ifspec, (svr_bindings->count + 30),
        &entry_rep_id, &entry_objs, &entry_bindings, st);

    if (STATUS_EQUAL(st, rpc_s_entry_not_found)) {
        /* Nameservice entry doesn't exist, create it
         */
        rs_ns_entry_add(name, cell_sec_id, rep_id, 
            ifspec, svr_bindings, st);
        return;
    }
    if (BAD_STATUS(st)) return;

    /*
     * make sure this isn't someone else's entry
     */
    rs_ns_entry_validate_is_mine(name, cell_sec_id, rep_id, 
        entry_objs, &entry_rep_id, entry_bindings, &nbindings_equal, st);
    if (BAD_STATUS(st)) goto clean_return;

    /*
     * if any obsolete bindings in the entry, just
     * replace entry (via delete and add)
     */
    if ((entry_bindings != NULL) &&
        (entry_bindings->count != nbindings_equal)) {
        rs_ns_entry_delete(name, st);
        if (BAD_STATUS(st)) goto clean_return;

        rs_ns_entry_add(name, cell_sec_id, rep_id, 
            ifspec, svr_bindings, st);
        return;
    }

    /*
     * if necessary, fix entry's rep_id field
     */
    if (!uuid_equal(rep_id, &entry_rep_id, &lst)) {
        if (uuid_is_nil(&entry_rep_id, &lst)) {
            rs_ns_entry_add_rep_id(name, rep_id, st);
        }
        else {
            rs_ns_entry_replace_rep_id(name, rep_id, 
                &entry_rep_id, st);
        }
        if (BAD_STATUS(st)) goto clean_return;
    }

    /* clean out any old, unsupported interfaces
     */
    rs_ns_entry_chk_ifspecs(name, &lst);

    /* finally, register "ifspec" interface w. latest svr_bindings.
     * It's my understanding that CDS will just ignore an update
     * when all fields match what's already in the entry.
     */
    rs_ns_entry_add_ifspec(name, cell_sec_id, ifspec, svr_bindings, st);
    if (BAD_STATUS(st)) goto clean_return;

clean_return:
    if (entry_objs != NULL)
        free(entry_objs);
    if (entry_bindings != NULL) 
        rpc_binding_vector_free(&entry_bindings, &lst);
}

/*
 * Make sure a name service entry is this server's 
 * entry.  Return the number of entry bindings
 * which match the server's current bindings in 
 * "nbindings_equal" 
 *
 * The caller will fixup any discrepancies in the 
 * entry.
 */
static void rs_ns_entry_validate_is_mine
  (
    unsigned_char_p_t       name,
    uuid_p_t                cell_sec_id,
    uuid_p_t                rep_id,
    rs_ns_uuid_vector_p_t   entry_objs,
    uuid_p_t                entry_rep_id,
    rpc_binding_vector_p_t  entry_bindings,
    unsigned32              *nbindings_equal,
    error_status_t          *st
  )
{
    error_status_t      lst;
    boolean32           cell_sec_id_found;
    unsigned32          i;

    CLEAR_STATUS(st);
    *nbindings_equal = 0;

    /* chk entry's cell_sec_id
     * if there are no object ids in entry, 
     * assume it's an empty entry for now.
     */
    cell_sec_id_found = false;

    if (entry_objs != NULL) {
        for (i = 0; i < entry_objs->count; i++) {
            if (uuid_equal(cell_sec_id, &entry_objs->uuid[i], &lst)) {
                cell_sec_id_found = true;
                break;
            }
        }
    
        if (!cell_sec_id_found) {
            /**** SVC_OPEN : Turn this into a debug statement
             *
             *    error("Cell security object id not in nameservice entry");
             */
             dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                            sec_s_rs_ns,
                            svc_c_sev_error,
                            sec_s_invalid_nameservice_entry);
            *st = sec_s_invalid_nameservice_entry;
            return;
        }
    }

    /*
     * get number of bindings in entry that
     * match server's current bindings 
     * (actually match by network address)
     */
    if (entry_bindings != NULL) {
        rs_server_bindings_naddr_equal(entry_bindings, nbindings_equal, st);
        if (BAD_STATUS(st)) return;
    }

    if (!uuid_equal(rep_id, entry_rep_id, &lst)) {
        /*
         * ns entry's rep_id is not the same as the server's rep_id.
         *
         * If the ns entry rep_id is nil (ie. not present) and there 
         * are no bindings registered, assume this is just our 
         * incomplete entry.
         *
         * If the entry's bindings match the server's addresses and
         * our cell_sec_id is in the entry, this is our incomplete 
         * entry (if entry_rep_id is nil) or our replica is being 
         * recreated.  (NB: earlier logic should have verified that
         * no database exists at this host.)
         */
        if ((entry_bindings == NULL) && 
            uuid_is_nil(entry_rep_id, &lst)) { 
                return;
        }
        if ((cell_sec_id_found) &&
            (entry_bindings != NULL) &&
             (*nbindings_equal == entry_bindings->count) ) {
                return;
        }
        
        /**** SVC_OPEN : Turn this into a debug statement
         *
         *    error("server's instance id in nameservice entry is incorrect");
         */
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_ns,
                       svc_c_sev_error,
                       sec_s_invalid_nameservice_entry);
        *st = sec_s_invalid_nameservice_entry;
        return;
    }

    /* 
     * chk entry's bindings/addresses. 
     * host's network addresses could have changed or
     * dbase could have been copied; latter is a fatal 
     * error if we can communicate with other incarnation
     */
    if ((entry_bindings != NULL) && (*nbindings_equal == 0)) {
            /* None of entry's bindings match server's current
             * addresses.
             * If can communicate with a server at one of
             * the ns entry's addresses, assume the dbase 
             * has been copied
             */
        if (rs_ns_server_dupe_found(cell_sec_id, rep_id, entry_bindings, &lst)) {
             /**** SVC_OPEN : Turn this into a debug statement
              *
              *    error("Another server is running with same identity at another host");
              *    error("(perhaps database was copied)");
              *
              */
            dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                           sec_s_rs_ns,
                           svc_c_sev_error,
                           sec_s_invalid_nameservice_entry);
            *st = sec_s_invalid_nameservice_entry;
            return;
        }
    }
}

/*
 * Add a server entry to the nameservice.
 */
void rs_ns_entry_add
  (
    unsigned_char_p_t       name,
    uuid_p_t                cell_sec_id,
    uuid_p_t                rep_id,
    rpc_if_handle_t         ifspec,
    rpc_binding_vector_p_t  bindings,
    error_status_t          *st
  )
{
    rs_ns_entry_add_ifspec(name, cell_sec_id, ifspec, bindings, st);
    if (BAD_STATUS(st)) return;

    rs_ns_entry_add_rep_id(name, rep_id, st);
    if (BAD_STATUS(st)) return;
}

/*
 * Remove a server entry from the nameservice
 */
void rs_ns_entry_delete
  (
    unsigned_char_p_t       name,
    error_status_t          *st
  )
{
    rpc_ns_mgmt_entry_delete(rpc_c_ns_syntax_dce, name, st);
    if (BAD_STATUS(st)) return;
}

/*
 * Return nameservice entry "name's" object uuids,
 * bindings, and rep_id object uuid
 */
static void rs_ns_entry_lookup
  (
    unsigned_char_p_t       name,
    uuid_p_t                cell_sec_id,
    rpc_if_handle_t         ifspec,
    unsigned32              max_binding_count,
    uuid_t                  *entry_rep_id,
    rs_ns_uuid_vector_p_t   *entry_objs,
    rpc_binding_vector_p_t  *entry_bindings,
    error_status_t          *st
  )
{
    uuid_p_t        obj_id;
    uuid_t          tmp_obj_id;
    error_status_t  lst;

    rs_ns_entry_lookup_rep_id(name, entry_rep_id, st);
    if (BAD_STATUS(st)) {
        /* if rep_id not found in entry, set
         * entry_rep_id to uuid_nil and
         * carry on
         */
        if (STATUS_EQUAL(st, rpc_s_no_more_members))
            uuid_create_nil(entry_rep_id, &lst);
        else
            return;
    }

    rs_ns_entry_lookup_objs(name, entry_objs, st);
    if (BAD_STATUS(st)) return;

    /*
     * if there aren't any objects in the entry
     * use object uuid nil to lookup the 
     * entry_bindings
     */
    if (*entry_objs == NULL) {
        uuid_create_nil(&tmp_obj_id, &lst);
        obj_id = &tmp_obj_id;
    }
    else {
        obj_id = cell_sec_id;
    }

    rs_ns_entry_lookup_ifspec(name, ifspec, obj_id, max_binding_count,
        entry_bindings, st);
    if (BAD_STATUS(st)) return;
}

static void rs_ns_entry_add_ifspec
  (
    unsigned_char_p_t       name,
    uuid_p_t                obj_id,
    rpc_if_handle_t         ifspec,
    rpc_binding_vector_p_t  bindings,
    error_status_t          *st
  )
{
    error_status_t          lst;
    dce_error_string_t      dce_error;
    uuid_vector_t           obj_uuids;

    obj_uuids.count = 1;
    obj_uuids.uuid[0] = obj_id;

    dce_svc_printf(SEC_RS_NS_BIND_EXPORT_MSG, (char *) name);

    rpc_ns_binding_export(rpc_c_ns_syntax_dce, name,
        ifspec, bindings, &obj_uuids, st);

    if (BAD_STATUS(st)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_ns,
                       svc_c_sev_notice_verbose,
                       *st);
        return;
    }
    dce_svc_printf(SEC_RS_NS_BIND_EXPORT_SUCC_MSG, (char *) name);
}

/*
 * Remove an interface from nameservice entry "name".
 * The interface is specified by an rpc_if_handle_t
 */
static void rs_ns_entry_delete_ifspec
  (
    unsigned_char_p_t       name,
    rpc_if_handle_t         ifspec,
    error_status_t          *st
  )
{
    rpc_ns_binding_unexport(rpc_c_ns_syntax_dce, name, 
        ifspec, NULL, st);

    if (STATUS_EQUAL(st, rpc_s_interface_not_found))
        CLEAR_STATUS(st);
}

/*
 * Remove an interface from nameservice entry "name".
 * The interface is specified by an rpc_if_id_p_t.
 */
static void rs_ns_entry_delete_if_id
  (
    unsigned_char_p_t       name,
    rpc_if_id_p_t           if_id,
    error_status_t          *st
  )
{
    rpc_ns_mgmt_binding_unexport(rpc_c_ns_syntax_dce, name, 
        if_id, rpc_c_vers_exact, NULL, st);

    if (STATUS_EQUAL(st, rpc_s_interface_not_found))
        CLEAR_STATUS(st);
}

static void rs_ns_entry_lookup_ifspec
  (
    unsigned_char_p_t       name,
    rpc_if_handle_t         ifspec,
    uuid_p_t                obj_id,
    unsigned32              max_binding_count,
    rpc_binding_vector_p_t  *entry_bindings,
    error_status_t          *st
  )
{ 
    rpc_ns_handle_t lookup_h;
    error_status_t  lst;

    *entry_bindings = NULL;

    rpc_ns_binding_lookup_begin(rpc_c_ns_syntax_dce, 
        name, ifspec, obj_id, max_binding_count, 
        &lookup_h, st);
    if (BAD_STATUS(st)) return;

    rpc_ns_binding_lookup_next(lookup_h,
        entry_bindings, st);

    if (STATUS_EQUAL(st, rpc_s_no_more_bindings))
        CLEAR_STATUS(st);

    rpc_ns_binding_lookup_done(&lookup_h, &lst);
}

/* chk entry's interfaces.
 * remove interfaces that this server no longer
 * supports
 */
static void rs_ns_entry_chk_ifspecs
  (
    unsigned_char_p_t       name,
    error_status_t          *st
  )
{
    rpc_if_id_vector_p_t    if_ids;
    rpc_if_id_vector_p_t    entry_if_ids;
    unsigned32              i,
                            j;
    boolean32               if_found;
    error_status_t          lst;

    rpc_mgmt_inq_if_ids(NULL, &if_ids, st);
    if (BAD_STATUS(st)) return;
    rpc_ns_mgmt_entry_inq_if_ids(rpc_c_ns_syntax_dce, name, 
        &entry_if_ids, st);
    if (BAD_STATUS(st)) {
        rpc_if_id_vector_free(&if_ids, &lst);
        return;
    }

    for (i = 0; i < entry_if_ids->count; i++) {
        if_found = false;

        for (j = 0; j < if_ids->count; j++) {
            if ( uuid_equal(&entry_if_ids->if_id[i]->uuid, 
                           &if_ids->if_id[j]->uuid, &lst) &&
                 (entry_if_ids->if_id[i]->vers_major == 
                     if_ids->if_id[j]->vers_major)        &&
                 (entry_if_ids->if_id[i]->vers_minor == 
                     if_ids->if_id[j]->vers_minor) ) {
                if_found = true;
                break;
            }
        }

        if (!if_found) {
            rs_ns_entry_delete_if_id(name, entry_if_ids->if_id[i], st);
            if (BAD_STATUS(st)) break;
        }
    }

    rpc_if_id_vector_free(&entry_if_ids, &lst);
    rpc_if_id_vector_free(&if_ids, &lst);
}

/*
 * rs_ns_entry_lookup_objs
 *
 * return the ns entry's object uuids in
 * entry_objs
 */
static void rs_ns_entry_lookup_objs
  (
    unsigned_char_p_t       name,
    rs_ns_uuid_vector_p_t   *entry_objs,
    error_status_t          *st
  )
{
    unsigned32              nobjs,
                            nobjs2; 
    rpc_ns_handle_t         lookup_h;
    uuid_t                  entry_obj_id,
                            prev_obj_id;
    rs_ns_uuid_vector_p_t   tmp_entry_objs;
    unsigned32              i;
    error_status_t          lst;

    *entry_objs = NULL;

    rpc_ns_entry_object_inq_begin(rpc_c_ns_syntax_dce, 
        name, &lookup_h, st);
    if (BAD_STATUS(st)) return;

    nobjs = 0;
    while (true) {
        rpc_ns_entry_object_inq_next(lookup_h, &entry_obj_id, st);
        if (BAD_STATUS(st)) {
            if (STATUS_EQUAL(st, rpc_s_no_more_members)) 
                CLEAR_STATUS(st);
            break;
        }

        nobjs++;
        prev_obj_id = entry_obj_id;
    }

    rpc_ns_entry_object_inq_done(&lookup_h, &lst);

    if (BAD_STATUS(st)) return;

    if (nobjs == 0) return;

    tmp_entry_objs = (rs_ns_uuid_vector_p_t) 
                    malloc( sizeof(rs_ns_uuid_vector_t) +
                    (sizeof(uuid_t) * (nobjs-1)) );

    if (nobjs == 1) {
        tmp_entry_objs->count = 1;
        tmp_entry_objs->uuid[0] = prev_obj_id;
        *entry_objs = tmp_entry_objs;
        return;
    }

    rpc_ns_entry_object_inq_begin(rpc_c_ns_syntax_dce, 
        name, &lookup_h, st);
    if (BAD_STATUS(st)) {
        free(tmp_entry_objs);
        return;
    }

    nobjs2 = 0;
    for (i = 0; i < nobjs; i++) {
        rpc_ns_entry_object_inq_next(lookup_h, &tmp_entry_objs->uuid[i], st);
        if (BAD_STATUS(st)) {
            if (STATUS_EQUAL(st, rpc_s_no_more_members)) 
                CLEAR_STATUS(st);
            break;
        }

        nobjs2++;
    }

    rpc_ns_entry_object_inq_done(&lookup_h, &lst);

    if (BAD_STATUS(st)) {
        free(tmp_entry_objs);
        return; 
    }

    tmp_entry_objs->count = nobjs2;
                                   
    *entry_objs = tmp_entry_objs;
}
 
/*
 * Add the replica instance uuid to the server's
 * ns entry.  The ns entry must already exist.
 *  
 * The replica instance uuid is a special, non-rpc
 * supported attribute whose attribute name is
 * in rs_ns_c_attrib_rep_id.  It is saved as a
 * uuid char string to avoid endian conversion
 * issues.
 */
static void rs_ns_entry_add_rep_id
  (
    unsigned_char_p_t       name,
    uuid_p_t                rep_id,
    error_status_t          *st
  )
{ 
    unsigned_char_p_t   rep_id_str;
    unsigned32          slen;
    error_status_t      lst;

    uuid_to_string(rep_id, &rep_id_str, st);
    if (BAD_STATUS(st)) return;

    slen = (unsigned32) strlen((char *) rep_id_str) + 1;

    rs_dns_attr_add_value(name, rs_ns_c_attrib_rep_id, 
        rep_id_str, slen, st);

    rpc_string_free(&rep_id_str, &lst);

    if (BAD_STATUS(st)) return;
}
 
static void rs_ns_entry_replace_rep_id
  (
    unsigned_char_p_t       name,
    uuid_p_t                rep_id,
    uuid_p_t                old_rep_id,
    error_status_t          *st
  )
{
    unsigned_char_p_t   old_rep_id_str;
    unsigned32          slen;
    error_status_t      lst;

    uuid_to_string(rep_id, &old_rep_id_str, st);
    if (BAD_STATUS(st)) return;

    slen = (unsigned32) strlen((char *) old_rep_id_str) + 1;

    rs_dns_attr_remove_value(name, rs_ns_c_attrib_rep_id, 
        old_rep_id_str, slen, st);
    rpc_string_free(&old_rep_id_str, &lst);
    if (BAD_STATUS(st)) return;

    rs_ns_entry_add_rep_id(name, rep_id, st);
    if (BAD_STATUS(st)) return;
}
 
static void rs_ns_entry_lookup_rep_id
  (
    unsigned_char_p_t       name,
    uuid_t                  *rep_id,
    error_status_t          *st
  )
{
    unsigned_char_p_t   attr_value;
    unsigned32          attr_value_len;
    error_status_t      lst;

    rs_dns_attr_read_value(name, rs_ns_c_attrib_rep_id, 
        &attr_value, &attr_value_len, st);
    if (BAD_STATUS(st)) return;

    uuid_from_string(attr_value, rep_id, st);
    free(attr_value);
    if (BAD_STATUS(st)) return;
}

/*
 * Add a member to group_name.
 *
 * Also add the security service object uuid to
 * the group_name in case it's not already 
 * there.
 */
static void rs_ns_group_add_member
  (
    unsigned_char_p_t       group_name,
    uuid_p_t                obj_id,
    unsigned_char_p_t       name,
    error_status_t          *st
  )
{
    uuid_vector_t       obj_ids;

    if ((group_name == NULL) ||
        (name == NULL)) {
        SET_STATUS(st, sec_rgy_bad_nameservice_name);
        return;
    }

    obj_ids.count = 1;
    obj_ids.uuid[0] = obj_id;

    rpc_ns_binding_export(rpc_c_ns_syntax_dce, group_name,
        NULL, NULL, &obj_ids, st);
    if (BAD_STATUS(st)) return;

    rpc_ns_group_mbr_add(rpc_c_ns_syntax_dce, group_name,
        rpc_c_ns_syntax_dce, name, st);
    if (BAD_STATUS(st)) return;
}

/*
 * Delete a member from group_name.
 */
static void rs_ns_group_delete_member
  (
    unsigned_char_p_t       group_name,
    unsigned_char_p_t       name,
    error_status_t          *st
  )
{
    if ((group_name == NULL) ||
        (name == NULL)) {
        SET_STATUS(st, sec_rgy_bad_nameservice_name);
        return;
    }

    rpc_ns_group_mbr_remove(rpc_c_ns_syntax_dce, group_name,
        rpc_c_ns_syntax_dce, name, st);
    if (BAD_STATUS(st)) {
        if (STATUS_EQUAL(st, rpc_s_group_member_not_found))
            CLEAR_STATUS(st);
        return;
    }
}

