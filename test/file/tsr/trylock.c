/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: trylock.c,v $
 * Revision 1.1.117.1  1996/10/17  18:36:55  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:58:08  damon]
 *
 * Revision 1.1.112.1  1994/02/04  20:47:36  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:25:37  devsrc]
 * 
 * Revision 1.1.110.1  1993/12/07  17:46:37  jaffe
 * 	New File from Transarc 1.0.3a
 * 	[1993/12/02  22:06:20  jaffe]
 * 
 * $EndLog$
 */
/* Copyright (C) 1993 Transarc Corporation - All rights reserved */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

main(argc, argv)
int argc;
char *argv[];
{
    int fd;
    int ret;
    int noPause = 0;
    char *lockFile;
    struct flock fl;

    lockFile = argv[argc-1];
    if ((argc > 1) && (strcmp(argv[1], "-nopause") == 0))
    {
	noPause++;
	argc--;
    }

    if (argc != 2)
    {
	(void) fprintf(stderr, "usage: %s [ -nopause ] file\n", argv[0]);
	exit(1);
    }

    if ((fd = open(lockFile, O_RDWR|O_CREAT, 0777)) < 0)
    {
	(void) fprintf(stderr, "cannot open file %s\n", lockFile);
	exit(1);
    }

    (void) memset((char *)&fl, 0, sizeof(fl));
    fl.l_type = 2;	/* write lock */
    if (fcntl(fd, F_SETLK, &fl) < 0)
    {
	(void) fprintf(stderr, "Failed to get exclusive lock on file (errno=%d)\n", errno);
	exit(1);
    }
    (void) printf("Got lock\n");
    if (!noPause)
	pause();
    exit(0);
}
