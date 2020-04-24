/*
 * @OSF_COPYRIGHT@
 */
/*
 * HISTORY
 * $Log: rep_queue.h,v $
 * Revision 1.1.5.1  1996/10/02  18:14:30  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:46:09  damon]
 *
 * $EndLog$
 */

#ifndef	_REP_QUEUE_H_
#define	_REP_QUEUE_H_

/*
 * (C) Copyright 1994 Transarc Corporation
 * Licensed Materials - Property of Transarc
 * All Rights Reserved.
 *
 * $Header: /u0/rcs_trees/dce/rcs/file/rep/rep_queue.h,v 1.1.5.1 1996/10/02 18:14:30 damon Exp $
 */

#include <dcedfs/sysincludes.h>
#include <dcedfs/icl.h>

#include <pthread.h>

typedef void*			rep_queue_cookie_t;

struct rep_queue_item;

typedef struct rep_queue {
    pthread_mutex_t		rq_mutex;
    pthread_cond_t		rq_hasData;
    struct rep_queue_item*	rq_head;
} rep_queue_t;


/*
 * repq_InitICL -- Initialize the ICL set for the repq module.
 */
extern void repq_InitICL(struct icl_set*);

/*
 * repq_Init -- Initializes a priority queue
 */
extern void repq_Init(rep_queue_t*);

/*
 * repq_Enter -- Place an entry in the queue for the first time
 */
extern rep_queue_cookie_t repq_Enter(rep_queue_t*, void*, time_t);

/*
 * repq_Delete -- Remove an entry from the queue for good
 */
extern void repq_Delete(rep_queue_t*, rep_queue_cookie_t);

/*
 * repq_Get -- Get an item from the head of the queue if its deadline has
 * 	passed; otherwise, sleep until an item's deadline does occur.
 */
extern void* repq_Get(rep_queue_t*, rep_queue_cookie_t*);

/*
 * repq_Put -- Put an item back on the queue with a new deadline
 */
extern void repq_Put(rep_queue_t*, rep_queue_cookie_t,
					 time_t);

/*
 * repq_ResetDeadline -- Reset an item's deadline
 */
extern void repq_ResetDeadline(rep_queue_t*, rep_queue_cookie_t, time_t);


#endif	/* _REP_QUEUE_H_ */
