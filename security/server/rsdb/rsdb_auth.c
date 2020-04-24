/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsdb_auth.c,v $
 * Revision 1.1.10.2  1996/10/03  15:12:58  arvind
 * 	CHFts19896: public key version support
 * 	[1996/09/13  23:43 UTC  aha  /main/DCE_1.2.2/aha_pk9_3/1]
 *
 * 	Change sec_passwd_privkey -> sec_passwd_pubkey
 * 	[1996/09/10  21:07 UTC  aha  /main/DCE_1.2.2/aha_pk9_2/2]
 *
 * 	Support version for krbtgt private keys
 * 	[1996/09/06  18:19 UTC  aha  /main/DCE_1.2.2/aha_pk9_2/1]
 *
 * 	Merge Registry migration functionality: database reformat and enable sec_passwd_genprivkey
 * 	[1996/07/31  15:38 UTC  aha  /main/DCE_1.2.2/aha_pk7/3]
 *
 * 	Make rsdb_auth_init_privkey_allowed() more robust per code-review:
 * 	verify conversion from cell_name to local krbtgt principal name,
 * 	check for allocated cell_name prior to free()
 * 	[1996/07/26  21:56 UTC  aha  /main/DCE_1.2.2/aha_pk7/2]
 *
 * 	Fix krbtgt principal name in rsdb_auth_init_privkey_allowed()
 * 	[1996/07/18  18:45 UTC  aha  /main/DCE_1.2.2/aha_pk7/1]
 *
 * 	Fix syntax error in rsdb_auth_init_privkey_allowed()
 * 	[1996/07/18  15:28 UTC  aha  /main/aha_pk6/4]
 *
 * 	Changes for Public Key Login; registry migration; provide function
 * 	to initialize accounts where rsdb_acct_item_privkey_ok flag should
 * 	be set
 * 	[1996/07/15  15:17 UTC  aha  /main/aha_pk6/3]
 *
 * 	Changes for Public Key Login; support Registry storage of KDC private key:
 * 	verify that version number is 1 for sec_passwd_genprivkey and sec_passwd_privkey.
 * 	[1996/07/13  20:49 UTC  aha  /main/aha_pk6/2]
 *
 * 	Merge-out and bugfixes and Registry work
 * 	[1996/06/18  20:55 UTC  aha  /main/aha_pk6/1]
 *
 * 	Changes for Public Key Login; support Registry storage of KDC private key
 * 	[1996/02/18  00:21:10  marty  1.1.8.2]
 *
 * Revision 1.1.10.1  1996/08/09  12:11:15  arvind
 * 	Merge Registry migration functionality: database reformat and enable sec_passwd_genprivkey
 * 	[1996/07/31  15:38 UTC  aha  /main/DCE_1.2.2/aha_pk7/3]
 * 
 * 	Make rsdb_auth_init_privkey_allowed() more robust per code-review:
 * 	verify conversion from cell_name to local krbtgt principal name,
 * 	check for allocated cell_name prior to free()
 * 	[1996/07/26  21:56 UTC  aha  /main/DCE_1.2.2/aha_pk7/2]
 * 
 * 	Fix krbtgt principal name in rsdb_auth_init_privkey_allowed()
 * 	[1996/07/18  18:45 UTC  aha  /main/DCE_1.2.2/aha_pk7/1]
 * 
 * 	Merge Registry support for KDC private key storage
 * 	[1996/07/18  17:34 UTC  aha  /main/DCE_1.2.2/1]
 * 
 * 	Fix syntax error in rsdb_auth_init_privkey_allowed()
 * 	[1996/07/18  15:28 UTC  aha  /main/aha_pk6/4]
 * 
 * 	Changes for Public Key Login; registry migration; provide function
 * 	to initialize accounts where rsdb_acct_item_privkey_ok flag should
 * 	be set
 * 	[1996/07/15  15:17 UTC  aha  /main/aha_pk6/3]
 * 
 * 	Changes for Public Key Login; support Registry storage of KDC private key:
 * 	verify that version number is 1 for sec_passwd_genprivkey and sec_passwd_privkey.
 * 	[1996/07/13  20:49 UTC  aha  /main/aha_pk6/2]
 * 
 * 	Merge-out and bugfixes and Registry work
 * 	[1996/06/18  20:55 UTC  aha  /main/aha_pk6/1]
 * 
 * 	Changes for Public Key Login; support Registry storage of KDC private key
 * 	[1996/02/18  00:21:10  marty  1.1.8.2]
 * 
 * Revision 1.1.8.2  1996/02/18  00:21:10  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:09:03  marty]
 * 
 * Revision 1.1.8.1  1995/12/08  17:58:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:22:09  root]
 * 
 * Revision 1.1.6.1  1994/08/04  16:14:29  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:13:25  mdf]
 * 
 * Revision 1.1.3.4  1992/12/29  16:38:28  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:28:05  zeliff]
 * 
 * Revision 1.1.3.3  1992/10/07  20:38:05  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:20:19  sekhar]
 * 
 * Revision 1.1.4.2  1992/10/06  16:03:31  burati
 * 	Second replication drop: bl5
 * 	Move master_key routines to rsdb_mkey.c
 * 
 * Revision 1.1.3.2  1992/09/29  20:13:55  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:39:51  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:44:55  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  
