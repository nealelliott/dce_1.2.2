/* Copyright (C) 1996, 1994 Transarc Corporation - All rights reserved. */

/* Episode File System -- low-level vnode management routines */

/* The external procedures in this module use the "vnm_" prefix and therefore
 * share this prefix with the routines in the file efs_misc.c for historical
 * reasons. */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/debug.h>

#include <dcedfs/lock.h>		/* osi_dlock_t */
#include <dcedfs/hash.h>
#include <dcedfs/episode/fifo.h>
#include <dcedfs/episode/file.h>	/* struct epif_status */
#include <dcedfs/osi_kvnode.h>
#include <dcedfs/dir_buffer.h>		/* dirb_Annul */
#include <dcedfs/episode/dacl_anode.h>	/* dacl_ReleaseAcl */
#include <dcedfs/episode/anode.h>	/* EFS_HOLE */

#include "efs_evnode.h"			/* struct evnode & ICL dcls */
#include "efs_volvnops.h"		/* volume private data primitives */
#include "efs_misc.h"			/* function prototypes */
#include "efs_dirhandle.h"

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/vnops/RCS/efs_vnode.c,v 1.182 1996/03/04 19:04:22 ota Exp $")

#define DEBUG_THIS_FILE EFS_DEBUG_VNODE

/* vn_count -- returns the evnode's refCount without locking. */

#define vn_count(evp) (EVTOV(evp)->v_count)

/* Global vnode lock and associated structures: hash table and lru list. */

static epi_mutex_t vntableLock;		/* lock for hash table */
static struct dfsh_hashTable vntable;	/* all vnodes with identities */
static long vntableLabel;		/* counter used by vnode iteration */

static long vnCount;			/* number of allocated vnodes */
static long vnCountTarget;		/* preferred number of vnodes */
static long vnCountMax;			/* max to allocate; ENFILE instead */

static struct fifo lruList;

/* To avoid losing stale, but held vnodes, put them on the staleList fifo using
 * the lru thread.  If !onHash && !onLRU then vnode will be on this list. */

static struct fifo staleList;

#define REMOVE_LRU(evp) \
    MACRO_BEGIN \
	if ((evp)->vd_onLRU) { \
	    FIFO_REMOVE (&lruList, &(evp)->vd_lru); \
	    (evp)->vd_onLRU = 0; \
	    if (!(evp)->vd_onHash) \
		FIFO_ADD (&staleList, &(evp)->vd_lru); \
	} \
    MACRO_END

#if 0
/* openVolList -- a list of openData structures identifying all open volumes
 *     and some associated state. */

typedef struct openData {
    struct openData *next;
    afs_hyper_t volid;
    int openbits;			/* combination of STOPUSE_NO_* bits */
#ifdef AFS_AIX_ENV
    struct buf *sideRail;
#endif
} openData_t;

static openData_t *openVolList;
#endif

/* VnodeHashEntry -- returns a hash value for a vnode.  It is used by hash
 *     table module.  Adapted from fixed_anode.c:HandleHashEntry. */

#define HASH_MULTIPLIER ((unsigned)0x10204081) /* odd with many bits on */
#define VNODE_HASH_ENTRY(volid, fid)	/* XXX What use is the big prime?? */ \
    (((1000003 * HASH_MULTIPLIER +	/* big prime to start things nicely */\
       (u_long)AFS_hgethi(volid)) * HASH_MULTIPLIER + \
      (u_long)AFS_hgetlo(volid)) * HASH_MULTIPLIER + \
     (u_long)epid_IndexPart(fid))

static unsigned long VnodeHashEntry(efs_vnode_t evp)
{
    unsigned long hash = VNODE_HASH_ENTRY(evp->vd_volid, &evp->vd_fid);
    return hash;
}

static efs_vnode_t HashNext (
  efs_vnode_t evp,
  long *stateP)				/* state variable */
{
    efs_vnode_t next;
    static int restartRehashes;		/* count these for code coverage */
    int restartRehash;

    Assert (epi_mutex_held(&vntableLock));

    restartRehash = (vntable.rehashCnt != *stateP);
    if (!evp || !evp->vd_onHash || restartRehash) {

	/* Either we're just starting the iteration or the entry we're given is
	 * no longer on the hash table or the hash table has been reorganized
	 * since we looked up the last entry.  We start over by reseting our
	 * state variable and clearing evp.  We keep the previous rehash count
	 * in *stateP so we can detect reorganizations. */

	if (restartRehash)
	    restartRehashes++;
	*stateP = vntable.rehashCnt;
	next = dfsh_HashNext (&vntable, NULL);

	/* If there was a reorg we might find the evp again, if so try again to
	 * get its next. */

	if (evp == next)
	    next = dfsh_HashNext (&vntable, next);
    } else {

	/* Now look up next entry.  If there isn't one then we can claim to
	 * have finished the iteration by returning NULL. */

	next = dfsh_HashNext (&vntable, evp);
    }
    Assert (!next || (next != evp));
    return next;
}

static efs_vnode_t HashLookup (
  afs_hyper_t volid,
  epi_volFileId_t *fid)
{
    efs_vnode_t evp;
    DFSH_LOOKUP (efs_vnode_t, &vntable, evp, VNODE_HASH_ENTRY(volid, fid),
		 (VOLID_EQ(volid, evp->vd_volid) &&
		  (epid_IndexPart(fid) == epid_IndexPart(&evp->vd_fid))));
    return evp;
}

int vnm_Init (
  u_long nVnodes)
{
    int code;

    epi_mutex_init (&vntableLock, "vnode table");

    /* Setup the vnode hash table.  The hash function is based on the volume id
     * (64bits) and the index part of the fid (32bits).  We insist that volid
     * and fid are adjacent and that index is the first word of the fid. */

    Assert (offsetof (struct evnode, vd_fid.index) -
	    offsetof (struct evnode, vd_volid) == 2*sizeof(u_int32));
    code = dfsh_HashInit (&vntable, VnodeHashEntry,
			  offsetof (struct evnode, vd_hashNext));
    if (code) return code;
    code = dfsh_HashInitKeyFixed
	(&vntable, offsetof(struct evnode, vd_volid), 3*sizeof(u_int32));
    if (code)
	return code;

    FIFO_INIT (&lruList, offsetof(struct evnode, vd_lru));
    FIFO_INIT (&staleList, offsetof(struct evnode, vd_lru));

    vnCountTarget = nVnodes;


#if	!defined(AFS_SUNOS5_ENV) && !defined(AFS_AIX_ENV)
    /* Initialize eb freelist lock */
    lock_Init (&eblock);
#endif	/* AFS_SUNOS5_ENV, AFS_AIX_ENV */

    return 0;
}

/* ClearIdentity -- removes the identity of a vnode.  We also reset
 *     some minimal part of the vnode's state.
 *
 * LOCKS USED -- Insists that idLock is held since we're changing it.
 *     Other per-vnode locks are held too. */

static void ClearIdentity (
  efs_vnode_t evp)
{
    int code;

    Assert (lock_Check (&evp->vd_idLock) &&
	    lock_Check (&evp->vd_vm.lock) &&
	    lock_Check (&evp->vd_file.lock) == -1 &&
	    lock_Check (&evp->vd_cache.lock) == -1 &&
	    (evp->vd_volp->v_count > 0));

    /* Make sure tlock has already been recycled. */
    Assert (elbb_IsZeroTrans(tlock_GetTrans(&evp->vd_tlock)));

    evp->vd_flags &= ~VD_DIRDELETED;

    if (evp->vd_avoidRecycle) {

	/* Any blocked pageins will fail, so it is okay to release them.
	 * Calling this function wakes up everyone but this shouldn't be a
	 * frequent hit. */

	evp->vd_vm.noWritable = evp->vd_vm.noReadable = 0;
	efs_WakeupBlockedPageIns (evp->vd_volp);
    }

    BEGIN_MUTEX (&vntableLock) {
	Assert ((vn_count(evp) > 0) && !NoIdentity(evp));

	code = dfsh_HashOut_r (&vntable, evp);
	MBZ (code);
	epid_MakeInvalid(&evp->vd_fid);
	AFS_hzero(evp->vd_volid);
	evp->vd_volp = NULL;
	evp->vd_onHash = 0;
	if (!evp->vd_onLRU)
	    FIFO_ADD (&staleList, &evp->vd_lru);

	evp->vd_avoidRecycle = 0;	/* we may still have a VM seg, tho */

	Assert (!evp->vd_file.anode && !EV_ISDIRTY(evp) &&
		!evp->vd_vm.valid && !evp->vd_vm.dirty);
    } END_LOCK;

    /* All the restriction bits are initialized by SetRestrictions the other
     * bits were asserted to be in their proper state above. */
}

