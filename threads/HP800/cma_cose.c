/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * @HP_COPYRIGHT@
 */
/*
 * HISTORY
 * $Log: cma_cose.c,v $
 * Revision 1.1.2.2  1996/10/03  15:16:41  arvind
 * 	Add OSF copyright
 * 	[1996/05/24  23:25 UTC  sommerfeld  /main/DCE_1.2.2/1]
 *
 * Revision 1.1.2.1  1996/06/05  14:19:41  arvind
 * 	Merge Spec 1170 changes for Walnut Creek.
 * 	[1995/07/10  20:08 UTC  jss  /main/HPDCE02/1]
 * 
 * 	Make it build.
 * 	[1995/06/23  17:42 UTC  jss  /main/jss_mothra_cose/2]
 * 
 * 	[1995/06/09  14:11 UTC  jss  /main/jss_mothra_cose/1]
 * 
 * $EndLog$
 */
/*
 *	Implement wrappers for COSE compliant functions
 */

/* 
 * This needs to be defined so that the COSE compliant structures are selected
 * from <sys/socket.h>
 */
#define _XOPEN_SOURCE_EXTENDED 

#include <cma.h>
#include <cma_thdio_defs.h>
#include <sys/socket.h>

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma_socket2:  create a COSE compliant socket 
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
cma_socket2(
	int	domain,
	int	type,
	int	protocol)
    {
    cma__ts_open (socket2, 
	    (domain, type, protocol), 
	    cma__open_general (__fd__));
    }


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma_socketpair2:  create a pair of COSE compliant connected sockets
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
cma_socketpair2(
	int	d,
	int	type,
	int	protocol,
	int	sv[2])
    {
    cma__ts_open2 (socketpair2, 
	    sv, 
	    (d, type, protocol, sv), 
	    (cma__open_general (sv[0]), cma__open_general (sv[1])));
    }


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma_sendmsg2: COSE compliant version of the BSD sendmsg system call
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
cma_sendmsg2(
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
    
    

    if (!cma__is_multithreaded()) return (sendmsg2 (socket_, msg, flags));
    if(!cma__hp_fd_reserve(socket_))
	return (cma__set_errno(EBADF), -1);

    /*
     * If the msg_control field is non-zero and the control message type is
     * SCM_RIGHTS, then the user is passing file descriptors to another process 
     * so we must clear the CMA non-blocking settings first.
     */
    if ((CMSG_FIRSTHDR(msg) != cma_c_null_ptr) 
	&& ((CMSG_FIRSTHDR(msg))->cmsg_type == SCM_RIGHTS)
	&& ((CMSG_FIRSTHDR(msg))->cmsg_level == SOL_SOCKET)) {
	fd_ptr = (cma_t_integer *)CMSG_DATA(CMSG_FIRSTHDR(msg));
	len = (CMSG_FIRSTHDR(msg))->cmsg_len - sizeof(struct cmsghdr);
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
	    res = sendmsg2 (socket_, msg, flags);
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
		    res = sendmsg2 (socket_, msg, flags);

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
    {
    cma__ts_func (sendmsg2, socket_, (socket_, msg, flags), cma__c_io_write, 0);
    }
#endif  /*  _HP_CMA_ASYNC_CLOSE_ */


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma_recvmsg2:  COSE compliant version of the BSD recvmsg system call
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
cma_recvmsg2(
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
    cma_t_integer   save_fromlen;
    cma_t_integer   save_controllen;
    cma_t_boolean   bad_wait = cma_c_false; 
    cma_t_integer   *fd_ptr;
    cma_t_integer   len;

    if (!cma__is_multithreaded()) return (recvmsg2 (socket_, msg, flags));

    if(!cma__hp_fd_reserve(socket_))
	return (cma__set_errno(EBADF), -1);

    save_fromlen = msg->msg_namelen;
    save_controllen = msg->msg_controllen;

    if (cma__g_file[socket_]->non_blocking)
	if (!cma__g_file[socket_]->set_non_blocking)
	    cma__set_nonblocking(socket_);

    if (cma__g_file[socket_]->non_blocking || 
	cma__g_file[socket_]->user_fl.user_non_blocking) {
        cma__interrupt_disable (0);
	TRY {
	    res = recvmsg2 (socket_, msg, flags);
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
		    msg->msg_controllen = save_controllen;
		    res = recvmsg2 (socket_, msg, flags);

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
     * If the msg_control field is non-zero and the control message type is
     * SCM_RIGHTS, then there are valid file descriptors being passed from 
     * another process. We need to import them to make CMA aware of its existence.
     */
    if ((res != -1)
	&& (CMSG_FIRSTHDR(msg) != cma_c_null_ptr)
	&& ((CMSG_FIRSTHDR(msg))->cmsg_type == SCM_RIGHTS)
	&& ((CMSG_FIRSTHDR(msg))->cmsg_level == SOL_SOCKET)) {
	fd_ptr = (cma_t_integer *)CMSG_DATA(CMSG_FIRSTHDR(msg));
	len = (CMSG_FIRSTHDR(msg))->cmsg_len - sizeof(struct cmsghdr);
	while (len > 0) {
	    cma__open_general(*fd_ptr++);
	    len -= sizeof(*fd_ptr);
	}
    }
    return res;
#else

    cma_t_integer   res;
    cma_t_boolean   done = cma_c_false;
    int		    save_fromlen;

    if (!cma__is_multithreaded()) return (recvmsg2 (socket_, msg, flags));

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
	    res = recvmsg2 (socket_, msg, flags);
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
		res = recvmsg2 (socket_, msg, flags);

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

#undef _XOPEN_SOURCE_EXTENDED 
