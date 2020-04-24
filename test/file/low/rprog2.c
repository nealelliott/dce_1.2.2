/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rprog2.c,v $
 * Revision 1.1.155.1  1996/10/17  18:29:44  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:54:56  damon]
 *
 * Revision 1.1.150.1  1994/02/04  20:47:15  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:25:27  devsrc]
 * 
 * Revision 1.1.148.1  1993/12/07  17:46:14  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:39:48  jaffe]
 * 
 * Revision 1.1.13.2  1993/07/19  19:44:58  zeliff
 * 	HP port of DFS
 * 	[1993/07/19  18:33:42  zeliff]
 * 
 * Revision 1.1.11.3  1993/07/16  22:14:24  kissel
 * 	Put HP RP work on the 1.0.3i branch.
 * 	[1993/06/21  15:12:55  kissel]
 * 
 * Revision 1.1.9.2  1993/06/04  18:36:36  kissel
 * 	Initial HPUX RP version.
 * 	[1993/06/03  22:47:08  kissel]
 * 
 * Revision 1.1.5.5  1993/03/09  21:23:54  jaffe
 * 	Transarc delta: jess-ot7194-fs-low-kerneldump 1.1
 * 	  Selected comments:
 * 	    Change the icl_KernelDump parameter according to the changes
 * 	    from icl utilities.
 * 	    See above.
 * 	[1993/03/09  20:58:40  jaffe]
 * 
 * Revision 1.1.5.2  1993/02/05  21:26:37  maunsell_c
 * 	hpux changes
 * 	[1993/02/05  21:25:57  maunsell_c]
 * 
 * Revision 1.1.3.2  1993/02/05  17:17:29  maunsell_c
 * 	01/27/93	maunsell_c	gid_t uid_t declares need __hpux ifndef
 * 
 * Revision 1.1.5.4  1993/01/11  18:33:43  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:16:21  htf]
 * 
 * Revision 1.1.5.3  1992/11/24  21:46:43  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:46:54  bolinger]
 * 
 * Revision 1.1.5.2  1992/10/28  18:14:03  jaffe
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
 * 	[1992/10/28  14:37:27  jaffe]
 * 
 * Revision 1.1.3.5  1992/06/19  17:19:16  jaffe
 * 	Transarc delta: rajesh-ot4304-low-rtest2 1.1
 * 	  Selected comments:
 * 	    low rprog2.c assumes that gid of files and dirs is the gid of the
 * 	    creating process. On OSF1 the group id of created files and dirs are
 * 	    that of the parent dir and not the process's group id. Fix the tests
 * 	    to operate correctly for OSF1 platforms.
 * 	[1992/06/19  17:17:19  jaffe]
 * 
 * Revision 1.1.3.4  1992/05/21  12:47:06  jaffe
 * 	Transarc delta: rajesh-low-rtest2-passuidgid 1.1
 * 	  Selected comments:
 * 	    To modify the rtest2 program so that rprog2.c does not need to be linked
 * 	    with the libdce.a so that the same cc command can be used both under DFS and
 * 	    AFS. Done by determining the uid and gid in the rtest2 shellscript and
 * 	    passing it to rprog2.c, instead of letting rprog2.c using DCE security API
 * 	    calls in DCE/DFS and system calls in AFS to do it.
 * 	    This now receives uid and gid of the user from the driver shell script.
 * 	Transarc delta: rajesh-low-tests-dfs-update 1.3
 * 	  Selected comments:
 * 	    To fix bugs and modify the low tests for DFS.
 * 	    To use the DCE security API calls to determine user's uid and gid
 * 	    when running under DFS.
 * 	    Change compile flag DFS to TR_DFS for rprog2.c
 * 	    Change compile flag DFS to TR_DFS
 * 	    DCE security API call fixes
 * 	[1992/05/20  20:41:18  jaffe]
 * 
 * Revision 1.1.3.3  1992/01/24  04:15:44  devsrc
 * 	Merging dfs6.3
 * 	[1992/01/24  00:24:11  devsrc]
 * 
 * Revision 1.1.3.2  1992/01/23  20:32:04  zeliff
 * 	Moving files onto the branch for dfs6.3 merge
 * 	[1992/01/23  18:49:07  zeliff]
 * 	Revision 1.1.1.3  1992/01/23  22:26:49  devsrc
 * 	Fixed logs
 * 
 * $EndLog$
*/
/*****************************************************************************/
/* Copyright (C) 1990 Transarc Corporation - All rights reserved */
/*****************************************************************************/


/*
 * The program must be invoked with 2 arguments.
 * 
 * Parameter 1 = User ID of tester
 * Parameter 2 = Group ID of tester
 *
 */

