/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: edquot.c,v $
 * Revision 1.1.63.1  1996/10/02  17:17:09  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:27:03  damon]
 *
 * Revision 1.1.57.2  1994/06/09  13:57:26  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:32:31  annie]
 * 
 * Revision 1.1.57.1  1994/02/04  20:11:09  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:09:07  devsrc]
 * 
 * Revision 1.1.55.1  1993/12/07  17:16:56  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  13:40:00  jaffe]
 * 
 * Revision 1.1.4.4  1993/01/18  22:03:08  cjd
 * 	embedded copyright notice
 * 	[1993/01/18  21:42:42  cjd]
 * 
 * Revision 1.1.4.3  1992/11/24  16:19:50  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:17:48  bolinger]
 * 
 * Revision 1.1.4.2  1992/08/31  18:33:36  jaffe
 * 	fixed RCSID
 * 	[1992/08/30  13:06:33  jaffe]
 * 
 * 	Transarc delta: mason-add-hp800-osi-routines 1.15
 * 	  Selected comments:
 * 	    This delta is misnamed - these are really porting fixes.
 * 	    Need to checkpoint to fix a different bug.
 * 	    Checkpoint the first system where test_anode works.  Only one change is not
 * 	    included here - the link line in anode/Makefile uses -lbsd.  On HP-UX, this
 * 	    is -lBSD.  This should be fixed more generally.
 * 	    More HP-UX changes.
 * 	    More HP-UX related changes.
 * 	    Fix compile problem on the RIOS.
 * 	    Fix more compilation problems.
 * 	    Checkpoint.
 * 	    Checpoint.
 * 	    In this snap we turned all malloc() calls into osi_Alloc calls.
 * 	    Also turned free calls into osi_Free.
 * 	    s/malloc/osi_Alloc/
 * 	    HP-UX EOF detection fix.
 * 	    Another checkpoint.
 * 	    This is a checkpoint of the first working HP-UX Episode system.
 * 	    Needed to add the vnops routines specific to HP-UX.
 * 	    Fix compilation problems.
 * 	[1992/08/30  01:47:53  jaffe]
 * 
 * Revision 1.1.2.2  1992/05/05  20:40:46  jdp
 * 	delta jdp-epimerge-1.0-to-1.0.1b9-into-2.17 1.20
 * 	[1992/05/04  20:18:53  jdp]
 * 
 * Revision 1.1  1992/01/19  02:36:55  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/* Copyright (C) 1990 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <errno.h>

RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/episode/anode/edquot.c,v 1.1.63.1 1996/10/02 17:17:09 damon Exp $")

PRIVATE char *whoami = "edquot";

#define ONE_MEG (1024*1024)

main(argc, argv)
  int argc;
  char *argv[];
{
    char *buf = (char *)osi_Alloc(ONE_MEG);
    char name[32];
    int i;				/* file number */
    int nFiles;				/* number of full size files created */
    int fd;				/* file descriptor */
    long code;
    int saveErrno;
    char *dirpath;

    osi_setlocale(LC_ALL, "");
    if (argc == 2) dirpath = argv[1];
    else dirpath = "/tmp";

    for (i=0; i<ONE_MEG; i++) buf[i] = 'q';

    nFiles = 0;
    for (i=0; ; i++) {
	sprintf (name, "%s/bigfile.%03d", dirpath, i);
	fd = open (name, O_RDWR+O_CREAT+O_TRUNC, 0777);
	if (fd == -1) {
	    com_err (whoami, errno, "opening %s", name);
	    exit(1);
	}
	code = write (fd, buf, ONE_MEG);
	if (code != ONE_MEG) break;
	code = close (fd);
	if (code) {
	    com_err (whoami, errno, "closing %s", name);
	    exit(2);
	}
	nFiles++;
	printf ("Write %s\n", name);
    }
    saveErrno = errno;
    printf ("failed writing %s", name);
    if (code == -1) {
	char *em = strerror (saveErrno);
	if (em && *em) printf (" with error '%s'", em);
    } else {
	printf (" after writing %d bytes", code);
    }
    {   struct stat info;
	code = fstat (fd, &info);
	if (code) {
	    com_err (whoami, errno, "stat'ing %s after failure", name);
	    exit(3);
	}
	printf ("\n  length is %d with %d allocated (%d*512)\n",
		info.st_size, info.st_blocks*512, info.st_blocks);
    }
    code = ftruncate (fd, 0);
    if (code) {
	com_err (whoami, errno, "truncating %s after failure", name);
	exit(4);
    }
    for (i=0; i<nFiles; i++) {		/* cleanup */
	char name[32];
	sprintf (name, "%s/bigfile.%03d", dirpath, i);
	code = unlink (name);
	if (code) {
	    com_err (whoami, errno, "deleting %s", name);
	    exit(5);
	}
	printf ("Deleted %s\n", name);
    }
    exit (0);
}
