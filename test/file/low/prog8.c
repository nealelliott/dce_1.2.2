/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: prog8.c,v $
 * Revision 1.1.152.1  1996/10/17  18:29:33  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:54:52  damon]
 *
 * Revision 1.1.147.1  1994/02/04  20:47:08  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:25:23  devsrc]
 * 
 * Revision 1.1.145.1  1993/12/07  17:46:08  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:38:52  jaffe]
 * 
 * Revision 1.1.13.2  1993/07/19  19:44:38  zeliff
 * 	HP port of DFS
 * 	[1993/07/19  18:33:29  zeliff]
 * 
 * Revision 1.1.11.3  1993/07/16  22:14:04  kissel
 * 	Put HP RP work on the 1.0.3i branch.
 * 	[1993/06/21  15:11:34  kissel]
 * 
 * Revision 1.1.9.2  1993/06/04  18:35:54  kissel
 * 	Initial HPUX RP version.
 * 	[1993/06/03  22:46:45  kissel]
 * 
 * 	use __hpux not HP ifdefs
 * 	[1993/04/28  18:09:49  maunsell_c]
 * 
 * Revision 1.1.5.7  1993/03/09  21:23:26  jaffe
 * 	Transarc delta: jess-ot5811-fix-low-test8 1.1
 * 	  Selected comments:
 * 	    This delta fixes the errex problem in prog8.c. "errex()" expects
 * 	    an error message but there was no message passed.
 * 	    Fix it.
 * 	Transarc delta: jess-ot5921-fix-low-prog8 1.1
 * 	  Selected comments:
 * 	    This delta fixes mtime and ctime problem caused by rigorous checking
 * 	    condition in st_compare function of prog8.c.
 * 	    Fix the problem by tolerating a slop between two stat calls for a possible
 * 	    storeback between the two calls.
 * 	Transarc delta: jess-ot7194-fs-low-kerneldump 1.1
 * 	  Selected comments:
 * 	    Change the icl_KernelDump parameter according to the changes
 * 	    from icl utilities.
 * 	    See above.
 * 	[1993/03/09  20:56:17  jaffe]
 * 
 * Revision 1.1.5.6  1993/01/11  18:33:32  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:15:57  htf]
 * 
 * Revision 1.1.5.5  1992/11/24  21:46:19  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:46:27  bolinger]
 * 
 * Revision 1.1.5.4  1992/10/28  18:13:33  jaffe
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
 * 	[1992/10/28  14:34:51  jaffe]
 * 
 * Revision 1.1.5.3  1992/09/25  17:24:49  jaffe
 * 	Transarc delta: jess-fix-low-test8-ctime 1.2
 * 	  Selected comments:
 * 	    The ctimes are definitly different between the client and the server
 * 	    after the client stores the file state back to the server. This delta
 * 	    is trying to fix the warnings for the ctime in the test.
 * 	    Fix the problem.
 * 	    Set ctime-allowance to 60 seconds.
 * 	    See above.
 * 	[1992/09/24  12:48:18  jaffe]
 * 
 * Revision 1.1.5.2  1992/08/28  18:15:58  jaffe
 * 	Transarc delta: jess-get-st_compare-info-temp 1.1
 * 	  Selected comments:
 * 	    This delta is for debug purpose and may later be defuncted.
 * 	    Change the st_compare so that the message will be printed
 * 	    on stdout.
 * 	[1992/08/28  12:30:21  jaffe]
 * 
 * Revision 1.1.4.2  1993/02/05  17:16:57  maunsell_c
 * 	01/27/93	maunsell_c	add ifdef for __hpux for getpid()
 * 
 * Revision 1.1.3.4  1992/05/21  12:46:47  jaffe
 * 	Transarc delta: fred-merge-tests 1.1
 * 	  Selected comments:
 * 	    Created this delta to merge Phil Hirsch's final DFS test code changes with
 * 	    the test code that the OSF had.  Hopefully, this means we and the OSF
 * 	    should be looking at the same test code.
 * 	    Changes to facilitate merge of local DFS test code with the OSF's.
 * 	Transarc delta: rajesh-low-tests-dfs-update 1.3
 * 	  Selected comments:
 * 	    To fix bugs and modify the low tests for DFS.
 * 	    Bug fixes for DFS - modification for can-read, can-write, can-ex macros.
 * 	    Change compile flag DFS to TR_DFS for rprog2.c
 * 	    DCE security API call fixes
 * 	[1992/05/20  20:36:48  jaffe]
 * 
 * Revision 1.1.3.3  1992/01/24  04:15:38  devsrc
 * 	Merging dfs6.3
 * 	[1992/01/24  00:24:01  devsrc]
 * 
 * Revision 1.1.3.2  1992/01/23  20:31:47  zeliff
 * 	Moving files onto the branch for dfs6.3 merge
 * 	[1992/01/23  18:48:53  zeliff]
 * 	Revision 1.1.1.3  1992/01/23  22:26:39  devsrc
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
#include <ctype.h>
#include "common.h"
#include <dcedfs/icl.h>
#include <string.h>


