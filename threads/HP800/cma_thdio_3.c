/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 *
 */
/*
 * HISTORY
 * $Log: cma_thdio_3.c,v $
 * Revision 1.1.6.1  1996/05/10  13:28:53  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:51 UTC  bissen
 * 	Merge from branch jss_single_thread.
 * 	[1994/12/22  20:02 UTC  jss  /main/HPDCE02/4]
 *
 * 	Changes for single threaded client.  Conditionalize wrapper usage based on whether
 * 	CMA is initialized.
 * 	[1994/12/22  19:24 UTC  jss  /main/HPDCE02/jss_single_thread/1]
 *
 * 	Merge from jss_fix_close to HPDCE02.
 * 	[1994/09/15  19:22 UTC  jss  /main/HPDCE02/3]
 *
 * 	Fix for CHFts12442.  Modified cma_fnctl().
 * 	[1994/09/13  19:48 UTC  jss  /main/HPDCE02/jss_fix_close/2]
 *
 * 	Merge from branch jss_close_fix.
 * 	[1994/09/08  19:06 UTC  jss  /main/HPDCE02/2]
 *
 * 	Fix for CHFts12442. Close while I/O pending bug.
 * 	[1994/08/30  14:24 UTC  jss  /main/HPDCE02/jss_fix_close/1]
 *
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:48 UTC  jss  /main/HPDCE02/1]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/25  14:18  truitt  /main/HPDCE01/1]
 *
 * 	re-merge kk and dce1_0_3
 * 	(watch the blocking/non-blocking stuff)
 * 	[1993/09/23  19:25:03  truitt]
 * 	 *
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:44:29  root  1.1.12.3]
 *
 * 	replace merged version with straight kk version for now
 * 	[1993/09/22  16:27:26  truitt]
 *
 * 	Fix for OT #8155 (cma__set_nonblocking() may use fcntl() in an
 * 	inconsistent way).
 *
 * 	Call the new cma__set_user_nonblock_flags() which does the correct
 * 	thing.
 * 	[1993/07/19  18:18:40  blurie]
 *
 * 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
 * 	[1993/06/17  14:46:26  hinman]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  18:57:59  root]
 *
 * 	Gamera Merge II
 * 	[1993/04/25  01:24:08  hopkins]
 *
 * 	Loading drop DCE1_0_2B23
 * 	[1993/04/21  20:46:03  root  1.1.1.10]
 *
 * 	CR10892 - fix copyright in file
 * 	[1994/06/09  13:25:38  devsrc]
 * 	 *
 *
 * 	Moved from src/threads to here
 * 	[1994/04/01  17:03:21  jd]
 *
 * 	Fix for OT #8155 (cma__set_nonblocking() may use fcntl() in an
 * 	inconsistent way).
 *
 * 	Call the new cma__set_user_nonblock_flags() which does the correct
 * 	thing.
 * 	[1993/07/19  18:18:40  blurie]
 *
 * 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
 * 	[1993/06/17  14:46:26  hinman]
 *
 * 	Check to see if the file got closed while we were waiting for the mutex
 * 	in cma_fd_reserve.
 * 	[1993/03/10  19:38:25  jd]
 *
 * 	Gamera merge
 * 	[1993/03/26  23:56:26  hopkins]
 *
 * 	Loading drop DCE1_0_2B18
 * 	[1993/03/11  22:28:48  osfrcs  1.1.1.8]
 *
 * 	Fixed fcntl wrapper to not set O_NONBLOCK
 * 	unless set_non_blocking is set, so that nonblocking
 * 	is only set on actual io.  Fixes a bug where
 * 	Oracle forks, then calls fcntl before exec'ing.
 * 	[1992/11/10  23:16:59  hopkins]
 *
 * Revision 1.1.2.2  1994/06/09  13:34:46  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:25:38  devsrc]
 * 
 * Revision 1.1.2.1  1994/04/01  20:13:50  jd
 * 	Moved from src/threads to here
 * 	[1994/04/01  17:03:21  jd]
 * 
 * Revision 1.1.11.3  1993/07/19  20:13:58  blurie
 * 	Fix for OT #8155 (cma__set_nonblocking() may use fcntl() in an
 * 	inconsistent way).
 * 
 * 		Call the new cma__set_user_nonblock_flags() which does the correct
 * 		thing.
 * 	[1993/07/19  18:18:40  blurie]
 * 
 * Revision 1.1.11.2  1993/06/24  21:18:51  hinman
 * 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
 * 	[1993/06/17  14:46:26  hinman]
 * 
 * Revision 1.1.4.5  1993/03/10  22:43:25  jd
 * 	Check to see if the file got closed while we were waiting for the mutex
 * 	in cma_fd_reserve.
 * 	[1993/03/10  19:38:25  jd]
 * 
 * Revision 1.1.4.4  1993/02/01  22:31:11  hinman
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  17:32:46  hinman]
 * 
 * 	[hinman@sni] - Correct SVR4 ifdefs
 * 	[1993/01/21  14:46:38  hinman]
 * 
 * 	[hinman] - Check in merged SNI version
 * 	[1993/01/11  16:22:19  hinman]
 * 
 * Revision 9.5.1.3  92/12/17  14:00:36  raj
 * 	[raj] New naming + parallelized reentrant library changes.
 * 
 * Revision 9.5.4.3  92/11/16  15:33:25  raj
 * 	[raj] More rlib name changes
 * 
 * Revision 9.5.4.2  92/11/06  11:20:09  raj
 * 	[raj] Changes for new naming scheme with reentrant libraries.
 * 
 * Revision 9.5.1.2  92/09/10  16:36:06  hinman
 * 	[hinman] - Fold forward from raj_om
 * 
 * Revision 9.5.2.2  92/09/10  16:10:35  raj
 * 	[raj] merging SVR4 changes into 1.0.2
 * 
 * Revision 9.9  92/09/02  14:09:06  hinman
 * 	Update checkin
 * 
 * Revision 1.1.2.3  1992/08/14  20:23:01  bolinger
 * 	Bring forward 1.0.1 revision 1.1.2.2 (fix for OT defect 1711),
 * 	1.1.2.3 (fix for 2540).
 * 	[1992/08/10  16:28:33  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/09  00:03:27  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/09  00:03:13  bolinger]
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
 *	Thread synchronous I/O, Module 3:  Miscellaneous File I/O wrappers,
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
 *	001	Webb Scales	 2 May 1991
 *		- Moved includes from cma_thdio_defs.h to here.
 *		- Added arguments to the bugcheck calls.
 *	002	Paul Curtin	 3 May 1991
 *		Replaced call to cma_init() with cma__int_init()
 *	003	Paul Curtin	13 May 1991
 *		Changed a macro name from cma___ to cma__
 *	004	Webb Scales	30 May 1991
 *		Created an un-import_fd function, the complement of the 
 *		import_fd function, here; tidied dup2 and fixed the error
 *		return path.
 *	005	Webb Scales	21 June 1991
 *		Changed protocol for calling close-general: it now requires
 *		the caller to lock the I/O data mutex.
 *	006	Dave Butenhof	12 September 1991
 *		Integrate Nat Mishkin's performance improvements to make more
 *		intelligent use of the file descriptor numbers; ULTRIX V4.2
 *		allows 4096 files, but most systems are generated to support
 *		lower numbers, and DECthreads is currently carrying around
 *		all that extra baggage. With this change, it'll only check
 *		and use the ones actually supported on the system. Also,
 *		shrink the TCB by dynamically allocating the file descriptor
 *		mask at thread creation time.
 *	007	Dave Butenhof	19 September 1991
 *		Integrate HPUX CMA5 reverse drop: maintain maximum file
 *		number across opens, and conditionalize some file control
 *		bits.
 *	008	Dave Butenhof	04 October 1991
 *		Use new _CMA_THREAD_SYNC_IO_ to disable compilation, rather
 *		than _CMA_UNIPROCESSOR_.
 */



