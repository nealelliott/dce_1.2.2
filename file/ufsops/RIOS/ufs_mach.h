/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ufs_mach.h,v $
 * Revision 1.1.11.1  1996/10/02  21:02:15  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:49:46  damon]
 *
 * Revision 1.1.6.2  1994/06/09  14:23:34  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:35:24  annie]
 * 
 * Revision 1.1.6.1  1994/02/04  20:33:57  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:19:35  devsrc]
 * 
 * Revision 1.1.4.1  1994/01/28  20:43:53  annie
 * 	expand OSF copyright
 * 	[1994/01/28  20:42:26  annie]
 * 
 * Revision 1.1.2.2  1993/01/28  18:34:59  shl
 * 	Transarc delta: cburnett-ot6781-aix-jfs-dump-restore-support 1.3
 * 	  Selected comments:
 * 	    Add AIX JFS support to ufsops for the new portable directory
 * 	    dump/restore format.  Also fix some existing bugs in the
 * 	    ufsops code.  In addition push AIX specific function into
 * 	    a platform specific dir.
 * 	    AIX specific ufsops support.
 * 	    The first revision of delta didn't compile under OSF/1.
 * 	    We need one more round to get this to compile under OSF/1.
 * 	[1993/01/27  21:14:05  shl]
 * 
 * $EndLog$
*/
/* Copyright (C) 1990 Transarc Corporation -- All Rights Reserved */

/*
 * AIX UFS specific declarations/macros
 */

#ifndef	_UFS_MACH_H_
#define	_UFS_MACH_H_

#include <sys/inode.h>

#define	VOL_UFSFIRSTINODE	16	/* First "available" aix inode */

#define	UFSMAXINO(vfsp)	        aixmaxino(vfsp)
extern long aixmaxino();

#define UFS_CRTIME(vfsp, vsp) aix_fscrtime(vfsp, vsp)

#endif	/* _UFS_MACH_H_ */
