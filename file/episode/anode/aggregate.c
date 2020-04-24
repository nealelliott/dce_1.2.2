/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */

/* aggregate.c -- contains procedures for managing aggregates. */

/* Copyright (C) 1996, 1990 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>
#include <dcedfs/osi_device.h>
/* XXX FIX ME XXX */
#undef	PRIVATE
#undef	SHARED
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/debug.h>
#include <dcedfs/hash.h>
#include <dcedfs/episode/instrument.h>
#include <dcedfs/episode/logbuf.h>
#include <dcedfs/episode/fifo.h>
#include <anode.h>
#include <anode_block.h>
#include <fixed_anode.h>
#include <block_alloc.h>
#include <volume_table.h>
#include <volume.h>
#include <volreg.h>
#include <aggregate.h>
#include <badblock.h>
#include <file.h>

/* This controls initialization of edsk_superBlockLocations in boot.h */
#define EDSK_INITIALIZE_SUPERBLOCKLOCATIONS

#include <boot.h>

RCSID ("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/anode/RCS/aggregate.c,v 10.22 1996/03/04 18:57:31 ota Exp $")

#define DEBUG_THIS_FILE EPIA_DEBUG_AGGREGATE

/* This structure is stored at the beginning of the AVL, in place of index #0.
 * To properly work with the volume table allocator the first work must be
 * non-zero.  We set this word to AVL_MAGIC. */

#define OLD_AVL_MAGIC 0xc7b65451
#define AVL_MAGIC 0x8946f098
#define DiskFormatMajorVersion(hdr) \
    (((hdr)->magic == AVL_MAGIC) ? EPIG_DISK_MAJOR_VERSION \
     : (((hdr)->magic == OLD_AVL_MAGIC) ? 0 \
	: -1))				/* unknown */
#define AVL_INDEX 1

#if 0
/* We no longer do this in anode fields.  The oid and gid are always 32 bits */
#define AVL_FLAGS 1		/* 128 bit principal_ids */
#endif
#define AVL_FLAGS 0

struct diskAggregateHeader {
    int32 magic;
    u_int32 flags;
    u_int32 blockSize;
    u_int32 fragmentSize;
    u_int32 firstBlock;			/* first device block usable */
    u_int32 nBlocks;			/* nBlocks in allocation pool
					 * from firstBlock onwards */
    u_int32 nLogBlocks;			/* size of log */
    u_int32 reservedIndexes;		/* for now this is AVL_INDEX */
    u_int32 bitmapIndex, logIndex;	/* indexes for these containers */
    u_int32 badblocksIndex;
    u_int32 timeCreated;
    u_int32 creatingProgram;		/* program that created the aggr */
    u_int32 diskFormatMinorVersion;	/* disk format minor version */
    u_int32 lastVerifyTime;		/* time of last verify/salvage */
    u_int32 lastVerifyCode;		/* last verify/salvage exit status */
    int32 spare[8];			/* returned by GetAggregateStatus */
};

static osi_dlock_t handleLock;	/* for allocation of new handles */
static int nAggregates;		/* number of inited aggregates */

/* aggregate handle flags bits */
#define AGGR_ATTACHED 0x01		/* aggregate is attached */
#define AGGR_OFFLINE  0x02		/* aggregate (being taken) offline */
#define AGGR_LOGGED   0x04		/* inited log */
#define AGGR_SALVAGE  0x08		/* used by standalone salvager */

struct aggregate {
    struct aggregate *next;
    struct tmutex_data lock;
    long flags;
    long refCount;
    epi_anode_t avl;			/* corresponding avl */
    struct async_device *device;
    struct diskAggregateHeader hdr;	/* hdr from superblock */
    epi_bitmap_t allocationHandle;
    u_long timeInited;
    u_long openVolumes;
    struct fifo_Thread list;		/* list entry for debugging */
};

static dfsh_hashTable_t aggrHash;

static struct fifo aggrList;		/* List of aggregates */

/*
 * hook for implementing monitoring and tuning specific to this submodule.
 */
/* EXPORT */
int ins_anode_aggregate(
 struct ins_opDesc *opcode,
 struct ins_varDesc *vars,
 struct ins_basicObjectId *argObjIds,
 struct ins_basicObjectId *resultObjIds,
 struct ins_basicObject *resultValues)
{
    /* not yet imlemented */
    return 0;
}

/*
 * Hash on the device major/minor pair in the device field.
 * This really should not have 16 hard-code, but it is elsewhere
 * (async/asevent.c) */

#define HASHDEV(maj, min) (((maj) << 16) ^ (min))

static u_long HashAggr(struct aggregate *ap)
{
    return HASHDEV(ap->device->majdev, ap->device->mindev);
}

/*
 * Comparison function for hash package, returning a boolean,
 * set via dfsh_HashInitKeyFunction().  This is more flexible than
 * dfsh_HashInitKeyFixed, and should have nearly the same performance
 * overhead. */

static int CmpAggr(struct aggregate *e1, struct aggregate *e2)
{
    afsl_Assert(e1 != NULL);
    afsl_Assert(e2 != NULL);

    /* Just compare device structure pointers */
    return e1->device == e2->device;
}

/* FindAggregate - locates an initialized aggregate using only the device
 * pointer.
 *   LOCKS:
 *     Assumes that the handleLock is held. */

static struct aggregate *FindAggregate(async_device_t *device)
{
    struct aggregate *h;
    struct aggregate key;

    epix_EnterCodeBlock(30);

    key.device = device;
    h = dfsh_HashLookup(&aggrHash, &key);

    return h;
}

/* AddAggregate - adds an initialized aggregate to the list.
 *   LOCKS:
 *     Assumes that the handleLock is held for write. */

static long AddAggregate(struct aggregate *new)
{
    long code;
#ifdef	AFS_DEBUG
    struct aggregate *h;
#endif	/* AFS_DEBUG */

    epix_EnterCodeBlock(1);
    afsl_Assert(new->refCount != 0);
#ifdef AFS_DEBUG
    h = dfsh_HashLookup(&aggrHash, new);
    afsl_Assert(h == NULL);
#endif	/* AFS_DEBUG */

    code = dfsh_HashIn_r(&aggrHash, new);
    afsl_MBZ(code);
    nAggregates++;

    /* Put on the debugging list */
    FIFO_ADD(&aggrList, &new->list);

    return 0;
}

/* DeleteAggregate - removes an aggregate from the list.
 *   LOCKS:
 *     Assumes that the handleLock is held for write. */

static long DeleteAggregate(IN struct aggregate *old)
{
    long code;
#ifdef	AFS_DEBUG
    struct aggregate *h;
#endif	/* AFS_DEBUG */

    epix_EnterCodeBlock(2);
    afsl_Assert(nAggregates > 0);

#ifdef	AFS_DEBUG
    h = dfsh_HashLookup(&aggrHash, old);
    afsl_Assert(h != NULL);
#endif
    code = dfsh_HashOut_r(&aggrHash, old);

    /* Remove from the debugging list */
    FIFO_REMOVE(&aggrList, &old->list);

    if (code == 0) {
	nAggregates--;
	return 0;
    }

    return -1;				/* not found */
}

/* epig_Init -- Initializes the Episode system in the anode layer and layers
 *     below (i.e. by calling \fcname{elbb_Init}).
 *
 * PARAMETERS --
 *     epigParams -- Initialization values. */

/* EXPORT */
long epig_Init(struct epig_params *epigParams)
{
    long code;
    static int inited = 0;
    struct icl_log *logp;

    if (code = elbb_Init(&epigParams->elbbParams))
	return code;

    if (inited) return 0;
    inited++;

#ifdef AFS_DEBUG
    /* CodeBlock: epia */
    epia_codeBlock[DEBUG_THIS_FILE][0] = 34;
#endif

    icl_CreateLogWithFlags_r("disk", 0, 0, &logp);
    /* not to be confused with epi_iclSetp, which is the vnops
     * version of this symbol which should be named efs_iclSet. */
    icl_CreateSetWithFlags_r("episode/anode", logp, 0,
				ICL_CRSET_FLAG_DEFAULT_OFF, &epi_iclSet);
    icl_LogRele_r(logp);

    lock_InitDesc (&handleLock, "aggregate handles");
    nAggregates = 0;

    /* Make debugging easier and use a list of the aggregates */
    FIFO_INIT(&aggrList, offsetof(struct aggregate, list));

    /* Initialize the hash package for the aggregate list */
    code = dfsh_HashInit(&aggrHash, HashAggr,
			 offsetof(struct aggregate, next));
    afsl_MBZ(code);
    code = dfsh_HashInitKeyFunction(&aggrHash, CmpAggr);
    afsl_MBZ(code);

    /* Initialize the anode layer */
    if ((code = epia_Init (epigParams->maxHandles)) ||
	(code = epib_Init()) ||
	(code = epit_Init()) ||
	(code = epiv_Init()) ||
	(code = epif_Init()))
	return code;

    return 0;
}

#define INITLOG_CREATE 1		/* 0 => LogInit, 1 => LogCreate */
#define INITLOG_CLOSE  2		/* 1 => LogClose */

static long InitLog(
 epi_anode_t avl,
 u_long index,
 u_long expectedNLogBlocks,
 int flags)				/* see INITLOG bits above */
{
    long    code;
    struct epia_info info;
    u_long  nLogBlocks;
    struct async_device *device;
    u_long length;
    daddr_t *logBlockArray = 0;		/* array of blocks in log container */
    u_int lbArraySize;			/* size of array */

    /* Now open and initialize the log */

    epix_EnterCodeBlock(3);
    BEGIN_LCRIT (epi_anode_t log,
		 code = epia_Open (avl, index, 0, 0, &log),
		 code = epia_Close (log)) {
	long blk;

	code = epia_GetInfo (log, &info);
	if (code)
	    EXIT_CRIT;
	device = epia_AnodeDevice(log);

	/* get conservative estimate of log size */
	if (!AFS_hfitsinu32(info.length)) { ABORT_CRIT (EPI_E_BADARGS) }
	length = AFS_hgetlo(info.length);
	nLogBlocks = length / device->blkSize;
	if (expectedNLogBlocks && (nLogBlocks != expectedNLogBlocks)) {
	    ABORT_CRIT (EPI_E_NOTANAGGREGATE)
	}
	lbArraySize = nLogBlocks * sizeof(daddr_t);

	/* The log module keeps this array if all goes well. */
	logBlockArray = osi_Alloc_r (lbArraySize);
	for (blk=0; blk<nLogBlocks; blk++) {
	    /* All log pages must be pre-allocated.  There is a faster way to
	     * look up all these addresses...  Then we kill them, but to be
	     * safe. */
	    daddr_t b;
	    code = epia_Map (log, blk, &b, /*device*/0);
	    if (code) {
		afsl_Trace (AFSL_TR_ERRMAP,
			    ("InitLog: from epia_Map got %u", code));
		code = EPI_E_BADLOGBLOCKS;
		break;
	    }
	    code = elbb_Kill (device, b, 0/*!metadatOK*/);
	    afsl_MBZ(code);		/* shouldn't be dirty log pages... */
	    logBlockArray[blk] = b;
	}
	if (code)
	    EXIT_CRIT;
	if (flags & INITLOG_CREATE) {
	    code = elbb_Shutdown (device);
	    afsl_MBZ(code);
	    /* Last arg of 0 = no cksum, 1 = CRC32 std cksum*/
	    code = elbb_LogCreate(device, device, logBlockArray, nLogBlocks, 1);
	} else
	    code = elbb_LogInit (device, device, logBlockArray, nLogBlocks);
    } END_CRIT;
    if (!code && (flags & INITLOG_CLOSE)) {
	/*
	 * Modified to ignore return value from elbb_LogClose (elbb_Shutdown)
	 * as it is never returns failure. If there is a problem, it panics.
	 */
	elbb_LogClose (device);
    }
    /*
     * The logBlockArray is no longer freed here if the INITLOG_CLOSE
     * is set, as elbb_LogClose ends up calling DeallocLogMap which has
     * been modified to take care of it.
     * Hence only need to free it in case of an error
     */
    if (logBlockArray && code )
	osi_Free_r (logBlockArray, lbArraySize);

#ifndef KERNEL
    if (code == RCVR_RECOVERED)
	printf ("Ran recovery on dev %d/%d\n", device->majdev, device->mindev);
#endif

    return code;
}

#define POWER_OF_TWO(x) (((x) & ((x)-1)) == 0)

/* SetFragSize -- fills in the fragSize and logFragSize fields of the device,
 *     doing various sanity checks on it first. */

static long SetFragSize(
 struct async_device *device,
 long fsize)
{   int i;
    epix_EnterCodeBlock(4);
    device->fragSize = fsize;
    if (!POWER_OF_TWO(fsize)) return -1;
    if ((fsize < 1024) || (fsize > device->blkSize)) return -1;
    for (i=10; fsize != (1<<i); i++);
    device->logFragSize = i;
    return 0;
}

#if !defined(KERNEL) /* don't include aggregate creation stuff in kernel */

/* WriteInitialBadBlockFile -- puts out a badblock file with the alternate
 *     superblocks in it. */

static long WriteInitialBadBlockFile(
 epi_anode_t bb,			/* bad blocks file */
 u_int nBlocks,
 daddr_t *blocks)
{
    long code;
    char badBlockDesc[epiz_bbFrameSize];
    struct epiz_bbFrame *bbFrame;
    struct epiz_bbEntry *bbEntry;
    struct uio uio;
    struct iovec iov;
    int i;

    if (nBlocks == 0) return 0;
    if (nBlocks*sizeof(daddr_t) + sizeof(bbFrame)-sizeof(bbFrame->data) +
	2*(sizeof(bbEntry)-sizeof(bbEntry->data)) > sizeof(badBlockDesc))
	return EPI_E_BADARGS;		/* too many blocks */
    bzero(badBlockDesc, sizeof(badBlockDesc));

    bbFrame = (struct epiz_bbFrame *) &(badBlockDesc[0]);
    bbFrame->beginStamp = (u_long) osi_Time();
    bbFrame->bbFrameMagic = epiz_BBFRAMEMAGIC;

    /* put blocks in superblock entry */
    bbEntry = (struct epiz_bbEntry *) bbFrame->data;
    bbEntry->type = epiz_bbetSuperBlocks;
    bbEntry->thisFrag = 0;
    bbEntry->totFrags = 1;
    bbEntry->length = nBlocks * sizeof(daddr_t);
    for (i = 0; i < nBlocks; ++i) {
	*(((u_long *) bbEntry->data) + i) = blocks[i];
    }

    /* fill out rest of frame with NULL entry. */
    bbEntry = (struct epiz_bbEntry *) (&bbEntry->data[bbEntry->length]);
    bbEntry->type = epiz_bbetNULL;
    bbEntry->length = sizeof(badBlockDesc) -
	((char *)(&bbEntry->data[0]) - (char *)badBlockDesc);

    epia_InitUio (&uio, &iov, 0, epiz_bbFrameSize, badBlockDesc);
    code = epia_Write(elbb_zeroTrans, bb, 0, &uio);
    return (code);
}

