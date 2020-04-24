/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * Copyright (C) 1992,1993 Transarc Corporation.
 * All rights reserved.
 */
/*
 * HISTORY
 * $Log: agfs_vnops.c,v $
 * Revision 1.1.19.1  1996/10/02  16:57:40  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:03:38  damon]
 *
 * Revision 1.1.13.2  1994/06/09  13:49:58  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:23:26  annie]
 * 
 * Revision 1.1.13.1  1994/02/04  20:04:16  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:05:43  devsrc]
 * 
 * Revision 1.1.11.1  1993/12/07  17:11:18  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/06  13:33:06  jaffe]
 * 
 * $EndLog$
 */

#include <dcedfs/param.h>
#include <dcedfs/sysincludes.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/xvfs_vnode.h>

RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/agfs/agfs_vnops.c,v 1.1.19.1 1996/10/02 16:57:40 damon Exp $")

extern int	agfs_NoSys();		/* From agfs_vfsops.c */
static int	agfs_NoOp();

#ifdef AFS_SUNOS5_ENV
static void	agfs_NoSys_Void();
static void	agfs_Inactive();
static int	agfs_Getattr();
static int	agfs_Readdir();
#endif /* AFS_SUNOS5_ENV */

#ifdef AFS_AIX31_ENV
static int	agfs_Hold(), agfs_Rele();
static int	agfs_Getattr();
#endif	/* AFS_AIX31_ENV */

#ifdef AFS_HPUX_ENV
static int	agfs_Getattr();
#endif /* AFS_HPUX_ENV */

