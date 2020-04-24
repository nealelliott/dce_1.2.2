/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* volume.c -- contains procedures to manage Episode volumes.  It uses the
 *     facilities of the volume_table module and the subsidiary quota and
 *     version number module to implement the volume abstraction. */

/* Copyright (C) 1996, 1990 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/debug.h>
#include <dcedfs/episode/instrument.h>
#include <dcedfs/episode/logbuf.h>
#include "anode.h"
#include "anode_block.h"
#include "fixed_anode.h"
#include "volume_table.h"
#include "volume.h"
#include "quota.h"

RCSID ("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/anode/RCS/volume.c,v 4.146 1996/03/04 19:02:02 ota Exp $")

#define DEBUG_THIS_FILE EPIA_DEBUG_VOLUME

struct stats {
    long updatedVV;			/* how many updates made */
    long skippedVV;			/* number skipped */
    long updatedAQ;			/* allocated quota updates */
    long updatedVQ;			/* visible quota updates */
};

/* This structure is stored in the fileRock in the anode handle of the volume
 * table container.  This means that the volume_table anode handle can be used
 * as the volume handle.
 *
 * This file actually contains two modules, the volume module and the
 * volume_quota module.  For convenience they share the same volume handle
 * pointer, but the fields they may use are well defined and disjoint.  In
 * particular the q_lock protects the allocated and visible structures of the
 * on-disk volume header.  The other lock protects everything else (except the
 * handleLock protects the flags and refCount fields).  The exceptions to this
 * rule are that Open initializes the disk structure with a single read
 * operation and gets fields managed by both simultaneously.  Also the
 * stat.updateQuota is only set by the quota module but may safely be read by
 * either.
 *
 * This is exported for use by file module to tlock aux containers on a per
 * fileset basis.  This is necessary because auxiliary containers are shared
 * between files so no single file's tlock would be sufficient.  A per aux
 * tlock would be possible but much less convenient.  A global lock, which was
 * used originally, is in correct because it may lead to calling EquivTran on
 * transactions in different logs, which is undefined.  Access to this tlock is
 * obtained using the epiv_GetAuxTLock macro.
 *
 * Ditto for the filesetZeroLock.
 */

#define VOL_OFFLINE		0x01	/* volume being deleted */

/* Like epiv_status except hypers are dfsh_diskHyper_ts instead. */

struct diskVolumeHeader {
    int32 magic;			/* VOLUME_MAGIC */
    struct diskIdent {
	dfsh_diskHyper_t id;		/* fileset's id & name */
	char name[EPIV_IDENT_NAME_SIZE];
    } ident;
    int32 flags;			/* miscellaneous, persistent bits */
    u_int32 index;			/* of fileset table anode in avl */
    dfsh_diskHyper_t version;		/* replica fileset version number */
    u_int32 unique;			/* uniquifier for new anodes */
    u_int32 createTime;			/* time fileset created */
    /* These two are sometimes stored simultaneously */
    u_int32 modTime;			/*   last modification */
    u_int32 accessTime;			/*   last epiv Close */
    int32 backingVolIndex;		/* avl index of backing fileset */
    dfsh_diskHyper_t backingVolId;	/* id of backing fileset */

    /* for rep/ft server */
    dfsh_diskHyper_t upLevelIds[EPIV_STATUS_UPLEVELIDS_HYPERS];

    struct epiv_quota allocated;
    u_int32 upLevelTime1;		/* time values for */
    u_int32 upLevelTime2;		/*    rep/ft/backup */
    u_int32 upLevelTime3;		/*    servers       */
    /* 57 words to this point */
    int32 sparel[2];
    struct epiv_quota visible;		/* visible space "used". */
    /* data from second anode */
    char statusMsg[EPIV_STATUS_MSG_SIZE]; /* What's up Doc? */
    int32 fileStatus[EPIV_STATUS_FILESTATUS_WORDS];
    char sparec[76];
};

struct volume {				/* Data storage for a volume */
    struct tmutex_data lock;
    struct tmutex_data q_lock;		/* lock for the quota stuff */
	/*
	 * Update EPIV_AUXTLOCK_OFF and EPIV_ZEROLOCK_OFF in volume.h if you
         * add any structures before filesetAuxTLock field.
	 */
    struct tlock_data filesetAuxTLock;	/* tlock for file module */
    epi_mutex_t filesetZeroLock;	/* lock for file module */

    /* The following two fields are protected by the handleLock. */
    long flags;
    u_int refCount;			/* number of opens */

    long magic;
    u_long openAnodes;			/* number of open anodes */
    int inited_volume_table;		/* whether InitVT called yet */
    epi_anode_t avl;			/* corresponding aggregate */
    int updatedVersion;			/* for optimizing multiple updates */
    daddr_t headerBlock;		/* block address of volume header */
    struct diskVolumeHeader disk;	/* memory copy of both header anodes */
    struct stats stats;

    /* quota stuff */
    unsigned noCheck:1;			/* suppress quota check */
};

/* These macros provide access to the volume header fields spread across both
 * of the first anode. The contents of both are represented in a struct
 * epiv_status, but neither part (currently) fills the whole anode. */

#define RESERVED_ANODES 2
#define FIRST_ANODE_SIZE offsetof(struct diskVolumeHeader, statusMsg[0])
#define SECOND_ANODE_SIZE (sizeof(struct diskVolumeHeader) - FIRST_ANODE_SIZE)
#define VOL_FIELD_OFFSET(field) offsetof(struct diskVolumeHeader, field)
#define VOL_2nd_FIELD_OFFSET(field) \
    (offsetof(struct diskVolumeHeader, field) - FIRST_ANODE_SIZE)

/* for allocation of new handles.  This lock protects the refCount and flags
 * field of all volume handles. */
static osi_dlock_t handleLock;

#define OkayDiskVol(d, ix) \
    (((d)->magic == VOLUME_MAGIC) && (!(ix) || ((d)->index == (ix))))

#define FairVol(v, volC) \
    ((v) && ((v)->magic == ANODE_BLOCK_VOLUME_MAGIC) && \
     OkayDiskVol (&(v)->disk, ((volC) ? epia_GetIndex((epi_anode_t)volC) : 0)))

#define OkayVol(v, volC) (FairVol(v, volC) && (v)->refCount)

/* set v to good looking volume handle from volC or zero. */

#define GetFairVol(v, volC) \
    (((volC) && (v = epia_GetFileRock(volC)) && FairVol (v, volC)) \
     ? v : (v = NULL))

#define GetOKvol(v, volC) \
    (((volC) && (v = epia_GetFileRock(volC)) && OkayVol (v, volC)) \
     ? v : (v = NULL))

/*
 * hook for implementing monitoring and tuning specific to this submodule.
 */
/* EXPORT  */
int ins_anode_volume(opcode,vars,argObjIds,resultObjIds,resultValues)
  IN struct ins_opDesc *opcode;
  IN struct ins_varDesc *vars;
  IN struct ins_basicObjectId *argObjIds;
  OUT struct ins_basicObjectId *resultObjIds;
  OUT struct ins_basicObject *resultValues;
{
    /* not yet implemented */
    return 0;
}

/* One of the important interfaces to the volume module is the support for
 * addressing a container by its index.  Externally, this allows a volume to be
 * regarded as a 1-origin array of containers.  Internally, we use indexes in
 * the volume table.  Because two anode slots are used for the header, we must
 * shift the external index up by one before using it internally. */

#define MapIndexToInternal(i) (i+1)	/* see also epiv_GetAnodeIndex */
#define MapIndexToExternal(i) (i-1)

/* MOD_FIELD_LEN -- provides a shorthand for setting the various fields of the
 *     first part of a volume header.  They use an unusual syntax.  Assumes
 *     variables "code", "trans" and "base" have the usual meanings. */

#define MOD_FIELD_LEN(bufP, field, len) \
    code = elbb_Modify (*(bufP), base + VOL_FIELD_OFFSET(field), \
	(caddr_t)&v->disk.field, len, trans, !elbb_IsZeroTrans(trans), bufP)

#define WITH_HEADER(v, bP) \
    BEGIN_LCRIT (struct elbb_buffer *bP, \
		 code = elbb_Read (epia_AnodeDevice((v)->avl), \
				   (v)->headerBlock, elbb_bufLogged, &bP), \
		 code = elbb_Release (bP, elbb_lazy))

/* Q U O T A   M O D U L E */

static long q_Init(struct volume *v)
{
    epix_EnterCodeBlock(22);
    afsl_Assert (OkayVol(v, NULL));
    tmutex_Init (&v->q_lock, "quota");
    v->noCheck = 0;
    return 0;
}

static long q_Create(
  IN epi_anode_t volC,
  INOUT struct diskVolumeHeader *disk)
{
    int fragmentsPerBlock;

    epix_EnterCodeBlock(1);
    afsl_Assert (OkayDiskVol (disk, epia_GetIndex(volC)));
    fragmentsPerBlock =
	epia_AnodeDevice(volC)->blkSize / epia_FragmentSize(volC);
    disk->allocated.usage = fragmentsPerBlock; /* first blk of volume table */
    disk->visible.usage = fragmentsPerBlock;
    return 0;
}

static long q_Delete(
  IN epi_anode_t volC,
  INOUT struct diskVolumeHeader *disk)
{
    int fragmentsPerBlock;

    epix_EnterCodeBlock(23);
    afsl_Assert (OkayDiskVol (disk, epia_GetIndex(volC)));
    fragmentsPerBlock =
	epia_AnodeDevice(volC)->blkSize / epia_FragmentSize(volC);
    if (disk->allocated.usage != fragmentsPerBlock)
	return EPI_E_NOTEMPTY;
    return 0;
}

