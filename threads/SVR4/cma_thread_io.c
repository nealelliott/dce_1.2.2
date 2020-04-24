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
 * $Log: cma_thread_io.c,v $
 * Revision 1.1.4.2  1996/02/17  23:33:35  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:49:47  marty]
 *
 * 	Created the cma__is_open function to replace the macro that used to be
 * 	in cma_thread_io.h. The is done so things outside of the thread_io modules
 * 	can check to see if cma already knows about a file without including
 * 	cma_thread_io.h and cma_thrdio_defs.h. cma_fdopen in cma_stdio.c needs to
 * 	do this.
 * 	[1993/03/25  18:48:04  jd]
 *
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  17:33:25  hinman]
 *
 * 	[hinman] - Check in merged SNI version
 * 	[1993/01/11  16:23:05  hinman]
 *
 * 	Insert copyright notices
 * 	[1992/12/07  19:43:05  alan]
 *
 * 	Fix OT defect 5344:  Since we're making OSF/1-specific hacks
 * 	anyhow, avoid calling select() on block-special files (on this
 * 	platform only).
 * 	[1992/10/16  19:54:10  bolinger]
 *
 * 	Obey "Boolean" convention for new is_char_dev member.
 * 	[1992/10/09  13:42:35  bolinger]
 *
 * 	For OSF/1 only, cause character special devices to be set as
 * 	non-blocking.  This is needed in OSF/1 1.1.1 because pipes
 * 	are implemented as STREAMS (i.e., character special) devices,
 * 	and because CMA needs pipes to be non-blocking.  At the same
 * 	time, since other character special devices may not accept
 * 	the fcntl() used to set non-blocking mode, permit the fcntl()
 * 	to fail if the fd involved refers to a character special
 * 	device.
 * 	[1992/10/08  20:11:20  bolinger]
 *
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:55:20  sekhar]
 *
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:07:14  bolinger]
 *
 * 	Move latest 1.0.1-based revision into 1.0.2.
 * 	[1992/08/28  20:42:57  bolinger]
 *
 * Revision 1.1.4.1  1995/12/07  21:57:54  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:03:57  root]
 * 
 * Revision 1.1.2.2  1994/06/10  20:53:15  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  16:49:55  devsrc]
 * 
 * 	Bring forward IBM integration changes to BL10 sources.
 * 	[1992/08/28  14:14:54  bolinger]
 * 
 * 	Bring forward 1.0.1 revision 1.1.2.2 (fix for OT defect 2540).
 * 	[1992/08/10  16:35:18  bolinger]
 * 
 * Revision 1.1.2.1  1994/04/19  20:07:01  ohara
 * 	lost and found in ODE
 * 	[1994/04/19  20:05:49  ohara]
 * 
 * 	moved again
 * 	[1994/04/14  20:59:38  ohara]
 * 
 * Revision 1.1.2.1  1994/04/01  20:15:04  jd
 * 	Moved from src/threads to here
 * 	[1994/04/01  18:31:23  jd]
 * 
 * Revision 1.1.18.3  1993/10/25  19:03:20  ohara
 * 	svr4 changes need
 * 	[1993/10/25  18:59:20  ohara]
 * 
 * Revision 1.1.18.2  1993/10/20  21:30:30  jd
 * 	(5151) Drop from HP.
 * 	[1993/10/20  21:03:38  jd]
 * 
 * Revision 1.1.18.1  1993/10/16  14:25:45  jd
 * 	Added check for ENOENT to the fstat error checking in cma_
 * 	open_general.
 * 	[1993/10/15  22:32:51  jd]
 * 
 * Revision 1.1.14.4  1993/07/19  20:16:49  blurie
 * 	Fix for OT #8155 (cma__set_nonblocking() may use fcntl() in an
 * 	inconsistent way), and OT #8219 (CMA will not return EAGAIN for an
 * 	inherited file descriptor), OT #7955 (read() blocks when
 * 	open(..,..,O_NDELAY) is used).
 * 
 * 		In cma__open_general(), get the flags already set on the file
 * 		descriptor and update the CMA IO database, non-blocking flags
 * 		with the information.
 * 
 * 		Add new cma__set_user_nonblock_flags() which actually sets the
 * 		CMA IO database non-blocking flags.
 * 
 * 		Make sure that cma__set_nonblocking() gets the user flags and
 * 		OR's them with the ones that CMA wants to set instead of directly
 * 		overwriting the user's one's.
 * 	[1993/07/19  18:24:37  blurie]
 * 
 * Revision 1.1.14.3  1993/06/24  21:20:22  hinman
 * 	[hinman] - Pick up recent HP change
 * 	[1993/06/21  12:46:19  hinman]
 * 
 * Revision 1.1.14.2  1993/06/15  21:10:56  sommerfeld
 * 	Improve error message from select error bugcheck.
 * 	[1993/06/15  20:39:54  sommerfeld]
 * 
 * Revision 9.5.1.11  92/12/21  14:40:14  raj
 * 	[raj] Roll out roll-in that caused problesm!
 * 
 * Revision 9.5.4.5  92/12/21  14:36:49  raj
 * 	[raj] - Roll back previous stuff to earlier version, which was better
 * 
 * Revision 9.12.1.4  92/12/21  13:43:53  root
 * 	Auto checkin of OSF rev 1.1.5.5
 * 
 * Revision 1.1.2.2  1992/05/09  00:06:52  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/09  00:06:34  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/21  20:36:15  keane
 * 	Additional refinements to fix for 2540.
 * 	[1992/05/21  19:07:02  keane]
 * 
 * 	Address defect 2540.  Move code to set files non-blocking from
 * 	cma__open_general to a new routine cma__set_nonblocking.  This new
 * 	routine is called before the first I/O operation on a file, and has the
 * 	effect of deferring setting the file non-blocking until the file is
 * 	actually used.
 * 	[1992/05/15  21:30:30  keane]
 * 
 * Revision 1.1  1992/01/19  14:40:49  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *  Copyright (c) 1990, 1992 by
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
 *	Thread synchronous I/O, Module 1:  Wrapper-support routines
 *
 *  AUTHORS:
 *
 *	Hans Oser
 *
 *  CREATION DATE:
 *
 *	20 September 1989
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Hans Oser	25 October 1989
 *		To complete CMA baselevel routines
 *	002	Hans Oser	26 October 1989
 *		program review changes
 *	003	Webb Scales	27 October 1989
 *		Added #include <cma_host.h> for cma__test_and_set
 *		Corrected cma__test_and_set & cma__unset arg to be by address
 *		Corrected cma_cond_wait_io_end references (to "cma__")
 *		Commented out cma__convert_unix_delta_time_to_absolute_ticks()
 *	004	Dave Butenhof	All Saints Day 1989
 *		Include cma_dispatch header file to get enter/exit kernel
 *		macro.
 *	005	Bob Conti	4 November 1989
 *		Removed superfluous include cma_host
 *	006	Dave Butenhof	16 November 1989
 *		Adapt to new timer interfaces.
 *	007	Webb Scales	19 November 1989
 *		Fix one that Dave missed ^
 *	008	Dave Butenhof	4 December 1989
 *		Upgrade to POSIXish names.
 *	009	Webb Scales	5 December 1989
 *		Completely overhauled non-blocking I/O
 *	010	Webb Scales	10 December 1989
 *		Made the select queue module-local.
 *		Corrected exclusive access to I/O data structures and routines.
 *		Added a check-I/O deferal for SIGIOs during kernel actions.
 *	011	Webb Scales	12 December 1989
 *		Corrected the cma__fdm_copy macro to test src instead of dst
 *	012	Webb Scales & Dave Butenhof	14 December 1989
 *		Handle alerts around condition waits.
 *	013	Webb Scales	9 January 1990
 *		- handle null timeout values correctly in cma_select
 *		- convert timeout to absolute time before inserting on the 
 *		    timer queue.
 *		- modify global select fd masks appropriately
 *		- make the call to cma_select alertable
 *		- handle file descriptor masks correctly
 *	014	Webb Scales	7 February 1990
 *		Fixed a bug (typo) in cma__io_available.  Also, file object
 *		condition variables are now created at initialization.
 *	015	Dave Butenhof	5 March 1990
 *		Integrate Kevin Ackley's changes for Apollo M68000 port.
 *	016	Webb Scales	20 March 1990
 *		Do not enable SIGIO by default.
 *		Moved cma___thread_io_signal to cma_signal.c.
 *	017	Webb Scales	27 March 1990
 *		Added additional thread-synchronous I/O routine wrappers.
 *		Deleted cma_shutdown.  Added cma_socketpair.
 *	018	Webb Scales & Dave Butenhof	29 March 1990
 *		Make select file descriptor masks available to the null thread.
 *	019	Webb Scales	26 April 1990
 *		Fixed cut-and-paste-o and simplified wrapper macros.
 *	020	Webb Scales	2 May 1990
 *		Disable interrupts during call to I/O functions.
 *	021	Webb Scales	4 June 1990
 *		Fix copy-fd macro to no-op if destination is null.
 *	022	Webb Scales	12 June 1990
 *		Fixed if-else nesting for blocking select.
 *	023	Webb Scales	13 June 1990
 *		Put a loop in _ts_ macro to avoid potential reading race.
 *		Modified _ts_ macro to allow accept to call open_general if
 *		    it is successful.
 *		Added names to IO mutexes and cv's.
 *		Fixed up all condition waits with proper cleanups.
 *	024	Webb Scales	24 July 1990
 *		Added 'creat', 'dup', and 'dup2'.
 *	025	Webb Scales	16 August 1990
 *		Accomodate Sun and HP platforms
 *	026	Dave Butenhof	10 September 1990
 *		Fix calls to external CMA functions to conform to new
 *		interfaces.
 *	027	Webb Scales	12 September 1990
 *		Put in workaround for Ultrix non-blocking I/O bug:  use (new)
 *		Berkley flag instead of POSIX flag in 'fcntl'.
 *	028	Webb Scales	13 September 1990
 *		Changed cond signals to interrupt version.
 *	029	Webb Scales	20 September 1990
 *		Added second mutex to TCB.
 *		Updated set-object-name to use correct field in handle.
 *	030	Webb Scales	25 September 1990
 *		Fixed select, threads now insert themselves on a separate queue.
 *		Removed vestiges of per-thread file RW pointers.
 *	031	Webb Scales	 2 October 1990
 *		Access to select queue is now under kernel.
 *		Added Apollo to set-nonblocking-mode #if tree.
 *		Added an if test to the ts_func macro.
 *		Rewrote cma_connect.
 *	032	Webb Scales	11 October 1990
 *		Added exceptfds mask to test that signals a thread waiting for
 *		I/O in io_available.
 *	033	Webb Scales	24 October 1990
 *		Revert to file-type sensitivity, but get the type from stat.
 *	034	Webb Scales	26 October 1990
 *		Make wrappers initialize CMA.
 *	034	Webb Scales	 4 December 1990
 *		Add timeout parameter to io_available.
 *		Make select file descriptor masks private again.
 *	035	Webb Scales	 6 December 1990
 *		Conditionalized a file type for Apollo.
 *	036	Webb Scales	11 December 1990
 *		Added wrapper for fcntl().
 *	037	Dave Butenhof	2 January 1991
 *		Correct several misspelled mutex operations.
 *	038	Webb Scales	28 January 1991
 *		Repackage import_fd (as open_general) to break circular init 
 *		dependency; added init calls to open-type macros.
 *		Fixed a non-blocking bug in close.
 *		io_available now bugchecks (temporarily) on closed files.
 *	039	Webb Scales	30 January 1991
 *		Created separate code paths for blocking & non-blocking mode 
 *		files.
 *	040	Dave Butenhof	1 February 1991
 *		Change all external synchronization to internal forms, so
 *		that the operations can be inlined to improve performance.
 *	041	Webb Scales	19 February 1990
 *		Modify IO_available so that a EINTR causes it to return
 * 		false.
 *	042	Dave Butenhof	27 February 1991
 *		Work around some compilation errors on RS/6000 platform.
 *	043	Webb Scales	28 March 1991
 *		Put in error handling to catch if stdin, stdout, or stderr 
 *		are closed when cma_init() is called.
 *	044	Dave Butenhof	09 April 1991
 *		Don't reference kernel_critical flag directly
 *	045	Paul Curtin	2 May 1991
 *		Segmented contents into cma_thdio_2/3/4 modules, etc
 *	046	Webb Scales	2 May 1991
 *		- Moved cma_select prototype to the module referencing it.
 *		- Moved cma_import_fd prototype to cma_ux.h.
 *		- Put a test in io_available to skip files which select says 
 *		    are ready but which we don't have listed as "open".
 *		- Moved includes from cma_thdio_defs.h to here.
 *		- Added argument to bugcheck calls
 *		- Cleaned up fcntl non-blocking mode flag definition
 *	047	Dave Butenhof	08 May 1991
 *		Use new macro to test for kernel already locked, which
 *		evaporates under multiprocessor implementation.
 *	048	Paul Curtin	10 May 1991
 *		Removed extra underscore from routine names ...
 *	049	Paul Curtin	24 May 1991
 *		Added cma__reinit_thread_io routine.
 *	050	Dave Butenhof	24 May 1991
 *		Fix typo in 050.
 *	051	Webb Scales	30 May 1991
 *		Added routine, close-general
 *	052	Paul Curtin	 5 June 1991
 *		Rearranged flags in reinit routine.
 *	053	Dave Butenhof	13 June 1991
 *		Now that ULTRIX V4.2 has increased the number of file
 *		descriptors from 64 to 4096, the process of creating a mutex
 *		and two condition variables for each file descriptor during
 *		DECthreads initialization is unreasonably expensive (and
 *		rather ridiculous). Change cma__open_general to create the
 *		sync objects dynamically if necessary.
 *	054	Webb Scales	20 June 1991
 *		Corrected the routine name in the assertion text in 
 *		close-general and some other random tidying.
 *	055	Dave Butenhof	12 September 1991
 *		Integrate Nat Mishkin's performance improvements to make more
 *		intelligent use of the file descriptor numbers; ULTRIX V4.2
 *		allows 4096 files, but most systems are generated to support
 *		lower numbers, and DECthreads is currently carrying around
 *		all that extra baggage. With this change, it'll only check
 *		and use the ones actually supported on the system. Also,
 *		shrink the TCB by dynamically allocating the file descriptor
 *		mask at thread creation time.
 *	056	Dave Butenhof	19 September 1991
 *		Integrate HPUX CMA5 reverse drop: fix cma__close_general() to
 *		close FD regardless of whether underlying file object has
 *		other FDs attached to it; also add HP-specific return status
 *		from fstat() (where EINVAL may mean legal network device).
 *	057	Dave Butenhof	04 October 1991
 *		Use new _CMA_THREAD_SYNC_IO_ to disable compilation, rather
 *		than _CMA_UNIPROCESSOR_.
 *	058	Dave Butenhof	29 October 1991
 *		Make the local file masks in io_available static to minimize
 *		the risk of overrunning a stack.
 *	059	Webb Scales	25 November 1991
 *		- Correct the mutex locking order of the "I/O Data mutex" 
 *		    w.r.t. the per-file mutexes.  
 *		- Use the symbolic constants in the fork handler instead of the 
 *		    hard-coded values.
 *		- Merge in Dale Tonogai's additions to the reinitialization.
 *	060	Dave Butenhof	11 February 1992
 *		Change dynamic file object initialization to use a mutex
 *		rather than a spin lock (which opened up the remote
 *		possibility of deadlocks, and the realistic likelyhood of
 *		unnecessary delays).
 *	061	Dave Butenhof	11 February 1992
 *		A law of nature has just been changed: cma__alloc_mem now
 *		returns cma_c_null_ptr on an allocation failure, rather than
 *		raising an exception. This allows greater efficiency, since
 *		TRY is generally expensive. Anyway, apply the process of
 *		evolution: adapt or die.
 *	062	Webb Scales	16 April 1992
 *		Add branch for EINVAL in the open-general fstat errror switch
 *		on Ultrix.
 */


