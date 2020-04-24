/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: kdc_preauth.c,v $
 * Revision 1.1.13.5  1996/11/13  18:06:07  arvind
 * 	CHFts20619: preserve any KDC_ERR_S_PRINCIPAL_UNKNOWN error from
 * 	check_padata(), since this is an indicator of an intercell login.
 * 	[1996/11/05  18:35 UTC  aha  /main/DCE_1.2.2/aha_pk11/2]
 *
 * 	Fix botched merge in verify_tp_request()
 * 	[1996/11/01  15:06 UTC  aha  /main/DCE_1.2.2/aha_pk11/1]
 *
 * 	Fix botched merge in verify_tp_request() that causes secd to
 * 	core-dump
 * 	[1996/10/31  21:09 UTC  aha  /main/DCE_1.2.2/8]
 *
 * 	CHFts20544: secd SEGV during security API tests
 * 	[1996/10/29  16:04 UTC  aha  /main/DCE_1.2.2/7]
 *
 * 	Fix server side of Third-Party preauth
 * 	[1996/10/28  21:39 UTC  aha  /main/DCE_1.2.2/6]
 *
 * 	CHFts20574: free scratch in get_etype_info to fix MLK
 * 	[1996/10/29  15:53 UTC  aha  /main/DCE_1.2.2/aha_pk10/28]
 *
 * 	Eliminate the retrieval of the client's key in verify_tp_request(),
 * 	since the new client_enc_key parameter already contains the client's
 * 	encryption key.
 * 	[1996/10/29  15:31 UTC  aha  /main/DCE_1.2.2/aha_pk10/27]
 *
 * 	Initialize length of salt in KRB5_PADATA_ETYPE_INFO to 0 rather than
 * 	to -1.
 * 	[1996/10/28  20:11 UTC  aha  /main/DCE_1.2.2/aha_pk10/26]
 *
 * 	Change signature on check_padata so it can handle a reply key
 * 	[1996/10/25  20:13 UTC  aha  /main/DCE_1.2.2/aha_pk10/25]
 *
 * 	Fix SEGV in return_pw_salt()
 * 	[1996/10/25  14:23 UTC  aha  /main/DCE_1.2.2/aha_pk10/24]
 *
 * 	Simplify the verification of padata against pre_auth_req flags
 * 	[1996/10/25  13:42 UTC  aha  /main/DCE_1.2.2/aha_pk10/23]
 *
 * 	Fix unnecessary diffs
 * 	[1996/10/24  20:26 UTC  aha  /main/DCE_1.2.2/aha_pk10/22]
 *
 * 	Always return good status from generate_tp_response() -
 * 	verify_tp_request and check_data will handle the case of
 * 	an invalid padata.  Also, store new encrypting_key
 * 	correctly.
 * 	[1996/10/24  16:53 UTC  aha  /main/DCE_1.2.2/aha_pk10/21]
 *
 * 	In verify_tp_request, remove encrypting_key
 * 	[1996/10/24  15:54 UTC  aha  /main/DCE_1.2.2/aha_pk10/20]
 *
 * 	Remove error code from good path in check_padata(); add
 * 	verify_tp_request()
 * 	[1996/10/24  15:34 UTC  aha  /main/DCE_1.2.2/aha_pk10/19]
 *
 * 	More preauth verification work
 * 	[1996/10/23  21:03 UTC  aha  /main/DCE_1.2.2/aha_pk10/18]
 *
 * 	Initialize retval in check_padata()
 * 	[1996/10/23  20:28 UTC  aha  /main/DCE_1.2.2/aha_pk10/17]
 *
 * 	Add KRB5_PADATA_ENC_TIMESTAMP as allowed for UNIX_TIME_ALLOWED_PADATA
 * 	[1996/10/23  20:00 UTC  aha  /main/DCE_1.2.2/aha_pk10/16]
 *
 * 	remove get_preauth_supported_and_allowed call
 * 	[1996/10/23  19:15 UTC  aha  /main/DCE_1.2.2/aha_pk10/15]
 *
 * 	Merge verify_preauth_policy() with check_padata()
 * 	[1996/10/23  19:05 UTC  aha  /main/DCE_1.2.2/aha_pk10/14]
 *
 * 	Include all PADATA types sent in verify_preauth_policy()
 * 	[1996/10/23  17:48 UTC  aha  /main/DCE_1.2.2/aha_pk10/13]
 *
 * 	Fix bad free in verify_enc_timestamp()
 * 	[1996/10/23  14:44 UTC  aha  /main/DCE_1.2.2/aha_pk10/12]
 *
 * 	Cleanup.
 * 	[1996/10/22  19:01 UTC  aha  /main/DCE_1.2.2/aha_pk10/11]
 *
 * 	Fix handling of errcode in prepare_enc_tp_error_edata()
 * 	[1996/10/21  20:22 UTC  aha  /main/DCE_1.2.2/aha_pk10/10]
 *
 * 	Remove PA_REQUIRED from KRB5_PADATA_ROOT_CERT preauth_systems entry.
 * 	This flag prevents a fallback to OSF_DCE even in cases where client
 * 	is allowed to fall back.
 * 	[1996/10/21  18:29 UTC  aha  /main/DCE_1.2.2/aha_pk10/9]
 *
 * 	Fix clean0 in generate_tp_response(); reset errcode only if not 0
 * 	[1996/10/21  17:02 UTC  aha  /main/DCE_1.2.2/aha_pk10/8]
 *
 * 	Fix data initialization in tp_edata
 * 	[1996/10/21  15:17 UTC  aha  /main/DCE_1.2.2/aha_pk10/7]
 *
 * 	prepare_enc_tp_error_edata has to call krb5_mk_error().
 * 	[1996/10/21  14:46 UTC  aha  /main/DCE_1.2.2/aha_pk10/6]
 *
 * 	Fixes to generate_tp_response()
 * 	[1996/10/18  20:22 UTC  aha  /main/DCE_1.2.2/aha_pk10/5]
 *
 * 	3pPA work; third pass
 * 	[1996/10/17  21:21 UTC  aha  /main/DCE_1.2.2/aha_pk10/4]
 *
 * 	Fix generate_tp_response()
 * 	[1996/10/17  14:38 UTC  aha  /main/DCE_1.2.2/aha_pk10/3]
 *
 * 	Fix build errors
 * 	[1996/10/16  14:39 UTC  aha  /main/DCE_1.2.2/aha_pk10/2]
 *
 * 	Make third-party preauthentication work again; third pass
 * 	[1996/10/15  15:45 UTC  aha  /main/DCE_1.2.2/aha_pk9_5/8]
 *
 * 	CHFts20121: make third-party preauthentication work under the KRB5
 * 	Beta 6 preauth framework; second pass
 * 	[1996/10/14  17:10 UTC  aha  /main/DCE_1.2.2/aha_p
 *
 * Revision 1.1.13.4  1996/10/15  20:51:14  arvind
 * 	Fix bug in handling of KRB_DIR_ROOT (strncpy does not leave its result
 * 	null-terminated if the string is not shorter than the length
 * 	parameter).
 * 	[1996/10/11  00:11 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_2/4]
 * 
 * 	CHFts20158: free checksum.
 * 	Probably has other memory leaks.
 * 	[1996/10/04  22:13 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_2/3]
 * 
 * 	fix memory leak.
 * 	[1996/10/03  19:14 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_2/2]
 * 
 * Revision 1.1.13.3  1996/10/04  16:58:13  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/04  16:57:56  arvind]
 * 
 * Revision /main/DCE_1.2.2/3  1996/10/03  19:02 UTC  sommerfeld
 * 	CHFts20141: don't be a dummy
 * 	[1996/10/03  19:01 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_2/1]
 * 
 * Revision /main/DCE_1.2.2/2  1996/09/26  23:30 UTC  sommerfeld
 * 	fix context arg to krb5_free_etype_info.
 * 	[1996/09/17  05:53 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_1/2]
 * 
 * 	Pick up new preauth module interface from KRB5beta6 (sort of).
 * 	plug pubkey preauth into it.
 * 	[1996/09/17  00:23 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_1/1]
 * 
 * Revision /main/DCE_1.2.2/1  1996/06/11  17:58 UTC  aha
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 * 
 * Revision /main/DCE122_PK/1  1996/06/04  18:52 UTC  psn
 * 	Update OSF copyright years
 * 	[1996/02/17  23:00:05  marty]
 * 	 *
 * 
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:57:35  root]
 * 
 * Revision /main/DCE_1.2/DCE12_PK/1  1996/05/30  21:54 UTC  sommerfeld
 * 	Flush realm compare, realm swap, leave a comment behind..
 * 	[1996/05/02  19:27 UTC  sommerfeld  /main/HPDCE02/sommerfeld_CHFts17874/3]
 * 
 * Revision 1.1.2.8  1994/08/26  18:43:53  mullan_s
 * 	[OT11557] Subtype DCE preauth data.
 * 	[1994/08/25  18:31:12  mullan_s]
 * 
 * Revision 1.1.2.7  1994/08/04  16:13:08  mdf
 * 	Init locally declared ptrs to NULL (OT 11422)
 * 	[1994/08/01  12:45:35  mdf]
 * 
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:10:22  mdf]
 * 
 * Revision 1.1.2.5  1994/07/21  20:16:44  baum
 * 	Removed bad free() OT 11363
 * 	[1994/07/21  20:15:54  baum]
 * 
 * Revision 1.1.2.4  1994/07/19  21:40:00  baum
 * 	Removed & from free(&my_realm.data), OT 11342
 * 	[1994/07/19  21:39:42  baum]
 * 
 * Revision 1.1.2.3  1994/07/15  15:00:31  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:17:51  mdf]
 * 
 * Revision 1.1.2.2  1994/06/10  16:58:30  annie
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  16:58:16  annie]
 * 
 * Revision 1.1.2.1  1994/06/02  21:28:55  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:16:54  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * $EndLog$
 */
