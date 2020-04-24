/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: prog9.c,v $
 * Revision 1.1.163.1  1996/10/17  18:29:36  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:54:53  damon]
 *
 * Revision 1.1.158.1  1994/02/04  20:47:11  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:25:25  devsrc]
 * 
 * Revision 1.1.156.1  1993/12/07  17:46:10  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:39:13  jaffe]
 * 
 * Revision 1.1.6.6  1993/01/11  18:33:36  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:16:04  htf]
 * 
 * Revision 1.1.6.5  1992/11/24  21:46:27  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:46:35  bolinger]
 * 
 * Revision 1.1.6.4  1992/10/28  18:13:40  jaffe
 * 	Transarc delta: jess-add-dumpkernel-in-low-tests 1.5
 * 	  Selected comments:
 * 	    Add function call (icl_DumpKernel) in the test suite. In case of running
 * 	    error of C programmes in the test suite the "icl_DumpKernel" function
 * 	    will be called in errex() to do the kernel trace. The dump file is located
 * 	    in "/tmp/prog#.kernel.dump.pid". You have to have the debug flag set to
 * 	    have the dump file.
 * 	    See above.
 * 	    Process another C file and add "low" as dump file header.
 * 	    Turn on the prog3 debug flags.
 * 	    It should not create the zero length dumpfile when there is nothing wrong.
 * 	    Fix the problem.
 * 	[1992/10/28  14:35:25  jaffe]
 * 
 * Revision 1.1.6.3  1992/09/25  17:24:55  jaffe
 * 	Transarc delta: jess-fix-low-test9-ctime 1.4
 * 	  Selected comments:
 * 	    The given lag time for the test may not be suitable for ctime which
 * 	    may exceeds the lag time because the skewed clock or delayed operation.
 * 	    Just return when ctime exceeds the lag time. It won't print out
 * 	    the warning message for ctime. The debug flag is turned on.
 * 	    Change lag time to 3 seconds.
 * 	    See above.
 * 	    Set ctime-allowance to 60 seconds.
 * 	    See above.
 * 	    Fix a minor compilling problem.
 * 	    See above.
 * 	Transarc delta: jess-low-prog9-set-debug-off 1.2
 * 	  Selected comments:
 * 	    Put WARNING message under control of DEBUG flag.
 * 	    See above.
 * 	[1992/09/24  12:48:50  jaffe]
 * 
 * Revision 1.1.6.2  1992/08/28  18:16:10  jaffe
 * 	Transarc delta: jess-low-prog9-set-debug-off 1.1
 * 	  Selected comments:
 * 	    Set the debug flag in prog9.c off.
 * 	    See above
 * 	[1992/08/28  12:30:46  jaffe]
 * 
 * Revision 1.1.3.4  1992/05/21  12:46:53  jaffe
 * 	Transarc delta: fred-merge-tests 1.1
 * 	  Selected comments:
 * 	    Created this delta to merge Phil Hirsch's final DFS test code changes with
 * 	    the test code that the OSF had.  Hopefully, this means we and the OSF
 * 	    should be looking at the same test code.
 * 	    Changes to facilitate merge of local DFS test code with the OSF's.
 * 	Transarc delta: rajesh-low-tests-dfs-update 1.3
 * 	  Selected comments:
 * 	    To fix bugs and modify the low tests for DFS.
 * 	    Bug fixes for DFS - modification for can-read, can-write, can-ex macros
 * 	    and to provide a lag time from the command line to override the default.
 * 	    Change compile flag DFS to TR_DFS for rprog2.c
 * 	    DCE security API call fixes
 * 	[1992/05/20  20:37:33  jaffe]
 * 
 * Revision 1.1.3.3  1992/01/24  04:15:40  devsrc
 * 	Merging dfs6.3
 * 	[1992/01/24  00:24:04  devsrc]
 * 
 * Revision 1.1.3.2  1992/01/23  20:31:54  zeliff
 * 	Moving files onto the branch for dfs6.3 merge
 * 	[1992/01/23  18:48:59  zeliff]
 * 	Revision 1.1.1.3  1992/01/23  22:26:43  devsrc
 * 	Fixed logs
 * 
 * $EndLog$
