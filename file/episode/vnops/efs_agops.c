/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* Copyright (C) 1996, 1990 Transarc Corporation
   All rights reserved.
 */

/*			    Episode File System
			Aggregate operations and vector
 */
#include <dcedfs/param.h>
#include <dcedfs/sysincludes.h>
#include <dcedfs/osi_kvnode.h>
#include <dcedfs/osi_ufs.h>

#include <dcedfs/osi_device.h>		/* osi_{major,minor} */
/* XXX FIX ME */
#undef	PRIVATE
#undef	SHARED

#include <dcedfs/stds.h>
#include <dcedfs/osi.h>			/* osi_Alloc */
#include <dcedfs/debug.h>
#include <dcedfs/osi_cred.h>		/* credential info */
#include <dcedfs/queue.h>		/* struct squeue */
#include <dcedfs/vol_init.h>
#include <dcedfs/volume.h>		/* struct volume */
#include <dcedfs/xvfs_vnode.h>		/* VOPX, etc. */
#include <dcedfs/episode/logbuf.h>	/* logTranRec_t */
#include <dcedfs/episode/tlock.h>	/* struct tlock_data */
#include <dcedfs/episode/volume.h>	/* struct epiv_status */
#include <dcedfs/episode/aggregate.h>	/* struct epi_aggregate_status */
#include <dcedfs/episode/file.h>        /* struct epif_status */
#include <efs_kvol.h>			/* for user-space testing */
#include <efs_evnode.h>			/* struct evnode */
#include <efs_misc.h>
#include <efs_dirhandle.h>              /* epi_Dir_Handle */
#include <efs_vfsops.h>			/* efs_sync() */

RCSID ("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/vnops/RCS/efs_agops.c,v 4.112 1996/03/25 20:40:02 cfe Exp $")

extern struct volumeops vol_efsops;
#define DEBUG_THIS_FILE EFS_DEBUG_AGOPS

extern int epi_Initted;

/* Episode-specific part of aggregate data */

struct ag_efsData {
    epi_anode_t aggh;			/* aggregate handle */
};

/*
 * ag_efsGetHandle -- get Episode aggregate handle
 *
 * Somewhat artificial to export this function rather than just exporting
 * the ag_efsData structure.  Oh well.
 */

/* SHARED */
epi_anode_t ag_efsGetHandle(struct aggr *aggrp)
{
    return ((struct ag_efsData *) aggrp->a_fsDatap)->aggh;
}