/* KillCachedStatus -- invalidates all vnode cached state. */

static void KillCachedStatus (
  efs_vnode_t evp)
{
    epi_anode_t ap;

    AssertWriteLocked (&evp->vd_cache.lock);

    if (!evp->vd_file.anode) {
	afsl_Assert (!evp->vd_acl && !EV_ISDIRTY(evp));
	return;
    }

    ap = evp->vd_file.ap;
    evp->vd_cache.newAtime = evp->vd_cache.newMtime =
	evp->vd_cache.newCtime = evp->vd_cache.newLen = 0;
    evp->vd_cache.lastAtime = epif_GetATime (ap);
    evp->vd_cache.lastMtime = epif_GetMTime (ap);
    evp->vd_cache.lastCtime = epif_GetCTime (ap);
    evp->vd_cache.dataVersion = epif_GetDV (ap);

    /* flush directory buffer cache unconditionally */
    if (EV_ISDIR(evp)) {
	struct epi_Dir_Handle dh;
	vndh_SetDirHandle(evp, &dh);
	dirb_Annul((struct Dir_Handle *)&dh);
    }

    /* Invalidate cached ACL if necessary */
    if (evp->vd_acl) {
	eacl_ReleaseAcl(evp->vd_acl);
	evp->vd_acl = 0;
    }
    if (evp->vd_cred) {
	icl_Trace1(efs_iclSet2, 
		   EFS_TRACE_ACCESS_CACHE_KILLCACHEDSTATUS_INVALIDATE,
		   ICL_TYPE_POINTER, evp);
	crfree(evp->vd_cred);
	evp->vd_cred = NULL;
	evp->vd_access = 0;
    }
}

/* FlushCachedStatus -- changes the state of information cached in the
 *     vnode.  This routine is harmless to call on a stale vnode.  In
 *     addition to being called when the vnode is changing state (e.g.
 *     Recycle) it should also be called opportunistically to write
 *     through update (e.g. when allocating blocks).
 *
 * PARAMETERS -- level is used to control whether the data should be
 *     killed, written through, or written and invalidated (okay sue me,
 *     I couldn't resist using bit encodings).
 *	   0 -- no effect.
 *	   1 -- clean cached status by writing through updates.
 *	   2 -- kill cached status without writing any updates through.
 *	   3 -- clean and invalidate cached status.
 *
 * LOCKS USED -- must be called with fileLock held for write. */

#define CACHE_CLEAN 1
#define CACHE_KILL  2
#define CACHE_CLEAR (CACHE_CLEAN|CACHE_KILL)

static void FlushCachedStatus (
  efs_vnode_t evp,
  int level)
{
    if ((level & CACHE_CLEAN) && EV_ISDIRTY(evp)) {
	vnm_UpdateAnode (evp, elbb_zeroTrans);
    }

    if (level & CACHE_KILL) {
	BEGIN_LOCK_WRITE (&evp->vd_cache.lock) {
	    KillCachedStatus (evp);
	} END_LOCK;
    }
}

/* Recycle -- destroys a vnode making it NOIDENTITY.  This operation
 *     ignores vnodes that are in use.  It will also eschew a vnode if
 *     it has a VM segment and PageIns are blocked.
 *
 *     First it grabs the idLock.  If the reference count is one then Recycle
 *     knows that no new users can gain access to the vnode or its VM since
 *     vnm_FindVnode will block on the idLock.  Thus the dirty pages can be
 *     written through and invalidated and it will stay that way (on AIX the VM
 *     segment will also be deleted).
 *
 *     On AIX -- We must check whether a volume operation has blocked PageIns.
 *         This check is made under the vmLock but this lock cannot be held
 *         while flushing VM.  Holding the idLock, however, will prevent
 *         StopUse from changing this.
 *
 *     We grab the fileLock and flush other cached vnode state such as
 *     atimes and ACLs.
 *
 *     Then we close the anode handle.
 *
 *     Finally, we call ClearIdentity which grabs the vntableLock
 *     obliterates the vnode's identity, removing it from the hash
 *     table.  When the caller drops the reference count to zero the
 *     vnode will be available for reuse.
 *
 * RETURN CODES -- returns true if the vnode was successfully recycled and the
 *     call has exclusive access to the resulting NoIdentity vnode. */

static int Recycle (
  efs_vnode_t evp)
{
    int code;
    int exclusive = 0;			/* caller has exclusive use of vnode */

    BEGIN_LOCK_WRITE (&evp->vd_idLock) {
	int notOkay;
	int noRecycle;

	BEGIN_LOCK (efs_lockvp(evp),
		    efs_unlockvp(evp)) {
	    noRecycle = ((vn_count(evp) > 1) || NoIdentity(evp));
	} END_LOCK;

	if (noRecycle) {
	    /* We are *not* the only user or *already* recycled. */
	    HERE (evp,w_norecycle);
	    EXIT_CRIT;
	}

	BEGIN_LOCK_WRITE (&evp->vd_vm.lock) {
	    notOkay = (evp->vd_vm.seg && evp->vd_vm.noWritable);
	} END_LOCK;
	if (notOkay) {

	    /* On AIX -- PageIns will block so we cannot delete the vm
	     *     segment without risk of deadlock. */

	    Assert (evp->vd_avoidRecycle);
	    EXIT_CRIT;
	}
	HERE (evp,w_recycle);

	/* Record recycle trace. Cheat (no locks) on the vnode ref count. */
	icl_Trace4(efs_iclSet, EFS_TRACE_RECYCLE,
		   ICL_TYPE_POINTER, evp,
		   ICL_TYPE_LONG, AFS_hgetlo(evp->vd_volid),
		   ICL_TYPE_LONG, epid_IndexPart(&evp->vd_fid),
		   ICL_TYPE_LONG, epid_UniquePart(&evp->vd_fid));

	/* Now flush dirty data and delete all pages.  On AIX, we must
	 * not hold the vmLock while we do this because PageIn grabs it
	 * and vnvm_recycle will wait for pending PageIns.  Since no
	 * users have this vnode we can safely update the VM related
	 * state bits upon return. */

	code = vnvm_recycle(EVTOV(evp), osi_credp);
	MBZ (code);			/* sidetrack this vnode? */

	BEGIN_LOCK_WRITE (&evp->vd_vm.lock) {

	    evp->vd_vm.valid = 0;
	    evp->vd_vm.dirty = 0;
	    evp->vd_vm.seg = 0;

	    ASSERT_VMBITS_CONSISTENT(evp);

	    BEGIN_LOCK_WRITE (&evp->vd_file.lock) {

		/* Clean out the vnode tlock */
		tlock_Recycle (&evp->vd_tlock);

		/* Write through cached atime, etc.  Invalidate
		 * directory buffers and acl. */
		FlushCachedStatus (evp, CACHE_CLEAR);

		if (evp->vd_file.anode) {
		    code = epif_Close(evp->vd_file.ap);
		    MBZ (code);
		    evp->vd_file.anode = 0;
		}
		BEGIN_LOCK_WRITE (&evp->vd_cache.lock) {
		    afsl_Assert (!EV_ISDIRTY(evp));
		    ClearIdentity (evp);

		    if (!evp->vd_onLRU) {

			/* Caller has removed vnode from the LRU list and we
                         * have just removed it from the hash table.  If the
                         * reference count is (still) one then the caller has
                         * the only possible reference to this vnode. */

			BEGIN_LOCK (efs_lockvp(evp),
				    efs_unlockvp(evp)) {
			    exclusive = (vn_count(evp) == 1);
			} END_LOCK;
		    }
		} END_LOCK; /* drop cacheLock */
	    } END_LOCK; /* drop fileLock */
	} END_LOCK; /* drop vmLock */
    } END_LOCK; /* drop idLock */
    return exclusive;
}

static void KillVnode (
  efs_vnode_t evp)
{
    Assert ((vn_count(evp) == 1) && NoIdentity(evp) && !evp->vd_onLRU);

    HERE(evp,w_init);
    tlock_Destroy (&evp->vd_tlock);
    lock_Destroy (&evp->vd_idLock);
    lock_Destroy (&evp->vd_vm.lock);
    lock_Destroy (&evp->vd_file.lock);
    lock_Destroy (&evp->vd_cache.lock);
#if !defined(KERNEL) || defined(AFS_AIX_ENV)
    lock_Destroy (&evp->refCountMutex);
#endif
#if defined(AFS_AIX_ENV)
    lock_Destroy (&evp->vmmLenLock);
#endif

    /* Do system dependent destruction (if any?) */
/*    osi_vn_kill (EVTOV(vp)); */
}

