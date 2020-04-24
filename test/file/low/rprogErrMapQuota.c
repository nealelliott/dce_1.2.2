/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rprogErrMapQuota.c,v $
 * Revision 1.1.656.1  1996/10/17  18:29:46  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:54:57  damon]
 *
 * Revision 1.1.651.1  1994/02/04  20:47:17  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:25:28  devsrc]
 * 
 * Revision 1.1.649.1  1993/12/07  17:46:15  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:40:06  jaffe]
 * 
 * Revision 1.1.4.5  1993/03/09  21:23:59  jaffe
 * 	Transarc delta: jess-ot7194-fs-low-kerneldump 1.1
 * 	  Selected comments:
 * 	    Change the icl_KernelDump parameter according to the changes
 * 	    from icl utilities.
 * 	    See above.
 * 	[1993/03/09  20:59:17  jaffe]
 * 
 * Revision 1.1.4.4  1993/01/11  18:33:45  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:16:30  htf]
 * 
 * Revision 1.1.4.3  1992/11/24  21:46:47  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:47:01  bolinger]
 * 
 * Revision 1.1.4.2  1992/10/28  18:14:16  jaffe
 * 	Transarc delta: jess-add-dumpkernel-in-low-tests 1.5
 * 	  Selected comments:
 * 	    Add function call (icl_DumpKernel) in the test suite. In case of running
 * 	    error of C programmes in the test suite the "icl_DumpKernel" function
 * 	    will be called in errex() to do the kernel trace. The dump file is located
 * 	    in "/tmp/prog#.kernel.dump.pid". You have to have the debug flag set to
 * 	    have the dump file.
 * 	    See above.
 * 	    Process another C file and add "low" as dump file header.
 * 	    Add the icl_DumpKernel function call in this file.
 * 	    Turn on the prog3 debug flags.
 * 	    It should not create the zero length dumpfile when there is nothing wrong.
 * 	    Fix the problem.
 * 	[1992/10/28  14:41:54  jaffe]
 * 
 * Revision 1.1.2.3  1992/05/29  22:59:32  mason
 * 	Transarc delta: fred-3754-change-script-name 1.1
 * 	  Selected comments:
 * 	    To clean up a number of defects with the tests.  In addition to defect
 * 	    3754, the changes also fix defects 3750 and 3748.
 * 	    Added exit(0) statement so return code in invoking shell script
 * 	    would be correct.
 * 	[1992/05/29  12:50:46  mason]
 * 
 * Revision 1.1.2.2  1992/05/21  12:47:13  jaffe
 * 	Transarc delta: fred-remove-sys 1.1
 * 	  Selected comments:
 * 	    To remove dependency on the AFS sys command in the test scripts.
 * 	    To remove dependency on AFS sys command.
 * 	Transarc delta: rajesh-test-error-mapping 1.1
 * 	  Selected comments:
 * 	    Tests to check DFS error mapping.
 * 	    Program to test mapping of EDQUOT error in DFS
 * 	[1992/05/20  20:42:05  jaffe]
 * 
 * $EndLog$
 */
/*
 * Copyright (C) 1992 Transarc Corporation - All rights reserved
 *
 * Program to test mapping of EDQUOT error in DFS
 */ 

#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <sys/param.h>
#include <dirent.h>
#include <unistd.h>
#include <dcedfs/icl.h>
#include <string.h>


/*
#ifdef __OSF__
#include <sys/mount.h>
#else
#include <sys/vfs.h>
#endif

#ifdef _AIX
#include <sys/statfs.h>
#endif

#ifdef HP
#include <sys/vfs.h>
#endif

#ifdef MIPS
#include <sys/param.h>
#include <sys/mount.h>
#endif

#ifdef NEXT
#include <sys/vfs.h>
#endif

#ifdef RT
#include <sys/vfs.h>
#endif

#ifdef SPARC
#include <sys/vfs.h>
#endif

#ifdef SUN3
#include <sys/vfs.h>
#endif

#ifdef VAX
#include <sys/param.h>
#include <sys/mount.h>
#endif

#ifndef NEXT
#ifndef RT
#include <unistd.h>
#endif
#endif
*/

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#ifdef sun
/* solaris only has ENOSPC */
#define EDQUOT ENOSPC
#endif /* sun */

/* Maximum test fileset size quota in bytes */
#define MAX_FTS_QUOTA 4096 

void errex();
int debug=FALSE;

main(argc, argv)
     int argc;
     char **argv;
{
  char *progName;
  char *ftsMountDirPath;
  int ftsQuota;
  char buf[MAX_FTS_QUOTA];
  char errMsg[256];

  int pid;
  char tFile1[MAXPATHLEN];
  int fd;
  int i;
  int res;

  progName = argv[0];
  ftsMountDirPath = argv[1];
  ftsQuota=atoi(argv[2])*1024;
  if (ftsQuota > MAX_FTS_QUOTA) {
    printf("Maximum quota for test fileset is %d. \n", MAX_FTS_QUOTA);
    printf("Rerun with a fileset of appropriate quota \n");
    exit(1);
  }


  pid = getpid();

  umask(0);
  sprintf(tFile1, "%s/tfile2",ftsMountDirPath);
  if (( fd = open(tFile1, O_CREAT|O_RDWR|O_TRUNC|O_SYNC, 0777)) < 0 ) {
    errex("DFS ERROR: could not create file");
  }

  for (i = 0; i < ftsQuota+1; i++ ) {
    buf[i] = 'a';
  }

  res = write(fd, buf, ftsQuota+1);
  if (res != -1 ) {
    errex("DFS/LFS ERROR: write() succeeded despite exceeding fileset quota \n");
    exit(2);
  }
  else {
    if (errno != EDQUOT ) {
      sprintf(errMsg,"MAPPING ERROR: write() did not fail with errno %d (EDQUOT) \n but with errno %d", EDQUOT, errno);
      errex(errMsg);
    }
  }
  exit(0);
}


void errex(s) char *s;
{
        FILE *dumpfilep;
        char dumpfile[256];

        printf("\n%s failed; errno = %d\n",s,errno);
        if (debug) {
            sprintf(dumpfile,"/tmp/low.rprogErrMapQuota.kernel.dump.%d",getpid());
            dumpfilep = fopen(dumpfile, "w");
            icl_DumpKernel(dumpfilep, NULL, ICL_DUMP_FORMATTED);
            fclose(dumpfilep);
        }
        exit(1);
}


