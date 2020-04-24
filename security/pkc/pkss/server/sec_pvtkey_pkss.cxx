//
//
// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// All Rights Reserved
//
// HISTORY
// $Log: sec_pvtkey_pkss.cxx,v $
// Revision 1.1.2.1  1996/10/03  20:33:17  arvind
// 	OSF DCE 1.2.2 Drop 4
// 	[1996/10/03  19:33:28  arvind]
//
// Revision /main/DCE_1.2.2/1  1996/09/09  21:31 UTC  arvind
// 	PKSS drop from DEC (DCE1.2.2)
// 	[1996/08/30  15:58 UTC  arvind  /main/arvind_pkss/1]
// 
// $EndLog$
//
// (c) Copyright 1996,
// Digital Equipment Corporation, Maynard, Massachusetts, USA
// All Rights Reserved
//
// Module: sec_pvtkey_pkss.cxx
//
// Purpose:  This module contains implementations for the sec_pvtkey API -
//	     see latest version of OSF RFC 68.x for details.
//

extern "C"
{
#include <stdlib.h>
#include <string.h>
#include <dce/rpc.h>
#include <dce/dcepkssvc.h>
#include <dce/sec_pk_base.h>
#include <dce/sec_pvtkey_base.h>
}

#include "sec_pvtkey_pkss.h"
#include "sec_pkss_util.h"
#include "sec_pkss_clt_if.h"
#include "sec_pkss_mgmt_if.h"
#include "sec_pkss_clt_api.h"
#include "sec_pkss_mgmt_api.h"
#include "sec_pkss_clt_util.h"
#include "pkss_config_common.h"

//
// sec_pvtkey_pkss_open
//

error_status_t sec_pvtkey_pkss_open
(
  idl_char *name_p,
  sec_pk_domain_t *domain_p,
  sec_pk_mechanism_handle_t *pk_context_p
)

{

  //
  // Initialize the pks_svc_handle.
  //

  error_status_t estatus = 0;

  estatus = sec_pkss_init_svc_handle ();

  if (estatus != 0)
    return (estatus);

  //
  // Create and initialize PKSS client context.
  //

  sec_pkss_clt_ctx_t *clt_ctx_p = (sec_pkss_clt_ctx_t *)
				  calloc (1, sizeof (sec_pkss_clt_ctx_t));

  clt_ctx_p->name_p = (char *)
		       calloc (1, strlen ((const char *)name_p)+1);

  (void) memcpy (clt_ctx_p->name_p,
		 (const char *)name_p,
		 strlen ((const char *)name_p));

  (void) memcpy (&(clt_ctx_p->domain), domain_p, sizeof (sec_pk_domain_t));

  //
  // Initialize the PKSS configuration facility.
  //

  estatus = pkss_config__init (PKSS_SERVER_NAME);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from pkss_config__init : 0x%x", estatus));

  if (estatus != 0)
  {
    return (estatus);
  }

  //
  // Obtain PKSS service profile name - for rpc_ns_binding_import_begin.
  //

  clt_ctx_p->pkss_svc_profile_name_p = pkss_config__cds__service_profile_name();

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from pkss_config__cds__service_profile_name : 0x%x",
		   estatus));

  if (clt_ctx_p->pkss_svc_profile_name_p == 0)
  {
    return (estatus);
  }

  *pk_context_p = (sec_pk_mechanism_handle_t *)clt_ctx_p;

  //
  // Exit this function.
  //

  return (estatus);

} // End sec_pvtkey_pkss_open


//
// sec_pvtkey_pkss_close
//

error_status_t sec_pvtkey_pkss_close
(
  sec_pk_mechanism_handle_t pk_context
)

{
  error_status_t estatus = 0;

  //
  // Obtain pointer to PKSS client context.
  //

  sec_pkss_clt_ctx_t *clt_ctx_p = (sec_pkss_clt_ctx_t *)pk_context;

  //
  // Zero and free client's name.
  //

  (void) memset (clt_ctx_p->name_p,
		 0,
		 strlen ((const char *)clt_ctx_p->name_p));
  free (clt_ctx_p->name_p);
  clt_ctx_p->name_p = 0;

  //
  // Zero the domain uuid.
  //

  (void) memset (&(clt_ctx_p->domain), 0, sizeof (clt_ctx_p->domain));

  //
  // Release the PKSS client context storage.
  //

  free (clt_ctx_p);

  //
  // Exit this function.
  //

  return (estatus);

} // End sec_pvtkey_pkss_close


