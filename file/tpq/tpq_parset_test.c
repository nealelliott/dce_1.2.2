/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: tpq_parset_test.c,v $
 * Revision 1.1.11.1  1996/10/02  18:49:20  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:49:27  damon]
 *
 * Revision 1.1.6.2  1994/07/13  22:30:11  devsrc
 * 	merged with bl-10
 * 	[1994/06/29  11:56:04  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  16:03:32  mbs]
 * 
 * Revision 1.1.6.1  1994/06/09  14:23:14  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:34:57  annie]
 * 
 * Revision 1.1.2.4  1993/01/21  15:56:27  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  17:40:30  cjd]
 * 
 * Revision 1.1.2.3  1992/11/24  19:48:42  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:33:23  bolinger]
 * 
 * Revision 1.1.2.2  1992/08/31  21:39:58  jaffe
 * 	Transarc delta: comer-ot3947-multithread-cm-daemon 1.25
 * 	[1992/08/30  13:20:36  jaffe]
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

void ToRun(arg)
void *arg;
{
    int s = (int)arg;
    (void) fprintf(stderr, "%d: ToRun(%d) called\n", time((long *)0), s);
    Sleep(s*6+1);
    (void) fprintf(stderr, "%d: ToRun(%d) returning\n", time((long *)0), s);
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
    void *setHandle;

    poolHandle = tpq_Init(2, 3, 4, 60, "tpq");
    if (!poolHandle)
    {
	(void) fprintf(stderr, "%d: init failed\n", time((long *)0));
	exit(1);
    }

    prtStatus(poolHandle);
    setHandle = tpq_CreateParSet(poolHandle);
    if (setHandle == NULL)
    {
	(void) fprintf(stderr, "Error in creating set handle\n");
	exit(1);
    }

    (void) fprintf(stderr, "%d: Adding to set\n", time((long *)0));
    for(i = 0; i < 10; i++)
	(void) tpq_AddParSet(setHandle, ToRun, (void *)i, TPQ_HIGHPRI, 30);
    (void) fprintf(stderr, "%d: Set Complete\n", time((long *)0));
    prtStatus(poolHandle);

    tpq_WaitParSet(setHandle);
    (void) fprintf(stderr, "%d: Wait Complete\n", time((long *)0));

    tpq_ShutdownPool(poolHandle);
    (void) fprintf(stderr, "%d: Pool shutdown\n", time((long *)0));
    exit(0);
}

