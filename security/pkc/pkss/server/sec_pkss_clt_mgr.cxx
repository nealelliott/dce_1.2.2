//
// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// All Rights Reserved
//
// HISTORY
// $Log: sec_pkss_clt_mgr.cxx,v $
// Revision 1.1.2.2  1996/11/13  20:41:14  arvind
// 	PKSS bug fix from DEC
// 	[1996/11/03  23:40 UTC  arvind  /main/DCE_1.2.2/arvind_capi2/1]
//
// Revision 1.1.2.1  1996/10/03  20:30:44  arvind
// 	OSF DCE 1.2.2 Drop 4
// 	[1996/10/03  19:28:40  arvind]
// 
// Revision /main/DCE_1.2.2/1  1996/09/09  21:29 UTC  arvind
// 	PKSS drop from DEC (DCE1.2.2)
// 	[1996/08/30  15:52 UTC  arvind  /main/arvind_pkss/1]
// 
// $EndLog$
//
// (c) Copyright 1996,
// Digital Equipment Corporation, Maynard, Massachusetts, USA
// All Rights Reserved
//
// PKSS server manager functions - see sec_pkss_clt_api.h for input/output
// descriptions
//

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <netinet/in.h>
#include "gdb44_methods.h"
#include "sec_pkss_dbkey_class.h"
#include "sec_bsafe.h"
#include "sec_pkss_asn.h"
#include "sec_pkss_lits.h"
#include "sec_pkss_util.h"
#include "sec_pkss_svr_util.h"
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
// Perform Diffie-Hellman key exchange with client.
//

error_status_t sec_pkss_clt_diffhell
(
  handle_t handle,
  sec_pkss_ctx_hdl_t *ctx_handle_p,
  sec_pkss_msg_buf_t *to_pkss_p,
  sec_pkss_msg_buf_p_t *from_pkss_p,
  error_status_t *estatus_p
)

{
  //
  // Local Variables
  //

  error_status_t pstatus = 0;
  r_buffer_t from_asn;
  PkssLoginP1MsgC loginP1Msg;
  char *tmp_name_p = NULL;
  char *tmp_domain_p = NULL;
  sec_pk_usage_flags_t tmp_usages = 0;
  unsigned32 tmp_version = 0;
  sec_pk_gen_data_t clt_dh_val = {0, 0};
  unsigned32 sym_alg_to_use = 0;
  sec_bsafe_alg_type_t sym_alg_id_pad = 0;
  sec_bsafe_alg_type_t sym_alg_id_nopad = 0;
  unsigned32 sym_pad_key_len = 0;
  unsigned32 sym_nopad_key_len = 0;
  sec_pk_gen_data_t *sym_pad_iv_p = 0;
  sec_pk_gen_data_t *sym_nopad_iv_p = 0;
  void *dh_context_p = 0;
  sec_pk_gen_data_t pkss_dh_val = {0, 0};
  sec_pk_gen_data_t *dh_key_p = 0;
  sec_pkss_dbrec_t dbrec = {0, 0};
  sec_pk_gen_data_t *sess_key_p = 0;
  sec_pk_gen_data_t *svr_nonce_p = 0;
  sec_pk_gen_data_t np_s = {0, 0};
  r_buffer_t dbrec_asn_buf;
  PkssDBrecC dbRec;
  sec_pk_gen_data_t h2 = {0, 0};
  sec_pk_gen_data_t s_h2 = {0, 0};
  sec_pk_gen_data_t h_clt_algs = {0, 0};
  sec_pk_gen_data_t clt_algs = {0, 0};
  buffer_t clt_algs_buf (SECURE);
  unsigned32 clt_alg_idx = 0;
  PkssUsrGetKeyP2DataF2C loginP2Data;
  buffer_t to_asn (SECURE);
  sec_pk_gen_data_t plaintext_in = {0, 0};
  sec_pk_gen_data_t ciphertext_out = {0, 0};
  PkssLoginP2MsgC loginP2Msg;
  unsigned msglen = 0;
  sec_pkss_context_t *context_p = 0;
  PkssDbKeyC *dbKeyP = 0;

#ifdef MALLOCTRACE
  malloc_mstatus ("Entering sec_pkss_clt_diffhell");
#endif

  *ctx_handle_p = 0;

  //
  // Initialize ASN.1 buffer object into which to move data
  // from RPC input buffer upon receipt from the PKSS client.
  //

  from_asn.data = to_pkss_p->msg_p;
  from_asn.data_length = to_pkss_p->msg_len;

  //
  // ASN.1-decode userKey:E(Dc):algList received from client into its
  // component elements.
  //

  *estatus_p = loginP1Msg.read (from_asn);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN read : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  //
  // Obtain input parameters userKey:E(Dc):algList translated from ASN.1.
  // Note that the contents of userKey constitute the PKSS database key.
  //

  *estatus_p = loginP1Msg.userKey.uname.get_value (tmp_name_p);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN get_value : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  *estatus_p = loginP1Msg.userKey.domain.get_value (tmp_domain_p);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN get_value : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  *estatus_p = loginP1Msg.userKey.usages.get_value (tmp_usages);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN get_value : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  *estatus_p = loginP1Msg.userKey.version.get_value (tmp_version);
  
    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN get_value : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  //
  // Obtain PKSS database key from values received from the client.
  //

  dbKeyP = new PkssDbKeyC (tmp_name_p, tmp_domain_p, tmp_usages, tmp_version);

  if (dbKeyP == 0)
  {
    dce_svc_printf(PKS_S_NO_MEMORY_MSG);
    *estatus_p = pks_s_no_memory;
    goto CHECK_ERROR_RETURN;
  }

  //
  // Obtain the client's exponentiated Diffie-Hellman value.
  //

  *estatus_p = loginP1Msg.eDc.get_value (clt_dh_val.data, clt_dh_val.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN get_value : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  //
  // Obtain algorithm list and algorithm to use from algList.  If there are no
  // matches between the list the client sent and the server's list, return an
  // error.
  //

  *estatus_p = sec_pkss_get_sym_alg_idx (&loginP1Msg.algList,
				          &sym_alg_to_use,
				          &clt_alg_idx);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
             "status from sec_pkss_get_sym_alg_idx : 0x%x, sym_alg_to_use: %d",
	     *estatus_p, sym_alg_to_use));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  if (sym_alg_to_use == 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  //
  // Obtain required symmetric key crypto info given a symmetric key
  // encryption algorithm identifier.  Note that it is the caller's
  // responsibility to free initial vector storage allocated in this
  // function.
  //

  *estatus_p = sec_pkss_get_sym_crypto_info (sym_alg_to_use,
                                             &sym_alg_id_pad,
                                             &sym_alg_id_nopad,
                                             &sym_pad_key_len,
                                             &sym_nopad_key_len,
                                             &sym_pad_iv_p,
                                             &sym_nopad_iv_p);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                 "status from sec_pkss_get_sym_crypto_info : 0x%x",
		 *estatus_p));

  if (*estatus_p != 0)
  {
    dce_svc_printf(PKS_S_SEC_BSAFE_MSG, *estatus_p);
    goto CHECK_ERROR_RETURN;
  }

  //
  // Obtain exponentiated D-H PKSS value E(Dp).
  //

  *estatus_p = sec_pkss_get_dh_val (&dh_context_p, &pkss_dh_val);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_pkss_get_dh_val : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  //
  // Compute a Diffie-Hellman key D, using E(Dc) and E(Dp).
  //

  dh_key_p = (sec_pk_gen_data_t *) calloc (1, sizeof (sec_pk_gen_data_t *));

  if (!(dh_key_p))
  {
    dce_svc_printf(PKS_S_NO_MEMORY_MSG);
    *estatus_p = pks_s_no_memory;
    goto CHECK_ERROR_RETURN;
  }

  dh_key_p->len = sym_pad_key_len;
  dh_key_p->data = (unsigned char *) calloc (1, dh_key_p->len);

  if (!(dh_key_p->data))
  {
    dce_svc_printf(PKS_S_NO_MEMORY_MSG);
    *estatus_p = pks_s_no_memory;
    goto CHECK_ERROR_RETURN;
  }

  *estatus_p = sec_bsafe_diffie_hellman_phase_2 (&dh_context_p,
                                                 &clt_dh_val,
                                                 dh_key_p);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_bsafe_diffie_hellman_phase_2 : 0x%x",
		   *estatus_p));

  if (*estatus_p != 0)
  {
    dce_svc_printf(PKS_S_SEC_BSAFE_MSG, *estatus_p);
    goto CHECK_ERROR_RETURN;
  }

  //
  // Search the PKSS database for userKey.  If not found, discontinue
  // processing and return an error.
  // 

  *estatus_p = sec_pkss_db_lookup (dbKeyP, &dbrec);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_pkss_db_lookup : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  //
  // Generate a random session key S.
  //

  sess_key_p = (sec_pk_gen_data_t *) calloc (1, sizeof (sec_pk_gen_data_t *));

  if (!(sess_key_p))
  {
    dce_svc_printf(PKS_S_NO_MEMORY_MSG);
    *estatus_p = pks_s_no_memory;
    goto CHECK_ERROR_RETURN;
  }

  sess_key_p->len = sym_pad_key_len;
  sess_key_p->data = (unsigned char *) calloc (1, sess_key_p->len);

  if (!(sess_key_p->data))
  {
    dce_svc_printf(PKS_S_NO_MEMORY_MSG);
    *estatus_p = pks_s_no_memory;
    goto CHECK_ERROR_RETURN;
  }

  *estatus_p = sec_bsafe_random_bytes (0, sess_key_p);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_bsafe_random_bytes : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    dce_svc_printf(PKS_S_SEC_BSAFE_MSG, *estatus_p);
    goto CHECK_ERROR_RETURN;
  }

  //
  // Generate a nonce Np.
  //

  svr_nonce_p = (sec_pk_gen_data_t *)
                 calloc (1, sizeof (sec_pk_gen_data_t *));

  if (!(svr_nonce_p))
  {
    dce_svc_printf(PKS_S_NO_MEMORY_MSG);
    *estatus_p = pks_s_no_memory;
    goto CHECK_ERROR_RETURN;
  }

  svr_nonce_p->len = PKSS_NONCE_SIZE;
  svr_nonce_p->data = (unsigned char *) calloc (1, svr_nonce_p->len);

  if (!(svr_nonce_p->data))
  {
    dce_svc_printf(PKS_S_NO_MEMORY_MSG);
    *estatus_p = pks_s_no_memory;
    goto CHECK_ERROR_RETURN;
  }

  *estatus_p = sec_bsafe_random_bytes (0, svr_nonce_p);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_bsafe_random_bytes : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    dce_svc_printf(PKS_S_SEC_BSAFE_MSG, *estatus_p);
    goto CHECK_ERROR_RETURN;
  }

  //
  // Encrypt Np with S to obtain [Np]S.  Recall that Np is NOT ASN.1-encoded
  // prior to encryption.
  //

  *estatus_p = sec_bsafe_symmetric_encrypt (sym_alg_id_nopad,
                                            sess_key_p,
                                            sym_nopad_iv_p,
                                            svr_nonce_p,
                                            &np_s);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
               "status from sec_bsafe_symmetric_encrypt : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    dce_svc_printf(PKS_S_SEC_BSAFE_MSG, *estatus_p);
    goto CHECK_ERROR_RETURN;
  }

  //
  // Obtain client's password hash H2 from the database record and
  // encrypt S with the client's password hash H2 to obtain [S]H2.
  //

  dbrec_asn_buf.data = dbrec.asn_rec_p,
  dbrec_asn_buf.data_length = dbrec.asn_rec_len;

  *estatus_p = dbRec.read (dbrec_asn_buf);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN read : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  *estatus_p = dbRec.H2.get_value (h2.data, h2.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN get_value : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  *estatus_p = sec_bsafe_symmetric_encrypt (sym_alg_id_nopad,
                                            &h2,
                                            sym_nopad_iv_p,
                                            sess_key_p,
                                            &s_h2);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                  "status from sec_bsafe_symmetric_encrypt : 0x%x",
		  *estatus_p));

  if (*estatus_p != 0)
  {
    dce_svc_printf(PKS_S_SEC_BSAFE_MSG, *estatus_p);
    goto CHECK_ERROR_RETURN;
  }

  //
  // Compute a hash H(algList) on the algorithm list algList sent by the client.
  //

  *estatus_p = loginP1Msg.algList.write (clt_algs_buf);

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  clt_algs.data = clt_algs_buf.data;
  clt_algs.len = clt_algs_buf.data_length;

  *estatus_p = sec_bsafe_compute_hash (sec_bsafe_hash_MD2,
                                       &clt_algs,
                                       &h_clt_algs);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                  "status from sec_bsafe_compute_hash : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    dce_svc_printf(PKS_S_SEC_BSAFE_MSG, *estatus_p);
    goto CHECK_ERROR_RETURN;
  }

  //
  // Initialize ASN.1 translation object in anticipation of
  // ASN.1-encoding [S]H2:H(algList) prior to encryption.
  //

  *estatus_p = loginP2Data.sH2.set_value (s_h2.data, s_h2.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                  "status from ASN set_value : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  *estatus_p = loginP2Data.hAlgs.set_value (h_clt_algs.data, h_clt_algs.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                  "status from ASN set_value : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  //
  // Initialize the ASN.1 buffer object and ASN.1-encode
  // [S]H2:H(algList).
  //

  *estatus_p = loginP2Data.write(to_asn);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                  "status from ASN write : 0x%x", *estatus_p));