static void InitVnode(efs_vnode_t evp)
{
    struct vnode *vp = EVTOV(evp);

    bzero((opaque)evp, sizeof (*evp));
    epid_MakeInvalid(&evp->vd_fid);

#ifdef AFS_AIX_ENV
    evp->vd_seg = 0;
    /* Must set up gnode before setting v_op, v_type, etc */
    vp->v_gnode = &evp->vd_aixgnode;
    vp->v_gnode->gn_vnode = vp;
    vp->v_next = vp;
    vp->v_mvfsp = (struct osi_vfs *)0;
#endif

    OSI_VN_INIT (vp, NULL, /*noop*/osi_vType(vp), (dev_t)0, evp);
    osi_SetVnodeOps(vp, epii_GetEPIOps());

#if !defined(AFS_AIX_ENV) && !defined(AFS_OSF_ENV)
    vp->v_vfsmountedhere = (struct osi_vfs *) 0;
#endif	/* !AFS_AIX_ENV && !AFS_OSF_ENV*/

#ifdef AFS_SUNOS5_ENV
    vp->v_pages = NULL;
    evp->vd_delayLen = 0;
    evp->vd_vm.seqReadOff = 0;
    evp->vd_vm.readAheadOff = 0;
    evp->vd_vm.lastReadAheadOff = 0;
#endif	/* AFS_SUNOS5_ENV */

#ifdef	AFS_HPUX_ENV
    vp->v_fstype = VEFS;
#endif	/* AFS_HPUX_ENV */

    evp->vd_lastDblk = EFS_HOLE;
    evp->vd_dblkOff = 0;

    /* Mark the vnode as not associated with a filesystem object */
    evp->vd_acl = NULL;

    evp->vd_cred = NULL;
    evp->vd_access = 0;

    QInit (&evp->vd_flocks);

    tlock_Init(&evp->vd_tlock, "evnode");
    lock_InitDesc(&evp->vd_idLock, "evnode id");
    lock_InitDesc(&evp->vd_vm.lock, "evnode vm");
    lock_InitDesc(&evp->vd_file.lock, "evnode file");
    lock_InitDesc(&evp->vd_cache.lock, "evnode cache");
#if !defined(KERNEL) || defined(AFS_AIX_ENV)
    lock_Init(&evp->refCountMutex);
#endif
#if defined(AFS_AIX_ENV)
    lock_Init(&evp->vmmLenLock);
#endif
}

/* ReinitVnode -- initializes all the messy generic vnodeops level state. */

static int ReinitVnode(efs_vnode_t evp, int preexisting)
{
    struct vnode *vp = EVTOV(evp);
    enum vtype type;			/* vnode type from mode field */
    epi_anode_t ap = evp->vd_file.ap;	/* anode handle for evp */
    dev_t rdev = (dev_t) 0;		/* device for special vnodes */

    type = EV_MODE_TO_VTYPE (epif_GetMode (ap));
    if (preexisting)
	afsl_Assert (osi_vType(vp) == type);
    osi_vSetType(vp, type);

    evp->vd_lastDblk = EFS_HOLE;
    evp->vd_dblkOff = 0;
#ifdef AFS_SUNOS5_ENV
    evp->vd_delayLen = 0;
    /* XXX locks */
    evp->vd_vm.seqReadOff = 0;
    evp->vd_vm.readAheadOff = 0;
    evp->vd_vm.lastReadAheadOff = 0;
#endif	/* AFS_SUNOS5_ENV */

    /*
     * For special files, initialize device.  During efs_create, this function
     * gets called before the file is fully initialized, so we allow EPI_E_EOF.
     */
    if (ISSPECTYPE(type)) {
	long code;
	struct uio uio;
	struct iovec iov;

	epia_InitUio (&uio, &iov, 0, sizeof (rdev), (char *) &rdev);
	code = epia_Read (ap, 0, &uio);
	if (code && code != EPI_E_EOF) {

	    EFS_TRACE_VNOPS(DEBUG_THIS_FILE, code, EIO);

	    /* Clean up a little, caller will do more */

	    code = epif_Close(evp->vd_file.ap);
	    MBZ (code);
	    evp->vd_file.anode = 0;

	    return EIO;
	}
    }
    if (preexisting)
	afsl_Assert (vp->v_rdev == rdev);
    vp->v_rdev = rdev;

    /* Initialize other generic fields */

    if (!preexisting) {

	/* The virtual memory resources from vnode's previous identity must
         * have been cleaned up. */

#ifdef AFS_AIX_ENV
	Assert (evp->vd_seg == 0);
#endif
#ifdef AFS_SUNOS5_ENV
	Assert(vp->v_pages == NULL);
	Assert(vp->v_stream == NULL);	/* VN_INIT clears this */
#endif

	vp->v_flag = 0;
	osi_vSetVfsp(vp, VTOVFS(evp->vd_volp));
	osi_vInitLocks(vp);

	/* Insist that we're still pointing to Episode vnode ops (InitVnode
         * does this) and indicate that we don't need to be glued up */

	Assert (((struct vnode *)evp)->v_op ==
		(struct osi_vnodeops *)epii_GetEPIOps());
	SET_CONVERTED(vp);

	/* Clear evnode flags */
	evp->vd_flags = 0;

	/* Set ROOT bit(s) */
	if (epid_IsVolumeRoot (&evp->vd_fid)) {
	    evp->vd_flags |= VD_ROOT;
	    vp->v_flag |= VROOT;
	} else
	    evp->vd_flags &= ~VD_ROOT;
    }

    evp->vd_cache.lookupValid = 0;
    evp->vd_cache.newAtime = evp->vd_cache.newMtime =
	evp->vd_cache.newCtime = evp->vd_cache.newLen = 0;
    evp->vd_cache.lastAtime = epif_GetATime (ap);
    evp->vd_cache.lastMtime = epif_GetMTime (ap);
    evp->vd_cache.lastCtime = epif_GetCTime (ap);
    evp->vd_cache.dataVersion = epif_GetDV (ap);

    if (evp->vd_cred) {
	icl_Trace1(efs_iclSet2, 
		   EFS_TRACE_ACCESS_CACHE_REINITVNODE_INVALIDATE,
		   ICL_TYPE_POINTER, evp);
	crfree(evp->vd_cred);
	evp->vd_cred = NULL;
	evp->vd_access = 0;
    }
    return 0;
}

/* ObtainVnode -- returns a held vnode.  If flag is NEW, the fid is
 *     ignored and the returned vnode will have no identity.  If flag is
 *     EXISTING the specified fid is used to locate a matching vnode,
 *     which is returned if possible.  If no existing vnode is found a
 *     vnode without an identity is returned instead. */

#define OBTAIN_EXISTING 1
#define OBTAIN_NEW	2

