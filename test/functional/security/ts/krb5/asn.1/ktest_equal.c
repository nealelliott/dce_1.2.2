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
 * $Log: ktest_equal.c,v $
 * Revision 1.1.2.1  1996/08/28  20:59:14  arvind
 * 	DCE 1.2.2 drop 3.5 - (test) files left out
 * 	[1996/08/28  20:51:20  arvind]
 *
 * Revision /main/DCE_1.2.2/1  1996/06/20  18:18 UTC  mullan_s
 * 	Merge to DCE_1.2.2
 * 
 * Revision /main/mullan_dce_krb5_der_work/1  1996/06/10  19:43 UTC  mullan_s
 * 	Port to DCE.
 * 
 * $EndLog$
 */
#include <stdlib.h>
#include <stdio.h>
#include "ktest_equal.h"

#define FALSE 0
#define TRUE 1

#define struct_equal(field,comparator)\
comparator(&(ref->field),&(var->field))

#define ptr_equal(field,comparator)\
comparator(ref->field,var->field)

#define scalar_equal(field)\
((ref->field) == (var->field))

#define len_equal(length,field,comparator)\
((ref->length == var->length) && \
 comparator(ref->length,ref->field,var->field))

int ktest_equal_authenticator(ref, var)
     krb5_authenticator * ref;
     krb5_authenticator * var;
{
  int p=TRUE;
  if(ref==var) return TRUE;
  else if(ref == NULL || var == NULL) return FALSE;
  p = p && ptr_equal(client,ktest_equal_principal_data);
  p = p && ptr_equal(checksum,ktest_equal_checksum);
  p = p && scalar_equal(cusec);
  p = p && scalar_equal(ctime);
  p = p && ptr_equal(subkey,ktest_equal_keyblock);
  p = p && scalar_equal(seq_number);
#ifndef OSF_DCE
  /* This field is new and is not in the DCE krb5_authenticator datatype. */
  p = p && ptr_equal(authorization_data,ktest_equal_authorization_data);
#endif
  return p;
}

int ktest_equal_principal_data(ref, var)
#ifdef OSF_DCE
     krb5_principal ref;
     krb5_principal var;
#else
     krb5_principal_data * ref;
     krb5_principal_data * var;
#endif
{
  if(ref==var) return TRUE;
  else if(ref == NULL || var == NULL) return FALSE;
#ifdef OSF_DCE
  /* DCE krb5_principal datatype is different than latest MIT, so
   * we have to do various tricks to convert and test the same fields.
   */
  return(ktest_equal_data(&krb5_princ_realm(*ref),&krb5_princ_realm(*var)) &&
	 ((krb5_princ_size(ref) == krb5_princ_size(var)) &&
          ktest_equal_array_of_data(krb5_princ_size(ref),ref,var)));
#else	 
  return(struct_equal(realm,ktest_equal_data) &&
	 len_equal(length,data,ktest_equal_array_of_data) &&
	 scalar_equal(type));
#endif
}

int ktest_equal_authdata(ref, var)
     krb5_authdata * ref;
     krb5_authdata * var;
{
  if(ref==var) return TRUE;
  else if(ref == NULL || var == NULL) return FALSE;
  return(scalar_equal(ad_type) &&
	 len_equal(length,contents,ktest_equal_array_of_octet)); }

int ktest_equal_checksum(ref, var)
     krb5_checksum * ref;
     krb5_checksum * var;
{
  if(ref==var) return TRUE;
  else if(ref == NULL || var == NULL) return FALSE;
  return(scalar_equal(checksum_type) && len_equal(length,contents,ktest_equal_array_of_octet));
}

int ktest_equal_keyblock(ref, var)
     krb5_keyblock * ref;
     krb5_keyblock * var;
{
  if(ref==var) return TRUE;
  else if(ref == NULL || var == NULL) return FALSE;
  return(scalar_equal(keytype) && len_equal(length,contents,ktest_equal_array_of_octet));
}

int ktest_equal_data(ref, var)
     krb5_data * ref;
     krb5_data * var;
{
  if(ref==var) return TRUE;
  else if(ref == NULL || var == NULL) return FALSE;
  return(len_equal(length,data,ktest_equal_array_of_char));
}

int ktest_equal_ticket(ref, var)
     krb5_ticket * ref;
     krb5_ticket * var;
{
  int p=TRUE;
  if(ref==var) return TRUE;
  else if(ref == NULL || var == NULL) return FALSE;
  p = p && ptr_equal(server,ktest_equal_principal_data);
  p = p && struct_equal(enc_part,ktest_equal_enc_data);
  /* enc_part2 is irrelevant, as far as the ASN.1 code is concerned */
  return p;
}

