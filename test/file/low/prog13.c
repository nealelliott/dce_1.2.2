/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: prog13.c,v $
 * Revision 1.1.124.1  1996/10/17  18:29:20  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:54:45  damon]
 *
 * Revision 1.1.119.1  1994/02/04  20:46:53  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:25:15  devsrc]
 * 
 * Revision 1.1.117.1  1993/12/07  17:45:56  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:36:15  jaffe]
 * 
 * Revision 1.1.10.2  1993/07/19  19:43:47  zeliff
 * 	HP port of DFS
 * 	[1993/07/19  18:32:57  zeliff]
 * 
 * Revision 1.1.8.3  1993/07/16  22:03:39  kissel
 * 	Get rid of the damn quotes in #include.
 * 	[1993/06/21  15:09:19  kissel]
 * 
 * Revision 1.1.5.6  1993/03/09  21:22:09  jaffe
 * 	Transarc delta: jess-ot7194-fs-low-kerneldump 1.1
 * 	  Selected comments:
 * 	    Change the icl_KernelDump parameter according to the changes
 * 	    from icl utilities.
 * 	    See above.
 * 	[1993/03/09  20:50:21  jaffe]
 * 
 * Revision 1.1.5.5  1993/01/11  18:33:10  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:14:49  htf]
 * 
 * Revision 1.1.5.4  1992/12/09  21:33:23  jaffe
 * 	Transarc delta: jess-ot6137-fix-low-prog13 1.1
 * 	  Selected comments:
 * 	    The low/prog13 leaves a orphan process running there after the
 * 	    parent cleaning up everything and exiting, which causes the child
 * 	    process calls the errex function to invoke the icl_KernelDump
 * 	    to create the dumpfile. This delta fixes the problem.
 * 	    Fix the problem by letting the parent wait util the child finished.
 * 	[1992/12/07  13:37:47  jaffe]
 * 
 * Revision 1.1.5.3  1992/11/24  21:45:40  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:45:30  bolinger]
 * 
 * Revision 1.1.5.2  1992/10/28  18:00:55  jaffe
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
 * 	[1992/10/28  14:29:27  jaffe]
 * 
 * Revision 1.1.3.5  1992/05/21  12:46:32  jaffe
 * 	Transarc delta: rajesh-fix-tests 1.1
 * 	  Selected comments:
 * 	    Bug fixes to tests
 * 	    Replaced file with version up at OSF
 * 	[1992/05/20  20:34:46  jaffe]
 * 
 * Revision 1.1.3.4  1992/04/20  16:08:05  delgado
 * 	Fix coredump on PMAX.  unlock needs an argument.
 * 	[1992/04/20  16:07:36  delgado]
 * 
 * Revision 1.1.3.3  1992/01/24  04:15:23  devsrc
 * 	Merging dfs6.3
 * 	[1992/01/24  00:23:37  devsrc]
 * 
 * Revision 1.1.3.2  1992/01/23  20:31:07  zeliff
 * 	Moving files onto the branch for dfs6.3 merge
 * 	[1992/01/23  18:48:21  zeliff]
 * 	Revision 1.1.1.3  1992/01/23  22:26:16  devsrc
 * 	Fixed logs
 * 
 * $EndLog$
 */
/*****************************************************************************/
/*       Copyright (C) 1991 Transarc Corporation - All rights reserved       */
/*****************************************************************************/

#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <common.h>
#include <dcedfs/icl.h>
#include <string.h>


#define TIMESTAMP     ((long)(time((time_t *)0)%1000))

#define BUFSZ    256
#define LK_READ  0
#define LK_WRITE 1

char buf[BUFSZ],fn[81],*pf;
int c2p[2],cpid=(-1),debug=TRUE,fd,ncc,npc,p2c[2],pid,pipe_exists=FALSE;
long t1;
struct flock fl;
struct cmd_line {char *name,*arg1,*arg2,*arg3;};
struct cmd_line ccmd[] =
   {
   {"open"}, {"stat"},
   {"sleep", "10"}, {"stat"},
   {"f_read", "This is a test."}, {"stat"},
   {"p_write", "Do not adjust your set."},
   {"p_read", "OK."},
   {"sleep", "2"},
   {"seek", "0"}, {"stat"},
   {"prep_delay"}, {"lock", "w", "b", "0"}, {"check_delay", "6"},
   {"sleep", "2"},
   {"unlock", "0"}, {"close"}, {"stat"}
   };
struct cmd_line pcmd[] =
   {
   {"open"}, {"stat"},
   {"truncate", "0"}, {"stat"},
   {"f_write", "This is a test."}, {"stat"},
   {"p_read", "Do not adjust your set."},
   {"p_write", "OK."},
   {"seek", "0"}, {"stat"},
   {"prep_delay"}, {"lock", "w", "b", "0"}, {"check_delay", "-1"},
   {"sleep", "10"}, {"unlock", "0"},
   {"close"}, {"stat"},
   {"sleep", "4"}
   };

