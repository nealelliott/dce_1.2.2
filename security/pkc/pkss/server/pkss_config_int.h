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
 * $Log: pkss_config_int.h,v $
 * Revision 1.1.2.1  1996/10/04  14:21:47  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/04  13:39:40  arvind]
 *
 * Revision /main/DCE_1.2.2/1  1996/09/09  21:28 UTC  arvind
 * 	PKSS drop (2) from DEC (DCE1.2.2)
 * 	[1996/09/04  14:27 UTC  arvind  /main/arvind_pkss/2]
 * 
 * $EndLog$
 */
/* (c) Copyright 1996,
 * Digital Equipment Corporation, Maynard, Massachusetts, USA
 * All Rights Reserved
 *
 */


#ifndef PKSS_CONFIG_INT_H_
#define PKSS_CONFIG_INT_H_

#ifndef __cplusplus
#error "This file requires a C++ compiler"
#endif

extern "C" {
#include <pthread.h>
#include <dns_cfg.h>
}

extern char initialized;

extern pthread_once_t configOnceBlock;

pthread_mutex_t init_mutex;

unsigned32 pkss__config_initialize(const char * n);

unsigned32 pkss_load_config(char * src, char * &dst, int absolute);

extern char * pkss__this_host;

#endif
