/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: efs_misc.c,v $
 * Revision 1.1.19.1  1996/10/02  17:44:06  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:35:52  damon]
 *
 * $EndLog$
 */
/*
 * Copyright (C) 1996, 1990 Transarc Corporation
 * All rights reserved.
 */
/*
 *			    Episode File System
 *		    Misc. support for Vnode operations
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/debug.h>
#include <dcedfs/lock.h>		/* osi_dlock_t */
#include <dcedfs/queue.h>		/* struct squeue */
#include <dcedfs/episode/file.h>	/* struct epif_status */
#include <dcedfs/episode/aggregate.h>	/* epig_ functions */
#include <dcedfs/episode/dacl_anode.h>
#include <dcedfs/osi_kvnode.h>
#include <dcedfs/dir.h>
#include <dcedfs/dir_buffer.h>
#include <efs_dirhandle.h>		/* struct epi_Dir_Handle */
#include <efs_evnode.h>			/* struct evnode & ICL dcls */
#include <efs_vnodeops.h>
#include <dcedfs/episode/efs_trace.h>
#include <dcedfs/episode/epi_thread.h>	/* Required by bomb.h */
#include <dcedfs/aggr.h>		/* VNOP_TYPE_READWRITE */
#include <dcedfs/volume.h>		/* VOL_RELE */
#include <dcedfs/volreg.h>
#include <dcedfs/xvfs_vnode.h>		/* SET_CONVERTED */
#include <dcedfs/bomb.h>

#include <efs_volvnops.h>
#include <efs_newalloc.h>
#include <efs_tmpcopy.h>
#include <efs_promote.h>
#include <efs_misc.h>

#define EFS_PRIVATE_POOL	/* use private pool for vnode allocation */

#if defined(AFS_OSF_ENV) && defined(_KERNEL)
#undef EFS_PRIVATE_POOL
#endif /* AFS_OSF_ENV */

#if defined(AFS_HPUX_ENV) && !defined(KERNEL)
#define VEFS 0x1234
#endif

#define DEBUG_THIS_FILE EFS_DEBUG_MISC

RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/episode/vnops/efs_misc.c,v 1.1.19.1 1996/10/02 17:44:06 damon Exp $")

#ifdef EFS_PRIVATE_POOL
/*
			Management of vnodes

All vnodes are taken from a pool.  The pool is allocated by vnm_Init; the
size of the pool is a parameter to that function.

Active vnodes are kept in a hash table.  The hash function takes the fid
and the volume handle, but the current implementation uses only the fid.

When an active vnode becomes inactive, it remains in the hash table, for use
if the file is reopened.  It is put on an LRU list.  If the vnode represents
a deleted file (and hence cannot be reopened), it is put at the head of the
LRU list; otherwise it is put at the tail.

A vnode on the LRU list, even though inactive, continues to be associated with
a file.  There may be data cached in the vnode about that file, e.g. the
atime.  If we take the vnode off the list in order to associate it with some
other file, we must first write the cached data about the old file through to
the anode layer.  This will generally require a transaction.  This introduces a
complication, in that a thread cannot start a transaction if it already has
a transaction pending.  Thus, the process of allocating a new vnode is divided
into two functions, PartialAlloc and FinishAlloc.  The former is called before
any transactions have been initiated, and hence may start transactions of its
own, provided it finishes them.  The latter is called after a transaction has
been started (and a new file has been created).

The usual entry to find a vnode, given a fid, is vnm_FindVnode.  This function
must be called before the caller starts any transactions.  If the caller
knows that the vnode isn't going to be in the hash table (e.g. for create or
mkdir), it can shortcut as follows:  before starting any transactions, call
vnm_PartialAlloc to get a vnode; afterwards, when the transaction has been
started and the file has been created, call vnm_FinishAlloc to get an anode
handle.
*/
#endif /* EFS_PRIVATE_POOL */

