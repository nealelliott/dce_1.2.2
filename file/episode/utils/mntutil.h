/* Copyright (C) 1995 Transarc Corporation - All rights reserved. */

/* $Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/utils/RCS/mntutil.h,v 1.2 1995/02/14 21:49:14 dlc Exp $ */

#include <sys/vfstab.h>

int get_vfs(char *mnt_arg,
	    struct vfstab *vp,
	    char *fstype);

void split_up(char *inargs,
	      char *outargs);

int opts_to_argv(const char *instr,
		 char *strbuf,
		 int buflen,
		 char **args,
		 const int nargs,
		 const char *seps,
		 const char *prefix);

int remove_arg(const char *torm,
	       char **args);
