/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_mkey.c,v $
 * Revision 1.1.15.1  1996/08/09  12:09:32  arvind
 * 	Merge Registry support for KDC private key storage
 * 	[1996/06/07  19:06 UTC  aha  /main/aha_pk6/1]
 *
 * 	Changes for Public Key Login; support Registry storage of KDC private key
 * 	[1994/08/25  20:14:23  sekhar  1.1.7.2]
 *
 * Revision 1.1.10.2  1996/02/18  00:18:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:07:15  marty]
 * 
 * Revision 1.1.10.1  1995/12/08  17:53:15  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:20:53  root]
 * 
 * Revision 1.1.7.2  1994/08/25  20:14:23  sekhar
 * 	Put back SVC messages for fatal_st() and error_st() calls.
 * 	[1994/08/25  20:12:49  sekhar]
 * 
 * Revision 1.1.7.1  1994/08/04  16:13:40  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:11:43  mdf]
 * 
 * Revision 1.1.3.6  1993/02/26  23:52:03  emartin
 * 	em_bl6f: rs_mkey_change - rsdb_checkpt takes next_rep_state arg
 * 	[1993/02/11  20:24:05  emartin]
 * 
 * Revision 1.1.3.5  1993/02/01  20:36:38  hinman
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  16:15:16  hinman]
 * 
 * Revision 1.1.3.4  1992/12/29  16:35:56  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:24:43  zeliff]
 * 	Revision 1.1.5.2  1993/01/11  16:12:49  hinman
 * 	[hinman] - Check in merged SNI version
 * 
 * Revision 1.1.3.3  1992/11/20  15:52:55  ahop
 * 	Add rs_mkey_store_in_db; replace file i/o with rsdb_mkey calls.
 * 	[1992/11/10  23:11:11  ahop]
 * 
 * Revision 1.1.3.2  1992/10/07  20:34:32  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:17:38  sekhar]
 * 
 * Revision 1.1.1.2  1992/10/06  17:20:03  burati
 * 	Second replication drop: bl5
 * 
 * $EndLog$
 */

/* 
 * Copyright (c) Hewlett-Packard Company 1992
 * Unpublished work. All Rights Reserved.
 */
/* 
 *      Registry Server - master_key routines
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = 
    "$Id: rs_mkey.c,v 1.1.15.1 1996/08/09 12:09:32 arvind Exp $";
#endif

#include <stdio.h>

#include <dce/rgynbase.h>

#ifdef SNI_SVR4
#include <unistd.h>
#endif   /* SNI_SVR4*/
#include <sys/types.h>
#include <sys/file.h>
#include <un_maloc.h>
#include <un_strng.h>
#include <un_time.h>
#include <bstring.h>

#include <dce/dce.h>
#include <rs_pvt.h>
#include <rs_state.h>
#include <rs_util.h>
#include <rs_rsdb.h>
#include <rs_mkey.h>
#include <rs_ch_lock.h>

#include <rsdb.h>
#include <rsdb_pvt.h>
#include <rsdb_auth.h>
#include <rsdb_acct.h>
#include <rsdb_util.h>
#include <rsdb_name_util.h>
#include <rsdb_policy.h>

#include <krb5/krb5.h>
#include <krb5/kdb.h>
#include <krb5/los-proto.h>

#include <sec_svc.h>

PRIVATE char *mkeyseed_prompt = "Enter keyseed for initial database master key: ";

PRIVATE krb5_keyblock       *master_keyblock_p;
PRIVATE krb5_principal      master_princ;
PRIVATE krb5_encrypt_block  master_encblock;
PRIVATE krb5_kvno           master_key_version;
PRIVATE rsdb_realm_info     rblock = { 0 };
PRIVATE krb5_keyblock       *new_master_keyblock_p;
PRIVATE krb5_encrypt_block  new_master_encblock;
PRIVATE krb5_kvno           new_master_key_version;
PRIVATE rsdb_realm_info     new_rblock = { 0 };



/*
 *  r s _ m k e y _ g e n _ i n i t _ k e y s e e d 
 *
 *  Use a user-entered "keyseed" and timeofday to generate 
 *  a DES key (used to seed the random key generator).
 *  Vendors should replace this with their favorite 
 *  random keyseed generator.
 *  If keyseed is a null string, the user is prompted for input.
 *  If keyseed is not null, it is memset to 0 after use.
 *
 *  mkey->keytype should be set to the desired key type.
 *
 *  Lock Notes:
 *  No lock required.
 */
PRIVATE void rs_mkey_gen_init_keyseed
#ifndef __STDC__
    ( mname, eblock, keyseed, mkey, st_p )
    krb5_principal      mname;              /* [in] */
    krb5_encrypt_block  *eblock;            /* [in] */
    char                *keyseed;           /* [in] */
    krb5_keyblock       *mkey;              /* [in, out] */
    error_status_t      *st_p;              /* [out] */
#else
    (
    krb5_principal      mname,
    krb5_encrypt_block  *eblock,
    char                *keyseed,
    krb5_keyblock       *mkey,
    error_status_t      *st_p
    )
#endif
{
    krb5_data           scratch;
    krb5_data           pwd;
    char                password[BUFSIZ]; 
    int                 plen;
    char                *p;
    struct timezone     tz;
    sec_timeval_t       curr_time;
    char                time_string[20];
    long                time_strlen = sizeof(time_string);
         
    CLEAR_STATUS(st_p);              

#ifdef SNI_SVR4_POSIX
    gettimeofday((struct timeval *) &curr_time);
#else
    gettimeofday((struct timeval *) &curr_time, &tz);
#endif /* SNI_SVR4_POSIX */
    sprintf(time_string, "%d", curr_time.usec);
    time_strlen = u_strlen(time_string);

    if(keyseed != NULL && (plen = u_strlen(keyseed)) != 0) {
        if(plen > (BUFSIZ - 1)) {
            dce_svc_printf(SEC_RS_MKEY_LONG_KEYSEED_MSG);
            SET_STATUS(st_p, sec_rgy_bad_data);
            return;
        }
        u_strcpy(password, keyseed);
        memset(keyseed, 0, plen);
    } else {
        /* plen must go in as size of password buffer;
         * it returns as length of string in password.
         */
        plen = sizeof(password);
        if (*st_p = krb5_read_password(mkeyseed_prompt,
                                   0, password, &plen))
            return;

    }
    if( plen > (BUFSIZ - 1) - time_strlen) {
        /* Make room for concatenating the time string */
        plen = (BUFSIZ - 1) - time_strlen;
        password[plen] = '\0';
    }
    u_strcat(password, time_string);
    plen += time_strlen;

    pwd.data = password;
    pwd.length = plen;
    if((*st_p = krb5_principal2salt(mname, &scratch)))
        return;
    *st_p = krb5_string_to_key(eblock, mkey->keytype, mkey, &pwd, &scratch);
    memset(scratch.data, 0, scratch.length);
    xfree(scratch.data);
    memset(password, 0, sizeof(password)); /* erase it */

}  



