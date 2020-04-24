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
 * $Log: cma_thread_io.c,v $
 * Revision 1.1.6.1  1996/05/10  13:32:05  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:53 UTC  bissen
 * 	OT 12823 fix for cma__init_thread_io
 * 	[1996/05/09  21:02:40  arvind]
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
 * Revision 1.1.2.2  1994/06/09  13:35:50  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:26:22  devsrc]
 * 
 * 	Bring forward IBM integration changes to BL10 sources.
 * 	[1992/08/28  14:14:54  bolinger]
 * 
 * 	Bring forward 1.0.1 revision 1.1.2.2 (fix for OT defect 2540).
 * 	[1992/08/10  16:35:18  bolinger]
 * 
 * Revision 1.1.2.1  1994/04/01  20:14:31  jd
 * 	Moved from src/threads to here
 * 	[1994/04/01  17:53:25  jd]
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
 * The cma__g_wait_fds variable is not dynamically allocated because C does
 * not support dynamic arrays; array index calculations would become more
 * complicated, and the amount of storage taken up by 3 instances of even a
 * very large bitmap (say 4096 FDs) isn't prohibitive (1536 bytes).
 */
cma__t_file_mask    cma__g_wait_fds[3];
cma_t_integer	    cma__g_nspm;
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
	    cma__g_file[fd]->waiters[(int)cma__c_io_read] == 0,
	    "cma__close_general:  Closing a file with threads waiting to read.");
    cma__assert_warn (
	    cma__g_file[fd]->waiters[(int)cma__c_io_write] == 0,
	    "cma__close_general:  Closing a file with threads waiting to write.");
    cma__assert_warn (
	    cma__g_file[fd]->waiters[(int)cma__c_io_except] == 0,
	    "cma__close_general:  Closing a file with threads waiting for exception.");
    cma__assert_warn (
	    !cma__fdm_is_set (fd, &cma__g_wait_fds[(int)cma__c_io_read]),
	    "cma__close_general:  Closing a file with read mask bit set.");
    cma__assert_warn (
	    !cma__fdm_is_set (fd, &cma__g_wait_fds[(int)cma__c_io_write]),
	    "cma__close_general:  Closing a file with write mask bit set.");
    cma__assert_warn (
	    !cma__fdm_is_set (fd, &cma__g_wait_fds[(int)cma__c_io_except]),
	    "cma__close_general:  Closing a file with except mask bit set.");

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
    cma_t_integer   i;

    EXCEPTION_INIT(io_avail_exc);


    cma__g_io_data_mutex = cma__get_mutex (&cma__g_def_attr);
    cma__obj_set_name (cma__g_io_data_mutex, "io database");
    cma___g_file_lock = cma__get_mutex (&cma__g_def_attr);
    cma__obj_set_name (cma___g_file_lock, "dynamic io init");

    cma__queue_init (&cma__g_select_queue);

    cma__fdm_zero (&cma__g_wait_fds[(int)cma__c_io_read]);
    cma__fdm_zero (&cma__g_wait_fds[(int)cma__c_io_write]);
    cma__fdm_zero (&cma__g_wait_fds[(int)cma__c_io_except]);

    /* 
     * Initialize CMA's I/O database such that all files are initially closed.
     */
    cma__g_file = (cma__t_file_obj **)cma__alloc_mem (
	    sizeof (cma__t_file_obj *) * cma__g_mx_file);
    cma__g_file_obj = (cma__t_file_obj *)cma__alloc_mem (
	    sizeof (cma__t_file_obj) * cma__g_mx_file);

    for (i = 0; i < cma__g_mx_file; i++) {
	cma_t_integer j;

	cma__g_file_obj[i].mutex = (cma__t_int_mutex *)cma_c_null_ptr;
	cma__g_file_obj[i].ref_count = 0;

	for (j = 0; j <= cma__c_max_io_type; j++)
		{
		cma__g_file_obj[i].waiters[j] = 0;
		}
	cma__g_file[i] = (cma__t_file_obj *)cma_c_null_ptr;
	}

    /*
     * "Open" all files that may have been passed across
     * the exec, not just stdin, stdout, stderr.
     *
     * While we're at it, we want to use cma__g_mx_file instead
     * of cma__c_mx_file (which is defined to be FD_SETSIZE, a
     * very large [2048] number).  Assert that cma__g_mx_file
     * is no larger than cma__c_mx_file, since we created
     * cma__g_file and cma__g_file_obj using cma__c_mx_file.
     */

    cma__assert_fail (
        cma__g_mx_file <= cma__c_mx_file,
        "cma__init_thread_io: cma__g_mx_file > cma__c_mx_file" );

    for (i = 0; i < cma__g_mx_file; i++) {
        if( fcntl(i, F_GETFL, 0) == -1 && errno == EBADF )
            continue;
        cma__open_general (i);
        }
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma__io_available:  test all file descriptors to find out if I/O
 *			can be continued.  Any threads blocked on a select
 *			that can complete are readied, and any threads blocked
 *			for I/O which can now be performed are signaled.
 *
 *  FORMAL PARAMETERS:
 *
 *	function:  read | write
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
 *	cma_c_true:	the selected I/O operation would not block
 *	cma_c_false:	the selected I/O operation would block
 *
 *  SIDE EFFECTS:
 *
 *	None
 */
