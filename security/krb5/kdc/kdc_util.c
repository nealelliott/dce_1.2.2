/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: kdc_util.c,v $
 * Revision 1.1.18.3  1996/10/03  14:59:58  arvind
 * 	merge up some stuff from beta6
 * 	[1996/09/16  20:48 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_1/1]
 *
 * 	Use KRB5KDC_ERR_MUST_USE_USER2USER instead of KRB5KDC_ERR_POLICY
 * 	[1996/04/30  15:04 UTC  burati  /main/DCE_1.2/mb_u2u/1]
 *
 * 	merge u2u work
 * 	[1996/04/29  21:34 UTC  burati  /main/HPDCE02/mb_mothra8/1]
 *
 * 	u2u work off Mothra, before 1.2 branch is available
 * 	[1996/01/03  19:13 UTC  psn  /main/DCE_1.2/1]
 *
 * Revision 1.1.18.2  1996/07/08  18:36:21  arvind
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 * 	[1996/06/11  17:57 UTC  aha  /main/DCE_1.2.2/2]
 * 
 * 	remove unused header.
 * 	[1996/05/08  19:43 UTC  sommerfeld  /main/HPDCE02/sommerfeld_CHFts17874/7]
 * 
 * 	fix typo in previous change.
 * 	[1996/05/02  18:52 UTC  sommerfeld  /main/HPDCE02/sommerfeld_CHFts17874/6]
 * 
 * 	change princ_compare to princ_equiv in 2tgt mode.
 * 	[1996/05/02  18:50 UTC  sommerfeld  /main/HPDCE02/sommerfeld_CHFts17874/5]
 * 
 * 	expunge code covered by #if 0's
 * 	[1996/05/02  18:44 UTC  sommerfeld  /main/HPDCE02/sommerfeld_CHFts17874/4]
 * 
 * 	more alias fixups.
 * 	[1996/05/02  18:42 UTC  sommerfeld  /main/HPDCE02/sommerfeld_CHFts17874/3]
 * 
 * 	remove bandaid, since it (a) caused crashes, and (b) didn't do what we
 * 	want.
 * 	[1996/04/30  23:24 UTC  sommerfeld  /main/HPDCE02/sommerfeld_CHFts17874/2]
 * 
 * 	Fix up inter-realm checks.
 * 	Move out DCE-specific code to kdb_rsdb.c where it belongs.
 * 	cleanup of cell-alias issues.
 * 	[1996/04/26  22:55 UTC  sommerfeld  /main/HPDCE02/sommerfeld_CHFts17874/1]
 * 
 * 	Use KRB5KDC_ERR_MUST_USE_USER2USER instead of KRB5KDC_ERR_POLICY
 * 	[1996/04/30  15:04 UTC  burati  /main/DCE_1.2/mb_u2u/1]
 * 
 * 	merge u2u work
 * 	[1996/04/29  21:34 UTC  burati  /main/HPDCE02/mb_mothra8/1]
 * 
 * 	u2u work off Mothra, before 1.2 branch is available
 * 	[1996/01/03  19:13 UTC  psn  /main/DCE_1.2/1]
 * 
 * 	Merge CHFts15432
 * 	[1995/06/08  17:31 UTC  mullan_s  /main/HPDCE02/5]
 * 
 * 	[CHFts15432]: Too many arguments to against_flag_policy_tgs,
 * 	include policy.h to pick up prototype.
 * 	[1995/06/08  17:21 UTC  mullan_s  /main/HPDCE02/mullan_mothra_chfts15432/1]
 * 
 * 	merge error return fix to mainline
 * 	[1995/04/20  17:00 UTC  rps  /main/HPDCE02/3]
 * 
 * 	[1994/12/14  20:21 UTC  mullan_s  /main/HPDCE02/1]
 * 
 * 	SET KRB5_KDB_PADATA_UNIX_TIME if account has pre_auth_req ERA of 1.
 * 	[1994/12/14  20:13 UTC  mullan_s  /main/mullan_mothra_chfts13565/1]
 * 
 * Revision 1.1.18.1  1996/06/04  22:00:48  arvind
 * 	Merge u2u: In validate_tgs_req(), if  KRB5_KDB_REQUIRES_USER_TO_USER flag set,
 * 	and request isn't u2u based, return  policy error
 * 	[1996/05/06  21:07 UTC  burati  /main/DCE_1.2/2]
 * 
 * 	merge u2u work
 * 	[1996/04/30  15:04 UTC  burati  /main/DCE_1.2/mb_u2u/1]
 * 
 * 	u2u work off Mothra, before 1.2 branch is available
 * 	[1996/04/29  21:34 UTC  burati  /main/HPDCE02/mb_mothra8/1]
 * 
 * Revision 1.1.16.3  1996/02/18  00:08:48  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:00:07  marty]
 * 
 * Revision 1.1.16.2  1995/12/08  17:45:05  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/5  1995/06/08  17:31 UTC  mullan_s
 * 	Merge CHFts15432
 * 
 * 	HP revision /main/HPDCE02/mullan_mothra_chfts15432/1  1995/06/08  17:21 UTC  mullan_s
 * 	[CHFts15432]: Too many arguments to against_flag_policy_tgs,
 * 	include policy.h to pick up prototype.
 * 
 * 	HP revision /main/HPDCE02/4  1995/05/15  21:03 UTC  greg
 * 	mergeout.
 * 	[1995/04/28  18:25 UTC  greg  /main/HPDCE02/SEC_migrate_merge/3]
 * 
 * 	copyright
 * 	[1995/04/05  17:18 UTC  rps  /main/HPDCE02/MOTHRA_panic/5]
 * 
 * 	fix typo in previous change.
 * 	[1995/04/03  23:49 UTC  sommerfeld  /main/HPDCE02/MOTHRA_panic/4]
 * 
 * 	Remove #if 0 code.
 * 	[1995/03/31  19:19 UTC  sommerfeld  /main/HPDCE02/MOTHRA_panic/3]
 * 
 * 	krb5b4p3 merge
 * 	[1995/03/08  21:41 UTC  rps  /main/HPDCE02/rps_hc/1]
 * 
 * 	[1995/03/08  21:40 UTC  rps  /main/HPDCE02/rps_hc/1]
 * 
 * 	make is_tgs public & rename, for use by priv server.
 * 	[1995/03/02  19:53 UTC  sommerfeld  /main/HPDCE02/sommerfeld_krb5_alias_1/2]
 * 
 * 	rip out misguided realmname canonicalization.
 * 	add is_tgs predicate.
 * 	silence a compiler warning.
 * 	[1995/02/11  00:06 UTC  sommerfeld  /main/HPDCE02/sommerfeld_krb5_alias_1/1]
 * 
 * 	Need to translate principal name to obj info whether
 * 	or not password expiration is supported.
 * 	[1995/03/23  14:30 UTC  greg  /main/SEC_migrate/3]
 * 
 * 	return if status of  principal lookup
 * 	is bad when attempting to retrieve
 * 	a passwd_exp attribute instance.
 * 	[1995/03/22  21:54 UTC  greg  /main/SEC_migrate/2]
 * 
 * 	Don't enforce password expiration unless the password expiration
 * 	feature is supported by the current version of the security software.
 * 	[1995/03/22  21:38 UTC  greg  /main/SEC_migrate/1]
 * 
 * 	HP revision /main/HPDCE02/3  1995/04/20  17:00 UTC  rps
 * 	merge error return fix to mainline
 * 
 * 	HP revision /main/HPDCE02/MOTHRA_panic/5  1995/04/05  17:18 UTC  rps
 * 	[1995/04/05  17:18 UTC  rps  /main/HPDCE02/MOTHRA_panic/5]
 * 
 * 	fix typo in previous change.
 * 	[1995/04/03  23:49 UTC  sommerfeld  /main/HPDCE02/MOTHRA_panic/4]
 * 
 * 	Remove #if 0 code.
 * 	[1995/03/31  19:19 UTC  sommerfeld  /main/HPDCE02/MOTHRA_panic/3]
 * 
 * 	krb5b4p3 merge
 * 	[1995/03/08  21:41 UTC  rps  /main/HPDCE02/rps_hc/1]
 * 
 * 	[1995/03/08  21:40 UTC  rps  /main/HPDCE02/rps_hc/1]
 * 
 * 	make is_tgs public & rename, for use by priv server.
 * 	[1995/03/02  19:53 UTC  sommerfeld  /main/HPDCE02/sommerfeld_krb5_alias_1/2]
 * 
 * 	rip out misguided realmname canonicalization.
 * 	add is_tgs predicate.
 * 	silence a compiler warning.
 * 	[1995/02/11  00:06 UTC  sommerfeld  /main/HPDCE02/sommerfeld_krb5_alias_1/1]
 * 
 * 	HP revision /main/HPDCE02/1  1994/12/14  20:21 UTC  mullan_s
 * 
 * 	HP revision /main/mullan_mothra_chfts13565/1  1994/12/14  20:13 UTC  mullan_s
 * 	SET KRB5_KDB_PADATA_UNIX_TIME if account has pre_auth_req ERA of 1.
 * 	[1995/12/08  16:57:36  root]
 * 
 * Revision 1.1.8.6  1994/10/07  18:30:45  rps
 * 	[OT12150] Cell alias support
 * 	[1994/10/07  15:21:03  rps]
 * 
 * Revision 1.1.8.5  1994/09/13  18:51:57  hondo
 * 	fix for memory leaks
 * 	[1994/09/13  18:51:43  hondo]
 * 
 * Revision 1.1.8.4  1994/09/12  21:16:50  hondo
 * 	fix for memory leak ....CR10924
 * 	[1994/09/12  21:13:05  hondo]
 * 
 * Revision 1.1.8.3  1994/08/29  15:51:40  hondo
 * 	OT 11919 --login activity idl change to support different address types
 * 	[1994/08/28  21:50:27  hondo]
 * 
 * Revision 1.1.8.2  1994/07/15  15:00:32  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:17:57  mdf]
 * 
 * Revision 1.1.5.2  1993/07/08  12:30:11  root
 * 	Initial King Kong branch
 * 	[1993/07/08  12:29:39  root]
 * 
 * Revision 1.1.3.4  1993/05/18  14:44:33  tmm
 * 	Back in remote authentication changes
 * 	[1993/05/18  14:41:45  tmm]
 * 
 * Revision 1.1.3.3  1993/05/14  23:21:05  tmm
 * 	Back out remote authentication changes
 * 	[1993/05/14  22:16:14  tmm]
 * 
 * 	Embedding copyright notice
 * 	[1992/12/28  20:54:47  zeliff]
 * 
 * 	[CR6293] Checksum field is optional; don't step through it until
 * 	you check it for NULL pointer.
 * 	We treat missing checksum fields like bogus checksum fields.
 * 	[1992/12/08  21:08:09  sommerfeld]
 * 
 * Revision 1.1.1.12  1993/05/04  20:32:19  mgm
 * 	    Loading drop DCE1_0_2FNL
 * 
 * Revision 1.1.2.3  1992/05/28  20:58:49  sommerfeld
 * 	[CR3650] Add krb5_add32_nowrap to implement non-wraparound addition.
 * 	This one would be significantly shorter in assembler, but it isn't
 * 	worth it.
 * 	[1992/05/27  22:28:19  sommerfeld]
 * 
 * Revision 1.1.2.2  1992/04/29  15:57:48  pato
 * 	 pato    Fix OT report 2408 - Insecure ticket granting
 * 	[1992/04/14  18:31:48  pato]
 * 
 * Revision 1.1  1992/01/19  14:54:28  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  kdc_util.c
