/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: seckey_list.c,v $
 * Revision 1.1.12.2  1996/03/11  13:27:41  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:15:31  marty]
 *
 * Revision 1.1.12.1  1995/12/08  18:03:37  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/09/08  18:19 UTC  burati
 * 	Merge fix for CHFts12162 from mb_mothra6
 * 
 * 	HP revision /main/sommerfeld_mothpatch/1  1995/08/07  20:05 UTC  sommerfeld
 * 	CHFts12162 -- If max ticket lifetime is zero, don't GC
 * 		(requires manual intevention in this case..)
 * 	[1995/12/08  16:31:07  root]
 * 
 * Revision 1.1.9.1  1994/08/26  16:01:14  sommerfeld
 * 	Fix for beta4.
 * 	[1994/08/26  15:50:53  sommerfeld]
 * 
 * Revision 1.1.7.2  1993/05/13  16:14:47  burati
 * 	CR7612 Fix garbage_collect's alg for deciding which keys to delete
 * 	[1993/05/13  14:25:15  burati]
 * 
 * Revision 1.1.5.6  1993/02/10  19:22:22  burati
 * 	CR7182 Call sec_key_mgmt__bind_rgy with update set to false
 * 	[1993/02/10  18:48:20  burati]
 * 
 * Revision 1.1.5.5  1993/02/08  19:43:48  sommerfeld
 * 	[OT7153] Fix status code conversion.
 * 	[1993/02/08  18:29:26  sommerfeld]
 * 
 * Revision 1.1.5.4  1993/01/18  19:45:46  burati
 * 	CR6798 Wasn't freeing global_name in get_next_key & get_next_ts
 * 	[1993/01/18  18:54:41  burati]
 * 
 * Revision 1.1.5.3  1992/12/29  12:43:14  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:38:07  zeliff]
 * 
 * Revision 1.1.5.2  1992/09/29  21:15:03  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:19:02  sekhar]
 * 
 * Revision 1.1.2.3  1992/06/18  21:22:34  burati
 * 	CR3983 Translate ENOENT to sec_key_mgmt_e_keytab_not_found
 * 	[1992/06/18  21:19:27  burati]
 * 
 * Revision 1.1.2.2  1992/05/26  21:30:15  pato
 * 	OT#3770 Keytab file only needs read access for query functions
 * 	the previous change did not really address OT 3620.
 * 	[1992/05/22  20:30:57  pato]
 * 
 * 	OT#3620 Keytab file only needs read access.
 * 	[1992/05/22  14:02:19  pato]
 * 
 * Revision 1.1  1992/01/19  14:48:05  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
** Copyright (c) Hewlett-Packard Company 1991, 1992, 1993
** Unpublished work. All Rights Reserved.
*/
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */

/* 
 *  DCE Security key management interface
 */

#ifdef MODULE_VERSION_ID
static char *VersionID =
"$Id: seckey_list.c,v 1.1.12.2 1996/03/11 13:27:41 marty Exp $";
#endif

#include <seckey_util.h>
#include <dce/keymgmt.h>
#include <dce/sec_authn.h>
#include <dce/policy.h>

#include <rgymacro.h>
#include <un_strng.h>
#include <un_maloc.h>
#include <un_time.h>

#include <u_str.h>

#include <sys/errno.h>

typedef struct sec_key_mgmt__cursor {
    krb5_kt_cursor          ktcursor;
    krb5_keytab             ktid;
    unsigned_char_p_t       gprinc;
    sec_passwd_type_t       keytype;
} sec_key_mgmt__cursor_t;

typedef struct knode {
    unsigned32      version;
    krb5_timestamp  timestamp;
} key_node_t, *key_node_p_t;

#define KTID(CURSOR)        ((sec_key_mgmt__cursor_t *) (CURSOR))->ktid
#define KTCURSOR(CURSOR)    ((sec_key_mgmt__cursor_t *) (CURSOR))->ktcursor
#define GPRINC(CURSOR)      ((sec_key_mgmt__cursor_t *) (CURSOR))->gprinc
#define KEYTYPE(CURSOR)     ((sec_key_mgmt__cursor_t *) (CURSOR))->keytype

