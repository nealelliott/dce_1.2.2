/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_prop_acct.c,v $
 * Revision 1.1.8.2  1996/02/18  00:18:52  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:07:28  marty]
 *
 * Revision 1.1.8.1  1995/12/08  17:53:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:21:03  root]
 * 
 * Revision 1.1.6.1  1994/07/15  15:01:49  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:20:26  mdf]
 * 
 * Revision 1.1.3.6  1993/03/19  22:20:58  emartin
 * 	CR 7472  mostly revert to rev 1.1.5.2
 * 	get_key calls rs_pwd_get_key_by_prnc
 * 	[1993/03/19  18:57:48  emartin]
 * 
 * 	CR 7472
 * 	get_key return encrypt_key
 * 	rs_prop_acct_add/replace - use slightly different primitive
 * 	 to decrypt the unix passwd.
 * 	[1993/03/17  21:15:31  emartin]
 * 
 * 	CR 7472 rs_prop_acct_replace - deal w. unix_passwd
 * 	 rs_prop_acct_add - log unix_passwd
 * 	[1993/03/17  18:40:04  emartin]
 * 
 * Revision 1.1.3.5  1992/12/29  16:36:17  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:25:12  zeliff]
 * 
 * Revision 1.1.3.4  1992/12/18  22:12:19  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:50:15  sekhar]
 * 
 * Revision 1.1.4.2  1992/12/11  21:44:09  emartin
 * 	bl6c: add chk_update calls
 * 
 * Revision 1.1.3.3  1992/11/04  19:37:33  ahop
 * 	Third replication drop: bl6a
 * 	[1992/11/03  18:39:46  ahop]
 * 
 * Revision 1.1.3.2  1992/10/07  20:35:16  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:18:07  sekhar]
 * 
 * Revision 1.1.1.2  1992/10/06  17:22:10  burati
 * 	Second replication drop: bl5
 * 
 * $EndLog$
 */


/*
 * Copyright (c) Hewlett-Packard Company 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 *      Registry Propagation server manager routines - Accounts
 */
    
#include <dce/uuid.h>
#include <rs_server.h>
#include <rs_pvt.h>
#include <rs_log_acct.h>
#include <rs_prop_acct.h>
#include <rs_pwd.h>
#include <rs_ch_lock.h>
#include <rs_master.h>
#include <rs_rep_auth.h>
#include <rs_rep_mgmt.h>
#include <rsdb_util.h>
#include <rsdb_acct.h>
#include <crypt.h>
      
/* get_key
 *
 * unpickle and decrypt the wire rep of a key using either the
 * initialization session key, or the update originator's key,
 * as appropriate
 */
PRIVATE  void get_key
#ifndef __STDC__
    (wire_rep, originator, passwd, status ) 
    rs_acct_key_transmit_t  *wire_rep;    /* [in] */
    sec_rgy_foreign_id_t    *originator;  /* [in] */
    sec_passwd_rec_t        *passwd;      /* [out] */
    error_status_t          *status;      /* [out] */
#else
  (
    rs_acct_key_transmit_t  *wire_rep,    /* [in] */
    sec_rgy_foreign_id_t    *originator,  /* [in] */
    sec_passwd_rec_t        *passwd,      /* [out] */
    error_status_t          *status
  )
#endif
{   
    static boolean32        nil_id_inited = false;
    static uuid_t           nil_id;
    sec_passwd_rec_t        *encrypt_key;

    if (!nil_id_inited) {
        uuid_create_nil(&nil_id, status); 
        if (BAD_STATUS(status)) {   /* not bloody likely */
            return;
        }
        nil_id_inited = true;
    }

    if (originator == NULL 
        || uuid_equal(&originator->principal, &nil_id, status)) {
        /*
         * need the initialization encryption key to decrypt the 
         * account key.
         */
        rs_rep_auth_get_init_ekey(&encrypt_key, status);
        if (GOOD_STATUS(status)) {
            rs_pwd_get_key_by_session_key(wire_rep, encrypt_key, passwd, status);  
            encrypt_key = NULL;
        }
    } else {
        /* use the originator's key to decrypt the account key */
        rs_pwd_get_key_by_prnc(wire_rep, &originator->principal, 
            passwd, &encrypt_key, status);
        if (GOOD_STATUS(status)) {
            sec_crypt_free_key(&encrypt_key);
        }
    }
}