**
** Copyright (c) Hewlett-Packard Company 1991, 1995, 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/kdc/kdc_util.c,v $
 *
 * Copyright 1990,1991 by the Massachusetts Institute of Technology.
 * All Rights Reserved.
 *
 * Export of this software from the United States of America may
 *   require a specific license from the United States Government.
 *   It is the responsibility of any person or organization contemplating
 *   export to obtain such a license before exporting.
 *
 * WITHIN THAT CONSTRAINT, permission to use, copy, modify, and
 * distribute this software and its documentation for any purpose and
 * without fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright notice and
 * this permission notice appear in supporting documentation, and that
 * the name of M.I.T. not be used in advertising or publicity pertaining
 * to distribution of the software without specific, written prior
 * permission.  M.I.T. makes no representations about the suitability of
 * this software for any purpose.  It is provided "as is" without express
 * or implied warranty.
 *
 *
 * Utility functions for the KDC implementation.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_kdc_util_c[] =
"$Id: kdc_util.c,v 1.1.18.3 1996/10/03 14:59:58 arvind Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/kdb.h>
#include <krb5/asn1.h>

#include "kdc_util.h"
#include "extern.h"
#include "policy.h"

#include <krb5/ext-proto.h>
#include <stdio.h>

#include <syslog.h>

#ifdef OSF_DCE
#include <dce/lbase.h>
#include <dce/rplbase.h>
#include <rs_ver.h>
#include <rs_ver_pvt.h>
#include <rsdb_pvt.h>
#include <rsdb_jrnl.h>
#include <rsdb_login.h>
#include <rsdb_attr.h>
#include <sec_attr_tools.h>




krb5_boolean
dce_security_service_principal(princ)
krb5_principal princ;
{
    static krb5_principal   rgy_princ = NULL;
    static krb5_principal   priv_princ = NULL;

    /*            
     * rgy_princ and priv_princ can be static pointers in a multi-threaded
     * environment if we assume an atomic write to the pointer - the
     * worst that will happen is that we will waste a little memory
     * when multiple threads all determine the default value hasn't 
    * been set yet and go off and ask for the value.
     */
    if (krb5_principal_equiv(princ, tgs_server)) {
        return TRUE;
    }
    
    if (rgy_princ == NULL) {
        (void) krb5_parse_name((char *)SEC_RGY_SERVER_NAME, &rgy_princ);
    }
    if (rgy_princ != NULL) {
	if (krb5_principal_equiv(princ, rgy_princ)) {
            return TRUE;
        }
    }
    
    if (priv_princ == NULL) {
        (void) krb5_parse_name((char *)SEC_PRIV_SERVER_NAME, &priv_princ);
    }
    if (priv_princ != NULL) {
	if (krb5_principal_equiv(princ, priv_princ)) {
            return TRUE;
        }
    }
    
    return FALSE;
}
#endif

/*
 * concatenate first two authdata arrays, returning an allocated replacement.
 * The replacement should be freed with krb5_free_authdata().
 */
krb5_error_code
concat_authorization_data(first, second, output)
krb5_authdata **first;
krb5_authdata **second;
krb5_authdata ***output;
{
    register int i, j;
    register krb5_authdata **ptr, **retdata;

    /* count up the entries */
    i = 0;
    if (first)
	for (ptr = first; *ptr; ptr++)
	    i++;
    if (second)
	for (ptr = second; *ptr; ptr++)
	    i++;
    
    retdata = (krb5_authdata **)malloc((i+1)*sizeof(*retdata));
    if (!retdata)
	return ENOMEM;
    retdata[i] = 0;			/* null-terminated array */
    for (i = 0, j = 0, ptr = first; j < 2 ; ptr = second, j++)
	while (ptr && *ptr) {
	    /* now walk & copy */
	    retdata[i] = (krb5_authdata *)malloc(sizeof(*retdata[i]));
	    if (!retdata[i]) {
		krb5_free_authdata(retdata);
		return ENOMEM;
	    }
	    *retdata[i] = **ptr;
	    if (!(retdata[i]->contents =
		  (krb5_octet *)malloc(retdata[i]->length))) {
		krb5_xfree(retdata[i]);
		retdata[i] = 0;
		krb5_free_authdata(retdata);
		return ENOMEM;
	    }
	    memcpy((char *) retdata[i]->contents,
		   (char *)(*ptr)->contents,
		   retdata[i]->length);

	    ptr++;
	    i++;
	}
    *output = retdata;
    return 0;
}



