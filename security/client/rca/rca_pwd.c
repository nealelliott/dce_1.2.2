/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rca_pwd.c,v $
 * Revision 1.1.9.2  1996/10/03  14:47:34  arvind
 * 	CHFts19811,CHFts19908: data integ error fix
 * 	[1996/08/28  17:43 UTC  aha  /main/DCE_1.2.2/aha_dbpk1_2/1]
 *
 * 	CHFts19811: need to set caller_key->pepper=NULL after freeing it.
 * 	[1996/08/27  14:18 UTC  aha  /main/DCE_1.2.2/2]
 *
 * 	CHFts19811: more robust fix for memory leak in
 * 	rca_pwd_gen_transmit_rep()
 * 	[1996/08/23  16:04 UTC  aha  /main/DCE_1.2.2/aha_dbpk1/2]
 *
 * 	CHFts19811: fix memory leak in rca_pwd_gen_transmit_rep() on
 * 	caller_key->pepper
 * 	[1996/08/21  16:59 UTC  aha  /main/DCE_1.2.2/aha_dbpk1/1]
 *
 * 	Merge Registry support for KDC private key storage
 * 	[1996/07/13  20:48 UTC  aha  /main/aha_pk6/1]
 *
 * 	Merge-out and bugfixes and Registry work
 * 	[1996/03/11  13:28:10  marty  1.1.7.2]
 *
 * Revision 1.1.9.1  1996/08/09  12:03:46  arvind
 * 	Merge Registry support for KDC private key storage
 * 	[1996/07/13  20:48 UTC  aha  /main/aha_pk6/1]
 * 
 * 	Merge-out and bugfixes and Registry work
 * 	[1996/03/11  13:28:10  marty  1.1.7.2]
 * 
 * Revision 1.1.7.2  1996/03/11  13:28:10  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:15:56  marty]
 * 
 * Revision 1.1.7.1  1995/12/08  18:07:29  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/24  19:27 UTC  mullan_s
 * 	Merge
 * 
 * 	HP revision /main/mullan_mothra_bin_compat10/1  1995/05/23  19:01 UTC  mullan_s
 * 	Include sec_login_util.h instead of dce/sec_login_util.h
 * 	because it is no longer exported.
 * 	[1995/12/08  16:32:28  root]
 * 
 * Revision 1.1.3.3  1992/12/29  13:03:27  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:41:05  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/29  21:16:12  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:20:08  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:47:33  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rca_pwd.c V=8 11/22/91 //littl/prgy/src/client/rca
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
/* 
 *      Registry Client - Password pickling
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)rca_pwd.c	8 - 11/22/91";
#endif

#include <stdio.h>
#include <un_maloc.h>
#include <un_io.h>
#include <bstring.h>
#include <un_strng.h>
#include <rgymacro.h>
#include <sec_login_util.h>
#include <sec_pkl.h>

#include <rca_pwd.h>
#include <u_str.h>
 
#include <crypt.h>
/* need these cause we're inlining crypt_inline.c */
#include <krb5/krb5.h>  
#include <krb5/encryption.h>
#include <crypt_inline.c>

/* 
 * r c a _ p w d _ g e n _ t r a n s m i t _ r e p
 *
 * generate a "pickled" version of the plain password suitable for
 * transmission to the server.  This routine allocates storage for
 * the pickled passwd which must be freed with a call to 
 * rca_pwd_free_transmit_rep.  The routine takes a fatal error 
 * if memory allocation fails.
 *
 * If the plain_passwd is zero length, we just marshall a '\0' 
 * character, so we have a legit pickle.
 * 
 * NOTE:
 *   For now, we only support plaintext passwords, and we don't
 *   bother encrypting them.  
 */


