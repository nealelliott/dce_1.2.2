/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: prog7.c,v $
 * Revision 1.1.121.1  1996/10/17  18:29:31  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:54:51  damon]
 *
 * Revision 1.1.116.1  1994/02/04  20:47:06  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:25:21  devsrc]
 * 
 * Revision 1.1.114.1  1993/12/07  17:46:05  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:38:26  jaffe]
 * 
 * Revision 1.1.10.2  1993/07/19  19:44:29  zeliff
 * 	HP port of DFS
 * 	[1993/07/19  18:33:25  zeliff]
 * 
 * Revision 1.1.8.3  1993/07/16  22:13:59  kissel
 * 	Get rid of the damn quotes in #include.
 * 	[1993/06/21  15:11:16  kissel]
 * 
 * Revision 1.1.5.6  1993/03/09  21:23:14  jaffe
 * 	Transarc delta: jess-ot7194-fs-low-kerneldump 1.1
 * 	  Selected comments:
 * 	    Change the icl_KernelDump parameter according to the changes
 * 	    from icl utilities.
 * 	    See above.
 * 	[1993/03/09  20:54:57  jaffe]
 * 
 * Revision 1.1.5.5  1993/01/11  18:33:29  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:15:44  htf]
 * 
 * Revision 1.1.5.4  1992/12/09  21:33:33  jaffe
 * 	Transarc delta: jaffe-fix-2.2-test-build 1.2
 * 	  Selected comments:
 * 
 * 	    use pid_t for pid and getpid.
 * 	[1992/12/07  13:38:25  jaffe]
 * 
 * Revision 1.1.5.3  1992/11/24  21:46:13  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:46:19  bolinger]
 * 
 * Revision 1.1.5.2  1992/10/28  18:13:26  jaffe
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
 * 	[1992/10/28  14:34:15  jaffe]
 * 
 * Revision 1.1.3.3  1992/01/24  04:15:35  devsrc
 * 	Merging dfs6.3
 * 	[1992/01/24  00:23:57  devsrc]
 * 
 * Revision 1.1.3.2  1992/01/23  20:31:39  zeliff
 * 	Moving files onto the branch for dfs6.3 merge
 * 	[1992/01/23  18:48:48  zeliff]
 * 	Revision 1.1.1.3  1992/01/23  22:26:35  devsrc
 * 	Fixed logs
 * 
 * $EndLog$
*/
/*****************************************************************************/
/* Copyright (C) 1990, 1991 Transarc Corporation - All rights reserved */
/*****************************************************************************/

#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <common.h>
#include <dcedfs/icl.h>
#include <string.h>


#ifndef S_ISDIR
#define S_ISDIR(m)	(((m)&(S_IFMT)) == (S_IFDIR))
#endif

#ifndef S_ISLNK
#define S_ISLNK(m)	(((m)&(S_IFMT)) == (S_IFLNK))
#endif

#ifndef S_ISREG
#define S_ISREG(m)	(((m)&(S_IFMT)) == (S_IFREG))
#endif

#define FMODE 0644
#define SFMODE 0466
#define DMODE 0755

int debug=TRUE;
struct timeval tv_prev,tv_cur;
struct timezone tz;

double endtime();
int st_compare();
void errex(), errex0(), starttime();

extern int errno;
extern pid_t getpid();

/*****************************************************************************/

