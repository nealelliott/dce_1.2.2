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
 * @HP_DCE_VERSION@
 * @(#)Module: cma_thread_io.h $Revision: 1.1.8.1 $ $Date: 1996/05/10 13:29:46 $
 */
/*
 * HISTORY
 * $Log: cma_thread_io.h,v $
 * Revision 1.1.8.1  1996/05/10  13:29:46  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:51 UTC  bissen
 * 	Merge fix for CHFts13125 and CHFts15594.
 * 	[1995/07/31  23:06 UTC  jss  /main/HPDCE02/6]
 *
 * 	Fix for CHFts13125. In cma__ts_func2() macro, cast buf parameter
 * 	as a (char *) so that we can do pointer arithmetic.
 * 	[1995/07/29  17:58 UTC  jss  /main/HPDCE02/jss_mothra_fix6/1]
 *
 * 	Merge from branch jss_single_thread.
 * 	[1994/12/22  20:02 UTC  jss  /main/HPDCE02/5]
 *
 * 	Changes for single threaded client.  Conditionalize wrapper usage based on whether
 * 	CMA is initialized.
 * 	[1994/12/22  19:25 UTC  jss  /main/HPDCE02/jss_single_thread/1]
 *
 * 	Merge from branch jss_fork_mothra to HPDCE02.
 * 	[1994/12/09  16:23 UTC  jss  /main/HPDCE02/4]
 *
 * 	Fix exec wrapper by removing unnecessary routine cma__restore_nonblocking().
 * 	[1994/11/28  16:26 UTC  jss  /main/HPDCE02/jss_fork_mothra/1]
 *
 * 	Merge from jss_fix_close to HPDCE02.
 * 	[1994/09/15  19:22 UTC  jss  /main/HPDCE02/3]
 *
 * 	Fix for CHFts12442.  Fixed cancel problem in cma__io_wait and wrappers.
 * 	[1994/09/14  16:10 UTC  jss  /main/HPDCE02/jss_fix_close/3]
 *
 * 	Merge from branch jss_close_fix.
 * 	[1994/09/08  19:06 UTC  jss  /main/HPDCE02/2]
 *
 * 	Fix for CHFts12442. Only define _HP_CMA_ASYNC_CLOSE_ when _HP_LIBC_R is
 * 	defined.
 * 	[1994/09/08  17:00 UTC  jss  /main/HPDCE02/jss_fix_close/2]
 *
 * 	Fix for CHFts12442. Close while I/O pending bug.
 * 	[1994/08/30  14:25 UTC  jss  /main/HPDCE02/jss_fix_close/1]
 *
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:48 UTC  jss  /main/HPDCE02/1]
 *
 * 	CHFts11248
 * 	[1994/04/12  22:18 UTC  jss  /main/HPDCE01/2]
 *
 * 	Fix for CHFts11248. Added routine cma__restore_nonblocking to support
 * 	restoring state after a return from a failed exec() call
 * 	[1994/04/12  22:08 UTC  jss  /main/HPDCE01/jss_rodan_0323/1]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/25  14:33  truitt  /main/HPDCE01/1]
 *
 * 	CHFts09399: Added ts_func2 macro to allow send()
 * 	to successfully work with large messages.
 * 	[1993/11/29  15:52:59  truitt]
 * 	 *
 *
 * 	re-merge kk and dce1_0_3
 * 	[1993/09/23  19:28:57  truitt]
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:44:42  root  1.1.12.3]
 *
 * 	replace merged version with straight kk version for now
 * 	[1993/09/22  16:28:36  truitt]
 *
 * 	Add prototype for cma__set_user_nonblock_flags().  Fix up cma__issue_io_call()
 * 	macros -- just make the source cleaner.
 * 	[1993/07/19  18:31:42  blurie]
 *
 * 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
 * 	[1993/06/17  14:47:45  hinman]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:14:38  hopkins]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  19:06:17  root]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:35:42  hopkins]
 *
 * 	Gamera Merge #3
 * 	[1993/05/06  16:49:15  hopkins]
 *
 * 	Loading drop DCE1_0_2FNL
 * 	[1993/05/04  22:36:02  mgm  1.1.1.11]
 *
 * 	CR10892 - fix copyright in file
 * 	[1994/06/09  13:25:44  devsrc]
 * 	 *
 *
 * 	Moved from src/threads to here
 * 	[1994/04/01  17:04:04  jd]
 *
 * 	Add prototype for cma__set_user_nonblock_flags().  Fix up cma__issue_io_call()
 * 	macros -- just make the source cleaner.
 * 	[1993/07/19  18:31:42  blurie]
 *
 * 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
 * 	[1993/06/17  14:47:45  hinman]
 *
 * 	Fixed a race condition after the call to cma__fd_reserve. We need to be
 * 	sure that the mutex call in cma__fd_reserve didn't sleep long enough
 * 	to have the file closed on it.
 * 	[1993/04/16  19:53:58  jd]
 *
 * 	Gamera Merge II
 * 	[1993/04/25  01:24:27  hopkins]
 *
 * 	Loading drop DCE1_0_2B23
 *
 * 	[1993/04/21  20:46:21  root  1.1.1.10]
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
 * 	Gamera merge
 *
 * Revision 1.1.2.2  1994/06/09  13:34:53  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:25:44  devsrc]
 * 
 * Revision 1.1.2.1  1994/04/01  20:13:59  jd
 * 	Moved from src/threads to here
 * 	[1994/04/01  17:04:04  jd]
 * 
 * Revision 1.1.5.6  1993/04/16  19:55:18  jd
 * 	Fixed a race condition after the call to cma__fd_reserve. We need to be
 * 	sure that the mutex call in cma__fd_reserve didn't sleep long enough
 * 	to have the file closed on it.
 * 	[1993/04/16  19:53:58  jd]
 * 
 * Revision 1.1.5.5  1993/03/29  20:21:53  jd
 * 	Removed the cma__is_open macro. Made it a function in cma_thread_io.c
 * 	so things outside of the thread_io modules can check to see if cma
 * 	already knows about a file without including cma_thread_io.h and
 * 	cma_thrdio_defs.h. cma_fdopen in cma_stdio.c needs to do this.
 * 	[1993/03/25  18:42:28  jd]
 * 
 * Revision 1.1.5.4  1993/03/10  22:43:35  jd
 * 	In cma_fd_reserve don't call cma__int_lock with cma__g_file[fd]->mutex.
 * 	cma__g_file might get nulled during the possible block resulting in a seg
 * 	on the setting of mutex ownership in cma__int_lock. Use the pointer to
 * 	the mutex itself instead.
 * 	[1993/03/10  19:44:21  jd]
 * 
 * Revision 1.1.5.3  1992/12/15  22:32:03  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:43:12  alan]
 * 
 * Revision 1.1.5.2  1992/09/29  20:16:09  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:55:32  sekhar]
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

#ifdef __cplusplus
extern "C" {
#endif

/*
 * CONSTANTS
 */

/*
 * This enables changes that allow threads waiting for I/O to return
 * to the caller with EBADF when the file is closed by another thread.
 * Without this change, cma__io_available will bugcheck after doing a 
 * select on the closed fd.
 */
#ifdef _HP_LIBC_R
#  define _HP_CMA_ASYNC_CLOSE_
#endif

/*
 * Maximum number of files (ie, max_fd+1)
 */
#define cma__c_mx_file	FD_SETSIZE

/*
 * Number of bits per file descriptor bit mask (ie number of bytes * bits/byte)
 */
#define cma__c_nbpm	NFDBITS

#ifdef _HP_LIBC_R
/*
 * Number of submasks (ie "int" sized chunks) per file descriptor mask
 */
#define cma__c_nspm	((cma__c_mx_file + cma__c_nbpm - 1)/cma__c_nbpm)
#endif

/*
 * TYPE DEFINITIONS
 */

typedef enum CMA__T_IO_TYPE {
    cma__c_io_read   = 0,
    cma__c_io_write  = 1,
    cma__c_io_except = 2
    } cma__t_io_type;
#define cma__c_max_io_type	2

/*
 * From our local <sys/types.h>:
 *
 *  typedef long    fd_mask;
 *
 *  typedef struct fd_set {
 *          fd_mask fds_bits[howmany(FD_SETSIZE, NFDBITS)];
 *  } fd_set;
 *
 */
typedef fd_mask cma__t_mask;
typedef fd_set  cma__t_file_mask;



/*
 *  GLOBAL DATA
 */

/*
 * Maximum number of files (ie, max_fd+1) as determined by getdtablesize().
 */
extern int	cma__g_mx_file;

#ifdef _HP_CMA_ASYNC_CLOSE_
/*
 * Maximum number of file objects, determined by cma_g_mx_file + cma_g_mx_file / 4
 * to make sure that there are enough fo's available if some of them are still
 * busy with "zombie waiters" from a deffered close.
 */
extern int      cma__g_mx_fo;
#endif

/*
 * Number of submasks (ie "int" sized chunks) per file descriptor mask as
 * determined by getdtablesize().
 */
extern int	cma__g_nspm;

/*
 * MACROS
 */

#ifdef _HP_CMA_ASYNC_CLOSE_
/*
 * Save a file object that is pointed to in the cma__g_file array.  This is
 * used to see if the underlying file object was de-allocated while blocked.
 */
#define cma__hp_save_fid(idp,fd) { \
        cma__t_fo_id *__idp__ = (cma__t_fo_id *)(idp); \
	__idp__->fo = cma__g_file[fd]; \
	if(__idp__->fo) \
	    __idp__->ser = __idp__->fo->serial; \
	else \
	    __idp__->ser = 0; \
	}
