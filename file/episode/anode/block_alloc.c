/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* block_alloc.c -- contains the aggregate block allocation procedures.  This
 *     include both managing the bitmap container and implementing allocation
 *     policy.
 *
 *     The storage for the allocator is maintained on a per aggregate basis and
 *     addressed by an allocation handle.  The bitmap container is supplied
 *     when aggregate initialization calls the Init procedure which allocates
 *     and returns the allocation handle.  There is a provision for associating
 *     some storage with each anode, perhaps for a reservation mechanism, but
 *     this is unimplemented. */

/* Copyright (C) 1996, 1990 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>
#include <dcedfs/osi.h>
#ifdef KERNEL
#include <dcedfs/osi_user.h>
#endif /* KERNEL */
#include <dcedfs/debug.h>
#include <dcedfs/stds.h>
#include <dcedfs/episode/epi_thread.h>	/* must come before bomb.h */
#include <dcedfs/bomb.h>
#include <dcedfs/episode/async.h>
#include <dcedfs/episode/fifo.h>
#include <dcedfs/episode/logbuf.h>
#include <anode.h>
#include <fixed_anode.h>
#include <block_alloc.h>

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/anode/RCS/block_alloc.c,v 4.219 1996/03/04 18:58:14 ota Exp $")

#define DEBUG_THIS_FILE EPIA_DEBUG_BLOCK_ALLOC

#define BITMAP_MAGIC 0x5b7674d7

struct bitmapPage {
    int32 initialStamp;			/* set for every page write */
    int32 magic;			/* BITMAP_MAGIC */
    daddr_t baseOffset;			/* block address of first bit */
    u_int32 blocksPerPage;		/* number blocks in this page */
    u_int32 bitmapLength;		/* total number of blocks */
    u_int16 fragmentsPerBlock;
    u_int16 spare16;
    int32 spare32;
    /* This array is only present if (fragmentsPerBlock > 1) and records, for
     * each partially allocated block of fragments in the "bits" array, whether
     * the frags contain logged data or not.  There are "blocksPerPage"/8
     * bytes in this array. */
    u_char logged[8];			/* logged frags; one per block */
    /* There is one bit per fragment in this array.  It is
     * "blocksPerPage"*"fragmentsPerBlock"/8 bytes in length. */
    u_char bits[8];			/* bits; one per fragment */
    int32 finalStamp;			/* same as initialStamp */
};

typedef struct bitmapPage bitmapPage_t;

#define BITMAP_PAGE_OVERHEAD \
    (offsetof(bitmapPage_t, logged) + \
     sizeof (bitmapPage_t) - offsetof(bitmapPage_t, finalStamp))
#define BitsArray(page, h) ((page)->logged + (h)->loggedLength)

/* Get fragment number of first fragment in the same block */
#define BaseFrag(h, firstFrag) ((firstFrag) & ~((h)->fragmentsPerBlock - 1))

struct allocationHint {
    daddr_t start;
};

static u_long nBlockAllocates = 0;
static u_long nFragAllocates = 0;
static u_long randomSeed = 0;

#define BIG_PRIME 1257056893		/* 0x4AED2A7d */

#define NewRandom(v) (randomSeed = randomSeed * BIG_PRIME + (v))

#ifdef AFS_DEBUG
/* This is an array of bit maps.  Each bit map contains 256 bits (32 bytes)
 * where each bit corresponds to one entry in the fragSearchArray or
 * fragCrossArray.  These bit maps are assigned to each possible fragment group
 * size in each block size, up to 8 fragments per block.  This uses the first
 * 15 bit maps to record usage of the fragSearchArray.  The 16th array records
 * usage of fragCrossArray in searching for fragment groups when the number of
 * fragments per block is larger than eight.
 *
 * bit map #, frags/block, nFrags
 *     0          1          1
 *     1-2	  2	     1-2
 *     3-6	  4	     1-4
 *     7-14	>=8	     1-8
 *     15	 >8	     n/a
 */

/* SHARED */
u_char epib_fragmentCoverage[16][32];

#ifndef _KERNEL
/* SHARED */
double epib_PercentFragmentCoverage(int map)
{
    int i, bits, cnt = 0;
    for (i = 0; i < 32; i++) {
	bits = epib_fragmentCoverage[map][i];
	while (bits != 0) {
	    cnt++;
	    bits &= bits - 1;
	}
    }
    return (double)cnt * 100.0 / 256.0;
}
#endif /* !_KERNEL */

#define MarkFragmentCoverage(fragsPerBlock, nFrags, byte) \
    (epib_fragmentCoverage[(nFrags <= 8) ? \
			   (nFrags - 1 + MIN(8, fragsPerBlock) - 1) : 15 \
			   ] [byte >> 3] |= 1 << (byte & 0x7), \
     (fragsPerBlock > 8) ? \
      epib_fragmentCoverage[15][byte >> 3] |= 1 << (byte & 0x7) : 0)
#else
#define MarkFragmentCoverage(a, b, c) 0
#endif

/*
 * These two structures allow general list and hash table management of the
 * structures used to manage the freed block list.  Structures that describe
 * both blocks and transactions can be handled similarly.  Both need hash table
 * access as well as a free list and a fixed allocation strategy.
 */
struct fb_header {
    struct fb_header *next;
    u_long key;
};

struct fb_objects {
    u_int max;				/* number of preallocated objects */
    u_int size;				/* size of each */
    u_int new;				/* number newly allocated */
    char *storage;			/* array of objects */
    struct fb_header *freeList;		/* unused objects */
    u_int htLength;			/* number of hash table buckets */
    u_int htEntries;			/* number of entries in ht */
    struct fb_header **hashTable;
};

static struct fb_header *
fb_Alloc(struct fb_objects *objs)
{
    struct fb_header *fbh = objs->freeList;
    caddr_t object;

    epix_EnterCodeBlock(47);
    if (fbh != NULL) {
	objs->freeList = fbh->next;
	object = (caddr_t)fbh;
    } else {
	epix_EnterCodeBlock(48);
	objs->new++;
	object = osi_Alloc_r(objs->size);
    }
    bzero(object, objs->size);
    return (struct fb_header *)object;
}

#define fb_Free(objs, fbh) \
    (((struct fb_header *)(fbh))->next = (objs)->freeList, \
     (objs)->freeList = (struct fb_header *)(fbh))

#define fb_HashIndex(objs, key) \
    ((key * BIG_PRIME) & ((objs)->htLength - 1))

static struct fb_header *
fb_Lookup(struct fb_objects *objs, u_long key)
{
    struct fb_header *fbh;
    u_int hi = fb_HashIndex(objs, key);

    epix_EnterCodeBlock(49);

    fbh = objs->hashTable[hi];
    while (fbh != NULL && fbh->key != key)
	fbh = fbh->next;

    return fbh;
}

static void
fb_Insert(struct fb_objects *objs, struct fb_header *fbh)
{
    u_int hi;				/* hash index */

    epix_EnterCodeBlock(50);

    afsl_Assert(fb_Lookup(objs, fbh->key) == NULL);
    hi = fb_HashIndex(objs, fbh->key);
    fbh->next = objs->hashTable[hi];
    objs->hashTable[hi] = fbh;
    objs->htEntries++;
}

/* fb_Remove -- removes a structure from  */

static void
fb_Remove(struct fb_objects *objs, struct fb_header *fbh)
{
    struct fb_header *chain;		/* for walking hash chain */
    u_int hi;				/* hash index */

    afsl_Assert(fbh == fb_Lookup(objs, fbh->key));
    hi = fb_HashIndex(objs, fbh->key);
    chain = objs->hashTable[hi];
    afsl_Assert(chain != NULL);
    if (chain == fbh)
	objs->hashTable[hi] = fbh->next;
    else {
	while (chain != NULL && chain->next != fbh)
	    chain = chain->next;

	afsl_Assert(chain != NULL);
	chain->next = fbh->next;
    }
    afsl_Assert(objs->htEntries != 0);
    objs->htEntries--;
}

/* fb_Init -- inits and preallocates space for an fb_objects structure. */

static void
fb_Init(u_int size, u_int max, u_int len, struct fb_objects *objs)
{
    int i;
    size_t s;				/* bytes to allocate */

    afsl_Assert(size >= sizeof (struct fb_header *) && (size & 0x3) == 0);
    objs->size = size;
    objs->max = max;
    objs->storage = osi_Alloc_r(max * size);
    for (i = 0; i < max; i++)
	fb_Free(objs, (struct fb_header *)&objs->storage[i * size]);
    objs->htLength = len;
    s = len * sizeof (struct fb_header *);
    objs->hashTable = osi_Alloc_r(s);
    bzero((caddr_t)objs->hashTable, s);
}

/* fb_Cleanup -- frees allocated memory used by an fb_objects structure. */

static void
fb_Cleanup(struct fb_objects *objs)
{
    struct fb_header *fbh, *next;

    epix_EnterCodeBlock(51);

    afsl_Assert(objs->htEntries == 0);	/* hash table is empty */

    /* free structures that were not preallocated individually */
    for (fbh = objs->freeList; fbh != NULL; fbh = next) {
	char *object = (char *)fbh;
	next = fbh->next;
	if (!(&objs->storage[0] <= object  &&
	    object < &objs->storage[objs->max * objs->size])) {
	    epix_EnterCodeBlock(52);
	    objs->new--;
	    osi_Free_r(object, objs->size);
	}
    }
    afsl_Assert(objs->new == 0); /* all non-preallocated ones were freed */

    /* free preallocated storage and hash table */
    osi_Free_r(objs->storage, objs->max * objs->size);
    osi_Free_r(objs->hashTable, objs->htLength * sizeof (struct fb_header *));
}

/* struct freeTrans -- describes a transaction used for freeing blocks.  It
 *     contains a flags word that identifies transactions that have been ended.
 *     It tracks the block reservations and allocations, as well as the list of
 *     block structures. */

struct freeTrans {
    struct fb_header hdr;		/* transaction id is key */
    /* hdr.key = (elbb_tranRec_t) trans; */
    long flags;				/* status of trans */
    u_int nBlocks;			/* number freed this trans */
    struct fifo_Thread live;		/* thread of active or ended trans */
    struct fifo blocks;			/* block list */
};
#define ft_TransOf(ft) ((elbb_tranRec_t)(ft)->hdr.key)

#define FB_TRANS_ENDED	0x1		/* transaction is ended & !committed */

/* struct freedBlock -- describes a single block freed under a transaction. */

struct freedBlock {
    struct fb_header hdr;		/* block that has been freed is key */
    /* hdr.key = (daddr_t) block; */
    int flags;				/* kill block on commit? */
    struct freeTrans *trans;		/* corresponding transaction info */
    struct fifo_Thread blocks;		/* next block same trans */
    struct bits64 {
	int32 words[2];
    } frags;				/* freed frags in block  */
};
#define fb_BlockOf(fb) ((daddr_t)(fb)->hdr.key)

#define FB_BLOCK_KILL	0x1		/* kill block when tran. commits */

/* struct freedBlockList -- is used to maintain a list of free blocks whose
 *     transactions are uncommitted.  The storage is managed using a fixed
 *     array of entries which is allocated when the aggregate is initialized.
 *     The entries represent transactions and the blocks freed in each one. */

struct freedBlockList {
    long waiters;			/* wakeup requested */
    epi_cond_t cond;			/* for awaiting freed blocks */
    async_device_t *device;		/* for doing buffer kills */
    struct fb_objects blocks;		/* freed blocks structures */
    struct fb_objects trans;		/* transaction structures */

    struct fifo activeTrans;		/* list of active trans */
    u_int nRunning;			/* number started but not ended */
    struct fifo endedTrans;		/* list of ended trans */
    elbb_tranRec_t deferredTrans;	/* for debugging NBS: deferred trans */
};

#define FB_MAX_STARTED_TRANS	100	/* see epib_StartTran comment, below */

struct allocationData {
    long magic;				/* BITMAP_MAGIC */
    epi_anode_t bitmap;			/* bitmap container */
    struct tmutex_data lock;		/* Used as a mutex */
    daddr_t base;			/* first block in bitmap */
    u_long length;			/* number of blocks in bitmap */
    u_long loggedLength;		/* bytes in logged array */
    u_int fragmentsPerBlock;
    u_int logFrags;			/* log of fragments per block */
    u_long blocksPerPage;		/* blocks represented by each page */
    u_long bitsLength;			/* bytes in pages' bitmap array */
    u_char *blockSearchArray;		/* info about a byte of bitmap */
    int32 *fragSearchArray;		/* table of free frag group pos */
    u_char *fragCrossArray;		/* frags free at edges of bytes */
    struct freedBlockList freedBlockList;
    u_long blocksFree;			/* number of wholly free blocks,
					 * valid if blocksFreeValid is set */
    u_long fragsFree;			/* number of frags in partial blocks */
    unsigned int blocksFreeValid:1;	/* 1 => blocksFree is valid */
};

/* Now define standard mappings from generic structures to freebBlock and
 * freeTrans structures. */

#define fb_AllocFB(fbl) ((struct freedBlock *)fb_Alloc(&(fbl)->blocks))
#define fb_AllocFT(fbl) ((struct freeTrans *)fb_Alloc(&(fbl)->trans))
#define fb_FreeFB(fbl, fbh) (fb_Free(&(fbl)->blocks, fbh))
#define fb_FreeFT(fbl, fbh) (fb_Free(&(fbl)->trans, fbh))

#define fb_LookupFB(fbl, block) \
    ((struct freedBlock *)fb_Lookup(&(fbl)->blocks, (u_long)block))
#define fb_LookupFT(fbl, trans) \
    ((struct freeTrans *)fb_Lookup(&(fbl)->trans, (u_long)trans))
#define fb_InsertFB(fbl, fb) \
    fb_Insert(&(fbl)->blocks, (struct fb_header *)(fb))
#define fb_InsertFT(fbl, ft) \
    fb_Insert(&(fbl)->trans, (struct fb_header *)(ft))
#define fb_RemoveFB(fbl, fb) \
    fb_Remove(&(fbl)->blocks, (struct fb_header *)(fb))
#define fb_RemoveFT(fbl, ft) \
    fb_Remove(&(fbl)->trans, (struct fb_header *)(ft))

static long AllocatePool(
  struct allocationData *h,		/* allocation handle */
  elbb_tranRec_t trans,			/* transaction identifier */
  epi_anode_t bitmapC,
  daddr_t baseBlock,			/* first block addressed by bitmap */
  u_long nBlocks,			/* number of blocks in bitmap */
  u_int frags,				/* fragments per block */
  u_int nExceptions,			/* length of exception list */
  daddr_t exceptionBlocks[],		/* blocks to exclude from alloc pool */
  int extend				/* grow aggregate? */
);

/* InitFreedBlocks -- initializes freed blocks list structures */

static void
InitFreedBlocks(struct allocationData *h)
{
    struct freedBlockList *fbl = &h->freedBlockList;
    epi_cond_init (&fbl->cond, "freed block list");
    fbl->device = epia_AnodeDevice(h->bitmap);
    fb_Init(sizeof(struct freedBlock), 25, 1<<6, &fbl->blocks);
    fb_Init(sizeof(struct freeTrans), 10, 1<<5, &fbl->trans);
    FIFO_INIT(&fbl->activeTrans, offsetof(struct freeTrans, live));
    FIFO_INIT(&fbl->endedTrans, offsetof(struct freeTrans, live));
}

/*
 * CleanupFreedBlocks -- frees memory allocated for freed blocks list.  This is
 *     called when the allocation handle freed, which happens only when the
 *     bitmap anode is GCed.
 */
static void
CleanupFreedBlocks(struct allocationData *h)
{
    struct freedBlockList *fbl = &h->freedBlockList;
    epi_cond_destroy (&fbl->cond);
    afsl_Assert(fbl->nRunning == 0);
    fb_Cleanup(&fbl->blocks);
    fb_Cleanup(&fbl->trans);
}

