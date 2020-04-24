/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: seckey_get.c,v $
 * Revision 1.1.11.1  1996/10/03  14:44:13  arvind
 * 	silence compiler warning about uninit variable.
 * 	[1996/09/16  20:02 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 *
 * Revision 1.1.9.2  1996/03/11  13:27:40  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:15:30  marty]
 * 
 * Revision 1.1.9.1  1995/12/08  18:03:32  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:31:06  root]
 * 
 * Revision 1.1.7.1  1994/08/26  16:01:12  sommerfeld
 * 	Fix for beta4.
 * 	[1994/08/26  15:50:42  sommerfeld]
 * 
 * Revision 1.1.5.2  1992/12/29  12:43:10  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:38:03  zeliff]
 * 
 * Revision 1.1.3.2  1992/05/26  21:29:29  pato
 * 	Restored blown away des_neuter branch
 * 	[1992/05/26  21:19:20  pato]
 * 
 * Revision 1.1.1.4  1992/05/22  20:47:43  pato
 * 	Fix ID string in module identification field
 * 
 * Revision 1.1.1.3  1992/05/22  20:02:35  pato
 * 	OT#3770 Keytab file only needs read access for query functions
 * 	the previous change did not really address OT 3620.
 * 
 * Revision 1.1.1.2  1992/05/22  14:00:26  pato
 * 	OT#3620 default RPC key retrieval should not need keytab type prefix.
 * 
 * Revision 1.1  1992/01/19  14:48:04  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
** Copyright (c) Hewlett-Packard Company 1991, 1992
** Unpublished work. All Rights Reserved.
*/

/* 
 *  DCE Security key management interface
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = 
"$Id: seckey_get.c,v 1.1.11.1 1996/10/03 14:44:13 arvind Exp $";
#endif

#include <seckey_util.h>
#include <dce/keymgmt.h>
#include <dce/sec_authn.h>

#include <rgymacro.h>
#include <un_strng.h>
#include <un_maloc.h>
#include <u_str.h>

#include <sys/errno.h>

#ifndef PRIVATE
#   define PRIVATE static
#endif

PRIVATE void sec_key_mgmt_get_latest_key
#ifndef __STDC__
    ( authn_service, arg, principal_name, keytype, key_vno, keydata, err )
    sec_key_mgmt_authn_service  authn_service; 
    void                        * arg;
    unsigned char               * principal_name;
    sec_passwd_type_t           keytype;
    unsigned32                  * key_vno;
    sec_passwd_rec_t            * keydata;
    error_status_t              * err;
#else
  (
    sec_key_mgmt_authn_service  authn_service, 
    void                        * arg,
    unsigned char               * principal_name,
    sec_passwd_type_t           keytype,
    unsigned32                  * key_vno,
    sec_passwd_rec_t            * keydata,
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
    boolean32               found_one = false;
    krb5_timestamp          ts = 0;

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
        while (*err == 0) {
            *err = krb5_kt_next_entry(ktid, &ktentry, &cursor);
            if (*err) {
                if (*err == KRB5_KT_END) {
                    if (found_one) {
                        *err = 0;
                    } else {
                        *err = sec_key_mgmt_e_key_unavailable;
                    }    	
                }
                break;
            }

            /*
             * Check to see if the key is for the desired principal
             */
            if (principal_name) {
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
                && ((gprinc == NULL)
                  	|| u_strcmp(gprinc, global_name) == 0) 
                && (!found_one || (found_one && ktentry.timestamp > ts)) ) {

                found_one = true;
                ts = ktentry.timestamp;

                keydata->version_number = ktentry.vno;
                keydata->pepper         = NULL;
                keydata->key.key_type   = sec_passwd_des; 
                memcpy(DES_PASSWD(keydata), ktentry.key.contents, 
                                sec_passwd_c_des_key_size);

                *key_vno = ktentry.vno;
            }
            if (global_name) {
                    free(global_name);
            }
            krb5_kt_free_entry(&ktentry);
        }

        /*
         * Release the sequential cursor
         */
        lst = krb5_kt_end_seq_get(ktid, &cursor);
        if (lst && !*err) {
            *err = lst;
        }
    }

    if (*err == EACCES) {
        *err = sec_key_mgmt_e_unauthorized;
    }

    if (gprinc) {
        free(gprinc);
    }

    /*
     * Release the keytab table.
     */
    krb5_kt_close(ktid);
 
}


