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
 * Revision 1.1.75.1  1996/10/02  17:43:04  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:35:23  damon]
 *
 * $EndLog$
 */
/* Copyright (C) 1996 Transarc Corporation - All rights reserved. */

#include <dcedfs/param.h>
#include <dcedfs/sysincludes.h>
#include <dcedfs/osi_kvnode.h>

#include <dcedfs/stds.h>
#include <dcedfs/osi.h>			/* osi_Alloc */
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

#include <ufs/quota.h>
#include <ufs/ufsmount.h>
#include <dcedfs/afs4int.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/episode/vnops/OSF1/efs_mount.c,v 1.1.75.1 1996/10/02 17:43:04 damon Exp $")

/*
 * Implementation of OSF/1 efs_mount call
 */

static char efs_mntfromname[] = "DCE/LFS: ";
extern int epi_Initted;

EXPORT int efs_mount(vfsp, pathname, private_data, ndp)
  struct osi_vfs *vfsp;		/* mounted virtual file system */
  char   *pathname;			/* dir name to be mounted on */
  caddr_t private_data;		/* efs epecific data; efs_args */
  struct nameidata *ndp;
  {
      struct efsMOUNT *newMp;		/* mount table entries */
      struct efs_args efsArgs;		/* kernel space copy of private data */
      struct vnode *bdevp;		/* block dev vnode */
      struct vnode *rootvp;		/* root of mounted fs */
      dev_t dev;			/* Unix major/minor device #s */
      int vid;				/* low word of vol Id */
      long code;			/* error return code */
      int  ignore_error;
      u_int size, fsize;
      struct vnode *cvp;
      char *tempstring;
      int rdonly = vfsp->m_flag & M_RDONLY;
      int flag = (rdonly ? FREAD : FREAD | FWRITE);

#ifdef _KERNEL
#if SEC_BASE
	/*
	 * Must have the mount privilege.
	 */
      if (!privileged(SEC_MOUNT, EPERM))
	  return (EPERM);
#else /* SEC_BASE */
      /*
       * Must be super user
       */
      if (!osi_suser(osi_getucred()))
	  return (EPERM);
#endif /* SEC_BASE */
	cvp = ndp->ni_vp;
	BM(MOUNT_LOCK(vfsp));
	if ((vfsp->m_flag & M_UPDATE) == 0) {
	  if (cvp->v_usecount != 1 )
		return(EBUSY);
	  if (cvp->v_type != VDIR)
		return(ENOTDIR);
	} else {
	  /*
	   * Purposely copy only partial arguments since we haven't
	   * verified there's a full efs_args yet.  It could
	   * be the mountd doing a mount for update.
	   */
	  code = copyin(private_data, (caddr_t)&efsArgs,sizeof(partial_args_t));
	  if ((code == 0) && 
	  ((efsArgs.exflags & M_EXPORTED) || (vfsp->m_flag & M_EXPORTED))) {
                if (efsArgs.exflags & M_EXPORTED)
                        vfsp->m_flag |= M_EXPORTED;
                else
                        vfsp->m_flag &= ~M_EXPORTED;
                if (efsArgs.exflags & M_EXRDONLY)
                        vfsp->m_flag |= M_EXRDONLY;
                else
                        vfsp->m_flag &= ~M_EXRDONLY;
                vfsp->m_exroot = efsArgs.exroot;
          }
	  BM(MOUNT_UNLOCK(vfsp));
	  return(code);
	}

	BM(MOUNT_UNLOCK(vfsp));
#endif /* _KERNEL */

    if (!epi_Initted)
	return EIO;

    code = copyin(private_data, (caddr_t)&efsArgs, sizeof (struct efs_args));
    if (code) 
        return (code);
    /* 
     * Get the dev number of efsArgs.fspec. 
     * NOTE: efsArgs.fspec is special device corresponding to the aggregate.
     */
    {
    char pnbuf[MAXPATHLEN];

    code = copyinstr(efsArgs.fspec, pnbuf, MAXPATHLEN, &size);
    if (code)
        return (code);
    code = osi_getmdev(pnbuf, &dev);
    if (code)
        return (code);
    }

    if (code = osi_lookupname(efsArgs.fspec, OSI_UIOUSER,
			      FOLLOW_LINK, (struct vnode *) 0, &bdevp))
	return (code);
#ifdef _KERNEL 
      VOP_OPEN (&bdevp, flag, ndp->ni_cred, code);
#else
      VOP_OPEN (&bdevp, flag, NULL, code);
#endif /* _KERNEL */

      if (code)
	  goto out3a;
      
      code = InitEM (efsArgs.volId, efsArgs.volName, dev, &newMp, vfsp);
      if (code)
	  goto out3;

      vfsp->m_data = (qaddr_t)newMp;
      vfsp->m_stat.f_bsize = newMp->deviceP->blkSize;

      /*
       * FS ID's:
       * We cannot just use the device (as other physical file systems do),
       * because the FS ID has to be unique, and Episode allows multiple FS'es
       * per device.  We cram the device minor, device major, and low 16 bits
       * of the volume ID into 32 bits, and cross our fingers.
       */

      vid = AFS_hgetlo(newMp->volId);
      vfsp->m_stat.f_fsid.val[0] = (vid << 16) + (long)dev;
      vfsp->m_stat.f_fsid.val[1] = MOUNT_EFS;

      if (vfsp->m_vnodecovered->v_type != VDIR) {
	  VN_RELE(bdevp);
	  return(ENOTDIR);
      }
      vfsp->m_flag &= ~M_LOCAL;

#ifdef _KERNEL
      tempstring = osi_Alloc(MAXPATHLEN);
      if (!tempstring)
	  return(ENOMEM);
      (void) copyinstr(pathname, tempstring, MAXPATHLEN - 1, &size);
      vfsp->m_stat.f_mntonname = osi_Alloc(size);
      if (!vfsp->m_stat.f_mntonname) {
	  osi_Free(tempstring, MAXPATHLEN);
	  return(ENOMEM);
      }
      bcopy(tempstring, vfsp->m_stat.f_mntonname, size);
      fsize = sizeof(efs_mntfromname) + strlen(efsArgs.volName);
      vfsp->m_stat.f_mntfromname = osi_Alloc(fsize);
      if (!vfsp->m_stat.f_mntfromname) {
	  osi_Free(tempstring, MAXPATHLEN);
	  osi_Free(vfsp->m_stat.f_mntonname, size);
	  return(ENOMEM);
      }
      bcopy(efs_mntfromname, vfsp->m_stat.f_mntfromname,
	    sizeof(efs_mntfromname));
      bcopy(efsArgs.volName, vfsp->m_stat.f_mntfromname +
	    sizeof(efs_mntfromname), strlen(efsArgs.volName));
      osi_Free(tempstring, MAXPATHLEN);

      vfsp->m_flag |= M_SWAP_NEVER;	/* try to make sure we never swap */
#else
      (void) copyinstr(pathname, vfsp->m_stat.f_mntonname,
		       sizeof(vfsp->m_stat.f_mntonname) - 1, &size);
      bzero(vfsp->m_stat.f_mntonname + size,
	    sizeof(vfsp->m_stat.f_mntonname) - size);
      size = strlen(efs_mntfromname);
      bcopy(efs_mntfromname, vfsp->m_stat.f_mntfromname, size);
      bcopy(efsArgs.volName, vfsp->m_stat.f_mntfromname + size,
	    strlen(efsArgs.volName));
#endif /* !_KERNEL */
      efs_statfs(vfsp);
      
      newMp->m_vfsp = vfsp;
      newMp->m_devvp = bdevp;
      code = efs_root (vfsp, &rootvp);
      if (code)
	  goto out3;
      VN_RELE (rootvp);
      return(0);

out3:
#ifdef _KERNEL
    VOP_CLOSE (bdevp, flag, ndp->ni_cred, ignore_error);
#else
    /*
     * Compiler bug:  if we call VOP_CLOSE at this point, the compiled
     * code calls the wrong function (vn_open instead of vn_close).
     * So we do it by hand.
     */
    /* VOP_CLOSE (bdevp, flag, NULL, ignore_error); */
    ignore_error = (*(bdevp->v_op->vn_close))(bdevp, flag, NULL);
#endif /* _KERNEL */
out3a:
    VN_RELE (bdevp);
    return (code);
}

