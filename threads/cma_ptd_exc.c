/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cma_ptd_exc.c,v $
 * Revision 1.1.12.2  1996/06/04  22:16:23  arvind
 * 	Merge change to DCE_1.2
 * 	[1996/05/01  15:40 UTC  bissen  /main/DCE_1.2/2]
 *
 * 	Add cma__is_multithreaded to ptdexc_yield
 * 	[1996/05/01  15:34 UTC  bissen  /main/DCE_1.2/bissen_122_work/1]
 *
 * 	Update OSF copyright year
 * 	[1996/02/17  22:50:18  marty]
 *
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:06:31  root]
 *
 * Revision 1.1.12.1  1996/05/10  13:39:51  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 * 
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:43 UTC  bissen
 * 	Merge in changes to CHFts14939 and CHFts14807 from branch jss_mothra_fix3.
 * 	[1995/04/26  20:03 UTC  jss  /main/HPDCE02/4]
 * 
 * 	Fix for CHFts14807. Fix ptdexc_setscheduler() to support SCHED_BG_NP.
 * 	[1995/04/10  18:34 UTC  jss  /main/HPDCE02/jss_mothra_fix3/1]
 * 
 * 	Merge from branch jss_single_threaded.
 * 	[1994/12/23  15:56 UTC  jss  /main/HPDCE02/3]
 * 
 * 	Change for single threaded client. pthread_yield should just return if we are not
 * 	multi-threaded.
 * 	[1994/12/23  15:52 UTC  jss  /main/HPDCE02/jss_single_thread/2]
 * 
 * 	Merge from branch jss_single_thread.
 * 	[1994/12/22  19:58 UTC  jss  /main/HPDCE02/2]
 * 
 * 	Changes for single threaded client. Added routine cma__int_init2() to
 * 	ptdexc_create.
 * 	[1994/12/15  15:42 UTC  jss  /main/HPDCE02/jss_single_thread/1]
 * 
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:43 UTC  jss  /main/HPDCE02/1]
 * 
 * 	Merge for RODAN cycle T bsu
 * 	[1994/06/16  17:25 UTC  jss  /main/HPDCE01/2]
 * 
 * 	If ptdexc_mutex_lock() blocks, another thread could have destroyed the mutex variable
 * 	before it returns.  This change tests the mutex handle after returning from
 * 	blocking to see if it is still valid.
 * 	[1994/06/16  15:54 UTC  jss  /main/HPDCE01/jss_rodan_r/1]
 * 
 * 	merge kk and hpdce01
 * 	[1994/01/24  16:40  truitt  /main/HPDCE01/1]
 * 
 * 	Change polarity of ifdef for including cma performance fixes
 * 	[1993/10/29  18:44:12  ajayb]
 * 	 *
 * 
 * 	Add proper ifdefs to include DEPT changes.
 * 	[1993/10/19  19:37:45  ajayb]
 * 
 * 	Merging DEPT changes
 * 	[1993/10/18  20:14:53  ajayb]
 * 
 * 	Merging DEPT changes
 * 	[1993/10/18  14:38:24  prepetit]
 * 
 * 	Merging DEPT changes
 * 	[1993/09/30  15:11:28  prepetit]
 * 
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:43:54  root  1.1.3.3]
 * 
 * Revision 1.1.8.2  1994/06/09  13:38:24  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:16  devsrc]
 * 
 * Revision 1.1.8.1  1994/04/01  20:16:53  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:18:15  jd]
 * 
 * Revision 1.1.5.4  1993/01/26  13:26:22  jd
 * 	Check for invalid timespec argument in pthread_cond_timedwait. This argument
 * 	is worth the time it takes since a bad value will probably result in very
 * 	mysterious errors from select. All of the paths into the threads package
 * 	except this one and pthread_cond_timedwait were already doing the check.
 * 	[1993/01/23  22:47:42  jd]
 * 
 * Revision 1.1.5.3  1992/12/15  22:29:18  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:40:15  alan]
 * 
 * Revision 1.1.5.2  1992/09/03  14:39:05  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:05:18  bolinger]
 * 
 * Revision 1.1.2.3  1992/08/14  20:21:52  bolinger
 * 	Bring forward 1.0.1 revision 1.1.3.3 (fix for OT defect 2994).
 * 	Also, map CMA-internal scheduling classes to SCHED_OTHER.
 * 	[1992/08/10  16:05:15  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/08  23:37:59  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:37:38  bolinger]
 * 
 * Revision 1.1.3.3  1992/05/15  15:30:28  bolinger
 * 	Integrate with scheduling class changes.
 * 	[1992/05/15  14:38:34  bolinger]
 * 
 * 	Fix OT defect 2994:  Change the types of the entry points in the
 * 	exception-raising Pthreads interface to match those of the
 * 	error-returning interface.  This, alas, also involves adding
 * 	mostly-useless "return(0);" statements to the functions so they can be
 * 	used at least somewhat transparently to replace their error-returning
 * 	kin.
 * 
 * 	The motivation behind this is to realize the claim in the App Dev Guide
 * 	that one can substitute either of these interfaces for the other just
 * 	by changing one #include line.
 * 	[1992/05/14  01:05:15  bolinger]
 * 
 * Revision 1.1.3.2  1992/05/12  23:41:03  keane
 * 	Address defects 2666 and 2622.  Add support for scheduling policies
 * 	SCHED_FG_NP and SCHED_BG_NP.
 * 	[1992/05/12  20:26:40  keane]
 * 
 * Revision 1.1  1992/01/19  14:41:04  devrcs
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
 *	Implement POSIX 1003.4 draft thread operations on top of CMA (from
 *	9 January 1990 draft).
 *
 *	ptdexc_*  routines (Exception generating).
 *
 *
 *  AUTHORS:
 *
 *	Paul Curtin
 *
 *  CREATION DATE:
 *
 *	28 August 1990
 *
 *  MODIFICATION HISTORY:
 * 
 *	001	Paul Curtin	13 September 1990
 *		Changing ftime to cma__ftime.
 *	002	Dave Butenhof	8 October 1990
 *		Change cma_once call to include new argument. Also clean up
 *		code some and remove unnecessary calls to cma_init().
 *	003	Paul Curtin	21 November 1990
 *		Switched the order of the arguments to pthread_keycreate.
 *	004	Dave Butenhof	4 February 1991
 *		Change names from "pthread_<foo>_e" to "ptdexc_<foo>" to
 *		avoid conflicts with new pthread exception aliases.
 *	005	Dave Butenhof	7 February 1991
 *		Adapt to new alert bit names.
 *	006	Dave Butenhof	21 March 1991
 *		Implement ptdexc_cond_signal_int_np().
 *	007	Dave Butenhof	21 March 1991
 *		Raise pthread_badparam_e on bad cancel state input
 *	008	Dave Butenhof	26 March 1991
 *		Add pthread_signal_to_cancel_np() for OSF DCE
 *	009	Dave Butenhof	01 May 1991
 *		Add arguments to cma__bugcheck() calls.
 *	010	Paul Curtin	04 May 1991
 *		Replace cma_init() with cma__int_init()
 *	011	Paul Curtin	13 May 1991
 *		Replaced a number of external interface calls
 *		with internal calls and macros. (Optimizations.)
 *	012	Dave Butenhof and Webb Scales	05 June 1991
 *		Include cma_px.h to get struct timespec definition.
 *	013	Paul Curtin	2 Aug 1991
 *		Added init check to ptdexc_lock_global_np
 *	014	Dave Butenhof	27 August 1991
 *		Fix bug in join: it should accept NULL status argument.
 *	015	Dave Butenhof	07 October 1991
 *		Add pthread_attr_setguardsize_np &
 *		pthread_attr_getguardsize_np functions.
 *	016	Paul Curtin	20 November 1991
 *		Alpha port: a number of platform specific changes.
 *	017	Dave Butenhof	05 December 1991
 *		Fix pthread_attr_setsched to support background policy!
 *	018	Dave Butenhof	10 February 1992
 *		A law of nature has just been changed: cma__alloc_mem now
 *		returns cma_c_null_ptr on an allocation failure, rather than
 *		raising an exception. This allows greater efficiency, since
 *		TRY is generally expensive. Anyway, apply the process of
 *		evolution: adapt or die.
 *	019	Dave Butenhof	09 March 1992
 *		The mutex block/unblock functions have just been changed to
 *		return status values. Change pthread mutex lock/unlock to
 *		call this directly instead of using cma__int_[un]lock and
 *		convert status, rather than using expensive TRY/CATCH.
 *	020	Dave Butenhof	10 March 1992
 *		Change 'timeb' to 'timeval' so we can use gettimeofday rather
 *		than the BSD-only ftime function.
 *	021	Dave Butenhof	12 March 1992
 *		Clean up 019 a little.
 *	022	Dave Butenhof	12 March 1992
 *		More integration of direct code: pthread_self,
 *		pthread_getspecific, pthread_setspecific.
 */


