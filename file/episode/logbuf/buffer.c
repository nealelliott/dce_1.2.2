/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */

/* buffer.c */

/*
 * Copyright (C) 1995, 1990 Transarc Corporation
 * All Rights Reserved
 */

/*
 * The internals of this package are described in the Episode Buffer/Log
 * Design document.  One major difference is that this implementation uses
 * the prefix "buffer" rather than "buf" to avoid confusion with existing
 * UNIX code.
 */

#include <dcedfs/param.h>
#include <dcedfs/sysincludes.h>
#include <dcedfs/lock.h>
#include <dcedfs/osi.h>
#include <dcedfs/osi_uio.h>
#include <dcedfs/debug.h>
#include <dcedfs/hash.h>
#include <dcedfs/episode/epi_thread.h>
#include <dcedfs/episode/async.h>
#include <dcedfs/episode/instrument.h>
#include <dcedfs/episode/fifo.h>
#include <dcedfs/episode/reserve.h>

#define DEBUG_THIS_FILE LOGBUF_DEBUG_BUFFER

#include <elb.h>
#include <logrec.h>
#include <tran.h>

RCSID ("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/logbuf/RCS/buffer.c,v 4.273 1995/05/30 16:30:07 blake Exp $")

/*
 * debugging - default is no debugging; if AFS_DEBUG is enabled
 * we track start tran records.
 */
#ifdef AFS_DEBUG
int afsdb_episode_logbuf = (1 << LOGBUF_DEBUG_STARTRAN);
#else
int afsdb_episode_logbuf = 0;
#endif /* AFS_DEBUG */

/* define short hands for debugging output and verbose version thereof. */

#define DBP logbuf_DBprint
#define DBPV(p) dmprintf(afsdb_episode_logbuf, LOGBUF_DEBUG_VBUFFER, p)

#if !defined(KERNEL)

/* Reschedule -- see ../anode/file.c. */

static void Reschedule ()
{
    if (afsdb_episode_logbuf & (1<<(LOGBUF_DEBUG_ADDPREEMPTS-1))) {
	osi_Reschedule ();
    }
}
#else
#define Reschedule()
#endif

/*
 * struct bufferPoolEntry -- describes a the collection of all buffer of a
 *     specific size.  These are allocated in an array index by the log of the
 *     buffer size (e.g. 0 => 1K buffers, 1 => 2K, 6 => 64K).  The first
 *     several fields are used in normal operation for locating buffers of this
 *     size.
 *	   blockList -- all the 128K blocks that are allocated to this pool.
 *	   freeList -- buffers that are not in use.  The inuse bit is false iff
 *	       the buffer is on the freeList.  The refCount of such buffers is
 *	       always zero, although some zero refCount buffers may
 *	       (transiently) not be on the free list.
 *     All the rest of the fields are for managing space allocation and for
 *     collecting statistics.  Explanations for the more complex ones follow:
 *	 bufferHits -- number of times a call to ReadGeneral found the buffer
 *	     it was looking for.
 *	 bufferMisses -- number of times a call to ReadGeneral needed to
 *	     allocate a new buffer to hold the requested data.
 *	 <bufferClock> -- this is not an actual field.  The sum of bufferHits
 *	     and bufferMisses gives the total number of calls to ReadGeneral
 *	     for a specific block size.  It is used as a per buffer pool
 *	     clock to time events affecting this pool.  Primarily this allows
 *	     buffer ages to be calculated.  See below.
 *	 lastHitsMisses -- The sum of hits and misses last time
 *	     MoreSpaceDesire was called on this pool.  Used to determine when
 *	     a pool is inactive.
 *	 recycleAge -- a decaying average of the age of all buffers when they
 *	     are used by AllocBuffer to hold a different block.  Generally
 *	     this will be the length of the buffer pool divided by the miss
 *	     rate.  This is because buffers are recycled in LRU order and a
 *	     buffer is recycled on every miss and the buffer clock ticks an
 *	     average of 1/missRate times between every miss.  See the
 *	     discussion under the variable DecayRate1 for an explanation of
 *	     the actual value of this variable.
 *	 reuseAge -- a decaying average of the age of all buffers that are
 *	     found by ReadGeneral already holding the requested data.  See
 *	     also DecayRate1.
 *	 missRatio -- a decaying average of 100 * number of misses + 0 *
 *	     number of hits.  This effectively gives misses/(hits+misses) as
 *	     a percent (since we're using integer arithmetic this is
 *	     easiest).  See also DecayRate1.
 *	 costRatio -- a decaying average of the cost (in arbitrary units)
 *	     when AllocBuffer must recycle a buffer.  See also DecayRate1.
 *	 moreSpaceDesire -- a number representing the desire of the pool for
 *	     more space.  Roughly speaking a value of 100 means more space is
 *	     needed.  Larger number reflect greater urgency.  This number is
 *	     calculated by the MoreSpaceDesire procedure, see which.
 *	 nextMore -- a down counter set in NeedMoreBufferSpace, which is
 *	     decremented and testing in AllocBuffer to determine if the
 *	     (expensive) GetMoreBufferSpace procedure should be called.  This
 *	     number is set relative to the parameter MoreSpaceInterval, see
 *	     which.
 *       bufferCreateBlocks -- number of times elbb_ReadGeneral was called with
 *           READDISK false (a buffer cache miss is assumed).  This is the
 *           elbb_CreateBlock path used to initialize newly allocated storage.
 *       bufferAllocWrites -- number of writes done from AllocBuffer to
 *           maintain a pool of clean buffers.
 *       bufferWrites -- number of writes of dirty buffers (a superset of the
 *           above writes).
 *       bufferForces -- number of log forces required to write buffers.
 *
 *     This data structure is only used internally, but must be placed at the
 *     top in order to assuage the complaints and failed generation of some
 *     compilers.
 */

struct bufferPoolEntry {
    unsigned		 logBlkSize;	/* log (base 2) block size */
    struct fifo		 blockList;	/* list of bufferBlocks */
    struct fifo		 freeList;	/* freeList fifo */
    epi_cond_t		 cond;		/* for sleeping */

    /* buffer reservation information */

    struct epis_ReservationData reserved;   /* current reservation info */
    unsigned		    global_res; /* # of blocks globally reserved */
    unsigned		    res_allocs; /* some thread adjusting res. */
    unsigned		    res_wtrs;   /* # of threads waiting */

    /* tuning and statistics variables */

    u_long   bufferTotal;		/* total # of buffers of this size */
    u_long   bufferCreateBlocks;	/* # Read calls w/ READDISK false */
    u_long   bufferAllocWrites;		/* # Writes cleaning bufs in Alloc */
    u_long   bufferAllocs;		/* total # of buffer alloc calls */
    u_long   bufferHits;		/* "hits" into buffer pool */
    u_long   bufferMisses;		/* "misses" into buffer pool */
    u_long   bufferInUse;		/* # buffers looked up & in use */
    u_long   lastHitsMisses;		/* H+M when desire last updated */
    long     recycleAge;		/* avg. age when buffer is recycled */
    long     reuseAge;			/* avg. age when buffer is reused */
    long     poolSize;			/* avg. free list length */
    long     missRatio;			/* avg. of misses/(hits+misses) *100 */
    long     allocCost;			/* avg. of cost to relaim buffer */
    u_long   moreSpaceDesire;		/* >=100 means yes */
    int32    nextMore;			/* get more space when zero */
    u_long   bufferWrites;		/* # Writes cleaning all bufs */
    u_long   bufferForces;		/* # log forces before write */

    /* not used */

    osi_dlock_t bpeLock;		/* buffer pool entry lock */
};
#define bpe_BlockSize(bpe) (BUF_MINBLKSIZE<<(bpe)->logBlkSize)

struct bufferBlock {
    struct fifo buffers;		/* this block's buffer list */
    struct bufferPoolEntry *poolP;      /* pointer to the correct pool */
    struct fifo_Thread poolThread;      /* blocks threaded in the pool */
    struct fifo_Thread blockListThread; /* free or active */
    u_int32 reclaimCost;		/* used when computing across all */
};

#define RECLAIM_MAX (100000)   /* above this the cost is too high */
#define RECLAIM_WRITEP (100)   /* cost to reclaim a buffer w/ write pending */
#define RECLAIM_DIRTY (200)    /* cost to reclaim a dirty buffer */
#define RECLAIM_INUSE (500)    /* cost to use a referenced buffer */

/*
 * Internal data structures + macros for buffer aging.
 */

/* internal functions, forward referenced */

static int32 AllocBuffer(struct bufferPoolEntry *, struct buffer **);
static void DeallocBuffer(struct buffer *);
static void SetType(struct buffer *, enum elbb_bufType);
static int32 Write(struct buffer *);
static int32 InitBufferAllocator(void);
static void FreeBufferHeader(struct buffer *);
static struct bufferPoolEntry *GetBufferPool(u_int32 bufSize);
static struct buffer *AllocBufferHeader(void);
static void FreeBufferSpace(struct bufferBlock *);
static void InitBufferHeader(struct buffer *);
static void ReinitBufferHeader(struct buffer *);
static void CostToFreeBlock(struct bufferBlock *);
static void DeallocLogBuffer(struct buffer *, int);
static int HandleLogFull(async_device_t *dP, int wait);
static int32 SetFHLocked(struct buffer *, u_int32);
static int WaitForWrite(struct buffer *bP, int wait, int32 *retvalP);
static void WaitForUpdate(struct buffer *bP);
static long AddBufferToSet(struct buffer *bP, asevent_set_t **setP);
static void DeleteBufferSet(asevent_set_t **setP);
static void CheckIOError(long code, struct buffer *bP);
static u_long HashAddress(struct buffer *bP);
static u_long HashFH(struct buffer *bP);

/*
 * heuristics stuff
 *
 * The average is computed using two possible values to tune the decay rate;
 * this allows us to represent a wider range of possible numbers, although not
 * a complete set.  This two-rate mechanism is currently unused.
 *
 * Variable constant definitions.
 *
 * DecayRate1 -- amount that the running average decays by.  This constant is
 *     used by the DAVERAGE_ADD and DAVERAGE_GET macros, below.  The basic
 *     behavior is that if ADD is called 2^DecayRate1 times with a constant
 *     value then the running average (assuming it was started at zero) will
 *     reach 63% of the constant value (this is 1/e).  If ADD is called
 *     DecayRate1 time again it will reach 86% (1/e^2), etc.  What this means
 *     is that the variable contains a number which is roughly 2^DecayRate1
 *     times the measured average.  This is the purpose of the correction
 *     applied by the DAVERAGE_GET macro.
 *
 * MinHitRatio -- minimal target for the ratio between the number hits and
 *     misses.  The MoreSpaceDesire procedure will try to make the hit rate
 *     larger than the miss rate by this multiple.  The use of this constant is
 *     explained further in the procedure comment for MoreSpaceDesire.
 *
 * MinPoolLength -- minimal target for the length of the buffer pool's freeList
 *     expresses in units of the PoolRatio (see below).  The MoreSpaceDesire
 *     procedure will try to make the buffer pool freelist's length longer than
 *     MinPoolLength times PoolRatio.  The use of this constant is explained
 *     further in the procedure comment for MoreSpaceDesire.
 *
 * PoolRatio -- target for the ratio between reuse age and recycle age.  The
 *     MoreSpaceDesire procedure will try to grow the buffer pool until the
 *     recycle is at least PoolRatio times as big as reuse age.  The use of
 *     this constant is explained further in the procedure comment for
 *     MoreSpaceDesire.
 *
 * logInactiveDecayInterval -- this number characterizes the speed (expressed
 *     as the log of the inverse of the rate) with which the desire of inactive
 *     buffer pools for more space decays towards zero.  This decay is relative
 *     to the global clock.  Thus for every 2^logInactiveDecayInterval calls to
 *     ReadGeneral the last calculated value of moreSpaceDesire of each pool is
 *     reduced by 1/8th (12.5%).
 *
 *     Note that generally there should be some connection between the size of
 *     this parameter and the DecayRate1 paramter.  Since the decaying averages
 *     (e.g. recycleAge) respond to changes in order of 2^DecayRate1 clock
 *     ticks (per pool), one would expect inactive pools to decay toward zero
 *     with about the same time constant (according to the global clock).  This
 *     similarity, however, much take account of the difference in granularity.
 *     The current settings are DecayRate1 is 8 so a sudden change in the input
 *     to a decaying average would reach 68% of its steady state value in 2^8
 *     (256) clock ticks.  The logInactiveDecayInterval is set to (8-3) 5 so
 *     that after 2^5 * 2^3 (256) global clock ticks a inactive pool would
 *     decay by about 66% (1 - (1-12.5)^8).
 *
 *     This is controled in the GetMoreBufferSpace and MoreSpaceDesire
 *     procedures, see which.
 *
 * MoreSpaceInterval -- the base interval (in calls to AllocBuffer) between
 *     calls to NeedMoreBufferSpace.  The actual value used for a pool is
 *     determined after each call to GetMoreBufferSpace depending on its
 *     result.  It is set to some multiple of this interval.  See
 *     NeedMoreBufferSpace for details of this calculation.
 */

static int DecayRate1 = 8;		/* log(resp. time of decaying avgs) */
static int DecayRate2 = 32;		/* not used */
static int ClockMult = 3;		/* not used */
static int MinHitRatio = 4;		/* target minimim hit to miss ratio */
static int MinPoolLength = 2;		/* base len. in units of pool ratio */
static int PoolRatio = 20;		/* recycle/reuse ratio AllocBuffer */
static int logInactiveDecayInterval = 5; /* to decay desire */
static int MoreSpaceInterval = 200;	/* after this # calls to AllocBuffer */

static u_long bufferClock;		/* global buffer clock */

static int returnAnyUndoZeroTrans;	/* don't keep undo-zero trans */

static struct epis_ReservationData globalBlockRes;

/*
 * x' = xn - x + y
 */
#define DAVERAGE_ADD(x, y) ((x) - ((x) >> DecayRate1) + y)

/*
 * we adjust for the decay rate
 */
#define DAVERAGE_GET(x) ((x) >> DecayRate1)

#define RECYCLE(bP, bpeP) do { \
    bufferClock++; \
    bpeP->bufferMisses++; \
    bpeP->recycleAge = DAVERAGE_ADD(bpeP->recycleAge, \
				    bpeP->bufferHits + \
				    bpeP->bufferMisses - \
				    (bP)->whenFreed); \
    bpeP->missRatio = DAVERAGE_ADD (bpeP->missRatio, 100); \
} while (0)
#define REUSE(bP, bpeP) do { \
    bufferClock++; \
    bpeP->bufferHits++; \
    if (bufferIsInuse(bP)) bpeP->bufferInUse++; \
    bpeP->reuseAge = DAVERAGE_ADD(bpeP->reuseAge, \
				  bpeP->bufferHits + \
				  bpeP->bufferMisses - \
				  (bP)->whenFreed); \
    bpeP->missRatio = DAVERAGE_ADD (bpeP->missRatio, 0); \
} while (0)
#define POOLSIZE(bpeP) do { \
    bpeP->poolSize = DAVERAGE_ADD(bpeP->poolSize, \
				  bpeP->freeList.length); \
} while (0)

/* RemoveAgeQ -- cleanup age queue and undo zero tran for a buffer that is no
 *     longer dirty.  First, if there is an undo-zero transaction, end it and
 *     clear the uzTran field.  Then remove the buffer from the age queue.  The
 *     safety of removing the buffer from the age queue depends on different
 *     reasoning for undo-zero and metadata buffers.
 *
 *     If this is an undo zero buffer then the log records may not have
 *     committed.  This is okay because we have cleared the dirty bit and are
 *     about to end the transaction.  Forcibly removing the buffer from the age
 *     queue will cause the log to forget about the undo zero transaction if
 *     this page is soon overwritten.
 *
 *     In the case of bona fide metadata buffers updates to those buffer must
 *     have committed by the time we clean the buffer.  If it was written out,
 *     we forced the log first.  If is being killed, we waited for the
 *     transaction that freed the metadata block to commit first, hence any
 *     prior updating transactions have also committed. */

#define RemoveAgeQ(bP) \
do { \
    int force = 0; \
    if (!elbt_IsNullTranId((bP)->uzTran)) { \
	elbt_EndTran((bP)->uzTran); \
	(bP)->uzTran = elbt_NullTranId; \
	force = 1; /* okay to force AgeQ removal */ \
    } \
    if (bP->currentAgeQ) elbl_UnageBuffer(bP, force); \
} while (0)

#define AddToFreeList(bP) \
do { \
    afsl_Assert(((bP)->refCount == 0) && bufferIsInuse(bP)); \
    FIFO_ADD(&((struct bufferPoolEntry *)((bP)->poolP))->freeList, \
	     &(bP)->freeList); \
    bufferClearInuse(bP); \
} while (0)

#define RemoveFromFreeList(bP) \
do { \
    afsl_Assert(((bP)->refCount == 0) && !bufferIsInuse(bP)); \
    FIFO_REMOVE(&((struct bufferPoolEntry *)((bP)->poolP))->freeList, \
		&(bP)->freeList); \
    bufferSetInuse(bP); \
} while (0)

#define RemoveFromAddrHash(bP) \
do { \
    if ((bP)->adev) { \
	(void) dfsh_HashOut_r(&addrHashTable, (bP)); \
	(bP)->adev = NULL; \
	(bP)->blockNum = 0; \
    } \
} while (0);

/* FORALL -- loops over all buffers for a device using a pair of loops, one
 *     over the block list for the pool and the other over the buffers in each
 *     block.  To restart the loop, set vbl to NULL and continue.  To break out
 *     of the loop, leave vbl non-NULL and break.  We inline the FIFO_FOR loop
 *     instead of using the above macro so we can exit the inner loop when vbl
 *     is cleared. */