/*
 *  r s _ m k e y _ g e n _ n e w 
 *
 *  Use the master_key represented in old_rblock 
 *  to generate a new master_key and master_encblock.
 *  Caller is responsible for cleaning up (memset 0 and free)
 *  new_master variables when they're no longer needed.
 *
 *  Lock Notes:
 *  No lock required.
 */
PRIVATE void rs_mkey_gen_new
#ifndef __STDC__
    ( old_rblock, new_mkeyblock, new_master_eblock, new_rblock, st_p )
    rsdb_realm_info         *old_rblock;        /* [in] */
    krb5_keyblock           **new_mkeyblock;    /* [out] */
    krb5_encrypt_block      *new_master_eblock; /* [out] */
    rsdb_realm_info         *new_rblock;        /* [out] */
    error_status_t          *st_p;              /* [out] */
#else
    (
    rsdb_realm_info         *old_rblock,
    krb5_keyblock           **new_mkeyblock,
    krb5_encrypt_block      *new_master_eblock,
    rsdb_realm_info         *new_rblock,
    error_status_t          *st_p
    )
#endif
{
    CLEAR_STATUS(st_p);

    /* Use the old master key to generate a new random master key. 
     * new_mkeyblock and new_mkeyblock->contents are allocated here.
     */
    if (*st_p = krb5_random_key(old_rblock->eblock, old_rblock->rseed, 
                                new_mkeyblock))
        return;

    /* new_master_eblock->crypto_entry must be set before calling 
     * krb5_process_key.
     */
    krb5_use_cstype(new_master_eblock, RSDB_AUTH_DEFAULT_ENCTYPE);

    /* Perform key preprocessing (compute key schedules for DES).
     * new_master_eblock->priv is allocated here.
     */
    if (*st_p = krb5_process_key(new_master_eblock, *new_mkeyblock))
        return;

    new_rblock->eblock = new_master_eblock;

    /* Initialize the random key generator in new_master_eblock
     * using new_mkeyblock 
     */
    *st_p = krb5_init_random_key(new_master_eblock, *new_mkeyblock,
                                 &(new_rblock->rseed));

    return;
}



/*
 *  r s _ m k e y _ e n c r y p t _ k e y
 *
 *  This code copied from kdb/encrypt_key.c.
 *
 *  Encrypt a key for storage in the database.  "eblock" is used
 *  to encrypt the key in "in" into "out"; the storage pointed to by "out"
 *  is allocated before use.
 * 
 *  Lock Notes:
 *  No lock required.
 */
PRIVATE void rs_mkey_encrypt_key
#ifndef __STDC__
    ( eblock, in, out, st_p )
    krb5_encrypt_block          *eblock;
    krb5_keyblock               *in;
    krb5_encrypted_keyblock     *out;
    error_status_t              *st_p;
#else
    (
    krb5_encrypt_block          *eblock,
    krb5_keyblock               *in,
    krb5_encrypted_keyblock     *out,
    error_status_t              *st_p
    )
#endif
{
    /* encrypted rep has the real (unencrypted) key length stored
       along with the encrypted key */

    krb5_keyblock               tmpin;

    CLEAR_STATUS(st_p);

    out->keytype = in->keytype;
    out->length = krb5_encrypt_size(in->length, eblock->crypto_entry);

    /* because of checksum space requirements imposed by the encryption
       interface, we need to copy the input key into a larger area. */
    tmpin.length = in->length;
    tmpin.contents = (krb5_octet *)malloc(out->length);
    if (!tmpin.contents) {
        out->length = 0;
        SET_STATUS(st_p, sec_rgy_cant_allocate_memory);
        return;
    }
    memcpy((char *)tmpin.contents, (const char *)in->contents, tmpin.length);

    out->length += sizeof(out->length);
    out->contents = (krb5_octet *)malloc(out->length);
    if (!out->contents) {
        free((char *)tmpin.contents);
        out->contents = 0;
        out->length = 0;
        SET_STATUS(st_p, sec_rgy_cant_allocate_memory);
        return;
    }
    /* copy in real length */
    memcpy((char *)out->contents, (const char *)&tmpin.length,
           sizeof(out->length));
    /* and arrange for encrypted key */
    *st_p = krb5_encrypt((krb5_pointer) tmpin.contents,
                          (krb5_pointer) (((char *) out->contents) +
                                          sizeof(out->length)),
                          tmpin.length, eblock, 0);
    free((char *)tmpin.contents);
    if (BAD_STATUS(st_p)) {
        free((char *)out->contents);
        out->contents = 0;
        out->length = 0;
    }
    return;
}



/*
 *  r s _ m k e y _ d e c r y p t _ k e y
 *
 *  This code copied from kdb/decrypt_key.c.
 *
 *  Decrypt a key from storage in the database.  "eblock" is used
 *  to decrypt the key in "in" into "out"; the storage pointed to by "out"
 *  is allocated before use.
 * 
 *  Lock Notes:
 *  No lock required.
 */
PRIVATE void rs_mkey_decrypt_key
#ifndef __STDC__
    ( eblock, in, out, st_p )
    krb5_encrypt_block          *eblock;
    krb5_encrypted_keyblock     *in;
    krb5_keyblock               *out;
    error_status_t              *st_p;
#else
    (
    krb5_encrypt_block          *eblock,
    krb5_encrypted_keyblock     *in,
    krb5_keyblock               *out,
    error_status_t              *st_p
    )
