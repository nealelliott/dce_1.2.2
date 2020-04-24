/*
 * Copyright (C) 1996, 1994 Transarc Corporation
 * All rights reserved.
 */
#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/osi_uio.h>
#include <dcedfs/osi_buf.h>
#include <dcedfs/osi_vmm.h>
#include <dcedfs/osi_device.h>
#include <dcedfs/episode/anode.h>
#include <dcedfs/episode/block_alloc.h>

#include "efs_evnode.h"
#include "efs_misc.h"
#include "efs_vnodeops.h"
#include "efs_tmpcopy.h"
#include "efs_newalloc.h"

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/vnops/RCS/efs_newalloc.c,v 1.211 1996/05/07 14:34:21 andi Exp $")

#define DEBUG_THIS_FILE EFS_DEBUG_NEWALLOC

/*
 * efs_CheckAllocation returns the  address of the disk block at the
 * specified offset (or EFS_HOLE if there is none), and the length
 * in bytes of contiguous storage starting at the offset.  Blocks
 * are only considered contiguous if they are of the same type
 * (i.e., writable or read-only).  The returned length is really
 * a lower bound on the size of the contiguous region, since the
 * function stops upon reaching the end of the current allocation
 * block.
 *
 * If the file is fragmented and all of the frags are not allocated,
 * the disk address of the currently allocated fragments is returned
 * and alenP is set to the number of bytes that can fit in the already
 * allocated frags.
 *
 * efs_CheckAllocation(evp, off, blkP, alenP, isCowP)
 *
 * Input Parameters:
 *	evp = the vnode of interest
 *	off = the offset at which to start looking (in bytes)
 *
 * Output Parameters:
 *	blkP = the disk address of the contiguous blocks that we found.
 *		If no blocks were allocated, it will be set to EFS_HOLE.
 *
 *	alenP = the length (in bytes) of contiguous blocks found, OR
 *		the length of the unallocated space beginning at off.
 *
 *	isCowP = set if the requested offset is in a COW block/frag.
 */
long
efs_CheckAllocation(
  efs_vnode_t evp, 
  long off,
  daddr_t *dblkP,
  long *alenP,
  int *isCowP)
{
    long code;
    epi_anode_t ap = EVTOA(evp);
    u_long anodeLen = DISK_SIZE(epia_GetLen32(ap), ap);
    daddr_t fsb;
    long blkOff;

    *alenP = 0;
    *isCowP = 0;

/* True on all paths but putpage. */
/* afsl_Assert(lock_Check(&evp->vd_file.lock) != 0); */

    /* If the requested offset is between the anode length and the vnode's
     * (dirty, cached) length just return EFS_HOLE since no blocks are
     * allocated. */
    if (off >= anodeLen) {
	/*
	 * Currently for fragmented files we cannot have any dirty
	 * cached length in the vnode due to the way efs_FragToFrag works
	 */
	u_long vnodeLen;		/* vnode's latest idea of length */

	afsl_Assert(EPIA_BLOCKED(ap) &&
		    (evp->vd_flags & VD_LENVALID));
	BEGIN_LOCK_WRITE(&evp->vd_cache.lock) {
	    afsl_Assert(evp->vd_cache.newLen);
	} END_LOCK;
	vnodeLen = AFS_hgetlo(evp->vd_len);
	afsl_Assert(off < vnodeLen);
	*dblkP = EFS_HOLE;
	*alenP = vnodeLen-off;
	*isCowP = 0;
	return 0;
    }

    afsl_Assert(off < anodeLen);

    if (EPIA_INLINE(ap)) {
	*dblkP = EFS_HOLE;
	*alenP = anodeLen - off;
	*isCowP = 0;
	return 0;
    }

    if (EPIA_FRAGMENTED(ap)) {/* file is fragmented */
	epix_fragDescr_t frags;
	int lgFsize = ap->device->logFragSize;

	/* Can't have holes in frag files */
	code = epix_GetFragInfo(ap, &frags);
	afsl_MBZ(code);
	*alenP = (frags.length << lgFsize) - off;
	afsl_Assert(*alenP > 0);
	if (EPIX_IsBlockInBacking(frags.block)) {
	    *isCowP = 1;
	    frags.block = epix_MapBlockToBacking(frags.block);
	} else
	    *isCowP = 0;
	*dblkP = EFS_FRAG_DADDR(ap->device, &frags) + btodb(off);
    } else {
	blkOff = off & ~(ap->device->blkSize - 1);
	code = epix_CheckBlockAllocation(ap, blkOff, &fsb, alenP);
	if (code != 0)
	    return (code);
	/* convert to disk blocks from filesystem blocks */
	if (EPIX_IsBlockInBacking(fsb)) {
	    *isCowP = 1;
	    fsb = epix_MapBlockToBacking(fsb);
	} else
	    *isCowP = 0;

	*dblkP = (fsb == EFS_HOLE) ? EFS_HOLE :
		abtodb(ap->device, fsb) + btodb(off - blkOff);
	*alenP -= off - blkOff;
    }
    return (0);
}

