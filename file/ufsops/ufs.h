/* Copyright (C) 1996, 1990 Transarc Corporation -- All Rights Reserved */
/* $Header: /u0/rcs_trees/dce/rcs/file/ufsops/ufs.h,v 1.1.842.1 1996/10/02 21:02:16 damon Exp $ */

/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ufs.h,v $
 * Revision 1.1.842.1  1996/10/02  21:02:16  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:49:47  damon]
 *
 * $EndLog$
 */

/*
 * Basic UFS specific declarations/macros
 */

#ifndef	TRANSARC_UFS_H
#define	TRANSARC_UFS_H

#include <dcedfs/xvfs_vnode.h>

#ifdef AFS_OSF_ENV
#include <ufs/inode.h>
#include <ufs/quota.h>
#include <ufs/ufsmount.h>
#endif /* AFS_OSF_ENV */

#ifdef	AFS_HPUX_ENV
#include <sys/fs.h>
#endif	/* AFS_HPUX_ENV */

extern struct icl_set *xufs_iclSetp;

/* 
 * This structure describes the UFS-specific part of aggregate data
 */
struct ufs_agData {
    struct osi_vfs *vfsp;		/* vfs of mounted volume */
    afs_hyper_t volId;		/* Id of 1st & only volume */
    struct vnode *rootvp;		/* vnode for the root directory */
    long spares[8];			/* We'll use them someday */
};

/*
 * Macros that directly access these fields (given the aggregate)
 */
#define AGRTOUFS(aggrp)			((struct ufs_agData *)(aggrp)->a_fsDatap)
#define UFS_AGTOVFSP(aggrp)		(AGRTOUFS(aggrp)->vfsp)
#define UFS_AGTOVOLID(aggrp)		(AGRTOUFS(aggrp)->volId)


/*
 * This structure describes the UFS-specific part of volume data
 */
struct ufs_volData {
    struct vol_stat_dy ufsData;
    long maxInodeIx;
};

/*
 * Macros that directly access these fields (given the volume)
 */
#define VOLTOUFS(volp)			((struct ufs_volData *)(volp)->v_fsDatap)
#define UFS_VOLTODATA(volp)		VOLTOUFS(volp)->ufsData


/*
 * Mapping of the afsFid for a UFS type volume
 */
struct ufs_afsFid {
#ifdef AFS_OSF_ENV
    u_short	ufid_len;	        /* length of structure */
    u_short	ufid_pad;	        /* force long alignment */
    ino_t	inode;	                /* file number (ino) */
    long        igen;            	/* generation number */
#else
    u_short	volume;			/* XXX ignored (was long!) XXX */
    ino_t	inode;			/* matches afs's vnode */
    long	igen;			/* matches afs's unique */
#endif /* AFS_OSF_ENV */
};

/*
 * Classify volops into read only, read-write and quiesce
 */
/* This is for UFS, which doesn't require synchronization for the same things
 * as more elaborate file systems such as Episode.  Therefore, we don't have
 * to shut down vnode ops for as many operations.
 */
#define VOL_ALLOK_OPS (VOL_OP_GETSTATUS | VOL_OP_SETSTATUS | \
			VOL_OP_SYNC | VOL_OP_PUSHSTATUS)
#define VOL_READONLY_OPS  (VOL_ALLOK_OPS | VOL_OP_GETATTR | \
			VOL_OP_GETACL | VOL_OP_SEEK | \
			VOL_OP_READ | VOL_OP_READHOLE | \
			VOL_OP_TELL | VOL_OP_SCAN | \
			VOL_OP_READDIR | VOL_OP_GETNEXTHOLES)
/* Detect a restore operation */
#define VOL_SETINCON_OPS (VOL_OP_CREATE | VOL_OP_WRITE | VOL_OP_TRUNCATE | \
			  VOL_OP_DELETE | VOL_OP_SETATTR | VOL_OP_SETACL | \
			  VOL_OP_COPYACL)
/* Ops that will require flushing the a directory name-cache */
#define VOL_DIRFLUSH_OPS (VOL_OP_CREATE | VOL_OP_WRITE | VOL_OP_TRUNCATE | \
			  VOL_OP_DELETE | VOL_OP_SETATTR | VOL_OP_SETACL | \
			  VOL_OP_COPYACL | VOL_OP_DEPLETE | VOL_OP_APPENDDIR)


/*
 * Some general UFS macros (which should be moved to XXX/ufs_mach.h, and
 * which are already moved for SUNOS5)
 */
#ifndef AFS_SUNOS5_ENV
#define	UFSROOTINO		2	/* formal name is "ROOTINO" */
#endif /* AFS_SUNOS5_ENV */


#ifdef EXIST_MACH_VOLOPS
#include <ufs_mach.h>
extern int vol_ufsOpen_mach(struct volume *, int, int, struct vol_handle *);
extern int vol_ufsScan_mach(struct osi_vfs *, int, struct vol_handle *);
extern int vol_ufsCreate_mach(struct osi_vfs *, int, struct xvfs_attr *,
			      struct vol_handle *, osi_cred_t *);
extern int vol_ufsDelete_mach(struct osi_vfs *, struct afsFid *, osi_cred_t *);
extern int vol_ufsSetattr_mach(struct vnode *, struct xvfs_attr *,
			       osi_cred_t *);
extern int vol_ufsVget_mach(struct osi_vfs *, struct afsFid *, struct vnode **);
extern int vol_ufsReaddir_mach(struct vnode *, int, char *, osi_cred_t *,
			       afs_hyper_t *, int *);
extern int vol_ufsAppenddir_mach(struct vnode *, u_int, u_int, char *, int,
				 osi_cred_t *);
extern int vol_ufsGetNextHoles_mach(struct vnode *, struct vol_NextHole *,
				    osi_cred_t *);
extern int vol_rdwr_mach(struct vnode *, enum uio_rw, afs_hyper_t, int,
			 char *, osi_cred_t *, int *);
#endif /* EXIST_MACH_VOLOPS */

#endif	/* TRANSARC_UFS_H */
