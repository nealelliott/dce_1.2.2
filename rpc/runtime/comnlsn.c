/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: comnlsn.c,v $
 * Revision 1.1.72.1  1996/05/10  13:10:18  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/2  1996/04/18  19:13 UTC  bissen
 * 	unifdef for single threaded client
 * 	[1996/02/29  20:41 UTC  bissen  /main/HPDCE02/bissen_st_rpc/1]
 *
 * 	Merge allocation failure detection cleanup work
 * 	[1995/05/25  21:40 UTC  lmm  /main/HPDCE02/3]
 *
 * 	HP revision /main/DCE_1.2/1  1996/01/03  19:00 UTC  psn
 * 	Remove Mothra specific code
 * 	[1995/11/16  21:31 UTC  jrr  /main/HPDCE02/jrr_1.2_mothra/1]
 *
 * 	Merge allocation failure detection cleanup work
 * 	[1995/05/25  21:40 UTC  lmm  /main/HPDCE02/3]
 *
 * 	allocation failure detection cleanup
 * 	[1995/05/25  21:01 UTC  lmm  /main/HPDCE02/lmm_alloc_fail_clnup/1]
 *
 * 	Merge allocation failure detection functionality into Mothra
 * 	[1995/04/02  01:14 UTC  lmm  /main/HPDCE02/2]
 *
 * 	add memory allocation failure detection
 * 	[1995/04/02  00:18 UTC  lmm  /main/HPDCE02/lmm_rpc_alloc_fail_detect/1]
 *
 * 	Submitted rfc31.0: Single-threaded DG client and RPC_PREFERRED_PROTSEQ.
 * 	[1994/12/09  19:18 UTC  tatsu_s  /main/HPDCE02/1]
 *
 * 	rfc31.0: Cleanup.
 * 	[1994/12/07  20:59 UTC  tatsu_s  /main/tatsu_s.st_dg.b0/2]
 *
 * 	rfc31.0: Initial version.
 * 	[1994/11/30  22:25 UTC  tatsu_s  /main/tatsu_s.st_dg.b0/1]
 *
 * Revision 1.1.68.2  1994/07/15  16:56:14  tatsu_s
 * 	Fix OT10589: Reversed the execution order of fork_handlers.
 * 	[1994/07/09  02:43:09  tatsu_s]
 * 
 * Revision 1.1.68.1  1994/01/21  22:35:38  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:55:39  cbrooks]
 * 
 * Revision 1.1.66.1  1993/09/15  15:39:51  damon
 * 	Synched with ODE 2.1 based build
 * 	[1993/09/15  15:32:04  damon]
 * 
 * Revision 1.1.7.2  1993/09/14  16:49:38  tatsu_s
 * 	Bug 8103 - Fixed race condition between rpc_server_listen() and
 * 	rpc__nlsn_fork_handler().
 * 	Added the call to pthread_detach() after pthread_join() to
 * 	reclaime the storage.
 * 	If the listener thread was not running before fork, don't start
 * 	it after fork.
 * 	Added the explicit initialization of static (INTERNAL) variables.
 * 	[1993/09/13  17:09:18  tatsu_s]
 * 
 * Revision 1.1.4.5  1993/03/05  19:20:40  weisman
 * 	      14-jan-93 bb        add pthread_testcancel after
 * 	                          pthread_setasynccancel.
 * 	[1993/03/05  19:09:06  weisman]
 * 
 * Revision 1.1.4.4  1993/01/03  23:22:31  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  20:02:28  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/23  20:44:32  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:34:13  zeliff]
 * 
 * Revision 1.1.4.2  1992/10/13  20:54:38  weisman
 * 	      21-aug-92 wh        Use rpc_g_default_pthread_attr when creating
 * 	                          the network listener thread.
 * 	[1992/10/13  20:47:18  weisman]
 * 
 * Revision 1.1.2.3  1992/05/21  13:15:54  mishkin
 * 	Raise debug level on a debug printf.
 * 	[1992/05/20  21:11:08  mishkin]
 * 
 * Revision 1.1.2.2  1992/05/01  17:02:18  rsalz
 * 	  5-mar-92 wh        DCE 1.0.1 merge.
 * 	 17-dec-91 markar    enable async cancels across select in NON_CANCELLABLE_IO path
 * 	[1992/05/01  16:49:59  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:09:52  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. &
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME:
**
**      comnlsn.c
**
**  FACILITY:
**
**      Remote Procedure Call (RPC)
**
**  ABSTRACT:
**
**      Network Listener Service internal operations.
**      This file provides the "less portable" portion of the PRIVATE
**      Network Listener internal interface (see comnet.c) and should
**      ONLY be called by the PRIVATE Network Listener Operations.
**
**      Different implementations may choose to provide an alternate
**      implementation of the listener thread processing by providing
**      a reimplementation of the operations in this file.  The portable
**      Network Listener Services operations (for the public and private
**      API) is in comnet.c
**
**      This particular implementation is "tuned" for:
**          a) the use of a pthread for listener processing
**          b) a rpc_socket_t is a UNIX File Descriptor
**          c) BSD UNIX select()
**
**
*/

