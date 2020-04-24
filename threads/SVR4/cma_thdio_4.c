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
 * $Log: cma_thdio_4.c,v $
 * Revision 1.1.4.2  1996/02/17  23:33:33  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:49:46  marty]
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
 * Revision 1.1.4.1  1995/12/07  21:57:13  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:03:42  root]
 * 
 * Revision 1.1.2.2  1994/06/10  20:52:53  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  16:49:52  devsrc]
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
 * Revision 1.1.2.1  1994/04/14  21:31:59  ohara
 * 	moved again
 * 	[1994/04/14  20:57:08  ohara]
 * 
 * Revision 1.1.2.1  1994/04/01  20:14:58  jd
 * 	Moved from src/threads to here
 * 	[1994/04/01  18:31:02  jd]
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
# include <poll.h>
# include <stropts.h>
#include <cma_errors.h>
#include <cma_tcb.h>
#include <cma_stack.h>
#include <cma_util.h>

# include <cma_io_redefs_4.h>

/*
 * LOCAL DATA
 */

/*
 * LOCAL FUNCTIONS
 *
 * The prototypes for this single "_" (external) routine is here because
 * we cannot include the header file which contains the normal prototypes
 * (cma_ux.h) because it also contains macros which redirect the I/O calls
 * to use the routines in this module, and we need to access the real I/O
 * calls from the routines in this module.
 */
extern int cma_poll (struct pollfd *,unsigned long,int);



/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma_poll:  thread-synchronous version of the SVR4 poll system call
 *
 *  FORMAL PARAMETERS:
 *
 *	fds				Specifies the file descriptors to be examined.
 *	nfds			Number of file descriptors in fds
 *	timeout			timeout for this request
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
 *	Total number of file descriptors for which 'revents' is non-zero.
 *
 *  SIDE EFFECTS:
 *
 *	The current thread may block
 */