//
// sec_pvtkey_pkss_store
//

error_status_t sec_pvtkey_pkss_store
(
  sec_pk_mechanism_handle_t pk_context,
  idl_char *pwd_p,
  sec_pk_usage_flags_t key_usages,
  unsigned32 key_vno,
  sec_pk_data_t *kxu_p,
  sec_pk_data_t *kou_p
)

{
  //
  // Obtain pointer to PKSS client context.
  //

  sec_pkss_clt_ctx_t *clt_ctx_p = (sec_pkss_clt_ctx_t *)pk_context;

  //
  // Add new user.
  //

  error_status_t estatus = 0;
  unsigned32 new_key_vno = 0;
  estatus = sec_pkss_mgmt_user_add_clt ((const char *)
					clt_ctx_p->pkss_svc_profile_name_p,
					(const char *)clt_ctx_p->name_p,
			      		clt_ctx_p->domain,
			      		key_usages,
			      		key_vno,
	  				(sec_pk_pvtkey_t *)kxu_p,
					(sec_pk_pubkey_t *)kou_p,
	      				(const char *)pwd_p,
					&new_key_vno);
  if (estatus != 0)
  {
    // error
  }

  //
  // Exit this function.
  //

  return (estatus);

} // End sec_pvtkey_pkss_store


//
// sec_pvtkey_pkss_generate
//

error_status_t sec_pvtkey_pkss_generate
(
  sec_pk_mechanism_handle_t pk_context,
  idl_char *pwd_p, 
  sec_pk_algorithm_id_t *alg_id_p,
  unsigned32 key_len,
  sec_pk_usage_flags_t key_usages,
  unsigned32 key_vno,
  sec_pk_data_t *new_kou_p
)

{
  //
  // Obtain pointer to PKSS client context.
  //

  sec_pkss_clt_ctx_t *clt_ctx_p = (sec_pkss_clt_ctx_t *)pk_context;

  //
  // Add new user to PKSS database (server shall generate and return new
  // asymmetric key pair.
  //

  error_status_t estatus = 0;
  unsigned32 new_key_vno = 0;
  estatus = sec_pkss_mgmt_user_add_svr ((const char *)
                                        clt_ctx_p->pkss_svc_profile_name_p,
					(const char *)clt_ctx_p->name_p,
			      		clt_ctx_p->domain,
			      		key_usages,
			      		key_vno,
					(const char *)pwd_p,
  					alg_id_p,
					key_len,
					(sec_pk_pubkey_t *)new_kou_p,
					&new_key_vno);
  if (estatus != 0)
  {
    // error
  }

  //
  // Exit this function.
  //

  return (estatus);

} // End sec_pvtkey_pkss_generate


//
// sec_pvtkey_pkss_get
//

error_status_t sec_pvtkey_pkss_get
(
  sec_pk_mechanism_handle_t pk_context,
  idl_char *pwd_p,
  sec_pk_usage_flags_t key_usages,
  unsigned32 *key_vno_p,
  sec_pk_data_t *kxu_p,
  sec_pk_data_t *kou_p
)

{
  //
  // Obtain pointer to PKSS client context.
  //

  sec_pkss_clt_ctx_t *clt_ctx_p = (sec_pkss_clt_ctx_t *)pk_context;

  //
  // Obtain client's asymmetric key pair.
  //

  error_status_t estatus = 0;
  estatus = sec_pkss_get_key ((const char *)
                              clt_ctx_p->pkss_svc_profile_name_p,
			      (const char *)clt_ctx_p->name_p,
			      clt_ctx_p->domain,
			      key_usages,
			      *key_vno_p,
			      (const char *)pwd_p,
			      (sec_pk_pvtkey_t *)kxu_p,
			      (sec_pk_pubkey_t *)kou_p,
			      key_vno_p);
  //
  // Exit this function.
  //

  return (estatus);

} // End sec_pvtkey_pkss_get


//
// sec_pvtkey_pkss_update
//

error_status_t sec_pvtkey_pkss_update
(
  sec_pk_mechanism_handle_t pk_context,
  idl_char *old_pwd_p,
  idl_char *new_pwd_p,
  sec_pk_usage_flags_t key_usages,
  unsigned32 *key_vno_p,
  sec_pk_data_t *new_kxu_p,
  sec_pk_data_t *new_kou_p
)

