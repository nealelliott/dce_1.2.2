//
// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// All Rights Reserved
//
// HISTORY
// $Log: sec_pkss_mgmt_mgr.cxx,v $
// Revision 1.1.2.2  1996/11/13  20:41:29  arvind
// 	PKSS bug fix from DEC
// 	[1996/11/03  23:40 UTC  arvind  /main/DCE_1.2.2/arvind_capi2/1]
//
// Revision 1.1.2.1  1996/10/03  20:32:04  arvind
// 	OSF DCE 1.2.2 Drop 4
// 	[1996/10/03  19:31:47  arvind]
// 
// Revision /main/DCE_1.2.2/1  1996/09/09  21:30 UTC  arvind
// 	PKSS drop from DEC (DCE1.2.2)
// 	[1996/08/30  15:55 UTC  arvind  /main/arvind_pkss/1]
// 
// $EndLog$
//
// (c) Copyright 1996,
// Digital Equipment Corporation, Maynard, Massachusetts, USA
// All Rights Reserved
//
// PKSS management manager functions - see sec_pkss_mgmt_api.h for input/output
// descriptions
//

#include <stdlib.h>
#include <netinet/in.h>
#include <dce/rpc.h>
#include "gdb44_methods.h"
#include "pkss_acl_mgr.h"
#include "sec_pkss_dbkey_class.h"
#include "sec_bsafe.h"
#include "sec_pkss_asn.h"
#include "sec_pkss_lits.h"
#include "sec_pkss_util.h"
#include "sec_pkss_svr_util.h"
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
// Construct new database record in the case that the PKSS mgmt client
// generated the new asymmetric key pair and sent it to the PKSS in the
// input parameters.
//
// Note that this module is intended ONLY for use by the PKSS svr manager.
// This function shall NOT be part of an exposed API.
//
// Called by: sec_pkss_mgmt_mgr_clt_util
//

static error_status_t sec_pkss_mgmt_get_dbrec
(
  PkssMgmtCltP3DataC *mgmtP3DataP,
  unsigned32 key_vno,
  sec_pkss_dbrec_t **dbrec_p
)

{
  //
  // Local variables
  //

  error_status_t estatus = 0;
  sec_pk_gen_data_t new_kxu_h1 = {0, 0};
  sec_pk_gen_data_t new_h2 = {0, 0};
  buffer_t kou_data_buf (SECURE);
  sec_pk_gen_data_t new_kou = {0, 0};
  PkssDBrecC newDBrec;
  buffer_t new_dbrec_buf (SECURE);

  //
  // Extract the PKSS database record elements from the input parameters.
  //

  estatus = mgmtP3DataP->kxuH1.get_value (new_kxu_h1.data, new_kxu_h1.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN get_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto EXIT;
  }

  estatus = mgmtP3DataP->H2.get_value (new_h2.data, new_h2.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN get_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto EXIT;
  }

  estatus = mgmtP3DataP->kou.write (kou_data_buf);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN write : 0x%x", estatus));

  new_kou.data = kou_data_buf.data;
  new_kou.len = kou_data_buf.data_length;

  //
  // Construct the PKSS database record from the input parameters.
  //

  estatus = newDBrec.KxuH1.set_value (new_kxu_h1.data, new_kxu_h1.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from DBrec set_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto EXIT;
  }
  
  estatus = newDBrec.H2.set_value (new_h2.data, new_h2.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from DBrec set_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto EXIT;
  }

  estatus = newDBrec.Kou.read (kou_data_buf);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from DBrec read : 0x%x", estatus));

  if (estatus != 0)
  {
    goto EXIT;
  }

  estatus = newDBrec.version.set_value (htonl (key_vno));

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from DBrec set_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto EXIT;
  }

  estatus = newDBrec.write (new_dbrec_buf);
  
    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from DBrec write : 0x%x", estatus));

  if (estatus != 0)
  {
    goto EXIT;
  }

  *dbrec_p = (sec_pkss_dbrec_t *) calloc (1, sizeof (sec_pkss_dbrec_t));

  if (!(*dbrec_p))
  {
    dce_svc_printf(PKS_S_NO_MEMORY_MSG);
    estatus = pks_s_no_memory;
    goto EXIT;
  }

  (*dbrec_p)->asn_rec_len = new_dbrec_buf.data_length;

  (*dbrec_p)->asn_rec_p = (unsigned char *) calloc (1, (*dbrec_p)->asn_rec_len);
  
  if (!((*dbrec_p)->asn_rec_p))
  {
    dce_svc_printf(PKS_S_NO_MEMORY_MSG);
    estatus = pks_s_no_memory;
    (*dbrec_p)->asn_rec_len = 0;
    free (*dbrec_p);
    *dbrec_p = 0;
    goto EXIT;
  }

  (void) memcpy ((*dbrec_p)->asn_rec_p,
		 new_dbrec_buf.data,
                 (*dbrec_p)->asn_rec_len);

EXIT:

  //
  // Exit this function.
  //

  return (estatus);

} // End sec_pkss_mgmt_get_dbrec


//
// This is a generic PKSS mgmt manager function that does the work for a
// change operation.
//
// Note that this module is intended ONLY for use by the PKSS svr manager.
// This function shall NOT be part of an exposed API.
//
// Called by:  sec_pkss_mgmt_mod_user_clt
//             sec_pkss_mgmt_mod_user_svr
//

static error_status_t sec_pkss_mgmt_get_old_vno
(
  PkssDbKeyC *dbKeyP,
  unsigned32 *old_key_vno_p
)

{
  //
  // Local variables
  //

  error_status_t estatus;
  sec_pkss_dbrec_t old_dbrec = {0, 0};
  r_buffer_t old_dbrec_asn;
  PkssDBrecC oldDBrec;

  estatus = sec_pkss_db_lookup (dbKeyP, &old_dbrec);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_pkss_db_lookup : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Initialize ASN.1 buffer object in anticipation of ASN.1-
  // decoding the old key version number from the PKSS database record
  // returned above.  Recall that PKSS database records are stored in their
  // ASN.1 representation.
  //

  old_dbrec_asn.data = old_dbrec.asn_rec_p;
  old_dbrec_asn.data_length = old_dbrec.asn_rec_len;

  //
  // Initialize ASN.1 translation object and ASN.1-decode
  // the PKSS database record key version number.
  //

  oldDBrec.read (old_dbrec_asn);

  estatus = oldDBrec.version.get_value (*old_key_vno_p);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from DBrec get_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  *old_key_vno_p = ntohl (*old_key_vno_p);

  //
  // On success, we fall through to rundown code to do buffer cleanup
  //

rundown:

  sec_pkss_zero_and_free (&old_dbrec);

  //
  // Exit this function.
  //

  return (estatus);

} // End sec_pkss_mgmt_get_old_vno


//
// This is a generic PKSS mgmt manager function that does the work for an
// add or change operation when the client supplies the asymmetric key pair.
//
// Note that this module is intended ONLY for use by the PKSS svr manager.
// This function shall NOT be part of an exposed API.
//
// Called by:  sec_pkss_mgmt_add_user_clt
//             sec_pkss_mgmt_mod_user_clt
//

static error_status_t sec_pkss_mgmt_mgr_clt_util
(
  handle_t handle,
  sec_pkss_context_t *context_p,
  sec_pkss_msg_buf_t *to_pkss_p,
  sec_pkss_msg_buf_p_t *from_pkss_p
)