/* q_NoCheck -- sets a bit in the volume structure which inhibits quota
 *     checking until further notice. */

static void q_NoCheck(
  IN struct volume *v,
  IN int nocheck)			/* 1=>disable, 0=>re-enable */
{
    epix_EnterCodeBlock(55);

    BEGIN_TMUTEX_NT (&v->q_lock) {
	v->noCheck = nocheck;
    } END_LOCK;
}

static long q_Check(
  IN struct volume *v,
  IN epia_quota_t frags)		/* number of frags being changed */
{
    long code;

    epix_EnterCodeBlock(2);
    afsl_Assert (OkayVol(v, NULL));
    code = 0;
    BEGIN_TMUTEX_NT (&v->q_lock) {
	int net = 0;			/* general sign of change */

	if ((frags.a > 0) || (frags.v > 0)) net = 1;
	else if (((frags.a < 0) || (frags.v < 0)) && (net <= 0)) net = -1;

	if (net >= 0 && !v->noCheck &&
	    (v->disk.allocated.usage + frags.a > v->disk.allocated.limit ||
	     v->disk.visible.usage + frags.v > v->disk.visible.limit))
	    code = EPI_E_VOLUMEFULL;
    } END_LOCK;
    return code;
}

static long q_Get(
  IN struct volume *v,
  OUT struct epiv_quota *allocated,
  OUT struct epiv_quota *visible)
{
    epix_EnterCodeBlock(24);
    afsl_Assert (OkayVol(v, NULL));
    BEGIN_TMUTEX_NT (&v->q_lock) {
	*allocated = v->disk.allocated;
	*visible = v->disk.visible;
    } END_LOCK;
    return 0;
}

/* q_Update -- adds "frags.a" to the volume's allocated usage; ditto for
 *     "frags.v".  If nocheck parameter is false (the normal case) return
 *     VOLUMEFULL if this change would leave the usage above the limit and
 *     [av]frags is non-negative.  Otherwise, just update the usage.  This
 *     latter case is used by the salvager for repairs and by {re,un}clone so
 *     they won't bailout in the middle.
 *
 * RETURN CODES -- Never returns VOLUMEFULL if nocheck is true or if frags is
 *     negative. */

static long q_Update(
  IN elbt_TranRec_t trans,
  IN epi_anode_t volC,
  IN epia_quota_t frags,		/* net frags being allocated */
  IN int nocheck)			/* suppress limit check, just do it! */
{
    struct volume *v;
    long code = 0;
    u_long base;
    epia_quota_t usage;			/* really should be unsigned... */
    int net;				/* general sign of change */

    epix_EnterCodeBlock(3);
    if (!GetOKvol(v, volC))
	return EPI_E_BADVOLUME;

    net = (frags.a > 0 || frags.v > 0);
    if (net == 0 && (frags.a < 0 || frags.v < 0))
	net = -1;

    if (v->noCheck)
	nocheck = 1;	/* force off checking */

    base = epic_IndexToOffset(0);
    BEGIN_TMUTEX (trans, &v->q_lock) {
	afsl_Assert((frags.a > 0 || v->disk.allocated.usage >= -frags.a) &&
		(frags.v > 0 || v->disk.visible.usage >= -frags.v));
	usage.a = v->disk.allocated.usage + frags.a;
	usage.v = v->disk.visible.usage + frags.v;
	if (net >= 0 && !nocheck &&
	    (usage.a > v->disk.allocated.limit ||
	     usage.v > v->disk.visible.limit)) {
	    ABORT_CRIT(EPI_E_VOLUMEFULL);
	}

	/* we've done the check so now go for it. */

	if (frags.a || frags.v) {
	    WITH_HEADER (v, buf) {
		if (frags.a) {
		    v->disk.allocated.usage = usage.a;
		    MOD_FIELD_LEN(&buf, allocated.usage, sizeof (int32));
		    if (code)
			EXIT_CRIT;
		    v->stats.updatedAQ++;
		}
		if (frags.v) {
		    v->disk.visible.usage = usage.v;
		    MOD_FIELD_LEN(&buf, visible.usage, sizeof (int32));
		    if (code)
			EXIT_CRIT;
		    v->stats.updatedVQ++;
		}
	    } END_CRIT;
	    afsl_MBZ (code);		/* can't easily unwind */
	}
    } END_LOCK;
    return code;
}

static long q_SetLimit(
  IN elbt_TranRec_t trans,
  IN epi_anode_t volC,
  IN u_long alimit,			/* new value for allocation limit */
  IN u_long vlimit)			/* ... visible limit */
{
    struct volume *v;
    long code;
    long base;

    epix_EnterCodeBlock(4);
    if (!GetOKvol (v, volC))
	return EPI_E_BADVOLUME;

    base = epic_IndexToOffset(0);
    BEGIN_TMUTEX (trans, &v->q_lock) {
	WITH_HEADER (v, buf) {
	    if (v->disk.allocated.limit != alimit) {
		v->disk.allocated.limit = alimit;
		MOD_FIELD_LEN (&buf, allocated.limit, sizeof(int32));
		if (code) continue;
	    }
	    if (v->disk.visible.limit != vlimit) {
		v->disk.visible.limit = vlimit;
		MOD_FIELD_LEN (&buf, visible.limit, sizeof(int32));
		if (code) continue;
	    }
	} END_CRIT;
	afsl_MBZ (code);
    } END_LOCK;
    return code;
}

/* q_Proc -- Performs the specified operation. The quota limits used during
 *     check and update operations will cause the operation to fail
 *     if the resulting usage exceeds the corresponding limit, but subject to
 *     certain special cases. The basic check for both visible and allocated
 *     quota is ${usage} + {adj} \leq {limit}$.
 *     Negative adjustments are allowed to succeed even if the result is over
 *     limit. However, if either adjustment is positive or neither adjustment
 *     is negative, then the operation fails if either limit is exceeded.
 *
 * PARAMETERS --
 *     trans -- provides the transaction in which this operation occurs.
 *     fileC -- container handle.
 *     op -- selector specifying the desired operation:
 *         QUOTA_CHECK -- checks that the proposed adjustment would succeed,
 *             but makes no update.
 *         QUOTA_UPDATE -- updates the volume quota usage to reflect the
 *             proposed adjustment if the change
 *             does not exceed the quota limits.
 *         QUOTA_UPDATE_NOCHECK -- like update but suppresses limit check. This
 *             succeeds in updating the quota even the resulting usage is above
 *             the limits.
 *	   QUOTA_NOCHECK -- disable quota checking on this volume until further
 *	       notice.
 *         QUOTA_NOTNOCHECK -- re-enable quota checking.
 *     frags -- adjustment to allocated and visible quota.  Either adjustment
 *         may be negative, positive or zero.
 *
 * RETURN CODES -- returns VOLUMEFULL if any quota check fails. */

static long q_Proc(
  IN elbt_TranRec_t trans,
  IN epi_anode_t fileC,
  IN int op,				/* update, check */
  IN epia_quota_t frags)		/* adjustment to allocated & visible */
{
    epi_volume_t vol;
    struct volume *v;

    /* The file being appended to can be the volume itself, so handle this. */
    epix_EnterCodeBlock(5);
    vol = GetOKvol(v, fileC) ? fileC :
	  (epi_volume_t)epia_GetVolumeRock(fileC);

    switch (op) {
      case QUOTA_CHECK:
	epix_EnterCodeBlock(6);
	if ((frags.v < 0 && frags.a <= 0) ||
	    (frags.a < 0 && frags.v <= 0))
	    return 0;			/* this is always okay */
	if (!GetOKvol(v, vol))
	    return EPI_E_BADVOLUME;
	return q_Check(v, frags);

      case QUOTA_UPDATE:
	return q_Update (trans, vol, frags, 0 /*!nocheck*/);

      case QUOTA_UPDATE_NOCHECK:
	return q_Update (trans, vol, frags, 1 /*nocheck*/);

      default:
	afsl_PAssert (0, ("Unknown quota op: %d", op));
	return 0;			/* not reached */
    }
}

/* V O L U M E   M O D U L E */

/* CheckVT -- makes sure volume table has been initialized.  If not it tries to
 *     call the initialization function, returning its code on failure,
 *     otherwise marking the volume as inited and returning zero. */

#define CheckVT(v, volC) \
    (v->inited_volume_table ? 0 \
     : (epit_InitVolumeTable (volC, RESERVED_ANODES) || \
	(v->inited_volume_table++, 0)))

/* epiv_Init -- Initializes the fileset module.  It is called only once during
 *     system initialization by \fcn{epig_Init}, see which. */

/* EXPORT  */
long epiv_Init ()
{
    static inited = 0;
    afsl_Assert ((FIRST_ANODE_SIZE <= ANODE_SIZE) &&
		 (SECOND_ANODE_SIZE <= ANODE_SIZE) &&
		 (EPIV_STATUS_FILESTATUS_WORDS <
		  (2<<(32-EPIV_STATUS_FILESTATUS_N_SHIFT))) &&
		 (sizeof(struct epiv_ident) ==
		  (sizeof (((struct epiv_ident *)0)->name) +
		   sizeof (((struct epiv_ident *)0)->id))) &&
		 (offsetof(struct volume, filesetAuxTLock) ==
		  EPIV_AUXTLOCK_OFF) &&
		 (offsetof(struct volume, filesetZeroLock) ==
		  EPIV_ZEROLOCK_OFF));

#ifdef AFS_DEBUG
    /* CodeBlock: epia */
    epia_codeBlock[DEBUG_THIS_FILE][0] = 57, epia_codeBlock[DEBUG_THIS_FILE][25] = -1;
#endif

    if (inited) return 0;
    inited++;
    lock_InitDesc (&handleLock, "volume handles");
    return 0;
}

