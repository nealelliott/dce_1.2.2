/* 
 * Copyright (C) 1996, 1993  Transarc Corporation - All rights reserved 
 */
/*
 * Test cache cold open/read time for files of size N*CM_CHUNKSIZE,
 * Builds a tree on the server glue, then cold cache reads them.
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
#define SIZE_MAX	64	/* no more than 4 MB in each file */
#define SIZE_DEFAULT	16	/* 1 MB in each file */
#define TFDATA		0x75 	/* u */
#define FNAMELEN	16
#define NFILES		10

extern int	in_subdir;	/* Flag to tell if we are in subdir */
char		*prog_name = NULL, *glue_path = NULL;
int		nfiles = NFILES, size=SIZE_DEFAULT;

void usage()
{
    fprintf(stdout, "usage: %s [-h] [-s] path [nfiles] [size]\n", prog_name);
    fprintf(stdout, "  Flags:  h    Help - print this usage info\n");
    fprintf(stdout, "          s    Skip setup phase (already done)\n");
    fprintf(stdout, "  path   - UFS path to glued current directory\n");
    fprintf(stdout, "  nfiles - number of files to create\n");
    fprintf(stdout, "  size   - size of files (in chunks)\n");
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
    char 		*opts, *buf, namep[MAXPATHLEN], path[MAXPATHLEN];
    int 		i, fd, read_usec, fds[OPEN_MAX], skip_setup = 0;
    struct timeval 	tv_read;
    double		read_time, u_time, s_time;
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

	      case 's':		/* skip setup phase */
		skip_setup = 1;
		break;

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
    if (argc < 1) {
	usage();
	exit(1);
    } else {
	/*
	 * First remaining arg had better be the path to glued UFS 
	 */
	glue_path = *argv++;
	argc--;
    }

    if (argc != 0) {
	nfiles = atoi(*argv);
	if (nfiles <= 0 || nfiles > OPEN_MAX) {
	    fprintf(stderr,
		    "Invalid number of files specified, using default\n");
	    nfiles = NFILES;
	}
	argc--; argv++;
    }
    if (argc != 0) {
	size = atoi(*argv);
	if (size <= 0 || size > SIZE_MAX) {
	    fprintf(stderr,
		    "Invalid size of files specified, using default\n");
	    size = SIZE_DEFAULT;
	}
    }
    fprintf(stdout, "\n# Micro3 Cache Chunk test, %d files %d chunks \n",
	    nfiles, size);

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
    size = size * CM_CHUNKSIZE;
    if ((buf = (char *)malloc(size)) == NULL) {
	perror("malloc1");
	exit(1);
    }

    (void) memset(buf, TFDATA, size);

    if (!skip_setup) {
	/*
	 * Temporarily change to the UFS-GLUE path to this directory
	 * So that we can populate the tree without touching the cache.
	 * Populate the test directory with speficied number
	 * of files all CM_CHUNKSIZE bytes long.
	 */
	if (getcwd(path, MAXPATHLEN) == NULL) {
	    perror("getwd");
	    exit(1);
	}
	
#if	defined(DEBUG)
	fprintf(stderr, "Moving to glued dir: %s\n",glue_path);
#endif /* DEBUG */
	if (chdir(glue_path) < 0) {
	    perror("chdir glue");
	    exit(1);
	}

	if (make_testdir(nfiles, buf, size) != 0) {
	    fprintf(stderr, "Could not create test tree files");
	    exit(1);
	}

	/*
	 * Change protection on test directory so DFS user can
	 * alter it later.
	 */
	if (chmod(".", 0777) < 0)
	    perror("chmod failed");

	/*
	 * Switch back to our current directory...
	 */
	if (chdir(path) < 0) {
	    perror("chdir back");
	    exit(1);
	}
    }

    /*
     * ... and into the test directory via DFS.
     */
    if (chdir(TDNAME) < 0) {
	perror("chdir to test directory");
	exit(1);
    }

#if	defined(DEBUG)
    fprintf(stderr, "\tcreated test directory.\n");
#endif	/* DEBUG */


    /*
     * Open all the files now, and save the descriptors to avoid
     * measuring the open time.
     */
    for (i=0; i<nfiles; i++) {
	sprintf(namep, "%s.%d", TFNAME, i);
	if ((fds[i] = open(namep, O_RDONLY)) < 0) {
	    perror("open");
	    fprintf(stderr, "Open %s failed\n", namep);
	    exit(1);
	}
    }

    /*
     * Time reads of all the data for all the files
     */
    if (aux_usage_start()) {
	perror("getting rusage start failed");
	exit(1);
    }
    timer_start();
    for (i=0; i<nfiles; i++) {

	if (read(fds[i], buf, size) != size) {
	    perror("read");
	    fprintf(stderr, "read %d failed\n", i);
	    exit(1);
	}		    
    }
    timer_stop();
    if (aux_usage_end()) {
	perror("getting rusage end failed");
	exit(1);
    }

    /*
     * Close all the files now, to play fair.
     */
    for (i=0; i<nfiles; i++) {
	if (close(fds[i]) < 0) {
	    perror("close");
	    fprintf(stderr, "close %d failed\n", i);
	}
    }

    /*
     * Compute net time and rusage
     */
    timer_value(&tv_read);
    aux_net_usage(&u_time, &s_time);

    read_usec = (tv_read.tv_sec * 1000000) + tv_read.tv_usec;

    /*
     * Average the read time.
     */
    read_usec = read_usec / nfiles;

    /*
     * Compute seconds
     */
    read_time = (double)read_usec / 1000000;

    /*
     * Display average times
     */
    fprintf(stdout, "\tAverage time: %2.5f sec -> %5.3f KB/sec\n",
	    read_time, ((double)size / read_time) / 1024);
    fprintf(stdout, "\tCPU time utilization:\t User: %2.5f   System %2.5f\n",
	    u_time, s_time);

    (void) free(buf);
    clean_up(0);
    /* NOTREACHED */
}
