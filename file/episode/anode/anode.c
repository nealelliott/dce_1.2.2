/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* anode.c -- contains procedures which implement the basic read and write
 *     operations.  Primarily this involves allocating new blocks to an anode
 *     during a write operation.  It depends on the fixed_anode module for the
 *     basic anode functions and on the block allocation module to allocate
 *     blocks, and indirectly on the volume_quota module to check and update
 *     volume quota. */

/* Copyright (C) 1996, 1990 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/episode/instrument.h>
#include <dcedfs/episode/logbuf.h>
#include <anode.h>
#include <fixed_anode.h>
#include <block_alloc.h>
#include <quota.h>

#if !defined(KERNEL)
#include <pthread.h>
#endif /* !KERNEL */

RCSID ("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/anode/RCS/anode.c,v 4.192 1996/03/04 18:57:44 ota Exp $")

#define DEBUG_THIS_FILE EPIA_DEBUG_ANODE

#ifdef AFS_DEBUG
int epia_codeBlock[EPIA_DEBUG_NFILES][EPIA_MAX_FILE_CODE_BLOCKS];
#endif

#define div_l2(a, b) ((a) >> (b))
#define ceil_l2(a, b) (((a) + ((1 << (b)) - 1)) >> (b))
#define mod_2(a, b) ((a) & ((b) - 1))

/*
 * hook for implementing monitoring and tuning specific to this submodule.
 */
/* EXPORT */
int ins_anode_anode(opcode,vars,argObjIds,resultObjIds,resultValues)
  IN struct ins_opDesc *opcode;
  IN struct ins_varDesc *vars;
  IN struct ins_basicObjectId *argObjIds;
  OUT struct ins_basicObjectId *resultObjIds;
  OUT struct ins_basicObject *resultValues;
{
    /* not yet imlemented */
    return 0;
}

#if 0
static long elbb_UndoZeroBuffer (b, off, len, bP)
  IN struct elbb_buffer *b;
  IN u_long off;
  IN u_long len;
  OUT struct elbb_buffer **bP;
{
    int lfs = b->adev->logFragSize;
    afsl_Assert (((off|len) & ((1<<lfs)-1)) == 0);
    DBprint (("epia_Write (undo-zero): blk=%d off=%df len=%df",
	      b->blockNum, off>>lfs, len>>lfs));
    return 0;
}
#endif /* 0 */

static int makezero(
  IN u_long zeroBytes,
  INOUT struct uio *uio)
{
    static const char zero[128] = { 0 }; /* assume this really is zero */
    int code;
    u_long l;
    epix_EnterCodeBlock(21);
    for (l = sizeof(zero); zeroBytes; zeroBytes -= l) {
	epix_EnterCodeBlock(1);
	if (zeroBytes < l) l = zeroBytes;
	code = osi_uiomove ((caddr_t)zero, l, UIO_READ, uio);
	if (code) return code;
    }
    return 0;
}

/* FsyncMagic -- provides a hash value that is probably different for every
 *     file in the system.  It is used to locate dirty blocks in the buffer
 *     pool during fsync. */

#define FsyncMagic(h) \
    (((u_long)epia_AnodeDevice(h)) + \
     (epia_GetVolIndex(h) << 16) + epia_GetIndex(h))

/*
 * Unbuffered I/O
 *
 * We use unbuffered I/O in kernels where there is a VM system that is
 * already buffering user data.  In these situations, using our own buffers
 * for user data causes double buffering, which is undesirable.
 *
 * Because VM systems do not buffer meta-data, we never use unbuffered I/O
 * for meta-data.  For simplicity, we do not use unbuffered I/O for inline
 * or fragmented user data either.
 *
 * On a call to epia_Write that would normally be unbuffered, if any zeroing
 * or promoting must be done, we revert to buffered I/O.  However, it is
 * important that the buffers thus created should be killed immediately,
 * before they go stale.  Therefore we release them with the ``syncdel'' mode.
 * <<NOTE:  use ``sync'' for now, until we can modify logbuf package>>
 *
 * Under AIX 3.1, the buffer passed to the driver's strategy function must
 * have an ``xmem'' handle (so that other processes can make the data area
 * addressable).  This is passed down to epia_Read and epia_Write in the
 * uio_xmem field of the uio structure.  Those functions must then get the
 * handle down to the async layer.  Instead of passing it to
 * asevent_QueueIO as another parameter, we slip it directly into the
 * buffer before calling that function.
 */

/* epia_Init -- initializes the anode module.  It is called only once during
 *     system initialization by \fcn{epig_Init}, see which.
 *
 * PARAMETERS -- maxHandles is number of anode handles to preallocate. */

/* EXPORT */
long epia_Init(u_long maxHandles)
{
    static inited = 0;

    if (inited != 0)
	return EPI_E_REINIT;

    inited = 1;

    epix_Init (maxHandles);

#ifdef AFS_DEBUG
    /* CodeBlock: epia */
    epia_codeBlock[DEBUG_THIS_FILE][0] = 93;
#endif

    return 0;
}

/* epia_StartTran -- Start a transaction associated with the device on which
 *     the specified container resides.  The argument \arg{serviceName} is a
 *     string used for debugging purposes. All the higher layers that use the
 *     anode layer for transaction management must call this procedure. This
 *     transaction must be ended by calling \fcn{epia_EndTran}. See
 *     \fcn{epib_StartTran} for additional details. This can also return
 *     error codes from the buffer package.
 *
 * PARAMETERS --
 *     serviceName -- string identifying the purpose of the transaction.
 *     anode -- container to associate with the transaction.
 *     transP -- pointer to the new transaction identifier. */

/* EXPORT */
long epia_StartTran(
  IN char *serviceName,
  IN epi_anode_t anode,
  OUT elbb_tranRec_t *transP)
{
    return epib_StartTran (serviceName, anode, transP);
}

/* epia_EndTran -- Ends the transaction \arg{trans} which may be associated
 *     with the specified container.  See \fcn{epib_EndTran} for additional
 *     details.  All the higher layers that use the anode layer for transaction
 *     management must use this procedure to end transactions.
 *
 * PARAMETERS --
 *     anode -- anode handle.
 *     trans -- transaction to end which must have been obtained by calling
 *         \fcn{epia_StartTran}. */

/* EXPORT */
long epia_EndTran (anode, trans)
  IN epi_anode_t anode;
  IN elbb_tranRec_t trans;
{
    return epib_EndTran (anode, trans);
}

/* epia_Read -- Reads data from a container. Any referenced bytes which have
 *     never been written but are less than the highest point ever written are
 *     read as zeros. If \arg{flags} is set to
 *     \cst{EPIA_RWFLAGS_UNBUFFERED}, the caller (\fcn{epia_Strategy})
 *     is operating on behalf of the virtual memory system and the data being
 *     read must be flushed from the buffer cache before return.
 *
 * PARAMETERS --
 *     h -- an anode handle.
 *     flags -- whether to leave data cached in the buffer system.
 *     uio -- a standard {\unix} data structure that describes the source and
 *         destination of an I/O request. */

/* EXPORT */
long epia_Read(
  IN epi_anode_t h,
  IN long flags,
  INOUT struct uio *uio)
{
    long code;
    u_long length;
    u_long offset;
    struct epia_info info;
    u_long len;
    enum epia_ws_method isAlloc;	/* initial allocation method */
    async_device_t *device;
    u_long bsize;
    u_int logbsize;
    u_long fsize;
    int xmem = 0;	/* remember if using xmem (AIXism) */

    epix_EnterCodeBlock(2);
    code = epia_GetInfo (h, &info);
    if (code) return code;

    afsl_Assert(AFS_hfitsinu32(info.length));
    len = AFS_hgetlo(info.length);

#ifdef	AFS_AIX31_VM
    if (uio->osi_uio_seg == UIO_XMEM) {
	uio->osi_uio_seg = OSI_UIOSYS;
	xmem = 1;
    }
#endif
    length = uio->osi_uio_resid;
    offset = uio->osi_uio_offset;
    if (length == 0) return 0;
    if (offset >= len) return EPI_E_EOF;
    if (offset+length > len) return EPI_E_READSHORT;
    /* zero length read w/ null uio ptrs is OK */
    if (!uio || !uio->osi_uio_iov) return EPI_E_BADARGS;

    device = epia_AnodeDevice (h);
    bsize = device->blkSize;
    logbsize = device->logBlkSize;
    fsize = device->fragSize;

    /* Decide what data storage method we should use */
    if (info.flags & EPIA_CFLAGS_INLINE) isAlloc = WS_INLINE;
    else if (info.flags & EPIA_CFLAGS_FRAGMENTED) isAlloc = WS_FRAGMENTED;
    else if (info.flags & EPIA_CFLAGS_BLOCKED) isAlloc = WS_BLOCKED;
    else isAlloc = WS_EMPTY;

    switch (isAlloc) {
      case WS_EMPTY:
	epix_EnterCodeBlock(31);
	code = makezero (length, uio);
	break;

      case WS_INLINE:
	{   u_long ulen = 0;
	    caddr_t uptr;
	    epix_EnterCodeBlock(32);
	    code = 0;

	    do { /* generally, go around at least twice */
		osi_uiomove_unit (ulen, length, uio, &uptr, &ulen);
		if (ulen == 0) break;
		epix_EnterCodeBlock(24);
		code = epix_GetInline (h, offset, ulen,
				       uio->osi_uio_seg, uptr);
		if (code) return code;
		length -= ulen;
		offset += ulen;
	    } while (ulen != 0);
	}
	break;

      case WS_FRAGMENTED:
	{   struct epix_fragDescription frags;
	    enum elbb_bufType type;	/* buffer type for regular data */

	    epix_EnterCodeBlock(33);
	    afsl_Assert (len <= (bsize - device->fragSize));
	    code = epix_GetFragInfo (h, &frags);
	    if (code) return code;
	    frags.block = epix_MapBlockToBacking (frags.block);
	    if (!epix_IsBlockAllocated(frags.block)) {
		epix_EnterCodeBlock(34);
		code = makezero (length, uio);
		break;
	    }
	    type = ((info.flags & EPIA_CFLAGS_LOGGED)
		    ? elbb_bufLogged : elbb_bufData);
	    BEGIN_LCRIT (struct elbb_buffer *buf,
			 code = elbb_Read(device, frags.block, type, &buf),
			 code = elbb_Release(buf,
					     (flags &
					      EPIA_RWFLAGS_UNBUFFERED) ? 
					     elbb_sdel : elbb_lazy)) {
		char *bfragBase = elbb_BufferData(buf) + frags.first * fsize;
		code = osi_uiomove (bfragBase+offset, length, UIO_READ, uio);
	    } END_CRIT;
	}
	break;

      case WS_BLOCKED:
	{   u_long firstPage = div_l2(offset, logbsize);
	    u_long lastPage = div_l2(offset + length - 1, logbsize);
	    u_long remaining = length;
	    u_long o = mod_2(offset, bsize);
	    u_long p;

	    epix_EnterCodeBlock(35);
	    for (p = firstPage; p <= lastPage; p++) {
		long l = MIN(bsize - o, remaining);
		epix_EnterCodeBlock(3);
		if (flags & EPIA_RWFLAGS_UNBUFFERED) {
		    struct asevent_event *aeP;
		    daddr_t b;
		    async_device_t *dev;
		    caddr_t uptr;
		    u_long ulen = 0;
		    long ll = l;
		    code = epia_Map (h, p, &b, &dev);
		    if (!code) {
			do {
			    struct buf *bp;
			    osi_uiomove_unit(ulen, ll, uio, &uptr, &ulen);
			    if (ulen == 0)
				break;
			    aeP = asevent_Create(xmem ? ASEVENT_NOPIN : 0);
			    bp = asevent_MakeBuf(B_READ, uptr, ulen, dev, b, o);
#ifdef	AFS_AIX31_VM
			    if (xmem)
				bp->b_xmemd = *uio->uio_xmem;
#endif	/* AFS_AIX31_VM */
			    code = asevent_QueueIO(dev, bp, aeP);
			    ll -= ulen;
			    o += ulen;
			    if (code == 0)
				code = asevent_WaitEvent(aeP);
			    asevent_Release(aeP);
			} while (!code && ulen != 0);
		    }
		} else
		    BEGIN_LCRIT (struct elbb_buffer *buf,
				 code = epix_GetBuffer (h, p, &buf),
				 code = elbb_Release (buf, elbb_lazy)) {
			char *page = elbb_BufferData (buf);
			code = osi_uiomove(page + o, l, UIO_READ, uio);
		    } END_CRIT;
		if (code == EPI_E_NOBLOCK) code = makezero (l, uio);
		if (code) return code;
		remaining -= l;
		o = 0;			/* subsequent blocks start at zero */
	    }
	}
	break;

      default:
	panic("illegal isAlloc");
    }
    return code;
}

/* Various constants associated with the writeState info */

/* Don't allocate more than we can free in a single trans in case we exit due
 * to volume quota. */
#define WS_ARRAYSIZE EPIB_MAX_FREEDBLOCKS_PER_TRANS

/* flags: */
#define WS_SOME_INSERTS        (0x1)   /* flag: must call insert */
#define WS_NEW_TRANS	       (0x2)   /* metaTrans created as needed */
#define WS_COPYONWRITE	       (0x4)   /* container is copy on write */
#define WS_GROW		       (0x8)   /* container's length is increasing */
#define WS_PROMOTE	      (0x10)   /* change old storage mode to new one */
#define WS_RELOCATE_FRAGS     (0x20)   /* copy old fragments to new ones */
#define WS_TRUNCATE_FIRST     (0x40)   /* truncate blocked containers before
                                        * converting to frag or inline
                                        * (conditional on SOME_INSERTS). */
#define WS_ZERO_GAP	      (0x80)   /* zero part of gapPage @ oldLength */
#define WS_START_XFER	     (0x100)   /* begin xfer at firstPage */
#define WS_FREE_FRAGS_FIRST  (0x200)   /* will need to free old frag group
                                        * (conditional on SOME_INSERTS). */
