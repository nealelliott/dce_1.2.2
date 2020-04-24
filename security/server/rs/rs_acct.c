/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_acct.c,v $
 * Revision 1.1.68.2  1996/10/03  15:08:54  arvind
 * 	Test code to debug private key retrieval.
 * 	[1996/09/16  23:06 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_1/1]
 *
 * 	CHFts19896: public key version support
 * 	[1996/09/13  23:43 UTC  aha  /main/DCE_1.2.2/aha_pk9_3/1]
 *
 * 	Change sec_passwd_privkey -> sec_passwd_pubkey
 * 	[1996/09/10  21:07 UTC  aha  /main/DCE_1.2.2/aha_pk9_2/3]
 *
 * 	Support versions for krbtgt public and private keys
 * 	[1996/09/06  18:19 UTC  aha  /main/DCE_1.2.2/aha_pk9_2/2]
 *
 * 	Check for NULL sec_passwd_plain passwd in rs_acct_replace(). CHFts19870.
 * 	[1996/08/23  19:59 UTC  jrr  /main/DCE_1.2.2/jrr_122_2/1]
 *
 * 	Merge up from aha_pk8
 * 	[1996/08/28  19:35 UTC  aha  /main/DCE_1.2.2/aha_pk8_1/1]
 *
 * 	Restore debug_pvtkey_retrieval for testing
 * 	[1996/08/27  22:25 UTC  aha  /main/DCE_1.2.2/aha_pk8/6]
 *
 * 	Merge-out
 * 	[1996/08/22  14:31 UTC  aha  /main/DCE_1.2.2/aha_pk8/5]
 *
 * 	Check for NULL sec_passwd_plain passwd in rs_acct_replace(). CHFts19870.
 * 	[1996/08/23  19:59 UTC  jrr  /main/DCE_1.2.2/jrr_122_2/1]
 *
 * 	Merge initial portion of KDC pvtkey retrieval
 * 	[1996/08/14  12:52 UTC  aha  /main/DCE_1.2.2/aha_dbpk1/2]
 *
 * 	*
 *
 * 	Merge Registry migration functionality: database reformat and enable sec_passwd_genprivkey
 * 	[1996/07/31  20:09 UTC  aha  /main/DCE_1.2.2/2]
 *
 * 	Include h/rs_ver_pvt.h for rs_c_sw_feature_kdcprivkey
 * 	[1996/07/18  18:31 UTC  aha  /main/aha_pk6/6]
 *
 * 	Changes for Public Key Login; set DCEPKPrivateKeyStorage ERA value
 * 	to sec_psm_kdc_pk_init_mechanism when a key-pair is generated for
 * 	the local krbtgt principal.
 * 	[1996/08/02  15:29 UTC  aha  /main/DCE_1.2.2/aha_pk8/3]
 *
 * 	More isolation of test hook
 * 	[1996/08/02  14:46 UTC  aha  /main/DCE_1.2.2/aha_pk8/2]
 *
 * 	#if test hook code in rs_acct_replace()
 * 	[1996/08/02  13:02 UTC  aha  /main/DCE_1.2.2/aha_pk8/1]
 *
 * 	Add temporary test hook for testing pvtkey_get()
 * 	[1996/07/18  17:33 UTC  aha  /main/DCE_1.2.2/1]
 *
 * 	Merge Registry support for KDC private key storage
 * 	[1996/07/13  20:49 UTC  aha  /main/aha_pk6/4]
 *
 * 	Merge-out and bugfixes and Registry work
 * 	[1996/06/19  20:33 UTC  aha  /main/aha_pk6/3]
 *
 * 	Changes for Public Key Login; fix call to rs_attr_update_verify_under_RIW
 * 	[1996/06/18  20:55 UTC  aha  /main/aha_pk6/2]
 *
 * 	Changes for Public Key Login; support Registry storage of KDC private key.
 * 	[1996/06/07  19:06 UTC  aha  /main/aha_pk6/1]
 *
 * 	Changes for Public Key Login; support Registry storage of KDC private key
 * 	[1994/07/15  15:01:13  mdf  1.1.64.3]
 *
 * Revision 1.1.68.1  1996/08/09  12:08:50  arvind
 * 	CHFts19677: accept only versions 0 and 1
 * 
 * 	Changes for Public Key Login; in rs_acct_replace(), retain the input
 * 	sec_passwd_genprivkey version number in the created
 * 	sec_passwd_privkey.
 * 	[1996/07/26  14:34 UTC  aha  /main/DCE_1.2.2/aha_dbpk1/1]
 * 
 * 	Merge Registry migration functionality: database reformat and enable sec_passwd_genprivkey
 * 	[1996/07/18  18:45 UTC  aha  /main/DCE_1.2.2/aha_pk7/1]
 * 
 * 	Merge Registry support for KDC private key storage
 * 	[1996/07/18  17:33 UTC  aha  /main/DCE_1.2.2/1]
 * 
 * 	Include h/rs_ver_pvt.h for rs_c_sw_feature_kdcprivkey
 * 	[1996/07/18  15:35 UTC  aha  /main/aha_pk6/5]
 * 
 * 	Changes for public key login; registry migration: create two functions to
 * 	check for valid password types - one for 122, the other for 121.  These
 * 	are then invoked from rs_acct_replace via an epv.
 * 	[1996/07/13  20:49 UTC  aha  /main/aha_pk6/4]
 * 
 * 	Merge-out and bugfixes and Registry work
 * 	[1996/06/19  20:33 UTC  aha  /main/aha_pk6/3]
 * 
 * 	Changes for Public Key Login; fix call to rs_attr_update_verify_under_RIW
 * 	[1996/06/18  20:55 UTC  aha  /main/aha_pk6/2]
 * 
 * 	Changes for Public Key Login; support Registry storage of KDC private key.
 * 	[1996/06/07  19:06 UTC  aha  /main/aha_pk6/1]
 * 
 * 	Changes for Public Key Login; support Registry storage of KDC private key
 * 	[1994/07/15  15:01:13  mdf  1.1.64.3]
 * 
 * Revision 1.1.66.2  1996/02/18  00:17:24  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:06:22  marty]
 * 
 * Revision 1.1.66.1  1995/12/08  17:50:35  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:20:11  root]
 * 
 * Revision 1.1.64.3  1994/07/15  15:01:13  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:19:02  mdf]
 * 
 * Revision 1.1.64.2  1994/05/11  19:28:48  ahop
 * 	hp_sec_to_osf_2 drop
 * 	Add support for password strength checking.
 * 	[1994/04/29  21:41:45  ahop]
 * 
 * Revision 1.1.5.4  1993/03/19  22:20:17  emartin
 * 	CR 7472 - rs_acct_add/replace - use slightly different primitives
 * 	to encrypt the unix passwd.
 * 	[1993/03/19  18:55:57  emartin]
 * 
 * 	CR 7472 rs_acct_add - generate unix_passwd wire rep, pass it
 * 	 to rs_log_acct_add.
 * 	rs_acct_replace - rsdb_acct_replace takes gen_unix_passwd flag (true).
 * 	 if passwd being changed, generate unix_passwd_wire rep and pass
 * 	 it to rs_log_acct_replace.
 * 	[1993/03/16  22:29:09  emartin]
 * 
 * Revision 1.1.5.3  1992/12/29  16:34:43  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:23:07  zeliff]
 * 
 * Revision 1.1.5.2  1992/10/07  20:32:21  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:15:58  sekhar]
 * 
 * Revision 1.1.3.2  1992/10/06  16:08:11  burati
 * 	Second replication drop: bl5
 * 	Add replication logic; code cleanup.
 * 
 * Revision 1.1.2.4  1992/07/08  01:16:56  ahop
 * 	  4429: in acct_replace, if password being replaced, check user permission bit
 * 	 [1992/07/06  22:52:33  ahop]
 * 
 * Revision 1.1.2.3  1992/06/18  15:26:41  burati
 * 	 CR3920 Make sure it always updates the "changed by" field
 * 	 [1992/06/17  21:18:28  burati]
 * 
 * Revision 1.1.2.2  1992/06/05  21:03:32  burati
 * 	 CR4023 Fixed bad status checking in rs_acct_get_projlist
 * 	 [1992/06/05  16:11:58  burati]
 * 
 * Revision 1.1  1992/01/19  14:44:17  devrcs
 * 	 Initial revision
 * 
 * $EndLog$
 */

