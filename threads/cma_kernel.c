/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cma_kernel.c,v $
 * Revision 1.1.10.1  1996/05/10  13:38:16  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:42 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:43 UTC  jss  /main/HPDCE02/1]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  16:14  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/21  13:02:41  truitt]
 * 	 *
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:43:39  root  1.1.4.3]
 *
 * 	OT8416: changed .eseq to .xseq in cma__enter_kern_record().
 * 	[1993/08/20  18:39:36  truitt]
 *
 * 	Changed "eseq" to "xseq" in cma__enter_kern_record() for CR 8416.
 * 	[1993/08/20  16:09:24  rhw]
 *
 * 	Loading drop DCE1_0_2FNL
 * 	[1993/05/04  22:34:26  mgm  1.1.1.11]
 *
 * Revision 1.1.6.2  1994/06/09  13:38:11  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:04  devsrc]
 * 
 * Revision 1.1.6.1  1994/04/01  20:16:35  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:17:51  jd]
 * 
 * Revision 1.1.4.2  1993/08/20  20:48:53  rhw
 * 	Changed "eseq" to "xseq" in cma__enter_kern_record() for CR 8416.
 * 	[1993/08/20  16:09:24  rhw]
 * 
 * Revision 1.1.2.3  1992/12/15  22:28:37  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:39:19  alan]
 * 
 * Revision 1.1.2.2  1992/09/03  14:38:13  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:04:34  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/08  23:30:29  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:30:05  bolinger]
 * 
 * Revision 1.1  1992/01/19  14:40:21  devrcs
 * 	Initial revision
 * 
 * $EndLog$
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
 * notice and should not be construed as a commitment by DIGITAL
 * Equipment Corporation.
 *
 * DIGITAL assumes no responability for the use or reliability
 * of its software on equipment which is not supplied by DIGITAL.
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
 *	001	Dave Butenhof	09 April 1991
 *		Use new type for "atomic bit" operation target
 *	002	Dave Butenhof	10 May 1991
 *		Add new global data for debug, and recording kernel
 *		functions.
 *	003	Dave Butenhof	13 May 1991
 *		Add recording "tryenter_kernel" function.
 *	004	Dave Butenhof	17 May 1991
 *		Have enter kernel functions check for multiple entry.
 *	005	Dave Butenhof	11 June 1991
 *		Add formatting function for kernel trace array.
 *	006	Dave Butenhof	25 July 1991
 *		Support debug build without VAXCRTL on VMS by using internal
 *		*printf emulation functions (#defined to real functions on
 *		UNIX platforms).
 *	007	Paul Curtin	20 August 1991
 *		Conditionalized out the include of stdio.h on VMS.
 *	008	Dave Butenhof	17 September 1991
 *		Fix cma__format_karray() to close the file.
 *	009	Dave Butenhof	10 October 1991
 *		Change format karray routine to go to stdout.
 *	010	Webb Scales	13 February 1992
 *		Perform undeferrals on enter-kernel.
 *	011	Dave Butenhof	24 March 1992
 *		Modify format karray to use redirectable I/O
 */

/*
 * INCLUDE FILES
 */

#include <cma.h>
#include <cma_defs.h>
#include <cma_kernel.h>
#include <cma_stack.h>
#include <cma_tcb_defs.h>
#if _CMA_TRACE_KERNEL_
# if _CMA_OS_ == _CMA__UNIX
#  include <stdio.h>
# endif
#endif

/*
 * CONSTANTS AND MACROS
 */

/*
 * TYPEDEFS
 */

#ifdef _CMA_TRACE_KERNEL_
typedef struct CMA___T_KARRAY {
    cma_t_integer	eline;
    char		*efile;
    cma__t_int_tcb	*ethd;
    cma_t_integer	eseq;
    cma_t_boolean	tryenter;
    cma_t_boolean	locked;
    cma_t_integer	xline;
    char		*xfile;
    cma__t_int_tcb	*xthd;
    cma_t_integer	xseq;
    cma_t_boolean	unlock;
    cma_t_boolean	unset;
    } cma___t_karray;
#endif

/*
 * GLOBAL DATA
 */

/*
 * Set when scheduling database is locked by a thread
 */
cma__t_atomic_bit	cma__g_kernel_critical = cma__c_tac_static_clear;

/*
 * LOCAL DATA
 */

#ifdef _CMA_TRACE_KERNEL_
static cma_t_integer	cma___g_kidx = 0;
static cma___t_karray	cma___g_karray[_CMA_TRACE_KERNEL_];
#endif