/* State information */
static krb5_keytab  keytabid;

/* s e c _ k e y _ m g m t _ g e t _ n t h _ k e y _ i n t e r n a l
 *
 * Extract the specified key from the local key store.  This operation 
 * can be used to scan all keys belonging to the specified principal and
 * stored in the local key store.
 *
 * Argument Usage: 
 *      INPUT
 *  authn_service   **  select the underlying authentication scheme
 *  arg             **  key acquisition function argument (same value
 *                      that is passed to rpc_server_register_auth_info.)
 *  principal_name  **  names the principal whose key is to be changed
 *  index               The nth key to extract.  0 indicates the first
 *                      key available.
 *  keytype         **  The keytype desired (type "none" if any will do)
 *
 *      IN/OUT
 *  key_vno             the key version to extract.
 *
 *  ** see NOTE on authenticated RPC related arguments above.
 *
 *      OUTPUT
 *  keydata         **  the key value extracted.  This data may be released
 *                      by calling sec_key_mgmt_free_key.
 *  name_out            The principal name for the entry found
 *  err                 error status if any.
 * 
 * Any error condition will leave the key state unchanged.
 *
 * Errors:
 *  sec_key_mgmt_e_key_unavailable      Requested key not present
 *  sec_key_mgmt_e_authn_invalid        Authentication service invalid
 *  sec_key_mgmt_e_unauthorized         Caller unauthorized to perform op
 */
void sec_key_mgmt_get_nth_key_internal
#ifndef __STDC__
    ( authn_service, arg, principal_name, index, keytype, key_vno, keydata, 
        name_out, err )
    sec_key_mgmt_authn_service  authn_service; 
    void                        * arg;
    unsigned char               * principal_name;
    signed32                    index;
    sec_passwd_type_t           keytype;
    unsigned32                  * key_vno;
    sec_passwd_rec_t            * keydata;
    unsigned char               ** name_out;
    error_status_t              * err;
#else
  (
    sec_key_mgmt_authn_service  authn_service, 
    void                        * arg,
    unsigned char               * principal_name,
    signed32                    index,
    sec_passwd_type_t           keytype,
    unsigned32                  * key_vno,
    sec_passwd_rec_t            * keydata,
    unsigned char               ** name_out,
    error_status_t              * err 
  )
#endif
{
    error_status_t          lst;
    krb5_keytab             ktid;
    krb5_kt_cursor          cursor;
    krb5_keytab_entry       ktentry;
    krb5_kvno               kvno;
    char                    *new_name;
    unsigned_char_p_t       gprinc = NULL;
    unsigned_char_p_t       global_name;

    CLEAR_STATUS(err);

    SEC_KEY_CHECK_AUTHN(authn_service, err);

    SEC_KEY_CHECK_INIT(err);

    if (*err) {
        return;
    }

    if (!sec_key_mgmt__resolve_mode(arg, true, &ktid, err)) {
        return;
    }

    if (principal_name && *principal_name) {
        *err = sec_id_globalize_name(principal_name, &gprinc);
    }

    if (*err == 0) {
        *err = krb5_kt_start_seq_get(ktid, &cursor);
    }

    if (*err == 0) {

        index++;
        while (index) {
            *err = krb5_kt_next_entry(ktid, &ktentry, &cursor);
            if (*err) {
                if (*err == KRB5_KT_END) {
                    *err = sec_key_mgmt_e_key_unavailable;
                }
                break;
            }

            /*
             * Check to see if the key is for the desired principal
             */
            if (principal_name || name_out) {
                if (*err = krb5_unparse_name(ktentry.principal, &new_name)) {
                    break;
                }
                *err = sec_id_globalize_name((unsigned_char_p_t) new_name, 
                                                &global_name);
                free(new_name);
                if (*err) {
                    break;
                }
            }

            if (((keytype == sec_passwd_none)
                    || ((keytype == sec_passwd_des)
                           && (ktentry.key.keytype == KEYTYPE_DES)))
                && ((*key_vno == 0) || (*key_vno == ktentry.vno))
                && ((gprinc == NULL)
                  	|| u_strcmp(gprinc, global_name) == 0) ) {
                index--;
            }

            if (index == 0) {
                if (name_out) {
                    *name_out = global_name;
                }

                keydata->version_number = ktentry.vno;
                keydata->key.key_type   = sec_passwd_des; 
                keydata->pepper         = NULL;
                memcpy(DES_PASSWD(keydata), ktentry.key.contents, 
                                sec_passwd_c_des_key_size);

                *key_vno = ktentry.vno;
            }
            krb5_kt_free_entry(&ktentry);
            if (global_name) {
                if (!name_out || (*name_out != global_name)) {
                    free(global_name);
                }
            }
        }

        /*
         * Release the sequential cursor
         */
        lst = krb5_kt_end_seq_get(ktid, &cursor);
        if (lst && !*err) {
            *err = lst;
        }
    }

    *err = sec_key_mgmt__convert_status(*err);

    if (gprinc) {
        free(gprinc);
    }

    /*
     * Release the keytab table.
     */
    krb5_kt_close(ktid);
 
}