static void Cleanup(epi_anode_t volC)
{
    struct volume *v;
    epix_EnterCodeBlock(7);
    if (GetFairVol (v, volC)) {
	afsl_Assert ((v->refCount == 0) && /* better not be an open volume */
		     !tmutex_held (&v->lock)); /* or with any lockers */
	/* Can't destroy tlock/tmutex's here as it would deadlock anode handle
         * cleanup.  This is db2386. */
	/* tmutex_Destroy (&v->lock); */
	/* tmutex_Destroy (&v->q_lock); */
	/* tlock_Destroy (&v->filesetAuxTLock); */
	epi_mutex_destroy (&v->filesetZeroLock);
	osi_Free_r (v, sizeof(*v));
    }
    else /* something wrong during init */;
    epia_SetFileRock (volC, 0, 0);
}

/* epiv_Create -- Creates a new fileset.  The caller provides an open anode
 *     which is empty to be used as the fileset anode table.  The
 *     \arg{status} information provided by the caller includes the name
 *     and the identifier of the fileset being created.
 *
 *     trans -- provides the transaction in which this operation occurs.
 *     volC -- anode handle for fileset anode table.
 *     status -- initialization parameters for the new fileset.
 *
 * LOCKS USED -- Assumes that the aggregate is write locked. */

/* EXPORT  */
long epiv_Create(
  IN elbt_TranRec_t trans,
  IN epi_anode_t avl,
  IN epi_anode_t volC,
  IN struct epiv_status *status)
{
    long code;
    int i;
    long offset;
    struct diskVolumeHeader disk;

    epix_EnterCodeBlock(8);
    if (AFS_hiszero(status->ident.id)) return EPI_E_BADARGS;
    if (epia_GetStatusSize(volC) < sizeof(struct epiv_ident))
	return EPI_E_NOTAVOLUME;

    disk.magic = VOLUME_MAGIC;		/* insist that this be set */
    DFSH_DiskFromMemHyper(disk.ident.id, status->ident.id);
    bcopy(status->ident.name, disk.ident.name, sizeof(disk.ident.name));
    disk.flags = status->flags;
    disk.index = epia_GetIndex(volC);
    DFSH_DiskFromMemHyper(disk.version, status->version);
    disk.unique = 0;
    disk.createTime = osi_Time();
    disk.modTime = status->modTime;
    disk.accessTime = status->accessTime;
    disk.backingVolIndex = 0;	/* NYI */
    DFSH_DiskFromMemHyper(disk.backingVolId, status->backingVolId);
    for (i=0; i<EPIV_STATUS_UPLEVELIDS_HYPERS; i++)
	DFSH_DiskFromMemHyper(disk.upLevelIds[i], status->upLevelIds[i]);
    disk.allocated = status->allocated;
    disk.upLevelTime1 = status->upLevelTime1;
    disk.upLevelTime2 = status->upLevelTime2;
    disk.upLevelTime3 = status->upLevelTime3;
    disk.sparel[0] = status->sparel[0];
    disk.sparel[1] = status->sparel[1];
    disk.visible = status->visible;
    bcopy(status->statusMsg, disk.statusMsg, EPIV_STATUS_MSG_SIZE);
    for (i=0; i<EPIV_STATUS_FILESTATUS_WORDS; i++)
	disk.fileStatus[i] = status->fileStatus[i];
    bcopy(status->sparec, disk.sparec, sizeof(disk.sparec));

    q_Create (volC, &disk);

    /* Allocate first block and initialize */
    code = epit_Create (trans, volC, RESERVED_ANODES);
    if (code) return code;

    offset = epic_IndexToOffset (0);
    BEGIN_LCRIT (struct elbb_buffer *buf,
		 code = epix_GetBuffer (volC, 0, &buf),
		 code = elbb_Release (buf, elbb_lazy)) {
	code = elbb_Modify (buf, offset, (caddr_t)&disk, sizeof(disk),
			    trans, !elbb_IsZeroTrans(trans), &buf);
    } END_CRIT;
    if (code) return code;
    code = epia_SetStatus (trans, volC, 0, sizeof(disk.ident),
			   (opaque)&disk.ident);
    if (code) return code;

    /* Now initialize volume handle so we can initialize its tlock. */

    BEGIN_LCRIT (epi_volume_t volP,	/* for volume table */
		 code = epiv_Open (avl, disk.index, &volP),
		 code = epiv_Close (trans, volP)) {

	/* While safe in this case, normally EquivAssign assumes the lock is
	 * held for write. */

	struct volume *v;
	GetOKvol (v, volC);
	afsl_Assert (v);
	tmutex_EquivAssign(trans, &v->lock);
    } END_CRIT;
    afsl_MBZ (code);
    return 0;
}

/* epiv_Open -- Opens a fileset, identified by its \arg{index}, within the
 *     aggregate specified by \arg{avl}.  The handle to the open fileset is
 *     returned in \arg{volP}.
 *
 * PARAMETERS --
 *     avl -- anode handle for the aggregate's AVL.
 *     index -- index in the AVL anode table.
 *     volP -- pointer to the opened fileset. */

/* EXPORT  */
long epiv_Open(
  IN epi_anode_t avl,			/* corresponding aggregate */
  IN u_long index,
  OUT epi_volume_t *volP)		/* volume handle */
{
    long code;
    struct volume *v;
    epi_anode_t volC;			/* for volume table */

    epix_EnterCodeBlock(9);
    code = epia_Open (avl, index, epia_GetAllocationHandle(avl), 0, &volC);
    if (code)
	return code;

    lock_ObtainWrite_r (&handleLock);
    v = epia_GetFileRock(volC);
    if (v) {
	epix_EnterCodeBlock(10);
	if (v->flags & VOL_OFFLINE) {
	    code = epia_Close(volC);
	    afsl_MBZ (code);
	    return EPI_E_VOLOFFLINE;
	}
	v->refCount++;
	lock_ReleaseWrite_r (&handleLock);

	/* Now grab the volume lock to make sure possible initialization by
         * another thread is finished. */

	BEGIN_TMUTEX_NT (&v->lock) {
	    if (!OkayDiskVol(&v->disk, index))
		{ ABORT_CRIT (EPI_E_BADDISKVOLUME) }
	    afsl_Assert (OkayVol (v, volC));
	} END_LOCK;

    } else {
	epix_EnterCodeBlock(0);
	
	v = osi_Alloc_r (sizeof(*v));
	bzero((caddr_t)v, sizeof (*v));
	v->magic = ANODE_BLOCK_VOLUME_MAGIC;
	v->refCount = 1;
	v->flags &= ~VOL_OFFLINE;
	tmutex_Init (&v->lock, "volume");
	BEGIN_TMUTEX_NT (&v->lock) {
	    u_int size;			/* volume container status size */
	    struct diskIdent ident;

	    epia_SetFileRock (volC, v, Cleanup);
	    lock_ReleaseWrite_r (&handleLock);
	    
	    bzero((caddr_t)&v->stats, sizeof (v->stats));
	    v->avl = avl;
	    v->updatedVersion = 0;
	    v->inited_volume_table = 0;
	    tlock_Init (&v->filesetAuxTLock, "fileset aux");
	    epi_mutex_init (&v->filesetZeroLock, "fileset ZLC list");
	    code = epia_Map (volC, 0, &v->headerBlock, 0);
	    if (code) continue;
	    
	    WITH_HEADER (v, buf) {
		bcopy(elbb_BufferData(buf) + epic_IndexToOffset(0),
		      (caddr_t)&v->disk, FIRST_ANODE_SIZE);
		bcopy(elbb_BufferData (buf) + epic_IndexToOffset (1),
		      v->disk.statusMsg, SECOND_ANODE_SIZE);
	    } END_CRIT;
	    if (code)
		continue;
	    if (!OkayDiskVol(&v->disk, index))
		{ ABORT_CRIT (EPI_E_BADDISKVOLUME) }
	    code = epia_GetStatus (volC, sizeof(ident), (opaque)&ident, &size);
	    afsl_MBZ (code);
	    if ((size != sizeof(ident)) ||
		(bcmp((caddr_t)&ident,
		      (caddr_t)&v->disk.ident, sizeof (ident)) != 0)) {

		/* To make sure we'll never return this volume handle
                 * successfully, change the magic number. */

		v->disk.magic = VOLUME_BAD_MAGIC;
		ABORT_CRIT (EPI_E_BADDISKVOLUME);
	    }
	    
	    q_Init (v);
	    epia_SetQuotaProc (volC, q_Proc);
	} END_LOCK;
    }

    if (code) {
	long ccode;

	/* Do a careful cleanup: epiv_Close does too much checking. */
	lock_ObtainWrite_r (&handleLock);
	v->refCount--;
	lock_ReleaseWrite_r (&handleLock);
	ccode = epia_Close(volC);
	afsl_MBZ (ccode);

	return code;
    }
    *volP = volC;
    return 0;
}

