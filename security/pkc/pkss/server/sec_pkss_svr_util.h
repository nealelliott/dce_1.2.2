//
// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// All Rights Reserved
//
// HISTORY
// $Log: sec_pkss_svr_util.h,v $
// Revision 1.1.2.2  1996/11/13  20:41:55  arvind
// 	PKSS bug fix from DEC
// 	[1996/11/03  23:40 UTC  arvind  /main/DCE_1.2.2/arvind_capi2/1]
//
// Revision 1.1.2.1  1996/10/04  14:25:20  arvind
// 	OSF DCE 1.2.2 Drop 4
// 	[1996/10/04  13:47:17  arvind]
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
// Module:  sec_pkss_svr_util.h
//
// Purpose:  This module contains the function prototypes for utility
//	     functions of particular interest to the PKSS server.
//

//
// Construct and return a PKSS database record key.  Note that it is the
// caller's responsibility to zero and release the storage associated with
// the database key when finished with it.
//

#ifndef SEC_PKSS_SVR_UTIL_H
#define SEC_PKSS_SVR_UTIL_H

#include <pthread.h>
#include "sec_pkss_db.h"
#include "sec_pkss_dbkey_class.h"

//
// Context block
//

typedef struct
{
  unsigned32 proto;
  unsigned32 sym_alg;
  unsigned32 state;
  unsigned32 deleted;
  pthread_mutex_t mutex;
  // ctx_timer;
  PkssDbKeyC *dbkey_p;
  sec_pk_gen_data_t *svr_nonce_p;
  sec_pk_gen_data_t *clt_nonce_p;
  sec_pk_gen_data_t *crypto_key_p;
} sec_pkss_context_t;


//
// sec_pkss_db_lookup prototype
//

error_status_t sec_pkss_db_lookup
(
  PkssDbKeyC *,
  sec_pkss_dbrec_t *
);


//
// sec_pkss_do_usage_flags prototype
//

error_status_t sec_pkss_do_usage_flags
(
  PkssDbKeyC *,
  sec_pkss_dbrec_t *
);

#endif // SEC_PKSS_SVR_UTIL_H

