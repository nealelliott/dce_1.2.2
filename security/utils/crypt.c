/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crypt.c,v $
 * Revision 1.1.44.1  1996/10/03  15:13:59  arvind
 * 	Deal with change to interface of pwd_keyproc()
 * 	[1996/09/16  23:32 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 *
 * Revision 1.1.42.2  1996/02/18  00:22:35  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:10:13  marty]
 * 
 * Revision 1.1.42.1  1995/12/08  18:02:02  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:22:54  root]
 * 
 * Revision 1.1.40.1  1993/10/05  22:31:12  mccann
 * 	CR8651 64 bit porting changes
 * 	[1993/10/04  19:14:36  mccann]
 * 
 * Revision 1.1.2.3  1992/12/29  16:40:29  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:31:50  zeliff]
 * 
 * Revision 1.1.2.2  1992/12/04  20:42:31  burati
 * 	CR6314 Don't allocate cksum contents, since it's done for us.
 * 	[1992/12/04  18:48:51  burati]
 * 
 * Revision 1.1  1992/01/19  14:41:42  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  crypt.c
**
** Copyright (c) Hewlett-Packard Company 1991,1992
** Unpublished work. All Rights Reserved.
**
*/
/* 
 *      Security Service (de/en)cryption API
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)crypt.c	3 - 10/03/91";
#endif

#include <stdio.h>
#include <un_maloc.h>
#include <bstring.h>
#include <rgymacro.h>
#include <u_str.h>  
#include <dce/rgynbase.h>
#include <dce/passwd.h>
#include <krb5/krb5.h>  
#include <krb5/encryption.h>
#include <sec_krb_util.h>
  
#include <crypt.h>
#include <crypt_inline.c>

PUBLIC  void  sec_crypt_compute_key
#ifndef __STDC__
    ( plain, cipher_type, cipher, stp ) 
     sec_passwd_rec_t    *plain;       /* [in] */
     sec_passwd_type_t   cipher_type;  /* [out] */
     sec_passwd_rec_t    *cipher;      /* [out] */
     error_status_t      *stp;
#else
  (
     sec_passwd_rec_t    *plain,       /* [in] */
     sec_passwd_type_t   cipher_type,  /* [out] */
     sec_passwd_rec_t    *cipher,      /* [out] */
     error_status_t      *stp
  )
#endif
{
    krb5_keyblock           *krb_key = NULL; 
    krb5_keytype            krb_keytype;
    krb5_data 		    keyproc_arg;
    krb5_data		    krb_salt;
    
    CLEAR_STATUS(stp);
    krb_keytype = sec_to_krb_keytype(cipher_type, stp);
    if (BAD_STATUS(stp)) {
        SET_STATUS(stp, sec_rgy_key_bad_type);
        return;
    }

    switch(PASSWD_TYPE(plain)) {
        
    case sec_passwd_plain:
	keyproc_arg.data = (char *) PLAIN_PASSWD(plain);
	keyproc_arg.length = strlen(keyproc_arg.data);

        if (keyproc_arg.length < 1) {
            SET_STATUS(stp, sec_login_s_null_password);
            return;
        }
        
        if (plain->pepper != NULL) {
	    krb_salt.length = u_strlen(plain->pepper);
	    krb_salt.data =  (char *)plain->pepper;
        }
        
        *stp = pwd_keyproc(NULL, krb_keytype,
			   &krb_salt, &keyproc_arg, &krb_key);
        
        if (GOOD_STATUS(stp)) {
            /* put the krb5 style key into its sec_passwd_rec_t equivalent */
            cipher->pepper = NULL;
            cipher->version_number = plain->version_number;
            memcpy(DES_PASSWD(cipher), krb_key->contents, sec_passwd_c_des_key_size);
            PASSWD_TYPE(cipher) = cipher_type;
        }

        if (krb_key != NULL) {
            krb5_free_keyblock(krb_key);
        }

        break;
        
        
    default:
        SET_STATUS(stp, sec_rgy_key_bad_type);
        break;
    }
}


PUBLIC  void  sec_crypt_free_key_contents
#ifndef __STDC__
    ( key ) 
     sec_passwd_rec_t    *key;
#else
  (
     sec_passwd_rec_t    *key
  )
#endif
{
    if (key) {
        switch(PASSWD_TYPE(key)) {

        case sec_passwd_plain:
            if (PLAIN_PASSWD(key)) {
                u_free(PLAIN_PASSWD(key));
            }
            break;
        case sec_passwd_des:
            memset(DES_PASSWD(key), 0, sec_passwd_c_des_key_size);
            break;
        default:
            break;
            
        }
               
        if (key->pepper != NULL) {
            u_free(key->pepper);

        }
    }
}


PUBLIC  void  sec_crypt_free_key
#ifndef __STDC__
    ( key ) 
     sec_passwd_rec_t    **key;
#else
  (
     sec_passwd_rec_t    **key
  )