/*
 *  INCLUDE FILES
 */

#include <cma.h>
#include <cma_px.h>			/* Get struct timespec */
#include <cma_handle.h>
#include <cma_util.h>
#include <cma_init.h>
#include <pthread_exc.h>
#if _CMA_OS_ == _CMA__VMS
# include <time.h>
#else
# include <sys/time.h>
#endif
#include <cma_tcb_defs.h>
#include <cma_attr.h>
#include <cma_alert.h>
#include <cma_mutex.h>
#include <cma_stack.h>
#include <cma_condition.h>
#include <cma_int_errno.h>
#include <cma_context.h>
#include <cma_vm.h>

#if _CMA_OS_ == _CMA__VMS
#include <lib$routines.h>
#include <libdtdef.h>
#endif

#if _CMA_OS_ == _CMA__UNIX
# include <signal.h>
#endif

/*
 * GLOBAL DATA
 */

#if _CMA_OS_ == _CMA__UNIX
 extern cma_t_boolean	cma__g_handle_active;
 extern cma_t_thread	cma__g_handle_thread, cma__g_handle_target;
 extern sigset_t	cma__g_handle_sigset;
 extern void pthread__cancel_thread (void);
#endif

/*
 * LOCAL DATA
 */

#if _CMA_OS_ == _CMA__VMS
/*
 * Conversion: VMS absolute time of UNIX "epoch", 00:00:00 Jan 1, 1970
 */
static cma_t_date_time pthread___g_epoch = {0x4BEB4000, 0x007C9567};
#endif

/*
 * LOCAL MACROS
 */

/*
 * LOCAL FUNCTIONS
 */

