/* Copyright (C) 1993, 1990 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>


RCSID ("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/anode/RCS/test_sscanf.c,v 1.7 1993/11/10 14:58:25 andi Exp $")

#if defined(AFS_AIX_ENV) || defined(AFS_HPUX_ENV)
#else
#define cma_lock_global()
#define cma_unlock_global()
#endif

#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */

static int processesCreated = 0;	/* this is the notify addr for both */
static int processesDeleted = 0;
static pthread_t initialProcess;

static int safe = 0;			/* use CMA global lock? */
static int trace = 0;			/* show each iteration */
static int quantum = 0;			/* scheduling granularity in usec */

struct processData {
    int procId;
    pthread_t pid;
    int scans;
};

static void SetQuantum (void)
{
# if (_CMA_OS_ == _CMA__UNIX) && defined(AFS_AIX_ENV)

    /* This special has only been checked on AIX.  It definitely won't work on
     * Solaris. */

    struct timeval q;
    struct itimerval timer;
    static struct itimerval oldtimer;
    long code;

    extern struct timeval cma__g_one_tick;

    if (quantum <= 0) return;
    if (quantum < 10000) {
	printf ("Can't set quantum below 10msec.\n");
	quantum = 10000;
    }
    q.tv_sec = quantum / 1000000;
    q.tv_usec = quantum % 1000000;
    code = getitimer (ITIMER_VIRTUAL, &timer);
    if (code) perror ("getitimer");
    timer.it_value = q;
    timer.it_interval = q;
    code = setitimer (ITIMER_VIRTUAL, &timer, &oldtimer);
    if (code) perror ("setitimer");
    cma__g_one_tick = q;
#else
    printf ("Can't change non-CMA scheduler quantum\n");
#endif
}

#if defined(AFS_AIX_ENV) || defined(AFS_HPUX_ENV)
#define setsched(t,n) \
    MACRO_BEGIN \
	int pri = ((n) == 0 ? PRI_RR_MAX : PRI_RR_MIN); \
	long code = pthread_setscheduler (t, SCHED_RR, pri); \
	if ((code == -1) && (errno != ENOSYS)) { \
	    perror ("pthread_setscheduler"); \
	    fprintf (stderr, "ProcessMain(%d): can't set scheduler to %d or priority to %d", \
		     n, SCHED_RR, pri); \
	    exit (1); \
	} \
    MACRO_END
#else
#define setsched(t,n)
#endif

static void *ProcessMain (void *arg)
{
    struct processData *o = arg;
    char buf[100];
    int p;
    int iter;
    int count, a, b, c, d;

    o->pid = pthread_self();
    setsched (o->pid, o->procId);

    p = o->procId;
    for (iter = 0; iter < o->scans; iter++) {
	if (trace) {
	    printf ("%d.", p); fflush(stdout);
	}
	sprintf (buf, "%d %d %d %d", iter*197, p*iter, p, p*163);
	if (safe) cma_lock_global();
	count = sscanf (buf,  "%d %d %d %d", &a, &b, &c, &d);
	if (safe) cma_unlock_global();
	if ((count != 4) ||
	    (a != iter*197) || (b != p*iter) || (c != p) || (d != p*163)) {
	    fprintf (stderr,
		     "sscanf failed for proc=%d/iter=%d on '%s': count is %d or %d != %d or %d != %d or %d != %d or %d != %d\n",
		     p, iter, buf, count, a,iter*197, b,p*iter, c,p, d,p*163);
	}
    }
    if (trace) printf ("\n");

    processesDeleted++;
    osi_Wakeup (&processesDeleted);
    pthread_exit(0);
}

void main (argc, argv)
  IN int   argc;
  IN char *argv[];
{
    long code;
    int nThreads = 10;
    int nScans = 1000;
    int i;

    if (argc == 0)
	printf ("Usage is: %s <nthreads> <scans> <safe> <trace> <quantum>\n",
		argv[0]);
    if (argc > 1) nThreads = atoi (argv[1]);
    if (argc > 2) nScans = atoi (argv[2]);
    if (argc > 3) safe = atoi (argv[3]);
    if (argc > 4) trace = atoi (argv[4]);
    if (argc > 5) quantum = atoi (argv[5]);
    if (nThreads == 0) {
	fprintf (stderr, "Too few threads\n");
	exit (1);
    }
    if (nScans == 0) {
	fprintf (stderr, "Too few scans\n");
	exit (1);
    }
    initialProcess = pthread_self();

    /* Make sure we are highest priority thread so we get everyone created
     * before any start running. */

    setsched (initialProcess, 0/*main*/);

    for (i=0; i<nThreads; i++) {
	pthread_t newPid;
	pthread_attr_t attr;
	struct processData *o;
	o = (struct processData *)osi_Alloc (sizeof(*o));
	o->procId = ++processesCreated;
	o->scans = nScans;
	pthread_attr_create(&attr);
	code = pthread_create(&newPid, attr, ProcessMain, (void *)o);
	pthread_attr_delete(&attr);
	if (code) {
	    fprintf (stderr, "main: pthread_create returned %d\n", code);
	    exit (1);
	}
    }
    printf ("Running test %s cma global lock.\n", (safe ? "with" : "without"));
    printf ("%d threads each doing %d scans.\n", nThreads, nScans);
    if (quantum > 0) {
	SetQuantum ();
	printf ("Setting quantum to %g seconds\n", quantum/1e6);
    }
    while (processesDeleted < processesCreated) {
	osi_Sleep ((caddr_t)&processesDeleted);
    }

    exit (0);
}
