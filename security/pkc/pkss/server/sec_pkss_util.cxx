//
// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// All Rights Reserved
//
// HISTORY
// $Log: sec_pkss_util.cxx,v $
// Revision 1.1.2.1  1996/10/03  20:32:37  arvind
// 	OSF DCE 1.2.2 Drop 4
// 	[1996/10/03  19:32:32  arvind]
//
// Revision /main/DCE_1.2.2/1  1996/09/09  21:31 UTC  arvind
// 	PKSS drop from DEC (DCE1.2.2)
// 	[1996/08/30  15:56 UTC  arvind  /main/arvind_pkss/1]
// 
// $EndLog$
//
// (c) Copyright 1996,
// Digital Equipment Corporation, Maynard, Massachusetts, USA
// All Rights Reserved
//
// PKSS utility functions of interest to both client and server
//

extern "C"
{
#include <stdio.h>
#include <dce/nbase.h>
#include <dce/dce.h>
#include <string.h>
#include <pthread.h>
#include <dce/dce_msg.h>
#include <dce/dce_svc.h>
#include <dce/dcesvcmsg.h>
#include <dce/dcepksmsg.h>
#include <dce/dcepkssvc.h>  
#include <dce/dcepksmac.h> 
#include <dce/sec_pk.h>
}

#include "sec_bsafe.h"
#include "sec_pkss_db.h"
#include "sec_pkss_asn.h"
#include "sec_pkss_util.h"
#include "sec_pkss_lits.h"
#include "sec_pkss_clt_if.h"
#include "sec_pkss_dh_parms.h"
#include "pkss_config_common.h"
#include "pkss_config_server.h"

#ifdef MALLOCTRACE
extern "C"
{
#include "malloc_trace.h"
}
#endif


//
// declare global serviceability handle and initialization flag
//

dce_svc_handle_t pks_svc_handle; // defined global in sec_pkss_util.cxx

unsigned  pks_svc_handle_is_valid = 0;

//
// Local interface to initialize PKSS serviceability handle - used
// by sec_pkss_init_svc_handle only.
//

static void pkss_init_svc_handle ()
{
  error_status_t estatus = 0;
  if (!(pks_svc_handle_is_valid))
  {
    pks_svc_handle = dce_svc_register (pks_svc_table,
				       (idl_char*)PKSS_SVC_PROG_NAME,
				       &estatus);  

    if (estatus != svc_s_ok)
    {
      fprintf(stderr, "unable to register serviceability message table\n");
      pks_svc_handle_is_valid = false;
      return;
    }

    dce_msg_define_msg_table (pks__table,
			      sizeof (pks__table) / sizeof (pks__table[0]),
			      &estatus);

    if ((estatus == msg_s_ok_text) ||
        (estatus == msg_s_ok))
    {
      pks_svc_handle_is_valid = true;
      DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from dce_msg_define_msg_table : 0x%x", estatus));
    }

    else
    {
      pks_svc_handle_is_valid = false;
      DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from dce_msg_define_msg_table : 0x%x", estatus));
    }
  }

} // End pkss_init_svc_handle


pthread_once_t once_control = pthread_once_init;

//
// Exposed interface to initialize PKSS serviceability handle.
//

error_status_t sec_pkss_init_svc_handle (void)
{
  error_status_t estatus = 0;

  estatus = pthread_once (&once_control, pkss_init_svc_handle);

  if (pks_svc_handle_is_valid != true)
    estatus = -1;

  return (estatus);

} // End sec_pkss_init_svc_handle


#if 0

//
// Zero sec_pk_data_t's.
//

void sec_pkss_zero
(
  sec_pk_data_t *p
)

{
  if (p)
    if (p->data) 
    {
      (void) memset (p->data, 0, p->len);
      p->len = 0;
    }

} // End sec_pkss_zero


//
// Zero and release allocated sec_pk_data_t's.
//

void sec_pkss_zero_and_free
(
  sec_pk_data_t *p
)

{
  sec_pkss_zero (p);
  if (p)
  {
    if (p->data)
    {
      free (p->data);
      p->data = 0;
    }
  }
} // End sec_pkss_zero_and_free

#endif  // 0


//
// Zero sec_pkss_msg_buf_t's.
//

void sec_pkss_zero
(
  sec_pkss_msg_buf_t *p
)

