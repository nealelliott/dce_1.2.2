/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems
 * Burlington, MA, USA
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: cma_thdio_4.c,v $
 * Revision 1.1.6.1  1996/05/10  13:29:08  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:51 UTC  bissen
 * 	Merge fix for CHFts13125 and CHFts15594.
 * 	[1995/07/31  23:06 UTC  jss  /main/HPDCE02/12]
 *
 * 	Fix for CHFts13125. Modify cma_send() paramater list to correspond to
 * 	standard HPUX_SOURCE prototype.
 * 	[1995/07/29  17:57 UTC  jss  /main/HPDCE02/jss_mothra_fix6/1]
 *
 * 	Merge Spec 1170 changes for Walnut Creek.
 * 	[1995/07/10  20:08 UTC  jss  /main/HPDCE02/11]
 *
 * 	CHFts15474. Fix problem with accrights feature in recvmsg/sendmsg.
 * 	[1995/06/09  18:14 UTC  jss  /main/HPDCE02/jss_mothra_cose/1]
 *
 * 	Merge changes for CHFts15159 and CHFts15160.
 * 	[1995/05/08  19:48 UTC  jss  /main/HPDCE02/10]
 *
 * 	Fix for CHFts15159. cma_select returns an unexpected error value. Fix race
 * 	condition by aquiring the kernel lock.
 * 	Fix for CHFts15160. Logic in sendmsg/recvmsg for passing fd's is incorrect.
 * 	Make sure that the msg_accrights field is not NULL when checking to see
 * 	if user is attempting to pass an fd.
 * 	[1995/05/01  17:18 UTC  jss  /main/HPDCE02/jss_mothra_fix3/1]
 *
 * 	Merge from branch jss_single_thread.
 * 	[1994/12/22  20:02 UTC  jss  /main/HPDCE02/9]
 *
 * 	Changes for single threaded client.  Conditionalize wrapper usage based on whether
 * 	CMA is initialized.
 * 	[1994/12/22  19:24 UTC  jss  /main/HPDCE02/jss_single_thread/1]
 *
 * 	Merge from branch jss_fork_mothra to HPDCE02.
 * 	[1994/12/09  16:21 UTC  jss  /main/HPDCE02/8]
 *
 * 	Clean up sendmsg routine.
 * 	[1994/12/09  16:14 UTC  jss  /main/HPDCE02/jss_fork_mothra/2]
 *
 * 	CHFts13807. Fix for cma_recvmsg(). There is a problem in the kernel where the
 * 	msg_accrightslen field of the msg structure gets cleared on return from the
 * 	recvmsg call. Need to save this field and restore it before calling recvmsg a
 * 	second time.
 * 	[1994/12/07  22:23 UTC  jss  /main/HPDCE02/jss_fork_mothra/1]
 *
 * 	Merge from branch jss_fix_close to HPDCE02.
 * 	[1994/10/19  19:56 UTC  jss  /main/HPDCE02/7]
 *
 * 	Fix for CHFts12442. Fix race condition in cma_select.
 * 	[1994/10/19  19:50 UTC  jss  /main/HPDCE02/jss_fix_close/8]
 *
 * 	Merge from branch jss_fix_close to HPDCE02.
 * 	[1994/10/05  14:28 UTC  jss  /main/HPDCE02/6]
 *
 * 	Fix for CHFts12442. Modified cma_connect to allow EBADF return.
 * 	[1994/10/05  14:26 UTC  jss  /main/HPDCE02/jss_fix_close/7]
 *
 * 	Merge from jss_fix_close to HPDCE02 performance improvements.
 * 	[1994/09/28  18:18 UTC  jss  /main/HPDCE02/5]
 *
 * 	Fix for CHFts12442. cma_select not clearing select bits.
 * 	[1994/09/28  18:08 UTC  jss  /main/HPDCE02/jss_fix_close/6]
 *
 * 	Merge select optimizations from jss_fix_close to HPDCE02.
 * 	[1994/09/26  17:40 UTC  jss  /main/HPDCE02/4]
 *
 * 	Fix for CHFts12442. Serialize close and select notification.
 * 	[1994/09/16  22:12 UTC  jss  /main/HPDCE02/jss_fix_close/5]
 *
 * 	Merge from jss_fix_close to HPDCE02.
 * 	[1994/09/15  19:22 UTC  jss  /main/HPDCE02/3]
 *
 * 	Fix for CHFts12442.  Fixed cancel problem in cma__io_wait and wrappers.
 * 	[1994/09/14  16:10 UTC  jss  /main/HPDCE02/jss_fix_close/4]
 *
 * 	Fix for CHFts12442.  Modified cma_connect().
 * 	[1994/09/13  19:49 UTC  jss  /main/HPDCE02/jss_fix_close/3]
 *
 * 	Merge from branch jss_close_fix.
 * 	[1994/09/08  19:06 UTC  jss  /main/HPDCE02/2]
 *
 * 	Fix for CHFts12442. Add select() functionality.
 * 	[1994/09/08  13:40 UTC  jss  /main/HPDCE02/jss_fix_close/2]
 *
 * 	Fix for CHFts12442. Close while I/O pending bug.
 * 	[1994/08/30  14:24 UTC  jss  /main/HPDCE02/jss_fix_close/1]
 *
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:48 UTC  jss  /main/HPDCE02/1]
 *
 * 	merge from jss_dce_threads to HPDCE01
 * 	[1994/02/28  20:00 UTC  jss  /main/HPDCE01/3]
 *
 * 	fix for OT8667 connect returns values other than -1 or 0
 * 	[1994/02/28  19:41 UTC  jss  /main/HPDCE01/jss_dce_threads/1]
 *
 * 	fixed merge botch
 * 	[1994/01/27  20:38  markar  /main/HPDCE01/2]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/25  14:24  truitt  /main/HPDCE01/1]
 *
 * 	CHFts09399: Changed cma_send from a call to ts_func
 * 	to ts_func2.  This new macro will enable send() to
 * 	retry when send is successful, but the entire
 * 	message buffer was not sent due to size reasons.
 * 	[1993/11/29  15:50:11  truitt]
 * 	 *
 *
 * 	CHFts09402: Additional fix... Check for tv_sec larger
 * 	that MAX_ALARM, and if so, quietly r
 *
 * Revision 1.1.2.2  1994/06/09  13:34:48  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:25:39  devsrc]
 * 
 * 	Integrate fixes for OT defect 2540 with fixes for defect 2317.
 * 	[1992/06/23  13:30:34  bolinger]
 * 
 * 	Fix OT defect 2317:  in cma_recvmsg(), recover properly if system
 * 	overwrites source address buffer length when no message read --
 * 	save away the original length and restore before next attempt
 * 	to read message.
 * 	[1992/05/22  14:43:23  bolinger]
 * 
 * 	Complete(!?) fix for defect 2540 by adding code to cma_connect()
 * 	to set socket non-blocking, if not already done.
 * 	[1992/05/22  15:06:46  bolinger]
 * 
 * 	Extend fix for defect 2540:  change special wrapper code
 * 	for recvfrom() to reflect new non-blocking I/O initialization.
 * 	[1992/05/22  14:40:01  bolinger]
 * 
 * Revision 1.1.2.1  1994/04/01  20:13:52  jd
 * 	Moved from src/threads to here
 * 	[1994/04/01  17:03:26  jd]
 * 
 * Revision 1.1.8.3  1993/07/19  20:15:03  blurie
 * 	Remove dead SVR4 code in cma_select().
 * 	[1993/07/19  18:19:09  blurie]
 * 
 * Revision 1.1.8.2  1993/06/24  21:19:32  hinman
 * 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
 * 	[1993/06/17  14:46:42  hinman]
 * 
 * Revision 9.5.1.7  92/12/17  14:02:10  raj
 * 	[raj] New naming + parallelized reentrant library changes.
 * 
 * Revision 9.5.4.2  92/11/06  11:24:17  raj
 * 	[raj] Changes for new naming scheme with reentrant libraries.
 * 
 * Revision 9.5.1.6  92/09/30  12:41:58  root
 * 	Acceptance of OSF rev 1.1.5.3
 * 
 * Revision 9.12.1.2  92/09/30  10:37:17  hinman
 * 	Auto checkin of OSF rev 1.1.5.3
 * 
 * Revision 1.1.2.3  1992/08/14  20:23:05  bolinger
 * 	Bring forward 1.0.1 revisions 1.1.3.2 and 1.1.3.3 (fix for OT
 * 	defect 2540), 1.1.3.4 (fix for 2317).
 * 	[1992/08/10  16:31:01  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/09  00:04:09  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/09  00:03:53  bolinger]
 * 
 * Revision 1.1  1992/01/19  14:41:08  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  Copyright (c) 1990, 1991 by
 *  Digital Equipment Corporation, Maynard Massachusetts.
 *  All rights reserved.
 *
 *  This software is furnished under a license and may be used and  copied
 *  only  in  accordance  with  the  terms  of  such  license and with the
 *  inclusion of the above copyright notice.  This software or  any  other
 *  copies  thereof may not be provided or otherwise made available to any
 *  other person.  No title to and ownership of  the  software  is  hereby
 *  transferred.
 *
 *  The information in this software is subject to change  without  notice
 *  and  should  not  be  construed  as  a commitment by DIGITAL Equipment
 *  Corporation.
 *
 *  DIGITAL assumes no responsibility for the use or  reliability  of  its
 *  software on equipment which is not supplied by DIGITAL.
 */