#endif
{
    CLEAR_STATUS(st_p);
    /* the encrypted version is stored as the unencrypted key length
       (in host byte order), followed by the encrypted key.
     */
    out->keytype = in->keytype;
    out->length = krb5_encrypt_size(in->length-sizeof(in->length),
                                    eblock->crypto_entry);
    out->contents = (krb5_octet *)malloc(out->length);
    if (!out->contents) {
        out->contents = 0;
        out->length = 0;
        SET_STATUS(st_p, sec_rgy_cant_allocate_memory);
        return;
    }
    /* copy out the real length count */
    memcpy((char *)&out->length, (char *)in->contents, sizeof(out->length));

    /* remember the contents of the encrypted version has a sizeof(in->length)
       integer length of the real embedded key, followed by the
       encrypted key, so the offset here is needed */
    if (*st_p = krb5_decrypt((krb5_pointer) (((char *) in->contents) +
                                              sizeof(in->length)),
                              (krb5_pointer) out->contents,
                              in->length-sizeof(in->length), eblock, 0)) {
        free((char *)out->contents);
        out->contents = 0;
        out->length = 0;
        return;
    }
    if (out->length < 0) {
        free((char *)out->contents);
        out->contents = 0;
        out->length = 0;
        SET_STATUS(st_p, sec_rgy_key_bad_size);
        return;
    }
    return;
}



/*
 *  r s _ m k e y _ f e t c h
 *
 *  This code derived from kdb/fetch_mkey.c.
 *  Caller is responsible for releasing key->contents storage.
 *  key->keytype should be set to the desired key type.
 * 
 *  Lock Notes:
 *  A database (lock_db) read lock is required for this operation
 *  so that the master_key doesn't change during the operation.
 */
PRIVATE void rs_mkey_fetch
#ifndef __STDC__
    ( keyfile, key, key_version, st_p )
    char                        *keyfile;       /* [in] */
    krb5_keyblock               *key;           /* [out] */
    krb5_kvno                   *key_version;   /* [out] */
    error_status_t              *st_p;          /* [out] */
#else
    (
    char                        *keyfile,
    krb5_keyblock               *key,
    krb5_kvno                   *key_version,
    error_status_t              *st_p
    )
#endif
{
    rsdb_auth_key_t     rsdb_key;

    CLEAR_STATUS(st_p);
    /* use bad keytype to detect file read error */
    rsdb_key.keytype = -1;

    rsdb_mkey_read(keyfile, &rsdb_key, st_p);

    if(rsdb_key.keytype != key->keytype) {
        SET_STATUS(st_p, sec_rgy_mkey_bad_stored);
    }
    if(GOOD_STATUS(st_p)) {
        key->length = rsdb_key.length;
        if (!(key->contents = (krb5_octet *)malloc(key->length))) {
            SET_STATUS(st_p, sec_rgy_cant_allocate_memory);
        }
    }
    if(GOOD_STATUS(st_p)) {
        bcopy((char *)rsdb_key.contents, (char *)key->contents, key->length);
       *key_version = rsdb_key.key_version;
    }
    memset(&rsdb_key, 0, sizeof(rsdb_key));
    return;
}



/*
 *  r s _ m k e y _ s t o r e _ o n _ d i s k
 *
 *  Store the database master_key in a file read/write-able only by root.
 *  key->keytype should be set to the desired key type.
 *  
 *  Lock Notes:
 *  This operation must be invoked with the global write lock.
 */
PRIVATE void rs_mkey_store_on_disk
#ifndef __STDC__
    ( keyfile, key, key_version, st_p )
    char                        *keyfile;       /* [in] */
    krb5_keyblock               *key;           /* [in] */
    krb5_kvno                   key_version;    /* [in] */
    error_status_t              *st_p;          /* [out] */
#else
    (
    char                        *keyfile,
    krb5_keyblock               *key,
    krb5_kvno                   key_version,
    error_status_t              *st_p
    )
#endif
{
    rsdb_auth_key_t     rsdb_key;

    CLEAR_STATUS(st_p);

    rsdb_key.key_version = key_version;
    rsdb_key.keytype     = key->keytype;
    if(key->length > sizeof(rsdb_key.contents))
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_mkey,
                       svc_c_sev_fatal | svc_c_action_abort,
                       sec_buf_too_small);
    rsdb_key.length      = key->length;
    bcopy((char *)key->contents, (char *)rsdb_key.contents, rsdb_key.length);

    rsdb_mkey_create(keyfile, &rsdb_key, st_p);
    if(BAD_STATUS(st_p)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_mkey,
                       svc_c_sev_fatal | svc_c_action_abort,
                       *st_p);
    }
}



/*
 *  r s _ m k e y _ e n c r y p t _ s t o r e _ i n _ d b
 *
 *  Encrypt the mkey under itself (eblock must correspond to mkey)
 *  and store it as the new encrypted master_key in the database.
 *  
 *  Lock Notes:
 *  This operation must be invoked with the global write lock.
 */
PRIVATE void rs_mkey_encrypt_store_in_db
#ifndef __STDC__
    ( mkey,  mkey_version, encblock, st_p )
    krb5_keyblock       *mkey;
    krb5_kvno           mkey_version;
    krb5_encrypt_block  *encblock;
    error_status_t      *st_p;
#else
  (
    krb5_keyblock       *mkey,
    krb5_kvno           mkey_version,
    krb5_encrypt_block  *encblock,
    error_status_t      *st_p
  )
#endif
{
    krb5_encrypted_keyblock ekey;
    rsdb_encrypt_key_t      encrypted_mkey;

    CLEAR_STATUS(st_p);
    ekey.contents = 0;
    encrypted_mkey.length = 0;

    /* Encrypt mkey and store in database 
     */
    rs_mkey_encrypt_key(encblock, mkey, &ekey, st_p);
    if (GOOD_STATUS(st_p)) {
        if(ekey.length > rsdb_auth_encrypt_key_max_len) {
            SET_STATUS(st_p, sec_rgy_key_bad_size);
        } else {
            encrypted_mkey.mkey_version = mkey_version;
            encrypted_mkey.keytype = ekey.keytype;
            encrypted_mkey.length = ekey.length;
            bcopy((char *)(ekey.contents), (char *)(encrypted_mkey.contents),
                  ekey.length);
            rsdb_store_new_enc_master_key(&encrypted_mkey, st_p);
        }
    }
    if(ekey.contents) {
        memset(ekey.contents, 0, ekey.length);
        free((char *)ekey.contents);
    }
    if(encrypted_mkey.length > 0) {
        memset(encrypted_mkey.contents, 0, encrypted_mkey.length);
    }
}



/*
 *  r s _ m k e y _ s a v e
 *
 *  Rename the mkey_file to old_mkey_file.
 *  Store the new master key in the mkey_file.
 *  Encrypt the new master key and store 
 *  encrypted_mkey in the database 
 *
 *  Lock Notes:
 *  rs_mkey_save should be invoked with the global write lock.
 */
