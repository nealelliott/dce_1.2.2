/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: async_queue.c,v $
BINRevision 1.1.2.1  1996/10/02  17:48:15  damon
BIN	Newest DFS from Transarc
BIN
 * $EndLog$
*/
/*
 * async_queue.c
 *
 * Routines to manage Async I/O buffer queue.
 *
 * (C) Copyright 1996 Transarc Corporation
 * All Rights Reserved
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <sys/errno.h>

#if defined(KERNEL) && defined(AFS_HPUX_ENV)
#include <dce/ker/pthread.h>    /* for osi_ThreadCreate */
#else   /* KERNEL && AFS_HPUX_ENV */
#include <pthread.h>            /* for osi_ThreadCreate */
#endif  /* KERNEL && AFS_HPUX_ENV */

#include <async_queue.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/ftutil/async_queue.c,v 1.1.2.1 1996/10/02 17:48:15 damon Exp $")

/* round integer VAL up to next multiple of N */
#define ROUND_UP(VAL,N) 	((((VAL)+(N)-1)/(N))*(N))

/*
 * thread to handle the output end of the queue. Passes each queue
 * element from the output queue to the thread routine and returns
 * the buffers to the input queue. The thread terminates whenever
 * it sees a queue element with the last flag set, or when the output
 * queue is empty and the queue's threadEnd flag is set.
 */

void asyncQueueThread(argP,unused)
void *argP;			/* pointer to queue element */
void *unused;
{
    asyncQueue_t *asyncP = (asyncQueue_t *)argP;
    asyncElement_t *elemP;
    int last = 0;

    /*
     * Process elements from the output queue, return them on the input
     * queue. The thread terminates when it sees a queue element with
     * the last flag set. The thread aborts if the threadEnd flag is set.
     */
    while(1) {
	/* wait for an output element */
        lock_ObtainWrite(&asyncP->qLock);
	while (asyncP->outHead == NULL && asyncP->threadEnd == 0) {
	    osi_SleepW(asyncP,&asyncP->qLock);
	    lock_ObtainWrite(&asyncP->qLock);
	}

	/* check for abort */
	if (asyncP->outHead == NULL) {
	    asyncP->threadDone = 1;
	    osi_Wakeup(asyncP);
	    lock_ReleaseWrite(&asyncP->qLock);
	    return;
	}

	/*  get an element from the output queue */
	elemP = asyncP->outHead;
	asyncP->outHead = elemP->next;
	if ( asyncP->outHead == NULL ) asyncP->outTail = NULL;
	elemP->next = NULL;
	lock_ReleaseWrite(&asyncP->qLock);

	/* check if the caller has set the last flag */
	if ( elemP->last ) last = 1;

	/* call the asyncRtn to process the element routine */
	(*asyncP->asyncRtn)(elemP, asyncP->threadState);

	/* check if the asyncRtn has set the last flag */
	if ( elemP->last ) last = 1;

	/* put the element onto the input queue */
	lock_ObtainWrite(&asyncP->qLock);
	if ( asyncP->inHead == NULL ) asyncP->inHead = elemP;
	else  asyncP->inTail->next = elemP;
	asyncP->inTail = elemP;
	elemP->next = NULL;
	osi_Wakeup(asyncP);
	lock_ReleaseWrite(&asyncP->qLock);

	/* terminate after the last buffer has been processed */
	if ( last ) {
	    lock_ObtainWrite(&asyncP->qLock);
	    asyncP->threadDone = 1;
	    osi_Wakeup(asyncP);
	    lock_ReleaseWrite(&asyncP->qLock);
	    return;
	}
    }
}

/*
 * get an element from the async queue
 */

asyncElement_t *getAsyncElement(asyncP)
asyncQueue_t *asyncP;
{
    asyncElement_t *elemP;

    /* wait for an element on the input queue */
    lock_ObtainWrite(&asyncP->qLock);
    while ((asyncP->inHead == NULL && asyncP->threadDone == 0) ||
           (asyncP->inHead != NULL && asyncP->inHead->hold != 0)) {
        osi_SleepW(asyncP,&asyncP->qLock);
        lock_ObtainWrite(&asyncP->qLock);
    }

    /* check for abort */
    if (asyncP->inHead == NULL) {
        lock_ReleaseWrite(&asyncP->qLock);
	return(NULL);
    }

    /* return the next element on the queue */
    elemP = asyncP->inHead;
    asyncP->inHead = elemP->next;
    if ( asyncP->inHead == NULL ) asyncP->inTail = NULL;
    elemP->next = NULL;
    lock_ReleaseWrite(&asyncP->qLock);
    return(elemP);
}