PRIVATE  void  get_unix_passwd
#ifndef __STDC__
    ( wire_rep, originator, unix_passwd_buf, status ) 
    rs_acct_key_transmit_t     *wire_rep;        /* [in] */
    sec_rgy_foreign_id_t       *originator;      /* [in] */
    sec_rgy_unix_passwd_buf_t  unix_passwd_buf;  /* [out] */
    error_status_t             *status;          /* [out] */
#else
  (
    rs_acct_key_transmit_t     *wire_rep,        /* [in] */
    sec_rgy_foreign_id_t       *originator,      /* [in] */
    sec_rgy_unix_passwd_buf_t  unix_passwd_buf,  /* [out] */
    error_status_t             *status
  )
#endif
{
    sec_passwd_rec_t  unix_passwd;

    CLEAR_STATUS(status);

    unix_passwd_buf[0] = '\0';

    if (wire_rep != NULL) {
        get_key(wire_rep, originator, &unix_passwd, status);

        if (GOOD_STATUS(status)) {
            rs_pwd_to_unix_pwd(&unix_passwd, unix_passwd_buf, status);
        }
    }
}
    
/*
 * rs_prop_acct_add 
 */
PUBLIC  void  rs_prop_acct_add
#ifndef __STDC__
    ( h, num_accts, accts, master_info, propq_only, status ) 
    handle_t                  h;              /* [in] */
    unsigned32                num_accts;      /* [in] */
    rs_prop_acct_add_data_t   accts[];        /* [in, size_is(num_accts)] */
    rs_replica_master_info_t  * master_info;  /* [in, ref] */
    boolean32                 propq_only;     /* [in] */
    error_status_t            * status;       /* [out] */
#else
  (
    handle_t                  h,              /* [in] */
    unsigned32                num_accts,      /* [in] */
    rs_prop_acct_add_data_t   accts[],        /* [in, size_is(num_accts)] */
    rs_replica_master_info_t  * master_info,  /* [in, ref] */
    boolean32                 propq_only,     /* [in] */
    error_status_t            * status
  )
#endif
{
    unsigned32                i; 
    error_status_t            xst;
    sec_passwd_rec_t          passwd; 
    sec_rgy_acct_key_t        key_parts = sec_rgy_acct_key_person;
    boolean32                 gen_unix_passwd = true;
    boolean32                 apply_update;

    CLEAR_STATUS(status);

    rs_rep_auth_check(h, status);
    if (BAD_STATUS(status)) {
        return;
    }
   
    if (! propq_only) {

        BEGIN_SLAVE_WRITE_EVENT(rs_c_event_slave_write, lock_db, master_info,
            status) { 

            rs_rep_mgmt_slave_chk_update(master_info, &apply_update, status);

            if (apply_update) {
                for (i = 0; i < num_accts && GOOD_STATUS(status); i++) {
                
                    get_key(accts[i].key, &accts[i].client, 
                            &passwd, status);
                
                    if (GOOD_STATUS(status) && 
                        accts[i].unix_passwd != NULL) {
                        /* 
                         * need to extract the unix 
                         * password provided by the master
                         */
                        get_unix_passwd(accts[i].unix_passwd, 
                            &accts[i].client,
                            accts[i].user_part.passwd, status);
                            gen_unix_passwd = false;
                    }

                    if (GOOD_STATUS(status)) {
                        rsdb_acct_add(&accts[i].login_name, key_parts, 
                                      &accts[i].user_part, 
                                      &accts[i].admin_part, 
                                      &passwd, gen_unix_passwd, 
                                      &passwd.version_number, status);
                    }
                    
                    if (GOOD_STATUS(status) &&
                        (rs_state.rep_state != rs_c_state_initializing)) {
                            rsdb_util_quota_decrement(
                                &accts[i].client.principal,
                                &accts[i].client.cell, &xst);
                            rs_log_acct_add(&accts[i].login_name, 
                                &accts[i].user_part, 
                                &accts[i].admin_part, accts[i].key,
                                &accts[i].client, accts[i].keytype, 
                                accts[i].unix_passwd, master_info); 
                    }
                }
            }
        } END_SLAVE_WRITE_EVENT;
    }
    else {
        /* just add updates to propq
         */
        BEGIN_RIW_EVENT(rs_c_event_propq_write, lock_db, status) { 

            rs_rep_mgmt_slave_chk_propq_upd(master_info, 
                                            &apply_update, status);
            if (apply_update) {
                if (GOOD_STATUS(status)) {
                    for (i = 0; i < num_accts; i++) {
                        rs_log_acct_add(&accts[i].login_name, 
                            &accts[i].user_part, &accts[i].admin_part, 
                            accts[i].key, &accts[i].client, 
                            accts[i].keytype, accts[i].unix_passwd,
                            master_info); 
                    }
                }
            }
        } END_RIW_EVENT;
    }
}

