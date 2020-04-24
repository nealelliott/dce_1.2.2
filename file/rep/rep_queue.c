/*
 * @OSF_COPYRIGHT@
 */
/*
 * HISTORY
 * $Log: rep_queue.c,v $
 * Revision 1.1.5.1  1996/10/02  18:14:29  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:46:09  damon]
 *
 * $EndLog$
 */

/*
 * (C) Copyright 1994 Transarc Corporation
 * Licensed Materials - Property of Transarc
 * All Rights Reserved.
 */

#include <dcedfs/stds.h>
#include <dcedfs/osi.h>

#include "rep_queue.h"
#include "rep_trace.h"

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/rep/rep_queue.c,v 1.1.5.1 1996/10/02 18:14:29 damon Exp $")

#define RQ_DEADLINE_SLOP	2

typedef struct rep_queue_item {
    struct rep_queue_item*	rqi_next;
    time_t			rqi_deadline;
    unsigned			rqi_flags;
    void*			rqi_data;
} rep_queue_item_t;

#define RQI_FLAG_INUSE	0x1
#define RQI_FLAG_RESET	0x2
#define RQI_FLAG_DELETE	0x4


static rep_queue_item_t*	FindPrev(rep_queue_t*, rep_queue_item_t*);
static void			Insert(rep_queue_t*, rep_queue_item_t*);
static void			LockQueue(rep_queue_t*);
static void			UnlockQueue(rep_queue_t*);
static void			WakeupWaiters(rep_queue_t*);

static struct icl_set*	iclSetP;

void
repq_InitICL(struct icl_set* isP)
{
    iclSetP = isP;
}	/* repq_InitICL */

void
repq_Init(rep_queue_t* rqP)
{
    int	code;

    icl_Trace1(iclSetP, REP_TRACE_REPQ_INIT, ICL_TYPE_POINTER, rqP);

    code = pthread_mutex_init(&rqP->rq_mutex, pthread_mutexattr_default);
    osi_assert(code != -1);

    code = pthread_cond_init(&rqP->rq_hasData, pthread_condattr_default);
    osi_assert(code != -1);

    rqP->rq_head = NULL;
}	/* repq_Init() */


rep_queue_cookie_t
repq_Enter(rep_queue_t* rqP, void* data, time_t deadline)
{
    rep_queue_item_t*	rqiP;

    osi_assert(data != NULL);

    icl_Trace3(iclSetP, REP_TRACE_REPQ_ENTER,
	       ICL_TYPE_POINTER, rqP, ICL_TYPE_POINTER, data,
	       ICL_TYPE_LONG, deadline);

    rqiP = osi_Alloc(sizeof *rqiP);
    osi_assert(rqiP != NULL);
    rqiP->rqi_next = NULL;
    rqiP->rqi_deadline = deadline;
    rqiP->rqi_flags = 0;
    rqiP->rqi_data = data;

    LockQueue(rqP);

    Insert(rqP, rqiP);

    UnlockQueue(rqP);

    icl_Trace1(iclSetP, REP_TRACE_LEAVE_REPQ_ENTER, ICL_TYPE_POINTER, rqiP);

    return (rep_queue_cookie_t)rqiP;
}	/* repq_Enter() */


void
repq_Delete(rep_queue_t* rqP, rep_queue_cookie_t cookie)
{
    rep_queue_item_t**	p;
    rep_queue_item_t*	rqiP = cookie;

    icl_Trace2(iclSetP, REP_TRACE_REPQ_DELETE,
	       ICL_TYPE_POINTER, rqP, ICL_TYPE_POINTER, cookie);

    LockQueue(rqP);

    if (rqiP->rqi_flags & RQI_FLAG_INUSE) {
	/* The item is in use--defer the delete until it is "put" */
	rqiP->rqi_flags |= RQI_FLAG_DELETE;
	icl_Trace0(iclSetP, REP_TRACE_REPQ_DEFERRED_DELETE);
    } else {
	rep_queue_item_t*	prevItemP = FindPrev(rqP, rqiP);

	if (prevItemP == NULL) {
	    /* We're deleting the head */

	    rqP->rq_head = rqiP->rqi_next; /* Splice the item out */

	    if (rqP->rq_head != NULL) {
		/* If there is at least one item left, wakeup the waiters. */
		WakeupWaiters(rqP);
	    }
	} else {
	    /* We're not deleting the head, just splice the item out */
	    prevItemP->rqi_next = rqiP->rqi_next;
	}

	osi_Free(rqiP, sizeof *rqiP);
    }

    UnlockQueue(rqP);

    icl_Trace0(iclSetP, REP_TRACE_LEAVE_REPQ_DELETE);
}	/* repq_Delete() */


