/*
 * (C) Copyright 1995, 1994 Transarc Corporation
 * All Rights Reserved
 */

#include <dcedfs/stds.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

#include <mntutil.h>

RCSID ("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/utils/RCS/mount_agfs.c,v 1.6 1995/02/14 21:49:15 dlc Exp $")

#define	MAXARGS	64

#define	ARG_PREFIX	"-"

#define	PROG_SLOT	0
#define	DASH_AGGR_SLOT	1
#define	AGGR_SLOT	2
#define	MNTOPTS_SLOT	3

main(int argc, char *argv[])
{
    char dfsexport[BUFSIZ];
    char *dfsexport_args[MAXARGS];
    char **mnt_opts;
    char *argbuf;
    int buflen;
    int numopts;

    /* step over prg name */
    argc--; argv++;

    numopts = 0;

    /* process '-' prefixed arguments */
    while (argc && argv[0][0] == '-') {
	switch(argv[0][1]) {
	  case 'O':
	  case 'm':
	  case 'r':
	    fprintf(stderr,
		    "mount_agfs: %s not supported, ignoring...\n", *argv);
	    argc--; argv++;
	    break;
	  case 'o':
	    argc--; argv++;

	    buflen = strlen(*argv) + 1 + (MAXARGS * strlen(ARG_PREFIX));
	    argbuf = malloc(buflen);

	    if (argbuf == NULL) {
		fprintf(stderr,
			"mount_agfs: can not allocate space for arguments\n");
		exit(1);
	    }

	    mnt_opts = &dfsexport_args[MNTOPTS_SLOT];
	    if ((numopts = opts_to_argv(*argv, argbuf, buflen,
				       mnt_opts, MAXARGS - MNTOPTS_SLOT,
				       " \t,", ARG_PREFIX)) < 0) {
		fprintf(stderr,
			"mount_agfs: error processing mount options\n");
		exit(1);
	    }

	    /*
	     * Remove any options that may have been in the vfstab file
	     * for the benefit of a script, but are meaningless to dfsexport
	     * and may have been passed to us.
	     */
	    numopts = remove_arg("-nobootmount", mnt_opts);
	    numopts = remove_arg("-bootmount", mnt_opts);

	    argc--; argv++;
	    break;
	  default:
	    fprintf(stderr, "mount_agfs: Unknown argument %s\n", *argv);
	    argc--; argv++;
	    break;
	}
    }
    /*
     * at this point argc should be 2 and the remaining args are the special
     * device and the mount point from the vfstab file.  We only use the
     * special device, and ignore the mount point.
     */
    if (argc != 2) {
	fprintf(stderr, "mount_agfs: incorrect invocation\n");
	exit(1);
    }

    /*
     * Build the command arguments for dfsexport
     */
    sprintf(dfsexport, "%s/bin/dfsexport", DCELOCAL_PATH);

    dfsexport_args[PROG_SLOT] = "dfsexport";
    dfsexport_args[DASH_AGGR_SLOT] = "-aggregate";
    dfsexport_args[AGGR_SLOT] = argv[0];
    dfsexport_args[MNTOPTS_SLOT + numopts] = NULL;

    /*
     * Just exec over top of ourself
     */
    if (execv(dfsexport, dfsexport_args) < 0) {
	fprintf(stderr, "mount_agfs: exec of %s failed\n", dfsexport);
	exit(1);
    }

    /*NOTREACHED*/
    exit(0);
}