krb5_boolean
realm_compare(princ1, princ2)
    krb5_principal princ1;
    krb5_principal princ2;
{
  krb5_data *realm1 = krb5_princ_realm(princ1);
  krb5_data *realm2 = krb5_princ_realm(princ2);

  return((realm1->length == realm2->length) &&
         !memcmp(realm1->data, realm2->data, realm1->length));
}

/* Is this principal (ignoring the realm)
   one of the TGS principals we support?
 */

krb5_boolean kdc_principal_is_tgs (krb5_const_principal p)
{
    int ncomp;
    krb5_const_principal p1;
    
    for (p1=p,ncomp=0; *p1; p1++, ncomp++)
	;
    if (ncomp != 3)
	return FALSE;

    if (p[1]->length != 6)
	return FALSE;

    if (memcmp(p[1]->data, "krbtgt", 6) != 0)
	return FALSE;

    return krb5_realm_equiv(p[2], tgs_server[0]);
}

struct kparg {
    krb5_keyblock *key;
    krb5_kvno kvno;
};

/*
 * Since we do the checking of the server name before passing into
 * krb5_rd_req_decoded, there's no reason to do it here, so we ignore the
 * "principal" argument.
 */

static krb5_error_code
kdc_rdreq_keyproc(DECLARG(krb5_pointer, keyprocarg),
		  DECLARG(krb5_principal, principal),
		  DECLARG(krb5_keytype, keytype),
		  DECLARG(krb5_kvno, vno),
		  DECLARG(krb5_keyblock **, key))
OLDDECLARG(krb5_pointer, keyprocarg)
OLDDECLARG(krb5_principal, principal)
OLDDECLARG(krb5_kvno, vno)
OLDDECLARG(krb5_keytype, keytype)
OLDDECLARG(krb5_keyblock **, key)
{
    register struct kparg *whoisit = (struct kparg *)keyprocarg;

    if (vno != whoisit->kvno)
	return KRB5KRB_AP_ERR_BADKEYVER;

    return(krb5_copy_keyblock(whoisit->key, key));
}

/*
 * Returns TRUE if the kerberos principal is the name of a Kerberos ticket
 * service.
 */
krb5_boolean krb5_is_tgs_principal(principal)
	krb5_principal	principal;
{
	if ((krb5_princ_component(principal, 0)->length ==
	     KRB5_TGS_NAME_SIZE) &&
	    (!memcmp(krb5_princ_component(principal, 0)->data,
		     KRB5_TGS_NAME, KRB5_TGS_NAME_SIZE)))
		return TRUE;
	return FALSE;
}

/*
 * given authentication data (provides seed for checksum), calculate checksum
 * for source data and compare to authdata checksum.  Storage for checksum
 * is provided.
 */
static krb5_error_code
comp_cksum(krb5_cksumtype type, krb5_data *source,
	   krb5_checksum *checksum, krb5_keyblock *session,
	   krb5_checksum *dest)
{
    krb5_error_code retval;

    /* first compute checksum */
    if (retval = krb5_calculate_checksum(type, 
					 source->data, 
					 source->length,
					 session->contents, /* seed */
					 session->length,   /* seed length */
					 dest)) {
	return retval;
    }
    if (dest->length != checksum->length ||
	memcmp((char *)dest->contents,
	       (char *)checksum->contents,
	       dest->length)) {
	return KRB5KRB_AP_ERR_BAD_INTEGRITY;
    }
    return 0;	
}

krb5_error_code verify_kdc_body_cksum (krb5_data *pkt,
				       krb5_kdc_req *request,
				       krb5_checksum *checksum,
				       krb5_keyblock *session)
{
    krb5_error_code retval;
    krb5_checksum our_cksum;
    krb5_data *scratch = NULL;
    krb5_data scratch1;
    our_cksum.contents = 0;

    our_cksum.checksum_type = checksum->checksum_type;
    if (!valid_cksumtype(our_cksum.checksum_type)) {
	retval = KRB5KDC_ERR_SUMTYPE_NOSUPP;
	goto cleanup;
    }	
    /* must be collision proof */
    if (!is_coll_proof_cksum(our_cksum.checksum_type)) {
	retval = KRB5KRB_AP_ERR_INAPP_CKSUM;
	goto cleanup;
    }

    /* check application checksum vs. tgs request */
    if (!(our_cksum.contents = (krb5_octet *)
	  malloc(krb5_checksum_size(our_cksum.checksum_type)))) {
	retval = ENOMEM;
	goto cleanup;
    }

    /*
     * Check application checksum vs. tgs request
     * 	 
     * We try checksumming the req-body two different ways: first we
     * try reaching into the raw asn.1 stream (if available), and
     * checksum that directly; if that fails, then we try encoding
     * using our local asn.1 library.
     */
    retval = KRB5KRB_AP_ERR_BAD_INTEGRITY;
    if (pkt && (fetch_asn1_field(pkt->data, 1, 4, &scratch1) >= 0)) {
	retval = comp_cksum(our_cksum.checksum_type, &scratch1,
			    checksum,
			    session,
			    &our_cksum);
    }
    if (retval) {
	if (retval = encode_krb5_kdc_req_body(request, &scratch)) 
	    goto cleanup;	 /* XXX retval should be in kdc range */
	retval = comp_cksum(our_cksum.checksum_type, scratch,
			    checksum, session,
			    &our_cksum);
    }
  cleanup:
    krb5_xfree(our_cksum.contents);
    if (scratch)
	krb5_free_data(scratch);
    return retval;
}

krb5_error_code 
kdc_process_tgs_req(request, from, pkt, ret_authdat)
krb5_kdc_req *request;
const krb5_fulladdr *from;
krb5_data *pkt;
krb5_tkt_authent **ret_authdat;
{
    krb5_ap_req *apreq = 0;
    krb5_tkt_authent *authdat, *nauthdat;
    struct kparg who;
    krb5_error_code retval = 0;
    krb5_data scratch2;
    krb5_pa_data **tmppa;
    krb5_boolean foreign_server = FALSE;
    krb5_enc_tkt_part *ticket_enc;

    if (!request->padata)
	return KRB5KDC_ERR_PADATA_TYPE_NOSUPP;
    for (tmppa = request->padata; *tmppa; tmppa++) {
	if ((*tmppa)->pa_type == KRB5_PADATA_AP_REQ)
	    break;
    }
    if (!*tmppa)			/* cannot find any AP_REQ */
	return KRB5KDC_ERR_PADATA_TYPE_NOSUPP;

    scratch2.length = (*tmppa)->length;
    scratch2.data = (char *)(*tmppa)->contents;

    if (retval = decode_krb5_ap_req(&scratch2, &apreq))
	return retval;

    if (!(authdat = (krb5_tkt_authent *)malloc(sizeof(*authdat)))) {
	retval = ENOMEM;
	goto cleanup;
    }
    memset((char *)authdat, 0, sizeof(*authdat));
    authdat->ticket = apreq->ticket;
    *ret_authdat = authdat;

    if (isflagset(apreq->ap_options, AP_OPTS_USE_SESSION_KEY) ||
	isflagset(apreq->ap_options, AP_OPTS_MUTUAL_REQUIRED)) {
	syslog(LOG_INFO, "TGS_REQ: SESSION KEY or MUTUAL");
	retval = KRB5KDC_ERR_POLICY;
	apreq->ticket = 0;		/* Caller will free the ticket */
	goto cleanup;
    }

    /*
     * Verify that the ticket is either for a TGS or that the 
     * the target server name matches the server name in the ticket
     * supplied.
     */
    if (! kdc_principal_is_tgs(apreq->ticket->server)
	&& ! krb5_principal_compare(apreq->ticket->server,
                                            request->server)) {
	retval = KRB5KRB_AP_ERR_NOT_US;
	apreq->ticket = 0;		/* Caller will free the ticket */
	goto cleanup;
    }

    if (retval = kdc_get_server_key(authdat->ticket,
				    &who.key,
				    &who.kvno)) {
	apreq->ticket = 0;		/* Caller will free the ticket */
	goto cleanup;
    }

    /* If the "server" principal in the ticket is not something
       in the local realm, then we must refuse to service the request
       if the client claims to be from the local realm.

       If we don't do this, then some other realm's nasty KDC can
       claim to be authenticating a client from our realm, and we'll
       give out tickets concurring with it!

       we set a flag here for checking below.
       */
    if (!krb5_realm_equiv(krb5_princ_realm(apreq->ticket->server),
		     krb5_princ_realm(tgs_server)) != 0)
	foreign_server = TRUE;

    retval = krb5_rd_req_decoded(apreq, apreq->ticket->server,
				 from->address,
				 0,	/* no fetchfrom */
				 kdc_rdreq_keyproc,
				 (krb5_pointer)&who,
				 kdc_rcache,
				 &nauthdat);
    krb5_free_keyblock(who.key);

    if (retval) {
	apreq->ticket = 0;		/* Caller will free the ticket */
	goto cleanup;
    }

    /*
     * no longer need to protect the ticket in apreq, since
     * authdat is about to get nuked --- it's going to get reassigned.
     */
    krb5_xfree(authdat);

    authdat = nauthdat;
    *ret_authdat = authdat;
    ticket_enc = authdat->ticket->enc_part2;

    /* now rearrange output from rd_req_decoded */

    /* make sure the client is of proper lineage (see above) */
    if (foreign_server) {
	if (krb5_realm_equiv(krb5_princ_realm(tgs_server),
			     krb5_princ_realm(ticket_enc->client)))
	{
	    /* someone in a foreign realm claiming to be local */
	    syslog(LOG_INFO, "PROCESS_TGS: failed lineage check");
	    retval = KRB5KDC_ERR_POLICY;
	    goto cleanup;
	}
    }
    if (!authdat->authenticator->checksum) {
	    retval = KRB5KRB_AP_ERR_INAPP_CKSUM; 
	    goto cleanup;
    }

    retval = verify_kdc_body_cksum(pkt, request,
				   authdat->authenticator->checksum,
				   authdat->ticket->enc_part2->session);

cleanup:
    if (apreq)
	krb5_free_ap_req(apreq);
    return retval;
}