*/
/*****************************************************************************/
/* Copyright (C) 1990, 1991 Transarc Corporation - All rights reserved */
/*****************************************************************************/

#include <dcedfs/param.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include "common.h"
#include <dcedfs/icl.h>
#include <string.h>


#ifdef _AIX
#include <sys/statfs.h>
#endif

#ifdef SUNOS5
#include <sys/statvfs.h>
#endif

#ifdef __hpux
#include <sys/vfs.h>
#endif

#ifdef __OSF__
#include <sys/mount.h>
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
#ifndef SPARC
#ifndef SUN3
#include <utime.h>
#endif
#endif
#endif
#endif

#ifdef RT
#include <sys/file.h>
#endif

#ifdef SPARC
#include <sys/file.h>
#endif

#ifdef SUN3
#include <sys/file.h>
#endif

#define TIME_ERROR 60; /* it is used for ctime checking */

/* 
 * The following macros have been replaced for DFS 
 *  
#ifdef MIPS
#define can_read(mode)    ((uid==0 || ((mode) & 0400))? TRUE:FALSE)
#else
#define can_read(mode)    (((!afsflag && uid==0) || \
                           ((mode) & 0400))? TRUE:FALSE)
#endif

#ifdef MIPS
#define can_write(mode)   ((uid==0 || ((mode) & 0200))? TRUE:FALSE)
#else
#define can_write(mode)   (((!afsflag && uid==0) || \
                           ((mode) & 0200))? TRUE:FALSE)
#endif

#ifdef _AIX
#define can_ex(mode)      (afsflag? ((((mode) & 0500) == 0500)? TRUE:FALSE): \
                                    (((mode) & 0100)? TRUE:FALSE))
#endif
#ifdef MIPS
#define can_ex(mode)      ((uid==0 || ((mode) & 0100))? TRUE:FALSE)
#endif
#ifndef can_ex
#define can_ex(mode)      (afsflag? ((((mode) & 0500) == 0500)? TRUE:FALSE): \
                                    ((uid==0 || ((mode) & 0100))? TRUE:FALSE))
#endif
*/

#define can_read(mode)    (((!afsflag && !dfsflag && uid==0) || \
                           ((mode) & 0400))? TRUE:FALSE)

#define can_write(mode)   (((!afsflag && !dfsflag && uid==0) || \
                           ((mode) & 0200))? TRUE:FALSE)

#define can_ex(mode)      (afsflag? ((((mode) & 0500) == 0500)? TRUE:FALSE): \
			  (dfsflag? ( ((mode) & 0100) ? TRUE:FALSE): \
                                    ((uid==0 || ((mode) & 0100))? TRUE:FALSE)))

int afsflag,dfsflag,debug=TRUE,uid, lag_time;

void check_file(),errex(), errex0();

unsigned int timeChecksCntr = 0;
unsigned int createTimeLagCntr = 0;
unsigned int changeTimeLagCntr = 0;
unsigned int accessTimeLagCntr = 0;

extern int errno;

/*****************************************************************************/

void DumpLagCounters()
{
    printf("Lag Time Data:\n\n");
    printf("Lag Time: %d seconds\n", lag_time);
    printf("Total Time Checks: %d\n", timeChecksCntr);
    printf("Create Times Exceeding Lag: %d\n", createTimeLagCntr);
    printf("Change Times Exceeding Lag: %d\n", changeTimeLagCntr);
    printf("Access Times Exceeding Lag: %d\n", accessTimeLagCntr);
}


