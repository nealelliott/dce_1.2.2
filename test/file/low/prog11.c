/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: prog11.c,v $
 * Revision 1.1.192.1  1996/10/17  18:29:15  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:54:42  damon]
 *
 * Revision 1.1.187.1  1994/02/04  20:46:48  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:25:12  devsrc]
 * 
 * Revision 1.1.185.1  1993/12/07  17:45:52  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:35:42  jaffe]
 * 
 * Revision 1.1.17.2  1993/07/19  19:43:35  zeliff
 * 	HP port of DFS
 * 	[1993/07/19  18:32:48  zeliff]
 * 
 * Revision 1.1.15.3  1993/07/16  22:03:32  kissel
 * 	Put HP RP work on the 1.0.3i branch.
 * 	[1993/06/21  15:08:38  kissel]
 * 
 * Revision 1.1.13.2  1993/06/04  18:35:31  kissel
 * 	Initial HPUX RP version.
 * 	[1993/06/03  22:46:21  kissel]
 * 
 * Revision 1.1.6.10  1993/03/09  21:21:54  jaffe
 * 	Transarc delta: jess-ot7194-fs-low-kerneldump 1.1
 * 	  Selected comments:
 * 	    Change the icl_KernelDump parameter according to the changes
 * 	    from icl utilities.
 * 	    See above.
 * 	[1993/03/09  20:49:00  jaffe]
 * 
 * Revision 1.1.4.2  1993/03/03  15:26:20  kissel
 * 	Made porting changes for HPUX (no flock).
 * 
 * Revision 1.1.6.9  1993/02/16  16:36:27  jaffe
 * 	Transarc delta: jess-ot5657-fix-low-prog11 1.2
 * 	  Selected comments:
 * 	    This delta fixes the error "exception 5b" and deadlock fails. In prog11.c,
 * 	    the test for exception 5b is not well defined because the printf and/or
 * 	    slow machine may just eat up time and make this error accur. The fix here
 * 	    is adjust the time error to 2 (large enough for any slow case). This
 * 	    delta also remove exception 6b test since this test has no general
 * 	    meaning to the testing purpose. The deadlock error may accur in distributed
 * 	    environment as the EDEADLK could not be detected during the running time
 * 	    of a process, which could finally cause the signal (SIGALRM) caught. The
 * 	    fix for this problem is put a larger wait-time number to tolerate
 * 	    slow machine or enviroment.
 * 	    See above.
 * 	    Change check point 5b to be a warning.
 * 	    See above.
 * 	[1993/02/15  19:36:32  jaffe]
 * 
 * Revision 1.1.6.8  1993/01/11  18:33:04  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:14:32  htf]
 * 
 * Revision 1.1.6.7  1992/12/09  21:33:16  jaffe
 * 	Transarc delta: jaffe-fix-2.2-test-build 1.2
 * 	  Selected comments:
 * 
 * 	    Define write and read on OSF/1 also, since they are now cma_read and cma_write.
 * 	[1992/12/07  13:37:28  jaffe]
 * 
 * Revision 1.1.6.6  1992/11/24  21:45:24  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:45:16  bolinger]
 * 
 * Revision 1.1.6.5  1992/10/28  18:00:37  jaffe
 * 	Transarc delta: jess-add-dumpkernel-in-low-tests 1.5
 * 	  Selected comments:
 * 	    Add function call (icl_DumpKernel) in the test suite. In case of running
 * 	    error of C programmes in the test suite the "icl_DumpKernel" function
 * 	    will be called in errex() to do the kernel trace. The dump file is located
 * 	    in "/tmp/prog#.kernel.dump.pid". You have to have the debug flag set to
 * 	    have the dump file.
 * 	    See above.
 * 	    See above.
 * 	    Process another C file and add "low" as dump file header.
 * 	    Turn on the prog3 debug flags.
 * 	    It should not create the zero length dumpfile when there is nothing wrong.
 * 	    Fix the problem.
 * 	[1992/10/28  14:28:21  jaffe]
 * 
 * Revision 1.1.6.4  1992/10/15  20:34:53  marty
 * 	Conditionalize out prototypes for read,write only on OSF.
 * 	[1992/10/15  20:06:49  marty]
 * 
 * Revision 1.1.6.3  1992/10/14  20:43:43  marty
 * 	OSC1.1.1 upgrade. Changes are compatible with osc1.0.4.
 * 	[1992/10/14  20:02:35  marty]
 * 
 * Revision 1.1.7.2  1992/10/08  21:07:03  garyf
 * 	conditionalize out prototypes for read, write
 * 	[1992/09/29  20:11:40  garyf]
 * 
 * Revision 1.1.6.2  1992/09/25  17:24:43  jaffe
 * 	Transarc delta: jess-fix-low-test11-lock 1.2
 * 	  Selected comments:
 * 	    In prog11 the parent process locks a file and forks a child process
 * 	    which will try to lock it. But it is hard to know when the child
 * 	    process will start. In case of the child is up so late it will
 * 	    miss the chance to lock a locked file which results a failure.
 * 	    Fix the problem by putting a synchronize read in the parent process.
 * 	    This will insure the child process is ready to do the lock.
 * 	    Just turn on the debug flag.
 * 	    See above.
 * 	[1992/09/24  12:47:43  jaffe]
 * 
 * Revision 1.1.3.6  1992/06/18  18:54:33  jaffe
 * 	Transarc delta: fred-3448-sync-procs 1.1
 * 	  Selected comments:
 * 	    Now uses pipes in combination with sleep calls to synchronize locking
 * 	    between parent and child processes.
 * 	[1992/06/18  18:45:40  jaffe]
 * 
 * 	Transarc delta: fred-3448-sync-procs 1.1
 * 	  Selected comments:
 * 	    Now uses pipes in combination with sleep calls to synchronize locking
 * 	    between parent and child processes.
 * 	[1992/06/18  18:25:08  jaffe]
 * 
 * Revision 1.1.3.5  1992/05/21  12:46:24  jaffe
 * 	Transarc delta: fred-merge-tests 1.1
 * 	  Selected comments:
 * 	    Created this delta to merge Phil Hirsch's final DFS test code changes with
 * 	    the test code that the OSF had.  Hopefully, this means we and the OSF
 * 	    should be looking at the same test code.
 * 	    Changes to facilitate merge of local DFS test code with the OSF's.
 * 	Transarc delta: rajesh-fix-tests 1.1
 * 	  Selected comments:
 * 	    Bug fixes to tests
 * 	    Replaced file with version up at OSF
 * 	[1992/05/20  20:33:27  jaffe]
 * 
 * Revision 1.1.3.4  1992/04/28  19:35:35  delgado
 * 	Add porting change for OSF1 - basically don't do flock deadlock detection testing since
 * 	flock on OSF1 does not do deadlock detection
 * 	[1992/04/28  19:35:13  delgado]
 * 
 * Revision 1.1.3.3  1992/01/24  04:15:19  devsrc
 * 	Merging dfs6.3
 * 	[1992/01/24  00:23:31  devsrc]
 * 
 * Revision 1.1.3.2  1992/01/23  20:30:54  zeliff
 * 	Moving files onto the branch for dfs6.3 merge
 * 	[1992/01/23  18:48:12  zeliff]
 * 	Revision 1.1.1.3  1992/01/23  22:26:09  devsrc
 * 	Fixed logs
 * 
 * $EndLog$