efs_vnode_t ObtainVnode (
  int flag,
  afs_hyper_t volid,
  epi_volFileId_t *fid)
{
    int alloc;				/* shall/did we allocate a new vnode */
    int kill;				/* kill this vnode */
    int killed;				/* don't kill any more vnodes */
    efs_vnode_t new, old, found;	/* vnode of each type */

    efs_handle_pending_inactive();

    alloc = 0;
    killed = 0;				/* only kill one vnode per call */

retry:
    found = new = old = NULL;
    BEGIN_MUTEX (&vntableLock) {
	if (flag == OBTAIN_EXISTING) {
	    found = HashLookup (volid, fid);
	    if (found) {
		FAST_VN_HOLD(found);
		REMOVE_LRU(found);
		EXIT_CRIT;
	    }
	}

	if (vnCount < vnCountTarget) {
	    /* allocate a new one rather than recycling an existing one. */
	    vnCount++;
	    alloc = 1;
	    EXIT_CRIT;
	}

	/* Find an unused vnode to reclaim.  Vnodes without an identity are
         * best.  Otherwise pick the oldest one to recycle.  The avoidRecycle
         * bit is managed by StopUse to allow us to safely exclude unsuitable
         * vnodes without dropping the vntableLock to check definitively.  We
         * may occasionally lose a race here with StopUse but it will cause
         * only minor inconvenience. */
	
	{   efs_vnode_t e;
	    FIFO_FOR (e, &lruList, vd_lru) {
		efs_lockvp(e);
		if (vn_count(e) == 0) {
		    if (NoIdentity(e)) {
			EVTOV(e)->v_count++;
			efs_unlockvp(e);
			REMOVE_LRU(e);
			new = e;
			old = NULL;
			break;
		    } else if (!old && !e->vd_avoidRecycle)
			old = e;
		}
		efs_unlockvp(e);
	    }
	}
	if (old) {
	    FAST_VN_HOLD(old);
	    REMOVE_LRU(old);
	}
    } END_LOCK; /* drop vntableLock */

    if (found) {
	HERE (found,w_found);		/* existing vnode was found */
	return found;
    }
    if (old) {
	if (Recycle (old)) {
	    new = old;
	    old = NULL;
	} else {
	    /* Recycle failed or someone else grabbed the vnode */
	    VN_RELE (EVTOV(old));
	    goto retry;
	}
    }
    if (new) {
	
	/* If this vnode is stale anyway and we're above the target vnode
         * count, consider giving this one back.  Don't let the LRU list get
         * too small or we might end up thrashing.  And don't give back more
         * than one per invocation to avoid unfairly burdening this caller. */
	
	afsl_Assert (NoIdentity(new) && (vn_count(new) == 1));
	if ((vnCount < vnCountTarget) || killed) {
	    /* avoid cost of exact check in the common case. */
	    return new;
	}
	kill = 0;
	BEGIN_MUTEX (&vntableLock) {
	    if (!killed && (vnCount > vnCountTarget) &&
		(lruList.length > 5)) {
		afsl_Assert (!new->vd_onLRU && !new->vd_onHash &&
			     (vn_count(new) == 1));
		FIFO_REMOVE (&staleList, &new->vd_lru);
		vnCount--;
		kill = 1;
	    }
	} END_LOCK;
	if (kill) {
	    KillVnode(new);
	    osi_Free_r (new, sizeof(*new));
	    killed = 1;			/* only kill one vnode per call */
	    goto retry;
	}
	return new;
    }

    /* No good candidates to recycle or we're below target number of vnodes, so
     * just allocate a new one.  But first try to gently shake vnodes lose from
     * the dnlc. */

    if (!alloc) {
	struct osi_vnodeops *epiops =
	    (struct osi_vnodeops *)epii_GetEPIOps();
	int anyReclaimed = osi_dnlc_fs_purge_r(epiops, 1);
	if (anyReclaimed)
	    goto retry;

	BEGIN_MUTEX (&vntableLock) {
	    if (!vnCountMax || (vnCount < vnCountMax)) {
		vnCount++;
		alloc = 1;
	    }
	} END_LOCK;
    }
    if (!alloc)
	return NULL;			/* => ENFILE */

    new = osi_Alloc_r (sizeof(*new));
    InitVnode (new);
    HERE (new,w_init);			/* new vnode allocated */
    BEGIN_MUTEX (&vntableLock) {
	Assert (NoIdentity(new) && (vn_count(new) == 1) &&
		!new->vd_onHash && !new->vd_onLRU);
	FIFO_ADD (&staleList, &new->vd_lru);
    } END_LOCK;
    return new;
}

/* SetIdentity -- converts a NOIDENTITY vnode into one labeled with a
 *     particular fid.  It fails if another vnode for the fid's index
 *     already exists in the hash table.  It happily accepts fids with
 *     DONT_CARE unique since the uniquifier isn't part of the hash
 *     function.  The caller must determine the actual unique and fill
 *     it in by calling SetUnique (below) before releasing the idLock.
 *
 * LOCKS USED -- Assumes that the caller holds all the per-vnode locks.
 *     This excludes any potential user from seeing a vnode with a bogus
 *     fid or which has not been uninitialized.
 *
 * RETURN CODES -- returns EEXIST if any vnode with same index is
 *     already present. */

static int SetIdentity (
  efs_vnode_t evp,
  struct volume *volp,
  epi_volFileId_t *fid)
{
    afs_hyper_t volid;
    int code;

    Assert (lock_Check (&evp->vd_idLock) &&
	    lock_Check (&evp->vd_vm.lock) &&
	    lock_Check (&evp->vd_file.lock) == -1 &&
	    lock_Check (&evp->vd_cache.lock) == -1 &&
	    volp->v_count > 0 && !evp->vd_vm.seg);

    VOLID_SET(volid, volp->v_volId);
    BEGIN_MUTEX (&vntableLock) {
	Assert (NoIdentity(evp));

	if (HashLookup (volid, fid)) {
	    code = EEXIST;
	    EXIT_CRIT;
	}
	evp->vd_fid = *fid;
	evp->vd_volid = volid;

	code = dfsh_HashIn_r (&vntable, evp);
	MBZ (code);
	evp->vd_onHash = 1;
	if (!evp->vd_onLRU)
	    FIFO_REMOVE (&staleList, &evp->vd_lru);

	/* Notify iterators that new vnode has been added.  See
	 * vnm_StopUse for details on the interation procedure. */
	evp->vd_label = vntableLabel;

	/* Keep pointer to other per volid state. */
	evp->vd_volp = volp;
	code = 0;
    } END_LOCK;
    return code;
}

/* SetUnique -- fills in the unique portion of a vnode's fid. */

static void SetUnique (
  efs_vnode_t evp,
  u_long unique)
{
    BEGIN_MUTEX (&vntableLock) {
	Assert (epid_IsDCUnique(&evp->vd_fid));
	epid_SetUnique(&evp->vd_fid, unique);
    } END_LOCK;
}

/* SetRestrictions -- called by FindVnode to initialize a new vnode or
 *     by StopUse to calculate the vnode state restrictions from the new
 *     volume open bits.  It also checks the volume header readonly bit
 *     and whether the corresponding anode has a non-zero copies count
 *     (implying an incomplete clone operation).
 *
 *     Initializes the restriction bits vd_{file,vm}.no*, plus
 *     vd_{file,vm}.readonly, and vd_file.anodeRO.
 *
 *     Should never be called on NOIDENTITY vnodes.
 *
 * LOCKS USED -- we assume the caller holds the per-vnode locks. */

static void SetRestrictions (
  efs_vnode_t evp)
{
    int openbits;

    Assert (lock_Check (&evp->vd_idLock) &&
	    lock_Check (&evp->vd_vm.lock) &&
	    lock_Check (&evp->vd_file.lock) == -1 &&
	    lock_Check (&evp->vd_cache.lock) == -1 &&
	    !NoIdentity(evp) && evp->vd_volp);

    openbits = VTOEVLD(evp->vd_volp)->vld_openbits;

    /* First the file bits */
    evp->vd_file.noAnode = ((openbits & STOPUSE_NO_ANODE) != 0);

    Assert (!evp->vd_file.noAnode == evp->vd_file.anode);

    /* The management of R/O mounts is confused.  Since we can not presently
     * exclude the PX during a mount operation we can't avoid races when making
     * vnodes readonly.  Thus we only check the R/O bit when deciding when to
     * allow a modification operation but we don't insist that no dirty data be
     * cached in the vnode. */

    /* Potential problem --
     *     A volume that is read-write can be exported read-write while being
     *     mounted read-only.  Should local users see writes made by remote
     *     users or should remote users be denied write access because of a
     *     local mount?  In the latter case shouldn't a readonly mount
     *     operation revoke all write tokens? */

    if (evp->vd_file.anode) {
	evp->vd_file.readonly = VTOEVLD(evp->vd_volp)->vld_roVolume;
	evp->vd_file.anodeRO = (epia_GetCopies(evp->vd_file.ap) > 0);
    } else {
	evp->vd_file.anodeRO = 1;
	evp->vd_file.readonly = 1;
    }

    evp->vd_cache.noStatus = ((openbits & STOPUSE_NO_STATUS) != 0);
    Assert (evp->vd_cache.noStatus == evp->vd_file.noAnode);

    evp->vd_cache.noDirty =
	((openbits & STOPUSE_NO_DIRTY) || (openbits & STOPUSE_NO_ANODE) ||
	 evp->vd_file.anodeRO || (!openbits ? evp->vd_file.readonly : 0));
    Assert (!evp->vd_cache.noDirty || !EV_ISDIRTY(evp));

    evp->vd_file.noDelete = evp->vd_cache.noChange =
	(openbits ? 1 : (evp->vd_file.anodeRO | evp->vd_file.readonly));

    /* Now the VM bits */

    evp->vd_vm.noReadable =
	((openbits & (STOPUSE_NO_ANODE|STOPUSE_NO_STATUS|STOPUSE_NO_VM)) != 0);
    evp->vd_vm.noWritable = evp->vd_vm.noReadable |
	((openbits & (STOPUSE_NO_DIRTY|STOPUSE_NO_DIRTYVM)) != 0);
    evp->vd_vm.readonly = evp->vd_file.readonly | evp->vd_file.anodeRO;

    /* Note that we can set noReadable via openbits.noAnode without
     * clearing VM. */
    Assert ((!(openbits & (STOPUSE_NO_VM|STOPUSE_NO_STATUS)) ||
	     !evp->vd_vm.valid) &&
	    (!evp->vd_vm.noWritable || !evp->vd_vm.dirty));
}

