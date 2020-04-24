/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* Log.c */

/* Copyright (C) 1995, 1990 Transarc Corporation - All rights reserved */

/*
 * Based upon the Episode Buffer/Log/Recovery Specification and Design.
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/sysincludes.h>
#include <dcedfs/lock.h>
#include <dcedfs/osi.h>
#include <dcedfs/debug.h>
#include <dcedfs/hash.h>
#include <dcedfs/episode/epi_thread.h>
#include <dcedfs/episode/async.h>
#include <dcedfs/episode/instrument.h>

#include <elb.h>
#include <logrec.h>
#include <recover.h>
#include <tran.h>

RCSID ("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/logbuf/RCS/log.c,v 4.226 1995/06/28 15:04:35 ota Exp $")

#define DEBUG_THIS_FILE LOGBUF_DEBUG_LOG
#define elbl_VDBprint(p) \
    dmprintf(afsdb_episode_logbuf, LOGBUF_DEBUG_VLOG, p)

extern int afsdb_episode_logbuf;
int afsdb_episode_logSleepCnt; /* used for debugging recovery */

/* types */
struct logMap {
    struct logMap        *next;         /* hash table linked list */
    struct async_device  *logDev;       /* device containing log */
    struct async_device  *dev;          /* device to log */
    struct fifo           freeBuf;      /* free buffers for this log */
    unsigned              numBufs;      /* # of buffers assigned to this log */

    /* To track the tran module's state.  See state diagram below. */
    struct fifo           aTranList;    /* active transaction list */
    struct fifo           pTranList;    /* pending transaction list */
    struct fifo           cTranList;    /* completed transaction list */

    struct fifo_Thread    mapStatus;    /* status thread for log map */
    struct fifo           inTransit;    /* buffers in transit to disk */
    struct buffer        *current;      /* current buffer in use */
    daddr_t              *logLocnArray; /* blocks for log */
    u_int32               logLocnArraySize; /* size of array */
    u_int16               currentLogBlock; /* current log block in use */
    u_int16               activeLogBlock; /* = currentLogBlock - pending i/o */
					/* first block not committed to disk */
    u_int16               passno;	/* pass number */
    u_int16               activeSize;	/* current active size of the log */
    u_int16               fullSize;	/* start log full proc. here */
    u_int32		  reserved;	/* bytes reserved for all trans */
    u_int32               lsn;		/* current lsn value */
    unsigned              logStatus;	/* log status */
    u_int32               currentBuf;	/* current buffer in pool in use */
    epi_mutex_t		  mapLock;	/* lock per map entry */
    asevent_set_t	 *eventSet;	/* event set per map entry */
    struct logPageHdr    *pageCache;	/* cache page entry */
    unsigned              gcstatus;	/* true if gc in order */
    u_int32               maxTranSize;	/* max # bytes in a single tran */
    u_int32               maxEndSize;	/* max # bytes in an end record */
    u_int32               maxUndoSize;	/* max # bytes in an undo zero tran */
    struct fifo           bufferAgeQ;	/* age queue for buffers */
    unsigned              tranSleepers;	/* # waiters for tran to complete */
    epi_cond_t		  cond;
    struct fifo           tranAgeQ;	/* age queue for transaction */
    unsigned char         cksum_ver;	/* Checksum Version */

    /* Measure recent average log page utilization.  See also NewLogPage. */
    u_long		  sumUtil;	/* decaying sum of util. in bytes */
    u_long		  avgUtil;	/* weighted avg from recent pages */
    u_long		  nSamples;	/* # pages included in sum */
    u_long		  deratedLogSize; /* log size reduced by eff. util */

    u_long		  avgActiveTrans; /* (16x) avg of # running trans */
};


static void EnsureFreeBuffer(struct logMap *);
static struct dfsh_hashTable logMapHashTable;

static struct logMap *logMapCache;

#define LookupLogMap(dP, lmP) \
    MACRO_BEGIN \
	if (!logMapCache || logMapCache->dev != dP) \
            DFSH_LOOKUP(struct logMap *, &logMapHashTable, logMapCache, dP, \
			logMapCache->dev == (dP)); \
	lmP = logMapCache; \
    MACRO_END

#define GetLogMap(dP, lmP) \
    MACRO_BEGIN \
	LOG_LOCK(); \
	LookupLogMap(dP, lmP); \
	if (lmP) \
	    map_lock(lmP); \
	LOG_UNLOCK(); \
    MACRO_END

/* Transactions are managed jointly by the tran and log modules.  The log
 * module has only a resticted view and gets most of its information from the
 * tran module.  The major contributions of the log module are that it knows
 * about log full conditions, transaction space reservations, and when a
 * transaction is really committed.
 *
 * To track the transitions that the tran module cares about the log module
 * keeps transactions on various fifos.  The state diagram explains the legal
 * transactions:
 *
 *       procedure        fifo chgs     res.   transition conditions
 *       *********        *********     ****   *********************
 *     elbl_StartTran    => tranAgeQ    full   n/a
 *                       => aTranList
 *
 *     elbl_PendingTran  => pTranList   end    from active
 *
 *     elbl_CompleteTran => cTranList   zero   from active or pending
 *                       <= tranAgeQ
 *
 *     elbl_GCTran       <= cTranList   n/c    from complete if committed
 *
 * The result of this is that the tranAgeQ is used to pin the tail of the log.
 * The reservation required is:
 *     length(aTranList) * (nested ? undoSize : fullSize) +
 *     length(pTranList) * endSize
 * The transactions on the cTranList are being held until their end records are
 * actually committed to disk.  Then they are returned to the tran module when
 * it calls elbl_GCTran.
 *
 * Each fifo has a corresponding bit in the log module's portion of tran
 * structure for consistency checking.
 */

#define FindOldestCompleteTran(lmP) \
    FIFO_LookAtOldest(&(lmP)->cTranList, \
		      offsetof(struct elbt_TranRec, logMapThread))
#define FindOldestActiveTran(lmP) \
    FIFO_LookAtOldest(&(lmP)->aTranList, \
		      offsetof(struct elbt_TranRec, logMapThread))
#define FindOldestTran(lmP) \
    FIFO_LookAtOldest(&(lmP)->tranAgeQ, \
		      offsetof(struct elbt_TranRec, logAgeThread))
#define FindOldestBuffer(lmP) \
    FIFO_LookAtOldest(&(lmP)->bufferAgeQ, offsetof(struct buffer, ageQ))

/* ComputePassDistance -- returns -size, 0, or size depending on whether pass1
 *     preceeds, is the same as, or follows pass2.  That is, the distance will
 *     be positive if pass1 is "ahead of" pass2.
 *
 *     We use an array lookup based on the low three bits of the 2 complement
 *     difference because across the (pass==0) boundary the arithmetic
 *     difference between adjacent pass numbers will be 2 (or -2).  Other tests
 *     would results in lots of conditionals.
 *
 * CAUTIONS -- pass1 and pass2 should never be more than one logical page
 *     apart.  If they are, the array will return 9 which should lead to
 *     garbage that out caller can assert against.
 */

int elbl_passDistance[8] = {
    /* 000b */0, /* 001b */1, /* 010b */1, /* 011b */9,
    /* 100b */9, /* 101b */9, /* 110b */-1,/* 111b */-1 };

#define ComparePass(pass1, pass2) \
    (elbl_passDistance[((pass1) - (pass2)) & 0x7])

#define ComputePassDistance(lmP, pass1, pass2) \
    ((int)(lmP)->logLocnArraySize * ComparePass(pass1, pass2))

/* ComputeLogDistance -- returns number of pages from point two to point one in
 *     the log.  This means the result is positive if #1 is *NEWER* than #2.
 *
 *     As with pass numbers, log offsets are unsigned int16s.  So we have to do
 *     arithmetic carefully.
 *
 * CAUTIONS -- Pass numbers must be logically adjacent.
 */

#define ComputeLogDistance(lmP, pass1, off1, pass2, off2) \
    (ComputePassDistance(lmP, pass1, pass2) + \
     (/*signed*/ int16)((off1) - (off2)))

#if 0 /* this comment is probably obsolete */
/*
 * Using the information in the log map entry, we compute the active
 * size of the log and update.  We add one to the result of this computation
 * because our size is inclusive of both endpoints (the oldest page and
 * the current page.)
 *
 *
 * PERF: this might be better with a good optimizing compiler if it reevaluated
 * its arguments and eliminated to (potential) stack storage of variables.
 */
/*
 * In the following rationale we are presuming the log is always ahead of the
 * oldest transaction; given this, we can look at four possibilities:
 *
 * (1) they are equal.  In this case lmP->passno - tP->firstPass = 0
 *     as does lmP->currentLogBlock - tP->firstOffset + 1; or
 *
 * (2) lmP->passno == tP->firstPass but lmP->currentLogBlock > tP->firstOffset,
 *     in which case size is (lmP->currentLogBlock - tP->firstOffset) + 1; or
 *
 * (3) lmP->passno > tP->firstPass but lmP->currentLogBlock <= tP->firstOffset
 *     in which case the size is lmP->logLocnArraySize - (tP->firstOffset +
 *     lmP->currentLogBlock) + 1; or
 *
 * (4) lmP->passno > tP->firstPass but lmP->currentLogBlock > tP->firstOffset,
 *     which is a log overflow condition, in which case the size is
 *     lmP->logLocnArraySize + (lmP->currentLogBlock - tP->firstOffset) + 1.
 *
 * A similar analysis applies to the oldest buffer.
 */

/*
 * note this definition assumes other code catches the case where
 * the log has "wrapped around" before it gets here.
 */
#endif

#define COMPUTE_TRAN_ACTIVE_LOG_SIZE(lmP,tP) \
    ((tP) \
     ? 1 + ((unsigned) \
	    ComputeLogDistance(lmP, (lmP)->passno,(lmP)->currentLogBlock, \
			       (tP)->firstPass,(tP)->firstOffset)) \
     : 1)

#define COMPUTE_DATA_ACTIVE_LOG_SIZE(lmP,bP) \
    ((bP) \
     ? 1 + ((unsigned) \
	    ComputeLogDistance(lmP, (lmP)->passno,(lmP)->currentLogBlock, \
			       (bP)->bufLogFirstPass,(bP)->bufLogFirstOff))\
    : 1)

/* EarlierPassno -- returns the passno corresponding to a log offset known to
 *     be earlier (older) than currentLogBlock. */

#define EarlierPassno(lmP, off) \
    (((lmP)->currentLogBlock < (off)) ? \
     DecPassno((lmP)->passno) : (lmP)->passno)

#define TranOneOlderThanTwo(t1P,t2P) \
    (ComparePosition (t1P->firstPass,t1P->firstOffset, \
		      t2P->firstPass,t2P->firstOffset) < 0)

/* TranCommitted -- returns true if tP's pass/off are strictly older than log's
 *     pass/off.  We use activeLogBlock for off; it is the most recent block
 *     *NOT* written to disk. */

#define TranCommitted(lmP,tP) \
    (ComparePosition ((tP)->lastPass, (tP)->lastOffset, \
		      EarlierPassno(lmP, (lmP)->activeLogBlock), \
		      (lmP)->activeLogBlock) \
     < 0)

/* BufCommitted -- returns true if bP's pass/off are strictly older than log's
 *     pass/off.  We use activeLogBlock for off; it is the most recent block
 *     *NOT* written to disk.  This function is analogous to TranCommitted and
 *     is used in UnageBuffer to assert that the buffer being removed from the
 *     bufAgeQ is indeed safe to remove. */

#define BufCommitted(lmP,bP) \
    (ComparePosition ((bP)->bufLogLastPass, (bP)->bufLogLastOff, \
		      EarlierPassno(lmP, (lmP)->activeLogBlock), \
		      (lmP)->activeLogBlock) \
     < 0)

#ifndef LOG_FULL_BUFFER
#define LOG_FULL_BUFFER (3)
#endif /* LOG_FULL_BUFFER */

#define IsLogFull(lmP) \
    ((lmP->activeSize + LOG_FULL_BUFFER) > lmP->logLocnArraySize)

/* BytesToLogBlocks -- converts a number of bytes to the smallest number of
 *     blocks that will hold them on the log device (i.e. An upward rounding
 *     divide by bsize). */

#define BytesToLogBlocks(lmP,bytes) \
    (((bytes) + ((lmP)->logDev->blkSize - 1)) >> (lmP)->logDev->logBlkSize)

/*
 * PERFORMANCE: all the macros to handle the elbl_Write/WriteLocked
 * case are to avoid the overhead of a function call from elbl_Write
 * to WriteLocked.
 */

/*
 * code to print out log record information; debugging only
 */
#if defined AFS_DEBUG
#define DumpLogRecord(newRecord) \
MACRO_BEGIN \
    if (afsdb_episode_logbuf & LOGBUF_DEBUG_VLOG) { \
	elbl_VDBprint(("writing new record:")); \
	elbl_VDBprint(("\trecType: 0x%x", newRecord->hdr.recType)); \
	elbl_VDBprint(("\trefCount: %d", newRecord->hdr.refCount)); \
	elbl_VDBprint(("\tdataLength: %d", newRecord->hdr.dataLength)); \
	elbl_VDBprint(("\ttranId: 0x%x", newRecord->hdr.tranId)); \
	elbl_VDBprint(("\tdataAddr: 0x%x", newRecord->hdr.dataAddr)); \
	elbl_VDBprint(("\tdataOffset: 0x%x", newRecord->hdr.dataOffset)); \
	elbl_VDBprint(("\tprevLogRecOff: %d", newRecord->hdr.prevLogRecOff)); \
	elbl_VDBprint(("\tlsn: %d", GetLogrecLSN(newRecord, 0))); \
    } \