#define WS_UNBUFFERED	     (0x400)   /* do unbuffered I/O if possible */
#define WS_GROW_IN_BLOCK     (0x800)   /* promotion frag->block within block */
#define	WS_XMEM		    (0x1000)   /* uio has AIX 3.1 xmem info */
#define WS_NOWITHHOLD	    (0x2000)   /* OK to consume withholding */

#define MAX_BLKS_ALLOC_IN_ONE_TRANS (10)
#define NEW_TRANS_FOR_BLK_ALLOC (999)

struct writeState {
    elbb_tranRec_t trans;
    epi_anode_t h;
    struct uio *uio;
    long flags;

    /* other booleans */
    unsigned newBlock:1;		/* must start undo zero */
    unsigned createPage:1;		/* use CreateBlock instead of Read */

    u_long length;			/* transfer length */
    u_long offset;			/* transfer offset */
    async_device_t *device;
    u_long bsize;
    u_long fsize;
    long fragsPerBlock;
    u_int maxInlineSize;		/* largest inline size we'll need */
    u_long oldLength;			/* initial length of container */
    u_long newLength;			/* offset of 1st byte following xfer */
    u_long maxLength;			/* max of above two lengths */
    u_long curLength;			/* currently allocated length */

    enum epia_ws_method isAlloc;	/* initial allocation method */
    enum epia_ws_method beAlloc;	/* desired allocation method */

    baddr_t thisPage;			/* current page to work on */
    struct elbb_buffer *buf;		/* buffer, if any */

    baddr_t firstPage, lastPage;	/* page range to consider */
    baddr_t gapPage;			/* page of first byte past oldLength */

    u_long remaining;			/* bytes left to transfer */
    u_long fragOff;			/* beginning of new frag in buffer */

    u_long promoteBO, zeroBO, uioDataO, promoteEO, zeroEO; /* region offsets */
    u_long promoteBL, zeroBL, uioDataL, promoteEL, zeroEL; /* region lengths */

    enum elbb_bufType type;		/* buffer type for regular data */
    int dataLog;			/* if write data is to be logged */
    elbb_tranRec_t metaTrans;		/* transaction for meta data */
    u_long totalNew;			/* total new blocks allocated */
    long pendingInsertQuota;		/* quota being returned (<0) after
                                         * doing inserts; visible part. */
    long pendingInsertAlloc;		/*  ditto; alloc part. */
    epia_qproc_t proc;			/* quota procedure */

    /* Cache anode's block map which is needed at various times during a write
     * to a BLOCKED container.  The block map is obtained by calling FindBlocks
     * with bmap.first as the starting page.  This info is later used to call
     * insert if the SOME_INSERTS bit is set in flags.  Before calling
     * FindBlocks call DoInserts to flush previous inserts. */
    struct { /* needed if beAlloc == BLOCKED */
	baddr_t first;			/* first page mapped in blocks */
	int n;				/* number of blocks */
	daddr_t blocks[WS_ARRAYSIZE];	/* blocks cur. in container */
	struct epix_blockLocation loc;
    } bmap;

    /* Cache the anode's frag info and inline data.  The frag info is obtained
     * by calling GetFragInfo and the inline data is obtained by calling
     * GetInline.  Since an anode can only be in one of these two states the
     * storage can be overlapped. */
    struct epix_fragDescription frags;	/* if FRAGMENTED: frag desc */
    char inlineData[200];		/* if INLINE: data */

    /* Cache of allocated but not yet used blocks/frags.  The fragment handline
     * is very simple since we don't really carry them around prospectively.
     * All the fragments we will need are allocated at the same time and we
     * commit to using them immediately.  With blocks we allocate a few at a
     * time.  Thus we may run out of quota in the middle of a long write and we
     * want to leave things in a consistent state which reflects the progress
     * so far.  On error, everything between new.b.next and new.b.n will be
     * returned to the block allocator.  Quota is consumed as it is used not
     * when blocks are allocated. */
    union {
	struct { /* if beAlloc == BLOCKED */
	    /* (b.next > b.n) indicates that b.blocks do not hold any
	     * "allocated unused" blocks */
	    u_int n;			/* number of newBlocks */
	    u_int next;			/* index of next unused block */
	    daddr_t blocks[WS_ARRAYSIZE]; /* allocated blocks, not used yet */
	} b;
	struct /* if beAlloc == FRAGMENTED */
	    epix_fragDescription f;	/* allocated frags */
    } new;

    /* We need to remember the frags allocated for the current buffer so we can
     * register it with the buffer system for new block security. */
    struct {				/* within block given by new.f.block */
	u_short first;			/* first fragment */
	u_short length;			/* number of frags */
    } nbs;
};

#define WS_NEXT_NEW_BLOCK(s) \
    (afsl_Assert((s)->new.b.next < (s)->new.b.n), \
     (s)->new.b.blocks[(s)->new.b.next])
#define WS_USE_NEW_BLOCK(s) \
    ((s)->new.b.next++)

/* whichPage - indicates the page that is to be empty */
#define WS_DUMMY_UP_EMPTY_BMAP(s /* struct writeState *s */, whichPage) \
    ((s)->bmap.loc.tree = EPIX_DUMMY_BLOCK_LOCATION, \
     (s)->bmap.first = (whichPage), \
     (s)->bmap.n = 1, \
     (s)->bmap.blocks[0] = EPIX_ANODE_BLOCKEMPTY)

#define WS_IS_DUMMY_BMAP(s /* struct writeState *s */) \
    ((s)->bmap.loc.tree == EPIX_DUMMY_BLOCK_LOCATION)

/* InitWrite -- Initializes the writeState structure associated with the
 *     parameters passed to epia_Write.  This structure, described above, is
 *     used by the various procedures involved with doing a write.
 *     Principally, see AllocateStorage, InitializeStorage, WriteData and
 *     FinishUp. */

/* for some reason the pmax compiler freaks out if the first parameter here is
 * called "trans"; a symbol table bug when compiling -g? */

static long InitWrite(
  IN elbb_tranRec_t transaction,
  IN epi_anode_t h,
  IN struct uio *uio,
  OUT struct writeState *s)
{
    long code;
    struct epia_info info;

    epix_EnterCodeBlock(14);
    bzero ((caddr_t)s, sizeof (*s));
    s->trans = transaction;
    s->h = h;
    s->uio = uio;
#ifdef	AFS_AIX31_VM
    if (uio->osi_uio_seg == UIO_XMEM) {
	uio->osi_uio_seg = OSI_UIOSYS;
	s->flags |= WS_XMEM;
    }
#endif

    s->length = uio->osi_uio_resid;
    s->offset = uio->osi_uio_offset;
    s->device = epia_AnodeDevice (h);
    s->bsize = s->device->blkSize;
    s->fsize = epia_FragmentSize(h);
    s->fragsPerBlock = div_l2(s->bsize, s->device->logFragSize);
    s->firstPage = div_l2(s->offset, s->device->logBlkSize);
    s->newLength = s->offset + s->length;
    if (s->newLength == 0) s->lastPage = 0;
    else if (s->length == 0) s->lastPage = s->firstPage;
    else s->lastPage = div_l2(s->newLength-1, s->device->logBlkSize);
    s->remaining = s->length;

    code = epia_GetInfo (h, &info);
    if (code) return code;
    if (info.copies != 0) return EPI_E_OUTSTANDINGCOPIES;
    /* The 32 bit restriction should be relatively easy to fix up ... */
    if (!AFS_hfitsinu32(info.length)) return EPI_E_BADARGS;
    s->oldLength = AFS_hgetlo(info.length);
    s->curLength = s->oldLength;
    s->gapPage = s->oldLength >> s->device->logBlkSize;
    if ((s->oldLength < s->offset) && (mod_2(s->oldLength, s->bsize) != 0))
	/* Don't actually allocate the gapPage just to zero it completely.
         * We'll still zero it if we allocate it to write uio data. */
	s->flags |= WS_ZERO_GAP;

    /* If logging isn't enabled yet, just pretend to use (the zero) trans for
     * regular and metadata updates and so neither start nor end transactions
     * here. */
    if (elbb_IsZeroTrans (transaction) && epia_DeviceLogged(h))
	s->flags |= WS_NEW_TRANS;
    if (info.flags & EPIA_CFLAGS_COPYONWRITE)
	s->flags |= WS_COPYONWRITE;	/* but see frag'd files below */
    if (info.flags & EPIA_CFLAGS_LOGGED) {
	epix_EnterCodeBlock(15);
	if (s->flags & WS_NEW_TRANS) return EPI_E_NOTRANS;
	s->dataLog = !elbb_IsZeroTrans(transaction); /* log regular data */
	s->type = elbb_bufLogged;
    } else {
	s->dataLog = 0;			/* don't log regular data */
	s->type = elbb_bufData;
    }
    s->metaTrans = transaction;

    /* Decide what data storage method we should use */
    if (info.flags & EPIA_CFLAGS_INLINE) s->isAlloc = WS_INLINE;
    else if (info.flags & EPIA_CFLAGS_FRAGMENTED) s->isAlloc = WS_FRAGMENTED;
    else if (info.flags & EPIA_CFLAGS_BLOCKED) s->isAlloc = WS_BLOCKED;
    else s->isAlloc = WS_EMPTY;
#if 0
    /* We should think about this more. */
    afsl_Assert ((s->isAlloc == WS_EMPTY) == (s->oldLength == 0));
#endif
    s->beAlloc = WS_EMPTY;
    if (s->newLength <= s->oldLength) {
	/* If length isn't extended don't change storage allocation type */
	s->beAlloc = s->isAlloc;
	s->maxLength = s->oldLength;
    } else
	s->maxLength = s->newLength;
    if (s->beAlloc == WS_EMPTY) {
	s->flags |= WS_GROW;
	/* Prefer inline if small enough.  If fragment and blocked use same
         * amount of space, prefer blocked. */
#if DEBUG_CHECK_MAXINLINE_SIZE
	s->maxInlineSize = epix_GetMaxInlineData(h);
	if ((s->maxLength > sizeof(s->inlineData)) &&
	    (s->maxLength < s->maxInlineSize)) {
	    afsl_Trace (AFSL_TR_UNUSUAL,
			("Punting inline too soon, %d < %d <= %d",
			 sizeof(s->inlineData), s->maxLength,
			 s->maxInlineSize));
	}
#endif
	if (s->maxLength < sizeof(s->inlineData)) {
	    s->maxInlineSize = epix_GetMaxInlineData(h);
	    if (s->maxLength <= s->maxInlineSize)
		s->beAlloc = WS_INLINE;
	}
	if (s->beAlloc == WS_EMPTY) {
	    if (s->maxLength > s->bsize - s->fsize)
		s->beAlloc = WS_BLOCKED;
	    else s->beAlloc = WS_FRAGMENTED;
	}
    }
    afsl_Assert (s->beAlloc != WS_EMPTY);
    /* invalidate unallocated storage */
    if (s->beAlloc == WS_FRAGMENTED) {
	s->new.f.block = EPIX_ANODE_BLOCKEMPTY;
	s->new.f.length = 0;
    } else if (s->beAlloc == WS_BLOCKED) {
	s->new.b.next = 1;
	s->new.b.n = 0;
    }

    if ((s->isAlloc != WS_EMPTY) && (s->isAlloc != s->beAlloc))
	s->flags |= WS_PROMOTE;

    /* truncate first - when promoting down from blocked to inline or
     * fragmented we must be careful to neither implicitly nest transactions
     * nor to exceed the limit on activity in a single transaction (especially
     * block frees).  We assume here that the only case where a container is
     * down promoted is where it has a single block allocated to it.  Down
     * promotion is triggered by increasing the length of file that is blocked
     * to a size where it can still be stored inline or as a fragmented
     * container.  Thus we require that when a large blocked file has its
     * length set smaller than a block it also must have all its blocks except
     * the first freed. */
    afsl_PAssert (!(((s->beAlloc == WS_INLINE) ||
		     (s->beAlloc == WS_FRAGMENTED)) &&
		    (s->isAlloc == WS_BLOCKED)) ||
		  (info.allocated <= s->fragsPerBlock),
		  ("InitWrite: truncate first - blocked container too large to truncate"));

    /* It is difficult to allocate blocks for a blocked container without
     * beginning the insertion of indirect blocks.  This means that even just
     * allocating blocks will overwrite the fragment or inline information in
     * the anode.  So, before allocation, we need to save the current state of
     * the anode which will be overwritten once we start inserting indirect
     * blocks.  In addition, it will be convenient, when promoting from INLINE
     * to other storage methods, to have the inline data available. */
    if (s->isAlloc == WS_INLINE) {
	afsl_Assert (sizeof(s->inlineData) >= s->oldLength);
	code = epix_GetInline (s->h, 0, s->oldLength, OSI_UIOSYS,
			       s->inlineData);
	if (code) return code;
	/* make sure maxInlineSize is set to something reasonable. */
	s->maxInlineSize = MAX (s->maxInlineSize, s->maxLength);
    }
    if (s->isAlloc == WS_FRAGMENTED) {
	code = epix_GetFragInfo (s->h, &s->frags);
	if (code) return code;
	if (!EPIX_IsBlockInBacking(s->frags.block)) {
	    /* If an anode is COW and its fragmented block has been reallocated
             * then, although it may still have a backing container, it isn't
             * COW as far as we are concerned.  */
	    s->flags &= ~WS_COPYONWRITE;
	}
    }

    s->flags |= WS_START_XFER;		/* thisPage <- firstPage */
    s->thisPage = 0;

    s->bmap.first = 0;
    s->bmap.n = 0;

    s->proc = epia_GetQuotaProc (h);
    return 0;
}

/* ReleaseBuf -- releases the buffer, if any, with the proper wbmode.  It is
 *     called from AdvancePage the first n-1 times and from FinishUp the last
 *     time. */