/* OpenVnode -- attaches the vnode to a particular fid.  The specified
 *     vnode should be a pretty good candidate, so we lock it down and
 *     try to make it all legal.
 *
 * PARAMETERS -- If we called from vnm_SetIdentity with a new file its anode
 *     handle is also provided.  In this case we use this instead of calling
 *     epif_Open, and also expect the provided evnode to be NoIdentity.
 *
 * LOCKS USED -- Uses the idLock to ensure that the fid corresponds to a
 *     existing file.
 *
 *     For a new vnode, all three vnode locks are held throughout its
 *     initialization.  This prevents any exposure of partially created vnodes.
 *
 * RETURN CODES -- returns EEXIST if a different vnode already exists
 *     for the specified fid.
 *
 *     Returns ESTALE if the fid does not match any file. 
 * 
 * TRACING -- callers rely on tracing in this function.
 */

static int OpenVnode (
  efs_vnode_t evp,
  struct volume *volp,
  epi_volFileId_t *fid,
  epi_anode_t file)
{
    afs_hyper_t volid;
    u_long unique;
    int code = 0;

    VOLID_SET (volid, volp->v_volId);

    BEGIN_LOCK_WRITE (&evp->vd_idLock) {
	Assert (vn_count(evp) > 0);

	/* Once we obtain idLock we know that the vnode is NoIdentity or
	 * it contains a full fid and a matching (open) anode handle. */

	if (epid_EqualIndexes(&evp->vd_fid, fid) &&
	    VOLID_EQ(evp->vd_volid, volid)) {

	    /* The vnode's fid must always match the anode if the anode is can
	     * be opened and the anode will be open if it can be opened
	     * (StopUse will do this) and OpenVnode can only be called when it
	     * is legal to open the anode.  Therefore, if the requested fid
	     * doesn't match it is because the requested unique is wrong and
	     * the fid is bogus.  This path should never be used by
	     * vnm_SetIdentity (i.e. file != NULL). */

	    Assert ((file == NULL) && evp->vd_file.anode &&
		    (epid_ToUnique(&evp->vd_fid) ==
		     epif_GetUnique(evp->vd_file.ap)));

	    if (!epid_IsDCUnique(fid) && !epid_Equal(&evp->vd_fid, fid)) {
		HERE (evp,w_ostale);
		code = ESTALE;
	    } else {
		HERE (evp,w_ofound);
	    }
	    EXIT_CRIT;
	}
	Assert (NoIdentity(evp) && (vn_count(evp) > 0));

	BEGIN_LOCK_WRITE (&evp->vd_vm.lock) ;
	BEGIN_LOCK_WRITE (&evp->vd_file.lock) ;
	BEGIN_LOCK_WRITE (&evp->vd_cache.lock) {

		/* Try to set this to the specified fid.  If this fails
		 * then someone else has already created a vnode with
		 * the same index so tell our caller to try again. */

		code = SetIdentity (evp, volp, fid);
		if (code) {
		    HERE (evp,w_nosetid);
		    Assert (code == EEXIST);
		    EXIT_CRIT;
		}

		if (file) {

		    /* If we are being called to create a vnode for a new file
		     * the matching anode handle will be supplied.  We use
		     * this, then clear the creator exclusive bit. */

		    evp->vd_file.ap = file;
		    epif_AllowOpens (file);

		} else {

		    /* Now with the identity set we make sure it is a valid fid
		     * by opening the anode.  We always open the anode while
		     * holding idLock.  At this point we've already installed
		     * this vnode in the hash table with the correct index.
		     * Therefore the anode at this index cannot be opened or
		     * deleted under the guise of any other vnode.  This
		     * prevents any possible trouble with the anode reference
		     * count being too high, even temporarily while checking a
		     * bogus fid. */

		    code = epif_Open (VTOVH(volp), &evp->vd_fid,
				      &evp->vd_file.ap);
		    if (code) {
			/* fid is no good */

			HERE (evp,w_ofail);
			ClearIdentity (evp);
			EFS_TRACE_VNOPS(DEBUG_THIS_FILE, code, ESTALE);
			code = ESTALE;
			EXIT_CRIT;
		    }
		}
		evp->vd_file.anode = 1;

		unique = epif_GetUnique(evp->vd_file.ap);
		if (epid_IsDCUnique(fid)) {
		    SetUnique (evp, unique);
		} else {
		    Assert (unique == epid_ToUnique(fid));
		}

		/* Finish initializing the vnode's state */

		code = ReinitVnode (evp, 0/*!preexisting*/);
		if (code) {
		    /* Something is seriously wrong, flush vnode. */
		    ClearIdentity (evp);
		    EXIT_CRIT;
		}

		/* Initialize the restriction bits from volume open state and
		 * other sources. */

		SetRestrictions (evp);
		HERE (evp,w_onew);
	    } END_LOCK; /* drop vm, file & cache locks */
	END_LOCK;
	END_LOCK;
    } END_LOCK; /* drop idLock */
    icl_Trace4(efs_iclSet, EFS_TRACE_OPENVNODE,
	       ICL_TYPE_LONG, AFS_hgetlo(evp->vd_volid),
	       ICL_TYPE_LONG, epid_IndexPart(&evp->vd_fid),
	       ICL_TYPE_POINTER, evp,
	       ICL_TYPE_LONG, code);
    return code;
}

/* vnm_FindVnode -- returns a held vnode for the specified fid.  After
 *     locating the vnode for the fid or allocating a new one, it opens
 *     the corresponding anode to make sure the specified fid is valid.
 *     The per-volume state of new vnodes is initialized.
 *
 * CAUTIONS -- caller must not have started a transaction.
 *
 * RETURN CODES -- returns ESTALE if the fid did not match any file. */

int vnm_FindVnode (
  struct volume *volp,
  epi_volFileId_t *fid,
  efs_vnode_t *evpP)
{
    int code = 0;
    efs_vnode_t evp;
    afs_hyper_t volid;

    VOLID_SET (volid, volp->v_volId);
    do {
	evp = ObtainVnode (OBTAIN_EXISTING, volid, fid);
	if (!evp) {
	    code = ENFILE;
	    break;
	}
	code = OpenVnode (evp, volp, fid, NULL);
	if (code) {
	    VN_RELE (EVTOV(evp));
	    evp = NULL;
	    if ((code == ESTALE) || (code == EIO)) {
		break;
	    }
	    Assert (code == EEXIST);
	}
    } while (!evp);
    *evpP = evp;
    Assert (code || (vn_count(evp) > 0));
    return code;
}

/* vnm_Allocate -- returns a held vnode without an identity for use with
 *     a newly created file.  This should be called before starting the
 *     transaction used to create the file and vnm_SetIdentity should be
 *     called afterwards to fill in the fid of the new file.
 *
 * CAUTIONS -- caller must not have started a transaction. */

efs_vnode_t vnm_Allocate (void)
{
    efs_vnode_t evp;
    afs_hyper_t dummy;

    evp = ObtainVnode(OBTAIN_NEW, dummy, 0/*volid,fidP*/);
    Assert (!evp || ((vn_count(evp) == 1) && NoIdentity(evp)));
    return evp;
}

/* vnm_SetIdentity -- fills in the identity of a vnode for a newly created file
 *     by calling OpenVnode.  We should never get ESTALE back (because we just
 *     created the object and presumably have the containing directory locked).
 *     However, we may get EEXIST if the fid is found in hash table.  This
 *     could happen if a racing VGET created a vnode for the new file before we
 *     finished this call.  In that case we grab the idLock of the other vnode
 *     which waits until the vget thread finishes noticing his ESTALE error and
 *     clears the identity of the vnode (thus removing it from the hash table).
 *     Then we try again.
 *
 * LOCKS USED -- Since our caller may have started a transaction, calling
 *     SetIdentity appears to violate the resource hierarchy.  We are saved
 *     from deadlock because the lock will be in a NoIdentity vnode and no one
 *     could be holding *that* lock while starting a transation.  Or, if the
 *     vnode being locked was found in the hash table, then it is being
 *     released by a failing VGET or Inactive.  In both cases no transactions
 *     are started or awaited (e.g. by tlock_Recycle).
 *
 * CAUTIONS -- Our caller would be pissed to find us calling vn_rele, which can
 *     be heavyweight, but we know the vnode has no identity so it is actually
 *     harmless. */