static void
pthread___unlock_int_mutex (pthread_addr_t);


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Create a thread attribute object
 *
 *  FORMAL PARAMETERS:
 *
 *	attr		Return attribute object handle
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	Zero (or none if exception raised)
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
ptdexc_attr_create(
	pthread_attr_t	*attr)
    {
    cma__t_int_attr     *int_att;


    cma__int_init ();
    int_att = cma__get_attributes (&cma__g_def_attr);

    if ((cma_t_address)int_att == cma_c_null_ptr)
	cma__error (exc_s_insfmem);
    else
	cma__object_to_handle ((cma__t_object *)int_att, (cma_t_attr *)attr);
	return (0);

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Delete a thread attribute object
 *
 *  FORMAL PARAMETERS:
 *
 *	attr		Attribute object handle
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	Zero (or none if exception raised)
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
ptdexc_attr_delete(
	pthread_attr_t	*attr)
    {
    cma__t_int_attr     *int_att;


    int_att = cma__validate_null_attr ((cma_t_attr *)attr);

    if (int_att == (cma__t_int_attr *)cma_c_null_ptr)
	return;

    cma__free_attributes (int_att);
    cma__clear_handle (attr);
	return (0);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Set priority attribute
 *
 *  FORMAL PARAMETERS:
 *
 *	attr		Attribute object
 *	priority	new priority
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	Zero (or none if exception raised)
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
ptdexc_attr_setprio(
	pthread_attr_t	*attr,
	int		priority)
    {
    cma_attr_set_priority (attr, (cma_t_priority)priority);
	return (0);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Get priority attribute
 *
 *  FORMAL PARAMETERS:
 *
 *	attr		Attribute object
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	priority attribute
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
ptdexc_attr_getprio(
	pthread_attr_t	attr)
    {
    int                 ret_val = 0;
    cma_t_priority	priority;


    cma__int_attr_get_priority (&attr, &priority);
    ret_val = (int)priority;

    return ret_val;
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Set scheduler attribute
 *
 *  FORMAL PARAMETERS:
 *
 *	attr		Attribute object
 *	scheduler	new scheduler
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	Zero (or none if exception raised)
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
ptdexc_attr_setsched(
	pthread_attr_t	*attr,
	int		scheduler)
    {
    cma_t_sched_policy	policy;
    cma_t_priority	priority;


    /*
     * Hopefully, POSIX 1003.4 pthread_attr_setsched will be redefined to
     * accept both policy and priority.  Since CMA has already addressed this
     * problem, this emulation needs to come up with a valid priority in the
     * new policy: for lack of any better ideas, simply use the midrange.
     */
    switch (scheduler) {
	case SCHED_FIFO : {
	    policy = cma_c_sched_fifo;
	    priority = cma_c_prio_fifo_mid;
	    break;
	    }
	case SCHED_RR : {
	    policy = cma_c_sched_rr;
	    priority = cma_c_prio_rr_mid;
	    break;
	    }
	case SCHED_FG_NP : {
	    policy = cma_c_sched_throughput;
	    priority = cma_c_prio_through_mid;
	    break;
	    }
	case SCHED_BG_NP : {
	    policy = cma_c_sched_background;
	    priority = cma_c_prio_back_mid;
	    break;
	    }
	default : {
	    break;
	    }

	}

    cma_attr_set_sched (attr, policy, priority);
	return (0);

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Get scheduler attribute
 *
 *  FORMAL PARAMETERS:
 *
 *	attr		Attribute object
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	scheduling policy attribute
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
ptdexc_attr_getsched(
	pthread_attr_t	attr)
    {
    int			ret_val = 0;
    cma_t_sched_policy	scheduler;


    cma__int_attr_get_sched (&attr, &scheduler);

    switch (scheduler) {
	case cma_c_sched_fifo : {
	    ret_val = SCHED_FIFO;
	    break;
	    }
	case cma_c_sched_rr : {
	    ret_val = SCHED_RR;
	    break;
	    }
	case cma_c_sched_throughput : {
	    ret_val = SCHED_FG_NP;
	    break;
	    }
	case cma_c_sched_background : {
	    ret_val = SCHED_BG_NP;
	    break;
	    }
	case cma_c_sched_ada_low : {
	    ret_val = SCHED_OTHER;
	    break;
	    }
	case cma_c_sched_idle : {
	    ret_val = SCHED_OTHER;
	    break;
	    }
	default : {
	    cma__bugcheck ("attr_getsched:1");
	    break;
	    }
	}

    return ret_val;
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Set inherit scheduling attribute
 *
 *  FORMAL PARAMETERS:
 *
 *	attr		Attribute object
 *	inherit		new inherit scheduling attribute
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	Zero (or none if exception raised)
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
ptdexc_attr_setinheritsched(
	pthread_attr_t	*attr,
	int		inherit)
    {
    cma_t_sched_inherit	flag;


    switch (inherit) {
	case PTHREAD_INHERIT_SCHED : {
	    flag = cma_c_sched_inherit;
	    break;
	    }
	case PTHREAD_DEFAULT_SCHED : {
	    flag = cma_c_sched_use_default;
	    break;
	    }
	default : {
	    break;
	    }
	}

    cma_attr_set_inherit_sched (attr, flag);
	return (0);

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Get inherit scheduling attribute
 *
 *  FORMAL PARAMETERS:
 *
 *	attr		Attribute object
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	inherit scheduling info attribute
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
ptdexc_attr_getinheritsched(
	pthread_attr_t	attr)
    {
    int			ret_val = 0;
    cma_t_sched_inherit	inherit;


    cma__int_attr_get_inherit_sched (&attr, &inherit);

    switch (inherit) {
	case cma_c_sched_inherit : {
	    ret_val = PTHREAD_INHERIT_SCHED;
	    break;
	    }
	case cma_c_sched_use_default : {
	    ret_val = PTHREAD_DEFAULT_SCHED;
	    break;
	    }
	default : {
	    break;
	    }
	}

    return (int)ret_val;
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Set stack size attribute
 *
 *  FORMAL PARAMETERS:
 *
 *	attr		Attribute object
 *	stacksize	new stack size
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	Zero (or none if exception raised)
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
ptdexc_attr_setstacksize(
	pthread_attr_t	*attr,
	long		stacksize)
    {
    cma__int_attr_set_stacksize (attr, (cma_t_natural)stacksize);
	return (0);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Get stack size attribute
 *
 *  FORMAL PARAMETERS:
 *
 *	attr		Attribute object
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	stack size attribute
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern long
ptdexc_attr_getstacksize(
	pthread_attr_t	attr)
    {
    int			ret_val = 0;
    cma_t_natural	stacksize;


    cma__int_attr_get_stacksize (&attr, &stacksize);
    ret_val = (long)stacksize;

    return (long)ret_val;
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Set guard size attribute
 *
 *  FORMAL PARAMETERS:
 *
 *	attr		Attribute object
 *	guardsize	new guard size
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	Zero (or none if exception raised)
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
ptdexc_attr_setguardsize_np(
	pthread_attr_t	*attr,
	long		guardsize)
    {
    cma__int_attr_set_guardsize (attr, (cma_t_natural)guardsize);
	return (0);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Get guard size attribute
 *
 *  FORMAL PARAMETERS:
 *
 *	attr		Attribute object
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	guard size attribute
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern long
ptdexc_attr_getguardsize_np(
	pthread_attr_t	attr)
    {
    int			ret_val = 0;
    cma_t_natural	guardsize;


    cma__int_attr_get_guardsize (&attr, &guardsize);
    ret_val = (long)guardsize;

    return (long)ret_val;
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Create a thread object
 *
 *  FORMAL PARAMETERS:
 *
 *	thread		Return new thread handle
 *	attr		Attribute object handle
 *	start_routine	Thread's start routine
 *	arg		Thread's argument
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	Zero (or none if exception raised)
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
ptdexc_create(
	pthread_t		*thread,
	pthread_attr_t		attr,
	pthread_startroutine_t	start_routine,
	pthread_addr_t		arg)
    {
    cma__int_init ();
    /*
     * We don't initialize all of CMA until necessary.  cma__int_init2() is
     * not called until we need to be multi-threaded.
     */
    cma__int_init2 ();
    cma_thread_create (thread, &attr, start_routine, arg);
	return (0);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Join with a thread (wait for it to terminate)
 *
 *  FORMAL PARAMETERS:
 *
 *	thread		The thread to join with
 *	status		Return thread's status
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	Zero (or none if exception raised)
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
ptdexc_join(
	pthread_t		thread,
	pthread_addr_t		*status)
    {
    cma_t_exit_status	thdstatus;
    cma_t_address	result;


    /*
     * CMA thread exit results in two separate values: a "result", and an
     * "exit status".  Pthread exit status combines these two into a single
     * value, with the value -1 corresponding to an error and other values
     * corresponding to a successful return.  Therefore, translate an error
     * status into a value of -1; otherwise return the thread's actual value.
     * The emulation of pthread_exit effectively reverses this mapping to
     * ensure that the correct value is returned.
     */
    cma_thread_join (&thread, &thdstatus, &result);

    if (status != (pthread_addr_t *)cma_c_null_ptr) {
	if (thdstatus == cma_c_term_normal)
	    *status = (pthread_addr_t)result;
	else
	    *status = (pthread_addr_t)-1;
	}
	return (0);

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Detach a thread (declare that its storage may be reclaimed when it
 *	has terminated: it will not be joined with).
 *
 *  FORMAL PARAMETERS:
 *
 *	thread		The thread to detach
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	Zero (or none if exception raised)
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
ptdexc_detach(
	pthread_t		*thread)
    {
    cma_thread_detach (thread);
	return (0);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Terminate the thread, returning status to joiner(s).
 *
 *  FORMAL PARAMETERS:
 *
 *	status		The status/result to return.
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	Terminates the thread: this operation does not return to the caller.
 */
extern void
ptdexc_exit(
	pthread_addr_t	status)
    {

    /*
     * Since CMA specifies separate exit operations depending on whether the
     * thread completed successfully or not, we have to appropriately
     * interpret the intent of the pthread_exit caller.  Since the standard
     * convention is to return "-1" on failure, we will call
     * cma_thread_exit_error on a pthread status of -1; and otherwise pass
     * the pthread status to cma_thread_exit_normal.
     *
     * Also, notice that noeptions are caught: the CMA exit routines can
     * only raise the special internal "termination" exception, and we want
     * that to propagate to the thread's base frame so it can do its job.
     */
    if ((int)status == -1)
	cma_thread_exit_error ();
    else
	cma_thread_exit_normal ((cma_t_address)status);

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Return the current thread's ID.
 *
 *  FORMAL PARAMETERS:
 *
 *	none
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	Thread ID
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern pthread_t
ptdexc_self(void)
    {
    cma__t_int_tcb	*tcb;
    cma_t_thread	thread;


    cma__int_init ();

    /*
     * Get a pointer to the current thread's TCB
     */
    tcb = cma__get_self_tcb ();

    /*
     * Point user's handle at the current thread object
     */
    cma__object_to_handle ((cma__t_object *)tcb, &thread);
    return thread;
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Set thread's priority
 *
 *  FORMAL PARAMETERS:
 *
 *	thread		Thread handle
 *	priority	new priority
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	Previous priority, or -1 if error.
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
ptdexc_setprio(
	pthread_t	thread,
	int		priority)
    {
    int			ret_val = 0;
    cma_t_priority	oldpri;


    cma__int_init ();
    cma_thread_get_priority (&thread, &oldpri);
    cma_thread_set_priority (&thread, (cma_t_priority)priority);
    ret_val = (int)oldpri;

    return ret_val;
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Get thread's priority
 *
 *  FORMAL PARAMETERS:
 *
 *	thread		Thread ID
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	Priority, or -1 if error.
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
ptdexc_getprio(
	pthread_t	thread)
    {
    int			ret_val = 0;
    cma_t_priority	priority;


    cma__int_init ();
    cma_thread_get_priority (&thread, &priority);
    ret_val = (int)priority;

    return ret_val;
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Set thread's scheduler
 *
 *  FORMAL PARAMETERS:
 *
 *	thread		Thread ID
 *	scheduler	new scheduler
 *	priority	new priority
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	Old scheduling policy, or -1 if error
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
ptdexc_setscheduler(
	pthread_t	thread,
	int		scheduler,
	int		priority)
    {
    int			ret_val = 0;
    cma_t_sched_policy	policy, oldpolicy;


    cma__int_init ();

    switch (scheduler) {
	case SCHED_FIFO : {
	    policy = cma_c_sched_fifo;
	    break;
	    }
	case SCHED_RR : {
	    policy = cma_c_sched_rr;
	    break;
	    }
	case SCHED_FG_NP : {
	    policy = cma_c_sched_throughput;
	    break;
	    }
	case SCHED_BG_NP : {
	    policy = cma_c_sched_background;
	    break;
	    }
	default : {
	    cma__error (cma_s_badparam);
	    break;
	    }

	}

    cma_thread_get_sched (&thread, &oldpolicy);
    cma_thread_set_sched (&thread, policy, (cma_t_priority)priority);

    switch (oldpolicy) {
	case cma_c_sched_fifo : {
	    ret_val = SCHED_FIFO;
	    break;
	    }
	case cma_c_sched_rr : {
	    ret_val = SCHED_RR;
	    break;
	    }
	case cma_c_sched_throughput : {
	    ret_val = SCHED_FG_NP;
	    break;
	    }
	case cma_c_sched_background : {
	    ret_val = SCHED_BG_NP;
	    break;
	    }
	case cma_c_sched_ada_low : {
	    ret_val = SCHED_OTHER;
	    break;
	    }
	case cma_c_sched_idle : {
	    ret_val = SCHED_OTHER;
	    break;
	    }
	default : {
	    break;
	    }

	}

    return ret_val;
    }


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Get thread's scheduler
 *
 *  FORMAL PARAMETERS:
 *
 *	thread		Thread ID
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	Scheduling policy, or -1 if error
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
ptdexc_getscheduler(
	pthread_t	thread)
    {
    int			ret_val = 0;
    cma_t_sched_policy	scheduler;


    cma__int_init ();
    cma_thread_get_sched (&thread, &scheduler);

    switch (scheduler) {
	case cma_c_sched_fifo : {
	    ret_val = SCHED_FIFO;
	    break;
	    }
	case cma_c_sched_rr : {
	    ret_val = SCHED_RR;
	    break;
	    }
	case cma_c_sched_throughput : {
	    ret_val = SCHED_FG_NP;
	    break;
	    }
	case cma_c_sched_background : {
	    ret_val = SCHED_BG_NP;
	    break;
	    }
	case cma_c_sched_ada_low : {
	    ret_val = SCHED_OTHER;
	    break;
	    }
	case cma_c_sched_idle : {
	    ret_val = SCHED_OTHER;
	    break;
	    }
	default : {
	    break;
	    }

	}

    return ret_val;
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Yield processor
 *
 *  FORMAL PARAMETERS:
 *
 *	none
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern void
ptdexc_yield(void)
    {
    if (!cma__is_multithreaded())
	return;
    cma_yield ();
    }

#ifndef _HP_CMA_PERF_
extern void
ptdexc_yield_noselect(void)
    {
    cma_yield_noselect ();
    }
#endif /* _HP_CMA_PERF_ */

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	One-time initialization
 *
 *  FORMAL PARAMETERS:
 *
 *	once_block	Control block (pthread_once_t)
 *	init_routine	Routine to call for initialization
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	Zero (or none if exception raised)
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
ptdexc_once(
	pthread_once_t		*once_block,
	pthread_initroutine_t	init_routine)
    {
    cma__int_init ();
    cma_once (
	    (cma_t_once *)once_block,
	    (cma_t_init_routine)init_routine,
	    cma_c_null_ptr);
	return (0);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Create thread-specific key value
 *
 *  FORMAL PARAMETERS:
 *
 *	destructor	Routine to call on thread termination
 *	key		Return value of key
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	Zero (or none if exception raised)
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
ptdexc_keycreate(
	pthread_key_t		*key,
	pthread_destructor_t	destructor)
    {
    cma__int_init ();
    cma_key_create (key, &cma_c_null, destructor);
	return (0);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Set thread-specific key value
 *
 *  FORMAL PARAMETERS:
 *
 *	key		Key to set
 *	value		Value to set
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	Zero (or none if exception raised)
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
ptdexc_setspecific(
	pthread_key_t	key,
	pthread_addr_t	value)
    {
    cma__t_int_tcb	*self;		/* Pointer to TCB */
    cma_t_integer	next_context;	/* Snapshot of global key count */


    /*
     * Note that we don't lock any mutexes. This keeps the getcontext
     * function lean and mean. There are two important assumptions in this:
     *
     * 1) If any other thread is modifying the maximum context value, we may
     * get the wrong value, with a possible "spurious" failure. However, that
     * would mean someone is creating a context key in one thread and USING
     * it in another without synchronization. Tough cookies: you get what you
     * pay for.
     *
     * 2) We assume that no other thread will access or modify the context
     * data in this thread's TCB. Since we don't lock the TCB, such access
     * could result in erroneous results in one or the other. DECthreads
     * doesn't support any interfaces to touch another thread's context
     * values, so anyone doing so is just asking for trouble, and I don't
     * mind giving it to them to save a lock operation!
     */
    next_context = cma__g_context_next;

    if ((key <= 0) || (key >= next_context))
	cma__error (cma_s_badparam);

    self = cma__get_self_tcb ();	/* Get current thread's TCB */

    /*
     * If the key value is higher than the current size of the thread's
     * context array, then allocate a new array.  Make it large enough to
     * hold the highest defined key value.  Copy the current context array
     * (if any) into the new one, and free the old one.
     */
    if (self->context_count <= key) {
	cma__t_context_list	new_list;	/* Pointer to new list */
	cma_t_natural		new_size;	/* Size of new list */


	new_size = sizeof (cma_t_address) * next_context;
	new_list = (cma__t_context_list)cma__alloc_zero (new_size);

	if ((cma_t_address)new_list == cma_c_null_ptr) {
	    cma__error (exc_s_insfmem);
	    }

	if (self->contexts != (cma__t_context_list)cma_c_null_ptr) {
	    cma__memcpy (
		    (char *)new_list,
		    (char *)self->contexts,
		    (self->context_count) * sizeof (cma_t_address));
	    cma__free_mem ((cma_t_address)self->contexts);
	    }

	self->contexts = new_list;
	self->context_count = next_context;
	}

    self->contexts[key] = value;
	return (0);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Get thread-specific key value
 *
 *  FORMAL PARAMETERS:
 *
 *	key		key to get
 *	value		Return value
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	Zero (or none if exception raised)
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
ptdexc_getspecific(
	pthread_key_t	key,
	pthread_addr_t	*value)
    {
    cma__t_int_tcb	*self;		/* Pointer to TCB */
    cma_t_integer	next_context;	/* Local copy of next key value */


    /*
     * Note that we don't lock any mutexes. This keeps the getcontext
     * function lean and mean. There are two important assumptions in this:
     *
     * 1) If any other thread is modifying the maximum context value, we may
     * get the wrong value, with a possible "spurious" failure. However, that
     * would mean someone is creating a context key in one thread and USING
     * it in another without synchronization. Tough cookies: you get what you
     * pay for.
     *
     * 2) We assume that no other thread will access or modify the context
     * data in this thread's TCB. Since we don't lock the TCB, such access
     * could result in erroneous results in one or the other. DECthreads
     * doesn't support any interfaces to touch another thread's context
     * values, so anyone doing so is just asking for trouble, and I don't
     * mind giving it to them to save a lock operation!
     */
    next_context = cma__g_context_next;

    if ((key <= 0) || (key >= next_context))
	cma__error (cma_s_badparam);

    self = cma__get_self_tcb ();	/* Get current thread's TCB */

    /*
     * If the requested key is not within the allocated context array (or if
     * there is no context array), then return the value null ("no context");
     * otherwise return the current value of the context (which may also be
     * null).
     */
    if (self->context_count <= key)
	*value = cma_c_null_ptr;
    else
	*value = self->contexts[key];
	return (0);

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Create a mutex attribute object
 *
 *  FORMAL PARAMETERS:
 *
 *	attr		Return attribute object handle
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	Zero (or none if exception raised)
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
ptdexc_mutexattr_create(
	pthread_mutexattr_t	*attr)
    {
    cma__t_int_attr     *int_att;


    cma__int_init ();
    int_att = cma__get_attributes (&cma__g_def_attr);

    if ((cma_t_address)int_att == cma_c_null_ptr)
	cma__error (exc_s_insfmem);
    else
	cma__object_to_handle ((cma__t_object *)int_att, (cma_t_attr *)attr);
	return (0);

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Delete a mutex attribute object
 *
 *  FORMAL PARAMETERS:
 *
 *	attr		Attribute object handle
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	Zero (or none if exception raised)
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
ptdexc_mutexattr_delete(
	pthread_mutexattr_t	*attr)
    {
    cma__t_int_attr     *int_att;


    int_att = cma__validate_null_attr ((cma_t_attr *)attr);

    if (int_att == (cma__t_int_attr *)cma_c_null_ptr)
	return;

    cma__free_attributes (int_att);
    cma__clear_handle (attr);
	return (0);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Set mutex kind attribute
 *
 *  FORMAL PARAMETERS:
 *
 *	attr		Attribute object
 *	kind		mutex kind
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	Zero (or none if exception raised)
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
ptdexc_mutexattr_setkind_np(
	pthread_mutexattr_t	*attr,
	int			kind)
    {
    cma_attr_set_mutex_kind (attr, (cma_t_mutex_kind)kind);
	return (0);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Get mutex kind attribute
 *
 *  FORMAL PARAMETERS:
 *
 *	attr		Attribute object
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	mutex kind attribute
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
ptdexc_mutexattr_getkind_np(
	pthread_attr_t	attr)
    {
    cma_t_mutex_kind	kind;


    cma_attr_get_mutex_kind (&attr, &kind);
    return (int)kind;
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Create a mutex object
 *
 *  FORMAL PARAMETERS:
 *
 *	mutex		Return mutex handle
 *	attr		Attribute object handle
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	Zero (or none if exception raised)
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
ptdexc_mutex_init(
	pthread_mutex_t		*mutex,
	pthread_mutexattr_t	attr)
    {
    cma__t_int_mutex    *imutex;
    cma__t_int_attr     *int_att;


    cma__int_init ();
    int_att = cma__validate_default_attr (&attr);
    imutex = cma__get_mutex (int_att);

    if ((cma_t_address)imutex == cma_c_null_ptr)
	cma__error (exc_s_insfmem);
    else
	cma__object_to_handle ((cma__t_object *)imutex, mutex);
	return (0);

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Delete a mutex object
 *
 *  FORMAL PARAMETERS:
 *
 *	mutex		Mutex object handle
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	Zero (or none if exception raised)
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
ptdexc_mutex_destroy(
	pthread_mutex_t	*mutex)
    {
    cma_mutex_delete (mutex);
	return (0);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Lock a mutex
 *
 *  FORMAL PARAMETERS:
 *
 *	mutex		Mutex object handle
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	Zero (or none if exception raised)
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
ptdexc_mutex_lock(
	pthread_mutex_t	*mutex)
    {
    cma__t_int_tcb	*tcb;
    cma__t_int_mutex    *int_mutex;

    /*
     * Get a pointer to the mutex structure; if this is a debugging CMA,
     * we'll validate the mutex handle to be sure it's valid.  For
     * performance, if it's an NDEBUG ("production") CMA, just fetch the
     * address of the object from the handle's pointer field.
     */
#ifdef NDEBUG
    int_mutex = (cma__t_int_mutex *)((cma__t_int_handle *)mutex)->pointer;
#else
    int_mutex = cma__validate_mutex (mutex);
    tcb = cma__get_self_tcb ();
#endif
                          
    /*
     * First, try to acquire the lock; if we get it, then we're done
     */
    if (cma__test_and_set (&int_mutex->lock)) {
	cma_t_status	res;


	res = cma__int_mutex_block (int_mutex);
        /*
	 * CHFts08244. Its possible that we returned here after the mutex
	 * object was destroyed by another thread. Check that it is still 
	 * valid
	 */
	if (((cma__t_int_handle *)mutex)->pointer == cma_c_null_ptr)
	  res = cma_s_existence;

	if (res != cma_s_normal)
	    cma__error (res);

	}

#ifndef NDEBUG
    int_mutex->owner = tcb;
#endif
	return (0);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Attempt to lock a mutex
 *
 *  FORMAL PARAMETERS:
 *
 *	mutex		Mutex object handle
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	0 if already locked; 1 if successful
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
ptdexc_mutex_trylock(
	pthread_mutex_t	*mutex)
    {
    cma__t_int_mutex	*int_mutex;
    cma__t_int_tcb	*tcb;


#ifdef NDEBUG
    int_mutex = (cma__t_int_mutex *)((cma__t_int_handle *)mutex)->pointer;
#else
    int_mutex = cma__validate_mutex (mutex);
    tcb = cma__get_self_tcb ();
#endif

    if (cma__test_and_set (&int_mutex->lock)) {

	switch (int_mutex->mutex_kind) {
	    case cma_c_mutex_nonrecursive : {
#ifdef NDEBUG
		tcb = cma__get_self_tcb ();
#endif
		cma__int_lock (int_mutex->int_lock);

		if (int_mutex->owner == (cma__t_int_tcb *)cma_c_null_ptr) {
		    int_mutex->owner = tcb;
		    cma__int_unlock (int_mutex->int_lock);
		    return 1;
		    }
		else {
		    cma__int_unlock (int_mutex->int_lock);
		    return 0;
		    }

		break;
		}

	    case cma_c_mutex_recursive : {
#ifdef NDEBUG
		tcb = cma__get_self_tcb ();
#endif
		cma__int_lock (int_mutex->int_lock);

		if (tcb == int_mutex->owner
			|| int_mutex->owner == (cma__t_int_tcb *)cma_c_null_ptr) {
		    int_mutex->nest_count++;
		    int_mutex->owner = tcb;
		    cma__int_unlock (int_mutex->int_lock);
		    return 1;
		    }
		else {
		    cma__int_unlock (int_mutex->int_lock);
		    return 0;
		    }

		break;
		}

	    case cma_c_mutex_fast : {
		return 0;
		break;
		}

	    }

	}

#ifndef NDEBUG
    int_mutex->owner = tcb;
#endif
    return 1;
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Unlock a mutex
 *
 *  FORMAL PARAMETERS:
 *
 *	mutex		Mutex object handle
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	Zero (or none if exception raised)
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
ptdexc_mutex_unlock(
	pthread_mutex_t	*mutex)
    {
    cma__t_int_mutex    *int_mutex;

#ifdef NDEBUG
    int_mutex = (cma__t_int_mutex *)((cma__t_int_handle *)mutex)->pointer;
#else
    cma__t_int_tcb	*tcb;

    int_mutex = cma__validate_mutex (mutex);
    tcb = cma__get_self_tcb ();

    if (int_mutex->mutex_kind == cma_c_mutex_fast) {
	cma__assert_warn (
		(tcb == int_mutex->owner),
		"attempt to release mutex owned by another thread");
	int_mutex->owner = (cma__t_int_tcb *)cma_c_null_ptr;
	}

#endif
    cma__unset (int_mutex->unlock);

    /*
     * Check whether there might be waiters, and reset the bit (TRUE means
     * "no waiters").  If there might be waiters, increment the semaphore to
     * release one.
     */
    if (!cma__test_and_set (&int_mutex->event)) {
	cma_t_status	res;


	res = cma__int_mutex_unblock (int_mutex);

	if (res != cma_s_normal)
	    cma__error (res);

	}
	return (0);

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Create a condition attribute object
 *
 *  FORMAL PARAMETERS:
 *
 *	attr		Return attribute object handle
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	Zero (or none if exception raised)
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
ptdexc_condattr_create(
	pthread_condattr_t	*attr)
    {
    cma__t_int_attr     *int_att;


    cma__int_init ();
    int_att = cma__get_attributes (&cma__g_def_attr);

    if ((cma_t_address)int_att == cma_c_null_ptr)
	cma__error (exc_s_insfmem);
    else
	cma__object_to_handle ((cma__t_object *)int_att, (cma_t_attr *)attr);
	return (0);

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Delete a condition attribute object
 *
 *  FORMAL PARAMETERS:
 *
 *	attr		Attribute object handle
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	Zero (or none if exception raised)
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
ptdexc_condattr_delete(
	pthread_condattr_t	*attr)
    {
    cma__t_int_attr     *int_att;


    int_att = cma__validate_null_attr ((cma_t_attr *)attr);

    if (int_att == (cma__t_int_attr *)cma_c_null_ptr)
	return;

    cma__free_attributes (int_att);
    cma__clear_handle (attr);
	return (0);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Create a condition object
 *
 *  FORMAL PARAMETERS:
 *
 *	cond		Return condition handle
 *	attr		Attribute object handle
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	Zero (or none if exception raised)
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
ptdexc_cond_init(
	pthread_cond_t		*cond,
	pthread_condattr_t	attr)
    {
    cma__t_int_cv               *cv; 
    cma__t_int_attr             *int_att; 


    cma__int_init ();
    int_att = cma__validate_default_attr (&attr);
    cv = cma__get_cv (int_att);

    if ((cma_t_address)cv == cma_c_null_ptr)
	cma__error (exc_s_insfmem);
    else
	cma__object_to_handle ((cma__t_object *)cv, cond);
	return (0);

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Delete a condition object
 *
 *  FORMAL PARAMETERS:
 *
 *	cond		Condition object handle
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	Zero (or none if exception raised)
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
ptdexc_cond_destroy(
	pthread_cond_t	*cond)
    {
    cma__int_cond_delete (cond);
	return (0);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Signal a condition
 *
 *  FORMAL PARAMETERS:
 *
 *	cond		Condition object handle
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	Zero (or none if exception raised)
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
ptdexc_cond_signal(
	pthread_cond_t	*cond)
    {
    cma__t_int_cv       *int_cv;


#ifdef NDEBUG
    int_cv = (cma__t_int_cv *)((cma__t_int_handle *)cond)->pointer;
#else
    int_cv = cma__validate_cv (cond);
#endif

    cma__int_signal (int_cv);
	return (0);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Signal a condition from interrupt level
 *
 *  FORMAL PARAMETERS:
 *
 *	cond		Condition object handle
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	Zero (or none if exception raised)
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
ptdexc_cond_signal_int_np(
	pthread_cond_t	*cond)
    {
    cma_cond_signal_int (cond);
	return (0);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Broadcast a condition
 *
 *  FORMAL PARAMETERS:
 *
 *	cond		Condition object handle
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	Zero (or none if exception raised)
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
ptdexc_cond_broadcast(
	pthread_cond_t	*cond)
    {
    cma__t_int_cv       *int_cv;


#ifdef NDEBUG
    int_cv = (cma__t_int_cv *)((cma__t_int_handle *)cond)->pointer;
#else
    int_cv = cma__validate_cv (cond);
#endif

    cma__int_broadcast (int_cv);
	return (0);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Wait on a condition
 *
 *  FORMAL PARAMETERS:
 *
 *	cond		Condition object handle
 *	mutex		Mutex associated with condition
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	Zero (or none if exception raised)
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
ptdexc_cond_wait(
	pthread_cond_t	*cond,
	pthread_mutex_t	*mutex)
    {
    cma__int_cond_wait (cond, mutex);
	return (0);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Wait on a condition, with timeout
 *
 *  FORMAL PARAMETERS:
 *
 *	cond		Condition object handle
 *	mutex		Mutex associated with condition
 *	abstime		Time to wait
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	status
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
ptdexc_cond_timedwait(
	pthread_cond_t	*cond,
	pthread_mutex_t	*mutex,
	struct timespec	*abstime)
    {
    cma_t_status	status;
    cma_t_date_time	time;
#if _CMA_OS_ == _CMA__VMS
    cma_t_date_time	delta;
    int			vstat;
    int			delta_secs;
#endif

    if (abstime->tv_nsec >= (1000 * 1000000) || abstime->tv_nsec < 0) {
	cma__set_errno (EINVAL);
	return -1;
    }
	
#if _CMA_OS_ == _CMA__VMS
    delta_secs = LIB$K_DELTA_SECONDS;

    vstat = lib$cvt_to_internal_time (
	    &delta_secs,
	    &abstime->tv_sec,
	    &delta);

    if ((vstat & 7) != 1)
	cma__error (cma_s_badparam);

    /*
     * Adjust the resulting delta time by the number of nanoseconds in the
     * timespec.  Note that VMS delta time is negative, so we subtract. The
     * low order longword of the VMS quadword time can express roughly 7
     * minutes of time, and the maximum value of tv_nsec is less than a
     * second, so the chances of an overflow occurring are one in roughly
     * 400; I'm not going to worry about it.
     */
    delta.low -= ((abstime->tv_nsec + 99) / 100);
    lib$add_times (&pthread___g_epoch, &delta, &time);
#else
    time.tv_sec = abstime->tv_sec;
    time.tv_usec = (abstime->tv_nsec + 999) / 1000;
#endif

    if (cma_cond_timed_wait (cond, mutex, &time) == cma_s_timed_out)
	status = EAGAIN;
    else
	status = 0;

    return status;
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Cancel a thread
 *
 *  FORMAL PARAMETERS:
 *
 *	thread		ID of thread to cancel
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	Zero (or none if exception raised)
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
ptdexc_cancel(
	pthread_t	thread)
    {
    cma_thread_alert (&thread);
	return (0);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Test for pending cancel
 *
 *  FORMAL PARAMETERS:
 *
 *	none
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	cancel thread if cancel is pending.
 */
extern void
ptdexc_testcancel(void)
    {
    cma__int_init ();
    cma__int_alert_test ();
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Set general cancelability of thread
 *
 *  FORMAL PARAMETERS:
 *
 *	state	New state of general cancelability (CANCEL_ON or CANCEL_OFF)
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	CANCEL_ON if cancellation was enabled; CANCEL_OFF if it was disabled.
 *
 *  SIDE EFFECTS:
                       *
 *	none
 */
extern int
ptdexc_setcancel(int	state)
    {
    int			previous;
    cma__t_int_tcb	*tcb;


    cma__int_init ();

    if (state != CANCEL_ON && state != CANCEL_OFF)
	cma__error (cma_s_badparam);

    tcb = cma__get_self_tcb ();
    cma__int_lock (tcb->mutex);
    previous = (tcb->alert.g_enable ? CANCEL_ON : CANCEL_OFF);
    tcb->alert.g_enable = (state == CANCEL_ON);
    cma__int_unlock (tcb->mutex);
    return previous;
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Set async cancelability of thread
 *
 *  FORMAL PARAMETERS:
 *
 *	state	New state of async cancelability (CANCEL_ON or CANCEL_OFF)
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	CANCEL_ON if cancellation was enabled; CANCEL_OFF if it was disabled.
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
ptdexc_setasynccancel(int	state)
    {
    int			previous;
    cma__t_int_tcb	*tcb;


    cma__int_init ();

    if (state != CANCEL_ON && state != CANCEL_OFF)
	cma__error (cma_s_badparam);

    tcb = cma__get_self_tcb ();
    cma__int_lock (tcb->mutex);
    previous = (tcb->alert.a_enable ? CANCEL_ON : CANCEL_OFF);
    tcb->alert.a_enable = (state == CANCEL_ON);
    cma__int_unlock (tcb->mutex);
    return previous;
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Convert a delta timespec to absolute (offset by current time)
 *
 *  FORMAL PARAMETERS:
 *
 *	delta	struct timespec; input delta time
 *
 *	abstime	struct timespec; output absolute time
 *
 *  IMPLICIT INPUTS:
 *
 *	current time
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	Zero (or none if exception raised)
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
ptdexc_get_expiration_np(struct timespec	*delta,
	struct timespec		*abstime)
    {
    struct timespec	current_time;


    if (delta->tv_nsec >= (1000 * 1000000) || delta->tv_nsec < 0)
	cma__error (cma_s_badparam);

    cma__gettimespec (&current_time);
    abstime->tv_nsec	= delta->tv_nsec + (current_time.tv_nsec);
    abstime->tv_sec	= delta->tv_sec + current_time.tv_sec;

    if (abstime->tv_nsec >= (1000 * 1000000)) {	/* check for carry */
	abstime->tv_nsec -= (1000 * 1000000);
	abstime->tv_sec += 1;
	}
	return (0);

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Wait for interval.
 *
 *  FORMAL PARAMETERS:
 *
 *	interval	struct timespec
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	Zero (or none if exception raised)
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
ptdexc_delay_np(struct timespec	*interval)
    {
    cma_t_date_time	time;


    cma__int_init ();

    if (interval->tv_nsec >= (1000 * 1000000) || interval->tv_nsec < 0)
	cma__error (cma_s_badparam);

    /*
     * The simplest way to get through to the underlying CMA timed wait
     * mechanism is to convert the pthread delta "timespec" into a CMA
     * "interval" (floating point seconds), and use cma_time_get_expiration
     * to convert that to an absolute cma_t_date_time.
     */
    cma_time_get_expiration (
	    &time,
	    (float)interval->tv_sec
		+ ((float)interval->tv_nsec / (1000000000.0)));
    cma__int_lock (cma__g_delay_mutex);

    /*
     * Using the pthread cleanup handler will catch any CMA exceptions which
     * get raised by the wait (specifically, cma_e_alerted, which is pthread
     * cancel).
     */
    pthread_cleanup_push (pthread___unlock_int_mutex, cma__g_delay_mutex);

    while (
	cma__int_timed_wait (
		cma__g_delay_cv,
		cma__g_delay_mutex,
		&time) != cma_s_timed_out);

    /*
     * Pop the cleanup handler, and execute it (ensure that the delay mutex
     * is unlocked whether the timed wait was cancelled or not).
     */
    pthread_cleanup_pop (1);
	return (0);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Lock global lock (for nonreentrant libraries)
 *
 *  FORMAL PARAMETERS:
 *
 *	none
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern void
ptdexc_lock_global_np(void)
    {
    cma__int_init ();
    cma__int_lock (cma__g_global_lock);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Unlock global lock (for nonreentrant libraries)
 *
 *  FORMAL PARAMETERS:
 *
 *	none
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern void
ptdexc_unlock_global_np(void)
    {
    cma__int_unlock (cma__g_global_lock);
    }

#if _CMA_OS_ == _CMA__UNIX
/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Create a thread that will sigwait() on a set of signals; when any of
 *	them arrives, it will cancel a specified thread.
 *
 *  FORMAL PARAMETERS:
 *
 *	sigset	The set of signals to catch
 *	thread	The ID of the thread to be canceled
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	If a worker thread already exists, it is canceled and a new one (with
 *	the new signal set and target thread) is started.
 */
extern void
ptdexc_signal_to_cancel_np(sigset_t	*sigset,
	pthread_t	*thread)
    {

    cma__int_lock (cma__g_global_lock);

    TRY {
	(void) cma__validate_tcb (thread);

	if (cma__g_handle_active) {
	    /*
	     * If there's already a handler thread running, cancel it and
	     * wait for it to complete. Note that we don't wait for the
	     * worker to actually exit; it doesn't matter.
	     */
	    cma_thread_alert (&cma__g_handle_thread);
	    cma_thread_detach (&cma__g_handle_thread);
	    }

	cma__g_handle_target = *thread;
	cma__g_handle_sigset = *sigset;
	cma__g_handle_active = cma_c_true;

	cma_thread_create (
		&cma__g_handle_thread,
		&cma_c_null,
		(cma_t_start_routine)pthread__cancel_thread,
		cma_c_null_ptr);

	}
    FINALLY {
	cma__int_unlock (cma__g_global_lock);
	}
    ENDTRY
    }
#endif

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Unlock a mutex on cancel or exit (used as cleanup handler)
 *
 *  FORMAL PARAMETERS:
 *
 *	arg		address of mutex handle
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
static void
pthread___unlock_int_mutex(pthread_addr_t arg)
    {
    cma__int_unlock ((cma__t_int_mutex *)arg);
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_PTD_EXC.C */
/*  *21   13-MAR-1992 14:09:11 BUTENHOF "Fix mutex lock/unlock code" */
/*  *20   10-MAR-1992 16:26:30 BUTENHOF "Eliminate need for TRY/CATCH on pthread mutex lock" */
/*  *19   18-FEB-1992 15:29:47 BUTENHOF "Adapt to alloc_mem changes" */
/*  *18    6-DEC-1991 07:19:38 BUTENHOF "Add background policy to attr_setsched" */
/*  *17   20-NOV-1991 12:31:36 CURTIN "Alpha work" */
/*  *16   18-NOV-1991 11:22:22 BUTENHOF "Modify signal_to_cancel_np" */
/*  *15   14-OCT-1991 13:39:20 BUTENHOF "Add get/set guardsize functions" */
/*  *14   27-AUG-1991 17:48:01 BUTENHOF "Fix pthread_join to accept null status" */
/*  *13    2-AUG-1991 15:51:23 CURTIN "added an init check to ptdexc_lock_global_np" */
/*  *12   10-JUN-1991 18:22:42 SCALES "Add sccs headers for Ultrix" */
/*  *11    6-JUN-1991 11:18:38 BUTENHOF "Fix cma_once" */
/*  *10    5-JUN-1991 18:37:50 BUTENHOF "Include cma_px.h" */
/*  *9    14-MAY-1991 13:19:47 CURTIN "Replaced external calls with internal macros" */
/*  *8     2-MAY-1991 13:58:33 BUTENHOF "Add argument to cma__bugcheck() calls" */
/*  *7    12-APR-1991 23:36:15 BUTENHOF "Change errno access for OSF/1" */
/*  *6     1-APR-1991 18:09:05 BUTENHOF "Add pthread_signal_to_cancel_np()" */
/*  *5    22-MAR-1991 13:42:50 BUTENHOF "Raise error on ptdexc_setcancel w/ bad value" */
/*  *4    21-MAR-1991 09:26:28 BUTENHOF "implement ptdexc_cond_signal_int()" */
/*  *3    12-FEB-1991 01:29:10 BUTENHOF "Redefine alert state" */
/*  *2     5-FEB-1991 00:59:43 BUTENHOF "Change pthread exc interface names" */
/*  *1    12-DEC-1990 21:48:38 BUTENHOF "P1003.4a support" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_PTD_EXC.C */
