/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: progErrMap.c,v $
 * Revision 1.1.10.1  1996/10/17  18:29:39  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:54:54  damon]
 *
 * Revision 1.1.4.5  1993/03/09  21:23:41  jaffe
 * 	Transarc delta: jess-ot7194-fs-low-kerneldump 1.1
 * 	  Selected comments:
 * 	    Change the icl_KernelDump parameter according to the changes
 * 	    from icl utilities.
 * 	    See above.
 * 	[1993/03/09  20:57:47  jaffe]
 * 
 * Revision 1.1.4.4  1993/01/11  18:33:39  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:16:09  htf]
 * 
 * Revision 1.1.4.3  1992/11/24  21:46:31  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:46:42  bolinger]
 * 
 * Revision 1.1.4.2  1992/10/28  18:13:49  jaffe
 * 	Transarc delta: jess-add-dumpkernel-in-low-tests 1.5
 * 	  Selected comments:
 * 	    Add function call (icl_DumpKernel) in the test suite. In case of running
 * 	    error of C programmes in the test suite the "icl_DumpKernel" function
 * 	    will be called in errex() to do the kernel trace. The dump file is located
 * 	    in "/tmp/prog#.kernel.dump.pid". You have to have the debug flag set to
 * 	    have the dump file.
 * 	    See above.
 * 	    See above.
 * 	    Process another C file and add "low" as dump file header.
 * 	    Turn on the prog3 debug flags.
 * 	    It should not create the zero length dumpfile when there is nothing wrong.
 * 	    Fix the problem.
 * 	[1992/10/28  14:36:42  jaffe]
 * 
 * Revision 1.1.2.3  1992/05/29  22:59:29  mason
 * 	Transarc delta: fred-3754-change-script-name 1.1
 * 	  Selected comments:
 * 	    To clean up a number of defects with the tests.  In addition to defect
 * 	    3754, the changes also fix defects 3750 and 3748.
 * 	    Added exit(0) statement so return code in invoking shell script
 * 	    would be correct.
 * 	[1992/05/29  12:50:04  mason]
 * 
 * Revision 1.1.2.2  1992/05/21  12:47:02  jaffe
 * 	Transarc delta: fred-remove-sys 1.1
 * 	  Selected comments:
 * 	    To remove dependency on the AFS sys command in the test scripts.
 * 	    To remove dependency on AFS sys command.
 * 	Transarc delta: rajesh-test-error-mapping 1.1
 * 	  Selected comments:
 * 	    Tests to check DFS error mapping.
 * 	    Program to test DFS error mappings for errors EPERM, ENOENT,
 * 	    EACCESS, EEXIST, ENOTDIR, EISDIR and ESPIPE
 * 	[1992/05/20  20:40:31  jaffe]
 * 
 * $EndLog$
 */