/*
 *  FACILITY:
 *
 *	CMA services
 *
 *  ABSTRACT:
 *
 *	Thread synchronous I/O Module 4:  Socket routines.
 *
 *  AUTHORS:
 *
 *	Paul Curtin
 *
 *  CREATION DATE:
 *
 *	1 May 1991  (Split from cma_thread_io.c)
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Webb Scales	2 May 1991
 *		- Moved cma_select prototype from cma_thread_io to this module
 *		    where it is referrenced in cma_connect.
 *		- Moved includes from cma_thdio_defs.h to here.
 *		- Added argument to bugcheck calls.
 *		- Use macros to modify select fd masks in cma_connect.
 *	002	Paul Curtin	3 May 1991
 *		Replaced cma_init() call with cma__int_init() call.
 *	003	Dave Butenhof	08 May 1991
 *		Use new macro to test for kernel already locked, which
 *		evaporates under multiprocessor implementation.
 *	004	Paul Curtin	13 May 1991
 *		Changed a macro name from cma___ to cma__
 *	005	Webb Scales	4 September 1991
 *		Added a validity check for the first argument to cma_select.
 *	006	Dave Butenhof	12 September 1991
 *		Integrate Nat Mishkin's performance improvements to make more
 *		intelligent use of the file descriptor numbers; ULTRIX V4.2
 *		allows 4096 files, but most systems are generated to support
 *		lower numbers, and DECthreads is currently carrying around
 *		all that extra baggage. With this change, it'll only check
 *		and use the ones actually supported on the system. Also,
 *		shrink the TCB by dynamically allocating the file descriptor
 *		mask at thread creation time.
 *	007	Dave Butenhof	04 October 1991
 *		Use new _CMA_THREAD_SYNC_IO_ to disable compilation, rather
 *		than _CMA_UNIPROCESSOR_.
 *	008	Paul Curtin	14 October 1991
 *		Added TRY/CATCH around cma__int_init in cma_select.
 *	009	Dave Butenhof, Webb Scales	29 October 1991
 *		Zero out the TCB select file masks before copying the user's
 *		masks for cma_select, to prevent a spurious return and/or
 *		file count.
 *	010	Webb Scales	14 November 1991
 *		Move edit #009 to correct place.
 *		Fix a return-out-of-CATCH_ALL.
 */


/*
 *  INCLUDE FILES
 */

#include <cma.h>

#if !_CMA_THREAD_SYNC_IO_		/* Don't compile unless needed! */

#include <cma_defs.h>
#include <cma_init.h>
#include <cma_thdio_defs.h>
# include <sys/socket.h>
#include <cma_errors.h>
#include <cma_tcb.h>
#include <cma_stack.h>
#include <cma_util.h>
#include <cma_ux.h>

/*
 * LOCAL DATA
 */

/*
 * LOCAL FUNCTIONS
 *
 */

#ifdef _HP_CMA_ASYNC_CLOSE_

/* Called only by cma_select */
static cma_t_boolean cma__hp_bump_waiters_up (
    cma__t_file_mask *rfds,
    cma__t_file_mask *wfds,
    cma__t_file_mask *efds,
    int nfds );

static cma_t_boolean cma__hp_bump_waiters_down (
    cma__t_file_mask *rfds,
    cma__t_file_mask *wfds,
    cma__t_file_mask *efds,
    int nfds );
#endif


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma_accept:  accept a connection to a socket
 *
 *  FORMAL PARAMETERS:
 *
 *	socket_		    fd of socket to accept a connection to 
 *	addr		    socket address of the connecting socket
 *	addrlen		    size of socket address block 
 *
 *  IMPLICIT INPUTS:
 *
 *	The CMA I/O database
 *
 *  IMPLICIT OUTPUTS:
 *
 *	None
 *
 *  FUNCTION VALUE:
 *
 *	The file descriptor for the connected socket, or -1 in case of error
 *
 *  SIDE EFFECTS:
 *
 *	The current thread may block.
 *
 *	Note: the accept operation is "select'able" as a "read" on the socket.
 */

extern int
cma_accept(
	int		socket_,
	void    	*addr,
	int		*addrlen)
    {
#ifdef _HP_CMA_ASYNC_CLOSE_

    /*
     * This is essentially the cma__ts_func macro, but hacked
     * a bit to lock the cma__g_io_data_mutex around the accept
     * calls. This guarantees serialization between this call and
     * other opens and closes.
     */

    cma_t_integer   __res__; 
    cma_t_boolean   __done__ = cma_c_false; 
    cma_t_boolean   __bad_wait__ = cma_c_false; 
    if (!cma__is_multithreaded()) return (accept (socket_, addr, addrlen)); 
    if (!cma__hp_fd_reserve(socket_)) return (cma__set_errno (EBADF), -1); 
    if (cma__issue_io_call(socket_)) {
	if ((!cma__g_file[socket_]->set_non_blocking) && 
		(cma__g_file[socket_]->non_blocking == cma_c_true)) 
	    cma__set_nonblocking(socket_); 
        cma__interrupt_disable (0); 
	TRY { 
	    cma__int_lock (cma__g_io_data_mutex);		
	    __res__ = accept (socket_, addr, addrlen); 
	    cma__int_unlock (cma__g_io_data_mutex);		
	    }
	CATCH_ALL { 
	    cma__interrupt_enable (0); 
	    cma__fd_unreserve (socket_); 
	    RERAISE; 
	    } 
	ENDTRY 
        cma__interrupt_enable (0); 
	if ((__res__ != -1) 
		|| (!cma__is_blocking (errno)) 
		|| (cma__g_file[socket_]->user_fl.user_non_blocking)) 
	    __done__ = cma_c_true; 
	} 
    if (__done__) { 
	cma__fd_unreserve (socket_); 
	} 
    else { 
	TRY { 
	    cma__io_prolog (cma__c_io_read, socket_); 
	    while (!__done__) { 
		cma__io_wait (cma__c_io_read, socket_, &__bad_wait__); 
		if( __bad_wait__ ) 
		    __done__ = cma_c_true; 
		else { 
		    cma__int_lock (cma__g_io_data_mutex);		
		    __res__ = accept (socket_, addr, addrlen); 
		    cma__int_unlock (cma__g_io_data_mutex);		
		    if ((__res__ != -1) 
			    || (!cma__is_blocking (errno)) 
			    || (cma__g_file[socket_]->user_fl.user_non_blocking)) 
		        __done__ = cma_c_true; 
		    } 
		} 
	    } 
	FINALLY { 
	    if( !__bad_wait__ ) { 
		cma__io_epilog (cma__c_io_read, socket_); 
	        cma__fd_unreserve (socket_); 
		} 
	    } 
	ENDTRY 
	} 
    if (__bad_wait__) return( cma__set_errno(EBADF), -1 ); 
    if (__res__ != -1) cma__open_general( __res__ );
    return __res__; 

#else
    cma__ts_func (
	    accept, 
	    socket_, 
	    (socket_, addr, addrlen), 
	    cma__c_io_read,
	    cma__open_general (__res__));
#endif
    }


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma_connect:  initiate a connection on a socket
 *
 *  FORMAL PARAMETERS:
 *
 *	socket_		    fd of socket to connect from 
 *	name		    socket address of the target of the connection
 *	namelen		    size of socket address block 
 *
 *  IMPLICIT INPUTS:
 *
 *	The CMA I/O database
 *
 *  IMPLICIT OUTPUTS:
 *
 *	None
 *
 *  FUNCTION VALUE:
 *
 *	0 indicates success, -1 in case of error
 *
 *  SIDE EFFECTS:
 *
 *	The current thread may block.
 *
 *	Note: once the connect operation is issued, select'ing for write
 *		indicates success, and read indicates failure.
 */