/* s e c _ k e y _ m g m t _ g e t _ k e y _ t y p e
 *
 * Extract the specified key from the local key store.
 *
 * Argument Usage: 
 *      INPUT
 *  authn_service   **  select the underlying authentication scheme
 *  arg             **  key acquisition function argument (same value
 *                      that is passed to rpc_server_register_auth_info.)
 *  principal_name  **  names the principal whose key is to be changed
 *  keytype             the key type desired.
 *  key_vno             the key version to extract.
 *
 *  ** see NOTE on authenticated RPC related arguments above.
 *
 *      OUTPUT
 *  keydata         **  the key value extracted.  This data may be released
 *                      by calling sec_key_mgmt_free_key.
 *                      In DCE 1.0 this parameter is an allocated array of
 *                      sec_passwd_rec_t.  The last entry in this array
 *                      is of type sec_passwd_none.  (DCE 1.0 only 
 *                      supports one encryption type, so this array will
 *                      always be of size 2)
 *  err                 error status if any.
 * 
 * Any error condition will leave the key state unchanged.
 *
 * Errors:
 *  sec_key_mgmt_e_key_unavailable      Requested key not present
 *  sec_key_mgmt_e_authn_invalid        Authentication service invalid
 *  sec_key_mgmt_e_unauthorized         Caller unauthorized to perform op
 */
void sec_key_mgmt_get_key_type
#ifndef __STDC__
    ( authn_service, arg, principal_name, keytype, key_vno, keydata, err )
    sec_key_mgmt_authn_service  authn_service; 
    void                        * arg;
    unsigned char               * principal_name;
    sec_passwd_type_t           keytype;
    unsigned32                  key_vno;
    void                        ** keydata;
    error_status_t              * err;
#else
  (
    sec_key_mgmt_authn_service  authn_service, 
    void                        * arg,
    unsigned char               * principal_name,
    sec_passwd_type_t           keytype,
    unsigned32                  key_vno,
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

    if (keydata) {
        *keydata = NULL;
    } else {
        /*
         * Caller didn't pass a valid output parameter - 
         * return with an error status. (This should not be a 
         * sec_rgy error status!)
         */
        SET_STATUS(err, sec_rgy_bad_data);
    }

    SEC_KEY_CHECK_AUTHN(authn_service, err);

    /*
     * Kerberos V5 key version numbers are only 1 byte. Check to make sure
     * the requested key version number is in range.  We do this by only
     * using the low order byte of the key.  If we obtain a 0 (the
     * distinguished "get best"), we check to make sure the original
     * request specified 0. 
     */
    if (key_vno != 0 && (key_vno & 0xFF) == 0) {
        SET_STATUS(err, sec_key_mgmt_e_key_unavailable);
        return;
    }
    key_vno = key_vno & 0xFF;

    SEC_KEY_CHECK_INIT(err);

    if (STATUS_OK(err)) {
        keys = (sec_passwd_rec_t *) malloc(2 * sizeof(*keys));
        if (keys) {
            if (key_vno == 0) {
                sec_key_mgmt_get_latest_key(authn_service, arg, principal_name,
                                                keytype, &key_vno,
                                                keys, err);
            } else {
                sec_key_mgmt_get_nth_key_internal(authn_service, arg, principal_name,
                                                0, keytype, &key_vno,
                                                keys, NULL, err);
            }
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

/* s e c _ k e y _ m g m t _ g e t _ k e y 
 *
 * Extract the specified key from the local key store.
 *
 * Argument Usage: 
 *      INPUT
 *  authn_service   **  select the underlying authentication scheme
 *  arg             **  key acquisition function argument (same value
 *                      that is passed to rpc_server_register_auth_info.)
 *  principal_name  **  names the principal whose key is to be changed
 *  key_vno             the key version to extract.
 *
 *  ** see NOTE on authenticated RPC related arguments above.
 *
 *      OUTPUT
 *  keydata         **  the key value extracted.  This data may be released
 *                      by calling sec_key_mgmt_free_key.
 *                      In DCE 1.0 this parameter is an allocated array of
 *                      sec_passwd_rec_t.  The last entry in this array
 *                      is of type sec_passwd_none.  (DCE 1.0 only 
 *                      supports one encryption type, so this array will
 *                      always be of size 2)
 *  err                 error status if any.
 * 
 * Any error condition will leave the key state unchanged.
 *
 * Errors:
 *  sec_key_mgmt_e_key_unavailable      Requested key not present
 *  sec_key_mgmt_e_authn_invalid        Authentication service invalid
 *  sec_key_mgmt_e_unauthorized         Caller unauthorized to perform op
 */
void sec_key_mgmt_get_key
#ifndef __STDC__
    ( authn_service, arg, principal_name, key_vno, keydata, err )
    sec_key_mgmt_authn_service  authn_service; 
    void                        * arg;
    unsigned char               * principal_name;
    unsigned32                  key_vno;
    void                        ** keydata;
    error_status_t              * err;
#else
  (
    sec_key_mgmt_authn_service  authn_service, 
    void                        * arg,
    unsigned char               * principal_name,
    unsigned32                  key_vno,
    void                        ** keydata,
    error_status_t              * err 
  )
#endif
{
    sec_key_mgmt_get_key_type(authn_service, arg, principal_name, 
                            sec_passwd_none, key_vno, keydata, err);
}
