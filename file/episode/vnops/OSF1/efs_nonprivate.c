/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * Copyright (C) 1996, 1990 Transarc Corporation
 * All rights reserved.
 */
/*
 * HISTORY
 * $Log: efs_nonprivate.c,v $
 * Revision 1.1.5.1  1996/10/02  17:43:07  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:35:24  damon]
 *
 * $EndLog$
 */

/*
 * Versions of various vnm_ routines intended to work with a system-wide
 * shared vnode pool.
 */
#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>			/* osi_Alloc */
#include <dcedfs/debug.h>
#include <dcedfs/lock.h>		/* osi_dlock_t */
#include <dcedfs/queue.h>		/* struct squeue */
#include <dcedfs/episode/file.h>	/* struct epif_status */
#include <dcedfs/episode/aggregate.h>	/* epig_ functions */
#include <dcedfs/episode/dacl_anode.h>
#include <dcedfs/osi_kvnode.h>
#include <dcedfs/dir.h>
#include <dcedfs/dir_buffer.h>
#include "efs_dirhandle.h"		/* struct epi_Dir_Handle */
#include "efs_evnode.h"			/* struct evnode & ICL dcls */
#include "efsmount.h"			/* INUSE, VFSINUSE */
#include "efs_vnodeops.h"
#include <dcedfs/episode/efs_trace.h>
#include <dcedfs/episode/epi_thread.h>	/* Required by bomb.h */
#include <dcedfs/aggr.h>		/* VNOP_TYPE_READWRITE */
#include <dcedfs/volume.h>		/* VOL_RELE */
#include <dcedfs/volreg.h>
#include <dcedfs/xvfs_vnode.h>		/* SET_CONVERTED */

#include "efs_misc.h"

#define EFS_PRIVATE_POOL	/* use private pool for vnode allocation */
#if defined(AFS_OSF_ENV) && defined(_KERNEL)
#undef EFS_PRIVATE_POOL
#endif /* AFS_OSF_ENV */

#ifndef EFS_PRIVATE_POOL

RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/episode/vnops/OSF1/efs_nonprivate.c,v 1.1.5.1 1996/10/02 17:43:07 damon Exp $")