PRIVATE void rs_mkey_save
#ifndef __STDC__
    ( new_mkeyblock, new_master_eblock, new_mkey_version, st_p )
    krb5_keyblock           *new_mkeyblock;     /* [in] */
    krb5_encrypt_block      *new_master_eblock; /* [in] */
    krb5_kvno               *new_mkey_version;  /* [in] */
    error_status_t          *st_p;              /* [out] */
#else
    (
    krb5_keyblock           *new_mkeyblock,
    krb5_encrypt_block      *new_master_eblock,
    krb5_kvno               *new_mkey_version,
    error_status_t          *st_p
    )
#endif
{
    sec_rgy_name_t          old_master_key_file;

    /* Do some initialization and setup */
    CLEAR_STATUS(st_p);  

    u_strcpy( old_master_key_file, MASTER_KEY_FILE );
    u_strcat( old_master_key_file, ".old" );

    /* Rename the current master_key file to <file>.old */
    rsdb_mkey_rename(MASTER_KEY_FILE, (char *)old_master_key_file, st_p);
    if(BAD_STATUS(st_p)) {
        return;
    }

    /* Store the new master_key in the master_key
     * file read&write-able only by root
     */
    rsdb_get_new_master_key_version(new_mkey_version, st_p);
    rs_mkey_store_on_disk(MASTER_KEY_FILE, new_mkeyblock, 
                          *new_mkey_version, st_p );
    
    if (GOOD_STATUS(st_p)) {
        /* Encrypt new master_key (under the new master_key)
         * for storage in the database.
         */
        rs_mkey_encrypt_store_in_db(new_mkeyblock, 
                                    *new_mkey_version,
                                    new_master_eblock,
                                    st_p);
    }
}

           

/*
 *  r s _ m k e y _ r e e n c r y p t _ a c c t _ k e y
 *
 *  Retrieve an account using cursor.
 *  If the account.mkey_version doesn't match old_mkey_version
 *  or new_mkey_version, database is in a bad state; return error.
 *  If the account.mkey_version matches new_mkey_version,
 *  re-encryption is not necessary, so just return success.
 *  Otherwise, decrypt the account's key(s) using old_eblock.
 *  Re-encrypt the key(s) using new_eblock.
 *  Store the new master key version number.
 *  Replace the account and its keys to the database.
 *
 *  Lock Notes:
 *  A database (lock_db) read lock is required for this operation
 *  so that the master_key doesn't change during the operation.
 * 
 * Do *not* purge expired versions of multi-versioned keys
 * here.  That can only be done during an account replace
 * (the master's current time at the point of garbage collection 
 * must be propagated to slaves in order to maintain key version
 * consistency among all replicas)
  */
PRIVATE void rs_mkey_reencrypt_acct_key
#ifndef __STDC__
    ( cursor, old_eblock, old_version, new_eblock, new_version, st_p )
    sec_rgy_cursor_t            *(cursor);      /* [in, out] */
    krb5_encrypt_block          *old_eblock;    /* [in] */
    krb5_kvno                   old_version;    /* [in] */
    krb5_encrypt_block          *new_eblock;    /* [in] */
    krb5_kvno                   new_version;    /* [in] */
    error_status_t              *st_p;          /* [out] */
#else
    (
    sec_rgy_cursor_t            *(cursor),      /* [in, out] */
    krb5_encrypt_block          *old_eblock,
    krb5_kvno                   old_version,
    krb5_encrypt_block          *new_eblock,
    krb5_kvno                   new_version,
    error_status_t              *st_p
    )
#endif
{
    sec_rgy_login_name_t        login_name;
    sec_rgy_acct_key_t          name_parts;
    rsdb_named_item_t           person;
    rsdb_named_item_t           group;
    rsdb_named_item_t           org;
    rsdb_acct_item_t            account;
    krb5_encrypted_keyblock     ekey;
    krb5_keyblock               key;
    long                        key_len;
    rsdb_authkey_item_t         auth_item;
    krb5_kvno                   next_version;
    krb5_keytype                key_type = RSDB_AUTH_DEFAULT_KEY_TYPE;

    CLEAR_STATUS(st_p);
    login_name.pname[0] = '\0';
    login_name.gname[0] = '\0';
    login_name.oname[0] = '\0';
    key.contents = NULL;
    ekey.contents = NULL;

    if(!rsdb_acct_lookup_acct_id( &login_name, cursor, &name_parts,
                                 &person, &group, &org, &account, st_p )) {
        return;
    }
    /* Check the account's master_key version. */
    if(account.mkey_version != old_version) {
        if(account.mkey_version != new_version) {
            dce_svc_printf(SEC_RS_MKEY_ACTVER_MISMATCH_MSG);
        }
        /* Account's key already encrypted under new_master_key.
         * Return with success.
         */
        return;
    }

    /* Re-encrypt the current key in account and store it */
    if(!(ekey.contents = (krb5_octet *)malloc(rsdb_auth_encrypt_key_max_len))) {
        SET_STATUS(st_p, sec_rgy_cant_allocate_memory);
        return;
    }    
    /* From this point on, don't return without executing the cleanup code */
    ekey.keytype = key_type;
    ekey.length  = account.key_len; 
    bcopy(ACCT_KEY(&account), ekey.contents, ekey.length);
    rs_mkey_decrypt_key(old_eblock, &ekey, &key, st_p);
    if(GOOD_STATUS(st_p)) {
        /* ekey.contents gets allocated again in rs_mkey_encrypt_key */
        free((char *)ekey.contents);
        ekey.contents = NULL;
        rs_mkey_encrypt_key(new_eblock, &key, &ekey, st_p);
        if (GOOD_STATUS(st_p)) {
            account.key_len = ekey.length;
            bcopy(ekey.contents, ACCT_KEY(&account), account.key_len);
        }
    }

    /* If this account supports multiple key versions,
     * re-encrypt all versions.
     */
    if ( GOOD_STATUS(st_p)) {

        if (MULT_KEY_VERS_OK(&account) && account.next_key_version != sec_passwd_c_version_none ) {
            next_version = account.next_key_version;
        } else if (PK_AUTHKEY_EXISTS(&account)) {
            next_version = 1;
            key_type = KEYTYPE_ENCRYPT_RSA_PRIV;
        } else {
            next_version = sec_passwd_c_version_none;
        }

        while( GOOD_STATUS(st_p) &&
               next_version != sec_passwd_c_version_none ) {
            if(!rsdb_auth_get_by_authkey_key(next_version, key_type,
                account.threads.id, &auth_item, st_p)) {
                break;
            }
            /* ekey should still be initialized correctly, but just in case */
            ekey.keytype = key_type;
            ekey.length  = auth_item.key_len;
            bcopy( &(auth_item.buf[auth_item.pepper_len]), ekey.contents,
                   ekey.length);
            /* key.contents gets allocated again in rsdb_auth_decrypt_key */
            free((char *)key.contents);
            key.contents = NULL;
            rs_mkey_decrypt_key(old_eblock, &ekey, &key, st_p);
            if(BAD_STATUS(st_p)) {
                break;
            }
            /* ekey.contents gets allocated again in rsdb_auth_encrypt_key */
            free((char *)ekey.contents);
            ekey.contents = NULL;
            rs_mkey_encrypt_key(new_eblock, &key, &ekey, st_p);
            if (BAD_STATUS(st_p)) {
                break;
            }
            auth_item.key_len = ekey.length;
            bcopy(ekey.contents, &(auth_item.buf[auth_item.pepper_len]),
                  ekey.length);
            rsdb_auth_store_by_authkey_key(next_version, key_type,
                account.threads.id, &auth_item, st_p);
            next_version = auth_item.next_version;
            if ((auth_item.next_version == sec_passwd_c_version_none) &&
                (key_type == RSDB_AUTH_DEFAULT_KEY_TYPE) &&
                PK_AUTHKEY_EXISTS(&account)) {
                next_version = 1;
                key_type = KEYTYPE_ENCRYPT_RSA_PRIV;
            }
        }
    }

    if(GOOD_STATUS(st_p)) {
        account.mkey_version = new_version;
        store_acct_by_id(account.threads.id, &account);
    }
     
    /* Cleanup */
    if(key.contents) {
        memset((char *)(key.contents), 0, key.length);
        free(key.contents);
    }
    if(ekey.contents) {
        memset((char *)(ekey.contents), 0, ekey.length);
        free(ekey.contents);
    }
}