{
  //
  // Local variables
  //

  error_status_t estatus = 0;
  error_status_t pstatus = 0;
  r_buffer_t from_asn_cipher;
  PkssMgmtCltP3MsgBaseC *mgmtP3MsgP = 0;
  sec_bsafe_alg_type_t sym_alg_id_pad = 0;
  sec_bsafe_alg_type_t sym_alg_id_nopad = 0;
  unsigned32 sym_pad_key_len = 0;
  unsigned32 sym_nopad_key_len = 0;
  sec_pk_gen_data_t *sym_pad_iv_p = 0;
  sec_pk_gen_data_t *sym_nopad_iv_p = 0;
  sec_pk_gen_data_t plaintext_in = {0, 0};
  sec_pk_gen_data_t plaintext_out = {0, 0};
  sec_pk_gen_data_t ciphertext_in = {0, 0};
  sec_pk_gen_data_t ciphertext_out = {0, 0};
  PkssMgmtCltP3DataC mgmtP3Data;
  r_buffer_t from_asn_plain;
  unsigned32 tmp_key_vno = 0;
  char *tmp_domain_p = NULL;
  char *tmp_name_p = NULL;
  PkssDbKeyC *dbKeyP = 0;
  unsigned32 old_key_vno = 0;
  unsigned32 new_key_vno = 0;
  sec_pkss_dbrec_t *dbrec_p = 0;
  PkssMgmtCltP4DataC mgmtP4Data;
  buffer_t to_asn_plain (SECURE);
  PkssMgmtCltP4MsgBaseC *mgmtP4MsgP = 0;
  buffer_t to_asn_cipher;
  unsigned32 msglen = 0;
  db_operation_t dbop;
  boolean32 auth_yn = 0;

#ifdef MALLOCTRACE
  malloc_mstatus ("Entering sec_pkss_mgmt_mgr_clt_util");
#endif

  //
  // Acquire context block mutex.
  //

  if (context_p->deleted)
    return (EINVAL);
  else
  {
    estatus = pthread_mutex_lock (&(context_p->mutex));

    if (estatus != 0)
      return (estatus);
  }

  //
  // Instantiate and initialize ASN.1 buffer object into which to move data
  // from RPC input buffer upon receipt from the PKSS management client.
  //

  from_asn_cipher.data = to_pkss_p->msg_p;
  from_asn_cipher.data_length = to_pkss_p->msg_len;

  //
  // Instantiate ASN.1 translation object in anticipation of ASN.1-decoding
  // the input parameters received from the PKSS mgmt client.
  //

  switch (context_p->proto)
  {
    case PKSS_PROTO_MGMT_ADD_USR_CLT:
      mgmtP3MsgP = new PkssMgmtCltAddP3MsgC;
      dbop = DBOP_CREATE_PRINCIPAL;
      break;

    case PKSS_PROTO_MGMT_MOD_USR_CLT:
      mgmtP3MsgP = new PkssMgmtCltModP3MsgC;
      dbop = DBOP_ALTER_PRINCIPAL;
      break;

    default:
      dce_svc_printf(PKS_S_INVALID_ARG_1_PARM_MSG, context_p->proto);
      estatus = pks_s_invalid_arg_1_parm;
      goto CHECK_ERROR_RETURN;

  } // End switch (context_p->ctx_proto)

  if (!(mgmtP3MsgP))
  {
    dce_svc_printf(PKS_S_NO_MEMORY_MSG);
    estatus = pks_s_no_memory;
    goto CHECK_ERROR_RETURN;
  }

  //
  // ASN.1-decode encrypted input parameter data [Userkey:[Kxu]H1:H2:Kou]D.
  //

  estatus = mgmtP3MsgP->read (from_asn_cipher);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN read : 0x%x", estatus));

  if (estatus != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  //
  // Obtain required symmetric key crypto info given a symmetric key
  // encryption algorithm identifier.  Note that it is the caller's
  // responsibility to free initial vector storage allocated in this
  // function.
  //

  estatus = sec_pkss_get_sym_crypto_info (context_p->sym_alg,
                                          &sym_alg_id_pad,
                                          &sym_alg_id_nopad,
                                          &sym_pad_key_len,
                                          &sym_nopad_key_len,
                                          &sym_pad_iv_p,
                                          &sym_nopad_iv_p);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_pkss_get_sym_crypto_info : 0x%x", estatus));

  if (estatus != 0)
  {
    dce_svc_printf(PKS_S_SEC_BSAFE_MSG, estatus);
    goto CHECK_ERROR_RETURN;
  }

  //
  // Obtain the ASN.1-decoded encrypted input parameter data and decrypt
  // it with Diffie-Hellman key D.
  //

  mgmtP3MsgP->get_value (ciphertext_in.data, ciphertext_in.len);

  estatus = sec_bsafe_symmetric_decrypt (sym_alg_id_pad,
                                         context_p->crypto_key_p,
                                         sym_pad_iv_p,
                                         &ciphertext_in,
                                         &plaintext_out);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_bsafe_symmetric_decrypt : 0x%x", estatus));

  if (estatus != 0)
  {
    dce_svc_printf(PKS_S_SEC_BSAFE_MSG, estatus);
    goto CHECK_ERROR_RETURN;
  }

  //
  // Delete the object pointed to by mgmtP3MsgP.
  //

  delete mgmtP3MsgP;
  mgmtP3MsgP = 0;

  //
  // Initialize ASN.1 buffer object into which to move data
  // in anticipation of ASN.1 translating the decrypted input parameters
  // Userkey:[Kxu]H1:H2:Kou.
  //

  from_asn_plain.data = plaintext_out.data;
  from_asn_plain.data_length = plaintext_out.len;

  //
  // ASN.1-decode input parameters.
  //

  estatus = mgmtP3Data.read (from_asn_plain);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN read : 0x%x", estatus));

  if (estatus != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  //
  // Obtain from the input parameters the userKey portion of the record
  // to be added/modified
  //

  estatus = mgmtP3Data.userKey.uname.get_value (tmp_name_p);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN get_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  //
  // Verify that the caller has the priviliges to perform the requested
  // operation on the requested user record.
  //

  estatus = pkss_inq_is_client_authorized (handle,
					   dbop,
					   tmp_name_p,
					   &auth_yn);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                "status from pkss_inq_is_client_authorized : 0x%x", estatus));

  if (estatus != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  if (!auth_yn)
  {
    dce_svc_printf(PKS_S_ACL_NOT_AUTHORIZED_MSG, tmp_name_p);
    estatus = pks_s_acl_not_authorized;
  }

  else
  {
    sec_pk_usage_flags_t tmp_usages;
    estatus = mgmtP3Data.userKey.usages.get_value (tmp_usages);

      DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                     "status from ASN get_value : 0x%x", estatus));

    if (estatus != 0)
    {
      goto CHECK_ERROR_RETURN;
    }

    tmp_key_vno = 0;

    estatus = mgmtP3Data.userKey.version.get_value (tmp_key_vno);

      DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                     "status from ASN get_value : 0x%x", estatus));

    if (estatus != 0)
    {
      goto CHECK_ERROR_RETURN;
    }

    estatus = mgmtP3Data.userKey.domain.get_value (tmp_domain_p);

      DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                     "status from ASN get_value : 0x%x", estatus));

    if (estatus != 0)
    {
      goto CHECK_ERROR_RETURN;
    }

    //
    // Obtain PKSS database key from values received from the mgmt client.
    //

    dbKeyP = new PkssDbKeyC (tmp_name_p, tmp_domain_p, tmp_usages, tmp_key_vno);

    if (dbKeyP == 0)
    {
      dce_svc_printf(PKS_S_NO_MEMORY_MSG);
      estatus = pks_s_no_memory;
      goto CHECK_ERROR_RETURN;
    }

    //
    // If this is a modify operation, obtain the old database record from
    // which to extract the old key version number and increment it; otherwise,
    // the key version number for new records is 1 (one).
    //

    old_key_vno = 0;

    if (context_p->proto == PKSS_PROTO_MGMT_MOD_USR_CLT)
    {
      estatus = sec_pkss_mgmt_get_old_vno (dbKeyP, &old_key_vno);

      DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                     "status from sec_pkss_mgmt_get_old_vno : 0x%x", estatus));

      if (estatus != 0)
      {
        goto CHECK_ERROR_RETURN;
      }
    }

    //
    // Obtain the appropriate new key version number for the new PKSS database
    // record and construct it from values received from the mgmt client.
    // Recall that integers in PKSS database records and keys are stored in
    // network byte order.
    //

    new_key_vno = 0;
    if (tmp_key_vno = 0)
      new_key_vno++;
    else
      new_key_vno = old_key_vno + 1;

    estatus = sec_pkss_mgmt_get_dbrec (&mgmtP3Data, new_key_vno, &dbrec_p);

    if (estatus != 0)
    {
      goto CHECK_ERROR_RETURN;
    }

    //
    // Insert the new key version number into the record key and insert the
    // new record Userkey:[Kxu]H1:H2:Kou into the database.  If this is a
    // request to add a new record, then for every bit set in the usage flags,
    // create a record with a key whose usages flags field has only that one
    // bit set.  Note that in the case of a mod request we add the new record
    // before deleting the old one to avoid the problem created if a delete
    // succeeded, but an add failed.
    //

    dbKeyP->setKeyVno (new_key_vno);

    if (context_p->proto == PKSS_PROTO_MGMT_MOD_USR_CLT)
    {
      pkss_db_p->txnBegin();

      estatus = pkss_db_p->insert ((char *)dbrec_p->asn_rec_p,
                                   dbrec_p->asn_rec_len,
                                   (char *)dbKeyP->p()->data,
                                   dbKeyP->p()->len);

      DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                    "status from pkss_db_p->insert : 0x%x", estatus));

      if (estatus != 0)
      {
        pkss_db_p->txnCancel();
        goto CHECK_ERROR_RETURN;
      }

      dbKeyP->setKeyVno (old_key_vno);

      estatus = pkss_db_p->remove ((char *)dbKeyP->p()->data,
				   dbKeyP->p()->len);

      DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                    "status from pkss_db_p->remove : 0x%x", estatus));

      if (estatus == 0)
        pkss_db_p->txnEnd();

      else
      {
        pkss_db_p->txnCancel();
        goto CHECK_ERROR_RETURN;
      }

    } // End if (context_p->proto == PKSS_PROTO_MGMT_MOD_USR_CLT)

    else
    {
      estatus = pkss_add_acl ((const char *)dbKeyP->getName());

      DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                     "status pkss_add_acl: 0x%x", estatus));

      if (estatus != 0)
      {
        goto CHECK_ERROR_RETURN;
      }

      estatus = sec_pkss_do_usage_flags (dbKeyP, dbrec_p);

      DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                     "status from do_usage: 0x%x", estatus));

      if (estatus != 0)
      {
        goto CHECK_ERROR_RETURN;
      }

    }

  } // End else path of if (!auth_yn)

