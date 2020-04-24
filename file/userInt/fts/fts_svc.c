/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: fts_svc.c,v $
 * Revision 1.1.7.1  1996/10/02  21:24:55  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:50:37  damon]
 *
 * Revision 1.1.2.2  1994/10/06  20:27:17  agd
 * 	expand copyright
 * 	[1994/10/06  14:27:09  agd]
 * 
 * Revision 1.1.2.1  1994/09/16  21:28:48  ruby
 * 	Implement partial S12Y (Internationalization) for DFS commands and daemons
 * 	[1994/09/16  21:25:20  ruby]
 * 
 * $EndLog$
 */

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsftimac.h>
#include <dfsftisvc.h>
#include <dfsftimsg.h>

DCE_SVC_DEFINE_HANDLE(fts_svc_handle, fts_svc_table, "fts")
