/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* 
 * HISTORY
 * $Log: ftutil_svc.c,v $
 * Revision 1.1.7.1  1996/10/02  17:49:01  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:38:43  damon]
 *
 * Revision 1.1.2.2  1994/08/30  15:56:10  sasala
 * 	DFS S12Y general cleanup - rename ftutil and tools cmd catalogs, add ftutil msgs to xaggr dfsexport, and episode salvage and vnops subcomponents
 * 	[1994/08/30  00:19:50  sasala]
 * 
 * Revision 1.1.2.1  1994/08/17  15:41:58  ruby
 * 	Implement partial S12Y (Internationalization) for DFS commands and daemons
 * 	[1994/08/16  20:31:43  ruby]
 * 
 * 	Implement partial S12Y (Internationalization) for DFS commands and daemons
 * 
 * $EndLog$
 */
/*
 * compat_svc.c -- servicability initialization
 *
 * Copyright (C) 1991 Transarc Corporation
 * All rights reserved.
 */

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsftlmac.h>
#include <dfsftlsvc.h>
#include <dfsftlmsg.h>

DCE_SVC_DEFINE_HANDLE(ftl_svc_handle, ftl_svc_table, "ftl")
