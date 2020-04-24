/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsdb_acct.h,v $
 * Revision 1.1.78.2  1996/10/03  14:50:59  arvind
 * 	CHFts19896: public key version support
 * 	[1996/09/13  23:42 UTC  aha  /main/DCE_1.2.2/aha_pk9_3/1]
 *
 * 	Define rsdb_acct_get_curr_key_version
 * 	[1996/09/06  18:19 UTC  aha  /main/DCE_1.2.2/aha_pk9_2/1]
 *
 * 	Merge Registry support for KDC private key storage
 * 	[1996/06/07  19:06 UTC  aha  /main/aha_pk6/1]
 *
 * 	Changes for Public Key Login; support Registry storage of KDC private key.
 * 	[1996/01/03  19:11 UTC  psn  /main/DCE_1.2/1]
 *
 * Revision 1.1.78.1  1996/08/09  12:04:59  arvind
 * 	Merge Registry support for KDC private key storage
 * 	[1996/06/07  19:06 UTC  aha  /main/aha_pk6/1]
 * 
 * 	Changes for Public Key Login; support Registry storage of KDC private key.
 * 	[1996/01/03  19:11 UTC  psn  /main/DCE_1.2/1]
 * 
 * 	[1994/12/15  03:25 UTC  rps  /main/HPDCE02/1]
 * 	[1996/01/03  19:11 UTC  psn  /main/DCE_1.2/1]
 * 
 * 	[OT12669] cell renaming
 * 	[1994/12/14  23:23 UTC  rps  /main/rps_mothra/1]
 * 
 * Revision 1.1.76.3  1996/02/18  22:59:54  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:18:23  marty]
 * 
 * Revision 1.1.76.2  1995/12/08  17:27:53  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1994/12/15  03:25 UTC  rps
 * 
 * 	HP revision /main/rps_mothra/1  1994/12/14  23:23 UTC  rps
 * 	[OT12669] cell renaming
 * 	[1995/12/08  16:53:40  root]
 * 
 * Revision 1.1.70.1  1994/07/15  14:59:44  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:17:01  mdf]
 * 
 * Revision 1.1.5.5  1993/03/19  22:19:54  emartin
 * 	CR 7472  rsdb_acct_replace - takes gen_unix_passwd arg
 * 	[1993/03/19  18:52:44  emartin]
 * 
 * Revision 1.1.5.4  1993/02/19  16:19:06  ahop
 * 	7191: add rsdb_acct_gen_default_pepper
 * 	[1993/02/19  03:51:23  ahop]
 * 
 * Revision 1.1.5.3  1992/12/29  13:25:50  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:46:08  zeliff]
 * 
 * Revision 1.1.5.2  1992/10/07  20:28:16  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:12:52  sekhar]
 * 
 * Revision 1.1.3.2  1992/10/06  15:45:34  burati
 * 	 Second replication code drop: bl5
 * 
 * Revision 1.1.2.2  1992/07/13  20:00:47  ahop
 * 	 3903:   add rsdb_acct_store_default_pepper
 * 	[1992/06/30  22:28:18  ahop]
 * 
 * Revision 1.1  1992/01/19  14:42:52  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1991, 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 *      Registry Server - database interlude - Account management
 */

#ifndef rsdb_acct__included
#define rsdb_acct__included

#include <dce/nbase.h>
#include <dce/rgynbase.h>
#include <dce/rsbase.h>

#include <rsdb_pvt.h>
#include <krb5/krb5.h>

void rsdb_acct_gen_default_pepper (
#ifdef __STDC__
    sec_rgy_name_t              pname,
    krb5_data                   *pepper,
    error_status_t              *status
#endif
);

void rsdb_acct_create (
#ifdef __STDC__
    void
#endif
);

void rsdb_acct_init (
#ifdef __STDC__
    void
#endif
);

