/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: sec_pvtkey_pkss.h,v $
 * Revision 1.1.2.1  1996/10/04  14:26:58  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/04  14:02:56  arvind]
 *
 * Revision /main/DCE_1.2.2/1  1996/09/09  21:32 UTC  arvind
 * 	PKSS drop from DEC (DCE1.2.2)
 * 	[1996/08/30  15:43 UTC  arvind  /main/arvind_pkss/1]
 * 
 * $EndLog$
 */
/* (c) Copyright 1996,
 * Digital Equipment Corporation, Maynard, Massachusetts, USA
 * All Rights Reserved
 *
 * Module: sec_pvtkey_pkss.h
 *
 * Purpose:  This module contains prototypes for the PKSS implementations of
 *	     the sec_pvtkey API.
 *
 */

#ifndef SEC_PVTKEY_PKSS_H
#define SEC_PVTKEY_PKSS_H

#ifndef IDLBASE_H
#include <dce/idlbase.h>
#endif

#ifdef __cplusplus
    extern "C" {
#endif

#ifndef nbase_v0_0_included
#include <dce/nbase.h>
#endif
#ifndef rgybase_v0_0_included
#include <dce/rgybase.h>
#endif

#include <dce/sec_pvtkey.h>


extern error_status_t sec_pvtkey_pkss_open
(
  idl_char *,
  sec_pk_domain_t *,
  sec_pk_mechanism_handle_t *
);

extern error_status_t sec_pvtkey_pkss_close
(
  sec_pk_mechanism_handle_t
);

extern error_status_t sec_pvtkey_pkss_capabilities
(
   sec_pk_mechanism_handle_t handle,
   void *capability_parm_p,
   sec_pk_pvtkey_capability *feature_p,
   boolean32 *supported_p
);


extern error_status_t sec_pvtkey_pkss_store
(
  sec_pk_mechanism_handle_t,
  idl_char *,
  sec_pk_usage_flags_t,
  unsigned32,
  sec_pk_data_t *,
  sec_pk_data_t *
);

extern error_status_t sec_pvtkey_pkss_delete
(
  sec_pk_mechanism_handle_t,
  idl_char *,
  sec_pk_usage_flags_t,
  unsigned32
);


extern error_status_t sec_pvtkey_pkss_generate
(
  sec_pk_mechanism_handle_t,
  idl_char *,
  sec_pk_algorithm_id_t *,
  unsigned32 key_length,
  sec_pk_usage_flags_t,
  unsigned32,
  sec_pk_data_t *
);

extern error_status_t sec_pvtkey_pkss_get
(
  sec_pk_mechanism_handle_t,
  idl_char *,
  sec_pk_usage_flags_t,
  unsigned32 *,
  sec_pk_data_t *,
  sec_pk_data_t *
);

extern error_status_t sec_pvtkey_pkss_update
(
  sec_pk_mechanism_handle_t,
  idl_char *,
  idl_char *,
  sec_pk_usage_flags_t,
  unsigned32 *,
  sec_pk_data_t *,
  sec_pk_data_t *
);

extern error_status_t sec_pvtkey_pkss_update_generate
(
  sec_pk_mechanism_handle_t handle,
  idl_char *,
  idl_char *,
  sec_pk_algorithm_id_t *,
  unsigned32,
  sec_pk_usage_flags_t,
  unsigned32 *,
  sec_pk_data_t *
);

#ifdef __cplusplus
}
#endif
#endif /* SEC_PVTKEY_PKSS_H */