CHECK_ERROR_RETURN:
  
  //
  // Zero and release the new database record and its key.
  //

  if (dbKeyP)
  {
    delete dbKeyP;
    dbKeyP = 0;
    sec_pkss_zero_and_free (&dbrec_p);
  }

  //
  // Initialize ASN.1 translation object in anticipation
  // of ASN.1-encoding the PKSS mgmt phase 4 message.
  //

  if (estatus != 0)
    new_key_vno = 0;

  pstatus = mgmtP4Data.version.set_value (new_key_vno);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN set_value : 0x%x", pstatus));

  if (pstatus != 0)
  {
    estatus = pstatus;
    dce_svc_printf (PKS_S_NO_GEN_REPLY_MSG, estatus);
    goto RUNDOWN;
  }

  pstatus = mgmtP4Data.status.set_value (estatus);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN set_value : 0x%x", pstatus));

  if (pstatus != 0)
  {
    dce_svc_printf (PKS_S_NO_GEN_REPLY_MSG, pstatus);
    estatus = pstatus;
    goto RUNDOWN;
  }

  //
  // ASN.1-encode return parameters prior to encryption.
  //

  pstatus = mgmtP4Data.write (to_asn_plain);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN write : 0x%x", pstatus));

  if (pstatus != 0)
  {
    estatus = pstatus;
    dce_svc_printf (PKS_S_NO_GEN_REPLY_MSG, estatus);
    goto RUNDOWN;
  }

  //
  // Move contents of ASN.1 buffer object to sec_pk_gen_data_t struct in
  // anticipation of encrypting it.  Make sure to clear out any old stuff
  // first.
  //

  plaintext_in.data = to_asn_plain.data;
  plaintext_in.len = to_asn_plain.data_length;

  //
  // Encrypt return parameters with Diffie-Hellman key D.  Recall that the
  // result of the encryption will be [V:Status]D if the PKSS mgmt client
  // generated the new asymmetric key pair and [V:Kou~:Status]D if the PKSS
  // generated the new asymmetric key pair.
  //

  pstatus = sec_bsafe_symmetric_encrypt (sym_alg_id_pad,
                                         context_p->crypto_key_p,
                                         sym_pad_iv_p,
                                         &plaintext_in,
                                         &ciphertext_out);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_bsafe_symmetric_encrypt : 0x%x", pstatus));

  if (pstatus != 0)
  {
    dce_svc_printf(PKS_S_SEC_BSAFE_MSG, pstatus);
    estatus = pstatus;
    goto RUNDOWN;
  }

  //
  // Zero and delete buffers containing sensitive data.
  //

  delete dbKeyP;
  dbKeyP = 0;
  sec_pkss_zero_and_free (&dbrec_p);
  sec_pkss_zero_and_free (&sym_pad_iv_p);
  sec_pkss_zero_and_free (&sym_nopad_iv_p);

  //
  // Instantiate ASN.1 translation object in anticipation of ASN.1-encoding
  // the output parameters to be sent to the PKSS mgmt client.
  //

  switch (context_p->proto)
  {
    case PKSS_PROTO_MGMT_ADD_USR_CLT:
      mgmtP4MsgP = new PkssMgmtCltAddP4MsgC;
      break;

    case PKSS_PROTO_MGMT_MOD_USR_CLT:
      mgmtP4MsgP = new PkssMgmtCltModP4MsgC;
      break;

    default:
      dce_svc_printf(PKS_S_INVALID_ARG_MSG);
      goto RUNDOWN;

  } // End switch (context_p->proto)

  if (!(mgmtP4MsgP))
  {
    dce_svc_printf(PKS_S_NO_MEMORY_MSG);
    estatus = pks_s_no_memory;
    goto RUNDOWN;
  }

  pstatus = mgmtP4MsgP->set_value (ciphertext_out.data, ciphertext_out.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN set_value : 0x%x", pstatus));

  if (pstatus != 0)
  {
    estatus = pstatus;
    goto RUNDOWN;
  }

  //
  // ASN.1-encode output parameters prior to transmission to the mgmt client.
  //

  pstatus = mgmtP4MsgP->write (to_asn_cipher);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN write : 0x%x", pstatus));

  if (pstatus != 0)
  {
    estatus = pstatus;
    goto RUNDOWN;
  }

RUNDOWN:

  //
  // Load output parameter.  If estatus at this point is non-zero, then an
  // error occurred, and there is no point to returning a login phase 2 msg.
  //

  msglen = sizeof (sec_pkss_msg_buf_t);
  
  if (estatus == 0)
    msglen += to_asn_cipher.data_length;

  *from_pkss_p = (sec_pkss_msg_buf_p_t)rpc_ss_allocate (msglen);

  if (!(*from_pkss_p))
  {
    dce_svc_printf(PKS_S_NO_MEMORY_MSG);
    estatus = pks_s_no_memory;
  }

  else
  {
    if (estatus != 0)
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

    } // End else path from if (estatus != 0)

  } // End else path from if (!(*from_pkss_p))

  //
  // Zero and release the contents of buffers containing sensitive data.
  //

  msglen = 0;
  tmp_key_vno = 0;
  old_key_vno = 0;
  new_key_vno = 0;
  sym_alg_id_pad = 0;
  sym_alg_id_nopad = 0;
  sym_pad_key_len = 0;
  sym_nopad_key_len = 0;
  sec_pkss_zero_and_free (&dbrec_p);
  sec_pkss_zero_and_free (&sym_pad_iv_p);
  sec_pkss_zero_and_free (&sym_nopad_iv_p);
  sec_pkss_zero_and_free (&plaintext_out);
  sec_pkss_zero_and_free (&ciphertext_out);

  if (mgmtP4MsgP)
  {
    delete mgmtP4MsgP;
    mgmtP4MsgP = 0;
  }
  
  if (dbKeyP)
  {
    delete dbKeyP;
    dbKeyP = 0;
  }

  //
  // Unlock the mutex on the context handle.
  //

  estatus = pthread_mutex_unlock (&(context_p->mutex));

  //
  // Exit this function.
  //

#ifdef MALLOCTRACE
  malloc_mstatus ("Exiting sec_pkss_mgmt_mgr_clt_util");
#endif

  return (estatus);
}

// End sec_pkss_mgmt_mgr_clt_util


//
// Complete construction of new database record in the case that the PKSS
// mgmt client requested that the PKSS generate the new asymmetric key
// pair on the client's behalf.
//
// Note that this module is intended ONLY for use by the PKSS svr manager.
// This function shall NOT be part of an exposed API.
//
// Called by: sec_pkss_mgmt_mgr_svr_util
//