#ifdef __OSF__
#include <sys/mount.h>
#endif

#ifdef _AIX
#include <sys/statfs.h>
#endif

#ifdef SUNOS5
#include <sys/statvfs.h>
#endif

#ifdef __hpux
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

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#ifdef __hpux
int open_wc_bug=TRUE;
#endif

#ifdef __OSF__
int open_wc_bug=FALSE;
#endif

#ifdef MIPS
#define DEC
int open_wc_bug=FALSE;
#endif

#ifdef NEXT
int open_wc_bug=FALSE;
#endif

#ifdef _AIX
int open_wc_bug=FALSE;
#endif

#ifdef RT
#include <sys/file.h>
int open_wc_bug=TRUE;
#endif

#if defined(SPARC) || defined(sparc)
#include <sys/file.h>
int open_wc_bug=TRUE;
#endif

#ifdef SUN3
#include <sys/file.h>
int open_wc_bug=TRUE;
#endif

#ifdef VAX
#define DEC
int open_wc_bug=FALSE;
#endif

#define max(a,b)  (((int) (a) > (int)(b)) ? (int)(a) : (int)(b))

/* 
 * The following macros have been replaced for DFS 
 *  
#ifdef _AIX
#define can_read(mode)    (((!afsflag && uid==0) || \
                           ((mode) & 0400))? TRUE:FALSE)
#else
#define can_read(mode)    ((uid==0 || ((mode) & 0400))? TRUE:FALSE)
#endif

#ifdef _AIX
#define can_write(mode)   (((!afsflag && uid==0) || \
                           ((mode) & 0200))? TRUE:FALSE)
#else
#define can_write(mode)   ((uid==0 || ((mode) & 0200))? TRUE:FALSE)
#endif

#ifdef MIPS
#define can_ex(mode)      ((uid==0 || ((mode) & 0100))? TRUE:FALSE)
#endif
#ifdef _AIX
#define can_ex(mode)      (afsflag? ((((mode) & 0500) == 0500)? TRUE:FALSE): \
                                    ((uid==0 || ((mode) & 0100))? TRUE:FALSE))
#endif
#ifndef can_ex
#define can_ex(mode)      (afsflag? ((uid==0 || (((mode) & 0500) == 0500))? \
                                      TRUE:FALSE): \
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


char em[256];
int afsflag,dfsflag,efsflag,file_created,debug=TRUE,
    multi_proc=FALSE,save_errno,uid;
int o_rw[] = {O_RDONLY, O_WRONLY, O_RDWR, 0};

/* 
* The TIME_ERROR is defined as a allowed error of time skewed in the network
* and/or the delayed time between the server and client.
*/

#define TIME_ERROR 60   /* one minute */

int st_compare();
void check_file(),errex(), errex0(), setup_file();

#if defined(OSF) || defined(sun) || defined(__hpux)
extern int fsync();
#else
extern int fsync(),getpid();
#endif /* OSF */
extern int errno;

/*****************************************************************************/

