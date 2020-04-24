/* 
 * Copyright (C) 1996, 1993  Transarc Corporation - All rights reserved 
 */
/*
 * Test cache dirty range write time.
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

#define CM_CHUNKSIZE	(64*1024)
#define NFILES		10
#define TFDATA		0x5a

extern int	in_subdir;	/* Flag to tell if we are in subdir */
char		*prog_name = NULL;
int		nfiles = NFILES;

void usage()
{
	fprintf(stdout, "usage: %s [-h] [nfiles]\n", prog_name);
	fprintf(stdout, "  Flags:  h    Help - print this usage info\n");
	fprintf(stdout, " nfiles:       Number of files to create\n");
}


void clean_up(sig)
int	sig;
{
	if (sig) fprintf(stderr, "Cleaning up now, sig = %d\n", sig);

	if (remove_testdir(nfiles, in_subdir) != 0) {
	    fprintf(stderr, "Failure in cleaning up test directory\n");
	    exit(1);
	}

	exit(0);
}    


int main(argc, argv)
	int argc;
	char *argv[];
{
    char 		*opts, *buf, fname[MAXPATHLEN];
    int 		i, *fds, size;
    struct timeval 	tv_write;
    long		write_usec;
    double		write_time, u_time, s_time;
    void		(*old_int_hand)(), (*old_quit_hand)();

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
	if (nfiles < 0 || nfiles > OPEN_MAX) nfiles = NFILES;
	argc--; argv++;
    }
    if (argc != 0) {
	fprintf(stderr, "too many parameters");
	usage();
	exit(1);
    }

    fprintf(stderr, "\n# Micro2 Write file dirty range test, %d files\n",
	    nfiles);

    /*
     * Adjust user file mask
     */
    (void) umask(0);

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

    if ((buf = (char *)malloc(CM_CHUNKSIZE)) == NULL) {
	perror("malloc1");
	exit(1);
    }

    (void) memset(buf, TFDATA, CM_CHUNKSIZE);

    if (make_testdir(nfiles, buf, CM_CHUNKSIZE) != 0) {
	fprintf(stderr, "make test directory failed, bailing out\n");
	exit(1);
    }

    if ((fds = (int *)malloc(nfiles * sizeof(int))) == NULL) {
	perror("malloc2");
	exit(1);
    }

    /*
     * First fill the fds array, so we don't have to pay
     * the open-time during our test.  This gets all our files
     * open at once
     */
    for (i=0; i<nfiles; i++) {
	sprintf(fname, "%s.%d",TFNAME, i);
	if ((fds[i] = open(fname, O_RDWR)) < 0) {
	    perror("open file");
	    fprintf(stderr, "file number %d\n", i);
	    exit(1);
	}
    }

    /*
     * Now we have all the files open and ready to write to
     */
    if (aux_usage_start()) {
	perror("getting rusage start failed");
	exit(1);
    }
    timer_start();
    for (i=0; i<nfiles; i++) {

	if (write(fds[i], &size, sizeof(int)) != sizeof(int)) {
	    perror("write");
	    exit(1);
	}	    

	if (close(fds[i]) < 0) {
	    perror("close");
	    exit(1);
	}	    
    }
    timer_stop();
    if (aux_usage_end()) {
	perror("getting rusage end failed");
	exit(1);
    }

    /*
     * Compute net time and rusage
     */
    timer_value(&tv_write);
    aux_net_usage(&u_time, &s_time);

    write_usec = (tv_write.tv_sec * 1000000) + tv_write.tv_usec;

    /*
     * Average the per-write and per-close times
     */
    write_usec /= nfiles;

    /*
     * Compute seconds
     */
    write_time = (double)write_usec / 1000000;

    /*
     * Display average times
     */
    fprintf(stdout, "\tAverage time %3.4f sec\n", write_time);
    fprintf(stdout, "\tCPU time utilization:\t User: %2.5f   System %2.5f\n",
	    u_time, s_time);

    (void) free(buf);
    (void) free(fds);

    clean_up(0);
    /* NOTREACHED */
}