#ifdef AFS_SUNOS5_ENV
/* SHARED */
struct osi_vnodeops agfs_vnops = {
    agfs_NoOp,			/* open */
    agfs_NoOp,			/* close */
    agfs_NoSys,			/* read */
    agfs_NoSys,			/* write */
    agfs_NoSys,			/* ioctl */
    agfs_NoSys,			/* setfl */
    agfs_Getattr,		/* getattr */
    agfs_NoSys,			/* setattr */
    agfs_NoOp,			/* access */
    agfs_NoSys,			/* lookup */
    agfs_NoSys,			/* create */
    agfs_NoSys,			/* remove */
    agfs_NoSys,			/* link */
    agfs_NoSys,			/* rename */
    agfs_NoSys,			/* mkdir */
    agfs_NoSys,			/* rmdir */
    agfs_Readdir,		/* readdir */
    agfs_NoSys,			/* symlink */
    agfs_NoSys,			/* readlink */
    agfs_NoSys,			/* fsync */
    agfs_Inactive,		/* inactive */
    agfs_NoSys,			/* fid */
    agfs_NoSys_Void,		/* rwlock */
    agfs_NoSys_Void,		/* rwunlock */
    agfs_NoSys,			/* seek */
    agfs_NoSys,			/* cmp */
    agfs_NoSys,			/* frlock */
    agfs_NoSys,			/* space */
    agfs_NoSys,			/* realvp */
    agfs_NoSys,			/* getpage */
    agfs_NoSys,			/* putpage */
    (int	(*)(struct vnode *, offset_t, struct as *, caddr_t *, u_int, u_char,
		    u_char, u_int, struct cred *))agfs_NoSys,  /* map */
    (int	(*)(struct vnode *, offset_t, struct as *, caddr_t, u_int, u_char,
		    u_char, u_int, struct cred *))agfs_NoSys,  /* addmap */
    agfs_NoSys,			/* delmap */
    (int	(*)(struct vnode *, short, int, short *, struct pollhead **))
        agfs_NoSys,			/* poll */
    agfs_NoSys,			/* dump */
    agfs_NoSys,			/* pathconf */
    agfs_NoSys,			/* pageio */
    agfs_NoSys			/* dumpctl */
};
#endif /* AFS_SUNOS5_ENV */
#ifdef AFS_AIX31_ENV
/* SHARED */
struct osi_vnodeops agfs_vnops = {
    agfs_NoSys,				/* link */
    agfs_NoSys,				/* mkdir */
    agfs_NoSys,				/* mknod */
    agfs_NoSys,				/* remove */
    agfs_NoSys,				/* rename */
    agfs_NoSys,				/* rmdir */
    agfs_NoSys,				/* lookup */
    agfs_NoSys,				/* fid */
    agfs_NoOp,				/* open */
    agfs_NoSys,				/* create */
    agfs_Hold,				/* hold */
    agfs_Rele,				/* rele */
    agfs_NoOp,				/* close */
    agfs_NoSys,				/* map */
    agfs_NoSys,				/* unmap */
    agfs_NoOp,				/* access */
    agfs_Getattr,			/* getattr */
    agfs_NoSys,				/* setattr */
    agfs_NoSys,				/* fclear */
    agfs_NoSys,				/* fsync */
    agfs_NoSys,				/* ftrunc */
    agfs_NoSys,				/* rdwr */
    agfs_NoSys,				/* lockctl */
    agfs_NoSys,				/* ioctl */
    agfs_NoSys,				/* readlink */
    agfs_NoSys,				/* select */
    agfs_NoSys,				/* symlink */
    agfs_NoOp,				/* readdir */
    agfs_NoSys,				/* strategy */
    agfs_NoSys,				/* revoke */
    agfs_NoSys,				/* getacl */
    agfs_NoSys,				/* setacl */
    agfs_NoSys,				/* getpcl */
    agfs_NoSys				/* setpcl */
};
#else
#ifdef AFS_OSF_ENV
/* SHARED */
struct osi_vnodeops agfs_vnops = {
    agfs_NoSys,				/* lookup */
    agfs_NoSys,				/* create */
    agfs_NoSys,				/* mknod */
    agfs_NoSys,				/* open */
    agfs_NoSys,				/* close */
    agfs_NoSys,				/* access */
    agfs_NoSys,				/* getattr */
    agfs_NoSys,				/* setattr */
    agfs_NoSys,				/* read */
    agfs_NoSys,				/* write */
    agfs_NoSys,				/* ioctl */
    agfs_NoSys,				/* select */
    agfs_NoSys,				/* mmap */
    agfs_NoSys,				/* fsync */
    agfs_NoSys,				/* seek */
    agfs_NoSys,				/* remove */
    agfs_NoSys,				/* link */
    agfs_NoSys,				/* rename */
    agfs_NoSys,				/* mkdir */
    agfs_NoSys,				/* rmdir */
    agfs_NoSys,				/* symlink */
    agfs_NoSys,				/* readdir */
    agfs_NoSys,				/* readlink */
    agfs_NoSys,				/* abortop */
    agfs_NoSys,				/* inactive */
    agfs_NoSys,				/* reclaim */
    agfs_NoSys,				/* bmap */
    agfs_NoSys,				/* strategy */
    agfs_NoSys,				/* print */
    agfs_NoSys,				/* pgrd */
    agfs_NoSys,				/* pgwr */
    agfs_NoSys,				/* pgwr */
    agfs_NoSys,				/* pgwr */
    agfs_NoSys,				/* pgwr */
    agfs_NoSys,				/* pgwr */
    agfs_NoSys,				/* pgwr */
    agfs_NoSys,				/* pgwr */
};
#else	/* AFS_OSF_ENV */
#ifdef	AFS_HPUX_ENV
SHARED struct osi_vnodeops agfs_vnops = {
    agfs_NoOp,				/* open */
    agfs_NoOp,				/* close */
    agfs_NoSys,				/* rdwr */
    agfs_NoSys,				/* ioctl */
    agfs_NoSys,				/* select */
    agfs_Getattr,			/* getattr */
    agfs_NoSys,				/* setattr */
    agfs_NoOp,				/* access */
    agfs_NoSys,				/* lookup */
    agfs_NoSys,				/* create */
    agfs_NoSys,				/* remove */
    agfs_NoSys,				/* link */
    agfs_NoSys,				/* rename */
    agfs_NoSys,				/* mkdir */
    agfs_NoSys,				/* rmdir */
    agfs_NoOp,				/* readdir */
    agfs_NoSys,				/* symlink */
    agfs_NoSys,				/* readlink */
    agfs_NoSys,				/* fsync */
    agfs_NoOp,				/* inactive */
    agfs_NoSys,				/* bmap */
    agfs_NoSys,				/* strategy */
    agfs_NoSys,				/* bread */
    agfs_NoSys,				/* brelse */
    agfs_NoSys,				/* pathsend */
#ifdef ACLS
    agfs_NoSys,				/* setacl */
    agfs_NoSys,				/* getacl */
#endif
#ifdef POSIX
    agfs_NoSys,				/* pathconf */
    agfs_NoSys,				/* fpathconf */
#endif
    agfs_NoSys,				/* lockctl */
    agfs_NoSys,				/* lockf */
    agfs_NoSys,				/* fid */
    agfs_NoSys,				/* fscntl */
    agfs_NoSys,				/* prefill */
    agfs_NoSys,				/* pagein */
    agfs_NoSys,				/* pageout */
    NULL,				/* dbddup */
    NULL,				/* dbddealloc */
};
#endif	/* AFS_HPUX_ENV */
#endif	/* AFS_OSF_ENV */
#endif	/* AFS_AIX31_ENV */


