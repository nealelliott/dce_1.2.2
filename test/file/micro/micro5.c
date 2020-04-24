/* 
 * Copyright (C) 1996, 1993  Transarc Corporation - All rights reserved 
 */
/*
 * Test background daemon rpc connection pool.
 * Measure cache store back write/close time.
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
#if	defined(AFS_AIX32_ENV)
#undef	_BSD
#endif
#include <sys/wait.h>


#define CM_CHUNKSIZE	(64*1024)
#define UNITS_PER_FILE	16	/* 1 MB in each file */
#define TFDATA		0x75 	/* u */
#define FNAMELEN	16
#define NFILES		10
#define NPROCS		1

extern int	in_subdir;	/* Flag to tell if we are in subdir */
char		*prog_name = NULL;
int		nfiles = NFILES, nprocs = NPROCS;

typedef struct c_result {
    int		res_proc;	/* the process number */
    long	res_rtime;	/* the running time in usecs */
    int		res_nfiles;	/* the number of files processed */ 
} cresult;

void usage()
{
    fprintf(stdout, "usage: %s [-h] [nfiles [procs]]\n", prog_name);
    fprintf(stdout, "  Flags:  h    Help - print this usage info\n");
    fprintf(stdout, "  nfiles - number of files to create\n");
    fprintf(stdout, "  procs  - the number or processes to run\n");
}


void slave_process(pnum, size)
    int pnum, size;
{
    int 	fd, i, startid, endid;
    char 	*buf, res_f[MAXPATHLEN], *p, *names, **namesp;
    struct timeval	tv = {0,0};
    long	slave_usec;

    /*
     * Create a list of the file names so as to avoid generating
     * them in-between open calls.  Sadly, it is easier to malloc
     * enough space to have all the file names, than to re-index
     * these file names to a local counter.
     */
    if ((namesp = (char **)malloc(nfiles * sizeof(char *))) == NULL) {
	fprintf(stderr, "Could not malloc pointer name space\n");
	exit(1);
    }
    if ((names = (char *)malloc(nfiles * FNAMELEN)) == NULL) {
	fprintf(stderr, "Could not malloc file name space\n");
	exit(1);
    }
    startid = pnum * (nfiles/nprocs);
    endid = ((pnum + 1) * (nfiles/nprocs)) - 1;
    /*
     * correct for odd distributions
     */
    if (nprocs == 1 || ((pnum == nprocs - 1) && (endid != nfiles - 1)))
	endid = nfiles - 1;

    p = names;
    for (i=startid; i<=endid; i++) {
	namesp[i] = p;
	sprintf(p, "%s.%d", TFNAME, i);
	p += FNAMELEN;
    }

    /*
     * Allocate a buffer for writing into the files
     */
    if ((buf = (char *)malloc(size)) == NULL) {
	fprintf(stderr, "Could not malloc buffer space\n");
	exit(1);
    }
    (void) memset(buf, TFDATA, size);

#if	defined(DEBUG)
    fprintf(stderr, "Child %d handling %s to %s\n",
	    pnum, namesp[startid], namesp[endid]);
#endif	/* DEBUG */

    timer_start();
    for (i=startid; i<=endid; i++) {
	fd = open(namesp[i], O_CREAT|O_RDWR, 0640);
	if (fd < 0) {
	    perror("open test file failed");
	    exit(1);
	}

	if (write(fd, buf, size) != size) {
	    perror("write of buffer data failed\n");
	    exit(1);
	}

	if (close(fd) != 0)
	    perror("child close");

    }
    timer_stop();
    timer_value(&tv);
    slave_usec = (tv.tv_sec * 1000000) + tv.tv_usec;

    /*
     * Create the results file for this child.
     */
    sprintf(res_f, "results.child.%d", pnum);
    if ((fd = open(res_f, O_CREAT|O_RDWR, 0640)) < 0) {
	perror("open of results file failed");
	exit(1);
    }
    sprintf(res_f, "Child %4d %18d %8d", pnum, slave_usec, endid-startid+1);
    if (write(fd, res_f, 39) != 39) {
	perror("write of results file failed");
	exit(1);
    }
    (void) close(fd);
    (void) free(names);
    (void) free(namesp);

}

