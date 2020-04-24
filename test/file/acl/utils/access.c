/*
 * HISTORY
 * $Log:$
 * $EndLog$
 */

/* Copyright (C) 1994 Transarc Corporation
 *  All rights reserved.
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#ifdef AIX_AIX31_ENV 
#include <sys/access.h>
#endif

extern int errno;

main(int argc, char ** argv)
{
    char *object, *permstr;
    int mode = 0;
    int code;

    if (argc < 3) {
	fprintf(stderr, "usage: object perm\n");
	exit(1);
    }

    object = argv[1];
    permstr = argv[2];

    if (!strcmp("r", permstr)) {
	mode |= R_OK;
    } 
    if (!strcmp("w", permstr)) {
	mode |= W_OK;
    }
    if (!strcmp("x", permstr)) {
	mode |= X_OK;
    }

    code = access(object, mode);
    if (code < 0) {
	exit(errno);
    }
    return 0;
}