/*
 * fb_CheckTrans -- called to deallocate an ended transaction structure if the
 *     transaction has comitted.
 */
static long
fb_CheckTrans(struct freedBlockList *fbl, struct freeTrans *ft, int *changeP)
{
    long code = 0;
    struct freedBlock *fb;
    struct freedBlock *next;

    epix_EnterCodeBlock(53);
    *changeP = 0;

    /* if trans isn't committed we can't change anything */
    if (!elbb_TranIsCommitted(ft_TransOf(ft))) {
	epix_EnterCodeBlock(54);
	return 0;
    }

    /* First unthread trans from ended list */
    afsl_Assert(ft->flags & FB_TRANS_ENDED);
    FIFO_REMOVE(&fbl->endedTrans, &ft->live);

    /* Then walk down the freed blocks and flush each of them. */
    for (fb = (struct freedBlock *)
	FIFO_LookAtOldest(&ft->blocks, offsetof(struct freedBlock, blocks));
	fb != NULL; fb = next) {
	afsl_Assert(fb->trans == ft);
	next = (struct freedBlock *)
	    FIFO_NextOldest(&ft->blocks, fb, &fb->blocks);
	if (fb->flags & FB_BLOCK_KILL) {
	    code = elbb_Kill(fbl->device, fb_BlockOf(fb), 1/*metadataOK*/);
	    if (code)
		return code;
	}
	fb_RemoveFB(fbl, fb);		/* unthread from hash table */
	fb_FreeFB(fbl, fb);
    }

    /* Finally remove and free the trans */
    fb_RemoveFT(fbl, ft);
    fb_FreeFT(fbl, ft);

    /* we changed the hash table so block no longer on list */
    *changeP = 1;
    return 0;
}

/*
 * fb_CommitAllTrans -- make sure all transaction on this aggregate are
 *     committed.  This is only called during shutdown so it is an error to
 *     have any active transactions.
 */
static long
fb_CommitAllTrans(struct allocationData *h)
{
    long code = 0;
    struct freedBlockList *fbl = &h->freedBlockList;
    struct freeTrans *ft;

    afsl_Assert(fbl->nRunning == 0 && IsFifoEmpty(&fbl->activeTrans));
    while (ft = (struct freeTrans *)
	FIFO_LookAtOldest(&fbl->endedTrans,
			  offsetof(struct freeTrans, live))) {
	int change;
	afsl_Assert(ft->flags & FB_TRANS_ENDED);
	code = elbb_LogForce(ft_TransOf(ft), 1);
	if (code == BUF_BADTRAN) code = 0;
	afsl_MBZ(code);

	afsl_Assert(elbb_TranIsCommitted(ft_TransOf(ft)));
	code = fb_CheckTrans(fbl, ft, &change);
	afsl_MBZ(code);
	afsl_Assert(change);
    }
    return 0;
}

#define LBITS (5)			/* log (bits-per-word) */
#define BITS (1 << LBITS)		/* bits-per-word */
/* MASK_FN -- mask of 0<n<=32 bits starting at bit position 0<=f<32. */
#define MASK_FN(f, n) ((((u_int32)-1) >> (BITS-(n))) << (f))

/*
 * AddFrags -- Insert specified sequence of fragments into frags bit array.
 *     These fragments should not already be marked so we panic if they are.
 *     The special value n=0 means mark the whole block.
 */
static void
AddFrags(struct bits64 *frags, u_long first, u_long n)
{
    int w;				/* word counter */

    afsl_Assert(n <= 64 && first + n <= 64);

    if (n == 0) {
	afsl_Assert(first == 0 &&
		    frags->words[0] == 0 && frags->words[1] == 0);
	frags->words[0] = frags->words[1] = -1;
	return;
    }

    w = first >> LBITS;
    first -= w << LBITS;
    while (n > 0) {
	int nn = MIN(n, BITS - first);
	int m = MASK_FN(first, nn);
	afsl_Assert((frags->words[w] & m) == 0); /* bits not yet set */
	frags->words[w] |= m;
	n -= nn;
	first = 0;
	w++;
    }
}

/*
 * InFrags -- Checks to see if a specified sequence of fragments is already
 *     marked in the frags bit array.  The special value n=0 means check the
 *     whole block.  Returns non-zero if any specified are present.
 */
static int
InFrags(struct bits64 *frags, u_long first, u_long n)
{
    int w;				/* word counter */

    afsl_Assert(n <= 64 && first + n <= 64);

    if (n == 0)
	return (frags->words[0] != 0) || (frags->words[1] != 0);

    w = first >> LBITS;
    first -= w << LBITS;
    while (n > 0) {
	int nn = MIN(n, BITS - first);
	int m = MASK_FN(first, nn);
	if ((frags->words[w] & m) != 0)
	    return 1;
	n -= nn;
	first = 0;
	w++;
    }
    return 0;
}

/*
 * IsFreedBlock -- is called to determine if a block being allocated is okay to
 *     return.  Recently freed blocks are not okay if the transaction in which
 *     they were freed has not committed yet.  If the block is very desirable
 *     (e.g. the next one in a consecutive series) the caller can request that
 *     this routine wait if that will help.  If the transaction has ended,
 *     calling LogForce will commit the transaction, which will allow the
 *     desired block to be returned.  Otherwise, there are two possibilities.
 *     Either we cannot wait because the transaction has not ended and waiting
 *     might lead to deadlock, or the caller is willing to pick a different
 *     block and doesn't wish to wait.
 */
static int
IsFreedBlock(
  struct allocationData *h,
  daddr_t block,			/* block to check for */
  u_long firstFrag,
  u_long nFrags,
  int wait)				/* whether to wait or not */
{
    long code = 0;
    struct freedBlockList *fbl = &h->freedBlockList;
    struct freedBlock *fb;
    struct freeTrans *ft;
    int change;

    epix_EnterCodeBlock(55);
    afsl_Assert(tmutex_held(&h->lock));

    fb = fb_LookupFB(fbl, block);
    if (fb == NULL)
	return 0;			/* not on freed block list */

    /*
     * Even tho an fb record exists, if we're checking for a frag group and
     * those frags aren't marked and a kill isn't requested then we can also
     * return "no"
     */
    if (nFrags != 0 && !(fb->flags & FB_BLOCK_KILL) &&
	!InFrags(&fb->frags, firstFrag, nFrags))
	return 0;

    ft = fb->trans;
    if (!(ft->flags & FB_TRANS_ENDED))
	return 1;	/* If not ended, we can't do anything */

    /* Take this opportunity to poll the buffer package to see if the
     * transaction has committed yet.  If so remove its blocks from the hash
     * table.  If this happens block is gone from list. */
    code = fb_CheckTrans(fbl, ft, &change);
    afsl_MBZ(code);
    if (change) {
	epix_EnterCodeBlock(56);
	return 0;
    }

    /* If no change then the trans is ended but not committed */
    if (!wait)
	return 1;		/* doesn't want to wait */

    /* TODO -- It would probably be straightforward to arrange to unlock the
     *     allocation handle while doing this LogForce.  But for now, it is too
     *     much to think about. */
    /* The above statement is not forceful enough.  Due to ECs we can not
     * assume that a transactions has ended just because we called EndTran.  So
     * we cannot safely call LogForce, which now, properly, waits for the
     * transaction to end before forcing the log, while holding any locks at
     * any level.  Since we cannot assert that we are holding no lock, we can
     * not wait.  Thus we must check to see if the transaction has ended before
     * calling LogForce. */
    epix_EnterCodeBlock(59);
    if (elbb_TranStat(ft_TransOf(ft)) != BUF_TRANCOMPLETE) {
	epix_EnterCodeBlock(60);
	return 1;			/* can't wait */
    }
    code = elbb_LogForce(ft_TransOf(ft), 0);
    switch (code) {
      case BUF_BADTRAN:
	code = 0;
	break;
      case BUF_WOULDBLOCK:
	return 1;
      default:
	afsl_MBZ(code);
    }
    afsl_Assert(elbb_TranIsCommitted(ft_TransOf(ft)));
    code = fb_CheckTrans(fbl, ft, &change);
    afsl_MBZ(code);
    afsl_Assert(change);
    return 0;
}

#ifdef AFS_DEBUG
/* for easy kernel patching keep this constant used for assertion checking in a
 * global variable. */
int epib_maxFreedBlocksPerTrans = EPIB_MAX_FREEDBLOCKS_PER_TRANS;
#endif

/*
 * AddFreedBlock -- adds a block to the list of freed blocks for this
 *     transaction.  The transaction must have been started by a call to
 *     epib_StartTran.  If the transaction has not already been used to free
 *     blocks, a new freeTrans block is allocated.  Prior to allocating a new
 *     trans, however, the list of currently active transactions is scanned to
 *     remove ones which have committed.
 *
 * PARAMETERS -- This can be used when freeing whole blocks or fragment groups.
 *     If the whole block is being freed nFrags is set to 0.
 */
static long
AddFreedBlock(
  struct allocationData *h,
  daddr_t block,
  u_long firstFrag,
  u_long nFrags,
  elbb_tranRec_t trans)
{
    long code = 0;
    struct freedBlockList *fbl = &h->freedBlockList;
    struct freedBlock *fb;
    struct freeTrans *ft;
    struct freeTrans *oldft;

    epix_EnterCodeBlock(57);
    if (trans == elbb_zeroTrans)	/* during initialization & salvage */
	return 0;

    afsl_Assert(tmutex_held(&h->lock));
    ft = fb_LookupFT(fbl, trans);
    if (ft == NULL) {
	/*
	 * This is the first free of this transaction.  The limits on
	 * allocating/freeing blocks per transaction should ensure that
	 * allocating an fb record never fails.  We first need to remove
	 * records for committed transactions so we don't stumble on any
	 * left-over records.
	 */
	ft = FIFO_LookAtOldest(&fbl->endedTrans,
				offsetof(struct freeTrans, live));
	while (ft != NULL) {
	    int change;
	    code = fb_CheckTrans(fbl, ft, &change);
	    if (code != 0)
		return code;
	    ft = (change) ?	/* start over if change */
		FIFO_LookAtOldest(&fbl->endedTrans,
				 offsetof(struct freeTrans, live)) :
		FIFO_NextOldest(&fbl->endedTrans, ft, &ft->live);
	}

	ft = fb_AllocFT(fbl);
	afsl_Assert(ft != NULL);
	ft->hdr.key = (u_long)trans;
	fb_InsertFT(fbl, ft);
	FIFO_ADD(&fbl->activeTrans, &ft->live);
	FIFO_INIT(&ft->blocks, offsetof(struct freedBlock, blocks));
    }

    fb = fb_LookupFB(fbl, block);
    if (fb != NULL)
	oldft = fb->trans;
    else {
	/* Check that we're not freeing an illegally large number of blocks in
	 * this transaction.  The limit is arbitrarily set to 10 or big enough
	 * to delete 3 aux containers and one, or perhaps two, volume table
	 * pages.  For example look at the abort path in epif_CreateE. */
#if defined(AFS_DEBUG) && !defined(KERNEL)
	{    u_long lbsize = epia_AnodeDevice(h->bitmap)->logBlkSize;
	     if (ft->nBlocks >= MAX(epib_maxFreedBlocksPerTrans,
				    1 + 3 * (8192 >> lbsize))) {
		 afsl_TraceEnabled(1, 1, AFSL_TR_ALWAYS,
 ("AddFreedBlock: Too many blocks freed (tran:%d, nBlocks:%d, bsize:%dK)",
  trans, ft->nBlocks+1, 1<<(lbsize-10)));
	     }
	}
#endif /* AFS_DEBUG && !KERNEL */
	fb = fb_AllocFB(fbl);
	afsl_Assert(fb != NULL); /* shouldn't fail if res. is working */
	fb->hdr.key = (u_long)block;
	fb_InsertFB(fbl, fb);
	oldft = NULL;
    }

    if (oldft != ft) {
	if (oldft != NULL) {
	    /*
	     * Remove from old transaction thread if any.  For full blocks any
	     * previous transaction should have been committed and hence freed
	     * above.  For fragments the situation is more complex.  Fragments
	     * sharing a block can be freed at different times in different
	     * transactions (these will be ECMerged) but will have different
	     * tranIds.
	     */
	    FIFO_REMOVE(&oldft->blocks, &fb->blocks);
	    oldft->nBlocks--;
	}
	/* if not already on the list for this transaction */
	FIFO_ADD(&ft->blocks, &fb->blocks);
	fb->trans = ft;
	ft->nBlocks++;
    } /* else already on same transaction */

    AddFrags(&fb->frags, firstFrag, nFrags);
    return 0;
}

/*
 * KillFreedBlock -- marks a block as wholly free and which should be killed
 *     when the current transaction is committed.  This is necessary to allow
 *     the buffer package to change the type of a block from logged to unlogged
 *     or the reverse.
 *
 * PARAMETERS -- We pass trans only so we can punt if trans is zero.
 */
static void KillFreedBlock(
  struct allocationData *h,
  daddr_t block,
  elbb_tranRec_t trans)		/* salvaging? */
{
    struct freedBlock *fb;

    epix_EnterCodeBlock(67);
    if (trans == elbb_zeroTrans)	/* during initialization & salvage */
	return;
    fb = fb_LookupFB(&h->freedBlockList, block);
    afsl_Assert(fb != NULL);
    fb->flags |= FB_BLOCK_KILL;
}

#define POWER_OF_TWO(x) (((x) & ((x) - 1)) == 0)

#define HANDLE_OK(h) \
    ((h) != NULL && \
     (h)->bitmap != NULL && (h)->length != 0 && \
     (h)->fragmentsPerBlock > 0 && \
     (h)->fragmentsPerBlock <= EPIB_MAX_FRAGMENTS_PER_BLOCK && \
     (h)->logFrags <= 6 && (h)->blocksPerPage != 0 && \
     (h)->loggedLength == ((h)->fragmentsPerBlock == 1) ? \
			    0 : (h)->blocksPerPage / 8 && \
     ((h)->blocksPerPage << (h)->logFrags) / 8 == (h)->bitsLength)

/*
 * InitHandle -- formats an allocation handle given a bitmap container and the
 *     contents of the first page.
 *
 * LOCKS
 *	Assumes caller has grabbed allocation handle mutex.
 * PARAMETERS -- It does not assume that the handle is in allocated storage or
 *     attempt to attach it to the container's rock.  This is because during
 *     bitmap creation a stack allocated handle is used temporarily.
 */
static long
InitHandle(
  epi_anode_t bitmapC,
  struct bitmapPage *firstPage,
  struct allocationData *h)
{
    long code = 0;
    struct epia_info info;
    int i, byte, maxSize;
    u_long length;			/* 32 bits of container length */
    async_device_t *device = epia_AnodeDevice(bitmapC);

    epix_EnterCodeBlock(1);
    afsl_Assert (tmutex_held(&h->lock));
    code = epia_GetInfo (bitmapC, &info);
    if (code) {
	afsl_Trace (AFSL_TR_ERRMAP,
 ("Init(allocation)Handle: got code %u returning NOTABITMAP", code));
	return EPI_E_NOTABITMAP;
    }
    if (info.statusSize != 0 || info.backingIndex != 0)
	return EPI_E_NOTABITMAP;
    if (!AFS_hfitsinu32(info.length))
	return EPI_E_NOTABITMAP;
    length = AFS_hgetlo(info.length);
    if ((length & (device->blkSize - 1)) != 0)
	return EPI_E_NOTABITMAP;

    h->bitmap = bitmapC;
    h->base = firstPage->baseOffset;

    if ((length >> device->logBlkSize) * firstPage->blocksPerPage
	< firstPage->bitmapLength)
	return EPI_E_NOTABITMAP;
    h->length = firstPage->bitmapLength;