*/
/*****************************************************************************/
/* Copyright (C) 1990, 1991 Transarc Corporation - All rights reserved */
/*****************************************************************************/

#include <dcedfs/param.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/file.h>     /* flock() except on HPUX */
#include "common.h"
#include <dcedfs/icl.h>
#include <string.h>

#ifdef SUNOS5
#include <sys/statvfs.h>
#include <unistd.h>       /* lockf() */
#endif

#ifdef _AIX
#include <sys/statfs.h>
#include <sys/lockf.h>    /* lockf() */
int alr[] = {0, 1, 2};
#endif

#ifdef __hpux
#include <sys/vfs.h>
#include <unistd.h>       /* lockf() */
int alr[] = {0, 2};	  /* We do not support flock. */
#endif

#ifdef __OSF__
#include <sys/mount.h>
#include <unistd.h>       /* lockf() */
int alr[] = {0, 1, 2};
#endif

#ifdef MIPS
#define DEC
#include <sys/param.h>
#include <sys/mount.h>
#include <unistd.h>       /* lockf() */
int alr[] = {0, 1, 2};
#endif

#ifdef NEXT
#include <sys/vfs.h>
#include <sys/fcntl.h>    /* fcntl(), lockf() */
int alr[] = {0, 1, 2};
#endif

