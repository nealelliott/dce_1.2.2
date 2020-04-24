/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dfsauth_localprincipal.h,v $
 * Revision 1.1.9.1  1996/10/02  20:59:01  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:47:25  damon]
 *
 * Revision 1.1.4.1  1994/06/09  14:19:33  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:31:27  annie]
 * 
 * Revision 1.1.2.3  1993/01/21  15:19:21  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  15:57:33  cjd]
 * 
 * Revision 1.1.2.2  1992/11/24  18:32:29  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:24:06  bolinger]
 * 
 * Revision 1.1  1992/01/19  02:52:10  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *	dfsauth_localprincipal.h -- routines for accessing the local principal for 
 * dfsauth package
 *
 * Copyright (C) 1991 Transarc Corporation
 * All rights reserved.
 */

#ifndef TRANSARC_DFSAUTH_LOCALPRINCIPAL_H
#define TRANSARC_DFSAUTH_LOCALPRINCIPAL_H 1

#include <dcedfs/param.h>
#include <dcedfs/stds.h>

IMPORT long dfsauth_PrinName_GetName _TAKES((
					     char *	baseNameP,
					     char *	uuidStringP,
					     char *	nameBufferP,
					     int	length
					   ));
IMPORT long dfsauth_PrinName_GetBaseName _TAKES((
						 char *	filenameP,
						 char *	nameBufferP,
						 int	length
					       ));
IMPORT long dfsauth_PrinName_SetBaseName _TAKES((
						 char *	filenameP,
						 char *	newNameP
					       ));

#endif /* TRANSARC_DFSAUTH_LOCALPRINCIPAL_H */