#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "common.h"
#include <dcedfs/icl.h>
#include <string.h>

#ifdef _AIX
#include <sys/chownx.h>
#endif

#if 0
typedef int gid_t
typedef int uid_t
#endif

#define NR_ENTRIES    3
#define USE_CHOWN     0
#define USE_FCHOWN    1

int debug=FALSE, dfsflag;
char *progName;
void check_own(),do_change(),errex(), errex0();

extern int errno;

/*****************************************************************************/

main(argc,argv) int argc; char *argv[];
{
char fn[21],subd[256],wd[256],z[256],z1[256];
int fd,i,l,t;
long rf;
uid_t g,g1,group,o,o1,owner;

#ifdef OSF1
struct stat statBuf;
#endif

setbuf(stdout,(char *)NULL);

progName=argv[0];
if ( argc < 2 ) {
    printf("Usage: %s uid gid \n", progName);
    exit(1);
}


printf("Change-owner tests.\n");

(void)getwd(wd);
dfsflag = (strncmp(wd,"/.../",5) == 0);
for(i=1; i<=NR_ENTRIES; ++i)
   {
   sprintf(fn,"file.%d",i);
   if((fd=open(fn,O_WRONLY|O_CREAT,0644)) < 0) errex("open(1)");
   sprintf(z,"This is %s.",fn); l=strlen(z);
   if((rf=write(fd,z,(RWA3)(l+1))) != l+1) errex("write(1)");
   if((rf=close(fd))) errex("close(1)");
   }

for(i=1; i<=NR_ENTRIES; ++i)
   {
   sprintf(fn,"dir.%d",i);
   if(rf=mkdir(fn,0777)) errex("mkdir(1)");
   }

/**************************************/

printf("Testing chown().\n");


for(t=0; t<=1; ++t)
   {
   for(i=1; i<=NR_ENTRIES; ++i)
      {
      sprintf(fn,"%s.%d",t? "file":"dir",i);
      if(debug) printf("   Testing '%s'.\n",fn);
      owner = atoi(argv[1]);
      /* In OSF1, BSD file creation semantics are always used */
#ifdef OSF1
      if ( stat(".",&statBuf) < 0 ) {
	  errex("rprog2: stat error");
      }
      group=statBuf.st_gid;
#else
      group = atoi(argv[2]);
#endif
      check_own(fn,owner,group);

      for(o=(-1); o<=100; ++o)
         {
         o1 = 99-o;
         for(g=(-1); g<=100; ++g)
            {g1 = 99-g; do_change(USE_CHOWN,fn,-1,o,g,o1,g1,&owner,&group);}
         for(g=100; g<=32000; g+=173)
            {g1 = 32100-g; do_change(USE_CHOWN,fn,-1,o,g,o1,g1,&owner,&group);}
         }

      for(o=100; o<=32000; o+=197)
         {
         o1 = 32100-o;
         for(g=(-1); g<=100; ++g)
            {g1 = 99-g; do_change(USE_CHOWN,fn,-1,o,g,o1,g1,&owner,&group);}
         for(g=100; g<=32000; g+=173)
            {g1 = 32100-g; do_change(USE_CHOWN,fn,-1,o,g,o1,g1,&owner,&group);}
         }

      if(rf=chown(fn,(uid_t)i,(gid_t)i)) errex("chown(1)");
      }
   }

for(i=1; i<=NR_ENTRIES; ++i)
   {
   sprintf(fn,"file.%d",i); check_own(fn,(uid_t)i,(gid_t)i);
   sprintf(fn,"dir.%d",i); check_own(fn,(uid_t)i,(gid_t)i);
   }

printf("Done testing chown().\n");

/**************************************/

printf("Testing fchown().\n");

for(t=0; t<=1; ++t)
   {
   for(i=1; i<=NR_ENTRIES; ++i)
      {
      sprintf(fn,"%s.%d",t? "file":"dir",i);
      if(debug) printf("   Testing '%s'.\n",fn);
      owner =  group = i;
      check_own(fn,owner,group);

      if((fd=open(fn,O_RDONLY)) < 0) errex("open(2)");
      if(t)
         {
         sprintf(z,"This is %s.",fn);
         if(read(fd,z1,(RWA3)sizeof(z1)) < 0) errex("read(1)");
         if(!EQ(z,z1)) errex0("read compare(1)");
         }

      for(o=(-1); o<=100; ++o)
         {
         o1 = 99-o;
         for(g=(-1); g<=100; ++g)
            {
            g1 = 99-g;
            do_change(USE_FCHOWN,fn,fd,o,g,o1,g1,&owner,&group);
            do_change(USE_CHOWN,fn,-1,o,g,o1,g1,&owner,&group);
            }
         for(g=100; g<=32000; g+=173)
            {
            g1 = 32100-g;
            do_change(USE_CHOWN,fn,-1,o,g,o1,g1,&owner,&group);
            do_change(USE_FCHOWN,fn,fd,o,g,o1,g1,&owner,&group);
            }
         }

      for(o=100; o<=32000; o+=197)
         {
         o1 = 32100-o;
         for(g=(-1); g<=100; ++g)
            {
            g1 = 99-g;
            do_change(USE_CHOWN,fn,-1,o,g,o1,g1,&owner,&group);
            do_change(USE_FCHOWN,fn,fd,o,g,o1,g1,&owner,&group);
            }
         for(g=100; g<=32000; g+=173)
            {
            g1 = 32100-g;
            do_change(USE_FCHOWN,fn,fd,o,g,o1,g1,&owner,&group);
            do_change(USE_CHOWN,fn,-1,o,g,o1,g1,&owner,&group);
            }
         }

      if(rf=fchown(fd,(uid_t)i,(gid_t)i)) errex("fchown(1)");
      if(close(fd)) errex("close(2)");
      }
   }

for(i=1; i<=NR_ENTRIES; ++i)
   {
   sprintf(fn,"file.%d",i); check_own(fn,(uid_t)i,(gid_t)i);
   sprintf(fn,"dir.%d",i); check_own(fn,(uid_t)i,(gid_t)i);
   }

printf("Done testing fchown().\n");

/**************************************/

printf("Cleaning up.\n");

for(i=1; i<=NR_ENTRIES; ++i)
   {
   sprintf(fn,"file.%d",i);
   if((fd=open(fn,O_RDONLY)) < 0) errex("open(3)");
   sprintf(z,"This is %s.",fn);
   if((rf=read(fd,z1,(RWA3)sizeof(z1))) < 0) errex("read(2)");
   if((rf=close(fd))) errex("close(3)");
   if(!EQ(z,z1)) errex0("read compare(2)");
   if((rf=unlink(fn)) != 0) errex("unlink(1)");

   sprintf(fn,"dir.%d",i);
   sprintf(z,"%s/%s",wd,fn);
   if(rf=chdir(fn)) errex("chdir(1)");
   (void)getwd(subd);
   if(!EQ(subd,z)) errex0("getwd compare(1)");
   if(rf=chdir("..")) errex("chdir(2)");
   (void)getwd(z);
   if(!EQ(z,wd)) errex0("getwd compare(2)");
   if((rf=rmdir(fn)) != 0) errex("rmdir(1)");
   }

printf("\nDone with change-owner tests.\n");
exit(0);
}

