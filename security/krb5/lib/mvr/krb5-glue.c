/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: krb5-glue.c,v $
 * Revision 1.1.19.3  1996/11/13  18:09:25  arvind
 * 	Merge to DCE_1.2.2
 * 	[1996/10/16  21:31 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce12_tp_preauth/1]
 *
 * 	Add encoding/decoding code for krb5_pa_enc_ts.
 * 	[1996/09/26  23:27 UTC  sommerfeld  /main/DCE_1.2.2/4]
 *
 * 	Merge to DCE_1.2.2
 * 	[1996/06/10  18:20 UTC  mullan_s  /main/mullan_dce_krb5_der_work/3]
 *
 * 	EncKDCRepPart: only encode the renew_till timestamp if the TKT_FLG_RENEWABLE
 * 	bit is set in the flags.
 * 	KRB-SAFE: do not encode the usec portion of the time if the timestamp is 0.
 * 	EncKrbPrivPart: do not encode the usec portion of the time if the timestamp is 0.
 * 	[1996/06/07  14:20 UTC  mullan_s  /main/mullan_dce_krb5_der_work/2]
 *
 * 	Allow caddr field of krb5_enc_tkt_part data type to be OPTIONAL
 * 	as per RFC1510.
 * 	[1996/06/05  19:05 UTC  sommerfeld  /main/mullan_dce_krb5_der_work/1]
 *
 * 	Check for null pointers and encode empty sequences.
 * 	[1996/06/11  17:57 UTC  aha  /main/DCE_1.2.2/1]
 *
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 *
 * 	[1996/06/10  14:58 UTC  aha  /main/DCE122_PK/2]
 *
 * 	Merge to DCE122_PK
 *
 * 	[1996/06/07  18:06 UTC  aha  /main/DCE122_PK/aha_pk6/2]
 *
 * 	Changes for public key login; fix warnings.
 *
 * 	[1996/06/07  17:46 UTC  aha  /main/DCE122_PK/aha_pk6/1]
 *
 * 	Changes for Public Key Login; change uses of int2krb5_int32 and krb5_int322int
 * 	to direct copies.
 *
 * 	[1996/02/18  00:15:05  marty  1.1.17.3]
 *
 * 	Merge ASN.1 code and encoding functions for public key login
 *
 * 	[1996/04/16  21:33 UTC  aha  /main/aha_pk2/1]
 *
 * 	Additions for public key login
 *
 * 	[1994/10/25  22:13:21  mullan_s  1.1.13.1]
 *
 * Revision 1.1.19.2  1996/10/03  15:05:39  arvind
 * 	bugfix (operator precedence braino).
 * 	[1996/09/25  18:29 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_1/2]
 * 
 * 	detect malloc and other failures and return an error.
 * 
 * 	fill in the blanks (partly) for some of the new types.
 * 
 * 	constify.
 * 	finish implementing preauth.
 * 
 * 	Fixup glue to match new ASN.1 definitions.
 * 	[1996/09/16  21:28 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_1/1]
 * 
 * 	Fix array bounds corruption in krb5_enc_tkt_part2EncTicketPart(). CHFts19877.
 * 	[1996/08/23  20:02 UTC  jrr  /main/DCE_1.2.2/jrr_122_2/1]
 * 
 * 	Merge to DCE_1.2.2
 * 	[1996/06/10  18:20 UTC  mullan_s  /main/mullan_dce_krb5_der_work/3]
 * 
 * 	EncKDCRepPart: only encode the renew_till timestamp if the TKT_FLG_RENEWABLE
 * 	bit is set in the flags.
 * 	KRB-SAFE: do not encode the usec portion of the time if the timestamp is 0.
 * 	EncKrbPrivPart: do not encode the usec portion of the time if the timestamp is 0.
 * 	[1996/06/07  14:20 UTC  mullan_s  /main/mullan_dce_krb5_der_work/2]
 * 
 * 	Allow caddr field of krb5_enc_tkt_part data type to be OPTIONAL
 * 	as per RFC1510.
 * 	[1996/06/05  19:05 UTC  sommerfeld  /main/mullan_dce_krb5_der_work/1]
 * 
 * 	Check for null pointers and encode empty sequences.
 * 	[1996/06/11  17:57 UTC  aha  /main/DCE_1.2.2/1]
 * 
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 * 
 * 	[1996/06/10  14:58 UTC  aha  /main/DCE122_PK/2]
 * 
 * 	Merge to DCE122_PK
 * 
 * 	[1996/06/07  18:06 UTC  aha  /main/DCE122_PK/aha_pk6/2]
 * 
 * 	Changes for public key login; fix warnings.
 * 
 * 	[1996/06/07  17:46 UTC  aha  /main/DCE122_PK/aha_pk6/1]
 * 
 * 	Changes for Public Key Login; change uses of int2krb5_int32 and krb5_int322int
 * 	to direct copies.
 * 
 * 	[1996/02/18  00:15:05  marty  1.1.17.3]
 * 
 * 	Merge ASN.1 code and encoding functions for public key login
 * 
 * 	[1996/04/16  21:33 UTC  aha  /main/aha_pk2/1]
 * 
 * 	Additions for public key login
 * 
 * 	[1994/10/25  22:13:21  mullan_s  1.1.13.1]
 * 
 * Revision 1.1.19.1  1996/07/08  18:37:16  arvind
 * 	Merge to DCE_1.2.2
 * 	[1996/06/10  18:20 UTC  mullan_s  /main/mullan_dce_krb5_der_work/3]
 * 
 * 	EncKDCRepPart: only encode the renew_till timestamp if the TKT_FLG_RENEWABLE
 * 	bit is set in the flags.
 * 	KRB-SAFE: do not encode the usec portion of the time if the timestamp is 0.
 * 	EncKrbPrivPart: do not encode the usec portion of the time if the timestamp is 0.
 * 	[1996/06/07  14:20 UTC  mullan_s  /main/mullan_dce_krb5_der_work/2]
 * 
 * 	Allow caddr field of krb5_enc_tkt_part data type to be OPTIONAL
 * 	as per RFC1510.
 * 	[1996/06/05  19:05 UTC  sommerfeld  /main/mullan_dce_krb5_der_work/1]
 * 
 * 	Check for null pointers and encode empty sequences.
 * 	[1996/06/11  17:57 UTC  aha  /main/DCE_1.2.2/1]
 * 
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 * 
 * 	[1996/06/10  14:58 UTC  aha  /main/DCE122_PK/2]
 * 
 * 	Merge to DCE122_PK
 * 
 * 	[1996/06/07  18:06 UTC  aha  /main/DCE122_PK/aha_pk6/2]
 * 
 * 	Changes for public key login; fix warnings.
 * 
 * 	[1996/06/07  17:46 UTC  aha  /main/DCE122_PK/aha_pk6/1]
 * 
 * 	Changes for Public Key Login; change uses of int2krb5_int32 and krb5_int322int
 * 	to direct copies.
 * 
 * 	[1996/02/18  00:15:05  marty  1.1.17.3]
 * 
 * 	Merge ASN.1 code and encoding functions for public key login
 * 
 * 	[1996/04/16  21:33 UTC  aha  /main/aha_pk2/1]
 * 
 * 	Additions for public key login
 * 
 * 	[1994/10/25  22:13:21  mullan_s  1.1.13.1]
 * 
 * Revision 1.1.17.3  1996/02/18  00:15:05  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:04:29  marty]
 * 
 * Revision 1.1.17.2  1995/12/08  18:02:49  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:00:25  root]
 * 
 * Revision 1.1.13.1  1994/10/25  22:13:21  mullan_s
 * 	[OT12495] Plaintext third party preauth fixes
 * 	[1994/10/25  22:12:49  mullan_s]
 * 
 * Revision 1.1.6.3  1994/08/26  18:41:55  mullan_s
 * 	[OT11557] Add functions to decode/encode data structures which subtype
 * 	DCE preauth data.
 * 	[1994/08/25  18:15:36  mullan_s]
 * 
 * Revision 1.1.6.2  1994/08/25  17:32:25  aha
 * 	OT11743: fix size of malloc in function asn1_field2krb5_data_ptr()
 * 	[1994/08/25  17:31:24  aha]
 * 
 * 	OT11743: fix size of malloc in function asn1_field2krb5_data_ptr()
 * 	[1994/08/25  17:27:14  aha]
 * 
 * Revision 1.1.6.1  1994/05/11  19:24:50  ahop
 * 	hp_sec_to_osf_2 drop
 * 	Add support for encoding/decoding krb5_pa_third_party struct.
 * 	Add support for encoding/decoding krb5_time_and_key struct.
 * 	[1994/04/29  21:22:08  ahop]
 * 
 * Revision 1.1.4.2  1992/12/29  16:00:29  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:16:48  zeliff]
 * 
 * Revision 1.1.2.2  1992/04/16  15:11:22  pato
 * 	Fix OT2756 change encoding of authenticator to know about optional
 * 	authorization data (conformant with revision 5 of the
 * 	Kerberos V5 spec).  This change does not affect
 * 	compatibility with earlier releases of the DCE - and
 * 	currently there is no way to actually take advantage of
 * 	the new field.
 * 	[1992/04/14  21:24:32  pato]
 * 
 * Revision 1.1  1992/01/19  14:50:02  devrcs
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
/* This file was once automatically generated, but isn't any more. */
#include <krb5/krb5.h>
#include <krb5/asn1.h>
#include <dce/asn1.h>
#include <krb5/mvr/KRB5_mvr.h>
#include <krb5/mvr/KRB5.h>
#include <krb5/mvr/krb5-glue.h>
#include <krb5/mvr/krb5-manual-glue.h>

static krb5_data null_data = {0, 0};
static krb5_enc_data null_enc_data = { 0, 0, { 0, 0 }};

#define CHECK_ASN_MALLOC(t,ty,s) {size_t l = (s)*sizeof(ty); ty *t1=(ty*)asn1_malloc(l?l:1); (t)=t1; if (t1 == NULL) return ENOMEM; memset(t1,0,l);}

#define CHECK_ASN_MALLOC1(t,ty) {ty *t1=(ty*)asn1_malloc(sizeof(ty)); (t)=t1; if (t1 == NULL) return ENOMEM; memset(t1,0,sizeof(ty));}

#define CHECK_MALLOC(t,ty,s) {size_t l = (s)*sizeof(ty); ty *t1=(ty*)malloc(l?l:1); (t)=t1; if (t1 == NULL) return ENOMEM; memset(t1,0,l);}

#define CHECK_MALLOC1(t,ty) {ty *t1=(ty*)malloc(sizeof(ty)); (t)=t1; if (t1 == NULL) return ENOMEM; memset(t1,0,sizeof(ty));}

#define ERRCHECK() {if (errcode) return errcode;}