/*
** Copyright (c) Hewlett-Packard Company 1991, 1992, 1994
** Unpublished work. All Rights Reserved.
*/

/* 
 *      Registry Server - Account Management
 */

#include <dce/rpc.h>
#include <dce/rgynbase.h>
#include <dce/passwd.h>
#include <rgymacro.h>
#include <dce/id_base.h>
#include <rs_acct.h>

#include <rs_pwd.h>
#include <rca_pwd.h>
#include <sec_encode.h>
#include <rs_pwd_mgmt_pvt.h>

#include <rs_base.h>
#include <rs_lock.h>
#include <rs_ch_lock.h>
#include <rs_auth.h>
#include <rs_log_acct.h>
#include <rs_audit_trail.h>
#include <rs_util.h>
#include <rs_attr_pvt.h>

#include <rsdb_util.h>
#include <rsdb_name_util.h>
#include <rsdb_acl.h>
#include <rsdb_acct.h>
#include <rsdb_policy.h>
#include <dce/sec_login.h>
#include <sec_pk.h>
#include <dce/sec_psm.h>
#include <sec_psm_base.h>

#include <crypt.h>

#include <bstring.h>
#include <un_strng.h>
#include <rs_ver_pvt.h>
#include <dce/idlbase.h>

#if defined(DCE_AUD_POINTS)
#include <dce/audit.h>
#include <sec_aud_evts.h>
#endif	/* defined(DCE_AUD_POINTS) */

#if defined(DCE_AUD_POINTS)
extern dce_aud_trail_t sec_aud_trail;
#endif	/* defined(DCE_AUD_POINTS) */

PRIVATE uuid_t  *per_mgr_type = &(rsdb_acl_mgr_uuids[rsdb_acl_mgr_type_person]);

PUBLIC void rs_acct_add
#ifndef __STDC__
    ( h, login_name, key_parts, user_part, admin_part, key, new_keytype,
      new_key_version, cache_info, status )
    handle_t                  h;                       /* [in] */
    sec_rgy_login_name_t      *login_name;             /* [in] */
    sec_rgy_acct_key_t        *key_parts;              /* [in, out] */
    sec_rgy_acct_user_t       *(user_part);            /* [in] */
    sec_rgy_acct_admin_t      *(admin_part);           /* [in] */
    rs_acct_key_transmit_t    *key;                    /* [in] */
    sec_passwd_type_t         new_keytype;             /* [in] */
    sec_passwd_version_t      *new_key_version;        /* [out] */
    rs_cache_data_t           *cache_info;             /* [out] */
    error_status_t            *(status);               /* [out] */
#else
  (
    handle_t                  h,                     
    sec_rgy_login_name_t      *login_name,           
    sec_rgy_acct_key_t        *key_parts,            
    sec_rgy_acct_user_t       *(user_part),          
    sec_rgy_acct_admin_t      *(admin_part),         
    rs_acct_key_transmit_t    *key,                  
    sec_passwd_type_t         new_keytype,           
    sec_passwd_version_t      *new_key_version,      
    rs_cache_data_t           *cache_info,           
    error_status_t            *(status)              
  )
#endif
{
    char                      buf[64];
    char                      name_buf[3 * sizeof(sec_rgy_name_t) + 1];
    sec_passwd_rec_t          *princ_key = NULL;
    sec_passwd_rec_t          new_key;
    sec_passwd_str_t          plain_passwd;
    error_status_t            tmp_status;
    sec_acl_permset_t         requested_perms;
    sec_acl_permset_t         parent_perms = NULL;
    boolean32                 check_quota = true;
    sec_rgy_foreign_id_t      client_ids;
    rs_acct_key_transmit_t    *unix_pwd_xmitrep = NULL;
    rs_replica_master_info_t  *master_info_p = NULL;

#if defined(DCE_AUD_POINTS)
    dce_aud_rec_t             ard;
    dce_aud_ev_info_t         ev_info;
    error_status_t            aud_status;
#endif	/* defined(DCE_AUD_POINTS) */

    CLEAR_STATUS(status);
    PLAIN_PASSWD(&new_key) = plain_passwd;

#if defined(DCE_AUD_POINTS)
    dce_aud_start(ACCT_Add, h, 0, aud_c_esl_cond_unknown, &ard, &aud_status);
#endif	/* defined(DCE_AUD_POINTS) */

    if (new_keytype != sec_passwd_des) {
        SET_STATUS(status, sec_rgy_key_bad_type);
    } else if (!CHECK_LOGIN_NAME(login_name) ||
               /* enforce person abbrev */
               !CHECK_ACCT_KEY_PARTS(key_parts) ||
               !CHECK_ACCT_USER_DATA(user_part)) {
        SET_STATUS(status, sec_rgy_bad_data);
    }
    if (BAD_STATUS(status)) {
        rs_util_bad_client(h, "rs_acct_add", status);
    }

    if (GOOD_STATUS(status)) {
        rsdb_name_canonicalize_and_convert(login_name->pname, status);
    }

#if defined(DCE_AUD_POINTS)
    if (ard) {
        sprintf(name_buf, "%s.%s.%s",
                (char *) login_name->pname,
                (char *) login_name->gname,
                (char *) login_name->oname);
        ev_info.format = aud_c_evt_info_char_string;
        ev_info.data.char_string = (idl_char *)name_buf;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);
        ev_info.format = aud_c_evt_info_long_int;
        ev_info.data.long_int = *key_parts;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);
    }
