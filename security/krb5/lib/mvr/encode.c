/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: encode.c,v $
 * Revision 1.1.6.3  1996/10/15  20:52:52  arvind
 * 	CHFts20156: fix memory leak, bogus return status.
 * 	[1996/10/04  22:18 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_2/1]
 *
 * 	Merge to DCE_1.2.2
 * 	[1996/06/10  18:18 UTC  mullan_s  /main/mullan_dce_krb5_der_work/1]
 *
 * 	Generate DER encodings instead of BER.
 * 	[1996/02/18  00:15:00  marty  1.1.4.2]
 *
 * Revision 1.1.6.2  1996/10/03  15:05:15  arvind
 * 	Fix functions used by mk_priv/rd_priv
 * 	[1996/09/20  21:31 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_1/2]
 * 
 * 	Deal with conversion failures, allocation failures.
 * 	fix a couple missing functions.
 * 	rewrite to make encoders type-safe.
 * 	Generate DER encodings instead of BER.
 * 	[1996/09/16  21:28 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_1/1]
 * 
 * 	Merge to DCE_1.2.2
 * 	[1996/06/10  18:18 UTC  mullan_s  /main/mullan_dce_krb5_der_work/1]
 * 
 * 	Generate DER encodings instead of BER.
 * 	[1996/02/18  00:15:00  marty  1.1.4.2]
 * 
 * Revision 1.1.6.1  1996/07/08  18:37:05  arvind
 * 	Merge to DCE_1.2.2
 * 	[1996/06/10  18:18 UTC  mullan_s  /main/mullan_dce_krb5_der_work/1]
 * 
 * 	Generate DER encodings instead of BER.
 * 	[1996/02/18  00:15:00  marty  1.1.4.2]
 * 
 * Revision 1.1.4.2  1996/02/18  00:15:00  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:04:25  marty]
 * 
 * Revision 1.1.4.1  1995/12/08  18:02:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:00:22  root]
 * 
 * Revision 1.1.2.2  1992/12/29  16:00:01  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:16:35  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:49:59  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  encode.c V=5 12/12/91 //littl/prgy/krb5/lib/mvr
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/mvr/encode.c,v $
 * $Author: arvind $
 *
 * Copyright 1990 by the Massachusetts Institute of Technology.
 *
 * For copying and distribution information, please see the file
 * <krb5/copyright.h>.
 *
 * encoding glue routines.
 */

#include <krb5/copyright.h>

#include <krb5/krb5.h>

#include <krb5/asn1.h>
#include <dce/asn1.h>
#include <krb5/mvr/KRB5_mvr.h>
#include <krb5/mvr/KRB5.h>
#include <krb5/mvr/krb5-glue.h>
#include <krb5/mvr/krb5-manual-glue.h>

#include <krb5/ext-proto.h>

#include <stdio.h>

#ifdef ASN1_DEBUG
#   define debug_printf(ARG_LIST) printf ARG_LIST
#else
#   define debug_printf(ARG_LIST)
#   define MVR_LOCK()
#   define MVR_UNLOCK()
#   define MVR_LEAKCHECK(x)
#endif

