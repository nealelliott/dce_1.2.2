/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: fixed_anode.c,v $
 * Revision 1.1.19.1  1996/10/02  17:17:30  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:27:18  damon]
 *
 * $EndLog$
 */
/* Copyright (C) 1996, 1990 Transarc Corporation - All rights reserved */

/* fixed_anode.c -- contains procedures in the fixed_anode module which
 *     provides low level access to anodes.  It will not provide for allocation
 *     of new pages on write; hence the name fixed_anode.
 *
 *     The basic interface maps an anode's logical block number into a buffer
 *     containing the corresponding physical disk block.
 *
 *     The anode handle exported by this module is also used by the
 *     dynamic_anode module and to that end several fields are provided to it
 *     to use.  These fields are untouched by this module. */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/debug.h>
#include <dcedfs/hash.h>
#include <dcedfs/episode/logbuf.h>
#include "anode.h"
#include "anode_block.h"
#include "fixed_anode.h"

RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/episode/anode/fixed_anode.c,v 1.1.19.1 1996/10/02 17:17:30 damon Exp $")

struct icl_set *epi_iclSet;

#define DEBUG_THIS_FILE EPIA_DEBUG_FIXED_ANODE
#ifdef AFS_DEBUG
char *epia_debugFilename[EPIA_DEBUG_NFILES] = {
    "anode_block.c",
    "fixed_anode.c",
    "block_alloc.c",
    "anode.c",
    "volume_table.c",
    "volume.c",
    "file.c",
    "aggregate.c",
    "strategy.c",
};
#endif /* AFS_DEBUG */

/* struct mv_returnData -- data output from MoveData operation.  This is blocks
 *     to be deallocated, fragments to deallocate, and quota adjustments.  This
 *     is passed throughout the MoveData code and eventually epia_Clone will
 *     actually take care of these things. */

struct mv_returnData {
    u_int fragsPerBlock;		/* for convenience */
    u_int nBlocks;			/* length of output block array */
    daddr_t *pBlock;			/* output block array */
    int nFound;				/* number of blocks in array */
    epia_quota_t deltaC;		/* cummulative quota changes to COW */
    epia_quota_t deltaB;		/*  ditto backing */
    /* also count calls to elbb_Modify and quit if too many of those */
};

/* forward declarations */
static long AfterRecursiveOps(
  elbb_tranRec_t trans, epi_anode_t h, epi_anode_t b,
  daddr_t *cbP, daddr_t *bbP, struct elbb_buffer **cbbP,
  struct elbb_buffer **bbbP, long op, int topLevel,
  int iBIndex, struct mv_returnData *retData);
static long ProcessITOps(
  elbb_tranRec_t trans, epi_anode_t h, epi_anode_t b,
  daddr_t *cbP, daddr_t *bbP, struct elbb_buffer **cbbP,
  struct elbb_buffer **bbbP, long base, long span, int reclone,
  int iTreeRoot, int iBIndex, struct mv_returnData *retDataP);
static long RecursiveOps(
  elbb_tranRec_t trans, epi_anode_t h, epi_anode_t b,
  daddr_t cb, daddr_t bb, long op, baddr_t base, u_long span,
  int reclone, struct mv_returnData *retData);

/* The following assumptions are made about type sizes, to make sure that
 * everything gets layed out correctly.  These assumptions are checked in the
 * Init procedure. */

#define SizesOkay() ((sizeof(int32) == sizeof(u_int32)) && \
		     (sizeof(int32) == sizeof(daddr_t)) && \
		     (sizeof(int32) == 2*sizeof(int16)) && \
		     (sizeof(int32) == sizeof(baddr_t)) && \
		     (sizeof(int32) <= sizeof(long)))

/* These are the anode level on-disk structures. */

/* anode flag bits */
#define AFLAGS_STATUSSIZE      EPIA_AFLAGS_STATUSSIZE

#define AFLAGS_TYPE		(EPIA_CFLAGS_TYPE << EPIA_AFLAGS_TYPE_SHIFT)
#define CTOAT(t) /* convert CFLAGS to AFLAGS type */ \
    (((t) & EPIA_CFLAGS_TYPE)<<EPIA_AFLAGS_TYPE_SHIFT)
#define AFLAGS_TYPE_AVL		CTOAT(EPIA_CFLAGS_AVL)
#define AFLAGS_TYPE_VOLUME     CTOAT(EPIA_CFLAGS_VOLUME)
#define AFLAGS_TYPE_BITMAP     CTOAT(EPIA_CFLAGS_BITMAP)
#define AFLAGS_TYPE_LOG		CTOAT(EPIA_CFLAGS_LOG)
#define AFLAGS_TYPE_BADBLOCKS  CTOAT(EPIA_CFLAGS_BADBLOCKS)
#define AFLAGS_TYPE_REGULAR    CTOAT(EPIA_CFLAGS_REGULAR)
#define AFLAGS_TYPE_DELETED    CTOAT(0)	/* deleted or invalid */

#define CTOAB(t) /* convert CFLAGS to AFLAGS bits */ \
    (((t)&EPIA_AFLAGS_BITS_MASK)<<EPIA_AFLAGS_BITS_SHIFT)
#define AFLAGS_LOGGED		CTOAB(EPIA_CFLAGS_LOGGED)
#define AFLAGS_COPY_ON_WRITE   CTOAB(EPIA_CFLAGS_COPYONWRITE)
#define AFLAGS_PREALLOCATED    CTOAB(EPIA_CFLAGS_PREALLOCATED)
#define AFLAGS_DATA_INLINE     CTOAB(EPIA_CFLAGS_INLINE)
#define AFLAGS_DATA_FRAGMENTED CTOAB(EPIA_CFLAGS_FRAGMENTED)
#define AFLAGS_DATA_NOTBLOCKED (AFLAGS_DATA_INLINE | AFLAGS_DATA_FRAGMENTED)
#define AFLAGS_UNSAFE		CTOAB(EPIA_CFLAGS_UNSAFE)
/*** xxxx (rns) ***/
#define AFLAGS_SPARE2		(1<<22)
#define AFLAGS_SPARE3		(1<<23)
#define AFLAGS_MAGIC_MASK      (0xff00f000) /* unused bits form magic number */
#define AFLAGS_MAGIC		(ANODE_MAGIC_BASE & AFLAGS_MAGIC_MASK)

#define DeletedAnode(f) ((f) == ANODE_MAGIC)
#define ExistingAnode(h) ((Disk(h)->flags & AFLAGS_MAGIC_MASK) == AFLAGS_MAGIC)
#define AnodeType(h) (Disk(h)->flags & AFLAGS_TYPE)

/* The disk format of the anode structure which describes a data container.
 * The total size is about 252 bytes which allows for a few header words per
 * anode-containing disk block. */

/* NOTE -- 2nd word of "spare[2]" reused as "visible". */

struct diskAnode { /* 24 words + status */
    /* the flags field must be first and is set to zero if the anode is free */
    int32 flags;			/* anode flags */
    struct epix_diskVolData volData;    /* not used <see epif_status> */
    u_int32 index;			/* index in (vol) anode container */
    u_int32 allocated;			/* fragments allocated to anode */
    dfsh_diskHyper_t diskLength;	/* size of container in bytes */
    union { /* a space saver */
	struct backPtr {		/* backing anode for copy-on-write */
	    daddr_t block;		/* disk block containing anode */
	    u_int32 offset;		/* byte offset in block */
	} backingAnode;
	dfsh_diskHyper_t diskVolId;	/* obs: provide fast access to volId */
    } c; /* depending on AFLAGS_COPY_ON_WRITE */
    u_int16 copies;			/* number of anodes this one backs */
    int16 spareShort;
    int32 spare[1];
    u_int32 visible;			/* fragments visible in anode */
    union { /* data */
	char inlineData[1];		/* size depends on status size */
	epix_fragDescr_t frags;		/* if fragmented data */
	struct { /* for containers bigger then one block */
	    daddr_t direct[ANODE_DIRECTBLOCKS]; /* direct blocks */
	    daddr_t			/* trees of increasing depth */
		indirectBlockTree[ANODE_INDIRECT_TREES];
	} blocks;
    } d;
    /* statusData - is allocated at the end of the anode, as defined by
     * ANODE_SIZE.  In-line data is allocated starting at "d.inlineData" and
     * grows towards the end. */
};

typedef struct diskAnode diskAnode_t;

/* Abbreviations.  */
#define bBlock  c.backingAnode.block
#define bOffset c.backingAnode.offset

#define fBlock  d.frags.block
#define fFirst  d.frags.first
#define fLength d.frags.length

#define dBlock  d.blocks.direct
#define iBlock  d.blocks.indirectBlockTree

#define ANODE_MAX_STATUSSIZE (ANODE_SIZE - sizeof (struct diskAnode))
#define AnodeMaxInlineData(h) \
    (ANODE_SIZE \
     - (sizeof (struct diskAnode) - sizeof (Disk(h)->d)) \
     - (Disk(h)->flags & AFLAGS_STATUSSIZE))
#define AnodeInlineDataAddress(h) (Disk(h)->d.inlineData)

/* AreBlockAddrsEmpty -- is a boolean macro that evaluates to true if, assuming
 *     the anode is BLOCKED, all the block addresses are empty.  Its argument
 *     is the disk ptr of the anode (Disk(h)). */

#define AreBlockAddrsEmpty(disk) \
    (((disk)->d.blocks.direct[0] == EPIX_ANODE_BLOCKEMPTY) && \
     ((disk)->d.blocks.direct[1] == EPIX_ANODE_BLOCKEMPTY) && \
     ((disk)->d.blocks.direct[2] == EPIX_ANODE_BLOCKEMPTY) && \
     ((disk)->d.blocks.direct[3] == EPIX_ANODE_BLOCKEMPTY) && \
     ((disk)->d.blocks.direct[4] == EPIX_ANODE_BLOCKEMPTY) && \
     ((disk)->d.blocks.direct[5] == EPIX_ANODE_BLOCKEMPTY) && \
     ((disk)->d.blocks.direct[6] == EPIX_ANODE_BLOCKEMPTY) && \
     ((disk)->d.blocks.direct[7] == EPIX_ANODE_BLOCKEMPTY) && \
     ((disk)->d.blocks.indirectBlockTree[0] == EPIX_ANODE_BLOCKEMPTY) && \
     ((disk)->d.blocks.indirectBlockTree[1] == EPIX_ANODE_BLOCKEMPTY) && \
     ((disk)->d.blocks.indirectBlockTree[2] == EPIX_ANODE_BLOCKEMPTY) && \
     ((disk)->d.blocks.indirectBlockTree[3] == EPIX_ANODE_BLOCKEMPTY))

#define ANODE_FO(field) offsetof(struct diskAnode, field)

/* These are some macros that call elbb_Modify properly to update anodes */

#define SET_ANODE_FIELD(trans, h, buf, field, newData) \
    ((Disk(h)->field = (newData)), \
     elbb_Modify (buf, (h)->offset+ANODE_FO(field), \
		  (caddr_t)&(Disk(h)->field), sizeof(Disk(h)->field), \
		  trans, !elbb_IsZeroTrans(trans), &(buf)))

#define SET_ANODE_ADDRESS(trans, h, buf, addr, newData) \
    ((*(addr) = (newData)), \
     elbb_Modify (buf, (h)->offset + (char *)(addr) - (char *)Disk(h), \
		  (caddr_t)addr, sizeof(newData), trans, \
		  !elbb_IsZeroTrans(trans), &(buf)))

#define SET_ANODE_ADDRESSES(trans, h, buf, addr, newDataAddr, count) \
    (bcopy ((char *)(newDataAddr), (char *)(addr), (count)*sizeof(daddr_t)), \
     elbb_Modify (buf, (h)->offset + (char *)(addr) - (char *)Disk(h), \
		  (caddr_t)addr, (count)*sizeof(daddr_t), trans, \
		  !elbb_IsZeroTrans(trans), &(buf)))

#define DISK_H_OK(abb, h) \
    (bcmp((char *)Disk(h), (char *)((abb)->data) + (h)->offset, ANODE_SIZE) \
     == 0)
/* make some kinda expensive checks on the buffer containing an anode */
#define CheckAnodeBlock(b, h) \
    (afsl_AssertCond (epic_AnodeBlockOkay( \
		      b, ((epia_GetType(h) == EPIA_CFLAGS_REGULAR) ? \
			   ANODE_BLOCK_VOLUME_MAGIC : ANODE_BLOCK_AVL_MAGIC), \
			h->volIndex)), \
     afsl_AssertCond (DISK_H_OK(b, h)))

/* Handle flags - mostly unused */

#define HFLAGS_DIRTY_DATA 0x1		/* two dirty bits */
#define HFLAGS_DIRTY_STATUS 0x2
#define HFLAGS_AVL 0x4			/* container is the AVL */
#define HFLAGS_VOLUME 0x8		/* container is a volume table */
/* HFLAGS_DEVICEUNLOGGED declared in anode.p.h */

/* Macros for managing opaque pointers in anode handles */

#define Disk(h) ((struct diskAnode *)((h)->disk))

/* Disk format of indirect blocks. */

#define INDIRECT_MAGIC 0x5A308D31
#define ANODE_INDIRECTBLOCK_OVERHEAD \
    (sizeof(struct epia_indirectBlock) - \
     sizeof(((struct epia_indirectBlock *)0)->blocks))
#define IBSIZE(h) (((h)->device->blkSize - ANODE_INDIRECTBLOCK_OVERHEAD) / \
		   sizeof(daddr_t))
#define IBOFFSET(l) (offsetof(struct epia_indirectBlock, blocks[l]))

struct epia_indirectBlock {
    int32 initialStamp;			/* set for every page write */
    int32 magic;			/* INDIRECT_MAGIC */
    /* Who points to this block?  If this is a top level indirect block, then
     * this is the block/offset pair that describe the anode.  If this is a
     * lower level block then this is the block of the parent indirect block
     * and the offset is the index in its block array that points to this block
     * (not the byte offset). */
    struct backPtr whoPoints;
    baddr_t baseOffset;			/* first container block address */
    u_int32 span;			/* addr sep. of consecutive blocks */
    u_int32 ibsize;			/* number of blocks */
    daddr_t blocks[/*ibsize*/1];	/* block addresses */
    int32 finalStamp;			/* same as initialStamp */
};

/* IndirectBlockOkay -- verified the consistency of an indirect block.  It only
 *     check span if specified */

#define IndirectBlockOkay(ib, h, parent, index, base, spn) \
    (((ib)->magic == INDIRECT_MAGIC) && \
     ((ib)->initialStamp == \
      *((int32 *)((char *)(ib) + (h)->device->blkSize - sizeof(int32)))) && \
     (((long)(parent) == -2) || /* don't check */ \
      (((long)(parent) == -1) \
	? (((ib)->whoPoints.block == (h)->block) && \
	  ((ib)->whoPoints.offset == (h)->offset)) \
	: (((ib)->whoPoints.block == (parent)) && \
	  ((ib)->whoPoints.offset == (index))))) && \
     (!(spn) || ((ib)->span == (spn))) && \
     ((ib)->ibsize == IBSIZE(h)) && \
     (!(base) || ((ib)->baseOffset == (base))))

/* EXPORT  */
int afsdb_episode_anode = 0;
#define ALWAYSprint(p) dprintf (1, p)

#ifdef AFS_DEBUG
/* SHARED */
void epix_BumpCodeBlock /* hack */ (fileNumber, blockNumber)
  IN int fileNumber;
  IN int blockNumber;
{
    afsl_Assert(fileNumber < EPIA_DEBUG_NFILES);
    afsl_Assert(blockNumber < EPIA_MAX_FILE_CODE_BLOCKS);
    epia_codeBlock[fileNumber][blockNumber]++;
}
#endif

#if defined(AFS_DEBUG) && !defined(KERNEL)
/* SHARED */
int epix_tr_downCounter;	/* must be zero before beg. output */
static int upCounter;
/* SHARED */
struct anodeList {
    long mindev;			/* minor device number */
    u_long volIndex;			/* index in AVL of VT */
    u_long anodeIndex;			/* index in VT of anode */
} *epix_tr_anodeList;			/* array of anodes to trace */

static int TraceAnode(IN epi_anode_t h)
{
    u_long vi = epia_GetVolIndex (h);
    u_long ai = epia_GetIndex (h);
    struct anodeList *al = epix_tr_anodeList;

    if (!h->device || !ExistingAnode(h)) return 0;
    if (!al) return 0;
    while (!((h->device->mindev == al->mindev) &&
	     (ai == al->anodeIndex) && (vi == al->volIndex))) {
	if (al->volIndex == 0)
	    return 0;			/* list exhausted */
	al++;
    }
    upCounter++;
    return (upCounter > epix_tr_downCounter);
}

#define IS_TRACE_ANODE(h, code) \
    if (epix_tr_anodeList && TraceAnode(h)) { \
	printf ("%04d %3d,%03d: ", upCounter, \
		epia_GetVolIndex (h), epia_GetIndex (h)); \
	code; \
	printf ("\n"); \
    }

#else
#define IS_TRACE_ANODE(h, code)
#endif

/* L O C K I N G   H I E R A R C H Y --
 *
 * A single thread may only wait for locks lower on this list than any of the
 * locks it already holds.  Multiple locks on the same level may not be held
 * simultaneously.  An exception to this is behavior is that when a handle is
 * first created, it is created locked.  This cannot lead to deadlock since,
 * by definition, no one else can access the handle because the handleLock is
 * held during this time.
 *   (epi_anode_t)AVL->lock
 *   (epi_anode_t)VolumeTable->lock
 *   (epi_anode_t)other->lock
 *   (epi_anode_t)backing->lock  --  if other is COW
 *   handleLock
 */

/* This lock protects the anode handle pool.  This includes the hash table and
 * the free and inuse fifos.  It also covers incrementing and decrementing the
 * handle refcounts so that every handle is consistent with respect to its
 * refcount.  It also protects the GConClose field. */

static epi_mutex_t handleLock;

/* Hash table for anode handles */

static struct dfsh_hashTable handleHT;

/* HandleHashEntry -- returns a hash value for an anode handle.  It is used by
 *     hash table module. */

static unsigned long HandleHashEntry(epi_anode_t h)
{   /* adapted from budb StringHashFunction */
    unsigned long hash = 1000003;	/* big prime to start things nicely */
					/* XXX appears quite useless */
#define HASH_MULTIPLIER 0x10204081
    hash = (unsigned long)h->device + hash*HASH_MULTIPLIER;
    hash = (unsigned long)h->block + hash*HASH_MULTIPLIER;
    hash = (unsigned long)h->offset + hash*HASH_MULTIPLIER;
    return hash;
}

/* All anode handles are stored on one of these three fifos.  If the device
 * field is zero then it will be on the zero list.  These are newly allocated
 * or invalidated handles and contain no cached information.  They should be
 * reused first.  If the refCount is zero then the handle will be on the free
 * list.  These are in the hash table and contain cached information but can be
 * reused if necessary.  Otherwise, it will be on the in-use list. */

static unsigned long nHandles;
static struct fifo handleZeroList;
static struct fifo handleFreeList;
static struct fifo handleInuseList;
static u_long nHandlesTlockWait;	/* # handles alloc'd to save tlocks */


/* epix_Init -- Initializes the fixed anode module.  The \arg{maxHandles}
 *     parameter specifies the intial number of anode handle structures to
 *     allocate. Additional handles will be allocated upon demand.  In addition
 *     to initializing various data structures this routine also checks various
 *     assertions that the Episode code depends upon (e.g.  that an int32
 *     occupies 4 bytes).  It is called only once during system initialization
 *     by \fcn{epig_Init}, see which. */

/* SHARED */
long epix_Init(u_long maxHandles)
{
    static inited = 0;
    epi_anode_t h = 0;
    long code;
    int i;
    struct epia_anode *handleArray;
    char *handleDiskCopyArray;

    if (inited) return EPI_E_REINIT;

    afsl_Assert(SizesOkay() &&
	    (ANODE_FO(allocated) == EPIA_DISKANODE_ALLOCATED) &&
	    (ANODE_FO(visible) == EPIA_DISKANODE_VISIBLE) &&
	    (ANODE_FO(index) == EPIA_DISKANODE_INDEX) &&
	    (ANODE_FO(volData) == EPIA_DISKANODE_VOLDATA) &&
	    (ANODE_FO(flags) == EPIA_DISKANODE_FLAGS) &&
	    (ANODE_FO(copies) == EPIA_DISKANODE_COPIES));

    epi_mutex_init (&handleLock, "anode handle");

    FIFO_INIT(&handleZeroList, offsetof(struct epia_anode, thread));
    FIFO_INIT(&handleFreeList, offsetof(struct epia_anode, thread));
    FIFO_INIT(&handleInuseList, offsetof(struct epia_anode, thread));

    /* allocate storage for anode handles, and cached block maps. */
    handleArray = osi_Alloc_r(maxHandles*sizeof(struct epia_anode));
    handleDiskCopyArray = osi_Alloc_r (maxHandles*ANODE_SIZE);

    for (i = 0; i < maxHandles; i++) {
	h = &handleArray[i];
	h->device = 0;			/* indicate it's free */
	h->disk = (opaque)(handleDiskCopyArray + i*ANODE_SIZE);
	FIFO_ADD (&handleZeroList, &h->thread);
    }
    nHandles = maxHandles;

    code = dfsh_HashInit (&handleHT, HandleHashEntry, /*thread offset*/0);
    if (code) return code;
    code = dfsh_HashInitKeyFixed
	(&handleHT, offsetof(struct epia_anode, device),
	 /*length*/ sizeof(h->device) + sizeof(h->block) + sizeof(h->offset));
    if (code) return code;

#ifdef AFS_DEBUG
    /* CodeBlock: epia */
    epia_codeBlock[DEBUG_THIS_FILE][0] = 94, epia_codeBlock[DEBUG_THIS_FILE][8] = -1, epia_codeBlock[DEBUG_THIS_FILE][9] = -1, epia_codeBlock[DEBUG_THIS_FILE][51] = -1, epia_codeBlock[DEBUG_THIS_FILE][53] = -1, epia_codeBlock[DEBUG_THIS_FILE][60] = -1;
#endif

    inited++;				/* don't do this again */
    return 0;
}

/* RunCleanup -- executes the cleanup function for a particular handle rock. */

static void RunCleanup(
  epi_anode_t h,
  struct epia_anodeRock *rock)
{
    if (rock->cleanup == 0) return;
    afsl_Assert(rock->cleanup != (void (*)())1);
    (*rock->cleanup) (h);		/* should zero cleanup ptr */
    afsl_Assert(rock->cleanup == 0 && rock->rock == 0);
}

/* CleanupHandle -- cleans up a handle marks it for reuse.  The caller must
 *     move the handle to the zero list. */

static void CleanupHandle(epi_anode_t h)
{
    long code;

    epix_EnterCodeBlock(1);
    if (h->device == 0) return;
    afsl_Assert(h->refCount == 0);	/* better not be any users / lockers */
    AssertTUnlocked(&h->lock);
    afsl_Assert(h->backingAnode == 0);

    RunCleanup (h, &h->allocation);
    RunCleanup (h, &h->volume);
    RunCleanup (h, &h->file);

    code = dfsh_HashOut_r (&handleHT, h);
    afsl_MBZ(code);				/* not in hash table? */

    /* Make sure we don't find this again.  We must force it to be read from
     * the disk next time (if there is one). */
    h->device = 0;
}

/* NewAnodeHandle -- locates an unused handle and returns it.  The handle
 *     returned is not on either handle fifo.
 *
 * LOCKS USED -- This is called with handleLock write-locked. */

static epi_anode_t NewAnodeHandle(void)
{
    epi_anode_t h;

    if (!IsFifoEmpty (&handleZeroList)) {
	/* use these first */
	h = (epi_anode_t)FIFO_LookAtOldest(&handleZeroList,
		offsetof(struct epia_anode, thread));
	FIFO_REMOVE (&handleZeroList, &h->thread);
	return h;
    }

    h = 0;
    for (h = (epi_anode_t)FIFO_LookAtOldest(&handleFreeList,
				offsetof(struct epia_anode, thread));
	 h;
	 h = (epi_anode_t)FIFO_NextOldest(&handleFreeList, h, &h->thread)) {

	/* Find a handle whose tlock trans is COMPLETE or COMMITTED so we can
	 * recycle it without waiting. */

	long state;
	elbb_tranRec_t trans;
	trans = tlock_GetTrans(&h->lock);
	state = elbb_TranStat (trans);
	if ((state != BUF_TRANCOMMIT) && (state != BUF_TRANCOMPLETE)) {
	    epix_EnterCodeBlock(0);
	    continue;
	}
	FIFO_REMOVE (&handleFreeList, &h->thread);
	CleanupHandle (h);
	return h;
    }

    h = osi_Alloc_r(sizeof (struct epia_anode) + ANODE_SIZE);
    h->device = 0;
    h->disk = (opaque) &h[1];

    nHandles++;
    if (!IsFifoEmpty (&handleFreeList)) {
	nHandlesTlockWait++;
	afsl_TraceEnabled (1, 1, AFSL_TR_ALWAYS,
 ("Allocating another handle, now %d, because %d have incomplete tlock trans",
  nHandles, handleFreeList.length));
    }

    return h;
}

/* CheckHandle -- locates an existing anode handle given a unique specification
 *     of it.  Returns true if one exists.  This is only called by CreateAVL.
 *
 * LOCKS USED -- This routine does all its work undercover of handleLock held
 *     for read. */

static int CheckHandle(
  IN struct async_device *device,
  IN daddr_t block,
  IN u_long  offset)
{
    epi_anode_t h;
    struct epia_anode handleKey;	/* to use for lookup */

    h = 0;
    afsl_Assert(device && epix_IsBlockAllocated(block) &&
	    epic_LegalOffset(offset, device->blkSize));
    handleKey.device = device;		/* setup key for hash lookup */
    handleKey.block = block;
    handleKey.offset = offset;

    BEGIN_MUTEX (&handleLock) {
	h = dfsh_HashLookup (&handleHT, &handleKey);
    } END_LOCK;
    return !!h;				/* booleanize */
}

/* NewHandle -- allocates a new anode handle.  Since the lock in the handle
 *     protects the disk data, it must be available and locked before the disk
 *     is referenced.  In this connection a newly created, and thus
 *     uninitialized, handle is returned write locked.  If the handle already
 *     exists it is returned with it's refCount bumped but not locked.  These
 *     two cases are distinguished the boolean output parameter new, whose
 *     value is set to one if the handle was newly created.  If a new handle is
 *     required but unavailable a zero handle pointer is returned (and new is
 *     set to zero).
 *
 *     This is only called from Open so that initialization code can be
 *     centralized there.
 *
 * LOCKS USED -- The handleLock is held for write while hash tables and fifos
 *     are being used.  The handleLock also protects against the race condition
 *     of two threads trying to create the same handle.  Thus it guarantees the
 *     creation of a new handle is atomic.  A newly created handle is created
 *     locked for write.
 *
 *     Note that even though the lock is held for write as an exclusive lock,
 *     the disk data is not modified during this operation, so no transaction
 *     is supplied. */

static epi_anode_t NewHandle(
  IN struct async_device *device,
  IN daddr_t block,
  IN u_long  offset,
  OUT int *newP)
{
    long code;
    epi_anode_t h;
    struct epia_anode handleKey;	/* to use for lookup */

    h = 0;
    *newP = 0;
    handleKey.device = device;		/* setup key for hash lookup */
    handleKey.block = block;
    handleKey.offset = offset;

    BEGIN_MUTEX (&handleLock) {
	h = dfsh_HashLookup (&handleHT, &handleKey);
	if (h) {
	    epix_EnterCodeBlock(3);
	    if (h->refCount == 0) {
		epix_EnterCodeBlock(2);
		FIFO_REMOVE (&handleFreeList, &h->thread);
		FIFO_ADD (&handleInuseList, &h->thread);
	    }
	    h->refCount++;
	    continue;
	}
	epix_EnterCodeBlock(4);
	h = NewAnodeHandle();
	if (h == 0) EXIT_CRIT;

	*newP = 1;
	{   char *save = h->disk;	/* zero all but disk buffer ptr */
	    bzero((caddr_t)h, sizeof (*h));
	    h->disk = save;
	}
	tlock_Init (&h->lock, "anode");
	tlock_ObtainWriteNT (&h->lock);

	/* these two fields are protected by the handleLock so set them here */
	h->refCount = 1;
	h->GConClose = 0;
	FIFO_ADD (&handleInuseList, &h->thread);

	/* these are needed for hashing */
	h->device = device;
	h->block = block;
	h->offset = offset;

	code = dfsh_HashIn_r (&handleHT, h);
	afsl_MBZ(code);		/* duplicate entry? */
    } END_LOCK;
    return h;
}

/* HANDLE_OK -- checks a handle for basic reasonability.  Returns TRUE if it is
 *     okay to use. */

#define HANDLE_OK(h) (((h) != 0) && (h)->device && (h)->refCount && \
		      ExistingAnode(h))

/* PutHandle -- decrements an anode's refCount.  If the anode has been
 *     deleted or if it has been marked for garbage collection the
 *     CleanupHandle function is called.
 *
 * LOCKS USED -- The anode must be locked for write. */

static void PutHandle(IN epi_anode_t handle)
{
    epix_EnterCodeBlock(5);
    if (handle == 0) return;
    AssertTWriteLocked (&handle->lock);

    BEGIN_MUTEX (&handleLock) {
	tlock_ReleaseWriteNT (&handle->lock);

	IS_TRACE_ANODE (handle, printf("Closed"));

	handle->refCount--;
	if (handle->refCount == 0) {
	    FIFO_REMOVE (&handleInuseList, &handle->thread);
	    if (!ExistingAnode(handle))
		/* don't cache anodes we don't own: deleted or never created */
		handle->GConClose++;
	    if (handle->GConClose) CleanupHandle (handle);

	    if (handle->device == 0)	/* reuse immediately */
		FIFO_ADD (&handleZeroList, &handle->thread);
	    else
		FIFO_ADD (&handleFreeList, &handle->thread);
	}
    } END_LOCK;
}

/* epix_SetGConClose -- Finds all the handles matching a particular pattern and
 *     marks them for cleanup next time their reference count reaches zero.
 *     The \arg{type} and \arg{rock} parameters are used in various ways to
 *     select which handles to garbage collect.  The \arg{type} is an
 *     \arg{enum} whose value specifies the interpretation of the \arg{rock}.
 *	  epix_gc_handle -- \arg{rock} is an anode handle.
 *	   epix_gc_device -- \arg{rock} is a device pointer.
 *	   epix_gc_volumeRock -- \arg{rock} is compared to volume rock.
 *	   epix_gc_fileRock -- \arg{rock} is compared to file rock.
 *     This mechanism is used to flush cached status information about an
 *     aggregate, in preparation for shutting it down. */

/* SHARED */
long epix_SetGConClose(
  IN enum epix_GCRockType type,
  IN opaque rock)
{
    static u_int handleOffsets[MAX(MAX(epix_gc_handle, epix_gc_device),
				   MAX(epix_gc_volumeRock, epix_gc_fileRock))
				+ 1];
    epi_anode_t h;
    long code = 0;

    if (!handleOffsets[epix_gc_handle]) {
	/* initialize table the first time */
	handleOffsets[epix_gc_handle] = -1; /* not used */
	handleOffsets[epix_gc_device] = offsetof(struct epia_anode, device);
	handleOffsets[epix_gc_volumeRock] =
	    offsetof(struct epia_anode, volume.rock);
	handleOffsets[epix_gc_fileRock] =
	    offsetof(struct epia_anode, file.rock);
    }
    if (rock == 0) return (EPI_E_BADROCK);

    BEGIN_MUTEX (&handleLock)
    switch (type) {
      case epix_gc_handle:
	h = (epi_anode_t)rock;
	epix_EnterCodeBlock(6);
	if (!HANDLE_OK(h)) { ABORT_CRIT (EPI_E_BADANODE); }
	h->GConClose = 1;
	/* Since we know the caller has an active reference (HANDLE_OK didn't
	 * fail) we don't have to call CleanupHandle ourselves. */
	break;

      case epix_gc_device:
      case epix_gc_volumeRock:
      case epix_gc_fileRock:
	{   /* walk both inuse and free lists */
	    u_int off = handleOffsets[type];
	    epi_anode_t next;
	    h = (epi_anode_t)FIFO_LookAtOldest(&handleFreeList,
					offsetof(struct epia_anode, thread));
	    for (; h; h = next) {
		next = (epi_anode_t)FIFO_NextOldest (&handleFreeList,
						     h, &h->thread);
		if (*(long *)((char *)h + off) == (long)rock) {
		    h->GConClose = 1;
		    if (h->refCount == 0) {
			/* do it now, if already closed */
			FIFO_REMOVE (&handleFreeList, &h->thread);
			CleanupHandle (h);
			FIFO_ADD (&handleZeroList, &h->thread);
		    }
		}
	    }
	    for (h = (epi_anode_t)
		FIFO_LookAtOldest(&handleInuseList,
				offsetof(struct epia_anode, thread));
		h;
		h = (epi_anode_t)
		FIFO_NextOldest (&handleInuseList, h, &h->thread)) {
		/* Since these are in use handles, don't have to check for zero
		 * device or refCount. */
		if (*(long *)((char *)h + off) == (long)rock)
		    h->GConClose = 1;
	    }
	}
	break;
      default:
	code = EPI_E_BADARGS;
    } END_LOCK;
    return code;
}

/* epix_CountOpenAnodes -- Determines the number of open anodes on a particular
 *     \arg{device} and assigns it to \arg{countP}.  This is mostly used as a
 *     check, before shutting down an aggregate, to make sure it is no longer
 *     in use. */

