/*
 * HISTORY
 * $Log$
 * $EndLog$
 */
/*
 * (C) Copyright 1994 Transarc Corporation
 * All Rights Reserved
 */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <utmpx.h>
#include <sys/vfstab.h>
#include <sys/wait.h>

#define DFS_RUN_LEVEL 3

static long GetRunLevel(void);

main(int argc, char **argv)
{
    struct vfstab vfsent;
    char *extra_args=NULL, cmd[BUFSIZ], exargs[BUFSIZ];
    int status;
    long runLevel;

    /* step over prg name */
    argc--; argv++;

    /* process '-' prefixed arguments */
    while (argc && argv[0][0] == '-') {
	switch(argv[0][1]) {
	  case 'O':
	  case 'm':
	  case 'r':
	    fprintf(stderr,
		    "umount_agfs: %s not supported, ignoring...\n", *argv);
	    argc--; argv++;
	    break;
	  case 'o':
	    argc--; argv++;
	    extra_args = *argv;
	    split_up(extra_args, exargs);
	    argc--; argv++;
	    break;
	  default:
	    fprintf(stderr, "umount_agfs: Unknown argument %s\n", *argv);
	    argc--; argv++;
	    break;
	}
    }
    /*
     * at this point argc should be 1 and the remaining string
     * can be either the mount point or the name of the
     * device to unmount from the vfstab file.  We have to disambiguate
     * now based on the vfstab file contents.
     */
    if (argc != 1) {
	fprintf(stderr, "umount_agfs: incorrect invocation\n");
	exit(1);
    }

    if (get_vfs(*argv, &vfsent, "agfs") != 0) {
	fprintf(stderr, "Could not determine device to unmount\n");
	exit(1);
    }

    runLevel = GetRunLevel();
    if (runLevel > -1 && runLevel < DFS_RUN_LEVEL)
	(void)putenv("DFSEXPORT_NO_TOKENS=1");

    /* just shuffle together the args for dfsexport */
    if (extra_args != NULL)
	sprintf(cmd, "/opt/dcelocal/bin/dfsexport %s -detach %s",
		vfsent.vfs_special, exargs);
    else
	sprintf(cmd, "/opt/dcelocal/bin/dfsexport %s -detach",
		vfsent.vfs_special);

    status = system(cmd);
    exit(WEXITSTATUS(status));
}


static long GetRunLevel(void)
{
   long			rlev = -1;
   struct utmpx         ut;
   struct utmpx*        utp;
   char*		c;
 
   ut.ut_type = RUN_LVL;
   utp = getutxid(&ut);
   if (utp == NULL) {
      perror("getutxid");
      return -1;
  }

   c = utp->ut_line;
   while (*c != '\0' && !isdigit(*c))
       ++c;

   if (*c != '\0')
       rlev = strtol(c, NULL, 0);

   return rlev;
}
