/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: osi_lock_pthread.c,v $
 * Revision 1.1.128.1  1996/10/02  18:10:51  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:44:58  damon]
 *
 * Revision 1.1.123.2  1994/06/09  14:17:01  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:29:12  annie]
 * 
 * Revision 1.1.123.1  1994/02/04  20:26:32  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:16:25  devsrc]
 * 
 * Revision 1.1.121.1  1993/12/07  17:31:06  jaffe
 * 	New File from Transarc 1.0.3a
 * 	[1993/12/02  21:32:17  jaffe]
 * 
 * $EndLog$
 */

/* Copyright (C) 1994, 1993 Transarc Corporation - All rights reserved. */

/*
 * User-level lock package implementation based on pthreads.
 */
#ifndef KERNEL

#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/lock.h>
#include <dcedfs/osi_intr.h>
#include <pthread.h>

RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/osi/osi_lock_pthread.c,v 1.1.128.1 1996/10/02 18:10:51 damon Exp $")

#ifdef OSI_TRACE_SYNC
/* This wouldn't really work if done in a build from scratch due to the
 * dependency loop between the osi and icl layers.  However, for dianostic
 * purposes this should be fine. */
 
#include <dcedfs/icl.h>
#include <dcedfs/episode/ethr_trace.h>
#include <dcedfs/episode/epi_thread.h>

extern struct icl_set *osi_lockInitSet;
extern struct icl_set *osi_lockSet;

#define traced_mutex_init(m,s) epi_MutexInit(m,s)
#define traced_mutex_lock(m) epi_MutexLock(m)
#define traced_mutex_unlock(m) epi_MutexUnlock(m)
#define traced_cond_init(cv,s) epi_CondInit(cv,s)
#define traced_cond_wait(cv,m) epi_CondWait(cv,m)
#define traced_cond_signal(cv) epi_CondSignal(cv)
#define traced_cond_broadcast(cv) epi_CondBroadcast(cv)

#else
/* If tracing is off, just turn these procedures into straight through calls
 * and use real pthread calls instead of epi wrappers. */

#undef icl_Trace1
#undef icl_Trace2
#define icl_Trace1(a, b, c1, d1) /* */
#define icl_Trace2(a, b, c1, d1, c2, d2) /* */

#define traced_mutex_init(m,s) pthread_mutex_init(m, pthread_mutexattr_default)
#define traced_mutex_lock(m) pthread_mutex_lock(m)
#define traced_mutex_unlock(m) pthread_mutex_unlock(m)
#define traced_cond_init(cv,s) pthread_cond_init(cv, pthread_condattr_default)
#define traced_cond_wait(cv,m) pthread_cond_wait(cv,m)
#define traced_cond_broadcast(cv) pthread_cond_broadcast(cv)
#define traced_cond_signal(cv) pthread_cond_signal(cv)

#endif

#define traced_mutex_destroy(m) pthread_mutex_destroy(m)
#define traced_cond_destroy(cv) pthread_cond_destroy(cv)

static pthread_cond_t lock_cond[LOCK_HASH_CONDS];
static pthread_mutex_t lock_mutex[LOCK_HASH_CONDS];
static long lock_debugAddr[LOCK_HASH_CONDS];
static int lock_debugType[LOCK_HASH_CONDS];
static int lock_debugCount[LOCK_HASH_CONDS];

static pthread_cond_t sleep_cond[LOCK_HASH_CONDS];
static pthread_mutex_t sleep_mutex[LOCK_HASH_CONDS];
static long sleep_debugAddr[LOCK_HASH_CONDS];
static int sleep_debugType[LOCK_HASH_CONDS];
static int sleep_debugCount[LOCK_HASH_CONDS];

pthread_once_t initDone = pthread_once_init;

#define CHECKINIT()	pthread_once(&initDone, lock_PackageInit)

