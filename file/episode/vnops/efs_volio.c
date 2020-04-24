/*
 * Copyright (C) 1996, 1994 Transarc Corporation
 * All rights reserved.
 */

/*
 * Disk I/O for volume ops
 */
#include <dcedfs/osi.h>
#include <dcedfs/lock.h>
/* XXX FIX ME XXX */
#undef	PRIVATE
#undef	SHARED
#include <dcedfs/osi_intr.h>
#include <dcedfs/osi_buf.h>
#include <dcedfs/osi_device.h>
#include <dcedfs/osi_kvnode.h>		/* VN_RELE */
#include "efs_volio.h"

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/vnops/RCS/efs_volio.c,v 1.17 1996/07/22 19:22:01 berman Exp $")

/*
 * Volume I/O buffers; these have three parts: a struct buf to describe
 * the I/O, a data area pointed to by buf->b_un.b_addr, and a next pointer
 * so that we can maintain a free list.  The buf must be the first member
 * since we cast it to get the address of the whole structure.
 */
struct vol_efsBuf {
    struct buf buf;
    struct vol_efsBuf *next;
};

typedef struct vol_efsBuf vol_efsBuf_t;

static vol_efsBuf_t *freeBuf;
static osi_cv_t volio_CV;
static u_int freeBufWaiters = 0;
static u_int efsBufsInTransit = 0;

static size_t vol_efsBufSize = VOL_EFS_BUFSIZE;
static u_int  vol_efsNumBuffers;

void
vol_efsInitIO(u_int nbuf)
{
    u_int i;
    vol_efsBuf_t *hdr;
    int s;
    long rc;

    osi_cv_init(&volio_CV);

    osi_Pin(&freeBuf, sizeof(freeBuf));
    osi_Pin(&volio_CV, sizeof(volio_CV));
    osi_Pin(&freeBufWaiters, sizeof(freeBufWaiters));
    osi_Pin(&efsBufsInTransit, sizeof(efsBufsInTransit));

    for (i = 0; i != nbuf; i++) {
	hdr = osi_AllocPinned_r(sizeof(vol_efsBuf_t));
	bzero((caddr_t)&hdr->buf, sizeof (struct buf));
	osi_bio_init(&hdr->buf);
	hdr->buf.b_un.b_addr = osi_AllocPinned_r(vol_efsBufSize);
	osi_intr_exclude(&osi_iodoneLock, s);
	hdr->next = freeBuf;
	freeBuf = hdr;
	osi_intr_allow(&osi_iodoneLock, s);
    }
    vol_efsNumBuffers = nbuf;
}

static osi_iodone_t
vol_efsIodone(struct buf *bp)
{
    vol_efsBuf_t *hdr = (vol_efsBuf_t *)bp;

    if (!(bp->b_flags & B_READ)) {
	osi_intr_start(&osi_iodoneLock);
	epia_ClearInTransit(bp);
	osi_intr_end(&osi_iodoneLock);
    }

#ifdef	AFS_AIX_ENV
    if (bp->b_error == ESOFT) {
	bp->b_resid = 0;
	bp->b_error = 0;
	bp->b_flags &= ~B_ERROR;
    }
#endif	/* AFS_AIX_ENV */

    osi_biodone(bp, &volio_CV);

    if (bp->b_flags & B_ASYNC) {
	osi_intr_start(&osi_iodoneLock);
	efsBufsInTransit--;
	hdr->next = freeBuf;
	freeBuf = hdr;
	if (freeBufWaiters != 0)
	    osi_intr_wakeup(&volio_CV);
	osi_intr_end(&osi_iodoneLock);
    }

    osi_iodone_return(0);
}

struct buf *
vol_efsGetBuf(
  struct evnode *evp,
  int flags,
  size_t len,
  daddr_t dblk)
{
    struct buf *bp;
    async_device_t *aggr = EVTOA(evp)->device;
    dev_t device = osi_makedev(aggr->majdev, aggr->mindev);
    vol_efsBuf_t *hdr;
    caddr_t data;
    int s;

    afsl_Assert(len <= vol_efsBufSize);

    osi_intr_exclude(&osi_iodoneLock, s);
    freeBufWaiters++;
    while (freeBuf == NULL)
	osi_intr_wait(&volio_CV, &osi_iodoneLock);
    freeBufWaiters--;
    hdr = freeBuf;
    freeBuf = hdr->next;
    osi_intr_allow(&osi_iodoneLock, s);

    bp = &hdr->buf;
    data = bp->b_un.b_addr;

    /*
     * Clean up from previous async I/O.  We do the VN_RELE here
     * rather than in iodone to avoid an interrupt-time call to
     * VOP_INACTIVE.
     */
    if (bp->b_flags & B_ASYNC)
	VN_RELE(bp->b_vp);

    bzero((caddr_t)bp, sizeof (*bp));
    osi_bio_init(bp);
    osi_BufInit(bp, flags, data, len,
		device, dblk, EVTOV(evp), vol_efsIodone);

    return bp;
}

