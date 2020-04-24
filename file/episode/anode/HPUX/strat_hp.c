/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
*/
/*
 * HPUX-specific strategy functions
 */
/*
 * HISTORY
 * $Log: strat_hp.c,v $
 * Revision 1.1.28.1  1996/10/02  17:15:37  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:06:57  damon]
 *
 * $EndLog$
 */

/* Copyright (C) 1996, 1993 Transarc Corporation - All rights reserved */

#include <strategy.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/episode/anode/HPUX/strat_hp.c,v 1.1.28.1 1996/10/02 17:15:37 damon Exp $")

#define DEBUG_THIS_FILE EPIA_DEBUG_STRATEGY

#define EPIA_IOBOUNDARY(x)	0
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

    do {
	struct buf *nextbp = (struct buf *) bp->b_work;
	iodone(bp);
	bp = nextbp;
    } while (bp != NULL);
}

/*
 * epia_SendGroup -- starts the I/O to the current group of blocks.  The
 *     resulting I/O event is passed to epia_AddXfer.
 *
 * PARAMETERS -- It gets all the information it needs from the group structure
 *     passed to it.  This allows for communication between the top-level
 *     control in epia_BufWrite and epia_AddToGroup.
 *
 *     The current group is ended by clearing group->firstBp.
 */
void
epia_SendGroup(struct epia_group *group)
{
    struct buf *firstBp = group->firstBp;
    long groupLen = group->len;
    asevent_event_t *ae;
    long ioCode;

    if (firstBp == NULL) {
	if (group->iodata != NULL) {
	/*
	 * This is the last cleanup call.  If we still have an iodata
	 * block, give it back but it had better be empty.  This can
	 * result from quota/space error.
	 */
	    afsl_Assert (!group->iodata->nBlks);
	    epia_FreeIodata (group->iodata);
	    group->iodata = 0;
	}
	return;
    }
    epix_EnterCodeBlock(31);

    firstBp->b_bcount = groupLen;

    if (epix_IsBlockEmpty (group->firstBlock)) {
	epix_EnterCodeBlock(34);
	epia_FinishBufIO (firstBp, group->code, /*don't unmap*/0);
    } else {
	struct buf *abp;
	ae = asevent_Create(ASEVENT_NOPIN);

	abp = asevent_MakeBuf(B_WRITE, firstBp->b_un.b_addr, groupLen);

	asevent_InitBuf(group->device,
	    group->firstBlock, group->off, groupLen, abp);
	ae->data = (opaque)firstBp;
	icl_Trace4(epi_iclSet, TR_STRAT_QIO_WRITE,
	    ICL_TYPE_POINTER, group->h, ICL_TYPE_POINTER, abp,
	    ICL_TYPE_LONG, abp->b_blkno, ICL_TYPE_LONG, groupLen);
	ioCode = asevent_QueueIO(group->device, abp, ae);
	if (ioCode) {
	    epix_EnterCodeBlock(35);
	    asevent_Release(ae);
	    epia_FinishBufIO (firstBp, ioCode, /*don't unmap*/0);
	    ae = 0;
	}

	if (group->iodata == NULL) /* get new extent group if necessary */
	    group->iodata = epia_StartXfer(group->device);

	if (epia_AddXfer(ae, group->firstBlock,
			group->off, groupLen, group->iodata) == 0) {
	    group->iodata = NULL;
	}
    }
    group->reqLength -= groupLen;
    group->firstBp = NULL;
}

/*
 * epia_BufWrite -- is called when epia_Strategy decides that a simple write
 *     is possible.  Blocks must be allocated to the container as necessary
 *     and contiguously if possible.  The request is divided into a sequence
 *     of sub-requests that are contiguous on disk.
 *
 *     To simplify the analysis of the undo-zero tracking problem we limit the
 *     number of blocks for which I/O events and allocation transactions are
 *     interrelated.  The iodata structure tracks this situation which is
 *     queried using the RemainginIOSpace macro.
 *
 * PARAMETERS -- We pass the fileLength in as a afs_hyper_t to ease the call to
 *     SetLength.  We also pass it on to MakeBlock since it has a transaction
 *     already and can set the length as a side effect very cheaply.  Otherwise
 *     we have to start a transaction to do it.
 *
 *     The caller always passes us block-aligned transfers.  However, this code
 *     makes some effort (see groupOff variable) to handle cases where this
 *     isn't true.  It isn't clear if this is useful or correct.
 *
 * RETURN CODES -- Since the procedure is responsible for calling MapBufs,
 *     which under normal circumstances are not unmapped until I/O completion
 *     time, we need to be very careful about returning error codes.  The rule
 *     is that we will never return an error code without unmapping and
 *     perhaps notifying the correct buffers.  Generally this means we will not
 *     return an error code at all but just Notify the affected bufs instead.
 *     The caller should just return.
 */
