/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rpriv_util.c,v $
 * Revision 1.1.19.3  1996/10/03  15:08:37  arvind
 * 	fix type mismatch for decode_krb5_authdata().
 * 	[1996/09/16  22:59 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_1/1]
 *
 * 	Submit OSF/DCE 1.2.1
 * 	HP 	[1995/05/18  23:26 UTC  rps  /main/HPDCE02/5]
 *
 * 	WP merge
 * 	HP 	[1995/05/09  21:12 UTC  sommerfeld  /main/DCE1_1_WP/2]
 *
 * 	CHFts14687: fix rpriv_util memory leak
 * 	HP 	[1995/03/31  13:19 UTC  aha  /main/HPDCE02/aha_mothra4/4]
 *
 * 	CHFts14687: Fix parameter to rpc_string_free call
 * 	HP 	[1995/03/28  22:03 UTC  aha  /main/HPDCE02/aha_mothra4/3]
 *
 * 	CHFts14676: better fix
 * 	HP 	[1995/03/28  21:55 UTC  aha  /main/HPDCE02/aha_mothra4/2]
 *
 * 	CHFts14687: Use rpc_string_free, not just free
 * 	HP 	[1995/03/28  20:43 UTC  aha  /main/HPDCE02/aha_mothra4/1]
 *
 * 	CHFts14687: need to free address in rpriv_util_get_dce_authz_info
 * 	HP 	[1995/01/10  16:35 UTC  greg  /main/HPDCE02/2]
 *
 * 	fix for CHFts13993 (and OSF CR# 12770).
 * 	HP 	[1995/01/10  15:10 UTC  greg  /main/HPDCE02/greg_mothra_bugfix/1]
 *
 * 	fix for CHFts13993 (and OSF CR# 12770)
 * 	HP 	[1994/12/22  13:56 UTC  greg  /main/greg_migration2/1]
 *
 * 	fix bug described in OT 12770.
 * 	Add rpriv_util_finish_ps_eblock().
 * 	HP 	[1994/12/15  03:25 UTC  rps  /main/HPDCE02/1]
 *
 * 	HP 	[1994/12/14  23:57 UTC  rps  /main/rps_mothra/1]
 * 	[1996/10/03  13:51:48  arvind]
 *
 * Revision 1.1.19.2  1996/07/08  18:38:03  arvind
 * 	Disable the audit point in rpriv_util_get_dce_authz_info().
 * 	[1996/04/11  18:02 UTC  jrr  /main/HPDCE02/jrr_davis_2/1]
 * 
 * 	merge in global group work for DCE 1.2.2
 * 	[1996/03/27  17:49 UTC  hanfei  /main/DCE_1.2/2]
 * 
 * 	uuid_from_string does not parse global names. Audit point
 * 	in rpriv_util_get_dce_authz_info() is just coded wrong.
 * 	[1996/03/21  18:39 UTC  jrr  /main/DCE_1.2/hanfei_dce1.2_b1/3]
 * 
 * 	rsdb_pgo_get_foreign_groupsets name change
 * 	[1996/02/21  15:40 UTC  hanfei  /main/DCE_1.2/hanfei_dce1.2_b1/2]
 * 
 * 	multi-cell group work: add rpriv_util_get_foreign_groupset() for
 * 	retrieving group set where a foreign principal is a member of.
 * 	[1996/01/31  17:55 UTC  hanfei  /main/DCE_1.2/hanfei_dce1.2_b1/1]
 * 
 * 	Update OSF copyright years
 * 	[1996/02/17  23:06:12  marty]
 * 
 * 	Submit OSF/DCE 1.2.1
 * 	HP 	[1995/05/18  23:26 UTC  rps  /main/HPDCE02/5]
 * 
 * 	WP merge
 * 	HP 	[1995/05/09  21:12 UTC  sommerfeld  /main/DCE1_1_WP/2]
 * 
 * 	CHFts14687: fix rpriv_util memory leak
 * 	HP 	[1995/03/31  13:19 UTC  aha  /main/HPDCE02/aha_mothra4/4]
 * 
 * 	CHFts14687: Fix parameter to rpc_string_free call
 * 	HP 	[1995/03/28  22:03 UTC  aha  /main/HPDCE02/aha_mothra4/3]
 * 
 * 	CHFts14676: better fix
 * 	HP 	[1995/03/28  21:55 UTC  aha  /main/HPDCE02/aha_mothra4/2]
 * 
 * 	CHFts14687: Use rpc_string_free, not just free
 * 	HP 	[1995/03/28  20:43 UTC  aha  /main/HPDCE02/aha_mothra4/1]
 * 
 * 	CHFts14687: need to free address in rpriv_util_get_dce_authz_info
 * 	HP 	[1995/01/10  16:35 UTC  greg  /main/HPDCE02/2]
 * 
 * 	fix for CHFts13993 (and OSF CR# 12770).
 * 	HP 	[1995/01/10  15:10 UTC  greg  /main/HPDCE02/greg_mothra_bugfix/1]
 * 
 * 	fix for CHFts13993 (and OSF CR# 12770)
 * 	HP 	[1994/12/22  13:56 UTC  greg  /main/greg_migration2/1]
 * 
 * 	fix bug described in OT 12770.
 * 	Add rpriv_util_finish_ps_eblock().
 * 	HP 	[1994/12/15  03:25 UTC  rps  /main/HPDCE02/1]
 * 
 * 	HP 	[1994/12/14  23:57 UTC  rps  /main/rps_mothra/1]
 * 
 * 	[OT12669] cell renaming
 * 	[1995/12/08  17:20:01  root]
 * 
 * Revision 1.1.19.1  1996/05/10  13:21:06  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 * 
 * 	HP revision /main/DCE_1.2/2  1996/03/27  17:49 UTC  hanfei
 * 	merge in global group work for DCE 1.2.2
 * 
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/3  1996/03/21  18:39 UTC  jrr
 * 	uuid_from_string does not parse global names. Audit point
 * 	in rpriv_util_get_dce_authz_info() is just coded wrong.
 * 
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/2  1996/02/21  15:40 UTC  hanfei
 * 	rsdb_pgo_get_foreign_groupsets name change
 * 
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/1  1996/01/31  17:55 UTC  hanfei
 * 	multi-cell group work: add rpriv_util_get_foreign_groupset() for
 * 	retrieving group set where a foreign principal is a member of.
 * 
 * 	HP revision /main/DCE_1.2/1  1996/01/03  19:36 UTC  psn
 * 	WP merge
 * 	[1995/05/18  23:26 UTC  rps  /main/HPDCE02/5]
 * 
 * 	If principal name is dce-ptgt, treat as "remote" request.
 * 	[1995/05/09  21:00 UTC  sommerfeld  /main/DCE1_1_WP/sommerfeld_wp_idmap/1]
 * 
 * 	Remove #if 0 code.
 * 	[1995/03/31  19:14 UTC  sommerfeld  /main/HPDCE02/MOTHRA_panic/3]
 * 
 * 	transit path eval
 * 	[1995/03/08  21:39 UTC  rps  /main/HPDCE02/rps_hc/1]
 * 
 * 	Cell alias work:
 * 	Fuzzy principal name compare.
 * 	Correct name of TGS.
 * 	[1995/03/02  19:48 UTC  sommerfeld  /main/HPDCE02/sommerfeld_krb5_alias_1/1]
 * 
 * 	CHFts14687: fix rpriv_util memory leak
 * 	[1995/03/31  13:22 UTC  aha  /main/HPDCE02/3]
 * 
 * 	CHFts14687: Fix parameter to rpc_string_free call
 * 	[1995/03/31  13:19 UTC  aha  /main/HPDCE02/aha_mothra4/4]
 * 
 * 	CHFts14676: better fix
 * 	[1995/03/28  22:03 UTC  aha  /main/HPDCE02/aha_mothra4/3]
 * 
 * 	CHFts14687: Use rpc_string_free, not just free
 * 	[1995/03/28  21:55 UTC  aha  /main/HPDCE02/aha_mothra4/2]
 * 
 * 	CHFts14687: need to free address in rpriv_util_get_dce_authz_info
 * 	[1995/03/28  20:43 UTC  aha  /main/HPDCE02/aha_mothra4/1]
 * 
 * 	fix for CHFts13993 (and OSF CR# 12770).
 * 	[1995/01/10  16:35 UTC  greg  /main/HPDCE02/2]
 * 
 * 	fix for CHFts13993 (and OSF CR# 12770)
 * 	[1995/01/10  15:10 UTC  greg  /main/HPDCE02/greg_mothra_bugfix/1]
 * 
 * 	fix bug described in OT 12770.
 * 	Add rpriv_util_finish_ps_eblock().
 * 	[1994/12/22  13:56 UTC  greg  /main/greg_migration2/1]
 * 
 * 	[1994/12/15  03:25 UTC  rps  /main/HPDCE02/1]
 * 
 * 	[OT12669] cell renaming
 * 	[1994/12/14  23:57 UTC  rps  /main/rps_mothra/1]
 * 
 * Revision 1.1.12.1  1994/10/25  20:23:39  kline_s
 * 	UHYP CR 12343: Enable Delegation audit points: utilize dce_aud_start_with_uuid()
 * 	to prevent audit from causing secd to go into an infinite loop.
 * 	[1994/10/25  20:20:25  kline_s]
 * 
 * Revision 1.1.2.10  1994/10/05  18:03:25  sekhar
 * 	[ OT 12466 ] Check return status from krb5_copy_principal() .
 * 	[1994/10/05  14:54:14  sekhar]
 * 
 * Revision 1.1.2.9  1994/09/28  20:50:22  greg
 * 	CR11910: pass delegation chain in priv_attr_trig_query calls.
 * 	[1994/09/28  18:02:13  greg]
 * 
 * Revision 1.1.7.2  94/09/21  9:44:54  greg
 * 	Add delegate info to priv_attr_trig callouts.
 * 
 * Revision 1.1.2.8  1994/08/25  16:19:42  mdf
 * 	Merge with Sue's changes.
 * 	[1994/08/25  14:44:48  mdf]
 * 
 * 	Ensure the  DCE 1.1 version of priv_attr_trig.idl
 * 	defines the protocol the way we expect it to work for a while,
 * 	minimally.
 * 	[1994/08/24  18:45:36  mdf]
 * 
 * Revision 1.1.2.7  1994/08/24  21:07:05  cuti
 * 	Add audit points.
 * 	[1994/08/24  20:08:00  cuti]
 * 
 * Revision 1.1.2.6  1994/08/18  20:25:36  greg
 * 	Flush the FAKE-EPAC authorization data protocol and use
 * 	the real 1.1 protocol instead.
 * 	[1994/08/16  19:28:34  greg]
 * 
 * Revision 1.1.2.5  1994/08/04  16:13:20  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/28  11:41:13  mdf]
 * 
 * Revision 1.1.2.4  1994/07/15  15:01:10  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:18:55  mdf]
 * 
 * Revision 1.1.3.3  1994/05/24  15:17:47  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 
 * Revision 1.1.2.1  1994/05/11  19:27:45  ahop
 * 	hp_sec_to_osf_2 drop
 * 	first cut at a real delegation token.
 * 	Use priv_attr interface.
 * 	add memory management function parameters to sec_encode interfaces
 * 	add common attribute processing code.
 * 
 * 	hp_sec_to_osf_2 drop
 * 	[1994/04/29  21:39:54  ahop]
 * 
 * $EndLog$
 */

/*
** Copyright (c) Hewlett-Packard Company 1994, 1995
** Unpublished work. All Rights Reserved.
*/

/*
 *     Privilege Server Common routines
 */

#include <rpriv_util.h>

#include <krb5/copyright.h>
#include <krb5/kdb.h>
#include <krb5/func-proto.h>
#include <krb5/los-proto.h>
#include <krb5/asn1.h>

#include <bstring.h>
#include <un_strng.h>
#include <un_maloc.h>

#include <rs_server.h>
#include <rs_lock.h>
#include <rs_ch_lock.h>
#include <rs_util.h>

#include <rsdb_acct.h>
#include <rsdb_policy.h>
#include <rsdb_pgo.h>
#include <rsdb_name_util.h>

#include <sec_krb_util.h>
#include <sec_pkl.h>
#include <sec_id_pac_util.h>
#include <priv_krb_util.h>
#include <u_str.h>
#include <rpriv_attr.h>

#include <dce/sec_authn.h>
#define PTHREAD_EXC        /* make one version of pthread_exc.h harmless */
#define PTHREAD_EXC_H      /* make another version of pthread_exc.h harmless */
#include <dce/stubbase.h>  /* includes pthread_exc.h */

#include <authz_util.h> 
#include <dce/authz_base.h>

#include <dce/audit.h>
#include <sec_aud_evts.h>

extern dce_aud_trail_t sec_aud_trail;


boolean32 priv_debug = false;

/*
 * The following declarations are needed to make use of kerberos
 * KDC library routines/data  whose signatures/declarations are not
 * exported in any public kerberos header files.  It's either that, or
 * put the priv server source in the kdc source library
 */
krb5_error_code kdc_process_tgs_req (
    krb5_kdc_req *request,
    krb5_fulladdr *from,
    krb5_data *pkt,
    krb5_tkt_authent **ret_authdat
);

extern krb5_encrypt_block master_encblock; /* referenced by macros in kdb.h */

/*
 * Private routines
 */


/* p a d a t a _ t o _  p a c
 *
 * Convert v.1.1 pa data to a v1.0 PAC.  To the extent that
 * the pa data resembles a pac, we can just alias the
 * storage in the pa data, but not so with foreign
 * groupsets, which are defined differently.  Any
 * storage which cannot be aliased is allocated using
 * rpc_ss_allocate(), so the RPC stub will garbage collect
 * it for us.
 */
PRIVATE  void  pa_data_to_pac(
    sec_id_pa_t  *pa,
    sec_id_pac_t *pac
)
{
    unsigned16    i, j, cur_group;

    memset(pac, 0, sizeof(*pac));
    pac->authenticated = true;
    pac->pac_type = sec_id_pac_format_v1;
    pac->realm = pa->realm;
    pac->principal = pa->principal;
    pac->group = pa->group;
    pac->num_groups = pa->num_groups;
    pac->groups = pa->groups;

    /* Need to "flatten" the epac foreign groupset
     * structure into a v1_pac structure
     */

    /* count the total number of foreign groups in the epac */
    pac->num_foreign_groups = 0;
    for (i = 0; i < pa->num_foreign_groupsets; i++)
	pac->num_foreign_groups += pa->foreign_groupsets[i].num_groups;

    if (pac->num_foreign_groups > 0) {
	pac->foreign_groups = (sec_id_foreign_t *)
	    rpc_ss_allocate( pac->num_foreign_groups * sizeof(sec_id_foreign_t));

	/* now do the "copy" */
	cur_group = 0;
	for (i = 0; i < pa->num_foreign_groupsets; i++) {
	    for (j = 0; j < pa->foreign_groupsets[i].num_groups; j++) {
		pac->foreign_groups[cur_group].realm
		    = pa->foreign_groupsets[i].realm;
		pac->foreign_groups[cur_group].id
		    = pa->foreign_groupsets[i].groups[j];
		cur_group++;
	    }
	}
    }
}


/* r p r i v _ k r b _ g e t _ r e q _ a u t h d a t a
 *
 * decrypt and decode encrypted authdata.
 */
PRIVATE boolean32  rpriv_krb_get_req_authdata(
     krb5_enc_data       *enc_authdata,
     krb5_encrypt_block  *eblock,
     krb5_authdata       ***authdata,
     error_status_t      *stp)
{
    krb5_data             scratch;
    krb5_authdata         **ap;

    CLEAR_STATUS(stp);
    *authdata = NULL;

    /*
     * No authdata is not necessarily an error.  We leave it to
     * the caller to sort that out.
     */
    if (! (enc_authdata && enc_authdata->ciphertext.data)) {
        return false;
    }

    if (!valid_etype(enc_authdata->etype)) {
        *stp = KDC_ERR_ETYPE_NOSUPP;
	return false;
    }

    scratch.length = enc_authdata->ciphertext.length;
    if (!(scratch.data =
          malloc(enc_authdata->ciphertext.length))) {
        SET_STATUS(stp, sec_priv_s_no_mem);
        return false;
    }

    /* call the encryption routine */
    *stp = krb5_decrypt((krb5_pointer) enc_authdata->ciphertext.data,
                              (krb5_pointer) scratch.data,
                              scratch.length, eblock, 0);
    if (STATUS_OK(stp)) {
	*stp = decode_krb5_authdata(&scratch, &ap);
    }
    free(scratch.data);
    if (STATUS_OK(stp)) {
	*authdata = ap;
	return true;
    }
    return false;
}


/*
 * check a principal name to see it is the priv server for
 * the local cell
 */
PRIVATE boolean32 is_priv_server(
    krb5_principal  princ,
    error_status_t  *stp
)
{
    static krb5_principal   priv_princ = NULL;

    CLEAR_STATUS(stp);

    /*
     * priv_princ can be a static pointer in a multi-threaded
     * environment if we assume an atomic write to the pointer - the
     * worst that will happen is that we will waste a little memory
     * when multiple threads all determine the default value hasn't
     * been set yet and go off and ask for the value.
     */

    if (priv_princ == NULL) {
        (void) krb5_parse_name((char *)SEC_PRIV_SERVER_NAME, &priv_princ);
    }
    if (priv_princ != NULL) {
	if (krb5_principal_equiv(princ, priv_princ)) {
            return 1;
        }
    }

    SET_STATUS(stp, sec_priv_s_invalid_request);
    return 0;
}


/* r p r i v _ k r b _ t r u s t _ p a t h _ v a l i d
 *
 * Evaluate the transited path from the authentication header ticket using
 * the DCE trust path evaluation algorithm.
 *
 */
PRIVATE  error_status_t  rpriv_krb_trust_path_valid(
    krb5_enc_tkt_part	*enc_tkt
)
{
    error_status_t  st;
    krb5_transited  *trans = &enc_tkt->transited;
    krb5_data	    lrealm;

    CLEAR_STATUS(&st);

    if (trans->tr_contents.length && *trans->tr_contents.data) {
	krb5_get_default_realm(&lrealm.data);
	lrealm.length = strlen(lrealm.data);
	st = krb5_check_transited_list(&trans->tr_contents,
				krb5_princ_realm(enc_tkt->client), &lrealm);
	free(lrealm.data);
	if (st)
	    SET_STATUS(&st, sec_priv_s_invalid_trust_path);
    }

    return st;
}


/*
 * retrieve the current key of the local ticket-granting-ticket
 * service from the database and, since we have to construct it
 * and it will be useful later on, the kerberos V5 formatted
 * ticket-granting-ticket service principal name as well.
 */
PRIVATE  void  rpriv_util_get_tgt_info(
    rpriv_krb_req_info_t    *req_info,        /* [in] */
    krb5_principal  *tgt_princ,  /* out */
    krb5_keyblock   *tgt_key,    /* [out] */
    krb5_kvno       *tgt_kvno,   /* [out] */
    error_status_t  *stp         /* [out] */
)
{
    unsigned_char_t       *cell_name;
    krb5_principal        tgtp;
    int                   num_db_entries = 1;
    krb5_boolean          dup;   /* duplicate principal entries? */
    krb5_db_entry         tgt_entry;
    krb5_enctype    etype = req_info->etype;

    *tgt_princ = NULL;
    memset(tgt_key, 0, sizeof(*tgt_key));

    *stp = krb5_tgtname (krb5_princ_realm(req_info->authent->ticket->server),
			 krb5_princ_realm(req_info->authent->ticket->server),
			 &tgtp);
    if (STATUS_OK(stp)) 
    {
	if (!kdc_principal_is_tgs(tgtp)) {
	    *stp = KRB5KRB_AP_ERR_NOT_US;
	    krb5_free_principal (tgtp);
	    return;
	}
    }

    if (STATUS_OK(stp)) {
	*stp = krb5_db_get_principal(tgtp,
		   krb5_csarray[etype]->system->proto_keytype,
		   0, &tgt_entry, &num_db_entries, &dup);

	if ((*stp != 0) || (dup != 0)) {
	    if (dup) {
		krb5_db_free_principal(&tgt_entry, num_db_entries);
	    }
	    krb5_free_principal(tgtp);
	    return;
	}
    }

    *tgt_kvno = tgt_entry.kvno;
    *stp = KDB_CONVERT_KEY_OUTOF_DB(&tgt_entry.key, tgt_key);

    /* done with tgt_entry, in any case */
    krb5_db_free_principal(&tgt_entry, num_db_entries);

    if (!STATUS_OK(stp)) {
	krb5_free_principal(tgtp);
	return;
    }

    *tgt_princ = tgtp;
}


/*
 * PUBLIC Routines
 */

/* p r i n t _ i d
 *
 * for debugging purposes
 *
 * "preamble" may be an empty string, but may NOT be NULL.
 */
extern  void print_id(
    sec_id_t  *id,
    char      *preamble
)
{
    unsigned_char_p_t  uuid_string;
    error_status_t     lst;

    uuid_to_string(&(id->uuid), &uuid_string, &lst);
    if (GOOD_STATUS(&lst)) {
        if (id->name) {
            printf("%s: <%s> <%s>\n", preamble, id->name, uuid_string);
        } else {
            printf("%s: <Unknown> <%s>\n", preamble, uuid_string);
        }

        rpc_string_free(&uuid_string, &lst);
    }

}


/* r p r i v _ u t i l  _ a l l o c _ k r b _ r e q _ i n f o
 *
 */
extern  void  rpriv_util_alloc_krb_req_info(
    rpriv_krb_req_info_t  **auth_info   /* [out] */
)
{
    *auth_info = (rpriv_krb_req_info_t *) malloc(sizeof(rpriv_krb_req_info_t));
    if (*auth_info == NULL) {
        return;
    }

    memset(*auth_info, 0, sizeof(rpriv_krb_req_info_t));

    /* authentication header gets allocated in kdc_process_tgs_request */
    /* req_authdata, if any, is allocated in get_req_info */
}


/* r p r i v _ u t i l _ f r e e  _ k r b _ r e q _ i n f o
 *
 */
extern  void  rpriv_util_free_krb_req_info(
    rpriv_krb_req_info_t  **auth_info   /* [in, out] */
)
{
    if (*auth_info == NULL) {
        return;
    }

    if ((*auth_info)->eblock.crypto_entry != NULL) {
        krb5_finish_key(&(*auth_info)->eblock);
    }

    if ((*auth_info)->authent != NULL) {
        if ((*auth_info)->authent->authenticator != NULL) {
            krb5_free_authenticator((*auth_info)->authent->authenticator);
        }
        if ((*auth_info)->authent->ticket != NULL) {
            krb5_free_ticket((*auth_info)->authent->ticket);
        }
        free( (char *) (*auth_info)->authent);
    }

    if ((*auth_info)->req_authdata != NULL) {
	krb5_free_authdata((*auth_info)->req_authdata);
    }

    free((char *) *auth_info);
    *auth_info = NULL;
}

/*
 * The 1.1 priv server only knows about rsa_md5 seals.
 */

/*
 * Look up the priv server key of the requested version and
 * type, and use it to generate an encryption block.
 * 
 * The key version is both input and output so that
 * we can request the "current" version and return the actual
 * version retrieved.  
 *
 * ENHANCEMENT
 *
 * If the "current" version is requested, we always have to
 * perform a database lookup, because the key could change at any
 * time. We could maintain a small cache of previously retreived keys
 * to minimize the database lookups needed when a specific key version 
 * other than the current one is requested (i.e. when decrypting incoming
 * delegation tokens).
 */
extern  void  rpriv_util_get_ps_eblock (
    krb5_enctype         etype,      /* [in] */
    krb5_kvno            req_kvno,   /* [in] */
    krb5_encrypt_block   *ps_eblock, /* [out] */
    krb5_kvno            *ps_kvno,   /* [out] */
    error_status_t       *stp        /* [out] */
)
{
    unsigned_char_t       *cell_name;
    krb5_principal        ptgtp;
    int                   num_db_entries = 1;
    krb5_boolean          dup;   /* duplicate principal entries? */
    krb5_db_entry         ptgt_entry;
    krb5_keyblock         *ps_key;

    ps_key = (krb5_keyblock *) malloc(sizeof(*ps_key));
    if (!ps_key) {
	SET_STATUS(stp, sec_priv_s_no_mem);
	return;
    }
    ps_key->contents = NULL;  
    ps_key->length = 0;
    ps_eblock->priv = ps_eblock->key = NULL;
    
    /* get the principal data for the local realm */
    dce_cf_get_cell_name((char **) &cell_name, stp);
    
    if (STATUS_OK(stp)) {
	sec_krb_ptgs_parsed_name( (char *) cell_name, &ptgtp, stp);
	free(cell_name);
    }     
    
    if (STATUS_OK(stp)) {
	*stp = krb5_db_get_principal(ptgtp,
		   krb5_csarray[etype]->system->proto_keytype,
		   req_kvno, &ptgt_entry, &num_db_entries, &dup);
    
	krb5_free_principal(ptgtp);
	if ((*stp != 0) || (dup != 0)) {
	    if (dup) {
		krb5_db_free_principal(&ptgt_entry, num_db_entries);
	    }
	    return;
	}
    }

    *ps_kvno = ptgt_entry.kvno;
    *stp = KDB_CONVERT_KEY_OUTOF_DB(&ptgt_entry.key, ps_key);

    /* done with ptgt_entry, in any case */
    krb5_db_free_principal(&ptgt_entry, num_db_entries);  

    krb5_use_cstype(ps_eblock, etype);
    if (STATUS_OK(stp)) {
	*stp = krb5_process_key(ps_eblock, ps_key);
	if (!STATUS_OK(stp)) {
	    krb5_free_keyblock(ps_key);
	}
    }
}
    

extern  void  rpriv_util_finish_ps_eblock (
    krb5_encrypt_block   *ps_eblock /* [in] */
)
{
    if (ps_eblock) {
	if (ps_eblock->priv) {
	    krb5_finish_key(ps_eblock);
	}
	if (ps_eblock->key) {
	    krb5_free_keyblock(ps_eblock->key);
	}
    }
}

/* r p r i v _ u t i l _ g e t _ k r b _ r e q _ i n f o
 *
 * retrieve and verify authentication info when the underlying
 * authentication service is kerberos.  If the request passes various
 * checks that ensure it is a legitimate priv server request, then we
 * return a filled in rpriv_krb_req_info_t.
 */
extern  void  rpriv_util_get_krb_req_info(
    krb5_data             *krb_req,
    rpriv_krb_req_info_t  **req_info,
    error_status_t        *stp
)
{
    krb5_kdc_req          *dec_req;
    rpriv_krb_req_info_t  *auth_info;
    krb5_fulladdr         client_addr; /* placeholder */

    CLEAR_STATUS(stp);
    *req_info = NULL;

    /* decode the kdc_req for the priv server */
    *stp = decode_krb5_tgs_req(krb_req, &dec_req);
    if (!STATUS_OK(stp)) {
        return;
    }

    rpriv_util_alloc_krb_req_info(&auth_info);
    if (auth_info == NULL) {
        SET_STATUS(stp, sec_priv_s_no_mem);
        krb5_free_kdc_req(dec_req);
        return;
    }

    /*XXX need client addrs for any reason?? they'd have to come over in the
     * request somehow.  In a real TGS req they're available from the transport
     * mechanism
     */
    memset(&client_addr, 0, sizeof(client_addr));
    WRITE_LOCK (lock_kdc) {
        *stp = kdc_process_tgs_req(dec_req, &client_addr, krb_req,
				   &(auth_info)->authent);
    } END_WRITE_LOCK;

    /*
     * If we successfully decode the AP_REQ, then check the server
     * field in the header ticket to be sure it's the priv server
     */
    if (STATUS_OK(stp)
    && is_priv_server(auth_info->authent->ticket->server, stp)) {

	int i;
	
	/*
	 * set up eblock for subkey.  It's used all over the place.
	 * Use a requested etype that we support.
	 */
	
	
	for (i = 0; i < dec_req->netypes; i++) {
	    if (valid_etype(dec_req->etype[i])) {
		break;
	    }
	}
	
	if (i == dec_req->netypes) {
	    /* unsupported etype */
	    *stp = KDC_ERR_ETYPE_NOSUPP;
	} else {
	    (auth_info)->etype = dec_req->etype[i];
	}

	if (STATUS_OK(stp)) {
	    krb5_use_cstype(&auth_info->eblock, auth_info->etype);

	    *stp = krb5_process_key(&auth_info->eblock,
		       auth_info->authent->authenticator->subkey
		       ? auth_info->authent->authenticator->subkey
		       : auth_info->authent->ticket->enc_part2->session);
	}
	if (STATUS_OK(stp)) {
	    /*
	     * get the authorization data, if any, that came over with the
	     * request
	     */
	    (void) rpriv_krb_get_req_authdata(&dec_req->authorization_data,
					      &auth_info->eblock,
					      &auth_info->req_authdata,  stp);
	}
	
	auth_info->nonce = dec_req->nonce;
    }

    /* we're done with the decode request, in any event */
    krb5_free_kdc_req(dec_req);

    if (STATUS_OK(stp)) {
	*req_info = auth_info;
    } else {
	rpriv_util_free_krb_req_info(&auth_info);
    }
}


extern  void  rpriv_util_free_authz_data(
    rpriv_dce_authz_data_t  *authz_data
)
{
    if (!authz_data) return;
}




/*  r p r i v _ u t i l _ g e t _ d c e _ a u t h z _ i n f o
 *
 * Extract the DCE information from the  requisite components of the
 * KRB5_TGS_REQ info
 */
extern  void  rpriv_util_get_dce_authz_info (
    unsigned32              aud_event,           /* [in] */ 
    handle_t                handle,              /* [in] */   
    rpriv_krb_req_info_t    *krb_req_info,       /* [in] */
    rpriv_dce_authz_data_t  *authz_data,         /* [out] */
    sec_id_pac_t            **req_authz_data,    /* [out] local pre-1.1 only */
    dce_aud_rec_t           *ard,                /* [out] */
    error_status_t          *stp                 /* [out] */
)
{
    krb5_principal  ptgt_princ;
    char                  *cpp = NULL, *ccp = NULL;
    krb5_tkt_authent      *authent = krb_req_info->authent;
    idl_pkl_t             *ppac;

#if 0
#if defined(DCE_AUD_POINTS)
    dce_aud_ev_info_t     ev_info;
    unsigned_char_t       *address;
    uuid_t                cell_uuid, princ_uuid;
    error_status_t        aud_status, aud_status2, ignore_status;
#endif	/* defined(DCE_AUD_POINTS) */
#endif

    CLEAR_STATUS(stp);

    memset (authz_data, 0, sizeof(*authz_data));

    /*
     * Make sure the transited path conforms to the rules
     * for hierarchical trust in DCE.
     */
    *stp = rpriv_krb_trust_path_valid(authent->ticket->enc_part2);
    if (BAD_STATUS(stp)) {
	return;
    }

    /*
     * Parse and canonicalize the client name.  It's used for various things
     * later on.
     */
    sec_dce_pair_from_krb_princ(authent->ticket->enc_part2->client,
                                &ccp, &cpp, stp);

    if (STATUS_OK(stp)) {
	strncpy((char *) authz_data->client_cell, ccp,
		sizeof(authz_data->client_cell));
	authz_data->client_cell[sizeof(authz_data->client_cell) -1] = '\0';
	rsdb_name_canonicalize(authz_data->client_cell, stp);
    }

    if (STATUS_OK(stp)) {
	strncpy((char *) authz_data->client_princ, cpp,
		sizeof(authz_data->client_princ));
	authz_data->client_princ[sizeof(authz_data->client_princ) -1] = '\0';
        rsdb_name_canonicalize(authz_data->client_princ, stp);
    }


/*
 * This audit point is being disabled because name-based 
 * authentication forces the use of dce_aud_start_with_name()
 * which in turn calls sec_id_parse_name(). In the local case
 * sec_id_parse_name() has been fixed to make internal calls
 * when running inside of secd. In the intercell case, however,
 * it needs to call out to the remote secd. The remote secd will
 * then audit the call, and when trying to sec_id_parse_name() our
 * name, will call back to us and deadlock. 
 *
 * The cause of all of these problems is a fundamental design
 * deficiency in the audit susbsytem in which it has severe problems 
 * running inside the TCB. Work is required to redesign the audit
 * subsystem to not rely on security services to audit security events.
 */

#if 0
#if defined(DCE_AUD_POINTS)
    if (STATUS_OK(stp)) {
        unsigned_char_t         *global_name = NULL;

        rpc_binding_to_string_binding(handle, &address, &aud_status2);

        if (STATUS_OK(&aud_status2)) {
            global_name = (unsigned_char_t *)malloc(sizeof(authz_data->client_cell) 
                                              + sizeof(authz_data->client_princ)
                                              + 2);
            if (global_name == NULL) {
                *stp = sec_priv_s_no_mem;
                return;
            }

            sprintf((char *)global_name, "%s/%s", authz_data->client_cell, 
                    authz_data->client_princ);

            dce_aud_start_with_name(aud_event, global_name, 
                                    address, 0, aud_c_esl_cond_unknown, 
                                    ard, &aud_status);
            rpc_string_free(&address, &ignore_status);
            free(global_name);
        }
        else 
            *ard = NULL;  /* just in case the rpc call failed */
    }
#endif	/* defined(DCE_AUD_POINTS) */
#endif /* 0 */


    if (STATUS_OK(stp)) {
        krb5_principal  tgt_princ;
	char            *tgtcp, *tgtpp;

	/*
	 * obtain the name of the TGT principal for the client's cell
	 */
        sec_krb_tgs_parsed_name(ccp, &tgt_princ, stp);
        if (STATUS_OK(stp)) {
            sec_dce_pair_from_krb_princ(tgt_princ, &tgtcp, &tgtpp, stp);
            krb5_free_principal(tgt_princ);
	    if (STATUS_OK(stp)) {
		strncpy((char *) authz_data->client_tgt_princ, tgtpp,
			sizeof(authz_data->client_tgt_princ));
		authz_data->client_tgt_princ[sizeof(authz_data->client_tgt_princ) -1] = '\0';
		rsdb_name_canonicalize(authz_data->client_tgt_princ, stp);
		free(tgtcp); free(tgtpp);
	    }
	}
    }

    if (ccp != NULL) free(ccp);  if (cpp != NULL) free(cpp);

    /*
     * check the client cell name to determine whether or not the
     * client is "local"
     */
    authz_data->local = rsdb_util_is_realm_local(
				authz_data->client_cell, NULL, stp);
    if (BAD_STATUS(stp)) {
        return;
    }

    /*
     * Intercell cell requests are accepted only from other privilege
     * servers so the  client principal name had better be the
     * architecturally defined priv server name.
     * 
     * Intracell requests using the architectural priv server name are
     * treated like intercell requests.
     */

    sec_krb_ptgs_parsed_name((char *)authz_data->client_cell,
				 &ptgt_princ, stp);
    if (BAD_STATUS(stp)) {
        return;
    }
    if (! sec_krb_name_parsed_cmp_princ (ptgt_princ,
					 authent->ticket->enc_part2->client))
    {
	if (!authz_data->local) {
	    SET_STATUS(stp, sec_priv_s_invalid_request);
	}
    } else {
	authz_data->local = FALSE;
    }
    krb5_free_principal(ptgt_princ);
    if (BAD_STATUS(stp)) {
        return;
    }

    /*
     * Now check the authorization data field of the ticket for
     * DCE auth data.  DCE authdata may be present in the any
     * of the following combinations.
     *
     * 0) none
     *
     *    local 1.0 or 1.1 request.  we'll figure  that out later.
     *
     * 1) 1.0.x data only (a v1 PAC)
     *
     *    In this case, we're dealing with an intercell request
     *    under the 1.0 protocol.
     *
     * 2) 1.0.x data and 1.1 data
     *
     *    1.1 data consist of a seal over the caller's EPAC
     *    chain (presented as a distinct argument in a 1.1
     *    priv server call and, if delegation has been enabled,
     *    a delegation token.  The delegation token is just a copy
     *    of the EPAC seal protected by the priv server's key.
     *
     *    The presence of 1.0 data indicate that the client has
     *    enabled 1.0 compatibility
     *
     * 3) 1.1 data only
     *
     *    An intercell 1.1 request.
     */

    if (authent->ticket->enc_part2->authorization_data) {
	unsigned32  ad_parts;
	
	ad_parts = priv_krb_util_parse_v0_authdata(rpc_ss_allocate, 
			   rpc_ss_free,
			   authent->ticket->enc_part2->authorization_data,
			   &authz_data->v1_1_ad, 
			   &ppac, stp);
	if (STATUS_OK(stp)) {
	    if (FLAG_SET(ad_parts, priv_ad_part_pac)) {
		error_status_t  local_st;
		
		sec_id_pac_t_unpickle(ppac, (void *(*)(unsigned32)) malloc,
				      &authz_data->v1_pac, &local_st);
		/* ppac will be freed in the server stub on return*/
		if (!STATUS_OK(&local_st)) {
		    UNSET(ad_parts, priv_ad_part_pac);
		}
	    }
	    
	    authz_data->data_parts = ad_parts;
	} else {
	    /*
	     * Don't complain about authdata failures now.  Anything
	     * missing that's needed later on will be reported then.
	     */
	    authz_data->data_parts = priv_ad_part_none;
	    CLEAR_STATUS(stp);
	}
    }
    
    /*
     * if there was any authorization data in the krb5_kdc_req, then
     * it would be a pickled v1.0 pac, so we take care of that now.
     * The caller may have instructed us to ignore any such data by
     * passing NULL in the  req_authz_data parameter
     */
    if (req_authz_data && krb_req_info->req_authdata) {
	priv_krb_util_authdata_to_pac(krb_req_info->req_authdata, &ppac, stp);
	if (STATUS_OK(stp) && ppac != NULL) {
	    sec_id_pac_t *pac_p = (sec_id_pac_t *) malloc(sizeof(sec_id_pac_t));

	    if (pac_p == NULL) {
		SET_STATUS(stp, sec_priv_s_no_mem);
	    } else {
		sec_id_pac_t_unpickle(ppac, (void *(*)(unsigned32)) malloc,
				      pac_p, stp);
		free((char *)ppac);
		if (STATUS_OK(stp)) {
		    *req_authz_data = pac_p;
		}
	    }
	}

	/*
         * Don't complain about failures now.  Lack of request
	 * authz data will be noticed later on
	 */
	CLEAR_STATUS(stp);
    }
}


/* r p r i v _ u t i l _ v e r i f y _ l o c a l _ p r i n c
 *
 * Verify the requested "primary" info for a principal in our cell.
 * If successfull, the princ_id, group_id, and cell_id
 * parameters are filled in the the appropriate uuids and
 * names.
 */

void rpriv_util_verify_local_princ(
    dce_aud_rec_t  ard,            /* [in] */
    sec_rgy_name_t princ_name,     /* [in] */
    sec_rgy_name_t cell_name,      /* [in] */
    sec_id_t       *req_princ_id,  /* [in] */
    sec_id_t       *req_group_id,  /* [in] */
    sec_id_t       *princ_id,      /* [out] */
    sec_id_t       *group_id,      /* [out] */
    sec_id_t       *cell_id,       /* [out] */
    error_status_t *stp            /* [out] */
)
{
    sec_rgy_sid_t          sid;
    sec_rgy_unix_sid_t     unix_sid;
    sec_rgy_acct_key_t     key_parts;
    sec_rgy_acct_user_t    user_part;
    sec_rgy_acct_admin_t   admin_part;
    sec_rgy_login_name_t   acct;
    sec_rgy_name_t         cell_princ_name;
    sec_rgy_cursor_t       cursor;
#if defined(DCE_AUD_POINTS)
    dce_aud_ev_info_t      ev_info;
    error_status_t         aud_status;
#endif	/* defined(DCE_AUD_POINTS) */

    /*
     * convert the local cell name to a uuid and store both in
     * the cell_id parameter
     */
    strncpy((char *)cell_princ_name, (char *)cell_name,
	    sizeof(cell_princ_name));
    cell_princ_name[sizeof(cell_princ_name) -1] = '\0';
    rsdb_name_canonicalize_and_convert(cell_princ_name, stp);
#if defined(DCE_AUD_POINTS)
    if (ard) {
         ev_info.format = aud_c_evt_info_uuid;
         ev_info.data.uuid = req_princ_id->uuid;
         dce_aud_put_ev_info(ard, ev_info, &aud_status);

         ev_info.format = aud_c_evt_info_uuid;
         ev_info.data.uuid = req_group_id->uuid;
         dce_aud_put_ev_info(ard, ev_info, &aud_status);
    }
#endif	/* defined(DCE_AUD_POINTS) */

    if (STATUS_OK(stp)) {
	rsdb_pgo_name_to_uid(sec_rgy_domain_person,
			     cell_princ_name,
			     &cell_id->uuid,
			     stp);
	if (!STATUS_OK(stp)) {
	    SET_STATUS(stp, sec_priv_s_invalid_request);
	    return;
	}
    }

    /*
     * verify the local principal and primary group uuids
     */
    u_strncpy(acct.pname, princ_name, sizeof(acct.pname));
    acct.pname[sizeof(acct.pname) -1] = '\0';
    acct.gname[0] = '\0';
    acct.oname[0] = '\0';

    rs_util_clear_cursor(&cursor);
    rsdb_acct_lookup(&acct, &cursor, &sid, &unix_sid, &key_parts,
		     &user_part, &admin_part, stp);
    if (STATUS_OK(stp)) {
	error_status_t lst;

	if (!(uuid_equal(&sid.person, &req_princ_id->uuid, &lst)
	|| uuid_equal(&sid.group, &req_group_id->uuid, &lst))) {
	    SET_STATUS(stp, sec_priv_s_invalid_principal);
	}
    } else {
	SET_STATUS(stp, sec_priv_s_invalid_request);
	DEBUG_PRINT( ("rpriv: (%s) - principal not found\n",
		      acct.pname) );
    }

    if (STATUS_OK(stp)) {	
	princ_id->uuid = sid.person;
/*	u_dynstrcpy(&princ_id->name, acct.pname);  No names as of 1.1 */
	princ_id->name = NULL;
        group_id->uuid = sid.group;
/*      u_dynstrcpy(&group_id->name, acct.gname); No names as of 1.1 */
	group_id->name = NULL;
/*	u_dynstrcpy(&cell_id->name, cell_name); No names as of 1.1 */
	cell_id->name = NULL;
    }
}


extern  void  rpriv_util_parse_request (
    unsigned32              aud_event,         /* [in] */
    handle_t                handle,            /* [in] */
    rpriv_pickle_t          *ptgt_req,         /* [in] */
    rpriv_dce_authz_data_t  *authz_data,       /* [out] */
    rpriv_krb_req_info_t    **req_info,        /* [out] */
    dce_aud_rec_t           *ard,              /* [out] */
    error_status_t          *stp               /* [out] */
)
{
    krb5_data               krb_req;

    memset(authz_data, 0, sizeof(*authz_data));
    *req_info = NULL;

    krb_req.length = ptgt_req->num_bytes;
    krb_req.data = (char *) ptgt_req->bytes;

    /*
     * Decode and decrypt the KRB5_TGS_REQ that came over
     * in the ptgt_req.  This requires a READ LOCK on the database
     * When that's done, we should have in hand a legitimate
     * KRB5_AP_REQ.  If not, then bail out.
     */

    BEGIN_READ(lock_db, stp) {
	rpriv_util_get_krb_req_info(&krb_req, req_info, stp);
	if (STATUS_OK(stp)) {
	    rpriv_util_get_dce_authz_info(
	                         aud_event,
                                 handle,
                                 *req_info, 
                                 authz_data, 
                                 NULL,
                                 ard,
                                 stp);
	}
    } END_READ;

    if (!STATUS_OK(stp)) {
	if (req_info != NULL) rpriv_util_free_krb_req_info(req_info);
        rpriv_util_free_authz_data(authz_data);
	return;
    }

}


extern  void  rpriv_util_parse_req_privs (
    sec_bytes_t         *requested_privs,  /* [in] */
    sec_id_epac_data_t  *epac_data,        /* [out] */
    error_status_t      *stp               /* [out] */
)
{
    sec_id_epac_set_t   epac_set;

    /* 
     * There always has to be something in
     * the requested_privs
     */
    if (!requested_privs 
    || !requested_privs->num_bytes
    || !requested_privs->bytes) {
	SET_STATUS(stp, sec_priv_s_invalid_request);
	return; 
    }
    sec_id_epac_set_decode(NULL, NULL,
			   requested_privs->num_bytes,
			   requested_privs->bytes,
			   &epac_set,
			   stp);

    /*
     * There must be one, and only one, epac in requested_privs in a local
     * get_eptgt request.
     */
    if (epac_set.num_epacs != 1
    || !epac_set.epacs[0].pickled_epac_data.num_bytes
    || !epac_set.epacs[0].pickled_epac_data.bytes) {
	SET_STATUS(stp, sec_priv_s_invalid_request);
	return;
    }

    sec_id_epac_data_decode(NULL, NULL,
			    epac_set.epacs[0].pickled_epac_data.num_bytes,
			    epac_set.epacs[0].pickled_epac_data.bytes,
			    epac_data,
			    stp);
}


extern  void  rpriv_util_verify_req_privs (
    dce_aud_rec_t       ard,
    unsigned_char_p_t   client_princ,
    unsigned_char_p_t   client_cell,
    sec_id_epac_data_t  *req_epac_data,
    sec_id_epac_data_t  *granted_epac_data,
    error_status_t      *stp
)
{
    CLEAR_STATUS(stp);
    memset(granted_epac_data, 0, sizeof(*granted_epac_data));

    /*
     * verify the sec_id_pa_t data portion of the requested_privs
     */
    BEGIN_READ(lock_db, stp) {
	rpriv_util_verify_local_princ(
                                      ard,
                                      client_princ,
				      client_cell,
				      &req_epac_data->pa.principal,
				      &req_epac_data->pa.group,
				      &granted_epac_data->pa.principal,
				      &granted_epac_data->pa.group,
				      &granted_epac_data->pa.realm,
				      stp);
	
	if (STATUS_OK(stp)) {
	    rpriv_util_verify_local_groups(ard,
                                           client_princ,
					   req_epac_data->pa.num_groups,
					   req_epac_data->pa.groups,
					   &granted_epac_data->pa.num_groups,
					   &granted_epac_data->pa.groups,
					   stp);
	}
    } END_READ;
}

extern  void  rpriv_util_verify_local_groups(
    dce_aud_rec_t  ard,
    unsigned char  *principal,         /* [in] - principal whose groupset needs
				                   verification */
    unsigned16     req_num_groups,     /* [in] - num requested local groups */
    sec_id_t       *req_groups,        /* [in] - requested local groupset */
    unsigned16	   *num_groups,        /* [out] - num local groups verified */
    sec_id_t       **verified_groups,  /* [out] - verifed local groupset */
    error_status_t *stp                /* [out] */
)
{
    sec_id_t            *groups;
    unsigned16          i;
    boolean32           is_mem  = false;
    sec_rgy_cursor_t    cursor;
    sec_rgy_name_t      gname;
    sec_rgy_pgo_item_t  pgo;
    error_status_t      lst, lst2;
#if defined(DCE_AUD_POINTS)
    dce_aud_ev_info_t   ev_info;
    error_status_t      aud_status;
#endif	/* defined(DCE_AUD_POINTS) */   

    *num_groups = 0;
    *verified_groups = NULL;
    CLEAR_STATUS(stp);

    /*
     * Allocate enough space to hold all requested groups, though
     * we might not use it all.
     */
    if (req_num_groups == 0) {
	return;
    }
	
    groups = rpc_ss_allocate(req_num_groups * sizeof(*groups));
    if (! groups) {
	SET_STATUS(stp, sec_priv_s_no_mem);
	return;
    }

    for (i = 0; i < req_num_groups; i++) {

	/*
	 * if group does not exist, or the group is not legal on a projlist,
	 * or the principal is not a member of the group, then move
	 * on to the next group
	 */
	rs_util_clear_cursor(&cursor);
	do {
	    rsdb_pgo_get_by_uid(sec_rgy_domain_group,
				&req_groups[i].uuid,
				true, &cursor, gname, &pgo, &lst);

	    if (STATUS_OK(&lst) && FLAG_SET(pgo.flags,
					    sec_rgy_pgo_projlist_ok)) {
		is_mem = rsdb_pgo_is_member(sec_rgy_domain_group, gname,
					    principal, &lst2);
	    } else {
		if (priv_debug) {
		    char buf[128];
		    sprintf(buf,
			    "Unable to add group to pac - status = %x",
			    lst);
		    print_id(&req_groups[i], buf);
		}
	    }
	} while (GOOD_STATUS(&lst) && (! is_mem));

	if (is_mem) {

	    /*
	     * This group is legitimately part of the principal's
	     * credentials, so put it in the groupset
	     */
	    groups[*num_groups].uuid =	req_groups[i].uuid;
/* no names in 1.1 credentials
	    u_dynstrcpy(&groups[*num_groups].name, (char *) gname);
 */
	    groups[*num_groups].name = NULL;
	    *num_groups += 1;

	    if (priv_debug) {
		print_id(&groups[i], "rpriv_get_ptgt: group added to pac:");
	    }
	} else if (GOOD_STATUS(&lst)) {
	    if (priv_debug) {
		print_id(&req_groups[i],
			 "rpriv_get_ptgt: requested group not in groupset");
	    }
	}

    } /* end for */

#if defined(DCE_AUD_POINTS)
    /* 
     * Of the requested_pac groupset, provide audit information
     * only for those which are valid locally.
     */

    if (ard) {
	ev_info.format = aud_c_evt_info_ushort_int;
	ev_info.data.ushort_int = *num_groups;
	dce_aud_put_ev_info(ard, ev_info, &aud_status);

	for (i = 0; (unsigned16) i < *num_groups; i++) {
	    ev_info.format = aud_c_evt_info_uuid;
	    ev_info.data.uuid = groups[i].uuid;
	    dce_aud_put_ev_info(ard, ev_info, &aud_status);
	}
    }
#endif	/* defined(DCE_AUD_POINTS) */


    if (req_num_groups != 0 && *num_groups == 0 ) {
	SET_STATUS(stp, sec_priv_s_not_member_any_group);
	free((char *) groups);
	DEBUG_PRINT(("rpriv_get_ptgt: initiator not in any group\n"));
    } else {
	*verified_groups = groups;
    }
}


extern  void  rpriv_util_verify_local_attrs(
    unsigned_char_p_t   client_princ,    /* [in] - client principal name */
    sec_id_epac_data_t  *granted_epac,   /* [in] - certified principal epac */
    unsigned32          num_upstream_delegates, /* [in] */ 
    sec_id_epac_data_t  upstream_delegates[],   /* [in] - certified delegates */
    unsigned32          num_req_attrs,  /* [in] - num requested attrs */
    sec_attr_t          req_attrs[],    /* [in] - requested attr set */
    unsigned32	        *num_attrs,     /* [out] - num local attrs verified */
    sec_attr_t          **attrs,        /* [out] - verifed attr set */
    error_status_t      *stp            /* [out] - return status */
)
{

    CLEAR_STATUS(stp);
    rpriv_attr_get_effective(client_princ, granted_epac, 
			     num_upstream_delegates, upstream_delegates,
			     num_req_attrs, req_attrs, 
			     true, NULL,
			     num_attrs, attrs,
			     stp);
			     
    /*
     * No need to free anything in the error case 'cause we
     * used rpc_ss_allocate(), meaning the stub will automatically
     * free it for us.   Errors are ok, because it's up to the client
     * requesting attributes or a server expecting them to determine what
     * to do when attributes that it expects are not there.
     */
    if (BAD_STATUS(stp)) {
        *num_attrs = 0;
	CLEAR_STATUS(stp);
    }
}


/*  r p r i v  _ u t i l  _ g e n _ v 1  _ 1 _  p r i v s
 *
 * This is a *very* busy routine.
 *
 * Construct a v0 dce authorization data stream and stuff it into
 * krb5_authdata suitable for inclusion in a kerberos v5 ticket.
 *
 * WARNING! The krb5_authdata is allocated using rpc_ss_allocate(), 
 * so it would be a BIG mistake to use the krb5_free_authdata()
 * routine to free it.  Just leave it alone, and the storage will
 * be garbage-collected by the runtime when the manger routine
 * exits.
 *
 * INPUTS
 *
 *   privs               - a single epac representing the privileges
 *                         of the last, and possibly only, epac to
 *                         be included in the granted privs.
 *
 *   num_upstream_privs  - number of initiator plus intermediary epacs,
 *                         if any, to be included in the granted privs.
 *                         May be 0.
 *
 *   upstream_privs      - initiator plus intermediary epacs, if any,
 *                         to be included in the granted privs.  May
 *                         be NULL.
 *
 * OUTPUTS
 *
 *   granted_privs       - an encoded epac set containing the granted
 *                         privileges.
 *
 *   authdata            - The v0 DCE authorization data field of an EPTGT,
 *                         in krb5_authdata format. Will always contain a 
 *                         seal of the granted_privs and may contain a 
 *                         delegation token and a v1_0 pickled PAC.
 */
extern  void  rpriv_util_gen_v1_1_privs(
    sec_id_epac_data_t  *privs,             /* [in] */
    unsigned32          num_upstream_privs, /* [in] */
    sec_id_epac_data_t  *upstream_privs,    /* [in] */
    unsigned32          expiration,         /* [in] */
    sec_bytes_t         *granted_privs,     /* [out] */
    krb5_authdata       ***authdata,        /* [out] */
    error_status_t      *stp                /* [out] */
)
{
    sec_id_epac_set_t      epac_set;
    unsigned32             cur_epac, num_epacs;
    sec_v1_1_authz_data_t  v1_1_authz_data;
    sec_id_seal_set_t      seal_set;
    sec_id_seal_t          seal;
    sec_dlg_token_set_t    token_set;
    sec_dlg_token_t        token;
    idl_byte               token_bytes[V0_REQ_TOKENSIZE],
                           scratch[V0_TOKEN_REQ_CLEAR_LEN];
    sec_id_pac_t           v1_pac;
    sec_authz_mp_t         mp;
    
    memset(&v1_1_authz_data, 0, sizeof(v1_1_authz_data));
    uuid_create(&v1_1_authz_data.session_id, stp);
    if (!STATUS_OK(stp)) {
	return;
    }
    v1_1_authz_data.seals = &seal_set;
    seal_set.num_seals = 1;
    seal_set.seals = &seal;
    seal.seal_type = sec_id_seal_type_md5;
  
    num_epacs = 1 + num_upstream_privs;
    epac_set.num_epacs = 0;
    epac_set.epacs = rpc_ss_allocate(num_epacs * sizeof(*(epac_set.epacs)));
    memset(epac_set.epacs, 0, sizeof(*(epac_set.epacs)) * num_epacs);

    /*
     * prepend any upstream epacs to the granted privileges
     */
    CLEAR_STATUS(stp);
    for (cur_epac = 0;
    cur_epac < num_upstream_privs && STATUS_OK(stp);
    cur_epac++) {
	sec_id_epac_data_encode(NULL, NULL, &upstream_privs[cur_epac],
	    &epac_set.epacs[cur_epac].pickled_epac_data.num_bytes,
	    &epac_set.epacs[cur_epac].pickled_epac_data.bytes,
            stp);
	if (STATUS_OK(stp)) {
	    epac_set.num_epacs += 1;
	}
    }

    /*
     * now add the last epac
     */
    if (STATUS_OK(stp)) {
	sec_id_epac_data_encode(NULL, NULL, privs,
	    &epac_set.epacs[cur_epac].pickled_epac_data.num_bytes,
	    &epac_set.epacs[cur_epac].pickled_epac_data.bytes,
            stp);
	if (STATUS_OK(stp)) {
	    epac_set.num_epacs += 1;
	}
    }

    /*
     * generate the encoded epac set.
     */
    if (STATUS_OK(stp)) {
	sec_id_epac_set_encode(NULL, NULL, &epac_set,
			       &granted_privs->num_bytes,
			       &granted_privs->bytes, stp);
    }
    if (!STATUS_OK(stp)) {
	return;
    }

    /* 
     * generate a seal over the granted_privs, and store
     * in the v1_1 authz_data structure.
     */
    sec_authz_util_gen_seal(rpc_ss_allocate, rpc_ss_free, granted_privs, &seal);

    /*
     *
     * If delegation is enabled, generate a delegation
     * token as well.
     */
    switch (privs->deleg_type) {
	krb5_encrypt_block   ps_eblock;
        krb5_kvno            ps_kvno;
	
    default:
	SET_STATUS(stp, sec_priv_s_bad_deleg_type);
	break;
    case sec_id_deleg_type_none:
	break;   /* do nothing */
    case sec_id_deleg_type_traced:
    case sec_id_deleg_type_impersonation:
	/*
	 * we've already computed the appropriate seal so
         * we just need to convert it to a delegation token
         */
	memset(token_bytes, 0, sizeof(token_bytes));
	memset(scratch, 0, sizeof(scratch));

	/*
         * construct plaintext buffer containing a 4-byte
	 * big-endian expiration timestamp followed by
	 * 16 bytes of seal data, then encrypt it under the
         * current priv server key into bytes 20-33 of
	 * the delegation token.
	 */
	mp = (sec_authz_mp_t) &scratch[0];
	AUTHZ_UTIL_MARSH_BE_UNSIGNED32(mp, expiration);
	AUTHZ_UTIL_ADVANCE_MP(mp, 4);
	AUTHZ_UTIL_MARSH_BYTES(mp, seal.seal_data, seal.seal_len);
	
	rpriv_util_get_ps_eblock(ETYPE_DES_CBC_CRC,
				 0,  /* current key */
				 &ps_eblock,
				 &ps_kvno,
				 stp);
	if (STATUS_OK(stp)) {
	    *stp = krb5_encrypt((krb5_pointer) scratch,
				(krb5_pointer) V0_TOKEN_CIPHER(token_bytes),
				V0_TOKEN_REQ_CLEAR_LEN,
				&ps_eblock,
				0);
	    rpriv_util_finish_ps_eblock(&ps_eblock);
	}
	if (STATUS_OK(stp)) {
	    TOKEN_VNO(token_bytes) = 0;
	    V0_TOKEN_KVNO(token_bytes) = ps_kvno;

            /* 
	     * not using any flags - because we don't support
	     * confidential bytes in 1.1.
             */
	    V0_TOKEN_FLAGS1(&token_bytes[0]) 
		= V0_TOKEN_FLAGS2(&token_bytes[0]) = 0;

	    token.token_bytes.bytes = token_bytes;

	    /* We're using only the required portion of the token */
	    token.token_bytes.num_bytes = V0_REQ_TOKENSIZE;
	    token.expiration = expiration;
	    
	    token_set.num_tokens = 1;
            token_set.tokens = &token;

	    v1_1_authz_data.deleg_tokens = &token_set;
	}
	break;
    }

    if (!STATUS_OK(stp)) {
	return;
    }


    /*
     * If compatibilty is enabled, then generate a version 0
     * NDR pickle containing an encoded sec_id_pac_t.  Before
     * we can do that we have to convert the pa data in the
     * appropriate epac (initiator or last caller) to a sec_id_pac_t
     */
    if (privs->compat_mode != sec_id_compat_mode_none) {
	sec_id_pa_t  tmp_pa;
	
	switch (privs->compat_mode)  {
	case sec_id_compat_mode_initiator:
	    if (num_upstream_privs != 0) {
		/* 
		 * Can only request initiator compat mode if our
		 * upstream caller allows it, otherwise, 
		 * we would be allowing a form of illegal impersonation
		 * in the compatibility case.
		 */
		if (upstream_privs[num_upstream_privs-1].compat_mode 
		        != sec_id_compat_mode_initiator) {
		    SET_STATUS(stp, sec_priv_s_cmode_not_enabled);
		} else {
		    tmp_pa = upstream_privs[0].pa;
		}
	    } else 
		tmp_pa = privs->pa;
	    break;
	case sec_id_compat_mode_caller:
	    /*
	     * Never any harm in allowing compat mode caller, since the
	     * requestor can always accomplish the equivalent
	     * by contacting the pre1.1 application service directly 
	     * without issuing a delegation request.
	     */
	    tmp_pa = privs->pa;
	    break;
	default:
	    SET_STATUS(stp, sec_priv_s_bad_compat_mode);
	    break;
	}
	
	if (STATUS_OK(stp)) {
	    pa_data_to_pac(&tmp_pa, &v1_pac);
	}
    }

    if (STATUS_OK(stp)) {
	priv_krb_util_gen_v0_authdata(
            rpc_ss_allocate, 
            rpc_ss_free,
	    &v1_1_authz_data,
	    (privs->compat_mode == sec_id_compat_mode_none ? NULL : &v1_pac),
	    authdata,
            stp);
    }
}


/*
 * Construct a Privilege Ticket-Granting Ticket
 * containing authorization data provided by the
 * caller.  This routine works for both PTGTs
 * and EPTGTs.
 *
 */
extern  void rpriv_util_build_ptgt(
    rpriv_krb_req_info_t    *req_info,        /* [in] */
    unsigned32              expiration,       /* [in] */
    krb5_authdata           **authdata,       /* [in] */
    krb5_ticket             **priv_tkt,       /* [out] */
    error_status_t          *stp              /* [out] */
)
{
    krb5_keyblock  tgt_key;
    krb5_ticket    *ptgt = NULL;

    *priv_tkt = NULL;

    ptgt = (krb5_ticket *) malloc(sizeof(krb5_ticket));
    if (ptgt == NULL) {
        SET_STATUS(stp, sec_priv_s_no_mem);
	return;
    }
    memset(ptgt, 0, sizeof(*ptgt));

    ptgt->enc_part2 = (krb5_enc_tkt_part *) malloc(sizeof(krb5_enc_tkt_part));
    if (ptgt->enc_part2 == NULL) {
        SET_STATUS(stp, sec_priv_s_no_mem);
        free((char *) ptgt);
        return;
    }
    memset(ptgt->enc_part2, 0, sizeof(krb5_enc_tkt_part));

    /*
     * Get the principal name and current key of the local
     * Ticket-Granting_Ticket service.  We plop the principal
     * name and key version directly into the PTGT that we are
     * constructing.
     */
    rpriv_util_get_tgt_info(req_info, &ptgt->server, &tgt_key,
			    &ptgt->enc_part.kvno, stp);

    if (!STATUS_OK(stp)) {
	krb5_free_ticket(ptgt);
        return;
    }

    /* cleartext portion */

    ptgt->enc_part.etype = req_info->etype;

    /* key version number already recorded above */

    /* encrypted portion */

    /*
     * endtime was computed for us by the caller
     */
    ptgt->enc_part2->times.endtime = expiration;
    ptgt->enc_part2->times.authtime =
        req_info->authent->ticket->enc_part2->times.authtime;
    ptgt->enc_part2->times.starttime = 0 ;

    /*
     * Priv tickets aren't renewable, so the renew_till time doesn't
     * mean anything.
     */
    ptgt->enc_part2->times.renew_till = 0;

    /*XXX
     * This method of random key generation  only works because the priv server
     * and the kdc are running in the same process
     */
    *stp = (*(krb5_csarray[req_info->etype]->system->random_key))
          (krb5_csarray[req_info->etype]->random_sequence,
           &ptgt->enc_part2->session);

    if (STATUS_OK(stp)) {
	/* use addresses from authentication header ticket */
	*stp = krb5_copy_addresses(req_info->authent->ticket->enc_part2->caddrs,
				   &ptgt->enc_part2->caddrs);
    }

    if (STATUS_OK(stp)) {

	/*XXX Policy Flags
	 *
	 * For now, we don't have renewable, proxiable, or forwardable
	 * priv tickets, so we just zero out the flags field, and avoid
	 * all the processing associated with policy checks
	 */
	ptgt->enc_part2->flags = 0;
	
	/* priv ticket client is the priv server in the local realm, which is
	 * precisely the server field in the authentication header ticket
	 */
	*stp = krb5_copy_principal(req_info->authent->ticket->server,
                                   &ptgt->enc_part2->client);

        if (GOOD_STATUS(stp)) {

	    /* DON'T CARRY TRANSITED INFO FORWARD */
	    ptgt->enc_part2->transited.tr_type = KRB5_DOMAIN_X500_COMPRESS;
	    ptgt->enc_part2->transited.tr_contents.length = 0;
	    ptgt->enc_part2->transited.tr_contents.data = NULL;
	
	    /* DCE authorization data */
	    if (authdata) {
	        *stp = krb5_copy_authdata(authdata,
				          &ptgt->enc_part2->authorization_data);
	        if (*stp == ENOMEM) {
		    SET_STATUS(stp, sec_priv_s_no_mem);
	        }
	    }
        }
    }
    if (STATUS_OK(stp)) {
	/* generate the encrypted piece of the priv ticket */
	*stp = krb5_encrypt_tkt_part(&tgt_key, ptgt);
    }

    /* done with tgt_key, in any case */
    memset(tgt_key.contents, 0, tgt_key.length);
    free(tgt_key.contents);

    if (STATUS_OK(stp)) {
	*priv_tkt = ptgt;
    } else {
	krb5_free_ticket(ptgt);
    }
}


extern  void  rpriv_util_build_ptgt_rep(
    rpriv_krb_req_info_t    *req_info,       /* [in] */
    krb5_pa_data            **padata_vec,    /* [in] */
    krb5_ticket             *ptgt,           /* [in] */
    rpriv_pickle_t          **ptgt_rep,      /* [out] */
    error_status_t          *stp             /* [out] */
)
{
    krb5_kdc_rep          *tgs_rep;
    krb5_enc_kdc_rep_part *tgs_rep_enc_part2;
    krb5_timestamp        kdc_time;
    krb5_address          *noaddrarray[1];  /* no addresses in kdc_rep */
    krb5_last_req_entry   *nolrarray[2],  nolrentry;
    krb5_data             *tgs_rep_data = NULL;

    tgs_rep = (krb5_kdc_rep *) malloc(sizeof(krb5_kdc_rep));
    if (tgs_rep == NULL) {
        SET_STATUS(stp, sec_priv_s_no_mem);
        return;
    }
    memset(tgs_rep, 0, sizeof(krb5_kdc_rep));

    tgs_rep->enc_part2 =
    (krb5_enc_kdc_rep_part *) malloc(sizeof(krb5_enc_kdc_rep_part));
    if (tgs_rep->enc_part2 == NULL) {
        SET_STATUS(stp, sec_priv_s_no_mem);
        free( (char *) tgs_rep);
        return;
    }
    memset(tgs_rep->enc_part2, 0, sizeof(krb5_enc_kdc_rep_part));
    tgs_rep_enc_part2 = tgs_rep->enc_part2;

    /* make the client aware of the new session key */
    *stp = krb5_copy_keyblock(ptgt->enc_part2->session,
			     &tgs_rep_enc_part2->session);

    if (STATUS_OK(stp)) {
	tgs_rep_enc_part2->nonce = req_info->nonce;
	
	/* no key expiration date, addresses, or last request info in reply */
	
	tgs_rep_enc_part2->key_exp = 0;
	
	nolrentry.lr_type = KRB5_LRQ_NONE;
	nolrentry.value = 0;
	
	nolrarray[0] = &nolrentry;
	nolrarray[1] = 0;
	
	tgs_rep_enc_part2->last_req = nolrarray;
	
	noaddrarray[0] = 0;
	tgs_rep_enc_part2->caddrs = noaddrarray;
	
	/*
	 * same flags and ticket times as sealed in the priv ticket,
	 * so the client can will be able to construct an informative
	 * credential cache entry
	 */
	tgs_rep_enc_part2->flags = ptgt->enc_part2->flags;
	
	tgs_rep_enc_part2->times = ptgt->enc_part2->times;
	
	/* now make the authtime the kdctime */
	krb5_timeofday(&kdc_time);
	tgs_rep_enc_part2->times.authtime = kdc_time;

	/* what server are we? */
	*stp = krb5_copy_principal(ptgt->server, &tgs_rep_enc_part2->server);
    }

    /* now, the cleartext part */

    if (STATUS_OK(stp)) {
	/*
	 * the msg_type is needed only to please the kerberos
	 * pickling software
	 */
	tgs_rep->msg_type = KRB5_TGS_REP;
	
	/* give the client back the priv server principal name */
	*stp = krb5_copy_principal(req_info->authent->ticket->server,
				  &tgs_rep->client);
    }

    if (STATUS_OK(stp)) {
	/* encrypted part uses etype and key from the authentication header */
	tgs_rep->enc_part.kvno = 0;         /* encrypted under a session key */
	tgs_rep->enc_part.etype = req_info->etype;

	/* plug in the caller's ticket and pa_data */
        tgs_rep->padata = padata_vec;
        tgs_rep->ticket = ptgt;
	*stp = krb5_encode_kdc_rep(KRB5_TGS_REP, tgs_rep_enc_part2,
	           req_info->authent->authenticator->subkey
		       ? req_info->authent->authenticator->subkey
		       : req_info->authent->ticket->enc_part2->session,
		   tgs_rep, &tgs_rep_data);
    }

    if (STATUS_OK(stp)) {
	unsigned32 pickle_size;
	/*
	 * put the pickle in storage that will disappear when the manager
	 * routine returns.
	 */
	pickle_size = (sizeof(rpriv_pickle_t) + tgs_rep_data->length);
	
	*ptgt_rep  = (rpriv_pickle_t *) rpc_ss_allocate(pickle_size);
	if (*ptgt_rep != NULL) {

	    (*ptgt_rep)->num_bytes = tgs_rep_data->length;
	    memcpy((*ptgt_rep)->bytes, tgs_rep_data->data,
		   tgs_rep_data->length);
	}
	krb5_free_data(tgs_rep_data);
    }
	

    /* some cleanup, so the free routines won't choke */
    tgs_rep->enc_part2->last_req = NULL;
    tgs_rep->enc_part2->caddrs = NULL;

    /*
     * NULL out aliases of caller-supplied data, so we don't free
     * out from under the caller.
     */

    tgs_rep->padata = NULL;
    tgs_rep->ticket = NULL;

    krb5_free_kdc_rep(tgs_rep);
}


#define MAX_GROUPS 10

/*
 *	r p r i v _ u t i l _ g e t _ f o r e i g n _ g r o u p s e t
 *
 *	For the global principal identified by realm_uuid and princ_uuid,
 *	if it belongs to any group in this cell, return it group(s) information
 *	in foreign_groupset.
 *	Return true if there is any returned, else false.
 */ 
extern  boolean32  rpriv_util_get_foreign_groupset (
    uuid_t			*realm_uuid,          /* [in] */
    uuid_t			*princ_uuid,          /* [in] */
    sec_id_foreign_groupset_t   *foreign_groupset,    /* [out] */
    error_status_t          *stp                      /* [out] */
)
{    
    signed32		num_groups=0;
    int			i;
    sec_rgy_cursor_t    group_cursor;
    sec_rgy_member_t    group_list[MAX_GROUPS];
    signed32            groups_returned=0;
    signed32		num = 0;
    sec_rgy_name_t	group_name;
    sec_rgy_properties_t        properties;


    BEGIN_READ(lock_db, stp) {

	group_cursor.valid = false;
	group_cursor.handle = 0;

        rsdb_pgo_get_foreign_groupsets(false, 
			realm_uuid, princ_uuid,
                        &group_cursor, MAX_GROUPS, group_list,
                        &groups_returned, &num_groups, stp);

	foreign_groupset->num_groups = 0;
        if ( num_groups > 0 ) {
	    foreign_groupset->groups = (sec_id_t *) 
			rpc_ss_allocate(sizeof(sec_id_t)*num_groups);

	    while ( num < num_groups && GOOD_STATUS(stp) ) {
	    	for ( i = 0; i < groups_returned; i++ ) {
		    /* foreign_groupset->groups[num].name is not set,
		     * data not relied on.  */
		    foreign_groupset->groups[num].name = NULL;
		    strcpy((char *)group_name, (char *)group_list[i]);
		    rsdb_pgo_name_to_uid(sec_rgy_domain_group, group_name, 
				     &foreign_groupset->groups[num].uuid,
				     stp);
		    num++;
	    	}
		if ( num < num_groups ) {
		    /* there are more groups left for retrieving */
       		     rsdb_pgo_get_foreign_groupsets(false, 
			realm_uuid, princ_uuid,
			&group_cursor, MAX_GROUPS, group_list,
                        &groups_returned, &num_groups, stp);
		}
	    }
        }

    } END_READ;

    if ( STATUS_OK(stp) ) {
	foreign_groupset->num_groups = num_groups;

	rsdb_properties_get_info(&properties, stp);
        if(BAD_STATUS(stp)) {
            return false;
    	}
	/* no need to fill in realm.name, it is not relied on */
	foreign_groupset->realm.name = NULL;
	foreign_groupset->realm.uuid = properties.realm_uuid;
    }
    if ( num_groups > 0 ) {
	return true;
    } else {
	return false;
    }
}