PUBLIC void rca_pwd_gen_transmit_rep
    ( identity, caller_key, new_key, transmit_rep, stp ) 
    sec_login_handle_t         identity;
    sec_passwd_rec_t           *caller_key;     /* [in]  */
    sec_passwd_rec_t           *new_key;        /* [in]  */
    rs_acct_key_transmit_t     **transmit_rep;  /* [out] */
    error_status_t             *stp;            /* [out] */
{
    sec_etype_t                etype;
    sec_chksum_t               key_chksum;
    sec_passwd_rec_t           ekey;
    boolean32                  use_ekey = false;
    sec_crypt_handle_t         ch = NULL;  
    idl_pkl_t                  *new_key_pickle;
    unsigned32                 new_key_pickle_len;
    idl_pkl_t                  *chksum_pickle;
    unsigned32                 chksum_pickle_len;

    CLEAR_STATUS(stp);

    *transmit_rep =  (rs_acct_key_transmit_t *) 
	malloc(sizeof(rs_acct_key_transmit_t));
    
    if (*transmit_rep == NULL) {
	SET_STATUS(stp, sec_rgy_cant_allocate_memory);
	return;
    }
    memset(*transmit_rep, 0, sizeof(rs_acct_key_transmit_t));
    
    (*transmit_rep)->key = 
	(rs_encrypted_pickle_t *) malloc(sizeof(rs_encrypted_pickle_t));
    if ((*transmit_rep)->key == NULL) {
	free((char *) *transmit_rep);
	SET_STATUS(stp, sec_rgy_cant_allocate_memory);
	return;
    }
    
    (*transmit_rep)->checksum = 
	(rs_encrypted_pickle_t *) malloc(sizeof(rs_encrypted_pickle_t));
    if ((*transmit_rep)->checksum == NULL) {
	free((char *) (*transmit_rep)->key);
	free((char *) *transmit_rep);
	SET_STATUS(stp, sec_rgy_cant_allocate_memory);
	return;
    }
    
    memset((*transmit_rep)->key, 0, sizeof(rs_encrypted_pickle_t));
    memset((*transmit_rep)->checksum, 0, sizeof(rs_encrypted_pickle_t));
    
    memset(&key_chksum, 0, sizeof(key_chksum));
    memset(&ekey, 0, sizeof(ekey));
    etype = sec_etype_des_cbc_crc;             /* only supported etype */
    
    /* now we can get down to business */

    /*
     * Compute the caller's ciphertext key from plaintext, if necessary. 
     * if the caller's key is plaintext, and has no "pepper", then use
     * the pepper in the login context, if any, to compute the ciphertext
     * key.
     */
    if (PASSWD_TYPE(caller_key) == sec_passwd_plain) {
	char	*pepper = NULL;

        if (caller_key->pepper == NULL) {
            pepper = sec_login_util_get_pepper(identity, stp);
	    caller_key->pepper = (unsigned_char_p_t) pepper;
        }

        if (GOOD_STATUS(stp)) {
            use_ekey = true;
            sec_crypt_compute_key(caller_key, sec_passwd_des, &ekey, stp);
        }

        if (pepper != NULL) {
            free(pepper);
	    pepper = NULL;
            caller_key->pepper = NULL;
	}
    } else {
        PASSWD_TYPE(&ekey) = PASSWD_TYPE(caller_key);
    }
    
    if (GOOD_STATUS(stp)) {
	if (use_ekey) {
	    ch = sec_crypt_create_context(&ekey, etype, stp);
	} else {
	    ch = sec_crypt_create_context(caller_key, etype, stp);
	}
    }

    (*transmit_rep)->enc_keytype    = PASSWD_TYPE(&ekey);
    sec_crypt_free_key_contents(&ekey);   

    if (BAD_STATUS(stp)) {
	rca_pwd_free_transmit_rep(transmit_rep);
	return;
    }

    /* 
     * pickle the new key, and encrypt the resulting data stream
     */
    sec_passwd_rec_t_pickle(new_key, NULL, (void *(*)(unsigned32)) malloc, (unsigned32)8,
                            &new_key_pickle, &new_key_pickle_len, stp);

    if (BAD_STATUS(stp)) {
	rca_pwd_free_transmit_rep(transmit_rep);
	return;
    }
    
    sec_crypt_encrypt(ch, NULL, NULL, new_key_pickle_len, (Pointer) new_key_pickle,
		      &(*transmit_rep)->key->enc_pickle_len,
		      (Pointer *) &(*transmit_rep)->key->enc_pickle, NULL, stp);

    
    if (BAD_STATUS(stp)) {
	(*transmit_rep)->key->enc_pickle = NULL;
	(*transmit_rep)->key->enc_pickle_len = 0;
	rca_pwd_free_transmit_rep(transmit_rep);

        free(new_key_pickle);
	sec_crypt_destroy_context(&ch);
	return;
    }

    /* compute a collision proof checksum of the pickle */
    key_chksum.chksum_type = sec_chksum_des_cbc;  /*!!! must be collision-proof */
    sec_crypt_compute_checksum(ch, new_key_pickle_len, *new_key_pickle, 
                               &key_chksum, stp);

    (*transmit_rep)->key_pickle_len = new_key_pickle_len;

    /* we're done with the passwd pickle, regardless */
    free(new_key_pickle);

    /* 
     * now pickle and encrypt the checksum
     */
    sec_chksum_t_pickle(&key_chksum, NULL, (void *(*)(unsigned32)) malloc, (unsigned32)8, 
                        &chksum_pickle, &chksum_pickle_len, stp);
    if (BAD_STATUS(stp)) {
	rca_pwd_free_transmit_rep(transmit_rep);
	sec_crypt_destroy_context(&ch);
	return;
    }

    sec_crypt_encrypt(ch, NULL, NULL, chksum_pickle_len, (Pointer) chksum_pickle,
		      &(*transmit_rep)->checksum->enc_pickle_len,
		      (Pointer *) &(*transmit_rep)->checksum->enc_pickle, NULL, stp);
    
    /* done with key_chksum and encryption context, regardless */
    sec_crypt_free_cksum_contents(&key_chksum);
    sec_crypt_destroy_context(&ch);

    if (BAD_STATUS(stp)) {
	(*transmit_rep)->checksum->enc_pickle = NULL;
	(*transmit_rep)->checksum->enc_pickle_len = 0;
	rca_pwd_free_transmit_rep(transmit_rep);

        free(chksum_pickle);
	return;
    }

    free(chksum_pickle);

    (*transmit_rep)->enc_key_version = caller_key->version_number;
    (*transmit_rep)->enc_type = etype;
}



PUBLIC void rca_pwd_free_transmit_rep
#ifndef __STDC__
    ( transmit_rep ) 
    rs_acct_key_transmit_t  **transmit_rep;  /* [in, out] */
#else
  (
    rs_acct_key_transmit_t  **transmit_rep
  )
#endif
{
  
    if (transmit_rep == NULL) {
	return;
    }

    if (*transmit_rep != NULL) {

	if ((*transmit_rep)->key != NULL) {
	    if ((*transmit_rep)->key->enc_pickle != NULL) {
		free((char *) (*transmit_rep)->key->enc_pickle);
	    }
	    free((char *) (*transmit_rep)->key);
	}

	if ((*transmit_rep)->checksum != NULL) {
	    if ((*transmit_rep)->checksum->enc_pickle != NULL) {
		free((char *) (*transmit_rep)->checksum->enc_pickle);
	    }
	    free((char *) (*transmit_rep)->checksum);
	}

	free( (char *) (*transmit_rep));
	*transmit_rep = NULL;
    }
}