void b_sleep(),errex(),execute(), errex0(),
     f_close(),f_lock(),f_open(),f_read(),f_seek(),
     f_stat(),f_truncate(),f_unlock(),f_write(),
     p_read(),p_write(),prep_delay(),check_delay(),comment();

extern int errno;

/*****************************************************************************/

main(argc,argv) int argc; char *argv[];
{
char *dir1,*dir2;

setbuf(stdout,(char *)NULL);
if(argc != 3) errex0("argument count");
dir1 = argv[1]; dir2 = argv[2];
fd = -1;
ncc = sizeof(ccmd)/sizeof(ccmd[0]);
npc = sizeof(pcmd)/sizeof(pcmd[0]);

printf("Prog13 starting.\n");
printf("Using directories %s and %s\n\n",dir1,dir2);

/* Set up pipes for parent-child communication */
/* p2c is for parent to child; c2p is for child to parent */
/* element[0] is always the read side; element[1] is the write side */
/* so child reads p2c[0], writes c2p[1]; parent reads c2p[0], writes p2c[1] */

if(pipe(p2c) == -1) errex("pipe(1)");
if(pipe(c2p) == -1) errex("pipe(2)");
pipe_exists = TRUE;

if((cpid=fork()) < 0) errex("fork(1)");
pid = getpid();

if(cpid == 0) /* child */
   {
   if(close(c2p[0])) errex("close(1)");
   if(close(p2c[1])) errex("close(2)");
   sprintf(fn,"%s/tfile.13",dir1); pf = "   ";
   if(debug) printf("%sChild starting, PID %d.\n",pf,pid);

   execute(ccmd,ncc);

   if(close(p2c[0])) errex("close(3)");
   if(close(c2p[1])) errex("close(4)");
   printf("%sChild exiting.\n",pf);
   exit(0);
   }

/* parent */
if(close(p2c[0])) errex("close(5)");
if(close(c2p[1])) errex("close(6)");
sprintf(fn,"%s/tfile.13",dir2); pf = "";
if(debug) printf("%sParent starting, PID %d.\n",pf,pid);

execute(pcmd,npc);

if(close(c2p[0])) errex("close(7)");
if(close(p2c[1])) errex("close(8)");
printf("%sParent exiting.\n",pf);
exit(0);
}

/*****************************************************************************/

void execute(c,nc)
struct cmd_line c[];
int nc;
{
char *nm,*a1,*a2,*a3;
int i;

for(i=0; i<nc; ++i)
   {
   nm = c[i].name;
   a1 = c[i].arg1;
   if (!a1) a1 = "";
   a2 = c[i].arg2;
   if (!a2) a2 = "";
   a3 = c[i].arg3;
   if (!a3) a3 = "";
   if(debug) printf("%sPID %d executing %s(%s,%s,%s) at %ld.\n",
      pf,pid,nm,a1,a2,a3,TIMESTAMP);
   if(EQ(nm,"check_delay")) check_delay(atoi(a1));
   else if(EQ(nm,"close")) f_close();
   else if(EQ(nm,"comment")) printf("%s\n",a1);
   else if(EQ(nm,"lock")) f_lock(*a1,*a2,atoi(a3));
   else if(EQ(nm,"open")) f_open();
   else if(EQ(nm,"prep_delay")) prep_delay();
   else if(EQ(nm,"f_read")) f_read(a1);
   else if(EQ(nm,"p_read")) p_read(a1);
   else if(EQ(nm,"seek")) f_seek(atol(a1));
   else if(EQ(nm,"sleep")) b_sleep(atol(a1));
   else if(EQ(nm,"stat")) f_stat();
   else if(EQ(nm,"truncate")) f_truncate(atol(a1));
   else if(EQ(nm,"unlock")) f_unlock(atoi(a1));
   else if(EQ(nm,"f_write")) f_write(a1);
   else if(EQ(nm,"p_write")) p_write(a1);
   else errex0("execute(1)");
   }
}

/*****************************************************************************/

void b_sleep(s)
long s;
{
sleep((unsigned)s);
if(debug) printf("%sPID %d done sleeping at %ld.\n",pf,pid,TIMESTAMP);
}

/**************************************/

void f_close()
{
if(fd < 0) errex0("f_close(1)");
if(close(fd)) errex("close(9)");
fd = -1;
}

/**************************************/

void f_lock(rw_flag,block_flag,nb)
char rw_flag,block_flag;
int nb;
{
int rf;

if(rw_flag!='r' && rw_flag!='w') errex0("f_lock(1)");
if(block_flag!='b' && block_flag!='n') errex0("f_lock(2)");
if(nb < 0) errex0("f_lock(3)");
if(fd < 0) errex0("f_lock(4)");

fl.l_type = (rw_flag == 'r')? F_RDLCK:F_WRLCK;
fl.l_whence = 0;
fl.l_start = lseek(fd,(off_t)0,1);
fl.l_len = nb;
if(block_flag == 'b')
   {
   if(fcntl(fd,F_SETLKW,&fl) == -1) errex("fcntl(1)");
   if(debug) printf("%sPID %d got the lock at %ld.\n",pf,pid,TIMESTAMP);
   }
else
   {
   rf = fcntl(fd,F_SETLK,&fl);
   if(debug)
      {
      if(rf != -1) printf("%sPID %d got the lock at %ld.\n",pf,pid,TIMESTAMP);
      else printf("%sPID %d could not get the lock at %ld.\n",pf,pid,TIMESTAMP);
      }
   }
}

