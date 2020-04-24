/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: aggrDesc.h,v $
 * Revision 1.1.12.1  1996/10/02  21:11:20  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:51:25  damon]
 *
 * Revision 1.1.7.2  1994/07/13  22:30:40  devsrc
 * 	merged with bl-10
 * 	[1994/06/29  12:12:01  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  16:04:46  mbs]
 * 
 * Revision 1.1.7.1  1994/06/09  14:25:27  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:37:24  annie]
 * 
 * Revision 1.1.3.2  1993/01/21  16:33:01  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  18:26:53  cjd]
 * 
 * Revision 1.1  1992/01/19  02:51:43  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/* Copyright (C) 1990 Transarc Corporation -- All Rights Reserved */

/* $Header: /u0/rcs_trees/dce/rcs/file/xaggr/aggrDesc.h,v 1.1.12.1 1996/10/02 21:11:20 damon Exp $ */

/*
 *			UFS Aggregate Descriptors
 */

#ifndef	_AGGRDESCH_
#define	_AGGRDESCH_

#include <aggr.h>
#include <astab.h>

#define	AGGRDESC	"./aggrDesc"

struct aggrDesc {
    struct astab astab;
    struct ufs_astab ufstab;
    struct ag_status astatus;
};

#endif	/* _AGGRDESCH_ */
