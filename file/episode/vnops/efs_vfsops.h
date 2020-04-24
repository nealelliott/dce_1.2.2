/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: efs_vfsops.h,v $
 * Revision 1.1.26.1  1996/10/02  17:44:33  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:36:11  damon]
 *
 * $EndLog$
 */
/* Copyright (C) 1996, 1993 Transarc Corporation - All rights reserved. */

/*
 * $Header: /u0/rcs_trees/dce/rcs/file/episode/vnops/efs_vfsops.h,v 1.1.26.1 1996/10/02 17:44:33 damon Exp $
 */

/*
 * Declarations for Episode vfs operations
 */

#ifndef _TRANSARC_EFS_VFSOPS_H
#define _TRANSARC_EFS_VFSOPS_H

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/sysincludes.h>

#ifdef AFS_AIX_ENV
extern int efs_mount(struct osi_vfs *vfsp);
#elif AFS_SUNOS5_ENV
extern int efs_mount(
  struct vfs *vfsp,
  struct vnode *mvp,
  struct mounta *uap,
  osi_cred_t *credp);
#endif

extern int efs_root(struct osi_vfs *vfsp, struct vnode **vpp);

#ifdef AFS_SUNOS5_ENV
extern int efs_unmount(struct osi_vfs *vfsp, osi_cred_t *credp);
#else
extern int efs_unmount(struct vfs *vfsp, int flag);
#endif

#ifndef AFS_OSF_ENV
extern int efs_statfs(struct osi_vfs *vfsp, osi_statfs_t *sbP);
#endif

#ifdef AFS_SUNOS53_ENV
extern int efs_sync(struct osi_vfs *, short, osi_cred_t *);
#else /* AFS_SUNOS53_ENV */
extern int efs_sync();
#endif /* AFS_SUNOS53_ENV */

extern int efs_vget(
    struct osi_vfs *vfsp,
    struct vnode **vpp,
    struct fid *fidP
);
extern int efs_fscntl();

#ifdef	AFS_SUNOS5_ENV
int efs_mountroot();
int efs_swapvp();
#endif	/* AFS_SUNOS5_ENV */

#ifdef AFS_OSF_ENV
int efs_start();
int efs_quotactl();
int efs_fhtovp();
int efs_vptofh();
int efs_init();
#endif /* AFS_OSF_ENV */

#ifdef AFS_SUNOS53_ENV
#ifndef _KERNEL
/* Need some declarations in user space as well, for simulation. */
extern struct vfssw vfssw[];
extern vnode_t *specvp(struct vnode *, dev_t, vtype_t, struct cred *);
extern vnode_t *makespecvp(dev_t, vtype_t);
#endif /* not _KERNEL */
#endif /* AFS_SUNOS53_ENV */

#ifdef AFS_HPUX_ENV
extern int efs_getmount(); /* see the hp-ux specific code */
#endif /* AFS_HPUX_ENV */

#if OLD_VNSYNC
/* SHARED */
extern int InitEM(
    afs_hyper_t volId,			/* volume ID */
    char *volName,			/* volume name */
    dev_t dev,				/* device number */
    struct efsMOUNT **empP,		/* where to put mount table pointer */
    struct vfs *vfsp			/* virtual file system */
);
#endif

#endif /* _TRANSARC_EFS_VFSOPS_H */