/* SHARED */
long epix_CountOpenAnodes(
  IN struct async_device *device,
  OUT long *countP)
{
    epix_EnterCodeBlock(10);
    afsl_Assert (device && countP);
    *countP = 0;
    BEGIN_MUTEX (&handleLock) {
	epi_anode_t h;
	for (h = (epi_anode_t)
		FIFO_LookAtOldest(&handleInuseList,
				offsetof(struct epia_anode, thread));
		h;
	     h = (epi_anode_t)
		FIFO_NextOldest (&handleInuseList, h, &h->thread)) {
	    /* Since these are in use handles, don't check for 0 refCount */
	    if (h->device != device) continue;
	    (*countP)++;
	}
    } END_LOCK;
    return 0;
}

/* Open -- The internal version which opens a container given the disk address
 *     of its anode and returns its handle.  A new handle is allocated if
 *     necessary.  The handle should be release by calling PutHandle.
 *
 *     Since this can be called during salvage we must not panic here.  If
 *     things are too badly trashed return EPI_E_BADANODEPTR.
 *
 * PARAMETERS -- The type parameter is used when reading a new anode handle
 *     from the disk.  If non-zero, it is used to check the anode flags word.
 *     If -1 then the anode must exist but the type is unknown.  The zero value
 *     is used by create and requires that the anode type be deleted.  The type
 *     obtained from the anode flags is used to assert that the magic field in
 *     the block containing the anode is consistent.
 *
 *     The volIndex parameter is used to check anode block consistency.
 *
 *     The output handle is always set to zero if an error occurs, otherwise
 *     the handle is returned with its refCount bumped. */

static long Open(
  IN struct async_device *device,
  IN daddr_t block,
  IN u_long  offset,
  IN int type,
  IN epi_anode_t volC,
  IN u_long volIndex,
  OUT epi_anode_t *handleP)
{
    long  code;
    epi_anode_t h;
    int new;

    /* First validate anode block/offset pair.  More checks below. */

    if (!block || !offset || !epix_IsBlockAllocated(block) ||
	!epic_OffsetOK (offset) ||
	!epic_LegalOffset(offset, device->blkSize)) {
	*handleP = 0;
	return EPI_E_BADANODEPTR;
    }

    code = 0;
    h = NewHandle (device, block, offset, &new);
    if (!new) {

	if (h == volC) {
	    /* Handle recursive opening of avl gracefully */
	    afsl_Assert (ExistingAnode(h) && type &&
			 ((type == -1) || (type == AnodeType(h))));
	    *handleP = h;
	    return 0;
	}

	tlock_ObtainWriteNT(&h->lock);
	if (type) {
	    if (!ExistingAnode(h) || ((type > 0) && (type != AnodeType(h)))) {
		PutHandle(h);
		h = NULL;
		return EPI_E_BADANODEINDEX;
	    }
	} else {
	    afsl_Assert (!ExistingAnode(h));
	}

    } else {
	/* The new handle is returned already write locked. */
	afsl_Assert(tlock_Check(&h->lock) == -1);
    }

    if (new || (type == 0)) {

	/* Initialize new handle with copy of disk data, and check that this is
         * really an anode.  Or if creating an anode read the
         * volume_table-modified disk data into the handle (new or not). */

	afsl_Assert(!h->allocationHandle && !h->quotaProc &&
		    !h->allocation.cleanup && !h->volume.cleanup &&
		    !h->file.cleanup &&
		    !h->flags);			/* no flags yet */

	BEGIN_LCRIT (struct elbb_buffer *b,
		     code = elbb_Read (device, block, elbb_bufLogged, &b),
		     code = elbb_Release (b, elbb_lazy)) {
	    char *anodeBlock;		/* block containing avl anode */
	    struct diskAnode *a;	/* ptr to disk anode in buffer */
	    long blockType;		/* type field of anode block */

#if (AFSL_DEBUG_LEVEL > 1) || !defined(KERNEL)

	    /* if user space (salvage) or debugging enabled, check the anode
             * block.  If bad we'll handle it below. */

	    if (!epic_AnodeBlockOkay (b, 0, volIndex))
		{ ABORT_CRIT (EPI_E_BADANODEPTR) }
#endif
	    anodeBlock = elbb_BufferData (b);
	    a = (struct diskAnode *)(anodeBlock + offset);
	    bcopy ((caddr_t)a, h->disk, ANODE_SIZE);
	    DFSH_MemFromDiskHyper(h->length, a->diskLength);
	    h->volIndex = epic_GetVolIndex (b);
	    blockType = epic_AnodeBlockType(b);
	    
	    /* If type specified, then check it.  If not being created, check
             * for legal type (volume contain only regular anodes, and the AVL
             * never contains regular anodes).  If specific anode type
             * required, check for the desired type.  If being created, assert
             * that the anode is deleted. */

	    if (((blockType != ANODE_BLOCK_VOLUME_MAGIC) &&
		 (blockType != ANODE_BLOCK_AVL_MAGIC)) ||
		(type &&
		 (((blockType == ANODE_BLOCK_VOLUME_MAGIC)
		  != (AnodeType(h) == AFLAGS_TYPE_REGULAR)) ||
		  !ExistingAnode(h) ||
		  ((type > 0) && (type != AnodeType(h)))))) {
		code = EPI_E_BADANODEINDEX;
		continue;
	    }
	} END_CRIT;
	if (code) {
	    /* I/O error or not a good anode block/offset */
	    PutHandle(h);		/* flush this handle immediately */
	    h = NULL;
	} else
	    afsl_Assert (type || DeletedAnode(Disk(h)->flags));
    }

    if (h)
	tlock_ReleaseWriteNT (&h->lock);

    IS_TRACE_ANODE (h, printf("Opened"));

    *handleP = h;
    return code;
}

/* GetBackingAnode -- Sets \arg{backingP} to the handle for the backing anode
 *     of anode \arg{h}.  It is not an error to call this with an anode that is
 *     not COW; a zero handle is returned in that case.  The caller must {\em
 *     not} close the returned handle.  The backing handle will be closed
 *     automatically when the COW handle is closed, deleted, or cloned.  Thus
 *     the returned handle is only safe to use as long as these operations are
 *     prevented.
 *
 *     This can only return a non-zero error code if \arg{h} is COW but the
 *     backing block/offset infomation is bad.  The salvager will want to see
 *     this as an error code.  Other callers should panic indicating disk
 *     format corruption.
 *
 * LOCKS USED -- This may lock the backing anode (as a side effect of calling
 *     Open.  So this procedure may not be used if the caller holds the backing
 *     anode locked (e.g. epix_MoveData).
 *
 * CAUTIONS -- The caller MUST NOT close the returned handle. */

static long GetBackingAnode(
  IN epi_anode_t h,
  OUT epi_anode_t *backingP)
{
    daddr_t block;
    u_long offset;
    long code;

    epix_EnterCodeBlock(11);
    afsl_Assert (AnodeType(h) != 0);
    AssertTWriteLocked (&h->lock);	/* as we may modify the handle... */

    if (*backingP = h->backingAnode) return 0;
    if (!(Disk(h)->flags & AFLAGS_COPY_ON_WRITE)) return 0;

    block = Disk(h)->c.backingAnode.block;
    offset = Disk(h)->c.backingAnode.offset;

    code = Open (h->device, block, offset, AnodeType(h), NULL, 0, backingP);
    if ((code == 0) && (epia_GetCopies(*backingP) == 0))
	code = EPI_E_BADBACKING;

    h->backingAnode = *backingP;
    return code;
}

/* epix_GetBackingAnode -- external version of GetBackingAnode. */

long epix_GetBackingAnode (h, backingP)
  IN epi_anode_t h;
  OUT epi_anode_t *backingP;
{
    long code;
    BEGIN_TLOCK_WRITE_NT (&h->lock) {
	code = GetBackingAnode (h, backingP);
    } END_LOCK;
    return code;
}

/* FindAppropriateTree -- see epix_FindAppropriateTree.
 *
 *     This function may only be called on BLOCKED anodes with \arg{block}
 *     addressing some indirect block.  Returns the indirect block treesize in
 *     which \arg{block} is located in \arg{treesizeP}. */

static void FindAppropriateTree(
  IN baddr_t   block,
  IN u_int     ibsize,			/* number elements in indirect block */
  OUT struct epix_blockLocation *loc,	/* tree info */
  OUT u_long  *treesizeP)		/* size of indirect tree */
{
    baddr_t base;
    long treesize;

    epix_EnterCodeBlock(12);
    afsl_Assert(block >= ANODE_DIRECTBLOCKS); /* direct blocks handled elsewhere */

    base = ANODE_DIRECTBLOCKS;
    if ((block - base) < (treesize = ibsize)) {
	loc->tree = 0;
	loc->span = 1;
    } else if ((block - (base += ibsize)) < (treesize *= ibsize)) {
	loc->tree = 1;
	loc->span = ibsize;
    } else {
	base += treesize;
	loc->span = treesize;		/* ibsize^2 */
#define CUBE_ROOT_MAX_VALUE_U_INT32   1625 /* + 0.48254 */
	if (ibsize > CUBE_ROOT_MAX_VALUE_U_INT32) {
	    loc->tree = 2;
	    treesize = 0;		/* can't represent this */
	} else if ((block - base) < (treesize *= ibsize)) {
	    loc->tree = 2;
	} else {
	    base += treesize;
	    loc->span = treesize;	/* ibsize^3 */
#define FOURTH_ROOT_MAX_VALUE_U_INT32  255 /* + 0.99809 */
	    if (ibsize > FOURTH_ROOT_MAX_VALUE_U_INT32) {
		loc->tree = 3;
		treesize = 0;		/* can't represent this */
	    } else if ((block - base) < (treesize *= ibsize)) {
		loc->tree = 3;
	    }
	}
    }
    /* this can't really happen, given 32-bit block addresses */
    afsl_Assert(loc->tree < ANODE_INDIRECT_TREES);
    loc->base = base;
    *treesizeP = treesize;
}

/*
 * epix_FindAppropriateTree -- Initializes the \arg{loc} structure for an
 *     indirect block reference to the logical \arg{block}, assuming the
 *     indirect block holds \arg{ibsize} block addresses (see
 *     \fcn{epix_GetIBSize}).  It is also suitable for use with non-blocked
 *     anodes and with direct blocks, in which case \arg{ibsize} is ignored.
 *
 *     The static function FindAppropriateTree does the real work. */

/* SHARED */
void epix_FindAppropriateTree(
  IN baddr_t   block,
  IN u_int     ibsize,			/* number elements in indirect block */
  OUT struct epix_blockLocation *loc)	/* tree info */
{
    if (block < ANODE_DIRECTBLOCKS) {
	EPIX_MakeDirectBlockLoc (loc, block);
    } else {
	u_long dummy;
	FindAppropriateTree (block, ibsize, loc, &dummy);
    }
}

/*
 * epix_GetIBSize -- Returns number of pointers in an indirect block on the
 *     aggregate in which \arg{h} resides.
 */

/* SHARED */
long epix_GetIBSize(epi_anode_t h)
{
    return IBSIZE(h);
}

/*
 * How many blocks are needed (including metadata) to store nblocks worth
 * of user data
 */
int
epix_BlocksNeeded(epi_anode_t ap, int nblocks)
{
    int blocksPerIndirect = IBSIZE(ap);
    int indirects, overhead = 0;
    int doNotFit;

    /*
     * blocks that need more indirect blocks in order to be addressed.
     */
    doNotFit = nblocks - ANODE_DIRECTBLOCKS;

    while (doNotFit > 0) {
	/*
	 * how many indirect blocks do we need to address the blocks
	 * that don't yet fit
	 */
	indirects = howmany(doNotFit, blocksPerIndirect);
	overhead += indirects;
	/*
	 * of these indirect blocks we can only fit one in the anode
	 * the rest have to go into higher level indirects
	 */
	doNotFit = indirects - 1;
    }

    return (nblocks + overhead);
}

/* epix_blockType(daddr_t fsb) 0 -> empty,  1 -> COW, 2 -> Writable */

#define epix_blockType(fsb, isCow) \
    (!epix_IsBlockEmpty(fsb)) + (epix_IsBlockWritable(fsb) && !(isCow))

#define epix_Contiguous(t, p, isCow) \
    ((t) == (p) + 1 && epix_blockType(t, isCow) == epix_blockType(p, isCow))

int
epix_CheckBlockAllocation(
  epi_anode_t ap,
  off_t off,
  daddr_t *fsbP,
  long *alenP)
{
    long code;
    int i;
    u_long treesize;			/* size of whole indirect tree | 0 */
    daddr_t ibAddr;			/* physical block addr of IB */
    u_int ibsize;			/* number elements in indirect block */
    baddr_t lbn;
    epix_blockLoc_t loc;
    diskAnode_t *dap;
    int inCOWtree = 0;
    async_device_t *aggr = ap->device;

    tlock_ObtainRead(&ap->lock);
    afsl_Assert(off < epia_GetLen32(ap));

    dap = Disk(ap);
    lbn = osi_btoab(aggr, off);

    /* Check for simple direct block case. */

    if (lbn < ANODE_DIRECTBLOCKS) {
	for (i = lbn + 1; i < ANODE_DIRECTBLOCKS; i++) {
	    daddr_t this = dap->dBlock[i];
	    daddr_t prev = dap->dBlock[i - 1];

	    if (!epix_Contiguous(this, prev, inCOWtree))
		break;
	}
	*fsbP = dap->dBlock[lbn];
	*alenP = osi_abtob(aggr, i - lbn);
	tlock_ReleaseRead(&ap->lock);
	return (0);
    }

    /* it's an indirect block */

    ibsize = IBSIZE(ap);

    loc.depth = 0;
    FindAppropriateTree(lbn, ibsize, &loc, &treesize);

    /*
     * Now we've found the correct tree of indirect blocks.
     * "treesize" is the size of the whole tree (in blocks), or zero
     * if the size of the tree can't be represented in 32 bits.
     * "span" is initialized to the separation, in blocks, between
     * consecutive entries in the top indirect block.  "lbn" is the
     * desired logical block address.  "base" is the logical address,
     *in blocks, of the first block in the tree.
     */

    afsl_Assert(treesize != 0);
    ibAddr = dap->iBlock[loc.tree];
    do {
	struct elbb_buffer *ibb;	/* indirect block buffer */
	struct epia_indirectBlock *ib;
	u_long index;			/* last index in current ib */

	index = (lbn - loc.base) / loc.span;
	afsl_Assert(index < ibsize);

	if (epix_IsBlockEmpty(ibAddr)) {
	    /*
	     * the indirect block is not allocated, thus our block is a hole
	     * and so are all the other blocks that belong in this tree.  To
	     * avoid integer overflow, we put a bound on the number of blocks.
	     */
	    u_long empty_blocks = loc.span * (ibsize - index);
	    empty_blocks = MIN(empty_blocks, osi_btoab(aggr, INT_MAX));
	    *fsbP = EFS_HOLE;
	    *alenP = osi_abtob(aggr, empty_blocks);
	    tlock_ReleaseRead(&ap->lock);
	    return (0);
	}
	if (!epix_IsBlockWritable(ibAddr)) {
	    inCOWtree = 1;
	    ibAddr = epix_MapBlockToBacking(ibAddr);
	}

	afsl_Assert(loc.depth < ANODE_INDIRECT_TREES);
	loc.blocks[loc.depth] = ibAddr;
	loc.indexes[loc.depth] = index;
	loc.depth++;

	code = elbb_Read(aggr, ibAddr, elbb_bufLogged, &ibb);
	if (code != 0) {
	    tlock_ReleaseRead(&ap->lock);
	    return code;
	}

	ib = (struct epia_indirectBlock *) elbb_BufferData(ibb);

	if (loc.span == 1) {
	    /* This is it.  Gather up as many blocks as we can. */
	    for (i = index + 1; i < ibsize; i++) {
		daddr_t this = ib->blocks[i];
		daddr_t prev = ib->blocks[i - 1];

		if (!epix_Contiguous(this, prev, inCOWtree))
		    break;
	    }
	    *fsbP = ib->blocks[index];
	    *alenP = osi_abtob(aggr, i - index);
	    if (inCOWtree)
		*fsbP |= EPIX_ANODE_BLOCKBACKING;
	} else
	    /* loc.span != 1 so we have to get more indirect blocks */
	    ibAddr = ib->blocks[index];

	code = elbb_Release(ibb, elbb_lazy);
	if (code) {
	    tlock_ReleaseRead(&ap->lock);
	    return code;
	}

	/*
	 * If it's an indirect block, calculate new base&span
	 * and go  around again
	 */
	loc.base += index * loc.span;
	loc.span /= ibsize;
    } while (loc.span != 0);

    tlock_ReleaseRead(&ap->lock);
    return 0;
}

/* CheckIBFree -- determines how much of an indirect block is free.  restFree
 *     gives the offset of the first non-free block following index, but at
 *     least index+nBlocks, but never past the end of the current indirect
 *     block, unless all the following blocks are free but allFree is false, in
 *     which case restFree indicates one past the end of the indirect block.
 *     allFree is set if the entire block is empty. */

void CheckIBFree(
  IN struct epia_indirectBlock *ib,
  IN int index,
  IN int nBlocks,
  OUT int *restFreeP,
  OUT int *allFreeP)
{
    int i;
    afsl_Assert(nBlocks > 0);

    *allFreeP = 1;
    for (i = 0; i < ib->ibsize; i++) {	/* might be able to free this ib */
	if (epix_IsBlockReadable(ib->blocks[i])) {
	    epix_EnterCodeBlock(13);
	    *allFreeP = 0;
	    break;
	}
    }
    if (*allFreeP) {
	epix_EnterCodeBlock(14);
	*restFreeP = ib->ibsize-1;	/* stay in this indirect block */
	return;
    }

    /* For indirect blocks we'll keep going, so nBlocks only applies later */
    if (ib->span > 1) nBlocks = 1;
    else if (nBlocks > ib->ibsize-index) nBlocks = ib->ibsize-index;

    /* First check blocks to be returned.  If some are used, set restFree to
     * follow them, unless that would put in into the next block. */
    for (i = index; i < index+nBlocks; i++) {
	if (epix_IsBlockReadable(ib->blocks[i])) {
	    epix_EnterCodeBlock(15);
	    if (index+nBlocks >= ib->ibsize) /* stay in this ib */
		*restFreeP = ib->ibsize-1;
	    else *restFreeP = index+nBlocks;
	    return;
	}
    }
    /* Then check the blocks (if any) following the ones being returned.  If we
     * encounter one that's not free, set restFree just before it. */
    for (i = index + nBlocks; i < ib->ibsize; i++) {
	if (epix_IsBlockReadable(ib->blocks[i])) {
	    epix_EnterCodeBlock(16);
	    *restFreeP = i;
	    return;
	}
    }

    /* Clear sailing from here to end of indirect block.  Whole I.B. is not
     * free only because of blocks "behind us".  So restFree is set to get past
     * this indirect block since we won't be able to free it. */
    epix_EnterCodeBlock(17);
    *restFreeP = ib->ibsize;
}

/* FindBlocks -- Walks the block map for \arg{h} and locates the physical
 *     block(s) corresponding to (and following) logical \arg{block}.
 *
 *     The block address at the requested location and block addresses
 *     following it are copied into the \arg{pBlock} array.  The length of the
 *     array is given by \arg{nBlocks}, and no more than this many addresses
 *     will be returned.  The number actually found is returned in
 *     \arg{nFoundP}.  If \arg{forRead} is non-zero, all block addresses
 *     encountered are mapped to the backing anode (i.e. the COW bit is
 *     cleared).  If it is zero, the block addresses referencing blocks
 *     allocated in a backing anode are treated as unallocated for the purposes
 *     of continuing the block map walk, but allocated for the purposes of
 *     deciding if an indirect block is empty.
 *
 *     The \arg{nSkipP} output parameter, if non-zero, gives a conservative
 *     estimate of the distance to the next allocated block.  It can be added
 *     to the value of the \arg{block} parameter to locate the next allocated
 *     block.  This call is quaranteed to return at least one (possibly
 *     unallocated) block and therefore \arg{nFoundP} will be set to at least
 *     one.  The skip distance is not guaranteed to be efficient.  If
 *     \arg{block} is zero and the rest of the container (past \arg{nFoundP})
 *     is empty then \arg{nSkipP} is set to \hex{ffffffff}.  This value
 *     distinguishes it from the one where an empty indirect block is being
 *     returned (see below).  Otherwise the termination condition is when
 *     $\arg{block} + (*\arg{nSkipP}) = 0$.
 *
 *     If the \arg{nSkipP} pointer is non-zero and \arg{block} is inside an
 *     empty indirect block then the address of the indirect block itself is
 *     returned.  This is used to locate indirect blocks so they can be
 *     deallocated.  The returned value of \arg{nSkipP} is zero in this case.
 *
 *     There are four combinations of the \arg{forRead} and \arg{nSkipP}
 *     parameters that customize the behavior for different uses.
 *	  (forRead && !nSkipP) -- epix_Map and epia_Reserve (visible)
 *	  (forRead && nSkipP) -- epia_Strategy (read) and epia_Truncate to
 *	      count backing indirect block trees,
 *	  (!forRead && !nSkipP) -- epia_Write, epia_Strategy (write) and
 *		epia_Reserve (allocated), and
 *	  (!forRead && nSkipP) -- epia_Truncate and epia_Fsync.
 *
 * CAUTIONS -- called during salvage (see caution at epia_Map).
 *
 * \typ{struct epix_blockLocation} --
 *     The position in the block map of the block returned is indicated by
 *     values in the location structure.  This structure encapsulates the state
 *     of a block reference.  While primarily needed for indirect block
 *     references it also encodes direct block references.  This structure is
 *     filled in during the tree walk and this information is used in the
 *     \fcn{epix_Insert} and \fcn{epix_MoveData} procedures which modify the an
 *     anode's block map.  The information stored in the \arg{loc} structure
 *     describes the position of the physical disk address corresponding to a
 *     logical block number in the anode's block map.  Implicit in this
 *     information is the anode's handle and the logical block being addressed.
 *
 *     The field \fld{tree} is (-1) for a direct block reference or the index
 *     in indirectBlockTree array (0..3) for an indirect block reference.  If
 *     \fld{tree} is (-1) then the position is given by \fld{directBlock} which
 *     gives the index in the anode's direct array.  Otherwise the remaining
 *     fields specify a position in the tree of indirect blocks.
 *
 *     Generally the \fld{base} and \fld{span} fields describe the last
 *     indirect block in the \fld{blocks} array at index $\fld{depth}-1$.  The
 *     \fld{base} is the logical block number corresponding to the first block
 *     of \fld{tree}.  The \fld{span} is the separation, in logical blocks
 *     between consecutive addresses in the indirect block.  This is 1 for the
 *     terminal (1st level) indirect block which contains data block addresses,
 *     it will have the value ibsize (see \fcn{epix_GetIBSize}) for 2nd level
 *     indirect blocks, $\mbox{ibsize}^2$ for third level indirect blocks, etc.
 *
 *     The \fld{depth} field specifies how many entries in the \fld{blocks} and
 *     \fld{indexes} arrays are in use.  The \fld{blocks} array contains the
 *     disk addresses of each indirect block traversed.  The \fld{indexes}
 *     array contains the index in the corresponding indirect block of the next
 *     block.
 *
 *     In the case where \fld{tree} is not (-1) and \fld{depth} is zero, the
 *     reference is to the top level indirect block specified by \fld{tree}.
 *     The \fld{base} and \fld{span} fields refer to the indirect block what
 *     would appear at this point.  Thus the values of \fld{base} and
 *     \fld{span} are the same, both with \fld{depth} zero and \fld{depth} one.
 *
 *     The \fld{visibleAdjust} field is used only when a backing reference to
 *     an indirect tree is being removed.  \fcn{epix_Insert} must calculate the
 *     size of the tree being removed so it can update the anode's visible
 *     value.  The caller of \fcn{epix_Insert} must also update the fileset
 *     quota, so this information is returned in this field of the \arg{loc}
 *     structure. */

static long FindBlocks(
  IN epi_anode_t h,
  IN baddr_t block,
  IN u_long nBlocks,			/* size of pBlock array */
  IN int forRead,			/* map block addrs to backing anode */
  OUT daddr_t pBlock[],			/* physical block addresses */
  OUT int *nFoundP,			/* number addresses returned */
  OUT u_long *nSkipP,			/* distance to next allocated block */
  OUT struct epix_blockLocation *loc)
{
    long code;
    int i;
    struct epix_blockLocation tmpLoc;	/* in case location == NULL */
    u_long treesize;			/* size of whole indirect tree | 0 */
    daddr_t ibAddr;			/* physical block addr of IB */
#ifdef AFS_DEBUG
    daddr_t lastIBAddr;			/* previous IB addr */
    int lastIndex;			/* index in previous IB */
#define IfDebug(x) (x)
#else
#define IfDebug(x)
#endif
    u_int ibsize;			/* number elements in indirect block */
    int nf;				/* number found */
    baddr_t ns;				/* number to skip */
    int allFree;			/* whole ib is free */
    int restFree;			/* offset of next used ib slot */
    baddr_t oldBase;
    u_long oldSpan;
    int cowok;				/* COW anode implies backing refs ok */
    struct diskAnode *dap = Disk(h);

    epix_EnterCodeBlock(18);
    if (dap->flags & AFLAGS_DATA_NOTBLOCKED)
	return EPI_E_NOTBLOCKEDDATA;

    cowok = (dap->flags & AFLAGS_COPY_ON_WRITE);

    nf = 0;
    if (nFoundP != NULL)
	*nFoundP = 0;
    if (nSkipP != NULL)
	*nSkipP = 0;

    if (loc == NULL)
	loc = &tmpLoc;		/* to avoid checking for null */
    loc->tree = -1;		/* initialize to illegal values */
    loc->directBlock = -1;
    loc->depth = 0;

    /* Check for simple direct block case. */

    if (block < ANODE_DIRECTBLOCKS) {
	epix_EnterCodeBlock(19);
	loc->tree = -1;
	loc->directBlock = block;
	loc->base = 0;
	loc->span = 0;
	nf = MIN(ANODE_DIRECTBLOCKS - block, nBlocks);
	for (i = 0; i < nf; i++) {
	    daddr_t b = dap->dBlock[block + i];
	    if (!cowok && EPIX_IsBlockBacking(b))
		return EPI_E_BADBACKING;
	    if (forRead)
		b = epix_MapBlockToBacking(b);
	    pBlock[i] = b;
	}
	if (nFoundP != NULL)
	    *nFoundP = nf;
	if (nSkipP != NULL) {
	    int empty = 1;
	    epix_EnterCodeBlock(20);
	    ns = nf;		/* count following empty blocks */
	    for (i = block + nf; empty && i < ANODE_DIRECTBLOCKS; i++) {
		empty = epix_IsBlockEmpty(dap->dBlock[i]);
		if (empty)
		    ns++;
	    }
	    /* if direct blocks are empty, check trees as cheap opt. */
	    for (i = 0; empty && i < ANODE_INDIRECT_TREES; i++) {
		empty = epix_IsBlockEmpty(dap->iBlock[i]);
	    }
	    if (empty) {
		*nSkipP = (block == 0) ? -1 : -block;
	    } else
		*nSkipP = ns;
	}
	return 0;
    }

    ibsize = IBSIZE(h);
    FindAppropriateTree(block, ibsize, loc, &treesize);

    /* Now we've found the correct tree of indirect blocks.  "treesize" is the
     * size of the whole tree (in blocks), or zero if the size of the tree
     * can't be represented in 32 bits.  "span" is initialized to the
     * separation, in blocks, between consecutive entries in the top indirect
     * block.  "block" is the desired logical block address.  "base" is the
     * logical address, in blocks, of the first block in the tree. */

    IfDebug(lastIBAddr = -1);		/* top level: check anode */
    ibAddr = dap->iBlock[loc->tree];
    if (!cowok && EPIX_IsBlockBacking(ibAddr))
	return EPI_E_BADBACKING;
    if (forRead && EPIX_IsBlockInBacking(ibAddr)) {
	ibAddr = epix_MapBlockToBacking(ibAddr);
	IfDebug(lastIBAddr = -2);	/* don't check parent: in backing */
    }
    if (!epix_IsBlockAllocated(ibAddr)) {
	epix_EnterCodeBlock(21);
	if (nSkipP != NULL) {
	    int empty = 1;
	    /* entire tree is empty, check rest of trees as cheap opt. */
	    epix_EnterCodeBlock(22);
	    for (i = loc->tree + 1; empty && i < ANODE_INDIRECT_TREES; i++) {
		empty = epix_IsBlockEmpty(dap->iBlock[i]);
	    }
	    /* Block is not zero here, so nSkip will not be zero. */
	    *nSkipP = (empty) ? -block : treesize - (block - loc->base);
	}
	afsl_Assert(loc->depth == 0);	/* top level indirect block */
	pBlock[0] = ibAddr;
	if (nFoundP)
	    *nFoundP = 1;
	return 0;
    }

    /* need to walk an indirect block tree */
    epix_EnterCodeBlock(23);
    allFree = 0;
    restFree = 0;
    oldBase = loc->base;
    oldSpan = loc->span;
    while (1) {
	struct elbb_buffer *ibb;	/* indirect block buffer */
	struct epia_indirectBlock *ib;
	u_long index;			/* last index in current ib */

	index = (block - loc->base) / loc->span;
	afsl_Assert(index < ibsize);
	loc->blocks[loc->depth] = ibAddr;
	loc->indexes[loc->depth] = index;
	loc->depth++;
	afsl_Assert(loc->depth > 0 && loc->depth <= ANODE_INDIRECT_TREES);

	BEGIN_CRIT (code = elbb_Read(h->device, ibAddr, elbb_bufLogged, &ibb),
		    code = elbb_Release(ibb, elbb_lazy)) {
	    ib = (struct epia_indirectBlock *) elbb_BufferData (ibb);
	    afsl_Assert(IndirectBlockOkay(ib, h, lastIBAddr, lastIndex,
					loc->base, loc->span));
	    afsl_Assert(loc->depth == 1 ||
		    lastIndex == loc->indexes[loc->depth - 2]);
	    if (loc->span == 1) {
		/* This is it.  Copy out as many blocks as we can. */
		epix_EnterCodeBlock(24);
		nf = MIN(ibsize - index, nBlocks);
		for (i = 0; i < nf; i++) {
		    daddr_t b = ib->blocks[index + i];
		    if (!cowok && EPIX_IsBlockBacking(b))
			{ ABORT_CRIT (EPI_E_BADBACKING) }
		    if (forRead)
			b = epix_MapBlockToBacking(b);
		    pBlock[i] = b;
		}
	    } else {
		epix_EnterCodeBlock(25);
		IfDebug (lastIBAddr = ibAddr);
		IfDebug (lastIndex = index);
		ibAddr = ib->blocks[index];
		if (!cowok && EPIX_IsBlockBacking(ibAddr))
		    { ABORT_CRIT (EPI_E_BADBACKING) }
		if (forRead && EPIX_IsBlockInBacking(ibAddr)) {
		    ibAddr = epix_MapBlockToBacking(ibAddr);
		    IfDebug(lastIBAddr = -2); /* don't chk parent: backing */
		}
		pBlock[0] = ibAddr;	/* in case we're done */
		nf = 1;
	    }
	    if (nSkipP)
		CheckIBFree(ib, index, nBlocks, &restFree, &allFree);
	} END_CRIT;
	if (code)
	    return code;

	/* found terminal indirect block or found empty indirect block ptr */
	if (loc->span == 1 || !EPIX_IsBlockAccessible(ibAddr, forRead))
	    break;

	/* otherwise it's an indirect block so calculate new base&span and go
	 * around again */
	oldBase = loc->base;
	oldSpan = loc->span;
	loc->base += index * loc->span;
	loc->span /= ibsize;
	afsl_Assert(loc->span > 0);
    }

    /* We've found the last indirect block so just finish up */
    if (!forRead && nSkipP != NULL && allFree) {
	/* If we're looking for allocated blocks and this is a completely empty
	 * indirect block, then return it instead.  Set nSkip to zero so that
	 * we can free the whole right edge of a tree where bumping block by
	 * even one would skip the whole rest of the tree. */
	epix_EnterCodeBlock(26);
	nf = 1;
	loc->depth--;
	loc->base = oldBase;
	loc->span = oldSpan;
	pBlock[0] = loc->blocks[loc->depth];
	*nSkipP = 0;
    } else if (nSkipP != NULL) {
	/* We want to push into the first block of the indirect block indicated
	 * by restFree. */
	/* don't use oldSpan */
	*nSkipP = loc->base + restFree * loc->span - block;
    }

    if (nFoundP != NULL)
	*nFoundP = nf;
    return 0;
}

/* Map -- the internal version of the Map call.  It has a bunch of output
 *     parameters that tell the caller what happened during the map operation,
 *     any of which may be zero.  Much of the block tree walk is performed by
 *     FindBlocks.
 *
 * (obs) PARAMETERS -- If backingP is non-null the and the block is not present
 *     in the specified container backing is set to one.  If backingP is null
 *     and the block is not present it will be looked up in the backing
 *     container and the resulting address will be returned. */

