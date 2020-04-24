/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: efs_mount.c,v $
 * Revision 1.1.95.1  1996/10/02  17:42:58  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:35:20  damon]
 *
 * $EndLog$
 */
/* Copyright (C) 1996, 1992 Transarc Corporation - All rights reserved. */

#include <dcedfs/param.h>
#include <dcedfs/osi.h>
#include <dcedfs/osi_user.h>
#include <dcedfs/osi_kvnode.h>

#include <dcedfs/stds.h>
#include <dcedfs/debug.h>		/* dmprintf (for EFS_TRACE_VNOPS) */
#include <dcedfs/lock.h>		/* struct lock_data */
#include <dcedfs/queue.h>		/* struct squeue */
#include <dcedfs/xvfs_vnode.h>		/* struct xvfs_vfsops */

#include <dcedfs/episode/async.h>	/* struct asevent */
#include <dcedfs/episode/boot.h>	/* SuperBlock and EPISODE_BLOCKSIZE  */
#include <dcedfs/episode/volume.h>	/* epi_volume_t, epiv_XXX functions */
#include <dcedfs/episode/aggregate.h>	/* epig_XXX functions */
#include <dcedfs/episode/anode.h>	/* epi_anode_t, epia_XXX functions */
#include <dcedfs/episode/fid.h>		/* epi_volFileId_t */
#include <efs_evnode.h>			/* struct evnode */
#include <efsmount.h>			/* struct efsMOUNT */

#include <sys/inode.h>


RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/episode/vnops/HP800/efs_mount.c,v 1.1.95.1 1996/10/02 17:42:58 damon Exp $")

extern int epi_Initted;

/*
 * HP-UX version of efs_mount
 */

EXPORT int efs_mount(vfsp, pathname, private_data)
  struct osi_vfs *vfsp;
  char *pathname;
  caddr_t private_data;
  {
    struct efsMOUNT *newMp;		/* mount table entries */
    struct efs_args efsArgs;		/* kernel space copy of private data */
    struct vnode *bdevp;		/* block dev vnode */
    struct vnode *rootvp;		/* root of mounted fs */
    dev_t dev;				/* Unix major/minor device #s */
    int vid;				/* low word of vol Id */
    long code;				/* error return code */
    int rdonly = vfsp->vfs_flag & VFS_RDONLY;
    int flag = (rdonly ? FREAD : FREAD | FWRITE);
    site_t site;
#ifdef GETMOUNT
    int pathLen, devnameLen, ftnameLen;
    struct epiv_ident volIdent;
#endif /* GETMOUNT */

    if (!epi_Initted)
	return EIO;

    code = copyin(private_data, (caddr_t)&efsArgs, sizeof (struct efs_args));
    if (code) { 
        return (code);
    }

    /* 
     * Get the dev number of efsArgs.fspec. 
     * NOTE: efsArgs.fspec is special device corresponding to the aggregate.
     */
    code = getmdev(efsArgs.fspec, &dev, &site);

    if (code) {
        return (code);
    }
    
    /*
     * get a vnode for the block device.  We then use this to open the
     * device & later to close it (also used in efs_bmap).
     */
    bdevp = devtovp(dev);

    /*
     * We must open the device by calling the driver on HP-UX.  The
     * interface to this call depends upon the compilation flags.  The
     * ufs code I stole from notes that this really should become a vnode
     * operation (like VOP_OPEN, maybe?)
     */
#ifdef FULLDUX
#ifdef HPNSE
    code = opend (&dev,
		  (vfsp->vfs_flag & VFS_MI_DEV) ? IFBLK|IF_MI_DEV : IFBLK,
		  site,
		  (vfsp->vfs_flag & VFS_RDONLY) ? FREAD : FREAD|FWRITE,
		  0, 0, 0);
#else  /* HPNSE */
    code = opend (&dev,
		  (vfsp->vfs_flag & VFS_MI_DEV) ? IFBLK|IF_MI_DEV : IFBLK,
		  site,
		  (vfsp->vfs_flag & VFS_RDONLY) ? FREAD : FREAD|FWRITE,
		  0);
#endif /* HPNSE */
#else  /* FULLDUX */
#ifdef HPNSE
    code = opend (&dev,
		  (vfsp->vfs_flag & VFS_MI_DEV) ? IFBLK|IF_MI_DEV : IFBLK,
		  (vfsp->vfs_flag & VFS_RDONLY) ? FREAD : FREAD|FWRITE,
		  0, 0, 0);
#else  /* HPNSE */
    code = opend (&dev,
		  (vfsp->vfs_flag & VFS_MI_DEV) ? IFBLK|IF_MI_DEV : IFBLK,
		  (vfsp->vfs_flag & VFS_RDONLY) ? FREAD : FREAD|FWRITE,
		  0);
#endif /* HPNSE */
#endif /* FULLDUX */

    if (code) {
	goto out3a;
    }

    /*
     * Initialize the episode mount information
     */
    code = InitEM(efsArgs. volId, efsArgs.volName, dev, &newMp, vfsp);
    if (code) {
	goto out3;
    }

    /*
     * attach our mount data to the vfs pointer.
     */
    vfsp->vfs_data = (caddr_t)newMp;
    vfsp->vfs_bsize = newMp->deviceP->blkSize;

    /*
     * FS ID's:
     *	We cannot just use the device (as other physical file systems do),
     *  because the FS ID has to be unique, and Episode allows multiple FS'es
     *  per device.  We cram the device minor, device major, and low 16 bits
     *  of the volume ID into 32 bits, and cross our fingers.
     */

    vid = AFS_hgetlo(newMp->volId);

    vfsp->vfs_fsid[0] = (vid << 16) + (long)dev;
    vfsp->vfs_fsid[1] = vfsp->vfs_mtype;

    /*
     * Finish setting up the episode file system data area.
     */
    newMp->m_vfsp = vfsp; /* back ptr */
    newMp->m_devvp = bdevp; /* block device vnode */

    /*
     * This code checks to make sure the mostly mounted file system was OK
     * on mount; if we can get the root vnode we assume it is good enough
     * to complete the mount operation.
     */
    code = efs_root (vfsp, &rootvp);
    if (code)
	goto out3;
    VN_RELE(rootvp);

#ifdef GETMOUNT
    /*
     * Record mount pathname
     */
    pathLen = strlen (pathname);
    newMp->mntpath = (char *) osi_Alloc (pathLen + 1);
    bcopy (pathname, newMp->mntpath, pathLen + 1);
    newMp->mntpathlen = pathLen + 1;
    /*
     * Set vfs_name.  We append the fileset name to the special device name.
     */
    devnameLen = strlen (efsArgs.fspec);
    bcopy (efsArgs.fspec, vfsp->vfs_name, devnameLen+1);
    epiv_GetIdent (newMp->volHandle, &volIdent);
    ftnameLen = strlen (volIdent.name);
    if (devnameLen + ftnameLen + 2 < MAXPATHLEN) {
	char *cp = &vfsp->vfs_name[devnameLen];
	*cp++ = ':';
	bcopy (volIdent.name, cp, ftnameLen+1);
    }
#endif /* GETMOUNT */

    return(0);

    /*
     * error recovery/cleanup
     */
  out3:
#ifdef FULLDUX
    closed(dev, IFBLK, flag, my_site);
#else
    closed(dev, IFBLK, flag);
#endif
  out3a:
    VN_RELE(bdevp);
    return (code);
}

