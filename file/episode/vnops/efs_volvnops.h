/*
 * Copyright (C) 1996, 1994 Transarc Corporation - All rights reserved.
 *
 * $Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/vnops/RCS/efs_volvnops.h,v 1.29 1996/02/12 15:13:50 cfe Exp $
 */

#ifndef TRANSARC_EFS_VOLVNOPS_H
#define TRANSARC_EFS_VOLVNOPS_H

#include <dcedfs/aggr.h>
#include <dcedfs/volume.h>
#include "efs_evnode.h"

/* Private data for volume structure */

struct efs_volStat {

    /* fields needed by volume operations */

    u_long vld_index;			/* index of volume within aggregate */
    u_long vld_lastIndex;		/* highest valid epiv-style index */
    u_long vld_wholeVolProgress;	/* where we left off: destroy/clone */
    epi_volume_t vld_vh;		/* volume handle */
    int vld_flags;			/* unused misc. flags */

#ifdef AFS_AIX_ENV
    struct buf *sideRail;		/* pageins waiting for volume ops */
#endif
    /* Stuff needed by vnode (and vfs) operations */

    int vld_openbits;			/* vnm_StopUse bits */
    struct osi_vfs *vld_vfs;		/* virtual file system */
#ifndef AFS_AIX_ENV
    struct vnode *vld_root;		/* root of locally mounted fileset */
#endif
    unsigned vld_roVolume:1;		/* volume header status claims R/O */
    unsigned vld_inconVolume:1;		/*  ... INCONSISTENT */
    unsigned vld_syncMeta:1;		/* NFS Semantics mount option */
    unsigned vld_writeInProgress:1;	/* A volume write in progress */

    epi_volFileId_t vld_writeFid;	/* Fid of file being written to */
    u_long vld_writeLen;		/* counter used in vol_efsWrite */
};

typedef struct efs_volStat efs_volStat_t;

/* Flags in vld_flags */
/* #define VLD_SWAPPED 0x1 */			/* recently swapped */

#ifndef AFS_AIX_ENV
#define VLD_ROOT(vfsp) (VTOEVLD(VFSTOV(vfsp))->vld_root)
#else
#define VLD_ROOT(vfsp) ((vfsp)->vfs_mntd)
#endif

extern epi_anode_t ag_efsGetHandle(struct aggr *);

/* Episode-specific data of a volume */
#define VTOEVLD(volp) ((struct efs_volStat *) ((volp)->v_fsDatap))
/* Episode volume handle for a volume */
#define VTOVH(volp) ((VTOEVLD (volp))->vld_vh)
/* Episode aggregate handle (avl) for a volume */
#define VTOAH(volp) ag_efsGetHandle((volp)->v_paggrp)
/* VFS pointer for a volume */
#define VTOVFS(volp) ((VTOEVLD (volp))->vld_vfs)
/* Block device vnode for a volume */
#define VTODEVVP(volp) ((volp)->v_paggrp->devvp)

/* Mount option flags */
#define efs_MntFlagNFS(evp) (VTOEVLD((evp)->vd_volp)->vld_syncMeta)
#define efs_SyncMetaData(evp) (osi_ProcIsNFS() || efs_MntFlagNFS(evp))

/* Convert vfs ptr to efsmount ptr.  */

#ifdef AFS_OSF_ENV
#define VFSTOV(vfsP)    ((struct volume *)((vfsP)->m_data))
#else
#define VFSTOV(vfsP)    ((struct volume *)((vfsP)->vfs_data))
#endif /* AFS_OSF_ENV */

/* Deleted-, invalid-afsFid creation and test macros */

#define vol_MakeInvalidAfsFid(afidP) \
((afidP)->Vnode = EPID_INVALID_INDEX, (afidP)->Unique = EPID_INVALID_UNIQUE)

#define vol_MakeDeletedAfsFid(afidP, vnode) \
((afidP)->Vnode = (vnode), (afidP)->Unique = EPID_DONTCARE_UNIQUE)

#define vol_IsInvalidAfsFid(afidP) \
(((afidP)->Vnode  == EPID_INVALID_INDEX) && \
 ((afidP)->Unique == EPID_INVALID_UNIQUE))

#define vol_IsDeletedAfsFid(afidP) ((afidP)->Unique == EPID_DONTCARE_UNIQUE)

/* Episode vnode for an afsFid */

#define VFtoEV(volP, afidP, evpP, codeP) \
{ \
      epi_volFileId_t epifid; \
      if (vol_IsInvalidAfsFid(afidP)) { \
          *(codeP) = EINVAL; \
      } else if (vol_IsDeletedAfsFid(afidP)) { \
	  *(codeP) = ESTALE; \
      } else { \
          epid_Assemble(&epifid, (afidP)->Vnode, (afidP)->Unique); \
          *(codeP) = vnm_FindVnode((volP), &epifid, (evpP)); \
      } \
}


extern void vnvl_GetVolumeReadonly (
  struct volume *volp
);

#if  defined(AFS_AIX_ENV) && defined(KERNEL)
extern void efs_WakeupBlockedPageIns(struct volume *volp);
#else
#define efs_WakeupBlockedPageIns(volp) osi_Wakeup(volp)
#endif

extern int vol_efsHold(struct volume *volp);
extern int vol_efsRele(struct volume *volp);
extern int vol_efsLock(struct volume *volp, int type);
extern int vol_efsUnlock(struct volume *volp, int type);
extern int vol_efsSetDyStat(struct volume *volp, struct vol_stat_dy *dystatp);
extern int vol_efsAttach(struct volume *volp);
extern int vol_efsFreeDyStat(struct volume *volp);
extern int vol_efsDetach(struct volume *volp, int anyForce);
extern int vol_efsVget(
  struct volume *volp,
  struct afsFid *Fidp,
  struct vnode **evpP);
extern int vol_efsRoot(struct volume *volp, struct vnode **evpP);
extern int vol_efsIsRoot(
  struct volume *volp,
  struct afsFid *Fidp,
  int *flagP);

#ifndef KERNEL
/*
 * For accessing volops vector from user space.  In the kernel, these
 * are defined in volume.h as references through the volops vector.
 */
#define VOL_HOLD(VOLP)		vol_efsHold(VOLP)
#define VOL_RELE(VOLP)		vol_efsRele(VOLP)
#define VOL_LOCK(VOLP, TYPE)	vol_efsLock(VOLP, TYPE)
#define VOL_UNLOCK(VOLP, TYPE)	vol_efsUnlock(VOLP, TYPE)
#define VOL_VGET(VOLP, FID, VPP) vol_efsVget(VOLP, FID, VPP)
#define VOL_ROOT(VOLP, VPP)	vol_efsRoot(VOLP, VPP)
#define VOL_ISROOT(VOLP, FID, ISFLAG) vol_efsIsRoot(VOLP, FID, ISFLAG)
#define VOL_SETDYSTAT(VOLP, DYSTATP) vol_efsSetDyStat(VOLP, DYSTATP)
#define VOL_FREEDYSTAT(VOLP)	vol_efsFreeDyStat(VOLP)
#define VOL_ATTACH(VOLP)	vol_efsAttach(VOLP)
#define VOL_DETACH(VOLP)	vol_efsDetach(VOLP, 0)
#endif /* !KERNEL */

#endif /* !TRANSARC_EFS_VOLVNOPS_H */
