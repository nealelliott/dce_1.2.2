/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: bossvr_afsFiles.h,v $
 * Revision 1.1.42.1  1996/10/02  17:04:22  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:04:53  damon]
 *
 * Revision 1.1.36.2  1994/06/09  13:52:15  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:25:57  annie]
 * 
 * Revision 1.1.36.1  1994/02/04  20:06:20  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:06:40  devsrc]
 * 
 * Revision 1.1.34.1  1993/12/07  17:12:59  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  12:56:45  jaffe]
 * 
 * Revision 1.1.2.3  1993/01/18  19:55:52  cjd
 * 	Embedded copyright notice
 * 	[1993/01/18  19:32:02  cjd]
 * 
 * Revision 1.1.2.2  1992/11/24  15:40:47  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:08:47  bolinger]
 * 
 * Revision 1.1  1992/01/19  02:49:19  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  bossvr_afsFiles.h -- macros defining path names for important AFS files
 * and protypes for routines dealing with those files and directories
 *
 *
 *	Copyright (C) 1991, 1990 Transarc Corporation
 *	All rights reserved.
 */


#ifndef _TRANSARC_BOSSVR_AFSFILES_H_
#define _TRANSARC_BOSSVR_AFSFILES_H_

#include <dcedfs/param.h>
#include <dcedfs/stds.h>

/* exported routines */
IMPORT int DirAccessOK _TAKES((
			       void
			     ));

IMPORT int GetRequiredDirPerm _TAKES((
				      IN char * path
				    ));

IMPORT int GetRequiredOwner _TAKES((
				    IN char * path
				  ));

#endif /* _TRANSARC_BOSSVR_AFSFILES_H_ */