extern cma_t_boolean
cma__io_available(
	cma__t_io_type	function,
	int		fd,
	struct timeval	*timeout)
    {
    static cma__t_file_mask	readfds, writefds, exceptfds;
    cma_t_integer	nfound = 0;
    cma_t_integer	i;
    struct stat		buf;


    cma__assert_fail (
	    cma__tac_isset (&cma__g_kernel_critical),
	    "cma__io_available:  kernel is unlocked.");
    cma__assert_fail (
	    ((int)function >= (int)cma__c_io_read) && 
		((int)function <= (int)cma__c_io_write),
	    "cma__io_available:  bad value for function.");
    cma__assert_fail (
	    (fd >= 0) && (fd < cma__g_file_num) && (fd < cma__g_mx_file),
	    "cma__io_available:  bad value for file descriptor.");

    /*
     * Unmutexed reads
     */
    cma__fdm_copy (
	    &cma__g_wait_fds[(int)cma__c_io_read],
	    &readfds,
	    cma__g_mx_file);
    cma__fdm_copy (
	    &cma__g_wait_fds[(int)cma__c_io_write],
	    &writefds,
	    cma__g_mx_file);
    cma__fdm_copy (
	    &cma__g_wait_fds[(int)cma__c_io_except],
	    &exceptfds,
	    cma__g_mx_file);
 /*
  * Special code for null thread so that we do not miss out on any
  * signals that wakeup sigwaiters.  We repeat some code so that
  * we don't have to pay the overhead of the TRY clause on every
  * call to cma__io_available.
  */
 if (cma__get_self_tcb()->kind == cma__c_thkind_null) {
  cma_t_integer breakout = 0;

  TRY {
   io_avail_null_thread = 1;

   if (cma__g_defers_processed == 0)
    breakout = 1;

   else {
    while (nfound <= 0) {
	nfound = select (
		cma__g_file_num,
		(fd_set *)&readfds,
		(fd_set *)&writefds,
		(fd_set *)&exceptfds,
		timeout); 

	if (nfound == 0) {
	    breakout = 1;
	    break;
	}

	if (nfound == -1) 

	    /*
	     * An error occurred.  
	     *
	     * If it is EBADF, take corrective action, then retry the select.
	     */

	    if (errno == EBADF) {

		/*
		 * One of the specified bit masks was bad.  Presumeably, this 
		 * is because the "file" is no longer open.  Find the bad 
		 * one(s), declare it(them) closed, and signal any waiters.
		 */

		for (i = 0; i < cma__g_mx_file; i++) {

		    if (    cma__fdm_is_set (i, &readfds) || 
			    cma__fdm_is_set (i, &writefds) || 
			    cma__fdm_is_set (i, &exceptfds))
			{
			if ((fstat (i, &buf) == -1) && (errno == EBADF)) {
			    cma__assert_warn (
				i < cma__g_file_num,
				"Big, bad, file descriptor.");

			    /*
			     * FIX-ME:  
			     * 
			     * What should happen here is we need to "defer"
			     * a cma_close() on this file.
			     */
			    cma__bugcheck ("cma__io_available: File was unexpectedly closed");

			    }
			}
		    }
		}

	    else if (errno == EINTR) {
		breakout = 1;
		break;
	    }
	    /*
	     * We are do not expect any other errors here, and we do not know
	     * how to recover should they occur.
	     */
	    else
		cma__bugcheck("cma__io_available: unexpected select error");
	}
    }  
   io_avail_null_thread = 0;
  }

  CATCH (io_avail_exc) {
   breakout = 1;
   /* io_avail_null_thread = 0; this should be done when exception is raised */
  }

  CATCH_ALL {
   io_avail_null_thread = 0;
   RERAISE;
  }

  ENDTRY

  if (breakout) return cma_c_false;
 }
 else {
    while (nfound <= 0) {
	nfound = select (
		cma__g_file_num,
		(fd_set *)&readfds,
		(fd_set *)&writefds,
		(fd_set *)&exceptfds,
		timeout); 

	if (nfound == 0)  return cma_c_false;	/* No files are ready. */

	if (nfound == -1) 

	    /*
	     * An error occurred.  
	     *
	     * If it is EBADF, take corrective action, then retry the select.
	     */

	    if (errno == EBADF) {

		/*
		 * One of the specified bit masks was bad.  Presumeably, this 
		 * is because the "file" is no longer open.  Find the bad 
		 * one(s), declare it(them) closed, and signal any waiters.
		 */

		for (i = 0; i < cma__g_mx_file; i++) {

		    if (    cma__fdm_is_set (i, &readfds) || 
			    cma__fdm_is_set (i, &writefds) || 
			    cma__fdm_is_set (i, &exceptfds))
			{
			if ((fstat (i, &buf) == -1) && (errno == EBADF)) {
			    cma__assert_warn (
				i < cma__g_file_num,
				"Big, bad, file descriptor.");

			    /*
			     * FIX-ME:  
			     * 
			     * What should happen here is we need to "defer"
			     * a cma_close() on this file.
			     */
			    cma__bugcheck ("cma__io_available: File was unexpectedly closed");

			    }
			}
		    }
		}
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
	    else if (errno == EINTR)
		return cma_c_false;

	    /*
	     * We are do not expect any other errors here, and we do not know
	     * how to recover should they occur.
	     */
	    else
		cma__bugcheck("cma__io_available: unexpected select error");
	} 

 }
    /*
     * We have found that some of our pending I/O's can now be satisfied.
     */

    /*
     * For each thread blocked on a select, check if it any of it's selected
     * file descriptors are ready for I/O.  If one or more are, set these bits
     * in the thread's masks, remove the thread from the timer and select 
     * queues and ready it.
     */
    if (!cma__queue_empty (&cma__g_select_queue)) {
	cma__t_queue	   *select_queue;
	cma_t_integer	    bits;
	static cma__t_file_mask    temp_r, temp_w, temp_e;

	/*
	 * The queue node is inside the select block in the tcb, so it's 
	 * address cannot be used as the pointer to the tcb.
	 */
	select_queue = cma__queue_next (&cma__g_select_queue);

	while (select_queue != &cma__g_select_queue) 
	    {
	    cma__t_int_tcb	   *select_tcb;


	    select_tcb = cma__base(select_queue, select.queue, cma__t_int_tcb);
	    cma__fdm_and (&temp_r, &readfds,   select_tcb->select.rfds);
	    cma__fdm_and (&temp_w, &writefds,  select_tcb->select.wfds);
	    cma__fdm_and (&temp_e, &exceptfds, select_tcb->select.efds);

	    /*
	     * Count the number of ready file descriptors.
	     */
	    bits = 0;
	    cma__fdm_count_bits (&temp_r, bits);
	    cma__fdm_count_bits (&temp_w, bits);
	    cma__fdm_count_bits (&temp_e, bits);

	    /*
	     * If there are files which are ready for their I/O, copy the
	     * masks indicating the ready files into the tcb, remove the
	     * thread from the timer queue
	     */
	    if (bits > 0) {
		cma__fdm_copy (
			&temp_r,
			select_tcb->select.rfds,
			cma__g_mx_file);
		cma__fdm_copy (
			&temp_w,
			select_tcb->select.wfds,
			cma__g_mx_file);
		cma__fdm_copy (
			&temp_e,
			select_tcb->select.efds,
			cma__g_mx_file);
		select_tcb->select.nfound = bits;
		cma__int_signal2_int (select_tcb->tswait_cv);
		}

	    select_queue = cma__queue_next(select_queue);
	    }
	}
    
    /* 
     * For each "file" which has a thread blocked for I/O on it, signal the
     * condition variable indicating that it is ready.
     */
    for (i = 0; nfound > 0; i++) {
	cma__assert_fail (
		i < cma__g_file_num,
		"cma__io_available:  i exceeded cma__g_file_num before nfound reached zero."
		);

	/*
	 * This test is intended to catch instances where the user is 
	 * selecting a file which DECthreads doesn't realize is open.
	 */
	if (cma__g_file[i] == (cma__t_file_obj *)cma_c_null_ptr) continue;

	if (cma__fdm_is_set (i, &readfds)) {
	    cma__int_signal2_int (cma__g_file[i]->io_wait[(int)cma__c_io_read]);
	    nfound--;
	    }

	if (cma__fdm_is_set (i, &writefds)) {
	    cma__int_signal2_int(cma__g_file[i]->io_wait[(int)cma__c_io_write]);
	    nfound--;
	    }

	if (cma__fdm_is_set (i, &exceptfds)) {
	    cma__int_signal2_int (cma__g_file[i]->io_wait[(int)cma__c_io_read]);
	    cma__int_signal2_int(cma__g_file[i]->io_wait[(int)cma__c_io_write]);
	    nfound--;
	    }
	}

    return (cma__fdm_is_set (fd, &exceptfds)
	 || cma__fdm_is_set (
		    fd, 
		    (function == cma__c_io_read ? &readfds : &writefds)));
    }

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

    if ((--cma__g_file[fd]->waiters[(int)function]) == 0) { 
	cma__int_lock (cma__g_io_data_mutex);
	cma__fdm_clr_bit (fd, &cma__g_wait_fds[(int)function]);
	cma__int_unlock (cma__g_io_data_mutex);
        do_check = cma_c_false;
	}
    else
	do_check = cma__is_open(fd);

    if ((--cma__g_file[fd]->waiters[(int)cma__c_io_except]) == 0) {
	cma__int_lock (cma__g_io_data_mutex);
	cma__fdm_clr_bit (fd, &cma__g_wait_fds[(int)cma__c_io_except]);
	cma__int_unlock (cma__g_io_data_mutex);
	}

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

    cma__g_file[fd]->waiters[(int)function]++;
    cma__g_file[fd]->waiters[(int)cma__c_io_except]++;
    cma__int_lock (cma__g_io_data_mutex);
    cma__fdm_set_bit (fd, &cma__g_wait_fds[(int)function]);
    cma__fdm_set_bit (fd, &cma__g_wait_fds[(int)cma__c_io_except]);
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
		    cma__obj_set_name (mu, "file mutex");
		    cma__obj_set_name (read_cv, "file read cv");
		    cma__obj_set_name (write_cv, "file write cv");
		    cma__g_file_obj[fo].mutex = mu;
		    cma__g_file_obj[fo].io_wait[(int)cma__c_io_read] = read_cv;
		    cma__g_file_obj[fo].io_wait[(int)cma__c_io_write] = write_cv;
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
	if ( (uflags = fcntl(fd, F_GETFL)) != -1 )
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

		cma__g_file[fd]->user_fl.o_ndelay =   ((flag & O_NDELAY) != 0);
		cma__g_file[fd]->user_fl.o_nonblock = ((flag & O_NONBLOCK) != 0);
		cma__g_file[fd]->user_fl.user_non_blocking = 
		    cma__g_file[fd]->user_fl.o_nonblock |
		    cma__g_file[fd]->user_fl.o_ndelay;

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

	    cma__fdm_zero (&cma__g_wait_fds[(int)cma__c_io_read]);
	    cma__fdm_zero (&cma__g_wait_fds[(int)cma__c_io_write]);
	    cma__fdm_zero (&cma__g_wait_fds[(int)cma__c_io_except]);
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
    if (fcntl (fd, F_SETFL, 
				fcntl(fd, F_GETFL) | cma___c_non_blocking_flag) == -1)
	cma__bugcheck("cma__set_nonblocking:  unexpected fcntl error");

    cma__g_file[fd]->set_non_blocking = cma_c_true;

    }

#endif					/* if !_CMA_THREAD_SYNC_IO_ */
