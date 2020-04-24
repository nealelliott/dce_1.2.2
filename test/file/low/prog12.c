/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: prog12.c,v $
 * Revision 1.1.123.1  1996/10/17  18:29:18  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:54:44  damon]
 *
 * Revision 1.1.118.1  1994/02/04  20:46:51  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:25:14  devsrc]
 * 
 * Revision 1.1.116.1  1993/12/07  17:45:54  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:35:57  jaffe]
 * 
 * Revision 1.1.10.2  1993/07/19  19:43:41  zeliff
 * 	HP port of DFS
 * 	[1993/07/19  18:32:53  zeliff]
 * 
 * Revision 1.1.8.3  1993/07/16  22:03:36  kissel
 * 	Get rid of the damn quotes in #include.
 * 	[1993/06/21  15:09:01  kissel]
 * 
 * Revision 1.1.5.5  1993/03/09  21:22:02  jaffe
 * 	Transarc delta: jess-ot7194-fs-low-kerneldump 1.1
 * 	  Selected comments:
 * 	    Change the icl_KernelDump parameter according to the changes
 * 	    from icl utilities.
 * 	    See above and also fix some minor bugs and compiler warnings on pmax.
 * 	[1993/03/09  20:49:41  jaffe]
 * 
 * Revision 1.1.5.4  1993/01/11  18:33:07  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:14:42  htf]
 * 
 * Revision 1.1.5.3  1992/11/24  21:45:33  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:45:23  bolinger]
 * 
 * Revision 1.1.5.2  1992/10/28  18:00:47  jaffe
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
 * 	[1992/10/28  14:28:46  jaffe]
 * 
 * Revision 1.1.3.4  1992/05/21  12:46:28  jaffe
 * 	Transarc delta: fred-merge-tests 1.1
 * 	  Selected comments:
 * 	    Created this delta to merge Phil Hirsch's final DFS test code changes with
 * 	    the test code that the OSF had.  Hopefully, this means we and the OSF
 * 	    should be looking at the same test code.
 * 	    Changes to facilitate merge of local DFS test code with the OSF's.
 * 	[1992/05/20  20:34:03  jaffe]
 * 
 * Revision 1.1.3.3  1992/01/24  04:15:22  devsrc
 * 	Merging dfs6.3
 * 	[1992/01/24  00:23:34  devsrc]
 * 
 * Revision 1.1.3.2  1992/01/23  20:31:00  zeliff
 * 	Moving files onto the branch for dfs6.3 merge
 * 	[1992/01/23  18:48:17  zeliff]
 * 	Revision 1.1.1.3  1992/01/23  22:26:13  devsrc
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
#include <sys/uio.h>
#include <sys/stat.h>
#include <common.h>
#include <dcedfs/icl.h>
#include <string.h>


#define BUFSIZE       8193
#define IOVSIZE       16

int nb[][IOVSIZE] =
    {
    {1},
    {2},
    {511},
    {512},
    {513},
    {8192},
    {8193},
    {1, 1},
    {2, 2},
    {511, 511},
    {512, 512},
    {513, 513},
    {8192, 8192},
    {8193, 8193},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
    {511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511},
    {512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512},
    {513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513},
    {8192, 8192, 8192, 8192, 8192, 8192, 8192, 8192, 8192, 8192, 8192, 8192, 8192, 8192, 8192, 8192},
    {8193, 8193, 8193, 8193, 8193, 8193, 8193, 8193, 8193, 8193, 8193, 8193, 8193, 8193, 8193, 8193},
    {1, 2, 511, 512, 513, 8192, 8193},
    {8193, 8192, 513, 512, 511, 2, 1},
    {141, 464, 3633, 648, 9, 50, 77, 3337, 3, 2225, 1, 451, 13, 80, 1710, 242}
    };

unsigned char buf[BUFSIZE],ibuf[IOVSIZE][BUFSIZE];
int debug=FALSE;

void errex(), errex0();

extern int errno;

/*****************************************************************************/