static long Map(
  IN epi_anode_t handle,
  IN baddr_t block,
  OUT daddr_t *physicalBlockP)
{
    long code;
    epi_anode_t backingHandle;

    if (!HANDLE_OK(handle)) return EPI_E_BADANODE;
    AssertTLocked (&handle->lock);
    afsl_Assert(physicalBlockP);
    code = FindBlocks (handle, block,	/*nBlocks*/1, /*forRead*/1,
			physicalBlockP, 0, 0, 0);
    if (code) return code;
    if (epix_IsBlockReadable(*physicalBlockP)) {
	/* clear the backin bit (just in case) and return it */
	*physicalBlockP = epix_MapBlockToBacking (*physicalBlockP);
	return 0;
    }
#if 1
    afsl_Assert (!epix_IsBlockViaBacking(*physicalBlockP));
#else
    if (epix_IsBlockViaBacking(*physicalBlockP)) {
	epix_EnterCodeBlock(27);
	code = epix_GetBackingAnode (handle, &backingHandle);
	if (code) return code;
	if (backingHandle == 0) return EPI_E_BADBACKING;

	/* Only check backing container if backingP is null, otherwise just
	 * return TRUE. */
	else {
	    /* ??? LOCKING ??? */
	    code = Map (backingHandle, block, physicalBlockP);
	    if (code) return EPI_E_BADBACKING;
	}
	return 0;
    }
#endif
    afsl_Assert(epix_IsBlockEmpty(*physicalBlockP));
    return EPI_E_NOBLOCK;
}

/* epix_FindBlocks -- provides a SHARED interface to the FindBlocks procedure,
 *     see which.
 *
 * LOCKS USED -- The anode is read locked. */

/* SHARED */
long epix_FindBlocks(
  IN epi_anode_t h,
  IN baddr_t block,
  IN u_int nBlocks,
  IN int forRead,
  OUT daddr_t *pBlocks,
  OUT int *nFoundP,			/* signed for compat. w/ insert */
  OUT u_long *nSkipP,
  OUT struct epix_blockLocation *loc)	/* in indirect block tree */
{
    long code;

    afsl_AssertCond(HANDLE_OK(h));
    afsl_AssertCond(nBlocks != 0 && pBlocks != NULL);
    BEGIN_TLOCK_READ (&h->lock) {
	code = FindBlocks(h, block, nBlocks, forRead,
			  pBlocks, nFoundP, nSkipP, loc);
	afsl_PAssert((!nSkipP || (*nSkipP == 0xffffffff) ||
			(*nSkipP <= 0x7fffffff) || (*nSkipP + block == 0)),
			("nSkip probably negative: %#x", *nSkipP));
    } END_LOCK;
    return code;
}

/* epia_Map -- Maps a logical block in a container to the corresponding
 *     physical block address.  It provides a simplified EXPORTed interface to
 *     \fcn{epix_FindBlocks}.
 *
 *     This function is the only way to really find out which parts of a file
 *     are acutally backed by real data and which have never been written and
 *     will be read as zero. A sparse file containing only two blocks 100
 *     million bytes apart would be very annoying for a volume dump operation,
 *     unless it noticed that ${length} \gg 10^3 \times {allocated}$. There is
 *     also a proposal to add a powerful function to fileset operations vector
 *     that would facilitate dumping sparse files. This may require exposing a
 *     function like \fcn{epix_FindBlocks} as an EXPORTed function using the
 *     \prefix{epia_}.
 *
 * PARAMETERS --
 *     handle -- anode handle.
 *     block -- logical block address in container to be mapped.
 *     pBlockP -- is set to the corresponding physical block address, if
 *	  non-null.
 *     deviceP -- is set to the device for this container, if non-null. Since
 *	  no one really uses the deviceP argument it should be flushed.
 *
 * CAUTIONS -- This routine may be called during salvage (to check log for
 *     recovery) on a potentially damaged anode.  We will only ask about block
 *     zero so only that case must not panic. */

/* EXPORT  */
long epia_Map(
  IN epi_anode_t handle,
  IN u_long block,
  OUT daddr_t *pBlockP,
  OUT struct async_device **deviceP)
{
    long code;

    if (!HANDLE_OK(handle)) return EPI_E_BADANODE;
    if (deviceP) *deviceP = handle->device;
    if (pBlockP) {
	BEGIN_TLOCK_READ (&handle->lock) {
	    code = FindBlocks (handle, block, /*nBlocks*/1, /*forRead*/1,
				pBlockP, 0, 0, 0);
	    if (code) continue;
	    if (epix_IsBlockEmpty(*pBlockP)) code = EPI_E_NOBLOCK;
	} END_LOCK;
	if (code) return code;
    }
    return 0;
}

static long IndexToOffset(
  IN epi_anode_t h,
  IN u_long index,
  OUT daddr_t *physicalBlockP,
  OUT u_long *offsetP)
{
    long code;
    int anodesPerBlock = epic_anodesPerBlock[h->device->logBlkSize-10];
    u_long block = index / anodesPerBlock;

    epix_EnterCodeBlock(28);
    code = Map (h, block, physicalBlockP);
    if (code) {
	afsl_Trace (AFSL_TR_ERRMAP,
		    ("IndexToOffset: got code %u from Map", code));
	return EPI_E_BADANODEINDEX;
    }
    *offsetP = epic_IndexToOffset(index % anodesPerBlock);
    return 0;
}

/* epia_Open -- Returns a handle for the anode at a specified index in an anode
 *     table. If an operation changes the size of the container and the
 *     \argname{quotaProc} is specified, it is called to validate the size
 *     change.  For details see the description of \fcname{q_Proc}, below.  If
 *     this procedure returns a non-zero value the size-changing operation will
 *     be aborted.  And the returned value will also be returned to its caller
 *
 *     The two optional parameters can be set later via explicit macro calls:
 *     \fcname{epia_SetQuotaProc} and \fcname{epia_SetAllocationHandle}.  An
 *     anode handle needs a \argname{quotaProc} if it is desired to perform
 *     quota checking.  It needs an \argname{allocationH} whenever a container
 *     grows on demand; it is an implicit parameter to the block allocation
 *     module.
 *
 *     The returned anode handle is an opaque pointer to a reference counted
 *     memory structure which caches the state of the anode.  And is used for
 *     all subsequent operation on the anode.  The handle is released with a
 *     call to \fcn{epia_Close} (or \fcn{epia_Delete}, if the anode is to be
 *     deleted as well as closed).
 *
 * PARAMETERS --
 *     volC -- anode table (fileset or AVL) in which the anode resides.
 *     index -- index of the anode in the anode table.
 *     allocationH -- optional handle for the block allocation module.
 *     quotaProc -- optional procedure to call when the size of an anode is
 *	  changed.
 *     handleP -- a pointer to the anode handle for the requested container.
 *	  On error the handle is zeroed.
 *
 * LOCKS USED -- Note that the new handle lock is held for write as an
 *     exclusive lock, the disk data is not modified during this operation so
 *     no transaction is supplied. */

/* EXPORT  */
long epia_Open(
  IN epi_anode_t volC,
  IN unsigned long index,
  IN opaque allocationH,
  IN epia_qproc_t quotaProc,
  OUT epi_anode_t *handleP)
{
    long code;
    u_long offset;
    daddr_t block;
    epi_anode_t h;

    epix_EnterCodeBlock(29);
    if (!handleP) return EPI_E_BADARGS;
    *handleP = 0;
    if (!HANDLE_OK(volC)) return EPI_E_BADANODE;

    h = 0;
    BEGIN_TLOCK_READ (&volC->lock) {
	code = IndexToOffset (volC, index, &block, &offset);
	if (code) continue;
	code = Open (volC->device, block, offset,
		     /*unknown type*/-1, volC, epia_GetIndex(volC), &h);
    } END_LOCK;
    if (code)
	return code;
#ifdef KERNEL
    afsl_Assert (index == epia_GetIndex(h));
#else
    /* don't assert if this might be the salvager */
    if (index != epia_GetIndex(h))
	code = EPI_E_BADANODEINDEX;
#endif

    /* propagate unlogged-ness */
    if (!epia_DeviceLogged(volC)) h->flags |= EPIA_HFLAGS_DEVICEUNLOGGED;
    else h->flags &= ~EPIA_HFLAGS_DEVICEUNLOGGED;
    
    if (!code) {
	epix_EnterCodeBlock(30);
	BEGIN_TLOCK_WRITE_NT (&h->lock) {
	    if (!ExistingAnode (h))
		{ ABORT_CRIT (EPI_E_BADANODEINDEX) }
	    /* only allow setting to same value */
	    if ((quotaProc && h->quotaProc &&
		 (quotaProc != h->quotaProc)) ||
		(allocationH && h->allocationHandle &&
		 (allocationH != h->allocationHandle)))
		{ ABORT_CRIT (EPI_E_NYI_MULTIPLEQUOTAPROCS) }
	    if (quotaProc) h->quotaProc = quotaProc;
	    if (allocationH) h->allocationHandle = allocationH;
	} END_LOCK;
    }

    if (code && h) {
	long ccode = epia_Close(h);
	afsl_MBZ (ccode);
	h = 0;
    }
    *handleP = h;
    return code;
}

/* epia_Close -- Closes the anode associated with \argname{handle}.
 *
 * PARAMETERS -- handle as returned by \fcn{epia_Open}.
 *
 * LOCKS USED -- Note that the lock is held for write as an exclusive lock, the
 *     disk data is not modified during this operation so no transaction is
 *     supplied. */

/* EXPORT  */
long epia_Close(epi_anode_t handle)
{
    long code;

    epix_EnterCodeBlock(31);
    afsl_Assert (HANDLE_OK(handle));
    BEGIN_CRIT (tlock_ObtainWriteNT (&handle->lock),
		if (code) tlock_ReleaseWriteNT (&handle->lock)) {
	if (handle->backingAnode) {
	    epix_EnterCodeBlock(32);
	    code = epia_Close (handle->backingAnode);
	    if (code) continue;
	    handle->backingAnode = 0;
	}
    } END_CRIT;
    if (code) return code;
    PutHandle (handle);
    return 0;
}

/* epix_GetBuffer -- Returns a buffer \arg{bP} that contains the data specified
 *     by logical address \arg{block} in the container \arg{handle}.  The
 *     caller must return this buffer when finished by calling
 *     \fcn{elbb_Release}. */

/* SHARED */
long epix_GetBuffer(
  IN epi_anode_t handle,
  IN baddr_t block,
  OUT struct elbb_buffer **bP)
{
    long code;
    daddr_t pBlock;
    enum elbb_bufType type;

    epix_EnterCodeBlock(34);
    *bP = 0;
    if (!HANDLE_OK(handle)) return EPI_E_BADANODE;
    if (Disk(handle)->flags & AFLAGS_DATA_NOTBLOCKED)
	return EPI_E_NOTBLOCKEDDATA;

    BEGIN_TLOCK_READ (&handle->lock) {
	if (Disk(handle)->flags & AFLAGS_LOGGED) type = elbb_bufLogged;
	else type = elbb_bufData;

	code = Map (handle, block, &pBlock);
	if (code) continue;
	code = elbb_Read (handle->device, pBlock, type, bP);
	if (code) continue;
    } END_LOCK;
    return code;
}

/* IsAnodeAllEmpty -- is a boolean function that returns true if the anode is
 *     BLOCKED and all the block addresses are empty.  Always returns zero if
 *     applied to an unBLOCKED anode, or to a COW anode. */

static int IsAnodeAllEmpty(IN epi_anode_t h)
{
    epix_EnterCodeBlock(41);
    AssertTLocked (&h->lock);
    if (Disk(h)->flags & AFLAGS_DATA_NOTBLOCKED) return 0;
#ifdef PAST_VISIBLE_QUOTA_TRANSITION_PERIOD
    if (Disk(h)->visible) return 0;
#else
    if (Disk(h)->allocated) return 0;
    if (!AreBlockAddrsEmpty(Disk(h))) return 0;
#endif
    return 1;
}

/*
 * epia_GetInfo -- returns information about an anode. This information
 *     includes its length, the number of fragments allocated to it, among
 *     other things.  It does a considerable amount of reorganizing and
 *     reformating of the data from the disk anode.
 *
 *     It maps various flag bits, notably the disk anode does not have a
 *     \cst{EPIA_CFLAGS_BLOCKED} bit, but instead uses the absence of the
 *     \cst{EPIA_CFLAGS_INLINE} and \cst{EPIA_CFLAGS_FRAGMENTED} bits to
 *     indicate blocked.  Here we set the \cst{EPIA_CFLAGS_BLOCKED} bit in this
 *     case unless the file is empty (zero length, or all block addresses
 *     empty) in which case all three bits will be off.  Various functions in
 *     \fname{anode.c} and \fname{strategy.c} use this behavior.
 *
 *     The volume and anode indexes of the backing anode, if any, are obtained.
 *
 * PARAMETERS --
 *     h -- anode handle.
 *     info -- structure containing information about an anode.
 */
long epia_GetInfo(epi_anode_t h, struct epia_info *info)
{
    long code = 0;
    long flags;

    epix_EnterCodeBlock(35);
    afsl_AssertCond(HANDLE_OK(h));

    bzero((caddr_t)info, sizeof (*info)); /* XXX waste of time */
    /* We need to use a write lock here because epix_GetBackingAnode may
     * fill in the backingAnode field. */
    tlock_ObtainWriteNT(&h->lock);
    flags = Disk(h)->flags;
    info->flags = AnodeType(h) >> 8;
    if (flags & AFLAGS_LOGGED)
	info->flags |= EPIA_CFLAGS_LOGGED;
    if (flags & AFLAGS_PREALLOCATED)
	info->flags |= EPIA_CFLAGS_PREALLOCATED;
    if (flags & AFLAGS_UNSAFE)
	info->flags |= EPIA_CFLAGS_UNSAFE;

    if (flags & AFLAGS_DATA_INLINE)
	info->flags |= EPIA_CFLAGS_INLINE;
    else if (flags & AFLAGS_DATA_FRAGMENTED)
	info->flags |= EPIA_CFLAGS_FRAGMENTED;
    else if (!IsAnodeAllEmpty(h))
	info->flags |= EPIA_CFLAGS_BLOCKED;

    info->length = epia_GetLength(h);
    info->allocated = epia_GetAllocated(h);
    info->visible = epia_GetVisible(h);
    info->index = epia_GetIndex(h);
    info->statusSize = epia_GetStatusSize(h);
    info->copies = epia_GetCopies(h);
    if (info->copies)
	info->flags |= EPIA_CFLAGS_BACKING;
    info->backingIndex = 0;
    info->backingVolIndex = 0;
    AFS_hzero(info->volId);
    if (flags & AFLAGS_COPY_ON_WRITE) {
	epi_anode_t backing;
	epix_EnterCodeBlock(36);
	info->flags |= EPIA_CFLAGS_COPYONWRITE;
	code = GetBackingAnode(h, &backing);
	if (code == 0 && backing != NULL) { /* else disk damage */
	    info->backingIndex = epia_GetIndex(backing);
	    info->backingVolIndex = backing->volIndex;
	}
    } else {
	/* This field is not used. */
	/* info->volId = h->volId; */
    }
    tlock_ReleaseWriteNT(&h->lock);
    return code;
}

/*
 * epix_GetFragInfo -- Fills in \arg{desc} to describe the fragment group of
 *     \arg{h}, which must be \cst{EPIA_CFLAGS_FRAGMENTED}.  In particular, the
 *     \fld{block} field of \arg{desc} specifies the address of the block that
 *     is being divided into fragments, \fld{length} is the number of fragments
 *     being used in that block, and \fld{first} is the location of the first
 *     fragment in that block.  The block address may have the COW bit set if
 *     the container is copy-on-write.
 */
long epix_GetFragInfo(epi_anode_t h, epix_fragDescr_t *desc)
{
    long code;
    struct diskAnode *dap;

    epix_EnterCodeBlock(68);
    afsl_AssertCond(HANDLE_OK(h));

    tlock_ObtainRead(&h->lock);

    dap = Disk(h);
    if (dap->flags & AFLAGS_DATA_FRAGMENTED) {
	code = 0;
	*desc = dap->d.frags;
    } else
	code = EPI_E_NOTFRAGMENTEDDATA;

    tlock_ReleaseRead(&h->lock);
    return code;
}

/*
 * epix_ReadInline -- Reads length bytes starting at offset from inline
 *	anode h into buffer.  h must be inline and the requested transfer
 *	must not extend beyond EOF.
 */
long
epix_ReadInline(epi_anode_t h, u_long offset, u_long length, caddr_t buffer)
{
    struct diskAnode *dap;
    long eof;

    afsl_AssertCond(HANDLE_OK(h));
    tlock_ObtainRead(&h->lock);
    dap = Disk(h);
    eof = epia_GetLen32(h);
    afsl_Assert((dap->flags & AFLAGS_DATA_INLINE) && offset + length <= eof);
    bcopy(&dap->d.inlineData[offset], buffer, length);
    tlock_ReleaseRead(&h->lock);
    return 0;
}

/*
 * epix_WriteInline -- Write length bytes starting at offset from buffer
 *	into anode h.  h must be inline and the requested transfer must
 *	not extend beyond EOF, so that no changes outside the user-data
 *	area are necessary.  We update the in-core copy of the anode block
 *	but don't actually schedule the I/O here.
 */
long
epix_WriteInline(
  elbb_tranRec_t trans,
  epi_anode_t h, 
  u_long offset, 
  u_long length, 
  caddr_t buffer)
{
    struct diskAnode *dap;
    struct elbb_buffer *ablk;
    u_long ablkOff;
    long code;

    afsl_AssertCond(HANDLE_OK(h));

    tlock_ObtainWrite(trans, &h->lock);
    dap = Disk(h);

    afsl_Assert((dap->flags & AFLAGS_DATA_INLINE) &&
		offset + length <=  AnodeMaxInlineData(h) && 
		dap->copies == 0);

    code = elbb_Read(h->device, h->block, elbb_bufLogged, &ablk);
    if (code != 0)
	goto done;

    CheckAnodeBlock(ablk, h);	/* sanity check */

    ablkOff = h->offset + ANODE_FO(d.inlineData[0]) + offset;
    code = elbb_Modify(ablk, ablkOff, buffer, length, trans, 1, &ablk);
    afsl_MBZ(code);
    code = elbb_Release(ablk, elbb_lazy);
    afsl_MBZ(code);
    bcopy(buffer, &dap->d.inlineData[offset], length);

done:
    tlock_ReleaseWrite(trans, &h->lock);
    return (code);
}

/*
 * epix_ZeroInline -- Zeroes the specified range in inline files.
 */
long 
epix_ZeroInline(
  elbb_tranRec_t trans,
  epi_anode_t h, 
  u_long offset, 
  u_long length)
{
    struct diskAnode *dap;
    struct elbb_buffer *buf;
    u_long bufOff;
    long code;

    afsl_AssertCond(HANDLE_OK(h));

    tlock_ObtainWrite(trans, &h->lock);
    dap = Disk(h);

    afsl_Assert((dap->flags & AFLAGS_DATA_INLINE) &&
		offset + length <=  AnodeMaxInlineData(h) && 
		dap->copies == 0);

    code = elbb_Read(h->device, h->block, elbb_bufLogged, &buf);
    if (code != 0)
	goto done;

    CheckAnodeBlock(buf, h);	/* sanity check */

    bufOff = h->offset + ANODE_FO(d.inlineData[0]) + offset;
    code = elbb_Zero(buf, bufOff, length, trans, 1, &buf);
    afsl_MBZ(code);
    code = elbb_Release(buf, elbb_lazy);
    afsl_MBZ(code);   
    bzero(&dap->d.inlineData[offset], length);

done:
    tlock_ReleaseWrite(trans, &h->lock);
    return (code);
} 

/*
 * epix_GetInline -- Reads \arg{length} bytes from contents of the inline
 *     container \arg{h} starting at byte \arg{offset}.  The data is copied
 *     into \arg{buffer}. If \arg{iospace} is set to \cst{OSI_UIOSYS} then the
 *     data is transfered using \fcn{bcopy} otherwise \fcn{copyout} is to used.
 */
long epix_GetInline (
  IN epi_anode_t h,
  IN u_int offset,
  IN u_int length,
  IN osi_uio_seg_t iospace,
  OUT char *buffer)
{
    long code = 0;
    struct uio xuio;
    struct iovec xiov;

    epix_EnterCodeBlock(69);
    afsl_AssertCond(HANDLE_OK(h));
    BEGIN_TLOCK_READ (&h->lock) {
	u_int len;
	struct diskAnode *dap = Disk(h);
	if (!(dap->flags & AFLAGS_DATA_INLINE))
	    { ABORT_CRIT (EPI_E_NOTINLINEDATA) }
	afsl_Assert(AFS_hfitsinu32(epia_GetLength(h)));
	len = epia_GetLen32(h);
	afsl_Assert(len <= AnodeMaxInlineData(h));
	if (offset >= len) { ABORT_CRIT(EPI_E_EOF) }
	if (offset + length > len) { ABORT_CRIT(EPI_E_READSHORT) }

	epia_InitUio(&xuio, &xiov, offset, length, buffer);
	xuio.osi_uio_seg = iospace;
	code = osi_uiomove(dap->d.inlineData + offset, length, UIO_READ, &xuio);
    } END_LOCK;
    return code;
}

/*
 * epix_GetMaxInlineData -- Returns the maximum number of bytes
 *     that can be stored in-line in container \arg{h}.
 */
u_int
epix_GetMaxInlineData(epi_anode_t h)
{
    afsl_AssertCond(HANDLE_OK(h));
    return AnodeMaxInlineData(h);
}

/* FormatAnode -- prepares an anode.  Mostly it packs various bits into the
 *     flags word and initializes the block addresses.  It assumes the file is
 *     block allocated. */

static void FormatAnode(
  IN struct diskAnode *a,		/* ptr to anode buffer */
  IN u_long index,
  IN long   flags,
  IN u_int  statusSize)
{
    int i;

    epix_EnterCodeBlock(37);
    bzero((caddr_t)a, ANODE_SIZE);
    afsl_Assert(statusSize <= AFLAGS_STATUSSIZE);
    a->flags = AFLAGS_MAGIC + statusSize + CTOAT(flags & EPIA_CFLAGS_TYPE);
    if (flags & EPIA_CFLAGS_LOGGED) a->flags |= AFLAGS_LOGGED;
    if (flags & EPIA_CFLAGS_PREALLOCATED) a->flags |= AFLAGS_PREALLOCATED;
    a->index = index;
    a->c.backingAnode.block = 0;
    a->c.backingAnode.offset = 0;

    /* initialize block addresses */
    /* initially, assume container is block allocated */
    for (i = 0; i < ANODE_DIRECTBLOCKS; i++)
	a->dBlock[i] = (daddr_t)EPIX_ANODE_BLOCKEMPTY;
    for (i = 0; i < ANODE_INDIRECT_TREES; i++)
	a->iBlock[i] = (daddr_t)EPIX_ANODE_BLOCKEMPTY;
}

/* FormatCopyAnode -- initialized an anode to be a copy on write or backing of
 *     another specified anode.  That other anode it updated as appropriate,
 *     and its contents a copied into the provided anode buffer.  Some fields
 *     are modified as necessary (the copies, flags and, backing fields and the
 *     COW bit of block addresses).
 *
 * PARAMETERS -- flags specifies whether the new anode is to be a copy-on-write
 *     anode or a backing anode.  If the anode is to be backing then, contrary
 *     to the name, backingAnodeC and backingIndex specify the anode which
 *     should become the copy-on-write predecessor of the new anode.  The new
 *     anode is spliced in between the specified anode and its current backing
 *     anode, if any. */

static long FormatCopyAnode(
  IN elbb_tranRec_t trans,
  IN int flags,				/* 0=>cow, 1=>back */
  IN u_long index,			/* index being created */
  IN daddr_t block,			/* new block */
  IN u_long  offset,			/*  and offset */
  IN epi_anode_t backingAnodeC,		/* backpointers for COPYONWRITE */
  IN unsigned long backingIndex,
  OUT struct diskAnode *a)		/* ptr to anode buffer */
{
    long code;
    int i;
    struct async_device *device;
    daddr_t bb;				/* backing anode block */
    u_long bo;				/* backing anode offset */
    int cow = !(flags & 1);		/* new anode is COW. */

    epix_EnterCodeBlock(38);
    device = backingAnodeC->device;
    BEGIN_TLOCK_READ (&backingAnodeC->lock) {
	u_long volIndex = epia_GetIndex(backingAnodeC);
	code = IndexToOffset (backingAnodeC, backingIndex, &bb, &bo);
	if (code) continue;
	BEGIN_LCRIT (epi_anode_t bh,
		     code = Open (device, bb, bo, AFLAGS_TYPE_REGULAR,
				  backingAnodeC, volIndex, &bh),
		     code = epia_Close (bh)) ;
	BEGIN_TLOCK_WRITE (trans, &bh->lock) ;
	BEGIN_LCRIT (struct elbb_buffer *b,
		     code = elbb_Read (device, bb, elbb_bufLogged, &b),
		     code = elbb_Release (b, elbb_lazy)) {
	    CheckAnodeBlock (b, bh);
	    /* mostly new anode is a copy of the backing */
	    bcopy (bh->disk, (caddr_t)a, ANODE_SIZE);
	    if (cow) {
		code = SET_ANODE_FIELD (trans, bh, b,
					copies, Disk(bh)->copies+1);
		afsl_MBZ (code);
		afsl_Assert(Disk(bh)->copies > 0);/* worry about overflow... */
		a->copies = 0;
		a->flags |= AFLAGS_COPY_ON_WRITE;
		a->c.backingAnode.block = bb;
		a->c.backingAnode.offset = bo;
	    } else { /* back */
		struct backPtr backing;
		if (bh->backingAnode) {
		    /* We're going to change the backing address so we need to
		      * flush the cached backing reference, if one exists. */
		    epix_EnterCodeBlock(7);
		    code = epia_Close (bh->backingAnode);
		    if (code) continue;
		    bh->backingAnode = 0;
		}
		backing.block = block;
		backing.offset = offset;
		code = SET_ANODE_FIELD (trans, bh, b, c.backingAnode, backing);
		afsl_MBZ (code);
		if (!(Disk(bh)->flags & AFLAGS_COPY_ON_WRITE)) {
		    /* if it wasn't COW before it must become so */
		    long cowFlags = Disk(bh)->flags | AFLAGS_COPY_ON_WRITE;
		    code = SET_ANODE_FIELD (trans, bh, b, flags, cowFlags);
		    afsl_MBZ (code);
		}
		a->copies = 1;
		/* inherit backingAnode and flags:COW as is. */
	    }
	} END_CRIT;
	END_LOCK;
	END_CRIT;
    } END_LOCK;
    if (code) return code;
    a->index = index;
    a->allocated = 0;
    if (a->flags & AFLAGS_DATA_INLINE) {
	epix_EnterCodeBlock(61);
	/* not really copy on write, but we preserve the anode-to-anode ptr */
    } else if (!cow) {
	/* make it all empty, caller will call reclone later */
	epix_EnterCodeBlock(94);
	a->flags &= ~AFLAGS_DATA_NOTBLOCKED;
	a->diskLength.dh_high = a->diskLength.dh_low = 0;
	a->visible = 0;
	for (i = 0; i < ANODE_DIRECTBLOCKS; i++)
	    a->dBlock[i] = (daddr_t)EPIX_ANODE_BLOCKEMPTY;
	for (i = 0; i < ANODE_INDIRECT_TREES; i++)
	    a->iBlock[i] = (daddr_t)EPIX_ANODE_BLOCKEMPTY;
    } else /* cow */ if (a->flags & AFLAGS_DATA_FRAGMENTED) {
	epix_EnterCodeBlock(71);
	a->d.frags.block |= EPIX_ANODE_BLOCKBACKING;
    } else {
	epix_EnterCodeBlock(72);
	for (i = 0; i < ANODE_DIRECTBLOCKS; i++)
	    a->dBlock[i] |= EPIX_ANODE_BLOCKBACKING;
	for (i = 0; i < ANODE_INDIRECT_TREES; i++)
	    a->iBlock[i] |= EPIX_ANODE_BLOCKBACKING;
    }
    return 0;
}

/* epia_Create -- Creates a new container with the requested properties. If the
 *     create is successful, an open handle for the container created is
 *     returned.  \fcn{epia_Close} must be called on this handle, when finished
 *     using the container.  The size of the status information must be less
 *     than an implementation specific value, currently 150 bytes.
 *
 *     The companion function is \fcn{epia_Delete}.  However, note that if
 *     either \cst{EPIA_CFLAGS_COPYONWRITE} or \cst{EPIA_CFLAGS_BACKING} is
 *     specified, an immediate call to \fcn{epia_Delete} will fail.  In the
 *     case of creating a backing anode this is because the \fld{copies} field
 *     is non-zero (the corresponding COW anode must be deleted or uncloned
 *     first).  In the case of creating a COW anode it will fail if the backing
 *     anode has any allocated or visible size because the container will not
 *     be empty (the container must be truncated to zero length first).
 *
 * PARAMETERS --
 *     trans -- provides the transaction in which this operation occurs.
 *     volC -- fileset or AVL anode table container in which the anode is to be
 *	  created.
 *     flags -- define special features of the new container.  The type of the
 *	  new container must be one of the following values.
 *	      EPIA_CFLAGS_AVL -- this type is illegal here because the AVL
 *		   anode is only created using \fcn{epix_CreateAVL}.
 *	      EPIA_CFLAGS_VOLUME -- a fileset anode table anode.  \arg{volC}
 *		   must be the AVL.
 *	      EPIA_CFLAGS_BITMAP -- the bitmap anode.  \arg{volC} must be the
 *		   AVL.
 *	      EPIA_CFLAGS_LOG -- the log anode.  \arg{volC} must be the AVL.
 *	      EPIA_CFLAGS_BADBLOCKS -- the badblocks anode.  \arg{volC} must
 *		   be the AVL.
 *	      EPIA_CFLAGS_REGULAR -- a file or ACL anode.  \arg{volC} must be
 *		   an anode of type \cst{EPIA_CFLAGS_VOLUME}.
 *	  In addition various bits can be combined with the type value as
 *	  follows to form the flag argument.
 *	      EPIA_CFLAGS_PREALLOCATED -- block allocation fixed during
 *		   aggregate creation.
 *	      EPIA_CFLAGS_LOGGED -- marks the container as \glos{logged},
 *		   meaning that all modifications to its contents will go
 *		   through the log and therefore be recoverable.
 *	      EPIA_CFLAGS_COPYONWRITE -- marks this anode as
 *		   \glos{copy-on-write}.  Its initial contents are identical to
 *		   the contents of the anode specified by
 *		   \argname{backingAnodeC} and \argname{backingIndex}.
 *	      EPIA_CFLAGS_BACKING -- marks this anode as the backing anode of
 *		   an existing anode which will become \glos{copy-on-write}.
 *		   The container will be empty.  The anode it backs is
 *		   specified by \argname{backingAnodeC} and
 *		   \argname{backingIndex}.
 *     statusSize -- size of the status information, in bytes.
 *     estLength -- If bit \cst{EPIA_CFLAGS_ESTLENGTH} is set in \arg{flags},
 *	  then this container will not be larger then \arg{estLength} bytes.
 *	  This feature is presently unused.
 *     volId -- OBS: the fileset identifier.  Valid only if anode is of type
 *	  \cst{EPIA_CFLAGS_VOLUME}.
 *     backingAnodeC -- description of the backing anode if the
 *	  \cst{EPIA_CFLAGS_COPYONWRITE} flag bit is set, or of the
 *	  copy-on-write anode if the \cst{EPIA_CFLAGS_BACKING} flag bit is
 *	  set.  This parameter gives the backing container's fileset table.
 *     backingIndex -- index of the anode in the fileset table specified by
 *	  \arg{backingAnodeC}.
 *     handleP -- pointer to handle for the created anode. */