    h->fragmentsPerBlock = firstPage->fragmentsPerBlock;
    if (h->fragmentsPerBlock != device->blkSize >> device->logFragSize)
	return EPI_E_NOTABITMAP;
    if (!(h->fragmentsPerBlock >= 1 &&
	  h->fragmentsPerBlock <= EPIB_MAX_FRAGMENTS_PER_BLOCK))
	return EPI_E_NOTABITMAP;
    if (!POWER_OF_TWO(h->fragmentsPerBlock))
	return EPI_E_NOTABITMAP;
    h->logFrags = device->logBlkSize - device->logFragSize;

    h->blocksPerPage = firstPage->blocksPerPage;
    if ((h->blocksPerPage & 7) != 0)
	return EPI_E_NOTABITMAP;
    h->loggedLength = (h->fragmentsPerBlock == 1) ?
			0 : h->blocksPerPage / 8;
    h->bitsLength = (h->blocksPerPage / 8) * h->fragmentsPerBlock;

    h->blocksFreeValid = 0;		/* not determined yet */
    h->fragsFree = 0;

    /*
     * Calculate fragSearchArray.  This table is indexed by each possible
     * pattern of eight bits and is intended to let us deduce quickly from
     * a byte of bitmap information whether it contains a free fragment
     * group of a given size.  Each entry is a 32 bit value composed of
     * eight 4-bit fields.  The least significant nibble contains the
     * field describing the availability of fragment groups of size 1.
     * The most significant nibble is for groups of size 8.  The high bit
     * (0x8) of each field is set if a group of the corresponding size
     * is free and the low three bits provide its position (0-7) in
     * the byte.  The position always specifies the first available group
     * in the byte.  If the high bit is not set, the whole field will be
     * zero.
     */
    epix_EnterCodeBlock(2);
    h->fragSearchArray = osi_Alloc_r(256 * sizeof(int32));
    maxSize = MIN(h->fragmentsPerBlock, 8);
    for (byte = 0; byte < 256; byte++) {
	int size;
	int32 word = 0;
	for (size = 1; size <= maxSize; size++) {
	    int mask = (1 << size) - 1;
	    int found = 0, blkStart, pos;
	    for (blkStart = 0; blkStart != 8 && !found; blkStart += maxSize) {
		for (pos = blkStart; pos + size <= blkStart + maxSize; pos++) {
		    found = ((byte & (mask << pos)) == 0);
		    if (found)	/* this frag is free */
			break;
		}
	    }
	    if (found)
		word |= (pos | 0x8) << ((size - 1) * 4);
	    else
		break;	/* no larger sizes will fit either */
	}
	h->fragSearchArray[byte] = word;
    }
    if (h->fragmentsPerBlock > 8) {
	/*
	 * Calculate fragCrossArray.  Since, for block sizes larger than 8
	 * fragments, a frag group may cross bitmap byte boundaries, we need a
	 * way to search for frag groups in multiple bitmap bytes.  This array
	 * gives the number of free fragments at the edges of a byte.  The low
	 * nibble gives the number of free fragments at the beginning and the
	 * high 4 bits gives the number free at the end.  For example, the
	 * value for (byte==0) is 0x88, and for (byte==0x10) is 0x34.
	 */
	epix_EnterCodeBlock(3);
	h->fragCrossArray = osi_Alloc_r(256);
	for (byte = 0; byte < 256; byte++) {
	    int lofree, hifree;
	    int lomask, himask;
	    lomask = 1;
	    himask = 0xff << 7;
	    lofree = hifree = 0;
	    for (i = 1; i <= 8; i++) {
		if ((byte & lomask) == 0)
		    lofree++;
		if ((byte & himask) == 0)
		    hifree++;
		lomask = (lomask << 1) | 1;
		himask >>= 1;
	    }
	    h->fragCrossArray[byte] = (hifree << 4) | lofree;
	}
    }

    InitFreedBlocks(h);

    /* Init random seed */
    for (i = 0; i < h->loggedLength; i++)
	NewRandom(firstPage->logged[i]);
    for (i = 0; i < h->bitsLength; i++)
	NewRandom(BitsArray(firstPage, h)[i]);

    return 0;
}

/*
 * CleanupHandle -- freed the allocated storage associated with an allocation
 *     handle.  It is called by Cleanup and by Create.
 */
static void CleanupHandle(struct allocationData *h)
{
    h->bitmap = 0;
    if (h->fragSearchArray != NULL)
	osi_Free_r(h->fragSearchArray, sizeof (int32) * 256);
    h->fragSearchArray = 0;
    if (h->blockSearchArray != NULL)
	osi_Free_r(h->blockSearchArray, 256);
    h->blockSearchArray = 0;
    if (h->fragCrossArray)
	osi_Free_r(h->fragCrossArray, 256);
    h->fragCrossArray = 0;
    CleanupFreedBlocks(h);
}

/* set h to good looking bitmap handle from bitmapC or zero. */

#define GetOKbitmap(h, bitmapC) \
    (bitmapC != NULL && \
     (h = (struct allocationData *)epia_GetVolumeRock(bitmapC)) != NULL && \
      HANDLE_OK(h))

/* Cleanup -- deallocates an allocation handle when the bitmap container is
 *     GCed after the aggregate is closed. */

static void Cleanup(epi_anode_t bitmapC)
{
    struct allocationData *h;
    epix_EnterCodeBlock(4);
    if (GetOKbitmap(h, bitmapC)) {
	CleanupHandle(h);
	tmutex_Destroy(&h->lock);
	osi_Free_r(h, sizeof(*h));
    }
    epia_SetVolumeRock(bitmapC, 0, 0);
}

#if !defined(KERNEL) || (AFSL_DEBUG_LEVEL > 3)

/* Since the "real" limit on started transactions is per aggregate we need to
 * allow for remembering transactions for the product of the two.
 *
 * CAUTIONS -- This number for max aggregates is entirely arbitrary and may
 *     cause spurious panics in systems with this debugging enabled running
 *     highly parallel tests on more aggregates than this. */

#define FB_MAX_AGGREGATES 10
#define FB_MAX_THREADTRANS (FB_MAX_STARTED_TRANS * FB_MAX_AGGREGATES)
#if (FB_MAX_THREADTRANS > 100)
#undef FB_MAX_THREADTRANS
#define FB_MAX_THREADTRANS 100
#endif

static osi_dlock_t threadTableLock;
static void *threadsWithActiveTrans[FB_MAX_THREADTRANS];

static void RememberThreadTrans(void)
{
    int i;
    void *thisThread = (void *)osi_ThreadUnique();
    int slot_used = -1;

    BEGIN_LOCK_WRITE(&threadTableLock) {
	for (i = 0; i < FB_MAX_THREADTRANS; i++) {
	    if (threadsWithActiveTrans[i] == 0 && slot_used == -1) {
		threadsWithActiveTrans[i] = thisThread;
		slot_used = i;
	    } else {
		afsl_Assert((thisThread != threadsWithActiveTrans[i]));
	    }
	}
    } END_LOCK;
    afsl_Assert(slot_used >= 0 || osi_IsKernelPanic());
}

static void ForgetThreadTrans(void)
{
    int i;
    void *thisThread = (void *)osi_ThreadUnique();

    BEGIN_LOCK_WRITE(&threadTableLock) {
	for (i = 0; i < FB_MAX_THREADTRANS; i++)
	if (threadsWithActiveTrans[i] == thisThread) {
	    threadsWithActiveTrans[i] = 0;
	    break;
	}
    } END_LOCK;
    afsl_Assert(i != FB_MAX_THREADTRANS || osi_IsKernelPanic());
}

#define InitThreadTrans() (lock_InitDesc(&threadTableLock, "thread table"))

#else
#define RememberThreadTrans()
#define ForgetThreadTrans()
#define InitThreadTrans()
#endif

/* epib_Init -- Initializes the block allocation module.  It is called only
 *     once during system initialization by \fcname{epig_Init}, see which. */

/* EXPORT */
long epib_Init(void)
{
    static inited = 0;
    if (inited)
	return 0;
    nBlockAllocates = 0;
    randomSeed = 0;
    InitThreadTrans();
#ifdef AFS_DEBUG
    /* CodeBlock: epia */
    epia_codeBlock[DEBUG_THIS_FILE][0] = 69, epia_codeBlock[DEBUG_THIS_FILE][26] = -1, epia_codeBlock[DEBUG_THIS_FILE][63] = -1, epia_codeBlock[DEBUG_THIS_FILE][64] = -1, epia_codeBlock[DEBUG_THIS_FILE][65] = -1, epia_codeBlock[DEBUG_THIS_FILE][66] = -1;
#endif
    inited++;
    return 0;
}

/* epib_StartTran -- Starts a transaction, whose identifier is returned in
 *     \arg{transP}, that might be used to free blocks.  Generally this
 *     means all transactions used by the anode and vnode layers.  It limits
 *     the number of transactions that might attempt to free blocks
 *     simultaneously and thus limits the storage required by the freed block
 *     list.  This limit should be a configuration parameter.  This calls the
 *     logbuf layer procedure \fcname{elbb_StartTran} to do most of the work.
 *     The \arg{serviceName} parameter is used for diagnostic purposes and
 *     is passed directly to \fcname{elbb_StartTran}.  See also the companion
 *     function \fcname{epib_EndTran} which must be used to end this
 *     transaction. */

/* SHARED */
long epib_StartTran(
  char *serviceName,
  epi_anode_t anode,
  elbb_tranRec_t *transP)
{
    long code = 0;
    struct allocationData *h;
    struct freedBlockList *fbl;

    h = (struct allocationData *)epia_GetAllocationHandle(anode);
    afsl_AssertCond(HANDLE_OK(h));
    fbl = &h->freedBlockList;

    BEGIN_TMUTEX_NT(&h->lock) {
	while (fbl->nRunning >= FB_MAX_STARTED_TRANS &&
		!osi_IsKernelPanic()) {
	    epix_EnterCodeBlock(58);
	    fbl->waiters++;
	    afsl_Trace(AFSL_TR_SLEEP,
			("epib_StartTran: sleeping for transaction to end"));
	    tmutex_cond_wait (&fbl->cond, &h->lock);
	    fbl->waiters--;
	}
	fbl->nRunning++;
	RememberThreadTrans();		/* check for one trans/thread */
    } END_LOCK;
    code = elbb_StartTran(serviceName, 0, epia_AnodeDevice(anode), transP);
    afsl_MBZ(code);				/* otherwise nRunning is too large */
    return 0;
}

/* epib_EndTran -- Ends the transaction \arg{trans} which must have been
 *     obtained from \fcn{epib_StartTran}.  Any threads waiting for a
 *     transaction to end are sent wakeups.  If the transaction was used to
 *     free blocks or fragments they are marked for release (which actually
 *     happens when the transaction commits).  This calls logbuf layer
 *     procedures to do most of the work. */

/* SHARED */
long epib_EndTran(
  epi_anode_t anode,
  elbb_tranRec_t trans)
{
    long code = 0;
    struct allocationData *h;
    struct freedBlockList *fbl;

    h = (struct allocationData *)epia_GetAllocationHandle(anode);
    afsl_Assert(anode->refCount > 0 && HANDLE_OK(h));
    fbl = &h->freedBlockList;

    /* As a debugging aid, provide a way to defer transaction ends
     * indefinitely.  To do this we starts another transaction associated with
     * the freed block structure (i.e. one per aggregate).  Every regular
     * transaction is merged with this one so that none reach the completed
     * state.  Turning off the bit will end the special transaction next time
     * epib_EndTran is called.
     *
     * CAUTIONS -- This violates the basic rule that a single thread will not
     *     start more than one (unnested) transaction.  We do this explicitly
     *     since we want to cause a deadlock, but the log must be big enough to
     *     start two concurrent transaction or this call to elbl_StartTran will
     *     not return. */

    if (afsdb_episode_anode & (1 << (EPIA_DEBUG_DEFERTRANS - 1))) {
	if (!fbl->deferredTrans) {
	    BEGIN_TMUTEX_NT(&h->lock) {
		if (!fbl->deferredTrans) { /* check for race */
		    afsl_TraceEnabled(1, 1, AFSL_TR_ALWAYS,
					("epib_EndTran: Starting defer trans"));
		    code = elbb_StartTran("defer trans", 0,
					     epia_AnodeDevice(anode),
					     &fbl->deferredTrans);
		    afsl_MBZ(code);
		    afsl_TraceEnabled(1, 1, AFSL_TR_ALWAYS,
					("epib_EndTran: Defer trans is %d",
					fbl->deferredTrans));
		}
	    } END_LOCK;
	}
	elbb_EquivTran(fbl->deferredTrans, trans, 0);
    } else if (fbl->deferredTrans) {
	/* cleanup after turning bit off */
	BEGIN_TMUTEX_NT(&h->lock) {
	    if (fbl->deferredTrans) {	/* check for race */
		afsl_TraceEnabled(1, 1, AFSL_TR_ALWAYS,
				   ("epib_EndTran: Ending deferred trans %d",
				    fbl->deferredTrans));
		elbb_EndTran(fbl->deferredTrans);
	    }
	    fbl->deferredTrans = 0;
	} END_LOCK;
    }

    elbb_EndTran(trans);

    BEGIN_TMUTEX_NT(&h->lock) {
	struct freeTrans *ft;

	ForgetThreadTrans();
	afsl_Assert(fbl->nRunning > 0);
	fbl->nRunning--;		/* one fewer */
	if (fbl->waiters > 0) {
	    epi_cond_signal (&fbl->cond);
	}
	ft = fb_LookupFT(fbl, trans);
	if (ft == NULL)
	     EXIT_CRIT;		/* no blocks freed in this trans */
	ft->flags |= FB_TRANS_ENDED;
	FIFO_REMOVE(&fbl->activeTrans, &ft->live);
	FIFO_ADD(&fbl->endedTrans, &ft->live);
    } END_LOCK;
    return 0;
}

/* bitmap length enforced only on first and last pages */
#define BITMAP_PAGE_LENGTH_OK(page, h) \
    (!((page)->baseOffset == (h)->base || \
     (page)->baseOffset + (page)->blocksPerPage >= (h)->base + (h)->length) || \
     (page)->bitmapLength == (h)->length)

#define BITMAP_PAGE_MAGIC_OK(page, bsize) \
    ((page)->initialStamp == \
      *(int32 *)((char *)(page) - sizeof(int32) + bsize) && \
     (page)->magic == BITMAP_MAGIC)

#define BITMAP_PAGE_OK(page, h) \
    (BITMAP_PAGE_MAGIC_OK(page, epia_AnodeDevice((h)->bitmap)->blkSize) && \
     ((page)->baseOffset - (h)->base) % (h)->blocksPerPage == 0 && \
     (page)->blocksPerPage <= (h)->blocksPerPage && \
     (page)->fragmentsPerBlock == (h)->fragmentsPerBlock && \
     BITMAP_PAGE_LENGTH_OK(page, h))

static void FormatPage(
  struct bitmapPage *page,	/* in core bitmap page */
  long bsize,			/* size of page in bytes */
  daddr_t base,			/* starting block addr on page */
  u_long blocksPerPage,		/* blocks rep. by this page */
  u_long bitmapLength,		/* blocks rep. whole bitmap */
  u_int frags)			/* fragments per block */
{
    u_long loggedLength;	/* size of logged array */

    epix_EnterCodeBlock(32);
    page->initialStamp = (int32)page;
    page->magic = BITMAP_MAGIC;
    page->baseOffset = base;
    page->blocksPerPage = blocksPerPage;
    page->bitmapLength = bitmapLength;
    page->fragmentsPerBlock = frags;
    page->spare32 = page->spare16 = 0;
    *(int32 *)((char *)page + bsize - sizeof(int32)) = page->initialStamp;
    loggedLength = (frags == 1) ? 0 : blocksPerPage / 8;
    if (loggedLength != 0)
	bzero((caddr_t)page->logged, loggedLength);
    bzero((caddr_t)page->logged + loggedLength,
		(blocksPerPage * frags + 7) / 8);
}