int vnm_SetIdentity (
  efs_vnode_t evp,
  struct volume *volp,
  epi_volFileId_t *fid,
  epi_anode_t file)
{
    int code;
    afs_hyper_t volid;

    Assert ((vn_count(evp) == 1) && NoIdentity(evp));
    Assert (!epid_IsDCUnique(fid));	/* must pass a fully specified fid */
    VOLID_SET (volid, volp->v_volId);

    while ((code = OpenVnode (evp, volp, fid, file)) == EEXIST) {
	efs_vnode_t other;
	BEGIN_MUTEX (&vntableLock) {
	    other = HashLookup (volid, fid);
	    if (other) {
		HERE (other,w_silookup);
		FAST_VN_HOLD(other);
	    }
	} END_LOCK;
	if (!other)
	    continue;

	/* We have to flush this guy out of the hash table.  Once we obtain the
	 * idLock the interloper will have noticed that the file doesn't really
	 * exist yet and departed the scene. */

	BEGIN_LOCK_WRITE (&other->vd_idLock) {
	    Assert (NoIdentity(other));
	} END_LOCK;
	VN_RELE (EVTOV(other));
    }
    Assert (code != ESTALE);
    return code;
}

/* vnm_Delete -- this deletes the file corresponding to a particular volp/fid
 *     making the vnode stale if it is in use.  Unlike inactive it takes no
 *     consideration of the vnode's refCount or the file's linkCount.
 *
 *     Use by vol_efsDelete (during restore). */

int vnm_Delete (
  struct volume *volp,
  epi_volFileId_t *fid)
{
    int code;
    efs_vnode_t evp;

    Assert (!epid_IsDCUnique(fid));

    code = vnm_FindVnode (volp, fid, &evp);
    if (code)
	return code;
    Assert (evp->vd_file.anode);

    BEGIN_LOCK_WRITE (&evp->vd_idLock) {
	if (NoIdentity(evp))
	    EXIT_CRIT;
	BEGIN_LOCK_WRITE (&evp->vd_vm.lock) {
	    Assert (!evp->vd_vm.valid);
	    ASSERT_VMBITS_CONSISTENT(evp);
	    BEGIN_LOCK_WRITE (&evp->vd_file.lock) {
		epi_anode_t ap = evp->vd_file.ap;
		u_int lc;		/* link count */

		/* Clean out the vnode tlock */
		tlock_Recycle (&evp->vd_tlock);

		FlushCachedStatus (evp, CACHE_KILL);

		code = epif_Delete(ap, EPIF_DELETE_IGNORE_LINKCNT, NULL);
		MBZ(code);
		evp->vd_file.anode = 0;

		BEGIN_LOCK_WRITE (&evp->vd_cache.lock) {
		    afsl_Assert (!EV_ISDIRTY(evp));
		    ClearIdentity (evp);
		    HERE (evp,w_delete);
		} END_LOCK;
	    } END_LOCK; /* drop fileLock */
	} END_LOCK; /* drop vmLock */
    } END_LOCK; /* drop idLock */
    VN_RELE (EVTOV(evp));
    return 0;
}

/* AddToLRU -- put the vnode on the LRU list at a good place.  If the
 *     reference count is high, actually take it off the LRU list, as
 *     inactive will get another shot at it.  If the vnode has no
 *     identity move it to the front, otherwise add it to the back. */

static void AddToLRU (
  efs_vnode_t evp)
{
    BEGIN_MUTEX (&vntableLock) {
	int front = NoIdentity(evp);
	int off = (vn_count(evp) > 1);

	if (off) {
	    /* generally, held vnodes aren't on the LRU list */
	    REMOVE_LRU (evp);
	} else if (front && evp->vd_onLRU) {
	    /* just shift vnode to the front */
	    FIFO_REMOVE (&lruList, &evp->vd_lru);
	    FIFO_ADD_OLDEST (&lruList, &evp->vd_lru);
	} else if (!evp->vd_onLRU) {
	    /* Add vnode to LRU list if no one hold it. */
	    if (!evp->vd_onHash)
		FIFO_REMOVE (&staleList, &evp->vd_lru);
	    if (front)
		FIFO_ADD_OLDEST (&lruList, &evp->vd_lru);
	    else
		FIFO_ADD (&lruList, &evp->vd_lru);
	    evp->vd_onLRU = 1;
	}
    } END_LOCK;
}

/* vnm_Inactive -- does cleanup processing on a vnode when the caller is
 *     releasing it and the reference count is one. */

int vnm_Inactive (
  struct vnode *vp)
{
    efs_vnode_t evp = VTOEV(vp);
    int code;

    lock_ObtainWrite_r (&evp->vd_idLock);
    efs_lockvp(evp);
    if (vp->v_count > 1) {
	VN_DECR(vp);
	efs_unlockvp(evp);
	lock_ReleaseWrite_r (&evp->vd_idLock);
	return 0;
    }
    efs_unlockvp(evp);

    Assert (vn_count(evp) > 0);
    if (NoIdentity(evp)) {
	HERE (evp,w_istale);
	if (evp->vd_vm.seg) {

	    /* Even a stale vnode may still have a VM segment, at least on AIX,
	     * so delete it here.  But it shouldn't have any data. */

	    afsl_Assert (!evp->vd_avoidRecycle);
	    code = vnvm_delete(vp, osi_credp);
	    afsl_MBZ(code);

	    BEGIN_LOCK_WRITE (&evp->vd_vm.lock) {
		Assert (!evp->vd_vm.valid && !evp->vd_vm.dirty);
		ASSERT_VMBITS_CONSISTENT(evp);
		evp->vd_vm.seg = 0;
	    } END_LOCK;
	}
    } else {
	int delete;

	BEGIN_LOCK_READ (&evp->vd_file.lock) {
	    if (!evp->vd_file.noDelete) {
		Assert (evp->vd_file.anode);
		delete = (epif_GetLinkCount(evp->vd_file.ap) == 0);
	    } else {
		/* Never delete files on open volumes */
		delete = 0;
	    }
	} END_LOCK;

	if (delete) {
	    HERE (evp,w_idelete);

	    /* Discard all pages and try to avoid writing dirty pages. */

	    afsl_Assert (!evp->vd_avoidRecycle);
	    code = vnvm_delete(vp, osi_credp);
	    afsl_MBZ(code);

	    BEGIN_LOCK_WRITE (&evp->vd_vm.lock) {

		evp->vd_vm.valid = 0;
		evp->vd_vm.dirty = 0;
		evp->vd_vm.seg = 0;

		ASSERT_VMBITS_CONSISTENT(evp);

		BEGIN_LOCK_WRITE (&evp->vd_file.lock) {
		    long code;
		    u_int lc;		/* link count */

		    /* Clean out the vnode tlock */
		    tlock_Recycle (&evp->vd_tlock);

		    FlushCachedStatus (evp, CACHE_KILL);

		    code = epif_Delete(evp->vd_file.ap, 0 /* flags */, NULL);
		    afsl_MBZ(code);
		    evp->vd_file.anode = 0;

		    BEGIN_LOCK_WRITE (&evp->vd_cache.lock) {
			afsl_Assert (!EV_ISDIRTY(evp));
			ClearIdentity (evp);
		    } END_LOCK;
		} END_LOCK; /* drop fileLock */
	    } END_LOCK; /* drop fileLock */
	} else {
#ifdef AFS_SUNOS5_ENV
	    if (evp->vd_vm.dirty) {

		/* We don't want to clean vnodes being deleted so do this
		 * here.  This asynchronously writes all dirty pages.
		 * Generally this will cause the reference count to increase
		 * and so we will come here again. */

		/* We don't need to do this under the vmLock since this is
		 * advisory.  Is the rare dirty page for an inactive vnode
		 * okay? */

		code = vnvm_schedule(vp, osi_credp, 0/*from inactive*/);
		afsl_MBZ(code);
	    }
#endif
	}
    }

    AddToLRU(evp);

    BEGIN_LOCK (efs_lockvp(evp),
		efs_unlockvp(evp)) {
	VN_DECR(vp);
	lock_ReleaseWrite_r (&evp->vd_idLock);
    } END_LOCK;
    return 0;
}

#ifdef AFS_SUNOS5_ENV
/*
 * vnm_purge_vol_check
 *
 * Remove all entries for this volume from the dnlc, as part of
 * shutting down the volume.  Called from vnm_StopUse.
 */
static int
purge_dnlc_check(struct ncache *ncp, int vol_arg)
{
    int do_purge;
    struct osi_vnodeops *epiops;

    (void)osi_PreemptionOff();
    epiops = (struct osi_vnodeops *)epii_GetEPIOps();
    if (ncp->vp->v_op != epiops)		/* skip if not Episode */
	do_purge = 0;
    else
	do_purge = (VTOEV(ncp->vp)->vd_volp == (struct volume *)vol_arg);
    osi_RestorePreemption(0);

    return do_purge;
}
#else
#define purge_dnlc_check 0
#endif /* AFS_SUNOS5_ENV */