MACRO_END
#else
#define DumpLogRecord(newRecord)
#endif /* AFS_DEBUG */

/*
 * This code handles updating a transaction after the log record has been
 * modified.
 */
#define UpdateTran(tP, pass, off) \
MACRO_BEGIN \
	/*  we just update the last information. */ \
	(tP)->lastPass = pass; \
	(tP)->lastOffset = off; \
MACRO_END

/*
 * This code handles writing a log record;  it is used by both the
 * elbl_Write and WriteLocked calls; elbl_Write handling the common
 * case where no special page handling need be done and WriteLocked
 * handling calls from within the package + the special-case code.
 */
#define WriteLogRecord(newRecord, newrecOffset, lmP, recType, \
		tP, dataLength, diskBlock, dataOffset, oldData, newData) \
MACRO_BEGIN \
    newRecord = GetLogrec((lmP)->current->data, newRecOffset); \
    elbr_InitLogRecHdr( &((newRecord)->hdr) ); \
    (newRecord)->hdr.recType = (u_int16)(recType); \
    (newRecord)->hdr.refCount = ++((tP)->refCount); \
    (newRecord)->hdr.dataLength = dataLength; \
    (newRecord)->hdr.tranId  = (tP)->tranId; \
    (newRecord)->hdr.dataAddr = diskBlock; \
    (newRecord)->hdr.dataOffset = dataOffset; \
    (newRecord)->hdr.prevLogRecOff = (lmP)->pageCache->lastLogRecOff; \
    *(u_int32 *)((char *)(newRecord) + LSNOffset(newRecord)) = \
	(lmP)->lsn = (++((lmP)->lsn) ? (lmP)->lsn : ++((lmP)->lsn)); \
    DumpLogRecord(newRecord); \
    /* Remember the page offset */ \
    lmP->pageCache->lastLogRecOff = newRecOffset; \
    /*  The new record is in the page, now we need to copy the data... */ \
    if (dataLength) { \
	if (LOGREC_ISFILL((u_int16)recType)) { \
	    if (LOGREC_HASOLDDATA((u_int16)recType)) \
		LOGCOPY(oldData,(char *)newRecord+OldDataOffset(newRecord),1);\
	    if (LOGREC_HASNEWDATA((u_int16)recType))\
		LOGCOPY(newData,(char *)newRecord+NewDataOffset(newRecord),1);\
	} else { /* must be BEND or DATA types */\
	    if (LOGREC_HASOLDDATA((u_int16)recType)) \
		LOGCOPY(oldData, (char *)newRecord + OldDataOffset(newRecord),\
			newRecord->hdr.dataLength); \
	    if (LOGREC_HASNEWDATA((u_int16)recType)) \
		LOGCOPY(newData, (char *)newRecord + NewDataOffset(newRecord),\
			newRecord->hdr.dataLength);\
	}\
    } \
    (tP)->tranSize += NextRecOffset(newRecord); \
    newRecOffset += NextRecOffset(newRecord); \
MACRO_END

#define LogPageRemaining(lmP, offset) \
    ((lmP)->current->adev->blkSize - (offset))


#ifndef LOG_FULL_MAGIC
#define LOG_FULL_MAGIC 64 /* this gives us 50% */
#endif /* LOG_FULL_MAGIC */

static unsigned logFullSize = LOG_FULL_MAGIC;

#define LOG_FULL_POWEROFTWO 7

/*
 * This macro should be called after the log map record is updated;
 * it depends on the map being up-to-date.
 *
 * PERF: we use 2^LOG_FULL_POWEROFTWO as the divisor to get better
 * computational speed.  This is close enough to approximate almost
 * any desired value. I've started it out at 50% full.
 */
#define IsLogGettingFull(lmP) \
    ((lmP)->fullSize < (lmP)->activeSize)

#define IsLogPageFull(lmP, offset) \
    (LogPageRemaining(lmP, offset) < (NewLogrecSize(LOGREC_DATA, ALIGN_SIZE)))

#define GetNewRecordOffset(lmP, newRecOffset, prevRecord) \
MACRO_BEGIN \
      if ((lmP)->pageCache->lastLogRecOff)  { \
	  prevRecord = GetLogrec((lmP)->current->data, \
				 (lmP)->pageCache->lastLogRecOff); \
	  newRecOffset = (lmP)->pageCache->lastLogRecOff + \
	      NextRecOffset(prevRecord); \
      } else { \
	  newRecOffset = elbr_FirstLogRecordOffset((lmP)->pageCache) ; \
      } \
MACRO_END

/*
 * AgeBuffer - take a buffer and add it to the age queue if necessary
 * this macro assumes the buffer is on an age queue if it points to it!
 *
 * Note that aging must begin when the log record begins!  Otherwise the
 * log will not compute its length properly.
 */
#define AgeBuffer(bP, lmP) \
MACRO_BEGIN \
    map_assertlocked(lmP); \
    if ((bP)->currentAgeQ) {\
	afsl_Assert((bP)->currentAgeQ == &(lmP)->bufferAgeQ); \
    } else { /* time stamp it & age it */ \
	bP->bufLogFirstPass = lmP->passno; \
	bP->bufLogFirstOff = lmP->currentLogBlock; \
	FIFO_ADD(&(lmP)->bufferAgeQ, &(bP)->ageQ); \
        (bP)->currentAgeQ = &(lmP)->bufferAgeQ; \
    } \
MACRO_END

static struct fifo logMapFreeList;	/* free log map entries */
static struct fifo logMapActiveList;	/* active log map entries */

static epi_mutex_t logLock;		/* log package lock */

#define LOG_LOCK() epi_mutex_lock(&logLock)
#define LOG_UNLOCK() epi_mutex_unlock(&logLock)
#define LOG_IS_LOCKED()	(epi_mutex_held(&logLock))

#define map_lock(map) epi_mutex_lock(&(map)->mapLock)
#define map_unlock(map) epi_mutex_unlock(&(map)->mapLock)
#define map_sleep(map) epi_cond_wait(&map->cond, &(map)->mapLock)
#define map_assertlocked(map) afsl_Assert(epi_mutex_held(&(map)->mapLock))

#define IsActiveTrans(mapP) ((mapP)->tranAgeQ.length)

/*
 * Locking in the log package consists of the following locks:
 *
 * LOG_LOCK: global lock on internal data structures
 * map_lock: lock on individual map records
 *
 * To acquire a map_lock the LOG_LOCK must be held, LookupLogMap is called,
 * map_lock is taken, and then LOG_LOCK is dropped.
 *
 * [ do we need a more complex locking scheme for a transaction?
 *   currently reads against a transaction could be inconsistent; an
 *   alternative to this would be to provide query functions which
 *   lock the transaction while reading it. - tm 2/12/91 ]
 *
 */

#define LOG_LOGMAP_STAT_OK (0x1A70)
#define LOG_LOGMAP_STAT_SHUTDOWN (0xFE3)
#define LOG_LOGMAP_STAT_CLOSED (0xc3d784)
#define LOG_LOGMAP_STAT_FREE (0)

/* maximum number of logs to support */
#if !defined(MAX_NUM_LOGS)
#define MAX_NUM_LOGS  (1024)
#endif /* MAX_NUM_LOGS */

static elbl_MaxLogs = MAX_NUM_LOGS;

/*
 * this is presumtive, not declarative; it reflects our understanding of
 * higher-level users.
 */
#ifndef MAX_TRAN_SIZE
#define MAX_TRAN_SIZE (70 * 1024)
#endif
#ifndef MIN_TRAN_BLOCKS
#define MIN_TRAN_BLOCKS 5
#endif

#define CheckIOError(code, devP, blk) \
    MACRO_BEGIN \
	if (code) { \
	    printf("Error %d in log on (%d,%d) block %d\n", (code), \
	       (devP)->majdev, (devP)->mindev, (blk)); \
	    panic("Unrecoverable I/O error in log"); \
	} \
    MACRO_END
/*
 * these macros are used to pack/unpack disk representation into in-core &
 *  vice-versa.
 */
/* forward references */
static void AllocLogMap(struct logMap **, struct async_device *);
static void DeallocLogMap(struct logMap *);
static void InitNewPage(struct logMap *, struct buffer *, unsigned);
static long UnpackLogRec(struct logRec *, caddr_t);
static long RecycleLogBufs(struct logMap *);
#if defined (AFS_AIX31_ENV)
/* this declaration fails on AIX, but I don't know why! */
static long ForceMap ();
#else
static long ForceMap(struct logMap *, unsigned);
#endif
static long WriteLocked(struct elbt_TranRec *tP, unsigned,
				  u_int32, daddr_t, u_int32, caddr_t,
				  const caddr_t, unsigned, u_int32 *);
static long ECCountLocked(struct elbt_TranRec *, unsigned *, unsigned *);
static long ECEquiv(struct elbt_TranRec *, struct elbt_TranRec *);
static void InitLogMap(void);
static void ForceAllLogPages(struct logMap *);
static void WaitForAllLogPages(struct logMap *);
static void NewLogPage(struct logMap *);
static void ComputeActiveLog(struct logMap *);
static unsigned elbl_MinTranReservation(unsigned);

#if !defined(MAX_TRAN_RECORDS)
#define MAX_TRAN_RECORDS (1024)
#endif /* MAX_TRAN_RECORDS */

static u_long elbl_MaxTran = MAX_TRAN_RECORDS;


/*
 * instrumentation related routines.
 */
/*
 * the list of tunable variables in the log package. These are supported by
 * ins_log_tune procedure.
 */
static struct ins_varTableEntry varTable[] = {
    {(long *)&elbl_MaxLogs,
	 (long)sizeof(elbl_MaxLogs)}, /* index 0 - logMaxLogs */
    {(long *)&logFullSize,
	 sizeof(logFullSize)},	     /* index 1 - logFullSize */
    {(long *)&elbl_MaxTran,
	 sizeof(elbl_MaxTran)}	     /* index 2 - logMaxTran */
    /* the rest are computed */
};

static long varTableSize = sizeof(varTable)/sizeof(struct ins_varTableEntry);

/*
 * variable indices supported by ins_log procedure.
 */
#define PASSNO 1	/* passno */
#define ACTIVESIZE 2	/* activeSize */
#define LSN 3		/* lsn */
#define ECTHROTTLE 4	/* ecThrottleCounter */


/*
 * get or set the value of variable indicated by <vars>->varIdx. The
 * result of lookup is stored in <resultValues>. Object Id parameters
 * are ignored for now.
 */
