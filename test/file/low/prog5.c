/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: prog5.c,v $
 * Revision 1.1.119.1  1996/10/17  18:29:28  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:54:49  damon]
 *
 * Revision 1.1.114.1  1994/02/04  20:47:01  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:25:19  devsrc]
 * 
 * Revision 1.1.112.1  1993/12/07  17:46:02  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:37:45  jaffe]
 * 
 * Revision 1.1.10.2  1993/07/19  19:44:16  zeliff
 * 	HP port of DFS
 * 	[1993/07/19  18:33:16  zeliff]
 * 
 * Revision 1.1.8.3  1993/07/16  22:13:51  kissel
 * 	Get rid of the damn quotes in #include.
 * 	[1993/06/21  15:10:44  kissel]
 * 
 * Revision 1.1.5.5  1993/03/09  21:22:44  jaffe
 * 	Transarc delta: jess-ot7194-fs-low-kerneldump 1.1
 * 	  Selected comments:
 * 	    Change the icl_KernelDump parameter according to the changes
 * 	    from icl utilities.
 * 	    See above.
 * 	[1993/03/09  20:53:22  jaffe]
 * 
 * Revision 1.1.5.4  1993/01/11  18:33:22  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:15:27  htf]
 * 
 * Revision 1.1.5.3  1992/11/24  21:46:03  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:46:04  bolinger]
 * 
 * Revision 1.1.5.2  1992/10/28  18:13:11  jaffe
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
 * 	    Fix teh problem.
 * 	[1992/10/28  14:33:15  jaffe]
 * 
 * Revision 1.1.3.3  1992/01/24  04:15:31  devsrc
 * 	Merging dfs6.3
 * 	[1992/01/24  00:23:50  devsrc]
 * 
 * Revision 1.1.3.2  1992/01/23  20:31:26  zeliff
 * 	Moving files onto the branch for dfs6.3 merge
 * 	[1992/01/23  18:48:38  zeliff]
 * 	Revision 1.1.1.3  1992/01/23  22:26:29  devsrc
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

#define FILESIZE      (16 * 1024)
#define BUFSIZE       8192

int debug=FALSE;

void errex(), errex0();

extern int errno;

/*****************************************************************************/

main(argc,argv) int argc; char *argv[];
{
char buf[BUFSIZE],c,*file="./tfile.5",z[81];
int fd,i0=0,i1=255,iz,i,j;
off_t of;
struct stat s;

setbuf(stdout,(char *)NULL);


printf("Memory march test: filesize %ld\n",FILESIZE);
for(i=0; i<=1; ++i)
   {
   printf("Running with (i0,i1) = (%x,%x)\n",i0,i1);

   printf("Initializing file (1)\n");
   if((fd=open(file,O_WRONLY|O_CREAT|O_TRUNC,0644)) < 0) errex("open(1)");
   for(j=0; j<BUFSIZE; ++j) buf[j]=(char)i0;
   for(j=0; j<FILESIZE/BUFSIZE; ++j)
      {if(write(fd,buf,(RWA3)BUFSIZE) != BUFSIZE) errex("write(1)");}
   if(fsync(fd)) errex("fsync(1)");
   if(close(fd)) errex("close(1)");
   if(stat(file,&s)) errex("stat(1)");
   if(s.st_size != FILESIZE) errex0("stat compare (1)");

   printf("Making forward pass (2)\n");
   if((fd=open(file,O_RDWR)) < 0) errex("open(2)");
   for(of=0; of<FILESIZE; ++of)
      {
      if(lseek(fd,of,0) != of) errex("lseek(2)");
      c = 165; if(read(fd,&c,(RWA3)1) != 1) errex("read(1)");
      if(c != (char)i0)
         {
         sprintf(z,"first byte compare (2) (of=%ld, got %x, not %x)",
            (long)of,c,i0);
         errex0(z);
         }
      if(lseek(fd,(off_t)-1,1) != of) errex("lseek(2)");
      c = (char)i1; if(write(fd,&c,(RWA3)1) != 1) errex("write(2)");
      if(lseek(fd,(off_t)-1,1) != of) errex("lseek(2)");
      c = 165; if(read(fd,&c,(RWA3)1) != 1) errex("read(2)");
      if(c != (char)i1)
         {
         sprintf(z,"second byte compare (2) (of=%ld, got %x, not %x)",
            (long)of,c,i1);
         errex0(z);
         }
      }

   printf("Making reverse pass (3)\n");
   for(of=FILESIZE-1; of>=0; --of)
      {
      if(lseek(fd,of,0) != of) errex("lseek(3)");
      c = 165; if(read(fd,&c,(RWA3)1) != 1) errex("read(3)");
      if(c != (char)i1)
         {
         sprintf(z,"first byte compare (3) (of=%ld, got %x, not %x)",
            (long)of,c,i1);
         errex0(z);
         }
      if(lseek(fd,(off_t)-1,1) != of) errex("lseek(3)");
      c = (char)i0; if(write(fd,&c,(RWA3)1) != 1) errex("write(3)");
      if(lseek(fd,(off_t)-1,1) != of) errex("lseek(3)");
      c = 165; if(read(fd,&c,(RWA3)1) != 1) errex("read(4)");
      if(c != (char)i0)
         {
         sprintf(z,"second byte compare (3) (of=%ld, got %x, not %x)",
            (long)of,c,i0);
         errex0(z);
         }
      }

   printf("Closing file (4)\n");
   if(close(fd)) errex("close(4)");

   iz = i0; i0 = i1; i1 = iz;
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
            sprintf(dumpfile,"/tmp/low.prog5.kernel.dump.%d",getpid());
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
            sprintf(dumpfile,"/tmp/low.prog5.kernel.dump.%d",getpid());
            dumpfilep = fopen(dumpfile, "w");
            icl_DumpKernel(dumpfilep, NULL, ICL_DUMP_FORMATTED);
            fclose(dumpfilep);
        }
        exit(1);
}


/*****************************************************************************/