/* SHARED */
int ag_efsStatesToFlags(u_long states, int32 *flagsp)
{/* Edit xvolume's states bits into Episode volume's flags bits. */
    register u_int32 flg;
    register u_long stc;

    flg = *flagsp;
    flg &= ~(EPIV_STATUS_FLAGS_READONLY
	      | EPIV_STATUS_FLAGS_COMPLETE
	      | EPIV_STATUS_FLAGS_INCONSISTENT
	      | EPIV_STATUS_FLAGS_DELETENOW
	      | EPIV_STATUS_FLAGS_CLONEINPROG
	      | EPIV_STATUS_FLAGS_REPFIELD
	      | EPIV_STATUS_FLAGS_TYPEFIELD
	      | EPIV_STATUS_FLAGS_REPSERVER_MGD
	      | EPIV_STATUS_FLAGS_NOEXPORT
	      | EPIV_STATUS_FLAGS_IS_REPLICATED
	      | EPIV_STATUS_FLAGS_MOVE_SOURCE
	      | EPIV_STATUS_FLAGS_MOVE_TARGET);

    /* Ensure that only one of VOL_RW and VOL_READONLY is set. */
    stc = states & (VOL_RW | VOL_READONLY);
    if (stc == 0 || stc == (VOL_RW | VOL_READONLY)) return -1;
    if (states & VOL_READONLY) flg |= EPIV_STATUS_FLAGS_READONLY;

    /* _FLAGS_COMPLETE is set from VOL_IS_COMPLETE */
    if (states & VOL_IS_COMPLETE) flg |= EPIV_STATUS_FLAGS_COMPLETE;

    /* _FLAGS_INCONSISTENT is set from VOL_DELONSALVAGE */
    if (states & VOL_DELONSALVAGE) flg |= EPIV_STATUS_FLAGS_INCONSISTENT;

    /* _FLAGS_DELETENOW is set from VOL_ZAPME */
    if (states & VOL_ZAPME) flg |= EPIV_STATUS_FLAGS_DELETENOW;

    /* _FLAGS_CLONEINPROG is set from VOL_CLONEINPROG */
    if (states & VOL_CLONEINPROG) flg |= EPIV_STATUS_FLAGS_CLONEINPROG;

    /* _FLAGS_REPFIELD is set from VOL_REPFIELD */
    switch (states & VOL_REPFIELD) {
	case VOL_REP_RELEASE:
	    flg |= EPIV_STATUS_FLAGS_REP_RELEASE;
	    break;
	case VOL_REP_LAZY:
	    flg |= EPIV_STATUS_FLAGS_REP_LAZY;
	    break;
    }

    /* _FLAGS_TYPEFIELD is set from VOL_TYPEFIELD */
    switch (states & VOL_TYPEFIELD) {
	case VOL_TYPE_RW:
	    flg |= EPIV_STATUS_FLAGS_TYPE_RW;
	    break;
	case VOL_TYPE_RO:
	    flg |= EPIV_STATUS_FLAGS_TYPE_RO;
	    break;
	case VOL_TYPE_BK:
	    flg |= EPIV_STATUS_FLAGS_TYPE_BK;
	    break;
	case VOL_TYPE_TEMP:
	    flg |= EPIV_STATUS_FLAGS_TYPE_TEMP;
	    break;
    }

    /* _FLAGS_REPSERVER_MGD is set from VOL_REPSERVER_MGD */
    if (states & VOL_REPSERVER_MGD) flg |= EPIV_STATUS_FLAGS_REPSERVER_MGD;

    /* _FLAGS_NOEXPORT is set from VOL_NOEXPORT */
    if (states & VOL_NOEXPORT) flg |= EPIV_STATUS_FLAGS_NOEXPORT;

    /* _FLAGS_IS_REPLICATED is set from VOL_IS_REPLICATED */
    if (states & VOL_IS_REPLICATED) flg |= EPIV_STATUS_FLAGS_IS_REPLICATED;

    /* _FLAGS_MOVE_SOURCE is set from VOL_MOVE_SOURCE */
    if (states & VOL_MOVE_SOURCE) flg |= EPIV_STATUS_FLAGS_MOVE_SOURCE;

    /* _FLAGS_MOVE_TARGET is set from VOL_MOVE_TARGET */
    if (states & VOL_MOVE_TARGET) flg |= EPIV_STATUS_FLAGS_MOVE_TARGET;

    *flagsp = flg;
    return 0;
}

