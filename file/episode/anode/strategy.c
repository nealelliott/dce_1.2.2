/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * strategy.c -- the low-level interface for reading and writing pages
 *	to disk.   Once upon a time, this was a big, complicated module
 *	because we did lazy disk block allocation.  Now we do all our
 *	allocation ahead of time at the VOP_WRITE/VOP_GETPAGE level, so
 *	there is comparatively little to do here.
 *
 *	Our main responsibilities are scheduling disk I/O and handling
 *	post-I/O interrupt-time clean-up (iodone), including updating
 *	the database of pending undo-zero transactions.
 */

/* Copyright (C) 1996, 1991 Transarc Corporation - All rights reserved */

#include <strategy.h>

#include <dcedfs/episode/logbuf.h>
#include <dcedfs/osi_buf.h>
#include <dcedfs/osi_device.h>
#include <quota.h>

#if !defined(KERNEL)
#include <pthread.h>
#endif /* !KERNEL */

#ifdef AFS_AIX_ENV
#include <sys/sleep.h>
#endif

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/anode/RCS/strategy.c,v 4.341 1996/03/04 19:00:52 ota Exp $")

/*
 * Data structure to keep track of in-transit disk blocks
 */
struct epia_intransit {
    struct epia_intransit *next;
    daddr_t dblk;		/* The disk block */
    int waiters;		/* waiters count */
    int flags;			/* In-transit */
    osi_cv_t condvar;		/* Condition var. for synchronization */
};

#define EPIA_INTRANS_BUSY	0x1	/* Block is in transit */

typedef struct epia_intransit epia_intransit_t;

#define EPIA_INTRANS_NHASH	256
#define EPIA_INTRANS_HASH(dblk)	((u_long)(dblk) & (EPIA_INTRANS_NHASH - 1))

#define EPIA_INTRANS_NUM_NEW_FREE_ELEM  32
#define EPIA_INTRANS_MAX_FREELIST_SIZE  512

static epia_intransit_t *intrans_freelist;
static epia_intransit_t *intrans_hashTab[EPIA_INTRANS_NHASH];

/* Current intransit waiters count */
static int epia_intransit_cur_waiters=0;
/* Cumulative intransit waiters count */
static int epia_intransit_cum_waiters=0;

static osi_cv_t strat_wait;

int epia_intransit_log = 0;

/* XXX */
struct epia_logrec {
    int op;
    int dblk;
};

#define CREATEREC	1
#define FREEREC		2
#define WR_LOOKREC	3
#define RD_LOOKREC	4
#define MARKBUSY	5
#define CLEARBUSY	6
#define ERR_CLEARBUSY	7
#define WR_MARKWAIT	8
#define RD_MARKWAIT	9
#define WAKEWAITERS	10
#define ERR_WAKEWAITERS 11
#define WR_WAITOVER	12
#define RD_WAITOVER	13

char *symOp[] = {
    "dummy",
    "Create",
    "Free",
    "Writelookup",
    "Readlookup",
    "MarkBusy",
    "ClearBusy",
    "ErrorClearBusy",
    "WriteMarkWait",
    "ReadMarkWait",
    "WakeWaiters",
    "ErrorWakeWaiters",
    "WriteWaitOver",
    "ReadWaitOver"
};

#define RECTABSIZE 4096

static struct epia_logrec recTab[RECTABSIZE];
int recIndex = 0;
int wrapped = 0;

#define EPIA_RECORD(type, cnt, diskblk) \
    MACRO_BEGIN \
       if (epia_intransit_log) { \
	   afsl_Assert((((type) >> 16) & 0xFFFF) == 0); \
	   recTab[recIndex].op = \
	       ((((type) & 0xFFFF) << 16) | ((cnt) & 0xFFFF)); \
	   recTab[recIndex++].dblk = (diskblk); \
	   if (recIndex == RECTABSIZE) { \
	       recIndex = 0; \
	       wrapped++; \
	   } \
       } \
     MACRO_END