/* EXPORT */
int ins_log_tune(
  IN struct ins_opDesc *opcode,
  IN struct ins_varDesc *vars,
  IN struct ins_basicObjectId *argObjIds,
  OUT struct ins_basicObjectId *resultObjIds,
  OUT struct ins_basicObject *resultValues)
{

    if (vars->varIdx >= 0 && vars->varIdx < varTableSize){
	/* the variable index is within range */
	LOG_LOCK();
	if (opcode->opcode == INS_GET) {
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
	LOG_UNLOCK();

	return 0;
    } else{
	return EINVAL;
    }
}


/*
 *
 */
/* EXPORT */
int ins_log(
  IN struct ins_opDesc *opcode,
  IN struct ins_varDesc *vars,
  IN struct ins_basicObjectId *argObjIds,
  OUT struct ins_basicObjectId *resultObjIds,
  OUT struct ins_basicObject *resultValues)
{
    long *dataAddr;
    long dataSize;
    int error = 0;

    dataAddr = (long *)0;
    dataSize = 0;
    LOG_LOCK();
    if (logMapCache) {	/* one element cache is active */
	if (vars->varIdx) {
	    switch (vars->varIdx) {
	      case PASSNO:
		dataAddr = (long *) &(logMapCache->passno);
		dataSize = sizeof(logMapCache->passno);
		break;
	      case ACTIVESIZE:
		dataAddr = (long *) &(logMapCache->activeSize);
		dataSize = sizeof(logMapCache->activeSize);
		break;
	      case LSN:
		dataAddr = (long *) &(logMapCache->lsn);
		dataSize = sizeof(logMapCache->lsn);
		break;
	      case ECTHROTTLE:
		dataAddr = (long *) 0;
		dataSize = 0;
		break;
	      default:
		break;
	    }
	}
	if (dataAddr && dataSize && dataSize <= INS_MAXOBJLEN) {
	    if (opcode->opcode == INS_SET) {
		/* can't set readonly values */
		error = EINVAL;
	    } else if (opcode->opcode == INS_GET) {
		bcopy((caddr_t)dataAddr, (caddr_t)resultValues->value,
			dataSize);
	    } else if (opcode->opcode == INS_GETNEXT) {
		/* not yet supported */
	    }
	}
    }
    LOG_UNLOCK();
    return error;
}


/* routines */

/*
 * Initialization done only once.
 */

void elbl_Init (void)
{
      static int internal_initialized = 0;

      if (internal_initialized)
	  return;

      epi_mutex_init(&logLock, "log");

      /* initialize the log map system */

      InitLogMap();
      internal_initialized++;
}

/*
 * elbl_InitLog - Initialize a new log.
 *
 * LOCKS: Takes LOG_LOCK and map lock.  May sleep with LOG_LOCK held.
 *
 * TODO: this routine should probably allocate the log map before calling
 *       recovery so only that device need be locked out. - tm 2/14/91
 */

/* SHARED */
long elbl_InitLog(
     IN struct async_device *ldP,
     IN struct async_device *dP,
     IN daddr_t *logLocnArray,
     IN u_long logLocnArraySize,
     IN struct buffer **logBufArray,
     IN u_long logBufArraySize,
     IN long (*fsync)(struct async_device *)) /* XXX obs. */
{
      struct logMap *lmP;
      struct buffer *bP;
      long retval;
      struct logPageHdr *lphP,myLogHdr;
      unsigned index;
      int code;
      asevent_event_t *ae;
      struct buf *bufp;

      /* first, check to assure this devices isn't already logged */
      LOG_LOCK();
      LookupLogMap(dP, lmP);

      if (lmP) {
	  LOG_UNLOCK();
	  return LOG_LOGEXISTS;
      }

#ifdef AFS_DEBUG
      if (afsdb_episode_logbuf & (1 << (LOGBUF_DEBUG_VLOG))) {
	  int index2;

	  elbl_VDBprint(("log created with blocks:"));
	  for (index2 = 0; index2 < logBufArraySize; index2++) {
	      elbl_VDBprint(("\tpage # %d is block #: %d",
			       index2, logBufArray[index2]->blockNum));
	  }
      }
#endif


      /*
       * We check to see if there is a clean restart record on
       * disk.  If so, we continue.  If not, we attempt recovery and
       * return the resulting error value (never OK) to the caller.
       */
      if (elbr_GetCleanLogInfo(ldP, *logLocnArray, &myLogHdr) == 0) {
	  code = elbr_Recover(ldP, dP, logLocnArray, logLocnArraySize);
	  LOG_UNLOCK();
	  if (code == 0)
	      return RCVR_RECOVERED;	/* so upper levels can clear buffers */
	  else return code;
      }
      lphP = &myLogHdr;

      /* now, obtain a new log record */
      AllocLogMap(&lmP, dP);

      if (!lmP) {
	  LOG_UNLOCK();
	  return LOG_TOOMANYLOGS;
      }

      if (elbl_MinLogSize(lmP->dev, 0) > logLocnArraySize) {
	  /*
	   * free it and return log full (we didn't get enough
	   * space!
	   */
	  lmP->dev = (struct async_device *)0;
	  lmP->logStatus = LOG_LOGMAP_STAT_FREE;
	  FIFO_REMOVE(&logMapActiveList, &lmP->mapStatus);
	  FIFO_ADD(&logMapFreeList, &lmP->mapStatus);
	  LOG_UNLOCK();
	  return LOG_LOGFULL;
      }

      /* lock map entry */
      map_lock(lmP); /* lock out access to this map page */
      LOG_UNLOCK(); /* don't need whole-log lock anymore */

      lmP->logDev = ldP;
      lmP->dev = dP;
      lmP->cksum_ver = myLogHdr.cksum_ver;

      /*
       * initialize the first log buffer
       */
      bP = *logBufArray;
      bP->adev = ldP;
      bP->blockNum = *logLocnArray;
      bP->bufType = elbb_bufLog;

      ae = bP->aevent = asevent_Create(ASEVENT_NOPIN);
      ae->data = (opaque)bP;
      bufp = asevent_MakeBuf(
		B_READ, bP->data, bP->adev->blkSize,
		bP->adev, bP->blockNum, 0);
      retval = asevent_QueueIO(bP->adev, bufp, ae);
      if (retval == 0)
	 retval = asevent_WaitEvent(ae);
      CheckIOError(retval, bP->adev, bP->blockNum);
      asevent_Release(ae);

      /* insert into hash table */
      DFSH_HASHIN_R(&logMapHashTable, lmP);

      lmP->currentLogBlock = 0;
      FIFO_INIT (&lmP->freeBuf, offsetof(struct buffer, freeList));
      FIFO_INITTHREAD (&bP->freeList);
      FIFO_INIT (&lmP->inTransit, offsetof(struct buffer, ageQ));
      lmP->current = bP;

      /* put all but first on free list. */
      for (index = 1; index < logBufArraySize; index++) {
	  bP = *(logBufArray + index);
	  FIFO_ADD (&lmP->freeBuf, &bP->freeList);
	  bP->bufType = elbb_bufLog;
	  /*
	   * block num is set when buffer is really used.
	   * -wam [ota]
	   */
      }
      lmP->numBufs = logBufArraySize;
      lmP->logLocnArray = logLocnArray;
      lmP->logLocnArraySize = logLocnArraySize;
      lmP->passno = IncPassno(lphP->passNumber);
      lmP->activeSize = 1;		/* current page is always active */
      lmP->fullSize = ((lmP->logLocnArraySize *
			logFullSize)>>LOG_FULL_POWEROFTWO);
      lmP->reserved = 0;
#if 0
      lmP->lsn = restart.lsn;
#endif
      lmP->logStatus = LOG_LOGMAP_STAT_OK;

      /* now, set up the first buffer */
      InitNewPage(lmP, lmP->current, 0);
      afsl_Assert (!(lmP->current) || (lmP->current->bufType == elbb_bufLog));
      lmP->current->bufFH = 0; /* OVERLOAD: which offset in log block array */

      map_unlock(lmP);
      return BUFFER_OK;
}

/*
 * elbl_MinLogSize: compute the smallest possible log necessary for a certain
 * workload.
 *
 * Locking: none necessary.
 */
/* EXPORT */
unsigned elbl_MinLogSize(struct async_device *dP, unsigned load)
{
      unsigned minSize;

      /*
       * must be one block larger than the smallest possible log necessary
       * (see the code in log_Init where it computes if the logLocnArraySize
       * is large enough).
       */
      if (load == 0) load = 1; /* load can never be zero... */
      minSize = (load * elbl_MinTranReservation(dP->blkSize)) +
	  LOG_FULL_BUFFER + 1;
      return minSize;
}

/*
 * elbl_MinTranReservation: compute the initial reservation in logical device
 * blocks to use for a specific device.
 *
 */
static unsigned elbl_MinTranReservation(u_int blkSize)
{
      unsigned int defBlks, nblks;

      /* not much, but it is something */
      afsl_MBZ(blkSize & 1023); /* must be power-of-two >= 1K */
      /*
       * Algorithm: The minimum is 5 log blocks OR MAX_TRAN_SIZE (35K),
       * whichever is larger.
       */
      defBlks = ((MAX_TRAN_SIZE + blkSize)/blkSize);

      nblks = MIN_TRAN_BLOCKS;

      return ((defBlks < nblks) ? nblks:defBlks);
}

/*
 * elbl_Create: handle creation of a new log.
 *
 * Locking: This code presumes no other thread is calling for this
 * specific device.  Because it calls the buffer package to perform the
 * I/O, it presumes locks are not held in the buffer package.
 *
 * XXX -- This is stupid, we should do our own I/O and not have all these
 *     dependencies on the buffer layer. */

/* SHARED */
long elbl_Create (
     IN struct async_device *ldP,
     IN const daddr_t *logLocnArray,
     IN unsigned logLocnArraySize,
     IN unsigned char cksumVer)
{
      struct buffer *bP;
      struct logRec restart;
      unsigned index;
      struct logPageHdr newHdr;
      unsigned code;

      /*
       * we build the prototype "newHdr"
       */
      elbr_InitLogPageHdr(&newHdr,cksumVer);

      /*
       * We skip the first block because we will be writing a restart
       * record into it.  All other blocks are initialized to zero.
       * Note that we don't try to optimize this at all; this is a
       * rare operation.
       */
      for (index = 1; index < logLocnArraySize; index++) {
	  buffer_CreateBlock(ldP, logLocnArray[index], elbb_bufData, &bP);

	  /*
           * bzero the entire page instead of only log record portion
           * as the page has nothing of value currently
           */
	  bzero(bufferData(bP), ldP->blkSize);

	  /*
	   * fill in the variable fields of the newHdr structure.
	   */
	  newHdr.pageNumber = newHdr.oldestDataPage = newHdr.oldestTranPage =
	      index;
	  newHdr.timeWritten = osi_Time();

	  *(struct logPageHdr *)bufferData(bP) = newHdr;

	  /* write back synchronously and delete the buffer */
	  bufferSetDirty(bP);
	  buffer_Release(bP, elbb_sdel);
      }

      newHdr.lastLogRecOff = elbr_FirstLogRecordOffset(&newHdr);

      /* write a clean restart record */
      logbuf_DBprint(("elbl_Create: initializing log"));
      elbr_InitLogRecHdr( (&(restart.hdr)) );
      restart.hdr.recType = LOGREC_RSTRT;
      restart.oldData = (caddr_t)0;
      restart.newData = (caddr_t)0;
      restart.lsn = 0;

      code = elbr_WriteRestartBlock(ldP, *logLocnArray, &restart,
				    LOGREC_INITIAL_LOGPAGE_PASSNO,
				    cksumVer);
      afsl_MBZ(code);

      /** done */
      return BUFFER_OK;
}

static int LogTooFull (struct logMap *lmP, u_int thisBlocks)
{
    int i;
    /*
     * If the log is too full and various attempts to handle the problem
     * ourselves are unsuccessful then return LOGFULL error.
     *
     * The log full computation here should mirror that in IsLogFull - this
     * one should NEVER be more generous than IsLogFull!  This condition is
     * met because thisBlocks is never negative.
     */
    /* Keep checking, trying a little harder each time. */

    for (i=1; i<=3; i++) {
	struct elbt_TranRec *tP;

	ComputeActiveLog(lmP);
	if ((lmP->activeSize + LOG_FULL_BUFFER + thisBlocks) <=
	    lmP->logLocnArraySize) {
	    /* there is space in the log */
	    return 0;
	}

	tP = FindOldestTran(lmP);
	if (COMPUTE_TRAN_ACTIVE_LOG_SIZE(lmP, tP) < lmP->activeSize) {
	    /* There is no space but the tail is pinned by dirty buffer so
             * there is nothing we can do at this level. */
	    return 1;
	}

	switch (i) {
	  case 1:			/* await already scheduled I/O */
	    WaitForAllLogPages (lmP); break;
	  case 2:			/* force current page & await */
	    ForceAllLogPages (lmP); break;
	  case 3:			/* can't oblige... */
	    /* can't fix logfull */
	    return 1;
	}
    }
    /* not reached */
    return 1;
}

/*
 * elbl_Write - called by an entity that wishes to only write out to the
 * log, but does not need to associate to a disk block (that type of
 * caller should use elbl_WriteWithBuf).
 *
 * locks: transaction should be locked; acquires map lock.
 */
/* SHARED */
long elbl_Write(
  IN struct elbt_TranRec *tP,
  IN unsigned recType,
  IN u_int32 dataLength,
  IN const caddr_t data)
{
      long retval;
      struct logMap *lmP;
      unsigned newRecOffset;
      struct logRec *prevRecord;
      u_long logFullState = 0;

      lmP = (struct logMap *)tP->mapP;
      afsl_Assert (lmP);
      map_lock(lmP);
      afsl_Assert (lmP->current);
      afsl_Assert (lmP->current->bufType == elbb_bufLog);
      afsl_Assert (dataLength <= tP->dev->blkSize);
      GetNewRecordOffset(lmP, newRecOffset, prevRecord);
      retval = WriteLocked (tP, recType, dataLength, 0, 0,
			    (const caddr_t)data, (const caddr_t)data,
			    newRecOffset, &logFullState);

      map_unlock(lmP);
      return retval;
}

/* SHARED */
long elbl_WriteExtended(tP, recType, dataLength, diskBlock, dataOffset,
			       oldData, newData, logFullState)
  IN struct elbt_TranRec *tP;
  IN unsigned recType;
  IN u_int32 dataLength;
  IN daddr_t diskBlock;
  IN u_int32 dataOffset;
  IN const caddr_t oldData;
  IN const caddr_t newData;
  OUT u_long *logFullState;
{
      long retval;
      struct logMap *lmP;
      unsigned newRecOffset;
      struct logRec *prevRecord;

      /* clear logFullState */

      *logFullState=0;

      lmP = (struct logMap *)tP->mapP;
      afsl_Assert (lmP);
      map_lock(lmP);
      afsl_Assert (lmP->current);
      afsl_Assert (lmP->current->bufType == elbb_bufLog);
      afsl_Assert (dataLength <= tP->dev->blkSize);
      GetNewRecordOffset(lmP, newRecOffset, prevRecord);
      retval = WriteLocked (tP, recType, dataLength, diskBlock, dataOffset,
			    oldData, newData, newRecOffset, logFullState);
      map_unlock(lmP);
      return retval;
}

/*
 * elbl_WriteWithBuffer: handles logging of updates.
 *
 */
/* SHARED */
long elbl_WriteWithBuffer(
     IN struct elbt_TranRec *tP,
     IN unsigned recType,
     IN u_int32 dataLength,
     IN u_int32 dataOffset,
     IN struct buffer *bP,
     IN const caddr_t newData,
     OUT u_long *logFullState)
{
      long retval;
      struct logMap *lmP;
      unsigned newRecOffset;
      struct logRec *newRecord, *prevRecord;
      unsigned thisBlocks;		/* additional blocks we require */
      /* these are the old parameters we get from the buffer */
      daddr_t diskBlock = bP->blockNum;
      caddr_t oldData;

      afsl_Assert(tP && logFullState && tP->mapP);
      *logFullState = 0; /* always reset - will be set if necessary */

      if (LOGREC_ISFILL(recType))	/* source char is always newData */
	  oldData = newData;
      else
	  oldData = &bP->data[dataOffset];

      lmP = (struct logMap *)tP->mapP;
      map_lock(lmP);
      afsl_Assert (lmP->current && (lmP->current->bufType == elbb_bufLog) &&
		   (dataLength <= tP->dev->blkSize));

      /*
       * We call WriteLocked to handle the "hard" cases - big page writes,
       * handling end-of-page conditions, etc.  The first thing we do is
       * find the offset into the current page.  Then we check to see if there
       * is even enough room to write the new log record.  Finally we check
       * to see if after we write the log record the page will be full.  If
       * either of these two is true, we call the "real" call.  Otherwise we
       * just stick the new log record into the page and run!
       */
      GetNewRecordOffset(lmP, newRecOffset, prevRecord);
      {
	  unsigned thisSize;		/* size of this record */
	  unsigned minSize;		/* size of smallest record */
	  unsigned remaining;		/* unused bytes in current page */

	  thisSize = NewLogrecSize(recType, dataLength);
	  minSize = NewLogrecSize(LOGREC_DATA, ALIGN_SIZE);
	  remaining = LogPageRemaining(lmP, newRecOffset);
	  if (remaining >= thisSize + minSize)
	  thisBlocks = 0;
	  else if (remaining >= thisSize)
	      /* will need a new page but this record will fit. */
	      thisBlocks = 1;
	  else
	      /* This calculate is somewhat approximate as it doesn't take
               * extra log page or log record headers into account. */
	      thisBlocks = 1 + BytesToLogBlocks (lmP, thisSize - remaining);
      }
      if (LogTooFull (lmP, thisBlocks)) {
	  map_unlock(lmP);
	  return LOG_LOGFULL;
      }	  

      /* Now just write the record out, using appropriate technology. */

      if (thisBlocks > 0) {
	  /*
	   * The "hard" case: use full strength version.
	   *
	   * Note that we start aging the buffer AFTER we are sure we will
           * be writing this log update, but BEFORE we've actually
           * (potentially) changed the log page number.
           */
	  AgeBuffer(bP, lmP);
	  retval = WriteLocked (tP, recType, dataLength, diskBlock,
				dataOffset, oldData, (const caddr_t)newData,
				newRecOffset, logFullState);
      } else {

	  /* The "easy" case: record fits on this page.  Start aging on the
           * passed in buffer and append the record. */

	  AgeBuffer(bP, lmP);
	  WriteLogRecord(newRecord, newRecOffset, lmP, recType, tP,
			 dataLength, diskBlock, dataOffset, oldData, newData);
	  UpdateTran(tP, lmP->passno, lmP->currentLogBlock);
	  GetNewRecordOffset(lmP, newRecOffset, prevRecord);
	  afsl_Assert(!IsLogPageFull(lmP, newRecOffset));
	  retval = 0;
      }

      /*
       * We must label the buffer so we know how far to force the log before we
       * can write this dirty buffer to the disk.  We use the trans label since
       * that is updated with each log record and a new log page might have
       * been started if the last record just filled the previous page.  Assert
       * that calling ForceMap with these parameters will have some effect.
       */
      if (!retval) {
	  bP->bufLogLastPass = tP->lastPass;
	  bP->bufLogLastOff = tP->lastOffset;
	  afsl_Assert (!((bP->bufLogLastPass == lmP->passno) &&
			 (bP->bufLogLastOff == lmP->currentLogBlock) &&
			 (lmP->pageCache->lastLogRecOff == 0)));
      }

      map_unlock(lmP);

      return retval;
}

/*
 * WriteLocked - actually performs log write call.
 *
 * This routine assumes it is called with the transaction already "looked up",
 * the locks held, and all it need do is the hard work.  By keeping the
 * locking outside this routine we allow the recursive approach it relies
 * upon.
 *
 * LOCKS: map lock.
 */
static long WriteLocked(
     IN struct elbt_TranRec *tP,
     IN unsigned recType,
     IN u_int32 dataLength,
     IN daddr_t diskBlock,
     IN u_int32 dataOffset,
     IN caddr_t oldData,
     IN const caddr_t newData,
     IN unsigned newRecOffset,
     OUT u_long *logFullState)
{
    struct logRec *newRecord, *prevRecord;
    unsigned remaining;
    unsigned newRecSize;
    struct logMap *lmP = (struct logMap *)tP->mapP;
    long code;
    caddr_t nData = newData;

    map_assertlocked(lmP);
    logbuf_DBprint(("WriteLocked: entered"));

    /*
     * New algorithm for writing out log records:
     *
     * 1. look up log information based upon the tran ID.  Check all
     *    data to make sure it is OK.
     *
     * 2. Read in the current log page's header.
     *
     * 3. Using the current log page header, read the last record
     *    written to the log page.
     *
     * 4. Compute available space on the page.
     *
     * 5. (a) If there is enough space, write out the record.
     *    (b) If there is not enough space, fill out the page,
     *        schedule it for write and recursively call this routine
     *        with remaining data.
     *
     * 6. Check to see if the page is full enough to push out.  If so, do
     *    it.
     *
     * 7. Return to caller.
     *
     */

    /* The # of bytes remaining is the total on the page - newRecOffset.  */
    afsl_Assert(newRecOffset < lmP->current->adev->blkSize);

    /*
     * The # of bytes needed for the new record is sizeof(log part)
     * + 2 * size of data (once for old, once for new.)
     */
    newRecSize = NewLogrecSize(recType, dataLength);
    remaining = LogPageRemaining(lmP, newRecOffset);

    if (newRecSize > remaining) {
	/*
	 * If this log record is a "splittable" type, we'll split it.  If
	 * it isn't, we'll force the current page out and call this routine
	 * recursively.
	 */
	if (LOGREC_NOSPLIT(recType)) {
	    /*
	     * We must force this page out, re-compute the base parameters
	     * and call WriteLocked again.
	     */
	    NewLogPage(lmP);
	    GetNewRecordOffset(lmP, newRecOffset, prevRecord);
	    code = WriteLocked(tP, recType, dataLength, diskBlock,
			       dataOffset,oldData, (const caddr_t)nData,
			       newRecOffset, logFullState);
	    afsl_MBZ(code);
	    return BUFFER_OK;
	}

	/*
	 * DB 3182: don't use recursion, iterate.  Otherwise the stack
	 * can get very large when a 1K file system is called with an 8K
	 * ACL (for instance).
	 */
	while (newRecSize > remaining) {
	    /*
	     * We don't have enough space.  We first check to
	     * see if we have enough space to write all the
	     * data out; if not, we won't do anything.
	     * If we do, we call WriteLocked recursively:
	     *
	     *        (a) We call it with enough data to fill up the
	     *            current page.
	     *        (b) We call it with the remaining data.
	     *
	     * DB 3182: compute this correctly. (wam 930219)
	     *
	     * (1) how much space is there left for data
	     * (2) we know there is data (otherwise it would have fit and
	     *     we wouldn't be in this path) so we figure out if it is
	     *     both old and new data, in which case we divide by two.
	     *     Otherwise it just has old data or new data and we ignore
	     *     it.
	     */
	    newRecSize = remaining - NewLogrecSize(LOGREC_DATA,0);

	    if (LOGREC_HASOLDDATA(recType) && LOGREC_HASNEWDATA(recType))
		newRecSize /= 2;

	    code = WriteLocked(tP, recType, newRecSize, diskBlock,
			       dataOffset,oldData, (const caddr_t)nData,
			       newRecOffset, logFullState);
	    afsl_MBZ(code);

	    /*
	     * Now, recompute the state and try again; note that this
	     * left-recursive approach never lets the stack go more than
	     * two-deep (this call, plus the next call we are about to
	     * make.)
	     */
	    dataLength -= newRecSize;
	    dataOffset += newRecSize;
	    oldData += newRecSize;
	    nData += newRecSize;

	    GetNewRecordOffset(lmP, newRecOffset, prevRecord);
	    newRecSize = NewLogrecSize(recType, dataLength);
	    remaining = LogPageRemaining(lmP, newRecOffset);
	}

	code = WriteLocked(tP, recType, dataLength, diskBlock, dataOffset,
			   oldData, (const caddr_t)nData, newRecOffset,
			   logFullState);

	afsl_MBZ(code);
	return BUFFER_OK;
    }

    /*
     * We have enough space.  We build the new record and update the
     * transaction record.
     */

    WriteLogRecord(newRecord, newRecOffset, lmP, recType, tP, dataLength,
		   diskBlock, dataOffset, oldData, nData);
    UpdateTran(tP, lmP->passno, lmP->currentLogBlock);
    afsl_Assert(lmP->pageCache->lastLogRecOff <=
		    (unsigned)lmP->current->adev->blkSize);
    /*
     *  If we cannot write at least a log record plus
     *  twice the alignment requirement of log
     *  data, we consider the page full.
     *
     */
    GetNewRecordOffset(lmP, newRecOffset, prevRecord);
    if (IsLogPageFull(lmP, newRecOffset)) {
	/* flush this log page to disk and get a new page */

	NewLogPage(lmP);

	/*
	 * now we don't care - just set the status to be correct
	 */

	if (IsLogGettingFull(lmP)) {
	    *logFullState = 1;
	}
    }
    return BUFFER_OK;
}

/*
 * elbl_FullInfo - provide information about log full conditions.
 *
 * this routine is called by the buffer package when a log full condition
 * is signaled; it returns buffers from the age queue which should be
 * written; once enough buffers have been written, this routine will
 * return null.
 *
 * The force flag is used when every buffer is to be returned, not just those
 * without the write pending bits set.
 */
/* EXPORT */
void elbl_FullInfo(dP, force, ageQSizeP, bPP)
     IN struct async_device *dP;
     IN unsigned force;
     OUT u_long *ageQSizeP;
     OUT struct buffer **bPP;
{
      struct logMap *lmP;
      struct elbt_TranRec *tP;
      struct buffer *bP;
      u_int16 tranOffset, tranPass;

      GetLogMap(dP, lmP);
      afsl_Assert(lmP);

      if (ageQSizeP)
	  *ageQSizeP = lmP->bufferAgeQ.length;

      /*
       * we unpin as far as the transaction system will currently allow -
       * beyond that there isn't any reason to force buffer I/O.
       */

      tP = FindOldestTran(lmP);

      if (tP) {
	  tranOffset = tP->firstOffset;
	  tranPass = tP->firstPass;
      } else {
	  tranOffset = lmP->currentLogBlock;
	  tranPass = lmP->passno;
      }

      /*
       * now we look for the oldest buffer.
       */
      bP = FindOldestBuffer(lmP);

      if (!force) {
	  while(bP) {
	      /*
	       * If this is an unlogged buffer, it must be an Undo-zero
	       * operation.  The buffer package controls these so we ignore
	       * them in figuring out which buffers we should schedule, since
	       * if such buffers get scheduled they can pull up the tail
	       * of the log.
	       */
	      if (bufferIsDirty(bP) && !bufferIsWritep(bP) &&
		  bufferIsLogged(bP)) {
		  break; /* from while loop */
	      }
	      bP = (struct buffer *)FIFO_NextOldest(&(lmP)->bufferAgeQ,
						    bP, &bP->ageQ);
	  }
      }

      /* If the oldest tran is "ahead of" the oldest buffer then return the
       * buffer else someone should try to cleanup the transaction instead. */

      if (bP &&
	  (ComparePosition (tranPass,tranOffset,
			    bP->bufLogFirstPass,bP->bufLogFirstOff) >= 0))
	  *bPP = bP;
      else *bPP = 0;

      map_unlock(lmP);
      return;
}

/*
 * elbl_Shutdown -- terminate logging on the specified device.
 *
 *     This call halts all logging on the specified device.  It assumes all
 *     transactions have completed.
 *
 * Locking: this routine acquires both the global log lock and individual
 * map locks.
 */
/* SHARED */
void elbl_Shutdown(IN struct async_device *dP)
{
    struct logMap *lmP;

    GetLogMap(dP, lmP);
    if (!lmP)
	return;

    /* disallow simultaneous callers */
    afsl_Assert(lmP->logStatus == LOG_LOGMAP_STAT_OK);
    afsl_Assert (!(lmP->current) || (lmP->current->bufType == elbb_bufLog));

    afsl_Assert(IsFifoEmpty(&lmP->tranAgeQ));
    /*
     * At this point we know there are no longer any running (active or
     * pending) transactions and that we can now force the log to the current
     * block.  At the end of that operation, we should have no transactions
     * ended but not committed.
     */
    ForceMap(lmP, (unsigned) lmP->currentLogBlock);
    /*
     * there shouldn't be any more log activity or completed transactions.
     */
    afsl_Assert(IsFifoEmpty(&lmP->inTransit));

    lmP->logStatus = LOG_LOGMAP_STAT_CLOSED;
    map_unlock(lmP);
    return;
}

/*
 * elbl_Close - close a log device.
 *
 * This routine is inclusive of elbl_Shutdown;  it however always blocks
 * pending the completion of any active transactions.  It then writes a
 * restart block into the log recovery area.
 *
 * This routine returns a fifo of the remaining transactions attached to
 * this log.  They should all be considered committed upon return from
 * this function (the log is clean, all meta-data is written to disk, etc.)
 *
 * XXX: should we, instead of writing this as the first log block, write
 * this at the current location?  This was suggested at the 3/91 Episode
 * seminar and it has some positive benefits. - wam 4/5/91
 */
/* SHARED */
void elbl_Close(
  IN struct async_device *dP,
  INOUT struct fifo *fP,
  IN void (*const deallocP)(struct buffer *, int))
{
      struct logMap *lmP;
      struct logRec ckpt, restart;
      unsigned passNum, pgNum;
      struct buffer *bP;

      elbl_Shutdown(dP);

      /* Before we grab the logmap lock but after forcing the log all
       * transactions should be committed.  We shouldn't have to ask GCTran to
       * force.  Move them to the output fifo for our caller. */
      elbl_GCTran (dP, 0/*!force*/, fP);

/*      GetLogMap(dP, lmP); */
      LOG_LOCK();
      LookupLogMap(dP, lmP);
      if (lmP) {
	  map_lock(lmP);
      } else {
	  LOG_UNLOCK();
	  return;
      }
      afsl_Assert(lmP->logStatus == LOG_LOGMAP_STAT_CLOSED);

      /* The GCTran call should have removed all transactions. */
      afsl_Assert(IsFifoEmpty(&lmP->cTranList));

      /*
       * We return all buffers used by this log to the buffer pool.
       * This includes those buffers on the free list, those on the
       * in-use list, and finally the current buffer.
       */

      while (bP = (struct buffer *)FIFO_LookAtOldest(&lmP->freeBuf,
				 offsetof(struct buffer, freeList))) {
	  FIFO_REMOVE(&lmP->freeBuf, &bP->freeList);
	  BUFFER_RELEASE(bP);
	  bP->bufFH = 0; /* we overloaded, so we must clear it */
	  bP->bufFHNext = (struct buffer *)0;
	  (*deallocP)(bP, 0); /* deallocate buffer */
      }

      if (lmP->current) {
          bP = lmP->current;
	  BUFFER_RELEASE(bP); /* done with buffer */
	  bP->bufFH = 0; /* we overloaded, so we must clear it */
	  bP->bufFHNext = (struct buffer *)0;
	  (*deallocP)(bP, 0); /* deallocate buffer */
	  lmP->current = (struct buffer *)0;
      }

      lmP->logStatus = LOG_LOGMAP_STAT_FREE;

      /* build the checkpoint record */
      elbr_InitLogRecHdr( &(ckpt.hdr) );
      ckpt.hdr.recType = LOGREC_CKPT;
      ckpt.hdr.tranId = (u_int32)elbt_NullTranId;
      ckpt.oldData = (caddr_t)0;
      ckpt.newData = (caddr_t)0;
      ckpt.lsn = lmP->lsn;

      passNum = lmP->passno;
      pgNum = lmP->currentLogBlock+1;
      if (pgNum >= lmP->logLocnArraySize) {
	  pgNum %= lmP->logLocnArraySize;
	  passNum = IncPassno(passNum);
      }

      /* write the checkpoint block */
      elbr_WriteCheckpointBlock(lmP->logDev, lmP->logLocnArray[pgNum],
				&ckpt, passNum, pgNum, lmP->cksum_ver);

      /* build the restart record */
      elbr_InitLogRecHdr( &(restart.hdr) );
      restart.hdr.recType = LOGREC_RSTRT;
      restart.hdr.tranId = (u_int32)elbt_NullTranId;
      restart.oldData = (caddr_t)0;
      restart.newData = (caddr_t)0;
      restart.lsn = lmP->lsn;

      /* write the restart block */
      elbr_WriteRestartBlock(lmP->logDev, *(lmP->logLocnArray),
			      &restart, lmP->passno, lmP->cksum_ver );

      DeallocLogMap(lmP); /* UNLOCKS lmP! */
      LOG_UNLOCK();
      return;
}

/*
 * ForceAllLogPages -- writes all the log pages, for a particular log map, to
 *     disk and waits for the I/O to complete.
 */
static void ForceAllLogPages(IN struct logMap *lmP)
{
      long code;

      map_assertlocked(lmP);

      code = ForceMap(lmP, (unsigned) lmP->currentLogBlock);
      afsl_MBZ(code);
}

/*
 * WaitForAllLogPages -- waits for all I/O to complete on the log pages
 *     presently in-transit.
 */
static void WaitForAllLogPages(IN struct logMap *lmP)
{
      struct buffer *bP;
      long code;

      map_assertlocked(lmP);

      /*
       * wait for all pending I/O.
       */
      while (!IsFifoEmpty(&lmP->inTransit)) {
	  /*
	   * wait for the event to occur.
	   */
	  bP = (struct buffer *)
	    FIFO_LookAtOldest(&lmP->inTransit, offsetof(struct buffer, ageQ));
	  code = asevent_WaitEvent(bP->aevent);
	  CheckIOError(code, bP->adev, bP->blockNum);

	  /*
	   * now recycle
	   */
	  RecycleLogBufs(lmP);
      }
}

static int enableShowTranThrottling = 0;

/*
 * elbl_StartTran -- add a transaction record to a log map.
 *
 *     This routine is called by the transaction manager to start a new
 *     transaction against the specified log.  This will fail if the log is too
 *     full.  Failure is indicated by tP->mapP being NULL upon return.
 *
 * RETURN CODES -- on success returns zero.  On failure it returns a boolean
 *     which is true if the log is full due to dirty buffers and false if it is
 *     full due to uncompleted transactions or both data and trans pin the same
 *     page. */

/* SHARED */
int elbl_StartTran(
  struct async_device *dP,
  struct elbt_TranRec *tP,
  unsigned force)
{
      struct logMap *lmP;

      GetLogMap(dP, lmP);
      afsl_Assert(lmP &&
		  (!lmP->current || (lmP->current->bufType == elbb_bufLog)) &&
		  (lmP->logStatus == LOG_LOGMAP_STAT_OK) &&
		  (!tP->nested == !force));

      /* Update decaying sum of number of running transactions. */

      lmP->avgActiveTrans = (lmP->avgActiveTrans - (lmP->avgActiveTrans>>4) +
			     lmP->aTranList.length);

      /* Figure reservation depending on whether forced (undo zero) or not.  In
       * the future we'll want to take this size as an input parameter, but for
       * now just use two different sizes here, regular and nested trans. */

      tP->reserved = (tP->nested ? lmP->maxUndoSize : lmP->maxTranSize);

      /*
       * If the force option is set we proceed regardless of the available
       * space in the log.  The caller is guaranteeing that the new
       * transaction size plus old transaction size will be less than the
       * maximum for a single transaction.  Future calculations, however,
       * will count this transaction, so additional activity is throttled.
       *
       * This is dangerous, but necessary to cascade UndoZero transactions with
       * the allocate transaction.  They are not logically merged, but
       * they need to be logically ordered. - wam 920805
       */
      if (!force && !osi_IsKernelPanic()) {
	  /*
           * Each running transaction is assumed to need MAX_TRAN_SIZE of log
           * space.  If there is enough to start a new transaction, fine.
           * Otherwise we force the caller to wait (or return, if !wait.)
           */
	  u_long spaceInUse;
	  spaceInUse = lmP->activeSize + LOG_FULL_BUFFER +
	      BytesToLogBlocks (lmP, lmP->reserved);

	  /*
	   * If there is enough space, proceed.  Because we assume that the log
	   * is large enough for at least one transaction, we also proceed if
	   * there are no transactions currently running.  Also call LogTooFull
	   * to see if there is at least room for the start record.
           *
           * See commentary in NewLogPage for a description of deratedLogSize.
           */
	  if ((((spaceInUse + BytesToLogBlocks(lmP, tP->reserved)) >
		lmP->deratedLogSize) && (lmP->reserved != 0)) ||
	      (LogTooFull (lmP, 0))) {

	      /* this is the failure case */

	      struct buffer *obP = FindOldestBuffer(lmP);
	      struct elbt_TranRec *otP = FindOldestTran(lmP);
	      unsigned dsize = COMPUTE_DATA_ACTIVE_LOG_SIZE(lmP,obP);
	      unsigned tsize = COMPUTE_TRAN_ACTIVE_LOG_SIZE(lmP,otP);

	      icl_Trace3 (elb_iclSet, TR_LOG_STARTTRAN_FULL,
			  ICL_TYPE_LONG, lmP->aTranList.length,
			  ICL_TYPE_LONG, dsize,
			  ICL_TYPE_LONG, tsize);

	      map_unlock(lmP);
	      tP->mapP = NULL;		/* indicate failure */
	      return (dsize > tsize ? 1 : 0); /* write dirty buffers */
	  }
      }

      FIFO_ADD(&lmP->aTranList, &tP->logMapThread);
      tP->atran = 1;
      tP->ptran = 0;
      tP->ctran = 0;
      FIFO_ADD(&lmP->tranAgeQ, &tP->logAgeThread);
      tP->ageq = 1;

      /* Make the reservation. */
      lmP->reserved += tP->reserved;

      /*
       * Must initialize log pkgs fields of the data structure.  Cannot
       * defer writing the pass & offset fields because they might
       * be referenced before we get around to setting them (used to
       * defer until elbl_Write was called).  This has the side-effect
       * of possibly increasing the size of the active log.
       */
      tP->lastPass = tP->firstPass = lmP->pageCache->passNumber;
      tP->lastOffset = tP->firstOffset = lmP->pageCache->pageNumber;
      tP->ldev = lmP->logDev; /* XXX: is this necessary? */
      tP->mapP = (opaque)lmP;
      map_unlock(lmP);
      return 0;
}

/*
 * elbl_PendTran -- change the status of a transation from active to pending.
 *
 * LOCKS USED -- the transaction must be locked.
 */
/* SHARED */
void elbl_PendTran(IN struct elbt_TranRec *tP)
{
    struct logMap *lmP;

    lmP = (struct logMap *)tP->mapP;
    afsl_Assert(lmP);
    map_lock(lmP);

    afsl_Assert (tP->atran && tP->ageq);

    FIFO_REMOVE(&lmP->aTranList, &tP->logMapThread);
    FIFO_ADD(&lmP->pTranList, &tP->logMapThread);
    tP->atran = 0;
    tP->ptran = 1;

    /* update reservation */
    lmP->reserved -= tP->reserved;
    tP->reserved = lmP->maxEndSize;
    lmP->reserved += tP->reserved;

    map_unlock(lmP);
}

/*
 * elbl_CompleteTran -- change the status of a transation from active or
 *     pending to complete.  We assume the end record has been written before
 *     this routine is called.  If the transaction is currently in pending
 *     state then the EC head tran must be provided so that we can properly
 *     update the last log position.
 *
 * LOCS USED -- the transaction and the ec head, if provided, must be locked.
 */
/* SHARED */
void elbl_CompleteTran(
  IN struct elbt_TranRec *tP,
  IN unsigned finalSize,		/* bytes in transaction */
  IN struct elbt_TranRec *ecP)
{
      struct logMap *lmP;

      lmP = (struct logMap *)tP->mapP;
      afsl_Assert(lmP);
      map_lock(lmP);

      afsl_Assert ((tP->atran != tP->ptran) && tP->ageq);

      if (ecP) {

	  /* Insist that ecP is on the same log, that it is either active or
           * pending, and that its most recent update (its END record) is at
           * least as new as tP's most recent update (its BEND record). */

	  afsl_Assert ((lmP == (struct logMap *)ecP->mapP) &&
		       (ecP->atran || ecP->ptran));
	  afsl_Assert (ComputeLogDistance (lmP, ecP->lastPass,ecP->lastOffset,
					   tP->lastPass,tP->lastOffset) >= 0);
	  UpdateTran (tP, ecP->lastPass, ecP->lastOffset);
      }

      if (tP->atran) {
	  FIFO_REMOVE(&lmP->aTranList, &tP->logMapThread);
	  tP->atran = 0;
      }
      if (tP->ptran) {
	  FIFO_REMOVE(&lmP->pTranList, &tP->logMapThread);
	  tP->ptran = 0;
      }

      FIFO_ADD(&lmP->cTranList, &tP->logMapThread);
      tP->ctran = 1;
      FIFO_REMOVE(&lmP->tranAgeQ, &tP->logAgeThread);
      tP->ageq = 0;

      lmP->reserved -= tP->reserved;
      tP->reserved = 0;

      if (lmP->tranSleepers) {
	  lmP->tranSleepers = 0;
	  epi_cond_broadcast(&lmP->cond);
      }

      /*
       * If we encounter a transaction which was larger than the reservation
       * for this aggregate, we increase future reservations.
       */
      if (lmP->maxTranSize < finalSize) {
	  printf("elbl_CompleteTran: increasing (%s) transaction reservation",
		 tP->opString);
	  printf(" from %dKb to %dKb\n",
		 (lmP->maxTranSize+1023)>>10, (finalSize+1023)>>10);
	  lmP->maxTranSize = finalSize;
	  /*
	   * There must be enough space to start a new transaction.  This means
	   * at least one more block must be available.
	   */
	  afsl_Assert((BytesToLogBlocks(lmP,lmP->maxTranSize) +
		       LOG_FULL_BUFFER) < lmP->logLocnArraySize);
      }

      /* recompute the active log size, since we've changed the ageQ */
      ComputeActiveLog(lmP);

      map_unlock(lmP);
}

/*
 * elbl_GCTran -- handle cleanup of committed transactions.
 *
 *     This routine is called by the transaction manager whenever it needs to
 *     reclaim transaction structures.  It creates a list of transactions which
 *     are no longer needed by the log manager - these transactions are already
 *     implicitly freed so the caller need not call elbl_DeleteTran.
 *
 *     If dP is NULL we GC all devices.
 *
 * PARAMETERS -- If force is specified, the some non-trivially expensive
 *     measures may be taken to reclaim at least some transactions.
 */
/* SHARED */
void elbl_GCTran(
  IN struct async_device *dP,
  IN int force,
  INOUT struct fifo *fP)
{
    struct async_device *nextDevice = dP;
    struct elbt_TranRec *tP;
    struct logMap *lmP;

    do {
	/* First get the right logMap.  If a device was specified use that.
         * Otherwise prepare to iterate over all logMaps.  To do this safely we
         * lookup the device of the next logMap and use that on the next
         * iteration. */

	LOG_LOCK();
	if (nextDevice)
	    LookupLogMap (nextDevice, lmP);
	else {
	    /* Start with first logMap if no device was specified. */
	    lmP = FIFO_LookAtOldest(&logMapActiveList,
				    offsetof(struct logMap, mapStatus));
	}
	if (lmP) {
	    struct logMap *nextMap;
	    map_lock(lmP);

	    /* Get the device of the next logMap. */
	    nextMap = FIFO_NextOldest(&logMapActiveList, lmP, &lmP->mapStatus);
	    if (nextMap)
		nextDevice = nextMap->dev;
	    else
		nextDevice = NULL;
	}
	LOG_UNLOCK();
	if (!lmP) {
	    /* we consider a nonexistent log to be garbage collected... */
	    return;
	}
	/*
         * If there are buffers on the in transit list.  We cheaply clean up
         * the in-transit list before we do too much checking.
         */
	if (!IsFifoEmpty(&lmP->inTransit)) {
	    RecycleLogBufs(lmP);
	}

	do {
	    tP = FindOldestCompleteTran(lmP);
	    if (!tP)
		break;			/* cTran list is empty */

	    while (tP) {
		struct elbt_TranRec *nextP;

		nextP = (struct elbt_TranRec *)
		    FIFO_NextOldest(&lmP->cTranList, tP, &tP->logMapThread);
		afsl_Assert (tP->ctran && !tP->ageq && tP->mapP);

		/*
		 * We can GC this transaction if this transaction is not in
		 * the write window of the log.  We do this by computing the
		 * distance it is from the log itself.
		 */
		if (TranCommitted (lmP, tP)) {
		    /* we GC this one! */
		    FIFO_REMOVE(&lmP->cTranList, &tP->logMapThread);
		    tP->ctran = 0;
		    tP->mapP = (opaque)0;
		    FIFO_ADD (fP, &tP->logMapThread);
		}
		tP = nextP;
	    }

	    /*
	     * Quit if we made progress or if we have not been asked to force.
	     * Otherwise, try harder and go around again.
	     */
	    if (!IsFifoEmpty(fP) || !force)
		break;

	    if (!IsFifoEmpty(&lmP->inTransit)) {
		/*
		 * Only wait for pending I/O
		 */
		WaitForAllLogPages (lmP);
	    } else {
		/*
		 * We try very hard now.
		 */
		ForceAllLogPages (lmP);
	    }
	} while (1);

	map_unlock(lmP);
    } while (!dP && nextDevice && IsFifoEmpty(fP));
}

/*
 * elbl_DeleteTran -- destroy the connection between a transaction and the
 *     specific log.
 *
 * locks: the transaction must be locked.
 */
/* SHARED */
void elbl_DeleteTran(struct elbt_TranRec *tP)
{
      struct logMap *lmP;

      lmP = (struct logMap *)tP->mapP;
      afsl_Assert(lmP);
      map_lock(lmP);

      afsl_Assert (tP->ctran && !tP->ageq);
      FIFO_REMOVE(&lmP->cTranList, &tP->logMapThread);
      tP->ctran = 0;
      tP->mapP = (opaque)0;

      map_unlock(lmP);
}

/*
 * elbl_WriteBend - write a batch end record.
 *
 * locks: the transactions must be locked.  It locks the log map.
 *
 * TODO -- This should take the service name of the transaction and write it
 *     out into the BEND record just like in end records.
 */
/* SHARED */
void elbl_WriteBend(struct elbt_TranRec *t1P, struct elbt_TranRec *t2P)
{
      char tmpBuf[sizeof(struct logRec)];
      struct logRec *tempLogRec = (struct logRec *)tmpBuf;
      struct logRec *prevRecord;
      struct logMap *lmP;
      unsigned newRecOffset;
      long code;
      u_long logFullState;

      lmP = (struct logMap *)t1P->mapP;
      afsl_Assert(lmP != 0);

      map_lock(lmP);
      tempLogRec->hdr.recType = (u_int16)LOGREC_END;
      tempLogRec->hdr.refCount = ++(t2P->refCount);
      tempLogRec->hdr.dataLength = 0;
      tempLogRec->hdr.tranId = t2P->tranId;
      tempLogRec->hdr.dataAddr = (daddr_t)0;
      tempLogRec->hdr.dataOffset = 0;
      tempLogRec->hdr.prevLogRecOff = lmP->pageCache->lastLogRecOff;
      *(u_int32 *)((char *)tempLogRec + LSNOffset(tempLogRec)) =
	  ((lmP->lsn+1) ? lmP->lsn+1 : 1);

      GetNewRecordOffset(lmP, newRecOffset, prevRecord);
      code = WriteLocked(t1P,LOGREC_BEND, NewLogrecSize(LOGREC_END,0), 0,
			 0, tmpBuf, 0, newRecOffset, &logFullState);
      afsl_MBZ(code);

      map_unlock(lmP);
}

/*
 * elbl_ForceTran -- force the log to disk up to the specified transaction's
 *     last update.
 *
 *     This routine is a wrapper for the ForceMap call.  It is called by the
 *     higher-level (which knows nothing about log maps.)
 */
/* SHARED */
void elbl_ForceTran(IN struct elbt_TranRec *tP)
{
    struct logMap *lmP;
    long code;

    lmP = (struct logMap *)tP->mapP;
    if (!lmP)
	return;				/* out of our hands */
    map_lock(lmP);

    if (TranCommitted(lmP, tP)) {
	/* no need to force at all, tran is already committed. */
	map_unlock(lmP);
	return;
    }

    code = ForceMap(lmP, (unsigned)tP->lastOffset);
    afsl_MBZ(code);

    map_unlock(lmP);
    return;
}

/* elbl_BufferCommitCost -- returns a measure of the cost of forcing the log
 *     before the buffer can be written.
 *
 * RETURN CODES -- returns
 *     zero -- if the buffer can be written immediately,
 *     one -- if only inTransit log pages need to be awaited, and
 *     two or more -- if a partial page written.  We can base this on how full
 *         the page is.  We compute the cost using a rather arbitrarily method
 *         that strongly discourages mostly empty pages. */

int elbl_BufferCommitCost (IN struct buffer *bP)
{
    struct logMap *lmP;
    int cost;

    if (!bP->currentAgeQ)
	/* log forcing cost is zero for clean or unlogged buffers. */
	return 0;

    GetLogMap(bP->adev, lmP);
    afsl_Assert(lmP && (bP->currentAgeQ == &lmP->bufferAgeQ));

    if (BufCommitted (lmP, bP)) {
	cost = 0;
    } else if (lmP->currentLogBlock != (unsigned)bP->bufLogLastOff) {
	cost = 1;			/* only have to wait for inTransit */
    } else {

        /* Decide how many eighths (1/8) of the current page are empty.  This
         * number is squared to increase the cost of mostly empty pages.  Then
         * we return two plus this number.  Thus:
         *     nearly empty page => cost is 2+8^2,
         *     nearly full page => cost is 2+0. */

	int eighthsEmpty =
	    8 - (lmP->pageCache->lastLogRecOff >> (lmP->logDev->logBlkSize-3));
	cost = 2 + eighthsEmpty*eighthsEmpty;
    }

    map_unlock(lmP);
    return cost;
}
/*
 * elbl_ForceBuffer -- force the log to disk up to the specified buffer's
 *     last update.
 *
 *     This routine is a wrapper for the ForceMap call.  It is called by the
 *     higher-level (which knows nothing about log maps.)
 */
/* SHARED */
void elbl_ForceBuffer(IN struct buffer *bP)
{
    struct logMap *lmP;
    long code;

    GetLogMap(bP->adev, lmP);
    afsl_Assert(lmP);

    code = ForceMap(lmP, (unsigned)bP->bufLogLastOff);
    afsl_MBZ(code);
    afsl_Assert (BufCommitted (lmP, bP));

    map_unlock(lmP);
    return;
}

/*
 * ForceMap -- force the log associated with a map.
 *
 * This routine handles the forcing of a log device.  It expects the log map
 * to be held.
 */
static long ForceMap(
     struct logMap *lmP,
     unsigned pageOffset)
{
      struct buffer *bP;
      unsigned found;
      long code;

      afsl_Assert(lmP);
      map_assertlocked(lmP);
      afsl_Assert(lmP->current);
      afsl_Assert(lmP->current->bufType == elbb_bufLog);
      afsl_Assert(pageOffset < lmP->logLocnArraySize);
      /* caller should not be holding the log lock */

      logbuf_DBprint(("ForceMap: force log to page %d", pageOffset));

      /*
       * If the block # we must force to is the current block #
       * and that page isn't empty, we force the I/O.  We are assuming
       * we can *always* allocate another page - note the value of
       * LOG_FULL_BUFFER.
       */

      if ((lmP->currentLogBlock == pageOffset) &&
	  (lmP->pageCache->lastLogRecOff != 0)) {
	  NewLogPage(lmP);
      }

      /*
       * Now, we wait until all pending I/O of blockNum or before has
       * completed.  This is a slight optimization of the work done by
       * WaitForAllLogPages.
       */

      found = 0;
      lmP->eventSet = asevent_CreateSet();
      bP = (struct buffer *)
	FIFO_LookAtOldest(&lmP->inTransit, offsetof(struct buffer, ageQ));
      while (bP) {
	  /*
	   * we must wait for all in-transit buffers older than the one to
	   * which we are forcing.  We don't assume in-order writes.  We can
	   * use the knowledge that the "window" is no greater than the total
	   * # of buffers which could be simultaneously in transit.
	   *
	   * We use the knowledge that we are looking at a FIFO to figure out
	   * which buffers we need.  We wait for any page at least as old as
	   * the specified one.
           */
	  if (ComputeLogDistance (lmP, EarlierPassno(lmP,bP->bufFH),bP->bufFH,
				  EarlierPassno(lmP,pageOffset),pageOffset)
	      > 0) {
	      /* This buf holds a newer page than the desired one so we don't
               * need to await this or any newer ones. */
	      break;
	  }
	  found = 1;
	  asevent_AddToSet(lmP->eventSet, bP->aevent);
	  bP = (struct buffer *) FIFO_NextOldest(&lmP->inTransit, bP,
						 &bP->ageQ);
      }
      if (found) {
	  code = 0;
	  code = asevent_WaitAll(lmP->eventSet);
	  CheckIOError(code, lmP->logDev, lmP->current->blockNum);
      }
      asevent_ReleaseSet(lmP->eventSet);
      RecycleLogBufs(lmP);

      /* Insist that a transaction at this offset would show as committed (or
       * that the requested page is active and empty). */
      afsl_Assert (((pageOffset == lmP->currentLogBlock) &&
		    (pageOffset == lmP->activeLogBlock) &&
		    (lmP->pageCache->lastLogRecOff == 0)) ||
		   ComparePosition (EarlierPassno(lmP,pageOffset), pageOffset,
				    EarlierPassno(lmP,lmP->activeLogBlock),
				    lmP->activeLogBlock) < 0);
      return BUFFER_OK;
}

/*
 * elbl_GetLogPageOff - find current log page offset & pass #.
 *
 */
void elbl_GetLogPageOff(
  IN struct async_device *dP,
  OUT u_int16 *pass,
  OUT u_int16 *offset)
{
      struct logMap *lmP;

      GetLogMap(dP, lmP);
      if (lmP == NULL) {
	  /* handle no log case gracefully by returning invalid passno */
	  *pass = 0;
	  return;
      }

      /*
       * we handle the case of a clean log page - we don't depend on that
       * page - we depend upon the page before it!
       */
      if (lmP->pageCache->lastLogRecOff) {
	  *pass = (u_int16)lmP->passno;
	  *offset = (u_int16)lmP->currentLogBlock;
      } else {
	  if (!lmP->currentLogBlock)  {
	      /*
	       *  we handle the "backwards" rolling case.
	       */
	      *pass = (u_int16)DecPassno(lmP->passno);
	      *offset = (u_int16)lmP->logLocnArraySize - 1;
	  } else {
	      *pass = (u_int16)lmP->passno;
	      *offset = (u_int16)lmP->currentLogBlock-1;
	  }
      }
      map_unlock(lmP);
}

/*
 * elbl_IsTranCommitted -- check to see if the last record of this transaction
 *     has committed to disk.  It returns 1 if the transaction is committed
 *     (last log page touched on disk).  This routine is not expected to take a
 *     "long" time.
 */
/* SHARED */
int elbl_IsTranCommitted(IN struct elbt_TranRec *tP)
{
    struct logMap *lmP;
    int code;

    lmP = (struct logMap *)tP->mapP;
    if (!lmP) {
	/*
	 * a transaction not threaded on a log must be committed.
	 */
	return 1;
    }
    map_lock(lmP);

    if (!IsFifoEmpty(&lmP->inTransit)) {
	/*
         * If there are buffers on the in transit list.  We cheaply clean up
         * the in-transit list before we do too much checking.
         */
	RecycleLogBufs(lmP);
    }

    code = TranCommitted (lmP, tP);

    map_unlock(lmP);
    return code;
}

/*
 * code local to this module
 */
static unsigned long logMapHash(struct logMap *lmP)
{
      return ((unsigned)(lmP->dev));
}

/*
 * InitLogMap - initialize the log map system.
 *
 * this routine must be be called holding the log lock.  It initializes
 * the hashing system.  This call should be called at most once.
 */
static void InitLogMap(void)
{
      static int inited;
      struct logMap lmP;
      unsigned offset;
      long code;

      afsl_Assert(!inited);
      code = dfsh_HashInit (&logMapHashTable, logMapHash, 0);
      afsl_MBZ(code);
      offset = (unsigned)(((char *)(&lmP.dev))-((char *)(&lmP)));
      code = dfsh_HashInitKeyFixed (&logMapHashTable, offset, 4);
      afsl_MBZ(code);

      FIFO_INIT(&logMapFreeList, offsetof(struct logMap, mapStatus));
      FIFO_INIT(&logMapActiveList, offsetof(struct logMap, mapStatus));
      inited++;
}

/*
 * AllocLogMap - allocate a log map entry
 *
 * This routine must be called holding the log lock.  The returned entry
 * is marked "FREE" but must be marked "OK" before the caller drops the
 * actual lock.
 */
static void AllocLogMap(
     struct logMap **lmPP,  /* allocated log record */
     struct async_device *dP)   /* new device being logged */
{
      struct logMap *lmP;
      static mapsAllocated = 0;


      afsl_Assert(LOG_IS_LOCKED());

      if (IsFifoEmpty(&logMapFreeList)) {
	  if (mapsAllocated < elbl_MaxLogs) {
	      lmP = osi_Alloc_r(sizeof(struct logMap));
	      mapsAllocated++;
	  } else {
	      /* cannot allocate more maps */
	      *lmPP = (struct logMap *)0;
	  }

      } else {
	  lmP = (struct logMap *)FIFO_LookAtOldest(
		&logMapFreeList, offsetof(struct logMap, mapStatus));
	  FIFO_REMOVE(&logMapFreeList, &lmP->mapStatus);
      }

      /*
       * initialize the log map entry
       */
      lmP->next = (struct logMap *)0;
      lmP->logDev = (struct async_device *)0;
      lmP->dev = dP;
      /*
       * we initialize the fifos.  The free list is used to track
       * the buffers allocated to this log which are available for use.
       * The aTranList is the list of active transactions on this log.
       * The pTranList is the list of completed transactions on this log.
       * The cTranList is the list of pending transactions on this log.
       * The mapStatus thread is used to keep track of this log map entry
       */
      FIFO_INIT(&lmP->freeBuf, offsetof(struct buffer, freeList));
      FIFO_INIT(&lmP->aTranList, offsetof(struct elbt_TranRec, logMapThread));
      FIFO_INIT(&lmP->pTranList, offsetof(struct elbt_TranRec, logMapThread));
      FIFO_INIT(&lmP->cTranList, offsetof(struct elbt_TranRec, logMapThread));

      /*
       * thread on active log map list
       */
      FIFO_INITTHREAD(&lmP->mapStatus);
      FIFO_ADD(&logMapActiveList, &lmP->mapStatus);

      /*
       * standard initialization
       */
      lmP->current = (struct buffer *)0;
      lmP->logLocnArray = (daddr_t *)0;
      lmP->logLocnArraySize = 0;
      lmP->currentLogBlock = 0;
      lmP->activeLogBlock = 0;
      lmP->passno = 0;
      lmP->lsn = 1;
      lmP->logStatus = LOG_LOGMAP_STAT_FREE; /* XXX: not necessary anymore? */
      lmP->currentBuf = 0;
      lmP->eventSet = NULL;
      lmP->pageCache = (struct logPageHdr *)0;
      lmP->gcstatus = 0;
      lmP->tranSleepers = 0;

      lmP->maxTranSize =
	  elbl_MinTranReservation(dP->blkSize) << dP->logBlkSize;
      lmP->maxUndoSize = 512;		/* 20 undo zero records... */
      lmP->maxEndSize = 64;

      epi_mutex_init(&lmP->mapLock, "log map");
      epi_cond_init(&lmP->cond, "log map");

      FIFO_INIT(&lmP->bufferAgeQ, offsetof(struct buffer, ageQ));
      /*
       * A transaction always stays on this queue, from the moment it is
       * created.  This keeps it in strict-age order.
       */
      FIFO_INIT(&lmP->tranAgeQ, offsetof(struct elbt_TranRec, logAgeThread));

      lmP->sumUtil = lmP->avgUtil = lmP->nSamples = lmP->deratedLogSize = 0;
      lmP->avgActiveTrans = 0;

      *lmPP = lmP;
      return;
}

/*
 * DeallocLogMap - deallocate a log map entry
 *
 * Locking is NOT normal for this call - it must be called with
 * the log map entry locked;  the lock is NOT held upon return!
 * The global log lock also needs to be held as the logmap active and
 * free lists are modified.
 */
static void DeallocLogMap(struct logMap *lmP)
{

    long code;

    /* Assert the proper locks are held */
    afsl_Assert(LOG_IS_LOCKED());
    map_assertlocked(lmP);

    afsl_Assert(IsFifoEmpty(&lmP->inTransit));
    /* remove from hash table */
    code = dfsh_HashOut_r(&logMapHashTable, lmP);
    afsl_MBZ(code);

    /* Free up the array containing the disk addresses for the log blocks */
    osi_Free_r(lmP->logLocnArray, lmP->logLocnArraySize * sizeof(daddr_t));
    /* To be paranoid reset the log size field */
    lmP->logLocnArraySize=0;

    lmP->dev = (struct async_device *)0;
    lmP->logStatus = LOG_LOGMAP_STAT_FREE;

    map_unlock(lmP);
    epi_mutex_destroy(&lmP->mapLock);
    epi_cond_destroy(&lmP->cond);

    FIFO_REMOVE(&logMapActiveList, &lmP->mapStatus);
    FIFO_ADD(&logMapFreeList, &lmP->mapStatus);
}

/*
 * InitNewPage - initialize a new log page
 *
 */
static void InitNewPage(
     IN struct logMap *lmP,
     IN struct buffer *bP,
     IN unsigned pageNum)
{
      caddr_t newDataP;

      map_assertlocked(lmP);

      afsl_Assert(bP);
      newDataP = bP->data;

      /* deal with the boundary condition */
      lmP->pageCache = (struct logPageHdr *)newDataP;
      elbr_InitLogPageHdr(lmP->pageCache, lmP->cksum_ver);
      lmP->pageCache->passNumber = lmP->passno;
      lmP->pageCache->pageNumber = pageNum;
      lmP->pageCache->io_window = (u_int16) lmP->numBufs;
}

/*
 * UnpackLogRec - unpacks a log record
 *
 * really only used when a log is started.
 */
static long UnpackLogRec(struct logRec *rec, caddr_t buffer)
{
      rec->hdr = *(struct logRecHdr *)buffer;
      if ( ! LOGREC_ISVALIDTYPE(rec->hdr.recType))
	  return BUF_BADBUF; /* invalid buffer */

      if ((rec)->hdr.dataLength) {
	  (rec)->oldData = buffer + sizeof(struct logRecHdr);
	  (rec)->newData = (rec)->oldData + (rec)->hdr.dataLength;
      } else {
	  (rec)->oldData =  (rec)->newData = (caddr_t)0;
      }
      buffer += sizeof(struct logRecHdr) + 2 * (rec)->hdr.dataLength;
      LOGCOPY_ALIGNED(buffer, (caddr_t)&(rec->lsn), sizeof(u_int32));

      return BUFFER_OK;
}

/*
 * EnsureFreeBuffer - guarantee the existence of a new log buffer.
 *
 * This routine is used when a particular log map needs another buffer.
 *
 */
static void EnsureFreeBuffer(IN struct logMap *lmP)
{
    map_assertlocked(lmP);
    while (IsFifoEmpty(&lmP->freeBuf)) {
	long code = RecycleLogBufs(lmP);
	map_assertlocked(lmP);
	if (code == LOG_NOFREEBUFS) {
	    /* RecycleLogBufs will move from inuse to free any buffers whose
             * I/O is finished.  So wait for any buffer to complete its I/O. */
	    struct buffer *bP;
	    bP = (struct buffer *)FIFO_LookAtOldest(
		&lmP->inTransit, offsetof(struct buffer, ageQ));
	    lmP->eventSet = asevent_CreateSet();
	    while (bP) {
		asevent_AddToSet(lmP->eventSet, bP->aevent);
		bP = (struct buffer *)FIFO_NextOldest(&lmP->inTransit, bP,
							&bP->ageQ);
	    }
	    code = asevent_WaitAny(lmP->eventSet);
	    afsl_MBZ (code);
	    asevent_ReleaseSet(lmP->eventSet);
	    map_assertlocked(lmP);
	}
    }
}

/*
 * RecycleLogBufs -- garbage collect log buffers.
 *
 *     This call is non-blocking!
 */
static long RecycleLogBufs(IN struct logMap *lmP)
{
    struct buffer *bP, *nbP;
    asevent_event_t *ae;
    long signaled;
    long returnCode;

    map_assertlocked(lmP);

    /*
     * Because we will be removing elements from the inTransit list, we compute
     * the next element before we delete the current element.  Conceputally
     * simple but the code here looks ugly.
     */
    for (bP = (struct buffer *)
	 FIFO_LookAtOldest(&lmP->inTransit, offsetof(struct buffer, ageQ));
	 bP;
	 bP = nbP) {

	nbP = (struct buffer *)
	    FIFO_NextOldest(&lmP->inTransit, bP, &bP->ageQ);

	afsl_Assert (bufferIsWritep(bP));
	ae = bP->aevent;
	returnCode = asevent_StatEvent(ae, &signaled);
	CheckIOError(returnCode,bP->adev,bP->blockNum);

	if (signaled) {
	    /* move to free list */
	    afsl_Assert(ae->buf->b_flags & B_DONE);
	    asevent_Release(ae);
	    bufferClearWritep(bP);
	    FIFO_REMOVE(&lmP->inTransit, &bP->ageQ);
	    if (!lmP->freeBuf.length)
		osi_Wakeup(&lmP->freeBuf);
	    FIFO_ADD (&lmP->freeBuf, &bP->freeList);
	}
    }

    /*
     * Now, recompute the oldest active log block.  We stash the current offset
     * away in the buffer structure (bufFH - not used for log buffers) and
     * since the inTransit fifo is kept in order we just use the oldest,
     * otherwise use currentLogBlock.
     */
    bP = (struct buffer *)
	FIFO_LookAtOldest(&lmP->inTransit, offsetof(struct buffer, ageQ));
    if (bP) lmP->activeLogBlock = bP->bufFH;
    else lmP->activeLogBlock = lmP->currentLogBlock;

    return (IsFifoEmpty (&lmP->freeBuf) ? LOG_NOFREEBUFS : BUFFER_OK);
}

/*
 * elbl_Tune - the log packages tune package.  Called from buffer_Tune.
 *
 * at the end-of-file because it must be able to see all static data types.
 * There is an implicit assumption in this code that an assignment to
 * a 32 bit quantity is atomic w.r.t. all readers.
 */
/* SHARED */
long elbl_Tune(
     IN u_long op, /* operation */
     IN u_long type, /* 0 = get; 1 = set */
     INOUT opaque *value) /* pointer to appropriate data type */
{
      long code = BUFFER_OK;

      switch (op) {
	case BUFFER_MAXTRAN:
	  if (type == BUFFER_SETVAL) {
	      code = BUF_BADVALUE;
	  } else /* BUFFER_GETVAL */
	      *(u_long *)value = elbl_MaxTran;
	case BUFFER_MAXLOG:
	  if (type == BUFFER_SETVAL) {
	      if (*(u_long *)value >= elbl_MaxLogs) /* only increases */
		  elbl_MaxLogs = *(u_long *)value;
	      else
		  code = BUF_BADVALUE;
	  } else /* BUFFER_GETVAL */
	      *(u_long *)value = elbl_MaxLogs;
	  break;
	case BUFFER_LOGLIMIT:
	  if (type == BUFFER_SETVAL) {
	      if (*(u_long *)value <= (2 << LOG_FULL_POWEROFTWO)) {
		  logFullSize = (unsigned)*(u_long *)value;
		  /* XXX: go back and recompute for all active log maps. */
	      } else {
		  code = BUF_BADVALUE;
	      }
	  } else { /* BUFFER_GETVAL */
	      *(u_long *)value = logFullSize;
	  }
	  break;
	default:
	  code = BUF_BADOP;
	  break;
      }
      return code;
}

/*
 * elbl_UnageBuffer -- remove a buffer from the age queue.  If force is true we
 *     don't insist that the buffer's updates have committed.
 */
/* SHARED */
void elbl_UnageBuffer(IN struct buffer *bP, IN int force)
{
      struct logMap *lmP;

      GetLogMap(bP->adev, lmP);
      afsl_Assert(lmP && (bP->currentAgeQ == &lmP->bufferAgeQ));

      afsl_Assert (force || BufCommitted (lmP, bP));
      FIFO_REMOVE(bP->currentAgeQ, &bP->ageQ);
      bP->currentAgeQ = (struct fifo *)0;

      /* update the size information for the log */
      ComputeActiveLog(lmP);
      map_unlock(lmP);

      return;
}

/*
 * elbl_NextAgedBuffer -- walk the age queue
 *
 * PARAMETERS -- This routine allows the caller to walk the entire age queue
 *     (used during shutdown, for example.)  For the first buffer, it can be
 *     called with (bP==0).  Out param is null when we get to the end of the
 *     list.
 */
/* SHARED */
void elbl_NextAgedBuffer(
     IN struct async_device *dP,
     IN struct buffer *bP,
     OUT struct buffer **bbP)
{
      struct logMap *lmP;

      GetLogMap(dP, lmP);
      if (!lmP) {
	  *bbP = (struct buffer *)0;
	  return;
      }

      *bbP = (struct buffer *)(bP ? FIFO_NextOldest(&lmP->bufferAgeQ, bP,
						    &bP->ageQ) :
			       FIFO_LookAtOldest(&lmP->bufferAgeQ,
					offsetof(struct buffer, ageQ)));
      map_unlock(lmP);

      return;
}

/*
 * elbl_GetSomeTran -- provides a way of quickly figuring out the newest (or
 *     oldest) transaction of a specified type associated with a particular
 *     log.  We interpret the input mask to determine which lists to check.
 *     Note the active list and ageQ are kept in order the others are not.
 */
/* SHARED */
struct elbt_TranRec *elbl_GetSomeTran(
  IN struct async_device *dP,
  IN int age,				/* newest, oldest, any */
  IN unsigned which,			/* what set of trans to consider */
  OUT elbt_TranRec_t *tidP)		/* id of returned tran */
{
    struct logMap *lmP;
    struct elbt_TranRec *tP;
    struct elbt_TranRec *ctP;
    int newest = (age == ELBL_TRAN_NEWEST);
    int any = (age == ELBL_TRAN_NEWEST);

    GetLogMap(dP, lmP);
    if (!lmP)
	return 0;

    tP = NULL;

    /* Some of these cases can be special cased */
#define NEWEST 0x80
    if (newest) which |= NEWEST;
    switch (which) {

      case ELBL_TRAN_ACTIVE:
	tP = FindOldestActiveTran (lmP);
	break;

      case ELBL_TRAN_ACTIVE|ELBL_TRAN_PENDING:
	tP = FindOldestTran (lmP);
	break;

      case NEWEST|ELBL_TRAN_ACTIVE:
	tP = FIFO_LookAtNewest(&lmP->aTranList,
			       offsetof(struct elbt_TranRec, logMapThread));
	break;

      case NEWEST|ELBL_TRAN_ACTIVE|ELBL_TRAN_PENDING:
	tP = FIFO_LookAtNewest(&lmP->tranAgeQ,
			       offsetof(struct elbt_TranRec, logAgeThread));
	break;

#define SORT_TRANS do { \
    if (!tP) tP = ctP;			/* initialize if this is first */     \
    if (any) break;			/* for "any" this is good enough */   \
    if ((tP != ctP) &&			/* if this isn't the first: sort */   \
	(TranOneOlderThanTwo (tP, ctP) == newest))			      \
	tP = ctP;			/* swap them */			      \
} while (0)

      default:
	if (which&ELBL_TRAN_ACTIVE) {
	    FIFO_FOR (ctP, &lmP->aTranList, logMapThread) {
		if ((which & ELBL_TRAN_NESTED) && !ctP->nested)
		    continue;
		else if ((which & ELBL_TRAN_UNNESTED) && ctP->nested)
		    continue;
		else /* don't care about nested... */;
		SORT_TRANS;
	    }
	}
	if (tP && any)
	    break;
	if (which&ELBL_TRAN_PENDING) {
	    FIFO_FOR (ctP, &lmP->pTranList, logMapThread) {
		if ((which & ELBL_TRAN_NESTED) && !ctP->nested)
		    continue;
		else if ((which & ELBL_TRAN_UNNESTED) && ctP->nested)
		    continue;
		else /* don't care about nested... */;
		SORT_TRANS;
	    }
	}
	if (tP && any)
	    break;
	if (which&(ELBL_TRAN_COMPLETED|ELBL_TRAN_COMMITTED)) {
	    FIFO_FOR (ctP, &lmP->cTranList, logMapThread) {
		/* Check for comleted only and committed only cases */
		if (!(which&ELBL_TRAN_COMPLETED) &&
		    !TranCommitted (lmP, ctP))
		    continue;		/* skip uncommitted */
		else if (!(which&ELBL_TRAN_COMMITTED) &&
			 TranCommitted (lmP, ctP))
		    continue;		/* skip committed */
		else /* both set means anythin on cTranList... */;
		SORT_TRANS;
	    }
	}
	break;
    }
    if (tP && tidP)
	*tidP = tP->tranId;
    map_unlock(lmP);
    return tP;
}