{
  if (p)
    if (p->msg_p)
    {
      (void) memset (p->msg_p, 0, p->msg_len);
      p->msg_len = 0;
    }

} // End sec_pkss_zero


//
// Zero and release allocated sec_pkss_msg_buf_t's.
//

void sec_pkss_zero_and_free
(
  sec_pkss_msg_buf_t *p
)

{
  sec_pkss_zero (p);
  if (p)
    if (p->msg_p)
    {
      rpc_ss_free (p->msg_p);
      p->msg_p = 0;
    }

} // End sec_pkss_zero_and_free


//
// Zero and release fully allocated sec_pkss_msg_buf_t's.
//

void sec_pkss_zero_and_free
(
  sec_pkss_msg_buf_t **p
)

{
  sec_pkss_zero (*p);
  if (p)
    if (*p) 
    {
      if ((*p)->msg_p)
        free ((*p)->msg_p);
      free (*p);
      *p = 0;
    }

} // End sec_pkss_zero_and_free


//
// Zero sec_pk_gen_data_t's.
//

void sec_pkss_zero
(
  sec_pk_gen_data_t *p
)

{
  if (p)
    if (p->data) 
    {
      (void) memset (p->data, 0, p->len);
      p->len = 0;
    }

} // End sec_pkss_zero


//
// Zero and release allocated sec_pk_gen_data_t's.
//

void sec_pkss_zero_and_free
(
  sec_pk_gen_data_t *p
)

{
  sec_pkss_zero (p);
  if (p)
  {
    if (p->data)
    {
      free (p->data);
      p->data = 0;
    }
  }

} // End sec_pkss_zero_and_free


//
// Zero and release fully allocated sec_pk_gen_data_t's.
//

void sec_pkss_zero_and_free
(
  sec_pk_gen_data_t **p
)

{
  if (p)
  {
    sec_pkss_zero_and_free (*p);
    if (*p)
    {
      free (*p);
      *p = 0;
    }
  }

} // End sec_pkss_zero_and_free


//
// Zero sec_pkss_dbkey_t's.
//

void sec_pkss_zero
(
  sec_pkss_dbkey_t *p
)

{
  if (p)
    if (p->data) 
    {
      (void) memset (p->data, 0, p->len);
      p->len = 0;
    }

} // End sec_pkss_zero


//
// Zero and release allocated sec_pkss_dbkey_t's.
//

void sec_pkss_zero_and_free
(
  sec_pkss_dbkey_t *p
)

{
  sec_pkss_zero (p);
  if (p)
  {
    if (p->data)
    {
      free (p->data);
      p->data = 0;
    }
  }

} // End sec_pkss_zero_and_free


//
// Zero and free fully allocated sec_pkss_dbkey_t's.
//

void sec_pkss_zero_and_free
(
  sec_pkss_dbkey_t **p
)

{
  sec_pkss_zero_and_free (*p);
  if (p)
    if (*p)
    {
      free (*p);
      *p = 0;
    }

} // End sec_pkss_zero_and_free


//
// Zero sec_pkss_dbrec_t's.
//

void sec_pkss_zero
(
  sec_pkss_dbrec_t *p
)

{
  if (p)
    if (p->asn_rec_p)
    {
      (void) memset (p->asn_rec_p, 0, p->asn_rec_len);
      p->asn_rec_len = 0;
    }

} // End sec_pkss_zero


//
// Zero and release allocated sec_pkss_dbrec_t's.
//

void sec_pkss_zero_and_free
(
  sec_pkss_dbrec_t *p
)

{
  sec_pkss_zero (p);
  if (p)
  {
    if (p->asn_rec_p)
    {
      free (p->asn_rec_p);
      p->asn_rec_p = 0;
    }
  }

} // End sec_pkss_zero_and_free

//
// Zero and free fully allocated sec_pkss_dbrec_t's.
//

void sec_pkss_zero_and_free
(
  sec_pkss_dbrec_t **p
)

{
  sec_pkss_zero_and_free (*p);
  if (p)
    if (*p)
    {
      free (*p);
      *p = 0;
    }

} // End sec_pkss_zero_and_free


//
// Compare two sec_pk_gen_data_t's.
//

error_status_t sec_pkss_pk_gen_data_cmp
(
  sec_pk_gen_data_t *buf1_p,
  sec_pk_gen_data_t *buf2_p
)

