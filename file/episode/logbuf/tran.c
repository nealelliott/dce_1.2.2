/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: tran.c,v $
 * Revision 1.1.62.1  1996/10/02  17:25:42  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:32:49  damon]
 *
 * $EndLog$
 */
/* tran.c */

/* Copyright (C) 1995, 1992 Transarc Corporation - All Rights Reserved. */

/*
 * This package provides transaction management services.
 */

#include <dcedfs/param.h>
#include <dcedfs/sysincludes.h>
#include <dcedfs/lock.h>
#include <dcedfs/osi.h>
#include <dcedfs/debug.h>

RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/episode/logbuf/tran.c,v 1.1.62.1 1996/10/02 17:25:42 damon Exp $")

#define DEBUG_THIS_FILE LOGBUF_DEBUG_TRAN

#include <elb.h>
#include <logrec.h>
#include <tran.h>

#define elbt_VDBprint(p) \
    dmprintf(afsdb_episode_logbuf, LOGBUF_DEBUG_VTRAN, p)
#define elbt_DBprint(p) \
    dmprintf(afsdb_episode_logbuf, LOGBUF_DEBUG_VTRAN, p)

/*
 * Forward references to private routines
 */

static int AllocTran _TAKES((struct async_device *, unsigned,
			       struct elbt_TranRec **));
static void DeallocTran _TAKES((struct elbt_TranRec *));
static unsigned long HashTranId _TAKES((struct elbt_TranRec *));
static void CompleteEC _TAKES((struct elbt_TranRec *));
static void CompleteTran _TAKES((struct elbt_TranRec *,
				  struct elbt_TranRec *));
static void PendTran _TAKES((
  IN struct elbt_TranRec *tP));
#if AFSL_DEBUG_LEVEL > 3
static unsigned TranServiceHash _TAKES((const char *));
#else
#define TranServiceHash(s) 0
#endif
static void WaitForTran _TAKES((struct elbt_TranRec *, int));
#if AFSL_DEBUG_LEVEL > 3
/* transaction monitoring interface */
static void TranMonitorInit _TAKES((void));
static void TranEnded _TAKES((struct elbt_TranRec *));
static void TranMonitorDump _TAKES((void));
#else
#define TranMonitorInit() /* no-op */
#define TranEnded(t) /* no-op */
#define TranMonitorDump() /* no-op */
#endif

/*
 * Internal Macros
 */

#define LookupTranNoCache(tid, tP) \
    DFSH_LOOKUP(struct elbt_TranRec *, &tranHashTable, tP, tid, \
		((tP)->tranId == (tid)))

#define LookupTran(tid, tP) \
do { \
    TRAN_LOCK(); \
    if (!tranCache || tranCache->tranId != tid) { \
	LookupTranNoCache(tid, tranCache); \
    } \
    tP = tranCache; \
    TRAN_UNLOCK(); \
} while (0)

/* simple one-liner to walk a list of pointers to the head of the EC */
#define GetECHead(tP, echP) \
    for (echP=tP->ecHead; echP->ecHead!=echP; echP=echP->ecHead) /*empty*/

#define ChangeTranStatus(tP,status) \
    do { \
	tP->tranStatus = status; \
	if (tP->sleepers) { \
	    osi_Wakeup(tP); \
	    tP->sleepers = 0; \
	} \
    } while (0)

#define TranFree(tP) ((tP)->tranStatus == ELBT_TRANSTAT_FREE)
#define TranActive(tP) ((tP)->tranStatus == ELBT_TRANSTAT_ACTIVE)
#define TranComplete(tP) ((tP)->tranStatus == ELBT_TRANSTAT_COMPLETE)

#if !defined(KERNEL)

/* Reschedule -- see ../anode/file.c. */

static void Reschedule ()
{
    if (afsdb_episode_logbuf & (1<<(LOGBUF_DEBUG_ADDPREEMPTS-1))) {

	/* If enabled, reschedule 7 times out of 17. */

	static int count = 0;
	int myCount = count++;
	if (myCount % 17 < 7)
	    osi_Reschedule ();
    }
}
#else
#define Reschedule()
#endif

/*
 * data structures
 */
static struct fifo tranFreeList;
static struct fifo tranCompleteList;
static struct fifo tranPendingList;
static struct fifo tranActiveList;
static struct dfsh_hashTable tranHashTable;
static struct elbt_TranRec *tranCache;

epi_mutex_t elbt_tranSysLock;		/* global tran system lock */
epi_cond_t elbt_tranSleepers;
unsigned elbt_activeTranSleepers;	/* # of threads waiting to run */

#define TRAN_ASSERTLOCKED() afsl_Assert (epi_mutex_held(&elbt_tranSysLock))
#define TRAN_LOCK() epi_mutex_lock(&elbt_tranSysLock)
#define TRAN_UNLOCK() epi_mutex_unlock(&elbt_tranSysLock)
#define TRAN_SLEEP(cv) epi_cond_wait(cv, &elbt_tranSysLock)

/*
 * private data
 */
elbt_TranRec_t elbt_GlobalTranId
    = 0xfffffff6;			/* small neg. to test wrap code. */
u_int32 elbt_maxTrans			/* obsolete */
    = 100;				/* start small, but grow adaptively */
unsigned elbt_LargestTranSeen;		/* largest transaction ever seen */

epi_mutex_t elbt_ECSysLock;		/* global EC lock */

#define EC_ASSERTLOCKED() afsl_Assert(epi_mutex_held(&elbt_ECSysLock))
#define EC_LOCK() epi_mutex_lock(&elbt_ECSysLock)
#define EC_UNLOCK() epi_mutex_unlock(&elbt_ECSysLock)

struct lock_data elbt_GCSysLock;	/* global GC lock */
u_int elbt_GCRunning;			/* number of threads in GCTran */
int elbt_GCNotify;			/* request notify of GCTran exit */

#define GC_ASSERTLOCKED() afsl_AssertLocked(&elbt_GCSysLock)
#define GC_LOCK() lock_ObtainWrite_r(&elbt_GCSysLock)
#define GC_UNLOCK() \
    do { GC_ASSERTLOCKED(); \
	 lock_ReleaseWrite_r(&elbt_GCSysLock); } while (0)
#define GC_UNLOCK_AND_SLEEP(addr) osi_SleepW_r((opaque)addr,&elbt_GCSysLock)

/*
 * elbt_Init - initialize transaction management subsystem.
 *
 * Locks: assumes no locking (single-threaded at init time).  Call is
 * reentrant.
 */