void clean_up(sig)
int	sig;
{
    char	fname[MAXPATHLEN];
    int		i;
    
    if (sig) fprintf(stderr, "Cleaning up now, sig = %d\n", sig);

    /*
     * Delete results files
     */
    for (i=0; i<nprocs; i++) {
	sprintf(fname, "results.child.%d", i);
	if (unlink(fname) < 0) {
	    perror("unlink");
	    fprintf(stderr, "could not remove %s\n", fname);
	}
    }

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
    char 	*opts, str[256];
    int 	status, d, f;
    unsigned 	size;
    pid_t	*pids, apid;
    FILE	*fp;
    long	write_usecs;
    double	write_time;
    cresult	*crsp;

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
	nfiles = atoi(*argv);
	if (nfiles <= 0 || nfiles > OPEN_MAX) {
	    fprintf(stderr,
		    "Invalid number of files specified, using default\n");
	    nfiles = NFILES;
	}
	argv++;
	argc--;
    }
    if (argc != 0) {
	nprocs = atoi(*argv);
	if (nprocs <= 0)
	    nprocs = NPROCS;
	argv++;
	argc--;
    }
    if (argc != 0) {
	fprintf(stderr, "too many parameters");
	usage();
	exit(1);
    }

    if (nfiles < nprocs) {
	fprintf(stderr,
		"Inappropriate number of files specified for %d procs, using default\n", nprocs);
	if (nprocs <= NFILES)
	    nfiles = NFILES;
	else
	    nfiles = nprocs;
    }

    fprintf(stderr, "\n# Micro5 Store back file test, %d procs %d files\n",
	    nprocs, nfiles);

    /*
     * Adjust user file mask
     */
    (void) umask(0);
    in_subdir = 0;
    size = UNITS_PER_FILE * CM_CHUNKSIZE; /* file size in bytes */

    if (make_testdir(0, NULL, 0) != 0) {
	fprintf(stderr, "make test directory failed, bailing out\n");
	exit(1);
    }

#if	defined(DEBUG)
    fprintf(stderr, "Created test directory\n");
#endif	/* DEBUG */

    /*
     * Now fork all processes, and let them go to work on the file
     */
    if ((pids = (pid_t *) malloc(nprocs * sizeof(pid_t))) == NULL) {
	perror("Can't get memory for pids");
	exit(1);
    }

    /*
     * Fork off children to do writes to these files.
     * Compute the files each pid will execute by giving them
     * they're base and number of files.
     */
    for (d=0; d<nprocs; d++) {
	pids[d] = fork();
	if (pids[d] < 0) {
	    perror("fork failed!\n");
	    exit(1);
	} else if (pids[d] == 0) {
	    /* child process, start up slave */
	    slave_process(d, size);
	    exit(0);
	    /* NOT REACHED */
	}

    }
    for (d=0; d<nprocs; d++) {
	apid = wait(&status);
	if (apid > 0) {
	    if (!WIFEXITED(status))
		fprintf(stderr, "Something went wrong with child %d\n",apid);
#if	defined(DEBUG)
	    else
		fprintf(stderr, "Child %d exited\n",apid);
#endif	/* DEBUG */
	} else {
	    fprintf(stderr, "Something wrong with wait -> %d\n",apid);
	    perror("wait");
	}
    }
    (void) free(pids);

    /*
     * All children are now done.  Collect up their timing
     * data from their individual files and collate.
     */
    if ((crsp = (cresult *)malloc(nprocs * sizeof (cresult))) == NULL) {
	perror("Can't get memory for results");
	exit(1);
    }

    for (d=0; d<nprocs; d++) {
	sprintf(str, "results.child.%d", d);
	if ((fp = fopen(str, "r")) == NULL) {
	    perror("open results");
	    fprintf(stderr, "failed to open results file %s\n", str);
	    exit(1);
	}
	(void) fscanf(fp, "%*s %d %ld %d",
		      &(crsp[d].res_proc),
		      &(crsp[d].res_rtime),
		      &(crsp[d].res_nfiles));

	(void) fclose(fp);
    }

    write_time = 0;
    write_usecs = 0;
    f = 0;
    for (d=0; d<nprocs; d++) {
        /*
         * Consistency check the file data (okay it's a little lame).
         */
        if (crsp[d].res_proc != d) {
            fprintf(stderr, "Data for %d contains results for child %d!\n",
                    d, crsp[d].res_proc);
            exit(1);
        }

	write_usecs += crsp[d].res_rtime;
	f += crsp[d].res_nfiles;
    }

    if (f != nfiles) {
	fprintf(stderr, "Accounting error, expected %d but found %d files\n",
		nfiles, f);
	exit(1);
    }
    
    write_usecs = write_usecs / nfiles;		/* avg usecs over all files */
    write_time = (double)write_usecs / 1000000;	/* convert to seconds */

    fprintf(stdout, "Average time: %2.5f (sec) -> %5.3f KB/sec\n",
	    write_time, ((double)size / write_time) / 1024);

    clean_up(0);
    /* NOT REACHED */
}