#endif	/* defined(DCE_AUD_POINTS) */

    if (GOOD_STATUS(status)) {
        sprintf(name_buf, "%s.%s.%s",
                (char *) login_name->pname,
                (char *) login_name->gname,
                (char *) login_name->oname);

        sprintf(buf, "Attempt Add account (%d)", *key_parts);

        BEGIN_MASTER_RIW(lock_db, status) {

            rs_audit_trail_log(h, buf, name_buf);

            /*
             * Account's person must provide mgmt_info, auth_info, and
             * user_info (mau) rights. 
             */
            requested_perms = sec_acl_perm_mgmt_info |
                sec_acl_perm_auth_info |
                sec_acl_perm_user_info;

            if (rs_auth_is_authorized(h, requested_perms, parent_perms,
                                      sec_rgy_domain_person, 
                                      login_name->pname, per_mgr_type,
                                      check_quota, &client_ids, status)) {

                rs_pwd_get_key_by_prnc(key, &client_ids.principal, 
                    &new_key, &princ_key, status);

                /*
                 * see if password selected by user should be
                 * strength checked
                 */
                if (GOOD_STATUS(status)) {
                    rs_pwd_mgmt_strength_check(login_name,
                       &new_key, status);
                }

                if (GOOD_STATUS(status)) {
                    admin_part->creator = client_ids;
                    admin_part->last_changer = client_ids;
                    rs_util_apply_timestamp(&admin_part->creation_date);
                    admin_part->change_date = admin_part->creation_date;
                    user_part->passwd_dtm = admin_part->creation_date;

                    RIW_PROMOTE(lock_db);
                    /* 
                     * We always generate a unix passwd, if possible,
                     * from the key provided by the application client,
                     * hence the "true" argument to rsdb_acct_add.  
                     */
                    rsdb_acct_add(login_name, *key_parts, user_part,
                                  admin_part, &new_key, true,
                                  new_key_version, status);

                    /*
                     * Now encrypt the unix password with the client
                     * principal's key (to yield a unix password 
                     * transmit rep for logging and propagating).
                     */
                    if (GOOD_STATUS(status)) {
                        rs_pwd_unix_pwd_to_xmit(
                            princ_key,
                            user_part->passwd,
                            &unix_pwd_xmitrep, status);
                    }

                    if (GOOD_STATUS(status)) {
                        rsdb_util_quota_decrement(&client_ids.principal,
                                              &client_ids.cell, &tmp_status);
    
                        sprintf(buf, "Add account (%d)", *key_parts);
                        rs_audit_trail_log(h, buf, (char *) name_buf);
#if defined(DCE_AUD_POINTS)
                        if (ard) {
                             dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait,
                                            1, aud_c_esl_cond_success, &aud_status); 
                        }
#endif	/* defined(DCE_AUD_POINTS) */
    
                        rs_log_acct_add(login_name, user_part, admin_part,
                                        key, &client_ids, new_keytype,
                                        unix_pwd_xmitrep,
                                        master_info_p);
                    }
                }
            } 
        } END_MASTER_RIW
    }
#if defined(DCE_AUD_POINTS)
    /* fall-through catch-all of all of the above status queries */
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

    if (unix_pwd_xmitrep != NULL)
        rca_pwd_free_transmit_rep(&unix_pwd_xmitrep); 

    if (princ_key != NULL)
        sec_crypt_free_key(&princ_key);

    rs_util_get_cache_data(cache_info);
}

PUBLIC void rs_acct_delete
#ifndef __STDC__
    ( h, login_name, cache_info, status ) 
    handle_t               h;              /* [in] */
    sec_rgy_login_name_t   *login_name;    /* [in] */
    rs_cache_data_t        *cache_info;    /* [out] */
    error_status_t         *(status);      /* [out] */
#else
  (
    handle_t               h,            
    sec_rgy_login_name_t   *login_name,  
    rs_cache_data_t        *cache_info,  
    error_status_t         *(status)
  )
#endif
{
    char                    name_buf[3 * sizeof(sec_rgy_name_t) + 1];
    sec_acl_permset_t       requested_perms;
    sec_acl_permset_t       parent_perms = NULL;
    sec_rgy_foreign_id_t    client_ids;                   

#if defined(DCE_AUD_POINTS)
    dce_aud_rec_t           ard;
    dce_aud_ev_info_t       ev_info;
    error_status_t          aud_status;
#endif	/* defined(DCE_AUD_POINTS) */

    CLEAR_STATUS(status);

#if defined(DCE_AUD_POINTS)
    dce_aud_start(ACCT_Delete, h, 0, aud_c_esl_cond_unknown, &ard, &aud_status);
#endif	/* defined(DCE_AUD_POINTS) */

    if (!CHECK_LOGIN_NAME(login_name) ||
        IS_WILD(login_name->pname)) {

        SET_STATUS(status, sec_rgy_bad_data);
        rs_util_bad_client(h, "rs_acct_delete", status);
    }

    if (GOOD_STATUS(status)) {
        rsdb_name_canonicalize_and_convert(login_name->pname, status);
    }

#if defined(DCE_AUD_POINTS)
    if (ard) {
        sprintf(name_buf, "%s.%s.%s",
                (char *) login_name->pname,
                (char *) login_name->gname,
                (char *) login_name->oname);
        ev_info.format = aud_c_evt_info_char_string;
        ev_info.data.char_string = (idl_char *)name_buf;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);
    }
