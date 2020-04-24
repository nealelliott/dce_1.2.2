/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * Copyright (C) 1996, 1990 Transarc Corporation
 * All rights reserved.
 */

/*			    Episode File System
 *			Volume operations and vector
 */
#include <dcedfs/param.h>
#include <dcedfs/sysincludes.h>
#include <dcedfs/osi_kvnode.h>

#include <dcedfs/osi_device.h>		/* osi_makedev */
/* XXX FIX ME XXX */
#undef	PRIVATE
#undef	SHARED

#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/osi_cred.h>		/* osi_GetRUID */
#include <dcedfs/osi_buf.h>		/* struct buf */
#include <dcedfs/queue.h>		/* struct squeue */
#include <dcedfs/volume.h>		/* struct volume */
#include <dcedfs/vol_init.h>		/* core operations */
#include <dcedfs/vol_errs.h>		/* VOLERR defs */
#include <dcedfs/xvfs_vnode.h>		/* VOPX, etc. Also struct afsFid from
					 * dcedfs/common_data.h */
#include <dcedfs/aclint.h>
#include <dcedfs/dir.h>			/* DIR_OFF_TO_PIECE() */
#include <dcedfs/icl.h>			/* in-core logging */
#include <dcedfs/bomb.h>		/* bomb points (error injection) */
#include <dcedfs/dacl.h>		/* dacl_epi_Validate error codes */
#include <dcedfs/episode/logbuf.h>	/* logTranRec_t */
#include <dcedfs/episode/tlock.h>	/* struct tlock_data */
#include <dcedfs/episode/aggregate.h>	/* struct epif_status */
#include <dcedfs/episode/file.h>	/* struct epif_status */
#include <dcedfs/episode/fid.h>		/* epi_volFileId_t */
#include <efs_kvol.h>			/* for user-space testing */
#include <efs_dirhandle.h>		/* struct epi_Dir_Handle */
#include <efs_evnode.h>			/* struct evnode */
#include <efs_volvnops.h>
#include <efs_vattr.h>
#include <efs_misc.h>
#include <efs_dir.h>
#include <efs_newalloc.h>
#include <efs_tmpcopy.h>
#include <efs_volio.h>
#include <dcedfs/episode/efs_trace.h>	/* ICL trace tags */

#ifdef AFS_OSF_ENV
#include <sys/specdev.h>		/* struct specinfo for EQUAL_DEV */
#endif

RCSID ("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/vnops/RCS/efs_volops.c,v 4.756 1996/03/25 21:07:27 cfe Exp $")

#define DEBUG_THIS_FILE EFS_DEBUG_VOLOPS

/*
 * Classify volops into read only, stop-use, and detach-vnode
 */
#define VOL_ALLOK_OPS (VOL_OP_GETSTATUS)
#define VOL_READONLY_OPS  (VOL_ALLOK_OPS | VOL_OP_GETATTR | \
			VOL_OP_GETACL | VOL_OP_SEEK | \
			VOL_OP_READ | VOL_OP_READHOLE | VOL_OP_TELL | \
			VOL_OP_SCAN | VOL_OP_READDIR | \
			VOL_OP_GETNEXTHOLES | VOL_OP_SETSTATUS | \
			VOL_OP_SYNC | VOL_OP_PUSHSTATUS)
/* These calls need to empty the VM system and must also detach the vnodes */
#define VOL_DETACH_OPS    (VOL_OP_SWAPIDS | VOL_OP_SETSTATUS_ID | \
			VOL_OP_CREATE | VOL_OP_DELETE | VOL_OP_DEPLETE | \
			VOL_OP_DESTROY | VOL_OP_CLONE | VOL_OP_RECLONE | \
			VOL_OP_UNCLONE | VOL_OP_SETNEWVID | VOL_OP_APPENDDIR| \
			VOL_OP_DETACH)
/* These calls will cause the VM system to be emptied of the fileset's data */
#define VOL_STOPUSE_OPS   (VOL_OP_CREATE | VOL_OP_WRITE | \
			VOL_OP_READ | VOL_OP_READHOLE | \
			VOL_OP_TRUNCATE | VOL_OP_DELETE | \
			VOL_OP_GETATTR | VOL_OP_SETATTR | \
			VOL_OP_GETACL | VOL_OP_SETACL | \
			VOL_OP_COPYACL | VOL_OP_APPENDDIR | \
			VOL_OP_DETACH)

/* Detect a restore operation */
#define VOL_SETINCON_OPS (VOL_OP_CREATE | VOL_OP_WRITE | VOL_OP_TRUNCATE | \
			  VOL_OP_DELETE | VOL_OP_SETACL | VOL_OP_SETNEWVID | \
			  VOL_OP_APPENDDIR)

#define VOL_RESTORE_OPS (VOL_OP_CREATE | VOL_OP_WRITE | \
			 VOL_OP_TRUNCATE | VOL_OP_DELETE | VOL_OP_SETATTR | \
			 VOL_OP_SETACL | VOL_OP_COPYACL | VOL_OP_APPENDDIR)

/* Flags to pass to StopUse for various volume ops */
#define STOPUSE_CLOSE	  0
#define STOPUSE_VNODE_RO (STOPUSE_NO_CHANGE|STOPUSE_NO_DIRTYVM)
#define STOPUSE_VNODE_RW (STOPUSE_NO_CHANGE|STOPUSE_NO_VM)
#define STOPUSE_RO \
    (STOPUSE_NO_CHANGE|STOPUSE_NO_DIRTYVM|STOPUSE_NO_DIRTY)
#define STOPUSE_LITE	 (STOPUSE_NO_CHANGE)
#define STOPUSE_HEAVY \
    (STOPUSE_NO_CHANGE|STOPUSE_NO_ANODE|STOPUSE_NO_STATUS|STOPUSE_NO_VM)

#define VOL_IS_ATLEAST_STOPUSE_RO(volp) \
    ((VTOEVLD(volp)->vld_openbits & STOPUSE_NO_CHANGE) && \
     (VTOEVLD(volp)->vld_openbits & (STOPUSE_NO_DIRTYVM | STOPUSE_NO_VM)))

#define VOL_IS_STOPUSE_HEAVY(volp) \
    ((VTOEVLD(volp)->vld_openbits & STOPUSE_HEAVY) == STOPUSE_HEAVY)

/* XXX Should put these in an include file */

#define EFSINVALIDINO		0
#define EFSROOTINO		1
#define VOL_EFSFIRSTINO		EFSROOTINO + 1

#define ExternalToEFSIndex(index) \
    (((index) < VOL_ROOTINO) ? EFSINVALIDINO : \
     (((index) == VOL_ROOTINO) ? EFSROOTINO : \
      ((index) + VOL_EFSFIRSTINO)))

/*
 * XXX Functions imported from efs_agops.c
 */
extern int ag_efsStatesToFlags(u_long, int32 *);
extern int ag_efsFlagsToStates(int32, u_long *);

/* Forward declarations */
static long TwiddleInconBit(struct volume *, int);

/*
 * GENERAL NOTE:
 * The vnode locking done by most volops is not really necessary since
 * higher level synchronization ensures that no one will modifying/reading
 * any information a volop will be reading/modifying. The locking however
 * prevent any assertions in lower level functions from going off.
 * This is applicable to most volops.
 */

/*
 * vol_efsOpen -- open a volume
 *
 * We initialize a data structure that can be used for iterations over
 * all anodes in a volume.
 */
static int vol_efsOpen (volp, type, errorType, handlerp)
    struct volume *volp;		/* volume */
    int type;				/* volume open type */
    int errorType;			/* error code for conflicting opens */
    struct vol_handle *handlerp;	/* iteration handle */
{
    struct efs_volStat *voldata;	/* private data of volp */
    long code = 0;
    int stopUseMode;

    osi_MakePreemptionRight();

    /* initialize iteration handle as required */
    handlerp->volp     = volp;
    handlerp->opentype = type;
    vol_MakeInvalidAfsFid(&(handlerp->fid));

    voldata = VTOEVLD (volp);
    voldata->vld_wholeVolProgress = 0;	/* reset this once in a while */

    /*
     * determine the level of concurrency allowed for vnode ops
     */
    if ((type & VOL_ALLOK_OPS) == type)	/* all vnops OK? */
	volp->v_concurrency = VOL_CONCUR_ALLOPS;
    else if ((type & VOL_READONLY_OPS) == type)/* ops are read-only */
	volp->v_concurrency = VOL_CONCUR_READONLY;
    else					/* some read-write ops */
	volp->v_concurrency = VOL_CONCUR_NOOPS;

    /* If all vnode operations are not allowed, quiesce the volume */
    if ((type & VOL_ALLOK_OPS) != type) {
	/*
	 * VOL_OP_SCAN is specified by dump and restore, which need
	 * to create vnodes. Hence stopuse heavy only if vnodes do not need to
	 * be created.
	 *
	 * A dump usually does not specify VOL_OP_SWAPIDS except during
	 * in an intra server fts move.
	 * A restore (efts) reqeust also specifies VOL_OP_SWAPIDS thru
	 * FTU_SYS_DESTROY but the SWAPIDS is never done at least thru
	 * efts restore.
	 * Detaches should always stopuse heavy.
	 */
	if ((type & VOL_OP_DETACH) ||
	    (type & VOL_OP_SWAPIDS && !(type & VOL_OP_SCAN)))
	    stopUseMode = STOPUSE_HEAVY;
	else
	    stopUseMode = STOPUSE_LITE;
	vnm_StopUse (volp, stopUseMode);
    }

    if (code == 0 && (type & VOL_SETINCON_OPS))
	code = TwiddleInconBit(volp, 1);

    osi_UnmakePreemptionRight();
    return code;
}

static int vol_efsSeek (volp, position, handlerp)
    struct volume *volp;		/* volume */
    int position;			/* anode index */
    struct vol_handle *handlerp;	/* iteration handle */
{
    return ENOSYS;			/* obsolete function */
}

static long TwiddleInconBit(struct volume *volp, int turnon)
{
    long code;

    icl_Trace2(efs_iclSet, EFS_TRACE_TWIDDLE_BEGIN,
		ICL_TYPE_LONG, AFS_hgetlo(volp->v_volId),
		ICL_TYPE_LONG, turnon);
    code = epiv_TwiddleInconBit (VTOVH (volp), turnon);

    if (!code)
	if (turnon)
	    volp->v_states |= VOL_DELONSALVAGE;
	else
	    volp->v_states &= ~VOL_DELONSALVAGE;

    if (code) {
	EFS_TRACE_VNOPS(DEBUG_THIS_FILE, code, EIO);
	code = EIO;
    }

    icl_Trace2(efs_iclSet2, EFS_TRACE_TWIDDLE_END,
		ICL_TYPE_LONG, AFS_hgetlo(volp->v_volId),
		ICL_TYPE_LONG, code);

    return code;
}

/*
 * vol_efsTell -- index of currently selected anode
 */

/* EXPORT */
int vol_efsTell (volp, handlerp, positionP)
    struct volume *volp;		/* volume */
    struct vol_handle *handlerp;	/* iteration handle */
    int *positionP;			/* place to put index */
{
    return ENOSYS;                      /* obsolete function */
}

/*
 * vol_efsScan -- get descriptor for currently selected anode
 */

/* EXPORT */
int vol_efsScan (volp, position, handlerp)
    struct volume *volp;		/* volume */
    int position;			/* index */
    struct vol_handle *handlerp;	/* iteration handle */
{
    int code;				/* error return code */
    struct efs_volStat *voldata;	/* private data of volp */
    epi_volume_t volh;
    u_long lastIndex;
    u_long lastFid;
    long gscode;			/* error code from epif_GetStatus */
    epi_anode_t ap;			/* anode handle */
    struct epif_status fstat;		/* file status info about ap */
    epi_volFileId_t fid;		/* fid for epif_Open */
    int stopUseMode;

    osi_MakePreemptionRight();

    /* set iteration handle fid to invalid in case of failure */
    vol_MakeInvalidAfsFid(&(handlerp->fid));

    afsl_Assert(handlerp->volp == volp);
    voldata = VTOEVLD (volp);
    volh = voldata->vld_vh;
    position = ExternalToEFSIndex (position);
    if (position == EFSINVALIDINO)
	code = EINVAL;
    else {
	code = epiv_LastIndex(volh, &lastIndex);
	if (code) {
	    EFS_TRACE_VNOPS(DEBUG_THIS_FILE, code, EIO);
	    code = EIO;
	} else {
	    lastFid = epid_LastIndex(lastIndex, epia_AnodeDevice(volh));
	}
    }
    if (code) {
	osi_UnmakePreemptionRight();
	return code;
    }

    if (!(handlerp->opentype & VOL_RESTORE_OPS)) {
	/* dump */
	if (position <= lastFid) {
	    /* dump in progress */
	    stopUseMode = STOPUSE_VNODE_RO;
	} else {
	    /* end of dump */
	    if (handlerp->opentype & VOL_OP_SWAPIDS)
		stopUseMode = STOPUSE_HEAVY;
	    else
		stopUseMode = STOPUSE_LITE;
	}
    } else {
	/* restore */
	stopUseMode = STOPUSE_VNODE_RW;
	/* XXX cannot detect end of restore */
    }

    vnm_StopUse (volp, stopUseMode);

    if (position > lastFid) {
	icl_Trace2(efs_iclSet2, EFS_TRACE_VOL_SCAN_EOF,
		   ICL_TYPE_LONG, AFS_hgetlo(volp->v_volId),
		   ICL_TYPE_LONG, position);
	code = VOL_ERR_EOF;
    } else {
	/* Find out if it's a user file */

	epid_MakeIndexOnly (&fid, position);
	code = epif_Open (VTOVH (volp), &fid, &ap);
	if (code == EPI_E_NOENT) {
	    icl_Trace2(efs_iclSet2, EFS_TRACE_VOL_SCAN_ENOENT,
		       ICL_TYPE_LONG, AFS_hgetlo(volp->v_volId),
		       ICL_TYPE_LONG, position);
	    code = VOL_ERR_DELETED;
	    /* set iteration handle fid to deleted as required */
	    vol_MakeDeletedAfsFid(&(handlerp->fid), position);
	} else {
	    afsl_MBZ (code);
	}
    }
    if (!code) {
	gscode = epif_GetStatus (ap, &fstat);
	code = epif_Close (ap);
	afsl_MBZ (code);
	if (gscode) {

	    /* If this fails it probably means that something is wrong with
	     * the auxiliary containers (ACLs).  This means the volume is
	     * probably INCONSISTENT.  It got that way, however, not via
	     * some interrupted restore but because of a partial clone,
	     * reclone, unclone, or destroy. */

	    /* Don't try to dump from or restore into this fileset! */

	    icl_Trace3(efs_iclSet, EFS_TRACE_SCAN_BAD_STATUS,
		       ICL_TYPE_LONG, AFS_hgetlo(volp->v_volId),
		       ICL_TYPE_LONG, position,
		       ICL_TYPE_LONG, gscode);
	    EFS_TRACE_VNOPS(DEBUG_THIS_FILE, gscode, EIO);
	    code = EIO;
	}
    }
    if (!code) {
	handlerp->fid.Vnode  = position;
	handlerp->fid.Unique = fstat.unique;

	icl_Trace2(efs_iclSet2, EFS_TRACE_VOL_SCAN,
		   ICL_TYPE_LONG, AFS_hgetlo(volp->v_volId),
		   ICL_TYPE_LONG, position);
    }
    osi_UnmakePreemptionRight();
    return code;
}

/*
 * vol_efsClose -- close a volume
 */

/* EXPORT */
int vol_efsClose (volp, handlerp, isabort)
    struct volume *volp;
    struct vol_handle *handlerp;
    int isabort;
{
    long code = 0;

    osi_MakePreemptionRight();
    /*
     * Wait for all data written from the volume operation to be
     * synced to disk.
     */
    if (handlerp->opentype & VOL_OP_WRITE)
	vol_efsWaitAll();

    if (!isabort && (handlerp->opentype & VOL_SETINCON_OPS))
	code = TwiddleInconBit(volp, 0);

    /* If all vnode operations were not allowed during volume op,
     * un-quiesce the volume */
    if ((handlerp->opentype & VOL_ALLOK_OPS) != handlerp->opentype) {
	vnm_StopUse (volp, STOPUSE_CLOSE);	/* return to service */
#ifdef AFS_AIX31_VM
	/* Episode VMM code may have queued pagein buf requests, since
	 * episode strategy daemon is earliest place on AIX where we notice
	 * a strategy call that references an offline volume.  Can't
	 * check earlier because we're at interrupt level and can't
	 * obtain locks.
	 */
	efs_WakeupBlockedPageIns(volp);
#endif /* AFS_AIX31_VM */
    }

    icl_Trace3(efs_iclSet2, EFS_TRACE_EFSCLOSE,
		ICL_TYPE_LONG, AFS_hgetlo(volp->v_volId),
		ICL_TYPE_LONG, isabort,
		ICL_TYPE_LONG, code);

    osi_UnmakePreemptionRight();
    return code;
}

