/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 *      @HP_COPYRIGHT@
 */
/*
 * HISTORY
 * $Log: cma_libc.c,v $
 * Revision 1.1.2.2  1996/10/03  15:17:18  arvind
 * 	Add OSF copyright
 * 	[1996/05/24  23:18 UTC  sommerfeld  /main/DCE_1.2.2/1]
 *
 * Revision 1.1.2.1  1996/06/05  14:20:22  arvind
 * 	Merge fix for CHFts16250.
 * 	[1995/09/07  19:34 UTC  jss  /main/HPDCE02/7]
 * 
 * 	CHFts16250. Fix ordering problems with WC header files.
 * 	Remove function prototype for cma_wait() if we are  __hpux1010p.
 * 	[1995/09/01  14:15 UTC  jss  /main/HPDCE02/jss_wc_thd_tests/1]
 * 
 * 	Merge Spec 1170 changes for Walnut Creek.
 * 	[1995/07/10  20:08 UTC  jss  /main/HPDCE02/6]
 * 
 * 	Change symbol __hpux1010 to hpux1010p.
 * 	[1995/07/10  19:54 UTC  jss  /main/HPDCE02/jss_mothra_cose/3]
 * 
 * 	Change __hpux1010p to __hpux1010.
 * 	[1995/06/28  17:24 UTC  jss  /main/HPDCE02/jss_mothra_cose/2]
 * 
 * 	CHFts15474.  Add new COSE compliant entries to tepv.
 * 	[1995/06/09  19:59 UTC  jss  /main/HPDCE02/jss_mothra_cose/1]
 * 
 * 	Fix the single-threaded tepv, system and sleep are hosed.
 * 	[1995/04/27  21:06 UTC  jss  /main/HPDCE02/5]
 * 
 * 	Merge in changes to CHFts14939 and CHFts14807 from branch jss_mothra_fix3.
 * 	[1995/04/26  20:04 UTC  jss  /main/HPDCE02/4]
 * 
 * 	Fix for CHFts14939. Added new epv and cma__pre_init_libc() to support
 * 	single-threaded client initialization of libc.
 * 	[1995/04/05  22:07 UTC  jss  /main/HPDCE02/jss_mothra_fix3/1]
 * 
 * 	merge from branch jss_mothra_1108 to HPDCE02.
 * 	[1994/11/15  16:09 UTC  jss  /main/HPDCE02/3]
 * 
 * 	Modified tepv to be the same for 9.0 and 10.0. This was done so that libc_r
 * 	could use exc_push_ctx and exc_pop_ctx to fix the cancellation problems.
 * 	[1994/11/09  20:54 UTC  jss  /main/HPDCE02/jss_mothra_1108/1]
 * 
 * 	fix merge problem from hpdce02 merge.
 * 	[1994/07/06  16:47 UTC  jss  /main/HPDCE02/2]
 * 
 * 	merge from jss_dce_threads to HPDCE01
 * 	[1994/02/28  20:01 UTC  jss  /main/HPDCE01/2]
 * 
 * 	Added 3 new _tepv entries exc_push_ctx, exc_pop_ctx, and pthread_ctxcb_hpux.
 * 	[1994/02/28  19:44 UTC  jss  /main/HPDCE01/jss_dce_threads/1]
 * 
 * 	Added two entries to the _tepv structure for libc_r 10.0
 * 	support of language-folks requests.  The functions are
 * 	pthread_create and cma_sigwait, and were declared external
 * 	and added to the end of the _tepv structure.  All this
 * 	was if'd under _hpux1000p.
 * 	[1993/11/24  14:13:20  truitt]
 * 
 * 	*
 * 
 * 	Added three entries to the epv structure for libc_r 10.0
 * 	compatibility.  The functions pthread_setasynccancel,
 * 	pthread_setcancel, and pthread_testcancel were declared
 * 	external and added to the end of the structure.  All this
 * 	was if'd under __hpux1000p.
 * 	[1993/10/26  12:09:15  truitt]
 * 
 * 	Support for 10.0 libc_r exec calls.  Changed the epv
 * 	structure to match the fact that only execve (position
 * 	49) is still active, and the other positions are now
 * 	marked as 0.
 * 	[1993/10/22  15:02:53  truitt]
 * 
 * 	Change use of __hpux100 to __hpux1000p
 * 	[1993/09/22  19:58:27  hopkins]
 * 
 * 	Remove fattach/fdetach code ...
 * 	[1993/08/20  18:37:50  hopkins]
 * 
 * 	Change HPUX_100 define to __hpux100
 * 	[1993/08/11  17:47:03  hopkins]
 * 
 * 	Add entries for sleep() and exec();
 * 	add (conditionally) entries for STREAMS
 * 	_tepv entries and other new _tepv entries
 * 	requested by partners for HPUX 10.0 integration.
 * 	[1993/08/06  19:39:45  hopkins]
 * 
 * 	Initial King Kong branch
 * 	[1993/07/08  17:37:46  root]
 * 
 * 	Initial GAMERA branch
 * 	[1993/03/26  13:00:50  mgm  1.1.3.2]
 * 
 * 	Added declaration and vector for cma_system().
 * 	[1992/12/09  18:32:35  hopkins]
 * 
 * 	HP/UX Porting Changes.
 * 	[1992/06/18  19:08:50  rose_l]
 * 
 * $EndLog$
 */
