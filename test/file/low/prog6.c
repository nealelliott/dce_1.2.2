/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: prog6.c,v $
 * Revision 1.1.120.1  1996/10/17  18:29:29  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:54:50  damon]
 *
 * Revision 1.1.115.1  1994/02/04  20:47:03  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:25:20  devsrc]
 * 
 * Revision 1.1.113.1  1993/12/07  17:46:03  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:38:05  jaffe]
 * 
 * Revision 1.1.10.2  1993/07/19  19:44:22  zeliff
 * 	HP port of DFS
 * 	[1993/07/19  18:33:20  zeliff]
 * 
 * Revision 1.1.8.3  1993/07/16  22:13:55  kissel
 * 	Get rid of the damn quotes in #include.
 * 	[1993/06/21  15:11:02  kissel]
 * 
 * Revision 1.1.5.6  1993/03/09  21:22:50  jaffe
 * 	Transarc delta: jess-ot7194-fs-low-kerneldump 1.1
 * 	  Selected comments:
 * 	    Change the icl_KernelDump parameter according to the changes
 * 	    from icl utilities.
 * 	    See above.
 * 	[1993/03/09  20:53:53  jaffe]
 * 
 * Revision 1.1.5.5  1993/01/11  18:33:25  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:15:33  htf]
 * 
 * Revision 1.1.5.4  1992/12/09  21:33:28  jaffe
 * 	Transarc delta: jaffe-fix-2.2-test-build 1.2
 * 	  Selected comments:
 * 
 * 	    use pid_t for pid and getpid.
 * 	[1992/12/07  13:38:05  jaffe]
 * 
 * Revision 1.1.5.3  1992/11/24  21:46:08  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:46:11  bolinger]
 * 
 * Revision 1.1.5.2  1992/10/28  18:13:18  jaffe
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
 * 	[1992/10/28  14:33:37  jaffe]
 * 
 * Revision 1.1.3.3  1992/01/24  04:15:33  devsrc
 * 	Merging dfs6.3
 * 	[1992/01/24  00:23:54  devsrc]
 * 
 * Revision 1.1.3.2  1992/01/23  20:31:32  zeliff
 * 	Moving files onto the branch for dfs6.3 merge
 * 	[1992/01/23  18:48:42  zeliff]
 * 	Revision 1.1.1.3  1992/01/23  22:26:32  devsrc
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



#ifdef RT
#include <sys/dir.h>
#define dirent direct
#else
#ifdef NEXT
#include <sys/dir.h>
#include <sys/dirent.h>
#define dirent direct
#else
#include <dirent.h>
#endif
#endif

#define MAX_DIRS   100
#define MAX_FILES  100

int debug=TRUE;
int nr_times,nr_levels,nr_files,nr_dirs,rf=0;
pid_t pid;
struct timeval tv_prev,tv_cur;
struct timezone tz;

double endtime();
int make_dirs(),trav_dirs(),zap_dirs();
void errex(), errex0(), starttime();

extern int errno;
extern pid_t getpid();

/*****************************************************************************/

main(argc,argv) int argc; char *argv[];
{
int i;

setbuf(stdout,(char *)NULL);

if(argc < 5) {nr_times = nr_levels = nr_files = nr_dirs = 1;}
else
   {
   nr_times = atoi(argv[1]);
   nr_levels = atoi(argv[2]);
   nr_files = atoi(argv[3]);
   nr_dirs = atoi(argv[4]);
   }
if(nr_files > MAX_FILES) errex0("main: nr-files");
if(nr_dirs > MAX_DIRS) errex0("main: nr-dirs");
pid = getpid();


printf("Prog6 pid %d, nr_times %d, nr_levels %d, nr_files %d, nr_dirs %d.\n",
   pid,nr_times,nr_levels,nr_files,nr_dirs);

for(i=1; i<=nr_times || nr_times==0; ++i)
   {
   starttime();
   if(!make_dirs(nr_levels)) 
     {
	printf("Prog6 pid %d exit code %d.\n",pid,rf);
        errex0("make_dir");
     }
   printf("Pid %d made directories; time = %.1f seconds.\n",pid,endtime());

   starttime();
   if(!trav_dirs(nr_levels))
     {
	printf("Prog6 pid %d exit code %d.\n",pid,rf);
       	errex0("make_dir"); 
      }
   printf("Pid %d traversed directories; time = %.1f seconds.\n",pid,endtime());

   starttime();
   if(!zap_dirs(nr_levels))
     {
	printf("Prog6 pid %d exit code %d.\n",pid,rf);
       	errex0("make_dir"); 
      }
   printf("Pid %d deleted directories; time = %.1f seconds.\n",pid,endtime());
   }

printf("Prog6 pid %d exit code %d.\n",pid,rf);
exit(rf);
}

