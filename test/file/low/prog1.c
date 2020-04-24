/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: prog1.c,v $
 * Revision 1.1.120.1  1996/10/17  18:29:12  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:54:40  damon]
 *
 * Revision 1.1.115.1  1994/02/04  20:46:43  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:25:11  devsrc]
 * 
 * Revision 1.1.113.1  1993/12/07  17:45:47  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:34:57  jaffe]
 * 
 * Revision 1.1.10.2  1993/07/19  19:43:20  zeliff
 * 	HP port of DFS
 * 	[1993/07/19  18:32:40  zeliff]
 * 
 * Revision 1.1.8.3  1993/07/16  22:03:21  kissel
 * 	Get rid of the damn quotes in #include.
 * 	[1993/06/21  15:08:05  kissel]
 * 
 * Revision 1.1.5.5  1993/03/09  21:21:41  jaffe
 * 	Transarc delta: jess-ot7194-fs-low-kerneldump 1.1
 * 	  Selected comments:
 * 	    Change the icl_KernelDump parameter according to the changes
 * 	    from icl utilities.
 * 	    See above.
 * 	[1993/03/09  20:47:41  jaffe]
 * 
 * Revision 1.1.5.4  1993/01/11  18:32:57  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:14:22  htf]
 * 
 * Revision 1.1.5.3  1992/11/24  21:45:14  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:45:02  bolinger]
 * 
 * Revision 1.1.5.2  1992/10/28  18:00:24  jaffe
 * 	Transarc delta: jess-add-dumpkernel-in-low-tests 1.5
 * 	  Selected comments:
 * 	    Add function call (icl_DumpKernel) in the test suite. In case of running
 * 	    error of C programmes in the test suite the "icl_DumpKernel" function
 * 	    will be called in errex() to do the kernel trace. The dump file is located
 * 	    in "/tmp/prog#.kernel.dump.pid". You have to have the debug flag set to
 * 	    have the dump file.
 * 	    See above.
 * 	    Add the icl_DumpKernel() function call.
 * 	    Process another C file and add "low" as dump file header.
 * 	    Turn on the prog3 debug flags.
 * 	    It should not create the zero length dumpfile when there is nothing wrong.
 * 	    Fix the problem.
 * 	[1992/10/28  14:27:28  jaffe]
 * 
 * Revision 1.1.3.3  1992/01/24  04:15:15  devsrc
 * 	Merging dfs6.3
 * 	[1992/01/24  00:23:24  devsrc]
 * 
 * Revision 1.1.3.2  1992/01/23  20:30:42  zeliff
 * 	Moving files onto the branch for dfs6.3 merge
 * 	[1992/01/23  18:48:02  zeliff]
 * 	Revision 1.1.1.3  1992/01/23  22:26:03  devsrc
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
#include <sys/time.h>
#include <common.h>
#include <dcedfs/icl.h>
#include <string.h>


#define BUFSIZE       1024

#define N_OPENCLOSE   1500
#define N_SM_READ     8000
#define N_SM_CREATE   500
#define N_SM_WRITE    2000
#define N_SM_APPEND   20000

#ifndef N_LG_SEEK
#define N_LG_SEEK     1200
#endif

char *sm_file="./16_bytes",*lg_file="./half_meg";
char *rep_fmt="%-45s %5d times",*norep_fmt="%-57s";
int first_timestamp = TRUE,debug=FALSE;
struct timeval tv_prev,tv_cur;
struct timezone tz;

long next_random();
void errex(),timestamp();

extern int errno;

/*****************************************************************************/