static error_status_t sec_pkss_mgmt_get_dbrec
(
  PkssMgmtSvrP3DataC *mgmtP3DataP,
  unsigned32 key_vno,
  sec_bsafe_alg_type_t sym_alg_id_pad,
  sec_pk_gen_data_t *sym_pad_iv_p,
  sec_pk_algorithm_id_t *pk_alg_id_p,
  unsigned32 pk_key_len,
  sec_pkss_dbrec_t **dbrec_p
)

{
  //
  // Local variables
  //

  char *pwd_p = 0;
  char *name_p = 0;
  error_status_t estatus = 0;
  sec_pk_gen_data_t h1 = {0, 0};
  sec_pk_gen_data_t h2 = {0, 0};
  sec_pk_gen_data_t seed = {0, 0};
  sec_pk_gen_data_t rand = {0, 0};
  sec_pk_data_t new_kou = {0, 0};
  sec_pk_data_t new_kxu = {0, 0};
  sec_pk_gen_data_t new_kxu_h1 = {0, 0};
  unsigned32 hash_len = 0;
  r_buffer_t new_kou_buf;
  PkssDBrecC newDBrec;
  buffer_t new_dbrec_buf (SECURE);

  //
  // Obtain password from PKSS mgmt client input parameters.
  //

  estatus = mgmtP3DataP->pwd.get_value (pwd_p);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN get_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Obtain username from PKSS mgmt client input parameters.
  //

  estatus = mgmtP3DataP->userKey.uname.get_value (name_p);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN get_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Obtain new hashes H1~ and H2~ from pwd_p.   Note that it is the caller's
  // responsibility to free the storage allocated to contain the hashes.
  //

  switch (sym_alg_id_pad)
  {
    case sec_bsafe_enc_des:
      hash_len = 8;
      break;

    case sec_bsafe_enc_des_ede3:
      hash_len = 24;
      break;

    default:
      hash_len = 16;
      break;

  } // End switch (sym_alg_id_pad)


  estatus = sec_pkss_get_pwd_hashes (pwd_p, name_p, hash_len, &h1, &h2);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_pkss_get_pwd_hashes : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  //
  // Obtain random value for asymmetric key pair generator.  Since it is
  // difficult to foresee what residual bits a freshly malloc'ed buffer will
  // contain, such a buffer makes a fairly reliable, cheap source for a seed
  // value.  Observe that the seed's lifetime is as short as possible and that
  // the random value's life is nearly as brief.
  //

  rand.len = 64;
  rand.data = (unsigned char *) malloc (rand.len);

  if (!(rand.data))
  {
    dce_svc_printf(PKS_S_NO_MEMORY_MSG);
    estatus = pks_s_no_memory;
    goto rundown;
  }

  seed.len = rand.len;
  seed.data = (unsigned char *) malloc (seed.len);

  if (!(seed.data))
  {
    dce_svc_printf(PKS_S_NO_MEMORY_MSG);
    estatus = pks_s_no_memory;
    goto rundown;
  }

  estatus = sec_bsafe_random_bytes (&seed, &rand);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_bsafe_random_bytes : 0x%x", estatus));

  sec_pkss_zero_and_free (&seed);

  if (estatus != 0)
  {
    dce_svc_printf(PKS_S_SEC_BSAFE_MSG, estatus);
    goto rundown;
  }

  //
  // Generate a new asymmetric key pair Kxu~/Kou~.
  //

  estatus = sec_bsafe_gen_pub_key (pk_key_len, &rand, &new_kou, &new_kxu);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_bsafe_gen_pub_key : 0x%x", estatus));

  sec_pkss_zero_and_free (&rand);

  if (estatus != 0)
  {
    dce_svc_printf(PKS_S_SEC_BSAFE_MSG, estatus);
    goto rundown;
  }

  //
  // Encrypt the new private key Kxu~ with the new password hash H1~ to
  // obtain [Kxu~]H1~.  Zero and release contents of H1.
  //

  estatus = sec_bsafe_symmetric_encrypt (sym_alg_id_pad,
                                         &h1,
                                         sym_pad_iv_p,
                                         (sec_pk_gen_data_t *)&new_kxu,
                                         &new_kxu_h1);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_bsafe_symmetric_encrypt : 0x%x", estatus));

  if (estatus != 0)
  {
    dce_svc_printf(PKS_S_SEC_BSAFE_MSG, estatus);
    goto rundown;
  }

  sec_pkss_zero_and_free (&h1);

  //
  // Finish constructing new user record Userkey:[Kxu~]H1~:H2~:Kou~.  Recall
  // that the userKey portion of the record was filled in from parameter data
  // sent to the PKSS by the PKSS mgmt client (see above).
  //

  new_kou_buf.data = new_kou.data;
  new_kou_buf.data_length = new_kou.len;

  estatus = newDBrec.Kou.read (new_kou_buf);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from DBrec read : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  estatus = newDBrec.KxuH1.set_value (new_kxu_h1.data, new_kxu_h1.len);
  
    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from DBrec set_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  estatus = newDBrec.H2.set_value (h2.data, h2.len);
  
    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from DBrec set_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  estatus = newDBrec.version.set_value (key_vno);
  
    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from DBrec set_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  estatus = newDBrec.write (new_dbrec_buf);
  
    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from DBrec write : 0x%x", estatus));

  if (estatus != 0)
  {
    goto rundown;
  }

  *dbrec_p = (sec_pkss_dbrec_t *) calloc (1, sizeof (sec_pkss_dbrec_t));

  if (!(*dbrec_p))
  {
    dce_svc_printf(PKS_S_NO_MEMORY_MSG);
    estatus = pks_s_no_memory;
    goto rundown;
  }

  (*dbrec_p)->asn_rec_len = new_dbrec_buf.data_length;

  (*dbrec_p)->asn_rec_p = (unsigned char *) calloc (1, (*dbrec_p)->asn_rec_len);

  if (!((*dbrec_p)->asn_rec_p))
  {
    dce_svc_printf(PKS_S_NO_MEMORY_MSG);
    (*dbrec_p)->asn_rec_len = 0;
    free (*dbrec_p);
    *dbrec_p = 0;
    estatus = pks_s_no_memory;
    goto rundown;
  }

  (void) memcpy ((*dbrec_p)->asn_rec_p,
		 new_dbrec_buf.data,
                 (*dbrec_p)->asn_rec_len);

  //
  // Zero and release buffers containing sensitive data.
  //

  sec_pkss_zero_and_free (&new_kou);
  sec_pkss_zero_and_free (&new_kxu);
  sec_pkss_zero_and_free (&h2);
  sec_pkss_zero_and_free (&new_kxu_h1);

  //
  // Exit this function.
  //

  return (estatus);

rundown:

  sec_pkss_zero_and_free (&h1);
  sec_pkss_zero_and_free (&h2);
  sec_pkss_zero_and_free (&rand);
  sec_pkss_zero_and_free (&seed);
  sec_pkss_zero_and_free (&new_kou);
  sec_pkss_zero_and_free (&new_kxu);
  sec_pkss_zero_and_free (&new_kxu_h1);

  //
  // Exit this function.
  //

  return (estatus);

} // End sec_pkss_mgmt_get_dbrec


//
// This is a generic PKSS mgmt manager function that does the work for an
// add or change operation when the client requests of the PKSS that the
// PKSS generate the asymmetric key pair.
//
// Note that this module is intended ONLY for use by the PKSS svr manager.
// This function shall NOT be part of an exposed API.
//
// Called by:  sec_pkss_mgmt_add_user_svr
//             sec_pkss_mgmt_mod_user_svr
//

static error_status_t sec_pkss_mgmt_mgr_svr_util
(
  handle_t handle,
  sec_pkss_context_t *context_p,
  sec_pkss_msg_buf_t *to_pkss_p,
  sec_pkss_msg_buf_p_t *from_pkss_p
)