/*
 * Define how many iterations the anode-level ops are to make (without doing
 * anything) before returning to user space.  This value used to be 1000, but
 * we encountered RPC timeouts on big no-op reclone operations.
 */
#define EPI_VNOPS_MAX_INKERNEL_LOOP 75

/*
 * vol_efsDeplete -- deplete a volume, one anode at a time.  We use an
 *     iteration mechanism that runs for EPI_VNOPS_MAX_INKERNEL_LOOP indexes
 *     or until it deletes an anode.  Then it returns early with a
 *     distinguished error code (ELOOP), which means that the caller should
 *     retry.  The current progress of the deplete operation is maintained in
 *     the progress field of the voldata structure.
 *
 * RETURN CODES -- Unexpected errors are returned as EIO.  Iteration is
 *     triggered by returning ELOOP.
 */

/* EXPORT */
int vol_efsDeplete (volp)
  struct volume *volp;
{
    int code = 0;			/* error return code */
    long ccode;
    struct efs_volStat *voldata;	/* private data for volp */
    epi_volume_t volh;			/* volume handle from voldata */
    epi_anode_t ap;			/* anode to be deleted */
    elbt_TranRec_t transId;		/* transaction ID */
    afs_hyper_t h0;				/* zero length to pass to Truncate */
    u_long index;			/* epiv-style index we're working on */

    lock_ObtainRead(&volp->v_lock);
    if (volp->v_states & VOL_LCLMOUNT) {
	lock_ReleaseRead(&volp->v_lock);
	return EBUSY;
    }
    lock_ReleaseRead(&volp->v_lock);

    osi_MakePreemptionRight();
    AFS_hzero (h0);
    transId = buffer_nullTranId;
    voldata = VTOEVLD (volp);
    volh = voldata->vld_vh;

    code = 0;
    ap = 0;

    index = voldata->vld_wholeVolProgress;
    if (index == 0) {
	index = 1;		/* start from the beginning */
	icl_Trace1(efs_iclSet, EFS_TRACE_EFSDEPLETE_1ENTER,
		   ICL_TYPE_LONG, AFS_hgetlo(volp->v_volId));

	vnm_StopUse (volp, STOPUSE_HEAVY);

	/*
	 * The last index could have changed since the fileset was opened, so
	 * we update our cached copy.
	 */
	if (code = epiv_LastIndex(volh, &voldata->vld_lastIndex))
	    goto out;

	if (code = TwiddleInconBit(volp, 1))
	    goto out;

	icl_Trace1(efs_iclSet2, EFS_TRACE_EFSDEPLETE_1EXIT,
		   ICL_TYPE_LONG, AFS_hgetlo(volp->v_volId));
    }

    for (;index <= voldata->vld_lastIndex; index++) {
	code = epiv_OpenAnode (volh, index, &ap);
	if (!code) break;
	ap = 0;
	if (code != EPI_E_BADANODEINDEX) {
	    goto out;
	}
	code = 0;
	if (index >
	    voldata->vld_wholeVolProgress+EPI_VNOPS_MAX_INKERNEL_LOOP) {
	    /* return from kernel periodically, even if nothing is happening */
	    code = ELOOP;
	    goto out;
	}
    }

    if (ap) {
	/* we found an anode, so delete it */
	code = epia_Truncate (buffer_nullTranId, ap, 0, h0);
	if (code) {
	    (void) epiv_CloseAnode(ap);
	    goto out;
	}
	/* See whether epia_Truncate() did its job, according to Ted */
	if (!AFS_hiszero(epia_GetLength(ap))
	    || epia_GetAllocated(ap) != 0) {
	    (void) epiv_CloseAnode(ap);
	    goto out;
	}
	ccode = epia_StartTran ("vol_efsDeplete", volh, &transId);
	afsl_MBZ (ccode);
	code = BOMB_EXEC("vol_efsDeplete.1", epiv_DeleteAnode (transId, ap));
	ccode = epia_EndTran (volh, transId);
	afsl_MBZ (ccode);
	if (code) {
	    (void) epiv_CloseAnode(ap);
	    goto out;
	}
	code = ELOOP;			/* go around again */
    }

out:
    if (code == ELOOP) {
	voldata->vld_wholeVolProgress = index+1;
    } else {
	voldata->vld_wholeVolProgress = 0;
	icl_Trace2(efs_iclSet2, EFS_TRACE_VOL_DEPLETE_DONE,
		   ICL_TYPE_LONG, AFS_hgetlo(volp->v_volId),
		   ICL_TYPE_LONG, code);
	if (code) {
	    EFS_TRACE_VNOPS(DEBUG_THIS_FILE,code,EIO);
	    code = EIO;
	}
    }
    osi_UnmakePreemptionRight();
    return code;
}

/*
 * vol_efsDestroy -- Destroy an empty volume.
 */
int vol_efsDestroy (volp)
    struct volume *volp;
{
    int code;
    long ccode;
    elbt_TranRec_t transId;		/* transaction ID */
    struct efs_volStat *voldata;	/* private data for volp */
    epi_volume_t volh;			/* volume handle from voldata */
    epi_anode_t avl;			/* avl for volume */

    lock_ObtainRead(&volp->v_lock);
    if (volp->v_states & VOL_LCLMOUNT) {
	lock_ReleaseRead(&volp->v_lock);
	return EBUSY;
    }
    lock_ReleaseRead(&volp->v_lock);

    osi_MakePreemptionRight();
    voldata = VTOEVLD (volp);
    volh = voldata->vld_vh;

    vnm_StopUse (volp, STOPUSE_HEAVY);

    /* The volume should be empty, so finish it off */
    code = epiv_Deflate (volh);
    if (!code) {

	/* avl will stay valid after volume is deleted.  Use it for EndTran */

	ccode = epiv_GetAggregate (volh, &avl);
	afsl_MBZ (ccode);
	ccode = epia_StartTran ("vol_efsDestroy", avl, &transId);
	afsl_MBZ (ccode);
	code = epig_DeleteVolume (transId, volh);
	ccode = epia_EndTran (avl, transId);
	afsl_MBZ (ccode);
	voldata->vld_vh = (epi_volume_t) 0;
    }
    if (code) {
	EFS_TRACE_VNOPS(DEBUG_THIS_FILE,code,EIO);
	code = EIO;
    }
    osi_UnmakePreemptionRight();
    return code;
}

/*
 * vol_efsGetStatus -- Get volume status
 *
 * NOTE: Do not need to stopuse the volume
 *
 * LOCKS USED -- we leave preemption disabled most of the time since we're
 *     fetching things out of the volp.  Just reenable it when calling the
 *     lower levels of Episode. */

/* EXPORT */
int vol_efsGetStatus (volp, statusp)
    struct volume *volp;		/* volume */
    struct vol_status *statusp;		/* status data area */
{
    struct efs_volStat *voldata;	/* private data of volp */
    epi_volume_t volh;			/* volume handle */
    struct epiv_status vstat;		/* volume status info about volp */
    int logFragSize;			/* log-base-2 of the fragment size
					 * in bytes */
    long code = 0;
    struct async_device *as_dev;	/* disk device containing anode */
    u_long lastIndex;
    u_long lastFid;

    /* This can be called without the private data pointer in the
     * xvolume volume pointer set, essentially to get the vol id and vol
     * name. In this case this information can be got from the passed in
     * volp itself, on which an ag_efsVolInfo was done that filled in
     * relevant information in the xvolume volume structure. Hence
     * no need to call StopUse. */

    voldata = VTOEVLD (volp);
    if (voldata) {
	/* The volume is open at least in STOPUSE_LITE mode here. */

	osi_MakePreemptionRight();
	volh = voldata->vld_vh;
	code = epiv_GetStatus (volh, &vstat);
	if (!code) {
	    as_dev = epia_AnodeDevice (volh);
	    logFragSize = as_dev->logFragSize;
	    afsl_Assert(AFS_hsame(vstat.ident.id, volp->v_volId));

	    /* Calculate value for lastFid */
	    code = epiv_LastIndex(volh, &lastIndex);
	}
	if (!code) {
	    lastFid = epid_LastIndex(lastIndex, as_dev);
	} else {
	    EFS_TRACE_VNOPS(DEBUG_THIS_FILE, code, EIO);
	}
	osi_UnmakePreemptionRight();

	if (code)
	    return EIO;
    }

    statusp->vol_st = volp->v_stat_st;	/* structure assignment */
    if (voldata) {			/* update states */
	statusp->vol_st.states = volp->v_states;
	(void) ag_efsFlagsToStates (vstat.flags, &statusp->vol_st.states);
	statusp->vol_st.parentId = vstat.upLevelIds[1];
	statusp->vol_st.cloneTime.sec = vstat.upLevelTime1;
	statusp->vol_st.cloneTime.usec = 0;
	statusp->vol_st.vvCurrentTime.sec = vstat.upLevelTime3;
	statusp->vol_st.vvCurrentTime.usec = 0;
	/* Copy these out also, so that callers know what's really on the disk. */
	statusp->vol_st.volId = vstat.ident.id;
	bcopy(vstat.ident.name, statusp->vol_st.volName, VOLNAMESIZE);
    }

    bzero ((caddr_t) &statusp->vol_dy, sizeof (struct vol_stat_dy));
    if (voldata) {
	statusp->vol_dy.creationDate.sec = vstat.createTime;
	statusp->vol_dy.updateDate.sec = vstat.modTime;
	statusp->vol_dy.accessDate.sec = vstat.accessTime;
	statusp->vol_dy.copyDate.sec = vstat.upLevelTime2;
	;					/* backupDate? */
	statusp->vol_dy.volversion = vstat.version;
	statusp->vol_dy.backupId = vstat.upLevelIds[2];
	statusp->vol_dy.cloneId = vstat.upLevelIds[3];
	statusp->vol_dy.llBackId = vstat.backingVolId;
	statusp->vol_dy.llFwdId = vstat.upLevelIds[0];
	;					/* filecount? */

	/* Convert each vstat field in fragments to the corresponding vol_dy
	 * field in bytes. */
	AFS_hset64(statusp->vol_dy.allocLimit, 0, vstat.allocated.limit);
	AFS_hleftshift(statusp->vol_dy.allocLimit, logFragSize);
	AFS_hset64(statusp->vol_dy.allocUsage, 0, vstat.allocated.usage);
	AFS_hleftshift(statusp->vol_dy.allocUsage, logFragSize);
	AFS_hset64(statusp->vol_dy.visQuotaLimit, 0, vstat.visible.limit);
	AFS_hleftshift(statusp->vol_dy.visQuotaLimit, logFragSize);
	AFS_hset64(statusp->vol_dy.visQuotaUsage, 0, vstat.visible.usage);
	AFS_hleftshift(statusp->vol_dy.visQuotaUsage, logFragSize);

	;					/* minQuota? */
	;					/* owner? */
	statusp->vol_dy.unique = vstat.unique;
	statusp->vol_dy.index = epia_GetIndex(volh);
	;					/* rwIndex? */
	;					/* backupIndex? */
	statusp->vol_dy.parentIndex = vstat.backingVolIndex;
	;					/* cloneIndex? */
	statusp->vol_dy.nodeMax = (lastFid == EFSROOTINO ?
				   VOL_ROOTINO
				   : (lastFid - VOL_EFSFIRSTINO));
	statusp->vol_dy.aggrId = volp->v_paggrp->a_aggrId;
	bcopy(vstat.statusMsg, statusp->vol_dy.statusMsg,
	      sizeof (vstat.statusMsg));

	icl_Trace2(efs_iclSet2, EFS_TRACE_VOL_GETSTATUS,
		   ICL_TYPE_LONG, AFS_hgetlo(volp->v_volId),
		   ICL_TYPE_LONG, AFS_hgetlo(vstat.version));

    }
    return 0;
}

/*
 * SetStatus -- Helper for vol_efsSetStatus and vol_efsBulkSetStatus.
 *
 * CAUTION: Cannot call stopuse on the volume as the xvolume layer write
 * locked the volume before calling the volume operation that called us
 * and stopuse can result in glued vnode rele's which now needs to grab
 * the volume lock.
 */