/*
 * rs_prop_acct_delete
 */
PUBLIC  void  rs_prop_acct_delete
#ifndef __STDC__
    ( h, login_name, master_info, propq_only, status ) 
    handle_t                  h;              /* [in] */
    sec_rgy_login_name_t      * login_name;   /* [in] */
    rs_replica_master_info_t  * master_info;  /* [in, ref] */
    boolean32                 propq_only;     /* [in] */
    error_status_t            * status;       /* [out] */
#else
  (
    handle_t                  h,              /* [in] */
    sec_rgy_login_name_t      * login_name,   /* [in] */
    rs_replica_master_info_t  * master_info,  /* [in, ref] */
    boolean32                 propq_only,     /* [in] */
    error_status_t            * status
  )
#endif
{
    boolean32                 apply_update;

    CLEAR_STATUS(status);

    rs_rep_auth_check(h, status);
    if (BAD_STATUS(status)) {
        return;
    }
   
    if (! propq_only) {
        BEGIN_SLAVE_WRITE_EVENT(rs_c_event_slave_write, lock_db, master_info,
            status) { 

            rs_rep_mgmt_slave_chk_update(master_info, 
                                         &apply_update, status);
            if (apply_update) {
                if (GOOD_STATUS(status)) {
                    rsdb_acct_delete(login_name, status);
                }
                if (GOOD_STATUS(status)) {
                    rs_log_acct_delete(login_name, master_info); 
                }
            }
        } END_SLAVE_WRITE_EVENT;
    } 
    else {
        /* just add update to propq
         */
        BEGIN_RIW_EVENT(rs_c_event_propq_write, lock_db, status) { 

            rs_rep_mgmt_slave_chk_propq_upd(master_info, 
                                            &apply_update, status);
            if (apply_update) {
                if (GOOD_STATUS(status)) {
                    rs_log_acct_delete(login_name, master_info); 
                }
            }
        } END_RIW_EVENT;
    }
}


/*
 * rs_prop_acct_rename
 */
PUBLIC  void  rs_prop_acct_rename
#ifndef __STDC__
    ( h, old_login_name, new_login_name, master_info, propq_only, status )
    handle_t                  h;                 /* [in] */
    sec_rgy_login_name_t      * old_login_name;  /* [in] */
    sec_rgy_login_name_t      * new_login_name;  /* [in] */
    rs_replica_master_info_t  * master_info;     /* [in, ref] */
    boolean32                 propq_only;        /* [in] */
    error_status_t            * status;          /* [out] */
#else
  (
    handle_t                  h,
    sec_rgy_login_name_t      * old_login_name,
    sec_rgy_login_name_t      * new_login_name,
    rs_replica_master_info_t  * master_info,   
    boolean32                 propq_only,      
    error_status_t            * status
  )
