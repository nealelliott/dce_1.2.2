/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY 
 * $Log: rs_pwd.c,v $
 * Revision 1.1.14.1  1996/08/09  12:09:49  arvind
 * 	Merge Registry support for KDC private key storage
 * 	[1996/07/13  20:49 UTC  aha  /main/aha_pk6/1]
 *
 * 	Merge-out and bugfixes and Registry work
 * 	[1996/02/18  00:19:17  marty  1.1.12.3]
 *
 * Revision 1.1.12.3  1996/02/18  00:19:17  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:07:43  marty]
 * 
 * Revision 1.1.12.2  1995/12/08  17:54:54  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/06/05  21:53 UTC  jrr
 * 	Fix memory leak of crypto context in rs_pwd_get_key_by_session_key
 * 	[1995/06/01  18:32 UTC  burati  /main/HPDCE02/mb_mothra4/1]
 * 
 * 	HP revision /main/HPDCE02/1  1994/12/15  03:29 UTC  rps
 * 
 * 	HP revision /main/rps_mothra/1  1994/12/15  00:08 UTC  rps
 * 	[OT12669] cell renaming
 * 	[1995/12/08  17:21:16  root]
 * 
 * Revision 1.1.8.2  1994/08/04  16:13:51  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:12:03  mdf]
 * 
 * Revision 1.1.8.1  1994/06/02  21:23:04  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:19:30  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 	[1994/05/18  11:56:36  mdf]
 * 
 * 	HP revision /main/KK/ODESSA_2/1  1994/05/04  20:39 UTC  hanfei
 * 	 merge
 * 
 * 	HP revision /main/KK/hanfei_ch_master/1  1994/05/04  20:15 UTC  hanfei
 * 	merge
 * 
 * Revision 1.1.6.2  1993/11/02  14:35:05  mullan_s
 * 	Check all alias accounts in rs_pwd_get_key_by_prnc() before
 * 	assuming failure.
 * 	[1993/11/02  14:32:33  mullan_s]
 * 
 * Revision 1.1.4.3  1993/09/15  15:50:50  root
 * 	    Loading drop DCE1_0_3b03
 * 
 * Revision 1.1.6.7  1993/04/08  18:21:49  burati
 * 	CR7668 Don't free key contents in rs_pwd_unix_pwd_fr_xmit because
 * 	they were alloc'd with rpc_ss_allocate, and will be freed when we
 * 	do the rpc_ss_disable_allocate()
 * 	[1993/04/08  17:44:58  burati]
 * 
 * Revision 1.1.6.6  1993/03/19  22:21:08  emartin
 * 	CR 7472. rs_pwd_get_key => rs_pwd_get_key_by_prnc - returns
 * 	princ's key.  nuke some unix pwd routines.  Now just have
 * 	rs_pwd_unix_pwd_to/fr_xmit, rs_pwd_to_unix_pwd
 * 	[1993/03/19  18:55:38  emartin]
 * 
 * 	CR 7472. do rs_pwd_unix_pwd_fr_xmit_by_prnc, rs_pwd_to_unix_pwd
 * 	[1993/03/16  23:04:23  emartin]
 * 
 * 	CR 7472. rs_pwd_unix_pwd_to_xmit, rs_pwd_unix_pwd_to_xmit_by_prnc
 * 	convert unix password to its wire rep
 * 	[1993/03/15  23:06:56  emartin]
 * 
 * Revision 1.1.6.5  1992/12/29  16:36:33  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:25:41  zeliff]
 * 
 * Revision 1.1.6.4  1992/12/04  22:17:15  burati
 * 	CR6314 Wasn't free'ing chksum in get_key()
 * 	[1992/12/04  21:54:07  burati]
 * 
 * Revision 1.1.6.3  1992/10/28  20:15:57  burati
 * 	CR5829 Fix infinite loop problem in rs_pwd_get_key
 * 	[1992/10/27  21:31:59  burati]
 * 
 * Revision 1.1.6.2  1992/10/07  20:36:08  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:18:36  sekhar]
 * 
 * Revision 1.1.4.2  1992/10/06  17:24:10  burati
 * 	Second replication drop: bl5
 * 
 * Revision 1.1.3.2  1992/07/13  20:02:48  ahop
 * 	 3903: update rsdb_acct_get_auth_key_by_name params
 * 	[1992/06/30  22:31:58  ahop]
 * 
 * Revision 1.1  1992/01/19  14:44:00  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  
 * Copyright (c) Hewlett-Packard Company 1991, 1992, 1995
 * Unpublished work. All Rights Reserved.
 */
