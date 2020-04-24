/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: decode.c,v $
 * Revision 1.1.6.1  1996/10/03  15:05:01  arvind
 * 	Fix functions used by mk_priv/rd_priv.
 * 	[1996/09/20  21:31 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/2]
 *
 * 	Deal with conversion failures, allocation failure during conversion.
 * 	type-safe asn.1 encode/decode.
 * 	[1996/09/16  21:28 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 *
 * Revision 1.1.4.2  1996/02/18  00:14:55  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:04:20  marty]
 * 
 * Revision 1.1.4.1  1995/12/08  18:02:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:00:20  root]
 * 
 * Revision 1.1.2.2  1992/12/29  15:59:41  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:16:24  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:49:58  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  decode.c V=4 12/12/91 //littl/prgy/krb5/lib/mvr
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/mvr/decode.c,v $
 * $Author: arvind $
 *
 * Copyright 1990 by the Massachusetts Institute of Technology.
 *
 * For copying and distribution information, please see the file
 * <krb5/copyright.h>.
 *
 * decoding glue routines.
 */

#include <krb5/copyright.h>
#include <krb5/krb5.h>
#include <krb5/ext-proto.h>

#include <krb5/asn1.h>
#include <dce/asn1.h>
#include <krb5/mvr/KRB5_mvr.h>
#include <krb5/mvr/KRB5.h>
#include <krb5/mvr/krb5-glue.h>
#include <krb5/mvr/krb5-manual-glue.h>

#include <stdio.h>

#ifdef ASN1_DEBUG
#   define debug_printf(ARG_LIST) printf ARG_LIST
#else
#   define debug_printf(ARG_LIST)
#   define MVR_LEAKCHECK(x)
#endif

#define generic_decode_define_2(fname, ktype, mtype)  \
krb5_error_code decode_##fname (const krb5_data *in, ktype **out)  \
{  \
    mtype m;  \
    asn1 start, end, ret;  \
    ktype *res;  \
    start = (asn1)in->data;  \
    end = start + in->length;  \
    ret = mtype ## _dec (start, end, &m);  \
    if (ret == 0) {  \
	*out = NULL;  \
        return KRB5KRB_AP_ERR_MODIFIED; /* XXX could be better error code */ \
    } else {  \
	krb5_error_code val; \
        res = (ktype *)malloc (sizeof(ktype)); /* !!! malloc-fail-check */  \
        if (!res) return ENOMEM; \
	val = mtype ## 2 ## ktype (&m, res); \
	mtype ## _free (&m);  \
	*out = res;  \
	return val;  \
    }  \
}

#define generic_decode_define_vec(fname, ktype, mtype)  \
krb5_error_code decode_##fname (const krb5_data *in, ktype **out)  \
{  \
    mtype m;  \
    asn1 start, end, ret;  \
    start = (asn1)in->data;  \
    end = start + in->length;  \
    ret = mtype ## _dec (start, end, &m);  \
    if (ret == 0) {  \
	*out = NULL;  \
        return KRB5KRB_AP_ERR_MODIFIED; /* XXX could be better error code */ \
    } else {  \
	krb5_error_code val = mtype ## 2 ## ktype (&m, out);  \
	mtype ## _free (&m);  \
	return val;  \
    }  \
}

#define generic_decode_define(ktype, mtype) generic_decode_define_2(ktype, ktype, mtype)


#if 0

krb5_error_code
krb5_decode_generic(input, output, decoder, tempsize, translator, ressize, free_translation)
const krb5_data *input;
register krb5_pointer *output;
decoder_func decoder;
int tempsize;
translator_func translator;
int ressize;
free_func free_translation;
{
    char *temp = malloc (tempsize);
    char *res = malloc (ressize);
    asn1 ret;
    

    debug_printf(("decoding %d bytes:\n", input->length));
#ifdef ASN1_DEBUG
    (void) asn1_dump (stdout, (asn1)(input->data),
        (asn1)(input->data+input->length));
#endif

    MVR_LOCK();
    ret = (*decoder)((asn1)input->data, ((asn1)input->data)+input->length, temp);

    if (ret == 0) {
        /* decode failed */
        free(temp);
        free(res);
        MVR_UNLOCK();
        *output = 0;
        return KRB5KRB_AP_ERR_MODIFIED; /* XXX could be better error code */
    } else {
        (*translator)(temp, res);
        (*free_translation)(temp);
        MVR_LEAKCHECK("dec free");
        MVR_UNLOCK();
        free(temp);
        *output = res;
        return 0;
    }
}

#endif

generic_decode_define(krb5_authenticator, Authenticator)
generic_decode_define(krb5_enc_data, EncryptedData)
generic_decode_define(krb5_ticket, Ticket)
generic_decode_define(krb5_enc_tkt_part, EncTicketPart)
generic_decode_define(krb5_enc_kdc_rep_part, EncTGSRepPart)
generic_decode_define_2(krb5_tgs_rep, krb5_kdc_rep, TGS_REP)
generic_decode_define(krb5_ap_req, AP_REQ)
generic_decode_define(krb5_ap_rep, AP_REP)
generic_decode_define(krb5_ap_rep_enc_part, EncAPRepPart)
generic_decode_define_2(krb5_tgs_req, krb5_kdc_req, TGS_REQ)
generic_decode_define_2(krb5_kdc_req_body, krb5_kdc_req, KDC_REQ_BODY)
generic_decode_define(krb5_safe, KRB_SAFE)
generic_decode_define(krb5_priv, KRB_PRIV)
generic_decode_define_2(krb5_enc_priv_part, krb5_priv_enc_part, EncKrbPrivPart)
generic_decode_define(krb5_error, KRB_ERROR)
generic_decode_define_vec(krb5_authdata, krb5_authdata_vec, AuthorizationData)
generic_decode_define(krb5_keyblock, EncryptionKey)
generic_decode_define(krb5_time_and_key, TimeAndKey)
generic_decode_define(krb5_pa_third_party, PAThirdParty)
generic_decode_define(krb5_sub_pa_data, SubPaData)
generic_decode_define(krb5_sub_err_data, SubErrData)
generic_decode_define(krb5_dce_pa_keys, DcePAKeys)
generic_decode_define(krb5_dce_pa_err_info, DcePAErrInfo)
generic_decode_define_vec(krb5_padata_sequence, krb5_pa_data_vec, PA_DATA)
generic_decode_define_vec(krb5_etype_info, krb5_etype_info_entry_vec, ETYPE_INFO)
generic_decode_define(krb5_sam_challenge, PA_SAM_CHALLENGE)    
generic_decode_define(krb5_predicted_sam_response, EncryptionKey) /* !!! */
generic_decode_define(krb5_pk_authenticator, PKAuthenticator)
generic_decode_define(krb5_signed_pk_authenticator, SignedPKAuthenticator)
generic_decode_define(krb5_pa_pk_as_root, PA_PK_AS_ROOT)
generic_decode_define_2(krb5_enc_pa_pk_as_rep_part, krb5_enc_pa_pk_as_rep_part, EncPaPkAsRepPart)
generic_decode_define_2(krb5_enc_pa_pk_tmp_key, krb5_keyblock, EncPaPkAsRepTmpKey)
generic_decode_define(krb5_enc_pa_pk_as_rep_part_shell, EncPaPkAsRepPartShell)
generic_decode_define(krb5_pa_pk_as_rep, PA_PK_AS_REP)

generic_decode_define(krb5_pa_enc_ts, PA_ENC_TS_ENC)