/*
 * IsWholeBlockFree -- checks a bits array to see if the whole block containing
 *     the specified fragment is free.
 *
 * PARAMETERS -- this function is written to take the address of the bits array
 *     so it can also be called from VerifyBitmap.
 */
static int
IsWholeBlockFree(
  struct allocationData *h,
  u_char *bits,				/* bits array */
  u_long firstFrag)			/* frag addr within bits map */
{
    u_long base = BaseFrag(h, firstFrag);
    u_long byte = base >> 3;
    if (h->fragmentsPerBlock <= 8) {
	/* See if whole block at base of firstFrag free */
	u_char b = bits[byte];
	u_int firstBit = base & 7;
	u_char blockMask = ((1 << h->fragmentsPerBlock) - 1) << firstBit;

	epix_EnterCodeBlock(33);
	return ((b & blockMask) == 0);
    } else { /* multi-bytes per block */
	int bytesPerBlock = h->fragmentsPerBlock / 8;
	int i;
	epix_EnterCodeBlock(34);
	for (i = 0; i < bytesPerBlock; i++) {
	    if (bits[byte + i])
		return 0;
	}
	return 1;
    }
}

/*
 * IsBlockPartiallyFree -- checks a bits array to see if the block containing
 * the specified fragment is partially free, i.e., neither entirely free nor
 * entirely in-use.
 */
static int
IsBlockPartiallyFree(
  struct allocationData *h,
  u_char *bits,			/* bits array */
  u_long firstFrag)		/* frag addr within bits map */
{
    u_long base = BaseFrag(h, firstFrag);
    u_long byte = base >> 3;
    if (h->fragmentsPerBlock <= 8) {
	/* See if whole block at base of firstFrag free */
	u_char b = bits[byte];
	u_int firstBit = base & 7;
	u_char blockMask = ((1 << h->fragmentsPerBlock) - 1) << firstBit;

	epix_EnterCodeBlock(68);
	return ((b & blockMask) != 0 && (b & blockMask) != blockMask);
    } else { /* multi-bytes per block */
	int bytesPerBlock = h->fragmentsPerBlock / 8;
	int i, empty, full;		/* track both */
	epix_EnterCodeBlock(69);
	full = empty = 1;
	for (i = 0; i < bytesPerBlock; i++) {
	    u_char b = bits[byte + i];
	    if (b == 0)
		full = 0;
	    else if (b == 0xff)
		empty = 0;
	    else
		return 1;
	}
	return (!empty && !full);
    }
}

#define ALLOC 1
#ifdef FREE
#undef FREE
#endif /* FREE */
#define FREE 0

/*
 * MarkFrags -- sets the bits corresponding to a range of fragments in a page.
 *     These fragments never span a page boundary.
 *
 * PARAMETERS -- logged indicates whether the fragments are for a logged
 *     container.  This parameter is only consulted if nFrags is less than a
 *     full block.  When frags are being freed it is used ONLY in assertions.
 *     A value of -1 means unknown.  A value of -2 means a formerly whole block
 *     is being partially deallocated.
 *
 *     The wholeBlockFreeP parameter, in the case of alloc, means that the
 *     block was free before this operation is performed, and, in the case of
 *     free, it means that these are the last frags in the block.  It is set
 *     (to zero or one) if the caller as already evaluated this function, and
 *     -1 (or -2) otherwise.  Since FindFrags returns this parameter, this
 *     parameter is set when called from AllocateFrags and not otherwise.  We
 *     use this parameter for two purposes: 1) to set the logged bitmap if
 *     we're allocating the first frags in an free block (or freeing part of a
 *     block; this is the -2 case) for a logged container, and 2) to track the
 *     space usage of the aggregate.  If we have to evaluate this we return the
 *     result.
 */
static long
MarkFrags (
  elbb_tranRec_t trans,
  struct elbb_buffer **bufP,
  struct bitmapPage *page,
  struct allocationData *h,
  int logged,			/* frags are for logged container */
  u_long firstFrag,
  u_long nFrags,
  int alloc,			/* whether to alloc or free bits */
  int *wholeBlockFreeP)		/* is frag group in free block */
{
    int firstBit = firstFrag & 0x7;
    u_int firstByte = firstFrag >> 3;
    u_long lastFrag = firstFrag + nFrags - 1;
    int lastBit = lastFrag & 0x7;
    u_int lastByte = lastFrag >> 3;
    int mask, i;
    long code;
    u_char value;
    int log;
    int mbz = 0;

    static int startMask[8] = {0xff, 0xfe, 0xfc, 0xf8, 0xf0, 0xe0, 0xc0, 0x80};
    static int   endMask[8] = {0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff};

    epix_EnterCodeBlock(5);
    afsl_Assert (tmutex_held(&h->lock));
    log = !elbb_IsZeroTrans(trans);
    if (nFrags == 0)
	return 0;
    afsl_Assert(bufP == 0 ||
		(opaque)elbb_BufferData(*bufP) == (opaque)page);
    afsl_Assert(firstByte <= lastByte &&	/* make sure no wrapping */
		lastByte < h->bitsLength);

#define SET_BITMAP_BYTE(which, i, value) \
    afsl_Assert((page->logged + (which))[i] != (value)); \
    if (bufP) { \
	code = elbb_Modify \
	    (*bufP, (offsetof(struct bitmapPage, logged[0])+ (which) + (i)), \
	     (caddr_t)&(value), 1/*byte*/, trans, log, bufP); \
	if (mbz++) \
	    afsl_MBZ(code); \
	else if (code != 0) \
	    return code; \
    } else { \
	(page->logged + (which))[i] = (value); \
    }
#define SET_BITS_BYTE(i, value) SET_BITMAP_BYTE(h->loggedLength, i, value)
#define SET_LOGGED_BYTE(i, value) SET_BITMAP_BYTE(0, i, value)

    value = (alloc) ? 0xff : 0;
    for (i = firstByte + 1; i < lastByte; i++) {
	SET_BITS_BYTE(i, value);
    }

    /* Optimize to handle (firstBit==0) || (lastBit==7) in above loop? */
    mask = startMask[firstBit];
    if (firstByte == lastByte)
	mask &= endMask[lastBit];
    value = BitsArray(page, h)[firstByte];
    if (alloc)
	value |= mask;
    else
	value &= ~mask;
    SET_BITS_BYTE(firstByte, value);
    if (firstByte != lastByte) {
	epix_EnterCodeBlock(8);
	value = BitsArray(page, h)[lastByte];
	if (alloc)
	    value |= endMask[lastBit];
	else
	    value &= ~endMask[lastBit];
	SET_BITS_BYTE(lastByte, value);
    }
    if (nFrags < h->fragmentsPerBlock) {
	u_long block = firstFrag >> h->logFrags;
	u_long lbyte = block >> 3;	/* index into logged array */
	u_char lmask = 1 << (block & 7); /* mask for bit in byte */
	u_char logByte = page->logged[lbyte];
	int isLogged = (logByte & lmask) != 0;

	afsl_Assert(h->fragmentsPerBlock > 1 &&
		     (logged == 0 || logged == 1) &&
		     /* if alloc then must be set else may not be set */
		     (!alloc || *wholeBlockFreeP != -1));

	if (alloc) {
	    epix_EnterCodeBlock(35);
	    if (*wholeBlockFreeP) {
		/* first use of partial block */
		if (logged != isLogged) {
		    /* need to set it to the value to match this frag group */
		    u_char newByte;
		    epix_EnterCodeBlock(36);
		    newByte = (logged) ? (logByte|lmask) : (logByte & ~lmask);
		    SET_LOGGED_BYTE(lbyte, newByte);
		    isLogged = logged;
		}
	    }
	    if (h->blocksFreeValid) {
		if (*wholeBlockFreeP) {
		    h->blocksFree--;
		    h->fragsFree += h->fragmentsPerBlock - nFrags;
		} else
		    h->fragsFree -= nFrags;
	    }
	} else { /* free */
	    if (*wholeBlockFreeP == -2) {
		/*
		 * Block is becoming partially free so set logged map to the
		 * correct value for this frag group.  But block will not be
		 * free so set wholeBlockFree to more conventional boolean
		 * value.
		 */

		epix_EnterCodeBlock(6);
		*wholeBlockFreeP = 0;
		if (logged != isLogged) {
		    u_char newByte;
		    newByte = (logged ? (logByte|lmask) : (logByte & ~lmask));
		    SET_LOGGED_BYTE(lbyte, newByte);
		    isLogged = logged;
		}
	    }
	    if (h->blocksFreeValid) {
		epix_EnterCodeBlock(31);
		if (*wholeBlockFreeP == -1)
		    *wholeBlockFreeP =
			IsWholeBlockFree(h, BitsArray(page, h), firstFrag);
		if (*wholeBlockFreeP) {
		    h->blocksFree++;
		    h->fragsFree -= h->fragmentsPerBlock - nFrags;
		} else
		    h->fragsFree += nFrags;
	    }
	}
	afsl_Assert(logged == isLogged);
    } else { /* whole block(s) */
	if (h->blocksFreeValid) {
	    int nBlocks = nFrags >> h->logFrags;
	    afsl_Assert((nFrags & (h->fragmentsPerBlock - 1)) == 0);
	    if (alloc)
		nBlocks = -nBlocks;
	    h->blocksFree += nBlocks;
	}
	*wholeBlockFreeP = 1;		/* (cheap) in case caller cares */
    }
    return 0;
}

static long
MarkBlock(
  elbb_tranRec_t trans,
  struct elbb_buffer **bufP,
  struct bitmapPage *page,
  struct allocationData *h,
  daddr_t eb,
  int alloc)				/* whether to alloc or free bits */
{
    long code;
    long blocks = h->blocksPerPage;
    long fragInPage;
    int wholeBlockFree = -1;

    epix_EnterCodeBlock(37);
    afsl_Assert(eb >= page->baseOffset && eb < page->baseOffset + blocks);
    fragInPage = (eb - page->baseOffset) * page->fragmentsPerBlock;
    code = MarkFrags(trans, bufP, page, h, /*logged*/-1,
		     fragInPage, page->fragmentsPerBlock,
		     alloc, &wholeBlockFree);
    return code;
}

/*
 * FindBlock -- locates an available block within a single page and returns its
 *     block number corrected for the page baseOffset.
 *
 * PARAMETERS -- The hint parameter provides a page relative block number at
 *     which to start the search.
 */
static daddr_t
FindBlock(
  struct bitmapPage *page,
  struct allocationData *h,
  struct allocationHint *hint)
{
    daddr_t start;
    long nBytes = h->bitsLength;
    int bytesPerBlock;			/* bytes of bits for whole block */
    long bit = -1;			/* fragment in page */
    long i, j;
    daddr_t block;

    epix_EnterCodeBlock(38);
    afsl_Assert(tmutex_held(&h->lock));
    if (hint == NULL)
	start = 0;
    else {
	start = hint->start;
	afsl_Assert(start < h->blocksPerPage);
    }

    if (h->fragmentsPerBlock < 8) {
	int32 *f = h->fragSearchArray;
	int shift = (page->fragmentsPerBlock - 1) * 4;
	int bits, found;
	long firstByte = 0;
	int skipBits = 0;		/* mask of bits to ignore */

	/* special hacking to start in the middle of a byte */
	if (start != 0) {
	    int firstBit;		/* first bit in firstByte to check */
	    epix_EnterCodeBlock(9);
	    firstByte = start >> (3 - h->logFrags);
	    firstBit = (start & ((1 << (3 - h->logFrags)) - 1)) << h->logFrags;
	    skipBits = (1 << firstBit) - 1;
	}

	epix_EnterCodeBlock(10);
	for (i = firstByte; i < nBytes && bit == -1; i++) {
	    bits = BitsArray(page, h)[i];
	    if (i == firstByte)
		bits |= skipBits;
	    MarkFragmentCoverage(h->fragmentsPerBlock,
				 h->fragmentsPerBlock, bits);
	    found = (f[bits] >> shift) & 0xf;
	    if (found)
		bit = i * 8 + (found & 7);
	}
    } else { /* h->fragmentsPerBlock >= 8 */
	epix_EnterCodeBlock(11);
	bytesPerBlock = h->fragmentsPerBlock >> 3;
	/* translate starting block to starting byte index */
	start *= bytesPerBlock;
	for (i = start; i < nBytes && bit == -1; i += bytesPerBlock) {
	    for (j = 0; j < bytesPerBlock; j++) {
		if (BitsArray(page, h)[i + j] != 0)
		    break;
	    }
	    if (j == bytesPerBlock)	/* found free block */
		bit = i * 8;
	}
    }
    if (bit == -1)
	return EPIX_ANODE_BLOCKEMPTY;

    block = page->baseOffset + (bit >> h->logFrags);
    nBlockAllocates++;
    return (block);
}

static int IsLogged(
  struct bitmapPage *page,
  struct allocationData *h,
  u_long firstFrag)
{
    u_long block = firstFrag >> h->logFrags;
    int logged;
    if (h->logFrags == 0)
	return (-1);
    logged = page->logged[block >> 3] & (1<< (block & 7));
    return (logged != 0);
}

/*
 * FindFrags -- locates a group of contiguous sequence of free fragments of at
 *     least the desired length.  There are three main cases to consider:
 *
 *     1. (fragmentsPerBlock <= 8); by implication nFrags is also eight or less
 *	and any group must fit in a byte.
 *     2. (fragmentsPerBlock > 8) and (nFrags <= 8); thus admitting the
 *	possibility of a group crossing.
 *     3. (nFrags > 8); all groups must cross a byte boundary.
 *
 * PARAMETERS -- The hint parameter provides a page relative fragment number at
 *     which to start the search.  If this indicates a point past the end of
 *     the page we return -1 immediately.
 */