static long ReleaseBuf(IN struct writeState *s)
{
    long code;
    enum elbb_WBMode mode;

    if (!s->buf) return 0;

    if (s->newBlock)  {
	/* Now that we've made all the modifications we're going to we can
         * attach the undo-zero record.  If we do this any earlier, the logbuf
         * package might clean the buffer and end the undo-zero trans before we
         * actually get the new data into the buffer.  We must do this before
         * we end the transaction that allocated the storage. */
	if (s->flags & WS_NEW_TRANS) {
	    /* This is only necessary for user data. */
	    afsl_Assert (!(s->flags & WS_NEW_TRANS) ||
			 (s->metaTrans != elbb_zeroTrans));
	    elbb_UndoZeroBuffer(s->buf, s->nbs.first*s->fsize,
				s->nbs.length*s->fsize,
				s->metaTrans, &s->buf);
	}
	s->newBlock = 0;
    }

    mode = elbb_lazy;
    if ((s->flags & WS_UNBUFFERED)
	&& (s->beAlloc == WS_BLOCKED))
	mode = elbb_sdel;
    code = elbb_Release (s->buf, mode);
    s->buf = 0;
    return code;
}

/* AdvancePage -- increases thisPage variable according to the various
 *     requirements for promotion, zeroing and data copying.  See
 *     SetPageRegions for details.  This should be called at the beginning of
 *     AllocateStorage which needs thisPage set correctly. */

static long AdvancePage(IN struct writeState *s)
{
    long code;

    epix_EnterCodeBlock(36);
    afsl_Assert (s->thisPage <= s->lastPage);

    /* Before changing thisPage, punt previous buffer, if any. */
    code = ReleaseBuf (s);
    if (code) return code;

    if (s->flags & (WS_PROMOTE /* | WS_RELOCATE_FRAGS */))
	/* These operations are constrained to operate on less than a block.
         * Actually RELOCATE isn't calculated until later in AllocateStorage,
         * but since all fragments are constrained to operate in page zero this
         * should work because firstPage will be zero anyway. */
	s->thisPage = 0;
    else if (s->flags & WS_ZERO_GAP)
	/* arrange to zero region between oldLength and beg. of this xfer. or
         * the end of the page which ever comes first. */
	s->thisPage = s->gapPage;
    else if (s->flags & WS_START_XFER)
	s->thisPage = s->firstPage;	/* first xfer page */
    else s->thisPage++;			/* next xfer page */

    if (s->thisPage == s->gapPage) s->flags &= ~WS_ZERO_GAP;
    if (s->thisPage == s->firstPage) s->flags &= ~WS_START_XFER;
    return 0;
}

static long GetMetaTrans(INOUT struct writeState *s)
{
    long code;
    if (!(s->flags & WS_NEW_TRANS) || !elbb_IsZeroTrans(s->metaTrans))
	return 0;
    epix_EnterCodeBlock(4);
    code = epia_StartTran ("epia_Write", s->h, &s->metaTrans);
    return code;
}

/* epia_UseQuota -- update volume quota. */

/* SHARED */
long epia_UseQuota(
  epi_anode_t h,
  elbb_tranRec_t trans,
  epia_quota_t delta)
{
    long code = 0;
    epia_qproc_t proc = epia_GetQuotaProc(h);
    if (proc != NULL) {
	epix_EnterCodeBlock(37);
	code = (*proc)(trans, h, QUOTA_UPDATE, delta);
	epix_ConditionalCodeBlock(code == EPI_E_VOLUMEFULL, 9);
	/* how could this happen? */
	epix_ConditionalCodeBlock(code == EPI_E_AGGREGATEFULL, 10);
    }
    return code;
}

/* DoInserts -- perform actual insert operation on anode.
 *
 * CAUTIONS -- We can call epia_Truncate here.  Truncate sometimes calls
 *     epia_Write which would raise the possibility of a circularity.  However,
 *     it only calls epia_Write when growing the length, and here we are
 *     setting the length to zero. */

static long DoInserts(INOUT struct writeState *s)
{
    long code;

    if (!(s->flags & WS_SOME_INSERTS))
	return 0;

    epix_EnterCodeBlock(38);
    afsl_Assert (!(s->flags & WS_PROMOTE));
    if (s->flags & WS_TRUNCATE_FIRST) {
	afs_hyper_t zero;
	epix_EnterCodeBlock(39);
	AFS_hzero (zero);
	/* truncate first - We must use the metaTrans because to use trans
         * would risk implicit transaction nesting.  This should be OK because
         * there is supposed to be only a single block allocated to this
         * container. */
	code = epia_Truncate (s->metaTrans, s->h, 0L, zero);
	if (code) return code;
	s->flags &= ~WS_TRUNCATE_FIRST;
	s->isAlloc = WS_EMPTY;
	s->curLength = s->oldLength = 0;
    }
    if (s->pendingInsertQuota) {
	epia_quota_t delta;
	delta.v = s->pendingInsertQuota;
	delta.a = s->pendingInsertAlloc;
	code = epia_UseQuota(s->h, s->metaTrans, delta);
	afsl_MBZ (code);
	s->pendingInsertQuota = s->pendingInsertAlloc = 0;
    }
    if (s->flags & WS_FREE_FRAGS_FIRST) {
	/* we better have copied the data already */
	epix_EnterCodeBlock(40);
	if (s->flags & WS_RELOCATE_FRAGS)
	    return EPI_E_BADWRITESTATE;
	/* must have already adjusted quota */
	afsl_Assert ((s->isAlloc == WS_FRAGMENTED) &&
		     epix_IsBlockAllocated(s->frags.block) &&
		     (s->frags.length > 0));
	code = epib_FreeFrags (s->metaTrans, s->h, /*keep*/0, &s->frags);
	afsl_MBZ (code);
	s->flags &= ~WS_FREE_FRAGS_FIRST;
	s->curLength = s->oldLength = 0;
    }
    switch (s->beAlloc) {
      case WS_INLINE:
	epix_EnterCodeBlock(41);
	code = GetMetaTrans (s);
	if (code) return code;
	code = epix_InsertInline (s->metaTrans, s->h, 0, s->maxLength,
				  s->inlineData);
	if (code) return code;
	s->curLength = s->maxLength;
	s->isAlloc = WS_INLINE;
	break;

      case WS_FRAGMENTED:
	epix_EnterCodeBlock(42);
	afsl_Assert(s->new.f.length > 0);
	afsl_Assert(
	    !(s->flags & WS_NEW_TRANS) || !elbb_IsZeroTrans(s->metaTrans));
	code = epix_InsertFragments (s->metaTrans, s->h, &s->new.f);
	if (code) return code;
	s->curLength = 1 + (s->new.f.length - 1) * s->fsize;
	s->isAlloc = WS_FRAGMENTED;
	break;

      case WS_BLOCKED:
	epix_EnterCodeBlock(43);
	if (s->bmap.n == 0) break;
	epix_EnterCodeBlock(44);
	afsl_Assert(
	    !(s->flags & WS_NEW_TRANS) || !elbb_IsZeroTrans(s->metaTrans));
	code = epix_Insert (s->metaTrans, s->h, s->bmap.n, s->bmap.blocks,
			    &s->bmap.loc);
	if (code) return code;
	s->isAlloc = WS_BLOCKED;
	break;

      default:
	panic("illegal beAlloc");
    }
    s->flags &= ~WS_SOME_INSERTS;
    return 0;
}

/* GetSomeBlocks -- is called to make sure there are enough allocated blocks to
 *     fulfill a need for a block.  A macro, WS_NEXT_NEW_BLOCK, actually
 *     extracts the next block from the preallocated batch.  This routine
 *     returns immediately if there are allocated blocks otherwise it allocates
 *     a new group. */

static long GetSomeBlocks(INOUT struct writeState *s)
{
    long code;
    u_long n;

    if (s->new.b.next < s->new.b.n) return 0;

    if ((s->flags & WS_NEW_TRANS) &&
	(s->totalNew >= MAX_BLKS_ALLOC_IN_ONE_TRANS)) {
	/* Get a new transaction every so often.  Before ending previous,
         * finish up ongoing work. */
	afs_hyper_t length;
	epix_EnterCodeBlock(6);
	afsl_Assert (!elbb_IsZeroTrans (s->metaTrans));
	code = DoInserts (s);
	if (code != 0)
	    return code;
	AFS_hset64(length, 0, s->curLength);
	code = epix_SetLength(s->metaTrans, s->h, length, 1);
	if (code != 0)
	    return code;
	code = epia_EndTran (s->h, s->metaTrans);
	s->metaTrans = elbb_zeroTrans;
	if (code) return code;
	s->totalNew = 0;
    }

    /* We need to calculate how many blocks to allocate.  We want to do this
     * conservatively so that we don't have to free any blocks on non-error
     * exits.  We do this by examining the block addresses obtained earlier
     * from FindBlocks and counting one for each unallocated block.  If this
     * turns out to be too small, all subsequent calls to GetSomeBlocks will
     * timidly only allocate blocks one at time.  This appears to be very
     * unfortunate if a single call to epia_Write would modify many times
     * WS_ARRAYSIZE (== 10) blocks as only the first would actual do bulk
     * allocation. */

    if (s->bmap.n == 0 || s->bmap.first != s->firstPage)
	n = 1;
    else {
	int i, numPages;
	epix_EnterCodeBlock(7);
	afsl_Assert ((s->bmap.first == s->firstPage) &&
		     (s->firstPage <= s->lastPage));
	numPages = MIN(s->bmap.n, s->lastPage - s->firstPage + 1);
	n = 0;
	for (i = s->thisPage - s->firstPage; i < numPages; i++) {
	    if (!epix_IsBlockAllocated(s->bmap.blocks[i])) n++;
	}
    }
    epix_EnterCodeBlock(8);
    afsl_Assert(n != 0 && n <= WS_ARRAYSIZE);
    code = GetMetaTrans (s);
    if (code != 0)
	return code;
    code = epib_Allocate(s->metaTrans, s->h, &n, s->new.b.blocks, 
			 ((s->flags & WS_NOWITHHOLD)? EPIB_DONT_WITHHOLD:0));
    if (code != 0)
	return code;
    s->new.b.n = n;
    s->new.b.next = 0;
    s->totalNew += n;			/* new trans created periodically */

    return code;
}

/* GetIndirectBlkChain - Determine the indirect block chain leading
 *  to the block described by s->bmap.first. If any of the indirect
 *  block is not allocated, allocate them. */

static long GetIndirectBlkChain(INOUT struct writeState *s)
{
    long code;

    /* FindBlocks will change s->bmap structure which DoInserts relies on.
     * Hence we need to have called DoInserts before calling FindBlocks,
     * to insert any outstanding blocks. And we did this in AllocateStorage
     * already */

    /* Priming call to FindBlocks */
    s->bmap.first = s->thisPage;
    code = epix_FindBlocks (s->h, s->bmap.first, WS_ARRAYSIZE, 0,
			    s->bmap.blocks, &s->bmap.n,
			    (u_long *)0, &s->bmap.loc);
    if (code) return code;
    afsl_Assert (s->bmap.n <= WS_ARRAYSIZE);

    if (EPIX_IsLocNonTerminalIndirect(&s->bmap.loc)) {
	int numIBlkNeeded;

	/* Determine the number of blocks we will need to allocate to
	 * populate the indirect chain. */

	numIBlkNeeded = s->bmap.loc.tree + 1 - s->bmap.loc.depth;

	/* Add 1 to numIBlkNeeded to account for atleast 1 data block.
	 * If these many blocks cannot be allocated in the current transaction
	 * set conditions to start a new one */

	if ((numIBlkNeeded + 1 > s->new.b.n - s->new.b.next) &&
	    (numIBlkNeeded + 1 >
	     MAX_BLKS_ALLOC_IN_ONE_TRANS - s->totalNew )) {

	    s->totalNew = NEW_TRANS_FOR_BLK_ALLOC;  /*Start a new transaction*/
	}

	/* The following loop should be repeated "numIBlkNeeded" times */
	do {
	    daddr_t pb;
	    unsigned long thisPageLen; /* Length till end of "thisPage" */
	    epia_quota_t delta;

	    epix_EnterCodeBlock(17);
	    code = GetSomeBlocks (s);
	    if (code) return code;
	    delta.a = s->fragsPerBlock;
	    if (epix_IsBlockEmpty(s->bmap.blocks[0]))
		delta.v = delta.a;
	    else delta.v = 0;
	    code = epia_UseQuota(s->h, s->metaTrans, delta);

	    if (code) return code;
	    pb = WS_NEXT_NEW_BLOCK(s);
	    code = epix_Insert (s->metaTrans, s->h, 1, &pb, &s->bmap.loc);
	    afsl_MBZ (code);
	    /* The newLength could end anywher in the lastPage */
	    thisPageLen = (s->thisPage + 1) * s->bsize;
	    afsl_Assert(s->thisPage * s->bsize < s->newLength);
	    if (thisPageLen > s->curLength) {
		s->curLength = MIN(thisPageLen, s->newLength);
	    }
	    WS_USE_NEW_BLOCK(s);	/* consume */

	    s->bmap.first = s->thisPage;
	    code = epix_FindBlocks (s->h, s->bmap.first, WS_ARRAYSIZE, 0,
				    s->bmap.blocks, &s->bmap.n,
				    (u_long *)0, &s->bmap.loc);
	    if (code) return code;
	    afsl_Assert (s->bmap.n <= WS_ARRAYSIZE);
	} while (EPIX_IsLocNonTerminalIndirect(&s->bmap.loc));
    }
    return 0;
}

/* AllocateStorage -- does some preallocation to simpilify the job of
 *     InitializeStorage.  It does not actually allocate blocks in the common
 *     case.  It handles deallocation when a container is being promoted.  It
 *     tries to extend existing fragments. The rest of the work is done in
 *     InitializeStorage. */