/* epiv_Close -- Closes the anode containing the fileset anode table.  This is
 *     the companion function to \fcn{epiv_Open}.  This also results in
 *     invalidating the file status information cached by the file module (see
 *     also \fcn{epiv_GetZLCAddr}).
 *
 * PARAMETERS --
 *     trans -- provides the transaction in which this operation occurs.  This
 *	   is unused.
 *     vol -- fileset being closed. */

/* EXPORT  */
long epiv_Close(elbt_TranRec_t trans, epi_volume_t vol)
{
    long code;
    struct volume *v;

    epix_EnterCodeBlock(11);
    if (!GetOKvol (v, vol)) return EPI_E_BADVOLUME;

    lock_ObtainWrite_r (&handleLock);
    afsl_Assert ((v->refCount > 0) &&
		 ((v->refCount > 1) || (v->openAnodes == 0)));
    v->refCount--;
    lock_ReleaseWrite_r (&handleLock);

    code = epia_Close(vol);
    afsl_MBZ (code);

    return 0;
}

/* epiv_Delete -- Closes and deletes the fileset.  The caller must have the
 *     only active reference to the fileset.  The fileset provided must be
 *     empty and \fcn{epiv_Deflate}d in order to bound the size of the
 *     transaction.
 *
 * PARAMETERS --
 *     trans -- provides the transaction in which this operation occurs.
 *     vol -- fileset to be deleted.
 *
 * LOCKS USED -- Assumes that the aggregate is write locked. */

/* EXPORT  */
long epiv_Delete(elbt_TranRec_t trans, epi_volume_t vol)
{
    long code;
    struct volume *v;

    epix_EnterCodeBlock(26);
    if (!GetOKvol (v, vol)) return EPI_E_BADVOLUME;
    afsl_Assert(!elbb_IsZeroTrans(trans));

    lock_ObtainWrite_r (&handleLock);

    /* This means that the vnops layer better not call close or delete if the
     * volume is still in use. */

    if (v->refCount > 1) {
	lock_ReleaseWrite_r (&handleLock);
	return (EPI_E_VOLUMEOPEN);
    } else /* actually decrement it down below, when we're sure. */;
    afsl_PAssert ((v->openAnodes == 0),
		  ("Volume delete leaves %d open anodes", v->openAnodes));

    epix_EnterCodeBlock(12);

    /* This might take a longish time so drop the handle lock but lock the
     * volume and mark it as offline. */

    BEGIN_TMUTEX_NT (&v->lock) {
	u_long lastIndex;

	afsl_Assert(!(v->flags & VOL_OFFLINE));
	v->flags |= VOL_OFFLINE;
	lock_ReleaseWrite_r (&handleLock);

	/* Check to see that all anodes have been deleted.  Then check that the
         * volume table has been deflated (shrunk to a single block). */

	if ((code = CheckVT (v, vol)) != 0)
	    EXIT_CRIT;	/* ensure voltable is init'd */
	code = epit_LastIndex (vol, &lastIndex);
	if (code) continue;
	afsl_Assert (lastIndex > 0);
	if (lastIndex+1 != RESERVED_ANODES)
	    { ABORT_CRIT (EPI_E_NOTEMPTY) }

	code = q_Delete (vol, &v->disk);
	if (code) continue;		/* fails if not deflated */
	epia_SetQuotaProc (vol, 0);	/* don't update quota past here */

	/* Delete the volume table, container should be empty now.  After this
         * completes successfully we are committed to the delete. */

	code = epit_Delete (trans, vol, RESERVED_ANODES);
	if (code) continue;

	/* generate cleanup on volume container */
	code = epix_SetGConClose (epix_gc_fileRock, (opaque)v);
	if (!code)
	    /* and cleanup all containers in the volume */
	    code = epix_SetGConClose (epix_gc_volumeRock, (opaque)vol);
	afsl_MBZ (code);		/* these can't really fail */
    } END_LOCK;

    /* If we're not really going to do the delete just turn OFFLINE back off.
     * Otherwise, finish up by deleting the volume table anode.  Hold the
     * handleLock over the delete to lock out anyone else trying to open it,
     * (even transiently to check the OFFLINE bit).  This acutally closes the
     * handle.  Anything that goes wrong at this point is really serious so
     * panic if we get a bad code. */

    lock_ObtainWrite_r (&handleLock);
    if (code) {
	v->flags &= ~VOL_OFFLINE;	/* clear off-line */
    } else {
	v->refCount--;
	afsl_Assert ((v->refCount == 0) && (v->flags & VOL_OFFLINE));
	code = epia_Delete (trans, vol);
	afsl_MBZ (code);
    }
    lock_ReleaseWrite_r (&handleLock);
    return code;
}

/* epiv_GetStatus -- Gets the status for the fileset.
 *
 * PARAMETERS --
 *     h -- fileset handle.
 *     status -- pointer to the fileset's status. */

/* EXPORT  */
long epiv_GetStatus(epi_volume_t h, struct epiv_status *status)
{
    struct volume *v;
    long code;

    epix_EnterCodeBlock(13);
    if (!GetOKvol (v, h)) return EPI_E_BADVOLUME;
    BEGIN_TMUTEX_NT (&v->lock) {
	int i;

	status->magic = v->disk.magic;
	DFSH_MemFromDiskHyper(status->ident.id, v->disk.ident.id);
	bcopy(v->disk.ident.name, status->ident.name,
	      sizeof(status->ident.name));
	status->flags = v->disk.flags;
	status->index = v->disk.index;
	DFSH_MemFromDiskHyper(status->version, v->disk.version);
	status->unique = v->disk.unique;
	status->createTime = v->disk.createTime;
	status->modTime = v->disk.modTime;
	status->accessTime = v->disk.accessTime;
	status->backingVolIndex = v->disk.backingVolIndex;
	DFSH_MemFromDiskHyper(status->backingVolId, v->disk.backingVolId);
	for (i=0; i<EPIV_STATUS_UPLEVELIDS_HYPERS; i++)
	    DFSH_MemFromDiskHyper(status->upLevelIds[i],
				  v->disk.upLevelIds[i]);
	status->allocated = v->disk.allocated;
	status->upLevelTime1 = v->disk.upLevelTime1;
	status->upLevelTime2 = v->disk.upLevelTime2;
	status->upLevelTime3 = v->disk.upLevelTime3;
	status->sparel[0] = v->disk.sparel[0];
	status->sparel[1] = v->disk.sparel[1];
	status->visible = v->disk.visible;
	bcopy(v->disk.statusMsg, status->statusMsg, EPIV_STATUS_MSG_SIZE);
	for (i=0; i<EPIV_STATUS_FILESTATUS_WORDS; i++)
	    status->fileStatus[i] = v->disk.fileStatus[i];
	bcopy(v->disk.sparec, status->sparec, sizeof(status->sparec));

	v->updatedVersion = 0;
    } END_LOCK;
    code = q_Get (v, &status->allocated, &status->visible);
    return code;
}

/* epiv_GetZLCAddr -- Returns a pointer to the fileStatus array in volume
 *     header.  The pointer will be valid as long as the fileset is open (which
 *     includes the time when all constituent containers are open).  The
 *     contents of the array are {\em read-only}.  The \fcn{epiv_SetStatus}
 *     function {\em must} be used to do updates.
 *
 * PARAMETERS --
 *     h -- fileset handle.
 *     zlcListP -- pointer to a pointer to the first element of the file status
 *         array.
 *
 * LOCKS USED -- The caller assumes all responsibility for locking.
 *
 * CAUTIONS -- See above.  This function is a minefield for the unwary. */

/* EXPORT  */
long epiv_GetZLCAddr(epi_volume_t h, int32 **zlcListP)
{
    struct volume *v;

    epix_EnterCodeBlock(45);
    if (!GetOKvol (v, h)) return EPI_E_BADVOLUME;
    *zlcListP = &v->disk.fileStatus[0];
    return 0;
}

/* epiv_GetFlags -- Gets just the flags for the fileset.
 *
 * PARAMETERS --
 *     h -- fileset handle.
 *     flagsP -- pointer to the output flags. */

/* EXPORT  */
long epiv_GetFlags(epi_volume_t h, int32 *flagsP)
{
    struct volume *v;

    epix_EnterCodeBlock(0);
    if (!GetOKvol (v, h)) return EPI_E_BADVOLUME;
    BEGIN_TMUTEX_NT (&v->lock) {
	*flagsP = v->disk.flags;
    } END_LOCK;
    return 0;
}

/* epiv_GetAggregate -- Returns the aggregate to which the specified fileset
 *     belongs.
 *
 * PARAMETERS --
 *     h -- fileset handle.
 *     avlP -- pointer to the AVL for the aggregate. */

/* EXPORT  */
long epiv_GetAggregate(epi_volume_t h, epi_anode_t *avlP)
{
    struct volume *v;

    epix_EnterCodeBlock(14);
    if (!GetOKvol (v, h)) return EPI_E_BADVOLUME;
    if (avlP) *avlP = v->avl;           /* no lock since this is immutable */
    return 0;
}

/* epiv_GetIdent -- Returns the id and name of a fileset.  It can be used
 *     even if the fileset was not opened with \fcn{epiv_Open}, and thus is a
 *     very efficient way to get a fileset's identifier.
 *
 * PARAMETERS --
 *     h --  fileset handle.
 *     ident -- pointer to the identifier for the fileset.
 *
 * LOCKS USED -- The caller is responsible for locking. */