#include <commonp.h>
#include <com.h>
#include <comp.h>
#include <comnetp.h>

/*
*****************************************************************************
*
* local data structures
*
*****************************************************************************
*/


/*
 * Some defaults related to select() fd_sets.
 */

#ifndef RPC_C_SELECT_NFDBITS
#  define RPC_C_SELECT_NFDBITS      NFDBITS
#endif

#ifndef RPC_SELECT_FD_MASK_T
#  define RPC_SELECT_FD_MASK_T      fd_mask
#endif

#ifndef RPC_SELECT_FD_SET_T
#  define RPC_SELECT_FD_SET_T       fd_set
#endif

#ifndef RPC_SELECT_FD_COPY
#  define RPC_SELECT_FDSET_COPY(src_fd_set,dst_fd_set,nfd) { \
    int i; \
    RPC_SELECT_FD_MASK_T *s = (RPC_SELECT_FD_MASK_T *) &src_fd_set; \
    RPC_SELECT_FD_MASK_T *d = (RPC_SELECT_FD_MASK_T *) &dst_fd_set; \
    for (i = 0; i < (nfd); i += RPC_C_SELECT_NFDBITS) \
        *d++ = *s++; \
   }
#endif


/*
 * Miscellaneous Data Declarations
 */

INTERNAL pthread_t                  listener_thread;
INTERNAL boolean                    listener_thread_running = false;
INTERNAL boolean                    listener_thread_was_running = false;
INTERNAL boolean                    listener_should_handle_cancels = false;

INTERNAL rpc_listener_state_t       listener_state_copy;

INTERNAL RPC_SELECT_FD_SET_T        listener_readfds;
INTERNAL int                        listener_nfds = 0;

/*
 * This should really be heap allocated (see g_e_t_dtablesize(2)) to
 * deal with systems that have an extremely large fd_set.  For now,
 * at least keep this off the stack.
 */
INTERNAL RPC_SELECT_FD_SET_T        readfds_copy;



INTERNAL void copy_listener_state _DCE_PROTOTYPE_ ((
        rpc_listener_state_p_t   /*lstate*/
    ));

INTERNAL void lthread _DCE_PROTOTYPE_ ((
        rpc_listener_state_p_t   /*lstate*/
    ));

INTERNAL void lthread_loop _DCE_PROTOTYPE_ ((void));



/*
 * R P C _ _ N L S N _ A C T I V A T E _ D E S C
 *
 * Mark that the specified descriptor is now "live" -- i.e., that events
 * on it should be processed.  This routine is also responsible for
 * starting up a listener thread if once doesn't already exist.  
 *
 * Note that once a socket has been activated, it is should never be
 * removed and not closed from the set of sockets that are
 * select(2)'d on, because we always must always "drain" the socket
 * of events (via recv or accept) so it doesn't get "clogged up"
 * with stuff (that would consume system resources). 
 */

