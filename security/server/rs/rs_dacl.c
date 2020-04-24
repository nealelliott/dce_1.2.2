/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_dacl.c,v $
 * Revision 1.1.12.3  1996/02/18  00:17:44  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:06:40  marty]
 *
 * Revision 1.1.12.2  1995/12/08  17:51:10  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/07/13  21:08 UTC  burati
 * 	Merge ACL alias fix (CHFts15622) from mb_mothra5
 * 
 * 	HP revision /main/HPDCE02/mb_mothra5/1  1995/07/13  20:47 UTC  burati
 * 	CHFts15622 (OT12915) Alias ACL bug
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/15  21:05 UTC  greg
 * 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
 * 	[1995/04/21  18:57 UTC  greg  /main/SEC_migrate_merge/1]
 * 
 * 	Merge work from greg_sec_migrate_acls branch.
 * 	[1995/04/18  18:19 UTC  greg  /main/SEC_migrate/1]
 * 
 * 	Add software version to sec_acl_mgr_configure()
 * 	call in rs_dacl_init().
 * 	[1995/04/13  17:09 UTC  greg  /main/greg_sec_migrate_acls/1]
 * 
 * Revision 1.1.7.4  1994/07/15  15:01:19  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:19:20  mdf]
 * 
 * Revision 1.1.7.3  1994/06/02  21:25:03  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:18:19  mdf]
 * 
 * Revision 1.1.7.2  1994/04/22  19:14:00  rps
 * 	Support registry database polymorphism
 * 	[1994/04/21  00:13:51  rps]
 * 
 * Revision 1.1.8.2  1994/03/29  23:44:45  rps
 * 	Fix get_manager_types to use number of returned types rather than total
 * 
 * Revision 1.1.7.1  1994/01/28  23:11:17  burati
 * 	Delegation changes - use rdaclifv0 until v1 integrated (dlg_bl1)
 * 	[1994/01/20  04:22:40  burati]
 * 
 * Revision 1.1.5.5  1993/02/11  23:12:20  ahop
 * 	7186: fix text in comment
 * 	[1993/02/11  22:34:00  ahop]
 * 
 * 	7186: include rs_master.h, rs_replist2.h
 * 	[1993/02/11  22:24:31  ahop]
 * 
 * 	7186: implement rdacl_get_referral and fix crash in marshalling code
 * 	[1993/02/11  21:50:58  ahop]
 * 
 * Revision 1.1.5.4  1992/12/29  16:34:56  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:23:28  zeliff]
 * 
 * Revision 1.1.5.3  1992/11/04  19:35:34  ahop
 * 	Third replication drop: bl6a
 * 	  new rs_log_acl_replace semantics
 * 	[1992/11/03  18:30:10  ahop]
 * 
 * Revision 1.1.5.2  1992/10/07  20:32:38  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:16:18  sekhar]
 * 
 * Revision 1.1.3.2  1992/10/06  16:09:13  burati
 * 	Second replication drop: bl5
 * 	Add replication logic; code cleanup
 * 
 * Revision 1.1.2.3  1992/05/08  15:03:55  burati
 * 	Added rdacl_get_mgr_types_semantics(...)
 * 	[1992/04/29  03:16:05  burati]
 * 
 * Revision 1.1.2.2  1992/03/30  16:34:08  ahop
 * 	 support locksmith mode
 * 	[1992/03/27  23:58:30  ahop]
 * 
 * Revision 1.1  1992/01/19  14:44:29  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
 * Copyright (c) Hewlett-Packard Company 1991, 1992, 1994, 1995
 * Unpublished work. All Rights Reserved.
 */
/*
 *      Registry Server - sec_acl wire interface routines
 */

#ifdef MODULE_VERSION_ID
static char *VersionID =
    "$Id: rs_dacl.c,v 1.1.12.3 1996/02/18 00:17:44 marty Exp $";
#endif

#include <dce/uuid.h>
#include <dce/rpc.h>
#include <dce/rplbase.h>
#include <dce/sec_acl_encode.h>
#include <rdaclifv0.h>
#include <rdaclif.h>
#include <dce/sec_daclmgr.h>

#include <rgymacro.h>
#include <rs_auth.h>
#include <rs_lock.h>
#include <rs_ch_lock.h>
#include <rs_log_acl.h>
#include <rs_audit_trail.h>
#include <rs_master.h>
#include <rs_replist2.h>
#include <macros.h>
#include <un_maloc.h>
#include <rsdb_acl.h>
#include <rsdb_name_util.h>