/* 
 * Registry Server - Password management
 */

#ifdef MODULE_VERSION_ID
static char *VersionID =
    "$Id: rs_pwd.c,v 1.1.14.1 1996/08/09 12:09:49 arvind Exp $";
#endif

#include <un_maloc.h>
#include <bstring.h>
#include <un_strng.h>
#include <rgymacro.h>
#include <dce/dce.h>
#include <dce/sec_base.h>
#include <dce/pkl.h>
#include <sec_krb_util.h>
#include <krb5/base-defs.h>
#include <krb5/krb5.h>
#include <krb5/encryption.h>

#include <sec_pkl.h>
#include <rs_pwd.h>
#include <rs_util.h>
#include <rs_lock.h>
#include <rsdb_pgo.h>
#include <rsdb_acct.h>

#include <crypt.h>  /* public version */
#include <crypt_inline.c>

#include <sec_krb_util.h>
#include <sec_svc.h>

/*
 * PRIVATE FUNCTIONS 
 */

PRIVATE void get_auth_key_by_prnc
  (
    uuid_t                 *princ,
    sec_passwd_type_t      key_type,
    sec_passwd_version_t   key_version,
    sec_passwd_rec_t       **princ_key,
    sec_rgy_cursor_t       *cursor,
    error_status_t         *stp
  )
{
    sec_rgy_pgo_item_t     pitem;
    sec_rgy_name_t         pname;
    krb5_keyblock          *krb_key = NULL;
    krb5_keytype           krb_keytype;
    boolean32              default_pepper;
    krb5_kvno              mkey_version;

    CLEAR_STATUS(stp);
    *princ_key = NULL;

    rsdb_pgo_get_by_uid(sec_rgy_domain_person, princ, true, 
                        cursor, pname, &pitem, stp);
    if (GOOD_STATUS(stp)) {
        krb_keytype = sec_to_krb_keytype(key_type, stp);
        if (GOOD_STATUS(stp)) {
            rsdb_acct_get_auth_key_by_name(pname, NULL,
					   krb_keytype, key_version,
                                           &krb_key, &default_pepper,
                                           NULL, &mkey_version, stp);
            if (BAD_STATUS(stp)) {
                krb_key = NULL;
            }
        }
    }
        
    if (GOOD_STATUS(stp)) {
        *princ_key = krb_to_sec_key(krb_key, stp);
        krb5_free_keyblock(krb_key);  /* can handle NULL pointers */
    }

}

/*
 * Can't deal with multiple key types yet
 * ASSUME DATABASE IS LOCKED.
 */
PRIVATE  void  get_encryption_context
  (
    uuid_t                 *princ,
    sec_etype_t            etype,
    sec_passwd_type_t      key_type,
    sec_passwd_version_t   key_version,
    sec_passwd_rec_t       **princ_key,
    sec_crypt_handle_t     *context,
    sec_rgy_cursor_t       *cursor,
    error_status_t         *stp
  )
{
    CLEAR_STATUS(stp);
    *context = NULL;
    *princ_key = NULL;

    get_auth_key_by_prnc(
        princ, key_type, key_version, princ_key, cursor, stp);
    
    if (GOOD_STATUS(stp)) {
        *context = sec_crypt_create_context((*princ_key), etype, stp);

        if (BAD_STATUS(stp)) {
            sec_crypt_free_key(princ_key);
        }
    }
}

/*
 * g e t _ k e y
 *
 * Retrieve a password rec from the pickled network rep.
 *
 * NOTE WELL: all heap storage for (sub)fields in the rs_acct_key_transmit_t
 * parameter MUST be allocated using rpc_ss_allocate.  As this routine uses
 * rpc_ss_allocate to allocate internal pickle storage, there is no need to
 * free pickle storage--we just assume it will all go away at some point after
 * the routine completes.
 *
 */