/* EXPORT  */
long epia_Create(
  IN elbb_tranRec_t trans,
  IN epi_anode_t volC,
  IN unsigned long index,
  IN long flags,
  IN unsigned int statusSize,
  IN unsigned long estLength,
  IN afs_hyper_t volId,			/* obs */
  IN epi_anode_t backingAnodeC,		/* backpointers for COPYONWRITE */
  IN unsigned long backingIndex,
  OUT epi_anode_t *handleP)
{
    long   code;
    struct async_device *device;
    daddr_t block;
    u_long offset;
    char   diskAnode[ANODE_SIZE];	/* buffer for new anode data */
    struct diskAnode *a;		/* ptr to anode buffer */
    int    type;			/* desired type of anode */

    epix_EnterCodeBlock(39);
    if (handleP == 0) return EPI_E_BADARGS;
    *handleP = 0;
    if (!HANDLE_OK(volC)) return EPI_E_BADANODE;

    /* Check for bogus flags */
    type = flags & EPIA_CFLAGS_TYPE;
    if (type == 0) return EPI_E_BADFLAGS;
    if (type == EPIA_CFLAGS_REGULAR) {
	if (flags & EPIA_CFLAGS_PREALLOCATED) return EPI_E_BADFLAGS;
    } else {
	if ((type != EPIA_CFLAGS_LOG) && !(flags & EPIA_CFLAGS_LOGGED))
	    return EPI_E_BADFLAGS;
	if ((flags & (EPIA_CFLAGS_COPYONWRITE|EPIA_CFLAGS_BACKING)) ||
	    backingAnodeC)
	    return EPI_E_COPYONWRITE_ILLEGAL;
    }
    if ((flags & EPIA_CFLAGS_COPYONWRITE) && (flags & EPIA_CFLAGS_BACKING))
	return EPI_E_BADARGS;		/* can't specify both */
    if (((flags & (EPIA_CFLAGS_COPYONWRITE|EPIA_CFLAGS_BACKING)) != 0) !=
	(backingAnodeC != 0))
	return EPI_E_BADARGS;		/* both specified or neither */
    if (statusSize > ANODE_MAX_STATUSSIZE) return EPI_E_STATUSSIZE;

    device = volC->device;
    a = (struct diskAnode *)diskAnode;

    if (!backingAnodeC) {
	FormatAnode (a, index, flags, statusSize);
    } else {
	if (backingAnodeC->device != device) return EPI_E_COPYONWRITE_ILLEGAL;
	if (statusSize) return EPI_E_STATUSSIZE;
	/*  call FormatCopyAnode after we call IndexToOffset */
    }

    BEGIN_TLOCK_READ (&volC->lock) {
	u_long volIndex = epia_GetIndex(volC);
	/* get block and offset early: needed if this is CFLAG_BACKING */
	code = IndexToOffset (volC, index, &block, &offset);
	afsl_MBZ (code);

	if (backingAnodeC) {
	    int cow = ((flags & EPIA_CFLAGS_COPYONWRITE) ? 0 : 1);
	    code = FormatCopyAnode (trans, cow, index, block, offset,
				    backingAnodeC, backingIndex, a);
	    if (code) continue;
	}

	BEGIN_LCRIT(epi_anode_t h,
		    code = Open(device, block, offset, 0, volC, volIndex, &h),
		    if (code && h) {
			long code2;
			code2 = epia_Close(h);
			afsl_MBZ(code2);
		    });
	BEGIN_TLOCK_WRITE (trans, &h->lock) ;
	BEGIN_LCRIT (struct elbb_buffer *b,
		     code = elbb_Read (device, block, elbb_bufLogged, &b),
		     code = elbb_Release (b, elbb_lazy)) {
	    char  *anodeBlock = elbb_BufferData (b);
	    struct diskAnode *olda; /* disk copy of anode */

	    afsl_AssertCond (epic_AnodeBlockOkay
			     (b, ((type == EPIA_CFLAGS_REGULAR) ?
				  ANODE_BLOCK_VOLUME_MAGIC :
				  ANODE_BLOCK_AVL_MAGIC), volIndex));

	    /* Make sure anode doesn't already exist by checking the flags
	     * word.  A deleted anode's flags or those in a freshly formated
	     * anode page should be set accordingly. */
	    olda = (struct diskAnode *)(anodeBlock + offset);
	    if (!DeletedAnode(olda->flags)) {
		epix_EnterCodeBlock(40);
		code = EPI_E_INDEXEXISTS;
		continue;
	    }
	    bcopy((caddr_t)a, h->disk, ANODE_SIZE);
	    DFSH_MemFromDiskHyper(h->length, a->diskLength);
	    /* write out whole anode */
	    code = elbb_Modify (b, offset, (caddr_t)a, ANODE_SIZE,
				trans, !elbb_IsZeroTrans(trans), &b);
	} END_CRIT; /* release buffer */
	END_LOCK;   /* release anode lock */
	if (code) continue;

	/* propagate unlogged-ness */
	if (!epia_DeviceLogged(volC)) h->flags |= EPIA_HFLAGS_DEVICEUNLOGGED;
	else h->flags &= ~EPIA_HFLAGS_DEVICEUNLOGGED;

	*handleP = h;

	IS_TRACE_ANODE (h, printf("Created"));

	END_CRIT;   /* MAYBE close anode if error */
    } END_LOCK; /* release volume lock */
    if (backingAnodeC) {
	afsl_MBZ (code);			/* can't backout anymore */
    }
    return code;
}

/* UnbackAnode -- unlinks an copy on write anode from its backing anode if it
 *     is empty.
 *
 * LOCKS USED -- The call assumes that the copy-on-write anode is already
 *     locked for write and that the backing anode is NOT locked. */

static long UnbackAnode(
  IN elbb_tranRec_t trans,
  IN epi_anode_t cow,
  OUT int *actionP)
{
    long code;
    epi_anode_t backing;
    struct async_device *device = cow->device;

    epix_EnterCodeBlock(42);
    AssertTWriteLocked (&cow->lock);

    if (actionP) *actionP = 0;
    if (!(Disk(cow)->flags & AFLAGS_COPY_ON_WRITE)) return 0;
    if (!IsAnodeAllEmpty (cow)) return EPI_E_NOTEMPTY;

    code = GetBackingAnode (cow, &backing);
    afsl_Assert (!code && backing);

    BEGIN_TLOCK_WRITE (trans, &backing->lock) {
	BEGIN_LCRIT (struct elbb_buffer *b,
		     code = elbb_Read (device, backing->block, elbb_bufLogged,
					&b),
		     code = elbb_Release (b, elbb_lazy)) {
	    CheckAnodeBlock(b, backing);
	    /* cow is empty so just stop pointing to backing. */
	    afsl_Assert(Disk(backing)->copies > 0);
	    code = SET_ANODE_FIELD (trans, backing, b,
				    copies, Disk(backing)->copies-1);
	    afsl_MBZ (code);
	} END_CRIT;
    } END_LOCK;
    if (code) return code;

    if (cow->backingAnode) {		/* we're going to change this... */
	code = epia_Close (cow->backingAnode);
	afsl_MBZ (code);
	cow->backingAnode = 0;
    }
    BEGIN_LCRIT (struct elbb_buffer *b,
		 code = elbb_Read (device, cow->block, elbb_bufLogged, &b),
		 code = elbb_Release (b, elbb_lazy)) {
	int32 flags;
	struct backPtr backPtr;
	backPtr.block = 0, backPtr.offset = 0;
	CheckAnodeBlock (b, cow);
	flags = Disk(cow)->flags & ~AFLAGS_COPY_ON_WRITE;
	code = SET_ANODE_FIELD (trans, cow, b, flags, flags);
	if (code) continue;
	code = SET_ANODE_FIELD (trans, cow, b, c.backingAnode, backPtr);
	if (code) continue;
    } END_CRIT;
    afsl_MBZ (code);
    if (actionP) *actionP = 1;
    return 0;
}

/* UncloneAnode -- is called when the cow anode has been uncloned.  The backing
 *     anode, which must exist, must be empty.
 *
 * LOCKS USED -- The call assumes that the copy-on-write anode is already
 *     locked for write and that the backing anode is NOT locked. */

static long UncloneAnode(
  IN elbb_tranRec_t trans,
  IN epi_anode_t cow)
{
    long code;
    epi_anode_t backing;
    struct async_device *device = cow->device;
    struct backPtr backPtr;

    epix_EnterCodeBlock(43);
    AssertTWriteLocked (&cow->lock);
    if (!(Disk(cow)->flags & AFLAGS_COPY_ON_WRITE))
	return EPI_E_NOTCOPYONWRITE;

    code = GetBackingAnode (cow, &backing);
    afsl_Assert (!code && backing);

    BEGIN_TLOCK_WRITE (trans, &backing->lock) {
	if (!IsAnodeAllEmpty (backing)) { ABORT_CRIT (EPI_E_NOTEMPTY) }
	BEGIN_LCRIT (struct elbb_buffer *b,
		     code = elbb_Read (device, backing->block, elbb_bufLogged,
					&b),
		     code = elbb_Release (b, elbb_lazy)) {
	    /* The backing anode can be unthreaded from the from the chain. */
	    struct backPtr emptyPtr;
	    int32 flags;
	    CheckAnodeBlock (b, backing);

	    /* save this for use by copy-on-write anode */
	    backPtr = Disk(backing)->c.backingAnode;

	    if (backing->backingAnode) { /* eliminating this... */
		code = epia_Close (backing->backingAnode);
		afsl_MBZ (code);
		backing->backingAnode = 0;
	    }
	    emptyPtr.block = 0;
	    emptyPtr.offset = 0;
	    code = SET_ANODE_FIELD (trans, backing, b, c.backingAnode,
				    emptyPtr);
	    afsl_MBZ (code);
	    flags = Disk(backing)->flags & ~AFLAGS_COPY_ON_WRITE;
	    code = SET_ANODE_FIELD (trans, backing, b, flags, flags);
	    afsl_MBZ (code);
	    afsl_Assert(Disk(backing)->copies == 1);
	    code = SET_ANODE_FIELD (trans, backing, b, copies, 0);
	    afsl_MBZ (code);
	} END_CRIT;
    } END_LOCK;
    if (code) return code;

    if (cow->backingAnode) {		/* we're going to change this... */
	code = epia_Close (cow->backingAnode);
	afsl_MBZ (code);
	cow->backingAnode = 0;
    }
    BEGIN_LCRIT (struct elbb_buffer *b,
		 code = elbb_Read (device, cow->block, elbb_bufLogged, &b),
		 code = elbb_Release (b, elbb_lazy)) {
	CheckAnodeBlock (b, cow);
	if ((backPtr.block == 0) && (backPtr.offset == 0)) {
	    int32 flags;
	    epix_EnterCodeBlock(44);
	    flags = Disk(cow)->flags & ~AFLAGS_COPY_ON_WRITE;
	    code = SET_ANODE_FIELD (trans, cow, b, flags, flags);
	    if (code) continue;
	}
	code = SET_ANODE_FIELD (trans, cow, b, c.backingAnode, backPtr);
	if (code) continue;
    } END_CRIT;
    afsl_MBZ (code);
    return 0;
}

/* epia_Delete -- Deletes the anode and closes the handle.  The container must
 *     be zero length, not be opened more than once (i.e. \fld{refCount}
 *     == 1) and not be used by any copy-on-write anodes (i.e.
 *     \fld{copies} == 0).
 *
 * PARAMETERS --
 *     trans -- provides the transaction in which this operation occurs.
 *     anodeC -- the container to be deleted.
 *
 * LOCKS USED -- The lock is held for write since its flags field will be set
 *     to DELETED.  PutHandle is used to remove the handle reference, and it
 *     assumes the write lock is used only for exclusion (which epia_Close
 *     needs).  Since PutHandle doesn't take a transaction, the transaction is
 *     supplied when the lock is obtained.  This is only slightly sub-optimal.
 */

/* EXPORT  */
long epia_Delete(elbb_tranRec_t trans, epi_anode_t anodeC)
{
    long code;
    struct elbb_buffer *b;

    epix_EnterCodeBlock(45);
    if (!HANDLE_OK(anodeC)) return EPI_E_BADANODE;
    BEGIN_CRIT (tlock_ObtainWrite (trans, &anodeC->lock),
		if (code) tlock_ReleaseWriteNT (&anodeC->lock)) {
	int action;
	if (Disk(anodeC)->copies) { ABORT_CRIT (EPI_E_OUTSTANDINGCOPIES) }
	if (Disk(anodeC)->visible) { ABORT_CRIT (EPI_E_NOTEMPTY); }
	if (anodeC->refCount > 1) { ABORT_CRIT (EPI_E_OPEN); }
	code = UnbackAnode (trans, anodeC, &action);
	if (code) continue;
	if (anodeC->backingAnode) {
	    code = epia_Close (anodeC->backingAnode);
	    afsl_MBZ (code);
	    anodeC->backingAnode = 0;
	}
	BEGIN_CRIT (code = elbb_Read (anodeC->device, anodeC->block,
				      elbb_bufLogged, &b),
		    code = elbb_Release (b, elbb_lazy)) {
	    CheckAnodeBlock (b, anodeC);
	    /* just return flags to state they had prior to Create */
	    code = SET_ANODE_FIELD (trans, anodeC, b, flags, ANODE_MAGIC);
	} END_CRIT;
	if (action) { afsl_MBZ (code); }
    } END_CRIT;
    if (code) return code;
    AssertTWriteLocked (&anodeC->lock);

    IS_TRACE_ANODE (anodeC, printf("Deleted"));

    PutHandle (anodeC);
    return 0;
}

/* GetBlockArray -- get snapshot of part of an array of blocks.  Called from
 *     Insert.
 *
 *     The array might be from the anode itself, or from an indirect block.  If
 *     from the anode itself, it might be an array of direct blocks, or an
 *     array of indirect trees.
 */

static long GetBlockArray(
  IN epi_anode_t h,
  IN struct epix_blockLocation *loc,	/* position to start inserting */
  IN int nBlocks,			/* number of blocks in operation */
  OUT int *indexP,			/* index in blockArray */
  OUT daddr_t **blockArrayP,		/* ptr to disk blocks */
  OUT struct elbb_buffer **ibbP)	/* ptr to indirect block buffer */
{
    long code;
    u_int l;				/* index in block array */
    daddr_t ibAddr;			/* i.b. block addr */
    struct elbb_buffer *ibb = 0;	/* indirect block buffer, if nec. */
    u_int ibsize;			/* block ptrs per indirect block */

    epix_EnterCodeBlock(46);
    *ibbP = 0;
    if (nBlocks <= 0) return EPI_E_INSERTERROR;

    if (loc->tree == -1) {		/* direct block */
	epix_EnterCodeBlock(47);
	afsl_Assert(loc->span == 0 && loc->depth == 0 &&
		loc->directBlock >= 0);
	*indexP = l = loc->directBlock;
	*blockArrayP = Disk(h)->dBlock;
	if (nBlocks > ANODE_DIRECTBLOCKS - l) return EPI_E_INSERTERROR;
	return 0;
    }

    afsl_Assert(loc->tree >= 0 && loc->tree < ANODE_INDIRECT_TREES &&
	    loc->base >= ANODE_DIRECTBLOCKS && loc->span > 0);

    if (loc->depth == 0) {		/* top level indirect block */
	epix_EnterCodeBlock(48);
	*indexP = loc->tree;
	*blockArrayP = Disk(h)->iBlock;
	if (nBlocks != 1) return EPI_E_INSERTERROR;
	return 0;
    }

    epix_EnterCodeBlock(49);
    ibAddr = loc->blocks[loc->depth-1];
    l = loc->indexes[loc->depth-1];
    ibsize = IBSIZE(h);
    afsl_Assert(l < ibsize);
    BEGIN_CRIT (code = elbb_Read (h->device, ibAddr, elbb_bufLogged, &ibb),
		if (code) elbb_Release (ibb, elbb_lazy)) {
	struct epia_indirectBlock *ib;
	ib = (struct epia_indirectBlock *) elbb_BufferData (ibb);
	afsl_Assert(IndirectBlockOkay (ib, h,
		  (loc->depth >= 2) ? loc->blocks[loc->depth - 2] : -1,
		 (loc->depth >= 2) ? loc->indexes[loc->depth - 2] : 0,
		 loc->base, loc->span));
	*indexP = l;
	*blockArrayP = ib->blocks;
	if (nBlocks > ibsize-l) { ABORT_CRIT (EPI_E_INSERTERROR) }
    } END_CRIT;
    if (code) return code;
    *ibbP = ibb;
    return 0;
}

static long FormatIndirectBlock(
  IN elbb_tranRec_t trans,
  IN epi_anode_t h,
  IN daddr_t pBlock,
  IN daddr_t oldBlock,
  IN daddr_t parent,			/* parent ib or EPIX_ANODE_BLOCKEMPTY */
  IN u_int index,			/* index in parent */
  IN u_int ibsize,
  IN baddr_t base,
  IN u_long span)
{
    struct async_device *d = h->device;
    long bsize = d->blkSize;
    int log;
    long code;
    struct epia_indirectBlock *ib;
    const int firstPartSize		/* size of ib header */
	= sizeof(*ib)-sizeof(ib->blocks)-sizeof(ib->finalStamp);

    int i;

    epix_EnterCodeBlock(50);
    log = !elbb_IsZeroTrans (trans);

    BEGIN_LCRIT (struct elbb_buffer *ibb,
		 code = elbb_CreateBlock (d, pBlock, elbb_bufLogged, &ibb),
		 code = elbb_Release (ibb, elbb_lazy)) ;

	/*
	 * If oldBlock is a COW block, copy it.  We do this now rather than
	 * later so that it won't overwrite the header and trailer.
	 */
	if (EPIX_IsBlockInBacking (oldBlock)) {
	    daddr_t b = epix_MapBlockToBacking (oldBlock);

	    code = elbb_CopyBlock (ibb, d, b);
	    if (code) continue;
	}

	ib = (struct epia_indirectBlock *) elbb_BufferData (ibb);

	/* Initialize header */

	ib->initialStamp = (int32)h;
	ib->magic = INDIRECT_MAGIC;
	if (parent == EPIX_ANODE_BLOCKEMPTY) {
	    ib->whoPoints.block = h->block;
	    ib->whoPoints.offset = h->offset;
	} else {
	    epix_EnterCodeBlock(52);
	    ib->whoPoints.block = parent;
	    ib->whoPoints.offset = index;
	}
	ib->baseOffset = base;
	ib->span = span;
	ib->ibsize = ibsize;

	IS_TRACE_ANODE (h,
	    {
		printf ("Format IB %d ", pBlock);
		if (EPIX_IsBlockInBacking(oldBlock))
		    printf ("(from %d) ", epix_MapBlockToBacking(oldBlock));
		printf ("(<= %d[%d]) %d+i*%d",
			ib->whoPoints.block, ib->whoPoints.offset,
			ib->baseOffset, ib->span);
	    });

	/* Replicate initialStamp at final stamp */
	*((int32 *) (elbb_BufferData (ibb) + bsize - sizeof (int32))) =
	    ib->initialStamp;

	/* Now fill in the block addresses, either EMPTY or copied from backing
	 * container. */

	if (EPIX_IsBlockInBacking(oldBlock)) {
	    for (i = 0; i < ibsize; i++)
		ib->blocks[i] |= EPIX_ANODE_BLOCKBACKING;
	    code = elbb_Initialize (ibb, 0, bsize, trans, log, &ibb);
	} else {
	    /*
	     * This assertion replaces obsolete code to handle the case
	     * epix_IsBlockViaBacking (oldBlock).
	     */
	    afsl_Assert (epix_IsBlockEmpty (oldBlock));

	    code = elbb_Initialize (ibb, 0, firstPartSize, trans, log, &ibb);
	    if (code) continue;

	    code = elbb_Initialize (ibb, bsize - sizeof (ib->finalStamp),
				      sizeof (ib->finalStamp), trans, log, &ibb);
	    if (code) continue;

	    code = elbb_Fill (ibb,
				offsetof(struct epia_indirectBlock, blocks[0]),
				EPIX_ANODE_BLOCKEMPTY_BYTE,
				ibsize * sizeof (daddr_t), 0,
				trans, log, &ibb);
	}

	if (code) continue;

	afsl_Assert (IndirectBlockOkay (ib, h, parent, index, base, span));

    END_CRIT; /* release ibb */

    return code;
}

/* CountIndirectTree -- increments a count of the number of allocated and
 *     visible *blocks* in the specified indirect tree, not including the
 *     indirect block itself.
 *
 * PARAMETERS -- Since this routine is recursive it only increments the
 *     counters.  Thus the top-level caller is responsible for initializing the
 *     counters to zero before it starts.
 *
 * RETURN CODES -- returns BADINDIRECT instead of asserting so that it can be
 *     used from the salvager. */

static long CountIndirectTree(
  IN epi_anode_t h,
  IN daddr_t block,			/* indirect block */
  IN baddr_t base,
  IN u_long span,
  INOUT u_long *adeltaP,		/* # allocated blocks encountered */
  INOUT u_long *vdeltaP)		/* # visible blocks encountered */
{
    long code;
    if (EPIX_IsBlockInBacking(block))
	adeltaP = 0;			/* don't count more allocated blks */
    if (!adeltaP && !vdeltaP)
	return 0;

    block = epix_MapBlockToBacking(block);
    BEGIN_LCRIT (struct elbb_buffer *ibb,
		 code = elbb_Read (h->device, block, elbb_bufLogged, &ibb),
		 code = elbb_Release (ibb, elbb_lazy)) {
	struct epia_indirectBlock *ib;
	u_int ibsize = IBSIZE(h);
	u_int i;
	baddr_t nextBase;
	u_long nextSpan;

	ib = (struct epia_indirectBlock *) elbb_BufferData (ibb);
	if (!IndirectBlockOkay (ib, h, -2, 0, base, span))
	    { ABORT_CRIT (EPI_E_BADINDIRECT); }

	nextSpan = span / ibsize;
	for (i = 0, nextBase = base; i < ibsize; i++, nextBase += span) {
	    daddr_t b = ib->blocks[i];
	    if (epix_IsBlockEmpty(b)) continue;
	    if (span > 1) {
		code = CountIndirectTree (h, b, nextBase, nextSpan,
					  adeltaP, vdeltaP);
		if (code) break;
	    }
	    if (adeltaP && epix_IsBlockAllocated(b)) (*adeltaP)++;
	    if (vdeltaP) (*vdeltaP)++;
	}
    } END_CRIT;
    return code;
}

/* ApplyAllocationDelta -- adjust allocated and visible fields of anode.
 *     Called from Insert and MoveData. */

static long ApplyAllocationDelta(
  IN elbb_tranRec_t trans,
  IN epi_anode_t handle,
  IN epia_quota_t delta,
  INOUT struct elbb_buffer **bP)
{
    long code;

    if (delta.a) {
	afsl_Assert ((delta.a > 0) || (Disk(handle)->allocated >= -delta.a));
	code = SET_ANODE_FIELD (trans, handle, *bP, allocated,
				Disk(handle)->allocated + delta.a);
	if (code) return code;
    }
    if (delta.v) {
	afsl_Assert ((delta.v > 0) || (Disk(handle)->visible >= -delta.v));
	code = SET_ANODE_FIELD (trans, handle, *bP, visible,
				Disk(handle)->visible + delta.v);
	if (code) return code;
    }

    IS_TRACE_ANODE (handle, printf("Allocated + %d => %d; Visible + %d => %d",
				   delta.a, Disk(handle)->allocated,
				   delta.v, Disk(handle)->visible));

    return 0;
}

#define ERROR_AND_UNCOMMITTED(code, changes) \
    (((changes) == 0 || afsl_MBZ(code)), code)

/* MakeAnodeAllEmpty -- set block addresses to EMPTY and update flags field to
 *     indicate BLOCKED, but don't adjust allocated or visible fields or set
 *     the length. */

static long MakeAnodeAllEmpty(
  IN elbb_tranRec_t trans,
  IN epi_anode_t h,
  INOUT struct elbb_buffer **bufP)
{
    long code;
    struct diskAnode *dk = Disk(h);

    epix_EnterCodeBlock(73);
    AssertTLocked (&h->lock);
    if (dk->flags & AFLAGS_DATA_NOTBLOCKED) {
	daddr_t *da = &dk->dBlock[0];
	daddr_t empties[ANODE_DIRECTBLOCKS + ANODE_INDIRECT_TREES];
	int i;

	/* we assume direct and indirect block addrs are contiguous */
	afsl_Assert(&dk->dBlock[ANODE_DIRECTBLOCKS] == &dk->iBlock[0]);

	code = SET_ANODE_FIELD(trans, h, *bufP, flags,
				dk->flags & ~AFLAGS_DATA_NOTBLOCKED);
	if (code != 0)
	    return code;

	for (i = 0; i < ANODE_DIRECTBLOCKS + ANODE_INDIRECT_TREES; i++)
	    empties[i] = (daddr_t)EPIX_ANODE_BLOCKEMPTY;

	code = elbb_Modify (*bufP, h->offset + (caddr_t)da - (caddr_t)dk,
			    (caddr_t)empties, sizeof (empties), trans,
			    !elbb_IsZeroTrans(trans), bufP);
	afsl_MBZ(code);
	bcopy ((caddr_t)empties, (caddr_t)da, sizeof (empties));
    }
    return 0;
}

/* epix_Insert -- Inserts or removes \arg{nBlocks} physical block addresses
 *     provided in the array \arg{pBlock} into the block map of \arg{handle}.
 *     If \arg{nBlocks} is positive, the blocks are inserted, if negative, they
 *     are removed.  The location in the block map is described by \arg{loc},
 *     which the caller initializes by calling \fcn{epix_FindBlocks}.  The
 *     anode will be converted to \cst{EPIA_CFLAGS_BLOCKED} first, if
 *     necessary.  Block addresses in \arg{pBlock} whose value is
 *     \cst{EPIX_ANODE_BLOCKEMPTY} are skipped.  Other block addresses must
 *     match exactly the address in the corresponding position in the block map
 *     or the block map address should be empty.  For removal, a match is
 *     required.  For insertion, a match is a no-op and empty positions in the
 *     block map will be filled with the corresponding block address from
 *     \arg{pBlock}.
 *
 *     The actual allocation of blocks being inserted or deallocation of blocks
 *     being removed is the responsibility of the caller. */

/* EXPORT  */
long epix_Insert(
  IN elbb_tranRec_t trans,
  IN epi_anode_t handle,
  IN int nBlocks,			/* negative means remove, not insert */
  IN daddr_t pBlock[],			/* physical addresses to insert */
  INOUT struct epix_blockLocation *loc)	/* position to start inserting */
{
    long code;
    struct async_device *device;
    int l;				/* current value of location element */
    daddr_t *blockArray;		/* block array into which l indexes */
    daddr_t savedBlock;			/* block being replaced if IB */

    int i;
    int goodBlocks = 0;
    int log;				/* transaction is valid */
    epia_quota_t delta;			/* adjustment to allocated & visible */
    int fragsPerBlock;
    int mbz = 0;
    int makeempty = 0;
    int insert;				/* TRUE => insert new blocks,
					 * FALSE => remove old blocks */

    epix_EnterCodeBlock(54);
    afsl_AssertCond(HANDLE_OK(handle));
    device = handle->device;

    afsl_Assert(nBlocks != 0 && pBlock != NULL);
    if (nBlocks < 0) {
	insert = 0;
	nBlocks = -nBlocks;
    } else
	insert = 1;
    loc->visibleAdjust = 0;

    IS_TRACE_ANODE (handle,
	{
	    printf ("%s %d blocks (trans %d) at ",
		    (insert ? "Insert" : "Remove"), nBlocks, trans);
	    if (loc->tree == -1)
		printf ("(direct %d)", loc->directBlock);
	    else if (loc->depth == 0) {
		printf ("(tree root %d)", loc->tree);
	    } else {
		int i;
		printf ("(tree %d", loc->tree);
		for (i = 0; i < loc->depth; i++)
		    printf (" %d[%d]", loc->blocks[i], loc->indexes[i]);
		printf (")");
	    }
	});

    BEGIN_TLOCK_WRITE (trans, &handle->lock) {
	if (Disk(handle)->copies)	/* backing anode is read-only */
	    { ABORT_CRIT (EPI_E_OUTSTANDINGCOPIES) }
	BEGIN_LCRIT (struct elbb_buffer *ibb = 0,
		     code = GetBlockArray (handle, loc, nBlocks,
					   &l, &blockArray, &ibb),
		     if (ibb) code = elbb_Release (ibb, elbb_lazy)) {
	    /*
	     * Compare old and new values for various errors, and determine
	     * quota increments.
	     */
	    delta.a = delta.v = 0;
	    fragsPerBlock = device->blkSize / device->fragSize;
	    log = !elbb_IsZeroTrans(trans);

	    if (Disk(handle)->flags & AFLAGS_DATA_NOTBLOCKED) {
		afsl_Assert (MAX(Disk(handle)->allocated,
				 Disk(handle)->visible) < fragsPerBlock);
		delta.a = -Disk(handle)->allocated;
		delta.v = -Disk(handle)->visible;
		makeempty = 1;
	    }

	    for (i = 0; i < nBlocks; i++) {
		daddr_t newb = pBlock[i];
		daddr_t oldb;
		if (epix_IsBlockEmpty(newb)) continue;
		oldb = blockArray[l + i];
		if (insert) {
		    if (makeempty) oldb = (daddr_t)EPIX_ANODE_BLOCKEMPTY;
		    if (newb == oldb) continue; /* no-op */
		    epix_EnterCodeBlock(55);
		    if (EPIX_IsBlockBacking(oldb) &&
			!(Disk(handle)->flags & AFLAGS_COPY_ON_WRITE)) {
			code = EPI_E_BADBACKING;
			if (ERROR_AND_UNCOMMITTED(code, mbz))
			    break;
		    }
		    if (epix_IsBlockAllocated (oldb)) {
			code = EPI_E_BLOCKEXISTS;
			if (ERROR_AND_UNCOMMITTED(code, mbz))
			    break;
		    }
		    if (epix_IsBlockEmpty(oldb))
			delta.v += fragsPerBlock;
		    delta.a += fragsPerBlock;
		} else { /* remove */
		    epix_EnterCodeBlock(57);
		    if (oldb != newb) {
			code = EPI_E_INSERTERROR;
			if (ERROR_AND_UNCOMMITTED(code, mbz))
			    break;
		    }
		    newb = pBlock[i] = (daddr_t)EPIX_ANODE_BLOCKEMPTY;
		    if (epix_IsBlockAllocated(oldb))
			delta.a -= fragsPerBlock;
		    delta.v -= fragsPerBlock;
		}

		IS_TRACE_ANODE (handle, printf ("  [.+%d] %d <= %d",
						i, oldb, newb));

	    }				/* all nBlocks in pBlock */

	    if (EPIX_IsLocNonTerminalIndirect(loc))
		/* blockArray is about to go away */
		savedBlock = blockArray [l];

	} END_CRIT;			/* cleanup ibb */
	if (code)
	    EXIT_CRIT;

	goodBlocks = i;			/* Save count of complete iterations */

	/* indirect block hacking */
	if (EPIX_IsLocNonTerminalIndirect(loc) && goodBlocks > 0) {
	    u_int ibsize = IBSIZE (handle);
	    baddr_t base = loc->base;
	    u_long span = loc->span;

	    afsl_Assert(nBlocks == 1 && goodBlocks == 1);
	    if (loc->depth > 0) {
		span = loc->span / ibsize;
		base += loc->span * l;
	    }

	    if (insert) {

		/* inserting an indirect block */

		epix_EnterCodeBlock(56);
		code = FormatIndirectBlock
		    (trans, handle, pBlock[0], savedBlock,
		     (loc->depth > 0)
		     ? loc->blocks[loc->depth-1]
		     : (daddr_t)EPIX_ANODE_BLOCKEMPTY,
		     l, ibsize, base, span);
		if (code) continue;
	    } else if (EPIX_IsBlockInBacking(savedBlock)) {

		/* Removing a backing reference to an IB.  We must count its
		  * size to adjust the visible field correctly. */

		long bufferReservation = 4; /* 4th lvl ib in a 1k filesys */
		unsigned reservationBlockSize = device->blkSize;
		u_long vdelta = 0;

		elbb_Reserve(bufferReservation, reservationBlockSize);
		code = CountIndirectTree (handle, savedBlock, base, span,
					  0, &vdelta);
		afsl_MBZ (code);
		elbb_Reserve(-(bufferReservation), reservationBlockSize);
		loc->visibleAdjust = vdelta * fragsPerBlock;
		delta.v -= loc->visibleAdjust;
	    }
	}

	/* Update anode */
	BEGIN_LCRIT (struct elbb_buffer *abb,
		     code = elbb_Read (device, handle->block,
					elbb_bufLogged, &abb),
		     code = elbb_Release (abb, elbb_lazy)) {
	    CheckAnodeBlock (abb, handle);

	    if (makeempty) {
		code = MakeAnodeAllEmpty (trans, handle, &abb);
		if (ERROR_AND_UNCOMMITTED(code, mbz++))
		    EXIT_CRIT;
	    }

	    if (!EPIX_IsLocIndirectArray (loc) && goodBlocks > 0) {
		code = SET_ANODE_ADDRESSES(trans, handle, abb,
					   &blockArray[l], pBlock, goodBlocks);
		if (code != 0)
		    EXIT_CRIT;
	    }

	    code = ApplyAllocationDelta (trans, handle, delta, &abb);
	    if (ERROR_AND_UNCOMMITTED(code, mbz++))
		EXIT_CRIT;
	} END_CRIT; /* cleanup abb */
	if (code)
	    EXIT_CRIT;

	if (EPIX_IsLocIndirectArray (loc) && goodBlocks > 0) {
	    BEGIN_LCRIT (struct elbb_buffer *ibb = 0,
			 code = elbb_Read (device, loc->blocks[loc->depth-1],
					   elbb_bufLogged, &ibb),
			 code = elbb_Release (ibb, elbb_lazy)) {
		code = elbb_Modify(ibb, IBOFFSET(l), (caddr_t) pBlock,
				   goodBlocks * sizeof (daddr_t),
				   trans, log, &ibb);
	    } END_CRIT;
	}
    } END_LOCK; /* release handle lock */
    return code;
}

/* Macros that map from the variables affecting the current move state to the
 * operations that are required for each. */
#define MV_BlockType(b) \
    (epix_IsBlockEmpty(b) ? 1 : (epix_IsBlockAllocated(b) ? 2 : 0))
#define MV_StateValue(cb, bb, reclone, span) \
    (((MV_BlockType(cb)*3 + MV_BlockType(bb)) << 2) + \
      ((reclone) ? 0 : 2) + (((span) == 0) ? 1 : 0))

/* These are the operations that can be encoded for any move state.  Each
 * operation is applied to a location in the COW anode's (C) block map and the
 * matching spot in the backing anode (B).  Quota updates, recorded in
 * retdata, are indicated for each as "adj" (move quota), "dec" (reduce
 * quota), or "inc" (increase quota) on "a" (allocated) and/or "v" (visible).
 *
 * There are some implicit orderings between these actions to allow for quiting
 * early, actions marked "(e)", to avoid large transactions.  In particular,
 * recursive operations, are done before updates that affecting the current
 * level.  These operations are applied to indirect block (IB) trees and are
 * marked below with "(r)" */

#define MV_CgetsB     (1<<0)  /* move block from B to C; B <= empty; adj a,v */
#define MV_BgetsC     (1<<1)  /* move block from C to B; C |= backing; adj a */
#define MV_BgetsE     (1<<2)  /* B <= empty; dec v */
#define MV_FreeB      (1<<3)  /* (e) free block from B; dec a */
#define MV_FreeC      (1<<4)  /* (e) free blk of C; C |= backing; dec a; [1] */
#define MV_IncvB      (1<<5)  /* increase quota of B by one block; inc v */
#define MV_ReParentB  (1<<6)  /* adjust parent ptrs in IB in B */
#define MV_ReParentC  (1<<7)  /* adjust parent ptrs in IB in C */
#define MV_FreeBTree  (1<<8)  /* (r,e) free IB tree of B; dec a,v; [2] */
#define MV_DecvBTree  (1<<9)  /* (r) dec visible quota of B's IB tree; adj v */
#define MV_IncBfromC  (1<<10) /* (r) mv C's IB tree quota to B; adj a,v; [3] */
#define MV_MoveCtoB   (1<<11) /* (r) mv C's IB tree quota to B; adj a,v; [4] */
#define MV_MoveBtoC   (1<<12) /* (r) mv B's IB tree quota to C; adj a,v; [5] */
#define MV_Recurse    (1<<13) /* (r,e) recurse on IB trees of both C & B */
#define MV_DecvB      (1<<14) /* UNUSED: decrement quota of B by one block;
				 dec v */
