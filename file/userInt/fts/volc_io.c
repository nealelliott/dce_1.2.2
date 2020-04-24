/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: volc_io.c,v $
 * $EndLog$
 */
/* Copyright (C) 1995, 1989 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>
#include <dcedfs/sysincludes.h>
#include <dce/rpc.h>
#include <dcedfs/ftserver_proc.h>
#include <dcedfs/queue.h> 
#include <dcedfs/volume.h>
#include <dcedfs/fldb_data.h>
#include <dcedfs/flserver.h>
#include <dcedfs/ftserver.h>
#include <dcedfs/ftserver_trans.h>
#include <dcedfs/cmd.h>
#include <dcedfs/compat.h>

#include <volc.h>

#if !defined(KERNEL)
#include <pthread.h>
#endif /* !KERNEL */

char programName[BUFSIZ];

char *AggrType[3] = {"ufs", "LFS", "aix3"};
int verbose = 0;

/*
 * This module contains functions that deal with I/O related operations.
 */


/*
 * It returns 1 if the named file, fileNamep, is ok else 0
 */
FileExists(fileNamep)
    char *fileNamep;
{
    int fd, code = 1;
    struct stat status;

    fd = open(fileNamep, O_RDONLY, 0444);
    if (fd < 0 || fstat(fd, &status) < 0) {
	VOLSERLOG(0, "Can't access file %s: %s\n", fileNamep, strerror(errno));
	code = 0;
    }
    if (fd >= 0) close(fd);
    return code;
}


/*
 * The main printing/logging interface for the package. Level of 0 means mandatory
 * printing else print only if the level and verbose have common bits on...
 */
VOLSERLOG(level, format, a,b,c,d,e,f,g,h,i,j,k,l,m,n)
    long level;
    char *format;
    char *a,*b,*c,*d,*e,*f,*g,*h,*i,*j,*k,*l,*m,*n;
{
    if (!level) {
	fprintf(stderr, format, a,b,c,d,e,f,g,h,i,j,k,l,m,n);
    } else if (verbose & level) {
	fprintf(stderr, format, a,b,c,d,e,f,g,h,i,j,k,l,m,n);
	fflush(stderr);
    }
}

/*
 * Tries to print a meaningful message for the "code", if appropriate.
 */
PrintVLError(whoami, code)
    char *whoami;
    long code;
{

    if (code == error_status_ok) return 0;

    switch (code) {
	case EACCES:
	    code = FTS_ERR_BADACCESS;
	    break;
	case ENOENT: case ENODEV:
	    code = FTS_ERR_NOVOL;
	    break;
	case EEXIST:
	    code = FTS_ERR_MULTIRWVOL;
	    break;
    }

    if (whoami == (char *)NULL)
      fprintf(stderr, "Error: %s\n", dfs_dceErrTxt(code));
    else
      fprintf(stderr, "Error in %s: %s\n", whoami, dfs_dceErrTxt(code));

    return code;
}

/*
 * Tries to print a meaningful message for the "code", if appropriate.
 */
PrintError(whoami, code)
char *whoami;
long code;
{
    return PrintVLError(whoami, code);
}