static long AllocateStorage(INOUT struct writeState *s)
{
    long code;

    epix_EnterCodeBlock(45);

    switch (s->beAlloc) {
      default:
	panic("illegal beAlloc");
	/* NOTREACHED */

      case WS_INLINE:
	epix_EnterCodeBlock(16);
	if (s->isAlloc == WS_FRAGMENTED) {
	    epia_quota_t delta;
	    afsl_Assert (s->frags.length > 0);
	    delta.v = -s->frags.length;
	    if (epix_IsBlockAllocated(s->frags.block)) {
		s->flags |= (WS_FREE_FRAGS_FIRST | WS_SOME_INSERTS);
		delta.a = delta.v;
	    } else delta.a = 0;
	    code = GetMetaTrans (s);
	    if (code) return code;
	    code = epia_UseQuota(s->h, s->metaTrans, delta);
	    afsl_MBZ (code);		/* deltas are both negative */
	} else if (s->isAlloc == WS_BLOCKED)
	    s->flags |= (WS_TRUNCATE_FIRST | WS_SOME_INSERTS);
	break;

      case WS_FRAGMENTED:
	epix_EnterCodeBlock(46);
	{   u_int nfrags;		/* number frags needed by container */
	    u_int nconsume;		/* newly allocated frags */
	    int maybeInplace = 0;	/* attempt to grow frags in-place */
	    epia_quota_t delta;
	    int flags;			/* flags to epib_AllocateFrags */

	    if (!(s->flags & (WS_PROMOTE|WS_GROW|WS_COPYONWRITE))) {
		afsl_Assert (s->frags.length > 0);
		s->new.f = s->frags;
		break;
	    }

	    nfrags = ceil_l2(s->maxLength, s->device->logFragSize);
	    afsl_Assert ((nfrags != 0) && (nfrags < s->fragsPerBlock));

	    if (s->isAlloc == WS_BLOCKED) {

		/* Do down promote in place (if not COW).  Just free the frags
                 * at the end of the current (and only) block. */

		int keep;		/* for calling FreeFrags */

		epix_EnterCodeBlock(0);
		afsl_Assert ((s->flags & WS_PROMOTE) && (s->thisPage == 0) &&
			     (epia_GetAllocated(s->h) == s->fragsPerBlock));
		s->bmap.first = 0;
		code = epix_FindBlocks (s->h, s->bmap.first, WS_ARRAYSIZE, 0,
				    s->bmap.blocks, &s->bmap.n, (u_long *)0,
				    &s->bmap.loc);
		if (code) return code;
#ifdef AFS_DEBUG
		while (epix_IsBlockEmpty(s->bmap.blocks[s->bmap.n-1]))
		    s->bmap.n--;
		afsl_Assert (s->bmap.n == 1);
#endif
		if (EPIX_IsBlockInBacking(s->bmap.blocks[0])) {
		    /* Since this is COW we use the regular approach. */
		    s->flags |= WS_TRUNCATE_FIRST;
		    goto just_alloc_frags;
		}

		code = GetMetaTrans (s);
		if (code) return code;

		/* dummy up the equivalent fragment description and free
                 * trailing fragments. */

		s->new.f.block = s->bmap.blocks[0];
		s->new.f.first = 0;
		s->new.f.length = s->fragsPerBlock;
		keep = nfrags;
		delta.a = delta.v = nfrags - s->new.f.length;

		code = epib_FreeFrags (s->metaTrans, s->h, keep, &s->new.f);
		afsl_MBZ (code);

		/* Now blow the existing block away to prepare for later
                 * call to insert fragments. */

		code = epix_Insert (s->metaTrans, s->h, -1, s->bmap.blocks,
				    &s->bmap.loc);
		afsl_MBZ (code);
		s->isAlloc = WS_EMPTY;	/* except length isn't zero... */
		s->flags &= ~WS_PROMOTE; /* no need for any data copying */

		code = epia_UseQuota(s->h, s->metaTrans, delta);
		afsl_MBZ (code);	/* always negative */

		/* Ignore reserved stuff. */
		s->flags |= WS_SOME_INSERTS;
		break;
	    }

just_alloc_frags:

	    afsl_Assert (s->new.f.length == 0); /* none allocated yet */

	    if ((s->isAlloc == WS_FRAGMENTED) &&
		!(s->flags & WS_COPYONWRITE)) {
		epix_EnterCodeBlock(47);
		afsl_Assert (epix_IsBlockAllocated (s->frags.block));
		if (nfrags == s->frags.length) {
		    /* no new frags needed */
		    s->new.f = s->frags;
		    break;
		}
		maybeInplace = EPIB_EXTEND_FRAGS;
	    }

	    code = GetMetaTrans (s);
	    if (code) return code;
	    flags = maybeInplace | EPIB_MOVE_FRAGS |
		((s->flags & WS_NOWITHHOLD) ? EPIB_DONT_WITHHOLD : 0);
	    code = epib_AllocateFrags (s->metaTrans, s->h, flags, nfrags,
				       &s->new.f);
	    if (code) {
		s->flags &= ~WS_GROW;	/* don't set the length */
		return code;
	    }
	    nconsume = nfrags;
	    s->nbs.first = s->new.f.first; /* assume whole group is new */
	    s->nbs.length = s->new.f.length;
	    if (s->isAlloc == WS_FRAGMENTED) {
		if (maybeInplace) nconsume = nfrags - s->frags.length;
		afsl_Assert (nconsume != 0);
		if ((s->frags.block == s->new.f.block) &&
		    (s->frags.first == s->new.f.first)) {
		    /* Just extend current fragment group.  Only the new part
                     * needs undo-zero treatment. */
		    afsl_Assert(maybeInplace && !(s->flags & WS_COPYONWRITE));
		    s->nbs.first = s->frags.first+s->frags.length;
		    s->nbs.length = nconsume;
		} else {
		    epix_EnterCodeBlock(48);
		    s->flags |= WS_RELOCATE_FRAGS;
		    /* Make sure we're still on page zero, otherwise too late
                     * for relocation.  However, since any fragmented container
                     * only contains one page this shouldn't be a problem. */
		    afsl_Assert (s->thisPage == 0);
		    /* free old frag group if we're just moving it, but don't
                     * actually do the free until its data has been copied into
                     * the new frag group, otherwise another process may
                     * allocate and use them.  We do not hold a global
                     * allocation lock during a write. */
		    if (maybeInplace) s->flags |= WS_FREE_FRAGS_FIRST;
		}
	    }

	    /* consume/check quota, and give back frags if we fail. */

	    delta.a = nconsume;
	    delta.v = nfrags - s->frags.length;
	    code = epia_UseQuota(s->h, s->metaTrans, delta);
	    if (code) {
		long lclCode;
		int keep;
		if ((s->isAlloc != WS_FRAGMENTED) ||
		    (s->flags & WS_RELOCATE_FRAGS))
		    keep = 0;		/* get rid of all new frags */
		else keep = s->frags.length; /* keep the old ones */
		lclCode = epib_FreeFrags (s->metaTrans, s->h, keep, &s->new.f);
		afsl_MBZ (lclCode);
		s->flags &= ~WS_GROW;	/* don't set the length */
		return code;
	    }

	    /* With fragmented storage there's not much point in allowing
             * backout, so we commit immediately.  Also note that we will need
             * to use undo-zero trans. */

	    s->newBlock = 1;
	    s->flags |= WS_SOME_INSERTS;
	}
	break;

      case WS_BLOCKED:
	epix_EnterCodeBlock(49);
	if ((s->thisPage >= s->bmap.first) &&
	    (s->thisPage < s->bmap.first + s->bmap.n)) {
	    /* the current page has already been mapped */
	    return 0;
	}

	/* We need to perform the DoInserts HERE as if we are promoting
	 * INLINE or FRAGMENTED to BLOCKED, as we do not want s->isAlloc
	 * to remain INLINE/FRAGMENTED for the 2nd block (thisPage=1),
	 * if any. Also, since we perform the DoInserts here, when isAlloc
	 * is BLOCKED, we need not call DoInserts in GetIndirectBlkChain,
	 * called later from this function */

	code = DoInserts (s);
	if (code) return code;

	if ((s->isAlloc == WS_INLINE) || (s->isAlloc == WS_FRAGMENTED)) {
	    epix_EnterCodeBlock(50);
	    afsl_Assert (s->thisPage == 0);

	    /* special case result of FindBlocks if anode isn't currently using
             * BLOCKED storage */
	    WS_DUMMY_UP_EMPTY_BMAP(s, 0 /* indicates page that is empty */);
	    EPIX_MakeDirectBlockLoc (&s->bmap.loc, s->thisPage);

	    if (s->isAlloc == WS_FRAGMENTED) {
		struct epix_fragDescription frags;

		epix_EnterCodeBlock(51);
		code = GetMetaTrans (s);
		if (code) return code;

		/* Check to see if extending the current fragment into a whole
                 * block is possible.  Namely, that the current group starts at
                 * the beginning of the block and is not a backing pointer. */

		if ((s->frags.first != 0) ||
		    !epix_IsBlockWritable (s->frags.block))
		    code = EPI_E_CANNOTEXTEND;
		else {
		    int flags = EPIB_EXTEND_FRAGS |
			((s->flags & WS_NOWITHHOLD) ? EPIB_DONT_WITHHOLD : 0);

		    /* try to grow current fragment group into a whole block */

		    epix_EnterCodeBlock(92);
		    code = epib_AllocateFrags(s->metaTrans, s->h, flags,
					      s->fragsPerBlock, &frags);
		    if (code && (code != EPI_E_CANNOTEXTEND)) {
			s->flags &= ~WS_GROW; /* don't set the length */
			return code;
		    }
		    if (code == 0) {

			/* Note that we don't want to set the flags bit
                         * SOME_INSERTS until the quota check is successful or
                         * DoInserts (invoked during cleanup) will fail.  Also
                         * we must free the new fragment(s) in this case as
                         * well. */

			int fragDelta = s->fragsPerBlock - s->frags.length;
			epia_quota_t delta;
			epix_EnterCodeBlock(52);
			s->bmap.blocks[0] = frags.block;

			delta.a = delta.v = fragDelta;
			code = epia_UseQuota(s->h, s->metaTrans, delta);
			if (code) {
			    long lclCode;
			    lclCode = epib_FreeFrags (s->metaTrans, s->h,
						      /*keep*/s->frags.length,
						      &frags);
			    afsl_MBZ (lclCode);
			    s->flags &= ~WS_GROW; /* don't set the length */
			    return code;
			}

			s->nbs.first = s->frags.length;
			s->nbs.length = (u_short)fragDelta;
			s->newBlock = 1;
			s->flags |= (WS_SOME_INSERTS | WS_GROW_IN_BLOCK);
		    }
		}
		if (code == EPI_E_CANNOTEXTEND) {

		    /* The InitializeStorage routine will copy the frag data to
                     * first block */

		    epix_EnterCodeBlock(93);
		    code = 0;
		    s->flags |= WS_RELOCATE_FRAGS;
		    s->pendingInsertQuota = -s->frags.length;
		    if (epix_IsBlockWritable (s->frags.block)) {
			/* If this should fail for some reason, don't free the
                         * fragments on exit. */
			s->pendingInsertAlloc = s->pendingInsertQuota;
			s->flags |= WS_FREE_FRAGS_FIRST;
		    }
		}
	    }
	} else { /* BLOCKED or EMPTY */
	    /* The aim is to avoid allocating a block if its only going
	     * be "bzeroed". Also, avoid allocating any empty indirect
	     * blocks leading to this data block. */

	    if (s->length) {
		/* Real I/O expected */
		code = GetIndirectBlkChain(s);
		if (code) return code;
	    } else if (s->oldLength > s->thisPage * s->bsize &&
		       s->oldLength <= (s->thisPage + 1) * s->bsize) {
		/* Call FindBlocks in READ mode to determine whether
		 * target block is absent in backing anode too */
		s->bmap.first = s->thisPage;
		code = epix_FindBlocks (s->h, s->bmap.first, WS_ARRAYSIZE,
					1 /* forRead */,
					s->bmap.blocks, &s->bmap.n,
					(u_long *)0, &s->bmap.loc);
		if (code) return code;
		if (!epix_IsBlockEmpty(s->bmap.blocks[0])) {
		    code = GetIndirectBlkChain(s);
		    if (code) return code;
		} else {
		    WS_DUMMY_UP_EMPTY_BMAP(s, s->thisPage);
   		}
	    } else {
		WS_DUMMY_UP_EMPTY_BMAP(s, s->thisPage);
	    }
	}
	break;
    }
    return 0;
}

/* SetPageRegions -- set intra-page offsets to appropriate values.  It should
 *     be called at the beginning of InitializeStorage which needs the offset
 *     and lengths.  It depends on various flags set in InitWrite, on thisPage
 *     set in AdvancePage, and on WS_RELOCATE_FRAGS set in AllocateStorage.
 *
 *     This algorithm changes to be somewhat simple with the requirements of
 *     new block security.  Basically we always want to zero newly allocated
 *     storage that isn't otherwise being overwritten.  This means that we
 *     always use the interior page model described below.  The last page
 *     optimization can not be used.
 *
 * The order of precidence is to PROMOTE/RELOCATE/COPYONWRITE up to the
 * oldLength, then zero from oldLength to offset or all of new pages, then copy
 * uio data.  The uio data occupies a contiguous region from offset to
 * newLength.  The zeroing area is either a region that precedes the uio data
 * from oldLength to offset if oldLength is smaller, or beginning and ending
 * areas that surround the uio data in newly allocated pages.  The promotion
 * region exists if the promotion, relocation or copy-on-write flags are on.
 * It is always below oldLength so will precede the zero area and may follow
 * the uio data.  The promotion area extends from 0 to the oldLength or offset,
 * which ever is less, and from newLength to oldLength if oldLength is greater.
 *
 * Graphically here is the layout of the regions:
 *    interior page                      last page
 * 0                       bsize      0         oldLength            bsize
 * :                       :          :         :                    :
 * +-----------------------+          +---------+---------+----------+
 * | promote(COW) or zero  |          | promote |   zero  : 0 if new |
 * +-----------------------+          +---------+---------+----------+
 *        +--------+                              +-------+
 * <------|  data  |------>           <-----------| data  |---------->
 *        +--------+                              +-------+
 *        :        :                              :       :
 *        offset   newLength                      offset  newLength
 *
 * If an interior page is unallocated and there is a backing block then the
 * promote region occupies the entire block otherwise the zero region occupies
 * the whole block.  In the last page the zero region extends from the old
 * length to offset.  Offset may be on a following page, in which case it
 * would extend only to bsize.  The result of these constraints on the regions
 * is the definition of five areas, which are always in this order but any of
 * which may be empty:
 *   0 => | promoteB | zeroB | uioData | promoteE | zeroE | <= bsize
 */

