/* 
 * Copyright (C) 1996, 1993  Transarc Corporation - All rights reserved 
 */
/*
 * Test cache create/open/read/close/unlink time for very small files.
 * Builds a tree on the server.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>

#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#if	defined(__hpux)
#define OPEN_MAX FOPEN_MAX
#endif
#if	defined(AIX32)
#include <sys/limits.h>
#endif
#if	defined(SUNOS5)
#include <limits.h>
#endif
#include <fcntl.h>
#include "aux.h"

#define TFDATA		0x75 	/* u */
#define FNAMELEN	16
#define REPS		20
#define NFILES		48
#define FILESIZE	4

extern int	in_subdir;	/* Flag to tell if we are in subdir */
char		*prog_name = NULL;
int		nfiles = NFILES;

void usage()
{
    fprintf(stdout, "usage: %s [-h] [number of files]\n", prog_name);
    fprintf(stdout, "  Flags:  h    Help - print this usage info\n");
    fprintf(stdout, "  nfiles - number of files to create\n");
}


void clean_up(sig)
int	sig;
{
    if (sig) fprintf(stderr, "Cleaning up now, sig = %d\n", sig);

    if (remove_testdir(nfiles, in_subdir) != 0) {
	fprintf(stderr, "Error while cleaning up test dir\n");
	exit(1);
    }

    exit(0);
}    


int main(argc, argv)
	int argc;
	char *argv[];
{
    char 	*opts, buf[FILESIZE], *names, **namesp, *p;
    int 	i, j, *fds, open_usec, close_usec, unlink_usec;
    struct timeval tv_open, tv_close, tv_unlink;
    double	open_time, close_time, unlink_time, u_time, s_time;
    void	(*old_int_hand)(), (*old_quit_hand)();

    setbuf(stdout, NULL);
    prog_name = *argv++;
    argc--;
    while (argc && **argv == '-') {
	for (opts = &argv[0][1]; *opts; opts++) {
	    switch (*opts) {
	      case 'h':		/* help */
		usage();
		exit(1);

	      default:
		fprintf(stderr, "unknown option '%c'",
			*opts);
		usage();
		exit(1);
	    }
	}
	argc--;
	argv++;
    }
    if (argc != 0) {
	nfiles = atoi(*argv);
	if (nfiles <= 0 || nfiles > OPEN_MAX) {
	    fprintf(stderr,
		    "Invalid number of files specified, using default\n");
	    nfiles = NFILES;
	}
    }

    fprintf(stdout, "\n# Micro4 Cache small file op test, %d files\n", nfiles);

    /*
     * Set up signal handler to clean up if we bail out
     */
    in_subdir = 0;
    if ((old_int_hand = signal(SIGINT, &clean_up)) == SIG_ERR) {
	perror("signal intr");
	exit(1);
    }
    if ((old_quit_hand = signal(SIGQUIT, &clean_up)) == SIG_ERR) {
	perror("signal quit");
	exit(1);
    }

    /*
     * Adjust user file mask
     */
    (void) umask(0);

    /*
     * Set up a buffer with some data to fill into files
     */
    (void) memset(buf, TFDATA, FILESIZE);

    if (make_testdir(nfiles, buf, FILESIZE) != 0) {
	fprintf(stderr, "Could not create test tree files");
	exit(1);
    }

#if	defined(DEBUG)
    fprintf(stderr, "\tcreated test directory.\n");
#endif	/* DEBUG */

    /*
     * Create a list of the file names so as to avoid generating
     * them in-between open calls.
     */
    if ((namesp = (char **)malloc(nfiles * sizeof(char *))) == NULL) {
	perror("malloc");
	exit(1);
    }

    if ((names = (char *)malloc(nfiles * FNAMELEN)) == NULL) {
	perror("malloc");
	exit(1);
    }

    p = names;

    for (i=0; i<nfiles; i++) {
	namesp[i] = p;
	sprintf(p, "%s.%d", TFNAME, i);
	p += FNAMELEN;
    }

    /*
     * Make an array for file descriptors.
     */
    if ((fds = (int *)malloc(nfiles * sizeof(int))) == NULL) {
	perror("malloc failed");
	exit(1);
    }

    /*
     * Start opening these created files.
     * Time how long it takes to do an open.
     */
#if	defined(DEBUG)
    fprintf(stderr, "Starting open/close phase...");
#endif	/* DEBUG */

    open_usec = close_usec = unlink_usec = 0;
    if (aux_usage_start()) {
	perror("getting rusage start failed");
	exit(1);
    }
    for (j=0; j<REPS; j++) {
	timer_start();
	for (i=0; i<nfiles; i++) {
	    if ((fds[i] = open(namesp[i], O_RDONLY)) < 0) {
		perror("open");
		fprintf(stderr, "Open %s failed\n", namesp[i]);
	    }
	}
	timer_stop();
	timer_value(&tv_open);
	open_usec += (tv_open.tv_sec * 1000000) + tv_open.tv_usec;

	timer_start();
	for (i=0; i<nfiles; i++) {
	    if (close(fds[i]) != 0) {
		perror("close");
		fprintf(stderr, "close %s failed\n", namesp[i]);
	    }
	}
	timer_stop();
	timer_value(&tv_close);
	close_usec += (tv_close.tv_sec * 1000000) + tv_close.tv_usec;
    }

#if	defined(DEBUG)
    fprintf(stderr, "unlink phase...\n");
#endif	/* DEBUG */
    timer_start();
    for (i=0; i<nfiles; i++) {
	if (unlink(namesp[i]) < 0) {
	    perror("unlink");
	}
    }
    timer_stop();
    timer_value(&tv_unlink);
    unlink_usec = (tv_unlink.tv_sec * 1000000) + tv_unlink.tv_usec;

    /*
     * Get total rusage information
     */
    if (aux_usage_end()) {
	perror("getting rusage end failed");
	exit(1);
    }

    /*
     * Compute net rusage
     */
    aux_net_usage(&u_time, &s_time);

    /*
     * Average the open, close, unlink times.
     */
    open_usec = open_usec / (REPS * nfiles);
    close_usec = close_usec / (REPS * nfiles);
    unlink_usec = unlink_usec / nfiles;

    /*
     * Compute seconds
     */
    open_time = (double)open_usec / 1000000;
    close_time = (double)close_usec / 1000000;
    unlink_time = (double)unlink_usec / 1000000;

    /*
     * Display average times
     */
    fprintf(stdout, "\tAverage time: open: %2.5f close: %2.5f unlink: %2.5f\n",
	    open_time, close_time, unlink_time);
    fprintf(stdout, "\tCPU time utilization:\t User: %2.5f   System %2.5f\n",
	    u_time, s_time);

    (void) free(names);
    (void) free(namesp);
    (void) free(fds);
    clean_up(0);
    /* NOTREACHED */
}