long
epia_BufWrite(
  epi_anode_t h,
  struct buf *bp,			/* first buf in list */
  u_long length,			/* length of whole buf list */
  afs_hyper_t fileLength,			/* length to set file to */
  struct epia_reservation *resP)	/* vnode's reservation data */
{
    long code;
    async_device_t *device = epia_AnodeDevice (h);
    u_long bsize = device->blkSize;
    struct epia_group group;		/* info about current group */
    u_int bcount;

    icl_Trace4(epi_iclSet, TR_STRAT_BUFWRITE,
	ICL_TYPE_POINTER, h, ICL_TYPE_POINTER, bp,
	ICL_TYPE_LONG, dbtob(bp->b_blkno), ICL_TYPE_LONG, length);
    epix_EnterCodeBlock(15);

    bzero((caddr_t)&group, sizeof (group));
    group.device = device;
    group.h = h;
    group.reqLength = length;
    group.fileLength = fileLength;
    group.resP = resP;

    while (bp) {
	struct buf *nextBp = (struct buf *)bp->b_work;
	if (nextBp) bcount = dbtob(nextBp->b_blkno - bp->b_blkno);
	else bcount = bp->b_bcount;

	epia_AddToGroup(&group, bp, bcount);

	if (group.firstBp &&
	    EPIA_IOBOUNDARY(osi_abtob(device, group.firstBlock) + group.len)) {
	    /* next block would start a new track so end this group here */
	    epix_EnterCodeBlock(16);
	    epia_SendGroup(&group);
	}
	bp = nextBp;
    }

    epia_SendGroup(&group);		/* handle any left over pieces */
    afsl_Assert(!group.reqLength && !group.iodata && !group.firstBp);

    if (AFS_hcmp(fileLength, epia_GetLength(h)) > 0) {
	/* if no allocations were necessary, we have to do this ourselves */
	epix_EnterCodeBlock(18);
	BEGIN_LCRIT(elbt_TranRec_t trans,
		    code = epia_StartTran("epia_BufWrite: set length",
					  h, &trans),
		    code = epia_EndTran(h, trans)) {
	    code = epix_SetLength (trans, h, fileLength, 1);
	} END_CRIT;

	/* TODO -- We need a better way to pass this error code back. */
	afsl_TraceEnabled (1, code, AFSL_TR_ALWAYS,
 ("Start/End Trans or SetLength failed w/ code=%d; ignoring.", code));
    }
    return 0;
}

/*
 * epia_BufRead -- is called when epia_Strategy decides that a simple read 
 *     is possible.  The main job here is to divide the request into a
 *     sequence of sub-requests that are contiguous on disk.
 *
 * CAUTIONS -- See note below on use of the b_resid field.
 */
