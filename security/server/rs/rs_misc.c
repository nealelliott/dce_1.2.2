/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_misc.c,v $
 * Revision 1.1.11.3  1996/02/18  00:18:34  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:07:14  marty]
 *
 * Revision 1.1.11.2  1995/12/08  17:53:11  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/4  1995/07/13  22:08 UTC  burati
 * 	merge fix for CHFts15709 from mb_mothra5 (uninitialized data)
 * 
 * 	HP revision /main/HPDCE02/mb_mothra5/2  1995/07/13  15:02 UTC  burati
 * 	remove gratuitous CLEAR_STATUS()
 * 
 * 	HP revision /main/HPDCE02/mb_mothra5/1  1995/07/13  00:05 UTC  burati
 * 	Check status from rs_misc_dce_aud_start
 * 
 * 	HP revision /main/HPDCE02/3  1995/04/04  17:31 UTC  burati
 * 	Merge from mb_mothra2
 * 
 * 	HP revision /main/HPDCE02/mb_mothra2/1  1995/03/28  21:02 UTC  burati
 * 	Fix auditing deadlock
 * 
 * 	HP revision /main/HPDCE02/2  1994/12/15  03:28 UTC  rps
 * 
 * 	HP revision /main/rps_mothra/1  1994/12/15  00:23 UTC  rps
 * 	[OT12669] cell renaming
 * 
 * 	HP revision /main/HPDCE02/1  1994/12/08  17:29 UTC  hanfei
 * 	merge
 * 
 * 	HP revision /main/hanfei_cpi_bl2/1  1994/12/07  19:32 UTC  hanfei
 * 	merge
 * 
 * 	HP revision /main/hanfei_cpi_bl1/2  1994/12/07  17:03 UTC  hanfei
 * 	change rs_checkpt_reset_interval interface
 * 
 * 	HP revision /main/hanfei_cpi_bl1/1  1994/12/05  14:49 UTC  hanfei
 * 	work for rgy checkpoint to be configurable
 * 	[1995/12/08  17:20:52  root]
 * 
 * Revision 1.1.7.3  1994/09/20  20:14:43  rps
 * 	[OT11944] add rs_check_consistency
 * 	[1994/09/20  20:10:05  rps]
 * 
 * Revision 1.1.7.2  1994/07/15  15:01:43  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:20:06  mdf]
 * 
 * Revision 1.1.6.3  1994/05/24  15:18:58  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 
 * Revision 1.1.6.2  1994/05/18  11:56:10  mdf
 * 	hp_sec_to_osf_3 drop
 * 
 * 	HP revision /main/ODESSA_2/1  1994/05/11  20:33 UTC  kline_s
 * 	 Second round of audit code point insertions.
 * 
 * 	HP revision /main/kline_s_bl6/1  1994/05/11  19:40 UTC  kline_s
 * 	Second round of audit code point insertions.
 * 
 * Revision 1.1.4.4  1992/12/29  16:35:52  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:24:37  zeliff]
 * 
 * Revision 1.1.4.3  1992/10/26  17:51:38  burati
 * 	CR5775 Check cell_prop for hidden passwds before passing it back
 * 	[1992/10/26  17:00:13  burati]
 * 
 * Revision 1.1.4.2  1992/10/07  20:34:17  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:17:32  sekhar]
 * 
 * Revision 1.1.2.2  1992/10/06  17:19:36  burati
 * 	Second replication drop: bl5
 * 	Add replication logic; code cleanup
 * 
 * Revision 1.1  1992/01/19  14:44:05  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
 * Copyright (c) Hewlett-Packard Company 1991, 1995
 * Unpublished work. All Rights Reserved.
 *
 */
/* 
 *      Registry Server - miscellaneous
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = 
    "$Id: rs_misc.c,v 1.1.11.3 1996/02/18 00:18:34 marty Exp $";
#endif

#include <u_str.h>

#include <rs_base.h>
#include <rs_pvt.h>

#include <rs_misc.h>
#include <dce/uuid.h>

#include <rs_lock.h>
#include <rs_ch_lock.h>


#include <rs_util.h>
#include <rs_auth.h>
#include <rs_secidmap.h>
#include <rsdb_acct.h>
#include <rsdb_pgo.h>
#include <rsdb_acl.h>
#include <rsdb_policy.h>
#include <rsdb_sysdep.h>

#if defined(DCE_AUD_POINTS)
#include <dce/audit.h>
#include <sec_aud_evts.h>
#endif	/* defined(DCE_AUD_POINTS) */

#if defined(DCE_AUD_POINTS)
extern dce_aud_trail_t sec_aud_trail;
#endif	/* defined(DCE_AUD_POINTS) */

#define LOGIN_RESULT(R) (R).tagged_union.result

