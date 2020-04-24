/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* Copyright (C) 1996, 1994 Transarc Corporation - All rights reserved. */
/*
 * HISTORY
 * $Log: efs_mount.c,v $
 * Revision 1.1.11.1  1996/10/02  17:43:18  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:35:31  damon]
 *
 * $EndLog$
 */

#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/debug.h>		/* dmprintf (for EFS_TRACE_VNOPS) */
#include <dcedfs/lock.h>
#include <dcedfs/volume.h>
#include <dcedfs/volreg.h>
#include <dcedfs/episode/anode.h>	/* epi_anode_t, epia_XXX functions */
#include <dcedfs/episode/fid.h>		/* epi_volFileId_t */

#include <sys/mount.h>
#include <dcedfs/osi_kvnode.h>

#include <efs_evnode.h>
#include <efs_misc.h>
#include <efs_volvnops.h>
#include <efs_vfsops.h>
#include <efsmount.h>

RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/episode/vnops/RIOS/efs_mount.c,v 1.1.11.1 1996/10/02 17:43:18 damon Exp $")

extern int epi_Initted;

static int GetRoot (struct osi_vfs *vfsp, struct vnode **rootvpp)
{
    epi_volFileId_t fid;                /* file ID */
    struct evnode *evp;			/* evnode */
    int code;

    epid_MakeVolumeRoot (&fid);
    code = vnm_FindVnode (VFSTOV (vfsp), &fid, &evp);
    *rootvpp = EVTOV(evp);
    return code;
}

/*
 * AIX implementation of efs_mount
 */

EXPORT int efs_mount(vfsp)
  struct osi_vfs *vfsp;			/* mounted virtual file system */
{
    struct efs_mount_data mountData;	/* Episode-specific mount args */
    struct afsFid fid;			/* for volreg_Lookup of fset id */
    struct vnode *stubvp;		/* stub vnode */
    struct vnode *rootvp;		/* root vnode of mounted fs */
    dev_t dev;				/* Unix major/minor device #s */
    int vid;				/* low word of volume Id */
    long code;				/* error return code */
    struct volume *volp;
    afs_hyper_t volId;
    char *spec;

    if (!epi_Initted)
	return EIO;

    /* Check for super-user */
    if (!osi_suser(credp))
	return EPERM;

    spec = vmt2dataptr(vfsp->vfs_mdata, VMT_OBJECT);
    stubvp = vfsp->vfs_mntdover;
    bcopy (vmt2dataptr(vfsp->vfs_mdata, VMT_INFO),
	   &mountData, sizeof mountData);

    /* Get device special file */
    AFS_hzero(volId);
    if (mountData.flags & EFS_MOUNT_DATA_FLAG_USE_ID) {
	/* Use the ID passed in the mount data */
	fid.Volume = mountData.volId;
	code = volreg_Lookup(&fid, &volp);
    } else {
	/* Ignore the mount data; spec contains the fileset name */
	code = volreg_LookupByName(spec, &volp);
    }
    if (code)
	return (code);

    lock_ObtainWrite(&volp->v_lock);
    VOLID_SET (volId, volp->v_volId);
    /*
     * Check that this volume is not targeted for destruction
     */
    if (volp->v_states & (VOL_DEADMEAT | VOL_DELONSALVAGE | VOL_LCLMOUNT))
	code = EBUSY;
    else if (VTOEVLD(volp)->vld_inconVolume)
	code = EIO;

    if (!code) {
	afsl_Assert (!VTOEVLD(volp)->vld_vfs);
	volp->v_states |= VOL_LCLMOUNT;
	VTOEVLD(volp)->vld_vfs = vfsp;

	/* need to fill this in before we can get root vnode */
	vfsp->vfs_data = (opaque)volp;	/* don't release volp after here */
	vfsp->vfs_bsize = epia_AnodeDevice(VTOAH(volp))->blkSize;
	dev = VTODEVVP(volp)->v_rdev;
    }

