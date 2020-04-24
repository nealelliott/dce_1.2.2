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
 * $Log: cma_hpux.c,v $
 * Revision 1.1.2.2  1996/10/03  15:17:06  arvind
 * 	Add OSF copyright
 * 	[1996/05/24  23:18 UTC  sommerfeld  /main/DCE_1.2.2/1]
 *
 * 	Add dale tonogai's cma_ioctl fix.
 * 	[1992/11/12  18:27:55  hopkins]
 *
 * 	HP/UX Porting Changes.
 * 	[1992/06/18  19:08:45  rose_l]
 *
 * Revision 1.1.2.1  1996/06/05  14:20:04  arvind
 * 	Merge fixes for CHFts16291 and CHFts16294.
 * 	[1995/09/14  20:10 UTC  jss  /main/HPDCE02/9]
 * 
 * 	Conditionalize include of stropts.h
 * 	[1995/09/14  14:47 UTC  jss  /main/HPDCE02/jss_wc_thd_tests/2]
 * 
 * 	Fix for CHFts16291. Do not allow cma__g_mx_file to be larger than FD_SETSIZE.
 * 	Fix for CHFts16294. ioctl wrapper needs to be modified to handle thread-sync I/O.
 * 	[1995/09/13  20:35 UTC  jss  /main/HPDCE02/jss_wc_thd_tests/1]
 * 
 * 	Merge Spec 1170 changes for Walnut Creek.
 * 	[1995/07/10  20:08 UTC  jss  /main/HPDCE02/8]
 * 
 * 	Change symbol __hpux1010 to hpux1010p.
 * 	[1995/07/10  19:54 UTC  jss  /main/HPDCE02/jss_mothra_cose/2]
 * 
 * 	Fix for CHFts15603. Signal changes to support WC kernel Spec. 1170 changes.
 * 	[1995/06/28  17:24 UTC  jss  /main/HPDCE02/jss_mothra_cose/1]
 * 
 * 	Fix single-threaded tepv problems with sleep.
 * 	[1995/04/27  21:05 UTC  jss  /main/HPDCE02/7]
 * 
 * 	Merge cma_setrlimt fix from branch jss_mothra_fix2.
 * 	[1995/03/13  16:52 UTC  jss  /main/HPDCE02/6]
 * 
 * 	Single threaded client changes.  Conditionalize cma_setrlimit().
 * 	[1995/03/10  16:08 UTC  jss  /main/HPDCE02/jss_mothra_fix2/2]
 * 
 * 	Merge from branch jss_mothra_fix2.
 * 	[1995/02/16  15:44 UTC  jss  /main/HPDCE02/5]
 * 
 * 	Fix cma_system() to block only the calling thread and not the entire process.
 * 	[1995/02/13  17:02 UTC  jss  /main/HPDCE02/jss_mothra_fix2/1]
 * 
 * 	Merge from branch jss_single_thread.
 * 	[1994/12/22  20:01 UTC  jss  /main/HPDCE02/4]
 * 
 * 	Changes for single threaded client.  Conditionalize wrapper usage based on whetherCMA is initialized.
 * 	[1994/12/22  19:24 UTC  jss  /main/HPDCE02/jss_single_thread/1]
 * 
 * 	Merge from branch jss_close_fix.
 * 	[1994/09/08  19:06 UTC  jss  /main/HPDCE02/3]
 * 
 * 	Fix for CHFts12442. Close while I/O pending bug.
 * 	[1994/08/30  14:23 UTC  jss  /main/HPDCE02/jss_fix_close/1]
 * 
 * 	code cleanup:, removed  _CMA_PROTO_ from function definitions
 * 	[1994/07/06  16:47 UTC  jss  /main/HPDCE02/2]
 * 
 * 	[1994/03/23  19:49 UTC  jss  /main/HPDCE01/4]
 * 
 * 	Changed 10.0 shell variable to point to /sbin/sh for the POSIX shell.
 * 	[1994/03/23  19:40 UTC  jss  /main/HPDCE01/jss_rodan_0318/1]
 * 
 * 	Merge from jss_fix_ioctl to HPDCE01
 * 	[1994/03/18  14:37 UTC  jss  /main/HPDCE01/3]
 * 
 * 	fix for CHFts11106. The FIONBIO flag was not recognized
 * 	by cma_ioctl().  Modify cma_ioctl() to handle FIONBIO
 * 	in the same way that it currently handles FIOSNBIO.
 * 	[1994/03/14  16:54 UTC  jss  /main/HPDCE01/jss_fix_ioctl/1]
 * 
 * 	merge from jss_rodan_ic1 to HPDCE01
 * 	[1994/03/08  20:34 UTC  jss  /main/HPDCE01/2]
 * 
 * 	Conditionalize SHELL variable so that it points to the correct shell
 * 	on 9.0 and 10.0 systems.
 * 	[1994/03/08  18:53 UTC  jss  /main/HPDCE01/jss_rodan_ic1/1]
 * 
 * 	Added the structure _ctxcb_spec_t to initialize the callback
 * 	information for C++ callbacks.  Also added the function
 * 	pthread_ctxcb_hpux() to take care of loading any user
 * 	information into the callback structure.
 * 	[1993/11/09  18:42:25  truitt]
 * 
 * 	*
 * 
 * 	The cma_sleep() call would core dump when passed a negative
 * 	number.  Changed the parameter passed in from unsigned int
 * 	to int and made a stronger check in the body of the function
 * 	for a proper parameter.
 * 	[1993/10/21  18:30:04  truitt]
 * 
 * 	Initial King Kong branch
 * 	[1993/07/08  17:32:51  root]
 * 
 * 	initialized wblk.queue in HP_WAIT using
 * 	cma__queue_zero(), so that we don't get
 * 	assertion failures from cma__queue_insert()
 * 	now that it requires flink and blink for
 * 	inserted element to be zero.
 * 	[1993/04/09  18:00:53  hopkins]
 * 
 * 	Initial GAMERA branch
 * 	[1993/03/26  12:59:52  mgm  1.1.5.2]
 * 
 * 	Add sleep wrapper, cma_sleep, that calls
 * 	pthread_delay_np.
 * 	[1993/01/04  16:10:22  hopkins]
 * 
 * 	Add dale tonogai's changes to cma___hp_sigchld
 * 	to raise an exception if invoked when null thread
 * 	is in cma__io_available.  Part of fix for signal
 * 	race condition where cma__io_available checks
 * 	defer queue, but blocks forever on select if
 * 	a signal comes in between checking and selecting.
 * 	Also added cma_system, which is exactly like the
 * 	libc system but does not block SIGCHLD (it would
 * 	otherwise never return because waitpid would never
 * 	return).
 * 	[1992/12/07  21:59:22  hopkins]
 * 
 * 	Add dale tonogai's cma_ioctl fix.
 * 	[1992/11/12  18:27:55
 * 
 * $EndLog$
 */
/*
 * Wrappers for HP-UX system calls
 */

#include <cma.h>
#include <cma_thdio_defs.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/resource.h>
#include <sys/ioctl.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/wait.h>
#if __hpux1010p
# include <sys/stropts.h>
#endif

#ifdef __hpux
extern EXCEPTION	io_avail_exc;
extern cma_t_integer	io_avail_null_thread;
#endif

/*
 * Local macros
 */

/* secs to wait between polls - currently set to the length of 1 time-slice */
#define POLLWAIT	0.2

/*
 * macros for manipulating semop bitmap array
 */
/* number of bits per unsigned integer type */
#define SEMAP_BITS	(sizeof(unsigned int) * 8)

/* number of elements in IPC_NOWAIT bitmap array */
#define SEMAP_LEN	((SEMOPM+SEMAP_BITS-1) / SEMAP_BITS)

/* set bit n in mask m */
#define SEMAP_SET(m,n)	((m)[(n) / SEMAP_BITS] |= (1 << ((n) % SEMAP_BITS)))

/* is bit n in mask m set ? */
#define SEMAP_ISSET(m,n) ((m)[(n) / SEMAP_BITS] & (1 << ((n) % SEMAP_BITS)))

/*
 * Macro to implement wait(), wait3() and waitpid() system calls
 *
 * NOTE: We really should be using a mutex to protect accesses
 *	to cma___g_hpwait_queue, but since we're already blocking
 *	SIGCHLD we may as well block SIGVTALRM.  This gets us the
 *	atomicity we require.
 */
#define HP_WAIT(waitcall)						\
{					  				\
    pid_t	ret_val;						\
    sigset_t	sig_mask;						\
    _CMA_VOLATILE_ cma___t_hpwait_block	wblk;				\
    wblk.tcb = cma__get_self_tcb();					\
    wblk.cv  = wblk.tcb->tswait_cv;					\
    wblk.waiting = 1;							\
    if (sigprocmask(SIG_BLOCK, &cma___g_hpwait_sigmask, &sig_mask) == -1) \
	cma__bugcheck("cma_wait:1");					\
    if ((ret_val = (waitcall)) != 0) {					\
	if (sigprocmask(SIG_SETMASK, &sig_mask, (sigset_t *)NULL) == -1)\
	    cma__bugcheck("cma_wait:2");				\
	return ret_val;							\
    }									\
    cma__queue_zero((cma__t_queue *)&wblk.queue);			\
    cma__queue_insert((cma__t_queue *)&wblk.queue, &cma___g_hpwait_queue); \
    if (sigprocmask(SIG_SETMASK, &sig_mask, (sigset_t *)NULL) == -1) 	\
	cma__bugcheck("cma_wait:3");					\
    cma__int_lock(wblk.tcb->tswait_mutex);				\
    TRY {								\
	while (1) {							\
	    cma__int_wait(wblk.tcb->tswait_cv, wblk.tcb->tswait_mutex); \
	    if (sigprocmask(SIG_BLOCK, &cma___g_hpwait_sigmask, &sig_mask) == -1)\
		cma__bugcheck("cma_wait:4");				\
	    if ((ret_val = (waitcall)) != 0) break;			\
	    wblk.waiting = 1;						\
	    if (sigprocmask(SIG_SETMASK, &sig_mask, (sigset_t *)NULL) == -1) \
		cma__bugcheck("cma_wait:5");				\
	}								\
	cma__queue_remove(&wblk.queue);					\
	if (sigprocmask(SIG_SETMASK, &sig_mask, (sigset_t *)NULL) == -1) \
	    cma__bugcheck("cma_wait:5.5");				\
	cma__int_unlock(wblk.tcb->tswait_mutex);			\
    }									\
    CATCH_ALL {								\
	if (sigprocmask(SIG_BLOCK, &cma___g_hpwait_sigmask, &sig_mask) == -1) \
	    cma__bugcheck("cma_wait:6");				\
	cma__queue_remove(&wblk.queue);					\
	if (sigprocmask(SIG_SETMASK, &sig_mask, (sigset_t *)NULL) == -1) \
	    cma__bugcheck("cma_wait:7");				\
	cma__int_unlock(wblk.tcb->tswait_mutex);			\
	RERAISE;							\
    }									\
    ENDTRY								\
    return ret_val;							\
}


/*
 * Data structures
 */

typedef struct CMA___T_HPWAIT_BLOCK {
    cma__t_queue	queue;		/* Must be first field */
    cma_t_integer	waiting;	/* =1 if thread not signalled yet */
    cma__t_int_cv	*cv;		/* Condition var. thread blocks on */
    cma__t_int_tcb	*tcb;		/* For debugging */
    } cma___t_hpwait_block;

#ifdef __hpux
/*
 * callback structure
 */
_ctxcb_spec_t	_cb = {0, 0, 0, 0};
#endif

/*
 * Local data
 */

static sigset_t		cma___g_hpwait_sigmask;
static cma__t_queue	cma___g_hpwait_queue;


#ifdef __hpux
/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	pthread_ctxcb_hpux
 *
 *	Allows user to load the callback structure with from one
 *	to four callback functions.  Used at thread start, stop,
 *	context switch out and context switch in times.
 *
 *  FORMAL PARAMETERS:
 *
 *	cb - the user-supplied callback structure
 *
 *  IMPLICIT INPUTS:
 *
 *	None.
 *
 *  IMPLICIT OUTPUTS:
 *
 *	None.
 *
 *  FUNCTION VALUE:
 *
 *	None.
 *
 *  SIDE EFFECTS:
 *
 *	None.
 */

extern void
pthread_ctxcb_hpux(_ctxcb_spec_t *cb)
{
_cb.start = cb->start;
_cb.end = cb->end;
_cb.in = cb->in;
_cb.out = cb->out;
}

#endif


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma_setrlimit
 *
 *	Wrapper for setrlimit() which allows us to keep track of
 *	increases/decreases in the maximum open file limit.
 *	This is necessary for performance reasons.  The alternative
 *	is to assume the worst case, ie. that up to 2048 files
 *	can be open, which is rarely going to be true.
 *
 *  FORMAL PARAMETERS:
 *
 *	resource -  specifies resource being modified
 *	rlimit	 -  hard and soft limit structure
 *
 *  IMPLICIT INPUTS:
 *
 *	None.
 *
 *  IMPLICIT OUTPUTS:
 *
 *	cma__g_mx_file and cma__g_nspm
 *
 *  FUNCTION VALUE:
 *
 *	0 on success, -1 on failure.
 *
 *  SIDE EFFECTS:
 *
 *	None
 */

extern int
cma_setrlimit (
	int	resource,
	struct	rlimit *rlp)
{
    int ret_val = 0;

    if (!cma__is_multithreaded())  return (setrlimit(resource, rlp));
    if (resource == RLIMIT_NOFILE) {
#ifndef HP_CMA_FIX_CHFts16291
        if (rlp->rlim_cur > FD_SETSIZE) {
	    errno = EINVAL;
	    return -1;
	}
#endif  /* HPDCE_CMA_FIX_CHFts16291 */
	cma__int_lock(cma__g_io_data_mutex);
	if ((ret_val = setrlimit(resource, rlp)) != -1) {
	    cma__g_mx_file = rlp->rlim_cur;
	    cma__g_nspm = ((cma__g_mx_file + cma__c_nbpm - 1)/cma__c_nbpm);
	}
	cma__int_unlock(cma__g_io_data_mutex);
	return ret_val;
    }

    else
	return (setrlimit(resource, rlp));

}


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma_ioctl
 *
 *	Wrapper for ioctl() which allows us to check if non-blocking
 *	I/O is being enabled via the FIOSNBIO command.  We need to
 *	record this fact just as we do when non-blocking I/O is
 *	enabled via fcntl().
 *
 *	All other ioctl commands are passed unchanged/unchecked to
 *	the kernel.
 *
 *  FORMAL PARAMETERS:
 *
 *	fd	-  a Unix file descriptor, a number.
 *	request -  specifies operation to be performed
 *	arg	-  argument to the request
 *
 *  IMPLICIT INPUTS:
 *
 *	The CMA I/O database
 *
 *  IMPLICIT OUTPUTS:
 *
 *	The CMA I/O database
 *
 *  FUNCTION VALUE:
 *
 *	Depends on the request.
 *
 *  SIDE EFFECTS:
 *
 *	None
 */

extern int
cma_ioctl (
	int	fd,
	int	request,
	int	arg)
{
    cma_t_integer   flags;
    cma_t_integer   old_user_non_blocking;
    cma_t_integer   ret_val; 
    cma_t_boolean   done = cma_c_false; 
    cma_t_boolean   bad_wait = cma_c_false; 
    cma__t_io_type  io_type;

    if (!cma__is_multithreaded())  return (ioctl(fd, request, arg));
#ifdef _HP_CMA_ASYNC_CLOSE_
    if(!cma__hp_fd_reserve(fd))  return( cma__set_errno(EBADF), -1 );
#else
    if ((fd < 0) || (fd >= cma__g_mx_file))  return (cma__set_errno (EBADF), -1);
    if (!cma__is_open(fd))  return (cma__set_errno (EBADF), -1);
    cma__fd_reserve (fd);
#endif

#if __hpux1010p
    /*
     * As of Walnut Creek, ioctl() can be used to send and receive data
     * from a STREAMS XTI driver. We must support this capability by using
     * the standard cma__ts_func macro around this function to enable thread
     * synchronous I/O.
     */
    if (request == I_XTI_SND || request == I_XTI_RCV
	|| request == I_XTI_USND || request == I_XTI_URCV) {

	switch (request) {
	    case I_XTI_SND:
	    case I_XTI_USND:
		io_type = cma__c_io_write;
		break;
	    case I_XTI_RCV:
	    case I_XTI_URCV:
		io_type = cma__c_io_read;
		break;
	    default : 
		cma__bugcheck ("cma_ioctl:1");
		break;
	}
	
	if (cma__issue_io_call(fd)) {
	    if ((!cma__g_file[fd]->set_non_blocking) && 
		(cma__g_file[fd]->non_blocking == cma_c_true)) 
		cma__set_nonblocking(fd); 
	    cma__interrupt_disable (0); 
	    TRY { 
		ret_val = ioctl(fd, request, arg);
	    }
	    CATCH_ALL { 
		cma__interrupt_enable (0); 
		cma__fd_unreserve (fd); 
		RERAISE; 
	    } 
	    ENDTRY 
		cma__interrupt_enable (0); 
	    if ((ret_val != -1) 
		|| (!cma__is_blocking (errno)) 
		|| (cma__g_file[fd]->user_fl.user_non_blocking)) 
		done = cma_c_true; 
	} 
	if (done) { 
	    cma__fd_unreserve (fd); 
	} 
	else { 
	    TRY { 
		cma__io_prolog (io_type, fd); 
		while (!done) { 
		    cma__io_wait (io_type, fd, &bad_wait); 
		    if( bad_wait ) 
			done = cma_c_true; 
		    else { 
			ret_val = ioctl(fd, request, arg);
			if ((ret_val != -1) 
			    || (!cma__is_blocking (errno)) 
			    || (cma__g_file[fd]->user_fl.user_non_blocking)) 
			    done = cma_c_true; 
		    } 
		} 
	    } 
	    FINALLY { 
		if( !bad_wait ) { 
		    cma__io_epilog (io_type, fd); 
		    cma__fd_unreserve (fd); 
		} 
	    } 
	    ENDTRY 
	}
	if (bad_wait) return( cma__set_errno(EBADF), -1 ); 
	return ret_val; 
    }
    /* Not a STREAMS XTI request for I/O from/to the driver */
    else {
#endif	/* __hpux1010p */

	ret_val = ioctl(fd, request, arg);

	if ((request == FIOSNBIO || request == FIONBIO) && ret_val != -1) {
	    cma__g_file[fd]->user_fl.fiosnbio = *(int *)arg;
	    old_user_non_blocking = cma__g_file[fd]->user_fl.user_non_blocking;
	    cma__g_file[fd]->user_fl.user_non_blocking = 
		cma__g_file[fd]->user_fl.o_nonblock |
		cma__g_file[fd]->user_fl.o_ndelay |
		cma__g_file[fd]->user_fl.fiosnbio;
	   
	    /*
	     * If we are using non-blocking I/O internally, then
	     * we have some extra work to do (assuming we have already
	     * enabled it, ie. set_non_blocking is set).
	     * If the application is enabling FIOSNBIO, then we must
	     * disable O_NONBLOCK if necessary.
	     * If the application is disabling FIOSNBIO, then we must
	     * re-enable O_NONBLOCK assuming no other form of non-blocking
	     * I/O is being used.
	     */
	    if (cma__g_file[fd]->non_blocking &&
		cma__g_file[fd]->set_non_blocking) {
		if (*(int *)arg == 1) {
		    if (old_user_non_blocking == 0) {
			flags = fcntl(fd, F_GETFL, 0);
			flags &= ~O_NONBLOCK;
			fcntl(fd, F_SETFL, flags);
		    }
		}
		else {
		    if (cma__g_file[fd]->user_fl.user_non_blocking == 0) {
			flags = fcntl(fd, F_GETFL, 0);
			flags |= O_NONBLOCK;
			fcntl(fd, F_SETFL, flags);
		    }
		}
	    }
	}
	cma__fd_unreserve (fd);
	return ret_val;
#if __hpux1010p
    }
#endif
}


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma_msgrcv
 *
 *	Wrapper for msgrcv() which allows us to attempt the
 *	non-blocking form of the call.  If the call returns
 *	because it would have blocked, we sleep for a short
 *	time and try the call again later.
 *
 *  FORMAL PARAMETERS:
 *
 *	msqid	-  message queue identifier
 *	msgp	-  pointer to buffer where message will be stored
 *	msgsz	-  maximum length of message text
 *	msgtyp	-  message type
 *	msgflg	-  optional actions
 *
 *  IMPLICIT INPUTS:
 *
 *	None
 *
 *  IMPLICIT OUTPUTS:
 *
 *	None.
 *
 *  FUNCTION VALUE:
 *
 *	length of message read or -1
 *
 *  SIDE EFFECTS:
 *
 *	None
 */

extern int
cma_msgrcv (
	int	msqid,
	void	*msgp,
	int	msgsz,
	long	msgtyp,
	int	msgflg)
{
    int ret_val;
    
    if (!cma__is_multithreaded())  
	return (msgrcv(msqid, msgp, msgsz, msgtyp, msgflg));
    if (msgflg & IPC_NOWAIT)
	return (msgrcv(msqid, msgp, msgsz, msgtyp, msgflg));

    /*
     * Caller has not requested NOWAIT option, so we'll set it.
     * Then keep trying the operation until either it succeeds
     * or we get an error besides ENOMSG.
     */
    msgflg |= IPC_NOWAIT;
    while (1) {
	ret_val = msgrcv(msqid, msgp, msgsz, msgtyp, msgflg);    
	if (ret_val != -1 || errno != ENOMSG)
	    return ret_val;

	cma_delay(POLLWAIT);
    }
}


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma_msgsnd
 *
 *	Wrapper for msgsnd() which allows us to attempt the
 *	non-blocking form of the call.  If the call returns
 *	because it would have blocked, we sleep for a short
 *	time and try the call again later.
 *
 *  FORMAL PARAMETERS:
 *
 *	msqid	-  message queue identifier
 *	msgp	-  pointer to user-defined buffer that contains
 *		   the message
 *	msgsz	-  length of message text
 *	msgflg	-  optional actions
 *
 *  IMPLICIT INPUTS:
 *
 *	None
 *
 *  IMPLICIT OUTPUTS:
 *
 *	None.
 *
 *  FUNCTION VALUE:
 *
 *	0 or -1
 *
 *  SIDE EFFECTS:
 *
 *	None
 */

extern int
cma_msgsnd (
	int	msqid,
	void	*msgp,
	int	msgsz,
	int	msgflg)
{
    int ret_val;
    
    if (!cma__is_multithreaded())  
	return (msgsnd(msqid, msgp, msgsz, msgflg));
    if (msgflg & IPC_NOWAIT)
	return (msgsnd(msqid, msgp, msgsz, msgflg));

    /*
     * Caller has not requested NOWAIT option, so we'll set it.
     * Then keep trying the operation until either it succeeds
     * or we get an error besides EAGAIN.
     */
    msgflg |= IPC_NOWAIT;
    while (1) {
	ret_val = msgsnd(msqid, msgp, msgsz, msgflg);    
	if (ret_val != -1 || errno != EAGAIN)
	    return ret_val;

	cma_delay(POLLWAIT);
    }
}


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma_semop
 *
 *	Wrapper for semop() which allows us to attempt the
 *	non-blocking form of the call.  If the call returns
 *	because it would have blocked, we sleep for a short
 *	time and try the call again later.
 *
 *  FORMAL PARAMETERS:
 *
 *	semid	-  semaphore identifier
 *	sops	-  array of semaphore-operation structures
 *	nsops	-  number of structures in array
 *
 *  IMPLICIT INPUTS:
 *
 *	None
 *
 *  IMPLICIT OUTPUTS:
 *
 *	None.
 *
 *  FUNCTION VALUE:
 *
 *	-1 on error
 *
 *  SIDE EFFECTS:
 *
 *	None
 */

extern int
cma_semop (
	int		semid,
	struct sembuf	*sops,
	int		nsops)
{
    int i;
    int ret_val;
    unsigned int nowait_map;
    unsigned int nowait_bigmap[SEMAP_LEN];
    int flag = 0;
    
    if (!cma__is_multithreaded())  
	return (semop(semid, sops, nsops));
    /*
     * Check if caller is already using the IPC_NOWAIT flag.  If so
     * then there's nothing for us to do, just call the normal
     * semops system call.  If not, then we'll set it where necessary.
     */
    if (nsops <= 32) {	/* the simple case: < 32 sembuf's */
	nowait_map = 0;
	for (i = 0; i < nsops; i++) {
	    if (sops[i].sem_op <= 0 && !(sops[i].sem_flg & IPC_NOWAIT)) {
		sops[i].sem_flg |= IPC_NOWAIT;
		nowait_map |= 1 << i;
	    }
	}
	if (nowait_map == 0)
	    return (semop(semid, sops, nsops));
    }
    else {		/* life gets more complicated here */
	memset(nowait_bigmap, 0, SEMAP_LEN);
	for (i = 0; i < nsops; i++) {
	    if (sops[i].sem_op <= 0 && !(sops[i].sem_flg & IPC_NOWAIT)) {
		sops[i].sem_flg |= IPC_NOWAIT;
		SEMAP_SET(nowait_bigmap, i);
		flag = 1;
	    }
	}
	if (flag == 0)
	    return (semop(semid, sops, nsops));
    }

    /*
     * Caller has not requested NOWAIT option, so we'll set it.
     * Then keep trying the operation until either it succeeds
     * or we get an error besides EAGAIN.
     */
    while (1) {
	ret_val = semop(semid, sops, nsops);
	if (ret_val != -1 || errno != EAGAIN) break;
	cma_delay(POLLWAIT);
    }

    /*
     * Restore sops array to the way it was.
     */
    if (nsops <= 32) {
	for (i = 0; i < nsops; i++) {
	    if (nowait_map & (1 << i))
		sops[i].sem_flg &= ~IPC_NOWAIT;
	}
    }
    else {
	for (i = 0; i < nsops; i++) {
	    if (SEMAP_ISSET(nowait_bigmap, i))
		sops[i].sem_flg &= ~IPC_NOWAIT;
	}
    }

    return ret_val;
}


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma_wait
 *
 *	Wrapper for wait() which translates the call into
 *	the equivalent waitpid() call and attempts the
 *	non-blocking form of the call.  If not successful,
 *	the thread is put to sleep until a SIGCHLD signal
 *	is received.
 *
 *  FORMAL PARAMETERS:
 *
 *	stat_loc - returned status information
 *
 *  IMPLICIT INPUTS:
 *
 *	None
 *
 *  IMPLICIT OUTPUTS:
 *
 *	None.
 *
 *  FUNCTION VALUE:
 *
 *	The process ID of the waited on process, or -1 on error.
 *
 *  SIDE EFFECTS:
 *
 *	None
 */

extern pid_t
cma_wait (int	*stat_loc)
{

    if (!cma__is_multithreaded())  
	return (wait(stat_loc));
    HP_WAIT(waitpid(-1, stat_loc, WNOHANG));
}


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma_wait3
 *
 *	Wrapper for wait3() which attempts the non-blocking
 *	form of the call.  If not successful, the thread is put 
 *	to sleep until a SIGCHLD signal is received.
 *
 *  FORMAL PARAMETERS:
 *
 *	stat_loc - returned status information
 *	options  - wait flags
 *	reserved - unused
 *
 *  IMPLICIT INPUTS:
 *
 *	None
 *
 *  IMPLICIT OUTPUTS:
 *
 *	None.
 *
 *  FUNCTION VALUE:
 *
 *	The process ID of the waited on process, or -1 on error.
 *
 *  SIDE EFFECTS:
 *
 *	None
 */

extern pid_t
cma_wait3 (
	int	*stat_loc,
	int	options,
	int	*reserved)
{

    if ((options & WNOHANG) || (!cma__is_multithreaded()))
	return (wait3(stat_loc, options, reserved));

    HP_WAIT(wait3(stat_loc, options | WNOHANG, reserved));
}


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma_waitpid
 *
 *	Wrapper for waitpid() which attempts the non-blocking
 *	form of the call.  If not successful, the thread is put 
 *	to sleep until a SIGCHLD signal is received.
 *
 *  FORMAL PARAMETERS:
 *
 *	pid	 - pid to wait on
 *	stat_loc - returned status information
 *	options  - wait flags
 *
 *  IMPLICIT INPUTS:
 *
 *	None
 *
 *  IMPLICIT OUTPUTS:
 *
 *	None.
 *
 *  FUNCTION VALUE:
 *
 *	The process ID of the waited on process, or -1 on error.
 *
 *  SIDE EFFECTS:
 *
 *	None
 */

extern pid_t 
cma_waitpid (
	pid_t	pid,
	int	*stat_loc,
	int	options)
{

    if ((options & WNOHANG) || (!cma__is_multithreaded()))
	return (waitpid(pid, stat_loc, options));

    HP_WAIT(waitpid(pid, stat_loc, options | WNOHANG));
}


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma___hp_sigchld
 *
 *	Signal handler for SIGCHLD
 *
 *  FORMAL PARAMETERS:
 *
 *	sig	- signal number
 *	code	- additional error code
 *	scp	- pointer to signal context buffer
 *
 *  IMPLICIT INPUTS:
 *
 *	None
 *
 *  IMPLICIT OUTPUTS:
 *
 *	None
 *
 *  FUNCTION VALUE:
 *
 *	None
 *
 *  SIDE EFFECTS:
 *
 *	Threads blocked on wait() system calls are awakened.
 */

extern void
cma___hp_sigchld (
        int                     sig,
        int                     code,
        struct  sigcontext      *scp)
{
    cma___t_hpwait_block	*wblk;
    cma_t_integer		wakeup = 0;

    wblk = (cma___t_hpwait_block *)cma__queue_next (&cma___g_hpwait_queue);
    while (wblk != (cma___t_hpwait_block*)&cma___g_hpwait_queue) {
	if (wblk->waiting) {
	    wblk->waiting = 0;
	    wakeup = 1;
	    cma__int_signal_int(wblk->cv);
	}
	wblk = (cma___t_hpwait_block *)cma__queue_next (&wblk->queue);
    }

    cma__sig_deliver(sig, code, scp);

    if (io_avail_null_thread && wakeup) {
	io_avail_null_thread = 0;
#if __hpux1010p
	if (sigprocmask (SIG_SETMASK, &scp->sc_sx.sl_uc.uc_sigmask,
			 (sigset_t *)cma_c_null_ptr) == -1)
	    cma__bugcheck("hp_sigchld:1");
#else
	if (sigsetmask(scp->sc_mask) == -1)
	    cma__bugcheck("hp_sigchld:1");
#endif  /* if __hpux1010p */
	exc_raise(&io_avail_exc);
    }
}


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma__init_hp
 *
 *	Initialization for HP specific features.
 *
 *  FORMAL PARAMETERS:
 *
 *	None
 *
 *  IMPLICIT INPUTS:
 *
 *	None
 *
 *  IMPLICIT OUTPUTS:
 *
 *	None
 *
 *  FUNCTION VALUE:
 *
 *	None
 *
 *  SIDE EFFECTS:
 *
 *	None
 */

extern void
cma__init_hp(void)
{

    cma__queue_init(&cma___g_hpwait_queue);

    if (sigemptyset(&cma___g_hpwait_sigmask) == -1)
	cma__bugcheck("init_hp:1");
    if (sigaddset(&cma___g_hpwait_sigmask, SIGCHLD) == -1)
	cma__bugcheck("init_hp:2");
    if (sigaddset(&cma___g_hpwait_sigmask, SIGVTALRM) == -1)
	cma__bugcheck("init_hp:3");
}


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma__reinit_hp
 *
 *	Pre/post fork re-initialization.
 *
 *  FORMAL PARAMETERS:
 *
 *	flag
 *
 *  IMPLICIT INPUTS:
 *
 *	None
 *
 *  IMPLICIT OUTPUTS:
 *
 *	None
 *
 *  FUNCTION VALUE:
 *
 *	None
 *
 *  SIDE EFFECTS:
 *
 *	None
 */

extern void
cma__reinit_hp(cma_t_integer	flag)
{

    if (flag == cma__c_reinit_postfork_clear)
	cma__queue_init(&cma___g_hpwait_queue);
}

/*
 * Version of system that doesn't block SIGCHLD
 * (waitpid never returns if SIGCHLD blocked.)
 */

#include <errno.h>
#include <sys/signal.h>
#include <unistd.h>

#if __hpux1000p
#define	SHELL	"/sbin/sh"
#else
#define	SHELL	"/bin/posix/sh"
#endif /* __hpux1000p */

cma_system(s)
char *s;
{
pid_t pid;
int status;
sigset_t oldmask;
struct sigaction act;
struct sigaction o_int_act;
struct sigaction o_quit_act;

    if (!cma__is_multithreaded())  return (system(s));
    /*
     * ANSI-C and XPG4 require that we return non-zero when the command
     * interpreter is available and zero otherwise.  POSIX.2 requires that we 
     * return non-zero, since in a POSIX.2 conformant system, the interpreter
     * is always available.
     */
    if (s == (char *)0)
    {
	if (access(SHELL, X_OK) == 0)
	    return 1;
	else
	    return 0;
    }

    /*
     * POSIX.2 and XPG4 requires that we ignore SIGINT and SIGQUIT.
     * We must also block SIGCHLD.
     */
    act.sa_handler = SIG_IGN;
    (void) sigemptyset(&act.sa_mask);
    /*
     * The following calls to sigemptyset() are technically required by
     * POSIX, but on HP-UX they are not necessary (given that sa_mask is of
     * type sigset_t which is an array of longs.  I'm leaving this code 
     * commented out for now, but we may want to reconsider if the HP-UX
     * sigset_t definition changes.
     *
     * (void) sigemptyset(&o_int_act.sa_mask);
     * (void) sigemptyset(&o_quit_act.sa_mask);
     */

    /*
     * In the context of CMA, these sigaction calls affect only
     * the calling thread ...
     */
    act.sa_flags = 0;
    (void) sigaction(SIGINT, &act, &o_int_act);
    (void) sigaction(SIGQUIT, &act, &o_quit_act);

    /* Block SIGCHLD */
    /* Just use the sa_mask from the act struct since it has already been
       initialized */

    /*
     * Can't block SIGCHLD in CMA, no matter what the
     * standards say ... wait will cease working.
     *
     * (void) sigaddset(&act.sa_mask, SIGCHLD);
     * (void) sigprocmask(SIG_BLOCK, &act.sa_mask, &oldmask);
     */
    
    if ((pid = fork()) == 0)	/* fork successful, in child process */
    {
        /* Restore original handlers and signal mask in child before exec */
	(void) sigaction(SIGINT, &o_int_act, (struct sigaction *)0);
        (void) sigaction(SIGQUIT, &o_quit_act, (struct sigaction *)0);
	/*
	 * Since we didn't block SIGCHLD, don't restore sigmask
         *
	 * (void) sigprocmask(SIG_SETMASK, &oldmask, (sigset_t *)0);
	 */

	/* "Q" flag tells the shell to ignore the "ENV" variable if present */
	(void) execl(SHELL, "sh", "-cQ", s, 0);
	_exit(127);		/* exec failed */
    }

    /* else parent process */
    /* check that the fork() succeeded */
    if (pid == -1)
    {
	/*
	 * fork failed: set status (return value) to -1
	 *     errno should have been set by fork()
	 */
	status = -1;
    }
    else
    {
        /*
         * POSIX.2 and XPG4 also require that system() not return until 
	 * the child process has terminated.  This means that if we get
	 * interrupted for any other reason, we need to loop on the 
	 * waitpid().
         */
	int save_errno = errno;

        while(1)
        {
	    errno = 0;
	    if (cma_waitpid(pid, &status, 0) == -1)
	    {
	        if (errno == EINTR)
		    continue;
	        else
	        {
		    status = -1;
		    break;
	        }
	    }
	    else
	        break;
        }
    }

    /* Restore original handlers and signal mask */
    (void) sigaction(SIGINT, &o_int_act, (struct sigaction *)0);
    (void) sigaction(SIGQUIT, &o_quit_act, (struct sigaction *)0);
    /*
     * Since we didn't block SIGCHLD, don't restore sigmask
     *
     * (void) sigprocmask(SIG_SETMASK, &oldmask, (sigset_t *)0);
     */

    return status;
}

int
cma_sleep(int secs)
{
	struct timespec ts;

	if (secs < 0)
	    return(-1);
	else if (secs == 0)
	    return(0);

	ts.tv_sec = secs;
	ts.tv_nsec = 0;

	pthread_delay_np(&ts);
	return(0);
}


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma__hp_singlethreaded_delay()
 *
 *	Timed delay routine for single threaded applications.
 *
 *  FORMAL PARAMETERS:
 *
 *	timeout  - absolute time value
 *
 *  IMPLICIT INPUTS:
 *
 *	None
 *
 *  IMPLICIT OUTPUTS:
 *
 *	None
 *
 *  FUNCTION VALUE:
 *
 *	None
 *
 *  SIDE EFFECTS:
 *
 *	None
 *

extern void
cma__hp_singlethreaded_delay (cma_t_date_time  *timeout)
{
    cma_t_date_time    delta, current;
    cma_t_integer      result;

    while (cma_c_true) {
	cma__get_time (&current);
	if (cma__time_lss (timeout, &current))
	    break;
	cma__subtract_time (&delta, timeout, &current);
	if ((result = select (0, 0, 0, 0, &delta)) == 0)
	    break;
	else if ((result == -1) && (errno != EINTR))
	    cma__bugcheck("cma__hp_singlethreaded_delay: unexpected return value from select");
    }
} */