krb5_error_code
kdc_get_server_key(ticket, key, kvno)
krb5_ticket *ticket;
krb5_keyblock **key;
krb5_kvno *kvno;
{
    krb5_error_code retval;
    int nprincs;
    krb5_db_entry server;
    krb5_boolean more;
    krb5_principal server_princ = ticket->server;
    
#ifndef OSF_DCE
    if (krb5_principal_equiv(tgs_server, ticket->server)) {
	*kvno = tgs_kvno;
	return krb5_copy_keyblock(&tgs_key, key);
    } else
#endif
    {
	nprincs = 1;

	if (retval = krb5_db_get_principal(ticket->server,
			   krb5_csarray[ticket->enc_part.etype]->system->proto_keytype,
			   ticket->enc_part.kvno,
			   &server, &nprincs, &more)) {
	    return(retval);
	}
	if (more) {
	    krb5_db_free_principal(&server, nprincs);
	    return(KRB5KDC_ERR_PRINCIPAL_NOT_UNIQUE);
	} else if (nprincs != 1) {
	    char *sname;

	    krb5_db_free_principal(&server, nprincs);
	    if (!krb5_unparse_name(ticket->server, &sname)) {
		syslog(LOG_ERR, "TGS_REQ: UNKNOWN SERVER: server='%s'",
		       sname);
		free(sname);
	    }
	    return(KRB5KDC_ERR_S_PRINCIPAL_UNKNOWN);
	}
	/* convert server.key into a real key (it may be encrypted
	   in the database) */
	if (*key = (krb5_keyblock *)malloc(sizeof **key)) {
	    retval = KDB_CONVERT_KEY_OUTOF_DB(&server.key, *key);
	} else
	    retval = ENOMEM;
	*kvno = server.kvno;
	krb5_db_free_principal(&server, nprincs);
	return retval;
    }
}

/* This probably wants to be updated if you support last_req stuff */

static krb5_last_req_entry nolrentry = { KRB5_LRQ_NONE, 0 };
static krb5_last_req_entry *nolrarray[] = { &nolrentry, 0 };

krb5_error_code
fetch_last_req_info(dbentry, lrentry)
krb5_db_entry *dbentry;
krb5_last_req_entry ***lrentry;
{
    *lrentry = nolrarray;
    return 0;
}


/* XXX!  This is a temporary place-holder */

krb5_error_code
check_hot_list(ticket)
krb5_ticket *ticket;
{
    return 0;
}


#define MAX_REALM_LN 500


/*
 * subrealm - determine if r2 is a subrealm of r1
 *
 *            SUBREALM takes two realms, r1 and r2, and
 *            determines if r2 is a subrealm of r1.
 *            r2 is a subrealm of r1 if (r1 is a prefix
 *            of r2 AND r1 and r2 begin with a /) or if
 *            (r1 is a suffix of r2 and neither r1 nor r2
 *            begin with a /).
 *
 * RETURNS:   If r2 is a subrealm, and r1 is a prefix, the number
 *            of characters in the suffix of r2 is returned as a
 *            negative number.
 *
 *            If r2 is a subrealm, and r1 is a suffix, the number
 *            of characters in the prefix of r2 is returned as a
 *            positive number.
 *
 *            If r2 is not a subrealm, SUBREALM returns 0.
 */
static  int
subrealm(r1,r2)
char	*r1;
char	*r2;
{
    int	l1,l2;
    l1 = strlen(r1);
    l2 = strlen(r2);
    if(l2 <= l1) return(0);
    if((*r1 == '/') && (*r2 == '/') && (strncmp(r1,r2,l1) == 0)) return(l1-l2);
    if((*r1 != '/') && (*r2 != '/') && (strncmp(r1,r2+l2-l1,l1) == 0))
	return(l2-l1);
    return(0);
}

/*
 * add_to_transited  Adds the name of the realm which issued the
 *                   ticket granting ticket on which the new ticket to
 *                   be issued is based (note that this is the same as
 *                   the realm of the server listed in the ticket
 *                   granting ticket. 
 *
 * ASSUMPTIONS:  This procedure assumes that the transited field from
 *               the existing ticket granting ticket already appears
 *               in compressed form.  It will add the new realm while
 *               maintaining that form.   As long as each successive
 *               realm is added using this (or a similar) routine, the
 *               transited field will be in compressed form.  The
 *               basis step is an empty transited field which is, by
 *               its nature, in its most compressed form.
 *
 * ARGUMENTS: krb5_data *tgt_trans  Transited field from TGT
 *            krb5_data *new_trans  The transited field for the new ticket
 *            krb5_principal tgs    Name of ticket granting server
 *                                  This includes the realm of the KDC
 *                                  that issued the ticket granting
 *                                  ticket.  This is the realm that is
 *                                  to be added to the transited field.
 *            krb5_principal client Name of the client
 *            krb5_principal server The name of the requested server.
 *                                  This may be the an intermediate
 *                                  ticket granting server.
 *
 *            The last two argument are needed since they are
 *            implicitly part of the transited field of the new ticket
 *            even though they are not explicitly listed.
 *
 * RETURNS:   krb5_error_code - Success, or out of memory
 *
 * MODIFIES:  new_trans:  ->length will contain the length of the new
 *                        transited field.
 * 
 *                        If ->data was not null when this procedure
 *                        is called, the memory referenced by ->data
 *                        will be deallocated. 
 *
 *                        Memory will be allocated for the new transited field
 *                        ->data will be updated to point to the newly
 *                        allocated memory.  
 *
 * BUGS:  The space allocated for the new transited field is the
 *        maximum that might be needed given the old transited field,
 *        and the realm to be added.  This length is calculated
 *        assuming that no compression of the new realm is possible.
 *        This has no adverse consequences other than the allocation
 *        of more space than required.  
 *
 *        This procedure will not yet use the null subfield notation,
 *        and it will get confused if it sees it.
 *
 *        This procedure does not check for quoted commas in realm
 *        names.
 */

