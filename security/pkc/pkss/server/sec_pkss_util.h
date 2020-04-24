//
// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// All Rights Reserved
//
// HISTORY
// $Log: sec_pkss_util.h,v $
// Revision 1.1.2.1  1996/10/04  14:25:32  arvind
// 	OSF DCE 1.2.2 Drop 4
// 	[1996/10/04  13:47:36  arvind]
//
// Revision /main/DCE_1.2.2/1  1996/09/09  21:31 UTC  arvind
// 	PKSS drop (2) from DEC (DCE1.2.2)
// 	[1996/09/04  14:33 UTC  arvind  /main/arvind_pkss/2]
// 
// $EndLog$
//
// (c) Copyright 1996,
// Digital Equipment Corporation, Maynard, Massachusetts, USA
// All Rights Reserved
//
// PKSS utility functions prototypes and selected #defines required
// by the PKSS user client, management client, and server.
//

#ifndef SEC_PKSS_UTIL_H
#define SEC_PKSS_UTIL_H

extern "C"
{
#include <stdlib.h>
#include "sec_bsafe.h"
}
#include "sec_pkss_db.h"
#include "sec_pkss_asn.h"
#include "sec_pkss_lits.h"
#include "sec_pkss_types_if.h"

#ifndef MIN
#define MIN (x,y) (((x)<(y))?(x):(y))
#endif

#ifndef MAX
#define MAX (x,y) (((x)>(y))?(x):(y))
#endif

//
// Remove this when a real sec_pvtkey.h is available.
//

typedef void *sec_pk_mechanism_handle_t;

//
// external symbols for serviceability handle
//

extern  dce_svc_handle_t pks_svc_handle; // defined global in sec_pkss_util.cxx
extern  unsigned  pks_svc_handle_is_valid;

//
// Initialize PKSS serviceability handle.
//

error_status_t sec_pkss_init_svc_handle (void);

//
// Zero sec_pk_gen_data_t's.
//

void sec_pkss_zero
(
  sec_pk_gen_data_t *
);


//
// Zero sec_pk_data_t's.
//

void sec_pkss_zero
(
  sec_pk_data_t *
);


//
// Zero sec_pkss_msg_buf_t's.
//

void sec_pkss_zero
(
  sec_pkss_msg_buf_t *
);


//
// Zero and release allocated sec_pkss_msg_buf_t's.
//

void sec_pkss_zero_and_free
(
  sec_pkss_msg_buf_t **
);


//
// Zero sec_pkss_dbrec_t's.
//

void sec_pkss_zero
(
  sec_pkss_dbrec_t *
);


//
// Zero and release allocated sec_pkss_dbrec_t's.
//

void sec_pkss_zero_and_free
(
  sec_pkss_dbrec_t *
);


//
// Zero and release fully allocated sec_pkss_dbrec_t's.
//

void sec_pkss_zero_and_free
(
  sec_pkss_dbrec_t **
);


//
// Zero and release allocated sec_pk_gen_data_t's.
//

void sec_pkss_zero_and_free
(
  sec_pk_gen_data_t *
);

//
// Zero and release fully allocated sec_pk_gen_data_t's.
//

void sec_pkss_zero_and_free
(
  sec_pk_gen_data_t **
);


//
// Zero and release allocated sec_pk_data_t's.
//

void sec_pkss_zero_and_free
(
  sec_pk_data_t *
);

//
// Zero sec_pkss_dbkey_t's.
//

void sec_pkss_zero
(
  sec_pkss_dbkey_t *
);


//
// Zero and release allocated sec_pkss_dbkey_t's.
//

void sec_pkss_zero_and_free
(
  sec_pkss_dbkey_t *
);


//
// Zero and release fully allocated sec_pkss_dbkey_t's.
//

void sec_pkss_zero_and_free
(
  sec_pkss_dbkey_t **
);


//
// Zero sec_pkss_dbrec_t's.
//

void sec_pkss_zero
(
  sec_pkss_dbrec_t *
);


//
// Zero and release allocated sec_pkss_dbrec_t's.
//

void sec_pkss_zero_and_free
(
  sec_pkss_dbrec_t **
);


//
// Zero and release allocated sec_pkss_msg_buf_t's.
//

void sec_pkss_zero_and_free
(
  sec_pkss_msg_buf_t *
);

//
// Obtain a Diffie-Hellman seed value.
//

error_status_t sec_pkss_get_dh_val
(
  void **,
  sec_pk_gen_data_t *
);


//
// Compare two sec_pk_gen_data_t's.
//

error_status_t sec_pkss_pk_gen_data_cmp
(
  sec_pk_gen_data_t *,
  sec_pk_gen_data_t *
);


//
// Obtain password hashes H1 and H2.  Note that it is the caller's
// responsibility to free the storage allocated to contain the hashes.
//

error_status_t sec_pkss_get_pwd_hashes
(
  const char *,
  const char *,
  unsigned32,
  sec_pk_gen_data_t *,
  sec_pk_gen_data_t *
);

//
// Obtain required symmetric key crypto info given a symmetric key
// encryption algorithm identifier.  Note that it is the caller's
// responsibility to free initial vector storage allocated in this
// function.
//

error_status_t sec_pkss_get_sym_crypto_info
(
  sec_bsafe_alg_type_t,
  sec_bsafe_alg_type_t *,
  sec_bsafe_alg_type_t *,
  unsigned32 *,
  unsigned32 *,
  sec_pk_gen_data_t **,
  sec_pk_gen_data_t **
);


//
// Obtain algorithm list and algorithm to use from algorithm list received
// from the client.
//

unsigned32 sec_pkss_get_sym_alg_idx
(
  PkssAlgListC *,
  unsigned32 *,
  unsigned32 *
);

//
// Bind to server and return binding handle.
//

error_status_t sec_pkss_bind (rpc_binding_handle_t *);

//
// Hard-luck debugger tracing
//

void mtrace (const char *format_p, ...);

void mtrace (void);

#endif // SEC_PKSS_UTIL_H

