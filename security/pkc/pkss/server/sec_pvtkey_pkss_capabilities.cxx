//
// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// All Rights Reserved
//
// HISTORY
// $Log: sec_pvtkey_pkss_capabilities.cxx,v $
// Revision 1.1.2.1  1996/10/03  20:33:29  arvind
// 	OSF DCE 1.2.2 Drop 4
// 	[1996/10/03  19:33:38  arvind]
//
// Revision /main/DCE_1.2.2/1  1996/09/09  21:31 UTC  arvind
// 	PKSS drop from DEC (DCE1.2.2)
// 	[1996/08/30  15:59 UTC  arvind  /main/arvind_pkss/1]
// 
// $EndLog$
//
// (c) Copyright 1996,
// Digital Equipment Corporation, Maynard, Massachusetts, USA
// All Rights Reserved
//
// Module: sec_pvtkey_pkss.cxx
//
// Purpose:  This module implements sec_pkss_capabilities
//

extern "C"
{
#include <stdlib.h>
#include <string.h>
#include <dce/dcepkssvc.h>
#include <dce/sec_pk_base.h>
#include <dce/sec_pvtkey_base.h>
}

#include "sec_bsafe.h"
#include "sec_pvtkey_pkss.h"

//
// sec_pvtkey_pkss_capabilities
//

error_status_t sec_pvtkey_pkss_capabilities
(
   sec_pk_mechanism_handle_t handle,
   void *capability_parm_p,
   sec_pk_pvtkey_capability *feature_p,
   boolean32 *supported_p
)

{
  unsigned char *parm_p = (unsigned char *)capability_parm_p;
  sec_bsafe_alg_type_t alg_type = (sec_bsafe_alg_type_t)(*parm_p);

  switch (alg_type)
  {
    case sec_enc_rsa_priv:
    case sec_enc_rsa_pub:
      break;

    default:
      alg_type = 0;
      break;

  } // End switch (alg_type)

  *supported_p = 0;

  //
  // Implement PKSS policy:  The PKSS will generate or allow storage for
  // RSA asymmetric keys only.  The PKSS allows deletions.  Nothing else
  // is supported as of 7/96.
  //

  if (((memcmp (&SEC_PVTKEY_CAP_KEYGEN, feature_p, sizeof (uuid_t)) == 0) ||
      (memcmp (&SEC_PVTKEY_CAP_KEYALG, feature_p, sizeof (uuid_t)) == 0)) &&
      (alg_type))
    *supported_p = 1;
  else
    if (memcmp (&SEC_PVTKEY_CAP_KEYDEL, feature_p, sizeof (uuid_t)) == 0)
      *supported_p = 1;

  return (0);

} // End sec_pvtkey_pkss_capabilities