#ifndef KERNEL
void DumpIOLog(FILE *file)
{
    int i;

    fprintf(file, "\nDumping IO LOG \n\n");

    if (wrapped == 0) {
	fprintf(file, "Buffer not wrapped. %d records\n", recIndex);
	fprintf(file, "Disk blk \tOperation\n");
	for (i = 0; i < recIndex; i++)
	    fprintf(file, "%d \t%s, %d\n", recTab[i].dblk,
		    symOp[(recTab[i].op >> 16) & 0xFFFF],
		    recTab[i].op & 0xFFFF);
    } else {
	fprintf(file, "Buffer wrapped %d times\n", wrapped);
	fprintf(file, "Earliest buffer record serial number is %d\n",
		recIndex);
	fprintf(file, "Disk blk \tOperation\n");
	for (i = recIndex; i < RECTABSIZE; i++)
	    fprintf(file, "%d \t%s, %d\n", recTab[i].dblk,
		    symOp[(recTab[i].op >> 16) & 0xFFFF],
		    recTab[i].op & 0xFFFF);
	for (i = 0; i < recIndex; i++)
	    fprintf(file, "%d \t%s, %d\n", recTab[i].dblk,
		    symOp[(recTab[i].op >> 16) & 0xFFFF],
		    recTab[i].op & 0xFFFF);
    }
}
#endif

/*
 * Find the in-transit record for the specified disk block.
 * If none exists and create is not true, return NULL.
 * If none exists and create is true, and free list has free elements,
 * initialize a new one to return; otherwise return NULL.
 * predPP returns the record's predecessor in the hash list to allow the
 * record to be removed easily.
 *
 * The osi_iodoneLock must be held, and interrupts must be blocked.
 */
static epia_intransit_t *
GetIntransRec(daddr_t dblk, int create, epia_intransit_t ***predPP)
{
    epia_intransit_t **head = &intrans_hashTab[EPIA_INTRANS_HASH(dblk)];
    epia_intransit_t **predP = head;
    epia_intransit_t *h = *head;

    while (h != NULL && h->dblk != dblk) {
	predP = &h->next;
	h = h->next;
    }

    if (h != NULL || !create) {
/* XXX */
	if (epia_intransit_log && (h != NULL)) {
	    epia_intransit_t *n = h->next;
	    while (n != NULL) {
		osi_assert(n->dblk != dblk);
		n = n->next;
	    }
	}

	*predPP = predP;

	return h;
    }

    h = intrans_freelist;
    if (h != NULL) {
	intrans_freelist = h->next;

	h->dblk = dblk;
	h->flags = 0;
	h->waiters = 0;

	EPIA_RECORD(CREATEREC, 0, dblk);

	osi_cv_init(&h->condvar);
	h->next = *head;
	*head = h;
	*predPP = head;
    }
    return h;
}

static void
FreeIntransRec(epia_intransit_t *rec, epia_intransit_t **predP)
{
    afsl_Assert(rec->waiters == 0);
    afsl_Assert(*predP == rec && rec->flags == 0);
    *predP = rec->next;
    rec->next = intrans_freelist;
    rec->dblk = 0;
    intrans_freelist = rec;

    EPIA_RECORD(FREEREC, 0, rec->dblk);
}

/*
 * Add n new in-transit record to the free list.  Called without the
 * osi_iodoneLock held.  We may sleep as a result of calling osi_Alloc.
 */
static void
MakeNewIntransRec(int n)
{
    int i;
    epia_intransit_t *list;
    int s;
    list = osi_AllocPinned_r(n * sizeof(epia_intransit_t));
    for (i = 0; i != n; i++) {
	list[i].dblk = 0;
	list[i].flags = 0;
	list[i].waiters = 0;
	if (i != n - 1)
	    list[i].next = &list[i + 1];
   }
   osi_intr_exclude(&osi_iodoneLock, s);
   list[n - 1].next = intrans_freelist;
   intrans_freelist = &list[0];
   osi_intr_allow(&osi_iodoneLock, s);
}