/* EXPORT  */
long epiv_GetIdent(epi_volume_t h, struct epiv_ident *ident)
{
    long code;
    struct volume *v;
    struct diskIdent did;
    struct diskIdent *from;

    epix_EnterCodeBlock(15);
    afsl_Assert(h);
    if (!GetOKvol (v, h)) {
	epix_EnterCodeBlock(16);
	if (epia_GetType(h) != EPIA_CFLAGS_VOLUME) return EPI_E_NOTAVOLUME;
	code = epia_GetStatus (h, sizeof(did), (opaque)&did, 0);
	if (code)
	    return code;
	from = &did;
    } else {
	from = &v->disk.ident;
    }
    DFSH_MemFromDiskHyper(ident->id, from->id);
    bcopy(from->name, ident->name, sizeof(ident->name));
    return 0;
}

/* epiv_GetVV -- Gets the volume version number for the fileset.
 *
 * PARAMETERS --
 *     h -- fileset handle.
 *     versionP -- pointer to the fileset's VV. */

void epiv_GetVV (epi_volume_t h, afs_hyper_t *versionP)
{
    struct volume *v;

    epix_EnterCodeBlock(0);
    GetOKvol (v, h);
    afsl_Assert (v);
    BEGIN_TMUTEX_NT (&v->lock) {
	DFSH_MemFromDiskHyper(*versionP, v->disk.version);
	v->updatedVersion = 0;
    } END_LOCK;
    return;
}

static long UpdateMTime(
  IN elbt_TranRec_t trans,
  IN struct volume *v,
  IN struct elbb_buffer **bufP,
  IN int current)			/* false if 5 secs is good enough */
{
    long code;
    u_long now = osi_Time();
    int granularity = (current ? 1 : 5);
    long base = epic_IndexToOffset (0);

    epix_EnterCodeBlock(17);
    if (now - MIN(v->disk.accessTime, v->disk.modTime) >= granularity) {
	v->disk.accessTime = v->disk.modTime = now;
	MOD_FIELD_LEN (bufP, modTime, 2*sizeof(u_int32));
	if (code) return code;
    }
    return 0;
}

/* epiv_SetStatus -- Sets the status for a fileset in accordance with the
 *     values of \arg{mask} and \arg{status}.
 *
 * PARAMETERS --
 *     trans -- provides the transaction in which this operation occurs.
 *     h -- fileset handle.
 *     mask -- specifies which attributes to select from the \arg{status}.
 *     status -- pointer to the input status values. */

/* EXPORT  */
long epiv_SetStatus(
  IN elbt_TranRec_t trans,
  IN epi_volume_t h,
  IN long mask,
  IN struct epiv_status *status)
{
    struct volume *v;
    long code;
    long base;

    epix_EnterCodeBlock(18);
    if ((mask == 0) || (mask & ~EPIV_STATUS_LEGAL_BITS)) return EPI_E_BADARGS;
    if (!GetOKvol (v, h)) return EPI_E_BADVOLUME;
    afsl_Assert ((v->flags & VOL_OFFLINE) == 0);

    base = epic_IndexToOffset (0);
    BEGIN_TMUTEX (trans, &v->lock) {
        int newIdent = 0;
	WITH_HEADER (v, buf) {

#define SET_FIELD(field, num) \
            epix_EnterCodeBlock /* hack */ (num); \
	    v->disk.field = status->field; \
	    MOD_FIELD_LEN(&buf, field, sizeof(v->disk.field)); \
	    if (code) continue;

	    if (mask & EPIV_STATUS_NAME) {
		epix_EnterCodeBlock(19);
		bcopy((caddr_t)status->ident.name,
		       (caddr_t)v->disk.ident.name,
		       sizeof (v->disk.ident.name));
		MOD_FIELD_LEN(&buf, ident.name[0], sizeof(v->disk.ident.name));
		if (code) continue;
		newIdent = 1;
	    }
	    if (mask & EPIV_STATUS_ID) {
		epix_EnterCodeBlock(21);
		DFSH_DiskFromMemHyper(v->disk.ident.id, status->ident.id);
		MOD_FIELD_LEN (&buf, ident.id, sizeof(v->disk.ident.id));
		if (code) continue;
		newIdent = 1;
	    }
	    if (mask & EPIV_STATUS_FILESTATUS) {
		epix_EnterCodeBlock(39);
		bcopy((caddr_t)status->fileStatus,
		       (caddr_t)v->disk.fileStatus,
		       sizeof (v->disk.fileStatus));
		MOD_FIELD_LEN(&buf, fileStatus[0], sizeof(v->disk.fileStatus));
		if (code) continue;
	    }
	    if (mask & EPIV_STATUS_FILESTATUS_WORD_N) {
		int n = (mask >> EPIV_STATUS_FILESTATUS_N_SHIFT) & 0xf;
		if (n >= EPIV_STATUS_FILESTATUS_WORDS)
		    { ABORT_CRIT (EPI_E_BADARGS) }
		SET_FIELD (fileStatus[n], /*CodeBlock(*/46);
	    }
	    if (mask & EPIV_STATUS_UPLEVELIDS) {
		int i;
		epix_EnterCodeBlock(49);
		for (i=0; i<EPIV_STATUS_UPLEVELIDS_HYPERS; i++)
		    DFSH_DiskFromMemHyper(v->disk.upLevelIds[i],
					  status->upLevelIds[i]);
		MOD_FIELD_LEN(&buf, upLevelIds[0], sizeof(v->disk.upLevelIds));
		if (code) continue;
	    }
	    if (mask & EPIV_STATUS_UPLEVELIDS_WORD_N) {
		int n = (mask >> EPIV_STATUS_UPLEVELIDS_N_SHIFT) & 0xf;
		epix_EnterCodeBlock(50);
		afsl_Assert(n<EPIV_STATUS_UPLEVELIDS_HYPERS);
		DFSH_DiskFromMemHyper(v->disk.upLevelIds[n],
				      status->upLevelIds[n]);
		MOD_FIELD_LEN(&buf, upLevelIds[n],
			      sizeof(v->disk.upLevelIds[n]));
	    }
	    if (mask & EPIV_STATUS_MSG) {
		epix_EnterCodeBlock(20);
		bcopy((caddr_t)status->statusMsg,
		      (caddr_t)v->disk.statusMsg,
		       sizeof (v->disk.statusMsg));
		code = elbb_Modify (buf,
				    epic_IndexToOffset(1) +
				    VOL_2nd_FIELD_OFFSET(statusMsg[0]),
				    (caddr_t)v->disk.statusMsg,
				    sizeof(v->disk.statusMsg), trans,
				    !elbb_IsZeroTrans(trans), &buf);
		if (code) continue;
	    }
	    if (mask & EPIV_STATUS_FLAGS)
		{ SET_FIELD (flags, /*CodeBlock(*/29); }
	    if (mask & EPIV_STATUS_VERSION) {
		epix_EnterCodeBlock(41);
		DFSH_DiskFromMemHyper(v->disk.version, status->version);
		MOD_FIELD_LEN(&buf, version, sizeof(dfsh_diskHyper_t));
	    }
	    if (mask & EPIV_STATUS_UNIQUE)
		{ SET_FIELD (unique, /* CodeBlock(*/42); }
	    if (mask & EPIV_STATUS_BACKING_INDEX)
		{ SET_FIELD (backingVolIndex, /* CodeBlock(*/43); }
	    if (mask & EPIV_STATUS_BACKING_ID) {
		epix_EnterCodeBlock(44);
		DFSH_DiskFromMemHyper(v->disk.backingVolId,
				      status->backingVolId);
		MOD_FIELD_LEN(&buf, backingVolId, sizeof(dfsh_diskHyper_t));
	    }
	    if (mask & EPIV_STATUS_UPLEVELTIME_1)
		{ SET_FIELD (upLevelTime1, /*CodeBlock(*/53); }
	    if (mask & EPIV_STATUS_UPLEVELTIME_2)
		{ SET_FIELD (upLevelTime2, /*CodeBlock(*/54); }
	    if (mask & EPIV_STATUS_UPLEVELTIME_3)
	        { SET_FIELD (upLevelTime3, /*CodeBlock(*/55); }

	    code = UpdateMTime (trans, v, &buf, /*current*/1);
	    if (code) continue;
	} END_CRIT;
	if (code) continue;
	if (newIdent) {
	    code = epia_SetStatus (trans, h, 0, sizeof(v->disk.ident),
				   (opaque)&v->disk.ident);
	    if (code) continue;
	}
	v->updatedVersion = 0;
    } END_LOCK;
    if (code) return code;

#define Q_BOTH (EPIV_STATUS_Q_ALLOCATED|EPIV_STATUS_Q_VISIBLE)

    if (mask & Q_BOTH) {
	struct epiv_quota allocated, visible;

	epix_EnterCodeBlock(27);
	if ((mask & Q_BOTH) != Q_BOTH) {
	    /* get existing values if both not being set. */
	    code = q_Get (v, &allocated, &visible);
	    if (code) return code;
	}
	if (mask & EPIV_STATUS_Q_ALLOCATED)
	    allocated.limit =  status->allocated.limit;
	if (mask & EPIV_STATUS_Q_VISIBLE)
	    visible.limit = status->visible.limit;

	code = q_SetLimit (trans, h, allocated.limit, visible.limit);
	if (code) return code;
    }
    return 0;
}

