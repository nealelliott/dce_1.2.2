/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: prog3a.c,v $
 * Revision 1.1.126.1  1996/10/17  18:29:23  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:54:46  damon]
 *
 * Revision 1.1.121.1  1994/02/04  20:46:56  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:25:17  devsrc]
 * 
 * Revision 1.1.119.1  1993/12/07  17:45:58  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:36:53  jaffe]
 * 
 * Revision 1.1.10.2  1993/07/19  19:44:00  zeliff
 * 	HP port of DFS
 * 	[1993/07/19  18:33:05  zeliff]
 * 
 * Revision 1.1.8.3  1993/07/16  22:13:39  kissel
 * 	Get rid of the damn quotes in #include.
 * 	[1993/06/21  15:09:52  kissel]
 * 
 * Revision 1.1.5.5  1993/03/09  21:22:21  jaffe
 * 	Transarc delta: jess-ot7194-fs-low-kerneldump 1.1
 * 	  Selected comments:
 * 	    Change the icl_KernelDump parameter according to the changes
 * 	    from icl utilities.
 * 	    See above and also fix some compiler warnings.
 * 	[1993/03/09  20:51:43  jaffe]
 * 
 * Revision 1.1.5.4  1993/01/11  18:33:14  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:15:05  htf]
 * 
 * Revision 1.1.5.3  1992/11/24  21:45:49  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:45:45  bolinger]
 * 
 * Revision 1.1.5.2  1992/10/28  18:12:51  jaffe
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
 * 	[1992/10/28  14:31:47  jaffe]
 * 
 * Revision 1.1.3.4  1992/05/21  12:46:43  jaffe
 * 	Transarc delta: fred-merge-tests 1.1
 * 	  Selected comments:
 * 	    Created this delta to merge Phil Hirsch's final DFS test code changes with
 * 	    the test code that the OSF had.  Hopefully, this means we and the OSF
 * 	    should be looking at the same test code.
 * 	    Changes to facilitate merge of local DFS test code with the OSF's.
 * 	[1992/05/20  20:36:03  jaffe]
 * 
 * Revision 1.1.3.3  1992/01/24  04:15:25  devsrc
 * 	Merging dfs6.3
 * 	[1992/01/24  00:23:40  devsrc]
 * 
 * Revision 1.1.3.2  1992/01/23  20:31:12  zeliff
 * 	Moving files onto the branch for dfs6.3 merge
 * 	[1992/01/23  18:48:25  zeliff]
 * 	Revision 1.1.1.3  1992/01/23  22:26:19  devsrc
 * 	Fixed logs
 * 
 * $EndLog$
*/
/*****************************************************************************/
/* Copyright (C) 1990, 1991 Transarc Corporation - All rights reserved */
/*****************************************************************************/

#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <common.h>
#ifdef __OSF__
#include <sys/mount.h>
#else 
#include <sys/vfs.h>
#endif

#ifdef NEXT
#include <sys/fcntl.h>
#else
#include <fcntl.h>
#endif

#if defined(_AIX) || defined(sun)
#include <sys/statfs.h>
#endif

#ifdef HP
#include <sys/vfs.h>
#endif

#ifdef MIPS
#define DEC
#include <sys/param.h>
#include <sys/mount.h>
#endif

#ifdef NEXT
#include <sys/vfs.h>
#endif


#ifdef SPARC
#include <sys/vfs.h>
#define RWA3              int
#endif

#ifdef SUN3
#include <sys/vfs.h>
#endif

#ifdef VAX
#define DEC
#include <sys/param.h>
#include <sys/mount.h>
#endif
#include <dcedfs/icl.h>
#include <string.h>


#define MAXBUF        8192

char *file="./tfile.3";
unsigned char rbuf[MAXBUF],wbuf[MAXBUF];
/* All bsize[] entries must be MAXBUF / (some power of 2) */
int bsize[] = {512, 8192};
int debug=TRUE;
long filesize,offset[] = {0L, 1L};
long inc,nr_blocks;
struct timeval tv_prev,tv_cur;
struct timezone tz;