extern int
cma_connect(
	int			socket_,
	const void	        *name,
	int			namelen)
    {
    cma_t_integer	res;
    cma_t_boolean	done = cma_c_false;
    cma__t_file_mask	rfds, wfds;

    if (!cma__is_multithreaded()) return (connect (socket_, name, namelen)); 
    /*
     * Make sure the file (socket) is OK by us.
     */
#ifdef _HP_CMA_ASYNC_CLOSE_
    if(!cma__hp_fd_reserve(socket_))
	return (cma__set_errno (EBADF), -1);
#else
    if ((socket_ < 0) || (socket_ >= cma__g_mx_file)) 
	return (cma__set_errno (EBADF), -1);

    if (!cma__is_open(socket_)) 
	return (cma__set_errno (EBADF), -1);

    /*
     * Get exclusive access to the socket.
     */
    cma__fd_reserve (socket_);
    /* 
     * See if the file for some reason got closed during possible
     * block in cma__fd_reserve 
     */
    if (!cma__is_open(socket_))
        return (cma__set_errno (EBADF), -1);
#endif
    if (!cma__g_file[socket_]->set_non_blocking)
	cma__set_nonblocking(socket_); 
    /*
     * Attempt a connection.  The socket is set to non-blocking mode, so
     * this call will return immediately.  (Block interrupts during the call
     * as appropriate.)
     */
    cma__interrupt_disable (0);
    res = connect (socket_, name, namelen);
    cma__interrupt_enable (0);

    /*
     * The connect is now either complete, failed, or "in progress".  
     * Regardless, we can now let go of the socket.
     */
    cma__fd_unreserve (socket_);

    /*
     * If the result was a "blocking"-type error, set up to detect completion.
     */
    if ((res == -1) 
	    && (cma__is_blocking (errno)) 
	    && !cma__g_file[socket_]->user_fl.user_non_blocking)  {

	while (!done) {
	    /*
	     * Check the socket for both read and write availability.
	     */
	    cma__fdm_zero(&rfds);
	    cma__fdm_zero(&wfds);
	    cma__fdm_set_bit (socket_, &rfds);
	    cma__fdm_set_bit (socket_, &wfds);
	    res = cma_select (
		    (int)(socket_ + 1), 
		    (int *)(&rfds), 
		    (int *)(&wfds), 
		    (int *)0, 
		    (struct timeval *)0);

	    /*
	     * If the socket is ready for I/O, determine which kind.
	     */
	    if (res > 0) {
		if (cma__fdm_is_set (socket_, &rfds)) {
		    /*
		     * Ready for read:  connection failure.
		     * Issue a read to set errno to the failure status.
		     */
		    done = cma_c_true;

		    res = read (socket_, cma_c_null_ptr, 0);
		    }
		else if (cma__fdm_is_set (socket_, &wfds)) {
		    /*
		     * Ready for write:  successful connection.
		     */
		    done = cma_c_true;
		    }
		else
		    /*
		     * Neither bit was set...but some bit was...
		     * This should never happen.
		     */
		    cma__bugcheck("cma_connect: Inconsistent return from cma_select");
		}
		/*
		 *  if _HP_CMA_ASYNC_CLOSE_
		 * select will return EBADF if the socket was closed while waiting.
		 */
	    else if (res < 0)
		/*
		 * There shouldn't be any errors...  (We'll ignore EINTR.)
		 */
#ifdef _HP_CMA_ASYNC_CLOSE_
		if ((errno != EINTR) && (errno != EBADF))
#else
		if (errno != EINTR)
#endif
		    cma__bugcheck ("cma_connect: Error returned from cma_select");
	    }
	}

    return (res == -1 ? -1 : 0);
    }


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma_recv:  receive a message from a socket
 *
 *  FORMAL PARAMETERS:
 *
 *	socket_		fd of socket to receive message on
 *	buf		address of buffer to receive message into
 *	length		length of buffer
 *	flags		options which modify the operation
 *
 *  IMPLICIT INPUTS:
 *
 *	The CMA I/O database
 *
 *  IMPLICIT OUTPUTS:
 *
 *	None
 *
 *  FUNCTION VALUE:
 *
 *	The number of characters received, or -1 in case of error
 *
 *  SIDE EFFECTS:
 *
 *	The current thread may block.
 */

extern int
cma_recv(
	int	socket_,
	void	*buf,
	int	length,
	int	flags)
    {
    cma__ts_func (
	    recv, 
	    socket_, 
	    (socket_, buf, length, flags), 
	    cma__c_io_read,
	    0);
    }



/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma_recvfrom:  receive a message from a socket
 *
 *  FORMAL PARAMETERS:
 *
 *	socket_		fd of socket to receive message on
 *	buf		address of buffer to receive message into
 *	length		length of buffer
 *	flags		options which modify the operation
 *	from		socket address of the source of the message
 *	fromlen		length of the socket address block
 *
 *  IMPLICIT INPUTS:
 *
 *	The CMA I/O database
 *
 *  IMPLICIT OUTPUTS:
 *
 *	None
 *
 *  FUNCTION VALUE:
 *
 *	The number of characters received, or -1 in case of error
 *
 *  SIDE EFFECTS:
 *
 *	The current thread may block.
 */