/* epiv_QuiesceHeader -- Makes sure that updates to the volume header are
 *     serialized with respect to other operations on metadata in the same
 *     aggregate or committed to permanent storage before return.
 *
 * PARAMETERS --
 *     h -- fileset handle.
 *     consistency -- value indicating required consistency guarantee.
 *         ELBB_QUIESCE_SERIALIZE -- ensures all currently active transactions
 *             have ended.
 *         ELBB_QUIESCE_COMMIT -- as above, but waits until all transactions
 *             have also committed. */

/* EXPORT  */
long epiv_QuiesceHeader(epi_volume_t h, unsigned consistency)
{
    struct volume *v;
    long code;
    elbt_TranRec_t trans;

    afsl_Assert ((consistency == ELBB_QUIESCE_SERIALIZE) ||
		 (consistency == ELBB_QUIESCE_COMMIT));
    epix_EnterCodeBlock(56);
    if (!GetOKvol (v, h)) return EPI_E_BADVOLUME;
    BEGIN_TMUTEX_NT (&v->lock) {
	trans = tlock_GetTrans (&v->lock);
    } END_LOCK;
    /* Must not hold any locks while doing this. */
    code = elbb_LogForce (trans,
			  ((consistency == ELBB_QUIESCE_COMMIT) ? 1 : 2));
    return code;
}

/* epiv_TwiddleInconBit -- Handles transistions of the inconsistent bit,
 *	making sure that everything is serialized properly.
 *
 * PARAMETERS --
 *     h -- fileset handle.
 *     turnon -- true if fileset's \cst{EPIV_STATUS_FLAGS_INCONSISTENT} bit is
 *         to be turned on.  False if bit is to be cleared. */

/* EXPORT  */
long epiv_TwiddleInconBit(epi_volume_t h, int turnon)
{
    struct epiv_status vstat;
    buffer_tranRec_t transId;
    long code, code2;

    code = epiv_GetStatus(h, &vstat);
    if (code)
	return code;

    if (turnon) {
	code = epia_StartTran("epiv_TwiddleInconBit turnon", h, &transId);
	afsl_MBZ(code);
	vstat.flags |= EPIV_STATUS_FLAGS_INCONSISTENT;
	code = epiv_SetStatus(transId, h, EPIV_STATUS_FLAGS, &vstat);
	code2 = epia_EndTran(h, transId);
	afsl_MBZ(code2);
    } else {
	elbb_QuiesceSystem(epia_AnodeDevice((epi_anode_t)h),
			   ELBB_QUIESCE_SERIALIZE);

	code = epia_StartTran("epiv_TwiddleInconBit turnoff", h,
			      &transId);
	afsl_MBZ(code);
	vstat.flags &= ~EPIV_STATUS_FLAGS_INCONSISTENT;
	code = epiv_SetStatus(transId, h, EPIV_STATUS_FLAGS, &vstat);
	code2 = epia_EndTran(h, transId);
	afsl_MBZ(code2);
    }

    if (!code)
	code = epiv_QuiesceHeader(h, ELBB_QUIESCE_SERIALIZE);

    return code;
}

/* epiv_SuppressQuotaCheck -- suppresses quota checking on this volume until
 *     explicitly re-enabled.  However, this state is in-memory only, so on
 *     detach or reboot quota checking will be reenabled.
 *
 * PARAMETERS --
 *     volC -- fileset handle.
 *     on -- if true the fileset's quota checking will be disabled.  If false
 *         the normal checking will be re-enabled.
 */

/* EXPORT  */
long epiv_SuppressQuotaCheck(epi_volume_t volC, unsigned on)
{
    struct volume *v;

    epix_EnterCodeBlock(57);
    if (!GetOKvol (v, volC)) return EPI_E_BADVOLUME;
    q_NoCheck(v, on);
    return 0;
}

/* epiv_NewUnique -- If \arg{gen} is non-zero, generate a unique value
 *     associated with the \arg{index} in the specified fileset, otherwise (as
 *     during fileset restore) \arg{uniqueP} is stored into the fileset header
 *     if it is more recent than the fileset header's current value.
 *
 * PARAMETERS --
 *     trans -- provides the transaction in which this operation occurs.
 *     volC -- fileset handle.
 *     gen -- is a flag that is used to get a new unique value.
 *     index -- index within the fileset.
 *     uniqueP -- pointer to the new unique value generated.
 *
 * LOCKS USED -- Why doesn't this routine do any locking?  See OT 5967. */

/* EXPORT  */
long epiv_NewUnique (trans, volC, gen, index, uniqueP)
  IN elbt_TranRec_t trans;
  IN epi_volume_t volC;
  IN int gen;				/* generate new unique */
  IN u_long index;
  INOUT u_long *uniqueP;
{
    long code;
    struct volume *v;
    int base, offset;
    u_long unique;

    epix_EnterCodeBlock(28);
    if (!GetOKvol (v, volC)) return EPI_E_BADVOLUME;
    if (index) index = MapIndexToInternal(index);
    base = epic_IndexToOffset(0);
    offset = base + VOL_FIELD_OFFSET (unique);
    code = 0;
    BEGIN_TMUTEX (trans, &v->lock) {
	if (gen) {
	    unique = v->disk.unique + 1;
	    if (unique == 0) unique = 1; /* never return 0 */
	} else {
	    /* this is a wrap-around compare */
	    u_long ahead, behind;
	    epix_EnterCodeBlock(30);
	    unique = (u_long)*uniqueP;
	    if (unique == 0) return EPI_E_BADUNIQUE;
	    ahead = unique - v->disk.unique;
	    behind = v->disk.unique - unique;
	    if (behind < ahead) return 0; /* no update necessary */
	}
	WITH_HEADER (v, buf) {
	    code = elbb_Modify (buf, offset,
				(caddr_t)&unique, sizeof(v->disk.unique),
				trans, !elbb_IsZeroTrans(trans), &buf);
	    if (code) continue;
	    code = UpdateMTime (trans, v, &buf, /*!current*/0);
	} END_CRIT;
	if (code) continue;
	v->disk.unique = unique;
    } END_LOCK;
    *uniqueP = unique;
    return code;
}

/* epiv_NewVolumeVersion -- Obtains the next fileset version number for the
 *     specified fileset. This is used to generate a new value when modifying a
 *     file to set its file status volumeVersionNumber.
 *
 * PARAMETERS --
 *     trans -- provides the transaction in which this operation occurs.
 *     volC -- fileset handle.
 *     newVersionP -- pointer to the new fileset version number.
 */

/* EXPORT  */
long epiv_NewVolumeVersion(
  IN elbt_TranRec_t trans,
  IN epi_volume_t volC,
  OUT afs_hyper_t *newVersionP)
{
    struct volume *v;
    long code = 0;
    int got;

    epix_EnterCodeBlock(31);
    GetOKvol (v, volC);
    afsl_Assert (v);

    BEGIN_TMUTEX_NT (&v->lock) {
	/* Make sure we're linked to last VV update, then use current value */
	tmutex_EquivNoAssign (trans, &v->lock);
	if (got = v->updatedVersion) {
	    v->stats.skippedVV++;	/* not under write lock... */
	    DFSH_MemFromDiskHyper(*newVersionP, v->disk.version);
	}
    } END_LOCK;
    if (got)
	return code;

    BEGIN_TMUTEX (trans, &v->lock) {
	afs_hyper_t version;
	int offset;

	offset = epic_IndexToOffset(0) + VOL_FIELD_OFFSET (version);
	DFSH_MemFromDiskHyper(version, v->disk.version);
	AFS_hadd32(version, 1);
	DFSH_DiskFromMemHyper(v->disk.version, version);

	WITH_HEADER (v, buf) {
	    code = elbb_Modify (buf, offset, (caddr_t)&v->disk.version,
				sizeof(v->disk.version),
				trans, !elbb_IsZeroTrans(trans), &buf);
	    if (code) continue;
	    code = UpdateMTime (trans, v, &buf, /*!current*/0);
	    if (code) continue;
	} END_CRIT;
	if (code) continue;
	*newVersionP = version;
	v->updatedVersion = 1;
	v->stats.updatedVV++;
    } END_LOCK;
    return code;
}

/* epiv_CreateAnode -- Creates a new anode in a fileset and returns an open
 *     handle for a new container.  The \arg{indexP} points to the desired
 *     index if \cst{EPIV_CFLAGS_USEFID} bit is set in \arg{flags} or
 *     a new index will be allocated and returned in \arg{indexP}.  This
 *     index will be chosen to be close to the value of \arg{nearIndex} if
 *     the flag bit \cst{EPIV_CFLAGS_USENEARHINT} is set.  If the
 *     \cst{EPIV_CFLAGS_TYPE_FILE} is set then an index suitable for a
 *     {\unix} file will be allocated (i.e. an index that can be converted into
 *     a \glos{file number}; but this is still an epiv-stye index).  If the
 *     \cst{EPIV_CFLAGS_TYPE_AUX} is set then an index suitable for an
 *     ACL will be allocated (such an index cannot be mapped to a file number
 *     or inserted into a fid).  Other parameters and flag
 *     bits (e.g. \cst{EPIA_CFLAGS_COPYONWRITE}) are passed to
 *     \fcn{epia_Create}.
 *
 *     If \arg{backingAnodeC} is specified exactly one of
 *     \cst{EPIA_CFLAGS_BACKING} or \cst{EPIA_CFLAGS_COPYONWRITE} must be set
 *     (i.e. this is a clone operation).  If creating a COW anode, we update
 *     the fileset's visible quota.  See also the description of the
 *     corresponding parameters to \fcn{epia_Create}.
 *
 * PARAMETERS --
 *     trans -- provides the transaction in which this operation occurs.
 *     volC -- fileset in which the anode is to be created.
 *     flags -- controls various options.  Most flags accepted by
 *         \fcn{epia_Create} are legal.  In addition various values described
 *         below have special meaning.
 *     statusSize -- size of the status information, in bytes.
 *     estLength -- estimated length of container (unused).
 *     backingAnodeC -- fileset container of backing source if the new
 *         container is copy-on-write or backing.
 *     backingIndex -- index in \arg{backingAnodeC}.
 *     nearIndex -- neighboring index.
 *     indexP -- index of the new container.
 *     anodeP -- pointer to the anode handle for the new container.  This
 *         handle is zeroed on any error.
 */