int ktest_equal_enc_data(ref, var)
     krb5_enc_data * ref;
     krb5_enc_data * var;
{
  int p=TRUE;
  if(ref==var) return TRUE;
  else if(ref == NULL || var == NULL) return FALSE;
  p=p&&scalar_equal(etype);
  p=p&&scalar_equal(kvno);
  p=p&&struct_equal(ciphertext,ktest_equal_data);
  return p;
}

int ktest_equal_encryption_key(ref, var)
     krb5_keyblock * ref;
     krb5_keyblock * var;
{
  int p=TRUE;
  if(ref==var) return TRUE;
  else if(ref == NULL || var == NULL) return FALSE;
  p = p && scalar_equal(keytype);
  p = p && len_equal(length,contents,ktest_equal_array_of_octet);
  return p;
}

int ktest_equal_enc_tkt_part(ref, var)
     krb5_enc_tkt_part * ref;
     krb5_enc_tkt_part * var;
{
  int p=TRUE;
  if(ref==var) return TRUE;
  else if(ref == NULL || var == NULL) return FALSE;
  p = p && scalar_equal(flags);
  p = p && ptr_equal(session,ktest_equal_encryption_key);
  p = p && ptr_equal(client,ktest_equal_principal_data);
  p = p && struct_equal(transited,ktest_equal_transited);
  p = p && struct_equal(times,ktest_equal_ticket_times);
  p = p && ptr_equal(caddrs,ktest_equal_addresses);
  p = p && ptr_equal(authorization_data,ktest_equal_authorization_data);
  return p;
}

int ktest_equal_transited(ref, var)
     krb5_transited * ref;
     krb5_transited * var;
{
  int p=TRUE;
  if(ref==var) return TRUE;
  else if(ref == NULL || var == NULL) return FALSE;
  p = p && scalar_equal(tr_type);
  p = p && struct_equal(tr_contents,ktest_equal_data);
  return p;
}

int ktest_equal_ticket_times(ref, var)
     krb5_ticket_times * ref;
     krb5_ticket_times * var;
{
  int p=TRUE;
  if(ref==var) return TRUE;
  else if(ref == NULL || var == NULL) return FALSE;
  p = p && scalar_equal(authtime);
  p = p && scalar_equal(starttime);
  p = p && scalar_equal(endtime);
  p = p && scalar_equal(renew_till);
  return p;
}

int ktest_equal_address(ref, var)
     krb5_address * ref;
     krb5_address * var;
{
  int p=TRUE;
  if(ref==var) return TRUE;
  else if(ref == NULL || var == NULL) return FALSE;
  p=p&&scalar_equal(addrtype);
  p=p&&len_equal(length,contents,ktest_equal_array_of_octet);
  return p;
}

int ktest_equal_enc_kdc_rep_part(ref, var)
     krb5_enc_kdc_rep_part * ref;
     krb5_enc_kdc_rep_part * var;
{
  int p=TRUE;
  if(ref==var) return TRUE;
  else if(ref == NULL || var == NULL) return FALSE;
  p=p&&ptr_equal(session,ktest_equal_keyblock);
  p=p&&ptr_equal(last_req,ktest_equal_last_req);
  p=p&&scalar_equal(nonce);
  p=p&&scalar_equal(key_exp);
  p=p&&scalar_equal(flags);
  p=p&&struct_equal(times,ktest_equal_ticket_times);
  p=p&&ptr_equal(server,ktest_equal_principal_data);
  p=p&&ptr_equal(caddrs,ktest_equal_addresses);
  return p;
}

int ktest_equal_priv(ref, var)
     krb5_priv * ref;
     krb5_priv * var;
{
  int p=TRUE;
  if(ref==var) return TRUE;
  else if(ref == NULL || var == NULL) return FALSE;
  p=p&&struct_equal(enc_part,ktest_equal_enc_data);
  return p;
}

#ifndef OSF_DCE
/* krb5_cred datatype not in DCE */
int ktest_equal_cred(ref, var)
     krb5_cred * ref;
     krb5_cred * var;
{
  int p=TRUE;
  if(ref==var) return TRUE;
  else if(ref == NULL || var == NULL) return FALSE;
  p=p&&ptr_equal(tickets,ktest_equal_sequence_of_ticket);
  p=p&&struct_equal(enc_part,ktest_equal_enc_data);
  return p;
}
#endif

