/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsdb_auth.h,v $
 * Revision 1.1.7.1  1996/08/09  12:05:11  arvind
 * 	Merge Registry support for KDC private key storage
 * 	[1996/07/13  20:48 UTC  aha  /main/aha_pk6/1]
 *
 * 	Merge-out and bugfixes and Registry work
 * 	[1996/02/18  23:00:04  marty  1.1.5.2]
 *
 * Revision 1.1.5.2  1996/02/18  23:00:04  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:18:33  marty]
 * 
 * Revision 1.1.5.1  1995/12/08  17:28:01  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:53:44  root]
 * 
 * Revision 1.1.3.3  1992/12/29  13:25:56  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:46:16  zeliff]
 * 
 * Revision 1.1.3.2  1992/10/07  20:28:26  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:12:56  sekhar]
 * 
 * Revision 1.1.1.2  1992/10/06  15:45:52  burati
 * 	 Second replication code drop: bl5
 * 	 Move master_key operations to rsdb_mkey.h
 * 
 * Revision 1.1  1992/01/19  14:43:31  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
 * Copyright (c) Hewlett-Packard Company 1991, 1992
 * Unpublished work. All Rights Reserved.
 */

#ifndef rsdb_auth__included
#define rsdb_auth__included


#include <rsdb_pvt.h>
#include <krb5/kdb.h>

void rsdb_auth_store_by_authkey_key (
#ifdef __STDC__
    sec_passwd_version_t        version,
    krb5_keytype                key_type,
    rsdb_pvt_id_t               acct_id,
    rsdb_authkey_item_t         *auth_item,
    error_status_t              *st
#endif
);

boolean32 rsdb_auth_get_by_authkey_key (
#ifdef __STDC__
    sec_passwd_version_t        version,
    krb5_keytype                key_type,
    rsdb_pvt_id_t               acct_id,
    rsdb_authkey_item_t         *auth_item,
    error_status_t              *st
#endif
);

void rsdb_auth_purge_by_authkey_key (
#ifdef __STDC__
    sec_passwd_version_t        version,
    krb5_keytype                key_type,
    rsdb_pvt_id_t               acct_id,
    error_status_t              *st
#endif
);

void rsdb_auth_purge_acct_keys (
#ifdef __STDC__
    rsdb_acct_item_t            *acct,
    error_status_t              *st
#endif
);

void rsdb_auth_purge_expired_keys (
#ifdef __STDC__
    rsdb_acct_item_t            *acct,
    sec_timeval_sec_t           *time_now,
    error_status_t              *st
#endif
);

void rsdb_auth_set_expire_time (
#ifdef __STDC__
    sec_rgy_login_name_t        *acct_name,
    sec_timeval_sec_t           *time_now,
    sec_timeval_sec_t           *expire_time,
    error_status_t              *st
#endif
);

void rsdb_auth_set_version_number (
#ifdef __STDC__
    sec_passwd_rec_t            *passwd,
    rsdb_acct_item_t            *acct,
    sec_timeval_sec_t           *time_now,
    sec_passwd_version_t        *new_version,
    error_status_t              *st
#endif
);

boolean32 rsdb_auth_version_ok (
#ifdef __STDC__
    sec_passwd_version_t        version,
    krb5_keytype                key_type,
    rsdb_pvt_id_t               acct_id,
    sec_timeval_sec_t           *time_now,
    error_status_t              *st
#endif
);

boolean32 rsdb_auth_mult_key_vers_allowed (
#ifdef __STDC__
    sec_rgy_name_t        pname
#endif
);

/*
 *  r s d b _ a u t h _ p r i v k e y _ a l l o w e d
 *
 *  Use the input principal name to determine whether private
 *  key authkeys are allowed for this account.
 *  Currently, private key authkeys are allowed for:
 *      the local krbtgt principal
 *
 */
boolean32 rsdb_auth_privkey_allowed (
#ifdef __STDC__
    sec_rgy_name_t        pname
#endif
);


#endif

