/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_policy.c,v $
 * Revision 1.1.8.2  1996/02/18  00:18:48  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:07:24  marty]
 *
 * Revision 1.1.8.1  1995/12/08  17:53:39  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:21:01  root]
 * 
 * Revision 1.1.6.2  1994/07/15  15:01:47  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:20:19  mdf]
 * 
 * Revision 1.1.6.1  1994/04/01  21:14:34  kline_s
 * 	[OT 10264] First audit code point drop for security.
 * 	[1994/04/01  20:53:37  kline_s]
 * 
 * Revision 1.1.4.3  1992/12/29  16:36:12  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:25:04  zeliff]
 * 
 * Revision 1.1.4.2  1992/10/07  20:35:05  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:18:01  sekhar]
 * 
 * Revision 1.1.2.2  1992/10/06  17:21:44  burati
 * 	Second replication drop: bl5
 * 
 * Revision 1.1  1992/01/19  14:44:11  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
 * Copyright (c) Hewlett-Packard Company 1991
 * Unpublished work. All Rights Reserved.
 */
/* 
 *      Registry Server - policy and properties management
 */

#ifdef MODULE_VERSION_ID
static char *VersionID =
    "$Id: rs_policy.c,v 1.1.8.2 1996/02/18 00:18:48 marty Exp $";
#endif

#include <un_strng.h>

#include <dce/nbase.h>
#include <dce/aclbase.h>
#include <rs_plcy.h>

#include <rs_server.h>
#include <rs_log_policy.h>
#include <rs_auth.h>
#include <rs_ch_lock.h>
#include <rsdb_policy.h>
#include <rsdb_acl.h>
#include <rsdb_util.h>
#include <rsdb_name_util.h>
#include <rsdb_acct.h>

#if defined(DCE_AUD_POINTS)
#include <dce/audit.h>
#include <sec_aud_evts.h>
#endif	/* defined(DCE_AUD_POINTS) */

#if defined(DCE_AUD_POINTS)
extern dce_aud_trail_t sec_aud_trail;
#endif	/* defined(DCE_AUD_POINTS) */

PRIVATE sec_rgy_name_t          plcy_name = SEC_POLICY_OBJ_NAME;
PRIVATE uuid_t  *plcy_mgr_type = &(rsdb_acl_mgr_uuids[rsdb_acl_mgr_type_rgy]);
PRIVATE sec_rgy_foreign_id_t    princ_ids;

PUBLIC void rs_properties_get_info
#ifndef __STDC__
    ( h, properties, cache_info, status )
    handle_t               h;              /* [in] */
    sec_rgy_properties_t   *(properties);  /* [out] */
    rs_cache_data_t        *cache_info;    /* [out] */
    error_status_t         *(status);      /* [out] */
#else
  (
    handle_t               h,              /* [in] */
    sec_rgy_properties_t   *(properties),  /* [out] */
    rs_cache_data_t        *cache_info,    /* [out] */
    error_status_t         *(status)
  )
