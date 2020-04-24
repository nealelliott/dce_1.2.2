/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: prog14.c,v $
 * Revision 1.1.116.1  1996/10/17  18:29:22  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:54:45  damon]
 *
 * Revision 1.1.111.1  1994/02/04  20:46:54  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:25:16  devsrc]
 * 
 * Revision 1.1.108.2  1994/01/17  16:43:44  root
 * 	Fix precedence problem in read_it and add test case
 * 	for read returning EOF
 * 	[1994/01/17  16:42:53  root]
 * 
 * Revision 1.1.108.1  1993/12/07  17:45:57  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:36:33  jaffe]
 * 
 * Revision 1.1.9.2  1993/07/19  19:43:53  zeliff
 * 	HP port of DFS
 * 	[1993/07/19  18:33:02  zeliff]
 * 
 * Revision 1.1.7.3  1993/07/16  22:03:43  kissel
 * 	Get rid of the damn quotes in #include.
 * 	[1993/06/21  15:09:35  kissel]
 * 
 * Revision 1.1.4.5  1993/03/09  21:22:15  jaffe
 * 	Transarc delta: jess-ot7194-fs-low-kerneldump 1.1
 * 	  Selected comments:
 * 	    Change the icl_KernelDump parameter according to the changes
 * 	    from icl utilities.
 * 	    See above.
 * 	[1993/03/09  20:50:56  jaffe]
 * 
 * Revision 1.1.4.4  1993/01/11  18:33:12  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:14:57  htf]
 * 
 * Revision 1.1.4.3  1992/11/24  21:45:45  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:45:38  bolinger]
 * 
 * Revision 1.1.4.2  1992/10/28  18:01:04  jaffe
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
 * 	[1992/10/28  14:31:23  jaffe]
 * 
 * Revision 1.1.2.2  1992/05/21  12:46:39  jaffe
 * 	Transarc delta: fred-merge-tests 1.1
 * 	  Selected comments:
 * 	    Created this delta to merge Phil Hirsch's final DFS test code changes with
 * 	    the test code that the OSF had.  Hopefully, this means we and the OSF
 * 	    should be looking at the same test code.
 * 	    Changes to facilitate merge of local DFS test code with the OSF's.
 * 	[1992/05/20  20:35:24  jaffe]
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
#include <common.h>
#include <dcedfs/icl.h>
#include <string.h>




#define BUFSIZE 512

char buf[BUFSIZE],*p;
int afsflag,blks_per_io,debug=TRUE,fd,n_of,plen;
long size;
long of[] = {10,100,1000,1023,4096,8193,
             100000L,1000000L,10000000L /* ,100000000L,1000000000L */};
struct stat s;

void list_it(),read_it(),write_it(),errex(), errex0();

extern int errno;

/*****************************************************************************/

main(argc,argv) int argc; char *argv[];
{
char buf[BUFSIZE],fn[21],z[256];
int pid;

setbuf(stdout,(char *)NULL);
strcpy(fn,"tfile.14");
n_of = sizeof(of)/sizeof(of[0]);
pid = getpid();

printf("Prog14 pid %d starting.\n",pid);
(void)getwd(z); afsflag = (strncmp(z,"/afs/",5) == 0);
printf("This is%s an AFS filesystem.\n\n",afsflag? "":" not");

p = "test14"; plen=strlen(p);

printf("Opening test file %s.\n",fn);
if((fd=open(fn,O_RDWR|O_CREAT|O_TRUNC,0644)) < 0) errex("open(1)");

printf("\nWriting test file.\n"); write_it();
if(debug) list_it(fn);
printf("\nReading test file.\n"); read_it();

printf("\nClosing and re-opening test file.\n");
if(close(fd)) errex("close(1)");
if((fd=open(fn,O_WRONLY|O_TRUNC)) < 0) errex("open(2)");
printf("\nWriting test file.\n"); write_it();

if(debug) list_it(fn);

printf("\nClosing and re-opening test file.\n");
if(close(fd)) errex("close(2)");
if((fd=open(fn,O_RDONLY)) < 0) errex("open(3)");
printf("\nReading test file.\n"); read_it();

printf("\nCleaning up and going home.\n");
if(close(fd)) errex("close(3)");
if(unlink(fn)) errex("unlink(1)");
printf("\nProg14 pid %d exiting.\n",pid);
exit(0);
}

/*****************************************************************************/

void write_it()
{
int i,j;

for(i=0; i<n_of; ++i)
   {
   if(afsflag && of[i] > 1024*1024)
      {
      printf(">>> Skipping offset-%d write in AFS <<<\n",of[i]);
      continue;
      }
   if(debug) printf("Writing at offset %ld...",of[i]);
   if(lseek(fd,of[i],0) != of[i]) errex("lseek(1)");
   if(write(fd,p,plen) != plen) errex("write(1)");
   if(fstat(fd,&s)) errex("fstat(1)");
   if(s.st_size != of[i]+plen) errex0("st_size check (1)");
   size = s.st_size;
   if(i == 0) blks_per_io = s.st_blocks;
   if(debug) printf("%ld blocks used.\n",s.st_blocks);
   if(s.st_blocks > (i+1)*blks_per_io+1)
      printf(">>> Warning: %ld blocks used <<<\n",s.st_blocks);
   }
}

/*****************************************************************************/

void read_it()
{
int i,j,rf;
long bytes_read=0,loc,next_ix=1,next_of=of[0];

if(lseek(fd,0L,0) != 0) errex("lseek(2)");

while(bytes_read < size)
   {
   if(debug && next_of-bytes_read<BUFSIZE)
      printf("Reading from offset %ld.\n",bytes_read);
   if ((rf = read(fd,buf,(RWA3)BUFSIZE)) < 0) 
       errex("read(1)");
   if (rf == 0){
     if (debug)
         printf("Unexpected EOF offset %ld size %ld ",bytes_read, size);
     errex("read(2)");
   }
 
   for(i=0; i<rf; ++i)
      {
      if(next_of && bytes_read+i>=next_of) /* should be in string p */
         {
         j = bytes_read+i-next_of;
         if(buf[i] != p[j]) errex0("data compare(1)");
         if(j+1 == plen) /* just saw last character of p */
            {
            next_of = (next_ix<n_of)? of[next_ix++]:0;
            }
         }
      else /* should be zeroes */
         {if(buf[i] != 0) errex0("data check(2)");}
      }
   bytes_read += rf;
   }

if(debug) printf("Data reads OK.\n");
}

/*****************************************************************************/

void list_it(fn)
char *fn;
{
printf("\n");
sprintf(buf,"ls -l %s",fn); system(buf);
sprintf(buf,"du -a %s",fn); system(buf);
}

/*****************************************************************************/


void errex(s) char *s;
{
        FILE *dumpfilep;
        char dumpfile[256];

        printf("\n%s failed; errno = %d\n",s,errno);
        if (debug) {
            sprintf(dumpfile,"/tmp/low.prog14.kernel.dump.%d",getpid());
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
            sprintf(dumpfile,"/tmp/low.prog14.kernel.dump.%d",getpid());
            dumpfilep = fopen(dumpfile, "w");
            icl_DumpKernel(dumpfilep, NULL, ICL_DUMP_FORMATTED);
            fclose(dumpfilep);
        }
        exit(1);
}

/*****************************************************************************/