/*
 * Wait if any of the disk blocks associated with bp is "in transit"
 * because of a write in progress.  Also, mark each block "in-transit"
 * if we are about to start a write.  The in-transit state is cleared
 * by the interrupt-time iodone routine upon completion of the write,
 * using epia_ClearInTransit.
 *
 * This lets us avoid races between I/O initiated by the VM
 * system and writes that are part of promotion, allocation,
 * or volume operations.
 *
 * Big assumption: the blocks associated with this buf will not become
 * in transit as a result of concurrent activities by other threads.
 * Thus, we can wait here for each block to leave the in-transit state
 * and then proceed without worrying that some of them may now be in
 * transit again.  The higher-level vnode locking should guarantee this.
 *
 * Related assumption: only one thread will find itself waiting for
 * a particular in-transit disk block.  The VM system will not issue
 * multiple I/O requests for a given block, and other writers are
 * synchronized by higher-level locking.  This lets readers put
 * in-transit records onto the free list without worrying about other
 * threads waiting for the same block.  If this asssumption did not
 * hold, we would need a reference count or some similar mechanism to
 * coordinate multiple readers.
 */
void
epia_MarkInTransit(struct buf *bp)
{
    daddr_t endBlk, dblk;
    int nblk = btodb(bp->b_bcount);
    epia_intransit_t *intrans, **predP;
    int mark = ((bp->b_flags & B_READ) == 0);
    int slept;
#ifdef AFS_AIX_ENV
    int s;
#endif

    dblk = bp->b_blkno;
    endBlk = bp->b_blkno + nblk;

    osi_intr_exclude(&osi_iodoneLock, s);
    while (dblk != endBlk) {
	intrans = GetIntransRec(dblk, 1, &predP);

	EPIA_RECORD((mark ? WR_LOOKREC : RD_LOOKREC), 0, dblk);

	if (intrans == NULL) {
	   /*
	    * If we get here, no in-transit record existed for this
	    * block and the free list was empty.  Add some new records
	    * to the free list and try again.
	    */
	    osi_intr_allow(&osi_iodoneLock, s);
	    MakeNewIntransRec(EPIA_INTRANS_NUM_NEW_FREE_ELEM);
	    osi_intr_exclude(&osi_iodoneLock, s);
	} else {
	    /*
	     * We depend on only one thread at a time waiting for
	     * a particular block.
	     */
	    afsl_Assert((intrans->flags & EPIA_INTRANS_BUSY) ?
			(intrans->waiters == 0) :
			(intrans->waiters <= 1));
	    /*
	     * Wait for block if it is already in transit
	     */
	    slept = 0;
	    while (intrans->flags & EPIA_INTRANS_BUSY) {
		slept = 1;
		intrans->waiters++;

                epia_intransit_cur_waiters++;
                epia_intransit_cum_waiters++;

		EPIA_RECORD((mark ? WR_MARKWAIT : RD_MARKWAIT),
		       intrans->waiters, dblk);
		osi_intr_wait(&intrans->condvar, &osi_iodoneLock);

                osi_intr_allow(&osi_iodoneLock, s);

                if (mark)
                    icl_Trace4(epi_iclSet, TR_WR_INTRANSIT_BUSY,
                               ICL_TYPE_POINTER, osi_ThreadID(),
                               ICL_TYPE_LONG, dblk,
                               ICL_TYPE_LONG, (long)osi_major(osi_bdev(bp)),
                               ICL_TYPE_LONG, (long)osi_minor(osi_bdev(bp)));
                else
                    icl_Trace4(epi_iclSet, TR_RD_INTRANSIT_BUSY,
                               ICL_TYPE_POINTER, osi_ThreadID(),
                               ICL_TYPE_LONG, dblk,
                               ICL_TYPE_LONG, (long)osi_major(osi_bdev(bp)),
                               ICL_TYPE_LONG, (long)osi_minor(osi_bdev(bp)));

                osi_intr_exclude(&osi_iodoneLock, s);
                epia_intransit_cur_waiters--;
		intrans->waiters--;
		afsl_Assert(intrans->dblk == dblk);
	    }

	    EPIA_RECORD((mark ? WR_WAITOVER : RD_WAITOVER), 0, dblk);

	    afsl_Assert(intrans->flags == 0);
	    if (mark) {
		intrans->flags = EPIA_INTRANS_BUSY;

		EPIA_RECORD(MARKBUSY, 0, dblk);
	    } else {
		if (slept) {
		    /* predP may have changed while we slept */
		    epia_intransit_t *it = GetIntransRec(dblk, 0, &predP);
		    afsl_Assert(it == intrans);
		}
		if (intrans->waiters == 0)
		    FreeIntransRec(intrans, predP);
	    }
	    dblk++;
	}
    }
    osi_intr_allow(&osi_iodoneLock, s);
}

