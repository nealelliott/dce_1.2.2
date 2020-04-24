//
// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// All Rights Reserved
//
// HISTORY
// $Log: sec_pkss_clt_api.h,v $
// Revision 1.1.2.1  1996/10/04  14:23:00  arvind
// 	OSF DCE 1.2.2 Drop 4
// 	[1996/10/04  13:45:34  arvind]
//
// Revision /main/DCE_1.2.2/1  1996/09/09  21:29 UTC  arvind
// 	PKSS drop (2) from DEC (DCE1.2.2)
// 	[1996/09/04  14:29 UTC  arvind  /main/arvind_pkss/2]
// 
// $EndLog$
//
// (c) Copyright 1996,
// Digital Equipment Corporation, Maynard, Massachusetts, USA
// All Rights Reserved
//
// PKSS client API prototypes and selected #define's
//

#ifndef SEC_PKSS_CLT_API_H
#define SEC_PKSS_CLT_API_H


extern "C"
{
#include <dce/nbase.h>
#include <dce/sec_pk_base.h>
}
#include "sec_pkss_clt_util.h"

//
// Retrieve user's private key for use in [RFC 68.2] login
//
// Input
//
//    pkss_svc_profile_name_p: Pointer to entry name in namespace where the
//                             search for compatible binding handles will
//                             begin.
//    name_p: Pointer to the user's canonical name within the
//            specified domain.
//    domain_id: The application domain this key belongs to.
//    key_usages: The usage(s) permitted of this keypair.
//    key_vno: The key version number. (0 may be specified to
//             indicate that the routine should select the next
//             appropriate or newest key version number).
//             (In the reference implementation, key_vno starts with
//             1, then monotonically increase 1 for each update.)
//    pwd_p:  Pointer to the user's password string.
//
// Output
//
//    pvt_key_p: Pointer to the encoded private key component of
//		 the user's asymmetric key pair.   It is the caller's
//		 responsibility to release the storage allocated for
//		 the private key component by this function.
//
//    pub_key_p: Pointer to the encoded public key component of
//		 the user's asymmetric key pair.   It is the caller's
//		 responsibility to release the storage allocated for
//		 the public key component by this function.
//
//    ret_key_vno_p:  Pointer to the actual key version number of
//		      the returned key.  It is the caller's responsibility
//		      to provide the storage to contain the actual key
//		      version number.
//
// Calls 	sec_pkss_clt_diffhell
//		sec_pkss_clt_pkey_get
//		
//

#if __cplusplus
extern "C"
#endif
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
 );

//
// Change user's password and/or asymmetric key pair (client generates
// new asymmetric key pair).
//
// Input
//
//    pkss_svc_profile_name_p: Pointer to entry name in namespace where the
//                             search for compatible binding handles will
//                             begin.
//    name_p: Pointer to the user's canonical name within the
//            specified domain.
//    domain_id: The application domain this key belongs to.
//    key_usages: The usage(s) permitted of this keypair.
//    key_vno: The key version number. (0 may be specified to
//             indicate that the routine should select the next
//             appropriate or newest key version number).
//             (In the reference implementation, key_vno starts with
//             1, then monotonically increase 1 for each update.)
//    old_pwd_p:  Pointer to the user's current password.
//    new_pwd_p:  Pointer to the user's new password.  Callers should
//		  set this field to NULL if only the asymmetric key pair
//		  is changing, but the password is staying the same.
//    new_pvt_key_p:  a pointer to a buffer containing the private key
//		      component of the new asymmetric key pair.
//    new_pub_key_p:  a pointer to a buffer containing the public key
//		      component of the new asymmetric key pair.
//
// Output
//
//    new_key_vno_p:  Pointer to the actual key version number of
//		      the new key.  It is the caller's responsibility
//		      to provide the storage to contain the actual key
//		      version number.
//
// Calls: 	sec_pkss_clt_diffhell
//		sec_pkss_clt_pkey_get
//		sec_pkss_chg_key_clt
//

#if __cplusplus
extern "C"
#endif
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
 );

//
// Change user's password and/or asymmetric key pair (server generates
// new asymmetric key pair).
//
// Input
//
//    pkss_svc_profile_name_p: Pointer to entry name in namespace where the
//                             search for compatible binding handles will
//                             begin.
//    name_p: Pointer to the user's canonical name within the
//            specified domain.
//    domain_id: The application domain this key belongs to.
//    key_usages: The usage(s) permitted of this keypair.
//    key_vno: The key version number. (0 may be specified to
//             indicate that the routine should select the next
//             appropriate or newest key version number).
//             (In the reference implementation, key_vno starts with
//             1, then monotonically increase 1 for each update.)
//    old_pwd_p:  Pointer to the user's current password.
//    new_pwd_p:  Pointer to the user's new password.  Callers should
//		  set this field to NULL if only the asymmetric key pair
//		  is changing, but the password is staying the same.
//    key_change_flag:  If set, instructs PKSS to generate a new
//			asymmetric key pair.
//    pk_alg_id_p: Pointer to algorithm to use when generating new public key.
//    pk_len: Key length of public key to be generated by PKSS server.
//
//
// Output
//
//    new_pub_key_p:  Pointer to a buffer to contain the public key
//		      component of the user's asymmetric key pair.  It is
//		      the caller's responsibility to call sec_pkss_key_free()
//		      to release the key storage allocated by this function to
//		      contain the key.
//
//    new_key_vno_p:  Pointer to the actual key version number of
//		      the new key.  It is the caller's responsibility
//		      provide the storage to contain the actual key
//		      version number.
//
// Calls: 	sec_pkss_clt_diffhell
//		sec_pkss_clt_pkey_get
//		sec_pkss_chg_key_svr
//

#if __cplusplus
extern "C"
#endif
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
 );

#endif // SEC_PKSS_CLT_API_H
