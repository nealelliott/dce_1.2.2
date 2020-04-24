/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: agfs_mount.h,v $
 * Revision 1.1.100.1  1996/10/02  16:57:35  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:03:36  damon]
 *
 * Revision 1.1.94.2  1994/06/09  13:49:54  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:23:24  annie]
 * 
 * Revision 1.1.94.1  1994/02/04  20:04:11  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:05:41  devsrc]
 * 
 * Revision 1.1.92.1  1993/12/07  17:11:15  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  12:41:21  jaffe]
 * 
 * Revision 1.1.2.4  1993/01/18  19:49:45  cjd
 * 	Embedded copyright notice
 * 	[1993/01/18  19:23:15  cjd]
 * 
 * Revision 1.1.2.3  1992/11/24  15:36:36  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:04:06  bolinger]
 * 
 * Revision 1.1.2.2  1992/11/18  17:37:24  jaffe
 * 	Transarc delta: jdp-ot5415-mount-aggregate-on-attach 1.4
 * 	  Selected comments:
 * 	    This delta contains the bulk of the changes necessary to have aggregates
 * 	    mounted at attach time.  The purpose of this feature is to provide a
 * 	    means by which exported filesets (with no local mount points) will be
 * 	    sync'd periodically.  To accomplish this, we have created a new VFS type
 * 	    (agfs).  Whenever an aggregate is exported, a local mount of the aggregate
 * 	    is also done; then, when the sync daemon does its thing, the aggregate's sync
 * 	    vfsop will be called.  The agfs sync then calls the new sync aggrop, which
 * 	    in the case of Episode, calls efs_sync().  The UFS sync aggrop doen't do
 * 	    anything, since we know the UFS filesystem is mounted, and so, its sync vfsop
 * 	    will get called anyway.
 * 	    This delta must be bundled with the corresponding deltas in the Episode and
 * 	    test configurations.
 * 	    This file defines the data structures used in manipulating the agfs
 * 	    mount table.
 * 	    Remove leftover junk from $OFILES.
 * 	    Rearrange things slightly to make use of a vendor-supplied definition
 * 	    of MOUNT_AGFS.
 * 	    Define MOUNT_AGFS from AFS_MOUNT_AGFS, if necessary.
 * 	    Fix bogus linker warnings about shared libraries when compiling .o files.
 * 	[1992/11/17  19:31:39  jaffe]
 * 
 * $EndLog$
 */
/*
 * Copyright (C) 1992 Transarc Corporation.
 * All rights reserved.
 */

#ifndef _TRANSARC_DFS_AGFS_MOUNT_H
#define _TRANSARC_DFS_AGFS_MOUNT_H

/* $Header: /u0/rcs_trees/dce/rcs/file/agfs/agfs_mount.h,v 1.1.100.1 1996/10/02 16:57:35 damon Exp $ */

#include <dcedfs/osi.h>
#include <dcedfs/osi_mount.h>
#include <dcedfs/astab.h>
#include <dcedfs/aggr.h>

#ifdef AFS_HPUX_ENV
#define MOUNT_AGFS_NAME		"hp-dfs-agfs"
#endif /* AFS_HPUX_ENV */

#ifndef MOUNT_AGFS
#define MOUNT_AGFS OSI_MOUNT_TYPE_AGFS	/* From <dcedfs/osi_mount.h> */
#endif	/* MOUNT_AGFS */

#define	AGFS_MOUNT_FLAGS_PREALLOCATED	0x1
#define AGFS_MOUNT_STATE_INUSE		0x1

typedef struct agfs_mount {
    u_short		am_flags;
    u_short		am_state;
    struct aggr		am_aggr;
    struct vnode	am_rootvnode;
#ifdef AFS_AIX31_ENV
    struct gnode	am_rootgnode;
#elif defined(AFS_OSF_ENV)
    dev_t		am_rdev;
#endif
} agfs_mount_t;


#ifdef AFS_OSF_ENV
#define VFSTOAGFS(vfsP)    ((agfs_mount_t*)((vfsP)->m_data))
#else
#define VFSTOAGFS(vfsP)    ((agfs_mount_t*)((vfsP)->vfs_data))
#endif	/* AFS_OSF_ENV */

typedef struct astab	agfs_mount_data_t;


extern int agfs_GetMountTableEntry(
    agfs_mount_data_t*	mountDataP,
    dev_t			dev,
    int				flags,
    struct vnode*		devVP,
    struct osi_vfs*		vfsP,
    agfs_mount_t**		mtEntryPP
);

extern int agfs_PutMountTableEntry(agfs_mount_t *mtEntryP);

extern int agfs_inited;

#endif	/* _TRANSARC_DFS_AGFS_MOUNT_H */