/* SHARED */
void elbt_Init(IN u_int32 maxTrans) /* max # of allowed transactions */
  {
      static inited;
      struct elbt_TranRec *tP = (struct elbt_TranRec *)0;
      long code;

      if (inited) {
	  /* no error on reinitialization */
	  return;
      }

      epi_mutex_init(&elbt_tranSysLock, "global tran");
      epi_cond_init(&elbt_tranSleepers, "tran sleepers");

      epi_mutex_init(&elbt_ECSysLock, "tran EC");
      lock_InitDesc(&elbt_GCSysLock, "tran GC");

      TRAN_LOCK();

      /*
       * Initialize the various transaction status queues.
       */

      FIFO_INIT(&tranActiveList,
	offsetof(struct elbt_TranRec, statusThread));

      FIFO_INIT(&tranPendingList,
	offsetof(struct elbt_TranRec, statusThread));

      FIFO_INIT(&tranCompleteList,
	offsetof(struct elbt_TranRec, statusThread));
      FIFO_INIT(&tranFreeList,
	offsetof(struct elbt_TranRec, statusThread));

      /*
       * now set the maximum limit on transactions within the system.
       * Note that if this value is zero the system will not restrict
       * the number of simultaneous transactions - but it might exhaust
       * available memory.
       */
      if (maxTrans) {
	  elbt_maxTrans = maxTrans;
      }

      code = dfsh_HashInit (&tranHashTable, HashTranId,
			    (int)((char *)(&tP->nextHash) - (char *)tP));
      afsl_MBZ(code);
      code = dfsh_HashInitKeyFixed (&tranHashTable,
				    (unsigned)((char *)(&tP->tranId) -
					       (char *)tP),
				    sizeof(elbt_TranRec_t));
      afsl_MBZ(code);

      TRAN_UNLOCK();

      GC_LOCK();
      elbt_GCRunning = 0;
      GC_UNLOCK();

      TranMonitorInit();
  }

/*
 * elbt_GetTran -- This function acquires a locked transaction record for a
 *     transaction.  If active is true then the transaction must be active
 *     otherwise states PENDING or COMPLETE are okay.
 *
 * LOCKS USED -- The global TRAN lock protects the hash table but cannot be
 *     held while waiting for the lock on a tran record.  We resolve this
 *     slight window with a bit of extra checking.
 *
 *     Note that after the lock is acquired we check to assure ourselves that
 *     this transaction record is still pointing to the right transaction and
 *     that this transaction hasn't been freed.  If the transaction has been
 *     freed then we just return a null pointer to the caller.
 *
 *     If the tid is wrong then the transaction record was reused in the window
 *     between when the drop the global TRAN lock and when we acquired the lock
 *     on the tP we looked up in the hash table.  If this happens we unlock it
 *     and go around again.
 */
struct elbt_TranRec *elbt_GetTran(
  elbt_TranRec_t tid,
  int active)
{
    struct elbt_TranRec *tP;

    while (1) {
	LookupTran(tid, tP);
	if (tP == NULL)
	    return NULL;
	elbt_TranLock(tP);
	if (tP->tranId == tid) {
	    if (active ?
		(tP->tranStatus == ELBT_TRANSTAT_ACTIVE) :
		(tP->tranStatus != ELBT_TRANSTAT_FREE))
		return tP;		/* return matching tran locked */
	    else {
		elbt_TranUnlock(tP);	/* no such tran id */
		return NULL;
	    }
	} else
	    elbt_TranUnlock(tP);	/* id doesn't match: check again */
    }
}

/* AnyActive -- return true if any non-nested transactions are active or
 *     pending.  This will be true if other threads can be counted upon to call
 *     CompleteTran and send us a wakeup. */

static int AnyActive (struct async_device *dP)
{
    struct elbt_TranRec *tP;
    
    FIFO_FOR (tP, &tranActiveList, statusThread) {
	if ((tP->dev == dP) && !tP->nested)
	    return 1;
    }
    FIFO_FOR (tP, &tranPendingList, statusThread) {
	if ((tP->dev == dP) && !tP->nested)
	    return 1;
    }
    return 0;
}

/*
 * elbt_StartTran -- transaction system starts a transaction.
 *
 *     We depend on the caller (elbb_StartTran) to invoke HandleLogFull if we
 *     indicate a failure.  This is expected remove buffers from the ageQ,
 *     end undo zero transactions, and even force the log to GC completed
 *     transactions.
 *
 * PARAMETERS -- If oldTranId is non-null then start a nested transaction.
 *     This implies that we pass the force flag to AllocTran as well as to the
 *     underlying log module StartTran.
 *
 * LOCKS USED -- None required. Internally grabs the transaction system lock
 *     and a lock on the transaction record.
 */
/* SHARED */
void elbt_StartTran(
  IN const caddr_t serviceName,
  IN elbt_TranRec_t oldTranId,
  IN struct async_device *dP,
  OUT elbt_TranRec_t *newTranIdP)
  {
      struct elbt_TranRec *tP = NULL;
      int force = !elbt_IsNullTranId(oldTranId);
      int dsize;			/* dirty buffers are the reason */

retry:
      dsize = AllocTran(dP, force, &tP);
      if (!tP) {

	  /* We can't allocate a new transaction because the log is too full to
           * start a new transaction.  Do what we can here then return and let
           * the caller try again.  It will call HandleLogFull after every
           * unsuccessful return.  This will flush dirty buffers from the ageQ,
           * end undo zero trans and commit completed transactions (via forced
           * GCTran), all of which can pin log pages. */

	  afsl_Assert (!force);
	  *newTranIdP = 0;		/* we will return failure */

	  if (dsize)
	      return;			/* let buffer.c write dirty bufs */

	  TRAN_LOCK();
	  if (!osi_IsKernelPanic() &&
	      AnyActive (dP)) {

	      /* If we are not being forced and there are other transactions
               * running which are not nested (i.e. not undo zero) then we can
               * safely wait for one of them to finish. */

	      elbt_activeTranSleepers++;
	      TRAN_SLEEP(&elbt_tranSleepers);
	      elbt_activeTranSleepers--;
	      TRAN_UNLOCK();
	      goto retry;
	  } else {

	      /* We rely on our caller (elbb_StartTran) not to give up without
               * getting a transaction.  This is because the condition we slept
               * on above was notified using "signal" instead of "broadcast".
               * This means that every thread that wakes up must obtain a
               * transaction or pass the baton to someone else via another
               * "signal". */

	      TRAN_UNLOCK();
	  }

	  /* If there are completed transactions then we can reclaim a bit of
           * space in the log by forcing them to commit.  The GCTran procedure
           * does this, however, since HandleLogFull also does this we don't
           * really need to call this here.  Further HLF has a better idea what
           * is needed so we defer forcing the log. */

	  /* Otherwise there are undo zero transactions or dirty buffers on the
           * age queue.  So just return. */

	  return;
      }

      elbt_AssertTranLocked(tP);
      *newTranIdP = tP->tranId;

      /*
       * build a magic hash value.  For now, we'll stick this into the
       * reserved field of the log record.  It is only intended to be a
       * debugging aid anyway...
       */
      if (serviceName) {
	  tP->opString = serviceName;
	  tP->opHash = TranServiceHash(serviceName);
      } else {
	  tP->opHash = 0;
	  tP->opString = 0;
      }

#ifdef AFS_DEBUG
      /*
       * Start records are only written as debugging aids.  This limits
       * the size of the log (slightly) and cuts down on the overhead
       * associated with starting a transaction.
       */
      if (afsdb_episode_logbuf & (1<<LOGBUF_DEBUG_STARTRAN)) {
	  /*
	   * Note that we ignore errors from this call to elbl_Write - this
	   * is a debugging record only and there is really no reason to do
	   * any serious work if this operation fails.
	   */
	  (void) elbl_Write(tP, LOGREC_START, (u_int32)strlen(serviceName)+1,
			    serviceName);
      }
      if (afsdb_episode_logbuf & (1<<LOGBUF_DEBUG_BUFFER)) {
	  elbt_VDBprint(("started tran %d for service %s with hash value %d\n",
			 tP->tranId, serviceName, tP->opHash));
      }
#endif /* AFS_DEBUG */
      elbt_TranUnlock(tP);
}

