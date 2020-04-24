/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for 
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: tgt_pack.h,v $
 * Revision 1.1.4.1  1996/10/02  17:49:45  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:39:09  damon]
 *
 * Revision 1.1.2.1  1994/07/26  19:12:56  mckeen
 * 	Expanded copyrights
 * 	[1994/07/26  18:57:40  mckeen]
 * 
 * 	Initial Version
 * 	[1994/07/26  16:08:09  mckeen]
 * 
 * $EndLog$
 */
/* $Header: /u0/rcs_trees/dce/rcs/file/gateway/dfs_login/tgt_pack.h,v 1.1.4.1 1996/10/02 17:49:45 damon Exp $ */

/*
 * tgt_pack.h - Header file for tgt_pack module
 */

int tgt_pack
#ifdef __STDC__
	(krb5_creds *cred, char **packet, int *len);
#else 
	();
#endif /* __STDC__ */