/*
 * Initialize a buf in preparation for disk I/O; we assume that
 * on entry, the buf has been zeroed and its locks have properly
 * initialized.
 *
 * efs_{Get,Put}ContentsNoVM use this interface.  In the case of
 * ordinary I/O resulting from page-ins or page-outs, the buf will
 * be initialized by other means: in pageio_setup on SunOS, and
 * in the strategy daemon on AIX.
 */
static void
InitBuf(
  struct buf *bp,
  int flags,
  caddr_t addr,			/* in-core data area */
  size_t length,		/* transfer length */
  daddr_t dblkno,		/* disk address */
  struct vnode *vp)		/* vnode plus aggregate info */
{
    async_device_t *aggr = epia_AnodeDevice(EVTOA(VTOEV(vp)));
    dev_t device = osi_makedev(aggr->majdev, aggr->mindev);

    osi_BufInit(bp, flags, addr, length, device, dblkno, vp, epia_Iodone);
}

/*
 * efs_GetContentsNoVM copies a file's user data into temporary storage
 * without creating any VM pages; efs_PutContentsNoVM similarly writes
 * user data to disk without using any pages.  Clearly, for this to work
 * correctly, there must be no dirty pages corresponding to the data
 * that we are copying, and there must be no possibility of creating
 * any until the copying is complete.  We depend on the higher-level
 * code to ensure this.  These interfaces only allow reading or writing
 * a contiguous range of disk blocks.
 */
long
efs_GetContentsNoVM(
  struct evnode *evp,
  long len,		/* how much to read */
  daddr_t rblk,		/* disk address to start reading */
  char *tmp)		/* where to read into */
{

    struct buf *bp;
    long code;

    osi_assert(len > 0);
    bp = osi_GetBuf_r();
    InitBuf(bp, B_READ, tmp, len, rblk, EVTOV(evp));
    code = epia_Strategy(bp, EVTOA(evp));
    afsl_MBZ(code);
    code = epia_BioWait(bp);
    osi_ReleaseBuf_r(bp);
    return (code);
}

long
efs_PutContentsNoVM(
  struct evnode *evp,
  long len,				/* how much to write */
  daddr_t wblk,				/* disk address to start writing */
  char *tmp)				/* buffer to read from */
{
    struct buf *bp;
    long code;

    osi_assert(len > 0);
    bp = osi_GetBuf_r();
    InitBuf(bp, B_WRITE, tmp, len, wblk, EVTOV(evp));
    code = epia_Strategy(bp, EVTOA(evp));
    afsl_MBZ(code);
    code = epia_BioWait(bp);
    osi_ReleaseBuf_r(bp);
    return (code);
}

/*
 * Put a limit on how many blocks we allocate in one transaction in
 * order to keep the transactions from getting too big.
 */
#define MAX_BLKS_ALLOC_IN_ONE_TRAN	10

/*
 * CreateBlocks allocates a set of contiguous disk blocks for the
 * anode at the specified offset.  lenP is an in-out parameter.
 * On entry, it contains the number of bytes of storage desired.
 * If this much contiguous space cannot be found, CreateBlocks may
 * allocate to a smaller amount (but not zero); the caller is responsible
 * for calling CreateBlocks as many times as necessary until all the
 * allocation is performed. If cowSplit is 1 this block is created
 * because we are trying to write into a COW block and it means that the
 * volume's visible quota shouldn't be charged with that block
 *
 * This routine handles both blocked and fragmented files.  However,
 * it does not do promotion, so the file must be of the right type by
 * the time that we call this routine.
 *
 * XXX This function is too long and complicated.
 *
 * Input Params:
 *	evp = the vnode for which we want to create blocks
 *      tran = the transaction in which this should be done
 *      isCow = are we creating a copy of a COW block (for quota)
 * Inout Params:
 *	lenP =	on entry, the desired length (in bytes) of the allocation;
 *		on exit, the length of the actual allocation.
 *      offP =	on entry, the offset where allocation is needed;
 *		on exit, where we started allocating (<= initial value).
 *	dblkP = on entry, a hint about where we think this block should be
 *		allocated (or EFS_HOLE); on exit, the disk address of
 *		start of the allocation.
 */