main(argc,argv) int argc; char *argv[];
{
char fn[21],nfn[21],z[256],z1[81];
int cnt,fd,fpid,i,l,longmode,mode,nr_times,nrw,oflag,pid,rf,st,um;
int f_fe,f_rw,f_cr,f_ex,f_tr,f_ap,mode1,mode2,mode3,m2s,m2e,m3s,m3e;
off_t ls;
#ifdef DEC
struct fs_data fs;
#else
#ifdef SUNOS5
struct statvfs fs;
#else
struct statfs fs;
#endif
#endif

setbuf(stdout,(char *)NULL);

if(argc < 3) {nr_times = 1; longmode = FALSE;}
else
   {
   nr_times = atoi(argv[1]);
   longmode = (EQ(argv[2],"-long"))? TRUE:FALSE;
   }
nrw = sizeof(o_rw)/sizeof(int);
pid = getpid();
uid = getuid();
um = umask(0);

printf("Prog8 pid %d, nr_times %d, %s mode%s.\n",
   pid,nr_times,longmode? "long":"short",(uid? "":"; USER ID IS ROOT"));

(void)getwd(z);
afsflag = (strncmp(z,"/afs/",5) == 0);
dfsflag = (strncmp(z,"/.../",5) == 0);

#ifdef SUNOS5
if ( statvfs(".", &fs) < 0 )
#else
if ( statfs(".", &fs) < 0 )
#endif
{
    strcpy(em,"statfs() failed"); errex(em);
}

#ifdef sun
else efsflag = FALSE;
#else /* not sun */
#ifndef DEC
#ifndef __hpux
else efsflag = (fs.f_fsid.val[1]==10);
#else /* __hpux */
else efsflag = (fs.f_fsid[1]==10);
#endif /*not-HP*/
#else /* DEC */
else efsflag = FALSE; /* don't yet know how to do this for Vaxen or Pmaxen */
#endif /* not-DEC */
#endif /* not sun */

printf("This is%s an AFS filesystem.\n",afsflag? "":" not");
printf("This is%s a DFS filesystem.\n",dfsflag? "":" not");
printf("This is%s an Episode filesystem.\n",efsflag? "":" not");
if(afsflag)
   {
   printf(">>> RWX recognition fudged for AFS. <<<\n");
   if(open_wc_bug) printf(">>> open_wc_bug fudged for AFS. <<<\n");
   }
printf("\n");

for(i=1; i<=nr_times || nr_times==0; ++i)
   {
   printf("Prog8 pid %d starting iteration %d.\n",pid,i);

   printf("\nProg8 pid %d iter %i starting open test.\n",pid,i);
   cnt = 0;
   for(f_fe=0; f_fe<=2; ++f_fe) /* file nonexistent, file empty, file w/ data */
      {
      if(multi_proc)
         {
         if((fpid=fork()) == -1)
            {sprintf(em,"- ERROR 1: fork(%d) failed",f_fe); errex(em);}
         }
      else fpid = 0;
      if(fpid == 0) /* child */
         {
         if(multi_proc)
           {pid = getpid(); printf("Child %d (pid %d) started.\n",f_fe,pid);}

         for(f_ex=0; f_ex<=1; ++f_ex) /* O_EXCL or not */
         for(f_tr=0; f_tr<=1; ++f_tr) /* O_TRUNC or not */
         for(f_cr=0; f_cr<=1; ++f_cr) /* O_CREAT or not */
         for(f_ap=0; f_ap<=1; ++f_ap) /* O_APPEND or not */
         for(f_rw=0; f_rw<nrw; ++f_rw) /* O_RDONLY, O_WRONLY, O_RDWR, none */
         for(mode1=0; mode1<=7; ++mode1) /* mode octet 1 (msb) */
            {
            if(longmode) {m2s = m3s = 1; m2e = m3e = 7;}
            else m2s = m3s = m2e = m3e = mode1;
            for(mode2=m2s; mode2<=m2e; ++mode2) /* mode octet 2 */
            for(mode3=m3s; mode3<=m3e; ++mode3) /* mode octet 3 (lsb) */
               {
               file_created = FALSE;
               oflag = o_rw[f_rw];
               if(f_cr) oflag |= O_CREAT;
               if(f_ex) oflag |= O_EXCL;
               if(f_tr) oflag |= O_TRUNC;
               if(f_ap) oflag |= O_APPEND;
               mode = mode1*64 + mode2*8 + mode3;
	       if (!mode) continue;
               sprintf(fn,"f%d.%o.%03o",f_fe,oflag,mode);
               setup_file(fn,f_fe,mode); ++cnt;
               save_errno = -1; fd = open(fn,oflag,mode); save_errno = errno;
               if(fd >= 0) file_created = TRUE;
               check_file(fn,fd,f_fe,oflag,mode,FALSE);
               if(file_created)
                  {
                  sprintf(nfn,"F%d.%o.%03o",f_fe,oflag,mode);
                  if(rename(fn,nfn))
                     {sprintf(em,"- ERROR 2: rename(%s) failed",fn); errex(em);}
                  check_file(nfn,fd,f_fe,oflag,mode,TRUE);
                  }
               if(fd>=0 && close(fd))
                  {sprintf(em,"- ERROR 3: close(1;%s) failed",fn); errex(em);}
               if(file_created && unlink(file_created? nfn:fn))
                  {
                  sprintf(em,"- ERROR 4: unlink(1;%s) failed",
                     file_created? nfn:fn);
                  errex(em);
                  }
               }
            }

         if(multi_proc)
            {
            printf("   Child %d (pid %d) performed %d opens.\n",f_fe,pid,cnt);
            exit(0);
            }
         }
      }

   if(multi_proc)
      {
      if(debug) printf("Prog8 pid %d iter %d waiting.\n",pid,i);
      if((fpid=wait(&st))==-1 || st!=0)
         {sprintf(em,"- ERROR 5: wait(1) failed (st=%d)",st); errex(em);}
      else if(debug) printf("   Prog8 pid %d child %d done (1).\n",pid,fpid);
      if((fpid=wait(&st))==-1 || st!=0)
         {sprintf(em,"- ERROR 6: wait(2) failed (st=%d)",st); errex(em);}
      else if(debug) printf("   Prog8 pid %d child %d done (2).\n",pid,fpid);
      if((fpid=wait(&st))==-1 || st!=0)
         {sprintf(em,"- ERROR 7: wait(3) failed (st=%d)",st); errex(em);}
      else if(debug) printf("   Prog8 pid %d child %d done (3).\n",pid,fpid);
      }
   else printf("Performed %d opens.\n",cnt);
   printf("Prog8 pid %d iter %i done with open test.\n",pid,i);


   printf("\nProg8 pid %d iter %i starting creat test.\n",pid,i);
   cnt = 0;
   for(f_fe=0; f_fe<=2; ++f_fe) /* file nonexistent, file empty, file w/ data */
   for(mode1=0; mode1<=7; ++mode1) /* mode octet 1 (msb) */
      {
      if(longmode) {m2s = m3s = 1; m2e = m3e = 7;}
      else m2s = m3s = m2e = m3e = mode1;
      for(mode2=m2s; mode2<=m2e; ++mode2) /* mode octet 2 */
      for(mode3=m3s; mode3<=m3e; ++mode3) /* mode octet 3 (lsb) */
         {
         file_created = FALSE;
         mode = mode1*64 + mode2*8 + mode3;
         if (!mode) continue;
         sprintf(fn,"f%d.%03o",f_fe,mode);
         setup_file(fn,f_fe,mode); ++cnt;
         save_errno = -1; fd = creat(fn,mode); save_errno = errno;
         if(fd >= 0) file_created = TRUE;
         check_file(fn,fd,f_fe,-1,mode,FALSE);
         if(file_created)
            {
            sprintf(nfn,"F%d.%03o",f_fe,mode);
            if(rename(fn,nfn))
               {sprintf(em,"- ERROR 8: rename(%s) failed",fn); errex(em);}
            check_file(nfn,fd,f_fe,-1,mode,TRUE);
            }
         if(fd>=0 && close(fd))
            {sprintf(em,"- ERROR 9: close(2;%s) failed",fn); errex(em);}
         if(file_created && unlink(file_created? nfn:fn))
            {
            sprintf(em,"- ERROR 10: unlink(2;%s) failed",file_created? nfn:fn);
            errex(em);
            }
         }
      }
   printf("Performed %d creats.\n",cnt);
   printf("Prog8 pid %d iter %i done with creat test.\n",pid,i);

#ifdef _AIX
   if(afsflag) {printf(">>> Skipping O_DEFER test in AFS. <<<\n"); goto L1;}
   if(dfsflag) {printf(">>> Skipping O_DEFER test in DFS. <<<\n"); goto L1;}
   if(efsflag) {printf(">>> Skipping O_DEFER test in Episode. <<<\n"); goto L1;}
   printf("\nProg8 pid %d iter %i starting O_DEFER/fsync test.\n",pid,i);
   strcpy(fn,"defer_file");
   sprintf(z,"This is %s.",fn); l=strlen(z);

   if((fd=open(fn,O_RDWR|O_CREAT|O_DEFER,0666)) <= 0)
      {sprintf(em,"- ERROR 11: open(%s,RDWR) failed",fn); errex(em);}
   if((rf=write(fd,z,(RWA3)(l+1))) != l+1)
      {sprintf(em,"- ERROR 12: write(%s) failed (got %d)",fn,rf); errex(em);}
   if((ls=lseek(fd,(off_t)0,0)) != 0)
      {
      sprintf(em,"- ERROR 13: lseek (%s) failed (got %ld)",fn,(long)ls);
      errex(em);
      }
   if((rf=read(fd,z1,(RWA3)sizeof(z1))) < 0)
      {sprintf(em,"- ERROR 14: read(%s) failed (got %d)",fn,rf); errex(em);}
   if(!EQ(z,z1))
      {sprintf(em,"- ERROR 15: read compare(%s) failed",fn); errex0(em);}
   printf("Prog8 read what was written.\n");
   if(close(fd)) {sprintf(em,"- ERROR 16: close(%s) failed",fn); errex(em);}
   /* O_DEFER without fsync() should leave data unwritten to disk */

   if((fd=open(fn,O_RDONLY)) <= 0)
      {sprintf(em,"- ERROR 17: open(%s,READ) failed",fn); errex(em);}
   if((rf=read(fd,z1,(RWA3)sizeof(z1))) != 0)
      {sprintf(em,"- ERROR 18: read(%s) failed (got %d)",fn,rf); errex(em);}
   printf("Prog8 read nothing without fsync().\n");
   if(close(fd)) {sprintf(em,"- ERROR 19: close(%s) failed",fn); errex(em);}

   sprintf(z,"THIS IS ALSO %s!",fn); l=strlen(z);
   if((fd=open(fn,O_WRONLY|O_DEFER)) <= 0)
      {sprintf(em,"- ERROR 20: open(%s WRITE) failed",fn); errex(em);}
   if((rf=write(fd,z,(RWA3)(l+1))) != l+1)
      {sprintf(em,"- ERROR 21: write(%s) failed (got %d)",fn,rf); errex(em);}
   if(fsync(fd)) {sprintf(em,"- ERROR 22: fsync(%s) failed",fn); errex(em);}
   if(close(fd)) {sprintf(em,"- ERROR 23: close(%s) failed",fn); errex(em);}

   if((fd=open(fn,O_RDONLY)) <= 0)
      {sprintf(em,"- ERROR 24: open(%s,READ) failed",fn); errex(em);}
   if((rf=read(fd,z1,(RWA3)sizeof(z1))) < 0)
      {sprintf(em,"- ERROR 25: read(%s) failed (got %d)",fn,rf); errex(em);}
   if(!EQ(z,z1))
      {sprintf(em,"- ERROR 26: read compare(%s) failed",fn); errex0(em);}
   printf("Prog8 read properly after fsync().\n");
   if(close(fd)) {sprintf(em,"- ERROR 27: close(%s) failed",fn); errex(em);}

   if(unlink(fn)) {sprintf(em,"- ERROR 28: unlink(%s) failed",fn); errex(em);}
   L1: i = i;
#endif
   }

if(umask(um) != 0) {strcpy(em,"umask"); errex(em);}
printf("\nProg8 pid %d exiting.\n",pid);
exit(0);
}

