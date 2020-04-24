/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: compat_debug.h,v $
 * Revision 1.1.9.1  1996/10/02  17:54:21  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:42:06  damon]
 *
 * Revision 1.1.4.1  1994/06/09  14:13:13  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:26:26  annie]
 * 
 * Revision 1.1.2.3  1993/01/19  16:08:06  cjd
 * 	embedded copyright notice
 * 	[1993/01/19  15:11:35  cjd]
 * 
 * Revision 1.1.2.2  1992/11/24  17:54:57  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:09:47  bolinger]
 * 
 * Revision 1.1  1992/01/19  02:47:34  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * compat_debug.h -- definition of the compat package debugging symbols
 *
 * Copyright (C) 1991 Transarc Corporation
 * All rights reserved.
 */

#ifndef TRANSARC_COMPAT_DEBUG_H
#define TRANSARC_COMPAT_DEBUG_H 1

#include <dcedfs/param.h>
#include <dcedfs/stds.h>

#include <dcedfs/debug.h>

IMPORT unsigned long compat_debug_flag;

#define COMPAT_DEBUG_BIT_TRACE		1
#define COMPAT_DEBUG_BIT_PRINCIPAL	2
#define COMPAT_DEBUG_BIT_DCE_TRACE	3

/*
  COMPAT_DEBUG_BIT_TRACE		=> 0x00000001
  COMPAT_DEBUG_BIT_PRINCIPAL		=> 0x00000002
  COMPAT_DEBUG_BIT_DCE_TRACE		=> 0x00000004
*/

IMPORT void compat_LogParamError _TAKES((
					 char *	routineNameP,
					 char *	paramNameP,
					 int	logCondition,
					 char *	filenameP,
					 int	line
				       ));

#endif /* TRANSARC_COMPAT_DEBUG_H */
