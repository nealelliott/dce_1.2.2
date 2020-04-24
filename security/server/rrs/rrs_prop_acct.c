/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rrs_prop_acct.c,v $
 * Revision 1.1.12.1  1996/08/09  12:08:24  arvind
 * 	Merge Registry support for KDC private key storage
 * 	[1996/06/07  19:06 UTC  aha  /main/aha_pk6/1]
 *
 * 	Changes for Public Key Login; support for Registry storage of KDC private key
 * 	[1994/08/04  16:13:15  mdf  1.1.8.1]
 *
 * Revision 1.1.10.2  1996/02/18  00:16:45  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:05:44  marty]
 * 
 * Revision 1.1.10.1  1995/12/08  17:47:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:19:29  root]
 * 
 * Revision 1.1.8.1  1994/08/04  16:13:15  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:10:42  mdf]
 * 
 * Revision 1.1.6.1  1993/10/15  20:46:28  ahop
 * 	OT7832: malloc/free large bulk transfer structures instead of using
 * 	automatic variables that gobble dangerous amounts of stack
 * 	[1993/10/15  19:26:17  ahop]
 * 
 * Revision 1.1.3.6  1993/03/19  22:20:11  emartin
 * 	CR 7472  rrs_prop_acct_replace - propagates unix_passwd arg
 * 	[1993/03/19  18:55:06  emartin]
 * 
 * Revision 1.1.3.5  1993/02/03  18:51:30  ahop
 * 	7085: free passwd->pepper after calling rsdb_acct_key_get_next in add_all
 * 	[1993/02/02  19:19:55  ahop]
 * 
 * Revision 1.1.3.4  1992/12/29  16:33:42  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:21:39  zeliff]
 * 
 * Revision 1.1.3.3  1992/12/18  22:11:11  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:47:44  sekhar]
 * 
 * Revision 1.1.4.2  1992/12/11  21:37:16  emartin
 * 	bl6c: add rrs_prop_acct_init.
 * 
 * Revision 1.1.3.2  1992/10/07  20:30:10  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:14:19  sekhar]
 * 
 * Revision 1.1.1.2  1992/10/06  15:53:50  burati
 * 	 Second replication code drop: bl5
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 *      Registry Server Propagation Client - Accounts
 */

#include <dce/dce.h>    
#include <rs_server.h>
#include <rs_util.h>
#include <rs_prop_acct.h>
#include <rrs_prop_acct.h>
#include <rrs_prop_util.h> 
#include <rrs_prop_plcy.h> 
#include <rsdb_acct.h>
#include <rsdb_policy.h>
                          
#include <rca_pwd.h>
#include <crypt.h>
#include <sec_svc.h>

/* the maximum number of accts that 
 * can be propagated in a single call 
 */
#define RS_PROP_ACCT_INIT_BULK_MAX 10

/* XXX-Size of RS_PROP_ACCT_KEY_BULK_MAX needs to be revisited if we
 * ever do full multi-crypto support.
 */
#define RS_PROP_ACCT_KEY_BULK_MAX  15 


PRIVATE  void gen_unix_passwd_wire_rep
#ifndef __STDC__
    ( encryption_key, unix_passwd, wire_rep, st )
    sec_passwd_rec_t           *encryption_key;  /* [in] */
    sec_rgy_unix_passwd_buf_t  unix_passwd;      /* [in] */
    rs_acct_key_transmit_t     **wire_rep;       /* [out] */
    error_status_t             *st;   
#else
  (
    sec_passwd_rec_t           *encryption_key,
    sec_rgy_unix_passwd_buf_t  unix_passwd,    
    rs_acct_key_transmit_t     **wire_rep,     
    error_status_t             *st
  )
