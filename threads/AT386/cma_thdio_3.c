/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 *
 */
/*
 * HISTORY
 * $Log: cma_thdio_3.c,v $
 * Revision 1.1.6.2  1996/02/17  23:32:38  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:49:00  marty]
 *
 * Revision 1.1.6.1  1995/12/07  21:44:06  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  20:58:34  root]
 * 
 * Revision 1.1.2.2  1994/06/09  13:33:42  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:24:56  devsrc]
 * 
 * Revision 1.1.2.1  1994/04/01  20:13:14  jd
 * 	Moved from src/threads to here.
 * 	[1994/04/01  16:23:20  jd]
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
 *	Thread synchronous I/O, Module 3:  Miscellaneous File I/O wrappers
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
cma_creat (
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
    int	    res;


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

    cma__int_init();
    va_start (ap, request);


    arg = va_arg(ap, int);
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
#ifdef O_NDELAY
		(void)(cma__g_file[fd]->user_fl.o_ndelay
		    ? (ret_val |= O_NDELAY)
		    : (ret_val &= ~O_NDELAY));
#endif
#ifdef _CMA_FNDELAY_
		(void)(cma__g_file[fd]->user_fl.fndelay
		    ? (ret_val |= FNDELAY)
		    : (ret_val &= ~FNDELAY));
#endif
#ifdef O_NONBLOCK
		(void)(cma__g_file[fd]->user_fl.o_nonblock
		    ? (ret_val |= O_NONBLOCK)
		    : (ret_val &= ~O_NONBLOCK));
#endif
#ifdef _CMA_FNBLOCK_
		(void)(cma__g_file[fd]->user_fl.fnblock
		    ? (ret_val |= FNBLOCK)
		    : (ret_val &= ~FNBLOCK));
#endif
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
# ifdef O_NDELAY
		tmp_arg |= O_NDELAY;
# endif
# ifdef _CMA_FNDELAY_
		tmp_arg |= FNDELAY;
# endif
# ifdef O_NONBLOCK
		tmp_arg |= O_NONBLOCK;
# endif
# ifdef _CMA_FNBLOCK_
		tmp_arg |= FNBLOCK;
# endif

		}

	    ret_val = fcntl (fd, request, tmp_arg);

	    if (ret_val != -1) {
		   /*
			* We masked in the bits to set the file nonblocking, so
			* we can set the set_non_blocking flag so we won't try to
			* do this again in the I/O wrapper.
			*/
			cma__g_file[fd]->set_non_blocking = cma_c_true;

			/*
			*	Set the user nonblocking flags.
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
	    ret_val = fcntl (fd, request, arg);

	    if (ret_val != -1) {
		if (ret_val >= cma__g_mx_file)  
		    cma__bugcheck ("cma_fcntl: dup'd fd too large");
		cma__int_lock (cma__g_io_data_mutex);
		cma__g_file[ret_val] = cma__g_file[fd];
		cma__g_file[fd]->ref_count++;
		if (ret_val >= cma__g_file_num)
		    cma__g_file_num = ret_val + 1;
		cma__int_unlock (cma__g_io_data_mutex);
		}

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
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_THDIO_3.C */
/*  *10   14-OCT-1991 13:41:13 BUTENHOF "Use new config symbols" */
/*  *9    24-SEP-1991 16:28:56 BUTENHOF "Handle variable # of FDs more efficiently" */
/*  *8    17-SEP-1991 13:24:22 BUTENHOF "Handle variable # of FDs more efficiently" */
/*  *7    21-JUN-1991 19:54:51 SCALES "change to close-general protocol" */
/*  *6    10-JUN-1991 18:24:21 SCALES "Add sccs headers for Ultrix" */
/*  *5    31-MAY-1991 18:42:29 SCALES "Create an un-import_fd function" */
/*  *4    14-MAY-1991 13:46:19 BUTENHOF "Integrate changes lost in disk crash" */
/*  *3     7-MAY-1991 10:12:30 CURTIN "Replaced call to cma_init w/cma__int_init" */
/*  *2     2-MAY-1991 22:28:29 SCALES "Rearrange includes" */
/*  *1     2-MAY-1991 16:26:37 CURTIN "Thread synchronous I/O cont" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_THDIO_3.C */
