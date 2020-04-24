/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: seckey_set.c,v $
 * Revision 1.1.4.2  1996/03/11  13:27:42  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:15:32  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:03:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:31:09  root]
 * 
 * Revision 1.1.2.6  1993/02/08  19:44:03  sommerfeld
 * 	[OT7153] Fix status code conversion.
 * 	[1993/02/08  18:29:58  sommerfeld]
 * 
 * Revision 1.1.2.5  1992/12/29  12:43:17  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:38:11  zeliff]
 * 
 * Revision 1.1.2.4  1992/10/27  14:44:55  burati
 * 	CR5817 Check for EACCESS before setting err to keytable invalid
 * 	[1992/10/26  23:03:50  burati]
 * 
 * Revision 1.1.2.3  1992/10/19  15:32:27  burati
 * 	CR5133 sec_key_mgmt_set_key should check parity of DES keys
 * 	[1992/10/16  22:22:59  burati]
 * 
 * Revision 1.1.2.2  1992/10/14  15:26:42  burati
 * 	CR4615 Also need to check for ENOENT
 * 	[1992/10/13  22:39:39  burati]
 * 
 * 	CR4615 check for KRB5_KT_NOTFOUND explicitly before adding to keytable
 * 	[1992/10/13  21:46:33  burati]
 * 
 * Revision 1.1  1992/01/19  14:48:06  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  seckey_set.c V=4 12/19/91 //littl/prgy/src/client/key_mgmt
**
** Copyright (c) Hewlett-Packard Company 1991,1992
** Unpublished work. All Rights Reserved.
*/
/* 
 *  DCE Security key management interface
 */

#ifdef MODULE_VERSION_ID
static char *VersionID =
    "$Id: seckey_set.c,v 1.1.4.2 1996/03/11 13:27:42 marty Exp $";
#endif

#include <seckey_util.h>
#include <dce/keymgmt.h>
#include <dce/sec_authn.h>
#include <dce/sec_login.h>

#include <krb5/des_int.h>
#include <rgymacro.h>
#include <u_str.h>
#include <un_maloc.h>

/* s e c _ k e y _ m g m t _ s e t _ k e y
 *
 * Perform all local activities necessary to update a principal's key to 
 * the specified value.  This function will not update the authentication
 * service's value for the principal's key.  This function is generally
 * useful for replicated services running on independent machines but
 * sharing a common key.
 *
 *  ** see NOTE on authenticated RPC related arguments above.
 *
 * Errors:
 *  sec_key_mgmt_e_key_unavailable      Old key not present, unable to use
 *                                      it to set a client side authentication
 *                                      context.
 *  sec_key_mgmt_e_authn_invalid        Authentication service invalid.
 *  sec_key_mgmt_e_unauthorized         Caller unauthorized to perform op.
 *  sec_key_mgmt_e_key_unsupported      Key type not supported.
 *  sec_key_mgmt_e_key_version_exists   Key with this version number already
 *                                      exists.
 */
void sec_key_mgmt_set_key
#ifndef __STDC__
    ( authn_service, arg, principal_name, key_vno, keydata, err )
    sec_key_mgmt_authn_service  authn_service; 
    void                        * arg;
    unsigned char               * principal_name;
    unsigned32                  key_vno;
    void                        * keydata;
    error_status_t              * err;
#else
  (
    sec_key_mgmt_authn_service  authn_service, 
    void                        * arg,
    unsigned char               * principal_name,
    unsigned32                  key_vno,
    void                        * keydata,
    error_status_t              * err 
  )