#endif	/* defined(DCE_AUD_POINTS) */

    if (GOOD_STATUS(status)) {
        sprintf(name_buf, "%s.%s.%s",
                (char *) login_name->pname,
                (char *) login_name->gname,
                (char *) login_name->oname);

        BEGIN_MASTER_RIW(lock_db, status) {

            rs_audit_trail_log(h, "Attempt Delete Account", name_buf);

            /*
             * Account's person must provide mgmt_info, auth_info, and
             * user_info (mau) rights. 
             */
            requested_perms = sec_acl_perm_mgmt_info |
                sec_acl_perm_auth_info |
                sec_acl_perm_user_info;

            if (rs_auth_is_authorized(h, requested_perms, parent_perms,
                  sec_rgy_domain_person, login_name->pname, per_mgr_type,
                                      false, &client_ids, status)) {

                if (rs_util_pgo_is_required(sec_rgy_domain_person,
                                            login_name->pname, status)) {
                    SET_STATUS(status, sec_rgy_s_pgo_is_required);
                } 
                else {
                    RIW_PROMOTE(lock_db);
                    rsdb_acct_delete(login_name, status);

                    if (GOOD_STATUS(status)) {
                        rs_audit_trail_log(h, "Delete Account", name_buf);
#if defined(DCE_AUD_POINTS)
                        if (ard) {
                             dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait,
                                            1, aud_c_esl_cond_success, &aud_status); 
                        }
#endif	/* defined(DCE_AUD_POINTS) */

                        rs_log_acct_delete(login_name,
                                      (rs_replica_master_info_t *) NULL);
                    }
                }
            }
        } END_MASTER_RIW
    }
#if defined(DCE_AUD_POINTS)
    /* fall-through catch-all of all of the above status queries */
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
           
/* 
 * Client must have the m "management" permission on old_login_name.pname
 * to change the account group or org (rename).
 */
PUBLIC void rs_acct_rename
#ifndef __STDC__
    ( h, old_login_name, new_login_name, new_key_parts, status )
    handle_t               h;                /* [in] */
    sec_rgy_login_name_t   *old_login_name;  /* [in] */
    sec_rgy_login_name_t   *new_login_name;  /* [in] */
    sec_rgy_acct_key_t     *(new_key_parts); /* [in, out] */
    error_status_t         *(status);        /* [out] */
#else
  (
    handle_t               h,                /* [in] */
    sec_rgy_login_name_t   *old_login_name,  /* [in] */
    sec_rgy_login_name_t   *new_login_name,  /* [in] */
    sec_rgy_acct_key_t     *(new_key_parts), /* [in, out] */
    error_status_t         *(status)
  )
#endif
{
    char                    name_buf[3 * sizeof(sec_rgy_name_t) + 1];
    sec_acl_permset_t       requested_perms;
    sec_acl_permset_t       parent_perms = NULL;
    sec_rgy_foreign_id_t    client_ids;                   
    rs_replica_master_info_t  *master_info_p = NULL;

#if defined(DCE_AUD_POINTS)
    dce_aud_rec_t           ard;
    dce_aud_ev_info_t       ev_info;
    error_status_t          aud_status;
#endif	/* defined(DCE_AUD_POINTS) */

    CLEAR_STATUS(status);

#if defined(DCE_AUD_POINTS)
    dce_aud_start(ACCT_Rename, h, 0, aud_c_esl_cond_unknown, &ard, &aud_status);
#endif	/* defined(DCE_AUD_POINTS) */

    if (!CHECK_LOGIN_NAME(old_login_name) ||
        IS_WILD(old_login_name->pname)    ||
    !CHECK_LOGIN_NAME(new_login_name) ||
        IS_WILD(new_login_name->pname)    ||
        !CHECK_ACCT_KEY_PARTS(new_key_parts)) {     /* enforce person abbrev */
        SET_STATUS(status, sec_rgy_bad_data);
        rs_util_bad_client(h, "rs_acct_rename", status);
    } 

    if (GOOD_STATUS(status)) {
        rsdb_name_canonicalize_and_convert(old_login_name->pname, status);
    }
    if (GOOD_STATUS(status)) {
        rsdb_name_canonicalize_and_convert(new_login_name->pname, status);
    }

#if defined(DCE_AUD_POINTS)
    if (ard) {
        sprintf(name_buf, "%s.%s.%s",
                (char *) old_login_name->pname,
                (char *) old_login_name->gname,
                (char *) old_login_name->oname);
        ev_info.format = aud_c_evt_info_char_string;
        ev_info.data.char_string = (idl_char *)name_buf;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);

        sprintf(name_buf, "%s.%s.%s",
                (char *) new_login_name->pname,
                (char *) new_login_name->gname,
                (char *) new_login_name->oname);
        ev_info.format = aud_c_evt_info_char_string;
        ev_info.data.char_string = (idl_char *)name_buf;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);
    }
#endif	/* defined(DCE_AUD_POINTS) */

    if (GOOD_STATUS(status)) {

    sprintf(name_buf, "%s.%s.%s", 
                (char *) old_login_name->pname,
        (char *) old_login_name->gname,
        (char *) old_login_name->oname);

        BEGIN_MASTER_RIW(lock_db, status) {

            rs_audit_trail_log(h, "Attempt Rename Account", name_buf);

            /* Account's person must provide mgmt_info (m) rights.
             */
            requested_perms = sec_acl_perm_mgmt_info;
    
            if (rs_auth_is_authorized(h, requested_perms, parent_perms,
                sec_rgy_domain_person, old_login_name->pname, per_mgr_type,
                false, &client_ids, status)) {

                RIW_PROMOTE(lock_db);
                rsdb_acct_rename(old_login_name, new_login_name, 
                     new_key_parts, status);

                if (GOOD_STATUS(status)) {
                    sprintf(name_buf, "%s.%s.%s",
                        (char *) new_login_name->pname,
                        (char *) new_login_name->gname,
                        (char *) new_login_name->oname,
                        name_buf);
                    rs_audit_trail_log(h, "Rename Account", name_buf);
#if defined(DCE_AUD_POINTS)
                    if (ard) {
                         dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait,
                                        1, aud_c_esl_cond_success, &aud_status); 
                    }
#endif	/* defined(DCE_AUD_POINTS) */

                    rs_log_acct_rename(old_login_name, new_login_name,
                                       master_info_p);
                }
            }

        } END_MASTER_RIW
    }
#if defined(DCE_AUD_POINTS)
    /* fall-through catch-all of all of the above status queries */
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
}

#define ACCT_RESULT(R) (R).tagged_union.result