boolean32 rsdb_acct_get_auth_key_by_name (
#ifdef __STDC__
    sec_rgy_name_t      svr_pname,
    char		*realm,
    unsigned32          key_type,
    unsigned32          key_version,
    krb5_keyblock       **auth_key,
    boolean32           *default_pepper,
    krb5_data           *pepper,
    krb5_kvno           *mkey_version,
    error_status_t      *st
#endif
);

void rsdb_acct_store_default_pepper (
#ifdef __STDC__
    sec_rgy_name_t      pname,              /* [in] */
    error_status_t      *st                 /* [out] */
#endif
);

boolean32 rsdb_get_acct_by_id (
#ifdef __STDC__
    rsdb_pvt_id_t      id,
    rsdb_acct_item_t   *acct
#endif
);

void rsdb_purge_account (
#ifdef __STDC__
    boolean32          update_person,
    rsdb_named_item_t  *person,
    rsdb_acct_item_t   *account
#endif
);

void rsdb_acct_add (
#ifdef __STDC__
    sec_rgy_login_name_t   *login_name,             /* [in] */
    sec_rgy_acct_key_t     key_parts,               /* [in] */
    sec_rgy_acct_user_t    *(user_part),            /* [in] */
    sec_rgy_acct_admin_t   *(admin_part),           /* [in] */
    sec_passwd_rec_t       *passwd,                 /* [in] */
    boolean32              gen_unix_passwd,         /* [in] */
    sec_passwd_version_t   *passwd_version_number,  /* [out] */
    error_status_t         *(status)
#endif
);

void rsdb_acct_get_cur_key_version (
#ifdef __STDC__
    sec_rgy_login_name_t   *login_namep,
    sec_passwd_type_t      key_type,
    unsigned32             *key_version,
    error_status_t         *st_p
#endif
);

void rsdb_acct_add_key_version (
#ifdef __STDC__
    sec_rgy_login_name_t   *login_name,             /* [in] */
    sec_passwd_rec_t       *passwd,                 /* [in] */
    boolean32              current,                 /* [in] */
    sec_timeval_sec_t      *version_expiration,     /* [in] */
    error_status_t         *(status)
#endif
);

void rsdb_acct_delete (
#ifdef __STDC__
    sec_rgy_login_name_t   *login_name,
    error_status_t         *(status)
#endif
);

void rsdb_acct_rename (
#ifdef __STDC__
    sec_rgy_login_name_t  *old_login_name,
    sec_rgy_login_name_t  *new_login_name,
    sec_rgy_acct_key_t    *new_key_parts,
    error_status_t        *(status)
#endif
);

void rsdb_acct_lookup (
#ifdef __STDC__
    sec_rgy_login_name_t   *login_name,
    sec_rgy_cursor_t       *(cursor),
    sec_rgy_sid_t          *(sid),
    sec_rgy_unix_sid_t     *(unix_sid),
    sec_rgy_acct_key_t     *(key_parts),
    sec_rgy_acct_user_t    *(user_part),
    sec_rgy_acct_admin_t   *(admin_part),
    error_status_t         *(status)
#endif
);    

void rsdb_acct_key_get_next (
#ifdef __STDC__
    sec_rgy_login_name_t   *name,             /* [in] */
    sec_rgy_cursor_t       *cursor,           /* [in, out] */
    sec_passwd_rec_t       *key,              /* [out] */
    boolean32              *current,          /* [out] */
    sec_timeval_sec_t      *garbage_collect,  /* [out] */    
    error_status_t         *status            /* [out] */
#endif
);

void rsdb_acct_authkey_get_next (
#ifdef __STDC__
    sec_rgy_login_name_t   *name,             /* [in] */
    krb5_keytype           keytype,           /* [in] */
    sec_rgy_cursor_t       *cursor,           /* [in, out] */
    sec_passwd_rec_t       *key,              /* [out] */
    boolean32              *current,          /* [out] */
    sec_timeval_sec_t      *garbage_collect,  /* [out] */    
    error_status_t         *status            /* [out] */
#endif
);