/*
 *  INCLUDE FILES
 */


#include <cma.h>

#if !_CMA_THREAD_SYNC_IO_		/* Don't compile unless needed! */

#include <cma_defs.h>
#include <cma_init.h>
#include <cma_thdio_defs.h>
#include <sys/file.h>
#include <cma_errors.h>
# include <stdarg.h>
#include <cma_ux.h>

/*
 * LOCAL DATA
 */



/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma_creat:  create a file
 *
 *  FORMAL PARAMETERS:
 *
 *	path		path name to file
 *	mode		protection umask for file created
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
 *	None
 */

extern int
cma_creat(
	const char	*path,
	mode_t	mode)
    {
    cma__ts_open (creat, (path, mode), cma__open_general (__fd__));
    }


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma_dup:  duplicate an existing file descriptor
 *
 *  FORMAL PARAMETERS:
 *
 *	oldd		file descriptor of an existing (open) file
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
 *	A new file descriptor referencing the same file
 *
 *  SIDE EFFECTS:
 *
 *	None
 */

extern int
cma_dup(
	int	oldd)
    {
#ifdef _HP_CMA_ASYNC_CLOSE_
    int	newd;

    if (!cma__is_multithreaded()) return (dup (oldd)); 
    /*
     * We can't use cma__ts_open because we need to lock
     * the fd before calling dup, but we can't unlock it
     * again except in the post_process code which is only
     * run if the dup succeeds.
     */

    if(!cma__hp_fd_reserve(oldd))
	return( cma__set_errno(EBADF), -1 );

    cma__int_lock (cma__g_io_data_mutex);
    newd = dup (oldd);

    if (newd >= cma__g_mx_file)
	cma__bugcheck ("cma_dup:  dup'd fd is too large");

    if (newd >= 0) {
	/*
	 * May want to add a check to make sure
	 * cma__g_file[newd] is null before we
	 * assign to it.
	 */
	cma__g_file[newd] = cma__g_file[oldd];
	cma__g_file[oldd]->ref_count++;
	if (newd >= cma__g_file_num) cma__g_file_num = newd + 1;
	}

    cma__int_unlock (cma__g_io_data_mutex);
    cma__fd_unreserve(oldd);
    return newd;
#else

    if (!cma__is_multithreaded()) return (dup (oldd)); 
    if ((oldd < 0) || (oldd >= cma__g_mx_file)) return (cma__set_errno (EBADF), -1);
    if (!cma__is_open(oldd)) return (cma__set_errno (EBADF), -1);

    cma__ts_open (dup, 
	    (oldd), 
	    {cma__int_lock (cma__g_file[oldd]->mutex);
		cma__int_lock (cma__g_io_data_mutex);
		cma__g_file[__fd__] = cma__g_file[oldd];
		cma__g_file[oldd]->ref_count++;
		if (__fd__ >= cma__g_file_num) cma__g_file_num = __fd__ + 1;
		cma__int_unlock (cma__g_io_data_mutex);
		cma__int_unlock (cma__g_file[oldd]->mutex);});
#endif
    }


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma_dup2:  duplicate an existing file descriptor using a specified fd
 *
 *  FORMAL PARAMETERS:
 *
 *	oldd		file descriptor of an existing (open) file
 *	newd		file descriptor to hold copy
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
 *	A new file descriptor referencing the same file
 *
 *  SIDE EFFECTS:
 *
 *	If newd refers to an open file, that file is closed first.
 */