#if defined(DCE_AUD_POINTS)
#include <dce/audit.h>
#include <sec_aud_evts.h>
#endif	/* defined(DCE_AUD_POINTS) */

#if defined(DCE_AUD_POINTS)
extern dce_aud_trail_t sec_aud_trail;
#endif	/* defined(DCE_AUD_POINTS) */

void rdaclv0_lookup(
    handle_t h,
    sec_acl_component_name_t component_name,
    uuid_t *manager_type,
    sec_acl_type_t sec_acl_type,
    sec_acl_result_t *result
);
globaldef rdaclif_v0_0_epv_t rdaclif_v0_0_manager_epv = {
    rdaclv0_lookup,
    rdacl_replace,
    rdacl_get_access,
    rdacl_test_access,
    rdacl_test_access_on_behalf,
    rdacl_get_manager_types,
    rdacl_get_printstring,
    rdacl_get_referral,
    rdacl_get_mgr_types_semantics
};
globaldef rdaclif_v1_0_epv_t rdaclif_v1_0_manager_epv = {
    rdacl_lookup,
    rdacl_replace,
    rdacl_get_access,
    rdacl_test_access,
    rdacl_test_access_on_behalf,
    rdacl_get_manager_types,
    rdacl_get_printstring,
    rdacl_get_referral,
    rdacl_get_mgr_types_semantics
};

/* Handle to the sec_acl manager, initialized once, used for each operation */
PRIVATE sec_acl_mgr_handle_t    sec_acl_mgr;

PRIVATE uuid_t  *dir_mgr_type = &(rsdb_acl_mgr_uuids[rsdb_acl_mgr_type_dir]);
 
/*
 *  Translate rgy status codes to acl codes
 */
PRIVATE void rgy_to_acl_status
    (
    error_status_t  *st_p
    )
{
    switch(*st_p) {
    case sec_rgy_not_authorized:
        SET_STATUS(st_p, sec_acl_not_authorized);
        break;
    case sec_rgy_object_not_found:
        SET_STATUS(st_p, sec_acl_object_not_found);
        break;
    case sec_rgy_rep_not_master:
        SET_STATUS(st_p, sec_acl_site_read_only);
        break;
    case sec_rgy_rep_bad_state:
        SET_STATUS(st_p, sec_acl_server_bad_state);
        break;
    default:
        /* All other status codes remain unchanged */
        break;
    }
}


/*
 *  Callers of rs_is_authorized are passed the manager type of the
 *  desired end result, which does not necessarily match the object
 *  itself -- for instance, changing the default initial object acl of a
 *  group domain directory is passed as (acltype==def_obj, mgrtype==group).
 */
static boolean32
rs_dacl_is_authorized(
    handle_t			h,
    sec_acl_permset_t		requested_perms,
    sec_acl_permset_t		parent_perms,
    sec_rgy_name_t		object_name,
    uuid_t			*mgr_type,
    sec_acl_type_t		acl_type,
    boolean32			check_quota,
    sec_rgy_foreign_id_t	*client_ids,
    error_status_t		*st)
{
    uuid_t	*obj_mgr_type;

    /* Deduce the manager type of the actual object to be checked */
    if (OBJECT_ACL_TYPE(acl_type))
	obj_mgr_type = mgr_type;
    else
	obj_mgr_type = &rsdb_acl_mgr_uuids[rsdb_acl_mgr_type_dir];

    return rs_is_authorized(h, requested_perms, parent_perms,
				object_name, obj_mgr_type,
				check_quota, client_ids, st);
}



/*
 * Initialize data needed in the sec_acl wire interface routines.
 *
 * Note: This routine should be called inside a read lock on lock_db.
 * Currently, it's only called in rs_main inside a global lock.
 */
PUBLIC void rs_dacl_init
    (
    error_status_t  *st_p
    )
{
    unsigned32     cur_sw_rev;
    error_status_t lst;

    rs_master_info_get_sw_rev(&cur_sw_rev, &lst);
    
    if (BAD_STATUS(&lst)) {
	/*
	 * If we can't determine our current sw rev,
	 * we're in deep, deep, trouble, so just
         * log a fatal exit through the serviceability
         * module.
	 */
	SEC_SVC_PRINT_MSG_ST(SEC_REP_CANT_GET_SW_REV_FATAL_MSG, lst);
    }

    sec_acl_mgr_configure( sec_acl_mgr_config_stable, cur_sw_rev,
                           (unsigned_char_p_t)NULL, &sec_acl_mgr, st_p);
}


/*
 * IMPLEMENTATION OF THE SEC_ACL WIRE INTERFACE ROUTINES
 */