#ifdef DCE_AUD_POINTS
PRIVATE void rs_misc_dce_aud_start(
    unsigned32 ev,
    rpc_binding_handle_t h,
    unsigned32 opt,
    unsigned32 outcome,
    dce_aud_rec_t *ard,
    error_status_t *stp
)
{
    rpc_authz_cred_handle_t	caller;
    unsigned32      		protect_level, authn_svc, authz_svc;
    sec_rgy_name_t              cell_name, princ_name;
    rsec_id_output_selector_t   selector = (rsec_id_output_select_puuid |
	rsec_id_output_select_cuuid);
    *ard = NULL;

    /* Find out if the caller was name based */
    rpc_binding_inq_auth_caller(h, &caller, NULL, &protect_level, &authn_svc,
        &authz_svc, stp);
    if (BAD_STATUS(stp)) {
	return;
    }

    /* If name based, do the following fast path code because I'm secd and
     * I can, whereas dce_aud_start() would work, but is slow (and could
     * cause deadlock in certain secd uses) because it has to work for any
     * caller.
     */
    if (authz_svc == rpc_c_authz_name) {
	unsigned_char_t *cname;
	uuid_t princ_id, cell_id;

        /* Get the client's name from the caller's creds */
	sec_cred_get_client_princ_name(caller, &cname, stp);
        if (BAD_STATUS(stp)) {
            return;
        }

	/* Internal/Local call to parse a name into uuids */
        rs_sec_id_parse_name(sec_rgy_domain_person, cname, selector, cell_name,
                             &cell_id, princ_name, &princ_id, stp);
	if (GOOD_STATUS(stp)) {

            /* Don't have svr uuid, but it isn't used by the auditing code
	     * anyway, so pass the client uuid as both the client and the svr.
	     */
            dce_aud_start_with_uuid(ev, princ_id, princ_id, cell_id, NULL,
                opt, outcome, ard, stp);

	} else if (STATUS_EQUAL(stp, sec_id_e_foreign_cell_referral)) {

	    /* If this is a foreign caller, must audit the hard way */
            dce_aud_start(ev, h, opt, outcome, ard, stp);
	}
    } else {
        dce_aud_start(ev, h, opt, outcome, ard, stp);
    }
}
#endif /* DCE_AUD_POINTS */


/*  r s _ l o g i n _ g e t _ i n f o
 *
 *  Auth_check for r-bit on person name.  If auth_check fails,
 *  allow name-based authentication for local principals only
 *  (for use by sec_login).
 */
PUBLIC void rs_login_get_info
  (
    handle_t               h,                  /* [in] */
    sec_rgy_login_name_t   *login_name,        /* [in, out] */
    rs_cache_data_t        *cache_info,        /* [out] */
    rs_login_info_t        *(result),          /* [out] */
    signed32               max_number,         /* [in] */
    signed32               *supplied_number,   /* [out] */
    uuid_t                 id_projlist[],      /* [out] */
    signed32               unix_projlist[],    /* [out] */
    signed32               *num_projects       /* [out] */
  )
{
    sec_rgy_cursor_t        cursor;
    sec_rgy_name_t          org_name;
    sec_rgy_properties_t    cell_props;
    error_status_t          *rstp;
    sec_acl_permset_t       requested_perms;
    sec_acl_permset_t       parent_perms = NULL;
    sec_rgy_foreign_id_t    client_ids;
    error_status_t          status;
    uuid_t  *per_mgr_type = &(rsdb_acl_mgr_uuids[rsdb_acl_mgr_type_person]);

#if defined(DCE_AUD_POINTS)
    dce_aud_rec_t             ard;
    dce_aud_ev_info_t         ev_info;
    error_status_t            aud_status;
#endif	/* defined(DCE_AUD_POINTS) */

    CLEAR_STATUS(&status);

#if defined(DCE_AUD_POINTS)
    rs_misc_dce_aud_start(LOGIN_GetInfo, h, 0, aud_c_esl_cond_unknown, 
                  &ard, &aud_status);
    if (GOOD_STATUS(&aud_status) && ard) {
        ev_info.format = aud_c_evt_info_char_string;
        ev_info.data.char_string = login_name->pname;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);
    }