PUBLIC void rs_acct_lookup
#ifndef __STDC__
    ( h, login_name, cursor, cache_info, result )
    handle_t               h;              /* [in] */
    sec_rgy_login_name_t   *login_name;    /* [in, out] */
    sec_rgy_cursor_t       *(cursor);      /* [in, out] */
    rs_cache_data_t        *cache_info;    /* [out] */
    rs_acct_info_t         *(result);      /* [out] */
#else
  (
    handle_t               h,              /* [in] */
    sec_rgy_login_name_t   *login_name,    /* [in, out] */
    sec_rgy_cursor_t       *(cursor),      /* [in, out] */
    rs_cache_data_t        *cache_info,    /* [out] */
    rs_acct_info_t         *(result)
  )
#endif
{
    sec_rgy_login_name_t    save_login_name;
    error_status_t          status, auth_status;
    sec_acl_permset_t       requested_perms;
    sec_acl_permset_t       parent_perms = NULL;
    sec_rgy_foreign_id_t    client_ids;                   

#if defined(DCE_AUD_POINTS)
    char                    name_buf[3 * sizeof(sec_rgy_name_t) + 1];
    dce_aud_rec_t           ard;
    dce_aud_ev_info_t       ev_info;
    error_status_t          aud_status;
#endif	/* defined(DCE_AUD_POINTS) */

    CLEAR_STATUS(&status);
    SET_STATUS(&auth_status, sec_rgy_not_authorized);

#if defined(DCE_AUD_POINTS)
    dce_aud_start(ACCT_Lookup, h, 0, aud_c_esl_cond_unknown, &ard, &aud_status);
#endif	/* defined(DCE_AUD_POINTS) */

    if (!CHECK_LOGIN_NAME(login_name)) {
        SET_STATUS(&status, sec_rgy_bad_data);
        rs_util_bad_client(h, "rs_acct_lookup", &status);
    }

    if (GOOD_STATUS(&status)) {
        rsdb_name_canonicalize_and_convert(login_name->pname, &status);
    }

    /* Account's person must provide read rights */
    requested_perms = sec_acl_perm_read;
                                   
    if (GOOD_STATUS(&status)) {
        rs_util_check_cursor(cursor);
        COPY_LOGIN_NAME(&save_login_name, login_name);

        BEGIN_READ(lock_db, &status) {
            /* Do access check after the lookup so that
             * the login_name->pname is filled in.
             */
            while( GOOD_STATUS(&status) && BAD_STATUS(&auth_status) ) {
                rsdb_acct_lookup(login_name, cursor,
                                &(ACCT_RESULT(*result).sid), 
                                &(ACCT_RESULT(*result).unix_sid), 
                                &(ACCT_RESULT(*result).key_parts), 
                                &(ACCT_RESULT(*result).user_part), 
                                &(ACCT_RESULT(*result).admin_part), 
                                &status);
                if(login_name->pname[0] != '\0') {
                    /* if good status, but not authorized, lookup next.
                     * if bad status and not authorized, return auth status.
                     */
                    if(!rs_auth_is_authorized(h, requested_perms, parent_perms,
                        sec_rgy_domain_person, login_name->pname, per_mgr_type,
                        false, &client_ids, &auth_status)) {
                        if(GOOD_STATUS(&status)) {
                            COPY_LOGIN_NAME(login_name, &save_login_name);
                        } else {
                            COPY_STATUS(&auth_status, &status);
                        }
                    }
                }
            }

        } END_READ
    }

    COPY_STATUS(&status, &(result->status)); 

    if (GOOD_STATUS(&status)) {                                  
        /* if password shadowing set, copy asterisk into passwd field */
        if( rsdb_prop_flag_is_set(sec_rgy_prop_shadow_passwd) ) { 
            bzero( (Pointer)ACCT_RESULT(*result).user_part.passwd,
                   sizeof(ACCT_RESULT(*result).user_part.passwd) );
            strcpy((char *)(ACCT_RESULT(*result).user_part.passwd), "*" );       
        }
    }   

#if defined(DCE_AUD_POINTS)
    if (ard) {
        sprintf(name_buf, "%s.%s.%s",
                (char *) login_name->pname,
                (char *) login_name->gname,
                (char *) login_name->oname);
        ev_info.format = aud_c_evt_info_char_string;
        ev_info.data.char_string = (idl_char *)name_buf;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);

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

PUBLIC void rs_acct_replace
#ifndef __STDC__
    ( h, login_name, key_parts, modify_parts, user_part, admin_part, key, new_keytype,
         new_key_version, cache_info, status )
    handle_t                  h;                 /* [in] */
    sec_rgy_login_name_t      *login_name;       /* [in] */
    sec_rgy_acct_key_t        *key_parts;        /* [in, out] */
    rs_acct_parts_t           modify_parts;      /* [in] */
    sec_rgy_acct_user_t       *(user_part);      /* [in] */
    sec_rgy_acct_admin_t      *(admin_part);     /* [in] */
    rs_acct_key_transmit_t    *key;              /* [in] */
    sec_passwd_type_t         new_keytype;       /* [in] */
    sec_passwd_version_t      *new_key_version;  /* [out] */
    rs_cache_data_t           *cache_info;       /* [out] */
    error_status_t            *(status);         /* [out] */
#else
  (
    handle_t                  h,                 /* [in] */
    sec_rgy_login_name_t      *login_name,       /* [in] */
    sec_rgy_acct_key_t        *key_parts,        /* [in, out] */
    rs_acct_parts_t           modify_parts,      /* [in] */
    sec_rgy_acct_user_t       *(user_part),      /* [in] */
    sec_rgy_acct_admin_t      *(admin_part),     /* [in] */
    rs_acct_key_transmit_t    *key,              /* [in] */
    sec_passwd_type_t         new_keytype,       /* [in] */
    sec_passwd_version_t      *new_key_version,  /* [out] */
    rs_cache_data_t           *cache_info,       /* [out] */
    error_status_t            *(status)
  )
#endif
{
    char                      name_buf[3 * sizeof(sec_rgy_name_t) + 1];
    sec_passwd_rec_t          *princ_key = NULL;
    sec_passwd_rec_t          new_key; 
    sec_passwd_str_t          plain_passwd;
    boolean32                 passwd_valid = true;  
    boolean32                 key_parts_valid = true;  
    sec_acl_permset_t         requested_perms = NULL;
    sec_acl_permset_t         parent_perms = NULL;
    sec_rgy_foreign_id_t      client_ids;  
    sec_rgy_cursor_t          cursor;   
    sec_rgy_sid_t             old_sid;     
    sec_rgy_unix_sid_t        old_unix_sid;
    sec_rgy_acct_key_t        old_key_parts;
    sec_rgy_acct_user_t       old_user_part; 
    sec_rgy_acct_admin_t      old_admin_part;
    rs_acct_key_transmit_t    *unix_pwd_xmitrep = NULL; 
    sec_timeval_sec_t         now;              /* for key garbage collection */
    error_status_t            auth_status, lst;
    sec_pk_pubkey_t           sec_pk_pubkey;
    sec_pk_pvtkey_t           sec_pk_pvtkey;
    sec_rgy_name_t            component_name;
    sec_attr_t                in_attrs[2];
    signed32                  failure_index;
    rsdb_util_rgy_obj_info_t  rgy_obj_info;
    rsdb_attr_sch_list_t      *sch_list=NULL;
    signed32                  *xref=NULL;
    char		      buf[sizeof(sec_rgy_name_t) + 80];
    signed32                  i;
    boolean32                 svr_gen_key_xmit_rep=false;
    rs_acct_key_transmit_t    *key_internal;

#if defined(DCE_AUD_POINTS)
    dce_aud_rec_t             ard;
    dce_aud_ev_info_t         ev_info;
    error_status_t            aud_status;
#endif	/* defined(DCE_AUD_POINTS) */

    CLEAR_STATUS(status);
    PLAIN_PASSWD(&new_key) = plain_passwd;
    *plain_passwd = NULL;
    sec_pk_data_init(&sec_pk_pubkey);
    sec_pk_data_init(&sec_pk_pvtkey);

    /* key for internal manipulation */
    key_internal=key;

#if defined(DCE_AUD_POINTS)
    dce_aud_start(ACCT_Replace, h, 0, aud_c_esl_cond_unknown, &ard, &aud_status);
#endif	/* defined(DCE_AUD_POINTS) */

    if (modify_parts & rs_acct_part_login_name) {
       key_parts_valid = CHECK_ACCT_KEY_PARTS(key_parts);
    }

    if (modify_parts & rs_acct_part_passwd) {
      if (new_keytype == sec_passwd_genprivkey)
           rs_sw_feature_test_support(rs_c_sw_feature_kdcprivkey, status);
      else if (new_keytype != sec_passwd_des)
           SET_STATUS(status, sec_rgy_key_bad_type);
    }

    if (GOOD_STATUS(status)) {
        if (!CHECK_LOGIN_NAME(login_name) ||
            IS_WILD(login_name->pname) ||
            !key_parts_valid ||                  /* enforce person abbrev */
            !CHECK_ACCT_USER_DATA(user_part) ) {

            SET_STATUS(status, sec_rgy_bad_data);
        }
    }
    
    if (BAD_STATUS(status)) {
        rs_util_bad_client(h, "rs_acct_add", status);
    }

    if (GOOD_STATUS(status)) {
        rsdb_name_canonicalize_and_convert(login_name->pname, status);
    }

#if defined(DCE_AUD_POINTS)
    if (ard) {
        sprintf(name_buf, "%s.%s.%s",
                (char *) login_name->pname,
                (char *) login_name->gname,
                (char *) login_name->oname);
        ev_info.format = aud_c_evt_info_char_string;
        ev_info.data.char_string = (idl_char *)name_buf;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);
        ev_info.format = aud_c_evt_info_long_int;
        ev_info.data.long_int = *key_parts;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);
    }