static u_long
FindFrags(
  struct bitmapPage *page,
  struct allocationData *h,
  int logged,			/* for a logged fragment */
  u_int nFrags,
  struct allocationHint *hint,
  int *wholeBlockFreeP)		/* state of surrounding block */
{
    u_long start;			/* first frag to start with */
    int32 *f = h->fragSearchArray;
    u_char *e = h->fragCrossArray;
    long nBytes = h->bitsLength;
    u_long firstFrag;			/* fragment in page */
    u_long firstByte;			/* first byte of page to search */
    u_int firstBit;			/* first bit if first byte */
    int shift;				/* amount to shift fragSearch by */
    int bits;				/* bitmap byte adj. for firstBit!=0 */
    long i;
    int found;				/* bits past "i" */
    int trailingEdge = 0;		/* bits free at end of byte */
    int empty;

    afsl_Assert(tmutex_held(&h->lock) &&
		(nFrags <= page->fragmentsPerBlock));
    if (wholeBlockFreeP)
	*wholeBlockFreeP = -1;

    if (hint == NULL)
	start = firstByte = firstBit = 0;
    else {
	start = (u_long)hint->start;
	if (start >= h->blocksPerPage * h->fragmentsPerBlock) {
	    /* we're off this page so return not-found */
	    epix_EnterCodeBlock(13);
	    return (u_long)-1;
	}
	firstByte = start >> 3;
	firstBit = start & 7;
    }

    shift = (nFrags <= 8) ? (nFrags - 1) * 4 : 0;

    i = firstByte;
    bits = BitsArray(page, h)[i];
    trailingEdge = 0;
    if (firstBit != 0) {
	epix_EnterCodeBlock(12);
	bits |= (1 << firstBit) - 1;
    }

    firstFrag = (u_long)-1;		/* -1 means none found */

    if (h->fragmentsPerBlock <= 8) {
	int blockMask;			/* mask of frags in block */

	epix_EnterCodeBlock(61);
	blockMask = (1 << h->fragmentsPerBlock) - 1;
	while (i < nBytes) {
	    MarkFragmentCoverage(h->fragmentsPerBlock, nFrags, bits);
	    found = (f[bits] >> shift) & 0xf;
	    if (found) {
		firstFrag = i * 8 + (found & 7);
		/* if whole block is also free but logged-ness doesn't match,
		 * then skip to next block. */
		empty = IsWholeBlockFree(h, BitsArray(page, h), firstFrag);
		if (!empty && (IsLogged(page, h, firstFrag) != logged)) {
		    /* mask out all of current block, and go around again */
		    epix_EnterCodeBlock(14);
		    bits |= blockMask << BaseFrag(h, found & 7);
		    firstFrag = (u_long)-1;
		    continue;		/* not found after all */
		}
		break;
	    }
	    bits = BitsArray(page, h)[++i];	/* try next byte */
	}
    } else {
	/* more than one bitmap byte per block */
	int blockMask;			/* mask on intra-block byte index */

	epix_EnterCodeBlock(39);
	blockMask = (h->fragmentsPerBlock >> 3) - 1;
	empty = 1;
	while (i < nBytes) {
	    MarkFragmentCoverage(h->fragmentsPerBlock, nFrags, bits);
	    if (trailingEdge + (e[bits] & 0xf) >= nFrags) {
					/* group begins in previous byte */
		epix_EnterCodeBlock(15);
		afsl_Assert(i * 8 >= trailingEdge); /* must not go negative */
		firstFrag = i * 8 - trailingEdge;
	    } else if (nFrags <= 8) {
		found = (f[bits] >> shift) & 0xf;
		if (found) {
		    epix_EnterCodeBlock(16);
		    firstFrag = i * 8 + (found & 7);
		}
	    }
	    if (firstFrag != (u_long)-1) {
		if (empty)
		    empty = IsWholeBlockFree(h, BitsArray(page, h), firstFrag);
		if (!empty && (IsLogged(page, h, firstFrag) != logged)) {
		    epix_EnterCodeBlock(40);
		    i |= blockMask;	/* skip rest of this block */
		} else
		    break;
		firstFrag = (u_long)-1;	/* not found after all */
	    }

	    /* trailingEdge links bytes within a single block. */

	    if ((i & blockMask) == blockMask) { /* next block */
		trailingEdge = 0;
		empty = 1;
	    } else {
		if (bits == 0) {
		    /* whole byte free so keep accumulating */
		    trailingEdge += 8;
		} else {
		    /* must start search over */
		    trailingEdge = (e[bits] >> 4) & 0xf;
		}
	    }
	    bits = BitsArray(page, h)[++i];	/* try next byte */
	    if (bits != 0) empty = 0;
	}
    }

    if (firstFrag != (u_long)-1) {
	if (wholeBlockFreeP) *wholeBlockFreeP = empty;
	nFragAllocates++;
    }
    return firstFrag;
}

/* InitFreeSpace --
 *
 * If the return value is zero, then the handle is guaranteed to
 * have a valid blocksFree value.
 *
 * LOCKS
 *	Assumes that caller has grabbed the allocation handle mutex.
 */
static long
InitFreeSpace(struct allocationData *h)
{
    long code = 0;
    u_char bcounts[256];		/* for small block/frag ratios */
    static u_char fcounts[256] = {	/* free frags in a byte */
	0x8, 0x7, 0x7, 0x6, 0x7, 0x6, 0x6, 0x5, 0x7, 0x6, 0x6, 0x5, 0x6, 0x5,
	0x5, 0x4, 0x7, 0x6, 0x6, 0x5, 0x6, 0x5, 0x5, 0x4, 0x6, 0x5, 0x5, 0x4,
	0x5, 0x4, 0x4, 0x3, 0x7, 0x6, 0x6, 0x5, 0x6, 0x5, 0x5, 0x4, 0x6, 0x5,
	0x5, 0x4, 0x5, 0x4, 0x4, 0x3, 0x6, 0x5, 0x5, 0x4, 0x5, 0x4, 0x4, 0x3,
	0x5, 0x4, 0x4, 0x3, 0x4, 0x3, 0x3, 0x2, 0x7, 0x6, 0x6, 0x5, 0x6, 0x5,
	0x5, 0x4, 0x6, 0x5, 0x5, 0x4, 0x5, 0x4, 0x4, 0x3, 0x6, 0x5, 0x5, 0x4,
	0x5, 0x4, 0x4, 0x3, 0x5, 0x4, 0x4, 0x3, 0x4, 0x3, 0x3, 0x2, 0x6, 0x5,
	0x5, 0x4, 0x5, 0x4, 0x4, 0x3, 0x5, 0x4, 0x4, 0x3, 0x4, 0x3, 0x3, 0x2,
	0x5, 0x4, 0x4, 0x3, 0x4, 0x3, 0x3, 0x2, 0x4, 0x3, 0x3, 0x2, 0x3, 0x2,
	0x2, 0x1, 0x7, 0x6, 0x6, 0x5, 0x6, 0x5, 0x5, 0x4, 0x6, 0x5, 0x5, 0x4,
	0x5, 0x4, 0x4, 0x3, 0x6, 0x5, 0x5, 0x4, 0x5, 0x4, 0x4, 0x3, 0x5, 0x4,
	0x4, 0x3, 0x4, 0x3, 0x3, 0x2, 0x6, 0x5, 0x5, 0x4, 0x5, 0x4, 0x4, 0x3,
	0x5, 0x4, 0x4, 0x3, 0x4, 0x3, 0x3, 0x2, 0x5, 0x4, 0x4, 0x3, 0x4, 0x3,
	0x3, 0x2, 0x4, 0x3, 0x3, 0x2, 0x3, 0x2, 0x2, 0x1, 0x6, 0x5, 0x5, 0x4,
	0x5, 0x4, 0x4, 0x3, 0x5, 0x4, 0x4, 0x3, 0x4, 0x3, 0x3, 0x2, 0x5, 0x4,
	0x4, 0x3, 0x4, 0x3, 0x3, 0x2, 0x4, 0x3, 0x3, 0x2, 0x3, 0x2, 0x2, 0x1,
	0x5, 0x4, 0x4, 0x3, 0x4, 0x3, 0x3, 0x2, 0x4, 0x3, 0x3, 0x2, 0x3, 0x2,
	0x2, 0x1, 0x4, 0x3, 0x3, 0x2, 0x3, 0x2, 0x2, 0x1, 0x3, 0x2, 0x2, 0x1,
	0x2, 0x1, 0x1, 0
    };

    u_long pages;
    int p;				/* current page number */

    epix_EnterCodeBlock(46);
    afsl_Assert (tmutex_held(&h->lock));

    pages = (h->length + h->blocksPerPage-1)/h->blocksPerPage;

    if ((h->fragmentsPerBlock > 1) && (h->fragmentsPerBlock < 8)) {
	/* Calculate table of free blocks and fragments per byte.  There are
	 * really only two cases here two and four frags/block. */
	int b;				/* byte value */
	for (b = 0; b < 256; b++) {
	    int byte = b;
	    int fb = 0;			/* free blocks */
	    if (h->fragmentsPerBlock == 2) {
		if ((byte & 0x03) == 0) fb++, byte |= 0x03;
		if ((byte & 0x0c) == 0) fb++, byte |= 0x0c;
		if ((byte & 0x30) == 0) fb++, byte |= 0x30;
		if ((byte & 0xc0) == 0) fb++, byte |= 0xc0;
	    } else {
		if ((byte & 0x0f) == 0) fb++, byte |= 0x0f;
		if ((byte & 0xf0) == 0) fb++, byte |= 0xf0;
	    }
	    bcounts[b] = (fb << 4) | fcounts[byte];
	}
    }

    h->blocksFreeValid = 0;
    h->blocksFree = h->fragsFree = 0;
    for (p = 0; p < pages; p++) {
	BEGIN_LCRIT(struct elbb_buffer *buf,
		    code = epix_GetBuffer(h->bitmap, p, &buf),
		    code = elbb_Release(buf, elbb_lazy)) {
	    struct bitmapPage *page;
	    u_int b;
	    u_char *ba;
	    page = (struct bitmapPage *)elbb_BufferData(buf);
	    if (!BITMAP_PAGE_OK(page, h))
		{ ABORT_CRIT (EPI_E_NOTABITMAP) }

	    switch (h->logFrags) {
	      case 0: /* fragsPerBlock == 1 */
		/* Note that there is not logged array */
		for (b = 0, ba = page->logged; b < h->bitsLength; b++, ba++)
		    h->blocksFree += fcounts[*ba];
		break;

	      case 1: /* fragsPerBlock == 2 */
	      case 2: /* fragsPerBlock == 4 */
		for (b = 0, ba = BitsArray(page, h);
		     b < h->bitsLength; b++, ba++) {
		    u_int bcount = bcounts[*ba];
		    h->blocksFree += bcount >> 4;
		    h->fragsFree += bcount & 0xf;
		}
		break;

	      case 3: /* fragsPerBlock == 8 */
		for (b = 0, ba = BitsArray(page, h);
		     b < h->bitsLength; b++, ba++) {
		    if (*ba == 0)
			h->blocksFree++;
		    else
			h->fragsFree += fcounts[*ba];
		}
		break;

	      case 4: /* fragsPerBlock == 16 */
		for (b = 0, ba = BitsArray(page, h); b < h->bitsLength; b += 2) {
		    u_char byte1 = *ba++;
		    u_char byte2 = *ba++;
		    if ((byte1 == 0) && (byte2 == 0)) h->blocksFree++;
		    else h->fragsFree += fcounts[byte1] + fcounts[byte2];
		}
		break;

	      case 5: /* fragsPerBlock == 32 */
		for (b = 0, ba = BitsArray(page, h); b < h->bitsLength; b += 4) {
		    u_char byte1 = *ba++;
		    u_char byte2 = *ba++;
		    u_char byte3 = *ba++;
		    u_char byte4 = *ba++;
		    if ((byte1 == 0) && (byte2 == 0) &&
			(byte3 == 0) && (byte4 == 0)) h->blocksFree++;
		    else h->fragsFree += fcounts[byte1] + fcounts[byte2] +
			fcounts[byte3] + fcounts[byte4];
		}
		break;

	      case 6: /* fragsPerBlock == 64 */
		for (b = 0, ba = BitsArray(page, h); b < h->bitsLength; b+=8) {
		    u_char byte1 = *ba++;
		    u_char byte2 = *ba++;
		    u_char byte3 = *ba++;
		    u_char byte4 = *ba++;
		    u_char byte5 = *ba++;
		    u_char byte6 = *ba++;
		    u_char byte7 = *ba++;
		    u_char byte8 = *ba++;
		    if ((byte1 == 0) && (byte2 == 0) &&
			(byte3 == 0) && (byte4 == 0) &&
			(byte5 == 0) && (byte6 == 0) &&
			(byte7 == 0) && (byte8 == 0)) h->blocksFree++;
		    else
			h->fragsFree += fcounts[byte1] + fcounts[byte2] +
			fcounts[byte3] + fcounts[byte4] + fcounts[byte5] +
			fcounts[byte6] + fcounts[byte7] + fcounts[byte8];
		}
		break;

	      default:
		/* never happens! */
		afsl_Assert(h->fragmentsPerBlock == 1);
	    }
	} END_CRIT;
	if (code) break;
    }
    if (!code) h->blocksFreeValid = 1;

    return code;
}

/* epib_CreateAllocationPool -- Constructs an allocation pool given a zero
 *     length container, \arg{bitmapC}, in which to maintain the bitmap.
 *     The range of legal block addresses which are to be part of the pool is
 *     given by \arg{baseBlock} to $\arg{baseBlock} + \arg{nBlocks}
 *     - 1$, inclusive.  A list of \arg{nExceptions} blocks specified in
 *     the array \arg{exceptionBlocks} are excluded from the pool.  This
 *     array should be freed by the caller upon return.
 *
 *     The bitmap is not required to be in the same aggregate as the blocks
 *     being allocated.  (But, of course, no one has actually tried this). */

/* EXPORT */
long epib_CreateAllocationPool(
  epi_anode_t bitmapC,
  daddr_t baseBlock,			/* first block addressed by bitmap */
  u_long nBlocks,			/* number of blocks in bitmap */
  u_int frags,				/* fragments per block */
  u_int nExceptions,			/* length of exception list */
  daddr_t exceptionBlocks[])		/* blocks to exclude from alloc pool */
{
    long code = 0;
    struct allocationData *h;		/* allocation handle */
    struct allocationData data;		/* allocation handle data */

    if ((nBlocks == 0) || (frags == 0)) return EPI_E_BADARGS;
    if ((frags < 1) || (frags > EPIB_MAX_FRAGMENTS_PER_BLOCK))
	return EPI_E_BADARGS;

    h = &data;
    /* initialize the handle here so we can go ahead and lock it */
    bzero((caddr_t)h, sizeof(*h));
    tmutex_Init(&h->lock, "allocation pool");

    BEGIN_TMUTEX_NT(&h->lock) {
	code = AllocatePool(h, elbb_zeroTrans, bitmapC, baseBlock, nBlocks,
			    frags, nExceptions, exceptionBlocks, /*extend*/ 0);
    } END_LOCK;
    CleanupHandle(h);

    return(code);
}

/* epib_GrowAllocationPool -- Extend an existing allocation pool.
 *     \arg{nBlocks} gives the new total length and must be at least as
 *     large as the current value.  Other arguments are the same as for
 *     \fcname{epib_CreateAllocationPool}. */

/* EXPORT */
long epib_GrowAllocationPool(
  elbb_tranRec_t trans,			/* transaction identifier */
  epi_anode_t bitmapC,
  daddr_t baseBlock,			/* first block addressed by bitmap */
  u_long nBlocks,			/* number of blocks in bitmap */
  u_int nExceptions,			/* length of exception list */
  daddr_t exceptionBlocks[])		/* blocks to exclude from alloc pool */
{
    struct allocationData *h;		/* allocation handle */
    long code = 0;

    /* The allocation handle exists, so we're just going to modify it */
    h = (struct allocationData *)epia_GetVolumeRock(bitmapC);
    if (!HANDLE_OK(h))
	return EPI_E_NOALLOCHANDLE;
    if (nBlocks < h->length+3) return EPI_E_BADARGS;	/* 3 blocks is min */

    BEGIN_TMUTEX_NT(&h->lock) {
	code = AllocatePool(h, trans, bitmapC, baseBlock, nBlocks,
			     /* frags -- TBD */(u_int)0,
			     nExceptions, exceptionBlocks, /*extend*/ 1);
    } END_LOCK;
    afsl_MBZ(code);
    return(0);
}

/*
 * AllocatePool -- This is the auxilliary function used by both
 *     epib_CreateAllocationPool and epib_GrowAllocation Pool.  Creation or
 *     extension is controlled by the ``extend'' parameter.
 *
 *     For creation, it constructs an allocation pool given a zero
 *     length container in which to maintain the bitmap, a range of legal block
 *     addresses which are part of the pool, and a list of exception blocks
 *     which are to be excluded from the pool.
 *
 *     For extension, it allocations the needed extra blocks and updates
 *     the in-memory allocation handle.
 *
 *     The bitmap is not required to be in the same aggregate as the blocks
 *     being allocated.
 *
 *
 * LOCKS USED -- The allocation handle is locked on entry.
 *
 * PARAMETERS -- The array of exception blocks should be freed by the caller
 *     upon return.
 */