void*
repq_Get(rep_queue_t* rqP, rep_queue_cookie_t* outCookieP)
{
    int			code;
    void*		data = NULL;
    rep_queue_item_t*	head;

    icl_Trace1(iclSetP, REP_TRACE_REPQ_GET, ICL_TYPE_POINTER, rqP);

    LockQueue(rqP);

    while (data == NULL) {
	head = rqP->rq_head;

	if (head == NULL) {
	    /*
	     * There is nothing in the queue, so we wait without a wakeup 
	     * time.
	     */
	    icl_Trace0(iclSetP, REP_TRACE_REPQ_GET_WAIT_FOREVER);
	    code = pthread_cond_wait(&rqP->rq_hasData, &rqP->rq_mutex);
	    osi_assert(code != -1);
	} else {
	    time_t	deadline = head->rqi_deadline;
	    time_t	now = osi_Time();

	    if (now < deadline - RQ_DEADLINE_SLOP) {
		struct timespec	wakeupTime;

		/*
		 * We're not close to the deadline of the item at the head
		 * so, wait until the deadline.
		 */

		icl_Trace1(iclSetP, REP_TRACE_REPQ_GET_WAIT_FOR_DEADLINE,
			   ICL_TYPE_LONG, deadline);

		wakeupTime.tv_sec = deadline;
		wakeupTime.tv_nsec = 0;

		code = pthread_cond_timedwait(&rqP->rq_hasData, &rqP->rq_mutex,
					      &wakeupTime);
		osi_assert(code != -1 || errno == EAGAIN);
	    } else {
		/*
		 * The deadline is here or it has past, so we return the
		 * item at the head.
		 */
		data = head->rqi_data;
		rqP->rq_head = head->rqi_next;
		head->rqi_next = NULL;
		head->rqi_flags = RQI_FLAG_INUSE;
		if (outCookieP != NULL)
		    *outCookieP = (rep_queue_cookie_t)head;
	    }
	}
    }

    UnlockQueue(rqP);

    osi_assert(data != NULL);

    icl_Trace2(iclSetP, REP_TRACE_LEAVE_REPQ_GET,
	       ICL_TYPE_POINTER, data, ICL_TYPE_POINTER, *outCookieP);

    return data;
}	/* repq_Get() */


void
repq_Put(rep_queue_t* rqP, rep_queue_cookie_t cookie, time_t deadline)
{
    rep_queue_item_t*	rqiP = cookie;

    icl_Trace3(iclSetP, REP_TRACE_REPQ_PUT,
	       ICL_TYPE_POINTER, rqP, ICL_TYPE_POINTER, cookie,
	       ICL_TYPE_LONG, deadline);

    LockQueue(rqP);

    if (rqiP->rqi_flags & RQI_FLAG_DELETE) {
	/*
	 * A delete request occured while the item was in use, so delete
	 * it now.
	 */
	icl_Trace0(iclSetP, REP_TRACE_REPQ_DELETE_ON_PUT);
	osi_Free(rqiP, sizeof *rqiP);
    } else {
	/*
	 * If the deadline was reset while the item was in use, set the
	 * deadline to the minimum of the two; otherwise, set it to
	 * our value.
	 */
	if (!(rqiP->rqi_flags & RQI_FLAG_RESET) ||
	    rqiP->rqi_deadline > deadline) {
	    rqiP->rqi_deadline = deadline;
	}

	icl_Trace1(iclSetP, REP_TRACE_REPQ_PUT_SET_DEADLINE,
		   ICL_TYPE_LONG, rqiP->rqi_deadline);

	rqiP->rqi_flags = 0;

	Insert(rqP, rqiP);
    }

    UnlockQueue(rqP);

    icl_Trace0(iclSetP, REP_TRACE_LEAVE_REPQ_PUT);
}	/* repq_Put() */


