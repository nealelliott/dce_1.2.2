/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cma_host.c,v $
 * Revision 1.1.8.1  1996/05/10  13:26:36  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:49 UTC  bissen
 * 	Merge Spec 1170 changes for Walnut Creek.
 * 	[1995/07/10  20:08 UTC  jss  /main/HPDCE02/4]
 *
 * 	Change symbol __hpux1010 to hpux1010p.
 * 	[1995/07/10  19:54 UTC  jss  /main/HPDCE02/jss_mothra_cose/2]
 *
 * 	Fix for CHFts15603. Signal changes to support WC kernel Spec. 1170 changes.
 * 	[1995/06/28  17:22 UTC  jss  /main/HPDCE02/jss_mothra_cose/1]
 *
 * 	Merge singlestack code.
 * 	[1994/10/27  21:06 UTC  chris_p  /main/HPDCE02/3]
 *
 * 	Change symbol names with "carevue" to "singlestack"
 * 	[1994/10/17  20:30 UTC  chris_p  /main/HPDCE02/proven_threads_940803/2]
 *
 * 	Project carevue.
 * 	Threads now use only one stack.
 * 	Stacks are copied at context switch time.
 * 	[1994/09/06  22:52 UTC  chris_p  /main/HPDCE02/proven_threads_940803/1]
 *
 * 	fixed merge problem with hpdce02 merge.
 * 	[1994/07/07  16:08 UTC  jss  /main/HPDCE02/2]
 *
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:47 UTC  jss  /main/HPDCE02/1]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  18:30  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/20  14:04:48  truitt]
 *
 * 	CR10892 - fix copyright in file
 * 	[1994/06/09  13:25:22  devsrc]
 * 	 *
 *
 * 	First drop for code cleanup
 * 	[1994/03/28  19:30:33  jd]
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:41:34  root  1.1.9.3]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  17:29:55  root]
 *
 * 	Gamera merge
 * 	[1993/03/26  23:53:29  hopkins]
 *
 * 	Loading drop DCE1_0_2B18
 * 	[1993/03/11  22:24:15  osfrcs  1.1.1.8]
 *
 * 	DCE 1.0.1 Release
 * 	[1992/09/23  20:36:40  hopkins]
 *
 * Revision 1.1.4.2  1994/06/09  13:34:24  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:25:22  devsrc]
 * 
 * Revision 1.1.4.1  1994/04/01  20:13:35  jd
 * 	First drop for code cleanup
 * 	[1994/03/28  19:30:33  jd]
 * 
 * Revision 1.1.2.3  1992/12/15  22:21:03  alan
 * 	Insert copyright notices
 * 	[1992/12/07  15:59:31  alan]
 * 
 * Revision 1.1.2.2  1992/10/03  19:46:22  sommerfeld
 * 	Add implementation of getdtablesize() in terms of getrlimit().
 * 	[1992/09/09  20:28:54  sommerfeld]
 * 
 * Revision 1.1  1992/01/19  14:40:04  devrcs
 * 	Initial revision
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
#include <cma_util.h>
#include <cma_alert.h>
#include <cma_defer.h>
# if !_CMA_THREAD_SYNC_IO_
#  include <cma_thread_io.h>
# endif
# include <sys/time.h>


/*
 * GLOBAL DATA
 */

/* Indicates what, if any, action the timer procedure wants to take after
   it has processed a timer signal.  */
extern int		cma__g_timer_action;	 
#ifndef _HP_CMA_PERF_
extern int      cma__ck_io_at_yield;
#endif /* _HP_CMA_PERF_ */

#ifdef HP_CMA_SINGLESTACK_OPT
extern int cma_singlestack_opt;
#endif

/*??? make sure host-specific interfaces are listed in cma_host_hp9000s800.h */

#define SAVE_CTX(ctx) { \
}

/*??? inhibit around restore? */
#define RESTORE_CTX(ctx) { \
}

/*
 * Function Prototypes
 */

extern void cma__periodic ();

void cma__create_thread (cma__t_static_ctx *, void *, cma__t_int_tcb *, void*	(*func)(), void	*arg);


void cma__create_thread ( cma__t_static_ctx *ctx, void *stk, cma__t_int_tcb *tcb, void*	(*func)(), void	*arg)
{
    /*** Init static context */
    ctx->start_routine =          func;
    ctx->arg =                    arg;

    cma__enter_kernel();
#ifdef HP_CMA_SINGLESTACK_OPT
	if (cma_singlestack_opt) 
		cma_singlestack_init_thread_stack(stk, tcb, ctx);
	else
#endif
    	cma__init_thread_stack(stk, tcb, ctx);
    cma__start_thread(tcb);
    cma__exit_kernel();
}


void cma__thread_start1 (tcb)
cma__t_int_tcb 		*tcb;
{
    cma__t_static_ctx *ctx = &tcb->static_ctx;
    
    /*** Set pm state which is thread specific */
    RESTORE_CTX(ctx);

    cma__thread_base(tcb, ctx->start_routine, ctx->arg);

    /*** Never get here, cma__thread_base doesn't return. */
}

