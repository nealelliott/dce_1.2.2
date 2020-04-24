/* 
 * Copyright (C) 1996  Transarc Corporation - All rights reserved 
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
#include <dirent.h>
#include "aux.h"

#define FNAMELEN	16
#define NFILES		64
#define REPS		64
#define OP_CREAT	1
#define OP_OPEN_CLOSE	2
#define OP_STAT		3
#define OP_UNLINK	4
#define OP_READDIR	5

char *test_names[] = { "NULL",
		       "CREATE",
		       "OPEN and CLOSE",
		       "STAT",
		       "UNLINK",
		       "READDIR",
		       "INVALID"
		     };

extern int	in_subdir;	/* Flag to tell if we are in subdir */
int		*fds, nfiles = NFILES;
char 		*names, **namesp, *prog_name = NULL;
void 		do_op_test(int test);

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
    char 	*opts, *p;
    int 	i;
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

    fprintf(stdout, "\n# Micro6 Misc file op test: %d files\n", nfiles);

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

    if (make_testdir(0, 0, 0) != 0) {
	fprintf(stderr, "make test directory failed, bailing out\n");
	exit(1);
    }
    do_op_test(OP_CREAT);
    do_op_test(OP_OPEN_CLOSE);
    do_op_test(OP_STAT);
    do_op_test(OP_READDIR);
    do_op_test(OP_UNLINK);

    (void) free(names);
    (void) free(namesp);
    (void) free(fds);
    clean_up(0);
    /* NOTREACHED */
}


void do_op_test(int test)
{
    int		i, j, usec_op1, usec_op2, ops;
    struct timeval tv_op1, tv_op2;
    double	time_op1, time_op2, u_time, s_time;
    struct stat	statbuf;
    struct dirent	*direntp;
    DIR			*dirp;

    usec_op1 = usec_op2 = 0;
    time_op1 = time_op2 = u_time = s_time = 0;
    ops = REPS*nfiles;

    switch(test) {
      case OP_CREAT:
	/*
	 * Start opening these created files.
	 * Time how long it takes to do a creat.
	 */
	if (aux_usage_start()) {
	    perror("getting rusage start failed");
	    exit(1);
	}
	
	for (j=0; j<REPS; j++) {
	    timer_start();
	    for (i=0; i<nfiles; i++) {
		if ((fds[i] = creat(namesp[i], TFMODE)) < 0) {
		    perror("creat");
		    fprintf(stderr, "Creat %s failed\n", namesp[i]);
		}
	    }
	    timer_stop();
	    timer_value(&tv_op1);
	    usec_op1 += (tv_op1.tv_sec * 1000000) + tv_op1.tv_usec;

	    /*
	     * Close them now to conserve descriptors
	     */
	    for (i=0; i<nfiles; i++) {
		if (close(fds[i]) < 0) {
		    perror("close");
		    fprintf(stderr, "Close %s failed\n", namesp[i]);
		}
	    }

	}
	if (aux_usage_end()) {
	    perror("getting rusage end failed");
	    exit(1);
	}
	break;
	
      case OP_OPEN_CLOSE:
	/*
	 * Start opening these created files.
	 * Time how long it takes to do an open.
	 */
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
	    timer_value(&tv_op1);
	    usec_op1 += (tv_op1.tv_sec * 1000000) + tv_op1.tv_usec;

	    timer_start();
	    for (i=0; i<nfiles; i++) {
		if (close(fds[i]) != 0) {
		    perror("close");
		    fprintf(stderr, "close %s failed\n", namesp[i]);
		}
	    }
	    timer_stop();
	    timer_value(&tv_op2);
	    usec_op2 += (tv_op2.tv_sec * 1000000) + tv_op2.tv_usec;
	}
	if (aux_usage_end()) {
	    perror("getting rusage end failed");
	    exit(1);
	}
	break;
	
      case OP_STAT:
	if (aux_usage_start()) {
	    perror("getting rusage start failed");
	    exit(1);
	}
	timer_start();
	for (j=0; j<REPS; j++) {
	    for (i=0; i<nfiles; i++) {
		if (stat(namesp[i], &statbuf) != 0) {
		    perror("stat");
		    fprintf(stderr, "stat %s failed\n", namesp[i]);
		}
	    }
	}
	timer_stop();
	if (aux_usage_end()) {
	    perror("getting rusage end failed");
	    exit(1);
	}
	timer_value(&tv_op1);
	usec_op1 = (tv_op1.tv_sec * 1000000) + tv_op1.tv_usec;
	break;
	
      case OP_UNLINK:
	if (aux_usage_start()) {
	    perror("getting rusage start failed");
	    exit(1);
	}
	timer_start();
	for (i=0; i<nfiles; i++) {
	    if (unlink(namesp[i]) != 0) {
		fprintf(stderr, "unlink %s failed\n", namesp[i]);
		perror("unlink");
	    }
	}
	timer_stop();
	if (aux_usage_end()) {
	    perror("getting rusage end failed");
	    exit(1);
	}
	timer_value(&tv_op1);
	usec_op1 = (tv_op1.tv_sec * 1000000) + tv_op1.tv_usec;
	ops = nfiles;
	break;
	
      case OP_READDIR:
	if ((dirp = opendir(".")) == NULL) {
	    fprintf(stderr,"opendir . failed\n");
	    perror("opendir");
	    exit(1);
	}
	if (aux_usage_start()) {
	    perror("getting rusage start failed");
	    exit(1);
	}
	timer_start();
	for (j=0; j<REPS; j++) {
	    rewinddir(dirp);
	    for (i=0; i<nfiles; i++) {
		if ((direntp = readdir(dirp)) == NULL) {
		    break;
		}
	    }
	}
	timer_stop();
	if (aux_usage_end()) {
	    perror("getting rusage end failed");
	    exit(1);
	}
	timer_value(&tv_op1);
	usec_op1 = (tv_op1.tv_sec * 1000000) + tv_op1.tv_usec;
	if (closedir(dirp) != 0) {
	    fprintf(stderr, "closedir failed\n");
	    perror("closedir");
	    exit(1);
	}
	break;
	
      default:
	fprintf(stderr, "Unsupported test #%d\n", test);
	break;
    }

    /*
     * Retrieve the net rusage times
     */
    aux_net_usage(&u_time, &s_time);

    /*
     * Compute average seconds
     */
    usec_op1 = usec_op1 / ops;
    usec_op2 = usec_op2 / ops;
    time_op1 = (double)usec_op1 / 1000000;
    time_op2 = (double)usec_op2 / 1000000;

    /*
     * Display average times
     */
    fprintf(stdout, "Results for %s test (%d ops)\n", test_names[test], ops);
    fprintf(stdout, "\tAverage time: op1: %2.8f (%5.3f Ops/Sec)\n",
	    time_op1, (double) ops/time_op1);
    if (time_op2 != 0) {
	fprintf(stdout, "\tAverage time: op2: %2.8f (%5.3f Ops/Sec)\n",
		time_op2, (double) ops/time_op2);
    }
    fprintf(stdout, "\tCPU time utilization:\t User: %2.5f   System %2.5f\n",
	    u_time, s_time);

}