/**************************************/

void f_open()
{
if(fd >= 0) errex0("f_open(1)");
if(!*fn) errex0("f_open(2)");
if((fd=open(fn,O_RDWR|O_CREAT,0644)) < 0) errex("open(1)");
}

/**************************************/

void f_read(should_be)
char *should_be;
{
if(fd < 0) errex0("f_read(1)");

bzero(buf,BUFSZ);
if(read(fd,buf,(RWA3)BUFSZ) <= 0) errex("read(1)");
if(debug) printf("%sPID %d got <%s> at %ld.\n",pf,pid,buf,TIMESTAMP);
if(*should_be && !EQ(buf,should_be)) errex0("f_read(2)");
}

/**************************************/

void f_seek(ofs)
long ofs;
{
if(ofs < -1) errex0("f_seek(1)");
if(fd < 0) errex0("f_seek(2)");

if(ofs >= 0)
   {if(lseek(fd,(off_t)ofs,0) != ofs) errex("lseek(1)");}
else /* offset -1 means seek to EOF */
   {if(lseek(fd,(off_t)0,2) < 0) errex("lseek(2)");}
}

/**************************************/

void f_stat()
{
struct stat s;

if(!*fn) errex0("f_stat(1)");
if(stat(fn,&s)) errex("stat(1)");
if(debug)
   {
   printf("%sPID %d stat(%s,%ld): size is %ld bytes.\n",
      pf,pid,fn,(long)s.st_ino,(long)s.st_size);
   if(fd >= 0)
      {
      printf("%sFile is open; seek offset is %ld.\n",
         pf,(long)lseek(fd,(off_t)0,1));
      }
   else printf("%sFile is closed.\n",pf);
   }
}

/**************************************/

void f_truncate(sz)
long sz;
{
if(!*fn) errex0("f_truncate(1)");
if(truncate(fn,(off_t)sz)) errex("truncate(1)");
}

/**************************************/

void f_unlock(nb)
int nb;
{
if(nb < 0) errex0("f_unlock(1)");
if(fd < 0) errex0("f_unlock(2)");

fl.l_type = F_UNLCK;
fl.l_whence = 0;
fl.l_start = lseek(fd,(off_t)0,1);
fl.l_len = nb;
if(fcntl(fd,F_SETLK,&fl) == -1) errex("fcntl(2)");
if(debug) printf("%sPID %d unlocked the file at %ld.\n",pf,pid,TIMESTAMP);
}

/**************************************/

void f_write(str)
char *str;
{
int len=strlen(str)+1;

if(fd < 0) errex0("f_write(1)");

if(write(fd,str,(RWA3)len) != len) errex("write(1)");
}

/*****************************************************************************/

void p_read(should_be)
char *should_be;
{
bzero(buf,BUFSZ);
if(read(cpid? c2p[0]:p2c[0],buf,(RWA3)BUFSZ) <= 0) errex("read(2)");
if(EQ(buf+strlen(buf)-5,"ERROR")) errex0("p_read(1)");
if(debug) printf("%sPID %d got <%s> at %ld.\n",pf,pid,buf,TIMESTAMP);
if(*should_be && !EQ(buf,should_be)) errex0("p_read(2)");
}

/**************************************/

void p_write(str)
char *str;
{
int len=strlen(str)+1;
if(write(cpid? p2c[1]:c2p[1],str,len) != len) errex("write(2)");
}

/*****************************************************************************/

void prep_delay()
{
t1 = TIMESTAMP;
}

/**************************************/

void check_delay(s)
int s;
{
long d,t2=TIMESTAMP;
if(s == 0) errex0("check_delay(1)");
if(s>0 && (d=t2-t1)<s) errex0("check_delay(2)");
if(s<0 && (d=t2-t1)>(-1*s)) errex0("check_delay(3)");
if(debug) printf("%sPID %d delay was %ld seconds.\n",pf,pid,d);
}

/*****************************************************************************/

void errex(s) char *s;
{
        FILE *dumpfilep;
        char dumpfile[256];

        printf("\n%s failed; pid = %d, errno = %d\n",s,pid,errno);
        if (debug) {
            sprintf(dumpfile,"/tmp/low.prog13.kernel.dump.%d",getpid());
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

        printf("\n%s failed; pid = %d\n",s,pid);
        if (debug) {
            sprintf(dumpfile,"/tmp/low.prog13.kernel.dump.%d",getpid());
            dumpfilep = fopen(dumpfile, "w");
            icl_DumpKernel(dumpfilep, NULL, ICL_DUMP_FORMATTED);
            fclose(dumpfilep);
        }
        exit(1);
}


/*****************************************************************************/