/*  r d a c l _ l o o k u p
 *
 *  retrieve an acl associated with the object referred to in the handle
 *  parameter.  The component_name argument is used to further identify
 *  the entity being protected by the sec_acl.
 * 
 *  Comparable to POSIX acl_read()
 */
PUBLIC void rdacl_lookup (
    handle_t                    h,
    sec_acl_component_name_t    component_name,
    uuid_t                      *manager_type_p,
    sec_acl_type_t              sec_acl_type,
    sec_acl_result_t            *sec_acl_result_p
)
{
    error_status_t          st;
    sec_id_t                user_obj, group_obj, *user_p, *group_p;
    rsdb_acl_ids_t          ids; 
    sec_acl_list_t          *sec_acl_list_p;
    sec_acl_permset_t       requested_perms;
    sec_acl_permset_t       parent_perms = NULL;
    sec_rgy_foreign_id_t    client_ids;
    sec_rgy_name_t          *object_name_p;
#if defined(DCE_AUD_POINTS)
    dce_aud_rec_t           ard;
    dce_aud_ev_info_t       ev_info;
    error_status_t          aud_status;
#endif	/* defined(DCE_AUD_POINTS) */

#if defined(DCE_AUD_POINTS)
    dce_aud_start(ACL_Lookup, h, 0, aud_c_esl_cond_unknown, 
                  &ard, &aud_status);
    if (ard) {
        ev_info.format = aud_c_evt_info_char_string;
        ev_info.data.char_string = component_name;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);

        ev_info.format = aud_c_evt_info_uuid;
        ev_info.data.uuid = *manager_type_p;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);

        ev_info.format = aud_c_evt_info_ulong_int;
        ev_info.data.ulong_int = sec_acl_type;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);
    }
#endif	/* defined(DCE_AUD_POINTS) */

    /* any permission required on object named by component name */
    requested_perms = NULL;
    object_name_p = (sec_rgy_name_t *)component_name;

    user_p = &user_obj; group_p = &group_obj;

    BEGIN_READ(lock_db, &st) {
	if (rs_dacl_is_authorized(h, requested_perms, parent_perms,
				  *object_name_p, manager_type_p, sec_acl_type,
				  false, &client_ids, &st))
	    {
            rsdb_acl_get_ids_from_name(component_name, sec_acl_type,
                                       manager_type_p, &user_p, 
                                       &group_p, &ids, &st);
            if (GOOD_STATUS(&st)) {
                sec_acl_mgr_lookup(sec_acl_mgr, (sec_acl_key_t)&ids,
                                   manager_type_p, sec_acl_type, 
                                   &sec_acl_list_p, &st);
            }
        }
    } END_READ
    if (GOOD_STATUS(&st)) {
        /* Copy acl_list to automatically-freed rpc_allocated buffer.
         * Free sec_acl_list.
         */
        rsdb_acl_copy_list_to_rpc_buf(&ids, sec_acl_list_p,
            &sec_acl_result_p->tagged_union.sec_acl_list, &st);
        rsdb_acl_free_list(&sec_acl_list_p);
    } else {
        rgy_to_acl_status(&st);
    }
    SET_STATUS(&(sec_acl_result_p->st), st);

#if defined(DCE_AUD_POINTS)
    if (ard) {    
        if (GOOD_STATUS(&st)) {
            dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1,  
                           aud_c_esl_cond_success, &aud_status); 
        } else {  
            if (rs_util_status_is_denial(st)) {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1,  
                               aud_c_esl_cond_denial, &aud_status);
            } else {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1,
                               aud_c_esl_cond_failure, &aud_status);
            } 
        }
    }
#endif	/* defined(DCE_AUD_POINTS) */

}


/*  r d a c l v 0 _ l o o k u p
 *
 *  retrieve an acl associated with the object referred to in the handle
 *  parameter.  The component_name argument is used to further identify
 *  the entity being protected by the sec_acl.
 * 
 *  Comparable to POSIX acl_read()
 */
PUBLIC void rdaclv0_lookup (
    handle_t                    h,
    sec_acl_component_name_t    component_name,
    uuid_t                      *manager_type_p,
    sec_acl_type_t              sec_acl_type,
    sec_acl_result_t            *sec_acl_result_p
)
{
    rdacl_lookup(h, component_name, manager_type_p, sec_acl_type,
        sec_acl_result_p);

    /* Replace new entry types with extended types so that the old client
     * will understand how to manipulate them.
     */
    if (GOOD_STATUS(&(sec_acl_result_p->st))) {
        sec_acl_encode_v0_entries(rpc_ss_allocate, rpc_ss_free, false,
          sec_acl_result_p->tagged_union.sec_acl_list, &sec_acl_result_p->st);
    }
}