void vnm_purge_vol_dnlc(struct volume *volp)
{
    while (osi_dnlc_iter_purge_r(purge_dnlc_check, (int)volp))
	;
}

/* StopUse -- does the work of making a vnode compatible with the new
 *     open state of the volume. */

static void StopUse (
  efs_vnode_t evp,
  int openbits)
{
    int code = 0;

    if (openbits & STOPUSE_NO_VM) {

	/* As an optimization try to flush vnodes if we're going to be
	 * invalidating VM.  Otherwise, or if Recycle fails, phantomize
	 * it instead. */

	Recycle (evp);
    }
    BEGIN_LOCK_WRITE (&evp->vd_idLock) {

	/* Check if the vnode is stale.  If it is we are done so return.
	 * Otherwise it will stay valid because we hold the idLock. */

	if (NoIdentity(evp)) {
	    HERE (evp,w_sustale);
	    EXIT_CRIT;
	}
	HERE (evp,w_stopuse);

	/* Phantomize */

	BEGIN_LOCK_WRITE (&evp->vd_vm.lock) {

	    /* Put the vnode's VM into the desired state under cover of
	     * the vmLock so it stays that way. */

	    ASSERT_VMBITS_CONSISTENT(evp);
	    if (((openbits & STOPUSE_NO_STATUS) ||
		 (openbits & STOPUSE_NO_VM)) && evp->vd_vm.valid) {
		/* Synchronously write through and kill all resident
		 * pages. */
		code = vnvm_StopUse(EVTOV(evp), osi_credp);
		afsl_MBZ(code);
		evp->vd_vm.valid = 0;
		evp->vd_vm.dirty = 0;
	    } else if ((openbits & STOPUSE_NO_DIRTYVM) && evp->vd_vm.dirty) {
		/* Synchronously write through the dirty pages and
		 * protect (or eliminate) all writable pages. */
		code = vnvm_Clean(EVTOV(evp), osi_credp);
		afsl_MBZ(code);
		evp->vd_vm.dirty = 0;
	    }
	    ASSERT_VMBITS_CONSISTENT(evp);

	    /* We must hold the cacheLock while setting restrictions and
	     * possibly clearing the identity.  However, we cannot hold it
	     * while calling UpdateAnode so we loop until we get the cacheLock
	     * and the status is clean.  Then we can proceed to invalidate the
	     * status and to the rest of StopUse processing. */

	    BEGIN_LOCK_WRITE (&evp->vd_file.lock) ;
	    BEGIN_LOCK_WRITE (&evp->vd_cache.lock) {
		while (EV_ISDIRTY(evp)) {
		    lock_ReleaseWrite_r(&evp->vd_cache.lock);
		    vnm_UpdateAnode(evp, elbb_zeroTrans);
		    lock_ObtainWrite_r(&evp->vd_cache.lock);
		}
		if (openbits & STOPUSE_NO_STATUS)
		    KillCachedStatus (evp);

		if (openbits & STOPUSE_NO_ANODE) {
		    if (evp->vd_file.anode) {
			code = epif_Close (evp->vd_file.ap);
			MBZ (code);
			evp->vd_file.anode = 0;
		    }
		} else if (!evp->vd_file.anode) {
		    /* Make sure the fid is still valid by opening the
		     * anode.  If we are opening a volume without an anode
		     * layer volume handle, it means the volume is being
		     * forcibly detached.  We cannot open the anode so make
		     * the vnode stale instead.  If we try but fail to open
		     * the anode, also make the vnode stale.  For stale
		     * vnodes we just wait for the holder to release it. */

		    if (VTOEVLD(evp->vd_volp) && VTOVH(evp->vd_volp)) {
			code = epif_Open (VTOVH(evp->vd_volp),
					  &evp->vd_fid, &evp->vd_file.ap);
		    } else
			code = ESTALE;

		    if (code == 0) {
			evp->vd_file.anode = 1;
			code = ReinitVnode (evp, 1/*preexisting*/);
			/* non-zero code means couldn't read rdev from anode */
		    }
		    if (code) {
			/* Clean out the vnode tlock
			 *
			 * Its safe to recycle the tlock i.e. wait for
			 * the tlock trans to complete here as no
			 * new transactions should have been started up
			 * due to creates. The creates should be blocked
			 * due to the glue */
			tlock_Recycle (&evp->vd_tlock);

			/* detaching or fid is no good */
			ClearIdentity (evp);
		    }
		}

		if (!code)
		    SetRestrictions (evp);

		BEGIN_MUTEX (&vntableLock) {
		    evp->vd_avoidRecycle =
			(evp->vd_vm.seg && evp->vd_vm.noWritable);
		} END_LOCK;

	    } END_LOCK; /* drop cacheLock & fileLock */
	    END_LOCK;
	} END_LOCK; /* drop vmLock */
    } END_LOCK; /* drop idLock */
}

/* vnm_StopUse -- iterates over all vnodes (in a fileset) and cleans or
 *     invalidates any VM or other cached state they have that is
 *     inconsistent with the specified open mode.  This is called after
 *     vol_open and before each major phase of a volume operation.  It
 *     is also called from vol_close to return all vnodes to normal
 *     operation.
 *
 *     We assume that a higher level lock protects the fileset so that
 *     only one thread calls this routine at a time for each fileset.
 *     It makes a single pass over the vnodes assuming that there is no
 *     ongoing activity that would reestablish any inconsistent vnode
 *     state.  Before this routine starts all incompatible vnode
 *     operations are already blocked.  Setting the restriction bits on
 *     each vnode will cause VM operations to take appropriate action.
 *	 - PageIn blocks incompatible requests.
 *	 - PageOut is unaffected.
 *     Clearing these bits returns these operations to their normal
 *     behavior.
 *
 *     The basic algorithm is to use the vnode hash table to locate all
 *     vnodes of interest.  This is protected by the vntableLock which
 *     we can not hold while flushing vnode state.  So, under the
 *     vntableLock, we identify and hold the *next* vnode to work on.
 *     Then we drop the vntableLock and flush the *current* vnode.  The
 *     next vnode can become stale while the current vnode is being
 *     process.  In this rare case the iteration is restarted.
 *
 *     The current vnode must be phantomized, which means made
 *     consistent with the specified open bits.  Every phantomized
 *     vnode is marked with the label associated with the hash table
 *     when we started.  This allows us to inexpensively skip it if we
 *     must restart the iteration.  Any vnode created while StopUse is
 *     running will be labeled with the current label (or a subsequent
 *     one) by SetIdentity and its openbits are initialized to the same
 *     value by SetRestrictions called from vnm_FindVnode.  StopUse can
 *     safely skip these also.  Therefore all phantomized vnodes will
 *     remain consistent with the current volume open mode because their
 *     restriction bits have been set correctly and since only
 *     compatible vnode operations will be running. */

void vnm_StopUse(
  struct volume *volp,
  int openbits)
{
    afs_hyper_t volid;
    int oldOpen;			/* open bits before this StopUse */
    long myLabel;			/* label at start of iteration */
    int kick;				/* wakeup unblocked PageIns */

    oldOpen = (VTOEVLD(volp) ? VTOEVLD(volp)->vld_openbits : STOPUSE_NO_ANODE);
    if (openbits == oldOpen)
	return;				/* already StopUse'd */

    VOLID_SET (volid, volp->v_volId);

    if (openbits & STOPUSE_NO_VM) {

	/* This is a rather heavyweight volume operation so make sure we
	 * shake loose all possible vnodes. */

	vnm_purge_vol_dnlc(volp);
    }
    efs_wait_for_pending_inactive();

    if (!(openbits & STOPUSE_NO_ANODE)) {
	vnvl_GetVolumeReadonly (volp);
    }

    BEGIN_MUTEX (&vntableLock) {
	long state = 0;			/* hash table iteration state */
	efs_vnode_t evp, next;		/* current and next vnodes */

	/* Figure out whether we'll need to wakeup any blocked PageIns.
	 * But defer the notify until after we've revived all the
	 * vnodes. */

	kick = (((oldOpen & STOPUSE_NO_VM) && !(openbits & STOPUSE_NO_VM)) ||
		((oldOpen & STOPUSE_NO_DIRTYVM) &&
		 !(openbits & STOPUSE_NO_DIRTYVM)));

	/* Remember openbits for this volume so subsequently created
	 * vnodes will have the same restrictions we are imposing here.
	 * Bump the label at the same time. */

	if (VTOEVLD(volp))
	    VTOEVLD(volp)->vld_openbits = openbits;
	myLabel = ++vntableLabel;

	evp = NULL;
	do { /* for all vnodes */
	    next = HashNext(evp, &state);

	    /* Skip vnodes without identities, those not in the desired
	     * volume, newly created vnodes and those we've already
	     * done. */

	    while (next &&
		   (!VOLID_EQ(next->vd_volid, volid) ||
		    (((signed)(next->vd_label - myLabel)) >= 0)))
		next = HashNext(next, &state);

	    /* Now hold it so it'll be here when we finish with evp. */
	    if (next)
		FAST_VN_HOLD(next);

	    if (evp) {
		epi_mutex_unlock (&vntableLock);
		StopUse (evp, openbits);
		BEGIN_MUTEX (&vntableLock) {
		    evp->vd_label = myLabel;
		} END_LOCK;
		VN_RELE (EVTOV(evp));
		evp = NULL;
		epi_mutex_lock (&vntableLock);
	    }

	    evp = next;
	} while (evp);
    } END_LOCK;
    if (kick)
	efs_WakeupBlockedPageIns (volp);
}