#define MV_RecloneAA  (1<<15) /* (r) reclone (a, a) indirect tree */
#define MV_UncloneBE  (1<<16) /* Indicates (b,e) pair during unclone. Checkout
				* rationale under VALIDATE_OPS */
#define MV_Error      -1
#define MV_NoOp	      0

#define MV_RecursiveOps			/* ops in RecursiveOps proc. */ \
    (MV_FreeBTree + MV_DecvBTree + MV_IncBfromC + MV_MoveCtoB + MV_MoveBtoC + \
     MV_Recurse + MV_RecloneAA)

/* Notes:
 * [1] -- Only used when both COW and backing anodes have allocated indirect
 *     blocks.  Since we recurse first, the backing anode's IB has all the
 *     desired block addresses, so we actually free the IB from the COW anode
 *     and have it point to the backing anode's IB.  The freed IB will contain
 *     only empty or backing block references.
 * [2] -- The block address in B is unchanged.  The allocated and visible quota
 *     adjustment only includes the contents of the tree not the indirect block
 *     itself.  This operation can quit early so quota adjustment must be
 *     maintained correctly throughout.
 * [3] -- quota is moved from C to B where B is assumed to be empty.  Thus B's
 *     quota is only increased and C's allocated quota will decrease but its
 *     visible quota is unchanged (by definition).
 * [4] -- quota is moved from C to B where B is assumed to be backing (i.e.
 *     overwritten in C).  This is like IncBfromC but we must decrement B's
 *     quota (which is all visible, none allocated) by the amount used in its
 *     IB tree.
 * [5] -- This is only used to unclone an IB tree that hasn't been modified
 *     since it was (re)cloned.  B will be emptied so its quota must be reduced
 *     by the ammount that C's quota is increased.  Again, C's visible quota is
 *     unchanged, by definition.
 */

/* This array describes the operations to do for each state.  The state is
 * described by a triplet: (cow block type, backing block type, direction).
 * The types are one of "e" for empty, "b" for backing, and "a" for allocated.
 * The direction is one of "re" or "un" clone.  The first triplet of each pair
 * applies to indirect blocks, the second applies to direct blocks.
 *
 * NOTE: If the primitives used for (a, a, unclone, indirect) or
 * (a, e, reclone, indirect) are changed, re-evaluate the definition
 * of (a, a, reclone, indirect) as implemented inside ProcessITOps().
 */
static int mv_stateOperations[36] = {
    /* b,b,re */ MV_NoOp,
		 MV_NoOp,
    /* b,b,un */ MV_DecvBTree + MV_BgetsE,
		 MV_BgetsE,
    /* b,e,re */ MV_Error,
		 MV_Error,
    /* b,e,un */ MV_NoOp + MV_UncloneBE,
		 MV_NoOp + MV_UncloneBE,
    /* b,a,re */ MV_NoOp,
		 MV_NoOp,
    /* b,a,un */ MV_MoveBtoC + MV_ReParentB + MV_CgetsB,
		 MV_CgetsB,
    /* e,b,re */ MV_DecvBTree + MV_BgetsE,
		 MV_BgetsE,
    /* e,b,un */ MV_DecvBTree + MV_BgetsE,
		 MV_BgetsE,
    /* e,e,re */ MV_NoOp,
		 MV_NoOp,
    /* e,e,un */ MV_NoOp,
		 MV_NoOp,
    /* e,a,re */ MV_FreeBTree + MV_FreeB + MV_BgetsE,
		 MV_FreeB + MV_BgetsE,
    /* e,a,un */ MV_FreeBTree + MV_FreeB + MV_BgetsE,
		 MV_FreeB + MV_BgetsE,
    /* a,b,re */ MV_MoveCtoB + MV_ReParentC + MV_BgetsC,
		 MV_BgetsC,
    /* a,b,un */ MV_DecvBTree + MV_BgetsE,
		 MV_BgetsE,
    /* a,e,re */ MV_IncBfromC + MV_ReParentC + MV_BgetsC + MV_IncvB,
		 MV_BgetsC + MV_IncvB,
    /* a,e,un */ MV_NoOp,
		 MV_NoOp,
    /* a,a,re */ MV_RecloneAA,
		  MV_FreeB + MV_BgetsC,
    /* a,a,un */ MV_Recurse + MV_FreeB + MV_BgetsE,
		 MV_FreeB + MV_BgetsE
		 };

#ifdef AFS_DEBUG
/* SHARED */
long epix_mv_allOps = 0;		/* OR of all used Ops */
/* SHARED */
int epix_mv_allStates[36];	/* times each state is executed */
#endif

int MV_GetOps(
  IN daddr_t cb,
  IN daddr_t bb,
  IN int reclone,
  IN u_long span)
{
    int state = MV_StateValue(cb,bb,reclone,span);
    long ops = mv_stateOperations[state];
#ifdef AFS_DEBUG
    epix_mv_allStates[state]++;
    epix_mv_allOps |= ops;
#endif
    afsl_Assert ((ops != MV_Error) &&
		 (((state >> 2) == 0 /*b,b,X*/) ? (cb == bb) : 1) &&
		 (((state >> 2) == 2 /*b,a,X*/)
		  ? (epix_MapBlockToBacking(cb) == bb) : 1));
    return ops;
}

/*
 * VALIDATE_OPS
 *
 * This macro further validates the operations in the case of the
 * the address in the cow anode being backing and the address in the
 * backing being empty for an unclone operation. It also turns off the
 * MV_UncloneBE bit in the \arg{op}.
 *
 * RATIONALE:
 * During unclone, if cow block is backed by a block in the clone, and
 * that block in the clone is being backed by another clone, i.e.
 * the chain is (b, b, {a | [b ...]}), then in case of a partial unclone
 * due to EPI_E_NOTDONE, we could have (b, e, {a | [b ...]}) for a
 * block that has been processed by earlier partial unclone iterations.
 * However, in the first iteration of an unclone, this is an error.
 *
 * We know that for any blocks processed by immediately previous partial
 * unclone iterations, the current iteration should not have to do any real
 * work (such as freeing of blocks, quota updates i.e. the op should be
 * MV_NoOp for such blocks).
 *
 * Since, we cannot determine whether the current iteration of unclone
 * is the first one, we attempt to simulate that check by asserting that
 * (b, e) pair is only valid for an unclone if no real activity has been
 * done in this iteration yet.
 */

#define QuotasZero(q) ((q).a == 0 && (q).v == 0)

#define VALIDATE_OPS(op, reclone, backAnodeH, retDataP) \
    MACRO_BEGIN \
	 if (!(reclone) && ((op) == (MV_NoOp | MV_UncloneBE))) { \
	     afsl_Assert((Disk(backAnodeH)->flags & AFLAGS_COPY_ON_WRITE) && \
			 ((retDataP)->nFound == 0) &&  \
			 QuotasZero((retDataP)->deltaC) && \
			 QuotasZero((retDataP)->deltaB)); \
	     op &= ~MV_UncloneBE; \
	 } \
    MACRO_END

/* struct mv_modifyParams -- describes information about a block's parent.
 *     There are two cases to consider.  If the block is a direct block or a
 *     top-level indirect block and the parent is the anode.  Otherwise the
 *     block is a non-toplevel indirect block in which case the parent is an
 *     indirect block.  The following table explains how the structure's fields
 *     are interpreted.
 *         field    direct       toplevel indirect   non-toplevel indirect
 *         -----    ------       -----------------   ---------------------
 *	   bP       anode block  anode block	     indirect block
 *         bArray   &dBlock[i]   &iBlock[tree]	     NULL
 *	   ibIndex  N/A		 N/A		     index in ib->blocks[]
 */
struct mv_modifyParams {
    epi_anode_t h;
    daddr_t *bArray;
    u_int ibIndex;
    struct elbb_buffer **bP;
};

static long ReParentIndirect(
  IN elbb_tranRec_t trans,
  IN daddr_t block,			/* indirect block */
  IN struct mv_modifyParams *parent)	/* info about "block"s parent */
{
    long code;
    int log = !elbb_IsZeroTrans(trans);	/* transaction is valid */
    struct backPtr whoPoints;
    afsl_Assert(epix_IsBlockAllocated(block));

    whoPoints.block = (parent->bArray
		       ? parent->h->block : elbb_BufferBlock(*parent->bP));
    whoPoints.offset = (parent->bArray ? parent->h->offset : parent->ibIndex);

    BEGIN_LCRIT (struct elbb_buffer *ibb,
		 code = elbb_Read (parent->h->device, block, elbb_bufLogged,
				   &ibb),
		 code = elbb_Release (ibb, elbb_lazy)) {
	u_int fo = offsetof(struct epia_indirectBlock, whoPoints);

	/* minimal checking is possible */
	afsl_Assert(IndirectBlockOkay ((struct epia_indirectBlock *)
				   elbb_BufferData(ibb),
				   parent->h, -2, 0, 0, 0));

	code = elbb_Modify (ibb, fo, (caddr_t)&whoPoints, sizeof(whoPoints),
			    trans, log, &ibb);
    } END_CRIT;
    return code;
}

/* FreeIndirectTree -- deallocates an indirect block tree.  Both allocated and
 *     visible quota are updated.
 *
 * RETURN CODES -- Can quit early by returning NOTDONE. */

static long FreeIndirectTree(
  IN elbb_tranRec_t trans,
  IN epi_anode_t h,			/* copy on write anode */
  IN daddr_t block,			/* indirect block */
  IN baddr_t base,
  IN u_long span,
  INOUT struct mv_returnData *retData)
{
    long code;
    int log = !elbb_IsZeroTrans(trans);	/* transaction is valid */
    afsl_Assert(epix_IsBlockAllocated(block));

    BEGIN_LCRIT (struct elbb_buffer *ibb,
		 code = elbb_Read (h->device, block, elbb_bufLogged, &ibb),
		 code = elbb_Release (ibb, elbb_lazy)) {
	struct epia_indirectBlock *ib;
	u_int ibsize = IBSIZE(h);
	u_int i;
	baddr_t nextBase;
	u_long nextSpan;
	daddr_t empty = (daddr_t)EPIX_ANODE_BLOCKEMPTY;

	ib = (struct epia_indirectBlock *) elbb_BufferData (ibb);
	afsl_Assert(IndirectBlockOkay (ib, h, -2, 0, base, span));

	nextSpan = span / ibsize;
	for (i = 0, nextBase = base; i < ibsize; i++, nextBase += span) {
	    daddr_t b = ib->blocks[i];

	    if (epix_IsBlockEmpty(b)) continue;

	    if (epix_IsBlockAllocated(b)) {

		/* If this is an IB, recurse to empty it.  Then exit if
		  * transaction is full.  Otherwise, return block. */

		if (span > 1) {
		    code = FreeIndirectTree (trans, h, b, nextBase, nextSpan,
					     retData);
		    if (code) break;
		}
		if (retData->nFound >= retData->nBlocks) {
		    code = EPI_E_NOTDONE;
		    break;
		}
		 retData->pBlock[retData->nFound++] = b;
		 retData->deltaB.a -= retData->fragsPerBlock;
		 retData->deltaB.v -= retData->fragsPerBlock;

	    } else {

		/* Block is a backing reference.  If it's an IB then count its
		  * visible size.  Adjust quota. */

		 u_long delta = 1;

		if (span > 1) {
		    code = CountIndirectTree (h, b, nextBase, nextSpan,
					      0, &delta);
		    afsl_Assert (code != EPI_E_BADINDIRECT);
		    if (code) break;
		}

		retData->deltaB.v -= delta * retData->fragsPerBlock;
	    }

	    /* In either case, set it to <empty>.  [optimize this by doing all
	     * modifies in one IB in one call?] */

	    code = elbb_Modify (ibb, IBOFFSET(i), (caddr_t)&empty,
				sizeof(daddr_t), trans, log, &ibb);
	    if (code) break;
	}
    } END_CRIT;
    return code;
}

static long ModifyBlocks(
  IN elbb_tranRec_t trans,
  IN struct mv_modifyParams *p,
  IN daddr_t new)
{
    long code;
    int log = !elbb_IsZeroTrans(trans);	/* transaction is valid */

    if (p->bArray) {
	/* block is direct or toplevel indirect */
	afsl_Assert(!(Disk((p)->h)->flags & AFLAGS_DATA_NOTBLOCKED));
	code = SET_ANODE_ADDRESS (trans, p->h, *(p->bP), p->bArray, new);
    } else
	/* block non-toplevel indirect */
	code = elbb_Modify (*(p->bP), IBOFFSET(p->ibIndex), (caddr_t)&new,
			    sizeof(daddr_t), trans, log, p->bP);
    return code;
}

/* AfterRecursiveOps -- are performed after all recursive steps.  Only the
 *     FreeB & FreeC operations require quiting early because of insufficient
 *     space to return blocks requiring deallocation.  These operations are
 *     mutually exclusive and must be executed before any other "after
 *     recursive ops".  The other operations modify the parent indirect block
 *     or anode (the *gets* ops) or require information about the parent (the
 *     Reparent ops).  The needed information about the parent is passed in the
 *     modifyParams structures.
 *
 * RETURN CODES:
 *     EPI_E_NOTDONE - the operation has been partially completed and
 *	caller should call down again to complete the operation.
 */

static long AfterRecursiveOps(
  IN elbb_tranRec_t trans,
  IN epi_anode_t h,
  IN epi_anode_t b,
  IN daddr_t *cbP, /* block in cow */
  IN daddr_t *bbP, /* block in backing */
  IN struct elbb_buffer **cbbP,
  IN struct elbb_buffer **bbbP,
  IN long op,  /* operation for this state */
  IN int topLevel, /* 1 = block address inside anode i.e. top level
		      direct block or top level indirect block */
  IN int iBIndex, /* unused for top level direct blocks */
  INOUT struct mv_returnData *retData)
{
    long code;
    struct mv_modifyParams pC, *cow;	/* params for updating COW blocks */
    struct mv_modifyParams pB, *back;	/*   ditto for backing  */
    daddr_t cb, bb;			/* block addresses */

    cow = &pC, back = &pB;
    cb = *cbP, bb = *bbP;

    if (topLevel) {
	pC.h = h, pB.h = b;
	pC.bArray = cbP, pB.bArray = bbP;
	pC.bP = cbbP, pB.bP = bbbP;
    } else {
	pC.h = h, pB.h = b;
	pC.bArray = pB.bArray = 0; /* parent is indirect block */
	pC.ibIndex = pB.ibIndex = iBIndex;
	pC.bP = cbbP, pB.bP = bbbP;
    }

    if (op & MV_FreeB) {
	if (retData->nFound >= retData->nBlocks) return EPI_E_NOTDONE;
	retData->pBlock[retData->nFound++] = bb;
	retData->deltaB.a -= retData->fragsPerBlock;
    }
    if (op & MV_FreeC) { /* and C points to B */
	daddr_t new = bb | EPIX_ANODE_BLOCKBACKING;
	if (retData->nFound >= retData->nBlocks) return EPI_E_NOTDONE;
	retData->pBlock[retData->nFound++] = cb;
	retData->deltaC.a -= retData->fragsPerBlock;
	afsl_Assert (epix_IsBlockAllocated(bb));
	code = ModifyBlocks (trans, cow, new);
	if (code) return code;
    }
    if (op & MV_ReParentC) { /* block from C becomes owned by B */
	code = ReParentIndirect (trans, cb, back);
	if (code) return code;
    }
    if (op & MV_BgetsC) { /* and C becomes backing */
	daddr_t new = cb | EPIX_ANODE_BLOCKBACKING;
	afsl_Assert(epix_IsBlockAllocated(cb));
	code = ModifyBlocks (trans, back, cb);
	if (code) return code;
	code = ModifyBlocks (trans, cow, new);
	if (code) return code;
	retData->deltaB.a += retData->fragsPerBlock;
	retData->deltaC.a -= retData->fragsPerBlock;
    }
    if (op & MV_ReParentB) { /* block from B becomes owned by C */
	code = ReParentIndirect (trans, bb, cow);
	if (code) return code;
    }
    if (op & MV_CgetsB) { /* and B gets empty */
	daddr_t new = (daddr_t)EPIX_ANODE_BLOCKEMPTY;
	afsl_Assert(epix_IsBlockAllocated(bb));
	code = ModifyBlocks (trans, cow, bb);
	if (code) return code;
	code = ModifyBlocks (trans, back, new);
	if (code) return code;
	retData->deltaC.a += retData->fragsPerBlock;
	retData->deltaB.a -= retData->fragsPerBlock;
	retData->deltaB.v -= retData->fragsPerBlock;
    }
    if (op & MV_BgetsE) { /* don't update delta */
	daddr_t new = (daddr_t)EPIX_ANODE_BLOCKEMPTY;
	code = ModifyBlocks (trans, back, new);
	if (code) return code;
	retData->deltaB.v -= retData->fragsPerBlock;
    }
    if (op & MV_IncvB) {
	retData->deltaB.v += retData->fragsPerBlock;
    }
    if (op & MV_DecvB) {
	retData->deltaB.v -= retData->fragsPerBlock;
    }
    return 0;
}

static long ProcessITOps(
  IN elbb_tranRec_t trans,
  IN epi_anode_t h,		/* copy on write anode */
  IN epi_anode_t b,		/* backing anode */
  IN daddr_t *cbP,		 /* address of indirect blk slot in cow */
  IN daddr_t *bbP,		 /* address of indirect blk slot in backing*/
  IN struct elbb_buffer **cbbP,
  IN struct elbb_buffer **bbbP,
  IN long base,
  IN long span,
  IN int reclone,		   /* direction to move blocks */
  IN int iTreeRoot,		 /* root of indirect tree ? */
  IN int iBIndex,		   /* indirect block index in parent */
  INOUT struct mv_returnData *retDataP)
{
    int code = 0;
    int op;

    op = MV_GetOps(*cbP, *bbP, reclone, span);

    VALIDATE_OPS(op, reclone, b, retDataP);

    if (op == MV_NoOp) {
	return 0;
    }

    if (op & MV_RecursiveOps) {
	if (op & MV_RecloneAA) {
	    long rOp;

	    /* Implement reclone of (a,a) indirect blocks as
	     * unclone of (a,a) followed by reclone (a,e).
	     *
	     * Why?
	     * If the source anode of the reclone is backing another anode,
	     * that other anode is not left with a dangling COW indirect
	     * block address. (Sybase defect 4570)
	     */

	    /* MV_StateValue(a, a, unclone, indirect) */
	    rOp = MV_Recurse + MV_FreeB + MV_BgetsE;
	    code = RecursiveOps(trans, h, b, *cbP, *bbP, rOp, base, span,
				0 /* unclone */, retDataP);
	    if (code) return code;
	    code = AfterRecursiveOps(trans, h, b, cbP, bbP, cbbP, bbbP,
				     rOp, iTreeRoot, iBIndex, retDataP);
	    if (code) return code;

	    /* MV_StateValue(a, e, reclone, indirect) */
	    op = MV_IncBfromC + MV_ReParentC  + MV_BgetsC + MV_IncvB;
	    reclone = 1;
	}
	code = RecursiveOps(trans, h, b, *cbP, *bbP, op, base, span,
			    reclone, retDataP);
	if (code) return code;
    }
    code = AfterRecursiveOps(trans, h, b, cbP, bbP, cbbP, bbbP,
			     op, iTreeRoot, iBIndex, retDataP);
    return code;
}

/* RecursiveOps -- are operations on indirect blocks that are recursively
 *     applied to an entire tree of blocks.  This procedure is recursively
 *     called when both trees exist.  Calls to other whole-tree procedures made
 *     instead, when only one tree is valid.  All these operations are mutually
 *     exclusive, only one path is taken.
 *
 * RETURN CODES:
 *     EPI_E_NOTDONE - the operation has been partially completed and
 *	caller should call down again to complete the operation.
 */
static long RecursiveOps(
  IN elbb_tranRec_t trans,
  IN epi_anode_t h,			/* copy on write anode */
  IN epi_anode_t b,			/* backing anode */
  IN daddr_t cb,			/* indirect block in cow */
  IN daddr_t bb,			/* indirect block in backing */
  IN long op,				/* operation for this state */
  IN baddr_t base,
  IN u_long span,
  IN int reclone,			/* direction to move blocks */
  INOUT struct mv_returnData *retData)
{
    long code = 0;
    struct async_device *device = h->device;
    u_int ibsize = IBSIZE(h);
    u_long adelta, vdelta;
    int deconly = 0;

    switch (op & MV_RecursiveOps) {

      case MV_NoOp: return 0;		/* probably redundant */

      case MV_MoveBtoC:
	vdelta = adelta = 0;
	code = CountIndirectTree (b, bb, base, span, &adelta, &vdelta);
	adelta *= retData->fragsPerBlock;
	vdelta *= retData->fragsPerBlock;
	retData->deltaC.a += adelta;
	retData->deltaB.a -= adelta;
	retData->deltaB.v -= vdelta;
	break;

      case MV_DecvBTree:
	deconly = 1;	/* FALLTHROUGH */
      case MV_MoveCtoB:
	/* Decrement visible quota of backing IB tree from B.  Space for the
	 * indirect block itself is not counted. */
	vdelta = adelta = 0;
	code = CountIndirectTree (b, bb, base, span, &adelta, &vdelta);
	afsl_Assert (adelta == 0);	/* should be a backing IB tree */
	vdelta *= retData->fragsPerBlock;
	retData->deltaB.v -= vdelta;
	if (deconly) break;

	/* FALL THROUGH */

      case MV_IncBfromC:
	/* move quota from C To B without regard to contents of B */
	vdelta = adelta = 0;
	code = CountIndirectTree (h, cb, base, span, &adelta, &vdelta);
	adelta *= retData->fragsPerBlock;
	vdelta *= retData->fragsPerBlock;
	retData->deltaB.a += adelta;
	retData->deltaB.v += vdelta;
	retData->deltaC.a -= adelta;
	break;

      case MV_FreeBTree:
	code = FreeIndirectTree (trans, b, bb, base, span, retData);
	break;

      case MV_Recurse:
	BEGIN_LCRIT (struct elbb_buffer *cibb,
		     code = elbb_Read (device, cb, elbb_bufLogged, &cibb),
		     code = elbb_Release (cibb, elbb_lazy)) ;
	BEGIN_LCRIT (struct elbb_buffer *bibb,
		     code = elbb_Read (device, bb, elbb_bufLogged, &bibb),
		     code = elbb_Release (bibb, elbb_lazy)) {
	    struct epia_indirectBlock *cib, *bib;
	    baddr_t nextBase;
	    u_long nextSpan;
	    u_int i;

	    cib = (struct epia_indirectBlock *) elbb_BufferData (cibb);
	    bib = (struct epia_indirectBlock *) elbb_BufferData (bibb);
	    afsl_Assert(IndirectBlockOkay (cib, h, -2, 0, base, span));
	    afsl_Assert(IndirectBlockOkay (bib, b, -2, 0, base, span));

	    nextSpan = span/ibsize;
	    for (i = 0, nextBase = base; i < ibsize; i++, nextBase += span) {
		code = ProcessITOps(trans, h, b,
				    &(cib->blocks[i]), &(bib->blocks[i]),
				    &cibb, &bibb,
				    nextBase, nextSpan, reclone,
				    0 /* not indirect tree root */,
				    i /* index in parent indirect block */,
				    retData);
		if (code) break;
	    }
	} END_CRIT;
	END_CRIT;
	break;

      default:
	afsl_Assert(!op);			/* shouldn't happen */
    }
    return code;
}

#define WillAnodeBeAllEmpty(h, delta) \
    (AreBlockAddrsEmpty(Disk(h)) && ((Disk(h)->allocated + delta.a) == 0) && \
     ((Disk(h)->visible + delta.v) == 0))

/* MoveUnblockedAnodes -- moves data description when block map traversal is
 *     not necessary.  The COW container is always inline or fragmented.  The
 *     backing container may be anything since this routine is called from two
 *     places in MoveAnodes. */

static long MoveUnblockedAnodes(
  IN elbb_tranRec_t trans,
  IN epi_anode_t h,			/* copy on write anode */
  IN epi_anode_t b,			/* backing anode */
  IN struct elbb_buffer **cabbP,	/* anode buffers */
  IN struct elbb_buffer **babbP,
  IN int reclone,			/* direction to move blocks */
  INOUT struct epix_fragDescription *frags, /* fragments to be freed */
  INOUT struct mv_returnData *retData)	/* blocks to be freed; quota */
{
    long code;

    afsl_AssertCond (((Disk(h)->flags&AFLAGS_DATA_NOTBLOCKED) ||
		      WillAnodeBeAllEmpty (h, retData->deltaC)) &&
		     ((Disk(b)->flags&AFLAGS_DATA_NOTBLOCKED) ||
		      WillAnodeBeAllEmpty(b, retData->deltaB)));
    if (reclone) {
	int moveInline, moveFrags;

	moveInline = moveFrags = 0;
	if (Disk(h)->flags & AFLAGS_DATA_INLINE) moveInline = 1;
	else if (Disk(h)->flags & AFLAGS_DATA_FRAGMENTED) {
	    if (EPIX_IsBlockInBacking (Disk(h)->d.frags.block)) {

		/* already recloned so don't do anything. */

		afsl_Assert (epix_MapBlockToBacking(Disk(h)->d.frags.block) ==
			     epix_MapBlockToBacking(Disk(b)->d.frags.block));
	    } else
		moveFrags = 1;
	} else {
	    afsl_Assert (Disk(h)->flags & AFLAGS_DATA_NOTBLOCKED);
	}

	epix_EnterCodeBlock(74);
	if ((moveInline || moveFrags) &&
	    Disk(b)->flags & AFLAGS_DATA_FRAGMENTED) {

	    /* The backing anode will be overwritten so, free its fragment
	     * group, if it has an allocated one. */

	    epix_EnterCodeBlock(75);
	    if (epix_IsBlockAllocated (Disk(b)->d.frags.block)) {
		*frags = Disk(b)->d.frags;
		retData->deltaB.a -= Disk(b)->allocated;
	    } else {
		afsl_Assert (Disk(b)->allocated == 0);
	    }
	    retData->deltaB.v -= Disk(b)->visible;
	} else {
	    /* both inline and empty have zero allocated/visible fields */;
	}

	if (moveInline) {
	    int32 newFlags = ((Disk(b)->flags & ~AFLAGS_DATA_FRAGMENTED) |
			      AFLAGS_DATA_INLINE);
	    u_long anodeBlockOffset = b->offset + ANODE_FO(d.inlineData[0]);
	    char *buffer;
	    u_long length;

	    epix_EnterCodeBlock(76);
	    if (Disk(b)->flags != newFlags) {
		code = SET_ANODE_FIELD (trans, b, *babbP, flags, newFlags);
		if (code) return code;
	    }
	    afsl_Assert(AFS_hfitsinu32 (epia_GetLength(h)));
	    length = epia_GetLen32(h);
	    if (bcmp (&Disk(h)->d.inlineData[0], &Disk(b)->d.inlineData[0],
		      length)) {
		/* bytes are different so move them */
		buffer = &(Disk(h)->d.inlineData[0]);
		code = elbb_Modify (*babbP, anodeBlockOffset, buffer, length,
				    trans, !elbb_IsZeroTrans(trans), babbP);
		afsl_MBZ (code);
		bcopy (buffer, Disk(b)->d.inlineData, length);
	    } /* else bytes are already the same (re-reclone) */

	} else if (moveFrags) {
	    struct epix_fragDescription f;
	    epix_EnterCodeBlock(77);
	    f = Disk(h)->d.frags;
	    afsl_Assert (epix_IsBlockAllocated(f.block));
	    code = SET_ANODE_FIELD (trans, b, *babbP, d.frags, f);
	    if (code) return code;
	    f.block |= EPIX_ANODE_BLOCKBACKING;
	    code = SET_ANODE_FIELD (trans, h, *cabbP, d.frags, f);
	    afsl_MBZ (code);
	    if (!(Disk(b)->flags & AFLAGS_DATA_FRAGMENTED)) {
		int32 newFlags = ((Disk(b)->flags & ~AFLAGS_DATA_INLINE) |
				  AFLAGS_DATA_FRAGMENTED);
		code = SET_ANODE_FIELD (trans, b, *babbP, flags, newFlags);
		afsl_MBZ (code);
	    }
	    retData->deltaC.a -= f.length;
	    retData->deltaB.a += f.length;
	    retData->deltaB.v += f.length;

	}
    } else { /* unclone */

	if ((Disk(h)->flags & AFLAGS_DATA_FRAGMENTED) &&
	    EPIX_IsBlockInBacking (Disk(h)->d.frags.block) &&
	    epix_IsBlockAllocated (Disk(b)->d.frags.block)) {

	    /* The backing anode actually contains the fragments so move
	     * them to the COW anode. */

	    struct epix_fragDescription f;
	    epix_EnterCodeBlock(79);
	    afsl_Assert (((Disk(h)->d.frags.block ^ Disk(b)->d.frags.block)
			  == EPIX_ANODE_BLOCKBACKING) &&
			 (Disk(h)->d.frags.first == Disk(b)->d.frags.first) &&
			 (Disk(h)->d.frags.length == Disk(b)->d.frags.length));
	    f = Disk(b)->d.frags;
	    f.block = epix_MapBlockToBacking(f.block);
	    code = SET_ANODE_FIELD (trans, h, *cabbP, d.frags.block, f.block);
	    if (code) return code;
	    retData->deltaC.a += f.length;
	    retData->deltaB.a -= f.length;
	    retData->deltaB.v -= f.length;

	} else {

	    /* The storage in the backing anode isn't used, so free its
	     * fragment group, if it has one. */

	    epix_EnterCodeBlock(80);
	    if (Disk(b)->flags & AFLAGS_DATA_FRAGMENTED) {
		u_int fraglen = Disk(b)->d.frags.length;
		if (epix_IsBlockAllocated (Disk(b)->d.frags.block)) {
		    *frags = Disk(b)->d.frags;
		    retData->deltaB.a -= fraglen;
		}
		retData->deltaB.v -= fraglen;
	    }
	    /* else if inline every thing is zero */
	    /* else if blocked allocated and visible are not updated yet */
	}
	code = MakeAnodeAllEmpty (trans, b, babbP);
	if (code) return code;
    }
    return 0;
}

/* MoveAnodes -- moves the data description between the two anodes.  Sorts out
 *     the conversion cases between blocked and non-blocked representations.
 *     Update of allocated and visible fields left to caller. */

static long MoveAnodes(
  IN elbb_tranRec_t trans,
  IN epi_anode_t h,			/* copy on write anode */
  IN epi_anode_t b,			/* backing anode */
  IN struct elbb_buffer **cabbP,	/* anode buffers */
  IN struct elbb_buffer **babbP,
  IN int reclone,			/* direction to move blocks */
  INOUT struct epix_fragDescription *frags, /* fragments to be freed */
  INOUT struct mv_returnData *retData)	/* blocks to be freed; quota */
{
    long code;
    int i;
    u_int ibsize;
    int tree;
    baddr_t base;
    u_long span;
    int hBlocked, bBlocked;
    daddr_t empty = (daddr_t)EPIX_ANODE_BLOCKEMPTY;

    epix_EnterCodeBlock(81);
    AssertTWriteLocked (&h->lock);
    AssertTWriteLocked (&b->lock);
    hBlocked = !(Disk(h)->flags & AFLAGS_DATA_NOTBLOCKED);
    bBlocked = !(Disk(b)->flags & AFLAGS_DATA_NOTBLOCKED);

    if (!hBlocked && !bBlocked) {

	/* handle simple cases that don't involve block map traversal. */

	code = MoveUnblockedAnodes (trans, h, b, cabbP, babbP, reclone,
				    frags, retData);
	return code;
    }

    if (!bBlocked) {

	/* In reclone we flush the backing anode's data.  In unclone we can
	 * also flush it because we know that the COW anode's first block has
	 * been modified because it is no longer fragmented, therefore the COW
	 * contents will govern. */

	epix_EnterCodeBlock(82);
	afsl_Assert(hBlocked);
	if (Disk(b)->flags & AFLAGS_DATA_FRAGMENTED) {
	    if (epix_IsBlockAllocated (Disk(b)->d.frags.block)) {
		*frags = Disk(b)->d.frags;
		retData->deltaB.a -= Disk(b)->allocated;
	    } else {
		afsl_Assert (Disk(b)->allocated == 0);
	    }
	    retData->deltaB.v -= Disk(b)->visible;
	}
	code = MakeAnodeAllEmpty (trans, b, babbP);
	if (code) return code;
	bBlocked = 1;
    }
    /* b is blocked at this point */

    /* Push the COW blocks (or emptiness, if non-blocked) over backing */

    for (i = 0; i < ANODE_DIRECTBLOCKS; i++) {
	daddr_t *cbP = (hBlocked ? &(Disk(h)->dBlock[i]) : &empty);
	daddr_t *bbP = &(Disk(b)->dBlock[i]);
	int op;

	op = MV_GetOps(*cbP, *bbP, reclone, 0 /*span*/);

	VALIDATE_OPS(op, reclone, b, retData);

	if (op == MV_NoOp) continue;

	/* span == 0, no recursion here */
	code = AfterRecursiveOps(trans, h, b, cbP, bbP, cabbP, babbP,
				 op,
				 1 /* direct block */,
				 -1 /* don't care value for top level direct
					block */,
				 retData);
	if (code) return code;
    }

    ibsize = IBSIZE(h);
    afsl_Assert(ibsize == IBSIZE(b));
    for (tree = 0, span = 1, base = ANODE_DIRECTBLOCKS;
	 tree < ANODE_INDIRECT_TREES;
	 tree++, span *= ibsize, base += span) {
	daddr_t *cbP = (hBlocked) ? &Disk(h)->iBlock[tree] : &empty;
	daddr_t *bbP = &Disk(b)->iBlock[tree];

	code = ProcessITOps(trans, h, b, cbP, bbP, cabbP, babbP,
			    base, span, reclone,
			    1 /* root of indirect tree */, tree,
			    retData);
	if (code) return code;
    }

    if (hBlocked) return 0;		/* all done */
    /* else bBlocked && !hBlocked */

    /* We've emptied backing because COW was non-blocked.  Now push it. */

    epix_EnterCodeBlock(83);
    afsl_AssertCond(WillAnodeBeAllEmpty(b, retData->deltaB));
    code = MoveUnblockedAnodes (trans, h, b, cabbP, babbP, reclone,
				frags, retData);
    return code;
}