#endif	/* defined(DCE_AUD_POINTS) */

    if (GOOD_STATUS(status)) {

        sprintf(name_buf, "%s [%s] [%s]", 
                (char *) login_name->pname,
                (char *) login_name->gname,
                (char *) login_name->oname);
        sprintf(buf, "Attempt Replace account (%d)", *key_parts);

        BEGIN_MASTER_RIW(lock_db, status) {

            rs_audit_trail_log(h, buf, name_buf);

            /*
             * lookup old account to see which fields in
             * admin part were changed on this replace.
             */
            cursor.valid = false;
            rsdb_acct_lookup(login_name, &cursor,
                             &old_sid, 
                             &old_unix_sid, 
                             &old_key_parts, 
                             &old_user_part, 
                             &old_admin_part, 
                             status);
            if (GOOD_STATUS(status)) {

                if (modify_parts & rs_acct_part_admin) {
                    rs_auth_acct_admin_update_check(&old_admin_part,
                             admin_part, &requested_perms, status);

                } else {

                    /*
                     * Always update the admin part to save the 
                     * last changer/date 
                     */
                    *admin_part = old_admin_part;
                    modify_parts |= rs_acct_part_admin;
                }
            }

            if (modify_parts & (rs_acct_part_user | rs_acct_part_passwd))
                requested_perms |= sec_acl_perm_user_info;

            /*
             * If not authorized, return authorization
             * status rather than object not found, etc.
             * to avoid database object existence probing.
             */
            if (!rs_auth_is_authorized(h, requested_perms,
                             parent_perms, sec_rgy_domain_person, 
                             login_name->pname, per_mgr_type,
                                       false, &client_ids, &auth_status))
                COPY_STATUS(&auth_status, status);

            /*
             * Catch all bad status up to here
             */
            if (BAD_STATUS(status))
                goto lock_out;

            if (modify_parts & rs_acct_part_passwd)
            {
/* XXX- TEST HOOK */
                static boolean32  debug_pvtkey_retrieval=false;
/* XXX- END TEST HOOK */
                /*
                 * Decode and decrypt the new key
                 */
                rs_pwd_get_key_by_prnc(key_internal, &client_ids.principal, 
                    &new_key, &princ_key, status);
                if (BAD_STATUS(status))
                    goto lock_out;

                if (debug_pvtkey_retrieval) {
/* XXX-TEST HOOK */
                    if (new_key.key.key_type == sec_passwd_genprivkey && new_key.version_number == 99) {
                        sec_psm_handle_t  psm_handle;
                        sec_pk_gen_data_t sign_data;
                        char              *to_be_signed="This is a test.  This is only a test.";
                        unsigned32        to_be_signed_len;
                        sec_pk_signed_t   signature;
                        unsigned32        kvno=0;

                        *status=sec_psm_open(login_name->pname,NULL,&sec_pk_domain_kdc_pk_init,&psm_handle);
                        if (BAD_STATUS(status)) {
                            goto lock_out;
                        }
                        sign_data.data=(byte *)to_be_signed;
                        sign_data.len=strlen(to_be_signed);
                        *status=sec_psm_sign_data(psm_handle,MD5RSA,sec_pk_usage_digitalSignature,
                                                  &sign_data, &kvno, &signature);
                        if (BAD_STATUS(status)) {
                           sec_psm_close(psm_handle);
                           goto lock_out;
                        }
                        sec_pk_data_free(&signature);
                        sec_psm_close(psm_handle);
                        goto lock_out;
                    }
                }

                /* Check for a NULL sec_passwd_plain passwd */
                if ((PASSWD_TYPE(&new_key) == sec_passwd_plain) &&
                    (PLAIN_PASSWD(&new_key) == NULL)) {
                    SET_STATUS(status, sec_rgy_bad_data);
                    goto lock_out;
                }

                /*
                 * see if password selected by user should be
                 * strength checked
                 */
                rs_pwd_mgmt_strength_check(login_name, &new_key, status);
                if (BAD_STATUS(status))
                    goto lock_out;

                /*
                 * See if password is a request to generate a
                 * private key
                 */
                if (new_keytype == sec_passwd_genprivkey)
                {
                    unsigned32       cur_ver;

                    /*
                     * Generate public key-pair
                     * XXX-temp: seed == 0
                     * Note that memory for keys must be freed
                     * using sec_pk_data_free
                     */
                    *status=sec_psm_gen_pub_key(RSA_PKCS,
                                                MOD_SIZE_PASSWD(&new_key),
                                                0,
                                                &sec_pk_pubkey,
                                                &sec_pk_pvtkey);
                    if (BAD_STATUS(status))
                        goto lock_out;

                    /* No need to deallocate new_key fields, because a
                     * sec_passwd_genprivkey has no dynamically allocated
                     * fields; shouldn't hurt to call 
                     * sec_encode_pwd_free(rpc_ss_free, &new_key), however
                     *
                     * Set new_key to a sec_passwd_pubkey; do not change
                     * the input version number, however, if it is not 0.
                     */
                    new_key.pepper=NULL;
                    PUBKEY_PASSWD_DATA(&new_key)=(byte *)rpc_ss_allocate(sec_pk_pvtkey.len);
                    if (PUBKEY_PASSWD_DATA(&new_key) == NULL)
                    {
                        *status=sec_s_no_memory;
                        goto lock_out;
                    }
                    PASSWD_TYPE(&new_key)=sec_passwd_pubkey;
                    new_keytype=sec_passwd_pubkey;
                    PUBKEY_PASSWD_LEN(&new_key)=sec_pk_pvtkey.len;
                    if (new_key.version_number == 0) {
                        rsdb_acct_get_cur_key_version(login_name,
                                                      sec_passwd_pubkey,
                                                      &cur_ver,
                                                      status);
                        if (cur_ver >= rsdb_auth_key_max_version_number)
                            new_key.version_number = rsdb_auth_key_min_version_number;
                        else
                            new_key.version_number = cur_ver+1;
                    }

                    /* Copy the private key data to the new_key; don't
                     * just assign pointer, because sec_pk_pvtkey.data
                     * was allocated using malloc, while new_key is allocated
                     * by rpc_ss_allocate
                     */
                    memcpy(PUBKEY_PASSWD_DATA(&new_key), sec_pk_pvtkey.data, sec_pk_pvtkey.len);

                    /* Don't free existing "key" because it was allocated
                     * by the IDL runtime and will automatically be
                     * deallocated upon exit from this function;
                     * rca_pwd_free_transmit_rep uses "free", which
                     * can't be used for memory allocated by the IDL
                     * runtime.
                     *
                     * Generate the transmit_rep of the new key for the log
                     * Don't need a login_handle, since no pepper used
                     */
                    key_internal=NULL;
                    rca_pwd_gen_transmit_rep((sec_login_handle_t)0,
                                             princ_key,
                                             &new_key,
                                             &key_internal,
                                             status);
                    if (BAD_STATUS(status))
                        goto lock_out;
                    else
                        /* Record that a new key was generated by the
                         * server (using the client function that uses
                         * malloc) so that it can be deallocated using
                         * the client function that uses free.
                         */
                        svr_gen_key_xmit_rep=true;

                    /*
                     * Get information required for writing the public key and
                     * the KDC pvtkey mechanism to ERAs
                     */
                    u_strcpy(component_name, "principal/");
                    u_strcat(component_name,login_name->pname);
                    *status=sec_pk_attr_write_setup(rpc_ss_allocate,
                                                    rpc_ss_free,
                                                    &in_attrs[0],
                                                    DCEPKAuthentication_id,
                                                    &new_key.version_number,
                                                    &sec_pk_pubkey);
                    if (BAD_STATUS(status))
                        goto lock_out;
                    in_attrs[1].attr_id=DCEPKPrivateKeyStorage_id;
                    in_attrs[1].attr_value.attr_encoding=sec_attr_enc_uuid;
                    in_attrs[1].attr_value.tagged_union.uuid=sec_psm_kdc_pk_init_mechanism;

                    rs_attr_update_verify_under_RIW(h,
                                                    (sec_attr_component_name_t)component_name,
                                                    2,
                                                    in_attrs,
                                                    &rgy_obj_info,
                                                    &failure_index,
                                                    &sch_list,
                                                    &xref,
                                                    status);
                    if (BAD_STATUS(status))
                        goto lock_out;
                }

                /* 
                 * Get the current time. It's needed for key garbage collection
                 */
                 rs_util_apply_timestamp(&now);
            }

            admin_part->last_changer = client_ids;
            rs_util_apply_timestamp(&admin_part->change_date);
            user_part->passwd_dtm = admin_part->change_date;

            RIW_PROMOTE(lock_db);

            /* For changes of the default password, we always
             * generate a unix password; for changes to the
             * private key, we do not generate a new unix password,
             * and the user_part->passwd field is left as is.
             */
            if (new_keytype!=sec_passwd_des)
                strcpy((char *)user_part->passwd, (char *)old_user_part.passwd);
            rsdb_acct_replace(login_name, 
                              *key_parts, modify_parts,
                              user_part,  admin_part, 
                              &new_key, (new_keytype==sec_passwd_des),
                              &now, new_key_version, status);
            if (BAD_STATUS(status))
                goto lock_out;

            if (modify_parts & rs_acct_part_passwd)
            {

                if (new_keytype == sec_passwd_pubkey)
                {
                    /*
                     * store public key half in DCEPKAuthentication ERA
                     */
                    sprintf(buf, "Attempting attribute updates on object %s", 
                            (char *)component_name);
                    rs_audit_trail_log(h, buf, (char *)component_name);
                    rsdb_attr_update(&rgy_obj_info, 2, in_attrs,
                                     sch_list, xref, &failure_index, status);
                    if(BAD_STATUS(status))
                        goto lock_out;

                    sprintf(buf, "Updated attributes on object %s", (char *)component_name);
                    rs_audit_trail_log(h, buf, (char *)component_name);
                    rs_log_attr_update(component_name, 2, in_attrs, NULL_PTR);
                }
                /* 
                 * Encrypt unix passwd with client 
                 * principal's key
                 */
                rs_pwd_unix_pwd_to_xmit(princ_key,
                                        user_part->passwd,
                                        &unix_pwd_xmitrep,
                                        status);
                if (BAD_STATUS(status))
                    goto lock_out;
            }

            sprintf(buf, "Replace account (%d)", *key_parts);
            rs_audit_trail_log(h, buf, name_buf);

#if defined(DCE_AUD_POINTS)
            if (ard) {
                 dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait,
                                1, aud_c_esl_cond_success, &aud_status); 
            }
#endif	/* defined(DCE_AUD_POINTS) */

            rs_log_acct_replace(login_name, modify_parts, user_part, admin_part, key_internal, 
                                &client_ids, new_keytype, unix_pwd_xmitrep, &now,
                                (rs_replica_master_info_t *) NULL);
lock_out:;
        } END_MASTER_RIW
    }