static long SetPageRegions(IN struct writeState *s)
{
    int promotePage;			/* promote from existing data */
    int newPage;			/* zero instead of promote from COW */
    u_long thisPageBase;		/* byte offset of thisPage */
    u_long curLenInPage;
    u_long maxLenInPage;

    afsl_Assert (s->offset + s->length == s->newLength);
    afsl_Assert (((s->thisPage > s->firstPage ?
	       s->bsize - mod_2(s->offset, s->bsize) +
		 (s->thisPage - 1 - s->firstPage) * s->bsize
	      : 0) + s->remaining) == s->length);

    promotePage = 0;
    newPage = 0;
    if ((s->flags & WS_RELOCATE_FRAGS) ||
	((s->isAlloc == WS_FRAGMENTED) && (s->flags & WS_COPYONWRITE)) ||
	((s->flags & WS_PROMOTE) && (s->isAlloc != WS_FRAGMENTED)))
	/* The above conditions cover a complicated set of circumstances.
	 * First if we are relocating fragments then certainly promotion is
	 * necessary.  Secondly, fragmented containers that are COW also need
	 * promotion (blocked COW is handled below).  Thirdly, if the container
	 * is being promoted, and it isn't fragmented, we'll need to copy the
	 * data (if it is fragmented and not grown in place, then
	 * RELOCATE_FRAGS will be set). */
	promotePage = 1;
    else if ((s->isAlloc == WS_EMPTY) && (s->beAlloc == WS_BLOCKED)) {
	epix_EnterCodeBlock(5);
	newPage = 1;
	promotePage = 0;
    }
    else if (s->isAlloc == WS_BLOCKED) {
	daddr_t old = s->bmap.blocks[s->thisPage - s->bmap.first];
	if (epix_IsBlockEmpty(old)) {
	    epix_EnterCodeBlock(30);
	    newPage = 1;
	    promotePage = 0;
	} else if (EPIX_IsBlockInBacking(old)) {
	    promotePage = 1;
	}
    }

    thisPageBase = s->thisPage * s->bsize;
    if (s->oldLength <= thisPageBase) curLenInPage = 0;
    else curLenInPage = MIN (s->oldLength - thisPageBase, s->bsize);
    afsl_Assert (s->maxLength >= thisPageBase);
    if (newPage || promotePage) switch (s->beAlloc) {
      case WS_INLINE: maxLenInPage = s->maxLength; break;
      case WS_FRAGMENTED: maxLenInPage = s->new.f.length*s->fsize; break;
      case WS_BLOCKED: maxLenInPage = s->bsize; break;
    }
    else maxLenInPage = 0;

    if (s->thisPage == 0) {
	/* fragmented and inline data always fits in a single block */
	epix_EnterCodeBlock(54);
	afsl_Assert ((s->beAlloc == WS_BLOCKED) ||
		((s->beAlloc == WS_FRAGMENTED) &&
		 (s->maxLength <= s->bsize - s->fsize)) ||
		((s->beAlloc == WS_INLINE) &&
		 (s->maxLength <= s->maxInlineSize)));

	/* calculate uio data area */
	if (s->offset < s->bsize) {
	    s->uioDataO = s->offset;
	    s->uioDataL = MIN(s->bsize, s->newLength) - s->offset;
	} else {
	    s->uioDataO = s->bsize;
	    s->uioDataL = 0;
	}

	/* calculate zeroB area */
	s->zeroBO = s->uioDataO;
	if (s->oldLength < s->uioDataO) {
	    s->zeroBO = s->oldLength;
	    s->zeroBL = s->uioDataO - s->zeroBO;
	} else if (newPage) {
	    s->zeroBO = 0;
	    s->zeroBL = s->uioDataO;
	} else s->zeroBL = 0;

	/* calculate promotion area(s) */
	s->promoteBO = s->zeroBO;
	s->promoteEO = s->uioDataO + s->uioDataL;
	if (promotePage) {
	    epix_EnterCodeBlock(55);
	    s->promoteBO = 0;
	    s->promoteBL = s->zeroBO;
	    if (s->oldLength > s->promoteEO) {
		epix_EnterCodeBlock(56);
		s->promoteEL = curLenInPage - s->promoteEO;
	    } else s->promoteEL = 0;
	} else
	    s->promoteBL = s->promoteEL = 0;
    } else { /* other than page 0 */
	epix_EnterCodeBlock(57);
	afsl_Assert (!(s->flags & (WS_PROMOTE | WS_RELOCATE_FRAGS)));
	if (s->thisPage == s->firstPage)
	    s->uioDataO = mod_2(s->offset, s->bsize);
	else if (s->thisPage > s->firstPage)
	    s->uioDataO = 0;
	else s->uioDataO = s->bsize;
	s->uioDataL = MIN (s->remaining, s->bsize - s->uioDataO);

	s->zeroBO = s->uioDataO;
	if (curLenInPage < s->uioDataO) {
	    s->zeroBO = curLenInPage;
	    s->zeroBL = s->uioDataO - s->zeroBO;
	} else if (newPage) {
	    s->zeroBO = 0;
	    s->zeroBL = s->uioDataO - s->zeroBO;
	}
	else s->zeroBL = 0;

	s->promoteBO = s->zeroBO;
	s->promoteEO = s->uioDataO + s->uioDataL;
	if (promotePage) {
	    epix_EnterCodeBlock(58);
	    s->promoteBO = 0;
	    s->promoteBL = s->zeroBO;
	    if (s->promoteEO < curLenInPage) {
		epix_EnterCodeBlock(59);
		s->promoteEL = curLenInPage - s->promoteEO;
	    } else s->promoteEL = 0;
	} else
	    s->promoteBL = s->promoteEL = 0;
    }

    /* calculate zeroE area */
    s->zeroEO = s->promoteEO + s->promoteEL;
    if (maxLenInPage > s->zeroEO) {
	s->zeroEL = maxLenInPage - s->zeroEO;
    } else
	s->zeroEL = 0;

    /* If the page is new or if the only part of the page being written is
     * beyond the container's length then allocate a new buffer instead of
     * reading the irrelevant contents of the block from disk.  If we're going
     * to completely overwrite a block *AND* the container isn't logged, we
     * can also use create. */

    s->createPage = 0;
    if (newPage ||
	((s->beAlloc == WS_BLOCKED) && (s->type != elbb_bufLogged) &&
	 (s->promoteBO == 0) && (s->zeroEO+s->zeroEL == s->bsize)))
	s->createPage = 1;

    /* check basic region constraints */
    afsl_Assert ((s->promoteEL == 0) ||
	    (s->thisPage*s->bsize + s->promoteEO + s->promoteEL
	     <= s->oldLength));
    afsl_Assert ((s->zeroBL == 0) ||
	    (s->thisPage*s->bsize + s->zeroBO + s->zeroBL <= s->offset));

    /* ensure that regions are all contiguous and non-overlapping */
    afsl_Assert ((s->promoteBO + s->promoteBL == s->zeroBO) &&
	    (s->zeroBO + s->zeroBL == s->uioDataO) &&
	    (s->uioDataO + s->uioDataL == s->promoteEO) &&
	    (s->promoteEO + s->promoteEL <=
	     ((s->beAlloc == WS_BLOCKED)
	      ? s->bsize : ((s->beAlloc == WS_FRAGMENTED)
			    ? MIN(s->maxLength, s->bsize - s->fsize)
			    : MIN(s->maxLength, s->maxInlineSize)))) &&
	    (s->promoteEO + s->promoteEL == s->zeroEO) &&
	    (s->zeroEO + s->zeroEL <= s->bsize));
    return 0;
}

/* InitializeStorage - It allocates indirect blocks, any data blocks etc */