/*
**
** Copyright (c) Hewlett-Packard Company 1994, 1995, 1996
** Unpublished work. All Rights Reserved.
**
*/
#ifdef OSF_DCE
#define OSF_DCE_FEATURE
#define OSF_DCE_ARCHAISM
#endif

#include <dce/nbase.h>
#include <krb5/krb5.h>
#include <krb5/kdb.h>
#include <krb5/free.h>
#include <krb5/encryption.h>
#include <krb5/preauth.h>
#include <krb5/los-proto.h>
#include <krb5/asn1.h>
#include <krb5/osconf.h>
#include <com_err.h>
#include <syslog.h>
#include "kdc_util.h"
#include "extern.h"

#include <krb5/ext-proto.h>

#define in_clock_skew(date) (abs((date)-currenttime) < krb5_clockskew)

#ifdef OSF_DCE_FEATURE
/* krb5_etype_info_entry and krb5_etype_info are defined in krb5.h */

#define KRB5_PADATA_ROOT_CERT_FLAG     (1<<KRB5_PADATA_ROOT_CERT)
#define KRB5_PADATA_OSF_DCE_FLAG       (1<<KRB5_PADATA_OSF_DCE)
#define KRB5_PADATA_ENC_UNIX_TIME_FLAG (1<<KRB5_PADATA_ENC_UNIX_TIME)
#define KRB5_PADATA_ENC_TIMESTAMP_FLAG (1<<KRB5_PADATA_ENC_TIMESTAMP)
#define KRB5_PADATA_NONE_FLAG          (1<<KRB5_PADATA_NONE)
#define ROOT_CERT_ALLOWED_PADATA KRB5_PADATA_ROOT_CERT_FLAG
#define OSF_DCE_ALLOWED_PADATA (ROOT_CERT_ALLOWED_PADATA | KRB5_PADATA_OSF_DCE_FLAG)
#define TIMESTAMPS_ALLOWED_PADATA (OSF_DCE_ALLOWED_PADATA | KRB5_PADATA_ENC_UNIX_TIME_FLAG | KRB5_PADATA_ENC_TIMESTAMP_FLAG)
#define ANY_ALLOWED_PADATA (TIMESTAMPS_ALLOWED_PADATA | KRB5_PADATA_NONE_FLAG)
#endif /* OSF_DCE_FEATURE */

extern krb5_deltat krb5_clockskew;
extern krb5_encrypt_block master_encblock;

krb5_error_code verify_timestamp(time)
krb5_timestamp      time;
{
    krb5_error_code     retval;
    krb5_timestamp      currenttime, patime;


    retval = krb5_timeofday(&currenttime);
    if (retval)
        return retval;

    if (!in_clock_skew(time))
        return KRB5KRB_AP_ERR_SKEW;

    return 0;

}

#ifdef OSF_DCE_FEATURE /* third-party preauth */
/*
****************************************************
**
** OSF DCE THIRD-PARTY PRE-AUTHENTICATION
**
** verify_tp_request contains former third_party_padata()
** logic.
****************************************************
*/

/* verify_tp_request
 */
static krb5_error_code
verify_tp_request (context, client, request, client_enc_key, enc_tkt_reply, pa)
    krb5_context        context;
    krb5_db_entry       *client;
    krb5_kdc_req        *request;
    krb5_keyblock       *client_enc_key;
    krb5_enc_tkt_part   *enc_tkt_reply;
    krb5_pa_data        *pa;
{
    krb5_error_code     errcode=0;
    krb5_data           preauth;
    krb5_sub_pa_data    *dce_padata=NULL;
    krb5_kvno           kvno;
    krb5_data           in;
    krb5_data           et;
    krb5_data           random_key;
    krb5_data           ts_reply;
    krb5_data           ts_reply_clear;
    krb5_keyblock       user_key;
    krb5_encrypt_block  eblock;
    krb5_keyblock       *machine_session_key=NULL;
    krb5_pa_third_party *decoded_padata=NULL;
    krb5_enc_data       *decoded_ts_reply=NULL;
    krb5_enc_data       *decoded_random_key=NULL;
    krb5_enc_data       *decoded_user_info=NULL;
    krb5_keyblock       *clear_random_key=NULL;
    krb5_time_and_key   *clear=NULL;
    krb5_ticket         *ticket =NULL;
    static krb5_data    my_realm = {0, NULL};

    /*
     * First we must decode the padata into a sub-padata structure
     * which will tell us the subtype of the OSF DCE padata
     * contained within.
     */
    preauth.length = pa->length;
    preauth.data = (char *)(pa->contents);
    if (errcode = decode_krb5_sub_pa_data(&preauth, &dce_padata))
        return(KDC_PREAUTH_FAILED);
    if ((dce_padata->pa_subtype) != OSF_DCE_THIRD_PARTY) {
        errcode = KRB5_PREAUTH_BAD_TYPE;
        goto clean;
    }

#define cleanup_key(key) { \
    if (key.contents) { \
        memset((char *)(key.contents), 0, key.length); \
        free(key.contents); \
    } \
}

    /* 
     * initialize variables
     */
    memset((char*)&ts_reply_clear,0,sizeof(krb5_data));
    memset((char*)&ts_reply,0,sizeof(krb5_data));
    memset((char*)&random_key,0,sizeof(krb5_data));
    memset((char*)&user_key,0,sizeof(krb5_keyblock));
    eblock.priv = NULL;

    /* 
     * padata is in the following format 
     * 	machine_tgt
     * 	(random-key)machine_session_key
     *      ((timestamp,encrypting_key)user-key)random_key
     */

    /* 
     * first the whole padata is encoded, so decode it 
     */

    in.length = dce_padata->length;
    in.data = (char *) dce_padata->contents;
    if (errcode = decode_krb5_pa_third_party(&in,&decoded_padata) ) 
        goto clean0;

    /* 
     * the first field in the decoded padata is the machine ticket 
     *         and it is also encoded, so decode it 
     */
    if (errcode = decode_krb5_ticket(decoded_padata->machine_tgt_data,
                                    &ticket)) 
        goto clean0;

    /*
     * If this is a cross-realm request, we need to adjust the server
     * principal and the realm.
     */
    if (my_realm.data == NULL) {
        if (errcode = krb5_get_default_realm(&my_realm.data))
            goto clean0;
        my_realm.length = strlen(my_realm.data);
    }

    /*
     * we used to do a "kdc_swap_foreign_tgs_name" here, but
     * this is now handled inside kdc_get_server_key
     */

    /*
     * to do the next phase, decryption of the random key,
     * we need to have the machine_session_key  
     * so we use the ticket to get the machine session key
     */
    if (errcode = kdc_get_server_key(ticket, &machine_session_key, &kvno)) {
        /*
         * For transitive trust:
         * The cell princ won't have an account/key,
         * so convert error to internal form for error handling.
         */
        if (krb5_is_tgs_principal(ticket->server)) {
            errcode = KDC_ERR_S_PRINCIPAL_UNKNOWN;
        }
        goto clean0;
    }

    if (errcode = krb5_decrypt_tkt_part(machine_session_key,ticket))
        goto clean0;

    /* 
     * the second field in the decoded_padata is the random key
     * and it is encoded and encrypted so first we need to decode it 
     */

    et.data=(char *)decoded_padata->single_ebuf_data->data;
    et.length=decoded_padata->single_ebuf_data->length;
    if (errcode=decode_krb5_enc_data(&et, &decoded_random_key)) 
        goto clean0;

    /* 
     * now we need to decrypt the random key using the machine session key
     * the function requires the use of an "eblock" so we construct one
     *      by taking keytype from the decrypted ticket
     *	and put the machine session key in the eblock
     */
    krb5_use_keytype(&eblock, ticket->enc_part2->session->keytype);
    if (errcode = krb5_process_key(&eblock, ticket->enc_part2->session))
        goto clean1;

    /* 
     * now we can actually decrypt the random key 
     * the function arguments are...
     * 	krb5_decrypt(in, out, size, eblock, ivec)
     */
    random_key.length=decoded_random_key->ciphertext.length;
    if (!(random_key.data =
          malloc(decoded_random_key->ciphertext.length))) {
        errcode=ENOMEM;
        goto clean1;
    }

    if (errcode=krb5_decrypt(decoded_random_key->ciphertext.data,
               random_key.data, random_key.length, &eblock, 0)) 
        goto clean1;

    (void) krb5_finish_key(&eblock);

    /* 
     * The key block for the random key is encoded again.....
     * so decode it
     */
    if (errcode=decode_krb5_keyblock(&random_key, &clear_random_key))
        goto clean1;

    /* 
     * Phew! now we have the clear_random_key.....
     * Now we can start all over again on the last field of the padata.....
     *   "enc_user_info" the timestamp and reply random key
     * 
     */

    if (errcode=decode_krb5_enc_data(decoded_padata->double_ebuf_data,
               &decoded_user_info)) 
        goto clean1;

    /* 
     *  The result of the decoding is "doubly" encrypted.
     *  First in the random key
     *  the function requires the use of an "eblock" so we construct one
     *      take etype from the encoded data string
     *		the krb5_use_cstype() resets the cryptosystem
     *		pointer to the right entry for this etype
     *		(so we can reuse this structure if we reset this)
     *	put the machine session key in the eblock
     */

    krb5_use_cstype(&eblock, decoded_user_info->etype);
    if (errcode = krb5_process_key(&eblock, clear_random_key))
        goto clean1;

    ts_reply.length =decoded_user_info->ciphertext.length;
    if (!(ts_reply.data =
          malloc(decoded_user_info->ciphertext.length))) {
        errcode=ENOMEM;
        goto clean2;
    }

    if (errcode=krb5_decrypt(decoded_user_info->ciphertext.data, 
               ts_reply.data, ts_reply.length, &eblock, 0))  
        goto clean2;

    (void) krb5_finish_key(&eblock);


    /* 
     * This block is encoded again.....
     */
    if (errcode=decode_krb5_enc_data(&ts_reply, &decoded_ts_reply))
        goto clean2;

    /* The client_enc_key is the derived user's key, so we no longer
     * retrieve it from the database here.  Note that this will not
     * work if another padata type also results in a changed
     * "client_enc_key"
     */

    /* 
     *  Next use the derived user's key
     *  the function requires the use of an "eblock" so we reuse the eblock
     *      take etype from the encoded data string
     *		the krb5_use_cstype() resets the cryptosystem
     *		pointer to the right entry for this etype
     *		(so we can reuse this structure if we reset this)
     *	put the machine session key in the eblock
     */
    krb5_use_cstype(&eblock, decoded_ts_reply->etype);
    if (errcode = krb5_process_key(&eblock, client_enc_key)) 
        goto clean3;

    ts_reply_clear.length = decoded_ts_reply->ciphertext.length;
    if (!(ts_reply_clear.data =
          malloc(decoded_ts_reply->ciphertext.length))) {
        errcode=ENOMEM;
        goto clean3;
    }

    if (errcode=krb5_decrypt(decoded_ts_reply->ciphertext.data, 
                            ts_reply_clear.data,
                            ts_reply_clear.length, &eblock, 0)) {
        goto clean4;
    }

    /*  DECODE */

    if (errcode = decode_krb5_time_and_key(&ts_reply_clear,&clear))
        goto clean4;

    /* 
     * using existing kerberos timestamp
     * check function but it has 2 extraneous arguments, 
     * client->principal and src_addrs
     * because its called through an operations epv
     * these arguments are never used 
     * in this function so we fake it here
     */
    if (errcode= (verify_timestamp(clear->timestamp)) ) 
        goto clean4;

    /* Clear existing client_enc_key contents */
    memset(client_enc_key->contents, 0, client_enc_key->length);
    xfree(client_enc_key->contents);

    /* Put new key into client_enc_key */
    *client_enc_key = clear->key;