main(argc,argv) int argc; char *argv[];
{
char *dir="tdir.7",fn[256],wd[256],z[256],z1[256];
int afsflag,cnt,dfsflag,fd,i,j,k,l,nr_times,nr_files,nr_dirs,nr_hard,
    nr_subf,nr_sym,nr_ne_sym,rf,um;
pid_t pid,pid2;
struct stat s,s1;

setbuf(stdout,(char *)NULL);

if(argc < 8)
   {nr_times = nr_files = nr_dirs = nr_hard = nr_subf = nr_sym = nr_ne_sym = 1;}
else
   {
   nr_times = atoi(argv[1]);
   nr_subf = atoi(argv[2]);
   nr_files = atoi(argv[3]);
   nr_dirs = atoi(argv[4]);
   nr_hard = atoi(argv[5]);
   nr_sym = atoi(argv[6]);
   nr_ne_sym = atoi(argv[7]);
   }

pid = getpid(); pid2 = pid%10000;
um = umask(0);

printf("Prog7 pid %d, nr_times %d, nr_files %d, nr_dirs %d, nr_subf %d.\n",
   pid,nr_times,nr_files,nr_dirs,nr_subf);
printf("                 nr_hard %d, nr_sym %d, nr_ne_sym %d.\n",
   nr_hard,nr_sym,nr_ne_sym);

(void)getwd(wd);
afsflag = (strncmp(wd,"/afs/",5) == 0);
dfsflag = (strncmp(wd,"/.../",5) == 0);

printf("This is%s an AFS filesystem.\n",afsflag? "":" not");
printf("This is%s a DFS filesystem.\n",dfsflag? "":" not");

for(i=1; i<=nr_times || nr_times==0; ++i)
   {
/**************************************/

   printf("\nProcess %d starting iteration number %d.\n",pid,i);
   if(debug) printf("Creating files and directories.\n");

   for(j=1; j<=nr_dirs; ++j)
      {
      sprintf(fn,"dir.%d.%d",pid,j);
      if(rf=mkdir(fn,DMODE)) errex("mkdir(1)");
      if(rf=chdir(fn)) errex("chdir(1)");

      for(k=1; k<=nr_subf; ++k)
         {
         sprintf(fn,"subf.%d.%d",j,k);
         if((fd=open(fn,O_WRONLY|O_CREAT,SFMODE)) < 0) errex("open(1)");
         sprintf(z,"This is %s/%s.",wd,fn); l=strlen(z);
         if(write(fd,z,(RWA3)(l+1)) != l+1) errex("write(1)");
         if(close(fd)) errex("close(1)");
         }

      if(rf=chdir("..")) errex("chdir(2)");
      }

   for(j=1; j<=nr_files; ++j)
      {
      sprintf(fn,"file.%d.%d",pid,j);
      if((fd=open(fn,O_WRONLY|O_CREAT,FMODE)) < 0) errex("open(2)");
      sprintf(z,"This is %s/%s.",wd,fn); l=strlen(z);
      if(write(fd,z,(RWA3)(l+1)) != l+1) errex("write(2)");
      if(close(fd)) errex("close(2)");
      }

/**************************************/

   if(debug) printf("Making hard links in '%s'.\n",dir);
   if(rf=chdir(dir)) errex("chdir(3)");

   if(nr_hard)
      {
      cnt=0; starttime();
      if(afsflag)
         {
         printf(">>> Cannot make hard links in AFS. <<<\n");
         nr_hard=0; goto L1;
         }
      if(dfsflag)
         {
         printf(">>> Cannot make hard links in DFS. <<<\n");
         nr_hard=0; goto L1;
         }

      for(j=1; j<=nr_files; ++j)
         {
         sprintf(fn,"%s/file.%d.%d",wd,pid,j);
         for(k=1; k<=nr_hard; ++k)
            {
            sprintf(z,"hf.%d.%d.%d",pid2,j,k);
            if(rf=link(fn,z)) errex("link(1)"); else ++cnt;
            }
         }

      for(j=1; j<=nr_dirs; ++j)
         {
         for(k=1; k<=nr_subf; ++k)
            {
            sprintf(fn,"%s/dir.%d.%d/subf.%d.%d",wd,pid,j,j,k);
            for(l=1; l<=nr_hard; ++l)
               {
               sprintf(z,"hs.%d.%d.%d.%d",pid2,j,k,l);
               if(rf=link(fn,z)) errex("link(2)"); else ++cnt;
               }
            }
         }

L1:
      printf("Pid %d made %d hard links; time = %.1f seconds.\n",
         pid,cnt,endtime());
      }

/**************************************/

   if(debug) printf("Making symlinks in '%s'.\n",dir);
   if(nr_sym)
      {
      cnt=0; starttime();

      for(j=1; j<=nr_files; ++j)
         {
         sprintf(fn,"%s/file.%d.%d",wd,pid,j);
         for(k=1; k<=nr_sym; ++k)
            {
            sprintf(z,"sf.%d.%d.%d",pid2,j,k);
            if(rf=symlink(fn,z)) errex("symlink(1)"); else ++cnt;
            }
         }

      for(j=1; j<=nr_dirs; ++j)
         {
         sprintf(fn,"%s/dir.%d.%d",wd,pid,j);
         for(k=1; k<=nr_sym; ++k)
            {
            sprintf(z,"sd.%d.%d.%d",pid2,j,k);
            if(rf=symlink(fn,z)) errex("symlink(2)"); else ++cnt;
            }
         for(k=1; k<=nr_subf; ++k)
            {
            sprintf(fn,"%s/dir.%d.%d/subf.%d.%d",wd,pid,j,j,k);
            for(l=1; l<=nr_sym; ++l)
               {
               sprintf(z,"ss.%d.%d.%d.%d",pid2,j,k,l);
               if(rf=symlink(fn,z)) errex("symlink(3)"); else ++cnt;
               }
            }
         }

      printf("Pid %d made %d symlinks; time = %.1f seconds.\n",
         pid,cnt,endtime());
      }

/**************************************/

   if(debug) printf("Making non-existent symlinks in '%s'.\n",dir);
   if(nr_ne_sym)
      {
      cnt=0; starttime();

      for(j=1; j<=nr_files; ++j)
         {
         sprintf(fn,"%s/FILE.%d.%d",wd,pid,j);
         for(k=1; k<=nr_ne_sym; ++k)
            {
            sprintf(z,"sF.%d.%d.%d",pid2,j,k);
            if(rf=symlink(fn,z)) errex("symlink(4)"); else ++cnt;
            }
         }

      for(j=1; j<=nr_dirs; ++j)
         {
         sprintf(fn,"%s/DIR.%d.%d",wd,pid,j);
         for(k=1; k<=nr_ne_sym; ++k)
            {
            sprintf(z,"sD.%d.%d.%d",pid2,j,k);
            if(rf=symlink(fn,z)) errex("symlink(5)"); else ++cnt;
            }
         for(k=1; k<=nr_subf; ++k)
            {
            sprintf(fn,"%s/DIR.%d.%d/SUBF.%d.%d",wd,pid,j,j,k);
            for(l=1; l<=nr_ne_sym; ++l)
               {
               sprintf(z,"sS.%d.%d.%d.%d",pid2,j,k,l);
               if(rf=symlink(fn,z)) errex("symlink(6)"); else ++cnt;
               }
            }
         }

      printf("Pid %d made %d non-existent symlinks; time = %.1f seconds.\n",
         pid,cnt,endtime());
      }

/**************************************/

   if(debug) printf("Checking links.\n");
   /* We are currently in the 'dir' directory. */
   starttime();

   for(j=1; j<=nr_files; ++j)
      {
      sprintf(fn,"%s/file.%d.%d",wd,pid,j);
      if(stat(fn,&s) != 0) errex("stat(1)");
      if((fd=open(fn,O_RDONLY)) < 0) errex("open(3)");
      if(fstat(fd,&s1) != 0) errex("fstat(1)");
      if(!st_compare(s,s1)) errex0("st_compare(1)");
      sprintf(z,"This is %s.",fn);
      if(read(fd,z1,(RWA3)sizeof(z1)) < 0) errex("read(1)");
      if(!EQ(z,z1)) errex0("read compare(1)");
      if(close(fd)) errex("close(3)");
      sprintf(fn,"../file.%d.%d",pid,j);
      if(stat(fn,&s1) != 0) errex("stat(2)");
      if(!st_compare(s,s1)) errex0("st_compare(2)");
      if(!S_ISREG(s.st_mode)) errex0("stat mode(1)");
      if((s.st_mode & 0777) != FMODE) errex0("stat mode(2)");
      if(s.st_nlink != nr_hard+1) errex0("stat nlink(1)");
      if(s.st_size != strlen(z)+1) errex0("stat size(1)");

      for(k=1; k<=nr_hard; ++k)
         {
         sprintf(fn,"hf.%d.%d.%d",pid2,j,k);
         if(stat(fn,&s1) != 0) errex("stat(3)");
         if(!st_compare(s,s1)) errex0("st_compare(3)");
         if((fd=open(fn,O_RDONLY)) < 0) errex("open(4)");
         if(fstat(fd,&s1) != 0) errex("fstat(2)");
         if(!st_compare(s,s1)) errex0("st_compare(4)");
         sprintf(z,"This is %s/file.%d.%d.",wd,pid,j);
         if(read(fd,z1,(RWA3)sizeof(z1)) < 0) errex("read(2)");
         if(!EQ(z,z1)) errex0("read compare(2)");
         if(close(fd)) errex("close(4)");
         }

      for(k=1; k<=nr_sym; ++k)
         {
         sprintf(fn,"sf.%d.%d.%d",pid2,j,k);
         if((rf=readlink(fn,z1,sizeof(z1)-1)) <= 0) errex("readlink(1)");
         else z1[rf] = '\0';
         sprintf(z,"%s/file.%d.%d",wd,pid,j);
         if(!EQ(z,z1)) errex0("readlink compare(1)");
         if(lstat(fn,&s1) != 0) errex("lstat(1)");
         if(!S_ISLNK(s1.st_mode)) errex0("lstat mode(1)");
         if(s1.st_nlink != 1) errex0("lstat nlink(1)");
         if(s1.st_size != strlen(z)) errex0("lstat size(1)");

         if(stat(fn,&s1) != 0) errex("stat(4)");
         if(!st_compare(s,s1)) errex0("st_compare(5)");
         if((fd=open(fn,O_RDONLY)) < 0) errex("open(5)");
         if(fstat(fd,&s1) != 0) errex("fstat(3)");
         if(!st_compare(s,s1)) errex0("st_compare(6)");
         sprintf(z,"This is %s/file.%d.%d.",wd,pid,j);
         if(read(fd,z1,(RWA3)sizeof(z1)) < 0) errex("read(3)");
         if(!EQ(z,z1)) errex0("read compare(3)");
         if(close(fd)) errex("close(5)");
         }

      for(k=1; k<=nr_ne_sym; ++k)
         {
         sprintf(fn,"sF.%d.%d.%d",pid2,j,k);
         if((rf=readlink(fn,z1,sizeof(z1)-1)) <= 0) errex("readlink(2)");
         else z1[rf] = '\0';
         sprintf(z,"%s/FILE.%d.%d",wd,pid,j);
         if(!EQ(z,z1)) errex0("readlink compare(2)");
         if(lstat(fn,&s1) != 0) errex0("lstat(2)");
         if(!S_ISLNK(s1.st_mode)) errex0("lstat mode(2)");
         if(s1.st_nlink != 1) errex0("lstat nlink(2)");
         if(s1.st_size != strlen(z)) errex0("lstat size(2)");
         }
      }

   for(j=1; j<=nr_dirs; ++j)
      {
      sprintf(fn,"%s/dir.%d.%d",wd,pid,j);
      if(stat(fn,&s) != 0) errex("stat(5)");
      sprintf(fn,"../dir.%d.%d",pid,j);
      if(stat(fn,&s1) != 0) errex0("stat(6)");
      if(!st_compare(s,s1)) errex0("st_compare(7)");
      if(!S_ISDIR(s.st_mode)) errex0("stat mode(3)");
      if((s.st_mode & 0777) != DMODE) errex0("stat mode(4)");

      for(k=1; k<=nr_sym; ++k)
         {
         sprintf(fn,"sd.%d.%d.%d",pid2,j,k);
         if((rf=readlink(fn,z1,sizeof(z1)-1)) <= 0) errex("readlink(3)");
         else z1[rf] = '\0';
         sprintf(z,"%s/dir.%d.%d",wd,pid,j);
         if(!EQ(z,z1)) errex0("readlink compare(3)");
         if(lstat(fn,&s1) != 0) errex0("lstat(3)");
         if(!S_ISLNK(s1.st_mode)) errex0("lstat mode(3)");
         if(s1.st_nlink != 1) errex0("lstat nlink(3)");
         if(s1.st_size != strlen(z)) errex0("lstat size(3)");
         if(stat(fn,&s1) != 0) errex("stat(7)");
         if(!st_compare(s,s1)) errex0("st_compare(8)");
         }

      for(k=1; k<=nr_ne_sym; ++k)
         {
         sprintf(fn,"sD.%d.%d.%d",pid2,j,k);
         if((rf=readlink(fn,z1,sizeof(z1)-1)) <= 0) errex("readlink(4)");
         else z1[rf] = '\0';
         sprintf(z,"%s/DIR.%d.%d",wd,pid,j);
         if(!EQ(z,z1)) errex0("readlink compare(4)");
         if(lstat(fn,&s1) != 0) errex0("lstat(4)");
         if(!S_ISLNK(s1.st_mode)) errex0("lstat mode(4)");
         if(s1.st_nlink != 1) errex0("lstat nlink(4)");
         if(s1.st_size != strlen(z)) errex0("lstat size(4)");
         }

      for(k=1; k<=nr_subf; ++k)
         {
         sprintf(fn,"%s/dir.%d.%d/subf.%d.%d",wd,pid,j,j,k);
         if(stat(fn,&s) != 0) errex("stat(8)");
         if((fd=open(fn,O_RDONLY)) < 0) errex("open(6)");
         if(fstat(fd,&s1) != 0) errex("fstat(4)");
         if(!st_compare(s,s1)) errex0("st_compare(9)");
         sprintf(z,"This is %s/subf.%d.%d.",wd,j,k);
         if(read(fd,z1,(RWA3)sizeof(z1)) < 0) errex("read(4)");
         if(!EQ(z,z1)) errex0("read compare(4)");
         if(close(fd)) errex("close(6)");
         if(!S_ISREG(s.st_mode)) errex0("stat mode(5)");
         if((s.st_mode & 0777) != SFMODE) errex0("stat mode(6)");
         if(s.st_nlink != nr_hard+1) errex0("stat nlink(2)");
         if(s.st_size != strlen(z)+1) errex0("stat size(2)");

         for(l=1; l<=nr_hard; ++l)
            {
            sprintf(fn,"hs.%d.%d.%d.%d",pid2,j,k,l);
            if(stat(fn,&s1) != 0) errex("stat(9)");
            if(!st_compare(s,s1)) errex0("st_compare(10)");
            if((fd=open(fn,O_RDONLY)) < 0) errex("open(7)");
            if(fstat(fd,&s1) != 0) errex("fstat(5)");
            if(!st_compare(s,s1)) errex0("st_compare(11)");
            sprintf(z,"This is %s/subf.%d.%d.",wd,j,k);
            if(read(fd,z1,(RWA3)sizeof(z1)) < 0) errex("read(5)");
            if(!EQ(z,z1)) errex0("read compare(5)");
            if(close(fd)) errex("close(7)");
            }

         for(l=1; l<=nr_sym; ++l)
            {
            sprintf(fn,"ss.%d.%d.%d.%d",pid2,j,k,l);
            if((rf=readlink(fn,z1,sizeof(z1)-1)) <= 0) errex("readlink(5)");
            else z1[rf] = '\0';
            sprintf(z,"%s/dir.%d.%d/subf.%d.%d",wd,pid,j,j,k);
            if(!EQ(z,z1)) errex0("readlink compare(5)");
            if(lstat(fn,&s1) != 0) errex("lstat(5)");
            if(!S_ISLNK(s1.st_mode)) errex0("lstat mode(5)");
            if(s1.st_nlink != 1) errex0("lstat nlink(5)");
            if(s1.st_size != strlen(z)) errex0("lstat size(5)");

            if(stat(fn,&s1) != 0) errex("stat(10)");
            if(!st_compare(s,s1)) errex0("st_compare(12)");
            if((fd=open(fn,O_RDONLY)) < 0) errex("open(8)");
            if(fstat(fd,&s1) != 0) errex("fstat(6)");
            if(!st_compare(s,s1)) errex0("st_compare(13)");
            sprintf(z,"This is %s/subf.%d.%d.",wd,j,k);
            if(read(fd,z1,(RWA3)sizeof(z1)) < 0) errex("read(6)");
            if(!EQ(z,z1)) errex0("read compare(6)");
            if(close(fd)) errex("close(8)");
            }

         for(l=1; l<=nr_ne_sym; ++l)
            {
            sprintf(fn,"sS.%d.%d.%d.%d",pid2,j,k,l);
            if((rf=readlink(fn,z1,sizeof(z1)-1)) <= 0) errex("readlink(6)");
            else z1[rf] = '\0';
            sprintf(z,"%s/DIR.%d.%d/SUBF.%d.%d",wd,pid,j,j,k);
            if(!EQ(z,z1)) errex0("readlink compare(6)");
            if(lstat(fn,&s1) != 0) errex("lstat(6)");
            if(!S_ISLNK(s1.st_mode)) errex0("lstat mode(6)");
            if(s1.st_nlink != 1) errex0("lstat nlink(6)");
            if(s1.st_size != strlen(z)) errex0("lstat size(6)");
            }
         }
      }

   printf("Pid %d checked links; time = %.1f seconds.\n",pid,endtime());

/**************************************/

   if(debug) printf("Cleaning up '%s'.\n",wd);
   if(rf=chdir(wd)) errex("chdir(4)");
   cnt=0; starttime();

   for(j=1; j<=nr_dirs; ++j)
      {
      sprintf(fn,"dir.%d.%d",pid,j);
      if(rf=chdir(fn)) errex("chdir(5)");
      for(k=1; k<=nr_subf; ++k)
         {
         sprintf(fn,"subf.%d.%d",j,k);
         if(rf=unlink(fn)) errex("unlink(1)"); else ++cnt;
         }
      if(rf=chdir("..")) errex("chdir(6)");
      sprintf(fn,"dir.%d.%d",pid,j);
      if(rf=rmdir(fn)) errex("rmdir(1)");
      }
   for(j=1; j<=nr_files; ++j)
      {
      sprintf(fn,"file.%d.%d",pid,j);
      if(rf=unlink(fn)) errex("unlink(2)"); else ++cnt;
      }

   if(debug) printf("Cleaning up '%s'.\n",dir);
   if(rf=chdir(dir)) errex("chdir(7)");

   for(j=1; j<=nr_files; ++j)
      {
      for(k=1; k<=nr_hard; ++k)
         {
         sprintf(fn,"hf.%d.%d.%d",pid2,j,k);
         if(rf=unlink(fn)) errex("unlink(3)"); else ++cnt;
         }
      for(k=1; k<=nr_sym; ++k)
         {
         sprintf(fn,"sf.%d.%d.%d",pid2,j,k);
         if(rf=unlink(fn)) errex("unlink(4)"); else ++cnt;
         }
      for(k=1; k<=nr_ne_sym; ++k)
         {
         sprintf(fn,"sF.%d.%d.%d",pid2,j,k);
         if(rf=unlink(fn)) errex("unlink(5)"); else ++cnt;
         }
      }

   for(j=1; j<=nr_dirs; ++j)
      {
      for(k=1; k<=nr_sym; ++k)
         {
         sprintf(fn,"sd.%d.%d.%d",pid2,j,k);
         if(rf=unlink(fn)) errex("unlink(6)"); else ++cnt;
         }
      for(k=1; k<=nr_ne_sym; ++k)
         {
         sprintf(fn,"sD.%d.%d.%d",pid2,j,k);
         if(rf=unlink(fn)) errex("unlink(7)"); else ++cnt;
         }
      for(k=1; k<=nr_subf; ++k)
         {
         for(l=1; l<=nr_hard; ++l)
            {
            sprintf(fn,"hs.%d.%d.%d.%d",pid2,j,k,l);
            if(rf=unlink(fn)) errex("unlink(8)"); else ++cnt;
            }
         for(l=1; l<=nr_sym; ++l)
            {
            sprintf(fn,"ss.%d.%d.%d.%d",pid2,j,k,l);
            if(rf=unlink(fn)) errex("unlink(9)"); else ++cnt;
            }
         for(l=1; l<=nr_ne_sym; ++l)
            {
            sprintf(fn,"sS.%d.%d.%d.%d",pid2,j,k,l);
            if(rf=unlink(fn)) errex("unlink(10)"); else ++cnt;
            }
         }
      }

   printf("Pid %d deleted %d links; time = %.1f seconds.\n",pid,cnt,endtime());

/**************************************/

   if(rf=chdir("..")) errex("chdir(8)");
   }

if(umask(um) != 0) errex("umask");
printf("Prog7 pid %d exiting.\n",pid);
exit(0);
}

