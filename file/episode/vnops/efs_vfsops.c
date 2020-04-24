/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * Copyright (C) 1995, 1990 Transarc Corporation
 * All rights reserved.
 */
/*			    Episode File System
			 Vfs operations and vector
 */

#include <dcedfs/osi.h>
#include <dcedfs/osi_user.h>
#include <dcedfs/osi_kvnode.h>
/* XXX FIX ME XXX */
#undef PRIVATE
#undef SHARED
#include <dcedfs/osi_device.h>
#include <dcedfs/debug.h>		/* dmprintf (for EFS_TRACE_VNOPS) */
#include <dcedfs/lock.h>		/* struct lock_data */
#include <dcedfs/queue.h>		/* struct squeue */
#include <dcedfs/xvfs_vnode.h>		/* struct xvfs_vfsops */
#include <dcedfs/volreg.h>
#include <dcedfs/episode/async.h>	/* struct asevent */
#include <dcedfs/episode/boot.h>	/* SuperBlock and EPISODE_BLOCKSIZE  */
#include <dcedfs/episode/volume.h>	/* epi_volume_t, epiv_XXX functions */
#include <dcedfs/episode/aggregate.h>	/* epig_XXX functions */
#include <dcedfs/episode/anode.h>	/* epi_anode_t, epia_XXX functions */
#include <dcedfs/episode/fid.h>		/* epi_volFileId_t */
#include <efs_evnode.h>			/* struct evnode */
#include <efs_volvnops.h>		/* VFSTOV */
#include <efs_misc.h>
#include <efs_vfsops.h>
#ifdef AFS_OSF_ENV
#include <ufs/quota.h>
#include <ufs/ufsmount.h>
#ifdef _KERNEL
#include <sys/specdev.h>		/* struct specinfo for EQUAL_DEV */
#endif /* _KERNEL */
#include <dcedfs/afs4int.h>
#endif /* AFS_OSF_ENV */

#ifdef AFS_AIX31_ENV
#include <sys/statfs.h>
#endif

#ifdef AFS_HPUX_ENV
#include <sys/inode.h>
#endif

RCSID ("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/vnops/RCS/efs_vfsops.c,v 4.189 1995/06/16 21:14:16 andi Exp $")

/* NOTE -- On SunOS5 we do are called directly from the kernel (no glue layer)
 *     and so the preemption_lock is not held on entry.  So, unlike the vnode
 *     operations we can just assume the preemption lock is okay here. */

extern int epi_Initted;

#define DEBUG_THIS_FILE EFS_DEBUG_VFSOPS

/* XXX Temporary until we know where to put these */

#ifndef MOUNT_EFS
#define MOUNT_EFS 10			/* Distinguished int for fsid of vfs */
#endif /* MOUNT_EFS */

#ifdef AFS_OSF_ENV
struct xvfs_vfsops efs_vfsops = {
{
    efs_mount,
    efs_start,
    efs_unmount,
    efs_root,
    efs_quotactl,
    efs_statfs,
    efs_sync,
    efs_fhtovp,
    efs_vptofh,
    efs_init
}
};
static char efs_mntfromname[] = "DCE/LFS: ";
#else
struct xvfs_vfsops efs_vfsops = {
/*
 * Need an extra layer of {}'s here, because struct osi_vfsops is a
 * subfield of struct xvfs_vfsops.
 */
{
	efs_mount,
	efs_unmount,
	efs_root,
	efs_statfs,
	efs_sync,
	efs_vget,
#ifdef	AFS_SUNOS5_ENV
	efs_mountroot,
	efs_swapvp,
#endif	/* AFS_SUNOS5_ENV */
#ifdef	AFS_AIX31_ENV
	efs_fscntl,	/* fscntl */
#endif	/* AFS_AIX31_ENV */
#ifdef AFS_HPUX_ENV
#ifdef GETMOUNT
	efs_getmount,
#endif /* GETMOUNT */
#endif /* AFS_HPUX_ENV */
}
};
#endif /* AFS_OSF_ENV */

#ifdef AFS_SUNOS5_ENV
/*
 * efs_mountroot -- mount the root filesystem
 */

/* EXPORT */
int
efs_mountroot(struct vfs *vfsp, enum whymountroot why)
{
    return ENOSYS;			/* Not yet implemented */
}

/*
 * efs_swapvp -- return vnode for swap
 */

/* EXPORT */
int
efs_swapvp(struct vfs *vfsp, struct vnode **vpp, char *nm)
{
    return ENOSYS;			/* Not yet implemented */
}
#endif /* AFS_SUNOS5_ENV */

/*
 * efs_root -- return root vnode of a mounted Episode volume
 */

