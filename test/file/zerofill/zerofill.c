/* Copyright (C) 1994 Transarc Corporation - All rights reserved. */

/*
 * $Header: /afs/tr/project/fs/dev/dfs-dev/src/test/file/zerofill/RCS/zerofill.c,v 1.2 1994/07/07 16:17:20 blake Exp $
 */

/*
 * zerofill tests for correct file system operation, particularly
 * with respect to holes.  It repeatedly grows a file through a series
 * of randomly-sized forward seeks, writes, and truncates (in either
 * direction), and checks that the unwritten parts of the file contain
 * zeroes and the parts written to contain the correct data.
 *
 * Episode passes the test, but many other file systems (e.g., UFS on
 * SunOS) do not.
 *
 * Usage:
 *    zerofill  [-f file] [-n niter] [-s step] [-l filelen] [-r seed]
 *
 * where
 *    -f file	is the name of the data file.  (default zerofill.data)
 *    -n niter	is the number of times to grow the file from zero to
 *		its maximum length.  (default 250)
 *    -s step	controls the amount by with the file is extended on
 *		each seek-write-truncate cycle.  The length will change
 *		by an amount between -step and 6 * step, with an average
 *		around 1.5 * step.  (default 4096)
 *    -l filelen is the length to which we will grow the data file
 *		(plus some small random amount).  (default 1000000)
 *    -r seed	is a seed for the random number generator, to allow
 *		reproducible results.  (default = current time)
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/stat.h>

#ifndef roundup
#define roundup(x, y)	((((x) + (y) - 1) / (y)) * (y))
#endif

#ifndef PAGESIZE
#define PAGESIZE 4096
#endif

void
Quit(char *why)
{
    perror(why);
    exit(1);
}

/*
 * GetRandom -- returns a uniformly distributed random integer in the
 *     interval [0-n).
 */
int
GetRandom(int n)
{
    return (n * drand48());
}

/*
 * GetRandomSkewed -- returns a random integer in the interval [0-n) whose
 * distribution is skewed towards zero (mean n/3, variance 4n/45).
 */
int
GetRandomSkewed(int n)
{
    double f = drand48();
    return (f * f * n);
}

/*
 * GetRandomBoolean -- returns a random TRUE or FALSE value.
 */
int
GetRandomBoolean(void)
{
    long l = lrand48();
    return ((l & 0x4000) == 0);
}

/*
 * Return length of file corresponding to fd.
 */
int
FGetLength(int fd)
{
    struct stat b;
    int code = fstat(fd, &b);
    if (code)
	Quit("stat");
    return b.st_size;
}

#define NITER		250
#define FILE_LEN	1000000

/*
 * Program parameters:
 */
char *filename = "zerofill.data";	/* the test file */
long fileLen = 1000000;			/* length to which we grow it */
int psize = PAGESIZE;			/* step size param. - see below */
int niter = NITER;			/* No. of iterations to do */
long seed = 0;				/* Seed for RNG */

u_char *buf;				/* Data buffer */
long totalNbytes, totalNzero;		/* Summary statistics */

/*
 * Extend the file by a random amount.  We do this via a three-step
 * procedure:
 *
 * (1) We seek forward from the current EOF by a random amount,
 * (2) We write a random amount of data,
 * (3) We truncate the file by a random increment (+ or -).
 *
 * Following this we verify that the unwritten portions of the
 * region between the old and new EOF contain zeroes and that the
 * part that we wrote to has the correct data.
 *
 * Each call will change the file's length by an amount between -psize
 * and 6 * psize, with an average of about 1.5 * psize.  The updated
 * length is returned.
 */