/* EXPORT  */
long epiv_CreateAnode(
  IN elbt_TranRec_t trans,
  IN epi_volume_t volC,
  IN long flags,
  IN unsigned int statusSize,
  IN unsigned long estLength,
  IN epi_anode_t backingAnodeC,
  IN u_long backingIndex,
  IN u_long nearIndex,
  INOUT u_long *indexP,
  OUT epi_anode_t *anodeP)
{
    long code;
    struct volume *v;
    epi_anode_t h;			/* new anode's handle */
    afs_hyper_t hZero;
    int type;				/* flags to epit_Allocate */

    epix_EnterCodeBlock(32);
    *anodeP = 0;
    if (flags & (EPIA_CFLAGS_TYPE | EPIA_CFLAGS_PREALLOCATED))
	return EPI_E_BADFLAGS;
    if (!GetOKvol (v, volC)) return EPI_E_BADVOLUME;
    afsl_Assert ((v->flags & VOL_OFFLINE) == 0);

    if (flags & EPIV_CFLAGS_USEFID) {
	/* Use the requested index */
	epix_EnterCodeBlock(33);
	if (flags & EPIV_CFLAGS_USENEARHINT) return EPI_E_BADFLAGS;
	if (!*indexP) return EPI_E_BADANODEINDEX;
	nearIndex = MapIndexToInternal(*indexP);
    }
    else if (flags & EPIV_CFLAGS_USENEARHINT) {
	if (!nearIndex) return EPI_E_BADANODEINDEX;
	nearIndex = MapIndexToInternal(nearIndex);
    }
    else if (nearIndex) return EPI_E_BADARGS;

    if (backingIndex) backingIndex = MapIndexToInternal(backingIndex);

    if (flags & EPIV_CFLAGS_TYPE_FILE) type = EPIT_TYPE_FILE;
    else if (flags & EPIV_CFLAGS_TYPE_AUX) type = EPIT_TYPE_AUX;
    else /* both zero means don't care, both set is illegal. */
	type = EPIT_TYPE_DONTCARE;
    if (backingAnodeC)
	type |= EPIT_DONT_WITHHOLD;

    flags |= EPIA_CFLAGS_REGULAR;

    AFS_hzero (hZero);

    BEGIN_TMUTEX (trans, &v->lock) {

	/* It is important to use the trans in obtaining the tlock here to
         * merge creates on the same volume table page.  This should be fixed
         * by db2568. */

	code = CheckVT (v, volC);	/* ensure voltable is init'd */
	if (!code)
	    v->openAnodes++;
    } END_LOCK;
    if (code)
	return code;

    BEGIN_LCRIT (epia_quota_t delta,
		 delta.a = delta.v = 0,
		 if (code && (delta.a || delta.v)) {
		     long code;
		     delta.a = -delta.a;
		     delta.v = -delta.v;
		     code = q_Update (trans, volC, delta, 1 /*nocheck*/);
		     afsl_MBZ (code);
		 }) ;
    
    /* Get backing anode's size so we can update/check visible quota of new
     * COW anode.  Otherwise we do a check with delta.[av]==0 which only
     * fails if we are over quota, thereby preventing all creates in this
     * case. */
    
    if (flags & EPIA_CFLAGS_COPYONWRITE) {
	epi_anode_t b;
	code = epia_Open (backingAnodeC, backingIndex, 0, 0, &b);
	if (code) continue;
	delta.v = epia_GetVisible(b);
	code = epia_Close (b);
	afsl_MBZ (code);
    }
    code = q_Update (trans, volC, delta, 0);
    if (code) {
	/* don't give any back on exit. */
	delta.a = delta.v = 0;
	continue;
    }
    
    BEGIN_LCRIT (u_long index = 0,
		 code = epit_Allocate (trans, volC, type, nearIndex,
				       &index),
		 if (code) {	/* give index back */
		     long ccode = epit_Free (trans, volC, index);
		     afsl_MBZ (ccode);
		 }) ;
    
    if (flags & EPIV_CFLAGS_USEFID) {
	if (nearIndex != index) { ABORT_CRIT (EPI_E_INDEXEXISTS); }
    }
    
    /* Note that when creating a backing or COW anode we can not recover
     * from errors by calling epia_Delete, so we must do quota checking
     * above.  There are no significant failures after the create call
     * succeeds. */
    
    code = epia_Create (trans, volC, index, flags, statusSize,
			estLength, hZero, backingAnodeC, backingIndex, &h);
    if (code) continue;
    
    epia_SetVolumeRock (h, volC, 0);
    epia_SetQuotaProc(h, q_Proc);
    epia_SetAllocationHandle (h, epia_GetAllocationHandle(volC));
    *anodeP = h;
    *indexP = MapIndexToExternal(index);
    
    END_CRIT /* allocate */;
    END_CRIT /* quota */;

    if (code) {
	BEGIN_TMUTEX_NT (&v->lock) {
	    v->openAnodes--;
	} END_LOCK;
	return code;
    }
    return 0;
}

/* epiv_DeleteAnode -- This works just like the function \fcn{epia_Delete},
 *     but also updates the fileset in which the anode resides.  This must be
 *     used to delete anodes created with \fcn{epiv_CreateAnode}.
 *
 * PARAMETERS --
 *     trans -- provides the transaction in which this operation occurs.
 *     anodeC -- anode to be deleted. */

/* EXPORT  */
long epiv_DeleteAnode(elbt_TranRec_t trans, epi_anode_t anodeC)
{
    long code;
    epi_volume_t vol;
    struct volume *v;
    u_long index = epia_GetIndex (anodeC);

    epix_EnterCodeBlock(35);
    vol = (epi_volume_t)epia_GetVolumeRock (anodeC);
    if (!GetOKvol (v, vol)) return EPI_E_BADVOLUME;
    BEGIN_TMUTEX (trans, &v->lock) {

	/* It is important to use the trans in obtaining the tlock here to
         * merge creates on the same volume table page.  This should be fixed
         * by db2568. */

	code = CheckVT (v, vol);
	if (!code) {
	    afsl_Assert(v->openAnodes > 0);
	    v->openAnodes--;
	}
    } END_LOCK;
    if (code)
	return code;

    code = epia_Delete (trans, anodeC);
    if (code) {
	BEGIN_TMUTEX_NT (&v->lock) {
	    v->openAnodes++;
	} END_LOCK;
	return code;
    }

    code = epit_Free (trans, vol, index);
    afsl_MBZ (code);

    return 0;
}

/* epiv_OpenAnode -- Opens a container given its \arg{index} in the fileset
 *     \arg{volC}.  It associates the appropriate allocation handle and quota
 *     procedure with the returned handle.  The matching procedure,
 *     \fcn{epiv_CloseAnode}, must be called when done.
 *
 * PARAMETERS --
 *     volC -- fileset handle.
 *     index -- index of the container to be opened.
 *     hP -- pointer to the handle for the opened anode.  This handle is zeroed
 *         on any error.
 *
 * LOCKS USED -- This procedure obtains an exclusive (write) lock on the volume
 *     handle's lock but does not supply a transaction, since only in-memory
 *     updates are being made. */

/* EXPORT  */
long epiv_OpenAnode(
  IN epi_volume_t volC,
  IN u_long index,
  OUT epi_anode_t *hP)
{
    long code;
    struct volume *v;

    epix_EnterCodeBlock(36);
    *hP = 0;
    if (index == 0) return EPI_E_BADANODEINDEX;

    index = MapIndexToInternal(index);

    if (!GetOKvol (v, volC)) return EPI_E_BADVOLUME;
    afsl_Assert ((v->flags & VOL_OFFLINE) == 0);

    code = epia_Open (volC, index,
		      epia_GetAllocationHandle(volC), q_Proc, hP);
    if (code)
	return code;
    epia_SetVolumeRock (*hP, volC, 0);

    BEGIN_TMUTEX_NT (&v->lock) {
	v->openAnodes++;
    } END_LOCK;

    return 0;
}

/* epiv_CloseAnode -- Closes the specified anode.  This must be used when the
 *     caller is through using an anode handle obtained from
 *     \fcn{epiv_OpenAnode}.
 *
 * PARAMETERS -- anodeC is anode being closed. */

