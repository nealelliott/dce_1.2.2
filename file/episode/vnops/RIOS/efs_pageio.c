/*
 * Copyright (C) 1995, 1990 Transarc Corporation
 * All rights reserved.
*/

#include <dcedfs/param.h>

#include <dcedfs/sysincludes.h>
#include <dcedfs/osi_uio.h>
#include <dcedfs/osi_user.h>
#include <dcedfs/osi_buf.h>
#include <dcedfs/osi_intr.h>
#include <sys/vmuser.h>
#include <sys/m_except.h>
#include <sys/adspace.h>
#include <dcedfs/stds.h>
#include <dcedfs/xvfs_vnode.h>
#include <dcedfs/aggr.h>
#include <dcedfs/volume.h>
#include <dcedfs/debug.h>
#include <dcedfs/queue.h>
#include <dcedfs/lock.h>

#include <dcedfs/episode/file.h>

#include "efs_evnode.h"
#include "efs_misc.h"
#include "efs_tmpcopy.h"
#include "efs_newalloc.h"

RCSID ("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/vnops/RIOS/RCS/efs_pageio.c,v 1.64 1995/09/26 15:33:13 blake Exp $")

#define DEBUG_THIS_FILE EFS_DEBUG_PAGEIO

/*
 * Flags for efs_strategy.
 */
#define BP_PROTECT 0x01

/*
 * Disk alignment check.  A single disk I/O cannot cross a 128K boundary.
 */
#define DISK_TRACKALIGN         0x20000         /* 128 K */
#define EFS_IOBOUNDARY(addr) (((addr) & (DISK_TRACKALIGN - 1)) == 0)

/*
 * Boundary to trigger write behinds.
 */
#define EFS_WTRIGGER 0x10000			/* 64 K */


/*
 * Routines called by the episode VM handling daemons with a list of bufs.
 * Their job is to do all the work necessary to call epia_Strategy.
 *
 * General Description
 *
 *	In AIX we have 3 daemons servicing pagefaults:
 *
 *		1. The pageout daemon. This process simply
 *	schedules the I/O to the disk. It asserts that all the pages
 *	that it's asked to write have backing disk space. It only
 *	readlocks the vnode's fileLock
 *
 *		2. The pagein daemon. This process reads data into
 *	the pages from disk. If the page that it read does not have
 *	allocated writable disk blocks it returns the page write
 *	protected even if it was called with PF_STORE set. It locks
 *	the vnode's vmLock and readlocks its fileLock. It also increments
 *	the vnode's pagefault counter. If the pagein specified offset+len
 *	past EOF and it was called on behalf of shmat() it again returns
 *	a writeprotected page
 *
 *		3. The page-unprotect daemon. This process gets the
 *	pagefaults for the pages that the pagein daemon filled in but
 *	returned write protected. It does disk allocation, length
 *	setting and promotion if appropriate and returns. It write locks
 *	vnode's vd_tlock, vmLock and fileLock.
 *
 *	Notes: The third daemon is only used for mapped files. In the
 *	efs_vmwrite path we do all the length setting, promotion, and
 *	disk allocation, before calling vm_move, and we call vm_move while
 *	holding the vd_tlock because we know that the 3rd daemon won't be
 *	needed for our vm_move to complete.
 *
 *	Since AIX does not use pages but bufs to communicate with us and
 *	bufs don't have an offset field it puts the offset in logical
 *	disk block units in buf->b_blkno. Before we call epia_Strategy we
 *	must overwrite this field with the actual disk address of the block
 *	that we want the transfer to start at.
 *
 *
 */

/*
 * efs_vmrdwr -- AIX 3 I/O to/from a file
 */