/*****************************************************************************/

void check_own(fn,owner,group)
char *fn;
uid_t owner,group;
{
int rf;
struct stat s;

if(rf=stat(fn,&s)) errex("stat(1)");
if(s.st_uid != owner) errex0("uid check(1)");
if(s.st_gid != group) errex0("gid check(1)");
}

/**************************************/

void do_change(c_flag,fn,fd,o,g,o1,g1,owner,group)
int c_flag,fd;
char *fn;
uid_t o,o1,*owner;
gid_t g,g1,*group;
{
int flagx,rf;

#ifdef _AIX
flagx = 0;
if(o == -1) flagx |= T_OWNER_AS_IS; else *owner = o;
if(g == -1) flagx |= T_GROUP_AS_IS; else *group = g;
if(c_flag == USE_CHOWN) {if(rf=chownx(fn,o,g,flagx)) errex("chownx(1)");}
else {if(rf=fchownx(fd,o,g,flagx)) errex("fchownx(1)");}
check_own(fn,*owner,*group);
#endif

if(o1 != -1) *owner = o1;
if(g1 != -1) *group = g1;
if(c_flag == USE_CHOWN) {if(rf=chown(fn,o1,g1)) errex("chown(2)");}
else {if(rf=fchown(fd,o1,g1)) errex("fchown(2)");}
check_own(fn,*owner,*group);
}

/*****************************************************************************/

void errex(s) char *s;
{
        FILE *dumpfilep;
        char dumpfile[256];

        printf("\n%s failed; errno = %d\n",s,errno);
        if (debug) {
            sprintf(dumpfile,"/tmp/low.rprog2.kernel.dump.%d",getpid());
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
            sprintf(dumpfile,"/tmp/low.rprog2.kernel.dump.%d",getpid());
            dumpfilep = fopen(dumpfile, "w");
            icl_DumpKernel(dumpfilep, NULL, ICL_DUMP_FORMATTED);
            fclose(dumpfilep);
        }
        exit(1);
}


/*****************************************************************************/