int ktest_equal_error(ref, var)
     krb5_error * ref;
     krb5_error * var;
{
  int p=TRUE;
  if(ref==var) return TRUE;
  else if(ref == NULL || var == NULL) return FALSE;
  p=p&&scalar_equal(ctime);
  p=p&&scalar_equal(cusec);
  p=p&&scalar_equal(susec);
  p=p&&scalar_equal(stime);
  p=p&&scalar_equal(error);
  p=p&&ptr_equal(client,ktest_equal_principal_data);
  p=p&&ptr_equal(server,ktest_equal_principal_data);
  p=p&&struct_equal(text,ktest_equal_data);
  p=p&&struct_equal(e_data,ktest_equal_data);
  return p;
}

int ktest_equal_ap_req(ref, var)
     krb5_ap_req * ref;
     krb5_ap_req * var;
{
  int p=TRUE;
  if(ref==var) return TRUE;
  else if(ref == NULL || var == NULL) return FALSE;
  p=p&&scalar_equal(ap_options);
  p=p&&ptr_equal(ticket,ktest_equal_ticket);
  p=p&&struct_equal(authenticator,ktest_equal_enc_data);
  return p;
}

int ktest_equal_ap_rep(ref, var)
     krb5_ap_rep * ref;
     krb5_ap_rep * var;
{
  int p=TRUE;
  if(ref==var) return TRUE;
  else if(ref == NULL || var == NULL) return FALSE;
  p=p&&struct_equal(enc_part,ktest_equal_enc_data);
  return p;
}

int ktest_equal_ap_rep_enc_part(ref, var)
     krb5_ap_rep_enc_part * ref;
     krb5_ap_rep_enc_part * var;
{
  int p=TRUE;
  if(ref==var) return TRUE;
  else if(ref == NULL || var == NULL) return FALSE;
  p=p&&scalar_equal(ctime);
  p=p&&scalar_equal(cusec);
  p=p&&ptr_equal(subkey,ktest_equal_encryption_key);
  p=p&&scalar_equal(seq_number);
  return p;
}

int ktest_equal_safe(ref, var)
     krb5_safe * ref;
     krb5_safe * var;
{
  int p=TRUE;
  if(ref==var) return TRUE;
  else if(ref == NULL || var == NULL) return FALSE;
  p=p&&struct_equal(user_data,ktest_equal_data);
  p=p&&scalar_equal(timestamp);
  p=p&&scalar_equal(usec);
  p=p&&scalar_equal(seq_number);
  p=p&&ptr_equal(s_address,ktest_equal_address);
  p=p&&ptr_equal(r_address,ktest_equal_address);
  p=p&&ptr_equal(checksum,ktest_equal_checksum);
  return p;
}


#ifndef OSF_DCE
/* krb5_cred_enc_part datatype not in DCE */
int ktest_equal_enc_cred_part(ref, var)
     krb5_cred_enc_part * ref;
     krb5_cred_enc_part * var;
{
  int p=TRUE;
  if(ref==var) return TRUE;
  else if(ref == NULL || var == NULL) return FALSE;
  p=p&&scalar_equal(nonce);
  p=p&&scalar_equal(timestamp);
  p=p&&scalar_equal(usec);
  p=p&&ptr_equal(s_address,ktest_equal_address);
  p=p&&ptr_equal(r_address,ktest_equal_address);
  p=p&&ptr_equal(ticket_info,ktest_equal_sequence_of_cred_info);
  return p;
}
#endif

int ktest_equal_enc_priv_part(ref, var)
     krb5_priv_enc_part * ref;
     krb5_priv_enc_part * var;
{
  int p=TRUE;
  if(ref==var) return TRUE;
  else if(ref == NULL || var == NULL) return FALSE;
  p=p&&struct_equal(user_data,ktest_equal_data);
  p=p&&scalar_equal(timestamp);
  p=p&&scalar_equal(usec);
  p=p&&scalar_equal(seq_number);
  p=p&&ptr_equal(s_address,ktest_equal_address);
  p=p&&ptr_equal(r_address,ktest_equal_address);
  return p;
}