#if OLD_VNSYNC
#define VNHASHSIZE 511			/* nice juicy non-prime */

#ifdef EFS_PRIVATE_POOL
static struct evnode *vnpool = 0;	/* pool of all vnodes */
static int vnnvnodes;			/* size of pool */
#endif /* EFS_PRIVATE_POOL */
static struct squeue vnhash[VNHASHSIZE]; /* vnode hash table */
#ifdef EFS_PRIVATE_POOL
static struct squeue vnlru;		/* LRU queue head */
static struct squeue *vnlruP;		/* pointer to vnlru */
#endif /* EFS_PRIVATE_POOL */
static osi_dlock_t vntable_lock;	/* lock for hash table */
#endif

/* Forward declarations */

#if OLD_VNSYNC
static void vhash(struct evnode *, epi_volFileId_t *, epi_volume_t);
static void unvhash(struct evnode *);
static int fid_hash(epi_volFileId_t *, epi_volume_t);
#endif

/*
 * freelist of size one for vnm_AllocEBuffer and vnm_FreeEBuffer, and
 * a lock to protect it
 */

#if !defined(AFS_SUNOS5_ENV) && !defined(AFS_AIX_ENV)

struct efs_buf *ebfree = (struct efs_buf *) 0;

osi_dlock_t vnm_eblock;

#endif /* AFS_SUNOS5_ENV, AFS_AIX_ENV */

enum vtype vnm_stat_type_to_vtype [16] = {
    VNON,				/* 0 */
    VFIFO,				/* 01 - S_IFIFO */
    VCHR,				/* 02 - S_IFCHR */
    VBAD,
    VDIR,				/* 04 - S_IFDIR */
#if defined(AFS_SUNOS5_ENV) && !defined(AFS_SUNOS54_ENV)
    VXNAM,				/* 05 */
#else
    VBAD,
#endif
    VBLK,				/* 06 - S_IFBLK */
    VBAD,
    VREG,				/* 10 - S_IFREG */
    VBAD,
    VLNK,				/* 12 - S_IFLNK */
    VBAD,
#ifdef AFS_SUNOS5_ENV
    VBAD,
#else
    VSOCK,				/* 14 - S_IFSOCK */
#endif
    VBAD, VBAD, VBAD};

int vnm_vtype_to_stat_type [10] = {
    0,					/* VNON */
    S_IFREG,				/* VREG */
    S_IFDIR,				/* VDIR */
    S_IFBLK,				/* VBLK */
    S_IFCHR,				/* VCHR */
    S_IFLNK,				/* VLNK */

/*
 * So far so good.  But now we have to go different ways
 * for different platforms.
 */

#ifdef AFS_AIX31_ENV
    S_IFSOCK,				/* VSOCK */
    0,					/* VBAD */
    S_IFIFO,				/* VFIFO */
    S_IFCHR};				/* VMPC */
#elif defined(AFS_OSF_ENV)
    S_IFSOCK,				/* VSOCK */
    S_IFIFO,				/* VFIFO */
    0, 0};				/* VBAD */
#elif defined(AFS_SUNOS5_ENV)
    S_IFIFO,				/* VFIFO */
#ifdef AFS_SUNOS54_ENV
    0,					/* 7 */
#else
    S_IFNAM,				/* VXNAM */
#endif
    0, 0};				/* VBAD */
#else					/* suitable for some platforms we
					 * aren't currently supporting */
    S_IFSOCK,				/* VSOCK */
    0,					/* VBAD */
    S_IFIFO, 0};			/* VFIFO */
#endif

#if !defined(AFS_SUNOS5_ENV) && !defined(AFS_AIX_ENV)
/*
 * vnm_AllocEBuffer -- allocate a buffer for efs_bread
 */
/* EXPORT */
struct efs_buf *vnm_AllocEBuffer(void)
{
    struct efs_buf *ebuf;