static long InitializeStorage(INOUT struct writeState *s)
{
    long code;
    daddr_t old, new;
    long bufferReservation = 0;
    elbb_WBMode_t bpmode;
    unsigned long thisPageLen;

    epix_EnterCodeBlock(60);
    code = SetPageRegions(s);
    if (code) return code;
    if (!(s->promoteBL||s->zeroBL||s->uioDataL||s->promoteEL||s->zeroEL))
	return 0;

    if (s->flags & WS_UNBUFFERED)
	bpmode = elbb_sdel;
    else
	bpmode = elbb_lazy;

#define GetReservationIfNeeded() \
do { \
    /* We look-ahead to see if a reservation to the buffer system \
       is  necessary - must be done before read call */ \
    if (((s->isAlloc == WS_FRAGMENTED) || \
	 (s->isAlloc == WS_BLOCKED)) && \
	(s->promoteEL || s->promoteBL)) { \
	/* reservation required */ \
	afsl_MBZ(bufferReservation); /* nobody else has reserved yet */ \
	bufferReservation = 2; /* src, dest */ \
	elbb_Reserve(bufferReservation, s->device->blkSize); \
    } \
} while(0)

#define MustReleaseReservation() \
do { \
    afsl_Assert(bufferReservation); /* we did reserve, right? */ \
    elbb_Reserve(-(bufferReservation), s->device->blkSize); \
    bufferReservation = 0; /* we've released it */ \
} while (0)

#define ReleaseAnyReservation() \
do { \
    if (bufferReservation) \
	elbb_Reserve(-(bufferReservation), s->device->blkSize); \
    bufferReservation = 0; \
} while (0)

    switch (s->beAlloc) {
      case /* beAlloc */ WS_INLINE:
	epix_EnterCodeBlock(61);
	s->flags |= WS_SOME_INSERTS;
	if (s->zeroBL)
	    bzero (s->inlineData + s->zeroBO, s->zeroBL);
	afsl_Assert (s->zeroEL == 0);	/* never pad for inline files */
	if ((s->promoteEL == 0) && (s->promoteBL == 0)) break;
	switch (s->isAlloc) {
	  case /* beAlloc INLINE, isAlloc */ WS_EMPTY:
	  case /* beAlloc INLINE, isAlloc */ WS_INLINE:
	    epix_EnterCodeBlock(62);
	    break;
	  case /* beAlloc INLINE, isAlloc */ WS_FRAGMENTED:
	    epix_EnterCodeBlock(63);
	    old = s->frags.block;
	    if (EPIX_IsBlockInBacking(old)) {
		afsl_Assert (s->flags & WS_COPYONWRITE);
		old = epix_MapBlockToBacking (old);
	    }
	    BEGIN_LCRIT (struct elbb_buffer *bbuf,
			 code = elbb_Read (s->device, old, s->type, &bbuf),
			 code = elbb_Release (bbuf, elbb_lazy)) {
		char *bfragBase = elbb_BufferData(bbuf) +
		    s->frags.first * s->fsize;
		if (s->promoteBL)
		    bcopy (bfragBase + s->promoteBO,
			   s->inlineData + s->promoteBO, s->promoteBL);
		if (s->promoteEL)
		    bcopy (bfragBase + s->promoteEO,
			   s->inlineData + s->promoteEO, s->promoteEL);
	    } END_CRIT;
	    if (code) return code;
	    break;
	  case /* beAlloc INLINE, isAlloc */ WS_BLOCKED:
	    /* The GetBuffer function also handles copy on write */
	    epix_EnterCodeBlock(64);
	    BEGIN_LCRIT (struct elbb_buffer *bbuf,
			 code = epix_GetBuffer (s->h, 0, &bbuf),
			 code = elbb_Release (bbuf, bpmode)) {
		char *page = elbb_BufferData (bbuf);
		if (s->promoteBL)
		    bcopy (page + s->promoteBO,
			   s->inlineData + s->promoteBO, s->promoteBL);
		if (s->promoteEL)
		    bcopy (page + s->promoteEO,
			   s->inlineData + s->promoteEO, s->promoteEL);
	    } END_CRIT;
	    if (code) return code;
	    break;
	  default:
	    panic("illegal isAlloc");
	}
	break;

      case /* beAlloc */ WS_FRAGMENTED:
	if (s->buf) return 0;
	epix_EnterCodeBlock(65);
	afsl_Assert (epix_IsBlockAllocated(s->new.f.block) &&
		(s->new.f.length > 0));
	s->fragOff = s->new.f.first * s->fsize;
	/* We look-ahead to see if a reservation to the buffer system is
	 * necessary - must be done before read call */
	GetReservationIfNeeded();
	code = elbb_Read (s->device, s->new.f.block, s->type, &s->buf);
	if (code) {
	    ReleaseAnyReservation();
	    return code;
	}

	if (s->zeroBL) {
	    code = elbb_Zero (s->buf, s->fragOff+s->zeroBO, s->zeroBL,
			      s->trans, s->dataLog, &s->buf);
	    afsl_MBZ(code);
	}

	if ((s->promoteEL == 0) && (s->promoteBL == 0)) break;

	switch (s->isAlloc) {
	  case /* beAlloc FRAGMENTED, isAlloc */ WS_EMPTY:
	    epix_EnterCodeBlock(66);
	    break;
	  case /* beAlloc FRAGMENTED, isAlloc */ WS_INLINE:
	    epix_EnterCodeBlock(67);
	    if (s->promoteBL) {
		code = elbb_Modify (s->buf, s->fragOff + s->promoteBO,
				    s->inlineData + s->promoteBO, s->promoteBL,
				    s->trans, s->dataLog, &s->buf);
		if (code) {
		    ReleaseAnyReservation();
		    return code;
		}
	    }
	    if (s->promoteEL) {
		code = elbb_Modify (s->buf, s->fragOff + s->promoteEO,
				    s->inlineData + s->promoteEO, s->promoteEL,
				    s->trans, s->dataLog, &s->buf);
		if (code) {
		    ReleaseAnyReservation();
		    return code;
		}
	    }
	    break;
	  case /* beAlloc FRAGMENTED, isAlloc */ WS_FRAGMENTED:
	    epix_EnterCodeBlock(68);
	    if (!(s->flags & WS_RELOCATE_FRAGS)) {
		afsl_Assert (!(s->flags & WS_COPYONWRITE));
		break;
	    }
	    old = s->frags.block;
	    if (EPIX_IsBlockInBacking(old)) {
		afsl_Assert (s->flags & WS_COPYONWRITE);
		old = epix_MapBlockToBacking (old);
	    }
	    BEGIN_LCRIT (struct elbb_buffer *bbuf,
			 code = elbb_Read (s->device, old, s->type, &bbuf),
			 code = elbb_Release (bbuf, elbb_lazy)) {
		char *bfragBase = elbb_BufferData(bbuf) +
		    s->frags.first * s->fsize;
		if (s->promoteBL) {
		    code = elbb_Modify (s->buf, s->fragOff + s->promoteBO,
					bfragBase + s->promoteBO, s->promoteBL,
					s->trans, s->dataLog, &s->buf);
		    if (code) continue;
		}
		if (s->promoteEL) {
		    code = elbb_Modify (s->buf, s->fragOff + s->promoteEO,
					bfragBase + s->promoteEO, s->promoteEL,
					s->trans, s->dataLog, &s->buf);
		    if (code) continue;
		}
	    } END_CRIT;
	    MustReleaseReservation();
	    if (code) return code;
	    break;
	  case /* beAlloc FRAGMENTED, isAlloc */ WS_BLOCKED:
	    /* The GetBuffer function also handles copy on write */
	    epix_EnterCodeBlock(69);
	    BEGIN_LCRIT (struct elbb_buffer *bbuf,
			 code = epix_GetBuffer (s->h, 0, &bbuf),
			 code = elbb_Release (bbuf, bpmode)) {
		char *page = elbb_BufferData (bbuf);
		if (s->promoteBL) {
		    code = elbb_Modify (s->buf, s->fragOff + s->promoteBO,
					page + s->promoteBO, s->promoteBL,
					s->trans, s->dataLog, &s->buf);
		    if (code) continue;
		}
		if (s->promoteEL) {
		    code = elbb_Modify (s->buf, s->fragOff + s->promoteEO,
					page + s->promoteEO, s->promoteEL,
					s->trans, s->dataLog, &s->buf);
		    if (code) continue;
		}
	    } END_CRIT;
	    MustReleaseReservation();
	    if (code) return code;
	    break;
	  default:
	    panic("illegal isAlloc");
	}
	break;

      case /* beAlloc */ WS_BLOCKED:
	if (s->buf) return 0;

	epix_EnterCodeBlock(18);

	afsl_Assert(s->thisPage >= s->bmap.first &&
		    s->thisPage < s->bmap.first + s->bmap.n);

	old = s->bmap.blocks[s->thisPage - s->bmap.first];
	if (epix_IsBlockAllocated(old)) {
	    int needbuf;

	    epix_EnterCodeBlock(19);
	    needbuf = 1;
	    if (s->flags & WS_UNBUFFERED) {
		if (!(s->promoteBL||s->zeroBL||s->promoteEL||s->zeroEL)
		    && !(s->flags & WS_GROW_IN_BLOCK)) {
		    needbuf = 0;
		}
	    }
	    if (needbuf) {
		GetReservationIfNeeded();
		if (s->createPage) {
		    /* on-disk data is uninitialized (or will be completely
		     * overwritten, if unlogged). */
		    code = elbb_CreateBlock (s->device, old, s->type, &s->buf);
		} else {
		    code = elbb_Read (s->device, old, s->type, &s->buf);
		}
		if (code) {
		    ReleaseAnyReservation();
		    return code;
		}
	    }
	} else {
	    /* Need a new block here.  Grab a new block from the list and
	     * create a buffer around it.  Because of freed block code we don't
	     * need to read old contents so call CreateBlock.  Also NBS,
	     * requires a buffer for attaching the undo-zero trans so ignore
	     * needbuf flag we calculated above. */
	    epia_quota_t delta;
	    epix_EnterCodeBlock(70);
	    if (!s->promoteBL && !s->uioDataL && !s->promoteEL) {
		/* don't allocate a page just to write zeros into it */
		return 0;
	    }
	    afsl_Assert(!(WS_IS_DUMMY_BMAP(s))); /* to be paranoid */

	    epix_EnterCodeBlock(76);
	    code = GetSomeBlocks (s);
	    if (code == 0) {
		delta.a = s->fragsPerBlock;
		if (epix_IsBlockEmpty(old))
		    delta.v = delta.a;
		else delta.v = 0;
		delta.v += s->pendingInsertQuota;
		delta.a += s->pendingInsertAlloc;
		s->pendingInsertQuota = s->pendingInsertAlloc = 0;
		code = epia_UseQuota(s->h, s->metaTrans, delta);
	    }
	    if (code) {
		if ((s->isAlloc == WS_INLINE) ||
		    (s->isAlloc == WS_FRAGMENTED)) {
		    /* Don't set the length if we still have the old
                     * organization. */
		    s->flags &= ~WS_GROW;
		}
		return code;
	    }
	    new = WS_NEXT_NEW_BLOCK(s);
	    GetReservationIfNeeded();
	    code = elbb_CreateBlock (s->device, new, s->type, &s->buf);
	    if (code) return code;
	    WS_USE_NEW_BLOCK(s);

	    s->bmap.blocks[s->thisPage - s->bmap.first] = new;
	    s->nbs.first = 0;
	    s->nbs.length = s->fragsPerBlock;
	    s->newBlock = 1;
	    s->flags |= WS_SOME_INSERTS;
	    thisPageLen = (s->thisPage + 1) * s->bsize;
	    afsl_Assert(s->thisPage * s->bsize < s->newLength);
	    if (thisPageLen > s->curLength) {
		s->curLength = MIN(thisPageLen, s->newLength);
	    }
	}

	if (s->buf)
	    elbb_SetFH (s->buf, FsyncMagic(s->h));

	if (s->zeroBL) {
	    code = elbb_Zero (s->buf, s->zeroBO, s->zeroBL,
			      s->trans, s->dataLog, &s->buf);
	    afsl_MBZ(code);
	}
	if (s->zeroEL) {
	    code = elbb_Zero (s->buf, s->zeroEO, s->zeroEL,
			      s->trans, s->dataLog, &s->buf);
	    afsl_MBZ(code);
	}

	if ((s->promoteEL == 0) && (s->promoteBL == 0)) break;

	switch (s->isAlloc) {
	  case /* beAlloc BLOCKED, isAlloc */ WS_EMPTY:
	    epix_EnterCodeBlock(71);
	    break;
	  case /* beAlloc BLOCKED, isAlloc */ WS_INLINE:
	    epix_EnterCodeBlock(72);
	    if (s->promoteBL) {
		code = elbb_Modify (s->buf, s->promoteBO,
				    s->inlineData + s->promoteBO, s->promoteBL,
				    s->trans, s->dataLog, &s->buf);
		if (code) return code;
	    }
	    if (s->promoteEL) {
		code = elbb_Modify (s->buf, s->promoteEO,
				    s->inlineData + s->promoteEO, s->promoteEL,
				    s->trans, s->dataLog, &s->buf);
		if (code) return code;
	    }
	    break;
	  case /* beAlloc BLOCKED, isAlloc */ WS_FRAGMENTED:
	    epix_EnterCodeBlock(73);
	    afsl_Assert(s->thisPage == 0 && s->oldLength <= s->bsize);
	    old = s->frags.block;
	    if (EPIX_IsBlockInBacking(old)) {
		afsl_Assert(s->flags & WS_COPYONWRITE);
		old = epix_MapBlockToBacking (old);
	    }
	    BEGIN_LCRIT (struct elbb_buffer *bbuf,
			 code = elbb_Read (s->device, old, s->type, &bbuf),
			 code = elbb_Release (bbuf, elbb_lazy)) {
		char *bfragBase = elbb_BufferData(bbuf) +
		    s->frags.first * s->fsize;
		if (s->promoteBL) {
		    code = elbb_Modify (s->buf, s->promoteBO,
					bfragBase + s->promoteBO, s->promoteBL,
					s->trans, s->dataLog, &s->buf);
		    if (code) continue;
		}
		if (s->promoteEL) {
		    code = elbb_Modify (s->buf, s->promoteEO,
					bfragBase + s->promoteEO, s->promoteEL,
					s->trans, s->dataLog, &s->buf);
		    if (code) continue;
		}
	    } END_CRIT;
	    MustReleaseReservation();
	    if (code) return code;
	    break;
	  case /* beAlloc BLOCKED, isAlloc */  WS_BLOCKED:
	    /* This clause only handles copy on write */
	    epix_EnterCodeBlock(74);
	    afsl_Assert (!(s->flags & WS_PROMOTE));
	    if (!EPIX_IsBlockInBacking(old)) break;
	    epix_EnterCodeBlock(20);
	    afsl_Assert (s->flags & WS_COPYONWRITE);
	    old = epix_MapBlockToBacking (old);
	    BEGIN_LCRIT (struct elbb_buffer *bbuf,
			 code = elbb_Read (s->device, old, s->type, &bbuf),
			 code = elbb_Release (bbuf, bpmode)) {
		char *page = elbb_BufferData (bbuf);
		if (s->promoteBL) {
		    code = elbb_Modify (s->buf, s->promoteBO,
					page + s->promoteBO, s->promoteBL,
					s->trans, s->dataLog, &s->buf);
		    if (code) continue;
		}
		if (s->promoteEL) {
		    code = elbb_Modify (s->buf, s->promoteEO,
					page + s->promoteEO, s->promoteEL,
					s->trans, s->dataLog, &s->buf);
		    if (code) continue;
		}
	    } END_CRIT;
	    MustReleaseReservation();
	    if (code) return code;
	    break;
	  default:
	    panic("illegal isAlloc");
	}
	break;

      default:
	panic("illegal beAlloc");
    }

    s->flags &= ~(WS_PROMOTE|WS_RELOCATE_FRAGS);
    afsl_MBZ(bufferReservation);  /* unless we exited with an earlier error */
    return 0;
/* do not need this macro anymore */
#undef RelReturn
}

/* WriteData -- actually transfers the uio data.  If the uio io vector is null
 *     then the data is left uninitialized, but the relevant storage is
 *     allocated. */

static long WriteData(INOUT struct writeState *s)
{
    long code;
    u_long xferO = s->uioDataO;
    u_long xferL = s->uioDataL;
    u_long ulen = 0;
    caddr_t uptr;
    struct uio xuio;
    struct iovec xiov;

    if (xferL == 0) return 0;
    epix_EnterCodeBlock(23);

    code = 0;
    if (s->uio->osi_uio_iov) do { /* generally, go around at least twice */
	osi_uiomove_unit (ulen, xferL, s->uio, &uptr, &ulen);
	if (ulen == 0) break;
	epix_EnterCodeBlock(75);
	switch (s->beAlloc) {
	  case WS_INLINE:
	    s->flags |= WS_SOME_INSERTS;
	    epia_InitUio(&xuio, &xiov, s->uio->osi_uio_offset, ulen, uptr);
	    xuio.osi_uio_seg = s->uio->osi_uio_seg;
	    code = osi_uiomove (s->inlineData + xferO, ulen, UIO_WRITE, &xuio);
	    break;
	  case WS_FRAGMENTED:
	    afsl_Assert(s->buf != NULL);
	    code = elbb_ModifySource (s->buf, s->fragOff + xferO, uptr, ulen,
				      s->uio->osi_uio_seg,
				      s->trans, s->dataLog, &s->buf);
	    break;
	  case WS_BLOCKED:
	    afsl_Assert ((s->flags & WS_UNBUFFERED) || s->buf);
	    if (s->buf)
		code = elbb_ModifySource (s->buf, xferO, uptr, ulen,
					  s->uio->osi_uio_seg,
					  s->trans, s->dataLog, &s->buf);
	    else {
		struct asevent_event *aeP = asevent_Create(
			(s->flags & WS_XMEM) ? ASEVENT_NOPIN : 0);
		struct buf *bp = asevent_MakeBuf(
			B_WRITE, uptr, ulen, s->device,
			s->bmap.blocks[s->thisPage - s->bmap.first], xferO);	

#ifdef	AFS_AIX31_VM
		if (s->flags & WS_XMEM)
		    bp->b_xmemd = *s->uio->uio_xmem;
#endif /* AFS_AIX31_VM */
		code = asevent_QueueIO(s->device, bp, aeP);
		if (code == 0)
		    code = asevent_WaitEvent(aeP);
		asevent_Release(aeP);
	    }
	    break;
	  default:
	    panic("illegal beAlloc");
	}
	if (code) break;
	xferO += ulen;
	xferL -= ulen;
    } while (ulen != 0);
    if (code) return code;

    s->remaining -= s->uioDataL;
    afsl_Assert(s->thisPage != s->lastPage || s->remaining == 0);
    return 0;
}

/*
 * AdjustFileLength -- Update file length following a write; truncate file if
 *     necessary to get rid of blocks (or frags) that we may have allocated
 *     beyond the end of file.  If we failed to promote a file and the new
 *     length would be inconsistent with the original allocation the WS_GROW
 *     bit will be cleared and we will return without changing the length.
 */
static long AdjustFileLength(IN struct writeState *s)
{
    u_long newlen;
    afs_hyper_t nlen;
    long code;

    if (!(s->flags & WS_GROW))
	return 0;
    afsl_Assert(s->newLength > s->oldLength);

    code = GetMetaTrans(s);
    if (code != 0)
	return (code);

    newlen = MAX(s->newLength - s->remaining, s->oldLength);
    AFS_hset64(nlen, 0, newlen);

    if (s->curLength > newlen) {
	afs_hyper_t clen;
	AFS_hset64(clen, 0, s->curLength);
	code = epix_SetLength(s->metaTrans, s->h, clen, 1);
	if (code != 0)
	    return (code);
	code = epia_Truncate(s->metaTrans, s->h, 0L, nlen);
    } else
	code = epix_SetLength(s->metaTrans, s->h, nlen, 1);

    return (code);
}