main(argc,argv) int argc; char *argv[];
{
char dn[21],fn[21],z[256],z1[81];
int fd,i,len,mode1,mode2,mode3,mode,nr_times,pid,um;
long l;
time_t atm,mtm,ctm,d_atm,d_mtm,d_ctm;
struct stat s;

#ifdef DEC
struct fs_data fs;
#else
#ifdef SUNOS5
struct statvfs fs;
#else
struct statfs fs;
#endif
#endif

struct timeval tv[2];
#ifndef NEXT
#ifndef RT
#ifndef SPARC
#ifndef SUN3
struct utimbuf ut;
#endif
#endif
#endif
#endif

setbuf(stdout,(char *)NULL);

if(argc < 3) {nr_times = 1; lag_time = 3;}
else {nr_times = atoi(argv[1]); lag_time = atoi(argv[2]);}

pid = getpid();
uid = getuid();
if((um=umask(0)) < 0) errex("umask(1)");

printf("Prog9 pid %d, nr_times %d; User ID is%s root.\n",
   pid,nr_times,(uid? " not":""));

(void)getwd(z);
afsflag = (strncmp(z,"/afs/",5) == 0);

dfsflag = (strncmp(z,"/.../",5) == 0);

printf("This is%s an AFS filesystem.\n",afsflag? "":" not");
if(afsflag) printf(">>> [acm]times fudged for AFS. <<<\n");
printf("\n");

printf("This is%s a DFS filesystem.\n",dfsflag? "":" not");

for(i=1; i<=nr_times; ++i)
   {
   printf("\nProg9 pid %d iteration %d.\n",pid,i);



   printf("\nProg9 pid %d creating test file.\n",pid);
   strcpy(dn,".");
   if(stat(dn,&s)) errex("stat(1)");
   d_atm = s.st_atime; d_ctm = s.st_ctime; d_mtm = s.st_mtime;
   check_file(dn,d_atm,d_mtm,d_ctm,-1);
   sleep(3);

   strcpy(fn,"tfile.9"); mode=0666; (void)unlink(fn);
   atm = mtm = ctm = time((time_t *)0);
   d_mtm = d_ctm = time((time_t *)0);
   if((fd=open(fn,O_RDWR|O_CREAT,mode)) < 0) errex("open(1)");
   check_file(fn,atm,mtm,ctm,S_IFREG|mode);
   check_file(dn,d_atm,d_mtm,d_ctm,-1);
   sleep(3);

   printf("Prog9 pid %d testing chmod with file open.\n",pid);
   for(mode1=0; mode1<=7; ++mode1) /* mode octet 1 (msb) */
   for(mode2=0; mode2<=7; ++mode2) /* mode octet 2 */
   for(mode3=0; mode3<=7; ++mode3) /* mode octet 3 (lsb) */
      {
      mode = mode1*64 + mode2*8 + mode3;
      ctm = time((time_t *)0);
      if(chmod(fn,mode)) errex("chmod(1)");
      check_file(fn,atm,mtm,ctm,S_IFREG|mode);
      check_file(dn,d_atm,d_mtm,d_ctm,-1);
      }
   sleep(3);

   printf("Prog9 pid %d testing fchmod with file open.\n",pid);
   for(mode1=0; mode1<=7; ++mode1) /* mode octet 1 (msb) */
   for(mode2=0; mode2<=7; ++mode2) /* mode octet 2 */
   for(mode3=0; mode3<=7; ++mode3) /* mode octet 3 (lsb) */
      {
      mode = mode1*64 + mode2*8 + mode3;
      ctm = time((time_t *)0);
      if(fchmod(fd,mode)) errex("fchmod(1)");
      check_file(fn,atm,mtm,ctm,S_IFREG|mode);
      check_file(dn,d_atm,d_mtm,d_ctm,-1);
      }
   sleep(3);

#ifndef __hpux
   printf("Prog9 pid %d testing utimes with file open.\n",pid);
#ifndef NEXT
#ifndef RT
   atm = mtm = ctm = time((time_t *)0);
   if(utimes(fn,(struct timeval *)NULL)) errex("utimes(1)");
   check_file(fn,atm,mtm,ctm,S_IFREG|mode);
   check_file(dn,d_atm,d_mtm,d_ctm,-1);
#endif
#endif
   for(l=0; l<=1000000000L; l+=1000000L)
      {
      atm = tv[0].tv_sec = l; tv[0].tv_usec = 0;
      mtm = tv[1].tv_sec = l*2+1000; tv[1].tv_usec = 0;
      ctm = time((time_t *)0);
      if(utimes(fn,tv)) errex("utimes(2)");
      check_file(fn,atm,mtm,ctm,S_IFREG|mode);
      check_file(dn,d_atm,d_mtm,d_ctm,-1);
      }
#ifndef NEXT
#ifndef RT
   atm = mtm = ctm = time((time_t *)0);
   if(utimes(fn,(struct timeval *)0)) errex("utimes(3)");
   check_file(fn,atm,mtm,ctm,S_IFREG|mode);
   check_file(dn,d_atm,d_mtm,d_ctm,-1);
#endif
#endif
   sleep(3);
#endif

#ifndef NEXT
#ifndef RT
#ifndef SPARC
#ifndef SUN3
   printf("Prog9 pid %d testing utime with file open.\n",pid);
   for(l=0; l<=1000000000L; l+=1000000L)
      {
      atm = ut.actime = l;
      mtm = ut.modtime = l*2+1000;
      ctm = time((time_t *)0);
      if(utime(fn,&ut)) errex("utime(1)");
      check_file(fn,atm,mtm,ctm,S_IFREG|mode);
      check_file(dn,d_atm,d_mtm,d_ctm,-1);
      }
   sleep(3);
#endif
#endif
#endif
#endif

   if(afsflag) atm = ctm = mtm = time((time_t *)0);
   if(close(fd)) errex("close(1)");
   sleep(3);

   printf("Prog9 pid %d testing chmod with file closed.\n",pid);
   for(mode1=0; mode1<=7; ++mode1) /* mode octet 1 (msb) */
   for(mode2=0; mode2<=7; ++mode2) /* mode octet 2 */
   for(mode3=0; mode3<=7; ++mode3) /* mode octet 3 (lsb) */
      {
      mode = mode1*64 + mode2*8 + mode3;
      ctm = time((time_t *)0);
      if(chmod(fn,mode)) errex("chmod(2)");
      check_file(fn,atm,mtm,ctm,S_IFREG|mode);
      check_file(dn,d_atm,d_mtm,d_ctm,-1);
      }
   sleep(3);

#ifndef __hpux
   printf("Prog9 pid %d testing utimes with file closed.\n",pid);
#ifndef NEXT
#ifndef RT
   atm = mtm = ctm = time((time_t *)0);
   if(utimes(fn,(struct timeval *)0)) errex("utimes(4)");
   check_file(fn,atm,mtm,ctm,S_IFREG|mode);
   check_file(dn,d_atm,d_mtm,d_ctm,-1);
#endif
#endif
   for(l=0; l<=1000000000L; l+=1000000L)
      {
      atm = tv[0].tv_sec = l; tv[0].tv_usec = 0;
      mtm = tv[1].tv_sec = l*2+1000; tv[1].tv_usec = 0;
      ctm = time((time_t *)0);
      if(utimes(fn,tv)) errex("utimes(5)");
      check_file(fn,atm,mtm,ctm,S_IFREG|mode);
      check_file(dn,d_atm,d_mtm,d_ctm,-1);
      }
#ifndef NEXT
#ifndef RT
   atm = mtm = ctm = time((time_t *)0);
   if(utimes(fn,(struct timeval *)0)) errex("utimes(6)");
   check_file(fn,atm,mtm,ctm,S_IFREG|mode);
   check_file(dn,d_atm,d_mtm,d_ctm,-1);
#endif
#endif
   sleep(3);
#endif

#ifndef NEXT
#ifndef RT
#ifndef SPARC
#ifndef SUN3
   printf("Prog9 pid %d testing utime with file closed.\n",pid);
   for(l=0; l<=1000000000L; l+=1000000L)
      {
      atm = ut.actime = l;
      mtm = ut.modtime = l*2+1000;
      ctm = time((time_t *)0);
      if(utime(fn,&ut)) errex("utime(2)");
      check_file(fn,atm,mtm,ctm,S_IFREG|mode);
      check_file(dn,d_atm,d_mtm,d_ctm,-1);
      }
   sleep(3);
#endif
#endif
#endif
#endif

   printf("Prog9 pid %d unlinking test file.\n",pid);
   d_mtm = d_ctm = time((time_t *)0);
   if(unlink(fn)) errex("unlink(1)");
   check_file(dn,d_atm,d_mtm,d_ctm,-1);
   sleep(3);

   printf("\nProg9 pid %d re-creating test file.\n",pid);
   atm = mtm = ctm = time((time_t *)0);
   d_mtm = d_ctm = time((time_t *)0);
   mode=0644; if((fd=open(fn,O_RDWR|O_CREAT,mode)) < 0) errex("open(2)");
   check_file(fn,atm,mtm,ctm,S_IFREG|mode);
   check_file(dn,d_atm,d_mtm,d_ctm,-1);
   sleep(3);

   printf("Prog9 pid %d writing test file.\n",pid);
   sprintf(z,"This is %s.",fn); len=strlen(z);
#ifdef NEXT /* This really looks bogus!! */
   atm = time((time_t *)0);
#else
   mtm = ctm = time((time_t *)0);
#endif
   if(write(fd,z,(RWA3)(len+1)) != len+1) errex("write(1)");
   if(stat(fn,&s)) errex("stat(2)");
   if(s.st_size != strlen(z)+1) errex0("stat size(1)");
   check_file(fn,atm,mtm,ctm,S_IFREG|mode);
   check_file(dn,d_atm,d_mtm,d_ctm,-1);
   sleep(3);

   sprintf(z,"THIS IS %s.",fn); len=strlen(z);
   if(lseek(fd,(off_t)0,0) != 0) errex("lseek(1)");
#ifndef NEXT /* More bogosity... */
   mtm = ctm = time((time_t *)0);
#endif
   if(write(fd,z,(RWA3)(len+1)) != len+1) errex("write(2)");
   if(stat(fn,&s)) errex("stat(3)");
   if(s.st_size != strlen(z)+1) errex0("stat size(2)");
   check_file(fn,atm,mtm,ctm,S_IFREG|mode);
   check_file(dn,d_atm,d_mtm,d_ctm,-1);
   sleep(3);

   if(lseek(fd,(off_t)0,0) != 0) errex("lseek(2)");
#ifndef NEXT /* And still more bogosity... */
   atm = time((time_t *)0);
#endif
   if(read(fd,z1,(RWA3)(strlen(z)+1)) < 0) errex("read(1)");
   if(!EQ(z,z1)) errex0("read compare(1)");
   check_file(fn,atm,mtm,ctm,S_IFREG|mode);
   check_file(dn,d_atm,d_mtm,d_ctm,-1);
   sleep(3);

   if(close(fd)) errex("close(2)");

   printf("Prog9 pid %d reading test file.\n",pid);
   if((fd=open(fn,O_RDWR|O_CREAT,mode)) < 0) errex("open(3)");
#ifdef __hpux
   ctm = time((time_t *)0);
#endif
#ifdef NEXT /* Hard to believe, and bogus as can be... */
   ctm = mtm = time((time_t *)0);
#endif
   check_file(fn,atm,mtm,ctm,S_IFREG|mode);
   check_file(dn,d_atm,d_mtm,d_ctm,-1);
   sleep(3);

#ifndef NEXT /* bogus */
   atm = time((time_t *)0);
#endif
   if(read(fd,z1,(RWA3)(strlen(z)+1)) < 0) errex("read(2)");
   if(!EQ(z,z1)) errex0("read compare(2)");
   check_file(fn,atm,mtm,ctm,S_IFREG|mode);
   check_file(dn,d_atm,d_mtm,d_ctm,-1);
   sleep(3);

   if(close(fd)) errex("close(3)");
   check_file(fn,atm,mtm,ctm,S_IFREG|mode);
   check_file(dn,d_atm,d_mtm,d_ctm,-1);

   printf("Prog9 pid %d truncating test file.\n",pid);
   if((fd=open(fn,O_RDWR|O_CREAT,mode)) < 0) errex("open(4)");
   check_file(fn,atm,mtm,ctm,S_IFREG|mode);
   check_file(dn,d_atm,d_mtm,d_ctm,-1);
   sleep(3);

   mtm = ctm = time((time_t *)0);
   if(ftruncate(fd,(off_t)0)) errex("ftruncate(1)");
   if(stat(fn,&s)) errex("stat(4)");
   if(s.st_size != 0) errex0("stat size(3)");
   check_file(fn,atm,mtm,ctm,S_IFREG|mode);
   check_file(dn,d_atm,d_mtm,d_ctm,-1);

   printf("Prog9 pid %d renaming and unlinking test file.\n",pid);
   d_mtm = d_ctm = time((time_t *)0);
   if(rename(fn,"fn")) errex("rename(1)");
   check_file(dn,d_atm,d_mtm,d_ctm,-1);

   if(close(fd)) errex("close(4)");
   d_mtm = d_ctm = time((time_t *)0);
   if(unlink("fn")) errex("unlink(2)");
   check_file(dn,d_atm,d_mtm,d_ctm,-1);
   sleep(3);



   printf("\nProg9 pid %d creating test directory.\n",pid);
   strcpy(dn,"tdir.9"); mode=0777; (void)unlink(dn); (void)rmdir(dn);
   d_atm = d_mtm = d_ctm = time((time_t *)0);
   if(mkdir(dn,mode)) errex("mkdir(1)");
   if((fd=open(dn,O_RDONLY)) < 0) errex("open(5)");
   check_file(dn,d_atm,d_mtm,d_ctm,S_IFDIR|mode);
   sleep(3);

   printf("Prog9 pid %d testing chmod with directory open.\n",pid);
   for(mode1=0; mode1<=7; ++mode1) /* mode octet 1 (msb) */
   for(mode2=0; mode2<=7; ++mode2) /* mode octet 2 */
   for(mode3=0; mode3<=7; ++mode3) /* mode octet 3 (lsb) */
      {
      mode = mode1*64 + mode2*8 + mode3;
      d_ctm = time((time_t *)0);
      if(chmod(dn,mode)) errex("chmod(3)");
      check_file(dn,d_atm,d_mtm,d_ctm,S_IFDIR|mode);
      }
   sleep(3);

   printf("Prog9 pid %d testing fchmod with directory open.\n",pid);
   for(mode1=0; mode1<=7; ++mode1) /* mode octet 1 (msb) */
   for(mode2=0; mode2<=7; ++mode2) /* mode octet 2 */
   for(mode3=0; mode3<=7; ++mode3) /* mode octet 3 (lsb) */
      {
      mode = mode1*64 + mode2*8 + mode3;
      d_ctm = time((time_t *)0);
      if(fchmod(fd,mode)) errex("fchmod(2)");
      check_file(dn,d_atm,d_mtm,d_ctm,S_IFDIR|mode);
      }
   sleep(3);

#ifndef __hpux
   printf("Prog9 pid %d testing utimes with directory open.\n",pid);
#ifndef NEXT
#ifndef RT
   d_atm = d_mtm = d_ctm = time((time_t *)0);
   if(utimes(dn,(struct timeval *)NULL)) errex("utimes(7)");
   check_file(dn,d_atm,d_mtm,d_ctm,S_IFDIR|mode);
#endif
#endif
   for(l=0; l<=1000000000L; l+=1000000L)
      {
      d_atm = tv[0].tv_sec = l; tv[0].tv_usec = 0;
      d_mtm = tv[1].tv_sec = l*2+1000; tv[1].tv_usec = 0;
      d_ctm = time((time_t *)0);
      if(utimes(dn,tv)) errex("utimes(8)");
      check_file(dn,d_atm,d_mtm,d_ctm,S_IFDIR|mode);
      }
#ifndef NEXT
#ifndef RT
   d_atm = d_mtm = d_ctm = time((time_t *)0);
   if(utimes(dn,(struct timeval *)0)) errex("utimes(9)");
   check_file(dn,d_atm,d_mtm,d_ctm,S_IFDIR|mode);
#endif
#endif
   sleep(3);
#endif

#ifndef NEXT
#ifndef RT
#ifndef SPARC
#ifndef SUN3
   printf("Prog9 pid %d testing utime with directory open.\n",pid);
   for(l=0; l<=1000000000L; l+=1000000L)
      {
      d_atm = ut.actime = l;
      d_mtm = ut.modtime = l*2+1000;
      d_ctm = time((time_t *)0);
      if(utime(dn,&ut)) errex("utime(3)");
      check_file(dn,d_atm,d_mtm,d_ctm,S_IFDIR|mode);
      }
   sleep(3);
#endif
#endif
#endif
#endif

   if(close(fd)) errex("close(5)");
   sleep(3);

   printf("Prog9 pid %d testing chmod with directory closed.\n",pid);
   for(mode1=0; mode1<=7; ++mode1) /* mode octet 1 (msb) */
   for(mode2=0; mode2<=7; ++mode2) /* mode octet 2 */
   for(mode3=0; mode3<=7; ++mode3) /* mode octet 3 (lsb) */
      {
      mode = mode1*64 + mode2*8 + mode3;
      d_ctm = time((time_t *)0);
      if(chmod(dn,mode)) errex("chmod(4)");
      check_file(dn,d_atm,d_mtm,d_ctm,S_IFDIR|mode);
      }
   sleep(3);

#ifndef __hpux
   printf("Prog9 pid %d testing utimes with directory closed.\n",pid);
#ifndef NEXT
#ifndef RT
   d_atm = d_mtm = d_ctm = time((time_t *)0);
   if(utimes(dn,(struct timeval *)0)) errex("utimes(10)");
   check_file(dn,d_atm,d_mtm,d_ctm,S_IFDIR|mode);
#endif
#endif
   for(l=0; l<=1000000000L; l+=1000000L)
      {
      d_atm = tv[0].tv_sec = l; tv[0].tv_usec = 0;
      d_mtm = tv[1].tv_sec = l*2+1000; tv[1].tv_usec = 0;
      d_ctm = time((time_t *)0);
      if(utimes(dn,tv)) errex("utimes(11)");
      check_file(dn,d_atm,d_mtm,d_ctm,S_IFDIR|mode);
      }
#ifndef NEXT
#ifndef RT
   d_atm = d_mtm = d_ctm = time((time_t *)0);
   if(utimes(dn,(struct timeval *)0)) errex("utimes(12)");
   check_file(dn,d_atm,d_mtm,d_ctm,S_IFDIR|mode);
#endif
#endif
   sleep(3);
#endif

#ifndef NEXT
#ifndef RT
#ifndef SPARC
#ifndef SUN3
   printf("Prog9 pid %d testing utime with directory closed.\n",pid);
   for(l=0; l<=1000000000L; l+=1000000L)
      {
      d_atm = ut.actime = l;
      d_mtm = ut.modtime = l*2+1000;
      d_ctm = time((time_t *)0);
      if(utime(dn,&ut)) errex("utime(4)");
      check_file(dn,d_atm,d_mtm,d_ctm,S_IFDIR|mode);
      }
   sleep(3);
#endif
#endif
#endif
#endif

   if(rmdir(dn)) errex("rmdir(1)");
   }

if(umask(um) != 0) errex("umask(2)");
DumpLagCounters();
printf("\nProg9 pid %d exiting.\n",pid);
exit(0);
}