/*****************************************************************************/

int st_compare(s1,s2)
struct stat s1,s2;
{
return(
   (s1.st_dev == s2.st_dev) &&
   (s1.st_ino == s2.st_ino) &&
   (s1.st_mode == s2.st_mode) &&
   (s1.st_nlink == s2.st_nlink) &&
   (s1.st_uid == s2.st_uid) &&
   (s1.st_gid == s2.st_gid) &&
   (s1.st_rdev == s2.st_rdev) &&
   (s1.st_size == s2.st_size) &&
   (s1.st_mtime == s2.st_mtime) &&
   (s1.st_ctime == s2.st_ctime) &&
   (s1.st_blksize == s2.st_blksize) &&
   (s1.st_blocks == s2.st_blocks)
   );
}

/*****************************************************************************/
void errex(s) char *s;
{
        FILE *dumpfilep;
        char dumpfile[256];

        printf("\n%s failed; errno = %d\n",s,errno);
        if (debug) {
            sprintf(dumpfile,"/tmp/low.prog7.kernel.dump.%d",getpid());
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
            sprintf(dumpfile,"/tmp/low.prog7.kernel.dump.%d",getpid());
            dumpfilep = fopen(dumpfile, "w");
            icl_DumpKernel(dumpfilep, NULL, ICL_DUMP_FORMATTED);
            fclose(dumpfilep);
        }
        exit(1);
}


/*****************************************************************************/

void starttime() {
#if	defined(_SVID_GETTOD)
    gettimeofday(&tv_cur);
#else /* _SVID_GETTOD */
    gettimeofday(&tv_cur,&tz);
#endif /* _SVID_GETTOD */
}

double endtime() {
    long nsec,nusec;
    tv_prev = tv_cur;

#if	defined(_SVID_GETTOD)
    gettimeofday(&tv_cur);
#else /* _SVID_GETTOD */
    gettimeofday(&tv_cur,&tz);
#endif /* _SVID_GETTOD */

    nsec = tv_cur.tv_sec - tv_prev.tv_sec;
    nusec = tv_cur.tv_usec - tv_prev.tv_usec;
    return((double)nsec+(double)nusec/1000000.);
}

/*****************************************************************************/