/*
 * elbl_WaitForAnyTran -- wait for any transaction on the specified device to
 *     complete.  There is no specific guarantee with this routine just that it
 *     will wait at least a little while (if it safe to).
 *
 * LOCKS USED -- Since our caller can't hold log-module locks which prevent the
 *     tranAgeQ from changing we can not exclude the possibility that we will
 *     get here and there will be nothing to wait for.  If this happens print a
 *     message and return. */

/* SHARED */
void elbl_WaitForAnyTran(IN struct async_device *dP)
{
      struct logMap *lmP;

      GetLogMap(dP, lmP);

      /* Very funny race if nothing to wait for. */
      if (!lmP || (lmP->tranAgeQ.length == 0)) {
	  printf("elbl_WaitForAnyTran: lost race, dP=%#x, lmP=%#x.\n",
		(int)dP, (int)lmP);
      }

      /* We only wait if there are running transactions.  Note that although we
       * bump the tranSleepers count, we do not decrement it - that will be
       * done by the awakener to avoid races in the shutdown code (where lmP
       * might not be valid after return from this call) */

      /* It seems that this is now broken because cond_wait automatically
       * reobtains the lmP lock.  This means that if this thread does not get
       * to run immediately after being signaled and if the log is shutdown it
       * may free the logMap before this thread can reobtain the lock.
       * Heavy-weight as it is, a refCount system is probably called for here.
       * -ota 950125 */

      lmP->tranSleepers++;
      map_sleep(lmP);
      map_unlock(lmP);
}

