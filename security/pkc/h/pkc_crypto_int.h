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
 * $Log: pkc_crypto_int.h,v $
 * Revision 1.1.2.2  1996/11/13  20:38:13  arvind
 * 	CAPI drop from DEC
 * 	[1996/10/16  20:24 UTC  arvind  /main/DCE_1.2.2/arvind_capi/1]
 *
 * Revision 1.1.2.1  1996/10/03  20:28:13  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/03  19:41:13  arvind]
 * 
 * Revision /main/DCE_1.2.2/1  1996/09/09  21:25 UTC  arvind
 * 	PKSS drop from DEC (DCE1.2.2)
 * 	[1996/08/30  16:10 UTC  arvind  /main/arvind_pkss/1]
 * 
 * $EndLog$
 */
/*
 * (c) Copyright 1996,
 * Digital Equipment Corporation, Maynard, Massachusetts, USA
 * All Rights Reserved
 *
 */


#ifndef PKC_CRYPTO_INT_
#define PKC_CRYPTO_INT_

#include <dce/pkc_crypto_reg.h>

#ifdef __cplusplus
extern "C" {
#endif

void pkc_crypto_init(void);

extern pkc_signature_algorithm_t * pkc_preloaded_algorithms[];

#ifdef __cplusplus
}
#endif

#endif
