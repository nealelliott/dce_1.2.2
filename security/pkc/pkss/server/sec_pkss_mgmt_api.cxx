//
// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// All Rights Reserved
//
// HISTORY
// $Log: sec_pkss_mgmt_api.cxx,v $
// Revision 1.1.2.1  1996/10/03  20:31:47  arvind
// 	OSF DCE 1.2.2 Drop 4
// 	[1996/10/03  19:31:03  arvind]
//
// Revision /main/DCE_1.2.2/1  1996/09/09  21:30 UTC  arvind
// 	PKSS drop from DEC (DCE1.2.2)
// 	[1996/08/30  15:54 UTC  arvind  /main/arvind_pkss/1]
// 
// $EndLog$
//
// (c) Copyright 1996,
// Digital Equipment Corporation, Maynard, Massachusetts, USA
// All Rights Reserved
//
// PKSS management API - see sec_pkss_mgmt_api.h for input/output
// descriptions.
//

#include "sec_bsafe.h"
#include "sec_pkss_asn.h"
#include "sec_pkss_lits.h"
#include "sec_pkss_util.h"
#include "sec_pkss_mgmt_if.h"
#include "sec_pkss_mgmt_api.h"
#include <dce/dce_msg.h>
#include <dce/dcepksmsg.h>
#include <dce/dcepkssvc.h>  
#include <dce/dcepksmac.h> 

#ifdef MALLOCTRACE
extern "C"
{
#include "malloc_trace.h"
}
#endif

//
// Do a Diffie-Hellman key exchange with the PKSS over integrity-
// protected RPC.  This function is used by the mgmt add and mod
// protocols, but NOT del.  This function is NOT part of the exposed
// mgmt interface, but is intended solely for use by the functions
// listed below.
//
// Called by:  sec_pkss_mgmt_user_add_clt
//             sec_pkss_mgmt_user_add_svr
//             sec_pkss_mgmt_user_mod_clt
//             sec_pkss_mgmt_user_mod_svr
//
// PKSS RPC's: 	sec_pkss_mgmt_diffhell
//

error_status_t sec_pkss_mgmt_do_dh
(
  const char *pkss_svc_profile_name_p,
  unsigned32 proto_id,
  sec_pkss_ctx_hdl_t *ctx_handle_p,
  rpc_binding_handle_t *binding_handle_p,
  sec_bsafe_alg_type_t *sym_alg_idx_p,
  sec_pk_gen_data_t *dh_key_p
)

