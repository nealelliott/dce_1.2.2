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
 * $Log: cma_thread_io.h,v $
 * Revision 1.1.6.2  1996/02/18  23:09:15  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:33:27  marty]
 *
 * 	Add prototype for cma__set_user_nonblock_flags().  Fix up cma__issue_io_call()
 * 	macros -- just make the source cleaner.
 * 	[1993/07/19  18:31:42  blurie]
 *
 * 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
 * 	[1993/06/17  14:47:45  hinman]
 *
 * Revision 1.1.6.1  1995/12/07  21:57:27  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:03:48  root]
 * 
 * 	Fixed a race condition after the call to cma__fd_reserve. We need to be
 * 	sure that the mutex call in cma__fd_reserve didn't sleep long enough
 * 	to have the file closed on it.
 * 	[1993/04/16  19:53:58  jd]
 * 
 * 	Removed the cma__is_open macro. Made it a function in cma_thread_io.c
 * 	so things outside of the thread_io modules can check to see if cma
 * 	already knows about a file without including cma_thread_io.h and
 * 	cma_thrdio_defs.h. cma_fdopen in cma_stdio.c needs to do this.
 * 	[1993/03/25  18:42:28  jd]
 * 
 * 	In cma_fd_reserve don't call cma__int_lock with cma__g_file[fd]->mutex.
 * 	cma__g_file might get nulled during the possible block resulting in a seg
 * 	on the setting of mutex ownership in cma__int_lock. Use the pointer to
 * 	the mutex itself instead.
 * 	[1993/03/10  19:44:21  jd]
 * 
 * 	Insert copyright notices
 * 	[1992/12/07  19:43:12  alan]
 * 
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:55:32  sekhar]
 * 
 * Revision 1.1.2.2  1994/06/10  20:53:17  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  16:49:56  devsrc]
 * 
 * Revision 1.1.2.1  1994/04/14  21:31:39  ohara
 * 	moved again
 * 	[1994/04/14  21:03:37  ohara]
 * 
 * Revision 1.1.2.1  1994/04/01  20:15:07  jd
 * 	Moved from src/threads to here
 * 	[1994/04/01  18:31:54  jd]
 * 
 * Revision 1.1.2.2  1992/05/21  20:34:04  keane
 * 	Address defect 2540.  Modify the cma__ts_func macro to check if
 * 	a file has been set to non-blocking mode.  If it hasn't then
 * 	it will do so prior to performing the I/O operation.
 * 	[1992/05/15  21:26:46  keane]
 * 
 * Revision 1.1  1992/01/19  14:40:51  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  Copyright (c) 1989, 1991 by
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
 *	Header file for thread synchrounous I/O
 *
 *  AUTHORS:
 *
 *	Hans Oser
 *
 *  CREATION DATE:
 *
 *	19 September 1989
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Hans Oser	25 October 1989
 *		To complete CMA baselevel routines
 *	002	Hans Oser	25 October 1989
 *		program review changes
 *	003	Dave Butenhof	4 December 1989
 *		"cma_t_condition" is now "cma_t_cond" to conform to POSIX.
 *	004	Dave Butenhof	4 December 1989
 *		Move tcb substructure into cma_tcb.h
 *	005	Webb Scales	5 December 1989
 *		Completely overhauled non-blocking I/O
 *	006	Webb Scales	10 December 1989
 *		Made the select queue module-local.
 *	007	Webb Scales	10 January 1990
 *		Added cma__c_io_except.
 *	008	Webb Scales & Dave Butenhof	29 March 1990
 *		Make select file descriptor masks available to the null thread.
 *	009	Webb Scales	 4 December 1990
 *		Add timeout parameter to io_available.
 *		Make select file descriptor masks private again.
 *	010	Paul Curtin	 2 May 1991
 *		Placed some contents in new cma_thdio_defs.h 
 *		(a necessary seperation cuz' circular dependencies)
 *	011	Webb Scales	 2 May 1991
 *		Converted over to use system definitions of fd masks.
 *		Rearranged the include files between here and cma_thdio_defs.h
 *		Added arguments to bugcheck calls
 *	012	Paul Curtin	 3 May 1991
 *		Replaced cma_init() call with cma__int_init.
 *	013	Webb Scales	 8 May 1991
 *		Correct the number of underscores on I/O macros 
 *		Added routine prototypes (although they currently have too many
 *		    underscores...)
 *	014	Paul Curtin	 9 May 1991
 *		Adjust macro argument lists.
 *		Removed extra underscore from routine prototypes.
 *	015	Paul Curtin	24 May 1991
 *		Added cma__reinit_thread_io prototype.
 *	016	Webb Scales	30 May 1991
 *		Added prototype for close-general; added miscellaneous "externs"
 *	017	Paul Curtin & Webb Scales   18 June 1991
 *		Added an include for IBMR2_UNIX, etc.
 *	018	Dave Butenhof	12 September 1991
 *		Integrate Nat Mishkin's performance improvements to make more
 *		intelligent use of the file descriptor numbers; ULTRIX V4.2
 *		allows 4096 files, but most systems are generated to support
 *		lower numbers, and DECthreads is currently carrying around
 *		all that extra baggage. With this change, it'll only check
 *		and use the ones actually supported on the system. Also,
 *		shrink the TCB by dynamically allocating the file descriptor
 *		mask at thread creation time.
 *	019	Paul Curtin	14 October 1991
 *		Added try/catch's to cma__ts_open/2.
 *	020	Dave Butenhof	06 November 1991
 *		Modify cma__ts_func macro to catch exc_SIGPIPE_e and recover
 *		correctly so that applications can rely on the exception to
 *		detect writes to broken pipes.
 *	021	Dave Butenhof, Webb Scales, Paul Curtin	11 November 1991
 *		Remove "return" from ts_open macro to avoid skipping the
 *		exc_pop_ctx call in ENDTRY.
 */


#ifndef CMA_THREAD_IO
#define CMA_THREAD_IO

/*
 *  INCLUDE FILES
 */

#include <cma_config.h>
#include <sys/file.h>
#include <cma.h>
#include <sys/types.h>
#include <sys/time.h>
#include <cma_init.h>
#include <cma_errors.h>
#include <poll.h>

/*
 * CONSTANTS
 */

/*
 * Define symbols which indicate whether to compile code for obsolete 
 * "non-blocking mode" flags:  FNDELAY and FNBLOCK.  If the obsolete
 * symbols are defined, and if their replacement symbols are defined 
 * and are different or if they are undefined, then define a symbol
 * that says to compile the code in; otherwise no code will be compiled
 * for these obsolete symbols.
 */
#ifdef FNDELAY
# ifdef O_NDELAY
#  if O_NDELAY != FNDELAY
#   define _CMA_FNDELAY_
#  endif
# else
#  define _CMA_FNDELAY_
# endif
#endif

#ifdef FNBLOCK
# ifdef O_NONBLOCK
#  if O_NONBLOCK != FNBLOCK
#   define _CMA_FNBLOCK_
#  endif
# else
#  define _CMA_FNBLOCK_
# endif
#endif


typedef enum CMA__T_IO_TYPE {
	cma__c_io_read			=	0,
	cma__c_io_write			=	1,
	cma__c_io_getmsg		=	2,
	cma__c_io_getmsg_pri	=	3,
	cma__c_io_getmsg_band	=	4,
	cma__c_io_putmsg		=	5,
	cma__c_io_putpmsg		=	6,
	cma__c_io_pollin		=	7
	}	cma__t_io_type;
#define cma__c_max_io_type 7

typedef struct pollfd cma__t_poll_element;

typedef struct CMA__T_POLL_INFO
	{
	cma_t_integer	nfds;
	cma__t_poll_element	*poll_e_p;
	}	cma__t_poll_info;

/*
 *  GLOBAL DATA
 */

/*
 * Maximum number of files (ie, max_fd+1) as determined by getdtablesize().
 */
extern int	cma__g_mx_file;


/*
 * MACROS
 */

/*
 * Define a constant for the errno value which indicates that the requested
 * operation was not performed because it would block the process.
 */
# define cma__is_blocking(s)										\
    ((s == EAGAIN) || (s == EWOULDBLOCK) || (s == EINPROGRESS) ||	\
     (s == EALREADY) || (s == EDEADLK))								\


/*
*	It is necessary to issue an I/O function, before calling cma__io_wait()
*	in the following cases:
*
*		*	This file descriptor has been set non-blocking by CMA
*		*	This file descriptor has been set non-blocking by the user.
*       *   This file descriptor is not pollable.
*/

#define cma__issue_io_call(fd)							\
	( (cma__g_file[fd]->non_blocking) ||				\
	  (cma__g_file[fd]->user_fl.user_non_blocking) ||	\
	  (cma__g_file[fd]->pollable == cma_c_false) )

#define cma__set_user_nonblocking(flags) \

/*
 * Determine if the file is open
 */
/*
 * If the file gets closed while waiting for the mutex cma__g_file[rfd]
 * gets set to null. This results in a crash if NDEBUG is set to 0 
 * since cma__int_lock tries to dereference it to set the mutex ownership 
 * after it gets the mutex. The following will still set the ownership
 * in cma__int_lock so we'll set it back to noone if cma__g_file is null
 * when we come back just in case it matters. It shouldn't since its no
 * longer in use but..... 
 * Callers of this should recheck cma__g_file after the reservation to
 * make sure continueing makes sense.
 */
#define cma__fd_reserve(rfd)										\
		{															\
		cma__t_int_mutex *__mutex__;								\
		__mutex__ = cma__g_file[rfd]->mutex;						\
		cma__int_lock (__mutex__);									\
		if(cma__g_file[rfd] == (cma__t_file_obj *)cma_c_null_ptr)	\
			cma__int_unlock(__mutex__);								\
		}
		

/*
 * Unreserve a file descriptor
 */
#define cma__fd_unreserve(ufd)	cma__int_unlock (cma__g_file[ufd]->mutex)

/*
 * System calls are interruptible
 * by the virtual-time signal. However, not all system calls are automatically
 * restarted by the OS (see sigaction(2) and SA_RESTART). Therefore, CMA 
 * must restart them for the user (regardless how expensive the call is) 
 * unless another signal is generated while the virtual-time signal is 
 * being handled. 
 */


#define cma__int_syscall(res,func,arglist)				\
    {													\
    cma__t_int_tcb  *__tcb__ = cma__get_self_tcb();		\
    cma_t_natural   *__p__   = &__tcb__->syscall_state;	\
    do {												\
       *__p__ = 0;										\
       res = func arglist;								\
       }												\
    while( ((res) == -1) && 							\
		  (errno == EINTR) && 							\
		  (*__p__ == cma__c_syscall_intintrpt) );		\
    }


/*
 * CMA "thread-synchronous" I/O read/write operations
 */

    /*
     * Since all CMA "thread-synchronous" I/O (read or write) operations on 
     * U*ix follow the exact same structure, the wrapper routines have been
     * condensed into a macro.
     *
     * The steps performed are as follows:
     *	1. Check that the file descriptor is a legitimate value.
     *	2. Check that the entry in the CMA file "database" which corresponds to 
     *	    the file descriptor indicates that the "file" was "opened" by CMA.
     *  3. Reserve the file, to serialized access to files.  This not only 
     *	    simplifies things, but also defends against non-reentrancy.
     *  4. If the "file" is "set" for non-blocking I/O, check if we
     *      have actually set the file non-blocking yet, and if not do so.
     *	    Then, issue the I/O operantion.
     *	    Success or failure is returned immediately, after unreserving the 
     *	    file.  If the error indicates that the operation would have caused
     *	    the process to block, continue to the next step.
     *	5. The I/O prolog adds this "file" to the global bit mask, which 
     *	    represents all "files" which have threads waiting to perform I/O on 
     *	    them, and causes the thread to block on the condition variable for
     *	    this "file".  Periodically, a select is done on this global bit 
     *	    mask, and the condition variables corresponding to "files" which 
     *	    are ready for I/O are signaled, releasing those waiting threads to
     *	    perform their I/O.
     *  6. When the thread returns from the I/O prolog, it can (hopefully) 
     *	    perform its operation without blocking the process.
     *	7. The I/O epilog clears the bit in the global mask and/or signals the 
     *	    the next thread waiting for this "file", as appropriate.
     *  8. If the I/O failed, continue to loop.
     *	9. Finally, the "file" is unreserved, as we're done with it, and the
     *	    result of the operation is returned.
     *
     *
     * Note:  currently, we believe that timeslicing which is based on the
     *	    virtual-time timer does not cause system calls to return EINTR.  
     *	    Threfore, any EINTR returns are relayed directly to the caller.
     *	    On platforms which do not support a virtual-time timer, the code
     *	    should probably catch EINTR returns and restart the system call.
     */

/*
 * This macro is used for both read-type and write-type functions.
 *
 * Note:  the second call to "func" may require being bracketed in a
 *	  cma__interrupt_disable/cma__interrupt_enable pair, but we'll 
 *	  wait and see if this is necessary.
 */
#define cma__ts_func(func,fd,arglist,type,post_process)	{		\
    cma_t_integer   __res__;									\
    cma_t_boolean   __done__ = cma_c_false;						\
    if ((fd < 0) || (fd >= cma__g_mx_file)) 					\
		return (cma__set_errno (EBADF), -1);					\
    if (!cma__is_open(fd)) return (cma__set_errno (EBADF), -1);	\
    cma__fd_reserve (fd);										\
    if (!cma__is_open(fd)) return (cma__set_errno (EBADF), -1);	\
    if (cma__issue_io_call(fd)) {								\
	if ((!cma__g_file[fd]->set_non_blocking) &&					\
		(cma__g_file[fd]->non_blocking == cma_c_true))			\
	    cma__set_nonblocking(fd);								\
        cma__interrupt_disable (0);								\
	TRY {														\
	    __res__ = func arglist;									\
	    }														\
	CATCH_ALL {													\
	    cma__interrupt_enable (0);								\
	    cma__fd_unreserve (fd);									\
	    RERAISE;												\
	    }														\
	ENDTRY														\
        cma__interrupt_enable (0);								\
	if ((__res__ != -1)											\
		|| (!cma__is_blocking (errno))							\
		|| (cma__g_file[fd]->user_fl.user_non_blocking))		\
	    __done__ = cma_c_true;									\
	}															\
    if (__done__) {												\
	cma__fd_unreserve (fd);										\
	}															\
    else {														\
	TRY {														\
	    cma__io_prolog (type, fd);								\
	    while (!__done__) {										\
		cma__io_wait (type, fd);								\
		__res__ = func arglist;									\
		if ((__res__ != -1)										\
			|| (!cma__is_blocking (errno))						\
			|| (cma__g_file[fd]->user_fl.user_non_blocking))	\
		    __done__ = cma_c_true;								\
		}														\
	    }														\
	FINALLY {													\
	    cma__io_epilog (type, fd);								\
	    cma__fd_unreserve (fd);									\
	    }														\
	ENDTRY														\
	}															\
    if (__res__ != -1)  post_process;							\
    return __res__;												\
    }

    /*
     * Since most CMA "thread-synchronous" I/O ("open"-type) operations on 
     * U*ix follow the exact same structure, the wrapper routines have been
     * condensed into a macro.
     *
     * The steps performed are as follows:
     *	1. Issue the open function.
     *	2. If the value returned indicates an error, return it to the caller.
     *  3. If the file descriptor returned is larger than what we think is the
     *	    maximum value (ie if it is too big for our database) then bugcheck.
     *  4. "Open" the "file" in the CMA file database.
     *	5. Return the file descriptor value to the caller.
     *
     * FIX-ME: for the time being, if the I/O operation returns EINTR, we 
     *	    simply return it to the caller; eventually, we should catch this 
     *	    and "do the right thing" (if we can figure out what that is).
     */