/*
 * elbt_EndTran -- handle changing the state of a transaction from active to
 *     complete or pending (EC).
 *
 * LOCKS USED -- See logbuf-locking file for details.
 */
/* SHARED */
void elbt_EndTran(IN elbt_TranRec_t tranId)
  {
      struct elbt_TranRec *tP, *ecHead;
      struct elbt_TranRec *member;
      u_int32 code;

      EC_LOCK();

      tP = elbt_GetTran(tranId, 1);
      afsl_Assert(tP && TranActive(tP) && tP->ecHead);

      GetECHead(tP, ecHead);
      afsl_Assert (ecHead->ecSize > 0);
      if (--ecHead->ecSize > 0) {

	  /* This tran is part of an EC but we are not the last.  Just mark
           * this tran as pending, and clear the ec head ptr as a consistency
           * check. */

	  EC_UNLOCK();
	  PendTran(tP);
	  elbt_VDBprint(("ending tran %d ec head is tran %d\n",
			 tP->tranId, ecHead->tranId));
	  elbt_TranUnlock(tP);
	  return;
      }

      /* The ecSize is zero so we are the only thread handling this EC. */

      if (ecHead != tP)
	  elbt_TranLock (ecHead);

      /* Now we have the EC's head locked and we can look at its list of EC
       * members.  If this list is empty we handle the null EC case: i.e.  no
       * merges were done. */

      if (IsFifoEmpty (&ecHead->ecList)) {

	  /* This tran is its own EC */

	  afsl_Assert (ecHead == tP);
	  EC_UNLOCK();

	  /* We write out the name of the routine which started this routine in
           * the end record, then call CompleteTran to handle bookkeeping. */

	  /* Should be strlen(...)+1; see ot 5513 -ota */
	  code = elbl_Write(tP, LOGREC_END,
			    strlen((char *)tP->opString),
			    (const caddr_t)tP->opString);
	  afsl_MBZ(code);
	  CompleteTran (tP, 0);
	  elbt_TranUnlock(tP);
	  return;
      }

      /* This is a "real" EC.  Lock all the members and call CompleteEC(). */

      FIFO_FOR (member, &ecHead->ecList, ecList.thread) {
	  if (member == tP)
	      continue;			/* tP is already locked */
	  afsl_Assert (member != ecHead); /* not on its own list */
	  elbt_TranLock(member);
      }

      EC_UNLOCK();

      {   unsigned before, after;

	  before = tranPendingList.length;
	  elbt_VDBprint(("before finishing EC %d transactions\n", before));
	  elbt_VDBprint(("EC (tran %d) size is %d\n",ecHead->tranId,
			 ecHead->ecList.length+1));

	  CompleteEC(ecHead);		/* unlocks whole EC */

	  after = tranPendingList.length;
	  elbt_VDBprint(("after finishing EC %d transactions\n", after));
	  /* This output can be dubious as it isn't protected by any lock. */
      }

#if defined(AFS_FORCE_METADATA)
      /*
       * DB 3189: make transactions atomic
       *
       * The caller now waits for the transaction to commit.  Note that this
       * isn't required for correct functionality, but does assure the
       * higher-level users of the file system that the operations will
       * always be there.  Sort of a moral fsync for meta-data operations.
       * This makes Episode look more like FFS.
       *
       * Note that we do this by calling elbt_TranForce with the "waitOK"
       * field set to 1.  This means that the call will not return
       * until the transaction has committed.  Very permanent.
       */

      code = (u_int32) elbt_TranForce(tranId, 1);
#endif /* AFS_FORCE_METADATA */
  }

/*
 * elbt_Shutdown -- shut down transactions on the specified device.
 *
 * locks: none required. Will acquire global tran lock & individual tran locks.
 */
/* SHARED */
void elbt_Shutdown(
  struct async_device *dP,
  void (*const deallocP)(struct buffer *, int))
  {
      struct fifo gcList;
      struct elbt_TranRec *tP;

      /* set up fifo */
      FIFO_INIT(&gcList, offsetof(struct elbt_TranRec, logMapThread));
      /*
       * shutting down at this level is fairly straight-forward - we
       * need only signal the log package to write out all its log pages
       * and wait for the system to quiesce.
       *
       * N.B.  Locking here isn't even done.  We are assuming that higher-
       * levels have properly synchronized access to this aggregate.
       *
       * elbl_Close gives us back a list of any remaining transactions
       * requiring garbage collection.
       */
      elbl_Close(dP, &gcList, deallocP);

      /*
       * finish up those transactions
       */
      while (tP = ((struct elbt_TranRec *)
		   FIFO_LookAtOldest(&gcList,
			offsetof(struct elbt_TranRec, logMapThread)))) {
	  FIFO_REMOVE(&gcList, &tP->logMapThread);
	  elbt_TranLock(tP);
	  DeallocTran(tP);		/* implicit elbt_TranUnlock(tP) */
      }

      if (afsdb_episode_logbuf & (1<<DEBUG_THIS_FILE)) {
	  TranMonitorDump();
      }
  }