static long AllocatePool(
  struct allocationData *h,		/* allocation handle */
  elbb_tranRec_t trans,			/* transaction identifier */
  epi_anode_t bitmapC,
  daddr_t baseBlock,			/* first block addressed by bitmap */
  u_long nBlocks,			/* number of blocks in bitmap */
  u_int frags,				/* fragments per block */
  u_int nExceptions,			/* length of exception list */
  daddr_t exceptionBlocks[],		/* blocks to exclude from alloc pool */
  int extend)				/* grow aggregate? */
{
    long code = 0;
    int i;
    long bsize;
    long blocksPerPage;			/* maximum blocks rep. by each page */
    long neededPages;			/* pages needed to represent nBlocks */
    afs_hyper_t length;			/* length of bitmap container */
    struct epix_blockLocation loc;	/* for inserting blocks into bitmap */
    struct bitmapPage *page;		/* page currently being constructed */
    struct elbb_buffer *buf;		/* bitmap page buffer */
    long b;				/* base of current page */
    long p;				/* page # in bitmap */
    int e;				/* first remaining exception to find */
    daddr_t bitmapBlock;		/* block address in bitmap container */
    daddr_t pb;				/* extra blocks at end of last page */
    u_long bitmapSize;			/* useful size of bitmap page */
    int doFormatPage;		/* call FormatPage()? */
    int doAllocate;		/* need to allocate a page for bitmap? */
    int firstPageUpdated;	/* # of 1st bitmap page updated for growing */
    int firstNewBlock;		/* first new block of bitmap extension */
    long freeBlocks;		/* # blocks to free in curr bitmap (extend) */
    long blockInPage;

    epix_EnterCodeBlock(17);
    afsl_Assert (tmutex_held(&h->lock));
    if (extend) {
	firstNewBlock = h->length;
	frags = h->fragmentsPerBlock;		/* overwrite parameter */
	blocksPerPage = h->blocksPerPage;
	baseBlock = h->base;
	nBlocks -= baseBlock;
	bsize = epia_AnodeDevice(bitmapC)->blkSize;
	p = AFS_hgetlo(epia_GetLength(bitmapC));
	p /= bsize;
	p--;

	/* read in the last page of the bitmap */
	page = osi_Alloc_r(bsize);
	BEGIN_CRIT(code = epix_GetBuffer(h->bitmap, p, &buf),
		   code = elbb_Release(buf, elbb_lazy))
	{
	    struct bitmapPage *Page;

	    Page = (struct bitmapPage *) elbb_BufferData(buf);
	    bcopy((caddr_t)Page, (caddr_t)page, bsize);
	} END_CRIT;
	if (code) return(code);

	/*
	 * Due to limitations in the maximum transaction size, we
	 * have to restrict the about of bitmap that we grow at any
	 * one time.  We will therefore do it a page at a time.
	 * epig_GrowAllocation will check to see if the bitmap is
	 * large enough when we return and it will call us again if it
	 * isn't.  We'll do this by adjusting nBlocks.
	 */
	blockInPage = h->length % blocksPerPage;
	freeBlocks = MIN((nBlocks - h->length), (blocksPerPage - blockInPage));
	nBlocks = h->length + freeBlocks;
	if (blockInPage) {
	    /* we have some more stuff to write into the current page */

	    /*
	     * The idea here is that there we have the last page of
	     * the existing bitmap.  When this page was created,
	     * blocks represented by the map but beyond the actual
	     * size of the aggregate were marked as allocated.  Since
	     * we have grown the aggregate, some of these blocks are
	     * now there so the map must be updated to reflect this.
	     * A special case occurs if the bitmap page exactly
	     * represented actual blocks (no additional blocks marked
	     * as allocated in the bitmap).  In this case, no update
	     * is required.  This case is indicated when blockInPage
	     * is 0. */

	    /* Mark as free all the blocks in the last bitmap page
	     * that are really now free */
	    int wholeBlockFree = -1;
	    u_long blocksFree = 0;

	    if (h->blocksFreeValid) {
		blocksFree = h->blocksFree;
	    }
	    code = MarkFrags(trans, 0, page, h, /*logged*/1,
			     blockInPage*frags, freeBlocks*frags,
			     FREE, &wholeBlockFree);
	    if (code) return(code);
	    if (blocksFree) {
		osi_assert(h->blocksFree == blocksFree + freeBlocks);
	    }

	    h->length += freeBlocks;  /* MarkFrags will update h->freeBlocks */
	    doAllocate = 0;	      /* since we read in an existing page */
	    page->bitmapLength += freeBlocks;
	    doFormatPage = 0;		/* since we read in a formatted page */
	    /* no need to update length of bitmapC */
	} else {
	    /*
	     * We have a whole page to write but it's the NEXT one.
	     */
	    p++;
	    doAllocate = 1;		/* do the allocate */
	    doFormatPage = 1;		/* format the new (in-core) page */

	    /* update lengths */
	    page->bitmapLength += freeBlocks;

	    /* adjust allocation handle */
	    h->length = nBlocks;
	    if (h->blocksFreeValid) h->blocksFree += freeBlocks;

	    length = epia_GetLength(bitmapC);
	    AFS_hadd32(length, bsize);	/* 1 more block */
	    code = epix_SetLength(trans, bitmapC, length, 0);
	    if (code) return code;
	}

	/*
	 * we keep track of the first page of the bitmap that is
	 * updated so that we can later go back and tell whether
	 * the first page of the bitmap will need to be updated
	 * later.
	 */
	firstPageUpdated = p;

	b = p * blocksPerPage;
    } else { /* if (!extend) */
	/* Set container length and call format page so InitHandle can work. */
	bsize = epia_AnodeDevice(bitmapC)->blkSize;
	bitmapSize = bsize - BITMAP_PAGE_OVERHEAD;
	if (frags == 1)
	    /* no need for logged bit if frag size == block size */
	    blocksPerPage = bitmapSize * 8;
	else
	    /* need an extra bit per block for remembering
	     * logged-ness of partial blocks.  Round down to get
	     * number of byte in both arrays, then multiply by 8 to
	     * get number of blocks representable. */
	    blocksPerPage = (bitmapSize / (frags+1)) * 8;

	neededPages = (nBlocks+(blocksPerPage-1)) / blocksPerPage;

	AFS_hset64(length, 0, bsize * neededPages);
	code = epix_SetLength(elbb_zeroTrans, bitmapC, length, 0);
	if (code)
	    return code;

	page = osi_Alloc_r(bsize);
	FormatPage(page, bsize, baseBlock, blocksPerPage, nBlocks, frags);
	doFormatPage = 0;
	doAllocate = 1;
	code = InitHandle(bitmapC, page, h);
	afsl_MBZ(code);
	p = 0;				/* first page */
	b = 0;				/* first block */
    }

    /* superblock always here */
    epix_ConditionalCodeBlock(nExceptions > 1, 18);
    for (e = 0; b < nBlocks; b += h->blocksPerPage, p++) {
	daddr_t base = b + baseBlock;
	if (doFormatPage)
	    FormatPage(page, bsize, base, blocksPerPage, nBlocks, frags);
	afsl_Assert(BITMAP_PAGE_OK(page, h));
	doFormatPage = 1;		/* format all subsequent pages */

	for (i = e; i < nExceptions; i++) {
	    daddr_t eb = exceptionBlocks[i];
	    if (!(epix_IsBlockEmpty(eb) ||
		  (eb < base) || (eb >= base+nBlocks))) {
		if (eb >= base + page->blocksPerPage)
		    continue;		/* get these later */
		if (extend && (eb < firstNewBlock))
		    continue;	/* already done */
		code = MarkBlock(trans, 0, page, h, eb, ALLOC);
		afsl_MBZ(code);
	    }
	    /* punt these to be beginning of the list and skip them in the
	     * future */
	    if (i != e) {
		exceptionBlocks[i] = exceptionBlocks[e];
		exceptionBlocks[e] = eb;
	    }
	    e++;
	}
	if (doAllocate)	{		/* allocate a new page to bitmap */
	    u_long blocksFree;

	    /* Look for a block for each page of bitmap on that page.
	     * There must be at least one free block (plus necessary
	     * indirect blocks) after exceptions are eliminated.  This
	     * constraint could be relaxed... */

	    while (1) {
		code = epix_FindBlocks(bitmapC, p, 1, 0, &bitmapBlock,
				       0, 0, &loc);
		if (code) break;
		if (epix_IsBlockAllocated(bitmapBlock)) break;
		bitmapBlock = FindBlock(page, h, /*hint*/0);
		if (epix_IsBlockEmpty(bitmapBlock))
		    return (EPI_E_BADBITMAPBLOCKS);
		code = MarkBlock(trans, 0, page, h, bitmapBlock, ALLOC);
		afsl_MBZ(code);
		code = epix_Insert(trans, bitmapC, 1, &bitmapBlock, &loc);
		if (code) break;
	    }
	    if (code) return(code);

	    /* Mark as allocated all the blocks in the last bitmap
	     * page past the actual block length */
	    if (h->blocksFreeValid) blocksFree = h->blocksFree;
	    for (pb = nBlocks; pb < b + h->blocksPerPage; pb++) {
		code = MarkBlock(trans, 0, page, h, pb+baseBlock, ALLOC);
		afsl_MBZ(code);
	    }
	    if (h->blocksFreeValid) h->blocksFree = blocksFree;

	    /* Now write out data in page */
	    BEGIN_CRIT (code = elbb_Read (epia_AnodeDevice(bitmapC),
					  bitmapBlock, elbb_bufLogged, &buf),
			code = elbb_Release (buf, elbb_lazy)) {
		code = elbb_Modify (buf, 0, (caddr_t)page, bsize,
				    trans, /*logRequest*/extend, &buf);
	    } END_CRIT;
	} else {
	    /* modify existing page */
	    BEGIN_CRIT(code = epix_GetBuffer(bitmapC, p, &buf),
			code = elbb_Release(buf, elbb_lazy))
	    {
		code = elbb_Modify(buf, 0, (caddr_t)page, bsize,
				    trans, /*logRequest*/1, &buf);

	    } END_CRIT;
	    doAllocate = 1;		/* allocate subsequent pages */
	}
	if (code) break;
    } /* for each page */
    if (code) return(code);

    osi_Free_r(page, bsize);
    if (extend) {
	/*
	 * If firstPageUpdated != 0, the first page of the bitmap has not
	 * been updated yet.  We do that now.
	 */
	if (firstPageUpdated != 0) {
	    u_int32	bitmapLength;
	    /* update all bitmap pages */
	    BEGIN_CRIT(code = epix_GetBuffer(bitmapC, 0, &buf),
			code = elbb_Release(buf, elbb_lazy)) {
		struct bitmapPage *Page;

		Page = (struct bitmapPage *)elbb_BufferData(buf);
		if (!BITMAP_PAGE_MAGIC_OK(Page,
					  epia_AnodeDevice(bitmapC)->blkSize))
		{ ABORT_CRIT (EPI_E_NOTABITMAP) }

		bitmapLength = h->length;
		code = elbb_Modify(buf,
				   offsetof(struct bitmapPage, bitmapLength),
				   (caddr_t)&bitmapLength, sizeof(u_int32),
				   trans, /*logRequest*/1, &buf);
	    } END_CRIT;
	    afsl_MBZ(code);
	}
    } else {
	if (code)
	    return(code);

	/* on exit from loop "p" is number of pages in bitmap. */
	afsl_Assert(p == neededPages);
    }
    return 0;
}


/* epib_InitAllocationPool -- Initializes a new allocation pool and returns a
 *     handle in \arg{allocationHandleP}.  This handle should be attached
 *     to each anode (using the \arg{allocationH} parameter to
 *     \fcname{epia_Open}, or \fcname{epia_SetAllocationHandle}) for which
 *     allocation will be requested from this pool.  The container
 *     \arg{bitmapC} holds the bitmap.  The parameters \arg{firstBlock}
 *     and \arg{nBlocks} come from the superblock and are used only for
 *     consistency checking. */

/* EXPORT */
long epib_InitAllocationPool(
  epi_anode_t bitmapC,
  daddr_t firstBlock,			/* for consistency checking */
  u_long nBlocks,			/* ditto */
  epi_bitmap_t *allocationHandleP)
{
    long code = 0;
    struct allocationData *h;
    struct elbb_buffer *buf;		/* bitmap page buffer */
    struct bitmapPage *page;

    epix_EnterCodeBlock(19);
    if (GetOKbitmap(h, bitmapC)) {
	*allocationHandleP = (epi_bitmap_t)h;
	return 0;
    }
    epix_EnterCodeBlock(20);
    h = osi_Alloc_r(sizeof (*h));

    BEGIN_CRIT(code = epix_GetBuffer(bitmapC, /*first block*/0, &buf),
		code = elbb_Release(buf, elbb_lazy)) {
	page = (struct bitmapPage *)elbb_BufferData(buf);
	if (!BITMAP_PAGE_MAGIC_OK(page, epia_AnodeDevice(bitmapC)->blkSize)) {
	    ABORT_CRIT(EPI_E_NOTABITMAP)
	}
	/* initialize allocation handle */
	bzero((caddr_t)h, sizeof(*h));
	tmutex_Init (&h->lock, "allocation pool");

	BEGIN_TMUTEX_NT (&h->lock) {
	    code = InitHandle (bitmapC, page, h);
	    if (code)
		EXIT_CRIT;
	    afsl_Assert(BITMAP_PAGE_OK (page, h));

	    if ((firstBlock != h->base) || (nBlocks != h->length))
		code =  EPI_E_NOTABITMAP;
	} END_LOCK;
    } END_CRIT;
    if (code) {
	osi_Free_r(h, sizeof(*h));
	*allocationHandleP = 0;
	return code;
    }
    *allocationHandleP = (epi_bitmap_t)h;
    epia_SetVolumeRock(bitmapC, h, Cleanup);
    return 0;
}

/* epib_CloseAllocationPool -- Shutdown the allocation pool specified by
 *     \arg{allocationHandle} as a prelude to shutting the system down. */

/* EXPORT */
long epib_CloseAllocationPool(epi_bitmap_t allocationHandle)
{
    long code = 0;
    struct allocationData *h;
    epix_EnterCodeBlock(21);
    h = (struct allocationData *)allocationHandle;
    afsl_Assert(HANDLE_OK(h));

    code = fb_CommitAllTrans(h);
    if (code) return code;

    code = epia_Close(h->bitmap);
    return code;
}

/* We use these values to decide how many blocks to withhold.  For large
 * aggregates 1% is plenty, while for small aggregates more than 10% would be
 * onerous.  In between 2000K allows clones of filesets containing as many as
 * 8000 files. */

/* EXPORT */
u_long epib_leastWithheld = 20;	/* in mills: 10 => 2.0% */
/* EXPORT */
u_long epib_mostWithheld = 100;	/* 10.0% */
/* EXPORT */
u_long epib_withholding = 4096;	/* target value, in Kbytes */

/* Withholding -- returns number of blocks being withheld from use for
 *     "regular" uses.  This is used to prevent quota/space reservations from
 *     succeeding if the aggregate has too few free blocks.  The intent is to
 *     reserve enough free space to allow a fileset clone to succeed. */

static u_long Withholding(struct allocationData *h)
{
    u_long floorT, ceilT;
    u_long t;				/* target withholding in blocks */

    if (h->length < 1000000) {
	floorT = (h->length*epib_leastWithheld) / 1000;
	ceilT = (h->length*epib_mostWithheld) / 1000;
    } else {
	floorT = (h->length/1000) * epib_leastWithheld;
	ceilT = (h->length/1000) * epib_mostWithheld;
    }
    t = epib_withholding / (epia_AnodeDevice(h->bitmap)->blkSize >> 10);
    t = MAX(t, floorT);
    t = MIN(t, ceilT);
    return t;
}

/* epib_GetInfo -- Extracts various private fields from an allocation handle.
 */
/* EXPORT */
long epib_GetInfo(epi_bitmap_t allocationHandle, struct epib_info *info)
{
    struct allocationData *h;
    long code = 0;

    epix_EnterCodeBlock(62);
    h = (struct allocationData *)allocationHandle;
    afsl_Assert(HANDLE_OK(h) && info);

    BEGIN_TMUTEX_NT (&h->lock) {
	u_long reserved;
	u_long withholding;

	if (!h->blocksFreeValid) {
	    code = InitFreeSpace (h);
	    if (code) continue;
	}

	info->bitmapC = h->bitmap;
	info->base = h->base;
	info->length = h->length;

	info->realFree = (h->blocksFree << h->logFrags) + h->fragsFree;

	withholding = Withholding(h) << h->logFrags;
	/* Withholding can be larger than free, so just return zero. */
	withholding = MIN(withholding, info->realFree);
	info->avail = info->realFree - withholding;

	info->freeFrags = h->fragsFree;

    } END_LOCK;
    return code;
}

