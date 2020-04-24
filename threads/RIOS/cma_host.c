/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 *
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems
 * Burlington, MA, USA
 */
/*
 * HISTORY
 * $Log: cma_host.c,v $
 * Revision 1.1.4.2  1996/02/17  23:33:04  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:49:22  marty]
 *
 * Revision 1.1.4.1  1995/12/07  21:50:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:00:51  root]
 * 
 * Revision 1.1.2.2  1994/06/09  13:35:34  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:26:06  devsrc]
 * 
 * Revision 1.1.2.1  1994/04/01  20:14:10  jd
 * 	Moved from src/threads to here
 * 	[1994/04/01  17:52:33  jd]
 * 
 * $EndLog$
 */



#include <cma.h>
#include <cma_defs.h>
#include <cma_timer.h>
#include <cma_tcb.h>
#include <cma_dispatch.h>
#include <cma_stack.h>
#include <cma_condition.h>
#include <cma_semaphore.h>
#include <cma_assem.h>
#include <cma_util.h>
#include <cma_alert.h>
#include <cma_defer.h>
#include <cma_thread_io.h>
#include <sys/time.h>

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Periodic dispatch timer
 *
 *  FORMAL PARAMETERS:
 *
 *	sig	- The value of the signal (the signal number)  [not used]
 *	code	- Signal argument (for system signals)  [not used]
 *	scp	- The address of a signal context structure  (The value in the
 *		    process signal mask field is saved in the TCB in the event
 *		    of a preemption.)
 *
 *  IMPLICIT INPUTS:
 *
 *	The timer queue.
 *
 *  IMPLICIT OUTPUTS:
 *
 *      none
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	This routine will reactivate itself
 *
 */
extern void
cma__periodic
	(int			sig,
	int			siginfo,
	struct sigcontext	*scp)
    {
    cma__t_int_tcb  *cur_tcb;
    cma_t_boolean   lock;


    /*
     * Try to lock the kernel, and remember whether it was already locked
     * (the value of "lock" is the previous setting of the flag). Even if
     * we're in a non-uniprocessor, we still need to test and save rather
     * than using cma__enter_kernel(), since the timer might have caught the
     * the thread we're running in while it had locked the kernel; and
     * waiting for it to release still wouldn't work very well.
     */
    lock = cma__tryenter_kernel ();

    /*
     * On uniprocessor configurations, getting the current tcb will never
     * cause an exception (since it is stored in a variable), so the TRY
     * block and the error handling are superfluous.
     */
    cur_tcb = cma__get_self_tcb ();


    /*
     * Check if any timer has elapsed, and, on Unix, check for pending I/O
     * operations which are now ready.
     */
    if (!lock) {
	static struct timeval polling_timeout = {0, 0};


	/* 
	 * The first two parameters are arbitrary; poll (null timeout).
	 */
	(void)cma__io_available (
		cma__c_io_read, 
		0, 
		&polling_timeout);
	(void)cma__check_timer_queue ((cma_t_date_time *)cma_c_null_ptr);
	}

    /*
     * In the current thread, decrement the quanta-remaining. 
     * Force a dispatch if it falls to zero.
     */
    if (cur_tcb->kind != cma__c_thkind_null)
        --cur_tcb->timer.quanta_remaining;

    if (!lock) {

	if (cur_tcb->alert.pending
		&& cur_tcb->alert.a_enable
		&& cur_tcb->alert.g_enable
	    ) {
	    /*
	     * Save the address of the signal mask.  This is necessary in
	     * order to "dismiss" the interrtupt, and to restor
	     */

	    cur_tcb->async_ctx.interrupt_ctx = (cma_t_address)&scp->sc_mask;
	    /*
	     * Dismiss the interrupt and exit the kernel.  Now everything
	     * is "normal", so just request that the alert be delivered.
	     * We know that it is pending, it should cause this frame to
	     * be popped; bugcheck if it doesn't.
	     */
	    cma__clear_interrupt (cur_tcb->async_ctx.interrupt_ctx);
	    cma__unset_kernel ();
	    cma__attempt_delivery (cur_tcb);
	    cma__bugcheck ("cma__periodic: return from attempt-delivery");
	    }

	/*
	 * If the current thread has run out of quanta, then we preempt it.
	 * Note that we do so even if there are no other threads ready to
	 * run! This is a very important feature: the "signal from interrupt"
	 * function is implemented by inserting an item on a queue of fixed
	 * length (300 entries by default). If this queue overflows,
	 * DECthreads will complain by raising cma_e_signal_q_full
	 * (pthread_signal_q_full_e). Because the defer queue is processed on
	 * a preemption, this "unnecessary" preemption when only one thread
	 * is ready creates a bound on the amount of time that the queue can
	 * remain unprocessed, limiting the chance that it will overflow.
	 */
	if (((cur_tcb->timer.quanta_remaining <= 0) && (!cur_tcb->sched.rtb))
		|| (cur_tcb->kind == cma__c_thkind_null)
	    ) {
	    /*
	     * Save the address of the signal mask.  This is necessary in
	     * order to "dismiss" the interrtupt, and to restor
	     */
	    cur_tcb->async_ctx.interrupt_ctx = (cma_t_address)&scp->sc_mask;
	    /*
	     * Note the fact that this is an async context switch and then
	     * simply yield.  The dispatch code will deal with the interrupt.
	     */
	    cur_tcb->async_ctx.valid = cma_c_true;
	    cma__undefer ();
	    cma__yield_processor ();
	    cur_tcb->async_ctx.valid = cma_c_false;
	    }

	/*
	 * Since we locked the kernel, unlock it before we leave. Do not
	 * perform "undefer" actions since they might involve signalling a
	 * condition variable, and we are running at interrupt level.
	 */
	cma__unset_kernel ();
	}

    }