/*
 * Routines for interfacing to reentrant libc
 */

#include <pthread.h>

/* _tepv is defined in libc */
extern int (**_tepv)();

extern int cma_accept();
extern int cma_close();
extern int cma_connect();
extern int cma_creat();
extern int cma_dup();
extern int cma_dup2();
extern int cma_fcntl();
extern int cma_fork();
extern int cma_ioctl();
extern int cma_msgrcv();
extern int cma_msgsnd();
extern int cma_open();
extern int cma_pipe();
extern int cma_read();
extern int cma_readv();
extern int cma_recv();
extern int cma_recvfrom();
extern int cma_recvmsg();
extern int cma_select();
extern int cma_semop();
extern int cma_send();
extern int cma_sendmsg();
extern int cma_sendto();
extern int cma_setrlimit();
extern int cma_sigaction();
extern int cma_socket();
extern int cma_socketpair();
extern int cma_system();
#if !__hpux1010p
extern int cma_wait();
#endif
extern int cma_wait3();
extern int cma_waitpid();
extern int cma_write();
extern int cma_writev();
extern int hp__mutex_init();

extern int cma_sleep();
extern int cma_execve();

#if __hpux1000p
extern int cma_getmsg();
extern int cma_getpmsg();
extern int cma_poll();
extern int cma_putmsg();
extern int cma_putpmsg();
#endif /* __hpux1000p */
#if __hpux1010p
extern int cma_recvmsg2();
extern int cma_sendmsg2();
extern int cma_socket2();
extern int cma_socketpair2();
#endif /* __hpux1010p */

extern int accept();
extern int close();
extern int connect();
extern int creat();
extern int dup();
extern int dup2();
extern int fcntl();
extern int fork();
extern int ioctl();
extern int msgrcv();
extern int msgsnd();
extern int open();
extern int pipe();
extern int read();
extern int readv();
extern int recv();
extern int recvfrom();
extern int recvmsg();
extern int select();
extern int semop();
extern int send();
extern int sendmsg();
extern int sendto();
extern int setrlimit();
extern int sigaction();
extern int socket();
extern int socketpair();
extern int system();
extern int wait();
extern int wait3();
extern int waitpid();
extern int write();
extern int writev();
extern int hp__mutex_init();

extern int sleep();
extern int execve();

#if __hpux1000p
extern int getmsg();
extern int getpmsg();
extern int poll();
extern int putmsg();
extern int putpmsg();
#endif /* __hpux1000p */
#if __hpux1010p
extern int recvmsg2();
extern int sendmsg2();
extern int socket2();
extern int socketpair2();
#endif /* __hpux1010p */

/*
 * The real pthread_equal is a macro,
 * so we need this routine in order to
 * export pthread_equal via _tepv ...
 */
int hp__pthread_equal();

/*
 * These are present to help libc_r
 * do cancels
 */
extern int pthread_setasynccancel();
extern int pthread_setcancel();
extern void pthread_testcancel();

/*
 * These are present to help the language
 * folks do something or other
 */
extern int pthread_create();
extern int cma_sigwait();
extern void exc_push_ctx();
extern void exc_pop_ctx();
extern void pthread_ctxcb_hpux();

/*
 * Multi-threaded epv
 * The following must be in the same order as
 * given in "tepv.h"
 */
