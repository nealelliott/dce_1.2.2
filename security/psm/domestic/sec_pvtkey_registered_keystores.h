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
 * $Log: sec_pvtkey_registered_keystores.h,v $
 * Revision 1.1.2.1  1996/10/04  15:54:17  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/04  15:53:44  arvind]
 *
 * Revision /main/DCE_1.2.2/1  1996/09/09  21:32 UTC  arvind
 * 	     PKSS drop from DEC (DCE1.2.2)
 * 	     [1996/08/30  15:45 UTC  arvind  /main/arvind_pkss/1]
 * 
 * $EndLog$
 */
/* (c) Copyright 1996,
 * Digital Equipment Corporation, Maynard, Massachusetts, USA
 * All Rights Reserved
 * 
 * Module:
 * 
 * Purpose:
 * 
 */

#ifndef SEC_PKSS_REGISTERED_KEYSTORES_H_
#define SEC_PKSS_REGISTERED_KEYSTORES_H_

#ifdef __cplusplus 
extern "C" {
#endif

#define SEC_PVTKEY_KEYSTORE_MAGIC 0x14241ec9ul

#include "sec_pvtkey_switch.h"

/* 
 * This next #define can be changed, but 10 keystores should be plentry for 
 * the forseable future.
 */
#define MAX_KEYSTORES 10

extern keystore_epv installed_keystore[];


#ifdef __cplusplus 
}
#endif

#endif