** Copyright (c) Hewlett-Packard Company 1991, 1992
** Unpublished work. All Rights Reserved.
*/
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
/*
 *      Authentication key database functions.
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)rsdb_auth.c	12 - 12/05/91";
#endif

#include <sys/types.h>
#include <sys/file.h>
#include <un_time.h>
#include <un_io.h>
#include <Pointer.h>
#include <bstring.h>

#include <dce/dce.h>
#include <rs_base.h>
#include <rs_util.h>

#include <rsdb_pvt.h>
#include <rsdb_auth.h>
#include <rsdb_policy.h>
#include <sec_svc.h>
#include <dce/passwd.h>
#include <dce/dce_cf.h>
#include <rgymacro.h>

/*
 *  r s d b _ a u t h _ s t o r e _ b y _ a u t h k e y _ k e y
 *
 *  Use input key version, key_type, and acct_id to construct
 *  a database key to store the input auth_item.
 */
PUBLIC void rsdb_auth_store_by_authkey_key
#ifndef __STDC__
    ( version, key_type, acct_id, auth_item, st )
    sec_passwd_version_t        version;        /* [in] */
    krb5_keytype                key_type;       /* [in] */
    rsdb_pvt_id_t               acct_id;        /* [in] */
    rsdb_authkey_item_t         *auth_item;     /* [in] */
    error_status_t              *st;            /* [out] */
#else
    (
    sec_passwd_version_t        version,
    krb5_keytype                key_type,
    rsdb_pvt_id_t               acct_id,
    rsdb_authkey_item_t         *auth_item,
    error_status_t              *st
    )
#endif
{
    rsdb_authkey_key_t      authkey_key;
    char                    *data;
    int                     data_len;
    int                     auth_item_size;

    CLEAR_STATUS(st);

    bzero((Pointer) &authkey_key, sizeof(authkey_key));

    authkey_key.record_type = AUTH_TYPE;
    authkey_key.version     = version;
    authkey_key.key_type    = key_type;
    authkey_key.acct_id     = acct_id;

    auth_item_size = AUTH_SIZE(auth_item);

    rsdb_replace(rsdb_acct_db, (Pointer)&authkey_key, sizeof(authkey_key),
        (Pointer) auth_item, auth_item_size, st);
}



/*
 *  r s d b _ a u t h _ g e t _ b y _ a u t h k e y _ k e y
 *
 *  Use input key version, key_type, and acct_id to construct
 *  a database retrieval key to lookup the associated auth_item.
 *  NOTE: if the input version is the current version, this routine
 *  will fail because the current version of a key is stored in
 *  the acct item and not stored under an authkey_key.
 */
PUBLIC boolean32 rsdb_auth_get_by_authkey_key
#ifndef __STDC__
    ( version, key_type, acct_id, auth_item, st )
    sec_passwd_version_t        version;        /* [in] */
    krb5_keytype                key_type;       /* [in] */
    rsdb_pvt_id_t               acct_id;        /* [in] */
    rsdb_authkey_item_t         *auth_item;     /* [out] */
    error_status_t              *st;            /* [out] */
#else
    (
    sec_passwd_version_t        version,
    krb5_keytype                key_type,
    rsdb_pvt_id_t               acct_id,
    rsdb_authkey_item_t         *auth_item,
    error_status_t              *st
    )