/*
 * elbt_GCTran -- garbage collect transaction list for a particular device.  If
 *     force is true it guarantees to return at least one transaction to the
 *     free list if at all possible.
 *
 * LOCKS USED -- To ensure forward progress we must prevent potentially
 *     freeable transactions from hiding in the private gcList of another
 *     caller of GCTran.  So we use a global lock.
 *
 *     We also lock each individual tran being freed.  This means the caller
 *     should not hold any (completed) tran locks.
 */
/* SHARED */
void elbt_GCTran(IN struct async_device *dP, IN int force)
{
    struct fifo gcList;
    int found = 0;
    struct elbt_TranRec *tP;

    FIFO_INIT(&gcList, offsetof(struct elbt_TranRec, logMapThread));

try_again:
    GC_LOCK();
    elbt_GCRunning++;
    GC_UNLOCK();

    /* Return a list of committed trans.  Force means try very hard to return
     * at least one transaction. */
    elbl_GCTran(dP, force, &gcList);

    /* (If preempts are enabled) Let others run while we hold these
     * transactions on our private list to make sure the GCRunning mechanism
     * works.  We hold no locks at this point so various interesting things
     * might happen. */
    Reschedule();

    /*
     * We traverse the list of garbage collected transactions and clean
     * the transactions up.
     */
    while (tP = (struct elbt_TranRec *)
	   FIFO_LookAtOldest(&gcList,
		offsetof(struct elbt_TranRec, logMapThread))) {
	FIFO_REMOVE(&gcList, &tP->logMapThread);
	elbt_TranLock(tP);
	DeallocTran(tP);		/* implicit elbt_TranUnlock(tP) */
	found++;
    }

    GC_LOCK();
    elbt_GCRunning--;
    if (!found && elbt_GCRunning && force && !osi_IsKernelPanic()) {
	/* perhaps someone else is still working on some. */
	elbt_GCNotify++;
	GC_UNLOCK_AND_SLEEP (&elbt_GCRunning);
	goto try_again;
    }
    if (elbt_GCNotify) {
	elbt_GCNotify = 0;
	osi_Wakeup (&elbt_GCRunning);
    }
    GC_UNLOCK();
}

/*
 * elbt_ECMerge -- joins two transactions into an equivalence class.  The
 *     second tran must be active.
 *
 * LOCKS USED -- Uses global EC lock to protect EC structures and two avoid
 *     deadlocks when obtaining locks on both transactions.  See logbuf-locking
 *     file for details.
 *
 * PERFORMANCE -- The ordering of the initial transaction lookups are done to
 *     provide best performance in the common case.
 */
/* EXPORT */
int32 elbt_ECMerge(
  IN elbt_TranRec_t tran1,
  IN elbt_TranRec_t tran2,
  IN unsigned long debugValue)
{
    struct elbt_TranRec *t1P, *t2P;
    struct elbt_TranRec *ec1P, *ec2P;
    struct elbt_TranRec *tmp;

    /*
     * Extraneous case - should be optimized out by caller
     */
    if (tran1 == tran2) {
	return BUFFER_OK;
    }

    TRAN_LOCK();
    LookupTranNoCache(tran1, t1P);
    if (!t1P) {
	/* no such transaction - must be very old */
	TRAN_UNLOCK();
	return BUFFER_OK;
    }
    /* else try getting second tran */
    if (tranCache && tranCache->tranId == tran2)
	t2P = tranCache;
    else
	LookupTranNoCache(tran2, t2P);
    TRAN_UNLOCK();

    /* We assert that the second transaction must be active.  This is a
     * regression test to catch bugs where an invalid transaction is being
     * placed into the tlock.  Because this is an invariant it is safe to look
     * without holding the tran's lock. */
    afsl_Assert (TranActive(t2P));

    EC_LOCK();

    elbt_TranLock(t1P);
    if ((t1P->tranId != tran1) || TranFree(t1P) ||
	TranComplete(t1P)) {
	/* transaction is gone - must have just been GCed */
	EC_UNLOCK();
	elbt_TranUnlock(t1P);
	return BUFFER_OK;
    }
    elbt_TranLock(t2P);

    /* Also assert that these two ECs are for the same log; this is a
     * regression test for global tlocks. */
    afsl_Assert(t1P->mapP == t2P->mapP);

    /* Everything else we do is protected by EC lock so drop both tran locks */

    elbt_TranUnlock(t1P);
    elbt_TranUnlock(t2P);

    /*
     * Now, check to see if these two transactions are already equivalent to
     * each other.
     */
    GetECHead(t1P, ec1P);
    GetECHead(t2P, ec2P);

    if (ec1P == ec2P) {
	/* transactions are already equivalent */
	EC_UNLOCK();
	return BUFFER_OK;
    }

    /*
     * We now have two transaction EC's which must be merged.
     */
    elbt_VDBprint(("Merging transaction %d with %d\n", tran1, tran2));

    /* While we hold the EC lock the ecSize field can not change.  In addition,
     * if it isn't zero (which it can't be unless we're somehow running
     * concurrently with someone in EndTran!), we can manipulate the ecList
     * protected only by the EC lock.  Once ecSize reaches zero the ecList can
     * be protected by the only by the tranLock (see EndTran and logbuf-locking
     * for details). */

    /*
     * we always fold the smaller EC into the larger EC.  This gives us fairly
     * strong guarantees on the number of indirections needed to get to the
     * head of the ec through the iterative process.
     */
    if (ec1P->ecSize > ec2P->ecSize) {
	tmp = ec1P;			/* swap ec1 & ec2 */
	ec1P = ec2P;
	ec2P = tmp;
    }
    afsl_Assert (ec1P->ecSize && ec2P->ecSize &&
		 (ec1P->ecSize <= ec2P->ecSize));

    /* fold ec1 into ec2 */
    ec2P->ecSize += ec1P->ecSize;
    ec1P->ecSize = 0;
    ec1P->ecHead = ec2P;

    /* XXX: optimize this to just splice the loops */
    while (tmp = FIFO_LookAtOldest(&ec1P->ecList,
	       offsetof(struct elbt_TranRec, ecList.thread))) {
	FIFO_REMOVE(&ec1P->ecList, &tmp->ecList.thread);
	FIFO_ADD(&ec2P->ecList, &tmp->ecList.thread);
    }
    /*
     * finally, insert the now vanishing EC head.
     */
    FIFO_ADD(&ec2P->ecList, &ec1P->ecList.thread);

    EC_UNLOCK();
    return BUFFER_OK;
}

/*
 * CompleteEC -- routine to handle the final end in an EC.
 *
 * LOCK USED -- caller locks all the members of the EC including the head but
 *     does not hold the EC lock.  All tran locks of EC members are unlocked
 *     upon return.
 */