/* SHARED */
int ag_efsFlagsToStates(int32 flags, u_long *statesp)
{
    /* Edit Episode flags bits into xvolume's ``states'' bits. */
    u_long sts;

    sts = *statesp;
    sts &= ~(VOL_RW | VOL_READONLY
	      | VOL_IS_COMPLETE | VOL_DELONSALVAGE
	      | VOL_ZAPME | VOL_CLONEINPROG
	      | VOL_REPFIELD | VOL_TYPEFIELD
	      | VOL_REPSERVER_MGD
	      | VOL_NOEXPORT | VOL_IS_REPLICATED
	      | VOL_MOVE_SOURCE | VOL_MOVE_TARGET);

    /* VOL_READONLY or VOL_RW are set from Epi's _FLAGS_READONLY */
    if (flags & EPIV_STATUS_FLAGS_READONLY) {
	sts |= VOL_READONLY;
    } else {
	sts |= VOL_RW;
    }

    /* VOL_IS_COMPLETE is set from Epi's _FLAGS_COMPLETE */
    if (flags & EPIV_STATUS_FLAGS_COMPLETE) sts |= VOL_IS_COMPLETE;

    /* VOL_DELONSALVAGE is set from Epi's _FLAGS_INCONSISTENT */
    if (flags & EPIV_STATUS_FLAGS_INCONSISTENT) sts |= VOL_DELONSALVAGE;

    /* VOL_ZAPME is set from Epi's _FLAGS_DELETENOW */
    if (flags & EPIV_STATUS_FLAGS_DELETENOW) sts |= VOL_ZAPME;

    /* VOL_CLONEINPROG is set from Epi's _FLAGS_CLONEINPROG */
    if (flags & EPIV_STATUS_FLAGS_CLONEINPROG) sts |= VOL_CLONEINPROG;

    /* VOL_REPFIELD is set from Epi's _FLAGS_REPFIELD */
    switch (flags & EPIV_STATUS_FLAGS_REPFIELD) {
	case EPIV_STATUS_FLAGS_REP_RELEASE:
	    sts |= VOL_REP_RELEASE;
	    break;
	case EPIV_STATUS_FLAGS_REP_LAZY:
	    sts |= VOL_REP_LAZY;
	    break;
    }

    /* VOL_TYPEFIELD is set from Epi's _FLAGS_TYPEFIELD */
    switch (flags & EPIV_STATUS_FLAGS_TYPEFIELD) {
	case EPIV_STATUS_FLAGS_TYPE_RW:
	    sts |= VOL_TYPE_RW;
	    break;
	case EPIV_STATUS_FLAGS_TYPE_RO:
	    sts |= VOL_TYPE_RO;
	    break;
	case EPIV_STATUS_FLAGS_TYPE_BK:
	    sts |= VOL_TYPE_BK;
	    break;
	case EPIV_STATUS_FLAGS_TYPE_TEMP:
	    sts |= VOL_TYPE_TEMP;
	    break;
    }

    /* VOL_REPSERVER_MGD is set from Epi's _FLAGS_REPSERVER_MGD */
    if (flags & EPIV_STATUS_FLAGS_REPSERVER_MGD) sts |= VOL_REPSERVER_MGD;

    /* VOL_NOEXPORT is set from Epi's _FLAGS_NOEXPORT */
    if (flags & EPIV_STATUS_FLAGS_NOEXPORT) sts |= VOL_NOEXPORT;

    /* VOL_IS_REPLICATED is set from Epi's _FLAGS_IS_REPLICATED */
    if (flags & EPIV_STATUS_FLAGS_IS_REPLICATED) sts |= VOL_IS_REPLICATED;

    /* VOL_MOVE_SOURCE is set from Epi's _FLAGS_MOVE_SOURCE */
    if (flags & EPIV_STATUS_FLAGS_MOVE_SOURCE) sts |= VOL_MOVE_SOURCE;

    /* VOL_MOVE_TARGET is set from Epi's _FLAGS_MOVE_TARGET */
    if (flags & EPIV_STATUS_FLAGS_MOVE_TARGET) sts |= VOL_MOVE_TARGET;

    *statesp = sts;
    return 0;
}

/*
 * ag_efsAttach -- FS-specific actions associated with attaching an aggregate
 */