#endif
{
    rsdb_authkey_key_t      authkey_key;
    char                    *data;
    int                     data_len;

    CLEAR_STATUS(st);
    if(key_type != RSDB_AUTH_DEFAULT_KEY_TYPE &&
       key_type != KEYTYPE_ENCRYPT_RSA_PRIV) {
        SET_STATUS(st, sec_rgy_key_bad_type);
        return false;
    }

    bzero((Pointer) &authkey_key, sizeof(authkey_key));

    authkey_key.record_type = AUTH_TYPE;
    authkey_key.version     = version;
    authkey_key.key_type    = key_type;
    authkey_key.acct_id     = acct_id;

    rsdb_fetch(rsdb_acct_db, (Pointer)&authkey_key, sizeof(authkey_key),
               &data, &data_len, st);
    if (BAD_STATUS(st)) {
        SET_STATUS(st, sec_rgy_key_bad_version);
        return false;
    }
    if (data_len <= sizeof(rsdb_authkey_item_t)) {
        bcopy(data, (Pointer)auth_item, data_len);
    } else {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_rsdb,
                       svc_c_sev_fatal | svc_c_action_abort,
                       sec_buf_too_small);
    }
    return true;
}

/*
 *  r s d b _ a u t h _ p u r g e _ b y _ a u t h k e y _ k e y
 *
 *  Use input key version, key_type, and acct_id to construct
 *  a database retrieval key to delete the associated auth_item.
 *  NOTE: if the input version is the current version, this routine
 *  will fail because the current version of a key is stored in
 *  the acct item and not stored under an authkey_key.
 */
PUBLIC void rsdb_auth_purge_by_authkey_key
#ifndef __STDC__
    ( version, key_type, acct_id, st )
    sec_passwd_version_t        version;        /* [in] */
    krb5_keytype                key_type;       /* [in] */
    rsdb_pvt_id_t               acct_id;        /* [in] */
    error_status_t              *st;            /* [out] */
#else
    (
    sec_passwd_version_t        version,
    krb5_keytype                key_type,
    rsdb_pvt_id_t               acct_id,
    error_status_t              *st
    )
#endif
{
    rsdb_authkey_key_t      authkey_key;
    char                    *data;
    int                     data_len;

    CLEAR_STATUS(st);
    if((key_type != RSDB_AUTH_DEFAULT_KEY_TYPE)
    && (key_type != KEYTYPE_ENCRYPT_RSA_PRIV)) {
        SET_STATUS(st, sec_rgy_key_bad_type);
        return;
    }

    bzero((Pointer) &authkey_key, sizeof(authkey_key));

    authkey_key.record_type = AUTH_TYPE;
    authkey_key.version     = version;
    authkey_key.key_type    = key_type;
    authkey_key.acct_id     = acct_id;

    rsdb_delete(rsdb_acct_db, (Pointer)&authkey_key, sizeof(authkey_key), st);
}

/*
 *  r s d b _ a u t h _ p u r g e _ a c c t _ k e y s
 *
 *  Use input acct->next_key_version to trace all authkeys
 *  and delete them.
 *
 *  XXX-this does not handle purging keys that are not
 *  of the default type.
 */
PUBLIC void rsdb_auth_purge_acct_keys
#ifndef __STDC__
    ( acct, st )
    rsdb_acct_item_t            *acct;          /* [in] */
    error_status_t              *st;            /* [out] */
#else
    (
    rsdb_acct_item_t            *acct,
    error_status_t              *st
    )
#endif
{
    krb5_kvno               next_version;
    rsdb_authkey_item_t     auth_item;

    CLEAR_STATUS(st);
    if(!MULT_KEY_VERS_OK(acct)) {
        /* no multiple versions allowed, so nothing to purge */
        return;
    }

    next_version  = acct->next_key_version;
    while(next_version != sec_passwd_c_version_none && STATUS_OK(st)) {
        rsdb_auth_get_by_authkey_key(next_version, RSDB_AUTH_DEFAULT_KEY_TYPE,
                                     acct->threads.id, &auth_item, st);
        if(BAD_STATUS(st))
            return;
        rsdb_auth_purge_by_authkey_key(next_version, RSDB_AUTH_DEFAULT_KEY_TYPE,
                                       acct->threads.id, st);
        next_version = auth_item.next_version;
    }
}


/*
 *  r s d b _ a u t h _ p u r g e _ e x p i r e d _ k e y s
 *
 *  Use input acct->next_key_version to trace all authkeys
 *  and delete those that have expired.
 *  Caller should call store_acct_by_id after this operation
 *  if acct information, acct->next_key_version, has changed.
 *
 *  This operation should only be called during an account replace
 *  because the master's current time at the point of garbage
 *  collection must be propagated to slaves in order to maintain
 *  key version consistency among all replicas.
 *
 *  XXX-This does not yet support purging expired keys that
 *  are not of the default keytype.
 */