krb5_error_code krb5_addresses2HostAddresses(k, m)
    const krb5_addresses *k;
    HostAddresses *m;
{
    krb5_error_code errcode;
    int n=0, size;
    if (k != NULL)
	for (; k[n] != 0; n++) ;
    size = n;
    m->n = size;
    CHECK_ASN_MALLOC(m->v, HostAddresses_SEQUENCE_OF_SEQUENCE, size);
    for (n=0; n < size; n++) {
	errcode = krb5_address2HostAddresses_SEQUENCE_OF_SEQUENCE(k[n], &m->v[n]);
	ERRCHECK();
    }
    return 0;
}
krb5_error_code krb5_authdata_vec2AuthorizationData(k, m)
    const krb5_authdata_vec *k;
    AuthorizationData *m;
{
    krb5_error_code errcode;
    int n=0, size;
    if (k != NULL) for (; k[n] != 0; n++) ;
    size = n;
    m->n = size;
    CHECK_ASN_MALLOC(m->v, AuthorizationData_SEQUENCE_OF_SEQUENCE, size);
    for (n=0; n < size; n++) {
	errcode = krb5_authdata2AuthorizationData_SEQUENCE_OF_SEQUENCE(k[n], &m->v[n]);
	ERRCHECK();
    }
    return 0;
}
krb5_error_code krb5_last_req_entries2LastReq(k, m)
    const krb5_last_req_entries *k;
        LastReq *m;
{
    krb5_error_code errcode;
    int n=0, size;
    if (k != NULL) for (; k[n] != 0; n++) ;
    size = n;
    m->n = size;
    CHECK_ASN_MALLOC(m->v, LastReq_SEQUENCE_OF_SEQUENCE, size);
    for (n=0; n < size; n++) {
	errcode = krb5_last_req_entry2LastReq_SEQUENCE_OF_SEQUENCE(k[n], &m->v[n]);
	ERRCHECK();
    }
    return 0;
}
krb5_error_code krb5_ticket_vec2KDC_REQ_BODY_SEQUENCE_additional_tickets(k, m)
    const krb5_ticket_vec *k;
        KDC_REQ_BODY_SEQUENCE_additional_tickets *m;
{
    krb5_error_code errcode;
    int n=0, size;
    if (k != NULL) for (; k[n] != 0; n++) ;
    size = n;
    m->n = size;
    CHECK_ASN_MALLOC(m->v, Ticket, size);
    for (n=0; n < size; n++) {
	errcode = krb5_ticket2Ticket(k[n], &m->v[n]);
	ERRCHECK();
    }
    return 0;
}
krb5_error_code krb5_pa_data_vec2PA_DATA(k, m)
    const krb5_pa_data_vec *k;
        PA_DATA *m;
{
    krb5_error_code errcode;
    int n=0, size;
    if (k != NULL) for (; k[n] != 0; n++) ;
    size = n;
    m->n = size;
    CHECK_ASN_MALLOC(m->v, PA_DATA_SEQUENCE_OF_SEQUENCE, size);
    
    for (n=0; n < size; n++) {
	errcode = krb5_pa_data2PA_DATA_SEQUENCE_OF_SEQUENCE(k[n], &m->v[n]);
	ERRCHECK();
    }
    return 0;
}
krb5_error_code HostAddresses2krb5_addresses(m, k)
    const HostAddresses *m;
        krb5_addresses **k;
{
    krb5_error_code errcode;
    int n, size;
    size = m->n;
    CHECK_MALLOC(*k, krb5_address *, size+1);
    for (n=0; n < size; n++) {
	CHECK_MALLOC1((*k)[n], krb5_address);
	errcode = HostAddresses_SEQUENCE_OF_SEQUENCE2krb5_address
	    (&m->v[n], (*k)[n]);
	ERRCHECK();
    }
    (*k)[n] = 0;
    return 0;
}
krb5_error_code AuthorizationData2krb5_authdata_vec(m, k)
    const AuthorizationData *m;
        krb5_authdata_vec **k;
{
    krb5_error_code errcode;
    int n, size;
    size = m->n;
    CHECK_MALLOC(*k, krb5_authdata *, size+1);
    for (n=0; n < size; n++) {
	CHECK_MALLOC1((*k)[n], krb5_authdata);
	errcode = AuthorizationData_SEQUENCE_OF_SEQUENCE2krb5_authdata(&m->v[n], (*k)[n]);
	ERRCHECK();
    }
    (*k)[n] = 0;
	return 0;
}
krb5_error_code LastReq2krb5_last_req_entries(m, k)
    const LastReq *m;
        krb5_last_req_entries **k;
{
    krb5_error_code errcode;
    int n, size;
    size = m->n;
    CHECK_MALLOC(*k, krb5_last_req_entry *, size+1);
    for (n=0; n < size; n++) {
	CHECK_MALLOC1((*k)[n], krb5_last_req_entry);
	errcode = LastReq_SEQUENCE_OF_SEQUENCE2krb5_last_req_entry(&m->v[n], (*k)[n]);
	ERRCHECK();
    }
    (*k)[n] = 0;
	return 0;
}
krb5_error_code KDC_REQ_BODY_SEQUENCE_additional_tickets2krb5_ticket_vec(m, k)
    const KDC_REQ_BODY_SEQUENCE_additional_tickets *m;
        krb5_ticket_vec **k;
{
    krb5_error_code errcode;
    int n, size;
    size = m->n;
    CHECK_MALLOC(*k, krb5_ticket *, size+1);    
    for (n=0; n < size; n++) {
	CHECK_MALLOC1((*k)[n], krb5_ticket);	
	errcode = Ticket2krb5_ticket(&m->v[n], (*k)[n]);
	ERRCHECK();
    }
    (*k)[n] = 0;
	return 0;
}
krb5_error_code PA_DATA2krb5_pa_data_vec(m, k)
    const PA_DATA *m;
        krb5_pa_data_vec **k;
{
    krb5_error_code errcode;
    int n, size;
    size = m->n;
    CHECK_MALLOC(*k, krb5_pa_data *, size+1);    
    for (n=0; n < size; n++) {
	CHECK_MALLOC1((*k)[n], krb5_pa_data);		
	errcode = PA_DATA_SEQUENCE_OF_SEQUENCE2krb5_pa_data(&m->v[n], (*k)[n]);
	ERRCHECK();
    }
    (*k)[n] = 0;
	return 0;
}
krb5_error_code opt_krb5_kvno2int(k, m)
    const krb5_kvno *k;
    int **m;
{
    if ( *k == 0) {
	*m = NULL;
    } else {
	int *ret;
	CHECK_ASN_MALLOC1(ret, int);
	*ret = *k;
	*m = ret;
    }
    return 0;
}
krb5_error_code opt_krb5_ui_22int(k, m)
    const krb5_ui_2 *k;
    int **m;
{
    if ( *k == 0) {
	*m = NULL;
    } else {
	int *ret;
	CHECK_ASN_MALLOC1(ret, int);
	*ret = *k;
	*m = ret;
    }
    return 0;
}
krb5_error_code opt_krb5_int322int(k, m)
    const krb5_int32 *k;
    int **m;
{
    if ( *k == 0) {
	*m = NULL;
    } else {
	int *ret;
	CHECK_ASN_MALLOC1(ret, int);
	*ret = *k;
	*m = ret;
    }
    return 0;
}
krb5_error_code opt_krb5_data2asn1_field(k, m)
    const krb5_data *k;
    asn1_field **m;
{
    krb5_error_code errcode;
    if ( k->length == 0) {
	*m = NULL;
    } else {
	asn1_field *ret;
	CHECK_ASN_MALLOC1(ret, asn1_field);
	errcode = krb5_data2asn1_field(k, ret);
	ERRCHECK();
	*m = ret;
    }
    return 0;
}
krb5_error_code opt_krb5_timestamp2asn1_field(k, m)
    const krb5_timestamp *k;
    asn1_field **m;
{
    krb5_error_code errcode;
    if ((*k) == 0) {
	*m = NULL;
    } else {
	asn1_field *ret;
	CHECK_ASN_MALLOC1(ret, asn1_field);
	errcode = krb5_timestamp2asn1_field(k, ret);
	ERRCHECK();
	*m = ret;
    }
    return 0;
}
krb5_error_code opt_krb5_authdata_vec2AuthorizationData(k, m)
    const krb5_authdata_vec *k;
    AuthorizationData **m;
{
    krb5_error_code errcode;
    if ( (k == 0) || ((*k) == 0))
	*m = NULL;
    else
    {
	AuthorizationData *ret;
	CHECK_ASN_MALLOC1(ret, AuthorizationData);
	errcode = krb5_authdata_vec2AuthorizationData(k, ret);
	ERRCHECK();
	*m = ret;
    }
    return 0;
}
krb5_error_code opt_krb5_ticket_vec2KDC_REQ_BODY_SEQUENCE_additional_tickets(k, m)
    const krb5_ticket_vec *k;
    KDC_REQ_BODY_SEQUENCE_additional_tickets **m;
{
    krb5_error_code errcode;
    if ( (k == 0) || ((*k) == 0))
	*m = NULL;
    else
    {
	KDC_REQ_BODY_SEQUENCE_additional_tickets *ret;
	CHECK_ASN_MALLOC1(ret, KDC_REQ_BODY_SEQUENCE_additional_tickets);
	errcode = krb5_ticket_vec2KDC_REQ_BODY_SEQUENCE_additional_tickets(k, ret);
	ERRCHECK();
	*m = ret;
    }
    return 0;
}
krb5_error_code opt_krb5_addresses2HostAddresses(k, m)
    const krb5_addresses *k;
    HostAddresses **m;
{
    krb5_error_code errcode;
    if ( (k == 0) || ((*k) == 0))
	*m = NULL;
    else
    {
	HostAddresses *ret;
	CHECK_ASN_MALLOC1(ret, HostAddresses);
	errcode = krb5_addresses2HostAddresses(k, ret);
	ERRCHECK();
	*m = ret;
    }
    return 0;
}
krb5_error_code opt_krb5_last_req_entries2LastReq(k, m)
    const krb5_last_req_entries *k;
    LastReq **m;
{
    krb5_error_code errcode;
    if ( (k == 0) || ((*k) == 0))
	*m = NULL;
    else
    {
	LastReq *ret;
	CHECK_ASN_MALLOC1(ret, LastReq);
	errcode = krb5_last_req_entries2LastReq(k, ret);
	ERRCHECK();
	*m = ret;
    }
    return 0;
}
krb5_error_code opt_krb5_keyblock2EncryptionKey(k, m)
    const krb5_keyblock *k;
    EncryptionKey **m;
{
    krb5_error_code errcode;
    if ( (k == 0))
	*m = NULL;
    else
    {
	EncryptionKey *ret;
	CHECK_ASN_MALLOC1(ret, EncryptionKey);
	errcode = krb5_keyblock2EncryptionKey(k, ret);
	ERRCHECK();
	*m = ret;
    }
    return 0;
}
krb5_error_code opt_krb5_address2HostAddress(k, m)
    const krb5_address *k;
    HostAddress **m;
{
    krb5_error_code errcode;
    if ( (k == 0))
	*m = NULL;
    else
    {
	HostAddress *ret;
	CHECK_ASN_MALLOC1(ret, HostAddress);
	errcode = krb5_address2HostAddress(k, ret);
	ERRCHECK();
	*m = ret;
    }
    return 0;
}
krb5_error_code opt_krb5_checksum2Checksum(k, m)
    const krb5_checksum *k;
    Checksum **m;
{
    krb5_error_code errcode;
    if ( (k == 0))
	*m = NULL;
    else
    {
	Checksum *ret;
	CHECK_ASN_MALLOC1(ret, Checksum);
	errcode = krb5_checksum2Checksum(k, ret);
	ERRCHECK();
	*m = ret;
    }
    return 0;
}
krb5_error_code opt_krb5_enc_data2EncryptedData(k, m)
    const krb5_enc_data *k;
    EncryptedData **m;
{
    krb5_error_code errcode;
    if ( ((k->ciphertext.length == 0) || (k->ciphertext.data == 0)))
	*m = NULL;
    else
    {
	EncryptedData *ret;
	CHECK_ASN_MALLOC1(ret, EncryptedData);
	errcode = krb5_enc_data2EncryptedData(k, ret);
	ERRCHECK();
	*m = ret;
    }
    return 0;
}
krb5_error_code opt_krb5_pa_data_vec2PA_DATA(k, m)
    const krb5_pa_data_vec *k;
    PA_DATA **m;
{
    krb5_error_code errcode;
    if ( (k == 0) || (*k == 0))
	*m = NULL;
    else
    {
	PA_DATA *ret;
	CHECK_ASN_MALLOC1(ret, PA_DATA);
	errcode = krb5_pa_data_vec2PA_DATA(k, ret);
	ERRCHECK();
	*m = ret;
    }
    return 0;
}
krb5_error_code opt_int2krb5_kvno(m, k)
    const int *m;
    krb5_kvno *k;
{
    if ( m == 0)
	*k = 0;
    else
    {
	krb5_kvno ret;
	ret = *m;
	*k = ret;
    }
    return 0;
}
krb5_error_code opt_int2krb5_ui_2(m, k)
    const int *m;
    krb5_ui_2 *k;
{
    if ( m == 0)
	*k = 0;
    else
    {
	krb5_ui_2 ret;
	ret = *m;
	*k = ret;
    }
    return 0;
}
krb5_error_code opt_int2krb5_int32(m, k)
    const int *m;
    krb5_int32 *k;
{
    if ( m == 0)
	*k = 0;
    else
    {
	krb5_int32 ret;
	ret = *m;
	*k = ret;
    }
    return 0;
}
krb5_error_code opt_asn1_field2krb5_data(m, k)
    const asn1_field *m;
    krb5_data *k;
{
    krb5_error_code errcode;
        if ( m == 0)
                *k = null_data;
        else
        {
                krb5_data ret;
                errcode = asn1_field2krb5_data(m, &ret);
	ERRCHECK();
                *k = ret;
        }
    return 0;
}
krb5_error_code opt_asn1_field2krb5_timestamp(m, k)
    const asn1_field *m;
    krb5_timestamp *k;
{
    krb5_error_code errcode;
        if ( m == 0)
                *k = 0;
        else
        {
                krb5_timestamp ret;
                errcode = asn1_field2krb5_timestamp(m, &ret);
		ERRCHECK();
                *k = ret;
        }
    return 0;
}
krb5_error_code opt_AuthorizationData2krb5_authdata_vec(m, k)
    const AuthorizationData *m;
    krb5_authdata_vec **k;
{
    krb5_error_code errcode;
        if ( m == 0)
                *k = NULL;
        else
        {
                krb5_authdata_vec *ret;
                errcode = AuthorizationData2krb5_authdata_vec(m, &ret);
	ERRCHECK();
                *k = ret;
        }
    return 0;
}
krb5_error_code opt_KDC_REQ_BODY_SEQUENCE_additional_tickets2krb5_ticket_vec(m, k)
    const KDC_REQ_BODY_SEQUENCE_additional_tickets *m;
    krb5_ticket_vec **k;
{
    krb5_error_code errcode;
        if ( m == 0)
                *k = NULL;
        else
        {
                krb5_ticket_vec *ret;
                errcode = KDC_REQ_BODY_SEQUENCE_additional_tickets2krb5_ticket_vec(m, &ret);
	ERRCHECK();
                *k = ret;
        }
    return 0;
}
krb5_error_code opt_HostAddresses2krb5_addresses(m, k)
    const HostAddresses *m;
    krb5_addresses **k;
{
    krb5_error_code errcode;
        if ( m == 0)
                *k = NULL;
        else
        {
                krb5_addresses *ret;
                errcode = HostAddresses2krb5_addresses(m, &ret);
	ERRCHECK();
                *k = ret;
        }
    return 0;
}
krb5_error_code opt_LastReq2krb5_last_req_entries(m, k)
    const LastReq *m;
    krb5_last_req_entries **k;
{
    krb5_error_code errcode;
        if ( m == 0)
                *k = NULL;
        else
        {
                krb5_last_req_entries *ret;
                errcode = LastReq2krb5_last_req_entries(m, &ret);
	ERRCHECK();
                *k = ret;
        }
    return 0;
}
krb5_error_code opt_Ticket2krb5_ticket(m, k)
    const Ticket *m;
    krb5_ticket **k;
{
    krb5_error_code errcode;
        if ( m == 0)
                *k = NULL;
        else
        {
                krb5_ticket *ret;
		CHECK_MALLOC1(ret, krb5_ticket);
                errcode = Ticket2krb5_ticket(m, ret);
	ERRCHECK();
                *k = ret;
        }
    return 0;
}
krb5_error_code opt_EncryptionKey2krb5_keyblock(m, k)
    const EncryptionKey *m;
    krb5_keyblock **k;
{
    krb5_error_code errcode;
    if ( m == 0)
	*k = NULL;
    else
    {
	krb5_keyblock *ret;
	CHECK_MALLOC1(ret, krb5_keyblock);
	errcode = EncryptionKey2krb5_keyblock(m, ret);
	ERRCHECK();
	*k = ret;
    }
    return 0;
}
krb5_error_code opt_HostAddress2krb5_address(m, k)
    const HostAddress *m;
    krb5_address **k;
{
    krb5_error_code errcode;
    if ( m == 0)
	*k = NULL;
    else
    {
	krb5_address *ret;
	CHECK_MALLOC1(ret, krb5_address);
	errcode = HostAddress2krb5_address(m, ret);
	ERRCHECK();
	*k = ret;
    }
    return 0;
}
krb5_error_code opt_Checksum2krb5_checksum(m, k)
    const Checksum *m;
    krb5_checksum **k;
{
    krb5_error_code errcode;
    if ( m == 0)
	*k = NULL;
    else
    {
	krb5_checksum *ret;
	CHECK_MALLOC1(ret, krb5_checksum);
	errcode = Checksum2krb5_checksum(m, ret);
	ERRCHECK();
	*k = ret;
    }
    return 0;
}
krb5_error_code opt_EncryptedData2krb5_enc_data(m, k)
    const EncryptedData *m;
    krb5_enc_data *k;
{
    krb5_error_code errcode;
    if ( m == 0)
	*k = null_enc_data;
    else
    {
	krb5_enc_data ret;
	errcode = EncryptedData2krb5_enc_data(m, &ret);
	ERRCHECK();
	*k = ret;
    }
    return 0;
}
krb5_error_code opt_PA_DATA2krb5_pa_data_vec(m, k)
    const PA_DATA *m;
    krb5_pa_data_vec **k;
{
    krb5_error_code errcode;
        if ( m == 0)
                *k = NULL;
        else
        {
                krb5_pa_data_vec *ret;
                errcode = PA_DATA2krb5_pa_data_vec(m, &ret);
	ERRCHECK();
                *k = ret;
        }
    return 0;
}
krb5_error_code krb5_pa_data2PA_DATA_SEQUENCE_OF_SEQUENCE(k, m)
    const krb5_pa_data *k;
    PA_DATA_SEQUENCE_OF_SEQUENCE *m;
{
    krb5_error_code errcode;
    m->padata_type = k->pa_type;
    errcode = krb5_octets2asn1_field (&k->length, &k->contents, &m->pa_data);
	ERRCHECK();
    return 0;
}