/*
 * This macro is used for all "open"-type functions which return a single file
 * desciptor by immediate value.
 */
#define cma__ts_open(func,arglist,post_process)  {		\
    int	__fd__;											\
    TRY {												\
	cma__int_init ();									\
	cma__int_lock (cma__g_io_data_mutex);				\
	__fd__ = func arglist;								\
	cma__int_unlock (cma__g_io_data_mutex);				\
	if (__fd__ >= 0 && __fd__ < cma__g_mx_file)			\
	    post_process;									\
	}													\
    CATCH_ALL											\
	{													\
	cma__set_errno (EBADF);								\
	__fd__ = -1;										\
	}													\
    ENDTRY												\
    if (__fd__ >= cma__g_mx_file)						\
	cma__bugcheck ("cma__ts_open:  fd is too large");	\
    return __fd__;										\
    }
/*
 * This macro is used for all "open"-type functions which return a pair of file
 * desciptors by reference parameter.
 */
#define cma__ts_open2(func,fdpair,arglist,post_process)  {				\
    int	    __res__;													\
    TRY {																\
	cma__int_init ();													\
	cma__int_lock (cma__g_io_data_mutex);								\
	__res__ = func arglist;												\
	cma__int_unlock (cma__g_io_data_mutex);								\
	if (__res__ >= 0 && fdpair[0] < cma__g_mx_file						\
		&& fdpair[1] < cma__g_mx_file)									\
	    post_process;													\
	}																	\
    CATCH_ALL															\
	{																	\
	cma__set_errno (EBADF);												\
	__res__ = -1;														\
	}																	\
    ENDTRY																\
    if ((fdpair[0] >= cma__g_mx_file) || (fdpair[1] >= cma__g_mx_file))	\
	cma__bugcheck ("cma__ts_open2:  one of fd's is too large");			\
    return __res__;														\
    }

/*
 * INTERNAL INTERFACES
 */
extern void
cma__close_general (int);

extern void cma__init_thread_io  (void);

extern cma_t_boolean cma__io_available  (cma__t_io_type,int, struct timeval *);

extern void cma__io_epilog  (cma__t_io_type,int);

extern void cma__io_prolog  (cma__t_io_type,int);

extern void cma__io_wait  (cma__t_io_type,int);

extern void cma__open_general  (int);

extern void cma__reinit_thread_io  (int);

extern void cma__set_nonblocking  (int);

extern void cma__set_user_nonblock_flags  (int,int);

extern cma_t_boolean cma__is_open (int);

extern cma_t_boolean cma__poll_io_ready  (cma__t_io_type,cma__t_poll_element);

extern void cma__poll_set_gio_events  (cma_t_integer,cma_t_integer);

extern void cma__poll_clear_gio_events  (cma_t_integer,cma_t_integer);

extern void cma__poll_set_gio_function  (cma_t_integer,cma__t_io_type);

extern void cma__poll_clear_gio_function  (cma_t_integer,cma__t_io_type);

extern void cma__poll_gio_reset  (cma_t_integer,cma_t_integer);

#endif