/* ScanBadBlockFile -- checks the bad block file for correctness of format and
 *     marks the mentioned blocks so that subsequent references to those blocks
 *     will report errors and so the bitmap (which should have all bad blocks
 *     marked) can be correctly verified.
 *
 *     We explicitly allow block references that are out of bounds of the
 *     current aggregate.  We only mention these blocks if in verbose mode. */

static long ScanBadBlockFile(
 struct epiz_salvageState *ss,
 epi_anode_t file)
{
    u_long length;			/* container length */
    u_long len;				/* remaining length */
    u_long off;				/* current offset */
    u_long l;				/* length of this frame */
    long code;
    char badBlockDesc[epiz_bbFrameSize];
    struct epiz_bbFrame *bbF;
    struct epiz_bbEntry *bbE;
    struct uio uio;
    struct iovec iov;
    int i;

    if (epia_GetFlags(file) & EPIA_CFLAGS_COPYONWRITE) {
	epiz_ReportError (ss, 0, EPIZ_E_BB_COW);
	return (-1);
    }
    if (!AFS_hfitsinu32(epia_GetLength (file))) {
	epiz_ReportError (ss, 0, EPIZ_E_BB_TOOLONG);
	return -1;
    }
    length = AFS_hgetlo(epia_GetLength(file));
    for (len = length, off = 0; len; len -= l, off += l) {
	l = MIN(len, epiz_bbFrameSize);

	/* We must have enough data for frame header (2 words) and head of
	 * first entry (4 shorts). */

	if (l < 16) {
	    epiz_ReportError (ss, 0, EPIZ_E_BB_TOOSHORT);
	    return -1;
	}
	epia_InitUio (&uio, &iov, off, l, badBlockDesc);
	code = epia_Read (file, 0, &uio);
	if (code) {
	    epiz_ReportError (ss, code, EPIZ_E_BB_IOERROR);
	    return -1;
	}
	bbF = (struct epiz_bbFrame *) badBlockDesc;
	if (bbF->bbFrameMagic != epiz_BBFRAMEMAGIC) {
	    epiz_ReportError (ss, 0, EPIZ_E_BB_MAGIC, bbF->bbFrameMagic);
	    return -1;
	}
	for (bbE = (struct epiz_bbEntry *) bbF->data;
	     (char *)bbE - (char *)badBlockDesc < l;
	     bbE = (struct epiz_bbEntry *)(&bbE->data[bbE->length])) {

	    /* For each entry in frame */

	    if (bbE->length % 2) {
		/* length must be a multiple of 2 bytes. */
		epiz_ReportError (ss, 0, EPIZ_E_BB_BADENTRY);
		return -1;
	    }
	    if (((char *)(&bbE->data[bbE->length]) - (char *)badBlockDesc)
		> l) {

		/* Due to a bug in making BadBlock files we need to allow this
		 * case if the type is bbetNULL. */

		if ((bbE->type != 0) || (bbE->length != 0)) {
		    epiz_ReportError (ss, 0, EPIZ_E_BB_BADENTRY);
		    return -1;
		}
	    }
	    if (bbE->type == epiz_bbetNULL)
		continue;
	    if ((bbE->type == epiz_bbetSuperBlocks) ||
		(bbE->type == epiz_bbetBadBlocks)) {
		for (i=0; i<bbE->length/sizeof(daddr_t); i++) {
		    daddr_t b = ((daddr_t *)bbE->data)[i];

		    /* Now check each block.  We'll not attempt any repairs
		     * here.  A badblock utility should do this. */

		    if (epix_IsBlockEmpty (b)) continue;
		    if (epix_BlockAddressSpecial(b) ||
			(b > ss->maxBlock) || (b < ss->minBlock)) {
			/* advise of these oddities, if in -verbose mode */
			epiz_ReportError (ss, 0, EPIZ_E_BB_BADBLOCK, b);
			continue;
		    }

		    /* Call either MarkUsed or SaveAnodeRef */
		    code = (*ss->blockRef) (b, 0, NULL, ss, 1);
		    if (code == EPI_E_DUPBLKUSE) {
			epiz_ReportError (ss, 0, EPIZ_E_BB_DUPBLK, b);
			continue;
		    }
		    afsl_MBZ(code);
		}
	    }
	}
    }
    return 0;
}

int epig_creatingProgram = EPIG_CREATOR_NEWAGGR;

/* epig_CreateAggregate -- Formats a new, empty aggregate.  No transactions are
 *     provided at this point since the logging system has not started yet.
 *     The maximum legal block address supported by the new aggregate will be
 *     ${firstBlock} + {blocks} - 1$.
 *
 * PARAMETERS --
 *     device -- device on which the aggregate resides.
 *     fragmentSize -- fragment size for the aggegate.
 *     firstBlock -- first usable block in the device.
 *     superBlock -- location of the primary superblock.
 *     blocks -- number of blocks to use.
 *     nLogBlocks -- size of the log.
 *     nExceptions -- blocks not available for use by the aggregate. These may
 *	 be alternate superblocks or bad blocks.
 *     exceptions -- array of blocks to exclude from allocation.
 *
 * LOCKS USED --
 *     Holds the handleLock throughout the create, to forestall, other creates
 *     and attempts to initialize a partially created aggregate.  We don't have
 *     an aggregate handle so we can't call AddAggregate or lock the handle
 *     itself.  Once we close the avl all evidence of the aggregate is gone
 *     from the anode layer, after the call to shutdown the buffer pool is
 *     cleared (we haven't been doing any logging or anything) so then another
 *     thread can call InitAggregate or CreateAggregate safely.
 *
 * ENVIRONMENT --
 *     The buffer package is assumed to be initialized already, since it is
 *     system-wide and not per aggregate.
 *
 *     The initialization of the anode layer is also assumed for similar
 *     reasons, namely, the anode handles are a system-wide resource.
 *
 *     Test programs are required to set the epig_creatingProgram variable
 *     appropriately.  This value is stamped into the aggregate header. */

/* EXPORT */
long epig_CreateAggregate (
  async_device_t *device,
  u_long fragmentSize,			/* fragment size for this aggregate */
  daddr_t firstBlock,			/* first usable block in device */
  daddr_t superBlock,			/* location of primary superblock */
  u_long blocks,			/* number of blocks to use */
  u_long nLogBlocks,			/* size of log */
  u_int nExceptions,
  daddr_t exceptions[])			/* blocks to exclude from allocation */
{
    long code;
    epi_anode_t avl;
    epi_anode_t bitmap;
    epi_anode_t log;
    epi_anode_t badblocks;
    u_long bitmapIndex, logIndex, badblocksIndex;
    epi_bitmap_t allocationHandle;
    long offset;
    struct diskAggregateHeader hdr;
    afs_hyper_t hZero;
    long len;
    afs_hyper_t length;

    epix_EnterCodeBlock(5);
    if ((MIN(superBlock, firstBlock) < 0) || (superBlock < firstBlock))
	return EPI_E_BADARGS;
    AFS_hzero (hZero);
    /* initialize aggregate header */
    bzero((caddr_t)&hdr, sizeof (hdr));
    hdr.magic = AVL_MAGIC;
    hdr.blockSize = device->blkSize;
    hdr.fragmentSize = fragmentSize;
    hdr.firstBlock = firstBlock;
    hdr.nBlocks = blocks;
    hdr.nLogBlocks = nLogBlocks;
    hdr.flags = AVL_FLAGS;
    hdr.reservedIndexes = AVL_INDEX;
    hdr.timeCreated = osi_Time();
    hdr.creatingProgram = epig_creatingProgram;
    hdr.diskFormatMinorVersion = EPIG_DISK_MINOR_VERSION;

    BEGIN_LOCK_WRITE (&handleLock) {
	if (FindAggregate (device)) {
	    ABORT_CRIT (EPI_E_AGGREGATEEXISTS)
	}

	code = SetFragSize (device, fragmentSize);
	if (code) {
	    ABORT_CRIT (EPI_E_BADARGS)
	}

	code = epit_CreateAVL (device, superBlock, AVL_INDEX, &avl);
	if (code)
	   EXIT_CRIT;

	code = epit_Allocate (elbb_zeroTrans, avl,
			      EPIT_TYPE_DONTCARE|EPIT_DONT_WITHHOLD,
			      0, &bitmapIndex);
	if (code)
	    EXIT_CRIT;
	code = epia_Create
	    (elbb_zeroTrans, avl, bitmapIndex,
	     EPIA_CFLAGS_PREALLOCATED+ EPIA_CFLAGS_LOGGED+ EPIA_CFLAGS_BITMAP,
	     0, 0, hZero, 0, 0, &bitmap);
	if (code)
	    EXIT_CRIT;
	hdr.bitmapIndex = bitmapIndex;

	{   int exceptSize = (nExceptions+1) * sizeof(daddr_t);
	    daddr_t *except = osi_Alloc_r (exceptSize);
	    int i;
	    epix_ConditionalCodeBlock(nExceptions != 0, 6);
	    for (i = 0; i < nExceptions; i++)
		except[i] = exceptions[i];
	    except[i] = superBlock;

	    code = epib_CreateAllocationPool
		(bitmap, firstBlock, blocks, device->blkSize/fragmentSize,
		 nExceptions+1, except);
	    osi_Free_r (except, exceptSize);
	    if (code)
		EXIT_CRIT;
	}

	code = epib_InitAllocationPool (bitmap, hdr.firstBlock,
					hdr.nBlocks, &allocationHandle);
	if (code)
	    EXIT_CRIT;
	epia_SetAllocationHandle (avl, allocationHandle);

	code = epit_Allocate (elbb_zeroTrans, avl,
			      EPIT_TYPE_DONTCARE|EPIT_DONT_WITHHOLD,
			      0, &logIndex);
	if (code)
	    EXIT_CRIT;
	code = epia_Create
	    (elbb_zeroTrans, avl, logIndex,
	     EPIA_CFLAGS_PREALLOCATED + EPIA_CFLAGS_LOG,
	     0, 0, hZero, 0, 0, &log);
	if (code)
	     EXIT_CRIT;
	hdr.logIndex = logIndex;
	epia_SetAllocationHandle (log, allocationHandle);

	len = hdr.nLogBlocks*device->blkSize;
	AFS_hset64 (length, 0, len);
	code = epix_SetLength (elbb_zeroTrans, log, length, 0);
	if (code)
	    EXIT_CRIT;

	/* allocate all the pages we'll need. */
	{   struct uio uio;
	    epia_InitUio (&uio, 0, 0, len, 0);
	    code = epia_Write (elbb_zeroTrans, log, 0, &uio);
	    if (code)
		EXIT_CRIT;
	}
	code = epia_Close (log);
	if (code)
	     EXIT_CRIT;

	/* and initialize it */
	code = InitLog (avl, logIndex, hdr.nLogBlocks,
			INITLOG_CREATE|INITLOG_CLOSE);
	if (code)
	     EXIT_CRIT;

	code = epit_Allocate (elbb_zeroTrans, avl,
			      EPIT_TYPE_DONTCARE|EPIT_DONT_WITHHOLD,
			      0, &badblocksIndex);
	if (code)
	    EXIT_CRIT;
	code = epia_Create
	    (elbb_zeroTrans, avl, badblocksIndex,
	     EPIA_CFLAGS_PREALLOCATED+EPIA_CFLAGS_LOGGED+EPIA_CFLAGS_BADBLOCKS,
	     0, 0, hZero, 0, 0, &badblocks);
	if (code)
	    EXIT_CRIT;
	hdr.badblocksIndex = badblocksIndex;
	epia_SetAllocationHandle(badblocks, allocationHandle);
	code = WriteInitialBadBlockFile (badblocks, nExceptions, exceptions);
	if (code)
	    EXIT_CRIT;
	code = epia_Close (badblocks);
	if (code)
	    EXIT_CRIT;

	/* now write out the aggregate header as avl index 0 */
	offset = epic_IndexToOffset (0);
	BEGIN_LCRIT (struct elbb_buffer *buf,
		     code = epix_GetBuffer (avl, 0, &buf),
		     code = elbb_Release (buf, elbb_lazy)) {
	    code = elbb_Modify (buf, offset, (caddr_t)&hdr, sizeof(hdr),
				elbb_zeroTrans, 0, &buf);
	} END_CRIT;
	if (code)
	    EXIT_CRIT;

	/* get all the anodes in this device to be GC'ed quickly */
	code = epix_SetGConClose (epix_gc_device, (opaque)device);
	if (code)
	    EXIT_CRIT;
	/* shutdown the bitmap, but first make sure no one is using it */
	{   long count;
	    code = epix_CountOpenAnodes (device, &count);
	    if (code)
		EXIT_CRIT;
	    afsl_Assert(count == 2);	/* just the avl and the bitmap */
	}
	code = epib_CloseAllocationPool (allocationHandle);
	if (code)
	    EXIT_CRIT;

	code = epia_Close (avl);
	if (code)
	    EXIT_CRIT;
	code = elbb_Shutdown (device);
	if (code)
	    EXIT_CRIT;
    } END_LOCK;
    return code;
}

#endif /* out-of-kernel aggregate creation stuff */

/* epig_GrowAggregate -- Increase the aggregate's size to \arg{blocks},
 *     which must be larger than the aggregate's current size by at least three
 *     (3) blocks.
 *
 * PARAMETERS --
 *     avl -- handle for the aggregate.
 *     blocks -- number of blocks to use.
 *     nExceptions -- blocks not available for use by the aggregate. These may
 *	 be alternate superblocks or bad blocks.
 *     exceptions -- array of blocks to exclude from allocation. */

/* EXPORT */
long epig_GrowAggregate(
  epi_anode_t avl,
  u_long blocks,		/* desired new aggregate size */
  u_int nExceptions,
  daddr_t exceptions[])		/* blocks to exclude from allocation */
{
    long code;
    long closeCode;
    struct aggregate *h;
    epi_anode_t bitmap;
    u_long newSize;			/* new size of bitmap */
    elbb_tranRec_t trans;		/* transaction identifier */
    daddr_t base;

    h = (struct aggregate *)epia_GetFileRock(avl);
    code = epia_Open (avl, h->hdr.bitmapIndex, 0, 0, &bitmap);
    if (code)
	return (code);

    do {
	/* grow aggregate under a transaction */
	BEGIN_CRIT(code = elbb_StartTran("grow_alloc", 0,
					  epia_AnodeDevice(bitmap), &trans),
		   elbb_EndTran(trans)) {
	    struct epib_info info;

	    code = epib_GrowAllocationPool(trans, bitmap, 0, blocks,
					   nExceptions, exceptions);
	    if (code)
		EXIT_CRIT;

	    code = epib_GetInfo (h->allocationHandle, &info);
	    if (code)
		EXIT_CRIT;
	    base = info.base;
	    newSize = info.length;

	    /* update aggregate header in-core and on disk */
	    BEGIN_TMUTEX(trans, &h->lock) {
		h->hdr.nBlocks = newSize;

		BEGIN_LCRIT (struct elbb_buffer *b,
			     code = elbb_Read (h->device, avl->block,
					       elbb_bufLogged, &b),
			     code = elbb_Release (b, elbb_lazy)) {
		    code = elbb_Modify (b, (offsetof(struct diskAggregateHeader, nBlocks) +
					    epic_IndexToOffset(0)),
					(caddr_t)&h->hdr.nBlocks, sizeof(u_int32),
					trans, /*logRequest*/1, &b);
		} END_CRIT;
	    } END_LOCK;
	    afsl_MBZ(code);
	} END_CRIT;
    } while (!code && newSize + base < blocks);

    closeCode = epia_Close (bitmap);
    afsl_MBZ(closeCode);

    return (code);
}