static void
lock_PackageInit(void)
{
    int i;

#ifdef OSI_TRACE_SYNC
    osi_InitLockTrace_r();
#endif

    for (i = 0; i < LOCK_HASH_CONDS; i++) {
#ifdef OSI_TRACE_SYNC
	char lmname[20];
	char lcname[20];
	char cmname[20];
	char ccname[20];
	sprintf (lmname, "dlock mutex #%d", i);
	sprintf (lcname, "dlock cond #%d", i);
	sprintf (cmname, "dcond mutex #%d", i);
	sprintf (ccname, "dcond cond #%d", i);
#endif
	traced_mutex_init(&lock_mutex[i], lmname);
	traced_cond_init(&lock_cond[i], lcname);
	traced_mutex_init(&sleep_mutex[i], cmname);
	traced_cond_init(&sleep_cond[i], ccname);
    }
}

int
lock_Init(osi_dlock_t *ap)
{
    CHECKINIT();
    ap->wait_states = 0;
    ap->excl_locked = 0;
    ap->readers_reading = 0;
    return 0;
}

int
lock_InitDesc(osi_dlock_t *ap, const char *desc)
{
    int code = lock_Init(ap);
    icl_Trace2(osi_lockInitSet, TR_ETHR_DLOCK_INIT,
	       ICL_TYPE_STRING, desc, ICL_TYPE_POINTER, ap);
    return code;
}

void lock_Destroy(osi_dlock_t *ap)
{
    (void) lock_Init(ap);
}

static long
lock_HashAddr(long a)
{
    long tval = ((a >> 8) + (a >> 16)) & (LOCK_HASH_CONDS - 1);
    return tval;
}

void
lock_ObtainWrite(osi_dlock_t *alock)
{
    long hashVal = lock_HashAddr((long)alock);
    int blocked;

    CHECKINIT();
    traced_mutex_lock(&lock_mutex[hashVal]);
    if (blocked = !(alock->excl_locked == 0 && alock->readers_reading == 0)) {
	icl_Trace1(osi_lockSet, TR_ETHR_MUTEX_BLOCK,
		   ICL_TYPE_POINTER, alock);
    }
    while (1) {
	if (alock->excl_locked == 0 && alock->readers_reading == 0) {
	    alock->excl_locked = WRITE_LOCK;
	    break;
	}
	/* otherwise, someone's got the lock right now, so we're going
	 * to sleep for a bit.
	 */
	alock->wait_states |= WRITE_LOCK;
	/* sleep, and atomically drop the lock */
	lock_debugCount[hashVal]++;
	lock_debugType[hashVal] = 1;
	lock_debugAddr[hashVal] = (long) alock;
	traced_cond_wait(&lock_cond[hashVal], &lock_mutex[hashVal]);
	--lock_debugCount[hashVal];
    }
    if (blocked)
	icl_Trace1(osi_lockSet, TR_ETHR_MUTEX_HOLD,
		   ICL_TYPE_POINTER, alock);
    else
	icl_Trace1(osi_lockSet, TR_ETHR_MUTEX_GRAB,
		   ICL_TYPE_POINTER, alock);
    traced_mutex_unlock(&lock_mutex[hashVal]);
}

int
lock_ObtainWriteNoBlock(osi_dlock_t *alock)
{
    int	rtnVal = 0;
    long hashVal = lock_HashAddr((long)alock);

    CHECKINIT();

    /* this should be a short-term lock, so we don't mind waiting here */
    pthread_mutex_lock(&lock_mutex[hashVal]);

    if (alock->excl_locked == 0 && alock->readers_reading == 0) {
	alock->excl_locked = WRITE_LOCK;
	rtnVal = WRITE_LOCK;
	icl_Trace1(osi_lockSet, TR_ETHR_MUTEX_GRAB,
		   ICL_TYPE_POINTER, alock);
    }

    pthread_mutex_unlock(&lock_mutex[hashVal]);
    return rtnVal;
}