/*  r d a c l _ r e p l a c e
 *
 * Replace the acl associated with the object named by component_name.
 * ACLs are immutable, the replace operation takes the new acl and throws
 * away the old acl associated with the object.
 *
 * The client (identified by handle h) must have owner permission on the 
 * object named by component name in order to modify its acl.
 *
 * Comparable to POSIX acl_write()
 */
PUBLIC void rdacl_replace (
    handle_t                    h,
    sec_acl_component_name_t    component_name,
    uuid_t                      *manager_type_p,
    sec_acl_type_t              sec_acl_type,
    sec_acl_list_t              *sec_acl_list_p,
    error_status_t              *st_p
)
{
    char                    buf[64];
    char                    acl_type_buf[20];
    sec_id_t                user_obj, group_obj, *user_p, *group_p;
    rsdb_acl_ids_t          ids;
    sec_acl_permset_t       requested_perms;
    sec_acl_permset_t       parent_perms = NULL;
    sec_rgy_foreign_id_t    client_ids;
    sec_rgy_name_t          *object_name_p;
#if defined(DCE_AUD_POINTS)
    dce_aud_rec_t           ard;
    dce_aud_ev_info_t       ev_info;
    error_status_t          aud_status;
#endif	/* defined(DCE_AUD_POINTS) */

    CLEAR_STATUS(st_p);

    /* If any extended entries that are known to be encoded DCE1.1 ACL entry
     * types (added for delegation) exist in the ACL, then decode them and
     * replace the extended entry with the decoded entry.
     */
    sec_acl_decode_v0_entries(rpc_ss_allocate, rpc_ss_free, false,
        sec_acl_list_p, st_p);

    /* owner permission required on object named by component name */
    requested_perms = sec_acl_perm_owner;
    object_name_p = (sec_rgy_name_t *)component_name;

    user_p = &user_obj; group_p = &group_obj;

    switch(sec_acl_type) {
    case sec_acl_type_object:
        sprintf(acl_type_buf, "object");
        break;
    case sec_acl_type_default_object:
        sprintf(acl_type_buf, "initial object");
        break;
    case sec_acl_type_default_container:
        sprintf(acl_type_buf, "initial container");
        break;
    default:
        sprintf(acl_type_buf, "unknown");
        break;
    }

#if defined(DCE_AUD_POINTS)
    dce_aud_start(ACL_Replace, h, 0, aud_c_esl_cond_unknown, 
                  &ard, &aud_status);
    if (ard) {
        ev_info.format = aud_c_evt_info_char_string;
        ev_info.data.char_string = component_name;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);

        ev_info.format = aud_c_evt_info_uuid;
        ev_info.data.uuid = *manager_type_p;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);

        ev_info.format = aud_c_evt_info_ulong_int;
        ev_info.data.ulong_int = sec_acl_type;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);

/* Need old and new acl list */
/*        dce_aud_put_ev_info(ard, ev_info, &aud_status);
*/
    }
#endif	/* defined(DCE_AUD_POINTS) */

    BEGIN_MASTER_RIW(lock_db, st_p) {
        sprintf(buf, "Attempt Replace acl of type %s", acl_type_buf);
        rs_audit_trail_log(h, buf, (char *)object_name_p);

	if (rs_dacl_is_authorized(h, requested_perms, parent_perms,
				  *object_name_p, manager_type_p, sec_acl_type,
				  false, &client_ids, st_p))
	    {
            rsdb_acl_get_ids_from_name(component_name, sec_acl_type, 
                                       manager_type_p, &user_p, 
                                       &group_p, &ids, st_p);
            if (GOOD_STATUS(st_p)) {
                RIW_PROMOTE(lock_db);
                sec_acl_mgr_replace(sec_acl_mgr, (sec_acl_key_t)&ids, 
                                    manager_type_p, sec_acl_type, 
                                    sec_acl_list_p, st_p);
                if(STATUS_OK(st_p)) {
                    sprintf(buf, "Replace acl of type %s", acl_type_buf);
                    rs_audit_trail_log(h, buf, (char *)object_name_p);
                    rs_log_acl_replace(sec_acl_mgr, component_name,
                                    manager_type_p, sec_acl_type, 
                                    sec_acl_list_p,
                                    (rs_replica_master_info_t *) NULL);
                }
            }
        }
    } END_MASTER_RIW