/*
 *  r s _ m k e y _ r e e n c r y p t _ d b _ k e y s
 *
 *  For each acct record in the database, call
 *  rs_mkey_reencrypt_acct_key with the input
 *  new and old master_key info.
 *
 *  Lock Notes:
 *  This operation should be called with the GLOBAL_RIW lock.
 *  This lock will be promoted and demoted during operation execution.
 */
PRIVATE void rs_mkey_reencrypt_db_keys
#ifndef __STDC__
    ( old_eblock, old_version, new_eblock, new_version, st )
    krb5_encrypt_block          *old_eblock;    /* [in] */
    krb5_kvno                   old_version;    /* [in] */
    krb5_encrypt_block          *new_eblock;    /* [in] */
    krb5_kvno                   new_version;    /* [in] */
    error_status_t              *(st);
#else
    (
    krb5_encrypt_block          *old_eblock,
    krb5_kvno                   old_version,
    krb5_encrypt_block          *new_eblock,
    krb5_kvno                   new_version,
    error_status_t              *(st)
    )
#endif
{
    unsigned32              i;
    sec_rgy_cursor_t        cursor;

    CLEAR_STATUS(st);

    /* For each account in the database, decrypt its key
     * with the old master key and re-encrypt with the
     * new master key.  Demote the write lock every 150
     * accounts so as to allow DB queries.
     */
    i = 0;
    cursor.valid = false;
    GLOBAL_RIW_PROMOTE;
    while( GOOD_STATUS(st) ) {
        i++;
        rs_mkey_reencrypt_acct_key(&cursor, old_eblock, old_version, 
                                     new_eblock, new_version, st);
        if( i >= 150 ) {
            GLOBAL_RIW_DEMOTE;
            GLOBAL_RIW_PROMOTE;
            i = 0;
        }
    }
    GLOBAL_RIW_DEMOTE;   

    /* Don't return bad status if status is "no_more_entries" 
     */
    if(STATUS_EQUAL(st, sec_rgy_no_more_entries))
        CLEAR_STATUS(st);

    return;
}



/*
 *  r s _ m k e y _ c r e a t e
 *
 *  Generate the intial master key.  Process the key
 *  in preparation for its use in encrypting database keys.
 *  Assign to master_key variables and store it in the
 *  special master_key file on disk.  Encrypt it under
 *  itself and store in the database.
 *
 *  Lock Notes:
 *  This operation should only be called by sec_create_db
 *  (with, in essence, a global write lock).
 */
PUBLIC void rs_mkey_create
#ifndef __STDC__
    ( rgy_local_cell, keyseed )
    sec_id_t            *rgy_local_cell;     /* [in] */
    char                *keyseed;            /* [in] */
#else
  (
    sec_id_t            *rgy_local_cell,
    char                *keyseed
  )
