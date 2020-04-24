/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 *
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems
 * Burlington, MA, USA
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: cma_thdio_2.c,v $
 * Revision 1.1.4.2  1996/02/17  23:32:37  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:48:58  marty]
 *
 * 	Check to see if the file got closed while we were waiting for the mutex
 * 	in cma_fd_reserve.
 * 	[1993/03/10  19:34:36  jd]
 *
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  17:32:34  hinman]
 *
 * 	[hinman@sni] - Correct SVR4 ifdefs
 * 	[1993/01/21  14:46:24  hinman]
 *
 * 	[hinman] - Check in merged SNI version
 * 	[1993/01/11  16:22:04  hinman]
 *
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:54:57  sekhar]
 *
 * Revision 1.1.4.1  1995/12/07  21:44:00  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  20:58:32  root]
 * 
 * Revision 1.1.2.2  1994/06/09  13:33:40  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:24:54  devsrc]
 * 
 * Revision 1.1.2.1  1994/04/01  20:13:12  jd
 * 	Moved from src/threads to here.
 * 	[1994/04/01  16:23:04  jd]
 * 
 * Revision 1.1.10.3  1993/07/19  18:42:37  blurie
 * 	Fix for OT #8287 (need a cma wrapper for ioctl(...I_RECVFD...)).
 * 
 * 		Add cma_ioctl() code for SVR4.
 * 
 * 		Have cma_getpmsg() call cma_ioctl() instead of ioctl().
 * 	[1993/07/19  18:16:57  blurie]
 * 
 * Revision 1.1.10.2  1993/06/24  21:18:29  hinman
 * 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
 * 	[1993/06/17  14:46:13  hinman]
 * 
 * Revision 9.5.1.7  92/12/17  13:58:05  raj
 * 	[raj] New naming + parallelized reentrant library changes.
 * 
 * Revision 9.5.3.2  92/11/06  11:17:32  raj
 * 	[raj] Changes for new naming scheme with reentrant libraries.
 * 
 * Revision 9.5.1.6  92/09/30  12:41:47  root
 * 	Acceptance of OSF rev 1.1.5.2
 * 
 * Revision 9.12.1.2  92/09/30  10:37:03  hinman
 * 	Auto checkin of OSF rev 1.1.5.2
 * 
 * Revision 1.1.2.2  1992/05/13  19:57:08  bolinger
 * 	Complete fix for OT defect 1711:  convert cma_open() to match the POSIX
 * 	prototype of open(2).
 * 	[1992/05/12  16:51:56  bolinger]
 * 
 * 	Fix OT defect 1711:  fix wrapper signatures to match POSIX/ANSI/XPG3
 * 	specs.
 * 	[1992/05/12  14:03:23  bolinger]
 * 
 * Revision 1.1  1992/01/19  14:40:47  devrcs
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
 *	Thread synchronous I/O (Module 2)
 *
 *  AUTHORS:
 *
 *	Paul Curtin
 *
 *  CREATION DATE:
 *
 *	1 May 1991
 *	    (Split from original module created by Hans Oser 1989)
 *
 *  MODIFICATION HISTORY:
 *	001	Paul Curtin	13 May 1991
 *		Changed a macro name from cma___ to cma__
 *	002	Webb Scales	30 May 1991
 *		Moved much of the functionality from the close wrapper to a
 *		new close-general function.
 *	003	Webb Scales	20 June 1991
 *		Reworked close to remove a race with open.
 *	004	Dave Butenhof	12 September 1991
 *		Integrate Nat Mishkin's performance improvements to make more
 *		intelligent use of the file descriptor numbers; ULTRIX V4.2
 *		allows 4096 files, but most systems are generated to support
 *		lower numbers, and DECthreads is currently carrying around
 *		all that extra baggage. With this change, it'll only check
 *		and use the ones actually supported on the system. Also,
 *		shrink the TCB by dynamically allocating the file descriptor
 *		mask at thread creation time.
 *	005	Dave Butenhof	19 September 1991
 *		Integrate HPUX CMA5 reverse drop: fix a problem with the
 *		error path of cma_close() by releasing the FD reservation.
 *	006	Dave Butenhof	04 October 1991
 *		Use new _CMA_THREAD_SYNC_IO_ to disable compilation, rather
 *		than _CMA_UNIPROCESSOR_.
 */