PRIVATE void get_key
  (
    rs_acct_key_transmit_t     *transmit_rep,
    sec_crypt_handle_t         econtext,     
    sec_passwd_rec_t           *new_key,     
    error_status_t             *stp
  )
{
    sec_chksum_t               cksum, my_cksum;
    idl_pkl_t                  *key_pkl, *sum_pkl;

    CLEAR_STATUS(stp);

    sec_crypt_decrypt(econtext, (Pointer (*)(unsigned32))rpc_ss_allocate,
                                (void (*)(Pointer))rpc_ss_free, NULL,
                      transmit_rep->checksum->enc_pickle_len,
                      (Pointer)transmit_rep->checksum->enc_pickle,
                      (Pointer *)&sum_pkl, stp);

    if (GOOD_STATUS(stp)) {
        sec_chksum_t_unpickle((idl_pkl_t *)sum_pkl, rpc_ss_allocate,
                                &cksum, stp);
    }

    if (GOOD_STATUS(stp)) {
        sec_crypt_decrypt(econtext, (Pointer (*)(unsigned32))rpc_ss_allocate,
                          (void (*)(Pointer))rpc_ss_free, NULL,
                          transmit_rep->key->enc_pickle_len,
                          (Pointer)transmit_rep->key->enc_pickle,
                          (Pointer *)&key_pkl, stp);
    }
    if (GOOD_STATUS(stp)) {

        my_cksum.chksum_type = cksum.chksum_type;
        my_cksum.len = 0;
        my_cksum.chksum = NULL;
        sec_crypt_compute_checksum(econtext, transmit_rep->key_pickle_len,
                                   *key_pkl, &my_cksum, stp);
        if (GOOD_STATUS(stp)) {
            if (!sec_crypt_checksum_equal(&cksum, &my_cksum))
                SET_STATUS(stp, sec_rgy_bad_integrity);
        }
        sec_crypt_free_cksum_contents(&my_cksum);

        if (GOOD_STATUS(stp)) {
            sec_passwd_rec_t_decode(transmit_rep->key_pickle_len, (idl_byte *)key_pkl,
                                    rpc_ss_allocate, rpc_ss_free,
                                    new_key,
                                    stp);
        }
    }
}


/*
 * PUBLIC functions
 */


/*
 * r s _ p w d _ g e t _ k e y _ b y _ p r n c
 *
 * Retrieve a password rec from the pickled network rep using the originator's
 * principal key.
 */
PUBLIC void rs_pwd_get_key_by_prnc
  (
    rs_acct_key_transmit_t     *transmit_rep,  /* [in] */
    uuid_t                     *principal,     /* [in] */
    sec_passwd_rec_t           *new_key,       /* [out] */
    sec_passwd_rec_t           **princ_key,     /* [out] */
    error_status_t             *stp
  )
{
    boolean32                  done = false;
    sec_rgy_cursor_t           cursor;
    sec_crypt_handle_t         ch;
    error_status_t             save_status;

    CLEAR_STATUS(stp);
    CLEAR_STATUS(&save_status);
    *princ_key = NULL;

    if (transmit_rep == NULL) {
        dce_svc_printf(SEC_RS_PWD_BOGUS_PICKLE_MSG);
    }

    rs_util_clear_cursor(&cursor);
    while (!done) {
        get_encryption_context(principal,
                               transmit_rep->enc_type,
                               transmit_rep->enc_keytype,
                               transmit_rep->enc_key_version,
                               princ_key,
                               &ch, &cursor, stp);
        if (BAD_STATUS(stp)) {
            if (STATUS_EQUAL(stp, sec_rgy_no_more_entries)) {
		/* no_more_entries indicates
		 * that there are no [more] principal or alias
		 * accounts to check for the key.  
		 * Return the save_status, if bad.  Otherwise,
		 * assume the inability to find a key that decrypts
		 * means the key used to encrypt was bad -->
		 * translate error to bad_integrity.
		 */
                if (!BAD_STATUS(&save_status)) {
		    SET_STATUS(&save_status, sec_rgy_bad_integrity);
                }
		done = true;
            } else {
		/* Any other bad status should be saved.
                 * Loop again to try next alias for key.
		 */
		COPY_STATUS(stp, &save_status);
	    }
        } else {
            get_key(transmit_rep, ch, new_key, stp);
         
            /* done with the encryption context regardless */
            sec_crypt_destroy_context(&ch);

            if (GOOD_STATUS(stp)) {
                CLEAR_STATUS(&save_status);
                done = true;
            }
            else {
		/* Will restore this status if all accounts fail */
		COPY_STATUS(stp, &save_status);
                sec_crypt_free_key(princ_key);
            }
        }
    }  /* while */

    if (BAD_STATUS(&save_status)) {
	COPY_STATUS(&save_status, stp);
    }
}