/*
 * Test whether to file objects are the same.
 */
#define cma__hp_fid_match(idp,fd) \
	( ((cma__t_fo_id *)(idp))->fo == cma__g_file[fd] && \
	  ((cma__t_fo_id *)(idp))->ser == cma__g_file[fd]->serial )

#endif   /* _HP_CMA_ASYNC_CLOSE_ */

/*
 * Define a constant for the errno value which indicates that the requested
 * operation was not performed because it would block the process.
 */
# define cma__is_blocking(s) \
    ((s == EAGAIN) || (s == EWOULDBLOCK) || (s == EINPROGRESS) || \
     (s == EALREADY) || (s == EDEADLK))

/*
*	It is necessary to issue an I/O function, before calling cma__io_wait()
*	in the following cases:
*
*		*	This file descriptor has been set non-blocking by CMA
*		*	This file descriptor has been set non-blocking by the user.
*/

#define cma__issue_io_call(fd)					\
	( (cma__g_file[fd]->non_blocking) || \
	  (cma__g_file[fd]->user_fl.user_non_blocking) )


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
#define cma__fd_reserve(rfd) 	\
		{ \
		cma__t_int_mutex *__mutex__; \
		__mutex__ = cma__g_file[rfd]->mutex; \
		cma__int_lock (__mutex__); \
		if(cma__g_file[rfd] == (cma__t_file_obj *)cma_c_null_ptr) \
			cma__int_unlock(__mutex__); \
		}
		