extern int
cma_recvfrom(
	int		socket_,
	void		*buf,
	int		length,
	int		flags,
	void    	*from,
	int		*fromlen)
    {
    /*
     * We can't use cma__ts_func(recvfrom...) directly, because if recvfrom
     * returns with nothing read, then fromlen gets set to zero, and will
     * remain that way when we redo the call. Instead, break it up and save
     * the original length.
     *    cma__ts_func (
     *	    recvfrom, 
     *	    socket_, 
     *	    (socket_, buf, length, flags, from, fromlen), 
     *	    cma__c_io_read,
     *	    0);
     */
#ifdef _HP_CMA_ASYNC_CLOSE_
    cma_t_integer   res;
    cma_t_boolean   done = cma_c_false;
    int		    save_fromlen;
    cma_t_boolean   bad_wait = cma_c_false; 

    if (!cma__is_multithreaded()) 
	return (recvfrom (socket_, buf, length, flags, from, fromlen));

    if(!cma__hp_fd_reserve(socket_))
	return (cma__set_errno(EBADF), -1);

    if (fromlen) save_fromlen = *fromlen;

    if (cma__g_file[socket_]->non_blocking)
	if (!cma__g_file[socket_]->set_non_blocking)
	    cma__set_nonblocking(socket_); 

    if (cma__g_file[socket_]->non_blocking || 
	cma__g_file[socket_]->user_fl.user_non_blocking) {
        cma__interrupt_disable (0);
	TRY {
	    res = recvfrom (socket_, buf, length, flags, from, fromlen);
	    }
	CATCH_ALL {
            cma__interrupt_enable (0);
            cma__fd_unreserve (socket_);
            RERAISE;
            }
        ENDTRY
        cma__interrupt_enable (0);

	if ((res != -1)
		|| (!cma__is_blocking (errno))
		|| (cma__g_file[socket_]->user_fl.user_non_blocking))
	    done = cma_c_true;

	}

    if (done) {
	cma__fd_unreserve (socket_);
	}
    else {
	TRY {
	    cma__io_prolog (cma__c_io_read, socket_);

	    while (!done) {
		cma__io_wait (cma__c_io_read, socket_, &bad_wait);
		if (bad_wait) 
		    done = cma_c_true;
		else {
		    if (fromlen) *fromlen = save_fromlen;
		    res = recvfrom (socket_, buf, length, flags, from, fromlen);

		    if ((res != -1)
			|| (!cma__is_blocking (errno))
			|| (cma__g_file[socket_]->user_fl.user_non_blocking))
			done = cma_c_true;
		    }

		}

	    }
	FINALLY {
	    if (!bad_wait) { 
		cma__io_epilog (cma__c_io_read, socket_);
		cma__fd_unreserve (socket_);
	        }
	    }
	ENDTRY
	}

    if (bad_wait) return( cma__set_errno(EBADF), -1 ); 
    return res;
#else

    cma_t_integer   res;
    cma_t_boolean   done = cma_c_false;
    int		    save_fromlen;

    if (!cma__is_multithreaded()) 
	return (recvfrom (socket_, buf, length, flags, from, fromlen));

    if ((socket_ < 0) || (socket_ >= cma__g_mx_file)) 
	return (errno = EBADF, -1);

    if (!cma__is_open(socket_)) return (errno = EBADF, -1);

    cma__fd_reserve (socket_);
    /*
     * See if the file for some reason got closed during possible
     * block in cma__fd_reserve
     */
    if (!cma__is_open(socket_))
        return (cma__set_errno (EBADF), -1);

    if (fromlen) save_fromlen = *fromlen;

    if (cma__g_file[socket_]->non_blocking)
	if (!cma__g_file[socket_]->set_non_blocking)
	    cma__set_nonblocking(socket_); 

    if (cma__g_file[socket_]->non_blocking || 
	cma__g_file[socket_]->user_fl.user_non_blocking) {
        cma__interrupt_disable (0);
	TRY {
	    res = recvfrom (socket_, buf, length, flags, from, fromlen);
	    }
	CATCH_ALL {
            cma__interrupt_enable (0);
            cma__fd_unreserve (socket_);
            RERAISE;
            }
        ENDTRY
        cma__interrupt_enable (0);

	if ((res != -1)
		|| (!cma__is_blocking (errno))
		|| (cma__g_file[socket_]->user_fl.user_non_blocking))
	    done = cma_c_true;

	}

    if (done) {
	cma__fd_unreserve (socket_);
	}
    else {
	TRY {
	    cma__io_prolog (cma__c_io_read, socket_);

	    while (!done) {
		cma__io_wait (cma__c_io_read, socket_);
		if (fromlen) *fromlen = save_fromlen;
		res = recvfrom (socket_, buf, length, flags, from, fromlen);

		if ((res != -1)
			|| (!cma__is_blocking (errno))
			|| (cma__g_file[socket_]->user_fl.user_non_blocking))
		    done = cma_c_true;

		}

	    }
	FINALLY {
	    cma__io_epilog (cma__c_io_read, socket_);
	    cma__fd_unreserve (socket_);
	    }
	ENDTRY
	}

    return res;
#endif
    }



/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma_recvmsg:  thread-synchronous version of the BSD recvmsg system call
 *
 *  FORMAL PARAMETERS:
 *
 *	socket		    fd of socket to receive message on
 *	msg		    address of message header
 *	flags		    flags
 *
 *  IMPLICIT INPUTS:
 *
 *	The CMA I/O database
 *
 *  IMPLICIT OUTPUTS:
 *
 *	None
 *
 *  FUNCTION VALUE:
 *
 *	The number of characters received, or -1 in case of error
 *
 *  SIDE EFFECTS:
 *
 *	The current thread may block
 */