int efs_vmrdwr(vp, uiop, rw, ioflag, credp)
    struct vnode *vp;
    struct uio *uiop;
    enum uio_rw rw;
    int ioflag;
    osi_cred_t *credp;
{
    long code;
    long fileSize;
    long xfrSize, xfrOffset;
    long sid;
    struct evnode *evp = VTOEV(vp);
    off_t headBegin, tailBegin, tailEnd, transferEnd;
    long headLen, tailLen;

    /* special case easy transfer; apparently a lot are done */
    if ((xfrSize=uiop->osi_uio_resid) == 0) return 0;

    /* Check for stale file */
    code = EV_CHECKSTALE(evp);
    if (code != 0)
	return code;

    if (rw == UIO_READ) {
	EFS_VMLEN_RLOCK(evp);
	icl_Trace3(efs_iclSet2, EFS_TRACE_VMREAD, ICL_TYPE_POINTER, (evp),
	       ICL_TYPE_LONG, (long) uiop->uio_offset,
	       ICL_TYPE_LONG, (long) uiop->uio_resid);
	tlock_ObtainRead(&evp->vd_tlock);
    } else {
	EFS_VMLEN_WLOCK(evp);
	icl_Trace3(efs_iclSet2, EFS_TRACE_VMWRITE, ICL_TYPE_POINTER, (evp),
	       ICL_TYPE_LONG, (long) uiop->uio_offset,
	       ICL_TYPE_LONG, (long) uiop->uio_resid);
	tlock_ObtainWriteNT(&evp->vd_tlock);
    }

    code = efs_getlength(vp, &fileSize, credp);
    if (code)
	goto done;

    /* handle IO_APPEND mode */
    if (rw == UIO_WRITE && (ioflag & IO_APPEND))
	uiop->osi_uio_offset = fileSize;

    /* compute xfrOffset now, and do some checks */
    xfrOffset = uiop->osi_uio_offset;
    if (xfrOffset < 0 || xfrOffset + xfrSize < 0) {
	code = EINVAL;
	goto done;
    }

    /* check for "file too big" error, which should really be done above us */
    if (rw == UIO_WRITE && xfrOffset + xfrSize > osi_getufilelimit()) {
	code = EFBIG;
	goto done;
    }

    /* copy VM structures out of our vnode */
    code = efs_getvmh (vp);
    if (code)
	goto done;
    sid = vp->v_gnode->gn_seg;

    if (rw == UIO_READ) {
	/* Reading */

	/* don't read past EOF */
	if (xfrSize+xfrOffset > fileSize)
	    xfrSize = fileSize - xfrOffset;

	/* set atime */
	vnm_SetAtime (VTOEV (vp));
	lock_ReleaseRead(&evp->vd_tlock.lock);
	code = vm_move(sid, xfrOffset, xfrSize, rw, uiop);
	/*
	 * Were done, so get out now.
	 */
	EFS_VMLEN_RUNLOCK(evp);
	return code;
    } else {
	/* Writing */
	long w;
	daddr_t d;
 	epi_anode_t ap;

	transferEnd = xfrOffset + xfrSize;

	if (transferEnd > fileSize) { /* extending */
	    code = vnm_Truncate(evp, transferEnd, TRUNCATE_USEVM, credp);
	    if (code)
		goto done;
	}

 	ap = EVTOA(evp);
	if (!EPIA_INLINE(ap)) {
	    /*
	     * Since vm_move() is not guaranteed to call efs_pagein we'll
	     * make sure that if we are doing a write there is backing disk
	     * space. Since MakeWritableBlocks does not use VM on AIX we'll
	     * have to make sure that if we need to split a COW block and
	     * there is any data in VM in this block that we will not modify
	     * with the upcoming vm_move() that data is copied on disk.
	     */
	    lock_ObtainWrite(&evp->vd_vm.lock);
	    lock_ObtainWrite(&evp->vd_file.lock);

	    if (EPIA_BLOCKED(ap)) {
		long blockSize = ap->device->blkSize;
		headLen = tailLen = 0;
		if (xfrOffset & (blockSize - 1)) {
		    headBegin = xfrOffset & ~(blockSize - 1);
		    headLen = ((xfrOffset - headBegin) <= PAGESIZE) ?
			PAGESIZE : blockSize;
		}
		if ((transferEnd & (blockSize - 1)) &&
		    (transferEnd < fileSize)) {
		    if (NEXT_BLOCK(transferEnd, ap) !=
			NEXT_BLOCK(xfrOffset, ap)) {
			tailBegin = transferEnd & ~(blockSize - 1);
			tailEnd = NEXT_BLOCK(transferEnd, ap);
			tailLen = ((tailEnd - transferEnd) <= PAGESIZE) ?
			    PAGESIZE : blockSize;
		    } else if (headLen) {
			headLen = blockSize; /* must cover whole block */
		    }
		}
	    } else {	/* fragmented file */
		tailLen = headBegin = 0;
		headLen = NEXT_BOUNDARY(fileSize, ap->device->fragSize);
		if ((xfrOffset == 0) && (transferEnd >= fileSize)) {
		    efs_CreateDirtyZeroPages(evp, 0, headLen);
		    headLen = 0;
		}
	    }
	    /*
	     * the only blocks that are of interest are the first and
	     * the last since all the ones in the middle will be completely
	     * overwritten
	     */
	    if (headLen != 0 || tailLen != 0) {
		int old;
		if (headLen) {
		    code = efs_FindWritableBlocks(evp, xfrOffset, &d, &w);
		    if (code)
			goto done1;
		    if ((w != 0) || (d == EFS_HOLE))
			headLen = 0;
		}
		if (tailLen) {
		    code = efs_FindWritableBlocks(evp, transferEnd, &d, &w);
		    if (code)
			goto done1;
		    if ((w != 0) || (d == EFS_HOLE))
			tailLen = 0;
		}
		if ((headLen > 0) || (tailLen > 0)) {
		    struct uio headUio, tailUio;
		    struct iovec headIov, tailIov;
		    char *headBuf, *tailBuf;

		    lock_ReleaseWrite(&evp->vd_file.lock);
		    if (headLen > 0) {
			headBuf = osi_AllocPinned(headLen);
			epia_InitUio(&headUio, &headIov, headBegin,
				     headLen , headBuf);
		    }
		    if (tailLen > 0) {
			tailBuf = osi_AllocPinned(tailLen);
			epia_InitUio(&tailUio, &tailIov, tailBegin,
				     tailLen, tailBuf);
		    }
		    do {
			old = evp->pagefaults;
			lock_ReleaseWrite(&evp->vd_vm.lock);
			if (headLen) {
			    code = vm_protectp(sid, headBegin >> PGSHIFT,
					howmany(headLen, PAGESIZE), RDONLY);
			    if (code == 0) {
				code = vm_move(sid, headBegin, headLen,
							UIO_READ, &headUio);
			    }
			}
			if (tailLen && (code == 0)) {
			    code = vm_protectp(sid, tailBegin >> PGSHIFT,
					howmany(tailLen, PAGESIZE), RDONLY);
			    if (code == 0) {
				code = vm_move(sid, tailBegin, tailLen,
							UIO_READ, &tailUio);
			    }
			}
			lock_ObtainWrite(&evp->vd_vm.lock);
		    } while (old != evp->pagefaults);
		    lock_ObtainWrite(&evp->vd_file.lock);
		    if (headLen) {
			if (code == 0) {
			    if (EPIA_BLOCKED(ap)) {
				code = efs_MakeABlock(evp, headBegin, &d);
			    } else {
				code = efs_SplitCOWFrags(evp, &d);
			    }
			    if (code == 0)
				efs_PutContentsNoVM(evp, headLen, d, headBuf);
			}
			osi_FreePinned(headBuf, headLen);
		    }
		    if (tailLen) {
			if (code == 0) {
			    osi_assert(EPIA_BLOCKED(ap));
			    code = efs_MakeABlock(evp, tailBegin, &d);
			    if (code == 0)
				efs_PutContentsNoVM(evp, tailLen, d, tailBuf);
			}
			osi_FreePinned(tailBuf, tailLen);
		    }
		}
		if (code)
		    goto done1;
	    }
	    code = efs_MakeWritableBlocks(evp,xfrOffset, xfrSize,
					  MWB_USE_VM | MWB_ZEROFILL |
					  MWB_OVERWRITE);
	  done1:
	    lock_ReleaseWrite (&evp->vd_vm.lock);
	    lock_ReleaseWrite(&evp->vd_file.lock);
	    if (code) {
		goto done;
	    }
	} else {	/* inline file */
	    if ((xfrOffset == 0) && (transferEnd >= fileSize)) {
		lock_ObtainWrite(&evp->vd_vm.lock);
		efs_CreateDirtyZeroPages(evp, 0, transferEnd);
		lock_ReleaseWrite (&evp->vd_vm.lock);
	    }
	}
	/*
	 * since we have to drop the vd_vm.lock if a reader sneaks in before
	 * our vm_move goes through they might get to read 0s instead of
	 * either the old or the new data. Given the hoops that
	 * we would have to jump through to eliminate this possibility
	 * and the limited value of that achievement (there would still
	 * be no guarantee that the concurrent reader gets only new
	 * data or only old data) we are better off not even trying.
	 */

	vm_protectp (sid, xfrOffset >> PGSHIFT,
		(NEXT_PAGE(transferEnd) >> PGSHIFT) - (xfrOffset >> PGSHIFT),
		FILEKEY);
	tlock_ReleaseWriteNT(&evp->vd_tlock);
	code = vm_move(sid, xfrOffset, xfrSize, rw, uiop);
	tlock_ObtainWriteNT(&evp->vd_tlock);
	if (code)
	    goto done;

	/*
	 * If the length of the file is not a whole number of pages, we
	 * write-protect the last page.  Then when a user with the file mapped
	 * stores into that page, our pagein gets called notified.  When it
	 * does, we extend the file length to cover the whole page.  Corny,
	 * but that's how we extend a mapped file in AIX.
	 */

	code = efs_getlength(vp, &fileSize, credp);
	if (code)
	    goto done;
	vnm_SetMCtime (VTOEV (vp), MARK_MC);
        /* Clear SUID and SGID bits if necessary */
        if (osi_GetUID (credp) != 0 &&
            (epif_GetMode (ap) & (VSUID | VSGID)))
            vnm_ClearSUID (VTOEV (vp));
	if (fileSize & (PAGESIZE-1))
	    vm_protectp(sid, fileSize >> PGSHIFT, 1, RDONLY);

	/*
	 * If requested, fsync the file after every write, else consider
	 * a write behind.
	 */
	if (ioflag & FSYNC) {
	    efs_fsync (vp, credp);
	} else {
	    if (transferEnd && ((transferEnd & (EFS_WTRIGGER-1)) == 0)) {
		long start, count;

		start = MIN((transferEnd - EFS_WTRIGGER), xfrOffset);
		count = MAX(EFS_WTRIGGER, NEXT_PAGE(transferEnd - xfrOffset));
		vm_writep(sid, start >> PGSHIFT, count >> PGSHIFT);
	    }
	}
    }

  done:
    if (rw == UIO_READ) {
	lock_ReleaseRead(&evp->vd_tlock.lock);
	EFS_VMLEN_RUNLOCK(evp);
    } else {
	tlock_ReleaseWriteNT(&evp->vd_tlock);
	EFS_VMLEN_WUNLOCK(evp);
    }
    return code;
}