    lock_ObtainWrite_r (&vnm_eblock);
    if (ebuf = ebfree) {
	ebfree = 0;
	goto out;
    }
    ebuf = osi_Alloc_r (sizeof (struct efs_buf));
out:
    lock_ReleaseWrite_r (&vnm_eblock);
    return (ebuf);
}

/*
 * vnm_FreeEBuffer -- free a buffer used for efs_bread
 */
/* EXPORT */
void vnm_FreeEBuffer(struct efs_buf *eb)
{
    lock_ObtainWrite_r (&vnm_eblock);
    if (ebfree)
	osi_Free_r ((opaque)eb, sizeof (struct efs_buf));
    else
	ebfree = eb;
    lock_ReleaseWrite_r (&vnm_eblock);
}

#endif /* AFS_SUNOS5_ENV, AFS_AIX_ENV */

/*
 * vnm_MapCommonError -- translate error codes in some common cases
 *
 * This maps error codes from the Episode anode layer and logbuf layer
 * into standard Unix error codes.
 */

int
vnm_MapCommonError(int epiCode, int debugBit)
{
    int unixCode;

    /*
     * Don't bother mapping if it is not an extended code (ie. doesn't fit in
     * a byte).
     */
    if (epiCode >= 0 && epiCode <= 0xff)
	return epiCode;

    switch (epiCode) {
      /* codes used in writing */
      case EPI_E_VOLUMEFULL:
#ifdef	AFS_SUNOS5_ENV
	unixCode = ENOSPC;
#else
	unixCode = EDQUOT;
#endif	/* AFS_SUNOS5_ENV */
	break;

      case EPI_E_AGGREGATEFULL:
	unixCode = ENOSPC;
	break;

      /* BLOCKTOOBIG can't occur till we have a 64-bit platform */
      case EPI_E_BLOCKTOOBIG:
      case EPI_E_TOOBIG: /* TOOBIG:  epif_WriteAux can unixCode = this */
	unixCode = EFBIG;
	break;

      /* codes used in file creation */
      case EPI_E_INDEXEXISTS:
	unixCode = EEXIST;
	break;

      /* codes used in clone/unclone/reclone */
      case EPI_E_COPYONWRITE_ILLEGAL:
      case EPI_E_NOTCOPYONWRITE:
	unixCode = EXDEV;
	break;

      case EPI_E_NYI:
	unixCode = ENOSYS;
	break;

      default:
	unixCode = EIO;
	break;
    }

    EFS_TRACE_VNOPS(debugBit, epiCode, unixCode);
    return unixCode;
}

/*
 * vnm_GetZLCVnode -- returns a vnode for the file specified by
 * '*iterP' in the LFS ZLC list.  This function is called from
 * VOL_GETZLC volume op.  It returns a held vnode in evPP.
 */
/* EXPORT */
int vnm_GetZLCVnode(
  struct volume *volp,			/* volume handle */
  u_int *iterP,				/* iteration handle pointer */
  struct evnode **evPP)			/* returned vnode */
{
    epi_volFileId_t fid;		/* file handle */
    int code;				/* error return code */

    code = epif_NextDeletedFile (VTOVH(volp), *iterP, ((u_long*)iterP), &fid);
    if (code) return(code);
    code = vnm_FindVnode (volp, &fid, evPP);
    return code;
}

/*
 * vnm_Truncate -- truncate a file (up or down); set the length, do
 *	any required promotions and COW splitting, discard any pages past
 *	new EOF and ensure that the unused portion of the final block
 *	or frag in the file contains zeroes.
 *
 *	The \arg{useVM} specifies whether there can be any existing VM pages
 *	for this file and is is typically set to FALSE from the volume op
 *	path.
 *
 * LOCKS:
 *	The caller is expected to lock the vnode.  This function may drop
 *	and re-obtain that lock.
 *
 * NOTES:
 *	It is possible to get quota/space errors on truncating, due to
 *	creating a new indirect block during COW.
 *
 *	This function may cause a page fault when it zeroes out
 *	the remainder of the final page.
 *
 *	Not atomic.  If we crash before updating the length, user may
 *	find zeroes in his file.
 */
