/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: checksum.c,v $
 * Revision 1.1.8.1  1996/10/17  18:34:00  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:56:54  damon]
 *
 * Revision 1.1.3.1  1994/07/13  22:31:44  devsrc
 * 	moving files from src/file/episode to test/file
 * 	[1994/03/22  20:40:19  delgado]
 * 
 * Revision 1.1.21.1  1994/02/04  20:14:33  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:11:00  devsrc]
 * 
 * Revision 1.1.19.1  1993/12/07  17:20:10  jaffe
 * 	New File from Transarc 1.0.3a
 * 	[1993/12/02  20:54:30  jaffe]
 * 
 * $EndLog$
 */
/* Copyright (C) 1993 Transarc Corporation - All rights reserved */

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdio.h>

#define BUF_SIZE (32*1024)
char program[512];
char errmsg[512];
int debug = 0;

void usage ()
{
    fprintf(stderr,
	    "Usage: %s <file> \n", program);
    exit(1);
}

main(int argc, char **argv)
{
    char *file, *buffer;
    int len;
    int rfd;
    unsigned long checksum;
    
    if (argc < 2)  {
	usage();
    }

    file = argv[1];

    buffer = (char *) calloc(1, BUF_SIZE);
    if (buffer == (char *)NULL) {
	fprintf(stderr, "%s: Could not allocate memory for buffer\n",
		argv[0]);
	exit(1);
    }

    rfd = open(file, O_RDONLY);
    if (rfd < 0) {
	sprintf(errmsg, "Could not open input file %s", file);
	perror(errmsg);
	exit(1);
    }

    checksum = 0;
    while ( (len = read(rfd, buffer, BUF_SIZE)) > 0 ) {
	CalcCRC32Std (buffer, len, &checksum);
    }

    close(rfd);
    (void) free((void *)buffer);

    printf("%lu \n", checksum);
    
    return 0;
}