krb5_error_code 
add_to_transited(DECLARG(krb5_data *, tgt_trans),
                 DECLARG(krb5_data *, new_trans),
                 DECLARG(krb5_principal, tgs),
                 DECLARG(krb5_principal, client),
                 DECLARG(krb5_principal, server))
OLDDECLARG(krb5_data *, tgt_trans)
OLDDECLARG(krb5_data *, new_trans)
OLDDECLARG(krb5_principal, tgs)
OLDDECLARG(krb5_principal, client)
OLDDECLARG(krb5_principal, server)
{
  char        *realm;
  char        *trans;
  char        *otrans;

  /* The following are for stepping through the transited field     */

  char        prev[MAX_REALM_LN];
  char        next[MAX_REALM_LN];
  char        current[MAX_REALM_LN];
  char        exp[MAX_REALM_LN];      /* Expanded current realm name     */

  int         clst, nlst;    /* count of last character in current and next */
  int         pl, pl1;       /* prefix length                               */
  int         added;         /* TRUE = new realm has been added             */

  if (!(realm = (char *) malloc(krb5_princ_realm(tgs)->length+1))) {
    return(ENOMEM);
  }
  memcpy(realm, krb5_princ_realm(tgs)->data, krb5_princ_realm(tgs)->length);
  realm[krb5_princ_realm(tgs)->length] = '\0';

  if (!(otrans = (char *) malloc(tgt_trans->length+1))) {
    return(ENOMEM);
  }
  memcpy(otrans, tgt_trans->data, tgt_trans->length);
  otrans[tgt_trans->length] = '\0';

  if (!(trans = (char *) malloc(strlen(realm) + strlen(otrans) + 1))) {
    return(ENOMEM);
  }
#ifdef OSF_DCE
  trans[0] = '\0';
#endif

  if (new_trans->data)  free(new_trans->data);
  new_trans->data = trans;

  /* For the purpose of appending, the realm preceding the first */
  /* realm in the transited field is considered the null realm   */

  prev[0] = '\0';

  /***** In next statement, need to keep reading if the , was quoted *****/
  /* read field into current */

  if (sscanf(otrans, "%[^,]", current) == 1) {
    otrans += strlen(current);
  }
  else {
    current[0] = '\0';
  }

  if (otrans[0] == ',')  otrans++;
             
  added = (krb5_princ_realm(client)->length == strlen(realm) &&
           !strncmp(krb5_princ_realm(client)->data, realm, strlen(realm))) ||
          (krb5_princ_realm(server)->length == strlen(realm) &&
           !strncmp(krb5_princ_realm(server)->data, realm, strlen(realm)));

  while (current[0]) {

    /* figure out expanded form of current name */

    clst = strlen(current) - 1;
    if (current[0] == ' ') {
      strcpy(exp, current+1);
    }
    else if ((current[0] == '/') && (prev[0] == '/')) {
      strcpy(exp, prev);
      strcat(exp, current);
    }
    else if (current[clst] == '.') {
      strcpy(exp, current);
      strcat(exp, prev);
    }
    else {
      strcpy(exp, current);
    }

    /***** next statement, need to keep reading if the , was quoted *****/
    /* read field into next */

    if (sscanf(otrans, "%[^,]", next) == 1) {
      otrans += strlen(next);
      nlst    = strlen(next) - 1;
    }
    else {
      next[0] = '\0';
      nlst    = 0;
    }

    if (otrans[0] == ',')  otrans++;

    if (!strcmp(exp, realm))  added = TRUE;

    /* If we still have to insert the new realm */

    if (!added) {

      /* Is the next field compressed?  If not, and if the new */
      /* realm is a subrealm of the current realm, compress    */
      /* the new realm, and insert immediately following the   */
      /* current one.  Note that we can not do this if the next*/
      /* field is already compressed since it would mess up    */
      /* what has already been done.  In most cases, this is   */
      /* not a problem because the realm to be added will be a */
      /* subrealm of the next field too, and we will catch     */
      /* it in a future iteration.                             */

      if ((next[nlst] != '.') && (next[0] != '/') &&
          (pl = subrealm(exp, realm))) {
        added = TRUE;
        strcat(current, ",");
        if (pl > 0) {
          strncat(current, realm, pl);
        }
        else {
          strncat(current, realm+strlen(realm)+pl, -pl);
        }
      }

      /* Whether or not the next field is compressed, if the    */
      /* realm to be added is a superrealm of the current realm,*/
      /* then the current realm can be compressed.  First the   */
      /* realm to be added must be compressed relative to the   */
      /* previous realm (if possible), and then the current     */
      /* realm compressed relative to the new realm.  Note that */
      /* if the realm to be added is also a superrealm of the   */
      /* previous realm, it would have been added earlier, and  */
      /* we would not reach this step this time around.         */

      else if (pl = subrealm(realm, exp)) {
        added      = TRUE;
        current[0] = '\0';
        if (pl1 = subrealm(prev,realm)) {
          if (pl1 > 0) {
            strncat(current, realm, pl1);
          }
          else {
            strncat(current, realm+strlen(realm)+pl1, -pl1);
          }
        }
        else { /* If not a subrealm */
          if ((realm[0] == '/') && prev[0]) {
            strcat(current, " ");
          }
          strcat(current, realm);
        }
        strcat(current,",");
        if (pl > 0) {
          strncat(current, exp, pl);
        }
        else {
          strncat(current, exp+strlen(exp)+pl, -pl);
        }
      }
    }

    if (new_trans->length != 0)  strcat(trans, ",");
    strcat(trans, current);
    new_trans->length = strlen(trans) + 1;

    strcpy(prev, exp);
    strcpy(current, next);
  }

  if (!added) {
    if (new_trans->length != 0)  strcat(trans, ",");
    if((realm[0] == '/') && trans[0])  strcat(trans, " ");
    strcat(trans, realm);
    new_trans->length = strlen(trans) + 1;
  }

  return(0);
}

/*
 * add two signed integers, clipping the result to INT32_MAX or
 * INT32_MIN rather than wrapping around on overflow.
 */

krb5_int32 krb5_add32_nowrap(a, b)
    krb5_int32 a;
    krb5_int32 b;
{
    krb5_int32 sum;
    krb5_int32 sign_bit = ~((~(unsigned)0) & ((~(unsigned)0) >> 1));
    sum = a + b;

    /*
     * The following assumes twos-complement binary arithmetic.
     * If you don't like it, fix your ALU.
     *
     * We overflow if sign(a) == sign(b) && sign(sum) != sign(a)
     */
    if (((a & sign_bit) ^ (b & sign_bit)) == 0)
    {
        if ((a & sign_bit) ^ (sum & sign_bit))
        {
            /* we overflowed */
            if (a & sign_bit)
            {
                sum = KRB5_INT32_MIN;
            } else
            {
                sum = KRB5_INT32_MAX;
            }
        }
    }
    return sum;
}

/*
 * Routines that validate a AS request; checks a lot of things.  :-)
 *
 * Returns a Kerberos protocol error number, which is _not_ the same
 * as a com_err error number!
 */
#define AS_OPTIONS_HANDLED (KDC_OPT_FORWARDABLE | KDC_OPT_PROXIABLE | \
                             KDC_OPT_ALLOW_POSTDATE | KDC_OPT_POSTDATED | \
                             KDC_OPT_RENEWABLE | KDC_OPT_RENEWABLE_OK)