#endif	/* defined(DCE_AUD_POINTS) */

    rstp = (error_status_t *) &(result->status);

    if (!CHECK_LOGIN_NAME(login_name)) {
        SET_STATUS(rstp, sec_rgy_bad_data);
        rs_util_bad_client(h, "rs_login_get_info", rstp);
        return;
    }

    cursor.valid = false;

    /* Account's person must provide read rights */
    requested_perms = sec_acl_perm_read;

    BEGIN_READ(lock_db, &status) {
        /* This auth call provides authorized access for local
         * clients coming in with only name-based authentication as
         * well as for dce-authenticated clients.
         */
        if(rs_auth_is_auth_by_name_ok(h, requested_perms, parent_perms,
                        sec_rgy_domain_person, login_name->pname, per_mgr_type,
                        false, &client_ids, &status)) {
            rsdb_acct_login_lookup(login_name, &cursor,
                                   &(LOGIN_RESULT(*result).sid),
                                   &(LOGIN_RESULT(*result).unix_sid),
                                   &(LOGIN_RESULT(*result).key_parts),
                                   &(LOGIN_RESULT(*result).user_part),
                                   &(LOGIN_RESULT(*result).admin_part),
                                   &status);
    
            if (STATUS_OK(&status)) {
                sec_rgy_cursor_t  projlist_cursor;
    
                rs_util_clear_cursor(&projlist_cursor);
                rsdb_acct_get_projlist(login_name, &projlist_cursor, 
                                       max_number, supplied_number, 
                                       id_projlist, unix_projlist,
                                       num_projects, &status);
            }
    
            if (STATUS_OK(&status)) {
                rsdb_properties_get_info(&cell_props, &status);
    
                if (STATUS_OK(&status)) {
                    if (FLAG_SET(cell_props.flags,sec_rgy_prop_shadow_passwd)) {
                        memset(LOGIN_RESULT(*result).user_part.passwd, 0,
                                sizeof(LOGIN_RESULT(*result).user_part.passwd));
                        strcpy((char *)LOGIN_RESULT(*result).user_part.passwd,
                                "*");
                    }
                    rsdb_pgo_uid_to_name(sec_rgy_domain_org,
                                         &(LOGIN_RESULT(*result).sid.org), 
                                         org_name, &status);
        
                    rsdb_policy_get_effective(org_name,
                      &(LOGIN_RESULT(*result).policy_data),
                      &status);
                }
            }
        }
    
    } END_READ

    if (STATUS_OK(&status)) {
	char *cell_name;

        LOGIN_RESULT(*result).cell_uuid = cell_props.realm_uuid;

	dce_cf_get_cell_name(&cell_name, &status);
	if (STATUS_OK(&status)) {
	    u_strcpy(LOGIN_RESULT(*result).cell_name, cell_name);
	    free(cell_name);
	}
    } else {
        /* avoid marshalling garbage into out parameters */
        *supplied_number = 0;
    }

    SET_STATUS(rstp, STATUS(&status));

#if defined(DCE_AUD_POINTS)
    if (ard) {    
        if (GOOD_STATUS(&status)) {
            dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                           aud_c_esl_cond_success, &aud_status); 
        } else {  
            if (rs_util_status_is_denial(status)) {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                               aud_c_esl_cond_denial, &aud_status);
            } else {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                               aud_c_esl_cond_failure, &aud_status);
            } 
        }
    }
#endif	/* defined(DCE_AUD_POINTS) */

    rs_util_get_cache_data(cache_info);
}

PUBLIC boolean32 rs_wait_until_consistent
  (
    handle_t         h,
    rs_cache_data_t *cache_info,
    error_status_t  *status
  )
{
    boolean32		retry;
    rs_update_seqno_t	seqno;

    /*
     * Conservative non-blocking/retrying support for old interface...
     * Return "some replica did not receive updates" rather than blocking.
     */
    seqno.high = NULL;
    seqno.low = NULL;
    (void) rs_check_consistency(h, &retry, &seqno, cache_info, status);
    return !retry;
}


PUBLIC boolean32 rs_check_consistency
  (
    handle_t		h,
    boolean32		*retry,
    rs_update_seqno_t	*seqno,
    rs_cache_data_t	*cache_info,
    error_status_t	*status
  )
{
    boolean32		result;

    CLEAR_STATUS(status);

    rs_util_get_cache_data(cache_info);

    READ_LOCK(lock_replist) {

	result = rs_m_replist_check_prop_seqno(retry, seqno, status);

    } END_READ_LOCK;

    return result;
}


PUBLIC void rs_checkpt_reset_interval
  (
    handle_t         h,
    boolean32        do_checkpt_now,
    signed32	     new_checkpt_interval,
    idl_char	     *at_time_str,
    rs_cache_data_t	*cache_info,
    error_status_t   *status
  )
{
    sec_acl_permset_t       requested_perms;
    sec_acl_permset_t       parent_perms;
    sec_rgy_name_t          *policy_obj_name;
    uuid_t                  *mgr_type;
    sec_rgy_foreign_id_t    dummy_ids;


    CLEAR_STATUS(status);

    rs_util_get_cache_data(cache_info);

    requested_perms=sec_acl_perm_mgmt_info;
    parent_perms=(sec_acl_permset_t)NULL;
    policy_obj_name = (sec_rgy_name_t *)SEC_POLICY_OBJ_NAME;
    mgr_type = &(rsdb_acl_mgr_uuids[rsdb_acl_mgr_type_rgy]);

    BEGIN_READ(lock_db, status) {
        rs_is_authorized(h, requested_perms, parent_perms, 
	      *policy_obj_name, mgr_type, false, &dummy_ids, status);
    } END_READ

    if ( GOOD_STATUS(status) ) {
	rsdb_checkpt_reset_interval(do_checkpt_now, new_checkpt_interval, 
		at_time_str, status);
    }
}