/* set h to good looking aggregate handle from avl or zero. */

#define GetAggrHandle(h, avl) \
    (((avl) && (h = (struct aggregate *)epia_GetFileRock(avl)) && \
      (h->hdr.magic == AVL_MAGIC)) ? h : (h = 0))
#define GetFairAggr(h, avl) \
    ((GetAggrHandle(h, avl) && (h->avl == avl)) ? h : (h = 0))
#define GetOKagg(h,avl) (GetFairAggr (h, avl) && h->refCount)

static void Cleanup(epi_anode_t avl)
{
    struct aggregate *h;
    epix_EnterCodeBlock(7);
    GetAggrHandle (h, avl);
    if (h) {
	afsl_Assert(h->avl == 0 && (h->flags & AGGR_OFFLINE) &&
		h->refCount == 0);
	afsl_Assert (!tmutex_held(&h->lock)); /* better not be any lockers */
	osi_Free_r(h, sizeof (*h));
    }
    epia_SetFileRock (avl, 0, 0);
}

/* AGGREGATE_HDR_OK -- verifies that the aggregate header obtained from a
 *     superblock is legitimate.  These tests should parallel the tests and
 *     corresponding explanations provided in the epig_SayWhyNotLikeSuperBlock
 *     function below. */

#define AGGREGATE_HDR_OK(hdr, device) \
    (((hdr)->magic == AVL_MAGIC) && \
     ((hdr)->flags == AVL_FLAGS) && \
     ((hdr)->creatingProgram > 0) && \
     ((hdr)->creatingProgram <= EPIG_CREATOR_NEWAGGR) && \
     ((hdr)->blockSize >= ANODE_MIN_BLOCKSIZE) && \
     ((hdr)->blockSize <= ANODE_MAX_BLOCKSIZE) && \
     POWER_OF_TWO((hdr)->blockSize) && \
     (!(device) || \
      ((hdr)->blockSize == ((struct async_device *)(device))->blkSize)) && \
     ((hdr)->fragmentSize >= 1024) && POWER_OF_TWO((hdr)->fragmentSize) && \
     ((hdr)->fragmentSize <= (hdr)->blockSize) && \
     ((hdr)->nBlocks > (hdr)->nLogBlocks+3) && \
     ((hdr)->firstBlock <= 64) && \
     (!(device) || \
      ((hdr)->firstBlock <= \
       (64 >> (((struct async_device *)(device))->logBlkSize-10)))) && \
     ((hdr)->reservedIndexes == AVL_INDEX) && \
     ((hdr)->bitmapIndex))

/* HowMuchLikeSuperBlock - returns an estimate of how much the given page is
 * like a superblock, in range [0,10000]. */

static long HowMuchLikeSuperBlock(
 struct async_device *dev,
 opaque dataptr,
 int *valP)
{
    int val;
    int32 magic;
    long code;
    opaque first;
    struct diskAggregateHeader *dahp;

    epix_EnterCodeBlock(8);
    *valP = 0;
    code = epic_HowMuchLikeAnodeBlock(dev, dataptr, &val, &magic, &first);
    if (code != 0) return code;
    if (magic != ANODE_BLOCK_AVL_MAGIC) return 0;
    dahp = (struct diskAggregateHeader *) first;
    if (!AGGREGATE_HDR_OK(dahp, 0)) {
	epix_EnterCodeBlock(9);
	return 0;
    }
    if (dev && (dahp->blockSize != dev->blkSize)) {
	epix_EnterCodeBlock(10);
	*valP = (val / 100);
    } else {
	*valP = val;
    }
    return 0;
}

#ifndef KERNEL
/* epig_SayWhyNotLikeSuperBlock -- Returns a string that explains why the
 *     provided superblock is not any good.  This routine is only available
 *     outside the kernel.
 *
 *     The return value is zero if there is no similarity between the specified
 *     block and a superblock.  If there number returned is less than 100 the
 *     block is definitely not a superblock but some recognizable fields were
 *     located.  The higher the value the more like a superblock it is.  If a
 *     \arg{dev} is provided additional checks based only upon the block size
 *     can be made on the reasonableness of the superblock; the fragment size
 *     fields can not be relied upon.  If all tests pass the value returned is
 *     10000. These values are returned in lieu of the standard Episode error
 *     codes. */

/* EXPORT */
int epig_SayWhyNotLikeSuperBlock(
  async_device_t *dev,			/* device ptr or zero */
  opaque dataptr,			/* ptr to superblock data */
  int len,				/* length of buffer */
  char *buf)				/* printable reason */
{
    long code;
    int32 magic;			/* anode block magic value */
    struct diskAggregateHeader *hdr;	/* aggregate header in superblock */
    int val = 0;			/* return value */
    int majorV;				/* major disk version number */

    if (len <= 1) return 0;
    buf[len-1] = 0;			/* terminate */

    code = epic_HowMuchLikeAnodeBlock (dev, dataptr, &val, &magic,
				       (opaque *)&hdr);
    if (code != 0) {
	sprintf (buf, "anode no good: code=%d", code);
	goto done;
    }
    if ((magic != ANODE_BLOCK_AVL_MAGIC) || (val < 5000)) {
	/* Try byte swapping magic, just for grins */
	int32 swappedMagic;
	swappedMagic = ((magic<<24)&0xff000000) | ((magic<<8)&0xff0000) |
	    ((magic>>8)&0xff00) | ((magic>>24)&0xff);
	if (swappedMagic == ANODE_BLOCK_AVL_MAGIC) {
	    sprintf (buf, "aggregate appears to be from a machine with reversed byte order");
	    val = 1;
	    goto done;
	}
	if (magic == ANODE_BLOCK_VOLUME_MAGIC) {
	    sprintf (buf, "anode magic was VOLUME not AVL");
	    val = 2;
	    goto done;
	}
	val = 0;
	sprintf (buf, "anode magic not AVL, instead %#lx", magic);
	goto done;			/* nothing here */
    }

    val++;
    majorV = DiskFormatMajorVersion(hdr);
    if (majorV < 0) {
	sprintf (buf, "aggregate header contained unknown magic: %#lx",
		 hdr->magic);
	goto done;
    }
    val++;
    if (majorV < EPIG_DISK_MAJOR_VERSION) {
	sprintf (buf, "aggregate is old major version #%d", majorV);
	goto done;
    }
    val = 100;
    if (hdr->flags != AVL_FLAGS) {
	sprintf (buf, "aggregate flags not zero (%#x)", hdr->flags);
	goto done;
    }
    val++;
    if (hdr->creatingProgram < 1) {
	sprintf (buf, "illegal creating program value: %d",
		 hdr->creatingProgram);
	goto done;
    }
    val++;
    if (hdr->creatingProgram > EPIG_CREATOR_NEWAGGR) {
	sprintf (buf, "unknown creating program: %d", hdr->creatingProgram);
	goto done;
    }
    val++;
    if (!hdr->blockSize) {
	sprintf (buf, "block size is zero");
	goto done;
    }
    val++;
    if (!POWER_OF_TWO(hdr->blockSize)) {
	sprintf (buf, "block size is not a power of two: %u", hdr->blockSize);
	goto done;
    }
    val++;
    if (hdr->blockSize > (64*1024)) {
	sprintf (buf, "block size is too large: %u", hdr->blockSize);
	goto done;
    }
    val++;
    if (hdr->blockSize < 1024) {
	sprintf (buf, "block size is too small: %u", hdr->blockSize);
	goto done;
    }
    val++;
    if (dev && (dev->blkSize != hdr->blockSize)) {
	sprintf (buf, "block size (%u) does not match device block size(%u)",
		 dev->blkSize, hdr->blockSize);
	goto done;
    }
    val++;
    if (!hdr->fragmentSize) {
	sprintf (buf, "fragment size is zero");
	goto done;
    }
    val++;
    if (!POWER_OF_TWO(hdr->fragmentSize)) {
	sprintf (buf, "fragment size is not a power of two: %u",
		 hdr->fragmentSize);
	goto done;
    }
    if (hdr->fragmentSize < 1024) {
	sprintf (buf, "fragment size is too small: %u", hdr->fragmentSize);
	goto done;
    }
    val++;
    if (hdr->fragmentSize > hdr->blockSize) {
	sprintf (buf, "fragment size (%u) is larger than block size (%u)",
		 hdr->fragmentSize, hdr->blockSize);
	goto done;
    }
    val++;
    if (hdr->nBlocks <= hdr->nLogBlocks+3) {
	sprintf (buf, "fewer aggregate blocks (%u) than log blocks (%u)",
		 hdr->nBlocks, hdr->nLogBlocks);
	goto done;
    }
    val++;
    if (hdr->firstBlock > (64 >> (dev ? (dev->logBlkSize-10) : 0))) {
	if (dev)
	    sprintf (buf, "first block (%lu) is larger than %lu",
		     hdr->firstBlock, (64 >> (dev->logBlkSize-10)));
	else sprintf (buf, "first block was %lu but can not be larger than 64",
		      hdr->firstBlock);
	goto done;
    }
    val++;
    if (hdr->reservedIndexes != AVL_INDEX) {
	sprintf (buf, "AVL index is %d, but should be %d",
		 hdr->reservedIndexes, AVL_INDEX);
	goto done;
    }
    val++;
    if (!hdr->bitmapIndex) {
	sprintf (buf, "Bitmap index was zero");
	goto done;
    }
    val++;

    sprintf (buf, "aggregate header okay");
    val = 10000;
done:
    afsl_PAssert (buf[len-1] == 0, ("print buffer overflowed: %s", buf));
    return val;
}
#endif

/* CustomizeMinor -- sets up any state needed to support upward compatibility
 *     of disk format minor version numbers. */

static long CustomizeMinor(IN struct aggregate *h)
{
    static int checkedFidStyle;

    /* Check if we have run the salvager to properly set the visible fields of
     * all anodes. */

    if (h->hdr.diskFormatMinorVersion <= EPIG_OLD_MINOR_VISIBLE_Q) {
	afsl_TraceEnabled (1, 1, AFSL_TR_ALWAYS,
 ("CustomizeMinor: incompatible minor version (%d) need at least %d.  Run salvage.",
  h->hdr.diskFormatMinorVersion, EPIG_OLD_MINOR_VISIBLE_Q+1));
	return 25/*ENOTTY*/;
    }

    /* Check minor version number and set fid style appropriately, we only
     * support one or the other, not per aggregate (or volume) basis. */

    if (!checkedFidStyle) {
	checkedFidStyle = 1;
	if (h->hdr.diskFormatMinorVersion <= EPIG_OLD_MINOR_FIDINDEX) {
	    printf ("InitAggregate: using old style fid indexes\n");
	    epit_useOldFidIndex = 1;
	}
    } else {
	if ((epit_useOldFidIndex == 1) !=
	    (h->hdr.diskFormatMinorVersion <= EPIG_OLD_MINOR_FIDINDEX))
	    return EPI_E_NOTANAGGREGATE;
    }
    return 0;
}

/* GetAggrHdrFromDisk -- locates a valid superblock from a device using the
 *     static array of superblock locations.  It tries each location in turn
 *     until it finds one that is reasonable looking.  The block number of the
 *     superblock and its header structure are returned on success.
 *
 * CAUTIONS -- Due to problems handling I/O errors during aggregate
 *     initialization the search for alternate superblocks was causing panics
 *     if there were no good superblocks.  Since our primary caller
 *     (ultimately, epig_InitAggregate) will fail if we don't return the
 *     primary superblock, and since the salvager doesn't presently
 *     support using the alternates, we will omit the search and only return
 *     the primary or an error. -ota 930817
 *
 * RETURN CODES -- Returns EPI_E_NOTANAGGREGATE if it finds a recognizable but
 *     inadequate superblock or if no recognizable ones are found.  Unexpected
 *     errors are also mapped to this code. */

#define DISABLE_ALTERNATE_SUPERBLOCKS 1

static long GetAggrHdrFromDisk(
 struct async_device *device,
 daddr_t *superBlockP,
 struct diskAggregateHeader *hdr)
{
    int i;
    long code = EPI_E_NOTANAGGREGATE;

    epix_EnterCodeBlock(11);
    for (i=0;
#ifdef DISABLE_ALTERNATE_SUPERBLOCKS
	 i<1;				/* ignore all but primary */
#else
	 i<EDSK_MAXNSUPERBLOCKS;
#endif
	 i++) {
	u_long blkno = edsk_superBlockLocations[i];
	u_long block;

	if (blkno == 0) continue;
	block = blkno * (EPISODE_SUPERBLOCK_QUANTUM >> device->logBlkSize);

	BEGIN_LCRIT (struct elbb_buffer *b,
		     code = elbb_Read (device, block, elbb_bufLogged, &b),
		     code = elbb_Release (b, elbb_lazy)) {
	    int val;			/* goodness value */
	    opaque data = elbb_BufferData (b);
	    u_long offset = epic_IndexToOffset (0);

	    code = HowMuchLikeSuperBlock (0, data, &val);
	    if (code)
		EXIT_CRIT;
	    if (val < 10) { ABORT_CRIT (EPI_E_NOTANAGGREGATE) }
	    *hdr = *(struct diskAggregateHeader *)((caddr_t)data + offset);
	} END_CRIT;
	if (code) {			/* try next superBlock location */
	    epix_EnterCodeBlock(12);
	    continue;
	}
	*superBlockP = blkno * (EPISODE_SUPERBLOCK_QUANTUM / hdr->blockSize);
	return 0;
    }
    if (code) {
	afsl_Trace (AFSL_TR_ERRMAP, ("GetAggrHdrFromDisk: got code %u", code));
	return EPI_E_NOTANAGGREGATE;
    }
    return 0;
}

/* GetGoodDevice -- obtains various device/aggregate parameters given a device
 *     major-minor pair.  There are two steps.  First try to get an existing
 *     async device ptr if one exists.  The trick here is to call asevent
 *     InitDevice with a block size of zero; if it can find one for the
 *     specified major/minor it will return it, but it will not create if not
 *     found.  If this succeeds we read the superblock and return.  If there is
 *     no existing device the next step is to determine the aggregate's block
 *     size and create an appropriate async device.  We do this by creating a
 *     dummy device with a 1K bsize and use it to read the superblock.  Since
 *     the superblock (and all the alternates) are at well known *byte* offsets
 *     this can be accomplished without reference to the correct block size.
 *     With the superblock in hand we shutdown/close the dummy device and
 *     create a device with the appropriate block size. */

static long GetGoodDevice(
 int maj,				/* device numbers */
 int min,
 struct async_device **deviceP,
 daddr_t *superBlockP,
 struct diskAggregateHeader *hdr)
{
    long code;
    struct async_device *device;

