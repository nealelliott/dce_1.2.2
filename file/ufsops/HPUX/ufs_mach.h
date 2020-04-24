/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * @HP_COPYRIGHT@
 */
/* Copyright (C) 1994 Transarc Corporation -- All Rights Reserved */
/*
 * HISTORY
 * $Log: ufs_mach.h,v $
 * Revision 1.1.15.1  1996/10/02  18:49:38  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:49:40  damon]
 *
 * Revision 1.1.10.2  1994/06/09  14:23:25  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:35:13  annie]
 * 
 * Revision 1.1.10.1  1994/02/04  20:33:46  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:19:30  devsrc]
 * 
 * Revision 1.1.8.2  1994/01/28  20:43:51  annie
 * 	expand OSF copyright
 * 	[1994/01/28  20:42:24  annie]
 * 
 * Revision 1.1.8.1  1994/01/26  20:31:13  mbs
 * 	Overwrote the file with D. Kinneys
 * 	version.  Commented out macros already
 * 	defined in ../ufs.h.  The macros were
 * 	left in because they really do belong
 * 	in this file.  A job for code cleanup...
 * 	[1994/01/26  20:29:19  mbs]
 * 
 * Revision 1.1.6.2  1993/07/19  19:36:28  zeliff
 * 	HP port of DFS
 * 	[1993/07/19  18:27:25  zeliff]
 * 
 * Revision 1.1.4.3  1993/07/16  20:47:27  kissel
 * 	*** empty log message ***
 * 	[1993/06/21  14:41:34  kissel]
 * 
 * 	Initial creation (modelled after the OSF1 version).
 * 	[1993/04/12  18:22:51  toml]
 * 
 * Revision 1.1.2.2  1993/06/04  17:52:42  kissel
 * 	Initial HPUX RP version.
 * 	[1993/06/03  21:53:35  kissel]
 * 
 * $EndLog$
 */

/*
 * HPUX UFS specific declarations/macros
 */

#ifndef	_UFS_MACH_H_
#define	_UFS_MACH_H_

# if 0
/* 
 * Already defined in ../ufs.h, but should be moved here. Also ufs.h
 * defines VOL_UFSFIRSTINODE to be 4, not 3 as is done here.  Which
 * is correct?
*/
#define	VOL_UFSFIRSTINODE	3       /* First "available" ufs inode */
#define LO_MOUNT(vfsp)		((struct mount *)(vfsp)->vfs_data)
#define LO_FS(mt)               ((struct fs *)(mt)->m_bufp->b_un.b_fs)
#define	UFSMAXINO(vfsp)		LO_FS(LO_MOUNT(vfsp))->fs_ncg * LO_FS(LO_MOUNT(vfsp))->fs_ipg
#endif

#define UFS_CRTIME(vfsp, vsp) hpux_fscrtime(vfsp, vsp)

#endif	/* _UFS_MACH_H_ */