PRIVATE void rpc__nlsn_activate_desc 
#ifdef _DCE_PROTO_
(
    rpc_listener_state_p_t  lstate,
    unsigned32              idx,
    unsigned32              *status
)
#else
(lstate, idx, status)
rpc_listener_state_p_t  lstate;
unsigned32              idx;
unsigned32              *status;
#endif
{
    RPC_MUTEX_LOCK_ASSERT (lstate->mutex);

    lstate->socks[idx].is_active = true;

    /*
     * If the listener thread is running, then just cancel it and let
     * it do the work.  Note that that thread must do the work since
     * this module's state is not covered by a mutex (for efficiency).
     * Otherwise, start the listen thread now.
     */

    if (listener_thread_running)
    {
        pthread_cancel (listener_thread);
    }
    else
    {
	/*
	 * Start the timer thread.
	 * Once we become multi-threaded, can't go back to single-threaded...
	 *
	 * Note: We are taking the global lock to modify
	 * rpc_g_is_single_threaded unless it's a client socket. (The client
	 * has the global lock taken by call_start().
	 */
	if (rpc_g_is_single_threaded)
	{
	    if (lstate->socks[idx].is_server)
		RPC_LOCK(0);

	    if ((*status = rpc__timer_start()) == rpc_s_no_memory){
		if (lstate->socks[idx].is_server)
		    RPC_UNLOCK(0);
		return;
	    }
	    rpc_g_is_single_threaded = false;

	    if (lstate->socks[idx].is_server)
		RPC_UNLOCK(0);
	}

        listener_thread_running = true;
        listener_should_handle_cancels = true;
	TRY {
        pthread_create (
            &listener_thread,                   /* new thread    */
            rpc_g_default_pthread_attr,         /* attributes    */
            (pthread_startroutine_t) lthread,   /* start routine */
            (pthread_addr_t) lstate);           /* arguments     */
	} CATCH_ALL {
	    listener_thread_running = false;
	    listener_should_handle_cancels = false;
	    *status = rpc_s_no_memory;
	    return;
	} ENDTRY
    }

    *status = rpc_s_ok;
}

/*
 * R P C _ _ N L S N _ D E A C T I V A T E _ D E S C
 *
 * Mark that the specified descriptor is no longer "live" -- i.e., that
 * events on it should NOT be processed.
 */

PRIVATE void rpc__nlsn_deactivate_desc 
#ifdef _DCE_PROTO_
(
    rpc_listener_state_p_t  lstate,
    unsigned32              idx,
    unsigned32              *status
)
#else
(lstate, idx, status)
rpc_listener_state_p_t  lstate;
unsigned32              idx;
unsigned32              *status;
#endif
{
    pthread_t   current_thread;

    RPC_MUTEX_LOCK_ASSERT (lstate->mutex);

    *status = rpc_s_ok;

    lstate->socks[idx].is_active = false;

    /*
     * If the listener thread is not running, there's nothing more to do.
     */

    if (! listener_thread_running)
    {
        return;
    }

    /*
     * If we're the listener thread, then just update the listener state
     * ourselves.  If we're not, then cancel the listener thread so it
     * will pick up the new state.  Note that in the latter case, we
     * synchronize with the listener thread.  The point of this exercise
     * is that when we return from this function, we want to make SURE
     * that the listener thread will NOT try to select() on the FD we're
     * trying to deactivate.  (Some callers of this function close the FD
     * and some other [perhaps not RPC runtime] thread might call open()
     * or socket() and get that FD.  We need to make sure that the listener
     * thread never uses this reincarnated FD.)
     */

    current_thread = pthread_self();
    if (pthread_equal (current_thread, listener_thread))
    {
        copy_listener_state(lstate);
    }
    else 
    {
        lstate->reload_pending = true;
        pthread_cancel (listener_thread);

        while (lstate->reload_pending)
        {
            RPC_COND_WAIT (lstate->cond, lstate->mutex);
        }
    }
} 



/*
 * C O P Y _ L I S T E N E R _ S T A T E
 *
 * Copy the listener state pointed to by the input parameter into our
 * copy of the listener state.  N.B. This routine must be called only
 * from the listener thread.
 */

