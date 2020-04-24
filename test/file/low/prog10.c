/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: prog10.c,v $
 * Revision 1.1.121.1  1996/10/17  18:29:13  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:54:41  damon]
 *
 * Revision 1.1.116.1  1994/02/04  20:46:46  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:25:11  devsrc]
 * 
 * Revision 1.1.114.1  1993/12/07  17:45:49  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:35:22  jaffe]
 * 
 * Revision 1.1.10.2  1993/07/19  19:43:25  zeliff
 * 	HP port of DFS
 * 	[1993/07/19  18:32:44  zeliff]
 * 
 * Revision 1.1.8.3  1993/07/16  22:03:28  kissel
 * 	Get rid of the damn quotes in #include.
 * 	[1993/06/21  15:08:23  kissel]
 * 
 * Revision 1.1.5.5  1993/03/09  21:21:49  jaffe
 * 	Transarc delta: jess-ot7194-fs-low-kerneldump 1.1
 * 	  Selected comments:
 * 	    Change the icl_KernelDump parameter according to the changes
 * 	    from icl utilities.
 * 	    See above and also fix some compiler warnings on pmax.
 * 	[1993/03/09  20:48:17  jaffe]
 * 
 * Revision 1.1.5.4  1993/01/11  18:33:01  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:14:27  htf]
 * 
 * Revision 1.1.5.3  1992/11/24  21:45:18  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:45:09  bolinger]
 * 
 * Revision 1.1.5.2  1992/10/28  18:00:33  jaffe
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
 * 	[1992/10/28  14:27:56  jaffe]
 * 
 * Revision 1.1.3.3  1992/01/24  04:15:17  devsrc
 * 	Merging dfs6.3
 * 	[1992/01/24  00:23:27  devsrc]
 * 
 * Revision 1.1.3.2  1992/01/23  20:30:48  zeliff
 * 	Moving files onto the branch for dfs6.3 merge
 * 	[1992/01/23  18:48:07  zeliff]
 * 	Revision 1.1.1.3  1992/01/23  22:26:06  devsrc
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
#include <common.h>
#include <dcedfs/icl.h>
#include <string.h>

#define BUFSIZE    1024

int debug=FALSE;
int bsize[] = {1,128,512,1024};
/* int bsize[] = {1,2,50,100,128,256,511,512,513,1024}; */

void errex(), errex0();

extern int errno;

/*****************************************************************************/