int
validate_as_request(request, client, server, kdc_time, status)
register krb5_kdc_req *request;
krb5_db_entry *client;
krb5_db_entry server;
krb5_timestamp kdc_time;
char    **status;
{
    int         errcode;

    /*
     * If an illegal option is set, complain.
     */
    if (request->kdc_options & ~(AS_OPTIONS_HANDLED)) {
        *status = "INVALID AS OPTIONS";
        return KDC_ERR_BADOPTION;
    }

     /* An AS request must include the addresses field */
    if (request->addresses == 0) {
        *status = "NO ADDRESS";
        return KRB_AP_ERR_BADADDR;
    }


    /* The client's password must not be expired */
    if (client->pw_expiration && client->pw_expiration < kdc_time) {
	*status = "CLIENT KEY EXPIRED";
#ifdef KRBCONF_VAGUE_ERRORS
        return(KRB_ERR_GENERIC);
#else
        return(KDC_ERR_KEY_EXP);
#endif
    }

    /* The client must not be expired */
    if (client->expiration && client->expiration < kdc_time) {
        *status = "CLIENT EXPIRED";
#ifdef KRBCONF_VAGUE_ERRORS
        return(KRB_ERR_GENERIC);
#else
        return(KDC_ERR_NAME_EXP);
#endif
    }

    /* The server must not be expired */
    if (server.expiration && server.expiration < kdc_time) {
        *status = "SERVICE EXPIRED";
            return(KDC_ERR_SERVICE_EXP);
    }

    /*
     * If the client requires password changing, then only allow the
     * pwchange service.
     */
    if (isflagset(client->attributes, KRB5_KDB_REQUIRES_PWCHANGE) &&
        !isflagset(server.attributes, KRB5_KDB_PWCHANGE_SERVICE)) {
        *status = "REQUIRED PWCHANGE";
        return(KDC_ERR_KEY_EXP);
    }

    /* Client and server must allow postdating tickets */
    if ((isflagset(request->kdc_options, KDC_OPT_ALLOW_POSTDATE) ||
         isflagset(request->kdc_options, KDC_OPT_POSTDATED)) &&
        (isflagset(client->attributes, KRB5_KDB_DISALLOW_POSTDATED) ||
         isflagset(server.attributes, KRB5_KDB_DISALLOW_POSTDATED))) {
        *status = "POSTDATE NOT ALLOWED";
        return(KDC_ERR_CANNOT_POSTDATE);
    }

    /* Client and server must allow forwardable tickets */
    if (isflagset(request->kdc_options, KDC_OPT_FORWARDABLE) &&
        (isflagset(client->attributes, KRB5_KDB_DISALLOW_FORWARDABLE) ||
         isflagset(server.attributes, KRB5_KDB_DISALLOW_FORWARDABLE))) {
        *status = "FORWARDABLE NOT ALLOWED";
        return(KDC_ERR_POLICY);
    }

    /* Client and server must allow renewable tickets */
    if (isflagset(request->kdc_options, KDC_OPT_RENEWABLE) &&
        (isflagset(client->attributes, KRB5_KDB_DISALLOW_RENEWABLE) ||
         isflagset(server.attributes, KRB5_KDB_DISALLOW_RENEWABLE))) {
        *status = "RENEWABLE NOT ALLOWED";
        return(KDC_ERR_POLICY);
    }

    /* Client and server must allow proxiable tickets */
    if (isflagset(request->kdc_options, KDC_OPT_PROXIABLE) &&
        (isflagset(client->attributes, KRB5_KDB_DISALLOW_PROXIABLE) ||
         isflagset(server.attributes, KRB5_KDB_DISALLOW_PROXIABLE))) {
        *status = "PROXIABLE NOT ALLOWED";
        return(KDC_ERR_POLICY);
    }

    /* Check to see if client is locked out */
    if (isflagset(client->attributes, KRB5_KDB_DISALLOW_ALL_TIX)) {
        *status = "CLIENT LOCKED OUT";
        return(KDC_ERR_C_PRINCIPAL_UNKNOWN);
    }

    /* Check to see if server is locked out */
    if (isflagset(server.attributes, KRB5_KDB_DISALLOW_ALL_TIX)) {
        *status = "SERVICE LOCKED OUT";
        return(KDC_ERR_S_PRINCIPAL_UNKNOWN);
    }

    /* Check to see if server is allowed to be a service */
    if (isflagset(server.attributes, KRB5_KDB_DISALLOW_SVR)) {
        *status = "SERVICE NOT ALLOWED";
        return(KDC_ERR_S_PRINCIPAL_UNKNOWN);
    }

    /* Check to see if preauthentication is required */
    if (isflagset(client->attributes, KRB5_KDB_REQUIRES_PRE_AUTH) &&
        !request->padata) {
        *status = "MISSING PRE_AUTH";
#ifdef KRBCONF_VAGUE_ERRORS
        return KRB_ERR_GENERIC;
#else
        return KDC_ERR_PREAUTH_FAILED;
#endif
    }

#ifndef KRB5_BETA1
    /*
     * Check against local policy
     */
    errcode = against_local_policy_as(request, server, client,
                                      kdc_time, status);
    if (errcode)
        return errcode;
#endif

    return 0;
}

#define ASN1_ID_CLASS	(0xc0)
#define ASN1_ID_TYPE    (0x20)
#define ASN1_ID_TAG	(0x1f)	
#define ASN1_CLASS_UNIV	(0)
#define ASN1_CLASS_APP	(1)
#define ASN1_CLASS_CTX	(2)
#define ASN1_CLASS_PRIV	(3)
#define asn1_id_constructed(x) 	(x & ASN1_ID_TYPE)
#define asn1_id_primitive(x) 	(!asn1_id_constructed(x))
#define asn1_id_class(x)	((x & ASN1_ID_CLASS) >> 6)
#define asn1_id_tag(x)		(x & ASN1_ID_TAG)

/*
 * asn1length - return encoded length of value.
 *
 * passed a pointer into the asn.1 stream, which is updated
 * to point right after the length bits.
 *
 * returns -1 on failure.
 */
static int
asn1length(astream)
unsigned char **astream;
{
    int length;		/* resulting length */
    int sublen;		/* sublengths */
    int blen;		/* bytes of length */ 
    unsigned char *p;	/* substring searching */	

    if (**astream & 0x80) {
        blen = **astream & 0x7f;
	if (blen > 3) {
	   return(-1);
	}
	for (++*astream, length = 0; blen; ++*astream, blen--) {
	    length = (length << 8) | **astream;
	}
	if (length == 0) {
		/* indefinite length, figure out by hand */
	    p = *astream;
	    p++;
	    for (;;) {
		/* compute value length. */
		if ((sublen = asn1length(&p)) < 0) {
		    return(-1);
		}
		p += sublen;
                /* check for termination */
		if ((!*p++) && (!*p)) {
		    p++;
		    break;
		}
	    }
	    length = p - *astream;	 
	}
    } else {
	length = **astream;
	++*astream;
    } 
   return(length);
}

/*
 * fetch_asn1_field - return raw asn.1 stream of subfield.
 *
 * this routine is passed a context-dependent tag number and "level" and returns
 * the size and length of the corresponding level subfield.
 *
 * levels and are numbered starting from 1.  
 *
 * returns 0 on success, -1 otherwise.
 */
int
fetch_asn1_field(astream, level, field, data)
unsigned char *astream;
unsigned int level;
unsigned int field;
krb5_data *data;
{
    unsigned char *estream;	/* end of stream */
    int classes;		/* # classes seen so far this level */
    int levels = 0;		/* levels seen so far */
    int lastlevel = 1000;       /* last level seen */
    int length;			/* various lengths */
    int tag;			/* tag number */
    unsigned char savelen;      /* saved length of our field */

    /* we assume that the first identifier/length will tell us 
       how long the entire stream is. */
    astream++;
    estream = astream;
    if ((length = asn1length(&astream)) < 0) {
	return(-1);
    }
    estream += length;
    /* search down the stream, checking identifiers.  we process identifiers
       until we hit the "level" we want, and then process that level for our
       subfield, always making sure we don't go off the end of the stream.  */
    while (astream < estream) {
	if (!asn1_id_constructed(*astream)) {
	    return(-1);
	}
        if (asn1_id_class(*astream) == ASN1_CLASS_CTX) {
            if ((tag = (int)asn1_id_tag(*astream)) <= lastlevel) {
                levels++;
                classes = -1;
            }
            lastlevel = tag; 
            if (levels == level) {
	        /* in our context-dependent class, is this the one we're looking for ? */
	        if (tag == field) {
		    /* return length and data */ 
		    astream++;
		    savelen = *astream;
		    if ((data->length = asn1length(&astream)) < 0) {
		        return(-1);
	 	    }
		    /* if the field length is indefinite, we will have to subtract two
                       (terminating octets) from the length returned since we don't want
                       to pass any info from the "wrapper" back.  asn1length will always return
                       the *total* length of the field, not just what's contained in it */ 
		    if ((savelen & 0xff) == 0x80) {
		      data->length -=2 ;
		    }
		    data->data = (char *)astream;
		    return(0);
	        } else if (tag <= classes) {
		    /* we've seen this class before, something must be wrong */
		    return(-1);
	        } else {
		    classes = tag;
	        }
	    }
        }
        /* if we're not on our level yet, process this value.  otherwise skip over it */
	astream++;
	if ((length = asn1length(&astream)) < 0) {
	    return(-1);
	}
	if (levels == level) {
	    astream += length;
	}
    }
    return(-1);
}