/*
 * Map Episode I/O error code onto standard Unix code.
 */
static long
MapErrorCode(long code)
{
    long newcode = code;

    if (code) {
	switch (code) {
	  case EPI_E_VOLUMEFULL:
	    newcode = EDQUOT;
	    break;
	  case EPI_E_AGGREGATEFULL:
	    newcode = ENOSPC;
	    break;
	  case EPI_E_BLOCKTOOBIG:
	    newcode = EFBIG;
	    break;
	  case EIO:
	    newcode = EIO;
	    break;
	  default:
	    newcode = EIO;
	    break;
	}
    }
    return newcode;
}

void
efs_FinishVMIO(struct buf *bp, long code)
{
    if (code != 0)
	code = MapErrorCode(code);

    do {
	struct buf *nextbp = (struct buf *) bp->av_forw;
	bp->av_forw = NULL;
	if (code) {
	    bp->b_flags |= B_ERROR;
	    bp->b_error = code;
	}
	iodone(bp);
	bp = nextbp;
    } while (bp != NULL);
}

#define EFS_MAX_QUEUED_OSI_BUFS 64
static struct buf *efs_pageioBufList = NULL;
static long efs_pageioBufCount = 0;
static struct buf efs_pageioZeroBuf;

/*
 * Perform initialization for functions in this module.
 * The main function is to pin data used durring interrupt level
 * processing.
 */