#endif
{
    error_status_t          lst;
    krb5_keytab             ktid;
    krb5_keytab_entry       ktentry;
    krb5_keytab_entry       oldktentry;
    sec_krb_parsed_name     parsed_name;
    sec_passwd_rec_t        *key = (sec_passwd_rec_t *) keydata;
 
    if (!err) {
        err = &lst;
    }

    CLEAR_STATUS(err);

    SEC_KEY_CHECK_AUTHN(authn_service, err);

    ktentry.key.contents = NULL;
    /*
     * Kerberos V5 key version numbers are only 1 byte. Check to make sure
     * the requested key version number is in range.  We do this by only
     * using the low order byte of the key.  If we obtain a 0 (the
     * distinguished "don't care") we return "key unavailable". Eventually,
     * (when we support something other than the kerberos V5 protocol) we might
     * have a use for high order bytes.
     * 
     */
    ktentry.vno = key_vno & 0xFF;
    if (ktentry.vno == 0) {
        SET_STATUS(err, sec_key_mgmt_e_key_unavailable);
        return;
    }

    /* DES keys must pass a parity check */
    if (key->key.key_type == sec_passwd_des) {
        if (!mit_des_check_key_parity(key->key.tagged_union.des_key)) {
            SET_STATUS(err, sec_s_bad_key_parity);
            return;
        }
    }

    SEC_KEY_CHECK_INIT(err);

    if (STATUS_OK(err)) {
        void * old_keydata;

        if (!sec_key_mgmt__resolve(arg, &ktid, err)) {
            return;
        }


        *err = sec_krb_sec_parse_name(sec_login_default_handle,
            rpc_c_authn_level_pkt_integrity, principal_name, &parsed_name);

        if (*err)
            return;
        
        ktentry.principal = (krb5_principal) parsed_name;

        /*
         * Check to make sure the entry doesn't exist already.
         */
        lst = krb5_kt_get_entry(ktid, ktentry.principal, ktentry.vno,
                                        &oldktentry);
	lst = sec_key_mgmt__convert_status(lst);
	
        if (lst == 0) {
            *err = sec_key_mgmt_e_key_version_ex;
            /*
             * We're done with this entry, release it.
             */
            krb5_kt_free_entry(&oldktentry);

        } else if ((lst == KRB5_KT_NOTFOUND) || 
	    (lst == sec_key_mgmt_e_keytab_not_found)) {

            switch(key->key.key_type) {
                case sec_passwd_plain: {
                    krb5_data   pwd;
                    krb5_data   salt;

                    *err = krb5_principal2salt(ktentry.principal, &salt);
                    if (*err) {
                        break;
                    }

                    pwd.length  = u_strlen(PLAIN_PASSWD(key));
                    pwd.data    = (char *) PLAIN_PASSWD(key);

                    *err = krb5_string_to_key(&master_encblock, 
                                master_keyblock.keytype,
                                &ktentry.key,
                                &pwd,
                                &salt);

                    if (*err) {
                        break;
                    }

                    *err = krb5_kt_add_entry(ktid, &ktentry);

                    if (ktentry.key.contents) {
                        free((char *) ktentry.key.contents);
                    }

                    break;
                }

                case sec_passwd_des:
                    ktentry.key.keytype = KEYTYPE_DES;
                    ktentry.key.length  = sec_passwd_c_des_key_size;
                    ktentry.key.contents = DES_PASSWD(key);

                    *err = krb5_kt_add_entry(ktid, &ktentry);
                    ktentry.key.contents = NULL;
                    break;

                case sec_passwd_none:
                default:
                    *err = sec_key_mgmt_e_key_unsupported;

            }
        } else {
            if (lst == EACCES) {
                *err = sec_key_mgmt_e_unauthorized;
            } else {
                /* Any other error means the file isn't even open */
                *err = sec_key_mgmt_e_ktfile_err;
            }
        }

        /*
         * Release storage for parsed name.
         */
        sec_krb_parsed_name_free(&parsed_name);

        /*
         * Release the keytab table.
         */
        if ((*err != sec_key_mgmt_e_ktfile_err) &&
            (*err != sec_key_mgmt_e_unauthorized)) {
            krb5_kt_close(ktid);
        }
    }

    if (*err == EACCES) {
        *err = sec_key_mgmt_e_unauthorized;
    }

}

