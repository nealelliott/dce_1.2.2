/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cma_errno.c,v $
 * Revision 1.1.6.2  1996/02/17  23:33:56  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:50:07  marty]
 *
 * Revision 1.1.6.1  1995/12/07  22:02:27  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:05:39  root]
 * 
 * Revision 1.1.4.2  1994/06/09  13:37:57  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:27:54  devsrc]
 * 
 * Revision 1.1.4.1  1994/04/01  20:16:16  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:17:24  jd]
 * 
 * Revision 1.1.2.3  1992/12/15  22:28:03  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:38:26  alan]
 * 
 * Revision 1.1.2.2  1992/09/03  14:37:08  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:03:10  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/08  23:17:33  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:17:20  bolinger]
 * 
 * Revision 1.1  1992/01/19  14:40:59  devrcs
 * 	Initial revision
 * 
 * $EndLog$
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
 *	Services to manage per-thread UNIX(R) errno
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	12 February 1990
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	12 April 1991
 *		Adapt for OSF/1, which has latent support for per-thread
 *		errno in the standard errno.h, but requires help from the
 *		thread library (us) to implement it.
 *	002	Paul Curtin	18 November 1991
 *		Made cma_errno return value volatile on Alpha platform.
 *	003	Dave Butenhof	27 November 1991
 *		Add code for Alpha DEC C.
 *	004	Webb Scales	25 February 1992
 *		Reorganized errno handling, added a vms-specific routine
 *		for fetching the vms-specific errno.
 */

/*
 *  INCLUDE FILES
 */

#include <cma.h>
#include <cma_int_errno.h>

/*
 * GLOBAL DATA
 */

/*
 * LOCAL DATA
 */

/*
 * LOCAL MACROS
 */

/*
 * LOCAL FUNCTIONS
 */

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Return address of per-thread errno variable
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
 *	address of errno (int *)
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern cma_t_integer *
cma_errno(void)
    {
#if _CMA_ERRNO_TYPE_ != _CMA__INTERN_FUNC
    /*
     * This should really be a per-thread errno cell in the TCB.  However, on
     * uniprocessor implementations we'd like to coexist with old libraries
     * that don't know about threads, without a lot of complication. While
     * it's possible to make a per-thread errno coexist with the global, it
     * can't be done without a set of constructs like "geterrno" and
     * "seterrno".
     *
     * Therefore, errno will be treated like a machine register; it will be
     * context switched.  This is WRONG, as it cannot work on a true
     * multiprocessor; however, it's good enough for a uniprocessor
     * implementation.
     */
    return (cma_t_integer *)&errno;
#else
    return &(cma__get_self_tcb ()->thd_errno);
#endif
    }


#if _CMA_OS_ == _CMA__VMS
/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Return address of per-thread, VMS-specific errno variable
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
 *	address of vms-errno (int *)
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern cma_t_integer *
cma_vms_errno(void)
    {
# if _CMA_ERRNO_TYPE_ != _CMA__INTERN_FUNC
    /*
     * This should really be a per-thread errno cell in the TCB.  However, on
     * uniprocessor implementations we'd like to coexist with old libraries
     * that don't know about threads, without a lot of complication. While
     * it's possible to make a per-thread errno coexist with the global, it
     * can't be done without a set of constructs like "geterrno" and
     * "seterrno".
     *
     * Therefore, errno will be treated like a machine register; it will be
     * context switched.  This is WRONG, as it cannot work on a true
     * multiprocessor; however, it's good enough for a uniprocessor
     * implementation.
     */
    return (cma_t_integer *)&vaxc$errno;
# else
    return &(cma__get_self_tcb ()->thd_vmserrno);
# endif
    }
#endif

#if _CMA_OSIMPL_ == _CMA__OS_OSF
/*
 * Declare the functions used by the runtime libraries for errno. Note that
 * the seterrno function also sets the global errno; this is useless for
 * multithreaded applications, but allows single threaded applications that
 * somehow pull in DECthreads (but don't actually create threads) to continue
 * to operate "the old way".
 */
int
geterrno (void)
    {
    return (cma__get_errno ());
    }

void
seterrno (int value)
    {
    cma__set_errno (value);

	/*
	 * Open a new block so that we can position the "undef" below after
	 * the "set-errno" above, to keep it from interfering.
	 */
	{
# undef errno
	extern int	errno;

	errno = value;
	}
    }

int *
_errno (void)
    {
    return &(cma__get_self_tcb ()->thd_errno);
    }
#endif					/* _CMA__OS_OSF */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_ERRNO.C */
/*  *7    26-FEB-1992 19:15:08 SCALES "Integrate with errno image" */
/*  *6    27-NOV-1991 11:03:30 BUTENHOF "Use geterrno" */
/*  *5    18-NOV-1991 11:06:36 CURTIN "Made cma_errno return value volatile on Alpha platform" */
/*  *4    14-OCT-1991 13:38:44 BUTENHOF "Modify for uniprocessor OSF/1" */
/*  *3    10-JUN-1991 18:21:39 SCALES "Add sccs headers for Ultrix" */
/*  *2    12-APR-1991 23:35:43 BUTENHOF "Change errno access for OSF/1" */
/*  *1    12-DEC-1990 21:45:11 BUTENHOF "Per-thread errno" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_ERRNO.C */
