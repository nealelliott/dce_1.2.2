#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/debug.h>
#include <dcedfs/lock.h>

#include <dcedfs/episode/epi_thread.h>

int nThreads = 2;
int nSpins = 1e7;

epi_mutex_t runners;
epi_cond_t cond;

int processesCreated = 0;
int processesDeleted = 0;

int Create(pthread_startroutine_t start_fun, pthread_t *pP)
{
}

work()
{
}

void *child(void *rock)
{
    int i;

    for (i=0; i<nSpins; i++) {
    }

    epi_mutex_lock (&runners);
    processesDeleted++;
    epi_mutex_unlock (&runners);

    epi_cond_broadcast (&cond);
    pthread_exit(0);
}

main(int argc, char *argv[])
{
    pthread_t p[35];
    pthread_attr_t attr;
    long code;
    int i;

    if (argc > 1)
	nSpins = atoi(argv[1]);
    if (argc > 2)
	nThreads = atoi(argv[2]);
    if (nThreads > sizeof(p)/sizeof(p[0])) {
	fprintf (stderr, "Too many threads: %d\n", nThreads);
	exit (1);
    }
    printf ("Starting %d threads to loop %d times each.\n", nThreads, nSpins);

    epi_mutex_init(&runners, "runners");
    epi_cond_init(&cond, "runners");

    pthread_attr_create(&attr);
    for (i=0; i<nThreads; i++) {
	
	code = pthread_create(&p[i], attr, child, NULL);
	if (code) {
	    perror ("thread create");
	}

	epi_mutex_lock (&runners);
	processesCreated++;
	epi_mutex_unlock (&runners);
    }
    pthread_attr_delete(&attr);

    for (i=0; i<nThreads; i++) {
	pthread_join (p[i], NULL);
    }

    epi_mutex_lock (&runners);
    while (processesCreated != processesDeleted) {
	epi_cond_wait (&cond, &runners);
    }
    epi_mutex_unlock (&runners);
    exit (0);
}
