/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: cma_kernel.h $Revision: 1.1.9.1 $ $Date: 1996/05/10 13:38:29 $
 */
/*
 * HISTORY
 * $Log: cma_kernel.h,v $
 * Revision 1.1.9.1  1996/05/10  13:38:29  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:42 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:43 UTC  jss  /main/HPDCE02/1]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  16:12  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/21  13:05:32  truitt]
 * 	 *
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:43:41  root  1.1.10.3]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:12:43  hopkins]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  18:23:49  root]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:33:47  hopkins]
 *
 * 	Gamera merge
 * 	[1993/03/26  23:55:08  hopkins]
 *
 * 	Loading drop DCE1_0_2B18
 * 	[1993/03/11  22:27:24  osfrcs  1.1.1.8]
 *
 * 	DCE 1.0.1 Release
 * 	[1992/09/23  20:38:15  hopkins]
 *
 * Revision 1.1.5.2  1994/06/09  13:38:13  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:06  devsrc]
 * 
 * Revision 1.1.5.1  1994/04/01  20:16:37  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:17:54  jd]
 * 
 * Revision 1.1.2.3  1992/12/15  22:28:42  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:39:27  alan]
 * 
 * Revision 1.1.2.2  1992/09/03  14:38:19  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:04:39  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/08  23:31:37  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:31:19  bolinger]
 * 
 * Revision 1.1  1992/01/19  14:40:33  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *	%W%	(DEC OSF/1)	%G%
 */
/*
 *  Copyright (c) 1990, 1992 by
 *  Digital Equipment Corporation, Maynard Massachusetts.
 *  All rights reserved.
 *
 * This software is furnished under a licence and may be used and
 * copied only in accordance of the terms of such license and
 * with the inclusion of the above copyright notice. This
 * software or any other copies thereof may be provided or
 * otherwise made available to any other person. No title to and
 * ownership of the software is herby transferred.
 *
 * The information in this software is subject to change without
 * notice and should not be construed as a commitment by Digital
 * Equipment Corporation.
 *
 * Digital assumes no responability for the use or reliability
 * of its software on equipment which is not supplied by Digital.
 */

/*
 * FACILITY:
 *
 *	CMA services
 *
 * ABSTRACT:
 *
 *	This module defines the interface for locking and unlocking the kernel
 *	scheduling database.
 *
 * AUTHORS:
 *
 *	Dave Butenhof
 *
 * CREATION DATE:
 *
 *	14 June 1990
 *
 * MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof & Webb Scales	03 August 1990
 *		Include cma_defer.h for deferral code
 *	002	Dave Butenhof	09 April 1991
 *		Use new type for "atomic bit" operation target
 *	003	Dave Butenhof	26 April 1991
 *		Optimize cma__enter_kernel() for uniprocessors.
 *	004	Dave Butenhof	08 May 1991
 *		Add new test macro to assert that kernel isn't already locked
 *		on uniprocessors (but evaporate on multiprocessors).
 *	005	Dave Butenhof	13 May 1991
 *		Conditionalize extern declarations for recorded kernel
 *		operations.
 *	006	Dave Butenhof	03 June 1991
 *		Uniprocessor cma__enter_kernel() call will bugcheck if kernel
 *		was already locked.
 *	007	Dave Butenhof	11 June 1991
 *		Add formatting function for kernel trace array.
 *	008	Webb Scales	13 February 1992
 *		Perform undeferrals on entering the kernel
 */

#ifndef CMA_KERNEL
#define CMA_KERNEL

/*
 * INCLUDE FILES
 */

#include <cma_defs.h>
#include <cma_defer.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * CONSTANTS AND MACROS
 */

#if _CMA_UNIPROCESSOR_
    /*
     * On a uniprocessor, DECthreads does all context switching of threads in
     * user mode within a single kernel context (process). It is impossible
     * for two threads to contend for the kernel lock simultaneously.
     * Therefore, when a thread attempts to lock the kernel, the lock must
     * always be clear.  Even if the thread is preempted by a timeslice
     * between reading the value of the lock and setting the lock (on
     * hardware without an atomic test-and-set), the lock must have been
     * restored to 0 before it continues at the set operation (or it could
     * not be running).
     *
     * Therefore, a loop is useless; the only way the lock can already be
     * set is if it's going to hang (because the *current thread* already has
     * it locked).  Instead, we'll declare a bugcheck if the lock is already
     * set.  This probably isn't as easy to diagnose as a hang (which leaves
     * the state intact), but we can't use a loop because the MIPS
     * uniprocessor cma__test_and_set() function uses cma__enter_kernel()
     * within a comma-list expression, and a loop isn't allowed.
     *
     * Additionally, define a macro that will test whether the kernel is
     * currently locked, and declare an assertion error if so: this is useful
     * on uniprocessors to detect programming errors that could result in
     * DECthreads hangs, but they are incorrect on multiprocessor
     * implementations where another thread may hold the kernel lock.
     */
