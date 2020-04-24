/* epis_reserve.c */

/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* 
 * HISTORY
 * $Log: epis_reserve.c,v $
 * Revision 1.1.46.1  1996/10/02  17:29:03  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:34:50  damon]
 *
 * Revision 1.1.41.2  1994/06/09  14:03:58  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:38:49  annie]
 * 
 * Revision 1.1.41.1  1994/02/04  20:15:40  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:11:32  devsrc]
 * 
 * Revision 1.1.39.1  1993/12/07  17:21:16  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  15:00:51  jaffe]
 * 
 * $EndLog$
 */
/*
 * (C) Copyright 1994, 1992 Transarc Corporation - All Rights Reserved
 */

/* this is cheating */
#define DEBUG_THIS_FILE OSI_DEBUG_LOCK

#include <dcedfs/param.h>
#include <dcedfs/osi.h>
#include <dcedfs/stds.h>
#include <dcedfs/debug.h>

#include "fifo.h"
#include "epi_thread.h"
#include "reserve.h"

RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/episode/tools/epis_reserve.c,v 1.1.46.1 1996/10/02 17:29:03 damon Exp $")

static void CheckReservation _TAKES((IN struct epis_ReservationData *));
static void Reserve _TAKES((IN struct epis_ReservationData *, IN int,
			    IN epi_mutex_t *));
static unsigned LargestWaiter _TAKES((IN struct epis_ReservationData *));

/*
 * epis_InitReservationData - intialize a reservation data structure.
 *
 * locks - none required; acquires the reservation lock itself.  Note that
 * this code does not protect against re-initialization of the data structure.
 */
/* EXPORT */
void epis_InitReservationData(resdP,initialAllocation,allocFunc)
  IN struct epis_ReservationData *resdP;
  IN unsigned initialAllocation;
  IN void * (*allocFunc)();
{
    struct epis_ResWaitInfo *waitP;
    int code;

    epi_mutex_init(&resdP->resLock, "reservation");
    epi_mutex_lock(&resdP->resLock);
    FIFO_INIT(&resdP->waiters, offsetof(struct epis_ResWaitInfo, thread));
    code = epis_AdjustReservationUnitsWL(resdP,initialAllocation,
					 &resdP->resLock);
    afsl_Assert(code == initialAllocation);
    resdP->units = initialAllocation;
    resdP->alloc = allocFunc;
    epi_mutex_unlock(&resdP->resLock);
}

/*
 * epis_AdjustReservationWL -- adjust the total number of units available to
 *     this resource.  This allows the allocator to gain even more resource
 *     from an external source, if possible.
 *
 *     If the call is to reduce the size of the total allocation, this call
 *     blocks until it has adjusted the downward size as appropriate.
 *
 *     This call will return the number of units adjusted; it can return fewer
 *     units than requested.
 *
 * LOCKS USED -- passed in.  Must be write locked.  This call can sleep, and
 *     will drop and reobtain the lock, if adjust < 0.
 */
/* EXPORT */
int epis_AdjustReservationUnitsWL(resdP, adjust, lockp)
  IN struct epis_ReservationData *resdP;
  IN int adjust;
  IN epi_mutex_t *lockp;
{
    int max_res;
    int grab;

    afsl_Assert(resdP && epi_mutex_held(lockp) &&
		(((int) resdP->totalUnits + adjust) >= 0));

    if (adjust >= 0) {

	/* Increasing the pool size always succeeds without sleeping. */

	resdP->totalUnits += adjust;
	resdP->units += adjust;
	return adjust;
    }

    /*
     * First, we must find out what the largest, current outstanding
     * reservation request is - we cannot force the system into a position
     * where any thread permanently blocks - or panics.
     */
    max_res = LargestWaiter(resdP);

    /* Make amount to remove from pool positive for easier thinking.  Calculate
     * the amount we can get safely without sleeping.  If there is any do it
     * and return. */

    grab = -adjust;
    grab = MIN (grab, resdP->units);
    grab = MIN (grab, resdP->totalUnits - max_res);
    if (grab > 0) {
	resdP->totalUnits -= grab;
	resdP->units -= grab;
	return -grab;			/* preserve sign of input argument. */
    }

    /* Otherwise we must sleep.  There are two cases: if we need more than
     * are available then we can just try to reserve the space.  Otherwise
     * there are waiters that would be left out of water if we remove this
     * much space so we must wait for the waiters. */

    grab = -adjust;
    if (grab > resdP->units) {

	/* Reserving the desired amount of space will sleep.  But we still
	 * can't just remove the space from circulation since there may be
	 * other waiters, so the simplest thing to do is return and let our
	 * caller try again. */

	Reserve(resdP, grab, lockp);	/* sleep until available */
	Reserve(resdP, -grab, lockp);	/* now give it back */

    } else {

	/* We must wait for some waiter.  We could search for max_res guy but
	 * that's extra trouble now. */

	struct epis_ResWaitInfo *waitP = (struct epis_ResWaitInfo *)
	    FIFO_LookAtOldest(&resdP->waiters,
			      offsetof(struct epis_ResWaitInfo, thread));
	afsl_Assert(waitP && (waitP->need > 0));
	osi_SleepPrint(("Adjust: %d units (at 0x%x, lock 0x%x)",
			adjust,resdP,lockp));
	epi_cond_wait (&waitP->cv, lockp);
    }

    /* No adjustment made, we just slept. */

    return 0;
}