void
lock_ObtainRead(osi_dlock_t *alock)
{
    long hashVal = lock_HashAddr((long)alock);
    int blocked;

    CHECKINIT();
    traced_mutex_lock(&lock_mutex[hashVal]);
    if (blocked = !((alock->excl_locked & WRITE_LOCK) == 0)) {
	icl_Trace1(osi_lockSet, TR_ETHR_MUTEX_BLOCK,
		   ICL_TYPE_POINTER, alock);
    }
    while (1) {
	if ((alock->excl_locked & WRITE_LOCK) == 0) {
	    alock->readers_reading++;
	    break;
	}
	/* otherwise, we're going to sleep */
	alock->wait_states |= READ_LOCK;
	lock_debugCount[hashVal]++;
	lock_debugType[hashVal] = 0;
	lock_debugAddr[hashVal] = (long) alock;
	traced_cond_wait(&lock_cond[hashVal], &lock_mutex[hashVal]);
	--lock_debugCount[hashVal];
    }
    traced_mutex_unlock(&lock_mutex[hashVal]);
    if (blocked)
	icl_Trace1(osi_lockSet, TR_ETHR_MUTEX_HOLD,
		   ICL_TYPE_POINTER, alock);
    else
	icl_Trace1(osi_lockSet, TR_ETHR_MUTEX_GRAB,
		   ICL_TYPE_POINTER, alock);
}

int
lock_ObtainReadNoBlock(osi_dlock_t *alock)
{
    int	rtnVal = 0;
    long hashVal = lock_HashAddr((long)alock);

    CHECKINIT();

    /* this should be a short_term lock, so we don't mind waiting here */
    pthread_mutex_lock(&lock_mutex[hashVal]);

    if ((alock->excl_locked & WRITE_LOCK) == 0) {
	rtnVal = ++alock->readers_reading;
    }

    pthread_mutex_unlock(&lock_mutex[hashVal]);
    return rtnVal;
}

static int lock_contentionPrintfs = 0;

void
lock_ReleaseWrite(osi_dlock_t *alock)
{
    long hashVal = lock_HashAddr((long)alock);
    void *broadcast = NULL;
    int waiters = lock_debugCount[hashVal];

    traced_mutex_lock(&lock_mutex[hashVal]);
    if (waiters != lock_debugCount[hashVal]) {
	if (lock_contentionPrintfs)
	    printf ("Heavy contention on %#x called from %#x\n",
		    alock, osi_caller());
    }
    if (!(alock->excl_locked & (WRITE_LOCK | SHARED_LOCK)))
	panic("lock_ReleaseWrite: lock is not shared- or write-locked");
    /* clear both bits so that we can use this to release an unboosted
     * shared lock, too.
     */
    alock->excl_locked &= ~(WRITE_LOCK | SHARED_LOCK);
    if (alock->wait_states) {
	alock->wait_states = 0;
	/* wakeup anyone who might have been waiting for this */
	broadcast = &lock_cond[hashVal];
    }
    if (lock_debugCount[hashVal] > 10) {
	if (lock_contentionPrintfs)
	    printf ("Many waiters on %#x called from %#x\n",
		    alock, osi_caller());
    }
    icl_Trace1(osi_lockSet, TR_ETHR_MUTEX_RELE,
	       ICL_TYPE_POINTER, alock);
    traced_mutex_unlock(&lock_mutex[hashVal]);
    if (broadcast != NULL)		/* after dropping the mutex... */
	traced_cond_broadcast(broadcast);
}

void
lock_ReleaseRead(osi_dlock_t *alock)
{
    long hashVal = lock_HashAddr((long)alock);
    void *broadcast = NULL;

    traced_mutex_lock(&lock_mutex[hashVal]);
    /* clear both bits so that we can use this to release an unboosted
     * shared lock, too.
     */
    if (alock->readers_reading <= 0)
	panic("lock_ReleaseRead: lock is not read-locked");
    if ((--alock->readers_reading) == 0 && alock->wait_states != 0) {
	alock->wait_states = 0;
	/* wakeup anyone who might have been waiting for this */
	broadcast = &lock_cond[hashVal];
    }
    icl_Trace1(osi_lockSet, TR_ETHR_MUTEX_RELE,
	       ICL_TYPE_POINTER, alock);
    traced_mutex_unlock(&lock_mutex[hashVal]);
    if (broadcast != NULL)		/* after dropping the mutex... */
	traced_cond_broadcast(broadcast);
}