/*****************************************************************************/

void check_file(fn,fd,f_fe,oflag,mode,rflag)
/* oflag == -1 means this file was created via creat() */
/* rflag == TRUE means this is the renamed version of the file */
char *fn;
int fd,f_fe,oflag,mode;
{
char z[81],z1[81],z2[81];
int cflag=FALSE,f1,f2,f3,fd1,fd2,i,l,rf;
static int dtype=0; /* cycle amongst 3 ways to do the dup() function */
off_t ls;
struct stat s,s1;

if(oflag == -1) {cflag=TRUE; oflag = O_WRONLY | O_CREAT | O_TRUNC;}

f1 = oflag & 07;           /* r/w open mode (O_RDONLY or O_WRONLY or O_RDWR) */
f2 = (f_fe==2 && !(oflag & O_TRUNC));     /* TRUE iff contents "This is ..." */
f3 = oflag & O_APPEND;                     /* TRUE iff opened in append mode */

/* check the value of fd (to see if the open succeeded) **********************/

if(f_fe && (oflag & O_CREAT) && (oflag & O_EXCL))
   {
   /* open(O_CREAT|O_EXCL) should fail (EEXIST) if the file already exists */
   if(fd >= 0)
      {
      sprintf(em,"- ERROR 29: '%s' existed but open-exclusive succeeded",fn);
      errex0(em);
      }
   else if(save_errno != EEXIST)
      {
      sprintf(em,"- ERROR 30: open-exclusive(%s) errno = %d is wrong",
         fn,save_errno);
      errex0(em);
      }
   }
else if(!f_fe && !(oflag & O_CREAT))
   {
   /* open(not-O_CREAT) should fail (ENOENT) if the file does not exist */
   if(fd >= 0)
      {
      sprintf(em,"- ERROR 31: '%s' didn't exist but open succeeded",fn);
      errex0(em);
      }
   else if(save_errno != ENOENT)
      {
      sprintf(em,"- ERROR 32: open(%s) errno = %d is wrong",fn,save_errno);
      errex0(em);
      }
   }
else if(f_fe && ((!can_read(mode) && f1!=O_WRONLY) ||
                 (!can_write(mode) && (f1!=O_RDONLY || (oflag & O_TRUNC)))))
   {
   /* open() with oflag/mode conflict should fail (EACCES) */
   if(fd >= 0)
      {
      sprintf(em,"- ERROR 33: '%s' open succeeded despite oflag/mode conflict",
         fn);
      errex0(em);
      }
   else if(save_errno != EACCES)
      {
      sprintf(em,"- ERROR 34: open(%s) errno = %d is wrong",fn,save_errno);
      errex(em);
      }
   }
else if(open_wc_bug && afsflag && f_fe &&
   can_write(mode) && !can_read(mode) && (oflag & O_CREAT))
   {
   /*
      Open() with O_WRONLY|O_CREAT will fail (EACCES) if the file exists and
      is writable but not readable. This only happens on certain machine types
      using AFS 3.1 (i.e., it doesn't happen on the local disk on those
      machines).
   */
   if(fd >= 0)
      {
      sprintf(em,"- ERROR 35: '%s' open succeeded despite WRONLY|CREAT bug",
         fn);
      errex0(em);
      }
   else if(save_errno != EACCES)
      {
      sprintf(em,"- ERROR 36: open(%s) errno = %d is wrong",fn,save_errno);
      errex0(em);
      }
   }
else if(fd < 0) /* otherwise the open should succeed */
   {
   sprintf(em,"- ERROR 37: %s(%s) failed (errno=%d)",cflag? "creat":"open",
      fn,save_errno);
   errex0(em);
   }

if(fd < 0) return;

/* fd1=dup(fd); stat() and read() using both fd's; close(fd1) ****************/

switch(dtype) /* dtype cycles amongst ways to do dup() */
   {
   case 0:
      if((fd1=dup(fd)) < 0)
         {sprintf(em,"- ERROR 38: dup(%s) failed",fn); errex(em);}
      ++dtype; break;
   case 1:
      if((fd1=dup2(fd,10)) < 0)
         {sprintf(em,"- ERROR 39: dup2(%s) failed",fn); errex(em);}
      ++dtype; break;
   case 2:
      if((fd1=fcntl(fd,F_DUPFD,0)) < 0)
         {sprintf(em,"- ERROR 40: fcntl-dup(%s) failed",fn); errex(em);}
      ++dtype; break;
   case 3:
      if((fd1=fcntl(fd,F_DUPFD,10)) < 0)
         {sprintf(em,"- ERROR 41: fcntl-dup2(%s) failed",fn); errex(em);}
      dtype = 0; break;
   default: sprintf(em,"- ERROR 42: dtype switch failed"); errex0(em);
   }

if(stat(fn,&s) != 0)
   {sprintf(em,"- ERROR 43: stat(%s) failed",fn); errex(em);}
if(fstat(fd,&s1) != 0)
   {sprintf(em,"- ERROR 44: fstat(%s) failed",fn); errex(em);}
if(!st_compare(s,s1))
   {sprintf(em,"- ERROR 45: st_compare(%s) failed",fn); errex0(em);}
if(fstat(fd1,&s1) != 0)
   {sprintf(em,"- ERROR 46: dup fstat(%s) failed",fn); errex(em);}
if(!st_compare(s,s1))
   {sprintf(em,"- ERROR 47: dup st_compare(%s) failed",fn); errex0(em);}

#ifdef _AIX
if(!afsflag && (mode & S_IFDIR)) s.st_mode &= (~0x10000); /* JFS log bit */
#endif

if(s.st_mode != (S_IFREG|mode))
   {
   sprintf(em,"- ERROR 48: st_mode of %s is wrong (%lu, not %d)",
      fn,(unsigned long)s.st_mode,S_IFREG|mode);
   errex0(em);
   }

sprintf(z,"This is %s.",fn);
if((f2 && s.st_size!=strlen(z)+1) || (!f2 && s.st_size!=0))
   {sprintf(em,"- ERROR 49: st_size of %s is wrong",fn); errex0(em);}

if(rflag) /* file has been renamed */
   {
   if(f2 && can_read(mode) && f1!=O_WRONLY)
      {
       /*
	* RA: Since I have a file descriptor, the following read 
	* should succeed. Hence commented out and replaced by
	* appropriate read call
	*
	 if((rf=read(fd,z1,(RWA3)sizeof(z1))) > 0)
         {
         sprintf(em,"- ERROR 50: read(%s) succeeded despite rename (got %d)",
            fn,rf);
         errex0(em);
         }
        */
	 if((rf=read(fd,z1,(RWA3)sizeof(z1))) < 0)
         {
         sprintf(em,"- ERROR 50: read(%s) failed after rename (got %d)",
            fn,rf);
         errex(em);
         }
       }
   if(f2 && can_read(mode))
      {
      if((fd2=open(fn,O_RDONLY)) == -1)
         {sprintf(em,"- ERROR 51: open(%s) failed",fn); errex(em);}
      sprintf(z,"This is %s.",fn); z[8]=tolower(z[8]);
      if((rf=read(fd2,z1,(RWA3)sizeof(z1))) < 0)
         {sprintf(em,"- ERROR 52: read(%s) failed (got %d)",fn,rf); errex(em);}
      if(!EQ(z,z1))
         {sprintf(em,"- ERROR 53: read compare(%s) failed",fn); errex0(em);}
      if(close(fd2) == -1)
         {sprintf(em,"- ERROR 54: close(%s) failed",fn); errex(em);}
      }
   }
else /* file has not been renamed */
   {
   if(f2 && can_read(mode) && f1!=O_WRONLY)
      {
      if((rf=read(fd,z1,(RWA3)(sizeof(z1)))) < 0)
         {sprintf(em,"- ERROR 55: read(%s) failed (got %d)",fn,rf); errex(em);}
      if(!EQ(z,z1))
         {sprintf(em,"- ERROR 56: contents of '%s' are wrong",fn); errex0(em);}

      if((ls=lseek(fd1,(off_t)0,0)) != 0)
         {
         sprintf(em,"- ERROR 57: dup lseek (%s) failed (got %ld)",fn,(long)ls);
         errex(em);
         }
      if((rf=read(fd1,z1,(RWA3)(sizeof(z1)))) < 0)
         {
         sprintf(em,"- ERROR 58: dup read(%s) failed (got %d)",fn,rf);
         errex(em);
         }
      if(!EQ(z,z1))
         {sprintf(em,"- ERROR 59: dup contents of '%s' are wrong",fn); errex0(em);}
      }
   }

if(close(fd1)) {sprintf(em,"- ERROR 60: dup close(%s) failed",fn); errex(em);}

/* check access() results ****************************************************/

save_errno = -1; rf = access(fn,R_OK); save_errno = errno;
if(rf!=0 && can_read(mode))
   {
   sprintf(em,"- ERROR 61: read-access test on '%s' failed (rf=%d, errno=%d)",
      fn,rf,save_errno);
   errex0(em);
   }
else if(!can_read(mode) && (rf==0 || save_errno!=EACCES))
   {
   sprintf(em,"- ERROR 62: read-access test on '%s' failed (rf=%d, errno=%d)",
      fn,rf,save_errno);
   errex0(em);
   }

save_errno = -1; rf = access(fn,W_OK); save_errno = errno;
if(rf!=0 && can_write(mode))
   {
   sprintf(em,"- ERROR 63: write-access test on '%s' failed (rf=%d, errno=%d)",
      fn,rf,save_errno);
   errex0(em);
   }
else if(!can_write(mode) && (rf==0 || save_errno!=EACCES))
   {
   sprintf(em,"- ERROR 64: write-access test on '%s' failed (rf=%d, errno=%d)",
      fn,rf,save_errno);
   errex0(em);
   }

save_errno = -1; rf = access(fn,X_OK); save_errno = errno;
if(rf!=0 && can_ex(mode))
   {
   sprintf(em,"- ERROR 65: exec-access test on '%s' failed (rf=%d, errno=%d)",
      fn,rf,save_errno);
   errex0(em);
   }
else if(!can_ex(mode) && (rf==0 || save_errno!=EACCES))
   {
   sprintf(em,"- ERROR 66: exec-access test on '%s' failed (rf=%d, errno=%d)",
      fn,rf,save_errno);
   errex0(em);
   }

/* do more write()'s and read()'s (if possible) ******************************/
#ifdef SPARC
if(rflag && f1!=O_RDONLY && can_write(mode) && !(afsflag && !can_read(mode)))
#else
if(rflag && f1!=O_RDONLY && can_write(mode))
#endif
   {
   strcpy(z1,"This is a test of the emergency broadcast system."); l=strlen(z1);
   for(i=1; i<=2; ++i)
      {
      if((ls=lseek(fd,(off_t)0,0)) != 0)
         {
         sprintf(em,"- ERROR 67: lseek(%s) failed (got %ld)",fn,(long)ls);
         errex(em);
         }
      if((rf=write(fd,z1,(RWA3)(l+1))) != l+1)
         {sprintf(em,"- ERROR 68: write(%s) failed (got %d)",fn,rf); errex(em);}
      }
   if(fstat(fd,&s) != 0)
      {sprintf(em,"- ERROR 69: fstat(%s) failed",fn); errex(em);}
   if(f2) {sprintf(z,"This is %s.",fn); z[8]=tolower(z[8]);} else *z='\0';
   l = f3? strlen(z)+2*strlen(z1)+2+(*z? 1:0):max(strlen(z),strlen(z1))+1;
   if(s.st_size != l)
      {sprintf(em,"- ERROR 70: st_size of %s is wrong",fn); errex0(em);}

   if(f1!=O_WRONLY && can_read(mode))
      {
      if((ls=lseek(fd,(off_t)0,0)) != 0)
         {
         sprintf(em,"- ERROR 71: lseek (%s) failed (got %ld)",fn,(long)ls);
         errex(em);
         }
      if(f2 && f3) strcpy(z1,z);
      if((rf=read(fd,z2,(RWA3)(strlen(z1)))) < 0)
         {sprintf(em,"- ERROR 72: read(%s) failed (got %d)",fn,rf); errex(em);}
      if(strncmp(z1,z2,strlen(z1)) != 0)
         {sprintf(em,"- ERROR 73: read compare '%s' failed",fn); errex0(em);}
      }
   }
}