clean4:
    if (clear !=NULL) {
        if (errcode)
            cleanup_key(clear->key);
        free(clear);
    }
    if (ts_reply_clear.data) 
        free(ts_reply_clear.data);
clean3:
    cleanup_key(user_key);
clean2:
    if (decoded_ts_reply) {
        if (decoded_ts_reply->ciphertext.data) 
            xfree(decoded_ts_reply->ciphertext.data);
        free(decoded_ts_reply);
    }
    if (ts_reply.data) 
        free(ts_reply.data);
clean1:
    if (eblock.priv != NULL)
        (void) krb5_finish_key(&eblock);
    if (clear_random_key)
        krb5_free_keyblock(clear_random_key);
    if (decoded_random_key) {
        if (decoded_random_key->ciphertext.data) 
            xfree(decoded_random_key->ciphertext.data);
        free(decoded_random_key);
    }
    if (decoded_user_info) {
        if (decoded_user_info->ciphertext.data) 
            xfree(decoded_user_info->ciphertext.data);
        free(decoded_user_info);
    }
    if (random_key.data)
        free(random_key.data);
clean0:
    if (machine_session_key)
        krb5_free_keyblock(machine_session_key);
    if (ticket)
        krb5_free_ticket(ticket); 
    if (decoded_padata) {
        if (decoded_padata->machine_tgt_data->data)
            krb5_free_data(decoded_padata->machine_tgt_data);
        if (decoded_padata->single_ebuf_data->data)
            krb5_free_data(decoded_padata->single_ebuf_data);
        if (decoded_padata->double_ebuf_data->data)
            krb5_free_data(decoded_padata->double_ebuf_data);
        free(decoded_padata);
    }
clean:
    if (dce_padata) {
        if (dce_padata->contents)
            free(dce_padata->contents);
        free(dce_padata);
    }
    return errcode;

}

#endif /* OSF_DCE_FEATURE third-party preauth */


#ifdef OSF_DCE_ARCHAISM
/* prepare_enc_tp_error_edata
 *
 * Note that this can not be called as a (*edata_proc)
 * because DCE 1.1 returned output from encode_krb5_sub_err_data()
 * as the errpkt.e_data, and not as one of a sequence of
 * krb5_pa_data structures.
 */
krb5_error_code
prepare_enc_tp_error_edata (request, error, sub_err, e_data, client)
register krb5_kdc_req *request;
int error, sub_err;
krb5_data *e_data;
krb5_db_entry  *client;
{
    krb5_data *encoded_dce_err_data;
    krb5_error_code retval;
    krb5_sub_err_data dce_err_data;

#if defined(DCE_AUD_POINTS)
    /*
     * Handle the "error" audit conditions here.
     */

    dce_aud_start(AS_Request, h, 0, aud_c_esl_cond_failure,
                  &ard, &aud_status);

    /* No event specific info associated with a AS_REQUEST event. */

    if (ard) {
        dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1,
                       aud_c_esl_cond_failure, &aud_status);
    }
#endif  /* defined(DCE_AUD_POINTS) */

    /* Initialize pointers */
    e_data->length=0;
    e_data->data=NULL;
    retval=0;

    /* Make OSF DCE adjustments to errcode and sub_err
     * for backwards compatibility
     */
    if (error == KDC_ERR_S_PRINCIPAL_UNKNOWN) {
        error = KDC_PREAUTH_FAILED;
        sub_err = OSF_DCE_ERR_PREAUTH_INTERCELL;
    } else if (error == KDC_ERR_POLICY) {
        sub_err = OSF_DCE_ERR_ADMIN_VALID;
    } else {
        sub_err = OSF_DCE_ERR_PREAUTH_SALT;
    }
    
    dce_err_data.err_subtype = sub_err;

    switch (client->salt_type) {
	krb5_data *data_foo, data_bar;

    case KRB5_KDB_SALTTYPE_V4:
	return error_status_ok;

    case KRB5_KDB_SALTTYPE_NOREALM:
	if (retval = norealm_salt(request->client, &data_bar))
	{
	    return retval;
	}
	dce_err_data.length = data_bar.length;
	dce_err_data.contents = (krb5_octet *) data_bar.data;
	retval = encode_krb5_sub_err_data(&dce_err_data, &encoded_dce_err_data);
	free(data_bar.data);
	break;
    case KRB5_KDB_SALTTYPE_ONLYREALM:
	data_foo = krb5_princ_realm(request->client);
	dce_err_data.length = data_foo->length;
	dce_err_data.contents = (krb5_octet *) data_foo->data;
	retval = encode_krb5_sub_err_data(&dce_err_data, &encoded_dce_err_data);
	krb5_free_data(data_foo);
	break;
    case KRB5_KDB_SALTTYPE_SPECIAL:
	dce_err_data.length = client->salt_length;
	dce_err_data.contents = client->salt;
	retval = encode_krb5_sub_err_data(&dce_err_data, &encoded_dce_err_data);
	break;
    default:
	dce_err_data.length = 0;
	dce_err_data.contents = NULL;
	retval = encode_krb5_sub_err_data(&dce_err_data, &encoded_dce_err_data);
	break;
    }
    if (!retval) {
        e_data->length=encoded_dce_err_data->length;
        e_data->data=encoded_dce_err_data->data;
    }
    free(encoded_dce_err_data);
    return retval;
}
#endif

/* verify is called to verify incoming preauth data.
   return_proc is used to generate outgoing preauth data.
   edata is used to send salts, etc., to the client
   */
   

#ifdef OSF_DCE_FEATURE
typedef krb5_error_code (*verify_proc)
    (krb5_context, krb5_db_entry *client,
		    krb5_kdc_req *request,
                    krb5_keyblock *client_enc_key,
		    krb5_enc_tkt_part * enc_tkt_reply, krb5_pa_data *data);
#else
typedef krb5_error_code (*verify_proc)
    (krb5_context, krb5_db_entry *client,
		    krb5_kdc_req *request,
		    krb5_enc_tkt_part * enc_tkt_reply, krb5_pa_data *data);
#endif

typedef krb5_error_code (*edata_proc)
    (krb5_context, krb5_kdc_req *request,
		    krb5_db_entry *client, krb5_db_entry *server,
		    krb5_pa_data *data);

typedef krb5_error_code (*return_proc)
    (krb5_context, krb5_pa_data * padata, 
		    krb5_db_entry *client,
		    krb5_kdc_req *request, krb5_kdc_rep *reply,
		    krb5_key_data *client_key,
		    krb5_keyblock *encrypting_key,
		    krb5_pa_data **send_pa);

typedef struct _krb5_preauth_systems {
    int		type;
    int		flags;
    edata_proc	get_edata;
    verify_proc	verify_padata;
    return_proc return_padata;
} krb5_preauth_systems;

#ifdef OSF_DCE_FEATURE
static krb5_error_code verify_enc_timestamp
    (krb5_context, krb5_db_entry *client,
		    krb5_kdc_req *request,
                    krb5_keyblock *client_enc_key,
		    krb5_enc_tkt_part * enc_tkt_reply, krb5_pa_data *data);
#else
static krb5_error_code verify_enc_timestamp
    (krb5_context, krb5_db_entry *client,
		    krb5_kdc_req *request,
		    krb5_enc_tkt_part * enc_tkt_reply, krb5_pa_data *data);
#endif

#ifdef OSF_DCE_ARCHAISM
static krb5_error_code verify_enc_unix_time
    (krb5_context, krb5_db_entry *client,
		    krb5_kdc_req *request,
                    krb5_keyblock *client_enc_key,
		    krb5_enc_tkt_part * enc_tkt_reply, krb5_pa_data *data);