#endif
{
    sec_rgy_acct_key_t        key_parts = sec_rgy_acct_key_person;  
    boolean32                 apply_update;

    CLEAR_STATUS(status);
                                          
    rs_rep_auth_check(h, status);
    if (BAD_STATUS(status)) {
        return;
    }
   
    if (! propq_only) {
        BEGIN_SLAVE_WRITE_EVENT(rs_c_event_slave_write, lock_db, master_info,
            status) { 
            rs_rep_mgmt_slave_chk_update(master_info, 
                                         &apply_update, status);
            if (apply_update) {
                if (GOOD_STATUS(status)) {
                    rsdb_acct_rename(old_login_name, new_login_name, 
                                     &key_parts, status);
                }
                if (GOOD_STATUS(status)) {
                    rs_log_acct_rename(old_login_name, new_login_name, 
                                       master_info); 
                }
            }
        } END_SLAVE_WRITE_EVENT;
    } else {
        /* just add update to propq
         */
        BEGIN_RIW_EVENT(rs_c_event_propq_write, lock_db, status) { 
            rs_rep_mgmt_slave_chk_propq_upd(master_info, 
                                            &apply_update, status);
            if (apply_update) {
                if (GOOD_STATUS(status)) {
                    rs_log_acct_rename(old_login_name, 
                            new_login_name, master_info); 
                }
            }
        } END_RIW_EVENT;
    }
}


/*
 * rs_prop_acct_replace 
 */
PUBLIC  void  rs_prop_acct_replace
#ifndef __STDC__
    ( h, login_name, modify_parts, user_part, admin_part, key, client_ids, 
      new_keytype, unix_passwd, time_now, master_info, propq_only, status )
    handle_t                  h;              /* [in] */
    sec_rgy_login_name_t      * login_name;   /* [in] */
    rs_acct_parts_t           modify_parts;   /* [in] */
    sec_rgy_acct_user_t       * user_part;    /* [in] */
    sec_rgy_acct_admin_t      * admin_part;   /* [in] */
    rs_acct_key_transmit_t    * key;          /* [in, ptr] - may be NULL */
    sec_rgy_foreign_id_t      *client_ids;    /* [in, ref] */
    sec_passwd_type_t         new_keytype;    /* [in] */
    rs_acct_key_transmit_t    *unix_passwd;   /* [in]  may be NULL */
    sec_timeval_sec_t         * time_now;     /* [in, ref] */
    rs_replica_master_info_t  * master_info;  /* [in, ref] */
    boolean32                 propq_only;     /* [in] */
    error_status_t            * status;       /* [out] */
#else
  (
    handle_t                  h,            
    sec_rgy_login_name_t      * login_name, 
    rs_acct_parts_t           modify_parts, 
    sec_rgy_acct_user_t       * user_part,  
    sec_rgy_acct_admin_t      * admin_part, 
    rs_acct_key_transmit_t    * key,        
    sec_rgy_foreign_id_t      *client_ids,  
    sec_passwd_type_t         new_keytype,  
    rs_acct_key_transmit_t    *unix_passwd,
    sec_timeval_sec_t         * time_now,   
    rs_replica_master_info_t  * master_info,
    boolean32                 propq_only,   
    error_status_t            * status
  )