#endif
{
    char                    *mkey_name = n_master_key;
    char                    *realm;
    krb5_keyblock           seed_keyblock;
    sec_rgy_name_t          old_master_key_file;
    error_status_t          status, *st_p = &status;

#define cleanup { \
    if(seed_keyblock.contents) { \
        memset((char *)(seed_keyblock.contents), 0, seed_keyblock.length); \
        free(seed_keyblock.contents); \
    } \
}
#define err_cleanup { \
    if (master_keyblock_p) { \
        krb5_free_keyblock(master_keyblock_p); \
    } \
    cleanup; \
}

    master_keyblock_p = 0;
    seed_keyblock.contents = 0;

    /* delete <mkey> and <mkey>.old in case they exist; ignore errors */
    u_strcpy( (char *)old_master_key_file, MASTER_KEY_FILE );
    u_strcat( (char *)old_master_key_file, ".old" );
    rsdb_mkey_delete(MASTER_KEY_FILE, st_p);
    rsdb_mkey_delete((char *)old_master_key_file, st_p);
    /* ignore errors from rsdb_mkey_delete */
    CLEAR_STATUS(st_p);

    if(!(rsdb_name_util_realm_from_cell((char *)(rgy_local_cell->name), &realm))) {
        dce_svc_printf(SEC_RGY_MKY_BAD_CELLNAME_MSG, 
                       (char *) rgy_local_cell->name);
        dce_svc_printf(SEC_RGY_MKY_GET_REALM_NAME_MSG);
    }

    seed_keyblock.keytype = RSDB_AUTH_DEFAULT_KEY_TYPE;
    krb5_use_cstype(&master_encblock, RSDB_AUTH_DEFAULT_ENCTYPE);
    /* assemble & parse the master key name */
    if (*st_p = krb5_db_setup_mkey_name(mkey_name, realm, NULL,
                                      &master_princ)) {
        SEC_SVC_PRINT_MSG_ST(SEC_RGY_MKY_SETUP_MKEY_NAME_MSG, *st_p);
    }
    /* Generate a keyseed to initialize random key generator.
     * Use random key generator to create master key.
     * Use master_key to re-initialize random key generator.
     */
    rs_mkey_gen_init_keyseed(master_princ, &master_encblock, keyseed,
                             &seed_keyblock, st_p);
    if (BAD_STATUS(st_p)) {
        SEC_SVC_PRINT_MSG_ST(SEC_RGY_MKY_INIT_KEYSEED_MSG, *st_p);
    }
    if (*st_p = krb5_process_key(&master_encblock, &seed_keyblock)) {
        err_cleanup;
        SEC_SVC_PRINT_MSG_ST(SEC_RGY_MKY_PROCESS_KEYSEED_MSG, *st_p);
    }
    rblock.eblock = &master_encblock;
    if (*st_p = krb5_init_random_key(&master_encblock, &seed_keyblock,
				      &rblock.rseed)) {
        err_cleanup;
        (void) krb5_finish_key(&master_encblock);
        SEC_SVC_PRINT_MSG_ST(SEC_RGY_MKY_INIT_RANDOM_MSG, *st_p);
    }

    if (*st_p = krb5_random_key(rblock.eblock, rblock.rseed, &master_keyblock_p)) {
        err_cleanup;
        SEC_SVC_PRINT_MSG_ST(SEC_RGY_MKY_GEN_RANDOM_MSG, *st_p);
    }
    if (*st_p = krb5_process_key(&master_encblock, master_keyblock_p)) {
        err_cleanup;
        SEC_SVC_PRINT_MSG_ST(SEC_RGY_MKY_PROCESS_MASTER_KEY_MSG, *st_p);
    }
    if (*st_p = krb5_init_random_key(&master_encblock, master_keyblock_p,
				      &rblock.rseed)) {
        err_cleanup;
        (void) krb5_finish_key(&master_encblock);
        SEC_SVC_PRINT_MSG_ST(SEC_RGY_MKY_INIT_RANDOM_MSG, *st_p);
    }

    /* Initialize master_key_version.  New_master_key_version
     * is 0 to indicate no valid information in new master key variables.
     */
    master_key_version = 1;
    new_master_key_version = 0;
    /* Store the master_key in a file read&write-able only by root */
    rs_mkey_store_on_disk(MASTER_KEY_FILE, master_keyblock_p, 
                          master_key_version, st_p);
    if(BAD_STATUS(st_p)) {
        err_cleanup;
        SEC_SVC_PRINT_MSG_ST(SEC_RGY_MKY_STORE_DISK_MSG, *st_p);
    }
    /* Encrypt master_key and store in database for
     * master_key verification routine.
     */
    rs_mkey_encrypt_store_in_db(master_keyblock_p, master_key_version,
                                &master_encblock, st_p);
    if(BAD_STATUS(st_p)) {
        err_cleanup;
        SEC_SVC_PRINT_MSG_ST(SEC_RGY_MKY_STORE_DB_MSG, *st_p);
    }
    cleanup;
}



/*
 *  r s _ m k e y _ i n i t
 *
 *  Retrieve the master key from its file on disk.  Process the key
 *  in preparation for its use in encrypting database keys
 *  and initializing the random key generator.
 *  The rs_state.db_state and the presence of one or
 *  both of the <mkey> and <mkey>.old files determines
 *  which version of the stored master_key to use to
 *  initialize the globals.
 *  If db_state is mkey_changing and <mkey>.old exists,
 *  then the rs_mkey_change database work was not saved
 *  and the master_key variables should reflect the old
 *  master_key: rename <mkey>.old to <mkey>.  If <mkey>.old
 *  doesn't exist in this state, the the "old" master_key
 *  still resides in the <mkey> file.
 *  If the state is anything else, the new master_key database
 *  changes were saved and the new master_key already resides
 *  in the <mkey> file: delete <mkey>.old if it exists.
 *  Call rs_mkey_verify to ensure master_key is in synch with
 *  database.
 *
 *  Lock Notes:
 *  rs_mkey_init must be invoked with the global write lock
 *  and should only be invoked during secd initialization.
 *  
 */
PUBLIC void rs_mkey_init
#ifndef __STDC__
    ( rep_state, st_p )
    unsigned32          rep_state;      /* [in] */
    error_status_t      *st_p;          /* [out] */
#else
  (
    unsigned32          rep_state,
    error_status_t      *st_p
  )
#endif
{
    sec_rgy_name_t          old_master_key_file;
    boolean32               old_mkey_file_exists = false;
    error_status_t          tmp_status;

    CLEAR_STATUS(st_p);
    master_keyblock_p = 0;
    u_strcpy( (char *)old_master_key_file, MASTER_KEY_FILE );
    u_strcat( (char *)old_master_key_file, ".old" );

    if(rsdb_mkey_exists((char *)old_master_key_file))
        old_mkey_file_exists = true;

    if(old_mkey_file_exists) {
        if (rep_state == rs_c_state_mkey_changing) {
            /* Rename <mkey>.old to <mkey>, overwriting
             * <mkey> if it already exists.
             */
            rsdb_mkey_rename((char *)old_master_key_file, 
                             MASTER_KEY_FILE, st_p);
            if(BAD_STATUS(st_p)) {
                return;
            }
        } else {
            /* delete <mkey>.old  */
            rsdb_mkey_delete((char *)old_master_key_file, &tmp_status);
        }
    }

    krb5_use_cstype(&master_encblock, RSDB_AUTH_DEFAULT_ENCTYPE);
    if (!(master_keyblock_p =
              (krb5_keyblock *)malloc(sizeof(*master_keyblock_p)))) {
        SET_STATUS(st_p, sec_rgy_cant_allocate_memory);
        return;
    }
    master_keyblock_p->keytype = RSDB_AUTH_DEFAULT_KEY_TYPE;
                                      
    rs_mkey_fetch(MASTER_KEY_FILE, master_keyblock_p,
                  &master_key_version, st_p);
    if (BAD_STATUS(st_p)) {
        return;
    }

    if (*st_p = krb5_process_key(&master_encblock, master_keyblock_p)) {
        krb5_free_keyblock(master_keyblock_p);
        return;
    }
    rblock.eblock = &master_encblock;
    if (*st_p = krb5_init_random_key(&master_encblock, master_keyblock_p,
				      &rblock.rseed)) {
        krb5_free_keyblock(master_keyblock_p);
	krb5_finish_key(&master_encblock);
        return;
    }

    /* Make sure the the current master key globals are in synch
     * with the current encrypted master_key stored in the database.
     */
    rs_mkey_verify(master_keyblock_p, master_key_version, 
                   &master_encblock, st_p);
    if(BAD_STATUS(st_p))
        rs_mkey_purge();

}