krb5_error_code krb5_address2HostAddresses_SEQUENCE_OF_SEQUENCE(k, m)
    const krb5_address *k;
    HostAddresses_SEQUENCE_OF_SEQUENCE *m;
{
    krb5_error_code errcode;
    m->addr_type = k->addrtype;
    errcode = krb5_octets2asn1_field (&k->length, &k->contents, &m->address);
	ERRCHECK();
    return 0;
}

krb5_error_code krb5_address2HostAddress(k, m)
    const krb5_address *k;
    HostAddress *m;
{
    krb5_error_code errcode;
    m->addr_type = k->addrtype;
    errcode = krb5_octets2asn1_field (&k->length, &k->contents, &m->address);
	ERRCHECK();
    return 0;
}

krb5_error_code krb5_authdata2AuthorizationData_SEQUENCE_OF_SEQUENCE(k, m)
    const krb5_authdata *k;
        AuthorizationData_SEQUENCE_OF_SEQUENCE *m;
{
    krb5_error_code errcode;
    m->ad_type = k->ad_type;
    errcode = krb5_octets2asn1_field (&k->length, &k->contents, &m->ad_data);
	ERRCHECK();
    return 0;
}

krb5_error_code krb5_last_req_entry2LastReq_SEQUENCE_OF_SEQUENCE(k, m)
    const krb5_last_req_entry *k;
        LastReq_SEQUENCE_OF_SEQUENCE *m;
{
    krb5_error_code errcode;
    m->lr_type = k->lr_type;
    errcode = krb5_timestamp2asn1_field (&k->value, &m->lr_value);
	ERRCHECK();
    return 0;
}

krb5_error_code krb5_enc_data2EncryptedData(k, m)
    const krb5_enc_data *k;
        EncryptedData *m;
{
    krb5_error_code errcode;
    m->etype = k->etype;
    errcode = opt_krb5_kvno2int(&k->kvno, &m->kvno);
	ERRCHECK();
    errcode = krb5_data2asn1_field (&k->ciphertext, &m->cipher);
	ERRCHECK();
    return 0;
}

krb5_error_code krb5_transited2TransitedEncoding(k, m)
    const krb5_transited *k;
        TransitedEncoding *m;
{
    krb5_error_code errcode;
    m->tr_type = k->tr_type;
    errcode = krb5_data2asn1_field (&k->tr_contents, &m->contents);
	ERRCHECK();
    return 0;
}

krb5_error_code krb5_authenticator2Authenticator(k, m)
    const krb5_authenticator *k;
        Authenticator *m;
{
    krb5_error_code errcode;
    m->authenticator_vno = KRB5_PVNO;
    errcode = krb5_principal2PrincipalRealm (&k->client, &m->cname, &m->crealm);
	ERRCHECK();
    errcode = opt_krb5_checksum2Checksum(k->checksum, &m->cksum);
	ERRCHECK();
    m->cusec = k->cusec;
    errcode = krb5_timestamp2asn1_field (&k->ctime, &m->ctime);
	ERRCHECK();
    errcode = opt_krb5_keyblock2EncryptionKey(k->subkey, &m->subkey);
	ERRCHECK();
    errcode = opt_krb5_int322int(&k->seq_number, &m->seq_number);
	ERRCHECK();
    /*
     * Optional authorization data is not currently passed through
     */
    m->authorization_data = NULL;
    return 0;
}

krb5_error_code krb5_keyblock2EncryptionKey(k, m)
    const krb5_keyblock *k;
        EncryptionKey *m;
{
    krb5_error_code errcode;
    m->keytype = k->keytype;
    errcode = krb5_octets2asn1_field (&k->length, &k->contents, &m->keyvalue);
	ERRCHECK();
    return 0;
}

