/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: encode.h,v $
 * Revision 1.1.16.2  1996/10/03  14:57:04  arvind
 * 	Fix functions used by mk_priv/rd_priv
 * 	[1996/09/20  21:31 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_1/2]
 *
 * 	fix a couple missing functions..
 * 	Simplify macrology, make encode/decode calls type-safe.
 * 	Add encodings for public key preauth.
 * 	[1996/09/16  21:59 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_1/1]
 *
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 * 	[1996/06/04  18:50 UTC  psn  /main/DCE122_PK/1]
 *
 * 	Merge ASN.1 code and encoding functions for public key login
 * 	[1996/04/17  13:22 UTC  aha  /main/aha_pk2/1]
 *
 * 	Additions for public key login
 * 	[1994/10/25  22:05:31  mullan_s  1.1.10.1]
 *
 * Revision 1.1.16.1  1996/07/08  18:35:14  arvind
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 * 	[1996/06/04  18:50 UTC  psn  /main/DCE122_PK/1]
 * 
 * 	Merge ASN.1 code and encoding functions for public key login
 * 	[1996/04/17  13:22 UTC  aha  /main/aha_pk2/1]
 * 
 * 	Additions for public key login
 * 	[1994/10/25  22:05:31  mullan_s  1.1.10.1]
 * 
 * Revision 1.1.14.3  1996/02/18  23:02:47  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:21:16  marty]
 * 
 * Revision 1.1.14.2  1995/12/08  17:43:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:57:15  root]
 * 
 * Revision 1.1.10.1  1994/10/25  22:05:31  mullan_s
 * 	[OT12495] Plaintext third party preauth fixes: new prototypes
 * 	for decoding/encoding error info struct and new key struct.
 * 	[1994/10/25  22:04:58  mullan_s]
 * 
 * Revision 1.1.4.2  1994/08/26  18:46:11  mullan_s
 * 	[OT11557] Add macros for encoding/decoding subtyped DCE preauth data.
 * 	[1994/08/25  18:24:00  mullan_s]
 * 
 * Revision 1.1.4.1  1994/05/11  19:17:27  ahop
 * 	hp_sec_to_osf_2 drop
 * 	Add encode_krb5_pa_third_party and decode_krb5_pa_third_party.
 * 	Add encode_krb5_time_and_key and decode_krb5_time_and_key.
 * 	[1994/04/29  21:14:50  ahop]
 * 
 * Revision 1.1.2.2  1992/12/29  14:00:45  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:53:18  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:48:15  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
**
** Copyright (c) Hewlett-Packard Company 1991, 1994, 1996
** Unpublished work. All Rights Reserved.
**
*/
/* ID info
 * 
 * Copyright Hewlett-Packard Company 1990, 1991
 *  Copyright (c) Hewlett-Packard Company 1991
 * Unpublished work. All Rights Reserved.
 *
 *
 * VersionID = "@(#)encode.h	8 - 06/25/91";
 */

/*
 *
 * Copyright 1990 by the Massachusetts Institute of Technology.
 *
 * For copying and distribution information, please see the file
 * <krb5/copyright.h>.
 */



#ifndef KRB5_ENCODE_DEFS__
#define KRB5_ENCODE_DEFS__

#ifdef OSF_DCE
#define OSF_DCE_FEATURE
#endif

krb5_error_code decode_krb5_as_rep (const krb5_data *, krb5_kdc_rep **);
krb5_error_code encode_krb5_as_rep (const krb5_kdc_rep *, krb5_data **);
krb5_error_code encode_krb5_as_req (const krb5_kdc_req *, krb5_data **);

/* the input here is non-const.  see decode_arq.c for the reason why */
krb5_error_code decode_krb5_as_req (krb5_data *, krb5_kdc_req **);


/*
 * note: the "mtype" parameter here is for documentation purposes only
 * see encode.c/decode.c for actual uses..
 */
/*
 * fname: name suffix of encode/decode function
 * ktype: kerberos datatype
 * mtype: MAVROS datatype
 */

#define generic_encode_declare(ktype, mtype) generic_encode_declare_2(ktype, ktype, mtype)
#define generic_decode_declare(ktype, mtype) generic_decode_declare_2(ktype, ktype, mtype)
#define generic_encode_declare_2(fname, ktype, mtype) extern krb5_error_code encode_##fname (const ktype *, krb5_data **)
#define generic_decode_declare_2(fname, ktype, mtype) extern krb5_error_code decode_##fname (const krb5_data *, ktype **)

	
generic_encode_declare(krb5_authenticator, Authenticator);
generic_decode_declare(krb5_authenticator, Authenticator);

generic_encode_declare(krb5_enc_data, EncryptedData);
generic_decode_declare(krb5_enc_data, EncryptedData);

generic_encode_declare(krb5_ticket, Ticket);
generic_decode_declare(krb5_ticket, Ticket);

generic_encode_declare(krb5_enc_tkt_part, EncTicketPart);
generic_decode_declare(krb5_enc_tkt_part, EncTicketPart);