/*
 * return an element to the async queue
 */

void relAsyncElement(asyncP, elemP)
asyncQueue_t *asyncP;
asyncElement_t *elemP;
{
    lock_ObtainWrite(&asyncP->qLock);
    elemP->next = NULL;
    if ( asyncP->outHead == NULL ) asyncP->outHead = elemP;
    else  asyncP->outTail->next = elemP;
    asyncP->outTail = elemP;
    osi_Wakeup(asyncP);
    lock_ReleaseWrite(&asyncP->qLock);
    return;
}

/*
 * initialize the async queue
 */

asyncQueue_t *initAsyncQueue(nBuf, bufSize, asyncRtn, callerState, threadState)
long nBuf;
long bufSize;
void (*asyncRtn)();
void *callerState;
void *threadState;
{
    int i;
    long code;
    asyncQueue_t *asyncP;
    int asyncSize;
    asyncElement_t *elemP;
    int elemSize;
    char *allocP;
    int allocSize;
    char *dataP;
    int dataSize;

    /*
     * allocate the memory for the queue. Round sizes of structureS
     * and data buffers up to ensure double word alignment.
     */
    asyncSize = ROUND_UP(sizeof(asyncQueue_t), 2*sizeof(long));
    elemSize = ROUND_UP(sizeof(asyncElement_t), 2*sizeof(long));
    dataSize = ROUND_UP(bufSize, 2*sizeof(long));
    allocSize = asyncSize + nBuf * (elemSize + dataSize);
    allocP = osi_Alloc(allocSize);
    asyncP = (asyncQueue_t *)allocP;
    elemP = (asyncElement_t *)(allocP + asyncSize);
    dataP = allocP + asyncSize + nBuf * elemSize;
    if (!allocP) return(NULL);

    /* initialize the queue structure */
    lock_Init(&asyncP->qLock);
    asyncP->inHead = NULL;
    asyncP->inTail = NULL;
    asyncP->outHead = NULL;
    asyncP->outTail = NULL;
    asyncP->threadEnd = 0;
    asyncP->threadDone = 0;
    asyncP->allocSize = allocSize;
    asyncP->asyncRtn = asyncRtn;
    asyncP->threadState = threadState;
    asyncP->callerState = callerState;

    /* initialize the queue elements */
    for ( i = 0 ; i < nBuf ; i++ ) {
	elemP->data = dataP;
	elemP->size = bufSize;
	elemP->count = -1;
	elemP->offset = -1;
	elemP->error = 0;
	elemP->last = 0;
	elemP->hold = 0;
	if (asyncP->outTail == NULL)
	    asyncP->outTail = elemP;
	elemP->next = asyncP->outHead;
	asyncP->outHead = elemP;
	elemP = (asyncElement_t *)((char *)elemP + elemSize);
	dataP = dataP + dataSize;
    }

    /* start the thread */
    osi_ThreadCreate(asyncQueueThread, (void *)asyncP, 0, 0, "async", code);
    if (code) {
	osi_Free(asyncP,asyncP->allocSize);
	return(NULL);
    }

    return(asyncP);
}

/*
 * terminate the async queue
 */

void termAsyncQueue(asyncP)
asyncQueue_t *asyncP;
{
    if (!asyncP) return;

    /* wait for the thread to terminate and free resources */
    lock_ObtainWrite(&asyncP->qLock);
    asyncP->threadEnd = 1;
    osi_Wakeup(asyncP);
    while ( asyncP->threadDone == 0 ) {
        osi_SleepW(asyncP,&asyncP->qLock);
        lock_ObtainWrite(&asyncP->qLock);
    }
    osi_Free(asyncP,asyncP->allocSize);

    return;
}