/* s e c _ k e y _ m g m t _ g e t _ n t h _ k e y 
 *
 * Extract the specified key from the local key store.  This operation 
 * can be used to scan all keys belonging to the specified principal and
 * stored in the local key store.
 *
 * Argument Usage: 
 *      INPUT
 *  authn_service   **  select the underlying authentication scheme
 *  arg             **  key acquisition function argument (same value
 *                      that is passed to rpc_server_register_auth_info.)
 *  principal_name  **  names the principal whose key is to be changed
 *  index               The nth key to extract.  0 indicates the first
 *                      key available.
 *
 *  ** see NOTE on authenticated RPC related arguments above.
 *
 *      OUTPUT
 *  key_vno             the key version extracted.
 *  keydata         **  the key value extracted.  This data may be released
 *                      by calling sec_key_mgmt_free_key.
 *  err                 error status if any.
 * 
 * Any error condition will leave the key state unchanged.
 *
 * Errors:
 *  sec_key_mgmt_e_key_unavailable      Requested key not present
 *  sec_key_mgmt_e_authn_invalid        Authentication service invalid
 *  sec_key_mgmt_e_unauthorized         Caller unauthorized to perform op
 */
void sec_key_mgmt_get_nth_key
#ifndef __STDC__
    ( authn_service, arg, principal_name, index, key_vno, keydata, err )
    sec_key_mgmt_authn_service  authn_service; 
    void                        * arg;
    unsigned char               * principal_name;
    signed32                    index;
    unsigned32                  * key_vno;
    void                        ** keydata;
    error_status_t              * err;
#else
  (
    sec_key_mgmt_authn_service  authn_service, 
    void                        * arg,
    unsigned char               * principal_name,
    signed32                    index,
    unsigned32                  * key_vno,
    void                        ** keydata,
    error_status_t              * err 
  )
#endif
{
    error_status_t          lst;
    sec_passwd_rec_t        *keys;

    if (!err) {
        err = &lst;
    }

    CLEAR_STATUS(err);

    SEC_KEY_CHECK_AUTHN(authn_service, err);

    SEC_KEY_CHECK_INIT(err);

    if (STATUS_OK(err)) {
        keys = (sec_passwd_rec_t *) malloc(2 * sizeof(*keys));
        *key_vno = 0;
        if (keys) {
            sec_key_mgmt_get_nth_key_internal(authn_service, arg, principal_name,
                                                index, sec_passwd_none, key_vno, 
                                                keys, NULL, err);
            if (STATUS_OK(err)) {
                /*
                 * Mark end of key list!
                 */
                keys[1].version_number  = 0;
                keys[1].key.key_type    = sec_passwd_none;

                *keydata = (void *) keys;
            } else {
                free((char *) keys);
            }
        } else {
            *err = sec_s_no_memory;
        }
    }
}