#endif /* OSF_DCE_ARCHAISM */
static krb5_error_code get_etype_info
    (krb5_context, krb5_kdc_req *request,
		    krb5_db_entry *client, krb5_db_entry *server,
		    krb5_pa_data *data);
static krb5_error_code return_pw_salt
    (krb5_context, krb5_pa_data * padata, 
		    krb5_db_entry *client,
		    krb5_kdc_req *request, krb5_kdc_rep *reply,
		    krb5_key_data *client_key,
		    krb5_keyblock *encrypting_key,
		    krb5_pa_data **send_pa);

/* SAM preauth support */
static krb5_error_code verify_sam_response
    (krb5_context, krb5_db_entry *client,
		    krb5_kdc_req *request,
		    krb5_enc_tkt_part * enc_tkt_reply, krb5_pa_data *data);

static krb5_error_code get_sam_edata
    (krb5_context, krb5_kdc_req *request,
		    krb5_db_entry *client, krb5_db_entry *server,
		    krb5_pa_data *data);

#ifdef OSF_DCE_FEATURE
/*
 * public key preauth support
 */
static krb5_error_code
    verify_pk_request (krb5_context, krb5_db_entry *client,
		       krb5_kdc_req *request,
                       krb5_keyblock *client_enc_key,
		       krb5_enc_tkt_part * enc_tkt_reply,
		       krb5_pa_data *data);

static krb5_error_code
    generate_pk_response (krb5_context, krb5_pa_data * padata, 
			  krb5_db_entry *client,
			  krb5_kdc_req *request,
			  krb5_kdc_rep *reply,
			  krb5_key_data *client_key,
			  krb5_keyblock *encrypting_key,
			  krb5_pa_data **send_pa);
#endif /* OSF_DCE_FEATURE public key preauth support */

/*
 * Preauth property flags
 */
#define PA_HARDWARE	0x00000001
#define PA_REQUIRED	0x00000002
#define PA_SUFFICIENT	0x00000004

static krb5_preauth_systems preauth_systems[] = {
    {
        KRB5_PADATA_ENC_TIMESTAMP,
        0,
        0,
	verify_enc_timestamp,
	0
    },
#ifdef OSF_DCE_ARCHAISM
    {
        KRB5_PADATA_ENC_UNIX_TIME,
        0,
        0,
	verify_enc_unix_time,
	0
    },
#endif /* OSF_DCE_ARCHAISM */
#ifdef OSF_DCE_FEATURE
    {
	KRB5_PADATA_OSF_DCE,
        0,
	0,
	verify_tp_request,
	0,
    },
#endif
    {
	KRB5_PADATA_ETYPE_INFO,
	0,
	get_etype_info,
	0,
	0
    },
    {
	KRB5_PADATA_PW_SALT,
	0,
	0, 
	0,
	return_pw_salt
    },
#ifdef OSF_DCE_FEATURE
    {
	KRB5_PADATA_ROOT_CERT,
	PA_SUFFICIENT,               /* if present and valid, is sufficient */
	0,
	verify_pk_request,
	generate_pk_response
    },
#endif
#if 0
    {
	KRB5_PADATA_SAM_RESPONSE,
	0,
	0,
	verify_sam_response,
	0
    },
    {
	KRB5_PADATA_SAM_CHALLENGE,
	PA_HARDWARE,		/* causes get_preauth_hint_list to use this */
	get_sam_edata,
	0,
	0
    },
#endif
    { -1,}
};

#define MAX_PREAUTH_SYSTEMS (sizeof(preauth_systems)/sizeof(preauth_systems[0]))

static krb5_error_code
find_pa_system(type, preauth)
    int			type;
    krb5_preauth_systems	**preauth;
{
    krb5_preauth_systems 	*ap = preauth_systems;
    
    while ((ap->type != -1) && (ap->type != type))
	ap++;
    if (ap->type == -1)
	return(KRB5_PREAUTH_BAD_TYPE);
    *preauth = ap;
    return 0;
} 

const char *missing_required_preauth(client, server, enc_tkt_reply)
    krb5_db_entry *client, *server;
    krb5_enc_tkt_part *enc_tkt_reply;
{
#if 0
    /*
     * If this is the pwchange service, and the pre-auth bit is set,
     * allow it even if the HW preauth would normally be required.
     * 
     * Sandia national labs wanted this for some strange reason... we
     * leave it disabled normally.
     */
    if (isflagset(server->attributes, KRB5_KDB_PWCHANGE_SERVICE) &&
	isflagset(enc_tkt_reply->flags, TKT_FLG_PRE_AUTH))
	return 0;
#endif
    
    if (isflagset(client->attributes, KRB5_KDB_REQUIRES_PRE_AUTH) &&
	 !isflagset(enc_tkt_reply->flags, TKT_FLG_PRE_AUTH))
	return "NEEDED_PREAUTH";
    
    if (isflagset(client->attributes, KRB5_KDB_REQUIRES_HW_AUTH) &&
	!isflagset(enc_tkt_reply->flags, TKT_FLG_HW_AUTH))
	return "NEEDED_HW_PREAUTH";

    return 0;
}

#ifdef OSF_DCE_FEATURE /* get_preauth_required() */
/* get_preauth_required
 *
 * Added since neither missing_required_preauth() nor
 * the PA_REQUIRED,PA_SUFFICIENT flags in
 * preauth_systems[] were sufficient to capture the
 * semantics of OSF DCE pre_auth_req ERA usage.  This
 * is called from check_padata() to determine any
 * padata types the user is required to have.  If more
 * than one type is allowed, then any of the allowed
 * types, if valid, is sufficient.
 */
unsigned32
get_preauth_required(krb5_db_entry *client)
{
    krb5_preauth_systems   *ap;

    /* Get set of PADATA types this client is allowed to use */
    if (isflagset(client->attributes, KRB5_KDB_PADATA_UNIX_TIME))
        return TIMESTAMPS_ALLOWED_PADATA;
    else if (isflagset(client->attributes, KRB5_KDB_PADATA_OSF_DCE))
        return OSF_DCE_ALLOWED_PADATA;
    else if (isflagset(client->attributes, KRB5_KDB_PADATA_ROOT_CERT))
        return ROOT_CERT_ALLOWED_PADATA;
    else
        return ANY_ALLOWED_PADATA;
}
#endif /* OSF_DCE_FEATURE get_preauth_required() */

/* get_preauth_hint_list()
 *
 * Runs through table of preauth_systems[] and calls
 * each get_edata() entry.
 */
void get_preauth_hint_list(request, client, server, e_data)
    krb5_kdc_req *request;
    krb5_db_entry *client, *server;
    krb5_data *e_data;
{
    int hw_only;
    krb5_preauth_systems *ap;
    krb5_pa_data **pa_data, **pa;
    krb5_data *edat;
    krb5_error_code retval;
#ifdef OSF_DCE_FEATURE
    unsigned32   allowed_and_supported;
#endif
    
    /* Zero these out in case we need to abort */
    e_data->length = 0;
    e_data->data = 0;
    
    hw_only = isflagset(client->attributes, KRB5_KDB_REQUIRES_HW_AUTH);
    pa_data = malloc(sizeof(krb5_pa_data *) * (MAX_PREAUTH_SYSTEMS+1));
    if (pa_data == 0)
	return;
    memset(pa_data, 0, sizeof(krb5_pa_data *) * (MAX_PREAUTH_SYSTEMS+1));
    pa = pa_data;

    for (ap = preauth_systems; ap->type != -1; ap++) {
	if (hw_only && !(ap->flags & PA_HARDWARE))
	    continue;
	*pa = malloc(sizeof(krb5_pa_data));
	if (*pa == 0)
	    goto errout;
	memset(*pa, 0, sizeof(krb5_pa_data));
#ifndef OSF_DCE_ARCHAISM
	(*pa)->magic = KV5M_PA_DATA;
#endif
	(*pa)->pa_type = ap->type;
	if (ap->get_edata)
	    (ap->get_edata)(kdc_context, request, client, server, *pa);
	pa++;
    }
    retval = encode_krb5_padata_sequence(pa_data, &edat);
    if (retval)
	goto errout;
    *e_data = *edat;
    free(edat);

errout:
    krb5_free_pa_data(/*kdc_context,*/ pa_data);
    return;
}

/*
 * This routine is called to verify the preauthentication information
 * for a V5 request.
 * 	
 * Returns 0 if the pre-authentication is valid, non-zero to indicate
 * an error code of some sort.
 *
 * For OSF DCE, check_padata also verifies that the client has
 * passed in at least one valid padata type of any required types.
 */

#ifdef OSF_DCE_FEATURE
krb5_error_code
check_padata (context, client, request, enc_tkt_reply, client_enc_key)
    krb5_context	context;
    krb5_db_entry *	client;
    krb5_kdc_req *	request;
    krb5_enc_tkt_part * enc_tkt_reply;
    krb5_keyblock *     client_enc_key;
#else
krb5_error_code
check_padata (context, client, request, enc_tkt_reply)
    krb5_context	context;
    krb5_db_entry *	client;
    krb5_kdc_req *	request;
    krb5_enc_tkt_part * enc_tkt_reply;
