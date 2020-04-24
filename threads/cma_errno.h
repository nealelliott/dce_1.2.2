/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: cma_errno.h $Revision: 1.1.11.1 $ $Date: 1996/05/10 13:36:36 $
 */
/*
 * HISTORY
 * $Log: cma_errno.h,v $
 * Revision 1.1.11.1  1996/05/10  13:36:36  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:41 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:42 UTC  jss  /main/HPDCE02/1]
 *
 * 	CR10892 - fix copyright in file
 * 	[1994/06/09  13:27:55  devsrc]
 * 	 *
 *
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:17:27  jd]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  15:49  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/21  12:34:16  truitt]
 * 	 *
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:43:28  root  1.1.8.3]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:12:14  hopkins]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  18:12:05  root]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:33:18  hopkins]
 *
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  17:25:59  hinman]
 *
 * 	[hinman] - Check in merged SNI version
 * 	[1993/01/11  16:19:39  hinman]
 *
 * 	Loading drop DCE1_0_2FNL
 *
 * 	[1993/05/04  22:34:03  mgm  1.1.1.11]
 *
 * Revision 1.1.7.2  1994/06/09  13:37:58  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:27:55  devsrc]
 * 
 * Revision 1.1.7.1  1994/04/01  20:16:18  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:17:27  jd]
 * 
 * Revision 1.1.4.4  1993/02/01  22:17:53  hinman
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  17:25:59  hinman]
 * 
 * 	[hinman] - Check in merged SNI version
 * 	[1993/01/11  16:19:39  hinman]
 * 
 * Revision 9.5.1.6  92/09/22  16:06:43  raj
 * 	[raj] On SVR4 redefine errno to cma_errno. This insulates applications from
 * 	internal implementation of errno.
 * 
 * Revision 9.5.3.2  92/09/21  17:50:07  raj
 * 	[raj] On SVR4 redefine errno to cma_errno so that applications are insulated from
 * 	      internal implentation.
 * 
 * Revision 9.5.1.5  92/09/10  16:32:24  hinman
 * 	[hinman] - Fold forward from raj_om
 * 
 * Revision 9.5.2.2  92/09/10  16:01:41  raj
 * 	[raj] merging SVR4 changes into 1.0.2
 * 
 * Revision 9.9  92/09/02  14:02:52  hinman
 * 	Update checkin
 * 
 * Revision 1.1.2.3  1992/08/14  20:21:37  bolinger
 * 	Fix reference to _CMA_REENTRANT_CLIB_ to reflect fact that it's
 * 	always defined (to either 1 or 0).
 * 	[1992/08/14  20:02:59  bolinger]
 * 
 * 	Bring forward 1.0.1 revision 1.1.2.2 (fix for OT defect 1787).
 * 	[1992/08/10  15:21:52  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/08  23:18:11  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:17:57  bolinger]
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
 *	Header file for CMA's per-thread errno.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	11 October 1990
 *
 *  MODIFIED BY:
 *
 *	Dave Butenhof
 *	Paul Curtin
 */

#ifndef CMA_ERRNO
# define CMA_ERRNO

/*
 *  INCLUDE FILES
 */

# include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif

# ifndef EDEADLK
#  define EDEADLK	EWOULDBLOCK
# endif

/*
 * CONSTANTS AND MACROS
 */

/*
 * Errno function
 */
# if !defined(_CMA_REENTRANT_CLIB_) || (_CMA_UNIX_TYPE ==  _CMA__SVR4)
#  ifndef _CMA_NOWRAPPERS_
#   undef errno
#   define errno	*cma_errno()
#  endif

/*
 * TYPEDEFS
 */

/*
 *  GLOBAL DATA
 */

/*
 * INTERNAL INTERFACES
 */

extern int *
cma_errno (void);

# endif /* !_CMA_REENTRANT_CLIB_ */

#ifdef __cplusplus
}
#endif

#endif