void
efs_pageioInit()
{
    int code;

    bzero(&efs_pageioZeroBuf, sizeof(efs_pageioZeroBuf));
    code = pin(&efs_pageioBufList, sizeof(efs_pageioBufList));
    afsl_MBZ(code);
    code = pin(&efs_pageioBufCount, sizeof(efs_pageioBufCount));
    afsl_MBZ(code);
    code = pin(&efs_pageioZeroBuf, sizeof(efs_pageioZeroBuf));
    afsl_MBZ(code);
}

/*
 * Handle releasing buf structs at interrupt level.  osi_ReleaseBuf()
 * cannot handle this so put the buffer on a private queue.  The
 * buffer can be reused later, or eventually returned back to the
 * osi layer at process level.
 */
void
efs_PageioReleaseBuf(struct buf *bp)
{
    bp->av_forw = efs_pageioBufList;
    efs_pageioBufList = bp;
    efs_pageioBufCount++;
}

/*
 * Get a buf structure checking the private q first.  If there
 * are none on the private q, then get one from the osi layer via
 * osi_GetBuf().
 */
struct buf *
efs_PageioGetBuf()
{
    int opri;
    struct buf *bp = 0;

    osi_intr_exclude(&osi_iodoneLock, opri);

    /*
     * First see if there are bufs on our private list
     * from previous IO requests before going to osi for one.
     */
    if (efs_pageioBufList != NULL) {
	bp = efs_pageioBufList;
	efs_pageioBufList = bp->av_forw;
	efs_pageioBufCount--;

	/*
	 * Don't be greedy with these buffers.
	 * Give some back to osi if we have a bunch.
	 */
	while (efs_pageioBufCount > EFS_MAX_QUEUED_OSI_BUFS) {
	    struct buf *rbuf;

	    afsl_Assert(efs_pageioBufList != NULL);
	    rbuf = efs_pageioBufList;
	    efs_pageioBufList = rbuf->av_forw;
	    efs_pageioBufCount--;
	    osi_intr_allow(&osi_iodoneLock, opri);
	    osi_ReleaseBuf(rbuf);
	    osi_intr_exclude(&osi_iodoneLock, opri);
	}
    }
    osi_intr_allow(&osi_iodoneLock, opri);

    if (bp) {
	*bp = efs_pageioZeroBuf;
    } else {
	bp = osi_GetBuf();
    }
    return(bp);
}