#if defined(DCE_AUD_POINTS)
    if (ard) {    
        if (GOOD_STATUS(st_p)) {
            dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1,
                           aud_c_esl_cond_success, &aud_status); 
        } else {  
            if (rs_util_status_is_denial(*st_p)) {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1,  
                               aud_c_esl_cond_denial, &aud_status);
            } else {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1,
                               aud_c_esl_cond_failure, &aud_status);
            } 
        }
    }
#endif	/* defined(DCE_AUD_POINTS) */

    /* return only sec_acl status codes */
    rgy_to_acl_status(st_p);
}


/* r d a c l _ t e s t _ a c c e s s
 *
 * Determine if the caller has the requested access.
 * 
 * Return true if client has the requested access to the object.
 */
PUBLIC boolean32 rdacl_test_access (
    handle_t                    h,
    sec_acl_component_name_t    component_name,
    uuid_t                      *manager_type_p,
    sec_acl_permset_t           desired_permset,
    error_status_t              *st_p
)
{
    sec_acl_permset_t       parent_perms = NULL;
    sec_rgy_foreign_id_t    client_ids;
    sec_rgy_name_t          *object_name_p;
    boolean32               authorized = true;
#if defined(DCE_AUD_POINTS)
    dce_aud_rec_t           ard;
    dce_aud_ev_info_t       ev_info;
    error_status_t          aud_status;
#endif	/* defined(DCE_AUD_POINTS) */

    CLEAR_STATUS(st_p);
    object_name_p = (sec_rgy_name_t *)component_name;

#if defined(DCE_AUD_POINTS)
    dce_aud_start(ACL_TestAccess, h, 0, aud_c_esl_cond_unknown, 
                  &ard, &aud_status);
    if (ard) {
        ev_info.format = aud_c_evt_info_char_string;
        ev_info.data.char_string = component_name;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);

        ev_info.format = aud_c_evt_info_uuid;
        ev_info.data.uuid = *manager_type_p;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);

        ev_info.format = aud_c_evt_info_ulong_int;
        ev_info.data.ulong_int = desired_permset;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);
    }
#endif	/* defined(DCE_AUD_POINTS) */

    BEGIN_READ(lock_db, st_p) {
        authorized = rs_is_authorized(h, desired_permset,
                                      parent_perms, *object_name_p,
                                      manager_type_p, false, 
                                      &client_ids, st_p);
    } END_READ
    rgy_to_acl_status(st_p);
    /* status not_authorized translates to successful call with false return */
    if (STATUS_EQUAL(st_p, sec_acl_not_authorized)) 
        CLEAR_STATUS(st_p);

#if defined(DCE_AUD_POINTS)
    if (ard) {    
        if (GOOD_STATUS(st_p)) {
            dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                           aud_c_esl_cond_success, &aud_status); 
        } else {  
            if (rs_util_status_is_denial(*st_p)) {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1,  
                               aud_c_esl_cond_denial, &aud_status);
            } else {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1,  
                               aud_c_esl_cond_failure, &aud_status);
            } 
        }
    }
#endif	/* defined(DCE_AUD_POINTS) */

    return authorized;
}


/* r d a c l _ t e s t _ a c c e s s _ o n _ b e h a l f
 *
 *  Not supported in secd, because there's no reason for it to be.
 */
PUBLIC boolean32 rdacl_test_access_on_behalf (
    handle_t                    h,
    sec_acl_component_name_t    component_name,
    uuid_t                      *manager_type_p,
    sec_id_pac_t                *subject_p,
    sec_acl_permset_t           desired_permset,
    error_status_t              *st_p
)
{
    SET_STATUS(st_p, sec_acl_not_implemented);
    return false;
}


/* r d a c l _ g e t _ m a n a g e r _ t y p e s
 *
 * Determine the types of acls protecting an object.  ACL editors/browsers
 * use this operation to determine the acl manager types that a particular
 * reference monitor is using to protect a selected entity.
 *
 */
PUBLIC void rdacl_get_manager_types (
    handle_t                    h,
    sec_acl_component_name_t    component_name,
    sec_acl_type_t              acl_type,
    unsigned32                  size_avail,
    unsigned32                  *size_used_p,
    unsigned32                  *num_types_p,
    uuid_t                      *manager_types,
    error_status_t              *st_p
)
{
    sec_acl_permset_t       requested_perms;
    sec_acl_permset_t       parent_perms = NULL;
    sec_rgy_foreign_id_t    client_ids;
    sec_rgy_name_t          *object_name_p; 
    long                    i;
#if defined(DCE_AUD_POINTS)
    dce_aud_rec_t           ard;
    dce_aud_ev_info_t       ev_info;
    error_status_t          aud_status;
#endif	/* defined(DCE_AUD_POINTS) */
                            
    CLEAR_STATUS(st_p);
    /* any permission required on object named by component name */
    requested_perms = NULL;
    object_name_p = (sec_rgy_name_t *)component_name;

#if defined(DCE_AUD_POINTS)
    dce_aud_start(ACL_GetMgrTypes, h, 0, aud_c_esl_cond_unknown, 
                  &ard, &aud_status);
    if (ard) {
        ev_info.format = aud_c_evt_info_char_string;
        ev_info.data.char_string = component_name;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);

        ev_info.format = aud_c_evt_info_ulong_int;
        ev_info.data.ulong_int = acl_type;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);
    }