extern int
cma_dup2(
	int	oldd,
	int	newd)
    {
#ifdef _HP_CMA_ASYNC_CLOSE_

    int             res;
    cma_t_boolean   was_open = cma_c_false;

    if ((!cma__is_multithreaded()) || (oldd == newd))
	return (dup2 (oldd, newd)); 

    if( oldd == newd )
	return (cma__set_errno (EBADF), -1);

    if( newd < 0 || newd >= cma__g_mx_file )
	return (cma__set_errno (EBADF), -1);

    if(!cma__hp_fd_reserve(oldd))
	return (cma__set_errno (EBADF), -1);

    cma__int_lock(cma__g_io_data_mutex);

    while( !was_open && cma__is_open(newd) ) {
	cma__int_unlock(cma__g_io_data_mutex);
	if(cma__hp_fd_reserve(newd)) {
	    was_open = cma_c_true;
	    }
	cma__int_lock(cma__g_io_data_mutex);
	}

    /*
     * We've got oldd locked, and newd is locked if it was
     * open when we started.  Now get the io_data mutex
     * and enter the kernel, so that cma__io_available can't
     * run while we're actually doing the dup:  we don't want
     * select to catch any EBADF errors.  Also, if newd was open,
     * we want to turn of the wait_fds bits for it, since it's
     * been closed.
     */

    cma__enter_kernel();
    /* we're counting on newd still being open if dup2 fails */
    if( (res = dup2 (oldd, newd)) != -1 && was_open ) {
	if(cma__fdm_is_set(newd, &cma__g_wait_fds[(int)cma__c_io_read])) {
	    cma__fdm_clr_bit(newd, &cma__g_wait_fds[(int)cma__c_io_read]);
	    }
	if(cma__fdm_is_set(newd, &cma__g_wait_fds[(int)cma__c_io_write])) {
	    cma__fdm_clr_bit(newd, &cma__g_wait_fds[(int)cma__c_io_write]);
	    }
	if(cma__fdm_is_set(newd, &cma__g_wait_fds[(int)cma__c_io_except])) {
	    cma__fdm_clr_bit(newd, &cma__g_wait_fds[(int)cma__c_io_except]);
	    }
	}
    cma__exit_kernel();

    if( res != -1 ) {
	if( was_open ) cma__close_general(newd);
	cma__g_file[newd] = cma__g_file[oldd];
	cma__g_file[newd]->ref_count++;
	if( newd >= cma__g_file_num ) cma__g_file_num = newd + 1;
	}
    else {
	/*
	 * We assume that if newd was open to
	 * start with, it's still open.
	 */
	if( was_open ) cma__fd_unreserve(newd);
	}

    cma__int_unlock(cma__g_io_data_mutex);
    cma__fd_unreserve(oldd);
    return( res );

#else

    int	    res;


    if (!cma__is_multithreaded()) return (dup2 (oldd, newd)); 

    if ((oldd < 0) || (oldd >= cma__g_mx_file)) 
	return (cma__set_errno (EBADF), -1);
    if ((newd < 0) || (newd >= cma__g_mx_file)) 
	return (cma__set_errno (EBADF), -1);
    if (!cma__is_open(oldd)) 
	return (cma__set_errno (EBADF), -1);

    cma__fd_reserve (oldd);
    if (cma__is_open (newd))
        cma__fd_reserve (newd);

    /*
     * FIX-ME: If newd was open when the dup2() call was made, and the dup2() 
     *	    call fails, is newd still open when the call returns?
     */
    res = dup2 (oldd, newd);
    if (res < 0)  {
	cma__fd_unreserve (oldd);
	if (cma__is_open (newd))
	    cma__fd_unreserve (newd);
	return res;
	}

    if (cma__is_open (newd))  {
	/*
	 * The close-general call requires that the file be reserved.  It will 
	 * unreserve it or deallocate it (which, in effect, unreserves it).  
	 * It also requires that the I/O data mutex be locked before the call.
	 */
	cma__int_lock (cma__g_io_data_mutex);
	cma__close_general (newd);
	cma__int_unlock (cma__g_io_data_mutex);
	}

    cma__int_lock (cma__g_io_data_mutex);
    cma__g_file[newd] = cma__g_file[oldd];
    cma__g_file[oldd]->ref_count++;
    if (newd >= cma__g_file_num) cma__g_file_num = newd + 1;
    cma__int_unlock (cma__g_io_data_mutex);

    cma__fd_unreserve (oldd);

    return res;
#endif
    }


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma_fcntl
 *
 *	Wrapper for fcntl() which allows us to hide the fact from the client
 *	that files are set to "non-blocking" mode whenever possible.
 *
 *	This routine acts as a filter between the client and the real fcntl(),
 *	modifying the parameter to the "set" request and the results of the 
 *	"get" request for "non-blocking" mode.
 *
 *	The relevant requests are F_GETFL and F_SETFL, get and set file flags,
 *	and the relevant flag masks are O_NDELAY and its obsolete synonym
 *	FNDELAY (BSD?), and O_NONBLOCK and its obsolete synonym FNBLOCK (POSIX).
 *
 *  FORMAL PARAMETERS:
 *
 *	fd	-  a Unix file descriptor, a number.
 *	request	-  specifies what operation to perform
 *	...	-  additional arguments
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
 *	We promptly extract a third argument, arg, from the
 *	variable arg list, since there's no way to pass the
 *	list uninterpreted to fcntl(2).
 *
 *	arg	-  argument to the request
 */