/* EXPORT */
int efs_root(
    struct osi_vfs *vfsp,		/* vfs pointer */
    struct vnode **vpp)			/* place to put root vnode */
{
    *vpp = VLD_ROOT(vfsp);
    if (*vpp) {
	FAST_VN_HOLD (VTOEV(*vpp));
	return 0;
    } else {
	*vpp = NULL;
	return ENOENT;			/* XXX */
    }
}

/*
 * efs_statfs -- Get the Episode statistics for a given volume
 */

/* EXPORT */
#ifdef AFS_OSF_ENV
int efs_statfs(struct osi_vfs *vfsp)
#else
int efs_statfs(struct osi_vfs *vfsp, osi_statfs_t *sbP)
#endif /* AFS_OSF_ENV */
{
    epi_volume_t volHandle;		/* Volume Handle */
    struct epiv_status volStatus;	/* status of volume */
    u_int32 fsize;			/* Fragmentation size */
#ifdef AFS_OSF_ENV
    osi_statfs_t *sbP = &vfsp->m_stat;
#endif /* AFS_OSF_ENV */
    long code;

    volHandle = VTOVH(VFSTOV(vfsp));
    code = epiv_GetStatus (volHandle, &volStatus);
    if (code) {
	EFS_TRACE_VNOPS(DEBUG_THIS_FILE, code, EIO);
	return EIO;
    }
    fsize = epia_FragmentSize (volHandle);

#ifdef AFS_OSF_ENV
    sbP->f_fsize = vfsp->m_stat.f_bsize;
#elif	defined(AFS_SUNOS5_ENV)
    sbP->f_frsize = fsize;
#else
    sbP->f_bsize = fsize;
#endif

    /* return visible not allocated sizes */
    sbP->f_blocks = volStatus.visible.limit;
    sbP->f_bfree = volStatus.visible.limit - volStatus.visible.usage;
    sbP->f_bavail = sbP->f_bfree;

    /* Episode doesn't have inodes, so f_files and f_free are meaningless */
    sbP->f_files = -1;
    sbP->f_ffree = -1;

#ifdef AFS_OSF_ENV
    /* don't need to update this */
#elif	defined(AFS_SUNOS5_ENV)
    sbP->f_fsid = vfsp->vfs_fsid.val[0];
#elif	defined(AFS_HPUX_ENV)
    sbP->f_fsid[0] = vfsp->vfs_fsid[0];
    sbP->f_fsid[1] = vfsp->vfs_fsid[1];
#else
    sbP->f_fsid = vfsp->vfs_fsid;
#endif

#ifdef AFS_OSF_ENV
    sbP->f_type = MOUNT_EFS;
#ifdef AFS_OSF11_ENV
    sbP->f_linkmax = LINK_MAX;
    sbP->f_namemax = NAME_MAX;
    sbP->f_pathmax = PATH_MAX;
    sbP->f_fsflags = f_notrunc;
    strcpy(sbP->f_vfstype, "efs");
#endif /* AFS_OSF11_ENV */
#endif /* AFS_OSF_ENV */

#ifdef	AFS_AIX31_ENV
    sbP->f_vfstype = MOUNT_EFS;
    sbP->f_vfsnumber = vfsp->vfs_number;
    /* Use volume name as file system name */
    strncpy(sbP->f_fname, volStatus.ident.name, sizeof(sbP->f_fname));
    bzero (sbP->f_fpack, sizeof(sbP->f_fpack)); /* what to put here? */
    sbP->f_name_max = MAXNAMLEN;
#endif	/* AFS_AIX31_ENV */

#ifdef	AFS_SUNOS5_ENV
    sbP->f_bsize = epia_BlockSize (volHandle);
    sbP->f_favail = -1;
    strcpy(sbP->f_basetype, vfssw [vfsp->vfs_fstype].vsw_name);
    sbP->f_flag = vf_to_stf (vfsp->vfs_flag);
    sbP->f_namemax = OSI_MAXNAMLEN;
    ;					/* f_fstr */
#endif	/* AFS_SUNOS5_ENV */

    return (0);
}

/*
 * efs_sync -- sync an Episode file system
 *
 * XXX We ignore the vfsp and always sync all Episode file systems.
 */
static void
DoSync(int sync_flags)
{
    static int lock_initted = 0;
    static osi_dlock_t efs_syncLock;

    if (!epi_Initted)
	return;

    if (!lock_initted) {
	lock_Init(&efs_syncLock);
	lock_initted = 1;
    }

    if (lock_ObtainWriteNoBlock(&efs_syncLock)) {
	vnm_SyncVnodes(sync_flags);
	elbb_SyncForce();
	lock_ReleaseWrite_r(&efs_syncLock);
    }
}