static long
CreateBlocks(
  struct evnode *evp,
  elbb_tranRec_t tran,
  long *offP,
  long *lenP,
  daddr_t *dblkP,
  int *tranSpaceP,
  int isCow)
{
    epi_anode_t ap = EVTOA(evp);
    async_device_t *aggr = ap->device;
    int lgFsize = aggr->logFragSize;
    int lgFPB = aggr->logBlkSize - lgFsize;
    long reqOff = *offP, reqLen = *lenP;
    long reqEnd = reqOff + reqLen;
    epix_blockLoc_t loc;
    daddr_t ibP[ANODE_INDIRECT_TREES];
    daddr_t ibAddr;
    daddr_t fsb[MAX_BLKS_ALLOC_IN_ONE_TRAN];
    baddr_t lbn;
    u_long nblocks;
    int i, ibneeded = 0;
    epia_quota_t delta;
    long code, tcode;

    afsl_Assert(reqLen > 0);
    if (EPIA_FRAGMENTED(ap)) {
	epix_fragDescr_t frags;

	code = epix_GetFragInfo(ap, &frags);
	afsl_MBZ(code);

	if (isCow)
	   reqEnd = MAX(epia_GetLen32(ap), reqEnd);

	delta.v = (NEXT_FRAG(reqEnd, ap) >> lgFsize) - frags.length;
	delta.a = epix_IsBlockWritable(frags.block) ?
		  delta.v : delta.v + frags.length;
	afsl_Assert(delta.a > 0);

	code = epia_UseQuota(ap, tran, delta);
	if (code != 0)
	    goto fail;

	code = efs_ExtendFragAllocation(evp, tran, reqEnd, &frags);
	if (code == 0) {
	    *dblkP = EFS_FRAG_DADDR(aggr, &frags);
	    *lenP = EFS_FRAG_LENGTH(aggr, &frags) - reqOff;
	} else {
	    delta.a = -delta.a;
	    delta.v = -delta.v;
	    tcode = epia_UseQuota(ap, tran, delta);
	    afsl_MBZ(tcode);
	    goto fail;
	}
	goto done;
    }

    /*
     * Give the allocator the location of our last allocation as a hint.
     */
     if (*dblkP != EFS_HOLE)
	epia_SetAllocationRock(ap, dbtoab(aggr, *dblkP - 1), 0);

    /*
     * Call epix_FindBlocks() to get a loc struct for epix_Insert()
     */
    nblocks = osi_btoab(aggr, NEXT_BLOCK(reqLen, ap));
    nblocks = MIN(nblocks, *tranSpaceP);
    afsl_Assert(nblocks != 0 && nblocks <= MAX_BLKS_ALLOC_IN_ONE_TRAN);

    lbn = osi_btoab(aggr, reqOff);
    code = epix_FindBlocks(ap, lbn, nblocks, 0 /* forWrite */,
			   fsb, NULL, NULL, &loc);
    if (code != 0)
	goto fail;

    if (EPIX_IsLocNonTerminalIndirect(&loc)) {
	/*
	 * There is a COW or hole in the indirect tree that we must
	 * replace before we can insert any data blocks.
	 *
	 * Determine the number of blocks we will need to allocate to
	 * populate the indirect chain.  Also, make sure that we don't
	 * try to insert more than one allocation block's worth of data
	 * blocks.
	 */
	u_long nalloc = ibneeded = loc.tree + 1 - loc.depth;
	epix_blockLoc_t rLoc;
	u_long ibCows = 0;

	if (ibneeded + 1 > *tranSpaceP) {
	    /*
	     * We can't even allocate 1 data block without starting
	     * a new transaction so bail out now. Our caller will
	     * get a new transaction and call us back
	     */
	    *tranSpaceP = 0;
	    *lenP = 0;
	    goto done;
	}

	*tranSpaceP -= nalloc;
	nblocks = MIN(nblocks, epix_GetIBSize(ap));
	ibAddr = fsb[0];

	if (ibAddr != EFS_HOLE) {
	    /*
	     * Call epix_FindBlocks a second time with the forRead
	     * parameter set to discover the number of COW allocation
	     * blocks.
	     */
	    code = epix_FindBlocks(ap, lbn, 1, 1 /* forRead */,
				   fsb, NULL, NULL, &rLoc);
	    afsl_MBZ(code);
	    afsl_Assert(loc.tree == rLoc.tree);
	    ibCows = rLoc.depth - loc.depth;
	    afsl_Assert(ibCows != 0 && ibCows <= ibneeded);
	}

	/*
	 * The general algorithm is to do the allocation and
	 * then adjust the quota.  If the quota upgrade fails,
	 * free the blocks.  The reason for using this sequence is that
	 * the call to allocate direct blocks will not always allocate
	 * as many blocks as we ask.
	 */
	code = epib_Allocate(tran, ap, &nalloc, ibP, 0);
	if (code != 0)
	    goto fail;

	afsl_Assert(nalloc == ibneeded);
	delta.a = ibneeded << lgFPB;
	delta.v = (ibneeded - ibCows) << lgFPB;
	code = epia_UseQuota(ap, tran, delta);
	if (code != 0) {
	    tcode = epib_Free(tran, ap, ibneeded, ibP);
	    afsl_MBZ(tcode);
	    goto fail;
	}
    }

    code = epib_Allocate(tran, ap, &nblocks, fsb, EPIB_CONTIG_ONLY);

    *tranSpaceP -= nblocks;

    if (code == 0) {
	epia_quota_t moredelta;

	moredelta.a = nblocks << lgFPB;
	moredelta.v = (isCow) ? 0 : moredelta.a;
	code = epia_UseQuota(ap, tran, moredelta);
	if (code != 0) {
	    tcode = epib_Free(tran, ap, nblocks, fsb);
	    afsl_MBZ(tcode);
	}
    }

    if (code != 0) {
	if (ibneeded != 0) {
	    /* free the indirect blocks */
	    tcode = epib_Free(tran, ap, ibneeded, ibP);
	    afsl_MBZ(tcode);
	    delta.a = -delta.a;
	    delta.v = -delta.v;
	    tcode = epia_UseQuota(EVTOA(evp), tran, delta);
	    afsl_MBZ(tcode);
	}
	goto fail;
    }

    /*
     * We have allocated all the indirect blocks and at least
     * one data block.
     *
     * If we allocated indirect blocks, ibAddr now points to the
     * last indirect block that was either COW or empty.
     */
    for (i = 0; i < ibneeded; i++) {
	afsl_Assert(EPIX_IsLocNonTerminalIndirect(&loc));
	code = epix_Insert(tran, ap, 1, &ibP[i], &loc);
	afsl_MBZ(code);
	code = epix_FindBlocks(ap, lbn, 1, 0 /* for write */, &ibAddr,
				NULL, NULL, &loc);
	afsl_MBZ(code);
    }

    /* now insert the contiguous data blocks */

    code = epix_Insert(tran, ap, nblocks, fsb, &loc);
    afsl_MBZ(code);

    *offP = osi_abtob(aggr, lbn);
    *lenP = osi_abtob(aggr, nblocks);
    *dblkP = abtodb(aggr, fsb[0]);

done:
    icl_Trace4(efs_iclSetalloc, EFS_TRACE_CREATEBLOCK,
	       ICL_TYPE_POINTER, (evp),
	       ICL_TYPE_LONG, *offP,
	       ICL_TYPE_LONG, *lenP,
	       ICL_TYPE_LONG, *dblkP);
    return code;

fail:
    icl_Trace1(efs_iclSetalloc, EFS_TRACE_CRBL_FAIL,
	       ICL_TYPE_LONG, code);
    goto done;
}