#if defined(DCE_AUD_POINTS)
    /* fall-through catch-all of all of the above status queries */
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

    if (unix_pwd_xmitrep != NULL)
        rca_pwd_free_transmit_rep(&unix_pwd_xmitrep);

    if (sec_pk_pvtkey.data)
        sec_pk_data_free(&sec_pk_pvtkey);

    if (sec_pk_pubkey.data)
        sec_pk_data_free(&sec_pk_pubkey);

    if (svr_gen_key_xmit_rep)
        /* A new transmit rep was generated by the server;
         * since it was allocated using malloc, we must
         * free it using the client function that uses
         * free.
         */
        rca_pwd_free_transmit_rep(&key_internal);

    if (princ_key != NULL)
        sec_crypt_free_key(&princ_key);

    if (sch_list) {
        /* free internal ptrs of each schema_entry allocated */
        for (i=0; i < sch_list->count; i++) {
	    sec_attr_util_sch_ent_free_ptrs(&(sch_list->entries[i].sch_entry));
	}
	free(sch_list);
    }

    if(xref)
	free(xref);

    rs_util_get_cache_data(cache_info);
}
      
PUBLIC void rs_acct_get_projlist
#ifndef __STDC__
    ( h, name, projlist_cursor, max_number,  number_supplied, id_projlist,
        unix_projlist, num_projects, cache_info, status )
    handle_t                h;                 /* [in] */
    sec_rgy_login_name_t    *name;             /* [in] */
    sec_rgy_cursor_t        *(projlist_cursor);/* [in, out] */
    signed32                max_number;        /* [in] */
    signed32                *number_supplied;  /* [out] */
    uuid_t                  id_projlist[];     /* [out] */
    signed32                unix_projlist[];   /* [out] */
    signed32                *num_projects;     /* [out] */
    rs_cache_data_t         *cache_info;       /* [out] */
    error_status_t          *(status);         /* [out] */