{
  //
  // Obtain pointer to PKSS client context.
  //

  sec_pkss_clt_ctx_t *clt_ctx_p = (sec_pkss_clt_ctx_t *)pk_context;

  //
  // Change client's asymmetric key pair (key pair supplied by caller).
  // If the old password is NULL, invoke the managememt version.
  //

  error_status_t estatus = 0;
  if (old_pwd_p)
  {
    estatus = sec_pkss_key_chg_clt ((const char *)
                                    clt_ctx_p->pkss_svc_profile_name_p,
				    (const char *)clt_ctx_p->name_p,
				    clt_ctx_p->domain,
			            key_usages,
			            *key_vno_p,
			            (const char *)old_pwd_p,
			            (const char *)new_pwd_p,
			            (sec_pk_pvtkey_t *)new_kxu_p,
			            (sec_pk_pubkey_t *)new_kou_p,
			            key_vno_p);

  } // End if (pwd_p)

  else
  {
    estatus = sec_pkss_mgmt_user_mod_clt ((const char *)
                                          clt_ctx_p->pkss_svc_profile_name_p,
					  (const char *)clt_ctx_p->name_p,
					  clt_ctx_p->domain,
					  key_usages,
					  *key_vno_p,
			                  (sec_pk_pvtkey_t *)new_kxu_p,
			                  (sec_pk_pubkey_t *)new_kou_p,
			                  (const char *)new_pwd_p,
			                  key_vno_p);

  } // End else path from if (pwd_p)

  //
  // Exit this function.
  //

  return (estatus);

} // End sec_pvtkey_pkss_update


//
// sec_pvtkey_pkss_update_generate
//

error_status_t sec_pvtkey_pkss_update_generate
(
  sec_pk_mechanism_handle_t pk_context,
  idl_char *old_pwd_p, 
  idl_char *new_pwd_p, 
  sec_pk_algorithm_id_t *pk_alg_id_p,
  unsigned32 pk_key_len,
  sec_pk_usage_flags_t key_usages,
  unsigned32 *key_vno_p,
  sec_pk_data_t *kou_p
)

{
  //
  // Obtain pointer to PKSS client context.
  //

  sec_pkss_clt_ctx_t *clt_ctx_p = (sec_pkss_clt_ctx_t *)pk_context;

  //
  // Set key change flag to change asymmetric key pair.
  //

  unsigned32 key_chg_flag = 1;

  //
  // Change client's asymmetric key pair (caller requests that the PKSS
  // generate the new key pair).  If the old password is NULL, invoke the
  // managememt version.
  //

  error_status_t estatus = 0;
  if (old_pwd_p)
  {
    estatus = sec_pkss_key_chg_svr ((const char *)
                                    clt_ctx_p->pkss_svc_profile_name_p,
				    (const char *)clt_ctx_p->name_p,
				    clt_ctx_p->domain,
				    key_usages,
				    *key_vno_p,
  				    (const char *)old_pwd_p,
  				    (const char *)new_pwd_p,
				    key_chg_flag,
				    pk_alg_id_p,
				    pk_key_len,
				    kou_p,
				    key_vno_p);
  } // End if (old_pwd_p)

  else
  {
    estatus = sec_pkss_mgmt_user_mod_svr ((const char *)
                                          clt_ctx_p->pkss_svc_profile_name_p,
					  (const char *)clt_ctx_p->name_p,
				          clt_ctx_p->domain,
				          key_usages,
				          *key_vno_p,
					  (const char *)new_pwd_p,
				          pk_alg_id_p,
				          pk_key_len,
					  kou_p,
					  key_vno_p);

  } // End else path of if (old_pwd_p)


  //
  // Exit this function.
  //

  return (estatus);

} // End sec_pvtkey_pkss_update_generate


//
// sec_pvtkey_pkss_delete
//

error_status_t sec_pvtkey_pkss_delete
(
  sec_pk_mechanism_handle_t pk_context,
  idl_char *pwd_p,
  sec_pk_usage_flags_t key_usages,
  unsigned32 key_vno
)

{
  //
  // Obtain pointer to PKSS client context.
  //

  sec_pkss_clt_ctx_t *clt_ctx_p = (sec_pkss_clt_ctx_t *)pk_context;

  //
  // Delete the indicated user.
  //

  error_status_t estatus = 0;
  estatus = sec_pkss_mgmt_user_del ((const char *)
                                    clt_ctx_p->pkss_svc_profile_name_p,
				    (const char *)clt_ctx_p->name_p,
				    clt_ctx_p->domain,
				    key_usages,
				    key_vno);
  //
  // Exit this function.
  //

  return (estatus);

} // End sec_pvtkey_pkss_delete