/*
 * Clear the in-transit bit for the disk blocks associated with bp.
 * If another thread is waiting for one of the blocks, wake it up;
 * otherwise free the in-transit record for the block.
 *
 * Note that this routine is called at interrupt time.
 */
void
epia_ClearInTransit(struct buf *bp)
{
    daddr_t firstBlk = bp->b_blkno, dblk;
    int nblk = btodb(bp->b_bcount);
    epia_intransit_t *intrans, **predP;

    afsl_Assert((bp->b_flags & B_READ) == 0);
    for (dblk = firstBlk; dblk != firstBlk + nblk; dblk++) {
	intrans = GetIntransRec(dblk, 0, &predP);
	afsl_Assert(intrans != NULL &&
		    (intrans->flags & EPIA_INTRANS_BUSY));
	/*
	 * Wake anyone waiting for the block
	 */
	intrans->flags &= ~EPIA_INTRANS_BUSY;

	EPIA_RECORD(CLEARBUSY, 0, intrans->dblk);

	if (intrans->waiters) {

	    EPIA_RECORD(WAKEWAITERS, intrans->waiters, intrans->dblk);

	    osi_intr_wakeup(&intrans->condvar);
	} else
	    FreeIntransRec(intrans, predP);
    }
}

/*
 * External interface to let epia_Strategy callers clean up following
 * an I/O error.  Removes any remaining in-transit blocks for bp.
 */
void
epia_BufError(struct buf *bp)
{
    daddr_t firstBlk = bp->b_blkno, dblk;
    int nblk = btodb(bp->b_bcount);
    epia_intransit_t *intrans, **predP;
#ifdef AFS_AIX_ENV
    int s;
#endif

    if ((bp->b_flags & B_READ) != 0)
	return;

    osi_intr_exclude(&osi_iodoneLock, s);
    for (dblk = firstBlk; dblk != firstBlk + nblk; dblk++) {
	intrans = GetIntransRec(dblk, 0, &predP);
	if (intrans == NULL || !(intrans->flags & EPIA_INTRANS_BUSY))
	    continue;
	/*
	 * Wake anyone waiting for the block
	 */
	intrans->flags &= ~EPIA_INTRANS_BUSY;

	EPIA_RECORD(ERR_CLEARBUSY, 0, dblk);

	if (intrans->waiters) {

	    EPIA_RECORD(ERR_WAKEWAITERS, intrans->waiters, intrans->dblk);

	    osi_intr_wakeup(&intrans->condvar);
	} else
	    FreeIntransRec(intrans, predP);
    }
    osi_intr_allow(&osi_iodoneLock, s);
}

/*
 * Interrupt-time processing of bufs after I/O.  Our responsibilities
 * here are to mark the buf "done" and wake up any waiters, to clean up
 * after asynchronous I/O (e.g., by unlocking pages), and to update the
 * undo-zero database.
 *
 * XXX The undo-zero processing is not yet implemented.  When it is,
 * we will have to provide a mutual exclusion mechanism to allow updating.
 * This will most likely involve generalizing the existing osi_splbio()
 * mechanism.
 */