{
  //
  // Local storage
  //

  error_status_t estatus = 0;
  error_status_t cstatus = 0;
  void *dh_context_p = 0;			// pointer to D-H context
  sec_pk_gen_data_t clt_dh_val = {0, 0};	// buffer for D-H value
  PkssMgmtP1MsgC mgmtP1Msg;		// ASN translation object
  PkssMgmtP2MsgC mgmtP2Msg;		// ASN translation object
  unsigned i = 0 ;
  buffer_t to_asn;			// ASN buffer
  sec_pkss_msg_buf_t to_pkss = {0, 0};	// msg buffer for ASN.1 data
  sec_pkss_msg_buf_t *from_pkss_p = 0;	// ret ASN.1 msg buf ptr from PKSS
  r_buffer_t from_asn;			// ret ASN.1 msg buf from PKSS
  sec_pk_gen_data_t pkss_dh_val = {0, 0}; // D-H params transltd from ASN.1
  sec_pk_gen_data_t pkss_h_algs = {0, 0}; // PKSS alg list hash buffer
  sec_pk_gen_data_t h_algs = {0, 0};	  // local alg list hash buffer
  unsigned32 clt_algs_list[PKSS_NUM_CRYPTO_ALGS];// all crypto algs avail to clt
  buffer_t clt_algs_buf;
  sec_pk_gen_data_t clt_algs = {0, 0};
  unsigned32 sym_alg_idx = 0;
  unsigned32 dh_key_len = 0;
  asn_integer *new_child_p = 0;
  dh_key_p->len = 0;
  dh_key_p->data = 0;

  //
  // Obtain secure binding handle for mgmt operations.
  //

  estatus = sec_pkss_bind (pkss_svc_profile_name_p,
			   sec_pkss_mgmt_if_v1_0_c_ifspec,
			   binding_handle_p);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from sec_pkss_bind : 0x%x", estatus));

  if (estatus != 0)
  {
    return (estatus);
  }

  estatus = sec_pkss_secure_clt_handle (*binding_handle_p);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from sec_pkss_secure_clt_handle : 0x%x", estatus));

  if (estatus != 0)
  {
    rpc_binding_free (binding_handle_p, &cstatus);
    return (estatus);
  }

  //
  // Obtain exponentiated D-H client value E(Dc).
  //

  estatus = sec_pkss_get_dh_val (&dh_context_p, &clt_dh_val);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from sec_pkss_get_dh_val : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Initialize the ASN.1 translation object in anticipation
  // of translating PKSS management protocol phase 1 message I:E(Dc):algList
  // to ASN.1.
  //

  estatus = mgmtP1Msg.protoID.set_value (proto_id);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from ASN set_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  estatus = mgmtP1Msg.eDc.set_value (clt_dh_val.data, clt_dh_val.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from ASN set_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Obtain list of crypto algorithms available to PKSS client.  In a future
  // revision, these should be replaced by ASN.1 algorithm ID types
  // "AlgorithmIdentifier".
  //

  for (i = 0; i < PKSS_NUM_CRYPTO_ALGS; i++)
  {
    new_child_p = mgmtP1Msg.algList.new_child ();

    if (new_child_p == 0)
    {
      estatus = pks_s_no_memory;
      goto rundown;
    }

    estatus = new_child_p->set_value(pkss_master_alg_list[i]);

    if (estatus != 0)
    {
      goto rundown;
    }

    clt_algs_list[i] = pkss_master_alg_list[i];
  }

  //
  // Initialize the ASN.1 buffer object into which to write
  // the translated PKSS management protocol phase 1 message I:E(Dc):algList
  // in anticipation of transmission to the PKSS.
  //

  estatus = mgmtP1Msg.write(to_asn);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from ASN write : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Move encrypted data from ASN.1 buffer object to RPC input buffer in
  // anticipation of transmission to the PKSS.
  //

  to_pkss.msg_len = to_asn.data_length;
  to_pkss.msg_p = to_asn.data;

  //
  // Send exponentiated D-H client value E(Dc) to PKSS; receive
  // exponentiated D-H server value E(Dp) from PKSS.
  //

  cstatus = 0;

  cstatus = sec_pkss_mgmt_diffhell (*binding_handle_p,
			  	    ctx_handle_p,
			  	    &to_pkss,
			  	    &from_pkss_p,
			  	    &estatus);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from sec_pkss_mgmt_diffhell : 0x%x", estatus));

  if (cstatus != rpc_s_ok)
    estatus = cstatus;

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Note that this also zeroes to_asn.  Also zero and release contents
  // of clt_dh_val.
  //

  sec_pkss_zero_and_free (&clt_dh_val);

  //
  // Instantiate ASN.1 buffer object into which to move data from RPC output
  // buffer upon return from the PKSS.  Move the RPC output data to the ASN.1
  // buffer object in anticipation ASN.1 translation.
  //

  from_asn.data = from_pkss_p->msg_p;
  from_asn.data_length = from_pkss_p->msg_len;

  //
  // ASN.1-decode E(Dp):H(algList):algId.
  //

  estatus = mgmtP2Msg.read (from_asn);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from ASN read : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Obtain output parameters translated from ASN.1.
  //

  estatus = mgmtP2Msg.eDp.get_value (pkss_dh_val.data, pkss_dh_val.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from ASN get_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  estatus = mgmtP2Msg.hAlgs.get_value (pkss_h_algs.data, pkss_h_algs.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from ASN get_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  estatus = mgmtP2Msg.algIdx.get_value (*sym_alg_idx_p);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from ASN get_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Zero and release from_pkss.
  //

  sec_pkss_zero_and_free (&from_pkss_p);

  //
  // Compute algList hash H(algList) on client's copy of the crypto algorithm
  // list.  Compare result with H(algs) returned from the PKSS.  If they do
  // not match, assume a message stream modification attack and discontinue
  // processing.
  //

  estatus = mgmtP1Msg.algList.write (clt_algs_buf);

  if (estatus != 0)
  {
    goto rundown;
  }

  clt_algs.data = clt_algs_buf.data;
  clt_algs.len = clt_algs_buf.data_length;

  estatus = sec_bsafe_compute_hash (sec_bsafe_hash_MD2,
                                    &clt_algs,
                                    &h_algs);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from sec_bsafe_compute_hash : 0x%x", estatus));

  if (estatus != 0)
  {
    dce_svc_printf(PKS_S_SEC_BSAFE_MSG, estatus);
    goto rundown;
  }

  estatus = sec_pkss_pk_gen_data_cmp (&pkss_h_algs, &h_algs);

  if (estatus != 0)
  {
    goto rundown;
  }

  sec_pkss_zero_and_free (&h_algs);

  //
  // Compute D-H key D using E(Dc) and E(Dp).
  //

  estatus = sec_bsafe_symmetric_alginfo (pkss_master_alg_list[*sym_alg_idx_p],
                                         &dh_key_len,
                                         0, 0, 0);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_bsafe_symmetric_alginfo : 0x%x", estatus));

  if (estatus != 0)
  {
    dce_svc_printf(PKS_S_SEC_BSAFE_MSG, estatus);
    goto rundown;
  }

  dh_key_p->len = dh_key_len;
  dh_key_p->data = (unsigned char *) calloc (1, dh_key_p->len);

  if (!(dh_key_p->data))
  {
    dce_svc_printf(PKS_S_NO_MEMORY_MSG);
    goto rundown;
  }

  estatus = sec_bsafe_diffie_hellman_phase_2 (&dh_context_p,
                                              &pkss_dh_val,
                                              dh_key_p);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from sec_bsafe_diffie_hellman_phase_2 : 0x%x",
		   estatus));

  if (estatus != 0)
  {
    dce_svc_printf(PKS_S_SEC_BSAFE_MSG, estatus);
    goto rundown;
  }

  //
  // Exit this function.
  //

  return (estatus);

rundown:

  sec_pkss_zero_and_free (&clt_dh_val);
  sec_pkss_zero_and_free (&from_pkss_p); // this also takes care of from_asn
  sec_pkss_zero_and_free (&h_algs);

  //
  // Exit this function.
  //

  return (estatus);

} // End sec_pkss_mgmt_do_dh


//
// Add or modify a user (client generates asymmetric key pair).  This function
// is NOT part of the exposed mgmt interface, but is intended solely for use by
// the "Called by" functions listed below.
//
// Called by:  sec_pkss_mgmt_user_add_clt
//             sec_pkss_mgmt_user_mod_clt
//
// Calls: 	sec_pkss_mgmt_do_dh
//
// PKSS RPC's: 	sec_pkss_mgmt_add_user_clt
//              sec_pkss_mgmt_mod_user_clt
//

error_status_t sec_pkss_mgmt_clt_gen_util
(
  const char *pkss_svc_profile_name_p,
  unsigned32 proto_id,
  const char *name_p,
  sec_pk_domain_t domain_id,
  sec_pk_usage_flags_t key_usages,
  unsigned32 key_vno,
  sec_pk_pvtkey_t *pvt_key_p,
  sec_pk_pubkey_t *pub_key_p,
  const char *pwd_p,
  unsigned32 *new_key_vno_p
)

{
  //
  // Local variables
  //

  error_status_t estatus = 0;
  error_status_t cstatus = 0;
  sec_bsafe_alg_type_t sym_alg_id = 0;		// chosen algorithm id
  sec_pkss_ctx_hdl_t ctx_handle = 0;
  sec_pk_gen_data_t dh_key = {0, 0};
  sec_pk_gen_data_t h1 = {0, 0};		// H1 pwd hash buffer
  sec_pk_gen_data_t h2 = {0, 0};		// H2 pwd hash buffer
  sec_pk_gen_data_t mod_h1 = {0, 0};		// modified H1 pwd hash buffer
  sec_pk_gen_data_t mod_h2 = {0, 0};		// modified H2 pwd hash buffer
  sec_bsafe_alg_type_t sym_alg_id_pad = 0;
  sec_bsafe_alg_type_t sym_alg_id_nopad = 0;
  unsigned32 sym_pad_key_len = 0;
  unsigned32 sym_nopad_key_len = 0;
  sec_pk_gen_data_t *sym_pad_iv_p = 0;
  sec_pk_gen_data_t *sym_nopad_iv_p = 0;
  sec_pk_gen_data_t kxuH1 = {0, 0};		// private key enc. w/h1 buf
  PkssMgmtCltP3DataC mgmtCltP3Data;		// ASN translation obj
  unsigned char *uuid_string_p = 0;
  buffer_t pub_key_data;
  buffer_t to_asn_plain (SECURE);		// ASN buffer
  sec_pk_gen_data_t plaintext_in = {0, 0};
  sec_pk_gen_data_t plaintext_out = {0, 0};
  sec_pk_gen_data_t ciphertext_in = {0, 0};
  sec_pk_gen_data_t ciphertext_out = {0, 0};
  buffer_t to_asn_cipher (SECURE);		// buf for PKSS
  PkssMgmtCltP3MsgBaseC *mgmtCltP3MsgP = 0;
  sec_pkss_msg_buf_t to_pkss = {0, 0};
  sec_pkss_msg_buf_t *from_pkss_p = 0;		
  r_buffer_t from_asn_cipher;
  PkssMgmtCltP4MsgBaseC *mgmtCltP4MsgP = 0;
  r_buffer_t from_asn_plain;
  PkssMgmtCltP4DataC mgmtCltP4Data;		// ASN translation obj
  unsigned32 sym_alg_idx = 0;

  //
  // If no key usage bits are set, return an error.
  //

  if (key_usages == 0)
  {
    estatus = sec_pvtkey_multiple_key_usages;
    return (estatus);
  }

  //
  // If more than one key usage bit is set on a modify operation,
  // return an error.
  //

  if (proto_id == PKSS_PROTO_MGMT_MOD_USR_CLT)
  {
    estatus = sec_pkss_count_usages (key_usages);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from sec_pkss_count_usages : 0x%x", estatus));

    if (estatus != 0)
    {
      estatus = sec_pvtkey_multiple_key_usages;
      return (estatus);
    }
  }

  //
  // Obtain D-H key for secure communication with the PKSS.
  //

  rpc_binding_handle_t binding_handle;
  estatus = sec_pkss_mgmt_do_dh (pkss_svc_profile_name_p,
				 proto_id,
				 &ctx_handle,
				 &binding_handle,
				 &sym_alg_idx,
				 &dh_key);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from sec_pkss_mgmt_do_dh : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Obtain required symmetric key crypto info given a symmetric key
  // encryption algorithm identifier.  Note that it is the caller's
  // responsibility to free initial vector storage allocated in this
  // function.
  //

  if (sym_alg_idx < PKSS_NUM_CRYPTO_ALGS)
    sym_alg_id = pkss_master_alg_list[sym_alg_idx];
  else
  {
    goto rundown;
  }

  estatus = sec_pkss_get_sym_crypto_info (sym_alg_id,
                                          &sym_alg_id_pad,
                                          &sym_alg_id_nopad,
                                          &sym_pad_key_len,
                                          &sym_nopad_key_len,
                                          &sym_pad_iv_p,
                                          &sym_nopad_iv_p);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from sec_pkss_get_sym_crypto_info : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Obtain hashes H1 and H2 from contents of name_p and pwd_p.
  //

  estatus = sec_pkss_get_pwd_hashes (pwd_p, name_p, sym_pad_key_len, &h1, &h2);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from sec_pkss_get_pwd_hashes : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Encrypt the private key Kxu with the new password hash H1 to
  // obtain [Kxu]H1.  Zero and release contents of H1.
  //

  estatus = sec_bsafe_symmetric_encrypt (sym_alg_id_pad,
                                         &h1,
                                         sym_pad_iv_p,
                                         (sec_pk_gen_data_t *)pvt_key_p,
                                         &kxuH1);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from sec_bsafe_symmetric_encrypt : 0x%x", estatus));

  if (estatus != 0)
  {
    dce_svc_printf(PKS_S_SEC_BSAFE_MSG, estatus);
    goto rundown;
  }

  sec_pkss_zero_and_free (&h1);

  //
  // Initialize the ASN.1 translation object in anticipation
  // of translating mgmt protocol (client generates keys) phase 3 client
  // message Username:[Kxu]H1:H2:Kou to ASN.1 before encrypting.
  //

  estatus = mgmtCltP3Data.userKey.uname.set_value (name_p);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from ASN set_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  uuid_to_string (&domain_id, &uuid_string_p, &estatus);
  estatus = mgmtCltP3Data.userKey.domain.set_value
						((const char *)uuid_string_p);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from ASN set_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  estatus = mgmtCltP3Data.userKey.usages.set_value (key_usages);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from ASN set_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  estatus = mgmtCltP3Data.userKey.version.set_value (key_vno);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from ASN set_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  estatus = mgmtCltP3Data.kxuH1.set_value (kxuH1.data, kxuH1.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from ASN set_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  estatus = mgmtCltP3Data.H2.set_value (h2.data, h2.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from ASN set_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Write the public key data.  Recall that this layer of software 
  // leaves the public key data in ASN.1 form for use by the calling 
  // layer.  Note that it is the caller's responsibility to clear 
  // and free the allocated storage.
  //

  pub_key_data.data = pub_key_p->data;
  pub_key_data.data_length = pub_key_p->len;

  estatus = mgmtCltP3Data.kou.read (pub_key_data);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from ASN read : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // ASN.1-encode Username:[Kxu]H1:H2:Kou in anticipation of encrypting.
  //

  estatus = mgmtCltP3Data.write (to_asn_plain);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from ASN write : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Zero and release contents of kxuH1 and uuid_string_p.
  //

  sec_pkss_zero_and_free (&kxuH1);

  (void) memset (uuid_string_p, 0, strlen ((const char *)uuid_string_p));
  rpc_string_free (&uuid_string_p, &estatus);
  uuid_string_p = 0;

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Move contents of ASN.1 buffer object to sec_pk_gen_data_t struct in
  // anticipation of encrypting it.
  //

  plaintext_in.data = to_asn_plain.data;
  plaintext_in.len = to_asn_plain.data_length;

  //
  // Encrypt username:[Kxu]H1:H2:Kou with D to obtain
  // [Username:[Kxu]H1:H2:Kou]D.
  //

  estatus = sec_bsafe_symmetric_encrypt (sym_alg_id_pad,
                                         &dh_key,
                                         sym_pad_iv_p,
                                         &plaintext_in,
                                         &ciphertext_out);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from sec_bsafe_symmetric_encrypt : 0x%x", estatus));

  if (estatus != 0)
  {
    dce_svc_printf(PKS_S_SEC_BSAFE_MSG, estatus);
    goto rundown;
  }

  //
  // Instantiate the ASN.1 translation object given by proto_id in anticipation
  // of translating mgmt (client generates new keys) protocol phase 3 message
  // [Username:[Kxu]H1:H2:Kou]D to ASN.1.
  //

  switch (proto_id)
  {
    case PKSS_PROTO_MGMT_ADD_USR_CLT:
      mgmtCltP3MsgP = new PkssMgmtCltAddP3MsgC;
      break;

    case PKSS_PROTO_MGMT_MOD_USR_CLT:
      mgmtCltP3MsgP = new PkssMgmtCltModP3MsgC;
      break;

    default:
      dce_svc_printf(PKS_S_INVALID_ARG_MSG);
      estatus = pks_s_invalid_arg;
      goto rundown;

  } // End switch (proto_id)

  if (!(mgmtCltP3MsgP))
  {
    dce_svc_printf(PKS_S_NO_MEMORY_MSG);
    estatus = pks_s_no_memory;
    goto rundown;
  }

  //
  // Initialize the ASN.1 translation object in anticipation of translating
  // mgmt protocol (client generates keys) phase 3 message
  // [Username:[Kxu]H1:H2:Kou]D to ASN.1.
  //

  mgmtCltP3MsgP->set_value (ciphertext_out.data, ciphertext_out.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from ASN set_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // ASN.1-encode [Username:[Kxu]H1:H2:Kou]D for transmission to PKSS.
  //

  estatus = mgmtCltP3MsgP->write (to_asn_cipher);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from ASN write : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Zero and delete instance of ASN.1 translation object after writing
  // ASN.1-encoded data to ASN.1 buffer object.  Also zero and delete
  // contents of ciphertext_out.
  //

  delete mgmtCltP3MsgP;
  mgmtCltP3MsgP = 0;
  sec_pkss_zero_and_free (&ciphertext_out);

  //
  // Move encrypted data from ASN.1 buffer object to RPC input buffer in
  // anticipation of transmission to the PKSS.
  //

  to_pkss.msg_len = to_asn_cipher.data_length;
  to_pkss.msg_p = to_asn_cipher.data;

  //
  // Call PKSS to process indicated request.
  //

  switch (proto_id)
  {
    case PKSS_PROTO_MGMT_ADD_USR_CLT:

      cstatus = sec_pkss_mgmt_add_user_clt (binding_handle,
				  	    &ctx_handle,
				  	    &to_pkss,
				  	    &from_pkss_p,
				  	    &estatus);

        DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "exit status from  sec_pkss_mgmt_add_user_clt : 0x%x",
		   estatus));

        DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "comm status from  sec_pkss_mgmt_add_user_clt : 0x%x",
		   cstatus));
      break;

    case PKSS_PROTO_MGMT_MOD_USR_CLT:

      cstatus = sec_pkss_mgmt_mod_user_clt (binding_handle,
				  	    &ctx_handle,
				  	    &to_pkss,
				  	    &from_pkss_p,
				  	    &estatus);

        DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "exit status from  sec_pkss_mgmt_mod_user_clt : 0x%x",
		   estatus));

        DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "comm status from  sec_pkss_mgmt_mod_user_clt : 0x%x",
		   cstatus));
      break;

    default:
      dce_svc_printf(PKS_S_INVALID_ARG_MSG);
      estatus = pks_s_invalid_arg;
      goto rundown;

  } // End switch (proto_id)

  if (cstatus != rpc_s_ok)
    estatus = cstatus;

  rpc_binding_free (&binding_handle, &cstatus);

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Move data from RPC output buffer upon return from the PKSS.  The 
  // contents of the output buffer are an ASN.1-encoded bit string of 
  // encrypted data.  Move the RPC output data to the ASN.1 buffer object
  // in anticipation ASN.1 translation.
  //

  from_asn_cipher.data = from_pkss_p->msg_p;
  from_asn_cipher.data_length = from_pkss_p->msg_len;

  //
  // Instantiate ASN.1 translation object given by proto_id for encrypted
  // mgmt (client generates keys) protocol phase 4 message [V:Status]D
  // received from the PKSS.
  //

  switch (proto_id)
  {
    case PKSS_PROTO_MGMT_ADD_USR_CLT:
      mgmtCltP4MsgP = new PkssMgmtCltAddP4MsgC;
      break;

    case PKSS_PROTO_MGMT_MOD_USR_CLT:
      mgmtCltP4MsgP = new PkssMgmtCltModP4MsgC;
      break;

    default:
      dce_svc_printf(PKS_S_INVALID_ARG_MSG);
      estatus = pks_s_invalid_arg;
      goto rundown;

  } // End switch (proto_id)


  if (!(mgmtCltP4MsgP))
  {
    dce_svc_printf(PKS_S_NO_MEMORY_MSG);
    estatus = pks_s_no_memory;
    goto rundown;
  }

  //
  // ASN.1-decode [V:Status]D upon return from the PKSS.
  //

  estatus = mgmtCltP4MsgP->read(from_asn_cipher);

  if (estatus != 0)
  {
    goto rundown;
  }

  estatus = mgmtCltP4MsgP->get_value (ciphertext_in.data, ciphertext_in.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from ASN get_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Zero and release from_pkss.
  //

  sec_pkss_zero_and_free (&from_pkss_p);

  //
  // Decrypt [V:status]D with D to obtain V:status.
  //

  estatus = sec_bsafe_symmetric_decrypt (sym_alg_id_pad,
                                         &dh_key,
                                         sym_pad_iv_p,
                                         &ciphertext_in,
                                         &plaintext_out);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from sec_bsafe_symmetric_decrypt : 0x%x", estatus));

  if (estatus != 0)
  {
    dce_svc_printf(PKS_S_SEC_BSAFE_MSG, estatus);
    goto rundown;
  }

  //
  // Move decrypted data from struct pointed to by plaintext to ASN.1 buffer
  // object in anticipation of populating the ASN.1 translation object.
  //

  from_asn_plain.data = plaintext_out.data;
  from_asn_plain.data_length = plaintext_out.len;

  //
  // ASN.1-decode V:status.
  //

  estatus = mgmtCltP4Data.read(from_asn_plain);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from ASN read : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Zero and release everything that hasn't already.
  //

  sec_pkss_zero_and_free (&dh_key); 
  sec_pkss_zero_and_free (&plaintext_out);
  sec_pkss_zero_and_free (&sym_pad_iv_p);
  sec_pkss_zero_and_free (&sym_nopad_iv_p);

  //
  // Load return parameters.
  //

  estatus = mgmtCltP4Data.version.get_value (*new_key_vno_p); // V

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from ASN get_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  estatus = mgmtCltP4Data.status.get_value (estatus); // status

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from ASN get_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Exit this function.
  //

  return (estatus);

rundown:

  sym_alg_id = 0;
  sec_pkss_zero_and_free (&dh_key);
  sec_pkss_zero_and_free (&h1);
  sec_pkss_zero_and_free (&h2);
  sym_alg_id_pad = 0;
  sym_alg_id_nopad = 0;
  sym_pad_key_len = 0;
  sym_nopad_key_len = 0;
  sec_pkss_zero_and_free (&sym_nopad_iv_p);
  sec_pkss_zero_and_free (&sym_pad_iv_p);
  sec_pkss_zero_and_free (&kxuH1);

  if (uuid_string_p) {
    (void) memset (uuid_string_p, 0, strlen ((const char *)uuid_string_p));
    rpc_string_free (&uuid_string_p, &estatus);
    uuid_string_p = 0;
  }

  sec_pkss_zero_and_free (&plaintext_out);
  sec_pkss_zero_and_free (&ciphertext_out);

  if (mgmtCltP3MsgP)
  {
    delete mgmtCltP3MsgP;
    mgmtCltP3MsgP = 0;
  }

  if (mgmtCltP4MsgP)
  {
    delete mgmtCltP4MsgP;
    mgmtCltP4MsgP = 0;
  }

  sec_pkss_zero_and_free (&from_pkss_p);

  rpc_binding_free (&binding_handle, &cstatus);

  //
  // Exit this function.
  //

  return (estatus);

} // End sec_pkss_mgmt_clt_gen_util