void
lock_ObtainShared(osi_dlock_t *alock)
{
    long hashVal = lock_HashAddr((long)alock);

    CHECKINIT();
    traced_mutex_lock(&lock_mutex[hashVal]);
    while (1) {
	if (alock->excl_locked == 0) {	/* compatible with readers */
	    alock->excl_locked = SHARED_LOCK;
	    break;
	}
	/* otherwise, someone's got the lock right now, so we're going
	 * to sleep for a bit.
	 */
	alock->wait_states |= SHARED_LOCK;
	lock_debugCount[hashVal]++;
	lock_debugType[hashVal] = 2;
	lock_debugAddr[hashVal] = (long) alock;
	/* sleep, and atomically drop the lock */
	traced_cond_wait(&lock_cond[hashVal], &lock_mutex[hashVal]);
	--lock_debugCount[hashVal];
    }
    traced_mutex_unlock(&lock_mutex[hashVal]);
}

int
lock_ObtainSharedNoBlock(osi_dlock_t *alock)
{
    int	rtnVal = 0;
    long hashVal = lock_HashAddr((long)alock);

    CHECKINIT();

    /* this should be a short-term lock, so we don't mind waiting here */
    traced_mutex_lock(&lock_mutex[hashVal]);

    if (alock->excl_locked == 0) {	/* compatible with readers */
	alock->excl_locked = SHARED_LOCK;
	rtnVal = SHARED_LOCK;
    }

    traced_mutex_unlock(&lock_mutex[hashVal]);
    return rtnVal;
}

/* lock_ReleaseShared is a macro that simply calls lock_ReleaseWrite */

/* this call can block waiting for all of the readers to clear out */
void
lock_UpgradeSToW(osi_dlock_t *alock)
{
    long hashVal = lock_HashAddr((long)alock);

    traced_mutex_lock(&lock_mutex[hashVal]);
    while (1) {
	/* don't forget that we already have a SHARED lock, so we know
	 * that there aren't any other compatible exclusive-style locks.
	 * Thus, all we have to do is wait for the readers to vanish; no
	 * one else can get going during this time.
	 *
	 * Even though we've released a SHARED lock, a write lock is
	 * incompatible with all the locks that a SHARED lock is, so
	 * there should't be anyone new who should be woken up now.
	 */
	if (!(alock->excl_locked & SHARED_LOCK))
	    panic("lock_UpgradeSToW: lock is not shared-locked");
	if (alock->readers_reading == 0) {
	    /* we can do it right now */
	    alock->excl_locked &= ~SHARED_LOCK;
	    alock->excl_locked |= WRITE_LOCK;
	    break;
	}
	/* otherwise, we indicate that someone's waiting for a write lock,
	 * and sleep until the readers disappear.
	 */
	alock->wait_states |= WRITE_LOCK;
	traced_cond_wait(&lock_cond[hashVal], &lock_mutex[hashVal]);
    }
    traced_mutex_unlock(&lock_mutex[hashVal]);
}

int
lock_UpgradeSToWNoBlock(osi_dlock_t *alock)
{
  int	rtnVal = 0;
  long	hashVal = lock_HashAddr((long)alock);

  /* this should be a short-term lock, so we don't mind waiting here */
  traced_mutex_lock(&lock_mutex[hashVal]);

  /* don't forget that we already have a SHARED lock, so we know
   * that there aren't any other compatible exclusive-style locks.
   * Thus, all we have to do is wait for the readers to vanish; no
   * one else can get going during this time.
   *
   * Even though we've released a SHARED lock, a write lock is
   * incompatible with all the locks that a SHARED lock is, so
   * there should't be anyone new who should be woken up now.
   */
  if (!(alock->excl_locked & SHARED_LOCK))
      panic("lock_UpgradeSToWNoBlock: lock is not shared-locked");
  if (alock->readers_reading == 0) {
    /* we can do it right now */
    alock->excl_locked &= ~SHARED_LOCK;
    alock->excl_locked |= WRITE_LOCK;

    rtnVal = WRITE_LOCK;
  }

  traced_mutex_unlock(&lock_mutex[hashVal]);
  return rtnVal;
}

