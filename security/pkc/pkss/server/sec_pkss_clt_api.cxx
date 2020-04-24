//
// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// All Rights Reserved
//
// HISTORY
// $Log: sec_pkss_clt_api.cxx,v $
// Revision 1.1.2.1  1996/10/04  14:22:48  arvind
// 	OSF DCE 1.2.2 Drop 4
// 	[1996/10/04  13:45:21  arvind]
//
// 	OSF DCE 1.2.2 Drop 4
//
// Revision /main/DCE_1.2.2/1  1996/09/09  21:29 UTC  arvind
// 	PKSS drop from DEC (DCE1.2.2)
// 	[1996/08/30  15:51 UTC  arvind  /main/arvind_pkss/1]
// 
// $EndLog$
//
// (c) Copyright 1996,
// Digital Equipment Corporation, Maynard, Massachusetts, USA
// All Rights Reserved
//
// PKSS client API functions - see sec_pkss_clt_api.h for input/output
// descriptions
//

#include <stdlib.h>
#include <dce/rpc.h>
#include <dce/dcesvcmsg.h>
#include "sec_bsafe.h"
#include <dce/sec_pk_base.h>
#include "sec_pkss_asn.h"
#include "sec_pkss_lits.h"
#include "sec_pkss_util.h"
#include "sec_pkss_clt_if.h"
#include "sec_pkss_clt_api.h"
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
// Improved encrypted key exchange.  Used by user key acquisition protocol
// and user key/password change protocols.  This function is NOT part of
// the exposed client interface, but is intended solely for use by the
// functions listed below.
//
// Called by:  sec_pkss_get_key
//             sec_pkss_key_chg_clt
//             sec_pkss_key_chg_svr
//
// PKSS RPC's:  sec_pkss_clt_diffhell
//              sec_pkss_clt_pkey_get
//


error_status_t sec_pkss_clt_ieke
(
  const char *pkss_svc_profile_name_p,
  const char *name_p,
  sec_pk_domain_t domain_id,
  sec_pk_usage_flags_t key_usages,
  unsigned32 key_vno,
  const char *pwd_p,
  unsigned32 proto_id,		// 1 of key aquisition or key chg svr or clt
  sec_pk_pvtkey_t *pvt_key_p,
  sec_pk_pubkey_t *pub_key_p,
  unsigned32 *ret_key_vno_p,
  sec_pkss_ctx_hdl_t *ctx_handle_p,
  rpc_binding_handle_t *handle_p,
  sec_pk_gen_data_t *h1_p,
  sec_pk_gen_data_t *h2_p,
  sec_bsafe_alg_type_t *sym_alg_id_pad_p,
  sec_bsafe_alg_type_t *sym_alg_id_nopad_p,
  unsigned32 *sym_pad_key_len_p,
  unsigned32 *sym_nopad_key_len_p,
  sec_pk_gen_data_t **sym_pad_iv_p,
  sec_pk_gen_data_t **sym_nopad_iv_p,
  sec_pk_gen_data_t *s_key_p,
  sec_pk_gen_data_t *clt_nonce_p
)

{
  //
  // Local variables
  //

  error_status_t estatus = 0;
  error_status_t cstatus = 0;
  unsigned i = 0;				// looping variable
  PkssUsrGetKeyP1MsgBaseC *usrGetKeyP1MsgP = 0; // ptr to ASN.1 phase 1 obj
  void *dh_context_p = 0;			// pointer to D-H context
  sec_pk_gen_data_t clt_dh_val = {0, 0};	// D-H data
  unsigned char *uuid_string_p = 0;
  unsigned32 clt_algs_list[PKSS_NUM_CRYPTO_ALGS];// all crypto algs avail to clt
  buffer_t to_asn_plain(SECURE);		// ASN.1 buffer
  sec_pkss_msg_buf_t to_pkss = {0, 0};		// msg buffer for ASN.1 data
  sec_pkss_msg_buf_t *from_pkss_p = 0;		// ASN.1 msg buf ptr from PKSS
  r_buffer_t from_asn_plain;			// ASN.1 return buffer
  PkssUsrGetKeyP2MsgBaseC *usrGetKeyP2MsgP = 0;	// ptr to ASN.1 phase 2 obj
  sec_pk_gen_data_t pkss_dh_val = {0, 0};	// D-H parms x-lated from ASN.1
  sec_pk_gen_data_t s_h2_algs_d = {0, 0};	// hold algorithm
  sec_pk_gen_data_t pkss_nonce_s = {0, 0};			
  sec_bsafe_alg_type_t sym_alg_id = 0;		// encryption algorithm ident
  unsigned32 sym_alg_idx = 0;
  sec_pk_gen_data_t dh_key = {0, 0};				
  PkssUsrGetKeyP2DataF2C usrGetKeyP2DataF2;	// ASN.1 phase 2 translation obj
  sec_pk_gen_data_t s_h2 = {0, 0};				
  sec_pk_gen_data_t pkss_h_algs = {0, 0};
  sec_pk_gen_data_t clt_h_algs = {0, 0};	// client algs list hash 
  sec_pk_gen_data_t clt_algs = {0, 0};		// client algs list
  sec_pk_gen_data_t pkss_nonce = {0, 0};
  PkssUsrGetKeyP3DataC usrGetKeyP3Data;		// ASN.1 phase 3 object
  sec_pk_gen_data_t plaintext = {0, 0};
  sec_pk_gen_data_t ciphertext = {0, 0};
  PkssUsrGetKeyP3MsgBaseC *usrGetKeyP3MsgP = 0;	// ptr to ASN.1 phase 3 object 
  buffer_t to_asn_cipher;			// ASN.1 encode buffer
  r_buffer_t from_asn_cipher;			// ASN.1 buf for data from PKSS 
  PkssUsrGetKeyP4MsgBaseC *usrGetKeyP4MsgP = 0;	// ptr to ASN.1 phase 4 obj
  PkssUsrGetKeyP4DataC usrGetKeyP4Data;		// ASN.1 data translation object
  sec_pk_gen_data_t kxu_h1 = {0, 0};		// output params 
  buffer_t kou_buf;
  sec_pk_gen_data_t clt_nonce_from_pkss = {0, 0};
  sec_pk_gen_data_t h1_key = {0, 0};
  buffer_t clt_algs_buf;
  unsigned32 clt_alg_idx = 0;

  //
  // Initialize the pks_svc_handle, if not done already
  //

  estatus = sec_pkss_init_svc_handle ();

  if (estatus != 0)
    return (estatus);

  //
  // If more than one usage flags bit is set, return an error.
  //

  estatus = sec_pkss_count_usages (key_usages);

  if (estatus != 0)
  {
    estatus = sec_pvtkey_multiple_key_usages;
    return (estatus);
  }

  //
  // Obtain a client (login) interface  binding handle to an
  // up-and-running PKSS.
  //

  estatus = sec_pkss_bind (pkss_svc_profile_name_p,
			   sec_pkss_clt_if_v1_0_c_ifspec,
			   handle_p);

  if (estatus != 0)
  {
    return (estatus);
  }

#ifndef USE_BELLOVIN_MERRITT_PROTOCOL

  estatus = sec_pkss_secure_clt_handle (*handle_p);

  DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                "status from sec_pkss_secure_clt_handle : 0x%x", estatus));

  if (estatus != 0)
  {
    rpc_binding_free (handle_p, &cstatus);
    return (estatus);
  }

