/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 *
 * Copyright (c) Hewlett-Packard Company 1996
 * Unpublished work. All Rights Reserved.
 */
/*
 * HISTORY
 * $Log: ktest_equal.h,v $
 * Revision 1.1.2.1  1996/09/25  21:03:17  arvind
 * 	OSF DCE 1.2.2 - missing files
 * 	[1996/09/25  21:03:04  arvind]
 *
 * Revision /main/DCE_1.2.2/1  1996/06/20  18:18 UTC  mullan_s
 * 	Merge to DCE_1.2.2
 * 
 * Revision /main/mullan_dce_krb5_der_work/1  1996/06/10  19:42 UTC  mullan_s
 * 	Remove references to data types not defined in DCE.
 * 
 * $EndLog$
 */
#ifndef __KTEST_EQUAL_H__
#define __KTEST_EQUAL_H__

#include "krb5.h"

/* int ktest_equal_structure(krb5_structure *ref, *var) */
/* effects  Returns true (non-zero) if ref and var are
             semantically equivalent (i.e. have the same values,
	     but aren't necessarily the same object).
	    Returns false (zero) if ref and var differ. */

#define generic(funcname,type)\
int funcname PROTOTYPE((type *ref, type *var))

#define len_array(funcname,type)\
int funcname PROTOTYPE((const int length, type *ref, type *var))

generic(ktest_equal_authenticator,krb5_authenticator);
#ifdef OSF_DCE
int ktest_equal_principal_data PROTOTYPE((krb5_principal ref, krb5_principal var));
#else
generic(ktest_equal_principal_data,krb5_principal_data);
#endif
generic(ktest_equal_checksum,krb5_checksum);
generic(ktest_equal_keyblock,krb5_keyblock);
generic(ktest_equal_data,krb5_data);
generic(ktest_equal_authdata,krb5_authdata);
generic(ktest_equal_ticket,krb5_ticket);
generic(ktest_equal_enc_tkt_part,krb5_enc_tkt_part);
generic(ktest_equal_transited,krb5_transited);
generic(ktest_equal_ticket_times,krb5_ticket_times);
generic(ktest_equal_address,krb5_address);
generic(ktest_equal_enc_data,krb5_enc_data);

generic(ktest_equal_enc_kdc_rep_part,krb5_enc_kdc_rep_part);
generic(ktest_equal_priv,krb5_priv);
#ifndef OSF_DCE
generic(ktest_equal_cred,krb5_cred);
#endif
generic(ktest_equal_error,krb5_error);
generic(ktest_equal_ap_req,krb5_ap_req);
generic(ktest_equal_ap_rep,krb5_ap_rep);
generic(ktest_equal_ap_rep_enc_part,krb5_ap_rep_enc_part);
generic(ktest_equal_safe,krb5_safe);

generic(ktest_equal_last_req_entry,krb5_last_req_entry);
generic(ktest_equal_pa_data,krb5_pa_data);
#ifndef OSF_DCE
generic(ktest_equal_cred_info,krb5_cred_info);

generic(ktest_equal_enc_cred_part,krb5_cred_enc_part);
#endif
generic(ktest_equal_enc_priv_part,krb5_priv_enc_part);
generic(ktest_equal_as_rep,krb5_kdc_rep);
generic(ktest_equal_tgs_rep,krb5_kdc_rep);
generic(ktest_equal_as_req,krb5_kdc_req);
generic(ktest_equal_tgs_req,krb5_kdc_req);
generic(ktest_equal_kdc_req_body,krb5_kdc_req);
generic(ktest_equal_encryption_key,krb5_keyblock);

#ifndef OSF_DCE
generic(ktest_equal_passwd_phrase_element,passwd_phrase_element);
generic(ktest_equal_krb5_pwd_data,krb5_pwd_data);
#endif

int ktest_equal_last_req
	PROTOTYPE((krb5_last_req_entry **ref, krb5_last_req_entry **var));
int ktest_equal_sequence_of_ticket
	PROTOTYPE((krb5_ticket **ref, krb5_ticket **var));
int ktest_equal_sequence_of_pa_data
	PROTOTYPE((krb5_pa_data **ref, krb5_pa_data **var));
#ifndef OSF_DCE
int ktest_equal_sequence_of_cred_info
	PROTOTYPE((krb5_cred_info **ref, krb5_cred_info **var));
#endif

len_array(ktest_equal_array_of_enctype,krb5_enctype);
#ifdef OSF_DCE
len_array(ktest_equal_array_of_data,krb5_data *);
#else
len_array(ktest_equal_array_of_data,krb5_data);
#endif
len_array(ktest_equal_array_of_octet,krb5_octet);

#ifndef OSF_DCE
int ktest_equal_array_of_passwd_phrase_element
	PROTOTYPE((passwd_phrase_element **ref, passwd_phrase_element **var));
#endif
int ktest_equal_authorization_data
	PROTOTYPE((krb5_authdata **ref, krb5_authdata **var));
int ktest_equal_addresses
	PROTOTYPE((krb5_address **ref, krb5_address **var));
int ktest_equal_array_of_char
	PROTOTYPE((const int length, char *ref, char *var));

#endif