//
// Add or modify a user (server generates asymmetric key pair).  This function
// is NOT part of the exposed mgmt interface, but is intended solely for use by
// the "Called by" functions listed below.
//
// Called by:  sec_pkss_mgmt_user_add_svr
//             sec_pkss_mgmt_user_mod_svr
//
// Calls: 	sec_pkss_mgmt_do_dh
//
// PKSS RPC's: 	sec_pkss_mgmt_add_user_svr
//             	sec_pkss_mgmt_mod_user_svr
//

error_status_t sec_pkss_mgmt_svr_gen_util
(
  const char *pkss_svc_profile_name_p,
  unsigned32 proto_id,
  const char *name_p,
  sec_pk_domain_t domain_id,
  sec_pk_usage_flags_t key_usages,
  unsigned32 key_vno,
  const char *pwd_p,
  sec_pk_algorithm_id_t *pk_alg_id_p,
  unsigned32 pk_key_len,
  sec_pk_pubkey_t *new_pub_key_p,
  unsigned32 *new_key_vno_p
)

{

  //
  // Local variables
  //

  error_status_t estatus = 0;
  error_status_t pstatus = 0;
  error_status_t cstatus = 0;
  sec_bsafe_alg_type_t sym_alg_id = 0;
  unsigned32 sym_alg_idx = 0;
  sec_pkss_ctx_hdl_t ctx_handle = 0;
  sec_pk_gen_data_t dh_key = {0, 0};
  sec_bsafe_alg_type_t sym_alg_id_pad = 0;
  sec_bsafe_alg_type_t sym_alg_id_nopad = 0;
  unsigned32 sym_pad_key_len = 0;
  unsigned32 sym_nopad_key_len = 0;
  sec_pk_gen_data_t *sym_pad_iv_p = 0;
  sec_pk_gen_data_t *sym_nopad_iv_p = 0;
  PkssMgmtSvrP3DataC mgmtSvrP3Data;
  unsigned char *uuid_string_p = 0;
  buffer_t to_asn_plain;		
  sec_pk_gen_data_t plaintext_in = {0, 0};
  sec_pk_gen_data_t plaintext_out = {0, 0};
  sec_pk_gen_data_t ciphertext_in = {0, 0};
  sec_pk_gen_data_t ciphertext_out = {0, 0};
  PkssMgmtSvrP3MsgBaseC *mgmtSvrP3MsgP = 0;
  buffer_t to_asn_cipher;
  sec_pkss_msg_buf_t to_pkss = {0, 0};
  sec_pkss_msg_buf_t *from_pkss_p = 0;
  r_buffer_t from_asn_cipher;
  PkssMgmtSvrP4MsgBaseC *mgmtSvrP4MsgP = 0;
  r_buffer_t from_asn_plain;
  PkssMgmtSvrP4DataC mgmtSvrP4Data;
  buffer_t pub_key_data;


  //
  // One and only one key usage bit may be set.
  //

  estatus = sec_pkss_count_usages (key_usages);

  DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                 "status from sec_pkss_count_usages : 0x%x", estatus));

  if (estatus != 0)
  {
    estatus = sec_pvtkey_multiple_key_usages;
    return (estatus);
  }

  //
  // Obtain D-H key for secure communication with the PKSS.
  //

  rpc_binding_handle_t binding_handle;
  estatus = sec_pkss_mgmt_do_dh (pkss_svc_profile_name_p,
				 proto_id,
                                 &ctx_handle,
                                 &binding_handle,
                                 &sym_alg_idx,
                                 &dh_key);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from sec_pkss_mgmt_do_dh : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Obtain required symmetric key crypto info given a symmetric key
  // encryption algorithm identifier.  Note that it is the caller's
  // responsibility to free initial vector storage allocated in this
  // function.
  //

  if (sym_alg_idx < PKSS_NUM_CRYPTO_ALGS)
    sym_alg_id = pkss_master_alg_list[sym_alg_idx];
  else
  {
    estatus = pks_s_invalid_arg;
    dce_svc_printf(PKS_S_INVALID_ARG_1_PARM_MSG, sym_alg_idx);
    goto rundown;
  }

  //
  // Obtain required symmetric key crypto info given a symmetric key
  // encryption algorithm identifier.  Note that it is the caller's
  // responsibility to free initial vector storage allocated in this
  // function.
  //

  estatus = sec_pkss_get_sym_crypto_info (sym_alg_id,
                                          &sym_alg_id_pad,
                                          &sym_alg_id_nopad,
                                          &sym_pad_key_len,
                                          &sym_nopad_key_len,
                                          &sym_pad_iv_p,
                                          &sym_nopad_iv_p);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from sec_pkss_get_sym_crypto_info : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Initialize the ASN.1 translation object in anticipation
  // of translating mgmt protocol (server generates keys) phase 3 client
  // message Username:password to ASN.1 before encrypting.
  //

  estatus = mgmtSvrP3Data.userKey.uname.set_value (name_p);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from ASN set_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  uuid_to_string (&domain_id, &uuid_string_p, &estatus);
  estatus = mgmtSvrP3Data.userKey.domain.set_value
						((const char *)uuid_string_p);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from ASN set_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  estatus = mgmtSvrP3Data.userKey.usages.set_value (key_usages);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from ASN set_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  estatus = mgmtSvrP3Data.userKey.version.set_value (key_vno);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from ASN set_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  estatus = mgmtSvrP3Data.pkAlgID.set_value (pk_alg_id_p->data,
					     pk_alg_id_p->len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from ASN set_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  estatus = mgmtSvrP3Data.pkKeyLen.set_value (pk_key_len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from ASN set_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  estatus = mgmtSvrP3Data.pwd.set_value (pwd_p);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from ASN set_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // ASN.1-encode Username:password in anticipation of encrypting.
  //

  estatus = mgmtSvrP3Data.write (to_asn_plain);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from ASN write : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Move contents of ASN.1 buffer object to sec_pk_gen_data_t struct in
  // anticipation of encrypting it.
  //

  plaintext_in.data = to_asn_plain.data;
  plaintext_in.len = to_asn_plain.data_length;

  //
  // Encrypt username:password with the Diffie-Hellman key D to obtain
  // [Username:password]D
  //

  estatus = sec_bsafe_symmetric_encrypt (sym_alg_id_pad,
                                         &dh_key,
                                         sym_pad_iv_p,
                                         &plaintext_in,
                                         &ciphertext_out);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from sec_bsafe_symmetric_encrypt : 0x%x", estatus));

  if (estatus != 0)
  {
    dce_svc_printf(PKS_S_SEC_BSAFE_MSG, estatus);
    goto rundown;
  }

  //
  // Zero and release contents of uuid_string_p.
  //

  (void) memset (uuid_string_p, 0, strlen ((const char *)uuid_string_p));
  rpc_string_free (&uuid_string_p, &estatus);
  uuid_string_p = 0;

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Instantiate the ASN.1 translation object given by proto_id in anticipation
  // of translating mgmt (server generates new keys) protocol phase 3 message
  // [Username:password]D to ASN.1.
  //

  switch (proto_id)
  {
    case PKSS_PROTO_MGMT_ADD_USR_SVR:
      mgmtSvrP3MsgP = new PkssMgmtSvrAddP3MsgC;
      break;

    case PKSS_PROTO_MGMT_MOD_USR_SVR:
      mgmtSvrP3MsgP = new PkssMgmtSvrModP3MsgC;
      break;

    default:
      dce_svc_printf(PKS_S_INVALID_ARG_MSG);
      estatus = pks_s_invalid_arg;
      goto rundown;

  } // End switch (proto_id)

  if (!(mgmtSvrP3MsgP))
  {
    dce_svc_printf(PKS_S_NO_MEMORY_MSG);
    estatus = pks_s_no_memory;
    goto rundown;
  }

  //
  // Initialize the ASN.1 translation object in anticipation of translating
  // mgmt protocol (server generates keys) phase 3 message
  // [Username:password]D to ASN.1.
  //

  estatus = mgmtSvrP3MsgP->set_value (ciphertext_out.data, ciphertext_out.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from ASN set_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // ASN.1-encode [Username:password]D for transmission to PKSS.
  //

  estatus = mgmtSvrP3MsgP->write (to_asn_cipher);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from ASN write : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Zero and delete instance of ASN.1 translation object after writing
  // ASN.1-encoded data to ASN.1 buffer object.  Also zero and delete
  // contents of ciphertext_out.
  //

  delete mgmtSvrP3MsgP;
  mgmtSvrP3MsgP = 0;
  sec_pkss_zero_and_free (&ciphertext_out);

  //
  // Move encrypted data from ASN.1 buffer object to RPC input buffer in
  // anticipation of transmission to the PKSS.
  //

  to_pkss.msg_len = to_asn_cipher.data_length;
  to_pkss.msg_p = to_asn_cipher.data;

  //
  // Call PKSS to process indicated request.
  //

  cstatus = 0;

  switch (proto_id)
  {
    case PKSS_PROTO_MGMT_ADD_USR_SVR:

      cstatus = sec_pkss_mgmt_add_user_svr (binding_handle,
				  	    &ctx_handle,
				  	    &to_pkss,
				  	    &from_pkss_p,
				  	    &estatus);

        DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from sec_pkss_mgmt_add_user_svr : 0x%x", estatus));

        DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from sec_pkss_mgmt_add_user_svr : 0x%x", cstatus));

      break;

    case PKSS_PROTO_MGMT_MOD_USR_SVR:

      cstatus = sec_pkss_mgmt_mod_user_svr (binding_handle,
				  	    &ctx_handle,
				  	    &to_pkss,
				  	    &from_pkss_p,
				  	    &estatus);

        DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from sec_pkss_mgmt_mod_user_svr : 0x%x", estatus));

        DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from sec_pkss_mgmt_mod_user_svr : 0x%x", cstatus));

      break;

    default:
      dce_svc_printf(PKS_S_INVALID_ARG_MSG);
      estatus = pks_s_invalid_arg;
      goto rundown;

  } // End switch (proto_id)

  if (cstatus != rpc_s_ok)
    estatus = cstatus;

  rpc_binding_free (&binding_handle, &cstatus);

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Return from the PKSS.  The contents of the output buffer are
  // an ASN.1-encoded bit string of encrypted data.  Move the RPC output data
  // to the ASN.1 buffer object in anticipation ASN.1 translation.
  //

  from_asn_cipher.data = from_pkss_p->msg_p;
  from_asn_cipher.data_length = from_pkss_p->msg_len;

  //
  // Instantiate ASN.1 translation object given by proto_id for encrypted
  // mgmt (server generates keys) protocol phase 4 message [V:Kou:Status]D
  // received from the PKSS.
  //

  switch (proto_id)
  {
    case PKSS_PROTO_MGMT_ADD_USR_SVR:
      mgmtSvrP4MsgP = new PkssMgmtSvrAddP4MsgC;
      break;

    case PKSS_PROTO_MGMT_MOD_USR_SVR:
      mgmtSvrP4MsgP = new PkssMgmtSvrModP4MsgC;
      break;

    default:
      dce_svc_printf(PKS_S_INVALID_ARG_MSG);
      estatus = pks_s_invalid_arg;
      goto rundown;

  } // End switch (proto_id)

  if (!(mgmtSvrP4MsgP))
  {
    dce_svc_printf(PKS_S_NO_MEMORY_MSG);
    estatus = pks_s_no_memory;
    goto rundown;
  }

  //
  // ASN.1-decode [V:Kou:Status]D upon return from the PKSS.
  //

  estatus = mgmtSvrP4MsgP->read(from_asn_cipher);

  if (estatus != 0)
  {
    goto rundown;
  }

  estatus = mgmtSvrP4MsgP->get_value (ciphertext_in.data, ciphertext_in.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from ASN get_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Zero and release from_pkss.
  //

  sec_pkss_zero_and_free (&from_pkss_p);

  //
  // Decrypt [V:Kou:status]D with D to obtain V:Kou:status.
  //

  estatus = sec_bsafe_symmetric_decrypt (sym_alg_id_pad,
                                         &dh_key,
                                         sym_pad_iv_p,
                                         &ciphertext_in,
                                         &plaintext_out);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from sec_bsafe_symmetric_decrypt : 0x%x", estatus));

  if (estatus != 0)
  {
    dce_svc_printf(PKS_S_SEC_BSAFE_MSG, estatus);
    goto rundown;
  }

  //
  // Move decrypted data from struct pointed to by plaintext to ASN.1 buffer
  // object to RPC input buffer in anticipation of populating the ASN.1
  // translation object.
  //

  from_asn_plain.data = plaintext_out.data;
  from_asn_plain.data_length = plaintext_out.len;

  //
  // ASN.1-decode V:Kou:status.
  //

  estatus = mgmtSvrP4Data.read(from_asn_plain);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from ASN read : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Zero and release contents of plaintext.
  //

  sec_pkss_zero_and_free (&plaintext_out);

  //
  // Obtain return parameters.
  //

  estatus = mgmtSvrP4Data.status.get_value (pstatus); // status

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from ASN get_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  if (pstatus != 0)
  {
    estatus = pstatus;
    goto rundown;
  }

  estatus = mgmtSvrP4Data.version.get_value (*new_key_vno_p); // V

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from ASN get_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Write the public key data into ASN.1 buffer object.  Recall that this 
  // layer of software leaves the public key data in ASN.1 form for use by 
  // the calling layer.  Note that it is the caller's responsibility to 
  // clear and free the allocated storage.
  //

  estatus = mgmtSvrP4Data.kou.write (pub_key_data);
 
    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from ASN write : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  new_pub_key_p->data = pub_key_data.data;
  new_pub_key_p->len = pub_key_data.data_length;

  pub_key_data.detach_buffer();

  //
  // Exit this function.
  //

  return (estatus);

rundown:

  sym_alg_id = 0;
  sec_pkss_zero_and_free (&dh_key);
  sym_alg_id_pad = 0;
  sym_alg_id_nopad = 0;
  sym_pad_key_len = 0;
  sym_nopad_key_len = 0;
  sec_pkss_zero_and_free (&sym_pad_iv_p);
  sec_pkss_zero_and_free (&sym_nopad_iv_p);
  sec_pkss_zero_and_free (&plaintext_out);
  sec_pkss_zero_and_free (&ciphertext_out);
  sec_pkss_zero_and_free (&from_pkss_p);

  if (uuid_string_p) {
    (void) memset (uuid_string_p, 0, strlen ((const char *)uuid_string_p));
    rpc_string_free (&uuid_string_p, &estatus);
    uuid_string_p = 0;
  }

  if (mgmtSvrP3MsgP)
  {
    delete mgmtSvrP3MsgP;
    mgmtSvrP3MsgP = 0;
  }

  if (mgmtSvrP4MsgP)
  {
    delete mgmtSvrP4MsgP;
    mgmtSvrP4MsgP = 0;
  }

  rpc_binding_free (&binding_handle, &cstatus);

  //
  // Exit this function.
  //

  return (estatus);

} // End sec_pkss_mgmt_svr_gen_util


//
// Add a new user (client generates asymmetric key pair).
//
// Calls: 	sec_pkss_mgmt_clt_gen_util
//

error_status_t sec_pkss_mgmt_user_add_clt
(
  const char *pkss_svc_profile_name_p,
  const char *name_p,
  sec_pk_domain_t domain_id,
  sec_pk_usage_flags_t key_usages,
  unsigned32 key_vno,
  sec_pk_pvtkey_t *pvt_key_p,
  sec_pk_pubkey_t *pub_key_p,
  const char *pwd_p,
  unsigned32 *new_key_vno_p
)

{
  //
  // Local variables
  //

  error_status_t estatus = 0;

  //
  // Set operation ID "I" to indicate "add new user - client generates keys".
  //

  unsigned32 proto_id = PKSS_PROTO_MGMT_ADD_USR_CLT;

  estatus = sec_pkss_mgmt_clt_gen_util (pkss_svc_profile_name_p,
					proto_id,
					name_p,
					domain_id,
					key_usages,
					key_vno,
					pvt_key_p,
					pub_key_p,
					pwd_p,
					new_key_vno_p);
  //
  // Exit this function.
  //

  return (estatus);

} // End sec_pkss_mgmt_user_add_clt


//
// Add a new user (server generates asymmetric key pair).
//
// Calls: 	sec_pkss_mgmt_svr_gen_util
//

error_status_t sec_pkss_mgmt_user_add_svr
(
  const char *pkss_svc_profile_name_p,
  const char *name_p,
  sec_pk_domain_t domain_id,
  sec_pk_usage_flags_t key_usages,
  unsigned32 key_vno,
  const char *pwd_p,
  sec_pk_algorithm_id_t *pk_alg_id_p,
  unsigned32 pk_key_len,
  sec_pk_pubkey_t *new_pub_key_p,
  unsigned32 *new_key_vno_p
)

{

  //
  // Local variables
  //

  error_status_t estatus = 0;
  unsigned32 proto_id;

  //
  // Set operation ID "I" to indicate "add new user - server generates keys".
  //

  proto_id = PKSS_PROTO_MGMT_ADD_USR_SVR;

  estatus = sec_pkss_mgmt_svr_gen_util (pkss_svc_profile_name_p,
					proto_id,
					name_p,
					domain_id,
					key_usages,
					key_vno,
					pwd_p,
					pk_alg_id_p,
					pk_key_len,
					new_pub_key_p,
					new_key_vno_p);
  //
  // Exit this function.
  //

  return (estatus);

} // End sec_pkss_mgmt_user_add_svr


//
// Delete a user.  Recall that the delete operation does NOT require
// a Diffie-Hellman key negotiation since this function does not require
// data privacy.  Recall further that since this operation executes in one
// RPC, no context handle is necessary.
//
// PKSS RPC's: 	sec_pkss_mgmt_del_user
//

error_status_t sec_pkss_mgmt_user_del
(
  const char *pkss_svc_profile_name_p,
  const char *name_p,
  sec_pk_domain_t domain_id,
  sec_pk_usage_flags_t key_usages,
  unsigned32 key_vno
)

{

  //
  // Local variables
  //

  error_status_t estatus = 0;
  unsigned32 proto_id;
  PkssMgmtDelP3MsgC mgmtDelP3Msg;
  unsigned char *uuid_string_p = 0;
  buffer_t to_asn;
  sec_pkss_msg_buf_t to_pkss = {0, 0};
  sec_pkss_msg_buf_t *from_pkss_p = 0;
  error_status_t cstatus = 0;
  r_buffer_t from_asn;
  PkssMgmtDelP4MsgC mgmtDelP4Msg;

  estatus = sec_pkss_count_usages (key_usages);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from sec_pkss_count_usages : 0x%x", estatus));

  if (estatus != 0)
  {
    estatus = sec_pvtkey_multiple_key_usages;
    return (estatus);
  }

  //
  // Obtain secure binding handle for mgmt operations.
  //

  rpc_binding_handle_t binding_handle;
  estatus = sec_pkss_bind (pkss_svc_profile_name_p,
			   sec_pkss_mgmt_if_v1_0_c_ifspec,
			   &binding_handle);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from sec_pkss_bind : 0x%x", estatus));

  if (estatus != 0)
  {
    return (estatus);
  }

  estatus = sec_pkss_secure_clt_handle (binding_handle);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from sec_pkss_secure_clt_handle : 0x%x", estatus));

  if (estatus != 0)
  {
    rpc_binding_free (&binding_handle, &cstatus);
    return (estatus);
  }

  //
  // Set operation ID "I" to indicate "delete user".
  //

  proto_id = PKSS_PROTO_MGMT_DEL_USR;

  //
  // Initialize the ASN.1 translation object in anticipation
  // of translating delete user mgmt protocol phase 3 client message Username
  // to ASN.1 before encrypting.
  //

  estatus = mgmtDelP3Msg.userKey.uname.set_value (name_p);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from ASN set_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  uuid_to_string (&domain_id, &uuid_string_p, &estatus);

  if (estatus != 0)
  {
    goto rundown;
  }

  estatus = mgmtDelP3Msg.userKey.domain.set_value ((const char *)uuid_string_p);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from ASN set_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  estatus = mgmtDelP3Msg.userKey.usages.set_value (key_usages);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from ASN set_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  estatus = mgmtDelP3Msg.userKey.version.set_value (key_vno);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from ASN set_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // ASN.1-encode Username in anticipation of transmission to the PKSS.
  //

  estatus = mgmtDelP3Msg.write (to_asn);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from ASN write : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Move ASN.1-encoded data from ASN.1 buffer object to RPC input buffer in
  // anticipation of transmission to the PKSS.
  //

  to_pkss.msg_len = to_asn.data_length;
  to_pkss.msg_p = to_asn.data;

  //
  // Call PKSS to delete user.
  //

  cstatus = sec_pkss_mgmt_del_user (binding_handle,
				    &to_pkss,
				    &from_pkss_p,
				    &estatus);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from sec_pkss_mgmt_del_user : 0x%x", estatus));

  if (cstatus != rpc_s_ok)
    estatus = cstatus;

  rpc_binding_free (&binding_handle, &cstatus);

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Move data from RPC output buffer upon return from the PKSS.  Move 
  // the RPC output data to the ASN.1 buffer object in anticipation ASN.1 
  // translation.
  //

  from_asn.data = from_pkss_p->msg_p;
  from_asn.data_length = from_pkss_p->msg_len;

  //
  // ASN.1-decode Status and load output parameter.
  //

  estatus = mgmtDelP4Msg.read(from_asn);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from ASN read : 0x%x", estatus));

  estatus = mgmtDelP4Msg.status.get_value (estatus);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from ASN get_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Zero and release from_pkss.
  //

  sec_pkss_zero_and_free (&from_pkss_p);

  //
  // Exit this function.
  //

  return (estatus);

rundown:

  proto_id = 0;

  if (uuid_string_p) {
    (void) memset (uuid_string_p, 0, strlen ((const char *)uuid_string_p));
    rpc_string_free (&uuid_string_p, &estatus);
    uuid_string_p = 0;
  }

  sec_pkss_zero_and_free(&from_pkss_p);

  rpc_binding_free (&binding_handle, &cstatus);

  //
  // Exit this function.
  //

  return (estatus);

} // sec_pkss_mgmt_user_del


//
// Modify a user record (client generates asymmetric key pair).
//
// Calls: 	sec_pkss_mgmt_clt_gen_util
//

error_status_t sec_pkss_mgmt_user_mod_clt
(
  const char *pkss_svc_profile_name_p,
  const char *name_p,
  sec_pk_domain_t domain_id,
  sec_pk_usage_flags_t key_usages,
  unsigned32 key_vno,
  sec_pk_pvtkey_t *pvt_key_p,
  sec_pk_pubkey_t *pub_key_p,
  const char *pwd_p,
  unsigned32 *new_key_vno_p
)

{

  //
  // Local variables
  //

  error_status_t estatus = 0;
  unsigned32 proto_id;

  //
  // Set operation ID "I" to indicate "modify user - client generates keys".
  //

  proto_id = PKSS_PROTO_MGMT_MOD_USR_CLT;

  estatus = sec_pkss_mgmt_clt_gen_util (pkss_svc_profile_name_p,
					proto_id,
					name_p,
					domain_id,
					key_usages,
					key_vno,
					pvt_key_p,
					pub_key_p,
					pwd_p,
					new_key_vno_p);
  //
  // Exit this function.
  //

  return (estatus);

} // End sec_pkss_mgmt_user_mod_clt


//
// Modify a user record (server generates asymmetric key pair).
//
// Calls: 	sec_pkss_mgmt_do_dh
//
// PKSS RPC's: 	sec_pkss_mgmt_mod_user_svr
//
error_status_t sec_pkss_mgmt_user_mod_svr
(
  const char *pkss_svc_profile_name_p,
  const char *name_p,
  sec_pk_domain_t domain_id,
  sec_pk_usage_flags_t key_usages,
  unsigned32 key_vno,
  const char *pwd_p,
  sec_pk_algorithm_id_t *pk_alg_id_p,
  unsigned32 pk_key_len,
  sec_pk_pubkey_t *new_pub_key_p,
  unsigned32 *new_key_vno_p
)

{

  //
  // Local variables
  //

  error_status_t estatus = 0;
  unsigned32 proto_id;

  //
  // Set operation ID "I" to indicate "modify user - server generates keys".
  //

  proto_id = PKSS_PROTO_MGMT_MOD_USR_SVR;

  estatus = sec_pkss_mgmt_svr_gen_util (pkss_svc_profile_name_p,
					proto_id,
					name_p,
					domain_id,
					key_usages,
					key_vno,
					pwd_p,
					pk_alg_id_p,
					pk_key_len,
					new_pub_key_p,
					new_key_vno_p);

  //
  // Exit this function.
  //

  return (estatus);

} // End sec_pkss_mgmt_user_mod_svr