/* simply convert a write lock down to a shared lock */
void
lock_ConvertWToS(osi_dlock_t *alock)
{
    long hashVal = lock_HashAddr((long)alock);

    traced_mutex_lock(&lock_mutex[hashVal]);
    if (!(alock->excl_locked & WRITE_LOCK))
	panic("lock_ConvertWToS: lock is not write-locked");
    alock->excl_locked |= SHARED_LOCK;
    alock->excl_locked &= ~WRITE_LOCK;
    if (alock->wait_states) {
	alock->wait_states = 0;
	/* wake up anyone who might have been waiting for this */
	traced_cond_broadcast(&lock_cond[hashVal]);
    }
    traced_mutex_unlock(&lock_mutex[hashVal]);
}

/* simply convert a write lock down to a shared lock */
void
lock_ConvertWToR(osi_dlock_t *alock)
{
    long hashVal = lock_HashAddr((long)alock);

    traced_mutex_lock(&lock_mutex[hashVal]);
    if (!(alock->excl_locked & WRITE_LOCK))
	panic("lock_ConvertWToR: lock is not write-locked");
    alock->readers_reading++;
    alock->excl_locked &= ~WRITE_LOCK;
    if (alock->wait_states) {
	alock->wait_states = 0;
	/* wakeup anyone who might have been waiting for this */
	traced_cond_broadcast(&lock_cond[hashVal]);
    }
    traced_mutex_unlock(&lock_mutex[hashVal]);
}

/* simply convert a write lock down to a shared lock */
void
lock_ConvertSToR(osi_dlock_t *alock)
{
    long hashVal = lock_HashAddr((long)alock);

    traced_mutex_lock(&lock_mutex[hashVal]);
    if (!(alock->excl_locked & SHARED_LOCK))
	panic("lock_ConvertSToR: lock is not shared-locked");
    alock->readers_reading++;
    alock->excl_locked &= ~SHARED_LOCK;
    if (alock->wait_states) {
	alock->wait_states = 0;
	/* wakeup anyone who might have been waiting for this */
	traced_cond_broadcast(&lock_cond[hashVal]);
    }
    traced_mutex_unlock(&lock_mutex[hashVal]);
}

/* this function is allowed to return spuriously */
int
osi_Sleep(opaque addr)
{
    int hashVal = lock_HashAddr((long)addr);

    CHECKINIT();
    traced_mutex_lock(&sleep_mutex[hashVal]);
    sleep_debugCount[hashVal]++;
    sleep_debugAddr[hashVal] = (long) addr;
    sleep_debugType[hashVal] = 3;
    traced_cond_wait(&sleep_cond[hashVal], &sleep_mutex[hashVal]);
    sleep_debugCount[hashVal]--;
    traced_mutex_unlock(&sleep_mutex[hashVal]);
    return 0;
}

int
osi_SleepInterruptably(opaque addr)
{
    return osi_Sleep(addr);
}

int
osi_Wakeup(opaque addr)
{
    int hashVal = lock_HashAddr((long)addr);

    CHECKINIT();
    icl_Trace1(osi_lockSet, TR_ETHR_COND_BROADCAST,
	       ICL_TYPE_POINTER, addr);
    traced_mutex_lock(&sleep_mutex[hashVal]);
    traced_cond_broadcast(&sleep_cond[hashVal]);
    traced_mutex_unlock(&sleep_mutex[hashVal]);
    return 0;
}