static long FinishUp(INOUT struct writeState *s)
{
    long code;

    /* The last thing is to end any transtion we may have started */
    epix_EnterCodeBlock(11);
    BEGIN_CRIT (/* nothing */0,
		(((s->flags & WS_NEW_TRANS) &&
		  !elbb_IsZeroTrans(s->metaTrans))
		 ? ((code = epia_EndTran (s->h, s->metaTrans)),
		    s->metaTrans = elbb_zeroTrans): 0)) ;

    /* Try to set the length to the last point we wrote on the way out, even if
     * there was an error.  To set the length we need a meta data transaction,
     * which must be started if we haven't got one yet. */

    BEGIN_CRIT ( /* nothing */ 0,
		 if (s->newLength > s->oldLength) {
		     code = AdjustFileLength(s);
		     afsl_MBZ (code);
		 });

    /* First release any buffers */
    code = ReleaseBuf (s);
    if (code)
	EXIT_CRIT;

    /* Finish up any left-over insertions. */
    code = DoInserts(s);
    if (code != 0)
	EXIT_CRIT;

    /* Return any unused blocks */
    if (s->beAlloc == WS_BLOCKED) {
	epix_EnterCodeBlock(77);
	if (s->new.b.next < s->new.b.n) {
	    epix_EnterCodeBlock(12);
	    afsl_Assert (!(s->flags & WS_NEW_TRANS) ||
		    !elbb_IsZeroTrans(s->metaTrans));
	    code = epib_Free (s->metaTrans, s->h, s->new.b.n - s->new.b.next,
			      s->new.b.blocks + s->new.b.next);
	    if (code)
		EXIT_CRIT;
	}
    }
    END_CRIT; /* setting length */
    END_CRIT; /* ending trans */

    return code;
}

/* epia_Write -- Insures that a container is large enough to hold the byte
 *     range specified by the \fld{offset} and \fld{resid} fields
 *     in \arg{uio}.  Any missing blocks or fragments in that interval
 *     are allocated, and the container's length is updated if necessary.
 *     Allocating storage may cause failures if the aggregate or volume becomes
 *     full.  The storage method (e.g. inline, fragmented or blocked) may also
 *     be changed if it is necessary to acomodate the container's new size.  If
 *     the \fld{osi_uio_iov} field of \arg{uio} is non-null then the indicated
 *     data are copied into the container using \fcn{uiomove}.
 *
 *     If the anode is marked as \cst{EPIA_CFLAGS_LOGGED} then these
 *     metadata modifications will be logged and \arg{trans} must be
 *     non-null or the error \cst{EPI_E_NOTRANS} will be returned.
 *     Otherwise \arg{trans} should be null and sequence of new
 *     transactions will be created, such that each is relatively small.  If a
 *     transaction is provided, it is used for the entire operation, no matter
 *     how large it is.
 *
 *     Because of the use of multiple transactions, this routine can return an
 *     error code, or the system may crash, after the write has been
 *     partly completed.  The file will be consistent in the most literal
 *     sense, but some of the requested modifications may not have occurred.
 *     This proviso does not apply when a transaction is specified for
 *     the whole operation, as is true of metadata containers.
 *
 *     If \arg{flags} is set to \cst{EPIA_RWFLAGS_UNBUFFERED}, the
 *     caller (\fcn{epia_Strategy}) is operating on behalf of the virtual
 *     memory system and the data being written must be flushed from the buffer
 *     cache before return.
 *
 * PARAMETERS --
 *     trans -- optional transaction to use for the operation.
 *     h -- anode handle.
 *     flags -- whether to leave data cached in the buffer system.
 *     uio -- standard {\unix} data structure that specifies the source of I/O
 *	request.
 *
 * LOCKS USED -- The container is assumed to be locked by a higher level to
 *     insure consistency throughout this operation. */

/* EXPORT */
long epia_Write(
  elbb_tranRec_t trans,
  epi_anode_t h,
  long flags,
  struct uio *uio)
{
    long code;
    struct writeState state;

    epix_EnterCodeBlock(25);

    BEGIN_CRIT (code = InitWrite (trans, h, uio, &state),
		code = FinishUp (&state)) {
	if (flags & EPIA_RWFLAGS_UNBUFFERED)
	    state.flags |= WS_UNBUFFERED;
	if (flags & EPIA_RWFLAGS_NOWITHHOLD)
	    state.flags |= WS_NOWITHHOLD;
	do {
	    if (code = AdvancePage(&state)) break;
	    if (code = AllocateStorage (&state)) break;
	    if (code = InitializeStorage (&state)) break;
	    if (code = WriteData (&state)) break;
	} while (state.thisPage != state.lastPage);
	if (code) continue;

	afsl_Assert (state.remaining == 0);
	/* since we're allocating conservatively... */
	afsl_Assert ((state.beAlloc != WS_BLOCKED) ||
		(state.new.b.n == 0) ||
		(state.new.b.next == state.new.b.n));
    } END_CRIT;
    return code;
}

#if 0
/* CountVisible -- counts the visible blocks under a given indirect block
 *     starting at a specific block address.  It is called from Truncate when
 *     it encounters a backing reference to an indirect block. */

static long CountVisible(
  IN epi_anode_t h,
  IN baddr_t block,
  IN struct epix_blockLocation *loc,
  OUT u_long *nVisibleP)
{
#define N_DEPTH (sizeof(loc->blocks)/sizeof(loc->blocks[0]))
#define N_PBLOCK 100
    const int depth = loc->depth;	/* indect of starting IB */
    u_long visible = 0;			/* blocks found */
    long code;
    daddr_t pBlock[N_PBLOCK];
    int nFound;
    u_long nSkip;			/* distance to next non-empty block */
    struct epix_blockLocation tloc;
    int i;

    /* Empty rest of loc's block array.  We'll use it to track IBs we encounter
     * during our traverse. */

    for (i=depth+1; i<N_DEPTH; i++)
	loc->blocks[i] = EPIX_ANODE_BLOCKEMPTY;

    for (;;) {
	code = epix_FindBlocks (h, block, N_PBLOCK, /*forRead*/1, pBlock,
				&nFound, &nSkip, &tloc);
	if (code) return code;

	/* See if we're still in the IB we were asked to count. */

	if ((tloc.tree != loc->tree) || (tloc.depth <= depth) ||
	    (tloc.blocks[depth] != loc->blocks[depth])) break;

	/* Count the indirect blocks that changed since last time. */

	for (i=depth+1; i<tloc.depth; i++)
	    if (tloc.blocks[i] != loc->blocks[i]) {
		/* Count this IB but save it so we see it only once. */
		visible++;
		loc->blocks[i] = tloc.blocks[i];
	    }

	/* Then count the blocks actually returned */

	for (i=0; i<nFound; i++)
	    if (!epix_IsBlockEmpty (pBlock[i]))
		visible++;

	block += nSkip;
	if (block == 0 || block == (baddr_t)-1)
	    break;
    }
    *nVisibleP = visible;
    return 0;
}
#endif /* 0 */

/* epia_Truncate -- Truncates a container to the specified length.  All the
 *     blocks allocated beyond the new length are freed (and the appropriate
 *     quota update procedure is called).  This call always fails if the anode
 *     is being used as a backing anode for another container.
 *
 *     If the anode is marked as \cst{EPIA_CFLAGS_LOGGED} then these
 *     metadata modifications will be logged and \arg{trans} must be
 *     non-null or the error \cst{EPI_E_NOTRANS} will be returned.
 *     Otherwise \arg{trans} should be null and a sequence of new
 *     transactions will be created, such that each is relatively small.  If a
 *     transaction is provided, it is used for the entire operation, no matter
 *     how large it is.
 *
 *     Because of the use of multiple transactions, this routine can return an
 *     error code, or the system may crash, after the truncation has been
 *     partly completed.  The file will be consistent in the most literal
 *     sense, but the contents of the file beyond the requested length may be
 *     zero.  This proviso does not apply when a transaction is specified for
 *     the whole operation, as is true of metadata containers.
 *
 *     The \arg{flags} argument is only used when calling
 *     \fcn{epia_Write}, which only happens when the length is being
 *     increased. It can always be set to zero for meta data (since we always
 *     use the buffer pool for meta data). In addition, when the length is
 *     being set to zero (for instance, as part of a delete operation) clearly
 *     the length is not being extended and \fcn{epia_Write} will not be
 *     called.  Principally, the vnops layer needs to specify
 *     \cst{EPIA_RWFLAGS_UNBUFFERED} when calling this routine from
 *     \fcn{efs_setattr} if the same file might also be modified via calls to
 *     \fcn{epia_Strategy}.
 *
 * PARAMETERS --
 *     trans --  optional transaction to use for the operation.
 *     h -- anode handle.
 *     flags -- flags passed to \fcn{epia_Write}, if necessary.
 *     length -- new length for the file.
 *
 * LOCKS USED -- The container is assumed to be locked by a higher level to
 *     insure consistency throughout this operation. */

/* EXPORT */
long epia_Truncate(
  IN elbb_tranRec_t trans,
  IN epi_anode_t h,
  IN long flags,			/* flags to pass to write, if needed */
  IN afs_hyper_t length)			/* desired length of container */
{
    long code;
    async_device_t *device;
    long fragsPerBlock;
    u_long newLength;
    u_long oldLength;

    elbb_tranRec_t metaTrans;		/* transaction for meta data */
    int newTrans;			/* if metaTrans created as needed */

    epia_qproc_t proc;			/* quota procedure */
    epia_quota_t delta;

    epix_EnterCodeBlock(26);

    if (epia_GetCopies(h) != 0)
	return EPI_E_OUTSTANDINGCOPIES;

    /* The 32 bit restriction should be relatively easy to fix up ... */
    afsl_Assert(AFS_hfitsinu32(length));
    newLength = AFS_hgetlo(length);
    oldLength = epia_GetLen32(h);

    if (newLength == oldLength)
	return 0;			/* nothing to do... */
    else if (newLength > oldLength) {	/* grow file w/ zeros */
	/* Use Write to grow the file to the desired  new length */
	struct uio uio;
	epix_EnterCodeBlock(27);
	epia_InitUio(&uio, 0, newLength, 0, 0);
	code = epia_Write(trans, h, flags, &uio);
	return code;
    }

    device = epia_AnodeDevice(h);
    fragsPerBlock = device->blkSize >> device->logFragSize;

    /* If logging isn't enabled yet, just pretend to use (the zero) trans for
     * regular and metadata updates and so neither start nor end transactions
     * here.  Otherwise decide whether we need to start our own transactions or
     * not. */
    newTrans = elbb_IsZeroTrans(trans) && epia_DeviceLogged(h);
    if (epia_GetType(h) != EPIA_CFLAGS_REGULAR) {
	epix_EnterCodeBlock(28);
	if (newTrans)
	    return EPI_E_NOTRANS;
    }
    /* Regular files, even logged ones such as directories & ACLs, can be
     * truncated in pieces */
    metaTrans = trans;

#define GET_metaTrans() \
    ((newTrans && elbb_IsZeroTrans(metaTrans)) ? \
     epia_StartTran("epia_Truncate", h, &metaTrans) : 0)

    proc = epia_GetQuotaProc(h);

    /* Now the hard part.  Deallocate all the blocks past the one
     * containing the last remaining data byte.
     *
     * For inline containers the length governs how much data is there. Zero
     * fill bytes past oldlength to newlength.
     *
     * For fragmented containers which are COW and which are being truncated to
     * zero length we insert a zero length frag description and release the
     * visible quota.  For COW containers being truncated to any other length:
     * setting the length will have the desired effect.  Otherwise, entirely
     * empty trailing fragments are deallocated and quota is adjusted.
     *
     * For blocked containers, we flush entirely free blocks even if they are
     * only backing blocks.  This is necessary otherwise an unclone operation
     * will effectively recreate those pages.  To do this even when the first
     * page to be removed is in a backing indirect block we may need to
     * allocate new indirect blocks.  This may lead to anomalous seeming
     * behavior of consuming quota when truncating a COW container.  To
     * ameliorate this we update the quota using "NOCHECK" so that we won't
     * actually fail to complete a truncate because of this.  Also we create
     * these indirect blocks before actually modifying the block map which
     * means that if no actual blocks need to be removed (e.g. the new and old
     * lengths are both in the same block) then we insert and indirect block
     * for no purpose. */

    BEGIN_CRIT (/* nothing */0,
		if (newTrans && !elbb_IsZeroTrans(metaTrans))
		code = epia_EndTran (h, metaTrans)) {

	if (EPIA_INLINE(h)) {
	    epix_EnterCodeBlock(29);
	    /* zerofill from new EOF to old EOF */
	    if (newLength != 0) {
		code = GET_metaTrans();
		if (code)
		    EXIT_CRIT;		
		code = epix_ZeroInline(metaTrans, h, newLength, 
					oldLength - newLength);
		afsl_MBZ(code);
	    }
	} else if (EPIA_FRAGMENTED(h)) {
	    epix_fragDescr_t frags;
	    u_int nfrags;

	    epix_EnterCodeBlock(78);
	    nfrags = ceil_l2(newLength, device->logFragSize);
	    code = epix_GetFragInfo(h, &frags);
	    if (code)
		EXIT_CRIT;
	    afsl_Assert(nfrags <= frags.length);
	    if (nfrags == 0 || (nfrags < frags.length &&
				  epix_IsBlockAllocated(frags.block))) {
		/* Make anode empty, or free dangling fragments */
		delta.v = nfrags - frags.length;
		delta.a = epix_IsBlockAllocated(frags.block) ? delta.v : 0;
		code = GET_metaTrans();
		if (code)
		    EXIT_CRIT;
		epix_EnterCodeBlock(79);
		if (epix_IsBlockAllocated(frags.block)) {
		    /* free the frags if they're allocated */
		    code = epib_FreeFrags (metaTrans, h, nfrags, &frags);
		    if (code)
			EXIT_CRIT;
		    afsl_Assert(frags.length == nfrags && delta.a < 0);
		} else {
		    /* Make COW anode empty by throwing backing frags away */
		    frags.length = 0;
		    afsl_Assert(nfrags == 0);
		}
		code = epix_InsertFragments(metaTrans, h, &frags);
		afsl_MBZ(code);
		if (proc != NULL) {
		    code = (*proc)(metaTrans, h, QUOTA_UPDATE, delta);
		    afsl_MBZ (code);	/* delta always negative */
		}
	    }
	} else {

	    /* Except for allocation of indirect blocks all changes to quota
	     * should be negative which should always succeed. */

	    u_long firstPage;		/* first whole block to nuke */
	    u_long lastPage;		/* last whole block which exist */
	    u_long page;		/* block freeing index variable */
	    u_int totalBlocks;		/* n blocks freed in current trans. */

	    epix_EnterCodeBlock(82);
	    delta.a = delta.v = 0;
	    totalBlocks = 0;

	    /* Use old length to get lastPage.  We could use this to terminate
	     * the truncate, but we won't, for now.  Instead we'll note when
	     * this wouldn't work, and go on cleaning out blocks all the way
	     * to the end of the container.  However, we will use lastPage to
	     * avoid doing any work at all if the new length and the old length
	     * are in the same block.  This has the advantage of eliminating
	     * the funny case where truncating a COW container by a small
	     * amount causes a bunch of indirect blocks to be created but does
	     * nothing else.
	     *
	     * NOTE -- In the future we could implement a punch operation which
	     *     removes blocks from the middle of a file.  Using lastPage to
	     *     really stop the block removal would be an easy way to do
	     *     this. */

	    afsl_Assert (oldLength != 0);
	    lastPage = div_l2(oldLength - 1, device->logBlkSize);

	    /* Calculate the first page needing removal. */

	    page = firstPage = ceil_l2(newLength, device->logBlkSize);
	    if (page <= lastPage) while (1) {
		daddr_t pBlock[EPIB_MAX_FREEDBLOCKS_PER_TRANS];
		int nFound;
		u_long nSkip;		/* distance to next allocated block */
		struct epix_blockLocation loc;
		int i;

		code = epix_FindBlocks
		    (h, page, EPIB_MAX_FREEDBLOCKS_PER_TRANS,
		     /*!forRead*/0, pBlock, &nFound, &nSkip, &loc);
		if (code == EPI_E_EOF)	/* nothing past here */
		    { code = 0; break; }
		if (code) break;

		/* Specially handle backing IB refs */

		if (EPIX_IsLocNonTerminalIndirect(&loc) &&
		    EPIX_IsBlockInBacking(pBlock[0])) {
		    afsl_Assert (nFound == 1);
		    if (page == firstPage) {

			/* In this case we must allocate an indirect block so
			 * we can free the blocks starting with firstPage. */

			daddr_t pb;
			u_long nBlocks = 1;

			epix_EnterCodeBlock(81);
			code = GET_metaTrans();
			if (code) break;
			code = epib_Allocate(metaTrans, h, &nBlocks, &pb, 0);
			if (code == EPI_E_AGGREGATEFULL)
			    break;
			afsl_MBZ(code);
			code = epix_Insert (metaTrans, h, 1, &pb, &loc);
			afsl_MBZ(code);
			if (proc != NULL) {
			    epia_quota_t ldelta;
			    ldelta.a = fragsPerBlock;
			    ldelta.v = 0;
			    code = (*proc)(metaTrans, h,
					   QUOTA_UPDATE_NOCHECK, ldelta);
			    afsl_MBZ (code);
			}
			continue;	/* try again to get this page... */
		    } else {
#if 0
			u_long visible;
			baddr_t newpage; /* page to continue on */

			loc.blocks[loc.depth] =
			    epix_MapBlockToBacking (pBlock[0]);
			code = CountVisible (h, page, &loc, &visible);
			afsl_MBZ (code);
			delta.v -= visible * fragsPerBlock;
#endif
		    }
		}

		while ((nFound > 0) && epix_IsBlockEmpty(pBlock[nFound-1]))
		    nFound--;

		if (nFound) {
		    u_int nAlloc = 0;	/* number allocated blocks removed */
		    u_int nBack = 0;	/* number visible blocks removed */

		    for (i = 0; i < nFound; i++)
			if (epix_IsBlockAllocated(pBlock[i])) nAlloc++;
			else if (!epix_IsBlockEmpty(pBlock[i])) nBack++;

		    /* Periodically we start a new trans so they stay small.
		     * However, we must leave things consistent so the quota
		     * must be updated. */
		    /* TODO -- I don't think we need to update the length on
		     *     the way out.  Right? */
		    if (newTrans && (totalBlocks + nAlloc >
				     EPIB_MAX_FREEDBLOCKS_PER_TRANS)) {
			epix_EnterCodeBlock(83);
			afsl_Assert (!elbb_IsZeroTrans (metaTrans) &&
				     (delta.a < 0) && (delta.v < 0));
			if (proc) {
			    code = (*proc) (metaTrans, h, QUOTA_UPDATE, delta);
			    afsl_MBZ (code); /* delta always negative */
			}
			code = epia_EndTran (h, metaTrans);
			afsl_MBZ (code);
			metaTrans = elbb_zeroTrans;
			delta.a = delta.v = 0;
			totalBlocks = 0;
		    }

		    /* free all the blocks we found */
		    code = GET_metaTrans();
		    if (code) break;
		    if (nAlloc) {
			code = epib_Free (metaTrans, h, nFound, pBlock);
			if (code) break; /* any valid errors here? */
		    }
		    code = epix_Insert (metaTrans, h, -nFound, pBlock, &loc);
		    afsl_MBZ (code);
		    totalBlocks += nAlloc;
		    delta.a -= nAlloc * fragsPerBlock;
		    delta.v -= (nAlloc + nBack) * fragsPerBlock;

		    /* In some cases we have a backing reference to an indirect
		     * block.  If so want get its size from epix_Insert so we
		     * can adjust the quota correctly. */

		    delta.v -= loc.visibleAdjust;
		}

		afsl_TraceEnabled (1, ((page > lastPage) && nFound &&
				       !EPIX_IsLocNonTerminalIndirect(&loc)),
				   AFSL_TR_ALWAYS,
				   ("truncate removing block past EOF"));

		page += nSkip;
		if ((page == 0 || page == (u_long)-1) && (nSkip != 0)) break;
	    }
	    if (delta.v && proc) {	/* delta.v is superset of delta.a */
		/* Update the quota for any blocks already freed, but better
		 * not be an error exit. */
		epix_EnterCodeBlock(80);
		afsl_AssertTruth (!code || (code == EPI_E_AGGREGATEFULL));
		code = (*proc) (metaTrans, h, QUOTA_UPDATE_NOCHECK, delta);
		afsl_MBZ (code);
	    }
	    if (code)
		EXIT_CRIT;
	}

	/* Set the length to the desired value */
	code = GET_metaTrans();
	if (code) continue;
	code = epix_SetLength (metaTrans, h, length, /*max*/0);
	if (code) continue;

    } END_CRIT;
    return code;
}