/*
 * INTERNAL INTERFACES
 */

#ifdef _CMA_TRACE_KERNEL_

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	This routine locks the kernel; normally, the inline macro version
 *	(cma__enter_kernel) is used instead, but this function is called when
 *	running in a non-production (!defined(NDEBUG)) environment with
 *	kernel lock recording (_CMA_TRACE_KERNEL_) enabled.  Mostly just
 *	because the code is sufficiently large and slow that the call
 *	probably doesn't matter: but also because it provides a way to set a
 *	breakpoint at kernel entry and exit (as well as being able to
 *	redefine the recording code without recompiling every DECthreads
 *	module).
 *
 *  FORMAL PARAMETERS:
 *
 *	line		Line number of call
 *	file		File name of call
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
cma__enter_kern_record(
	cma_t_integer	line,
	char		*file)
    {
    cma__t_int_tcb	*self = cma__get_self_tcb ();


    while (cma__kernel_set (&cma__g_kernel_critical));	/* Do the lock */

    cma__assert_fail (
	    !cma___g_karray[cma___g_kidx].locked,
	    "enter kernel succeeded when kernel was already locked");

    cma___g_karray[cma___g_kidx].locked = cma_c_true;
    cma___g_karray[cma___g_kidx].eline = line;
    cma___g_karray[cma___g_kidx].efile = file;
    cma___g_karray[cma___g_kidx].ethd = self;
    cma___g_karray[cma___g_kidx].eseq = self->header.sequence;
    cma___g_karray[cma___g_kidx].xline = 0;
    cma___g_karray[cma___g_kidx].xfile = (char *)cma_c_null_ptr;
    cma___g_karray[cma___g_kidx].xthd = (cma__t_int_tcb *)cma_c_null_ptr;
    cma___g_karray[cma___g_kidx].xseq = 0;
    cma___g_karray[cma___g_kidx].tryenter = cma_c_false;
    cma___g_karray[cma___g_kidx].unlock = cma_c_false;
    cma___g_karray[cma___g_kidx].unset = cma_c_false;

    if (!cma__kernel_set (&cma__g_defers_processed))  cma__undefer ();
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	This routine unlocks the kernel; normally, the inline macro version
 *	(cma__enter_kernel) is used instead, but this function is called when
 *	running in a non-production (!defined(NDEBUG)) environment with
 *	kernel lock recording (_CMA_TRACE_KERNEL_) enabled.  Mostly just
 *	because the code is sufficiently large and slow that the call
 *	probably doesn't matter: but also because it provides a way to set a
 *	breakpoint at kernel entry and exit (as well as being able to
 *	redefine the recording code without recompiling every DECthreads
 *	module).
 *
 *  FORMAL PARAMETERS:
 *
 *	line		Line number of call
 *	file		File name of call
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
cma__exit_kern_record(
	cma_t_integer	line,
	char		*file)
    {
    cma__t_int_tcb	*self = cma__get_self_tcb ();


    cma___g_karray[cma___g_kidx].xline = line;
    cma___g_karray[cma___g_kidx].xfile = file;
    cma___g_karray[cma___g_kidx].xthd = self;
    cma___g_karray[cma___g_kidx].xseq = self->header.sequence;

    cma__assert_fail (
	    cma__tac_isset (&cma__g_kernel_critical),
	    "cma_exit_kernel:  kernel critical already unlocked");

    cma___g_karray[cma___g_kidx].unlock = cma_c_true;
    cma___g_karray[cma___g_kidx].locked = cma_c_false;

    if (cma___g_kidx >= _CMA_TRACE_KERNEL_-1)
	cma___g_kidx = 0;
    else
	cma___g_kidx++;

    if (!cma__kernel_set (&cma__g_defers_processed))
	cma__undefer ();

    cma__kernel_unset (&cma__g_kernel_critical);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	This routine formats the entries in cma___g_karray to assist in
 *	debugging.
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
cma__format_karray(void)
    {
    cma_t_integer	i, empty = -1;
    char		buffer[256];


    buffer[0] = '\0';
    cma__putstring (buffer, "Dump of kernel trace array:");
    cma__puteol (buffer);
    cma__puteol (buffer);
    cma__putformat (buffer, "\tCurrent active index is %d", cma___g_kidx);
    cma__puteol (buffer);

    for (i = 0; i < _CMA_TRACE_KERNEL_; i++) {

	if (!cma___g_karray[i].locked && !cma___g_karray[i].unlock) {
	    /*
	     * If this entry is empty, then just skip it. If it's the first
	     * in a series of empty entries, record the index.
	     */
	    if (empty == -1) empty = i;
	    }
	else {
	    /*
	     * If we've found a non-empty entry, then display it. First, if
	     * we just completed a series of empty entries, report the
	     * range.
	     */
	    if (empty != -1) {
		cma__putformat (
			buffer,
			"[[Entries %d to %d are empty]]",
			empty,
			i-1);
		cma__puteol (buffer);
		empty = -1;
		}

	    cma__putformat (
		    buffer,
		    "[%03d] %sentry thd %08x (%d) from \"%s\":%d,",
		    i,
		    (cma___g_karray[i].tryenter ? "try" : ""),
		    cma___g_karray[i].ethd,
		    cma___g_karray[i].eseq,
		    cma___g_karray[i].efile,
		    cma___g_karray[i].eline);
	    cma__puteol (buffer);

	    if (cma___g_karray[i].locked)
		cma__putstring (buffer, "  never unlocked.");
	    else
		cma__putformat (
			buffer,
			"  %s thd %08x (%d) from \"%s\":%d.",
			(cma___g_karray[i].unset ? "unset" : "exit"),
			cma___g_karray[i].xthd,
			cma___g_karray[i].xseq,
			cma___g_karray[i].xfile,
			cma___g_karray[i].xline);

	    cma__puteol (buffer);
	    }

	}

    if (empty != -1) {
	cma__putformat (
		buffer,
		"[[Entries %d to %d are empty]]",
		empty,
		_CMA_TRACE_KERNEL_-1);
	cma__puteol (buffer);
	empty = -1;
	}

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	This routine locks the kernel; normally, the inline macro version
 *	(cma__set_kernel) is used instead, but this function is called when
 *	running in a non-production (!defined(NDEBUG)) environment with
 *	kernel lock recording (_CMA_TRACE_KERNEL_) enabled.  Mostly just
 *	because the code is sufficiently large and slow that the call
 *	probably doesn't matter: but also because it provides a way to set a
 *	breakpoint at kernel entry and exit (as well as being able to
 *	redefine the recording code without recompiling every DECthreads
 *	module).
 *
 *  FORMAL PARAMETERS:
 *
 *	line		Line number of call
 *	file		File name of call
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
cma__set_kern_record(
	cma_t_integer	line,
	char		*file)
    {
    cma__t_int_tcb	*self = cma__get_self_tcb ();


    while (cma__kernel_set (&cma__g_kernel_critical));	/* Do the lock */

    cma__assert_fail (
	    !cma___g_karray[cma___g_kidx].locked,
	    "set kernel succeeded when kernel was already locked");

    cma___g_karray[cma___g_kidx].locked = cma_c_true;
    cma___g_karray[cma___g_kidx].eline = line;
    cma___g_karray[cma___g_kidx].efile = file;
    cma___g_karray[cma___g_kidx].ethd = self;
    cma___g_karray[cma___g_kidx].eseq = self->header.sequence;
    cma___g_karray[cma___g_kidx].xline = 0;
    cma___g_karray[cma___g_kidx].xfile = (char *)cma_c_null_ptr;
    cma___g_karray[cma___g_kidx].xthd = (cma__t_int_tcb *)cma_c_null_ptr;
    cma___g_karray[cma___g_kidx].eseq = 0;
    cma___g_karray[cma___g_kidx].tryenter = cma_c_false;
    cma___g_karray[cma___g_kidx].unlock = cma_c_false;
    cma___g_karray[cma___g_kidx].unset = cma_c_false;
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	This routine attempts to lock the kernel and returns the previous
 *	state; normally, the inline macro version (cma__enter_kernel) is used
 *	instead, but this function is called when running in a non-production
 *	(!defined(NDEBUG)) environment with kernel lock recording
 *	(_CMA_TRACE_KERNEL_) enabled.  Mostly just because the code is
 *	sufficiently large and slow that the call probably doesn't matter:
 *	but also because it provides a way to set a breakpoint at kernel
 *	entry and exit (as well as being able to redefine the recording code
 *	without recompiling every DECthreads module).
 *
 *  FORMAL PARAMETERS:
 *
 *	line		Line number of call
 *	file		File name of call
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
extern cma_t_boolean
cma__tryenter_kern_record(
	cma_t_integer	line,
	char		*file)
    {
    cma__t_int_tcb	*self = cma__get_self_tcb ();


    if (cma__kernel_set (&cma__g_kernel_critical))
	return cma_c_true;		/* Return TRUE if already locked */
    else {
	cma__assert_fail (
		!cma___g_karray[cma___g_kidx].locked,
		"enter kernel succeeded when kernel was already locked");

	cma___g_karray[cma___g_kidx].locked = cma_c_true;
	cma___g_karray[cma___g_kidx].eline = line;
	cma___g_karray[cma___g_kidx].efile = file;
	cma___g_karray[cma___g_kidx].ethd = self;
	cma___g_karray[cma___g_kidx].eseq = self->header.sequence;
	cma___g_karray[cma___g_kidx].xline = 0;
	cma___g_karray[cma___g_kidx].xfile = (char *)cma_c_null_ptr;
	cma___g_karray[cma___g_kidx].xthd = (cma__t_int_tcb *)cma_c_null_ptr;
	cma___g_karray[cma___g_kidx].xseq = 0;
	cma___g_karray[cma___g_kidx].tryenter = cma_c_true;
	cma___g_karray[cma___g_kidx].unlock = cma_c_false;
	cma___g_karray[cma___g_kidx].unset = cma_c_false;
	return cma_c_false;
	}

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	This routine unlocks the kernel without processing deferrals;
 *	normally, the inline macro version (cma__kernel_unset) is used
 *	instead, but this function is called when running in a non-production
 *	(!defined(NDEBUG)) environment with kernel lock recording
 *	(_CMA_TRACE_KERNEL_) enabled.  Mostly just because the code is
 *	sufficiently large and slow that the call probably doesn't matter:
 *	but also because it provides a way to set a breakpoint at kernel
 *	entry and exit (as well as being able to redefine the recording code
 *	without recompiling every DECthreads module).
 *
 *  FORMAL PARAMETERS:
 *
 *	line		Line number of call
 *	file		File name of call
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
cma__unset_kern_record(
	cma_t_integer	line,
	char		*file)
    {
    cma__t_int_tcb	*self = cma__get_self_tcb ();


    cma___g_karray[cma___g_kidx].xline = line;
    cma___g_karray[cma___g_kidx].xfile = file;
    cma___g_karray[cma___g_kidx].xthd = self;
    cma___g_karray[cma___g_kidx].xseq = self->header.sequence;

    cma__assert_fail (
	    cma__tac_isset (&cma__g_kernel_critical),
	    "cma_exit_kernel:  kernel critical already unlocked");

    cma___g_karray[cma___g_kidx].unlock = cma_c_true;
    cma___g_karray[cma___g_kidx].unset = cma_c_true;	/* released by unset */
    cma___g_karray[cma___g_kidx].locked = cma_c_false;

    if (cma___g_kidx >= _CMA_TRACE_KERNEL_-1)
	cma___g_kidx = 0;
    else
	cma___g_kidx++;

    cma__kernel_unset (&cma__g_kernel_critical);
    }
#endif
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_KERNEL.C */
/*  *14   24-MAR-1992 14:47:18 BUTENHOF "Use redirectable I/O for kernel trace" */
/*  *13   19-FEB-1992 13:50:17 SCALES "Undefer on enter-kernel" */
/*  *12   14-OCT-1991 13:39:06 BUTENHOF "Change trace dumps to go to stdout" */
/*  *11   17-SEP-1991 13:21:15 BUTENHOF "Fix format debug array functions" */
/*  *10   21-AUG-1991 16:42:37 CURTIN "Removed VMS include of stdio.h" */
/*  *9    25-JUL-1991 13:53:39 BUTENHOF "Use cma__int_*printf functions" */
/*  *8    11-JUN-1991 17:17:02 BUTENHOF "Add & use functions to dump kernel/sem trace arrays" */
/*  *7    10-JUN-1991 18:22:11 SCALES "Add sccs headers for Ultrix" */
/*  *6    29-MAY-1991 17:14:44 BUTENHOF "Add multiple entry checks to enter_kernel" */
/*  *5    14-MAY-1991 13:43:26 BUTENHOF "Add global data for kernel trace" */
/*  *4    10-MAY-1991 16:43:35 BUTENHOF "Add global data for kernel trace" */
/*  *3    10-MAY-1991 16:18:46 BUTENHOF "Add global data for kernel trace" */
/*  *2    12-APR-1991 23:35:59 BUTENHOF "Change type of internal locks" */
/*  *1    12-DEC-1990 21:46:50 BUTENHOF "Kernel lock support" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_KERNEL.C */