{
  error_status_t estatus = 0;

  if (buf1_p->len != buf2_p->len)
  {
     return (1);
  }

  if (memcmp (buf1_p->data, buf2_p->data, buf2_p->len) != 0)
  {
     return (2);
  }

  //
  // Exit this function.
  //

  return (estatus);

} // End sec_pkss_pk_gen_data_cmp


error_status_t sec_pkss_get_dh_val
(
  void **dh_context_p,
  sec_pk_gen_data_t *clt_dh_val_p
)

{
  sec_pk_gen_data_t dh_parms;
  dh_parms.data = (unsigned char *)dh_parameters_BER_1024x256;
  dh_parms.len = sizeof (dh_parameters_BER_1024x256);
  error_status_t estatus = 0;

  estatus = sec_bsafe_diffie_hellman_phase_1 (dh_context_p,
                                              0,
                                              &dh_parms,
                                              clt_dh_val_p);

  if (estatus != 0)
  {
    dce_svc_printf(PKS_S_SEC_BSAFE_MSG, estatus);
  }

  // 
  // Exit this function.
  //

  return (estatus);

} // End sec_pkss_get_dh_val


//
// Obtain required symmetric key crypto info given a symmetric key
// encryption algorithm identifier.  Note that it is the caller's
// responsibility to free initial vector storage allocated in this
// function.
//

error_status_t sec_pkss_get_sym_crypto_info
(
  sec_bsafe_alg_type_t alg,
  sec_bsafe_alg_type_t *alg_id_pad_p,
  sec_bsafe_alg_type_t *alg_id_nopad_p,
  unsigned32 *pad_key_len_p,
  unsigned32 *nopad_key_len_p,
  sec_pk_gen_data_t **pad_iv_p,
  sec_pk_gen_data_t **nopad_iv_p
)

{

  //
  // local data
  //

  error_status_t estatus = 0;
  unsigned32 pad_iv_len = 0;
  unsigned32 nopad_iv_len = 0;


  //
  // Map ASN.1 algorithm identifier to sec_bsafe algorithm identifier.  Recall
  // that a mapping from ASN.1 to sec_bsafe is 1:2 where the "2" means padded
  // and unpadded versions of the same algorithm.
  //

  switch (alg)
  {
    case sec_bsafe_enc_des:
    case sec_bsafe_enc_des_nopad:
      *alg_id_pad_p = sec_bsafe_enc_des;
      *alg_id_nopad_p = sec_bsafe_enc_des_nopad;
      break;
  
    case sec_bsafe_enc_des_ede3:
    case sec_bsafe_enc_des_ede3_nopad:
      *alg_id_pad_p = sec_bsafe_enc_des_ede3;
      *alg_id_nopad_p = sec_bsafe_enc_des_ede3_nopad;
      break;

    default:
      dce_svc_printf(PKS_S_INVALID_ARG_MSG);
      goto rundown;

  } // End switch (alg)

  //
  // Obtain selected information about the chosen algorithm identifiers.
  //

  estatus = sec_bsafe_symmetric_alginfo (*alg_id_pad_p,
                                         pad_key_len_p,
                                         &pad_iv_len, 0, 0);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_bsafe_symmetric_alginfo : 0x%x", estatus));

  if (estatus != 0)
  {
    dce_svc_printf(PKS_S_SEC_BSAFE_MSG, estatus);
    goto rundown;
   }

  estatus = sec_bsafe_symmetric_alginfo (*alg_id_nopad_p,
                                         nopad_key_len_p,
                                         &nopad_iv_len, 0, 0);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from sec_bsafe_symmetric_alginfo : 0x%x", estatus));

  if (estatus != 0)
  {
    dce_svc_printf(PKS_S_SEC_BSAFE_MSG, estatus);
    goto rundown;
  }

  //
  // Obtain initial vector for encryptions and decryptions.
  //

  if (pad_iv_p != 0)
  {
    *pad_iv_p = (sec_pk_gen_data_t *) calloc (1, sizeof (sec_pk_gen_data_t *));

    if ( !(*pad_iv_p) )
    {
      dce_svc_printf(PKS_S_NO_MEMORY_MSG);
      goto rundown;
    }

    (*pad_iv_p)->len = pad_iv_len;
    (*pad_iv_p)->data = (unsigned char *) calloc (1, (*pad_iv_p)->len);

    if ( !((*pad_iv_p)->data) )
    {
      dce_svc_printf(PKS_S_NO_MEMORY_MSG);
      goto rundown;
    }

  } // End if (pad_iv_p != 0)

  if (nopad_iv_p != 0)
  {
    *nopad_iv_p = (sec_pk_gen_data_t *)
			 calloc (1, sizeof (sec_pk_gen_data_t *));

    if ( !(*nopad_iv_p) )
    {
      dce_svc_printf(PKS_S_NO_MEMORY_MSG);
      goto rundown;
    }

    (*nopad_iv_p)->len = nopad_iv_len;
    (*nopad_iv_p)->data = (unsigned char *) calloc (1, (*nopad_iv_p)->len);

    if ( !((*nopad_iv_p)->data) )
    {
      dce_svc_printf(PKS_S_NO_MEMORY_MSG);
      goto rundown;
    }

  } // End if (nopad_iv_p != 0)

  //
  // Exit this function.
  //

  return (estatus);

  //
  // on error - zero out all stack data, allocated data and return
  //

rundown:

  pad_iv_len = 0;
  nopad_iv_len = 0;
  sec_pkss_zero_and_free (pad_iv_p);
  sec_pkss_zero_and_free (nopad_iv_p);

  //
  // Exit this function.
  //

  return (estatus);

} // End sec_pkss_get_sym_crypto_info