/*
 * Called by the anode layer (epia_Iodone) when there is
 * a chain of VM bufs from an async request to be processed.
 * This is called at splbio().
 */
void
efs_FinishDiskIO(struct buf *bp, long code)
{
    /*
     * Give the VM buffers back to VM.
     */
    afsl_Assert(bp->b_flags & B_DONE);
    afsl_Assert(bp->b_forw);
    efs_FinishVMIO(bp->b_forw, code);

    /* Now we cannot give bp back via osi_ReleaseBuf() since
     * we are at interrupt level.  So place it on
     * a private queue which is managed at interrupt level.
     * When efs_strategy() needs a buf struct, it will check the
     * private queue first, before getting more buffers from osi.
     */
    efs_PageioReleaseBuf(bp);
}

/*
 * Init a buffer structure for an IO request to be sent to
 * epia_Strategy, using the VMM buffer to setup some of the
 * fields. epia_Strategy will to the final setup.
 */
static void InitIOBuffer(
  int bflags,
  struct buf *inbp,
  struct buf *iobp,
  long blkno,
  long len)
{
    iobp->b_flags = (bflags & (B_READ|B_WRITE));
    iobp->b_bcount = len;
    iobp->b_blkno = blkno;
    iobp->b_un.b_addr = inbp->b_un.b_addr;
    iobp->b_xmemd = inbp->b_xmemd;
    iobp->b_vp = BPTOVP(inbp);
    /*
     * If this is an async request, setup up info so that
     * iodone processing will occur on VM bufs at IO completion.
     */
    if (bflags & B_ASYNC) {
	iobp->b_forw = inbp;	/* The chain of VMM buf structs */
	iobp->b_back = (struct buf *)efs_FinishDiskIO;
    }
}