extern int
cma_fcntl (
	int	fd,
	int	request,
	...)
    {
    va_list	ap;
    int	ret_val, arg;

    va_start (ap, request);
    arg = va_arg(ap, int);

    if (!cma__is_multithreaded()) return (fcntl (fd, request, arg));

    cma__int_init();
#ifdef _HP_CMA_ASYNC_CLOSE_
    if(!cma__hp_fd_reserve(fd)) {
	va_end(ap);
	return( cma__set_errno(EBADF), -1 );
    }
#else
    if ((fd < 0) || (fd >= cma__g_mx_file)) {
		va_end(ap);
		return (cma__set_errno (EBADF), -1);
	}
    if (!cma__is_open(fd)) {
		va_end(ap);
		return (cma__set_errno (EBADF), -1);
	}
    cma__fd_reserve (fd);
    /* 
     * Check possible to see if the file got closed during possible
     * block in cma__fd_reserve.
     */
    if (!cma__is_open(fd)) {
                va_end(ap);
                return (cma__set_errno (EBADF), -1);
        }
#endif

    switch (request) {
    	case F_GETFL: {
	    /*
	     * The client has requested that the system fetch the file flags.
	     * Therefore, do the "get" on behalf of the client, and then fix up 
	     * the bits that we are "using transparently" in the result before 
	     * returning it.  
	     */
	    ret_val = fcntl (fd, request, arg);

	    if (ret_val != -1) {
		if (cma__g_file[fd]->non_blocking &&
		    !cma__g_file[fd]->user_fl.o_nonblock)
			ret_val &= ~O_NONBLOCK;
		}

	    break;
	    }

	case F_SETFL: {
	    /*
	     * The client has requested that the system set the file flags.
	     * Therefore, do the "set" on behalf of the client, first fixing up 
	     * the bits that we are "using transparently" in the mask before 
	     * making the call.  If the "set" is successful, set the bits in
	     * our file data structure to reflect the "new state" of those bits.
	     */
	    int tmp_arg;

	    tmp_arg = arg;

	    if (cma__g_file[fd]->non_blocking) {
		if (!(arg & O_NDELAY) && !cma__g_file[fd]->user_fl.fiosnbio) {
		    if (cma__g_file[fd]->set_non_blocking)
		    	tmp_arg |= O_NONBLOCK;
		}
	    }

	    ret_val = fcntl (fd, request, tmp_arg);

	    if (ret_val != -1) {
		   /*
		    * Set the user nonblocking flags.
		    */
		   cma__set_user_nonblock_flags( fd, arg );
		}

	    break;
	    }

	case F_DUPFD: {
	    /*
	     * The client has requested that the system create a duplicate
	     * file descriptor for a file.  Therefore, make the system call
	     * on behalf of the client, and then make the appropriate changes
	     * in the CMA I/O database.  
	     */
#ifdef _HP_CMA_ASYNC_CLOSE_
	    cma__int_lock (cma__g_io_data_mutex);
#endif
	    ret_val = fcntl (fd, request, arg);

	    if (ret_val != -1) {
		if (ret_val >= cma__g_mx_file)  
		    cma__bugcheck ("cma_fcntl: dup'd fd too large");
#ifndef _HP_CMA_ASYNC_CLOSE_
		cma__int_lock (cma__g_io_data_mutex);
#endif
		cma__g_file[ret_val] = cma__g_file[fd];
		cma__g_file[fd]->ref_count++;
		if (ret_val >= cma__g_file_num)
		    cma__g_file_num = ret_val + 1;
#ifndef _HP_CMA_ASYNC_CLOSE_
		cma__int_unlock (cma__g_io_data_mutex);
#endif
		}
#ifdef _HP_CMA_ASYNC_CLOSE_
	    cma__int_unlock (cma__g_io_data_mutex);
#endif

	    break;
	    }

	default: {
	    /*
	     * The caller has requested some function that we are not filtering.
	     * Therefore, do the call on behalf of the client, and return the
	     * results.  
	     */
	    ret_val = fcntl (fd, request, arg);

	    break;
	    }
	}

    cma__fd_unreserve (fd);
	va_end(ap);
    return ret_val;
    }


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma_import_fd
 *
 *	This function allows programs to inform CMA that the indicated file
 *	has already been opened (ie, not via a CMA-wrapped routine; such as,
 *	descriptors inherited via fork()/exec(), etc.)
 *
 *  FORMAL PARAMETERS:
 *
 *	fd  -  a Unix file descriptor, a number.
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
 *	Initializes CMA.
 *	See cma__open_general.
 */