void
osi_SleepR(opaque addr, osi_dlock_t *alock)
{
    int hashVal = lock_HashAddr((long)addr);

    /* this function must work atomically w.r.t. sleep_Wakeups directed
     * to the same address.  That is, after the lock is released,
     * we must assume that a wakeup could be issued, and if we lose
     * the wakeup, we're violating a guarantee that we're trying to make.
     *
     * During the period between the lock_ReleaseRead and the time we
     * actually get queued for sleeping, we hold the mutex sleep_mutex[hashVal],
     * which prevents a wakeup from being issued until we're asleep.
     */

    traced_mutex_lock(&sleep_mutex[hashVal]);
    sleep_debugCount[hashVal]++;
    sleep_debugAddr[hashVal] = (long) addr;
    sleep_debugType[hashVal] = 0;
    lock_ReleaseRead(alock);
    traced_cond_wait(&sleep_cond[hashVal], &sleep_mutex[hashVal]);
    sleep_debugCount[hashVal]--;
    traced_mutex_unlock(&sleep_mutex[hashVal]);
}

void
osi_SleepS(opaque addr, osi_dlock_t *alock)
{
    int hashVal = lock_HashAddr((long)addr);

    /* this function must work atomically w.r.t. lock_Wakeups directed
     * to the same address.  That is, after the lock is released,
     * we must assume that a wakeup could be issued, and if we lose
     * the wakeup, we're violating a guarantee that we're trying to make.
     *
     * During the period between the lock_ReleaseShared and the time we
     * actually get queued for sleeping, we hold the mutex lock_mutex[hashVal],
     * which prevents a wakeup from being issued until we're asleep.
     */

    traced_mutex_lock(&sleep_mutex[hashVal]);
    sleep_debugCount[hashVal]++;
    sleep_debugAddr[hashVal] = (long) addr;
    sleep_debugType[hashVal] = 2;
    lock_ReleaseShared(alock);
    traced_cond_wait(&sleep_cond[hashVal], &sleep_mutex[hashVal]);
    sleep_debugCount[hashVal]--;
    traced_mutex_unlock(&sleep_mutex[hashVal]);
}

void
osi_SleepW(opaque addr, osi_dlock_t *alock)
{
    int hashVal = lock_HashAddr((long)addr);

    /* this function must work atomically w.r.t. lock_Wakeups directed
     * to the same address.  That is, after the lock is released,
     * we must assume that a wakeup could be issued, and if we lose
     * the wakeup, we're violating a guarantee that we're trying to make.
     *
     * During the period between the lock_ReleaseWrite and the time we
     * actually get queued for sleeping, we hold the mutex lock_mutex[hashVal],
     * which prevents a wakeup from being issued until we're asleep.
     */

    traced_mutex_lock(&sleep_mutex[hashVal]);
    sleep_debugCount[hashVal]++;
    sleep_debugAddr[hashVal] = (long) addr;
    sleep_debugType[hashVal] = 2;
    lock_ReleaseWrite(alock);
    icl_Trace1(osi_lockSet, TR_ETHR_COND_WAIT,
	       ICL_TYPE_POINTER, addr);
    traced_cond_wait(&sleep_cond[hashVal], &sleep_mutex[hashVal]);
    icl_Trace1(osi_lockSet, TR_ETHR_COND_WAKE,
	       ICL_TYPE_POINTER, addr);
    sleep_debugCount[hashVal]--;
    traced_mutex_unlock(&sleep_mutex[hashVal]);
}

int
osi_SleepRI(opaque addr, osi_dlock_t *alock)
{
    osi_SleepR(addr, alock);
    return 0;
}

int
osi_SleepSI(opaque addr, osi_dlock_t *alock)
{
    osi_SleepS(addr, alock);
    return 0;
}

int
osi_SleepWI(opaque addr, osi_dlock_t *alock)
{
    osi_SleepW(addr, alock);
    return 0;
}

/*
 * The following routines are implemented only in user space. These routines
 * are similar to the sleep routine above except that they wakeup once the
 * wait time expires, if they haven't woken up already. To distinguish
 * the case between another thread waking up the calling thread and the time
 * limit expiration, the timedOut variable is set to zero if this thread is
 * woken up and is set to a non-zero value if the time limit expires. Also,
 * note that absWaitTime should be set to the absolute time when the thread
 * should wakeup if it hasn't woken up already.
 */