int
vol_efsBioWait(struct buf *bp)
{
    int s, code;

    /*
     * We don't expect anyone to wait explicitly for an async I/O.
     */
    afsl_Assert(!(bp->b_flags & B_ASYNC));
    osi_intr_exclude(&osi_iodoneLock, s);
    while (!(bp->b_flags & B_DONE))
	osi_bio_wait(bp, &volio_CV, &osi_iodoneLock);

    osi_intr_allow(&osi_iodoneLock, s);

    code = geterror(bp);
    if (code != 0)
	epia_BufError(bp);

    return code;
}

void
vol_efsReleaseBuf(struct buf *bp)
{
    vol_efsBuf_t *hdr;
    int s;

    hdr = (vol_efsBuf_t *)bp;
    hdr->buf.b_flags &= ~B_ASYNC;

    osi_intr_exclude(&osi_iodoneLock, s);
    hdr->next = freeBuf;
    freeBuf = hdr;
    if (freeBufWaiters != 0)
	osi_cv_broadcast(&volio_CV);
    osi_intr_allow(&osi_iodoneLock, s);
}

static int
StartInlineIO(struct buf *bp, epi_anode_t ap)
{
    u_long length = bp->b_bcount;
    elbb_tranRec_t tranId;
    long code;

    afsl_Assert(length <= epix_GetMaxInlineData(ap));
    if (bp->b_flags & B_READ)
	code = epix_ReadInline(ap, 0, length, bp->b_un.b_addr);
    else {
	code = epia_StartTran("StartInlineIO", ap, &tranId);
	afsl_MBZ(code);
	code = epix_WriteInline(tranId, ap, 0, length, bp->b_un.b_addr);
	if ((bp->b_flags & B_ASYNC) == 0 && code == 0)
	    (void) epix_FsyncForce(ap);
	(void) epia_EndTran(ap, tranId);
    }
    osi_biodone(bp, &volio_CV);
    if (bp->b_flags & B_ASYNC)
	vol_efsReleaseBuf(bp);

    return (code);
}
/*
 * Start disk I/O; a stripped-down version of epia_Strategy.
 */
int
vol_efsStartIO(struct buf *bp)
{
    struct vnode *vp = bp->b_vp;
    epi_anode_t ap = EVTOA(VTOEV(vp));
    u_long flags = epia_GetFlags(ap);
    long code;
    int s;

    /*
     * Handle in-line anodes specially (no disk I/O)
     */
    if (flags & EPIA_CFLAGS_INLINE)
	return StartInlineIO(bp, ap);

    osi_assert(bp->b_blkno != EFS_HOLE);
    bp->b_bcount = dbroundup(bp->b_bcount);

    /*
     * Update kernel resource usage statistics.
     */
    if (bp->b_flags & B_READ)
	osi_inc_ru_inblock(1);
    else
	osi_inc_ru_oublock(1);

    /*
     * Hold vnode if I/O is async since caller may rele it.
     */
    if (bp->b_flags & B_ASYNC)
	FAST_VN_HOLD(VTOEV(vp));

    /*
     * Mark these disk blocks in-transit if this is a write,
     * first waiting if any of them is already in transit.
     */
    epia_MarkInTransit(bp);

    /*
     * Keep count of async I/O's so that we can wait for them
     * all to finish in vol_efsWaitAll.
     */
    osi_intr_exclude(&osi_iodoneLock, s);
    if (bp->b_flags & B_ASYNC)
	efsBufsInTransit++;
    osi_intr_allow(&osi_iodoneLock, s);

    /*
     * Start the I/O.
     */
    osi_strategy(bp);
    return 0;
}

/*
 * Wait for all asynchronous volume I/O to complete.
 *
 * XXX We should maintain per-volume information so that we can wait
 * for a single volume instead of all of them.
 *
 */
void
vol_efsWaitAll(void)
{
    int s;
    vol_efsBuf_t *hdr;
    struct vnode **vnpool;
    long vnpool_cnt;
    long code;

    vnpool = osi_AllocPinned_r(vol_efsNumBuffers * sizeof(struct vnode *));

    /*
     * On AIX we cannot afford to take page fault while releasing vnodes
     * when the priority level has been raised, the vnode associated with
     * a buffer whose I/O has been completed is put into a private queue.
     * When the priority level has been lowered, the vnodes in this
     * queue are released.
     */

#ifndef	AFS_SUNOS5_ENV
    osi_RestorePreemption(0);
#endif
    osi_intr_exclude(&osi_iodoneLock, s);
    freeBufWaiters++;
    while (efsBufsInTransit != 0)
	osi_intr_wait(&volio_CV, &osi_iodoneLock);

    freeBufWaiters--;

    vnpool_cnt = 0;
    for (hdr = freeBuf; hdr != NULL; hdr = hdr->next) {
	if (hdr->buf.b_flags & B_ASYNC) {
	    vnpool[vnpool_cnt++] = hdr->buf.b_vp;
	    hdr->buf.b_flags &= ~B_ASYNC;
	}
    }
    osi_intr_allow(&osi_iodoneLock, s);
#ifndef	AFS_SUNOS5_ENV
    (void) osi_PreemptionOff();
#endif

    /*
     * Now release any held vnodes.
     */
    afsl_Assert(vnpool_cnt <= vol_efsNumBuffers);
    while (--vnpool_cnt >= 0) {
	VN_RELE(vnpool[vnpool_cnt]);
    }

    osi_FreePinned_r(vnpool, vol_efsNumBuffers * sizeof(struct vnode *));
}