#else
  (
    handle_t                h,                 /* [in] */
    sec_rgy_login_name_t    *name,             /* [in] */
    sec_rgy_cursor_t        *(projlist_cursor),/* [in, out] */
    signed32                max_number,        /* [in] */
    signed32                *number_supplied,  /* [out] */
    uuid_t                  id_projlist[],     /* [out] */
    signed32                unix_projlist[],   /* [out] */
    signed32                *num_projects,     /* [out] */
    rs_cache_data_t         *cache_info,       /* [out] */
    error_status_t          *(status)
  )
#endif
{
    sec_acl_permset_t       requested_perms;
    sec_acl_permset_t       parent_perms = NULL;
    sec_rgy_foreign_id_t    client_ids;                   
    error_status_t          auth_status;

#if defined(DCE_AUD_POINTS)
    char                    name_buf[3 * sizeof(sec_rgy_name_t) + 1];
    dce_aud_rec_t           ard;
    dce_aud_ev_info_t       ev_info;
    error_status_t          aud_status;
#endif	/* defined(DCE_AUD_POINTS) */

    CLEAR_STATUS(status);

#if defined(DCE_AUD_POINTS)
    dce_aud_start(ACCT_GetProjlist, h, 0, aud_c_esl_cond_unknown, &ard, &aud_status);
#endif	/* defined(DCE_AUD_POINTS) */

    if (!CHECK_LOGIN_NAME(name)) {
        SET_STATUS(status, sec_rgy_bad_data);
        rs_util_bad_client(h, "rs_acct_get_projlist", status);
    }

    if (GOOD_STATUS(status)) {
        rsdb_name_canonicalize_and_convert(name->pname, status);
    }

    /* Account's person must provide read rights */
    requested_perms = sec_acl_perm_read;

    if (GOOD_STATUS(status)) {

        BEGIN_READ(lock_db, status) {

            /* Do access check after the lookup so that
             * the name->pname is filled in.
             */
            rsdb_acct_get_projlist(name, projlist_cursor, max_number,
                   number_supplied, id_projlist, unix_projlist,
                   num_projects,  status);
            if (GOOD_STATUS(status)) { 
                /* if not authorized, error status will be set. 
                 */ 
                rs_auth_is_authorized(h, requested_perms, parent_perms,
                sec_rgy_domain_person, name->pname, per_mgr_type,
                false, &client_ids, status);
            } 
            else if (!(IS_WILD(name->pname))) {
                /* if not authorized, auth status will be returned. 
                 */
                if(!rs_auth_is_authorized(h, requested_perms, parent_perms,
                    sec_rgy_domain_person, name->pname, per_mgr_type,
                    false, &client_ids, &auth_status)) {
                    COPY_STATUS(&auth_status, status);
                }   
            }

        } END_READ
    }

    if (BAD_STATUS(status)) {
        *number_supplied = 0;
        *num_projects = 0;
    }

#if defined(DCE_AUD_POINTS)
    if (ard) {
        sprintf(name_buf, "%s.%s.%s",
                (char *) name->pname,
                (char *) name->gname,
                (char *) name->oname);
        ev_info.format = aud_c_evt_info_char_string;
        ev_info.data.char_string = (idl_char *)name_buf;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);

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