main(argc,argv) int argc; char *argv[];
{
char rbuf[BUFSIZE],wbuf[BUFSIZE],z[81];
int fd1,fd2,fd3,fd4,fd5,fd6,i,sm_nb;
long lg_nb,nb_read,nb_written,rf,max;
off_t pos;
struct stat s;

if(stat(sm_file,&s)) errex("stat(sm_file)"); else sm_nb = s.st_size;
if(stat(lg_file,&s)) errex("stat(lg_file)"); else lg_nb = s.st_size;
for(i=0; i<BUFSIZE; ++i) wbuf[i]='A';
setbuf(stdout,(char *)NULL);
printf("Notes: 'small' is %d bytes, and 'large' is %ld bytes\n",sm_nb,lg_nb);
printf("       Large files are read and written in %d-byte pieces\n\n",BUFSIZE);
timestamp();

printf(rep_fmt,"Open/close (two files, three ways each)",N_OPENCLOSE);
for(i=1; i<N_OPENCLOSE; ++i)
   {
   if((fd1=open(sm_file,O_RDONLY)) < 0) errex("open(sm_file,R))");
   if((fd2=open(lg_file,O_RDONLY)) < 0) errex("open(lg_file,R))");
   if((fd3=open(sm_file,O_WRONLY)) < 0) errex("open(sm_file,W))");
   if((fd4=open(lg_file,O_WRONLY)) < 0) errex("open(lg_file,W))");
   if((fd5=open(sm_file,O_RDWR)) < 0) errex("open(sm_file,RW))");
   if((fd6=open(lg_file,O_RDWR)) < 0) errex("open(lg_file,RW))");
   if(close(fd1)) errex("close(sm_file,R))");
   if(close(fd2)) errex("close(lg_file,R))");
   if(close(fd3)) errex("close(sm_file,W))");
   if(close(fd4)) errex("close(lg_file,W))");
   if(close(fd5)) errex("close(sm_file,RW))");
   if(close(fd6)) errex("close(lg_file,RW))");
   }
timestamp();

printf("\n");

printf(rep_fmt,"Small create/write/close (separate files)",N_SM_CREATE);
for(i=1; i<N_SM_CREATE; ++i)
   {
   sprintf(z,"./temp%d",i);
   if((fd1=open(z,O_WRONLY|O_CREAT,0644)) < 0) errex("open");
   if(write(fd1,wbuf,(RWA3)sm_nb) != sm_nb) errex("write");
   if(close(fd1)) errex("close");
   }
timestamp();

printf(rep_fmt,"Small open/read/close (separate files)",N_SM_CREATE);
for(i=1; i<N_SM_CREATE; ++i)
   {
   sprintf(z,"./temp%d",i);
   if((fd1=open(z,O_RDONLY)) < 0) errex("open");
   if(read(fd1,rbuf,(RWA3)sm_nb) != sm_nb) errex("read");
   if(close(fd1)) errex("close");
   }
timestamp();

printf(rep_fmt,"Repeat small open/read/close (separate)",N_SM_CREATE);
for(i=1; i<N_SM_CREATE; ++i)
   {
   sprintf(z,"./temp%d",i);
   if((fd1=open(z,O_RDONLY)) < 0) errex("open");
   if(read(fd1,rbuf,(RWA3)sm_nb) != sm_nb) errex("read");
   if(close(fd1)) errex("close");
   }
timestamp();

printf("\n");

printf(rep_fmt,"Small open/write/close (same file)",N_SM_WRITE);
for(i=1; i<N_SM_WRITE; ++i)
   {
   if((fd1=open("./temp1",O_WRONLY)) < 0) errex("open");
   if(write(fd1,wbuf,(RWA3)sm_nb) != sm_nb) errex("write");
   if(close(fd1)) errex("close");
   }
timestamp();

printf(rep_fmt,"Small open/read/close (same file)",N_SM_READ);
for(i=1; i<N_SM_READ; ++i)
   {
   if((fd1=open(sm_file,O_RDONLY)) < 0) errex("open");
   if(read(fd1,rbuf,(RWA3)BUFSIZE) != sm_nb) errex("read");
   if(close(fd1)) errex("close");
   }
timestamp();

printf(rep_fmt,"Small append (same file)",N_SM_APPEND);
if((fd1=open("./temp1",O_WRONLY|O_APPEND)) < 0) errex("open");
for(i=1; i<N_SM_APPEND; ++i)
   {
   if(write(fd1,wbuf,(RWA3)sm_nb) != sm_nb) errex("write");
   }
timestamp();

if(stat("./temp1",&s)) errex("stat");
sprintf(z,"Close modified file (%ld bytes)",(long)(s.st_size));
printf(norep_fmt,z);
if(close(fd1)) errex("close");
timestamp();

printf("\n");

printf(norep_fmt,"Unlink temp files");
for(i=1; i<N_SM_CREATE; ++i)
   {
   sprintf(z,"./temp%d",i);
   if(unlink(z)) errex("unlink");
   }
timestamp();

printf("\n");

printf(norep_fmt,"Large open/read/close");
if((fd1=open(lg_file,O_RDONLY)) < 0) errex("open");
nb_read = 0;
while((rf = read(fd1,rbuf,(RWA3)BUFSIZE)) != 0) nb_read += rf;
if(close(fd1)) errex("close");
if(nb_read != lg_nb) errex("read");
timestamp();

printf(norep_fmt,"Repeat large open/read/close");
if((fd1=open(lg_file,O_RDONLY)) < 0) errex("open");
nb_read = 0;
while((rf = read(fd1,rbuf,(RWA3)BUFSIZE)) != 0) nb_read += rf;
if(close(fd1)) errex("close");
if(nb_read != lg_nb) errex("read");
timestamp();

printf("\n");

printf(norep_fmt,"Large open/write");
if((fd1=open("./temp1",O_WRONLY|O_CREAT,0644)) < 0) errex("open");
nb_written = 0;
while(nb_written < nb_read)
   {
   if(write(fd1,wbuf,(RWA3)BUFSIZE) != BUFSIZE) errex("write");
   nb_written += BUFSIZE;
   }
timestamp();

printf(norep_fmt,"Close new large file");
if(close(fd1)) errex("close");
timestamp();

printf(norep_fmt,"Large read/read/write (using 3 files)");
if((fd1=open(lg_file,O_RDONLY)) < 0) errex("open (1)");
if((fd2=open("./temp1",O_RDONLY)) < 0) errex("open (2)");
if((fd3=open("./temp2",O_WRONLY|O_CREAT,0644)) < 0) errex("open (3)");
nb_written = 0;
while(nb_written < nb_read)
   {
   if(read(fd1,rbuf,(RWA3)BUFSIZE) != BUFSIZE) errex("read (1)");
   if(read(fd2,rbuf,(RWA3)BUFSIZE) != BUFSIZE) errex("read (2)");
   if(write(fd3,wbuf,(RWA3)BUFSIZE) != BUFSIZE) errex("write");
   nb_written += BUFSIZE;
   }
if(close(fd1)) errex("close (1)");
if(close(fd2)) errex("close (2)");
timestamp();

printf(norep_fmt,"Close new large file");
if(close(fd3)) errex("close");
timestamp();

printf("\n");

printf(rep_fmt,"Large random seek/read (using 3 files)",N_LG_SEEK);
if((fd1=open(lg_file,O_RDONLY)) < 0) errex("open (1)");
if((fd2=open("./temp1",O_RDONLY)) < 0) errex("open (2)");
if((fd3=open("./temp2",O_RDONLY)) < 0) errex("open (3)");
rf = 0; max = lg_nb / BUFSIZE - 1;
for(i=1; i<=N_LG_SEEK/3; ++i)
   {
   rf = next_random(rf,max); pos = rf * BUFSIZE;
   if(lseek(fd1,pos,0) != pos) errex("lseek (1)");
   if(read(fd1,rbuf,(RWA3)BUFSIZE) != BUFSIZE) errex("read (1)");
   rf = next_random(rf,max);
   if(lseek(fd2,pos,0) != pos) errex("lseek (2)");
   if(read(fd2,rbuf,(RWA3)BUFSIZE) != BUFSIZE) errex("read (2)");
   rf = next_random(rf,max);
   if(lseek(fd3,pos,0) != pos) errex("lseek (3)");
   if(read(fd3,rbuf,(RWA3)BUFSIZE) != BUFSIZE) errex("read (3)");
   }
timestamp();
if(close(fd1)) errex("close (1)");
if(close(fd2)) errex("close (2)");
if(close(fd3)) errex("close (3)");

printf(rep_fmt,"Repeat large random seek/read",N_LG_SEEK);
if((fd1=open(lg_file,O_RDONLY)) < 0) errex("open (1)");
if((fd2=open("./temp1",O_RDONLY)) < 0) errex("open (2)");
if((fd3=open("./temp2",O_RDONLY)) < 0) errex("open (3)");
rf = 0; max = lg_nb / BUFSIZE - 1;
for(i=1; i<=N_LG_SEEK/3; ++i)
   {
   rf = next_random(rf,max); pos = rf * BUFSIZE;
   if(lseek(fd1,pos,0) != pos) errex("lseek (1)");
   if(read(fd1,rbuf,(RWA3)BUFSIZE) != BUFSIZE) errex("read (1)");
   rf = next_random(rf,max);
   if(lseek(fd2,pos,0) != pos) errex("lseek (2)");
   if(read(fd2,rbuf,(RWA3)BUFSIZE) != BUFSIZE) errex("read (2)");
   rf = next_random(rf,max);
   if(lseek(fd3,pos,0) != pos) errex("lseek (3)");
   if(read(fd3,rbuf,(RWA3)BUFSIZE) != BUFSIZE) errex("read (3)");
   }
timestamp();
if(close(fd1)) errex("close (1)");
if(close(fd2)) errex("close (2)");
if(close(fd3)) errex("close (3)");

printf("\n");

printf(norep_fmt,"Unlink large test files");
if(unlink("./temp1")) errex("unlink(temp1)");
if(unlink("./temp2")) errex("unlink(temp2)");
timestamp();

exit(0);
}