int
vnm_Truncate(struct evnode *evp, u_long len, int flags, osi_cred_t *credp)
{
    long code = 0;
    epi_anode_t ap = EVTOA(evp);
    u_long blocksize = ap->device->blkSize;
    long filelen;
    int shrinking;
    daddr_t lastBlk;
    int useVM = flags & TRUNCATE_USEVM;

    /* to prevent rival truncates */
    AssertTWriteLocked(&evp->vd_tlock);

    if ((long)len < 0)
	return (EFBIG);

#if defined(AFS_AIX_ENV) && defined(KERNEL)
    /*
     * If useVM is set, make sure that the file has a VM segment so that
     * subsequent AIX VM operations will succeed.  Checking vd_seg
     * before calling efs_getvmh is a minor performance optimization.
     * It is safe to test it without holding the vmLock because the seg
     * will not be deleted while the vnode is in use.
     *
     * IBM 19202, db7254, OT13282
     */
    if (useVM && !EVTOV(evp)->v_gnode->gn_seg) {
        code = efs_getvmh(EVTOV(evp));
        if (code)
            return(code);
    }
#endif /* AFS_AIX_ENV and KERNEL */

    /* prevent any new pages from being created */
    lock_ObtainWrite_r(&evp->vd_vm.lock);
    /* write-lock fileLock to change anode contents */
    lock_ObtainWrite_r(&evp->vd_file.lock);

    if (useVM == 0) {
	osi_assert(evp->vd_vm.valid == 0);
    }

    filelen = efs_GetLength(evp);
    shrinking = (len < filelen);

    if (len == filelen)	/* no-op */
	goto done;

#ifdef AFS_AIX_ENV
    if (useVM) {
	code = efs_AvoidVMRaces(evp, len, blocksize, shrinking);
	if (code != 0)
	    goto done;
    }
#endif /* AFS_AIX_ENV */

    if (shrinking) {
	u_int maxInline = epix_GetMaxInlineData(ap);
	long diskEnd = DISK_SIZE(len, ap);

	if (useVM) {
	    /*
	     * Discard pages beyond the block or frag containing EOF.
	     * We allow pages to exist in the region between EOF and the
	     * following block or frag boundary because otherwise we
	     * would have to write zeroes to disk immediately in order
	     * to initialize the unused part of the block, which turns
	     * out to be a big performance hit.
	     *
	     * This will drop and reobtain the fileLock on AIX.
	     */
	    code = efs_DiscardVM(evp, filelen, diskEnd, credp);
	    if (code != 0)
		goto done;

	    /*
	     * If we are shrinking a COW file, we must make a writable copy
	     * of the last block so that we can zero it past the new EOF.
	     * Since this operation may fail because of quota or space errors,
	     * we do this before discarding any on-disk  data. Note that we use
	     * blocksize even when the file is fragged because if a frag is COW
	     * then the entire file system block that it lives in must be COW.
	     * We don't need to do anything for a file that will become inline.
	     */
	    if (len > maxInline) {
		long alen;
		off_t lastOff;
		int isCow;

		/*
		 * Check the nature (alloc, cow, hole) of the final
		 * block/frag after we truncate.
		 */
		lastOff = len & ~(blocksize - 1);
		code = efs_CheckAllocation(evp, lastOff, &lastBlk, &alen,
					   &isCow);
		if (code != 0)
		    goto done;

		/*
		 * If the new EOF is in the middle of a COW block, copy it.
		 * This means that shrinking the file may fail with a quota
		 * error!
		 *
		 * XXX If shrinking a COW file to a frag aligned length,
		 * can optimize by just reducing the COW length instead of
		 * breaking the COW linkage - ra.
		 */
		if (isCow && (len & (blocksize - 1)) != 0) {
		    osi_assert(lastBlk != EFS_HOLE);
		    code = efs_MakeWritableBlocks(evp, lastOff, len - lastOff,
						  MWB_USE_VM | MWB_ZEROFILL);
		    if (code != 0)
			goto done;
		}
	    }
	}

	/*
	 * Discard on-disk storage beyond new EOF.  However, if the file
	 * is fragmented, we defer the on-disk truncation until efs_Promote.
	 * If we were to free any fragments now, a concurrent pageout could
	 * write into the freed frags (and these might already be reallo-
	 * cated).  When we do it in efs_Promote, we will first lock the
	 * page covering EOF.
	 *
	 * The same problem does not arise for blocked files because
	 * the file system block size is always >= PAGESIZE.
	 */
	if (!EPIA_FRAGMENTED(ap)) {
	    afs_hyper_t hlen;
	    AFS_hset64(hlen, 0, len);
	    code = epia_Truncate(buffer_nullTranId, ap, 0, hlen);
	    if (code != 0)
		goto done;
	}

	/* Zero the data in the last block past EOF */
	if (useVM) {
	    if (!efs_MustBePromoted(evp, len) && !EPIA_INLINE(ap) &&
		lastBlk != EFS_HOLE) {
		/*
		 * Create zero-filled dirty pages to cover the last block past
		 * past EOF.  We only have to do this if the last block is not
		 * a hole and the old EOF was in a different page.  Also, if
		 * the file is about to be promoted, we let efs_Promote do the
		 * work for us.
		 */
		off_t endPage = NEXT_PAGE(len);
		off_t endBlock = EPIA_FRAGMENTED(ap) ?
		    NEXT_FRAG(len, ap) : NEXT_BLOCK(len, ap);

		if (endPage < endBlock && endPage < NEXT_PAGE(filelen))
		    efs_CreateDirtyZeroPages(evp, endPage, endBlock);
	    }
	}
    }
    code = efs_Promote(evp, len, useVM);
    if (shrinking)
	afsl_MBZ(code);

  done:
    /*
     * At this point, all pages past the final block have been
     * discarded and we have created dirty zero pages to cover
     * the block past EOF.  Now all we need to do is zero-fill
     * the portion of the last page beyond EOF.  Since we are
     * still holding the vd_lock, the length cannot change.
     *
     * If the last block is a hole, we do not need to do this.
     *
     * Note that the last block cannot be COW -- since we are not
     * on a page boundary and hence not on an FS block boundary,
     * we will already have made a writable copy above.  Thus, we
     * we can't fail here with allocation errors
     */
    if (useVM) {
	if (code == 0 && shrinking && (len & (PAGESIZE - 1)) != 0 &&
	    (EPIA_INLINE(ap) || lastBlk != EFS_HOLE)) {
	    EFS_ZEROFILL_LAST_PAGE(evp, len);
	}
    }

    lock_ReleaseWrite_r(&evp->vd_file.lock);
    lock_ReleaseWrite_r(&evp->vd_vm.lock);

    VNM_MAP_COMMON_ERROR (code, DEBUG_THIS_FILE);
    return (code);
}