krb5_error_code krb5_checksum2Checksum(k, m)
    const krb5_checksum *k;
        Checksum *m;
{
    krb5_error_code errcode;
    m->cksumtype = k->checksum_type;
    errcode = krb5_octets2asn1_field (&k->length, &k->contents, &m->checksum);
	ERRCHECK();
    return 0;
}

krb5_error_code krb5_kdc_req2KDC_REQ_BODY(k, m)
    const krb5_kdc_req *k;
        KDC_REQ_BODY *m;
{
    krb5_error_code errcode;
    krb5_flags2asn1_field (&k->kdc_options, &m->kdc_options);
    /* !!! Realm check */
    errcode = krb5_principal2OPrincipalRealm (&k->client, &m->cname, 0);
	ERRCHECK();
    errcode = krb5_principal2PrincipalRealm (&k->server, &m->sname, &m->realm);
	ERRCHECK();

    errcode = opt_krb5_timestamp2asn1_field(&k->from, &m->from);
	ERRCHECK();
    errcode = krb5_timestamp2asn1_field (&k->till, &m->till);
	ERRCHECK();
    errcode = opt_krb5_timestamp2asn1_field(&k->rtime, &m->rtime);
	ERRCHECK();
    m->nonce = k->nonce;
    errcode = krb5_enctype_vec2INTSEQ (&k->etype, &k->netypes, &m->etype);
	ERRCHECK();
    errcode = opt_krb5_addresses2HostAddresses(k->addresses, &m->addresses);
	ERRCHECK();
    errcode = opt_krb5_enc_data2EncryptedData(&k->authorization_data, &m->enc_authorization_data);
	ERRCHECK();
    errcode = opt_krb5_ticket_vec2KDC_REQ_BODY_SEQUENCE_additional_tickets(k->second_ticket, &m->additional_tickets);
	ERRCHECK();
    return 0;
}

krb5_error_code krb5_kdc_req2TGS_REQ(k, m)
    const krb5_kdc_req *k;
        TGS_REQ *m;
{
    krb5_error_code errcode;
    m->pvno = KRB5_PVNO;
    m->msg_type = k->msg_type;
    errcode = opt_krb5_pa_data_vec2PA_DATA(k->padata, &m->padata);
	ERRCHECK();
    errcode = krb5_flags2asn1_field (&k->kdc_options, &m->req_body.kdc_options);
	ERRCHECK();
    /* !!! Realm check */
    errcode = krb5_principal2OPrincipalRealm (&k->client, &m->req_body.cname, 0);
	ERRCHECK();
    errcode = krb5_principal2PrincipalRealm (&k->server, &m->req_body.sname,
				   &m->req_body.realm);
	ERRCHECK();
    errcode = opt_krb5_timestamp2asn1_field(&k->from, &m->req_body.from);
	ERRCHECK();
    errcode = krb5_timestamp2asn1_field (&k->till, &m->req_body.till);
	ERRCHECK();
    errcode = opt_krb5_timestamp2asn1_field(&k->rtime, &m->req_body.rtime);
	ERRCHECK();
    m->req_body.nonce = k->nonce;
    errcode = krb5_enctype_vec2INTSEQ (&k->etype, &k->netypes, &m->req_body.etype);
	ERRCHECK();
    errcode = opt_krb5_addresses2HostAddresses(k->addresses, &m->req_body.addresses);
	ERRCHECK();
    errcode = opt_krb5_enc_data2EncryptedData(&k->authorization_data, &m->req_body.enc_authorization_data);
	ERRCHECK();
    errcode = opt_krb5_ticket_vec2KDC_REQ_BODY_SEQUENCE_additional_tickets(k->second_ticket, &m->req_body.additional_tickets);
	ERRCHECK();
    return 0;
}

krb5_error_code krb5_kdc_req2AS_REQ(k, m)
    const krb5_kdc_req *k;
    AS_REQ *m;
{
    krb5_error_code errcode;
    m->pvno = KRB5_PVNO;
    m->msg_type = k->msg_type;
    errcode = opt_krb5_pa_data_vec2PA_DATA(k->padata, &m->padata);
	ERRCHECK();
    errcode = krb5_flags2asn1_field (&k->kdc_options, &m->req_body.kdc_options);
	ERRCHECK();
    /* !!! Realm check */
    errcode = krb5_principal2OPrincipalRealm (&k->client, &m->req_body.cname, 0);
	ERRCHECK();
    errcode = krb5_principal2PrincipalRealm (&k->server, &m->req_body.sname, &m->req_body.realm);
	ERRCHECK();
    errcode = opt_krb5_timestamp2asn1_field(&k->from, &m->req_body.from);
	ERRCHECK();
    errcode = krb5_timestamp2asn1_field (&k->till, &m->req_body.till);
	ERRCHECK();
    errcode = opt_krb5_timestamp2asn1_field(&k->rtime, &m->req_body.rtime);
	ERRCHECK();
    m->req_body.nonce = k->nonce;
    errcode = krb5_enctype_vec2INTSEQ (&k->etype, &k->netypes, &m->req_body.etype);
	ERRCHECK();
    errcode = opt_krb5_addresses2HostAddresses(k->addresses, &m->req_body.addresses);
	ERRCHECK();
    errcode = opt_krb5_enc_data2EncryptedData(&k->authorization_data, &m->req_body.enc_authorization_data);
	ERRCHECK();
    errcode = opt_krb5_ticket_vec2KDC_REQ_BODY_SEQUENCE_additional_tickets(k->second_ticket, &m->req_body.additional_tickets);
	ERRCHECK();
    return 0;
}

krb5_error_code krb5_ticket2Ticket(k, m)
    const krb5_ticket *k;
        Ticket *m;
{
    krb5_error_code errcode;
        m->tkt_vno = KRB5_PVNO;
        errcode = krb5_principal2PrincipalRealm (&k->server, &m->sname, &m->realm);
	ERRCHECK();
        errcode = krb5_enc_data2EncryptedData (&k->enc_part, &m->enc_part);
	ERRCHECK();
    return 0;
}

krb5_error_code krb5_kdc_rep2TGS_REP(k, m)
    const krb5_kdc_rep *k;
        TGS_REP *m;
{
    krb5_error_code errcode;
        m->pvno = KRB5_PVNO;
        m->msg_type = k->msg_type;
        errcode = opt_krb5_pa_data_vec2PA_DATA(k->padata, &m->padata);
	ERRCHECK();
        errcode = krb5_principal2PrincipalRealm (&k->client, &m->cname, &m->crealm);
	ERRCHECK();
        errcode = krb5_ticket2Ticket (k->ticket, &m->ticket);
	ERRCHECK();
        errcode = krb5_enc_data2EncryptedData (&k->enc_part, &m->enc_part);
	ERRCHECK();
    return 0;
}

krb5_error_code krb5_enc_kdc_rep_part2EncASRepPart(k, m)
    const krb5_enc_kdc_rep_part *k;
        EncASRepPart *m;
{
    krb5_error_code errcode;
    errcode = krb5_keyblock2EncryptionKey (k->session, &m->key);
	ERRCHECK();
    errcode = krb5_last_req_entries2LastReq (k->last_req, &m->last_req);
	ERRCHECK();
        m->nonce = k->nonce;
        errcode = opt_krb5_timestamp2asn1_field(&k->key_exp, &m->key_expiration);
	ERRCHECK();
        errcode = krb5_flags2asn1_field (&k->flags, &m->flags);
	ERRCHECK();
        errcode = krb5_timestamp2asn1_field (&k->times.authtime, &m->authtime);
	ERRCHECK();
        errcode = opt_krb5_timestamp2asn1_field(&k->times.starttime, &m->starttime);
	ERRCHECK();
        errcode = krb5_timestamp2asn1_field (&k->times.endtime, &m->endtime);
	ERRCHECK();
	if (k->flags & TKT_FLG_RENEWABLE) {
	    errcode = opt_krb5_timestamp2asn1_field(&k->times.renew_till, &m->renew_till);
	    ERRCHECK();
	} else {
	    m->renew_till = 0;
	}
        errcode = krb5_principal2PrincipalRealm (&k->server, &m->sname, &m->srealm);
	ERRCHECK();
        errcode = opt_krb5_addresses2HostAddresses(k->caddrs, &m->caddr);
	ERRCHECK();
    return 0;
}

krb5_error_code krb5_enc_kdc_rep_part2EncTGSRepPart(k, m)
    const krb5_enc_kdc_rep_part *k;
        EncTGSRepPart *m;
{
    krb5_error_code errcode;
    errcode = krb5_keyblock2EncryptionKey (k->session, &m->key);
	ERRCHECK();
    errcode = krb5_last_req_entries2LastReq (k->last_req, &m->last_req);
	ERRCHECK();
        m->nonce = k->nonce;
        errcode = opt_krb5_timestamp2asn1_field(&k->key_exp, &m->key_expiration);
	ERRCHECK();
        errcode = krb5_flags2asn1_field (&k->flags, &m->flags);
	ERRCHECK();
        errcode = krb5_timestamp2asn1_field (&k->times.authtime, &m->authtime);
	ERRCHECK();
        errcode = opt_krb5_timestamp2asn1_field(&k->times.starttime, &m->starttime);
	ERRCHECK();
        errcode = krb5_timestamp2asn1_field (&k->times.endtime, &m->endtime);
	ERRCHECK();
	if (k->flags & TKT_FLG_RENEWABLE) {
	    errcode = opt_krb5_timestamp2asn1_field(&k->times.renew_till, &m->renew_till);
	    ERRCHECK();
	} else {
	    m->renew_till = 0;
	}
        errcode = krb5_principal2PrincipalRealm (&k->server, &m->sname, &m->srealm);
	ERRCHECK();
        errcode = opt_krb5_addresses2HostAddresses(k->caddrs, &m->caddr);
	ERRCHECK();
    return 0;
}

krb5_error_code krb5_ap_req2AP_REQ(k, m)
    const krb5_ap_req *k;
        AP_REQ *m;
{
    krb5_error_code errcode;
        m->pvno = KRB5_PVNO;
        m->msg_type = KRB5_AP_REQ;
        errcode = krb5_flags2asn1_field (&k->ap_options, &m->ap_options);
	ERRCHECK();
        errcode = krb5_ticket2Ticket (k->ticket, &m->ticket);
	ERRCHECK();
        errcode = krb5_enc_data2EncryptedData (&k->authenticator, &m->authenticator);
	ERRCHECK();
    return 0;
}

krb5_error_code krb5_ap_rep2AP_REP(k, m)
    const krb5_ap_rep *k;
        AP_REP *m;
{
    krb5_error_code errcode;
        m->pvno = KRB5_PVNO;
        m->msg_type = KRB5_AP_REP;
        errcode = krb5_enc_data2EncryptedData (&k->enc_part, &m->enc_part);
	ERRCHECK();
    return 0;
}

krb5_error_code krb5_ap_rep_enc_part2EncAPRepPart(k, m)
    const krb5_ap_rep_enc_part *k;
        EncAPRepPart *m;
{
    krb5_error_code errcode;
        krb5_timestamp2asn1_field (&k->ctime, &m->ctime);
        m->cusec = k->cusec;
        errcode = opt_krb5_keyblock2EncryptionKey(k->subkey, &m->subkey);
	ERRCHECK();
        errcode = opt_krb5_int322int(&k->seq_number, &m->seq_number);
	ERRCHECK();
    return 0;
}