/*
 * epis_AdjustReservationUnits - version of epis_AdjustReservationUnitsWL
 * which uses the reservation lock to synchronize activity.
 *
 * c.f. epis_AdjustReservationUnitsWL for more information.  Notable exception
 * here is that no lock is required coming in to this routine.  Do not
 * mix this interface with the *WL interfaces because the locking will not
 * be correct if you do.
 */
/* EXPORT */
int epis_AdjustReservationUnits(resdP,units)
  IN struct epis_ReservationData *resdP;
  IN int units;
{
    int code;

    epi_mutex_lock(&resdP->resLock);
    code = epis_AdjustReservationUnitsWL(resdP,units, &resdP->resLock);
    epi_mutex_unlock(&resdP->resLock);
    return code;
}

/*
 * epis_ReserveWL - release or acquire a reservation.  It returns 0 if
 * the reservation failed, 1 if it succeeded. If sleep is set, the only
 * reason 0 is returned is because there will never be enough of the object
 * to satisfy the request.
 *
 * In all cases, allocObj is set upon return.
 *
 * locks - passed in parameter, must be locked. This
 * call can sleep if allocAmount > 0 and sleep is non-zero.
 */
/* EXPORT */
int epis_ReserveWL(resdP, allocAmount, sleep, allocObj, lockp)
  IN struct epis_ReservationData *resdP;
  IN int allocAmount;
  IN int sleep;
  INOUT opaque *allocObj;
  IN epi_mutex_t *lockp;
{
    int code = 1;

    afsl_Assert(epi_mutex_held(lockp));
    if (allocAmount > 0) {
	if ((allocAmount > (int)resdP->totalUnits) ||
	    ((allocAmount > (int)resdP->units) && !sleep)) {
	    /*
	     * either there will never be enough, or we'd have to sleep
	     * now to satisfy the request, but caller asked for no sleep
	     */
	    if (sleep) afsl_Assert(allocAmount >(int) resdP->totalUnits);
	    else afsl_Assert(allocAmount > (int) resdP->units);
	    code = 0;
	    if (allocObj) allocObj = 0;
	} else {
	    /* Go for it! */
	    Reserve(resdP, allocAmount, lockp);
	    /* call allocator function */
	    if (allocObj)
		allocObj = (resdP->alloc)?(*resdP->alloc)(allocAmount) : 0;
	}
    } else {
	Reserve(resdP, allocAmount, lockp);
	if (allocObj) *allocObj = (opaque)0;
    }
    afsl_Assert(resdP->units <= resdP->totalUnits);
    return code;
}

/*
 * epis_Reserve - version of epis_ReserveWL which does locking for caller
 *
 * locks - none required; acquires the reservation structure lock.  This
 * interface is mutually exclusive with those callers who use the *WL
 * calls directly.
 */
/* EXPORT */
int epis_Reserve(resdP,allocAmount,sleep,allocObj)
  IN struct epis_ReservationData *resdP;
  IN int allocAmount;
  IN int sleep;
  INOUT opaque *allocObj;
{
    int code;

    epi_mutex_lock(&resdP->resLock);
    code = epis_ReserveWL(resdP,allocAmount, sleep, allocObj,
			  &resdP->resLock);
    epi_mutex_unlock(&resdP->resLock);
    return code;
}

/*
 * epis_QueryUnusedUnitsWL - return # of units not in use and not needed
 * to resolve a pending request.
 *
 * locks - necessary lock is assumed to be held by the caller.  Since this
 * call does not sleep, it is sufficient to guarantee that the lock is
 * properly held.  This corresponds to the *WL interface.  For internal
 * locking, use the epis_QueryUnusedUnits call.
 */

/* EXPORT */
unsigned epis_QueryUnusedUnitsWL(resdP)
  IN struct epis_ReservationData *resdP;
{
    if (!IsFifoEmpty (&resdP->waiters))
	return 0;			/* existing waiters get priority */
    return (int)resdP->units;
}

/*
 * epis_QueryUnusedUnits - return # of units not in use and not needed
 * to resolve a pending request.
 *
 * locks - none required.  Users of external locks should use the *WL
 * interface.  This interface is the internal-locking version.
 */
/* EXPORT */
unsigned epis_QueryUnusedUnits(resdP)
  IN struct epis_ReservationData *resdP;
{
    unsigned code;

    epi_mutex_lock(&resdP->resLock);
    code = epis_QueryUnusedUnitsWL(resdP);
    epi_mutex_unlock(&resdP->resLock);

    return code;
}

#if 0
/*
 * CheckReservation - walk through the reservation list to determine if any
 * of the requests can be satisfied.  Note that it satisfies them in FIFO
 * order where possible.
 *
 * locks - the reservation structure must be locked by caller.
 */