/*
 * CopyBlocks produces writable copies of a set of read-only disk blocks.
 * lenP has the same interpretation as for CreateBlocks.  The read-only
 * blocks are guaranteed to be contiguous.
 *
 * This routine presents some tricky problems.  Initializing the new
 * copies synchronously here is easy but probably slow.  If we do the
 * write asynchronously, we must make sure there are pages containing
 * the data so that we don't read from the disk until the new blocks
 * are initialized.
 *
 * XXX We only handle blocked files here.  Fragments get copied
 * in efs_ExtendFragAllocation, called via CreateBlocks.  If we
 * want to handle fragmented files here, we will need to avoid
 * trying to copy the data twice.
 */
static long
CopyBlocks(
   struct evnode *evp,
   elbb_tranRec_t tran,
   long off,
   long len,
   daddr_t rblk,
   int *tranSpaceP,
   int useVM)
{
    epi_anode_t ap = EVTOA(evp);
    long remaining;
    long fsbsize = ap->device->blkSize;
    long code = 0;
    long diskEnd, tlen;
    daddr_t wblk;
    long off0;
    char *tmp = osi_AllocPinned_r(fsbsize);
    TMPBUF_DECL

    /*
     * XXX For now, we do the copy one block at a time.  We don't
     * want to lock down an unbounded number of pages between the
     * calls to GETCONTENTS and PUTCONTENTS, and we must get the
     * old contents before calling CreateBlocks, since that will
     * insert new blocks and make the old data inaccessible.  To
     * keep things simple, we will just handle one block at a time,
     * but a better solution would be first to allocate as many
     * contiguous blocks as we can (without inserting them), then
     * read the old data and insert the new blocks.  This would
     * entail splitting CreateBlocks into separate allocation and
     * insertion parts.
     */

    /* First figure out how far we'll need to read */
    afsl_Assert(EPIA_BLOCKED(ap));
    diskEnd = NEXT_BLOCK(off + len, ap);

    /* Always back up to the beginning of the filesystem block */
    off0 = off;
    off &= ~(fsbsize - 1);
    remaining = diskEnd - off;
    rblk -= btodb(off0 - off);
    icl_Trace4(efs_iclSetalloc, EFS_TRACE_CPBL, ICL_TYPE_POINTER, (evp),
	       ICL_TYPE_LONG, off,
	       ICL_TYPE_LONG, remaining,
	       ICL_TYPE_LONG, rblk);

    while (remaining > 0) {
	long tlen0;

	off0 = off;
	tlen0 = tlen = MIN(remaining, fsbsize);

	if (useVM)
	    code = EFS_GETCONTENTS(evp, off, tlen, rblk);
	else
	    code = efs_GetContentsNoVM(evp, tlen, rblk, tmp);
	if (code != 0)
	    goto done;

	wblk = EFS_HOLE;
	code = CreateBlocks(evp, tran, &off, &tlen, &wblk, tranSpaceP, 1);
	if (code != 0 || tlen == 0) {
	    /*
	     * CreateBlocks failed or we couldn't allocate any more blocks
	     * in these transaction. In any case let our caller handle it
	     */
	    if (useVM)
		/* unlock the pages and free the temporary buffers */
		EFS_PUTCONTENTS(evp, EFS_HOLE, 0, 1/* justUnlock*/);
	    goto done;
	}

	/* Since off is filesystem block size aligned */
	osi_assert(off == off0);
	osi_assert(off + tlen <= NEXT_BLOCK(off0 + tlen0, ap));

	if (useVM)
	    EFS_PUTCONTENTS(evp, wblk, tlen, 0);
	else {
	    code = efs_PutContentsNoVM(evp, tlen, wblk, tmp);
	    afsl_MBZ(code);
	}

	remaining -= tlen;
	rblk += btodb(tlen);
	off += tlen;

	if (*tranSpaceP == 0 && remaining > 0)
	    goto done; /* to avoid the zerobytes */
    }

    /*
     * if the block that we copied was the last block in the file and
     * the file's length is more than a page away from the end of the
     * newly allocated block, we need to do some zerofilling.
     */
    diskEnd = NEXT_BLOCK(off, ap);

    if (useVM)
	efs_CreateDirtyZeroPages(evp, NEXT_PAGE(off), diskEnd);
    else
	efs_ZeroBytes(evp, wblk, tlen, diskEnd - off);

done:
    osi_FreePinned_r(tmp, fsbsize);
    icl_Trace1(efs_iclSetalloc, EFS_TRACE_CPBL_FAIL, ICL_TYPE_LONG, code);
    return (code);
}