/*****************************************************************************/

void errex(s) char *s;
{
        FILE *dumpfilep;
        char dumpfile[256];

        printf("\n%s failed; errno = %d\n",s,errno);
        if (debug) {
            sprintf(dumpfile,"/tmp/low.prog1.kernel.dump.%d",getpid());
            dumpfilep = fopen(dumpfile, "w");
            icl_DumpKernel(dumpfilep, NULL, ICL_DUMP_FORMATTED);
            fclose(dumpfilep);
        }
        exit(1);
}

/*****************************************************************************/

long next_random(old,max) long old,max;
{long r = old + 31; return((r > max)? r-max:r);}

/*****************************************************************************/

void timestamp()
{
    if(first_timestamp) {
#if	defined(_SVID_GETTOD)
	gettimeofday(&tv_cur);
#else	/* _SVID_GETTOD */
	gettimeofday(&tv_cur,&tz);
#endif	/* _SVID_GETTOD */
	first_timestamp=FALSE;
    }
    else {
	long nsec,nusec;
	tv_prev = tv_cur;
#if	defined(_SVID_GETTOD)
	gettimeofday(&tv_cur);
#else /* _SVID_GETTOD */
	gettimeofday(&tv_cur,&tz);
#endif /* _SVID_GETTOD */
	nsec = tv_cur.tv_sec - tv_prev.tv_sec;
	nusec = tv_cur.tv_usec - tv_prev.tv_usec;
	printf("%10.1f sec\n",(double)nsec+(double)nusec/1000000.);
    }
}

/*****************************************************************************/
