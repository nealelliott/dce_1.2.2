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
 * Revision 1.1.6.1  1996/05/10  13:31:53  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:53 UTC  bissen
 * 	Port single-threaded
 * 	[1996/05/09  21:02:30  arvind]
 *
 * 	Check to see if the file got closed while we were waiting for the mutex
 * 	in cma_fd_reserve.
 * 	[1993/03/10  19:39:09  jd]
 *
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  17:33:00  hinman]
 *
 * 	[hinman] - Check in merged SNI version
 * 	[1993/01/11  16:22:35  hinman]
 *
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:55:07  sekhar]
 *
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:06:57  bolinger]
 *
 * Revision 1.1.2.2  1994/06/09  13:35:47  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:26:19  devsrc]
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
 * Revision 1.1.2.1  1994/04/01  20:14:27  jd
 * 	Moved from src/threads to here
 * 	[1994/04/01  17:53:16  jd]
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
#include <sys/socket.h>
# include <sys/select.h>
#include <cma_errors.h>
#include <cma_tcb.h>
#include <cma_stack.h>
#include <cma_util.h>
#include <cma_ux.h>

/*
 * LOCAL DATA
 */



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
    cma__ts_func (
	    accept, 
	    socket_, 
	    (socket_, addr, addrlen), 
	    cma__c_io_read,
	    cma__open_general (__res__));
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
	const                   void *name,
	int			namelen)
    {
    cma_t_integer	res;
    cma_t_boolean	done = cma_c_false;
    cma__t_file_mask	rfds, wfds;

    if (!cma__is_multithreaded()) return (connect (socket_, name, namelen));
    /*
     * Make sure the file (socket) is OK by us.
     */
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
		    (void *)(&rfds), 
		    (void *)(&wfds), 
		    (void *)0, 
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
	    else if (res < 0)
		/*
		 * There shouldn't be any errors...  (We'll ignore EINTR.)
		 */
		if (errno != EINTR)
		    cma__bugcheck ("cma_connect: Error returned from cma_select");
	    }
	}

    return res;
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
	void 	        *from,
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

    if (cma__g_file[socket_]->non_blocking) {
	if (!cma__g_file[socket_]->set_non_blocking)
	    cma__set_nonblocking(socket_); 
        cma__interrupt_disable (0);
	res = recvfrom (socket_, buf, length, flags, from, fromlen);
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
     * returns with nothing read, then on some systems msg->msg_namelen
     * gets set to zero, and will remain that way when we redo the call.
     * Instead, break the call up and save the original length.
     *
	 * cma__ts_func (recvmsg,
	 * 		socket_,
	 * 		(socket_, msg, flags),
	 * 		cma__c_io_read,
	 * 		0);
     */
    cma_t_integer   res;
    cma_t_boolean   done = cma_c_false;
    int		    save_addrlen;

    if (!cma__is_multithreaded()) return (recvmsg (socket_, msg, flags));

    if ((socket_ < 0) || (socket_ >= cma__g_mx_file)) 
		return (errno = EBADF, -1);

    if (!cma__is_open(socket_))
		return (errno = EBADF, -1);

    cma__fd_reserve (socket_);
    /*
     * See if the file for some reason got closed during possible
     * block in cma__fd_reserve
     */
    if (!cma__is_open(socket_))
        return (cma__set_errno (EBADF), -1);

	save_addrlen = msg->msg_namelen;
    if (cma__g_file[socket_]->non_blocking) {
		if (!cma__g_file[socket_]->set_non_blocking)
			cma__set_nonblocking(socket_); 
        cma__interrupt_disable (0);
		res = recvmsg (socket_, msg, flags);
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
			msg->msg_namelen = save_addrlen;
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

extern int
cma_select(
	unsigned long		nfds,
	void		*readfds,
	void		*writefds,
	void		*exceptfds,
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
    TRY {
	init_error = cma_c_false;
        cma__int_init();
	}
    CATCH_ALL {
	init_error = cma_c_true;
	}
    ENDTRY
    if (init_error) return (cma__set_errno (ENOMEM), -1);

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
	const void	*buf,
	int	length,
	int	flags)
    {
    cma__ts_func (
	    send, 
	    socket_, 
	    (socket_, buf, length, flags), 
	    cma__c_io_write,
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
    cma__ts_func (sendmsg, socket_, (socket_, msg, flags), cma__c_io_write, 0);
    }

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
	void		*msg,
	int		length,
	int		flags,
	void	        *to,
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
#endif					/* if !_CMA_THREAD_SYNC_IO_ */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_THDIO_4.C */
/*  *13   14-NOV-1991 18:08:47 SCALES "Fix bug in cma_select" */
/*  *12   31-OCT-1991 12:39:47 BUTENHOF "Zero TCB file masks before copy (cma_select)" */
/*  *11   14-OCT-1991 18:41:11 CURTIN "added try/catch to cma_select" */
/*  *10   14-OCT-1991 13:41:21 BUTENHOF "Use new config symbols" */
/*  *9    24-SEP-1991 16:29:02 BUTENHOF "Handle variable # of FDs more efficiently" */
/*  *8    17-SEP-1991 13:24:29 BUTENHOF "Handle variable # of FDs more efficiently" */
/*  *7     4-SEP-1991 15:38:06 SCALES "Add validity check for first arg to cma_select" */
/*  *6    10-JUN-1991 18:24:24 SCALES "Add sccs headers for Ultrix" */
/*  *5    14-MAY-1991 13:46:23 BUTENHOF "Integrate changes lost in disk crash" */
/*  *4    10-MAY-1991 17:52:18 BUTENHOF "Use new macro to test for kernel locked" */
/*  *3     7-MAY-1991 10:13:01 CURTIN "Replaced call to cma_init w/cma__int_init" */
/*  *2     2-MAY-1991 22:28:42 SCALES "Fix a bug in cma_select" */
/*  *1     2-MAY-1991 16:26:54 CURTIN "Thread synchronous I/O cont" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_THDIO_4.C */