    epix_EnterCodeBlock(34);
    if (!deviceP) deviceP = &device;
    code = asevent_InitDevice (maj, min, 0, deviceP);
    if (code == 0) {
	code = GetAggrHdrFromDisk(*deviceP, superBlockP, hdr);
	if (code) return code;
	if ((*deviceP)->fragSize == 0) {
	    code = SetFragSize (*deviceP, hdr->fragmentSize);
	    if (code) {
		afsl_Trace (AFSL_TR_ERRMAP,
			    ("GetGoodDevice: SetFragSize got code %u", code));
		return (EPI_E_NOTANAGGREGATE);
	    }
	}
	return 0;
    }
    if (code != ASEVENT_NODEVICE) return code;

    /* open device w/ dummy block size */
    epix_EnterCodeBlock(14);

    BEGIN_LCRIT (struct async_device *dumdevice, /* dummy device */
		 code = asevent_InitDevice (maj, min, 1024, &dumdevice),
		 {   code = elbb_Shutdown (dumdevice);
		     code = asevent_CloseDevice(dumdevice); }) {
	code = GetAggrHdrFromDisk (dumdevice, superBlockP, hdr);
    } END_CRIT;
    if (code) return code;

    /* now get real device */
    code = asevent_InitDevice (maj, min, hdr->blockSize, deviceP);
    if (code) return code;
    /* Must set fragment size before epix_InitAggregate so that its Open()
     * doesn't abort */
    code = SetFragSize (*deviceP, hdr->fragmentSize);
    if (code) return code;
    return 0;
}

static long GetGoodAggregate(
  struct async_device *device,
  daddr_t superBlock,
  struct diskAggregateHeader *hdr,
  int initLog,
  struct aggregate **handleP)
{
    long code;
    epi_anode_t avl;
    struct aggregate *h;
    daddr_t primarySuperBlock;

    epix_EnterCodeBlock(15);

    primarySuperBlock = edsk_superBlockLocations[0] *
	(EPISODE_SUPERBLOCK_QUANTUM / hdr->blockSize);
    if (primarySuperBlock != superBlock) {

	/* Don't do this.  Run salvager to clean things up properly. */

	afsl_Trace (AFSL_TR_ERRMAP,
		    ("Using alternate superblock at block %#x", superBlock));
	return EPI_E_NOTANAGGREGATE;
    }

    code = epix_InitAggregate (device, 0, superBlock, hdr->reservedIndexes,
			       &avl);
    if (code) {
	afsl_Trace (AFSL_TR_ERRMAP,
		    ("GetGoodAggregate: epix_InitAggregate got code %u",
		     code));
	return (EPI_E_NOTANAGGREGATE);
    }
    if (GetFairAggr(h, avl)) {
	/* Already have a handle.  If it is initialized, which is more or less
	 * synonymous with (refCount > 0) just return.  Otherwise, re-attempt
	 * initialization. */
	epix_EnterCodeBlock(16);
	BEGIN_TMUTEX_NT (&h->lock) {
	    if (h->flags & AGGR_OFFLINE) {
		/* At the moment we can't recover from failure during
		 * close (refCount>0) or during open (log inited but
		 * not put online). */
		if ((h->refCount > 0) ||
		    (h->flags & AGGR_LOGGED) || h->allocationHandle) {
		    ABORT_CRIT (EPI_E_AGGOFFLINE);
		}
		/* Since we hold both the handleLock and the
		 * aggregate's lock, there can not be anyone else
		 * trying to init this aggregate.  Thus we have a
		 * closed aggregate that wasn't freed (shouldn't
		 * happen) or a failed open.  In either case, just
		 * re-init this sucker */
		afsl_Assert(h->avl == 0);
		afsl_Assert(FindAggregate(device) == 0);
	    } else {
		/* A duplicate call to init */
		afsl_Assert(h->refCount > 0);
		afsl_Assert(h->avl == avl);
		afsl_Assert(h == FindAggregate(device));
		if (h->flags & AGGR_LOGGED) /* we've called x_I.A. */
		    epia_SetDeviceLogged (avl);
		/* Since we already have a ptr to the avl in the handle,
		 * release this one. */
		code = epia_Close (avl);
		if (code)
		    EXIT_CRIT;
		*handleP = h;		/* we'll return this */
	    }
	} END_LOCK;
	if (code)
	    return code;
	if (h->refCount)
	    return 0;	/* we're done */

	Cleanup (avl);			/* flush old handle */
    }

    /* Now initialize an aggregate handle. */

    epix_EnterCodeBlock(17);
    h = osi_Alloc_r(sizeof (*h));
    bzero((caddr_t)h, sizeof (*h));
    h->hdr = *hdr;
    h->refCount = 0;
    h->flags = AGGR_OFFLINE;
    h->device = device;
    tmutex_Init (&h->lock, "aggregate");

    /* After this point, "h" is managed by the AVL cleanup handler.  Closing
     * the avl on error exit, will (eventually) free it. */

    BEGIN_TMUTEX_NT (&h->lock) {
	epi_anode_t bitmap;

	h->avl = avl;
	h->timeInited =  osi_Time();
	epia_SetFileRock (avl, h, Cleanup);
	*handleP = h;

	if (!AGGREGATE_HDR_OK(hdr, device))
	    { ABORT_CRIT (EPI_E_NOTANAGGREGATE) }

	code = CustomizeMinor (h);
	if (code)
	    EXIT_CRIT;

	if (initLog) {
	    if ((hdr->diskFormatMinorVersion > EPIG_OLD_MINOR_TIMESTAMPS) &&
		(hdr->lastVerifyCode != 0 && hdr->lastVerifyCode != 3)) {

		/* The last verification indicate problems with this aggregate
                 * so don't let it be put on line. */

		ABORT_CRIT (EPI_E_DAMAGEDAGGREGATE);
	    }
	    code = InitLog (avl, hdr->logIndex, hdr->nLogBlocks, 0);
	    if (code)
		EXIT_CRIT;
	    epia_SetDeviceLogged (avl);
	    h->flags |= AGGR_LOGGED;
	}

	code = epit_InitVolumeTable (avl, hdr->reservedIndexes);
	if (code)
	    EXIT_CRIT;

	DBprint (("Using indexes: bitmap %d, log %d, and badblocks %d",
		  h->hdr.bitmapIndex, h->hdr.logIndex, h->hdr.badblocksIndex));

	code = epia_Open (avl, h->hdr.bitmapIndex, 0, 0, &bitmap);
	if (code)
	    EXIT_CRIT;
	code = epib_InitAllocationPool (bitmap, hdr->firstBlock, hdr->nBlocks,
					&h->allocationHandle);
	if (code)
	    EXIT_CRIT;
	epia_SetAllocationHandle (avl, h->allocationHandle);

	/* everything looks good */
	h->flags &= ~AGGR_OFFLINE;
	afsl_Assert(h->refCount == 0);
    } END_LOCK;
    if (code) {
	h->avl = 0;
	(void) epia_Close(avl);
    }
    return code;
}

/* epig_InitAggregate -- Get a handle for the aggregate on the specified device
 *     identified by the \argname{maj}/\argname{min} pair.  This procedure is
 *     the means to make an Episode aggregate available for use.  The avl is
 *     released by calling \fcname{epig_CloseAggregate}.
 *
 * PARAMETERS --
 *     maj -- major device number.
 *     min -- minor device number.
 *     logged -- whether to turn on logging.  Usually true, only false for some
 *	 debug and test programs.
 *     deviceP -- pointer to the physical device handle associated with the
 *	 aggregate.  May be zero.
 *     avlP -- pointer to the AVL corresponding to the aggregate.
 *
 * LOCKS USED --
 *     A global handle lock is held while a the aggregate handle is allocated
 *     and associated with the avl anode.  This lock also protects the
 *     refCount.  Once the handle is allocated, locked, its refCount bumped and
 *     stashed in the avl we can release the handleLock and proceed with the
 *     majority of the initialization under protection of the lock in the
 *     handle itself.  The fields initialized under the handleLock is
 *     contrained by those that need to be valid to make the Cleanup procedure
 *     happy.
 *
 * ENVIRONMENT --
 *     Since this initialization procedure may be called repeatedly, even in
 *     the face of arbitrary failures, we have to cleanup carefully on error
 *     exits.  The most important requirement is to flush any in-core cached
 *     information.  This allows user mode disk operations that may invalidate
 *     such cached data.
 */

/* EXPORT */
long epig_InitAggregate(
  int maj,			/* device numbers */
  int min,
  int logged,			/* turn on logging */
  struct async_device **deviceP,
  epi_anode_t *avlP)
{
    long code;
    struct async_device *device;

    epix_EnterCodeBlock(13);
    if (deviceP) *deviceP = 0;
    if (avlP) *avlP = 0;

    do {
	code = 0;
	BEGIN_LOCK_WRITE (&handleLock) {
	    daddr_t superBlock;
	    struct diskAggregateHeader hdr;
	    struct aggregate *h;

	    device = 0;
	    code = GetGoodDevice (maj, min, &device, &superBlock, &hdr);
	    if (code)
		EXIT_CRIT;

	    code = GetGoodAggregate (device, superBlock, &hdr, logged, &h);
	    if (code)
		EXIT_CRIT;
	    h->refCount++;
	    if (!FindAggregate(device)) {
		code = AddAggregate (h);
		afsl_MBZ(code);
	    }
	    if (avlP) *avlP = h->avl;
	} END_LOCK;
	if (code && device) {
	    epix_SetGConClose (epix_gc_device, (opaque)device);
	    (void) elbb_Shutdown (device);
	    (void) asevent_CloseDevice (device);
	}
    } while (code == RCVR_RECOVERED);
    if (code) return code;
    if (deviceP) *deviceP = device;
    return 0;
}

/* epig_GetAggregateStatus -- Obtains the attributes corresponding to the
 *     aggregate associated with the \argname{avl}.
 *
 * PARAMETERS --
 *     avl -- handle for the aggregate.
 *     StatusP -- pointer to attributes of the aggregate.  See
 *	 section~\ref{aggrdtref} for details on \typename{struct
 *	 epig_status}.
 *
 * LOCKS USED -- Not locking done here because all (MOST?) fields in the
 *     superblock are immutable.
 *
 * PEROFRMANCE -- should cache the superblock in the aggregate handle. */

/* EXPORT */
long epig_GetAggregateStatus(epi_anode_t avl, struct epig_status *StatusP)
{
    struct aggregate *h;
    long code;
    struct epib_info info;

    epix_EnterCodeBlock(18);
    if (StatusP == 0) return EPI_E_BADARGS;
    if (!GetOKagg (h, avl)) return EPI_E_BADAGGREGATE;
    if (avl->block == 0) return EPI_E_BADAGGREGATE;
    StatusP->whichSuperBlockUsed = avl->block;
    StatusP->deviceMajor = h->device->majdev;
    StatusP->deviceMinor = h->device->mindev;
#if 0
    BEGIN_LCRIT (struct elbb_buffer *b,
		 code = elbb_Read (h->device, avl->block, elbb_bufLogged, &b),
		 code = elbb_Release (b, elbb_lazy)) {
	int val;			/* goodness value */
	opaque data = elbb_BufferData (b);
	u_long offset = epic_IndexToOffset (0);

	code = HowMuchLikeSuperBlock (h->device, data, &val);
	if (code)
	    EXIT_CRIT;
	if (val < 10) { ABORT_CRIT (EPI_E_NOTANAGGREGATE) }
	hdr = *(struct diskAggregateHeader *)((caddr_t)data + offset);
    } END_CRIT;
    if (code) return code;
#endif
    StatusP->blockSize = h->hdr.blockSize;
    StatusP->fragmentSize = h->hdr.fragmentSize;
    StatusP->firstBlock = h->hdr.firstBlock;
    StatusP->nBlocks = h->hdr.nBlocks;
    StatusP->nLogBlocks = h->hdr.nLogBlocks;
    StatusP->reservedIndexes = h->hdr.reservedIndexes;
    StatusP->bitmapIndex = h->hdr.bitmapIndex;
    StatusP->logIndex = h->hdr.logIndex;
    StatusP->badblocksIndex = h->hdr.badblocksIndex;
    StatusP->timeCreated = h->hdr.timeCreated;
    StatusP->diskFlags = h->hdr.flags;
    StatusP->coreFlags = h->flags;
    StatusP->timeInited = h->timeInited;
    StatusP->creatingProgram = h->hdr.creatingProgram;
    StatusP->diskFormatMinorVersion = h->hdr.diskFormatMinorVersion;
    StatusP->diskFormatMajorVersion = DiskFormatMajorVersion(&h->hdr);
    StatusP->lastVerifyTime = h->hdr.lastVerifyTime;
    StatusP->lastVerifyCode = h->hdr.lastVerifyCode;
    afsl_Assert (sizeof(StatusP->spare) == sizeof(h->hdr.spare));
    bcopy((caddr_t)h->hdr.spare, (caddr_t)StatusP->spare,
	sizeof (StatusP->spare));
    StatusP->fragsPerBlock = h->device->blkSize >> h->device->logFragSize;

    code = epib_GetInfo(h->allocationHandle, &info);
    if (code) return code;
    StatusP->sparel1 = 0;
    StatusP->fragmentsFree = info.avail;
    StatusP->fragmentsFreeReally = info.realFree;
    StatusP->fragmentsFreePartial = info.freeFrags;
    return 0;
}

/* epig_GetFragmentSize -- Returns the size of the fragments (in bytes) in use
 *     by the aggregate.
 *
 * PARAMETERS --
 *     avl -- handle for the aggregate.
 *     fragmentSizeP -- pointer to the fragment size.
 *
 * LOCKS USED -- Doesn't obtain a lock because the fragment size is immutable,
 *     once anyone has initialized the aggregate it never changes. */

/* EXPORT */
long epig_GetFragmentSize(epi_anode_t avl, u_long *fragmentSizeP)
{
    struct aggregate *h;		/* aggregate info */

    epix_EnterCodeBlock(19);
    if (fragmentSizeP == 0) return EPI_E_BADARGS;
    if (!GetOKagg (h, avl)) return EPI_E_BADAGGREGATE;
    *fragmentSizeP = h->hdr.fragmentSize;
    return 0;
}

static long EnterVolume(
 char *rock,
 epi_anode_t volC,
 u_long index)
{
    long code;
    epi_anode_t h;
    struct epia_info info;
    struct epiv_ident ident;

    epix_EnterCodeBlock(20);
    BEGIN_CRIT (code = epia_Open (volC, index, 0, 0, &h),
		code = epia_Close (h)) {
	code = epia_GetInfo (h, &info);
	if (code)
	    EXIT_CRIT;
	if ((info.flags & EPIA_CFLAGS_TYPE) != EPIA_CFLAGS_VOLUME) {
	    epix_EnterCodeBlock(21);
	    continue;
	}
	code = epiv_GetIdent (h, &ident);
	if (code) { ABORT_CRIT (EPI_E_NOTAVOLUME) }

	code = vr_AddVolume (ident.id, ident.name, volC, index);
	if ((code == EPI_E_VOLIDEXISTS) || (code == EPI_E_VOLNAMEEXISTS)) {
	    epix_EnterCodeBlock(22);
	    DBprint (("Skipping attach of volume with duplicate id (#%u,,%u) or name %s",
		      AFS_HGETBOTH(ident.id), ident.name));
	    code = 0;
	}
    } END_CRIT;
    return code;
}