/*
 * int efs_MakeWritableBlocks(vp, off, len, flags)
 *
 * Make sure that writable disk blocks back the given vp offset and length,
 * allocating if necessary (except in the case of in-line files).  Returns
 * an error if the allocation cannot be done.  Called with vp->vd_file.lock
 * write-locked.
 *
 * Any allocated blocks should appear to be zero-filled in case no one
 * ever gets around to filling them up with data. We will do so if the
 * flags specify that zerofilling is to be done. It the flags also
 * specify use of pages, then this zerofilling can be done by creating
 * dirty zero-filled pages corresponding to the new blocks. Making the
 * pages may get sticky if block size < page size; we will ignore that
 * case.  If flags do not specify use of pages, then the blocks have
 * to be zeroed on disk. If the flags also specify "overwrite mode", then
 * do not copy the data over when breaking COW links.
 *
 * The reason for not unconditionally creating pages is that some I/O
 * paths (e.g., volume ops) want to avoid the VM system. The reason
 * for not zerofilling unconditionally is that the sometimes volume
 * ops guarantee to overwrite all allocated disk storage immediately;
 * in such cases, we would like to avoid an extra disk I/O for the
 * zerofill.
 *
 * Assumptions: vp has the correct storage type (no promotion needed).
 *
 * Hidden assumption: the block allocator should be smart enough so
 * that successive calls at adjacent offsets are likely to result in
 * contiguous disk allocation.
 */