static void CompleteEC(IN struct elbt_TranRec *ecP)
{
    struct elbt_TranRec *ntP;
    int32 code;

    /*
     * This is the last element within the EC; we must now commit the EC.  To
     * do this we must do several things:
     *   (1) Write batch end records out against the transactions
     *       within the EC.
     *   (2) write the end record for the head transaction
     *   (3) complete all the transactions.
     */

    elbt_AssertTranLocked(ecP);
    afsl_Assert(ecP->ecSize == 0);

    icl_Trace1 (elb_iclSet, TR_EC_COMPLETE,
		ICL_TYPE_LONG, ecP->ecList.length+1);

    /*
     * We won't write the end record out for the final transaction until
     * after all the batch end records have been written out.
     */
    FIFO_FOR (ntP, &ecP->ecList, ecList.thread) {
	afsl_Assert (ntP != ecP);
	elbl_WriteBend(ecP, ntP);
    }
    /*
     * now we end the main EC transaction.
     */
    /* Should be strlen(...)+1; see ot 5513 -ota */
    code = elbl_Write(ecP, LOGREC_END,
		      strlen((char *)ecP->opString),
		      (const caddr_t)ecP->opString);
    afsl_MBZ(code);

    /* These trans are now safely ended so mark them so.  Pass in the head ecP
     * so that the last log position of each can be set to match the position
     * of the end record of ecP.  One of these guys is still active, the others
     * are pending.  End the active one last. */

    while (ntP = (struct elbt_TranRec *)
	   FIFO_LookAtOldest(&ecP->ecList,
			     offsetof(struct elbt_TranRec, ecList.thread))) {
	FIFO_REMOVE(&ecP->ecList, &ntP->ecList.thread);
	CompleteTran(ntP, ecP);
	elbt_TranUnlock(ntP);
    }
    CompleteTran (ecP, ecP);
    elbt_TranUnlock(ecP);		/* unlock EC head tran last */
}

/* PendTran -- handle changing state from active to pending. */

static void PendTran(IN struct elbt_TranRec *tP)
{
    afsl_Assert (tP->tranStatus == ELBT_TRANSTAT_ACTIVE);

    TRAN_LOCK();
    FIFO_REMOVE(&tranActiveList, &tP->statusThread);
    FIFO_ADD(&tranPendingList, &tP->statusThread);
    TRAN_UNLOCK();

    if (tP->mapP) {
	/* notify the log package of the status change. */
	elbl_PendTran (tP);
    }
    ChangeTranStatus(tP, ELBT_TRANSTAT_PENDING);
}
/*
 * CompleteTran -- handle changing state from active or pending to complete.
 *     If completing a pending transaction the ecHead is also supplied so that
 *     the up last log position can be updated appropriately.
 *
 * LOCKS USED -- The input tran must be locked.  If the ec head is supplied it
 *     must also be locked.
 */
static void CompleteTran(
  IN struct elbt_TranRec *tP,
  IN struct elbt_TranRec *ecHead)
{
    int wakeup = 0;

    afsl_Assert(tP);
    elbt_AssertTranLocked(tP);

    TRAN_LOCK();
    switch(tP->tranStatus) {
      case ELBT_TRANSTAT_PENDING:
	FIFO_REMOVE(&tranPendingList, &tP->statusThread);
	break;
      case ELBT_TRANSTAT_ACTIVE:
	FIFO_REMOVE(&tranActiveList, &tP->statusThread);
	break;
      case ELBT_TRANSTAT_COMPLETE:
      case ELBT_TRANSTAT_FREE:
      default:
	panic("CompleteTran");
    }
    FIFO_ADD(&tranCompleteList, &tP->statusThread);

    if (tP->tranSize > elbt_LargestTranSeen)
	elbt_LargestTranSeen = tP->tranSize;

    if (elbt_activeTranSleepers && tP->mapP) {
	/* Only do the wakeup if we're going to remove the tran from the log
         * module's list.  This avoids spurious wakeups due to failed
         * elbl_StartTran's. */
	wakeup++;
    }
    TRAN_UNLOCK();

    if (tP->mapP) {
	/* Notify the log package of the status change. */

	/* Assert that if ecHead is specified it is also locked.  If it isn't
         * specified then the specified transaction must not be pending. */
	afsl_Assert ((ecHead && elbt_TranWriteLocked(ecHead)) ||
		     (!ecHead && TranActive(tP)));

	elbl_CompleteTran(tP, tP->tranSize, ecHead);
    }

    if (wakeup)
	epi_cond_signal(&elbt_tranSleepers);

    ChangeTranStatus(tP, ELBT_TRANSTAT_COMPLETE);
}

/*
 * elbt_GetTranStatus -- return the status of a transaction.
 *
 * Locks: None required. Acquires tranSysLock and lock on transaction.
 */
/* SHARED */
int32 elbt_GetTranStatus(IN elbt_TranRec_t tranId)
  {
      struct elbt_TranRec *tP;
      int32 code;

      /* we must find the transaction and lock it. */
      tP = elbt_GetTran(tranId, 0);

      if (!tP) {
	  return BUF_TRANCOMMIT;
      }

      switch(tP->tranStatus) {
	case ELBT_TRANSTAT_ACTIVE:
	  code = BUF_TRANACTIVE;
	  break;
	case ELBT_TRANSTAT_PENDING:
	  code = BUF_TRANENDED;
	  break;
	case ELBT_TRANSTAT_COMPLETE:
	  code = (elbl_IsTranCommitted(tP)
		  ? BUF_TRANCOMMIT : BUF_TRANCOMPLETE);
	  break;
	case ELBT_TRANSTAT_FREE:
	  code = BUF_TRANCOMMIT;
	  break;
	default:
	  panic("elbt_GetTranStatus");
      }
      elbt_TranUnlock(tP);

      return code;
  }

/*
 * elbt_TranForce -- force a transaction to be commited or something.
 *
 * PARAMETERS -- waitOK is specifies a spectrum of behaviors for this routine.
 *     0 -- caller can't wait for other transactions to end.  Commit only if
 *         transaction is complete, otherwise return BUF_WOULDBLOCK.
 *     1 -- don't return until transaction has committed.  This guarantees
 *         persistence for updated done under this transaction.  "fsync" is the
 *         prototypical caller.
 *     2 -- don't return until transaction has completed.  This ensures
 *         serialization of this transaction with respect to any transactions
 *         started after this call returns.
 *
 * RETURN CODES -- returns BUF_WOULDBLOCK only if waitOK is zero and the
 *     specified transaction has not ended.
 */