#endif	/* defined(DCE_AUD_POINTS) */

    BEGIN_READ(lock_db, st_p) {
        sec_acl_mgr_get_manager_types(sec_acl_mgr, 
                                  (sec_acl_key_t)component_name,
                                   acl_type, size_avail, size_used_p,
                                   num_types_p, manager_types, st_p);
        if(STATUS_OK(st_p)) {
            /* Caller must have some permissions on component_name. */
	    for(i = 0; i < *size_used_p; i++) {
		if (!rs_dacl_is_authorized(h, requested_perms, parent_perms,
					   *object_name_p, &manager_types[i],
					   acl_type, false, &client_ids, st_p))
		{
                    *num_types_p = 0;
                    *size_used_p = 0;
                    rgy_to_acl_status(st_p);
                    break;
                }
            }
        }
    } END_READ

#if defined(DCE_AUD_POINTS)
    if (ard) {    
        if (GOOD_STATUS(st_p)) {
            dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                           aud_c_esl_cond_success, &aud_status); 
        } else {  
            if (rs_util_status_is_denial(*st_p)) {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1,
                               aud_c_esl_cond_denial, &aud_status);
            } else {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1,
                               aud_c_esl_cond_failure, &aud_status);
            } 
        }
    }
#endif	/* defined(DCE_AUD_POINTS) */

}


/* r d a c l _ g e t _ m g r _ t y p e s _ s e m a n t i c s
 *
 * Determine the types of acls protecting an object.  ACL editors/browsers
 * use this operation to determine the acl manager types that a particular
 * reference monitor is using to protect a selected entity.
 * Let the caller know we don't support the current optional POSIX semantics
 */
PUBLIC void rdacl_get_mgr_types_semantics (
    handle_t                    h,
    sec_acl_component_name_t    component_name,
    sec_acl_type_t              acl_type,
    unsigned32                  size_avail,
    unsigned32                  *size_used_p,
    unsigned32                  *num_types_p,
    uuid_t                      *manager_types,
    sec_acl_posix_semantics_t   posix_semantics[],
    error_status_t              *st_p
)
{
    int i;
    rdacl_get_manager_types(h, component_name, acl_type, size_avail,
                            size_used_p, num_types_p, manager_types, st_p);
    if (GOOD_STATUS(st_p)) {
        for (i = 0; i < *size_used_p; i++) {
            posix_semantics[i] = sec_acl_posix_no_semantics;
        }
    }
}


/*  r d a c l _ g e t _ p r i n t s t r i n g
 *
 *  Retrieve printable representations for each permission bit that 
 *  the acl manager will support.  There may be aliases for common
 *  permission combinations - by convention simple entries should appear
 *  at the beginning of the array, and combinations should appear at the
 *  end.  When false the tokenize flag indicates that permission
 *  printstrings are unambiguous and therefore printstrings for various 
 *  permissions can be concatenated.  When true, however,  this property 
 *  does not hold and the strings should be tokenized before input or output.
 */
PUBLIC void rdacl_get_printstring (
    handle_t                h,
    uuid_t                  *manager_type_p,
    unsigned32              size_avail,
    uuid_t                  *manager_type_chain_p,
    sec_acl_printstring_t   *manager_info_p,
    boolean32               *tokenize_p,
    unsigned32              *total_num_printstrings_p,
    unsigned32              *size_used_p,
    sec_acl_printstring_t   printstrings[],
    error_status_t          *st_p
)
{
    CLEAR_STATUS(st_p);
    BEGIN_READ(lock_db, st_p) {
        sec_acl_mgr_get_printstring(sec_acl_mgr, manager_type_p, size_avail,
                                    manager_type_chain_p, manager_info_p,
                                    tokenize_p, total_num_printstrings_p,
                                    size_used_p, printstrings, st_p);
    } END_READ
}