krb5_error_code krb5_safe2KRB_SAFE(k, m)
    const krb5_safe *k;
        KRB_SAFE *m;
{
    krb5_error_code errcode;
        m->pvno = KRB5_PVNO;
        m->msg_type = KRB5_SAFE;
        errcode = krb5_data2asn1_field (&k->user_data, &m->safe_body.user_data);
	ERRCHECK();
        errcode = opt_krb5_timestamp2asn1_field(&k->timestamp, &m->safe_body.timestamp);
	ERRCHECK();
	if (k->timestamp) {
	    errcode = opt_krb5_int322int(&k->usec, &m->safe_body.usec);
	    ERRCHECK();
	} else {
	    m->safe_body.usec = 0;
	}
        errcode = opt_krb5_int322int(&k->seq_number, &m->safe_body.seq_number);
	ERRCHECK();
        errcode = krb5_address2HostAddress (k->s_address, &m->safe_body.s_address);
	ERRCHECK();
        errcode = opt_krb5_address2HostAddress(k->r_address, &m->safe_body.r_address);
	ERRCHECK();
        errcode = krb5_checksum2Checksum (k->checksum, &m->cksum);
	ERRCHECK();
    return 0;
}

krb5_error_code krb5_priv2KRB_PRIV(k, m)
    const krb5_priv *k;
        KRB_PRIV *m;
{
    krb5_error_code errcode;
    m->pvno = KRB5_PVNO;
    m->msg_type = KRB5_PRIV;
    errcode = krb5_enc_data2EncryptedData (&k->enc_part, &m->enc_part);
    ERRCHECK();
    return 0;
}

krb5_error_code krb5_priv_enc_part2EncKrbPrivPart(k, m)
    const krb5_priv_enc_part *k;
        EncKrbPrivPart *m;
{
    krb5_error_code errcode;
    errcode = krb5_data2asn1_field (&k->user_data, &m->user_data);
    ERRCHECK();
    errcode = opt_krb5_timestamp2asn1_field(&k->timestamp, &m->timestamp);
    ERRCHECK();
    if (k->timestamp) {
	errcode = opt_krb5_int322int(&k->usec, &m->usec);
	ERRCHECK();
    } else {
	m->usec = 0;
    }
    errcode = opt_krb5_int322int(&k->seq_number, &m->seq_number);
    ERRCHECK();
    errcode = krb5_address2HostAddress (k->s_address, &m->s_address);
    ERRCHECK();
    errcode = opt_krb5_address2HostAddress(k->r_address, &m->r_address);
    ERRCHECK();
    return 0;
}

krb5_error_code krb5_error2KRB_ERROR(k, m)
    const krb5_error *k;
        KRB_ERROR *m;
{
    krb5_error_code errcode;
        m->pvno = KRB5_PVNO;
        m->msg_type = KRB5_ERROR;
        errcode = opt_krb5_timestamp2asn1_field(&k->ctime, &m->ctime);
	ERRCHECK();
        errcode = opt_krb5_int322int(&k->cusec, &m->cusec);
	ERRCHECK();
        errcode = krb5_timestamp2asn1_field (&k->stime, &m->stime);
	ERRCHECK();
        m->susec = k->susec;
        m->error_code = k->error;
        errcode = krb5_principal2OPrincipalORealm (&k->client, &m->cname, &m->crealm);
	ERRCHECK();
        errcode = krb5_principal2PrincipalRealm (&k->server, &m->sname, &m->realm);
	ERRCHECK();
        errcode = opt_krb5_data2asn1_field(&k->text, &m->e_text);
	ERRCHECK();
        errcode = opt_krb5_data2asn1_field(&k->e_data, &m->e_data);
	ERRCHECK();
    return 0;
}

krb5_error_code krb5_enc_tkt_part2EncTicketPart(k, m)
    const krb5_enc_tkt_part *k;
        EncTicketPart *m;
{
    krb5_error_code errcode;
    errcode = krb5_flags2asn1_field (&k->flags, &m->flags);
	ERRCHECK();
    errcode = krb5_keyblock2EncryptionKey (k->session, &m->key);
	ERRCHECK();
    errcode = krb5_principal2PrincipalRealm (&k->client, &m->cname, &m->crealm);
	ERRCHECK();
    errcode = krb5_transited2TransitedEncoding (&k->transited, &m->transited);
	ERRCHECK();
    errcode = krb5_timestamp2asn1_field (&k->times.authtime, &m->authtime);
	ERRCHECK();
    errcode = opt_krb5_timestamp2asn1_field(&k->times.starttime, &m->starttime);
	ERRCHECK();
    errcode = krb5_timestamp2asn1_field (&k->times.endtime, &m->endtime);
	ERRCHECK();
    errcode = opt_krb5_timestamp2asn1_field(&k->times.renew_till, &m->renew_till);
    ERRCHECK();
    /*
     * For DCE 1.2.2 we changed the caddr field to be OPTIONAL as defined
     * in RFC1510. However, to preserve pre-DCE 1.2.2 interoperability.
     * we will always encode the caddr field even if it is NULL.
     *
     * At some later release, the next 2 lines can be changed to:
     *     opt_krb5_addresses2HostAddresses(k->caddrs, &m->caddr);
     */
    CHECK_ASN_MALLOC1(m->caddr, HostAddresses);
    errcode = krb5_addresses2HostAddresses (k->caddrs, m->caddr);
    ERRCHECK();
    errcode = opt_krb5_authdata_vec2AuthorizationData(k->authorization_data, &m->authorization_data);
	ERRCHECK();
    return 0;
}

#ifdef OSF_DCE
krb5_error_code krb5_time_and_key2TimeAndKey(k, m)
    const krb5_time_and_key *k;
        TimeAndKey *m;
{
    krb5_error_code errcode;
    errcode = krb5_timestamp2asn1_field(&k->timestamp, &m->time);
	ERRCHECK();
    errcode = krb5_keyblock2EncryptionKey(&k->key, &m->key);
	ERRCHECK();
    return 0;
}

krb5_error_code krb5_pa_third_party2PAThirdParty(k, m)
    const krb5_pa_third_party *k;
    PAThirdParty *m;
{
    krb5_error_code errcode;
    errcode = krb5_data2asn1_field (k->machine_tgt_data, &m->machine_tgt_data);
	ERRCHECK();
    errcode = krb5_data2asn1_field (k->single_ebuf_data, &m->single_ebuf_data);
	ERRCHECK();
    errcode = krb5_data2asn1_field (k->double_ebuf_data, &m->double_ebuf_data);
	ERRCHECK();
    return 0;
}

krb5_error_code krb5_sub_pa_data2SubPaData(k, m)
    const krb5_sub_pa_data *k;
        SubPaData *m;
{
    krb5_error_code errcode;
    m->sub_type = k->pa_subtype;
    errcode = krb5_octets2asn1_field (&k->length, &k->contents, &m->sub_data);
	ERRCHECK();
    return 0;
}

krb5_error_code krb5_sub_err_data2SubErrData(k, m)
    const krb5_sub_err_data *k;
        SubErrData *m;
{
    krb5_error_code errcode;
        m->sub_type = k->err_subtype;
        errcode = krb5_octets2asn1_field (&k->length, &k->contents, &m->sub_data);
	ERRCHECK();
    return 0;
}

krb5_error_code krb5_dce_pa_keys2DcePAKeys(k, m)
    const krb5_dce_pa_keys *k;
	DcePAKeys *m;
{
    krb5_error_code errcode;
    errcode = krb5_data2asn1_field (&k->user_key, &m->user_key);
	ERRCHECK();
    errcode = krb5_keyblock2EncryptionKey (&k->reply_key, &m->reply_key);
	ERRCHECK();
    return 0;
}

krb5_error_code krb5_dce_pa_err_info2DcePAErrInfo(k, m)
    const krb5_dce_pa_err_info *k;
	DcePAErrInfo *m;
{
    krb5_error_code errcode;
    errcode = opt_krb5_int322int(&k->err, &m->err);
	ERRCHECK();
    errcode = opt_krb5_int322int(&k->type, &m->type);
	ERRCHECK();
    errcode = opt_krb5_int322int(&k->subtype, &m->subtype);
	ERRCHECK();
    errcode = opt_krb5_data2asn1_field(&k->salt, &m->salt);
	ERRCHECK();
    return 0;
}
#endif /* OSF_DCE */
krb5_error_code PA_DATA_SEQUENCE_OF_SEQUENCE2krb5_pa_data(m, k)
    const PA_DATA_SEQUENCE_OF_SEQUENCE *m;
        krb5_pa_data *k;
{
    krb5_error_code errcode;
    k->pa_type = m->padata_type;
    errcode = asn1_field2krb5_octets (&m->pa_data, &k->length, &k->contents);
	ERRCHECK();
    return 0;
}

krb5_error_code HostAddresses_SEQUENCE_OF_SEQUENCE2krb5_address(m, k)
    const HostAddresses_SEQUENCE_OF_SEQUENCE *m;
        krb5_address *k;
{
    krb5_error_code errcode;
    k->addrtype = m->addr_type;
    errcode = asn1_field2krb5_octets (&m->address, &k->length, &k->contents);
	ERRCHECK();
    return 0;
}

krb5_error_code HostAddress2krb5_address(m, k)
    const HostAddress *m;
        krb5_address *k;
{
    krb5_error_code errcode;
    k->addrtype = m->addr_type;
    errcode = asn1_field2krb5_octets (&m->address, &k->length, &k->contents);
	ERRCHECK();
    return 0;
}

krb5_error_code AuthorizationData_SEQUENCE_OF_SEQUENCE2krb5_authdata(m, k)
    const AuthorizationData_SEQUENCE_OF_SEQUENCE *m;
    krb5_authdata *k;
{
    krb5_error_code errcode;
    k->ad_type = m->ad_type;
    errcode = asn1_field2krb5_octets (&m->ad_data, &k->length, &k->contents);
	ERRCHECK();
    return 0;
}

krb5_error_code LastReq_SEQUENCE_OF_SEQUENCE2krb5_last_req_entry(m, k)
    const LastReq_SEQUENCE_OF_SEQUENCE *m;
    krb5_last_req_entry *k;
{
    krb5_error_code errcode;
    k->lr_type = m->lr_type;
    errcode = asn1_field2krb5_timestamp (&m->lr_value, &k->value);
	ERRCHECK();
    return 0;
}

krb5_error_code EncryptedData2krb5_enc_data(m, k)
    const EncryptedData *m;
        krb5_enc_data *k;
{
    krb5_error_code errcode;
    k->etype = m->etype;
    errcode = opt_int2krb5_kvno(m->kvno, &k->kvno);
	ERRCHECK();
    errcode = asn1_field2krb5_data (&m->cipher, &k->ciphertext);
	ERRCHECK();
    return 0;
}

krb5_error_code TransitedEncoding2krb5_transited(m, k)
    const TransitedEncoding *m;
    krb5_transited *k;
{
    krb5_error_code errcode;
    k->tr_type = m->tr_type;
    errcode = asn1_field2krb5_data (&m->contents, &k->tr_contents);
	ERRCHECK();
    return 0;
}

