/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: krb5-glue.h,v $
 * Revision 1.1.18.2  1996/11/13  18:04:28  arvind
 * 	Merge to DCE_1.2.2
 * 	[1996/10/16  21:32 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce12_tp_preauth/1]
 *
 * 	Fix prototype for krb5_pa_enc_ts2PA_ENC_TS_ENC.
 * 	[1996/09/26  23:25 UTC  sommerfeld  /main/DCE_1.2.2/1]
 *
 * Revision 1.1.18.1  1996/10/03  14:57:17  arvind
 * 	detect malloc failures and return an error.
 * 
 * 	fill in the blanks for a couple new fields.
 * 
 * 	code cleanup.
 * 	merge more things in from beta6
 * 	add `const' to a bunch of places.
 * 
 * 	Add preauth for public key stuff.
 * 	Remove (meaningless) OSF_DCE ifdef.
 * 	[1996/09/16  21:59 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 * 
 * Revision 1.1.16.3  1996/02/18  23:02:49  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:21:19  marty]
 * 
 * Revision 1.1.16.2  1995/12/08  17:43:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:57:16  root]
 * 
 * Revision 1.1.12.1  1994/10/25  22:08:06  mullan_s
 * 	[OT12495] Plaintext third party preauth fixes: prototypes
 * 	for error info struct and new key struct.
 * 	[1994/10/25  22:07:31  mullan_s]
 * 
 * Revision 1.1.6.2  1994/08/26  18:45:45  mullan_s
 * 	[OT11557] Add prototypes for decoding/encoding DCE preauth subtype
 * 	routines.
 * 	[1994/08/25  18:21:46  mullan_s]
 * 
 * Revision 1.1.6.1  1994/05/11  19:17:44  ahop
 * 	hp_sec_to_osf_2 drop
 * 	Add prototypes for krb5_pa_third_party2PAThirdParty and
 * 	PAThirdParty2krb5_pa_third_party.
 * 	Add prototypes for krb5_time_and_key2TimeAndKey and TimeAndKey2krb5_time_and_key.
 * 	[1994/04/29  21:15:25  ahop]
 * 
 * Revision 1.1.4.2  1992/12/29  14:00:54  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:53:23  zeliff]
 * 
 * Revision 1.1.2.2  1992/04/10  07:51:10  sommerfeld
 * 	Use PROTOTYPE macro so file can be built by non-ANSI compilers.
 * 	[1992/04/09  20:46:19  sommerfeld]
 * 
 * Revision 1.1  1992/01/19  14:48:12  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
**
** Copyright (c) Hewlett-Packard Company 1991, 1994
** Unpublished work. All Rights Reserved.
**
*/
typedef krb5_address *krb5_addresses;
krb5_error_code krb5_addresses2HostAddresses (const krb5_addresses *k, HostAddresses *m);
krb5_error_code krb5_authdata_vec2AuthorizationData (const krb5_authdata_vec *k, AuthorizationData *m);
typedef krb5_last_req_entry *krb5_last_req_entries;
krb5_error_code krb5_last_req_entries2LastReq (const krb5_last_req_entries *k, LastReq *m);
typedef krb5_ticket *krb5_ticket_vec;
krb5_error_code krb5_ticket_vec2KDC_REQ_BODY_SEQUENCE_additional_tickets (const krb5_ticket_vec *k, KDC_REQ_BODY_SEQUENCE_additional_tickets *m);
krb5_error_code krb5_pa_data_vec2PA_DATA (const krb5_pa_data_vec *k, PA_DATA *m);
krb5_error_code HostAddresses2krb5_addresses (const HostAddresses *m, krb5_addresses **k);
krb5_error_code AuthorizationData2krb5_authdata_vec (const AuthorizationData *m, krb5_authdata_vec **k);
krb5_error_code LastReq2krb5_last_req_entries (const LastReq *m, krb5_last_req_entries **k);
krb5_error_code KDC_REQ_BODY_SEQUENCE_additional_tickets2krb5_ticket_vec (const KDC_REQ_BODY_SEQUENCE_additional_tickets *m, krb5_ticket_vec **k);
krb5_error_code PA_DATA2krb5_pa_data_vec (const PA_DATA *m, krb5_pa_data_vec **k);


krb5_error_code opt_krb5_kvno2int (const krb5_kvno *k, int **m);
krb5_error_code opt_krb5_ui_22int (const krb5_ui_2 *k, int **m);
krb5_error_code opt_krb5_int322int (const krb5_int32 *k, int **m);
krb5_error_code opt_krb5_data2asn1_field (const krb5_data *k, asn1_field **m);
krb5_error_code opt_krb5_timestamp2asn1_field (const krb5_timestamp *k, asn1_field **m);
krb5_error_code opt_krb5_authdata_vec2AuthorizationData (const krb5_authdata_vec *k, AuthorizationData **m);
krb5_error_code opt_krb5_ticket_vec2KDC_REQ_BODY_SEQUENCE_additional_tickets (const krb5_ticket_vec *k, KDC_REQ_BODY_SEQUENCE_additional_tickets **m);
krb5_error_code opt_krb5_addresses2HostAddresses (const krb5_addresses *k, HostAddresses **m);
krb5_error_code opt_krb5_last_req_entries2LastReq (const krb5_last_req_entries *k, LastReq **m);
krb5_error_code opt_krb5_keyblock2EncryptionKey (const krb5_keyblock *k, EncryptionKey **m);
krb5_error_code opt_krb5_address2HostAddress (const krb5_address *k, HostAddress **m);
krb5_error_code opt_krb5_checksum2Checksum (const krb5_checksum *k, Checksum **m);
krb5_error_code opt_krb5_enc_data2EncryptedData (const krb5_enc_data *k, EncryptedData **m);
krb5_error_code opt_krb5_pa_data_vec2PA_DATA (const krb5_pa_data_vec *k, PA_DATA **m);

krb5_error_code opt_int2krb5_kvno (const int *m, krb5_kvno *k);
krb5_error_code opt_int2krb5_ui_2 (const int *m, krb5_ui_2 *k);
krb5_error_code opt_int2krb5_int32 (const int *m, krb5_int32 *k);
krb5_error_code opt_asn1_field2krb5_data (const asn1_field *m, krb5_data *k);
krb5_error_code opt_asn1_field2krb5_timestamp (const asn1_field *m, krb5_timestamp *k);
krb5_error_code opt_AuthorizationData2krb5_authdata_vec (const AuthorizationData *m, krb5_authdata_vec **k);
krb5_error_code opt_KDC_REQ_BODY_SEQUENCE_additional_tickets2krb5_ticket_vec (const KDC_REQ_BODY_SEQUENCE_additional_tickets *m, krb5_ticket_vec **k);
krb5_error_code opt_HostAddresses2krb5_addresses (const HostAddresses *m, krb5_addresses **k);
krb5_error_code opt_LastReq2krb5_last_req_entries (const LastReq *m, krb5_last_req_entries **k);
krb5_error_code opt_Ticket2krb5_ticket (const Ticket *m, krb5_ticket **k);
krb5_error_code opt_EncryptionKey2krb5_keyblock (const EncryptionKey *m, krb5_keyblock **k);
krb5_error_code opt_HostAddress2krb5_address (const HostAddress *m, krb5_address **k);
krb5_error_code opt_Checksum2krb5_checksum (const Checksum *m, krb5_checksum **k);
krb5_error_code opt_EncryptedData2krb5_enc_data (const EncryptedData *m, krb5_enc_data *k);
krb5_error_code opt_PA_DATA2krb5_pa_data_vec (const PA_DATA *m, krb5_pa_data_vec **k);


krb5_error_code krb5_pa_data2PA_DATA_SEQUENCE_OF_SEQUENCE (const krb5_pa_data *k,PA_DATA_SEQUENCE_OF_SEQUENCE *m);
krb5_error_code krb5_address2HostAddresses_SEQUENCE_OF_SEQUENCE (const krb5_address *k,HostAddresses_SEQUENCE_OF_SEQUENCE *m);
krb5_error_code krb5_address2HostAddress (const krb5_address *k,HostAddress *m);
krb5_error_code krb5_authdata2AuthorizationData_SEQUENCE_OF_SEQUENCE (const krb5_authdata *k,AuthorizationData_SEQUENCE_OF_SEQUENCE *m);
krb5_error_code krb5_last_req_entry2LastReq_SEQUENCE_OF_SEQUENCE (const krb5_last_req_entry *k,LastReq_SEQUENCE_OF_SEQUENCE *m);
krb5_error_code krb5_enc_data2EncryptedData (const krb5_enc_data *k,EncryptedData *m);
krb5_error_code krb5_transited2TransitedEncoding (const krb5_transited *k,TransitedEncoding *m);
krb5_error_code krb5_authenticator2Authenticator (const krb5_authenticator *k,Authenticator *m);
krb5_error_code krb5_keyblock2EncryptionKey (const krb5_keyblock *k,EncryptionKey *m);
krb5_error_code krb5_checksum2Checksum (const krb5_checksum *k,Checksum *m);
krb5_error_code krb5_kdc_req2KDC_REQ_BODY (const krb5_kdc_req *k,KDC_REQ_BODY *m);
krb5_error_code krb5_kdc_req2TGS_REQ (const krb5_kdc_req *k,TGS_REQ *m);
krb5_error_code krb5_kdc_req2AS_REQ (const krb5_kdc_req *k,AS_REQ *m);
krb5_error_code krb5_ticket2Ticket (const krb5_ticket *k,Ticket *m);
krb5_error_code krb5_kdc_rep2TGS_REP (const krb5_kdc_rep *k,TGS_REP *m);
krb5_error_code krb5_enc_kdc_rep_part2EncASRepPart (const krb5_enc_kdc_rep_part *k,EncASRepPart *m);
krb5_error_code krb5_enc_kdc_rep_part2EncTGSRepPart (const krb5_enc_kdc_rep_part *k,EncTGSRepPart *m);
krb5_error_code krb5_ap_req2AP_REQ (const krb5_ap_req *k,AP_REQ *m);
krb5_error_code krb5_ap_rep2AP_REP (const krb5_ap_rep *k,AP_REP *m);
krb5_error_code krb5_ap_rep_enc_part2EncAPRepPart (const krb5_ap_rep_enc_part *k,EncAPRepPart *m);
krb5_error_code krb5_safe2KRB_SAFE (const krb5_safe *k,KRB_SAFE *m);
krb5_error_code krb5_priv2KRB_PRIV (const krb5_priv *k,KRB_PRIV *m);
krb5_error_code krb5_priv_enc_part2EncKrbPrivPart (const krb5_priv_enc_part *k,EncKrbPrivPart *m);
krb5_error_code krb5_error2KRB_ERROR (const krb5_error *k,KRB_ERROR *m);
krb5_error_code krb5_enc_tkt_part2EncTicketPart (const krb5_enc_tkt_part *k,EncTicketPart *m);
krb5_error_code PA_DATA_SEQUENCE_OF_SEQUENCE2krb5_pa_data (const PA_DATA_SEQUENCE_OF_SEQUENCE *m,krb5_pa_data *k);
krb5_error_code HostAddresses_SEQUENCE_OF_SEQUENCE2krb5_address (const HostAddresses_SEQUENCE_OF_SEQUENCE *m,krb5_address *k);
krb5_error_code HostAddress2krb5_address (const HostAddress *m,krb5_address *k);
krb5_error_code AuthorizationData_SEQUENCE_OF_SEQUENCE2krb5_authdata (const AuthorizationData_SEQUENCE_OF_SEQUENCE *m,krb5_authdata *k);
krb5_error_code LastReq_SEQUENCE_OF_SEQUENCE2krb5_last_req_entry (const LastReq_SEQUENCE_OF_SEQUENCE *m,krb5_last_req_entry *k);
krb5_error_code EncryptedData2krb5_enc_data (const EncryptedData *m,krb5_enc_data *k);
krb5_error_code TransitedEncoding2krb5_transited (const TransitedEncoding *m,krb5_transited *k);
krb5_error_code Authenticator2krb5_authenticator (const Authenticator *m,krb5_authenticator *k);
krb5_error_code EncryptionKey2krb5_keyblock (const EncryptionKey *m,krb5_keyblock *k);
krb5_error_code Checksum2krb5_checksum (const Checksum *m,krb5_checksum *k);
krb5_error_code KDC_REQ_BODY2krb5_kdc_req (const KDC_REQ_BODY *m,krb5_kdc_req *k);
krb5_error_code TGS_REQ2krb5_kdc_req (const TGS_REQ *m,krb5_kdc_req *k);
krb5_error_code AS_REQ2krb5_kdc_req (const AS_REQ *m,krb5_kdc_req *k);
krb5_error_code Ticket2krb5_ticket (const Ticket *m,krb5_ticket *k);
krb5_error_code TGS_REP2krb5_kdc_rep (const TGS_REP *m,krb5_kdc_rep *k);
krb5_error_code EncASRepPart2krb5_enc_kdc_rep_part (const EncASRepPart *m,krb5_enc_kdc_rep_part *k);
krb5_error_code EncTGSRepPart2krb5_enc_kdc_rep_part (const EncTGSRepPart *m,krb5_enc_kdc_rep_part *k);
krb5_error_code AP_REQ2krb5_ap_req (const AP_REQ *m,krb5_ap_req *k);
krb5_error_code AP_REP2krb5_ap_rep (const AP_REP *m,krb5_ap_rep *k);
krb5_error_code EncAPRepPart2krb5_ap_rep_enc_part (const EncAPRepPart *m,krb5_ap_rep_enc_part *k);
krb5_error_code KRB_SAFE2krb5_safe (const KRB_SAFE *m,krb5_safe *k);
krb5_error_code KRB_PRIV2krb5_priv (const KRB_PRIV *m,krb5_priv *k);
krb5_error_code EncKrbPrivPart2krb5_priv_enc_part (const EncKrbPrivPart *m,krb5_priv_enc_part *k);
krb5_error_code KRB_ERROR2krb5_error (const KRB_ERROR *m,krb5_error *k);
krb5_error_code EncTicketPart2krb5_enc_tkt_part (const EncTicketPart *m,krb5_enc_tkt_part *k);
krb5_error_code krb5_time_and_key2TimeAndKey (const krb5_time_and_key *k, TimeAndKey *m);
krb5_error_code TimeAndKey2krb5_time_and_key (const TimeAndKey *m, krb5_time_and_key *k);
krb5_error_code krb5_pa_third_party2PAThirdParty (const krb5_pa_third_party *k, PAThirdParty *m);
krb5_error_code PAThirdParty2krb5_pa_third_party (const PAThirdParty *m, krb5_pa_third_party *k);
krb5_error_code opt_asn1_field2krb5_data_ptr (const asn1_field *m, krb5_data **k);
krb5_error_code krb5_sub_pa_data2SubPaData (const krb5_sub_pa_data *k, SubPaData *m);
krb5_error_code SubPaData2krb5_sub_pa_data (const SubPaData *m, krb5_sub_pa_data *k);
krb5_error_code krb5_sub_err_data2SubErrData (const krb5_sub_err_data *k, SubErrData *m);
krb5_error_code SubErrData2krb5_sub_err_data (const SubErrData *m, krb5_sub_err_data *k);
krb5_error_code krb5_dce_pa_keys2DcePAKeys (const krb5_dce_pa_keys *k, DcePAKeys *m);
krb5_error_code DcePAKeys2krb5_dce_pa_keys (const DcePAKeys *m, krb5_dce_pa_keys *k);
krb5_error_code krb5_dce_pa_err_info2DcePAErrInfo (const krb5_dce_pa_err_info *k, DcePAErrInfo *m);
krb5_error_code DcePAErrInfo2krb5_dce_pa_err_info (const DcePAErrInfo *m, krb5_dce_pa_err_info *k);

krb5_error_code krb5_enc_data2Signature (const krb5_enc_data *k, Signature *m);
krb5_error_code krb5_pk_authenticator2PKAuthenticator(const krb5_pk_authenticator *k, PKAuthenticator *m);
krb5_error_code krb5_signed_pk_authenticator2SignedPKAuthenticator(const krb5_signed_pk_authenticator *k, SignedPKAuthenticator *m);
krb5_error_code krb5_pa_pk_as_root2PA_PK_AS_ROOT(const krb5_pa_pk_as_root *k, PA_PK_AS_ROOT *m);
krb5_error_code krb5_enc_pa_pk_as_rep_part2EncPaPkAsRepPart(const krb5_enc_pa_pk_as_rep_part *k, EncPaPkAsRepPart *m);
krb5_error_code krb5_enc_pa_pk_as_rep_part_shell2EncPaPkAsRepPartShell(const krb5_enc_pa_pk_as_rep_part_shell *k, EncPaPkAsRepPartShell *m);
krb5_error_code krb5_keyblock2EncPaPkAsRepTmpKey (const krb5_keyblock *k, EncPaPkAsRepTmpKey *m);

krb5_error_code krb5_pa_pk_as_rep2PA_PK_AS_REP(const krb5_pa_pk_as_rep *k, PA_PK_AS_REP *m);

krb5_error_code Signature2krb5_enc_data (const Signature *m, krb5_enc_data *k);
krb5_error_code PKAuthenticator2krb5_pk_authenticator(const PKAuthenticator *m, krb5_pk_authenticator *k);
krb5_error_code SignedPKAuthenticator2krb5_signed_pk_authenticator(const SignedPKAuthenticator *m, krb5_signed_pk_authenticator *k);
krb5_error_code PA_PK_AS_ROOT2krb5_pa_pk_as_root(const PA_PK_AS_ROOT *m, krb5_pa_pk_as_root *k);
krb5_error_code EncPaPkAsRepPart2krb5_enc_pa_pk_as_rep_part(const EncPaPkAsRepPart *m, krb5_enc_pa_pk_as_rep_part *k);
krb5_error_code EncPaPkAsRepPartShell2krb5_enc_pa_pk_as_rep_part_shell(const EncPaPkAsRepPartShell *m, krb5_enc_pa_pk_as_rep_part_shell *k);
krb5_error_code EncPaPkAsRepTmpKey2krb5_keyblock (const EncPaPkAsRepTmpKey *m, krb5_keyblock *k);
krb5_error_code PA_PK_AS_REP2krb5_pa_pk_as_rep(const PA_PK_AS_REP *m, krb5_pa_pk_as_rep *k);

krb5_error_code krb5_etype_info_entry2ETYPE_INFO_ENTRY(const krb5_etype_info_entry *k, ETYPE_INFO_ENTRY *m);
krb5_error_code ETYPE_INFO_ENTRY2krb5_etype_info_entry(const ETYPE_INFO_ENTRY *m, krb5_etype_info_entry *k);
krb5_error_code krb5_etype_info_entry_vec2ETYPE_INFO (const krb5_etype_info_entry_vec *k, ETYPE_INFO *m);
krb5_error_code ETYPE_INFO2krb5_etype_info_entry_vec (const ETYPE_INFO *m, krb5_etype_info_entry_vec **k);

krb5_error_code PA_SAM_CHALLENGE2krb5_sam_challenge (const PA_SAM_CHALLENGE *, krb5_sam_challenge *);
krb5_error_code krb5_sam_challenge2PA_SAM_CHALLENGE (const krb5_sam_challenge *, PA_SAM_CHALLENGE *);

krb5_error_code EncryptionKey2krb5_predicted_sam_response(const EncryptionKey *, krb5_predicted_sam_response *);
krb5_error_code krb5_predicted_sam_response2EncryptionKey(const krb5_predicted_sam_response *, EncryptionKey *);

krb5_error_code PA_ENC_TS_ENC2krb5_pa_enc_ts(const PA_ENC_TS_ENC *, krb5_pa_enc_ts *);
krb5_error_code krb5_pa_enc_ts2PA_ENC_TS_ENC(const krb5_pa_enc_ts *, PA_ENC_TS_ENC *);