{
  //
  // Local variables
  //

  error_status_t estatus = 0;
  error_status_t pstatus = 0;
  r_buffer_t from_asn_cipher;
  PkssMgmtSvrP3MsgBaseC *mgmtP3MsgP = 0;
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
  r_buffer_t from_asn_plain;
  PkssMgmtSvrP3DataC mgmtP3Data;
  char *tmp_name_p = 0;
  sec_pk_usage_flags_t tmp_usage = 0;
  unsigned32 tmp_key_vno = 0;
  char *tmp_domain_p = 0;
  PkssDbKeyC *dbKeyP = 0;
  unsigned32 old_key_vno = 0;
  sec_pk_algorithm_id_t pk_alg_id = {0, 0};
  unsigned32 pk_key_len = 0;
  unsigned32 new_key_vno = 0;
  sec_pkss_dbrec_t *dbrec_p = 0;
  PkssMgmtSvrP4MsgBaseC *mgmtP4MsgP = 0;
  PkssMgmtSvrP4DataC mgmtP4Data;
  r_buffer_t dbrec_rbuf;
  PkssDBrecC dbRec;
  buffer_t kou_buf;
  buffer_t to_asn_plain (SECURE);
  buffer_t to_asn_cipher;
  unsigned32 msglen = 0;
  db_operation_t dbop;
  boolean32 auth_yn = 0;

#ifdef MALLOCTRACE
  malloc_mstatus ("Entering sec_pkss_mgmt_mgr_svr_util");
#endif

  //
  // Acquire context block mutex.
  //

  if (context_p->deleted)
    return (EINVAL);
  else
  {
    estatus = pthread_mutex_lock (&(context_p->mutex));

    if (estatus != 0)
      return (estatus);
  }

  //
  // Initialize ASN.1 buffer object into which to move data
  // from RPC input buffer upon receipt from the PKSS management client.
  //

  from_asn_cipher.data = to_pkss_p->msg_p;
  from_asn_cipher.data_length = to_pkss_p->msg_len;

  //
  // Instantiate ASN.1 translation object in anticipation of ASN.1-decoding
  // the input parameters received from the PKSS mgmt client.
  //

  switch (context_p->proto)
  {
    case PKSS_PROTO_MGMT_ADD_USR_SVR:
      mgmtP3MsgP = new PkssMgmtSvrAddP3MsgC;
      dbop = DBOP_CREATE_PRINCIPAL;
      break;

    case PKSS_PROTO_MGMT_MOD_USR_SVR:
      mgmtP3MsgP = new PkssMgmtSvrModP3MsgC;
      dbop = DBOP_ALTER_PRINCIPAL;
      break;

    default:
      dce_svc_printf(PKS_S_INVALID_ARG_MSG);
      goto CHECK_ERROR_RETURN;

  } // End switch (context_p->proto)

  if (!(mgmtP3MsgP))
  {
    dce_svc_printf(PKS_S_NO_MEMORY_MSG);
    estatus = pks_s_no_memory;
    goto CHECK_ERROR_RETURN;
  }

  //
  // ASN.1-decode input parameter data [Userkey:newpwd:A:L]D received from the
  // client.
  //

  estatus = mgmtP3MsgP->read (from_asn_cipher);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN read : 0x%x", estatus));

  if (estatus != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  //
  // Obtain required symmetric key crypto info given a symmetric key
  // encryption algorithm identifier.  Note that it is the caller's
  // responsibility to free initial vector storage allocated in this
  // function.
  //

  estatus = sec_pkss_get_sym_crypto_info (context_p->sym_alg,
                                          &sym_alg_id_pad,
                                          &sym_alg_id_nopad,
                                          &sym_pad_key_len,
                                          &sym_nopad_key_len,
                                          &sym_pad_iv_p,
                                          &sym_nopad_iv_p);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_pkss_get_sym_crypto_info : 0x%x", estatus));

  if (estatus != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  //
  // Obtain the ASN.1-decoded encrypted input parameter data and decrypt
  // it with Diffie-Hellman key D.
  //

  mgmtP3MsgP->get_value (ciphertext_in.data, ciphertext_in.len);

  estatus = sec_bsafe_symmetric_decrypt (sym_alg_id_pad,
                                         context_p->crypto_key_p,
                                         sym_pad_iv_p,
                                         &ciphertext_in,
                                         &plaintext_out);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_bsafe_symmetric_decrypt : 0x%x", estatus));

  if (estatus != 0)
  {
    dce_svc_printf(PKS_S_SEC_BSAFE_MSG, estatus);
    goto CHECK_ERROR_RETURN;
  }

  //
  // Delete the object pointed to by mgmtP3MsgP.
  //

  delete mgmtP3MsgP;
  mgmtP3MsgP = 0;

  //
  // Initialize ASN.1 buffer object into which to move data
  // in anticipation of ASN.1 translating the decrypted input parameters.
  // Recall that in the case of the PKSS mgmt client generating the new
  // asymmetric key pair, the input parameters were Userkey:[Kxu]H1:H2:Kou.
  // In the case that the PKSS mgmt client requested that the PKSS generate
  // the new asymmetric key pair, the input parameters were Userkey:newpwd:A:L.
  //

  from_asn_plain.data = plaintext_out.data;
  from_asn_plain.data_length = plaintext_out.len;

  //
  // ASN.1-decode input parameters.
  //

  estatus = mgmtP3Data.read (from_asn_plain);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN read : 0x%x", estatus));

  if (estatus != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  //
  // Obtain from the input parameters the userKey portion of the record
  // to be added/modified
  //

  estatus = mgmtP3Data.userKey.uname.get_value (tmp_name_p);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN get_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  //
  // Verify that the caller has the priviliges to perform the requested
  // operation on the requested user record.
  //

  estatus = pkss_inq_is_client_authorized (handle, dbop, tmp_name_p, &auth_yn);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                "status from pkss_inq_is_client_authorized : 0x%x", estatus));

  if (estatus != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  if (!auth_yn)
  {
    dce_svc_printf(PKS_S_ACL_NOT_AUTHORIZED_MSG, tmp_name_p);
    estatus = pks_s_acl_not_authorized;
    goto CHECK_ERROR_RETURN;
  }

  estatus = mgmtP3Data.userKey.usages.get_value (tmp_usage);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN get_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  tmp_key_vno = 0;
  estatus = mgmtP3Data.userKey.version.get_value (tmp_key_vno);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN get_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  estatus = mgmtP3Data.userKey.domain.get_value (tmp_domain_p);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN get_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  //
  // Construct new PKSS database key.
  //

  dbKeyP = new PkssDbKeyC (tmp_name_p, tmp_domain_p, tmp_usage, tmp_key_vno);

  if (!(dbKeyP))
  {
    dce_svc_printf(PKS_S_NO_MEMORY_MSG);
    estatus = pks_s_no_memory;
    goto CHECK_ERROR_RETURN;
  }
  
  //
  // If this is a modify operation, obtain the old database record from
  // which to extract the old key version number and increment it; otherwise,
  // the key version number for new records is 1 (one).
  //

  old_key_vno = 0;
  if (context_p->proto == PKSS_PROTO_MGMT_MOD_USR_SVR)
  {
    estatus = sec_pkss_mgmt_get_old_vno (dbKeyP, &old_key_vno);

    if (estatus != 0)
    {
      goto CHECK_ERROR_RETURN;
    }
  }

  //
  // Obtain public key algorithm ID and key length.
  //

  estatus = mgmtP3Data.pkAlgID.get_value (pk_alg_id.data, pk_alg_id.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN get_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  pk_key_len = 0;
  estatus = mgmtP3Data.pkKeyLen.get_value (pk_key_len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN get_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto CHECK_ERROR_RETURN;
  }


  //
  // Obtain the appropriate new key version number for the new PKSS database
  // record and construct the record from values received from the mgmt client.
  //

  new_key_vno = 0;

  if (tmp_key_vno = 0)
    new_key_vno++;
  else
    new_key_vno = old_key_vno + 1;

  estatus = sec_pkss_mgmt_get_dbrec (&mgmtP3Data,
				     new_key_vno,
				     sym_alg_id_pad,
				     sym_pad_iv_p,
				     &pk_alg_id,
				     pk_key_len,
				     &dbrec_p);
  if (estatus != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  //
  // Insert the new key version number into the record key and insert the
  // new record [Kxu]H1:H2:Kou:V into the database.  Recall that
  // integers in PKSS database records and keys are stored in network byte
  // order.  If this is a request to add a new record, then for every bit
  // set in the usage flags, create a record with a key whose usages flags
  // field has only that one bit set.  Note that the insert preceeds the
  // delete to avoid the problem created if a delete failed, but an insert
  // succeeded.
  //

  dbKeyP->setKeyVno (new_key_vno);

  if (context_p->proto == PKSS_PROTO_MGMT_MOD_USR_SVR)
  {
    pkss_db_p->txnBegin();
    
    estatus = pkss_db_p->insert ((char *)dbrec_p->asn_rec_p,
                                 dbrec_p->asn_rec_len,
                                 (char *)dbKeyP->p()->data,
                                 dbKeyP->p()->len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                  "status from pkss_db_p->insert: 0x%x", estatus));

    if (estatus != 0)
    {
      pkss_db_p->txnCancel();
      goto CHECK_ERROR_RETURN;
    }

    dbKeyP->setKeyVno (old_key_vno);

    estatus = pkss_db_p->remove ((char *)dbKeyP->p()->data,
				   dbKeyP->p()->len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                  "status from pkss_db_p->remove: 0x%x", estatus));

    if (estatus == 0)
      pkss_db_p->txnEnd();

    else
    {
      pkss_db_p->txnCancel();
      goto CHECK_ERROR_RETURN;
    }

  } // End if (context_p->proto == PKSS_PROTO_MGMT_MOD_USR_SVR)

  else
  {
    estatus = pkss_add_acl ((const char *)dbKeyP->getName());

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status pkss_add_acl: 0x%x", estatus));

    if (estatus != 0)
    {
      goto CHECK_ERROR_RETURN;
    }

    estatus = sec_pkss_do_usage_flags (dbKeyP, dbrec_p);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from do_usage: 0x%x", estatus));

    if (estatus != 0)
    {
      goto CHECK_ERROR_RETURN;
    }
  }


  estatus = mgmtP4Data.version.set_value (new_key_vno);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN set_value : 0x%x", estatus));

  if (estatus != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  //
  // Add the new public key component of the asymmetric key pair if this is
  // an operation in which the server generated the new asymmetric key pair.
  //

  dbrec_rbuf.data = dbrec_p->asn_rec_p;
  dbrec_rbuf.data_length = dbrec_p->asn_rec_len;
  estatus = dbRec.read (dbrec_rbuf);

  DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                 "status from db read : 0x%x", estatus));

  if (estatus != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  estatus = dbRec.Kou.write (kou_buf);

  DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                 "status from DBrec write : 0x%x", estatus));

  if (estatus != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  estatus = mgmtP4Data.kou.read (kou_buf);

  DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                 "status from ASN read : 0x%x", estatus));

  if (estatus != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

CHECK_ERROR_RETURN:

  //
  // Initialize ASN.1 translation object in anticipation
  // of ASN.1-encoding the PKSS mgmt phase 4 message V:Kou~:Status.
  //

  pstatus = mgmtP4Data.status.set_value (estatus);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN set_value : 0x%x", pstatus));

  if (pstatus != 0)
  {
    estatus = pstatus; 
    goto RUNDOWN;
  }

  //
  // ASN.1-encode return parameters prior to encryption.
  //

  pstatus = mgmtP4Data.write (to_asn_plain);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN write : 0x%x", pstatus));

  if (pstatus != 0)
  {
    estatus = pstatus;
    goto RUNDOWN;
  }

  //
  // Move contents of ASN.1 buffer object to sec_pk_gen_data_t struct in
  // anticipation of encrypting it.
  //

  plaintext_in.data = to_asn_plain.data;
  plaintext_in.len = to_asn_plain.data_length;

  //
  // Encrypt return parameters V:Kou~:Status with Diffie-Hellman key D
  // to obtain [V:Kou~:Status]D.
  //

  pstatus = sec_bsafe_symmetric_encrypt (sym_alg_id_pad,
                                         context_p->crypto_key_p,
                                         sym_pad_iv_p,
                                         &plaintext_in,
                                         &ciphertext_out);
  if (pstatus != 0)
  {
    estatus = pstatus;
    dce_svc_printf(PKS_S_SEC_BSAFE_MSG, pstatus);
    goto RUNDOWN;
  }

  //
  // Zero buffers containing sensitive data.
  //

  delete dbKeyP;
  dbKeyP = 0;

  sec_pkss_zero_and_free (&dbrec_p);
  sec_pkss_zero_and_free (&sym_pad_iv_p);
  sec_pkss_zero_and_free (&sym_nopad_iv_p);

  //
  // Instantiate ASN.1 translation object in anticipation of ASN.1-encoding
  // the output parameters to be sent to the PKSS mgmt client.
  //

  switch (context_p->proto)
  {
    case PKSS_PROTO_MGMT_ADD_USR_SVR:
      mgmtP4MsgP = new PkssMgmtSvrAddP4MsgC;
      break;

    case PKSS_PROTO_MGMT_MOD_USR_SVR:
      mgmtP4MsgP = new PkssMgmtSvrModP4MsgC;
      break;

    default:
      dce_svc_printf(PKS_S_INVALID_ARG_MSG);
      goto RUNDOWN;

  } // End switch (context_p->proto)

  if (!(mgmtP4MsgP))
  {
    dce_svc_printf(PKS_S_NO_MEMORY_MSG);
    estatus = pks_s_no_memory;
    goto RUNDOWN;
  }

  pstatus = mgmtP4MsgP->set_value (ciphertext_out.data, ciphertext_out.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN set_value : 0x%x", pstatus));

  if (pstatus != 0)
  {
    estatus = pstatus;
    goto RUNDOWN;
  }

  //
  // ASN.1-encode output parameters prior to transmission to the mgmt client.
  //

  pstatus = mgmtP4MsgP->write (to_asn_cipher);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN write : 0x%x", pstatus));

  if (pstatus != 0)
  {
    estatus = pstatus;
    goto RUNDOWN;
  }

RUNDOWN:

  //
  // Load output parameter.  If estatus at this point is non-zero, then an
  // error occurred, and there is no point to returning a login phase 2 msg.
  //

  msglen = sizeof (sec_pkss_msg_buf_t);
  
  if (estatus == 0)
    msglen += to_asn_cipher.data_length;

  *from_pkss_p = (sec_pkss_msg_buf_p_t)rpc_ss_allocate (msglen);

  if (!(*from_pkss_p))
  {
    dce_svc_printf(PKS_S_NO_MEMORY_MSG);
    estatus = pks_s_no_memory;
  }

  else
  {
    if (estatus != 0)
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

    } // End else path from if (estatus != 0)

  } // End else path from if (!(*from_pkss_p))


  if (mgmtP3MsgP)
  {
    delete mgmtP3MsgP;
    mgmtP3MsgP = 0;
  }
   
  msglen = 0;
  pk_key_len = 0;
  new_key_vno = 0;
  tmp_usage =  0;
  tmp_key_vno = 0;
  old_key_vno = 0;
  sym_alg_id_pad = 0;
  sym_alg_id_nopad = 0;
  sym_pad_key_len = 0;
  sym_nopad_key_len = 0;
  sec_pkss_zero_and_free (&dbrec_p);
  sec_pkss_zero_and_free (&sym_pad_iv_p);
  sec_pkss_zero_and_free (&sym_nopad_iv_p);
  sec_pkss_zero_and_free (&plaintext_out);
  sec_pkss_zero_and_free (&ciphertext_out);

  if (mgmtP4MsgP)
  {
    delete mgmtP4MsgP;
    mgmtP4MsgP = 0;
  }

  if (dbKeyP)
  {
    delete dbKeyP;
    dbKeyP = 0;
  }

  //
  // Unlock the mutex on the context handle.
  //

  estatus = pthread_mutex_unlock (&(context_p->mutex));

  //
  // Exit this function.
  //

#ifdef MALLOCTRACE
  malloc_mstatus ("Exiting sec_pkss_mgmt_mgr_svr_util");
#endif

  return (estatus);

} // End sec_pkss_mgmt_mgr_svr_util


