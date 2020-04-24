/* reserve.h */

/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: reserve.h,v $
 * Revision 1.1.45.1  1996/10/02  17:42:09  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:34:53  damon]
 *
 * Revision 1.1.40.2  1994/06/09  14:04:19  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:39:05  annie]
 * 
 * Revision 1.1.40.1  1994/02/04  20:15:44  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:11:33  devsrc]
 * 
 * Revision 1.1.38.1  1993/12/07  17:21:38  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  15:01:22  jaffe]
 * 
 * $EndLog$
 */
/*
 * (C) Copyright 1994, 1992 Transarc Corporation - All Rights Reserved
 */

#include <dcedfs/lock.h>

struct epis_ResWaitInfo {
    unsigned            need;           /* # of units needed */
    struct fifo_Thread  thread;         /* fifo of these objects */
    epi_cond_t		cv;
};

struct epis_ReservationData {
    epi_mutex_t	resLock;    /* lock on this data structure */
    struct fifo	waiters;    /* anybody waiting for this resource */
    unsigned	units;      /* # of units */
    unsigned	totalUnits; /* total # of units in res. system */
    void	*(*alloc)(); /* allocator function */
};

/*
 * epis_InitReservationData - initialize a reservation data structure.
 *
 * This routine is used to initialize the reservation data structure and
 * set the initial number of units available to potential callers.
 *
 * locking - none performed.  The internal reservation data lock is
 * initialized and acquired.  No new locks are held upon return from
 * this function.
 *
 * The third parameter is a function pointer to a generic allocator function
 * which can optionally be provided by the caller.  If this is set, it will
 * be called upon subsequent calls to epis_Reserve.
 */

extern void epis_InitReservationData(
    IN struct epis_ReservationData *, /* reservation data structure */
    IN unsigned,                      /* initial # of units avail. */
    IN void * (*)(IN long));/* optional allocation func */
/*
 * epis_ReserveWL - reserve a specific # of objects, using the specified
 * lock.
 *
 * This is the fully generalized reservation routine.  It should be used by
 * callers wishing to provide their own lock to be used for synchronization.
 * Since this routine can sleep, this is important to provide locking-safe
 * semantics.
 *
 * For callers not requiring the provision of their own external lock
 * package, they should use the epis_Reserve interface.  Only one interface
 * can be safely used. Do not use both.
 *
 * locks - passed in.  This lock may be dropped during the progress of this
 * call.  Because of this, callers must re-compute state after a successful
 * call.  If a caller wishes to make a "probe" it can be done by not setting
 * the sleep option (int option).  A failure return would indicate the
 * call will sleep.  If the sleep option is set, the only reason for a failure
 * return is because the caller is requesting more units than exist for this
 * resource.
 *
 * If the optional return object field is null, that field is not referenced
 * and the allocation function is not called.
 */
extern int epis_ReserveWL(
    IN struct epis_ReservationData *,	/* reservation data struct */
    IN int,				/* # of units to get/release */
    IN int,				/* sleep true/false */
    INOUT opaque *,			/* opt. returned data obj */
    IN epi_mutex_t *);			/* lock to use */

/*
 * epis_Reserve - internal locking version of epis_ReserveWL
 *
 * EXPORT int epis_Reserve
 * _TAKES((IN struct epis_ReservationData *,  reservation data struct
 *    IN int,                                # of units to get/release
 *    IN int,                                 sleep true/false
 *    INOUT opaque *));                       opt. returned data obj
 *
 * This call uses the reservation data structure's internal lock. If the
 * caller chooses to use this interface, the *WL interface must not be
 * used - otherwise correctness will be compromised.
 */
extern int epis_Reserve(
    IN struct epis_ReservationData *,	/* reservation data struct */
    IN int,				/* # of units to get/release */
    IN int,				/* sleep true/false */
    INOUT opaque *);			/* opt. returned data obj */

extern unsigned epis_QueryUnusedUnitsWL(struct epis_ReservationData *);
extern unsigned epis_QueryUnusedUnits(struct epis_ReservationData *);

/*
 * epis_AdjustReservationunitsWL - adjust the available resource pool.
 *
 * This is the fully generalized reservation management routine.  It should
 * be used by callers wishing to provide their own lock to be used for
 * synchronization.  This routine can sleep, so this interface is used
 * to provide locking-safe semantics.
 *
 * Callers not requiring their own lock be used can use the
 * epis_AdjustReservationUnits which utilizes the reservation structure
 * lock itself.  Regardless of which approach is used, the assumption is
 * that the caller is consistent in the use of locking.
 *
 * locks - passed in.  This lock may be dropped during the progress of this
 * call.  Because of this, callers must re-compute state after this call
 * returns.  Only negative adjustments can sleep.  Positive adjustments
 * never require a state-recomputation.
 *
 * It is the responsibility of the caller to adjust any information used by
 * the caller-provided allocation function.  That function is not called
 * by this routine.
 *
 * This call returns the actual number of units adjusted.  If this number
 * is not the same as the number of units requested, it implies that this call
 * was forced to sleep.  The caller is responsible for recomputing state and
 * calling again if necessary.
 */

extern int epis_AdjustReservationUnitsWL(
    IN struct epis_ReservationData *,	/* reservation data struct. */
    IN int,				/* adjustment amount */
    IN epi_mutex_t *);			/* lock to use */

/*
 * epis_AdjustReservationUnits - internal locking version of
 *                               epis_AdjustReservationUnitsWL
 *
 * This call uses the reservation data structure's internal lock.  If the
 * caller chooses to use this interface, the *WL interface must not be
 * used.  Results otherwise will not be correct.
 */
extern int epis_AdjustReservationUnits(
    IN struct epis_ReservationData *,	/* reservation data struct */
    IN int);				/*  adjustment amount */

/*
 * epis_QueryTotalUnits - return total # of units for this resource.
 *
 * EXPORT unsigned epis_QueryTotalUnits
 *  _TAKES((IN struct epis_ReservationData *)); -- reservation data struct.
 *
 * This macro does not lock the reservation data structure.
 */
#define epis_QueryTotalUnits(resP) ((resP)->totalUnits)

/*
 * epis_QueryFreeUnits - return present # of free units for this resource.
 *
 * EXPORT unsigned epis_QueryFreeUnits
 *  _TAKES((IN struct epis_ReservationData *)); -- reservation data struct.
 *
 * This macro does not lock the reservation data structure.
 */
#define epis_QueryFreeUnits(resP) ((resP)->units)