/* s e c _ k e y _ m g m t _ g e t _ n e x t _ k e y 
 *
 * Extract keys from the local key store.  This operation can be used to
 * scan all keys in the local key store.
 *
 * Argument Usage: 
 *      INPUT
 *  cursor              Marker into key storage.
 *
 *  ** see NOTE on authenticated RPC related arguments above.
 *
 *      OUTPUT
 *  principal_name      name of the principal assoc with this key
 *  key_vno             the key version extracted.
 *  keydata         **  the key value extracted.  This data may be released
 *                      by calling sec_key_mgmt_free_key.
 *  err                 error status if any.
 * 
 * Errors:
 *  sec_key_mgmt_e_authn_invalid        Authentication service invalid
 *  sec_key_mgmt_e_unauthorized         Caller unauthorized to perform op
 */
void sec_key_mgmt_get_next_key
#ifndef __STDC__
    (cursor, principal_name, key_vno, keydata, err)
    void                        *cursor;
    unsigned char               **principal_name;
    unsigned32                  *key_vno;
    void                        **keydata;
    error_status_t              *err;
#else
  (
    void                        *cursor,
    unsigned char               **principal_name,
    unsigned32                  *key_vno,
    void                        **keydata,
    error_status_t              *err
  )
#endif
{
    error_status_t          lst;
    krb5_keytab_entry       ktentry;
    krb5_kvno               kvno;
    char                    *new_name;
    unsigned_char_p_t       global_name;
    sec_passwd_rec_t        *keys;
    boolean32               found = false;

    CLEAR_STATUS(err);

    while (GOOD_STATUS(err) && !found) {

        *err = krb5_kt_next_entry(KTID(cursor), &ktentry, &KTCURSOR(cursor));
        if (*err == KRB5_KT_END) {
            SET_STATUS(err, sec_key_mgmt_e_key_unavailable);
            return;
        }

        /*
         * Check to see if the key is for the desired principal
         */
        *err = krb5_unparse_name(ktentry.principal, &new_name);
        if (GOOD_STATUS(err))
            *err = sec_id_globalize_name((unsigned_char_p_t) new_name,
                                            &global_name);
        free(new_name);

        if ((GOOD_STATUS(err)) &&
            ((KEYTYPE(cursor) == sec_passwd_none)
                || ((KEYTYPE(cursor) == sec_passwd_des)
                        && (ktentry.key.keytype == KEYTYPE_DES)))
            && ((GPRINC(cursor) == NULL)
              	|| u_strcmp(GPRINC(cursor), global_name) == 0) ) {

            /*
             * We got a key, lets allocate and fill in the 
             * sec_passwd_rec_t - in this case we know there is only
             * one key so we always allocate an array of two structs.
             */

            if (principal_name) {
                *principal_name = global_name;
            } else if (global_name) {
                free(global_name);
            }

            /*
             * Allocate and copy the key itself if it was asked for.
             */
            if (keydata) {
                keys = (sec_passwd_rec_t *) malloc(2 * sizeof(*keys));
                if (keys) {
                    keys[0].version_number  = ktentry.vno;
                    keys[0].pepper          = NULL;
                    keys[0].key.key_type    = sec_passwd_des;
                    memcpy(keys[0].key.tagged_union.des_key, ktentry.key.contents,
                                    sec_passwd_c_des_key_size);
                    /*
                     * Mark end of key list!
                     */
                    keys[1].version_number  = 0;
                    keys[1].key.key_type    = sec_passwd_none;
    
                    *keydata = (void *) keys;
                } else {
                    SET_STATUS(err, sec_s_no_memory);
                }
            }

            *key_vno = ktentry.vno;
            found = true;
        } else if (GOOD_STATUS(err) && (global_name != NULL)) {
            free(global_name);
        }
        krb5_kt_free_entry(&ktentry);

	*err = sec_key_mgmt__convert_status (*err);
    }
}