//  (void) printf ("\nto_asn.data_length: %d\nto_asn.data: %2.2x %2.2x %2.2x %2.2x %2.2x\n", to_asn.data_length, to_asn.data[0], to_asn.data[1], to_asn.data[2], to_asn.data[3], to_asn.data[4]); 

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  //
  // Encrypt [S]H2:H(algList) with D to obtain [[S]H2:H(algList)]D
  //

  plaintext_in.data = to_asn.data;
  plaintext_in.len = to_asn.data_length;
  
  *estatus_p = sec_bsafe_symmetric_encrypt (sym_alg_id_pad,
                                            dh_key_p,
                                            sym_pad_iv_p,
                                            &plaintext_in,
                                            &ciphertext_out);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
               "status from sec_bsafe_symmetric_encrypt : 0x%x", *estatus_p));

//  (void) printf ("ciphertext_out.len: %d\nciphertext_out.data: %2.2x %2.2x %2.2x %2.2x %2.2x\n\n", ciphertext_out.len, ciphertext_out.data[0], ciphertext_out.data[1], ciphertext_out.data[2], ciphertext_out.data[3], ciphertext_out.data[4]); 

  if (*estatus_p != 0)
  {
    dce_svc_printf(PKS_S_SEC_BSAFE_MSG, *estatus_p);
    goto CHECK_ERROR_RETURN;
  }

  //
  // Clear the to_asn buffer so that we can re-use it to ASN.1-encode
  // E(Dp):[[S]H2:H(algList)]D:[Np]S:alg
  //

  to_asn.clear();

  //
  // ASN.1-encode E(Dp):[[S]H2:H(algList)]D:[Np]S:alg 
  // before returning to client.
  //

  *estatus_p = loginP2Msg.eDp.set_value (pkss_dh_val.data, pkss_dh_val.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                  "status from ASN set_value : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  *estatus_p = loginP2Msg.sH2HalgsD.set_value (ciphertext_out.data,
					       ciphertext_out.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                  "status from ASN set_value : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  *estatus_p = loginP2Msg.nps.set_value (np_s.data, np_s.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                  "status from ASN set_value : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  *estatus_p = loginP2Msg.algIdx.set_value (clt_alg_idx);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                  "status from ASN set_value : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  //
  // Initialize the ASN.1 buffer object into which to write
  // the translated PKSS user-get-key protocol phase 2 message
  // E(Dp):[[S]H2:H(algList)]D:[Np]S:alg in anticipation of transmission to
  // the PKSS client.
  //

  *estatus_p = loginP2Msg.write(to_asn);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                  "status from ASN set_value : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

CHECK_ERROR_RETURN:

  //
  // Load output parameter.  If estatus at this point is non-zero, then an
  // error occurred, and there is no point to returning a login phase 2 msg.
  //

  msglen = sizeof (sec_pkss_msg_buf_t);

  if (*estatus_p == 0)
    msglen += to_asn.data_length;

  *from_pkss_p = (sec_pkss_msg_buf_p_t)rpc_ss_allocate (msglen);

  if (!(*from_pkss_p))
  {
    dce_svc_printf(PKS_S_NO_MEMORY_MSG);
    *estatus_p = pks_s_no_memory;
  }

  else
  {
    if (*estatus_p != 0)
    {
      (*from_pkss_p)->msg_len = 0;
      (*from_pkss_p)->msg_p = 0;
    }

    else
    {
      (*from_pkss_p)->msg_len = to_asn.data_length;
      (*from_pkss_p)->msg_p = (unsigned char *)(*from_pkss_p) +
                              sizeof (sec_pkss_msg_buf_t);
      (void) memcpy ((*from_pkss_p)->msg_p, to_asn.data, to_asn.data_length);

      //
      // Generate user context block and set ctx_handle_p.  Save session key S,
      // symmetric encryption algorithm ID algId in context block.
      //

      context_p = (sec_pkss_context_t *)
		  calloc (1, sizeof (sec_pkss_context_t));

      if (!(context_p))
      {
        dce_svc_printf(PKS_S_NO_MEMORY_MSG);
        *estatus_p = pks_s_no_memory;
      }

      else
      {
        context_p->sym_alg = sym_alg_to_use;
        context_p->state = 0;
        context_p->deleted = 0;
        context_p->crypto_key_p = sess_key_p;
        context_p->svr_nonce_p = svr_nonce_p;
        context_p->dbkey_p = dbKeyP;
	*estatus_p = pthread_mutex_init (&(context_p->mutex),
					 pthread_mutexattr_default);
        *ctx_handle_p = (sec_pkss_ctx_hdl_t)context_p;

      } // End else path from if (!(context_p))

    } // End else path from if (*estatus_p != 0)

  } // End else path from if (!(*from_pkss_p))

  //
  // Zero buffers with sensitive data before returning.
  //

  msglen = 0;
  tmp_usages = 0;
  tmp_version = 0;
  sym_alg_to_use = 0;
  sym_alg_id_pad = 0;
  sym_alg_id_nopad = 0;
  sym_pad_key_len = 0;
  sym_nopad_key_len = 0;
  sec_pkss_zero_and_free (&dbrec);
  sec_pkss_zero_and_free (&pkss_dh_val);
  sec_pkss_zero_and_free (&sym_pad_iv_p);
  sec_pkss_zero_and_free (&sym_nopad_iv_p);
  sec_pkss_zero_and_free (&np_s);
  sec_pkss_zero_and_free (&s_h2);
  sec_pkss_zero_and_free (&h_clt_algs);
  sec_pkss_zero_and_free (&ciphertext_out);
  sec_pkss_zero_and_free (&dh_key_p);

  //
  // Exit this function.
  //

#ifdef MALLOCTRACE
  malloc_mstatus ("Exiting sec_pkss_clt_diffhell");
#endif

  return (rpc_s_ok);

} // End sec_pkss_clt_diffhell