#endif
{
    sec_acl_permset_t       requested_perms;
    sec_acl_permset_t       parent_perms;

#if defined(DCE_AUD_POINTS)
    dce_aud_rec_t           ard;
    error_status_t          aud_status;
#endif	/* defined(DCE_AUD_POINTS) */

    CLEAR_STATUS(status);
    requested_perms = sec_acl_perm_read;
    parent_perms = (sec_acl_permset_t)NULL;

#if defined(DCE_AUD_POINTS)
    dce_aud_start(PROP_GetInfo, h, 0, aud_c_esl_cond_unknown, 
                  &ard, &aud_status);
#endif	/* defined(DCE_AUD_POINTS) */

    BEGIN_READ(lock_db, status) {
        if(rs_is_authorized(h, requested_perms, parent_perms, plcy_name,
                            plcy_mgr_type, false, &princ_ids, status)) {
            rsdb_properties_get_info(properties, status);
        }
    } END_READ

#if defined(DCE_AUD_POINTS)
    if (ard) {
        if (GOOD_STATUS(status)) {
            dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                           aud_c_esl_cond_success, &aud_status); 
        } else {  
            if (rs_util_status_is_denial(*status)) {
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


PUBLIC void rs_properties_set_info
#ifndef __STDC__
    ( h, properties, cache_info, status )
    handle_t               h;             /* [in] */
    sec_rgy_properties_t   *(properties);  /* [out] */
    rs_cache_data_t        *cache_info;    /* [out] */
    error_status_t         *(status);      /* [out] */
#else
  (
    handle_t               h,             /* [in] */
    sec_rgy_properties_t   *(properties),  /* [out] */
    rs_cache_data_t        *cache_info,    /* [out] */
    error_status_t         *(status)
  )
#endif
{
    sec_acl_permset_t       requested_perms;
    sec_acl_permset_t       parent_perms;
#if defined(DCE_AUD_POINTS)
    dce_aud_rec_t           ard;
    error_status_t          aud_status;
#endif	/* defined(DCE_AUD_POINTS) */

    CLEAR_STATUS(status);
    requested_perms = sec_acl_perm_mgmt_info;
    parent_perms = (sec_acl_permset_t)NULL;

#if defined(DCE_AUD_POINTS)
    dce_aud_start(PROP_SetInfo, h, 0, aud_c_esl_cond_unknown, 
                  &ard, &aud_status);
#endif	/* defined(DCE_AUD_POINTS) */

    BEGIN_MASTER_RIW(lock_db, status) {
        rs_audit_trail_log(h, "Attempt set properties", NULL_PTR);
        if(rs_is_authorized(h, requested_perms, parent_perms, plcy_name,
                        plcy_mgr_type, false, &princ_ids, status)) {
            RIW_PROMOTE(lock_db);
            rsdb_properties_set_info(properties, status);
            if (STATUS_OK(status)) {
                rs_audit_trail_log(h, "Set properties", NULL_PTR);
#if defined(DCE_AUD_POINTS)
                if (ard) {
                     dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait,
                                    1, aud_c_esl_cond_success, &aud_status); 
                }
#endif	/* defined(DCE_AUD_POINTS) */
                rs_log_policy_props(properties, NULL_PTR);
            }
        }
    } END_MASTER_RIW;

#if defined(DCE_AUD_POINTS)
    if (BAD_STATUS(status)) {
        if (ard) {
            if (rs_util_status_is_denial(*status)) {
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


PUBLIC void rs_policy_get_info
#ifndef __STDC__
    ( h, organization, policy_data, cache_info, status )
    handle_t             h;               /* [in] */
    sec_rgy_name_t       organization;    /* [in] */
    sec_rgy_plcy_t       *(policy_data);  /* [out] */
    rs_cache_data_t      *cache_info;     /* [out] */
    error_status_t       *(status);       /* [out] */
#else
  (
    handle_t             h,               /* [in] */
    sec_rgy_name_t       organization,    /* [in] */
    sec_rgy_plcy_t       *(policy_data),  /* [out] */
    rs_cache_data_t      *cache_info,     /* [out] */
    error_status_t       *(status)
  )
#endif
{
    sec_acl_permset_t       requested_perms;
    sec_acl_permset_t       parent_perms;
    sec_rgy_name_t          full_org_name;
    uuid_t              *mgr_type = &(rsdb_acl_mgr_uuids[rsdb_acl_mgr_type_org]);
    boolean32               authorized;

#if defined(DCE_AUD_POINTS)
    dce_aud_rec_t           ard;
    dce_aud_ev_info_t       ev_info;
    error_status_t          aud_status;
#endif	/* defined(DCE_AUD_POINTS) */

    CLEAR_STATUS(status);
    requested_perms = sec_acl_perm_read;
    parent_perms = (sec_acl_permset_t)NULL;

#if defined(DCE_AUD_POINTS)
    dce_aud_start(POLICY_GetInfo, h, 0, aud_c_esl_cond_unknown, 
                  &ard, &aud_status);
    if (ard) {
        ev_info.format = aud_c_evt_info_char_string;
        ev_info.data.char_string = (idl_char *)organization;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);
    }
#endif	/* defined(DCE_AUD_POINTS) */

    if (!CHECK_PGO_NAME(organization)) {
        SET_STATUS(status, sec_rgy_bad_data);
        rs_util_bad_client(h, "rs_policy_get_info", status);
    }

    if (STATUS_OK(status)) {
        BEGIN_READ(lock_db, status) {
            if(RGY_POLICY(organization)) {
                authorized = rs_is_authorized(h, requested_perms, parent_perms,
                        plcy_name, plcy_mgr_type, false, &princ_ids, status);
            } else {
                rsdb_name_util_complete_name(sec_rgy_domain_org, organization,
                                        full_org_name, status);
                authorized = rs_is_authorized(h, requested_perms, parent_perms,
                             full_org_name, mgr_type, false, &princ_ids, status);
            }
            if(authorized) {
                rsdb_policy_get_info(organization, policy_data, status);
            }
        } END_READ
    }

#if defined(DCE_AUD_POINTS)
    if (ard) {
        if (GOOD_STATUS(status)) {
            dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                           aud_c_esl_cond_success, &aud_status); 
        } else {
            if (rs_util_status_is_denial(*status)) {
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


PUBLIC void rs_policy_set_info
#ifndef __STDC__
    ( h, organization, policy_data, cache_info, status )
    handle_t               h;             /* [in] */
    sec_rgy_name_t         organization;   /* [in] */
    sec_rgy_plcy_t         *(policy_data); /* [out] */
    rs_cache_data_t        *cache_info;    /* [out] */
    error_status_t         *(status);      /* [out] */
#else
  (
    handle_t               h,             /* [in] */
    sec_rgy_name_t         organization,   /* [in] */
    sec_rgy_plcy_t         *(policy_data), /* [out] */
    rs_cache_data_t        *cache_info,    /* [out] */
    error_status_t         *(status)
  )
#endif
{
    sec_acl_permset_t       requested_perms;
    sec_acl_permset_t       parent_perms;
    sec_rgy_name_t          full_org_name;
    uuid_t         *mgr_type = &(rsdb_acl_mgr_uuids[rsdb_acl_mgr_type_org]);
    boolean32               authorized;

#if defined(DCE_AUD_POINTS)
    dce_aud_rec_t           ard;
    dce_aud_ev_info_t       ev_info;
    error_status_t          aud_status;
#endif	/* defined(DCE_AUD_POINTS) */

    CLEAR_STATUS(status);
    requested_perms = sec_acl_perm_mgmt_info;
    parent_perms = (sec_acl_permset_t)NULL;

#if defined(DCE_AUD_POINTS)
    dce_aud_start(POLICY_SetInfo, h, 0, aud_c_esl_cond_unknown, 
                  &ard, &aud_status);
    if (ard) {
        ev_info.format = aud_c_evt_info_char_string;
        ev_info.data.char_string = (idl_char *)organization;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);
    }
#endif	/* defined(DCE_AUD_POINTS) */

    if (!CHECK_PGO_NAME(organization)) {
        SET_STATUS(status, sec_rgy_bad_data);
        rs_util_bad_client(h, "rs_policy_set_info", status);
    }

    if (STATUS_OK(status)) {
        BEGIN_MASTER_RIW(lock_db, status) {
            rs_audit_trail_log(h, "Attempt set policy", (char *) organization);
            if (RGY_POLICY(organization)) {
                authorized = rs_is_authorized(h, requested_perms,
                              parent_perms, plcy_name,
                              plcy_mgr_type, false, &princ_ids, status);
            } else {
                rsdb_name_util_complete_name(sec_rgy_domain_org, organization,
                                        full_org_name, status);
                authorized = rs_is_authorized(h, requested_perms,
                                  parent_perms, full_org_name,
                                  mgr_type, false, &princ_ids, status);
            }

            if (authorized) {
                RIW_PROMOTE(lock_db);
                rsdb_policy_set_info(organization, policy_data,
                                            status);
                if (STATUS_OK(status)) {
                    rs_audit_trail_log(h, "Set policy",
                                            (char *) organization);
#if defined(DCE_AUD_POINTS)
                    if (ard) {
                         dce_aud_commit(sec_aud_trail, ard, 
                                        aud_c_evt_commit_sync_no_wait, 1,
                                        aud_c_esl_cond_success, &aud_status); 
                    }
#endif	/* defined(DCE_AUD_POINTS) */
                    rs_log_policy_policy(organization, policy_data,
                                         NULL_PTR);
                }
            }
        } END_MASTER_RIW
    }

#if defined(DCE_AUD_POINTS)
    if (BAD_STATUS(status)) {
        if (ard) {
            if (rs_util_status_is_denial(*status)) {
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

/* Note: since rs_policy_get_effective reads both the registry policy
 * and the organization's policy, both the RGY and the ORG (if named)
 * objects must be authorized for this client.
 */
PUBLIC void rs_policy_get_effective
#ifndef __STDC__
    ( h, organization, policy_data, cache_info, status )
    handle_t               h;
    sec_rgy_name_t         organization;   /* [in ref] */
    sec_rgy_plcy_t         *(policy_data); /* [out] */
    rs_cache_data_t        *cache_info;    /* [out] */
    error_status_t         *(status);      /* [out] */
#else
  (
    handle_t               h,
    sec_rgy_name_t         organization,   /* [in ref] */
    sec_rgy_plcy_t         *(policy_data), /* [out] */
    rs_cache_data_t        *cache_info,    /* [out] */
    error_status_t         *(status)
  )
#endif
{
    sec_acl_permset_t       requested_perms;
    sec_acl_permset_t       parent_perms;
    sec_rgy_name_t          full_org_name;
    uuid_t         *mgr_type = &(rsdb_acl_mgr_uuids[rsdb_acl_mgr_type_org]);
    boolean32               org_authorized = true;
    boolean32               rgy_authorized;
    error_status_t          org_status;

#if defined(DCE_AUD_POINTS)
    dce_aud_rec_t           ard;
    dce_aud_ev_info_t       ev_info;
    error_status_t          aud_status;
#endif	/* defined(DCE_AUD_POINTS) */

    CLEAR_STATUS(status);
    CLEAR_STATUS(&org_status);
    requested_perms = sec_acl_perm_read;
    parent_perms = (sec_acl_permset_t)NULL;

#if defined(DCE_AUD_POINTS)
    /* Note: same audit point is used here as in rs_policy_get_info() */
    dce_aud_start(POLICY_GetInfo, h, 0, aud_c_esl_cond_unknown, 
                  &ard, &aud_status);
    if (ard) {
        ev_info.format = aud_c_evt_info_char_string;
        ev_info.data.char_string = (idl_char *)organization;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);
    }
#endif	/* defined(DCE_AUD_POINTS) */

    if (!CHECK_PGO_NAME(organization)) {
        SET_STATUS(status, sec_rgy_bad_data);
        rs_util_bad_client(h, "rs_policy_get_effective_info", status);
    }

    if (STATUS_OK(status)) {
        BEGIN_READ(lock_db, status) {
            if (!RGY_POLICY(organization)) {
                rsdb_name_util_complete_name(sec_rgy_domain_org, organization,
                                        full_org_name, status);
                org_authorized = rs_is_authorized(h, requested_perms,
                                 parent_perms, full_org_name,
                                 mgr_type, false, &princ_ids, &org_status);
            }
            rgy_authorized = rs_is_authorized(h, requested_perms,
                             parent_perms, plcy_name,
                             plcy_mgr_type, false, &princ_ids, status);
            if(rgy_authorized && org_authorized) {
                rsdb_policy_get_effective(organization, policy_data, status);
            } else {
                if( STATUS_EQUAL(&org_status, sec_rgy_object_not_found) ||
                    STATUS_EQUAL(status, sec_rgy_object_not_found) ) {
                    SET_STATUS(status, sec_rgy_object_not_found);
                } else {
                    SET_STATUS(status, sec_rgy_not_authorized);
                }
            }
        } END_READ
    }

#if defined(DCE_AUD_POINTS)
    if (ard) {
        if (GOOD_STATUS(status)) {
            dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                           aud_c_esl_cond_success, &aud_status); 
        } else {
            if (rs_util_status_is_denial(*status)) {
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

/* Override info is no longer provided by secd.
 * rsdb_policy_get_override_info returns not_implemented.
 * This operation remains simply to maintain i/f consistency.
 */
PUBLIC void rs_policy_get_override_info
#ifndef __STDC__
    ( h, category, p_d, cache_info, st )
    handle_t                        h;
    sec_rgy_name_t                  category;
    rs_policy_override_flags_t      *p_d;
    rs_cache_data_t                 *cache_info;
    error_status_t                  *st;
#else
  (
    handle_t                        h,
    sec_rgy_name_t                  category,
    rs_policy_override_flags_t      *p_d,
    rs_cache_data_t                 *cache_info,
    error_status_t                  *st
  )
#endif
{
    CLEAR_STATUS(st);

    if (!CHECK_PGO_NAME(category)) {
        SET_STATUS(st, sec_rgy_bad_data);
        rs_util_bad_client(h, "rs_policy_get_override_info", st);
    }

    if (STATUS_OK(st)) {
        BEGIN_READ(lock_db, st) {
            rsdb_policy_get_override_info(category, p_d, st);
        } END_READ
    }

    rs_util_get_cache_data(cache_info);
}

/* Override info is no longer provided by secd.
 * rsdb_policy_get_override_info returns not_implemented.
 * This operation remains simply to maintain i/f consistency.
 */
PUBLIC void rs_policy_set_override_info
#ifndef __STDC__
    ( h, category, p_d, cache_info, st )
    handle_t                        h;
    sec_rgy_name_t                  category;
    rs_policy_override_flags_t      p_d;
    rs_cache_data_t                 *cache_info;
    error_status_t                  *st;
#else
  (
    handle_t                        h,
    sec_rgy_name_t                  category,
    rs_policy_override_flags_t      p_d,
    rs_cache_data_t                 *cache_info,
    error_status_t                  *st
  )
#endif
{

    CLEAR_STATUS(st);

    if (!CHECK_PGO_NAME(category)) {
        SET_STATUS(st, sec_rgy_bad_data);
        rs_util_bad_client(h, "rs_policy_set_override_info", st);
    }

    if (STATUS_OK(st)) {
        BEGIN_MASTER_RIW(lock_db, st) {
            rs_audit_trail_log(h, "Set overide info", (char *) category);
            RIW_PROMOTE(lock_db);

            rsdb_policy_set_override_info(category, p_d, st);

            if (STATUS_OK(st)) {
                rs_audit_trail_log(h, "Set overide info", (char *) category);
                rs_log_policy_override(category, p_d, NULL_PTR);
            }
        } END_MASTER_RIW
    }

    rs_util_get_cache_data(cache_info);
}


PUBLIC void  rs_auth_policy_get_info
#ifndef __STDC__
    ( h, account, auth_policy, cache_info, status )
    handle_t               h;
    sec_rgy_login_name_t   *account;         /* [in ref] */
    sec_rgy_plcy_auth_t    *(auth_policy);   /* [in] */
    rs_cache_data_t        *(cache_info);    /* [out] */
    error_status_t         *(status);        /* [out] */
#else
  (
    handle_t               h,
    sec_rgy_login_name_t   *account,         /* [in ref] */
    sec_rgy_plcy_auth_t    *(auth_policy),   /* [in] */
    rs_cache_data_t        *(cache_info),    /* [out] */
    error_status_t         *(status)
  )
#endif
{
    sec_acl_permset_t       requested_perms;
    sec_acl_permset_t       parent_perms;
    sec_rgy_name_t          full_person_name;
    uuid_t      *mgr_type = &(rsdb_acl_mgr_uuids[rsdb_acl_mgr_type_person]);
    boolean32               authorized = true;

#if defined(DCE_AUD_POINTS)
    char                    name_buf[3 * sizeof(sec_rgy_name_t) + 1];
    dce_aud_rec_t           ard;
    dce_aud_ev_info_t       ev_info;
    error_status_t          aud_status;
#endif	/* defined(DCE_AUD_POINTS) */

    CLEAR_STATUS(status);
    requested_perms = sec_acl_perm_read;
    parent_perms = (sec_acl_permset_t)NULL;

#if defined(DCE_AUD_POINTS)
    dce_aud_start(AUTHPOLICY_GetInfo, h, 0, aud_c_esl_cond_unknown, 
                  &ard, &aud_status);
    if (ard) {
        sprintf(name_buf, "%s.%s.%s", (char *) account->pname,
               (char *) account->gname, (char *) account->oname);
        ev_info.format = aud_c_evt_info_char_string;
        ev_info.data.char_string = (idl_char *)name_buf;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);
}
#endif	/* defined(DCE_AUD_POINTS) */

    if ( !CHECK_LOGIN_NAME(account) ||
         (!RGY_AUTH_POLICY(account) && IS_WILD(account->pname)) ) {
        SET_STATUS(status, sec_rgy_bad_data);
        rs_util_bad_client(h, "rs_auth_policy_get_info", status);
    }

    if (STATUS_OK(status)) {
        BEGIN_READ(lock_db, status) {
            if(RGY_AUTH_POLICY(account)) {
                authorized = rs_is_authorized(h, requested_perms, parent_perms,
                             plcy_name, plcy_mgr_type, false, &princ_ids, status);
            } else {
                rsdb_name_util_complete_name(sec_rgy_domain_person,
                                            account->pname,
                                            full_person_name, status);
                authorized = rs_is_authorized(h, requested_perms,
                                 parent_perms, full_person_name,
                                 mgr_type, false, &princ_ids, status);
            }
            if(authorized) {
                rsdb_auth_policy_get_info(account, auth_policy, status);
            }
        } END_READ
    }

#if defined(DCE_AUD_POINTS)
    if (ard) {
        if (GOOD_STATUS(status)) {
            dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                           aud_c_esl_cond_success, &aud_status); 
        } else {  
            if (rs_util_status_is_denial(*status)) {
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

PUBLIC void rs_auth_policy_set_info
#ifndef __STDC__
    ( h, account, auth_policy, cache_info, status )
    handle_t               h;
    sec_rgy_login_name_t   *account;         /* [in ref] */
    sec_rgy_plcy_auth_t    *(auth_policy);   /* [in] */
    rs_cache_data_t        *(cache_info);    /* [out] */
    error_status_t         *(status);        /* [out] */
#else
  (
    handle_t               h,
    sec_rgy_login_name_t   *account,         /* [in ref] */
    sec_rgy_plcy_auth_t    *(auth_policy),   /* [in] */
    rs_cache_data_t        *(cache_info),    /* [out] */
    error_status_t         *(status)
  )
#endif
{
    sec_acl_permset_t       requested_perms;
    sec_acl_permset_t       parent_perms;
    sec_rgy_name_t          full_person_name;
    uuid_t          *mgr_type = &(rsdb_acl_mgr_uuids[rsdb_acl_mgr_type_person]);
    boolean32               authorized;
    char                    name_buf[3 * sizeof(sec_rgy_name_t) + 1];

#if defined(DCE_AUD_POINTS)
    dce_aud_rec_t           ard;
    dce_aud_ev_info_t       ev_info;
    error_status_t          aud_status;
#endif	/* defined(DCE_AUD_POINTS) */

    CLEAR_STATUS(status);
    requested_perms = sec_acl_perm_auth_info;
    parent_perms = (sec_acl_permset_t)NULL;

#if defined(DCE_AUD_POINTS)
    dce_aud_start(AUTHPOLICY_SetInfo, h, 0, aud_c_esl_cond_unknown, 
                  &ard, &aud_status);
    if (ard) {
        sprintf(name_buf, "%s.%s.%s", (char *) account->pname,
               (char *) account->gname, (char *) account->oname);
        ev_info.format = aud_c_evt_info_char_string;
        ev_info.data.char_string = (idl_char *)name_buf;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);
    }
#endif	/* defined(DCE_AUD_POINTS) */

    if ( !CHECK_LOGIN_NAME(account) ||
         (!RGY_AUTH_POLICY(account) && IS_WILD(account->pname)) ) {
        SET_STATUS(status, sec_rgy_bad_data);
        rs_util_bad_client(h, "rs_auth_policy_set_info", status);
    }

   if (STATUS_OK(status)) {
        BEGIN_MASTER_RIW(lock_db, status) {
            sprintf(name_buf, "%s.%s.%s", (char *) account->pname,
                    (char *) account->gname, (char *) account->oname);
            rs_audit_trail_log(h, "Attempt set auth policy", name_buf);
            if(RGY_AUTH_POLICY(account)) {
                authorized = rs_is_authorized(h, requested_perms,
                             parent_perms, plcy_name, plcy_mgr_type,
                             false, &princ_ids, status);
            } else {
                rsdb_name_util_complete_name(sec_rgy_domain_person,
                                            account->pname,
                                            full_person_name, status);
                authorized = rs_is_authorized(h, requested_perms,
                                 parent_perms, full_person_name,
                                 mgr_type, false, &princ_ids, status);
            }
            if (authorized) {
                RIW_PROMOTE(lock_db);
                rsdb_auth_policy_set_info(account, auth_policy,
                                            status);
                if (STATUS_OK(status)) {
                    rs_audit_trail_log(h, "Set auth policy", name_buf);
#if defined(DCE_AUD_POINTS)
                    if (ard) {
                         dce_aud_commit(sec_aud_trail, ard, 
                                        aud_c_evt_commit_sync_no_wait, 1,
                                        aud_c_esl_cond_success, &aud_status); 
            }
#endif	/* defined(DCE_AUD_POINTS) */
                    rs_log_policy_auth_policy(account, auth_policy,
                                              NULL_PTR);
                }
            }
        } END_MASTER_RIW
    }

#if defined(DCE_AUD_POINTS)
    if (BAD_STATUS(status)) {
        if (ard) {
            if (rs_util_status_is_denial(*status)) {
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



/* Note: since rs_auth_policy_get_effective reads both the registry auth_policy
 * and the account's auth_policy, both the RGY and the account's PERSON
 * (if named) objects must be authorized for this client.
 */
PUBLIC void  rs_auth_policy_get_effective
#ifndef __STDC__
    ( h, account, auth_policy, cache_info, status )
    handle_t               h;
    sec_rgy_login_name_t   *account;          /* [in ref] */
    sec_rgy_plcy_auth_t    *(auth_policy);    /* [out] */
    rs_cache_data_t        *(cache_info);     /* [out] */
    error_status_t         *(status);         /* [out] */
#else
  (
    handle_t               h,
    sec_rgy_login_name_t   *account,          /* [in ref] */
    sec_rgy_plcy_auth_t    *(auth_policy),    /* [out] */
    rs_cache_data_t        *(cache_info),     /* [out] */
    error_status_t         *(status)
  )
#endif
{
    sec_acl_permset_t       requested_perms;
    sec_acl_permset_t       parent_perms;
    sec_rgy_name_t          full_person_name;
    uuid_t         *mgr_type = &(rsdb_acl_mgr_uuids[rsdb_acl_mgr_type_person]);
    boolean32               person_authorized = true;
    boolean32               rgy_authorized;
    error_status_t          person_status;

#if defined(DCE_AUD_POINTS)
    char                    name_buf[3 * sizeof(sec_rgy_name_t) + 1];
    dce_aud_rec_t           ard;
    dce_aud_ev_info_t       ev_info;
    error_status_t          aud_status;
#endif	/* defined(DCE_AUD_POINTS) */

    CLEAR_STATUS(status);
    CLEAR_STATUS(&person_status);
    requested_perms = sec_acl_perm_read;
    parent_perms = (sec_acl_permset_t)NULL;

#if defined(DCE_AUD_POINTS)
    /* Note: same audit point is used here as in rs_auth_policy_get_info() */
    dce_aud_start(AUTHPOLICY_GetInfo, h, 0, aud_c_esl_cond_unknown, 
                  &ard, &aud_status);
    if (ard) {
        sprintf(name_buf, "%s.%s.%s", (char *) account->pname,
               (char *) account->gname, (char *) account->oname);
        ev_info.format = aud_c_evt_info_char_string;
        ev_info.data.char_string = (idl_char *)name_buf;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);
    }
#endif	/* defined(DCE_AUD_POINTS) */

    if ( !CHECK_LOGIN_NAME(account) ||
         /* if account is named, pname can't be null */
         (!RGY_AUTH_POLICY(account) && IS_WILD(account->pname)) ) {
        SET_STATUS(status, sec_rgy_bad_data);
        rs_util_bad_client(h, "rs_auth_policy_get_effective", status);
    }

    if (STATUS_OK(status)) {
        BEGIN_READ(lock_db, status) {
            if (!RGY_AUTH_POLICY(account)) {
                rsdb_name_util_complete_name(sec_rgy_domain_person,
                                             account->pname,
                                             full_person_name, status);
                person_authorized = rs_is_authorized(h, requested_perms,
                                 parent_perms, full_person_name,
                                 mgr_type, false, &princ_ids, &person_status);
            }
            rgy_authorized = rs_is_authorized(h, requested_perms,
                             parent_perms, plcy_name,
                             plcy_mgr_type, false, &princ_ids, status);
            if(rgy_authorized && person_authorized) {
                rsdb_auth_policy_get_effective(account, auth_policy, status);
            } else {
                if( STATUS_EQUAL(&person_status, sec_rgy_object_not_found) ||
                    STATUS_EQUAL(status, sec_rgy_object_not_found) ) {
                    SET_STATUS(status, sec_rgy_object_not_found);
                } else {
                    SET_STATUS(status, sec_rgy_not_authorized);
                }
            }
        } END_READ
    }
#if defined(DCE_AUD_POINTS)
    if (ard) {
        if (GOOD_STATUS(status)) {
            dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                           aud_c_esl_cond_success, &aud_status); 
        } else {  
            if (rs_util_status_is_denial(*status)) {
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