/*
 * Do I/O on a chain of buffers, breaking it into pieces that
 * correspond to contiguous regions on disk.  The buffer chain
 * is assumed to be logically contiguous.
 */
int efs_strategy(struct buf *bp, unsigned long stratFlags)
{
    struct evnode *evp;
    epi_anode_t ap;
    off_t off;
    long wlen, len;
    daddr_t dblk;
    struct buf *iobp, *tbp, *groupbp, *nextbp;
    char *baddr;
    long filelen;
    long xfersize;
    int howManyRO, firstRO;
    int ioflags;
    long code;

    evp= VTOEV(BPTOVP(bp));
    ap = EVTOA(evp);
    off = dbtob(bp->b_blkno);
    code = 0;

    filelen = epia_GetLen32(ap);
    while (bp) {
	ioflags = 0;
	if (!EPIA_INLINE(ap) || off >= filelen) {
	    firstRO = -1;

	    if (bp->b_flags & B_READ) {
		dblk = EFS_HOLE;
		len = 0;
		if (off < filelen) {
		    if (code = efs_FindBlocks(evp, off, &dblk, &wlen, &len))
			break;
		}
		if (dblk == EFS_HOLE) {
		    long startbuf = dbtob(bp->b_blkno);
		    long pageoffset = off - startbuf;
		    vmid_t sid = EVTOV(evp)->v_gnode->gn_seg;

		    if (len <= 0) len = bp->b_bcount;
		    len = MIN(len, (bp->b_bcount - pageoffset));

		    /* Zero the page */
		    baddr = vm_att(bp->b_xmemd.subspace_id, bp->b_un.b_addr);
		    bzero(baddr + pageoffset, len);
		    vm_det(baddr);

		    /*
		     * Always protect pages with no backing storage (holes),
		     * so the protection fault handler will get called
		     * to allocate blocks and set the file length.
		     */
		    (void)vm_protectp(sid, startbuf >> PGSHIFT, 1, RDONLY);

		    /* Break the chain of buffers and call iodone */
		    if ((pageoffset + len) == bp->b_bcount) {
			tbp = bp;
			bp = bp->av_forw;
			tbp->av_forw = NULL;
			efs_FinishVMIO(tbp, 0);
		    }
		    off += len;
		    continue;
		} else if (wlen < len) {
		    /*
		     * some of the blocks that we found are COW. Let's
		     * keep a note of that because we'll need to write
		     * protect the pages that are backed by these blocks
		     */
		    firstRO = (off + wlen) << PGSHIFT;
		    howManyRO = howmany((len -wlen), PAGESIZE);
		}
	    } else {
		if (off >= filelen) break;	/* done */
		if (code = efs_FindWritableBlocks(evp, off, &dblk, &len))
		    break;
	    }
	    osi_assert((dblk != EFS_HOLE) && (len > 0));

	    /*
	     * Figure if any whole bufs can be satisfied by len. The
	     * idea is to group as many bufs as possible and process
	     * them in an asynchronous IO.
	     */
	    if (len >= bp->b_bcount) {
		xfersize = 0;
		groupbp = bp;
		while (bp && ((xfersize + bp->b_bcount) <= len)) {
		    xfersize += bp->b_bcount;
		    tbp = bp;	/* save the previous bp */
		    bp = bp->av_forw;
		    if (EFS_IOBOUNDARY(dbtob(dblk) + xfersize)) break;
		}
		tbp->av_forw = NULL; /* sever the chain to end the group */
		tbp = groupbp;
	    } else {
		xfersize = len;
		/*
		 * There may still be hope to do this asynchronously if
		 * it is a write and it would span to end of file.
		 */
		if (!(bp->b_flags & B_READ) && ((off + xfersize) >= filelen)
		    && !(stratFlags & BP_PROTECT)) {
		    groupbp = bp;
		    bp = bp->av_forw;
		    groupbp->av_forw = NULL;
		    tbp = groupbp;
		} else {		/* Must do synchronously */
		    groupbp = NULL;
		    tbp = bp;
		}
	    }
	    /*
	     * Setup the IO flags.  The request can be asynchronous
	     * if bufs have been grouped and the BP_PROTECT flag is
	     * not set. Note that for the async case, the buffers in
	     * in the async request should not be referenced after the
	     * the call to epia_Strategy.
	     */
	    ioflags = (tbp->b_flags & (B_READ|B_WRITE));
	    ioflags |= ((groupbp && !(stratFlags & BP_PROTECT)) ? B_ASYNC : 0);

	    iobp = efs_PageioGetBuf();
	    InitIOBuffer(ioflags, tbp, iobp, dblk, xfersize);
	    code = epia_Strategy(iobp, ap);
	    osi_assert(code == 0);
	    if (!(ioflags & B_ASYNC)) {	/* Must wait for IO to complete */
		epia_BioWait(iobp);
		osi_ReleaseBuf(iobp);
		if (groupbp) {
		    if (stratFlags & BP_PROTECT) {
			vm_protectp(EVTOV(evp)->v_gnode->gn_seg,
				(dbtob(tbp->b_blkno)) >> PGSHIFT, 1, RDONLY);
		    }
		    efs_FinishVMIO(groupbp, 0);
		}
	    }
	    off += xfersize;
	    /*
	     * if there were any readonly blocks write protect
	     * the relevant pages
	     */
	    if (firstRO != -1) {
		vmid_t sid = EVTOV(evp)->v_gnode->gn_seg;
		vm_protectp(sid, firstRO, howManyRO, RDONLY);
	    }
	} else {	/* handle inline data */
	    osi_assert((off == 0) && (bp->av_forw == NULL));
	    ioflags = bp->b_flags & (B_READ|B_WRITE);
	    iobp = efs_PageioGetBuf();
	    InitIOBuffer(ioflags, bp, iobp, 0, filelen);
	    code = epia_Strategy(iobp, ap);
	    osi_assert(code == 0);
	    osi_ReleaseBuf(iobp);

	    if (bp->b_flags & B_READ) {
		baddr = vm_att(bp->b_xmemd.subspace_id, bp->b_un.b_addr);
		bzero(baddr + filelen, bp->b_bcount - filelen);
		vm_det(baddr);
	    }
	    break;
	}
    }		/* end while */

  done:
    /*
     * If any buffers are left, perform iodone processing on them.
     */
    if (bp) {
	if (stratFlags & BP_PROTECT) {
	    vm_protectp(EVTOV(evp)->v_gnode->gn_seg,
		(dbtob(bp->b_blkno)) >> PGSHIFT, 1, RDONLY);
	}
	efs_FinishVMIO(bp, code);
    }
    return code;
}


