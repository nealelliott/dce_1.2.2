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
 * $Log: pkc_common.h,v $
 * Revision 1.1.2.2  1996/11/13  20:37:11  arvind
 * 	OSF DCE 1.2.2 Drop 5
 *
 * 	CAPI drop from DEC
 * 	[1996/10/16  20:24 UTC  arvind  /main/DCE_1.2.2/arvind_capi/1]
 * 	Revision 1.1.2.1  1996/10/03  20:27:38  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/03  19:39:06  arvind]
 *
 * Revision /main/DCE_1.2.2/1  1996/09/09  21:25 UTC  arvind
 * 	PKSS drop from DEC (DCE1.2.2)
 * 	[1996/08/30  16:09 UTC  arvind  /main/arvind_pkss/1]
 * 
 * $EndLog$
 */
/*
 * (c) Copyright 1996,
 * Digital Equipment Corporation, Maynard, Massachusetts, USA
 * All Rights Reserved
 *
 */

#ifndef PKC_COMMON_
#define PKC_COMMON_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <dce/dce.h>
#include <dce/nbase.h>
#include <dce/dce_msg.h>
#include <dce/dcepkcmsg.h>
#include <dce/dcepkcmac.h>
#include <dce/dcepkcsvc.h>

#define pkc_V1 1

void * pkc_malloc(size_t bytes);
void pkc_free(void * p);

extern dce_svc_handle_t pkc__svc_handle;
error_status_t pkc_init_svc_handle(void);

#ifdef __cplusplus
}
#endif

#endif