/*  r d a c l _ g e t _ r e f e r r a l
 *
 * Obtain a referral to an acl update site.  This function is used when
 * the current acl site yields a sec_acl_site_readonly error.  Some
 * replication managers will require all updates for a given object to
 * be directed to a given replica.  Clients of the generic acl interface may
 * know they are dealing with an object that is replicated in this way. 
 * This function allows them to recover from this problem and rebind to
 * the proper update site.
 *
 * Client must have any access to object named by component_name.
 */
PUBLIC void rdacl_get_referral (
    handle_t                    h,
    sec_acl_component_name_t    component_name,
    uuid_t                      *manager_type_p, 
    sec_acl_type_t              sec_acl_type,
    sec_acl_tower_set_t         *towers_p,
    error_status_t              *st_p
)
{
    sec_acl_permset_t       requested_perms;
    sec_acl_permset_t       parent_perms = NULL;
    sec_rgy_foreign_id_t    client_ids;
    sec_rgy_name_t          *object_name_p;
    boolean32               master_known;
    uuid_t                  master_id;
    rs_update_seqno_t       master_seqno;
    rs_replica_item_t       rep_item;
    unsigned32              nreps;
    boolean32               authorized = false;
#if defined(DCE_AUD_POINTS)
    dce_aud_rec_t           ard;
    dce_aud_ev_info_t       ev_info;
    error_status_t          aud_status;
#endif	/* defined(DCE_AUD_POINTS) */

    /* Initialize return values in case of bad status */
    *towers_p = NULL;

    /* any permission required on object named by component name */
    requested_perms = NULL;
    object_name_p = (sec_rgy_name_t *)component_name;

#if defined(DCE_AUD_POINTS)
    dce_aud_start(ACL_GetReferral, h, 0, aud_c_esl_cond_unknown, 
                  &ard, &aud_status);
    if (ard) {
        ev_info.format = aud_c_evt_info_char_string;
        ev_info.data.char_string = component_name;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);

        ev_info.format = aud_c_evt_info_uuid;
        ev_info.data.uuid = *manager_type_p;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);

        ev_info.format = aud_c_evt_info_ulong_int;
        ev_info.data.ulong_int = sec_acl_type;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);
    }
#endif	/* defined(DCE_AUD_POINTS) */

    BEGIN_READ(lock_db, st_p) {
	authorized = rs_dacl_is_authorized(h, requested_perms, parent_perms,
					   *object_name_p, manager_type_p,
					   sec_acl_type, false, &client_ids, st_p);
    } END_READ
    if(authorized && GOOD_STATUS(st_p)) {
        BEGIN_READ_EVENT(rs_c_event_primitive_read, lock_replist, st_p) {
            rs_master_info_get(&master_known, &master_id, &master_seqno, st_p);

            if (GOOD_STATUS(st_p)) {
                if (!master_known) {
                    SET_STATUS(st_p, sec_rgy_rep_master_not_found);
                }
                else {
                    /* get master's towers from replist.
                     * rs_replist2_read uses rpc_ss_allocate
                     * to get memory for towers; this memory
                     * will be freed by stub
                     */
                    rs_replist2_read(&master_id, 1, &nreps, &rep_item, st_p);

                    if (GOOD_STATUS(st_p)) {
                        *towers_p = (sec_acl_tower_set_t)rep_item.rep_twrs;
                    }
                }
            }
        } END_READ_EVENT;
    }
    if(BAD_STATUS(st_p)) {
        rgy_to_acl_status(st_p);
    }

#if defined(DCE_AUD_POINTS)
    if (ard) {    
        if (GOOD_STATUS(st_p)) {
            dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                           aud_c_esl_cond_success, &aud_status); 
        } else {  
            if (rs_util_status_is_denial(*st_p)) {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                               aud_c_esl_cond_denial, &aud_status);
            } else {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                               aud_c_esl_cond_failure, &aud_status);
            } 
        }
    }
#endif	/* defined(DCE_AUD_POINTS) */

    return;
}


/*  r d a c l _ g e t _ a c c e s s 
 *
 *  Determine the caller's access to the specified object.  This is 
 *  useful for implementing operations like the conventional UNIX access
 *  function.
 *
 *  If the caller has no access, return object_not_found.
 */