/*
 * r s _ p w d _ g e t _ k e y _ b y _ s e s s i o n _ k e y 
 *
 * Retrieve a password rec from the pickled network rep using the 
 * session key supplied by the caller
 */
PUBLIC void rs_pwd_get_key_by_session_key
    ( transmit_rep, session_key, new_key, stp )
    rs_acct_key_transmit_t     *transmit_rep;  /* [in] */
    sec_passwd_rec_t           *session_key;   /* [in] */
    sec_passwd_rec_t           *new_key;       /* [out] */
    error_status_t             *stp;
{
    sec_crypt_handle_t         ch;

    CLEAR_STATUS(stp);

    if (transmit_rep == NULL) {
        dce_svc_printf(SEC_RS_PWD_BOGUS_PICKLE_MSG);
    }

    ch = sec_crypt_create_context(session_key, transmit_rep->enc_type, stp);
    if (GOOD_STATUS(stp)) {
        get_key(transmit_rep, ch, new_key, stp);
	sec_crypt_destroy_context(&ch);
    }
}


/*
 *  rs_pwd_unix_pwd_to_xmit
 *
 * Generate a pickled network rep from a unix password
 * using the "encryption_key" argument.
 */
PUBLIC void rs_pwd_unix_pwd_to_xmit
  (
    sec_passwd_rec_t           *encryption_key,
    sec_rgy_unix_passwd_buf_t  unix_passwd,    
    rs_acct_key_transmit_t     **xmit_rep,     
    error_status_t             *stp
  )
{
    sec_passwd_rec_t           passwd;

    CLEAR_STATUS(stp);
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
        SET_STATUS(stp, sec_s_no_memory);
        return;
    }
    u_strcpy(PLAIN_PASSWD(&passwd), unix_passwd);
    rca_pwd_gen_transmit_rep(NULL, encryption_key, 
                             &passwd, xmit_rep, stp);
    sec_crypt_free_key_contents(&passwd);
}


/*
 *  rs_pwd_unix_pwd_fr_xmit
 *
 * Retrieve a unix password from its pickled network rep.
 * Perform the decryption using the "encryption_key" 
 * argument.
 */
PUBLIC void rs_pwd_unix_pwd_fr_xmit
  (
    rs_acct_key_transmit_t      *xmit_rep,        /* [in] */
    sec_passwd_rec_t            *encryption_key,
    sec_rgy_unix_passwd_buf_t   unix_passwd,      /* [out] */
    error_status_t              *stp
  ) 
{
    sec_passwd_rec_t  passwd;

    CLEAR_STATUS(stp);
    memset(&passwd, 0, sizeof(passwd));

    rs_pwd_get_key_by_session_key(xmit_rep, encryption_key, 
                                  &passwd, stp);
    
    if (GOOD_STATUS(stp)) {
        rs_pwd_to_unix_pwd(&passwd, unix_passwd, stp);
    }
}


/*
 * Convert a generic sec_passwd to a unix_passwd_buffer
 */
PUBLIC void rs_pwd_to_unix_pwd
  (
    sec_passwd_rec_t            *passwd,
    sec_rgy_unix_passwd_buf_t   unix_passwd,      /* [out] */
    error_status_t              *stp
  )
{
    CLEAR_STATUS(stp);

    if (PASSWD_TYPE(passwd) != sec_passwd_plain) {
        SET_STATUS(stp, sec_rgy_key_bad_type);
    } else {
        u_strncpy(unix_passwd, PLAIN_PASSWD(passwd),
                  sizeof(sec_rgy_unix_passwd_buf_t));
        unix_passwd[sizeof(sec_rgy_unix_passwd_buf_t) -1] = '\0';
    }
}