//
// Return asymmetric key pair to client.
//

error_status_t sec_pkss_clt_pkey_get
(
  handle_t IDL_handle,
  sec_pkss_ctx_hdl_t *ctx_handle_p,
  sec_pkss_msg_buf_t *to_pkss_p,
  sec_pkss_msg_buf_p_t *from_pkss_p,
  error_status_t *estatus_p
)

{

  //
  // Local variables
  //

  r_buffer_t from_asn_cipher;
  PkssLoginP3MsgC loginP3Msg;
  sec_pkss_context_t *context_p = 0;
  sec_bsafe_alg_type_t sym_alg_id_pad = 0;
  sec_bsafe_alg_type_t sym_alg_id_nopad = 0;
  unsigned32 sym_pad_key_len = 0;
  unsigned32 sym_nopad_key_len = 0;
  sec_pk_gen_data_t *sym_pad_iv_p = 0;
  sec_pk_gen_data_t *sym_nopad_iv_p = 0;
  sec_pk_gen_data_t ciphertext_in = {0, 0};
  sec_pk_gen_data_t ciphertext_out = {0, 0};
  sec_pk_gen_data_t plaintext_in = {0, 0};
  sec_pk_gen_data_t plaintext_out = {0, 0};
  sec_pk_gen_data_t tmp_descriptor = {0, 0};
  r_buffer_t from_asn_plain;
  PkssUsrGetKeyP3DataC loginP3Data;
  sec_pk_gen_data_t svr_nonce = {0, 0};
  sec_pkss_dbrec_t dbrec = {0, 0};
  r_buffer_t dbrec_asn_buf;
  PkssDBrecC dbRec;
  PkssUsrGetKeyP4DataC loginP4Data;
  unsigned32 tmp_version = 0;
  sec_pk_gen_data_t tmp_kxu_h1 = {0, 0};
  buffer_t pub_key_buf (SECURE);
  buffer_t to_asn_plain (SECURE);
  PkssLoginP4MsgC loginP4Msg;
  buffer_t to_asn_cipher (SECURE);
  unsigned32 msglen = 0;

#ifdef MALLOCTRACE
  malloc_mstatus ("Entering sec_pkss_clt_pkey_get");
#endif

  //
  // Set pointer to context block and acquire mutex.
  //

  context_p = (sec_pkss_context_t *)*ctx_handle_p;

  if (context_p->deleted)
    return (EINVAL);
  else
  {
    *estatus_p = pthread_mutex_lock (&(context_p->mutex));

    if (*estatus_p != 0)
      return (*estatus_p);
  }

  //
  // Instantiate and initialize ASN.1 buffer object into which to move data
  // from RPC input buffer upon receipt from the PKSS client.
  //

  from_asn_cipher.data = to_pkss_p->msg_p;
  from_asn_cipher.data_length = to_pkss_p->msg_len;

  //
  // ASN.1-decode the encrypted input parameters [I:Np:Nc]S 
  // received from the PKSS client.
  //

  *estatus_p = loginP3Msg.read (from_asn_cipher);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                  "status from ASN read : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  //
  // Obtain required symmetric key crypto info given a symmetric key
  // encryption algorithm identifier.  Note that it is the caller's
  // responsibility to free initial vector storage allocated in this
  // function.
  //

  *estatus_p = sec_pkss_get_sym_crypto_info (context_p->sym_alg,
                                             &sym_alg_id_pad,
                                             &sym_alg_id_nopad,
                                             &sym_pad_key_len,
                                             &sym_nopad_key_len,
                                             &sym_pad_iv_p,
                                             &sym_nopad_iv_p);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                  "status from sec_pkss_get_sym_crypto_info : 0x%x",
		  *estatus_p));

  if (*estatus_p != 0)
  {
    dce_svc_printf(PKS_S_SEC_BSAFE_MSG, *estatus_p);
    goto CHECK_ERROR_RETURN;
  }

  //
  // Obtain the ASN.1-decoded encrypted input parameter data and decrypt
  // [I:Np:Nc]S with the session key S to obtain I:Np:Nc.
  //

  loginP3Msg.get_value (ciphertext_in.data, ciphertext_in.len);

  *estatus_p = sec_bsafe_symmetric_decrypt (sym_alg_id_pad,
                                            context_p->crypto_key_p,
                                            sym_pad_iv_p,
                                            &ciphertext_in,
                                            &plaintext_out);