#ifdef AFS_SUNOS5_ENV
int
efs_sync(struct osi_vfs *vfsp, short flag, osi_cred_t *credp)
{
    int sync_flags = (flag == SYNC_ATTR);
#ifdef _KERNEL
    label_t recursive_panic_exit;

    if (osi_IsKernelPanic()) {
	if (setjmp(&recursive_panic_exit) != 0)
	    return 0;	/* bail out on recursive panic */
	osi_SetPanicExit(&recursive_panic_exit);
    }
#endif /* KERNEL */

    DoSync(sync_flags);

    return 0;
}
#else
int
efs_sync(struct osi_vfs *vfsp)
{
#ifdef _KERNEL
    if (osi_IsKernelPanic())	/* avoid deadlock during panic */
	return 0;		/* zeroth order fix:  don't sync at all */
#endif /* _KERNEL */

    DoSync(0);
    return 0;
}
#endif /* AFS_SUNOS5_ENV */

/*
 * efs_vget -- Get an Episode vnode based on a Fid
 */

/* EXPORT */
int efs_vget(
    struct osi_vfs *vfsp,
    struct vnode **vpp,
    struct fid *fidP)
{
    epi_volFileId_t newFidP;
    int code = 0;
    struct evnode *evp;

    bcopy (fidP->fid_data, (caddr_t) &newFidP, sizeof (epi_volFileId_t));

    code = vnm_FindVnode(VFSTOV(vfsp), &newFidP, &evp);
    *vpp = (code) ? NULL : EVTOV(evp);
    if (code) {
	Assert (code == ESTALE);
	code = ESTALE;
    }

    return code;
}

#ifdef AFS_OSF_ENV
/*
 * Make a filesystem operational.
 * Nothing to do at the moment.
 */
efs_start(mp, flags)
	struct osi_vfs *mp;
	int flags;
{

	return (0);
}
/*
 * Do operations associated with quotas
 */
efs_quotactl(mp, cmds, uid, arg)
	struct osi_vfs *mp;
	int cmds;
	uid_t uid;
	caddr_t arg;
{
	return (EOPNOTSUPP);

}

efs_fhtovp(mp, fhp, vpp)
	register struct osi_vfs *mp;
	struct fid *fhp;
	struct vnode **vpp;
{
    epi_volFileId_t newFidP, nbo_fid;
    int code;

    emp = VFSTOEFS(mp);
    bcopy (fhp->fid_data, &newFidP, sizeof (epi_volFileId_t));

    /* convert fid to network byte order */
    epid_Assemble(&nbo_fid, newFidP.index, newFidP.unique);

    code = vnm_FindVnode(&nbo_fid, emp->volHandle, mp, vpp);
    return (code);
}

efs_vptofh(vp, fhp)
	struct vnode *vp;
	struct fid *fhp;
{
    int code;
    struct afsFid myfid;

    code = efs_afsfid(vp, &myfid, 0);
    if (code != 0)
       return(code);
    fhp->fid_len = sizeof(u_long) + sizeof(u_long);
    bcopy(&myfid.Vnode, fhp->fid_data, sizeof(epi_volFileId_t));
    return(0);
}

efs_init(mp)
	register struct osi_vfs *mp;
{
  return(0);
}
#endif /* AFS_OSF_ENV */


/*
 * efs_vfsgetvolume -- get a volume pointer, given a vfs pointer
 */

int efs_vfsgetvolume(
    struct osi_vfs *vfsp,		/* virtual file system */
    struct volume **volpp)		/* where to put volume ptr */
{
    struct volume *volp;

    volp = VFSTOV (vfsp);
    VOL_HOLD(volp);
    *volpp = volp;
    return 0;
}

#ifdef AFS_AIX31_ENV

#ifndef FS_EXTENDFS
#define FS_EXTENDFS 1
#endif

int efs_fscntl(vfsp, cmd, arg, argsz)
  struct vfs *vfsp;			/* virtual file system */
  int cmd;
  int arg;
  int argsz;
{
    u_long blocks;
    epi_anode_t avl;
    int bsize;
    long code;

    if (cmd != FS_EXTENDFS)
	return(EINVAL);

    avl = VTOAH(VFSTOV(vfsp));
    bsize = epia_AnodeDevice(avl)->blkSize;

    /* convert arg from 512b blocks into FS blocks */
    blocks = arg * 512 / bsize;

    /*
     * The exception-setting code merely sets the specified blocks
     * as alocated.  It does not add them to the bad block file, so
     * if the aggregate is salvaged, it will complain.  The intent is
     * the exceptions can be used for alternate superblock handling.
     */
    code = epig_GrowAggregate (avl, blocks, /*nExceptions*/0, 0);

    /*
     * In case of error, there's not a lot we can return here.
     * There is no way to tell if the request is too big.
     * This means that if too many blocks are specified, the
     * machine will panic.  This needs some work.
     */
    if (code) {
	EFS_TRACE_VNOPS(DEBUG_THIS_FILE, code, EINVAL);
	return(EINVAL);
    }

    return(0);
}

#endif /* AFS_AIX31_ENV */