/* epix_MoveData -- Called by \fcn{epia_Clone} during fileset reclone and
 *     unclone operations to move data blocks (or fragments) between \arg{h}
 *     and \arg{b}.  The anode \arg{h} must be COW, and \arg{b} is its
 *     immediately backing anode. Bit \hex{1} of \arg{flags} determines whether
 *     the anode is to be recloned (if set) or uncloned (if reset).  The number
 *     of blocks requiring deallocation is returned in \arg{nFoundP} and their
 *     addresses are copied into \arg{pBlock}.  If a fragment group needs to be
 *     freed their description will be returned in \arg{frags} (its
 *     \fld{length} field will be non-zero).  The caller is responsible for
 *     calling \fcn{epib_Free} or \fcn{epib_FreeFrags}, as appropriate.
 *
 *     This function adjusts the allocated and visible fields of the two anodes
 *     to account for the relative changes in quota usage.  These values are
 *     returned in \arg{deltaC} (for \arg{h}) and in \arg{deltaB} (for
 *     \arg{b}).  The caller must apply these changes to the corresponding
 *     fileset quotas.
 *
 *     This routine must be able to return before it is finished both to keep
 *     transactions short-lived and because only \arg{nBlocks} blocks can be
 *     returned at once.  If the operation can not be completed in a single
 *     call, it returns the \cst{EPI_E_NOTDONE} error code.  In this case, the
 *     caller should free the returned blocks (or fragments), end the current
 *     transaction, start a new one, and call this routine again, repeatedly if
 *     necessary, until the operation is completed.
 *
 * PARAMETERS -- Several of the complex parameters are to allow this procedure
 *     to return information that the caller need to do its job.  Basically it
 *     need the change in the quota of the volumes containing the two anodes
 *     and it need the list of blocks (or fragments) that need to be freed.
 *
 * CAUTIONS -- See the note below about anode state cached by higher levels
 *     being invalid after a reclone.  Also note the potential race condition.
 */

/* SHARED */
long epix_MoveData(
  IN elbb_tranRec_t trans,
  IN epi_anode_t h,			/* copy on write container */
  IN epi_anode_t b,			/* backing container */
  IN long flags,			/* 0x1 => reclone */
  IN u_int nBlocks,			/* size of output block array */
  OUT daddr_t pBlock[],			/* physical block addresses */
  OUT u_int *nFoundP,			/* number addresses returned */
  OUT struct epix_fragDescription *frags, /* fragments to be freed */
  OUT epia_quota_t *deltaCP,		/* quota adjustments for COW */
  OUT epia_quota_t *deltaBP)		/*  ditto  for backing */
{
    long code;
    struct async_device *device;
    int reclone, unclone;		/* type of operation to do.  These are
					     * complements of each other */
    struct mv_returnData retData;
    long bufferReservation = 0;
    unsigned reservationBlockSize;

    epix_EnterCodeBlock(58);
    if (!HANDLE_OK(h) || !HANDLE_OK(b)) return EPI_E_BADANODE;
    if ((device = h->device) != b->device) return EPI_E_BADBACKING;

    afsl_Assert ((nBlocks > 0) && pBlock && nFoundP && deltaCP && deltaCP);

    *nFoundP = 0;
    deltaCP->a = deltaCP->v = 0;
    *deltaBP = *deltaCP;

    frags->block = (daddr_t)EPIX_ANODE_BLOCKEMPTY;
    frags->first = 0;
    frags->length = 0;

    if (flags & ~1) return EPI_E_BADFLAGS;
    reclone = ((flags & 1) != 0);
    unclone = !reclone;

    retData.nBlocks = nBlocks;
    retData.pBlock = pBlock;
    retData.nFound = 0;
    retData.deltaC = *deltaCP;
    retData.deltaB = *deltaBP;
    retData.fragsPerBlock = h->device->blkSize / h->device->fragSize;

    BEGIN_TLOCK_WRITE (trans, &h->lock) {
	{   epi_anode_t backing;
	    code = GetBackingAnode (h, &backing);
	    afsl_Assert (!code && backing);
	    if (b != backing) { ABORT_CRIT (EPI_E_BADBACKING) }
	}
	BEGIN_TLOCK_WRITE (trans, &b->lock) {
	    {   int cow;		/* if h is copy-on-write */
		cow = (Disk(h)->flags & AFLAGS_COPY_ON_WRITE);
		if (unclone && !cow) continue; /* all done */
		if (!cow) { ABORT_CRIT (EPI_E_NOTCOPYONWRITE) }
	    }
	    if (Disk(b)->copies != 1)	/* h must be only copy-on-writer */
		{ ABORT_CRIT (EPI_E_OUTSTANDINGCOPIES) }

	    /* need to reserve a pessimal number of buffers to avoid deadlock.
	     * one obvious improvement to this code is to begin releasing the
	     * reservation when we detect we will no longer need the "pessimal"
	     * case.  To that end, we preserve it in a local variable which
	     * can be decreased and at the end the balance of the reservation
	     * is released. - wam 920727 */
	    bufferReservation = 11;	/* 4th lvl ib in a 1k filesys */
	    reservationBlockSize = h->device->blkSize;
	    elbb_Reserve(bufferReservation, reservationBlockSize);

	    /* add special high-speed short-circuit here */

	    BEGIN_LCRIT (struct elbb_buffer *cabb,
			 code = elbb_Read (device, h->block,
					   elbb_bufLogged, &cabb),
			 code = elbb_Release (cabb, elbb_lazy)) ;
	    BEGIN_LCRIT (struct elbb_buffer *babb,
			 code = elbb_Read (device, b->block,
					   elbb_bufLogged, &babb),
			 code = elbb_Release (babb, elbb_lazy)) {
		int notDone = 0;	/* quit early due to trans size */
		CheckAnodeBlock (cabb, h);
		CheckAnodeBlock (babb, b);

		/* Do the real work! */

		code = MoveAnodes (trans, h, b, &cabb, &babb, reclone, frags,
				   &retData);
		if (code) {
		    if (code != EPI_E_NOTDONE) continue;
		    notDone = 1;
		    code = 0;
		}

		code = ApplyAllocationDelta (trans, h, retData.deltaC, &cabb);
		if (code) continue;
		code = ApplyAllocationDelta (trans, b, retData.deltaB, &babb);
		if (code) continue;

		*nFoundP = retData.nFound;
		*deltaCP = retData.deltaC;
		*deltaBP = retData.deltaB;

		if (notDone) {
		    epix_EnterCodeBlock(59);
		    code = EPI_E_NOTDONE;
		    continue;
		}
		if (reclone) {
		    int ss = Disk(h)->flags & AFLAGS_STATUSSIZE;
		    int anodeOffset = ANODE_SIZE-ss;

		    if (AFS_hcmp (epia_GetLength(h), epia_GetLength(b)) != 0) {
			dfsh_diskHyper_t newLength;
			DFSH_DiskFromMemHyper(newLength, epia_GetLength(h));
			code = SET_ANODE_FIELD (trans, b, babb, diskLength,
						newLength);
			if (code) continue;
		    }
		    if (bcmp ((char *)(h->disk)+anodeOffset,
			      (char *)(b->disk)+anodeOffset, ss) != 0) {
			code = elbb_Modify (babb, (b)->offset + anodeOffset,
					    (char *)(h->disk)+anodeOffset,
					    ss, trans,
					    !elbb_IsZeroTrans(trans), &babb);
			if (code) continue;
			bcopy ((char *)(h->disk)+anodeOffset,
				(char *)(b->disk)+anodeOffset, ss);
		    }
		    /* Since we've just trashed much of the anode we should try
		      * to cleanup the handle and thus flush any cached data. */

		    /* RACE -- this admits a small race condition in which,
		      *     before the caller call close "b" (and thus bring the
		      *     refCount to zero) some other thread opens this file
		      *     and calls epia_GetInfo (which re-establishes the
		      *     backing anode).  If the caller is really locking out
		      *     new references to the backing volume this is not
		      *     likely to be a real possibility. */

		    code = epix_SetGConClose (epix_gc_handle, (opaque)b);
		    afsl_MBZ (code);
		}
	    } END_CRIT;
	    END_CRIT;
	} END_LOCK; /* release b lock */
	if (code) continue;

	/* now that B is unlocked, if we're uncloning try unbacking H */
	if (unclone) {
	    code = UncloneAnode (trans, h);
	    if (code) continue;
	} else /* if (reclone) */ {
	    if (h->backingAnode) {
		code = epia_Close (h->backingAnode);
		afsl_MBZ (code);
		h->backingAnode = 0;
	    }
	}
    } END_LOCK; /* release h lock */
    if (bufferReservation) {
	/* release balance of the buffer reservation */
	afsl_Assert(bufferReservation > 0);
	elbb_Reserve(-(bufferReservation), reservationBlockSize);
    }
    return code;
}

/*
 * epix_InsertFragments -- It is analogous to \fcn{epix_Insert} except it
 *     operates on a fragment group instead of blocks. The \arg{frags}
 *     structure describes new the fragment group to be inserted into \arg{h}.
 *     If the \fld{length} field of \arg{frags} is zero then the anode is made
 *     empty.  Otherwise, the \fld{block} field of \arg{frags} must indicate an
 *     allocated block (not a backing reference to a block) and the anode's
 *     \fld{allocated} and \fld{visible} fields will be set to \fld{length}.
 */
long epix_InsertFragments(
  elbb_tranRec_t trans,
  epi_anode_t h,
  epix_fragDescr_t *frags) /* allocated frags */
{
    long code;
    u_int fragsPerBlock;
    struct diskAnode *dap;
    struct elbb_buffer *abb;
    long newFlags;
    int mbz = 0;

    afsl_AssertCond(HANDLE_OK(h));
    fragsPerBlock = h->device->blkSize >> h->device->logFragSize;
    if ((frags->length > 0 && !epix_IsBlockAllocated(frags->block)) ||
	frags->first + frags->length > fragsPerBlock) {
	return EPI_E_BADARGS;
    }

    tlock_ObtainWriteNT(&h->lock);
    dap = Disk(h);
    if (dap->copies != 0) {
	code = EPI_E_OUTSTANDINGCOPIES;
	goto done;
    }

    if (dap->flags & AFLAGS_DATA_INLINE) {
	epix_EnterCodeBlock(84);
	afsl_Assert(dap->allocated == 0 && dap->visible == 0);
    } else if (dap->flags & AFLAGS_DATA_FRAGMENTED) {
	epix_EnterCodeBlock(85);
	afsl_Assert(dap->allocated ==
		(epix_IsBlockAllocated(dap->d.frags.block) ?
		   dap->d.frags.length : 0) &&
		dap->visible == dap->d.frags.length);
    } else { /* blocked */
	epix_EnterCodeBlock(86);
	if (!IsAnodeAllEmpty(h)) {
	    code = EPI_E_NOTEMPTY;
	    goto done;
	}
	afsl_Assert(dap->allocated == 0 && dap->visible == 0);
    }

    code = elbb_Read(h->device, h->block, elbb_bufLogged, &abb);
    if (code != 0)
	goto done;

    CheckAnodeBlock(abb, h);
    if (frags->length == 0) {
	code = MakeAnodeAllEmpty(trans, h, &abb);
	if (code != 0)
	    goto release;
    } else {
	newFlags = (dap->flags & ~AFLAGS_DATA_INLINE) | AFLAGS_DATA_FRAGMENTED;
	if (newFlags != dap->flags) {
	    code = SET_ANODE_FIELD(trans, h, abb, flags, newFlags);
	    if (ERROR_AND_UNCOMMITTED(code, mbz++))
		goto release;
	}
	code = SET_ANODE_FIELD(trans, h, abb, d.frags, *frags);
	if (ERROR_AND_UNCOMMITTED(code, mbz++))
	    goto release;
    }
    code = SET_ANODE_FIELD(trans, h, abb, allocated, frags->length);
    afsl_MBZ(code);
    code = SET_ANODE_FIELD(trans, h, abb, visible, frags->length);
    afsl_MBZ(code);
release:
    code = elbb_Release(abb, elbb_lazy);
    afsl_MBZ(code);

done:
    tlock_ReleaseWrite(trans, &h->lock);
    return code;
}

/* epix_InsertInline -- Inserts \arg{length} bytes of data from \arg{buffer}
 *     into the container \arg{h} as in-line data starting at position
 *     \arg{offset}.  The anode is converted to \cst{EPIA_CFLAGS_INLINE}, if
 *     necessary.  The anode's \fld{allocated} and \fld{visible} fields will be
 *     set to zero.  If $\arg{offset} + \arg{length}$ is greater than the
 *     anode's current length it will be extended.  This new length must be
 *     less than the maximum inline size reported by
 *     \fcn{epix_GetMaxInlineData}. */

/* SHARED */
long epix_InsertInline(
  IN elbb_tranRec_t trans,
  IN epi_anode_t h,
  IN u_int offset,
  IN u_int length,
  IN char *buffer)
{
    long code;
    int mbz = 0;
    struct diskAnode *dap = Disk(h);

    afsl_AssertCond(HANDLE_OK(h));
    afsl_Assert(offset + length <= AnodeMaxInlineData(h));
    BEGIN_TLOCK_WRITE (trans, &h->lock) {
	if (dap->copies) { ABORT_CRIT(EPI_E_OUTSTANDINGCOPIES) }
	if (dap->flags & AFLAGS_DATA_INLINE) {
	    epix_EnterCodeBlock(87);
	    afsl_Assert(dap->allocated == 0 && dap->visible == 0);
	} else if (dap->flags & AFLAGS_DATA_FRAGMENTED) {
	    epix_EnterCodeBlock(88);
	    afsl_Assert((dap->allocated ==
			 (epix_IsBlockAllocated(dap->d.frags.block)
			   ? dap->d.frags.length : 0)) &&
			dap->visible == dap->d.frags.length);
	} else { /* blocked */
	    epix_EnterCodeBlock(89);
	    if (!IsAnodeAllEmpty (h)) { ABORT_CRIT (EPI_E_NOTEMPTY) }
	    afsl_Assert(dap->allocated == 0 && dap->visible == 0);
	}
	BEGIN_LCRIT (struct elbb_buffer *abb,
		     code = elbb_Read (h->device, h->block, elbb_bufLogged,
					&abb),
		     code = elbb_Release (abb, elbb_lazy)) {
	    long newFlags;
	    u_long anodeBlockOffset;
	    u_int len;

	    CheckAnodeBlock(abb, h);
	    afsl_Assert(AFS_hfitsinu32(epia_GetLength(h)));
	    len = AFS_hgetlo(epia_GetLength(h));
	    newFlags = (dap->flags & ~AFLAGS_DATA_FRAGMENTED) |
			AFLAGS_DATA_INLINE;
	    anodeBlockOffset = h->offset + ANODE_FO(d.inlineData[0]) + offset;

	    if (newFlags != dap->flags) {
		code = SET_ANODE_FIELD (trans, h, abb, flags, newFlags);
		if (ERROR_AND_UNCOMMITTED(code, mbz++))
		    EXIT_CRIT;
	    }
	    code = SET_ANODE_FIELD (trans, h, abb, allocated, 0);
	    if (ERROR_AND_UNCOMMITTED(code, mbz++))
		EXIT_CRIT;
	    code = SET_ANODE_FIELD (trans, h, abb, visible, 0);
	    afsl_MBZ (code);

	    code = elbb_Modify (abb, anodeBlockOffset, buffer, length,
				trans, !elbb_IsZeroTrans(trans), &abb);
	    afsl_MBZ (code);
	    bcopy(buffer, &dap->d.inlineData[offset], length);

	    if (offset + length > len) {
		dfsh_diskHyper_t newLength;
		newLength.dh_high = 0;
		newLength.dh_low = offset + length;
		code = SET_ANODE_FIELD (trans, h, abb, diskLength, newLength);
		afsl_MBZ (code);
		DFSH_MemFromDiskHyper(h->length, dap->diskLength);
	    }
	} END_CRIT;
    } END_LOCK;
    return code;
}

/* epix_SetLength -- Sets the length of \arg{h} to \arg{length}.  If the new
 *     length is incompatible with the storage mode of the container the error
 *     code \cst{EPI_E_BADLENGTH} is returned (e.g.  container is
 *     \cst{EPIA_CFLAGS_INLINE} and \arg{length} is larger than the max size
 *     returned by \fcn{epix_GetMaxInlineData}).
 *
 *     If \arg{max} is non-zero then the anode's length is only changed if
 *     \arg{length} is larger than the current length. */

/* SHARED */
long epix_SetLength(
  IN elbb_tranRec_t trans,
  IN epi_anode_t h,
  IN afs_hyper_t length,			/* desired length of container */
  IN int max)				/* max length into current length */
{
    long code = 0;

    epix_EnterCodeBlock(62);
    afsl_AssertCond(HANDLE_OK(h));
    BEGIN_TLOCK_WRITE (trans, &h->lock) {
	struct diskAnode *dk = Disk(h);
	int c;
	u_long len;

	/* see if there's actually any work to do. */
	c = AFS_hcmp (epia_GetLength(h), length);
	if (c == 0 || (c > 0 && max))
	    EXIT_CRIT;
	if (dk->copies) {
	    ABORT_CRIT (EPI_E_OUTSTANDINGCOPIES)
	}
	len = AFS_hgetlo(length);
	if (dk->flags & AFLAGS_DATA_INLINE) {
	    afsl_Assert(len <= AnodeMaxInlineData(h));
	} else if (dk->flags & AFLAGS_DATA_FRAGMENTED) {
	    afsl_Assert(len <= (dk->d.frags.length << h->device->logFragSize));
	}
	epix_EnterCodeBlock(63);
	BEGIN_LCRIT (struct elbb_buffer *abb,
		code = elbb_Read(h->device, h->block, elbb_bufLogged, &abb),
		code = elbb_Release(abb, elbb_lazy)) {
	    dfsh_diskHyper_t newLength;
	    CheckAnodeBlock(abb, h);

	    DFSH_DiskFromMemHyper(newLength, length);
	    code = SET_ANODE_FIELD(trans, h, abb, diskLength, newLength);
	    if (code)
		EXIT_CRIT;
	    DFSH_MemFromDiskHyper(h->length, dk->diskLength);
	    if ((dk->flags & AFLAGS_DATA_NOTBLOCKED) && AFS_hiszero(length)) {
		afsl_Assert(!dk->allocated && !dk->visible);
		code = MakeAnodeAllEmpty(trans, h, &abb);
		afsl_MBZ (code);
	    }
	    afsl_AssertCond(VerifyNoBlocksAfterEOF(h));
	} END_CRIT;
    } END_LOCK;
    return code;
}

/* epia_SetStatus -- Write into the status data of an anode. Starting at byte
 *     \arg{offset} and continuing for \arg{length} bytes, copy data from
 *     \arg{buffer} into the anode's status area. It is an error if the copy
 *     extends past the end of the status data declared when the anode was
 *     created.
 *
 * PARAMETERS --
 *     trans -- provides the transaction in which this operation occurs.
 *     h -- anode handle.
 *     offset -- offset in the status area to be written.
 *     length -- number of bytes of status to write.
 *     buffer -- pointer to the new status information. */

/* EXPORT  */
long epia_SetStatus(
  IN elbb_tranRec_t trans,
  IN epi_anode_t h,
  IN unsigned int offset,
  IN unsigned int length,
  IN char *buffer)
{
    int ss;
    long code;
    int anodeOffset;		      /* offset of update in anode */
    char *handleCopy;			/* ptr to location in handle */
    int log;

    epix_EnterCodeBlock(64);
    afsl_Assert(length != 0 && buffer != NULL);
    afsl_AssertCond(HANDLE_OK(h));
    ss = Disk(h)->flags & AFLAGS_STATUSSIZE;
    if (offset + length > ss) return EPI_E_TOOBIG;
    log = !elbb_IsZeroTrans(trans);

    anodeOffset = ANODE_SIZE-ss + offset;
    handleCopy = (char *)(h->disk) + anodeOffset;
    BEGIN_TLOCK_WRITE (trans, &h->lock) {
	if (Disk(h)->copies)		/* read-only */
	    { ABORT_CRIT (EPI_E_OUTSTANDINGCOPIES) }
	BEGIN_LCRIT (struct elbb_buffer *abb,
		     code = elbb_Read (h->device, h->block, elbb_bufLogged,
					&abb),
		     code = elbb_Release (abb, elbb_lazy)) {
	    CheckAnodeBlock (abb, h);
	    code = elbb_Modify (abb, (h)->offset + anodeOffset,
				buffer, length, trans, log, &abb);
	    if (code) continue;
	    bcopy (buffer, handleCopy, length);
	} END_CRIT;
    } END_LOCK;
    return code;
}

/* epia_GetStatus -- Get the status data from an open anode. The first
 *     \arg{bufLength} bytes of the anode's status data is returned in the
 *     provided \arg{buffer}. If \arg{statusSizeP} is not zero the actual
 *     length of the status data is returned in it.
 *
 * PARAMETERS --
 *     h -- a container handle.
 *     bufLength -- length of buffer in which to return the status information.
 *     buffer -- buffer into which the status information should be copied.
 *     statusSizeP -- pointer to the length of the status data. */

/* EXPORT  */
long epia_GetStatus(
  IN epi_anode_t h,
  IN unsigned int bufLength,
  OUT char *buffer,
  OUT unsigned int *statusSizeP)
{
    int ss;
    int anodeOffset;			/* offset of update in anode */
    char *handleCopy;			/* ptr to location in handle */

    epix_EnterCodeBlock(65);
    afsl_AssertCond(bufLength == 0 || buffer != NULL);
    afsl_AssertCond(HANDLE_OK(h));
    BEGIN_TLOCK_READ (&h->lock) {
	ss = Disk(h)->flags & AFLAGS_STATUSSIZE;
	if (statusSizeP) *statusSizeP = ss;
	if (bufLength) {
	    anodeOffset = ANODE_SIZE-ss;
	    handleCopy = (char *)(h->disk) + anodeOffset;
	    if (bufLength > ss) bufLength = ss;
	    bcopy (handleCopy, buffer, bufLength);
	}
    } END_LOCK;
    return 0;
}

/* epix_FsyncForce -- Ensures that all modifications to the container
 *     \arg{h} have been committed to the disk.  This exports a persistence
 *     guarantee that allows the caller to know that updates will survive a
 *     system crash and thus can be communicted to outside agents (e.g.
 *     programs running on other machines, or a human with their finger on the
 *     power switch). */

/* SHARED */
long epix_FsyncForce(epi_anode_t h)
{
    long code;
    elbb_tranRec_t trans;

    epix_EnterCodeBlock(91);
    afsl_AssertCond(HANDLE_OK(h));
    tlock_ObtainRead(&h->lock);
    trans = tlock_GetTrans(&h->lock);
    tlock_ReleaseRead(&h->lock);
    /* Must not hold any locks while doing this. */
    code = elbb_LogForce(trans, 1);
    return code;
}

/* epix_CreateAVL -- Create an AVL anode for the aggregate on \arg{device}.
 *     Block \arg{avlBlock} on the \arg{device} is formated as an anode
 *     page with the anode at index \arg{avlIndex} created to describe
 *     the AVL.  This block is inserted at logical address zero in the
 *     container.  Other blocks for the AVL will have to be inserted by the
 *     caller.  A handle for the AVL is returned in \arg{avlP}.  This is called
 *     only by \fcn{epit_CreateAVL}.  On subsequent initializations, when the
 *     aggregate already exists, use \arg{epix_InitAggregate}. */

/* SHARED */
long epix_CreateAVL(
  IN struct async_device *device,
  IN daddr_t avlBlock,
  IN u_long avlIndex,
  OUT epi_anode_t *avlP)
{
    long   code;
    u_long avlOffset;
    struct elbb_buffer *b;
    char   diskAnode[ANODE_SIZE];	/* buffer for new anode data */
    struct diskAnode *a;		/* ptr to anode buffer */
    elbb_tranRec_t trans;
    epi_anode_t h;			/* handle for avl */
    int anodesPerBlock;
    baddr_t block;			/* logical block in avl of avlIndex */
    struct epix_blockLocation loc;	/* where to insert block */

    epix_EnterCodeBlock(66);
    avlOffset = epic_IndexToOffset (avlIndex);
    if (! epic_OffsetOK (avlOffset)) return EPI_E_BADANODEINDEX;
    anodesPerBlock = epic_anodesPerBlock[device->logBlkSize-10];
    block = avlIndex / anodesPerBlock;
    if (block != 0) return EPI_E_BADANODEINDEX;
    trans = elbb_zeroTrans;		/* no transaction */

    if (CheckHandle (device, avlBlock, avlOffset)) {
	/* this code path never tried. -ota 911009 */
	epix_EnterCodeBlock(93);
	return EPI_E_AGGREGATEEXISTS;
    }

    a = (struct diskAnode *)diskAnode;
    bzero((caddr_t)a, ANODE_SIZE);
    FormatAnode
	(a, avlIndex,
	 EPIA_CFLAGS_LOGGED | EPIA_CFLAGS_PREALLOCATED + EPIA_CFLAGS_AVL,
	 /*statusSize*/0);

    BEGIN_CRIT (code = elbb_Read (device, avlBlock, elbb_bufLogged, &b),
		code = elbb_Release (b, elbb_lazy)) {
	code = epic_FormatAnodeBlock (trans, &b, avlIndex,
				      ANODE_BLOCK_AVL_MAGIC);
	if (code) continue;
	code = elbb_Modify (b, avlOffset, (caddr_t)a, ANODE_SIZE,
			    trans, 0, &b);
	if (code) continue;
    } END_CRIT;
    if (code) return code;

    code = Open (device, avlBlock, avlOffset, AFLAGS_TYPE_AVL,
		 NULL, avlIndex, &h);
    if (code) return code;
    h->flags |= EPIA_HFLAGS_DEVICEUNLOGGED;

    EPIX_MakeDirectBlockLoc (&loc, block);
    code = epix_Insert (trans, h, 1, &avlBlock, &loc);
    if (code) return code;

    if (avlP) *avlP = h;
    return 0;
}

/* epix_InitAggregate -- Initializes the aggregate on \arg{device} whose AVL
 *     anode is located in block \arg{avlBlock} at index \arg{avlIndex}.  These
 *     parameters are described is more detail for the \fcn{epix_CreateAVL}
 *     function.  The AVL is opened an its anode handle is returned in
 *     \arg{avlHandleP}.  The \arg{flags} parameters is unused and must be
 *     zero. */

/* SHARED */
long epix_InitAggregate(
  IN struct async_device *device,
  IN long   flags,
  IN daddr_t avlBlock,
  IN u_long avlIndex,
  OUT epi_anode_t *avlHandleP)
{
    long    code;
    u_long avlOffset;
    epi_anode_t h;			/* avl handle */

    epix_EnterCodeBlock(67);
    if (flags) return EPI_E_BADFLAGS;

    /* verify offset before calling Open */
    avlOffset = epic_IndexToOffset (avlIndex);
    code = Open (device, avlBlock, avlOffset, AFLAGS_TYPE_AVL,
		 NULL, avlIndex, &h);
    if (code) return code;
    if (Disk(h)->flags & AFLAGS_COPY_ON_WRITE)
	return EPI_E_COPYONWRITE_ILLEGAL;
    if (h->refCount == 1) h->flags |= EPIA_HFLAGS_DEVICEUNLOGGED;

    *avlHandleP = h;
    return 0;
}

#ifndef KERNEL
#include <verify.h>
#include <salvage.h>

/* We should convert this into a general routine (like MaybePatchBacking) that
 * unconditionally does the appropriate repair during the DUPFIXPASS and is
 * called just before the recursion step (or epiz_MarkUsed) in the block
 * replacement. -ota 930402 */

/*
 * SalvageNewFrags -- obtain a replacement block/frag for "blkno" from the
 *     reservation list (allocated replacements for duplicate blocks/frags).
 *     The replacement block is returned (newDaddr) and the caller must store
 *     it into the anode or indirect block as appropriate.  We duplicate the
 *     contents of "blkno" in "newDaddr".
 *
 *     Some of the complexity of the code is due to FRAGMENTED anodes.  This is
 *     especially true at the elbb_Modify call.
 */

/* SHARED */
static long epiz_SalvageNewFrags(
  IN struct epiz_salvageState *ss,
  IN epi_anode_t h,
  IN daddr_t blkNo,
  OUT daddr_t *newDaddr,
  OUT u_int16 *newOff)
{
    struct elbb_buffer *oldDataBuf, *newDataBuf;
    struct epix_fragDescription frags;	/* from desc */
    struct epix_fragDescription newFrags; /* to (new) desc */
    char * oldData;
    int code = 0;

    afsl_Assert(ss && h && newDaddr != 0);

    /* Make sure we aren't over quota. */
    if (Disk(h)->flags & AFLAGS_DATA_FRAGMENTED) {
	frags = Disk(h)->d.frags;
    } else {
	frags.length = h->device->blkSize / h->device->fragSize;
    }
    /* Comment out since we no longer update the quota. -ota 921130 */
    /* if ((ss->quotaVal + frags.length) > epiz_ssQuotaLimit(ss))
	return (EPI_E_VOLUMEFULL); */
    /*
     * Read a copy of the old block into a buffer, allocate a new block, then
     * return the new block into newDaddr, Special salvager feature: we are
     * copying this data because two entities are not allowed to share it.
     */
    DBprint(( "epiz_SalvageNewFrags: blkNo %x\n", blkNo ));
    BEGIN_CRIT (code = elbb_Read (epia_AnodeDevice(h), blkNo,
				  elbb_bufLogged, &oldDataBuf),
		code = elbb_Release (oldDataBuf, elbb_lazy)) {
	struct epiz_anodeItem * iP;
	oldData = (char *) elbb_BufferData(oldDataBuf);

	/*
	 ** The allocated blocks are found in the reservation list.
	 */
	if (Disk(h)->flags & AFLAGS_DATA_FRAGMENTED) {
	    afsl_Assert(blkNo == Disk(h)->d.frags.block);
	    iP = epiz_FindAnodeItem( ss->reserveFrags,
				    Disk(h)->index, h->volIndex);
	    afsl_Assert (iP && (iP->flags & EPIZ_AI_ALLOC));
	    newFrags.block = iP->aiNewFrag;
	    newFrags.first = iP->aiNewFirst;
	    newFrags.length = frags.length;

	    afsl_Assert ((newFrags.block != frags.block) &&
			 (iP->aiFragLen == frags.length));
	    *newDaddr = newFrags.block;
	    if (newOff) *newOff = newFrags.first;
	    DBprint(( "epiz_SalvageNewFrags: new fragblock %x\n", *newDaddr ));
	} else {
	    newFrags.first = frags.first = 0;

	    /* Allocate a fresh block */
	    iP = epiz_FindResBlock( ss->reserveBlocks,
				   Disk(h)->index, h->volIndex, blkNo);
	    afsl_Assert (iP && (iP->flags & EPIZ_AI_ALLOC));
	    *newDaddr = newFrags.block = iP->aiNewBlock;
	    DBprint(( "epiz_SalvageNewFrags: new block %x\n", *newDaddr ));
	}
	DBprint(( "epiz_SalvageNewFrags: oldblkno code %d\n", code ));

	/* Now make this one a copy of the old one. */
	BEGIN_CRIT (code = elbb_Read (epia_AnodeDevice(h), newFrags.block,
				      elbb_bufLogged, &newDataBuf),
		    code = elbb_Release (newDataBuf, elbb_lazy)) {
	    /* Transfer the data from old to new. */
	    DBprint(( "epiz_SalvageNewFrags: newblkno code %d offset %x from %x length %x\n",
		     code,
		     newFrags.first << h->device->logFragSize,
		     oldData + (frags.first << h->device->logFragSize),
		     frags.length << h->device->logFragSize
		     ));
	    code = elbb_Modify(newDataBuf,
				/* offset in newDataBuf (dest) */
				newFrags.first << h->device->logFragSize,
				/* from (source address), length */
				oldData + (frags.first<<h->device->logFragSize),
				frags.length << h->device->logFragSize,
				elbb_zeroTrans, 0, &newDataBuf);
	    if (code) continue;
	} END_CRIT;
	if (code) continue;
	/* Comment out the following lines since they are causing spurious
	 * quota errors -BWL 10/20/92 */
	/* ss->quotaVal += frags.length; */
	/* ss->quotaAnode += frags.length; */
	iP->flags &= ~EPIZ_AI_ALLOC;	/* used this block */
    } END_CRIT;
    DBprint(( "epiz_SalvageNewFrags: endcrit code %d\n", code ));

    /* XXX
     ** Here we ought to have some code to check for elbb_Read
     ** failure due to 'bad block'.  We ought to still allocate a block
     ** but just zero the new contents.  Alternatively, the upper
     ** layer could notice the error, and zero the block address.
     ** The anode has to be marked as UNSAFE in either case.
     */

    return (code);
}