static long RemoveVolume(
 char *rock,
 epi_anode_t volC,
 u_long index)
{
    long code;
    epi_anode_t h;

    epix_EnterCodeBlock(23);
    BEGIN_CRIT (code = epia_Open (volC, index, 0, 0, &h),
		code = epia_Close (h)) {
	struct epiv_ident ident;
	epi_anode_t avl;
	epi_anode_t volAvl;
	u_long index;

	if (epia_GetType(h) != EPIA_CFLAGS_VOLUME) continue;
	code = epiv_GetIdent (h, &ident);
	if (code) { ABORT_CRIT (EPI_E_NOTAVOLUME) }

	code = vr_LookupVolumeId (ident.id, &avl, &index);
	if (code == EPI_E_NOENT) {
	    epix_EnterCodeBlock(24);
	    code = 0;
	    continue;
	}
	if (code)
	    EXIT_CRIT;

	/* If volume is already closed this will return NOTAVOLUME */
	code = epiv_GetAggregate (h, &volAvl);
	if ((code == 0) && ((avl != volAvl) || (index != epia_GetIndex(h))))
	    /* some how got the wrong volume */
	    { ABORT_CRIT (EPI_E_VOLIDEXISTS); }
	if (code && (code != EPI_E_BADVOLUME)) continue;
	code = vr_RemoveVolume (ident.id, avl, index);
	if (code)
	    EXIT_CRIT;
    } END_CRIT;
    return code;
}

/* epig_AttachAggregate -- Test procedure only. */

/* SHARED */
long epig_AttachAggregate(IN epi_anode_t avl)
{
    long code;
    struct aggregate *h;		/* aggregate info */

    epix_EnterCodeBlock(25);
    code = vr_Init ();
    if (code) return code;

    if (!GetOKagg (h, avl)) return EPI_E_BADAGGREGATE;
    BEGIN_TMUTEX_NT (&h->lock) {
	h->flags |= AGGR_ATTACHED;
	code = epit_EnumerateVolumeTable (avl, EnterVolume, 0);
    } END_LOCK;
    return code;
}

/* epig_DetachAggregate -- only used internally, but perhaps of some value in
 *     test programs.
 *
 * LOCKS USED --
 *     Use exclusive lock on aggregate since this can't be reentrant. */

/* SHARED */
long epig_DetachAggregate(IN epi_anode_t avl)
{
    long code;
    struct aggregate *h;		/* aggregate info */

    epix_EnterCodeBlock(26);
    if (!GetOKagg (h, avl)) return EPI_E_BADAGGREGATE;
    code = 0;
    BEGIN_TMUTEX_NT (&h->lock) {
	if (!(h->flags & AGGR_ATTACHED)) { ABORT_CRIT (0) }
	if (h->openVolumes > 0) { ABORT_CRIT (EPI_E_OPENVOLUMES) }
	code = epit_EnumerateVolumeTable (avl, RemoveVolume, 0);
	if (code)
	    EXIT_CRIT;
	h->flags &= ~AGGR_ATTACHED;
    } END_LOCK;
    return code;
}

/* epig_CloseAggregate -- Releases an aggregate that was obtained earlier by
 *     calling \fcn{epig_InitAggregate}.  All the filesets must be closed
 *     before calling this.  It also decrements the reference count and closes
 *     the AVL for the aggregate.  When the reference count reaches zero (on
 *     the last close), all cached data associated with the aggregate is
 *     flushed to the disk and the in-core state is invalidated.  For instance,
 *     this means that if the disk is on removable media it could be safely
 *     unloaded after the last call to this routine returns. */

/* EXPORT */
long epig_CloseAggregate(epi_anode_t avl)
{
    long code;
    struct aggregate *h;		/* aggregate info */
    struct async_device *device;

    epix_EnterCodeBlock(27);
    if (!GetOKagg (h, avl)) return EPI_E_BADAGGREGATE;

    lock_ObtainWrite_r (&handleLock);
    if (h->refCount > 1) {
	epix_EnterCodeBlock(28);
	h->refCount--;
	lock_ReleaseWrite_r (&handleLock);
	return 0;			/* someone else will do it */
    }

    /* At this point we have a refCount of 1 so we're going to really close it
     * down.  We need make it offline so we can work in peace.  No other
     * closers should attempt this code (since everyone else has closed it
     * already).  However, to interlock against other callers of InitAggregate,
     * we'll hold the handleLock over the whole (relatively rare) operation.
     * The lock on the handle (which we can't hold, because the handle itself
     * will be freed) is probably gratuitous (since we are the last close). */

    BEGIN_CRIT (afsl_AssertWriteLocked(&handleLock),
		lock_ReleaseWrite_r(&handleLock)) {
	BEGIN_TMUTEX_NT (&h->lock) {
	    if (h->openVolumes > 0) { ABORT_CRIT (EPI_E_OPENVOLUMES) }
	    afsl_Assert(!(h->flags & AGGR_OFFLINE));
	    h->flags |= AGGR_OFFLINE;
	} END_LOCK;
	if (code)
	    EXIT_CRIT;
	device = epia_AnodeDevice(avl);

	code = epig_DetachAggregate (avl);
	if (code)
	    EXIT_CRIT;

	code = epib_CloseAllocationPool (h->allocationHandle);
	if (code)
	    EXIT_CRIT;
	h->allocationHandle = 0;

	/* Now we're really going to close the avl so zero the refCount. */
	code = DeleteAggregate (h);
	afsl_MBZ(code);
	afsl_Assert(h->refCount == 1);
	h->refCount = 0;		/* let Cleanup free it */
	h->avl = 0;
	h->flags &= ~AGGR_LOGGED;
	code = epia_Close (avl);
	afsl_MBZ(code);

	/* Check for open anodes. Should this be conditionally defined? */
	{   long count;
	    code = epix_CountOpenAnodes (device, &count);
	    if (code)
		EXIT_CRIT;
	    afsl_Assert(count == 0);
	}

	/* Garbage collect all the anodes from this device */
	code = epix_SetGConClose (epix_gc_device, (opaque)device);
	if (code)
	    EXIT_CRIT;

	code = elbb_Shutdown (device);
	afsl_MBZ(code);			/* deep trouble if this fails */

	code = asevent_CloseDevice (device);
    } END_CRIT;				/* release handleLock */
    return code;
}

/* epig_CreateVolume -- Create a fileset with the attributes specified by
 *     \argname{status}.  There is some support for testing without the normal
 *     kernel volume registry, namely, if the flag bit
 *     \constname{EPIG_NOATTACH} is not specified, the newly created fileset is
 *     attached in the test environment's volume registry.  Normal usage always
 *     sets this bit.
 *
 *     The new fileset is not opened.  Use the index returned in
 *     \argname{indexP} to obtain a fileset handle from
 *     \fcname{epig_OpenVolume}.
 *
 * PARAMETERS --
 *     trans -- transaction to use while creating the fileset.
 *     avl -- handle for the aggregate.
 *     flags -- specifies whether the fileset should be attached after
 *	 creation.
 *     status -- attributes of the fileset being created.  See volume.h for
 *	 details on \typename{struct epiv_status}.
 *     indexP -- pointer to the index of the fileset in the AVL. */

/* EXPORT */
long epig_CreateVolume(
  elbb_tranRec_t trans,
  epi_anode_t avl,
  long flags,
  struct epiv_status *status,
  u_long *indexP)
{
    long code;
    struct aggregate *h;

    epix_EnterCodeBlock(29);
    if (!GetOKagg (h, avl)) return EPI_E_BADAGGREGATE;

    if (!(flags & EPIG_NOATTACH)) {
	code = vr_LookupVolumeId (status->ident.id, 0,0);
	if (code == 0) return EPI_E_VOLIDEXISTS;
	if (code != EPI_E_NOENT) return code;
	code = vr_LookupVolumeName (status->ident.name, 0,0);
	if (code == 0) return EPI_E_VOLIDEXISTS;
	if (code != EPI_E_NOENT) return code;
    }

    /* The error handling here is quite complicated.  Basically, though, we
     * just undo each step if there is an error farther along. */

    BEGIN_TMUTEX (trans, &h->lock) {
	if (h->flags & AGGR_OFFLINE) { ABORT_CRIT (EPI_E_AGGOFFLINE); }
	BEGIN_LCRIT (u_long index,	/* index of volume in aggregate */
		     code = epit_Allocate (trans, avl, (EPIT_TYPE_DONTCARE|
							EPIT_DONT_WITHHOLD),
					   0, &index),
		     if (code) { /* give back index */
			 long ccode = epit_Free (trans, avl, index);
			 afsl_MBZ(ccode);
		     }) {
	    BEGIN_LCRIT (epi_anode_t volC = 0, /* volume table container */
			 code = epia_Create
			     (trans, avl, index,
			      EPIA_CFLAGS_LOGGED + EPIA_CFLAGS_VOLUME,
			      sizeof(struct epiv_ident), 0,
			      status->ident.id, 0, 0, &volC),
			 if (code) { /* delete anode */
			     long ccode;
			     ccode = epia_Delete (trans, volC);
			     afsl_MBZ(ccode);
			 }) {
		epia_SetAllocationHandle (volC, h->allocationHandle);

		code = epiv_Create (trans, avl, volC, status);
		if (code)
		    EXIT_CRIT;

		if (!(flags & EPIG_NOATTACH)) {

		    /* The only errors this can return are detected above
		     * in the call to vr_LookupVolumeId/Name. */

		    epix_EnterCodeBlock(31);
		    code = vr_AddVolume (status->ident.id,
					 status->ident.name,
					 avl, index);
		    afsl_MBZ(code);
		}
		if (code)
		    EXIT_CRIT;
		code = epia_Close (volC);
	    } END_CRIT;
	    if (code)
		EXIT_CRIT;
	    *indexP = index;		/* return this */
	} END_CRIT;
    } END_LOCK;
    if (code) return code;
    return 0;
}

/* epig_OpenVolume -- This opens the fileset specified by \argname{index} in
 *     the \argname{avl}. This is the mechanism for getting a useful handle for
 *     a fileset.
 *
 * PARAMETERS --
 *     avl -- aggregate handle
 *     index -- index of the fileset in the AVL.
 *     volP -- pointer to the handle for the opened fileset. */

/* EXPORT */
long epig_OpenVolume(epi_anode_t avl, u_long index, epi_volume_t *volP)
{
    long code;
    struct aggregate *h;

    epix_EnterCodeBlock(32);
    if (!GetOKagg (h, avl))
	return EPI_E_BADAGGREGATE;
    code = 0;
    BEGIN_TMUTEX_NT (&h->lock) {
	if (h->flags & AGGR_OFFLINE)
	    { ABORT_CRIT (EPI_E_AGGOFFLINE) }
	h->openVolumes++;
    } END_LOCK;
    if (code)
	return code;

    code = epiv_Open (avl, index, volP);
    if (code) {
	BEGIN_TMUTEX_NT (&h->lock) {
	    h->openVolumes--;
	} END_LOCK;
	return code;
    }
    return 0;
}

/* epig_CloseVolume -- Closes a fileset handle which was obtained from
 *     \fcname{epig_OpenVolume}.
 *
 * PARAMETERS --
 *     trans -- transaction to be used for the operation.  This is unused and
 *	 may be null.
 *     volC -- fileset handle.
 *
 * LOCKS USED -- Use an exclusive lock to protect openVolumes field.  No disk
 *     structures are modified so no transaction need be provided, even though
 *     a transaction is available. */

/* EXPORT */
long epig_CloseVolume(elbb_tranRec_t trans, epi_volume_t volC)
{
    long code;
    epi_anode_t avl;
    struct aggregate *h;

    epix_EnterCodeBlock(33);
    code = epiv_GetAggregate (volC, &avl);
    if (code) return code;
    if (!GetOKagg (h, avl))
	return EPI_E_BADAGGREGATE;

    afsl_Assert (h->openVolumes > 0);

    code = epiv_Close (trans, volC);
    if (code)
	return code;

    BEGIN_TMUTEX_NT (&h->lock) {
	afsl_Assert (h->openVolumes > 0);
	h->openVolumes--;
    } END_LOCK;
    return 0;
}

/* epig_DeleteVolume -- This procedure deletes and closes the fileset (which
 *     must be empty) associated with the handle \argname{volC}.
 *
 * PARAMETERS --
 *     trans -- transaction to use while deleting the fileset.
 *     volC -- handle for the fileset to delete. */

/* EXPORT */
long epig_DeleteVolume(elbb_tranRec_t trans, epi_volume_t volC)
{
    long code;
    epi_anode_t avl;
    struct aggregate *h;
    u_long index;

    epix_EnterCodeBlock(33);
    code = epiv_GetAggregate (volC, &avl);
    if (code) return code;
    if (!GetOKagg (h, avl)) return EPI_E_BADAGGREGATE;
    index = epia_GetIndex (volC);
    (void) RemoveVolume (0, avl, index);

    BEGIN_TMUTEX (trans, &h->lock) {
	afsl_Assert (h->openVolumes > 0);
	code = epiv_Delete (trans, volC);
	if (code)
	    EXIT_CRIT;
	h->openVolumes--;

	/* can't recover past here. */
	code = epit_Free (trans, avl, index);
	afsl_MBZ(code);
    } END_LOCK;
    return code;
}

#ifndef KERNEL
#include <dcedfs/episode/us.h>
#include "verify.h"
#include "salvage.h"
#include <com_err.h>

static long epiz_AllocResFrags(
 struct epiz_salvageState *ss,
 epi_anode_t vol,
 epi_anode_t file,
 struct epiz_anodeItem *item)
{
    int code;
    struct epix_fragDescription newFrags;       /* to (new) desc */

    afsl_Assert (ss->bitmap && (file != 0) && (item != 0) &&
		 ((item->flags&EPIZ_AI_ALLOC) == 0));

    code = epib_AllocateFrags(elbb_zeroTrans, file,
			      EPIB_MOVE_FRAGS|EPIB_DONT_WITHHOLD,
			      item->aiFragLen, &newFrags);
    assert(newFrags.length == item->aiFragLen);
    if (code == 0) {
	item->aiNewFrag = newFrags.block;
	item->aiNewFirst = newFrags.first;
	item->flags |= EPIZ_AI_ALLOC;
    } else if (code == EPI_E_AGGREGATEFULL) {
	printf( "epiz_AllocResFrags: Aggregate full for Volume %x/Anode %x\n",
		item->anode->volIx, item->anode->anodeIx );
	item->flags |= EPIZ_AI_AFULL;
	item->aiNewFrag = EPIX_ANODE_BLOCKEMPTY;
	item->aiNewFirst = 0;
	code = 0;
    }
    afsl_MBZ(code);
    return (0);

}

