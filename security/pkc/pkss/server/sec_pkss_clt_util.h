//
// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// All Rights Reserved
//
// HISTORY
// $Log: sec_pkss_clt_util.h,v $
// Revision 1.1.2.1  1996/10/04  14:23:37  arvind
// 	OSF DCE 1.2.2 Drop 4
// 	[1996/10/04  13:46:02  arvind]
//
// Revision /main/DCE_1.2.2/1  1996/09/09  21:29 UTC  arvind
// 	PKSS drop (2) from DEC (DCE1.2.2)
// 	[1996/09/04  14:30 UTC  arvind  /main/arvind_pkss/2]
// 
// $EndLog$
//
// (c) Copyright 1996,
// Digital Equipment Corporation, Maynard, Massachusetts, USA
// All Rights Reserved
//
// Module:  sec_pkss_svr_util.h
//
// Purpose:  This module contains function prototypes for utility functions
//           of particular interest to PKSS clients.
//

#ifndef SEC_PKSS_CLT_UTIL_H
#define SEC_PKSS_CLT_UTIL_H

extern "C"
{
#include <dce/rpc.h>
#include <dce/sec_pk_base.h>
}


//
// Client context handle - required by sec_pvtkey_* layer
//

typedef struct
{
  char *name_p;
  char *pkss_svc_profile_name_p;
  sec_pk_domain_t domain;
} sec_pkss_clt_ctx_t;


//
// Given an interface specification, return a handle to the first server
// that is up and receiving connections.
//

error_status_t sec_pkss_bind
(
  const char *,
  rpc_if_handle_t,
  rpc_binding_handle_t *
);


//
// Count the number of usage flags bits set, and return an error
// if that number is greater than one.
//

error_status_t sec_pkss_count_usages
(
  sec_pk_usage_flags_t
); 


//
// Set security information into a binding handle.  Used only for
// PKSS management API operations.
//

error_status_t sec_pkss_secure_clt_handle
(
  rpc_binding_handle_t
);


#endif // SEC_PKSS_CLT_UTIL_H