/* SHARED */
long elbt_TranForce(tranId, waitOK)
  IN elbt_TranRec_t tranId;
  IN unsigned waitOK;
{
    struct elbt_TranRec *tP;

    tP = elbt_GetTran(tranId, 0);
    if (!tP)
	return BUFFER_OK;
    if (waitOK == 0) {
	long code;
	if (TranComplete(tP)) {
	    elbl_ForceTran(tP);
	    afsl_Assert (elbl_IsTranCommitted(tP));
	    code = BUFFER_OK;
	} else
	    code = BUF_WOULDBLOCK;
	elbt_ReleaseTran(tP);
	return code;
    }

    /*
     * Wait for either complete or commit as requested.  For commit this is a
     * little stronger than really needed, we wait for the transaction to
     * really go away.  Tran is unlocked (and probably freed) upon return.
     */
    /* implicit elbt_TranUnlock(tP) */
    WaitForTran(tP, ((waitOK == 1) ? 1/*commit*/ : 0));
    return BUFFER_OK;
}

/***********************************************************************
 *
 * Code local to this module
 *
 ************************************************************************/

/*
 * AllocTran -- allocate a transaction record.
 *
 *     This call is responsible for handling the allocation of a new
 *     transaction record.  If force is set, this routine will not wait for
 *     another transaction to end if there are insufficient transaction record
 *     nor fail even if the log is full.  Otherwise, if it returns a null
 *     transaction, this implies there is insufficient space in the log to
 *     actually start a new transaction.
 *
 * LOCKS USED -- The allocated transaction is returned locked.
 *
 * RETURN CODES -- on failure returns true if dirty buffers are clogging up the
 *     log and false otherwise. */

static int AllocTran(
  IN struct async_device *dP,
  IN unsigned force,
  IN struct elbt_TranRec **tPP)
  {
      struct elbt_TranRec *tP = (struct elbt_TranRec *)0;
      int dsize;			/* reason for failure to allocate */

      TRAN_LOCK();

      while (1) {
	  TRAN_ASSERTLOCKED();

	  tP = FIFO_LookAtOldest(&tranFreeList,
				 offsetof(struct elbt_TranRec, statusThread));
	  if (tP) {
	      FIFO_REMOVE(&tranFreeList, &tP->statusThread);
	      break;
	  }

	  /* We didn't get a transaction because the free list is empty.  Try
           * GCing committed transactions.  If that fails then allocate a new
           * one. */

	  if (!IsFifoEmpty(&tranCompleteList)) {
	      TRAN_UNLOCK();
	      elbt_GCTran(NULL/*all devs*/, 0/*!force*/);
	      TRAN_LOCK();
	  }
	  if (IsFifoEmpty (&tranFreeList)) {

	      /* Free list is still empty so just allocate a new one */

	      TRAN_UNLOCK();
	      tP = osi_Alloc_r(sizeof(struct elbt_TranRec));
	      bzero((caddr_t)tP, sizeof (struct elbt_TranRec));
	      /* only Init the lock the first time */
	      lock_InitDesc(&tP->tranLock, "tran");
	      TRAN_LOCK();
	      break;
	  }
      }

      afsl_Assert(TranFree(tP));

      /*
       * We initialize the structure now.  Note that we never use the
       * null transaction ID.
       */
      elbt_GlobalTranId++;
      if (elbt_IsNullTranId(elbt_GlobalTranId)) elbt_GlobalTranId++;

      tP->tranId = elbt_GlobalTranId;
      tP->refCount = 0;

      afsl_Assert(tP->statusThread.next == 0);
      FIFO_ADD(&tranActiveList, &(tP)->statusThread);
      afsl_Assert(tP->sleepers == 0);	/* regression test */
      FIFO_INIT(&tP->ecList, offsetof(struct elbt_TranRec, ecList));
      tP->ecHead = tP;			/* an EC by itself */
      tP->ecSize = 1;			/* ditto */
      tP->tranStatus = ELBT_TRANSTAT_ACTIVE;
      tP->dev = dP;
      tP->nested = force;
      tP->tranSize = 0;
      tP->nextHash = (struct elbt_TranRec *)0;
      DFSH_HASHIN_R (&tranHashTable, tP);

#if AFS_DEBUG
      {   /* sanity check */
	  struct elbt_TranRec *tmpP;
	  LookupTranNoCache(tP->tranId, tmpP);
	  afsl_Assert(tmpP == tP);
      }
#endif

      TRAN_UNLOCK();
      elbt_TranLock(tP);
      afsl_Assert (TranActive(tP)); /* ended before we returned it? */

      /* tP->opString -- handled by caller */
      /* tP->opHash -- handled by caller */

      /* tP->logMapThread - handled by log package */
      /* tP->logAgeThread - handled by log package */
      /* tP->firstPass -- handled by log package */
      /* tP->lastPass -- handled by log package */
      /* tP->firstOffset -- handled by log package */
      /* tP->lastOffset; -- handled by log package */
      /* tP->ldev -- handled by log package */
      /* tP->mapP -- handled by log package */

      /*
       * we have the transaction record now; verify this is OK with the
       * log package.
       */
      dsize = elbl_StartTran(dP, tP, force);

      if (tP->mapP) {
	  /* success */
	  *tPP = tP;
	  return 0;
      }
      /*
       * something went wrong in the allocation. We deallocate the transaction
       * and go on.
       */
      CompleteTran(tP, 0);
      DeallocTran(tP);			/* implicit elbt_TranUnlock(tP) */
      *tPP = 0;
      return dsize;			/* true if log full of dirty buffers */
 }

/*
 * DeallocTran -- deallocate a transaction record.
 *
 *     This call is responsible for deallocating a transaction record.
 *
 * LOCKS USED -- global transaction lock must be held; transaction must be
 *     locked.  Upon return the transaction is unlocked and should no longer be
 *     referenced.
 */
static void DeallocTran(IN struct elbt_TranRec *tP)
  {
      afsl_Assert(tP && tP->tranStatus == ELBT_TRANSTAT_COMPLETE);
      elbt_AssertTranLocked(tP);

      afsl_Assert (!tP->mapP);		/* log module must have released it */

      TranEnded(tP);

      /*
       * and adjust its status...
       */
      TRAN_LOCK();
      FIFO_REMOVE(&tranCompleteList, &tP->statusThread);
      ChangeTranStatus(tP, ELBT_TRANSTAT_FREE);
      FIFO_ADD(&tranFreeList, &tP->statusThread);
      dfsh_HashOut_r(&tranHashTable, tP);
      tP->nextHash = NULL;
      TRAN_UNLOCK();
      elbt_TranUnlock(tP);
  }