main(argc,argv) int argc; char *argv[];
{
char fn[21],z[256];
unsigned char rbuf[BUFSIZE],wbuf[BUFSIZE],val;
int afsflag,b,bs,fd,i,j,nr_b,nr_times,openflag,pid;
long fsize,maxsize,nb,rf;
struct stat s;

setbuf(stdout,(char *)NULL);

if(argc < 3) {nr_times = 1; maxsize = 2*BUFSIZE+1;}
else
   {
   nr_times = atoi(argv[1]);
   maxsize = atol(argv[2]);
   }
nr_b = sizeof(bsize)/sizeof(bsize[0]);
pid = getpid();
strcpy(fn,"tfile.10");

printf("Prog10 pid %d, nr_times %d, filesize %ld.\n",pid,nr_times,maxsize);
#ifdef MIPS
(void)getwd(z); afsflag = (strncmp(z,"/afs/",5) == 0);
printf("This is%s an AFS filesystem.\n",afsflag? "":" not");
#endif

for(i=1; i<=nr_times || !nr_times; ++i)
   {
   printf("\nProg10 pid %d iteration %d.\n",pid,i);

   for(openflag=0; openflag<=1; ++openflag)
      {
      for(b=0; b<nr_b; ++b)
         {
         bs = bsize[b];
         printf("Prog10 openflag=%s, bitesize %d...",
            openflag? "TRUE":"FALSE",bs);

#ifdef MIPS
         if(afsflag)
            {
            printf(">>> Skipping fcntl(F_SETFL,O_APPEND) test in AFS. <<<\n");
            if((fd=open(fn,O_RDWR|O_CREAT|O_TRUNC|O_APPEND,0644)) < 0)
               errex("open(1a)");
            }
         else
#endif
            {
            if((fd=open(fn,O_RDWR|O_CREAT|O_TRUNC,0644)) < 0)
               errex("open(1b)");
            if(!openflag) {if(close(fd)) errex("close(1)");}
            else
               {
               if(fcntl(fd,F_SETFL,O_APPEND) == -1) errex("fcntl(1)");
               if((j=fcntl(fd,F_GETFL,0)) == -1) errex("fcntl(2)");
               if(!(j & O_APPEND)) errex("O_APPEND check (1)");
               }
            }

         printf("growing...");
         for(fsize=0,val=0; fsize<=maxsize; fsize+=bs,++val)
            {
            for(j=0; j<bs; ++j) wbuf[j] = val;
            if(!openflag)
               {
#ifdef MIPS
               if(afsflag)
                  {
                  printf(">>> Skipping fcntl(F_SETFL,O_APPEND) test in AFS. <<<\n");
                  if((fd=open(fn,O_RDWR|O_APPEND)) < 0) errex("open(2a)");
                  }
               else
#endif
                  {
                  if((fd=open(fn,O_RDWR)) < 0) errex("open(2b)");
                  if(fcntl(fd,F_SETFL,O_APPEND) == -1) errex("fcntl(3)");
                  if((j=fcntl(fd,F_GETFL,0)) == -1) errex("fcntl(4)");
                  if(!(j & O_APPEND)) errex("O_APPEND check (2)");
                  }
               }
            if(lseek(fd,(off_t)0,0) != 0) errex("lseek(1)");
            if((rf=write(fd,wbuf,(RWA3)bs)) != bs) errex("write(1)");
            if(!openflag && (rf=close(fd))) errex("close(2)");
            if(stat(fn,&s)) errex("stat(1)");
            if(s.st_size != fsize+bs) errex0("st_size check(1)");
            }

         printf("checking...");
         if(!openflag && ((fd=open(fn,O_RDONLY))<0)) errex("open(3)");
         if(lseek(fd,(off_t)0,0) != 0) errex("lseek(2)");
         for(nb=0,val=0; nb<fsize; nb+=bs,++val)
            {
            for(j=0; j<bs; ++j) wbuf[j] = val;
            bzero((char *)rbuf,bs);
            if((rf=read(fd,rbuf,(RWA3)bs)) < 0) errex("read(1)");
            if(bcmp((char *)rbuf,(char *)wbuf,bs) != 0) errex0("read check(1)");
            }
         if(!openflag && (rf=close(fd))) errex("close(3)");

         printf("shrinking...");
         for(; fsize>0; fsize-=bs)
            {
            if(!openflag)
               {if((fd=open(fn,O_RDWR)) < 0) errex("open(4)");}
            if(ftruncate(fd,(off_t)(fsize-bs))) errex("ftruncate(1)");
            if(!openflag && (rf=close(fd))) errex("close(4)");
            if(stat(fn,&s)) errex("stat(2)");
            if(s.st_size != fsize-bs) errex0("st_size check(2)");
            }

         printf("\n");
         if(openflag && (rf=close(fd))) errex("close(5)");
         }
      }
   }

if(unlink(fn)) errex("unlink(1)");
printf("\nProg10 pid %d exiting.\n",pid);
exit(0);
}

/*****************************************************************************/

void errex(s) char *s;
{
        FILE *dumpfilep;
        char dumpfile[256];

        printf("\n%s failed; errno = %d\n",s,errno);
        if (debug) {
            sprintf(dumpfile,"/tmp/low.prog10.kernel.dump.%d",getpid());
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
            sprintf(dumpfile,"/tmp/low.prog10.kernel.dump.%d",getpid());
            dumpfilep = fopen(dumpfile, "w");
            icl_DumpKernel(dumpfilep, NULL, ICL_DUMP_FORMATTED);
            fclose(dumpfilep);
        }
        exit(1);
}

/*****************************************************************************/