#endif
{
    krb5_error_code retval=0;
    krb5_pa_data **padata;
    krb5_preauth_systems *pa_sys;
#ifdef OSF_DCE_FEATURE
    unsigned32    pa_types_required;
    boolean32     pa_required_passed=FALSE;

    pa_types_required=get_preauth_required(client);
    if (pa_types_required & KRB5_PADATA_NONE_FLAG)
        pa_required_passed=TRUE;
    if (request->padata == 0)
        goto errout;
#else
    if (request->padata == 0)
	return 0;
#endif /* OSF_DCE_FEATURE get_preauth_required */

    for (padata = request->padata; *padata; padata++) {
	if (find_pa_system((*padata)->pa_type, &pa_sys))
	    continue;
	if (pa_sys->verify_padata == 0)
	    continue;
#ifdef OSF_DCE_FEATURE
	retval = pa_sys->verify_padata(context, client, request, client_enc_key,
				       enc_tkt_reply, *padata);
#else
	retval = pa_sys->verify_padata(context, client, request,
				       enc_tkt_reply, *padata);
#endif
	if (retval) {
	    if (pa_sys->flags & PA_REQUIRED)
		break;
	} else {
#ifdef OSF_DCE_FEATURE
            /* if padata type is one of those required for this user,
             * treat as PA_SUFFICIENT
             */
            if ((1<<((*padata)->pa_type)) & pa_types_required) {
                pa_required_passed=TRUE;
                break;
            }
#endif /* OSF_DCE_FEATURE */
	    if (pa_sys->flags & PA_SUFFICIENT)
		break;
	}
    }
errout:
#ifdef OSF_DCE_FEATURE
    if (!pa_required_passed || retval) {
        com_err("krb5kdc", retval, "pa verify failure");
        if (retval != KDC_ERR_S_PRINCIPAL_UNKNOWN)
            retval=KRB5KDC_ERR_PREAUTH_FAILED;
    }
#else
   if (retval) {
        com_err("krb5kdc", retval, "pa verify failure");
	retval = KRB5KDC_ERR_PREAUTH_FAILED;
    }
#endif /* OSF_DCE_FEATURE */
    return retval;
}

/*
 * return_padata creates any necessary preauthentication
 * structures which should be returned by the KDC to the client
 *
 * It calls the return_padata function for each supported
 * padata type; if the supported padata type matches one of
 * the padata types in the request, then the input padata
 * is passed to the return_padata function.
 */
krb5_error_code
return_padata(context, client, request, reply,
	      client_key, encrypting_key)
    krb5_context	context;
    krb5_db_entry *	client;
    krb5_kdc_req *	request;
    krb5_kdc_rep *	reply;
    krb5_key_data *	client_key;
    krb5_keyblock *	encrypting_key;
{
    krb5_error_code		retval;
    krb5_pa_data **		padata;
    krb5_pa_data **		send_pa_list;
    krb5_pa_data **		send_pa;
    krb5_pa_data *		pa = 0;
    krb5_preauth_systems *	ap;
    int 			size = 0;

    for (ap = preauth_systems; ap->type != -1; ap++) {
	if (ap->return_padata)
	    size++;
    }

    if ((send_pa_list = malloc((size+1) * sizeof(krb5_pa_data *))) == NULL)
	return ENOMEM;

    send_pa = send_pa_list;
    *send_pa = 0;
    
    for (ap = preauth_systems; ap->type != -1; ap++) {
	if (ap->return_padata == 0)
	    continue;
	pa = 0;
	if (request->padata) {
	    for (padata = request->padata; *padata; padata++) {
		if ((*padata)->pa_type == ap->type) {
		    pa = *padata;
		    break;
		}
	    }
	}
	if ((retval = ap->return_padata(context, pa, client, request, reply,
					client_key, encrypting_key, send_pa)))
	    goto cleanup;

	if (*send_pa)
	    send_pa++;
	*send_pa = 0;
    }
    
    retval = 0;

    if (send_pa_list[0]) {
	reply->padata = send_pa_list;
	send_pa_list = 0;
    }
    
cleanup:
    if (send_pa_list)
	krb5_free_pa_data(/*context, */send_pa_list);
    return (retval);
}

#ifdef OSF_DCE_FEATURE
static krb5_error_code
verify_enc_timestamp(context, client, request, client_enc_key, enc_tkt_reply, pa)
    krb5_context	context;
    krb5_db_entry *	client;
    krb5_kdc_req *	request;
    krb5_keyblock *     client_enc_key;
    krb5_enc_tkt_part * enc_tkt_reply;
    krb5_pa_data *	pa;
#else
static krb5_error_code
verify_enc_timestamp(context, client, request, enc_tkt_reply, pa)
    krb5_context	context;
    krb5_db_entry *	client;
    krb5_kdc_req *	request;
    krb5_enc_tkt_part * enc_tkt_reply;
    krb5_pa_data *	pa;
