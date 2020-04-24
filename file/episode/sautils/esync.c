/*
 * Copyright (C) 1994 Transarc Corporation
 * All rights reserved.
 */

/* 
 * esync -- Utility to sync user-data and meta-data for episode vnodes.
 *
 * Usage:
 * 	esync [ -m ]
 *
 *		-m: sync meta-data only
 */

#include <stdio.h>
#include <dcedfs/stds.h>		/* For RCSID */
#include <dcedfs/syscall.h>		/* For afs_syscall */
#include <dcedfs/episode/efs_opcode.h>	/* For EPIOP_DEBUG */
#include <dcedfs/episode/efs_debug.h>	/* For EFS_DEBUG_VNOPS */

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/sautils/RCS/esync.c,v 1.2 1994/06/24 21:09:39 rajesh Exp $")

void usage();

char *progname;

void usage()
{
    fprintf(stderr, "Usage: %s [ -m ]\n", progname);
    exit(1);
}

main(int argc, char **argv)
{
    int metaOnly = 0;
    int code;

    progname = *argv++;
    argc--;

    if (argc > 1) 
	usage();
    if (argc) {
	if (argv[0][0] != '-')
	    usage();
	if (argv[0][1] != 'm')
	    usage();
	metaOnly = 1;
    }

    printf("Syncing Episode vnodes %s \n", metaOnly ? "(meta-data only)" : "");

    code = afs_syscall(AFSCALL_EPISODE, EPIOP_DEBUG, EFS_DEBUG_VNOPS,
		       EFS_DEBUG_VNOPS_SYNC_VNODES, 
		       metaOnly ? EFS_DEBUG_VNOPS_SYNC_VNODES_METAONLY : 0,
		       0 /* dummy  */);

    if (code < 0) {
	perror(progname);
	exit(1);
    }
    return 0;
}