/*
 *  r s _ m k e y _ s t o r e _ i n _ d b
 *
 *  Encrypt the mkey under itself (eblock must correspond to mkey)
 *  and store it as the new encrypted master_key in the database.
 *  
 *  Lock Notes:
 *  This operation must be invoked with the global write lock.
 */
PUBLIC void rs_mkey_store_in_db
#ifndef __STDC__
    ( st_p )
    error_status_t      *st_p;
#else
  (
    error_status_t      *st_p
  )
#endif
{
    /* Encrypt master_key and store in database for
     * master_key verification routine.
     */
    rs_mkey_encrypt_store_in_db(master_keyblock_p, master_key_version,
                                &master_encblock, st_p);
}

/*
 *  r s _ m k e y _ v e r i f y
 *
 *  Verify that the master_key in *mkey matches
 *  the master_key stored in the database in
 *  encrypted form.
 *
 *  eblock points to an encrypt_block used for the realm in question.
 *  krb5_process_key must have been previously called for this eblock.
 * 
 *  Lock Notes:
 *  A database (lock_db) read lock is required for this operation
 *  so that the master_key doesn't change during the operation.
 */
PUBLIC void rs_mkey_verify
#ifndef __STDC__
    ( mkey, mkey_version, eblock, st_p )
    krb5_keyblock           *mkey;          /* [in] */
    krb5_kvno               mkey_version;   /* [in] */
    krb5_encrypt_block      *eblock;        /* [in] */
    error_status_t          *st_p;          /* [out] */
#else
    (
    krb5_keyblock           *mkey,
    krb5_kvno               mkey_version,
    krb5_encrypt_block      *eblock,
    error_status_t          *st_p
    )
#endif
{
    rsdb_encrypt_key_t          encrypted_mkey;
    krb5_encrypted_keyblock     ekey;
    krb5_keyblock               tempkey;

    CLEAR_STATUS(st_p);
    encrypted_mkey.length = 0;
    ekey.contents = 0;
    tempkey.contents = 0;

    rsdb_get_enc_master_key(&encrypted_mkey, st_p);
    if (GOOD_STATUS(st_p)) {
        if (encrypted_mkey.mkey_version != mkey_version)
            SET_STATUS(st_p, sec_rgy_mkey_bad);
        else if(!(ekey.contents = (krb5_octet *)malloc(encrypted_mkey.length)))
            SET_STATUS(st_p, sec_rgy_cant_allocate_memory);
        else {
            ekey.keytype = encrypted_mkey.keytype;
            ekey.length  = encrypted_mkey.length;
            bcopy( encrypted_mkey.contents, ekey.contents, ekey.length);
            rs_mkey_decrypt_key(eblock, &ekey, &tempkey, st_p);
            if (GOOD_STATUS(st_p)) {
                if (memcmp((char *)mkey->contents, (char *)tempkey.contents,
                           mkey->length))
                    SET_STATUS(st_p, sec_rgy_mkey_bad);
            }
        }
    }

    /* Cleanup */
    if(encrypted_mkey.length)
        memset((char *)(encrypted_mkey.contents), 0, encrypted_mkey.length);
    if(ekey.contents) {
        memset((char *)(ekey.contents), 0, ekey.length);
        free(ekey.contents);
    }
    if(tempkey.contents) {
        memset((char *)(tempkey.contents), 0, tempkey.length);
        free(tempkey.contents);
    }
    return;
}



/*
 *  r s _ m k e y _ e n c r y p t _ w i t h _ m a s t e r
 *
 *  Encrypt a key (using the current master_key)
 *  for storage in the database.   The master_encblock is used
 *  to encrypt the key in "in" into "out";
 *  the storage pointed to by "out" is allocated before use.
 *
 *  Lock Notes:
 *  A database (lock_db) read lock is required for this operation
 *  so that the master_key doesn't change during the operation.
 */
PUBLIC void rs_mkey_encrypt_with_master
#ifndef __STDC__
    ( in, out, st_p )
    krb5_keyblock               *in;        /* [in] */
    krb5_encrypted_keyblock     *out;       /* [in, out] */
    error_status_t              *st_p;      /* [out] */
#else
    (
    krb5_keyblock               *in,
    krb5_encrypted_keyblock     *out,
    error_status_t              *st_p
    )
#endif
{
    CLEAR_STATUS(st_p);
 
    rs_mkey_encrypt_key(&master_encblock, in, out, st_p);
    return;
}



/*
 *  r s _ m k e y _ d e c r y p t _ w i t h _ m a s t e r
 *
 *
 *  Decrypt a key (using the master_key indicated by mkey_version)
 *  from its encrypted storage format in the database.  
 *  Either master_encblock or new_master_encblock is used
 *  to decrypt the key in "in" into "out"; the storage pointed to by "out"
 *  is allocated before use.
 *
 *  Lock Notes:
 *  A database (lock_db) read lock is required for this operation
 *  so that the master_key doesn't change during the operation.
 */
PUBLIC void rs_mkey_decrypt_with_master
#ifndef __STDC__
    ( mkey_version, in, out, st_p )
    krb5_kvno                   mkey_version;
    krb5_encrypted_keyblock     *in;
    krb5_keyblock               *out;
    error_status_t              *st_p;
#else
    (
    krb5_kvno                   mkey_version,
    krb5_encrypted_keyblock     *in,
    krb5_keyblock               *out,
    error_status_t              *st_p
    )
#endif
{
    if(mkey_version == master_key_version)
        rs_mkey_decrypt_key(&master_encblock, in, out, st_p);
    else if(mkey_version == new_master_key_version &&
            new_master_key_version != 0)
        rs_mkey_decrypt_key(&new_master_encblock, in, out, st_p);
    else
        dce_svc_printf(SEC_RS_MKEY_UNKNOWN_MSG);
}



/*
 *  r s _ m k e y _ r a n d o m
 *
 *  Generate a random DES key.
 *
 *  Lock Notes:
 *  A database (lock_db) read lock is required for this operation
 *  so that the master_key doesn't change during the operation.
 */
PUBLIC void rs_mkey_random
#ifndef __STDC__
    ( passwd, st_p )
    sec_passwd_rec_t    *passwd;             /* [in, out] */
    error_status_t      *st_p;