/**************************************/

int make_dirs(n) int n; /* return TRUE => OK, FALSE => error */
{
char fn[21],subd[256],wd[256],z[256];
int fd,i,l,rf1;

if(n <= 0) return(TRUE);
else (void)getwd(wd);
if(debug) printf("\nThis is prog6, pid %d, in make_dirs(%d).\n",pid,n);

for(i=1; i<=nr_files; ++i)
   {
   sprintf(fn,"file.%d.%d",pid,i);
   if((fd=open(fn,O_WRONLY|O_CREAT,0644)) < 0)
      {
      printf("- ERROR: make_dirs: open(%s) returned %d (errno %d)\n",
         fn,fd,errno);
      rf = 1; goto OUT1;
      }
   sprintf(z,"This is %s.",fn); l=strlen(z);
   if(write(fd,z,(RWA3)(l+1)) != l+1) errex("make_dirs: write");
   if(close(fd)) errex("make_dirs: close");
   }

for(i=1; i<=nr_dirs; ++i)
   {
   sprintf(fn,"dir.%d.%d",pid,i);
   sprintf(z,"%s/%s",wd,fn);
   if(rf1=mkdir(fn,0777))
      {
      printf("- ERROR: make_dirs: mkdir(%s) returned %d (errno %d)\n",
         fn,rf1,errno);
      rf = 1; goto OUT1;
      }
   if(rf1=chdir(fn))
      {
      printf("- ERROR: make_dirs: chdir(%s) returned %d (errno %d)\n",
         fn,rf1,errno);
      rf = 1; goto OUT1;
      }
   (void)getwd(subd);
   if(!EQ(subd,z))
      {
      printf("- ERROR: make_dirs: getwd(1) got '%s', not '%s'\n",subd,z);
      rf = 1; goto OUT1;
      }

   if(!make_dirs(n-1)) goto OUT1;

   (void)getwd(subd);
   if(!EQ(subd,z))
      {
      printf("- ERROR: make_dirs: getwd(2) got '%s', not '%s'\n",subd,z);
      rf = 1; goto OUT1;
      }
   if(rf1=chdir(".."))
      {
      printf("- ERROR: make_dirs: chdir(..) returned %d (errno %d)\n",
         rf1,errno);
      rf = 1; goto OUT1;
      }
   (void)getwd(z);
   if(!EQ(z,wd))
      {
      printf("- ERROR: make_dirs: getwd(3) got '%s', not '%s'\n",z,wd);
      rf = 1; goto OUT1;
      }
   }

OUT1:
   if(debug) printf("   Exit prog6, pid %d, make_dirs(%d); rf = %d.\n",pid,n,rf);
   if (rf) errex0("cleanup check");
   return(!rf);
}

/**************************************/