/*
 * efs_unmount -- unmount a file system
 *
 * This is the OSF/1 version.  OSF/1 has a flags argument which may
 * indicate forcible unmount, but at present this is ignored.
 */
/* EXPORT */
int efs_unmount (vfsp, flag)
    struct osi_vfs *vfsp;		/* mounted file system */
    int flag;
{
#ifdef _KERNEL
#if SEC_BASE
    /*
     * Must have the mount privilege.
     */
    if (!privileged(SEC_MOUNT, EPERM))
	return (EPERM);
#else
    if (!osi_suser(osi_getucred()))
	return (EPERM);
#endif
    if (error = vflush(vfsp, NULLVP, NULL)) /* release text vnode */
	    return (error);
#endif /* _KERNEL */

    return efs_unmount_subr (vfsp, 0, NULL);
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
    int ignore_error;
    int error;
    int rdonly = vfsp->m_flag & M_RDONLY;
    int flag = (rdonly ? FREAD : FREAD | FWRITE);
    int inuseflags = INUSE_VFSVALID | INUSE_ALLOWROOT;

    emp = VFSTOEFS (vfsp);
    inuse = vnm_Inuse (vfsp, emp->volHandle, inuseflags);

    if ((inuse & VFSINUSE) && !force)
        return (EBUSY);

    if (!inuse) {
	epig_CloseVolume (elbt_NullTranId, emp->volHandle);
	epig_CloseAggregate (emp->aggHandle);

#ifdef _KERNEL
	VOP_CLOSE (emp->m_devvp, flag, u.u_cred, ignore_error);
#else /* _KERNEL */
	VOP_CLOSE (emp->m_devvp, flag, NULL, ignore_error);
#endif /* _KERNEL */

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