extern int
cma_recvmsg(
	int		socket_,
	struct msghdr	*msg,
	int		flags)
    {
    /*
     * We can't use cma__ts_func(recvmsg...) directly, because if recvmsg
     * returns with nothing read, then "fromlen" gets set to zero, and will
     * remain that way when we redo the call. Instead, break it up and save
     * the original length. This also happens with the msg_accrightslen field
     * so we need to save and restore it as well.
     *    cma__ts_func (
     *      recvmsg, 
     *      socket_, 
     *      (socket_, msg, flags), 
     *      cma__c_io_read, 
     *      0);
     */
#ifdef _HP_CMA_ASYNC_CLOSE_
    cma_t_integer   res;
    cma_t_boolean   done = cma_c_false;
    int		    save_fromlen;
    cma_t_integer   save_accrightslen;
    cma_t_integer   *save_accrights;
    cma_t_boolean   bad_wait = cma_c_false; 
    cma_t_integer   len;

    if (!cma__is_multithreaded()) return (recvmsg (socket_, msg, flags));

    if(!cma__hp_fd_reserve(socket_))
	return (cma__set_errno(EBADF), -1);

    save_fromlen = msg->msg_namelen;
    save_accrightslen = msg->msg_accrightslen;
    save_accrights = (cma_t_integer *)msg->msg_accrights;

    if (cma__g_file[socket_]->non_blocking)
	if (!cma__g_file[socket_]->set_non_blocking)
	    cma__set_nonblocking(socket_);

    if (cma__g_file[socket_]->non_blocking || 
	cma__g_file[socket_]->user_fl.user_non_blocking) {
        cma__interrupt_disable (0);
	TRY {
	    res = recvmsg (socket_, msg, flags);
	    }
	CATCH_ALL {
            cma__interrupt_enable (0);
            cma__fd_unreserve (socket_);
            RERAISE;
            }
        ENDTRY
        cma__interrupt_enable (0);

	if ((res != -1)
		|| (!cma__is_blocking (errno))
		|| (cma__g_file[socket_]->user_fl.user_non_blocking))
	    done = cma_c_true;

	}

    if (done) {
	cma__fd_unreserve (socket_);
	}
    else {
	TRY {
	    cma__io_prolog (cma__c_io_read, socket_);

	    while (!done) {
		cma__io_wait (cma__c_io_read, socket_, &bad_wait);
		if (bad_wait) 
		    done = cma_c_true;
		else {
		    msg->msg_namelen = save_fromlen;
		    msg->msg_accrightslen = save_accrightslen;
		    res = recvmsg (socket_, msg, flags);

		    if ((res != -1)
			|| (!cma__is_blocking (errno))
			|| (cma__g_file[socket_]->user_fl.user_non_blocking))
			done = cma_c_true;
		    }
		}

	    }
	FINALLY {
	    if (!bad_wait) { 
		cma__io_epilog (cma__c_io_read, socket_);
		cma__fd_unreserve (socket_);
	        }
	    }
	ENDTRY
	}

    if (bad_wait) return( cma__set_errno(EBADF), -1 ); 

    /*
     * Assume that if the accrights field is non-NULL, that the msg
     * contains valid file descriptors being passed from another process.
     * We need to import them to make CMA aware of their existence.
     */
    if ((res != -1) && (msg->msg_accrights != cma_c_null_ptr)) {
	len = msg->msg_accrightslen;
	while (len > 0) {
	    cma__open_general(*save_accrights++);
	    len -= sizeof(*save_accrights);
	}
    }
    return res;
#else

    cma_t_integer   res;
    cma_t_boolean   done = cma_c_false;
    int		    save_fromlen;

    if (!cma__is_multithreaded()) return (recvmsg (socket_, msg, flags));

    if ((socket_ < 0) || (socket_ >= cma__g_mx_file)) 
	return (errno = EBADF, -1);

    if (!cma__is_open(socket_)) return (errno = EBADF, -1);

    cma__fd_reserve (socket_);
    /*
     * See if the file for some reason got closed during possible
     * block in cma__fd_reserve
     */
    if (!cma__is_open(socket_))
        return (cma__set_errno (EBADF), -1);

    save_fromlen = msg->msg_namelen;

    if (cma__g_file[socket_]->non_blocking)
	if (!cma__g_file[socket_]->set_non_blocking)
	    cma__set_nonblocking(socket_);

    if (cma__g_file[socket_]->non_blocking || 
	cma__g_file[socket_]->user_fl.user_non_blocking) {
        cma__interrupt_disable (0);
	TRY {
	    res = recvmsg (socket_, msg, flags);
	    }
	CATCH_ALL {
            cma__interrupt_enable (0);
            cma__fd_unreserve (socket_);
            RERAISE;
            }
        ENDTRY
        cma__interrupt_enable (0);

	if ((res != -1)
		|| (!cma__is_blocking (errno))
		|| (cma__g_file[socket_]->user_fl.user_non_blocking))
	    done = cma_c_true;

	}

    if (done) {
	cma__fd_unreserve (socket_);
	}
    else {
	TRY {
	    cma__io_prolog (cma__c_io_read, socket_);

	    while (!done) {
		cma__io_wait (cma__c_io_read, socket_);
		msg->msg_namelen = save_fromlen;
		res = recvmsg (socket_, msg, flags);

		if ((res != -1)
			|| (!cma__is_blocking (errno))
			|| (cma__g_file[socket_]->user_fl.user_non_blocking))
		    done = cma_c_true;

		}

	    }
	FINALLY {
	    cma__io_epilog (cma__c_io_read, socket_);
	    cma__fd_unreserve (socket_);
	    }
	ENDTRY
	}

    return res;
#endif  /*  _HP_CMA_ASYNC_CLOSE_ */
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma_select:  thread-synchronous version of the BSD select system call
 *
 *  FORMAL PARAMETERS:
 *
 *	nfds		    the range of descriptors (ie, 0 through nfds-1)
 *	readfds		    mask of reader fd's
 *	writefds	    mask od writer fd's
 *	exceptfds	    mask of exception conds
 *	timeout		    timeout time of this request 
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
 *	The number of file descriptors with the specified I/O available
 *
 *  SIDE EFFECTS:
 *
 *	The current thread may block
 */

#ifdef  _HP_CMA_ASYNC_CLOSE_
extern int
cma_select(
	int		nfds,
	int		*readfds,
	int		*writefds,
	int		*exceptfds,
	struct timeval	*timeout)
    {
    struct timeval	no_wait;
    cma__t_file_mask	rfds, wfds, efds;
    cma_t_date_time	abs_time;
    cma_t_integer	fd, result;
    cma__t_int_tcb	*cur_tcb;
    cma_t_boolean	init_error;

    if (!cma__is_multithreaded()) 
	return (select (nfds, readfds, writefds, exceptfds, timeout));
    cma__assert_not_kernel ();
    cma__zero (&no_wait);

    /*
     * If nfds has an unreasonable value, use the whole mask.  It seems really
     * atrocious not to report any error, but this is what the "real" select
     * does, so we have to emulate this behavior too, I guess.
     */
    if ((nfds < 0) || (nfds > cma__g_mx_file))
	nfds = cma__g_mx_file;

    /*
     * Check usec parameter of timeout to make sure it is legal.
     * Return EINVAL if not.
     */
    if (timeout)
    {
	if (timeout->tv_sec > MAX_ALARM)
	    timeout->tv_sec = MAX_ALARM;
	if ((timeout->tv_usec < 0) || (timeout->tv_usec > 999999))
	    return (cma__set_errno(EINVAL), -1);
    }

    /*
     * Make copies of the file descriptor masks, since the call to select 
     * will destroy them.
     */
    cma__fdm_zero (&rfds);
    cma__fdm_zero (&wfds);
    cma__fdm_zero (&efds);
    cma__fdm_copy (readfds,   &rfds, nfds);
    cma__fdm_copy (writefds,  &wfds, nfds);
    cma__fdm_copy (exceptfds, &efds, nfds);

    /*
     * attempt real select, with immediate timeout
     */
    result = select (nfds, readfds, writefds, exceptfds, &no_wait);

    /*
     * If we get a positive result or an error, return now.
     */
    if (result != 0) return result;

    /*
     * Return now if the caller specified an immediate timeout.
     */
    if (timeout)
	if ((timeout->tv_sec  == 0) && (timeout->tv_usec == 0)) return result;

    /*
     * The select would have blocked, so set up to block the thread and 
     * awaken it later, when one of the selected files is ready, or when
     * the timeout, if specified, has elapsed.
     */

    /*
     * The client code may not have initialized CMA yet.  Do this on the 
     * blocking (slow) path, since it is not needed above.
     */
#ifndef _HP_LIBC_R
    TRY {
	init_error = cma_c_false;
        cma__int_init();
	}
    CATCH_ALL {
	init_error = cma_c_true;
	}
    ENDTRY
    if (init_error) return (cma__set_errno (ENOMEM), -1);
#endif

    /*
     * Bump up the waiters field in the file object
     * for each file.  Will unbump and return false
     * if we get an EBADF.
     */
    cma__int_lock (cma__g_close_select_mutex);	/* Prevent race with close */

    if(!cma__hp_bump_waiters_up(&rfds, &wfds, &efds, nfds)) {
        cma__fdm_copy (&rfds, readfds,   nfds);
        cma__fdm_copy (&wfds, writefds,  nfds);
        cma__fdm_copy (&efds, exceptfds, nfds);
	return( cma__set_errno(EBADF), -1 );
	}

    /*
     * Copy the select info into the tcb and block
     */
    cur_tcb = cma__get_self_tcb ();
    cma__fdm_zero (cur_tcb->select.rfds);
    cma__fdm_zero (cur_tcb->select.wfds);
    cma__fdm_zero (cur_tcb->select.efds);
    cma__fdm_copy (&rfds, cur_tcb->select.rfds, nfds);
    cma__fdm_copy (&wfds, cur_tcb->select.wfds, nfds);
    cma__fdm_copy (&efds, cur_tcb->select.efds, nfds);
    cur_tcb->select.nfound = 0;

    /*
     * Now insert the thread on the select queue and set the bits
     * in the wait_fds masks. This has to be done with the kernel
     * locked so that we don't allow cma__io_available to sneak in
     * and run cma__hp_notify_bad_select. This would cause this thread
     * to be flagged as having closed fd's on the select if the wait_fds
     * bits are not yet set.  Make sure we have the cma__g_io_data_mutex
     * before going into the kernel so we can safely set the  wait_fds
     * bits
     */
    cma__int_lock( cma__g_io_data_mutex );
    cma__enter_kernel();
    cma__queue_insert (&(cur_tcb->select.queue), &cma__g_select_queue);

    /*
     * Set the bits in the wait_fds masks.
     */

    cma__fdm_or(&cma__g_wait_fds[(int)cma__c_io_read],
		&cma__g_wait_fds[(int)cma__c_io_read],
		&rfds );
    cma__fdm_or(&cma__g_wait_fds[(int)cma__c_io_write],
		&cma__g_wait_fds[(int)cma__c_io_write],
		&wfds );
    cma__fdm_or(&cma__g_wait_fds[(int)cma__c_io_except],
		&cma__g_wait_fds[(int)cma__c_io_except],
		&efds );
    cma__exit_kernel();
    cma__int_unlock( cma__g_io_data_mutex );

    /*
     * If specified, convert the timeout from a delta time to an absolute
     * time.
     */
    if (timeout) {
	/*
	 * Add timeout (delta) to current (absolute) time to make an
	 * absolute timeout time.
	 */
	cma__get_time (&abs_time);
	cma__add_time (&abs_time, &abs_time, (cma_t_date_time *)timeout);
	}

    /*
     * Now block on a condition variable, so that we can be alerted, until
     * one or more of the files selected files are ready or until we time out.
     */
    cma__int_unlock (cma__g_close_select_mutex);
    cma__int_lock (cur_tcb->tswait_mutex);

    TRY {
	while (cur_tcb->select.nfound == 0) {
	    if (timeout) {
		if (cma_s_timed_out == cma__int_timed_wait (
			    cur_tcb->tswait_cv,
			    cur_tcb->tswait_mutex,
			    &abs_time))
		    break;
		}
	    else
		cma__int_wait (
			cur_tcb->tswait_cv,
			cur_tcb->tswait_mutex);
	    }
	}
    FINALLY {
	cma__int_unlock (cur_tcb->tswait_mutex);

	/*
	 * Bump waiters down, _and_ unset wait_fds as necessary. 
         * If a bad file is detected while bumping down, set 
         * nfound = -1. Make sure to remove the struct from queue 
         * before bumping down so that io_available doesn't 
         * think we are waiting on a closed file. 
	 */
	cma__enter_kernel();
	cma__queue_remove (&(cur_tcb->select.queue));
	cma__exit_kernel();

        if(!cma__hp_bump_waiters_down(&rfds, &wfds, &efds, nfds))
	    cur_tcb->select.nfound = -1;
        }
    ENDTRY

    /* Bad file was detected either by cma__hp_notify_bad_select() while 
     * blocked or by cma__hp_bump_waiters_down() after returning from wait.
     */
    if( cur_tcb->select.nfound == -1 ) {
        cma__fdm_copy (&rfds, readfds,   nfds);
        cma__fdm_copy (&wfds, writefds,  nfds);
        cma__fdm_copy (&efds, exceptfds, nfds);
	return( cma__set_errno(EBADF), -1 );
	}
    else {
        cma__fdm_copy (cur_tcb->select.rfds, readfds,   nfds);
        cma__fdm_copy (cur_tcb->select.wfds, writefds,  nfds);
        cma__fdm_copy (cur_tcb->select.efds, exceptfds, nfds);
        return( cur_tcb->select.nfound );
	}
    }    

#else  /* _HP_CMA_ASYNC_CLOSE_ */

extern int
cma_select(
	int		nfds,
	int		*readfds,
	int		*writefds,
	int		*exceptfds,
	struct timeval	*timeout)
    {
    struct timeval	no_wait;
    cma__t_file_mask	rfds, wfds, efds;
    cma_t_date_time	abs_time;
    cma_t_integer	fd, result;
    cma__t_int_tcb	*cur_tcb;
    cma_t_boolean	init_error;


    if (!cma__is_multithreaded()) 
	return (select (nfds, readfds, writefds, exceptfds, timeout));
    cma__assert_not_kernel ();
    cma__zero (&no_wait);

    /*
     * If nfds has an unreasonable value, use the whole mask.  It seems really
     * atrocious not to report any error, but this is what the "real" select
     * does, so we have to emulate this behavior too, I guess.
     */
    if ((nfds < 0) || (nfds > cma__g_mx_file))
	nfds = cma__g_mx_file;

    /*
     * Check usec parameter of timeout to make sure it is legal.
     * Return EINVAL if not.
     */
    if (timeout)
    {
	if (timeout->tv_sec > MAX_ALARM)
	    timeout->tv_sec = MAX_ALARM;
	if ((timeout->tv_usec < 0) || (timeout->tv_usec > 999999))
	    return (cma__set_errno(EINVAL), -1);
    }

    /*
     * Make copies of the file descriptor masks, since the call to select 
     * will destroy them.
     */
    cma__fdm_copy (readfds,   &rfds, nfds);
    cma__fdm_copy (writefds,  &wfds, nfds);
    cma__fdm_copy (exceptfds, &efds, nfds);

    /*
     * attempt real select, with immediate timeout
     */
    result = select (nfds, readfds, writefds, exceptfds, &no_wait);

    /*
     * If we get a positive result or an error, return now.
     */
    if (result != 0) return result;

    /*
     * Return now if the caller specified an immediate timeout.
     */
    if (timeout)
	if ((timeout->tv_sec  == 0) && (timeout->tv_usec == 0)) return result;

    /*
     * The select would have blocked, so set up to block the thread and 
     * awaken it later, when one of the selected files is ready, or when
     * the timeout, if specified, has elapsed.
     */

    /*
     * The client code may not have initialized CMA yet.  Do this on the 
     * blocking (slow) path, since it is not needed above.
     */
#ifndef _HP_LIBC_R
    TRY {
	init_error = cma_c_false;
        cma__int_init();
	}
    CATCH_ALL {
	init_error = cma_c_true;
	}
    ENDTRY
    if (init_error) return (cma__set_errno (ENOMEM), -1);
#endif

    /*
     * Copy the select info into the tcb and block
     */
    cur_tcb = cma__get_self_tcb ();
    cma__fdm_zero (cur_tcb->select.rfds);
    cma__fdm_zero (cur_tcb->select.wfds);
    cma__fdm_zero (cur_tcb->select.efds);
    cma__fdm_copy (&rfds, cur_tcb->select.rfds, nfds);
    cma__fdm_copy (&wfds, cur_tcb->select.wfds, nfds);
    cma__fdm_copy (&efds, cur_tcb->select.efds, nfds);
    cur_tcb->select.nfound = 0;

    /*
     * Set the bits in the global fd masks
     */
    for (fd = 0; fd < nfds; fd++) {
	if     (cma__fdm_is_set (fd, &rfds) ||
		cma__fdm_is_set (fd, &wfds) ||
		cma__fdm_is_set (fd, &efds)) {

	    cma__int_lock (cma__g_file[fd]->mutex);
	    cma__int_lock (cma__g_io_data_mutex);

	    if (fd >= cma__g_file_num) 
		cma__g_file_num = fd + 1;

	    if cma__fdm_is_set (fd, &rfds) {
		cma__fdm_set_bit (fd, &cma__g_wait_fds[(int)cma__c_io_read]);
		cma__g_file[fd]->waiters[(int)cma__c_io_read]++;
		}
	    if cma__fdm_is_set (fd, &wfds) {
		cma__fdm_set_bit (fd, &cma__g_wait_fds[(int)cma__c_io_write]);
		cma__g_file[fd]->waiters[(int)cma__c_io_write]++;
		}
	    if cma__fdm_is_set (fd, &efds) {
		cma__fdm_set_bit (fd, &cma__g_wait_fds[(int)cma__c_io_except]);
		cma__g_file[fd]->waiters[(int)cma__c_io_except]++;
		}

	    cma__int_unlock (cma__g_io_data_mutex);
	    cma__int_unlock (cma__g_file[fd]->mutex);
	    }
	}

    /*
     * If specified, convert the timeout from a delta time to an absolute
     * time.
     */
    if (timeout) {
	/*
	 * Add timeout (delta) to current (absolute) time to make an
	 * absolute timeout time.
	 */
	cma__get_time (&abs_time);
	cma__add_time (&abs_time, &abs_time, (cma_t_date_time *)timeout);
	}

    /*
     * Now block on a condition variable, so that we can be alerted, until
     * one or more of the files selected files are ready or until we time out.
     */
    cma__int_lock (cur_tcb->tswait_mutex);

    TRY {
	cma__enter_kernel();
	cma__queue_insert (&(cur_tcb->select.queue), &cma__g_select_queue);
	cma__exit_kernel();

	while (cur_tcb->select.nfound == 0) {
	    if (timeout) {
		if (cma_s_timed_out == cma__int_timed_wait (
			    cur_tcb->tswait_cv,
			    cur_tcb->tswait_mutex,
			    &abs_time))
		    break;
		}
	    else
		cma__int_wait (
			cur_tcb->tswait_cv,
			cur_tcb->tswait_mutex);
	    }
	}
    FINALLY {
	cma__enter_kernel();
	(void)cma__queue_remove (&(cur_tcb->select.queue));
	cma__exit_kernel();

	cma__int_unlock (cur_tcb->tswait_mutex);

	/*
	 * Clear the bits (if appropriate) that we set in the global fd masks.
	 */
	for (fd = 0; fd < nfds; fd++) {
	    if (cma__fdm_is_set (fd, &rfds)) {
		cma__int_lock (cma__g_file[fd]->mutex);
		if ((--cma__g_file[fd]->waiters[(int)cma__c_io_read]) == 0) {
		    cma__int_lock (cma__g_io_data_mutex);
		    cma__fdm_clr_bit (
			    fd, 
			    &cma__g_wait_fds[(int)cma__c_io_read]);
		    cma__int_unlock (cma__g_io_data_mutex);
		    }
		cma__int_unlock (cma__g_file[fd]->mutex);
		}

	    if (cma__fdm_is_set (fd, &wfds)) {
		cma__int_lock (cma__g_file[fd]->mutex);
		if ((--cma__g_file[fd]->waiters[(int)cma__c_io_write]) == 0) {
		    cma__int_lock (cma__g_io_data_mutex);
		    if cma__fdm_is_set (fd, &wfds)
			cma__fdm_clr_bit (
				fd, 
				&cma__g_wait_fds[(int)cma__c_io_write]);
		    cma__int_unlock (cma__g_io_data_mutex);
		    }
		cma__int_unlock (cma__g_file[fd]->mutex);
		}

	    if (cma__fdm_is_set (fd, &efds)) {
		cma__int_lock (cma__g_file[fd]->mutex);
		if ((--cma__g_file[fd]->waiters[(int)cma__c_io_except]) == 0) {
		    cma__int_lock (cma__g_io_data_mutex);
		    if cma__fdm_is_set (fd, &efds)
			cma__fdm_clr_bit (
				fd, 
				&cma__g_wait_fds[(int)cma__c_io_except]);
		    cma__int_unlock (cma__g_io_data_mutex);
		    }
		cma__int_unlock (cma__g_file[fd]->mutex);
		}
	    }
	}
    ENDTRY

    cma__fdm_copy (cur_tcb->select.rfds, readfds,   nfds);
    cma__fdm_copy (cur_tcb->select.wfds, writefds,  nfds);
    cma__fdm_copy (cur_tcb->select.efds, exceptfds, nfds);

    return cur_tcb->select.nfound;
    }    
#endif  /* _HP_CMA_ASYNC_CLOSE_ */

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma_send:  send a message from a socket
 *
 *  FORMAL PARAMETERS:
 *
 *	socket_		fd of socket to receive message on
 *	buf		address of buffer to receive message into
 *	length		length of buffer
 *	flags		options which modify the operation
 *
 *  IMPLICIT INPUTS:
 *
 *	The CMA I/O database
 *
 *  IMPLICIT OUTPUTS:
 *
 *	None
 *
 *  FUNCTION VALUE:
 *
 *	The number of characters sent, or -1 in case of error
 *
 *  SIDE EFFECTS:
 *
 *	The current thread may block.
 */
extern int
cma_send(
	int	socket_,
	void    *buf,
	int	length,
	int	flags)
    {
    cma__ts_func2 (
	    send, 
	    socket_, 
	    (socket_, buf, length, flags), 
	    cma__c_io_write,
	    buf,
	    length,
	    0);
    }


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma_sendmsg:  thread-synchronous version of the BSD sendmsg system call
 *
 *  FORMAL PARAMETERS:
 *
 *	socket		    socket to receive msg
 *	msg		    message to be sent
 *	flags		    how to send
 *
 *  IMPLICIT INPUTS:
 *
 *	The CMA I/O database
 *
 *  IMPLICIT OUTPUTS:
 *
 *	None
 *
 *  FUNCTION VALUE:
 *
 *	The number of characters sent, or -1 in case of error
 *
 *  SIDE EFFECTS:
 *
 *	The current thread may block
 */
extern int
cma_sendmsg(
	int		socket_,
	const struct msghdr	msg[],
	int		flags)
    {
    /*
     * We can't use cma__ts_func(sendmsg...) directly, because if sendmsg
     * is being used to pass an fd to another process via the msg_accrights
     * field, then we must clear the CMA non-blocking settings first.
     *    cma__ts_func (
     *      sendmsg, 
     *      socket_, 
     *      (socket_, msg, flags), 
     *      cma__c_io_write, 
     *      0);
     */
#ifdef _HP_CMA_ASYNC_CLOSE_
    cma_t_integer   res;
    cma_t_boolean   done = cma_c_false;
    cma_t_boolean   bad_wait = cma_c_false; 
    cma_t_integer   *fd_ptr;
    cma_t_integer   len;

    if (!cma__is_multithreaded()) return (sendmsg (socket_, msg, flags));
    if(!cma__hp_fd_reserve(socket_))
	return (cma__set_errno(EBADF), -1);

    /*
     * If the msg_accrightslen is non-zero, assume that the user is
     * passing file descriptors to another process so we must clear
     * the CMA non-blocking settings first.
     */
    if (msg->msg_accrights != cma_c_null_ptr) { 
	fd_ptr = (cma_t_integer *)msg->msg_accrights;
	len = msg->msg_accrightslen;
	while (len > 0) {
	    cma__set_blocking(*fd_ptr++);
	    len -= sizeof(*fd_ptr);
	}
    }

    if (cma__g_file[socket_]->non_blocking)
	if (!cma__g_file[socket_]->set_non_blocking)
	    cma__set_nonblocking(socket_);

    if (cma__g_file[socket_]->non_blocking || 
	cma__g_file[socket_]->user_fl.user_non_blocking) {
        cma__interrupt_disable (0);
	TRY {
	    res = sendmsg (socket_, msg, flags);
	    }
	CATCH_ALL {
            cma__interrupt_enable (0);
            cma__fd_unreserve (socket_);
            RERAISE;
            }
        ENDTRY
        cma__interrupt_enable (0);

	if ((res != -1)
		|| (!cma__is_blocking (errno))
		|| (cma__g_file[socket_]->user_fl.user_non_blocking))
	    done = cma_c_true;

	}

    if (done) {
	cma__fd_unreserve (socket_);
	}
    else {
	TRY {
	    cma__io_prolog (cma__c_io_write, socket_);

	    while (!done) {
		cma__io_wait (cma__c_io_write, socket_, &bad_wait);
		if (bad_wait) 
		    done = cma_c_true;
		else {
		    res = sendmsg (socket_, msg, flags);

		    if ((res != -1)
			|| (!cma__is_blocking (errno))
			|| (cma__g_file[socket_]->user_fl.user_non_blocking))
			done = cma_c_true;
		    }
		}

	    }
	FINALLY {
	    if (!bad_wait) { 
		cma__io_epilog (cma__c_io_write, socket_);
		cma__fd_unreserve (socket_);
	        }
	    }
	ENDTRY
	}

    if (bad_wait) return( cma__set_errno(EBADF), -1 ); 
    return res;
    }
#else
    cma__ts_func (sendmsg, socket_, (socket_, msg, flags), cma__c_io_write, 0);
    }