#endif
{
    sec_passwd_rec_t           passwd;

    CLEAR_STATUS(st);
    memset(&passwd, 0, sizeof(passwd));

    /* 
     * there is no official "unix password" arm of the passwd union, but
     * unix passwords all live in  ascii space, 
     * so we can use the "plain" arm.
     */
    PASSWD_TYPE(&passwd) = sec_passwd_plain;
    PLAIN_PASSWD(&passwd) = 
                 (unsigned_char_p_t) malloc(u_strlen(unix_passwd) + 1);
    if (PLAIN_PASSWD(&passwd) == NULL) {
        SET_STATUS(st, sec_s_no_memory);
        return;
    }
    u_strcpy(PLAIN_PASSWD(&passwd), unix_passwd);
    rca_pwd_gen_transmit_rep(NULL, encryption_key, 
                             &passwd, wire_rep, st);
    sec_crypt_free_key_contents(&passwd);
}


/* rrs_prop_acct_add_key_version
 *
 * Add the versioned keys for the specified account.
 * "login_name" must not contain wildcards.
 * Note: this operation is only used during initialization
 * propagation.
 */
PRIVATE  void rrs_prop_acct_add_key_version
#ifndef __STDC__
    ( h, login_name, num_keys, keys, master_info, st )
    handle_t                  h;              /* [in] */
    sec_rgy_login_name_t      * login_name;   /* [in] */
    unsigned32                num_keys;       /* [in] */
    rs_prop_acct_key_data_t   keys[];         /* [in] */
    rs_replica_master_info_t  * master_info;  /* [in, ref] */
    error_status_t            * st;   
#else
  (
    handle_t                  h,              
    sec_rgy_login_name_t      * login_name,   
    unsigned32                num_keys,       
    rs_prop_acct_key_data_t   keys[],         
    rs_replica_master_info_t  * master_info,  
    error_status_t            * st
  )
#endif
{
    (*rs_prop_acct_v1_0_c_epv.rs_prop_acct_add_key_version)(h, 
                login_name, num_keys, keys, master_info, st);

}


/* rrs_prop_acct_add_keys_init
 *
 * iterate over all an account's remaining keys/key_versions and propagate 
 * them using rrs_prop_acct_add_key_version
 */
PRIVATE  void  rrs_prop_acct_add_keys_init
#ifndef __STDC__
   ( rep_binding, login_name, cursor, encryption_key, 
     client, master_info, st )
    rs_rep_binding_t          *rep_binding;     /* [in] */
    sec_rgy_login_name_t      *login_name;      /* [in] */
    sec_rgy_cursor_t          *cursor;          /* [in, out] */
    sec_passwd_rec_t          *encryption_key;  /* [in] */
    sec_rgy_foreign_id_t      *client;          /* [in] */
    rs_replica_master_info_t  *master_info;     /* [in] */  
    error_status_t            *st;  
#else
  (
    rs_rep_binding_t          *rep_binding,
    sec_rgy_login_name_t      *login_name,      
    sec_rgy_cursor_t          *cursor,          
    sec_passwd_rec_t          *encryption_key,  
    sec_rgy_foreign_id_t      *client,          
    rs_replica_master_info_t  *master_info,     
    error_status_t            *st
  )