/*
 * Unreserve a file descriptor
 */
#define cma__fd_unreserve(ufd)	cma__int_unlock (cma__g_file[ufd]->mutex)

/*
 * AND together two select file descriptor masks
 */
#define cma__fdm_and(target,a,b)					\
	{								\
	int __i__ = cma__g_nspm;					\
	while (__i__--)							\
	    (target)->fds_bits[__i__] =					\
		(a)->fds_bits[__i__] & (b)->fds_bits[__i__];		\
	}

/*
 * Clear a bit in a select file descriptor mask
 *
 * FD_CLR(n, p)  :=  ((p)->fds_bits[(n)/NFDBITS] &= ~(1 << ((n) % NFDBITS)))
 */
#define cma__fdm_clr_bit(n,p)	FD_CLR (n, p)

/*
 * Copy the contents of one file descriptor mask into another.  If the 
 * destination operand is null, do nothing; if the source operand is null, 
 * simply zero the destination.
 */
#define cma__fdm_copy(src,dst,nfds) {					\
	if (dst)							\
	    if (src) {							\
		cma__t_mask *__s__ = (cma__t_mask *)(src);		\
		cma__t_mask *__d__ = (cma__t_mask *)(dst);		\
		int __i__;						\
		for (__i__ = 0; __i__ < (nfds); __i__ += cma__c_nbpm)	\
		    *__d__++ = *__s__++;				\
		}							\
	    else							\
		cma__fdm_zero (dst);					\
	    }

/*
 * To increment count for each bit set in fd - mask
 */
#define cma__fdm_count_bits(map,count)					\
	{								\
	int	__i__ = cma__g_nspm;					\
	while (__i__--) {						\
	    cma__t_mask    __tm__;				        \
	    __tm__ = (map)->fds_bits[__i__];				\
	    while(__tm__) {						\
		(count)++;						\
		__tm__ &= ~(__tm__ & (-__tm__)); /* Assumes 2's comp */	\
		}							\
	    }								\
	}