/* epia_Clone -- Called during fileset operations to reclone or unclone a
 *     container.  The anode \arg{b} must be the first backing anode for
 *     \arg{h}.  The return value \cst{EPI_E_NOTDONE} is only returned if a
 *     non-null transaction is provided.
 *
 * PARAMETERS --
 *     trans -- optional transaction to use for the operation.
 *     h -- copy-on-write container.
 *     b -- backing container.
 *     flags -- to reclone (if 1) or unclone (if 0). */

/* EXPORT */
long epia_Clone(
  IN elbb_tranRec_t trans,
  IN epi_anode_t h,			/* copy-on-write container */
  IN epi_anode_t b,			/* backing container */
  IN long flags)			/* to reclone or to unclone */
{
    long code;
    int reclone;			/* recloning or not */

    int newTrans;			/* if transactions created as needed */
    int notDone;			/* not completed in current trans */

    epia_qproc_t procC;			/* quota procedure for COW */
    epia_qproc_t procB;			/* quota procedure for backing */
    epia_quota_t delta;			/* dummy to check for over quota */

    epix_EnterCodeBlock(84);
    if (flags & ~1) return EPI_E_BADFLAGS;
    if (flags & 1) reclone = 1;
    else reclone = 0;

    newTrans = elbb_IsZeroTrans (trans);

    afsl_Assert(epia_DeviceLogged(h));	/* we don't do this during agg init */
    afsl_Assert (epia_AnodeDevice(h) == epia_AnodeDevice(b));

    procC = epia_GetQuotaProc (h);
    procB = epia_GetQuotaProc (b);
    afsl_Assert (procC && procB);

    /* check to make sure appropriate volume isn't already over quota */
    delta.a = delta.v = 0;
    if (reclone) code = (*procB) (0, b, QUOTA_CHECK, delta);
    else code = (*procC) (0, h, QUOTA_CHECK, delta);
    if (code) return code;

    do {
	notDone = 0;
	BEGIN_CRIT (if (newTrans)
			code = epia_StartTran ("epia_Clone", h, &trans),
		    if (newTrans) code = epia_EndTran (h, trans)) {
	    daddr_t pBlock[WS_ARRAYSIZE]; /* blocks to be freed */
	    u_int nFound;
	    struct epix_fragDescription frags; /* fragments to be freed */
	    epia_quota_t deltaC, deltaB;

	    code = epix_MoveData (trans, h, b, flags,
				  WS_ARRAYSIZE, pBlock, &nFound,
				  &frags, &deltaC, &deltaB);
	    if (code == EPI_E_NOTDONE) notDone = 1, code = 0;
	    if (code) continue;
	    if (deltaC.a || deltaC.v)
		code = (*procC) (trans, h, QUOTA_UPDATE_NOCHECK, deltaC);
	    afsl_MBZ (code);
	    if (deltaB.a || deltaB.v)
		code = (*procB) (trans, b, QUOTA_UPDATE_NOCHECK, deltaB);
	    afsl_MBZ (code);
	    if (nFound)
		code = epib_Free (trans, b, nFound, pBlock);
	    afsl_MBZ (code);
	    if (frags.length)
		code = epib_FreeFrags (trans, b, /*keep*/0, &frags);
	    afsl_MBZ (code);
	} END_CRIT;
	if (code) break;
    } while (notDone && newTrans);
    if (code) return code;
    if (notDone) {
	epix_EnterCodeBlock(85);
	return EPI_E_NOTDONE;
    }
    return 0;
}

/* epia_Fsync -- Flush out all user and meta-data associated with the
 *     container.  It returns only after the data has been safely written to
 *     the disk.
 *
 * PARAMETERS -- h is an anode handle. */

/* EXPORT */
long epia_Fsync(IN epi_anode_t h)
{
    long code;
    struct epia_info info;
    enum epia_ws_method isAlloc;		/* allocation method */
    async_device_t *device;

    epix_EnterCodeBlock(86);
    code = epia_GetInfo (h, &info);
    if (code) return code;
    if ((info.flags & EPIA_CFLAGS_COPYONWRITE)) {
	/* Recurse on backing handle.  Note that this function is not really
	 * opening the handle, but just "lending" you the handle.  It is valid
	 * as long as the h handle stays open and is not cloned. */
	epi_anode_t b;
	code = epix_GetBackingAnode (h, &b);
	afsl_PAssert (!code && b, ("Can't get backing anode"));
	code = epia_Fsync (b);
    }

    /* Decide what data storage method is used */
    isAlloc = (info.flags & EPIA_CFLAGS_INLINE) ? WS_INLINE :
	      (info.flags & EPIA_CFLAGS_FRAGMENTED) ? WS_FRAGMENTED :
	      (info.flags & EPIA_CFLAGS_BLOCKED) ? WS_BLOCKED :
	      WS_EMPTY;
    device = epia_AnodeDevice (h);

    switch (isAlloc) {
      case WS_EMPTY:
      case WS_INLINE:
	/* Nothing to do here */
	epix_EnterCodeBlock(87);
	break;

      case WS_FRAGMENTED:
	{   struct epix_fragDescription frags;

	    epix_EnterCodeBlock(88);
	    code = epix_GetFragInfo (h, &frags);
	    if (code) return code;
	    if (epix_IsBlockAllocated(frags.block)) {
		code = elbb_FsyncForce (device, 0, &frags.block, 1);
		if (code) return code;
	    }
	}
	break;

      case WS_BLOCKED:
#define FS_SHORT_BLOCKS 8 /* ANODE_DIRECTBLOCKS */
	{   afs_hyper_t maxFastLen;
	    daddr_t blocks[FS_SHORT_BLOCKS];
	    int nFound = 0;
	    u_long nSkip;		/* distance to next allocated block */
	    u_long magic;
	    int i;

	    epix_EnterCodeBlock(89);
	    AFS_hset64 (maxFastLen, 0, FS_SHORT_BLOCKS<<(device->logBlkSize));
	    if (AFS_hcmp(info.length, maxFastLen) <= 0) {

		/* Fetch the easy block addrs.  We set forRead to false so that
		 * we only get blocks actually allocated to this container.
		 * The recursion step at the beginning of this procedure takes
		 * care of any backing containers. */

		epix_EnterCodeBlock(90);
		code = epix_FindBlocks (h, /*starting block*/0,
					FS_SHORT_BLOCKS, /*!forRead*/0,
					blocks, &nFound, &nSkip, 0);
		if (code) return code;
		if (nSkip != (u_long)-1) {
		    /* we didn't get them all, a surprising result considering
		     * we've already checked the length, but... */
		    epix_EnterCodeBlock(91);
		    afsl_TraceEnabled (1,1,AFSL_TR_ALWAYS,
 ("epia_Fsync: more blocks found (%d) than indicated by length (%u)",
  nFound, AFS_hgetlo(info.length)));
		    nFound = 0;		/* don't use block list */
		}
	    }

	    magic = FsyncMagic (h);

	    for (i=0; i<nFound; i++)
		if (!epix_IsBlockAllocated(blocks[i]))
		    blocks[i] = EPIX_ANODE_BLOCKEMPTY;
	    code = elbb_FsyncForce (device, magic, blocks, nFound);
	    if (code) return code;
	}
	break;

      default:
	afsl_PAssert (0, ("illegal isAlloc"));
    }
    code = epix_FsyncForce (h);
    return code;
}

/*
 * Calculate whether anode has any holes or COW blocks by comparing
 * the number of allocated blocks with the file length.
 */

int epia_HasHoles(epi_anode_t ap, long len)
{
    u_long needed ;	/* FS blocks needed for user data + metadata */
    u_long allocated;	/* FS blocks currently allocated to this vnode */
    async_device_t *aggr = epia_AnodeDevice(ap);
    int lgFsize = aggr->logFragSize;
    u_long fragAlloc = epia_GetAllocated(ap);	/* # frags allocated */

    if (EPIA_INLINE(ap))
	return (0);
    else if (EPIA_FRAGMENTED(ap)) {
	return (len >= (fragAlloc << lgFsize));
    } else {	/* blocked file */
	needed =
	    epix_BlocksNeeded(ap, osi_btoab(aggr, len + aggr->blkSize - 1));
	allocated = osi_btoab(aggr, fragAlloc << lgFsize);
	return (needed != allocated);
    }
}