PUBLIC void rsdb_auth_purge_expired_keys
#ifndef __STDC__
    ( acct, time_now, st )
    rsdb_acct_item_t            *acct;          /* [in] */
    sec_timeval_sec_t           *time_now;      /* [in] - the current time */
    error_status_t              *st;            /* [out] */
#else
    (
    rsdb_acct_item_t            *acct,
    sec_timeval_sec_t           *time_now,
    error_status_t              *st
    )
#endif
{
    krb5_kvno               this_version;
    krb5_kvno               last_version;
    rsdb_authkey_item_t     this_item;
    rsdb_authkey_item_t     last_item;

    CLEAR_STATUS(st);
    if(!MULT_KEY_VERS_OK(acct)) {
        /* no multiple versions allowed, so nothing to purge */
        return;
    }
    last_version = sec_passwd_c_version_none;

    this_version  = acct->next_key_version;
    while(this_version != sec_passwd_c_version_none) {
        rsdb_auth_get_by_authkey_key(this_version, RSDB_AUTH_DEFAULT_KEY_TYPE,
                                     acct->threads.id, &this_item, st);
        if(BAD_STATUS(st))
            return;
        if(AUTH_KEY_EXPIRED(&this_item, *time_now)) {
            rsdb_auth_purge_by_authkey_key(this_version, 
                                           RSDB_AUTH_DEFAULT_KEY_TYPE,
                                           acct->threads.id, st);
            if(BAD_STATUS(st))
                return;
            this_version = this_item.next_version;
        }
        /* If this_item had expired, was purged and if there were no next_version,
         * save "next_version = none" in the last unexpired auth_item (last_item).
         * OR, if this_item had not expired, save its version in the next_version
         * field of the last_item.  this_item becomes new last_item.
         */
        if( (AUTH_KEY_EXPIRED(&this_item, *time_now) &&
             this_version == sec_passwd_c_version_none) ||
            !AUTH_KEY_EXPIRED(&this_item, *time_now) ) {
            if(last_version == sec_passwd_c_version_none) {
                if( acct->next_key_version != this_version ) {
                    acct->next_key_version = this_version;
                    /* possibly store acct here */
                }
            } else {
                if(last_item.next_version != this_version) {
                    last_item.next_version = this_version;
                    rsdb_auth_store_by_authkey_key(last_version,
                        RSDB_AUTH_DEFAULT_KEY_TYPE,
                        acct->threads.id, &last_item, st);
                    if(BAD_STATUS(st))
                        return;
                }
            }
            if( !AUTH_KEY_EXPIRED(&this_item, *time_now) ) {
                last_version = this_version;
                bcopy((char *)&this_item, (char *)&last_item, sizeof(this_item));
                this_version = this_item.next_version;
            }
        }
    }
}



/*
 *  r s d b _ a u t h _ s e t _ e x p i r e _ t i m e
 *
 *  Use input acct_name to retrieve the effective auth_policy.
 *  Calculate expire_time by adding the max_renewable_lifetime
 *  to the current time.  Old versions of keys should be kept
 *  until any tickets that refer to them have expired.
 */
PUBLIC void rsdb_auth_set_expire_time
#ifndef __STDC__
    ( acct_name, time_now, expire_time, st )
    sec_rgy_login_name_t        *acct_name;     /* [in] */
    sec_timeval_sec_t           *time_now;      /* [in] */
    sec_timeval_sec_t           *expire_time;   /* [out] */
    error_status_t              *st;            /* [out] */
#else
    (
    sec_rgy_login_name_t        *acct_name,
    sec_timeval_sec_t           *time_now,
    sec_timeval_sec_t           *expire_time,
    error_status_t              *st
    )
#endif
{
    sec_rgy_plcy_auth_t         auth_policy;

    CLEAR_STATUS(st);
    rsdb_auth_policy_get_effective(acct_name, &auth_policy, st);
    if(BAD_STATUS(st))
        return;

    *expire_time = *time_now + auth_policy.max_renewable_lifetime;
}


/*
 *  r s d b _ a u t h _ s e t _ v e r s i o n _ n u m b e r
 *
 *  Use input passw_rec and acct_item to set the next
 *  version number.  If passwd->version_number is set,
 *  verify that it is a legal value; otherwise,
 *  increment the current version number.
 *  For accounts with multiple versions allowed, make
 *  sure the selected version number is not used
 *  for an unexpired key.
 *  Return sec_rgy_key_bad_version if a bad value is requested.
 */
