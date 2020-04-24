/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_rpladmn.c,v $
 * Revision 1.1.4.2  1996/02/18  00:20:01  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:08:12  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:56:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:21:36  root]
 * 
 * Revision 1.1.2.2  1992/12/29  16:37:21  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:26:41  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:44:14  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rs_rpladmn.c V=8 11/20/91 //littl/prgy/src/server/rs
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/* 
 *      Registry Server - Replication
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)rs_rpladmn.c	8 - 11/20/91";
#endif

#include <un_strng.h>
#include <sys/time.h>

#include <dce/rgynbase.h>
#include <rs_rpladmn.h>

#include <rs_base.h>
#include <rs_util.h>
#include <rs.h>
#include <rs_audit_trail.h>
#include <rs_pvt.h>
#include <rs_lock.h>
#include <rs_auth.h>
#include <rs_rsdb.h>
#include <rs_rpladmn_util.h>
#include <rsdb.h>
#include <rsdb_pvt.h>
#include <rsdb_acl.h>
#include <rsdb_auth.h>
#include <rsdb_policy.h>
#include <krb5/kdb.h>  

PRIVATE uuid_t  *plcy_mgr_type = &(rsdb_acl_mgr_uuids[rsdb_acl_mgr_type_rgy]);
PRIVATE sec_rgy_foreign_id_t    dummy_ids;

PRIVATE void rs_check_caller(
#ifdef __STDC__
    handle_t        handle,
    error_status_t  *st
#endif
);

PRIVATE void rs_check_caller(handle, st)
    handle_t        handle;
    error_status_t  *st;
{
    sec_acl_permset_t       requested_perms;
    sec_acl_permset_t       parent_perms;
    sec_rgy_name_t  *policy_obj_name = (sec_rgy_name_t *)SEC_POLICY_OBJ_NAME;

    CLEAR_STATUS(st);
    requested_perms = sec_acl_perm_Admin;
    parent_perms = (sec_acl_permset_t)NULL;

    READ_LOCK(lock_db) {
        rs_is_authorized(handle, requested_perms, parent_perms, 
                            *policy_obj_name,
                            plcy_mgr_type, false, &dummy_ids, st);
    } END_READ_LOCK;
}


PUBLIC void rs_rep_admin_stop
#ifndef __STDC
    (handle, st)
    handle_t            handle;
    error_status_t      *(st);
#else
    (
    handle_t            handle,
    error_status_t      *(st)
    )
#endif
{     
    rs_check_caller(handle, st);
    if (BAD_STATUS(st)) return;

    rs_audit_trail_log(handle, "Stop Server", (char *) NULL);

    shutdown();                     
    rpc_mgmt_stop_server_listening(NULL, st);

    printf("SECD: stopped by administrator\n");
}            


PUBLIC void rs_rep_admin_maint
#ifndef __STDC
    (handle, in_maintenance, st)
    handle_t            handle;
    boolean32           in_maintenance;
    error_status_t      *(st);
#else
    (
    handle_t            handle,
    boolean32           in_maintenance,
    error_status_t      *(st)
    )
#endif
{ 
    CLEAR_STATUS(st);
    rs_check_caller(handle, st);
    if (BAD_STATUS(st)) return;

    if (in_maintenance)
        rs_into_maintenance(handle, st);
    else
        rs_out_of_maintenance(handle, st);
}                                              


/*  r s _ r e p _ a d m i n _ m k e y 
 *                            
 *  The rs_reencrypt_db_keys_if_needed operation in
 *  rs_rpladmn_util.c corresponds closely to this routine
 *  in order to recover from a crash in rs_rep_admin_mkey.
 *  Changes to rs_rep_admin_mkey may require changes to
 *  rs_reencrypt_db_keys_if_needed.
 *
 */
PUBLIC void rs_rep_admin_mkey
#ifndef __STDC
    (handle, st)
    handle_t            handle;
    error_status_t      *(st);
#else
    (
    handle_t            handle,
    error_status_t      *(st)
    )
#endif
{     
    CLEAR_STATUS(st);

    rs_check_caller(handle, st);
    if (BAD_STATUS(st)) return;

    READ_INTEND_WRITE_LOCK(lock_db) {
        rs_audit_trail_log(handle, "Changing the master key", (char *) NULL);

        /* REPLICA: revisit rs_application_enabled call */
        if (rs_application_enabled(st) && 
            rs_state.db_state == rsdb_st_consistent) { 
            READ_INTEND_WRITE_PROMOTE(lock_db);
            /* Set database state to master_key changing */
            rs_state_save_state(rs_st_in_service, rsdb_st_mkey_changing, st);
            READ_INTEND_WRITE_DEMOTE(lock_db);
            if(STATUS_OK(st)) {
                /* riw_lock gets promoted/demoted inside rs_reencrypt_database */
                rs_reencrypt_database(rs_state.db_state, lock_mode_riw, st);
                if(STATUS_OK(st)) {
                    rs_audit_trail_log(handle, 
                                   "Database re-encrypted under new master key", 
                                   (char *) NULL);
                }
            }
        } else {
            if (STATUS_OK(st)) 
                SET_STATUS(st, sec_rgy_rep_bad_state);
        }
    } END_READ_INTEND_WRITE_LOCK; 
}            


      
#ifdef NOTDEF

PUBLIC void rs_rep_admin_info(handle, rep_info, st)
    handle_t           handle;
    rs_replica_info_t  *(rep_info);
    error_status_t     *(st);
{ 
    rep_info->rep_state = rs_state.rep_state;
    rep_info->rs_id = rs_state.rs_id;
    rs_replica_gen_timestamp(&rep_info->curr_time);
    rep_info->update_ts = rs_state.update_ts;

    rep_info->master = rs_state.master;
    rep_info->replist_readonly = rs_state.replist_readonly;
    rep_info->master_known = rs_state.master_known;
    
    /* XXX Do we have a replist id if the state is ! master_known?
       I think yes, since master_known can get cleared.  Check this out.
     */
    rep_info->replist_id = rs_state.replist_id;
    if (rs_state.master_known) {
        rep_info->master_ts = rs_state.master_ts;
        rep_info->master_id = rs_state.master_id;
    }

    CLEAR_STATUS(st);
}                                              

#endif /* NOTDEF */

