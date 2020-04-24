/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: tpq_test.c,v $
 * Revision 1.1.11.1  1996/10/02  18:49:27  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:49:29  damon]
 *
 * Revision 1.1.6.2  1994/07/13  22:30:14  devsrc
 * 	merged with bl-10
 * 	[1994/06/29  11:56:09  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  16:03:37  mbs]
 * 
 * Revision 1.1.6.1  1994/06/09  14:23:19  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:35:03  annie]
 * 
 * Revision 1.1.2.4  1993/01/21  15:56:40  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  17:40:55  cjd]
 * 
 * Revision 1.1.2.3  1992/11/24  19:49:00  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:33:47  bolinger]
 * 
 * Revision 1.1.2.2  1992/08/31  21:40:22  jaffe
 * 	Transarc delta: comer-ot3947-multithread-cm-daemon 1.25
 * 	[1992/08/30  13:23:15  jaffe]
 * 
 * $EndLog$
 */
#include <stdio.h>
#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <pthread.h>

#include <tpq.h>


void Sleep(t)
long t;
{
    struct timespec ts;

    ts.tv_sec = t;
    ts.tv_nsec = 0;

    pthread_delay_np(&ts);
}

void highPriority(arg, cookie)
void *arg;
void *cookie;
{
    int s = (int)arg;
    (void) fprintf(stderr, "%d: highPriority(%d, %x) called\n", time((long *)0), s, cookie);
    Sleep(s);
}

void medPriority(arg, cookie)
void *arg;
void *cookie;
{
    int s = (int)arg;
    (void) fprintf(stderr, "%d: medPriority(%d, %x) called\n", time((long *)0), s, cookie);
    Sleep(s);
}

void lowPriority(arg, cookie)
void *arg;
void *cookie;
{
    int s = (int)arg;
    (void) fprintf(stderr, "%d: lowPriority(%d, %x) called\n", time((long *)0), s, cookie);
    Sleep(s);
}

void prtStatus(poolHandle)
void *poolHandle;
{
    int i;
    struct tpq_stats st;

    tpq_Stat(poolHandle, &st);

    (void) fprintf(stderr, "%d: %d threads\n", time((long *)0), st.nthreads);
    for(i = 0; i < TPQ_NPRIORITIES; i++)
    {
	(void) fprintf(stderr, "%d: \t%d: queued: %d, sluggish: %d\n", time((long *)0),
		       i, st.totalQueued[i], st.sluggishQueued[i]);
    }
}

main()
{
    int i;
    long now;
    void *poolHandle;
    void *opHandle;


    poolHandle = tpq_Init(2, 3, 4, 60, "tpq");
    if (!poolHandle)
    {
	(void) fprintf(stderr, "%d: init failed\n", time((long *)0));
	exit(1);
    }

    prtStatus(poolHandle);
    Sleep(5);
    (void) fprintf(stderr, "%d: High priority\n", time((long *)0));
    for(i = 0; i < 10; i++)
	(void) tpq_QueueRequest(poolHandle, highPriority, (void *)30, 0, 60, 0, 0);
    opHandle = tpq_QueueRequest(poolHandle, highPriority, (void *)30, 0, 60, 120, 0);
    now = time((long *)0);
    (void) tpq_QueueRequest(poolHandle, highPriority, (void *)30, 0, 60, 120, now+480);

    prtStatus(poolHandle);
    Sleep(11*30);
    prtStatus(poolHandle);

    (void) fprintf(stderr, "%d: Medium priority\n", time((long *)0));
    for(i = 0; i < 10; i++)
	(void) tpq_QueueRequest(poolHandle, medPriority, (void *)30, 1, 60, 0, 0);

    prtStatus(poolHandle);
    Sleep(11*30);
    prtStatus(poolHandle);

    (void) fprintf(stderr, "%d: Low priority\n", time((long *)0));
    for(i = 0; i < 10; i++)
	(void) tpq_QueueRequest(poolHandle, lowPriority, (void *)30, 2, 60, 0, 0);

    prtStatus(poolHandle);
    (void) fprintf(stderr, "%d: Going to sleep\n", time((long *)0));
    Sleep(11*30);
    (void) fprintf(stderr, "%d: Low queue should be processed\n", time((long *)0));
    prtStatus(poolHandle);

    Sleep(10*60);
    (void) fprintf(stderr, "%d: Should be quiescent except for rescheduled high\n", time((long *)0));
    prtStatus(poolHandle);
    (void) fprintf(stderr, "%d: Try to dequeue %x\n", time((long *)0), opHandle);
    prtStatus(poolHandle);
    tpq_DequeueRequest(poolHandle, opHandle);
    (void) fprintf(stderr, "%d: Request dequeued\n", time((long *)0));
    Sleep(120);
    (void) fprintf(stderr, "%d: Should be quiescent\n", time((long *)0));
    prtStatus(poolHandle);
    tpq_ShutdownPool(poolHandle);
    (void) fprintf(stderr, "%d: Pool shutdown\n", time((long *)0));
    exit(0);
}