/*
 * Read a set of pages into memory.
 */

int efs_pagein(struct buf *bp)
{
    long llen, bplen;

    struct evnode *evp = VTOEV(BPTOVP(bp));
    int file_page, buf_page;
    int needSetLength = 0;
    int bufPage, filePage;
    unsigned long stratFlags;
    long code;

    lock_ObtainWrite (&evp->vd_vm.lock);
    lock_ObtainRead (&evp->vd_file.lock);
    evp->vd_vm.valid = 1;
    afsl_Assert(!(evp->vd_vm.noReadable));
    if (bp->b_flags & B_PFSTORE) {
	/*
	 * If we are doing a PF_STORE we have to see if any length setting
	 * will be necessary. Note that we won't do the length setting here
	 * but we will write protect the page so that the 3rd daemon will
	 * get called
	 */

	afsl_AssertTruth(!(bp->b_flags & B_PFPROT));
	/*
	 * get the file's length. We can do this safely since we are holding
	 * the vmLock
	 */

	efs_getlength (EVTOV(evp), &llen, 0);
	bplen = dbtob(bp->b_blkno) + bp->b_bcount;

	bufPage = bplen -1 >> PGSHIFT;
	filePage = (llen == 0 ? 0 : ((llen-1) >> PGSHIFT));
	if (filePage < bufPage) {
	    /*
	     * pages shouldn't be created past EOF except if PFEOF is set
	     */
	    if (bp->b_flags & B_PFEOF) {
		lock_ReleaseWrite(&(evp->vd_vm.lock));
		return EXCEPT_EOF;
	    } else {
		needSetLength = 1;
	    }
	} else if (filePage == bufPage) {
	    if (bplen > llen)
		needSetLength = 1;
	}
    }
    /*
     * Would like to check for pages created past EOF even in the non-PFSTORE
     * case, but AIX does not set the PFEOF flag unless it is also setting
     * PFSTORE, so we're out of luck.
     */
    evp->pagefaults++;
    if (needSetLength || evp->vd_vm.noWritable || evp->vd_vm.readonly) {
	stratFlags = BP_PROTECT;
    } else {
	stratFlags = 0;
	evp->vd_vm.dirty = 1;
    }
    code = efs_strategy(bp, stratFlags);
    lock_ReleaseRead(&evp->vd_file.lock);
    lock_ReleaseWrite(&evp->vd_vm.lock);
    return(code);
}