PUBLIC void rsdb_auth_set_version_number
#ifndef __STDC__
    ( passwd, acct, time_now, new_version, st )
    sec_passwd_rec_t            *passwd;        /* [in] */
    rsdb_acct_item_t            *acct;          /* [in] */
    sec_timeval_sec_t           *time_now;      /* [in] */
    sec_passwd_version_t        *new_version;   /* [out] */
    error_status_t              *st;            /* [out] */
#else
    (
    sec_passwd_rec_t            *passwd,
    rsdb_acct_item_t            *acct,
    sec_timeval_sec_t           *time_now,
    sec_passwd_version_t        *new_version,
    error_status_t              *st
    )
#endif
{
    error_status_t              tmp_status;

    CLEAR_STATUS(st);

    if ((PASSWD_TYPE(passwd) == sec_passwd_genprivkey) ||
        (PASSWD_TYPE(passwd) == sec_passwd_pubkey)) {

        if (passwd->version_number == sec_passwd_c_version_none) {
            if (PK_PRIVKEY_VER_GET(acct) >= rsdb_auth_key_max_version_number)
                *new_version=rsdb_auth_key_min_version_number;
            else
                *new_version = (PK_PRIVKEY_VER_GET(acct)) + 1;
        }
        else if ((passwd->version_number < rsdb_auth_key_min_version_number) ||
                 (passwd->version_number > rsdb_auth_key_max_version_number) ) {
            SET_STATUS(st, sec_rgy_key_bad_version);
        }
        else
            *new_version = passwd->version_number;

        return;
    }

    /* If version_number specified, verify that it's not
     * the current version and that it doesn't already exist
     */
    if(passwd->version_number != sec_passwd_c_version_none) {
        if( (passwd->version_number == acct->cur_key_version)  ||
            (MULT_KEY_VERS_OK(acct) &&
             !rsdb_auth_version_ok(passwd->version_number,
                                   RSDB_AUTH_DEFAULT_KEY_TYPE,
                                   acct->threads.id, 
                                   time_now, &tmp_status))      ||
            (passwd->version_number < rsdb_auth_key_min_version_number ||
             passwd->version_number > rsdb_auth_key_max_version_number) ) {
            SET_STATUS(st, sec_rgy_key_bad_version);
            return;
        }
        *new_version = passwd->version_number;
    } else {
        /* version_number not specified, so increment current version */
        if(acct->cur_key_version == rsdb_auth_key_max_version_number) {
            *new_version = 1;
        } else {
            *new_version = acct->cur_key_version + 1;
        }
        if(MULT_KEY_VERS_OK(acct)) {
            while(!rsdb_auth_version_ok(*new_version, 
                                        RSDB_AUTH_DEFAULT_KEY_TYPE,
                                        acct->threads.id, time_now,
                                        &tmp_status)) {
                if(*new_version == rsdb_auth_key_max_version_number)
                    *new_version = 1;
                else
                    (*new_version)++;
            }
        }
    }
}



/*
 *  r s d b _ a u t h _ v e r s i o n _ o k
 *
 *  Return true if the input version number is not being used
 *  for any key of the input key_type for the input acct_id.
 *  If a key exists for the given version, but it has expired,
 *  delete the expired key and return true.
 *  NOTE: this routine does not compare the input version with
 *  the current version number for the default keytype.
 *  The caller should determine that the input version is
 *  not the current version *before* calling this routine.
 *
 *  XXX-This function does not check for any expired authkeys
 *  where the keytype is not the default.  It does check for
 *  version == 1 in this case.
 */
PUBLIC boolean32 rsdb_auth_version_ok
#ifndef __STDC__
    ( version, key_type, acct_id, time_now, st )
    sec_passwd_version_t        version;
    krb5_keytype                key_type;
    rsdb_pvt_id_t               acct_id;
    sec_timeval_sec_t           *time_now;
    error_status_t              *st;
#else
    (
    sec_passwd_version_t        version,
    krb5_keytype                key_type,
    rsdb_pvt_id_t               acct_id,
    sec_timeval_sec_t           *time_now,
    error_status_t              *st
    )