static int (*epv[])() = {
        cma_accept,                 /* 0 */
	cma_close,                  /* 1 */
        cma_connect,                /* 2 */
        cma_creat,                  /* 3 */
        cma_dup,                    /* 4 */
        cma_dup2,                   /* 5 */
        cma_fcntl,                  /* 6 */
        cma_fork,                   /* 7 */
        cma_ioctl,                  /* 8 */
        cma_open,                   /* 9 */
	cma_pipe,                   /* 10 */
        cma_read,                   /* 11 */
        cma_readv,                  /* 12 */
        cma_recv,                   /* 13 */
        cma_recvfrom,               /* 14 */
        cma_recvmsg,                /* 15 */
	cma_select,                 /* 16 */
        cma_send,                   /* 17 */
        cma_sendmsg,                /* 18 */
        cma_sendto,                 /* 19 */
	cma_setrlimit,              /* 20 */
        cma_sigaction,              /* 21 */
        cma_socket,                 /* 22 */
        cma_socketpair,             /* 23 */
        cma_write,                  /* 24 */
        cma_writev,                 /* 25 */
        hp__mutex_init,             /* 26 */
	pthread_mutex_destroy,      /* 27 */
        pthread_mutex_lock,         /* 28 */
        pthread_mutex_unlock,       /* 29 */
        pthread_mutex_trylock,      /* 30 */
        pthread_self,               /* 31 */
        pthread_delay_np,           /* 32 */
	cma_msgrcv,                 /* 33 */
	cma_msgsnd,                 /* 34 */
	cma_semop,                  /* 35 */
	cma_wait,                   /* 36 */
	cma_wait3,                  /* 37 */
	cma_waitpid,                /* 38 */
	cma_fork,                   /* 39 */
	pthread_keycreate,          /* 40 */
	pthread_getspecific,        /* 41 */
	pthread_setspecific,        /* 42 */
	cma_system,                 /* 43 */
	cma_sleep,                  /* 44 __hpux1000p only */
	0,                          /* 45 removed cma_execl */
	0,                          /* 46 removed cma_execle */
	0,                          /* 47 removed cma_execlp */
	0,                          /* 48 removed cma_execv */
	cma_execve,                 /* 49 __hpux1000p only */
	0,                          /* 50 removed cma_execvp */
	0,                          /* 51 removed fattach */
	0,                          /* 52 removed fdetach */
#if __hpux1000p
	cma_getmsg,                 /* 53 __hpux1000p only */
	cma_getpmsg,                /* 54 __hpux1000p only */
	cma_poll,                   /* 55 __hpux1000p only */
	cma_putmsg,                 /* 56 __hpux1000p only */
	cma_putpmsg,                /* 57 __hpux1000p only */
#else
	0,                          /* 53 __hpux1000p only */
	0,                          /* 54 __hpux1000p only */
	0,                          /* 55 __hpux1000p only */
	0,                          /* 56 __hpux1000p only */
	0,                          /* 57 __hpux1000p only */
#endif
	pthread_once,               /* 58 __hpux1000p only */
	hp__pthread_equal,          /* 59 __hpux1000p only */
	pthread_setasynccancel,     /* 60 __hpux1000p only */
	pthread_setcancel,          /* 61 __hpux1000p only */
	pthread_testcancel,         /* 62 __hpux1000p only */
	pthread_create,             /* 63 __hpux1000p only */
	cma_sigwait,                /* 64 __hpux1000p only */
	exc_push_ctx,               /* 65 */
	exc_pop_ctx,                /* 66 */
	pthread_ctxcb_hpux,         /* 67 */
#if __hpux1010p
	cma_recvmsg2,               /* 68 __hpux1010p only */
	cma_sendmsg2,               /* 69 __hpux1010p only */
	cma_socket2,                /* 70 __hpux1010p only */
	cma_socketpair2             /* 71 __hpux1010p only */
#else
	0,                          /* 68 __hpux1010p only */
	0,                          /* 69 __hpux1010p only */
	0,                          /* 70 __hpux1010p only */
	0                           /* 71 __hpux1010p only */
#endif
        };

/*
 * Single threaded epv - This epv will be put in place in _start and
 * and is replaced by the standard epv when hp__init_libc() is executed
 * in cma__init_static2(). This hack is here to support HPUX 10.02 language
 * libraries and should be replaced in subsequent releases with 
 * the atthreadinit() solution.
 * The following must be in the same order as
 * given in "tepv.h"
 */