unsigned32 sec_pkss_get_sym_alg_idx
(
  PkssAlgListC *asn_alg_list_p,
  unsigned32 *alg_p,
  unsigned32 *alg_idx_p
)

{
  //
  // Obtain algorithm list and algorithm to use from algorithm list received
  // from the client.
  //

  unsigned32 estatus = 0;
  unsigned32 test_alg = 0;
  size_t num_algs = asn_alg_list_p->child_count();
  *alg_p = 0;

  for (unsigned i = 0; i < num_algs; i++)
  {
    estatus = (*asn_alg_list_p)[i]->get_value (test_alg);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from ASN get_value : 0x%x", estatus));

    if (estatus != 0)
    {
      return (estatus);
    }

    for (unsigned j = 0; j < num_algs; j++)
    {
      if (test_alg == pkss_master_alg_list[j])
      {
        *alg_p = test_alg;
	*alg_idx_p = i;
	return (0);
      }
    }
  }

  //
  // Exit this function.
  //

  return (estatus);

} // End sec_pkss_get_sym_alg_idx



static error_status_t sec_pkss_calculate_hash(const char * p1_p,
                                              const char * p2_1,
                                              sec_pk_gen_data_t * h_p);


error_status_t sec_pkss_get_pwd_hashes
(
    const char *pwd_p,
    const char *username_p,
    unsigned32 hash_len,
    sec_pk_gen_data_t *h1_p,
    sec_pk_gen_data_t *h2_p
) {

// The hashing algorithm used in the PKSS is a variant of the RFC 1507
// algorithm, consisting of multiple alternating rounds of DES and MD2.
// The algorithm is expressly designed to be slow, to reduce the opportunity
// of a password-guessing attack.  The hash also includes the cell-relative
// portion of the associated username as a salt, along with the domain-UUID
// to prevent the use of a pre-calculated hash dictionary.
//
// The RFC 1507 algorithm (modified for the DCE principal naming environment)
// is as follows:
//
// a) Concatenate the provided password with the username and compute TEMP
//    as the MD2 hash of this quantity.
//
// b) Repeat the following 40 times:
//    Use the first 64 bits of TEMP as a DES key to encrypt the second 64
//    bits.  XOR the result with the first 64 bits of TEMP.  Compute a new
//    TEMP as MD2 of the 128 bit result.
//
// c) Use the final 64 bits of the result as H1 (key-protection key), and
//    the first 64 bits as H2 (proof of knowledge of password).
//
// To support possible future use of > 64 bit symmetric algorithms, we wish to
// extend this algorithm to generate two 128-bit hash values.  The method used
// to do this is very simple-minded: Use the above hash algorithm, but use the
// entire 128-bit result of step (b) as H1.  To generate H2, repeat the entire
// algorithm, but instead of starting with a concatenation {password|username},
// use {username|password}.
//

    error_status_t status;
    status = sec_pkss_calculate_hash(pwd_p, username_p, h1_p);
    if (status)
      return status;
    status = sec_pkss_calculate_hash(username_p, pwd_p, h2_p);
    if (status)
      return status;

    if (hash_len == 8)
    {
       (void) memset (&(h1_p->data[8]), 0, hash_len);
       (void) memset (&(h2_p->data[8]), 0, hash_len);
    }

    if (hash_len == 24)
    {
       unsigned char *h1_data_p = h1_p->data;
       unsigned char *h2_data_p = h2_p->data;
       h1_p->data = (idl_byte *) realloc (h1_p->data, hash_len);
       h2_p->data = (idl_byte *) realloc (h2_p->data, hash_len);
       (void) memcpy (h1_p->data, h1_data_p, 16);
       (void) memcpy (&(h1_p->data[16]), h1_data_p, 8);
       (void) memcpy (h2_p->data, h2_data_p, 16);
       (void) memcpy (&(h2_p->data[16]), h2_data_p, 8);
    }

    if (hash_len != 16)
    {
       h1_p->len = hash_len;
       h2_p->len = hash_len;
    }

    return (0);

} // sec_pkss_get_pwd_hashes