int ktest_equal_as_rep(ref, var)
     krb5_kdc_rep * ref;
     krb5_kdc_rep * var;
{
  int p=TRUE;
  if(ref==var) return TRUE;
  else if(ref == NULL || var == NULL) return FALSE;
  p=p&&scalar_equal(msg_type);
  p=p&&ptr_equal(padata,ktest_equal_sequence_of_pa_data);
  p=p&&ptr_equal(client,ktest_equal_principal_data);
  p=p&&ptr_equal(ticket,ktest_equal_ticket);
  p=p&&struct_equal(enc_part,ktest_equal_enc_data);
  p=p&&ptr_equal(enc_part2,ktest_equal_enc_kdc_rep_part);
  return p;
}

int ktest_equal_tgs_rep(ref, var)
     krb5_kdc_rep * ref;
     krb5_kdc_rep * var;
{
  return ktest_equal_as_rep(ref,var);
}

int ktest_equal_as_req(ref, var)
     krb5_kdc_req * ref;
     krb5_kdc_req * var;
{
  int p=TRUE;
  if(ref==var) return TRUE;
  else if(ref == NULL || var == NULL) return FALSE;
  p=p&&scalar_equal(msg_type);
  p=p&&ptr_equal(padata,ktest_equal_sequence_of_pa_data);
  p=p&&scalar_equal(kdc_options);
  p=p&&ptr_equal(client,ktest_equal_principal_data);
  p=p&&ptr_equal(server,ktest_equal_principal_data);
  p=p&&scalar_equal(from);
  p=p&&scalar_equal(till);
  p=p&&scalar_equal(rtime);
  p=p&&scalar_equal(nonce);
  p=p&&len_equal(netypes,etype,ktest_equal_array_of_enctype);
  p=p&&ptr_equal(addresses,ktest_equal_addresses);
  p=p&&struct_equal(authorization_data,ktest_equal_enc_data);
/* This field isn't actually in the ASN.1 encoding. */
/* p=p&&ptr_equal(unenc_authdata,ktest_equal_authorization_data); */
  return p;
}

int ktest_equal_tgs_req(ref, var)
     krb5_kdc_req * ref;
     krb5_kdc_req * var;
{
  return ktest_equal_as_req(ref,var);
}

int ktest_equal_kdc_req_body(ref, var)
     krb5_kdc_req * ref;
     krb5_kdc_req * var;
{
  int p=TRUE;
  if(ref==var) return TRUE;
  else if(ref == NULL || var == NULL) return FALSE;
  p=p&&scalar_equal(kdc_options);
  p=p&&ptr_equal(client,ktest_equal_principal_data);
  p=p&&ptr_equal(server,ktest_equal_principal_data);
  p=p&&scalar_equal(from);
  p=p&&scalar_equal(till);
  p=p&&scalar_equal(rtime);
  p=p&&scalar_equal(nonce);
  p=p&&len_equal(netypes,etype,ktest_equal_array_of_enctype);
  p=p&&ptr_equal(addresses,ktest_equal_addresses);
  p=p&&struct_equal(authorization_data,ktest_equal_enc_data);
  /* This isn't part of the ASN.1 encoding. */
  /* p=p&&ptr_equal(unenc_authdata,ktest_equal_authorization_data); */
  return p;
}

int ktest_equal_last_req_entry(ref, var)
     krb5_last_req_entry * ref;
     krb5_last_req_entry * var;
{
  int p=TRUE;
  if(ref==var) return TRUE;
  else if(ref == NULL || var == NULL) return FALSE;
  p=p&&scalar_equal(lr_type);
  p=p&&scalar_equal(value);
  return p;
}

int ktest_equal_pa_data(ref, var)
     krb5_pa_data * ref;
     krb5_pa_data * var;
{
  int p=TRUE;
  if(ref==var) return TRUE;
  else if(ref == NULL || var == NULL) return FALSE;
  p=p&&scalar_equal(pa_type);
  p=p&&len_equal(length,contents,ktest_equal_array_of_octet);
  return p;
}

#ifndef OSF_DCE
/* krb5_cred_info datatype not in DCE */
int ktest_equal_cred_info(ref, var)
     krb5_cred_info * ref;
     krb5_cred_info * var;
{
  int p=TRUE;
  if(ref==var) return TRUE;
  else if(ref == NULL || var == NULL) return FALSE;
  p=p&&ptr_equal(session,ktest_equal_keyblock);
  p=p&&ptr_equal(client,ktest_equal_principal_data);
  p=p&&ptr_equal(server,ktest_equal_principal_data);
  p=p&&scalar_equal(flags);
  p=p&&struct_equal(times,ktest_equal_ticket_times);
  p=p&&ptr_equal(caddrs,ktest_equal_addresses);

  return p;
}