#endif
{
    sec_passwd_rec_t          passwd;  
    rs_prop_acct_key_data_t   keys[RS_PROP_ACCT_KEY_BULK_MAX];
    unsigned32                cur_key, num_keys = 0;   
    krb5_keytype              keytype;

    memset(&passwd, 0, sizeof(passwd));
    CLEAR_STATUS(st);
    while (GOOD_STATUS(st)) {
        
        /* free any key data allocated during the previous pass */
        for (cur_key = 0; cur_key < num_keys; cur_key++) {
            rca_pwd_free_transmit_rep(&keys[cur_key].key);
        }
        
        num_keys = cur_key = 0;
        if (!cursor->valid)
            /* This is the first call; initialize keytype to
             * default.
             */
            keytype = RSDB_AUTH_DEFAULT_KEY_TYPE;
        while (STATUS_OK(st) && 
               num_keys < RS_PROP_ACCT_KEY_BULK_MAX) {

            rsdb_acct_authkey_get_next(login_name, keytype, cursor, &passwd, 
                                       &keys[cur_key].current, 
                                       &keys[cur_key].garbage_collect, 
                                       st);

            if (STATUS_EQUAL(st, sec_rgy_no_more_entries) &&
                (keytype == RSDB_AUTH_DEFAULT_KEY_TYPE)) {
                /* Reset type and cursor to get any private keys
                 * associated with this account.
                 * XXX-for general multi-crypto, this needs to
                 * be revisited.
                 */
                CLEAR_STATUS(st);
                keytype = KEYTYPE_ENCRYPT_RSA_PRIV;
                cursor->valid = false;
                continue;
            }
        
            if (GOOD_STATUS(st)) {
                rca_pwd_gen_transmit_rep(NULL, encryption_key, &passwd, 
                                         &keys[cur_key].key, st);
                if (GOOD_STATUS(st)) {
                    num_keys += 1;
                    cur_key += 1;
                }
                /* free passwd->pepper if it was allocated by key_get_next.
                 * Set it to NULL so sec_crypt_free_key_contents doesn't
                 * attempt to free it again.
                 */
                if(passwd.pepper) {
                    free(passwd.pepper);
                    passwd.pepper = NULL;
                }
            }   
        }      

        if ((GOOD_STATUS(st) || 
             STATUS_EQUAL(st, sec_rgy_no_more_entries)) &&
            num_keys > 0) {

            master_info->previous_update_seqno = master_info->update_seqno;
            rs_util_seqno_next(&master_info->update_seqno);

            PROP_SETUP_RETRY {
                rrs_prop_acct_add_key_version(rep_binding->rep_handle,
                    login_name, num_keys, keys,  master_info, st);
                PROP_CHECK_RETRY(rrs_prop_util_retry_handler,
                                 rep_binding, st);
            } PROP_END_RETRY
        }
    } /* end while (GOOD_STATUS)) */

    /*
     * clean up storage for any key data allocated in the final pass through
     * the loop
     */
    for (cur_key = 0; cur_key <  num_keys; cur_key++) {
        rca_pwd_free_transmit_rep(&keys[cur_key].key);
    }

    /* don't leave the last passwd retrieved hanging around in memory */
    sec_crypt_free_key_contents(&passwd);
 
    if (STATUS_EQUAL(st, sec_rgy_no_more_entries)) {
        CLEAR_STATUS(st);
    }
}

/*
 *  rrs_prop_acct_add_init
 *
 */
PRIVATE void rrs_prop_acct_add_init
#ifndef __STDC__
    (rep_binding, num_accts, accts, master_info, st)
    rs_rep_binding_t            *rep_binding;   /* [in] */
    unsigned32                  num_accts;      /* [in] */
    rs_prop_acct_add_data_t     accts[];        /* [in] */
    rs_replica_master_info_t    *master_info;   /* [in, out] */
    error_status_t              *st;
#else
  (
    rs_rep_binding_t            *rep_binding,
    unsigned32                  num_accts,
    rs_prop_acct_add_data_t     accts[],
    rs_replica_master_info_t    *master_info,
    error_status_t              *st
  )
#endif
{
    CLEAR_STATUS(st);

    master_info->previous_update_seqno = master_info->update_seqno;
    rs_util_seqno_next(&master_info->update_seqno);

    PROP_SETUP_RETRY {

        rrs_prop_acct_add(rep_binding->rep_handle,
            num_accts, accts, master_info, false, st);

        PROP_CHECK_RETRY(rrs_prop_util_retry_handler, rep_binding, st)

    } PROP_END_RETRY
}


/*  rrs_prop_acct_add_all
 *
 * Propagate all accounts to an initializing slave.  
 */
PUBLIC  void  rrs_prop_acct_add_all
#ifndef __STDC__
    ( rep_binding, encryption_key, master_info, st ) 
    rs_rep_binding_t           *rep_binding;      /* [in] */
    sec_passwd_rec_t           *encryption_key;   /* [in] */
    rs_replica_master_info_t   *master_info;      /* [in] */  
    error_status_t             *st;         
#else
  (
    rs_rep_binding_t           *rep_binding,
    sec_passwd_rec_t           *encryption_key, 
    rs_replica_master_info_t   *master_info,     
    error_status_t             *st
  )