/*
 * vnm_SetAtime -- update vnode copy of atime.  We only do this on files that
 *     can be modified.  This excludes files in readonly volumes or those being
 *     accessed during a volume operation.  The POSIX requirements are very
 *     relaxed for atime updates, so this is okay.
 *
 * LOCKS USED -- the cacheLock protects the vnode version atime and the bits
 *     that describe when it is okay to actually set the atime.  We don't
 *     depend on the tlock being held but it always is. */

/* EXPORT */
void vnm_SetAtime(struct evnode *evp)
{
    BEGIN_LOCK_WRITE (&evp->vd_cache.lock) {
	if (!EV_ISREADONLY(evp) && !evp->vd_cache.noChange) {
	    afsl_Assert (!evp->vd_cache.noDirty);
	    osi_GetTime(&evp->vd_cache.lastAtime);
	    evp->vd_cache.newAtime = 1;
	}
    } END_LOCK;
}

/*
 * vnm_SetMCtime -- update vnode copy of ctime and maybe mtime.
 */
/* EXPORT */
void vnm_SetMCtime(
  efs_evnode_t *evp,
  int markM)
{
    BEGIN_LOCK_WRITE (&evp->vd_cache.lock) {
	afsl_Assert (!evp->vd_cache.noDirty);
	osi_GetTime (&evp->vd_cache.lastCtime);
	evp->vd_cache.newCtime = 1;
	if (markM == MARK_MC) {
	    evp->vd_cache.lastMtime = evp->vd_cache.lastCtime;
	    evp->vd_cache.dataVersion++;
	    evp->vd_cache.newMtime = 1;
	}
    } END_LOCK;
}