long
efs_MakeWritableBlocks(
  struct evnode *evp,
  long off,
  long len,
  long flags)
{
    epi_anode_t ap = EVTOA(evp);
    elbb_tranRec_t tran;
    daddr_t dblk;
    long alen, code;
    int tranSpace = MAX_BLKS_ALLOC_IN_ONE_TRAN;
    int tranStarted = 0;
    int useVM = (flags & MWB_USE_VM);
    int zeroFill = (flags & MWB_ZEROFILL);
    int dontRead = (flags & MWB_OVERWRITE);
    int isCow;

    afsl_Assert(lock_WriteLocked(&evp->vd_file.lock));
    afsl_Assert(lock_Check(&evp->vd_vm.lock) != 0);

    if (evp->vd_file.anodeRO)
	return (EROFS);

    if (EPIA_INLINE(ap)) {
	afsl_Assert(off + len <= epix_GetMaxInlineData(ap));
	return 0;
    }

    if ((off + len) > DISK_SIZE(epia_GetLen32(ap), ap)) {
	BEGIN_LOCK_WRITE (&evp->vd_cache.lock) {
	    afsl_Assert (evp->vd_cache.newLen != 0);
	    code = epia_StartTran("efs_MakeWritableBlocks", ap, &tran);
	    afsl_MBZ(code);
	    tranStarted = 1;
	    code = epix_SetLength(tran, ap, evp->vd_len, 0);
	    afsl_MBZ(code);
	    evp->vd_cache.newLen = 0;
	} END_LOCK;
    }

    while (len > 0) {
	long off0 = off;
	/* What is at this offset? */
	code = efs_CheckAllocation(evp, off, &dblk, &alen, &isCow);
	if (code != 0)
	    break;

	if (dblk != EFS_HOLE && !isCow) {
	    evp->vd_lastDblk = dblk;
	    evp->vd_dblkOff = dbround(off);
	}
	alen = MIN(len, alen);
	if (dblk == EFS_HOLE ||
	    isCow && (dontRead || EPIA_FRAGMENTED(ap))) {
	    /*
	     * the block has not been allocated, or it's a COW but our caller
	     * is about to overwite it, or it's a COW fragmented file in which
	     * case CreateBlocks will copy the data to the newly allocated
	     * fragments.   XXX Note that we can't call CopyBlocks on
	     * fragmented COWs because efs_ExtendFragAllocation will copy
	     * the data and we would get into locking difficulties if
	     * CopyBlocks also tried to copy it.
	     */
	    if (!tranStarted) {
		code = epia_StartTran("efs_MakeWritableBlocks", ap, &tran);
		afsl_MBZ(code);
		tranStarted = 1;
	    }
	    dblk = (evp->vd_lastDblk == EFS_HOLE) ? EFS_HOLE :
		evp->vd_lastDblk + btodb(off - evp->vd_dblkOff);
	    code = CreateBlocks(evp, tran, &off, &alen, &dblk,
				&tranSpace, isCow);
	    if (code != 0)
		break;

	    evp->vd_lastDblk = dblk;
	    evp->vd_dblkOff = off;

	    if (zeroFill) {
		/*
		 * Either create zero-filled pages to cover the allocated
		 * storage, or write zeroes directly to disk to initialize
		 * the unused portions of the block.
		 */
		if (useVM)
		    efs_CreateDirtyZeroPages(evp, off, off + alen);
		else {
		    if (off < off0)
			efs_ZeroBytes(evp, dblk, 0, off0 - off);
		    if (off0 + len < off + alen)
			efs_ZeroBytes(evp, dblk, off0 + len - off,
				      off + alen - (off0 + len));
		}
	    }
	    len += off0 - off;
	} else if (isCow) {
	    if (!tranStarted) {
		code = epia_StartTran("efs_MakeWritableBlocks", ap, &tran);
		afsl_MBZ(code);
		tranStarted = 1;
	    }
	    code = CopyBlocks(evp, tran, off, alen, dblk, &tranSpace, useVM);
	    if (code != 0)
		break;
	} /* else do nothing if allocated & writable */
	len -= alen;
	off += alen;

	if (tranSpace == 0 && len != 0) {
	    /* start a new transaction */
	    afsl_Assert(tranStarted);
	    (void) epia_EndTran(ap, tran);
	    tranSpace = MAX_BLKS_ALLOC_IN_ONE_TRAN;
	    tranStarted = 0;
	}
    }
    if (tranStarted)
	(void) epia_EndTran(ap, tran);

    return vnm_MapCommonError(code, DEBUG_THIS_FILE);
}

/*
 * Find the number of allocated blocks starting at the specified
 * offset.  wrLenP returns the number of contiguous writable blocks
 * starting at off, and rdLenP returns the number of contiguous
 * readable blocks.
 */
long
efs_FindBlocks(
  struct evnode *evp,
  long off,
  daddr_t *blockP,
  long *wrLenP,
  long *rdLenP)
{
    long wlen = 0, rlen = 0;
    daddr_t first_blk, ablk;
    epi_anode_t ap = EVTOA(evp);
    long filelen = DISK_SIZE(epia_GetLen32(ap), ap);
    long alen;
    long code;
    int isCow;

    ap = EVTOA(evp);

    afsl_Assert(lock_Check(&evp->vd_file.lock) != 0);

    if (EPIA_INLINE(ap)) {
	*blockP = EFS_HOLE;
	*wrLenP = *rdLenP = filelen - off;
	return 0;
    }

    code = efs_CheckAllocation(evp, off, &ablk, &alen, &isCow);
    if (code != 0)
	return vnm_MapCommonError(code, DEBUG_THIS_FILE);

    first_blk = ablk;
    while (ablk != EFS_HOLE && ablk == first_blk + rlen) {
	if (!isCow && wlen == rlen)
	    wlen += alen;
	rlen += alen;
	off += alen;
	if (off >= filelen)
	    break;
	code = efs_CheckAllocation(evp, off, &ablk, &alen, &isCow);
	if (code != 0)
	    break;
    }
    if (code == 0) {
	*blockP = first_blk;
	*wrLenP = wlen;
	*rdLenP = rlen;
    }
    return vnm_MapCommonError(code, DEBUG_THIS_FILE);
}

/*
 * Like efs_FindBlocks, but returns only the number of consecutive
 * writable blocks.
 *
 * OUTPUT
 *
 * File type	*blockP			*lenP
 *
 * allocated	disk blk addr		# writable bytes from off to END
 *					of allocated disk storage unit
 *
 * cow		backing disk blk addr	zero
 *
 * hole		EFS_HOLE		zero
 */