/*
 * epib_Allocate -- Allocates *nBlocksP file system blocks from the allocation
 *	pool used by the anode, returning their addresses in bArray.  Note
 *	that the addresses are in units of the file system block size, and
 *	hence must be shifted before they can be used as disk addresses.
 *
 *	This routine walks through the bitmap looking for free blocks.
 *	It starts after the block specified in the allocation handle
 *	(see \fcname{epia_GetAllocationHandle}) of \arg{anode} or, if this
 *	number is zero, at a random point in the bitmap.  Each page of
 *	the bitmap is searched in order, starting with the page containing the
 *	starting point.  Except for the starting page, subsequent pages are
 *	searched starting at their baseOffset.  The starting page is searched
 *	twice, the first time starting at the starting point, and the second,
 *	starting at its beginning.
 *
 *      If flags has EPIB_CONTIG_ONLY set, the search is terminated as soon as
 *      a noncontiguous block is found.  If EPIB_DONT_WITHHOLD is set then the
 *      withholding can be consumed.
 *
 *      The allocated blocks are returned in \arg{bArray}.  If \arg{*nBlocksP}
 *      could not be allocated, some elements of \arg{bArray} may be set to
 *      \constname{EPIX_ANODE_BLOCKEMPTY}; if no blocks could be allocated,
 *      EPI_E_AGGREGATEFULL is returned.  In both cases, *nBlocksP is updated
 *      with the number of allocated blocks.  We require that *nBlocksP be
 *      non-zero on entry.
 */
/* EXPORT */
long
epib_Allocate(
  elbb_tranRec_t trans,
  epi_anode_t anode,			/* to which blocks are to allocated */
  u_long *nBlocksP,			/* number to allocate */
  daddr_t bArray[],			/* for returned block addresses */
  int flags)				/* see above */
{
    struct allocationData *h;		/* allocation pool */
    long code = 0;
    struct allocationHint hint;
    struct elbb_buffer *buf;		/* bitmap page buffer */
    bitmapPage_t *page;			/* page currently being examined */
    long lastPage;			/* highest page number in bitmap */
    long initialPage;			/* page to start search on */
    daddr_t start;			/* preferred block number */
    daddr_t lastBlock;			/* place where we stopped last time */
    daddr_t block;			/* allocated block */
    daddr_t prevBlock;			/* previous block found in loop */
    int passno;				/* indicator that search has wrapped */
    long p;				/* page number in bitmap */
    int i;				/* index in bArray */
    u_long blksAvailable;		/* blocksFree adj. for withholding */
    u_long nblks = *nBlocksP;		/* No. of blocks wanted */
    u_long b;				/* No. of blocks allocated */
    int wait;				/* wait for recently freed block */
    int freedBlocksSkipped;		/* # blocks skipped */
    int reallyWait;			/* always wait on second pass */
    int contig_only = (flags & EPIB_CONTIG_ONLY);
    int eop;				/* end-of-page boolean */

    BOMB_RETURN("epib_Allocate.1");

    epix_EnterCodeBlock(22);

    h = (struct allocationData *)epia_GetAllocationHandle(anode);
    afsl_AssertCond(HANDLE_OK(h));
    afsl_Assert(nblks != 0);

    lastBlock = (daddr_t)epia_GetAllocationRock(anode);
    wait = (lastBlock != 0 && lastBlock >= h->base);
    if (!wait) {
	epix_EnterCodeBlock(23);
	lastBlock = randomSeed % h->length + h->base;
	NewRandom(1);
    }

    /*
     * The variable "start" controls the search of each page.  Initially it
     * starts after the last block allocated for the anode.  After each block
     * is found it is advanced to the the following block address.  It wraps
     * from h->length+h->base to h->base.  After each page is exhausted start
     * is set to EMPTY indicating that the search should start at the beginning
     * of the next page.  Start is in units of **file system blocks**.
     */
    start = lastBlock + 1;
    if (start >= h->length + h->base) {
	start = h->base;
	wait = 0;
    }

    lastPage = (h->length - 1) / h->blocksPerPage; /* page bounds */
    initialPage = (start - h->base) / h->blocksPerPage;

    epix_ConditionalCodeBlock(nblks > 1, 24);
    b = 0;				/* found block index */
    p = initialPage;			/* page to start on */

    /*
     * We traverse each page looking for free blocks.  We start on the
     * requested initialPage (at the requested offset) and end after
     * searching that page a second time (this time starting at the beginning
     * of the page).  If we do not find enough blocks but skipped some blocks
     * because of IsFreedBlock, we go around again, this time always waiting
     * if possible.
     */
    passno = 0;			/* we do the first page an extra time */
    freedBlocksSkipped = 0;	/* bumped when IsFreedBlocks=>TRUE */
    reallyWait = 0;

    BEGIN_TMUTEX(trans, &h->lock);
    if (!h->blocksFreeValid) {
	code = InitFreeSpace (h);
	MBZ(code);
    }
    blksAvailable = h->blocksFree;
    if (!(flags & EPIB_DONT_WITHHOLD)) {
	int w = Withholding (h);
	if (w > blksAvailable)
	    blksAvailable = 0;
	else
	    blksAvailable -= w;
    }
    if (blksAvailable <= 0) {
	ABORT_CRIT(EPI_E_AGGREGATEFULL);
    }
    if (blksAvailable < nblks) {
	/* only enough for partial request */
	/* mark blocks that we know we won't be able to find */
	for (i = blksAvailable; i < nblks; i++)
	    bArray[i] = EPIX_ANODE_BLOCKEMPTY;
	nblks = blksAvailable;
    }

    prevBlock = 0;
    while (1) {
	BEGIN_CRIT(code = epix_GetBuffer(h->bitmap, p, &buf),
		   code = elbb_Release(buf, elbb_lazy)) {
	    page = (struct bitmapPage *)elbb_BufferData(buf);
	    afsl_Assert(BITMAP_PAGE_OK(page, h));
	    if (start == EPIX_ANODE_BLOCKEMPTY) {
		start = page->baseOffset;
		wait = reallyWait;
	    }

	    /* as long as everything is okay, keep looking for blocks */
	    eop = 0;
	    while (code == 0 && b < nblks && !eop) {
		hint.start = start - page->baseOffset;
		block = FindBlock(page, h, &hint);
		if (epix_IsBlockEmpty(block) ||
		    contig_only && prevBlock != 0 && block != prevBlock + 1)
		    break;

		/*
		 * We can't return recently freed blocks.  We can wait if the
		 * trans has ended but not committed, and it is really a block
		 * we want or it is the second time around.  Otherwise don't
		 * bother to wait for it.
		 */
		if (block != start)
		     wait = reallyWait;

		if (!IsFreedBlock(h, block, 0, 0, wait)) {
		    code = MarkBlock(trans, &buf, page, h, block, ALLOC);
		    if (code != 0)
			break;
		    prevBlock = bArray[b++] = block;
		    DBprint(("epib_Allocate: getting %d (%d of %d)",
			      block, b, nblks));
		} else {
		    /* skip this block and try for next */
		    freedBlocksSkipped++;
		}
		start = block + 1;	/* try to get next block */
		eop = (start >= page->baseOffset + h->blocksPerPage ||
			start >= h->length + h->base);
	    }
	} END_CRIT;

	if (b == nblks || code != 0 || !eop && contig_only && b != 0)
	    break;

	if (p == initialPage) {
	    if (++passno > 2)	/* go around at most twice */
		break;
	    else if (passno == 2) {
		if (freedBlocksSkipped == 0)
		    break;	/* skip 2nd pass if nothing to wait for */
		reallyWait = 1;	/* always wait on 2nd pass */
	    }
	}
	p = (p == lastPage) ? 0 :  p + 1;
	start = EPIX_ANODE_BLOCKEMPTY;	/* at beginning of page */
	wait = reallyWait;		/* don't bother waiting */
    }
    if (code != 0)
	afsl_Assert(b == 0);	/* we'll lose these blocks... */
    else {
	epix_EnterCodeBlock(25);
	if (b == 0)
	    code = EPI_E_AGGREGATEFULL;	 /* none allocated */
	else {
	    for (i = b; i < nblks; i++)	/* blank rest of output array */
		bArray[i] = EPIX_ANODE_BLOCKEMPTY;
	    epia_SetAllocationRock(anode, bArray[b - 1], 0);
	}
    }
    END_LOCK;

    *nBlocksP = b;
    return code;
}

/*
 * epib_Free -- Frees \arg{nBlocks} blocks whose addresses are specified by
 *     the \arg{bArray} to the allocation pool used by \arg{anode}.
 */
/* EXPORT */
long
epib_Free(
  elbb_tranRec_t trans,
  epi_anode_t anode,			/* anode from which blocks come */
  u_long nBlocks,			/* number of blocks */
  daddr_t bArray[])			/* block addresses */
{
#define NPAGES (EPIB_MAX_FREEDBLOCKS_PER_TRANS + 5)
    long code;
    u_long n;
    struct allocationData *h;
    int i, j;			/* for looping over block array */
    daddr_t pb;			/* physical block address */
    struct elbb_buffer *buf;
    struct bitmapPage *page;
    long pages[NPAGES];		/* page addresses of each block */

    while (nBlocks > 0) {
	n = MIN(nBlocks, NPAGES);
	nBlocks -= n;

	epix_EnterCodeBlock(27);
	h = (struct allocationData *)epia_GetAllocationHandle(anode);
	afsl_Assert(HANDLE_OK(h));

	epix_ConditionalCodeBlock(n > 1, 28);
	for (i = 0; i < n; i++) {
	    pb = bArray[i];
	    if (!epix_IsBlockAllocated(pb))
		pages[i] = -1;
	    else
		pages[i] = (pb - h->base) / h->blocksPerPage;
	}

	BEGIN_TMUTEX(trans, &h->lock);
	for (i = 0; i < n; i++) {
	    long p = pages[i];
	    if (p < 0)
		continue;
	    BEGIN_CRIT(code = epix_GetBuffer(h->bitmap, p, &buf),
			code = elbb_Release(buf, elbb_lazy)) {
		page = (struct bitmapPage *) elbb_BufferData(buf);
		/* Now free all the blocks on this page. */
		for (j = i; j < n; j++) {
		    if (p == pages[j]) {
			code = MarkBlock(trans, &buf, page, h, bArray[j], FREE);
			if (code != 0)
			    break;
			/* remember this block till trans commits */
			code = AddFreedBlock(h, bArray[j], 0, 0, trans);
			if (code != 0)
			    break;
			if (epia_GetFlags(anode) & EPIA_CFLAGS_LOGGED)
			    KillFreedBlock(h, bArray[j], trans);
			else {
			/*
			 * Kill user data pages immediately to avoid
			 * unnecessary dirty buffer writes.  If it turns out to
			 * be a metadata block (e.g. an indirect block) just
			 * skip it.
			 */
			    code = elbb_Kill(epia_AnodeDevice(anode),
					bArray[j], 0/*!metadataOK*/);
			    if (code == BUF_CANTDELETE) {
				KillFreedBlock(h, bArray[j], trans);
				code = 0;
			    } else if (code != 0)
				 break;	/* some unexpected problem */
			}
			DBprint(("epib_Free: freeing %d (%d of %d)",
			      bArray[j], j + 1, n));
			pages[j] = -1;	/* don't do this one again */
		    }
		}
	    } END_CRIT;
	    afsl_MBZ(code);
	}
	END_LOCK;
    }
    return 0;
}

/*
 * epib_AllocateFrags -- Allocates a group of \arg{nFrags} fragments from
 *     the allocation pool used by the \arg{anode}.  It tries to extend the
 *     fragment group already allocated to \arg{anode}, if requested.  The
 *     extended or new fragment group description is returned in
 *     \arg{frags}.
 *
 *      The \arg{flags} parameter controls whether the existing fragment group
 *      should be extended in place.  It has three useful values:
 *      EPIB_EXTEND_FRAGS means extend in place (and return EPI_E_CANNOTEXTEND
 *      if this is impossible), EPIB_MOVE_FRAGS means always allocate a new
 *      fragment group, and EPIB_EXTEND_FRAGS|EPIB_MOVE_FRAGS means try to
 *      extend in place but allocate a new group if that fails.  A third bit:
 *      EPIB_DONT_WITHHOLD, means that we can exceed the withholding limit;
 *      this is not presently used.  On other values, we panic.
 */
/* EXPORT */
long
epib_AllocateFrags(
  elbb_tranRec_t trans,
  epi_anode_t anode,			/* to which blocks are to allocated */
  int flags,				/* see above */
  u_int nFrags,				/* number of frags needed */
  struct epix_fragDescription *frags)	/* returned frag group */
{
    long code = 0;
    struct allocationData *h;		/* allocation pool */
    struct allocationHint hint;
    u_long lastPage;			/* highest page number in bitmap */
    u_long initialPage;			/* page to start search on */
    u_long start = 0;			/* prefered fragment number */
    int passno;				/* indicator that search has wrapped */
    long p;				/* page number in bitmap */
    u_long firstFrag;			/* fragInPage of beginning of group */
    int found;				/* set if frag group is located */
    u_int tryFor;			/* if extending number to get */
    int logged;
    int freedBlocksSkipped;		/* # blocks skipped */
    int reallyWait;			/* always wait on second pass */

    BOMB_RETURN("epib_AllocateFrags.1");

    epix_EnterCodeBlock(29);
    h = (struct allocationData *)epia_GetAllocationHandle(anode);
    afsl_AssertCond(HANDLE_OK(h));
    afsl_Assert(((flags & ~(EPIB_EXTEND_FRAGS | EPIB_MOVE_FRAGS |
			    EPIB_DONT_WITHHOLD)) == 0) &&
		((flags & (EPIB_EXTEND_FRAGS | EPIB_MOVE_FRAGS)) != 0));
    afsl_Assert(nFrags != 0 && nFrags <= h->fragmentsPerBlock);

    /* try extending existing fragment group of anode if it has one */
    tryFor = 0;
    if (flags & EPIB_EXTEND_FRAGS) {
	struct epix_fragDescription desc;
	code = epix_GetFragInfo(anode, &desc);
	if (code != 0)
	    return code;
	if (epix_IsBlockAllocated(desc.block)) {
	    if (desc.first + nFrags <= h->fragmentsPerBlock) {
		/* the extended group could fix in this block */
		epix_EnterCodeBlock(41);
		*frags = desc;		/* in case we suceed fill in output */
		firstFrag = (desc.block << h->logFrags) + desc.first;
		start = firstFrag + desc.length;
		tryFor = nFrags - desc.length;
		if (tryFor == 0)
		    return 0;	/* no extending needed */
	    } /* else current frag group too close to end of block */
	} /* else probably not fragmented */
    }
    if (tryFor == 0 && !(flags & EPIB_MOVE_FRAGS)) {
	DBprint(("epib_AllocateFrags: can't extend due to placement"));
	return (EPI_E_CANNOTEXTEND);
    }

    logged = ((epia_GetFlags(anode) & EPIA_CFLAGS_LOGGED) != 0);

    /*
     * The variable "start" controls the search of each page.  Initially it
     * starts at the beginning of a random block unless we're extending an
     * existing fragment group.  After each page is exhausted start is set to
     * EMPTY indicating that the search should start at the beginning of the
     * next page.  Start is in units of **fragments**.
     */
    if (tryFor == 0) {
	start = (randomSeed % h->length + h->base) * h->fragmentsPerBlock;
	NewRandom(1);
    }

    lastPage = (h->length - 1) / h->blocksPerPage; /* page bounds */
    initialPage = (start / h->fragmentsPerBlock - h->base) / h->blocksPerPage;