static long epiz_AllocResBlocks(
 struct epiz_salvageState *ss,
 epi_anode_t vol,
 epi_anode_t file,
 struct epiz_anodeItem * item)
{
    int code;
    daddr_t NewBlkArr[1];
    u_long nBlocks = 1;

    afsl_Assert (ss->bitmap && ss->simBits && (file != 0) && (item != 0) &&
		 ((item->flags&EPIZ_AI_ALLOC) == 0));

    code = epib_Allocate(elbb_zeroTrans, file, &nBlocks, NewBlkArr,
			 EPIB_DONT_WITHHOLD);

    if (code == 0) {
	item->aiNewBlock = NewBlkArr[0];
	item->flags |= EPIZ_AI_ALLOC;
    } else if (code == EPI_E_AGGREGATEFULL) {
	printf( "epiz_AllocResBlocks: Aggregate full for Volume %x/Anode %x\n",
		item->anode->volIx, item->anode->anodeIx );
	item->flags |= EPIZ_AI_AFULL;
	item->aiNewBlock = EPIX_ANODE_BLOCKEMPTY;
	code = 0;
    }
    afsl_MBZ(code);
    return (0);
}

/* epiz_FreeResFrags -- returns blocks allocated reserve frags that were
 *     unused. */

static long epiz_FreeResFrags(
 struct epiz_salvageState *ss,
 epi_anode_t vol,
 epi_anode_t file,
 struct epiz_anodeItem * item)
{
    int code;
    struct epix_fragDescription newFrags;       /* to (new) desc */

    afsl_Assert (file && item);
    if (!(item->flags & EPIZ_AI_ALLOC)) return 0;

    newFrags.block = item->aiNewFrag;
    newFrags.first = item->aiNewFirst;
    newFrags.length = item->aiFragLen;
    code = epib_FreeFrags( elbb_zeroTrans, file, 0/*keep*/, &newFrags );
    afsl_MBZ(code);
    item->flags &= ~EPIZ_AI_ALLOC;
    item->aiNewFrag = EPIX_ANODE_BLOCKEMPTY;
    item->aiFragLen = 0;
    return (0);
}

static long epiz_FreeResBlocks(
 struct epiz_salvageState *ss,
 epi_anode_t vol,
 epi_anode_t file,
 struct epiz_anodeItem *item)
{
    long code;

    afsl_Assert (file && item);
    if (!(item->flags & EPIZ_AI_ALLOC)) return 0;

    code = epib_Free (elbb_zeroTrans, file, 1, &item->aiNewBlock);
    afsl_MBZ(code);
    item->aiNewBlock = EPIX_ANODE_BLOCKEMPTY;
    item->flags &= ~EPIZ_AI_ALLOC;
    return (0);
}

static long DescribeBadSuperBlock(IN struct epiz_salvageState *ss)
{
    long code;
    daddr_t primarySuperBlock;

    primarySuperBlock = edsk_superBlockLocations[0] *
	(EPISODE_SUPERBLOCK_QUANTUM / ss->device->blkSize);
    BEGIN_LCRIT (struct elbb_buffer *b,
		 code = elbb_Read (ss->device, primarySuperBlock,
				   elbb_bufLogged, &b),
		 code = elbb_Release (b, elbb_lazy)) {
	opaque data = elbb_BufferData (b);
	char msg[500];

	epig_SayWhyNotLikeSuperBlock (ss->device, data, sizeof(msg), msg);
	epiz_ReportError (ss, 0, EPIZ_E_BADSB, msg);
    } END_CRIT;
    if (code) return code;
    return -1;
}

/* CheckAggregate -- is a function called by the salvager to carefully open an
 *     aggregate and initialize the salvageState structure from the superblock.
 *
 * CAUTIONS -- We need to track proper unwinding for everything we do here so
 *     the UncheckAggregate call can cleanup all this.  Generally, allocated
 *     storage is freed if the corresponding ptr is not zero.  If the aggregate
 *     handle doesn't have the AGGR_SALVAGE bit set then we are being called
 *     from a running system, so we'll just decrement the refCount.  Otherwise
 *     if the allocH is non-zero we'll call epib_CloseAllocationPool, otherwise
 *     we'll assume there is not valid bitmap.  This latter situation may need
 *     to change when we get around to rebuilding the bitmap; it may no longer
 *     be convenient to leave these fields zero. */

static long CheckAggregate(
 struct epiz_salvageState *ss,
 struct diskAggregateHeader *hdr)
{
    long code;
    struct async_device *device;
    daddr_t superBlock;
    daddr_t primarySuperBlock;
    epi_anode_t avl;
    struct aggregate *h;
    osi_off_t dev_size;			/* size of disk device */
    u_long realNBlocks;			/* nBlocks bounded by dev_size */

    device = ss->device;
    code = GetGoodDevice((int)device->majdev, (int)device->mindev, &device,
			  &superBlock, hdr);
    if (code) {

	/* Should dispatch on various codes.  GetGoodDevice can't print better
	 * msg because it also runs in the kernel. */

	return DescribeBadSuperBlock(ss);
    }
    afsl_Assert (device == ss->device);	/* insist we get same one */

    primarySuperBlock = edsk_superBlockLocations[0] *
	(EPISODE_SUPERBLOCK_QUANTUM / hdr->blockSize);
    if (primarySuperBlock != superBlock) {

	/* Don't bother trying to muddle along.  At best the alternate SB will
	 * indicate an empty aggregate.  Until we figure out how to update the
	 * alternates periodically there isn't must point in fail-over. */

	return DescribeBadSuperBlock(ss);
    }

    if (hdr->diskFormatMinorVersion < EPIG_DISK_MINOR_VERSION) {
	/* Just a warning of old version. */
	epiz_ReportError (ss, 0, EPIZ_E_OLDMINOR, hdr->diskFormatMinorVersion,
			  EPIG_DISK_MINOR_VERSION);
    }

    if (hdr->diskFormatMinorVersion <= EPIG_OLD_MINOR_VISIBLE_Q) {
	/* Just a warning because we will suppress the errors. */
	epiz_ReportError (ss, 0, EPIZ_E_NOVISIBLEQUOTAMSGS);
	epiz_SuppressError (EPIZ_E_BADVISIBLE);
    }

    dev_size = us_psize (osi_makedev(device->majdev, device->mindev));
#ifdef AFS_SUNOS5_ENV
    afsl_PAssert (dev_size > 0, ("Illegal disk device size: %d", dev_size));
#else /* AFS_SUNOS5_ENV */
    afsl_PAssert (dev_size > 0, ("Illegal disk device size: %lld", dev_size));
#endif /* AFS_SUNOS5_ENV */

    /* Do this calculation carefully to avoid wrapping if nBlocks+firstBlock >=
     * 2^32.  Also worry about either of these being >= 2^31 since often
     * (daddr_t) (e.g. ss->minBlock) is signed.  Clearly, dev_size is an
     * upperbound on nBlocks, but it also bounds nBlocks+firstBlock.  Actually
     * firstBlock has already been verified as no bigger than superBlock so we
     * could rely on that.  Apply these limits in the conservative order,
     * anyway, just to be safe. */

    {   u_long dev_blks = dev_size>>device->logBlkSize;
	ss->minBlock = MIN(dev_blks, (u_long)hdr->firstBlock);
	realNBlocks = MIN(dev_blks - ss->minBlock, hdr->nBlocks);
	ss->maxBlock = realNBlocks + ss->minBlock - 1;
    }

    code = epix_InitAggregate (device, 0, superBlock, hdr->reservedIndexes,
			       &avl);
    if (code) {
	com_err ("CheckAggregate", code, "initializing AVL");
	return (EPI_E_NOTANAGGREGATE);
    }
    if (epia_GetIndex (avl) != hdr->reservedIndexes) {
	epiz_ReportError (ss, 0, EPIZ_E_AVL_BADINDEX,
			  epia_GetIndex(avl), hdr->reservedIndexes);
	return -1;
    }
    {   daddr_t firstBlock;		/* of AVL */

	code = epia_Map (avl, 0, &firstBlock, 0);
	if (code) {
	    epiz_ReportError (ss, code, EPIZ_E_AVL_NOBLOCKZERO);
	    return -1;
	}
	else if (firstBlock != superBlock) {
	    epiz_ReportError (ss, 0, EPIZ_E_AVL_WRONGBLOCKZERO, firstBlock);
	    return -1;
	}
    }
    if (hdr->blockSize <
	((hdr->creatingProgram == EPIG_CREATOR_NEWAGGR)
	 ? ANODE_MIN_KERNEL_BLOCKSIZE : ANODE_MIN_BLOCKSIZE)) {
	epiz_ReportError (ss, 0, EPIZ_E_AVL_WRONGBLOCKSIZE, hdr->blockSize);
	return -1;
    }
    ss->aggrAnode = avl;

    if ((hdr->creatingProgram == EPIG_CREATOR_TEST_ANODE) &&
	!(ss->flags & EPIZ_SS_NODIRSCAN)) {
	printf ("Aggregate created with \"test_anode\" so no directory structure is present.\n");
	ss->flags |= EPIZ_SS_NODIRSCAN;
    }

    ss->simBits = osi_Alloc_r (sizeof(*ss->simBits)),
    bzero((caddr_t)ss->simBits, sizeof (*ss->simBits));
    ss->simBits->blkSize = device->blkSize;
    ss->simBits->logBlkSize = device->logBlkSize;
    ss->simBits->fragSize = device->fragSize;
    ss->simBits->logFragSize = device->logFragSize;
    ss->simBits->fragsPerBlock = device->blkSize >> device->logFragSize;

    ss->simBits->bitmapOffset = hdr->firstBlock;
    ss->simBits->bitsLen =		/* need a bit per frag (rounded up) */
	(realNBlocks * ss->simBits->fragsPerBlock + 7) >> 3;
    ss->simBits->bits = osi_Alloc_r(ss->simBits->bitsLen);
    /* we bzero this before each bitmap checking pass */

    {   /* decorate the well known containers */
	u_long avlIndex = epia_GetIndex(ss->aggrAnode);

#define Decorate(i, n) \
	if (i) epiz_DecorateAnode (epiz_GetAnode(i, avlIndex), n);

	Decorate (hdr->reservedIndexes, "<AVL Container>");
	Decorate (hdr->bitmapIndex,	"<Bitmap Container>");
	Decorate (hdr->logIndex,	"<Log Container>");
	Decorate (hdr->badblocksIndex,	"<Badblocks Container>");

#undef Decorate
    }

    /* Handle the case where we are called from a running process (e.g. a test
     * program) by just returning. */
    if (GetOKagg (h, avl)) {
	struct epib_info info;
	if (h->flags & AGGR_LOGGED) /* we've called x_I.A. */
	    epia_SetDeviceLogged (avl);
	/* Since we already have a ptr to the avl in the handle,
	 * release this one. */
	afsl_Assert (h->avl == avl);
	afsl_Assert (h->allocationHandle == epib_GetAllocationHandle(avl));
	code = epib_GetInfo (h->allocationHandle, &info);
	afsl_MBZ(code);
	ss->bitmap = info.bitmapC;
	code = epia_Close (avl);	/* punt extra copy */
	afsl_MBZ(code);
	h->refCount++;
    } else {
	/* Create a handle we can reconize later and fill it in.  Set other
	 * fields so we can cleanup properly.  */
	afsl_Assert (epia_GetFileRock(avl) == 0);

	h = osi_Alloc_r (sizeof(*h));
	bzero((caddr_t)h, sizeof (*h));
	h->hdr = *hdr;
	h->refCount = 0;
	h->flags = AGGR_SALVAGE;
	h->device = device;
	tmutex_Init (&h->lock, "aggregate (salvage)");
	h->avl = avl;
	h->timeInited =  osi_Time();
	h->refCount = 1;

	epia_SetFileRock (avl, h, /*no cleanup*/0);
	epia_SetAllocationHandle (avl, 0);
	ss->bitmap = 0;

	if (!AGGREGATE_HDR_OK (hdr, device)) {
	    /* this shouldn't happen due to checks above */
	    return (EPI_E_NOTANAGGREGATE);
	}

	code = CustomizeMinor (h);
	if (code == 25) code = 0;	/* we are running salvage! */
	if (code) {
	    com_err ("CheckAggregate", code,
		     "customizing for minor version %d",
		     h->hdr.diskFormatMinorVersion);
	    return code;
	}

	/* At this point we're pretty sure both that this is an AVL anode block
	 * and that the AVL header looks like a good superblock. */

	if (ss->flags & EPIZ_SS_LOGCLEAN) {

	    /* The log is known to be clean.  This means that we did recovery
	     * on the previous call to CheckAggregate so we skip the log
	     * processing this time. */

	} else {

	    /* Check to see if recovery is needed.  Any errors we discover here
	     * will be rediscovered later, so we make no effort to deal with
	     * them graciously. */

	    BEGIN_LCRIT (epi_anode_t log,
			 code = epia_Open (avl, hdr->logIndex, 0, 0, &log),
			 code = epia_Close (log)) {
		daddr_t first;		/* block #0 in log */
		int clean;

		code = epia_Map (log, 0, &first, NULL);
		if (code) continue;
		clean = elbr_IsLogClean (device, first);
		if (clean)
		    ss->flags |= EPIZ_SS_LOGCLEAN;
	    } END_CRIT;
	    if (code)
		epiz_ReportError (ss, code, EPIZ_E_LOG_CANNOTCHECKRECOVERY);
	    else if (!(ss->flags & EPIZ_SS_RECOVER)) {
		if (!(ss->flags & EPIZ_SS_LOGCLEAN))
		    epiz_ReportError (ss, 0, EPIZ_E_LOG_NEEDRECOVERY);
	    } else if (ss->flags & EPIZ_SS_LOGCLEAN) {
		epiz_ReportError (ss, 0, EPIZ_E_LOG_RECOVERYUNNEEDED);
	    } else {

		/* Try to do recovery, but first we must carefully check the
		 * log container.  If there are any problems we need to bailout
		 * so InitLog doesn't crash.  */

		ss->expectedType = EPIA_CFLAGS_LOG;
		ss->pass = EPIZ_SS_SCANPASS;

		/* TODO -- Call BitmapSetup to scan BadBlock file first? */
		/* No, because we'll do this all again during the real
		 * SCANPASS.  -ota 940405 */
		ss->blockRef = epiz_MarkUsed;
		bzero((char *)ss->simBits->bits, ss->simBits->bitsLen);

		code = epia_VerifyAnode (ss, ss->aggrAnode, hdr->logIndex);
		ss->volIx = ss->anodeIx = 0;
		if (code)
		    return code;
		code = epiz_GetExitCode (ss);
		if ((code != 0) && (code != 3))
		    return EPI_E_BADLOGBLOCKS; /* Other strange problems? */
		ss->nAnodes = ss->nIndirectBlocks = 0; /* reset statistics */

		code = InitLog (ss->aggrAnode, hdr->logIndex, hdr->nLogBlocks,
				INITLOG_CLOSE);
		if (code == RCVR_RECOVERED) {
		    long code2;
		    /* No cleanup handle so free this here.  This case is
		     * handled by a loop in our caller:
		     * epig_VerifyExistingAggregate. */
		    osi_Free_r ((opaque)h, sizeof(*h));
		    code2 = epia_Close (avl);
		    afsl_MBZ(code2);
		    epix_SetGConClose (epix_gc_device, (opaque)device);
		    elbb_Shutdown (device);
		    /* By definition the log is now clean. */
		    ss->flags |= EPIZ_SS_LOGCLEAN;
		    return code;
		}
		if (code == LOG_LOGFULL) {
		    epiz_ReportError (ss, code, EPIZ_E_LOG_TOOSMALL,
				      hdr->nLogBlocks);
		    ss->flags |= EPIZ_SS_GROW_LOG;
		    code = 0;
		}
		if (code) {
		    epiz_ReportError (ss, code, EPIZ_E_LOG_RECOVERYFAILED);
		    return -1;
		}
	    }
	} /* log processing */

	/* We could write a more careful version of this that would check all
	 * the pages of the avl and try an work around the dead ones, but for
	 * now... */

	/* Note also the more checking is done by epit_VerifyVolumeTable called
	 * on the AVL below. */

	code = epit_InitVolumeTable (avl, hdr->reservedIndexes);
	if (code) {
	    epiz_ReportError (ss, code, EPIZ_E_BADAVL);
	    return -1;			/* map to NOTANAGGREGATE ? */
	}

	/* We could write a more careful version of this that would check each
	 * page of the bitmap, but probably if there's anything wrong we just
	 * want to rebuild it from scratch. */

	/* However, since we can't rebuid the bitmap from scratch and since the
	 * bitmap provides a valuable consistency check on the superblock
	 * (specifically on firstBlock, nBlocks, blksize and fragsize) we treat
	 * inconsistency errors here as fatal.  In the future we could return a
	 * different error code for errors with these fields and errors with
	 * other parts of the bitmap and try to rebuild in the latter case. */

	code = epia_Open (avl, hdr->bitmapIndex, 0, 0, &ss->bitmap);
	if (code)
	    epiz_ReportError (ss, code, EPIZ_E_BADBITMAP);
	if (!code) {
	    code = epib_InitAllocationPool (ss->bitmap, hdr->firstBlock,
					    hdr->nBlocks,
					    &h->allocationHandle);
	    if (code) {
		long ccode;
		epiz_ReportError (ss, code, EPIZ_E_BADBITMAP);
		ccode = epia_Close (ss->bitmap);
		afsl_MBZ(ccode);
		if (code == EPI_E_NOTABITMAP)
		    return -1;		/* fatal error, bail! */
		/* try to press on? */
	    }
	}
	if (code) {
	    /* guess at the bitmap's size using the super block info */
	    ss->bitmap = 0;
	    code = 0;
	} else {
	    afsl_Assert (ss->bitmap);
	}
    }
    if (ss->bitmap) {
	struct epib_info info;
	code = epib_GetInfo (h->allocationHandle, &info);
	if (code ||
	    !((ss->bitmap == info.bitmapC) && (info.base == hdr->firstBlock) &&
	      (info.length == hdr->nBlocks))) {

	    /* This can fail, for instance, if InitFreeSpace fails (e.g. an
	     * aggregate too short). */

	    epiz_ReportError (ss, code, EPIZ_E_BADBITMAP);
	    code = epib_CloseAllocationPool (h->allocationHandle);
	    afsl_MBZ(code);
	    h->allocationHandle = 0;
	    ss->bitmap = 0;
	} else {
	    epia_SetAllocationHandle (avl, h->allocationHandle);
	}
    }
    if (!ss->bitmap) {
	ss->flags |= EPIZ_SS_REPLACE_BITMAP;
    }