//
// Perform Diffie-Hellman key exchange with management client.
//
// Called by:  RPC runtime
//

error_status_t sec_pkss_mgmt_diffhell
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

  r_buffer_t from_asn;
  unsigned32 proto_id = 0;
  PkssMgmtP1MsgC mgmtP1Msg;
  sec_pk_gen_data_t clt_dh_val = {0, 0};
  unsigned32 sym_alg_to_use = 0;
  void *dh_context_p = 0;
  sec_pk_gen_data_t pkss_dh_val = {0, 0};
  sec_bsafe_alg_type_t sym_alg_id_pad = 0;
  sec_bsafe_alg_type_t sym_alg_id_nopad = 0;
  unsigned32 sym_pad_key_len = 0;
  unsigned32 sym_nopad_key_len = 0;
  sec_pk_gen_data_t *dh_key_p = 0;
  sec_pk_gen_data_t h_clt_algs = {0, 0};
  sec_pk_gen_data_t clt_algs = {0, 0};
  buffer_t clt_algs_buf (SECURE);
  unsigned32 clt_alg_idx = 0;
  PkssMgmtP2MsgC mgmtP2Msg;
  buffer_t to_asn;
  sec_pkss_context_t *context_p = 0;

#ifdef MALLOCTRACE
  malloc_mstatus ("Entering sec_pkss_mgmt_diffhell");