/*
 * This routine handles scheduling the I/O on the existing log page and
 * allocating a new page.  It also checks to see if a new page can be
 * allocated (e.g. log full).
 *
 * locks: logMap must be locked.
 */
static void NewLogPage(struct logMap *lmP)
{
      asevent_event_t *ae;
      struct buffer *bP;		/* oldest buffer */
      struct elbt_TranRec *tP;		/* oldest tran */
      struct buf *bufp;

      map_assertlocked(lmP);
      logbuf_DBprint(("NewLogPage: allocating a new page"));

      /*
       * We always update the "size" information we maintain about the log
       */
      ComputeActiveLog(lmP);

      /*
       * We're planning on getting a new page in the log - is there
       * one to get?
       */
      if (IsLogFull(lmP)) {
	  struct elbt_TranRec *tP;

	  tP = FindOldestTran(lmP);

	  if (COMPUTE_TRAN_ACTIVE_LOG_SIZE(lmP, tP) >= lmP->activeSize) {
	      /*
	       * transaction is pinning tail
	       *
	       * We see if there is any hope of GC by pushing
	       * out all the I/O pending pages.
	       */
	      WaitForAllLogPages(lmP);
	  }

	  ComputeActiveLog(lmP);

	  /*
	   * let's see if we actually accomplished anything.
	   */

	  if (IsLogFull(lmP)) {
	      unsigned dResult, tResult;
	      struct buffer *bP;
	      struct elbt_TranRec *tP;
	      /*
	       * at this point, we figure out why the log is full so
	       * someone after us can diagnose the failure.
	       */

	      bP = FindOldestBuffer(lmP);
	      tP = FindOldestTran(lmP);
	      dResult = COMPUTE_DATA_ACTIVE_LOG_SIZE(lmP, bP);
	      tResult = COMPUTE_TRAN_ACTIVE_LOG_SIZE(lmP, tP);
	      afsl_Assert (dResult != 0 && tResult != 0);

	      /*
	       * We get LOG_FULL_BUFFER-1 chances to make things better.  If
	       * we cannot do it within that interval, this code will panic.
	       * Better a good panic than to have the log overwritten,
	       * however.
	       */
	      if ((dResult >= lmP->logLocnArraySize) ||
		  (tResult >= lmP->logLocnArraySize)) {
		  afsl_PAssert(dResult < lmP->logLocnArraySize,
			       ("log full: data"));
		  afsl_PAssert(tResult < lmP->logLocnArraySize,
			       ("log full: tran"));
		  afsl_PAssert(0, ("log full: unknown"));
	      }
	  }
      }

      afsl_PAssert(lmP->logLocnArraySize > lmP->activeSize,
		   ("log full: after the check"));

#if 0
      /*
       * I used this code for verifying the log is getting pulled up OK.
       */

      printf("writing block %d, offset %d, data %d, tran %d, active %d\n",
	     lmP->current->blockNum,
	     lmP->pageCache->pageNumber,
	     lmP->pageCache->oldestDataPage,
	     lmP->pageCache->oldestTranPage,
	     lmP->activeSize);
#endif

      /* yes, write out current buffer and allocate a new one */

      {   /* Compute log page utilization.  The number of bytes used on the
           * page we are about to write out is added to a decaying sum.  We
           * reduce the old sum by (n-1)/n, where n is the number of blocks in
           * the log before adding in each new sample.  When the sum is divided
           * by n it gives a average weighted towards the more recent values
           * with a time constant of about one complete cycle of the log.
           * During the beginning, when we have fewer than n samples, this is
           * not very close the the true average.  To correct for this we omit
           * the decay and divide by the number of samples to compute the
           * average until the number of samples reaches the number of pages in
           * the log.  We further enhance this computation by doubling the
           * weight of utilization samples that are less than half of the
           * current average.  This improves the response time of the average
           * to sudden decreased in the utilization.
           *
           * This utilization measurement is used to derate the actual log size
           * for the purposed of deciding whether the a new transactions
           * reservation will fit in the space available in the log.  The
           * result is that periods of low utilization will throttle multiple
           * concurrent transactions.  This should also improve utilization
           * much as anti-thrashing measures improve throughput. */

	  u_long used;			/* bytes used on this page */

	  if (lmP->pageCache->lastLogRecOff) {
	      used = lmP->pageCache->lastLogRecOff +
		  NextRecOffset(GetLogrec(lmP->current->data,
					  lmP->pageCache->lastLogRecOff));
	  } else {
	      used = elbr_FirstLogRecordOffset(lmP->pageCache);
	  }
	  if (lmP->nSamples >= lmP->logLocnArraySize)
	      lmP->sumUtil -= lmP->avgUtil;
	  lmP->sumUtil += used;
	  lmP->nSamples++;
	  if (used*2 < lmP->avgUtil) {
	      /* If utilization is decreasing rapidly double its contribution
               * to the average so we get faster response to potential
               * problems. */
	      if (lmP->nSamples >= lmP->logLocnArraySize)
		  lmP->sumUtil -= lmP->avgUtil;
	      lmP->sumUtil += used;
	      lmP->nSamples++;
	  }
	  lmP->avgUtil = lmP->sumUtil /
	      MIN(lmP->logLocnArraySize, lmP->nSamples);
	  lmP->deratedLogSize = lmP->logLocnArraySize * lmP->avgUtil /
	      lmP->current->adev->blkSize;
      }

      /* update the I/O window */
      lmP->pageCache->io_window = lmP->inTransit.length;
      lmP->pageCache->timeWritten = osi_Time();

      /* Calculate and store the checksum in the log page */
      elbr_ChecksumLogPage(lmP->current->data,lmP->current->adev->blkSize);

      /* first, write out buffer */
      ae = lmP->current->aevent = asevent_Create(ASEVENT_NOPIN);
      ae->data = (opaque)lmP->current;
      bufp = asevent_MakeBuf(
		B_ASYNC | B_WRITE, lmP->current->data, lmP->logDev->blkSize,
		lmP->logDev, lmP->current->blockNum, 0);
      (void) asevent_QueueIO(lmP->logDev, bufp, ae);
      bufferSetWritep(lmP->current);
      FIFO_ADD(&lmP->inTransit, &lmP->current->ageQ);
      lmP->current = (struct buffer *)0;

      /* allocate a buffer */
      if (++lmP->currentLogBlock >= lmP->logLocnArraySize) {
	  lmP->currentLogBlock %= lmP->logLocnArraySize;
	  lmP->passno = IncPassno(lmP->passno);
      }
      if (FindOldestBuffer(lmP) || FindOldestTran(lmP)) {

	  /* If tail is pinned by either a tran or buffer, active region
	   * grows by one, otherwise it should be (stay at) one. */

	  lmP->activeSize++;
      }
      EnsureFreeBuffer (lmP);
      lmP->current =
	  (struct buffer *)FIFO_LookAtOldest(&lmP->freeBuf,
		offsetof(struct buffer, freeList));
      FIFO_REMOVE(&lmP->freeBuf, &lmP->current->freeList);
      lmP->current->blockNum = *(lmP->logLocnArray +
				 lmP->currentLogBlock);
      lmP->current->bufFH = lmP->currentLogBlock;

      /* now, initialize the buffer */
      InitNewPage(lmP, lmP->current, lmP->currentLogBlock);
}

