/* 
 * Copyright (C) 1996  Transarc Corporation - All rights reserved 
 */
/*
 * Test read and write times for files of varying size.
 * Builds a tree on the server, then performs tests.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>

#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
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

#define CHUNKSIZE	(8*1024)
#define SIZE_MAX	8192	/* no more than 64 MB in each file */
#define SIZE_DEFAULT	128	/* 1 MB in each file */
#define IOSIZE_MAX	256	/* 2 MB max IO */
#define IOSIZE_DEFAULT	8	/* 64 K default IO */
#define TFDATA		0x75 	/* u */
#define FNAMELEN	16
#define NFILES		10

#define OP_READ_SEQ	1
#define OP_READ_RAN	2
#define OP_WRITE_SEQ	3
#define OP_WRITE_RAN	4
#define DEBUG 1

char	*testnames[] = {"NULL",
			"SEQUENTIAL READ",
			"RANDOM READ",
			"SEQUENTIAL WRITE",
			"RANDOM WRITE",
			"INVALID"};
extern int	in_subdir;	/* Flag to tell if we are in subdir */
char		*prog_name = NULL, *glue_path = NULL, *buf;
int		nfiles = NFILES, size = SIZE_DEFAULT, iosize = IOSIZE_DEFAULT;
int		*offsets, num_ops, fds[OPEN_MAX];
void run_io_test(int test);