static int SetStatus(
    struct volume *volp,		/* volume */
    int mask,				/* which status fields to set */
    struct vol_status *statusp,		/* status data area */
    elbt_TranRec_t transIdParm)	/* transaction ID, possibly null */
{
    epi_volume_t volh;			/* volume handle from volp */
    register unsigned long emask = 0;	/* mask for Episode */
    elbt_TranRec_t transId;		/* transaction ID */
    struct epiv_status vstat;		/* status info about volh */
    int code = 0, code2;
    int logFragSize;			/* log_2 of fragment size in bytes */
    afs_hyper_t hDum;
    unsigned long fragMask;		/* fragsize - 1 */
    register unsigned long IDsMask;
    register unsigned long newstates;
    register unsigned long stc;

    /* Ensure that only one of VOL_RW and VOL_READONLY is set.
     * Plagiarized from efs_agops.c */
    if (mask & VOL_STAT_STATES) {
	stc = statusp->vol_st.states & (VOL_RW | VOL_READONLY);
	if (stc == 0 || stc == (VOL_RW | VOL_READONLY))
	    return EINVAL;
    }

    /* Check to see that if heavyweight stopuse is required, it has already
     * been done */
    if ((mask & VOL_STAT_VOLID) ||
	((mask & VOL_STAT_STATES) &&
	 (stc != (volp->v_states & (VOL_RW | VOL_READONLY)))) ||
	 ((mask & VOL_STAT_TYPE) &&
	  (volp->v_voltype != statusp->vol_st.type)))
	osi_assert(VOL_IS_STOPUSE_HEAVY(volp));

    volh = VTOVH (volp);
    if (transIdParm == buffer_nullTranId) {
	code = epia_StartTran ("vol_efsSetStatus", volh, &transId);
	afsl_MBZ(code);
    } else {
	transId = transIdParm;
    }

#define	VOL_STAT_ALLIDS	(VOL_STAT_PARENTID \
    | VOL_STAT_BACKUPID \
    | VOL_STAT_CLONEID \
    | VOL_STAT_LLFWDID)

    /* The epiv_SetVolume interface lets us set all four IDs or just one. */
    /* If our caller wants 2 or 3, we first get the status and then set */
    /* them all. */
    IDsMask = (mask & VOL_STAT_ALLIDS);
    if (IDsMask != 0) {	/* setting any of them? */
	emask |= (EPIV_STATUS_UPLEVELIDS);
	mask &= ~VOL_STAT_ALLIDS;	/* handling these here! */

	if (IDsMask == VOL_STAT_ALLIDS) {	/* just setting them all? */
	    vstat.upLevelIds[0] = statusp->vol_dy.llFwdId;
	    vstat.upLevelIds[1] = statusp->vol_st.parentId;
	    vstat.upLevelIds[2] = statusp->vol_dy.backupId;
	    vstat.upLevelIds[3] = statusp->vol_dy.cloneId;
	    volp->v_parentId = statusp->vol_st.parentId;
	} else {
	    /* Setting just a few.  Fetch the status and then set some. */
	    if (code = epiv_GetStatus(volh, &vstat)) {
		EFS_TRACE_VNOPS(DEBUG_THIS_FILE,code,EIO);
		code = EIO;
		goto done;
	    }

	    if (IDsMask & VOL_STAT_PARENTID) {
		volp->v_parentId = statusp->vol_st.parentId;
		vstat.upLevelIds[1] = statusp->vol_st.parentId;
	    }

	    if (IDsMask & VOL_STAT_BACKUPID)
		vstat.upLevelIds[2] = statusp->vol_dy.backupId;

	    if (IDsMask & VOL_STAT_CLONEID)
		vstat.upLevelIds[3] = statusp->vol_dy.cloneId;

	    if (IDsMask & VOL_STAT_LLFWDID)
		vstat.upLevelIds[0] = statusp->vol_dy.llFwdId;
	}
    }
    if (mask & VOL_STAT_VOLID) {
	/* XXX should this also set volp->v_volId? ans: NO! */
	vstat.ident.id = statusp->vol_st.volId;
	emask |= EPIV_STATUS_ID;
    }
    if (mask & VOL_STAT_LLBACKID) {
	vstat.backingVolId = statusp->vol_dy.llBackId;
	emask |= EPIV_STATUS_BACKING_ID;
    }
    if (mask & VOL_STAT_CLONETIME) {
	volp->v_cloneTime.sec = statusp->vol_st.cloneTime.sec;
	volp->v_cloneTime.usec = 0;	/* keep only what's stored on disk */
	vstat.upLevelTime1 = statusp->vol_st.cloneTime.sec;
	emask |= EPIV_STATUS_UPLEVELTIME_1;
    }
    if (mask & VOL_STAT_VVCURRTIME) {
	volp->v_vvCurrentTime.sec = statusp->vol_st.vvCurrentTime.sec;
	volp->v_vvCurrentTime.usec = 0;	/* keep only what's stored on disk */
	vstat.upLevelTime3 = statusp->vol_st.vvCurrentTime.sec;
	emask |= EPIV_STATUS_UPLEVELTIME_3;
    }
    if (mask & VOL_STAT_VVPINGCURRTIME) {
	volp->v_vvPingCurrentTime = statusp->vol_st.vvPingCurrentTime;
    }
    if (mask & VOL_STAT_TYPE) {
	volp->v_voltype = statusp->vol_st.type;
    }
    if (mask & VOL_STAT_STATES) {
	/* Set only those bits that the kernel doesn't maintain itself. */
	/* New values for the user bits: */
	newstates = statusp->vol_st.states & ~(VOL_BITS_NOSETSTATUS);
	/* Old values for the kernel bits: */
	newstates |= (volp->v_states & VOL_BITS_NOSETSTATUS);
	/* start with 'em all clear, then set from states */
	vstat.flags = 0;
	code = ag_efsStatesToFlags (newstates, &vstat.flags);
	if (code) {
	    EFS_TRACE_VNOPS(DEBUG_THIS_FILE,code,EINVAL);
	    code = EINVAL;
	    goto done;
	}
	emask |= EPIV_STATUS_FLAGS;
	volp->v_states = newstates;
    }
    if (mask & VOL_STAT_RECLAIMDALLY) {
	volp->v_reclaimDally = statusp->vol_st.reclaimDally;
    }
    if (mask & VOL_STAT_VOLMOVETIMEOUT) {
	volp->v_stat_st.volMoveTimeout = statusp->vol_st.volMoveTimeout;
    }
    if (mask & VOL_STAT_VOLNAME) {
	/* XXX should this also set volp->v_volName? ans: NO! */
	bcopy(statusp->vol_st.volName,vstat.ident.name,
	      sizeof(vstat.ident.name));
	emask |= EPIV_STATUS_NAME;
    }
    /* No interface to store these date fields except at creation time. */
    ;					/* creationDate */
    ;					/* updateDate */
    ;					/* accessDate */
    /* No interface to store backupDate ever. */
    ;					/* backupDate */
    if (mask & VOL_STAT_COPYDATE) {
	vstat.upLevelTime2 = statusp->vol_dy.copyDate.sec;
	emask |= EPIV_STATUS_UPLEVELTIME_2;
    }
    if (mask & VOL_STAT_VERSION) {
	vstat.version = statusp->vol_dy.volversion;
	emask |= EPIV_STATUS_VERSION;
    }

    /* Round up these byte quantities to units of fragments.  Anode layer still
     * these quota values in longs so use the maximum possible value if result
     * doesn't fit in 32bits. */

    logFragSize = (epia_AnodeDevice (volh))->logFragSize;
    fragMask = epia_FragmentSize (volh) - 1;
    if (mask & VOL_STAT_ALLOCLIMIT) {
	hDum = statusp->vol_dy.allocLimit;
	/* round up to the fragment size containing allocLimit bytes */
	AFS_hadd32(hDum, fragMask);
	AFS_hrightshift(hDum, logFragSize);
	if (!AFS_hfitsinu32(hDum)) {
	    /* Use VOL_MAX_QUOTA instead, it is guaranteed to be
	     * evenly divisible by 64K (our largest fragsize) and to fit
	     * in 32 bits when divided by 1K (our smallest fragsize). */
	    AFS_hset64(hDum, VOL_MAX_QUOTA_HIGH, VOL_MAX_QUOTA_LOW);
	    AFS_hrightshift(hDum, logFragSize);
	}
	vstat.allocated.limit = AFS_hgetlo(hDum);
	emask |= EPIV_STATUS_Q_ALLOCATED;
    }
    if (mask & VOL_STAT_VISLIMIT) {
	hDum = statusp->vol_dy.visQuotaLimit;
	AFS_hadd32(hDum, fragMask);
	AFS_hrightshift(hDum, logFragSize);
	if (!AFS_hfitsinu32(hDum)) {
	    /* Use VOL_MAX_QUOTA instead, it is guaranteed to be
	     * evenly divisible by 64K (our largest fragsize) and to fit
	     * in 32 bits when divided by 1K (our smallest fragsize). */
	    AFS_hset64(hDum, VOL_MAX_QUOTA_HIGH, VOL_MAX_QUOTA_LOW);
	    AFS_hrightshift(hDum, logFragSize);
	}
	vstat.visible.limit = AFS_hgetlo(hDum);
	emask |= EPIV_STATUS_Q_VISIBLE;
    }
    ;					/* minQuota */
    ;					/* owner */
    if (mask & VOL_STAT_UNIQUE) {
	vstat.unique = statusp->vol_dy.unique;
	emask |= EPIV_STATUS_UNIQUE;
    }
    ;					/* index */
    ;					/* rwIndex */
    ;					/* backupIndex */
    ;					/* parentIndex */
    ;					/* cloneIndex */
    if (mask & VOL_STAT_STATUSMSG) {
	bcopy(statusp->vol_dy.statusMsg,
	      vstat.statusMsg,
	      sizeof(vstat.statusMsg));
	emask |= EPIV_STATUS_MSG;
    }

    if (emask) {
	code = epiv_SetStatus (transId, volh, emask, &vstat);
	afsl_MBZ(code);
    }
done:
    if (transIdParm == buffer_nullTranId) {
	code2 = epia_EndTran (volh, transId);
	afsl_MBZ(code2);
    }
    icl_Trace4(efs_iclSet2, EFS_TRACE_VOL_SETSTATUS,
	       ICL_TYPE_LONG, code,
	       ICL_TYPE_LONG, AFS_hgetlo(vstat.ident.id),
	       ICL_TYPE_LONG, volp->v_states,
	       ICL_TYPE_LONG, AFS_hgetlo(vstat.version));

    return (code);
}

/* SwapPrivateData -- does the fsDatap fixup after a pair of volumes have had
 *     their ids swapped.  The basic rule here is that the volp is permanently
 *     associated with a particular volume id.  To preserve correctness we keep
 *     information tied to the volume id where it is and exchange information
 *     related to the physical representation.
 *
 * CAUTIONS -- we presently rely on the fact that the caller (in
 *     xvolume/vol_misc.c) swaps the fsDatap pointers.  So we unswap some of
 *     the fields of those structures so the results is what we want. */