#endif
{
    sec_rgy_cursor_t         cursor;
    rs_prop_acct_add_data_t  *bulk;
    sec_rgy_cursor_t         key_cursors[RS_PROP_ACCT_INIT_BULK_MAX];
    sec_rgy_sid_t            sid;
    sec_rgy_unix_sid_t       unix_sid;
    sec_rgy_plcy_auth_t      auth_policy;
    sec_passwd_rec_t         passwd;
    sec_rgy_acct_key_t       key_parts;   
    sec_rgy_foreign_id_t     nil_ids;  
    unsigned32               num_accts = 0, cur_acct;   
    boolean32                done;
    
    rs_util_clear_cursor(&cursor);
    CLEAR_STATUS(st);
    uuid_create_nil(&nil_ids.principal, st);
    uuid_create_nil(&nil_ids.cell, st);
    
    if (BAD_STATUS(st)) {   /* highly unlikely */
        return;
    }
    
    memset(&passwd, 0, sizeof(passwd));
    if(!(bulk =
        (rs_prop_acct_add_data_t *)malloc(RS_PROP_ACCT_INIT_BULK_MAX *
                                   sizeof(rs_prop_acct_add_data_t)))) {
        SET_STATUS(st, sec_rgy_cant_allocate_memory);
	return;
    }
    memset(bulk, 0, 
           RS_PROP_ACCT_INIT_BULK_MAX * sizeof(rs_prop_acct_add_data_t));
    /* 
     * set all client_ids to uuid_nil so the reciever will know that keys are 
     * encrypted under the initialization session key.
     */
    for (cur_acct = 0; cur_acct <  RS_PROP_ACCT_INIT_BULK_MAX; 
         cur_acct++) {
        (bulk + cur_acct)->client = nil_ids;
    }
    
    done = false;
    while (GOOD_STATUS(st) && !done) {
        
        /* free any key data allocated during the previous pass */
        for (cur_acct = 0; cur_acct < num_accts; cur_acct++) {
            rca_pwd_free_transmit_rep(&((bulk + cur_acct)->key));
            rca_pwd_free_transmit_rep(&((bulk + cur_acct)->unix_passwd));
        }
        
        num_accts = 0;
        while (STATUS_OK(st) && 
               num_accts < RS_PROP_ACCT_INIT_BULK_MAX) {
            CLEAR_LOGIN_NAME(&((bulk + num_accts)->login_name));
            
            rsdb_acct_lookup(&((bulk + num_accts)->login_name), 
                             &cursor, &sid, &unix_sid,
                             &key_parts, &((bulk + num_accts)->user_part), 
                             &((bulk + num_accts)->admin_part), st);
            
            if (GOOD_STATUS(st)) {
                boolean32          current;
                sec_timeval_sec_t  dummy_key_exp;
                /*
                 * get a current key for this account.  It must be 
                 * a current key because only a current key can be 
                 * propagated in an acct add.  Any previous key versions
                 * will be propagated after the account add.
                 */
                rs_util_clear_cursor(&key_cursors[num_accts]);
                rsdb_acct_key_get_next(&((bulk + num_accts)->login_name),
                                       &key_cursors[num_accts], &passwd, 
                                       &current, &dummy_key_exp, st);
                if (!current) {
                    dce_svc_printf(SEC_RRS_CANT_GET_NEXT_KEY_MSG);
                }
            
                if (GOOD_STATUS(st)) {  
                    /*
                     * generate the wire rep of the passwd encrypted under 
                     * the encryption key provided by the caller 
                     */
                    rca_pwd_gen_transmit_rep(NULL, encryption_key, &passwd, 
                                             &((bulk + num_accts)->key), st);

                    /* free passwd.pepper if not NULL. */
                    if(passwd.pepper) {
                        free(passwd.pepper);
                        passwd.pepper = NULL;
                    }
                }
                if (GOOD_STATUS(st)) {
                    /* 
                     * a unix password might exist for the current version
                     * of the key.  As only DES keys are currently supported,
                     * we can assume there is only a single current key.  If
                     * and when multiple key types are supported, handling of
                     * unix passwords will get a bit trickier
                     */
                    gen_unix_passwd_wire_rep(encryption_key, 
                                     (bulk + num_accts)->user_part.passwd,
                                     &((bulk + num_accts)->unix_passwd),
                                     st);
                }
                if (GOOD_STATUS(st)) {
                    num_accts += 1;
                }
            }
        }  /* end while */
        
        done = STATUS_EQUAL(st, sec_rgy_no_more_entries)
               || STATUS_EQUAL(st, sec_rgy_object_not_found); 
       
        if (GOOD_STATUS(st) || done) {

            if (num_accts > 0) {
            
                rrs_prop_acct_add_init(rep_binding, num_accts, bulk, 
                                       master_info, st);
            
                if (GOOD_STATUS(st)) {
                    
                    for (cur_acct = 0; 
                    cur_acct < num_accts && GOOD_STATUS(st);
                    cur_acct++) {
                        /* 
                         * propagate authentication policy for this
                         * account, if any 
                         */
                        rsdb_auth_policy_get_info(
                                    &((bulk + cur_acct)->login_name), 
                                    &auth_policy, st);
                        
                        if (GOOD_STATUS(st)) {
                            master_info->previous_update_seqno = 
                                    master_info->update_seqno;
                            rs_util_seqno_next(&master_info->update_seqno);

                            PROP_SETUP_RETRY {
                                rrs_prop_auth_plcy_set_info(
                                    rep_binding->rep_handle,  
                                    &((bulk + cur_acct)->login_name), 
                                    &auth_policy, master_info, 
                                    false, st);
                                PROP_CHECK_RETRY(
                                    rrs_prop_util_retry_handler, 
                                    rep_binding, st)
                            } PROP_END_RETRY

                        } else if (STATUS_EQUAL(st, sec_rgy_object_not_found)) {
                            CLEAR_STATUS(st);
                        }
                        
                        if (GOOD_STATUS(st)) {   
                            rrs_prop_acct_add_keys_init(rep_binding,
                                        &((bulk + cur_acct)->login_name),
                                        &key_cursors[cur_acct],
                                        encryption_key, 
                                        &nil_ids, master_info, st);
                        }
                    }
                }
            }
        }     
    }   /* end loop - get all accounts */

    /*
     * clean up storage for any key data allocated in the final pass through
     * the loop
     */
    for (cur_acct = 0; cur_acct <  num_accts; cur_acct++) {
        rca_pwd_free_transmit_rep(&((bulk + cur_acct)->key));
        rca_pwd_free_transmit_rep(&((bulk + cur_acct)->unix_passwd));
    }
    /* 
     * Free the bulk storage
     */
    free(bulk);

    /* don't leave the last passwd retrieved hanging around in memory */
    sec_crypt_free_key_contents(&passwd);

    if (STATUS_EQUAL(st, sec_rgy_no_more_entries)
    || STATUS_EQUAL(st, sec_rgy_object_not_found))  {
        CLEAR_STATUS(st);
    }

}