int trav_dirs(n) int n;
{
char dt[21],ft[21],*p,subd[256],wd[256],z[256];
int d[MAX_DIRS],f[MAX_FILES],fflag,i,nd=0,nf=0,rf1,saw1=FALSE,saw2=FALSE;
struct stat s;
struct dirent *direp;
DIR *dp;

if(n <= 0) return(TRUE);
else (void)getcwd(wd,sizeof(wd)-1);
if(debug) printf("\nThis is prog6, pid %d, in trav_dirs(%d).\n",pid,n);

for(i=0; i<MAX_DIRS; ++i) d[i]=0;
for(i=0; i<MAX_FILES; ++i) f[i]=0;

if(!(dp=opendir(".")))
   {
   printf("- ERROR: trav_dirs: opendir(%s) failed (errno %d)\n",wd,errno);
   rf = 1; goto OUT1;
   }

sprintf(ft,"file.%d.",pid);
sprintf(dt,"dir.%d.",pid);

while(direp = readdir(dp))
   {
   p = direp->d_name;
   if(EQ(p,".") && !saw1) {saw1=TRUE; continue;}
   else if(EQ(p,"..") && !saw2) {saw2=TRUE; continue;}

   if((rf1=stat(p,&s)) != 0)
      {
      printf("- ERROR: trav_dirs: stat(%s) returned %d (errno %d)\n",
         p,rf1,errno);
      rf = 1; goto OUT1;
      }
   
   if(strncmp(p,ft,strlen(ft)) == 0)
      {fflag = TRUE; ++nf; ++f[atoi(p+strlen(ft))];}
   else if(strncmp(p,dt,strlen(dt)) == 0)
      {fflag = FALSE; ++nd; ++d[atoi(p+strlen(dt))];}
   else
      {
      printf("- ERROR: trav_dirs: unrecognized filename '%s'\n",p);
      rf = 1; goto OUT1;
      }
   if(fflag) continue;

   sprintf(z,"%s/%s",wd,p);
   if(rf1=chdir(p))
      {
      printf("- ERROR: trav_dirs: chdir(%s) returned %d (errno %d)\n",
         p,rf1,errno);
      rf = 1; goto OUT1;
      }
   (void)getcwd(subd,sizeof(subd)-1);
   if(!EQ(subd,z))
      {
      printf("- ERROR: trav_dirs: getcwd(1) got '%s', not '%s'\n",subd,z);
      rf = 1; goto OUT1;
      }

   if(!trav_dirs(n-1)) goto OUT1;

   (void)getcwd(subd,sizeof(subd)-1);
   if(!EQ(subd,z))
      {
      printf("- ERROR: trav_dirs: getcwd(2) got '%s', not '%s'\n",subd,z);
      rf = 1; goto OUT1;
      }
   if(rf1=chdir(".."))
      {
      printf("- ERROR: trav_dirs: chdir(..) returned %d (errno %d)\n",
         rf1,errno);
      rf = 1; goto OUT1;
      }
   (void)getcwd(z,sizeof(z)-1);
   if(!EQ(z,wd))
      {
      printf("- ERROR: trav_dirs: getcwd(3) got '%s', not '%s'\n",z,wd);
      rf = 1; goto OUT1;
      }
   }

if(!saw1) {printf("- ERROR: trav_dirs: didn't see '.'\n"); rf = 1; goto OUT1;}
if(!saw2) {printf("- ERROR: trav_dirs: didn't see '..'\n"); rf = 1; goto OUT1;}

if(nd != nr_dirs)
   {
   printf("- ERROR: trav_dirs: saw %d directories, not %d\n",nd,nr_dirs);
   rf = 1; goto OUT1;
   }
if(nf != nr_files)
   {
   printf("- ERROR: trav_dirs: saw %d files, not %d\n",nf,nr_files);
   rf = 1; goto OUT1;
   }

for(i=1; i<=nd; ++i)
   {
   if(d[i] != 1)
      {
      printf("- ERROR: trav_dirs: d[%d] = %d, not 1\n",i,d[i]);
      rf = 1; goto OUT1;
      }
   }

for(i=1; i<=nf; ++i)
   {
   if(f[i] != 1)
      {
      printf("- ERROR: trav_dirs: f[%d] = %d, not 1\n",i,f[i]);
      rf = 1; goto OUT1;
      }
   }

(void)closedir(dp);

OUT1:
if(debug) printf("   Exit prog6, pid %d, trav_dirs(%d); rf = %d.\n",pid,n,rf);
return(!rf);
}