extern int
cma_poll(
	struct pollfd *fds,
	unsigned long nfds,
	int timeout)
    {
    int 					no_wait;
    cma_t_date_time			abs_time;
    struct timeval			act_time;
    cma_t_integer			result;
	cma_t_integer			fd;
    cma__t_int_tcb			*cur_tcb;
	cma__t_poll_element		poll_e[64], *poll_e_p = poll_e;


    cma__assert_not_kernel ();

    no_wait = 0;	/* immediate timeout */

    /*
     * attempt real poll, with immediate timeout
     */
    result = __nr_poll (fds, nfds, no_wait);

    /*
     * If we get a positive result or an error, return now.
     */
    if (result != 0) return result;

    /*
     * Return now if the caller specified an immediate timeout.
     */
    if (timeout == 0)
		{
		return( result );
		}

    /*
     * The poll would have blocked, so set up to block the thread and 
     * awaken it later, when one of the polled files is ready, or when
     * the timeout, if specified, has elapsed.
     */


	/*
	*	Copy the caller's poll info into our own.  Doing this ensures
	*	that we will not trip up on bad addresses later.
	*/
	if ( nfds > 64 )
		{
		/*
		*	The caller's poll info is greater than the one that
		*	we have on the stack.  Allocate enough memory to hold
		*	the caller's poll info.
		*/
		poll_e_p = (cma__t_poll_element *)cma__alloc_mem (
			sizeof (cma__t_poll_element) * nfds);
		if ( poll_e_p == (cma__t_poll_element *)NULL )
			{
			cma__set_errno(EAGAIN);
			return(-1);
			}
		}

	TRY
		{
		cma__memcpy(poll_e_p, fds, sizeof(cma__t_poll_element) * nfds);
		}
	CATCH (exc_illaddr_e)
		{
		/*
		*	Illegal user address.  Free memory if necessary and
		*	return error.
		*/
		if ( poll_e_p != poll_e )
			{
			cma__free_mem( poll_e_p );
			}
		cma__set_errno(EFAULT);
		return( -1 );
		}
	ENDTRY

    /*
     * Initialize the info in the tcb.
     */
    cur_tcb = cma__get_self_tcb ();
	cur_tcb->select.poll_info.nfds = nfds;
	cur_tcb->select.poll_info.poll_e_p = poll_e_p;
    cur_tcb->select.nfound = 0;

	/*
	 * Set the appropriate information in the global I/O structures.
	 */
	cma__int_lock (cma__g_io_data_mutex);
	for (fd = 0; fd < nfds; fd++) 
		{
		if ( poll_e_p[fd].fd < 0 )
			{
			continue;
			}

		cma__int_lock (cma__g_file[poll_e_p[fd].fd]->mutex);

		if (poll_e_p[fd].fd >= cma__g_file_num) 
			{
			cma__g_file_num = poll_e_p[fd].fd + 1;
			}

		cma__poll_set_gio_events( poll_e_p[fd].fd, poll_e_p[fd].events);
		cma__int_unlock (cma__g_file[poll_e_p[fd].fd]->mutex);
		}
	cma__int_unlock ( cma__g_io_data_mutex );

    /*
     * If specified, convert the timeout from a delta time to an absolute
     * time.
     */
    if (timeout != INFTIM) 
		{
		cma_t_date_time	timeout_tmp;
		
		cma__interval_to_time( timeout / 1000, &timeout_tmp );

		/*
		 * Add timeout (delta) to current (absolute) time to make an
		 * absolute timeout time.
		 */
		cma__get_time (&abs_time);
		cma__add_time (&abs_time, &abs_time, &timeout_tmp);
		}

	/*
	*	This is to record errors below.
	*/
	result = 0;

    /*
     * Now block on a condition variable, so that we can be alerted, until
     * one or more of the files selected files are ready or until we time out.
     */
    cma__int_lock (cur_tcb->tswait_mutex);

    TRY 
		{
		cma__enter_kernel();
		cma__queue_insert (&(cur_tcb->select.queue), &cma__g_select_queue);
		cma__exit_kernel();

		while (cur_tcb->select.nfound == 0) {
			if (timeout != INFTIM) {
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
    FINALLY 
		{
		cma__enter_kernel();
		cma__queue_remove (&(cur_tcb->select.queue));
		cma__exit_kernel();

		cma__int_unlock (cur_tcb->tswait_mutex);

		/*
		 * Clear the appropriate information that we set in the global I/O
		 * structures.
		 */
		cma__int_lock (cma__g_io_data_mutex);
		for (fd = 0; fd < nfds; fd++) 
			{
			if ( poll_e_p[fd].fd < 0 )
				{
				continue;
				}

			cma__int_lock (cma__g_file[poll_e_p[fd].fd]->mutex);
			cma__poll_clear_gio_events( poll_e_p[fd].fd, poll_e_p[fd].events );
			cma__int_unlock( cma__g_file[poll_e_p[fd].fd]->mutex );
			}
		cma__int_unlock( cma__g_io_data_mutex );

		/*
		*	Copy the poll information back to the user's structure.
		*	It is necessary that this be done within the above
		*	exception region because we must make sure that
		*	any memory which was allocated above is freed.
		*/
		TRY
			{
			cma__memcpy(fds, poll_e_p, sizeof(cma__t_poll_element) * nfds);
			}
		CATCH (exc_illaddr_e)
			{
			/*
			*	Illegal user address.  Make sure that the error is
			*	propagated to the user.
			*/
			cma__set_errno(EFAULT);
			result = -1;
			}
		ENDTRY

		/*
		*	Free memory if it had been allocated above.
		*/
		if ( poll_e_p != poll_e )
			{
			cma__free_mem( poll_e_p );
			}
		}
    ENDTRY

	/*
	*	Make sure that we return failure if there was a problem with
	*	the memory that the caller passed us.
	*/
	if ( result == -1 )
		{
		return( result );
		}
	else
		{
		return( cur_tcb->select.nfound );
		}
    }    

#endif					/* if !_CMA_THREAD_SYNC_IO_ */

