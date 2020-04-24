/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems
 * Burlington, MA, USA
 * All Rights Reserved
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: cma_int_errno.h $Revision: 1.1.8.1 $ $Date: 1996/05/10 13:38:05 $
 */
/*
 * HISTORY
 * $Log: cma_int_errno.h,v $
 * Revision 1.1.8.1  1996/05/10  13:38:05  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:42 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:43 UTC  jss  /main/HPDCE02/1]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  16:06  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/21  12:59:32  truitt]
 * 	 *
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:43:38  root  1.1.9.3]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:12:37  hopkins]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  18:22:07  root]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:33:41  hopkins]
 *
 * 	Gamera merge
 * 	[1993/03/26  23:55:03  hopkins]
 *
 * 	Loading drop DCE1_0_2B18
 * 	[1993/03/11  22:27:18  osfrcs  1.1.1.8]
 *
 * 	DCE 1.0.1 Release
 * 	[1992/09/23  20:38:08  hopkins]
 *
 * Revision 1.1.4.2  1994/06/09  13:38:10  devsrc
 * 	CR10892 - fix copyright
 * 	[1994/06/09  13:30:43  devsrc]
 * 
 * Revision 1.1.4.1  1994/04/01  20:16:32  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:17:48  jd]
 * 
 * Revision 1.1.2.4  1992/12/15  22:28:33  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:39:12  alan]
 * 
 * Revision 1.1.2.3  1992/09/29  20:15:04  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:49:14  sekhar]
 * 
 * Revision 1.1.2.2  1992/09/03  14:38:05  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:04:29  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/08  23:29:25  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:29:07  bolinger]
 * 
 * Revision 1.1  1992/01/19  14:40:41  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *	%W%	(DEC OSF/1)	%G%
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
 *	DECthreads services
 *
 *  ABSTRACT:
 *
 *	Header file for internal DECthreads errno functions
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	12 April 1991
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	30 October 1991
 *		OSF DCE wants to use DECthreads on OSF/1 without Mach threads
 *		and without libc_r.a. The _errno() function (and others)
 *		shouldn't be declared under these circumstances, but are
 *		currently compiled whenever built on an OSF/1-based system.
 *	002	Dave Butenhof	27 November 1991
 *		Under DEC C, use the library's get/set errno access functions
 *		rather than global errno.
 *	003	Webb Scales	25 February 1992
 *		Reorganized errno handling.
 *	004	Webb Scales	20 April 1992
 *		Temporarily returned errno dependency to VAXCRTL for VAX VMS.
 */

#ifndef CMA_INT_ERRNO
#define CMA_INT_ERRNO

/*
 *  INCLUDE FILES
 */
#include <errno.h>
#include <cma_defs.h>
#include <cma_tcb_defs.h>
#include <cma_stack.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef EDEADLK
# define EDEADLK	EWOULDBLOCK
#endif

/*
 * CONSTANTS AND MACROS
 */

/*
 * This symbol determines what sort of errno-processing DECthreads does:
 *	User-program references to "errno" result in:
 *	    STATIC_CELL -   a direct reference to a static cell
 *	    EXTERN_FUNC -   a call to a function which references a static cell
 *	    INTERN_FUNC -   a call to cma_errno() which references the TCB
 */
#define _CMA__STATIC_CELL	1
#define _CMA__EXTERN_FUNC	2
#define _CMA__INTERN_FUNC	3

/*
 * Errno functions
 *
 * FIX-ME:  _CMA_ERRNO_TYPE_ should be _CMA__EXTERN_FUNC on VAX VMS starting
 *          with V6.0 (i.e., remove the hardware test from the #if below).
 */

#if _CMA_VENDOR_ == _CMA__HP
# define _CMA_ERRNO_TYPE_	_CMA__STATIC_CELL
#endif

#ifndef _CMA_ERRNO_TYPE_
# if (_CMA_OS_ == _CMA__VMS) && (_CMA_HARDWARE_ == _CMA__ALPHA)
#  define _CMA_ERRNO_TYPE_	_CMA__EXTERN_FUNC
# else
#  if (_CMA_REENTRANT_CLIB_) && (!(_CMA_UNIX_TYPE ==  _CMA__SVR4))
#   define _CMA_ERRNO_TYPE_	_CMA__INTERN_FUNC
#  else
#   define _CMA_ERRNO_TYPE_	_CMA__STATIC_CELL

extern int *cma_errno (void);

#  endif
# endif
#endif


#if _CMA_ERRNO_TYPE_ == _CMA__INTERN_FUNC
# define cma__get_errno()		(cma__get_self_tcb ()->thd_errno)
# define cma__set_errno(value)	(cma__get_self_tcb ()->thd_errno = value)
# define cma__get_thderrno(thd)		(thd->thd_errno)
# define cma__set_thderrno(thd,value)	(thd->thd_errno = value)
#else
# if (_CMA_ERRNO_TYPE_ == _CMA__STATIC_CELL)
#  undef errno
# endif
# define cma__get_errno()		(errno)
# define cma__set_errno(value)		(errno = value)
# define cma__get_thderrno(thd)		(errno)
# define cma__set_thderrno(thd,value)	(errno = value)
#endif


/*
 * TYPEDEFS
 */

/*
 *  GLOBAL DATA
 */

/*
 * INTERNAL INTERFACES
 */

#if (_CMA_OSIMPL_ == _CMA__OS_OSF)
extern int geterrno (void);

extern void seterrno (int value);

extern int *_errno (void);
#endif

#ifdef __cplusplus
}
#endif

#endif