/**************************************/

int zap_dirs(n) int n;
{
char fn[21],subd[256],wd[256],z[256],z1[81];
int fd,i,rf1;

if(n <= 0) return(TRUE);
else (void)getwd(wd);
if(debug) printf("\nThis is prog6, pid %d, in zap_dirs(%d).\n",pid,n);

for(i=1; i<=nr_files; ++i)
   {
   sprintf(fn,"file.%d.%d",pid,i);
   if((fd=open(fn,O_RDONLY)) < 0)
      {
      printf("- ERROR: zap_dirs: open(%s) returned %d (errno %d)\n",
         fn,fd,errno);
      rf = 1; goto OUT1;
      }
   sprintf(z,"This is %s.",fn);
   if(read(fd,z1,(RWA3)sizeof(z1)) != strlen(z)+1)
      {
      printf("- ERROR: zap_dirs: read(%s) got '%s', not '%s'\n",fn,z1,z);
      rf = 1; goto OUT1;
      }
   if(close(fd)) errex("zap_dirs: close");
   if(!EQ(z,z1))
      {
      printf("- ERROR: zap_dirs: read(%s) got '%s', not '%s'\n",fn,z1,z);
      rf = 1; goto OUT1;
      }
   if((rf1=unlink(fn)))
      {
      printf("- ERROR: zap_dirs: unlink(%s) returned %d (errno %d)\n",
         fn,rf1,errno);
      rf = 1; goto OUT1;
      }
   }

for(i=1; i<=nr_dirs; ++i)
   {
   sprintf(fn,"dir.%d.%d",pid,i);
   sprintf(z,"%s/%s",wd,fn);

   if(rf1=chdir(fn))
      {
      printf("- ERROR: zap_dirs: chdir(%s) returned %d (errno %d)\n",
         fn,rf1,errno);
      rf = 1; goto OUT1;
      }
   (void)getwd(subd);
   if(!EQ(subd,z))
      {
      printf("- ERROR: zap_dirs: getwd(1) got '%s', not '%s'\n",subd,z);
      rf = 1; goto OUT1;
      }

   if(!zap_dirs(n-1)) goto OUT1;

   (void)getwd(subd);
   if(!EQ(subd,z))
      {
      printf("- ERROR: zap_dirs: getwd(2) got '%s', not '%s'\n",subd,z);
      rf = 1; goto OUT1;
      }
   if(rf1=chdir(".."))
      {
      printf("- ERROR: zap_dirs: chdir(..) returned %d (errno %d)\n",rf1,errno);
      rf = 1; goto OUT1;
      }
   (void)getwd(z);
   if(!EQ(z,wd))
      {
      printf("- ERROR: zap_dirs: getwd(3) got '%s', not '%s'\n",z,wd);
      rf = 1; goto OUT1;
      }

   if((rf1=rmdir(fn)) != 0)
      {
      printf("- ERROR: zap_dirs: rmdir(%s) returned %d (errno %d)\n",
         fn,rf1,errno);
      rf = 1; goto OUT1;
      }
   }

OUT1:
if(debug) printf("   Exit prog6, pid %d, zap_dirs(%d); rf = %d.\n",pid,n,rf);
return(!rf);
}

/*****************************************************************************/

void errex(s) char *s;
{
        FILE *dumpfilep;
        char dumpfile[256];

        printf("\n%s failed; errno = %d\n",s,errno);
        if (debug) {
            sprintf(dumpfile,"/tmp/low.prog6.kernel.dump.%d",getpid());
            dumpfilep = fopen(dumpfile, "w");
            icl_DumpKernel(dumpfilep, NULL, ICL_DUMP_FORMATTED);
            fclose(dumpfilep);
        }
        exit(1);
}

/* No errno printed in msg */
void errex0(s) char *s;
{
        FILE *dumpfilep;
        char dumpfile[256];

        printf("\n%s failed\n",s);
        if (debug) {
            sprintf(dumpfile,"/tmp/low.prog6.kernel.dump.%d",getpid());
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