/*
 * efs_unmount -- unmount a file system if it is not busy
 *
 * This is the HP/UX version.  There is no provision for forcible
 * unmount.
 */
/* EXPORT */
int efs_unmount(struct osi_vfs *vfsp)
{
    int code;

#ifdef GETMOUNT
    struct efsMOUNT *mp = VFSTOEFS (vfsp);
    char *p = mp->mntpath;
    int pl = mp->mntpathlen;
#endif /* GETMOUNT */

    code = efs_unmount_subr (vfsp, 0, NULL);

#ifdef GETMOUNT
    if (code == 0)
	osi_Free (p, pl);
#endif /* GETMOUNT */

    return (code);
}
/*
 * efs_unmount_subr -- unmount a file system, common subroutine
 *
 * If force is set, the file system is unmounted even if it is busy.
 */

static int efs_unmount_subr(
    struct osi_vfs *vfsp,		/* mounted file system */
    int force,
    osi_cred_t *credp)
{
    struct efsMOUNT *emp;		/* mount table entry */
    struct volume *volp;		/* a volp use in looking up the aggr */
    int inuse;				/* nonzero if there are vnodes
					 * in the file system */
    int rdonly = vfsp->vfs_flag & VFS_RDONLY;
    int flag = (rdonly ? FREAD : FREAD | FWRITE);
    int inuseflags = INUSE_VFSVALID;

    emp = VFSTOEFS (vfsp);
    inuse = vnm_Inuse (vfsp, emp->volHandle, inuseflags);

    if ((inuse & VFSINUSE) && !force)
        return (EBUSY);

    if (!inuse) {
	epig_CloseVolume (elbt_NullTranId, emp->volHandle);
	epig_CloseAggregate (emp->aggHandle);

#ifdef FULLDUX
	closed(emp->m_devvp->v_rdev, IFBLK, flag, my_site);
#else
	closed(emp->m_devvp->v_rdev, IFBLK, flag);
#endif

	OSI_VN_RELE (emp->m_devvp);
    }

    emp->volHandle = 0;			/* invalidate mount table entry */

#ifdef	_KERNEL
    if (efs_vfsgetvolume(vfsp, &volp) != 0)
	return EINVAL;

    lock_ObtainWrite(&volp->v_lock);
    volp->v_states &= ~VOL_LCLMOUNT;
    lock_ObtainWrite(&volp->v_paggrp->a_lock);
    osi_assert(volp->v_paggrp->a_localMounts > 0);
    volp->v_paggrp->a_localMounts--;
    lock_ReleaseWrite(&volp->v_paggrp->a_lock);
    lock_ReleaseWrite(&volp->v_lock);
    VOL_RELE(volp);
#endif	/* _KERNEL */    

    return (0);
}