    p = initialPage;			/* page to start on */

    /* See epib_Allocate for details on this looping algorithm. */

    passno = 0;				/* we do the first page twice */
    freedBlocksSkipped = 0;		/* bumped when IsFreedBlocks=>TRUE */
    reallyWait = 0;

    found = 0;
    BEGIN_TMUTEX (trans, &h->lock);
    if (!h->blocksFreeValid) {
	code = InitFreeSpace (h);
	MBZ(code);
    }

    if (!(flags & EPIB_DONT_WITHHOLD)) {
	int w = Withholding(h);

	/* Any fragment allocation might try to break up a free block so don't
         * let any fragment allocates happen if withholding is enabled and we
         * are at or below the limit. */

	if (w && (h->blocksFree <= w))
	    { ABORT_CRIT (EPI_E_AGGREGATEFULL) }
    }
    if (h->fragsFree < nFrags && h->blocksFree < 1)
	{ ABORT_CRIT (EPI_E_AGGREGATEFULL) }

    while (1) {
	BEGIN_LCRIT(struct elbb_buffer *buf,
		    code = epix_GetBuffer(h->bitmap, p, &buf),
		    code = elbb_Release(buf, elbb_lazy)) {
	    struct bitmapPage *page;
	    int wholeBlockFree;
	    page = (struct bitmapPage *)elbb_BufferData(buf);
	    afsl_Assert(BITMAP_PAGE_OK(page, h));

  search_this_page_again:
	    hint.start = (start == (u_long)(-1)) ? 0 :
			start - page->baseOffset * h->fragmentsPerBlock;

	    if (tryFor != 0) { /* first time try extending, if enabled */
		firstFrag = FindFrags(page, h, logged, tryFor,
				      &hint, &wholeBlockFree);
		if (hint.start == firstFrag &&
		    !IsFreedBlock(h, frags->block,
				   frags->first + frags->length, tryFor, 1)) {
		    /* we found free space following existing frags */
		    epix_EnterCodeBlock(42);
		    code = MarkFrags(trans, &buf, page, h, logged, firstFrag,
				      tryFor, ALLOC, &wholeBlockFree);
		    if (code == 0)
			found++;		/* we're OK! */
		    EXIT_CRIT;
		} else if (!(flags & EPIB_MOVE_FRAGS)) {
		    epix_EnterCodeBlock(43);
		    DBprint(("epib_AllocateFrags: can't get frag to extend"));
		    code = EPI_E_CANNOTEXTEND;
		    EXIT_CRIT;
		}
		/* optimize by bumping hint? */
		tryFor = 0;		/* no luck */
	    }
	    afsl_Assert(flags & EPIB_MOVE_FRAGS);
	    firstFrag = FindFrags(page, h, logged, nFrags,
				   &hint, &wholeBlockFree);
	    if (firstFrag == (u_long)-1)
		EXIT_CRIT; /* try next page */

	    frags->block = (firstFrag >> h->logFrags) + page->baseOffset;
	    frags->first = firstFrag & ((1 << h->logFrags) - 1);
	    if (IsFreedBlock(h, frags->block, frags->first, nFrags,
			      reallyWait)) {
		/* Advance start to first frag of next block and repeat.
		 * Note that FindFrags will gracefully handle page-wrap by
		 * failing immediately. */
		start = (frags->block + 1) << h->logFrags;
		freedBlocksSkipped++;
		goto search_this_page_again;
	    }
	    code = MarkFrags(trans, &buf, page, h, logged, firstFrag, nFrags,
			      ALLOC, &wholeBlockFree);
	    if (code == 0)
		found++;
	} END_CRIT;

	if (found || code != 0)
	    break;

	if (p == initialPage) {
	    if (++passno > 2) {	/* go around at most twice */
		epix_EnterCodeBlock(30);
		break;
	    } else if (passno == 2) {
		if (freedBlocksSkipped == 0) {
		    epix_EnterCodeBlock(7);
		    break;	/* skip 2nd pass if nothing to wait for */
		}
		reallyWait = 1;	/* always wait on 2nd pass */
	    }
	}
	p = (p == lastPage) ? 0 :  p + 1;
	start = (u_long)-1;		/* at beginning of page */
    }
    if (found) {
	frags->length = nFrags;	/* if successful... */
	afsl_Assert(frags->first < h->fragmentsPerBlock &&
		    frags->first+frags->length <= h->fragmentsPerBlock);
    } else if (code == 0) {
	DBprint (("epib_AllocateFrags: aggregate full"));
	code = EPI_E_AGGREGATEFULL;
    }
    END_LOCK;
    if (code == 0)
	DBprint(("epib_AllocateFrags: in block %d (f%d l%d tryFor %d)",
		  frags->block, frags->first, frags->length, tryFor));
    return code;
}

/*
 * epib_FreeFrags -- Frees all or part of the fragment group specified by
 *     \arg{frags}, which is allocated to \arg{anode}.  The number of
 *     fragments retained in the fragment group is specified by \arg{keep}.
 *     If \arg{keep} is zero then then whole group is freed.
 *
 *     It is legal to free some of the fragments at the of a whole block.
 *     There is special code to handle updating the logged map in this case.
 */
/* EXPORT */
long
epib_FreeFrags(
  elbb_tranRec_t trans,
  epi_anode_t anode,			/* anode from which frag group comes */
  u_int keep,				/* number to keep */
  struct epix_fragDescription *frags) /* frag group */
{
    long code = 0;
    struct allocationData *h;
    long p;				/* page address of block */
    int empty = -1;			/* unknown */
    int logged;

    epix_EnterCodeBlock(44);
    h = (struct allocationData *)epia_GetAllocationHandle(anode);
    afsl_Assert(HANDLE_OK(h));
    afsl_Assert(frags->first < h->fragmentsPerBlock &&
	    frags->length <= h->fragmentsPerBlock);
    afsl_Assert(epix_IsBlockAllocated(frags->block) && keep <= frags->length);

    if (keep == frags->length)
	return 0; /* no-op */

    logged = ((epia_GetFlags(anode) & EPIA_CFLAGS_LOGGED) != 0);

    p = (frags->block - h->base) / h->blocksPerPage;

    BEGIN_TMUTEX(trans, &h->lock);
    BEGIN_LCRIT(struct elbb_buffer *buf,
		 code = epix_GetBuffer(h->bitmap, p, &buf),
		 code = elbb_Release(buf, elbb_lazy)) {
	struct bitmapPage *page = (struct bitmapPage *) elbb_BufferData(buf);
	u_long firstFrag;
	u_long nFrags;

	afsl_Assert(BITMAP_PAGE_OK(page, h));
	afsl_Assert(frags->block >= page->baseOffset &&
		frags->block < page->baseOffset + page->blocksPerPage);

	firstFrag =
	    (frags->block - page->baseOffset) * page->fragmentsPerBlock +
	    frags->first + keep;
	nFrags = frags->length - keep;

	if (keep != 0)		/* can't be empty if keeping any */
	    empty = (frags->length == h->fragmentsPerBlock) ? -2 : 0;

	code = MarkFrags(trans, &buf, page, h, logged,
			  firstFrag, nFrags, FREE, &empty);
	if (code != 0)
	    EXIT_CRIT;
	code = AddFreedBlock(h, frags->block, frags->first + keep, nFrags,
			      trans);
	if (code != 0)
	    EXIT_CRIT;
	if (empty == -1)
	    empty = IsWholeBlockFree(h, BitsArray(page, h), firstFrag);
	if (empty != 0) {
	    if (logged)
		KillFreedBlock(h, frags->block, trans);
	    else {
		    /*
		     * Kill user data pages immediately to avoid unnecessary
		     * dirty buffer writes.
		     */
		    code = elbb_Kill(epia_AnodeDevice(anode), frags->block,
				      0/*!metadataOK*/);
		    afsl_MBZ(code);

	    }
	}
    } END_CRIT;
    if (code != 0)
	EXIT_CRIT;
    if (empty != 0) {
	epix_EnterCodeBlock(45);
	DBprint(("epib_FreeFrags: last frags freed from block %d (f%d l%d)",
		  frags->block, frags->first, frags->length));
    } else {
	DBprint(("epib_FreeFrags: freed from block %d (f%d l%d keep %d)",
		  frags->block, frags->first, frags->length, keep));
    }
    END_LOCK;
    if (code == 0)
	frags->length = keep;
    return code;
}

#ifndef KERNEL
#include <verify.h>

/* epib_VerifyBitmap -- Check that the bitmap computed via ``verif'' matches
 *     what's really out there on the disk.
 *
 * CAUTIONS -- This code does not maintain nor check the logged map.  For
 *     rebuilding the bitmap from scratch this will be necessary.
 *
 * RETURN CODES -- Normally this routine returns zero.  Bitmap errors are
 *     reported by setting the MISC_PROBLEM bit and repairs set the REPAIRED
 *     bit.  Unexpected errors or I/O problems return -1 or lower level code
 *     values. */

/* EXPORT */
long
epib_VerifyBitmap(
  epi_anode_t aggrAnode,
  struct epiz_salvageState *ss,
  epi_anode_t bitmapAnode,
  epi_bitmap_t epiBits)
{
    struct epiz_SimBits *verif =  ss->simBits;
    long code = 0;
    struct allocationData *h;		/* handle */
    u_long nPages;			/* number of pages in bitmap */
    u_long length;			/* length of bitmap container */
    struct bitmapPage *page;		/* page currently being constructed */
    struct elbb_buffer *buf;		/* bitmap page buffer */
    struct epia_info Info;
    u_long pageCount;
    daddr_t baseAddr;
    long Ix, IxOffs, HighIx;
    u_long BlocksHere;
    u_long BitsPerPage;
    u_long FragNo;

    /* initialize our description in ss for error reporting */
    ss->volIx = ss->anodeIx = 0;
    ss->bitmapPage = -1;

    if (!GetOKbitmap(h, bitmapAnode))
	return EPI_E_NOTABITMAP;
    if ((long)h != (long)epiBits)
	return EPI_E_NOTABITMAP;
    code = epia_GetInfo(bitmapAnode, &Info);
    if (code != 0)
	return code;
    if (!AFS_hfitsinu32(Info.length))
	return -1;
    length = AFS_hgetlo(Info.length);
    nPages = length / epia_AnodeDevice(bitmapAnode)->blkSize;
    BitsPerPage = h->blocksPerPage * h->fragmentsPerBlock;
    IxOffs = 0;
    DBVprint(("Analyzing bitmap of %lu pages, %lu bytes per page, whose first bit describes physical block %#lx on the disk.\n",
		nPages, h->bitsLength, h->base));
    if (nPages < 1)
	return -1;
    if (h->length > (verif->bitsLen<<3)>>h->logFrags) {
	epiz_ReportError(ss, 0, EPIZ_E_BITMAPTOOLARGE);
	ss->flags |= EPIZ_SS_REPLACE_BITMAP;
	return 0;
    }
    for (pageCount = 0; pageCount < nPages; ++pageCount) {
	ss->bitmapPage = pageCount;
	ss->bitmapBlock = 0;
	code = epia_Map(bitmapAnode, pageCount, &baseAddr, 0);
	if (code != 0) return code;
	ss->bitmapBlock = baseAddr;
	if (epix_IsBlockEmpty(baseAddr)) return EPI_E_NOBLOCK;
	if (EPIX_IsBlockInBacking(baseAddr) ||
	    epix_IsBlockViaBacking(baseAddr)) {
	    printf("Bitmap block %lu was backed!  Aborting.\n", pageCount);
	    return -1;
	}
	BEGIN_CRIT(code = elbb_Read(epia_AnodeDevice(bitmapAnode), baseAddr,
				      elbb_bufLogged, &buf),
		   code = elbb_Release(buf, elbb_lazy)) {
	    page = (struct bitmapPage *) elbb_BufferData(buf);
	    if ((!BITMAP_PAGE_OK(page, h)) ||
		(((page->baseOffset - h->base) / page->blocksPerPage)
		 != pageCount)) {
		printf("Bitmap page %lu/%#lx not ``OK''!  Aborting.\n",
			pageCount, baseAddr);
		code = -1;
		continue;
	    }
	    BlocksHere = MIN(page->blocksPerPage,
			     h->length + h->base - page->baseOffset);

	    /* First check the logged map if there is one. */
	    if (h->loggedLength > 0) {
		daddr_t blkno;		/* page relative */
		HighIx = BlocksHere >> 3; /* a bit per block */
		IxOffs = (page->baseOffset-h->base) >> 3;
		afsl_PAssert((HighIx <= h->loggedLength) &&
			      (HighIx+IxOffs <= verif->loggedLen),
 ("HighIx=%d, IxOffs=%d (>) loggedLength=%d, loggedLen=%d",
  HighIx, IxOffs, h->loggedLength, verif->loggedLen));
		for (blkno = 0; blkno < BlocksHere; blkno++) {
		    u_char mask = 1 << (blkno & 0x7);
		    u_long fragno;	/* verif bits relative */
		    int partial;

		    fragno = (blkno + page->baseOffset - verif->bitmapOffset)
			<< h->logFrags;
		    partial = IsBlockPartiallyFree(h, verif->bits, fragno);
		    Ix = blkno >> 3;
		    ss->bitmapIndex = Ix;
		    if (partial &&
			((page->logged[Ix] & mask) !=
			 (verif->logged[Ix+IxOffs] & mask))) {
			epiz_ReportError(ss, 0, EPIZ_E_BADLOGGED,
					  blkno+page->baseOffset,
					  verif->logged[Ix + IxOffs] & mask,
					  page->logged[Ix] & mask);
			if (ss->flags & EPIZ_SS_SALVAGE) {
			    /* We avoid suppressing following errors in the
			     * same byte by complementing only the offending
			     * bit. */
			    long off;
			    u_char newByte;

			    off = offsetof(struct bitmapPage, logged[0]) + Ix;
			    newByte = page->logged[Ix] ^ mask;
			    code = elbb_Modify(buf, off,
						(caddr_t)&newByte, 1 /*byte*/,
						elbb_zeroTrans, 0, &buf);
			    if (code != 0)
				return (code);
			}
		    }
		}
	    }

	    /* Then check the fragment bitmap. */
	    HighIx = (BlocksHere << h->logFrags) >> 3;
	    IxOffs = ((page->baseOffset-h->base) << h->logFrags) >> 3;
	    afsl_PAssert((HighIx <= h->bitsLength) &&
			  (HighIx+IxOffs <= verif->bitsLen),
			  ("HighIx=%d, IxOffs=%d (>) bitsLength=%d, bitsLen=%d",
			   HighIx, IxOffs, h->bitsLength, verif->bitsLen));
	    for (Ix = 0; Ix < HighIx; ++Ix) {
		ss->bitmapIndex = Ix;
		if (BitsArray(page, h)[Ix] != verif->bits[Ix + IxOffs]) {
		    FragNo = (pageCount * BitsPerPage) + (Ix * 8) +
			(h->base * h->fragmentsPerBlock);
		    epiz_ReportError(ss, 0, EPIZ_E_BADBITS,
				      FragNo >> h->logFrags,
				      FragNo & (h->fragmentsPerBlock-1),
				      verif->bits[Ix + IxOffs],
				      BitsArray(page, h)[Ix]);
		    if (ss->flags & EPIZ_SS_SALVAGE) {
			long off;
			caddr_t addr;
			off = offsetof(struct bitmapPage, logged[0]) +
			    h->loggedLength + Ix;
			addr = (caddr_t)&(verif->bits[Ix + IxOffs]);
			code = elbb_Modify(buf, off, addr, 1 /*byte*/,
					   elbb_zeroTrans, 0, &buf);
			if (code != 0)
			    return (code);
		    }
		}
	    }
	} END_CRIT;
	if (code)
	    return code;
    }
    return 0;
}
#endif /* not KERNEL */
