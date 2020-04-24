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

RCSID ("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/utils/RCS/mount_lfs.c,v 1.5 1995/02/14 21:49:15 dlc Exp $")

#define	MAXARGS	64

#define	ARG_PREFIX	"-"

#define	PROG_SLOT		0
#define	DASH_FILESET_SLOT	1
#define	FILESET_SLOT		2
#define	DASH_MNTPOINT_SLOT	3
#define	MNTPOINT_SLOT		4
#define	MNTOPTS_SLOT		5

main(int argc, char *argv[])
{
    char epimount[BUFSIZ];
    char *epimount_args[MAXARGS];
    char *argbuf;
    int buflen;
    char **mnt_opts;
    char *readonly;
    int numopts;

    /* step over prg name */
    argc--; argv++;

    readonly = NULL;
    numopts = 0;
    /* process '-' prefixed arguments */
    while (argc && argv[0][0] == '-') {
	switch(argv[0][1]) {
	  case 'O':
	  case 'm':
	    fprintf(stderr,
		    "mount_lfs: %s not supported, ignoring...\n", *argv);
	    argc--; argv++;
	    break;
	  case 'r':
	    readonly = "-readonly";
	    argc--; argv++;
	    break;
	  case 'o':
	    argc--; argv++;

	    buflen = strlen(*argv) + 1 + (MAXARGS * strlen(ARG_PREFIX));
	    argbuf = malloc(buflen);
	    if (argbuf == NULL) {
		fprintf(stderr,
			"mount_lfs: unable to allocate space for arguments\n");
		exit(1);
	    }

	    mnt_opts = &epimount_args[MNTOPTS_SLOT];
	    if ((numopts = opts_to_argv(*argv, argbuf, buflen,
					mnt_opts, MAXARGS - MNTOPTS_SLOT,
					" \t,", ARG_PREFIX)) < 0) {
		fprintf(stderr,
			"mount_lfs: error processing mount options\n");
		exit(1);
	    }

	    /*
	     * Remove any options which may have been in vfstab for the
	     * benefit of a script, but which are meaningless to
	     * epimount.
	     */
	    numopts = remove_arg("-nobootmount", mnt_opts);
	    numopts = remove_arg("-bootmount", mnt_opts);

	    argc--; argv++;
	    break;
	  default:
	    fprintf(stderr, "mount_lfs: Unknown argument %s\n", *argv);
	    argc--; argv++;
	    break;
	}
    }
    /*
     * at this point argc should be 2 and the remaining args are the
     * fileset name and the mount point from the vfstab file.
     */
    if (argc != 2) {
	fprintf(stderr, "mount_lfs: incorrect invocation\n");
	exit(1);
    }

    /*
     * Build the command arguments for epimount
     */
    sprintf(epimount, "%s/bin/epimount", DCELOCAL_PATH);

    epimount_args[PROG_SLOT] = "epimount";
    epimount_args[DASH_FILESET_SLOT] = "-fileset";
    epimount_args[FILESET_SLOT] = argv[0];
    epimount_args[DASH_MNTPOINT_SLOT] = "-mountpoint";
    epimount_args[MNTPOINT_SLOT] = argv[1];

    if (readonly != NULL) {
	if (MNTOPTS_SLOT + numopts >= MAXARGS - 1) {
	    fprintf(stderr, "mount_lfs: no more argument space\n");
	    exit(1);
	}
	epimount_args[MNTOPTS_SLOT + numopts] = readonly;
	numopts++;
    }
    epimount_args[MNTOPTS_SLOT + numopts] = NULL;

    /*
     * Just exec over top of ourself
     */
    if (execv(epimount, epimount_args) < 0) {
	fprintf(stderr, "mount_lfs: exec of %s failed\n", epimount);
	exit(1);
    }

    /*NOTREACHED*/
    exit(0);
}