krb5_error_code Authenticator2krb5_authenticator(m, k)
    const Authenticator *m;
        krb5_authenticator *k;
{
    krb5_error_code errcode;
    if (m->authenticator_vno != KRB5_PVNO) 
	printf("Warning: unexpected constant %d should be %d\n", m->authenticator_vno, KRB5_PVNO);
    errcode = PrincipalRealm2krb5_principal (&m->cname, &m->crealm, &k->client);
	ERRCHECK();
    errcode = opt_Checksum2krb5_checksum(m->cksum, &k->checksum);
	ERRCHECK();
    k->cusec = m->cusec;
    errcode = asn1_field2krb5_timestamp (&m->ctime, &k->ctime);
	ERRCHECK();
    errcode = opt_EncryptionKey2krb5_keyblock(m->subkey, &k->subkey);
	ERRCHECK();
    errcode = opt_int2krb5_int32(m->seq_number, &k->seq_number);
	ERRCHECK();
    return 0;
}

krb5_error_code EncryptionKey2krb5_keyblock(m, k)
    const EncryptionKey *m;
    krb5_keyblock *k;
{
    krb5_error_code errcode;
    k->keytype = m->keytype;
    errcode = asn1_field2krb5_octets (&m->keyvalue, &k->length, &k->contents);
	ERRCHECK();
    return 0;
}

krb5_error_code Checksum2krb5_checksum(m, k)
    const Checksum *m;
    krb5_checksum *k;
{
    krb5_error_code errcode;
    k->checksum_type = m->cksumtype;
    errcode = asn1_field2krb5_octets (&m->checksum, &k->length, &k->contents);
	ERRCHECK();
    return 0;
}

krb5_error_code KDC_REQ_BODY2krb5_kdc_req(m, k)
    const KDC_REQ_BODY *m;
        krb5_kdc_req *k;
{
    krb5_error_code errcode;
    errcode = asn1_field2krb5_flags (&m->kdc_options, &k->kdc_options);
	ERRCHECK();
    errcode = OPrincipalRealm2krb5_principal (&m->cname, &m->realm, &k->client);
	ERRCHECK();
    errcode = PrincipalRealm2krb5_principal (&m->sname, &m->realm, &k->server);
	ERRCHECK();
    errcode = opt_asn1_field2krb5_timestamp(m->from, &k->from);
	ERRCHECK();
    errcode = asn1_field2krb5_timestamp (&m->till, &k->till);
	ERRCHECK();
    errcode = opt_asn1_field2krb5_timestamp(m->rtime, &k->rtime);
	ERRCHECK();
    k->nonce = m->nonce;
    errcode = INTSEQ2krb5_enctype_vec (&m->etype, &k->etype, &k->netypes);
	ERRCHECK();
    errcode = opt_HostAddresses2krb5_addresses(m->addresses, &k->addresses);
	ERRCHECK();
    errcode = opt_EncryptedData2krb5_enc_data(m->enc_authorization_data, &k->authorization_data);
	ERRCHECK();
    k->unenc_authdata = 0;
    errcode = opt_KDC_REQ_BODY_SEQUENCE_additional_tickets2krb5_ticket_vec(m->additional_tickets, &k->second_ticket);
	ERRCHECK();
    return 0;
}

krb5_error_code TGS_REQ2krb5_kdc_req(m, k)
    const TGS_REQ *m;
    krb5_kdc_req *k;
{
    krb5_error_code errcode;
    if (m->pvno != KRB5_PVNO) 
	printf("Warning: unexpected constant %d should be %d\n", m->pvno, KRB5_PVNO);
    k->msg_type = m->msg_type;
    errcode = opt_PA_DATA2krb5_pa_data_vec(m->padata, &k->padata);
	ERRCHECK();
    errcode = asn1_field2krb5_flags (&m->req_body.kdc_options, &k->kdc_options);
	ERRCHECK();
    errcode = OPrincipalRealm2krb5_principal (&m->req_body.cname, &m->req_body.realm, &k->client);
	ERRCHECK();
    errcode = PrincipalRealm2krb5_principal (&m->req_body.sname, &m->req_body.realm, &k->server);
	ERRCHECK();
    errcode = opt_asn1_field2krb5_timestamp(m->req_body.from, &k->from);
	ERRCHECK();
    errcode = asn1_field2krb5_timestamp (&m->req_body.till, &k->till);
	ERRCHECK();
    errcode = opt_asn1_field2krb5_timestamp(m->req_body.rtime, &k->rtime);
	ERRCHECK();
    k->nonce = m->req_body.nonce;
    errcode = INTSEQ2krb5_enctype_vec (&m->req_body.etype, &k->etype, &k->netypes);
	ERRCHECK();
    errcode = opt_HostAddresses2krb5_addresses(m->req_body.addresses, &k->addresses);
	ERRCHECK();
    errcode = opt_EncryptedData2krb5_enc_data(m->req_body.enc_authorization_data, &k->authorization_data);
	ERRCHECK();
    k->unenc_authdata = 0;
    errcode = opt_KDC_REQ_BODY_SEQUENCE_additional_tickets2krb5_ticket_vec(m->req_body.additional_tickets, &k->second_ticket);
	ERRCHECK();
    return 0;
}

krb5_error_code AS_REQ2krb5_kdc_req(m, k)
    const AS_REQ *m;
    krb5_kdc_req *k;
{
    krb5_error_code errcode;
    if (m->pvno != KRB5_PVNO) 
	printf("Warning: unexpected constant %d should be %d\n", m->pvno, KRB5_PVNO);
    k->msg_type = m->msg_type;
    errcode = opt_PA_DATA2krb5_pa_data_vec(m->padata, &k->padata);
	ERRCHECK();
    errcode = asn1_field2krb5_flags (&m->req_body.kdc_options, &k->kdc_options);
	ERRCHECK();
    errcode = OPrincipalRealm2krb5_principal (&m->req_body.cname, &m->req_body.realm, &k->client);
	ERRCHECK();
    errcode = PrincipalRealm2krb5_principal (&m->req_body.sname, &m->req_body.realm, &k->server);
	ERRCHECK();
    errcode = opt_asn1_field2krb5_timestamp(m->req_body.from, &k->from);
	ERRCHECK();
    errcode = asn1_field2krb5_timestamp (&m->req_body.till, &k->till);
	ERRCHECK();
    errcode = opt_asn1_field2krb5_timestamp(m->req_body.rtime, &k->rtime);
	ERRCHECK();
    k->nonce = m->req_body.nonce;
    errcode = INTSEQ2krb5_enctype_vec (&m->req_body.etype, &k->etype, &k->netypes);
	ERRCHECK();
    errcode = opt_HostAddresses2krb5_addresses(m->req_body.addresses, &k->addresses);
	ERRCHECK();
    errcode = opt_EncryptedData2krb5_enc_data(m->req_body.enc_authorization_data, &k->authorization_data);
	ERRCHECK();
    k->unenc_authdata = 0;
    errcode = opt_KDC_REQ_BODY_SEQUENCE_additional_tickets2krb5_ticket_vec(m->req_body.additional_tickets, &k->second_ticket);
	ERRCHECK();
    return 0;
}

krb5_error_code Ticket2krb5_ticket(m, k)
    const Ticket *m;
        krb5_ticket *k;
{
    krb5_error_code errcode;
    if (m->tkt_vno != KRB5_PVNO) 
	printf("Warning: unexpected constant %d should be %d\n", m->tkt_vno, KRB5_PVNO);
    errcode = PrincipalRealm2krb5_principal (&m->sname, &m->realm, &k->server);
	ERRCHECK();
    errcode = EncryptedData2krb5_enc_data (&m->enc_part, &k->enc_part);
	ERRCHECK();
    k->enc_part2 = 0;
    return 0;
}

krb5_error_code TGS_REP2krb5_kdc_rep(m, k)
    const TGS_REP *m;
        krb5_kdc_rep *k;
{
    krb5_error_code errcode;
    if (m->pvno != KRB5_PVNO) 
	printf("Warning: unexpected constant %d should be %d\n", m->pvno, KRB5_PVNO);
    k->msg_type = m->msg_type;
    errcode = opt_PA_DATA2krb5_pa_data_vec(m->padata, &k->padata);
	ERRCHECK();
    errcode = PrincipalRealm2krb5_principal (&m->cname, &m->crealm, &k->client);
	ERRCHECK();
    errcode = opt_Ticket2krb5_ticket(&m->ticket, &k->ticket);
	ERRCHECK();
    errcode = EncryptedData2krb5_enc_data (&m->enc_part, &k->enc_part);
	ERRCHECK();
    k->enc_part2 = 0;
    return 0;
}

krb5_error_code EncASRepPart2krb5_enc_kdc_rep_part(m, k)
    const EncASRepPart *m;
        krb5_enc_kdc_rep_part *k;
{
    krb5_error_code errcode;
    errcode = opt_EncryptionKey2krb5_keyblock(&m->key, &k->session);
	ERRCHECK();
    errcode = opt_LastReq2krb5_last_req_entries(&m->last_req, &k->last_req);
	ERRCHECK();
    k->nonce = m->nonce;
    errcode = opt_asn1_field2krb5_timestamp(m->key_expiration, &k->key_exp);
	ERRCHECK();
    errcode = asn1_field2krb5_flags (&m->flags, &k->flags);
	ERRCHECK();
    errcode = asn1_field2krb5_timestamp (&m->authtime, &k->times.authtime);
	ERRCHECK();
    errcode = opt_asn1_field2krb5_timestamp(m->starttime, &k->times.starttime);
	ERRCHECK();
    errcode = asn1_field2krb5_timestamp (&m->endtime, &k->times.endtime);
	ERRCHECK();
    errcode = opt_asn1_field2krb5_timestamp(m->renew_till, &k->times.renew_till);
	ERRCHECK();
    errcode = PrincipalRealm2krb5_principal (&m->sname, &m->srealm, &k->server);
	ERRCHECK();
    errcode = opt_HostAddresses2krb5_addresses(m->caddr, &k->caddrs);
	ERRCHECK();
    return 0;
}

krb5_error_code EncTGSRepPart2krb5_enc_kdc_rep_part(m, k)
    const EncTGSRepPart *m;
        krb5_enc_kdc_rep_part *k;
{
    krb5_error_code errcode;
    errcode = opt_EncryptionKey2krb5_keyblock(&m->key, &k->session);
	ERRCHECK();
    errcode = opt_LastReq2krb5_last_req_entries(&m->last_req, &k->last_req);
	ERRCHECK();
    k->nonce = m->nonce;
    errcode = opt_asn1_field2krb5_timestamp(m->key_expiration, &k->key_exp);
	ERRCHECK();
    errcode = asn1_field2krb5_flags (&m->flags, &k->flags);
	ERRCHECK();
    errcode = asn1_field2krb5_timestamp (&m->authtime, &k->times.authtime);
	ERRCHECK();
    errcode = opt_asn1_field2krb5_timestamp(m->starttime, &k->times.starttime);
	ERRCHECK();
    errcode = asn1_field2krb5_timestamp (&m->endtime, &k->times.endtime);
	ERRCHECK();
    errcode = opt_asn1_field2krb5_timestamp(m->renew_till, &k->times.renew_till);
	ERRCHECK();
    errcode = PrincipalRealm2krb5_principal (&m->sname, &m->srealm, &k->server);
	ERRCHECK();
    errcode = opt_HostAddresses2krb5_addresses(m->caddr, &k->caddrs);
	ERRCHECK();
    return 0;
}