/*
 * Test if a bit is set in a select file descriptor mask
 *
 * FD_ISSET(n,p)  :=  ((p)->fds_bits[(n)/NFDBITS] & (1 << ((n) % NFDBITS)))
 */
#define cma__fdm_is_set(n,p)	FD_ISSET (n, p)

/*
 * OR together two select file descriptor masks
 */
#define cma__fdm_or(target,a,b)						\
	{								\
	int __i__ = cma__g_nspm;					\
	while (__i__--)							\
	    (target)->fds_bits[__i__] =					\
		(a)->fds_bits[__i__] | (b)->fds_bits[__i__];		\
	}

/*
 * Set a bit in a select file descriptor mask
 * 
 * FD_SET(n,p)  :=  ((p)->fds_bits[(n)/NFDBITS] |= (1 << ((n) % NFDBITS)))
 */
#define cma__fdm_set_bit(n,p)	FD_SET (n, p)

/*
 * Clear a select file descriptor mask.
 */
#define cma__fdm_zero(n)						\
	cma__memset ((char *) n, 0, cma__g_nspm * sizeof(cma__t_mask))





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
#ifdef _HP_CMA_ASYNC_CLOSE_

/* not used by accept, since accept also returns an fd */
#define cma__ts_func(func,fd,arglist,type,post_process)	{ \
    cma_t_integer   __res__; \
    cma_t_boolean   __done__ = cma_c_false; \
    cma_t_boolean   __bad_wait__ = cma_c_false; \
    if (!cma__is_multithreaded()) return (func arglist); \
    if (!cma__hp_fd_reserve(fd)) return (cma__set_errno (EBADF), -1); \
    if (cma__issue_io_call(fd)) {\
	if ((!cma__g_file[fd]->set_non_blocking) && \
		(cma__g_file[fd]->non_blocking == cma_c_true)) \
	    cma__set_nonblocking(fd); \
        cma__interrupt_disable (0); \
	TRY { \
	    __res__ = func arglist; \
	    } \
	CATCH_ALL { \
	    cma__interrupt_enable (0); \
	    cma__fd_unreserve (fd); \
	    RERAISE; \
	    } \
	ENDTRY \
        cma__interrupt_enable (0); \
	if ((__res__ != -1) \
		|| (!cma__is_blocking (errno)) \
		|| (cma__g_file[fd]->user_fl.user_non_blocking)) \
	    __done__ = cma_c_true; \
	} \
    if (__done__) { \
	cma__fd_unreserve (fd); \
	} \
    else { \
	TRY { \
	    cma__io_prolog (type, fd); \
	    while (!__done__) { \
		cma__io_wait (type, fd, &__bad_wait__); \
		if( __bad_wait__ ) __done__ = cma_c_true; \
		else { \
		    __res__ = func arglist; \
		    if ((__res__ != -1) \
			    || (!cma__is_blocking (errno)) \
			    || (cma__g_file[fd]->user_fl.user_non_blocking)) \
		        __done__ = cma_c_true; \
		    } \
		} \
	    } \
	FINALLY { \
	    if( !__bad_wait__ ) { \
		cma__io_epilog (type, fd); \
	        cma__fd_unreserve (fd); \
		} \
	    } \
	ENDTRY \
	} \
    if (__bad_wait__) return( cma__set_errno(EBADF), -1 ); \
    if (__res__ != -1)  post_process; \
    return __res__; \
    }

#else