#ifdef RT
#include <sys/vfs.h>
#include <sys/fcntl.h>    /* fcntl(), lockf() */
int alr[] = {0, 1, 2};
#endif

#if defined(SPARC) || defined(sparc)
#include <sys/vfs.h>
#include <unistd.h>       /* lockf() */

#ifdef sparc
int alr[] = {0, 2};
#define LOCK_EX 0         /* flock() doesn't exist, but we have to #define */
#define LOCK_NB 0         /* these symbols so the program will compile. */
#define LOCK_SH 0
#define LOCK_UN 0
int flock() {return 0;}
#endif /* sparc */

#endif

#ifdef SUN3
#include <sys/vfs.h>
#include <unistd.h>       /* lockf() */
int alr[] = {0, 1, 2};
#endif

#ifdef VAX
#define DEC
#include <sys/param.h>
#include <sys/mount.h>
#include <unistd.h>       /* lockf() */
int alr[] = {0, 1, 2};
#endif

/*
 *  Constant definitions.
 */
#define READ_FD    0
#define WRITE_FD   1

#define SYNC_STRING     "Your turn."

#define LOCK_TIMEOUT    150    /* number of seconds to block on locks */
#define LOCK_WAIT_TIME  5      /* # seconds to wait for parent/child  */
                               /* process to attempt a conflicting lock */
                               /* operation. */

/*
 *  Global variables.
 */
int parentPipes[2], childPipes[2];   /* These are file descriptors for the */
                                     /* parent and child to use to */
                                     /* synchronize lock accesses. */

int afsflag, cpid, debug = TRUE;

extern int read(), write();
extern int errno;

struct flock fl;

long br_start[] = {0L, 10L, 20L, 0L};
long br_len[] = {10L, 10L, 10L, 0L};
/* The following will increase runtime by a factor of 16 (160 -> 2700 iters) */
/*
long br_start[] = {0L, 0L, 0L, 0L,0L,9L,10L,10L,10L,10L,19L,20L,20L,20L,29L};
long br_len[] =   {1L,10L,20L,30L,0L,1L, 1L,10L,20L, 0L, 1L, 1L,10L, 0L, 1L};
*/

/*
 *  Private functions.
 */

/*
 *  Conflict() -
 *     Determines if a lock conflict exist.
 */
int Conflict(plt, clt, pbr, cbr)
  int plt, clt, pbr, cbr;
{
    long pst = br_start[pbr], plen = br_len[pbr];
    long cst = br_start[cbr], clen = br_len[cbr];

    if (plt==0 && clt==0)   /* both read locks */
	return(FALSE);
    if (!plen) 
	plen = 1000000L; 
    if (!clen)
	clen = 1000000L;
    /* 
     *  Return TRUE iff the locks overlap .
     */
    return(pst >= cst ? ((cst + clen - 1) >= pst) : ((pst + plen - 1) >= cst));
}


/*
 *  Errex() -
 *     Error printing routine.
 */
void Errex(s)
  char *s;
{
    FILE *dumpfilep;
    char dumpfile[256];
    printf("\n%s failed in %s; errno = %d\n", s, cpid ? "parent" : "child",
	   errno);
    if (debug) {
        sprintf(dumpfile,"/tmp/low.prog11.kernel.dump.%d",getpid());
        dumpfilep = fopen(dumpfile, "w");
        icl_DumpKernel(dumpfilep, NULL, ICL_DUMP_FORMATTED);
        fclose(dumpfilep); 
    }
    exit(1);
}