/*
 * vnm_UpdateAnode -- update anode from vnode.
 *
 *     The caller may provide a transaction in which case we use it otherwise
 *     we start (and end) one of our own.  The caller must ensure that the
 *     anode is valid.
 *
 *     As an optimization, we call this function at the end of some vnode
 *     operations which modify M/Ctimes (e.g. efs_create and efs_rename).  The
 *     basic policy is to explicitly flush the new times through to the anode
 *     if we have a transaction handy and the anode in question has been
 *     modified anyway.  This allows us to defer dirtying the anode just to
 *     write out the times.  For instance, efs_rmdir updates the parent because
 *     its link count is decreasing but not the child which is being deleted.
 *
 * LOCKS USED -- Use the cacheLock to protect the times, etc.  This is a
 *     low-level lock, below StartTran in the hierarchy, so we must be careful
 *     to start the transaction before we obtain it.  To avoid starting a
 *     transaction when there is no dirty data to write thru we check first
 *     without holding the lock.  This should be safe: if we miss a write
 *     someone later can write it through, and in the opposite case we may very
 *     rarely start a transaction which will just be empty.
 *
 * CAUTIONS -- This function can be called at almost anytime.  While the vnode
 *     clearly must be held, the vd_file.ap may not be valid due to an
 *     intervening volume operation.  We can assume the ap is valid if we are
 *     passed a transId or if the fileLock is held. */

/* EXPORT */
void vnm_UpdateAnode (
  struct evnode *evp,
  elbb_tranRec_t transId)
{
    int code;
    epi_anode_t ap;			/* anode for evp */
    int new = elbb_IsZeroTrans(transId);

    afsl_Assert (!new || lock_Check (&evp->vd_file.lock));

    if (!EV_ISDIRTY(evp))
	return;

    /* We start the transaction here because the vd_cacheLock is *BELOW*
     * StartTran in the hierarchy. */

    ap = EVTOA (evp);
    if (new) {
	code = epia_StartTran("vnm_UpdateAnode", ap, &transId);
	afsl_MBZ (code);
    }

    BEGIN_LOCK_WRITE (&evp->vd_cache.lock) {
	long flags = 0;			/* status fields to update */
	long markFlags = 0;		/* status fields to auto-update */
	struct epif_status fstat;	/* status info about evp */

	if (evp->vd_cache.newLen) {
	    code = epix_SetLength(transId, ap, evp->vd_len, 0);
	    afsl_MBZ(code);
	}

	if (evp->vd_cache.newAtime) {
	    fstat.aTime = evp->vd_cache.lastAtime;
	    flags |= EPIF_SFLAGS_ATIME;
	}

	if (evp->vd_cache.newMtime) {
	    fstat.mTime = evp->vd_cache.lastMtime;
	    fstat.dataVersionNumber = evp->vd_cache.dataVersion;
	    flags |= (EPIF_SFLAGS_MTIME | EPIF_SFLAGS_DVERSION);
	}

	if (evp->vd_cache.newCtime) {
	    fstat.cTime = evp->vd_cache.lastCtime;
	    flags |= EPIF_SFLAGS_CTIME;
	    markFlags |= EPIF_SFLAGS_VVERSION;
	}

	if (flags || markFlags) {
	    afsl_Assert (!evp->vd_cache.noDirty);
	    code = epif_SetStatusAndMark (transId, ap, markFlags, flags,
					  &fstat);
	    afsl_MBZ (code);
	}
	evp->vd_cache.newAtime = evp->vd_cache.newMtime =
	    evp->vd_cache.newCtime = evp->vd_cache.newLen = 0;

    } END_LOCK;

    if (new) {
	code = epia_EndTran(ap, transId);
	afsl_MBZ (code);
    }
}