#define cma__ts_func(func,fd,arglist,type,post_process)	{ \
    cma_t_integer   __res__; \
    cma_t_boolean   __done__ = cma_c_false; \
    if (!cma__is_multithreaded()) return (func arglist); \
    if ((fd < 0) || (fd >= cma__g_mx_file)) return (cma__set_errno (EBADF), -1); \
    if (!cma__is_open(fd)) return (cma__set_errno (EBADF), -1); \
    cma__fd_reserve (fd); \
    if (!cma__is_open(fd)) return (cma__set_errno (EBADF), -1); \
    if (cma__issue_io_call(fd)) {\
	if ((!cma__g_file[fd]->set_non_blocking) && \
		(cma__g_file[fd]->non_blocking == cma_c_true)) \
	    cma__set_nonblocking(fd); \
        cma__interrupt_disable (0); \
	TRY { \
	    __res__ = func arglist; \
	    } \
	CATCH_ALL { \
	    cma__interrupt_enable (0); \
	    cma__fd_unreserve (fd); \
	    RERAISE; \
	    } \
	ENDTRY \
        cma__interrupt_enable (0); \
	if ((__res__ != -1) \
		|| (!cma__is_blocking (errno)) \
		|| (cma__g_file[fd]->user_fl.user_non_blocking)) \
	    __done__ = cma_c_true; \
	} \
    if (__done__) { \
	cma__fd_unreserve (fd); \
	} \
    else { \
	TRY { \
	    cma__io_prolog (type, fd); \
	    while (!__done__) { \
		cma__io_wait (type, fd); \
		__res__ = func arglist; \
		if ((__res__ != -1) \
			|| (!cma__is_blocking (errno)) \
			|| (cma__g_file[fd]->user_fl.user_non_blocking)) \
		    __done__ = cma_c_true; \
		} \
	    } \
	FINALLY { \
	    cma__io_epilog (type, fd); \
	    cma__fd_unreserve (fd); \
	    } \
	ENDTRY \
	} \
    if (__res__ != -1)  post_process; \
    return __res__;  \
    }
#endif  /* _HP_CMA_ASYNC_CLOSE_ */

    /*
     * While most thread-synchronous I/O operations can use the standard
     * version of the above macro, one case in particular needs a slightly
     * enhanced version.  For cma_send(), we have to support the case where
     * a user is trying to send a very large message via sockets, and does
     * not have non-blocking set.  In this case, only 8192 bytes of data is
     * sent at any one time.  We need to support the retry so that larger
     * messages, if no error is received, will send completely.  By making
     * this change, we are matching the behavior of cma_send() to send().
     *
     * We are adding this as a macro, and not in-line code for cma_send()
     * in case we run into another instance of this problem for some
     * other command, and then we can hope :-) this macro will just work.
     *
     * Note the only difference between this macro and the one above is
     * how it handles the "done" condition, both in the immediate (first)
     * part of the macro and in the retry (second) part.  Not getting an
     * error condition after send() is no longer enough to say we are
     * truly done.  We must instead compare the length of the sent
     * message to the length returned by send().  If they do not match,
     * we need to adjust the message and the length, and send() again.
     *
     * We set done to true for the following cases...
     *   1. the return value of send (res) equals the length
     *      of the message sent.
     *   2. an errno was received that was NOT a blocking type
     *      error.
     *   3. regardless of the comparison between res and length,
     *      the user set the socket non-blocking, so we stop at
     *      this point and return.
     *
     * We don't set done for the case where no error was found
     * (res != -1) and the return value is less that the length
     * of the message sent.  This means we will need to call send
     * once again.
     *
     * The implied else case is the return value = -1 and the errno
     * that is set is a blocking-type error (EAGAIN, EWOULDBLOCK, etc).
     * We can just continue here and not worry about anything else yet.
     * This do-nothing attitude matches the behavior of send().
     */