long
efs_FindWritableBlocks(
  struct evnode *evp,
  long off,
  daddr_t *blockP,
  long *lenP)
{
    epi_anode_t ap = EVTOA(evp);
    long code = 0;
    int isCow;
    u_long vlen;

    /*
     * XXX Why does this function take the file length into account
     * for inline files, when it does not do so for frag and blk files
     */
    if (EPIA_INLINE(ap)) {
	long len = epix_GetMaxInlineData(ap);
	osi_assert(off < len);
	*blockP = EFS_HOLE;
	*lenP = len - off;
	return 0;
    }

    code = efs_CheckAllocation(evp, off, blockP, lenP, &isCow);
    if (code != 0)
	return vnm_MapCommonError(code, DEBUG_THIS_FILE);

    if (isCow || (*blockP == EFS_HOLE))
	*lenP = 0;

    return 0;
}

/*
 * Return offset where the NEXT allocated block appears in the given
 * range, or -1 if none. This function only searches forwards for the
 * end of the hole and not backwards.
 */
long
efs_NextBlock(struct evnode *evp, long *offp, long len)
{
    epi_anode_t ap = EVTOA(evp);
    daddr_t ablk = EFS_HOLE;
    long alen = 0;
    long code = 0;
    off_t off, eoff;
    int isCow;

    off = *offp;
    afsl_Assert(lock_Check(&evp->vd_file.lock) != 0);
    if (len == 0)
	len = epia_GetLen32(ap) - off;

    eoff = off + len;

    while (ablk == EFS_HOLE && off < eoff && code == 0) {
	code = efs_CheckAllocation(evp, off, &ablk, &alen, &isCow);
	off += MIN(INT_MAX - off, alen);
    }

    if (code == 0)
	*offp = (ablk == EFS_HOLE) ? -1 : off - alen;

    return vnm_MapCommonError(code, DEBUG_THIS_FILE);
}

/*
 * Like efs_NextBlock, but finds next writable block.
 */
long
efs_NextWritableBlock(struct evnode *evp, long *offp, long len)
{
    epi_anode_t ap = EVTOA(evp);
    daddr_t ablk;
    long alen;
    long code;
    off_t o, eoff;
    int isCow;

    o = *offp;
    afsl_Assert(lock_Check(&evp->vd_file.lock) != 0);
    if (len == 0)
	len = epia_GetLen32(ap) - o;

    code = efs_CheckAllocation(evp, o, &ablk, &alen, &isCow);

    eoff = o + len;
    while (isCow && o < eoff && code == 0) {
	o += alen;
	code = efs_CheckAllocation(evp, o, &ablk, &alen, &isCow);
    }

    if (code == 0)
	*offp = (isCow) ? -1 : o;

    return vnm_MapCommonError(code, DEBUG_THIS_FILE);
}

/*
 * extend a fragmented file's allocation enough to hold len bytes.
 *
 * XXX This is a poorly designed interface, in that it tries to do
 * too much.  Instead of copying fragments itself, it should ask for
 * help from its caller.  The current state of affairs means that we
 * cannot let CopyBlocks handle fragmented COW files, and instead
 * rely on CreateBlocks to copy them as a side-effect.
 */

long
efs_ExtendFragAllocation(
  struct evnode *evp,
  elbb_tranRec_t trans,
  long len,
  epix_fragDescr_t *frags)
{
    epi_anode_t ap = EVTOA(evp);
    int lgFsize = ap->device->logFragSize;
    u_int nfrags = NEXT_FRAG(len, ap) >> lgFsize;
    long code;
    TMPBUF_DECL

    if (!epix_IsBlockWritable(frags->block))
	code = EPI_E_CANNOTEXTEND;
    else {
	afsl_Assert(nfrags > frags->length);
	code = epib_AllocateFrags(trans, ap, EPIB_EXTEND_FRAGS, nfrags, frags);
    }

    if (code == 0) {
	/* we managed to extend in place */
	EFS_HOLD_CONTENTS(evp, 0, len);
	code = epix_InsertFragments(trans, ap, frags);
	afsl_MBZ(code);
	EFS_RELEASE_CONTENTS(evp);
    } else if (code != EPI_E_CANNOTEXTEND)
	return code;
    else {
	epix_fragDescr_t ofrags = *frags;
	daddr_t dblk;

	code = epib_AllocateFrags(trans, ap, EPIB_MOVE_FRAGS, nfrags, frags);
	if (code != 0)
	    return (code);
	/*
	 * we couldn't just extend the current fragment block so we allocated
	 * a new set of fragments. We now need to copy the data off of the
	 * old block to the new block and free the old block
	 */
	/*first get the disk sector that the old fragment allocation started*/
	dblk = EFS_FRAG_DADDR(ap->device, &ofrags);

	/*
	 * We can't tell at this level if the file's length reflects the old
	 * or the new allocation. However, since we are not allowed to ever
	 * have a page past the current EOF we only read from the disk into
	 * pages the minimum of the current allocation and the current length
	 */
	len = MIN(epia_GetLen32(ap), EFS_FRAG_LENGTH(ap->device, &ofrags));
	code = EFS_GETCONTENTS(evp, 0, len, dblk);
	if (code)
	     return (code);
	/* release frags only if not cow */
	if (epix_IsBlockWritable(ofrags.block)) {
	    code = epib_FreeFrags(trans, ap, 0, &ofrags);
	    afsl_MBZ(code);
	}
	code = epix_InsertFragments(trans, ap, frags);
	afsl_MBZ(code);
	EFS_PUTCONTENTS(evp, EFS_FRAG_DADDR(ap->device, frags),
			EFS_FRAG_LENGTH(ap->device, frags), 0);
    }
    afsl_Assert(nfrags == frags->length);
    return (0);
}