static void CheckReservation(resdP)
  IN struct epis_ReservationData *resdP;
  {
      struct epis_ResWaitInfo *waitP, *newWaitP;
      long unitCount;

      afsl_Assert(resdP);
      waitP = (struct epis_ResWaitInfo *)
	FIFO_LookAtOldest(&resdP->waiters,
			  offsetof(struct epis_ResWaitInfo, thread));
      /*
       * we only wake up enough sleepers to consume the allocation.  No
       * sense starting a thundering herd.
       */
      unitCount = (long) resdP->units;
      while(waitP) {
	  newWaitP = (struct epis_ResWaitInfo *)
	      FIFO_NextOldest(&resdP->waiters, waitP, &waitP->thread);
	  afsl_Assert(waitP->need <= resdP->totalUnits); /* sanity */
	  if (waitP->need <= unitCount) {
	      /* we can satisfy this request */
	      resdP->units -= waitP->need;
	      unitCount -= waitP->need;
	      waitP->need = 0;		/* tell sleeper he's been reserved */ 
	      FIFO_REMOVE(&resdP->waiters, &waitP->thread);
	      epi_cond_broadcast(&waitP->cv);
	  }
	  waitP = newWaitP;
      }
  }
#endif

/*
 * Reserve -- acquire/release a reservation.
 *
 * LOCKS USED -- the reservation data structure must be locked.  This call can
 *     sleep if acquiring a reservation (allocAmount positive).
 */
static void Reserve(resdP, allocAmount, lockp)
  IN struct epis_ReservationData *resdP;
  IN int allocAmount;
  IN epi_mutex_t *lockp;
{
    struct epis_ResWaitInfo *waitP;

    afsl_Assert(resdP && epi_mutex_held(lockp) &&
		(allocAmount <= (int) resdP->totalUnits) &&
		(-allocAmount <= (int) resdP->totalUnits));

    if (allocAmount > (int) resdP->units) {

	/* Desired reservation exceeds available amount so we must sleep. */

	waitP = osi_Alloc_r(sizeof(struct epis_ResWaitInfo));
	epi_cond_init(&waitP->cv, "reservation wait");
	waitP->need = allocAmount;
	FIFO_ADD(&resdP->waiters, &waitP->thread);
	osi_SleepPrint(("Reserve: %d units (at 0x%x, lock 0x%x, pid 0x%x)",
			allocAmount, resdP, lockp, osi_ThreadID()));

	while(waitP->need) {

	    /* Waker will set waitP->need to zero and remove waitP from the
             * waiters FIFO if we can proceed. */

	    epi_cond_wait (&waitP->cv, lockp);
	}
	osi_SleepPrint(("Reserved %d units (at 0x%x, lock 0x%x, pid 0x%x)",
			allocAmount, resdP, lockp, osi_ThreadID()));
	epi_cond_destroy (&waitP->cv);
	osi_Free_r(waitP, sizeof(struct epis_ResWaitInfo));
    } else {

	/* This is the easy aqcuire or the release case. */

	resdP->units -= allocAmount;
    }
    if ((allocAmount < 0) && !IsFifoEmpty (&resdP->waiters)) {

	/* We have released some reservation and so we must wakeup anyone
         * waiting for a reservation that can now be satisfied.  Presently we
         * wake up enough sleepers to consume the allocation using a first fit
         * type of algorithm. */

	/* There is some suggestion that we should really use strict FIFO so
         * that we always wait until the first sleeper can be satisfied. */
	  
	waitP = FIFO_LookAtOldest(&resdP->waiters,
				  offsetof(struct epis_ResWaitInfo, thread));
	while(waitP) {
	    struct epis_ResWaitInfo *newWaitP =
		FIFO_NextOldest(&resdP->waiters, waitP, &waitP->thread);

	    afsl_Assert(waitP->need <= resdP->totalUnits); /* sanity */
	    if (waitP->need <= resdP->units) {

		/* We can satisfy this request.  So give him the reservation
		 * by decrement units field.  In addition to notifying
		 * sleeper also clear his need field and removing his wait
		 * block from the FIFO.  He must still free it. */

		resdP->units -= waitP->need;
		waitP->need = 0;
		FIFO_REMOVE(&resdP->waiters, &waitP->thread);
		epi_cond_broadcast(&waitP->cv);
	    }
	    waitP = newWaitP;
	}
    }
    afsl_Assert(resdP->units <= resdP->totalUnits);
}

/*
 * LargestWaiter - get the allocation request of the largest waiting process.
 * Return value is the largest waiter.
 *
 * locks - the reservation struct must be locked.
 */
static unsigned LargestWaiter(resdP)
  IN struct epis_ReservationData *resdP;
{
    struct epis_ResWaitInfo *waitP, *newWaitP;
    unsigned max_res = 0;

    afsl_Assert(resdP);

    waitP = FIFO_LookAtOldest(&resdP->waiters,
			      offsetof(struct epis_ResWaitInfo, thread));
    while(waitP) {
	newWaitP = FIFO_NextOldest(&resdP->waiters, waitP, &waitP->thread);
	if (waitP->need > max_res) max_res = waitP->need;
	waitP = newWaitP;
    }

    return max_res;
}