#ifdef _HP_CMA_ASYNC_CLOSE_
#define cma__ts_func2(func,fd,arglist,type,buf,length,post_process) { \
    cma_t_integer  __res__, __total__ = 0; \
    cma_t_boolean  __done__ = cma_c_false; \
    cma_t_boolean   __bad_wait__ = cma_c_false; \
    if (!cma__is_multithreaded()) return (func arglist); \
    if (!cma__hp_fd_reserve(fd)) return (cma__set_errno (EBADF), -1); \
    if (cma__issue_io_call(fd)) { \
        if ((!cma__g_file[fd]->set_non_blocking) && \
            (cma__g_file[fd]->non_blocking == cma_c_true)) { \
            cma__set_nonblocking(fd); \
        } \
        cma__interrupt_disable(0); \
        TRY { \
            __res__ = func arglist; \
        } \
        CATCH_ALL { \
            cma__interrupt_enable(0); \
            cma__fd_unreserve(fd); \
            RERAISE; \
        } \
        ENDTRY \
        cma__interrupt_enable(0); \
        if ((__res__ == length) || \
            (!cma__is_blocking(errno)) || \
            (cma__g_file[fd]->user_fl.user_non_blocking)) { \
            __done__ = cma_c_true; \
            __total__ = __res__; \
        } \
        else if ((__res__ != -1) && (__res__ < length)) { \
            __total__ = __total__ + __res__; \
            length = length - __res__; \
            buf = (char *)buf + __res__; \
        } \
    } \
    if (__done__) { \
        cma__fd_unreserve(fd); \
    } \
    else { \
        TRY { \
            cma__io_prolog(type, fd); \
            while (!__done__) { \
                cma__io_wait(type, fd, &__bad_wait__); \
 		if( __bad_wait__ ) __done__ = cma_c_true; \
		else { \
                    __res__ = func arglist; \
                    if ((__res__ == length) || \
                        (!cma__is_blocking(errno)) || \
                        (cma__g_file[fd]->user_fl.user_non_blocking)) { \
                        __done__ = cma_c_true; \
                        __total__ = __total__ + __res__; \
                    } \
                    else if ((__res__ != -1) && (__res__ < length)) { \
                        __total__ = __total__ + __res__; \
                        length = length - __res__; \
                        buf = (char *)buf + __res__; \
                    } \
		} \
            } \
        } \
	FINALLY { \
	    if( !__bad_wait__ ) { \
		cma__io_epilog (type, fd); \
	        cma__fd_unreserve (fd); \
		} \
	    } \
	ENDTRY \
	} \
    if (__bad_wait__) return( cma__set_errno(EBADF), -1 ); \
    if (__total__ != -1)  post_process; \
    return __total__; \
    }

#else

#define cma__ts_func2(func,fd,arglist,type,buf,length,post_process) { \
    cma_t_integer  __res__, __total__ = 0; \
    cma_t_boolean  __done__ = cma_c_false; \
    if (!cma__is_multithreaded()) return (func arglist); \
    if ((fd < 0) || (fd >= cma__g_mx_file)) \
        return (cma__set_errno (EBADF), -1); \
    if (!cma__is_open(fd)) \
        return (cma__set_errno (EBADF), -1); \
    cma__fd_reserve(fd); \
    if (!cma__is_open(fd)) \
        return (cma__set_errno (EBADF), -1); \
    if (cma__issue_io_call(fd)) { \
        if ((!cma__g_file[fd]->set_non_blocking) && \
            (cma__g_file[fd]->non_blocking == cma_c_true)) { \
            cma__set_nonblocking(fd); \
        } \
        cma__interrupt_disable(0); \
        TRY { \
            __res__ = func arglist; \
        } \
        CATCH_ALL { \
            cma__interrupt_enable(0); \
            cma__fd_unreserve(fd); \
            RERAISE; \
        } \
        ENDTRY \
        cma__interrupt_enable(0); \
        if ((__res__ == length) || \
            (!cma__is_blocking(errno)) || \
            (cma__g_file[fd]->user_fl.user_non_blocking)) { \
            __done__ = cma_c_true; \
            __total__ = __res__; \
        } \
        else if ((__res__ != -1) && (__res__ < length)) { \
            __total__ = __total__ + __res__; \
            length = length - __res__; \
            buf = (char *)buf + __res__; \
        } \
    } \
    if (__done__) { \
        cma__fd_unreserve(fd); \
    } \
    else { \
        TRY { \
            cma__io_prolog(type, fd); \
            while (!__done__) { \
                cma__io_wait(type, fd); \
                __res__ = func arglist; \
                if ((__res__ == length) || \
                    (!cma__is_blocking(errno)) || \
                    (cma__g_file[fd]->user_fl.user_non_blocking)) { \
                   __done__ = cma_c_true; \
                   __total__ = __total__ + __res__; \
                } \
                else if ((__res__ != -1) && (__res__ < length)) { \
                    __total__ = __total__ + __res__; \
                    length = length - __res__; \
                    buf = (char *)buf + __res__; \
                } \
            } \
        } \
        FINALLY { \
            cma__io_epilog(type, fd); \
            cma__fd_unreserve(fd); \
        } \
        ENDTRY \
    } \
    if (__total__ != -1)  post_process; \
    return __total__; \
    }