static int
agfs_NoOp()
{
    return 0;
}

#ifdef AFS_SUNOS5_ENV
static void
agfs_NoSys_Void()
{
    (void) agfs_NoSys();
}

/* ARGSUSED */
static void
agfs_Inactive(vp, cred)
    struct vnode *vp;
    osi_cred_t *cred;
{
    (void) osi_PreemptionOff();
    osi_lockvp(vp);
    vp->v_count -= 1;
    osi_assert(vp->v_count >= 0);
    osi_unlockvp(vp);
    osi_RestorePreemption(0);
}

static int
agfs_Getattr(vp, vap, flags, cred)
    struct vnode *vp;
    struct vattr *vap;
    int flags;
    osi_cred_t *cred;
{
    bzero((caddr_t)vap, sizeof (*vap));
    vap->va_type = osi_vType(vp);
    vap->va_mode = MAKEIMODE(osi_vType(vp), 0666);
    vap->va_fsid = vp->v_vfsp->vfs_fsid.val[0];
    return 0;
}

static int
agfs_Readdir(dvp, uiop, cred, eofp)
    struct vnode *dvp;
    struct uio *uiop;
    osi_cred_t *cred;
    int *eofp;
{
    if (eofp != NULL)
	*eofp = 1;
    return 0;
}
#endif /* AFS_SUNOS5_ENV */

#ifdef AFS_HPUX_ENV
static int
agfs_Getattr(vp, vap, cred, vsync)
    struct vnode *vp;
    struct vattr *vap;
    osi_cred_t *cred;
    enum vsync vsync;
{
    bzero((caddr_t)vap, sizeof (*vap));
    vap->va_type = osi_vType(vp);
    vap->va_mode = MAKEIMODE(osi_vType(vp), 0666);
    return 0;
}
#endif /* AFS_HPUX_ENV */

#ifdef AFS_AIX31_ENV
static int
agfs_Hold (vp)
    struct vnode*	vp;
{
    vp->v_count++;
    return 0;
}	/* agfs_Hold() */


static int
agfs_Rele(vp)
    struct vnode*	vp;
{
    --vp->v_count;
    return 0;
}	/* agfs_Rele() */

static int
agfs_Getattr(vp, vattrp)
    struct vnode	*vp;
    struct vattr	*vattrp;
{
    bzero((caddr_t)vattrp, sizeof (*vattrp));
    vattrp->va_type = osi_vType(vp);
    vattrp->va_mode = MAKEIMODE(osi_vType(vp), 0666);
    return 0;
}
#endif	/* AFS_AIX31_ENV */
