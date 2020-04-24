/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems
 * Burlington, MA, USA
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: cma_pthread.c,v $
 * Revision 1.1.12.2  1996/06/04  22:16:35  arvind
 * 	Merge change
 * 	[1996/05/01  15:40 UTC  bissen  /main/DCE_1.2/2]
 *
 * 	Add cma__is_multithreaded to pthread_yield
 * 	[1996/05/01  15:34 UTC  bissen  /main/DCE_1.2/bissen_122_work/1]
 *
 * 	Loading drop DCE1_0_2FNL
 * 	[1993/05/04  22:34:54  mgm  1.1.1.11]
 *
 * 	Update OSF copyright year
 * 	[1996/02/17  22:50:20  marty]
 *
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:06:35  root]
 *
 * Revision 1.1.12.1  1996/05/10  13:40:05  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 * 
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:44 UTC  bissen
 * 	Merge in changes to CHFts14939 and CHFts14807 from branch jss_mothra_fix3.
 * 	[1995/04/26  20:04 UTC  jss  /main/HPDCE02/5]
 * 
 * 	Fix for CHFts14807. Fix ptdexc_setscheduler() to support SCHED_BG_NP.
 * 	[1995/04/10  18:34 UTC  jss  /main/HPDCE02/jss_mothra_fix3/3]
 * 
 * 	Merge in single-threaded client changes for scheduling and priorities.
 * 	[1995/03/24  18:55 UTC  jss  /main/HPDCE02/4]
 * 
 * 	Clean up pthread_setscheduler() redundant code.
 * 	[1995/03/17  20:23 UTC  jss  /main/HPDCE02/jss_mothra_fix3/2]
 * 
 * 	Single threaded client changes.  Remove cma__int_init2() from pthread_setscheduler().
 * 	We will do this at a lower level.
 * 	[1995/03/17  16:57 UTC  jss  /main/HPDCE02/jss_mothra_fix3/1]
 * 
 * 	Merge from branch jss_single_thread.
 * 	[1994/12/23  15:53 UTC  jss  /main/HPDCE02/3]
 * 
 * 	Change for single threaded client. pthread_yield should just return if we are not
 * 	multi-threaded.
 * 	[1994/12/23  15:50 UTC  jss  /main/HPDCE02/jss_single_thread/3]
 * 
 * 	Merge from branch jss_single_thread.
 * 	[1994/12/22  19:58 UTC  jss  /main/HPDCE02/2]
 * 
 * 	Changes for single threaded client. pthread_setscheduler() should initialize
 * 	the entire CMA before attempting the call.
 * 	[1994/12/22  19:24 UTC  jss  /main/HPDCE02/jss_single_thread/2]
 * 
 * 	Changes for single threaded client. Added cma__int_init2() routine to
 * 	pthread_create().
 * 	[1994/12/15  15:42 UTC  jss  /main/HPDCE02/jss_single_thread/1]
 * 
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:43 UTC  jss  /main/HPDCE02/1]
 * 
 * 	Merge for RODAN cycle T bsu
 * 	[1994/06/16  17:25 UTC  jss  /main/HPDCE01/3]
 * 
 * 	If pthread_mutex_lock() blocks, another thread could have destroyed the mutex variable
 * 	before it returns.  This change tests the mutex handle after returning from
 * 	blocking to see if it is still valid.
 * 	[1994/06/16  15:54 UTC  jss  /main/HPDCE01/jss_rodan_r/1]
 * 
 * 	fixed merge botch
 * 	[1994/01/27  17:06  markar  /main/HPDCE01/2]
 * 
 * 	merge kk and hpdce01
 * 	[1994/01/24  16:43  truitt  /main/HPDCE01/1]
 * 
 * 	Merging DEPT changes
 * 	[1993/10/18  14:38:41  prepetit]
 * 	 *
 * 	added code to pthread_setscheduler to return old priority and deleted code which
 * 	returned old policy. This is to conform with the POSIX P1003.4a/D4 draft.
 * 	[1993/08/18  20:26:18  rhw]
 * 	 *
 * 
 * 	merge kk and dce1_0_3
 * 	[1993/09/21  13:31:56  truitt]
 * 
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:43:56  root  1.1.4.3]
 * 
 * 	Oops.  Just correcting a typo.
 * 	[1993/08/20  14:35:17  truitt]
 * 
 * 	CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:18  devsrc]
 * 	 *
 * 
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:18:19  jd]
 * 
 * 	added code to pthread_setscheduler to return old priority and deleted code which
 * 	returned old policy. This is to conform with the POSIX P1003.4a/D4 draft.
 * 	[1993/08/18  20:26:18  rhw]
 * 
 * 	Put in call to initialization in pthread_yield, pthread_lock_global_np,
 * 	and pthread_unlock_global_np.
 * 	[1993/02/03  19:31:00  jd]
 * 
 * 	Check for invalid timespec argument in pthread_cond_timedwait. This argument
 * 	is worth the time it takes since a bad value will probably result in very
 * 	mysterious errors from select. All of the paths into the threads package
 * 	except this one and ptdexc_cond_timedwait were already doing the check.
 * 	[1993/01/23  22:46:03  jd]
 * 
 * 	Check for cma_s_in_use indicationg the thread tried to join itself.
 * 	[1993/01/21  22:07:10  jd]
 * 
 * 	Insert copyright notices
 * 	[1992/12/07  19:40:24  alan]
 * 
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:50:03  sekhar]
 * 
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:05:27  bolinger]
 * 
 * 	OT8277: This one is useful for us to pick up.  In order to conform with the
 * 	POSIX P1003.4a/D4 draft, the function pthread_setscheduler should return
 * 	the old priority value on success, not the old policy value as it was doing.
 * 	Man page needs to be updated also to reflect this change.
 * 	[1993/08/20  14:29:07  truitt]
 * 
 * 	Bring forward 1.0.1 revision 1.1.2.2 (fix for OT defect 2992).
 * 	Also map CMA-internal scheduling classes to SCHED_OTHER.
 * 
 * Revision 1.1.8.2  1994/06/09  13:38:26  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:18  devsrc]
 * 
 * Revision 1.1.8.1  1994/04/01  20:16:56  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:18:19  jd]
 * 
 * Revision 1.1.6.2  1993/08/18  21:35:35  rhw
 * 	added code to pthread_setscheduler to return old priority and deleted code which
 * 	returned old policy. This is to conform with the POSIX P1003.4a/D4 draft.
 * 	[1993/08/18  20:26:18  rhw]
 * 
 * Revision 1.1.4.6  1993/02/04  00:28:58  jd
 * 	Put in call to initialization in pthread_yield, pthread_lock_global_np,
 * 	and pthread_unlock_global_np.
 * 	[1993/02/03  19:31:00  jd]
 * 
 * Revision 1.1.4.5  1993/01/26  13:26:27  jd
 * 	Check for invalid timespec argument in pthread_cond_timedwait. This argument
 * 	is worth the time it takes since a bad value will probably result in very
 * 	mysterious errors from select. All of the paths into the threads package
 * 	except this one and ptdexc_cond_timedwait were already doing the check.
 * 	[1993/01/23  22:46:03  jd]
 * 
 * 	Check for cma_s_in_use indicationg the thread tried to join itself.
 * 	[1993/01/21  22:07:10  jd]
 * 
 * Revision 1.1.4.4  1992/12/15  22:29:25  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:40:24  alan]
 * 
 * Revision 1.1.4.3  1992/09/29  20:15:13  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:50:03  sekhar]
 * 
 * Revision 1.1.4.2  1992/09/03  14:39:15  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:05:27  bolinger]
 * 
 * Revision 1.1.2.3  1992/08/14  20:21:59  bolinger
 * 	Bring forward 1.0.1 revision 1.1.2.2 (fix for OT defect 2992).
 * 	Also map CMA-internal scheduling classes to SCHED_OTHER.
 * 	[1992/08/10  16:08:18  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/08  23:38:59  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:38:38  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/12  23:36:30  keane
 * 	Address defects 2666 and 2622.  Add support for scheduling policies
 * 	SCHED_FG_NP and SCHED_BG_NP.
 * 
 * 	Address defects 2968, 2992, 2993.  Move certain parameter validation
 * 	inside of existing try blocks to eliminated unwanted exception returns.
 * 
 * 	Address defect 2926.  pthread_mutex_init now sets errno to EINVAL in its
 * 	CATCH_ALL clause.
 * 	[1992/05/12  20:33:28  keane]
 * 
 * Revision 1.1  1992/01/19  14:41:06  devrcs
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
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	19 February 1990
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	5 March 1990
 *		Resolve use of ambiguous symbol cma_c_sched_default (change
 *		to cma_c_sched_use_default where appropriate).
 *	002	Dave Butenhof	23 March 1990
 *		Add one-time init code so caller doesn't have to call
 *		cma_init to use pthreads interface.
 *	003	Dave Butenhof	18 April 1990
 *		Remove default attribute handles to cma_client
 *	004	Dave Butenhof	2 May 1990
 *		Implement cancel.
 *	005	Webb Scales	4 May 1990
 *		Fixed typos in pthread_cond_timedwait (you can't catch 
 *		statuses with a CATCH clause), and added include for stack.h
 *		to pick up the proto for get self.
 *	006	Dave Butenhof	18 May 1990
 *		Change pthread_cond_timedwait to use struct timespec.
 *	007	Dave Butenhof	31 May 1990
 *		Check exception handling.
 *	008	Dave Butenhof	8 June 1990
 *		Reraise an alert in pthread_join, so that cancel isn't
 *		swallowed up.
 *	009	Dave Butenhof	22 June 1990
 *		Update interface to 1003.4a/D3 (May 11, 1990).  This includes
 *		changing synchronization variable parameters from "value" to
 *		"reference", but does not include degenerating attributes
 *		objects to public structures. It also does not include the
 *		new event operations.
 *	010	Dave Butenhof	29 June 1990
 *		Change interface to pthread_get_expiration_np (an in and an
 *		out instead of just one modify).
 *	011	Dave Butenhof	2 July 1990
 *		Implement pthread_mutexattr_setkind_np and
 *		pthread_mutexattr_getkind_np functions.
 *	012	Webb Scales	17 July 1990
 *		Moved return statements out from inside TRY blocks.
 *		Added compatibility for more scheduling policies.
 *	013	Webb Scales	16 August 1990
 *		Added accomodation for Sun platform
 *	014	Dave Butenhof	27 August 1990
 *		Change CMA interfaces to pass handles & structures by reference.
 *	015	Paul Curtin	13 September 1990
 *		Change ftime to cma__ftime.
 *	016	Dave Butenhof	8 October 1990
 *		Fix call to cma_once to include new argument. Also add back
 *		calls to cma_init() on the get/set current thread scheduling
 *		info functions, which might reasonably be called before
 *		creating any objects.
 *	017	Paul Curtin	21 November 1990
 *		Rearranged/removed return status on a number of routines
 *		for consistency and D4 compatibility.
 *	018	Paul Curtin	21 November 1990
 *		Switched the order of the arguments for pthread_keycreate
 *	019	Dave Butenhof	22 January 1991
 *		Fix exception names
 *	020	Dave Butenhof	5 February 1991
 *		Fix internal condition wait call arguments.
 *	021	Dave Butenhof	7 February 1991
 *		Adapt to new alert bit names.
 *	022	Dave Butenhof	20 March 1991
 *		Fix some return statuses to conform to P1003.4a/D4.
 *	023	Dave Butenhof	26 March 1991
 *		Add pthread_signal_to_cancel_np() for OSF DCE
 *	024	Dave Butenhof	03 April 1991
 *		Fix a bug in pthread___cancel_thread().
 *	025	Dave Butenhof	12 April 1991
 *		Use new cma__set_errno rather than direct assignment to
 *		"errno".
 *	026	Dave Butenhof	01 May 1991
 *		Add arguments to cma__bugcheck() calls.
 *	027	Paul Curtin	13 May 1991
 *		Replaced a number of external interface calls
 *		with internale calls and macros. (Optimizations.)
 *	028	Dave Butenhof	29 May 1991
 *		Return ENOSYS for attempts to fiddle with scheduling
 *		parameters under Mach kernel thread implementation (since
 *		Mach threads don't have P1003.4a scheduling yet).
 *	029	Dave Butenhof and Webb Scales	05 June 1991
 *		Include cma_px.h to get struct timespec definition.
 *	030	Dave Butenhof	27 August 1991
 *		Fix bug in join: it should accept NULL status argument.
 *	031	Dave Butenhof	07 October 1991
 *		Add pthread_attr_setguardsize_np &
 *		pthread_attr_getguardsize_np functions.
 *	032	Paul Curtin	20 November 1991
 *		Alpha port: added a number of platform specific changes.
 *	033	Dave Butenhof	05 December 1991
 *		Fix pthread_attr_setsched to support background policy!
 *	034	Dave Butenhof	17 December 1991
 *		Fix bugs in 032: 1) initialize the local that's supposed to
 *		contain the lib$cvt_to_internal_time operation code. 2) Don't
 *		raise an exception on error, since this is the status
 *		returning interface.
 *	035	Dave Butenhof	10 February 1992
 *		A law of nature has just been changed: cma__alloc_mem now
 *		returns cma_c_null_ptr on an allocation failure, rather than
 *		raising an exception. This allows greater efficiency, since
 *		TRY is generally expensive. Anyway, apply the process of
 *		evolution: adapt or die.
 *	036	Dave Butenhof	09 March 1992
 *		The mutex block/unblock functions have just been changed to
 *		return status values. Change pthread mutex lock/unlock to
 *		call this directly instead of using cma__int_[un]lock and
 *		convert status, rather than using expensive TRY/CATCH.
 *	037	Dave Butenhof	10 March 1992
 *		Change 'timeb' to 'timeval' so we can use gettimeofday rather
 *		than the BSD-only ftime function.
 *	038	Dave Butenhof	12 March 1992
 *		Clean up 036 a little.
 *	039	Dave Butenhof	12 March 1992
 *		More integration of direct code: pthread_self,
 *		pthread_getspecific, pthread_setspecific.
 *	040	Dave Butenhof	16 March 1992
 *		Fix typo in 039 for NDEBUG code.
 */