#endif

  *ctx_handle_p = 0;
 
  //
  // Initialize ASN.1 buffer object into which to move data
  // from RPC input buffer upon receipt from the PKSS management client.
  //

  from_asn.data = to_pkss_p->msg_p;
  from_asn.data_length = to_pkss_p->msg_len;

  //
  // ASN.1-decode I:E(Dc):algList into its component elements.
  //

  *estatus_p = mgmtP1Msg.read (from_asn);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN read : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto rundown;
  }

  //
  // Obtain input parameters translated from ASN.1.
  //

  *estatus_p = mgmtP1Msg.protoID.get_value (proto_id);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN get_value : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto rundown;
  }

  *estatus_p = mgmtP1Msg.eDc.get_value (clt_dh_val.data, clt_dh_val.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN get_value : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto rundown;
  }

  //
  // Obtain algorithm list and algorithm to use from algList.  If there are no
  // matches between the list the client sent and the server's list, return an
  // error.
  //

  sym_alg_to_use = 0;
  *estatus_p = sec_pkss_get_sym_alg_idx (&mgmtP1Msg.algList,
				       &sym_alg_to_use,
				       &clt_alg_idx);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
            "status from sec_pkss_get_sym_alg_idx : 0x%x, sym_alg_to_use: %d ",
	    *estatus_p, sym_alg_to_use ));

  if (*estatus_p != 0)
  {
    goto rundown;
  }

  if (sym_alg_to_use == 0)
  {
    goto rundown;
  }

  //
  // Obtain exponentiated D-H PKSS value E(Dp).
  //

  *estatus_p = sec_pkss_get_dh_val (&dh_context_p, &pkss_dh_val);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_pkss_get_dh_val : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto rundown;
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
                                          0,
                                          0);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                 "status from sec_pkss_get_sym_crypto_info : 0x%x",
		 *estatus_p));

  if (*estatus_p != 0)
  {
    dce_svc_printf(PKS_S_SEC_BSAFE_MSG, *estatus_p);
    goto rundown;
  }

  //
  // Compute a Diffie-Hellman key D, using E(Dc) and E(Dp).
  //

  dh_key_p = (sec_pk_gen_data_t *) calloc (1, sizeof (sec_pk_gen_data_t *));

  if (!(dh_key_p))
  {
    dce_svc_printf(PKS_S_NO_MEMORY_MSG);
    *estatus_p = pks_s_no_memory;
    goto rundown;
  }

  dh_key_p->len = sym_pad_key_len;
  dh_key_p->data = (unsigned char *) calloc (1, dh_key_p->len);

  if (!(dh_key_p->data))
  {
    dce_svc_printf(PKS_S_NO_MEMORY_MSG);
    *estatus_p = pks_s_no_memory;
    goto rundown;
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
    goto rundown;
  }

  //
  // Compute a hash H(algList) on the algorithm list algList sent by the client.
  //

  *estatus_p = mgmtP1Msg.algList.write (clt_algs_buf);

  if (*estatus_p != 0)
  {
    goto rundown;
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
    goto rundown;
  }

  //
  // Initialize the ASN.1 translation object in anticipation
  // of translating PKSS management protocol phase 2 message
  // E(Dp):H(algList):algIdx to ASN.1.
  //

  *estatus_p = mgmtP2Msg.eDp.set_value (pkss_dh_val.data, pkss_dh_val.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN set_value : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto rundown;
  }

  *estatus_p = mgmtP2Msg.hAlgs.set_value (h_clt_algs.data, h_clt_algs.len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN set_value : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto rundown;
  }

  *estatus_p = mgmtP2Msg.algIdx.set_value (clt_alg_idx);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN set_value : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto rundown;
  }

  //
  // Initialize the ASN.1 buffer object into which to write
  // the translated PKSS management protocol phase 2 message
  // E(Dp):H(algList):algId in anticipation of transmission to the PKSS.
  //

  *estatus_p = mgmtP2Msg.write(to_asn);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN write : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto rundown;
  }

  //
  // Create and load output parameter.
  //

  *from_pkss_p = (sec_pkss_msg_buf_p_t)
		 rpc_ss_allocate (sizeof (sec_pkss_msg_buf_t));

  if (!(*from_pkss_p))
  {
    dce_svc_printf(PKS_S_NO_MEMORY_MSG);
    *estatus_p = pks_s_no_memory;
    goto rundown;
  }

  (*from_pkss_p)->msg_p = (byte *) rpc_ss_allocate (to_asn.data_length);

  if (!(*from_pkss_p))
  {
    dce_svc_printf(PKS_S_NO_MEMORY_MSG);
    *estatus_p = pks_s_no_memory;
    goto rundown;
  }

  (*from_pkss_p)->msg_len = to_asn.data_length;
  (void) memcpy ((*from_pkss_p)->msg_p, to_asn.data, (*from_pkss_p)->msg_len);

  //
  // Zero and release contents of pkss_dh_val.  Zero mgmtP2Msg.
  //

  sec_pkss_zero_and_free (&pkss_dh_val);

  //
  // Generate user context block and set ctx_handle_p.  Save protocol ID I,
  // Diffie-Hellman key D, symmetric encryption algorithm ID algId in context
  // block.
  //

  context_p = (sec_pkss_context_t *) calloc (1, sizeof (sec_pkss_context_t));

  if (!(context_p))
  {
    dce_svc_printf(PKS_S_NO_MEMORY_MSG);
    *estatus_p = pks_s_no_memory;
    goto rundown;
  }

  context_p->proto = proto_id;
  context_p->sym_alg = sym_alg_to_use;
  context_p->deleted = 0;
  context_p->state = 0;
  context_p->crypto_key_p = dh_key_p;
  *estatus_p = pthread_mutex_init (&(context_p->mutex),
				   pthread_mutexattr_default);
  *ctx_handle_p = (sec_pkss_ctx_hdl_t *)context_p;

  //
  // Exit this function.
  //

  sec_pkss_zero_and_free (&h_clt_algs);

#ifdef MALLOCTRACE
  malloc_mstatus ("Exiting sec_pkss_mgmt_diffhell - normal");
#endif

  return (rpc_s_ok);

rundown: 

  sym_alg_id_pad = 0;
  sym_alg_id_nopad = 0;
  sym_pad_key_len = 0;
  sym_nopad_key_len = 0;

  sec_pkss_zero_and_free (&h_clt_algs);
  sec_pkss_zero_and_free (&pkss_dh_val);

  //
  // Exit this function.
  //

#ifdef MALLOCTRACE
  malloc_mstatus ("Exiting sec_pkss_mgmt_diffhell - error rundown");
#endif

  return (rpc_s_ok);

} // End sec_pkss_mgmt_diffhell


//
// Add a new user - mgmt client generates new asymmetric key pair.
//
// Called by:  RPC runtime
//
// calls: sec_pkss_mgmt_mgr_clt_util
//

error_status_t sec_pkss_mgmt_add_user_clt
(
  handle_t IDL_handle,
  sec_pkss_ctx_hdl_t *ctx_handle_p,
  sec_pkss_msg_buf_t *to_pkss_p,
  sec_pkss_msg_buf_p_t *from_pkss_p,
  error_status_t *estatus_p
)

{

  sec_pkss_context_t *context_p = (sec_pkss_context_t *)*ctx_handle_p;

  *estatus_p = sec_pkss_mgmt_mgr_clt_util (IDL_handle,
					   context_p,
					   to_pkss_p,
					   from_pkss_p);

  //
  // Zero and release the context handle.
  //

  // sec_pkss_ctx_hdl_t_rundown (*ctx_handle_p);
  // *ctx_handle_p = 0;

  //
  // Exit this function.
  //

  return (rpc_s_ok);

} // End sec_pkss_mgmt_add_user_clt


