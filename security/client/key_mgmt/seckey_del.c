/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: seckey_del.c,v $
 * Revision 1.1.4.2  1996/03/11  13:27:39  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:15:29  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:03:27  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:31:04  root]
 * 
 * Revision 1.1.2.2  1992/12/29  12:43:04  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:13:22  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:48:03  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  seckey_del.c V=4 12/19/91 //littl/prgy/src/client/key_mgmt
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/* 
 *  DCE Security key management interface
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)seckey_del.c	4 - 12/19/91";
#endif

#include <seckey_util.h>
#include <dce/keymgmt.h>
#include <dce/sec_authn.h>
#include <dce/sec_login.h>

#include <rgymacro.h>

#include <sys/errno.h>

/* s e c _ k e y _ m g m t _ d e l e t e _ k e y 
 *
 * Delete the specified key from the local key store.
 *
 * NOTE this function will delete all keys of the given version number
 * it does not take a keytype argument to determine which key should be
 * deleted.  (In practice we only support DES keys, but eventually
 * a new function should be added to allow the specification of the
 * key type)
 *
 * Argument Usage: 
 *      INPUT
 *  authn_service   **  select the underlying authentication scheme
 *  arg             **  key acquisition function argument (same value
 *                      that is passed to rpc_server_register_auth_info.)
 *  principal_name  **  names the principal whose key is to be changed
 *  key_vno             the key version to delete.
 *
 *  ** see NOTE on authenticated RPC related arguments above.
 *
 *      OUTPUT
 *  err                 error status if any.
 * 
 * Any error condition will leave the key state unchanged.
 *
 * Errors:
 *  sec_key_mgmt_e_key_unavailable      Requested key not present
 *  sec_key_mgmt_e_authn_invalid        Authentication service invalid
 *  sec_key_mgmt_e_unauthorized         Caller unauthorized to perform op
 */
void sec_key_mgmt_delete_key
#ifndef __STDC__
    ( authn_service, arg, principal_name, key_vno, err )
    sec_key_mgmt_authn_service  authn_service; 
    void                        * arg;
    unsigned char               * principal_name;
    unsigned32                  key_vno;
    error_status_t              * err;
#else
  (
    sec_key_mgmt_authn_service  authn_service, 
    void                        * arg,
    unsigned char               * principal_name,
    unsigned32                  key_vno,
    error_status_t              * err 
  )
#endif
{
    error_status_t          lst;
    krb5_keytab             ktid;
    krb5_keytab_entry       ktentry;
    sec_krb_parsed_name     parsed_name;

    if (!err) {
        err = &lst;
    }

    CLEAR_STATUS(err);

    SEC_KEY_CHECK_AUTHN(authn_service, err);

    /*
     * Kerberos V5 key version numbers are only 1 byte. Check to make sure
     * the requested key version number is in range.  We do this by only
     * using the low order byte of the key.  If we obtain a 0 (the
     * distinguished "don't care"), we reject the operation since deletes
     * must specify a given key, and cannot use the wildcard value.
     */
    ktentry.vno = key_vno & 0xFF;
    if (ktentry.vno == 0) {
        SET_STATUS(err, sec_key_mgmt_e_key_unavailable);
        return;
    }

    SEC_KEY_CHECK_INIT(err);

    if (STATUS_OK(err)) {
        if (!sec_key_mgmt__resolve(arg, &ktid, err)) {
            return;
        }

        *err = sec_krb_sec_parse_name(sec_login_default_handle,
            rpc_c_authn_level_pkt_integrity, principal_name, &parsed_name);

        if (*err)
            return;
        
        ktentry.principal = (krb5_principal) parsed_name;
        ktentry.key.keytype = KEYTYPE_DES;

        *err = krb5_kt_remove_entry(ktid, &ktentry);

        if (*err == EACCES) {
            *err = sec_key_mgmt_e_unauthorized;
        }

        /*
         * Release storage for parsed name.
         */
        sec_krb_parsed_name_free(&parsed_name);

        /*
         * Release the keytab table.
         */
        krb5_kt_close(ktid);
    }
}


/* s e c _ k e y _ m g m t _ d e l e t e _ k e y _ t y p e
 *
 * Delete the specified key from the local key store.  This function
 * deletes only the specified key type of the specified key version.
 *
 * Argument Usage: 
 *      INPUT
 *  authn_service   **  select the underlying authentication scheme
 *  arg             **  key acquisition function argument (same value
 *                      that is passed to rpc_server_register_auth_info.)
 *  principal_name  **  names the principal whose key is to be changed
 *  keytype         **  the keytype  (ONLY SUPPORTS sec_passwd_des AT 1.0)
 *  key_vno             the key version to delete.
 *
 *  ** see NOTE on authenticated RPC related arguments above.
 *
 *      OUTPUT
 *  err                 error status if any.
 * 
 * Any error condition will leave the key state unchanged.
 *
 * Errors:
 *  sec_key_mgmt_e_key_unavailable      Requested key not present
 *  sec_key_mgmt_e_authn_invalid        Authentication service invalid
 *  sec_key_mgmt_e_unauthorized         Caller unauthorized to perform op
 */
void sec_key_mgmt_delete_key_type
#ifndef __STDC__
    ( authn_service, arg, principal_name, keytype, key_vno, err )
    sec_key_mgmt_authn_service  authn_service; 
    void                        * arg;
    unsigned char               * principal_name;
    void                        * keytype;
    unsigned32                  key_vno;
    error_status_t              * err;
#else
  (
    sec_key_mgmt_authn_service  authn_service, 
    void                        * arg,
    unsigned char               * principal_name,
    void                        * keytype,
    unsigned32                  key_vno,
    error_status_t              * err 
  )
#endif
{
    error_status_t          lst;
    krb5_keytab             ktid;
    krb5_keytab_entry       ktentry;
    sec_krb_parsed_name     parsed_name;

    if (!err) {
        err = &lst;
    }

    CLEAR_STATUS(err);

    SEC_KEY_CHECK_AUTHN(authn_service, err);

    /*
     * DES is the only supported keytype for key tables in DCE 1.0
     */
    if ( (*(sec_passwd_type_t *)keytype) != sec_passwd_des ) {
        SET_STATUS(err, sec_key_mgmt_e_key_unsupported);
        return;
    }

    /*
     * Kerberos V5 key version numbers are only 1 byte. Check to make sure
     * the requested key version number is in range.  We do this by only
     * using the low order byte of the key.  If we obtain a 0 (the
     * distinguished "don't care"), we reject the operation since deletes
     * must specify a given key, and cannot use the wildcard value.
     */
    ktentry.vno = key_vno & 0xFF;
    if (ktentry.vno == 0) {
        SET_STATUS(err, sec_key_mgmt_e_key_unavailable);
        return;
    }

    SEC_KEY_CHECK_INIT(err);

    if (STATUS_OK(err)) {
        if (!sec_key_mgmt__resolve(arg, &ktid, err)) {
            return;
        }

        *err = sec_krb_sec_parse_name(sec_login_default_handle,
            rpc_c_authn_level_pkt_integrity, principal_name, &parsed_name);

        if (*err)
            return;
        
        ktentry.principal = (krb5_principal) parsed_name;
        ktentry.key.keytype = KEYTYPE_DES;

        *err = krb5_kt_remove_entry(ktid, &ktentry);

        if (*err == EACCES) {
            *err = sec_key_mgmt_e_unauthorized;
        }

        /*
         * Release storage for parsed name.
         */
        sec_krb_parsed_name_free(&parsed_name);

        /*
         * Release the keytab table.
         */
        krb5_kt_close(ktid);
    }
}

