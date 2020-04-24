/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
*/
/*
 * AIX-specific strategy functions
 */

/* Copyright (C) 1994, 1993 Transarc Corporation - All rights reserved */

#include <strategy.h>

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/anode/RIOS/RCS/strat_aix.c,v 1.13 1995/04/19 20:23:07 ota Exp $")

#define DEBUG_THIS_FILE EPIA_DEBUG_STRATEGY

#if !defined(KERNEL)
#include <pthread.h>
#endif /* !KERNEL */

/*
 * The alignment requirement for AIX is that no physical I/O crosses 128k
 * boundaries.  Return true if the specified disk address is on a track
 * boundary.
 *
 * XXX Surely there is some system-defined symbol for this?!
 */
#define EPIA_TRACKALIGN		0x20000		/* 128 K */

#define EPIA_IOBOUNDARY(addr) (mod_2(addr, EPIA_TRACKALIGN) == 0)

#ifdef KERNEL
/* epia_MapBufs -- make b_addr field of a chain of struct buf's available in the
 *     KERNEL address spacce.
 *
 * CAUTIONS -- On AIX31 we have to change the b_addr when we map in the
 *     buffers, so we have to save and restore the old value somewhere.  Since
 *     we have to pass this around between processes and over the time required
 *     for an I/O saving it in the buf itself is to be preferred.  We use the
 *     av_back field for this. */
void
epia_MapBufs(struct buf *bp)
{
    bp->av_back = (struct buf *)bp->b_un.b_addr;
    bp->b_un.b_addr = (caddr_t)vm_att(bp->b_xmemd.subspace_id,
				      bp->b_un.b_addr);
}

/* epia_UnmapBufs -- invert action of epia_MapBufs.  See which. */

void
epia_UnmapBufs(struct buf *bp)
{
    vm_det(bp->b_un.b_addr);
    bp->b_un.b_addr = (caddr_t)bp->av_back;
}

#else /* KERNEL */
/* user space simulation of address space mapping */

static struct cons {
    opaque car;
    long file;
    struct cons *cdr;
} *consList;

#define Cons(a, b)	MakeCons((struct cons *)(a), (struct cons *)(b))
#define push(list, a, b) ((list = Cons (a, list))->file = (b))
#define pop(list)	PopList(&(list))

static void
FreeCons(struct cons *c)
{
    c->cdr = consList;
    consList = c;
}

static opaque
PopList(struct cons **listP)
{
    struct cons *c = *listP;
    opaque car;
    car = c->car;
    *listP = c->cdr;
    FreeCons (c);
    return car;
}

static struct cons *
MakeCons(struct cons *a, struct cons *b)
{
    struct cons *c = consList;
    if (c == NULL)
	c = osi_Alloc_r(sizeof (*c));
    else
	consList = c->cdr;
    c->car = (opaque)a;
    c->cdr = b;
    return c;
}

#define MatchCons(c, blk, file) \
    (((opaque)(((struct cons *)(c))->car) == (opaque)blk) && \
     (((struct cons *)(c))->file == file))

static struct cons *
FindCons(struct cons *list, opaque blk, long file)
{
    struct cons *c;
    for (c=list; c; c=c->cdr)
	if (MatchCons(c, blk, file)) return c;
    return 0;
}

static struct cons *mappedBufs;

void
epia_MapBufs(struct buf *bp)
{
    int bcount = bp->b_bcount;
    daddr_t blk = bp->b_blkno;
    long file = (long)bp->b_vp;

    while (bcount > 0) {
	afsl_Assert (!FindCons (mappedBufs, (opaque)blk, file));
	push (mappedBufs, blk, file);
	blk++;
	bcount -= dbtob(1);
    }
}

void
epia_UnmapBufs(struct buf *bp)
{
    int bcount = bp->b_bcount;
    daddr_t blk = bp->b_blkno;
    long file = (long)bp->b_vp;

    while (bcount > 0) {
	int found = 0;
	struct cons **cP;
	cP = &mappedBufs;
	while (*cP) {
	    if (MatchCons (*cP, blk, file)) {
		/* remove what cP points to and adjust cdr */
		PopList (cP);
		found++;
	    }
	    else cP = &((*cP)->cdr);
	}
	afsl_Assert (found == 1);
	blk++;
	bcount -= dbtob(1);
    }
}
#endif /* KERNEL */

/*
 * Map Episode I/O error code onto standard Unix code and set
 * buffer error field.
 */
static void
MapErrorCode(struct buf *bp, long code)
{
    if (code) {
	int report = 0;

	switch (code) {
	  case EPI_E_VOLUMEFULL:
	    bp->b_error = EDQUOT;
	    break;
	  case EPI_E_AGGREGATEFULL:
	    bp->b_error = ENOSPC;
	    break;
	  case EPI_E_BLOCKTOOBIG:
	    bp->b_error = EFBIG;
	    break;
	  case EIO:
	    bp->b_error = EIO;
	    break;
	  default:
	    bp->b_error = EIO;
	    report = 1;
	    break;
	}

	afsl_TraceCond (AFSL_TR_ERRMAP, ((char)code) != ((char)bp->b_error),
			("epia_FinishBufIO: code %u mapped to %u",
			 code, bp->b_error));
	afsl_TraceEnabled (1, report, AFSL_TR_ALWAYS,
			   ("NotifyAnd...: I/O error=%d, file/blkno=%x/%d",
			    code, bp->b_vp, bp->b_blkno));

	code = bp->b_error;
	bp->b_flags |= B_ERROR;

	while (bp->b_work != 0) {
	    bp = (struct buf *) bp->b_work;
	    bp->b_flags |= B_ERROR;
	    bp->b_error = code;
	}
    }
}

/* epia_FinishBufIO -- called when I/O completes on a buf.
 *
 *     On AIX, it calls the iodone function and unmaps the buf data ptr
 *     from the KERNEL address space, thus reversing the effect of epia_MapBufs.
 *     On SunOS, we call our own function to do iodone-type cleanup.
 *
 * RETURN CODES -- We pass whatever error code we have to the buffer,
 *     mapping to errno codes. */
void
epia_FinishBufIO(
  struct buf *bp,			/* list of buf's for I/O */
  long code,				/* error to attach to bufs */
  int unmap)				/* whether to unmap or not */
{
    if (code != 0)
	MapErrorCode(bp, code);

    if (unmap)
	epia_UnmapBufs(bp);

    do {
	struct buf *nextbp = (struct buf *) bp->b_work;
	iodone(bp);
	bp = nextbp;
    } while (bp != NULL);
}

/*
 * There is currently no async daemon for AIX.
 */
void
epia_StartAsyncDaemon(void)
{
}