#define FORALL(vbl, poolP, dev) MACRO_BEGIN \
    struct bufferBlock *macro__bbP; \
    for (macro__bbP = \
	 FIFO_LookAtOldest(&(poolP)->blockList, \
			   offsetof(struct bufferBlock, poolThread)); \
	 macro__bbP; \
	 macro__bbP = (vbl ? NULL /* break */ \
		       : FIFO_NextOldest(&(poolP)->blockList, macro__bbP, \
					 &macro__bbP->poolThread))) \
	for (vbl = FIFO_LookAtOldest(&macro__bbP->buffers, \
				     offsetof(struct buffer, blockThread)); \
	     vbl; \
	     vbl = (vbl ? FIFO_NextOldest(&macro__bbP->buffers, vbl, \
					  &vbl->blockThread) \
		    : (macro__bbP = NULL) /* restart loop */)) \
	    if (vbl->adev == (dev)) {
#define FORALL_END } MACRO_END

/*
 * LogWrite -- handles calling elbl_Write in a loop which could possibly fail
 *     many times before there is enough log space to write the log record
 *     (this is exactly the case for small high-contention logs.) Note that the
 *     caller should check the return value, rv, to make sure some other error
 *     didn't occur.
 *
 *     The first processing error is that the log is so full it cannot write
 *     the requested log record; this is handled by forcing buffers to disk.
 *
 *     The second processing error is merely a warning that the log is filling
 *     up and should be written "real soon now".
 *
 * LOCKS USED -- Must be called with buffer lock held, but can drop it both
 *     before and after writing the requested data to the log.
 */

#define LogWrite(tid, type, len, off, bP, newp, rv, lf)\
do {\
    struct elbt_TranRec *tP;\
    BUFFER_ASSERTLOCKED();\
    afsl_Assert((bP)->refCount);\
    while (1) {\
        if (bufferIsWritep(bP)) WaitForWrite(bP, 1, NULL);\
	else if (bufferIsUpdate(bP)) WaitForUpdate(bP); \
	else break; \
    }\
    bufferSetUpdate(bP); \
    BUFFER_UNLOCK(); \
    tP = elbt_GetTran(tid, 1); \
    if (tP) {\
	(rv) = elbl_WriteWithBuffer(tP, type, len, off, bP, newp, &lf);\
	afsl_Assert(!bufferIsWritep(bP));\
	elbt_ReleaseTran(tP);\
    } else {\
	(rv) = BUF_OLDTRAN;\
    }\
    BUFFER_LOCK(); \
    bufferClearUpdate(bP); \
    if (bufferIsNotify(bP)) { \
	bufferClearNotify(bP); \
	epi_cond_broadcast(&bP->cond); \
    } \
    if ((rv) == LOG_LOGFULL) {\
	HandleLogFull((bP)->adev, 1);\
    }\
} while((rv) == LOG_LOGFULL)

#define LogWriteX(tid, type, len, dp, block, off, oldp, rv, lf)\
do {\
    struct elbt_TranRec *tP; \
    BUFFER_UNLOCK(); \
    tP = elbt_GetTran(tid, 1); \
    if (tP) {\
	(rv) = elbl_WriteExtended(tP, type, len, block, off, oldp, 0, &lf);\
	elbt_ReleaseTran(tP); \
    } else {\
	(rv) = BUF_OLDTRAN;\
    }\
    BUFFER_LOCK(); \
    if ((rv) == LOG_LOGFULL) { \
	HandleLogFull((dp), 1); \
    } \
} while((rv) == LOG_LOGFULL)

/*
 * Internally used data structures.
 */

static struct fifo ageQHead; /* for ageing buffers */

/* Sizes: 1k, 2k, 4k, 8k, 16k, 32k, 64k */
static struct bufferPoolEntry bufferPool[7];
static u_int nBufferPools = sizeof(bufferPool)/sizeof(bufferPool[0]);
static char bufferPoolInit; /* true when initialized */

/*
 * These two structures will replace the 128k buffer size.  A 128K
 * chunk will be one either the free or active BlockList.  When we
 * are salvaging memory we can then run down the active block list
 * & compute the exact cost to salvage each block.
 */
static struct fifo freeBlockList;
static struct fifo activeBlockList;

/*
 * the value of BUF_BLOCKSIZE is assumed to be >= BUF_MAXBUFSIZE.
 */
#define BUF_BLOCKSIZE (2 * BUF_MAXBUFSIZE)

/*
 * There are several types of locks in the buffer system.  One is the
 * global buffer lock.  Another is the morespaceLock.  Additionally, there
 * is an (as yet unused) lock in each buffer.  In fact, it appears there is
 * some need for a more generalized lock for the buffer pool itself.
 */

/* local data */
static epi_mutex_t bufferLock;		/* global lock */
static osi_dlock_t morespaceLock;	/* for GetMoreBufferSpace */

#define BUFFER_LOCK() epi_mutex_lock(&bufferLock)
#define BUFFER_UNLOCK() epi_mutex_unlock(&bufferLock)
#define BUFFER_SLEEP(cv) epi_cond_wait (cv, &bufferLock)
#define BUFFER_HAS_OWNER() epi_mutex_hasowner(&bufferLock)
#define BUFFER_ASSERTLOCKED() afsl_Assert (epi_mutex_held(&bufferLock))

/*
 * address hash table
 * buffer file handle hash table
 */
dfsh_hashTable_t addrHashTable;
dfsh_hashTable_t FH_HashTable;

#define LookupBuffer(dP, blkno, bP) \
DFSH_LOOKUP(struct buffer *, &addrHashTable, bP, \
	    (unsigned)(dP) ^ (unsigned)(blkno), \
	    (((bP)->adev == (dP)) && (((bP)->blockNum) == (blkno))))


/* list of valid indices for ins_elbb procedure */

#define BUFTOTAL 1
#define BUFCREATEBLOCKS 2
#define BUFALLOCWRITES 3
#define BUFRECYCLEAGE 4
#define BUFREUSEAGE 5
#define BUFPOOLSIZE 6
#define BUFALLOCS 7
#define BUFHITS 8
#define BUFMISSES 9
#define BUFINUSE 10
#define BUFMISSRATIO 11
#define BUFMORESPACEDESIRE 12
#define BUFNEXTMORE 13
#define BUFMAXTRANS 14			/* from tran.c: elbt_maxTrans */
#define BUFALLOCCOST 15
#define BUFWRITES 16
#define BUFFORCES 17

/*
 * the list of tunable global variables in the buffer package. These are
 * supported by ins_elbb_tune procedure.  Per buffer Pool variables are
 * exported by the ins_elbb procedure.
 */
static struct ins_varTableEntry varTable[] = {
    {/* 0 */(long *)&afsdb_episode_logbuf, sizeof(afsdb_episode_logbuf)},
    {/* 1 */(long *)&DecayRate1, (long) sizeof(DecayRate1)},
    {/* 2 */(long *)&PoolRatio, (long)sizeof(PoolRatio)},
    {/* not used */(long *)&ClockMult, (long)sizeof(ClockMult)},
    {/* 3 */(long *)&MinHitRatio, (long)sizeof(MinHitRatio)},
    {/* 4 */(long *)&MinPoolLength, (long)sizeof(MinPoolLength)},
    {/* 5 */(long *)&logInactiveDecayInterval, (long)sizeof(int)},
    {/* 6 */(long *)&MoreSpaceInterval, (long)sizeof(int)},
};

static long varTableSize = sizeof(varTable)/sizeof(struct ins_varTableEntry);

#define MAX_BUFFER_POOL_SIZE_IN_K bpe_BlockSize(&bufferPool[nBufferPools-1])
#define GETNEXT(num) ((num == MAX_BUFFER_POOL_SIZE_IN_K) ? 0: (num << 1))

/* ins_elbb -- hook for implementing monitoring and tuning specific to a buffer
 *     pool.  The specified instance is the block size of the desired pool in
 *     K.  Use ins_elbb_tune for global parameters.
 */

/* EXPORT */
int ins_elbb(
  IN struct ins_opDesc *opcode,
  IN struct ins_varDesc *vars,
  IN struct ins_basicObjectId *argObjIds,
  OUT struct ins_basicObjectId *resultObjIds,
  OUT struct ins_basicObject *resultValues)
{
    long *dataAddr;
    long dataSize;
    int error = 0;
    long instance = 0;
    long index;

    dataAddr = (long *)0;
    dataSize = 0;

    if (argObjIds->objId.length >= sizeof(long)) {
	/* Get instance if present. */
	bcopy((caddr_t)argObjIds->objId.value,
	      (caddr_t)&instance, sizeof (instance));
    }
    index = 0;
    if (vars->varIdx != BUFMAXTRANS) {

	/* Translate instance into a buffer pool index. */

	int i;				/* search for pool index */

	index = -1;
	for (i=0; i<nBufferPools; i++)
	    if (1<<i >= instance) {
		index = i;
		instance = 1<<i;
		break;
	    }
	if (index == -1)
	    /* Should advance to next objId.0 but we aren't handling this right
	     * yet.  So return an error instead. */
	    return EINVAL;
    }

#define CONST_TO_FIELD(field) \
    dataAddr = (long *) &(bufferPool[index].field), \
    dataSize = sizeof(bufferPool[index].field)

    if(vars->varIdx) {
	switch(vars->varIdx){
	  case BUFTOTAL: CONST_TO_FIELD (bufferTotal); break;
	  case BUFCREATEBLOCKS: CONST_TO_FIELD (bufferCreateBlocks); break;
	  case BUFALLOCWRITES: CONST_TO_FIELD (bufferAllocWrites); break;
	  case BUFRECYCLEAGE: CONST_TO_FIELD (recycleAge); break;
	  case BUFREUSEAGE: CONST_TO_FIELD (reuseAge); break;
	  case BUFPOOLSIZE: CONST_TO_FIELD (poolSize); break;
	  case BUFALLOCS: CONST_TO_FIELD (bufferAllocs); break;
	  case BUFHITS: CONST_TO_FIELD (bufferHits); break;
	  case BUFMISSES: CONST_TO_FIELD (bufferMisses); break;
	  case BUFINUSE: CONST_TO_FIELD (bufferInUse); break;
	  case BUFMISSRATIO: CONST_TO_FIELD (missRatio); break;
	  case BUFALLOCCOST: CONST_TO_FIELD (allocCost); break;
	  case BUFMORESPACEDESIRE: CONST_TO_FIELD (moreSpaceDesire); break;
	  case BUFNEXTMORE: CONST_TO_FIELD (nextMore); break;
	  case BUFWRITES: CONST_TO_FIELD (bufferWrites); break;
	  case BUFFORCES: CONST_TO_FIELD (bufferForces); break;
	  case BUFMAXTRANS: {
	      extern long elbt_maxTrans;
	      dataAddr = &elbt_maxTrans;
	      dataSize = sizeof(long);
	      break;
	  }
	  default:
	    return EINVAL;
	}
	if(dataAddr && dataSize && dataSize <= INS_MAXOBJLEN){
	    if(opcode->opcode == INS_SET){
		/* read-only variables */
		return EINVAL;
	    }
	    else if (opcode->opcode == INS_GETNEXT){
		instance = GETNEXT(instance);
	    }
	    else if (opcode->opcode != INS_GET){
		return EINVAL;
	    }
	    bcopy((caddr_t)dataAddr, (caddr_t)resultValues->value, dataSize);
	    bcopy((caddr_t)&instance, (caddr_t)resultObjIds->objId.value,
		sizeof (instance));
	    if (instance) {
		resultObjIds->objId.type = INS_DT_ULONG;
	    } else{
		resultObjIds->objId.type = INS_DT_GETNEXT_DROPOFF;
	    }
	    resultObjIds->objId.length = sizeof(long);
	}
    }

    return error;
}


/*
 * get or set the value of variable indicated by <vars>->varIdx. The
 * result of lookup is stored in <resultValues>. Object Id parameters
 * are ignored for now.
 */
/* EXPORT */
int ins_elbb_tune(
  IN struct ins_opDesc *opcode,
  IN struct ins_varDesc *vars,
  IN struct ins_basicObjectId *argObjIds,
  OUT struct ins_basicObjectId *resultObjIds,
  OUT struct ins_basicObject *resultValues)
{

    if (vars->varIdx >= 0 && vars->varIdx < varTableSize) {
	/* the variable index is within range */
	/* get any locks required */
	if(opcode->opcode == INS_GET){
	    bcopy((caddr_t)varTable[vars->varIdx].varAddr,
		(caddr_t)resultValues->value,
		varTable[vars->varIdx].varSize);
	} else if (opcode->opcode == INS_SET) {
	    bcopy((caddr_t)resultValues->value,
		(caddr_t)varTable[vars->varIdx].varAddr,
		varTable[vars->varIdx].varSize);
	} else if (opcode->opcode == INS_GETNEXT) {
	    /* not yet supported */
	}
	/* drop any locks acquired */

	return 0;
    } else{
	return EINVAL;
    }
}


/*
 * elbb_Init -- initialize buffer package with given memory pool.
 *
 * LOCKS USED -- None assumed; this package initializes buffer lock.
 *
 * This routine is called to intialize the buffer package.
 *
 * Perhaps this should be generalized (e.g. VM, etc.) ? -wam 3/8/91
 *
 */

struct icl_set *elb_iclSet;

/* EXPORT */
int32 elbb_Init(struct elbb_params *elbbParams)
{
      caddr_t startAddr = elbbParams->bufferPool;
      u_int32 byteCount = elbbParams->byteCount;
      unsigned blockCount = (byteCount / (2 * BUF_MAXBUFSIZE));
      unsigned i;
      long code;
      struct bufferBlock *bbP;
      struct buffer *bP = (struct buffer *)0;
      struct icl_log *logp;

      if (bufferPoolInit)
	  return BUF_REINIT;
      bufferPoolInit = 1;

      asevent_Init();			/* initialize async layer */

      icl_CreateLogWithFlags_r("disk", 0, /*flags*/0, &logp);
      icl_CreateSetWithFlags_r("episode/logbuf", logp, 0,
			    ICL_CRSET_FLAG_DEFAULT_OFF, &elb_iclSet);

      epi_mutex_init(&bufferLock, "buffer");
      lock_InitDesc(&morespaceLock, "get more buffer space");

      BUFFER_LOCK();

      logbuf_pin(startAddr, byteCount);

      startAddr = ALIGN(caddr_t, startAddr);

      /*
       * We initialize the buffer allocation system by initializing
       * the block lists and then threading the blocks onto the free
       * list.
       */

      FIFO_INIT(&freeBlockList,
		offsetof(struct bufferBlock, blockListThread));
      FIFO_INIT(&activeBlockList,
		offsetof(struct bufferBlock, blockListThread));

      InitBufferAllocator();

      for (i = 0; i < blockCount; i++) {
	  /*
	   * allocate and initialize the block.
	   */
	  bbP = osi_Alloc_r(sizeof(struct bufferBlock));
	  logbuf_pin(bbP, sizeof(struct bufferBlock));
	  bzero((caddr_t)bbP, sizeof (*bbP));
	  bbP->reclaimCost = 0; /* not in use */
	  FIFO_INIT (&bbP->buffers, offsetof(struct buffer, blockThread));
	  FIFO_INITTHREAD (&bbP->poolThread); /* not in a pool */
	  FIFO_INITTHREAD (&bbP->blockListThread);

	  /*
	   * thread onto free list
	   */
	  FIFO_ADD(&freeBlockList, &bbP->blockListThread);

	  /*
	   * get a buffer header
	   */
	  bP = AllocBufferHeader();
	  afsl_Assert(bP);

	  /*
	   * Initialize the new buffer
	   */
	  InitBufferHeader(bP);
	  bP->data = startAddr + (i * BUF_BLOCKSIZE);
	  FIFO_ADD(&bbP->buffers, &bP->blockThread);
      }

      /*
       * We now initialize each of the buffer pools.
       */
      for (i = 0; i < sizeof (bufferPool) / sizeof (bufferPool[0]); i++) {
	  bzero((caddr_t)&bufferPool[i], sizeof (bufferPool[i]));
	  FIFO_INIT (&bufferPool[i].freeList,
		     offsetof(struct buffer, freeList));
	  FIFO_INIT (&bufferPool[i].blockList,
		     offsetof(struct bufferBlock, poolThread));
	  bufferPool[i].logBlkSize = i;
	  epis_InitReservationData(&bufferPool[i].reserved, 0, 0);
	  epi_cond_init (&bufferPool[i].cond, "buffer pool");
      }

      /*
       * initialize age queue
       */
      FIFO_INIT(&ageQHead, offsetof(struct buffer, ageQ));

      /*
       * initialize address hash
       */

      code = dfsh_HashInit(&addrHashTable, HashAddress,
			   offsetof(struct buffer, addrHashNext));
      afsl_MBZ(code);

      /*
       * initialize file handle hash table
       */

      code = dfsh_HashInit(&FH_HashTable, HashFH,
			   offsetof(struct buffer, bufFHNext));
      afsl_MBZ(code);

      /*
       * note that for this key to work, we assume that the device and
       * block # fields are contiguous, with the device *first*.  We
       * also presume the types of the adev & blockNum fields.
       */
      code = dfsh_HashInitKeyFixed(&addrHashTable,
				   offsetof(struct buffer, adev),
				   sizeof(async_device_t *) +
				   sizeof(daddr_t));

      afsl_MBZ(code);

      /*
       * file handle hash table
       */

      code = dfsh_HashInitKeyFixed(&FH_HashTable,
				   offsetof(struct buffer, bufFH),
				   sizeof(u_int32));
      afsl_MBZ(code);

      /* initialize global buffer reservation system */
      epis_InitReservationData(&globalBlockRes, freeBlockList.length, 0);

      elbt_Init(0);			/* start tran module */
      elbl_Init();			/* start log module */

      BUFFER_UNLOCK();
      return (BUFFER_OK);
}

/*
 * elbb_Tune - interface to internal variables, constants, etc.
 *
 * Some of the variables controlled by elbb_Tune may not be decreased.
 * Note that elbb_Tune assumes the address of value is in its address
 * space.  copyin/copyout support is not implemented here (it could be added
 * or performed at a higher level.)
 */
/* EXPORT */
int32 elbb_Tune(
     IN u_int32 op,		/* operation */
     IN u_int32 type,		/* 0 = get; 1 = set */
     INOUT opaque *value)	/* pointer to appropriate data type */
{
      int32 code = BUFFER_OK;
      extern char *episode_buffer_version_date;
      extern char *episode_buffer_version_sum;
      extern unsigned episode_buffer_version;

      switch (op) {
	case BUFFER_DEBUG:
	  if (type == BUFFER_SETVAL)
	      afsdb_episode_logbuf = *(u_int32 *)value;
	  else /* BUFFER_GETVAL */
	      *(u_int32 *)value = afsdb_episode_logbuf;
	  break;
	case BUFFER_DECAY1:
	  if (type == BUFFER_SETVAL)
	      code = BUF_READONLYVAR;
	  else /* BUFFER_GETVAL */
	      *(u_int32 *)value = DecayRate1;
	case BUFFER_DECAY2:
	  if (type == BUFFER_SETVAL)
	      code = BUF_READONLYVAR;
	  else /* BUFFER_GETVAL */
	      *(u_int32 *)value = DecayRate2;
	  break;
	case BUFFER_VER_STR:
#ifdef VERSION
	  /* caller must copyin/copyout the version string */
	  *(u_int32 *)value = (u_int32)episode_buffer_version_sum;
	  break;
	case BUFFER_VER_DATE:
	  /* caller must copyin/copyout the version date string */
	  *(u_int32 *)value = (u_int32)episode_buffer_version_date;
	  break;
	case BUFFER_VER_NUM:
	  *(u_int32 *)value = episode_buffer_version;
#else
	  *(u_int32 *)value = 0;
#endif /* VERSION */
	  break;

	case BUFFER_MAXTRAN:
	case BUFFER_TRANSOFT:
	case BUFFER_TRANHARD:
	case BUFFER_MAXLOG:
	  /* these calls all belong to the log package */
	  code = elbl_Tune(op, type, value);
	  break;
	default:
	  code = BUF_BADOP;
	  break;
      }
      return code;
}

static long
WaitForRead(struct buffer *bP)
{
    asevent_event_t *ae = bP->aevent;
    long retval;

    BUFFER_UNLOCK();
    retval = asevent_WaitEvent(ae);
    BUFFER_LOCK();
    asevent_Release(ae);
    afsl_Assert(ae == bP->aevent);	/* no one has reused buf on us */
    bP->aevent = NULL;
    bufferClearReadp(bP);
    if (bufferIsNotify(bP)) {
	bufferClearNotify(bP);
	epi_cond_broadcast(&bP->cond);
    }
    if (retval != 0) {
	/* since we may have slept, we have to be careful here. */
	CheckIOError(retval, bP);
	BUFFER_RELEASE(bP);
	bufferSetDiscard(bP);
	if (bP->refCount == 0) {
	    DeallocBuffer(bP);
	}
    }
    return (retval);
}

/*
 * elbb_ReadGeneral is the general purpose read interface; the other's
 * are really only a veneer (in which the appropriate flags are set).
 * Currently, it supports three modes:
 *
 * (1) Just allocate a buffer (for overwriting an entire disk block.
 * (2) Allocate & read a buffer (waits for completion)
 * (3) Allocate & schedule read for a buffer (doesn't wait for completion).
 *
 * It checks to see if a buffer already exists for this block prior to
 * scheduling a read.
 *
 * Perhaps this is the correct place to add a new flag for "zero this page"
 * as an aid to security (e.g. reuse of pages.) - wam 3/7/91 XXX
 */
/* EXPORT */
int32 elbb_ReadGeneral(
  IN async_device_t *dP,		/* block device */
  IN daddr_t blkno,			/* virtual block on device */
  IN long options,
  IN enum elbb_bufType bt,		/* type of buffer needed */
  OUT struct buffer **bPP)		/* actual buffer */
{
      struct buffer *nbP;
      int32 slept;
      int32 retval;
      int initBufferContentsFromDisk;
      int dontWaitForRead;
      struct bufferPoolEntry *bpeP;

      /* only valid bits set */
      afsl_Assert ((options & ~BUFFER_RG_VALIDBITS) == 0);

      initBufferContentsFromDisk = (options & BUFFER_RG_READDISK);
      dontWaitForRead = (options & BUFFER_RG_DONTWAIT);
      bpeP = GetBufferPool(dP->blkSize);
      BUFFER_LOCK();
    loop:

      /* lookup the buffer */
      LookupBuffer(dP, blkno, nbP);
      if (nbP == 0) { /* if the buffer wasn't found... */

	  slept = AllocBuffer(bpeP, &nbP);
	  if (slept)
	      goto loop;
	  if (!nbP) {
	      BUFFER_UNLOCK();
	      return (BUF_NOMEM); /* -> ENOMEM */
	  }

	  /* Note that it is crucial that we HOLD the buffer and add it to the
	   * addr hash before we sleep.  Otherwise we might end up with
	   * multiple buffers for the same block. */
	  BUFFER_HOLD(nbP);
	  SetType(nbP, bt);

	  RECYCLE(nbP, bpeP);		/* note that the buffer is recycled */

	  nbP->adev = dP;
	  nbP->blockNum = blkno;
	  DFSH_HASHIN_R(&addrHashTable, nbP);
	  bufferClearError (nbP);	/* presumed innocent until... */

	  if (initBufferContentsFromDisk) {
	      asevent_event_t *ae = asevent_Create(ASEVENT_NOPIN);
	      struct buf *bp = asevent_MakeBuf(
			B_READ, nbP->data, nbP->adev->blkSize,
			nbP->adev, nbP->blockNum, 0);
	      retval = asevent_QueueIO(nbP->adev, bp, ae);
	      if (retval) {
		  afsl_Assert (nbP->refCount == 1); /* no sleeping... */
		  BUFFER_RELEASE(nbP);
		  bufferSetDiscard(nbP); /* throw away */
		  DeallocBuffer(nbP);
		  BUFFER_UNLOCK();
		  asevent_Release(ae);
		  return (retval);
	      }
	      afsl_Assert(nbP->aevent == NULL);
	      nbP->aevent = ae;
	      ae->data = (opaque)nbP;
	      bufferSetReadp(nbP);
	      afsl_Assert(!dontWaitForRead);
#if 0
	      if (dontWaitForRead) { /* don't wait */
		  /*
		   * XXX We never clean up (i.e., release the event)
		   * in this case, and it appears that we will fall
		   * into an infinite loop if we ever wait for the buf.
		   * Fortunately, we never really do this.
		   */
		  return (BUFFER_OK);
	      }
#endif /* 0 */
	      retval = WaitForRead(nbP);
	      if (retval != 0) {
		  BUFFER_UNLOCK();
		  return (retval);
	      }
	  } else {
	      bpeP->bufferCreateBlocks++;
#ifndef KERNEL
	      if (afsdb_episode_logbuf &
		  (1<<(LOGBUF_DEBUG_RANDOMIZE_BUFFERS-1))) {
		  /* see episode/anode/test_anode.c for details */
#define BIG_PRIME 1257056893		/* 0x4AED2A7d */
#define NextRandom() (r = r*BIG_PRIME + BIG_PRIME)
		  static u_long r = 1;
		  u_long *b = (u_long *)(nbP->data);
		  long nWords = nbP->adev->blkSize / sizeof(u_long);
		  while (nWords-- > 0) *b++ = NextRandom();
	      }
#endif /* KERNEL */
	  }
      } else {	/* LookupBuffer found buffer */
	  afsl_Assert (!bufferIsDiscard(nbP));
	  bpeP = (struct bufferPoolEntry *)nbP->poolP;
	  afsl_Assert (bpeP);
	  if (bt != nbP->bufType) {
	      /* don't bump refCount */
	      BUFFER_UNLOCK();
	      return (BUF_BADBUF);
	  }
	  if (dontWaitForRead) {
	      BUFFER_UNLOCK();
	      return (BUFFER_OK);
	  }
	  REUSE(nbP, bpeP);
	  if (!bufferIsInuse(nbP)) {
	      /* on free list, but cached.  Remove from free list */
	      RemoveFromFreeList (nbP);
	  }
	  BUFFER_HOLD(nbP);

	  while (bufferIsReadp(nbP)) {	/* waiting for completed read */
	      logbuf_SleepPrint (("Read: wait for pending read (blk %d)",
				  nbP->blockNum));
	      if (!osi_IsKernelPanic()) {
		  bufferSetNotify(nbP);
		  BUFFER_SLEEP(&nbP->cond);
	      } else {	/* do it ourselves on panic */
		  retval = WaitForRead(nbP);
		  if (retval != 0) {
		      BUFFER_UNLOCK();
		      return (retval);
		  }
	      }

	      if (bufferIsError(nbP)) {
		  /* The real reader is finished but found errors.  The
		   * buffer needs to be punted, but we want to try the
		   * I/O again, both to get the correct error code and
		   * perhaps to recover from a transient problem.
		   */
		  BUFFER_RELEASE(nbP);
		  /* we pull the buffer from the address hash! */
		  bufferSetDiscard(nbP);
		  if (nbP->refCount == 0) {
		      DeallocBuffer(nbP);
		  }
		  goto loop; /* try the I/O again */
	      } /* if */
	  } /* while */
      } /* ... else ... */

      *bPP = nbP;
      afsl_Assert(!bufferIsError(nbP));
      afsl_Assert(bufferIsInuse(nbP));

      POOLSIZE(bpeP);
      DBPV(("cache info: %d %d %d %d %d %d %d %d",
		      nbP->adev->blkSize, /* field #3 */
		      bpeP->bufferAllocs, /* field #4 */
		      bpeP->bufferHits,   /* field #5 */
		      bpeP->bufferMisses, /* field #6 */
		      bpeP->bufferInUse,  /* field #7 */
		      DAVERAGE_GET(bpeP->reuseAge), /* field #8 */
		      DAVERAGE_GET(bpeP->recycleAge), /* field #9 */
		      DAVERAGE_GET(bpeP->poolSize) /* field *10 */
		      ));
      BUFFER_UNLOCK();
      return (BUFFER_OK);
}

/*
 * elbb_Read - allocate & read a block synchronously.
 */

/* EXPORT */
int32 elbb_Read(
  IN async_device_t *dP,
  IN daddr_t blkno,
  IN enum elbb_bufType bt,
  OUT struct buffer **bPP)
{
    return elbb_ReadGeneral(dP, blkno, BUFFER_RG_READDISK, bt, bPP);
}

/*
 * elbb_ReadAhead - allocate & schedule a read of a block
 */
/* EXPORT */
int32 elbb_ReadAhead(
     IN async_device_t *dP,
     IN daddr_t blkno,
     IN enum elbb_bufType bt)
{
    struct buffer *bP; /* we simply pass it as an empty parameter */

    return elbb_ReadGeneral(dP, blkno,
			      BUFFER_RG_READDISK | BUFFER_RG_DONTWAIT,
			      bt, &bP);

}

/*
 * elbb_CopyBlock - read contents of one block into buffer for another
 */
/* EXPORT */
int32 elbb_CopyBlock(
  IN struct buffer *bP,			/* new buffer */
  IN async_device_t *dP,		/* block device */
  IN daddr_t blkno)			/* virtual block on device */
{
      struct buffer *obP;		/* old buffer */
      asevent_event_t *ae;		/* asynchronous event */
      struct buf *bufp;
      int32 retval;

      BUFFER_LOCK();
loop:
      LookupBuffer (dP, blkno, obP);
      if (obP) {			/* old block is in memory */
	  afsl_Assert (!bufferIsDiscard(obP));
	  if (bufferIsReadp (obP)) {	/* waiting for completed read */
	      logbuf_SleepPrint (("Copy: wait for pending read (blk %d)",
				   blkno));
	      bufferSetNotify(obP);
	      BUFFER_SLEEP (&obP->cond);
	      goto loop;
	  }
	  afsl_Assert(dP->blkSize == obP->adev->blkSize);
	  bcopy (bufferData (obP), bufferData (bP), dP->blkSize);
	  BUFFER_UNLOCK();
	  return (BUFFER_OK);
      }
      /* old block is not in memory; read it in */
      ae = asevent_Create(ASEVENT_NOPIN);
      bufp = asevent_MakeBuf(B_READ, bP->data, dP->blkSize, dP, blkno, 0);
      retval = asevent_QueueIO(dP, bufp, ae);
      if (retval) {
	  BUFFER_UNLOCK();
	  asevent_Release(ae);
	  return (retval);
      }
      afsl_Assert(bP->aevent == NULL);
      bP->aevent = ae;
      ae->data = (opaque)bP;
      bufferSetReadp (bP);
      BUFFER_UNLOCK();
      retval = asevent_WaitEvent(ae);
      BUFFER_LOCK();
      asevent_Release(ae);
      bP->aevent = NULL;
      bufferClearReadp (bP);
      if (retval) {
	  CheckIOError(retval, bP);
      }
      if (bufferIsNotify(bP)) {
	  bufferClearNotify(bP);
	  BUFFER_UNLOCK();
	  epi_cond_broadcast(&bP->cond);
      }
      else
	  BUFFER_UNLOCK();
      return (retval);
}

/*
 * elbb_Release - release a buffer back to buffer cache.
 *
 * The wbmode specifies the writeback semantics:
 *
 * async - schedule write immediately (buffer unlikely to be reused)
 * syncLog - force log up to most recent transaction on this buffer
 * sync - force this buffer (& possibly log)
 * lazy - delay scheduling write
 *
 * syncLog and sync may require I/O; if so they do not return until the
 * I/O has completed.
 *
 * async may require I/O but returns after scheduling.
 *
 * lazy always returns without I/O being scheduled.
 */
/* EXPORT */
int32 elbb_Release(IN struct buffer *bP, IN enum elbb_WBMode wbmode)
{
      long retval = BUFFER_OK;

      if (bP == (struct buffer *)0)
	  return BUFFER_OK;

      afsl_Assert(IsValidWBMode(wbmode));

      BUFFER_LOCK();
      afsl_Assert(bP->refCount);

      /* last reference deleted */

      switch (wbmode) {
	case elbb_async:
	  retval= Write(bP);
	  afsl_MBZ(retval);
	  break;

	case elbb_lazy:
	  /*
	   * DB 3188: write user data more aggressively.  Meta data can rot in
	   * the buffer pool - the log manager will get it out eventually.
	   * User data, however, really doesn't have anyone to "force" it out.
	   */
	  if (!bufferIsLogged(bP)) {
	      retval = Write(bP);
	      afsl_MBZ(retval);
	  }
	  break;

	case elbb_adel:
	  /* Can't presently handle asyncronous deletion of buffers.  To do it
	   * right ReadGeneral would have to check and wait.  So we just wait
	   * here instead. */
	case elbb_sdel:
	  bufferSetDiscard(bP); /* throw away when done writing */
	  /* FALLTHROUGH */
	case elbb_sync:
	  if (bufferIsUpdate(bP)) {
	      WaitForUpdate(bP);
	  }
	  if (bufferIsDirty(bP) && !bufferIsWritep(bP)) {
	      retval = Write(bP);
	      afsl_MBZ (retval);
	  }
	  if (bufferIsWritep(bP)) {
	      WaitForWrite(bP, 1, NULL);
	  }
	  break;

	case elbb_syncLog:
	  elbl_ForceBuffer(bP);
	  break;

	default:
	  afsl_Assert (wbmode == elbb_async);
      }

      /* now we drop the ref count and deallocate if necessary.  Note we
       * have to wait 'til now for the sdel and sync case as they call
       * WaitForWrite, which drops the buffer lock.
       */
      BUFFER_RELEASE(bP);
      if (bP->refCount == 0) {
	  DeallocBuffer(bP);
      }

      BUFFER_UNLOCK();
      return(retval);
}

/*
 * SetKilled -- mark a buffers contents as invalid.
 */
static void SetKilled (IN struct buffer *bP)
{
    /* regression: we assert that the buffer does not have pending I/O */
    afsl_Assert(!bufferIsWritep(bP));

    DBPV(("clearing dirty bit for block %d", bP->blockNum));
    bufferClearDirty(bP);
    SetFHLocked(bP, 0);			/* no worry about fsync, right? */
    RemoveAgeQ (bP);
    afsl_Assert (!bufferIsDirty(bP) && !bP->currentAgeQ &&
		 elbt_IsNullTranId((bP)->uzTran));

    RemoveFromAddrHash(bP);
    bufferClearDiscard(bP);		/* we've already done this */

    return;
}

/*
 * elbb_Kill -- removes the specified disk block from the buffer pool.
 *
 * PARAMETERS -- metadataOK is true if it is safe to kill dirty metadata
 *     buffers.  This will only be true if the transaction that freed the
 *     metadata block has committed, which implies that all transactions that
 *     modified the block have also committed.
 *
 * RETURN CODES -- returns zero if all went well, and CANTDELETE if metadataOK
 *     was false and the buffer was LOGGED and DIRTY. */

/* EXPORT */
int32 elbb_Kill(
  IN async_device_t *dP,
  IN daddr_t blkno,
  IN int metadataOK)
{

    struct buffer *bP;
    int32 retval;

    BUFFER_LOCK();

retry:
    /* lookup the buffer */
    LookupBuffer(dP, blkno, bP);

    if (!bP) {
	/* not in-core implies "killed" */
	retval = 0;
    } else {
	if (bufferIsUpdate(bP)) {
	    WaitForUpdate(bP);
	    goto retry;
	}
	if (bufferIsWritep(bP)) {
	    WaitForWrite(bP, 1, NULL);
	}

	if (!metadataOK && bufferIsLogged(bP) && bufferIsDirty(bP)) {
	    retval = BUF_CANTDELETE;
	} else {
	    SetKilled (bP);
	    retval = 0;
	}
    }

    BUFFER_UNLOCK();
    return retval;
}

/*
 * elbb_ModifySource - modify the data contents of a buffer.
 *
 * this routine changes the contents of a buffer and logs that change
 * if the tranId is not the invalid transaction id and the logRequest
 * flag is true.
 *
 * this routine returns the buffer handle in the nbP pointer - caller
 * cannot assume the buffer handle will not change across this call.
 *
 */

/* EXPORT */
int32 elbb_ModifySource(
     IN struct buffer *bP,
     IN u_int32 offset,
     IN const caddr_t newData,
     IN u_int32 newDataLen,
     IN osi_uio_seg_t newDataSource,	/* address space of newData ptr */
     IN elbt_TranRec_t tranId,
     IN int logRequest,
     OUT struct buffer **nbP)
{
      u_int32 retval, logFullState;

      afsl_Assert(bufferIsWrite(bP) && (bP->refCount > 0));

      BUFFER_LOCK();

#if 0
      /* If we're request logging we better have a valid tranId and a logged
       * buffer to work with. */
      afsl_Assert (!logRequest ||
		   (!elbt_IsNullTranId(tranId) && bufferIsLogged(bP)));

      /* Require that updates to logged buffers are either always logged or
       * never logged.  We use the fact that logging a modification to a buffer
       * always puts the buffer on the ageQ.  So, for each update, if the
       * buffer is on the ageQ then we better be requesting logging.
       * Similarly, if we have a dirty buffer (but not write pending) and we're
       * not on the ageQ then we better not be requesting logging. */
      afsl_Assert (!(bufferIsLogged(bP) &&
		     ((bP->ageQ.next && !logRequest) ||
		      (bufferIsDirty(bP) && !bufferIsWritep(bP) &&
		       !bP->ageQ.next && logRequest))));
#endif /* 0 */


      if (logRequest) {
	  LogWrite(tranId, LOGREC_DATA,  newDataLen, offset, bP, newData,
		   retval, logFullState);

	  afsl_MBZ(retval);
      } else {
	  while (bufferIsWritep(bP)) {
	      WaitForWrite(bP, 1, 0);
	  }
      }

      retval = 0;
      DBPV(("elbb_Modify: modifying block #%d", bP->blockNum));
      switch (newDataSource) {
	  case OSI_UIOSYS:
	      LOGCOPY(newData, bP->data+offset, newDataLen);
	      break;
	  case OSI_UIOUSER:
	      BUFFER_UNLOCK();
	      retval = osi_copyin_r(newData, bP->data + offset, newDataLen);
	      BUFFER_LOCK();
	      break;
	  default:
	      afsl_Assert (0);
      }
      if (retval) {
	  CheckIOError(retval, bP);
	  BUFFER_UNLOCK();
	  return (retval);
      }

      bufferSetDirty(bP);
      *nbP = bP;

      if (logRequest && logFullState)
	  HandleLogFull(bP->adev, 0);

      BUFFER_UNLOCK();
      return(BUFFER_OK);
}

/*
 * elbb_Fill - write a specified bit pattern into memory.
 *
 * This routine is used to implement a "flood" of a data area with a
 * specific bit pattern (1 char).  It optionally saves the old data
 * already present in the data structure.
 *
 */

/* EXPORT */
int32 elbb_Fill(
     IN struct buffer *bP,
     IN u_int32 offset,
     IN int pattern,
     IN u_int32 fillSize,
     IN unsigned saveOld,
     IN elbt_TranRec_t tranId,
     IN int logRequest,
     OUT struct buffer **nbP)
{
      u_int32 retval;
      u_int32 logFullState;

      afsl_Assert(bufferIsWrite(bP) && (bP->refCount > 0));

      BUFFER_LOCK();

#if 0
      /* If we're request logging we better have a valid tranId and a logged
       * buffer to work with. */
      afsl_Assert (!logRequest ||
	      (!elbt_IsNullTranId(tranId) && bufferIsLogged(bP)));

      /* Require that updates to logged buffers are either always logged or
       * never logged.  We use the fact that logging a modification to a buffer
       * always puts the buffer on the ageQ.  So, for each update, if the
       * buffer is on the ageQ then we better be requesting logging.
       * Similarly, if we have a dirty buffer (but not write pending) and we're
       * not on the ageQ then we better not be requesting logging. */
      afsl_Assert (!(bufferIsLogged(bP) &&
		((bP->ageQ.next && !logRequest) ||
		 (bufferIsDirty(bP) && !bufferIsWritep(bP) && !bP->ageQ.next &&
		  logRequest))));
#endif /* 0 */

      /*
       * A single call here can always be divided into chunks because of
       * the transactional nature of the sytem.  So long as the chunks are
       * all logged within the same transaction, we are fine.
       */

      if (saveOld && logRequest) {
	  /* we save the old data */
	  LogWrite(tranId, LOGREC_ODATA, fillSize, offset, bP, 0, retval,
		   logFullState);

	  afsl_MBZ(retval);
      }

      DBPV(("elbb_Fill: modifying block #%d", bP->blockNum));
      if (logRequest) {
	  /* we note what the bit pattern was */
	  unsigned char pat = pattern;	/* copy pattern to proper sized byte */
	  LogWrite(tranId, LOGREC_NFILL, fillSize, offset, bP,
		   (char *)&pat, retval, logFullState);
	  afsl_MBZ(retval);
      } else {
	  while (bufferIsWritep(bP)) {
	      WaitForWrite(bP, 1, 0);
	  }
      }

      /*
       * fill the data space as requested.  We assume the pattern is always
       * in our address space (so we don't need to call copyin).
       */
      LOGFILL(bP->data+offset, pattern, fillSize);

      bufferSetDirty(bP);
      *nbP = bP;

      if (logRequest && logFullState)
	  HandleLogFull(bP->adev, 0);

     BUFFER_UNLOCK();
     return(BUFFER_OK);

}

/*
 * elbb_Initialize - log the initialization of a buffer's data.
 *
 * By initialization, we mean modification in the case where the old data
 * do not have to be logged, i.e. the buffer was created by CreateBlock.
 */

/* EXPORT */
int32 elbb_Initialize(
     IN struct buffer *bP,
     IN u_int32 offset,
     IN u_int32 dataLen,
     IN elbt_TranRec_t tranId,
     IN int logRequest,
     OUT struct buffer **nbP)
{
      u_int32 retval, logFullState;

      afsl_Assert(bufferIsWrite(bP) && (bP->refCount > 0));

      BUFFER_LOCK();
#if 0
      /* If we're request logging we better have a valid tranId and a logged
       * buffer to work with. */
      afsl_Assert (!logRequest ||
	      (!elbt_IsNullTranId(tranId) && bufferIsLogged(bP)));


      /* Require that updates to logged buffers are either always logged or
       * never logged.  We use the fact that logging a modification to a buffer
       * always puts the buffer on the ageQ.  So, for each update, if the
       * buffer is on the ageQ then we better be requesting logging.
       * Similarly, if we have a dirty buffer (but not write pending) and we're
       * not on the ageQ then we better not be requesting logging. */
      afsl_Assert (!(bufferIsLogged(bP) &&
		((bP->ageQ.next && !logRequest) ||
		 (bufferIsDirty(bP) && !bufferIsWritep(bP) && !bP->ageQ.next &&
		  logRequest))));
#endif /* 0 */

      if (logRequest) {
	  LogWrite (tranId, LOGREC_NDATA, dataLen, offset, bP,
		    bufferData(bP) + offset, retval, logFullState);
	  afsl_MBZ (retval);
      }

      bufferSetDirty (bP);
      *nbP = bP;

      if (logRequest && logFullState)
	  HandleLogFull (bP->adev, 0);

      BUFFER_UNLOCK();
      return (BUFFER_OK);
}

/*
 * elbb_FastModify - perform a quick logged modification.
 *
 * This routine is equivalent to a ReadGeneral/ModifySource/Release operation
 *
 */
/* EXPORT */
int32 elbb_FastModify(
     IN async_device_t *dP,
     IN daddr_t blkno,
     IN u_int32 offset,
     IN caddr_t newData,
     IN u_int32 newDataLen,
     IN elbt_TranRec_t tranId)
{
      struct buffer *bP;
      int32 retval;
      elbt_TranRec_t tid;
      unsigned relBuf = 0;
      u_int32 logFullState;

      /*
       * if this routine was called without a transaction id we start
       * one; otherwise we use the one passed in.
       */
      tid = tranId;
      if (elbt_IsNullTranId(tid)) {
	  elbt_StartTran("fast_modify", 0, dP, &tid);

	  /* XXX this isn't correct.  At this level we must call elbb_StartTran
           * to handle log full conditions.  Luckily no one uses this fcn. */

	  afsl_Assert(!elbt_IsNullTranId(tid));
      }

      BUFFER_LOCK();
      /*
       * Now, we try to obtain the buffer quickly; if that fails we
       * pay the cost of calling ReadGeneral.
       */
      LookupBuffer(dP, blkno, bP);
      if (!bP) {
	  BUFFER_UNLOCK();
	  retval = elbb_ReadGeneral(dP, blkno, BUFFER_RG_READDISK,
				      elbb_bufLogged, &bP);
	  if (retval != BUFFER_OK)
	      return retval;
	  relBuf = 1;
	  BUFFER_LOCK();
      }
      afsl_Assert (!bufferIsDiscard(bP));
      /*
       * We now have the transaction and the buffer to modify; we
       * write the log record and modify the buffer.
       */
      LogWrite(tid, LOGREC_DATA, newDataLen, offset, bP, newData, retval,
	       logFullState);

      afsl_MBZ(retval);
      /*
       * Note that in ModifySource we check to determine if this comes
       * from the same address space.  Since user data is never logged, we
       * assume along this code path that we never have user data (e.g. we
       * don't have to use copyin().
       */
      LOGCOPY(newData, bP->data+offset, newDataLen);

      /* now the buffer is dirty */
      bufferSetDirty(bP);

      /*
       * Now clean-up.  The neat thing is that for in-core buffers we don't
       * even need to DO anything because we never removed them from the
       * free list, etc.
       */

      if (elbt_IsNullTranId(tranId)) {
	  /* we have to end the transaction */
	  elbt_EndTran(tid);
      }

      if (relBuf) {
	  /* we had to call ReadGeneral for this buffer, so we release it */
	  retval = elbb_Release(bP, elbb_lazy);
	  if (retval)
	      BUFFER_UNLOCK();
	      return(retval);
      }

      if (logFullState) {
	  HandleLogFull(dP, 0);
      }
      return BUFFER_OK;
}

/*
 * elbb_StartTran -- begin a transaction.
 *
 *     By providing an old transaction ID, this call will not block.  If there
 *     is no transaction ID, this call might wait pending completion of other
 *     transactions.
 *
 *     If this routine fails to allocate a transaction we presume it is because
 *     the log is full and so we invoke the log full handling and try again.
 */
/* EXPORT */
int32 elbb_StartTran(
     IN caddr_t serviceName,
     IN elbt_TranRec_t oldTranId,
     IN async_device_t *dP,
     OUT elbt_TranRec_t *newTranId)
{
    int nLoops = 0;
#define ELBB_MAX_PANIC_LOOP	3
    *newTranId = elbt_NullTranId;

    while (elbt_StartTran(serviceName, oldTranId, dP, newTranId),
	   elbt_IsNullTranId(*newTranId)) {
	int action;
	u_int16 pass, off;

	if (osi_IsKernelPanic() && ++nLoops == ELBB_MAX_PANIC_LOOP)
	    return BUF_NOMEM;

	/* Snap shot the current log position. */
	elbl_GetLogPageOff(dP, &pass, &off);

	BUFFER_LOCK();
	action = HandleLogFull(dP, 1);
	if (!action) {

	    /* HLF found nothing to do so it must be safe to wait for any kind
	     * of transaction, including VM undo-zeros that may be pinning the
	     * log.  After releasing the buffer lock we'll do this using
	     * Quiesce.  To make sure it won't run into any of "our" undo-zeros
	     * we signal callers of elbb_UndoZeroBuffer which don't currently
	     * hold the buffer lock to punt and start a new transaction. */

	    returnAnyUndoZeroTrans++;	/* signal undo-zero racer to punt */
	}
	BUFFER_UNLOCK();
	if (!action) {
	    elbt_Quiesce (dP, 2/*serialize undo-zeros*/, pass, off);
	}
    }

    return BUFFER_OK;
}

/*
 * elbb_LogInit - initialize logging for a particular device
 *
 */
/* EXPORT */
int32 elbb_LogInit(
     IN async_device_t *ldP,
     IN async_device_t *dP,
     IN daddr_t *logLocnArray,
     IN unsigned logLocnArraySize)
{
      unsigned bufsNeeded = LOGREC_MAX_BUFFERS;
      struct buffer *bP[LOGREC_MAX_BUFFERS];
      unsigned index;
      int32 retval;

      afsl_Assert(ldP);
      afsl_Assert(dP);
      afsl_Assert(logLocnArray);
      afsl_Assert(bufsNeeded <= LOGREC_MAX_BUFFERS);

      BUFFER_LOCK();

      for (index = 0; index < bufsNeeded; index++) {
	  bP[index] = AllocBufferHeader();
	  InitBufferHeader(bP[index]);
	  bP[index]->adev = ldP;
	  bP[index]->data = osi_Alloc_r(ldP->blkSize);
	  logbuf_pin(bP[index]->data, ldP->blkSize);
	  BUFFER_HOLD(bP[index]);
      }

      BUFFER_UNLOCK();

      retval = elbl_InitLog(ldP, dP, logLocnArray, logLocnArraySize, bP,
			    bufsNeeded, 0);

      if (retval != BUFFER_OK) {
	  /* log crashed */
	  BUFFER_LOCK();
	  for (index = 0; index < bufsNeeded; index++) {
	      BUFFER_RELEASE(bP[index]);
	      DeallocLogBuffer(bP[index], 0);
	  }
	  BUFFER_UNLOCK();

	  if (retval == RCVR_RECOVERED) {
	      /* log recovered, we invalidate the buffer pool by
	       * calling shutdown.  Note that we assume none of the
	       * buffers were dirty when elbb_LogInit was called.
	       */
	      elbb_Shutdown(dP);
	  }

	  return retval;
      }

      return (BUFFER_OK);
}

/*
 * elbb_LogCreate - initialize a new log.
 */
/*
 * ifndef to prevent compilation problems, between ANSI style function forward
 * declarations and K&R style function definitions, which erupted due to
 * the resultant type-checking for the new "unsigned char" parameter.
 */
/* EXPORT */
int32 elbb_LogCreate(
      IN async_device_t *ldP,
      IN async_device_t *dP,
      IN daddr_t *logLocnArray,
      IN unsigned logLocnArraySize,
      IN unsigned char cksumVer)
{
      int32 code;

      /* we create a new log */
      code = elbl_Create(ldP, logLocnArray, logLocnArraySize, cksumVer);
      afsl_MBZ(code);

      /* now do a normal initialization */
      return elbb_LogInit(ldP, dP, logLocnArray, logLocnArraySize);
}

/*
 * elbb_TranIsCommitted - check to see if a transaction is committed
 */
/* EXPORT */
int32 elbb_TranIsCommitted(IN elbt_TranRec_t bufTranId)
{
      int32 code;

      code = elbt_GetTranStatus(bufTranId);

      if ((code == BUF_BADTRAN) || (code == BUF_TRANCOMMIT))
	  return 1;
      else
	  return 0;
}

/*
 * elbb_SetFH - acquire buffer lock & then set/clear buffer magic #
 */
/* EXPORT */
int32 elbb_SetFH(struct buffer *bP, u_int32 fh)
{
      int32 code = 0;

      BUFFER_LOCK();
      code = SetFHLocked(bP, fh);
      BUFFER_UNLOCK();
      return code;
}

/*
 * SetFHLocked - set/clear buffer magic # - caller must hold buffer lock.
 */
static int32 SetFHLocked(struct buffer *bP, u_int32 fh)
{
      struct buffer *nbP;

      BUFFER_ASSERTLOCKED();

      if (bP->bufFH == fh)
	  return BUFFER_OK; /* no work to do */

      /*
       * if bP->bufFH is set, it should already be on a
       * chain here, so we remove it first.
       */
      if (bP->bufFH) {
	  /*
	   * we find the top of this chain.
	   */
	  DFSH_LOOKUP(struct buffer *, &FH_HashTable, nbP, bP->bufFH,
		      (bP->bufFH == nbP->bufFH));
	  /* there must be a chain */
	  afsl_Assert(nbP);
	  if (bP == nbP) {
	      if (bP->fhList.length > 0) {
		  /*
		   * this is the head of a non-empty list; we must remove
		   * it from the hash table and replace it with the new
		   * entry.
		   *
		   * we take the *newest* member guessing that by doing
		   * this we will eliminate many of the trips through
		   * this particular path. - wam 082391
		   */
		  nbP = (struct buffer *) FIFO_LookAtNewest(
			&bP->fhList, offsetof(struct buffer, fhList.thread));
		  afsl_Assert(nbP != bP);

		  nbP->fhList.length = bP->fhList.length;
		  nbP->fhList.threadOffset = bP->fhList.threadOffset;
		  FIFO_REMOVE(&nbP->fhList, &bP->fhList.thread);
		  dfsh_HashOut_r(&FH_HashTable, bP);
		  DFSH_HASHIN_R(&FH_HashTable, nbP);
#ifdef AFS_DEBUG
		  {   struct buffer *xbP;
		      DFSH_LOOKUP(struct buffer *, &FH_HashTable, xbP,
				  nbP->bufFH, (xbP->bufFH == nbP->bufFH));
		      afsl_Assert(xbP == nbP);
		  }
#endif
		  /* we zero all the fields */
		  bP->fhList.threadOffset = 0;
		  bP->fhList.length = 0;
	      } else {
		  /*
		   * bP->fhList.threadOffset == 0
		   *
		   * this is the case when this is the last element in
		   * the hash table; remove it from the hash table and
		   * zero all the fields in it.
		   */

		  dfsh_HashOut_r(&FH_HashTable, bP);
		  FIFO_INIT(&bP->fhList,
			    offsetof(struct buffer, fhList.thread));
		  bP->fhList.threadOffset = 0;
	      }
	  } else {
	      /*
	       * this is only a thread in the greater list; remove it from
	       * the chain.
	       */
	      FIFO_REMOVE(&nbP->fhList, &bP->fhList.thread);
	  }
      }

      if ((bP->bufFH = fh) == 0) {
	  /* done */
	  return BUFFER_OK;
      }

      /*
       * now we add it to the new hash chain
       */

      DFSH_LOOKUP(struct buffer *, &FH_HashTable, nbP, fh, (fh == nbP->bufFH));

      afsl_Assert(nbP != bP);

      if (nbP) {
	  /*
	   * we just add it to the existing thread
	   */
	  FIFO_ADD(&nbP->fhList, &bP->fhList.thread);
      } else {
	  /* we create a new thread */
	  FIFO_INIT(&bP->fhList, offsetof(struct buffer, fhList.thread));
	  /* and hash it in */
	  DFSH_HASHIN_R(&FH_HashTable, bP);
#ifdef AFS_DEBUG
	  {   struct buffer *xbP;
	      DFSH_LOOKUP(struct buffer *, &FH_HashTable, xbP, bP->bufFH,
			  (xbP->bufFH == bP->bufFH));
	      afsl_Assert(xbP == bP);
	  }
#endif
      }
      return BUFFER_OK;
}

/*
 * elbb_FsyncForce -- implements fsync using two basic methods of specifying
 *     the buffers to write out.  Either a magic value is used to tag each
 *     buffer, or an array of blocks is provided.
 */
/* EXPORT */
int32 elbb_FsyncForce(
  IN async_device_t *dP,
  IN u_int32 magic,
  IN daddr_t *blockArray,
  IN u_int32 blockArraySize)
{
    struct buffer *fbP;			/* head of fifo for specifed magic */
    u_int counter;			/* for iterating block array */
    struct buffer *bP;			/* loop variable */

    u_int count;
    asevent_set_t *aset;
    long code;

    BUFFER_LOCK();
    afsl_Assert(dP);

    aset = 0;				/* empty set */
    count = 0;
    bP = 0;				/* beginning of iteration */
    counter = 0;

    for (;;) {

	/* Do the loop control here, so we can easily start over and so we can
	 * shared the somewhat tricky buffer event management. */

	/* There are two cases of the input paramters.  If magic is specified
	 * we need to enumerate the buffer on the FH hash table then follow the
	 * fhList and write all buffer on the specified device.  Otherwise we
	 * are given a list of blocks.  In this case we lookup the
	 * corresponding buffers. */

	if (magic) {

	    /* In the magic case there are three stages: if bP is null look the
	     * first buffer up in the hash table, and save it in fbP.  If bP
	     * equals fbP then continue with the first buffer on the first
	     * buffers fhList.  Otherwise just look at the next buffer on that
	     * list.  We exit if this returns null.  If we have to drop the
	     * buffer lock, we just start over, by setting bP to null. */

	    if (bP == 0) {
		DFSH_LOOKUP(struct buffer *, &FH_HashTable, fbP, magic,
			    (fbP->bufFH == magic));
		bP = fbP;
	    } else if (bP == fbP) {
		bP = (struct buffer *)
		    FIFO_LookAtOldest(&fbP->fhList,
			offsetof(struct buffer, fhList.thread));
	    } else {
		bP = (struct buffer *)
		    FIFO_NextOldest(&fbP->fhList, bP, &bP->fhList.thread);
	    }
	    if (!bP)
		break;			/* we're done */
	    if (bP->adev != dP)
		continue;		/* not on specified device */
	} else {
	    daddr_t blkno;

	    /* In the block list case we use the counter variable to enumerate
	     * the input array of block addresses. */

	    if (counter >= blockArraySize)
		break;			/* end of array */
	    blkno = *(blockArray+counter);
	    counter++;
	    if (blkno == BUF_INVALID_BLOCK)
		continue;		/* skip this array element */
	    LookupBuffer(dP, blkno, bP);
	    if (!bP)
		continue;		/* not cached in buffer pool */
	}

	/* Now for each buffer, schedule the write and add the buffer to the
	 * event set. */

	if (!bufferIsDirty(bP))
	    continue;

	if (!bufferIsUpdate(bP) && !bufferIsWritep(bP)) {
	    code = Write(bP);
	    afsl_MBZ(code);
	}
	code = 0;
	if (!bufferIsUpdate(bP))
	    code = AddBufferToSet (bP, &aset);
	if (bufferIsUpdate(bP) || (code == ASEVENT_BUSY)) {
	    /*
	     * This happens when another process is using the event
	     * structure for this buffer.  Blow this set away, since
	     * we'll have to start over.  Then wait for this one here and
	     * start the loop over.
	     */
	    DeleteBufferSet (&aset);
	    if (bufferIsUpdate(bP))
		WaitForUpdate(bP);
	    else
		WaitForWrite(bP, 1, NULL);
	    count = 0;
	    bP = 0;			/* restart loop */
	    counter = 0;		/* ditto */
	    continue;
	}
	count++;
    }

    if (count) {

	/* Now, without holding the *global* BUFFER lock, wait for all
	 * these buffers. */

	BUFFER_UNLOCK();
	afsl_Assert (aset);
	code = asevent_WaitAll(aset);
	BUFFER_LOCK();

	DeleteBufferSet (&aset);
    }

    BUFFER_UNLOCK();
    return (BUFFER_OK);
}

/*
 * elbb_SyncForce - schedule write for all dirty buffers within the
 * buffer pool.  Realization of UNIX sync operation.
 */
/* EXPORT */
void elbb_SyncForce(void)
{
      struct buffer *bP;

      /*
       * Don't try sync-on-panic if buffer pool or log is locked.
       */
      if (osi_IsKernelPanic() &&
	  (BUFFER_HAS_OWNER() || elbl_LogIsLocked()))
	return;

      BUFFER_LOCK();
      /*
       * For sync, we do not want to wait.  We walk through the entire hash
       * pool, skipping buffers being updated (they're busy), and scheduling
       * dirty buffers.  Write does not sleep so we can do this whole operation
       * fast.
       */
      for (bP = dfsh_HashNext(&addrHashTable, NULL);
	   bP;
	   bP = dfsh_HashNext(&addrHashTable, bP)) {
	  if (!bufferIsUpdate(bP) && bufferIsDirty(bP) && !bufferIsWritep(bP))
	      Write(bP);
      }

      /* Now, walk back through the list and end any undo-zero transactions
       * which are now finished.  We don't wait for pending I/O, we just
       * clean up anything that is already finished.
       */
      for (bP = dfsh_HashNext(&addrHashTable, NULL);
	   bP;
	   bP = dfsh_HashNext(&addrHashTable, bP)) {
	  if (bufferIsWritep(bP)) {
	      WaitForWrite(bP, 0, NULL);
	  }
      }
      BUFFER_UNLOCK();
}

/*
 * elbb_Reserve - reservation call for code paths which require multiple
 *		buffers simultaneously; this avoids deadlock.
 *
 * locks: none required.  Acquires buffer lock.
 *
 */
/* EXPORT */
void elbb_Reserve(IN int nBuffers, IN unsigned bufferSize)
{
      struct bufferPoolEntry *bpeP;
      int code;
      int have, bpb, need, giveback, done;
      static long nCalls;


      if (nBuffers == 0) return; /* nothing to do */

      BUFFER_LOCK();
      if (nBuffers > 0) nCalls++;
      if (nBuffers < 0) nCalls--;
      bpeP = GetBufferPool(bufferSize);
      bpb = BUF_BLOCKSIZE >> (10+bpeP->logBlkSize); /* # bufs per block */

      /*
       * We are willing to sleep here waiting for the reservation; the
       * thinking is that the buffer space balancing code will do its
       * job of resource sharing, so we shouldn't disturb things unless
       * really necessary.
       *
       * If the reservation fails, it means we don't have enough buffers
       * to ever satisfy the request.  We try reserving enough additional
       * space to satisfy the caller.  We do NOT allocate the space - we
       * only reserve it!
       *
       * Note that we cannot call holding the buffer lock - otherwise
       * other threads cannot do anything!
       *
       * Although we unlock, it is safe to do so at this point because
       * the reservation package will serialize parallel requests.
       */

      code = epis_ReserveWL(&bpeP->reserved, nBuffers, 1 /* sleep */, 0,
			    &bufferLock);

      while (code == 0) {
	  /* the allocation failed, so we try to reserve more */

	  have = epis_QueryTotalUnits(&bpeP->reserved);
	  need = (nBuffers - have) / bpb;
	  /* we carefully round up if necessary */
	  need += ((need * bpb) == (nBuffers - have)) ? 0 : 1;
	  if (need < 0) need = 0; /* we never "need" less than zero */

	  /*
	   * We must worry about what happens if multiple threads are
	   * trying to allocate for this buffer pool.  We serialize them
	   * externally - only one thread proceeds while the others
	   * sleep waiting for the global reservation to complete.  When
	   * that is done, we wakeup the other threads, they then get another
	   * chance to try the allocation and go around the loop again!
	   *
	   * We cannot wait for a reservation while holding one without
	   * risking deadlock.  This is not acceptable.  What it really
	   * means is that we must *free* our existing reservation before
	   * we can increase our global reservation.  Ugh!
	   */
	  if (bpeP->res_allocs == 0) {
	      bpeP->res_allocs++;; /* semaphore */

	      if (need + bpeP->global_res >
		  epis_QueryTotalUnits(&globalBlockRes)) {
		  /*
		   * We cannot ask for more than exist in the system.  We
		   * must adjust our request
		   */
		  need = epis_QueryTotalUnits(&globalBlockRes) -
		      bpeP->global_res;

		  /* hey - if 100% won't cut it, we'll just panic below! */
	      }

	      /*
	       * before we can reserve, we must release our current
	       * reservation, which means we must adjust our local
	       * reservation pool.
	       */
	      done = epis_QueryTotalUnits(&bpeP->reserved);
	      while (done) {
		  afsl_Assert(bpeP->res_allocs == 1);
		  afsl_Assert(done <= epis_QueryTotalUnits(&bpeP->reserved));
		  done += epis_AdjustReservationUnitsWL(&bpeP->reserved,
							-(done), &bufferLock);
		  afsl_Assert(bpeP->res_allocs == 1);
	      }

	      /* now we free the global reservation - if any */
	      if (bpeP->global_res) {
		  code = epis_ReserveWL(&globalBlockRes, -(bpeP->global_res),
					0, 0, &bufferLock);
		  afsl_Assert(code); /* should never fail */
	      }
	      /* our need was based upon our current holdings. */
	      need += bpeP->global_res;
	      bpeP->global_res = 0;

	      /* and now we get what we wanted in the first place! */

	      code = epis_ReserveWL(&globalBlockRes, need, 1 /* sleep */, 0,
				    &bufferLock);
	      /*
	       * We should try to get more space even harder here, but
	       * for now we panic.
	       */
	      afsl_PAssert(code, ("Insufficient space for buffer reservation"));
	      bpeP->global_res += need;

	      /*
	       * Now adjust the total # of buffers we have to reflect this
	       * newfound largess.  Since this thread allocated from the global
	       * pool, it loops until it has adjusted the local reservation to
	       * have the correct reservation.
	       */
	      done = need * bpb;
	      while (done) {
		  afsl_Assert(bpeP->res_allocs == 1);
		  done -= epis_AdjustReservationUnitsWL(&bpeP->reserved, done,
							&bufferLock);
		  afsl_Assert(done >= 0);
		  afsl_Assert(bpeP->res_allocs == 1);
	      }

	      if (bpeP->res_wtrs) {
		  epi_cond_broadcast (&bpeP->cond);
	      }
	      bpeP->res_allocs--;
	  } else {
	      bpeP->res_wtrs++;
	      BUFFER_SLEEP(&bpeP->cond);
	      bpeP->res_wtrs--;
	      need = 0;
	  }

	  /*
	   * Now try the reservation again.  Note that it could still fail -
	   * maybe another thread snuck in while we were sleeping and changed
	   * things on us!
	   */

	  code = epis_ReserveWL(&bpeP->reserved, nBuffers, 1 /* sleep */,
				0, &bufferLock);
      }

      /*
       * If this was a "free" call, lets see if we can return some of the
       * global reservation.
       *
       * As we did above, we check to see if some other thread is already
       * in the global reservation path.  If so, we skip this.
       */
      if ((nBuffers < 0) && !bpeP->res_allocs) {
	  bpeP->res_allocs++; /* semaphore */
	  /*
	   * We compute how many of these buffers we really need.  Those
	   * we don't, we return.
	   */

	  /* This call returns approximately the number we can withdraw without
	   * sleeping. */
	  have = epis_QueryUnusedUnitsWL(&bpeP->reserved);
	  giveback = have / bpb;

	  if (giveback) {
	      /*
	       * We try to adjust the buffer pool size.  If this call completes
	       * partially, or not at all, we check that our rounding was
	       * correct and then we re-adjust the global pool to reflect the
	       * buffers we managed to retrieve from the pool.
	       */
	      afsl_Assert(bpeP->res_allocs == 1);
	      done = epis_AdjustReservationUnitsWL(&bpeP->reserved,
						   -(giveback * bpb),
						   &bufferLock);
	      afsl_Assert(bpeP->res_allocs == 1);
	      giveback = done / bpb;
	      done -= giveback * bpb;
	      /*
	       * We must put back any non-quantized reservation - it is
	       * useless to us.  Convert done to a positive number.
	       */
	      done = -done;
	      while (done) {
		  afsl_Assert(bpeP->res_allocs == 1);
		  done -= epis_AdjustReservationUnitsWL(&bpeP->reserved,
							done, &bufferLock);
		  afsl_Assert(bpeP->res_allocs == 1);
	      }
	      if (giveback) {

		  /* Return this amount to the global pool and reduce our idea
		   * of the amount we have. */

		  afsl_Assert(giveback < 0);
		  code = epis_ReserveWL(&globalBlockRes, giveback, 1, 0,
					&bufferLock);
		  afsl_Assert(code);
		  bpeP->global_res += giveback;
	      }
	  }
	  bpeP->res_allocs--;
	  if (bpeP->res_wtrs)
	      epi_cond_broadcast (&bpeP->cond);
      }
      afsl_Assert (bpeP->res_allocs ||	/* some other thread is adjusting */
		   (bpeP->global_res*bpb ==
		    epis_QueryTotalUnits(&bpeP->reserved)));

      BUFFER_UNLOCK();
}

static const char undoZero = 0;
/*
 * elbb_UndoZero - Adds an undo-zero record to an existing transaction for
 * the specified range of bytes.  This is to be used by the VM system for I/Os
 * that do not use the buffer system.  It is expected that this would not be
 * used for fragments (which currently use the buffer system) but there seems
 * no reason to preclude this.  Typical usage may span several contiguous
 * blocks which are allocated and written at the same time.
 *
 * locking - no locks are required.  This call may acquire the buffer lock.
 *
 */
/* EXPORT */
void elbb_UndoZero(
  IN async_device_t *dP,
  IN daddr_t blkno,
  IN u_int32 offset,
  IN u_int32 fillSize,
  IN elbt_TranRec_t tranId)
{
      unsigned bytesToWrite = fillSize;
      u_long logFullState = 0;
      long code;

      afsl_Assert(!elbt_IsNullTranId(tranId)); /* should not be null tran */

      BUFFER_LOCK();

      while (fillSize) {
	  bytesToWrite = dP->blkSize - offset;
	  bytesToWrite = MIN(bytesToWrite, fillSize);
	  LogWriteX(tranId, LOGREC_OFILL, bytesToWrite, dP, blkno, offset,
		    (const caddr_t)&undoZero, code, logFullState);
	  afsl_MBZ(code);
	  fillSize -= bytesToWrite;
	  if (bytesToWrite+offset == dP->blkSize) offset = 0;
      }

      if (logFullState)
	  HandleLogFull(dP, 0);

      BUFFER_UNLOCK();
}

/*
 * elbb_UndoZeroBuffer -- Adds an undo-zero record for the specified buffer.  A
 *     transaction is started and attached to the buffer if one is not already
 *     present.  All updates to the dirty buffer have been done when this call
 *     is made.
 */
/* EXPORT */
void elbb_UndoZeroBuffer(
  IN struct buffer *bP,
  IN u_int32 offset,
  IN u_int32 fillSize,
  IN elbt_TranRec_t oldTran,
  OUT struct buffer **nbP)
{
      static char *serviceName = "elbb_UndoZeroBuffer";
      elbt_TranRec_t newTranId = elbt_NullTranId;
      long retval;
      u_long logFullState;

      /* must fit within the current buffer */
      afsl_Assert(fillSize + offset <= bP->adev->blkSize);

      BUFFER_LOCK();

      while (elbt_IsNullTranId(bP->uzTran)) {
	  int racers;
	  newTranId = elbt_NullTranId;

	  if (!bufferIsDirty(bP)) {
	      afsl_Assert (elbt_IsNullTranId(bP->uzTran));
	      BUFFER_UNLOCK();
	      return;
	  }

	  /* To detect race conditions between starting the transaction and
	   * re-obtaining the buffer lock, snapshot this global variable which
	   * is incremented if elbb_StartTran needs to sleep for undo-zero
	   * transaction started by the VM system. */
	  racers = returnAnyUndoZeroTrans;

	  BUFFER_UNLOCK();
	  Reschedule();
	  elbb_StartTran(serviceName, oldTran, bP->adev, &newTranId);
	  BUFFER_LOCK();

	  /*
	   * It is quite possible that while we were futzing around getting a
	   * transaction, someone else already did for this buffer.  Or the
	   * buffer may have been cleaned, in which case we have nothing
	   * further to do.  Or we lost the race with StartTran (see which).
	   * We check - if any of these happened, we end our transaction and
	   * try again.  If not, stick the transaction ID into the buffer.
	   */
	  if ((racers == returnAnyUndoZeroTrans) &&
	      elbt_IsNullTranId(bP->uzTran) && bufferIsDirty(bP)) {
	      bP->uzTran = newTranId;
	      break;
	  } else {
	      BUFFER_UNLOCK();
	      Reschedule();
	      elbb_EndTran(newTranId);
	      BUFFER_LOCK();
	  }
      }

      afsl_Assert (bufferIsDirty(bP) && !elbt_IsNullTranId(bP->uzTran));

      LogWriteX(bP->uzTran, LOGREC_OFILL, fillSize, bP->adev, bP->blockNum,
		offset, (const caddr_t)&undoZero, retval, logFullState);
      if (retval == BUF_OLDTRAN) {
	  /* Buffer was cleaned and undoZero tran ended by someone else (i.e.
	   * HandleLogFull) and record was NOT written.  This is OK and our job
	   * is done. */
      } else {
	  afsl_MBZ (retval);
      }

      *nbP = bP;

      if (logFullState)
	  HandleLogFull(bP->adev, 0);

      BUFFER_UNLOCK();
}

/*
 * DeallocLogBuffer
 *
 * this routine handle deallocating log buffers when shutting down the
 * log.
 */
static void DeallocLogBuffer(IN struct buffer *bP, IN int flags)
{
      /* note flags is not used */
      logbuf_unpin(bP->data, bP->adev->blkSize);
      osi_Free_r(bP->data, bP->adev->blkSize);
      bP->data = (caddr_t)0;
      bP->adev = 0;
      bP->bufFlags = 0;

      bP->bufFH = 0;
      bP->refCount = 0;
      FreeBufferHeader(bP);
}

/*
 * elbb_Shutdown -- is called when closing down operations on a device (logged
 *     or unlogged.) this routine writes out all pending buffers and removes
 *     all buffers for this device from use.  It uses QuiesceSystem to do this
 *     work safely.  Then it shuts down the corresponding log and removes all
 *     the unused blocks from this buffer pool. */

/* EXPORT */
int32 elbb_Shutdown(IN async_device_t *dP)
{
      struct buffer *bP;
      struct bufferPoolEntry *bpeP = GetBufferPool (dP->blkSize);
      struct bufferBlock *bbP, *nbbP;

      /* Clean buffer system at full power */
      elbb_QuiesceSystem (dP, ELBB_QUIESCE_COMMIT|ELBB_QUIESCE_FLUSHALL);

      BUFFER_LOCK();

      /*
       * Close the log; this waits until all pending transactions are complete.
       * New transactions will not be permitted to start.
       *
       * Since we've cleaned up the meta-data state we are "safe" because we
       * can't be pinning the tail of the log with a buffer.
       */

      elbt_Shutdown(dP, DeallocLogBuffer);

      /*
       * We attempt to return any and all buffer blocks we can to the global
       * pool.  We compute the cost of reclaiming a block and do it
       * iff the cost is zero.
       *
       * N.B. the strong presumption that other routines clean up buffers
       * correctly.
       */

      for (bbP = FIFO_LookAtOldest(&bpeP->blockList,
				   offsetof(struct bufferBlock, poolThread));
	   bbP; bbP = nbbP) {
	  /*
	   * we need to figure out the next buffer first, before possibly
	   * deleting the current one from the pool.
	   */
	  nbbP = FIFO_NextOldest(&bpeP->blockList, bbP, &bbP->poolThread);
	  CostToFreeBlock(bbP);
	  if (bbP->reclaimCost == 0) {
	      /*
	       * Remove all the blocks from the free list and then free
	       * the block.
	       */
	      FIFO_FOR (bP, &bbP->buffers, blockThread) {

		  /* Since we've written and committed all updates and then
		   * killed all buffers for this device, we afsl_Assert that
		   * there are no active references for this buffer.  Buffer
		   * for other devices need to be removed from the address hash
		   * and free list. */

		  afsl_Assert (bP->adev != dP);

		  RemoveFromFreeList(bP);
		  RemoveFromAddrHash(bP);
	      }
	      FreeBufferSpace(bbP);
	      DBP(("Shutdown: freeing block from %dK pool",
		   1<<bpeP->logBlkSize));
	  }
      }
      BUFFER_UNLOCK();
      DBP(("Shutdown: device %d/%d with %dK block size",
	   dP->majdev, dP->mindev, 1<<bpeP->logBlkSize));

      return BUFFER_OK;
}

/*******************************************************************
 * Code local to this module only
 *******************************************************************/

static struct fifo bufferFreeHeaders;

/*
 * InitBufferAllocator - initialize the buffer allocator system.
 */
static int32 InitBufferAllocator(void)
{
      static initialized;

      if (initialized)
	  return BUFFER_OK;

      FIFO_INIT(&bufferFreeHeaders, offsetof(struct buffer, freeList));

      return BUFFER_OK;
}

/*
 * Allocate a buffer header by removing from free list.
 * If no more available, allocate a new block of memory and
 * continue.
 */
static struct buffer *AllocBufferHeader(void)
{
      struct buffer *bP;

      BUFFER_ASSERTLOCKED();
      /*
       * if the free list is empty, we allocate one and then
       * stick it on the free list.
       */
      if (IsFifoEmpty(&bufferFreeHeaders)) {
	  bP = osi_Alloc_r(sizeof(struct buffer));
	  logbuf_pin(bP, sizeof(struct buffer));
	  bzero((caddr_t)bP, sizeof(*bP));
	  epi_cond_init (&bP->cond, "buffer");
	  FreeBufferHeader(bP);
      }

      /* pull a buffer header off the free list */
      bP = FIFO_LookAtOldest(&bufferFreeHeaders,
			     offsetof(struct buffer, freeList));

      if (bP)
	  FIFO_REMOVE(&bufferFreeHeaders, &bP->freeList);

      return bP;
}

 /*
  * This routine initializes and places the specified buffer header
  * on the free list.  Note it does not free any memory associated
  * with the header.
  */
static void FreeBufferHeader(IN struct buffer *bP)
{
      SetFHLocked(bP, 0);
      InitBufferHeader(bP);
      bP->data = (caddr_t)0;
      FIFO_ADD (&bufferFreeHeaders, &bP->freeList);
 }

/*
 * InitBufferHeader - provide default initialization of a buffer header.
 *
 * Caller may need to do other initialization as well.
 * Does not set data field.
 */
static void InitBufferHeader(struct buffer *bP)
{
      ReinitBufferHeader(bP);
      afsl_Assert (bufferIsInuse(bP));	/* new buffers are not on free list. */
      FIFO_INITTHREAD(&bP->freeList);
      FIFO_INITTHREAD(&bP->ageQ);
      lock_Init(&bP->bLock);
      bP->poolP = (opaque)0;
      FIFO_INITTHREAD(&bP->blockThread);
      /* always clean out the fifo structure */
      FIFO_INIT(&bP->fhList, offsetof(struct buffer, fhList.thread));
      /* always set the undo zero tran to the null tran id */
      bP->uzTran = elbt_NullTranId;
}

/*
 * ReinitBufferHeader -- takes an already used buffer header and clears fields
 *     which should be cleared.
 *
 */
static void ReinitBufferHeader(struct buffer *bP)
{
      bP->bufFlags = BUFFER_NEW;
      bufferSetInuse(bP);		/* new buffers are not on free list. */
      bP->bufType = elbb_bufUnused;
      bP->refCount = (u_int32)0;
      bP->currentAgeQ = (struct fifo *)0;
      bP->adev = NULL;
      bP->blockNum = (daddr_t)0;
      bP->addrHashNext = (struct buffer *)0;
      bP->bufLogFirstPass = (u_int16)0;
      bP->bufLogFirstOff = (u_int16)0;
      bP->bufLogLastPass = (u_int16)0;
      bP->bufLogLastOff = (u_int16)0;
      bP->bufFH = 0;
      bP->bufFHNext = (struct buffer *)0;
      bP->aevent = NULL;
}

/*
 * GetBufferPool - given a block size, locate the buffer pool entry for
 * that size.
 *
 */
static struct bufferPoolEntry *GetBufferPool(IN u_int32 bufSize)
{
    struct bufferPoolEntry *bpeP;

    /* find the free queue associated with buffer */
    switch(bufSize) {
      case BUF_MINBLKSIZE:
	bpeP = &bufferPool[0];
	break;
      case 2 * BUF_MINBLKSIZE:
	bpeP = &bufferPool[1];
	break;
      case 4 * BUF_MINBLKSIZE:
	bpeP = &bufferPool[2];
	break;
      case 8 * BUF_MINBLKSIZE:
	bpeP = &bufferPool[3];
	break;
      case 16 * BUF_MINBLKSIZE:
	bpeP = &bufferPool[4];
	break;
      case 32 * BUF_MINBLKSIZE:
	bpeP = &bufferPool[5];
	break;
      case 64 * BUF_MINBLKSIZE:
	bpeP = &bufferPool[6];
	break;
      default:
	afsl_MBZ(1);
	break;
    }
    return bpeP;
}

/* MoreSpaceDesire -- returns a value which indicates the likelihood that a
 *     pool would request more space.  Zero means needs no space, 100 means
 *     will request more space, and values above 100 indicate increasing
 *     desires for additional space.  This is used to balance the needs of
 *     different pools against each other.
 *
 *     The calculated value decays towards zero if the pool hasn't been touched
 *     lately.  The amount to decay inactive pools is determined on a global
 *     clock (i.e. one that ticks based on calls to all pools).  In
 *     GetMoreBufferSpace the number of ticks since the last calls to
 *     MoreSpaceDesire is divided by the parameter InactiveDecayInterval to
 *     determine the amount of decay.  This amount is expressed as the number
 *     of times to reduce the desire by 1/8 (12.5%).  Fractional parts are
 *     saved until the next call.
 *
 *     TODO -- Currently there is no penalty for using the pool infrequently as
 *	 long are there are references within each InactiveDecayInterval.
 *
 *     The value returned is a combination of the following calculations each
 *     of which is scaled to 100 meaning get more space:
 *	 (1) MinHitRatio * misses/(hits+misses)
 *	 (2) MinPoolLength * PoolRatio/length
 *	 (3) PoolRatio * ReuseAge/RecycleAge
 *     Rationale for (1) -- if there aren't a significant number of both hits
 *	 and misses then the reuse and recycle ages are not being updated
 *	 frequently.  And, of course, if the hit rate is abysmal then clearly
 *	 we should try to get more space.  The expected value for the
 *	 MinHitRatio parameter is around 2-5 (e.g. 3 => 33% access miss).
 *     Rationale for (2) -- since the value of recycle should hover around the
 *	 length of the free list, and the smallest value that reuse can ever
 *	 have is 1 then the goal of achieving PoolRatio is impossible until
 *	 the pool contains at least that many buffers.  Clearly we would like
 *	 a few multiples of this for the length.  The typical value for the
 *	 MinPoolLength parameter is 2-3.
 *     Rationale for (3) -- ideally we will be reusing buffers from near the
 *	 front of the free list.  When a buffers reuse age reaches the
 *	 recycle age it is in imminent danger of being recycled before being
 *	 reused.  Thus the average age of reuse must be much smaller than the
 *	 average recycle age.  This seems like a better measure of the buffer
 *	 cache's performance than its absolute hit rate.  A typical value for
 *	 the PoolRatio parameter is 5-20.
 */

static long MoreSpaceDesire(
  IN struct bufferPoolEntry *pool,
  IN int decay)				/* how much to decay inactive pools */
{
    long currentHitsMisses = pool->bufferHits+pool->bufferMisses;
    long mhr;				/* miss ratio need */
    long mpl;				/* pool length need */
    long pr;				/* pool ratio need */

    if (currentHitsMisses == pool->lastHitsMisses) {
	/* nothing happened since last check so decay by 12.5% and return */
	while (decay-- > 0) {
	    long shrink = (pool->moreSpaceDesire>>3);
	    if ((shrink == 0) && (pool->moreSpaceDesire > 0)) shrink = 1;
	    pool->moreSpaceDesire -= shrink;
	}
	return pool->moreSpaceDesire;
    }
    pool->lastHitsMisses = currentHitsMisses;

    /* note: missRatio is already expressed as a percent (i.e. 0-100) */
    mhr = (MinHitRatio * pool->missRatio) >> DecayRate1;
    if (pool->freeList.length == 0) mpl = 100*100;
    else mpl = 100 * MinPoolLength * PoolRatio / pool->freeList.length;
    pool->moreSpaceDesire = mhr + mpl;
    if (pool->moreSpaceDesire > 2*100)
	/* if this number is bad the following ratio won't be well defined */
	return pool->moreSpaceDesire;

    pr = 100 * PoolRatio * pool->reuseAge / pool->recycleAge;
    pool->moreSpaceDesire += pr;
    return pool->moreSpaceDesire;
}

/* GrabBufferSpace -- pull a block off the free list and place on active block
 *     list of the specified pool.
 */

static void GrabBufferSpace(IN struct bufferPoolEntry *bpeP)
{
    struct bufferBlock *bbP;
    struct buffer *bP;			/* first new buffer being added */
    struct buffer *nbP;			/* subsequent new buffers */
    u_long bufSize;			/* bytes per buffer */
    u_int bufCount;			/* buffers per block */
    u_int index;
    unsigned freed;			/* whenFreed value for new bufs */

    bbP = FIFO_LookAtOldest(&freeBlockList,
			    offsetof(struct bufferBlock, blockListThread));
    afsl_Assert(bbP);
    FIFO_REMOVE(&freeBlockList, &bbP->blockListThread);
    FIFO_ADD(&activeBlockList, &bbP->blockListThread);

    FIFO_ADD(&bpeP->blockList, &bbP->poolThread);
    bbP->poolP = bpeP;

    /* get the (only) buffer on the free list. */

    bP = FIFO_LookAtOldest(&bbP->buffers,
			   offsetof(struct buffer, blockThread));
    afsl_Assert (bP);
    FIFO_REMOVE(&bbP->buffers, &bP->blockThread);

    /* divide the data area up into contiguous blocks.  This places the buffers
     * on the free list for the buffer pool. */

    bufSize = bpe_BlockSize(bpeP);
    bufCount = BUF_BLOCKSIZE / bufSize;

    /* Use age of oldest buffer for these new ones and add them to the oldest
     * end so that they are allocated first. */

    nbP = FIFO_LookAtOldest(&bpeP->freeList,
			    offsetof(struct buffer, freeList));
    freed = (nbP ? nbP->whenFreed : 0);

    /* Special case first buffer when index==0. */

    for (index = 0; index < bufCount; index++) {
	if (index > 0)
	    nbP = AllocBufferHeader();
	else
	    nbP = bP;
	/*
	 * Initialize the new buffer
	 */
	SetFHLocked(nbP, 0);
	InitBufferHeader(nbP);
	nbP->poolP = (opaque)bpeP;

	nbP->whenFreed = freed;
	FIFO_ADD_OLDEST(&bpeP->freeList, &nbP->freeList);

	bufferClearInuse(nbP);
	nbP->data = bP->data + (index * bufSize);
	FIFO_ADD(&bbP->buffers, &nbP->blockThread);
	bpeP->bufferTotal++;
    }
    epi_cond_broadcast (&bpeP->cond);
}

/* FreeBufferSpace -- return space to the general pool.  This routine is used
 *     to return an entire 128k block of memory to the global pool.  It must be
 *     called holding the global buffer lock.
 */

static void FreeBufferSpace(struct bufferBlock *bbP)
{
      struct buffer *bP, *nbP;
      struct bufferPoolEntry *bpeP;

      BUFFER_ASSERTLOCKED();
      /*
       * given a buffer block, we figure out which pool is using it. We
       * then remove all individual buffers from the pool and free the
       * buffer headers (except for the first one.)
       *
       * We know the oldest buffer has a pointer to the beginning of the
       * 128k block.  We preserve it. Extract the pool information.
       */

      bP = (struct buffer *)FIFO_LookAtOldest(&bbP->buffers,
			offsetof(struct buffer, blockThread));
      if (bP != NULL)
	  FIFO_REMOVE(&bbP->buffers, &bP->blockThread);
      bpeP = (struct bufferPoolEntry *)bbP->poolP;
      afsl_Assert (bP->poolP == (opaque)bpeP);

      /*
       * we now pull all the remaining elements off the fifo and
       * deallocate them.
       */
      while (nbP = (struct buffer *)FIFO_LookAtOldest(&bbP->buffers,
			offsetof(struct buffer, blockThread))) {
	  FIFO_REMOVE(&bbP->buffers, &nbP->blockThread);

	  /* A few paranoid checks --
	   *	 refCount is zero
	   *	 not on free list
	   *	 not dirty
	   *     not on age queue
	   *     not in address hash table
	   *	 all blocks are in the same pool */

	  afsl_Assert((nbP->refCount == 0) &&
		      (bufferIsInuse(nbP) && (nbP->freeList.next == 0)) &&
		      !bufferIsDirty(nbP) &&
		      (nbP->currentAgeQ == NULL) &&
		      (nbP->adev == NULL) &&
		      (nbP->poolP == (opaque)bpeP));

	  /*
	   * ... and free it.
	   */
	  FreeBufferHeader(nbP);

	  /*
	   * we reflect the fact that there are now fewer buffers.
	   */
	  bpeP->bufferTotal--;
      }

      /*
       * finally, we reinsert the original buffer header into the
       * buffer fifo.
       */
      SetFHLocked(bP, 0);
      FIFO_ADD(&bbP->buffers, &bP->blockThread);

      afsl_Assert((bP->refCount == 0) &&
		  (bufferIsInuse(bP) && (bP->freeList.next == 0)) &&
		  !bufferIsDirty(bP) &&
		  (bP->currentAgeQ == NULL) &&
		  (bP->adev == NULL) &&
		  (bP->poolP == (opaque)bpeP));

      /*
       * and readjust the buffer pointer to no longer point to the
       * buffer pool.
       */
      bP->poolP = (opaque)0;
      bpeP->bufferTotal--;

      /*
       * and the pool block list
       */
      FIFO_REMOVE(&bpeP->blockList, &bbP->poolThread);
      bbP->poolP = 0;

      /*
       * and from the active block list.
       */
      FIFO_REMOVE(&activeBlockList, &bbP->blockListThread);

      /*
       * and place it back on the free block list.
       */
      FIFO_ADD(&freeBlockList, &bbP->blockListThread);

      /* Mark the pool for immediate reevalution wrt more space desire. */
      bpeP->nextMore = 0;
}

/* Reclaim -- do what is necessary to clear out all users of this block of
 *     buffers and return it to the free block list if successful.  Otherwise
 *     just return.  If wait is set then caller's need is urgent so wait as
 *     appropriate.  If wait is false then do what can be done quickly and give
 *     up.  We can assume CostToFreeBlock has been run recently.
 *
 * RETURN CODES -- To wait for a held buffer we must release locks and sleep on
 *     the pool's freeList, the retry.  We can't do that but the caller will if
 *     we return the buffer he should wait on.
 */

static struct buffer *Reclaim(
  IN struct bufferBlock *bbP,
  IN int wait)
{
    struct buffer *bP;
    int mustSleep;
    struct buffer *held;		/* any buffer with non-zero refCount */

    mustSleep = 0;
    held = 0;

    if (bbP->reclaimCost > 0) {
	if (!wait && (bbP->reclaimCost > RECLAIM_DIRTY*10)) {
	    /* The effort is too great and caller isn't desparate so punt.  */
	    mustSleep++;
	} else {

	    /* There is apparently at least some non-trivial work to do and
	     * either we are forced of the work isn't too great.  Start by
	     * writing all the dirty buffers. */

	    FIFO_FOR (bP, &bbP->buffers, blockThread) {
		if (bufferIsUpdate(bP))
		    mustSleep++;
		else if (bufferIsDirty(bP) && !bufferIsWritep(bP)) {
		    Write(bP);		/* schedule the I/O */
		    mustSleep++;
		} else if (bufferIsWritep(bP)) {
		    WaitForWrite (bP, 0, NULL);
		    if (bufferIsWritep(bP))
			mustSleep++;
		}
		if (bP->refCount) {
		    mustSleep++;
		}
	    }
	}
    }

    if (!wait && mustSleep)
	return NULL;

    /* Now wait for any dirty buffers or I/O.  When we're done with this loop
     * we'll have a clean block but some buffers may be held. */

    while (mustSleep) {
	mustSleep = 0;
	held = 0;
	FIFO_FOR (bP, &bbP->buffers, blockThread) {
	    /*
	     * We must loop after waiting for because WaitForUpdate and
	     * WaitForWrite drop the buffer lock.  Thus it is possible for some
	     * other thread hold and/or dirty this buffer.
	     */
	    if (bufferIsUpdate(bP)) {
		WaitForUpdate(bP);
		mustSleep = 1;		/* go around again */
		break;
	    }
	    if (bufferIsDirty(bP) && !bufferIsWritep(bP))
		/* now, if this buffer is dirty, get it written out. */
		Write(bP);
	    if (bufferIsWritep(bP)) {
		WaitForWrite(bP, 1, NULL);
		mustSleep = 1;		/* go around again */
		break;
	    }
	    afsl_Assert (!bufferIsDirty(bP));
	    if (bP->refCount)
		held = bP;
	}
    }

    /* If there was a held buffer we have our caller mark it for reclaim and
     * start over. */

    if (held)
	return held;

    /* Else we're ready to reclaim the buffers in the block */

    FIFO_FOR (bP, &bbP->buffers, blockThread) {
	afsl_Assert ((bP->refCount == 0) && !bufferIsDirty(bP));
	RemoveFromFreeList(bP);
	RemoveFromAddrHash(bP);
	/* and pull off any magic # chains */
	SetFHLocked(bP, 0);
    }

    FreeBufferSpace (bbP);
    return NULL;
}

/* GetMoreBufferSpace -- allocate more space to a given buffer pool.
 *
 * RETURN CODE -- If zero then caller should try again soon, because he has
 *     merit but needs to wait for I/O to finish (or something).  If positive
 *     then some additional space was provided.  Otherwise, negative means that
 *     caller is not sufficiently deserving of more space.
 */

static int GetMoreBufferSpace(IN struct bufferPoolEntry *bpeP)
{
      struct bufferBlock *bbP;
      struct buffer *bP;
      unsigned bufCount;		/* buffers currently in this pool */
      int wait;				/* set if caller is desperate */
      int decay;			/* number of decays to do */
      long lowest, highest;		/* min & max desire of all pools */
      struct bufferPoolEntry *lowestPool;/* pool ptr of lowest */
      struct bufferBlock *lowestBbp;	/* cheapest block in lowest pool */
      int i;
      int got;				/* indicate success of "Get" */
      int startOverRetries = 0;		/* count of reclaim retries */

      static u_long lastBufferClock;	/* time we were last called */
      static u_long remainingDecay;	/* time since last decay interval */

      afsl_Assert(bufferPoolInit);
      BUFFER_ASSERTLOCKED();

      /* If caller isn't desperate and lock is already held, there is little
       * point in waiting since someone is already working here.  Just return
       * immediately. */

      wait = IsFifoEmpty (&bpeP->freeList);
      if ((lock_Check (&morespaceLock)) && !wait) {
	  DBP(("MoreSpace: punting because lock held"));
	  return 0;
      }

      /*
       * we only allow a single process to allocate more buffer space
       * at a time because of internal race conditions.  But we must do
       * this acquire OUTSIDE of a normal buffer lock!  Otherwise it will
       * deadlock.  Ugh!
       *
       * Note further that we squirrel away the # of buffers of this size
       * before - and if it isn't the same after, we quit out of this
       * routine.
       */
      bufCount = bpeP->bufferTotal;

start_over_after_reclaim:

      BUFFER_UNLOCK();
      lock_ObtainWrite_r(&morespaceLock);
      BUFFER_LOCK();

      wait = IsFifoEmpty (&bpeP->freeList);
      if (!wait && (bufCount != bpeP->bufferTotal)) {
	  lock_ReleaseWrite_r(&morespaceLock);
	  DBP(("MoreSpace: punting because bufCount(%d) != bufferTotal(%d)",
	       bufCount, bpeP->bufferTotal));
	  return 1;
      }

      /* There are free blocks so grab one immediately. */

      if (wait && !IsFifoEmpty(&freeBlockList)) {
	  GrabBufferSpace (bpeP);
	  lock_ReleaseWrite_r(&morespaceLock);
	  DBP(("MoreSpace: desperately grabbing free space for %dK pool",
	       1<<bpeP->logBlkSize));
	  return 1;
      }

      /* First consider global picture for all pools. */

      /* Decide how much to decay inactive pools by using global clock and
       * correctly handling round off error. */
      remainingDecay += bufferClock - lastBufferClock;
      decay = remainingDecay >> logInactiveDecayInterval;
      remainingDecay -= decay << logInactiveDecayInterval;
      lastBufferClock = bufferClock;

      /* Then evaluate MoreSpaceDesire for all pools and get min and max. */
      lowest = 1000*100;		/* least desperate */
      highest = 0;			/* most desperate */
      lowestPool = 0;
      for (i=0; i<sizeof(bufferPool)/sizeof(bufferPool[0]); i++) {
	  struct bufferPoolEntry *pool = &bufferPool[i];
	  long more;

	  more = MoreSpaceDesire (pool, decay);

	  if (pool == bpeP) continue;	/* don't count us in lowest/highest */

	  if ((pool->blockList.length > 1) && (more < lowest)) {
	      /* this might be a good candidate */
	      lowest = more;
	      lowestPool = &bufferPool[i];
	  }
	  if ((pool->blockList.length > 0) && (more > highest)) {
	      highest = more;
	  }
      }

      got = -1;				/* assume couldn't get one */
      lowestBbp = 0;			/* best block to reclaim */

      if (wait) {

	  /* We are desperate so grab the lowest block in any pool. */

	  /* Note we scale lowest by 100 so we don't have to divide desire by
	   * 100 everytime. */
	  long cost;
	  long lowest = RECLAIM_MAX * 100;

	  for (bbP = (struct bufferBlock *)FIFO_LookAtOldest(&activeBlockList,
			offsetof(struct bufferBlock, blockListThread));
	       bbP;
	       bbP = (struct bufferBlock *)FIFO_NextOldest(&activeBlockList,
				bbP, &bbP->blockListThread)) {
	      struct bufferPoolEntry *pool = bbP->poolP;
	      if (pool == bpeP)		/* skip this pool */
		  continue;
	      if (pool->blockList.length == 1) /* leave at least one */
		  continue;
	      CostToFreeBlock(bbP);
	      cost = bbP->reclaimCost * pool->moreSpaceDesire;
	      if (!lowestBbp || (cost < lowest)) {
		  lowest = cost;
		  lowestBbp = bbP;
	      }
	  }
	  if (!lowestBbp)
	      /* deep yogurt */
	      DBP(("MoreSpace: can't find reclaimable block"));

      } else if (bpeP->moreSpaceDesire >= highest) {

	  /* We are most deserving so proceed. */

	  if (!IsFifoEmpty(&freeBlockList)) {

	      /* If there is free space available, just grab some and run. */

	      DBP(("MoreSpace: grabbing free block for %dK pool (%d)",
		   1<<bpeP->logBlkSize, bpeP->moreSpaceDesire));
	      GrabBufferSpace (bpeP);
	      got = 1;

	  } else if (lowestPool && (bpeP->moreSpaceDesire > lowest*2)) {

	      /* We are not in trouble here so just try to improve balance
	       * different pools.  If highest and lowest are out of balance by
	       * more than a factor of two and if caller wants more space in
	       * the highest then get more space for him at the expense of
	       * lowest.  Do this by finding easiest block from this pool for
	       * reclamation. */

	      for (bbP = (struct bufferBlock *)FIFO_LookAtOldest(
		&lowestPool->blockList,
		offsetof(struct bufferBlock, poolThread));
		   bbP;
		   bbP = (struct bufferBlock *)FIFO_NextOldest(&lowestPool->blockList, bbP, &bbP->poolThread)) {
		  CostToFreeBlock(bbP);
		  if (!lowestBbp ||
		      (bbP->reclaimCost < lowestBbp->reclaimCost))
		      lowestBbp = bbP;
	      }
	  }
      }

      if (lowestBbp) {
	  /* do the reclaim if a good block was found */
	  DBP(("MoreSpace: block (%d) for %dK pool (%d) from %dK pool (%d)",
	       lowestBbp->reclaimCost, 1<<bpeP->logBlkSize,
	       bpeP->moreSpaceDesire, 1<<lowestBbp->poolP->logBlkSize,
	       lowestBbp->poolP->moreSpaceDesire));
	  bP = Reclaim (lowestBbp, wait);

	  /* if reclaim was successful then use the block */

	  if (!IsFifoEmpty(&freeBlockList)) {
	      GrabBufferSpace (bpeP);
	      got = 1;			/* got more space */
	  } else
	      got = 0;			/* would have liked to but await I/O */

      } else bP = 0;

      lock_ReleaseWrite_r(&morespaceLock);

      if (bP) {

	  /* Reclaim has decided we need to wait for a held buffer.  We can not
	   * do this with either the bufferLock (obviously) or the
	   * morespaceLock held.  So, since we are desperate, we bump a
	   * counter, to detect starvation, set the buffer's reclaim bit (which
	   * means notify when refCount == 0), release these locks, and go
	   * around again.  This restarts the whole process from scratch. */

	  if (startOverRetries && (startOverRetries % 10) == 0) {
	      printf("GetMoreBufferSpace: %d unsuccessful attempts to get %dK buffer space\n",
		     startOverRetries, 1 << bpeP->logBlkSize);
	  }
	  startOverRetries++;
	  DBP(("MoreSpace: %d buffer reclaims", startOverRetries));

	  bufferSetReclaim(bP);
	  logbuf_SleepPrint (("GetMoreBufferSpace: await reclaim of %#x", bP));
	  if (!osi_IsKernelPanic()) {
	      bufferSetNotify(bP);
	      BUFFER_SLEEP(&bP->cond);
	      goto start_over_after_reclaim;
	  } else {
	      BUFFER_UNLOCK();
	      return 0;
	  }
      }

      return got;
}

/* NeedMoreBufferSpace -- controls calls to GetMoreBufferSpace since it is
 *     quite expensive.  Basically it sets a parameter for the pool which is
 *     decremented towards zero and causes a call to this routine when it
 *     reaches zero.  We call GetMoreBufferSpace and upon return adjust the
 *     parameter appropriately for the next call.
 *
 *     There are several considerations we use to decide how soon we should be
 *     called again.
 *	 soon -- Basically if we obtained a buffer last time or if we would
 *	     have obtained a buffer except we have to wait for I/O then we
 *	     want to check again soon.
 *	 later -- This hold if we were not the most deserving of a buffer.
 *	     For this we use MoreSpaceInterval plus or minus a factor of two
 *	     depending on whether moreSpaceDesire is large or not.
 *	 never -- If we already have all the buffers there are.
 */

static void NeedMoreBufferSpace(IN struct bufferPoolEntry *pool)
{
    int result;

    result = GetMoreBufferSpace (pool);
    if (pool->blockList.length == activeBlockList.length+freeBlockList.length)
	/* we have them all so don't check again */
	pool->nextMore = 0x7fffffff; /* largest positive signed integer */
    else if (result > 0) {
	/* successful so try again soon in case there more to be had */
	pool->nextMore = MoreSpaceInterval/8;

    } else if (result < 0) {
	/* unsuccessful, so try much later if we're pretty happy, or soon if
	 * we're unhappy, otherwise just check again in normal interval. */
	if (pool->moreSpaceDesire < 50)
	    pool->nextMore = MoreSpaceInterval*2;
	else if (pool->moreSpaceDesire < 100)
	    pool->nextMore = MoreSpaceInterval;
	else
	    pool->nextMore = MoreSpaceInterval/2;

    } else /* (result == 0) */ {
	/* wait a jiffy for pending I/O or something */
	pool->nextMore = MoreSpaceInterval/16;
    }
    DBP(("NeedMore: %dK: len=%d reuse=%d recyc=%d miss=%d desire=%d nxt=%d",
	 1<<pool->logBlkSize, pool->blockList.length,
	 DAVERAGE_GET(pool->reuseAge), DAVERAGE_GET(pool->recycleAge),
	 DAVERAGE_GET(pool->missRatio), pool->moreSpaceDesire,
	 (pool->nextMore > 999999) ? 99
	 : (pool->nextMore*16/MoreSpaceInterval))); /* in 16ths */
}

#ifndef KERNEL
static char *MetaInfo(struct buffer *bP)
{
    static char info[32];
    char *cp = info;

    info[sizeof(info)-1] = 0;
    if (bufferIsInuse(bP))
	*cp++ = 'H';
    if (bufferIsDirty(bP)) {
	if (bufferIsWritep(bP))
	    *cp++ = 'W', *cp++ = 'p';
	else
	    *cp++ = 'D';
	if (bufferIsUpdate(bP))
	    *cp++ = 'U';
	*cp++ = '-';
	if (bP->currentAgeQ) {
	    int cost = elbl_BufferCommitCost(bP);
	    u_int passno = bP->bufLogLastPass;
	    u_int offset = bP->bufLogLastOff;
	    cp += sprintf (cp, "%d@%u/%u", cost, passno, offset);
	} else {
	    cp += sprintf (cp, "unaged");
	}
    } else {
	if (cp != info)
	    *cp++ = '-';
	cp += sprintf (cp, "clean");
    }
    afsl_Assert(info[sizeof(info)-1] == 0);
    return info;
}

static int enableShowList = 0;

static void ShowBufList(
  struct buffer *abP,
  struct bufferPoolEntry *bpeP)
{
    struct bufferBlock *bbP;
    struct buffer *bP;

    if (!bpeP)
	bpeP = (struct bufferPoolEntry *)abP->poolP;
    if (abP)
	printf ("Forcing log to write %#x (%s)",
		abP, MetaInfo(abP));
    else
	printf ("Forcing log");
    printf (" bufferList in %dK pool\n", 1<<bpeP->logBlkSize);
    for (bbP = FIFO_LookAtOldest(&bpeP->blockList,
				 offsetof(struct bufferBlock, poolThread));
	 bbP;
	 bbP = FIFO_NextOldest(&bpeP->blockList, bbP, &bbP->poolThread)) {
	FIFO_FOR (bP, &bbP->buffers, blockThread) {
	    printf ("  bP=%#x (%s)\n", bP, MetaInfo(bP));
	}
    }
}

static char *FreeBpInfo(struct buffer *bP)
{
    static char info[80];
    char *cp = info;
    struct bufferPoolEntry *bpeP = bP->poolP;
    unsigned clock = bpeP->bufferHits + bpeP->bufferMisses;

    if (bP->adev == NULL)
	cp += sprintf (cp, "blkno=NULL");
    else
	cp += sprintf (cp, "blkno=%d/%d", bP->adev->mindev, bP->blockNum);
    cp + sprintf (cp, " (%s) age=%d", MetaInfo(bP), clock - bP->whenFreed);
    return info;
}

static void ShowFreeList(
  struct buffer *abP,
  struct bufferPoolEntry *bpeP)
{
    struct buffer *bP;

    if (!bpeP) bpeP = abP->poolP;
    if (abP)
	printf ("Allocating %s\n", FreeBpInfo(abP));
    printf (" freeList in %dK pool; reuse=%d, recycle=%d\n",
	    1<<bpeP->logBlkSize,
	    DAVERAGE_GET(bpeP->reuseAge), DAVERAGE_GET(bpeP->recycleAge));

    FIFO_FOR (bP, &bpeP->freeList, freeList) {
	printf ("  %s\n", FreeBpInfo(bP));
    }
}
#endif

/*
 * AllocBuffer -- returns an unused buffer for use by ReadGeneral.  It will
 *     recycle a buffer from the free list if possible.  If the free list is
 *     empty it will wait for a held buffer to be released.  It also checks the
 *     balance of memory between pools an may obtain more buffer space.
 *
 *     This routine implements a policy of keeping a reserve of clean buffers
 *     by initiating writes of certain buffers.
 *
 * RETURN CODES -- a boolean indicating whether we blocked is returned.  If we
 *     did, the caller is expected to look at the address hash to see if it
 *     still needs a new buffer.  (Another process may have created the buffer
 *     we need while we were sleeping.)
 *
 * The caller of this routine will have to do additional initialization:
 * set the device field;
 * set the buffer type;
 */

static int32 AllocBuffer(
  IN struct bufferPoolEntry *bpeP,
  OUT struct buffer **bPP)
{
      u_int bufSize = bpe_BlockSize(bpeP);
      struct buffer *bP;
      unsigned cleanAge;		/* age at which to start cleaning */
      unsigned clock;			/* bpeP clock position */
      struct buffer *best;		/* best buffer to grab so far */
      int cost;				/* relative cost of best buffer */

      BUFFER_ASSERTLOCKED();

      *bPP = NULL;
      bpeP->bufferAllocs++;

      /* clean buffers as they reach 1/2 of recycleAge */
      cleanAge = DAVERAGE_GET(bpeP->recycleAge) >> 1;
      clock = bpeP->bufferHits + bpeP->bufferMisses;

      /*
       * Check to see if we should allocate more memory to this buffer pool,
       * but only if there are some blocks not allocated to THIS pool.  If
       * this pool already has them all we MUST fall through to the lower
       * case where we sleep.
       *
       * XXX: this code is probably still too aggressive.  Even if the free
       * list is empty, we shouldn't try stealing from other buffer pools for
       * fear of thrashing. -wam 920728
       */

      bpeP->nextMore--;
      if ((bpeP->nextMore <= 0) || IsFifoEmpty(&bpeP->freeList)) {
	  NeedMoreBufferSpace (bpeP);

	  if (IsFifoEmpty(&bpeP->freeList) && osi_IsKernelPanic())
	     return (0);

	  /*
	   * If that didn't do any good (or to us it doesn't look like it
	   * did any good) we will just sleep, waiting for someone to
	   * free up some resource.
	   */
	  while (IsFifoEmpty(&bpeP->freeList)) {
	      logbuf_SleepPrint (("AllocBuffer: wait free blocks of size %d",
				  bufSize));
	      BUFFER_SLEEP(&bpeP->cond);
	  }

	  /*
	   * No matter what, we have dropped the buffer lock
	   * (GetMoreBufferSpace always does) so we return to
	   * the caller - try it again.
	   */
	  return 1;
      }
      afsl_Assert(!IsFifoEmpty(&bpeP->freeList));

      /* Here we implement a policy regarding when to clean dirty buffers.
       * There are several criteria used to select a buffer to use, in each
       * case we use the oldest matching the least costly criterion.  In order
       * of decreasing desirability, the criteria are:
       *     (1) buffer is not dirty
       *     (2) buffer has already been scheduled
       *     (3) buffer is dirty and can be written without forcing the log
       *     (4) any remaining buffer
       *
       * We would like to pick up some concurrency in writing dirty buffers,
       * but we don't want to schedule a page for writing if it is going to be
       * reused very soon.  So schedule those whose age is half way to the
       * average recycle age.
       */

      FIFO_FOR (bP, &bpeP->freeList, freeList) {
	  if ((clock - bP->whenFreed) < cleanAge)
	      break;
	  if (!bufferIsUpdate(bP) && bufferIsDirty (bP) &&
	      !bufferIsWritep(bP) && (elbl_BufferCommitCost(bP) == 0)) {
	      int code = Write(bP);
	      afsl_MBZ(code);
	      bpeP->bufferAllocWrites++;
	  }
      }
      best = NULL;
      FIFO_FOR (bP, &bpeP->freeList, freeList) {
	  if (bufferIsWritep(bP)) {
	      /* Clean buffer if we can do this cheaply */
	      WaitForWrite (bP, 0/*!wait*/, NULL);
	  }
	  if (!bufferIsDirty(bP)) {
	      best = bP;
	      cost = 0;
	      break;
	  }
	  icl_Trace1 (elb_iclSet, TR_BUF_ALLOC_NONE_CLEAN,
		      ICL_TYPE_LONG, bufSize);

	  if ((!best || (cost > 1)) && bufferIsWritep(bP)) {
	      best = bP;
	      cost = 1;
	  } else if (!best || (cost > 2)) {
	      /* Dirty buffers cost something (10) for waiting for updates or
               * at least 2 plus some log forcing cost */
	      int thisCost = (bufferIsUpdate(bP)
			      ? 10
			      : 2+elbl_BufferCommitCost(bP));
	      if (!best || (cost > thisCost)) {
		  best = bP;
		  cost = thisCost;
	      }
	  }
      }
      afsl_Assert (best);		/* we know free list isn't empty */
      bpeP->allocCost = DAVERAGE_ADD (bpeP->allocCost, cost*100);
#ifndef KERNEL
      if (enableShowList) {
	  ShowFreeList(best, bpeP);
      }
#endif

      bP = best;
      if (bufferIsUpdate(bP)) {
	  WaitForUpdate(bP);
	  return 1;			/* slept */
      }
      if (bufferIsDirty(bP) && !bufferIsWritep(bP)) {
	  int code = Write(bP);
	  afsl_MBZ(code);
	  bpeP->bufferAllocWrites++;
      }
      if (bufferIsWritep(bP)) {
	  int slept = WaitForWrite (bP, 1/*wait*/, NULL);
	  if (slept)
	      return slept;
      }

      afsl_Assert(bP->blockThread.next); /* must be on block list */
      /*
       * we have successfully found a buffer; we clear out any "important"
       * fields and then call InitBufferHeader to set this up as a pristine
       * buffer.
       */
      DBPV(("removing block %d from address hash", bP->blockNum));
      RemoveFromAddrHash(bP);
      SetFHLocked(bP, 0);
      RemoveFromFreeList(bP);
      ReinitBufferHeader(bP);

      *bPP = bP;
      return 0;
}

/*
 * DeallocBuffer -- place a buffer back on free list after its refCount is
 *     decremented to zero.
 *
 *     If buffer's Discard bit is set, remove the block from the address hash
 *     (e.g.  the buffer really is "invalid".
 *
 *     Perhaps we should insert at the *front* of the free list if removing,
 *     since we aren't caching this buffer? - wam 3/7/91 XXX
 */
static void DeallocBuffer(struct buffer *bP)
{
      struct bufferPoolEntry *bpeP;

      bpeP = (struct bufferPoolEntry *)bP->poolP;
      afsl_Assert(bpeP);
      afsl_Assert(bP->blockThread.next);

      /* mark buffer as inactive */
      AddToFreeList(bP);
      bP->whenFreed = bpeP->bufferHits + bpeP->bufferMisses;

      /* remove from address hash if we no longer want it there! */
      if (bufferIsDiscard(bP)) {
	  afsl_Assert (!bufferIsDirty(bP) && (bP->adev != NULL));
	  RemoveFromAddrHash(bP);
	  bufferClearDiscard(bP);
      }

      if (bpeP->freeList.length == 1) {
	  /* wakeup any processes waiting for buffers */
	  epi_cond_broadcast (&bpeP->cond);
      }
}

/*
 * SetType
 *
 * Sets the type of a buffer, and does any flag setting/clearing
 * associated with that type.
 */

static void SetType(struct buffer *bP, enum elbb_bufType bt)
{
      bP->bufType = bt;
      switch (bt) {
	  case elbb_bufData:
	      bufferSetWrite(bP);
	      bufferClearLogged(bP);
	      break;
	  case elbb_bufLogged:
	      bufferSetWrite(bP);
	      bufferSetLogged(bP);
	      break;
	  case elbb_bufLog:
	      bufferSetRead(bP);
	      bufferClearLogged(bP);
	      break;
	  default:
	      afsl_MBZ(1);
      }
}

/*
 * Write
 *
 * This routine is responsible for scheduling writes; the caller is
 * responsible for waiting for completion (if desired.)  Note that this
 * routine DOES NOT SLEEP!  The buffer lock must be held upon calling!
 */
static int32 Write(struct buffer *bP)
{
      long retval;
      asevent_event_t *ae;
      struct buf *bufp;
      struct bufferPoolEntry *bpeP = bP->poolP;

      BUFFER_ASSERTLOCKED();
      afsl_Assert (!bufferIsUpdate(bP));

      /* first, check to see if this is a redundant write call */
      if (!bufferIsDirty(bP) || bufferIsWritep(bP))
	  return BUFFER_OK;

      /* not redundant, let's actually schedule something to happen! */

      if (bP->currentAgeQ) {
	  int cost = elbl_BufferCommitCost(bP);

	  /* maybe force write log */

#ifndef KERNEL
	  struct timeval t1, t2;
	  int elapsed = 0;
	  if (enableShowList && (cost > 1)) {
	      ShowBufList (bP, NULL);
	      osi_GetTime (&t1);
	      elapsed = 1;
	  }
#endif
	  if (cost > 0) {
	      bpeP->bufferForces++;
	      elbl_ForceBuffer(bP);
	  }

#ifndef KERNEL
	  if (elapsed) {
	      osi_GetTime (&t2);
	      printf ("Force took %d msec\n",
		      (t2.tv_sec - t1.tv_sec)*1000 +
		      (t2.tv_usec - t1.tv_usec + 500)/1000);
	  }
#endif
      }
      /* otherwise, delay the write until buffer reuse */

      DBPV(("calling write on block # %d", bP->blockNum));

      bpeP->bufferWrites++;
      ae = asevent_Create(ASEVENT_NOPIN);
      bufp = asevent_MakeBuf(
		B_ASYNC | B_WRITE, bP->data, bP->adev->blkSize,
		bP->adev, bP->blockNum, 0);
      retval = asevent_QueueIO(bP->adev, bufp, ae);
      if (retval != 0) {
	  asevent_Release(ae);
	  return retval;
      }
      ae->data = (opaque)bP;
      afsl_Assert(bP->aevent == NULL);
      bP->aevent = ae;

      /* mark buffer as having write pending */
      BUFFER_ASSERTLOCKED();
      afsl_Assert(!bufferIsWritep(bP));
      bufferSetWritep(bP);

      return (BUFFER_OK);
}

/*
 * WaitForWrite -- Call this routine to complete a write; it doesn't *start*
 *     the write.  If the flag is set then it waits for it to complete, if
 *     necessary.  If not it only checks to see if the I/O has finished and
 *     completes it if so.  The I/O return value is returned in the output
 *     parameter if it is non-null.  Otherwise we assert there was no error.
 *
 * LOCKS USED -- Called with bufferLock held, but will drop it if waiting for
 *     I/O if required.  Will *not* drop the lock if the wait parameter is
 *     false.  The return code indicates whether the lock was dropped.
 *
 * RETURN CODES -- Returns non-zero if buffer lock was dropped, and zero
 *     otherwise. */

static int WaitForWrite(
  IN struct buffer *bP,
  IN int wait,
  OUT int32 *retvalP)
{
    int32 retval;
    int32 signaled;			/* true iff no waiting necessary */
    asevent_event_t *ae = bP->aevent;

    BUFFER_ASSERTLOCKED();
    afsl_Assert(bufferIsWritep(bP) && !bufferIsUpdate(bP) && ae);

    retval = asevent_StatEvent(ae, &signaled);

    if (!wait && !signaled) {		/* not done yet */
	if (retvalP)
	    *retvalP = 0;
	return 0;
    }

    if (!signaled) {

	/* We must wait so remove the buffer from the free list, and bump the
	 * ref count so it can't go away.  Similarly hold the event we are
	 * waiting on so it will remain valid.  Then wait for the I/O to
	 * complete. */

	if (!bufferIsInuse(bP)) {
	    RemoveFromFreeList(bP);
	}
	BUFFER_HOLD(bP);
	asevent_Hold(ae);

	BUFFER_UNLOCK();		/* do not hold lock across sleep */
	Reschedule();
	retval = asevent_WaitEvent(ae);
	BUFFER_LOCK();

	asevent_Release(ae);
	if (bP->aevent != ae)
	    ae = NULL;			/* no longer the current event */
	BUFFER_RELEASE(bP);
	if (bP->refCount == 0) {
	    /* put buffer back on free list if necessary. */
	    DeallocBuffer (bP);
	}
    }

    if (retvalP) {
	if (retval)
	    CheckIOError(retval, bP);
	*retvalP = retval;
    } else {
	/* if caller isn't checking return code insist that it be zero. */
	afsl_MBZ (retval);
    }

    if (retval)
	return !signaled;		/* we slept */

    /*
     * Clean up this buffer provided that no one has reused it already.
     */
    afsl_Assert(bP->refCount > 0 || ae == NULL || ae->refCount == 1);
    if (bufferIsWritep(bP) && ae) {
	afsl_Assert ((bP->aevent == ae) && !bufferIsUpdate(bP));
	asevent_Release(ae);		/* delete event */
	/* update the buffer header */
	bP->aevent = NULL;
	bufferClearWritep(bP);
	DBPV(("clearing dirty bit for block %d", bP->blockNum));
	bufferClearDirty(bP);
	SetFHLocked(bP, 0);		/* only need to track dirty buffers */
	RemoveAgeQ(bP);
	afsl_Assert (!bP->currentAgeQ && elbt_IsNullTranId((bP)->uzTran));
    }

    return !signaled;
}

/* WaitForUpdate -- waits until the buffer's update bit is cleared.  This
 *     routine does not guarantee the identity of the buffer will remain
 *     unchange because it does not bump the refCount before sleeping like
 *     WaitForWrite does.  If the caller holds a refCount then the identity is
 *     assured.
 *
 * LOCKS USED -- drops the buffer lock. */

static void WaitForUpdate(struct buffer *bP)
{
    while (bufferIsUpdate(bP)) {
	bufferSetNotify(bP);
	BUFFER_SLEEP(&bP->cond);
    }
}

/*
 * HashAddress - address hash function for use by hash package.
 */
static u_long HashAddress(struct buffer *bP)
{
      /* cheap & easy */
      return ((u_long)bP->blockNum ^ (u_long)bP->adev);
}

/*
 * HashFH - file handle hash function for use by hash package.
 */
static u_long HashFH(struct buffer *bP)
{
      return (u_long)bP->bufFH;
}

/*
 * CostToFreeBlock -- compute the "cost" of freeing a 128k block.
 *
 * This is a metric for measuring the cost associated with freeing a particular
 * 128k block.  The current metric is as follows:
 *
 * 1) A block begins with a value of 0.
 * 2) Add 500 for any buffer with a non-zero ref. count.
 * 3) Add 200 for each dirty buffer.
 * 4) Add 100 for each buffer with write pending
 *
 * 128 * 500 = 64000 and 128 * 200 = 25600.  Thus, the maximum score for any
 * single block is 89600.  A score of >= 100K should be treated as terminal
 * (too expensive).
 *
 * It is considered an error to call this routine with an unallocated block.
 */
static void CostToFreeBlock(struct bufferBlock *bbP)
{
      struct buffer *bP;
      unsigned cost;

      BUFFER_ASSERTLOCKED();
      /*
       * Figure out what the cost is for this block.
       */
      cost = 0;
      FIFO_FOR (bP, &bbP->buffers, blockThread) {
	  if (bP->refCount) {
	      cost += RECLAIM_INUSE;
	  }

	  if (bufferIsWritep(bP)) {
	      /* when writep is set dirty remains set so check writep first */
	      cost += RECLAIM_WRITEP;
	  } else if (bufferIsDirty(bP)) {
	      /* dirty but not written yet */
	      int forceCost = elbl_BufferCommitCost (bP);
	      if (forceCost == 0)
		  cost += RECLAIM_DIRTY;
	      else if (forceCost == 1)
		  /* wait for write pending log page then write dirty buf */
		  cost += RECLAIM_DIRTY + RECLAIM_WRITEP;
	      else
		  /* between twice DIRTY if current log page is almost full and
		   * 10+x if logpage is nearly empty. */
		  cost += RECLAIM_DIRTY*forceCost;
	  }
      }
      bbP->reclaimCost = cost;
}

/* AddBufferToSet -- Safely add a buffer to an event set.  Can return busy. */

static long AddBufferToSet(
  IN struct buffer *bP,
  INOUT asevent_set_t **setP)
{
    long code;
    int created = 0;			/* true if we just created this set */

    if (*setP == NULL) {
	*setP = asevent_CreateSet();
	created = 1;
    }
    code = asevent_AddToSet(*setP, bP->aevent);
    if (code == ASEVENT_BUSY) {
	if (created) {
	    asevent_ReleaseSet(*setP);
	    *setP = 0;
	}
	return code;
    }
    afsl_MBZ(code);
    if (!bufferIsInuse(bP)) {
	/* alas, this destroys the LRU ordering... */
	RemoveFromFreeList(bP);
    }
    BUFFER_HOLD(bP);
    return 0;
}

/* DeleteBufferSet -- Safely delete a set of buffer.  Calls WaitForWrite,
 *     without sleeping, on each buffer. */

static void DeleteBufferSet(INOUT asevent_set_t **setP)
{
    asevent_event_t *ae, *oae;
    struct buffer *bP;

    if (*setP == 0) return;		/* no set to delete */

    oae = (asevent_event_t *)0;
    do {
	ae = asevent_EnumerateSet(*setP, oae);
	if (oae) {
	    bP = (elbb_buffer_t *)(oae->data);
	    if (bP != NULL && bufferIsWritep(bP)) {
		WaitForWrite(bP, 0/*!wait*/, 0);
	    }
	    asevent_RemoveFromSet(*setP, oae);
	    /* we detect an I/O error and note it here */
	    if (oae->code) {
		CheckIOError(oae->code, bP);
	    }
	    BUFFER_RELEASE(bP);
	    if (bP->refCount == 0) {
		DeallocBuffer(bP);
	    }
	}
	oae = ae;
    } while (oae);

    asevent_ReleaseSet(*setP);
    *setP = 0;
}


/*
 * HandleLogFull -- called when log informs us we are pinning the tail of the
 *     log (e.g. we must start writing data).
 *
 * PARAMTERS -- The wait is non-zero when the log is so full that the caller
 *     cannot make forward progress without the situation being cleared up.  If
 *     the wait is zero then the log is filling up (half full) and at least
 *     some progress should be made towards reducing log utilization.
 *
 * RETURN CODES -- returns a boolean true if some action was taken.  If wait is
 *     true and the return value is zero this may indicate conditions requiring
 *     some additional action on the part of the caller.  See elbb_StartTran,
 *     for example.
 */
static int HandleLogFull(
  IN async_device_t *dP,
  IN int wait)			/* force the I/O to complete */
{
      struct buffer *bP;
      struct buffer *waitBP;
      struct bufferPoolEntry *bpeP;
      asevent_set_t *aset;
      int32 code;
      unsigned count = 0;
      int action = 0;			/* have we made any progress? */
      int writep = 0;

      /* More aggressive log full handling for testing */
      if (afsdb_episode_logbuf & (1<<(LOGBUF_DEBUG_DEFERLOGFULL-1))) {
	  if (!wait) return 0;
      }

      BUFFER_ASSERTLOCKED();
      bpeP = GetBufferPool(dP->blkSize);
      /*
       * The log is filling up - we try to flush this device in an attempt to
       * unpin the tail of the log.
       */

      /* First, get reasonably cheap (depending on desperation) buffers older
       * than the oldest transaction. */
      do {
	  u_long ageQSize;

	  elbl_FullInfo(dP, 0, &ageQSize, &bP);
	  if (bP && bufferIsDirty(bP) && !bufferIsWritep(bP)) {

	      /* Skip buffers being updated.  If more than half of the ageQ is
               * dirty or the buffer commit cost of this buffer is not too
               * great then schedule this buffer.  If we don't schedule a
               * buffer we have to exit this loop to avoid getting the same
               * buffer indefinitely. */

	      if (bufferIsUpdate(bP))
		  break;
	      if (((ageQSize>8) && (ageQSize > (bpeP->freeList.length>>1))) ||
		  (elbl_BufferCommitCost(bP) < (wait ? 10 : 0))) {
		  action++;
		  Write(bP);
	      } else
		  break;
	  }
	  /* else if (bP) see note below about how buffers can be on the ageQ
	   * and not be dirty. */
      } while (bP);
      if (action && !wait)
	  return 1;

      /*
       * We look through the set of all buffers to find any undo-zero
       * transactions which might be pinning the tail of the log.  As before
       * only bother with cheap ones.
       */
      FORALL (bP, bpeP, dP) {
	  if (!elbt_IsNullTranId(bP->uzTran) && !bufferIsUpdate(bP)) {
	      action++;
	      Write(bP);
	  }
	  if (bufferIsWritep(bP)) {
	      writep++;			/* count these as we go */
	      if (bP->currentAgeQ) {
		  /* see if we can cheaply remove this from the ageQ */
		  WaitForWrite (bP, 0, NULL);
		  if (!bufferIsWritep(bP))
		      action++;
	      }
	  }
      } FORALL_END;

      if (!wait)
	  return (action != 0);

      /* If we didn't schedule some buffers above and if none are already write
       * pending, make sure at least one ageQ buffer is. */
      if (!action && !writep) {
	  bP = 0;
	  while (elbl_NextAgedBuffer(dP, bP, &bP), bP) {
	      if (!bufferIsUpdate(bP) && (elbl_BufferCommitCost(bP) <= 1)) {
		  action++;
		  Write(bP);
		  break;
	      }
	  }
	  if (!action) {
	      /* if no cheap buffers can be written just write the first guy on
	       * The ageQ. */
	      elbl_NextAgedBuffer(dP, 0, &bP);
	      if (bP) {
		  if (bufferIsUpdate(bP)) {
		      WaitForUpdate(bP);
		      return 1;
		  } else
		      Write(bP);
		  action++;
	      }
	  }
      }

      aset = 0;
      count = 0;
      waitBP = NULL;

      /* Now wait for them all. */
      FORALL (bP, bpeP, dP) {
	  if ((elbt_IsNullTranId(bP->uzTran) && !bP->currentAgeQ))
	      continue;
	  if (bufferIsWritep(bP)) {
	      code = AddBufferToSet (bP, &aset);
	      if (code == ASEVENT_BUSY) {
		  /*
		   * This happens when another process is using the event
		   * structure for this buffer.  Since someone else is waiting,
		   * just skip this if we already have events.  Otherwise wait
		   * for this event here.  Then, since we've made some
		   * progress, we can return.
		   */
		  if (count)
		      continue;		/* wait for existing set instead */
		  WaitForWrite(bP, 1, NULL);
		  break;
	      }
	      count++;
	  } else if (bufferIsUpdate(bP)) {
	      waitBP = bP;
	  }
      } FORALL_END;

      if (count) {
	  afsl_Assert (aset);
	  action++;

	  BUFFER_UNLOCK();
	  Reschedule();
	  code = asevent_WaitAll(aset);
	  BUFFER_LOCK();

	  DeleteBufferSet (&aset);
      } else if (waitBP) {
	  action++;
	  WaitForUpdate(waitBP);
      }

      /* Signal the transaction system to cleanup ended transactions.  Forcing
       * only if we've made no progress so far. */
#ifndef KERNEL
      if (enableShowList && !action)
	  ShowBufList (NULL, GetBufferPool(dP->blkSize));
#endif
      elbt_GCTran(dP, !action);
      return (action != 0);
}

/*
 * elbb_QuiesceSystem -- returns when all operations running at the beginning
 *     of the call have been serialized or committed depending on the
 *     parameter.  Also user data can be scheduled and possibly committed as a
 *     parallel option.
 */
/* EXPORT */
void elbb_QuiesceSystem(IN async_device_t *dP, IN u_int consistency)
{
    u_int16 pass, off;
    struct buffer *bP;
    asevent_set_t *aset;
    int32 code;
    unsigned count;
    int serial = (consistency & ELBB_QUIESCE_SERIALIZE);
    int commit = (consistency & ELBB_QUIESCE_COMMIT);
    int write = (consistency & ELBB_QUIESCE_WRITE);
    int fsync = (consistency & ELBB_QUIESCE_FSYNC);
    int flush = (consistency & ELBB_QUIESCE_FLUSH);
    int flushall = (consistency & ELBB_QUIESCE_FLUSHALL);
    int counter;

    if (flushall) flush=1;
    if (flush) fsync=1;
    if (commit) serial = 1;
    if (fsync) write = 1;

    if (serial) {
	/* Snap shot the current log position. */
	elbl_GetLogPageOff(dP, &pass, &off);
    }

    if (write) {
	/*
	 * We write all the dirty buffers on the specified device by walking
	 * through the entire hash pool.  Write does not sleep.
	 */
	BUFFER_LOCK();
	bP = 0;				/* start at first entry */
	while (bP = dfsh_HashNext(&addrHashTable, bP)) {
	    if ((bP->adev != dP) || !bufferIsDirty(bP))
		continue;
	    if (bufferIsUpdate(bP)) {
		WaitForUpdate(bP);
		bP = NULL;		/* start over since we slept */
	    } else if (!bufferIsWritep(bP)) {
		(void) Write(bP);
	    }
	}

	if (fsync) {
	    /* Now we need to wait for these guys... */

	    aset = 0;
	    count = 0;
	    bP=0;

	    while (bP = dfsh_HashNext(&addrHashTable, bP)) {
		if ((bP->adev == dP) && bufferIsWritep(bP)) {
		    code = AddBufferToSet (bP, &aset);
		    if (code == ASEVENT_BUSY) {
			/*
			 * This happens when another process is using the event
			 * structure for this buffer.  Blow this set away,
			 * since we'll have to start over.  Then wait for this
			 * one here and start the loop over.
			 */
			DeleteBufferSet (&aset);
			count = 0;
			WaitForWrite(bP, 1, NULL);
			bP = 0;
			continue;
		    }
		    count++;
		}
	    }

	    if (count) {
		afsl_Assert (aset);

		BUFFER_UNLOCK();
		Reschedule();
		code = asevent_WaitAll(aset);
		BUFFER_LOCK();

		DeleteBufferSet (&aset);
	    }
	} /* if fsync */

	/* buffers synched, now flush them */

	if (flush) {
	    counter = 0;
	    bP = 0;	/* start at beginning of hash table */
	    while (bP = dfsh_HashNext(&addrHashTable, bP)) {
		/* this code flushes every data buffer per aggregate */

		if (bP->adev == dP && (flushall || !bufferIsLogged(bP))) {
		    if (bP->refCount > 0 && !osi_IsKernelPanic()) {
			/* wait for refCount to go to 0 */

			bufferSetReclaim(bP);
			BUFFER_SLEEP(&bP->cond);
			if ((++counter % 10) == 0) {
			    printf("Unexpected counter overflow\n");
			    printf("in elbb_QuiesceSystem\n");
			}
			bP = 0;
			continue;
		    }

		    /* don't invalidate a dirty buffer */

		    if (bufferIsDirty(bP)) {
			if (bufferIsUpdate(bP)) {
			    WaitForUpdate(bP);
			    bP = NULL;	/* start over since we slept */
			    continue;
			}

			if (!bufferIsWritep(bP))
			    code = Write(bP);

			WaitForWrite(bP, 1, &code);
			afsl_MBZ(code);
			/* We may have dropped the lock, so go around again. */
			bP = 0;
			continue;
		    }

		    RemoveFromAddrHash(bP);
		    bP = 0;
		}
	    }
	}

	BUFFER_UNLOCK();
    } /* if write */

    if (serial) {
	/* By now, with any luck, we should have little waiting to do here. */
	elbt_Quiesce (dP, !!commit|(!!fsync<<1), pass, off);
    }
}

/*
 * CheckIOError-
 *
 * This routine is called when an I/O error was signalled from the I/O
 * subsystem.  If necessary, this routine will panic.  In user-space it jumps
 * through some hoops to not print errors about EINVAL and ENXIO because
 * test_anode generates these as a matter of course.
 */
static void CheckIOError(long code, struct buffer *bP)
{
#if !defined(KERNEL)
      if ((code == ENXIO) || (code == EINVAL))
	  return;
#endif /* KERNEL */
      if (code == 0) /* no error, shouldn't be called for this, though */
	  return;

      /*
       * We mark this buffer as "bad" and print an error identifying the
       * error value, the major & minor number and the block # on the device.
       * Not much else we can do.
       */
      bufferSetError(bP);
      printf("Error %d on (%d,%d) block %d\n", code, bP->adev->majdev,
	     bP->adev->mindev, bP->blockNum);

      /*
       * OK.  Now, if this was a meta-data buffer, we panic.  Note that we
       * ALWAYS call panic so that this string doesn't get sliced & diced
       * out by some misguided attempt to "turn off" debugging code.
       */
      if (bufferIsLogged(bP) && ((code) == EIO))
	  panic("Unrecoverable I/O error in meta-data");
}