/*
 *  INCLUDE FILES
 */

#include <cma.h>
#include <cma_px.h>			/* Get struct timespec */
#include <cma_kernel.h>
#include <cma_handle.h>
#include <cma_util.h>
#include <pthread.h>
#include <cma_int_errno.h>
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
#include <cma_init.h> 
#include <cma_context.h>
#include <cma_vm.h>

#if _CMA_OS_ == _CMA__VMS
#include <lib$routines.h>
#include <libdtdef.h>
#endif

#if _CMA_OS_ == _CMA__UNIX
# include <signal.h>
# include <dce/cma_sigwait.h>
#endif

/*
 * GLOBAL DATA
 */

#if _CMA_OS_ == _CMA__UNIX
 cma_t_boolean	cma__g_handle_active = cma_c_false;
 cma_t_thread	cma__g_handle_thread, cma__g_handle_target;
 sigset_t	cma__g_handle_sigset;
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

#ifndef ENOSYS
# define ENOSYS	EINVAL
#endif

/*
 * LOCAL FUNCTIONS
 */

static void
pthread___unlock_int_mutex  (pthread_addr_t arg);


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
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
pthread_attr_create(
	pthread_attr_t	*attr)
    {
    int	status = 0;
    cma__t_int_attr     *att_obj;


    cma__int_init ();
    att_obj = cma__get_attributes (&cma__g_def_attr);

    if ((pthread_addr_t)att_obj == cma_c_null_ptr) {
	cma__set_errno (ENOMEM);
	status = -1;
	}
    else
	cma__object_to_handle ((cma__t_object *)att_obj, (cma_t_attr *)attr);

    return status;
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
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
pthread_attr_delete(
	pthread_attr_t	*attr)
    {
    cma__t_int_attr     *int_att;


    if (cma__val_nullattr_stat (attr, &int_att) != cma_s_normal) {
	cma__set_errno (EINVAL);
	return -1;
	}

    if (int_att == (cma__t_int_attr *)cma_c_null_ptr)
	return 0;

    cma__free_attributes (int_att);
    cma__clear_handle (attr);
    return 0;
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
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
pthread_attr_setprio(
	pthread_attr_t	*attr,
	int		priority)
    {
    int	status = 0;


    TRY {
	cma_attr_set_priority (attr, (cma_t_priority)priority);
	}
    CATCH (exc_e_nopriv) {
	cma__set_errno (EPERM);
	status = -1;
	}
    CATCH (cma_e_badparam) {
	cma__set_errno (ERANGE);
	status = -1;
	}
    CATCH_ALL {
	cma__set_errno (EINVAL);
	status = -1;
	}
    ENDTRY

    return status;
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
pthread_attr_getprio(
	pthread_attr_t	attr)
    {
    int			ret_val = 0;
    cma_t_priority	priority;


    TRY {
	cma__int_attr_get_priority (&attr, &priority);
	ret_val = (int)priority;
	}
    CATCH_ALL {
	cma__set_errno (EINVAL);
	ret_val = -1;
	}
    ENDTRY

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
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
pthread_attr_setsched(
	pthread_attr_t	*attr,
	int		scheduler)
    {
    int			status = 0;
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
	    cma__set_errno (EINVAL);
	    return -1;
	    break;
	    }

	}

    TRY {
	cma_attr_set_sched (attr, policy, priority);
	}
    CATCH (exc_e_nopriv) {
	cma__set_errno (EPERM);
	status = -1;
	}
    CATCH_ALL {
	cma__set_errno (EINVAL);
	status = -1;
	}
    ENDTRY

    return status;
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
pthread_attr_getsched(
	pthread_attr_t	attr)
    {
    int			ret_val = 0;
    cma_t_sched_policy	scheduler;


    TRY {
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
	}
    CATCH_ALL {
	cma__set_errno (EINVAL);
	ret_val = -1;
	}
    ENDTRY

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
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
pthread_attr_setinheritsched(
	pthread_attr_t	*attr,
	int		inherit)
    {
    int			status = 0;
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
	    cma__set_errno (EINVAL);
	    return -1;
	    break;
	    }
	}

    TRY {
	cma_attr_set_inherit_sched (attr, flag);
	}
    CATCH_ALL {
	cma__set_errno (EINVAL);
	status = -1;
	}
    ENDTRY

    return status;
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
pthread_attr_getinheritsched(
	pthread_attr_t	attr)
    {
    int			ret_val = 0;
    cma_t_sched_inherit	inherit;


    TRY {
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
		cma__bugcheck ("attr_getinheritsched:1");
		break;
		}
	    }

	}
    CATCH_ALL {
	cma__set_errno (EINVAL);
	ret_val = -1;
	}
    ENDTRY

    return ret_val;
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
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
pthread_attr_setstacksize(
	pthread_attr_t	*attr,
	long		stacksize)
    {
    int	status = 0;


    TRY {
	cma__int_attr_set_stacksize (attr, (cma_t_natural)stacksize);
	}
    CATCH_ALL {
	cma__set_errno (EINVAL);
	status = -1;
	}
    ENDTRY

    return status;
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
pthread_attr_getstacksize(
	pthread_attr_t	attr)
    {
    int			ret_val = 0;
    cma_t_natural	stacksize;


    TRY {
	cma__int_attr_get_stacksize (&attr, &stacksize);
	ret_val = (long)stacksize;
	}
    CATCH_ALL {
	cma__set_errno (EINVAL);
	ret_val = -1;
	}
    ENDTRY

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
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
pthread_attr_setguardsize_np(
	pthread_attr_t	*attr,
	long		guardsize)
    {
    int	status = 0;


    TRY {
	cma__int_attr_set_guardsize (attr, (cma_t_natural)guardsize);
	}
    CATCH_ALL {
	cma__set_errno (EINVAL);
	status = -1;
	}
    ENDTRY

    return status;
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
pthread_attr_getguardsize_np(
	pthread_attr_t	attr)
    {
    int			ret_val = 0;
    cma_t_natural	guardsize;


    TRY {
	cma__int_attr_get_guardsize (&attr, &guardsize);
	ret_val = (long)guardsize;
	}
    CATCH_ALL {
	cma__set_errno (EINVAL);
	ret_val = -1;
	}
    ENDTRY

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
 *	0 if success, -1 if error (error code in errno).
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
pthread_create(
	pthread_t		*thread,
	pthread_attr_t		attr,
	pthread_startroutine_t	start_routine,
	pthread_addr_t		arg)
    {
    int	status = 0;


    cma__int_init ();
    /*
     * We don't initialize all of CMA until necessary.  cma__int_init2() is
     * not called until we need to be multi-threaded.
     */
    cma__int_init2 ();

    TRY {
	cma_thread_create (thread, &attr, start_routine, arg);
	}
    CATCH (exc_e_insfmem) {
	cma__set_errno (ENOMEM);
	status = -1;
	}
    CATCH_ALL {
	cma__set_errno (EAGAIN);
	status = -1;
	}
    ENDTRY

    return status;
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
 *	0 if success, -1 if error (error code in errno).
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
pthread_join(
	pthread_t		thread,
	pthread_addr_t		*status)
    {
    int			retstatus = 0;
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
    TRY {
	cma_thread_join (&thread, &thdstatus, &result);

	if (status != (pthread_addr_t *)cma_c_null_ptr) {
	    if (thdstatus == cma_c_term_normal)
		*status = (pthread_addr_t)result;
	    else
		*status = (pthread_addr_t)-1;
	    }

	}
    CATCH (cma_e_alerted) {
	RERAISE;
	}
    CATCH (cma_e_use_error) {
	cma__set_errno (ESRCH);
	retstatus = -1;
	}
    CATCH (cma_e_in_use) {        /* Thread attempted to join itself */
	cma__set_errno (EDEADLK);
	retstatus = -1;
	}
    CATCH_ALL {
	cma__set_errno (EINVAL);
	retstatus = -1;
	}
    ENDTRY

    return retstatus;
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
 *	0 if success, -1 if error (error code in errno).
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
pthread_detach(
	pthread_t		*thread)
    {
    int	status = 0;


    TRY {
	cma_thread_detach (thread);
	}
    CATCH (cma_e_use_error) {
	cma__set_errno (ESRCH);
	status = -1;
	}
    CATCH_ALL {
	cma__set_errno (EINVAL);
	status = -1;
	}
    ENDTRY

    return status;
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
pthread_exit(
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
     * Also, notice that no exceptions are caught: the CMA exit routines can
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
pthread_self(void)
    {
    cma__t_int_tcb	*tcb;
    cma_t_thread	thread;


    /*
     * NOTE: cma__get_self_tcb () (on a multiprocessor) may raise the
     * exception cma_e_notcmastack to indicate that it can't determine the
     * thread identity because the client code has switched to a stack not
     * controlled by DECthreads. If this happens, we really can't return a
     * meaningful answer, nor does pthread_self() provide for an error
     * return. Thus, we might as well just let the exception be raised for
     * now, so the thread will terminate in a controlled way.
     */
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
pthread_setprio(
	pthread_t	thread,
	int		priority)
    {
#if _CMA_KTHREADS_ == _CMA__MACH
    return (cma__set_errno (ENOSYS), -1);
#else
    int			ret_val = 0;
    cma_t_priority	oldpri;


    cma__int_init ();
    TRY {
	cma_thread_get_priority (&thread, &oldpri);
	cma_thread_set_priority (&thread, (cma_t_priority)priority);
	ret_val = (int)oldpri;
	}
    CATCH (exc_e_nopriv) {
	cma__set_errno (EPERM);
	ret_val = -1;
	}
    CATCH (cma_e_use_error) {
	cma__set_errno (ESRCH);
	ret_val = -1;
	}
    CATCH_ALL {
	cma__set_errno (EINVAL);
	ret_val = -1;
	}
    ENDTRY

    return ret_val;
#endif
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
pthread_getprio(
	pthread_t	thread)
    {
#if _CMA_KTHREADS_ == _CMA__MACH
    return (cma__set_errno (ENOSYS), -1);
#else
    int			ret_val = 0;
    cma_t_priority	priority;


    cma__int_init ();
    TRY {
	cma_thread_get_priority (&thread, &priority);
	ret_val = (int)priority;
	}
    CATCH (cma_e_use_error) {
	cma__set_errno (ESRCH);
	ret_val = -1;
	}
    CATCH_ALL {
	cma__set_errno (EINVAL);
	ret_val = -1;
	}
    ENDTRY

    return ret_val;
#endif
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
pthread_setscheduler(
	pthread_t	thread,
	int		scheduler,
	int		priority)
    {
#if _CMA_KTHREADS_ == _CMA__MACH
    return (cma__set_errno (ENOSYS), -1);
#else
    int			ret_val = 0;
    cma_t_sched_policy	policy, oldpolicy;
    cma_t_priority	oldpriority;

    cma__int_init ();

    TRY {
	cma_thread_get_priority (&thread, &oldpriority);
	ret_val = (int)oldpriority;
	}
    CATCH (cma_e_use_error) {
	cma__set_errno (ESRCH);
	ret_val = -1;
	}
    CATCH_ALL {
	cma__set_errno (EINVAL);
	ret_val = -1;
	}
    ENDTRY

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
	    cma__set_errno (EINVAL);
	    return -1;
	    break;
	    }
	}

    TRY {
	cma_thread_get_sched (&thread, &oldpolicy);
	cma_thread_set_sched (&thread, policy, (cma_t_priority)priority);
	}
    CATCH (exc_e_nopriv) {
	cma__set_errno (EPERM);
	ret_val = -1;
	}
    CATCH (cma_e_unimp) {
	cma__set_errno (ENOSYS);
	ret_val = -1;
	}
    CATCH (cma_e_use_error) {
	cma__set_errno (ESRCH);
	ret_val = -1;
	}
    CATCH_ALL {
	cma__set_errno (EINVAL);
	ret_val = -1;
	}
    ENDTRY

    return ret_val;
#endif
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
pthread_getscheduler(
	pthread_t	thread)
    {
#if _CMA_KTHREADS_ == _CMA__MACH
    return (cma__set_errno (ENOSYS), -1);
#else
    int			ret_val = 0;
    cma_t_sched_policy	scheduler;


    cma__int_init ();
    TRY {
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
		cma__bugcheck ("getscheduler:1");
		break;
		}
	    }

	}
    CATCH (cma_e_use_error) {
	cma__set_errno (ESRCH);
	ret_val = -1;
	}
    CATCH_ALL {
	cma__set_errno (EINVAL);
	ret_val = -1;
	}
    ENDTRY

    return ret_val;
#endif
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
pthread_yield(void)
    {
    if (!cma__is_multithreaded())
	return;
    cma__int_init();
    cma_yield ();
    }

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
 *	0 if successful, else -1 (with error in errno)
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
pthread_once(
	pthread_once_t		*once_block,
	pthread_initroutine_t	init_routine)
    {
    int	status = 0;


    cma__int_init ();

    TRY {
	cma_once (
		(cma_t_once *)once_block,
		(cma_t_init_routine)init_routine,
		cma_c_null_ptr);
	}
    CATCH_ALL {
	cma__set_errno (EINVAL);
	status = -1;
	}
    ENDTRY

    return status;
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
 *	0 if successful, else -1 (with error in errno)
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
pthread_keycreate(
	pthread_key_t		*key,
	pthread_destructor_t	destructor)
    {
    int	status = 0;


    cma__int_init ();

    TRY {
	cma_key_create (key, &cma_c_null, destructor);
	}
    CATCH (exc_e_exquota) {
	cma__set_errno (ENOMEM);
	status = -1;
	}
    CATCH (exc_e_insfmem) {
	cma__set_errno (EAGAIN);
	status = -1;
	}
    CATCH_ALL {
	cma__set_errno (EINVAL);
	status = -1;
	}
    ENDTRY

    return status;
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
 *	0 if successful, else -1 (with error in errno)
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
pthread_setspecific(
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

    if ((key <= 0) || (key >= next_context)) {
	cma__set_errno (EINVAL);
	return -1;
	}

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
	    cma__set_errno (ENOMEM);
	    return -1;
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
    return 0;
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
 *	0 if successful, else -1 (with error in errno)
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
pthread_getspecific(
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

    if ((key <= 0) || (key >= next_context)) {
	cma__set_errno (EINVAL);
	return -1;
	}

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

    return 0;
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
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
pthread_mutexattr_create(
	pthread_mutexattr_t	*attr)
    {
    int	status = 0;
    cma__t_int_attr     *att_obj;


    cma__int_init ();
    att_obj = cma__get_attributes (&cma__g_def_attr);

    if ((pthread_addr_t)att_obj == cma_c_null_ptr) {
	cma__set_errno (ENOMEM);
	status = -1;
	}
    else
	cma__object_to_handle ((cma__t_object *)att_obj, (cma_t_attr *)attr);

    return status;
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
 *	0 if successful, else -1
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
pthread_mutexattr_delete(
	pthread_mutexattr_t	*attr)
    {
    cma__t_int_attr     *int_att;


    if (cma__val_nullattr_stat (attr, &int_att) != cma_s_normal) {
	cma__set_errno (EINVAL);
	return -1;
	}

    if (int_att == (cma__t_int_attr *)cma_c_null_ptr)
	return 0;

    cma__free_attributes (int_att);
    cma__clear_handle (attr);
    return 0;
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
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
pthread_mutexattr_setkind_np(
	pthread_mutexattr_t	*attr,
	int			kind)
    {
    int	status = 0;


    TRY {
	cma_attr_set_mutex_kind (attr, (cma_t_mutex_kind)kind);
	}
    CATCH (exc_e_nopriv) {
	cma__set_errno (EPERM);
	status = -1;
	}
    CATCH (cma_e_badparam) {
	cma__set_errno (ERANGE);
	status = -1;
	}
    CATCH_ALL {
	cma__set_errno (EINVAL);
	status = -1;
	}
    ENDTRY

    return status;
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
pthread_mutexattr_getkind_np(
	pthread_attr_t	attr)
    {
    int			ret_val = 0;
    cma_t_mutex_kind	kind;


    TRY {
	cma_attr_get_mutex_kind (&attr, &kind);
	ret_val = (int)kind;
	}
    CATCH_ALL {
	cma__set_errno (EINVAL);
	ret_val = -1;
	}
    ENDTRY

    return ret_val;
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
 *	0 if successful, else -1
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
pthread_mutex_init(
	pthread_mutex_t		*mutex,
	pthread_mutexattr_t	attr)
    {
    cma__t_int_mutex    *imutex;
    cma__t_int_attr     *int_att;


    cma__int_init ();

    if (cma__val_defattr_stat (&attr, &int_att) != cma_s_normal) {
	cma__set_errno (EINVAL);
	return -1;
	}

    imutex = cma__get_mutex (int_att);

    if ((cma_t_address)imutex == cma_c_null_ptr) {
	cma__set_errno (ENOMEM);
	return -1;
	}
    else
	cma__object_to_handle ((cma__t_object *)imutex, mutex);

    return 0;
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
 *	0 if successful, else -1
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
pthread_mutex_destroy(
	pthread_mutex_t	*mutex)
    {
    int	status = 0;


    TRY {
	cma_mutex_delete (mutex);
	}
    CATCH (cma_e_in_use) {
	cma__set_errno (EBUSY);
	status = -1;
	}
    CATCH_ALL {
	cma__set_errno (EINVAL);
	status = -1;
	}
    ENDTRY

    return status;
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
 *	0 if successful, else -1
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
pthread_mutex_lock(
	pthread_mutex_t	*mutex)
    {
    cma__t_int_mutex    *int_mutex;
    cma_t_status	res;
#ifndef NDEBUG
    cma__t_int_tcb	*ltcb;
#endif


    /*
     * Get a pointer to the mutex structure; if this is a debugging CMA,
     * we'll validate the mutex handle to be sure it's valid.  For
     * performance, if it's an NDEBUG ("production") CMA, just fetch the
     * address of the object from the handle's pointer field.
     */
#ifdef NDEBUG
    int_mutex = (cma__t_int_mutex *)((cma__t_int_handle *)mutex)->pointer;

    if (cma__test_and_set (&int_mutex->lock)) {
        res = cma__int_mutex_block (int_mutex);
        /*
	 * CHFts08244. Its possible that we returned here after the mutex
	 * object was destroyed by another thread. Check that it is still 
	 * valid
	 */
	if (((cma__t_int_handle *)mutex)->pointer == cma_c_null_ptr)
	  res = cma_s_existence;
     }   
    else
	res = cma_s_normal;

#else

    if (cma__val_mutex_stat (mutex, &int_mutex) != cma_s_normal) {
	cma__set_errno (EINVAL);
	return -1;
	}

    ltcb = cma__get_self_tcb ();

    if (cma__test_and_set (&int_mutex->lock)) {
        res = cma__int_mutex_block (int_mutex);
        /*
	 * CHFts08244. Its possible that we returned here after the mutex
	 * object was destroyed by another thread. Check that it is still 
	 * valid
	 */
	if (((cma__t_int_handle *)mutex)->pointer == cma_c_null_ptr)
	  res = cma_s_existence;
     }   
    else
	res = cma_s_normal;

#endif

    switch (res) {
	case cma_s_normal : {
#ifndef NDEBUG
	    int_mutex->owner = ltcb;
#endif
	    return 0;
	    }
	case cma_s_in_use : {
	    cma__set_errno (EDEADLK);
	    return -1;
	    }
	default : {
	    cma__set_errno (EINVAL);
	    return -1;
	    }

	}

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
 *	0 if already locked; 1 if successful, -1 if error
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
pthread_mutex_trylock(
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
 *	0 if successful, else -1
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
pthread_mutex_unlock(
	pthread_mutex_t	*mutex)
    {
    cma__t_int_mutex    *int_mutex;
    cma_t_status	res;


#ifdef NDEBUG
    int_mutex = (cma__t_int_mutex *)((cma__t_int_handle *)mutex)->pointer;
#else
    cma__t_int_tcb	*utcb;


    if (cma__val_mutex_stat (mutex, &int_mutex) != cma_s_normal) {
	cma__set_errno (EINVAL);
	return -1;
	}

    utcb = cma__get_self_tcb ();

    if (int_mutex->mutex_kind == cma_c_mutex_fast) {
	cma__assert_warn (
		(utcb == int_mutex->owner),
		"attempt to release mutex owned by another thread");
	int_mutex->owner = (cma__t_int_tcb *)cma_c_null_ptr;
	}

#endif
    cma__unset (int_mutex->unlock);

    if (!cma__test_and_set (&int_mutex->event)) {
	res = cma__int_mutex_unblock (int_mutex);

	if (res != cma_s_normal) {
	    cma__set_errno (EINVAL);
	    return -1;
	    }

	}

    return 0;
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
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
pthread_condattr_create(
	pthread_condattr_t	*attr)
    {
    int	status = 0;
    cma__t_int_attr     *att_obj;


    cma__int_init ();
    att_obj = cma__get_attributes (&cma__g_def_attr);

    if ((pthread_addr_t)att_obj == cma_c_null_ptr) {
	cma__set_errno (ENOMEM);
	status = -1;
	}
    else
	cma__object_to_handle ((cma__t_object *)att_obj, (cma_t_attr *)attr);

    return status;
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
 *	0 if successful, else -1
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
pthread_condattr_delete(
	pthread_condattr_t	*attr)
    {
    cma__t_int_attr     *int_att;


    if (cma__val_nullattr_stat (attr, &int_att) != cma_s_normal) {
	cma__set_errno (EINVAL);
	return -1;
	}

    if (int_att == (cma__t_int_attr *)cma_c_null_ptr)
	return 0;

    cma__free_attributes (int_att);
    cma__clear_handle (attr);
    return 0;
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
 *	0 if successful, else -1
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
pthread_cond_init(
	pthread_cond_t		*cond,
	pthread_condattr_t	attr)
    {
    cma__t_int_cv	*cv; 
    cma__t_int_attr	*int_att; 


    cma__int_init ();

    if (cma__val_defattr_stat (&attr, &int_att) != cma_s_normal) {
	cma__set_errno (EINVAL);
	return -1;
	}

    cv = cma__get_cv (int_att);

    if ((cma_t_address)cv == cma_c_null_ptr) {
	cma__set_errno (ENOMEM);
	return -1;
	}
    else
	cma__object_to_handle ((cma__t_object *)cv, cond);

    return 0;
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
 *	0 if successful, else -1
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
pthread_cond_destroy(
	pthread_cond_t	*cond)
    {
    int	status = 0;


    TRY {
	cma__int_cond_delete (cond);
	}
    CATCH (cma_e_in_use) {
	cma__set_errno (EBUSY);
	status = -1;
	}
    CATCH_ALL {
	cma__set_errno (EINVAL);
	status = -1;
	}
    ENDTRY

    return status;
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
 *	0 if successful, else -1
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
pthread_cond_signal(
	pthread_cond_t	*cond)
    {
    int	status = 0;
    cma__t_int_cv       *int_cv;


    TRY {
#ifdef NDEBUG
	int_cv = (cma__t_int_cv *)((cma__t_int_handle *)cond)->pointer;
#else
	int_cv = cma__validate_cv (cond);
#endif
	cma__int_signal (int_cv);
	}
    CATCH_ALL {
	cma__set_errno (EINVAL);
	status = -1;
	}
    ENDTRY

    return status;
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
 *	0 if successful, else -1
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
pthread_cond_signal_int_np(
	pthread_cond_t	*cond)
    {
    int	status = 0;


    TRY {
	cma_cond_signal_int (cond);
	}
    CATCH_ALL {
	cma__set_errno (EINVAL);
	status = -1;
	}
    ENDTRY

    return status;
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
 *	0 if successful, else -1
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
pthread_cond_broadcast(
	pthread_cond_t	*cond)
    {
    int	status = 0;
    cma__t_int_cv       *int_cv;

                      
    TRY {
#ifdef NDEBUG
	int_cv = (cma__t_int_cv *)((cma__t_int_handle *)cond)->pointer;
#else
	int_cv = cma__validate_cv (cond);
#endif
	cma__int_broadcast (int_cv);
	}
    CATCH_ALL {
	cma__set_errno (EINVAL);
	status = -1;
	}
    ENDTRY

    return status;
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
 *	0 if successful, else -1
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
pthread_cond_wait(
	pthread_cond_t	*cond,
	pthread_mutex_t	*mutex)
    {
    int	status = 0;


    TRY {
	cma__int_cond_wait (cond, mutex);
	}
    CATCH (cma_e_alerted) {
	/*
	 * This exception is used to implement pthread_cancel; as such, the
	 * exception must be allowed to propagate out of the wait call so
	 * that the cleanup handlers can catch it, and eventually the thread
	 * will terminate.
	 */
	RERAISE;
	}
    CATCH_ALL {
	cma__set_errno (EINVAL);
	status = -1;
	}
    ENDTRY

    return status;
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
 *	0 if successful, else -1
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
pthread_cond_timedwait(
	pthread_cond_t	*cond,
	pthread_mutex_t	*mutex,
	struct timespec	*abstime)
    {
    int			status = 0;
    cma_t_date_time	time;
#if _CMA_OS_ == _CMA__VMS
    cma_t_date_time	delta;
    int			vstat;
    int                 delta_secs;
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

    if ((vstat & 7) != 1) {
	cma__set_errno (EINVAL);
	return -1;
	}

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

    TRY {

	if (cma_cond_timed_wait (cond, mutex, &time) == cma_s_timed_out) {
	    cma__set_errno (EAGAIN);
	    status = -1;
	    }

	}
    CATCH (cma_e_alerted) {
	/*
	 * This exception is used to implement pthread_cancel; as such, the
	 * exception must be allowed to propagate out of the wait call so
	 * that the cleanup handlers can catch it, and eventually the thread
	 * will terminate.
	 */
	RERAISE;
	}
    CATCH_ALL {
	cma__set_errno (EINVAL);
	status = -1;
	}
    ENDTRY

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
 *	0 if successful, else -1
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
pthread_cancel(
	pthread_t	thread)
    {
    int	status = 0;


    TRY {
	cma_thread_alert (&thread);
	}
    CATCH (cma_e_use_error) {
	cma__set_errno (ESRCH);
	status = -1;
	}
    CATCH_ALL {
	cma__set_errno (EINVAL);
	status = -1;
	}
    ENDTRY

    return status;
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
pthread_testcancel(void)
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
pthread_setcancel(int	state)
    {
    int			previous;
    cma__t_int_tcb	*tcb;


    cma__int_init ();

    if (state != CANCEL_ON && state != CANCEL_OFF) {
	cma__set_errno (EINVAL);
	return -1;
	}

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
pthread_setasynccancel(int	state)
    {
    int			previous;
    cma__t_int_tcb	*tcb;


    cma__int_init ();

    if (state != CANCEL_ON && state != CANCEL_OFF) {
	cma__set_errno (EINVAL);
	return -1;
	}

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
 *	0 if successful, else -1 and errno set to error code
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
pthread_get_expiration_np(struct timespec	*delta,
	struct timespec		*abstime)
    {
    struct timespec	current_time;


    if (delta->tv_nsec >= (1000 * 1000000) || delta->tv_nsec < 0) {
	cma__set_errno (EINVAL);
	return -1;
	}

    cma__gettimespec (&current_time);
    abstime->tv_nsec	= delta->tv_nsec + (current_time.tv_nsec);
    abstime->tv_sec	= delta->tv_sec + current_time.tv_sec;

    if (abstime->tv_nsec >= (1000 * 1000000)) {	/* check for carry */
	abstime->tv_nsec -= (1000 * 1000000);
	abstime->tv_sec += 1;
	}

    return 0;
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
 *	0 if successful, else -1 and errno set to error code
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern int
pthread_delay_np(struct timespec	*interval)
    {
    cma_t_date_time	time;


    cma__int_init ();

    if ((cma_t_address)interval == cma_c_null_ptr ||
		interval->tv_nsec >= (1000 * 1000000) || interval->tv_nsec < 0) {
	cma__set_errno (EINVAL);
	return -1;
	}

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
    return 0;
    }
#if (_CMA_UNIX_TYPE == _CMA__SVR4)

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Wait for interval.  This is different from pthread_delay_np() in
 *  that the parameter is an unsigned int, rather than a timespec.
 *
 *  FORMAL PARAMETERS:
 *
 *	sec	unsigned int
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
 *	0 if successful, else -1 and errno set to error code
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern unsigned int
pthread_delay_unsigned_np(unsigned sec)
    {
    struct timespec     interval;

	interval.tv_sec = sec;
	interval.tv_nsec = 0;
	return(pthread_delay_np(&interval));
    }
#endif /* _CMA_UNIX_TYPE == _CMA__SVR4 */

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
pthread_lock_global_np(void)
    {
    cma__int_init();
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
pthread_unlock_global_np(void)
    {
    cma__int_init();
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
extern int
pthread_signal_to_cancel_np(sigset_t	*sigset,
	pthread_t	*thread)
    {
    int	status = 0;

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
    CATCH_ALL {
	cma__int_unlock (cma__g_global_lock);
	status = -1;
	cma__set_errno (EINVAL);
	}
    ENDTRY

    if (status == 0)
	cma__int_unlock (cma__g_global_lock);

    return status;
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	The start routine for a thread that simply sigwaits; if sigwait()
 *	returns successfully, then a target thread is cancelled; if it
 *	returns with an error, the thread exits.
 *
 *  FORMAL PARAMETERS:
 *
 *	none
 *
 *  IMPLICIT INPUTS:
 *
 *	cma__g_handle_sigset	Signal set to wait on
 *	cma__g_handle_target	Thread to cancel
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
 *	Cancel target thread when signal occurs.
 */
extern void
pthread__cancel_thread(void)
    {

    while (1) {

	/*
	 * sigwait() on the signal set until canceled, or until sigwait()
	 * complains (which ought to be on the first try, if at all). Then
	 * just let the thread run down.
	 */
	if (cma_sigwait (&cma__g_handle_sigset) == -1)
	    return;

	cma_thread_alert (&cma__g_handle_target);
	}

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
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_PTHREAD.C */
/*  *25   16-MAR-1992 09:04:18 BUTENHOF "Fix typo in NDEBUG code" */
/*  *24   13-MAR-1992 14:09:27 BUTENHOF "Fix mutex lock/unlock code" */
/*  *23   10-MAR-1992 16:26:54 BUTENHOF "Eliminate need for TRY/CATCH on pthread mutex lock" */
/*  *22   19-FEB-1992 07:16:20 BUTENHOF "Fix type cast" */
/*  *21   18-FEB-1992 15:30:00 BUTENHOF "Adapt to alloc_mem changes" */
/*  *20   18-DEC-1991 06:45:25 BUTENHOF "Fix timedwait" */
/*  *19    6-DEC-1991 07:19:50 BUTENHOF "Add background policy to attr_setsched" */
/*  *18   20-NOV-1991 12:34:54 CURTIN "Alpha work" */
/*  *17   18-NOV-1991 11:22:36 BUTENHOF "Modify signal_to_cancel_np" */
/*  *16   14-OCT-1991 13:39:32 BUTENHOF "Add get/set guardsize functions" */
/*  *15   27-AUG-1991 17:48:12 BUTENHOF "Fix pthread_join to accept null status" */
/*  *14   10-JUN-1991 18:22:48 SCALES "Add sccs headers for Ultrix" */
/*  *13    5-JUN-1991 18:37:56 BUTENHOF "Include cma_px.h" */
/*  *12   29-MAY-1991 17:58:50 BUTENHOF "Return ENOSUP for sched ops on Mach threads" */
/*  *11   14-MAY-1991 12:44:29 CURTIN "Added some macros" */
/*  *10    2-MAY-1991 13:58:45 BUTENHOF "Add argument to cma__bugcheck() calls" */
/*  *9    12-APR-1991 23:36:23 BUTENHOF "Change errno access for OSF/1" */
/*  *8     3-APR-1991 13:40:31 BUTENHOF "Fix signal_to_cancel_np()" */
/*  *7     1-APR-1991 18:09:16 BUTENHOF "Add pthread_signal_to_cancel_np()" */
/*  *6    22-MAR-1991 13:43:05 BUTENHOF "Check param value for setasynccancel, too" */
/*  *5    21-MAR-1991 09:26:39 BUTENHOF "Fix some return statuses" */
/*  *4    12-FEB-1991 01:29:19 BUTENHOF "Redefine alert state" */
/*  *3     6-FEB-1991 01:33:00 BUTENHOF "Fix internal timed_wait call" */
/*  *2    24-JAN-1991 00:35:03 BUTENHOF "Fix exception name references" */
/*  *1    12-DEC-1990 21:48:32 BUTENHOF "P1003.4a support" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_PTHREAD.C */