#endif
{
    krb5_pa_enc_ts *		pa_enc = 0;
    krb5_error_code		retval;
    krb5_data			scratch;
    krb5_data			enc_ts_data;
    krb5_enc_data 		*enc_data = 0;
    krb5_keyblock		key;
    krb5_timestamp		timenow;
    krb5_encrypt_block          eblock;

    enc_ts_data.data = 0;
    scratch.data = (char *)pa->contents;
    scratch.length = pa->length;
    memset((char*)&enc_ts_data,0,sizeof(krb5_data));
    memset((char*)&key,0,sizeof(krb5_keyblock));
    memset((char*)&eblock,0,sizeof(krb5_encrypt_block));
    
    if ((retval = decode_krb5_enc_data(&scratch, &enc_data)) != 0)
	goto cleanup;

#ifdef OSF_DCE_ARCHAISM
    /*  OSF DCE does not use the KRB5 BETA6 krb5_dbe_search_enctype()
     *  and krb5_dbekd_decrypt_key_data() functions, so our logic
     *  diverges somewhat here.
     *
     *  Next, in the derived user's key
     * 	the function requires the use of an "eblock" so we 
     * 		add the encryption type to eblock
     *		put the user's key in the eblock
     */
    if (retval = KDB_CONVERT_KEY_OUTOF_DB(&(client->key), &key)) 
        goto cleanup;

    krb5_use_cstype(&eblock, enc_data->etype);
    if (retval = krb5_process_key(&eblock, &key)) 
        goto cleanup;

    enc_ts_data.length = enc_data->ciphertext.length;
    if (!(enc_ts_data.data =
          malloc(enc_data->ciphertext.length))) {
        retval=ENOMEM;
        goto cleanup;
    }

    if (retval=krb5_decrypt(enc_data->ciphertext.data, 
                            enc_ts_data.data,
                            enc_ts_data.length, &eblock, 0)) {
        goto cleanup;
    }
#endif /* OSF_DCE_ARCHAISM */

    if ((retval = decode_krb5_pa_enc_ts(&enc_ts_data, &pa_enc)) != 0)
	goto cleanup;

    if ((retval = krb5_timeofday(/*context,*/ &timenow)) != 0)
	goto cleanup;

#ifdef OSF_DCE_ARCHAISM    
    /* OSF DCE stores allowed skew in a global variable rather
     * than in each client's context
     */
    if (labs(timenow - pa_enc->patimestamp) > krb5_clockskew) {
#else
    if (labs(timenow - pa_enc->patimestamp) > context->clockskew) {
#endif /* OSF_DCE_ARCHAISM */
	retval = KRB5KRB_AP_ERR_SKEW;
	goto cleanup;
    }

    setflag(enc_tkt_reply->flags, TKT_FLG_PRE_AUTH);

    retval = 0;
    
cleanup:
    if (key.contents) {
        memset((char *)(key.contents), 0, (key.length));
        xfree(key.contents);
        key.contents = NULL;
    }
    if (enc_data) {
	if (enc_data->ciphertext.data)
	    krb5_xfree(enc_data->ciphertext.data);
	free(enc_data);
    }
    if (enc_ts_data.data)
	krb5_xfree(enc_ts_data.data);
    if (pa_enc)
	krb5_xfree(pa_enc);
    if (eblock.key) {
        if (eblock.key->contents)
            krb5_xfree(eblock.key->contents);
        krb5_xfree(eblock.key);
    }
    return retval;
}

#ifdef OSF_DCE_ARCHAISM
/* verify_enc_unix_time
 *
 * contains logic that was formerly in verify_timestamp_padata()
 * in krb5/lib/krb/preauth.c
 */
static krb5_error_code
verify_enc_unix_time(context, client, request, client_enc_key, enc_tkt_reply, pa)
    krb5_context	context;
    krb5_db_entry *	client;
    krb5_kdc_req *	request;
    krb5_keyblock *     client_enc_key;
    krb5_enc_tkt_part * enc_tkt_reply;
    krb5_pa_data *	pa;
{
    unsigned char       *tmp=NULL;
    krb5_error_code     retval=0;
    krb5_timestamp      currenttime, patime;
    krb5_encrypted_keyblock *enckey=NULL;
    krb5_keyblock decrypt_key;
    krb5_encrypt_block 	eblock;
    krb5_data		scratch;
#define in_clock_skew(date) (abs((date)-currenttime) < krb5_clockskew)


    /* Initialize pointers */

    decrypt_key.contents = NULL;
    eblock.crypto_entry=NULL;
    eblock.key=NULL;
    eblock.priv=NULL;
    scratch.data=NULL;

    /* start of key code from do_as_req.c check_padata()
     * client in check_padata() is same as client input
     * to this function
     */

    enckey = &(client->key);
    /* Extract client key/alt_key from master key */
    retval = KDB_CONVERT_KEY_OUTOF_DB(enckey,&decrypt_key);
    if (retval) {
	syslog( LOG_ERR, "AS_REQ: Unable to Extract Client Key/alt_key\n");
	return(0);
    }

    /* start of preauth.c krb5_verify_padata pre-processing
     * "data" corresponds to "pa" above;
     * decrypt_key corresponds to &tmpkey above
     */

    /* If we dont have a decryption key we are out of luck */
    if (!decrypt_key.contents) {
        retval=EINVAL;
        goto unix_out;
    }

    krb5_use_keytype(&eblock, decrypt_key.keytype);

    scratch.length = pa->length;
    if (!(scratch.data = (char *)malloc(scratch.length))) {
       retval=ENOMEM;
       goto unix_out;
    }

    /* do any necessary key pre-processing */
    retval = krb5_process_key(&eblock,&decrypt_key);
    if (retval)
       goto unix_out;

    /* Decrypt data */
    retval = krb5_decrypt((char *) pa->contents + 4,
                          (krb5_pointer) scratch.data,
                          scratch.length - 4, &eblock, 0);
    (void) krb5_finish_key(&eblock);
    if (retval)
       goto unix_out;

    scratch.length  = (((int) ((unsigned char *)pa->contents)[0] << 24)
                       + ((int) ((unsigned char *)pa->contents)[1] << 16)
                       + ((int) ((unsigned char *)pa->contents)[2] << 8)
                       + (int) ((unsigned char *)pa->contents)[3]);

   /* end of preauth.c krb5_verify_padata pre-processing
    * it then passes &scratch (=data) to krb5_verify_timestamp
    */    

    tmp = (unsigned char *) scratch.data;
    if (tmp[8] != 0) {
        retval=KRB5_PREAUTH_FAILED;
        goto unix_out;
    }
    patime = (int) tmp[9] << 24;
    patime += (int) tmp[10] << 16;
    patime += (int) tmp[11] << 8;
    patime += tmp[12];

    retval = krb5_timeofday(&currenttime);
    if (retval)
        goto unix_out;

    if (!in_clock_skew(patime)) {
        retval=KRB5_PREAUTH_FAILED;
        goto unix_out;
    }

unix_out:
    if (decrypt_key.contents) {
        memset((char *)decrypt_key.contents, 0, decrypt_key.length);
        xfree(decrypt_key.contents);
    }
    if (scratch.data)
        free(scratch.data);
    return retval;

}
#endif /* OSF_DCE_ARCHAISM */

/*
 * This function returns the etype information for a particular
 * client, to be passed back in the preauth list in the KRB_ERROR
 * message.
 */
static krb5_error_code
get_etype_info(context, request, client, server, pa_data)
    krb5_context 	context;
    krb5_kdc_req *	request;
    krb5_db_entry *	client;
    krb5_db_entry *	server;
    krb5_pa_data *	pa_data;
{
    krb5_etype_info_entry **	entry = 0;
    krb5_key_data		*client_key;
    krb5_error_code		retval;
    krb5_data			salt;
    krb5_data *			scratch = NULL;
    krb5_enctype		db_etype;
    int 			i = 0;
    int 			start = 0;

    salt.data = 0;
#ifdef OSF_DCE_ARCHAISM
    entry = malloc (2*sizeof(krb5_etype_info_entry *));
    if (entry == NULL)
	return ENOMEM;

    entry[1] = NULL;
    entry[0] = malloc (sizeof (krb5_etype_info_entry));
    if (entry[0] == NULL) {
	retval = ENOMEM;
	goto cleanup;
    }
    
    entry[0]->etype = ETYPE_DES_CBC_CRC;
    entry[0]->length = 0;
    entry[0]->salt = 0;
    retval = get_salt_from_key (context, request->client, client, &salt);
    if (retval)
	goto cleanup;
    
    if (salt.length > 0) {
	entry[0]->length = salt.length;
	entry[0]->salt = (unsigned char *)salt.data;
	salt.data = 0;
    }
#else

    entry = malloc((client->n_key_data * 2) * sizeof(krb5_etype_info_entry *));
    if (entry == NULL)
	return ENOMEM;
    entry[0] = NULL;

    while (1) {
	retval = krb5_dbe_search_enctype(context, client, &start, -1,
					 -1, 0, &client_key);
	if (retval == ENOENT)
	    break;
	if (retval)
	    goto cleanup;
	db_etype = client_key->key_data_type[0];
	if (db_etype == ENCTYPE_DES_CBC_MD4 || db_etype == ENCTYPE_DES_CBC_MD5)
	    db_etype = ENCTYPE_DES_CBC_CRC;
	
	while (1) {
	    if ((entry[i] = malloc(sizeof(krb5_etype_info_entry))) == NULL) {
		retval = ENOMEM;
		goto cleanup;
	    }
	    entry[i+1] = 0;
	    entry[i]->magic = KV5M_ETYPE_INFO_ENTRY;
	    entry[i]->etype = db_etype;
	    entry[i]->length = -1;
	    entry[i]->salt = 0;
	    retval = get_salt_from_key(context, request->client,
				       client_key,
				       &salt);
	    if (retval)
		goto cleanup;
	    if (salt.length >= 0) {
		entry[i]->length = salt.length;
		entry[i]->salt = salt.data;
		salt.data = 0;
	    }
	    i++;
	    /*
	     * If we have a DES_CRC key, it can also be used as a
	     * DES_MD5 key.
	     */
	    if (db_etype == ENCTYPE_DES_CBC_CRC)
		db_etype = ENCTYPE_DES_CBC_MD5;
	    else
		break;
	}
    }
#endif /* OSF_DCE_ARCHAISM */
    retval = encode_krb5_etype_info(entry, &scratch);
    if (retval)
	goto cleanup;
    pa_data->contents = (unsigned char *)scratch->data;
    pa_data->length = scratch->length;

    retval = 0;

cleanup:
    if (scratch)
        free(scratch);
    if (entry)
	krb5_free_etype_info(context, entry);
    if (salt.data)
	krb5_xfree(salt.data);
    return retval;
}

static krb5_error_code
return_pw_salt(context, in_padata, client, request, reply, client_key,
	       encrypting_key, send_pa)
    krb5_context	context;
    krb5_pa_data *	in_padata;
    krb5_db_entry *	client;
    krb5_kdc_req *	request;
    krb5_kdc_rep *	reply;
    krb5_key_data *	client_key;
    krb5_keyblock *	encrypting_key;
    krb5_pa_data **	send_pa;
{
    krb5_error_code	retval;
    krb5_pa_data        *padata;
    krb5_data *		scratch;
    krb5_data		salt_data;
    
    if (client->salt_type == KRB5_KDB_SALTTYPE_NORMAL)
	return 0;

    if (((padata) = malloc(sizeof(krb5_pa_data))) == NULL)
	return ENOMEM;
#ifndef OSF_DCE_ARCHAISM
    padata->magic = KV5M_PA_DATA;
#endif
    padata->pa_type = KRB5_PADATA_PW_SALT;
    padata->contents=0;
    padata->length=0;
    
    switch (client->salt_type) {
    case KRB5_KDB_SALTTYPE_V4:
	/* send an empty (V4) salt */
	break;
    case KRB5_KDB_SALTTYPE_NOREALM:
	if ((retval = krb5_principal2salt_norealm(kdc_context,
						  request->client,
						  &salt_data)))
	    goto cleanup;
	padata->contents = (krb5_octet *)salt_data.data;
	padata->length = salt_data.length;
	break;
#ifndef OSF_DCE_ARCHAISM
    case KRB5_KDB_SALTTYPE_AFS3:
	/* send an AFS style realm-based salt */
	/* for now, just pass the realm back and let the client
	   do the work. In the future, add a kdc configuration
	   variable that specifies the old cell name. */
	padata->pa_type = KRB5_PADATA_AFS3_SALT;
	/* it would be just like ONLYREALM, but we need to pass the 0 */
	scratch = krb5_princ_realm(/*kdc_context,*/ request->client);
	if ((padata->contents = malloc(scratch->length+1)) == NULL) {
	    retval = ENOMEM;
	    goto cleanup;
	}
	memcpy(padata->contents, scratch->data, scratch->length);
	padata->length = scratch->length+1;
	padata->contents[scratch->length] = 0;
	break;
#endif
    case KRB5_KDB_SALTTYPE_ONLYREALM:
	scratch = krb5_princ_realm(/*kdc_context,*/ request->client);
	if ((padata->contents = malloc(scratch->length)) == NULL) {
	    retval = ENOMEM;
	    goto cleanup;
	}
	memcpy(padata->contents, scratch->data, scratch->length);
	padata->length = scratch->length;
	break;
    case KRB5_KDB_SALTTYPE_SPECIAL:
	if ((padata->contents = malloc(client->salt_length))
	    == NULL) {
	    retval = ENOMEM;
	    goto cleanup;
	}
	memcpy(padata->contents, client->salt,
	       client->salt_length);
	padata->length = client->salt_length;
	break;
    default:
	free(padata);
	return 0;
    }

    *send_pa = padata;
    return 0;
    
cleanup:
    if (padata) {
        if (padata->contents)
           free(padata->contents);
        free(padata);
    }
    return retval;
}

#if 0 /* SAM stuff */
    
static struct {
  char* name;
  int   sam_type;
} *sam_ptr, sam_inst_map[] = {
  "SNK4", PA_SAM_TYPE_DIGI_PATH,
  "SECURID", PA_SAM_TYPE_SECURID,
  "GRAIL", PA_SAM_TYPE_GRAIL,
  0, 0
};

static krb5_error_code
get_sam_edata(context, request, client, server, pa_data)
    krb5_context 	context;
    krb5_kdc_req *	request;
    krb5_db_entry *	client;
    krb5_db_entry *	server;
    krb5_pa_data *	pa_data;
{
    krb5_error_code		retval;
    krb5_sam_challenge		sc;
    krb5_predicted_sam_response	psr;
    krb5_data *			scratch;
    int 			i = 0;
    int 			start = 0;
    krb5_encrypt_block		eblock;
    krb5_keyblock encrypting_key;
    char response[9];
    char inputblock[8];
    krb5_data predict_response;

    /* Given the client name we can figure out what type of preauth
       they need. The spec is currently for querying the database for
       names that match the types of preauth used. Later we should
       make this mapping show up in kdc.conf. In the meantime, we
       hardcode the following:
		/SNK4 -- Digital Pathways SNK/4 preauth.
		/GRAIL -- experimental preauth
       The first one found is used. See sam_inst_map above.

       For SNK4 in particular, the key in the database is the key for
       the device; kadmin needs a special interface for it.
     */

    {
      char *uname;
      int npr = 1, more;
      krb5_db_entry assoc;
      krb5_key_data  *assoc_key;
      krb5_principal newp;
      int newlen;
      int probeslot;

      sc.sam_type = 0;

      retval = krb5_copy_principal(/*kdc_context,*/ request->client, &newp);
      if (retval) {
	com_err("krb5kdc", retval, "copying client name for preauth probe");
	return retval;
      }

      probeslot = krb5_princ_size(/*context,*/ newp)++;
      krb5_princ_name(/*kdc_context,*/ newp) = 
	realloc(krb5_princ_name(/*kdc_context,*/ newp),
		krb5_princ_size(context, newp) * sizeof(krb5_data));

      for(sam_ptr = sam_inst_map; sam_ptr->name; sam_ptr++) {
	krb5_princ_component(/*kdc_context,*/newp,probeslot)->data = sam_ptr->name;
	krb5_princ_component(/*kdc_context,*/newp,probeslot)->length = 
	  strlen(sam_ptr->name);
	npr = 1;
	retval = krb5_db_get_principal(/*kdc_context,*/ newp, &assoc, &npr, &more);
	if(!retval) {
	  sc.sam_type = sam_ptr->sam_type;
	  break;
	}
      }
      /* if sc.sam_type is set, it worked */
      if (sc.sam_type) {
	/* so use assoc to get the key out! */
	{
	  /* here's what do_tgs_req does */
	  retval = krb5_dbe_find_enctype(/*kdc_context,*/ &assoc,
					 ENCTYPE_DES_CBC_RAW,
					 KRB5_KDB_SALTTYPE_NORMAL,
					 0,		/* Get highest kvno */
					 &assoc_key);
	  if (retval) {
	    char *sname;
	    krb5_unparse_name(/*kdc_context,*/ newp, &sname);
	    com_err("krb5kdc", retval, 
		    "snk4 finding the enctype and key <%s>", sname);
	    return retval;
	  }
	  /* convert server.key into a real key */
	  retval = krb5_dbekd_decrypt_key_data(/*kdc_context,*/
					       &master_encblock, 
					       assoc_key, &encrypting_key,
					       NULL);
	  if (retval) {
	    com_err("krb5kdc", retval, 
		    "snk4 pulling out key entry");
	    return retval;
	  }
	  /* now we can use encrypting_key... */
	}
      }

      krb5_princ_component(/*kdc_context,*/newp,probeslot)->data = 0;
      krb5_princ_component(/*kdc_context,*/newp,probeslot)->length = 0;
      krb5_princ_size(context, newp)--;
      krb5_free_principal(/*kdc_context,*/ newp);
    }
    sc.magic = KV5M_SAM_CHALLENGE;
    sc.sam_flags = KRB5_SAM_USE_SAD_AS_KEY;

    switch (sc.sam_type) {
    case PA_SAM_TYPE_GRAIL:
      sc.sam_type_name.data = "Experimental System";
      sc.sam_type_name.length = strlen(sc.sam_type_name.data);
      sc.sam_challenge_label.data = "experimental challenge label";
      sc.sam_challenge_label.length = strlen(sc.sam_challenge_label.data);
      sc.sam_challenge.data = "12345";
      sc.sam_challenge.length = strlen(sc.sam_challenge.data);

      psr.magic = KV5M_PREDICTED_SAM_RESPONSE;
      /* string2key on sc.sam_challenge goes in here */
      /* eblock is just to set the enctype */
      {
	const krb5_enctype type = ENCTYPE_DES_CBC_MD5;
	if (!valid_enctype(type)) return KRB5_PROG_ETYPE_NOSUPP;
	krb5_use_enctype(context, &eblock, type);
	retval = krb5_string_to_key(context, &eblock, 
				    &psr.sam_key, &sc.sam_challenge, 
				    0 /* salt */);
	retval = encode_krb5_predicted_sam_response(&psr, &scratch);
	if (retval) goto cleanup;
	
	{
	  krb5_enc_data tmpdata;
	  retval = krb5_encrypt_data(context, master_encblock.key, 0, 
				     scratch, &tmpdata);
	  sc.sam_track_id = tmpdata.ciphertext;
	}
	if (retval) goto cleanup;
      }

      sc.sam_response_prompt.data = "response prompt";
      sc.sam_response_prompt.length = strlen(sc.sam_response_prompt.data);
      sc.sam_pk_for_sad.length = 0;
      sc.sam_nonce = 0;
      /* Generate checksum */
      /*krb5_checksum_size(context, ctype)*/
      /*krb5_calculate_checksum(context,ctype,in,in_length,seed,
	seed_length,outcksum) */
      /*krb5_verify_checksum(context,ctype,cksum,in,in_length,seed,
	seed_length) */
      sc.sam_cksum.contents = (krb5_octet *)
	malloc(krb5_checksum_size(/*context,*/ CKSUMTYPE_RSA_MD5_DES));
      if (sc.sam_cksum.contents == NULL) return(ENOMEM);

      retval = krb5_calculate_checksum(/*context,*/ CKSUMTYPE_RSA_MD5_DES,
				       sc.sam_challenge.data,
				       sc.sam_challenge.length,
				       psr.sam_key.contents, /* key */
				       psr.sam_key.length, /* key length */
				       &sc.sam_cksum);
      if (retval) { free(sc.sam_cksum.contents); return(retval); }
      
      retval = encode_krb5_sam_challenge(&sc, &scratch);
      if (retval) goto cleanup;
#ifndef OSF_DCE_ARCHAISM
      pa_data->magic = KV5M_PA_DATA;
#endif
      pa_data->pa_type = KRB5_PADATA_SAM_CHALLENGE;
      pa_data->contents = scratch->data;
      pa_data->length = scratch->length;
      
      retval = 0;
      break;
    case PA_SAM_TYPE_DIGI_PATH:
      sc.sam_type_name.data = "Digital Pathways";
      sc.sam_type_name.length = strlen(sc.sam_type_name.data);
#if 1
      sc.sam_challenge_label.data = "Enter the following on your keypad";
      sc.sam_challenge_label.length = strlen(sc.sam_challenge_label.data);
#endif
      /* generate digit string, take it mod 1000000 (six digits.) */
      {
	int j;
	krb5_encrypt_block eblock;
	krb5_keyblock *session_key = 0;
	char outputblock[8];
	int i;
	memset(inputblock, 0, 8);
	krb5_use_enctype(/*kdc_context,*/ &eblock, ENCTYPE_DES_CBC_CRC);
	retval = krb5_random_key(/*kdc_context,*/ &eblock, 
				 krb5_enctype_array[ENCTYPE_DES_CBC_CRC]->random_sequence,
				 &session_key);
	if (retval) {
	  /* random key failed */
	  com_err("krb5kdc", retval,"generating random challenge for preauth");
	  return retval;
	}
	/* now session_key has a key which we can pick bits out of */
	/* we need six decimal digits. Grab 6 bytes, div 2, mod 10 each. */
	if (session_key->length != 8) {
	  com_err("krb5kdc", retval = KRB5KDC_ERR_ETYPE_NOSUPP,
		  "keytype didn't match code expectations");
	  return retval;
	}
	for(i = 0; i<6; i++) {
	  inputblock[i] = '0' + ((session_key->contents[i]/2) % 10);
	}
	if (session_key)
	  krb5_free_keyblock(/*kdc_context,*/ session_key);

	/* retval = krb5_finish_key(/*kdc_context, &eblock); */
	/* now we have inputblock containing the 8 byte input to DES... */
	sc.sam_challenge.data = inputblock;
	sc.sam_challenge.length = 6;

	krb5_use_enctype(/*kdc_context,*/ &eblock, ENCTYPE_DES_CBC_RAW);
	encrypting_key.enctype = ENCTYPE_DES_CBC_RAW;
	/* do any necessary key pre-processing */
	retval= krb5_process_key(/*kdc_context,*/ &eblock, &encrypting_key);

	if (retval) {
	  com_err("krb5kdc", retval, "snk4 processing key");
	}

	{
	  char ivec[8];
	  memset(ivec,0,8);
	  retval = krb5_encrypt(/*kdc_context,*/ inputblock, outputblock, 8,
				&eblock, ivec);
	}
	if (retval) {
	  com_err("krb5kdc", retval, "snk4 response generation failed");
	  return retval;
	}
	/* now output block is the raw bits of the response; convert it
	   to display form */
	for (j=0; j<4; j++) {
	  char n[2];
	  int k;
	  n[0] = outputblock[j] & 0xf;
	  n[1] = (outputblock[j]>>4) & 0xf;
	  for (k=0; k<2; k++) {
	    if(n[k] > 9) n[k] = ((n[k]-1)>>2);
	    /* This is equivalent to:
	       if(n[k]>=0xa && n[k]<=0xc) n[k] = 2;
	       if(n[k]>=0xd && n[k]<=0xf) n[k] = 3;
	       */
	  }
	  /* for v4, we keygen: *(j+(char*)&key1) = (n[1]<<4) | n[0]; */
	  /* for v5, we just generate a string */
	  response[2*j+0] = '0' + n[1];
	  response[2*j+1] = '0' + n[0];
	  /* and now, response has what we work with. */
	}
	response[8] = 0;
	predict_response.data = response;
	predict_response.length = 8;
#if 0				/* for debugging, hack the output too! */
sc.sam_challenge_label.data = response;
sc.sam_challenge_label.length = strlen(sc.sam_challenge_label.data);
#endif
      }

      psr.magic = KV5M_PREDICTED_SAM_RESPONSE;
      /* string2key on sc.sam_challenge goes in here */
      /* eblock is just to set the enctype */
      {
	const krb5_enctype type = ENCTYPE_DES_CBC_MD5;
	if (!valid_enctype(type)) return KRB5_PROG_ETYPE_NOSUPP;
	krb5_use_enctype(context, &eblock, type);
	retval = krb5_string_to_key(context, &eblock, 
				    &psr.sam_key, &predict_response, 
				    0 /* salt */);
	retval = encode_krb5_predicted_sam_response(&psr, &scratch);
	if (retval) goto cleanup;
	
	{
	  krb5_enc_data tmpdata;
	  retval = krb5_encrypt_data(context, master_encblock.key, 0, 
				     scratch, &tmpdata);
	  sc.sam_track_id = tmpdata.ciphertext;
	}
	if (retval) goto cleanup;
      }

      sc.sam_response_prompt.data = "Enter the displayed response";
      sc.sam_response_prompt.length = strlen(sc.sam_response_prompt.data);
      sc.sam_pk_for_sad.length = 0;
      sc.sam_nonce = 0;
      /* Generate checksum */
      /*krb5_checksum_size(context, ctype)*/
      /*krb5_calculate_checksum(context,ctype,in,in_length,seed,
	seed_length,outcksum) */
      /*krb5_verify_checksum(context,ctype,cksum,in,in_length,seed,
	seed_length) */
      sc.sam_cksum.contents = (krb5_octet *)
	malloc(krb5_checksum_size(/*context,*/ CKSUMTYPE_RSA_MD5_DES));
      if (sc.sam_cksum.contents == NULL) return(ENOMEM);

      retval = krb5_calculate_checksum(/*context,*/ CKSUMTYPE_RSA_MD5_DES,
				       sc.sam_challenge.data,
				       sc.sam_challenge.length,
				       psr.sam_key.contents, /* key */
				       psr.sam_key.length, /* key length */
				       &sc.sam_cksum);
      if (retval) { free(sc.sam_cksum.contents); return(retval); }
      
      retval = encode_krb5_sam_challenge(&sc, &scratch);
      if (retval) goto cleanup;
#ifndef OSF_DCE_ARCHAISM
      pa_data->magic = KV5M_PA_DATA;
#endif
      pa_data->pa_type = KRB5_PADATA_SAM_CHALLENGE;
      pa_data->contents = scratch->data;
      pa_data->length = scratch->length;
      
      retval = 0;
      break;
    }

cleanup:
    memset((char *)encrypting_key.contents, 0, encrypting_key.length);
    krb5_xfree(encrypting_key.contents);
    return retval;
}

static krb5_error_code
verify_sam_response(context, client, request, enc_tkt_reply, pa)
    krb5_context	context;
    krb5_db_entry *	client;
    krb5_kdc_req *	request;
    krb5_enc_tkt_part * enc_tkt_reply;
    krb5_pa_data *	pa;
{
    krb5_error_code		retval;
    krb5_data			scratch;
    krb5_sam_response		*sr = 0;
    krb5_predicted_sam_response	*psr = 0;
    krb5_enc_sam_response_enc	*esre = 0;
    krb5_timestamp		timenow;

    scratch.data = pa->contents;
    scratch.length = pa->length;
    
    retval = decode_krb5_sam_response(&scratch, &sr);
    if (retval) com_err("krb5kdc", retval, "decode_krb5_sam_response failed");
    if (retval) goto cleanup;

    {
      krb5_enc_data tmpdata;
      tmpdata.ciphertext = sr->sam_track_id;
      retval = krb5_decrypt_data(context, master_encblock.key, 0, 
				 &tmpdata, &scratch);
      if (retval) com_err("krb5kdc", retval, "decrypt track_id failed");
    }
    if (retval) goto cleanup;
    retval = decode_krb5_predicted_sam_response(&scratch, &psr);
    if (retval) com_err("krb5kdc", retval, "decode_krb5_predicted_sam_response failed");
    if (retval) goto cleanup;
    {
      /* now psr.sam_key is what we said to use... */
      retval = krb5_decrypt_data(context, &psr->sam_key, 0, 
				 &sr->sam_enc_nonce_or_ts, &scratch);
      if (retval) com_err("krb5kdc", retval, "decrypt nonce_or_ts failed");
    }
    if (retval) goto cleanup;
    retval = decode_krb5_enc_sam_response_enc(&scratch, &esre);
    if (retval) com_err("krb5kdc", retval, "decode_krb5_enc_sam_response_enc failed");
    if (retval) goto cleanup;
    if (esre->sam_timestamp != sr->sam_patimestamp) {
      retval = KRB5KDC_ERR_PREAUTH_FAILED;
      goto cleanup;
    }
    retval = krb5_timeofday(context, &timenow);
    if (retval) goto cleanup;

#ifdef OSF_DCE_ARCHAISM
    /* OSF DCE stores allowed skew in a global rather than in
     * each client's context
     */
    if (labs(timenow - sr->sam_patimestamp) > krb5_clockskew) {
#else
    if (labs(timenow - sr->sam_patimestamp) > context->clockskew) {
#endif
	retval = KRB5KRB_AP_ERR_SKEW;
	goto cleanup;
    }

    setflag(enc_tkt_reply->flags, TKT_FLG_HW_AUTH);
  cleanup:
    if (retval) com_err("krb5kdc", retval, "sam verify failure");
    if (sr) krb5_xfree(sr);
    if (psr) krb5_xfree(psr);
    if (esre) krb5_xfree(esre);

    return retval;
}

#endif /* SAM stuff */

#ifdef OSF_DCE_FEATURE

#include <u_str.h>
#include <dce/rgynbase.h>
#include <dce/sec_base.h>
#include <dce/sec_psm.h>
#include <dce/sec_pk.h> 

sec_psm_handle_t kdc_psm_h = NULL;

error_status_t kdc_psm_h_init(void)
{
    sec_rgy_name_t          pname;
    char	            *cell_name;
    error_status_t          st;

    krb5_pkinit_init();

    /* Code for constructing krbtgt principal name is taken from
     * dced/server/sv_clientd.c/rsec_pk_cache_kdc_key().  If error
     * found, fix in both places.
     */
    dce_cf_get_cell_name(&cell_name, &st);
    if ((KRB_DIR_ROOT_LEN + strlen(cell_name) - GLOBAL_DIR_ROOT_LEN) > sizeof(pname)) {
        free(cell_name);
        return sec_res_princ_cvt_err;
    }
    else {
        u_strcpy(pname, KRB_DIR_ROOT);
        u_strcat(pname, cell_name + GLOBAL_DIR_ROOT_LEN);
        free(cell_name);
        return sec_psm_open(pname, (idl_char *)NULL,
                            &sec_pk_domain_kdc_pk_init, &kdc_psm_h);
    }
}

static krb5_error_code
verify_pk_request (context, client, request, client_enc_key, enc_tkt_reply, pa)
    krb5_context	context;
    krb5_db_entry *	client;
    krb5_kdc_req *	request;
    krb5_keyblock *     client_enc_key;
    krb5_enc_tkt_part * enc_tkt_reply;
    krb5_pa_data *	pa;
{
    /* fields from the message */
    krb5_checksum pa_checksum;
    krb5_int32 pa_cusec;
    krb5_timestamp pa_ctime, currenttime;
    krb5_int32 pa_nonce;
    krb5_keyblock nullkey;	/* !!! */
    
    krb5_error_code errcode;

    if (kdc_psm_h == NULL) {
	errcode = kdc_psm_h_init();
	if (errcode) {
	    return errcode;
	}
    }
    
#define in_clock_skew(date) (abs((date)-currenttime) < krb5_clockskew)
    
    errcode = krb5_pkinit_decode_as_req (kdc_psm_h,
					 pa,
					 request->server,
					 client->principal,
					 &pa_checksum,
					 &pa_cusec,
					 &pa_ctime,
					 &pa_nonce);
    /*
     * !!! must require that checksum is "good enough" here...
     * mandate MD5 when we've got it.
     */
    if (errcode == 0) {
	nullkey.keytype = 0; /* !!! */
	nullkey.contents = NULL; /* !!! */
	nullkey.length = 0;	/* !!! */
	errcode = verify_kdc_body_cksum(NULL, request, &pa_checksum, &nullkey);
    }
    
    if (errcode == 0) {
	if (pa_nonce != request->nonce)
	    errcode = KRB5KDC_ERR_PREAUTH_FAILED;
    }
    if (errcode == 0) {
	errcode = krb5_timeofday(&currenttime);
    }
    if (errcode == 0) {
	if (!in_clock_skew(pa_ctime))
	    errcode = KRB5KDC_ERR_PREAUTH_FAILED;
    }
    /* !!! we don't do anything with cusec; why are we carrying it around? */
    free(pa_checksum.contents);
    return errcode;
}

static krb5_error_code generate_pk_response
    (context, in_padata, client, request, reply, client_key,
     encrypting_key, send_pa)
    krb5_context	context;
    krb5_pa_data *	in_padata;
    krb5_db_entry *	client;
    krb5_kdc_req *	request;
    krb5_kdc_rep *	reply;
    krb5_key_data *	client_key;
    krb5_keyblock *	encrypting_key;
    krb5_pa_data **	send_pa;
{
    krb5_pa_data *out_pa;
    krb5_keyblock *reply_key;
    krb5_error_code errcode;
    
    if (in_padata == NULL)
	return 0;		/* no pk data on request.. */

    out_pa = (krb5_pa_data *)malloc (sizeof(krb5_pa_data));

    errcode = krb5_pkinit_sign_as_rep (kdc_psm_h, 
				       request->server,
				       client->principal,
				       reply->enc_part.etype,
				       &reply_key,
				       request->nonce,
				       out_pa);
    if (errcode == 0) {
	memset(encrypting_key->contents, 0, encrypting_key->length);
	xfree(encrypting_key->contents);
	*encrypting_key = *reply_key;
	reply_key->contents = NULL;
	xfree(reply_key);
	*send_pa = out_pa;
    } else {
	free(out_pa);
    }
    return errcode;
}

#endif /* OSF_DCE_FEATURE public key preauth */