/* ARGSUSED */
static int ag_efsAttach(
    dev_t dev,				/* device number */
    struct vnode *bdevvP,		/* vnode for block device */
    int flags,				/* various bits */
    caddr_t efstabparm,			/* FS-specific aggregate info
					 * (in user space) */
    opaque *fsdatapP,		/* place to put FS-specific data */
    int *fsdatalenP)			/* place to put length of fsdatapP */
{
    struct ag_efsData *dat;		/* FS-specific data */
    struct async_device *asdev;		/* dummy arg to epig_InitAggregate */
    epi_anode_t aggh;			/* aggregate handle */
    int code = 0;			/* error return code */

    if (!epi_Initted)
	return EIO;

    OSI_OP_OPEN (bdevvP, FREAD | FWRITE, osi_getucred(), code);
    if (code)
	return code;

    osi_MakePreemptionRight();
    code = epig_InitAggregate(
	osi_major(dev), osi_minor(dev), 1, &asdev, &aggh);
    if (code) {
        EFS_TRACE_VNOPS(DEBUG_THIS_FILE, code, ENXIO);
        code = ENXIO;
    }

    if (code == 0) {
	dat = osi_Alloc_r(sizeof(struct ag_efsData));
	dat->aggh = aggh;

	*fsdatapP = dat;
	*fsdatalenP = sizeof(*dat);
    } else {
	OSI_OP_CLOSE (bdevvP, FREAD | FWRITE, osi_getucred());
    }

    osi_UnmakePreemptionRight();
    return (code);
}

/*
 * ag_efsStat -- get aggregate status
 */

static int ag_efsStat(struct aggr *aggrp, struct ag_status *astatp)
{
    epi_anode_t aggh = ((struct ag_efsData *) aggrp->a_fsDatap)->aggh;
    struct epi_aggregate_status eastat;
    register int kPerBlock;
    register int kPerFrag;
    long code;

    osi_MakePreemptionRight();
    code = epig_GetAggregateStatus (aggh, &eastat);
    afsl_MBZ (code);
    astatp->ag_st = aggrp->a_stat_st;	/* structure assignment */
    bzero ((caddr_t) &astatp->ag_dy, sizeof(astatp->ag_dy));

    kPerBlock = eastat.blockSize / 1024;
    kPerFrag = eastat.fragmentSize / 1024;

    astatp->ag_dy.blocks =
	(eastat.nBlocks + eastat.firstBlock) * eastat.fragsPerBlock;
    astatp->ag_dy.blocksize = eastat.blockSize;
    astatp->ag_dy.fragsize = eastat.fragmentSize;

    /* return everything in K, for consistency */
    astatp->ag_dy.minFree =		/* withholding amount */
	(eastat.fragmentsFreeReally - eastat.fragmentsFree) * kPerFrag;
    astatp->ag_dy.totalUsable =		/* de-rate by withholding */
	eastat.nBlocks * kPerBlock - astatp->ag_dy.minFree;
    astatp->ag_dy.realFree = eastat.fragmentsFree * kPerFrag;
    osi_UnmakePreemptionRight();
    return (0);
}

/*
 * ag_efsVolCreate -- create a new volume
 */