main(argc,argv) int argc; char *argv[];
{
int fd,i,j,k,isize,jsize;
long rf,tb;
struct iovec iov[IOVSIZE];
struct stat s;

setbuf(stdout,(char *)NULL);
isize = sizeof(nb)/sizeof(nb[0]);


printf("Scatter/gather test.\n");

for(i=0; i<isize; ++i)
   {
   printf("Test number %d of %d.\n",i+1,isize);
   for(j=0,jsize=IOVSIZE; j<IOVSIZE; ++j) {if(!nb[i][j]) {jsize=j+1; break;}}

   if((fd=open("tfile.12",O_RDWR|O_CREAT,0644)) < 0) errex("open(1)");

   for(j=0,tb=0; j<jsize; ++j)
      {
      iov[j].iov_base = (caddr_t)ibuf[j]; bzero((char *)&ibuf[j],BUFSIZE);
      iov[j].iov_len = nb[i][j];
      for(k=0; k<nb[i][j]; ++k) buf[k] = j+1;
      if((rf=write(fd,buf,(RWA3)nb[i][j])) != nb[i][j]) errex("write(1)");
      tb += nb[i][j];
      }
   if(debug) printf("   Wrote %ld bytes using write().\n",tb);

   if(lseek(fd,(off_t)0,0) != 0) errex("lseek(1)");
   if((rf=readv(fd,iov,jsize)) < 0) errex("readv(1)");
   if(debug) printf("   Read %ld bytes using readv()...",tb);
   for(j=0; j<jsize; ++j)
      {
      for(k=0; k<nb[i][j]; ++k) buf[k] = j+1;
      if(bcmp((char *)buf,(char * )&ibuf[j],nb[i][j]) != 0) errex0("compare(1)");
      }
   if(debug) printf("checked...");

   if(close(fd)) errex("close(1)");
   if(stat("tfile.12",&s)) errex("stat(1)");
   if(s.st_size != tb) errex0("st_size check(1)");
   if(debug) printf("s.st_size is %ld.\n",tb);
   if(unlink("tfile.12")) errex("unlink(1)");

   if((fd=open("tfile.12",O_RDWR|O_CREAT,0644)) < 0) errex("open(2)");

   for(j=0,tb=0; j<jsize; ++j)
      {
      iov[j].iov_base = (caddr_t)ibuf[j];
      iov[j].iov_len = nb[i][j];
      for(k=0; k<nb[i][j]; ++k) ibuf[j][k] = j+1;
      tb += nb[i][j];
      }
   if((rf=writev(fd,iov,jsize)) != tb) errex("writev(1)");
   if(debug) printf("   Wrote %ld bytes using writev().\n",tb);

   if(lseek(fd,(off_t)0,0) != 0) errex("lseek(2)");
   if((rf=readv(fd,iov,jsize)) < 0) errex("readv(2)");
   if(debug) printf("   Read %ld bytes using readv()...",tb);
   for(j=0; j<jsize; ++j)
      {
      for(k=0; k<nb[i][j]; ++k) buf[k] = j+1;
      if(bcmp((char *)buf,(char *) &ibuf[j],nb[i][j]) != 0) errex0("compare(2)");
      }
   if(debug) printf("checked...");
   if(stat("tfile.12",&s)) errex("stat(2)");
   if(s.st_size != tb) errex0("st_size check(2)");
   if(debug) printf("s.st_size is %ld.\n",tb);

   if(lseek(fd,(off_t)0,0) != 0) errex("lseek(3)");
   for(j=0; j<jsize; ++j)
      {
      bzero((char *)buf,BUFSIZE);
      if((rf=read(fd,buf,(RWA3)nb[i][j])) < 0) errex("read(1)");
      if(bcmp((char *)buf,(char *)&ibuf[j],nb[i][j]) != 0) errex0("compare(3)");
      }
   if(debug) printf("   Read and checked %ld bytes using read()...",tb);

   if(close(fd)) errex("close(2)");
   if(stat("tfile.12",&s)) errex("stat(3)");
   if(s.st_size != tb) errex0("st_size check(3)");
   if(debug) printf("s.st_size is %ld.\n",tb);
   if(unlink("tfile.12")) errex("unlink(2)");
   }

printf("Done.\n");
exit(0);
}

/*****************************************************************************/

void errex(s) char *s;
{
        FILE *dumpfilep;
        char dumpfile[256];

        printf("\n%s failed; errno = %d\n",s,errno);
        if (debug) {
            sprintf(dumpfile,"/tmp/low.prog12.kernel.dump.%d",getpid());
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
            sprintf(dumpfile,"/tmp/low.prog12.kernel.dump.%d",getpid());
            dumpfilep = fopen(dumpfile, "w");
            icl_DumpKernel(dumpfilep, NULL, ICL_DUMP_FORMATTED);
            fclose(dumpfilep);
        }
        exit(1);
}


/*****************************************************************************/