#define SWAPFIELD(field) \
  ((tmp_##field = vol1data->vld_##field), \
   (vol1data->vld_##field = vol2data->vld_##field), \
   (vol2data->vld_##field = tmp_##field))

static void SwapPrivateData (
  struct volume *vol1p,
  struct volume *vol2p)
{
    struct efs_volStat *vol1data = VTOEVLD(vol1p);
    struct efs_volStat *vol2data = VTOEVLD(vol2p);
    int tmp_openbits, tmp_syncMeta;
    struct osi_vfs *tmp_vfs;
    struct vnode *tmp_root;

    if (vol1p->v_paggrp != vol2p->v_paggrp) {
	/*
	 * For each volume, if it is mounted reduce the local mount counter on
	 * old parent aggr and increase count of local mounts on new parent
	 * aggr.  If they are the same aggr, no need to change (or lock) them.
	 *
	 * Lock aggrs in order of pointer value to avoid deadlock problems.
	 */
	if (vol1p->v_paggrp > vol2p->v_paggrp) {
	    lock_ObtainWrite(&vol1p->v_paggrp->a_lock);
	    lock_ObtainWrite(&vol2p->v_paggrp->a_lock);
	} else {
	    lock_ObtainWrite(&vol2p->v_paggrp->a_lock);
	    lock_ObtainWrite(&vol1p->v_paggrp->a_lock);
	}

	if (!EPI_ISNULLVFSP(VTOVFS(vol1p))) {
	    osi_assert(vol1p->v_paggrp->a_localMounts > 0);
	    vol1p->v_paggrp->a_localMounts--;
	    vol2p->v_paggrp->a_localMounts++;
	}
	if (!EPI_ISNULLVFSP(VTOVFS(vol2p))) {
	    osi_assert(vol2p->v_paggrp->a_localMounts > 0);
	    vol2p->v_paggrp->a_localMounts--;
	    vol1p->v_paggrp->a_localMounts++;
	}

	lock_ReleaseWrite(&vol1p->v_paggrp->a_lock);
	lock_ReleaseWrite(&vol2p->v_paggrp->a_lock);
    }
    SWAPFIELD(openbits);
    SWAPFIELD(vfs);
    SWAPFIELD(syncMeta);
#ifndef AFS_AIX_ENV
    /* XXX -- there is a race here between swapping the VFS's and the
     *     corresponding root vnodes.  We should be holding some vfs lock
     *     accross this or something. */
    SWAPFIELD(root);
#endif
    /* the vld_roVolume field is recalculated on every vol_close */
}

/*
 * vol_efsSetStatus -- Set volume status
 *
 * CAUTION: Cannot call stopuse. Refer comment in SetStatus.
 */

/* EXPORT */
int vol_efsSetStatus (volp, mask, statusp)
    struct volume *volp;		/* volume */
    int mask;				/* which status fields to set */
    struct vol_status *statusp;		/* status data area */
{
    int code;

    osi_MakePreemptionRight();
    code = SetStatus(volp, mask, statusp, buffer_nullTranId);
    osi_UnmakePreemptionRight();
    return code;
}

/*
 * vol_efsBulkSetStatus -- Set more than one volume's status under a
 *	single transaction.
 *
 * CAUTION: Cannot call stopuse. Refer comment in SetStatus.
 */
/* EXPORT */
int vol_efsBulkSetStatus(arrayLen, statusArray)
    unsigned arrayLen;
    struct vol_statusDesc *statusArray;
{
    unsigned i, j, k;
    long code, code2;
    elbt_TranRec_t transId;
    struct volume *volp;
    epi_anode_t avlh;
    epi_volume_t volh;
    struct savedStatus {
	epi_anode_t		avlh;
	epi_volume_t		volh;
	struct epiv_status	vstat;
    } *savedStatusArray;
    unsigned savedStatusSize = arrayLen * sizeof(struct savedStatus);
    epi_anode_t	avlArray[VOL_MAX_BULKSETSTATUS];
    unsigned numAvls = 0;
    struct volume *vol1p = 0, *vol2p = 0;

    osi_MakePreemptionRight();
    if (savedStatusSize <= osi_BUFFERSIZE)
	savedStatusArray = osi_AllocBufferSpace_r();
    else
	savedStatusArray = osi_Alloc_r(savedStatusSize);

    bzero((char*)avlArray, sizeof avlArray);

    for (i = 0; i < arrayLen; i++) {
	volp = statusArray[i].vsd_volp;
	volh = VTOVH (volp);

	code = epiv_GetAggregate(volh, &avlh);
	if (code)
	    goto free_array;

	if ((statusArray[i].vsd_mask & VOL_STAT_VOLID) &&
	    !AFS_hsame(volp->v_volId, statusArray[i].vsd_status.vol_st.volId)) {
	    if (vol1p == 0)
		vol1p = volp;
	    else if (vol2p == 0)
		vol2p = volp;
	    else {
		/* too many ID-swaps */
		code = EINVAL;
		goto free_array;
	    }
	}

	savedStatusArray[i].avlh = avlh;
	savedStatusArray[i].volh = volh;
	if (code = epiv_GetStatus(volh, &savedStatusArray[i].vstat))
	    goto free_array;

	/*
	 * See if we've seen a fileset on this aggregate yet.
	 */
	for (j = 0; j < numAvls; j++)
	    if (avlh == avlArray[j])
		break;

	/* If this is a new aggregate, remember it */
	if (j >= numAvls) {
	    /* There can't be more aggregates than status descriptors */
	    osi_assert(numAvls < arrayLen);

	    avlArray[numAvls] = avlh;
	    numAvls++;
	}
    }

    if (vol1p != 0) {
	if (vol2p == 0) {
	    /* Only 1 id-swap, not 0 or 2 */
	    code = EINVAL;
	    goto free_array;
	}
	osi_assert(VOL_IS_STOPUSE_HEAVY(vol1p));
	osi_assert(VOL_IS_STOPUSE_HEAVY(vol2p));
    }

    /* There must be at least one aggregate involved */
    osi_assert(numAvls > 0);

    /*
     * For each aggregate, start a new transaction and set the status of
     * each fileet on that aggregate.
     */
    for (i = 0; i < numAvls; i++) {
	osi_assert(avlArray[i] != NULL);

	code = epia_StartTran("vol_efsBulkSetStatus", avlArray[i], &transId);
	afsl_MBZ(code);

	/*
	 * Find each fileset on this aggregate
	 */
	for (j = 0; j < arrayLen; j++)
	    if (savedStatusArray[j].avlh == avlArray[i]) {
		code = SetStatus(statusArray[j].vsd_volp,
				 statusArray[j].vsd_mask,
				 &statusArray[j].vsd_status,
				 transId);
		if (code)
		    break;
	    }

	if (code) {
	    /*
	     * If there was a failure, we attempt to restore the volumes
	     * that were updated under the current transaction.  There is
	     * similar cleanup code below that restores the status of
	     * filesets that were updated under previous transactions.
	     *
	     * We failed on the j'th fileset, so we restore all filesets
	     * before the j'th one that are also on the same aggregate.
	     *
	     * If one of the restores fails, then we've violated our
	     * atomicity guarantee, so we panic.
	     */
	    for (k = 0; k < j; k++)
		if (savedStatusArray[k].avlh == avlArray[i]) {
		    code2 = epiv_SetStatus(transId, savedStatusArray[k].volh,
					   -1, &savedStatusArray[k].vstat);
		    afsl_MBZ(code2);
		}
	}

	code2 = epia_EndTran(avlArray[i], transId);
	afsl_MBZ(code2);
    }

    if (code) {
	/*
	 * If there was a failure, we attempt to restore the volumes that
	 * were updated under earlier transactions.
	 *
	 * We failed while updating filesets on the i'th aggregate (avl).
	 * Since the cleanup code above already restored any filesets on
	 * that aggregate, we restore the filesets on aggregates before the
	 * i'th one.
	 *
	 * If one of the restores fails, then we've violated our atomicity
	 * guarantee, so we panic.
	 */
	for (j = 0; j < i; j++) {
	    code2 = epia_StartTran("vol_efsBulkStatus cleanup",
				   avlArray[j], &transId);
	    afsl_MBZ(code2);

	    for (k = 0; k < arrayLen; k++)
		if (savedStatusArray[k].avlh == avlArray[j]) {
		    code2 = epiv_SetStatus(transId, savedStatusArray[k].volh,
					   -1, &savedStatusArray[k].vstat);
		    afsl_MBZ(code2);
		}

	    code2 = epia_EndTran(avlArray[j], transId);
	    afsl_MBZ(code2);
	}
    }

free_array:
    if (savedStatusSize <= osi_BUFFERSIZE)
	osi_FreeBufferSpace_r((struct osi_buffer *)savedStatusArray);
    else
	osi_Free_r(savedStatusArray, savedStatusSize);

    osi_UnmakePreemptionRight();

    if (code == 0 && vol1p && vol2p)
	SwapPrivateData (vol1p, vol2p);

    return code;
}

/*
 * vol_efsCreate -- create file within volume
 */
int vol_efsCreate (volp, position, xvattrp, handlerp, credp)
    struct volume *volp;		/* volume */
    int position;			/* index */
    struct xvfs_attr *xvattrp;		/* attributes of file */
    struct vol_handle *handlerp;	/* iteration handle */
    osi_cred_t *credp;			/* credential structure */
{
    struct evnode *evp;			/* Episode vnode */
    struct efs_volStat *voldata;	/* private data of volp */
    epi_volume_t volh;			/* volume handle */
    elbt_TranRec_t transId;		/* transaction ID */
    epi_volFileId_t fid;		/* file ID */
    epi_anode_t ap;			/* anode handle */
    u_int32 unique;			/* unique of file/dir being restored */
    int code = 0;			/* error return code */
    long ccode = 0;
    epi_anode_t file;
    struct afsFid afsFid;
    afs_hyper_t hlen;

    osi_MakePreemptionRight();
    afsl_Assert(handlerp->volp == volp);
    voldata = VTOEVLD (volp);
    volh = voldata->vld_vh;
    position = ExternalToEFSIndex(position);
    icl_Trace2(efs_iclSet2, EFS_TRACE_VOL_CREATE,
		ICL_TYPE_LONG, AFS_hgetlo(volp->v_volId),
	       ICL_TYPE_LONG, position);

    if (position == EFSINVALIDINO) {
	code = EINVAL;
	goto done;
    }

    /*
     * If the passed in index already contains a file with a different
     * uniqifier, delete the file
     */
    unique = AFS_hgetlo(xvattrp->xvattr.fileID);
    epid_MakeIndexOnly(&fid, position);
    code = epif_Open(VTOVH(volp), &fid, &ap);
    if (code == 0) {
	/*
	 * Should be safe to call lightweight epif_GetUnique instead of
	 * epif_GetStatus.
	 */
	u_int32 dUnique = epif_GetUnique(ap);
	int delete = (dUnique != unique);

	icl_Trace2(efs_iclSet2, EFS_TRACE_VOL_CREATE_EXISTING,
		ICL_TYPE_LONG, unique, ICL_TYPE_LONG, dUnique);
	code = epif_Close(ap);
	MBZ(code);
	afsFid.Vnode = position;
	afsFid.Unique = dUnique;
	if (delete) {

	    osi_UnmakePreemptionRight();
	    code = vol_efsDelete(volp, &afsFid, credp);
	    osi_MakePreemptionRight();

	    if (code != 0)
		goto done;
	} else {
	    /* Need to truncate to zero length. Could avoid this for files
	     * if we could punch holes in files but we do not have that feature
	     * yet */
	    AFS_hzero(hlen);
	    osi_UnmakePreemptionRight();
	    code = vol_efsTruncate(volp, &afsFid, hlen, credp);
	    if ((code == 0) &&
		(xvattrp->vattr.va_type == VREG ||
		 xvattrp->vattr.va_type == VLNK)) {

		/* Truncate to file len to set correct storage class */

		AFS_hset64(hlen, 0, xvattrp->vattr.va_size);
		code = vol_efsTruncate(volp, &afsFid, hlen, credp);
	    }
	    osi_MakePreemptionRight();
	    goto done;
	}
    } else if (code != EPI_E_NOENT)
	goto done;

    /* Else anode doesn't exist so we'll need to create it. */

    epid_Assemble(&fid, position, unique);
    code = epia_StartTran("vol_efsCreate", volh, &transId);
    afsl_MBZ(code);
    code = vnva_FileConstruct(volh, &xvattrp->vattr, &fid, &file, transId);
    ccode = epia_EndTran(volh, transId);
    afsl_MBZ(ccode);
    if (code != 0)
	goto done;

    ccode = epif_Close(file);		/* assume no races with VGET */
    afsl_MBZ(ccode);

    /*
     * Truncate to file len to set correct storage class
     *
     * IMPORTANT: Keep the call to vol_efsTruncate after the call
     * above to epif_Close. Else the vol_efsTruncate will fail with ESTALE
     * as epif_Open call from will fail with EPI_E_ENOENT as the newly
     * created file's FILE_JUST_CREATED bit would be set. The epif_Close
     * clears this bit.
     */
    if (xvattrp->vattr.va_type == VREG || xvattrp->vattr.va_type == VLNK) {
	afsFid.Vnode = position;
	afsFid.Unique = unique;
	AFS_hset64(hlen, 0, xvattrp->vattr.va_size);
	osi_UnmakePreemptionRight();
	code = vol_efsTruncate(volp, &afsFid, hlen, credp);
	osi_MakePreemptionRight();
    }
    if (code)
	goto done;

    handlerp->fid.Vnode = position;
    handlerp->fid.Unique = epid_ToUnique(&fid);

    /*
     * It doesn't make much sense to create a dir with no bytes and leave it
     * that way, so instead, we take this to mean that the directory should
     * be freshly initialized.  This fits nicely with the requirments of
     * the ftserver-restore usage.  (cribbed from efsTruncate 9-xii-1992 cfe)
     */
    if (xvattrp->vattr.va_type == VDIR) {
	struct epi_Dir_Handle newdirHandle;
	elbt_TranRec_t transId;

	VFtoEV(volp, &handlerp->fid, &evp, &code);
	if (code != 0)
	    goto done;

	code = epia_StartTran("vol_efsCreate makedir", volh, &transId);
	afsl_MBZ(code);

	vndh_SetDirHandle(evp, &newdirHandle);
	code = dir_MakeDir((struct Dir_Handle *)&newdirHandle, 0, 0, transId);

	ccode = epia_EndTran(volh, transId);
	afsl_MBZ(ccode);
	VN_RELE (EVTOV (evp));
    } else if (ISSPECTYPE (xvattrp->vattr.va_type)) { /* store rdev for special types */
	struct uio uio;
	struct iovec iov;
	dev_t sfdev;
	elbt_TranRec_t transId;

	VFtoEV(volp, &handlerp->fid, &evp, &code);
	if (code != 0)
	    goto done;
#ifdef AFS_OSF_ENV
	evp->v_rdev = sfdev = xvattrp->vattr.va_rdev;
#else
	(EVTOV(evp))->v_rdev = sfdev = xvattrp->vattr.va_rdev;
#endif
	code = epia_StartTran("vol_efsCreate set rdev", volh, &transId);
	afsl_MBZ(code);
	epia_InitUio (&uio, &iov, 0, sizeof (sfdev), (char *) &sfdev);
	code = epia_Write (transId, EVTOA (evp), 0, &uio);
	ccode = epia_EndTran(volh, transId);
	afsl_MBZ(ccode);
	VN_RELE (EVTOV (evp));
    }
done:
    if (code != 0) {
	icl_Trace3(efs_iclSet2, EFS_TRACE_VOL_CREATE_FAILED,
		   ICL_TYPE_LONG, AFS_hgetlo(volp->v_volId),
		   ICL_TYPE_LONG, position,
		   ICL_TYPE_LONG, code);
	VNM_MAP_COMMON_ERROR(code, DEBUG_THIS_FILE);

	/* set iteration handle fid to invalid as required on failure */
	vol_MakeInvalidAfsFid(&(handlerp->fid));
    } else {
	icl_Trace2(efs_iclSet2, EFS_TRACE_VOL_CREATE_SUCCESS,
		   ICL_TYPE_LONG, AFS_hgetlo(volp->v_volId),
		   ICL_TYPE_LONG, position);
    }
    osi_UnmakePreemptionRight();
    return (code);
}

/*
 * vol_efsRead -- read data from a file in a volume
 */
int
vol_efsRead(volp, Fidp, position, length, bufferp, credp, outlenP)
    struct volume *volp;		/* volume */
    struct afsFid *Fidp;		/* file ID */
    afs_hyper_t position;			/* starting byte position */
    int length;			/* # bytes to read */
    char *bufferp;			/* place to put bytes */
    osi_cred_t *credp;			/* credential structure */
    int *outlenP;			/* place for # bytes actually read */
{
    struct evnode *evp;
    epi_anode_t ap;
    struct buf *bp;
    long fileLen;
    int code;
    long off;
    long len = length;
    char *curBufp = bufferp;
    long rLen, wLen;
    daddr_t dblk;

    osi_MakePreemptionRight();
    VFtoEV(volp, Fidp, &evp, &code);
    if (code != 0) {
	osi_UnmakePreemptionRight();
	return (code);
    }

    /* Do not read/write special devices and fifo's */
    if (EV_ISVDEV(evp)) {
	code = EINVAL;
	goto out;
    }

    tlock_ObtainRead(&evp->vd_tlock);
    fileLen = efs_GetLength(evp);
    tlock_ReleaseRead(&evp->vd_tlock);

    ap = EVTOA(evp);

    off = AFS_hgetlo(position);
    *outlenP = len = MIN(length, fileLen - off);
    if (len < 0)
	code = EINVAL;

    icl_Trace4(efs_iclSet2, EFS_TRACE_VOL_READ1,
	       ICL_TYPE_LONG, evp,
	       ICL_TYPE_LONG, off,
	       ICL_TYPE_LONG, len,
	       ICL_TYPE_LONG, fileLen);

    while (len > 0 && code == 0) {
	lock_ObtainRead_r(&evp->vd_file.lock);
	code = efs_FindBlocks(evp, off, &dblk, &wLen, &rLen);
	MBZ(code);
	lock_ReleaseRead_r(&evp->vd_file.lock);

	if (rLen == 0) {
	    long off2 = off;
	    long zeroLen;
	    int i;
	    int tlen;
	    static char zeroBuf[64];

	    /* A file system block sized hole */
	    osi_assert(EPIA_BLOCKED(ap));

	    lock_ObtainRead_r(&evp->vd_file.lock);
	    code = efs_NextBlock(evp, &off2, len);
	    MBZ(code);
	    lock_ReleaseRead_r(&evp->vd_file.lock);
	    if (off2 == -1) {
		zeroLen = len;
	    } else {
		zeroLen = MIN(off2 - off, len);
		osi_assert(zeroLen < fileLen);
	    }

	    for(i = 0; (i < zeroLen) && (code == 0); i += tlen) {
		tlen = MIN((zeroLen - i), sizeof(zeroBuf));
		code = osi_copyout_r(zeroBuf, &curBufp[i], tlen);
	    }

	    curBufp += zeroLen;
	    off += zeroLen;
	    len -= zeroLen;
	} else {
	    rLen = MIN(rLen, len);
	    rLen = MIN(rLen, VOL_EFS_BUFSIZE);
	    bp = vol_efsGetBuf(evp, B_READ, rLen, dblk);
	    code = vol_efsStartIO(bp);
	    afsl_MBZ(code);
	    code = vol_efsBioWait(bp);
	    if (code == 0)
		code = osi_copyout_r(bp->b_un.b_addr, curBufp, rLen);
	    vol_efsReleaseBuf(bp);
	    off += rLen;
	    len -= rLen;
	    curBufp += rLen;
	}
    }

out:
    if (code != 0)
	*outlenP -= len;
    VN_RELE(EVTOV(evp));
    osi_UnmakePreemptionRight();
    return code;
}

/*
 * vol_efsWrite -- write data to a file in a volume
 */
int
vol_efsWrite(volp, Fidp, position, length, bufferp, credp)
    struct volume *volp;		/* volume */
    struct afsFid *Fidp;		/* file ID */
    afs_hyper_t position;			/* starting byte position */
    int length;				/* # bytes to write */
    char *bufferp;			/* user-space data buffer */
    osi_cred_t *credp;			/* credential structure */
{
    int code;
    struct evnode *evp;
    long fileLen;
    daddr_t dblk, dblk0, zDblk;
    long off, off0, zOff;
    long len, wLen, wLen0, zLen;
    char *curBufp = bufferp;
    struct efs_volStat *voldatap;	/* private data of volp */
    struct buf *bp;
    epi_volFileId_t fid;
    epi_anode_t ap;

    osi_MakePreemptionRight();
    voldatap = VTOEVLD(volp);

    VFtoEV(volp, Fidp, &evp, &code);
    if (code != 0) {
	osi_UnmakePreemptionRight();
	return (code);
    }

    ap = EVTOA(evp);

    /* Do not read/write special devices and fifo's */
    if (EV_ISVDEV(evp)) {
	code = EINVAL;
	goto out;
    }

    tlock_ObtainRead(&evp->vd_tlock);
    fileLen = efs_GetLength(evp);
    tlock_ReleaseRead(&evp->vd_tlock);
    len = length;
    osi_assert(len > 0);
    osi_assert(AFS_hfitsinu32(position));
    off = AFS_hgetlo(position);
    osi_assert(off + len <= fileLen);

    icl_Trace4(efs_iclSet2, EFS_TRACE_VOL_WRITE1,
	       ICL_TYPE_LONG, evp,
	       ICL_TYPE_LONG, off,
	       ICL_TYPE_LONG, len,
	       ICL_TYPE_LONG, fileLen);

    /*
     * Need to get write requests in 64K chunks. Only the last chunk (EOF)
     * can be less than 64K.
     */
#define VOL_WRITE_CHUNK_SIZE (64 * 1024)

    epid_Assemble(&fid, Fidp->Vnode, Fidp->Unique);
    if (voldatap->vld_writeInProgress) {
	if (!epid_Equal(&fid, &voldatap->vld_writeFid)) {
	    /* Have not got chunk sized data from write on previous file yet */
	    if ((voldatap->vld_writeLen & (VOL_WRITE_CHUNK_SIZE - 1)) != 0) {
		code = EINVAL;
		goto out;
	    }
	    if ((off & (VOL_WRITE_CHUNK_SIZE - 1)) != 0) {
		code = EINVAL;
		goto out;
	    }
	    epid_Assemble(&voldatap->vld_writeFid, Fidp->Vnode, Fidp->Unique);
	    voldatap->vld_writeLen = off;
	} else if (off != voldatap->vld_writeLen) {
	    code = EINVAL;
	    goto out;
	}
    } else {
	if ((off & (VOL_WRITE_CHUNK_SIZE - 1)) != 0) {
	    code = EINVAL;
	    goto out;
	}
	voldatap->vld_writeInProgress = 1;
	epid_Assemble(&voldatap->vld_writeFid, Fidp->Vnode, Fidp->Unique);
	voldatap->vld_writeLen = off;
    }

    lock_ObtainWrite_r(&evp->vd_vm.lock);
    lock_ObtainWrite_r(&evp->vd_file.lock);
    code = efs_MakeWritableBlocks(evp, off, len, MWB_OVERWRITE);
    lock_ReleaseWrite_r(&evp->vd_vm.lock);
    lock_ReleaseWrite_r(&evp->vd_file.lock);
    if (code)
	goto out;

    while (len > 0 && code == 0) {
	code = efs_FindWritableBlocks(evp, off, &dblk, &wLen);
	MBZ(code);
	osi_assert(wLen > 0);

	dblk0 = dblk;
	wLen0 = wLen;
	off0 = off;

	wLen = MIN(wLen, VOL_EFS_BUFSIZE);
	wLen = MIN(wLen, len);
	bp = vol_efsGetBuf(evp, B_WRITE | B_ASYNC, wLen, dblk);

	code = osi_copyin_r(curBufp, bp->b_un.b_addr, wLen);
	if (code) {
	    vol_efsReleaseBuf(bp);
	    break;
	}
	if (off + wLen == fileLen) {
	    size_t nzero = dbroundup(wLen) - wLen;
	    if (nzero != 0)
		bzero(bp->b_un.b_addr + wLen, nzero);
	}

	code = vol_efsStartIO(bp);
	MBZ(code);

	off += wLen;
	len -= wLen;
	curBufp += wLen;
    }

    /*
     * Zero out the portion of the final FS block past EOF for
     * non-inline files. Do this only if disk blocks exist
     * past EOF in the last allocated region on disk.
     */

    zOff = dbroundup(off);
    if (code == 0 && off == fileLen && zOff < off0 + wLen0) {

	zDblk = dblk0 + btodb(zOff - off0);
	zLen = off0 + wLen0 - zOff;

	while (zLen != 0) {
	    wLen = MIN(zLen, VOL_EFS_BUFSIZE);
	    bp = vol_efsGetBuf(evp, B_WRITE | B_ASYNC, wLen, zDblk);
	    bzero(bp->b_un.b_addr, wLen);
	    code = vol_efsStartIO(bp);
	    MBZ(code);
	    zLen -= wLen;
	    zDblk += btodb(wLen);
	}
    }

  out:
    if (code) {
	voldatap->vld_writeInProgress = 0;
    } else if (off == fileLen) {
	    voldatap->vld_writeInProgress = 0;
    } else {
	voldatap->vld_writeLen += length;
	if ((voldatap->vld_writeLen & (VOL_WRITE_CHUNK_SIZE - 1)) == 0) {
	    /* Received full chunk size worth of data. Next write
	     * need not be contiguous or may not get another
	     * chunk in this file */
	    voldatap->vld_writeInProgress = 0;
	}
    }

    VN_RELE(EVTOV(evp));
    osi_UnmakePreemptionRight();
    return code;
}

/*
 * vol_efsReadHole -- read data from a file and provide indication of
 *	unallocated holes in the read range or adjacent at the end of
 *	the read range. This volop is used to allow compact representation
 *	sparse files in a fileset dump.
 *
 * XXX Future - perhaps add some form of readahead
 */
int vol_efsReadHole(
  struct volume *volp,			/* volume */
  struct afsFid *Fidp,			/* file ID */
  struct vol_ReadHole *readHoleP,	/* read range, buffer etc */
  osi_cred_t *credp)			/* credential structure */
{
    struct evnode *evp;
    epi_anode_t ap;
    long fileLen;			/* file length */
    long len0;				/* length of read request */
    long len;				/* remaining length of data to read */
    long off;				/* next offset to examine */
    long nextDataOff = 0;		/* start of region adjacent to last
					 * allocated region read */
    long holeSeen = 0;			/* has a hole already been seen ? */
    struct buf *bp;
    char *curBufp = readHoleP->buffer;	/* current position in data buffer */
    long rLen, wLen;
    daddr_t dblk;
    int i;

    long code = 0;

    osi_MakePreemptionRight();
    icl_Trace4(efs_iclSet2, EFS_TRACE_VOL_READHOLE,
	       ICL_TYPE_POINTER, volp,
	       ICL_TYPE_FID, Fidp,
	       ICL_TYPE_HYPER, &readHoleP->offset,
	       ICL_TYPE_LONG, readHoleP->length);

    afsl_Assert(AFS_hfitsinu32(readHoleP->offset));
    off = AFS_hgetlo(readHoleP->offset);
    len = len0 = readHoleP->length;

    /* Initialise OUT arguments in case of error */
    readHoleP->flags = 0;
    AFS_hzero(readHoleP->offset);
    readHoleP->length = 0;
    AFS_hzero(readHoleP->hole.holeOff);
    AFS_hzero(readHoleP->hole.holeLen);

    if (len == 0)
	goto bad1;	/* Not really bad! */

    VFtoEV(volp, Fidp, &evp, &code);
    if (code != 0) {
	icl_Trace3(efs_iclSet, EFS_TRACE_VOL_READHOLE_ERR1,
		   ICL_TYPE_POINTER, volp,
		   ICL_TYPE_FID, Fidp,
		   ICL_TYPE_LONG, code);
	goto bad1;
    }
    ap = EVTOA(evp);

    /* Do not read/write special devices and fifo's */
    if (EV_ISVDEV(evp)) {
	code = EINVAL;
	icl_Trace3(efs_iclSet, EFS_TRACE_VOL_READHOLE_ERR2,
		   ICL_TYPE_POINTER, volp,
		   ICL_TYPE_FID, Fidp,
		   ICL_TYPE_LONG, code);
	goto bad2;
    }

    tlock_ObtainRead(&evp->vd_tlock);
    fileLen = efs_GetLength(evp);
    tlock_ReleaseRead(&evp->vd_tlock);

    if (off >= fileLen) {
	code = EINVAL;
	icl_Trace4(efs_iclSet, EFS_TRACE_VOL_READHOLE_ERR3,
		   ICL_TYPE_LONG, evp,
		   ICL_TYPE_LONG, fileLen,
		   ICL_TYPE_LONG, off,
		   ICL_TYPE_LONG, code);
	goto bad2;
    }
    len = MIN(len, fileLen - off);

    /*
     * We only indicate a logically contiguous hole and copy logically
     * contiguous data into the buffer supplied by the user in the
     * struct vol_ReadHole INOUT parameter
     */


    do {
	lock_ObtainRead_r(&evp->vd_file.lock);
	code = efs_FindBlocks(evp, off, &dblk, &wLen, &rLen);
	MBZ(code);
	lock_ReleaseRead_r(&evp->vd_file.lock);

	/*
	 * Currently vol_efsWrite is only able to handle 64K aligned
	 * and sized holes. Hence generate only 64K sized multiple holes
	 * that are 64K boundary aligned.
	 */
	if (rLen == 0) {
	    /* Unallocated region - hole */
	    long holeOff, holeLen, off2;
	    long startZeroOff, startZeroLen;	/* Non 64K aligned hole region
						 * at beginning of the hole */
	    long endZeroOff, endZeroLen;	/* Non 64K aligned hole region
						 * at end of the hole */
#define VOL_MIN_HOLE_LEN VOL_WRITE_CHUNK_SIZE
#define VOL_MIN_HOLE_MASK (VOL_MIN_HOLE_LEN - 1)
#define VOL_HOLE_BASE(off) ((off) & ~VOL_MIN_HOLE_MASK)
#define VOL_HOLE_NEXT(off) (((off) + VOL_MIN_HOLE_MASK) & ~VOL_MIN_HOLE_MASK)

	    holeOff = holeLen = startZeroOff = startZeroLen =
		endZeroOff = endZeroLen = 0;

	    off2 = off;

	    /* Find end of hole */
	    afsl_Assert(EPIA_BLOCKED(ap));
	    lock_ObtainRead_r(&evp->vd_file.lock);
	    code = efs_NextBlock(evp, &off2, len);
	    if (off2 == -1) {
		/* No allocated blocks in [off, off + len) */
		off2 = off + len;
	    }
	    afsl_MBZ(code);
	    lock_ReleaseRead_r(&evp->vd_file.lock);

	    /*
	     * Arrange to zero out any non VOL_MIN_HOLE_LEN sized/aligned
	     * hole region.
	     */
	    if (VOL_HOLE_BASE(off2) <= VOL_HOLE_NEXT(off)) {
		/*
		 * The above conditional clause captures 2 cases:
		 * - off2 is in the same logical VOL_MIN_HOLE_LEN sized region
		 *   as off.
		 * and
		 * - off2 is in a VOL_MIN_HOLE_LEN sized and aligned region
		 *   thats adjacent to the VOL_MIN_HOLE_LEN sized and aligned
		 *   region in which off lies.
		 *
		 * i.e. the spread in VOL_MIN_HOLE_LEN size and aligned chunks
		 * between the hole beg and end is <= 1.
		 */
		startZeroOff = off;
		startZeroLen = off2 - off;
	    } else {
		/*
		 * The hole spans more than a VOL_MIN_HOLE_LEN sized and
		 * aligned region
		 */
		if (off & VOL_MIN_HOLE_MASK) {
		    startZeroOff = off;
		    startZeroLen = VOL_HOLE_NEXT(off) - off;
		}
		holeOff = VOL_HOLE_NEXT(off);
		holeLen = VOL_HOLE_BASE(off2) - holeOff;

		if (off2 & VOL_MIN_HOLE_MASK) {
		    endZeroOff = VOL_HOLE_BASE(off2);
		    endZeroLen = off2 - endZeroOff;
		}
	    }

	    if (startZeroLen) {
		if ((nextDataOff != 0) && (off != nextDataOff))
		    break;
		code = osi_ZeroUData_r(curBufp, startZeroLen);
		if (code) {
		    icl_Trace3(efs_iclSet, EFS_TRACE_VOL_READHOLE_ERR5,
			       ICL_TYPE_POINTER, curBufp,
			       ICL_TYPE_LONG, startZeroLen,
			       ICL_TYPE_LONG, code);
		    break;
		}
		if (nextDataOff == 0) {
		    AFS_hset64(readHoleP->offset, 0, startZeroOff);
		}
		readHoleP->length += startZeroLen;
		off += startZeroLen;
		len -= startZeroLen;
		curBufp += startZeroLen;
		nextDataOff = off;
	    }
	    if (holeLen) {
		if (holeSeen)
		    break;
		AFS_hset64(readHoleP->hole.holeOff, 0, off);
		holeLen = off2 - off;
		AFS_hset64(readHoleP->hole.holeLen, 0, holeLen);
		off += holeLen;
		len -= holeLen;
		holeSeen = 1;
	    }
	    if (endZeroLen) {
		if (nextDataOff != 0 && off != nextDataOff)
		    break;
		code = osi_ZeroUData_r(curBufp, endZeroLen);
		if (code) {
		    icl_Trace3(efs_iclSet, EFS_TRACE_VOL_READHOLE_ERR5,
			       ICL_TYPE_POINTER, curBufp,
			       ICL_TYPE_LONG, endZeroLen,
			       ICL_TYPE_LONG, code);
		    break;
		}
		if (nextDataOff == 0) {
		    AFS_hset64(readHoleP->offset, 0, endZeroOff);
		}
		readHoleP->length += endZeroLen;
		off += endZeroLen;
		len -= endZeroLen;
		curBufp += endZeroLen;
		nextDataOff = off;
	    }
#if 0
	    /* Code without 64K hack considerations for restore */
	    AFS_hset64(readHoleP->hole.holeOff, 0, off);
	    if (off2 == -1) {
		/* No allocated blocks in [off, off + len) */
		holeLen = len;
	    } else {
		holeLen = off2 - off;
	    }
	    AFS_hset64(readHoleP->hole.holeLen, 0, holeLen);
	    off += holeLen;
	    len -= holeLen;
	    holeSeen = 1;
#endif
	} else {
	    long cLen, cLen0;

	    if ((nextDataOff != 0) && (off != nextDataOff))
		break;

	    cLen0 = cLen = MIN(rLen, len);
	    while (cLen > 0) {
		long ioLen;

		/*
		 * XXX Could start all I/O before waiting for any to
		 * complete
		 */
		ioLen = MIN(cLen, VOL_EFS_BUFSIZE);
		bp = vol_efsGetBuf(evp, B_READ, ioLen, dblk);
		code = vol_efsStartIO(bp);
		afsl_MBZ(code);
		code = vol_efsBioWait(bp);
		if (code == 0)
		    code = osi_copyout_r(bp->b_un.b_addr, curBufp, ioLen);
		vol_efsReleaseBuf(bp);
		if (code) {
		    icl_Trace2(efs_iclSet, EFS_TRACE_VOL_READHOLE_ERR4,
			       ICL_TYPE_POINTER, evp,
			       ICL_TYPE_LONG, code);
		    break;
		}
		dblk += btodb(ioLen);
		cLen -= ioLen;
		curBufp += ioLen;
	    }
	    if (code)
		break;
	    if (nextDataOff == 0) {
		AFS_hset64(readHoleP->offset, 0, off);
	    }
	    readHoleP->length += cLen0;
	    off += cLen0;
	    len -= cLen0;
	    /*
	     * If the offset of the next allocation region is equal to
	     * where we currently leave off, we can copy that data
	     * into the user supplied buffer in this call. Record
	     * the expected off in nextDataOff
	     */
	    nextDataOff = off;
	}
    } while (len > 0);
    if (code == 0 && off >= fileLen) {
	readHoleP->flags |= VOL_READHOLE_EOF;
    }
  bad2:
    VN_RELE(EVTOV(evp));
  bad1:
    osi_UnmakePreemptionRight();
    return code;
}


/*
 * vol_efsTruncate -- truncate a file in a volume
 */

int vol_efsTruncate (volp, Fidp, newSize, credp)
    struct volume *volp;		/* volume */
    struct afsFid *Fidp;		/* file ID */
    afs_hyper_t newSize;			/* size to truncate file to */
    osi_cred_t *credp;			/* credential structure */
{
    struct evnode *evp;			/* Episode vnode */
    int code;				/* error return code */
    u_long newsz;			/* short new size */

    osi_MakePreemptionRight();
    /* xxx if vnm_Truncate takes a afs_hyper_t, then this code must be changed xxx */
    if (!AFS_hfitsinu32(newSize)) {
	code = EINVAL;
	goto done;
    }
    newsz = AFS_hgetlo(newSize);

    VFtoEV(volp, Fidp, &evp, &code);
    if (code != 0)
	goto done;

    if (EV_ISDIR(evp) && newsz != 0) {
	code = EINVAL;	/* Can only truncate dirs to 0 */
	goto done;
    }

    tlock_ObtainWriteNT (&evp->vd_tlock);
    code = vnm_Truncate(evp, newsz, 0, credp);

    /*
     * It doesn't make much sense to truncate a directory to zero and leave it
     * that way, so instead, we take this to mean that the directory should
     * be freshly initialized.  This fits nicely with the requirements of
     * the ftserver-restore usage.
     */
    if (!code && EV_ISDIR(evp)) {
	struct epi_Dir_Handle newdirHandle;
	epi_volume_t volh = VTOVH (volp);
	elbt_TranRec_t transId;
	long code2;

        /*
         * For a directory the vnode should not have any dirty
         * cached length here.
         */
	afsl_Assert(evp->vd_cache.newLen == 0);

	code = epia_StartTran("vol_efsTruncate makedir", volh, &transId);
	afsl_MBZ(code);

	vndh_SetDirHandle(evp, &newdirHandle);
	code = dir_MakeDir((struct Dir_Handle *)&newdirHandle, 0, 0, transId);

	code2 = epia_EndTran(volh, transId);
	afsl_MBZ(code2);
    }

    tlock_ReleaseWriteNT (&evp->vd_tlock);
    VN_RELE (EVTOV (evp));
done:
    if (code != 0) {
	icl_Trace4(efs_iclSet2, EFS_TRACE_VOL_TRUNCATE_FAILED,
		   ICL_TYPE_LONG, AFS_hgetlo(volp->v_volId),
		   ICL_TYPE_LONG, Fidp->Vnode,
		   ICL_TYPE_LONG, newsz, ICL_TYPE_LONG, code);
    } else {
	icl_Trace3(efs_iclSet2, EFS_TRACE_VOL_TRUNCATE_SUCCESS,
		   ICL_TYPE_LONG, AFS_hgetlo(volp->v_volId),
		   ICL_TYPE_LONG, Fidp->Vnode,
	    ICL_TYPE_LONG, newsz);
    }
    osi_UnmakePreemptionRight();
    return (code);
}

/*
 * vol_efsDelete -- delete a file in a volume
 *
 * Implementation:  we perform a sequence of calls similar to those of
 * a normal file deletion (efs_remove followed by efs_inactive), except that
 * we do not go through the vnode layer, in order to avoid disturbing the
 * cache of carefully phantomized vnodes.  Note that we call epif_ChangeLink
 * once to get the link count to zero, and then again to actually delete the
 * file.
 */

int vol_efsDelete (volp, Fidp, credp)
    struct volume *volp;		/* volume */
    struct afsFid *Fidp;		/* file ID */
    osi_cred_t *credp;			/* credential structure */
{
    epi_volFileId_t epifid;		/* Episode file ID */
    int code;				/* error return code */

    osi_MakePreemptionRight();
    /*
     * We are in a delicate situation:  we are deleting an anode for which
     * there may exist a (phantomized) vnode with a non-zero reference count.
     */

    /* Deletes the file and stale the corresponding vnode with extreme
     * prejudice.  It does this without regard to the file's link count or the
     * vnode's reference count. */

    if (vol_IsInvalidAfsFid(Fidp)) {
	code = EINVAL;
    } else if (vol_IsDeletedAfsFid(Fidp)) {
	code = ESTALE;
    } else {
	/* Initialize fid to pass to vnm_Delete() */
	epid_Assemble(&epifid, Fidp->Vnode, Fidp->Unique);
	code = vnm_Delete(volp, &epifid);
    }

    icl_Trace4(efs_iclSet2, EFS_TRACE_VOL_DELETE,
	       ICL_TYPE_LONG, AFS_hgetlo(volp->v_volId),
	       ICL_TYPE_LONG, epid_ToIndex(&epifid),
	       ICL_TYPE_LONG, epid_ToUnique(&epifid),
	       ICL_TYPE_LONG, code);
    if (code) {
	VNM_MAP_COMMON_ERROR(code, DEBUG_THIS_FILE);
    }
    osi_UnmakePreemptionRight();
    return (code);
}

/*
 * vol_efsGetattr -- get extended attributes of an anode in a volume
 */

int vol_efsGetattr (volp, Fidp, xvattrp, credp)
    struct volume *volp;		/* volume */
    struct afsFid *Fidp;		/* file ID */
    struct xvfs_attr *xvattrp;		/* extended attribute structure */
    osi_cred_t *credp;			/* credential structure */
{
    struct evnode *evp;			/* Episode vnode */
    epi_volFileId_t epifid;             /* Episode file ID */
    int code;				/* error return code */

    osi_MakePreemptionRight();

    if (vol_IsInvalidAfsFid(Fidp)) {
	code = EINVAL;
    } else if (vol_IsDeletedAfsFid(Fidp)) {
	/* Spec requires that the high half of xvattrp->xvattr.fileID be set to
         * deleted fid's file number, with all other fields set to null (-1);
         * result code must be VOL_ERR_DELETED.
         */
	osi_Memset((char *)xvattrp, -1, sizeof(struct xvfs_attr));
	AFS_hset64(xvattrp->xvattr.fileID, Fidp->Vnode, -1);
	code = VOL_ERR_DELETED;

	icl_Trace2(efs_iclSet2, EFS_TRACE_VOL_GETATTR_ENOENT,
		   ICL_TYPE_LONG, AFS_hgetlo(volp->v_volId),
		   ICL_TYPE_LONG, Fidp->Vnode);
    } else {
	epid_Assemble(&epifid, Fidp->Vnode, Fidp->Unique);
	code = vnm_FindVnode(volp, &epifid, &evp);
    }

    if (!code) {
	tlock_ObtainRead (&evp->vd_tlock);
	/*
	 * Don't flush dirty cached m,ctime in vnode to disk for a dump.
	 * since the restore of the dump will be at a different physical
	 * location.
	 */
	code = vnva_GetAttr (evp, (struct vattr *) xvattrp, credp, 1);

	tlock_ReleaseRead (&evp->vd_tlock);
	VN_RELE (EVTOV (evp));

	icl_Trace3(efs_iclSet2, EFS_TRACE_VOL_GETATTR,
		   ICL_TYPE_LONG, code,
		   ICL_TYPE_LONG, xvattrp->vattr.va_nlink,
		   ICL_TYPE_LONG, AFS_hgetlo(xvattrp->xvattr.volVersion));
    }
    osi_UnmakePreemptionRight();
    return (code);
}

/*
 * vol_efsSetattr -- set extended attributes of an anode in a volume
 */

int vol_efsSetattr (volp, Fidp, xvattrp, credp)
    struct volume *volp;		/* volume */
    struct afsFid *Fidp;		/* file ID */
    struct xvfs_attr *xvattrp;		/* extended attribute structure */
    osi_cred_t *credp;			/* credential structure */
{
    struct evnode *evp;			/* Episode vnode */
    struct xvfs_attr v;			/* accepted attributes */
    int code;				/* error return code */

    osi_MakePreemptionRight();
    VFtoEV(volp, Fidp, &evp, &code);
    if (code != 0)
	goto done;

    v = *xvattrp;
    osi_vattr_init(&v.vattr, 0);

    if (((struct vattr *)xvattrp)->va_size != -1) {
	osi_vattr_add(&v.vattr, OSI_VA_SIZE);
	v.vattr.va_size = xvattrp->vattr.va_size;
    }

    osi_vattr_add(&v.vattr, OSI_VA_NLINK);
    osi_vattr_add(&v.vattr, OSI_VA_ATIME);
    osi_vattr_add(&v.vattr, OSI_VA_MTIME);
    osi_vattr_add(&v.vattr, OSI_VA_CTIME);
    v.vattr.va_nlink = xvattrp->vattr.va_nlink;
    v.vattr.va_atime = xvattrp->vattr.va_atime;
    v.vattr.va_mtime = xvattrp->vattr.va_mtime;
    v.vattr.va_ctime = xvattrp->vattr.va_ctime;

    /*
     * Need to set uid, gid, and mode in case vol_efsCreate only
     * truncated the file/dir on finding a fid uniqifier match.
     * This does duplicate efforts if vol_efsCreate did create the
     * file/dir but is much less than deletion and recreation of each
     * file. This duplication would be avoided with a diff based setattr
     * planned for the future.
     */
    osi_vattr_add(&v.vattr, OSI_VA_UID);
    osi_vattr_add(&v.vattr, OSI_VA_GID);
    osi_vattr_add(&v.vattr, OSI_VA_MODE);
    v.vattr.va_uid = xvattrp->vattr.va_uid;
    v.vattr.va_gid = xvattrp->vattr.va_gid;
    v.vattr.va_mode = xvattrp->vattr.va_mode;

    code = vnva_SetAttr (evp, (struct vattr *)&v, credp,
			 XVN_EXTENDED | XVN_VOL_SETATTR);
    VN_RELE (EVTOV (evp));

done:
    icl_Trace3(efs_iclSet2, EFS_TRACE_VOL_SETATTR,
	       ICL_TYPE_LONG, code,
	       ICL_TYPE_LONG, xvattrp->vattr.va_nlink,
	       ICL_TYPE_LONG, AFS_hgetlo(xvattrp->xvattr.volVersion));

    osi_UnmakePreemptionRight();
    return (code);
}

/*
 * vol_efsGetAcl -- Get ACL of a file in a volume
 */

/* EXPORT */
int vol_efsGetAcl (volp, Fidp, aclp, w, credp)
    struct volume *volp;		/* volume */
    struct afsFid *Fidp;		/* file ID */
    struct dfs_acl *aclp;		/* place to put ACL */
    int w;				/* which ACL to get */
    osi_cred_t *credp;			/* credential structure */
{
    epi_volFileId_t epifid;		/* file ID */
    epi_anode_t ap;			/* anode (file) handle */
    struct epif_status fstat;		/* status info for ap */
    int flag;				/* which-acl flag for epif layer */
    int alength;			/* length of acl */
    int code;				/* error return code */

    osi_MakePreemptionRight();

    /* Get anode handle */
    if (vol_IsInvalidAfsFid(Fidp)) {
	code = EINVAL;
    } else if (vol_IsDeletedAfsFid(Fidp)) {
	code = ESTALE;
    } else {
	epid_Assemble(&epifid, Fidp->Vnode, Fidp->Unique);
	code = epif_Open(VTOVH (volp), &epifid, &ap);
    }
    if (code) goto bad;

    /* Get status; includes acl lengths and mode */
    code = epif_GetStatus (ap, &fstat);
    if (code) goto bad1;

    /* Convert w to things we can use */
    switch (w) {
	case VNX_ACL_REGULAR_ACL:
	    flag = EPIF_AUX_IS_ACL;
	    alength = fstat.acl;
	    break;
	case VNX_ACL_DEFAULT_ACL:
	    flag = EPIF_AUX_IS_INITIAL_DIR_ACL;
	    alength = fstat.initialDirAcl;
	    break;
	case VNX_ACL_INITIAL_ACL:
	    flag = EPIF_AUX_IS_INITIAL_FILE_ACL;
	    alength = fstat.initialFileAcl;
	    break;
	default:
	    code = EINVAL;
	    goto bad1;
    }
    aclp->dfs_acl_len = alength;

    /* If no ACL, length will be zero */
    if (alength == 0) {
	code = 0;
    } else {
	/* Read the ACL */
	code = epif_ReadAux (ap, flag, (u_long) 0, alength, aclp->dfs_acl_val);
    }
bad1:
    epif_Close (ap);
bad:
    if (code)
	VNM_MAP_COMMON_ERROR(code, DEBUG_THIS_FILE);
    osi_UnmakePreemptionRight();
    return code;
}

/*
 * vol_efsSetAcl -- Set ACL of a file in a volume.
 */

/* EXPORT */
int vol_efsSetAcl (volp, Fidp, aclp, index, w, credp)
    struct volume *volp;		/* volume */
    struct afsFid *Fidp;		/* file ID */
    struct dfs_acl *aclp;		/* data of ACL */
    int index;				/* UNUSED: file from which to
					 * copy ACL */
    int w;				/* which ACL to set */
    osi_cred_t *credp;			/* credential structure */
{
    epi_volFileId_t epifid;		/* file ID */
    epi_anode_t ap;			/* anode (file) handle */
    elbt_TranRec_t transId;		/* transaction ID */
    int flag;				/* which-acl flag for epif layer */
    int code;				/* error return code */
    int ccode;
    dacl_t *daclP = NULL;		/* validated acl */
    char *aclBuf = NULL;		/* buffer for flattened ACL */
    u_int aclLen = 0;			/* length of flattened ACL */
    int aclBufRelease = 0;
    char *whichAcl;

    osi_MakePreemptionRight();
    osi_assert(aclp != NULL);

    /* Convert w to things we can use */
    switch (w) {
	case VNX_ACL_REGULAR_ACL:
	    flag = EPIF_AUX_IS_ACL;
	    whichAcl = "object";
	    break;
	case VNX_ACL_DEFAULT_ACL:
	    flag = EPIF_AUX_IS_INITIAL_DIR_ACL;
	    whichAcl = "IC";
	    break;
	case VNX_ACL_INITIAL_ACL:
	    flag = EPIF_AUX_IS_INITIAL_FILE_ACL;
	    whichAcl = "IO";
	    break;
	default:
	    code = EINVAL;
	    goto out;
    }

    if (aclp->dfs_acl_len > 0) {
	eacl_AllocateAcl (&daclP);
	/*
	 * Validate the ACL.
	 * If the ACL has unauth mask entries, throw it away. See DB5716
	 */
	osi_UnmakePreemptionRight();
	ccode = dacl_epi_ValidateBuffer (aclp->dfs_acl_val,
					 (unsigned int *)&aclp->dfs_acl_len,
					 DACL_VB_REMOVE_UNAUTH_MASK, &daclP);
	osi_MakePreemptionRight();
	if (ccode) {
	    if (ccode == DACL_REMOVED_UNAUTH_MASK) {
		/*
		 * The acl being restored from the dump has an ACL with
		 * an unauthenticated mask entry which is not used in DFS.
		 * The validate and parse routine dacl_epi_Validatebuffer
		 * has removed this entry from the ACL as per DB5716.
		 * Hence we need to generate a bytestream representation
		 * of the ACL storing to disk again.
		 */
		icl_Trace4(efs_iclSet2, EFS_TRACE_VOL_SETACL_UNAUTH_MASK,
			   ICL_TYPE_STRING, whichAcl,
			   ICL_TYPE_LONG, Fidp->Vnode,
			   ICL_TYPE_LONG, Fidp->Unique,
			   ICL_TYPE_HYPER, &volp->v_volId);
		osi_UnmakePreemptionRight();
		code = dacl_FlattenAcl(daclP, &aclBuf, &aclLen);
		osi_MakePreemptionRight();
		if (code) {
		    EFS_TRACE_VNOPS(DEBUG_THIS_FILE, code, ENOMEM);
		    eacl_ReleaseAcl(daclP);
		    code = ENOMEM;
		    goto out;
		}
		aclBufRelease = 1;
	    } else {
		if (ccode == DACL_ERROR_UNRECOGNIZED_ENTRY_TYPE)
		    code = ESPIPE;	/* bad entry type */
		else
		    code = EDOM;	/* some other bad ACL problem */
		EFS_TRACE_VNOPS(DEBUG_THIS_FILE, ccode, code);
		goto out;
	    }
	} else {
	    aclBuf = aclp->dfs_acl_val;
	    aclLen = aclp->dfs_acl_len;
	}
	eacl_ReleaseAcl(daclP);
    } else {
	/* Acl length zero => delete the ACL request */
	aclBuf = aclp->dfs_acl_val;
	aclLen = aclp->dfs_acl_len;
    }

    /* Get anode handle */
    if (vol_IsInvalidAfsFid(Fidp)) {
	code = EINVAL;
    } else if (vol_IsDeletedAfsFid(Fidp)) {
	code = ESTALE;
    } else {
	epid_Assemble(&epifid, Fidp->Vnode, Fidp->Unique);
	code = epif_Open(VTOVH (volp), &epifid, &ap);
    }
    if (code) goto out;

    /* Start a transaction */
    code = epia_StartTran ("vol_efsSetAcl", ap, &transId);
    afsl_MBZ(code);

    /* Write ACL */
    code = epif_WriteAux(transId, ap,
			 flag | EPIF_FLAGS_NEW_AUX | EPIF_FLAGS_TRUNCATE_AUX,
			 (u_long) 0, aclLen, aclBuf);
    /* End transaction */
    ccode = epia_EndTran (ap, transId);
    afsl_MBZ(ccode);

    ccode = epif_Close (ap);
    afsl_MBZ(ccode);

  out:
    /* Translate error codes */
    if (code)
	VNM_MAP_COMMON_ERROR(code, DEBUG_THIS_FILE);

    if (aclBufRelease)
	osi_Free_r(aclBuf, aclLen);

    osi_UnmakePreemptionRight();
    return code;
}

/*
 * vol_efsClone -- Clone a volume.  This uses an iteration mechanism similar to
 *     that used in Destroy.
 *
 * RETURN CODES -- EIO returned on unexpected errors.  ELOOP returned if caller
 *     should restart operation.
 */

/* EXPORT */
int vol_efsClone (volp, vol2p, credp)
    struct volume *volp;
    struct volume *vol2p;
    osi_cred_t *credp;			/* credential structure */
{
    register struct volume *srcvolp;	/* source volume */
    epi_volume_t srcvolh;		/* source volume handle */
    struct efs_volStat *srcvoldata;	/* private data for srcvolp */
    struct efs_volStat *voldata;	/* private data for volp */
    epi_volume_t volh;			/* volume handle */
    elbt_TranRec_t transId;		/* transaction ID */
    u_long newindex;			/* dummy arg. for epiv_CreateAnode */
    u_long index;			/* epiv-style index we're working on */
    epi_anode_t ap;			/* anode handle in source volume */
    epi_anode_t newap;			/* anode handle newly created */
    int flags;				/* flags for epiv_CreateAnode */
    int code;				/* error return code */
    long ccode;				/* for transactions */

    osi_MakePreemptionRight();
    srcvolp = vol2p;
    srcvoldata = VTOEVLD (srcvolp);
    srcvolh = srcvoldata->vld_vh;

    voldata = VTOEVLD (volp);
    volh = voldata->vld_vh;

#ifdef DFS_BOMB
    /* Simulate a generic failure in this routine */
    code = BOMB_EXEC("vol_efsClone.1", 0);
    if (code)
	goto out;
#endif	/* DFS_BOMB */

    code = 0;
    ap = 0;
    index = voldata->vld_wholeVolProgress;
    if (index == 0) {
	index = 1;		/* start from the beginning */
	icl_Trace2(efs_iclSet, EFS_TRACE_EFSCLONE_1ENTER,
		   ICL_TYPE_LONG, AFS_hgetlo(volp->v_volId),
		   ICL_TYPE_LONG, AFS_hgetlo(vol2p->v_volId));

	vnm_StopUse (volp, STOPUSE_HEAVY);
	if (!VOL_IS_ATLEAST_STOPUSE_RO(vol2p))
	    vnm_StopUse (vol2p, STOPUSE_RO);

	/*
	 * The last index could have changed since the fileset was opened, so
	 * we update our cached copy.
	 */
	if (code == 0)
	    code = epiv_LastIndex(srcvolh, &srcvoldata->vld_lastIndex);

	if (code == 0)
	    code = TwiddleInconBit(volp, 1);

	if (code != 0)
	    goto out;

	icl_Trace2(efs_iclSet2, EFS_TRACE_EFSCLONE_1EXIT,
		   ICL_TYPE_LONG, AFS_hgetlo(volp->v_volId),
		   ICL_TYPE_LONG, AFS_hgetlo(vol2p->v_volId));
    }

    for (; index <= srcvoldata->vld_lastIndex; index++) {
	code = epiv_OpenAnode (srcvolh, index, &ap);
	if (!code) break;
	if (code != EPI_E_BADANODEINDEX) goto out;
	ap = 0;
	code = 0;
	if (index >
	    srcvoldata->vld_wholeVolProgress+EPI_VNOPS_MAX_INKERNEL_LOOP) {
	    /* return from kernel periodically, even if nothing is happening */
	    code = ELOOP;
	    goto out;
	}
    }

    if (ap) {
	/* found an anode in source volume so create the clone */
	code = epiv_CloseAnode (ap);
	afsl_MBZ(code);

	flags = EPIV_CFLAGS_USEFID | EPIA_CFLAGS_COPYONWRITE;
	newindex = index;		/* required index of new anode */

	ccode = epia_StartTran ("vol_efsClone", volh, &transId);
	afsl_MBZ(ccode);
	code = BOMB_EXEC("vol_efsClone.1",
			 epiv_CreateAnode (transId, volh, flags, 0, 0,
					   srcvolh, /*backing*/index, 0,
					   &newindex, &newap));
	ccode = epia_EndTran (volh, transId);
	afsl_MBZ(ccode);

	if (code) goto out;
	code = epiv_CloseAnode (newap);
	afsl_MBZ(code);

	code = ELOOP;			/* go around again */
    } else {
	struct epiv_status vstat;	/* volume status info about volp */

	/* we're done with the whole volume; copy the filesetatus array */
	/* (the ZLC list) into the clone, and turn off the inconsistent bit */
	code = epiv_GetStatus(srcvolh, &vstat);
	if (code) goto out;

	code = epia_StartTran("vol_efsClone copy ZLC", volh, &transId);
	afsl_MBZ(code);
	code = epiv_SetStatus(transId, volh, EPIV_STATUS_FILESTATUS, &vstat);
	ccode = epia_EndTran(volh, transId);
	afsl_MBZ(ccode);

	if (!code && !(vstat.flags & EPIV_STATUS_FLAGS_INCONSISTENT))
	    code = TwiddleInconBit(volp, 0);
    }

out:
    if (code == ELOOP) {
	voldata->vld_wholeVolProgress = index+1;
    } else {
	voldata->vld_wholeVolProgress = 0;
	if (code)
	    VNM_MAP_COMMON_ERROR(code, DEBUG_THIS_FILE);
    }
    osi_UnmakePreemptionRight();
    return code;
}

/* SomeClone -- Worker for vol_efsReclone and vol_efsUnclone.  Iteration the
 *     same as Destroy and Clone.
 *
 * RETURN CODES -- Unexpected errors are returned as EIO.  Iteration is
 *     triggered by returning ELOOP.
 *     EBUSY -- Couldn't quiesce volume before mucking with it.
 *     EFBIG -- Truncate failed.
 *     EINVAL -- Anodes in vol don't point to bvol.
 *
 * TODO -- What about locking.  We can't vnode lock acls; should we vnode
 *     lock files and dirs?  They don't really change, but perhaps there are
 *     atomicity issues? */

static int SomeClone(
  IN struct volume *volp,		/* volume */
  IN struct volume *bvolp,		/* backing volume */
  IN osi_cred_t *credp,			/* credential structure */
  IN int doReClone)			/* flag for epia_Clone */
{
    long code, ccode;			/* error return code */
    u_long index;
    u_long lastIndex;
    struct efs_volStat *voldata;	/* private data for volp */
    epi_volume_t volh;			/* volume handle */
    epi_anode_t ap;			/* cow anode handle, if any */
    struct efs_volStat *bvoldata;	/* private data for bvolp */
    epi_volume_t bvolh;			/* backing volume handle */
    epi_anode_t b;			/* backing anode handle, if any */
    epi_anode_t bc;			/* backing container */
    epi_volume_t volB;			/* volume of backing container */
    elbt_TranRec_t transId;		/* transaction ID */
    int bIsMissing;			/* flag set if ap has no backing */
    struct epiv_status status;		/* episode volume status */

    voldata = VTOEVLD (volp);
    volh = voldata->vld_vh;
    bvoldata = VTOEVLD (bvolp);
    bvolh = bvoldata->vld_vh;

    code = 0;
    b = ap = bc = 0;
    volB = 0;
    index = voldata->vld_wholeVolProgress;
    if (index == 0) {
	index = 1;		/* start from the beginning */
	icl_Trace3(efs_iclSet, EFS_TRACE_EFSSOMECLONE_1ENTER,
		   ICL_TYPE_LONG, doReClone,
		   ICL_TYPE_LONG, AFS_hgetlo(volp->v_volId),
		   ICL_TYPE_LONG, AFS_hgetlo(bvolp->v_volId));

	if (!VOL_IS_ATLEAST_STOPUSE_RO(volp))
	    vnm_StopUse (volp, STOPUSE_RO);
	vnm_StopUse (bvolp, STOPUSE_HEAVY);

	/*
	 * The last index could have changed since filesets were opened, so
	 * we update our cached copies.
	 */
	if (code == 0)
	    code = epiv_LastIndex(volh, &voldata->vld_lastIndex);
	if (code == 0)
	    code = epiv_LastIndex(bvolh, &bvoldata->vld_lastIndex);

	if (code == 0)
	    code = TwiddleInconBit(bvolp, 1);

	if (code != 0)
	    goto out;

	icl_Trace3(efs_iclSet2, EFS_TRACE_EFSSOMECLONE_1EXIT,
		   ICL_TYPE_LONG, doReClone,
		   ICL_TYPE_LONG, AFS_hgetlo(volp->v_volId),
		   ICL_TYPE_LONG, AFS_hgetlo(bvolp->v_volId));
    }

    /* Set last index to the max */
    lastIndex = voldata->vld_lastIndex < bvoldata->vld_lastIndex ?
			bvoldata->vld_lastIndex : voldata->vld_lastIndex;
    for (; index <= lastIndex; index++) {
	bIsMissing = 0;
	code = epiv_OpenAnode (volh, index, &ap);
	if (code == 0) {
	    if (epia_GetFlags(ap) & EPIA_CFLAGS_COPYONWRITE)
		break;			/* go ahead */
	    code = epiv_OpenAnode (bvolh, index, &b);
	    if (code == 0)
		break;			/* if exists delete it */
	    if (code != EPI_E_BADANODEINDEX) {
		/* some other unexpected code */
		(void) epiv_CloseAnode (ap);
		ap = 0;
		goto out;
	    }
	    /* backing file is non-existent */
	    bIsMissing = 1;
	    if (doReClone) {
		break;			/* if doing a reclone make backing */
	    } else {
		/*
		 * if doing an unclone, this is a no-op,
		 * so back out gracefully
		 */
		code = epiv_CloseAnode (ap);
		ap = 0;
		if (code)
		    goto out;
	    }
	} else if (code != EPI_E_BADANODEINDEX)
	    goto out;
	else {
	    /* This volume slot is empty.  If we doing a reclone we want to
	     * trash the corresponding slot in the backing volume.  If this is
	     * unclone we can do this here to save work in Destroy. */
	    code = epiv_OpenAnode (bvolh, index, &b);
	    if (code != EPI_E_BADANODEINDEX) {
		if (code) goto out;	/* some other error... */
		break;			/* delete it */
	    }
	}
	if (index > voldata->vld_wholeVolProgress+EPI_VNOPS_MAX_INKERNEL_LOOP) {
	    /* return from kernel periodically, even if nothing is happening */
	    code = ELOOP;
	    goto out;
	}
	code = 0;
	b = ap = 0;
    }

    if (b) {
	/* Found an anode in the backing volume so delete it.  Note we unbump
	 * index in this case, so we will check this index again.  */
	afs_hyper_t h0;
	if (ap) {
	    code = epiv_CloseAnode (ap);
	    ap = 0;
	    if (code) {
		(void) epiv_CloseAnode (b);
		b = 0;
		goto out;		/* serious trouble */
	    }
	}
	AFS_hzero (h0);
	code = epia_Truncate (buffer_nullTranId, b, 0, h0);
	if (code) {
	    (void) epiv_CloseAnode (b);
	    b = 0;
	    goto out;
	}
	/* See whether epia_Truncate() did its job, according to Ted */
	if (!AFS_hiszero(epia_GetLength(b))
	    || epia_GetAllocated(b) != 0) {
	    (void) epiv_CloseAnode (b);
	    b = 0;
	    code = EFBIG;
	    goto out;
	}
	ccode = epia_StartTran ("vol_efsReclone delete", bvolh, &transId);
	afsl_MBZ(ccode);
	code = epiv_DeleteAnode (transId, b);
	ccode = epia_EndTran (bvolh, transId);
	afsl_MBZ(ccode);
	if (code) {
	    (void) epiv_CloseAnode (b);
	    b = 0;
	    goto out;
	}
	b = 0;
	index--;			/* hit this index when we... */
	code = ELOOP;			/* ...go around again */
	goto out;
    }

    if (ap && doReClone && bIsMissing) {
	/* Here we build a anode corresponding to this index in the
	 * backing volume. */
	long flags = EPIV_CFLAGS_USEFID | EPIA_CFLAGS_BACKING;
	u_long newindex = index;	/* required index of new anode */

	ccode = epia_StartTran ("vol_efsReclone create", bvolh, &transId);
	afsl_MBZ(ccode);
	code = epiv_CreateAnode (transId, bvolh, flags, 0, 0,
				 volh, index, 0, &newindex, &b);
	ccode = epia_EndTran (bvolh, transId);
	afsl_MBZ(ccode);
	if (code) {
	    (void) epiv_CloseAnode (ap);
	    ap = 0;
	    goto out;
	}
	code = epiv_CloseAnode (b);
	b = 0;
	if (code) {
	    (void) epiv_CloseAnode (ap);
	    ap = 0;
	    goto out;
	}
	/* fall through to actual reclone code */
    }

    if (ap) {
	/* found an anode so {re,un}clone it */
	code = epiv_OpenBackingAnode (ap, &volB, &bc);
	afsl_MBZ(code);
	/* For regular volume ops we require the following relationship to be
	 * true.  If this volume pair is bogus we fail.  If the volumes are ok
	 * but the index doesn't match we could have serious problems and so
	 * panic. */
	if (volB != bvolh) {
	    unsigned copies;
	    int isCow;

	    code = epiv_CloseAnode (bc);
	    bc = 0;
	    ccode = epiv_Close (buffer_nullTranId, volB);
	    volB = 0;
	    if (!code) code = ccode;
	    ccode = epiv_CloseAnode(ap);
	    ap = 0;
	    if (!code) code = ccode;
	    if (code)
		goto out;

	    code = epiv_OpenAnode(bvolh, index, &b);
	    if (code) {
		if (code == EPI_E_BADANODEINDEX) {
		    /* We must be restarting a failed destroy, and this anode
		     * has already been deleted. */
		    code = ELOOP;
		}
		goto out;
	    }

	    copies = epia_GetCopies(b);
	    isCow = (epia_GetFlags(b) & EPIA_CFLAGS_COPYONWRITE);

	    code = epiv_CloseAnode(b);
	    b = 0;
	    if (code)
		goto out;

	    if (copies == 0 && !isCow) {
		/* We must be restarting a partially completed unclone, and
		 * this anode has already been visited. */
		code = ELOOP;
		goto out;
	    }

	    code = EINVAL;
	    goto out;
	}
	afsl_PAssert(epiv_GetAnodeIndex(bc) == index,
		     ("COW and Backing anode not same index (%d != %d)",
		      index, epiv_GetAnodeIndex(bc)));

	/* note that bc is only an anode handle, not a "file" */
	code = epia_Clone (buffer_nullTranId, ap, bc, doReClone);

	ccode = epiv_CloseAnode (bc);
	bc = 0;
	if (!code) code = ccode;
	ccode = epiv_Close (buffer_nullTranId, volB);
	volB = 0;
	if (!code) code = ccode;
	ccode = epiv_CloseAnode (ap);
	ap = 0;
	if (!code) code = ccode;
	if (code) goto out;

	code = ELOOP;
    } else {
	/* we're done with the whole volume */

	if (doReClone) {
	    /* Copy some critical fields into the backing volume's header */
	    code = epiv_GetStatus(volh, &status);
	    if (code) goto out;

	    code = epia_StartTran("vol_efsReclone setstatus", bvolh, &transId);
	    afsl_MBZ(code);
	    code = epiv_SetStatus(transId, bvolh,
				  EPIV_STATUS_VERSION|EPIV_STATUS_UNIQUE
				  |EPIV_STATUS_FILESTATUS,
				  &status);
	    ccode = epia_EndTran(bvolh, transId);
	    afsl_MBZ(ccode);

	    /* Turn off the inconsistent bit */
	    if (!code && !(status.flags & EPIV_STATUS_FLAGS_INCONSISTENT))
		code = TwiddleInconBit(bvolp, 0);
	}
    }

out:
    afsl_PAssert(ap == NULL, ("ap not 0; reclone = %d", doReClone));
    afsl_PAssert(b == NULL, ("b not 0; reclone = %d", doReClone));
    afsl_PAssert(bc == NULL, ("bc not 0; reclone = %d", doReClone));
    afsl_PAssert(volB == NULL, ("volB not 0; reclone = %d", doReClone));
    if (code == ELOOP) {
	voldata->vld_wholeVolProgress = index+1;
    } else {
	voldata->vld_wholeVolProgress = 0;
	if (code)
	    VNM_MAP_COMMON_ERROR(code, DEBUG_THIS_FILE);
    }
    return code;
}

/*
 * vol_efsReclone -- reclone a volume
 */

/* EXPORT */
int vol_efsReclone (volp, vol2p, credp)
    struct volume *volp;		/* volume */
    struct volume *vol2p;		/* backing volume */
    osi_cred_t *credp;			/* credential structure */
{
    int code;

    osi_MakePreemptionRight();
    code = SomeClone(volp, vol2p, credp, 1);
    osi_UnmakePreemptionRight();
    return code;
}

/*
 * vol_efsUnclone -- unclone a volume
 */

/* EXPORT */
int vol_efsUnclone (volp, vol2p, credp)
    struct volume *volp;		/* volume */
    struct volume *vol2p;		/* backing volume */
    osi_cred_t *credp;			/* credential structure */
{
    int code;

    osi_MakePreemptionRight();
    code = SomeClone(volp, vol2p, credp, 0);
    osi_UnmakePreemptionRight();
    return code;
}

/*
 * vol_efsGetVV -- volume version number
 *
 * This is a special case of VOL_GETSTATUS.
 * Called with volp->v_lock shared-locked.
 */

/* EXPORT */
int vol_efsGetVV (volp, vnP)
    struct volume *volp;		/* volume */
    afs_hyper_t *vnP;		/* place to put version number */
{
    afs_hyper_t version;

    osi_MakePreemptionRight();
    epiv_GetVV (VTOVH (volp), &version);
    *vnP = version;
    osi_UnmakePreemptionRight();
    return (0);
}

/*
 * vol_efsSetNewVID -- set volume ID
 *
 * This is a special case of VOL_SETSTATUS.
 */

/* EXPORT */
int vol_efsSetNewVID (volp, idp)
    struct volume *volp;		/* volume */
    afs_hyper_t *idp;		/* volume ID */
{
    epi_volume_t volh;			/* volume handle from volp */
    struct epiv_status vstat;		/* status info about volh */
    elbt_TranRec_t transId;		/* transaction ID */
    long code;

    osi_MakePreemptionRight();
    volh = VTOVH (volp);

    vnm_StopUse (volp, STOPUSE_HEAVY);

    /* Fix anode-layer structure */
    code = epia_StartTran ("vol_efsSetNewVID", volh, &transId);
    afsl_MBZ(code);

    vstat.ident.id = *idp;
    code = epiv_SetStatus (transId, volh, EPIV_STATUS_ID, &vstat);
    afsl_MBZ(code);

    code = epia_EndTran (volh, transId);
    afsl_MBZ(code);
    osi_UnmakePreemptionRight();
    return (0);
}

/*
 * vol_efsCopyAcl -- Copy ACL of a file in a volume
 *
 * CAUTIONS -- This will *NOT* set the mode bits of the file as a side-effect
 *     of calling VOPX_SETACL with (aclp==0).
 */

/* EXPORT */
int vol_efsCopyAcl (volp, Fidp, destw, index, srcw, credp)
  IN struct volume *volp;		/* volume */
  IN struct afsFid *Fidp;		/* file ID */
  IN int destw;			/* which ACL to set */
  IN int index;			/* file from which to copy ACL */
  IN int srcw;				/* which ACL to copy */
  IN osi_cred_t *credp;			/* credential structure */
{
    struct vol_handle handler;		/* iteration handle */
    struct evnode *destevp;		/* Episode vnode who want new acl */
    struct evnode *srcevp;		/* Episode vnode whose acl we want */
    int code;				/* error return code */

    osi_MakePreemptionRight();
    srcevp = destevp = NULL;
    VFtoEV(volp, Fidp, &destevp, &code);
    if (code) goto out;
    bzero((char *)&handler, sizeof(handler));
    handler.volp = volp;
    /* This vol_efsScan call will not cause checkForBackingUp to trigger, with
     * the zeroed opentype */
    osi_UnmakePreemptionRight();
    code = vol_efsScan (volp, index, &handler);
    osi_MakePreemptionRight();
    if (code) goto out;
    VFtoEV(volp, &handler.fid, &srcevp, &code);
    if (code) goto out;
    code = vnva_SetAcl (destevp, 0/*no aclp*/, srcevp,
			destw, srcw, credp, 1 /* isVolop := TRUE */);

out:
    if (srcevp) VN_RELE (EVTOV (srcevp));
    if (destevp) VN_RELE (EVTOV (destevp));
    osi_UnmakePreemptionRight();
    return code;
}

/*
 * vol_efsConcurr -- Determine the level of concurrency for vnode ops
 */
/* EXPORT */
int vol_efsConcurr (volp, type, errorType, concurr)
  IN struct volume *volp;		/* volume */
  IN int type;				/* volops bit vector */
  IN int errorType;			/* error for conflicting opens */
  OUT unsigned char *concurr;		/* concurrency */
{
    unsigned char tcon;

    if ((type & VOL_ALLOK_OPS) == type)	/* all vnops OK? */
	tcon = VOL_CONCUR_ALLOPS;
    else if ( (type & VOL_READONLY_OPS) == type)
	tcon = VOL_CONCUR_READONLY;
    else
	tcon = VOL_CONCUR_NOOPS;
    *concurr = tcon;
    return(0);
}

/*
 * vol_efsSwapIDs -- swap volume IDs of two volumes
 *	We assume that the data contained in vol1p and vol2p has already
 *	been swapped, so we just write the data through to the disk.
 *
 *	In addition to the ID information, we also write the states (flags),
 *	since these are associated with the fileset of the given ID.
 */

/* EXPORT */
int vol_efsSwapIDs (vol1p, vol2p, credp)
    struct volume *vol1p, *vol2p;	/* volumes */
    osi_cred_t *credp;			/* credential structure */
{
    struct efs_volStat *vol1data;	/* private data for vol1p */
    epi_volume_t vol1h;			/* volume handle from vol1data */
    struct efs_volStat *vol2data;	/* private data for vol2p */
    epi_volume_t vol2h;			/* volume handle from vol2data */
    epi_volume_t tranvolh;		/* volume handle for the transaction */
    epi_anode_t avl1h, avl2h;	/* AVLs for the two volumes, to check equality */
    struct epiv_status vstat;		/* status info about volumes */
    elbt_TranRec_t transId;		/* transaction ID */
    long code;

    osi_MakePreemptionRight();
    vol1data = VTOEVLD (vol1p);
    vol1h = vol1data->vld_vh;
    vol2data = VTOEVLD (vol2p);
    vol2h = vol2data->vld_vh;

    osi_assert(VOL_IS_STOPUSE_HEAVY(vol1p));
    osi_assert(VOL_IS_STOPUSE_HEAVY(vol2p));

    code = epiv_GetAggregate(vol1h, &avl1h);
    if (!code)
	code = epiv_GetAggregate(vol2h, &avl2h);
    if (code) {
	osi_UnmakePreemptionRight();
	return code;
    }

    /* Fix anode-layer structure */
    code = epia_StartTran ("vol_efsSwapIDs-1", vol2h, &transId);
    afsl_MBZ(code);

    vstat.ident.id = vol2p->v_volId;
    bcopy(vol2p->v_volName, vstat.ident.name, sizeof vstat.ident.name);
    vstat.flags = 0;
    code = ag_efsStatesToFlags ((vol2p->v_states & ~VOL_BITS_NOSETSTATUS),
				&vstat.flags);
    afsl_MBZ (code);
    code = epiv_SetStatus (transId, vol2h,
			   EPIV_STATUS_ID|EPIV_STATUS_NAME|EPIV_STATUS_FLAGS,
			   &vstat);
    afsl_MBZ(code);

    if (vol1p->v_paggrp != vol2p->v_paggrp || avl1h != avl2h) {
	code = epia_EndTran (vol2h, transId);
	afsl_MBZ(code);

	code = epiv_QuiesceHeader(vol2h, ELBB_QUIESCE_COMMIT);
	afsl_MBZ(code);

	code = epia_StartTran ("vol_efsSwapIDs-2", vol1h, &transId);
	afsl_MBZ(code);

	tranvolh = vol1h;
    } else
	tranvolh = vol2h;

    vstat.ident.id = vol1p->v_volId;
    bcopy(vol1p->v_volName, vstat.ident.name, sizeof vstat.ident.name);
    vstat.flags = 0;
    code = ag_efsStatesToFlags ((vol1p->v_states & ~VOL_BITS_NOSETSTATUS),
			       &vstat.flags);
    afsl_MBZ(code);
    code = epiv_SetStatus (transId, vol1h,
			   EPIV_STATUS_ID|EPIV_STATUS_NAME|EPIV_STATUS_FLAGS,
			   &vstat);
    afsl_MBZ (code);

    code = epia_EndTran (tranvolh, transId);
    afsl_MBZ(code);
    osi_UnmakePreemptionRight();

    SwapPrivateData (vol1p, vol2p);
    return (0);
}

/*
 * vol_efsSync -- synchronously commit volume status to disk
 */

/* EXPORT */
int vol_efsSync (volp, guarantee)
    struct volume *volp;		/* volume */
    int guarantee;
{
    int code = 0;
    epi_volume_t volh;			/* volume handle */

    osi_MakePreemptionRight();
    volh = VTOVH (volp);

    switch (guarantee) {
      case VOL_SYNC_COMMITSTATUS:
	code = epiv_QuiesceHeader(volh, ELBB_QUIESCE_COMMIT);
	break;
      case VOL_SYNC_COMMITMETA:
	elbb_QuiesceSystem(epia_AnodeDevice((epi_anode_t)volh),
			   ELBB_QUIESCE_COMMIT);
	break;
      case VOL_SYNC_COMMITALL:
	elbb_QuiesceSystem(epia_AnodeDevice((epi_anode_t)volh),
			   ELBB_QUIESCE_COMMIT|ELBB_QUIESCE_FSYNC);
	break;
      default:
	code = EINVAL;
	break;
    }

    osi_UnmakePreemptionRight();
    return code;
}

/* EXPORT */
int vol_efsPushStatus(volp)
    struct volume *volp;
{
    int code;

    osi_MakePreemptionRight();
    code = epiv_QuiesceHeader(VTOVH (volp), ELBB_QUIESCE_SERIALIZE);
    osi_UnmakePreemptionRight();
    return code;
}

/*
 * vol_efsReaddir -- Read directory entries
 */

/* EXPORT */
int vol_efsReaddir (volp, fidp, length, bufferp, credp, positionp, numEntriesp)
    struct volume *volp;
    struct afsFid *fidp;
    int length;
    char *bufferp;
    osi_cred_t *credp;
    afs_hyper_t *positionp;
    int *numEntriesp;
{
    struct evnode *evp;
    long code;
    struct uio uio;
    struct iovec iovec;
    int eof;
    struct buf *bp;

    osi_MakePreemptionRight();
    VFtoEV(volp, fidp, &evp, &code);
    if (code) {
	osi_UnmakePreemptionRight();
	return code;
    }

    if (!EV_ISDIR (evp)) {
	code = ENOTDIR;
	goto rele_vp;
    }

    if ((bp = vnvm_PrepareUserBuffer(bufferp, length, 0)) == NULL) {
	code = EFAULT;
	goto rele_vp;
    }
    bzero((caddr_t)&iovec, sizeof iovec);
    iovec.iov_base = bp->b_un.b_addr;
    iovec.iov_len  = length;
    bzero((caddr_t)&uio, sizeof uio);
    osi_InitUIO(&uio);
    uio.uio_iov = &iovec;
    uio.uio_iovcnt = 1;
    uio.uio_resid = length;
#ifdef AFS_AIX31_VM
    uio.uio_xmem = &bp->b_xmemd;
    uio.osi_uio_seg = UIO_XMEM;
#else
#ifdef AFS_HPUX_ENV
    uio.osi_uio_seg = OSI_UIOUSER;
#else
    uio.osi_uio_seg = OSI_UIOSYS;
#endif	/* AFS_HPUX_ENV */
#endif	/* AFS_AIX31_VM */

    if (osi_uio_set_offset(uio, *positionp)) {
	code = EINVAL;
	goto rele_buf;
    }

    if (code = vnd_Read(evp, &uio, credp, 1, numEntriesp, &eof))
	goto rele_buf;

    osi_uio_get_offset(uio, *positionp);

  rele_buf:
    vnvm_ReleaseUserBuffer(bp, bufferp, length);
  rele_vp:
    VN_RELE (EVTOV (evp));
    osi_UnmakePreemptionRight();
    return code;
}

/*
 * vol_efsAppenddir -- Add entries to the end of a directory
 *
 * Note that this function can fail with ENOENT if and entry with a null name
 * is being added into directory, or with ENAMETOOLONG if the entry name
 * too long
 */
int
vol_efsAppenddir(
    struct volume *volp,
    struct afsFid *fidp,
    u_int numEntries,
    u_int length,
    char *bufferp,
    int preserveOffsets,
    osi_cred_t *credp)
{
    char *tmpBufp;
    unsigned i, j;
    vol_dirent_t *entryP;
    int code;
    struct evnode *evp;
    struct Dir_Handle dirh; /* directory handle for evp */
    epi_volFileId_t fid;
    epi_volume_t volh = VTOVH (volp);
    elbt_TranRec_t transId;
    long code2;
    int dirPage;
    int pieceNum;
    int fits;

    osi_MakePreemptionRight();
    icl_Trace4(efs_iclSet2, EFS_TRACE_VOL_APPENDDIR,
	       ICL_TYPE_LONG, AFS_hgetlo(volp->v_volId),
	       ICL_TYPE_LONG, fidp->Vnode,
	       ICL_TYPE_LONG, numEntries, ICL_TYPE_LONG, preserveOffsets);

    VFtoEV(volp, fidp, &evp, &code);
    if (code != 0)
	goto done;

    vndh_SetDirHandle(evp, (struct epi_Dir_Handle *)&dirh);

    tmpBufp = osi_Alloc_r(length);
    code = osi_copyin_r(bufferp, tmpBufp, length);
    if (code != 0)
	goto free_buf;

    entryP = (struct vol_dirent *)tmpBufp;

    for (i = 0; i < numEntries; i++) {
	int lastPageNum = dir_LastPage (&dirh);
	u_int pieces;

	code = vnd_CheckFilenameLength(entryP->name);
	if (code != 0)
	    goto free_buf;

	pieces = dir_NamePieces(strlen(entryP->name));
	icl_Trace4(efs_iclSet2, EFS_TRACE_VOL_APPENDDIR_ENTRY,
		ICL_TYPE_STRING, entryP->name, ICL_TYPE_LONG, entryP->vnodeNum,
		ICL_TYPE_LONG, entryP->offset, ICL_TYPE_LONG, pieces);
	if ((char *)entryP >= tmpBufp + length) {
	    code = E2BIG;
	    goto free_buf;
	}

	if (!preserveOffsets) {
	    struct Dir_Page *dpP;

	    if (dir_GetPiece(&dirh, lastPageNum << LEPP, &dpP) == 0) {
		code = EIO;
		goto free_buf;
	    }

	    dir_FindFreeSpace(dpP, lastPageNum, pieces, &pieceNum);
	    dirb_Release((caddr_t)dpP);
	} else {
	    if ((entryP->offset & ~(PIECESIZE - 1)) != entryP->offset) {
		code = EINVAL;
		goto free_buf;
	    }

	    dirPage = DIR_OFF_TO_PAGE(entryP->offset);
	    for (j = lastPageNum + 1; j <= dirPage; j++){
		code = epia_StartTran("vol_efsAppenddir add page",
				      volh, &transId);
		afsl_MBZ(code);
		code = dir_AddPage(&dirh, transId, j);
		if (code == 0) {
		    evp->vd_flags &= ~VD_LENVALID;
		    code = dir_SetPageCount(&dirh, j + 1, transId);
		}
		tlock_EquivAssign(transId, &evp->vd_tlock);
		code2 = epia_EndTran(volh, transId);
		afsl_MBZ(code2);
		if (code) {
		    dirb_AnnulPage(&dirh, j);
		    goto free_buf;
		}
	    }

	    pieceNum = DIR_OFF_TO_PIECE(entryP->offset);
	    code = dir_FitsAtEnd(&dirh, &pieceNum, entryP->name, &fits);
	    if (code)
		goto free_buf;

	    if (!fits) {
		code = EINVAL;
		goto free_buf;
	    }
	}

	code = epia_StartTran("vol_efsAppenddir create entry", volh,
			      &transId);
	afsl_MBZ(code);

	epid_MakeIndexOnly(&fid, EFS_NET_TO_EVNUM(entryP->vnodeNum));
	code = dir_Create(&dirh, entryP->name, &fid,
			  (long)pieceNum, 0, transId);

	tlock_EquivAssign(transId, &evp->vd_tlock);
	code2 = epia_EndTran(volh, transId);
	afsl_MBZ(code2);

	if (code)
	    goto free_buf;

	evp->vd_flags &= ~VD_LENVALID;
	entryP = (struct vol_dirent *)((char*)entryP + entryP->reclen);
    }

free_buf:
    VN_RELE(EVTOV(evp));
    osi_Free_r(tmpBufp, length);
done:
    if (code != 0) {
	icl_Trace3(efs_iclSet2, EFS_TRACE_VOL_APPENDDIR_FAILED,
		   ICL_TYPE_LONG, AFS_hgetlo(volp->v_volId),
		   ICL_TYPE_LONG, fidp->Vnode,
		   ICL_TYPE_LONG, code);
    } else {
	icl_Trace2(efs_iclSet2, EFS_TRACE_VOL_APPENDDIR_SUCCESS,
		   ICL_TYPE_LONG, AFS_hgetlo(volp->v_volId),
		   ICL_TYPE_LONG, fidp->Vnode);
    }
    osi_UnmakePreemptionRight();
    return code;
}

/*
 * vol_efsGetNextHoles -- Generate information about allocation holes
 */

/* EXPORT */
int vol_efsGetNextHoles(volp, fidp, iterp, credp)
    struct volume *volp;
    struct afsFid *fidp;
    struct vol_NextHole *iterp;
    osi_cred_t *credp;
{
    int code;
    struct evnode *evp;

    osi_MakePreemptionRight();
    VFtoEV(volp, fidp, &evp, &code);
    if (!code) {
	/* Not yet implemented */
	code = ENOSYS;
	VN_RELE(EVTOV(evp));
    }
    osi_UnmakePreemptionRight();
    return code;
}

/*
 * vol_efsGetZLC -- Get 'next' zlc file (based on iterator) from
 * volume ZLC list.
 */
/* EXPORT */
int vol_efsGetZLC (volp, iterP, evpP)
    struct volume *volp;		/* volume */
    u_int *iterP;
    struct vnode **evpP;		/* place to put Episode vnode pointer */
{
    int code;
    osi_MakePreemptionRight();
    code = vnm_GetZLCVnode (volp, iterP, ((struct evnode **)evpP));

    if (code == EPI_E_ITERATIONDONE) {
	/* no more files in list */
	*evpP = (struct vnode *)0;
	code = 0;
    }
    if (code)
	VNM_MAP_COMMON_ERROR(code, DEBUG_THIS_FILE);

    osi_UnmakePreemptionRight();
    return (code);
}

struct volumeops vol_efsops = {
	vol_fsHold,
	vol_fsRele,
	vol_fsLock,
	vol_fsUnlock,
	vol_efsOpen,
	vol_efsSeek,			/* obs */
	vol_efsTell,                    /* obs */
	vol_efsScan,
	vol_efsClose,
	vol_efsDestroy,
	vol_efsAttach,
	vol_efsDetach,
	vol_efsGetStatus,
	vol_efsSetStatus,

	vol_efsCreate,
	vol_efsRead,
	vol_efsWrite,
	vol_efsTruncate,
	vol_efsDelete,
	vol_efsGetattr,
	vol_efsSetattr,
	vol_efsGetAcl,
	vol_efsSetAcl,
	vol_efsClone,
	vol_efsReclone,
	vol_efsUnclone,

	vol_efsVget,
	vol_efsRoot,
	vol_efsIsRoot,
	vol_efsGetVV,
	vol_efsSetDyStat,
	vol_efsFreeDyStat,
	vol_efsSetNewVID,
	vol_efsCopyAcl,
	vol_efsConcurr,
	vol_efsSwapIDs,
	vol_efsSync,
	vol_efsPushStatus,
	vol_efsReaddir,
	vol_efsAppenddir,
	vol_efsBulkSetStatus,
	vol_efsGetZLC,
	vol_efsGetNextHoles,
	vol_efsDeplete,
	vol_efsReadHole,

	vol_fsDMWait,
	vol_fsDMFree,
	vol_fsStartVnodeOp,
	vol_fsEndVnodeOp,
	vol_fsStartBusyOp,
	vol_fsStartInactiveVnodeOp,
};