/*
 * vnm_ClearSUID -- clear SUID and SGID bits on behalf of writing
 *
 * LOCKS: called with vnode lock held for writing
 */

/* EXPORT */
void
vnm_ClearSUID(struct evnode *evp)
{
    int code;
    epi_anode_t ap;			/* anode handle for evp */
    elbb_tranRec_t transId;		/* transaction ID */
    struct epif_status fstat;		/* file status info for evp */

    ap = EVTOA (evp);

    /* Start transaction */
    code = epia_StartTran ("vnm_ClearSUID", ap, &transId);
    MBZ (code);

    /* Clear the bits */
    fstat.mode = epif_GetMode (ap) & ~(VSUID | VSGID);
    code = epif_SetStatus (transId, ap, EPIF_SFLAGS_MODE, &fstat);
    MBZ (code);

    /* Merge transaction */
    tlock_EquivAssign (transId, &evp->vd_tlock);

    /* End transaction */
    code = epia_EndTran (ap, transId);
    MBZ (code);
}

/*
 * vnm_CreateVolume -- create a volume, possibly with a root directory
 */
/* EXPORT */
int vnm_CreateVolume(
    epi_anode_t avl,			/* aggregate AVL */
    osi_cred_t *credP,			/* user credential structure */
    int createRoot,			/* true iff wish to create root dir */
    struct epiv_status *statusP,	/* volume status */
    u_long *indexP)			/* place to put index into AVL */
{
    epi_volume_t volh;			/* volume handle */
    struct epif_status fileStatus;	/* status for file creation */
    epi_volFileId_t fid;		/* fid for file creation */
    struct epi_Dir_Handle dirh;		/* dir handle for dir_MakeDir */
    u_long index;			/* local copy of indexP */
    elbb_tranRec_t transId = elbb_zeroTrans; /* transaction ID */
    int ccode, code;

    /* Create Volume */

    /* Start transaction */
    ccode = epia_StartTran ("vnm_CreateVolume", avl, &transId);
    afsl_MBZ (ccode);
    /* Do it */
    code = epig_CreateVolume (transId, avl, EPIG_NOATTACH, statusP, &index);
    epia_EndTran (avl, transId);
    if (code) goto bad;

    /* If requested, create root directory */
    if (createRoot) {
	epi_anode_t ap;

	/* Open volume */
	ccode = epig_OpenVolume (avl, index, &volh);
	afsl_MBZ (ccode);

	/* Start transaction */
	ccode = epia_StartTran ("vnm_CreateVolume", volh, &transId);
	afsl_MBZ (ccode);

	/* Initialize status for file creation */
	bzero ((caddr_t) &fileStatus, sizeof (fileStatus));
	/* Defect 4648 - Set root dir permissions bits to zero */
	fileStatus.mode = 0700 | S_IFDIR;
	fileStatus.oid = osi_GetUID (credP);
	fileStatus.gid = osi_GetGID (credP);

	/* Create the directory */
	code = epif_Create (transId, volh, EPIF_CFLAGS_VOLUMEROOT,
			    /* estLength */ 0,
			    &fileStatus,
			    /* acl, plist */ 0, 0,
			    &fid, &ap);
	if (code) {
	    goto end_tran;
	}

	/* Sanity check: did we get root fid? */
	afsl_Assert(epid_IsVolumeRoot(&fid));

	/* Initialize directory */
	edh_SetDirHandle (&dirh, ap, volh, &fid);

	code = dir_MakeDir((struct Dir_Handle *)&dirh, &fid, &fid, transId);
	dirb_Annul((struct Dir_Handle *)&dirh);
	if (code) {
	    /*
	     * XXX In this case since the directory is known to be small
	     * in size, it will be safe to truncate and delete the
	     * directory anode in one transaction. We can allow
	     * allow for using the existing tran to do this cleanup
	     */
	    ccode = epia_EndTran(avl, transId);
	    afsl_MBZ(ccode);
	    ccode = epif_Delete(ap, EPIF_DELETE_IGNORE_LINKCNT, NULL);
	    afsl_MBZ(ccode);
	    goto delete_vol;
	}

	/*
	 * Change link count from 1 to 2 because root's . and .. both
	 * point to itself.  Then end the transaction and clear the
	 * inconsistent bit.  Then do cleanup.
	 */
	code = epif_ChangeLink(transId, ap, 1, 0, (u_int *) 0);
	afsl_MBZ (code);
	code = epia_EndTran(volh, transId);
	afsl_MBZ (code);
	code = epiv_TwiddleInconBit(volh, 0);
	afsl_MBZ (code);
	code = epif_Close(ap);
	afsl_MBZ (code);
	code = epig_CloseVolume(elbb_zeroTrans, volh);
	afsl_MBZ (code);
    }
    if (indexP) *indexP = index;
    return 0;				/* Success */

/* Clean up paths for failed root creation: */

end_tran:
    ccode = epia_EndTran (avl, transId);
    afsl_MBZ (ccode);

delete_vol:
    ccode = epia_StartTran ("vnm_CreateVolume cleanup", avl, &transId);
    afsl_MBZ (ccode);
    ccode = epig_DeleteVolume (transId, volh);
    afsl_MBZ (ccode);
    ccode = epia_EndTran (avl, transId);
    afsl_MBZ (ccode);

bad:
    VNM_MAP_COMMON_ERROR (code, DEBUG_THIS_FILE);
    return code;
}