#endif  /*  _HP_CMA_ASYNC_CLOSE_ */

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma_sendto:  thread-synchronous version of the BSD sendto system call
 *
 *  FORMAL PARAMETERS:
 *
 *	socket		socket to receive msg
 *	msg		message to be sent
 *	length		length of message
 *	flags		how to send
 *	to		socket address of destination 
 *	tolen		size of socket address block
 *
 *  IMPLICIT INPUTS:
 *
 *	The CMA I/O database
 *
 *  IMPLICIT OUTPUTS:
 *
 *	None
 *
 *  FUNCTION VALUE:
 *
 *	The number of characters sent, or -1 in case of error
 *
 *  SIDE EFFECTS:
 *
 *	The current thread may block
 */

extern int
cma_sendto(
	int		socket_,
	const void	*msg,
	int		length,
	int		flags,
	const void	*to,
	int		tolen)
    {
    cma__ts_func (
	    sendto, 
	    socket_, 
	    (socket_, msg, length, flags, to, tolen),
	    cma__c_io_write,
	    0);
    }



/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma_socket:  create a socket 
 *
 *  FORMAL PARAMETERS:
 *
 *	domain		    communication domain to operate
 *	type		    socket type to create
 *	protocol	    protocol to use
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
 *	The socket file descriptor
 *
 *  SIDE EFFECTS:
 *
 *	None
 */
