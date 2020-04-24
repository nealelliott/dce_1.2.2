/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dfsauth_pathnames.h,v $
 * Revision 1.1.9.1  1996/10/02  20:59:05  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:47:26  damon]
 *
 * Revision 1.1.4.1  1994/06/09  14:19:37  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:31:29  annie]
 * 
 * Revision 1.1.2.3  1993/01/21  15:19:27  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  15:57:46  cjd]
 * 
 * Revision 1.1.2.2  1992/11/24  18:32:48  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:24:18  bolinger]
 * 
 * Revision 1.1  1992/01/19  02:52:11  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *	dfsauth_pathnames.h -- routines and declarations for manipulation of
 * various paths of significance to a server using the dfsauth package
 *
 * Copyright (C) 1991 Transarc Corporation
 * All rights reserved.
 */

#ifndef TRANSARC_DFSAUTH_PATHNAMES_H
#define TRANSARC_DFSAUTH_PATHNAMES_H 1

#include <dcedfs/param.h>
#include <dcedfs/stds.h>

/* these are declared in dfsauth.h, since they are intended to used outside the package */
/*
IMPORT long DfsAuth_Server_SetNoAuthStatus _TAKES((IN int newStatus));

IMPORT long DfsAuth_Server_GetNoAuthStatus _TAKES((INOUT int * oldStatusP));
*/

IMPORT long dfsauth_GenerateAdminListPath _TAKES((
						  IN    char *	filenameP,
						  INOUT char *	fullpathnameP,
						  IN	int	fullpathlength
						));

#endif /* TRANSARC_DFSAUTH_PATHNAMES_H */