/*
 * Write out a set of pages.
 */
efs_pageout(struct buf *bp)
{
    afsl_AssertTruth((bp->b_flags & B_PFPROT) == 0);
    (void)efs_strategy(bp, 0);
}

/*
 * This is the part of pagein that ensures that a page that is not
 * write protected has backing disk storage allocated. It also does
 * length adjustments.
 */

long efs_pageunprotect(struct buf *bp)
{
    struct evnode *evp;
    long code;
    off_t off;
    long llen;

    evp = VTOEV(BPTOVP(bp));
    afsl_AssertTruth((bp->b_flags & B_PFSTORE) && (bp->b_flags & B_PFPROT));
    afsl_AssertTruth(bp->av_forw == NULL);
    code = 0;
    off = dbtob(bp->b_blkno);

    tlock_ObtainWriteNT(&evp->vd_tlock);
    lock_ObtainWrite (&evp->vd_vm.lock);

    afsl_AssertTruth (evp->vd_vm.valid);
    evp->vd_vm.dirty = 1;
    evp->pagefaults++;

    lock_ObtainWrite(&evp->vd_file.lock);

    efs_getlength (EVTOV(evp), &llen, 0);

    /*
     * Do we need to do any length updates?  Updates are only done
     * if there are write mappers.
     */
    if (EVTOGP(evp)->gn_mwrcnt) {
	long bplen;

	bplen = off + bp->b_bcount;
	if (bplen > llen) {
	    llen = NEXT_PAGE(bplen);
	    code = efs_Promote(evp, llen);
	}
    }

    if (code == 0)
	code = efs_MakeWritableBlocks(evp, off,
				      MIN(bp->b_bcount, llen - off),
				      MWB_ZEROFILL | MWB_OVERWRITE);

    lock_ReleaseWrite(&evp->vd_file.lock);

    if (code != 0) code = vnm_MapCommonError(code, DEBUG_THIS_FILE);
    lock_ReleaseWrite(&evp->vd_vm.lock);
    tlock_ReleaseWriteNT(&evp->vd_tlock);
    efs_FinishVMIO(bp, code);
    return (code);
}
