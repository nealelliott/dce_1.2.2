/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: cma_sigwait.h $Revision: 1.1.8.1 $ $Date: 1996/05/10 13:41:36 $
 */
/*
 * HISTORY
 * $Log: cma_sigwait.h,v $
 * Revision 1.1.8.1  1996/05/10  13:41:36  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:45 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:44 UTC  jss  /main/HPDCE02/1]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  18:07  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/21  14:40:59  truitt]
 * 	 *
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:44:13  root  1.1.9.3]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:13:51  hopkins]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  18:46:43  root]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:34:55  hopkins]
 *
 * 	Gamera merge
 * 	[1993/03/26  23:55:44  hopkins]
 *
 * 	Loading drop DCE1_0_2B18
 * 	[1993/03/11  22:28:22  osfrcs  1.1.1.8]
 *
 * 	DCE 1.0.1 Release
 * 	[1992/09/23  20:38:36  hopkins]
 *
 * Revision 1.1.4.2  1994/06/09  13:38:45  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:31  devsrc]
 * 
 * Revision 1.1.4.1  1994/04/01  20:17:14  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:18:44  jd]
 * 
 * Revision 1.1.2.3  1992/12/15  22:30:48  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:41:32  alan]
 * 
 * Revision 1.1.2.2  1992/10/12  20:18:19  sommerfeld
 * 	Undo special case of HPPA here until we get our act straightened out..
 * 	[1992/10/01  19:58:45  sommerfeld]
 * 
 * Revision 1.1  1992/01/19  14:41:26  devrcs
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
 *	Header file for the CMA implementation of POSIX sigwait routine
 *
 *  AUTHORS:
 *
 *	Paul Curtin
 *
 *  CREATION DATE:
 *
 *	10 December 1990
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	5 February 1991
 *		To avoid breaking client code (now that this is pulled in
 *		transparently by cma.h, pthread.h, pthread_exc.h), drop all
 *		the header file includes; which means also dropping the
 *		prototypes (oh well).
 *	002	Paul Curtin	9 May 1991
 *		Removed empty proto for cma_sigwait, should acquire from
 *		system header (ie, we redefine system sigwait to cma_sigwait)
 *	003	Paul Curtin	14 May 1991 
 *		Added full proto for cma_sigwait, not part of some systems...
 *	004	Dave Butenhof	19 September 1991
 *		Integrate HPUX CMA5 reverse drop; don't define cma_sigwait()
 *		wrapper if system supports _POSIX_REENTRANT_FUNCTIONS.
 *	005	Dave Butenhof	19 November 1991
 *		Remove direct check for _POSIX_REENTRANT_FUNCTIONS; rely on
 *		_CMA_REENTRANT_CLIB_, since we control that. OSF/1 defines
 *		_POSIX_REENTRANT_FUNCTIONS, but OSF DCE reference port isn't
 *		using libc_r.a. Preserve the intent (I hope) of HP's changes
 *		via checks for HPPA hardware.
 */


#ifndef CMA_SIGWAIT
#define CMA_SIGWAIT

/*
 *  INCLUDE FILES
 */
#include <signal.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * CONSTANTS AND MACROS
 */

/*
 * Note:  sigwait is currently only defined in the POSIX spec and nowhere 
 *	else (ie, it is not in Section 2 of the Unix Manual, it is not part of
 *	the C RTL, and it is not part of the CMA specification) so it is an 
 *	orphan.  It is documented here until it is given its rightful place.
 */
# if !defined(_CMA_NOWRAPPERS_)
#  define sigwait cma_sigwait
# endif

/*
 * TYPEDEFS
 */

# if _CMA_OS_ == _CMA__VMS
#  ifndef _SIGSET_T_
#  define _SIGSET_T_
typedef	int	sigset_t;		/* For sigwait */
#  endif
# endif

/*
 *  GLOBAL DATA
 */

/*
 *  PROTOTYPES
 */
# if _CMA_OS_ != _CMA__VMS
extern cma_t_integer cma_sigwait  (sigset_t *);
# endif

#ifdef __cplusplus
}
#endif

#endif