/* EXPORT  */
long epiv_CloseAnode(epi_volume_t anodeC)
{
    long code;
    epi_volume_t vol;
    struct volume *v;

    epix_EnterCodeBlock(37);
    if (!anodeC) return EPI_E_BADARGS;
    vol = (epi_volume_t)epia_GetVolumeRock (anodeC);
    if (!GetOKvol (v, vol)) return EPI_E_BADVOLUME;
    code = epia_Close (anodeC);
    if (code)
	return code;

    BEGIN_TMUTEX_NT (&v->lock) {
	afsl_Assert(v->openAnodes > 0);
	v->openAnodes--;
    } END_LOCK;
    return 0;
}

/* epiv_OpenBackingAnode -- Opens the backing container corresponding to
 *     \arg{anodeC}.
 *
 * PARAMETERS --
 *     anodeC -- anode handle.
 *     vP -- pointer to backing fileset.
 *     hP -- pointer to backing anode.
 *
 * LOCKS USED -- Necessary locking is assumed to be performed by higher levels.
 */

/* EXPORT  */
long epiv_OpenBackingAnode(
  IN epi_anode_t anodeC,
  OUT epi_volume_t *vP,			/* desired backing volume */
  OUT epi_anode_t *hP)			/* desired backing anode */
{
    long code;
    struct epia_info info;
    epi_anode_t avl;

    epix_EnterCodeBlock(40);
    code = epia_GetInfo (anodeC, &info);
    if (code) return code;
    if (!(info.flags & EPIA_CFLAGS_COPYONWRITE) ||
	!info.backingVolIndex || !info.backingIndex)
	return EPI_E_NOTCOPYONWRITE;
    code = epiv_GetAggregate (epiv_GetVolume (anodeC), &avl);
    if (code) return code;
    BEGIN_CRIT (code = epiv_Open (avl, info.backingVolIndex, vP),
		if (code) code = epiv_Close (elbb_zeroTrans, *vP)) {
	code = epiv_OpenAnode (*vP, MapIndexToExternal(info.backingIndex), hP);
    } END_CRIT;
    return code;
}

/* epiv_LastIndex -- Returns the highest or last epiv-style index allocated in
 *     a fileset.
 *
 * PARAMETERS --
 *     volC -- fileset handle.
 *     indexP -- pointer to the last index value. */

/* EXPORT  */
long epiv_LastIndex(epi_volume_t volC, u_long *indexP)
{
    struct volume *v;
    long code;

    epix_EnterCodeBlock (38);
    *indexP = 0;
    if (!GetOKvol (v, volC)) return EPI_E_BADVOLUME;

    BEGIN_TMUTEX_NT (&v->lock) {
	if (code = CheckVT (v, volC)) continue;
	code = epit_LastIndex (volC, indexP);
    } END_LOCK;
    if (code) return code;

    afsl_Assert(*indexP);			/* zero is not a legal index */
    *indexP = MapIndexToExternal(*indexP);
    return code;
}

/* epiv_Deflate -- Called before deleting a fileset to wring out all the excess
 *     pages in the fileset anode table.
 *
 * PARAMETERS -- volC is a fileset handle.
 *
 * CAUTIONS -- The caller must not have started a transaction when this is
 *     called. */

/* EXPORT  */
long epiv_Deflate(epi_volume_t volC)
{
    struct volume *v;
    long code;

    if (!GetOKvol (v, volC)) return EPI_E_BADVOLUME;
    do {
        code = 0;
        BEGIN_LCRIT (elbt_TranRec_t trans,
                     code = epia_StartTran ("epiv_Deflate", volC, &trans),
                     code = epia_EndTran (volC, trans)) {
            BEGIN_TMUTEX (trans, &v->lock) {
                code = epit_Deflate (trans, volC);
            } END_LOCK;
        } END_CRIT;
    } while (code == EPI_E_NOTDONE);
    return code;
}

#ifndef KERNEL
#include "verify.h"
#include "salvage.h"

/*
** VerifyVolume
**
**   validate this volume's contribution to the bitmap
**   verify that all file anodes have acl/prop lists in the volume-table
**   verify that there are no unnecessary such lists
*/

/* SHARED */
long epiv_VerifyVolume(ss, AvlAnode, ix)
  IN struct epiz_salvageState *ss;
  IN epi_anode_t AvlAnode;
  IN long ix;
{
    struct epiz_SimBits *verif = ss->simBits;
    epi_anode_t Vol;
    long code, type;
    char *typeName;
    int CheckTheAnode;

    code = epia_Open(AvlAnode, ix, 0, 0, &Vol);
    if (code) return code;
    ss->vol = Vol;
    ss->qVolInfo = 0;

    CheckTheAnode = 1;			/* only bad blocks doesn't check */

    type = epia_GetType(Vol);
    switch (type) {
      case EPIA_CFLAGS_BADBLOCKS:
	CheckTheAnode = 0;
	/* fall through */
      case EPIA_CFLAGS_AVL:
      case EPIA_CFLAGS_BITMAP:
      case EPIA_CFLAGS_LOG:
	typeName = epiz_specialAnodeName[type];
	break;
      case EPIA_CFLAGS_VOLUME:
	typeName = 0;
	break;
      default:
	epiz_ReportError (ss, 0, EPIZ_E_AVL_ILLEGALTYPE, type);
	code =  epia_Close(Vol);
	afsl_MBZ (code);
	return 0;
    }

    if ((ss->pass == EPIZ_SS_SCANPASS) && typeName) {
	long fix = ss->specialIx[type];
	long eix = ss->expectedIx[type];

	afsl_Assert (type < sizeof (ss->specialIx)/sizeof(ss->specialIx[0]));
	if (ix != ss->expectedIx[type])
	    epiz_ReportError (ss, 0, EPIZ_E_AVL_BADSPECIALINDEX,
			      typeName, ss->expectedIx[type], ix);
	if (ss->specialIx[type] != -1)
	    epiz_ReportError (ss, 0, EPIZ_E_AVL_DUPLICATESPECIAL,
			      typeName, ix, ss->specialIx[type]);
	if (ss->specialIx[type] != ss->expectedIx[type])
	    ss->specialIx[type] = ix;
    }

    if (CheckTheAnode) {
	ss->expectedType = type;
	code = epia_VerifyAnode(ss, AvlAnode, ix);
	if (code) {
	    long ccode;
	    ccode = epia_Close(Vol);
	    afsl_MBZ (ccode);
	    return code;
	}
    }
    if (type == EPIA_CFLAGS_VOLUME) {
	struct epiv_status volInfo;

	code = epit_InitVolumeTable(Vol, RESERVED_ANODES);
	if (code != 0) {(void) epia_Close(Vol); return code;}
	ss->expectedType = EPIA_CFLAGS_REGULAR;
	ss->quotaVal.a = ss->quotaVal.v = 0;

	/*
	** Note: we actually place the Info structure into the ss.
	** That way, we can reference the info anywhere in the
	** other enumerate's.
	*/

	BEGIN_LCRIT( epi_volume_t volC,
		code = epiv_Open(AvlAnode, ix, &volC),
		code = epiv_Close( elbb_zeroTrans, volC ) ) {

	    ss->qVolInfo = (opaque)&volInfo;
	    if (0 != (code = epiv_GetStatus (volC, ssVolInfo(ss)))) {
		printf( "epiv_VerifyVolume: epiv_GetStatus (%d)\n", code );
		continue;
	    }
	} END_CRIT;
	if (code != 0) { (void) epia_Close( Vol ); return (code); }

	if (ss->pass == EPIZ_SS_SCANPASS) ss->nVolumes++;

	code = epit_VerifyVolumeTable (ss, Vol, epia_VerifyAnode, (char*)ss);
	if (code != 0) {(void) epia_Close(Vol); return code;}

	/* check allocated size against computed size */
	if ((CheckTheAnode) && (type == EPIA_CFLAGS_VOLUME)) {
	    /* add the size of the volume itself, compare values */
	    struct epia_info aInfo;
	    code = epia_GetInfo( Vol, &aInfo );
	    if (code != 0) {(void) epia_Close(Vol); return (code);}

	    ss->quotaVal.a += aInfo.allocated;
	    ss->quotaVal.v += aInfo.visible;
	    if ((ss->quotaVal.a != ssVolInfo(ss)->allocated.usage) ||
		(ss->quotaVal.v != ssVolInfo(ss)->visible.usage)) {
		epiz_ReportError (ss, 0, EPIZ_E_VOLQUOTA,
				  ssVolInfo(ss)->allocated.usage,
				  ssVolInfo(ss)->visible.usage,
				  ss->quotaVal.a, ss->quotaVal.v);
		if (ss->flags & EPIZ_SS_SALVAGE) {
		    epia_quota_t delta;
		    delta.a = ss->quotaVal.a - ssVolInfo(ss)->allocated.usage;
		    delta.v = ss->quotaVal.v - ssVolInfo(ss)->visible.usage;
		    BEGIN_LCRIT( epi_volume_t volC,
			    code = epiv_Open(AvlAnode, ix, &volC),
			    code = epiv_Close( elbb_zeroTrans, volC ) ) {

			code = q_Update (elbb_zeroTrans, volC, delta, 1);
			if (!code) {
			    /* refresh memory copy of this data */
			    code = epiv_GetStatus (Vol, ssVolInfo(ss));
			}
			if (code)
			    epiz_ReportError (ss, code, EPIZ_E_VOLQUOTAFAILED);
		    } END_CRIT;
		    if (code != 0) { (void) epia_Close( Vol ); return (code); }
		}
	    }
	}

	/* Remove the reference to an out-of-scope structure */
	ss->qVolInfo = 0;
    }
    code = epia_Close(Vol);
    return (code);
}
#endif /* not KERNEL */