off_t
ExtendFile(int fd, off_t len)
{
    off_t wrpos;		/* where we start writing */
    int dist, i, code;
    off_t newLen;		/* file's reported length */
    int nwrite;			/* # non-zero bytes to append */
    int nskip;			/* # bytes skipped by seek */
    int ntrunc;			/* # bytes skipped by ftruncate */
    static u_char dataVal = 0xa5; /* data value for writes */

    /*
     * Seek forward by a random amount.  Get distance to next "page"
     * boundary and randomly select a position  that is near there
     * (plus or minus).
     */
    dist = roundup(len + 1, psize) - len;
    nskip = dist + (GetRandomBoolean() ?
	-GetRandomSkewed(dist) : GetRandomSkewed(2 * psize));
    assert(nskip > 0 && nskip < 3 * psize);
    wrpos = len + nskip;
    code = lseek(fd, wrpos, SEEK_SET);
    if (code != wrpos)
	Quit("extending lseek");

    /*
     * Write up to 2 "pages" of data.
     */
    nwrite = GetRandom(2 * psize) + 1;
    memset(buf, dataVal, nwrite);
    code = write(fd, buf, nwrite);
    if (code != nwrite)
	Quit("write");

    newLen = FGetLength(fd);
    if (wrpos + nwrite != newLen) {
	fprintf(stderr, "File's length is %d but should be %d\n",
	 newLen, wrpos + nwrite);
	exit(2);
    }

    /*
     * Check that there are zeroes in the region that we
     * skipped over.
     */
    code = lseek(fd, len, SEEK_SET);
    if (code != len)
	Quit("lseek to old EOF");
    code = read(fd, buf, nskip);
    if (code != nskip)
	Quit("reading skipped region");

    for (i = 0; i != nskip; i++) {
	if (buf[i] != 0) {
	    fprintf(stderr, "Byte at %d+%d was %#x, not zero\n",
			len, i, buf[i]);
	    exit(2);
	}
    }

    /*
     * Verify the data that we wrote.
     */
    code = lseek(fd, wrpos, SEEK_SET);
    if (code != wrpos)
	Quit("lseek to written data");

    code = read(fd, buf, nwrite);
    if (code != nwrite) {
	perror("reading back data");
	fprintf(stderr,
	 "Short data read from: %d, req: %d, got: %d, len: %d\n",
	 wrpos, nwrite, code, newLen);
	exit(2);
    }

    for (i = 0; i < nwrite; i++) {
	if (buf[i] != dataVal) {
	    fprintf(stderr, "Byte at %d+%d was %#x, not %#x\n",
			wrpos, i, buf[i], dataVal);
	    exit(2);
	}
    }

    /*
     * Truncate the file by up to psize in either direction.
     */
    ntrunc = GetRandom(2 * psize + 1) - psize;
    if (ntrunc + newLen < 0)
	ntrunc = -newLen;
    newLen += ntrunc;
    code = ftruncate(fd, newLen);
    if (code == -1)
	Quit("ftruncate");

    /*
     * Check for zeroes after truncating upward.
     */
    if (ntrunc > 0) {
	code = lseek(fd, newLen - ntrunc, SEEK_SET);
	if (code != newLen - ntrunc)
	    Quit("lseek after truncating");
	code = read(fd, buf, ntrunc);
	if (code != ntrunc) {
	    perror("read from truncated region");
	    fprintf(stderr,
		"Short data read from:%d, req:%d, got:%d, len:%d\n",
		newLen - ntrunc, ntrunc, code, newLen);
	    exit(2);
	}
	for (i = 0; i < ntrunc; i++) {
	    if (buf[i] != 0) {
		fprintf(stderr,
			"Trunc. byte at %d+%d was %#x, not zero\n",
			newLen - ntrunc, i, buf[i]);
		exit(2);
	    }
	}
	totalNzero += ntrunc;
    } else if (nwrite + ntrunc >= 0)
	totalNbytes += ntrunc;	/* written data partly truncated */
    else {
	totalNbytes -= nwrite;
	totalNzero += nwrite + ntrunc;	/* skipped region truncated */
    }

    dataVal++;		/* Increment for next extension */

    totalNbytes += nwrite;
    totalNzero += nskip;

    return (newLen);
}

void
WriteFile(void)
{
    int fd;			/* file descriptor */
    off_t l;			/* file's current length */

    fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0777);
    if (fd == -1)
	Quit("open");

    l = 0;
    while (l < fileLen)
	l = ExtendFile(fd, l);

    if (close(fd) == -1)
	Quit("close");
}

void
Usage(char *progname)
{
    fprintf(stderr,
	"Usage: %s [-f file] [-n niter] [-s step] [-l filelen] [-r seed]\n",
	 progname);
    exit(1);
}

main(int argc, char **argv)
{
    int i, c;
    extern char *optarg;
    extern int optind;

    seed = time((time_t *)0);

    while ((c = getopt(argc, argv, "f:n:s:l:r:")) != EOF) {
	switch (c) {
	  case 'f':
	    filename = optarg;
	    break;

	  case 'n':
	    niter = atoi(optarg);
	    if (niter < 0)
		Usage(argv[0]);
	    break;

	  case 'l':
	    fileLen = atoi(optarg);
	    if (fileLen <= 0)
		Usage(argv[0]);
	    break;

	  case 's':
	    psize = atoi(optarg);
	    if (psize <= 0)
		Usage(argv[0]);
	    break;

	  case 'r':
	    seed = atoi(optarg);
	    break;

	  case '?':
	    Usage(argv[0]);
	}
    }

    if (optind < argc)
	Usage(argv[0]);

    srand48(seed);
    buf = malloc(3 * psize);
    if (buf == NULL)
	Quit("malloc");

    for (i = 0; i != niter; i++) {
	totalNbytes = totalNzero = 0;
	WriteFile();
	printf("File #%d: wrote %d bytes and skipped %d bytes\n",
		i, totalNbytes, totalNzero);
    }

    exit(0);
}