#if OLD_VNSYNC
#ifdef AFS_AIX_ENV
/*
 * efs_vn_link -- link an AIX vnode on lists hanging off its VFS and gnode
 *
 * vfsp might be null (for Episode fs, attached but not mounted).
 */
static void
efs_vn_link(struct vnode *vp, struct vfs *vfsp, struct gnode *gp)
{
    /* link on vfs list */

    if (vfsp) {
	vp->v_vfsnext = vfsp->vfs_vnodes;
	vp->v_vfsprev = NULL;
	vfsp->vfs_vnodes = vp;
	if (vp->v_vfsnext != NULL)
	    vp->v_vfsnext->v_vfsprev = vp;
    }
}

/*
 * efs_vn_unlink -- unlink an AIX vnode from various lists
 */
static void
efs_vn_unlink(struct vnode *vp)
{
    struct vfs *vfsp = vp->v_vfsp;

    /* unlink from vfs list */

    if (vfsp) {
	if (vfsp->vfs_vnodes == vp)
	    vfsp->vfs_vnodes = vp->v_vfsnext;
	if (vp->v_vfsnext != NULL)
	    vp->v_vfsnext->v_vfsprev = vp->v_vfsprev;
	if (vp->v_vfsprev != NULL)
	    vp->v_vfsprev->v_vfsnext = vp->v_vfsnext;
    }
}

#endif /* AFS_AIX_ENV */
#endif /* OLD_VNSYNC */
