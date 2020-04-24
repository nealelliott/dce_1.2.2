/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cm_pioctlStub.c,v $
 * Revision 1.1.9.1  1996/10/02  21:09:47  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:50:23  damon]
 *
 * Revision 1.1.4.1  1994/06/09  14:24:07  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:36:05  annie]
 * 
 * Revision 1.1.2.3  1993/01/21  16:29:05  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  18:22:23  cjd]
 * 
 * Revision 1.1.2.2  1992/11/24  20:40:21  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:35:50  bolinger]
 * 
 * Revision 1.1  1992/01/19  02:50:12  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * cm_pioctlStub.c -- debugging pioctl stub, for user interface checking
 *
 *
 *	Copyright (C) 1991 Transarc Corporation
 *	All rights reserved.
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>

EXPORT int pioctl(pathname, ioctlNumber, ioctlData, followSymlinkFlag)
     char *		pathname;
     unsigned int	ioctlNumber;
     struct afs_ioctl *	ioctlData;
     int		followSymlinkFlag;
{
  return 0;
}