krb5_error_code AP_REQ2krb5_ap_req(m, k)
    const AP_REQ *m;
        krb5_ap_req *k;
{
    krb5_error_code errcode;
    if (m->pvno != KRB5_PVNO) 
	printf("Warning: unexpected constant %d should be %d\n", m->pvno, KRB5_PVNO);
    if (m->msg_type != KRB5_AP_REQ) 
	printf("Warning: unexpected constant %d should be %d\n", m->msg_type, KRB5_AP_REQ);
    errcode = asn1_field2krb5_flags (&m->ap_options, &k->ap_options);
	ERRCHECK();
    errcode = opt_Ticket2krb5_ticket(&m->ticket, &k->ticket);
	ERRCHECK();
    errcode = EncryptedData2krb5_enc_data (&m->authenticator, &k->authenticator);
	ERRCHECK();
    return 0;
}

krb5_error_code AP_REP2krb5_ap_rep(m, k)
    const AP_REP *m;
        krb5_ap_rep *k;
{
    krb5_error_code errcode;
    if (m->pvno != KRB5_PVNO) 
	printf("Warning: unexpected constant %d should be %d\n", m->pvno, KRB5_PVNO);
    if (m->msg_type != KRB5_AP_REP) 
	printf("Warning: unexpected constant %d should be %d\n", m->msg_type, KRB5_AP_REP);
    errcode = EncryptedData2krb5_enc_data (&m->enc_part, &k->enc_part);
	ERRCHECK();
    return 0;
}

krb5_error_code EncAPRepPart2krb5_ap_rep_enc_part(m, k)
    const EncAPRepPart *m;
        krb5_ap_rep_enc_part *k;
{
    krb5_error_code errcode;
    errcode = asn1_field2krb5_timestamp (&m->ctime, &k->ctime);
	ERRCHECK();
    k->cusec = m->cusec;
    errcode = opt_EncryptionKey2krb5_keyblock(m->subkey, &k->subkey);
	ERRCHECK();
    errcode = opt_int2krb5_int32(m->seq_number, &k->seq_number);
	ERRCHECK();
    return 0;
}

krb5_error_code KRB_SAFE2krb5_safe(m, k)
    const KRB_SAFE *m;
        krb5_safe *k;
{
    krb5_error_code errcode;
    if (m->pvno != KRB5_PVNO) 
	printf("Warning: unexpected constant %d should be %d\n", m->pvno, KRB5_PVNO);
    if (m->msg_type != KRB5_SAFE) 
	printf("Warning: unexpected constant %d should be %d\n", m->msg_type, KRB5_SAFE);
    errcode = asn1_field2krb5_data (&m->safe_body.user_data, &k->user_data);
	ERRCHECK();
    errcode = opt_asn1_field2krb5_timestamp(m->safe_body.timestamp, &k->timestamp);
	ERRCHECK();
    errcode = opt_int2krb5_int32(m->safe_body.usec, &k->usec);
	ERRCHECK();
    errcode = opt_int2krb5_int32(m->safe_body.seq_number, &k->seq_number);
	ERRCHECK();
    errcode = opt_HostAddress2krb5_address(&m->safe_body.s_address, &k->s_address);
	ERRCHECK();
    errcode = opt_HostAddress2krb5_address(m->safe_body.r_address, &k->r_address);
	ERRCHECK();
    errcode = opt_Checksum2krb5_checksum(&m->cksum, &k->checksum);
	ERRCHECK();
    return 0;
}

krb5_error_code KRB_PRIV2krb5_priv(m, k)
    const KRB_PRIV *m;
        krb5_priv *k;
{
    krb5_error_code errcode;
    if (m->pvno != KRB5_PVNO) 
	printf("Warning: unexpected constant %d should be %d\n", m->pvno, KRB5_PVNO);
    if (m->msg_type != KRB5_PRIV) 
	printf("Warning: unexpected constant %d should be %d\n", m->msg_type, KRB5_PRIV);
    errcode = EncryptedData2krb5_enc_data (&m->enc_part, &k->enc_part);
	ERRCHECK();
    return 0;
}

krb5_error_code EncKrbPrivPart2krb5_priv_enc_part(m, k)
    const EncKrbPrivPart *m;
    krb5_priv_enc_part *k;
{
    krb5_error_code errcode;
    errcode = asn1_field2krb5_data (&m->user_data, &k->user_data);
	ERRCHECK();
    errcode = opt_asn1_field2krb5_timestamp(m->timestamp, &k->timestamp);
	ERRCHECK();
    errcode = opt_int2krb5_int32(m->usec, &k->usec);
	ERRCHECK();
    errcode = opt_int2krb5_int32(m->seq_number, &k->seq_number);
	ERRCHECK();
    errcode = opt_HostAddress2krb5_address(&m->s_address, &k->s_address);
	ERRCHECK();
    errcode = opt_HostAddress2krb5_address(m->r_address, &k->r_address);
	ERRCHECK();
    return 0;
}

krb5_error_code KRB_ERROR2krb5_error(m, k)
    const KRB_ERROR *m;
    krb5_error *k;
{
    krb5_error_code errcode;
    if (m->pvno != KRB5_PVNO) 
	printf("Warning: unexpected constant %d should be %d\n", m->pvno, KRB5_PVNO);
    if (m->msg_type != KRB5_ERROR) 
	printf("Warning: unexpected constant %d should be %d\n", m->msg_type, KRB5_ERROR);
    errcode = opt_asn1_field2krb5_timestamp(m->ctime, &k->ctime);
	ERRCHECK();
    errcode = opt_int2krb5_int32(m->cusec, &k->cusec);
	ERRCHECK();
    errcode = asn1_field2krb5_timestamp (&m->stime, &k->stime);
	ERRCHECK();
    k->susec = m->susec;
    k->error = m->error_code;
    errcode = OPrincipalORealm2krb5_principal (&m->cname, &m->crealm, &k->client);
	ERRCHECK();
    errcode = PrincipalRealm2krb5_principal (&m->sname, &m->realm, &k->server);
	ERRCHECK();
    errcode = opt_asn1_field2krb5_data(m->e_text, &k->text);
	ERRCHECK();
    errcode = opt_asn1_field2krb5_data(m->e_data, &k->e_data);
	ERRCHECK();
    return 0;
}

krb5_error_code EncTicketPart2krb5_enc_tkt_part(m, k)
    const EncTicketPart *m;
        krb5_enc_tkt_part *k;
{
    krb5_error_code errcode;
    errcode = asn1_field2krb5_flags (&m->flags, &k->flags);
	ERRCHECK();
    errcode = opt_EncryptionKey2krb5_keyblock(&m->key, &k->session);
	ERRCHECK();
    errcode = PrincipalRealm2krb5_principal (&m->cname, &m->crealm, &k->client);
	ERRCHECK();
    errcode = TransitedEncoding2krb5_transited (&m->transited, &k->transited);
	ERRCHECK();
    errcode = asn1_field2krb5_timestamp (&m->authtime, &k->times.authtime);
	ERRCHECK();
    errcode = opt_asn1_field2krb5_timestamp(m->starttime, &k->times.starttime);
	ERRCHECK();
    errcode = asn1_field2krb5_timestamp (&m->endtime, &k->times.endtime);
	ERRCHECK();
    errcode = opt_asn1_field2krb5_timestamp(m->renew_till, &k->times.renew_till);
	ERRCHECK();
    errcode = opt_HostAddresses2krb5_addresses(m->caddr, &k->caddrs);
	ERRCHECK();
    errcode = opt_AuthorizationData2krb5_authdata_vec(m->authorization_data, &k->authorization_data);
	ERRCHECK();
    return 0;
}

krb5_error_code TimeAndKey2krb5_time_and_key(m, k)
    const TimeAndKey *m;
        krb5_time_and_key *k;
{
    krb5_error_code errcode;
    errcode = asn1_field2krb5_timestamp(&m->time, &k->timestamp);
	ERRCHECK();
    errcode = EncryptionKey2krb5_keyblock(&m->key, &k->key);
	ERRCHECK();
    return 0;
}

krb5_error_code opt_asn1_field2krb5_data_ptr(m, k)
    const asn1_field *m;
    krb5_data **k;
{
    krb5_error_code errcode;
    if (m == 0)
	*k = NULL;
    else
    {
	krb5_data *ret;
	CHECK_MALLOC1(ret, krb5_data);
	errcode = asn1_field2krb5_data(m, ret);
	ERRCHECK();
	*k = ret;
    }
    return 0;
}

krb5_error_code PAThirdParty2krb5_pa_third_party(m, k)
    const PAThirdParty *m;
        krb5_pa_third_party *k;
{
    krb5_error_code errcode;
    errcode = opt_asn1_field2krb5_data_ptr(&m->machine_tgt_data, &k->machine_tgt_data);
	ERRCHECK();
    errcode = opt_asn1_field2krb5_data_ptr(&m->single_ebuf_data, &k->single_ebuf_data);
	ERRCHECK();
    errcode = opt_asn1_field2krb5_data_ptr(&m->double_ebuf_data, &k->double_ebuf_data);
	ERRCHECK();
    return 0;
}

krb5_error_code SubPaData2krb5_sub_pa_data(m, k)
    const SubPaData *m;
        krb5_sub_pa_data *k;
{
    krb5_error_code errcode;
    k->pa_subtype = m->sub_type;
    errcode = asn1_field2krb5_octets (&m->sub_data, &k->length, &k->contents);
	ERRCHECK();
    return 0;
}

krb5_error_code SubErrData2krb5_sub_err_data(m, k)
    const SubErrData *m;
    krb5_sub_err_data *k;
{
    krb5_error_code errcode;
    k->err_subtype = m->sub_type;
    errcode = asn1_field2krb5_octets (&m->sub_data, &k->length, &k->contents);
	ERRCHECK();
    return 0;
}

krb5_error_code DcePAKeys2krb5_dce_pa_keys(m, k)
    const DcePAKeys *m;
    krb5_dce_pa_keys *k;
{
    krb5_error_code errcode;
    errcode = asn1_field2krb5_data (&m->user_key, &k->user_key);
	ERRCHECK();
    errcode = EncryptionKey2krb5_keyblock(&m->reply_key, &k->reply_key);
	ERRCHECK();
    return 0;
}

krb5_error_code DcePAErrInfo2krb5_dce_pa_err_info(m, k)
    const DcePAErrInfo *m;
	krb5_dce_pa_err_info *k;
{
    krb5_error_code errcode;
    errcode = opt_int2krb5_int32(m->err, &k->err);
	ERRCHECK();
    errcode = opt_int2krb5_int32(m->type, &k->type);
	ERRCHECK();
    errcode = opt_int2krb5_int32(m->subtype, &k->subtype);
	ERRCHECK();
    errcode = opt_asn1_field2krb5_data(m->salt, &k->salt);
	ERRCHECK();
    return 0;
}

/************************************
 * Start of public key login
 * krb5->ASN.1 translation functions
 ************************************/

krb5_error_code krb5_enc_data2Signature
    (const krb5_enc_data *k, 
     Signature *m)
{
    krb5_error_code errcode;
    m->sigType = k->etype;
    errcode = opt_krb5_kvno2int(&k->kvno, &m->kvno);
	ERRCHECK();
    errcode = krb5_data2asn1_field (&k->ciphertext, &m->sigHash);
	ERRCHECK();
    return 0;
}

