/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for 
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: inetid.h,v $
 * Revision 1.1.6.1  1996/10/02  17:49:59  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:39:22  damon]
 *
 * Revision /main/HPDCE02/1  1995/02/14  22:42 UTC  maunsell_c
 * 	@sys @host changes for nfs/dfs gateway
 * 
 * Revision /main/maunsell_mothra1.45/1  1995/01/31  19:21 UTC  maunsell_c
 * 	add @sys @host support for dfs_login/dfsgwd/dfsgw
 * 
 * Revision 1.1.2.1  1994/07/26  19:13:09  mckeen
 * 	Expanded copyrights
 * 	[1994/07/26  18:57:48  mckeen]
 * 
 * 	Initial Version
 * 	[1994/07/26  16:01:54  mckeen]
 * 
 * $EndLog$
 */
/* $Header: /u0/rcs_trees/dce/rcs/file/gateway/dfsgw/inetid.h,v 1.1.6.1 1996/10/02 17:49:59 damon Exp $ */

/* 
 * inetid.h - Internet flavor of netid.
 */

#ifndef _INETID_H_
#define _INETID_H_

#include <netid.h>

extern netid_p inetid_init();

#endif /* _INETID_H_ */