INTERNAL void copy_listener_state 
#ifdef _DCE_PROTO_
(
    rpc_listener_state_p_t  lstate
)
#else
(lstate)
rpc_listener_state_p_t  lstate;
#endif
{
    unsigned16              nd;

    RPC_MUTEX_LOCK_ASSERT (lstate->mutex);

    /*
     * Make a copy of the active entries in the network information
     * table.  We'll pass the copy to the listen loop.  Using a copy
     * means that the listen loop can run without taking and releasing
     * locks.  Descriptors are presumably added/deleted infrequently
     * enough that this strategy is a net win.  We also compute the
     * "nfds" and "readfds" arguments to select(2), which we also pass
     * down to the listen loop.
     */

    FD_ZERO (&listener_readfds);
    listener_nfds = 0;

    for (nd = 0, listener_state_copy.num_desc = 0; nd < lstate->high_water; nd++)
    {
        rpc_listener_sock_p_t lsock = &lstate->socks[nd];

        if (lsock->busy)
        {
            listener_state_copy.socks[listener_state_copy.num_desc++] = *lsock;
            FD_SET (lsock->desc, &listener_readfds);
            if (lsock->desc + 1 > listener_nfds)
            {
                listener_nfds = lsock->desc + 1;
            }
        }
    }

    listener_state_copy.high_water = listener_state_copy.num_desc;

    /*
     * Let everyone who's waiting on our completion of the state update
     * know we're done.
     */
    lstate->reload_pending = false;
    RPC_COND_BROADCAST (lstate->cond, lstate->mutex);
}


/*
 * L T H R E A D
 *
 * Startup routine for the listen thread.
 */

INTERNAL void lthread 
#ifdef _DCE_PROTO_
(
    rpc_listener_state_p_t  lstate
)
#else
(lstate)
rpc_listener_state_p_t  lstate;
#endif
{
    /*
     * Loop, calling the real listen loop on each pass.  Each time a
     * socket (descriptor) is activated or deactivated this thread is
     * cancelled (so that we can know about the change and call "select"
     * with the right masks).  When the thread is supposed to really
     * go away, as on a fork, the handle_cancel boolean will be set
     * appropriately before the cancel is posted.
     */
              
    while (listener_should_handle_cancels)
    {                                 
        RPC_MUTEX_LOCK (lstate->mutex);
        copy_listener_state (lstate);
        RPC_MUTEX_UNLOCK (lstate->mutex);

        /*
         * Invoke the real listen loop, protecting against (presumably)
         * cancel-induced unwinds.
         */

        TRY
        {
            lthread_loop ();
        }     
        CATCH(pthread_cancel_e)
        {    
#ifdef NON_CANCELLABLE_IO
            pthread_setasynccancel(CANCEL_OFF);
#endif   
            RPC_DBG_PRINTF (rpc_e_dbg_general, 2, ("(lthread) Unwound\n"));
        }
        ENDTRY
    }
}



/*
 * L T H R E A D _ L O O P
 *
 * Server listen thread loop.
 */

