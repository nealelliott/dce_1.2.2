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
 * Revision 1.1.9.1  1996/10/02  18:50:01  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:49:44  damon]
 *
 * Revision 1.1.4.1  1994/06/09  14:23:30  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:35:19  annie]
 * 
 * Revision 1.1.2.2  1993/02/24  21:57:13  bolinger
 * 	OT defect 3542 (OSF/1 volops support):  system-specific definitions
 * 	for UFS volops code.
 * 	[1993/02/24  21:43:14  bolinger]
 * 
 * $EndLog$
 */
/* Copyright (C) 1990 Transarc Corporation -- All Rights Reserved */

/*
 * OSF/1 UFS specific declarations/macros
 */

#ifndef	_UFS_MACH_H_
#define	_UFS_MACH_H_
#include <ufs/inode.h>
#include <ufs/quota.h>
#include <ufs/ufsmount.h>

#define	VOL_UFSFIRSTINODE	3	/* First "available" OSF/1 UFS inode */

#define LO_MOUNT(vfsp)		((struct ufsmount *)(vfsp)->m_data)
#define LO_FS(mt)		((struct fs *)(mt)->um_fs)
#define	UFSMAXINO(vfsp) \
	(LO_FS(LO_MOUNT(vfsp))->fs_ncg * LO_FS(LO_MOUNT(vfsp))->fs_ipg)

#define UFS_CRTIME(vfsp, vsp) osf_fscrtime(vfsp, vsp)

#endif	/* _UFS_MACH_H_ */
