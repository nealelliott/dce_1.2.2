/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: prog3c.c,v $
 * Revision 1.1.128.1  1996/10/17  18:29:26  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:54:48  damon]
 *
 * Revision 1.1.123.1  1994/02/04  20:46:59  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:25:19  devsrc]
 * 
 * Revision 1.1.121.1  1993/12/07  17:46:01  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:37:29  jaffe]
 * 
 * Revision 1.1.11.2  1993/07/19  19:44:11  zeliff
 * 	HP port of DFS
 * 	[1993/07/19  18:33:13  zeliff]
 * 
 * Revision 1.1.9.3  1993/07/16  22:13:47  kissel
 * 	Get rid of the damn quotes in #include.
 * 	[1993/06/21  15:10:25  kissel]
 * 
 * Revision 1.1.5.5  1993/03/09  21:22:40  jaffe
 * 	Transarc delta: jess-ot7194-fs-low-kerneldump 1.1
 * 	  Selected comments:
 * 	    Change the icl_KernelDump parameter according to the changes
 * 	    from icl utilities.
 * 	    See above.
 * 	[1993/03/09  20:52:57  jaffe]
 * 
 * Revision 1.1.5.4  1993/01/11  18:33:19  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:15:21  htf]
 * 
 * Revision 1.1.5.3  1992/11/24  21:45:57  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:45:57  bolinger]
 * 
 * Revision 1.1.5.2  1992/10/28  18:13:05  jaffe
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
 * 	[1992/10/28  14:32:47  jaffe]
 * 
 * Revision 1.1.3.3  1992/01/24  04:15:29  devsrc
 * 	Merging dfs6.3
 * 	[1992/01/24  00:23:47  devsrc]
 * 
 * Revision 1.1.3.2  1992/01/23  20:31:22  zeliff
 * 	Moving files onto the branch for dfs6.3 merge
 * 	[1992/01/23  18:48:34  zeliff]
 * 	Revision 1.1.1.3  1992/01/23  22:26:25  devsrc
 * 	Fixed logs
 * 
 * $EndLog$
*/
/*****************************************************************************/
/* Copyright (C) 1990 Transarc Corporation - All rights reserved */
/*****************************************************************************/

#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <common.h>
#include <dcedfs/icl.h>
#include <string.h>



#define MAXBUF        8192
#define USE_TRUNC     1
#define USE_FTRUNC    2

int can_lengthen=FALSE,closed_can_lengthen=FALSE,use_fclear=FALSE;

char *file="./tfile.3";
unsigned char buf[MAXBUF+1];
int debug=TRUE,fd;
int tsize[] = {0, 1, 511, 512, 513, 1023, 1024, 1025, 8191, 8192, 8193};
    /* this requires that the original filesize be more than about 100KB */
long osize;
struct stat s;

#ifndef _AIX
long fclear(fd,nb) int fd; unsigned long nb; {return(0L);}
#endif

void errex(), errex0(), chop_and_check();

extern int errno;

/*****************************************************************************/

main(argc,argv) int argc; char *argv[];
{
int i;
long fsize;

setbuf(stdout,(char *)NULL);

#ifdef _AIX
can_lengthen = use_fclear = TRUE;
#endif

if(stat(file,&s) != 0) errex("stat(0)"); else fsize=osize=s.st_size;
printf("\nTruncate test: original filesize %ld; lengthening is%s allowed.\n",
   fsize,can_lengthen? "":" not");

if((fd=open(file,O_RDWR)) < 0) errex("open(1)");

for(i=0; i<sizeof(tsize)/sizeof(int); ++i)
   {
   printf("Truncate(), open file, tsize entry %d (%d bytes).\n",i,tsize[i]);
   chop_and_check(1,USE_TRUNC,fsize-tsize[i],&fsize);
   if(can_lengthen)
      {
      chop_and_check(2,USE_TRUNC,fsize+tsize[i],&fsize);
      chop_and_check(3,USE_TRUNC,fsize+tsize[i],&fsize);
      chop_and_check(4,USE_TRUNC,fsize-tsize[i],&fsize);
      }

   printf("Ftruncate(), open file, tsize entry %d (%d bytes).\n",i,tsize[i]);
   chop_and_check(5,USE_FTRUNC,fsize-tsize[i],&fsize);
   if(can_lengthen)
      {
      chop_and_check(6,USE_FTRUNC,fsize+tsize[i],&fsize);
      chop_and_check(7,USE_FTRUNC,fsize+tsize[i],&fsize);
      chop_and_check(8,USE_FTRUNC,fsize-tsize[i],&fsize);
      }
   }

if(close(fd)) errex("close(1)");

for(i=0; i<sizeof(tsize)/sizeof(int); ++i)
   {
   printf("Truncate(), closed file; tsize entry %d (%d bytes).\n",i,tsize[i]);
   chop_and_check(9,USE_TRUNC,fsize-tsize[i],&fsize);
   if(closed_can_lengthen)
      {
      chop_and_check(10,USE_TRUNC,fsize+tsize[i],&fsize);
      chop_and_check(11,USE_TRUNC,fsize+tsize[i],&fsize);
      chop_and_check(12,USE_TRUNC,fsize-tsize[i],&fsize);
      }
   }

for(i=sizeof(tsize)/sizeof(int)-1; i>=0; --i)
   {
   printf("Truncate() to size, closed file, tsize entry %d (%d bytes).\n",
      i,tsize[i]);
   chop_and_check(13,USE_TRUNC,tsize[i],&fsize);
   }

printf("\nDone truncating.\n\n");
exit(0);
}