static int ag_efsVolCreate(
    struct aggr *aggrp,		/* aggregate structure */
    afs_hyper_t *volIdp,	/* volume ID (not taken from statusp) */
    struct vol_status *statusp,	/* volume status structure */
    int flags)             /* creation flags */
{
    epi_anode_t aggh;			/* aggregate handle */
    struct epiv_status vstat;		/* volume status */
    u_long index;			/* index in avl */
    int logFragSize;			/* log_2 of fragment size in bytes */
    afs_hyper_t hDum;
    unsigned long fragMask;
    int code;				/* error return code */

    osi_MakePreemptionRight();
    aggh = ((struct ag_efsData *) aggrp->a_fsDatap)->aggh;

    bzero ((caddr_t) &vstat, sizeof (vstat));
    vstat.ident.id = *volIdp;
    strncpy(vstat.ident.name,statusp->vol_st.volName,sizeof(vstat.ident.name));
    code = ag_efsStatesToFlags(statusp->vol_st.states, &vstat.flags);
    if (code) {
	osi_UnmakePreemptionRight();
	return EINVAL;
    }

    /* Volume is inconsistent until it has a root directory */
    vstat.flags |= EPIV_STATUS_FLAGS_INCONSISTENT;
    statusp->vol_st.states |= VOL_DELONSALVAGE;
    vstat.version = statusp->vol_dy.volversion;
    vstat.unique = statusp->vol_dy.unique;
    if (statusp->vol_dy.creationDate.sec) {
	vstat.createTime = statusp->vol_dy.creationDate.sec;
    } else {
	vstat.createTime = osi_Time();
    }
    vstat.modTime = statusp->vol_dy.updateDate.sec;
    vstat.accessTime = statusp->vol_dy.accessDate.sec;
    vstat.upLevelTime1 = statusp->vol_st.cloneTime.sec;
    vstat.upLevelTime2 = statusp->vol_dy.copyDate.sec;
    vstat.upLevelTime3 = statusp->vol_st.vvCurrentTime.sec;
    vstat.backingVolId = statusp->vol_dy.llBackId;
    vstat.upLevelIds[0] = statusp->vol_dy.llFwdId;
    vstat.upLevelIds[1] = statusp->vol_st.parentId;
    vstat.upLevelIds[2] = statusp->vol_dy.backupId;
    vstat.upLevelIds[3] = statusp->vol_dy.cloneId;

    logFragSize = (epia_AnodeDevice (aggh))->logFragSize;
    fragMask = epia_FragmentSize (aggh) - 1;
    hDum = statusp->vol_dy.visQuotaLimit;
    AFS_hadd32(hDum, fragMask);		/* round up to fragment size */
    AFS_hrightshift(hDum, logFragSize);
    vstat.visible.limit = AFS_hgetlo(hDum);
    hDum = statusp->vol_dy.allocLimit;
    AFS_hadd32(hDum, fragMask);		/* round up */
    AFS_hrightshift(hDum, logFragSize);
    vstat.allocated.limit = AFS_hgetlo(hDum);

    strncpy(vstat.statusMsg,statusp->vol_dy.statusMsg,sizeof(vstat.statusMsg));

    code = vnm_CreateVolume(aggh, osi_getucred(),
			    (int)flags & AGGR_CREATE_ROOT, &vstat, &index);

    osi_UnmakePreemptionRight();
    if (!code) {
	statusp->vol_dy.index = index;
	if (flags & AGGR_CREATE_ROOT) {
	    statusp->vol_st.states &= ~VOL_DELONSALVAGE;
	}
	code = vol_Attach (volIdp, statusp, aggrp, &vol_efsops);
    }
    return (code);
}

/*
 * vnv_IsAVolume - test if a particular index in an aggregate holds a volume
 *
 * Too bad epig_OpenVolume doesn't do this.
 */

static int vnv_IsAVolume(
    epi_anode_t aggh,			/* aggregate handle */
    u_long index)			/* index */
{
    int code;				/* error return code */
    int result = 0;
    epi_anode_t vol;			/* potential volume handle */

    code = epia_Open(aggh, index, 0, 0, &vol);
    if (!code) {
	if (epia_GetType(vol) == EPIA_CFLAGS_VOLUME)
	    result = 1;
	code = epia_Close (vol);
	afsl_MBZ(code);
    }
    return result;
}

/*
 * ag_efsVolInfo -- info about the volume at a given index in a given aggregate
 */

static int ag_efsVolInfo(
    struct aggr *aggrp,			/* aggregate structure */
    int index,				/* index within aggregate */
    struct volume *volp)		/* volume structure */
{
    epi_anode_t aggh;			/* aggregate handle */
    epi_volume_t volh;			/* volume handle */
    struct epiv_status vstat;		/* status info about volume */
    int code;			        /* error return code */
    u_long lastindex;			/* highest valid index in aggregate */

    osi_MakePreemptionRight();
    aggh = ((struct ag_efsData *) aggrp->a_fsDatap)->aggh;

    code = epig_LastIndex (aggh, &lastindex);
    afsl_MBZ(code);
    if (index > lastindex)
	code = VOL_ERR_EOF;
    else if (!vnv_IsAVolume (aggh, index))
	code = ENOENT;
    else {
	code = epig_OpenVolume (aggh, index, &volh);
	if (code) {
	    EFS_TRACE_VNOPS(DEBUG_THIS_FILE, code, EIO);
	    code = EIO;
	}
    }
    if (code) {
	osi_UnmakePreemptionRight();
	return code;
    }