/* s e c _ k e y _ m g m t _ i n i t i a l i z e _ c u r s o r
 *
 * Reset the database cursor to return the first suitable entry. 
 *
 * Argument Usage: 
 *      INPUT
 *  authn_service   **  select the underlying authentication scheme
 *  arg             **  key acquisition function argument (same value
 *                      that is passed to rpc_server_register_auth_info.)
 *  principal_name  **  name of the principal used in searches (or NULL).
 *  keytype         **  the keytype
 *
 *  ** see NOTE on authenticated RPC related arguments above.
 *
 *      OUTPUT
 *  cursor              Marker into key storage.
 *  err                 error status if any.
 * 
 * Errors:
 *  sec_key_mgmt_e_authn_invalid        Authentication service invalid
 *  sec_key_mgmt_e_unauthorized         Caller unauthorized to perform op
 */
void sec_key_mgmt_initialize_cursor
#ifndef __STDC__
    (authn_service, arg, principal_name, keytype, cursor, err)
    sec_key_mgmt_authn_service  authn_service;
    void                        *arg;
    unsigned char               *principal_name;
    void                        *keytype;
    void                        **cursor;
    error_status_t              *err;
#else
  (
    sec_key_mgmt_authn_service  authn_service,
    void                        *arg,
    unsigned char               *principal_name,
    void                        *keytype,
    void                        **cursor,
    error_status_t              *err
  )
#endif
{
    CLEAR_STATUS(err);

    *cursor = (void *)malloc(sizeof(sec_key_mgmt__cursor_t));
    if (!*cursor) {
        SET_STATUS(err, sec_s_no_memory);
        return;
    }
    GPRINC(*cursor) = NULL;
    KEYTYPE(*cursor) = *(sec_passwd_type_t *)keytype;

    SEC_KEY_CHECK_AUTHN(authn_service, err);
    if (GOOD_STATUS(err)) {
        SEC_KEY_CHECK_INIT(err);

        if (!sec_key_mgmt__resolve_mode(arg, true, &KTID(*cursor), err)) {
	    *err = sec_key_mgmt__convert_status(*err);
            return;
        }

        if (principal_name && *principal_name) {
            *err = sec_id_globalize_name(principal_name, &GPRINC(*cursor));
        }

        if (GOOD_STATUS(err)) {
            *err = krb5_kt_start_seq_get(KTID(*cursor), &KTCURSOR(*cursor));
        }
    }
    *err = sec_key_mgmt__convert_status(*err);
}


/* s e c _ k e y _ m g m t _ r e l e a s e _ c u r s o r
 *
 * Release information associated with the specified cursor
 */
void sec_key_mgmt_release_cursor
#ifndef __STDC__
    (cursor, err)
    void                        **cursor;
    error_status_t              *err;
#else
  (
    void                        **cursor,
    error_status_t              *err
  )
#endif
{
    CLEAR_STATUS(err);
    if (*cursor) {
        *err = krb5_kt_end_seq_get(KTID(*cursor), &KTCURSOR(*cursor));
        if (GPRINC(*cursor)) {
            free(GPRINC(*cursor));
        }
        /*
         * Release the keytab table.
         */
        krb5_kt_close(KTID(*cursor));

	*err = sec_key_mgmt__convert_status(*err);
        free(*cursor);
        *cursor = NULL;
    }
}


/* s e c _ _ k e y _ m g m t _ g e t _ n e x t _ t s
 *
 * Extract key timestamps from the local key store.  This operation can be used
 * to scan all keys in the local key store.
 *
 * Argument Usage: 
 *      INPUT
 *  cursor              Marker into key storage.
 *
 *  ** see NOTE on authenticated RPC related arguments above.
 *
 *      OUTPUT
 *  principal_name      name of the principal assoc with this key
 *  key_vno             the key version extracted.
 *  timestamp           timestamp of the key extracted.
 *  err                 error status if any.
 * 
 * Errors:
 *  sec_key_mgmt_e_authn_invalid        Authentication service invalid
 *  sec_key_mgmt_e_unauthorized         Caller unauthorized to perform op
 */