/*
 * Routines that validate a TGS request; checks a lot of things.  :-)
 *
 * Returns a Kerberos protocol error number, which is _not_ the same
 * as a com_err error number!
 */
#define TGS_OPTIONS_HANDLED (KDC_OPT_FORWARDABLE | KDC_OPT_FORWARDED | \
			     KDC_OPT_PROXIABLE | KDC_OPT_PROXY | \
			     KDC_OPT_ALLOW_POSTDATE | KDC_OPT_POSTDATED | \
			     KDC_OPT_RENEWABLE | KDC_OPT_RENEWABLE_OK | \
			     KDC_OPT_ENC_TKT_IN_SKEY | KDC_OPT_RENEW | \
			     KDC_OPT_VALIDATE)

#define NO_TGT_OPTION (KDC_OPT_FORWARDED | KDC_OPT_PROXY | KDC_OPT_RENEW | \
		       KDC_OPT_VALIDATE)

int
validate_tgs_request(request, server, ticket, kdc_time, status)
register krb5_kdc_req *request;
krb5_db_entry server;
krb5_ticket *ticket;
krb5_timestamp kdc_time;
char **status;
{
    int		errcode;
    int		st_idx = 0;

    /*
     * If an illegal option is set, complain.
     */
    if (request->kdc_options & ~(TGS_OPTIONS_HANDLED)) {
	*status = "INVALID TGS OPTIONS";
	return KDC_ERR_BADOPTION;
    }
    
    /* Check to see if server has expired */
    if (server.expiration && server.expiration < kdc_time) {
	*status = "SERVICE EXPIRED";
	return(KDC_ERR_SERVICE_EXP);
    }

    /*
     * Verify that the server principal in authdat->ticket is correct
     * (either the ticket granting service or the service that was
     * originally requested)
     */
    if (request->kdc_options & NO_TGT_OPTION) {
	if (!krb5_principal_compare(ticket->server, request->server)) {
	    *status = "SERVER DIDN'T MATCH TICKET FOR RENEW/FORWARD/ETC";
	    return(KDC_ERR_SERVER_NOMATCH);
	}
    } else {
	/*
	 * OK, we need to validate the krbtgt service in the ticket.
	 *
	 * The krbtgt service is of the form:
	 * 		krbtgt/realm-A@realm-B
	 *
	 * Realm A is the "server realm"; the realm of the
	 * server of the requested ticket must match this realm.
	 * Of course, it should be a realm serviced by this KDC.
	 *
	 * Realm B is the "client realm"; this is what should be
	 * added to the transited field.  (which is done elsewhere)
	 */

	/* Make sure there are two components... */
	if (krb5_princ_size(ticket->server) != 2) {
	    *status = "BAD TGS SERVER LENGTH";
	    return KRB_AP_ERR_NOT_US;
	}
	/* ...that the first component is krbtgt... */
	if (!krb5_is_tgs_principal(ticket->server)) {
	    *status = "BAD TGS SERVER NAME";
	    return KRB_AP_ERR_NOT_US;
	}
	/* ...and that the second component matches the server realm... */
	if ((krb5_princ_component(ticket->server, 1)->length !=
	     krb5_princ_realm(request->server)->length) ||
	    memcmp(krb5_princ_component(ticket->server, 1)->data,
		   krb5_princ_realm(request->server)->data,
		   krb5_princ_realm(request->server)->length)) {
	    *status = "BAD TGS SERVER INSTANCE";
	    return KRB_AP_ERR_NOT_US;
	}
	/* XXX add check that second component must match locally
	 * supported realm?
	 */

	/* Server must allow TGS based issuances */
	if (isflagset(server.attributes, KRB5_KDB_DISALLOW_TGT_BASED)) {
	    *status = "TGT BASED NOT ALLOWED";
	    return(KDC_ERR_POLICY);
	}
    }
	    
    /* TGS must be forwardable to get forwarded or forwardable ticket */
    if ((isflagset(request->kdc_options, KDC_OPT_FORWARDED) ||
	 isflagset(request->kdc_options, KDC_OPT_FORWARDABLE)) &&
	!isflagset(ticket->enc_part2->flags, TKT_FLG_FORWARDABLE)) {
	*status = "TGT NOT FORWARDABLE";

	return KDC_ERR_BADOPTION;
    }

    /* TGS must be proxiable to get proxiable ticket */    
    if ((isflagset(request->kdc_options, KDC_OPT_PROXY) ||
	 isflagset(request->kdc_options, KDC_OPT_PROXIABLE)) &&
	!isflagset(ticket->enc_part2->flags, TKT_FLG_PROXIABLE)) {
	*status = "TGT NOT PROXIABLE";
	return KDC_ERR_BADOPTION;
    }

    /* TGS must allow postdating to get postdated ticket */
    if ((isflagset(request->kdc_options, KDC_OPT_ALLOW_POSTDATE) ||
	  isflagset(request->kdc_options, KDC_OPT_POSTDATED)) &&
	!isflagset(ticket->enc_part2->flags, TKT_FLG_MAY_POSTDATE)) {
	*status = "TGT NOT POSTDATABLE";
	return KDC_ERR_BADOPTION;
    }

    /* can only validate invalid tix */
    if (isflagset(request->kdc_options, KDC_OPT_VALIDATE) &&
	!isflagset(ticket->enc_part2->flags, TKT_FLG_INVALID)) {
	*status = "VALIDATE VALID TICKET";
	return KDC_ERR_BADOPTION;
    }

    /* can only renew renewable tix */
    if ((isflagset(request->kdc_options, KDC_OPT_RENEW) ||
	  isflagset(request->kdc_options, KDC_OPT_RENEWABLE)) &&
	!isflagset(ticket->enc_part2->flags, TKT_FLG_RENEWABLE)) {
	*status = "TICKET NOT RENEWABLE";
	return KDC_ERR_BADOPTION;
    }

    /* can not proxy ticket granting tickets */
    if (isflagset(request->kdc_options, KDC_OPT_PROXY) &&
#ifdef OSF_DCE
	(!krb5_princ_name(request->server) ||
	 krb5_is_tgs_principal(request->server))) {
#else
	(!request->server->data ||
	 request->server->data[0].length != KRB5_TGS_NAME_SIZE ||
	 memcmp(request->server->data[0].data, KRB5_TGS_NAME,
		KRB5_TGS_NAME_SIZE))) {
#endif /* OSF_DCE */
	*status = "CAN'T PROXY TGT";
	return KDC_ERR_BADOPTION;
    }
    
    /* Server must allow forwardable tickets */
    if (isflagset(request->kdc_options, KDC_OPT_FORWARDABLE) &&
	isflagset(server.attributes, KRB5_KDB_DISALLOW_FORWARDABLE)) {
	*status = "NON-FORWARDABLE TICKET";
	return(KDC_ERR_POLICY);
    }
    
    /* Server must allow renewable tickets */
    if (isflagset(request->kdc_options, KDC_OPT_RENEWABLE) &&
	isflagset(server.attributes, KRB5_KDB_DISALLOW_RENEWABLE)) {
	*status = "NON-RENEWABLE TICKET";
	return(KDC_ERR_POLICY);
    }
    
    /* Server must allow proxiable tickets */
    if (isflagset(request->kdc_options, KDC_OPT_PROXIABLE) &&
	isflagset(server.attributes, KRB5_KDB_DISALLOW_PROXIABLE)) {
	*status = "NON-PROXIABLE TICKET";
	return(KDC_ERR_POLICY);
    }
    
    /* Server must allow postdated tickets */
    if (isflagset(request->kdc_options, KDC_OPT_ALLOW_POSTDATE) &&
	isflagset(server.attributes, KRB5_KDB_DISALLOW_POSTDATED)) {
	*status = "NON-POSTDATABLE TICKET";
	return(KDC_ERR_CANNOT_POSTDATE);
    }
    
    /* Server must allow DUP SKEY requests */
    if (isflagset(request->kdc_options, KDC_OPT_ENC_TKT_IN_SKEY) &&
	isflagset(server.attributes, KRB5_KDB_DISALLOW_DUP_SKEY)) {
	*status = "DUP_SKEY DISALLOWED";
	return(KDC_ERR_POLICY);
    }

    /* Server must use user to user? */
    if (!isflagset(request->kdc_options, KDC_OPT_ENC_TKT_IN_SKEY) &&
	isflagset(server.attributes, KRB5_KDB_REQUIRES_USER_TO_USER)) {
	*status = "USER-TO-USER REQUIRED";
	return(KDC_ERR_MUST_USE_USER2USER);
    }

    /* Server must not be locked out */
    if (isflagset(server.attributes, KRB5_KDB_DISALLOW_ALL_TIX)) {
	*status = "SERVER LOCKED OUT";
	return(KDC_ERR_S_PRINCIPAL_UNKNOWN);
    }
	
    /* Server must be allowed to be a service */
    if (isflagset(server.attributes, KRB5_KDB_DISALLOW_SVR)) {
	*status = "SERVER NOT ALLOWED";
	return(KDC_ERR_S_PRINCIPAL_UNKNOWN);
    }

    /* Check the hot list */
    if (check_hot_list(ticket)) {
	*status = "HOT_LIST";
	return(KRB_AP_ERR_REPEAT);
    }
    
    /* Check the start time vs. the KDC time */
    if (isflagset(request->kdc_options, KDC_OPT_VALIDATE)) {
	if (ticket->enc_part2->times.starttime > kdc_time) {
	    *status = "NOT_YET_VALID";
	    return(KRB_AP_ERR_TKT_NYV);
	}
    }
    
    /*
     * Check the renew_till time.  The endtime was already
     * been checked in the initial authentication check.
     */
    if (isflagset(request->kdc_options, KDC_OPT_RENEW) &&
	(ticket->enc_part2->times.renew_till < kdc_time)) {
	*status = "TKT_EXPIRED";
	return(KRB_AP_ERR_TKT_EXPIRED);
    }
    
    /*
     * Checks for ENC_TKT_IN_SKEY:
     *
     * (1) Make sure the second ticket exists
     * (2) Make sure it is a ticket granting ticket
     */
    if (isflagset(request->kdc_options, KDC_OPT_ENC_TKT_IN_SKEY)) {
	if (!request->second_ticket ||
	    !request->second_ticket[st_idx]) {
	    *status = "NO_2ND_TKT";
	    return(KDC_ERR_BADOPTION);
	}
	if (!krb5_principal_equiv(request->second_ticket[st_idx]->server,
				  tgs_server)) {
		*status = "2ND_TKT_NOT_TGS";
		return(KDC_ERR_POLICY);
	}
	st_idx++;
    }

    /* Check for hardware preauthentication */
    if (isflagset(server.attributes, KRB5_KDB_REQUIRES_HW_AUTH) &&
	!isflagset(ticket->enc_part2->flags,TKT_FLG_HW_AUTH)) {
	*status = "NO HW PREAUTH";
	return KRB_ERR_GENERIC;
    }

    /* Check for any kind of preauthentication */
    if (isflagset(server.attributes, KRB5_KDB_REQUIRES_PRE_AUTH) &&
	!isflagset(ticket->enc_part2->flags, TKT_FLG_PRE_AUTH)) {
	*status = "NO PREAUTH";
	return KRB_ERR_GENERIC;
    }
    
    /*
     * Check local policy
     */
#ifdef OSF_DCE
    errcode = against_flag_policy_tgs(request, ticket);
#else
    errcode = against_local_policy_tgs(request, server, ticket, status);
#endif
    if (errcode)
	return errcode;
    
    
    return 0;
}