/*
 * HashTranId - hash function for transaction IDs.
 */
static unsigned long HashTranId(struct elbt_TranRec *tP)
  {
      return tP->tranId;
  }

/*
 * elbt_TranstatWait - handle waiting for a transaction's status to change.
 *
 * Locks: the transaction must be locked when called.
 *
 * NOTE -- This code can easily sleep forever.  It needs to be fixed.
 */
/* SHARED */
void elbt_TranstatWait(IN struct elbt_TranRec *tP)
  {
      unsigned oldStatus = tP->tranStatus;

      /* NOTE -- This code can easily sleep forever.  It needs to be fixed. */
      afsl_MBZ (1);			/* luckily no one calls it. */

      elbt_AssertTranLocked(tP);

      /*
       * If there are no other active transactions, we must force the
       * log (the state-change in that case must be from complete to
       * committed) and not sleep.
       */

      if (IsFifoEmpty(&tranActiveList)) {
	  afsl_Assert(TranComplete(tP));
	  elbl_ForceTran(tP);
      }

      /*
       * we cycle, waiting for the status of the transaction to
       * change.
       */

      while (tP->tranStatus == oldStatus) {
	  tP->sleepers++;
	  elbt_TranUnlockAndSleep(tP,tP);
	  elbt_TranLock(tP);
      }
  }

/* elbt_Quiesce -- forces the current state of a specified device into a
 *     consistent state.  There are two cases of interest: either the metadata
 *     operations are quiesced or all updates are committed.  The former is
 *     sufficient for operations requiring serialization, like clearing the
 *     fileset inconsistent bit, the latter is necessary for interactions with
 *     the outside world, like vol_fsync.  A third variant allows commit of
 *     only unnested transactions.  This may be useful in fileset operations
 *     that know they have not modified and data (e.g. reclone) and don't want
 *     to write user data buffers for users of other filesets that happen to be
 *     in the same aggregate.  Thus this is really a requirement due to the
 *     granularity mismatch between filesets and aggregates.
 *
 *     Quiescing without stopping the system works by waiting for all currently
 *     running transactions which are older than the input pass and
 *     off.  This effectivly snapshots a point in time (relative to the log)
 *     when the caller (elbb_QuiesceSystem) started.  Other activity is allowed
 *     to continue concurrently.
 *
 * PARAMETERS -- the three behaviors are controlled by a parameter called
 *     "consistency"; if zero, we wait for all unnested transactions started
 *     before (pass,off) to end.  Unnested transaction includes all those
 *     except undo zero transactions.  If it is one we wait for all unnested
 *     transactions to commit.  The other interesting value is three,
 *     requesting that we also commit undo zeros (see cautions below).  The
 *     value two serializes undo zero trans, this is used by elbb_StartTran.
 *
 *     Summary: consistency = ((!undo-zero)<<1) | (commit)
 *
 * LOCKS_USED -- We use the global TRAN lock to protect the tP returned by the
 *     log module while getting its tid.  This is like elbt_GetTran().
 *
 * CAUTIONS -- We can wait for nested transactions here.  This should be safe
 *     because if our caller specified "commit" it must have already waited for
 *     all dirty buffers to be written.  Any other undo zero transactions are
 *     from the VM system which has its own Async daemon to complete I/O.  We
 *     can safely wait for it to end its undo zeros.
 */

/* SHARED */
void elbt_Quiesce(
  IN struct async_device *dP,
  IN int consistency,
  IN int arg_pass,			/* These are really u_int16s */
  IN int arg_off)
  {
      struct elbt_TranRec *tP;
      u_int16 pass = arg_pass;
      u_int16 off = arg_off;
      int commit;
      unsigned select;

      select = ELBL_TRAN_ACTIVE|ELBL_TRAN_PENDING;
      if ((commit = (consistency & 1)) != 0)
	  select |= ELBL_TRAN_COMPLETED;
      if ((consistency & 2) == 0)
	  select |= ELBL_TRAN_UNNESTED;

      /*
       * Now, we wait until all other transactions older than tid have
       * committed.
       */
      for (;;) {
	  elbt_TranRec_t tid;

	  tP = elbl_GetSomeTran(dP, ELBL_TRAN_OLDEST, select, &tid);
	  if (!tP) break;

	  /* Check to see if it is older.  Note that all ways out now unlock
           * the transaction. */

	  /* (If preempts are enabled) Try to trigger db4701. */
	  Reschedule();

	  elbt_TranLock(tP);
	  if (tP->tranId != tid) {
	      /* This one probably freed itself. */
	      elbt_TranUnlock(tP);
	      continue;
	  }
	  if (ComparePosition(pass, off,
			      tP->firstPass, tP->firstOffset) < 0) {
	      /* If the oldest uncommitted tran is now younger than the log
	       * position we were given then we are done.  Any operation
	       * started before this call has committed. */
	      elbt_TranUnlock(tP);
	      break;
	  }
	  /* wait for this transaction to complete/commit */
	  WaitForTran(tP, commit);	/* implicit elbt_TranUnlock(tP) */
      }

      /*
       * at this point, all transactions which were started/running at the
       * beginning of this call are now *on disk* (a transaction is only
       * free when its end record is verified to be on the disk.)
       */
      return;
  }

/*
 * WaitForTran -- This handles the gory details of waiting for this
 *     transaction to complete or commit depending on the "commit" paramter.
 *
 * LOCKS USED -- The transaction passed in must be locked.  Purpose of this
 *     routine is the free the transaction it is passed.  As a consequence it
 *     will be unlocked upon return and the caller should not reference the
 *     tran record further.
 *
 * CAUTIONS -- The calling thread must not be holding any active transactions.
 */
