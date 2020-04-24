/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: tpq_wait.c,v $
 * Revision 1.1.9.1  1996/10/02  18:49:32  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:49:32  damon]
 *
 * Revision 1.1.4.1  1994/06/09  14:23:21  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:35:07  annie]
 * 
 * Revision 1.1.2.4  1993/01/21  15:56:44  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  17:41:01  cjd]
 * 
 * Revision 1.1.2.3  1992/11/24  19:49:04  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:33:53  bolinger]
 * 
 * Revision 1.1.2.2  1992/08/31  21:40:27  jaffe
 * 	Transarc delta: comer-ot3947-multithread-cm-daemon 1.25
 * 	[1992/08/30  13:23:57  jaffe]
 * 
 * $EndLog$
 */
/*
 *	Copyright (C) 1992 Transarc Corporation
 *	All rights reserved.
 */

#ifndef KERNEL
#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <pthread.h>

#define FREELISTMAXSIZE 3

/*
 * define structure to hold condition variables and mutex for each wait.  We're
 * just going to stick these things on an unordered linked list since we're
 * only expecting a few to be on the list at any given time.
 */
struct waitInfo
{
    struct osi_WaitHandle *handle;		/* how to locate these babies */
    pthread_cond_t	waitCond;		/* condition to wait upon */
    pthread_mutex_t 	waitMutex;		/* mutex protecting condition */
    int			waitPred;		/* predicate for wait */
    struct waitInfo	*next;			/* next in chain */
};

/*
 * define static globals for mutex to protect the
 * queue of wait structures and the free list.
 */
static pthread_mutex_t 	listMutex;
static struct waitInfo  *active = (struct waitInfo *)NULL;
static struct waitInfo  *freelist = (struct waitInfo *)NULL;
static int		freelistSize = 0;
static int 		initted = 0;

PRIVATE struct waitInfo *Find(handle)
  IN struct osi_WaitHandle *handle;
{
    register struct waitInfo *wp;

    /* search list for handle */
    (void) pthread_mutex_lock(&listMutex);
    for(wp = active; wp; wp = wp->next)
    {
	if (wp->handle == handle)
	    break;
    }
    (void) pthread_mutex_unlock(&listMutex);
    return(wp);
}

PRIVATE struct waitInfo *New(handle)
  IN struct osi_WaitHandle *handle;
{
    register struct waitInfo *wp;

    /* search list for handle */
    (void) pthread_mutex_lock(&listMutex);
    if (freelist != (struct waitInfo *)NULL)
    {
	wp = freelist;
	freelist = freelist->next;
	freelistSize--;
    }
    else
    {
	wp = osi_Alloc(sizeof(struct waitInfo));

	/* initialize private condition variable and mutex */
	(void) pthread_cond_init(&wp->waitCond, pthread_condattr_default);
	(void) pthread_mutex_init(&wp->waitMutex, pthread_mutexattr_default);
    }
    wp->waitPred = 0;
    wp->handle = handle;

    /* thread onto active list */
    wp->next = active;
    active = wp;

    (void) pthread_mutex_unlock(&listMutex);
    return(wp);
}

PRIVATE void Destroy(struct waitInfo *wEntryP)
{
    register struct waitInfo *wp;
    register struct waitInfo *pwp;

    (void) pthread_mutex_lock(&listMutex);

    /* find wEntryP on list */
    for(pwp = (struct waitInfo *)NULL, wp = active;
	wp != (struct waitInfo *)NULL;
	pwp = wp, wp = wp->next)
    {
	if (wp == wEntryP)
	{
	    if (wp == active)	/* at beginning of the list? */
		active = wp->next;
	    else
		pwp->next = wp->next;
	    break;
	}
    }
    if (wp)
    {
	if (freelistSize >= FREELISTMAXSIZE)
	{
	    pthread_cond_destroy(&wp->waitCond);
	    pthread_mutex_destroy(&wp->waitMutex);
	    osi_Free(wp, sizeof(struct waitInfo));
	}
	else
	{
	    /* keep only a few of these around */
	    wp->next = freelist;
	    freelist = wp;
	    freelistSize++;
	}
    }
    else
    {
	(void) fprintf(stderr, "tpq_wait: Attempting to destroy a non-existent wait structure\n");
    }

    (void) pthread_mutex_unlock(&listMutex);
}

EXPORT int osi_Wait(long ams, struct osi_WaitHandle *ahandle, int aintok)
{
    register struct waitInfo *wp;
    struct timespec expire;

    /* initialize if necessary */
    if (!initted)
    {
	/* initialize mutex */
	(void) pthread_mutex_init(&listMutex, pthread_mutexattr_default);
	initted = 1;
    }

    wp = New(ahandle);
    if (wp == (struct waitInfo *)NULL)
    {
	(void) fprintf(stderr, "tpq_wait: New() returns NULL\n");
	return(-1);
    }

    expire.tv_sec = osi_Time() + ams/1000;
    expire.tv_nsec = 0;
    pthread_mutex_lock(&wp->waitMutex);
    wp->waitPred = 0;	/* wait for this to change to 1 */
    while(!wp->waitPred)
    {
	int ret;
	ret = pthread_cond_timedwait(&wp->waitCond, &wp->waitMutex, &expire);
	if (ret == 0)
	    ahandle->proc = 0;		/* awakened */
	else
	    wp->waitPred = 1;		/* timed out */
    }
    pthread_mutex_unlock(&wp->waitMutex);

    Destroy(wp);

    return(0);
}

EXPORT int osi_CancelWait(struct osi_WaitHandle *ahandle)
{
    register struct waitInfo *wp;

    if (!initted)
	return(0);
    if ((wp = Find(ahandle)) == (struct waitInfo *)NULL)
	return(0);	/* not there yet */

    (void) pthread_mutex_lock(&wp->waitMutex);
    wp->waitPred = 1;
    (void) pthread_cond_signal(&wp->waitCond);
    (void) pthread_mutex_unlock(&wp->waitMutex);
    return(0);
}

#endif /* !KERNEL */