/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <cma_util.h>
#include <cma_vm.h>

#if !_CMA_THREAD_SYNC_IO_		/* Don't compile unless needed! */

#include <cma_defs.h>
#include <cma_thdio_defs.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <cma_errors.h>
#include <cma_tcb.h>
# include <stropts.h>
# include <poll.h>
#if _CMA_PLATFORM_ == _CMA__DCOSX_MIPS
# include <sys/resource.h>
#endif



/*
 * TYPE DEFINITIONS
 */

/*
 * CONSTANTS AND MACROS
 */
#define cma___c_non_blocking_flag O_NONBLOCK

/*
 * GLOBAL DATA
 */

cma__t_file_obj     **cma__g_file;
cma__t_file_obj     *cma__g_file_obj;
cma_t_integer	    cma__g_file_num = 0;

/*
*	This is the central I/O wait structure;  an array of pollfd structures.  
*	Currently, this structure is allocated at process init time.  This
*	can be easily extended to be dynamically allocated (and reallocated).
*	This would enable support for SVR4's ability to change the number
*	of process fd's at runtime.
*/
cma__t_poll_element *cma__g_poll_e_p;
cma__t_int_mutex    *cma__g_io_data_mutex;
cma__t_queue	    cma__g_select_queue;
cma_t_integer	    cma__g_mx_file;

EXCEPTION	    io_avail_exc;
cma_t_integer	    io_avail_null_thread = 0;
/*
 * LOCAL DATA
 */

/*
 * The following mutex is used when looking for free file objects.
 */
static cma__t_int_mutex	*cma___g_file_lock;

/*
 * LOCAL FUNCTIONS
 */

/*
 * FUNCTIONAL DESCRIPTION:
 *
 *     cma__is_open
 *
 *     Determine whether or not the CMA I/O database knows about the
 *     file associated with a given file descriptor.
 *
 * FORMAL PARAMETERS:
 *
 *     fd - a Unix file descriptor, a number.
 *
 * IMPLICIT INPUTS: 
 * 
 *     The CMA I/O database
 * FUNCTIONAL VALUE:
 * 
 *     boolean true if the file has been opened by CMA
 *
 * SIDE EFFECTS:
 *
 *     None
 */
extern cma_t_boolean
cma__is_open(int fd)
    {
	return(cma__g_file[(fd)] != (cma__t_file_obj *)cma_c_null_ptr);
    }


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma__close_general
 *
 *	The common "file" close function:  this function closes a "file" from
 *	the point of view of the CMA I/O database.
 *
 *  FORMAL PARAMETERS:
 *
 *	fd  -  a Unix file descriptor, a number.
 *
 *	NOTE:  The file must have been reserved and the I/O data mutex must 
 *		have been locked by the caller.
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
 *	None
 *
 *  SIDE EFFECTS:
 *
 *	The file is unreserved.
 */