void
osi_TimedSleepR(opaque addr, osi_dlock_t *alock, struct timespec *absWaitTime,
		int *timedOut)
{
    int hashVal = lock_HashAddr((long)addr);

    /* this function must work atomically w.r.t. lock_Wakeups directed
     * to the same address.  That is, after the lock is released,
     * we must assume that a wakeup could be issued, and if we lose
     * the wakeup, we're violating a guarantee that we're trying to make.
     *
     * During the period between the afslk_ReleaseRead and the time we
     * actually get queued for sleeping, we hold the mutex lock_mutex[hashVal],
     * which prevents a wakeup from being issued until we're asleep.
     */

    traced_mutex_lock(&sleep_mutex[hashVal]);
    sleep_debugCount[hashVal]++;
    sleep_debugAddr[hashVal] = (long) addr;
    sleep_debugType[hashVal] = 0;
    lock_ReleaseRead(alock);
    if (pthread_cond_timedwait(&sleep_cond[hashVal], &sleep_mutex[hashVal],
			      absWaitTime) == -1)
      *timedOut=(errno==EAGAIN);
    else *timedOut=0;
    sleep_debugCount[hashVal]--;
    traced_mutex_unlock(&sleep_mutex[hashVal]);
}

void
osi_TimedSleepS(opaque addr, osi_dlock_t *alock, struct timespec *absWaitTime,
		int *timedOut)
{
    int hashVal = lock_HashAddr((long)addr);

    /* this function must work atomically w.r.t. lock_Wakeups directed
     * to the same address.  That is, after the lock is released,
     * we must assume that a wakeup could be issued, and if we lose
     * the wakeup, we're violating a guarantee that we're trying to make.
     *
     * During the period between the afslk_ReleaseShared and the time we
     * actually get queued for sleeping, we hold the mutex lock_mutex[hashVal],
     * which prevents a wakeup from being issued until we're asleep.
     */

    traced_mutex_lock(&sleep_mutex[hashVal]);
    sleep_debugCount[hashVal]++;
    sleep_debugAddr[hashVal] = (long) addr;
    sleep_debugType[hashVal] = 2;
    lock_ReleaseShared(alock);
    if (pthread_cond_timedwait(&sleep_cond[hashVal], &sleep_mutex[hashVal],
			       absWaitTime) == -1)
      *timedOut=(errno == EAGAIN);
    else *timedOut=0;
    sleep_debugCount[hashVal]--;
    traced_mutex_unlock(&sleep_mutex[hashVal]);
}

void
osi_TimedSleepW(opaque addr, osi_dlock_t *alock, struct timespec *absWaitTime,
		int *timedOut)
{
    int hashVal = lock_HashAddr((long)addr);

    /* this function must work atomically w.r.t. lock_Wakeups directed
     * to the same address.  That is, after the lock is released,
     * we must assume that a wakeup could be issued, and if we lose
     * the wakeup, we're violating a guarantee that we're trying to make.
     *
     * During the period between the afslk_ReleaseWrite and the time we
     * actually get queued for sleeping, we hold the mutex lock_mutex[hashVal],
     * which prevents a wakeup from being issued until we're asleep.
     */

    traced_mutex_lock(&sleep_mutex[hashVal]);
    sleep_debugCount[hashVal]++;
    sleep_debugAddr[hashVal] = (long) addr;
    sleep_debugType[hashVal] = 2;
    lock_ReleaseWrite(alock);
    if (pthread_cond_timedwait(&sleep_cond[hashVal], &sleep_mutex[hashVal],
			      absWaitTime) == -1)
      *timedOut=(errno == EAGAIN);
    else *timedOut=0;
    sleep_debugCount[hashVal]--;
    traced_mutex_unlock(&sleep_mutex[hashVal]);
}
#endif /* !KERNEL */