PUBLIC void rdacl_get_access (
    handle_t                    h,
    sec_acl_component_name_t    component_name,
    uuid_t                      *manager_type_p,
    sec_acl_permset_t           *net_rights,
    error_status_t              *st_p
)
{
    rsdb_acl_ids_t          ids;
    sec_id_t                usr_obj, grp_obj, *user_p, *group_p;
    rpc_authz_cred_handle_t cred_h;
    sec_acl_permset_t       requested_perms;
    sec_acl_permset_t       parent_perms = NULL;
    sec_rgy_foreign_id_t    client_ids;
    sec_rgy_name_t          *object_name_p;
#if defined(DCE_AUD_POINTS)
    dce_aud_rec_t           ard;
    dce_aud_ev_info_t       ev_info;
    error_status_t          aud_status;
#endif	/* defined(DCE_AUD_POINTS) */

    CLEAR_STATUS(st_p);
    /* any permission required on object named by component name */
    requested_perms = NULL;
    object_name_p = (sec_rgy_name_t *)component_name;

#if defined(DCE_AUD_POINTS)
    dce_aud_start(ACL_GetAccess, h, 0, aud_c_esl_cond_unknown, 
                  &ard, &aud_status);
    if (ard) {
        ev_info.format = aud_c_evt_info_char_string;
        ev_info.data.char_string = component_name;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);

        ev_info.format = aud_c_evt_info_uuid;
        ev_info.data.uuid = *manager_type_p;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);
 
        /* Defer storing net_rights permissions into the audit record */
        /* until after the call to sec_acl_mgr_get_access(), which    */
        /* returns them.                                              */
    }
#endif	/* defined(DCE_AUD_POINTS) */

    user_p = &usr_obj;
    group_p = &grp_obj;
    BEGIN_READ(lock_db, st_p) {
        if(rs_is_authorized(h, requested_perms, parent_perms, *object_name_p,
	    manager_type_p, false, &client_ids, st_p)) {
	    
            rsdb_acl_get_ids_from_name(component_name, sec_acl_type_object,
                                       manager_type_p, &user_p, &group_p, 
                                       &ids, st_p);
            if (STATUS_OK(st_p)) {
                if(rs_auth_get_creds_from_handle(h, &cred_h, st_p)) {
                    sec_acl_mgr_get_access(sec_acl_mgr, cred_h, 
                                           (sec_acl_key_t)&ids, manager_type_p,
                                           user_p, group_p, net_rights, st_p);
                } else 
                    SET_STATUS(st_p, sec_acl_not_authorized);
            }
        }
    } END_READ
    rgy_to_acl_status(st_p);

#if defined(DCE_AUD_POINTS)
    if (ard) {    
        /* Have obtained the net_rights permissions -  */
        /* store them in the audit record.             */

        ev_info.format = aud_c_evt_info_ulong_int;
        ev_info.data.ulong_int = *net_rights;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);

        if (GOOD_STATUS(st_p)) {
            dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                           aud_c_esl_cond_success, &aud_status); 
        } else {  
            if (rs_util_status_is_denial(*st_p)) {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1,
                               aud_c_esl_cond_denial, &aud_status);
            } else {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                               aud_c_esl_cond_failure, &aud_status);
            } 
        }
    }
#endif	/* defined(DCE_AUD_POINTS) */

}


/*
 * rs_rdacl_prop_acl - Bug avoidance for secd's without the ALIAS ACL Fix.
 * 
 * Lookup the acl just saved by rs_pgo_add() and propagate it around.
 */
PUBLIC void rs_rdacl_prop_acl (
    sec_rgy_domain_t domain,
    sec_rgy_name_t   pgo_name,
    error_status_t   *stp
)
{
    sec_rgy_name_t      cname;
    sec_id_t            user_obj, group_obj, *user_p, *group_p;
    rsdb_acl_ids_t      ids; 
    sec_acl_list_t      *sec_acl_list_p;
    rsdb_acl_mgr_type_t mtype;
    uuid_t              mgr_type;

    if (! rsdb_name_util_complete_name(domain, pgo_name, cname, stp))
        return;

    user_p = &user_obj; group_p = &group_obj;

    mtype = GET_MGR_BY_DOMAIN(domain);
    rsdb_acl_get_mgr_uuid_from_type(mtype, &mgr_type, stp);
    rsdb_acl_get_ids_from_name(cname, sec_acl_type_object, &mgr_type,
        &user_p, &group_p, &ids, stp);
    if (GOOD_STATUS(stp)) {
        sec_acl_mgr_lookup(sec_acl_mgr, (sec_acl_key_t) &ids, &mgr_type,
            sec_acl_type_object, &sec_acl_list_p, stp);
        if (GOOD_STATUS(stp)) {
            rs_log_acl_replace(sec_acl_mgr, cname, &mgr_type,
                sec_acl_type_object, sec_acl_list_p,
                (rs_replica_master_info_t *)NULL);
        }
    }
}