/*
 * efs_Zerobytes--
 *
 * zeroes bytes on the disk without using the VM system.  We use this
 * when we allocate new disk blocks but want to avoid creating pages.
 * On SunOS, we don't currently use this, although we might if when
 * we start using this I/O path for volume operations.  We use it on
 * AIX and for user-level tests, since pages aren't available in those
 * environments.
 *
 * input params:
 *	evp is the evnode that the block belongs to
 *	dblk is the disk block address of the first disk block in the
 *        filesystem fragment/block.
 *	dblkoff is the byte offset in the filesystem frag/block where we
 *        start zeroing
 *	len indicates the number of bytes to be zeroed.
 *
 * This code assumes that before epia_Strategy(bp) schedules any
 * disk IO it will search for all the blocks that it is about to write
 * on the pending blocks list (the one that is used to record undoZeroes
 * and for each that it finds there it needs to check and set the
 * INTRANSIT bit (if it's set it needs to wait until it is unset again).
 * If the above is not true we can't set B_ASYNC.
 */
void
efs_ZeroBytes(
  struct evnode *evp,
  daddr_t dblk,
  long dblkoff,
  long len)
{
    int fsbsize = EVTOA(evp)->device->blkSize;
    char *mem = osi_AllocPinned_r(fsbsize);

    afsl_Assert(len >= 0);
    if (len == 0)
	return;
    afsl_Assert(len <= fsbsize);
    icl_Trace3(efs_iclSetalloc, EFS_TRACE_ZEROBYTE, ICL_TYPE_POINTER, (evp),
	       ICL_TYPE_LONG, len,
	       ICL_TYPE_LONG, dblk+ btodb(dblkoff));
    bzero(mem, len);
    (void) efs_PutContentsNoVM(evp, len, dblk + btodb(dblkoff), mem);
    osi_FreePinned_r(mem, fsbsize);
}

long efs_MakeABlock(
  struct evnode *evp,
  long off,
  daddr_t *dblkP)
{
    epi_anode_t ap = EVTOA(evp);
    long alen = ap->device->blkSize;
    elbb_tranRec_t tran;
    long code;
    int remainingInTrans = MAX_BLKS_ALLOC_IN_ONE_TRAN;

    afsl_Assert(lock_WriteLocked(&evp->vd_file.lock));
    afsl_Assert(lock_Check(&evp->vd_vm.lock) != 0);
    code = epia_StartTran("MakeABlock", ap, &tran);
    afsl_MBZ(code);
    code = CreateBlocks(evp, tran, &off, &alen, dblkP, &remainingInTrans, 0);
    (void) epia_EndTran(ap, tran);
    return vnm_MapCommonError(code, DEBUG_THIS_FILE);
}

/*
 * The following routine creates writable frags for a fragmented file
 * that has existing COW frags. It does not copy the data; the caller
 * is supposed to get that done
 */
long efs_SplitCOWFrags(
  struct evnode *evp,
  daddr_t *dblkP)
{
    epi_anode_t ap = EVTOA(evp);
    long nfrags;
    elbb_tranRec_t tran;
    long code;
    epix_fragDescr_t frags;
    epia_quota_t delta;

    afsl_Assert(lock_WriteLocked(&evp->vd_file.lock));
    afsl_Assert(lock_Check(&evp->vd_vm.lock) != 0);

    /* find out how many frags we had */
    code = epix_GetFragInfo(ap, &frags);
    afsl_MBZ(code);
    afsl_Assert(!epix_IsBlockWritable(frags.block) && (frags.length > 0));

    nfrags = frags.length;
    delta.a =  nfrags;
    delta.v = 0;

    code = epia_StartTran("SplitCOWFrags", ap, &tran);
    afsl_MBZ(code);

    code = epia_UseQuota(ap, tran, delta);
    if (code == 0) {
	code = epib_AllocateFrags(tran, ap, EPIB_MOVE_FRAGS, nfrags, &frags);
	if (code != 0) {
	    long mbz;

	    /* give back the quota */
	    delta.a = -nfrags;
	    delta.v = 0;
	    mbz = epia_UseQuota(ap, tran, delta);
	    afsl_MBZ(mbz);
	} else {
	    code = epix_InsertFragments(tran, ap, &frags);
	    afsl_MBZ(code);
	    *dblkP = EFS_FRAG_DADDR(ap->device, &frags);
	}
    }
    (void) epia_EndTran(ap, tran);
    return vnm_MapCommonError(code, DEBUG_THIS_FILE);
}
