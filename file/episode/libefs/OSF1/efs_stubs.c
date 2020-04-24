/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: efs_stubs.c,v $
 * Revision 1.1.11.1  1996/10/02  17:24:19  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:32:01  damon]
 *
 * Revision 1.1.6.1  1994/06/09  14:00:30  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:35:45  annie]
 * 
 * Revision 1.1.4.3  1993/01/19  15:26:43  cjd
 * 	embedded copyright notice
 * 	[1993/01/19  13:46:54  cjd]
 * 
 * Revision 1.1.4.2  1992/11/24  16:24:56  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:22:55  bolinger]
 * 
 * Revision 1.1.2.2  1992/06/15  14:50:16  garyf
 * 	add stubs for dfs configure routines
 * 	[1992/06/15  14:49:26  garyf]
 * 
 * $EndLog$
*/
#include <dcedfs/param.h>
#include <errno.h>

px_configure() {
	return(EINVAL);
}

cm_configure() {
	return(EINVAL);
}