    ss->simBits->loggedLen =		/* need a bit per block (rounded up) */
	(realNBlocks + 7) >> 3;
    ss->simBits->logged = osi_Alloc_r (ss->simBits->loggedLen);
    bzero((caddr_t)ss->simBits->logged, ss->simBits->loggedLen);
    ss->simBits->fraged = osi_Alloc_r (ss->simBits->loggedLen);
    bzero((caddr_t)ss->simBits->fraged, ss->simBits->loggedLen);

    {   int type;
	for (type = 0; type < 6; type++)
	    ss->specialIx[type] = -1;
    }
    ss->expectedIx[EPIA_CFLAGS_AVL] = hdr->reservedIndexes;
    ss->expectedIx[EPIA_CFLAGS_BITMAP] = hdr->bitmapIndex;
    ss->expectedIx[EPIA_CFLAGS_LOG] = hdr->logIndex;
    ss->expectedIx[EPIA_CFLAGS_BADBLOCKS] = hdr->badblocksIndex;

    return 0;
}

/* UncheckAggregate -- reverses the effects of CheckAggregate, see which. */

static void UncheckAggregate(IN struct epiz_salvageState *ss)
{
    long code;
    epi_anode_t avl;
    struct aggregate *h;

    avl = ss->aggrAnode;
    if (!avl) return;			/* nothing to do */

    GetOKagg (h, avl);
    afsl_Assert (h);
    if (!(h->flags & AGGR_SALVAGE)) {
	afsl_Assert(h->refCount > 1);
	h->refCount--;
    } else {
	/* get all the anodes in this device to be GC'ed quickly */
	code = epix_SetGConClose (epix_gc_device, (opaque)(ss->device));
	afsl_MBZ(code);

	if (h->allocationHandle) {
	    code = epib_CloseAllocationPool (h->allocationHandle);
	    afsl_MBZ(code);
	    h->allocationHandle = 0;
	} else {
	    /* no bitmap container */
	}

	{   long count;
	    code = epix_CountOpenAnodes (ss->device, &count);
	    afsl_MBZ(code);
	    afsl_Assert(count == 1);	/* just the avl */
	}

	epia_SetFileRock (avl, 0, 0);
	code = epia_Close (avl);
	afsl_MBZ(code);
	code = elbb_Shutdown (ss->device);
	afsl_MBZ(code);		/* deep trouble if this fails */
    }
    if (ss->simBits->bits)
	osi_Free_r (ss->simBits->bits, ss->simBits->bitsLen);
    if (ss->simBits->logged)
	osi_Free_r (ss->simBits->logged, ss->simBits->loggedLen);
    if (ss->simBits->fraged)
	osi_Free_r (ss->simBits->fraged, ss->simBits->loggedLen);
    if (ss->simBits)
	osi_Free_r (ss->simBits, sizeof(*ss->simBits));
}

/* ReportCode -- report code and caller.  If code is -1 then lower level has
 *     already printed detailed message so just explain where we are. */

#define ReportCode(code, procname) \
    com_err ("epig_VerifyExistingAggregate", ((code == -1) ? 0 : code), \
	      "%s failed", procname)

/* BitmapSetup -- prepares for bitmap verification done during SCAN and DUPFIX
 *     passes. */

static long BitmapSetup(IN struct epiz_salvageState *ss)
{
    u_long bbIndex = ss->expectedIx[EPIA_CFLAGS_BADBLOCKS];

    ss->blockRef = epiz_MarkUsed;
    bzero((char *)ss->simBits->bits, ss->simBits->bitsLen);
    /* The state of the logged/fraged map is persistent so leave it alone. */

    /* Note when (re-)building bitmap we want to scan the contents of the BAD
     * BLOCK anode BEFORE we make any references to any other anodes. */

    if (bbIndex > 0 ) {
	long code;

	ss->expectedType = EPIA_CFLAGS_BADBLOCKS;
	code = epia_VerifyAnode (ss, ss->aggrAnode, bbIndex);
	if (!code) {
	    BEGIN_LCRIT (epi_anode_t bb,
			 code = epia_Open (ss->aggrAnode, bbIndex, 0,0, &bb),
			 code = epia_Close (bb)) {
		ss->volIx = epia_GetIndex(ss->aggrAnode);
		ss->anodeIx = bbIndex;
		code = ScanBadBlockFile (ss, bb);
		if (code == -1) {
		    ss->flags |= EPIZ_SS_FIX_BADBLOCK;
		    code = 0;
		}
	    } END_CRIT;
	}
	ss->volIx = ss->anodeIx = 0;
	if (code) {
	    ReportCode (code, "VerifyAnode (bad block file)");
	    return -1;
	}
    }
    return 0;
}

/* ScanpassChecks -- do stuff that must happen after the first (SCAN) pass
 *     finishes.  A non-zero code returned from here aborts salvage. */

static long ScanpassChecks(IN struct epiz_salvageState *ss)
{
    long code;
    int type;

    for (type = 1; type < 6; type++) {
	char *typeName =  epiz_specialAnodeName[type];
	if (type == EPIA_CFLAGS_VOLUME) continue;
	if (ss->specialIx[type] == -1) {
	    epiz_ReportError (ss, 0, EPIZ_E_AVL_MISSINGSPECIAL, typeName);
	    ss->flags |= EPIZ_SS_FIX_SUPERBLOCK;
	} else if (ss->specialIx[type] != ss->expectedIx[type])
	    ss->flags |= EPIZ_SS_FIX_SUPERBLOCK;
    }

    /*
     * Perform Intermediate Verification...
     *
     * - Verify (and repair) the bitmap
     * - Find out if there were any bad anode copies counts..
     */
    if (ss->bitmap) {
	struct epiz_anode *a;

	a = epiz_FindAnode (epia_GetIndex(ss->bitmap),
			    epia_GetIndex(ss->aggrAnode));
	if (a && (a->refCnt>0)) {
	    epiz_ReportError (ss, 0, EPIZ_E_BITMAP_DAMAGED);
	    ss->flags |= EPIZ_SS_REPLACE_BITMAP;
	    ss->bitmap = 0;
	}

	code = epib_VerifyBitmap (ss->aggrAnode, ss, ss->bitmap,
				  epib_GetAllocationHandle(ss->aggrAnode));
	if (code) {
	    ReportCode (code, "VerifyBitmap");
	    ss->bitmap = 0;
	    return -1;
	}
	if (ss->flags & EPIZ_SS_REPLACE_BITMAP)
	    ss->bitmap = 0;
    }
    code = epiz_VerifyCopiesVerify( ss, 0 /* don't care how many fixed */ );
    if (code) {
	ReportCode (code, "VerifyCopiesVerify");
	return -1;
    }
    return 0;
}

/* DupfixpassSetup -- allocates the blocks from the reserved list. */

static DupfixpassSetup(IN struct epiz_salvageState *ss)
{
    long code;

    code = epiz_EnumerateAnodeItems (ss->reserveFrags, ss->aggrAnode,
				     epiz_AllocResFrags, (opaque)ss);
    afsl_MBZ(code);
    code = epiz_EnumerateAnodeItems (ss->reserveBlocks, ss->aggrAnode,
				     epiz_AllocResBlocks, (opaque)ss);
    afsl_MBZ(code);
    return 0;
}

/* DupfixpassChecks -- returns unused storage if any. */

static long DupfixpassChecks(IN struct epiz_salvageState *ss)
{
    long code;

    code = epiz_EnumerateAnodeItems (ss->reserveFrags, ss->aggrAnode,
				     epiz_FreeResFrags, (opaque)ss);
    afsl_MBZ(code);
    code = epiz_EnumerateAnodeItems (ss->reserveBlocks, ss->aggrAnode,
				     epiz_FreeResBlocks, (opaque)ss);
    afsl_MBZ(code);
    return 0;
}

static CowfixpassSetup(IN struct epiz_salvageState *ss)
{
    ss->blockRef = epiz_MarkNoop; /* no bitmap checking */
    return 0;
}

static CowfixpassChecks(IN struct epiz_salvageState *ss)
{
    return 0;
}

static long WriteHeader (
  struct epiz_salvageState *ss,
  struct diskAggregateHeader *hdr,
  u_int off,
  u_int len)
{
    long code;

    BEGIN_LCRIT (struct elbb_buffer *b,
		 code = elbb_Read (ss->device, ss->aggrAnode->block,
				   elbb_bufLogged, &b),
		 code = elbb_Release (b, elbb_lazy)) {
	code = elbb_Modify (b, epic_IndexToOffset(0) + off, (caddr_t)hdr + off,
			    len, elbb_zeroTrans, 0, &b);
    } END_CRIT;
    return code;
}

/* EXPORT */
int afsdb_episode_anode_verify = 0;

/* epig_VerifyExistingAggregate -- This procedure verifies the anode layer
 *     consistency of the aggregate. It can also be used to perform repairs.
 *     Normally it is called as part of the salvage command but can also be
 *     used by itself from test programs.  It does not run inside the kernel,
 *     and does no locking, therefore, it must be run single threaded.  Besides
 *     the return code, \argname{ss} communicates detailed information about
 *     the success of this procedure in verifying and repairing the aggregate.
 *
 *     The return code is interpreted in a non-standard fashion.  A value of
 *     zero (0) means the routine succeeded in verifying the aggregate.  See
 *     \argname{ss} for details of problems found and repairs made.  Any
 *     non-zero value means that something serious is wrong with the aggregate.
 *     If the value is -1, a descriptive message has already been printed.
 *     Otherwise the code gives some indication of the problem.
 *
 * PARAMETERS --
 *     device -- device on which the aggregate resides.
 *     ss -- represents the known state of the aggregate. */

/* EXPORT */
long epig_VerifyExistingAggregate(
  struct async_device *device,
  struct epiz_salvageState *ss)
{
    long code;

    int old_async_debug;
    struct diskAggregateHeader hdr;
    int verbose;

    verbose = (ss->flags & EPIZ_SS_VERBOSE);
    old_async_debug = afsdb_episode_async;
    if (verbose) afsdb_episode_anode_verify = -1;
    else if (afsdb_episode_anode_verify) {
	verbose = 1;
	ss->flags |= EPIZ_SS_VERBOSE;
    }

    DBVprint (("Starting work on device at %#lx.", device));
    DBVprint (("This work will%s salvage the aggregate as well.",
	       ((ss->flags & EPIZ_SS_SALVAGE) ? "" : " not")));

