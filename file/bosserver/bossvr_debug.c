/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: bossvr_debug.c,v $
 * Revision 1.1.13.1  1996/10/02  17:04:29  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:04:58  damon]
 *
 * Revision 1.1.7.2  1994/07/13  22:19:55  devsrc
 * 	merged with DCE1.1
 * 	[1994/07/11  15:17:37  devsrc]
 * 
 * 	merged with bl-10
 * 	[1994/06/28  19:58:33  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:53:58  mbs]
 * 
 * Revision 1.1.7.1  1994/06/09  13:52:22  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:26:08  annie]
 * 
 * Revision 1.1.7.1  1994/06/09  13:52:22  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:26:08  annie]
 * 
 * Revision 1.1.3.3  1993/01/18  19:56:12  cjd
 * 	Embedded copyright notice
 * 	[1993/01/18  19:32:40  cjd]
 * 
 * Revision 1.1.3.2  1992/11/24  15:41:22  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:09:17  bolinger]
 * 
 * Revision 1.1  1992/01/19  02:49:25  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *
 *  	bossvr_debug.c -- debugging flags for the bosserver
 *
 *	Copyright (C) 1990 Transarc Corporation
 *	All rights reserved.
 */

#include <dcedfs/stds.h>
#include <bossvr_debug.h>

/* put into data area so that its value can be changed without recompilation */
long bossvr_debug_flag = 0x0;

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/bosserver/bossvr_debug.c,v 1.1.13.1 1996/10/02 17:04:29 damon Exp $")
