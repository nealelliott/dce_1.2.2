/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * Copyright (C) 1996, 1990 Transarc Corporation
 * All rights reserved.
 */
/* $Header: /u0/rcs_trees/dce/rcs/file/episode/vnops/efsmount.h,v 1.1.102.1 1996/10/02 17:44:59 damon Exp $ */
/*
 * HISTORY
 * $Log: efsmount.h,v $
 * Revision 1.1.102.1  1996/10/02  17:44:59  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:36:15  damon]
 *
 * $EndLog$
*/

#ifndef EFS_EFSMOUNT_H
#define EFS_EFSMOUNT_H

struct efs_mount_data {
    unsigned	flags;
    afs_hyper_t	volId;
    char	devName[32];
};

#define EFS_MOUNT_DATA_FLAG_USE_ID	0x1
#define EFS_MOUNT_DATA_FLAG_NFS		0x2


#ifdef AFS_OSF_ENV
/*
 * Required to support NFS exported Episode.
 * Changes to ufs_args should be reflected here 
 * as well.  See efs_mount().
 */
typedef struct partial_args {
    char  *fspec;
    int   exflags;	/* export related flags */
    uid_t exroot;	/* mapping for root uid */
} partial_args_t;
#endif /* AFS_OSF_ENV */
#endif /* EFS_EFSMOUNT_H */
