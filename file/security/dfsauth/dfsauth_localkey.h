/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dfsauth_localkey.h,v $
 * Revision 1.1.9.1  1996/10/02  20:58:56  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:47:23  damon]
 *
 * Revision 1.1.4.1  1994/06/09  14:19:30  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:31:24  annie]
 * 
 * Revision 1.1.2.3  1993/01/21  15:19:15  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  15:57:21  cjd]
 * 
 * Revision 1.1.2.2  1992/11/24  18:32:16  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:23:54  bolinger]
 * 
 * Revision 1.1  1992/01/19  02:52:08  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *	dfsauth_localkey.h -- routines for accessing the local keyfile for dfsauth package
 *
 * Copyright (C) 1991 Transarc Corporation
 * All rights reserved.
 */

#ifndef TRANSARC_DFSAUTH_LOCALKEY_H
#define TRANSARC_DFSAUTH_LOCALKEY_H 1

#include <dcedfs/param.h>
#include <dcedfs/stds.h>

typedef enum dfs_krb_key_type {
  dfs_krb_key,
  dfs_krb_passwd
} dfs_krb_key_type_t;

IMPORT krb5_error_code Keytab_GetLocalKey _TAKES((
						  char *	keyfilenameP,
						  char *	prinNameP,
						  dfs_krb_key_type_t *	keyTypeP,
						  char **	theKeyPP
						));

#endif /* TRANSARC_DFSAUTH_LOCALKEY_H */