int ktest_equal_passwd_phrase_element(ref, var)
     passwd_phrase_element * ref;
     passwd_phrase_element * var;
{
  int p=TRUE;
  if(ref==var) return TRUE;
  else if(ref == NULL || var == NULL) return FALSE;
  p=p&&ptr_equal(passwd,ktest_equal_data);
  p=p&&ptr_equal(phrase,ktest_equal_data);
  return p;
}

int ktest_equal_krb5_pwd_data(ref, var)
     krb5_pwd_data * ref;
     krb5_pwd_data * var;
{
  int p=TRUE;
  if(ref==var) return TRUE;
  else if(ref == NULL || var == NULL) return FALSE;
  p=p&&scalar_equal(sequence_count);
  p=p&&ptr_equal(element,ktest_equal_array_of_passwd_phrase_element);
  return p;
}
#endif

/**** arrays ****************************************************************/

int ktest_equal_array_of_data(length, ref, var)
     const int length;
#ifdef OSF_DCE
     krb5_data ** ref;
     krb5_data ** var;
#else
     krb5_data * ref;
     krb5_data * var;
#endif
{
  int i,p=TRUE;

  if(ref==var) return TRUE;
  else if(ref == NULL || var == NULL) return FALSE;
#ifdef OSF_DCE
  for(i=1; i<(length+1); i++){
    p = p && ktest_equal_data(ref[i],var[i]);
#else
  for(i=0; i<(length); i++){
    p = p && ktest_equal_data(&(ref[i]),&(var[i]));
#endif
  }
  return p;
}

int ktest_equal_array_of_octet(length, ref, var)
     const int length;
     krb5_octet * ref;
     krb5_octet * var;
{
  int i, p=TRUE;

  if(ref==var) return TRUE;
  else if(ref == NULL || var == NULL) return FALSE;
  for(i=0; i<length; i++)
    p = p && (ref[i] == var[i]);
  return p;
}

int ktest_equal_array_of_char(length, ref, var)
     const int length;
     char * ref;
     char * var;
{
  int i, p=TRUE;

  if(ref==var) return TRUE;
  else if(ref == NULL || var == NULL) return FALSE;
  for(i=0; i<length; i++)
    p = p && (ref[i] == var[i]);
  return p;
}

int ktest_equal_array_of_enctype(length, ref, var)
     const int length;
     krb5_enctype * ref;
     krb5_enctype * var;
{
  int i, p=TRUE;

  if(ref==var) return TRUE;
  else if(ref == NULL || var == NULL) return FALSE;
  for(i=0; i<length; i++)
    p = p && (ref[i] == var[i]);
  return p;
}

#define array_compare(comparator)\
int i,p=TRUE;\
if(ref==var) return TRUE;\
if(!ref || !ref[0])\
  return (!var || !var[0]);\
if(!var || !var[0]) return FALSE;\
for(i=0; ref[i] != NULL && var[i] != NULL; i++)\
  p = p && comparator(ref[i],var[i]);\
if(ref[i] == NULL && var[i] == NULL) return p;\
else return FALSE

int ktest_equal_authorization_data(ref, var)
     krb5_authdata ** ref;
     krb5_authdata ** var;
{
  array_compare(ktest_equal_authdata);
}

int ktest_equal_addresses(ref, var)
     krb5_address ** ref;
     krb5_address ** var;
{
  array_compare(ktest_equal_address);
}

int ktest_equal_last_req(ref, var)
     krb5_last_req_entry ** ref;
     krb5_last_req_entry ** var;
{
  array_compare(ktest_equal_last_req_entry);
}

int ktest_equal_sequence_of_ticket(ref, var)
     krb5_ticket ** ref;
     krb5_ticket ** var;
{
  array_compare(ktest_equal_ticket);
}

int ktest_equal_sequence_of_pa_data(ref, var)
     krb5_pa_data ** ref;
     krb5_pa_data ** var;
{
  array_compare(ktest_equal_pa_data);
}

#ifndef OSF_DCE
/* krb5_cred_info datatype not in DCE */
int ktest_equal_sequence_of_cred_info(ref, var)
     krb5_cred_info ** ref;
     krb5_cred_info ** var;
{
  array_compare(ktest_equal_cred_info);
}

int ktest_equal_array_of_passwd_phrase_element(ref, var)
     passwd_phrase_element ** ref;
     passwd_phrase_element ** var;
{
  array_compare(ktest_equal_passwd_phrase_element);
}
#endif
