/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems
 * Burlington, MA, USA
 */
/*
 * HISTORY
 * $Log: crvb_cvx_002.c,v $
 * Revision 1.1.90.2  1996/02/17  23:30:07  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:46:52  marty]
 *
 * Revision 1.1.90.1  1995/12/11  22:58:53  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:11  root]
 * 
 * Revision 1.1.88.1  1994/04/01  20:09:23  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:13:32  jd]
 * 
 * Revision 1.1.86.1  1993/10/12  19:35:04  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:29:52  rhw]
 * 
 * Revision 1.1.7.2  1993/06/14  20:14:40  sommerfeld
 * 	Fix test by defining "resetup_interrupt" macro
 * 	that is called from interrupt handler to reinstall
 * 	handler, if necessary (for example, if setup_handler
 * 	uses signal to install handler).  This macro is
 * 	defined to be zero if reinstallation is not needed.
 * 	[1993/05/06  20:06:55  hopkins]
 * 
 * Revision 1.1.3.3  1992/12/30  16:29:59  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:06:45  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/29  20:37:53  devsrc
 * 	[OT 5373]  SNI/SVR4 Merge UPDATE: Fixed prototype headers to eliminate compile errors
 * 	           on AIX3.2.
 * 	[1992/09/24  20:01:32  sekhar]
 * 
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:32:01  sekhar]
 * 
 * Revision 1.1  1992/01/19  03:20:48  devrcs
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
 *  This software is furnished under a license and may be used and copied
 *  only in  accordance  with  the  terms  of  such  license and with the
 *  inclusion of the above copyright notice.  This software or  any other
 *  copies thereof may not be provided or otherwise made available to any
 *  other person.  No title to and ownership of  the  software  is hereby
 *  transferred.
 *
 *  The information in this software is subject to change  without notice
 *  and should  not  be  construed  as  a commitment by DIGITAL Equipment
 *  Corporation.
 *
 *  DIGITAL assumes no responsibility for the use or  reliability  of its
 *  software on equipment which is not supplied by DIGITAL.
 */

/*
 *  FACILITY:
 *
 *	CMA Test Programs
 *
 *  FILENAME:
 *
 *	CRVB_CVX_002
 *
 *  TEST TOPICS:
 *
 *	CVX  -  Condition Variable Operations
 *
 *  ABSTRACT:
 *
 *	This test ensures that the "signal-CV-from interrupt-code"
 *	service works properly, regardless of the thread in which
 *	the interrupt occurs.
 *
 *  AUTHORS:
 *
 *	Webb Scales
 *
 *  CREATION DATE:
 *
 *	04 September 1990
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	21 February 1991
 *		Don't reraise signal at interrupt level (but that's probably
 *		not the only reason it stopped working).
 *	002	Dave Butenhof	23 May 1991
 *		Fix compilation errors under new MIPS C compiler.
 *	003	Dave Butenhof	04 June 1991
 *		Try to fix segv in printf.
 *	004	Paul Curtin	06 June 1991
 *		Removed proto from macro setup_interrupt
 */


/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#if  _CMA_UNIX_TYPE == _CMA__SVR4
#include <dce/cma_defs.h>	/* get generic signal handler structs */
#endif
#include <dce/cts_test.h>
#if _CMA_OS_ == _CMA__UNIX
#include <signal.h>
#endif


#define	THREAD_CNT  8
#define	TEST_TIME   30.0


cma_t_thread	thread[THREAD_CNT];
cma_t_cond	cv[THREAD_CNT];
cma_t_mutex	mutex;
cma_t_integer	idx = 0;
cma_t_boolean	done = cma_c_false;


#if _CMA_OS_ == _CMA__UNIX
# define SIG_NUM	SIGUSR1

#if _CMA_UNIX_TYPE == _CMA__SVR4
	/*
	*	FIX-ME:
	*		It would be desirable to use sigaction(2) here, but
	*	the current implementation of DCE threads does not 
	*	allow asynchronous signals to be used with sigaction(2).
	*		Therefore we will use sigset(2) for now.
	*/
# define setup_interrupt() \
	if (sigset (SIG_NUM, (void (*)())interrupt) == SIG_ERR) cts_failed ("Error in 'sigset'")