/*
** epiz_SalvageBlockField
** replace the field passed in, at the offset indicated within
** the block.  note: the absense of locks. note: the routine can
** only modify meta data, since it uses elbb_bufLogged for the reads.
** size may be sizeof(u_int32) or sizeof(u_int16).
*/

/* SHARED */
long epiz_SalvageBlockField(
  IN epi_anode_t h,
  IN daddr_t blkNo,
  IN u_long offset,
  IN long value,
  IN long size)
{
    long code = 0;
    u_int32 longValue;
    u_int16 shortValue;

    if (size == sizeof(shortValue))
	shortValue = (u_int16) value;
    else
	longValue = (u_int32) value;

    BEGIN_LCRIT (struct elbb_buffer *b,
	     code = elbb_Read (h->device, blkNo, elbb_bufLogged, &b),
	     code = elbb_Release (b, elbb_lazy)) {

	code = elbb_Modify(b, offset,
	    (size == sizeof(shortValue)
		? (caddr_t) &shortValue
		: (caddr_t) &longValue),
	    size, elbb_zeroTrans, 0, &b);
    } END_CRIT;
    return (code);
}

/*
** epiz_SalvageAnodeField
** replace the long field passed in, at the offset indicated within
** the anode.  note: the absense of locks. note: the routine can
** only modify meta data, since it uses elbb_bufLogged for the reads.
*/

/* SHARED */
long epiz_SalvageAnodeField(
  IN epi_anode_t h,
  IN u_long offset,
  IN long value)
{
    long code = 0;

    afsl_Assert(h != 0 && Disk(h) != NULL);
    code = epiz_SalvageBlockField(h, h->block, h->offset + offset, value,
	sizeof (long));
    if (code) {
	/* UNSAFE, but we may not be able to mark as such */
    } else {
	if (Disk(h)) {
	    *(long *) (((char *)(Disk(h))) + offset) = value;
	}
    }
    return (code);
}

/*
** epiz_SalvageAnodeShortField
** replace the short field passed in, at the offset indicated within
** the anode.  note: the absense of locks. note: the routine can
** only modify meta data, since it uses elbb_bufLogged for the reads.
*/

/* SHARED */
long epiz_SalvageAnodeShortField(
  IN epi_anode_t h,
  IN u_long offset,
  IN u_int16 value)
{
    long code = 0;

    afsl_Assert(h != 0 && Disk(h) != NULL);
    code = epiz_SalvageBlockField( h, h->block, h->offset + offset,
	(long) value, sizeof(u_int16) );
    if (code) {
	/* UNSAFE, but we may not be able to mark as such */
    } else {
	if (Disk(h)) {
	    *(u_int16 *) (((char *)(Disk(h))) + offset) = value;
	}
    }
    return (code);
}

static long SalvageAnodeLength(
  IN epi_anode_t File,
  IN afs_hyper_t length)
{
    long code;
    dfsh_diskHyper_t newLength;

    DFSH_DiskFromMemHyper(newLength, length);
    code = epiz_SalvageAnodeField(File, ANODE_FO(diskLength.dh_high),
				  newLength.dh_high);
    if (code) return code;
    code = epiz_SalvageAnodeField(File, ANODE_FO(diskLength.dh_low),
				  newLength.dh_low);
    File->length = length;
    return code;
}

static long SalvageMakeAnodeAllEmpty(IN epi_anode_t h)
{
    long code;
    afs_hyper_t zero;

    if (Disk(h)->flags & AFLAGS_DATA_NOTBLOCKED) {
	code = epiz_SalvageAnodeField
	    (h, ANODE_FO(flags), Disk(h)->flags & ~AFLAGS_DATA_NOTBLOCKED);
	if (code) return code;
    }

    {   /* we assume direct and indirect block addrs are contiguous */
	u_long diskoff;			/* offset of block addrs */
	int i;

	afsl_Assert(&Disk(h)->dBlock[ANODE_DIRECTBLOCKS] == &Disk(h)->iBlock[0]);
	diskoff = ANODE_FO (dBlock[0]);
	for (i = 0; i < ANODE_DIRECTBLOCKS + ANODE_INDIRECT_TREES; i++) {
	    code = epiz_SalvageAnodeField
		(h, diskoff + i * sizeof (daddr_t), EPIX_ANODE_BLOCKEMPTY);
	    if (code) return code;
	}
    }
    AFS_hzero (zero);
    code = SalvageAnodeLength (h, zero);
    return code;
}

/*
** epiz_MarkUnsafeAnode
** put on the bits.
*/

/* SHARED */
long epiz_MarkUnsafeAnode(
  IN epi_anode_t h,
  IN struct epiz_anodeItem ** headP,
  IN struct epiz_salvageState * ss)
{
    int code = 0;
    afsl_Assert(h != 0 && Disk(h) != NULL);
    afsl_Assert(headP != NULL);

    /* add the entry to the list FIRST.
    ** Even if SalvageAnode failed, we would want to record this anode.
    */
    (void) epiz_AllocAnodeItem( headP, Disk(h)->index, h->volIndex);
    if (ss->flags & EPIZ_SS_SALVAGE) {
	if (Disk(h)->flags & AFLAGS_UNSAFE)
	    DBprint(( "epiz_MarkUnsafeAnode: %x already marked\n", h ));
	else {
	    DBprint(( "epiz_MarkUnsafeAnode: flags %x\n", Disk(h)->flags ));
	    code = epiz_SalvageAnodeField (h, ANODE_FO(flags),
					   Disk(h)->flags | AFLAGS_UNSAFE);
	    DBprint(( "epiz_MarkUnsafeAnode: flags %x\n", Disk(h)->flags ));
	}
    }
    return (code);
}

/* B L O C K   M A P   V E R I F I C A T I O N
 *
 * Block map verification has several components each of which must be
 * performed in the proper order.  Basically there are two kinds of checks:
 * those that require updates to global salvager state and those that don't.
 * If a repair is performed after a state change has already been made then are
 * resalvage will be necessary.
 *
 * There are several types of state modifications --
 *     bitmap -- allocated blocks and fragments are marked in the bitmap.
 *     logged map -- user-data and metadata fragments are marked in the logged
 *	  map.
 *     duplicates -- duplicate blocks and fragments are tracked on several
 *	  lists.
 *     quota -- allocated and visible storage is counted for the current anode
 *	  and volume.
 *
 * As luck would have it, all the state modifying steps either do not generate
 * errors or generate duplication errors which are fixed on the second pass.
 * Recursive steps involving indirect blocks may modify state depending on how
 * far they get.  This leads to the following sequence --
 *     block validation -- namely, CheckBlock and CheckEOF.
 *     COW blockmap similarity -- are COW and backing block maps consistent.
 *     recurse -- check subsidiary indirect blocks using ScanIndirect or
 *	  CountIndirectTree.
 *     update state -- update above mentioned state info.
 *     duplicate resolution -- (if DUPFIXPASS) replace duplicate blocks.
 */

/* epiz_CheckBlock -- Checks that the specified block is good to use.  It makes
 *     two checks, returning an appropriate reason (for passing to
 *     epiz_ReportError) or zero.
 *
 * RETURN CODES --
 *     {BLOCK,FRAG}_BEYONDDEV -- if the block is outside the bitmap.
 *     {BLOCK,FRAG}_ILLEGALCOWREF -- if the COW bit is on when if can't be. */

static long epiz_CheckBlock(
  IN struct epiz_salvageState *ss,
  IN daddr_t blkno)
{
    int fragd = (ss->loc.tree == -2);
    int cowWhy = (fragd ? EPIZ_E_FRAG_ILLEGALCOWREF
		  : EPIZ_E_BLOCK_ILLEGALCOWREF);
    int devWhy = (fragd ? EPIZ_E_FRAG_BEYONDDEV : EPIZ_E_BLOCK_BEYONDDEV);

    if (epix_IsBlockEmpty(blkno)) return 0;
    if (EPIX_IsBlockInBacking(blkno)) {
	if (ss->flags & EPIZ_SS_NOCOWREFS)
	    return cowWhy;
	blkno = epix_MapBlockToBacking(blkno);
    }
    if ((blkno > ss->maxBlock) || (blkno < ss->minBlock))
	return devWhy;
    return 0;
}

/* epiz_CheckEOF -- Checks that the logical block location is within the EOF.
 *
 * RETURN CODES --
 *     BLOCK_BEYONDLEN -- if the block reference is past EOF. */

static long epiz_CheckEOF(
  IN struct epiz_salvageState *ss,
  IN epi_anode_t File,
  IN baddr_t relblk)
{
    afs_hyper_t relbyte;

    afsl_Assert (!(Disk(File)->flags & AFLAGS_DATA_FRAGMENTED));

    AFS_hset64 (relbyte, 0, relblk);
    AFS_hleftshift(relbyte, ss->device->logBlkSize);
    if (AFS_hcmp(relbyte, epia_GetLength(File)) >= 0)
	return EPIZ_E_BLOCK_BEYONDLEN;
    return 0;
}

/*
 * The following routines are used by the anode block scanning code to either
 * mark, or 'find anode usage of' fragments, direct or indirect blocks.  The
 * routines are called indirectly through a pointer in ss.
 */

/* MarkQuota -- do quota update part of block map verification. */

static void MarkQuota(
  IN struct epiz_salvageState *ss,
  IN epi_anode_t File,
  IN daddr_t blkno)
{
    u_long numFrags;

    if (!File ||			/* ignore quota in BadBlocks */
	epix_IsBlockEmpty(blkno))	/* and empty blocks contribute nil */
	return;

    if (Disk(File)->flags & AFLAGS_DATA_FRAGMENTED)
	numFrags = Disk(File)->d.frags.length;
    else
	numFrags = ss->simBits->fragsPerBlock;

    ss->quotaVal.v += numFrags;
    ss->quotaAnode.v += numFrags;
    if (!EPIX_IsBlockInBacking(blkno)) {
	ss->quotaVal.a += numFrags;
	ss->quotaAnode.a += numFrags;
    }
}

/* epiz_MarkNoop -- used in blockRef on COW pass(es). */

/* SHARED */
u_long epiz_MarkNoop(
  IN daddr_t blkno,
  IN baddr_t relblk,
  IN epi_anode_t File,
  IN struct epiz_salvageState *ss,
  IN long height)
{
    MarkQuota (ss, File, blkno);
    return 0;
}

