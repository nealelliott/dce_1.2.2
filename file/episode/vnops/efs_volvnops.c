/* Copyright (C) 1996, 1994 Transarc Corporation - All rights reserved. */

/* Episode File System -- Volume operations on vnodes */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>

#include <dcedfs/osi_kvnode.h>		/* VN_RELE */
#include <dcedfs/vol_init.h>
#include <dcedfs/episode/file.h>
#include <dcedfs/volume.h>
#include <dcedfs/episode/aggregate.h>	/* epig_CloseVolume */
#include "efs_evnode.h"
#include "efs_volvnops.h"
#include "efs_misc.h"

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/vnops/RCS/efs_volvnops.c,v 1.25 1996/03/25 21:08:43 cfe Exp $")

#define DEBUG_THIS_FILE EFS_DEBUG_VOLVNOPS

/* vnvl_GetVolumeReadonly -- determine if a volume header specified R/O. */

void vnvl_GetVolumeReadonly(
  struct volume *volp)
{
    if (!VTOEVLD(volp))
	return;				/* detach */
    if (VTOVH (volp)) {
	long code;
	int32 volFlags;			/* flags from disk vol hdr */

	code = epiv_GetFlags (VTOVH (volp), &volFlags);
	afsl_MBZ (code);
	VTOEVLD(volp)->vld_roVolume =
	    ((volFlags & EPIV_STATUS_FLAGS_READONLY) != 0);
	VTOEVLD(volp)->vld_inconVolume =
	    ((volFlags & EPIV_STATUS_FLAGS_INCONSISTENT) != 0);
    } else {
	/* volume is being destroyed or detached */
	VTOEVLD(volp)->vld_roVolume = 1;
	VTOEVLD(volp)->vld_inconVolume = 1;
    }
}

/*
 * vol_efsHold -- note reference to a volume
 *
 * LOCKS USED -- expects the preemption lock to be held.
 */

/* EXPORT */
int vol_efsHold(struct volume *volp)
{
    lock_ObtainWrite(&volp->v_lock);
    volp->v_count++;
    lock_ReleaseWrite(&volp->v_lock);
    return 0;
}

/*
 * vol_efsRele -- discard reference to a volume
 *
 * LOCKS USED -- expects the preemption lock to be held.
 */

/* EXPORT */
int vol_efsRele(struct volume *volp)
{
    lock_ObtainWrite(&volp->v_lock);
    afsl_Assert(volp->v_count > 0);
    if (volp->v_count == 1) {
	vol_VolInactive(volp);
    } else {
	volp->v_count--;
	lock_ReleaseWrite(&volp->v_lock);
    }
    return 0;
}

/*
 * vol_efsLock -- obtain lock on a volume
 *
 * LOCKS USED -- expects the preemption lock to be held.
 */

/* EXPORT */
int vol_efsLock(
  struct volume *volp,	/* volume */
  int type)			/* type of lock - read, write, share */
{
    switch (type) {
	case READ_LOCK: lock_ObtainRead (&volp->v_lock); break;
	case WRITE_LOCK: lock_ObtainWrite (&volp->v_lock); break;
	case SHARED_LOCK: lock_ObtainShared (&volp->v_lock); break;
	default: return EINVAL;
    }
    return 0;
}

/*
 * vol_efsUnlock -- release lock on a volume
 *
 * LOCKS USED -- expects the preemption lock to be held.
 */

/* EXPORT */
int vol_efsUnlock(struct volume *volp, int type)
{
    switch (type) {
	case READ_LOCK: lock_ReleaseRead (&volp->v_lock); break;
	case WRITE_LOCK: lock_ReleaseWrite (&volp->v_lock); break;
	case SHARED_LOCK: lock_ReleaseShared (&volp->v_lock); break;
	default: return EINVAL;
    }
    return 0;
}

/*
 * vol_efsSetDyStat -- Allocate space for FS-specific data of volume
 *
 * LOCKS USED -- expects the preemption lock to be held.
 */

/* EXPORT */
int vol_efsSetDyStat(struct volume *volp, struct vol_stat_dy *dystatp)
{
    struct efs_volStat *voldata;	/* private data for volp */

    lock_ObtainWrite(&volp->v_lock);
    voldata = osi_Alloc (sizeof (struct efs_volStat));
    bzero ((caddr_t) voldata, sizeof(struct efs_volStat));
    voldata->vld_index = dystatp->index;
    volp->v_fsDatap = (opaque) voldata;
    lock_ReleaseWrite(&volp->v_lock);
    return (0);
}

/*
 * vol_efsAttach -- Per-volume code for aggregate attaching
 *
 *     For Episode, we get a handle for the volume and put it in the private
 *     data area.
 *
 * LOCKS USED -- expects the preemption lock to be held.
 */

/* EXPORT */
int vol_efsAttach(struct volume *volp)
{
    struct efs_volStat *voldata;	/* private data of volp */
    epi_volume_t volh;			/* volume handle */
    int code;				/* error return code */

    icl_Trace1(efs_iclSet, EFS_TRACE_EFSATTACH,
	       ICL_TYPE_LONG, AFS_hgetlo(volp->v_volId));
    lock_ObtainWrite(&volp->v_lock);
    voldata = VTOEVLD (volp);
    osi_MakePreemptionRight();

    code = epig_OpenVolume (VTOAH(volp), voldata->vld_index, &volh);
    /*
     * code must be zero here, because we don't get here unless
     * AG_VOLINFO has verified that there is a valid volume in this slot.
     */
    afsl_MBZ (code);
    voldata->vld_vh = volh;
    vnvl_GetVolumeReadonly (volp);

    osi_UnmakePreemptionRight();
    lock_ReleaseWrite(&volp->v_lock);
    return 0;
}