/*
 * This function returns salt information for a particular client_key
 */
krb5_error_code
get_salt_from_key(context, client, client_key, salt)
    krb5_context	       	context;
    krb5_principal		client;
#ifdef OSF_DCE_ARCHAISM
    krb5_db_entry *client_key;	/* too gross for words */
#else
    krb5_key_data *		client_key;
#endif
    krb5_data *			salt;

{
    krb5_error_code		retval;
    krb5_data *			realm;

    int type;
    
    salt->data = 0;
    salt->length = -1;
    
#ifndef OSF_DCE_ARCHAISM
    if (client_key->key_data_ver == 1)
	return 0;

    type = (client_key->key_data_type[1]);
#else
    type = client_key->salt_type;
#endif

    switch (type)
    {
    case KRB5_KDB_SALTTYPE_NORMAL:
	break;
    case KRB5_KDB_SALTTYPE_V4:
	/* send an empty (V4) salt */
	salt->data = 0;
	salt->length = 0;
	break;
    case KRB5_KDB_SALTTYPE_NOREALM:
	if ((retval = krb5_principal2salt_norealm(context, client, salt)))
	    return retval;
	break;
#ifndef OSF_DCE_ARCHAISM
    case KRB5_KDB_SALTTYPE_AFS3:
	/* send the same salt as with onlyrealm - but with no type info,
	   we just hope they figure it out on the other end. */
	/* fall through to onlyrealm: */
#endif
    case KRB5_KDB_SALTTYPE_ONLYREALM:
	realm = krb5_princ_realm(/*context,*/ client);
	salt->length = realm->length;
	if ((salt->data = malloc(realm->length)) == NULL)
	    return ENOMEM;
	memcpy(salt->data, realm->data, realm->length);
	break;
    case KRB5_KDB_SALTTYPE_SPECIAL:
#ifdef OSF_DCE_ARCHAISM
	salt->length = client_key->salt_length;
	if ((salt->data = malloc(salt->length)) == NULL)
	    return ENOMEM;
	memcpy(salt->data, client_key->salt, salt->length);
#else
	salt->length = client_key->key_data_length[1];
	if ((salt->data = malloc(salt->length)) == NULL)
	    return ENOMEM;
	memcpy(salt->data, client_key->key_data_contents[1], salt->length);
#endif
	break;
    }
    return 0;
}

#ifdef OSF_DCE_ARCHAISM
krb5_error_code
krb5_principal2salt_norealm(context, pr, ret)
    krb5_context context;
    register krb5_const_principal pr;
    krb5_data *ret;
{
    int size = 0, offset = 0;
    krb5_int32 nelem;
    register int i;

    if (pr == 0) {
	ret->length = 0;
	ret->data = 0;
	return 0;
    }

    nelem = krb5_princ_size(/*context, */pr);

    for (i = 0; i < (int) nelem; i++)
	size += krb5_princ_component(/*context,*/ pr, i)->length;

    ret->length = size;
    if (!(ret->data = malloc (size)))
	return ENOMEM;

    for (i = 0; i < (int) nelem; i++) {
	memcpy(&ret->data[offset], krb5_princ_component(/*context, */pr, i)->data,
	       krb5_princ_component(/*context, */pr, i)->length);
	offset += krb5_princ_component(/*context, */ pr, i)->length;
    }
    return 0;
}

#endif