#define generic_encode_define_2(ftype, ktype, mtype)  \
krb5_error_code encode_##ftype (const ktype *in, krb5_data **out) \
{ \
    mtype m; \
    asn1 new_ptr, buf; \
    int size, real_size; \
    krb5_data *d; \
    memset(&m, 0, sizeof(m)); \
    if (ktype ## 2 ## mtype (in, &m)) { mtype ## _free (&m); return ENOMEM; } \
    size = mtype ## _len (0, &m); \
    buf = malloc (size); if (buf == NULL) { mtype##_free(&m); return ENOMEM; } \
    new_ptr = mtype ## _cod (buf, -1, ASN1_X509, &m); \
    mtype ## _free (&m); \
    if (new_ptr == 0) { free(buf); return ENOMEM; } \
    real_size = new_ptr - buf; \
    new_ptr = realloc(buf, real_size); if(new_ptr == NULL) {free(buf); return ENOMEM; } \
    buf=new_ptr; \
    d = *out = (krb5_data *)malloc (sizeof(krb5_data)); {if (d == NULL) { free(buf); return ENOMEM; }}\
    d->length = real_size; \
    d->data = (char *)buf; \
    return 0; \
}

#define generic_encode_define(ktype, mtype) generic_encode_define_2(ktype, ktype, mtype)

#if 0
krb5_error_code
krb5_encode_generic(input, data_out, len, tempsize, encoder, translator, free_translation)
krb5_const_pointer input;
register krb5_data **data_out;
int tempsize;
len_func len;
encoder_func encoder;
translator_func translator;
free_func free_translation;
{
    char *mvr_out = malloc(tempsize);
    asn1 new_ptr, buf;
    int size, real_size;

    asn1_x509_mode_on();
    MVR_LOCK();
    (*translator)(input, mvr_out);

    size = (*len) (0, mvr_out);

    debug_printf(("encoding %d bytes\n", size));
    buf = malloc (size);

    new_ptr = (*encoder)(buf, -1, ASN1_X509, mvr_out);
    (*free_translation)(mvr_out);
    MVR_LEAKCHECK("enc_free");
    free(mvr_out);
    MVR_LEAKCHECK("enc_done");    
    MVR_UNLOCK();
    if (new_ptr == 0) {
        free(buf);
        return 0x6969dead;
    }
    real_size = new_ptr - buf;
    debug_printf(("Really encoded %d bytes\n", real_size));
    /* shrink-wrap the buffer. */
    buf = realloc (buf, real_size);
    
    *data_out = (krb5_data *)malloc (sizeof (krb5_data));
    (*data_out)->length = real_size;
    (*data_out)->data = (char *)buf;
    return 0;
}
#endif

generic_encode_define(krb5_authenticator, Authenticator)
generic_encode_define(krb5_enc_data, EncryptedData)
generic_encode_define(krb5_ticket, Ticket)
generic_encode_define(krb5_enc_tkt_part, EncTicketPart)
generic_encode_define(krb5_enc_kdc_rep_part, EncTGSRepPart)
generic_encode_define_2(krb5_tgs_rep, krb5_kdc_rep, TGS_REP)
generic_encode_define(krb5_ap_req, AP_REQ)
generic_encode_define(krb5_ap_rep, AP_REP)
generic_encode_define(krb5_ap_rep_enc_part, EncAPRepPart)
generic_encode_define_2(krb5_tgs_req, krb5_kdc_req, TGS_REQ)
generic_encode_define_2(krb5_kdc_req_body, krb5_kdc_req, KDC_REQ_BODY)
generic_encode_define(krb5_safe, KRB_SAFE)
generic_encode_define(krb5_priv, KRB_PRIV)
generic_encode_define_2(krb5_enc_priv_part, krb5_priv_enc_part, EncKrbPrivPart)
generic_encode_define(krb5_error, KRB_ERROR)
generic_encode_define_2(krb5_authdata, krb5_authdata_vec, AuthorizationData)
generic_encode_define(krb5_keyblock, EncryptionKey)
generic_encode_define(krb5_time_and_key, TimeAndKey)
generic_encode_define(krb5_pa_third_party, PAThirdParty)
generic_encode_define(krb5_sub_pa_data, SubPaData)
generic_encode_define(krb5_sub_err_data, SubErrData)
generic_encode_define(krb5_dce_pa_keys, DcePAKeys)
generic_encode_define(krb5_dce_pa_err_info, DcePAErrInfo)
generic_encode_define_2(krb5_padata_sequence, krb5_pa_data_vec, PA_DATA)
generic_encode_define_2(krb5_etype_info, krb5_etype_info_entry_vec, ETYPE_INFO)
generic_encode_define(krb5_sam_challenge, PA_SAM_CHALLENGE)    
generic_encode_define(krb5_predicted_sam_response, EncryptionKey) /* !!! */
generic_encode_define(krb5_pk_authenticator, PKAuthenticator)
generic_encode_define(krb5_signed_pk_authenticator, SignedPKAuthenticator)
generic_encode_define(krb5_pa_pk_as_root, PA_PK_AS_ROOT)
generic_encode_define_2(krb5_enc_pa_pk_as_rep_part, krb5_enc_pa_pk_as_rep_part, EncPaPkAsRepPart)
generic_encode_define_2(krb5_enc_pa_pk_tmp_key, krb5_keyblock, EncPaPkAsRepTmpKey)
generic_encode_define(krb5_enc_pa_pk_as_rep_part_shell, EncPaPkAsRepPartShell)
generic_encode_define(krb5_pa_pk_as_rep, PA_PK_AS_REP)
generic_encode_define(krb5_pa_enc_ts, PA_ENC_TS_ENC)