//
// Add a new user - PKSS mgmt manager generates new asymmetric key pair.
//
// Called by:  RPC runtime
//
// calls: sec_pkss_mgmt_mgr_svr_util
//

error_status_t sec_pkss_mgmt_add_user_svr
(
  handle_t IDL_handle,
  sec_pkss_ctx_hdl_t *ctx_handle_p,
  sec_pkss_msg_buf_t *to_pkss_p,
  sec_pkss_msg_buf_p_t *from_pkss_p,
  error_status_t *estatus_p
)

{

  sec_pkss_context_t *context_p = (sec_pkss_context_t *)*ctx_handle_p;

  *estatus_p = sec_pkss_mgmt_mgr_svr_util (IDL_handle,
					   context_p,
					   to_pkss_p,
					   from_pkss_p);

  //
  // Zero and release the context handle.
  //

  // sec_pkss_ctx_hdl_t_rundown (*ctx_handle_p);
  // *ctx_handle_p = 0;

  //
  // Exit this function.
  //

  return (rpc_s_ok);

} // End sec_pkss_mgmt_add_user_svr


//
// Delete a user
//
// Called by:  RPC runtime
//

error_status_t sec_pkss_mgmt_del_user
(
    handle_t IDL_handle,
    sec_pkss_msg_buf_t *to_pkss_p,
    sec_pkss_msg_buf_p_t *from_pkss_p,
    error_status_t *estatus_p
)

{
  //
  // Local variables
  //

  error_status_t pstatus = 0;
  r_buffer_t from_asn;
  PkssMgmtDelP3MsgC mgmtDelP3Msg;
  char *tmp_name_p = 0;
  sec_pk_usage_flags_t tmp_usage = 0;
  unsigned32 tmp_version = 0;
  char *tmp_domain_p = 0;
  PkssDbKeyC *dbKeyP = 0;
  PkssMgmtDelP4MsgC mgmtDelP4Msg;
  buffer_t to_asn;
  unsigned32 msglen = 0;
  boolean32 auth_yn = 0;
  sec_pkss_dbrec_t dbrec = {0, 0};

#ifdef MALLOCTRACE
  malloc_mstatus ("Entering sec_pkss_mgmt_del_user");
#endif


  //
  // Initialize ASN.1 buffer object into which to move data
  // from RPC input buffer upon receipt from the PKSS management client.
  //

  from_asn.data = to_pkss_p->msg_p;
  from_asn.data_length = to_pkss_p->msg_len;

  //
  // ASN.1-decode Username received from client into its component elements.
  //

  *estatus_p = mgmtDelP3Msg.read (from_asn);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN read : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  *estatus_p = mgmtDelP3Msg.userKey.uname.get_value (tmp_name_p);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN get_value : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  //
  // Verify that the caller has the priviliges to perform the requested
  // operation on the requested user record.
  //

  *estatus_p = pkss_inq_is_client_authorized (IDL_handle,
					      DBOP_DELETE_PRINCIPAL,
					      tmp_name_p,
					      &auth_yn);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from pkss_inq_is_client_authorized : 0x%x",
		   *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  if (!auth_yn)
  {
    dce_svc_printf(PKS_S_ACL_NOT_AUTHORIZED_MSG, tmp_name_p);
    *estatus_p = pks_s_acl_not_authorized;
    goto CHECK_ERROR_RETURN;
  }

  *estatus_p = mgmtDelP3Msg.userKey.usages.get_value (tmp_usage);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN get_value : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  *estatus_p = mgmtDelP3Msg.userKey.version.get_value (tmp_version);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN get_value : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  *estatus_p = mgmtDelP3Msg.userKey.domain.get_value (tmp_domain_p);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN get_value : 0x%x", *estatus_p));

  if (*estatus_p != 0)
  {
    goto CHECK_ERROR_RETURN;
  }

  dbKeyP = new PkssDbKeyC (tmp_name_p, tmp_domain_p, tmp_usage, tmp_version);

  if (!(dbKeyP))
  {
    dce_svc_printf(PKS_S_NO_MEMORY_MSG);
    *estatus_p = pks_s_no_memory;
    goto CHECK_ERROR_RETURN;
  }

  //
  // If key version number is 0, delete the record with the highest version
  // number; otherwise, delete the specified record.  Recall that
  // sec_pkss_db_lookup will modify the key to contain the highest version
  // number if the version number in the key was zero input.
  //

  if (tmp_version == 0)
  {
    *estatus_p = sec_pkss_db_lookup (dbKeyP, &dbrec);

    if (*estatus_p == 0)
      sec_pkss_zero_and_free (&dbrec);
    else
      goto CHECK_ERROR_RETURN;
  }

  pkss_db_p->txnBegin();

  *estatus_p = pkss_db_p->remove ((char *)dbKeyP->p()->data, dbKeyP->p()->len);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from pkss_db_p->remove : 0x%x", *estatus_p));

  if (*estatus_p == 0)
    pkss_db_p->txnEnd();
  else
    pkss_db_p->txnCancel();

  //
  // Zero and release contents dbKeyP.
  //

  delete dbKeyP;
  dbKeyP = 0;

  //
  // ASN.1-encode Status prior to transmission to the mgmt client.
  //

CHECK_ERROR_RETURN:

  pstatus = mgmtDelP4Msg.status.set_value (*estatus_p);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN set_value : 0x%x", pstatus));

  if (pstatus != 0)
  {
    *estatus_p = pstatus;
    goto RUNDOWN;
  }

  pstatus = mgmtDelP4Msg.write (to_asn);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN write : 0x%x", pstatus));

  if (pstatus != 0)
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
      (void) memcpy ((*from_pkss_p)->msg_p,
		     to_asn.data,
		     to_asn.data_length);

    } // End else path from if (*estatus_p != 0)

  } // End else path from if (!(*from_pkss_p))

  if (dbKeyP)
  {
    delete dbKeyP;
    dbKeyP = 0;
  }

  msglen = 0;

  //
  // Exit this function.
  //

#ifdef MALLOCTRACE
  malloc_mstatus ("Exiting sec_pkss_mgmt_del_user");
#endif

  return (rpc_s_ok);

} // sec_pkss_mgmt_del_user


//
// Modify a user - mgmt client generates new asymmetric key pair.
//
// Called by:  RPC runtime
//
// calls: sec_pkss_mgmt_mgr_clt_util
//

error_status_t sec_pkss_mgmt_mod_user_clt
(
  handle_t IDL_handle,
  sec_pkss_ctx_hdl_t *ctx_handle_p,
  sec_pkss_msg_buf_t *to_pkss_p,
  sec_pkss_msg_buf_p_t *from_pkss_p,
  error_status_t *estatus_p
)

{

  sec_pkss_context_t *context_p = (sec_pkss_context_t *)*ctx_handle_p;

  *estatus_p = sec_pkss_mgmt_mgr_clt_util (IDL_handle,
					   context_p,
					   to_pkss_p,
					   from_pkss_p);

  //
  // Zero and release the context handle.
  //

  // sec_pkss_ctx_hdl_t_rundown (*ctx_handle_p);
  // *ctx_handle_p = 0;

  //
  // Exit this function.
  //

  return (rpc_s_ok);

} // End sec_pkss_mgmt_mod_user_clt


//
// Modify a user - PKSS mgmt manager generates new asymmetric key pair.
//
// Called by:  RPC runtime
//
// calls: sec_pkss_mgmt_mgr_svr_util
//

error_status_t sec_pkss_mgmt_mod_user_svr
(
  handle_t IDL_handle,
  sec_pkss_ctx_hdl_t *ctx_handle_p,
  sec_pkss_msg_buf_t *to_pkss_p,
  sec_pkss_msg_buf_p_t *from_pkss_p,
  error_status_t *estatus_p
)

{

  sec_pkss_context_t *context_p = (sec_pkss_context_t *)*ctx_handle_p;

  *estatus_p = sec_pkss_mgmt_mgr_svr_util (IDL_handle,
					   context_p,
					   to_pkss_p,
					   from_pkss_p);

  //
  // Zero and release the context handle.
  //

  // sec_pkss_ctx_hdl_t_rundown (*ctx_handle_p);
  // *ctx_handle_p = 0;

  //
  // Exit this function.
  //

  return (rpc_s_ok);

} // End sec_pkss_mgmt_mod_user_svr

