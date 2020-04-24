/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: volDesc.h,v $
 * Revision 1.1.12.1  1996/10/02  21:11:28  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:51:29  damon]
 *
 * Revision 1.1.7.2  1994/07/13  22:30:45  devsrc
 * 	merged with bl-10
 * 	[1994/06/29  12:12:05  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  16:04:51  mbs]
 * 
 * Revision 1.1.7.1  1994/06/09  14:25:34  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:37:32  annie]
 * 
 * Revision 1.1.3.3  1993/01/21  16:33:20  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  18:27:21  cjd]
 * 
 * Revision 1.1.3.2  1992/11/24  20:44:16  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:40:07  bolinger]
 * 
 * Revision 1.1  1992/01/19  02:51:41  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/* Copyright (C) 1991 Transarc Corp. -- all rights reserved */

/* $Header: /u0/rcs_trees/dce/rcs/file/xaggr/volDesc.h,v 1.1.12.1 1996/10/02 21:11:28 damon Exp $ */

#ifndef	_VOLDESCH_
#define	_VOLDESCH_

#include <aggr.h>
#include <dcedfs/volume.h>
#include <astab.h>

#define	VOLDESC		"./volDesc"

struct volDesc {
    struct astab vstab;
    struct vol_status vstatus;
};

#endif	/* _VOLDESCH_ */