generic_encode_declare(krb5_enc_kdc_rep_part, EncTGSRepPart);
generic_decode_declare(krb5_enc_kdc_rep_part, EncTGSRepPart);

generic_encode_declare_2(krb5_tgs_rep, krb5_kdc_rep, TGS_REP);
generic_decode_declare_2(krb5_tgs_rep, krb5_kdc_rep, TGS_REP);

generic_encode_declare(krb5_ap_req, AP_REQ);
generic_decode_declare(krb5_ap_req, AP_REQ);

generic_encode_declare(krb5_ap_rep, AP_REP);
generic_decode_declare(krb5_ap_rep, AP_REP);

generic_encode_declare(krb5_ap_rep_enc_part, EncAPRepPart);
generic_decode_declare(krb5_ap_rep_enc_part, EncAPRepPart);

generic_encode_declare_2(krb5_tgs_req, krb5_kdc_req, TGS_REQ);
generic_decode_declare_2(krb5_tgs_req, krb5_kdc_req, TGS_REQ);

generic_encode_declare_2(krb5_kdc_req_body, krb5_kdc_req, KDC_REQ_BODY);
generic_decode_declare_2(krb5_kdc_req_body, krb5_kdc_req, KDC_REQ_BODY);

generic_encode_declare(krb5_safe, KRB_SAFE);
generic_decode_declare(krb5_safe, KRB_SAFE);

generic_encode_declare(krb5_priv, KRB_PRIV);
generic_decode_declare(krb5_priv, KRB_PRIV);

generic_encode_declare_2(krb5_enc_priv_part, krb5_priv_enc_part, EncKrbPrivPart);
generic_decode_declare_2(krb5_enc_priv_part, krb5_priv_enc_part, EncKrbPrivPart);

generic_encode_declare(krb5_error, KRB_ERROR);
generic_decode_declare(krb5_error, KRB_ERROR);

generic_encode_declare_2(krb5_authdata, krb5_authdata_vec, AuthorizationData);
generic_decode_declare_2(krb5_authdata, krb5_authdata_vec, AuthorizationData);

generic_encode_declare(krb5_pa_enc_ts, PA_ENC_TS_ENC);
generic_decode_declare(krb5_pa_enc_ts, PA_ENC_TS_ENC);

#ifdef OSF_DCE_FEATURE

generic_encode_declare(krb5_keyblock, EncryptionKey);
generic_decode_declare(krb5_keyblock, EncryptionKey);

generic_encode_declare(krb5_time_and_key, TimeAndKey);
generic_decode_declare(krb5_time_and_key, TimeAndKey);

generic_encode_declare(krb5_pa_third_party, PAThirdParty);
generic_decode_declare(krb5_pa_third_party, PAThirdParty);

generic_encode_declare(krb5_sub_pa_data, SubPaData);
generic_decode_declare(krb5_sub_pa_data, SubPaData);

generic_encode_declare(krb5_sub_err_data, SubErrData);
generic_decode_declare(krb5_sub_err_data, SubErrData);

generic_encode_declare(krb5_dce_pa_keys, DcePAKeys);
generic_decode_declare(krb5_dce_pa_keys, DcePAKeys);

generic_encode_declare(krb5_dce_pa_err_info, DcePAErrInfo);
generic_decode_declare(krb5_dce_pa_err_info, DcePAErrInfo);

generic_encode_declare_2(krb5_padata_sequence, krb5_pa_data_vec, PA_DATA);
generic_decode_declare_2(krb5_padata_sequence, krb5_pa_data_vec, PA_DATA);

generic_encode_declare_2(krb5_etype_info, krb5_etype_info_entry_vec, ETYPE_INFO);
generic_decode_declare_2(krb5_etype_info, krb5_etype_info_entry_vec, ETYPE_INFO);

generic_encode_declare(krb5_sam_challenge, PA_SAM_CHALLENGE);
generic_decode_declare(krb5_sam_challenge, PA_SAM_CHALLENGE);    

generic_encode_declare(krb5_predicted_sam_response, EncryptionKey); /* !!! */
generic_decode_declare(krb5_predicted_sam_response, EncryptionKey); /* !!! */

/******************************************
 * Start of additions for Public Key Login
 ******************************************/

generic_encode_declare(krb5_pk_authenticator, PKAuthenticator);
generic_decode_declare(krb5_pk_authenticator, PKAuthenticator);

generic_encode_declare(krb5_signed_pk_authenticator, SignedPKAuthenticator);
generic_decode_declare(krb5_signed_pk_authenticator, SignedPKAuthenticator);

generic_encode_declare(krb5_pa_pk_as_root, PA_PK_AS_ROOT);
generic_decode_declare(krb5_pa_pk_as_root, PA_PK_AS_ROOT);