void
repq_ResetDeadline(rep_queue_t* rqP, rep_queue_cookie_t cookie,
		   time_t newDeadline)
{
    rep_queue_item_t*	rqiP = cookie;

    icl_Trace3(iclSetP, REP_TRACE_REPQ_RESET_DEADLINE,
	       ICL_TYPE_POINTER, rqP, ICL_TYPE_POINTER, cookie,
	       ICL_TYPE_LONG, newDeadline);

    LockQueue(rqP);

    osi_assert(!(rqiP->rqi_flags & RQI_FLAG_DELETE));

    if (rqiP->rqi_flags & RQI_FLAG_INUSE) {
	icl_Trace0(iclSetP, REP_TRACE_REPQ_DEFERRED_RESET);
	/*
	 * If the deadline was already reset, set the deadline to the
	 * minimum of the two; otherwise, set it to our value.
	 */
	if (!(rqiP->rqi_flags & RQI_FLAG_RESET) ||
	    rqiP->rqi_deadline > newDeadline) {
	    rqiP->rqi_deadline = newDeadline;
	}
	rqiP->rqi_flags |= RQI_FLAG_RESET;
    } else if (newDeadline < rqiP->rqi_deadline) {
	rep_queue_item_t*	prevItemP = FindPrev(rqP, rqiP);

	/*
	 * The item is not in use, just reset the deadline; however, we may
	 * have to reorder the queue.
	 * 
	 * Note that we only allow the deadline to be reset to an earlier
	 * time.
	 */

	rqiP->rqi_deadline = newDeadline;

	if (prevItemP == NULL) {
	    /*
	     * We're changing the head of the queue, so we have to 
	     * wakeup any waiters.
	     */
	    WakeupWaiters(rqP);
	} else if (rqiP->rqi_deadline < prevItemP->rqi_deadline) {
	    /*
	     * We're changing an item in the middle of the list *and*
	     * its deadline is now earlier than the previous item's,
	     * so we must reorder the list.  We accomplish the reordering
	     * be removing the item and re-inserting it.
	     */
	    prevItemP->rqi_next = rqiP->rqi_next;
	    rqiP->rqi_next = NULL;	/* Just a sanity check */

	    /* Insert() will wakeup the waiters if this becomes the head. */
	    Insert(rqP, rqiP);
	}
    }

    UnlockQueue(rqP);

    icl_Trace0(iclSetP, REP_TRACE_LEAVE_REPQ_RESET_DEADLINE);
}	/* repq_ResetDeadline() */


static rep_queue_item_t*
FindPrev(rep_queue_t* rqP, rep_queue_item_t* rqiP)
{
    rep_queue_item_t*	p = NULL;

    /* If the item is at the head, return NULL; otherwise, search for it. */
    if (rqP->rq_head != rqiP) {
	p = rqP->rq_head;
	while (p != NULL) {
	    if (p->rqi_next == rqiP)
		break;

	    p = p->rqi_next;
	}
	osi_assert(p != NULL);		/* We should always find it */
    }

    return p;
}	/* FindPrev() */


static void
Insert(rep_queue_t* rqP, rep_queue_item_t* rqiP)
{
    rep_queue_item_t**	p;

    osi_assert(rqiP->rqi_next == NULL);

    for (p = &rqP->rq_head; *p != NULL; p = &(*p)->rqi_next) {
	if (rqiP->rqi_deadline < (**p).rqi_deadline)
	    break;
    }

    /* `p' now points to the new previous item's next pointer */
    rqiP->rqi_next = *p;
    *p = rqiP;

    if (*p == rqP->rq_head) {		/* Inserted at head */
	WakeupWaiters(rqP);
    }
}	/* Insert() */


static void
LockQueue(rep_queue_t* rqP)
{
    int	code;

    code = pthread_mutex_lock(&rqP->rq_mutex);
    osi_assert(code != -1);
}	/* LockQueue() */


static void
UnlockQueue(rep_queue_t* rqP)
{
    int	code;

    code = pthread_mutex_unlock(&rqP->rq_mutex);
    osi_assert(code != -1);
}	/* UnlockQueue() */


static void
WakeupWaiters(rep_queue_t* rqP)
{
    int code;

    icl_Trace0(iclSetP, REP_TRACE_REPQ_WAKEUP_WAITERS);
    code = pthread_cond_broadcast(&rqP->rq_hasData);
    osi_assert(code != -1);
}	/* WakeupWaiters() */