void Errex0(s)
  char *s;
{
    FILE *dumpfilep;
    char dumpfile[256];
    printf("\n%s failed\n", s);
    if (debug) {
        sprintf(dumpfile,"/tmp/low.prog11.kernel.dump.%d",getpid());
        dumpfilep = fopen(dumpfile, "w");
        icl_DumpKernel(dumpfilep, NULL, ICL_DUMP_FORMATTED);
        fclose(dumpfilep); 
    }
    exit(1);
}

/*
 *  AlarmSounded() -
 *     Signal hanling routine for alarms set around locks.
 */
void AlarmSounded()
{
    Errex("Deadlock avoidance(1)");
}

/*
 *  Do_Lock() -
 *     Sets file and byte range locks.
 */

int Do_Lock(lr, fd, rwflag, brange, bflag, expect)
  int lr;      /* locking routine to use */
  int fd;      /* file descriptor to lock */
  int rwflag;  /* 0=read, else write */
  int brange;  /* byte range to lock (index into br_start[] and br_len[]) */
  int bflag;   /* 0=non-blocking, else blocking */
  int expect;  /* TRUE iff we expect to get the lock immediately */
{
    int did_block = FALSE, rf;
    long len;
    off_t start;
    time_t t1 = time((time_t *)0), t2;

    start = br_start[brange]; len = br_len[brange];
    if (debug)
	printf("   %s locking %d (%s, %s, start %ld, len %ld) at %ld; expect=%s.\n",
	       cpid ? "Parent" : "Child",fd,rwflag ? "write":"read",
	       bflag ? "block" : "non-block",
	       (long) start, len, (t1 % 1000), expect ? "YES" : "NO");

#ifndef sun
    (void) alarm((unsigned int)(LOCK_TIMEOUT));
#endif /* not sun */
    if (lr == 0) /* fcntl */
    {
	fl.l_type = rwflag ? F_WRLCK:F_RDLCK;
	fl.l_whence = 0;
	fl.l_start = start; 
	fl.l_len = len;
	if (fcntl(fd,F_GETLK,&fl) == -1)
	    Errex("fcntl(1)");
	if (expect && fl.l_type!=F_UNLCK)
	    Errex0("expectation(1)");
	else {
	    if (!expect && fl.l_type==F_UNLCK)
		Errex0("expectation(2)");
	}

	fl.l_type = rwflag ? F_WRLCK : F_RDLCK;
	fl.l_whence = 0;
	fl.l_start = start; 
	fl.l_len = len;
	if (bflag) {
	    if ((rf = (fcntl(fd, F_SETLKW, &fl) == -1)) && errno != EDEADLK)
		Errex("fcntl(2)");
	}
	else {
	    while(fcntl(fd, F_SETLK, &fl) == -1)
		did_block=TRUE;
	}
   }

#ifndef __hpux
    if (lr == 1) /* flock (entire file only, can't test) */
    {
	if (bflag)
	{
	    if ((rf=flock(fd, rwflag ? LOCK_EX : LOCK_SH)) && errno != EDEADLK)
		Errex("flock(1)");
	}
	else {
	    while(flock(fd, ((rwflag ? LOCK_EX : LOCK_SH) | LOCK_NB)))
		did_block=TRUE;
	}
    }
#endif /* __hpux */

    if (lr == 2) /* lockf (writelocks only, always blocks) */
    {
	if (lseek(fd, start, 0) != start)
	    Errex("lseek(1)");
	rf = lockf(fd, F_TEST, len);
	if (expect && rf) 
	    Errex("expectation(3)");
	else {
	    if (!expect && rf != -1) 
		Errex("expectation(4)");
	}

	if (lseek(fd, start, 0) != start)
	    Errex("lseek(2)");
	if ((rf = lockf(fd, F_LOCK, len)) && errno != EDEADLK)
	    Errex("lockf(1)");
    }

    t2 = time((time_t *)0);
    if (debug)
    {
	if (!bflag || !rf)
	    printf("   %s got lock on %d at %ld.\n",
		   cpid ? "Parent" : "Child", fd, (t2 % 1000));
	else  /* bflag && rf => supposed to block, but the call failed */
	    printf("   %s did not get lock at %ld.\n",
		   cpid ? "Parent" : "Child", (t2 % 1000));
    }

    if (expect)
    {
	if (!bflag && did_block)
	    Errex0("expectation(5a)");
	if (t2-t1 > 2)
	    printf("WARNING: It was slow to get the lock");
    }
    else /* !expect */
    {
	if (!bflag && !did_block)
	    Errex0("expectation(6a)");
    }

    return(TRUE);
}