/* EXPORT */
int vnm_StopUse(
    epi_volume_t vol,			/* volume handle */
    int flags)				/* various flags */
{
    int makePhantom;			/* true to phantomize vnodes */
    struct squeue *hashq, *evq;		/* pointers to entries in hash table */
    struct evnode *evp;			/* temp vnode pointer */
    struct epi_Dir_Handle dh;		/* directory handle for dirb_Annul */
    struct epiv_ident vid;		/* volume ID of vol */
    int i, roused;

    makePhantom = (flags & 1);
    epiv_GetIdent (vol, &vid);
    icl_Trace3(efs_iclSet, EFS_TRACE_STOPUSE_BEGIN,
	       ICL_TYPE_LONG, AFS_hgetlo(vid.id),
	       ICL_TYPE_LONG, flags,
	       ICL_TYPE_LONG, VNHASHSIZE);
    vnm_SyncVnodes ();
    icl_Trace0(efs_iclSet, EFS_TRACE_STOPUSE_AFTER_SYNC);

    for (i = 0; i < VNHASHSIZE; i++) {
	hashq = &vnhash[i];
waitloop:
	lock_ObtainWrite (&vntable_lock);
	evq = QNext (hashq);
	lock_ReleaseWrite (&vntable_lock);
	while (evq != hashq) {
	    evp = HQTOV (evq);
	    /* Skip vnodes that are not associated with any filesystem object*/
	    if (evp->vd_flags & VD_NOIDENTITY)
		continue;

	    if (evp->vd_flags & VD_WAITFORME)
		if (evp->vd_volp == vol) {
		    icl_Trace4(efs_iclSet, EFS_TRACE_STOPUSE_SLEEPING,
			       ICL_TYPE_LONG, AFS_hgetlo(vid.id),
			       ICL_TYPE_POINTER, evp,
			       ICL_TYPE_LONG, i,
			       ICL_TYPE_LONG, evp->vd_flags);
		    evp->vd_flags |= VD_WAITERS;
		    osi_Sleep (evp);
		    goto waitloop;
		} else
		    goto nextvnode;
	    if (evp->vd_flags & VD_PHANTOM) {	/* if already phantomized */
		if (!AFS_hsame (vid.id, evp->vd_volid))
		    goto nextvnode;
		FAST_VN_HOLD (EVTOV(evp));
		evp->vd_flags |= (VD_WAITFORME | VD_ALLOWSTRAT);
		roused = 0;
		goto IsPhantom;
	    }
	    if (evp->vd_volp != vol) goto nextvnode;

	    icl_Trace4(efs_iclSet, EFS_TRACE_STOPUSE_STOPPING,
		       ICL_TYPE_LONG, AFS_hgetlo(vid.id),
		       ICL_TYPE_POINTER, evp,
		       ICL_TYPE_LONG, i,
		       ICL_TYPE_LONG, evp->vd_flags);

	    /* rouse if necessary */
	    if (!(EVTOV(evp))->v_count) {
		vnm_Rouse (evp);
		roused = 1;
	    } else {
		FAST_VN_HOLD (EVTOV(evp));
		evp->vd_flags |= (VD_WAITFORME | VD_ALLOWSTRAT);
		roused = 0;
	    }

	    /* flush directory buffer cache */
	    if (EV_ISDIR (evp)) {
		vndh_SetDirHandle (evp, &dh);
		dirb_Annul((struct Dir_Handle *)&dh);
	    }

	    /* Invalidate cached ACL if necessary */
	    if (evp->vd_acl) {
		dacl_ReleaseAcl (evp->vd_acl);
		evp->vd_acl = 0;
	    }

	    /* write through cached atime */
	    if (evp->vd_flags & VD_OUTOFDATE)
		vnm_UpdateAnode (evp);

	    /* flush VM */
	    vnvm_StopUse (EVTOV (evp), osi_credp);

	    if (makePhantom) {
		if (roused) {
		    evp_vd_flags |= VD_NOIDENTITY;
		} else {
		    /*Phantomize */
		    struct epiv_ident vid;
		    long code;
		    code = epiv_GetIdent (evp->vd_volp, &vid);
		    if (!code)
			code = epif_Close (evp->vd_ap);
		    afsl_MBZ (code);
		    evp->vd_ap = NULL;
		    evp->vd_volid = vid.id;
		    evp->vd_flags |= VD_PHANTOM;
		}
	    }
IsPhantom:
	    if ((evp->vd_flags & VD_PHANTOM) && flags)
		MakeStale (vol, evp, flags);

	    if (roused)
		vnm_Unrouse (evp);
	    else {
		evp->vd_flags &= ~(VD_WAITFORME | VD_ALLOWSTRAT);
		if (evp->vd_flags & VD_WAITERS) {
		    icl_Trace2(efs_iclSet, EFS_TRACE_STOPUSE_WAKING,
			       ICL_TYPE_POINTER, evp,
			       ICL_TYPE_LONG, evp->vd_flags);
		    evp->vd_flags &= ~VD_WAITERS;
		    osi_Wakeup (evp);
		}
		OSI_VN_RELE (EVTOV(evp));
	    }
nextvnode:
	    lock_ObtainWrite (&vntable_lock);
	    evq = QNext (evq);
	    lock_ReleaseWrite (&vntable_lock);
	}
    }
    icl_Trace1(efs_iclSet, EFS_TRACE_STOPUSE_END,
	       ICL_TYPE_LONG, AFS_hgetlo(vid.id));
}