/*
 * ComputeActiveLog - compute the size of the active portion of the log
 *                    results are stored in the log map structure.
 *
 * locks: map must be locked.
 */
static void ComputeActiveLog(struct logMap *lmP)
{
      struct buffer *bP;
      struct elbt_TranRec *tP;
      unsigned dResult, tResult;

      map_assertlocked(lmP);
      /*
       * We find out the oldest data buffer;  if the fifo is empty
       * we get null back & set the oldestDataPage field to be the
       * page # of the current page.
       */

      bP = FindOldestBuffer(lmP);
      (lmP)->pageCache->oldestDataPage = bP ?
	  bP->bufLogFirstOff : (lmP)->currentLogBlock;
      tP = FindOldestTran(lmP);
      (lmP)->pageCache->oldestTranPage = tP ? tP->firstOffset :
	  (lmP)->currentLogBlock;
      afsl_Assert((lmP)->pageCache->oldestTranPage < lmP->logLocnArraySize);

      dResult = COMPUTE_DATA_ACTIVE_LOG_SIZE(lmP,bP);
      tResult = COMPUTE_TRAN_ACTIVE_LOG_SIZE(lmP,tP);
      afsl_Assert (dResult > 0 && tResult > 0);
      lmP->activeSize = (dResult > tResult ? dResult : tResult);
}

/*
 * Test whether the log is currently locked.  Called from elbb_SyncForce.
 */
int
elbl_LogIsLocked(void)
{
    return LOG_IS_LOCKED();
}