/*****************************************************************************/
void errex(s) char *s;
{
        FILE *dumpfilep;
        char dumpfile[256];

        printf("\n%s failed; errno = %d\n",s,errno);
        if (debug) {
            sprintf(dumpfile,"/tmp/low.prog3c.kernel.dump.%d",getpid());
            dumpfilep = fopen(dumpfile, "w");
            icl_DumpKernel(dumpfilep, NULL, ICL_DUMP_FORMATTED);
            fclose(dumpfilep);
        }
        exit(1);
}

/* Don't print msg with errno */
void errex0(s) char *s;
{
        FILE *dumpfilep;
        char dumpfile[256];

        printf("\n%s failed\n",s);
        if (debug) {
            sprintf(dumpfile,"/tmp/low.prog3c.kernel.dump.%d",getpid());
            dumpfilep = fopen(dumpfile, "w");
            icl_DumpKernel(dumpfilep, NULL, ICL_DUMP_FORMATTED);
            fclose(dumpfilep);
        }
        exit(1);
}


/*****************************************************************************/

void chop_and_check(seq,type,size,cursize)
int seq; long type,size,*cursize;
{
char *p,z[81];
int expect,fd1,i,lflag,nb;
off_t ofs;

lflag = size > *cursize;
p = (type==USE_FTRUNC)? "ftruncate":"truncate";
if(lflag && use_fclear) p = "fclear";

if(debug)
   printf("%d: %s by %ld bytes, from %ld to %ld bytes.\n",
      seq,p,*cursize-size,*cursize,size);

if(lflag && use_fclear)
   {
   if(lseek(fd,(off_t)0,2) <= 0) errex("lseek(1)");
   if(fclear(fd,(unsigned long)(size - *cursize)) != size - *cursize)
      errex("fclear");
   }
else if(type == USE_FTRUNC)
   {
   if(ftruncate(fd,(off_t)size) != 0)
      {sprintf(z,"ftruncate(%d)",seq); errex(z);}
   }
else
   {
   if(truncate(file,(off_t)size) != 0)
      {sprintf(z,"truncate(%d)",seq); errex(z);}
   }

if(stat(file,&s) != 0) {sprintf(z,"stat(%d)",seq); errex(z);}
if(s.st_size != size)
  {sprintf(z,"stat size (%d; is %ld, not %ld)",seq,s.st_size,size); errex0(z);}
if(size<osize) osize = size;

if(size)
   {
   if((fd1=open(file,O_RDONLY)) < 0) errex("open(2)");

   ofs = (size <= MAXBUF)? 0:size-MAXBUF; nb = size - ofs;
   if(debug) printf("   Read & verify %d bytes from offset %ld\n",nb,(long)ofs);
   if(lseek(fd1,ofs,0) != ofs) errex("lseek(2)");
   if(read(fd1,buf,(RWA3)nb) < 0) errex("read(1)");
   for(i=0; i<nb; ++i)
      {
      expect = (ofs+i==0 || ofs+i+1>osize)? 0:(ofs+i-1)%256;
      if(buf[i] != expect)
         {
         sprintf(z,"read(%d) byte %d; got %d, not %d",seq,i,(int)buf[i],expect);
         errex0(z);
         }
      }

   if(close(fd1)) errex("close(2)");
   }

*cursize = size;
if(debug) printf("   File truncated to %ld bytes.\n\n",size);
}

/*****************************************************************************/