static error_status_t sec_pkss_calculate_hash(const char * p1_p,
                                              const char * p2_p,
                                              sec_pk_gen_data_t * h_p) {

    void * hash_context;
    error_status_t status = 0;
    sec_pk_gen_data_t temp = {0, 0};
    sec_pk_gen_data_t temp1 = {0, 0};
    sec_pk_gen_data_t des_key = {0, 0};
    sec_pk_gen_data_t des_data = {0, 0};
    sec_pk_gen_data_t des_iv = {0, 0};
    int i, j;
    int p;
    unsigned char iv[8];
    
// Step A:
    status = sec_bsafe_hash_init(sec_bsafe_hash_MD2,
                                 &hash_context);

    if (status) return status;

    temp.len = strlen(p1_p);
    temp.data = (unsigned char *)p1_p;
    
    status = sec_bsafe_hash_update(&hash_context,
                                   &temp);
    temp.data = NULL;
    if (status) return status;

    temp.len = strlen(p2_p);
    temp.data = (unsigned char *)p2_p;
    
    status = sec_bsafe_hash_update(&hash_context,
                                   &temp);
    temp.data = NULL;
    if (status) return status;

    status = sec_bsafe_hash_final(&hash_context,
                                  &temp);
    if (status) return status;

// Now we have the initial TEMP (in temp)

// Step B:
    des_iv.len = 8;
    des_iv.data = iv;
    memset(&iv, 0, 8);
        
    for (i=0;i<40;i++) {

// b) Repeat the following 40 times:
//    Use the first 64 bits of TEMP as a DES key to encrypt the second 64
//    bits.  XOR the result with the first 64 bits of TEMP.  Compute a new
//    TEMP as MD2 of the 128 bit result.

	des_key.data = temp.data;
	des_key.len = 8;

	des_data.data = temp.data + 8;
	des_data.len = 8;

	status = sec_bsafe_symmetric_encrypt(sec_bsafe_enc_des_nopad,
	                                     &des_key,
					     &des_iv,
					     &des_data,
					     &temp1);

	if (status) {
	    sec_pkss_zero_and_free(&temp);
	    return status;
	};
	
	for (j=0;j<8;j++) {
	    temp.data[j] ^= temp1.data[j];
	};

	sec_pkss_zero_and_free(&temp1);

	status = sec_bsafe_compute_hash(sec_bsafe_hash_MD2,
                                        &temp,
					&temp1);

	sec_pkss_zero_and_free(&temp);

	temp.len = temp1.len;
	temp.data = temp1.data;

    };


    h_p->data = temp.data;
    h_p->len = temp.len;
    return 0;

} // End sec_pkss_calculate_hash

//
// Cause a breakpoint trap while running in the debugger at arbitrary
// execution time points, but only if required for debugging.
//

#ifndef ENABLE_TRACE

void mtrace (const char *format_p, ...)
{
}

void mtrace ()
{
}

#else

void mtrace (const char *format_p, ...)
{
  va_list args;
  va_start (args, format_p);
  (void) vprintf (format_p, args);
  va_end(args);
  (void) fflush (stdout);
  raise (SIGTRAP);

} // End mtrace

void mtrace ()
{
  raise (SIGTRAP);

} // End mtrace

#endif // ENABLE_TRACE