/*
 *  Do_Unlock() -
 *     Release a lock held on a file or byte range.
 */
int Do_Unlock(lr, fd)
  int lr, fd;
{
    if (debug)
	printf("   %s releasing lock on %d at %ld.\n",
	       cpid ? "Parent"  :"Child", fd, (time((time_t *)0) % 1000));

    if (lr == 0) /* fcntl */
    {
	fl.l_type = F_UNLCK;
	fl.l_whence = 0;
	fl.l_start = 0L; 
	fl.l_len = 0L;
	if (fcntl(fd, F_SETLK, &fl) == -1)
	    Errex("fcntl(3)");
    }

#ifndef __hpux
    if (lr == 1) /* flock */
    {
	if (flock(fd, LOCK_UN))
	    Errex("flock(2)");
    }
#endif /* __hpux */

    if (lr == 2) /* lockf */
    {
	if (lseek(fd, (off_t)0, 0) != 0)
	    Errex("lseek(3)");
	if (lockf(fd, F_ULOCK, 0L) != 0)
	    Errex("lockf(2)");
    }

    return(TRUE);
}


/*
 *  Synchronize() -
 *     Issues I/O operations on the pipes set up between the parent and
 *     child process, to synchronize their locking operations.
 */
void Synchronize(procId, ioFunc, fileDesc)
  int procId;        /* parent/child process identifier */
  int (*ioFunc)();   /* I/O function to execute, read or write */
  int fileDesc;      /* file descriptor for I/O operation */
{
    char readBuf[sizeof(SYNC_STRING) + 1];
    char *ioBuffer;
    int ioBytes = strlen(SYNC_STRING);
    int ioReturnVal;

    ioBuffer = (ioFunc == (int(*)())read) ? readBuf : SYNC_STRING;
    if (((ioReturnVal = ioFunc(fileDesc, ioBuffer, ioBytes)) == -1) ||
	ioReturnVal != ioBytes){
	printf("Pipe %s failed in %s, errno = %d\n",
	      (ioFunc == (int(*)())read) ? "read" : "write",
	      (procId == 0) ? "child" : "parent", errno);
	exit(1);
    }
}