extern void
cma__close_general(int	fd)
    {
    cma__t_file_obj *temp = cma__g_file[fd];


    if ((fd < 0) || (fd >= cma__g_mx_file)) cma__error (cma_s_badparam);

    if (!cma__is_open(fd)) cma__error (cma_s_badparam);

    cma__assert_warn (
	    cma__g_file[fd]->ref_count,
	    "cma__close_general: file descriptor is already closed");
    cma__assert_warn (
	    ((cma__g_file[fd]->waiters[(int)cma__c_io_read] == 0) &&
	     (cma__g_file[fd]->waiters[(int)cma__c_io_write] == 0) &&
	     (cma__g_file[fd]->waiters[(int)cma__c_io_getmsg] == 0) &&
	     (cma__g_file[fd]->waiters[(int)cma__c_io_getmsg_pri] == 0) &&
	     (cma__g_file[fd]->waiters[(int)cma__c_io_getmsg_band] == 0) &&
	     (cma__g_file[fd]->waiters[(int)cma__c_io_putmsg] == 0) &&
	     (cma__g_file[fd]->waiters[(int)cma__c_io_putpmsg] == 0) &&
	     (cma__g_file[fd]->waiters[(int)cma__c_io_pollin] == 0)),
	    "cma__close_general:  Closing a file with threads waiting to perform I/O.");
    cma__assert_warn ( ! cma__g_poll_e_p[fd].events,
	    "cma__close_general:  Closing a file with bits set in global poll structure.");

    cma__trace ((cma__c_trc_io, "(close_general) fd %d closed", fd));

    /*
     * If there are no more references to the file object, make sure that the
     * reference count is exactly 0 so it can be reused later.
     */
    if ((--cma__g_file[fd]->ref_count) <= 0)
	cma__g_file[fd]->ref_count = 0;

    /*
     * Break the link -- this "closes" the file.
     */
    cma__g_file[fd] = (cma__t_file_obj *)cma_c_null_ptr;
    cma__int_unlock (temp->mutex);	/* cma__fd_unreserve (fd); */
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma__init_thread_io: initialize CMA thread-synchronous I/O database
 *
 *  FORMAL PARAMETERS:
 *
 *	None
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
 *	None
 *
 *  SIDE EFFECTS:
 *
 *	None
 */
extern void
cma__init_thread_io(void)
    {
    size_t	    	nfds, cur_fd, max_fds;
	cma_t_integer	poll_status;
    struct  rlimit  rip;
	cma_t_boolean	iterating;
    cma_t_integer   i;

    EXCEPTION_INIT(io_avail_exc);


    cma__g_io_data_mutex = cma__get_mutex (&cma__g_def_attr);
    cma__obj_set_name (cma__g_io_data_mutex, "io database");
    cma___g_file_lock = cma__get_mutex (&cma__g_def_attr);
    cma__obj_set_name (cma___g_file_lock, "dynamic io init");

    cma__queue_init (&cma__g_select_queue);


    /* 
     * Initialize CMA's I/O database such that all files are initially closed.
     */
    cma__g_file = (cma__t_file_obj **)cma__alloc_mem (
	    sizeof (cma__t_file_obj *) * cma__g_mx_file);
    cma__g_file_obj = (cma__t_file_obj *)cma__alloc_mem (
	    sizeof (cma__t_file_obj) * cma__g_mx_file);
	cma__g_poll_e_p = (cma__t_poll_element *)cma__alloc_mem (
		sizeof (cma__t_poll_element) * cma__g_mx_file);

    for (i = 0; i < cma__g_mx_file; i++) {
	cma_t_integer j;

	cma__g_file_obj[i].mutex = (cma__t_int_mutex *)cma_c_null_ptr;
	cma__g_file_obj[i].ref_count = 0;

	for (j = 0; j <= cma__c_max_io_type; j++)
		{
		cma__g_file_obj[i].waiters[j] = 0;
		}
	cma__g_poll_e_p[i].fd = -1;
	cma__g_poll_e_p[i].events = 0;
	cma__g_poll_e_p[i].revents = 0;
	cma__g_file[i] = (cma__t_file_obj *)cma_c_null_ptr;
	}


	    cma__g_poll_e_p[i].revents = 0;

    /*
     *  Make sure that CMA knows about the files which are open
     *  currently.
     */

    /*
     *  Get the current soft limit for file descriptors.  We can't use
     *  cma__g_mx_file because that is the hard limit for this process;
     *  poll() will return EINVAL if we pass more fds than the current
     *  soft limit for this process.
     */ 

    if ( getrlimit(RLIMIT_NOFILE, &rip) == -1 )
		{
		cma__bugcheck("cma__init_thread_io: getrlimit() returned failure");
		}
    max_fds = rip.rlim_cur;


    /*
     *  Initialize the poll structure that we are going to use.
	 *	We use the global poll structure since it is not in use yet.
     */
    for (i = 0; i < max_fds; i++)
	    {
	    cma__g_poll_e_p[i].fd = i;
	    cma__g_poll_e_p[i].events = POLLIN;
	    cma__g_poll_e_p[i].revents = 0;
	    }

    /*
     *  Now find out which files are open.
	 *
	 *	The basic idea is to poll all file descriptors which could
	 *  be open (this is the current soft limit for RLIMIT_NOFILE).
	 *	Unfortunately, there may be some open files which do not
	 *	support polling (for example /dev/null).  This type of 
	 *	file will cause poll() to fail with ENODEV.
	 *
	 *	When this happens, we will iterate (by one) from the current
	 *	fd being polled (cur_fd) until we get ENODEV again.  In
	 *	the worst case, it will be necessary to perform one poll()
	 *	call on every file descriptor from 0 to max_fds (in the
	 *	case where the file descriptor max_fds is the one causing
	 *	the ENODEV).  In the normal case though, it will be a very
	 *	low numbered file descriptor which is causing this (for
	 *	example when stdout is redirected to /dev/null).  Therefore
	 *	we don't worry about the potential overhead of having to
	 *	issue one poll() call for every file descriptor. 
     *
     */

    cur_fd = 0;				/* # of first fd being polled */
    nfds = max_fds;			/* # of fds being polled */
	iterating = cma_c_false;/* trying to find which fd caused ENODEV */

    while ( cur_fd < max_fds )
		{
		if ((poll_status = __nr_poll(&cma__g_poll_e_p[cur_fd], nfds, 0 )) == -1)
			{
			if ( cma__get_errno() != ENODEV )
				{
				cma__bugcheck("cma__init_thread_io:  poll() returned failure");
				}
			}
		
		if ( poll_status == -1 ) 
			{
			if ( iterating == cma_c_false )
				{
				/*
				*	One of the fds does not support polling.
				*	We need to iterate until we find the
				*	offending one.
				*
				*	Since nfds tells how many fds to poll at once, we 
				*	need to set it to one.
				*/
				nfds = 1;
				iterating = cma_c_true;
				continue;
				}
			else
				{
				/*
				*	We have found the fd which does not support polling.
				*	Put it into the CMA database and indicate that
				*	we are not iterating anymore so that we go back
				*	to "batch" polling.
				*/
				cma__open_general (cur_fd);
				iterating = cma_c_false;

				/*
				*	Reset the global I/O database.
				*/
				cma__g_poll_e_p[cur_fd].fd = -1;
				cma__g_poll_e_p[cur_fd].events = 0;
				cma__g_poll_e_p[cur_fd].revents = 0;
				}
			}
		else
			{
			for (i = 0; i < nfds; i++)
				{
				if ( ! (cma__g_poll_e_p[cur_fd + i].revents & POLLNVAL) )
					{
					cma__open_general(cur_fd + i);
					}
				cma__g_poll_e_p[cur_fd + i ].fd = -1;
				cma__g_poll_e_p[cur_fd + i].events = 0;
				cma__g_poll_e_p[cur_fd + i].revents = 0;
				}
			}

		/*
		*	Increment our "current" fd by the number polled last time.
		*	
		*	Set number of fds to poll next time to 1 if we are iterating,
		*	otherwise set it to the number left to poll (max - current).
		*/
		cur_fd += nfds;
		nfds = (iterating == cma_c_true) ? 1 : (max_fds - cur_fd);
		}	/* while (cur_fd < max_fds) */
    }


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma__io_available:  test all file descriptors to find out if I/O
 *			can be continued.  Any threads blocked on a poll
 *			that can complete are readied, and any threads blocked
 *			for I/O which can now be performed are signaled.
 *
 *  FORMAL PARAMETERS:
 *
 *	function:  I/O type
 *	fd	:  particular file descriptor to be checked
 *	timeout :  desired timeout value (usually zero)
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
 *	cma_c_true:		the selected I/O operation would not block
 *	cma_c_false:	the selected I/O operation would block
 *
 *  SIDE EFFECTS:
 *
 *	None
 */
extern cma_t_boolean
cma__io_available(
	cma__t_io_type	function,
	int				fd,
	struct timeval	*timeout)
    {
    cma_t_integer	nfound = 0, result = 0;
    cma_t_integer	i;
    struct stat		buf;
    cma_t_integer	poll_timeout;
	cma__t_poll_element	*poll_e_p;


    cma__assert_fail ( cma__tac_isset (&cma__g_kernel_critical),
						"cma__io_available:  kernel is unlocked.");
    cma__assert_fail ( (function >= 0) && (function <= cma__c_max_io_type),
						"cma__io_available:  bad value for function.");
    cma__assert_fail (
	    (fd >= 0) && (fd < cma__g_file_num) && (fd < cma__g_mx_file),
	    "cma__io_available:  bad value for file descriptor.");

    /*
    *	Convert timeout to time value that poll() likes.
    */
    if ( timeout != (struct timeval *)NULL )
		{
		poll_timeout = (timeout->tv_sec * 1000) + (timeout->tv_usec / 1000);
		}
	else
		{
		poll_timeout = INFTIM;
		}

 /*
  * Special code for null thread so that we do not miss out on any
  * signals that wakeup sigwaiters.  We repeat some code so that
  * we don't have to pay the overhead of the TRY clause on every
  * call to cma__io_available.
  */

if (cma__get_self_tcb()->kind == cma__c_thkind_null) 
	{
	cma_t_integer breakout = 0;

  	TRY 
		{
   		io_avail_null_thread = 1;

   		if (cma__g_defers_processed == 0)
			{
    			breakout = 1;
			}
   		else 
			{
    			while (result <= 0) 
				{
				result = __nr_poll ( cma__g_poll_e_p, cma__g_file_num, poll_timeout); 

				if (result == 0)  
					{
					breakout = 1;
					break;
					}

				if (result == -1) 
					{
					if (errno == EINTR)
						{
						breakout = 1;
						break;
						}
					/*
			 		* We are do not expect any other errors here, and we do not know
			 		* how to recover should they occur.
			 		*/
					else
						{
						cma__bugcheck("cma__io_available: unexpected poll error");
						}
					} 
				}
   			}
   		io_avail_null_thread = 0;
  		}

  	CATCH (io_avail_exc) 
		{
   		breakout = 1;
   		/* io_avail_null_thread = 0; this should be done when exception is raised */
  		}

  	CATCH_ALL 
		{
   		io_avail_null_thread = 0;
   		RERAISE;
  		}

  	ENDTRY

  	if (breakout) return cma_c_false;
	}
else 	
	{
    	while (result <= 0) 
		{
		result = __nr_poll ( cma__g_poll_e_p, cma__g_file_num, poll_timeout); 

		if (result == 0)  return cma_c_false;	/* No files are ready. */

		if (result == -1) 
			{
			/*
			 * If the error is EINTR then just return false.  This is done
			 * so that the null thread, which blocks the process in the 
			 * select call above, wakes up when a signal comes through, 
			 * and possibly dispatches a now ready thread.  
			 *
			 * Note that our only alternative to returning here would be to 
			 * retry the select, which would reblock the process causing 
			 * latency in the cond_signal_int wake-up.  We can't continue
			 * this routine because the select aborted.
			 *
			 * So, fake it.  Return "no I/O available".  The assumption here
			 * is that at most this introduces an extra timeslice delay into
			 * an I/O latency in a very small window of opportunity, but it
			 * makes the code much simpler.  A worthy tradeoff.
			 */
			if (errno == EINTR)
				{
				return(cma_c_false);
				}
			/*
			 * We are do not expect any other errors here, and we do not know
			 * how to recover should they occur.
			 */
			else
				{
				cma__bugcheck("cma__io_available: unexpected poll error");
				}
			} 
		}
	}

    /*
     * We have found that some of our pending I/O's might now be satisfied.
     */

    /*
     * For each thread blocked on a poll, check if it any of it's 
     * file descriptors are ready for I/O.  If one or more are, set these bits
     * in the thread's masks, remove the thread from the timer and select 
     * queues and ready it.
     */
    if (!cma__queue_empty (&cma__g_select_queue)) 
		{
		cma__t_queue	   		*select_queue;

		/*
		 * The queue node is inside the select block in the tcb, so it's 
		 * address cannot be used as the pointer to the tcb.
		 */
		select_queue = cma__queue_next (&cma__g_select_queue);

		while (select_queue != &cma__g_select_queue) 
			{
			cma__t_int_tcb	   *select_tcb;


			select_tcb = cma__base(select_queue, select.queue, cma__t_int_tcb);
			nfound = 0;

			/*
			*  See if any files that the thread is polling
			*  are ready.
			*/
			i = select_tcb->select.poll_info.nfds;
			poll_e_p = select_tcb->select.poll_info.poll_e_p;
			nfound = 0;

			while ( ( i > 0 ) && ( nfound < result ) )
				{
				cma_t_integer	tmp_fd = poll_e_p->fd;

				if ( tmp_fd > 0 )
					{
					/*
					*	I/O is ready if one of the returned poll bits
					*	matches what the thread asked for, or there
					*	was an error.
					*/
					if ( (poll_e_p->events | (POLLERR|POLLHUP|POLLNVAL)) & 
							cma__g_poll_e_p[tmp_fd].revents )
						{
						/*
						*  Copy the results from the global poll info
						*  into the the thread's poll info.
						*/
						poll_e_p->revents = 
							cma__g_poll_e_p[tmp_fd].revents;
						nfound++;
						}
					}
				i--;
				poll_e_p++;
				}

			if ( nfound )
				{
				select_tcb->select.nfound = nfound;
				cma__int_signal2_int (select_tcb->tswait_cv);
				}
			select_queue = cma__queue_next(select_queue);
			}
		}
    
    /* 
     * Go through the global poll structure looking for files which are
     * ready for I/O.
     *
     * For each "file" which has a thread blocked for I/O on it, signal the
     * condition variable indicating that it is ready.
	 *
	 * We are now waking up threads which are performing an actual
	 * I/O function (as opposed to a poll()).
     */
	i = 0;
	nfound = 0;
	while ( ( i <= cma__g_file_num ) && ( nfound < result ) )
		{
		cma_t_integer found = 0;
		/*
		 * This test is intended to catch instances where the user is 
		 * selecting a file which DECthreads doesn't realize is open.
		 */
		if (cma__g_file[i] != (cma__t_file_obj *)cma_c_null_ptr)
			{
			/*
			*	Skip files which have no I/O pending.
			*/
			if ( (cma__g_poll_e_p[i].fd != -1) && (cma__g_poll_e_p[i].revents) )
				{
				if ( cma__g_poll_e_p[i].revents & 
						(POLLRDNORM|POLLHUP|POLLERR|POLLNVAL) )
					{
					cma__int_signal2_int (
						cma__g_file[i]->io_wait[cma__c_io_read]);
					found++;
					}	
				if ( cma__g_poll_e_p[i].revents & 
						(POLLWRNORM|POLLHUP|POLLERR|POLLNVAL) )
					{
					cma__int_signal2_int (
						cma__g_file[i]->io_wait[cma__c_io_write]);
					found++;
					}
				/*
				*	Note:
				*		For getmsg functions, the fd is not ready if
				*		POLLHUP has occurred;  getmsg*() can function
				*		even though POLLHUP is set.  Therefore, don't
				*		use it to determine if I/O is ready.
				*/
				if ( cma__g_poll_e_p[i].revents & 
						(POLLRDNORM|POLLRDBAND|POLLPRI|POLLERR|POLLNVAL) )
					{
					cma__int_signal2_int (
						cma__g_file[i]->io_wait[cma__c_io_getmsg]);
					found++;
					}	
				if ( cma__g_poll_e_p[i].revents & 
						(POLLPRI|POLLERR|POLLNVAL) )
					{
					cma__int_signal2_int (
						cma__g_file[i]->io_wait[cma__c_io_getmsg_pri]);
					found++;
					}	
				if ( cma__g_poll_e_p[i].revents & 
						(POLLRDBAND|POLLERR|POLLNVAL) )
					{
					cma__int_signal2_int (
						cma__g_file[i]->io_wait[cma__c_io_getmsg_band]);
					found++;
					}	
				if ( cma__g_poll_e_p[i].revents & 
						(POLLWRNORM|POLLHUP|POLLERR|POLLNVAL) )
					{
					cma__int_signal2_int (
						cma__g_file[i]->io_wait[cma__c_io_putmsg]);
					found++;
					}	/* cma__c_io_read */
				if ( cma__g_poll_e_p[i].revents & 
						(POLLWRBAND|POLLHUP|POLLERR|POLLNVAL) )
					{
					cma__int_signal2_int (
						cma__g_file[i]->io_wait[cma__c_io_putpmsg]);
					found++;
					}	
				}
			}
		if ( found != 0 )
			{
			nfound++;
			}
		i++;
		}
    return (cma__poll_io_ready( function, cma__g_poll_e_p[fd]));
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma__poll_io_ready:
 *		Check a poll structure and see if certain I/O type would
 *		succeed.
 *
 *  FORMAL PARAMETERS:
 *
 *	function:  	I/O type
 *	poll_e:		Poll structure to check for I/O readyness
 *
 *  IMPLICIT INPUTS:
 *
 *  IMPLICIT OUTPUTS:
 *
 *  FUNCTION VALUE:
 *
 *	cma_c_true:		the selected I/O operation would not block
 *	cma_c_false:	the selected I/O operation would block
 *
 *  SIDE EFFECTS:
 *
 *	None
 */
cma_t_boolean
cma__poll_io_ready(
	cma__t_io_type	function,
	cma__t_poll_element	poll_e)
{
cma_t_integer	result = 0;

	switch (function)
		{
		case cma__c_io_read:
			{
			result = poll_e.revents & (POLLRDNORM|POLLHUP|POLLERR|POLLNVAL);
			break;
			}	/* cma__c_io_read */

		case cma__c_io_write:
			{
			result = poll_e.revents & (POLLWRNORM|POLLHUP|POLLERR|POLLNVAL);
			break;
			}	/* cma__c_io_write */

		/*
		*	Note:
		*		For getmsg functions, the fd is not ready if
		*		POLLHUP has occurred;  getmsg*() can function
		*		even though POLLHUP is set.  Therefore, don't
		*		use it to determine if I/O is ready.
		*/
		case cma__c_io_getmsg:
			{
			result = poll_e.revents & 
						(POLLRDNORM|POLLRDBAND|POLLPRI|POLLERR|POLLNVAL);
			break;
			}	/* cma__c_io_getmsg */

		case cma__c_io_getmsg_pri:
			{
			result = poll_e.revents & (POLLPRI|POLLERR|POLLNVAL);
			break;
			}	/* cma__c_io_getmsg_pri */

		case cma__c_io_getmsg_band:
			{
			result = poll_e.revents & (POLLRDBAND|POLLERR|POLLNVAL);
			break;
			}	/* cma__c_io_getmsg_band */

		case cma__c_io_putmsg:
			{
			result = poll_e.revents & (POLLWRNORM|POLLHUP|POLLERR|POLLNVAL);
			break;
			}	/* cma__c_io_putmsg */

		case cma__c_io_putpmsg:
			{
			result = poll_e.revents & (POLLWRBAND|POLLHUP|POLLERR|POLLNVAL);
			break;
			}	/* cma__c_io_putpmsg */

		case cma__c_io_pollin:
			{
			result = poll_e.revents & (POLLIN|POLLERR|POLLNVAL);
			break;
			}	/* cma__c_io_pollin */

		default:
			{
			cma__bugcheck("cma__poll_io_ready: invalid io function type");
			break;
			} 	/* default */

		}	/* switch (function) */
	
	return( (result == 0) ? cma_c_false : cma_c_true );

}	/* cma__poll_io_ready */

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma__poll_set_gio_events:
 *		Set the appropriate global I/O database information 
 *		for a given set of poll events, for a particular file
 *		descriptor.
 *
 *		It is assumed that the thread has reserved the "file" being
 *		accessed.  It is also assumed that the thread has exclusive
 *		access to the global poll information database.
 *
 *		Set the global poll information for the file descriptor
 *		passed in.  In addition to the global poll information, it
 *		is necessary to increment the appropriate "waiter" flags in the
 *		global "file" information for the file descriptor.
 *		
 *		While the "waiter" flags primarily indicate how many
 *		threads are performing I/O, they are also used
 *		to indicate that a thread is "using" certain bits in the
 *		global poll information database.  This ensures that
 *		the global poll bits do not get "turned off" when all other
 *		threads, which are performing "real" I/O (as opposed to
 *		a poll()) are finished.
 *
 *  FORMAL PARAMETERS:
 *
 *	fd:			File descriptor to set CMA database information for
 *	events:		poll() events for file descriptor
 *
 *  IMPLICIT INPUTS:
 *
 *	The global poll() information.  The "file" information for 'fd'.
 *
 *  IMPLICIT OUTPUTS:
 *	
 *  FUNCTION VALUE:
 *
 *	None
 */
extern void
cma__poll_set_gio_events(
	cma_t_integer	fd,
	cma_t_integer	events)
{
	
	/*
	*	Set up the global poll structure.
	*/

	cma__g_poll_e_p[fd].fd = fd;

	/*
	*	Make sure that we only set valid events.
	*/
	cma__g_poll_e_p[fd].events |= (events & (POLLIN|
											POLLRDNORM|
											POLLRDBAND|
											POLLPRI|
											POLLOUT|
											POLLWRNORM|
											POLLWRBAND));

	/*
	*	Set up the waiters in the per-file info structure.
	*/
	if ( events & POLLOUT )
		{
		cma__g_file[fd]->waiters[(int)cma__c_io_putmsg]++;
		}
	if ( events & POLLWRNORM )
		{
		cma__g_file[fd]->waiters[(int)cma__c_io_write]++;
		}
	if ( events & POLLWRBAND )
		{
		cma__g_file[fd]->waiters[(int)cma__c_io_putpmsg]++;
		}
	if ( events & POLLPRI )
		{
		cma__g_file[fd]->waiters[(int)cma__c_io_getmsg_pri]++;
		}
	if ( events & POLLRDBAND )
		{
		cma__g_file[fd]->waiters[(int)cma__c_io_getmsg_band]++;
		}
	if ( events & POLLRDNORM )
		{
		cma__g_file[fd]->waiters[(int)cma__c_io_read]++;
		}
	if ( events & POLLIN )
		{
		cma__g_file[fd]->waiters[(int)cma__c_io_pollin]++;
		}
}	/* cma__poll_set_gio */

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma__poll_clear_gio_events:
 *		Clear the appropriate global I/O database information 
 *		for a given set of poll events, for a particular file
 *		descriptor.
 *
 *		It is assumed that the thread has reserved the "file" being
 *		accessed.  It is also assumed that the thread has exclusive
 *		access to the global poll information database.
 *
 *		This function decrements the appropriate "waiter" flags in the
 *		global "file" information for the file descriptor.  It also 
 *		checks to see if each bit needs to be cleared from the
 *		global poll information for this file descriptor.
 *
 *  FORMAL PARAMETERS:
 *
 *	fd:			File descriptor to set CMA database information for
 *	events:		poll() events for file descriptor
 *
 *  IMPLICIT INPUTS:
 *
 *	The global poll() information.  The "file" information for 'fd'.
 *
 *  IMPLICIT OUTPUTS:
 *	
 *  FUNCTION VALUE:
 *
 *	None
 */
extern void
cma__poll_clear_gio_events(
	cma_t_integer	fd,
	cma_t_integer	events)
{
	
	if ( events & POLLOUT )
		{
		cma__g_file[fd]->waiters[(int)cma__c_io_putmsg]--;
		cma__poll_gio_reset( fd, POLLOUT );
		}
	if ( events & POLLWRNORM )
		{
		cma__g_file[fd]->waiters[(int)cma__c_io_write]--;
		cma__poll_gio_reset( fd, POLLWRNORM );
		}
	if ( events & POLLWRBAND )
		{
		cma__g_file[fd]->waiters[(int)cma__c_io_putpmsg]--;
		cma__poll_gio_reset( fd, POLLWRBAND );
		}
	if ( events & POLLPRI )
		{
		cma__g_file[fd]->waiters[(int)cma__c_io_getmsg_pri]--;
		cma__poll_gio_reset( fd, POLLPRI );
		}
	if ( events & POLLRDBAND )
		{
		cma__g_file[fd]->waiters[(int)cma__c_io_getmsg_band]--;
		cma__poll_gio_reset( fd, POLLRDBAND );
		}
	if ( events & POLLRDNORM )
		{
		cma__g_file[fd]->waiters[(int)cma__c_io_read]--;
		cma__poll_gio_reset( fd, POLLRDNORM );
		}
	if ( events & POLLIN )
		{
		cma__g_file[fd]->waiters[(int)cma__c_io_pollin]--;
		cma__poll_gio_reset( fd, POLLIN );
		}

	/*
	*	Make sure everything is in a consistent state
	*/
	cma__assert_fail( 
		(((cma__g_poll_e_p[fd].events == 0) &&
			((cma__g_file[fd]->waiters[(int)cma__c_io_read] == 0) &&
			 (cma__g_file[fd]->waiters[(int)cma__c_io_write] == 0) &&
			 (cma__g_file[fd]->waiters[(int)cma__c_io_getmsg] == 0) &&
			 (cma__g_file[fd]->waiters[(int)cma__c_io_getmsg_pri] == 0) &&
			 (cma__g_file[fd]->waiters[(int)cma__c_io_getmsg_band] == 0) &&
			 (cma__g_file[fd]->waiters[(int)cma__c_io_putmsg] == 0) &&
			 (cma__g_file[fd]->waiters[(int)cma__c_io_pollin] == 0)))
		 ||
		  ((cma__g_poll_e_p[fd].events != 0) &&
			((cma__g_file[fd]->waiters[(int)cma__c_io_read] != 0) ||
			 (cma__g_file[fd]->waiters[(int)cma__c_io_write] != 0) ||
			 (cma__g_file[fd]->waiters[(int)cma__c_io_getmsg] != 0) ||
			 (cma__g_file[fd]->waiters[(int)cma__c_io_getmsg_pri] != 0) ||
			 (cma__g_file[fd]->waiters[(int)cma__c_io_getmsg_band] != 0) ||
			 (cma__g_file[fd]->waiters[(int)cma__c_io_putmsg] != 0) ||
			 (cma__g_file[fd]->waiters[(int)cma__c_io_pollin] != 0)))),
	  "cma__poll_clear_gio_events:  inconsistent global I/O state");

	cma__assert_fail( 
		((cma__g_file[fd]->waiters[(int)cma__c_io_read] >= 0) &&
			 (cma__g_file[fd]->waiters[(int)cma__c_io_write] >= 0) &&
			 (cma__g_file[fd]->waiters[(int)cma__c_io_getmsg] >= 0) &&
			 (cma__g_file[fd]->waiters[(int)cma__c_io_getmsg_pri] >= 0) &&
			 (cma__g_file[fd]->waiters[(int)cma__c_io_getmsg_band] >= 0) &&
			 (cma__g_file[fd]->waiters[(int)cma__c_io_putmsg] >= 0) &&
			 (cma__g_file[fd]->waiters[(int)cma__c_io_pollin] >= 0)),
	  "cma__poll_clear_gio_events:  waiter count < 0");

	/*
	*	If no more events for this file, then set 'fd' to -1 so that
	*	poll() does not even look at this fd.
	*/
	if ( cma__g_poll_e_p[fd].events == 0 )
		{
		cma__g_poll_e_p[fd].fd = -1;
		}

}	/* cma__poll_clear_gio_events */

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma__poll_set_gio_function:
 *		Set the appropriate global I/O database information 
 *		for a given an I/O function type, for a particular file
 *		descriptor.
 *
 *		It is assumed that the thread has reserved the "file" being
 *		accessed.  It is also assumed that the thread has exclusive
 *		access to the global poll information database.
 *
 *		Increment the appropriate waiter flag for this I/O type for
 *		the "file" being accessed.  In addition, set the appropriate
 *		poll bits for this file descriptor.
 *
 *  FORMAL PARAMETERS:
 *
 *	fd:			File descriptor to set CMA database information for
 *	events:		poll() events for file descriptor
 *
 *  IMPLICIT INPUTS:
 *
 *	The global poll() information.  The "file" information for 'fd'.
 *
 *  IMPLICIT OUTPUTS:
 *	
 *  FUNCTION VALUE:
 *
 *	None
 */
extern void
cma__poll_set_gio_function(
	cma_t_integer	fd,
	cma__t_io_type	function)
{
cma_t_integer	bits;
	
	cma__g_file[fd]->waiters[(int)function]++;
	switch (function)
		{
		case cma__c_io_read:
			{
			bits = POLLRDNORM;
			break;
			}	/* cma__c_io_read */

		case cma__c_io_write:
			{
			bits = POLLWRNORM;
			break;
			}	/* cma__c_io_write */

		case cma__c_io_getmsg:
			{
			bits = POLLRDNORM|POLLRDBAND|POLLPRI;
			break;
			}	/* cma__c_io_getmsg */

		case cma__c_io_getmsg_pri:
			{
			bits = POLLPRI;
			break;
			}	/* cma__c_io_getmsg_pri */

		case cma__c_io_getmsg_band:
			{
			bits = POLLRDBAND;
			break;
			}	/* cma__c_io_getmsg_band */

		case cma__c_io_putmsg:
			{
			bits = POLLWRNORM;
			break;
			}	/* cma__c_io_putmsg */

		case cma__c_io_putpmsg:
			{
			bits = POLLWRBAND;
			break;
			}	/* cma__c_io_putpmsg */

		default:
			{
			cma__bugcheck("cma__poll_io_ready: invalid io function type");
			} 	/* default */
		}	/* switch (function) */
	
	cma__g_poll_e_p[fd].events |= bits;
	cma__g_poll_e_p[fd].fd = fd;

}	/* cma__poll_set_gio_function */

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma__poll_clear_gio_function:
 *		Celar the appropriate global I/O database information 
 *		for a given an I/O function type, for a particular file
 *		descriptor.
 *
 *		It is assumed that the thread has reserved the "file" being
 *		accessed.  It is also assumed that the thread has exclusive
 *		access to the global poll information database.
 *
 *		Decrement the appropriate waiter flag for this I/O type for
 *		the "file" being accessed.  In addition, check to see if
 *		the poll bits used for this I/O type can be cleared.
 *
 *  FORMAL PARAMETERS:
 *
 *	fd:			File descriptor to set CMA database information for
 *	events:		poll() events for file descriptor
 *
 *  IMPLICIT INPUTS:
 *
 *	The global poll() information.  The "file" information for 'fd'.
 *
 *  IMPLICIT OUTPUTS:
 *	
 *  FUNCTION VALUE:
 *
 *	None
 */
extern void
cma__poll_clear_gio_function(
	cma_t_integer	fd,
	cma__t_io_type	function)
{
	cma__g_file[fd]->waiters[(int)function]--;
	switch (function)
		{
		case cma__c_io_write:
		case cma__c_io_putmsg:
			{
			cma__poll_gio_reset( fd, POLLWRNORM );
			break;
			}

		case cma__c_io_read:
			{
			cma__poll_gio_reset( fd, POLLRDNORM );
			break;
			}

		case cma__c_io_putpmsg:
			{
			cma__poll_gio_reset( fd, POLLWRBAND );
			break;
			}
		
		case cma__c_io_getmsg_pri:
			{
			cma__poll_gio_reset( fd, POLLPRI );
			break;
			}

		case cma__c_io_getmsg_band:
			{
			cma__poll_gio_reset( fd, POLLRDBAND );
			break;
			}
		
		case cma__c_io_getmsg:
			{
			cma__poll_gio_reset( fd, POLLRDNORM|POLLRDBAND|POLLPRI );
			break;
			}
		}	/* switch (function) */

	/*
	*	Make sure things are in a consistent state.
	*/

	cma__assert_fail( 
		(((cma__g_poll_e_p[fd].events == 0) &&
			((cma__g_file[fd]->waiters[(int)cma__c_io_read] == 0) &&
			 (cma__g_file[fd]->waiters[(int)cma__c_io_write] == 0) &&
			 (cma__g_file[fd]->waiters[(int)cma__c_io_getmsg] == 0) &&
			 (cma__g_file[fd]->waiters[(int)cma__c_io_getmsg_pri] == 0) &&
			 (cma__g_file[fd]->waiters[(int)cma__c_io_getmsg_band] == 0) &&
			 (cma__g_file[fd]->waiters[(int)cma__c_io_putmsg] == 0) &&
			 (cma__g_file[fd]->waiters[(int)cma__c_io_pollin] == 0)))
		 ||
		  ((cma__g_poll_e_p[fd].events != 0) &&
			((cma__g_file[fd]->waiters[(int)cma__c_io_read] != 0) ||
			 (cma__g_file[fd]->waiters[(int)cma__c_io_write] != 0) ||
			 (cma__g_file[fd]->waiters[(int)cma__c_io_getmsg] != 0) ||
			 (cma__g_file[fd]->waiters[(int)cma__c_io_getmsg_pri] != 0) ||
			 (cma__g_file[fd]->waiters[(int)cma__c_io_getmsg_band] != 0) ||
			 (cma__g_file[fd]->waiters[(int)cma__c_io_putmsg] != 0) ||
			 (cma__g_file[fd]->waiters[(int)cma__c_io_pollin] != 0)))),
	  "cma__poll_clear_gio_function:  inconsistent global I/O state");

	cma__assert_fail( 
		((cma__g_file[fd]->waiters[(int)cma__c_io_read] >= 0) &&
			 (cma__g_file[fd]->waiters[(int)cma__c_io_write] >= 0) &&
			 (cma__g_file[fd]->waiters[(int)cma__c_io_getmsg] >= 0) &&
			 (cma__g_file[fd]->waiters[(int)cma__c_io_getmsg_pri] >= 0) &&
			 (cma__g_file[fd]->waiters[(int)cma__c_io_getmsg_band] >= 0) &&
			 (cma__g_file[fd]->waiters[(int)cma__c_io_putmsg] >= 0) &&
			 (cma__g_file[fd]->waiters[(int)cma__c_io_pollin] >= 0)),
	  "cma__poll_clear_gio_function:  waiter count < 0");

	/*
	*	If no more events for this file, then set 'fd' to -1 so that
	*	poll() does not even look at this fd.
	*/
	if ( cma__g_poll_e_p[fd].events == 0 )
		{
		cma__g_poll_e_p[fd].fd = -1;
		}
					 
}	/* cma__poll_set_gio */

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma__poll_gio_reset:
 *		Reset the poll bits passed in for this file descriptor in
 *		the global poll database, if appropriate.
 *
 *		It is assumed that the thread has reserved the "file" being
 *		accessed.  It is also assumed that the thread has exclusive
 *		access to the global poll information database.
 *
 *		For each poll event set, check to see if any threads are
 *		using it.  If not, reset the bit.   Each poll bit has
 *		a set of I/O function types which result in its being set.
 *		If there are any "waiters" for I/O types which result in
 *		the poll bit being set, then leave the poll bit set.
 *		Otherwise, set the bit to zero.
 *
 *  FORMAL PARAMETERS:
 *
 *	fd:			File descriptor to set CMA database information for
 *	events:		poll() events for file descriptor
 *
 *  IMPLICIT INPUTS:
 *
 *	The global poll() information.  The "file" information for 'fd'.
 *
 *  IMPLICIT OUTPUTS:
 *	
 *  FUNCTION VALUE:
 *
 *	None
 */
extern void
cma__poll_gio_reset(
	cma_t_integer	fd,
	cma_t_integer	event)
{
	
	if ( event & POLLWRNORM )
		{
		if ( (cma__g_file[fd]->waiters[(int)cma__c_io_putmsg] == 0) &&
			 (cma__g_file[fd]->waiters[(int)cma__c_io_write] == 0) )
			{
			cma__g_poll_e_p[fd].events &= (~POLLWRNORM);
			}
		}

	if ( event & POLLRDNORM )
		{
		if ( (cma__g_file[fd]->waiters[(int)cma__c_io_read] == 0) &&
			 (cma__g_file[fd]->waiters[(int)cma__c_io_getmsg] == 0))
			{
			cma__g_poll_e_p[fd].events &= (~POLLRDNORM);
			}
		}

	if ( event & POLLWRBAND )
		{
		if ( cma__g_file[fd]->waiters[(int)cma__c_io_putpmsg] == 0 )
			{
			cma__g_poll_e_p[fd].events &= (~POLLWRBAND);
			}
		}
		
	if ( event & POLLPRI )
		{
		if ( (cma__g_file[fd]->waiters[(int)cma__c_io_getmsg_pri] == 0) &&
			 (cma__g_file[fd]->waiters[(int)cma__c_io_getmsg] == 0))
			{
			cma__g_poll_e_p[fd].events &= (~POLLPRI);
			}
		}

	if ( event & POLLRDBAND )
		{
		if ( (cma__g_file[fd]->waiters[(int)cma__c_io_getmsg_band] == 0) &&
			 (cma__g_file[fd]->waiters[(int)cma__c_io_getmsg] == 0))
			{
			cma__g_poll_e_p[fd].events &= (~POLLRDBAND);
			}
		}

	if ( event & POLLIN )
		{
		if (cma__g_file[fd]->waiters[(int)cma__c_io_pollin] == 0)
			{
			cma__g_poll_e_p[fd].events &= (~POLLIN);
			}
		}
}	/* cma__poll_set_gio */

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma__io_epilog:  CMA I/O service epilog routine 
 *
 *  FORMAL PARAMETERS:
 *
 *	io_type		    Type of I/O  
 *	file_descriptor	    File descritor
 *
 *  IMPLICIT INPUTS:
 *
 *	All other file descriptors
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
 *	NOTE:  we assume that a previous call to cma__fd_reserve has locked 
 *		the cma__g_file[fd]->mutex, and a subsequent call to 
 *		cma__fd_unreserve will unlock it.
 */

extern void
cma__io_epilog(
	cma__t_io_type	function,
	int		fd)
    {
    cma_t_boolean do_check;


    cma__assert_not_kernel ();
    cma__assert_fail (
	    (((int)function >= 0) && ((int)function <= cma__c_max_io_type)),
	    "cma__io_epilog:  bad value for function.");
    cma__assert_fail (
	    (fd >= 0) && (fd < cma__g_file_num) && (fd < cma__g_mx_file),
	    "cma__io_epilog:  bad value for file descriptor.");
    cma__assert_warn (
	    cma__is_open(fd),
	    "cma__io_epilog:  file is not open.");

	cma__int_lock (cma__g_io_data_mutex);
	cma__poll_clear_gio_function( fd, function );
	cma__int_unlock (cma__g_io_data_mutex);
	if ( cma__g_file[fd]->waiters[(int)function] == 0 )
		{
        do_check = cma_c_false;
	}
    else
	do_check = cma__is_open(fd);


    if (do_check)
	cma__int_signal (cma__g_file[fd]->io_wait[(int)function]);

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma__io_prolog:  CMA I/O service prolog routine
 *
 *	This routine sets the global I/O data structures to reflect the fact
 *	that a thread is now attempting (about to block for) I/O.
 *
 *  FORMAL PARAMETERS:
 *
 *	io_type		    Type of I/O  
 *	file_descriptor	    File descriptor 
 *
 *  IMPLICIT INPUTS:
 *
 *	All other file descriptors
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
 *	NOTE:  we assume that a previous call to cma__fd_reserve has locked 
 *		the cma__g_file[fd]->mutex, and a subsequent call to 
 *		cma__fd_unreserve will unlock it.
 */
extern void
cma__io_prolog(
	cma__t_io_type	function,
	int		fd)
    {
    cma__assert_fail (
	    (((int)function >= 0) && ((int)function <= cma__c_max_io_type)),
	    "cma__io_prolog:  bad value for function.");

    cma__assert_fail (
	    (fd >= 0) && (fd < cma__g_file_num) && (fd < cma__g_mx_file),
	    "cma__io_prolog:  bad value for file descriptor.");
    cma__assert_fail (
	    cma__is_open(fd),
	    "cma__io_prolog:  file not open.");

    cma__int_lock (cma__g_io_data_mutex);
	cma__poll_set_gio_function( fd, function );
    cma__int_unlock (cma__g_io_data_mutex);

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma__io_wait:  CMA I/O service wait routine
 *
 *	This routine causes the current thread to block (alertably, by waiting
 *	at a condition variable) until it's I/O can be accomplished without
 *	blocking the process.
 *
 *  FORMAL PARAMETERS:
 *
 *	io_type		    Type of I/O  
 *	file_descriptor	    File descriptor 
 *
 *  IMPLICIT INPUTS:
 *
 *	All other file descriptors
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
 *	The current thread may block.
 *
 *	NOTE:  we assume that a previous call to cma__fd_reserve has locked 
 *		the cma__g_file[fd]->mutex, and a subsequent call to 
 *		cma__fd_unreserve will unlock it.
 */
extern void
cma__io_wait(
	cma__t_io_type	function,
	int		fd)
    {
    cma__assert_not_kernel ();
    cma__assert_fail (
	    (((int)function >= 0) && ((int)function <= cma__c_max_io_type)),
	    "cma__io_wait:  bad value for function.");
    cma__assert_fail (
	    (fd >= 0) && (fd < cma__g_file_num) && (fd < cma__g_mx_file),
	    "cma__io_wait:  bad value for file descriptor.");
    cma__assert_fail (
	    cma__is_open(fd),
	    "cma__io_wait:  file not open.");

    /*
     * Do the following loop exactly once for non-blocking mode files.  For
     * blocking mode files, loop until there is an I/O available.
     */
    do  {
	if (!cma__g_file[fd]->non_blocking) {
	    cma_t_boolean		io_available;
	    static struct timeval	polling_timeout = {0, 0};


	    cma__enter_kernel ();
	    io_available = cma__io_available (
		    function, 
		    fd, 
		    &polling_timeout);
	    cma__exit_kernel ();

	    if (io_available) break;
	    }

	cma__int_wait (
		cma__g_file[fd]->io_wait[(int)function],
		cma__g_file[fd]->mutex);

	} while (!cma__g_file[fd]->non_blocking);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma__open_general
 *
 *	The common "file" open function:  this function opens a "file" from
 *	the point of view of the CMA I/O database.
 *
 *  FORMAL PARAMETERS:
 *
 *	fd  -  a Unix file descriptor, a number.
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
 *	None
 *
 *  SIDE EFFECTS:
 *
 *	None
 */
extern void
cma__open_general(int	fd)
    {
    int		    	fo;
    struct stat	    statbuf;
	int				uflags;

    if ((fd < 0) || (fd >= cma__g_mx_file))  cma__error (cma_s_badparam);

    cma__assert_warn (
	    !cma__is_open(fd),
	    "cma__open_general: file descriptor is already open");

    if (cma__is_open(fd)) return;

    cma__trace ((cma__c_trc_io, "(open_general) opening fd %d", fd));

    /*
     * For each file object we encounter, we use the file lock to check
     * whether the object's mutex and cvs exist.  If not, we create them.
     * Then we release the file lock, and lock the file object's mutex to
     * check whether it's currently in use.  As soon as we find an object
     * that's not in use, we exit.
     * 
     * Note, we expect to exit the loop with the selected file object's mutex 
     * locked.
     */
    for (fo = 0; fo < cma__g_mx_file; fo++) {

	if (cma__g_file_obj[fo].mutex == (cma__t_int_mutex *)0) {
	    /*
	     * If this thread thinks that the mutex pointer is still
	     * uninitialized, take out the file object lock and check
	     * again to be sure that caches are sync-ed and some other thread
	     * hasn't snuck in and grabbed it.
	     */
	    cma__int_lock (cma___g_file_lock);

	    if (cma__g_file_obj[fo].mutex == (cma__t_int_mutex *)0) {
		cma__t_int_cv		*read_cv, *write_cv;
		cma__t_int_cv		*getmsg_cv, *getmsg_pri_cv, *getmsg_band_cv;
		cma__t_int_cv		*putmsg_cv, *putpmsg_cv;
		cma__t_int_mutex	*mu;
		cma_t_boolean		ok;


		mu = cma__get_mutex (&cma__g_def_attr);

		if ((cma_t_address)mu == cma_c_null_ptr)
		    ok = cma_c_false;
		else
		    ok = cma_c_true;

		if (ok) {
		    read_cv = cma__get_cv (&cma__g_def_attr);

		    if ((cma_t_address)read_cv == cma_c_null_ptr)
			ok = cma_c_false;

		    }
		else
		    read_cv = (cma__t_int_cv *)cma_c_null_ptr;

		if (ok) {
		    write_cv = cma__get_cv (&cma__g_def_attr);

		    if ((cma_t_address)write_cv == cma_c_null_ptr)
			ok = cma_c_false;

		    }
		if (ok) {
		    getmsg_cv = cma__get_cv (&cma__g_def_attr);

		    if ((cma_t_address)getmsg_cv == cma_c_null_ptr)
			ok = cma_c_false;

		    }
		if (ok) {
		    getmsg_pri_cv = cma__get_cv (&cma__g_def_attr);

		    if ((cma_t_address)getmsg_pri_cv == cma_c_null_ptr)
			ok = cma_c_false;

		    }
		if (ok) {
		    getmsg_band_cv = cma__get_cv (&cma__g_def_attr);

		    if ((cma_t_address)getmsg_band_cv == cma_c_null_ptr)
			ok = cma_c_false;

		    }
		if (ok) {
		    putmsg_cv = cma__get_cv (&cma__g_def_attr);

		    if ((cma_t_address)putmsg_cv == cma_c_null_ptr)
			ok = cma_c_false;

		    }
		if (ok) {
		    putpmsg_cv = cma__get_cv (&cma__g_def_attr);

		    if ((cma_t_address)putpmsg_cv == cma_c_null_ptr)
			ok = cma_c_false;

		    }


		if (ok) {
		    cma__obj_set_name (mu, "file mutex");
		    cma__obj_set_name (read_cv, "file read cv");
		    cma__obj_set_name (write_cv, "file write cv");
		    cma__obj_set_name (getmsg_cv, "file getmsg cv");
		    cma__obj_set_name (getmsg_pri_cv, "file getmsg_pri cv");
		    cma__obj_set_name (getmsg_band_cv, "file getmsg_band cv");
		    cma__obj_set_name (putmsg_cv, "file putmsg cv");
		    cma__obj_set_name (putpmsg_cv, "file putpmsg cv");
		    cma__g_file_obj[fo].mutex = mu;
		    cma__g_file_obj[fo].io_wait[(int)cma__c_io_read] = read_cv;
		    cma__g_file_obj[fo].io_wait[(int)cma__c_io_write] = write_cv;
		    cma__g_file_obj[fo].io_wait[(int)cma__c_io_getmsg] = getmsg_cv;
		    cma__g_file_obj[fo].io_wait[(int)cma__c_io_getmsg_pri] = getmsg_pri_cv;
		    cma__g_file_obj[fo].io_wait[(int)cma__c_io_getmsg_band] = getmsg_band_cv;
		    cma__g_file_obj[fo].io_wait[(int)cma__c_io_putmsg] = putmsg_cv;
		    cma__g_file_obj[fo].io_wait[(int)cma__c_io_putpmsg] = putpmsg_cv;
		    }
		else {
		    /*
		     * If one of the object creations failed, we don't have a
		     * write_cv (either it failed, or we didn't try because a
		     * previous creation failed); check whether we got one or
		     * both of the other objects, and free them if necessary.
		     * Then raise an exception to report the error.
		     */

		    if ((cma_t_address)mu != cma_c_null_ptr)
			cma__free_mutex (mu);

		    if ((cma_t_address)read_cv != cma_c_null_ptr)
			cma__free_cv (read_cv);

		    if ((cma_t_address)write_cv != cma_c_null_ptr)
			cma__free_cv (write_cv);

		    if ((cma_t_address)getmsg_cv != cma_c_null_ptr)
			cma__free_cv (getmsg_cv);

		    if ((cma_t_address)getmsg_pri_cv != cma_c_null_ptr)
			cma__free_cv (getmsg_pri_cv);

		    if ((cma_t_address)getmsg_band_cv != cma_c_null_ptr)
			cma__free_cv (getmsg_band_cv);

		    if ((cma_t_address)putmsg_cv != cma_c_null_ptr)
			cma__free_cv (putmsg_cv);

		    if ((cma_t_address)putpmsg_cv != cma_c_null_ptr)
			cma__free_cv (putpmsg_cv);

		    cma__int_unlock (cma___g_file_lock);
		    cma__error (exc_s_insfmem);
		    }

		}
		cma__int_unlock (cma___g_file_lock);

	    }

	cma__int_lock (cma__g_file_obj[fo].mutex);

	if (cma__g_file_obj[fo].ref_count == 0)
	    break;			/* Leave with mutex locked if free */

	cma__int_unlock (cma__g_file_obj[fo].mutex);
	}

    if (fo >= cma__g_mx_file)  cma__bugcheck("cma__open_general:  fd too large");

    cma__trace ((
	    cma__c_trc_io,
	    "(open_general) file obj %d used for fd %d",
	    fo, fd));
    cma__g_file_obj[fo].ref_count = 1;

    cma__int_lock (cma__g_io_data_mutex);
    cma__g_file[fd] = &cma__g_file_obj[fo];
    if (fd >= cma__g_file_num)  cma__g_file_num = fd + 1;
    cma__int_unlock (cma__g_io_data_mutex);

    /* 
     * At this point, the file used to be set non-blocking.  We now
     * defer this to the first time I/O is attempted on the file.
     * the "set_non_blocking" flag is initialized to false so we
     * know to do it later.
     */

	cma__g_file_obj[fo].set_non_blocking = cma_c_false;

	/*
	*	Figure out what flags are set on the file descriptor
	*	already.  
	*/
	if ( (uflags = __nr_fcntl(fd, F_GETFL)) != -1 )
		{
		cma__set_user_nonblock_flags(fd, uflags);
		}
		

    if (fstat (fd, &statbuf) == -1) {
	switch (errno)	{
	    /*
	     * The specified file is not open.
	     */
	    case EBADF: {
		cma__int_lock (cma__g_io_data_mutex);
		cma__g_file[fd] = (cma__t_file_obj *)cma_c_null_ptr;
		cma__int_unlock (cma__g_io_data_mutex);

		cma__g_file_obj[fo].ref_count = 0;
		cma__int_unlock (cma__g_file_obj[fo].mutex);

		cma__trace ((
			cma__c_trc_io,
			"(open_general) fd %d not open (EBADF)",
			fd));

		cma__error (cma_s_badparam);

		break;
		}

	    /*
	     * We can't get any info about this file, so try to recover by
	     * assuming its a "regular" file.  (A conservative approach.)
	     */
	    case EIO:
	    case ETIMEDOUT: {
		cma__trace ((
			cma__c_trc_io,
			"(open_general) fd %d assumed regular (EIO, ETIMEDOUT)",
			fd));
		cma__g_file_obj[fo].non_blocking = cma_c_false;
		break;
		}

	    /*
	     * According to the documentation, this means the file is a socket.
	     */
	    case EOPNOTSUPP:
		{
		cma__g_file_obj[fo].non_blocking = cma_c_true;
		cma__trace ((
			cma__c_trc_io,
			"(open_general) fd %d is a socket (EOPNOTSUPP)",
			fd));
		break;
		}

            case ENOENT: {
                /*
                 * This can apparently happen if the file is a socket 
                 * and the network connection is lost.
                 */
                cma__int_lock (cma__g_io_data_mutex);
                cma__g_file[fd] = (cma__t_file_obj *)cma_c_null_ptr;
                cma__int_unlock (cma__g_io_data_mutex);

                cma__g_file_obj[fo].ref_count = 0;
                cma__int_unlock (cma__g_file_obj[fo].mutex);

                cma__trace ((
                        cma__c_trc_io,
                        "(open_general) fd %d not open (ENOENT)",
                        fd));

                cma__error (cma_s_badparam);
                break;
            }

	    /*
	     * Die on all other errors.
	     */
	    default: {
		cma__trace ((
			cma__c_trc_io,
			"(open_general) fstat on fd %d returned errno %d",
			fd, errno));
		cma__bugcheck ("cma__open_general: unexpected fstat error");
		break;
		}
	    }
	}
    else {
		int format = statbuf.st_mode & S_IFMT;


	/*
	*	On SVR4 there may be some devices which
	*	do not support polling (and therefore select()).  Therefore
	*	we cannot use the central CMA waiting mechanism because it
	*	is based upon select()/poll() and cannot handle errors (nor
	*	would we want it to, since it make the code more complex and
	*	prehaps less efficient).
	*
	*	Therefore, check to see if the file
	*	supports polling.  
	*
	*	This may mean that I/O on this type of device may block
	*	the process, but that is better than a core dump.
	*/

		cma__t_poll_element	poll_e;

		cma__g_file_obj[fo].pollable = cma_c_true;

		poll_e.events = POLLIN;
		poll_e.fd = fd;

		if ( __nr_poll ( &poll_e, 1, 0 ) == -1 )
			{
			if ( errno != ENODEV ) 
				{
				cma__trace ((
					cma__c_trc_io,
					"(open_general) select on fd %d returned %d",
					fo,errno));
				cma__bugcheck("cma__open_general:  unexpected poll error");
				}
			cma__g_file_obj[fo].pollable = cma_c_false;

			/*
			*	This is to make sure that the I/O wrappers do
			*	not try to set this file to non-blocking mode.
			*/
			cma__g_file_obj[fo].set_non_blocking = cma_c_true;
			cma__trace ((
				cma__c_trc_io,
				"(open_general) poll on fd %d returned ENODEV",
				fd));
			}
		else
			{


	switch (format) {
	    /*
	     * For the following types of files, setting non-blocking mode is
	     * probably somewhere between unwarranted and unpleasant, so don't.
	     */
	    case S_IFCHR:
	    case S_IFBLK:
	    case S_IFDIR:
	    case S_IFREG:
	    case S_IFLNK: {
		cma__g_file_obj[fo].non_blocking = cma_c_false;
		break;
		}

	    /*
	     * For the following types of files, setting non-blocking mode is
	     * just the right thing to do, so do it.
	     */
	    case 0:		/* Yet another SOCKET possibility... */
	    case S_IFSOCK:
	    case S_IFIFO: {
		cma__g_file_obj[fo].non_blocking = cma_c_true;
		break;
		}

	    default: {
		cma__bugcheck ("cma__open_general: unexpected file type");
		break;
		}
	    } /* switch (format) */

		    }	/* if selectable() */

	}

    cma__int_unlock (cma__g_file_obj[fo].mutex);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma__set_user_nonblock_flags
 *
 *	Given status flags (as set by the user) for a file descriptor, 
 *	set the non-blocking status of this file descriptor.
 *
 *  FORMAL PARAMETERS:
 *
 *	fd  -  a Unix file descriptor, a number.
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
 *	None
 *
 *  SIDE EFFECTS:
 *
 *	None
 */
extern void
cma__set_user_nonblock_flags(
	cma_t_integer	fd,
	cma_t_integer	flag)
    {

#ifdef O_NDELAY
		cma__g_file[fd]->user_fl.o_ndelay =   ((flag & O_NDELAY) != 0);
#endif
#ifdef _CMA_FNDELAY_
		cma__g_file[fd]->user_fl.fndelay =    ((flag & FNDELAY) != 0);
#endif
#ifdef O_NONBLOCK
		cma__g_file[fd]->user_fl.o_nonblock = ((flag & O_NONBLOCK) != 0);
#endif
#ifdef _CMA_FNBLOCK_
		cma__g_file[fd]->user_fl.fnblock =    ((flag & FNBLOCK) != 0);
#endif

		cma__g_file[fd]->user_fl.user_non_blocking = 
#ifdef O_NONBLOCK
		    cma__g_file[fd]->user_fl.o_nonblock |
#endif
#ifdef _CMA_FNBLOCK_
		    cma__g_file[fd]->user_fl.fnblock |
#endif
#ifdef O_NDELAY
		    cma__g_file[fd]->user_fl.o_ndelay |
#endif
#ifdef _CMA_FNDELAY_
		    cma__g_file[fd]->user_fl.fndelay |
#endif
		    0;

		cma__trace ((
			cma__c_trc_io,
			"(set_user_nonblock_flags) fd %d user_non_blocking %d",
			fd, 
			cma__g_file[fd]->user_fl.user_non_blocking));

}	/* cma__set_user_nonblock_flags() */

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Performs pre- or post- `fork() reinitialization' work.
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
cma__reinit_thread_io(
	int	    flag)
    {
    cma_t_integer	i;


    if (flag == cma__c_reinit_prefork_lock) {		/* pre fork work */
	cma__int_lock (cma___g_file_lock);

	for (i = 0; i < cma__g_mx_file; i++) {
	    if (cma__g_file_obj[i].mutex != (cma__t_int_mutex *)0)
		cma__int_lock (cma__g_file_obj[i].mutex);
	    }

	cma__int_lock(cma__g_io_data_mutex);
	}
    else if (flag == cma__c_reinit_postfork_unlock) {	/* parent & child post work, out of kernel */
        cma__int_unlock(cma__g_io_data_mutex);

        for (i = 0; i < cma__g_mx_file; i++) {
	    if (cma__g_file_obj[i].mutex != (cma__t_int_mutex *)0)
		cma__int_unlock (cma__g_file_obj[i].mutex);
            }

	cma__int_unlock (cma___g_file_lock);
	}
    else if (flag == cma__c_reinit_postfork_clear) {	/* child post work, in kernel */
	cma__queue_init (&cma__g_select_queue);
 
	for (i = 0; i < cma__g_mx_file; i++) {
		cma_t_integer j;

		for (j = 0; j <= cma__c_max_io_type; j++)
			{
			cma__g_file_obj[i].waiters[j] = 0;
			}

		cma__g_poll_e_p[i].fd = -1;
		cma__g_poll_e_p[i].events = 0;
		cma__g_poll_e_p[i].revents = 0;
	    }
	}
	}


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma__set_nonblocking:  Sets specified fd to nonblocking mode
 *
 *	This routine sets the specified fd to non blocking mode.
 *      It assumes that the caller has determined that setting the
 *      file non blocking is correct.
 *
 *  FORMAL PARAMETERS:
 *
 *	file_descriptor	    File descritor 
 *
 *  IMPLICIT INPUTS:
 *
 *	All other file descriptors
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
 *	NOTE:  we assume that a previous call to cma__fd_reserve has locked 
 *		the cma__g_file[fd]->mutex, and a subsequent call to 
 *		cma__fd_unreserve will unlock it.
 */
extern void
cma__set_nonblocking(
	int		fd)
    {

    cma__assert_fail (
	    (fd >= 0) && (fd < cma__g_file_num) && (fd < cma__g_mx_file),
	    "cma__set_nonblocking:  bad value for file descriptor.");
    cma__assert_fail (
	    cma__is_open(fd),
	    "cma__set_nonblocking:  file not open.");
    cma__assert_fail (
	    cma__g_file[fd]->non_blocking,
	    "cma__set_nonblocking:  file incorrectly set non-blocking.");

    /*
     * If the file is of the appropriate type, enable the blocking I/O 
     * error (cma__is_blocking) but do not enable I/O ready signal 
     * (SIGIO).
     *
     * (I expect the fcntl call to work:  I don't know what to do if 
     * it doesn't, so bugcheck on error.)
     */
    if (__nr_fcntl (fd, F_SETFL, 
					__nr_fcntl(fd, F_GETFL) | cma___c_non_blocking_flag) == -1)
	cma__bugcheck("cma__set_nonblocking:  unexpected fcntl error");

    cma__g_file[fd]->set_non_blocking = cma_c_true;

    }

#endif					/* if !_CMA_THREAD_SYNC_IO_ */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_THREAD_IO.C */
/*  *30   16-APR-1992 19:42:47 SCALES "Add Jim Swist's fix to open-general" */
/*   28A1 23-MAR-1992 11:06:15 KEANE "Put socket fix in BL9" */
/*  *29   18-FEB-1992 15:31:02 BUTENHOF "Adapt to new alloc_mem protocol" */
/*  *28   25-NOV-1991 17:56:08 SCALES "Correct mutex ordering in fork handler" */
/*  *27   31-OCT-1991 12:39:57 BUTENHOF "Make local file masks in io_available static" */
/*  *26   18-OCT-1991 12:11:52 BUTENHOF "fix a bug in cma_close" */
/*  *25   14-OCT-1991 13:41:48 BUTENHOF "Use new config symbols" */
/*  *24   24-SEP-1991 16:29:19 BUTENHOF "Handle variable # of FDs more efficiently" */
/*  *23   17-SEP-1991 13:24:45 BUTENHOF "Handle variable # of FDs more efficiently" */
/*  *22   21-JUN-1991 19:55:04 SCALES "Fix assertion text in close-general" */
/*  *21   14-JUN-1991 15:30:17 BUTENHOF "Catch memory errors on dynamic init" */
/*  *20   14-JUN-1991 11:55:37 BUTENHOF "Create fd sync objects dynamically" */
/*  *19   10-JUN-1991 18:24:39 SCALES "Add sccs headers for Ultrix" */
/*  *18    5-JUN-1991 16:15:26 CURTIN "fork work" */
/*  *17   31-MAY-1991 18:42:44 SCALES "Add close-general function" */
/*  *16   29-MAY-1991 17:01:13 BUTENHOF "Fix synctax error" */
/*  *15   24-MAY-1991 16:50:01 CURTIN "Added a new reinit routine" */
/*  *14   10-MAY-1991 13:18:45 CURTIN "Removed extra underscore from routine names" */
/*  *13   10-MAY-1991 11:05:07 BUTENHOF "Use new macro to test for kernel locked" */
/*  *12    2-MAY-1991 22:29:05 SCALES "Fix a bug in cma_select" */
/*  *11    2-MAY-1991 16:30:38 CURTIN "Made into a number of smaller modules" */
/*  *10   12-APR-1991 23:37:15 BUTENHOF "Change references to kernel_critical" */
/*  *9    28-MAR-1991 19:06:13 SCALES "Fix initialization" */
/*  *8    27-FEB-1991 15:48:08 BUTENHOF "Fix for IBM" */
/*  *7    19-FEB-1991 23:49:18 SCALES "Put in null thread signal wakeup fix" */
/*  *6     5-FEB-1991 01:00:03 BUTENHOF "Switch to internal synchronization" */
/*  *5    30-JAN-1991 16:12:08 SCALES "Provide separate code path for non-blocking mode" */
/*  *4    29-JAN-1991 22:05:42 SCALES "Fix import_fd to avoid init dependency problem" */
/*  *3     2-JAN-1991 22:49:35 BUTENHOF "Fix names" */
/*  *2    18-DEC-1990 01:02:34 SCALES "Add fctnl() wrapper" */
/*  *1    12-DEC-1990 21:54:51 BUTENHOF "Thread synchronous I/O" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_THREAD_IO.C */