/*
 *  INCLUDE FILES
 */



#include <cma_config.h>

#if !_CMA_THREAD_SYNC_IO_		/* Don't compile unless needed! */

#include <sys/types.h>
#include <sys/uio.h>            /* Bring in struct iovec */
#include <stropts.h>            /* necessary for cma_getmsg etc. */

#include <cma_thdio_defs.h>
#include <sys/uio.h>
#include <stdarg.h>
#include <cma_ux.h>


/*
 * LOCAL DATA
 */


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma_close:  close the "file" corresponding to the descriptor.
 *
 *	Note that a file is still accessible until it is closed the same 
 *	number of times it was opened, that is, the U*ix system call 'close'
 *	is not issued until no threads currently consider the file to be open.
 *
 *  FORMAL PARAMETERS:
 *
 *	fd  -  a Unix file descriptor, a number.
 *
 *  IMPLICIT INPUTS:
 *
 *	The CMA I/O data base
 *
 *  IMPLICIT OUTPUTS:
 *
 *	The CMA I/O data base
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
cma_close(
	int	fd)
    {
    cma_t_integer   res;


    if ((fd < 0) || (fd >= cma__g_mx_file)) return (cma__set_errno (EBADF), -1);

    if (!cma__is_open(fd)) return (cma__set_errno (EBADF), -1);

    cma__fd_reserve (fd);			/* Prevent conflict w/read &c */

    /* 
     * Prevent race with close.....
     * We may have closed while waiting for the mutex.
     */
    if (!cma__is_open(fd)) return (cma__set_errno (EBADF), -1);

    cma__int_lock (cma__g_io_data_mutex);	/* Prevent race with open */

    while (cma_c_true) {
	res = close (fd);

	if (res != -1) {
	    /*
	     * The close was successful, now close the file from CMA's
	     * point of view.
	     *
	     * The close-general call requires that the file be reserved.
	     * It will unreserve it or deallocate it (which, in effect, 
	     * unreserves it).  It also requires that the I/O data mutex
	     * be locked before the call.
	     */
	    cma__close_general (fd);

	    /*
	     * The remaining code is for retry and error conditions; since we
	     * know the close worked, just release the mutex and go home.
	     */
	    cma__int_unlock (cma__g_io_data_mutex);
    	    return res;
	    }

	if (!cma__is_blocking (errno)) break;	/* Quit on most errors */

	if (cma__g_file[fd]->user_fl.user_non_blocking)
	    break;				/* Quit if user wants NB err */

	/*
	 * The close would have blocked the process, and the user doesn't
	 * expect to see non-blocking errors, so we will have to retry the
	 * close.  However, give other threads a chance to run so this 
	 * doesn't become a tight loop.  Also, things are in a consistent
	 * state at the moment, so release the I/O mutex so other threads
	 * can use it if need be.
	 */
	cma__int_unlock (cma__g_io_data_mutex);
	cma_yield ();
	cma__int_lock (cma__g_io_data_mutex)
	}

    /*
     * The close() didn't work; unlock the I/O database, unreserve the file
     * descriptor, and return the error.
     */
    cma__int_unlock (cma__g_io_data_mutex);
    cma__fd_unreserve (fd);
    return res;
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma_open:  open a file
 *
 *  FORMAL PARAMETERS:
 *
 *	path		path name to file
 *	flags		how to open the file
 *	...		additional arguments
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
 *	The file descriptor
 *
 *  SIDE EFFECTS:
 *
 *	We promptly extract a third argument, mode, from the
 *	variable arg list, since there's no way to pass the
 *	list uninterpreted to open(2).
 *
 *	mode		protection umask for file if created
 */

extern int
cma_open(
	const char	*path,
	int	flags,
	...)
    {
    va_list	ap;
	int mode;

    cma__int_init();
    va_start (ap, flags);

    mode = va_arg (ap, int);
    cma__ts_open (open, (path, flags, mode), cma__open_general (__fd__));
	va_end(ap);
    }


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma_read:  read from a file
 *
 *  FORMAL PARAMETERS:
 *
 *	fd		file descriptor of file to read
 *	buff		address of buffer to receive characters read
 *	nbytes		number of bytes to read
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
 *	The number of characters read, or -1 in case of error
 *
 *  SIDE EFFECTS:
 *
 *	The current thread may block.
 */

extern ssize_t
cma_read(
	int		fd,
	void		*buff,
	size_t	nbytes)
    {
    cma__ts_func (read, fd, (fd, (void *)buff, nbytes), cma__c_io_read, 0);
    }



/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma_readv:  read from a file
 *
 *  FORMAL PARAMETERS:
 *
 *	fd		file descriptor of file to read
 *	iov		I/O vector (ie, array of buffers)
 *	iovcnt		number of entries in the iov
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
 *	The number of characters read, or -1 in case of error
 *
 *  SIDE EFFECTS:
 *
 *	The current thread may block.
 */

extern int
cma_readv(
	int		fd,
	struct iovec	*iov,
	int		iovcnt)
    {
    cma__ts_func (readv, fd, (fd, iov, iovcnt), cma__c_io_read, 0);
    }


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma_write:  write to a file
 *
 *  FORMAL PARAMETERS:
 *
 *	fd		file descriptor of file to written to
 *	buff		address of buffer to be written
 *	nbytes		number of bytes to write
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
 *	The number of characters written, or -1 in case of error
 *
 *  SIDE EFFECTS:
 *
 *	The current thread may block.
 */

extern ssize_t
cma_write(
	int		fd,
	const void		*buff,
	size_t	nbytes)
    {
    cma__ts_func (write, fd, (fd, buff, nbytes), cma__c_io_write, 0);
    }



/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma_writev:  write to a file
 *
 *  FORMAL PARAMETERS:
 *
 *	fd		file descriptor of file to write
 *	iov		I/O vector (ie, array of buffers)
 *	iovcnt		number of entries in the iov
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
 *	The number of characters written, or -1 in case of error
 *
 *  SIDE EFFECTS:
 *
 *	The current thread may block.
 */

extern int
cma_writev(
	int		fd,
	struct iovec	*iov,
	int		iovcnt)
    {
    cma__ts_func (writev, fd, (fd, iov, iovcnt), cma__c_io_write, 0);
    }
#endif					/* if !_CMA_THREAD_SYNC_IO_ */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_THDIO_2.C */
/*  *9     5-NOV-1991 14:59:07 BUTENHOF "Remove undefined symbol for OSF/1" */
/*  *8    14-OCT-1991 13:41:07 BUTENHOF "Use new config symbols" */
/*  *7    24-SEP-1991 16:28:50 BUTENHOF "Handle variable # of FDs more efficiently" */
/*  *6    17-SEP-1991 13:24:15 BUTENHOF "Handle variable # of FDs more efficiently" */
/*  *5    21-JUN-1991 19:54:36 SCALES "Fix race between open & close" */
/*  *4    10-JUN-1991 18:24:18 SCALES "Add sccs headers for Ultrix" */
/*  *3    31-MAY-1991 18:42:20 SCALES "Modify close wrapper to call general close function" */
/*  *2    14-MAY-1991 13:46:15 BUTENHOF "Integrate changes lost in disk crash" */
/*  *1     2-MAY-1991 16:26:10 CURTIN "Thread synchronous I/O cont" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_THDIO_2.C */