# define resetup_interrupt()	0
#else
# define setup_interrupt()  \
	if (signal (SIG_NUM, (void (*)())interrupt) == SIG_ERR) cts_failed ("Error in 'signal'")
# define resetup_interrupt()  \
	if (signal (SIG_NUM, (void (*)())interrupt) == SIG_ERR) cts_failed ("Error in 'signal'")
#endif

# define cause_interrupt()  \
	if (kill (getpid (), SIG_NUM) == -1) cts_failed ("Error in 'kill'")
#endif

#if _CMA_OS_ == _CMA__VMS
# define setup_interrupt()	0
# define resetup_interrupt()	0

# define cause_interrupt()  \
	if (!(sys$dclast (interrupt, 0, 0) & 1)) cts_failed ("Error in $DCLAST")
#endif


void
interrupt
#   if _CMA_UNIX_TYPE == _CMA__SVR4
	(
	int					sig,
	cma__t_siginfo		siginfo,
	cma__t_sigcontext	*scp)
#   else
        (
	int			sig,
	int			siginfo,
	struct sigcontext	*scp)
#   endif
    {
    resetup_interrupt();
    cma_cond_signal_int (&cv[idx]);
    }


cma_t_address
thread_routine	(cma_t_address	parm)
    {
    cma_t_integer   id = (cma_t_integer)parm;
    cma_t_integer   cnt = 0;


    cma_mutex_lock (&mutex);
    while (cma_c_true) {
	while (idx != id && idx != -1)
	    cma_cond_wait (&cv[id], &mutex);

	/*
	 * Check for end-of-test
	 */
	if (idx == -1) break;

	/*
	 * Set to wake up the next thread and send an interrupt to do it.
	 */
	idx = (idx + 1) % THREAD_CNT;
	cause_interrupt ();
	cnt++;
	}
    cma_mutex_unlock (&mutex);

    return (cma_t_address)cnt;
    }


int
main(int argc, char *argv[], char *envp[])
    {
    cma_t_integer   i, j;
    cma_t_address   item;
    cma_t_exit_status	exit_status;
    cma_t_address   result;


    cts_test ("CRVB_CVX_002", "Test signal-CV-from-interrupt-code service");
    cma_init ();

    setup_interrupt();
    cma_mutex_create (&mutex, &cma_c_null);

    for (i = 0; i < THREAD_CNT; i++)
	cma_cond_create (&cv[i], &cma_c_null);

    for (i = 0; i < THREAD_CNT; i++)
	cma_thread_create (
		&thread[i],
		&cma_c_null,
		thread_routine,
		(cma_t_address)i);

    printf ("Starting %f delay...", TEST_TIME);
    cma_delay (TEST_TIME);

    /*
     * Set end-of-test flag
     */
    printf ("Finishing...");
    cma_mutex_lock (&mutex);
    idx = -1;
    cma_mutex_unlock (&mutex);

    printf ("Joining...\n");
    for (i = 0; i < THREAD_CNT; i++) {
	/*
	 * Wake this thread, in case it is sleeping, and then wait for it to
	 * terminate.
	 */
	cma_cond_signal (&cv[i]);
	cma_thread_join (
		&thread[i],
		&exit_status,
		&result);

	if (exit_status != cma_c_term_normal)
	    cts_failed ("Bad exit status for thread #%d.", i);

	printf ("Thread #%d sent %d signals\n", i, (cma_t_integer)result);
	}


    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_CVX_002.C */
/*  *7     6-JUN-1991 13:53:54 CURTIN "removed proto from macro" */
/*  *6     4-JUN-1991 13:18:50 BUTENHOF "Experiment on OSF/1" */
/*  *5    29-MAY-1991 18:01:56 BUTENHOF "Fix compilation errors" */
/*  *4    21-FEB-1991 21:50:40 BUTENHOF "more of same" */
/*  *3    21-FEB-1991 18:11:49 BUTENHOF "More fixes" */
/*  *2    21-FEB-1991 01:14:32 BUTENHOF "Don't reraise signal at interrupt level!" */
/*  *1     4-SEP-1990 16:14:41 SCALES "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_CVX_002.C */