/* rrs_prop_acct_add
 * 
 */
PUBLIC  void  rrs_prop_acct_add
#ifndef __STDC__
    ( h, num_accts, accts, master_info, propq_only, st ) 
    handle_t                  h;              /* [in] */
    unsigned32                num_accts;      /* [in] */
    rs_prop_acct_add_data_t   accts[];        /* [in] */
    rs_replica_master_info_t  *master_info;   /* [in] */
    boolean32                 propq_only;     /* [in] */
    error_status_t            *(st);
#else
  (
    handle_t                  h,              
    unsigned32                num_accts,      
    rs_prop_acct_add_data_t   accts[],        
    rs_replica_master_info_t  *master_info,   
    boolean32                 propq_only,     
    error_status_t            *(st)
  )
#endif
{
    (*rs_prop_acct_v1_0_c_epv.rs_prop_acct_add) (h, num_accts,
                accts, master_info, propq_only, st);
}

/*
 * rrs_prop_acct_delete
 */
PUBLIC  void  rrs_prop_acct_delete
#ifndef __STDC__
    ( h, login_name, master_info, propq_only, st ) 
    handle_t                  h;              /* [in] */
    sec_rgy_login_name_t      * login_name;   /* [in] */
    rs_replica_master_info_t  * master_info;  /* [in, ref] */
    boolean32                 propq_only;     /* [in] */
    error_status_t            * st; 