static void WaitForTran(
  IN struct elbt_TranRec *tP,
  IN int commit)			/* 0=>complete, 1=>commit */
{
      elbt_TranRec_t tid = tP->tranId;
      struct async_device *dP = tP->dev;

      elbt_AssertTranLocked(tP);

      while (!TranFree(tP) && (tP->tranId == tid)) {
	  if (!TranComplete(tP)) {

	      /* we sleep, waiting for this transaction to complete */

	      icl_Trace2 (elb_iclSet, TR_WFT_UNCOMPLETED,
			  ICL_TYPE_LONG, tid,
			  ICL_TYPE_LONG, tP->tranStatus);
	      tP->sleepers++;
	      elbt_TranUnlockAndSleep(tP,tP);
	      elbt_TranLock(tP);
	      icl_Trace2 (elb_iclSet, TR_WFT_UNCOMPLETED_WAKEUP,
			  ICL_TYPE_LONG, tid,
			  ICL_TYPE_LONG, tP->tranStatus);
	  } else {

	      if (!commit)
		  break;

	      /* Since this tran is complete all we should have to do is force
               * the log to commit it.  But we want to avoid gratuitous forces,
               * which generate incomplete log pages and extra I/O.  So, if
               * there is other activity on this log, we wait for any tran to
               * change state.  This gives other trans a chance to fill up the
               * current log page and commit our tran as a side effect.  If
               * there is no activity we just force the log. */

	      if (!elbl_TranActivity(tP->dev,
				     ELBL_TRAN_ACTIVE|ELBL_TRAN_UNNESTED)) {

		  /* We call GCTran to get some transactions back.  We know it
                   * will always make some progress and will force the log if
                   * necessary.  This will accomplish our goal, although not
                   * necessarily on the first pass. */

		  icl_Trace1 (elb_iclSet, TR_WFT_GC_START, ICL_TYPE_LONG, tid);
		  elbt_TranUnlock(tP);
		  elbt_GCTran(dP, 1);
		  elbt_TranLock(tP);
		  icl_Trace1 (elb_iclSet, TR_WFT_GC_END, ICL_TYPE_LONG, tid);

	      } else {
		  /*
                   * Somebody is still running so we wait for them in the hopes
                   * that they will push the current log page off to disk.
                   */
		  icl_Trace1(elb_iclSet, TR_WFT_ANY_START, ICL_TYPE_LONG, tid);
		  elbt_TranUnlock(tP);
		  elbl_WaitForAnyTran(dP);
		  elbt_GCTran(dP, 0);	/* reclaim any available trans */
		  elbt_TranLock(tP);
		  icl_Trace1 (elb_iclSet, TR_WFT_ANY_END, ICL_TYPE_LONG, tid);
	      }
	  }
      } /* while !free */
      /* we are done */
      elbt_TranUnlock(tP);
}

#if AFSL_DEBUG_LEVEL > 3
  /*
   * Code for monitoring transaction formation, length, etc.  Currently,
   * only enabled at high debugging levels because of its relative cost.
   */

/* the hash table is externally visible to ease in in-kernel analysis */
/* EXPORT */
struct dfsh_hashTable elbt_TranMonitorHash;

/* SHARED */
struct lock_data elbt_tranMonLock;

#define TRANMON_LOCK() lock_ObtainWrite_r(&elbt_tranMonLock)
#define TRANMON_UNLOCK() lock_ReleaseWrite_r(&elbt_tranMonLock)

/*
 * The transaction monitor data structure
 */

struct tranMonInfo {
    char                serviceName[64];/* most service names are short */
    unsigned            hash;         /* hash value of serviceName */
    struct tranMonInfo *next;         /* next in hash bucket */
    unsigned            sizes16[128]; /* group in 16 byte increments */
    unsigned            sizesk[64];   /* group in 1k byte increments */
};

/*
 * TranMonHash - hash function used by hash table pkg.
 */
static unsigned TranMonHash(struct tranMonInfo *mP)
  {
      return mP->hash;
  }

/*
 * TranMonitorInit - start the transaction monitoring service
 */
static void TranMonitorInit(void)
  {
      long code;
      struct tranMonInfo *mP;
      static init;

      if (init)
	  return;

      init = 1;

      lock_Init(&elbt_tranMonLock);

      TRANMON_LOCK();

      code = dfsh_HashInit(&elbt_TranMonitorHash,
			   (unsigned long (*)())TranMonHash,
			   (int)((char *)(&mP->next) - (char *)mP));
      afsl_MBZ(code);
      code = dfsh_HashInitKeyString(&elbt_TranMonitorHash,
				    (int)((char *)(mP->serviceName)-
					  (char *)mP));
      afsl_MBZ(code);

      /*
       * we create the "null" entry
       */
      mP = osi_Alloc_r(sizeof(struct tranMonInfo));
      bzero((caddr_t)mP, sizeof(struct tranMonInfo));
      DFSH_HASHIN_R(&elbt_TranMonitorHash, mP);

      TRANMON_UNLOCK();
  }

/*
 * TranEnded - record statistics for this transaction.
 */
static void TranEnded(struct elbt_TranRec *tP)
  {
      struct tranMonInfo *mP;
      TRANMON_LOCK();

      DFSH_LOOKUP(struct tranMonInfo *, &elbt_TranMonitorHash, mP, tP->opHash,
		  (tP->opHash == mP->hash));
      if (!mP) {
	  /* entry does not exist */
	  mP = osi_Alloc_r(sizeof(struct tranMonInfo));
	  bzero((caddr_t)mP, sizeof(struct tranMonInfo));
	  strncpy(mP->serviceName, tP->opString, 60);
	  mP->hash = TranServiceHash(tP->opString);
	  DFSH_HASHIN_R(&elbt_TranMonitorHash, mP);
      }

      /* mP points to the right entry */

      /*
       * I'm trusting the compiler can turn a divide by 2048 (const)
       * into a bit shift.
       */
      if (tP->tranSize > 2048) {
	  mP->sizesk[(tP->tranSize/2048)]++;
      } else {
	  mP->sizes16[(tP->tranSize/16)]++;
      }
      TRANMON_UNLOCK();
  }

/*
 * TranMonitorDump - print out statistics for the log.
 */
static void TranMonitorDump(void)
  {
      struct tranMonInfo *mP = (struct tranMonInfo *)0;
      unsigned index;

      while ((mP = dfsh_HashNext(&elbt_TranMonitorHash, mP))) {
	  if (mP->serviceName[0] == '\0') {
	      printf("%s:\n", "<empty>");
	  } else {
	      printf("%s:\n", mP->serviceName);
	  }
	  for(index = 0; index < 128; index++) {
	      printf("%8u ",mP->sizes16[index]);
	      if ((index & 0x7) == 0x7) {
		  printf("\n");
	      }
	  }
	  printf("\n\n");
	  for(index = 0; index < 64; index++) {
	      printf("%8u ",mP->sizesk[index]);
	      if ((index & 0x7) == 0x7) {
		  printf("\n");
	      }
	  }
	  printf("\n\n");
      }
  }

/*
 * TranServiceHash - performs hashing on the transaction service name.  This
 * code is used in StartTran and in the transaction monitoring package.
 */
static unsigned TranServiceHash(const char *p)
  {
      unsigned g, index, hash;

      for (hash = 0, index = 0; index < 60 && p != '\0'; p++, index++) {
	  hash = (hash << 4) + *p;
	  if ((g = (hash & 0xf0000000)) != 0) {
	      hash ^= g>>24;
	      hash ^= g;
	  }
      }

      return hash;
  }
#endif