void sec__key_mgmt_get_next_ts
#ifndef __STDC__
    (cursor, principal_name, key_vno, timestamp, err)
    void                        *cursor;
    unsigned char               **principal_name;
    unsigned32                  *key_vno;
    krb5_timestamp              *timestamp;
    error_status_t              *err;
#else
  (
    void                        *cursor,
    unsigned char               **principal_name,
    unsigned32                  *key_vno,
    krb5_timestamp              *timestamp,
    error_status_t              *err
  )
#endif
{
    error_status_t          lst;
    krb5_keytab_entry       ktentry;
    krb5_kvno               kvno;
    char                    *new_name;
    unsigned_char_p_t       global_name;
    sec_passwd_rec_t        *keys;
    boolean32               found = false;

    CLEAR_STATUS(err);

    while (GOOD_STATUS(err) && !found) {

        /* Get the next key */
        *err = krb5_kt_next_entry(KTID(cursor), &ktentry, &KTCURSOR(cursor));
        if (*err == KRB5_KT_END) {
            SET_STATUS(err, sec_key_mgmt_e_key_unavailable);
            return;
        }

        /*
         * Check to see if the key is for the desired principal
         */
        *err = krb5_unparse_name(ktentry.principal, &new_name);
        if (GOOD_STATUS(err))
            *err = sec_id_globalize_name((unsigned_char_p_t) new_name,
                                            &global_name);
        free(new_name);

        if ((GOOD_STATUS(err)) &&
            ((KEYTYPE(cursor) == sec_passwd_none)
                || ((KEYTYPE(cursor) == sec_passwd_des)
                        && (ktentry.key.keytype == KEYTYPE_DES)))
            && ((GPRINC(cursor) == NULL)
              	|| u_strcmp(GPRINC(cursor), global_name) == 0) ) {

            /* We got a key */
            if (principal_name) {
                *principal_name = global_name;
            } else if (global_name) {
                free(global_name);
            }

            *timestamp = ktentry.timestamp;
            *key_vno = ktentry.vno;
            found = true;
        } else if (GOOD_STATUS(err) && (global_name != NULL)) {
            free(global_name);
        }
        krb5_kt_free_entry(&ktentry);
    
	*err = sec_key_mgmt__convert_status(*err);
    }
}


/*
 * compare_keynodes - Internal rtn to compare two timestamp values
 */
PRIVATE int compare_keynodes
#ifndef __STDC__
    (key_node1, key_node2)
    void       *key_node1;
    void       *key_node2;
#else
  (
    const void *key_node1,
    const void *key_node2
  )
#endif
{
    return (((key_node_p_t)key_node2)->timestamp -
            ((key_node_p_t)key_node1)->timestamp);
}


/* s e c _ k e y _ m g m t _ g a r b a g e _ c o l l e c t
 *
 * Discard any obsolete key information for this principal.
 *
 * Note: it can't use any of the existing get/list rtns because
 * they don't return the internal ktentry structure which contains
 * the timestamp we need.
 *
 *  ** see NOTE on authenticated RPC related arguments above.
 *
 * Errors:
 *  If any operation yields an error, that error is returned immediately.
 *  sec_key_mgmt_e_authn_invalid        Authentication service invalid.
 *  sec_key_mgmt_e_unauthorized         Caller unauthorized to perform op.
 */
void sec_key_mgmt_garbage_collect
#ifndef __STDC__
    ( authn_service, arg, principal_name, err )
    sec_key_mgmt_authn_service  authn_service; 
    void                        * arg;
    unsigned char               * principal_name;
    error_status_t              * err;