static u_char markBits[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

/* Move duplicate block repair (during DUPFIXPASS) to separate function.
 * (SalvageNewFrags) This routine should never return DUPBLKUSE.  But only
 * update the dup db on SCANPASS. -ota 930402 */

/*
 * epiz_MarkUsed -- Marks block blkno as used in the bitmap (of ss->verify),
 *     and update the fragment usage block map.  This is the point at which we
 *     detect duplicate fragment references (or user/meta data conflicts in
 *     fragmented anodes).  In this case we record the offending fragments and
 *     associated anodes in a database for doing repairs on the DUPFIXPASS.
 *
 *     Update the quota usage for both the anode and containing volume.
 *
 *     We also read the contents of the bad blocks container (via ScanBBBlock).
 *     height indicates what meta-level of block this is; although ScanBBBlock
 *     lies since the contents of the bad block list is meta information, but
 *     isn't indirect blocks.
 *
 * CAUTIONS -- this routine will not be called to handle INLINE anodes.
 *
 * RETURN CODES -- returns EPI_E_DUPBLKUSE on second and subsequent references
 *     to any fragment.  This is true even during the second pass (DUPFIXPASS)
 *     when it could return this error for the first reference as well.  These
 *     first references are detected so that the anode can be marked as
 *     dubious, but it is not repaired so we don't return a non-zero code.
 */

/* SHARED */
u_long epiz_MarkUsed(
  IN daddr_t blkno,
  IN baddr_t relblk,	/* in the case of FRAG's, this is relFragNo */
  IN epi_anode_t File,
  IN struct epiz_salvageState *ss,
  IN long height)
{
    long code = 0;
    struct epiz_SimBits *verif = ss->simBits;
    long BitNo, ByteNo;
    u_long fragNo;
    u_long fragOff;
    u_int numFrags;
    u_int i;
    afs_hyper_t fragAddr;
    u_long hi = 0, lo = 0;

    if (epix_IsBlockEmpty(blkno)) return 0;
    if (EPIX_IsBlockInBacking(blkno)) {
	afsl_Assert (File);		/* BadBlocks are never "InBacking" */
	if (ss->flags & EPIZ_SS_NOCOWREFS) return EPI_E_BADBACKING;
    }

    MarkQuota (ss, File, blkno);

    if (epix_IsBlockEmpty(blkno) ||
	EPIX_IsBlockInBacking(blkno))
	return 0;

    afsl_Assert ((epiz_CheckBlock(ss, blkno) == 0) &&
		 epix_IsBlockAllocated(blkno));

    /* Check that the block being by a fragmented file is used consistently as
     * either logged or unlogged. */

    if ((File != NULL) && Disk(File)->flags & AFLAGS_DATA_FRAGMENTED) {
	u_long blkByte = (blkno - verif->bitmapOffset) >> 3;
	unsigned char blkMask = 1 << ((blkno - verif->bitmapOffset) & 0x7);
	int logged = ((Disk(File)->flags & AFLAGS_LOGGED) != 0);

	if (!(verif->fraged[blkByte] & blkMask)) {
	    /* We've never encountered a fragmented anode referencing this
	     * block before, so set the fraged bit and initialized the
	     * corresponding bit in the logged map. */
	    verif->fraged[blkByte] |= blkMask;
	    if (logged) verif->logged[blkByte] |= blkMask;
	} else {
	    /* Else we've seen this block ref'd before, so insist that the
	     * state is the same. */
	    if (!(verif->logged[blkByte] & blkMask) != !logged) {

		/* If it isn't the same treat this as a duplicate block.  On
		  * the first pass add all the frags in the block to the dup
		  * block list so we'll report all occurances.  For all
		  * references that don't match return DUPBLKUSE so they get
		  * reallocated.  Note that we give preference to the first
		  * reference; this is somewhat arbitrary. */

		if (ss->pass == EPIZ_SS_SCANPASS) {
		    int i;
		    for (i = 0; i < verif->fragsPerBlock; i++)
			epiz_AddDupBlock (blkno, i, ss);
		}
		code = EPI_E_DUPBLKUSE;

		/* Record anodes of frags in this conflict. */
		if ((ss->pass == EPIZ_SS_DUPFIXPASS) &&
		    ((code == EPI_E_DUPBLKUSE) ||
		     (epiz_IsDupBlock (blkno, 0, ss))))
		    (void) epiz_AllocAnodeItem (&ss->dupAnodeList,
				Disk(File)->index, File->volIndex);
		numFrags = Disk(File)->d.frags.length;
		goto quit;
	    }
	}
    }

    /* Calc vbls for fragment loop */

    fragNo = (blkno - verif->bitmapOffset) * verif->fragsPerBlock;
    if ((File != NULL) && Disk(File)->flags & AFLAGS_DATA_FRAGMENTED) {
	fragOff = Disk(File)->d.frags.first;
	numFrags = Disk(File)->d.frags.length;
    } else {
	fragOff = 0;
	numFrags = verif->fragsPerBlock;
    }

    /* note: "code" gets promoted in this loop, from zero, to some value.  Take
     * care over setting of code, since the loop can iterate a number of times.
     */

    /* note: we collect information about blocks which need to be reallocated.
     * Afterward, we determine what to do about the blocks... */

    for (i = 0, fragNo += fragOff ;
	 i < numFrags;
	 i++, ++fragNo, ++fragOff) {

	BitNo = fragNo & 0x7;
	ByteNo = fragNo >> 3;

	/* This check is actually done by epiz_CheckBlock above. */
	afsl_Assert (ByteNo < verif->bitsLen);

	if ((verif->bits[ByteNo] & markBits[BitNo]) != 0) {
	    if (ss->pass == EPIZ_SS_SCANPASS)
		epiz_AddDupBlock( blkno, fragOff, ss );
	    code = EPI_E_DUPBLKUSE;
	} else {
	    verif->bits[ByteNo] |= markBits[BitNo];
	}

	/* Record anodes of multiply referenced frags.  Either we noticed that
	 * the frag is already marked (second and subsequent references) or the
	 * frag is present in the DupBlock database. */

	/* The usefulness of the this list seems limited.  All these anodes are
	 * going to be put on the mayberestored list anyway and this list
	 * should not substitute for calling epiz_ReportError on the problem
	 * frags anyway.  In which case epiz_ReportErro should manage the list.
	 * -ota 930402 */

	/* This || condition seems unnecessary.  On pass 2 all frags should be
	 * in the dup db. -ota 930402 */

	if ((ss->pass == EPIZ_SS_DUPFIXPASS) &&
	    ((code == EPI_E_DUPBLKUSE) ||
	     (epiz_IsDupBlock (blkno, fragOff, ss)))) {
	    (void) epiz_AllocAnodeItem (&ss->dupAnodeList, Disk(File)->index,
					File->volIndex);
	}
    }

  quit:
    /*
     * Before we (early) return, we need to record what space to reserve, which
     * we will allocate between passes, at a high level.  We decide, because we
     * can do it in the first pass, to move second and subsequent duplicate
     * references.  This gives preference to the anode we encounter first.
     * While this may be arbitrary, it does mean that things like the bitmap
     * get priority.
     */
    if (code) {
	if ((code == EPI_E_DUPBLKUSE) && (ss->pass == EPIZ_SS_SCANPASS)) {
	    u_long lclCode; /* don't disturb code */
	    afsl_Assert(File != 0);
	    if (Disk(File)->flags & AFLAGS_DATA_FRAGMENTED) {
		/*
		** Because there is no array pointer for frags addreses
		** (ie the fragments are all contiguous), we have to
		** set up to reallocate the ENTIRE fragment.
		*/
		lclCode = epiz_AddReserveFrags( File,
			Disk(File)->d.frags.length, ss );
	    } else {
		/* Reserve a extra block for this file. */
		lclCode = epiz_AddReserveBlock( File, blkno, ss );
	    }
	    afsl_Assert(lclCode == 0);
	}
	return (code);
    }

#if 0
    /*
    ** We have the opportunity to scan the contents of the
    ** block.  Since we know the height (meta level) we're at,
    ** we can scrape bits off of the surface, and hand them to
    ** some processing code...
    */

    /* This should be handled completely separately.  It is ridiculously
     * confusing to fold this in with checking anode block/frag usage.
     * -ota 930402 */

    if (File == NULL) {
	/* Can't determine file type */
    } else if ((height == 0) &&
		(epia_GetType(File) == EPIA_CFLAGS_BADBLOCKS)) {
	/*
	** Scan the contents of the Bad Block file
	** note: ScanBBBlock will use MarkUsed to mark the blocks
	** referenced by the bad block list.  In that case, the
	** parameters to MarkUsed will be height of 1, File of NULL.
	*/
	code = ScanBBBlock(blkno, File, ss, relblk);
    }
#endif

    return (code);
}

/* MaybePatchBacking -- Clears the COW bit on a block address if we're trying
 *     to do the unclone due to backing problems.
 *
 * PARAMETERS -- The thisBlockP pointer must address the cached block address
 *     (e.g. the appropriate anode direct block) or a local variable if the
 *     address isn't cached (e.g. an indirect block).  It is updated with new
 *     block address (sans COW bit).
 *
 *     If bP is specified, offset if where on the page the block address
 *     resides.  If bP is null, we assume the block address is part of an anode
 *     so the offset is from the base of the anode.  We obtain the anode handle
 *     from ss->anode. */

static void MaybePatchBacking(
  IN struct epiz_salvageState *ss,
  IN daddr_t *thisBlockP,
  IN struct elbb_buffer **bP,
  IN long offset)
{
    long code;
    daddr_t newBlock;			/* actual block address */
    int release;			/* use local buffer */
    struct elbb_buffer *b;		/* local buffer pointer */

    if (!(ss->flags & EPIZ_SS_REPAIRCOWREFS) ||
	!(ss->flags & EPIZ_SS_SALVAGE) ||
	epix_IsBlockEmpty(*thisBlockP)) return;

    newBlock = epix_MapBlockToBacking(*thisBlockP);
    if (newBlock == *thisBlockP) return;

    epiz_ReportError (ss, 0, EPIZ_E_PATCHBACKING, newBlock);
    *thisBlockP = newBlock;

    if (!bP) {
	bP = &b;
	code = elbb_Read(ss->anode->device, ss->anode->block,
			 elbb_bufLogged, &b);
	afsl_MBZ (code);
	offset += ss->anode->offset;
	release=1;
    } else release = 0;			/* use callers buffer */

    code = elbb_Modify (*bP, offset, (const caddr_t)thisBlockP,
			sizeof(*thisBlockP),
			elbb_zeroTrans, 0, bP);
    if (release) code = elbb_Release(b, elbb_lazy);
    afsl_MBZ (code);
}

/* DupIndirectBlock -- handles the case where we need to modify an indirect
 *     block, but because we're in the DUPFIXPASS and the indirect block is
 *     duplicately referenced, we need to copy it first.
 *
 * RETURN CODES -- always returns EPI_E_DUPBLKUSE since the caller (of
 *     ScanIndirect) must update its reference to the new block. */

static long DupIndirectBlock(
  IN struct epiz_salvageState *ss,
  IN epi_anode_t File,			/* Addr belongs to epi_anode_t File */
  INOUT daddr_t *Addr)
{
    long code;
    daddr_t newBlock;

    code = epiz_SalvageNewFrags( ss, File, *Addr, &newBlock, (u_int16 *) 0 );
    afsl_MBZ (code);
    *Addr = newBlock;
    return EPI_E_DUPBLKUSE;
}

#define	BACKINGDADDR(x)	((x) | EPIX_ANODE_BLOCKBACKING)

/*
 * ScanIndirect -- Recursive descent to verify contents of the different
 *     indirect block pointers. File could be a cow anode.  If it is, Back is
 *     an epi_anode_t for the backing anode.
 *
 * PARAMETERS -- The block (*Addr) must be allocated.  Indirect blocks are only
 *     scanned when verifing the anode to which they actually belong.
 *
 *     In ss->flags the NOCOWREFS bit means that COW block references
 *     are errors.  When you find them remove them and mark the anode as
 *     unsafe.
 *
 * RETURN CODES -- This routine SHOULD BE careful to return only these codes:
 *     EPI_E_BADINDIRECT -- any miscellaneous problem
 *     EPI_E_DUPBLKUSE -- in case IB was duplicated
 */

static long ScanIndirect(
  INOUT daddr_t *Addr,
  IN epi_anode_t File,			/* Addr belong to epi_anode_t File */
  IN daddr_t backAddr,			/* corresp. block in backing anode */
  IN struct epiz_salvageState *ss,
  IN long Height,
  IN u_long StartLen,
  IN daddr_t bno,	/* block number of 'backreference'(whoPoints) */
  IN u_long off,	/* offset of 'backreference'(whoPoints) */
  IN char *Text)
{
    char DescBuff[500];
    long code;
    int why;
    long lclCode;			/* code that must be zero */
    u_long SkipHere;
    u_long ibsize;
    long X2;
    int modCopy = 0;
    long modCode = 0;			/* code to return if caller should
					 * update his IB. */
    int depth;				/* local copy of depth of traversal */

    afsl_Assert (epix_IsBlockAllocated(*Addr));
    afsl_Assert((Disk(File)->flags & AFLAGS_COPY_ON_WRITE) ||
		epix_IsBlockEmpty(backAddr));

    ss->loc.index = StartLen;
    ss->loc.block = *Addr;
    if (ss->pass == EPIZ_SS_SCANPASS) ss->nIndirectBlocks++;

    /* TODO -- I don't understand why we don't just copy it here
     *     unconditionally.  We've determined that it is a dup and if we don't
     *     allocate the new block then we won't remove the duplication.  It
     *     seems like this would lead to blocks being left on the reserveBlock
     *     list (we see this) and failed verification (we also see this but not
     *     complaining about duplication the second time).
     *
     *     Part of the problem here is that we are using the IsDupBlock test
     *     instead of using epiz_FindResBlock.  The IsDupBlock will return true
     *     on the every reference including the first.  But the first one isn't
     *     the one we want to replace. */

    /* If we need to modify the indirectBlock, sometimes we first need to make
     * a copy first. */
    if (ss->pass == EPIZ_SS_DUPFIXPASS) {
	/* IsDupBlock will only really report correctly after an inital pass */
	code = epiz_IsDupBlock( *Addr, 0, ss );
	if (code == EPI_E_DUPBLKUSE) {
	    modCopy = 1;		/* yes, a copy must be made first. */
	}
	code = 0;
    }

    ibsize = IBSIZE(File);
    SkipHere = 1;
    for (X2 = 0; X2 < Height; ++X2) SkipHere *= ibsize;

    DBprint(( "ScanIndirect: Block %x logged %d Height %d modCopy %d\n", *Addr,
	     elbb_bufLogged, Height, modCopy ));
    BEGIN_LCRIT (struct elbb_buffer *ibb,
		 code = elbb_Read(File->device, *Addr, elbb_bufLogged, &ibb),
		 code = elbb_Release(ibb, elbb_lazy)) ;

    BEGIN_LCRIT (struct elbb_buffer *bibb,
		 bibb = 0,
		 if (bibb) code = elbb_Release(bibb, elbb_lazy)) {
	struct epia_indirectBlock *ib;
	struct epia_indirectBlock *bib;

	ib = (struct epia_indirectBlock *) elbb_BufferData(ibb);
	if (!IndirectBlockOkay(ib, File, bno, off, StartLen, SkipHere)) {
	    DBprint(( "Height %x StartLen %x Span %x\n",
		     Height, StartLen, ib->span ));
	    code = EPI_E_BADINDIRECT;
	    continue;
	}

	/* If we are checking the backing indirect block it will be found
	 * in bib.  If it is null there is nothing to check. */

	bib = 0;
	if (!epix_IsBlockEmpty (backAddr)) {
	    code = elbb_Read(File->device, backAddr, elbb_bufLogged,
			     &bibb);
	    if (code) continue;
	    bib = (struct epia_indirectBlock *) elbb_BufferData(bibb);

	    /* We check the backing block as a indirect block.  If it isn't
	     * any good we assume it will be fixed later.  Here we assume
	     * it will be freed and treat it as EMPTY.
	     *
	     * Using File here isn't right, we should use Back, but we only
	     * use it to get the blkSize. */

	    if (!IndirectBlockOkay(ib, File, bno, off, StartLen, SkipHere)) {
		bib = 0;		/* do no checking */
	    }
	}

	depth = ++ss->loc.depth;

	for (X2 = 0; X2 < ibsize; ++X2, StartLen += SkipHere) {
	    long offset;
	    daddr_t thisBlock = ib->blocks[X2];
	    daddr_t backBlock;

	    if (epix_IsBlockEmpty(thisBlock)) continue;

	    backBlock = (bib ? bib->blocks[X2] : EPIX_ANODE_BLOCKEMPTY);

	    ss->loc.index = StartLen;
	    ss->loc.indexes[ss->loc.depth-1] = X2;
	    ss->loc.blocks[ss->loc.depth] = thisBlock;
	    ss->loc.indexes[ss->loc.depth] = -1;
	    ss->loc.block = thisBlock;

	    sprintf(DescBuff, "lvl-%d blk %lu(%lu)=%#lx of %s",
		    Height, X2, ibsize, thisBlock, Text);

	    offset = offsetof(struct epia_indirectBlock, blocks[X2]);
	    MaybePatchBacking (ss, &thisBlock, &ibb, offset);
	    ss->loc.blocks[ss->loc.depth] = thisBlock; /* in case it changes */
	    ss->loc.block = thisBlock;

	    /* Do some preliminary checking, then call ScanIndirect or
	     * CountIndirectTree, as appropriate. */

	    why = epiz_CheckBlock (ss, thisBlock);
	    if (!why) why = epiz_CheckEOF (ss, File, StartLen);
	    if (!why && bib && EPIX_IsBlockInBacking(thisBlock)) {
		if (thisBlock != BACKINGDADDR(backBlock)) {
		    epiz_ReportError (ss, 0, EPIZ_E_COW_BADBLOCK, backBlock);
		    why = EPIZ_E_NOOP;
		}
	    }
	    if (why) {
		epiz_ReportError (ss, 0, why);
		if (ss->flags & EPIZ_SS_SALVAGE) {
		    if (modCopy) {
			modCode = DupIndirectBlock (ss, File, Addr);
			modCopy = 0;
		    }
		    thisBlock = (daddr_t)EPIX_ANODE_BLOCKEMPTY;
		    code = elbb_Modify(ibb, offset,
					(caddr_t)&thisBlock, sizeof(thisBlock),
					elbb_zeroTrans, 0, &ibb);
		    afsl_MBZ (code);
		}
		continue;		/* skip to next X2 */
	    }

	    /*
	     * If this indirect block contains indirect blocks then before we
	     * can MarkUsed them, we need to validate all their children.  We
	     * don't want the block marked used if we don't believe we can
	     * really use it.
	     */
	    if ((Height > 0) && epix_IsBlockAllocated(thisBlock)) {
		code = ScanIndirect( &thisBlock, File, backBlock, ss,
				    Height-1, StartLen, *Addr, X2, DescBuff);

		ss->loc.depth = depth;	/* reset traversal state */
		ss->loc.indexes[ss->loc.depth] = -1;
		ss->loc.block = thisBlock;

		if (code == EPI_E_BADINDIRECT) {
		    /*
		    ** The reference will be removed.
		    ** If the indirect block we want to modify
		    ** is a duplicate block, then we need to
		    ** make a copy of IT before we make any
		    ** changes.
		    */
		    epiz_ReportError (ss, 0, EPIZ_E_BADIB);
		    if (ss->flags & EPIZ_SS_SALVAGE) {
			if (modCopy) {
			    modCode = DupIndirectBlock (ss, File, Addr);
			    modCopy = 0;
			}
			thisBlock = (daddr_t)EPIX_ANODE_BLOCKEMPTY;
			lclCode = elbb_Modify( ibb, offset,
					      (caddr_t)&thisBlock, sizeof(thisBlock),
					      elbb_zeroTrans, 0, &ibb );
			afsl_MBZ (lclCode);
		    }
		    code = 0;
		} else if (code == EPI_E_DUPBLKUSE) {
		    /* ib->block[X2] had to be duplicated to
		    ** salvage the deeper indirectBlock.
		    ** Value of ib->block[X2] (in ibb buffer)
		    ** needs to be modified.
		    */
		    printf( "Repairing Duplicate block use at %s\n", Text );
		    if (ss->flags & EPIZ_SS_SALVAGE) {
			if (modCopy) {
			    modCode = DupIndirectBlock (ss, File, Addr);
			    modCopy = 0;
			}
			lclCode = elbb_Modify( ibb, offset,
					      (caddr_t)&thisBlock, sizeof(thisBlock),
					      elbb_zeroTrans, 0, &ibb );
			afsl_MBZ (lclCode);
		    }
		    code = 0;
		}
		afsl_MBZ (code);
		code = 0;
	    } else if ((Height > 0) && EPIX_IsBlockInBacking(thisBlock)) {

		/* This indirect block is in backing so just count its visible
		 * size. */

		u_long visible = 0;
		code = CountIndirectTree (File, thisBlock, StartLen,
					  SkipHere/ibsize,
					  0, &visible);
		if (code) {
		    if (code == EPI_E_BADINDIRECT) code = 0;
		    epiz_ReportError (ss, code, EPIZ_E_COW_BADIB);
		    if (ss->flags & EPIZ_SS_SALVAGE) {
			if (modCopy) {
			    modCode = DupIndirectBlock (ss, File, Addr);
			    modCopy = 0;
			}
			thisBlock = (daddr_t)EPIX_ANODE_BLOCKEMPTY;
			code = elbb_Modify(ibb, offset, (caddr_t)&thisBlock,
					   sizeof(thisBlock), elbb_zeroTrans,
					   0, &ibb);
			afsl_MBZ (code);
		    }
		    code = 0;
		    continue;
		}
		visible *= ss->simBits->fragsPerBlock;
		ss->quotaVal.v += visible;
		ss->quotaAnode.v += visible;
	    }

	    /*
	     * Call MarkUsed/SaveAnodeRef
	     */
	    code = (*ss->blockRef) (thisBlock, StartLen, File, ss, Height);

	    if (code == EPI_E_DUPBLKUSE) {
		/* if its pass 2, (try to fix the problem)
		 ** move the reference to a new block */
		if (ss->pass == EPIZ_SS_DUPFIXPASS) {
		    daddr_t newBlock;
		    struct epiz_anodeItem ** HeadP = &ss->mayRestore;

		    printf( "Repairing duplicate block reference at %s, index %d\n",
			   Text, X2 );

		    code = epiz_SalvageNewFrags (ss, File, thisBlock,
						 &newBlock, (u_int16 *) 0 );
		    DBprint(( "ScanIndirect: %d epiz_SalvageNewFrags code %x\n",
			     __LINE__, code ));
		    if (code) {
			newBlock = (daddr_t)EPIX_ANODE_BLOCKEMPTY;
			HeadP = &ss->oughtRestore;
		    }
		    code = epiz_MarkUnsafeAnode( File, HeadP, ss );
		    afsl_MBZ(code);
		    if (modCopy) {
			modCode = DupIndirectBlock (ss, File, Addr);
			modCopy = 0;
		    }
		    /* update value in the block */
		    code = elbb_Modify(ibb, offset,
					(caddr_t)&newBlock, sizeof(newBlock),
					elbb_zeroTrans, 0, &ibb);
		    afsl_MBZ (code);	/* XXX do more here? */
		}
		code = 0;
	    }
	    afsl_MBZ (code);
	}				/* for all indirect block addresses */
    } END_CRIT;				/* release bib */
    END_CRIT;				/* release ib */
    if (code == EPI_E_BADINDIRECT)
	return code;
    if (code) {
	epiz_ReportError (ss, code, EPIZ_E_BADIB);
	return EPI_E_BADINDIRECT;
    }
    if (code == 0)
	code = modCode;			/* maybe return EPI_E_DUPBLKUSE */
    return code;
}

/*
 * VerifyNoBlocksAfterEOF -- check that there are no allocated
 * blocks or fragments at offsets beyond the end of the container.
 */
static int VerifyNoBlocksAfterEOF(IN epi_anode_t h)
{
    struct diskAnode *dk = Disk(h);
    baddr_t lastb;
    u_int log_bsize;
    u_long len, nSkip;
    struct epix_blockLocation loc;
    long code;
    daddr_t db;
    int nfind;

    len = epia_GetLen32(h);
    switch (dk->flags & AFLAGS_DATA_NOTBLOCKED) {
      case 0:	/* Blocked file */
	log_bsize = epia_AnodeDevice(h)->logBlkSize;
	lastb = (len == 0) ? 0 : (len - 1) >> log_bsize;
	nfind = 0;
	/*
	 * Iterate until FindBlocks reports no more allocated
	 * blocks (lastb + nSkip == 0 || lastb == 0 && nSkip == -1),
	 * or until we find an allocated block past EOF.  If nSkip
	 * is zero, meaning that we have hit an empty indirect
	 * block, advance to the following indirect block.
	 */
	do {
	    code = FindBlocks(h, lastb, 1L, 0, &db, (int *)NULL,
		&nSkip, &loc);
	    afsl_MBZ(code);
	    nfind++;
	    if (lastb != 0 || nSkip != (u_long)-1)
		lastb += nSkip;
	    if (nSkip == 0)
		lastb = loc.base + IBSIZE(h) * loc.span;
	} while (lastb != 0 &&
	    (nfind == 1 || !epix_IsBlockAllocated(db)));
	return (nfind == 1 || !epix_IsBlockAllocated(db));

      case AFLAGS_DATA_FRAGMENTED:
	/*
	 * If the anode is COW, the fragment description always reflects
	 * the state of the backing anode, so we can't do any meaningful
	 * check here.  There is no particular reason why the fragment
	 * description couldn't be updated during truncations, but it's
	 * not worth the extra effort.
	 */
	if (EPIX_IsBlockBacking(dk->fBlock))
	    return (1);
	else {
	    log_bsize =  epia_AnodeDevice(h)->logFragSize;
	    lastb = (len == 0) ? 0 : (len - 1) >> log_bsize;
	    return (lastb + 1 >= dk->fLength);
	}

      case AFLAGS_DATA_INLINE:
	return (1);

      default:
	DBprint(( "VerifyNoBlocksAfterEOF: illegal anode flags" ));
	return (0);
    }
}

/*
 * VerifyAnodeFrags -- Check a fragmented anode.
 */
static long VerifyAnodeFrags(
  IN epi_anode_t File,
  IN struct diskAnode * d,
  IN struct diskAnode * b, /* backing Anode */
  IN epi_anode_t VolAnode,
  IN long ix,
  IN struct epiz_salvageState * ss)
{
    int code;
    afs_hyper_t maxLength;
    char DescBuff[100];
    struct epiz_SimBits * verif = ss->simBits;
    int why;				/* error reporting reason */

    ss->loc.tree = -2;
    ss->loc.block = d->d.frags.block;
    ssFragFirst(ss) = d->d.frags.first;
    ssFragLength(ss) = d->d.frags.length;

    AFS_hset64 (maxLength, 0, (d->d.frags.length * File->device->fragSize));
    if (AFS_hcmp(epia_GetLength(File), maxLength) > 0) {
	epiz_ReportError (ss, 0, EPIZ_E_TOOFEWFRAGS,
			  epiz_Printed(epiz_PrintedHyper, maxLength));
	if (ss->flags & EPIZ_SS_SALVAGE) {
	    /* at least set it to the max value */
	    code = SalvageAnodeLength (File, maxLength);
	    afsl_MBZ (code);
	}
    }
    why = 0;
    if (d->d.frags.length == 0)
	why = EPIZ_E_ZEROFRAGS;
    else if (d->d.frags.length > verif->fragsPerBlock)
	why = EPIZ_E_TOOMANYFRAGS;
    else if ((d->d.frags.length + d->d.frags.first) > verif->fragsPerBlock)
	why = EPIZ_E_TOOFARFRAGS;
    else if (epix_IsBlockEmpty(d->d.frags.block))
	why = EPIZ_E_FRAG_BLOCKEMPTY;
    if (why) {
	epiz_ReportError (ss, 0, why);
	if (ss->flags & EPIZ_SS_SALVAGE) {
	    code = SalvageMakeAnodeAllEmpty (File);
	    afsl_MBZ (code);
	}
	return 0;			/* no longer fragmented */
    }
    sprintf(DescBuff, "frag-block of ix %lu in vol@%#lx", ix, VolAnode);

    MaybePatchBacking (ss, &d->d.frags.block, 0, ANODE_FO(d.frags.block));
    ss->loc.index = 0;
    ss->loc.block = d->d.frags.block;

    why = epiz_CheckBlock (ss, d->d.frags.block);
    if (!why && b && EPIX_IsBlockInBacking(d->d.frags.block)) {

	/* Make sure any COW block references are right.  */

	afsl_Assert ((d->flags & AFLAGS_COPY_ON_WRITE));
	if (!(d->flags & AFLAGS_DATA_FRAGMENTED)) {

	    /* The backing anode is either blocked or inline, but it must be
	     * fragmented because this one is. */

	    why = EPIZ_E_COW_NOTFRAGMENTED;
	} else if ((d->d.frags.block != BACKINGDADDR(b->d.frags.block)) ||
		   (d->d.frags.first != b->d.frags.first) ||
		   (d->d.frags.length != b->d.frags.length)) {

	    /* The two fragment descriptions must be the same except for the
	     * possible presence fo the COW bit in the block address. */

	    why = EPIZ_E_COW_BADFRAG;
	}
    }					/* XXX else error process code? */
    if (why) {
	epiz_ReportError (ss, 0, why);
	if (ss->flags & EPIZ_SS_SALVAGE) {
	    code = SalvageMakeAnodeAllEmpty (File);
	    afsl_MBZ (code);
	}
	/* That's all we can do here. */
	return 0;
    }

    /* Call either MarkUsed or SaveAnodeRef */
    code = (*ss->blockRef) (d->d.frags.block, 0, File, ss, 0);

    if (code == EPI_E_DUPBLKUSE) {
	/* if its pass 2, move the block to a new place */
	/* note: EPIZ_SS_DUPFIXPASS is only run when we
	 ** are salvaging the surface, not verifying it */
	if (ss->pass == EPIZ_SS_DUPFIXPASS) {
	    struct epiz_anodeItem ** HeadP = &ss->mayRestore;
	    daddr_t newBlock;
	    u_int16 newOff;
	    printf( "Repairing duplicate block reference at " );
	    printf( "anode %x (volume %x) (of fragmented file)\n",
		   ix, VolAnode );
	    code = epiz_SalvageNewFrags (ss, File, d->d.frags.block, &newBlock,
					 &newOff);
	    DBprint(( "VerifyAnode: %d epiz_SalvageNewFrags code %x\n",
		     __LINE__, code ));
	    if (code) {
		newBlock = (daddr_t)EPIX_ANODE_BLOCKEMPTY;
		HeadP = &ss->oughtRestore;
	    }
	    code = epiz_MarkUnsafeAnode( File, HeadP, ss );
	    afsl_MBZ(code);
	    /* update value in the block */
	    code = epiz_SalvageAnodeField( File, ANODE_FO(d.frags.block),
					  newBlock );
	    afsl_MBZ(code);	/* XXX do more here? */
	    code = epiz_SalvageAnodeShortField( File, ANODE_FO(d.frags.first),
					  newOff );
	    afsl_MBZ(code);
	}
	code = 0;
    }
    afsl_MBZ(code);

    return (0);
}

/*
** VerifyAnodeDBlocks
** Verify the direct blocks of an anode.
*/
static long VerifyAnodeDBlocks(
  IN epi_anode_t File,
  IN struct diskAnode *d,
  IN struct diskAnode *b, /* backing Anode */
  IN epi_anode_t VolAnode,
  IN long ix,
  IN struct epiz_salvageState *ss)
{
    int code;
    int Idx;
    char DescBuff[100];
    int why;

    ss->loc.tree = -1;

    /* Verify the direct block pointers */
    for (Idx = 0; Idx < ANODE_DIRECTBLOCKS; ++Idx) {

	if (epix_IsBlockEmpty(d->dBlock[Idx])) continue;

	ss->loc.index = Idx;
	ss->loc.block = d->dBlock[Idx];

	MaybePatchBacking (ss, &d->dBlock[Idx], 0, ANODE_FO(dBlock[Idx]));
	ss->loc.block = d->dBlock[Idx];

	sprintf(DescBuff, "direct block %d=%#lx of ix(%lu) %lu in vol@%#lx",
		Idx, d->dBlock[Idx], ix, epia_GetType(File), VolAnode);

	why = epiz_CheckBlock (ss, d->dBlock[Idx]);
	if (!why) why = epiz_CheckEOF (ss, File, Idx);
	if (!why && b && EPIX_IsBlockInBacking(d->dBlock[Idx])) {
	    afsl_Assert ((d->flags & AFLAGS_COPY_ON_WRITE));
	    if (b->flags & AFLAGS_DATA_NOTBLOCKED) {

		/* The backingAnode is either fragmented or inline, but the
		  * direct daddr_t block reference is a backingAnode reference,
		 * AND this isn't a fragmented COW anode! */

		why = EPIZ_E_COW_NOTBLOCKED;
	    } else if (d->dBlock[Idx] !=
			BACKINGDADDR(b->dBlock[Idx])) {
		epiz_ReportError (ss, 0, EPIZ_E_COW_BADBLOCK, b->dBlock[Idx]);
		why = EPIZ_E_NOOP;
	    }
	}
	if (why) {
	    epiz_ReportError (ss, 0, why);
	    if (ss->flags & EPIZ_SS_SALVAGE) {
		code = epiz_SalvageAnodeField (File,
						ANODE_FO( dBlock[Idx] ),
						EPIX_ANODE_BLOCKEMPTY);
		afsl_MBZ (code);
	    }
	    continue;			/* skip to next Idx */
	}

	/* Call either MarkUsed or SaveAnodeRef */
	code = (*ss->blockRef) (d->dBlock[Idx], Idx, File, ss, 0);

	if (code == EPI_E_DUPBLKUSE) {
	    /* if its pass 2, move the block
	     * reference to a new block */
	    if (ss->pass == EPIZ_SS_DUPFIXPASS) {
		struct epiz_anodeItem ** HeadP = &ss->mayRestore;
		daddr_t newBlock;
 DBprint(( "VerifyAnode: %d SalvageNewFrags allocated %x flags %x/LOG %x\n",
	  __LINE__, Disk(File)->allocated, Disk(File)->flags, AFLAGS_LOGGED ));
		code = epiz_SalvageNewFrags (ss, File, d->dBlock[Idx],
					     &newBlock, (u_int16 *)0);
 DBprint(( "VerifyAnode: %d epiz_SalvageNewFrags allocated %x code %x\n",
			 __LINE__, Disk(File)->allocated, code ));
		if (code) {
		    newBlock = (daddr_t)EPIX_ANODE_BLOCKEMPTY;
		    HeadP = &ss->oughtRestore;
		}
		code = epiz_MarkUnsafeAnode( File, HeadP, ss );
		afsl_MBZ(code);
		/* update value in the block */
		code = epiz_SalvageAnodeField (File,
						ANODE_FO( dBlock[Idx] ),
						newBlock);
		afsl_MBZ(code);	/* XXX do more here? */
	    }
	    code = 0;
	}
	afsl_MBZ (code);		/* unexpected code from MarkUsed */
    }
    return (0);
}

/* VerifyAnodeIBlocks -- verifies the indirect blocks of an anode. */

static long VerifyAnodeIBlocks(
  epi_anode_t File,
  struct diskAnode *d,
  epi_anode_t Back,
  struct diskAnode *b,		/* backing Anode */
  epi_anode_t VolAnode,
  long ix,
  struct epiz_salvageState *ss)
{
    int code;
    int Idx;
    long StartLen;
    u_long SkipHere, ibsize;
    char DescBuff[100];
    int why;

    /*
    ** Verify the 'indirect tree' block pointers
    ** Since we are going to use ScanIndirect for all
    ** our scans, we set up a few bits to indicate what
    ** is alright/not alright for that routine to allow.
    */

    afsl_Assert ((!(d->flags & AFLAGS_COPY_ON_WRITE) ==
		  ((ss->flags & EPIZ_SS_NOCOWREFS) != 0)) &&
		 (!(d->flags & AFLAGS_COPY_ON_WRITE) ||
		  !(ss->flags & EPIZ_SS_SALVAGE) || b));

    ibsize = IBSIZE(File);
    for (Idx = 0, SkipHere = 1, StartLen = ANODE_DIRECTBLOCKS;
	 Idx < ANODE_INDIRECT_TREES;
	 ++Idx, SkipHere *= ibsize, StartLen += SkipHere) {
	long offset = ANODE_FO(iBlock[Idx]);
	daddr_t newDaddr;
	daddr_t backDaddr;

	if (epix_IsBlockEmpty(d->iBlock[Idx])) continue;

	ss->loc.index = StartLen;
	ss->loc.tree = Idx;
	ss->loc.depth = 0;
	ss->loc.blocks[0] = d->iBlock[Idx];
	ss->loc.indexes[0] = -1;

	MaybePatchBacking (ss, &d->iBlock[Idx], 0, ANODE_FO(iBlock[Idx]));
	newDaddr = d->iBlock[Idx];
	ss->loc.block = newDaddr;

	sprintf(DescBuff, "indirect root #%d=%#lx of ix %lu in vol@%#lx",
		Idx, d->iBlock[Idx], ix, VolAnode);

	backDaddr = (((d->flags & AFLAGS_COPY_ON_WRITE) && b)
		     ? b->iBlock[Idx] : EPIX_ANODE_BLOCKEMPTY);

	DBprint(( "VerifyAnodeIBlocks: iBlock[%d] => %x\n",
		 Idx, newDaddr ));

	/* Do some preliminary checking, then call ScanIndirect or
	 * CountIndirectTree, as appropriate. */

	why = epiz_CheckBlock (ss, newDaddr);
	if (why) goto error;
	why = epiz_CheckEOF (ss, File, StartLen);
	if (why) goto error;
	if (b && EPIX_IsBlockInBacking(newDaddr)) {
	    afsl_Assert ((d->flags & AFLAGS_COPY_ON_WRITE));
	    if (b->flags & AFLAGS_DATA_NOTBLOCKED) {

		/* The backingAnode is either fragmented or inline, but the
		 * direct daddr_t block reference is a backingAnode reference,
		 * AND this isn't a fragmented COW anode! */

		why = EPIZ_E_COW_NOTBLOCKED;
		goto error;
	    } else if (newDaddr != BACKINGDADDR(backDaddr)) {
		epiz_ReportError (ss, 0, EPIZ_E_COW_BADBLOCK, backDaddr);
		why = EPIZ_E_NOOP;
		goto error;
	    }
	}

	if (epix_IsBlockAllocated(newDaddr)) {
	    code = ScanIndirect( &newDaddr, File, backDaddr, ss, Idx,
				StartLen, File->block,  File->offset,
				DescBuff);
	    DBprint(( "VerifyAnodeIBlocks: ScanIndirect code %d volQuota %d quotaAnode %d\n",
		     code, ss->quotaVal.a, ss->quotaAnode.a ));

	    ss->loc.depth = 0;
	    ss->loc.indexes[0] = -1;
	    ss->loc.block = d->iBlock[Idx];

	    if (code == EPI_E_BADINDIRECT) {
		why = EPIZ_E_BADIB;
		goto error;
	    } else if (code == EPI_E_DUPBLKUSE) {
		/* we should handle this here, but instead just reorganize to call SalvageNewFrags. */
		afsl_MBZ (code);	/* always panic */
	    } else {
		afsl_MBZ (code);
	    }
	} else if (EPIX_IsBlockInBacking(newDaddr)) {
	    u_long visible = 0;

	    code = CountIndirectTree (File, newDaddr, StartLen, SkipHere,
				      0, &visible);
	    if (code == EPI_E_BADINDIRECT) {
		why = EPIZ_E_COW_BADIB;
		goto error;
	    }
	    visible *= ss->simBits->fragsPerBlock;
	    ss->quotaVal.v += visible;
	    ss->quotaAnode.v += visible;
	}

	/* Call either MarkUsed or SaveAnodeRef */
	code = (*ss->blockRef)(d->iBlock[Idx], StartLen, File, ss, Idx);

	if (code == EPI_E_DUPBLKUSE) {
	    if (ss->pass == EPIZ_SS_DUPFIXPASS) {
		struct epiz_anodeItem ** HeadP = &ss->mayRestore;
		daddr_t newBlock;

		code = epiz_SalvageNewFrags (ss, File, d->iBlock[Idx],
					     &newBlock, (u_int16 *)0);
		DBprint(( "VerifyAnodeIBlocks: %d epiz_SalvageNewFrags code %x\n",
			 __LINE__, code ));
		if (code) {
		    newBlock = (daddr_t)EPIX_ANODE_BLOCKEMPTY;
		    HeadP = &ss->oughtRestore;
		}
		code = epiz_MarkUnsafeAnode( File, HeadP, ss );
		afsl_MBZ(code);
		/* update value in the block */
		code = epiz_SalvageAnodeField( File, ANODE_FO( iBlock[Idx] ),
					      newBlock );
		afsl_MBZ(code);	/* XXX do more here? */
		DBprint(( "VerifyAnodeIBlocks: newBlock %x\n",
			 d->iBlock[Idx] ));
	    }
	    code = 0;
	}
	afsl_MBZ (code);

error:
	if (why) {			/* blow the block away */
	    epiz_ReportError (ss, 0, why);
	    if (ss->flags & EPIZ_SS_SALVAGE) {
		code = epiz_SalvageAnodeField (File, offset,
						EPIX_ANODE_BLOCKEMPTY);
		afsl_MBZ (code);
	    }
	    code = 0;
	}
    }
    return (0);
}

/*
 * epia_VerifyAnode -- checks out an anode.
 *
 * RETURN CODES -- Note that any non-zero error code returned from here will
 *     cause the anode to be pruned from the volume table (and possibly leaving
 *     quite a lot of damage for a future salvage run to clean up).
 *
 *     A -1 is returned if epiz_ReportError has already been called.  When this
 *     code is passes to epiz_ReportError (e.g. by epit_VerifyVolumeTable) it
 *     will not try to interpret the code.
 */

/* SHARED */
long epia_VerifyAnode(
  struct epiz_salvageState *ss,
  epi_anode_t VolAnode,
  long ix)
{
    long code, type;
    struct diskAnode *d;
    struct epiz_SimBits * verif = ss->simBits;

    ss->volIx = epia_GetIndex (VolAnode);
    ss->anodeIx = ix;
    ss->quotaAnode.a = ss->quotaAnode.v = 0;

    BEGIN_LCRIT (epi_anode_t File,
		 code = epia_Open(VolAnode, ix, 0, 0, &File),
		 code = epia_Close (File)) {
	afs_hyper_t maxLength;
	epi_anode_t Back = 0;		/* backing anode for the cow */
	struct diskAnode *b = 0;	/* backing anode */

	/* defer setting ss->vol / ss->anode until we check things out a bit */
	ss->loc.block = (daddr_t)EPIX_ANODE_BLOCKEMPTY;
	if (ss->pass == EPIZ_SS_SCANPASS) ss->nAnodes++;

	/* make sure per-anode COW related bits are off */
	ss->flags &= ~(EPIZ_SS_REPAIRCOWREFS | EPIZ_SS_NOCOWREFS |
			EPIZ_SS_MAYBECOWUSES | EPIZ_SS_MAYBERESALVAGE);

	type = epia_GetType(File);
	if (epia_GetType(VolAnode) == EPIA_CFLAGS_VOLUME) {
	    DBprint(("Anode %lu in volume at %#lx(%s) type %d: len %lu,,%lu\n",
		     ix, VolAnode, epiz_ssVolName(ss), type,
		     AFS_HGETBOTH(epia_GetLength(File))));
	} else {
	    DBprint(("Anode %lu in volume at %#lx type %d: len %lu,,%lu\n",
		     ix, VolAnode, type, AFS_HGETBOTH(epia_GetLength(File))));
	}

	afsl_Assert ((verif->blkSize == File->device->blkSize) &&
		     Disk(File) && Disk(VolAnode));

	d = Disk(File);
#if 0 /* This check is now being done in epia_Open */
	if (d->index != ix) {
	    epiz_ReportError (ss, 0, EPIZ_E_BADANODEINDEX, d->index);
	    /* This anode it too damaged to continue with it.  The caller
	     * will blow it away. */
	    ABORT_CRIT (-1);
	}
#endif
#if 0 /* this field is no long signed */
	if (d->copies < 0) {
	    epiz_ReportError (ss, 0, EPIZ_E_NEGATIVECOPIES, d->copies);
	    /* Since this can't really happen, lets assume the anode is
	     * totally trashed and have the caller remove it. */
	    { ABORT_CRIT (-1) }
	}
#endif
	if (ss->expectedType != -1 && type != ss->expectedType) {
	    epiz_ReportError (ss, 0, EPIZ_E_UNEXPECTEDTYPE, type);
	    /* caller will trash it */
	    { ABORT_CRIT (-1) }
	}

	if (d->copies) {

	    /* If this anode is in the AVL we now any COW or copies stuff is
	     * wrong, so clear this field.  Otherwise bump out copies-map by
	     * the number we think are pointing to us. */

	    if (epia_GetVolIndex(File) == epia_GetIndex(ss->aggrAnode)) {
		epiz_ReportError (ss, 0, EPIZ_E_NOAVLCOPIES, (int)d->copies);
		if (ss->flags & EPIZ_SS_SALVAGE) {
		    code = epiz_SalvageAnodeShortField (File, ANODE_FO(copies),
							/*clear*/0);
		}
	    } else {
		ss->flags |= EPIZ_SS_MAYBECOWUSES;
		code = epiz_AddCopiesVerify (d->copies, File, d->index, ss);
	    }
	    afsl_MBZ (code);
	}
	if (d->flags & AFLAGS_COPY_ON_WRITE) {

	    /* This anode is copy-on-write; this it okay if we are not an anode
	     * in the AVL.  Also this bit means we reference backingAnode once,
	     * so check that backing reference is okay and decrement its
	     * copies-map by one.  We fail if the backing anode's copies count
	     * is zero (GetBackingAnode checks this).  While we will repair the
	     * copies count we will not change copies from zero to non-zero.
	     * This prevents loops in the COWFIXPASS. */

	    if (epia_GetVolIndex(File) == epia_GetIndex(ss->aggrAnode)) {
		epiz_ReportError (ss, 0, EPIZ_E_NOAVLCOW);
		code = -1;
	    } else {
		/* use locking version to avoid assert */
		code = epix_GetBackingAnode( File, &Back );
		if (code)
		    epiz_ReportError (ss, 0, EPIZ_E_COW_BADPTR,
				      d->c.backingAnode.block,
				      d->c.backingAnode.offset);
		else {
		    b = Disk(Back);
		    code = epiz_AddCopiesVerify( -1, Back, b->index, ss );
		    if (code)
			epiz_ReportError (ss, 0, EPIZ_E_COW_BADVOL,
					  Back->volIndex);
		}
	    }

	    if (code) {

		/* This failed because anode was in AVL or we couldn't open the
		 * backing anode or its volume.  In and of itself this
		 * shouldn't be fatal.  The smallest fix we can make is to
		 * break the COW link and hope the block addresses can be
		 * fixed. */

		ss->flags |= EPIZ_SS_REPAIRCOWREFS;
		if (ss->flags & EPIZ_SS_SALVAGE) {
		    do { /* This code cribbed from UnbackAnode */
			int32 flags;
			struct backPtr backPtr;

			backPtr.block = 0, backPtr.offset = 0;
			flags = Disk(File)->flags & ~AFLAGS_COPY_ON_WRITE;

			if (File->backingAnode) {
			    code = epia_Close (File->backingAnode);
			    if (code) continue;
			    File->backingAnode = 0;
			}

			code = epiz_SalvageAnodeField
			    (File, ANODE_FO (flags), flags);
			if (code) continue;
			code = epiz_SalvageAnodeField
			    (File, ANODE_FO (bBlock), backPtr.block);
			if (code) continue;
			code = epiz_SalvageAnodeField
			    (File, ANODE_FO (bOffset), backPtr.offset);
			if (code) continue;
		    } while (0);
		    afsl_MBZ (code);
		}
#if 0
		else {
		    /* If not salvaging then we can't cleanup this up so just
		     * punt the whole anode. */
		    code = -1;
		    continue;
		}
#else
		code = 0;
#endif
	    }
	}

	/* Since errors returned by this procedure cause the caller to kill the
	 * anode, which means calling ReportError to put it on the deadFiles or
	 * deadVolumes list.  If the vol/anode field of ss is set the resalvage
	 * bit will be set.  Also this is used to report better information
	 * about the failing anode (e.g. the length). */

	if (VolAnode == ss->aggrAnode) ss->vol = File;
	else ss->anode = File;

	/* Before we scan the block addresses check if we allow the COW bit */
	if (!(d->flags & AFLAGS_COPY_ON_WRITE))
	    ss->flags |= EPIZ_SS_NOCOWREFS;

	if (d->flags & AFLAGS_DATA_INLINE) {
	    code = 0;
	    AFS_hset64 (maxLength, 0, AnodeMaxInlineData(File));
	    if (AFS_hcmp(epia_GetLength(File), maxLength) > 0) {
		epiz_ReportError (ss, 0, EPIZ_E_MAXINLINE,
				  AFS_HGETBOTH(epia_GetLength(File)),
				  AnodeMaxInlineData(File));
		code = epiz_MarkUnsafeAnode( File, &ss->mayRestore, ss );
		if (ss->flags & EPIZ_SS_SALVAGE) {
		    /* at least set it to the max value */
		    code = SalvageAnodeLength(File, maxLength);
		    afsl_MBZ(code);
		}
	    }
	} else if (d->flags & AFLAGS_DATA_FRAGMENTED) {
	    code = VerifyAnodeFrags( File, d, b, VolAnode, ix, ss );
	} else {			/* do MarkUsed/SaveAnodeRef on blocks in container */
	    code = VerifyAnodeDBlocks( File, d, b, VolAnode, ix, ss );
	    if (code == 0)
		code = VerifyAnodeIBlocks (File, d, Back, b, VolAnode, ix, ss);
	}
	if (code) continue;		/* punt the anode */

	/* Check allocated against the calculated value */
	if (Disk(File)->allocated != ss->quotaAnode.a) {
	    epiz_ReportError (ss, 0, EPIZ_E_BADALLOC,
		Disk(File)->allocated, ss->quotaAnode.a);
	    if (ss->flags & EPIZ_SS_SALVAGE) {
		code = epiz_SalvageAnodeField( File, ANODE_FO( allocated ),
					      ss->quotaAnode.a );
		afsl_MBZ (code);
	    }
	}
	DBprint(( "epia_VerifyAnode: %d anode allocated correct (%d) volume %d\n",
		 Disk(File)->index, Disk(File)->allocated, ss->quotaVal.a ));

	/* Check visible against the calculated value */
	if (Disk(File)->visible != ss->quotaAnode.v) {
	    epiz_ReportError (ss, 0, EPIZ_E_BADVISIBLE,
		Disk(File)->visible, ss->quotaAnode.v);
	    if (ss->flags & EPIZ_SS_SALVAGE) {
		code = epiz_SalvageAnodeField( File, ANODE_FO( visible ),
					      ss->quotaAnode.v );
		afsl_MBZ (code);
	    }
	}
	DBprint(( "epia_VerifyAnode: %d anode visible correct (%d) volume %d\n",
		 Disk(File)->index, Disk(File)->visible, ss->quotaVal.v ));
    } END_CRIT;
    return code;
}

/* epiz_SalvageCopiesVerify -- Salvage one anodes's copies field.  The copies
 *     value passed in is relative to Disk(h)->copies.  We never change a zero
 *     copies field to non-zero; this avoid COWFIXPASS loops. */

/* SHARED */
long epiz_SalvageCopiesVerify(
  epi_anode_t vol,
  u_long anodeIx,
  struct epiz_copiesVerify *cv,
  struct epiz_salvageState *ss)
{
    long code = 0;

    afsl_Assert (vol && anodeIx);
    BEGIN_LCRIT( epi_anode_t h,
		code = epia_Open( vol, anodeIx, 0, 0, &h ),
		code = epia_Close( h ) ) {
	long newCopies;
	if ((h->block != cv->anode[anodeIx].block) ||
	    (h->offset != cv->anode[anodeIx].offset)) {

	    /* Backing anode isn't the one we thought it was so punt as if the
	     * index was bad. */

	    ABORT_CRIT (EPI_E_BADANODEINDEX);
	}
	newCopies = (long)(Disk(h)->copies) - cv->anode[anodeIx].copies;
	afsl_Assert (Disk(h)->copies &&
		     (newCopies >= 0) && (newCopies < (1<<16)));
	code = epiz_SalvageAnodeShortField( h, ANODE_FO( copies ), newCopies);
	afsl_MBZ (code);
    } END_CRIT;
    if (code) {
	epiz_ReportError (ss, 0, EPIZ_E_IRREPARABLECOPIES);
    }
    return 0;
}

/*
** epia_ClobberAnode.
** Given an epi_anode_t, and a kinda clobbering, ruin
** the anode.  A seocnd epi_anode_t is also passed in,
**
** This routine exists as a mechanism for testing the
** success of particular kinds of salvages.  It is not
** intended for general use.
**
** Both anodes passed in are already open. They are open
** (from test_anode) using a higher level interface:
** epif_Open.
**
*/
/* SHARED */
long epia_ClobberAnode(
  epi_anode_t dest,
  epi_anode_t src,
  long kind)
{
    struct epia_info info;
    int code;

    printf( "epia_ClobberAnode: dest anode %x src anode %x kind %x\n",
	dest, src, kind );

    code = epia_GetInfo( dest, &info );
    if (code == 0) {
	printf( "epia_ClobberAnode: flags %x length %x,,%x allocated %x index %x copies %x\n",
		info.flags, AFS_HGETBOTH(info.length), info.allocated,
		info.index, info.copies );
    } else {
	printf( "epia_ClobberAnode: no info on dest code %d\n", code );
    }

    code = epia_GetInfo( src, &info );
    if (code == 0) {
	printf( "epia_ClobberAnode: flags %x length %x,,%x allocated %x index %x copies %x\n",
		info.flags, AFS_HGETBOTH(info.length), info.allocated,
		info.index, info.copies );
    } else {
	printf( "epia_ClobberAnode: no info on src code %d\n", code );
    }

    switch( kind ) {
      case 0: /* dblock clobber */
	if ((Disk(dest) == 0) || (Disk(src) == 0)) {
	    printf( "Fry_Anode: No disk block for dest or src\n" );
	    code = -1;
	}
	printf( "dblock_clobber: dBlock Clobber dblock %x src dblock %x offset %d\n",
		Disk(dest)->dBlock[0], Disk(src)->dBlock[0],
		ANODE_FO( dBlock[0] ) );
	code = epiz_SalvageAnodeField( dest,
		ANODE_FO( dBlock[0] ),
		Disk(src)->dBlock[0] );
	printf( "dblock_clobber: dBlock Clobber code %d clobber dest dblock %x src dblock %x\n",
		code, Disk(dest)->dBlock[0], Disk(src)->dBlock[0] );
	break;

      case 1: /* iblock clobber */
	if ((Disk(dest) == 0) || (Disk(src) == 0)) {
	    printf( "Fry_Anode: No disk block for dest or src\n" );
	    code = -1;
	}
	printf( "dblock_clobber: iBlock Clobber code %d clobber dest iblock %x src iblock %x\n",
		code, Disk(dest)->iBlock[1], Disk(src)->iBlock[1] );
	code = epiz_SalvageAnodeField( dest,
		ANODE_FO( iBlock[1] ),
		Disk(src)->iBlock[1] );
	printf( "dblock_clobber: iBlock Clobber code %d clobber dest iblock %x src iblock %x\n",
		code, Disk(dest)->iBlock[1], Disk(src)->iBlock[1] );
	break;

      case 2: /* cowblock clobber */
	break;

      case 4: /* xblock -- move across tree depths */
	if ((Disk(dest) == 0) || (Disk(src) == 0)) {
	    printf( "Fry_Anode: No disk block for dest or src\n" );
	    code = -1;
	}
	printf( "dblock_clobber: xBlock Clobber code %d clobber dest xblock %x src iblock %x\n",
		code, Disk(dest)->iBlock[1], Disk(src)->iBlock[2] );
	code = epiz_SalvageAnodeField( dest,
		ANODE_FO( iBlock[1] ),
		Disk(src)->iBlock[2] );
	printf( "dblock_clobber: xBlock Clobber code %d clobber dest xblock %x src iblock %x\n",
		code, Disk(dest)->iBlock[1], Disk(src)->iBlock[2] );
	break;

      default:
	/* no such clobberKind */
	code = -1;
	break;
    }
    return (code);
}
#endif /* not KERNEL */
