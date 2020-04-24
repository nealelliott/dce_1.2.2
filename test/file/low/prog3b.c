/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: prog3b.c,v $
 * Revision 1.1.127.1  1996/10/17  18:29:25  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:54:47  damon]
 *
 * Revision 1.1.122.1  1994/02/04  20:46:57  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:25:18  devsrc]
 * 
 * Revision 1.1.120.1  1993/12/07  17:46:00  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:37:13  jaffe]
 * 
 * Revision 1.1.10.2  1993/07/19  19:44:05  zeliff
 * 	HP port of DFS
 * 	[1993/07/19  18:33:09  zeliff]
 * 
 * Revision 1.1.8.3  1993/07/16  22:13:44  kissel
 * 	Get rid of the damn quotes in #include.
 * 	[1993/06/21  15:10:03  kissel]
 * 
 * Revision 1.1.5.5  1993/03/09  21:22:35  jaffe
 * 	Transarc delta: jess-ot7194-fs-low-kerneldump 1.1
 * 	  Selected comments:
 * 	    Change the icl_KernelDump parameter according to the changes
 * 	    from icl utilities.
 * 	    See above.
 * 	[1993/03/09  20:52:26  jaffe]
 * 
 * Revision 1.1.5.4  1993/01/11  18:33:16  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:15:17  htf]
 * 
 * Revision 1.1.5.3  1992/11/24  21:45:53  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:45:51  bolinger]
 * 
 * Revision 1.1.5.2  1992/10/28  18:12:58  jaffe
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
 * 	[1992/10/28  14:32:18  jaffe]
 * 
 * Revision 1.1.3.3  1992/01/24  04:15:27  devsrc
 * 	Merging dfs6.3
 * 	[1992/01/24  00:23:44  devsrc]
 * 
 * Revision 1.1.3.2  1992/01/23  20:31:17  zeliff
 * 	Moving files onto the branch for dfs6.3 merge
 * 	[1992/01/23  18:48:29  zeliff]
 * 	Revision 1.1.1.3  1992/01/23  22:26:22  devsrc
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


#define MAXBUF        8192

char *file="./tfile.3";
unsigned char rbuf[MAXBUF];
int bsize[] = {512, 8192};
int debug=TRUE;
long offset[] = {0L, 1L};
long inc,nr_blocks;
struct timeval tv_prev,tv_cur;
struct timezone tz;

double endtime();
long randblock();
void errex(),starttime();

extern int errno;

/*****************************************************************************/

main(argc,argv) int argc; char *argv[];
{
char path[256];
double etime;
int afsflag,b,bs,dfsflag,fd,fflag,flush_flag,o;
long j,fsize;
off_t of,pos;
struct stat s;

setbuf(stdout,(char *)NULL);
flush_flag = (argc>1 && EQ(argv[1],"-flush"));

if(stat(file,&s)) errex("stat()"); else fsize = s.st_size;
printf("\nRead test: filesize %ld.\n",fsize);

(void)getwd(path);
afsflag = (strncmp(path,"/afs/",5) == 0);
dfsflag = (strncmp(path,"/.../",5) == 0);

printf("This is%s an AFS filesystem.\n",afsflag? "":" not");
printf("This is%s a DFS filesystem.\n",dfsflag? "":" not");
printf("\n");

for(fflag=0; fflag<=1; ++fflag)
   {
   if(fflag && !flush_flag) continue;

   printf("\nSequential read, %s before reads\n",fflag? "flush":"don't flush");
   for(o=0; o<sizeof(offset)/sizeof(long); ++o)
      {
      for(b=0; b<sizeof(bsize)/sizeof(int); ++b)
         {
         etime = 0.; bs = bsize[b]; of = offset[o];
         nr_blocks = fsize/bs;
         printf("Offset %ld, blocksize %d\n",(long)of,bs);
         if(fflag)
            {
            if(afsflag) system("fs flushv .; fs flush tfile.3");
            else if(dfsflag) system("cm flushf .; cm flush tfile.3");
            system("sleep 5; cat /*x* /bin/* /etc/* 2> /dev/null | wc > wcfile");
            }
         starttime();
         if((fd=open(file,O_RDONLY)) < 0) errex("open(seq)");
         if(lseek(fd,of,0) != of) errex("lseek(seq)");
         for(j=0; j<nr_blocks; ++j)
            {if(read(fd,rbuf,(RWA3)bs) != bs) errex("read(seq)");}
         if(close(fd)) errex("close(seq)");
         etime = endtime();

         printf("   Read %ld bytes sequentially in %.2f seconds\n",fsize,etime);
         printf("   Throughput = %.1f bytes per second\n",fsize/etime);
         }
      }
   }

printf("Done reading sequentially.\n");

for(fflag=0; fflag<=1; ++fflag)
   {
   if(fflag && !flush_flag) continue;

   printf("\nRandom read, %s before reads\n",fflag? "flush":"don't flush");
   for(o=0; o<sizeof(offset)/sizeof(long); ++o)
      {
      for(b=0; b<sizeof(bsize)/sizeof(int); ++b)
         {
         etime = 0.; bs = bsize[b]; of = offset[o];
         nr_blocks = fsize/bs;
         inc = nr_blocks/2 - 1;
         printf("Offset %ld, blocksize %d\n",(long)of,bs);
         if(fflag)
            {
            if(afsflag) system("fs flushv .; fs flush tfile.3");
            else if(dfsflag) system("cm flushf .; cm flush tfile.3");
            system("sleep 5; cat /*x* /bin/* /etc/* 2> /dev/null | wc > wcfile");
            }
         starttime();
         if((fd=open(file,O_RDONLY)) < 0) errex("open(rand)");
         for(j=0; j<nr_blocks; ++j)
            {
            pos = bs*randblock() + of;
            if(lseek(fd,pos,0) != pos) errex("lseek(rand)");
            if(read(fd,rbuf,(RWA3)bs) != bs) errex("read(rand)");
            }
         if(close(fd)) errex("close(rand)");
         etime = endtime();

         printf("   Read %ld bytes randomly in %.2f seconds\n",fsize,etime);
         printf("   Throughput = %.1f bytes per second\n",fsize/etime);
         }
      }
   }

printf("Done reading randomly.\n\n");
exit(0);
}

/*****************************************************************************/
void errex(s) char *s;
{
        FILE *dumpfilep;
        char dumpfile[256];

        printf("\n%s failed; errno = %d\n",s,errno);
        if (debug) {
            sprintf(dumpfile,"/tmp/low.prog3b.kernel.dump.%d",getpid());
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