#else
  (
    sec_passwd_rec_t    *passwd,
    error_status_t      *st_p
  )
#endif
{
    krb5_keyblock       *random_key_p;

    if (*st_p = krb5_random_key(rblock.eblock, rblock.rseed,
                              &random_key_p))
        return;
    PASSWD_TYPE(passwd) = sec_passwd_des;
    bcopy(random_key_p->contents, DES_PASSWD(passwd),
          sec_passwd_c_des_key_size);
    krb5_free_keyblock(random_key_p);
}



/*
 *  r s _ m k e y _ s t r i n g _ t o _ k e y
 *
 *  Convert "pwd_data" into an encryption key "key"
 *  of type "keytype" using the master_encblock.
 *  "pepper" is used to seed or alter the conversion algorithm.
 *  On input, *key must be allocated, although key->contents
 *  is allocated inside krb5_string_to_key.
 *  Caller is responsible fro freeing key->contents.
 *
 *  Lock Notes:
 *  A database (lock_db) read lock is required for this operation
 *  so that the master_key doesn't change during the operation.
 */
PUBLIC void rs_mkey_string_to_key
#ifndef __STDC__
    ( keytype, pwd_data, pepper, key, st_p )
    krb5_keytype        keytype;        /* [in] */
    krb5_data           *pwd_data;      /* [in] */
    krb5_data           *pepper;        /* [in] */
    krb5_keyblock       *key;           /* [out] */
    error_status_t      *st_p;          /* [out] */
#else
  (
    krb5_keytype        keytype,
    krb5_data           *pwd_data,
    krb5_data           *pepper,
    krb5_keyblock       *key,
    error_status_t      *st_p
  )
#endif
{
    CLEAR_STATUS(st_p);

    *st_p = krb5_string_to_key(&master_encblock, keytype,
                             key, pwd_data, pepper);
}



/*
 *  r s _ m k e y _ c h a n g e
 *
 *  This operation is called by rs_rep_admin_mkey in
 *  rs_repadm.c for the change_master_key function.
 *
 *  Lock Notes:
 *  This operation should be called with the GLOBAL_RIW lock.
 */
PUBLIC void rs_mkey_change
#ifndef __STDC__
    ( st_p )
    error_status_t          *st_p;
#else
    (
    error_status_t          *st_p
    )
#endif
{
    sec_rgy_name_t          old_master_key_file;
    error_status_t          tmp_status;

    CLEAR_STATUS(st_p);

    /* Do some initialization and setup */
    new_master_keyblock_p    = 0;
    new_master_encblock.priv = 0;
    new_rblock.rseed         = 0;
    u_strcpy( (char *)old_master_key_file, MASTER_KEY_FILE );
    u_strcat( (char *)old_master_key_file, ".old" );
    
    /* Make sure the the current master key variables are in synch
     * with the current encrypted master_key stored in the database.
     */
    rs_mkey_verify(master_keyblock_p, master_key_version, 
                   &master_encblock, st_p);
    if(BAD_STATUS(st_p)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_mkey,
                       svc_c_sev_fatal | svc_c_action_abort,
                       *st_p);
    }

    GLOBAL_RIW_PROMOTE;
    rs_state_set_state(rs_c_state_mkey_changing, st_p);

    rs_mkey_gen_new(&rblock, &new_master_keyblock_p, 
                    &new_master_encblock, &new_rblock, st_p);
    if(GOOD_STATUS(st_p)) {
        rs_mkey_save(new_master_keyblock_p, &new_master_encblock,
                     &new_master_key_version, st_p);
    }
    GLOBAL_RIW_DEMOTE;

    if(GOOD_STATUS(st_p)) {
        /* For each account in the database, decrypt its key
         * with the old master key and re-encrypt with the
         * new master key.
         * GLOBAL_RIW lock gets promoted and demoted in rs_mkey_reencrypt_db_keys
         */
        rs_mkey_reencrypt_db_keys(&master_encblock, master_key_version,
                                  &new_master_encblock, new_master_key_version,
                                  st_p);
    }

    if(GOOD_STATUS(st_p)) {
        /* rsdb_checkpt sets rep_state to rs_c_state_renaming, 
         * then to rs_c_state_in_service
         */
        rsdb_checkpt(rs_c_state_in_service, lock_mode_riw, st_p);
    }

    /* The global write lock is required for the following
     * code segment; no one else should be reading the master_key
     * variables while they are being shuffled.
     * A GLOBAL_RIW_PROMOTE is not done here because 
     * rsdb_checkpt leaves the global lock promoted.
     */
    if(GOOD_STATUS(st_p)) {
        /* delete old mkey file */
        rsdb_mkey_delete((char *)old_master_key_file, &tmp_status);

        /* Free old master key info and
         * copy new master key into global master key variables.
         * Clear new_master_key globals to indicate they're invalid.
         */
        krb5_free_keyblock(master_keyblock_p);
        krb5_finish_key(&master_encblock);
        krb5_finish_random_key(&master_encblock, &rblock.rseed);
        master_keyblock_p = new_master_keyblock_p;
        new_master_keyblock_p = 0;
        master_encblock = new_master_encblock;
        new_master_encblock.priv = 0;
        rblock.rseed = new_rblock.rseed;
        new_rblock.rseed = 0;
        master_key_version = new_master_key_version;
        new_master_key_version = 0;
    } else {
        /* Cleanup new_master_key variables */
        if(new_master_keyblock_p) {
            if(new_master_keyblock_p->contents) {
                krb5_free_keyblock(new_master_keyblock_p);
                new_master_keyblock_p = 0;
            }
        }
        if(new_master_encblock.priv) {
            krb5_finish_key(&new_master_encblock);
            new_master_encblock.priv = 0;
        }
        if(new_rblock.rseed) {
            krb5_finish_random_key(&new_master_encblock, &new_rblock.rseed);
            new_rblock.rseed = 0;
        }
    }
}

 

/*
 *  r s _ m k e y _ p u r g e
 *
 *  Memset to 0 and free all master_key structures.
 *  This operation is called at task completion for
 *  process cleanup.
 *
 *  Lock Notes:
 *  This operation should be called with the GLOBAL_RIW lock.
 */
PUBLIC void rs_mkey_purge (
#ifdef __STDC__
    void
#endif
)
{
    krb5_free_keyblock(master_keyblock_p);
    (void) krb5_finish_key(&master_encblock);
    krb5_finish_random_key(&master_encblock, &rblock.rseed);
}