generic_encode_declare_2(krb5_enc_pa_pk_as_rep_part, krb5_enc_pa_pk_as_rep_part, EncPaPkAsRepPart);
generic_decode_declare_2(krb5_enc_pa_pk_as_rep_part, krb5_enc_pa_pk_as_rep_part, EncPaPkAsRepPart);

generic_encode_declare_2(krb5_enc_pa_pk_tmp_key, krb5_keyblock, EncPaPkAsRepTmpKey);
generic_decode_declare_2(krb5_enc_pa_pk_tmp_key, krb5_keyblock, EncPaPkAsRepTmpKey);

generic_encode_declare(krb5_enc_pa_pk_as_rep_part_shell, EncPaPkAsRepPartShell);
generic_decode_declare(krb5_enc_pa_pk_as_rep_part_shell, EncPaPkAsRepPartShell);

generic_encode_declare(krb5_pa_pk_as_rep, PA_PK_AS_REP);
generic_decode_declare(krb5_pa_pk_as_rep, PA_PK_AS_REP);

/******************************************
 * End of additions for Public Key Login
 ******************************************/

#endif /* OSF_DCE */

#undef generic_encode_declare
#undef generic_decode_declare
#undef generic_encode_declare_2
#undef generic_decode_declare_2

/* ASN.1 encoding knowledge; KEEP IN SYNC WITH ASN.1 defs! */
/* here we use some knowledge of ASN.1 encodings */
/* 
  Ticket is APPLICATION 1.
  Authenticator is APPLICATION 2.
  AS_REQ is APPLICATION 10.
  AS_REP is APPLICATION 11.
  TGS_REQ is APPLICATION 12.
  TGS_REP is APPLICATION 13.
  AP_REQ is APPLICATION 14.
  AP_REP is APPLICATION 15.
  KRB_SAFE is APPLICATION 20.
  KRB_PRIV is APPLICATION 21.
  EncASRepPart is APPLICATION 25.
  EncTGSRepPart is APPLICATION 26.
  EncAPRepPart is APPLICATION 27.
  EncKrbPrivPart is APPLICATION 28.
  KRB_ERROR is APPLICATION 30.
 */
/* allow either constructed or primitive encoding, so check for bit 6
   set or reset */
#define krb5_is_krb_ticket(dat)\
        ((dat) && (dat)->length && ((dat)->data[0] == 0x61 ||\
                                    (dat)->data[0] == 0x21))
#define krb5_is_krb_authenticator(dat)\
        ((dat) && (dat)->length && ((dat)->data[0] == 0x62 ||\
                                    (dat)->data[0] == 0x22))
#define krb5_is_as_req(dat)\
        ((dat) && (dat)->length && ((dat)->data[0] == 0x6a ||\
                                    (dat)->data[0] == 0x2a))
#define krb5_is_as_rep(dat)\
        ((dat) && (dat)->length && ((dat)->data[0] == 0x6b ||\
                                    (dat)->data[0] == 0x2b))
#define krb5_is_tgs_req(dat)\
        ((dat) && (dat)->length && ((dat)->data[0] == 0x6c ||\
                                    (dat)->data[0] == 0x2c))
#define krb5_is_tgs_rep(dat)\
        ((dat) && (dat)->length && ((dat)->data[0] == 0x6d ||\
                                    (dat)->data[0] == 0x2d))
#define krb5_is_ap_req(dat)\
        ((dat) && (dat)->length && ((dat)->data[0] == 0x6e ||\
                                    (dat)->data[0] == 0x2e))
#define krb5_is_ap_rep(dat)\
        ((dat) && (dat)->length && ((dat)->data[0] == 0x6f ||\
                                    (dat)->data[0] == 0x2f))
#define krb5_is_krb_safe(dat)\
        ((dat) && (dat)->length && ((dat)->data[0] == 0x74 ||\
                                    (dat)->data[0] == 0x34))
#define krb5_is_krb_priv(dat)\
        ((dat) && (dat)->length && ((dat)->data[0] == 0x75 ||\
                                    (dat)->data[0] == 0x35))
#define krb5_is_krb_enc_as_rep_part(dat)\
        ((dat) && (dat)->length && ((dat)->data[0] == 0x79 ||\
                                    (dat)->data[0] == 0x39))
#define krb5_is_krb_enc_tgs_rep_part(dat)\
        ((dat) && (dat)->length && ((dat)->data[0] == 0x7a ||\
                                    (dat)->data[0] == 0x3a))
#define krb5_is_krb_enc_ap_rep_part(dat)\
        ((dat) && (dat)->length && ((dat)->data[0] == 0x7b ||\
                                    (dat)->data[0] == 0x3b))
#define krb5_is_krb_enc_krb_priv_part(dat)\
        ((dat) && (dat)->length && ((dat)->data[0] == 0x7c ||\
                                    (dat)->data[0] == 0x3c))
#define krb5_is_krb_error(dat)\
        ((dat) && (dat)->length && ((dat)->data[0] == 0x7e ||\
                                    (dat)->data[0] == 0x3e))


#endif