#endif  /* _HP_CMA_ASYNC_CLOSE_ */

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
    int	__fd__;							\
    if (!cma__is_multithreaded()) return (func arglist);        \
    TRY {							\
	cma__int_init ();					\
	cma__int_lock (cma__g_io_data_mutex);			\
	__fd__ = func arglist;					\
	cma__int_unlock (cma__g_io_data_mutex);			\
	if (__fd__ >= 0 && __fd__ < cma__g_mx_file)		\
	    post_process;					\
	}							\
    CATCH_ALL							\
	{							\
	cma__set_errno (EBADF);					\
	__fd__ = -1;						\
	}							\
    ENDTRY							\
    if (__fd__ >= cma__g_mx_file)				\
	cma__bugcheck ("cma__ts_open:  fd is too large");	\
    return __fd__;						\
    }
/*
 * This macro is used for all "open"-type functions which return a pair of file
 * desciptors by reference parameter.
 */
#define cma__ts_open2(func,fdpair,arglist,post_process)  {		\
    int	    __res__;							\
    if (!cma__is_multithreaded()) return (func arglist);                \
    TRY {								\
	cma__int_init ();						\
	cma__int_lock (cma__g_io_data_mutex);				\
	__res__ = func arglist;						\
	cma__int_unlock (cma__g_io_data_mutex);				\
	if (__res__ >= 0 && fdpair[0] < cma__g_mx_file			\
		&& fdpair[1] < cma__g_mx_file)				\
	    post_process;						\
	}								\
    CATCH_ALL								\
	{								\
	cma__set_errno (EBADF);						\
	__res__ = -1;							\
	}								\
    ENDTRY								\
    if ((fdpair[0] >= cma__g_mx_file) || (fdpair[1] >= cma__g_mx_file)) \
	cma__bugcheck ("cma__ts_open2:  one of fd's is too large"); \
    return __res__;							\
    }

/*
 * INTERNAL INTERFACES
 */
extern void cma__close_general  (int);

extern void cma__init_thread_io  (void);

extern cma_t_boolean cma__io_available  (cma__t_io_type,int,struct timeval *);

extern void cma__io_epilog  (cma__t_io_type,int);

extern void cma__io_prolog  (cma__t_io_type,int);

#ifdef _HP_CMA_ASYNC_CLOSE_
extern void cma__io_wait  (cma__t_io_type,int, cma_t_boolean *);
#else
extern void cma__io_wait  (cma__t_io_type,int);
#endif

extern void cma__open_general  (int);

extern void cma__reinit_thread_io  (int);

extern void cma__set_nonblocking  (int);

extern void cma__set_user_nonblock_flags  (int,int);

extern cma_t_boolean cma__is_open (int);

extern void cma__set_blocking  (int);

#ifdef _HP_CMA_ASYNC_CLOSE_
extern cma_t_boolean cma__hp_fd_reserve (int);
#endif

#ifdef __cplusplus
}
#endif

#endif


/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_THREAD_IO.H */
/*  *16   11-NOV-1991 11:56:20 BUTENHOF "Fix bug in open macro" */
/*  *15    6-NOV-1991 09:06:48 BUTENHOF "Handle SIGPIPE during write/send ops." */
/*  *14   14-OCT-1991 18:42:33 CURTIN "Added TRY/CATCH blocks" */
/*  *13   17-SEP-1991 13:21:43 BUTENHOF "Handle variable # of FDs more efficiently" */
/*  *12   18-JUN-1991 15:41:09 CURTIN "including new include for rios" */
/*  *11   11-JUN-1991 10:37:32 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *10   10-JUN-1991 19:22:10 BUTENHOF "Fix the sccs headers" */
/*  *9    10-JUN-1991 18:24:46 SCALES "Add sccs headers for Ultrix" */
/*  *8    31-MAY-1991 18:43:23 SCALES "Add close-general function" */
/*  *7    24-MAY-1991 16:50:20 CURTIN "Added a new reinit routine" */
/*  *6    10-MAY-1991 13:18:24 CURTIN "Removed extra underscore from routine prototypes" */
/*  *5     8-MAY-1991 18:34:47 SCALES "Correct number of underscores in fdm_copy" */
/*  *4     7-MAY-1991 10:13:54 CURTIN "Replaced cma_init with cma__int_init" */
/*  *3     2-MAY-1991 22:29:41 SCALES "convert to system def's for fd masks" */
/*  *2     2-MAY-1991 16:31:13 CURTIN "Made into a number of smaller modules" */
/*  *1    12-DEC-1990 21:54:56 BUTENHOF "Thread synchronous I/O" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_THREAD_IO.H */