void cma__restore_thread_ctx (new_ctx)
cma__t_static_ctx 	*new_ctx;
{
    /*** Switch to new thread, abandoning current stack */
    cma__restore_regs(new_ctx);
}

void cma__transfer_thread_ctx (cur_ctx, new_ctx)
cma__t_static_ctx 	*cur_ctx;
cma__t_static_ctx 	*new_ctx;
{
    /*** Has the stack overflowed??? */

    /*** Save the fp state??? */

    /*** Save pm state which is thread specific */
    SAVE_CTX(cur_ctx);

    /*** Switch to new thread */
    cma__transfer_regs(cur_ctx, new_ctx);

    /*** Restore pm state which is thread specific */
    RESTORE_CTX(cur_ctx);

    /*** Restore fp state??? */
}

void cma__hppa_timer_base (sig, code, scp)
    int		        sig;
    int		        code;
    struct sigcontext   *scp;

{
    int			current_action;
#if __hpux1010p
    sigset_t		old_sig_mask;
#else
    long		old_sig_mask;
#endif
    
    /* Clear the "timer action" flag, just in case the application
       has written over it. */
    cma__g_timer_action = cma__c_hppa_timer_no_action;    

    /* Call cma__periodic, where the real work gets done. */
    cma__periodic (sig, code, scp);
    
    /* If cma__periodic decided to take action on this thread it
       will set cma__g_timer_action accordingly. */
    switch (current_action = cma__g_timer_action) {
	
    case cma__c_hppa_timer_no_action:
	break;
    case cma__c_hppa_timer_yield:
    case cma__c_hppa_timer_alert:
	/* Unlike other CMA implementations, we cannot simply return back to
	   the interrupted thread's context with a new pc value which
	   will force the thread to yield the processor.  Instead we will
	   set the signal mask back to its pre-timer interrupt state, and
	   call cma__yield in this procedure.  When the thread gets its 
	   next cpu time slice, it will return here where we will doctor the
	   signal mask in the scp variable to be the same as the current
	   mask value and then do the interrupt return. */
	
#if __hpux1010p
	if (sigprocmask (SIG_SETMASK, &scp->sc_sx.sl_uc.uc_sigmask,
			 &old_sig_mask) == -1)
	    cma__bugcheck ("cma__hppa_timer_base:1");
#else
	old_sig_mask = sigsetmask (scp->sc_mask);
#endif  /* if __hpux1010p */

	if (current_action == cma__c_hppa_timer_yield) {
#  ifndef _HP_CMA_PERF_
	    cma__ck_io_at_yield = 1;
#  endif /* _HP_CMA_PERF_ */
	    cma__yield_processor ();
	    cma__undefer ();
	    cma__g_kernel_critical = 0;
	}
	else {
	    /* Process the asynchronous signal. */
	    cma__undefer ();
	    cma__g_kernel_critical = 0;
	    cma__async_delivery ();
	    
	    /* This version of CMA threads should never return from the
	       signal handling. */
	    cma__bugcheck ("host: Invalid return from signal handling");
	}
	
#if __hpux1010p
	if (sigprocmask (SIG_SETMASK, (sigset_t *)cma_c_null_ptr,
			 &scp->sc_sx.sl_uc.uc_sigmask) == -1)
	    cma__bugcheck ("cma__hppa_timer_base:2");
#else
	scp->sc_mask = sigblock (0L);
#endif  /* if __hpux1010p */
	break;
    default:
	cma__bugcheck ("host: Unknown action");
	break;
    }
    /* Exit back into the timer signal handler which will restore ALL the
       registers of the thread (including PC, SRs, CRs). */
}

#include <sys/resource.h>

int getdtablesize()
{
    struct rlimit rl;
    getrlimit(RLIMIT_NOFILE, &rl);
    return rl.rlim_cur;
}


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
	     * Cause cma___timer_base to process alert.
	     */
	    cma__g_timer_action = cma__c_hppa_timer_alert;

	    /*
	     * We've modified the return address for the timer routine so we
	     * will enter the assembly code for a asynchronous dispatch.
	     * This code needs to have the kernel locked, so return without
	     * executing the unset kernel_critical code at the end of the
	     * routine!
	     */
	    return;
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
	     * Cause cma___timer_base to yield processor.
	     */
	    cma__g_timer_action = cma__c_hppa_timer_yield;

	    /*
	     * We've modified the return address for the timer routine so we
	     * will enter the assembly code for a asynchronous dispatch. This
	     * code needs to have the kernel locked, so return without
	     * executing the unset kernel_critical code at the end of the
	     * routine!
	     */
	    return;
	    }

	/*
	 * Since we locked the kernel, unlock it before we leave. Do not
	 * perform "undefer" actions since they might involve signalling a
	 * condition variable, and we are running at interrupt level.
	 */
	cma__unset_kernel ();
	}

    }