#endif
{
    rsdb_authkey_item_t     auth_item;
    error_status_t          tmp_status;

    CLEAR_STATUS(st);
    if(version == sec_passwd_c_version_none) {
        SET_STATUS(st, sec_rgy_key_bad_version);
        return false;
    }
    if (key_type == KEYTYPE_ENCRYPT_RSA_PRIV && version != 1) {
        SET_STATUS(st, sec_rgy_key_bad_version);
        return false;
    }

    if(!rsdb_auth_get_by_authkey_key(version, key_type,
                                    acct_id, &auth_item, &tmp_status)) {
        return true;
    }
    /* auth_item exists for given key version.
     * If the caller provided the current time,
     * check to see if it has expired.
     */
    if (time_now != NULL) {
        if( auth_item.key_expire_time <= *time_now ) {
            /* delete expired key */
            rsdb_auth_purge_by_authkey_key(version, key_type, 
                                           acct_id, &tmp_status);
            return true;
        }
    }
    return false;
}



/*
 *  r s d b _ a u t h _ m u l t _ k e y _ v e r s _ a l l o w e d
 *
 *  Use the input principal name to determine whether multiple
 *  key versions are allowed for this account.
 *  Currently, multiple key versions are allowed for:
 *      dce/rgy, dce/ptgt, the entire krbtgt principal tree
 *
 */
PUBLIC boolean32 rsdb_auth_mult_key_vers_allowed
#ifndef __STDC__
    ( pname )
    sec_rgy_name_t        pname;
#else
    (
    sec_rgy_name_t        pname
    )
#endif
{
    if( (ustrcmp(SEC_PRIV_SERVER_NAME, pname) == 0) ||
        (ustrcmp(SEC_RGY_SERVER_NAME, pname) == 0)  ||
        (ustrncmp(KRB_DIR_ROOT, pname, KRB_DIR_ROOT_LEN) == 0) )
        return true;
    else
        return false;
}

/*
 *  r s d b _ a u t h _ p r i v k e y _ a l l o w e d
 *
 *  Use the input principal name to determine whether private
 *  key authkeys are allowed for this account.
 *  Currently, private key authkeys are allowed for:
 *      the local krbtgt principal
 *
 */
PUBLIC boolean32 rsdb_auth_privkey_allowed
#ifndef __STDC__
    ( pname )
    sec_rgy_name_t        pname;
#else
    (
    sec_rgy_name_t        pname
    )
#endif
{
    if( (ustrncmp(KRB_DIR_ROOT, pname, KRB_DIR_ROOT_LEN) == 0) )
    {
        char                  *cell_name;
        error_status_t        st;

        dce_cf_get_cell_name(&cell_name, &st);
        if (BAD_STATUS(&st))
            return false;

        if (ustrcmp(cell_name, pname+KRB_DIR_ROOT_LEN) == 0)
            return true;
    }
    else
        return false;
}

/*
 *  r s d b _ a u t h _ i n i t _ p r i v k e y _ a l l o w e d
 *
 *  Set the rsdb_acct_item_privkey_ok flag on accounts where allowed.
 *  Currently, private key authkeys are allowed for:
 *      the local krbtgt principal
 *
 */
PUBLIC rsdb_auth_init_privkey_allowed
#ifndef __STDC__
    ( st_p )
    error_status_t     *st_p;
#else
    (
    error_status_t     *st_p
    )
#endif
{
    sec_rgy_login_name_t    login_name;
    sec_rgy_cursor_t        cursor;
    sec_rgy_acct_key_t      name_parts;
    rsdb_named_item_t       person;
    rsdb_named_item_t       group;
    rsdb_named_item_t       org;
    rsdb_acct_item_t        account;
    char	            *cell_name;

    dce_cf_get_cell_name(&cell_name, st_p);
    if (GOOD_STATUS(st_p)) {

        if ((KRB_DIR_ROOT_LEN + strlen(cell_name) - GLOBAL_DIR_ROOT_LEN) > sizeof(login_name.pname))
            *st_p=sec_res_princ_cvt_err;
        else {
            u_strcpy(login_name.gname, "none");
            u_strcpy(login_name.oname, "none");
            u_strncpy(login_name.pname, KRB_DIR_ROOT, KRB_DIR_ROOT_LEN);
            u_strcat(login_name.pname, cell_name + GLOBAL_DIR_ROOT_LEN);
            cursor.valid = false;
            rsdb_acct_lookup_acct_id(&login_name, &cursor, &name_parts,
                                     &person, &group, &org, &account, st_p);
            if(GOOD_STATUS(st_p))
                account.flags |= rsdb_acct_item_privkey_ok;
                store_acct_by_id(account.threads.id, &account);
        }
    }
    if (cell_name)
        free(cell_name);
}