double endtime();
long randblock();
void errex(), errex0(), starttime();

extern int errno;

/*****************************************************************************/

main(argc,argv) int argc; char *argv[];
{
char z[256];
double dz,etime,ns_etime;
int afsflag,b,bs,dfsflag,fd,i,o,rf,sf;
long j,wb;
off_t of,pos;
struct stat s;
#ifndef VAX
struct statfs fs;
#else
struct fs_data fs;
#endif

setbuf(stdout,(char *)NULL);

if(argc < 2) {j = 1;}
else {j = atol(argv[1]);}
if(j < 100) j = 100;

/* Force j*1024 to be (2^n) * MAXBUF */
for(dz=(double)(j*1024)/MAXBUF,j=1; dz>1.; dz/=2.,j*=2) ;
filesize = j * MAXBUF;
for(i=0; i<MAXBUF; ++i) wbuf[i] = i%256;

printf("\nWrite test: filesize %ld.\n",filesize);

(void)getwd(z);
afsflag = (strncmp(z,"/afs/",5) == 0);
dfsflag = (strncmp(z,"/.../",5) == 0);

printf("This is%s an AFS filesystem.\n",afsflag? "":" not");
printf("This is%s a DFS filesystem.\n",dfsflag? "":" not");
printf("\n");

printf("Sequential write\n");
for(o=0; o<sizeof(offset)/sizeof(long); ++o)
   {
   for(b=0; b<sizeof(bsize)/sizeof(int); ++b)
      {
      for(sf=0; sf<=2; ++sf) /* synchronous-write flag */
         {
         /* do sync-writes first time only */
         if(sf && b) continue;

#ifdef NEXT
if(sf) continue; /* can't do synchronous writes on NeXT */
#endif

#ifdef RT
if(sf) continue; /* can't do synchronous writes on an RT */
#endif

         if(sf && afsflag)
            {printf(">>> Skipping sync-write test in AFS. <<<\n"); continue;}
         if(sf && dfsflag)
            {printf(">>> Skipping sync-write test in DFS. <<<\n"); continue;}

         etime = 0.; bs = bsize[b]; of = offset[o];
         nr_blocks = filesize/bs;
         printf("Offset %ld, blocksize %d%s\n",
            (long)of,bs,
            sf? (sf==1? ", open() sync-write":", fcntl() sync-write"):"");
         starttime();

         if((fd=open(file,O_WRONLY|O_CREAT|O_TRUNC|(sf==1? O_SYNC:0),0644)) < 0)
            errex("open(wr)");
         if(sf == 2) {if(fcntl(fd,F_SETFL,O_SYNC) == -1) errex("fcntl(SETFL)");}

         if((rf=fcntl(fd,F_GETFL,0)) == -1) errex("fcntl(GETFL)");
#ifndef NEXT
#ifndef RT
         if(sf && !(rf & O_SYNC)) errex("O_SYNC (not set)");
         if(!sf && (rf & O_SYNC)) errex("O_SYNC (is set)");
#endif
#endif

         if(lseek(fd,of,0) != of) errex("lseek(wr)");
         for(j=0,wb=0; j<nr_blocks; ++j)
            {if(write(fd,wbuf,(RWA3)bs) != bs) errex("write"); else wb+=bs;}
         if(fsync(fd)) errex("fsync(wr)");
         if(close(fd)) errex("close(wr)");
         etime = endtime(); if(!sf) ns_etime = etime;

         printf("   Wrote %ld bytes %sin %.2f seconds\n",
            filesize,sf? "synchronously ":"",etime);
         printf("   Throughput = %.1f bytes per second\n",filesize/etime);
         if(stat(file,&s)) errex("stat(1)");
         else if(s.st_size != of+filesize) errex0("st_size check(1)");

         if((fd=open(file,O_RDONLY)) < 0) errex("open(rd)");
         if(lseek(fd,of,0) != of) errex("lseek(rd)");
         for(j=0; j<nr_blocks; ++j)
            {
            bzero((char *)rbuf,bs);
            if(read(fd,rbuf,(RWA3)bs) < 0) errex("read");
            if(bcmp((char *)rbuf,(char *)wbuf,bs) != 0)
               {sprintf(z,"compare (block %d of %ld)",j,nr_blocks); errex0(z);}
            }
         if(close(fd)) errex("close(rd)");
         if(debug) printf("   Data checked OK\n");
         if(stat(file,&s)) errex("stat(2)");
         else if(s.st_size != of+filesize) errex0("st_size check(2)");

         if(sf && etime<2*ns_etime) errex0("sync write etime check");
         }
      }
   }

printf("Done writing sequentially.\n\n");

printf("Random write\n");
for(o=0; o<sizeof(offset)/sizeof(long); ++o)
   {
   for(b=0; b<sizeof(bsize)/sizeof(int); ++b)
      {
      etime = 0.; bs = bsize[b]; of = offset[o];
      nr_blocks = filesize/bs;
      inc = nr_blocks/2 - 1;
      printf("Offset %ld, blocksize %d\n",(long)of,bs);
      (void)unlink(file);
      starttime();
      if((fd=open(file,O_WRONLY|O_CREAT,0644)) < 0)
         errex("open(wr)");
      for(j=0; j<nr_blocks; ++j)
         {
         pos = bs*randblock() + of;
         if(lseek(fd,pos,0) != pos) errex("lseek(rand)");
         if(write(fd,wbuf,(RWA3)bs) != bs) errex("write");
         }
      if(fsync(fd)) errex("fsync(wr)");
      if(close(fd)) errex("close(wr)");
      etime = endtime();
      if(stat(file,&s)) errex("stat(3)");
      else if(s.st_size != of+filesize) errex0("st_size check(3)");

      printf("   Wrote %ld bytes in %.2f seconds\n",filesize,etime);
      printf("   Throughput = %.1f bytes per second\n",filesize/etime);

      if((fd=open(file,O_RDONLY)) < 0) errex("open(rd)");
      if(lseek(fd,of,0) != of) errex("lseek(rd)");
      for(j=0; j<nr_blocks; ++j)
         {
         bzero((char *)rbuf,bs);
         if(read(fd,rbuf,(RWA3)bs) < 0) errex("read");
         if(bcmp((char *)rbuf,(char *)wbuf,bs) != 0)
            {sprintf(z,"compare (block %d of %ld)",j,nr_blocks); errex0(z);}
         }
      if(close(fd)) errex("close(rd)");
      if(debug) printf("   Data checked OK\n");
      if(stat(file,&s)) errex("stat(4)");
      else if(s.st_size != of+filesize) errex0("st_size check(4)");
      }
   }

printf("Done writing randomly.\n\n");
exit(0);
}

/*****************************************************************************/
void errex(s) char *s;
{
        FILE *dumpfilep;
        char dumpfile[256];

        printf("\n%s failed; errno = %d\n",s,errno);
        if (debug) {
            sprintf(dumpfile,"/tmp/low.prog3a.kernel.dump.%d",getpid());
            dumpfilep = fopen(dumpfile, "w");
            icl_DumpKernel(dumpfilep, NULL, ICL_DUMP_FORMATTED);
            fclose(dumpfilep);
        }
        exit(1);
}

/* don't print the msg with errno  */

void errex0(s) char *s;
{
        FILE *dumpfilep;
        char dumpfile[256];

        printf("\n%s failed\n", s);
        if (debug) {
            sprintf(dumpfile,"/tmp/low.prog3a.kernel.dump.%d",getpid());
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

long randblock()
{
static long b = 0;
return(b = ((b + inc) % nr_blocks));
}

/*****************************************************************************/
