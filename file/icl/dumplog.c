/* Copyright (C) 1994 Transarc Corporation - All rights reserved. */

#include <dcedfs/param.h>
#include <dcedfs/osi.h>
#include <dcedfs/lock.h>

#include <dcedfs/icl.h>

#include <sys/types.h>
#include <sys/stat.h>

/* This program produces dfstrace output based on an ICL log in a binary file
 * (instead of reading the log from the kernel as dfstrace does). */

main(int argc, char *argv[])
{
    int fd;
    struct stat info;
    int code;
    int nbytes;				/* number of bytes read from file */
    long len;
    char *buf;				/* buffer containing log data */
    struct icl_log log;

    if (argc != 2) {
 usage:
	fprintf (stderr, "Usage: %s <binary log file>\n", argv[0]);
	exit (1);
    }
    fd = open (argv[1], O_RDONLY);
    if (fd < 0) {
	perror ("dumplog: open");
	goto usage;
    }
    code = fstat (fd, &info);
    if (code) {
	perror ("stat");
	goto usage;
    }
    len = info.st_size;
    if (len%4 != 0) {
	fprintf (stderr,
		 "file should contain 32-bit words: length is %d\n", len);
	exit (2);
    }
    buf = osi_Alloc(len);
    nbytes = read (fd, buf, len);
    if (nbytes != len) {
	perror ("read short");
	goto usage;
    }
    bzero (&log, sizeof(log));
    log.logSize = len/4;
    log.datap = (long *)buf;
    log.firstUsed = 0;
    log.firstFree = log.logSize-1;
    icl_DumpLog (stdout, &log, ICL_DUMP_FORMATTED);
    exit (0);
}