    ss->device = device;
    BEGIN_CRIT (do {   code = CheckAggregate (ss, &hdr);
		   } while (code == RCVR_RECOVERED),
		UncheckAggregate (ss)) {
	DBVprint (("AVL anode at %#lx", ss->aggrAnode));
	DBVprint (("Aggregate created with %s",
		   epig_ProgramName(hdr.creatingProgram)));
	DBVprint (("Disk format version is %d/%d",
		   DiskFormatMajorVersion(&hdr), hdr.diskFormatMinorVersion));

	ss->pass = ss->pass = EPIZ_SS_SCANPASS;
	do {
	    int pass = ss->pass;

	    switch (pass) {		/* do pass setup */
	      case EPIZ_SS_SCANPASS:

		/* First Scan
		 * -- build in-memory bitmap
		 * -- find dup references
		 * -- check COW block references */

		code = BitmapSetup (ss);
		if (code) break;
		ss->dupList = 0;
		break;

	      case EPIZ_SS_DUPFIXPASS:

		/* Second Scan
		 * -- rebuild in-memory bitmap
		 * -- report and repair dup references
		 * -- continue repairing COW block references
		 *
		 * If there were any dup references, perform a second scan to
		 * identify all the involved volume and anodes.  Since we have
		 * already scanned once, we expect to find the first references
		 * to the frags in the DupBlock database.
		 *
		 * Second and subsequent references to any frag triggers
		 * salvaging of these references with blocks (or frags as
		 * appropriate) from the reserve lists.  These reserve lists
		 * are allocated during the EnumerateAnodeItems calls in
		 * DupfixpassSetup.  Since only the items on the reserve lists
		 * are allocated and since fragment groups are not
		 * interchanable with blocks or other fragment groups of
		 * different sizes, we need to make sure that the second
		 * traversal is in the same order as the first (so that which
		 * is the first and which are later references is preserved).

		 * We zero the Bitmap AGAIN since we only want to replace the
		 * later references on multiply referenced blocks. */

		epiz_ReportError (ss, 0, EPIZ_E_DUPFIXPASS);
		code = BitmapSetup (ss);
		if (code) break;
		code = DupfixpassSetup (ss);
		break;

	      case EPIZ_SS_COWFIXPASS:

		/* Third and subsequent Scans
		 * -- continue repairing COW block references */

		epiz_ReportError (ss, 0, EPIZ_E_COWFIXPASS);
		code = CowfixpassSetup (ss);
		break;

	      default:
		afsl_PAssert (ss->pass == 0, ("Illegal pass %d", ss->pass));
	    }

	    ss->flags &= ~EPIZ_SS_DOCOWFIX;
	    code = epit_VerifyVolumeTable
		(ss, ss->aggrAnode, epiv_VerifyVolume, (char *)ss);
	    if (code) {
		if (code == EPI_E_NOTAVOLUME)
		    epiz_ReportError (ss, 0, EPIZ_E_BADAVL);
		else epiz_ReportError (ss, code, EPIZ_E_AVL_SCANERROR);
		code = -1;
		break;
	    }

	    /* Do end of pass checking and advance to next pass if necessary */

	    ss->pass = 0;		/* exit loop unless we must continue */
	    switch (pass) {
	      case EPIZ_SS_SCANPASS:
		code = ScanpassChecks (ss);
		if (ss->flags & EPIZ_SS_SALVAGE) {
		    if (ss->dupList)
			ss->pass = EPIZ_SS_DUPFIXPASS;
		    else if (ss->flags & EPIZ_SS_DOCOWFIX)
			ss->pass = EPIZ_SS_COWFIXPASS;
		}
		break;
	      case EPIZ_SS_DUPFIXPASS:
		code = DupfixpassChecks (ss);
		if ((ss->flags & EPIZ_SS_SALVAGE) &&
		    (ss->flags & EPIZ_SS_DOCOWFIX))
		    ss->pass = EPIZ_SS_COWFIXPASS;
		break;
	      case EPIZ_SS_COWFIXPASS:
		code = CowfixpassChecks (ss);
		if ((ss->flags & EPIZ_SS_SALVAGE) &&
		    (ss->flags & EPIZ_SS_DOCOWFIX))
		    ss->pass = EPIZ_SS_COWFIXPASS;
		break;
	      default:
		afsl_PAssert (ss->pass == 0, ("Illegal pass %d", ss->pass));
	    }
	    if (code) break;
	    if (!ss->bitmap && (ss->flags & EPIZ_SS_SALVAGE)) break;
	} while (ss->pass != 0);
	if (code)
	    EXIT_CRIT;
	if (!ss->bitmap && (ss->flags & EPIZ_SS_SALVAGE)) continue;

	if (ss->dirScanPass) {

	    /* This is the directory traversal pass.  It calls
	     * ../salvage/paths.c: epis_VerifyAggrPaths. */

	    code = (*ss->dirScanPass) (ss);
	    afsl_MBZ(code);
	}

	ss->volIx = 0;			/* done with per-file checks */
	ss->auxIx = 0;

	/* Now see if any important containers were trashed, and bailout. */
	{
	    struct epiz_anode *a;
	    u_long avlIndex = epia_GetIndex(ss->aggrAnode);

#define BADANODE(i) \
	    (hdr.i && (a = epiz_FindAnode (hdr.i, avlIndex)) && (a->refCnt>0))
		/* to get indenter to be happy */;

	    if (BADANODE(reservedIndexes)) {
		printf ("AVL container modified\n");
		ss->flags |= EPIZ_SS_FIX_SUPERBLOCK;
	    }
	    if (BADANODE(bitmapIndex)) {
		printf ("Bitmap container modified\n");
		ss->flags |= EPIZ_SS_REPLACE_BITMAP;
	    }
	    if (BADANODE(logIndex)) {
		printf ("Log container modified\n");
		ss->flags |= EPIZ_SS_MISC_TROUBLE;
	    }
	    if (BADANODE(badblocksIndex)) {
		printf ("Badblocks container modified\n");
		ss->flags |= EPIZ_SS_MISC_TROUBLE;
	    }
#undef BADANODE
	}

	if ((hdr.diskFormatMinorVersion == EPIG_OLD_MINOR_VISIBLE_Q) &&
	    (ss->flags & EPIZ_SS_SALVAGE)) {

	    epiz_ReportError (ss, 0, EPIZ_E_OLDMINOR_VISIBLEQUOTA);
	    hdr.diskFormatMinorVersion = EPIG_DISK_MINOR_VERSION;
	    code = WriteHeader (ss, &hdr,
				offsetof(struct diskAggregateHeader,
					 diskFormatMinorVersion),
				sizeof(int32));
	    if (code)
		EXIT_CRIT;
	}

	if ((hdr.diskFormatMinorVersion == EPIG_OLD_MINOR_TIMESTAMPS) &&
	    (ss->flags & EPIZ_SS_SALVAGE)) {

	    /* First upgrade minor version number. */

	    epiz_ReportError (ss, 0, EPIZ_E_OLDMINOR_TIMESTAMPS);
	    hdr.diskFormatMinorVersion = EPIG_DISK_MINOR_VERSION;
	    code = WriteHeader (ss, &hdr,
				offsetof(struct diskAggregateHeader,
					 diskFormatMinorVersion),
				sizeof(int32));
	    if (code)
		EXIT_CRIT;
	}

	if (ss->flags & (EPIZ_SS_SALVAGE | EPIZ_SS_RECOVER)) {

	    /* If disk is writable and we've checked the whole disk, update the
             * time and code.  But first maybe upgrade minor version number. */

	    hdr.lastVerifyTime = osi_Time();
	    hdr.lastVerifyCode = epiz_GetExitCode(ss);
	    code = WriteHeader (ss, &hdr,
				offsetof(struct diskAggregateHeader,
					 lastVerifyTime),
				2*sizeof(int32));
	    if (code)
		EXIT_CRIT;
	}

	/* now write a restart block, since we're all done */
	if (ss->flags & EPIZ_SS_SALVAGE) {
	    code = InitLog (ss->aggrAnode, hdr.logIndex, hdr.nLogBlocks,
			    INITLOG_CREATE|INITLOG_CLOSE);
	    if (code == LOG_LOGFULL) {
		epiz_ReportError (ss, code, EPIZ_E_LOG_TOOSMALL,
				  hdr.nLogBlocks);
		ss->flags |= EPIZ_SS_GROW_LOG;
		code = 0;
	    }
	    if (code) {
		epiz_ReportError (ss, code, EPIZ_E_LOG_RECOVERYFAILED);
		code = -1;		/* already reported */
		EXIT_CRIT;
	    }
	    ss->flags |= EPIZ_SS_LOGCLEAN;
	}
    } END_CRIT;
    ss->aggrAnode = 0;

    afsdb_episode_async = old_async_debug; /* restore */

    if (code) return code;
    if (ss->flags & EPIZ_SS_REPLACE_BITMAP) {
	printf ("Bitmap reconstruction code not yet implemented\n");
    }
    if (ss->flags & EPIZ_SS_GROW_LOG) {
	printf ("Log growing code not yet implemented\n");
    }
    if (ss->flags & EPIZ_SS_FIX_BADBLOCK) {
	printf ("Bad Block file reconstruction code not yet implemented\n");
    }
    if (ss->flags & EPIZ_SS_FIX_SUPERBLOCK) {
	printf ("Superblock rewriting code not yet implemented\n");
    }
    return 0;
}

/* epig_FillInPartInfo -- Fills in the attributes from the aggregate header
 *     stored in the AVL's \argname{firstAnode}.  This can be obtained calling
 *     \fcname{epic_HowMuchLikeAnodeBlock} with a pointer to the superblock,
 *     which works basically by adding the size of the anode block header to
 *     the address of the superblock.
 *
 * PARAMETERS --
 *     firstAnode -- pointer to a buffer containing the aggregate header.
 *     pdp -- pointer to description of the attributes of the superblock.  See
 *     boot.h for details on \typename{struct PartData}. */

/* EXPORT */
long epig_FillInPartInfo(opaque firstAnode, struct PartData *pdp)
{
    struct diskAggregateHeader *agg;

    agg = (struct diskAggregateHeader *) firstAnode;
    if (!AGGREGATE_HDR_OK(agg, 0))
	return EPI_E_NOTANAGGREGATE;

    pdp->BlockSize = agg->blockSize;
    pdp->FragmentSize = agg->fragmentSize;
    pdp->FileSysCreateTime = agg->timeCreated;
    pdp->TotalBlocks = agg->nBlocks;
    pdp->FirstBlock = agg->firstBlock;
    pdp->NLogBlocks = agg->nLogBlocks;
    if (agg->diskFormatMinorVersion > EPIG_OLD_MINOR_TIMESTAMPS) {
	pdp->lastVerifyTime = agg->lastVerifyTime;
	pdp->lastVerifyCode = agg->lastVerifyCode;
    } else {
	pdp->lastVerifyTime = 0;	/* unknown */
	pdp->lastVerifyCode = 0;	/* assume the best */
    }
    return 0;
}

#ifdef SCAN_SALVAGER

/* called only from verify.c */
/* SHARED */
long epig_GetFragSizeFromDisk(
  struct async_device *dev,
  opaque dataptr,
  u_long *fragSizeP)
{/* Get the listed fragment size from the given pointer. */
    int val; int32 magic;
    long code;
    opaque first;
    struct diskAggregateHeader *hdr;

    code = epic_HowMuchLikeAnodeBlock(dev, dataptr, &val, &magic, &first);
    if (code != 0) return code;
    if (magic != ANODE_BLOCK_AVL_MAGIC) return -1;
    dahp = (struct diskAggregateHeader *) first;
    if (dahp->magic != AVL_MAGIC || dahp->blockSize < dahp->fragmentSize || (dahp->blockSize % dahp->fragmentSize != 0)) {
	return -1;
    }
    *fragSizeP = dahp->fragmentSize;
    return 0;
}

/* Procedure for guessing whether a given anode block looks like part of the AVL. */
/* EXPORT */
long epig_BlockScore(
  struct epiz_ReconState *recon,
  u_long ix,
  opaque data,
  long *typeP,
  long *scoreP,
  int Recurse)
{/* evaluate whether the given block looks like a block of the AVL. */
    long score, type, code, SBscore, possScore;
    long subScore, subType;
    long offs;				/* offset in anodeBlock */
    struct diskAggregateHeader *AggHdr;
    int IsSuperBlock, ExpectedType;
    int NumAnodes, TotScore, AVLFirstPage, SawLog, SawBitmap, CurrIx;
    int32 anodeType, anodeIndex;

    SBscore = 0;
/* This could legally be either the first page of the AVL (the superblock) or some other page. */
/* Does it look like an aggr header? */
    AggHdr = (struct diskAggregateHeader *) (((char *)data) + epic_FirstOffset());
    if (AggHdr->magic == AVL_MAGIC) ++SBscore;
    else --SBscore;
    if (AggHdr->blockSize == recon->BlockSize) ++SBscore;
    else --SBscore;
    if (AggHdr->fragmentSize == recon->FragmentSize) ++SBscore;
    else --SBscore;
    if (AggHdr->bitmapIndex < 4 && AggHdr->logIndex < 4) ++SBscore;
    else --SBscore;
    if (AggHdr->reservedIndexes < 15) ++SBscore;
    else --SBscore;
    if (SBscore >= 4) {
	IsSuperBlock = 1;
    } else {
	IsSuperBlock = 0;
    }

/* Look through all the purported anodes in the block. */
    NumAnodes = TotScore = SawLog = SawBitmap = AVLFirstPage = 0;
    score = possScore = 0;
    for (offs = epic_IndexToOffset((IsSuperBlock ? 1 : 0));
	  epic_LegalOffset(offs, bsize);
	  offs = epic_NextOffset(offs)) {
	code = epix_AnodeScore(recon, ix, ((char *)data + offs), &subScore, 0, offs, &anodeType, &anodeIndex);
	if (code != 0) continue;
	if (anodeType == 0) continue;
	ExpectedType = TYPEANODEBLOCK;	/* wants to be a volume */
	CurrIx = epic_OffsetToIndex(offs);
	if (anodeIndex == CurrIx) {	/* first page of AVL if these agree */
	    AVLFirstPage = 1;
	    if (CurrIx == AggHdr->bitmapIndex) {
		if (anodeType == EPIA_CFLAGS_BITMAP) {
		    ExpectedType = TYPEBITMAPPAGE;
		    SawBitmap = 1;
		    ++score;
		} else --score;
		++possScore;
	    }
	    if (CurrIx == AggHdr->logIndex) {
		if (anodeType == EPIA_CFLAGS_LOG) {
		    ExpectedType = TYPELOGPAGE;
		    SawLog = 1;
		    ++score;
		} else --score;
		++possScore;
	    }
/*	    if (CurrIx == AVL?); */
	}
/* Test if this anodeIndex value is reasonable. */
	if ((anodeIndex % epic_AnodesPerBlock(recon->BlockSize)) == CurrIx)
	    ++score;
	else --score;
	++possScore;
	if (Recurse) {
	    code = epix_AnodeScore(recon, ix, ((char *)data + offs), &subScore, Recurse, offs, &anodeType, &anodeIndex, ExpectedType);
	}
	++NumAnodes;
	TotScore += subScore;
    }
    if (IsSuperBlock == AVLFirstPage) score += 2;
    else score -= 2;
    possScore += 2;
    if (IsSuperBlock && AVLFirstPage) {
	type = TYPEANODEBLOCK | SECTIONHEAD | LISTTYPEAVL;
	score += SBscore;
	if (SawLog && SawBitmap) ++score;
	else --score;
	possScore += 6;
    } else if (IsSuperBlock == 0 && AVLFirstPage == 0) {
	type = TYPEANODEBLOCK | SECTIONMIDDLE | LISTTYPEAVL;
	score -= SBscore;
	possScore += 5;
    }
    score = EvidenceConst(score, possScore);
    if (NumAnodes > 0) score = (3 * score + TotScore) / (3 + NumAnodes);
    if (score > EvidenceConst(1, 4) && Recurse) {
	recon->State[ix].NativeGuess = type;
	recon->State[ix].NativeEvidence = score + EVIDENCE_OFFSET;
    }
    *scoreP = score;
    *typeP = type;
    return 0;
}
#endif /* SCAN_SALVAGER */
#endif /* not KERNEL */