//  (void) printf ("\nciphertext_in.len: %d\nciphertext_in.data: %2.2x %2.2x %2.2x %2.2x %2.2x\n\n", ciphertext_in.len, ciphertext_in.data[0], ciphertext_in.data[1], ciphertext_in.data[2], ciphertext_in.data[3], ciphertext_in.data[4]); 

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                  "status from sec_bsafe_symmetric_decrypt : 0x%x",
		  *estatus_p));

  if (*estatus_p != 0)
  {
    dce_svc_printf(PKS_S_SEC_BSAFE_MSG, *estatus_p);
    goto CHECK_ERROR_RETURN;
  }

  //
  // Initialize ASN.1 buffer object into which to move data
  // in anticipation of ASN.1 translating the decrypted input parameters.
  //

  from_asn_plain.data = plaintext_out.data;
  from_asn_plain.data_length = plaintext_out.len;

  //
  // ASN.1-decode input parameters I:Np:Nc received from client.
  //

  *estatus_p = loginP3Data.read (from_asn_plain);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                  "status from ASN read : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  *estatus_p = loginP3Data.protoID.get_value (context_p->proto);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                  "status from ASN get_value : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  *estatus_p = loginP3Data.np.get_value (svr_nonce.data, svr_nonce.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                  "status from ASN get_value : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  *estatus_p = loginP3Data.nc.get_value (tmp_descriptor.data,
                                         tmp_descriptor.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                  "status from ASN get_value : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  context_p->clt_nonce_p = (sec_pk_gen_data_t *)
			    calloc (1, (sizeof (sec_pk_gen_data_t) +
				       tmp_descriptor.len));

  if (!(context_p->clt_nonce_p))
  {
    dce_svc_printf(PKS_S_NO_MEMORY_MSG);
    *estatus_p = pks_s_no_memory;
    goto CHECK_ERROR_RETURN;
  }

  context_p->clt_nonce_p->len = tmp_descriptor.len;
  context_p->clt_nonce_p->data = (unsigned char *)context_p->clt_nonce_p +
					sizeof (sec_pk_gen_data_t);
  (void) memcpy (context_p->clt_nonce_p->data,
                 tmp_descriptor.data,
                 tmp_descriptor.len);

  //
  // If Np received from the client does not match the Np generated by the
  // PKSS (and held in the context handle), discontinue processing and return
  // an error.
  //

  *estatus_p = sec_pkss_pk_gen_data_cmp (context_p->svr_nonce_p, &svr_nonce);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                  "status from sec_pkss_pk_gen_data_cmp : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  //
  // Obtain current user's PKSS database record.  If the indicated record
  // does not exist, discontinue processing and return an error.
  //

  *estatus_p = sec_pkss_db_lookup (context_p->dbkey_p, &dbrec);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                  "status from sec_pkss_db_lookup : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  //
  // Initialize ASN.1 buffer object in anticipation of ASN.1-
  // decoding the PKSS database record returned above.  Recall that PKSS
  // database records are stored in the ASN.1 representation.
  //

  dbrec_asn_buf.data = dbrec.asn_rec_p;
  dbrec_asn_buf.data_length = dbrec.asn_rec_len;

  //
  // Initialize ASN.1 translation object and ASN.1-decode
  // the PKSS database record [Kxu]H1:H2:Kou:V.
  //

  *estatus_p = dbRec.read (dbrec_asn_buf);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN read : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  //
  // Initialize ASN.1 translation object in anticipation of
  // ASN.1-encoding the return parameters [Kxu]H1:Kou:V:Nc.
  //

  *estatus_p = dbRec.version.get_value (tmp_version);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                  "status from ASN get_value : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  *estatus_p = loginP4Data.version.set_value (tmp_version);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                  "status from ASN get_value : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  *estatus_p = dbRec.KxuH1.get_value (tmp_kxu_h1.data, tmp_kxu_h1.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                  "status from ASN get_value : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  *estatus_p = loginP4Data.kxuH1.set_value (tmp_kxu_h1.data, tmp_kxu_h1.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                  "status from ASN set_value : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  *estatus_p = loginP4Data.nc.set_value (context_p->clt_nonce_p->data,
                                         context_p->clt_nonce_p->len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                  "status from ASN set_value : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  //
  // Write the public key data back to ASN.1 from the dbRec translation 
  // object.  Then read it back into the loginP4Data translation object.
  //

  *estatus_p = dbRec.Kou.write (pub_key_buf);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                  "status from ASN write : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  *estatus_p = loginP4Data.kou.read (pub_key_buf);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                  "status from ASN read : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  //
  // ASN.1-encode return parameters [Kxu]H1:Kou:V:Nc prior to encryption.
  //

  *estatus_p = loginP4Data.write (to_asn_plain);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                  "status from ASN write : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  //
  // Move contents of ASN.1 buffer object to sec_pk_gen_data_t struct in
  // anticipation of encrypting it.
  //

  plaintext_in.data = to_asn_plain.data;
  plaintext_in.len = to_asn_plain.data_length;

  //
  // Encrypt [Kxu]H1:Kou:V:Nc with S to obtain [[Kxu]H1:Kou:V:Nc]S.
  //

  *estatus_p = sec_bsafe_symmetric_encrypt (sym_alg_id_pad,
                                            context_p->crypto_key_p,
                                            sym_pad_iv_p,
                                            &plaintext_in,
                                            &ciphertext_out);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                  "status from sec_bsafe_symmetric_encrypt : 0x%x",
		  *estatus_p));

  if (*estatus_p != 0)
  {
    dce_svc_printf(PKS_S_SEC_BSAFE_MSG, *estatus_p);
    goto CHECK_ERROR_RETURN;
  }

  //
  // ASN.1-encode [[Kxu]H1:Kou:V:Nc]S  in anticipation of returning 
  // the output parameters to the PKSS client.
  //

  *estatus_p = loginP4Msg.set_value (ciphertext_out.data, ciphertext_out.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                  "status from ASN set_value : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  //
  // ASN.1-encode output parameters prior to transmission to the mgmt client.
  //

  *estatus_p = loginP4Msg.write (to_asn_cipher);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                  "status from ASN write : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

CHECK_ERROR_RETURN:

  //
  // On any error or if the operation ID I calls for a key lookup only,
  // zero and release the context handle.
  //

  if ((context_p->proto == PKSS_PROTO_USR_KEY_GET) ||
      (*estatus_p != 0))
  {
   // sec_pkss_ctx_hdl_t_rundown (*ctx_handle_p);
   // *ctx_handle_p = 0;
  }

  //
  // Load output parameter.  If estatus at this point is non-zero, then an
  // error occurred, and there is no point to returning a login phase 2 msg.
  //

  msglen = sizeof (sec_pkss_msg_buf_t);

  if (*estatus_p == 0)
    msglen += to_asn_cipher.data_length;

  *from_pkss_p = (sec_pkss_msg_buf_p_t)rpc_ss_allocate (msglen);

  if (!(*from_pkss_p))
  {
    dce_svc_printf(PKS_S_NO_MEMORY_MSG);
    *estatus_p = pks_s_no_memory;
  }

  else
  {
    if (*estatus_p != 0)
    {
      (*from_pkss_p)->msg_len = 0;
      (*from_pkss_p)->msg_p = 0;
    }

    else
    {
      (*from_pkss_p)->msg_len = to_asn_cipher.data_length;
      (*from_pkss_p)->msg_p = (unsigned char *)(*from_pkss_p) +
                              sizeof (sec_pkss_msg_buf_t);
      (void) memcpy ((*from_pkss_p)->msg_p,
		     to_asn_cipher.data,
		     to_asn_cipher.data_length);

    } // End else path from if (*estatus_p != 0)

  } // End else path from if (!(*from_pkss_p))

  //
  // Note that with most other PKSS routines, we zero out buffers
  // as we go along and after we're done with them, and then a second time
  // in the error rundown code.  The way we do it here eliminates this 
  // redundancy, and this method should be applied elsewhere as time permits.
  //

  msglen = 0;
  tmp_version = 0;
  sym_alg_id_pad = 0;
  sym_alg_id_nopad = 0;
  sym_pad_key_len = 0;
  sym_nopad_key_len = 0;
  sec_pkss_zero_and_free (&dbrec);
  sec_pkss_zero_and_free (&sym_pad_iv_p);
  sec_pkss_zero_and_free (&sym_nopad_iv_p);
  sec_pkss_zero_and_free (&ciphertext_out);
  sec_pkss_zero_and_free (&plaintext_out);

  //
  // Unlock the mutex on the context handle.
  //

  *estatus_p = pthread_mutex_unlock (&(context_p->mutex));

#ifdef MALLOCTRACE
  malloc_mstatus ("Exiting sec_pkss_clt_pkey_get");
#endif

  return (rpc_s_ok);

} // End sec_pkss_clt_pkey_get


//
// Change asymmetric key pair/password.  The client generated the new key pair.
//

error_status_t sec_pkss_chg_key_clt
(
  handle_t IDL_handle,
  sec_pkss_ctx_hdl_t *ctx_handle_p,
  sec_pkss_msg_buf_t *to_pkss_p,
  sec_pkss_msg_buf_p_t *from_pkss_p,
  error_status_t *estatus_p
)

{
  //
  // Local variables
  //

  error_status_t pstatus = 0;
  r_buffer_t from_asn_cipher;
  PkssCltChgP5MsgC cltChgP5Msg;
  sec_pkss_context_t *context_p = 0;
  sec_bsafe_alg_type_t sym_alg_id_pad = 0;
  sec_bsafe_alg_type_t sym_alg_id_nopad = 0;
  unsigned32 sym_pad_key_len = 0;
  unsigned32 sym_nopad_key_len = 0;
  sec_pk_gen_data_t *sym_pad_iv_p = 0;
  sec_pk_gen_data_t *sym_nopad_iv_p = 0;
  sec_pk_gen_data_t signedtext = {0, 0};
  r_buffer_t from_asn_signed_rbuf;
  PkssCltChgP5SignedC cltChgP5Signed;
  unsigned32 sig_alg = 0;
  sec_pk_data_t signature = {0, 0};
  sec_pkss_dbrec_t old_dbrec = {0, 0};
  r_buffer_t old_dbrec_buf;
  PkssDBrecC oldDBrec;
  buffer_t old_kou_buf (SECURE);
  sec_pk_gen_data_t old_kou = {0, 0};
  buffer_t new_dbrec_buf (SECURE);
  sec_pk_gen_data_t to_be_signed = {0, 0};
  unsigned32 old_key_vno = 0;
  unsigned32 new_key_vno = 0;
  PkssDBrecC newDBrec;
  PkssCltChgP5DataC newCltChgP5Data;
  unsigned char *octet_p = 0;
  size_t octet_len = 0;
  buffer_t new_kou_buf;
  sec_pkss_dbrec_t new_dbrec = {0, 0};
  PkssCltChgP6DataC cltChgP6Data;
  buffer_t to_asn_plain (SECURE);
  sec_pk_gen_data_t plaintext_in = {0, 0};
  sec_pk_gen_data_t ciphertext_in = {0, 0};
  sec_pk_gen_data_t ciphertext_out = {0, 0};
  buffer_t to_asn_cipher (SECURE);
  unsigned32 msglen = 0;
  PkssCltChgP6MsgC cltChgP6Msg;

#ifdef MALLOCTRACE
  malloc_mstatus ("Entering sec_pkss_chg_key_clt");
#endif

  //
  // Set pointer to context block and acquire mutex.
  //

  context_p = (sec_pkss_context_t *)*ctx_handle_p;

  if (context_p->deleted)
    return (EINVAL);
  else
  {
    *estatus_p = pthread_mutex_lock (&(context_p->mutex));

    if (*estatus_p != 0)
      return (*estatus_p);
  }

  //
  // Initialize ASN.1 buffer object into which to move data
  // from RPC input buffer upon receipt from the PKSS client.
  //

  from_asn_cipher.data = to_pkss_p->msg_p;
  from_asn_cipher.data_length = to_pkss_p->msg_len;

  //
  // ASN.1-decode encrypted, signed parameter data 
  // [{[Kxu~]H1~:H2~:Kou~}Kxu]S received from the client.
  //

  *estatus_p = cltChgP5Msg.read (from_asn_cipher);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                  "status from ASN read : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  //
  // Obtain required symmetric key crypto info given a symmetric key
  // encryption algorithm identifier.  Note that it is the caller's
  // responsibility to free initial vector storage allocated in this
  // function.
  //

  *estatus_p = sec_pkss_get_sym_crypto_info (context_p->sym_alg,
                                             &sym_alg_id_pad,
                                             &sym_alg_id_nopad,
                                             &sym_pad_key_len,
                                             &sym_nopad_key_len,
                                             &sym_pad_iv_p,
                                             &sym_nopad_iv_p);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                  "status from sec_pkss_get_sym_crypto_info : 0x%x",
		  *estatus_p));

  if (*estatus_p != 0)
  {
    dce_svc_printf(PKS_S_SEC_BSAFE_MSG, *estatus_p);
    goto CHECK_ERROR_RETURN;
  }

  //
  // Obtain the ASN.1-decoded encrypted input parameter data
  // [{[Kxu~]H1~:H2~:Kou~}Kxu]S and decrypt it with S in context handle
  // to obtain the signed parameter data {[Kxu~]H1~:H2~:Kou~}Kxu.
  //

  *estatus_p = cltChgP5Msg.get_value (ciphertext_in.data, ciphertext_in.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                  "status from ASN get_value : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  *estatus_p = sec_bsafe_symmetric_decrypt (sym_alg_id_pad,
                                            context_p->crypto_key_p,
                                            sym_pad_iv_p,
                                            &ciphertext_in,
                                            &signedtext);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                  "status from sec_bsafe_symmetric_decrypt : 0x%x",
		  *estatus_p));

  if (*estatus_p != 0)
  {
    dce_svc_printf(PKS_S_SEC_BSAFE_MSG, *estatus_p);
    goto CHECK_ERROR_RETURN;
  }

  //
  // Initialize ASN.1 buffer object in anticipation of
  // ASN.1-decoding {[Kxu~]H1~:H2~:Kou~}Kxu.
  //

  from_asn_signed_rbuf.data = signedtext.data;
  from_asn_signed_rbuf.data_length = signedtext.len;

  //
  // ASN.1-decode signed input parameters.
  //

  *estatus_p = cltChgP5Signed.read (from_asn_signed_rbuf);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                  "status from ASN read : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  //
  // Obtain signature algorithm, signature, and ASN.1-encoded signed data
  // buffer in anticipation of verifying the signature.
  //

  // ****This needs to be an object identifier.

  *estatus_p = cltChgP5Signed.sigAlg.get_value (sig_alg);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                  "status from ASN get_value : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  *estatus_p = cltChgP5Signed.signature.get_value (signature.data,
						   signature.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                  "status from ASN get_value : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  //
  // Obtain current PKSS database record to get the old Kou with which to
  // verify the signed data received from the PKSS client.
  //

  *estatus_p = sec_pkss_db_lookup (context_p->dbkey_p, &old_dbrec);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                  "status from sec_pkss_db_lookup : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  //
  // Initialize ASN.1 buffer object in anticipation of ASN.1-
  // decoding the PKSS database record.  Recall that PKSS database records
  // are stored in their ASN.1 representation.
  //

  old_dbrec_buf.data = old_dbrec.asn_rec_p;
  old_dbrec_buf.data_length = old_dbrec.asn_rec_len;

  //
  // Initialize ASN.1 translation object and ASN.1-decode
  // the PKSS database record.
  //

  *estatus_p = oldDBrec.read (old_dbrec_buf);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN read : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  //
  // Obtain Kou from database record so that it may be passed as input to
  // the signature verifcation function below.
  //

  *estatus_p = oldDBrec.Kou.write (old_kou_buf);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN write : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  old_kou.data = old_kou_buf.data;
  old_kou.len = old_kou_buf.data_length;

  //
  // re-ASN.1-encode [Kxu~]H1~:H2~:Kou~ to get the buffer that the client 
  // signed in anticipation of verifying the signature.  Note that this is 
  // also the first three fields of the PKSS database record [Kxu]H1:H2:Kou:V.
  //

  *estatus_p = cltChgP5Signed.dataToBeSigned.write (new_dbrec_buf);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                  "status from ASN write : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }
  
  to_be_signed.data = new_dbrec_buf.data;
  to_be_signed.len = new_dbrec_buf.data_length;

  //
  // ****Verify that the sig_alg received from the client matches the signature
  // algorithm identifier that lives with the public key.
  //
  // Verify signature.  If the signature doesn't verify, return an error.
  //

  *estatus_p = sec_bsafe_verify_data (sig_alg,
				      &to_be_signed,
				      (sec_pk_data_t *)&old_kou,
				      &signature);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                  "status from sec_bsafe_verify_data : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    dce_svc_printf(PKS_S_SEC_BSAFE_MSG, *estatus_p);
    goto CHECK_ERROR_RETURN;
  }
  
  //
  // Obtain old key version number from the database record and increment
  // it in anticipation of constructing the new database record and database
  // record key.  Recall that integers in PKSS database records and database
  // keys are stored in network byte order.
  //

  *estatus_p = oldDBrec.version.get_value (old_key_vno);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                  "status from ASN get_value : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  old_key_vno = ntohl (old_key_vno);
  new_key_vno = old_key_vno + 1;

  //
  // ASN.1-encode the new key version number and add it to the new record
  // buffer.  Recall that Kou~ will be empty if only the password is changing
  // and the asymmetric key pair are remaining the same.  In that case obtain
  // the old Kou from the old database record, and insert it into the new
  // database record.
  //

  *estatus_p = newCltChgP5Data.read (new_dbrec_buf);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN read : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  *estatus_p = newCltChgP5Data.newKxuH1.get_value (octet_p, octet_len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN get_value : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  *estatus_p = newDBrec.KxuH1.set_value (octet_p, octet_len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN set_value : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  *estatus_p = newCltChgP5Data.newH2.get_value (octet_p, octet_len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN get_value : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  *estatus_p = newDBrec.H2.set_value (octet_p, octet_len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN set_value : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  *estatus_p = newCltChgP5Data.newKou.write (new_kou_buf);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN write : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  *estatus_p = newDBrec.Kou.read (new_kou_buf);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN read : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  *estatus_p = newDBrec.version.set_value (htonl (new_key_vno));

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN set_value : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

//  *estatus_p = newDBrec.Kou.write (new_kou_buf);
//
//    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
//                   "status from ASN write : 0x%x", *estatus_p));
//
//  if (new_kou_buf.data_length == 0)
//  {
//    *estatus_p = newDBrec.Kou.read (old_kou_buf);
//
//    if (*estatus_p != 0)
//    {
//      goto CHECK_ERROR_RETURN;
//
//        DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
//                       "status from ASN read : 0x%x", *estatus_p));
//    }
//  }

  new_dbrec_buf.clear();

  *estatus_p = newDBrec.write (new_dbrec_buf);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN write : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  //
  // Insert the new asymmetric key pair version number into database record
  // key in anticipation of inserting the new database record.  Observe that
  // we add the new record before deleting the old one to prevent the problem
  // created if a delete succeeded, but an insert failed.
  //

  context_p->dbkey_p->setKeyVno(new_key_vno);

  //
  // Insert the new database record [Kxu~]H1~:H2~:Kou~:V~.  Observe that the
  // insert of the new record is followed by a delete of the old record and
  // that the two operations are a single transaction.
  //

  new_dbrec.asn_rec_p = new_dbrec_buf.data;
  new_dbrec.asn_rec_len = new_dbrec_buf.data_length;

  pkss_db_p->txnBegin();

  *estatus_p = pkss_db_p->insert ((char *)new_dbrec.asn_rec_p,
                                      new_dbrec.asn_rec_len,
                                      (char *)context_p->dbkey_p->p()->data,
                                      context_p->dbkey_p->p()->len);
 
    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                  "status from pkss_db_p->insert : 0x%x", *estatus_p));

  //
  // If this portion of the transaction failed, roll it back and proceed
  // to the error processing exit; otherwise, continue this transaction.
  //

  if (*estatus_p != 0)
  {
    pkss_db_p->txnCancel();
    goto CHECK_ERROR_RETURN;
  }

  //
  // Insert the old asymmetric key pair version number into database record
  // key in anticipation of removing the old database record.
  //

  context_p->dbkey_p->setKeyVno(old_key_vno);

  //
  // Remove the old database record upon successful write of the new one.
  //

  *estatus_p = pkss_db_p->remove ((char *)context_p->dbkey_p->p()->data,
                                  context_p->dbkey_p->p()->len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                  "status from pkss_db_p->remove : 0x%x", *estatus_p));

   //
   // If this portion of the transaction succeeded, commit the entire
   // transaction; otherwise,  roll it back and proceed to the error
   // processing exit.

   if (*estatus_p == 0)
     pkss_db_p->txnEnd();
   else
   {
     pkss_db_p->txnCancel();
     goto CHECK_ERROR_RETURN;
   }

CHECK_ERROR_RETURN:

  sec_pkss_zero_and_free (&signedtext);

  //
  // Initialize ASN.1 translation object in anticipation
  // of ASN.1-encoding the PKSS usr chg key phase 6 message Nc:V:Status.
  //

  pstatus = cltChgP6Data.status.set_value (*estatus_p);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                  "status from ASN set_value : 0x%x", pstatus));

  if (pstatus != 0)
  {
    *estatus_p = pstatus;
    goto RUNDOWN;
  }

  pstatus = cltChgP6Data.nc.set_value (context_p->clt_nonce_p->data,
				       context_p->clt_nonce_p->len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                  "status from ASN set_value : 0x%x", pstatus));

  if (pstatus != 0)
  {
    *estatus_p = pstatus;
    goto RUNDOWN;
  }

  //
  // On an error return, do not return a new key version number.
  //

  if (*estatus_p != 0)
    new_key_vno = 0;

  pstatus = cltChgP6Data.version.set_value (new_key_vno);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                  "status from ASN set_value : 0x%x", pstatus));

  if (pstatus != 0)
  {
    *estatus_p = pstatus;
    goto RUNDOWN;
  }

  //
  // ASN.1-encode Nc:V:Status prior to encryption.
  //

  pstatus = cltChgP6Data.write (to_asn_plain);

     DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                  "status from ASN write : 0x%x", pstatus));

  if (pstatus != 0)
  {
    *estatus_p = pstatus;
    goto RUNDOWN;
  }

  //
  // Move contents of ASN.1 buffer object to sec_pk_gen_data_t struct in
  // anticipation of encrypting it.
  //

  plaintext_in.data = to_asn_plain.data;
  plaintext_in.len = to_asn_plain.data_length;

  //
  // Encrypt Nc:V:Status with the session key S to obtain [Nc:V:Status]S.
  //

  pstatus = sec_bsafe_symmetric_encrypt (sym_alg_id_pad,
                                            context_p->crypto_key_p,
                                            sym_pad_iv_p,
                                            &plaintext_in,
                                            &ciphertext_out);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                  "status from sec_bsafe_symmetric_encrypt : 0x%x",
		  pstatus));

  if (pstatus != 0)
  {
    *estatus_p = pstatus;
    dce_svc_printf(PKS_S_SEC_BSAFE_MSG, *estatus_p);
    goto RUNDOWN;
  }

  //
  // Zero encryption initial vectors.
  //

  sec_pkss_zero_and_free (&sym_pad_iv_p);
  sec_pkss_zero_and_free (&sym_nopad_iv_p);

  //
  // Instantiate and initialize ASN.1 translation object in anticipation of
  // ASN.1-encoding [Nc:V:Status]S prior to transmission to the client.
  //

  pstatus = cltChgP6Msg.set_value (ciphertext_out.data, ciphertext_out.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN set_value : 0x%x", pstatus));

  if (pstatus != 0)
  {
    *estatus_p = pstatus;
    goto RUNDOWN;
  }

  //
  // ASN.1-encode output parameters prior to transmission to the mgmt client.
  //

  pstatus = cltChgP6Msg.write (to_asn_cipher);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN write : 0x%x", pstatus));

  if (*estatus_p != 0)
  {
    *estatus_p = pstatus;
    goto RUNDOWN;
  }

RUNDOWN:

  //
  // Load output parameter.  If estatus at this point is non-zero, then an
  // error occurred, and there is no point to returning a login phase 2 msg.
  //

  msglen = sizeof (sec_pkss_msg_buf_t);

  if (*estatus_p == 0)
    msglen += to_asn_cipher.data_length;

  *from_pkss_p = (sec_pkss_msg_buf_p_t)rpc_ss_allocate (msglen);

  if (!(*from_pkss_p))
  {
    dce_svc_printf(PKS_S_NO_MEMORY_MSG);
    *estatus_p = pks_s_no_memory;
  }

  else
  {
    if (*estatus_p != 0)
    {
      (*from_pkss_p)->msg_len = 0;
      (*from_pkss_p)->msg_p = 0;
    }

    else
    {
      (*from_pkss_p)->msg_len = to_asn_cipher.data_length;
      (*from_pkss_p)->msg_p = (unsigned char *)(*from_pkss_p) +
                              sizeof (sec_pkss_msg_buf_t);
      (void) memcpy ((*from_pkss_p)->msg_p,
		     to_asn_cipher.data,
		     to_asn_cipher.data_length);

    } // End else path from if (*estatus_p != 0)

  } // End else path from if (!(*from_pkss_p))

  //
  // Zero and release sensitive data before returning.
  //

  sig_alg = 0;
  old_key_vno = 0;
  new_key_vno = 0;
  msglen = 0;
  sym_alg_id_pad = 0;
  sym_alg_id_nopad = 0;
  sym_pad_key_len = 0;
  sym_nopad_key_len = 0;
  sec_pkss_zero_and_free (&sym_pad_iv_p);
  sec_pkss_zero_and_free (&sym_nopad_iv_p);
  sec_pkss_zero_and_free (&signedtext);
  sec_pkss_zero_and_free (&old_dbrec);
  sec_pkss_zero_and_free (&signature);
  sec_pkss_zero_and_free (&ciphertext_out);
  // sec_pkss_ctx_hdl_t_rundown (*ctx_handle_p);
  // *ctx_handle_p = 0;

  //
  // Unlock the mutex on the context handle.
  //

  *estatus_p = pthread_mutex_unlock (&(context_p->mutex));

  //
  // Exit this function.
  //

#ifdef MALLOCTRACE
  malloc_mstatus ("Exiting sec_pkss_chg_key_clt");
#endif

  return (rpc_s_ok);

} // End sec_pkss_chg_key_clt