main(argc,argv) 
  int argc;
  char *argv[];
{
    char wd[256];
    int cnt,fd,fd2,i,rf,st;
    int bflag,lr,plt,plt2,clt,pbr,cbr;

#ifdef DEC
struct fs_data fs;
#else
#ifdef SUNOS5
struct statvfs fs;
#else
struct statfs fs;
#endif
#endif

    debug = 1;

    setbuf(stdout,(char *)NULL);
#ifndef sun
    (void)signal(SIGALRM,AlarmSounded);
#endif /* not sun */
    
    printf("Prog11 starting.\n\n");

    (void)getwd(wd);
    afsflag = (strncmp(wd,"/afs/",5) == 0);

    printf("This is %s an AFS filesystem.\n\n",afsflag ? "" : " NOT");

    system("echo '12345678901234567890123456789' > tfile.11");
    system("echo '12345678901234567890123456789' > tfile2.11");

    /*
     *  Set up the pipes to synchronize the parent and child processes
     *  during locking.
     */
    if (pipe(parentPipes) != 0) {
	printf("Opening of parent pipes failed, errno = %d\n", errno);
	exit(1);
    }
    if (pipe(childPipes) != 0) {
	printf("Opening of child pipes failed, errno = %d\n", errno);
	exit(1);
    }

    cnt = 0;
    for(bflag=0; bflag<=1; ++bflag) { /* block flag: 0=no, 1=block */
	/* 
	 *  Cycle through the lock routines.
	 *  alr[] = available lock routines. 0=fcntl, 1=flock, 2=lockf
	 */
	for(i=0; i<sizeof(alr)/sizeof(alr[0]); ++i) {
	    /*
	     *  Cycle through the parent byte ranges.
	     */
	    for(pbr=0; pbr<sizeof(br_len)/sizeof(br_len[0]); ++pbr) {
		/*
		 *  Cycle through the child byte ranges.
		 */
		for(cbr=0; cbr<sizeof(br_len)/sizeof(br_len[0]); ++cbr) {
		    /*
		     *  Cycle through parent lock types,  0=read, 1=write
		     */
		    for(plt=0; plt<=1; ++plt) {
			/*
			 *  Cycle through child lock types,  0=read, 1=write
			 */
			for(clt=0; clt<=1; ++clt) {
			    lr = alr[i];
#ifdef _AIX
			    if (afsflag) continue;
#endif
#ifdef DEC
			    if (afsflag) continue;
#endif
#ifdef RT
			    if (afsflag) continue;
#endif
#ifdef SPARC
			    if (afsflag && (lr==0 || lr==2)) continue;
#endif
#ifdef __hpux
			    if (afsflag) continue;
#endif
			    if (lr==1 && (br_start[pbr] || br_len[pbr] || 
					 br_start[cbr] || br_len[cbr]))
				continue;
			    if (lr==2 && (!bflag || plt==0 || clt==0))
				continue;
			    /* 
			     *  Change readlock to write, or vice versa.
			     */
			    plt2 = (lr == 2) ? plt : 1 - plt; 

			    ++cnt;
			    printf("%d: block=%s, lr=%s, pbr=(%ld+%ld), cbr=(%ld+%ld), plt=%s, clt=%s.\n",
				   cnt, bflag? "TRUE":"FALSE",
				   lr? (lr==1? "flock":"lockf"):"fcntl",
				   br_start[pbr], br_len[pbr], br_start[cbr],
				   br_len[cbr],
				   plt? "write":"read",clt? "write":"read");

			    if ((cpid=fork()) < 0) Errex("fork(1)");

			    if (cpid) /* parent */
			    {
                                errno=0;
				if ((fd=open("tfile.11",O_RDWR)) < 0) 
				    Errex("open(1)");

				rf = Do_Lock(lr,fd,plt,pbr,bflag,TRUE);
				Synchronize(cpid, write, 
					    parentPipes[WRITE_FD]);
				Synchronize(cpid, read, childPipes[READ_FD]);

				/*
				 *  Sleep so child can issue lock call and
				 *  possibly block.
				 */
				sleep(LOCK_WAIT_TIME);
				rf = Do_Unlock(lr, fd);

				Synchronize(cpid, write,
					    parentPipes[WRITE_FD]);
				Synchronize(cpid, read, childPipes[READ_FD]);

				rf = Do_Lock(lr, fd, plt, pbr, bflag,
					     !Conflict(plt, clt, pbr, cbr));

				Synchronize(cpid, read, childPipes[READ_FD]);

				if (plt2 != plt)
				{
				    /*
				     *  Up/down grade the lock type, from read
				     *  to write or vise versa.
				     */
				    if (debug) 
					printf("  Parent ready to try second lock.\n");
				    rf = Do_Lock(lr, fd, plt2, pbr, bflag, 
						 TRUE);
				    Synchronize(cpid, write, 
						parentPipes[WRITE_FD]);
				    sleep(LOCK_WAIT_TIME);
				}

				rf = Do_Unlock(lr, fd);

				if ((rf = wait(&st)) == -1 || st != 0) 
				    Errex("wait(1)");
			    }
			    else /* child */
			    {
                                errno=0;

				if ((fd=open("tfile.11", 
					    clt? O_WRONLY:O_RDONLY)) < 0)
				    Errex("open(2)");

				Synchronize(cpid, read, parentPipes[READ_FD]);
				Synchronize(cpid, write, childPipes[WRITE_FD]);

				rf = Do_Lock(lr, fd, clt, cbr, bflag,
					     !Conflict(plt,clt,pbr,cbr));

				Synchronize(cpid, read, parentPipes[READ_FD]);
				Synchronize(cpid, write, childPipes[WRITE_FD]);
				sleep(LOCK_WAIT_TIME);

				rf = Do_Unlock(lr, fd);

				Synchronize(cpid, write, childPipes[WRITE_FD]);

				if (plt2 != plt)
				{
				    if (debug) 
					printf("  Child ready to try second lock.\n");
				    Synchronize(cpid, read, 
						parentPipes[READ_FD]);

				    rf = Do_Lock(lr, fd, clt, cbr, bflag,
						 !Conflict(plt2,clt,pbr,cbr));

				    rf = Do_Unlock(lr, fd);
				}
				exit(0);
			    }

			    if (debug) 
				printf("\n");
			    if (close(fd)) 
				Errex("close(1)");
			}
		    }
		}
	    }
	}
    }

    /*
     * Deadlock tests.
     *
     * Cycle through locking routines to use.
     */
    for(i=0; i<sizeof(alr)/sizeof(alr[0]); ++i) { 
	lr = alr[i];

#ifdef _AIX
	if (afsflag) continue;
#endif
#ifdef DEC
	if (afsflag) continue;
	if (lr == 1) continue;
#endif
#ifdef RT
	if (afsflag) continue;
#endif
#ifdef SPARC
	if (afsflag && (lr==0 || lr==2)) continue;
	if (lr == 1) continue;
#endif
#ifdef SUN3
	if (lr == 1) continue;
#endif
#ifdef __OSF__
	if (lr == 1) continue;
#endif
#ifdef __hpux
	if (afsflag) continue;
	if (lr == 1) continue;
#endif

	printf("Deadlock test, lr=%s.\n",lr? (lr==1? "flock":"lockf"):"fcntl");

	if ((cpid=fork()) < 0) Errex("fork(2)");

	if (cpid) /* parent */
	{
            errno=0;

	    if ((fd=open("tfile.11",O_RDWR)) < 0) 
		Errex("open(3)");
	    if ((fd2=open("tfile2.11",O_RDWR)) < 0)
		Errex("open(4)");

	    rf = Do_Lock(lr, fd, 1, 0, TRUE, TRUE);
	    Synchronize(cpid, write, parentPipes[WRITE_FD]);
	    Synchronize(cpid, read, childPipes[READ_FD]);

	    /* 
	     *  We should get this lock, but only AFTER the child's second
	     *  lock attempt fails due to the deadlock.  So this call will
	     *  block until the child unlocks this lock.
	     */
	    Synchronize(cpid, write, parentPipes[WRITE_FD]);
	    rf = Do_Lock(lr, fd2, 1, 0, TRUE, FALSE);

	    rf = Do_Unlock(lr, fd);
	    rf = Do_Unlock(lr, fd2);
	}
	else /* child */
	{
            errno=0;

	    if ((fd=open("tfile.11",O_RDWR)) < 0)
		Errex("open(3)");
	    if ((fd2=open("tfile2.11",O_RDWR)) < 0) 
		Errex("open(4)");

	    Synchronize(cpid, read, parentPipes[READ_FD]);

	    rf = Do_Lock(lr, fd2, 1, 0, TRUE, TRUE);
	    Synchronize(cpid, write, childPipes[WRITE_FD]);
	    Synchronize(cpid, read, parentPipes[READ_FD]);
	    sleep(LOCK_WAIT_TIME);

	    /* 
	     *  We are now in a deadlock situation.  This call will fail
	     *  because the parent holds this lock and is already attempting
	     *  to obtain the lock that we have.
	     */
	    rf = Do_Lock(lr, fd, 1, 0, TRUE, FALSE);

	    rf = Do_Unlock(lr, fd2);

	    exit(0);
	}

	if (close(fd))
	    Errex("close(2)");
	if (close(fd2))
	    Errex("close(3)");
    }

    if (unlink("tfile.11"))
	Errex("unlink(1)");
    if (unlink("tfile2.11"))
	Errex("unlink(2)");
    printf("\nProg11 exiting.\n");

    exit(0);
}