#else
  (
    handle_t                  h,              
    sec_rgy_login_name_t      * login_name,   
    rs_replica_master_info_t  * master_info,  
    boolean32                 propq_only,     
    error_status_t            * st
  )
#endif
{
        (*rs_prop_acct_v1_0_c_epv.rs_prop_acct_delete) (h, 
                    login_name, master_info, propq_only, st);
}  



/*
 * rrs_prop_acct_rename
 */
PUBLIC  void  rrs_prop_acct_rename
#ifndef __STDC__
    ( h, old_login_name, new_login_name, master_info, 
      propq_only, st )
    handle_t                  h;                 /* [in] */
    sec_rgy_login_name_t      * old_login_name;  /* [in] */
    sec_rgy_login_name_t      * new_login_name;  /* [in] */
    rs_replica_master_info_t  * master_info;     /* [in, ref] */
    boolean32                 propq_only;        /* [in] */
    error_status_t            * st;
#else
  (
    handle_t                  h,                 
    sec_rgy_login_name_t      * old_login_name,  
    sec_rgy_login_name_t      * new_login_name,  
    rs_replica_master_info_t  * master_info,     
    boolean32                 propq_only,        
    error_status_t            * st
  )
#endif
{
    (*rs_prop_acct_v1_0_c_epv.rs_prop_acct_rename) (h, old_login_name, 
                new_login_name, master_info, propq_only, st);
}

/*
 * rrs_prop_acct_replace 
 * 
 * "client_ids" identifies (by uuid) the principal under whose key the
 * updated "key" and "unix_passwd" are encrypted.  If "client_ids" 
 * contains NIL uuids, "key" and "unix_passwd" are encrypted under 
 * a session key.
 */
PUBLIC  void rrs_prop_acct_replace
#ifndef __STDC__
    ( h, login_name, modify_parts, user_part, admin_part, key, 
      client, new_keytype, unix_passwd, master_time, master_info, 
      propq_only, st )
    handle_t                  h;              /* [in] */
    sec_rgy_login_name_t      * login_name;   /* [in] */
    rs_acct_parts_t           modify_parts;   /* [in] */
    sec_rgy_acct_user_t       * user_part;    /* [in] */
    sec_rgy_acct_admin_t      * admin_part;   /* [in] */
    rs_acct_key_transmit_t    * key;          /* [in] may be NULL */
    sec_rgy_foreign_id_t      *client;        /* [in, ref] */
    sec_passwd_type_t         new_keytype;    /* [in] */
    rs_acct_key_transmit_t    *unix_passwd;   /* [in]  may be NULL */
    sec_timeval_sec_t         * master_time;  /* [in, ref] */
    rs_replica_master_info_t  * master_info;  /* [in, ref] */
    boolean32                 propq_only;     /* [in] */
    error_status_t            * st;   
#else
  (
    handle_t                  h,              
    sec_rgy_login_name_t      * login_name,   
    rs_acct_parts_t           modify_parts,   
    sec_rgy_acct_user_t       * user_part,    
    sec_rgy_acct_admin_t      * admin_part,   
    rs_acct_key_transmit_t    * key,          
    sec_rgy_foreign_id_t      *client,        
    sec_passwd_type_t         new_keytype,    
    rs_acct_key_transmit_t    *unix_passwd,
    sec_timeval_sec_t         * master_time,  
    rs_replica_master_info_t  * master_info,  
    boolean32                 propq_only,     
    error_status_t            * st
  )
#endif
{
    (*rs_prop_acct_v1_0_c_epv.rs_prop_acct_replace)(h, login_name, 
                modify_parts, user_part, admin_part,
                key, client, new_keytype, unix_passwd,
                master_time, master_info,
                propq_only, st);
}

