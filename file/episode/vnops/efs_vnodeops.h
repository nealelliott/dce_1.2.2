/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* Copyright (C) 1996, 1993 Transarc Corporation - All rights reserved. */
/*
 * $Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/vnops/RCS/efs_vnodeops.h,v 1.27 1996/03/04 16:10:26 cfe Exp $
 */

/*
 * Declarations for Episode vnode operations
 */

#ifndef _TRANSARC_EFS_VNODEOPS_H
#define _TRANSARC_EFS_VNODEOPS_H

#include <dcedfs/osi.h>
#include <dcedfs/osi_kvnode.h>
#include <dcedfs/osi_vmm.h>
#include <dcedfs/osi_cred.h>

/*
 * The basic functions
 */
extern int efs_null();
extern int efs_invalid();
extern int efs_nosys();
extern void efs_void_nosys();
extern int efs_panic();
extern int efs_rdwr();
extern int efs_getattr();
extern int efs_setattr();
extern int efs_access();
extern int efs_lookup();
extern int efs_create();
extern int efs_remove();
extern int efs_link();
extern int efs_rename();
extern int efs_mkdir();
extern int efs_rmdir();
extern int efs_readdir();
extern int efs_symlink();
extern int efs_readlink();
extern int efs_fsync();
extern int efs_inactive();
extern int efs_inactive_work(struct vnode *vp, osi_cred_t *cred);
extern int efs_fid();
extern int efs_getlength(struct vnode *vp, long *lenp, osi_cred_t *cred);

#if defined (AFS_HPUX_ENV)
int efs_hpstrategy();
int efs_hpustrategy();
#endif

/*
 * bmap, bread, brelse, strategy -- ancient (pre-VM) VFS functions
 */
#if	!defined(AFS_SUNOS5_ENV) && !defined(AFS_AIX_ENV)
extern int efs_bmap();
extern int efs_bread();
extern int efs_brelse();
#endif	/* AFS_SUNOS5_ENV, AFS_AIX_ENV */
#if	defined(AFS_AIX31_VM) && defined(KERNEL)
extern int efs_vmstrategy();
#endif	/* AFS_AIX31_VM */

/*
 * HP/UX VM functions
 */
#ifdef AFS_HPUX_ENV
extern int vfs_prefill();
extern int efs_pagein();
extern int efs_pageout();
#endif /* AFS_HPUX_ENV */

/*
 * File/record locking
 */
#ifdef AFS_SUNOS5_ENV
extern int efs_frlock(
    struct vnode *vp,
    int cmd,
    struct flock *flockp,
    int flag,
    offset_t offset,
    osi_cred_t *credp);
#else
extern int efs_lockctl();
#endif

/*
 * Episode ACL's
 */
extern int efsx_getacl();
extern int efsx_setacl();

/*
 * Functions that are X-ops, but couldn't be N-ops.
 */
#ifndef	AFS_NOGLUE
extern int efs_getxattr();
extern int efs_setxattr();
extern int efs_afsfid();
extern int efs_getvolume();
#endif	/* !AFS_NOGLUE */


/*
 * AIX 3
 */
#ifdef AFS_AIX_ENV
extern int efs_hold();
extern int efs_rele();
extern int efs_map();
extern int efs_unmap();
#endif

/*
 * OSF/1
 */
#ifdef AFS_OSF_ENV
extern int efs_reclaim();
#endif

/*
 * SunOS 5
 */
#ifdef AFS_SUNOS5_ENV
extern int efs_vmread(
    struct vnode *vp,
    struct uio *uiop,
    int ioflag,
    osi_cred_t *credp);
extern int efs_vmwrite(
    struct vnode *vp,
    struct uio *uiop,
    int ioflag,
    osi_cred_t *credp);
extern void efs_rwlock(struct vnode *vp, int write_lock);
extern void efs_rwunlock(struct vnode *vp, int write_lock);
extern int efs_seek(struct vnode *vp, offset_t ooff, offset_t *noffp);
extern int efs_space(
    struct vnode *vp,
    int cmd,
    struct flock *flockp,
    int flag,
    offset_t offset,
    osi_cred_t *credp);
extern int efs_getpage(
    struct vnode *vp,
    offset_t off,
    u_int len,
    u_int *protp,
    page_t *pl[],
    u_int plsz,
    struct seg *seg,
    caddr_t addr,
    enum seg_rw rw,
    osi_cred_t *credp);
extern int efs_putpage(
    struct vnode *vp,
    offset_t off,
    u_int len,
    int flags,
    osi_cred_t *credp);
extern int efs_putapage(
    struct vnode *vp,
    page_t *pp,
    u_int *offp,
    u_int *lenp,
    int flags,
    osi_cred_t *credp);
/*
 * XXX Since struct xvfs_xops doesn't specify prototypes and since
 * efs_addmap and efs_map have parameters of type u_char, we must _not_
 * have a prototype in scope in order to get correct behavior.  We
 * should probably change the u_char's to u_int's (and add prototypes
 * to xvfs_xops).
 */
extern int efs_addmap(
    struct vnode *vp,
    offset_t off,
    struct as *as,
    caddr_t addr,
    u_int len,
    u_char prot,
    u_char maxprot,
    u_int flags,
    osi_cred_t *credp
);
extern int efs_delmap(
    struct vnode *vp,
    offset_t off,
    struct as *as,
    caddr_t addr,
    u_int len,
    u_int prot,
    u_int maxprot,
    u_int flags,
    osi_cred_t *credp);
extern int efs_map(
    struct vnode *vp,
    offset_t off,
    struct as *as,
    caddr_t *addrp,
    u_int len,
    u_char prot,
    u_char maxprot,
    u_int flags,
    osi_cred_t *credp
);
extern int efs_pageio(
    struct vnode *vp,
    page_t *pp,
    u_int off,
    u_int len,
    int flags,
    osi_cred_t *credp);
extern int efs_getsecattr(
    struct vnode *vp,
    vsecattr_t *vsecattr,
    int flag,
    osi_cred_t *credp);
#endif /* AFS_SUNOS5_ENV */

#endif /* !_TRANSARC_EFS_VNODEOPS_H */