INTERNAL void lthread_loop (void)
{
    unsigned32          status;
    int                 nd;
    int                 n_found;

    /*
     * Loop waiting for incoming packets.
     */

    while (true)
    {
        /*
         * Wait for packets.
         */

        do
        { 
            RPC_SELECT_FDSET_COPY(listener_readfds, readfds_copy, listener_nfds);

            /*
             * Block waiting for packets.  We ocassionally need to see
             * changes in the readfds and listener state even if we aren't
             * recving pkts.  At such times, we cancel this thread to
             * get it to see the updates.  If 'select' isn't cancellable
             * on a particular implementation, enabling async
             * cancellability should do the trick.
             *
             * By posix definition pthread_setasynccancel is not a "cancel
             * point" because it must return an error status and an errno.
             * pthread_setasynccancel(CANCEL_ON) will not deliver
             * a pending cancel nor will the cancel be delivered asynchronously,
             * thus the need for pthread_testcancel.
             * 
             */
#ifdef NON_CANCELLABLE_IO
            pthread_setasynccancel(CANCEL_ON);
	    pthread_testcancel();
#endif
            RPC_LOG_SELECT_PRE;
            n_found = select (
                (int) listener_nfds, (int *) &readfds_copy, NULL, NULL, NULL);
            RPC_LOG_SELECT_POST;

#ifdef NON_CANCELLABLE_IO
            pthread_setasynccancel(CANCEL_OFF);
#endif   
            if (n_found < 0)
            {
                if (errno != EINTR)
                {
                    RPC_DBG_GPRINTF 
                        (("(lthread_loop) select failed: %d, errno=%d\n",
                        n_found, errno));
                
                    /*
                     * Check for pending cancels.  Select might return
                     * EIO (and not check for a pending cancel) because
                     * of a broken socket that another thread is trying
                     * to deactivate.  The testcancel will induce us
                     * to take the cancel that the thread trying to
                     * deactivate the socket sent us.
                     */
                    pthread_testcancel();
                }
                continue;
            }
        }
        while (n_found <= 0);

        /*
         * Process any descriptors that were active.
         */

        for (nd = 0; n_found && (nd < listener_state_copy.num_desc); nd++)
        {
            rpc_listener_sock_p_t lsock = &listener_state_copy.socks[nd];

            if (lsock->busy && FD_ISSET (lsock->desc, &readfds_copy))
            {
                n_found--;

                (*lsock->network_epv->network_select_disp)
                    (lsock->desc, lsock->priv_info, lsock->is_active, &status);
                if (status != rpc_s_ok)
                {
                    RPC_DBG_GPRINTF
                    (("(lthread) select dispatch failed: desc=%d *status=%d\n",
                        lsock->desc, status));

                    /*
                     * Check for pending cancels.  Select might have
                     * returned "success" (and not checked for a pending
                     * cancel) even though there is a broken socket that
                     * another thread is trying to deactivate.  The
                     * testcancel will induce us to take the cancel that
                     * the thread trying to deactivate the socket sent
                     * us.
                     */
                    pthread_testcancel();
                }
            }
        }
    }
}

#ifdef ATFORK_SUPPORTED
/*
 * R P C _ _ N L S N _ F O R K _ H A N D L E R
 *
 * Handle fork related issues for this module.
 */

PRIVATE void rpc__nlsn_fork_handler
#ifdef _DCE_PROTO_
(
  rpc_listener_state_p_t  lstate,
  rpc_fork_stage_id_t stage
)
#else
(lstate, stage)
rpc_listener_state_p_t  lstate;
rpc_fork_stage_id_t stage;
#endif
{ 
    unsigned32 st;

    switch ((int)stage)
    {
        case RPC_C_PREFORK:
            RPC_MUTEX_LOCK (lstate->mutex);
            listener_thread_was_running = false;
            if (listener_thread_running)
            {
                listener_should_handle_cancels = false;
                pthread_cancel(listener_thread);
                RPC_MUTEX_UNLOCK (lstate->mutex);
                pthread_join(listener_thread, (pthread_addr_t *) &st);
                RPC_MUTEX_LOCK (lstate->mutex);
                pthread_detach(&listener_thread);
                listener_thread_was_running = true;
                listener_thread_running = false;
            }
            break;

        case RPC_C_POSTFORK_PARENT:
            if (listener_thread_was_running)
            {
                listener_thread_was_running = false;
                listener_thread_running = true;
                listener_should_handle_cancels = true;
		TRY {
                pthread_create (
                    &listener_thread,                   /* new thread    */
                    rpc_g_default_pthread_attr,         /* attributes    */
                    (pthread_startroutine_t) lthread,   /* start routine */
                    (pthread_addr_t) lstate);           /* arguments*/
		} CATCH_ALL {
		    /*
		     * rpc_m_call_failed
		     * "%s failed: %s"
		     */
		    RPC_DCE_SVC_PRINTF (( 
			DCE_SVC(RPC__SVC_HANDLE, "%s"), 
		        rpc_svc_threads, 
		        svc_c_sev_fatal | svc_c_action_abort, 
		        rpc_m_call_failed, 
		        "rpc__nlsn_fork_handler", 
		        "pthread_create failure")); 
		} ENDTRY
            }
            RPC_MUTEX_UNLOCK (lstate->mutex);
            break;

        case RPC_C_POSTFORK_CHILD:  
            /*
             * Unset the flag that says the listern thread has been started
             */
            listener_thread_was_running = false;
            listener_thread_running = false;
            /*
             * The mutex has already been destroyed.
             * RPC_MUTEX_UNLOCK (lstate->mutex);
             */
            break;
    }
}
#endif