#endif
{
    sec_rgy_acct_key_t        key_parts = sec_rgy_acct_key_person;
    sec_passwd_version_t      passwd_version; 
    sec_passwd_rec_t          passwd; 
    boolean32                 gen_unix_passwd = true;
    boolean32                 apply_update;

    CLEAR_STATUS(status);

    rs_rep_auth_check(h, status);
    if (BAD_STATUS(status)) {
        return;
    }
   
    if (! propq_only) {
        BEGIN_SLAVE_WRITE_EVENT(rs_c_event_slave_write, lock_db, master_info,
            status) { 
            rs_rep_mgmt_slave_chk_update(master_info, 
                                         &apply_update, status);
            if (apply_update) {

                if (GOOD_STATUS(status) && 
                    (modify_parts & rs_acct_part_passwd)) {
                    get_key(key, client_ids, &passwd, status);

                    if (GOOD_STATUS(status) &&
                        (unix_passwd != NULL)) {
                        /* 
                         * need to extract the unix 
                         * password provided by the master
                         */
                        get_unix_passwd(unix_passwd, 
                            client_ids,
                            user_part->passwd, status);
                            gen_unix_passwd = false;
                    }
                }
                if (GOOD_STATUS(status)) {
                    rsdb_acct_replace(login_name, key_parts, 
                            modify_parts, user_part, admin_part, 
                            &passwd, gen_unix_passwd,
                            time_now, &passwd_version, 
                            status);   
                }
                if (GOOD_STATUS(status)) {
                    rs_log_acct_replace(login_name, modify_parts, 
                            user_part, admin_part, key, client_ids, 
                            new_keytype, unix_passwd, time_now, 
                            master_info);  
                }
            }
        } END_SLAVE_WRITE_EVENT;
    } 
    else {
        /* just add update to propq
         */
        BEGIN_RIW_EVENT(rs_c_event_propq_write, lock_db, status) { 
            rs_rep_mgmt_slave_chk_propq_upd(master_info, 
                                            &apply_update, status);
            if (apply_update) {
                if (GOOD_STATUS(status)) {
                    rs_log_acct_replace(login_name, modify_parts, 
                            user_part, admin_part, key, client_ids, 
                            new_keytype, unix_passwd, 
                            time_now, master_info); 
                } 
            }
        } END_RIW_EVENT;
    }
}


/* rs_prop_acct_key_add_version
 *
 * Add specific versions of an account key.  If current is true
 * the key is added as the current version and "garbage_collect"
 * is ignored (current keys are never garbage collected).  If
 * current is false, the key is stored as a back-version of the 
 * account's key using "garbage_collect."  This routine is used 
 * only during initialization to propagate all extant key types 
 * and versions from a surrogate master to an initializing slave.  
 * 
 * ERRORS:
 *    sec_rgy_key_bad_version  - key version already exists or 
 *        there is already a current version of this type key
 *                                 
 *    sec_rgy_key_bad_type     - key type not supported at this server.
 */
PUBLIC  void  rs_prop_acct_add_key_version
#ifndef __STDC__
    ( h, login_name, num_keys, keys, master_info, status )
    handle_t                  h;              /* [in] */
    sec_rgy_login_name_t      * login_name;   /* [in] */
    unsigned32                num_keys;       /* [in] */
    rs_prop_acct_key_data_t   keys[];         /* [in, sizeis(num_keys)] */
    rs_replica_master_info_t  * master_info;  /* [in] */
    error_status_t            * status;       /* [out] */
#else
    (
    handle_t                  h,
    sec_rgy_login_name_t      * login_name,
    unsigned32                num_keys,
    rs_prop_acct_key_data_t   keys[],
    rs_replica_master_info_t  * master_info,
    error_status_t            * status
    )
#endif
{
    sec_passwd_rec_t          passwd;
    unsigned32                cur_key;
    boolean32                 apply_update;

    CLEAR_STATUS(status);
                                 
    rs_rep_auth_check(h, status);
    if (BAD_STATUS(status)) {
        return;
    }

    BEGIN_SLAVE_WRITE_EVENT(rs_c_event_slave_write, lock_db, master_info,
        status) { 
        rs_rep_mgmt_slave_chk_update(master_info, 
                                     &apply_update, status);
        if (apply_update) {
            for(cur_key = 0; cur_key < num_keys && GOOD_STATUS(status); 
                cur_key++) {
                get_key(keys[cur_key].key, NULL, &passwd, status);

                if (GOOD_STATUS(status)) {
                    rsdb_acct_add_key_version(login_name, &passwd, 
                                          keys[cur_key].current,
                                          &keys[cur_key].garbage_collect,
                                          status);
                }
            }
        }
    } END_SLAVE_WRITE_EVENT;
}