/*****************************************************************************/

int check_time(which_t, correct_t, actual_t, lag_t)
  char *which_t;
  time_t correct_t, actual_t, lag_t;
{
    time_t dt, terr;

    terr=TIME_ERROR;
    timeChecksCntr++;
    dt = actual_t - correct_t;
    if (dt < 0) 
	dt = -dt;
    if (dt > lag_t) 
    {
        if ( (*which_t == 'c') && (dt < terr ) ) return; 

	if (debug)
	{
	    printf("WARNING: %s exceeds allowed lag time (%ld) \n", 
		   which_t, (long)lag_t);
	    printf("               should be %ld, actually is %ld \n", 
		   (long)correct_t, (long)actual_t);
	}

	/*
	 *  The following is NOT thread-safe, since no mutex's are locked 
	 *  prior to changing the counters.
	 */
	switch (*which_t) {
	  case 'c':
	    createTimeLagCntr++;
	    break;
	  case 'm':
	    changeTimeLagCntr++;
	    break;
	  case 'a':
	    accessTimeLagCntr++;
	    break;
	  default:
	    fprintf(stderr, "Invalid time check type, %s\n", which_t);
	    break;
	}
    }
    return;
}    

/*****************************************************************************/

void check_file(fn,atime,mtime,ctime,mode)
char *fn;
time_t atime,mtime,ctime;
int mode;
{
char *tt;
int i,mask,rf, dt;
time_t fluff,t1,t2;
struct stat s;

if(afsflag) atime = ctime = mtime; /* AFS 3 keeps only one time field */

if(stat(fn,&s)) errex("stat(5)");

fluff = lag_time;

for(i=1; i<=3; ++i)
   {
   switch(i)
      {
      case 1: tt = "atime"; t1 = atime; t2 = s.st_atime; break;
      case 2: tt = "mtime"; t1 = mtime; t2 = s.st_mtime; break;
      case 3: tt = "ctime"; t1 = ctime; t2 = s.st_ctime; break;
      default: errex0("switch(1)");
      }

/*   
 * If specified lag time is < 1 second, then the lag time should not be squared
 *
      if((t1-t2)*(t1-t2) > fluff*fluff)  
      {
      if(debug)
         {
         printf("acm should be %ld, %ld, %ld.\n",
            (long)atime,(long)ctime,(long)mtime);
         printf("          are %ld, %ld, %ld.\n",
            (long)s.st_atime,(long)s.st_ctime,(long)s.st_mtime);
         }
      errex0(tt);
      }
 */
      check_time(tt, t1, t2, fluff);

   }

if(mode == -1) return;

mask = ~0;

#ifdef _AIX
/* Account for the JFS log bit. */
if(!afsflag && (mode & S_IFDIR)) mask = ~0x10000;
#endif

#ifdef SUN3
/* Some Suns set the setgid bit on local directories. */
if(!afsflag && (mode & S_IFDIR)) mask = ~S_ISGID;
#endif

#ifdef SPARC
/* Some Suns set the setgid bit on local directories. */
if(!afsflag && (mode & S_IFDIR)) mask = ~S_ISGID;
#endif

if(mode != (s.st_mode & mask)) errex0("mode test(1)");

if(!afsflag || (mode & S_IFREG))
   {
   if((rf=access(fn,R_OK)) && can_read(mode)) errex("read-access test(1)");
   else if(!can_read(mode) && (rf==0 || errno!=EACCES))
      errex("no-read-access test(1)");

   if((rf=access(fn,W_OK)) && can_write(mode)) errex("write-access test(1)");
   else if(!can_write(mode) && (rf==0 || errno!=EACCES))
      errex("no-write-access test(1)");

   if((rf=access(fn,X_OK)) && can_ex(mode)) errex("exec-access test(1)");
   else if(!can_ex(mode) && (rf==0 || errno!=EACCES))
      errex("no-exec-access test(1)");
   }
else /* AFS directory - access() always succeeds */
   {
   if(rf = access(fn,R_OK)) errex("read-access test(2)");
   if(rf = access(fn,W_OK)) errex("write-access test(2)");
   if(rf = access(fn,X_OK)) errex("exec-access test(2)");
   }
}


/*****************************************************************************/

void errex(s) char *s;
{
        FILE *dumpfilep;
        char dumpfile[256];

        printf("\n%s failed; errno = %d\n",s,errno);
        if (debug) {
            sprintf(dumpfile,"/tmp/low.prog9.kernel.dump.%d",getpid());
            dumpfilep = fopen(dumpfile, "w");
            icl_DumpKernel(dumpfilep, NULL, ICL_DUMP_FORMATTED);
            fclose(dumpfilep);
        }
        exit(1);
}

void errex0(s) char *s;
{
        FILE *dumpfilep;
        char dumpfile[256];

        printf("\n%s failed\n",s);
        if (debug) {
            sprintf(dumpfile,"/tmp/low.prog9.kernel.dump.%d",getpid());
            dumpfilep = fopen(dumpfile, "w");
            icl_DumpKernel(dumpfilep, NULL, ICL_DUMP_FORMATTED);
            fclose(dumpfilep);
        }
        exit(1);
}


/*****************************************************************************/