osi_iodone_t
epia_Iodone(struct buf *bp)
{
#ifdef AFS_SUNOS5_ENV
    struct vnode *vp = bp->b_vp;
#endif /* AFS_SUNOS5_ENV */

#if defined(AFS_AIX_ENV) || defined(AFS_HPUX_ENV)
    afsl_Assert((bp->b_flags & B_DONE) != 0);
#else
    afsl_Assert((bp->b_flags & B_DONE) == 0);
#endif	/* AFS_AIX_ENV, AFS_HPUX_ENV */

    if (!(bp->b_flags & B_READ) && bp->b_blkno != EFS_HOLE) {
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

#if defined(AFS_SUNOS5_ENV) && defined(_KERNEL)
    /*
     * Avoid calling efs_inactive at interrupt time following asynchronous
     * I/O, since that would get us into locking difficulties.  If we are
     * about to release the last reference to the vnode here, hold it an
     * extra time and queue it for the async daemon, which will do the
     * final VN_RELE for us.
     */
    if ((bp->b_flags & B_ASYNC) && bp->b_blkno != EFS_HOLE) {
	VN_HOLD(vp);
	osi_biodone(bp, &strat_wait);

	osi_mutex_enter(&vp->v_lock);
	if (vp->v_count == 1)
	    epia_QueueAsync(vp);
	else {
	    --vp->v_count;
	    epia_AsyncUnreserve();
	}
	osi_mutex_exit(&vp->v_lock);
	osi_iodone_return(0);
    }
#endif /* AFS_SUNOS5_ENV  && _KERNEL */

#if defined(AFS_AIX_ENV) && defined(KERNEL)
    /*
     * If bp->b_forw is set, then call the provided FinishIO function
     * to free the chain of buffers related to this request and
     * free the request buffer.  We cannot reference bp After we call
     * the FinishIO function
     */
    if (bp->b_forw) {
	long code = ((bp->b_flags & B_ERROR)? bp->b_error: 0);
	void (*ioFinish)() = (void (*)())bp->b_back;

	(*ioFinish)(bp, code);
	osi_iodone_return(0);
    }
#endif /* AFS_AIX_ENV && KERNEL */

    osi_biodone(bp, &strat_wait);
    osi_iodone_return(0);
}

/*
 * Prepare buf bp for disk I/O.  At this point, the disk address,
 * transfer size, and data are already attached to the buf.  All
 * that remains is to set the device, iodone function, and BUSY
 * bits, with some OS-specific fiddling around on some platforms.
 *
 * XXX We should probably do the complete initialization earlier.
 * It is silly to reinitialize almost everything here with the
 * same values.
 */
static void
PrepareBuf(struct buf *bp, async_device_t *aggr)
{
    dev_t device = osi_makedev(aggr->majdev, aggr->mindev);
    bp->b_bcount = dbroundup(bp->b_bcount);
    osi_BufInit(
	bp, bp->b_flags, bp->b_un.b_addr, bp->b_bcount, device,
	bp->b_blkno, bp->b_vp, epia_Iodone);
}

/*
 * Handle a strategy read or write request on an in-line anode.
 */
static long
DoInlineIO(struct buf *bp, epi_anode_t ap)
{
    u_long length = bp->b_bcount;
    elbb_tranRec_t tranId;
    long code;

    afsl_Assert(length <= epix_GetMaxInlineData(ap));
    epia_MapBufs(bp);
    if (bp->b_flags & B_READ)
	code = epix_ReadInline(ap, 0, length, bp->b_un.b_addr);
    else {
	code = epia_StartTran("DoInlineIO", ap, &tranId);
	afsl_MBZ(code);
	code = epix_WriteInline(tranId, ap, 0, length, bp->b_un.b_addr);
	(void) epia_EndTran(ap, tranId);
	if ((bp->b_flags & B_ASYNC) == 0 && code == 0)
	    (void) epix_FsyncForce(ap);
    }
    epia_UnmapBufs(bp);
#if defined(AFS_AIX_ENV) || defined(AFS_HPUX_ENV)
    bp->b_flags |= B_DONE;
#endif	/* AFS_AIX_ENV, AFS_HPUX_ENV */
    epia_Iodone(bp);

    return (code);
}

/*
 * Strategy statistics
 */
static struct strat_stat {
	u_long nreads, nwrites;		/* How many reads, writes */
	u_long rblocks, wblocks;	/* # of disk blocks read, written */
} strat_stat;

/*
 * epia_Strategy -- Low-level I/O interface for reading and writing pages
 *	of user data.  Schedules the I/O specified by bp.  We assume that
 *	bp describes a contiguous region of disk storage; all necessary
 *	allocation must already have been done at this point.  Reading
 *	from holes and attempts to read beyond EOF should be handled at
 *	a higher level.  No transaction is needed here because we handle
 *	only user data.
 *
 * PARAMETERS --
 *	bp is a system buffer describing the I/O to be done. The transfer
 *	size in bytes is given by bp->b_bcount, and the starting disk
 *	address is given in bp->b_blkno.
 *
 *	ap is the anode handle for the I/O.  It must match the anode
 *	handle that is attached to the buf's vnode.  It would be safer
 *	to get this information directly from bp->b_vp but the current
 *	source code organization gets in the way.
 */
long
epia_Strategy(struct buf *bp, epi_anode_t ap)
{
    async_device_t *aggr = epia_AnodeDevice(ap);
    u_long flags = epia_GetFlags(ap);
    long code;
#ifdef AFS_DEBUG
    epia_info_t info;
    u_long nblks = btodb(dbroundup(bp->b_bcount));

    code = epia_GetInfo(ap, &info);
    afsl_MBZ(code);
    afsl_Assert(!(info.flags & EPIA_CFLAGS_LOGGED));
    afsl_Assert(AFS_hfitsinu32(info.length)); /* Disallow >2GB for now */
    afsl_Assert(bp->b_bcount != 0);

    if (bp->b_flags & B_READ) {
	strat_stat.nreads++;
	strat_stat.rblocks += nblks;
    } else {
	strat_stat.nwrites++;
	strat_stat.wblocks += nblks;
    }
#endif /*AFS_DEBUG*/

    /*
     * Handle in-line anodes specially (no disk I/O)
     */
    if (flags & EPIA_CFLAGS_INLINE)
	return DoInlineIO(bp, ap);

    bp->b_bcount = dbroundup(bp->b_bcount);
    osi_assert(bp->b_blkno != EFS_HOLE);

    /*
     * Set up bp for disk I/O.
     */
    PrepareBuf(bp, aggr);

    /*
     * Mark these disk blocks in-transit if this is a write,
     * first waiting if any of them is already in transit.
     */
    epia_MarkInTransit(bp);

#ifdef AFS_SUNOS5_ENV
    /*
     * If this is an async I/O, make sure that the async daemon has
     * enough resources allocated to handle an interrupt-time request
     * once the I/O completes.
     */
    if (bp->b_flags & B_ASYNC)
	epia_AsyncReserve();
#endif /* AFS_SUNOS5_ENV */

    /*
     * Update kernel resource usage statistics.
     */
    if (bp->b_flags & B_READ)
	osi_inc_ru_inblock(1);
    else
	osi_inc_ru_oublock(1);

    /*
     * Start the I/O.
     */
    osi_strategy(bp);
    return 0;
}

int
epia_BioWait(struct buf *bp)
{
    int s;
    int code;
    osi_intr_exclude(&osi_iodoneLock, s);
    while (!(bp->b_flags & B_DONE))
	osi_bio_wait(bp, &strat_wait, &osi_iodoneLock);

    osi_intr_allow(&osi_iodoneLock, s);

    code = geterror(bp);
    if (code != 0)
	epia_BufError(bp);

    return code;
}

/*
 * epia_InitStrategy -- Initialize the data-structures needed for the strategy
 *     code.  Called at startup time from efs_init().
 */
void
epia_InitStrategy(void)
{
#if defined(AFS_AIX_ENV) && defined(KERNEL)
    long code;

    code = pin(&intrans_freelist, sizeof(intrans_freelist));
    afsl_MBZ(code);
    code = pin(&intrans_hashTab[0], sizeof(intrans_hashTab));
    afsl_MBZ(code);

    code = pin(epia_intransit_log, sizeof(int));
    afsl_MBZ(code);
    code = pin(recIndex, sizeof(int));
    afsl_MBZ(code);
    code = pin(wrapped, sizeof(int));
    afsl_MBZ(code);
    code = pin(recTab, sizeof(struct epia_logrec) * RECTABSIZE);
    afsl_MBZ(code);
#endif /* AFS_AIX_ENV */
    recIndex = 0;

    osi_cv_init (&strat_wait);
}