/*
 * Copyright (C) 1992 Transarc Corporation - All rights reserved
 *
 * Program to test DFS error mappings for errors EPERM, ENOENT,
 * EACCESS, EEXIST, ENOTDIR, EISDIR and ESPIPE
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

void errex();
int debug=FALSE;

main(argc, argv)
     int argc;
     char **argv;
{
  char *progName;
  char curDFSPath[MAXPATHLEN];
  int numTmpDirs, numTmpFiles;
  int pid;
  DIR *dirHandle;
  int fd[2];
  char tDir1[MAXPATHLEN];
  char tDir2[MAXPATHLEN];
  char tDir3[MAXPATHLEN];
  char tDir4[MAXPATHLEN];
  char tDir5[MAXPATHLEN];
  char tFile1[MAXPATHLEN];
  char tFile2[MAXPATHLEN];
  char tFile3[MAXPATHLEN];
  char tFile4[MAXPATHLEN];
  char tFile5[MAXPATHLEN];
  int res;

  progName = argv[0];
  pid = getpid();
  (void) getwd(curDFSPath);  /* Returns /.. path in DFS */
  printf("Cur Dir is %s \n", curDFSPath);

  /* 1. EPERM test */
  printf("EPERM test \n");
  sprintf(tDir1, "%s.%d.tdir1",progName, pid);
  if ( mkdir(tDir1, 0777) < 0) errex("mkdir of ErrMapTestDir: ");
  res = unlink(tDir1);
  if ( res == 0 ) {
    errex("DFS ERROR: unlink of a directory did not fail.");
  }
  else {
    if (errno != EPERM) {
      errex("MAPPING ERROR: unlink of a directory did not fail with EPERM errno");
    }
  }
  
  /* 2. ENOENT test */
  printf("ENOENT test \n");
  sprintf(tDir2, "%s.%d.tdir2",progName, pid);
  sprintf(tFile2, "%s/tfile2",tDir2);
  if ( mkdir(tDir2, 0777) < 0) errex("mkdir of ErrMapTestDir: ");
  res = open(tFile2, O_RDONLY, 0555);
  if ( res == 0 ) {
    errex("DFS ERROR: open of a non-existent file did not fail.");
  }
  else {
    if (errno != ENOENT) {
      errex("MAPPING ERROR: open  of a non-existent file did not fail with ENOENT errno");
    }
  }
  
  /* 3. EACCES test */
  umask(0);
  printf("EACCES test \n");
  sprintf(tDir3, "%s.%d.tdir3",progName, pid);
  sprintf(tFile3, "%s/tfile3",tDir3);
  if ( mkdir(tDir3, S_IRUSR|S_IXUSR) < 0) errex("mkdir of ErrMapTestDir: ");
  res = open(tFile3, O_CREAT|O_WRONLY, 0555);
  if ( res == 0 ) {
    errex("DFS ERROR: creation of a file in a non-writable directory did not fail.");
  }
  else {
    if (errno != EACCES) {
      errex("MAPPING ERROR: creation of a file in a non-writable directory did not fail with EACCES errno");
    }
  }
  
  /* 4. ENOTDIR test */
  printf("ENOTDIR test \n");
  sprintf(tFile4, "%s.%d.tfile4",progName, pid);
  res = open(tFile4, O_CREAT|O_WRONLY|O_TRUNC, 0555);
  sprintf(tFile5, "%s/tfile.5", tFile4);
  dirHandle = opendir(tFile5);
  if ( dirHandle  != (DIR *) NULL) {
    ("DFS ERROR: creation of a file in a path containing a non-directory component did not fail.");
  }
  else {
    if (errno != ENOTDIR) {
      errex("MAPPING ERROR: creation of a file in a path containing a non-directory component did not fail with ENOTDIR errno");
    }
  }
  
  /* 5. EISDIR */
  printf("EISDIR test \n");
  sprintf(tDir5, "%s.%d.tdir5",progName, pid);
  if ( mkdir(tDir5, 0777) < 0) errex("mkdir of ErrMapTestDir: ");
  res = open(tDir5, O_RDWR);
  if ( res == 0 ) {
    errex("DFS ERROR: open of directory for read/write did not fail.");
  }
  else {
    if (errno != EISDIR) {
      errex("MAPPING ERROR: DFS ERROR: open of directory for read/write did not fail with EISDIR");
    }
  }

  /* 6. EISPIPE test */
  printf("EISPIPE test \n");
  if (pipe(fd) < 0 ) {
    errex("creation of pipe failed");
  }

  res = lseek(fd[1], 10, SEEK_CUR);
   if ( res == 0 ) {
    errex("DFS ERROR:  seek on a pipe did not fail.");

  }
  else {
    if (errno != ESPIPE) {
      errex("MAPPING ERROR: seek on a pipe did not fail with ESPIPE");
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
            sprintf(dumpfile,"/tmp/low.progErrMap.kernel.dump.%d",getpid());
            dumpfilep = fopen(dumpfile, "w");
            icl_DumpKernel(dumpfilep, NULL, ICL_DUMP_FORMATTED);
            fclose(dumpfilep);
        }
        exit(1);
}