void usage()
{
    fprintf(stdout, "usage: %s [-h] [-s|-t] path [nfiles] [size] [iosize]\n",
	    prog_name);
    fprintf(stdout, "  Flags:  h    Help - print this usage info\n");
    fprintf(stdout, "          s    Perform only setup phase\n");
    fprintf(stdout, "          t    Perform only test phase\n");
    fprintf(stdout, "  path   - UFS path to glued current directory\n");
    fprintf(stdout, "  nfiles - number of files to create\n");
    fprintf(stdout, "  size   - size of files (in chunks)\n");
    fprintf(stdout, "  iosize - max size of each read/write (in chunks)\n");
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
    char 		*opts, path[MAXPATHLEN];
    int 		i, setup = 0, test = 0;
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

	      case 's':		/* do only setup phase */
		setup = 1;
		break;

	      case 't':		/* do only test phase */
		test = 1;
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
    if (setup && test) {
	usage();
	exit(1);
    } else if (!setup && !test) {
	setup = test = 1;	/* warm cache version, do both */
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
	argc--; argv++;
    }
    if (argc != 0) {
	iosize = atoi(*argv);
	if (iosize <= 0 || iosize > IOSIZE_MAX || iosize > size) {
	    fprintf(stderr,
		    "Invalid I/O size specified, using default\n");
	    iosize = MIN(size, IOSIZE_DEFAULT);
	}
	argc--; argv++;
    }

    size = size * CHUNKSIZE;
    iosize = iosize * CHUNKSIZE;
    fprintf(stdout, "\n# Micro IO perf - %d files size=%d K iosize=%d K\n",
	    nfiles, size/1024, iosize/1024);

    /*
     * Set up signal handler to clean up if we bail out
     */
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
     * Set up an offset list to use in random read/write tests
     */
    num_ops = size/iosize;
    if ((offsets = (int *)malloc(num_ops * sizeof(int))) == NULL) {
	perror("malloc2");
	exit(1);
    }

    /*
     * Initialize the offsets to be the increasing integers.
     */
    for (i = 0; i < num_ops; i++) {
	offsets[i] = i;
    }
    /*
     * Randomize the offsets by swapping num_ops/2 pairs of them
     * selected via rand().
     */
    for (i = 0; i < num_ops/2; i++) {
	int r1 = (rand() % num_ops);
	int r2 = (rand() % num_ops);
	int hold;

	hold = offsets[r1];
	offsets[r1] = offsets[r2];
	offsets[r2] = hold;
    }
#if	defined(DEBUG)
    printf("Will perform %d operations. ", num_ops);
    printf("Buffer size is %d K bytes\n Order is: ", iosize/1024);
    for (i = 0; i < num_ops; i++) {
	printf("%d ", offsets[i]);
    }
    printf("\n");
#endif	/* DEBUG */

    /*
     * Set up a buffer with some data to fill into files
     */
    if ((buf = (char *)malloc(size)) == NULL) {
	perror("malloc1");
	exit(1);
    }

    (void) memset(buf, TFDATA, size);

    if (setup) {
	/*
	 * Temporarily change to the UFS-GLUE path to this directory
	 * So that we can populate the tree without touching the cache.
	 * Populate the test directory with speficied number
	 * of files all CHUNKSIZE bytes long.
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

    if (!test) {
	free(buf);
	exit(0);
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

    run_io_test(OP_READ_SEQ);
    run_io_test(OP_READ_RAN);
    run_io_test(OP_WRITE_SEQ);
    run_io_test(OP_WRITE_RAN);

    (void) free(buf);
    clean_up(0);
    /* NOTREACHED */
}


void run_io_test(int test)
{
    int			op_usec, i, j, bytes_left, this_op_size, this_op_offset;
    char		namep[MAXPATHLEN];
    struct timeval 	tv_op;
    double		op_time, u_time, s_time;

    /*
     * Open all the files now, and save the descriptors to avoid
     * measuring the open time.
     */
    for (i=0; i<nfiles; i++) {
	sprintf(namep, "%s.%d", TFNAME, i);
	if ((fds[i] = open(namep, O_RDWR)) < 0) {
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

    switch(test) {
      case OP_READ_SEQ:
	timer_start();
	for (i=0; i<nfiles; i++) {
	    bytes_left = size;
	    for (j=0; j<num_ops; j++) {
		this_op_size = MIN(bytes_left, iosize);
		if (read(fds[i], buf, this_op_size) != this_op_size) {
		    perror("read 1");
		    fprintf(stderr, "read %d [%d bytes] failed\n",
			    i, this_op_size);
		    exit(1);
		}		    
		bytes_left -= this_op_size;
	    }
	}
	timer_stop();
	break;

      case OP_READ_RAN:
	timer_start();
	for (i=0; i<nfiles; i++) {
	    for (j=0; j<num_ops; j++) {
		this_op_offset = offsets[j] * iosize;
		bytes_left = size - this_op_offset;
		this_op_size = MIN(bytes_left, iosize);
		if (lseek(fds[i], (off_t)this_op_offset, SEEK_SET) < 0) {
		    perror("lseek 1");
		    exit(1);
		}
		if (read(fds[i], buf, this_op_size) != this_op_size) {
		    perror("read 2");
		    fprintf(stderr, "read %d [%d bytes] failed\n",
			    i, this_op_size);
		    exit(1);
		}		    
	    }
	}
	timer_stop();
	break;

      case OP_WRITE_SEQ:
	timer_start();
	for (i=0; i<nfiles; i++) {
	    bytes_left = size;
	    for (j=0; j<num_ops; j++) {
		this_op_size = MIN(bytes_left, iosize);
		if (write(fds[i], buf, this_op_size) != this_op_size) {
		    perror("write 1");
		    fprintf(stderr, "write %d [%d bytes] failed\n",
			    i, this_op_size);
		    exit(1);
		}		    
		bytes_left -= this_op_size;
	    }
	}
	timer_stop();
	break;
      case OP_WRITE_RAN:
	timer_start();
	for (i=0; i<nfiles; i++) {
	    for (j=0; j<num_ops; j++) {
		this_op_offset = offsets[j] * iosize;
		bytes_left = size - this_op_offset;
		this_op_size = MIN(bytes_left, iosize);
		if (lseek(fds[i], (off_t)this_op_offset, SEEK_SET) < 0) {
		    perror("lseek 2");
		    exit(1);
		}
		if (write(fds[i], buf, this_op_size) != this_op_size) {
		    perror("write 2");
		    fprintf(stderr, "write %d [%d bytes] failed\n",
			    i, this_op_size);
		    exit(1);
		}		    
	    }
	}
	timer_stop();
	break;

      default:
	fprintf(stderr, "Unsupported test #%d\n", test);
	break;
    }


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
    timer_value(&tv_op);
    aux_net_usage(&u_time, &s_time);

    op_usec = (tv_op.tv_sec * 1000000) + tv_op.tv_usec;

    /*
     * Average the op time.
     */
    op_usec = op_usec / nfiles;

    /*
     * Compute seconds
     */
    op_time = (double)op_usec / 1000000;

    /*
     * Display average times
     */
    fprintf(stdout, "Results for %s test:\n", testnames[test]);
    fprintf(stdout, "\tAverage time: %2.5f sec -> %5.3f KB/sec\n",
	    op_time, ((double)size / op_time) / 1024);
    fprintf(stdout, "\tCPU time utilization:\t User: %2.5f   System %2.5f\n",
	    u_time, s_time);

}
