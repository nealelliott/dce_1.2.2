/*
 * (C) Copyright 1992 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * dirtest.c -- 
 *    Directory tests portion of the cache consistency tests.
 */
/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dirtest.c,v $
 * Revision 1.1.422.1  1996/10/17  18:17:49  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:49:48  damon]
 *
 * Revision 1.1.417.1  1994/02/04  20:40:05  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:22:20  devsrc]
 * 
 * Revision 1.1.415.1  1993/12/07  17:41:32  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  18:40:59  jaffe]
 * 
 * Revision 1.1.2.4  1993/01/11  18:25:14  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  16:56:44  htf]
 * 
 * Revision 1.1.2.3  1992/11/04  19:21:46  jaffe
 * 	Transarc delta: fred-cc-fix-socket-port 1.3
 * 	  Selected comments:
 * 	    Additional fixes were necessary.  A couple calls to htons were added and
 * 	    also normalized the optn() flags.  There values are different on AIX and
 * 	    OSF/1.
 * 	    No changes.
 * 	    Forgot to include new .h file.
 * 	[1992/11/04  17:34:29  jaffe]
 * 
 * Revision 1.1.2.2  1992/10/28  16:01:48  jaffe
 * 	Transarc delta: fred-add-cc-tests 1.2
 * 	  Selected comments:
 * 	    Added Mike Kazar's cache consistency tests to the test tree.
 * 	    Forgot to put a EndLog marker in the header comment.
 * 	    Added EndLog marker.
 * 	[1992/10/27  17:06:40  jaffe]
 * 
 * $EndLog$
 */

#include <sys/types.h>
#include <sys/file.h>
#include <errno.h>
#include <dirent.h>
#include <dcedfs/osi.h>

#define BASEDIRNAME	"tdir.mmt"
#define BASENAME	"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"

static ParseArgs(argc, argv, countp)
  int argc;
  char **argv;
  long *countp;
{
    register int i;

    /* setup defaults */
    *countp = 1;

    for(i=1; i<argc; i++) {
	if (strcmp(argv[i], "-count") == 0) {
	    *countp = atoi(argv[i+1]);
	    i++;	/* skip over next arg */
	}
	else {
	    printf("dirtest: unknown arg '%s'\n", argv[i]);
	    return -1;
	}
    }
    return 0;
}


/* directory stress test */
main(argc, argv)
  int argc;
  char **argv;
{
    char tbuffer[256];
    long count;
    long i;
    long code;
    DIR *tdirp;
    struct dirent *tdep;
    char *statep;
    char dirName[1024];

    sprintf(dirName, "%s.%d", BASEDIRNAME, getpid());

    code = ParseArgs(argc, argv, &count);
    if (code) exit(code);

    /* create and change to subdir */
    code = mkdir(dirName, 0777);
    if (code) {
	perror("mkdir");
	return -1;
    }
    code = chdir(dirName);
    if (code) {
	perror("chdir tdir");
	return -1;
    }

    /* create count files */
    printf("Creating %d files\n", count);
    for(i=0;i<count;i++) {
	sprintf(tbuffer, "%s%d", BASENAME, i);
	code = open(tbuffer, O_CREAT | O_TRUNC, 0666);
	if (code < 0) {
	    perror("create");
	    return -1;
	}
	close(code);
    }
    /* array for tracking file state during deletion */
    statep = (char *) malloc(count);
    bzero(statep, count);	/* initialize it */

    /* enumerate dir and ensure they're all there */
    printf("Verifying %d files\n", count);
    tdirp = opendir(".");
    if (!tdirp) {
	printf("can't opendir '.'\n");
	exit(1);
    }
    while(tdep = readdir(tdirp)) {
	if (strncmp(tdep->d_name, "abcd", 4) == 0) {
	    i = atoi(tdep->d_name+104);
	    if (i < 0 || i > count) {
		printf("read bogus dir entry '%s'\n", tdep->d_name);
		exit(1);
	    }
	    if (statep[i] == 0)
		statep[i] = 1;	/* found the file */
	    else {
		printf("found entry '%s' twice\n", tdep->d_name);
		exit(1);
	    }
	}
    }
    closedir(tdirp);
    for(i=0;i<count;i++) {
	if (statep[i] != 1) {
	    printf("file %d not in enumeration\n", i);
	    exit(1);
	}
    }

    /* delete all from dir */
    printf("Deleting %d files.\n", count);
    tdirp = opendir(".");
    if (!tdirp) {
	printf("can't opendir '.'\n");
	exit(1);
    }
    while(tdep = readdir(tdirp)) {
	if (strncmp(tdep->d_name, "abcd", 4) == 0) {
	    i = atoi(tdep->d_name+104);
	    if (i < 0 || i > count) {
		printf("read bogus dir entry '%s'\n", tdep->d_name);
		exit(1);
	    }
	    statep[i] = 2;	/* found the file again */
	    code = unlink(tdep->d_name);
	    if (code) {
		printf("failed to delete file '%s'\n", tdep->d_name);
		perror("unlink");
		return -1;
	    }
	}
    }
    closedir(tdirp);
    for(i=0;i<count;i++) {
	if (statep[i] != 2) {
	    printf("didn't find file %d when deleting\n", i);
	    exit(1);
	}
    }
    
    /* recheck that dir is now empty */
    printf("Verifying %d files are gone.\n", count);
    tdirp = opendir(".");
    if (!tdirp) {
	printf("can't opendir '.'\n");
	exit(1);
    }
    while(tdep = readdir(tdirp)) {
	if (strcmp(tdep->d_name, ".") == 0 || strcmp(tdep->d_name, "..") == 0)
	    continue;
	/* otherwise we found an unexpected file */
	printf("didn't expect to find file '%s' after deletion\n", tdep->d_name);
	exit(1);
    }
    closedir(tdirp);

    /* cd back up and remove the dir */
    code = chdir("..");
    if (code) {
	perror("second chdir");
	return -1;
    }
    code = rmdir(dirName);
    if (code) {
	perror("rmdir tdir");
	return -1;
    }

    /* done */
    printf("Done.\n");
    exit(0);
}