/* EXPORT */
void vnm_SyncVnodes(void)
{
    struct squeue *hashq, *evq;		/* pointers to entries in hash table */
    struct evnode *evp;			/* temp vnode pointer */
    int i;

    for (i = 0; i < VNHASHSIZE; i++) {
        hashq = &vnhash[i];
	lock_ObtainWrite (&vntable_lock);
        evq = QNext (hashq);
	lock_ReleaseWrite (&vntable_lock);
        while (evq != hashq) {
            evp = HQTOV (evq);

	    /* vnode in transit or phantomized or VFS about to be unmounted */
	    if (evp->vd_flags & (VD_WAITFORME | VD_PHANTOM | VD_NOIDENTITY))
                goto nextvnode;

            if (!(EVTOV(evp))->v_count) {
                if (evp->vd_flags & VD_OUTOFDATE) {
                    vnm_Rouse (evp);
                    vnm_UpdateAnode (evp);
                    vnm_Unrouse (evp);
                }
            } else {
                if (tlock_Check (&evp->vd_tlock)) /* if already locked */
                    goto nextvnode;
                if (evp->vd_flags & VD_OUTOFDATE)
                    vnm_UpdateAnode (evp);
            }
	    (void) vnvm_schedule (EVTOV (evp), osi_credp, 1);
nextvnode:
	    lock_ObtainWrite (&vntable_lock);
            evq = QNext (evq);
	    lock_ReleaseWrite (&vntable_lock);
        }
    }
}

/* EXPORT */
void vnm_SyncVnodes(void)
{
    struct squeue *hashq, *evq;		/* pointers to entries in hash table */
    struct evnode *evp;			/* temp vnode pointer */
    int i;

    for (i = 0; i < VNHASHSIZE; i++) {
        hashq = &vnhash[i];
	lock_ObtainWrite (&vntable_lock);
        evq = QNext (hashq);
	lock_ReleaseWrite (&vntable_lock);
        while (evq != hashq) {
            evp = HQTOV (evq);

	    /* vnode in transit or phantomized or VFS about to be unmounted */
	    if (evp->vd_flags & (VD_WAITFORME | VD_PHANTOM | VD_NOIDENTITY))
                goto nextvnode;

            if (!(EVTOV(evp))->v_count) {
                if (evp->vd_flags & VD_OUTOFDATE) {
                    vnm_Rouse (evp);
                    vnm_UpdateAnode (evp);
                    vnm_Unrouse (evp);
                }
            } else {
                if (tlock_Check (&evp->vd_tlock)) /* if already locked */
                    goto nextvnode;
                if (evp->vd_flags & VD_OUTOFDATE)
                    vnm_UpdateAnode (evp);
            }
	    (void) vnvm_schedule (EVTOV (evp), osi_credp, 1);
nextvnode:
	    lock_ObtainWrite (&vntable_lock);
            evq = QNext (evq);
	    lock_ReleaseWrite (&vntable_lock);
        }
    }
}

