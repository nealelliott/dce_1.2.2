/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: prrb_sig_005.c,v $
 * Revision 1.1.69.2  1996/02/17  23:31:38  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:48:07  marty]
 *
 * Revision 1.1.69.1  1995/12/11  23:02:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:22  root]
 * 
 * Revision 1.1.67.1  1993/10/12  19:37:25  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:56  rhw]
 * 
 * Revision 1.1.5.3  1993/02/01  21:46:14  hinman
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  17:07:25  hinman]
 * 
 * Revision 1.1.5.2  1992/12/30  16:53:38  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:11:53  zeliff]
 * 	Revision 1.1.3.2  1993/01/11  16:17:11  hinman
 * 	[hinman] - Check in merged SNI version
 * 
 * Revision 1.1.2.2  1992/06/02  14:33:58  bolinger
 * 	Fix OT defect 3953:  add testcase from defect 2886 to functional
 * 	test suite.
 * 	[1992/06/02  14:33:24  bolinger]
 * 
 * $EndLog$
 */
/*
 * Test to ensure fix is in place to handle signals reliably in
 * a multi-threaded CMA application.  The problem cited in the
 * test description below occurred when one thread in the app
 * exited, causing an outdated process-wide signal mask to be
 * restored.  The solution is to avoid saving/restoring the mask
 * during exception processing, which CMA has now adopted.
 *
 * Failure description:
 *
 * sigchild_test.c -- This test creates two threads and joins with them.
 * If you add a pthread_yield() call after the first pthread_create(),
 * then the test works successfully.  Otherwise, the program hangs.
 *
 * The first of the two threads blocks SIGCHLD and creates a thread to
 * handle this signal via sigwait().  It then creates a pipe, forks off a
 * child process, reads a message off the pipe, then joins with the
 * sigwait thread.  The child process pauses for 1 second (to ensure that
 * the parent blocks), writes a message on the pipe, and exits.
 *
 * The second thread simply creates a few new threads and exits.
 * Mysteriously, when this thread is not created, the program works fine.
 */

#include <sys/types.h>
#include <sys/file.h>
#include <signal.h>
#include <stdio.h>
#include <pthread.h>
#include <dce/cts_test.h>

#define PRINT(msg)	cts_comment(msg)

#define ERROR(msg)	{ cts_failed(msg); exit(1); }

void *DummyThread(arg)
  void *arg;
{
    int 	val = (int)arg;
    int		result;
    pthread_t 	th;

    PRINT("entered DummyThread");

    if (val != 0) {
	if (pthread_create(&th,pthread_attr_default,
			   DummyThread,(void *)(val-1)) == -1)
	    ERROR("create of DummyThread failed");

	if (pthread_join(th,(void *)&result) == -1)
	    ERROR("joining with DummyThread");
	if (result != 0)
	    ERROR("bad value from DummyThread exit");
    }

    pthread_exit(0);
}

void *WaitThread(arg)
  void *arg;
{
    int		sig;
    sigset_t	set;

    PRINT("entered WaitThread");

    sigemptyset(&set);
    sigaddset(&set,SIGCHLD);

    if ((sig = sigwait(&set)) == -1)
	ERROR("sigwait failed");

    if (sig != SIGCHLD)
	ERROR("receive non-SIGCHLD signal");

    PRINT("received SIGCHLD in WaitThread");
    return(0);
}

void *SignalThread(arg)
  void *arg;
{
    int		result;
    int		fd[2],pid;
    char	buf[40];
    sigset_t	set;
    pthread_t	th;

    PRINT("entered SignalThread");

    sigemptyset(&set);
    sigaddset(&set,SIGCHLD);

    if (sigprocmask(SIG_BLOCK,&set,NULL) != 0)
	ERROR("sigprocmask in SignalThread");

    if (pthread_create(&th,pthread_attr_default,WaitThread,0) == -1)
	ERROR("create of WaitThread failed");

    if (pipe(fd) < 0)
	ERROR("pipe failed");

    PRINT("created pipe");

    if ((pid = fork()) < 0)
	ERROR("fork failed");
    if (pid == 0) {
	/* in child, simulate a delay to force parent to block in read */
	sleep(1);
	if (write(fd[1],"This is a test, it is only a test\n",34) < 0)
	    ERROR("failure in write");
	exit(0);
    }

    if ((result = read(fd[0],buf,40)) < 0)
	ERROR("read failed");
#if (_CMA_UNIX_TYPE == _CMA__SVR4)
	/*
	*	Must null-terminate string received from read(2).
	*/
	buf[result] = '\0';
#endif
    cts_comment("read returned: %35s",buf);

    if (pthread_join(th,(void *)&result) == -1)
	ERROR("joining with threads");
    if (result != 0)
	ERROR("bad value from thread exit");

    return(0);
}

/*
 * main() -- main routine of test program.  Reads the command line 
 *     to determine the test conditions and starts the test threads.
 */
main(argc,argv)
  int argc;
  char **argv;
{
    int i,result;
    pthread_t th[2];
    
    cts_test ("PRRB_SIG_005", "Test SIGCHLD in multi-threaded parent");

    if (pthread_create(&th[0],pthread_attr_default,SignalThread,0) == -1)
	ERROR("create of SignalThread failed");

#ifdef	ADD_YIELD
    pthread_yield();
#endif	/* ADD_YIELD */

    if (pthread_create(&th[1],pthread_attr_default,DummyThread,(pthread_addr_t)3) == -1)
	ERROR("create of DummyThread failed");
		       
    for (i=0; i<2; i++) {
	if (pthread_join(th[i],(void *)&result) == -1)
	    ERROR("joining with threads");
	if (result != 0)
	    ERROR("bad value from thread exit");
    }

    PRINT("end of test");

    cts_result();
    exit(0);
}