# ifdef _CMA_TRACE_KERNEL_
#  define cma__enter_kernel() cma__enter_kern_record (__LINE__, __FILE__)
#  define cma__set_kernel() cma__set_kern_record (__LINE__, __FILE__)
#  define cma__tryenter_kernel() cma__tryenter_kern_record (__LINE__, __FILE__)
# else
#  define cma__enter_kernel() \
	(cma__kernel_set (&cma__g_kernel_critical) ? \
	    (cma__bugcheck ("enter_kernel: deadlock"), 0) : \
	    (!cma__kernel_set (&cma__g_defers_processed) ? cma__undefer () : 0))
#  define cma__set_kernel() \
	(cma__kernel_set (&cma__g_kernel_critical) ? \
	    (cma__bugcheck ("set_kernel: deadlock"), 0) : 0)
#  define cma__tryenter_kernel() (cma__kernel_set (&cma__g_kernel_critical))
# endif
# if _CMA_VENDOR_ == _CMA__HP
# define cma__assert_not_kernel() \
    (cma__tac_isset (&cma__g_kernel_critical) ? \
	    (cma__bugcheck("About to enter kernel when already in kernel\n    at line %d in %s.", __LINE__, __FILE__), 0) : 0)
# else
# define cma__assert_not_kernel() \
    cma__assert_fail ( \
	    !cma__tac_isset (&cma__g_kernel_critical), \
	    "About to enter kernel when already in kernel.");
# endif
#else
# ifdef _CMA_TRACE_KERNEL_
#  define cma__enter_kernel() cma__enter_kern_record (__LINE__, __FILE__)
#  define cma__set_kernel() cma__set_kern_record (__LINE__, __FILE__)
#  define cma__tryenter_kernel() cma__tryenter_kern_record (__LINE__, __FILE__)
# else
#  define cma__enter_kernel() { \
	while (cma__kernel_set (&cma__g_kernel_critical)); \
	if (!cma__kernel_set (&cma__g_defers_processed))  cma__undefer (); \
	}
#  define cma__set_kernel() while (cma__kernel_set (&cma__g_kernel_critical))
#  define cma__tryenter_kernel() (cma__kernel_set (&cma__g_kernel_critical))
# endif
# define cma__assert_not_kernel()
#endif

/*
 * NOTE: the test-and-set of "cma__g_defers_processed" is implemented with a
 * "cma__kernel_set" operation, even though it's an arbitrary bit, because we
 * know the kernel is already locked when we try it, so it can't be
 * interrupted.
 */
#ifdef _CMA_TRACE_KERNEL_
# define cma__exit_kernel() cma__exit_kern_record (__LINE__, __FILE__)
# define cma__unset_kernel() cma__unset_kern_record (__LINE__, __FILE__)
#else
# define cma__exit_kernel() ( \
    cma__assert_fail ( \
	    cma__tac_isset (&cma__g_kernel_critical), \
	    "cma_exit_kernel:  kernel critical already unlocked"), \
    (!cma__kernel_set (&cma__g_defers_processed) ? cma__undefer ():0), \
    cma__kernel_unset (&cma__g_kernel_critical))
# define cma__unset_kernel() (cma__kernel_unset (&cma__g_kernel_critical))
#endif

/*
 * TYPEDEFS
 */

/*
 * GLOBAL DATA
 */

extern cma__t_atomic_bit	cma__g_kernel_critical;	/* CMA in krnl */

/*
 * INTERNAL INTERFACES
 */

# ifdef _CMA_TRACE_KERNEL_
extern void cma__enter_kern_record (cma_t_integer,	char *);

extern void cma__exit_kern_record (cma_t_integer,char *);

extern void cma__format_karray (void);

extern cma_t_boolean cma__tryenter_kern_record (cma_t_integer,char *);

extern void cma__unset_kern_record (cma_t_integer,char *);
# endif

#ifdef __cplusplus
}
#endif

#endif
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_KERNEL.H */
/*  *13   19-FEB-1992 13:50:29 SCALES "Undefer on enter-kernel" */
/*  *12   14-OCT-1991 13:39:13 BUTENHOF "Refine/fix use of config symbols" */
/*  *11   11-JUN-1991 17:17:10 BUTENHOF "Add & use functions to dump kernel/sem trace arrays" */
/*  *10   10-JUN-1991 19:54:01 SCALES "Convert to stream format for ULTRIX build" */
/*  *9    10-JUN-1991 19:21:03 BUTENHOF "Fix the sccs headers" */
/*  *8    10-JUN-1991 18:22:16 SCALES "Add sccs headers for Ultrix" */
/*  *7     3-JUN-1991 17:13:08 BUTENHOF "Uniproc enter_kernel() should bugcheck if was set" */
/*  *6    29-MAY-1991 17:14:50 BUTENHOF "Change definition of kernel record symbol" */
/*  *5    14-MAY-1991 13:43:29 BUTENHOF "Add kernel test macro" */
/*  *4    10-MAY-1991 16:18:52 BUTENHOF "Add kernel test macro" */
/*  *3     2-MAY-1991 13:58:28 BUTENHOF "Optimize enter_kernel for uniprocessor" */
/*  *2    12-APR-1991 23:36:02 BUTENHOF "Change type of internal locks" */
/*  *1    12-DEC-1990 21:46:52 BUTENHOF "Kernel lock support" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_KERNEL.H */