/*****************************************************************************/

void setup_file(fn,flag,mode)
char *fn;
int flag,mode;
{
char z[81];
int fd,l,rf;

if(!flag) return;

if((fd=creat(fn,0666)) < 0)
   {sprintf(em,"- ERROR 74: create(%s) failed",fn); errex(em);}
else file_created = TRUE;

if(flag == 2)
   {
   sprintf(z,"This is %s.",fn); l=strlen(z);
   if((rf=write(fd,z,(RWA3)(l+1))) != l+1)
      {sprintf(em,"- ERROR 75: write(%s) failed (got %d)",fn,rf); errex(em);}
   }

if(close(fd)) {sprintf(em,"- ERROR 76: close(%s) failed",fn); errex(em);}

if(chmod(fn,mode)) {sprintf(em,"- ERROR 77: chmod(%s) failed",fn); errex(em);}
}

/*****************************************************************************/

int st_compare(s1,s2)
struct stat s1,s2;
{
int tmp;

if (s1.st_dev != s2.st_dev) { 
  if (debug)
    printf("s1.st_dev %d is not equal to s2.st_dev %d\n", s1.st_dev,s2.st_dev);
  return 0;
}
else if   (s1.st_ino != s2.st_ino) {
  if (debug)
    printf("s1.st_ino %d is not equal to s2.st_ino %d\n", s1.st_ino,s2.st_ino);
  return 0;
}
else if (s1.st_mode != s2.st_mode) {
  if (debug)
    printf("s1.st_mode %d is not equal to s2.st_mode %d\n", s1.st_mode,s2.st_mode);
  return 0;
}
else if (s1.st_nlink != s2.st_nlink) {
  if (debug)
    printf("s1.st_nlink %d is not equal to s2.st_nlink %d\n", s1.st_nlink,s2.st_nlink);
  return 0;
}
else if (s1.st_uid != s2.st_uid) {
  if (debug)
    printf("s1.st_uid %d is not equal to s2.st_uid %d\n", s1.st_uid,s2.st_uid);
  return 0;
}
else if (s1.st_gid != s2.st_gid) {
  if (debug)
    printf("s1.st_gid %d is not equal to s2.st_gid %d\n", s1.st_gid,s2.st_gid);
  return 0;
}
else if (s1.st_size != s2.st_size) {
  if (debug)
    printf("s1.st_size %d is not equal to s2.st_size %d\n", s1.st_size,s2.st_size);
  return 0;
}
else if (s1.st_mtime != s2.st_mtime) {
    tmp=s2.st_mtime - s1.st_mtime;
    if (tmp < 0 ) tmp = -tmp;
    if (tmp > TIME_ERROR ) {  
      if (debug) {
        printf("The difference between s1.st_mtime %d and s2.st_mtime %d is greater than the alowance %d\n", s1.st_mtime,s2.st_mtime,TIME_ERROR);
        printf("The system clock may be badly skewed or the operation is too slow\n");
      }
      return 0;
    }
}
else if (s1.st_ctime != s2.st_ctime) {
    tmp=s2.st_ctime - s1.st_ctime;
    if (tmp < 0 ) tmp = -tmp;
    if (tmp > TIME_ERROR ) {  
      if (debug) {
        printf("The difference between s1.st_ctime %d and s2.st_ctime %d is greate than the alowance %d\n", s1.st_ctime,s2.st_ctime,TIME_ERROR);
        printf("The system clock may be badly skewed or the operation is too slow\n");
      }
      return 0;
    }
}
else if (s1.st_blksize != s2.st_blksize) {
  if (debug)
    printf("s1.st_blksize %d is not equal to s2.st_blksize %d\n", s1.st_blksize,s2.st_blksize);
  return 0;
}
else if (s1.st_blocks != s2.st_blocks) {
  if (debug)
    printf("s1.st_blocks %d is not equal to s2.st_blocks %d\n", s1.st_blocks,s2.st_blocks);
  return 0;
}
else return 1;
}

/*****************************************************************************/

void errex(s) char *s;
{
        FILE *dumpfilep;
        char dumpfile[256];

        printf("\n%s; errno = %d\n",s,errno);
        if (debug) {
            sprintf(dumpfile,"/tmp/low.prog8.kernel.dump.%d",getpid());
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

        printf("\n%s\n",s);
        if (debug) {
            sprintf(dumpfile,"/tmp/low.prog8.kernel.dump.%d",getpid());
            dumpfilep = fopen(dumpfile, "w");
            icl_DumpKernel(dumpfilep, NULL, ICL_DUMP_FORMATTED);
            fclose(dumpfilep);
        }
        exit(1);
}


/*****************************************************************************/