long
epia_BufRead(
  IN epi_anode_t h,
  IN struct buf *bp,
  IN u_long length,
  INOUT int *foundHoleP)
{
    long code;
    async_device_t *device = epia_AnodeDevice (h);
    u_long bsize = device->blkSize; /* File system (not device) blk size */
    daddr_t firstBlock;		/* disk block of beginning of group */
    daddr_t this, next;		/* disk address of current, next blk */
    asevent_event_t *ae;	/* "event" to keep track of I/O */
    epia_findBlocksData_t blocks; /* opaque state from epia_GetBlock */
    long bcount;		/* size of last buf in group */
    int endGroup = 1;		/* True if we need to start a new group */
    epia_ioData_t *iodata;
    long bytes;			/* # bytes in current xfer */
    long offset;		/* block offset of start of xfer */
    struct buf *nextBp, *firstBp = NULL;
    struct buf *abp;

    icl_Trace4(epi_iclSet, TR_STRAT_BUFREAD,
	ICL_TYPE_POINTER, h, ICL_TYPE_POINTER, bp,
	ICL_TYPE_LONG, dbtob(bp->b_blkno), ICL_TYPE_LONG, length);
    bzero((caddr_t)&blocks, sizeof (blocks));
    code = epia_GetBlock(h, dbtoab(device, bp->b_blkno), &blocks, &this);
    if (code)
	return code;

    while (bp != NULL) {
	afsl_Assert(length > 0 && (bp->b_flags & B_READ) != 0);

	if (endGroup) {
	    /*
	     * Start a new group
	     */
	    endGroup = 0;
	    firstBp = bp;
	    firstBlock = this;
	}

	nextBp = (struct buf *)bp->b_work;
	if (nextBp == NULL) {
	    endGroup = 1;
	    bcount = bp->b_bcount;
	} else {
	    daddr_t blkno = nextBp->b_blkno;
	    baddr_t nextBlock = dbtoab(device, blkno);
	    u_long nextOff = mod_2(dbtob(blkno), bsize);
	    bcount = dbtob(nextBp->b_blkno - bp->b_blkno);

	    if (nextOff != 0)
		next = this;		/* next buf is in same block */
	    else if (dbtob(blkno - firstBp->b_blkno) >= length)
		endGroup = 1;
	    else {
		code = epia_GetBlock(h, nextBlock, &blocks, &next);
		endGroup =
		    (!EPIA_CONTIGUOUS(this, next) ||
		    EPIA_IOBOUNDARY(osi_abtob(device, next)));
	    }
	}

	if (epix_IsBlockEmpty(this)) {
	    *foundHoleP = 1;
	    /* zero this buf's data */
	    afsl_Assert (bp == firstBp);
	    epia_MapBufs(bp);
	    bzero(bp->b_un.b_addr, bcount);
	    bp->b_work = 0;		/* terminate chain here */
	    epia_FinishBufIO(bp, 0, /*unmap*/1);
	    length -= MIN(length, bcount);
	    endGroup = 1;
	} else if (endGroup) {		/* schedule I/O. */
	    bytes = dbtob(bp->b_blkno - firstBp->b_blkno) + bcount;

	    /*
	     * XXX Need to fix this; b_resid won't be preserved across I/O
	     */
	    if (bytes <= length) {
		firstBp->b_resid = 0;
	    } else {
		/*
		 * We remember the number of bytes between the end of the file
		 * (which we use for the, possibly imprecise, transfer length)
		 * and the buffer length, in the buf's b_resid field so that we
		 * can bzero them at I/O completion time.
		 */
		*foundHoleP = 1;
		firstBp->b_resid = bytes - length;
		firstBp->b_bcount = bytes;
		bytes = length;
	    }
	    offset = mod_2(dbtob(firstBp->b_blkno), bsize);
	    ae = asevent_Create(ASEVENT_NOPIN);
	    abp = asevent_MakeBuf(B_READ,
				  firstBp->b_un.b_addr, firstBp->b_bcount);

	    asevent_InitBuf(device, firstBlock, offset, bytes, abp);
	    ae->data = (opaque)firstBp;
	    icl_Trace4(epi_iclSet, TR_STRAT_QIO_READ,
		ICL_TYPE_POINTER, h, ICL_TYPE_POINTER, abp,
		ICL_TYPE_LONG, abp->b_blkno, ICL_TYPE_LONG, bytes);
	    code = asevent_QueueIO(device, abp, ae);
	    if (code != 0) {
		asevent_Release(ae);
		epia_FinishBufIO(firstBp, code, 0);
		break;
	    }
	    bp->b_work = 0;		/* terminate chain here */

	    iodata = epia_StartXfer(device);
	    code = epia_AddXfer(ae, firstBlock, offset, bytes, iodata);
	    afsl_MBZ(code);	/* should enqueue immediately */
	    length -= bytes;
	}

	/* Remaining bufs are beyond EOF; handle various strange cases
	 * where we get extra bufs */
	if (length == 0 && nextBp != NULL) {
	    epia_MapBufs(nextBp);
	    /* get count */
	    bp = nextBp;
	    while (bp->b_work)
		bp = (struct buf *)bp->b_work;
	    bcount = dbtob(bp->b_blkno - nextBp->b_blkno) + bp->b_bcount;
	    bzero(nextBp->b_un.b_addr, bcount);
	    epia_FinishBufIO(nextBp, /*code*/0, /*unmap*/1);
	    break;
	}
	bp = nextBp;
	this = next;
    }
    afsl_Assert (code != 0 || length == 0);
    return (code);
}