//
// Change asymmetric key pair/password.  The client requests that the server
// generate the new key pair.
//

error_status_t sec_pkss_chg_key_svr
(
  handle_t IDL_handle,
  sec_pkss_ctx_hdl_t *ctx_handle_p,
  sec_pkss_msg_buf_t *to_pkss_p,
  sec_pkss_msg_buf_p_t *from_pkss_p,
  error_status_t *estatus_p
)

{
  //
  // Local variables
  //

  r_buffer_t from_asn_cipher;
  PkssSvrChgP5MsgC svrChgP5Msg;
  sec_pkss_context_t *context_p = 0;
  sec_bsafe_alg_type_t sym_alg_id_pad = 0;
  sec_bsafe_alg_type_t sym_alg_id_nopad = 0;
  unsigned32 sym_pad_key_len = 0;
  unsigned32 sym_nopad_key_len = 0;
  sec_pk_gen_data_t *sym_pad_iv_p = 0;
  sec_pk_gen_data_t *sym_nopad_iv_p = 0;
  sec_pk_gen_data_t ciphertext_in = {0, 0};
  sec_pk_gen_data_t ciphertext_out = {0, 0};
  sec_pk_gen_data_t signedtext = {0, 0};
  r_buffer_t from_asn_signed_rbuf;
  PkssSvrChgP5SignedC svrChgP5Signed;
  unsigned32 sig_alg = 0;
  sec_pk_data_t signature = {0, 0};
  sec_pkss_dbrec_t old_dbrec = {0, 0};
  r_buffer_t old_dbrec_buf;
  PkssDBrecC oldDBrec;
  buffer_t old_kou_buf (SECURE);
  sec_pk_gen_data_t old_kou = {0, 0};
  buffer_t signed_asn_buf (SECURE);
  sec_pk_gen_data_t to_be_signed = {0, 0};
  unsigned32 old_key_vno = 0;
  unsigned32 new_key_vno = 0;
  sec_pk_gen_data_t old_h1 = {0, 0};
  sec_pk_gen_data_t old_h2 = {0, 0};
  char *newpwd_p = NULL;
  char key_chg_c = 0;
  unsigned key_chg = 0;
  sec_pk_gen_data_t pk_alg_id = {0, 0};
  unsigned32 pk_key_len = 0;
  sec_pk_data_t new_pub_key = {0, 0};
  sec_pk_data_t new_pvt_key = {0, 0};
  sec_pk_gen_data_t pvt_key_h1 = {0, 0};
  sec_pk_gen_data_t seed = {0, 0};
  sec_pk_gen_data_t rand = {0, 0};
  error_status_t pstatus = 0;
  sec_pk_gen_data_t new_h1 = {0, 0};
  sec_pk_gen_data_t new_h2 = {0, 0};
  sec_pk_gen_data_t new_kxu_h1 = {0, 0};
  PkssDBrecC newDBrec;
  r_buffer_t new_kou_rbuf;
  buffer_t new_dbrec_buf (SECURE);
  sec_pkss_dbrec_t new_dbrec = {0, 0};
  PkssSvrChgP6DataC svrChgP6Data;
  buffer_t to_asn_plain (SECURE);
  sec_pk_gen_data_t plaintext_in = {0, 0};
  PkssSvrChgP6MsgC svrChgP6Msg;
  buffer_t to_asn_cipher (SECURE);
  unsigned32 msglen = 0;
  
#ifdef MALLOCTRACE
  malloc_mstatus ("Entering sec_pkss_chg_key_svr");
#endif

  //
  // Set pointer to context block and acquire mutex.
  //

  context_p = (sec_pkss_context_t *)*ctx_handle_p;

  if (context_p->deleted)
    return (EINVAL);
  else
  {
    *estatus_p = pthread_mutex_lock (&(context_p->mutex));

    if (*estatus_p != 0)
      return (*estatus_p);
  }

  //
  // Instantiate and initialize ASN.1 buffer object into which to move data
  // from RPC input buffer upon receipt from the PKSS client.
  //

  from_asn_cipher.data = to_pkss_p->msg_p;
  from_asn_cipher.data_length = to_pkss_p->msg_len;

  //
  // ASN.1-decode encrypted, signed  parameter data 
  //[{H1:H2:newpwd:key-chg?}Kxu]S received from the client.
  //

  *estatus_p = svrChgP5Msg.read (from_asn_cipher);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN read : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  //
  // Obtain required symmetric key crypto info given a symmetric key
  // encryption algorithm identifier.  Note that it is the caller's
  // responsibility to free initial vector storage allocated in this
  // function.
  //

  *estatus_p = sec_pkss_get_sym_crypto_info (context_p->sym_alg,
                                          &sym_alg_id_pad,
                                          &sym_alg_id_nopad,
                                          &sym_pad_key_len,
                                          &sym_nopad_key_len,
                                          &sym_pad_iv_p,
                                          &sym_nopad_iv_p);
 
    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_pkss_get_sym_crypto_info : 0x%x",
		   *estatus_p));
 
  if (*estatus_p != 0)
  {
    dce_svc_printf(PKS_S_SEC_BSAFE_MSG, *estatus_p);
    goto CHECK_ERROR_RETURN;
  }

  //
  // Obtain the ASN.1-decoded encrypted input parameter data
  // [{H1:H2:newpwd:key-chg?:A:L}Kxu]S and decrypt it with S in context handle
  // to obtain the signed parameter data {H1:H2:newpwd:key-chg?:A:L}Kxu.
  //

  *estatus_p = svrChgP5Msg.get_value (ciphertext_in.data, ciphertext_in.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN get_value : 0x%x", *estatus_p));
 
  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  *estatus_p = sec_bsafe_symmetric_decrypt (sym_alg_id_pad,
                                           context_p->crypto_key_p,
                                           sym_pad_iv_p,
                                           &ciphertext_in,
                                           &signedtext);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_bsafe_symmetric_decrypt : 0x%x",
		   *estatus_p));
 
   if (*estatus_p != 0)
  {
    dce_svc_printf(PKS_S_SEC_BSAFE_MSG, *estatus_p);
    goto CHECK_ERROR_RETURN;
  }

  //
  // Instantiate and initialize ASN.1 buffer object in anticipation of
  // ASN.1-decoding {H1:H2:newpwd:key-chg?:A:L}Kxu.
  //

  from_asn_signed_rbuf.data = signedtext.data;
  from_asn_signed_rbuf.data_length = signedtext.len;

  //
  // ASN.1-decode signed input parameters.
  //

  *estatus_p = svrChgP5Signed.read (from_asn_signed_rbuf);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN read : 0x%x", *estatus_p));
 
  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  //
  // Obtain signature algorithm, signature, and ASN.1-encoded signed data
  // buffer in anticipation of verifying the signature.
  //

  // ****This needs to be an object identifier.

  *estatus_p = svrChgP5Signed.sigAlg.get_value (sig_alg);

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  *estatus_p = svrChgP5Signed.signature.get_value (signature.data,
						   signature.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN get_value : 0x%x", *estatus_p));
 
  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  //
  // Obtain current PKSS database record to get the old Kou with which to
  // verify the signed data received from the PKSS client.
  //

  *estatus_p = sec_pkss_db_lookup (context_p->dbkey_p, &old_dbrec);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_pkss_db_lookup : 0x%x", *estatus_p));
 
  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  //
  // Initialize ASN.1 buffer object in anticipation of ASN.1-
  // decoding the PKSS database record.  Recall that PKSS database records
  // are stored in their ASN.1 representation.
  //

  old_dbrec_buf.data = old_dbrec.asn_rec_p;
  old_dbrec_buf.data_length = old_dbrec.asn_rec_len;

  //
  // Initialize ASN.1 translation object and ASN.1-decode
  // the PKSS database record.
  //

  *estatus_p = oldDBrec.read (old_dbrec_buf);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN read : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  //
  // Obtain Kou from database record so that it may be passed as input to
  // the signature verifcation function below.
  //

  *estatus_p = oldDBrec.Kou.write (old_kou_buf);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN write : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  old_kou.data = old_kou_buf.data;
  old_kou.len = old_kou_buf.data_length;

  //
  // re-ASN.1-encode H1:H2:newpwd:key-chg?:A:L to get the buffer that 
  // the client signed in anticipation of verifying the signature.
  //

  *estatus_p = svrChgP5Signed.dataToBeSigned.write (signed_asn_buf);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN write : 0x%x", *estatus_p));
 
  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  to_be_signed.data = signed_asn_buf.data;
  to_be_signed.len = signed_asn_buf.data_length;

  //
  // ****Verify that the sig_alg received from the client matches the signature
  // algorithm identifier that lives with the public key.
  //
  // Verify signature.  If the signature doesn't verify, return an error.
  //

  *estatus_p = sec_bsafe_verify_data (sig_alg,
                                      &to_be_signed,
                                      (sec_pk_data_t *)&old_kou,
                                      &signature);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_bsafe_verify_data : 0x%x", *estatus_p));
 
  if (*estatus_p != 0)
  {
    dce_svc_printf(PKS_S_SEC_BSAFE_MSG, *estatus_p);
    goto CHECK_ERROR_RETURN;
  }

  //
  // Obtain old key version number from the database record and increment
  // it in anticipation of constructing the new database record and database
  // record key.  Recall that integers in PKSS database records and database
  // keys are stored in network byte order.
  //

  *estatus_p = oldDBrec.version.get_value (old_key_vno);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN get_value : 0x%x", *estatus_p));
 
  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  old_key_vno = ntohl (old_key_vno);
  new_key_vno = old_key_vno + 1;

  //
  // ASN.1-decode H1:H2:newpwd:key-chg?:A:L into its component elements.
  //

  *estatus_p = svrChgP5Signed.dataToBeSigned.oldH1.get_value (old_h1.data,
							      old_h1.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN get_value : 0x%x", *estatus_p));
 
  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  *estatus_p = svrChgP5Signed.dataToBeSigned.oldH2.get_value (old_h2.data,
							      old_h2.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN get_value : 0x%x", *estatus_p));
 
  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  *estatus_p = svrChgP5Signed.dataToBeSigned.newPwd.get_value (newpwd_p);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN get_value : 0x%x", *estatus_p));
 
  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  *estatus_p = svrChgP5Signed.dataToBeSigned.keyChg.get_value (key_chg_c);

  key_chg = (key_chg_c != 0);

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  *estatus_p = svrChgP5Signed.dataToBeSigned.pkAlgID.get_value (pk_alg_id.data,
							        pk_alg_id.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN get_value : 0x%x", *estatus_p));
 
  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  *estatus_p = svrChgP5Signed.dataToBeSigned.pkKeyLen.get_value (pk_key_len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN get_value : 0x%x", *estatus_p));
 
  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  //
  // Generate a new asymmetric key pair Kxu~/Kou~ if instructed by the client;
  // otherwise obtain old asymmetric key pair Kxu/Kou from the current PKSS
  // database record.
  //

  if (key_chg == 0)
  {
    new_pub_key.len = old_kou.len;
    new_pub_key.data = (unsigned char *) calloc (1, new_pub_key.len);

    if (!(new_pub_key.data))
    {
      dce_svc_printf(PKS_S_NO_MEMORY_MSG);
      *estatus_p = pks_s_no_memory;
      goto CHECK_ERROR_RETURN;
    }

    (void) memcpy (new_pub_key.data, old_kou.data, old_kou.len);

    *estatus_p = oldDBrec.KxuH1.get_value (pvt_key_h1.data, pvt_key_h1.len);

    if (*estatus_p != 0)
    {
      goto CHECK_ERROR_RETURN;
    }
    
    *estatus_p = sec_bsafe_symmetric_decrypt (sym_alg_id_nopad,
                                              &old_h1,
                                              sym_nopad_iv_p,
                                              &pvt_key_h1,
                                              (sec_pk_gen_data_t *)
					      &new_pvt_key);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_bsafe_symmetric_decrypt : 0x%x",
		   *estatus_p));
 
    if (*estatus_p != 0)
    {
      dce_svc_printf(PKS_S_SEC_BSAFE_MSG, *estatus_p);
      goto CHECK_ERROR_RETURN;
    }

  } // End if (key_chg == 0)

  else
  {
    rand.len = 64;
    rand.data = (unsigned char *) malloc (rand.len);

    if (!(rand.data))
    {
      dce_svc_printf(PKS_S_NO_MEMORY_MSG);
      *estatus_p = pks_s_no_memory;
      goto CHECK_ERROR_RETURN;
    }

    seed.len = rand.len;
    seed.data = (unsigned char *) malloc (seed.len);

    if (!(seed.data))
    {
      dce_svc_printf(PKS_S_NO_MEMORY_MSG);
      *estatus_p = pks_s_no_memory;
      goto CHECK_ERROR_RETURN;
    }

    *estatus_p = sec_bsafe_random_bytes (&seed, &rand);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_bsafe_random_bytes : 0x%x", *estatus_p));

    if (*estatus_p != 0)
    {
      goto CHECK_ERROR_RETURN;
    }

    sec_pkss_zero_and_free (&seed);

    *estatus_p = sec_bsafe_gen_pub_key (pk_key_len,
                                        &rand,
                                        &new_pub_key,
                                        &new_pvt_key);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_bsafe_gen_pub_key : 0x%x", *estatus_p));

    if (*estatus_p != 0)
    {
      dce_svc_printf(PKS_S_SEC_BSAFE_MSG, *estatus_p);
      goto CHECK_ERROR_RETURN;
    }

    sec_pkss_zero_and_free (&rand);

  } // End else path of if (key_chg)

  //
  // Obtain new hashes H1~ and H2~ from newpwd_p.  Note that it is the
  // caller's responsibility to free the storage allocated to contain
  // the hashes.  Recall that if only the asymmetric key pair is changing,
  // but the password is staying the same that H1 == H1~.
  //

  if (newpwd_p == 0)
  {
    new_h1.len = old_h1.len;
    new_h1.data = old_h1.data;
    new_h2.len = old_h2.len;
    new_h2.data = old_h2.data;
  }

  else
  {
    *estatus_p = sec_pkss_get_pwd_hashes (newpwd_p,
					  (const char *)
					  context_p->dbkey_p->getName(),
                                          sym_pad_key_len,
                                          &new_h1,
					  &new_h2);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_pkss_get_pwd_hashes : 0x%x", *estatus_p));

    if (*estatus_p != 0)
    {
      goto CHECK_ERROR_RETURN;
    }

  } // End else path of if (newpwd_p == 0)

  //
  // Encrypt Kxu with H1~ to obtain [Kxu]H1~.
  //

  *estatus_p = sec_bsafe_symmetric_encrypt (sym_alg_id_pad,
                                            &new_h1,
                                            sym_pad_iv_p,
                                            (sec_pk_gen_data_t *)&new_pvt_key,
                                            &new_kxu_h1);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_bsafe_symmetric_encrypt : 0x%x",
		   *estatus_p));

  if (*estatus_p != 0)
  {
    dce_svc_printf(PKS_S_SEC_BSAFE_MSG, *estatus_p);
    goto CHECK_ERROR_RETURN;
  }

  //
  // Compose new user security data record [Kxu]H1:H2:Kou and insert into
  // PKSS database.  Recall that on a key change operation it is necessary
  // to increment the asymmetric key version number.
  //

  *estatus_p = newDBrec.KxuH1.set_value (new_kxu_h1.data, new_kxu_h1.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN set_value : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  *estatus_p = newDBrec.H2.set_value (new_h2.data, new_h2.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN set_value : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  *estatus_p = newDBrec.version.set_value (htonl (new_key_vno));

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN set_value : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  new_kou_rbuf.data = new_pub_key.data;
  new_kou_rbuf.data_length = new_pub_key.len;

  *estatus_p = newDBrec.Kou.read (new_kou_rbuf);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN read : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  *estatus_p = newDBrec.write (new_dbrec_buf);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN write : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  //
  // Insert the new asymmetric key pair version number into database record
  // key in anticipation of inserting the new database record.
  //

  context_p->dbkey_p->setKeyVno (new_key_vno);

  //
  // Insert the new record Userkey:[Kxu]H1:H2:Kou into the database.
  // Observe that the insert of the new record is followed by a delete
  // of the old record and that the two operations are a single transaction.
  //

  new_dbrec.asn_rec_p = new_dbrec_buf.data;
  new_dbrec.asn_rec_len = new_dbrec_buf.data_length;

  pkss_db_p->txnBegin();

  *estatus_p = pkss_db_p->insert ((char *)new_dbrec.asn_rec_p,
                                  new_dbrec.asn_rec_len,
                                  (char *)context_p->dbkey_p->p()->data,
                                  context_p->dbkey_p->p()->len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from pkss_db_p->insert : 0x%x", *estatus_p));

  //
  // If this portion of the transaction failed, roll it back and proceed
  // to the error processing exit; otherwise, continue this transaction.
  //

  if (*estatus_p != 0)
  {
    pkss_db_p->txnCancel();
    goto CHECK_ERROR_RETURN;
  }

  //
  // Insert the old asymmetric key pair version number into database record
  // key in anticipation of removing the old database record.
  //

  context_p->dbkey_p->setKeyVno(old_key_vno);

  //
  // Remove the old database record upon successful write of the new one.
  //

  *estatus_p = pkss_db_p->remove ((char *)context_p->dbkey_p->p()->data,
                                  context_p->dbkey_p->p()->len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                  "status from pkss_db_p->remove : 0x%x", *estatus_p));

   //
   // If this portion of the transaction succeeded, commit the entire
   // transaction; otherwise,  roll it back and proceed to the error
   // processing exit.

   if (*estatus_p == 0)
     pkss_db_p->txnEnd();
   else
   {
     pkss_db_p->txnCancel();
     goto CHECK_ERROR_RETURN;
   }

  *estatus_p = svrChgP6Data.version.set_value (new_key_vno);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN set_value : 0x%x", pstatus));

  if (*estatus_p != 0)
  {
    goto RUNDOWN;
  }

  new_kou_rbuf.data = new_pub_key.data;
  new_kou_rbuf.data_length = new_pub_key.len;

  *estatus_p = svrChgP6Data.newKou.read (new_kou_rbuf);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN read : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

CHECK_ERROR_RETURN:

  sec_pkss_zero_and_free (&new_kxu_h1);

  //
  // Initialize ASN.1 translation object in anticipation
  // of ASN.1-encoding the PKSS usr chg key phase 6 message Nc:V:Status.
  //

  pstatus = svrChgP6Data.status.set_value (*estatus_p);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN set_value : 0x%x", pstatus));

  if (pstatus != 0)
  {
    *estatus_p = pstatus;
    goto RUNDOWN;
  }

  pstatus = svrChgP6Data.nc.set_value (context_p->clt_nonce_p->data,
                                       context_p->clt_nonce_p->len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN set_value : 0x%x", pstatus));

  if (pstatus != 0)
  {
    *estatus_p = pstatus;
    goto RUNDOWN;
  }

  //
  // ASN.1-encode Nc:V:Status:Kou~ prior to encryption.
  //

  pstatus = svrChgP6Data.write (to_asn_plain);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN write : 0x%x", pstatus));

  if (pstatus != 0)
  {
    *estatus_p = pstatus;
    goto RUNDOWN;
  }

  //
  // Move contents of ASN.1 buffer object to sec_pk_gen_data_t struct in
  // anticipation of encrypting it.
  //

  plaintext_in.data = to_asn_plain.data;
  plaintext_in.len = to_asn_plain.data_length;

  //
  // Encrypt Nc:V:Status:Kou~ with the session key S to obtain
  // [Nc:V:Status:Kou~]S.
  //

  pstatus = sec_bsafe_symmetric_encrypt (sym_alg_id_pad,
                                            context_p->crypto_key_p,
                                            sym_pad_iv_p,
                                            &plaintext_in,
                                            &ciphertext_out);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_bsafe_symmetric_encrypt : 0x%x",
		   pstatus));

  if (pstatus != 0)
  {
    *estatus_p = pstatus;
    dce_svc_printf(PKS_S_SEC_BSAFE_MSG, *estatus_p);
    goto RUNDOWN;
  }

  //
  // Zero buffers with sensitive data.
  //

  sec_pkss_zero_and_free (&sym_pad_iv_p);
  sec_pkss_zero_and_free (&sym_nopad_iv_p);
  sec_pkss_zero_and_free (&new_pub_key);
  sec_pkss_zero_and_free (&new_pvt_key);
  sec_pkss_zero_and_free (&old_dbrec);

  //
  // Initialize ASN.1 translation object in anticipation of
  // ASN.1-encoding [Nc:V:Status:Kou]S prior to transmission to the client.
  //

  pstatus = svrChgP6Msg.set_value (ciphertext_out.data, ciphertext_out.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN set_value : 0x%x", pstatus));

  if (pstatus != 0)
  {
    *estatus_p = pstatus;
    goto RUNDOWN;
  }

  //
  // ASN.1-encode output parameters [Nc:V:Status:Kou~]S prior to 
  // transmission to the mgmt client.
  //

  pstatus = svrChgP6Msg.write (to_asn_cipher);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN write : 0x%x", pstatus));

  if (pstatus != 0)
  {
    *estatus_p = pstatus;
    goto RUNDOWN;
  }

  //
  // Zero and release other buffers containing sensitive data.
  //

  sec_pkss_zero_and_free (&ciphertext_out);
  sec_pkss_zero_and_free (&new_pvt_key);
  sec_pkss_zero_and_free (&new_pub_key);
  sec_pkss_zero_and_free (&signedtext);

RUNDOWN:

  //
  // Load output parameter.  If estatus at this point is non-zero, then an
  // error occurred, and there is no point to returning a login phase 2 msg.
  //

  msglen = sizeof (sec_pkss_msg_buf_t);

  if (*estatus_p == 0)
    msglen += to_asn_cipher.data_length;

  *from_pkss_p = (sec_pkss_msg_buf_p_t)rpc_ss_allocate (msglen);

  if (!(*from_pkss_p))
  {
    dce_svc_printf(PKS_S_NO_MEMORY_MSG);
    *estatus_p = pks_s_no_memory;
  }

  else
  {
    if (*estatus_p != 0)
    {
      (*from_pkss_p)->msg_len = 0;
      (*from_pkss_p)->msg_p = 0;
    }

    else
    {
      (*from_pkss_p)->msg_len = to_asn_cipher.data_length;
      (*from_pkss_p)->msg_p = (unsigned char *)(*from_pkss_p) +
                              sizeof (sec_pkss_msg_buf_t);
      (void) memcpy ((*from_pkss_p)->msg_p,
		     to_asn_cipher.data,
		     to_asn_cipher.data_length);

    } // End else path from if (*estatus_p != 0)

  } // End else path from if (!(*from_pkss_p))

  //
  // Zero and release sensitive data.
  //

  msglen = 0;
  sig_alg = 0;
  key_chg = 0;
  pk_key_len = 0;
  old_key_vno = 0;
  new_key_vno = 0;
  sym_alg_id_pad = 0;
  sym_alg_id_nopad = 0;
  sym_pad_key_len = 0;
  sym_nopad_key_len = 0;
  if (newpwd_p)
  {
    sec_pkss_zero_and_free (&new_h1);
    sec_pkss_zero_and_free (&new_h2);
  }
  sec_pkss_zero_and_free (&sym_pad_iv_p);
  sec_pkss_zero_and_free (&sym_nopad_iv_p);
  sec_pkss_zero_and_free (&signedtext);
  sec_pkss_zero_and_free (&old_dbrec);
  sec_pkss_zero_and_free (&new_pvt_key);
  sec_pkss_zero_and_free (&new_pub_key);
  sec_pkss_zero_and_free (&seed);
  sec_pkss_zero_and_free (&rand);
  sec_pkss_zero_and_free (&new_kxu_h1);
  sec_pkss_zero_and_free (&ciphertext_out);
  // sec_pkss_ctx_hdl_t_rundown (*ctx_handle_p);
  // *ctx_handle_p = 0;
  
  //
  // Unlock the mutex on the context handle.
  //

  *estatus_p = pthread_mutex_unlock (&(context_p->mutex));

  //
  // Exit this function.
  //

#ifdef MALLOCTRACE
  malloc_mstatus ("Exiting sec_pkss_chg_key_svr");
#endif

  return (rpc_s_ok);

} // End sec_pkss_chg_key_svr
 