    /* epiv_GetStatus only returns an error if the volume handle is
     * trashed, implying the openVolume actually failed, so panic on error */
    code = epiv_GetStatus (volh, &vstat);
    afsl_MBZ(code);

    code = epig_CloseVolume (buffer_nullTranId, volh);
    afsl_MBZ (code);
    osi_UnmakePreemptionRight();

    volp->v_paggrp = aggrp;
    volp->v_volOps = &vol_efsops;

    bzero ((caddr_t) &volp->v_stat_st, sizeof(volp->v_stat_st));
    volp->v_volId = vstat.ident.id;
    volp->v_parentId = vstat.upLevelIds[1];
    volp->v_cloneTime.sec = vstat.upLevelTime1;
    volp->v_cloneTime.usec = 0;
    volp->v_vvCurrentTime.sec = vstat.upLevelTime3;
    volp->v_vvCurrentTime.usec = 0;
    ;					/* vvPingCurrentTime */
    ;					/* accStatus */
    ;					/* accError */
    (void) ag_efsFlagsToStates (vstat.flags, &volp->v_states);
    volp->v_voltype = volp->v_states & (VOL_RW | VOL_READONLY);
    strncpy(volp->v_volName, vstat.ident.name, VOLNAMESIZE);

    return (0);
}

/*
 * ag_efsDetach -- FS-specific actions associated with detaching an aggregate
 */
static int ag_efsDetach(struct aggr *aggrp)
{
    caddr_t dat;			/* FS-specific data for aggrp */
    epi_anode_t aggh;			/* aggregate handle */
    long code;

    osi_MakePreemptionRight();
    dat = aggrp->a_fsDatap;
    aggh = ((struct ag_efsData *)dat)->aggh;

    code = epig_CloseAggregate (aggh);

    osi_Free_r(dat, sizeof (struct ag_efsData));

    if (code != 0) {
	EFS_TRACE_VNOPS(DEBUG_THIS_FILE, code, ENXIO);
	code = ENXIO;
    }

    if (!code) {
	OSI_OP_CLOSE (aggrp->devvp, FREAD | FWRITE, osi_getucred());
    }
    osi_UnmakePreemptionRight();
    return code;
}

/*
 * ag_efsSync -- Sync all Episode filesystems
 */
static int ag_efsSync(struct aggr *aggrp, int syncType)
{
    caddr_t dat;			/* FS-specific data for aggrp */
    epi_anode_t aggh;			/* aggregate handle */
    long code = 0;

    osi_MakePreemptionRight();
    dat = aggrp->a_fsDatap;
    aggh = ((struct ag_efsData *)dat)->aggh;

    switch (syncType) {
      case AGGR_SYNC_FILESYS:
	/* vfsp is not used by efs_sync() */
#ifdef AFS_SUNOS5_ENV
	code = efs_sync((struct osi_vfs *)0, 0, (osi_cred_t *)0);
#else
	code = efs_sync((struct osi_vfs*)0);
#endif
	break;
      case AGGR_SYNC_COMMITMETA:
	elbb_QuiesceSystem(epia_AnodeDevice(aggh), ELBB_QUIESCE_COMMIT);
	break;
      case AGGR_SYNC_COMMITALL:
	elbb_QuiesceSystem(epia_AnodeDevice(aggh),
			   ELBB_QUIESCE_COMMIT|ELBB_QUIESCE_FSYNC);
	break;
      default:
	code = EINVAL;
    }

    osi_UnmakePreemptionRight();
    return code;
}

/* Aggregate ops vector */

struct aggrops ag_efsops = {
    ag_fsHold,
    ag_fsRele,
    ag_fsLock,
    ag_fsUnlock,
    ag_efsStat,
    ag_efsVolCreate,
    ag_efsVolInfo,
    ag_efsDetach,
    ag_efsAttach,
    ag_efsSync,
    ag_fsDMHold,
    ag_fsDMRele,
};
