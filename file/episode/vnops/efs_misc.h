/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: efs_misc.h,v $
 * Revision 1.1.20.1  1996/10/02  17:44:11  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:35:59  damon]
 *
 * $EndLog$
 */

/* $Header: /u0/rcs_trees/dce/rcs/file/episode/vnops/efs_misc.h,v 1.1.20.1 1996/10/02 17:44:11 damon Exp $ */
/*
 * Copyright (C) 1996, 1993 Transarc Corporation
 * All rights reserved.
 */

#ifndef TRANSARC_EFS_MISC_H
#define TRANSARC_EFS_MISC_H

#include <dcedfs/osi.h>
#include <dcedfs/episode/volume.h>
#include <dcedfs/episode/anode.h>
#include <dcedfs/episode/logbuf.h>
#include <efs_evnode.h>

/*
 * Functions exported by efs_misc.c and efs_vnode.c
 */

#define VNM_MAP_COMMON_ERROR(code, debugBit) \
    (code = vnm_MapCommonError(code, debugBit))

#define QV_RECYCLE 0x1

#if !defined(AFS_SUNOS5_ENV) && !defined(AFS_AIX_ENV)
extern struct efs_buf *vnm_AllocEBuffer(void);
extern void vnm_FreeEBuffer(struct efs_buf *eb);
#endif /* AFS_SUNOS5_ENV, AFS_AIX_ENV */

extern int vnm_MapCommonError(int code, int debugBit);
extern int vnm_GetZLCVnode(
    struct volume *volp,
    u_int *iterP,
    struct evnode **evPP
);

#define TRUNCATE_USEVM 0x1
extern int vnm_Truncate(
    struct evnode *evp,
    u_long len, 
    int flags,
    osi_cred_t *credp
);
extern void vnm_SetAtime(struct evnode *evp);
extern void vnm_SetMCtime(efs_vnode_t evp, int markM);
extern void vnm_UpdateAnode (
  efs_vnode_t evp,
  elbb_tranRec_t transId);

/* Values for markM */
#define MARK_MC		1
#define MARK_C		0

extern void vnm_ClearSUID(struct evnode *evp);
extern void vnm_VnodeBusy(struct evnode *evp);
extern void vnm_VnodeNotBusy(struct evnode *evp);
extern int vnm_WaitForBusyVnode(struct evnode *evp, osi_dlock_t *lockp);
extern void vnm_WaitForAnyBusyVnode(osi_dlock_t *lockp);
extern int vnm_Init(u_long nvnodes);

#if OLD_VNSYNC
extern void vnm_ReleaseResources(struct evnode *evp);
extern void vnm_Close(struct evnode *evp);
extern void vnm_Free(struct evnode *evp);
extern int vnm_PartialAlloc(struct evnode **evpP);
extern int vnm_FinishAlloc(
    epi_volFileId_t *fid,
    epi_volume_t volh,
    struct vfs *vfsp,
    struct evnode *evp,
    elbb_tranRec_t transId
);
extern int vnm_AbortAlloc(struct evnode *evp);
#endif

#ifdef AFS_SUNOS5_ENV
/* from SUNOS/efs_sunos_inactive.c */
extern int efs_queue_inactive(struct vnode *vp, osi_cred_t *cred);
extern int efs_handle_pending_inactive(void);
extern void efs_wait_for_pending_inactive(void);
#else
#define efs_handle_pending_inactive()	0
#define efs_wait_for_pending_inactive()
#endif

extern int vnm_FindVnode(
  struct volume *volp,
  epi_volFileId_t *fid,
  efs_vnode_t *evpP
);
extern int vnm_Delete(
  struct volume *volp,
  epi_volFileId_t *fid
);
extern efs_vnode_t vnm_Allocate(void);

extern int vnm_SetIdentity(
  efs_vnode_t evp,
  struct volume *volp,
  epi_volFileId_t *fid,
  epi_anode_t file
);
extern int vnm_Inactive(struct vnode *vp);
extern epi_anode_t vnm_GetFileH(efs_vnode_t evp);

extern int vnm_CreateVolume(
    epi_anode_t avl,
    osi_cred_t *credP,
    int createRoot,
    struct epiv_status *statusP,
    u_long *indexP
);
extern int vnm_RestartUse(struct evnode *evp);
extern void vnm_StopUse(struct volume *volp, int openbits);
extern void vnm_SyncVnodes(int flags);
extern void vnm_QuiesceVol(struct volume *volp, int recycle);
extern int vnm_Inuse(struct volume * volp);

extern void vnm_SetVfsP(struct volume * volp);

int efs_reclaim(struct vnode *vp);
#if defined(AFS_OSF_ENV) && defined(FREEZE_AGGR_OSF_1)
extern int efs_freeze(struct async_device *aggrDev)
extern int efs_unfreeze(struct async_device *aggrDev);
#endif /* AFS_OSF_ENV && FREEZE_AGGR_OSF_1 */

#endif /* TRANSARC_EFS_MISC_H */