/* EXPORT */
int vnm_Inuse(
    struct osi_vfs *vfsp,		/* virtual file system */
    epi_volume_t vol,			/* volume handle */
    int flags)				/* various bits */
{
    struct squeue *hashq, *evq;		/* pointers to entries in hash table */
    struct evnode *evp;			/* temp vnode pointer */
    struct epi_Dir_Handle dh;		/* directory handle for dirb_Annul */
    struct efsMOUNT *emp;		/* private data from mount table */
    int code = 0;			/* return code */
    int i;
    struct epiv_ident vid;		/* ident for vol */
    struct osi_vfs *xvfsp;              /* Scratch vfs pointer */

    if (flags & INUSE_VFSVALID) {
	emp = VFSTOEFS (vfsp);
	emp->flags |= MP_QUIESCENT;
    }

    /* Get volume ID in case we discover any phantoms */
    epiv_GetIdent (vol, &vid);

    for (i = 0; i < VNHASHSIZE; i++) {
        hashq = &vnhash[i];
waitloop:
	lock_ObtainWrite (&vntable_lock);
        evq = QNext (hashq);
	lock_ReleaseWrite (&vntable_lock);
        while (evq != hashq) {
            evp = HQTOV(evq);

	    if (evp->vd_flags & VD_NOIDENTITY)
		continue;

	    if (evp->vd_flags & VD_WAITFORME)
		if (evp->vd_volp == vol) {
		    icl_Trace2(efs_iclSet, EFS_TRACE_INUSE_SLEEPING,
			       ICL_TYPE_POINTER, evp,
			       ICL_TYPE_LONG, evp->vd_flags);
		    evp->vd_flags |= VD_WAITERS;
		    osi_Sleep (evp);
		    goto waitloop;
		} else
		    goto nextvnode;
	    if (evp->vd_flags & VD_PHANTOM) {
		if ((evp->vd_flags & VD_STALE)
		    || !AFS_hsame (vid.id, evp->vd_volid))
		    goto nextvnode;
		goto inuse;
	    }
            if (evp->vd_volp != vol) goto nextvnode;

	    /* expunge vnode from dir buffer cache */
            if (EV_ISDIR (evp)) {
                vndh_SetDirHandle (evp, &dh);
                dirb_Annul((struct Dir_Handle *)&dh);
            }

	    /* purge vnode's ACL */
	    if (evp->vd_acl) {
		dacl_ReleaseAcl (evp->vd_acl);
		evp->vd_acl = 0;
	    }

            if (!(EVTOV(evp))->v_count) {
                vnm_Rouse (evp);

                if (evp->vd_flags & VD_OUTOFDATE)
                    vnm_UpdateAnode (evp);

		/* flush VM */
		vnvm_recycle (EVTOV (evp), osi_credp);

                vnm_Unrouse (evp);
                /*
                 * volp is about to become invalid;
                 * make sure no one else uses it.
                 */
		evp->vd_flags |= VD_NOIDENTITY;
            } else {
inuse:
                if (epid_IsVolumeRoot (&evp->vd_fid)
                        && (flags & INUSE_ALLOWROOT)
                        && (EVTOV(evp))->v_count == 1) {
                    if (evp->vd_flags & VD_OUTOFDATE) {
                        vnm_UpdateAnode (evp);
                    }
                    goto nextvnode;
                }

                code |= INUSE;
		xvfsp = EVTOVFSP (evp);
		if (EPI_ISNULLVFSP (xvfsp)) {
		    code |= AGFSINUSE;
		} else {
		    if (flags & INUSE_VFSVALID) {
			code |= VFSINUSE;
		    }
		}
	    }
nextvnode:
	    lock_ObtainWrite (&vntable_lock);
	    evq = QNext (evq);
	    lock_ReleaseWrite (&vntable_lock);
        }
    }

    if (flags & INUSE_VFSVALID)
	if (!(emp->flags & MP_QUIESCENT))
	    code |= ( INUSE | VFSINUSE );

out:
    return (code);
}

/*
 * efs_reclaim -- Prepare a vnode to be reclaimed by the kernel
 *
 * This is used in OSF/1, a non-private-pool kernel.  It is an X-op.
 */

#ifdef AFS_OSF_ENV

int efs_reclaim(struct vnode *vp)
{
    struct evnode *evp;

    evp = VTOEV(vp);

#ifdef FREEZE_AGGR_OSF_1
    /*
     * The NOIDENTITY vnodes will soon be removed from the evnode hash table
     * and so need to be handled here.
     */
    if (evp->vd_flags & VD_NOIDENTITY) {
	return(0);
    }
#endif

    lock_ObtainWrite (&vntable_lock);
    /* Prevent recycling of vnode until sync daemon has finished with it */
    while (evp->vd_flags & (VD_NOIDENTITY | VD_WAITFORME) ==
	(VD_NOIDENTITY | VD_WAITFORME)) {
	icl_Trace2(efs_iclSet, EFS_TRACE_RECLAIM_SLEEPING,
		   ICL_TYPE_POINTER, evp,
		   ICL_TYPE_LONG, evp->vd_flags);
	(void) vnm_WaitForBusyVnode(evp, &vntable_lock);
    }

    if (!(evp->vd_flags & VD_NOIDENTITY)) {
	Recycle (evp);
	evp->vd_flags |= VD_NOIDENTITY;
    }
    SET_UNCONVERTED (EVTOV (evp));
    unvhash(evp);
    lock_ReleaseWrite (&vntable_lock);
    return (0);
}
#endif /* AFS_OSF_ENV */

#if defined(AFS_OSF_ENV) && defined(FREEZE_AGGR_OSF_1)