#endif
 
  //
  // Instantiate ASN.1 translation object given by proto_id for user key
  // acquisition protocol phase 1 message to be sent to PKSS.
  //

  switch (proto_id)
  {
    case PKSS_PROTO_USR_KEY_GET:
    case PKSS_PROTO_USR_KEY_CHG_CLT:
    case PKSS_PROTO_USR_KEY_CHG_SVR:
      usrGetKeyP1MsgP = new PkssLoginP1MsgC;
      break;

    default:
      dce_svc_printf(PKS_S_INVALID_ARG_MSG);
      estatus = pks_s_invalid_arg;
      goto rundown;

  } // End switch (proto_id)

  if (!(usrGetKeyP1MsgP))
  {
    dce_svc_printf(PKS_S_NO_MEMORY_MSG);
    estatus = pks_s_no_memory;
    goto rundown;
  }

  //
  // Obtain exponentiated D-H client value E(Dc).
  //

  estatus = sec_pkss_get_dh_val (&dh_context_p, &clt_dh_val);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_pkss_get_dh_val : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Initialize the ASN.1 translation object in anticipation of translating
  // user key acquisition protocol phase 1 message username:E(Dc):algList
  // to ASN.1.
  //

  estatus = usrGetKeyP1MsgP->userKey.uname.set_value (name_p);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
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

  estatus = usrGetKeyP1MsgP->userKey.domain.set_value
						((const char *)uuid_string_p);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN set_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  estatus = usrGetKeyP1MsgP->userKey.usages.set_value (key_usages);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN set_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  estatus = usrGetKeyP1MsgP->userKey.version.set_value (key_vno);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN set_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  estatus = usrGetKeyP1MsgP->eDc.set_value (clt_dh_val.data,
					    clt_dh_val.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
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
    usrGetKeyP1MsgP->algList.new_child ();
    usrGetKeyP1MsgP->algList[i]->set_value(pkss_master_alg_list[i]);
    clt_algs_list[i] = pkss_master_alg_list[i];
  }

  //
  // ASN.1-encode username:E(Dc):algList for transmission to the PKSS.
  //

  estatus = usrGetKeyP1MsgP->write(to_asn_plain);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN write : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Zero and delete instance of ASN.1 translation object after writing
  // ASN.1-encoded data to ASN.1 buffer object.  Also zero and free the uuid
  // string.
  //

  (void) memset (uuid_string_p, 0, strlen ((const char *)uuid_string_p));
  rpc_string_free (&uuid_string_p, &estatus);
  uuid_string_p = 0;

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Move data from ASN.1 buffer object to RPC input buffer in
  // anticipation of transmission to the PKSS.
  //

  to_pkss.msg_len = to_asn_plain.data_length;
  to_pkss.msg_p = to_asn_plain.data;

  //
  // Talk to PKSS.
  //

  cstatus = sec_pkss_clt_diffhell (*handle_p,
				   ctx_handle_p,
				   &to_pkss,
				   &from_pkss_p,
				   &estatus);

  if (cstatus != rpc_s_ok)
    estatus = cstatus;

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_pkss_clt_diffhell : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Clear to_asn_plain in anticipation of its re-use below.
  //

  to_asn_plain.clear();

  //
  // Move the RPC output data to the ASN.1 buffer object in anticipation 
  // ASN.1 translation.
  //

  from_asn_plain.data = from_pkss_p->msg_p;
  from_asn_plain.data_length = from_pkss_p->msg_len;

  //
  // Instantiate ASN.1 translation object given by proto_id for user key
  // acquisition protocol phase 2 message E(Dp):[[S]H2:H(algs)]D:[Np]S:alg
  // received from PKSS.
  //

  switch (proto_id)
  {
    case PKSS_PROTO_USR_KEY_GET:
    case PKSS_PROTO_USR_KEY_CHG_CLT:
    case PKSS_PROTO_USR_KEY_CHG_SVR:
      usrGetKeyP2MsgP = new PkssLoginP2MsgC;
      break;

    default:
      dce_svc_printf(PKS_S_INVALID_ARG_MSG);
      estatus = pks_s_invalid_arg;
      goto rundown;

  } // End switch (proto_id)

  if (!(usrGetKeyP2MsgP))
  {
    dce_svc_printf(PKS_S_NO_MEMORY_MSG);
    estatus = pks_s_no_memory;
    goto rundown;
  }

  //
  // ASN.1-decode E(Dp):[[S]H2:H(algs)]D:[Np]S:alg upon return from the PKSS.
  //

  estatus = usrGetKeyP2MsgP->read(from_asn_plain);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN read : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Obtain output parameters translated from ASN.1.
  //

  estatus = usrGetKeyP2MsgP->eDp.get_value (pkss_dh_val.data, pkss_dh_val.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN get_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  estatus = usrGetKeyP2MsgP->sH2HalgsD.get_value (s_h2_algs_d.data,
						  s_h2_algs_d.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN get_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  estatus = usrGetKeyP2MsgP->nps.get_value (pkss_nonce_s.data,
					    pkss_nonce_s.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN get_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  estatus = usrGetKeyP2MsgP->algIdx.get_value (sym_alg_idx);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN get_value : 0x%x", estatus));

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
					  sym_alg_id_pad_p,
					  sym_alg_id_nopad_p,
					  sym_pad_key_len_p,
					  sym_nopad_key_len_p,
					  sym_pad_iv_p,
					  sym_nopad_iv_p);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_pkss_get_sym_crypto_info : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  if (from_pkss_p)
  {
    if (from_pkss_p->msg_p)
    {
      (void) memset (from_pkss_p->msg_p, 0, from_pkss_p->msg_len);
      from_pkss_p->msg_len = 0;
      free (from_pkss_p->msg_p);
    }
    free (from_pkss_p);
    from_pkss_p = 0;
  };

  //
  // Compute D-H key D using E(Dc) and E(Dp).
  //

  dh_key.len = *sym_pad_key_len_p;
  dh_key.data = (unsigned char *) calloc (1, dh_key.len);

  if (!(dh_key.data))
  {
    dce_svc_printf(PKS_S_NO_MEMORY_MSG);
    goto rundown;
  }

  estatus = sec_bsafe_diffie_hellman_phase_2 (&dh_context_p,
					      &pkss_dh_val,
					      &dh_key);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
             "status from sec_bsafe_diffie_hellman_phase_2 : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Zero contents of pkss_dh_val and clt_dh_val.
  //

  sec_pkss_zero_and_free (&clt_dh_val);

  //
  // Decrypt [[S]H2:H(algs)]D with D-H key D using algorithm "alg" to
  // obtain [S]H2:H(algs).
  //

  estatus = sec_bsafe_symmetric_decrypt (*sym_alg_id_pad_p,
					 &dh_key,
					 *sym_pad_iv_p,
					 &s_h2_algs_d,
					 &plaintext);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_bsafe_symmetric_decrypt : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Zero and release the contents of the Diffie-Hellman key.
  //

  sec_pkss_zero_and_free (&dh_key);

  //
  // Move decrypted data from struct pointed to by plaintext to ASN.1 buffer
  // object in anticipation of populating the ASN.1 translation object.
  //

  from_asn_plain.data = plaintext.data;
  from_asn_plain.data_length = plaintext.len;

  //
  // ASN.1-decode [S]H2:H(algs).
  //

  estatus = usrGetKeyP2DataF2.read(from_asn_plain);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN read : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Obtain values of [S]H2:H(algs).
  //

  estatus = usrGetKeyP2DataF2.sH2.get_value (s_h2.data, s_h2.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN get_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  estatus = usrGetKeyP2DataF2.hAlgs.get_value (pkss_h_algs.data,
					       pkss_h_algs.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN get_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }


  //
  // Zero and release contents of plaintext.  Note that this also zeroes
  // from_asn_plain (which is OK, since that's an r-Buffer_t which doesn't
  // consider itself as owning any memory).
  //
  
  sec_pkss_zero_and_free (&plaintext);

  //
  // Compute algList hash H(algList) on client's copy of the crypto algorithm
  // list.  Compare result with H(algs) returned from the PKSS.  If they do
  // not match, assume a message stream modification attack and discontinue
  // processing.
  //

  estatus = usrGetKeyP1MsgP->algList.write (clt_algs_buf);

  if (estatus != 0)
  {
    goto rundown;
  }

  clt_algs.data = clt_algs_buf.data;
  clt_algs.len = clt_algs_buf.data_length;

  estatus = sec_bsafe_compute_hash (sec_bsafe_hash_MD2,
				    &clt_algs,
				    &clt_h_algs);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_bsafe_compute_hash : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }


  estatus = sec_pkss_pk_gen_data_cmp (&pkss_h_algs, &clt_h_algs);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_pkss_pk_gen_data_cmp : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Zero and release contents of clt_h_algs.
  //

  sec_pkss_zero_and_free (&clt_h_algs);

  //
  // Obtain hashes H1 and H2 from pwd_p.   Note that it is the caller's
  // responsibility to free the storage allocated to contain the hashes.
  //

  estatus = sec_pkss_get_pwd_hashes (pwd_p,
				     name_p,
				     *sym_pad_key_len_p,
				     h1_p,
				     h2_p);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_pkss_get_pwd_hashes : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Decrypt [S]H2 with H2 using algorithm "alg" to obtain session key S.
  // Recall that the PKSS did not ASN.1-encode S before encryption.  Zero
  // contents of H2 on a key acquisition operation.
  //

  estatus = sec_bsafe_symmetric_decrypt (*sym_alg_id_nopad_p,
					 h2_p,
					 *sym_nopad_iv_p,
					 &s_h2,
					 s_key_p);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_bsafe_symmetric_decrypt : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  if (proto_id == PKSS_PROTO_USR_KEY_GET)
  {
    sec_pkss_zero_and_free (h2_p);
  }

  //
  // Decrypt [Np]S to obtain server nonce Np.  Recall that the PKSS did not
  // ASN.1-encode Np before encryption.
  //

  estatus = sec_bsafe_symmetric_decrypt (*sym_alg_id_nopad_p,
					 s_key_p,
					 *sym_nopad_iv_p,
					 &pkss_nonce_s,
					 &pkss_nonce);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_bsafe_symmetric_decrypt : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Generate a nonce Nc.
  //

  clt_nonce_p->len = PKSS_NONCE_SIZE;
  clt_nonce_p->data = (unsigned char *) calloc (1, clt_nonce_p->len);

  if (!(clt_nonce_p->data))
  {
    dce_svc_printf(PKS_S_NO_MEMORY_MSG);
    estatus = pks_s_no_memory;
    goto rundown;
  }

  estatus = sec_bsafe_random_bytes (0, clt_nonce_p);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_bsafe_random_bytes : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Initialize the ASN.1 translation object in anticipation
  // of translating user key acquisition protocol phase 3 message I:Np:Nc
  // to ASN.1 before encrypting.
  //

  estatus = usrGetKeyP3Data.protoID.set_value (proto_id);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN set_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  estatus = usrGetKeyP3Data.np.set_value (pkss_nonce.data, pkss_nonce.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN set_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  estatus = usrGetKeyP3Data.nc.set_value (clt_nonce_p->data, clt_nonce_p->len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN set_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // ASN.1-encode I:Np:Nc prior to encryption.
  //

  estatus = usrGetKeyP3Data.write(to_asn_plain);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN write : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Zero contents of ASN.1 translation object usrGetKeyP3Data after writing
  // ASN.1-encoded data to ASN.1 buffer object.
  //

  //
  // Move contents of ASN.1 buffer object to sec_pk_gen_data_t struct in
  // anticipation of encrypting it.
  //
  // JCW: The next two lines are slightly dangerous, since plaintext is
  //      something that the error path(s) will want to zero&free, but
  //	  to_asn_plain will also zero/free the data.  However, if we
  //	  clear plaintext before we check for errors, we can get away with
  //	  this.
  //

  plaintext.data = to_asn_plain.data;
  plaintext.len = to_asn_plain.data_length;

  //
  // Encrypt I:Np:Nc with S to obtain [Nc:Np:I]S.
  //

  estatus = sec_bsafe_symmetric_encrypt (*sym_alg_id_pad_p,
					 s_key_p,
					 *sym_pad_iv_p,
					 &plaintext,
					 &ciphertext);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_bsafe_symmetric_encrypt : 0x%x", estatus));

  //
  // JCW: Added the following lines, since zeroing the data described by
  //      plaintext is already taken care of by to_asn_plain, and we don't
  //	  want to do it twice if we take an error.
  //

  plaintext.data = NULL;
  plaintext.len = 0;

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Zero and free contents of pkss_nonce.
  //

  sec_pkss_zero_and_free (&pkss_nonce);

  //
  // Instantiate ASN.1 translation object given by proto_id for encrypted
  // user key acquisition protocol phase 3 message to be sent to PKSS.
  //

  switch (proto_id)
  {
    case PKSS_PROTO_USR_KEY_GET:
    case PKSS_PROTO_USR_KEY_CHG_CLT:
    case PKSS_PROTO_USR_KEY_CHG_SVR:
      usrGetKeyP3MsgP = new PkssLoginP3MsgC;
      break;

    default:
      dce_svc_printf(PKS_S_INVALID_ARG_MSG);
      estatus = pks_s_invalid_arg;
      goto rundown;

  } // End switch (proto_id)

  if (!(usrGetKeyP3MsgP))
  {
    dce_svc_printf(PKS_S_NO_MEMORY_MSG);
    estatus = pks_s_no_memory;
    goto rundown;
  }

  //
  // Initialize the ASN.1 translation object in anticipation of translating
  // user key acquisition protocol phase 3 message [I:Np:Nc]S to ASN.1.
  //

  estatus = usrGetKeyP3MsgP->set_value (ciphertext.data, ciphertext.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN set_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // ASN.1-encode [I:Np:Nc]S for transmission to PKSS.
  //

  estatus = usrGetKeyP3MsgP->write (to_asn_cipher);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN write : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Zero and delete instance of ASN.1 translation object after writing
  // ASN.1-encoded data to ASN.1 buffer object.  Also zero and delete
  // contents of ciphertext.
  //

  delete usrGetKeyP3MsgP;
  usrGetKeyP3MsgP = 0;

  sec_pkss_zero_and_free (&ciphertext);

  //
  // Move encrypted data from ASN.1 buffer object to RPC input buffer in
  // anticipation of transmission to the PKSS.
  //

  to_pkss.msg_len = to_asn_cipher.data_length;
  to_pkss.msg_p = to_asn_cipher.data;
 
  //
  // Obtain user's public key pair from PKSS.
  //

  cstatus = sec_pkss_clt_pkey_get (*handle_p,
			 	   ctx_handle_p,
			 	   &to_pkss,
			 	   &from_pkss_p,
			 	   &estatus);

  if (cstatus != rpc_s_ok)
    estatus = cstatus;

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_pkss_clt_pkey_get : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // The contents of the output buffer are an ASN.1-encoded bit string 
  // of encrypted data.  Move the RPC output data to the ASN.1 buffer object 
  // in anticipation ASN.1 translation.
  //

  from_asn_cipher.data = from_pkss_p->msg_p;
  from_asn_cipher.data_length = from_pkss_p->msg_len;

  //
  // Instantiate ASN.1 translation object given by proto_id for user key
  // acquisition protocol phase 4 message [[Kxu]H1:Kou:V:Nc]S received
  // from PKSS.
  //

  switch (proto_id)
  {
    case PKSS_PROTO_USR_KEY_GET:
    case PKSS_PROTO_USR_KEY_CHG_CLT:
    case PKSS_PROTO_USR_KEY_CHG_SVR:
      usrGetKeyP4MsgP = new PkssLoginP4MsgC;
      break;

    default:
      dce_svc_printf(PKS_S_INVALID_ARG_MSG);
      estatus = pks_s_invalid_arg;
      goto rundown;

  } // End switch (proto_id)

  if (!(usrGetKeyP4MsgP))
  {
    dce_svc_printf(PKS_S_NO_MEMORY_MSG);
    estatus = pks_s_no_memory;
    goto rundown;
  }

  //
  // ASN.1-decode [[Kxu]H1:Kou:V:Nc]S upon return from the PKSS.
  //

  estatus = usrGetKeyP4MsgP->read(from_asn_cipher);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN read : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  estatus = usrGetKeyP4MsgP->get_value (ciphertext.data, ciphertext.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN get_value : 0x%x", estatus));

  //
  // If necessary, clear ciphertext pointer since it points to memory
  // it doesn't own, and rundown will attempt to zero and free it.
  //

  if (estatus != 0)
  {
    ciphertext.data = 0;
    ciphertext.len = 0;
    goto rundown;
  }

  //
  // Release from_pkss.
  //

  if (from_pkss_p)
  {
    if (from_pkss_p->msg_p)
    {
      (void) memset (from_pkss_p->msg_p, 0, from_pkss_p->msg_len);
      from_pkss_p->msg_len = 0;
      free (from_pkss_p->msg_p);
    }
    free (from_pkss_p);
    from_pkss_p = 0;
  };

  //
  // Decrypt [[Kxu]H1:Kou:V:Nc]S with S to obtain [Kxu]H1:Kou:V:Nc.
  //
  
  estatus = sec_bsafe_symmetric_decrypt (*sym_alg_id_pad_p,
					 s_key_p,
					 *sym_pad_iv_p,
					 &ciphertext,
					 &plaintext);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_bsafe_symmetric_decrypt : 0x%x", estatus));

  //
  // JCW: Clear ciperhtext, since it points to memory it doesn't own, and
  //      if we take an error, rundown will try to release it.
  //

  ciphertext.data = NULL;
  ciphertext.len = 0;

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Move decrypted data from struct pointed to by plaintext to ASN.1 buffer
  // object to RPC input buffer in anticipation of populating the ASN.1
  // translation object.
  //

  from_asn_plain.data = plaintext.data;
  from_asn_plain.data_length = plaintext.len;

  //
  // ASN.1-decode [Kxu]H1:Kou:V:Nc.
  //

  estatus = usrGetKeyP4Data.read(from_asn_plain);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN read : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Zero and release plaintext.
  //

  sec_pkss_zero_and_free (&plaintext);

  //
  // Obtain output parameters translated from ASN.1.
  //

  estatus = usrGetKeyP4Data.kxuH1.get_value (kxu_h1.data, kxu_h1.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN get_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  estatus = usrGetKeyP4Data.version.get_value (*ret_key_vno_p);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN get_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  estatus = usrGetKeyP4Data.nc.get_value (clt_nonce_from_pkss.data,
					  clt_nonce_from_pkss.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN get_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Compare the Nc returned in clt_nonce_from_pkss from the PKSS with the
  // Nc generated in this function.  If the two do not match, assume an
  // attack on the network traffic, discontinue processing, and return an
  // error.
  //

  estatus = sec_pkss_pk_gen_data_cmp (clt_nonce_p, &clt_nonce_from_pkss);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_pkss_pk_gen_data_cmp : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Write public key data.  Recall that this layer of software leaves the
  // public key data in ASN.1 form for use by the calling layer.  Note that
  // it is the caller's responsibility to clear and free the allocated storage.
  //

  estatus = usrGetKeyP4Data.kou.write (kou_buf);
  
    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN write : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  pub_key_p->len = kou_buf.data_length;
  pub_key_p->data = (unsigned char *) calloc (1, kou_buf.data_length);

  if (!(pub_key_p->data))
  {
    dce_svc_printf(PKS_S_NO_MEMORY_MSG);
    goto rundown;
  }

  (void) memcpy (pub_key_p->data, kou_buf.data, kou_buf.data_length);

  //
  // Decrypt [Kxu]H1 with H1 to obtain the user's private key Kxu.  Zero
  // the contents of H1 on a key acquisition operation.
  //

  h1_key.data = h1_p->data;
  h1_key.len = h1_p->len;
  estatus = sec_bsafe_symmetric_decrypt (*sym_alg_id_pad_p,
					 &h1_key,
					 *sym_pad_iv_p,
					 &kxu_h1,
					 &plaintext); 

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_bsafe_symmetric_decrypt : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Load output parameters.
  //


  pvt_key_p->len = plaintext.len;
  pvt_key_p->data = (unsigned char *) calloc (1, plaintext.len);

  if (!(pvt_key_p->data))
  {
    dce_svc_printf(PKS_S_NO_MEMORY_MSG);
    goto rundown;
  }

  (void) memcpy (pvt_key_p->data, plaintext.data, plaintext.len);

  //
  // Zero and release everything not zeroed and released yet.
  //

  if (proto_id == PKSS_PROTO_USR_KEY_GET)
  {
    sec_pkss_zero (&h1_key);
  }

  sec_pkss_zero_and_free (&plaintext);

  //
  // Exit this function.
  //

  return (estatus);

  //
  // On error, come here to zero out all stack, heap data.  Static objects must
  // do their own cleanup when they are deleted.
  //

rundown:

  if (usrGetKeyP1MsgP)
  {
    delete usrGetKeyP1MsgP;
    usrGetKeyP1MsgP = 0;
  }

  sec_pkss_zero_and_free (&clt_dh_val);

  if (uuid_string_p) {
    (void) memset (uuid_string_p, 0, strlen ((const char *)uuid_string_p));
    rpc_string_free (&uuid_string_p, &estatus);
    uuid_string_p = 0;
  }

  (void) memset (&clt_algs_list, 0, sizeof(unsigned32) * PKSS_NUM_CRYPTO_ALGS);

  if (from_pkss_p)
  {
    if (from_pkss_p->msg_p)
    {
      (void) memset (from_pkss_p->msg_p, 0, from_pkss_p->msg_len);
      from_pkss_p->msg_len = 0;
      free (from_pkss_p->msg_p);
    }
    free (from_pkss_p);
    from_pkss_p = 0;
  };

  if (usrGetKeyP2MsgP) 
  {
    delete usrGetKeyP2MsgP;
    usrGetKeyP2MsgP = 0;
  }

  sym_alg_id = 0;
  sec_pkss_zero_and_free (&dh_key);
  sec_pkss_zero_and_free (&plaintext);
  sec_pkss_zero_and_free (&clt_h_algs);
  sec_pkss_zero_and_free (&pkss_nonce);
  sec_pkss_zero_and_free (&ciphertext);

  if (usrGetKeyP3MsgP)
  {
    delete usrGetKeyP3MsgP;
    usrGetKeyP3MsgP = 0;
  }

  if (usrGetKeyP4MsgP)
  {
    delete usrGetKeyP4MsgP;
    usrGetKeyP4MsgP = 0;
  }

  sec_pkss_zero (&h1_key);

  //
  // Exit this function.
  //

  return (estatus);

} // End sec_pkss_clt_ieke

//
// Retrieve user's public key pair for use in [RFC 68.2] login
//
// Calls: sec_pkss_clt_ieke
//

error_status_t sec_pkss_get_key
(
  const char *pkss_svc_profile_name_p,
  const char *name_p,
  sec_pk_domain_t domain_id,
  sec_pk_usage_flags_t key_usages,
  unsigned32 key_vno,
  const char *pwd_p,
  sec_pk_pvtkey_t *pvt_key_p,
  sec_pk_pubkey_t *pub_key_p,
  unsigned32 *ret_key_vno_p
)

{
  //
  // Local variables
  //

  error_status_t estatus = 0;
  unsigned32 proto_id = PKSS_PROTO_USR_KEY_GET;
  sec_pkss_ctx_hdl_t ctx_handle = 0;
  rpc_binding_handle_t binding_handle;
  sec_pk_gen_data_t h1 = {0, 0};
  sec_pk_gen_data_t h2 = {0, 0};
  sec_bsafe_alg_type_t sym_alg_id_pad = 0;
  sec_bsafe_alg_type_t sym_alg_id_nopad = 0;
  unsigned32 sym_pad_key_len = 0;
  unsigned32 sym_nopad_key_len = 0;
  sec_pk_gen_data_t *sym_pad_iv_p = 0;
  sec_pk_gen_data_t *sym_nopad_iv_p = 0;
  sec_pk_gen_data_t s_key = {0, 0};
  sec_pk_gen_data_t clt_nonce = {0, 0};

  //
  // Obtain public key pair using improved encrypted key exchange protocol.
  //

  estatus = sec_pkss_clt_ieke  (pkss_svc_profile_name_p,
				name_p,
				domain_id,
				key_usages,
				key_vno,
				pwd_p,
				proto_id,
				pvt_key_p,
				pub_key_p,
				ret_key_vno_p,
				&ctx_handle,
				&binding_handle,
				&h1,
				&h2,
				&sym_alg_id_pad,
				&sym_alg_id_nopad,
				&sym_pad_key_len,
				&sym_nopad_key_len,
				&sym_pad_iv_p,
				&sym_nopad_iv_p,
				&s_key,
				&clt_nonce);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_pkss_clt_ieke : 0x%x", estatus));

  proto_id = 0;
  (void) memset (&ctx_handle, 0, sizeof(sec_pkss_ctx_hdl_t));
  sym_alg_id_pad = 0;
  sym_alg_id_nopad = 0;
  sym_pad_key_len = 0;
  sym_nopad_key_len = 0;
  sec_pkss_zero_and_free (&h1);
  sec_pkss_zero_and_free (&h2);
  sec_pkss_zero_and_free (&s_key);
  sec_pkss_zero_and_free (&clt_nonce);
  sec_pkss_zero_and_free (&sym_pad_iv_p);
  sec_pkss_zero_and_free (&sym_nopad_iv_p);
  error_status_t cstatus = 0;
  rpc_binding_free (&binding_handle, &cstatus);
  
  //
  // Exit this function.
  //

  return (estatus);

} // End sec_pkss_get_key

//
// Change user's password and/or asymmetric key pair (client generates
// new asymmetric key pair).
//
// calls:  sec_pkss_clt_ieke
//         sec_bsafe_sign_data
//
// PKSS RPC's: 	sec_pkss_chg_key_clt
//

error_status_t sec_pkss_key_chg_clt
(
  const char *pkss_svc_profile_name_p,
  const char *name_p,
  sec_pk_domain_t domain_id,
  sec_pk_usage_flags_t key_usages,
  unsigned32 key_vno,
  const char *old_pwd_p,
  const char *new_pwd_p,
  sec_pk_pvtkey_t *new_pvt_key_p,
  sec_pk_pubkey_t *new_pub_key_p,
  unsigned32 *new_key_vno_p
)

{
  //
  // Local variables
  //

  error_status_t estatus = 0;
  error_status_t cstatus = 0;
  unsigned32 proto_id = PKSS_PROTO_USR_KEY_CHG_CLT;
  unsigned32 ret_key_vno = 0;
  sec_pkss_ctx_hdl_t ctx_handle = 0;
  rpc_binding_handle_t binding_handle = 0;
  sec_pk_gen_data_t old_h1 = {0, 0};
  sec_pk_gen_data_t old_h2 = {0, 0};
  sec_pk_gen_data_t new_h1 = {0, 0};
  sec_pk_gen_data_t new_h2 = {0, 0};
  sec_pk_pvtkey_t old_pvt_key = {0, 0};
  sec_pk_pubkey_t old_pub_key = {0, 0};
  sec_bsafe_alg_type_t sym_alg_id_pad = 0;
  sec_bsafe_alg_type_t sym_alg_id_nopad = 0;
  unsigned32 sym_pad_key_len = 0;
  unsigned32 sym_nopad_key_len = 0;
  sec_pk_gen_data_t *sym_pad_iv_p = 0;
  sec_pk_gen_data_t *sym_nopad_iv_p = 0;
  sec_pk_gen_data_t s_key = {0, 0};
  sec_pk_gen_data_t clt_nonce = {0, 0};
  sec_pk_gen_data_t newKxuH1 = {0, 0};	// new private key enc. w/H1
  sec_pk_gen_data_t newKxu = {0, 0};	// new private key
  PkssCltChgP5SignedC cltChgP5Signed;	// ASN.1 phase 5 translation object
  r_buffer_t pub_key_buf;
  buffer_t to_asn_plain(SECURE);	// ASN.1 buffer object
  sec_pk_gen_data_t toBeSigned = {0, 0}; // buffer for signing
  sec_pk_data_t signature = {0, 0};	// signed buffer ret'ed from sec_bsafe
  sec_bsafe_alg_type_t sig_alg = 0;	// sign algorithm type
  buffer_t to_asn_signed(SECURE);	// ASN.1 buffer object
  sec_pk_gen_data_t plaintext = {0, 0};	// buffer for to_asn_signed data
  sec_pk_gen_data_t ciphertext = {0, 0}; // buffer for returned encrypted data
  PkssCltChgP5MsgC cltChgP5Msg;		// ASN.1 phase 5 message object
  buffer_t to_asn_cipher;		// encrypt buffer to send to PKSS
  sec_pkss_msg_buf_t to_pkss = {0, 0};	// RPC message buffer to send to PKSS
  sec_pkss_msg_buf_t *from_pkss_p = 0;	// RPC message buffer ptr from PKSS
  r_buffer_t from_asn_cipher;		// ASN.1 buffer object for PKSS return
  PkssCltChgP6MsgC cltChgP6Msg;		// ASN.1 phase 6 message object
  r_buffer_t from_asn_plain;		// ASN.1 buffer to hold plaintext
  PkssCltChgP6DataC cltChgP6Data;	// ASN.1 phase 6 translation object
  sec_pk_gen_data_t clt_nonce_from_pkss = {0, 0};// output parameters from PKSS

  sec_pk_pvtkey_t loc_new_pvt_key = {0, 0};
  sec_pk_pubkey_t loc_new_pub_key = {0, 0};

// JCW: Copy the caller-supplied new keys, as we're going to modify them, and
//      they're advertised as being IN parameters.  Also this lets us be
//	tolerant of callers who pass us NULL for these keys.

  if (new_pvt_key_p == NULL) {
     loc_new_pvt_key.len = 0;
     loc_new_pvt_key.data = NULL;
  } else {
     loc_new_pvt_key.len = new_pvt_key_p->len;
     loc_new_pvt_key.data = new_pvt_key_p->data;
  };

  if (new_pub_key_p == NULL) {
     loc_new_pub_key.len = 0;
     loc_new_pub_key.data = NULL;
  } else {
     loc_new_pub_key.len = new_pub_key_p->len;
     loc_new_pub_key.data = new_pub_key_p->data;
  };

  //
  // Obtain public key pair using improved encrypted key exchange protocol.
  //

  estatus = sec_pkss_clt_ieke  (pkss_svc_profile_name_p,
				name_p,
				domain_id,
				key_usages,
				key_vno,
				old_pwd_p,
				proto_id,
				&old_pvt_key,
				&old_pub_key,
				&ret_key_vno,
				&ctx_handle,
				&binding_handle,
				&old_h1,
				&old_h2,
				&sym_alg_id_pad,
				&sym_alg_id_nopad,
				&sym_pad_key_len,
				&sym_nopad_key_len,
				&sym_pad_iv_p,
				&sym_nopad_iv_p,
				&s_key,
				&clt_nonce);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_pkss_clt_ieke : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // If the contents of old_pwd_p are different from the contents of
  // new_pwd_p, compute new hashes H1~ and H2~ from new_pwd_p; otherwise,
  // use previous hashes H1 and H2 derived from old_pwd_p returned by
  // sec_pkss_clt_ieke.  Note that it is the caller's responsibility to
  // zero the contents of old_pwd_p and new_pwd_p before releasing their
  // storage.
  //

  if ((strlen (old_pwd_p) != strlen (new_pwd_p)) ||
      (memcmp (old_pwd_p, new_pwd_p, strlen (new_pwd_p)) != 0))
  {
    estatus = sec_pkss_get_pwd_hashes (new_pwd_p,
				       name_p,
				       sym_pad_key_len,
				       &new_h1,
				       &new_h2);

  }
  else
  {
// JCW : Note that on this branch, new_h1/new_h2 point to the same memory
//       that old_h1/old_h2 do, whereas if the then-part was taken, they
//	 point to different memory.  We have to be careful about freeing
//	 them up correctly.
    new_h1.len = old_h1.len;
    new_h1.data = old_h1.data;
    new_h2.len = old_h2.len;
    new_h2.data = old_h2.data;
  }
      
  DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                 "status from sec_pkss_get_pwd_hashes : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // If the contents of loc_new_pvt_key and loc_new_pub_key are empty,
  // assume that only the password is changing and copy the old key pair
  // into their respective parameters.
  //

  if ((loc_new_pvt_key.len == 0) &&
      (loc_new_pub_key.len == 0))
  {
    loc_new_pvt_key.len = old_pvt_key.len;
    loc_new_pvt_key.data = old_pvt_key.data;
    loc_new_pub_key.len = old_pub_key.len;
    loc_new_pub_key.data = old_pub_key.data;
  }

  //
  // Encrypt the new private key Kxu~ with the new password hash H1~ to
  // obtain [Kxu~]H1~.  Zero and release contents of H1 and H1~.
  //

  newKxu.len = loc_new_pvt_key.len;
  newKxu.data = loc_new_pvt_key.data;

  estatus = sec_bsafe_symmetric_encrypt (sym_alg_id_pad,
                                         &new_h1,
                                         sym_pad_iv_p,
                                         &newKxu,
                                         &newKxuH1);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_bsafe_symmetric_encrypt : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

// JCW: Changed the following to only zero&free new_h1 if the memory it
//      describes is different to that described by old_h1

  if (old_h1.data != new_h1.data) 
      sec_pkss_zero_and_free (&new_h1);

  sec_pkss_zero_and_free (&old_h1);

  //
  // Initialize the ASN.1 translation object in anticipation
  // of translating user change key (client generates new keys) protocol phase
  // 5 data [Kxu~]H1~:H2~:Kou~ to ASN.1 before signing.
  //

  estatus = cltChgP5Signed.dataToBeSigned.newKxuH1.set_value (newKxuH1.data,
						    	      newKxuH1.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN set_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  estatus = cltChgP5Signed.dataToBeSigned.newH2.set_value (new_h2.data,
						           new_h2.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN set_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Recall that there will be no new public key component if only
  // the password is changing.
  //
// JCW: The above comment described the original protocol design; however
//      we don't lose anything if we always send the public-key, and 
//	error-checking the protocol is tighter if we have as few optional
//	elements within the protocol messages as we can, so we always send
//	the public-key, whether or not it's changed.  The server can tell
//	whether we're changing the key by comparing the old and new
//	public-key values, so it doesn't lose any information this way.

  pub_key_buf.data = loc_new_pub_key.data;
  pub_key_buf.data_length = loc_new_pub_key.len;
  estatus = cltChgP5Signed.dataToBeSigned.newKou.read (pub_key_buf);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                  "status from newKou.read : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // ASN.1-encode [Kxu~]H1~:H2~:Kou~ in anticipation of signing.
  //

  estatus = cltChgP5Signed.dataToBeSigned.write (to_asn_plain);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from dataToBeSigned.write : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Zero and release contents of newKxuH1.
  //

  sec_pkss_zero_and_free (&newKxuH1);

  //
  // Move contents of ASN.1 buffer object to sec_pk_gen_data_t struct in
  // anticipation of signing it.
  //

  toBeSigned.data = to_asn_plain.data;
  toBeSigned.len = to_asn_plain.data_length;

  //
  // Sign [Kxu~]H1~:H2~:Kou~ with the old private key Kxu to obtain
  // {[Kxu~]H1~:H2~:Kou~}Kxu.
  //

  sig_alg = sec_sign_md2rsa;

  // ****read and map sig_alg out of old_pvt_key.

  estatus = sec_bsafe_sign_data (sig_alg,
				 &toBeSigned,
				 &old_pvt_key,
				 &signature);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_bsafe_sign_data : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Zero and release the contents of old_pvt_key.  Zero contents of
  // toBeSigned.
  //

  sec_pkss_zero_and_free (&old_pvt_key);

  //
  // Initialize the rest of the ASN.1 translation object in anticipation
  // of translating user change key protocol phase 5 data [Kxu~]H1~:H2~:Kou~
  // to ASN.1.
  //

  estatus = cltChgP5Signed.sigAlg.set_value (sig_alg);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN set_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  estatus = cltChgP5Signed.signature.set_value (signature.data, signature.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN set_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // ASN.1-encode {[Kxu~]H1~:H2~:Kou~}Kxu for transmission to the PKSS.  
  // Zero and release contents of H2 and H2~.
  //

  estatus = cltChgP5Signed.write(to_asn_signed);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN write : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  sec_pkss_zero_and_free (&new_h2);
  sec_pkss_zero_and_free (&old_h2);

  //
  // Zero contents of cltChgP5Signed.  Zero and release contents of signature.
  //

  sec_pkss_zero_and_free (&signature);

  //
  // Move contents of ASN.1 buffer object to sec_pk_gen_data_t struct in
  // anticipation of encrypting it.
  //

  plaintext.data = to_asn_signed.data;
  plaintext.len = to_asn_signed.data_length;

  //
  // Encrypt {[Kxu~]H1~:H2~:Kou~}Kxu with the session key S to obtain
  // [{[Kxu~]H1~:H2~:Kou~}Kxu]S.
  //

  estatus = sec_bsafe_symmetric_encrypt (sym_alg_id_pad,
                                         &s_key,
                                         sym_pad_iv_p,
                                         &plaintext,
                                         &ciphertext);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_bsafe_symmetric_encrypt : 0x%x", estatus));

  //
  // JCW: Added the following so that rundown doesn't try to free plaintext
  //      if we take an error.
  //

  plaintext.len = 0;
  plaintext.data = NULL;

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Initialize the ASN.1 translation object in anticipation
  // of translating user change key (client generates new keys) protocol phase
  // 3 message [{[Kxu~]H1~:H2~:Kou~}Kxu]S to ASN.1.
  //

  estatus = cltChgP5Msg.set_value (ciphertext.data, ciphertext.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN set_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // ASN.1-encode [{[Kxu~]H1~:H2~:Kou~}Kxu]S for transmission to PKSS.
  //

  estatus = cltChgP5Msg.write (to_asn_cipher);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN write : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Zero instance of ASN.1 translation object after writing  ASN.1-encoded
  // data to ASN.1 buffer object.  Also zero and delete contents of
  // ciphertext_p.
  //

  sec_pkss_zero_and_free (&ciphertext);
  
  //
  // Move encrypted data from ASN.1 buffer object to RPC input buffer in
  // anticipation of transmission to the PKSS.
  //

  to_pkss.msg_len = to_asn_cipher.data_length;
  to_pkss.msg_p = to_asn_cipher.data;

  //
  // Change user's password and/or asymmetric key pair.
  //

  cstatus = sec_pkss_chg_key_clt (binding_handle,
				  &ctx_handle,
				  &to_pkss,
				  &from_pkss_p,
				  &estatus);
  if (cstatus != rpc_s_ok)
    estatus = cstatus;

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_pkss_chg_key_clt : 0x%x", estatus));

  rpc_binding_free (&binding_handle, &cstatus);

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Zero and release contents of old_pvt_key and old_pub_key.
  //

  sec_pkss_zero_and_free (&old_pvt_key);
  sec_pkss_zero_and_free (&old_pub_key);

  //
  // The contents of the output buffer from_asn_cipher are
  // an ASN.1-encoded bit string of encrypted data.  Move the RPC output data
  // to the ASN.1 buffer object in anticipation ASN.1 translation.
  //

  from_asn_cipher.data = from_pkss_p->msg_p;
  from_asn_cipher.data_length = from_pkss_p->msg_len;

  //
  // ASN.1-decode [Nc:V:Status]S upon return from the PKSS.
  //

  estatus = cltChgP6Msg.read(from_asn_cipher);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN read : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  estatus = cltChgP6Msg.get_value (ciphertext.data, ciphertext.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN get_value : 0x%x", estatus));

  if (estatus != 0)
  {
    ciphertext.data = NULL; ciphertext.len = 0;
    goto rundown;
  }

  //
  // Release from_pkss.
  //

  if (from_pkss_p)
  {
    if (from_pkss_p->msg_p)
    {
      (void) memset (from_pkss_p->msg_p, 0, from_pkss_p->msg_len);
      from_pkss_p->msg_len = 0;
      free (from_pkss_p->msg_p);
    }
    free (from_pkss_p);
    from_pkss_p = 0;
  };

  //
  // Decrypt [Nc:V:Status]S to obtain Nc:V:Status.
  //

  estatus = sec_bsafe_symmetric_decrypt (sym_alg_id_pad,
                                         &s_key,
                                         sym_pad_iv_p,
                                         &ciphertext,
                                         &plaintext);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_bsafe_symmetric_decrypt : 0x%x", estatus));

  //
  // Clear ciphertext descriptor so that rundown doesn't try to free it
  // (since it doesn't own the meory it describes).
  //

  ciphertext.data = 0;
  ciphertext.len = 0;

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Move decrypted data from struct pointed to by plaintext to ASN.1 buffer 
  // object in anticipation of populating the ASN.1 translation object.
  //

  from_asn_plain.data = plaintext.data;
  from_asn_plain.data_length = plaintext.len;

  //
  // ASN.1-decode Nc:V:Status.
  //

  estatus = cltChgP6Data.read(from_asn_plain);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN read : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Zero and release contents of plaintext.  Note that this also zeroes
  // contents of from_asn_plain.  Also, zero and release the contents of
  // sym_pad_iv_p, sym_nopad_iv_p, and s_key.
  //

  sec_pkss_zero_and_free (&s_key);
  sec_pkss_zero_and_free (&plaintext);
  sec_pkss_zero_and_free (&sym_pad_iv_p);
  sec_pkss_zero_and_free (&sym_nopad_iv_p);

  //
  // Obtain output parameters translated from ASN.1.
  //

  estatus = cltChgP6Data.nc.get_value (clt_nonce_from_pkss.data,
                                       clt_nonce_from_pkss.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN get_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  estatus = cltChgP6Data.version.get_value (*new_key_vno_p);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN get_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  estatus = cltChgP6Data.status.get_value (estatus);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN get_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Compare the Nc returned in from_pkss from the PKSS with the Nc generated
  // in this function.  If the two do not match, assume an  attack on the
  // network traffic, discontinue processing, and return an error.
  //

  estatus = sec_pkss_pk_gen_data_cmp (&clt_nonce, &clt_nonce_from_pkss);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_pkss_pk_gen_data_cmp : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  sec_pkss_zero_and_free (&clt_nonce);

  //
  // Exit this function.
  //

  return (estatus);

  //
  // On error, come here to zero out all stack, heap data.  Static objects must
  // do their own cleanup when they are deleted.
  //

rundown:

  proto_id = 0;
  ret_key_vno = 0;
  (void) memset (&ctx_handle, 0, sizeof(sec_pkss_ctx_hdl_t));

// JCW: Changed the following to only zero&free new_h1 if the memory it
//      describes is different to that described by old_h1
  if (old_h1.data != new_h1.data) 
      sec_pkss_zero_and_free (&new_h1);
  sec_pkss_zero_and_free (&old_h1);

// JCW: Changed the following to only zero&free new_h2 if the memory it
//      describes is different to that described by old_h2
  if (old_h2.data != new_h2.data) 
      sec_pkss_zero_and_free (&new_h2);
  sec_pkss_zero_and_free (&old_h2);

  sec_pkss_zero_and_free (&old_pvt_key); 
  sec_pkss_zero_and_free (&old_pub_key); 
  sym_alg_id_pad = 0;
  sym_alg_id_nopad = 0;
  sym_pad_key_len = 0;
  sym_nopad_key_len = 0;
  sec_pkss_zero_and_free (&s_key);
  sec_pkss_zero_and_free (&sym_pad_iv_p);
  sec_pkss_zero_and_free (&sym_nopad_iv_p);
  sec_pkss_zero_and_free (&clt_nonce);
  sec_pkss_zero_and_free (&newKxuH1);
  sec_pkss_zero_and_free (&signature);
  sig_alg = 0;
  sec_pkss_zero_and_free (&ciphertext);
  sec_pkss_zero_and_free (&plaintext);

  if (from_pkss_p)
  {
    if (from_pkss_p->msg_p)
    {
      (void) memset (from_pkss_p->msg_p, 0, from_pkss_p->msg_len);
      from_pkss_p->msg_len = 0;
      free (from_pkss_p->msg_p);
    }
    free (from_pkss_p);
    from_pkss_p = 0;
  };

  rpc_binding_free (&binding_handle, &cstatus);

  //
  // Exit this function with the error status
  //

  return (estatus);

} // End sec_pkss_key_chg_clt

//
// Change user's password and/or asymmetric key pair (server generates
// new asymmetric key pair).
//
// calls:  sec_pkss_clt_ieke
//         sec_bsafe_sign_data
//
// PKSS RPC's: 	sec_pkss_chg_key_svr
//

error_status_t sec_pkss_key_chg_svr
(
  const char *pkss_svc_profile_name_p,
  const char *name_p,
  sec_pk_domain_t domain_id,
  sec_pk_usage_flags_t key_usages,
  unsigned32 key_vno,
  const char *old_pwd_p,
  const char *new_pwd_p,
  unsigned32 key_chg_flag,
  sec_pk_algorithm_id_t *pk_alg_id_p,
  unsigned32 pk_len,
  sec_pk_pubkey_t *new_pub_key_p,
  unsigned32 *new_key_vno_p
)

{
  //
  // Local variables
  //

  error_status_t estatus = 0;
  error_status_t cstatus = 0;
  unsigned32 proto_id = PKSS_PROTO_USR_KEY_CHG_SVR;
  sec_pkss_ctx_hdl_t ctx_handle = 0;
  rpc_binding_handle_t binding_handle;
  sec_pk_gen_data_t h1 = {0, 0};
  sec_pk_gen_data_t h2 = {0, 0};
  sec_pk_pvtkey_t pvt_key = {0, 0};
  sec_pk_pubkey_t pub_key = {0, 0};
  sec_bsafe_alg_type_t sym_alg_id_pad = 0;
  sec_bsafe_alg_type_t sym_alg_id_nopad = 0;
  unsigned32 sym_pad_key_len;
  unsigned32 sym_nopad_key_len;
  sec_pk_gen_data_t *sym_pad_iv_p = 0;
  sec_pk_gen_data_t *sym_nopad_iv_p = 0;
  sec_pk_gen_data_t s_key = {0, 0};
  sec_pk_gen_data_t clt_nonce = {0, 0};
  PkssSvrChgP5SignedC svrChgP5Signed; 	// ASN.1 translation object
  buffer_t to_asn_plain(SECURE);	// ASN.1 buffer object to send
  sec_pk_gen_data_t toBeSigned = {0, 0}; // data to be signed
  sec_pk_gen_data_t signature = {0, 0};	// returned signed data buffer
  sec_bsafe_alg_type_t sig_alg = 0;	// sign algorithm
  buffer_t to_asn_signed(SECURE);	// ASN.1 buf obj for signed data
  sec_pk_gen_data_t plaintext = {0, 0};	// plaintext buffer for signed data
  sec_pk_gen_data_t ciphertext = {0, 0}; // encode buffer from PKSS
  PkssSvrChgP5MsgC svrChgP5Msg;		// ASN.1 translation object
  buffer_t to_asn_cipher;		// ASN.1 buffer object going to PKSS
  sec_pkss_msg_buf_t to_pkss = {0, 0};	// RPC msg buf, xmt to PKSS
  sec_pkss_msg_buf_t *from_pkss_p = 0;	// RPC buffer ptr from PKSS  
  r_buffer_t from_asn_cipher;		// ASN.1 buf, returned data from PKSS
  PkssSvrChgP6MsgC svrChgP6Msg;		// ASN.1 translation object
  r_buffer_t from_asn_plain;		// ASN.1 buf obj, returned plaintext
  PkssSvrChgP6DataC svrChgP6Data;	// ASN.1 translation object
  sec_pk_gen_data_t clt_nonce_from_pkss = {0, 0};// output parameters from PKSS
  buffer_t pub_key_data;		// ASN.1 buf obj returned to caller

  //
  // Obtain public key pair using improved encrypted key exchange protocol.
  //

  estatus = sec_pkss_clt_ieke  (pkss_svc_profile_name_p,
				name_p,
				domain_id,
				key_usages,
				key_vno,
				old_pwd_p,
				proto_id,
				&pvt_key,
				&pub_key,
				new_key_vno_p,
				&ctx_handle,
				&binding_handle,
				&h1,
				&h2,
				&sym_alg_id_pad,
				&sym_alg_id_nopad,
				&sym_pad_key_len,
				&sym_nopad_key_len,
				&sym_pad_iv_p,
				&sym_nopad_iv_p,
				&s_key,
				&clt_nonce);

  //
  // Get rid of the old public key component (will be replaced by new public
  // key component PKSS server generates).
  //

  sec_pkss_zero_and_free (&pub_key);

  //
  // Initialize the ASN.1 translation object in anticipation
  // of translating user change key (server generates new keys) protocol phase
  // 5 data H1:H2:newpassword:key-chg?:A:L to ASN.1 before signing.
  //

  estatus = svrChgP5Signed.dataToBeSigned.oldH1.set_value (h1.data, h1.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN set_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  estatus = svrChgP5Signed.dataToBeSigned.oldH2.set_value (h2.data, h2.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN set_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  estatus = svrChgP5Signed.dataToBeSigned.newPwd.set_value (new_pwd_p);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN set_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  estatus = svrChgP5Signed.dataToBeSigned.keyChg.set_value (key_chg_flag);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN set_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  estatus = svrChgP5Signed.dataToBeSigned.pkAlgID.set_value (pk_alg_id_p->data,
							     pk_alg_id_p->len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN set_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  estatus = svrChgP5Signed.dataToBeSigned.pkKeyLen.set_value (pk_len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN set_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }


  //
  // ASN.1-encode H1:H2:newpassword:key-chg?:A:L in anticipation of 
  // signing.  Observe that H1 and H2 are hashes on the contents of the 
  // old_pwd_p and are returned by sec_pkss_clt_ieke.
  //

  estatus = svrChgP5Signed.dataToBeSigned.write (to_asn_plain);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN write : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Zero and release storage for H1 and H2.
  //

  sec_pkss_zero_and_free (&h1);
  sec_pkss_zero_and_free (&h2);

  //
  // Move contents of ASN.1 buffer object to sec_pk_gen_data_t struct in
  // anticipation of signing it.
  //

  toBeSigned.data = to_asn_plain.data;
  toBeSigned.len = to_asn_plain.data_length;

  //
  // Sign H1:H2:newpassword:key-chg? with the old private key Kxu returned
  // by sec_pkss_clt_ieke to obtain {H1:H2:newpassword:key-chg?:A:L}Kxu
  //

  sig_alg = sec_sign_md2rsa;

  estatus = sec_bsafe_sign_data (sig_alg,
                                 &toBeSigned,
                                 (sec_pk_data_t *)&pvt_key,
                                 (sec_pk_data_t *)&signature);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_bsafe_sign_data : 0x%x", estatus));

  sec_pkss_zero_and_free (&pvt_key);

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Initialize the rest of the ASN.1 translation object in anticipation
  // of translating the signed user change key protocol phase 5 data
  // {H1:H2:newpassword:key-chg?:A:L}Kxu to ASN.1.
  //

  estatus = svrChgP5Signed.sigAlg.set_value (sig_alg);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sigAlg.set_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  estatus = svrChgP5Signed.signature.set_value (signature.data, signature.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from signature.set_value : 0x%x", estatus));

  //
  // Zero and release contents of signature.
  //

  sec_pkss_zero_and_free (&signature);

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // ASN.1-encode {H1:H2:newpassword:key-chg?:A:L}Kxu prior to encryption.
  //

  estatus = svrChgP5Signed.write(to_asn_signed);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN write : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Move contents of ASN.1 buffer object to sec_pk_gen_data_t struct in
  // anticipation of encrypting it.
  //

  plaintext.data = to_asn_signed.data;
  plaintext.len = to_asn_signed.data_length;

  //
  // Encrypt {H1:H2:newpassword:key-chg?:A:L}Kxu with the session key S to
  // obtain [{H1:H2:newpassword:key-chg?:A:L}Kxu]S
  //

  estatus = sec_bsafe_symmetric_encrypt (sym_alg_id_pad,
                                         &s_key,
                                         sym_pad_iv_p,
                                         &plaintext,
                                         &ciphertext);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_bsafe_symmetric_encrypt : 0x%x", estatus));

  plaintext.len = 0; plaintext.data = NULL;

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Initialize the ASN.1 translation object in anticipation
  // of translating user change key (server generates new keys) protocol phase
  // 5 message [{H1:H2:newpassword:key-chg?:A:L}Kxu]S to ASN.1.
  //

  estatus = svrChgP5Msg.set_value (ciphertext.data, ciphertext.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN set_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // ASN.1-encode [{H1:H2:newpassword:key-chg?:A:L}Kxu]S for xmt to PKSS.
  //

  estatus = svrChgP5Msg.write (to_asn_cipher);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN write : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Zero instance of ASN.1 translation object after writing ASN.1-encoded
  // data to ASN.1 buffer object.  Also zero and delete contents of
  // ciphertext.
  //

  sec_pkss_zero_and_free (&ciphertext);

  //
  // Move encrypted data from ASN.1 buffer object to RPC input buffer in
  // anticipation of transmission to the PKSS.
  //

  to_pkss.msg_len = to_asn_cipher.data_length;
  to_pkss.msg_p = to_asn_cipher.data;

  //
  // Change user's password and/or asymmetric key pair.
  //

  cstatus = sec_pkss_chg_key_svr (binding_handle,
				  &ctx_handle,
				  &to_pkss,
				  &from_pkss_p,
				  &estatus);
  if (cstatus != rpc_s_ok)
    estatus = cstatus;

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_pkss_chg_key_svr : 0x%x", estatus));

  rpc_binding_free (&binding_handle, &cstatus);

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // The contents of the output buffer are an ASN.1-encoded bit string of 
  // encrypted data.  Move the RPC output data to the ASN.1 buffer object in 
  // anticipation ASN.1 translation.
  //

  from_asn_cipher.data = from_pkss_p->msg_p;
  from_asn_cipher.data_length = from_pkss_p->msg_len;

  //
  // ASN.1-decode [Nc:V:Status:Kou~]S upon return from the PKSS.
  //

  estatus = svrChgP6Msg.read(from_asn_cipher);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN read : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  estatus = svrChgP6Msg.get_value (ciphertext.data, ciphertext.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN get_value : 0x%x", estatus));

  if (estatus != 0)
  {
    ciphertext.data = NULL; ciphertext.len = 0;
    goto rundown;
  }

  //
  // Zero and release from_pkss.
  //

  if (from_pkss_p)
  {
    if (from_pkss_p->msg_p)
    {
      (void) memset (from_pkss_p->msg_p, 0, from_pkss_p->msg_len);
      from_pkss_p->msg_len = 0;
      free (from_pkss_p->msg_p);
    }
    free (from_pkss_p);
    from_pkss_p = 0;
  };

  //
  // Decrypt [Nc:V:Status:Kou~]S to obtain Nc:V:Status:Kou~.
  //

  estatus = sec_bsafe_symmetric_decrypt (sym_alg_id_pad,
                                         &s_key,
                                         sym_pad_iv_p,
                                         &ciphertext,
                                         &plaintext);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_bsafe_symmetric_decrypt : 0x%x", estatus));

  ciphertext.data = NULL; ciphertext.len = 0;

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Move decrypted data from struct pointed to by plaintext to ASN.1 buffer 
  // object in anticipation of populating the ASN.1 translation object.
  //

  from_asn_plain.data = plaintext.data;
  from_asn_plain.data_length = plaintext.len;

  //
  // ASN.1-decode Nc:V:Status:Kou~.
  //

  estatus = svrChgP6Data.read(from_asn_plain);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN read : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Zero and release contents of plaintext.  Note that this also zeroes
  // contents of from_asn_plain.  Also zero contents of sym_pad_iv_p,
  // sym_nopad_iv_p, and s_key.
  //

  sec_pkss_zero_and_free (&s_key);
  sec_pkss_zero_and_free (&plaintext);
  sec_pkss_zero_and_free (&sym_pad_iv_p);
  sec_pkss_zero_and_free (&sym_nopad_iv_p);

  //
  // Obtain output parameters.
  //

  estatus = svrChgP6Data.nc.get_value (clt_nonce_from_pkss.data,
				       clt_nonce_from_pkss.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN get_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  estatus = svrChgP6Data.version.get_value (*new_key_vno_p);

   DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN get_value : 0x%x", estatus));

  if (estatus != 0)
  {
   goto rundown;
  }

  estatus = svrChgP6Data.status.get_value (estatus);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN get_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }


  //
  // Write the public key data to pub_key_datapub_key_data. Recall that 
  // this layer of software leaves the public key data in ASN.1 form for 
  // use by the calling layer.  Note that it is the  caller's responsibility 
  // to clear and free the allocated storage.
  //

  estatus = svrChgP6Data.newKou.write (pub_key_data);
 
    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN write : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  new_pub_key_p->data = pub_key_data.data;
  new_pub_key_p->len = pub_key_data.data_length;

  pub_key_data.detach_buffer();

  //
  // Compare the Nc returned in from_pkss from the PKSS with the Nc generated
  // in this function.  If the two do not match, assume an  attack on the
  // network traffic, discontinue processing, and return an error.
  //

  estatus = sec_pkss_pk_gen_data_cmp (&clt_nonce, &clt_nonce_from_pkss);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_pkss_pk_gen_data_cmp : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Zero and release contents of clt_nonce.
  //

  sec_pkss_zero_and_free (&clt_nonce);

  //
  // Exit this function.
  //

  return (estatus);

  //
  // On error, come here to zero out all stack and heap data.  Static objects 
  // must do their own cleanup when they are deleted.
  //

rundown:

  proto_id = 0;
  (void) memset (&ctx_handle, 0, sizeof(sec_pkss_ctx_hdl_t));
  sec_pkss_zero_and_free (&h1);
  sec_pkss_zero_and_free (&h2);
  sec_pkss_zero_and_free (&pvt_key);
  sym_alg_id_pad = 0;
  sym_alg_id_nopad = 0;
  sym_pad_key_len = 0;
  sym_nopad_key_len = 0;
  sec_pkss_zero_and_free (&s_key);
  sec_pkss_zero_and_free (&sym_pad_iv_p);
  sec_pkss_zero_and_free (&sym_nopad_iv_p);
  sec_pkss_zero_and_free (&clt_nonce);
  sec_pkss_zero_and_free (&signature);
  sec_pkss_zero_and_free (&ciphertext);
  sec_pkss_zero_and_free (&plaintext);

  if (from_pkss_p)
  {
    if (from_pkss_p->msg_p)
    {
      (void) memset (from_pkss_p->msg_p, 0, from_pkss_p->msg_len);
      from_pkss_p->msg_len = 0;
      free (from_pkss_p->msg_p);
    }
    free (from_pkss_p);
    from_pkss_p = 0;
  };

  rpc_binding_free (&binding_handle, &cstatus);

  //
  // Exit this function.
  //

  return (estatus);

} // End sec_pkss_key_chg_svr