boolean32 rsdb_acct_lookup_acct_id (
#ifdef __STDC__
    sec_rgy_login_name_t  *login_name,     /* [in, out] */
    sec_rgy_cursor_t      *(cursor),      /* [in, out] */
    sec_rgy_acct_key_t    *name_parts,
    rsdb_named_item_t     *person,
    rsdb_named_item_t     *group,
    rsdb_named_item_t     *org,
    rsdb_acct_item_t      *account,
    error_status_t        *(status)
#endif
);

void rsdb_acct_gen_unix_info (
#ifdef __STDC__
    char               *buf,
    long               *buf_len,
    rsdb_acct_item_t   *account,
    sec_rgy_acct_key_t name_parts,
    rsdb_named_item_t  *person,
    rsdb_named_item_t  *group,
    rsdb_named_item_t  *org,
    long               *unix_id
#endif
);

void rsdb_acct_update_unix_info (
#ifdef __STDC__
    rsdb_acct_item_t   *account,
    sec_rgy_acct_key_t name_parts,
    rsdb_named_item_t  *person,
    rsdb_named_item_t  *group,
    rsdb_named_item_t  *org
#endif
);

void rsdb_acct_gen_pwent (
#ifdef __STDC__
    sec_rgy_login_name_t   login_name,     /* [in] */
    long                   login_name_len, /* [in] */
    sec_rgy_cursor_t       *(cursor),      /* [in, out] */
    long                   *datalen,       /* [in, out] */
    char                   *data,          /* [out] */
    long                   *uid,
    long                   *actid,
    error_status_t         *(status)
#endif
);

void rsdb_acct_get_unix_id (
#ifdef __STDC__
    rsdb_pvt_id_t      *actid,
    long               *unix_id
#endif
);

void rsdb_acct_login_lookup (
#ifdef __STDC__
    sec_rgy_login_name_t   *login_name,    /* [in, out] */
    sec_rgy_cursor_t       *(cursor),      /* [in, out] */
    sec_rgy_sid_t          *(sid),         /* [out] */
    sec_rgy_unix_sid_t     *(unix_sid),    /* [out] */
    sec_rgy_acct_key_t     *(key_parts),   /* [out] */
    sec_rgy_acct_user_t    *(user_part),   /* [out] */
    sec_rgy_acct_admin_t   *(admin_part),  /* [out] */
    error_status_t         *(status)
#endif
);

void rsdb_acct_get_projlist (
#ifdef __STDC__
    sec_rgy_login_name_t  *name,              /* [in] */
    sec_rgy_cursor_t      *(projlist_cursor), /* [in, out] */
    long                  max_num,            /* [in] */
    long                  *(num_returned),    /* [out] */
    uuid_t                *projlist,          /* [out] */
    long                  *unix_projlist,     /* [out] */
    long                  *(num_projects),    /* [out] */
    error_status_t        *(status)           /* [out] */
#endif
);


void rsdb_acct_replace (
#ifdef __STDC__
    sec_rgy_login_name_t   *login_name,
    sec_rgy_acct_key_t     key_parts,
    rs_acct_parts_t        modify_parts,
    sec_rgy_acct_user_t    *(user_part),
    sec_rgy_acct_admin_t   *(admin_part),
    sec_passwd_rec_t       *passwd,        /* [in] */
    boolean32              gen_unix_passwd,/* [in] */
    sec_timeval_sec_t      *time_now,      /* [in] */
    sec_passwd_version_t   *passwd_version_number,  /* [out] */
    error_status_t         *(status)
#endif
);

void rsdb_acct_auth_policy_get_info (
#ifdef __STDC__
    sec_rgy_login_name_t  *account,        /* [in] */
    sec_rgy_plcy_auth_t   *(auth_policy),  /* [out] */
    error_status_t        *(status)
#endif
);


void rsdb_acct_auth_policy_set_info (
#ifdef __STDC__
    sec_rgy_login_name_t  *account,        /* [in] */
    sec_rgy_plcy_auth_t   *(auth_policy),  /* [in] */
    error_status_t        *(status)
#endif
);

#endif