krb5_error_code krb5_pk_authenticator2PKAuthenticator(k, m)
    const krb5_pk_authenticator *k;
    PKAuthenticator       *m;
{
    krb5_error_code errcode;
    errcode = opt_krb5_checksum2Checksum(k->cksum, &m->cksum);
    ERRCHECK();
    m->cusec=k->cusec;
    errcode = krb5_timestamp2asn1_field(&k->ctime, &m->ctime);
    ERRCHECK();
    m->nonce=k->nonce;
    errcode = krb5_principal2PrincipalRealm(&k->kdcname, &m->kdcName, &m->kdcRealm);
    ERRCHECK();
    return 0;
}

krb5_error_code krb5_signed_pk_authenticator2SignedPKAuthenticator(k, m)
    const krb5_signed_pk_authenticator *k;
    SignedPKAuthenticator        *m;
{
    krb5_error_code errcode;
    errcode = krb5_pk_authenticator2PKAuthenticator(&k->pk_authenticator, &m->authent);
	ERRCHECK();
    errcode = krb5_enc_data2Signature(&k->authentsig, &m->authentSig);
	ERRCHECK();
    return 0;
}

krb5_error_code krb5_pa_pk_as_root2PA_PK_AS_ROOT(k, m)
    const krb5_pa_pk_as_root *k;
    PA_PK_AS_ROOT      *m;
{
    krb5_error_code errcode;
    m->rootCert.n = 0;
    m->rootCert.v = NULL;
    errcode = krb5_signed_pk_authenticator2SignedPKAuthenticator(&k->authent, &m->signedAuth);
	ERRCHECK();
    return 0;
}

krb5_error_code krb5_enc_pa_pk_as_rep_part2EncPaPkAsRepPart(k, m)
    const krb5_enc_pa_pk_as_rep_part *k;
    EncPaPkAsRepPart           *m;
{
    krb5_error_code errcode;
    errcode = krb5_keyblock2EncryptionKey(&k->enc_reply_key, &m->encReplyKey);
	ERRCHECK();
    m->nonce = k->nonce;
    return 0;
}

krb5_error_code krb5_keyblock2EncPaPkAsRepTmpKey (k, m)
    const krb5_keyblock *k;
    EncPaPkAsRepTmpKey *m;
{
    krb5_error_code errcode;
    errcode = krb5_keyblock2EncryptionKey(k, &m->encReplyTmpKey);
	ERRCHECK();
    return 0;
}

krb5_error_code krb5_enc_pa_pk_as_rep_part_shell2EncPaPkAsRepPartShell(k, m)
    const krb5_enc_pa_pk_as_rep_part_shell *k;
    EncPaPkAsRepPartShell      *m;
{
    krb5_error_code errcode;
    errcode = krb5_enc_pa_pk_as_rep_part2EncPaPkAsRepPart(&k->reply, &m->encReplyPart);
	ERRCHECK();
    errcode = krb5_enc_data2Signature(&k->replysig, &m->encReplyPartSig);
	ERRCHECK();
    return 0;
}

krb5_error_code krb5_pa_pk_as_rep2PA_PK_AS_REP(k, m)
    const krb5_pa_pk_as_rep *k;
    PA_PK_AS_REP      *m;
{
    krb5_error_code errcode;
    m->kdcCert.n = 0;
    m->kdcCert.v = NULL;    
    errcode = krb5_enc_data2EncryptedData(&k->enc_shell, &m->encryptShell);
	ERRCHECK();
    errcode = krb5_enc_data2EncryptedData(&k->enc_tmp_key, &m->encryptKey);
	ERRCHECK();    
    return 0;
}



/************************************
 * Start of public key login
 * ASN1->krb5 translation functions
 ************************************/

krb5_error_code Signature2krb5_enc_data (const Signature *m, krb5_enc_data *k)
{
    krb5_error_code errcode;
    k->etype = m->sigType;
    errcode = opt_int2krb5_kvno(m->kvno, &k->kvno);
	ERRCHECK();
    errcode = asn1_field2krb5_data (&m->sigHash, &k->ciphertext);
	ERRCHECK();
    return 0;
}


krb5_error_code PKAuthenticator2krb5_pk_authenticator(m, k)
    const PKAuthenticator *m;
    krb5_pk_authenticator *k;
{
    krb5_error_code errcode;
    errcode = opt_Checksum2krb5_checksum(m->cksum, &k->cksum);
	ERRCHECK();
    k->cusec=m->cusec;
    errcode = asn1_field2krb5_timestamp(&m->ctime, &k->ctime);
	ERRCHECK();
    k->nonce = m->nonce;
    errcode = PrincipalRealm2krb5_principal(&m->kdcName, &m->kdcRealm, &k->kdcname);
	ERRCHECK();
    return 0;
}

krb5_error_code SignedPKAuthenticator2krb5_signed_pk_authenticator(m, k)
    const SignedPKAuthenticator        *m;
    krb5_signed_pk_authenticator *k;
{
    krb5_error_code errcode;
    errcode = PKAuthenticator2krb5_pk_authenticator(&m->authent, &k->pk_authenticator);
	ERRCHECK();
    errcode = Signature2krb5_enc_data(&m->authentSig, &k->authentsig);
	ERRCHECK();
    return 0;
}

krb5_error_code PA_PK_AS_ROOT2krb5_pa_pk_as_root(m, k)
    const PA_PK_AS_ROOT      *m;
    krb5_pa_pk_as_root *k;
{
    krb5_error_code errcode;
    k->root_cert.data = NULL;
    k->root_cert.length = 0;
    errcode = SignedPKAuthenticator2krb5_signed_pk_authenticator(&m->signedAuth, &k->authent);
	ERRCHECK();
    return 0;
}

krb5_error_code EncPaPkAsRepPart2krb5_enc_pa_pk_as_rep_part(m, k)
    const EncPaPkAsRepPart           *m;
    krb5_enc_pa_pk_as_rep_part *k;
{
    krb5_error_code errcode;
    errcode = EncryptionKey2krb5_keyblock(&m->encReplyKey, &k->enc_reply_key);
	ERRCHECK();
    k->nonce = m->nonce;
    return 0;
}

krb5_error_code EncPaPkAsRepPartShell2krb5_enc_pa_pk_as_rep_part_shell(m, k)
    const EncPaPkAsRepPartShell *m;
    krb5_enc_pa_pk_as_rep_part_shell *k;
{
    krb5_error_code errcode;
    errcode = EncPaPkAsRepPart2krb5_enc_pa_pk_as_rep_part(&m->encReplyPart, &k->reply);
	ERRCHECK();
    errcode = Signature2krb5_enc_data(&m->encReplyPartSig, &k->replysig);
	ERRCHECK();
    return 0;
}

krb5_error_code EncPaPkAsRepTmpKey2krb5_keyblock(m, k)
    const EncPaPkAsRepTmpKey *m;
    krb5_keyblock *k;
{
    krb5_error_code errcode;
    errcode = EncryptionKey2krb5_keyblock(&m->encReplyTmpKey, k);
	ERRCHECK();
    return 0;
}

krb5_error_code PA_PK_AS_REP2krb5_pa_pk_as_rep(m, k)
    const PA_PK_AS_REP      *m;
    krb5_pa_pk_as_rep *k;
{
    krb5_error_code errcode;
    k->kdc_cert.data=NULL;
    k->kdc_cert.length=0;
    errcode = EncryptedData2krb5_enc_data(&m->encryptShell, &k->enc_shell);
	ERRCHECK();
    errcode = EncryptedData2krb5_enc_data(&m->encryptKey, &k->enc_tmp_key);
	ERRCHECK();    
    return 0;
}

/************************************
 * End of public key login
 * translation functions
 ************************************/

krb5_error_code krb5_etype_info_entry2ETYPE_INFO_ENTRY(k, m)
    const krb5_etype_info_entry *k;
    ETYPE_INFO_ENTRY *m;
{
    krb5_error_code errcode;
    m->etype = k->etype;
    errcode = opt_krb5_octets2asn1_field(&k->length, &k->salt, &m->salt);
	ERRCHECK();
    return 0;
}

krb5_error_code ETYPE_INFO_ENTRY2krb5_etype_info_entry(m, k)
    const ETYPE_INFO_ENTRY *m;
    krb5_etype_info_entry *k;    
{
    krb5_error_code errcode;
    k->etype = m->etype;
    errcode = opt_asn1_field2krb5_octets(m->salt, &k->length, &k->salt);
	ERRCHECK();
    return 0;
}

krb5_error_code krb5_etype_info_entry_vec2ETYPE_INFO (k, m)
    const krb5_etype_info_entry_vec *k;
    ETYPE_INFO *m;
{
    krb5_error_code errcode;
    int n=0, size;
    if (k != NULL) for (; k[n] != 0; n++) ;
    size = n;
    m->n = size;
    CHECK_ASN_MALLOC(m->v, ETYPE_INFO_ENTRY, size);
    for (n=0; n < size; n++) {
	errcode = krb5_etype_info_entry2ETYPE_INFO_ENTRY(k[n], &m->v[n]);
	ERRCHECK();
    }
    return 0;
}

krb5_error_code ETYPE_INFO2krb5_etype_info_entry_vec (m, k)
    const ETYPE_INFO *m;
    krb5_etype_info_entry_vec **k;
{
    krb5_error_code errcode;
    int n, size;
    size = m->n;
    CHECK_MALLOC(*k, krb5_etype_info_entry *, size+1);
    for (n=0; n < size; n++) {
	CHECK_MALLOC1((*k)[n], krb5_etype_info_entry);
	errcode = ETYPE_INFO_ENTRY2krb5_etype_info_entry(&m->v[n], (*k)[n]);
	ERRCHECK();
    }
    (*k)[n] = 0;
    return 0;
}

krb5_error_code PA_SAM_CHALLENGE2krb5_sam_challenge (const PA_SAM_CHALLENGE *m, krb5_sam_challenge *k)
{
    abort();
}
krb5_error_code krb5_sam_challenge2PA_SAM_CHALLENGE (const krb5_sam_challenge *k, PA_SAM_CHALLENGE *m)
{
    abort();
}
krb5_error_code EncryptionKey2krb5_predicted_sam_response(const EncryptionKey *m, krb5_predicted_sam_response *k)
{
    abort();
}
krb5_error_code krb5_predicted_sam_response2EncryptionKey(const krb5_predicted_sam_response *k, EncryptionKey *m)
{
    abort ();
}


krb5_error_code PA_ENC_TS_ENC2krb5_pa_enc_ts(const PA_ENC_TS_ENC *m, krb5_pa_enc_ts *k)
{
    krb5_error_code errcode;
    errcode = asn1_field2krb5_timestamp (&m->patimestamp, &k->patimestamp);
        ERRCHECK();
    errcode = opt_int2krb5_int32(m->pausec, &k->pausec);
        ERRCHECK();
    return 0;
}

krb5_error_code krb5_pa_enc_ts2PA_ENC_TS_ENC(const krb5_pa_enc_ts *k, PA_ENC_TS_ENC *m)
{
    krb5_error_code errcode;
    errcode = krb5_timestamp2asn1_field (&k->patimestamp, &m->patimestamp);
        ERRCHECK();
    if (k->patimestamp) {
        errcode = opt_krb5_int322int(&k->pausec, &m->pausec);
        ERRCHECK();
    } else {
        m->pausec = 0;
    }
    return 0;
}
