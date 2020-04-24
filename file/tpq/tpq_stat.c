/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* Copyright (C) 1994, 1992 Transarc Corporation - All rights reserved */
#include <dcedfs/param.h>
#include <dcedfs/stds.h>

#include <tpq.h>
#include <tpq_private.h>

#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/tpq/RCS/tpq_stat.c,v 1.9 1994/11/07 18:05:27 cfe Exp $")

/*
 *	This module contains:
 *
 */
/* EXPORT */
void tpq_Stat(poolHandle, statsP)
  IN void *poolHandle;
  IN struct tpq_stats *statsP;
{
    int i;
    int opsFound;
    int sluggishFound;
    struct tpq_threadPool *poolP = (struct tpq_threadPool *)poolHandle;
    struct tpq_queueEntry *entryP;

    /* find out number of threads */
    tpqLock_ObtainRead(&poolP->poolLock);
    statsP->nthreads = poolP->nthreads;
    tpqLock_ReleaseRead(&poolP->poolLock);

    /* 
     * count up number of ops queued and number of ops queued 
     * that haven't been run since last time.  We tell the
     * latter by setting the mark flag in each entry structure.
     */
    tpqLock_ObtainRead(&poolP->queueLock);
    for(i = 0; i < TPQ_NPRIORITIES; i++)
    {
	opsFound = sluggishFound = 0;
	entryP = poolP->queue[i];
	if (entryP != (struct tpq_queueEntry *)NULL)
	{
	    do{
		opsFound++;
		if (entryP->mark)
		    sluggishFound++;
		entryP->mark = 1;
		entryP = entryP->next;
	    } while(entryP != poolP->queue[i]);
	}
	statsP->totalQueued[i] = opsFound;
	statsP->sluggishQueued[i] = sluggishFound;
    }
    tpqLock_ReleaseRead(&poolP->queueLock);
}