#define V_FROZEN (0x40000)

/* Recycling status
 *  NOIDENTITY vnodes - can occur - dealt with in efs_reclaim
 *  PHANTOM	   - can occur - dealt with in efs_freeze, efs_unfreeze
 *  OPEN	   - should never happen
 *  CLOSED	   - can occur - dealt with in efs_freeze, efs_unfreeze
 */

/*
 * efs_freeze -- Mark episode vnodes corresponding to specified device as
 * frozen so that OSF/1 does not recycle them.
 */

int efs_freeze(struct async_device *aggrDev)
{
    struct squeue *hashq, *evq;	   /* pointers to entries in hash table */
    struct evnode *evp;		   /* temporary evnode pointer */
    struct vnode *vp;	      /* temporary vnode pointer */
    int i;

    lock_ObtainWrite (&vntable_lock);
    for (i = 0; i < VNHASHSIZE; i++) {
	hashq = &vnhash[i];
	evq = QNext (hashq);
	while (evq != hashq) {
	    evp = HQTOV (evq);
	    vp = EVTOV(evp);
	    /* Process entry */
	    if ( ! (evp->vd_flags & VD_PHANTOM) ) {
		if ( ! (evp->flags & VD_NOIDENTITY) ) {
		    /*
		     * Do not need to do anything with NOIDENTITY vnodes here.
		     * They are dealt with in efs_reclaim.
		     *
		     * CLOSED vnodes do not have anode handles. So use vol
		     * handle to get to device always.  OPEN vnodes also need
		     * to be marked frozen in case they are closed before the
		     * aggregate is unfrozen.
		     */
		    if ( epia_AnodeDevice(evp->vd_volp) == aggrDev ) {
			VN_LOCK(vp);
			vp->v_flag |= V_FROZEN;
			VN_UNLOCK(vp);
		    }
		}  /* ! (evp->flags & VD_NOIDENTITY) */
	    } else {
		/*
		 * A phantomized vnode can be dephantomized,
		 * and closed before the aggregate is unfrozen.
		 * And if recycled, this would cause problems. Freeze all
		 * phantomized vnodes as the corresponding device cannot be
		 * found.
		 */
		VN_LOCK(vp);
		vp->v_flag |= V_FROZEN;
		VN_UNLOCK(vp);
	    }	/* evp->vd_flags & VD_PHANTOM */
	    evq = QNext (evq);
	}  /* while */
    } /* for */
    lock_ReleaseWrite (&vntable_lock);
}

/*
 * efs_unfreeze -- Clear all frozen episode vnodes corresponding to specified
 * device.
 *
 * NOTE: As a vnode can be phantomized, the corresponding fileset moved to
 * another device, and then dephantomized, and all phantomized vnodes have
 * to be marked frozen, I cannot unfreeze on the basis of per device and have
 * to unfreeze all vnodes. Otherwise  V_FROZEN bit would remain set in this
 * vnode and not removed unless this new device was frozen and unfrozen.
 * This effectively limits us to freezing only one aggregate at a time
 * unless the dephantomization code is changed to handle frozen vnodes.
 */

int efs_unfreeze(struct async_device *aggrDev)
{
    struct squeue *hashq, *evq;	   /* pointers to entries in hash table */
    struct evnode *evp;		   /* temporary evnode pointer */
    struct vnode *vp;		      /* temporary vnode pointer */
    int i;

    lock_ObtainWrite (&vntable_lock);
    for (i = 0; i < VNHASHSIZE; i++) {
	hashq = &vnhash[i];
	evq = QNext (hashq);
	while (evq != hashq) {
	    evp = HQTOV (evq);
	    /* Process entry */
	    vp=EVTOV(evp);
	    VN_LOCK(vp);
	    vp->v_flag &= (~V_FROZEN);
	    VN_UNLOCK(vp);
	    evq = QNext (evq);
	}
    }
    lock_ReleaseWrite (&vntable_lock);
}
#endif /* AFS_OSF_ENV */

#endif /* !EFS_PRIVATE_POOL */