static int (*st_epv[])() = {
        accept,                     /* 0 */
	close,                      /* 1 */
        connect,                    /* 2 */
        creat,                      /* 3 */
        dup,                        /* 4 */
        dup2,                       /* 5 */
        fcntl,                      /* 6 */
        fork,                       /* 7 */
        ioctl,                      /* 8 */
        open,                       /* 9 */
	pipe,                       /* 10 */
        read,                       /* 11 */
        readv,                      /* 12 */
        recv,                       /* 13 */
        recvfrom,                   /* 14 */
        recvmsg,                    /* 15 */
	select,                     /* 16 */
        send,                       /* 17 */
        sendmsg,                    /* 18 */
        sendto,                     /* 19 */
	setrlimit,                  /* 20 */
        sigaction,                  /* 21 */
        socket,                     /* 22 */
        socketpair,                 /* 23 */
        write,                      /* 24 */
        writev,                     /* 25 */
        hp__mutex_init,             /* 26 */
	pthread_mutex_destroy,      /* 27 */
        pthread_mutex_lock,         /* 28 */
        pthread_mutex_unlock,       /* 29 */
        pthread_mutex_trylock,      /* 30 */
        pthread_self,               /* 31 */
        pthread_delay_np,           /* 32 */
	msgrcv,                     /* 33 */
	msgsnd,                     /* 34 */
	semop,                      /* 35 */
	wait,                       /* 36 */
	wait3,                      /* 37 */
	waitpid,                    /* 38 */
	cma_fork,                   /* 39 */
	pthread_keycreate,          /* 40 */
	pthread_getspecific,        /* 41 */
	pthread_setspecific,        /* 42 */
	system,                     /* 43 */
	cma_sleep,                  /* 44 __hpux1000p only */
	0,                          /* 45 removed cma_execl */
	0,                          /* 46 removed cma_execle */
	0,                          /* 47 removed cma_execlp */
	0,                          /* 48 removed cma_execv */
	cma_execve,                 /* 49 __hpux1000p only */
	0,                          /* 50 removed cma_execvp */
	0,                          /* 51 removed fattach */
	0,                          /* 52 removed fdetach */
#if __hpux1000p
	getmsg,                     /* 53 __hpux1000p only */
	getpmsg,                    /* 54 __hpux1000p only */
	poll,                       /* 55 __hpux1000p only */
	putmsg,                     /* 56 __hpux1000p only */
	putpmsg,                    /* 57 __hpux1000p only */
#else
	0,                          /* 53 __hpux1000p only */
	0,                          /* 54 __hpux1000p only */
	0,                          /* 55 __hpux1000p only */
	0,                          /* 56 __hpux1000p only */
	0,                          /* 57 __hpux1000p only */
#endif
	pthread_once,               /* 58 __hpux1000p only */
	hp__pthread_equal,          /* 59 __hpux1000p only */
	pthread_setasynccancel,     /* 60 __hpux1000p only */
	pthread_setcancel,          /* 61 __hpux1000p only */
	pthread_testcancel,         /* 62 __hpux1000p only */
	pthread_create,             /* 63 __hpux1000p only */
	cma_sigwait,                /* 64 __hpux1000p only */
	exc_push_ctx,               /* 65 */
	exc_pop_ctx,                /* 66 */
	pthread_ctxcb_hpux,         /* 67 */
#if __hpux1010p
	recvmsg2,                   /* 68 __hpux1010p only */
	sendmsg2,                   /* 69 __hpux1010p only */
	socket2,                    /* 70 __hpux1010p only */
	socketpair2                 /* 71 __hpux1010p only */
#else
	0,                          /* 68 __hpux1010p only */
	0,                          /* 69 __hpux1010p only */
	0,                          /* 70 __hpux1010p only */
	0                           /* 71 __hpux1010p only */
#endif
        };

/*
 * hp_mutex_init
 *
 * This routine calls pthread_mutex_init with the default mutex
 * attribute.  We could have called it directly from libc, but
 * that would have meant passing the address of 
 * pthread_mutexattr_default at initialization time.
 */
int
hp__mutex_init(mutex)
pthread_mutex_t *mutex;
{

	return(pthread_mutex_init(mutex, pthread_mutexattr_default));
}


/*
 * hp__init_libc
 *
 * This routine is called by cma__init_static2() to change the _tepv
 * value to the standard multi-threaded vector.
 */
void
hp__init_libc()
{
        _tepv = epv;    
}

/*
 * hp__pre_init_libc
 *
 * Do whatever initializations are necessary for the reentrant libc.
 * This routine is called by _start() and sets the _tepv
 * value to the single-threaded vector.
 * 
 */
void
hp__pre_init_libc()
{
	libc_init(st_epv);
}

/*
 * hp__pthread_equal
 *
 * Make the pthread_equal() macro into a function
 * so it can be exported via _tepv ...
 */
int
hp__pthread_equal(pthread_t t1, pthread_t t2)
{
	return(pthread_equal(t1,t2));
}