#endif
{
    if (key) {
        sec_crypt_free_key_contents(*key);
        free((char *) *key);
        *key = NULL;
    }
}


PUBLIC  sec_crypt_handle_t  sec_crypt_create_context
#ifndef __STDC__
    ( ekey, etype, stp )        
     sec_passwd_rec_t    *ekey;    /* in - must be ciphertext */
     sec_etype_t         etype;    /* in - requested encryption type */
     error_status_t      *stp;
#else
  (
     sec_passwd_rec_t    *ekey,    /* in - must be ciphertext */
     sec_etype_t         etype,    /* in - requested encryption type */
     error_status_t      *stp
  )
#endif
{
     sec_crypt_context_t  *cc;

     if (PASSWD_TYPE(ekey) == sec_passwd_plain) {
         SET_STATUS(stp, sec_rgy_key_bad_type);
         return NULL;
     }

     CLEAR_STATUS(stp);
     cc = (sec_crypt_context_t *) malloc(sizeof(sec_crypt_context_t));
     if (cc == NULL) {
         SET_STATUS(stp, sec_rgy_cant_allocate_memory);
         return NULL;
     }
     memset(cc, 0, sizeof(sec_crypt_context_t));

     cc->etype = sec_to_krb_etype(etype, stp);

     if (GOOD_STATUS(stp)) {
         cc->ekey = sec_to_krb_key(ekey, stp);
     }

     if (BAD_STATUS(stp)) {
         free((char *) cc);
         return NULL;
     }
   
     krb5_use_cstype(&cc->eblock, cc->etype);
     *stp = krb5_process_key(&cc->eblock, cc->ekey);

     if (BAD_STATUS(stp)) {
         krb5_free_keyblock(cc->ekey);
         free((char *) cc);
         return NULL;
     }

     return((sec_crypt_handle_t)cc);
}


PUBLIC  void  sec_crypt_destroy_context
#ifndef __STDC__
    ( context )        
     sec_crypt_handle_t  *context;
#else
  (
     sec_crypt_handle_t  *context
  )
#endif
{
    sec_crypt_context_t  *cc;

    if (context != NULL) {
        cc = (sec_crypt_context_t *) *context;
        if (cc != NULL) {
            krb5_finish_key(&cc->eblock);
            krb5_free_keyblock(cc->ekey);
            free((char *) cc);
        }
        *context = NULL;
    }
}


PUBLIC void  sec_crypt_free_cksum_contents
#ifndef __STDC__
    ( checksum )        
     sec_chksum_t        *checksum; 
#else
  (
     sec_chksum_t        *checksum
  )
#endif
{
    if (checksum) {
        if (checksum->len != 0 && checksum->chksum != NULL) {
            free((char *) checksum->chksum);
        }
    }
}


PUBLIC  void  sec_crypt_free_checksum
#ifndef __STDC__
    ( checksum )        
     sec_chksum_t        **checksum; 
#else
  (
     sec_chksum_t        **checksum
  )
#endif
{

    if (checksum) {
        if (*checksum) {
            sec_crypt_free_cksum_contents(*checksum);
            free((char *) *checksum);
            *checksum = NULL;
        }
    }
}


PUBLIC  boolean32  sec_crypt_checksum_equal
#ifndef __STDC__
    ( checksum1, checksum2 )        
     sec_chksum_t        *checksum1; 
     sec_chksum_t        *checksum2; 
#else
  (
     sec_chksum_t        *checksum1, 
     sec_chksum_t        *checksum2
  )
#endif
{
    return (checksum1->chksum_type == checksum2->chksum_type 
            && checksum1->len == checksum2->len
            && memcmp(checksum1->chksum, checksum2->chksum, checksum1->len) == 0);
}


PUBLIC  void  sec_crypt_compute_checksum
#ifndef __STDC__
    ( context, len, data, chksum, stp )        
     sec_crypt_handle_t  context;
     unsigned32          len; 
     unsigned_char_t     *data;
     sec_chksum_t        *chksum; 
     error_status_t      *stp;
#else
  (
     sec_crypt_handle_t  context,
     unsigned32          len, 
     unsigned_char_t     *data,
     sec_chksum_t        *chksum, 
     error_status_t      *stp
  )
#endif
{
    krb5_checksum        krb_cksum;
    sec_crypt_context_t  *cc = (sec_crypt_context_t *) context;

    krb_cksum.checksum_type = sec_to_krb_chksum_type(chksum->chksum_type, stp);
    if (BAD_STATUS(stp)) { 
        return;
    }

    *stp = (*krb5_cksumarray[krb_cksum.checksum_type]->sum_func)(data,
                                                                 len, 
                                                                 cc->ekey->contents,
                                                                 cc->ekey->length,
                                                                 &krb_cksum);
    if (GOOD_STATUS(stp)) {
        chksum->chksum = (unsigned_char_p_t) krb_cksum.contents;
        chksum->len = krb_cksum.length;
    } else {
        free((char *) krb_cksum.contents);
    }
}

