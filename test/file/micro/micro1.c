/* 
 * Copyright (C) 1996, 1993  Transarc Corporation - All rights reserved 
 */
/*
 * Test cache store back concurrency write/close time.
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
#include <fcntl.h>
#include "aux.h"

#define CM_CHUNKSIZE	(64*1024)
#define TFDATA		0x5a
#define TEST_FNAME	"file.0"
#define TEST_REP	10

extern int	in_subdir;	/* Flag to tell if we are in subdir */
char		*prog_name = NULL;

void usage()
{
	fprintf(stdout, "usage: %s [-h] [fsize]\n", prog_name);
	fprintf(stdout, "  Flags:  h    Help - print this usage info\n");
	fprintf(stdout, "  fsize:       Number of 64K chunks to write\n");
}


void clean_up(sig)
int	sig;
{
	if (sig) fprintf(stderr, "Cleaning up now, sig = %d\n", sig);

	if (remove_testdir(0, in_subdir) != 0) {
	    fprintf(stderr, "Failure in cleaning up test directory\n");
	    exit(1);
	}

	exit(0);
}    

int main(argc, argv)
	int argc;
	char *argv[];
{
    int 		i, fd, size = CM_CHUNKSIZE * 16;
    struct timeval 	time;
    long		write_usec;
    double		write_time, u_time, s_time;
    char 		*buf, *opts;
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
		fprintf(stderr, "unknown option '%c'", *opts);
		usage();
		exit(1);
	    }
	}
	argc--;
	argv++;
    }
    if (argc != 0) {
	size = CM_CHUNKSIZE * atoi(*argv);
	argc--; argv++;
    }
    if (argc != 0) {
	fprintf(stderr, "too many parameters");
	usage();
	exit(1);
    }

    fprintf(stderr, "\n# Micro1 Cache store back test, file size=%d K\n",
	    size/1024);

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
    if ((buf = (char *)malloc(size)) == NULL) {
	perror("malloc of buffer space failed");
	exit(1);
    }

    /*
     * Adjust user file mask
     */
    (void) umask(0);

    (void) memset(buf, TFDATA, size);

    if (make_testdir(1, 0, 0) != 0) {
	fprintf(stderr, "make test directory failed, bailing out\n");
	exit(1);
    }

    write_usec = 0;
    if (aux_usage_start()) {
	perror("getting rusage start failed");
	exit(1);
    }
    for (i=0; i<TEST_REP; i++) {
	if ((fd = open(TEST_FNAME, O_CREAT|O_RDWR, 0640)) < 0) {
	    perror("open test file failed");
	    exit(1);
	}
	timer_start();
	if (write(fd, buf, size) != size)
	    perror("write failed");
	(void) close(fd);
	timer_stop();
	timer_value(&time);

	if (unlink(TEST_FNAME) != 0)
	    perror("unlink");
	
	/*
	 * Accumulate the times to do this TEST_REP times.
	 */
	write_usec += (time.tv_sec * 1000000) + time.tv_usec;
    }
    
    if (aux_usage_end()) {
	perror("getting rusage end failed");
	exit(1);
    }

    /*
     * Average the per write and close times
     */
    write_usec /= TEST_REP;

    /*
     * Compute seconds
     */
    write_time = (double)write_usec / 1000000;

    /*
     * Compute net rusage
     */
    aux_net_usage(&u_time, &s_time);

    /*
     * Display average time
     */
    fprintf(stdout, "\tAverage time %3.4f sec -> %6.2f Kbytes/sec\n",
	    write_time, ((double)size / write_time)/1024);
    fprintf(stdout, "\tCPU time utilization:\t User: %2.5f   System %2.5f\n",
	    u_time, s_time);

    (void) free(buf);

    clean_up(0);
    /* NOT REACHED */
}