    lock_ReleaseWrite(&volp->v_lock);
    if (code)
	goto out;

    /*
     * FS ID's --
     *     We cannot just use the device (as other physical file systems do),
     *     because the FS ID has to be unique, and Episode allows multiple
     *     FS'es per device.  We cram the device minor, device major, and low
     *     16 bits of the volume ID into 32 bits, and cross our fingers.
     */

    vid = AFS_hgetlo(volId);

    vfsp->vfs_fsid.val[0] = (vid << 16)
				+ ((major(dev) & 0xff) << 8)
				+ (minor(dev) & 0xff);
    vfsp->vfs_fsid.val[1] = MOUNT_EFS;

    code = GetRoot (vfsp, &rootvp);
    if (!code) {
	stubvp->v_mvfsp = vfsp;
	vfsp->vfs_flag |= VFS_DEVMOUNT;

	lock_ObtainWrite(&volp->v_lock);
	afsl_Assert (!VLD_ROOT(vfsp));
	VLD_ROOT(vfsp) = rootvp;	/* Make sure vfsp knows the root */
	VTOEVLD(volp)->vld_syncMeta = 
	    ((mountData.flags & EFS_MOUNT_DATA_FLAG_NFS) != 0);
	lock_ObtainWrite(&volp->v_paggrp->a_lock);
	volp->v_paggrp->a_localMounts++;
	lock_ReleaseWrite(&volp->v_paggrp->a_lock);
	lock_ReleaseWrite(&volp->v_lock);
    } else {
	/* We assume cleanup in the vfsp will be done in caller. But just
	 * to be safe, we clear the private data pointer */
	vfsp->vfs_data = NULL; 
	lock_ObtainWrite(&volp->v_lock);
	volp->v_states &= ~VOL_LCLMOUNT;
	VTOEVLD(volp)->vld_vfs = NULL;
	lock_ReleaseWrite(&volp->v_lock);
    }
    vnm_SetVfsP (volp);		/* set vfsp of all existing vnodes on 
				 * success, unset on failure */
out:
    if (code)
	VOL_RELE(volp);
    return (code);
}

/*
 * efs_unmount -- unmount a file system
 *
 * This is the AIX 3.1 version.  A flag in the vfs tells whether to
 * unmount a busy file system.  The flag function argument is ignored.
 */
/* EXPORT */
int efs_unmount (vfsp, ignored_flag)
    struct vfs *vfsp;
    int ignored_flag;
{
    struct volume *volp;		/* a volp use in looking up the aggr */
    struct vnode *rootvp;
    int inuse;				/* nonzero if there are vnodes
					 * in the file system */
    int force = (vfsp->vfs_flag & VFS_SHUTDOWN);

    volp = VFSTOV(vfsp);
    rootvp = VLD_ROOT(vfsp);
    afsl_Assert (rootvp);

    inuse = vnm_Inuse (volp);
    if (inuse > 1) {
	vnm_QuiesceVol(volp, QV_RECYCLE);
	inuse = vnm_Inuse (volp);
	if (inuse > 1) {
	    return EBUSY;
	}
    }

    afsl_Assert (inuse == 1);		/* root should be mounted */
    OSI_VN_RELE (rootvp);
    VLD_ROOT(vfsp) = NULL;

    lock_ObtainWrite(&volp->v_lock);
    volp->v_states &= ~VOL_LCLMOUNT;
    VTOEVLD(volp)->vld_vfs = NULL;
    VTOEVLD(volp)->vld_syncMeta = 0;
    vfsp->vfs_data = NULL;

    lock_ObtainWrite(&volp->v_paggrp->a_lock);
    osi_assert(volp->v_paggrp->a_localMounts > 0);
    volp->v_paggrp->a_localMounts--;
    lock_ReleaseWrite(&volp->v_paggrp->a_lock);

    lock_ReleaseWrite(&volp->v_lock);

    vnm_SetVfsP (volp);			/* unset vfsp of all existing vnodes */

    VOL_RELE (volp);
    return (0);
}