/*
 * vol_efsFreeDyStat -- free FS-specific data structures for volume
 *     Called with volp->v_lock write-locked.
 *
 * LOCKS USED -- expects the preemption lock to be held.
 */

/* EXPORT */
int vol_efsFreeDyStat(struct volume *volp)
{
    osi_Free (volp->v_fsDatap, sizeof (struct efs_volStat));
    volp->v_fsDatap = (opaque) 0;	/* just in case */
    return (0);
}

/*
 * vol_efsDetach -- Per-volume code for aggregate detaching
 *
 *     For Episode, we close the handle for the volume.
 *
 * LOCKS USED -- expects the preemption lock to be held.
 */

/* EXPORT */
int vol_efsDetach(struct volume *volp, int anyForce)
{
    epi_volume_t volh;			/* volume handle */
    long code;

    osi_MakePreemptionRight();
    icl_Trace2(efs_iclSet, EFS_TRACE_EFSDETACH_BEGIN_2,
	       ICL_TYPE_LONG, AFS_hgetlo(volp->v_volId),
	       ICL_TYPE_LONG, (long) anyForce);
    lock_ObtainWrite_r(&volp->v_lock);
    do {
	volh = VTOVH (volp);
	if (volh == 0) {		/* it's already been deleted */
	    icl_Trace2(efs_iclSet, EFS_TRACE_EFSDETACH_DONEBEFORE,
		       ICL_TYPE_LONG, AFS_hgetlo(volp->v_volId),
		       ICL_TYPE_LONG, volp->v_count);
	    lock_ReleaseWrite_r(&volp->v_lock);
	    osi_UnmakePreemptionRight();
	    return 0;
	}

	/* we can't hold the volume lock when calling vnm_Inuse() */
	lock_ReleaseWrite_r(&volp->v_lock);
	code = vnm_Inuse (volp);
	if ((anyForce == 0) && (code != 0)) {
	    icl_Trace2(efs_iclSet, EFS_TRACE_EFSDETACH_BUSYFAIL2,
		       ICL_TYPE_LONG, AFS_hgetlo(volp->v_volId),
		       ICL_TYPE_LONG, code);
	    osi_UnmakePreemptionRight();
	    return EBUSY;
	} else {
	    vnm_StopUse (volp, (STOPUSE_NO_CHANGE|STOPUSE_NO_ANODE|
				STOPUSE_NO_STATUS|STOPUSE_NO_VM));
	}
	lock_ObtainWrite_r(&volp->v_lock);
    } while (volh != VTOVH(volp));  /* loop until volp's volh doesn't change */

    /* when we exit the loop, the volp is write-locked */

    code = epig_CloseVolume (buffer_nullTranId, volh);
    if (code) {
	lock_ReleaseWrite_r(&volp->v_lock);
	EFS_TRACE_VNOPS(DEBUG_THIS_FILE, code, EBUSY);	
	osi_UnmakePreemptionRight();
	return EBUSY;
    }
    VTOVH (volp) = (epi_volume_t) 0;
    lock_ReleaseWrite_r(&volp->v_lock);

    /*
     * calling vnm_StopUse(x, ~STOPUSE_NO_ANODE) with volh cleared
     * makes all vnodes stale
     */
    vnm_StopUse (volp, STOPUSE_NO_CHANGE);
    icl_Trace1(efs_iclSet, EFS_TRACE_EFSDETACH_SUCCEED,
	       ICL_TYPE_LONG, AFS_hgetlo(volp->v_volId));
    osi_UnmakePreemptionRight();
    return 0;
}

/*
 * vol_efsVget -- the vnode designated by a fid
 *
 * LOCKS USED -- expects the preemption lock to be held.
 */

/* EXPORT */
int vol_efsVget(
    struct volume *volp,		/* volume */
    struct afsFid *Fidp,		/* file ID */
    struct vnode **evpP)		/* place to put Episode vnode pointer */
{
    int code;

    osi_MakePreemptionRight();
    VFtoEV(volp, Fidp, ((struct evnode **)evpP), &code);
    osi_UnmakePreemptionRight();

    /* EIO is returned if we pass in a bad fid, but bad fids aren't
     * that serious an error when they come from the network.
     * ESTALE better categorizes this type of error.
     */
    if (code == EIO) code = ESTALE;
    return code;
}

/*
 * vol_efsRoot -- the vnode for the root of the volume
 *
 * LOCKS USED -- expects the preemption lock to be held.
 */

/* EXPORT */
int vol_efsRoot(struct volume *volp, struct vnode **evpP)
{
    epi_volFileId_t epifid;              /* Episode file ID */
    int code;

    epid_MakeVolumeRoot(&epifid);

    osi_MakePreemptionRight();
    code = vnm_FindVnode(volp, &epifid, ((struct evnode **)evpP));
    osi_UnmakePreemptionRight();
    return code;
}

/*
 * vol_efsIsRoot -- detect an EFS volume's root fid.
 */

/* EXPORT */
int vol_efsIsRoot(
    struct volume *volp,		/* volume */
    struct afsFid *Fidp,		/* file ID */
    int *flagp)				/* place to put result-true iff root */
{
    epi_volFileId_t epifid;		/* Episode file ID */

    epid_Assemble(&epifid, Fidp->Vnode, Fidp->Unique);
    *flagp = epid_IsVolumeRoot(&epifid);
    return (0);
}