extern void
cma_import_fd(int	fd)
    {
    cma__int_init();
    cma__int_init2();
    cma__open_general (fd);
    }


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma_pipe:  create an interprocess I/O channel
 *
 *  FORMAL PARAMETERS:
 *
 *	filedes		    array of two fd's 
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
cma_pipe(
	int	fildes[2])
    {
    cma__ts_open2 (pipe, 
	    fildes, 
	    (fildes), 
	    (cma__open_general (fildes[0]), cma__open_general (fildes[1])));
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma_unimport_fd
 *
 *	This function allows programs to inform CMA that the indicated file
 *	is now closed (e.g. via fclose(3)).
 *
 *  FORMAL PARAMETERS:
 *
 *	fd  -  a Unix file descriptor, a number.
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
 *	Initializes CMA.
 *	See cma__close_general
 */
extern void
cma_unimport_fd(int	fd)
    {
    /*
     * The close-general call requires that the file be reserved.  It will 
     * unreserve it or deallocate it (which, in effect, unreserves it).  
     * It also requires that the I/O data mutex be locked before the call.
     */

    if ((fd < 0) || (fd >= cma__g_mx_file))
	cma__error (cma_s_badparam);

    if (!cma__is_open(fd))
	cma__error (cma_s_badparam);

    cma__fd_reserve (fd);
    /* 
     * Check to see if the file got closed during possible
     * block in cma__fd_reserve.
     */
    if (!cma__is_open(fd))
        cma__error (cma_s_badparam);
    cma__int_lock (cma__g_io_data_mutex);
    cma__close_general (fd);
    cma__int_unlock (cma__g_io_data_mutex);
    }
#endif					/* if !_CMA_THREAD_SYNC_IO_ */