/*
 * Do some checks to avoid vnodes that are possibly in an unstable
 * state when we sync during panic.
 */
static int
AnodeInTransit(struct evnode *evp)
{
    return (NoIdentity(evp) || !evp->vd_file.anode ||
	    epia_WriteLocked(EVTOA(evp)));
}

/*
 * vnm_SyncVnodes -- update information cached in vnodes.
 *
 *     Called from sync.  Must be called before flushing the buffer system.
 *
 * PARAMETERS -- If flags has its least significant bit set, only sync
 *	cached anode information and not user data.
 */
void vnm_SyncVnodes(int flags)
{
    long state = 0;			/* hash table iteration state */
    efs_vnode_t evp, next;		/* current and next vnodes */

    BEGIN_MUTEX (&vntableLock) {
	evp = NULL;
	do { /* for all vnodes */
	    next = HashNext(evp, &state);

            /* Make sure the next vnode is one worth syncing.  This is
             * advisory, so its ok to check noWritable w/o the the proper
             * locking.  This check is an attempt to avoid un-needed queuing of
             * held vnodes on the volume's deferred inactive list. */

            while (next && next->vd_vm.noWritable) {
                next = HashNext(next, &state);
            }

	    /* Now hold it so it'll be here when we finish with evp. */
	    if (next != NULL)
		FAST_VN_HOLD(next);

	    if (evp != NULL) {
		struct vnode *vp = EVTOV(evp);
		int stale = 0;
		epi_mutex_unlock(&vntableLock);

		/* On SunOS the vnvm_ functions can defend themselves from
		 * stale vnodes since the v_pages field is effectively owned by
		 * the VM system.  On AIX, we must ensure that the VM segment
		 * id, which is what we pass to the VM system, is valid.  Only
		 * vnm_Inactive and Recycle delete the VM segment.  They do
		 * their work under the idLock and don't change the state of a
		 * held vnode.  We can be sure that the if the vnode is not
		 * stale here its VM segment will stay valid. [DB 5815]
		 */
		lock_ObtainWrite_r(&evp->vd_idLock);
		stale = NoIdentity(evp);
		lock_ReleaseWrite_r(&evp->vd_idLock);

		/*
		 * Skip stale vnodes.  Also don't sync this vnode if we are
		 * panicking and the anode is currently write-locked.
		 */
		if (!stale && (!osi_IsKernelPanic() || !AnodeInTransit(evp))) {
		    /*
		     * Schedule dirty pages unless flags say no or
		     * this vnode is a swap file.
		     */
		    if (!(flags & 1) && !OSI_IS_SWAPVP(vp)) {
			int code = vnvm_schedule(vp, osi_credp, 1);
			if (code !=  0) {
			    /* discard error */
			    EFS_TRACE_VNOPS(DEBUG_THIS_FILE, code, 0);
			}
		    }

		    BEGIN_LOCK_WRITE(&evp->vd_file.lock) {
			vnm_UpdateAnode(evp, elbb_zeroTrans);
		    } END_LOCK;

		}
		VN_RELE(vp);
		epi_mutex_lock(&vntableLock);
	    }

	    evp = next;
	} while (evp != NULL);
    } END_LOCK;
}

/*
 * vnm_QuiesceVol
 *
 *   Attempt to quiesce the volume i.e. clean dirty pages and status for a
 *   fileset synchronously.
 *   Also if \arg{recycle} specified attempt to recyle the vnodes
 *   in the volume.
 */
void vnm_QuiesceVol(
  struct volume *volp,
  int recycle)
{
    efs_evnode_t *evp, *nextvp;
    afs_hyper_t volid;
    long code = 0;
    long state = 0;
    int stale;

    VOLID_SET (volid, volp->v_volId);

    BEGIN_MUTEX (&vntableLock) {
	evp = HashNext(NULL, &state);
	while (evp && !VOLID_EQ(evp->vd_volid, volid))
	    evp = HashNext(evp, &state);
	if (evp)
	    FAST_VN_HOLD(evp);

	while (evp) {
	    nextvp = HashNext(evp, &state);
	    while (nextvp && !VOLID_EQ(nextvp->vd_volid, volid))
		nextvp = HashNext(nextvp, &state);
	    if (nextvp)
		FAST_VN_HOLD(nextvp);

	    epi_mutex_unlock(&vntableLock);

	    /* Attempt to recycle vnode if so specified */
	    if (recycle)
		Recycle(evp);

	    BEGIN_LOCK_WRITE (&evp->vd_idLock) {
		/* Don't pass stale vnodes to vnvm_ functions, see comment in
		 * vnm_SyncVnodes for details. */
		stale = NoIdentity(evp);
	    } END_LOCK;

	    if (!stale) {
		BEGIN_LOCK_WRITE (&evp->vd_vm.lock) {
		    code = vnvm_Clean(EVTOV(evp), osi_credp);
		    afsl_MBZ(code);
		    evp->vd_vm.dirty = 0;
		    BEGIN_LOCK_WRITE (&evp->vd_file.lock) {
			vnm_UpdateAnode (evp, elbb_zeroTrans);
		    } END_LOCK;
		} END_LOCK;
	    }
	    VN_RELE(EVTOV(evp));
	    epi_mutex_lock(&vntableLock);

	    evp = nextvp;
	}
    } END_LOCK;
}

/* vnm_Inuse -- this just returns true if there are any held vnodes in the
 *     specified volume.
 *
 * RETURN CODES -- returns 0 if there are no held vnode, 1 if there is only one
 *     held vnode only once and it is the root, and 2 if there are one or more
 *     other vnode holds. */

int vnm_Inuse (
  struct volume *volp)
{
    long state = 0;			/* hash table iteration state */
    efs_vnode_t evp;			/* current vnode */
    int inuse = 0;

    /* We want to shake loose all possible vnodes. */
    vnm_purge_vol_dnlc(volp);
    efs_wait_for_pending_inactive();

    BEGIN_MUTEX (&vntableLock) {
	evp = NULL;
	while (evp = HashNext(evp, &state)) {
	    int count;

	    if (evp->vd_volp != volp)
		continue;
	    BEGIN_LOCK (efs_lockvp(evp),
			efs_unlockvp(evp)) {
		count = vn_count(evp);
	    } END_LOCK;
	    if (count == 0)
		continue;
	    if (epid_IsVolumeRoot (&evp->vd_fid) && (count == 1)) {
		if (!inuse)
		    inuse = 1;
	    } else {
		inuse = 2;
		icl_Trace4(efs_iclSet, EFS_TRACE_INUSE_BUSY,
			   ICL_TYPE_POINTER, evp,
			   ICL_TYPE_LONG, evp->vd_fid.index,
			   ICL_TYPE_LONG, count,
			   ICL_TYPE_LONG, evp->vd_onLRU);
		break;
	    }
	}
    } END_LOCK;
    return inuse;
}

/* vnm_SetVfsP -- this fills in the vfsp of each vnode to match the vfs pointer
 *     in the volp. */

void vnm_SetVfsP (
  struct volume *volp)
{
    long state = 0;			/* hash table iteration state */
    efs_vnode_t evp;			/* current vnode */
    struct osi_vfs *vfs = VTOVFS(volp);

    BEGIN_MUTEX (&vntableLock) {
	evp = NULL;
	while (evp = HashNext(evp, &state)) {
	    if (evp->vd_volp == volp)
		osi_vSetVfsp(EVTOV(evp), vfs);
	}
    } END_LOCK;
}

/* vnm_GetFileH -- returns the anode handle for a vnode making various
 *     consistency checks in the process.  This should only be used by callers
 *     that hold the fileLock and are allowed to access the anode handle,
 *     namely most (all?) vnodeops and some volops. */

epi_anode_t vnm_GetFileH (
  efs_vnode_t evp)
{
    Assert (!NoIdentity(evp) && evp->vd_file.anode);
    return evp->vd_file.ap;
}