#else
  (
    sec_key_mgmt_authn_service  authn_service, 
    void                        * arg,
    unsigned char               * principal_name,
    error_status_t              * err 
  )
#endif
{
    unsigned32              next_vno = -1;
    error_status_t          lst;
    sec_passwd_type_t       keytype = sec_passwd_none;
    sec_rgy_login_name_t    account;
    sec_rgy_plcy_auth_t     auth_policy;
    struct timeval          tm;
    int                     oldest_tkt, i, num_keys = 0;
    void                    *cursor;
    krb5_timestamp          timestamp, oldest_valid_key = 0;
    key_node_t              *key_list = NULL;
    sec_rgy_handle_t        rgy_h;

    CLEAR_STATUS(err);

    if ((principal_name == NULL) || (strlen((char *)principal_name) == 0)) {
        SET_STATUS(err, sec_key_mgmt_e_key_unavailable);
        return;
    }

    /* Get the complete list of keys, so we can go through them in
     * reverse chronological order (based on timestamp) to find the
     * oldest possible valid key and remove any older than that one.
     */
    sec_key_mgmt_initialize_cursor(authn_service, arg, principal_name,
                                    &keytype, &cursor, err);
    if (BAD_STATUS(err)) {
        return;
    }
    while (GOOD_STATUS(err)) {
        sec__key_mgmt_get_next_ts(cursor, NULL, &next_vno, &timestamp, err);
        if (GOOD_STATUS(err)) {
	    num_keys++;
            if (key_list = realloc((void *)key_list, sizeof(key_node_t)*num_keys)) {
                key_list[num_keys-1].timestamp = timestamp;
                key_list[num_keys-1].version   = next_vno;
	    } else {
                SET_STATUS(err, sec_s_no_memory);
	    }
        }
    }
    /* Caller doesn't need to know we hit end of list */
    if (STATUS_EQUAL(err, sec_key_mgmt_e_key_unavailable)) {
        CLEAR_STATUS(err);
    }
    sec_key_mgmt_release_cursor(&cursor, &lst);
    if (BAD_STATUS(err) || (num_keys == 0) || (num_keys == 1)) {
        if (key_list != NULL) {
            free(key_list);
        }
        return;
    }
    
    /* Make sure the keys are in reverse chronological order */
    qsort(key_list, num_keys, sizeof(key_node_t), compare_keynodes);
    
    /* Figure out what the oldest ticket for this principal could be, by
     * getting the max certificate lifetime and subtracting from the current
     * time.
     */
    sec_key_mgmt__bind_rgy(principal_name, sec_rgy_bind_auth_none, NULL,
                            account.pname, NULL, false, &rgy_h, err);
    if (BAD_STATUS(err)) {
        return;
    }
    account.gname[0] = account.oname[0] = '\0';

    sec_rgy_auth_plcy_get_effective(rgy_h, &account, &auth_policy, err);
    sec_rgy_site_close(rgy_h, &lst);
    if (BAD_STATUS(err)) {
        free(key_list);
        return;
    }

    /* infinite ticket lifetime implies No GC.  Punt. */
    if (auth_policy.max_ticket_lifetime == 0)
    {
	free(key_list);
	return;
    }
        /* oldest possible ticket = now - max_lifetime */
#ifdef SNI_SVR4_POSIX
    gettimeofday(&tm);
#else
    gettimeofday(&tm, 0);
#endif
    oldest_tkt = tm.tv_sec - auth_policy.max_ticket_lifetime;

    for (i = 0; i < num_keys; i++) {
	/* If we found the oldest valid one, then delete the rest */
        if (oldest_valid_key) {
            sec_key_mgmt_delete_key(authn_service, arg, principal_name,
                                    key_list[i].version, err);
        } else if (key_list[i].timestamp < oldest_tkt) {
	    /*
	     * The first one we find older than the oldest
	     * possible ticket is the oldest valid key.
	     */
            oldest_valid_key = key_list[i].timestamp;
	}
    }
    free(key_list);
}