extern int
cma_socket(
	int	domain,
	int	type,
	int	protocol)
    {
    cma__ts_open (socket, 
	    (domain, type, protocol), 
	    cma__open_general (__fd__));
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma_socketpair:  create a pair of connected sockets
 *
 *  FORMAL PARAMETERS:
 *
 *	domain		    communication domain to operate
 *	type		    socket type to create
 *	protocol	    protocol to use
 *	sv		    array of two fd's for the sockets
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
 *	0 indicates success, -1 on error
 *
 *  SIDE EFFECTS:
 *
 *	None
 */
extern int
cma_socketpair(
	int	d,
	int	type,
	int	protocol,
	int	sv[2])
    {
    cma__ts_open2 (socketpair, 
	    sv, 
	    (d, type, protocol, sv), 
	    (cma__open_general (sv[0]), cma__open_general (sv[1])));
    }

#ifdef _HP_CMA_ASYNC_CLOSE_

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma__hp_bump_waiters_up:  bump the waiters counters in the file objects up.
 *
 *      Utility function used only by cma_select; bumps waiters counters in 
 *      file objects, checking for bad files at the same time. Unbump files you've
 *      bumped if you hit a bad file. If a bad file is found, return false.
 *
 *  FORMAL PARAMETERS:
 *
 *	rfds	            mask of reader fd's
 *	wfds	            mask od writer fd's
 *	efds	            mask of exception conds
 *	nfds		    the range of descriptors (ie, 0 through nfds-1)
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
 *	cma_c_false indicates a bad fd was found, cma_c_true on normal return
 *
 *  SIDE EFFECTS:
 *
 *	None
 */
static cma_t_boolean
cma__hp_bump_waiters_up
	( cma__t_file_mask *rfds,
	  cma__t_file_mask *wfds,
	  cma__t_file_mask *efds,
	  int nfds )
{
	cma_t_boolean no_bad_files = cma_c_true;
	int fd;
	cma__t_int_tcb	*cur_tcb;
	cma__t_fo_id  *select_save_fo;

	/*
	 * We must save the pointers to any file objects that we are
         * selecting on so that if the file was closed during the wait,
         * we still have a way to bump down the waiters in the file object.
	 */
	cur_tcb = cma__get_self_tcb ();
	if (nfds > cur_tcb->select_save_nfds) {
	    if (cur_tcb->select_save_fo != cma_c_null_ptr)
		cma__free_mem ((cma_t_address)cur_tcb->select_save_fo);
	    cur_tcb->select_save_fo = (cma_t_address)cma__alloc_mem (
		sizeof (cma__t_fo_id) * nfds);
	    cur_tcb->select_save_nfds = nfds;
	}
	select_save_fo = (cma__t_fo_id *)cur_tcb->select_save_fo;
	
	/*
	 *  Bump up waiters for any of the files being selected.  If a closed
         *  file is found, bump back down and return.
	 */
	fd = 0;
	for (fd = 0; fd < nfds; fd++) { 
	    if( cma__fdm_is_set( fd, rfds ) ||
		cma__fdm_is_set( fd, wfds ) ||
		cma__fdm_is_set( fd, efds ) ) {

		if(cma__hp_fd_reserve(fd)) {
		    cma__hp_save_fid(&select_save_fo[fd], fd);
		    cma__int_lock (cma__g_io_data_mutex);
	
		    if (fd >= cma__g_file_num) 
			cma__g_file_num = fd + 1;

		    if(cma__fdm_is_set( fd, rfds ))
			cma__g_file[fd]->waiters[(int)cma__c_io_read]++;
		    if(cma__fdm_is_set( fd, wfds ))
			cma__g_file[fd]->waiters[(int)cma__c_io_write]++;
		    if(cma__fdm_is_set( fd, efds ))
			cma__g_file[fd]->waiters[(int)cma__c_io_except]++;
		    cma__int_unlock (cma__g_io_data_mutex);
		    cma__fd_unreserve(fd);
		}
		else {		/* !cma__hp_fd_reserve(fd) */
		    no_bad_files = cma_c_false;
		    /*
		     * We have found a closed file, so stop bumping up
                     * and go back and bump down any waiters that have been
                     * bumped up.
		     */
		    cma__hp_bump_waiters_down(rfds, wfds, efds, fd);
		    break;
		}
	    }
	}
	return( no_bad_files );
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma__hp_bump_waiters_down:  bump the waiters counters in the file objects
 *
 *      Utility function used only by cma_select; bumps waiters counters in 
 *      file objects, checking for bad files at the same time.  Bumps down
 *      the waiters in the file objects and unset io bits if count goes to zero.
 *      If a bad file is encountered, report it by returning false.
 *
 *  FORMAL PARAMETERS:
 *
 *	rfds	            mask of reader fd's
 *	wfds	            mask od writer fd's
 *	efds	            mask of exception conds
 *	nfds		    the range of descriptors (ie, 0 through nfds-1)
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
 *	cma_c_false indicates a bad fd was found, cma_c_true on normal return
 *
 *  SIDE EFFECTS:
 *
 *	None
 */
static cma_t_boolean
cma__hp_bump_waiters_down
	( cma__t_file_mask *rfds,
	  cma__t_file_mask *wfds,
	  cma__t_file_mask *efds,
	  int nfds )
{
	cma_t_boolean no_bad_files = cma_c_true;
	int fd;
	cma__t_int_tcb	*cur_tcb;
	cma__t_fo_id    *select_save_fo;

	cur_tcb = cma__get_self_tcb ();
	select_save_fo = (cma__t_fo_id *)cur_tcb->select_save_fo;

	/*
	 *  Bump down waiters for any of the files being selected.  If a closed
         *  file is found, return false. Note that the global file bit is only
         *  cleared if the file object points to an open file.  This protects
         *  against clearing the bit when other waiters exist from re-opening
         *  the file.
	 */
	for (fd = 0; fd < nfds; fd++) { 
	    if( cma__fdm_is_set( fd, rfds ) ||
		cma__fdm_is_set( fd, wfds ) ||
		cma__fdm_is_set( fd, efds ) ) {

		cma__int_lock (select_save_fo[fd].fo->mutex);
		cma__int_lock (cma__g_io_data_mutex);
		if(cma__fdm_is_set( fd, rfds )) {
		  if(((--select_save_fo[fd].fo->waiters[(int)cma__c_io_read]) == 0) &&
		     (select_save_fo[fd].fo->ref_count != 0))
		      cma__fdm_clr_bit(fd, &cma__g_wait_fds[(int)cma__c_io_read]);
		}
		if(cma__fdm_is_set( fd, wfds )) {
		  if(((--select_save_fo[fd].fo->waiters[(int)cma__c_io_write]) == 0) &&
		     (select_save_fo[fd].fo->ref_count != 0))
		      cma__fdm_clr_bit(fd, &cma__g_wait_fds[(int)cma__c_io_write]);
		}
		if(cma__fdm_is_set( fd, efds )) {
		  if(((--select_save_fo[fd].fo->waiters[(int)cma__c_io_except]) == 0) &&
		     (select_save_fo[fd].fo->ref_count != 0))
		      cma__fdm_clr_bit(fd, &cma__g_wait_fds[(int)cma__c_io_except]);
		}
		if (!cma__hp_fid_match(&select_save_fo[fd], fd))
		    no_bad_files = cma_c_false;
	    cma__int_unlock (cma__g_io_data_mutex);
	    cma__int_unlock (select_save_fo[fd].fo->mutex);
	    }
	}
	return( no_bad_files );
    }
#endif  /* _HP_CMA_ASYNC_CLOSE_ */

#endif					/* if !_CMA_THREAD_SYNC_IO_ */
