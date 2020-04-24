/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 *
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems
 * Burlington, MA, USA
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: cma_px.h $Revision: 1.1.11.2 $ $Date: 1996/02/18 23:09:50 $
 */
/*
 * HISTORY
 * $Log: cma_px.h,v $
 * Revision 1.1.11.2  1996/02/18  23:09:50  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:34:37  marty]
 *
 * Revision 1.1.11.1  1995/12/07  22:04:49  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1994/07/07  16:07 UTC  jss
 * 	Fixed merge problem from hpdce02 merge.
 * 
 * 	HP revision /main/HPDCE02/1  1994/07/06  16:43 UTC  jss
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 
 * 	HP revision /main/HPDCE01/1  1994/01/25  18:28  truitt
 * 	merge kk and hpdce01
 * 	[1995/12/07  21:06:38  root]
 * 
 * Revision 1.1.8.6  1993/12/07  18:53:39  hopkins
 * 	Fix timespec problems, again ...
 * 	[1993/11/03  19:43:50  hopkins]
 * 
 * Revision 1.1.8.5  1993/09/21  13:42:50  truitt
 * 	merge kk and dce1_0_3
 * 	[1993/09/21  13:42:18  truitt]
 * 
 * Revision 1.1.9.3  1993/09/15  17:43:57  root
 * 	    Loading drop DCE1_0_3b03
 * 
 * Revision 1.1.8.4  1993/08/04  15:50:49  hopkins
 * 	C++ support:
 * 	    add 'extern "C" { ... }'
 * 	[1993/08/03  18:13:11  hopkins]
 * 
 * Revision 1.1.7.2  1993/06/24  21:02:09  hinman
 * 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
 * 	[1993/06/17  14:44:21  hinman]
 * 
 * Revision 1.1.8.3  1993/07/27  23:41:12  hopkins
 * 	Made all preproccessor directives start with #
 * 	in position 0, because non-ansi cpp chokes
 * 	when # is not first character on line.
 * 	[1993/07/27  23:38:46  hopkins]
 * 
 * Revision 1.1.8.2  1993/07/08  18:33:56  root
 * 	Initial King Kong branch
 * 	[1993/07/08  18:33:19  root]
 * 
 * Revision 1.1.6.11  1993/06/03  17:37:08  ahop
 * 	add rev_ctl
 * 	[1993/06/03  17:36:37  ahop]
 * 
 * Revision 1.1.6.10  1993/06/03  17:15:04  tatsu_s
 * 	Do not include <sys/timers.h> unless __hp9000s700 is defined.
 * 	The series 800 is not AES compliant.
 * 	[1993/06/03  17:10:47  tatsu_s]
 * 
 * Revision 1.1.6.9  1993/05/18  20:55:03  tatsu_s
 * 	Allow the inclusion of both cma_px.h and utc.h .
 * 	[1993/05/18  16:40:23  tatsu_s]
 * 
 * 	Need to define "struct timespec", #if defined(__hpux) &&
 * 	!defined(_INCLUDE_AES_SOURCE).
 * 	[1993/05/17  14:46:55  tatsu_s]
 * 
 * 	Do the timespec thing right.
 * 	[1993/05/14  20:25:08  tatsu_s]
 * 
 * Revision 1.1.6.8  1993/05/13  15:52:32  eheller
 * 	Added missing && in third level if in Constants & Macros section
 * 	[1993/05/13  15:51:52  eheller]
 * 
 * Revision 1.1.6.7  1993/05/05  15:42:06  hopkins
 * 	Back out timespec changes -- we can't do
 * 	the right thing because too many other
 * 	places in DCE do the wrong thing.
 * 	[1993/05/05  15:40:38  hopkins]
 * 
 * Revision 1.1.6.6  1993/05/04  20:21:42  hopkins
 * 	Do the timespec thing right.
 * 	[1993/05/04  20:21:23  hopkins]
 * 
 * Revision 1.1.6.5  1993/05/04  20:14:48  hopkins
 * 	Fix missing #endif bug from adding
 * 	_TIMESPEC_T_ define.
 * 	[1993/05/04  20:14:22  hopkins]
 * 
 * Revision 1.1.6.4  1993/05/04  19:06:11  hopkins
 * 	define _TIMESPEC_T_ after include of
 * 	timers.h if __hpux.
 * 	[1993/05/04  19:05:45  hopkins]
 * 
 * Revision 1.1.6.3  1993/05/04  16:17:24  hopkins
 * 	Include <sys/timers.h> if __hpux defined.
 * 	[1993/05/04  16:16:58  hopkins]
 * 
 * Revision 1.1.6.2  1993/03/29  19:56:46  hopkins
 * 	Gamera merge
 * 	[1993/03/26  23:55:14  hopkins]
 * 
 * Revision 1.1.1.8  1993/03/11  22:27:50  osfrcs
 * 	    Loading drop DCE1_0_2B18
 * 
 * Revision 1.1.3.3  1992/09/23  21:13:08  hopkins
 * 	DCE 1.0.1 Release
 * 	[1992/09/23  20:38:20  hopkins]
 * 
 * Revision 1.1.9.1  1994/06/09  13:38:29  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:21  devsrc]
 * 
 * Revision 1.1.7.2  1993/06/24  21:02:09  hinman
 * 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
 * 	[1993/06/17  14:44:21  hinman]
 * 
 * Revision 1.1.2.6  1993/02/05  20:32:48  jd
 * 	The proper way to include sys/timers.h on OSF/1 is to
 * 	check to check _AES_SOURCE which comes from standards.h.
 * 	Made it so.
 * 	[1993/02/05  20:32:20  jd]
 * 
 * Revision 1.1.2.5  1993/02/01  22:18:25  hinman
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  17:26:40  hinman]
 * 
 * Revision 1.1.2.4  1993/01/19  16:10:08  jd
 * 	Back out the change untill we know all of the dependencies. It's
 * 	causing no end of problems with the nightly builds.
 * 	[1993/01/19  14:40:51  jd]
 * 	Revision 1.1.3.2  1993/01/11  16:20:30  hinman
 * 	[hinman] - Check in merged SNI version
 * 
 * Revision 1.1.2.2  1992/12/15  22:29:35  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:40:30  alan]
 * 
 * Revision 1.1  1992/01/19  14:41:07  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  Copyright (c) 1990. 1991 by
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
 *	Header file for POSIX wrapper routines
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	18 May 1990
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Webb Scales	25 Septemeber 1990
 *		Put in ifndef/define around POSIX type definitions to prevent
 *		collisions with other pre-POSIX facilities.
 *	002	Paul Curtin	 11 December 1990
 *		Added sigaction work, and split some of cma_px.h off
 *		into cma_sigwait.h
 *	003	Paul Curtin	31 January 1991
 *		Added _CMA_NOWRAPPERS_ conditional statements
 *	004	Dave Butenhof	5 February 1991
 *		To avoid breaking client code (now that this is pulled in
 *		transparently by cma.h, pthread.h, pthread_exc.h), drop all
 *		the header file includes; which means also dropping the
 *		prototypes (oh well).
 *	005	Dave Butenhof	10 May 1991
 *		Remove cma_signal() declaration... the macro is sufficient,
 *		since it will "reroute" the declaration in
 *		/usr/include/signal.h when the client code includes it.
 *	006	Dave Butenhof	23 May 1991
 *		Add conditionals for _CMA_UNIPROCESSOR_ so kernel-thread
 *		versions of DECthreads can still use some wrappers (e.g.,
 *		cma_sigaction()).
 *	007	Webb Scales	10 June 1991
 *		Conditionalize out sigaction macro for kernel threads.
 *	008	Webb Scales	 8 July 1991
 *		Fix typo in os-impl symbol name.
 *	009	Dave Butenhof	19 September 1991
 *		Integrate HPUX CMA5 reverse drop: add check for
 *		_POSIX_REENTRANT_FUNCTIONS to disable wrappers.
 *	010	Dave Butenhof	04 October 1991
 *		Clean up use of _CMA_UNIPROCESSOR_
 *	011	Dave Butenhof	19 November 1991
 *		Remove direct check for _POSIX_REENTRANT_FUNCTIONS; rely on
 *		_CMA_REENTRANT_CLIB_, since we control that. OSF/1 defines
 *		_POSIX_REENTRANT_FUNCTIONS, but OSF DCE reference port isn't
 *		using libc_r.a.
 */


#ifndef CMA_PX
#define CMA_PX

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  INCLUDE FILES
 */

/*
 * CONSTANTS AND MACROS
 */

#if (!_CMA_REENTRANT_CLIB_) || (_CMA_UNIX_TYPE ==  _CMA__SVR4) || (_CMA_VENDOR_ == _CMA__HP)
# if (_CMA_UNIX_TYPE ==  _CMA__SVR4) && (!defined(_CMA_NOWRAPPERS_))
#  if !defined(_REENTRANT_LIBC_) && \
		!defined(_REENTRANT_RTLD_) && \
        !defined(_REENTRANT_MALLOC_) && \
		!defined(_REENTRANT_EXIT_) && \
        !defined(_REENTRANT_STDIO_) && \
		!defined(_REENTRANT_LIBGEN_) && \
        !defined(_REENTRANT_LIBM_) && \
		!defined(_REENTRANT_NSL_) && \
        !defined(_REENTRANT_LIBNSL_) && \
		!defined(_REENTRANT_LIBSOCKET_) && \
        !defined(_REENTRANT_LIBRESOLV_) && \
		!defined(_REENTRANT_TCPIP_)
#   define sigaction cma_sigaction
#  endif
# else
#  ifndef _CMA_NOWRAPPERS_
#   if !_CMA_THREAD_IS_VP_
#    define sigaction cma_sigaction   
#   endif
#  endif
# endif  /*_CMA__SVR4 */
#endif


 
/*
 * TYPEDEFS
 */


#ifdef __hpux

#include <dce/hpdce_platform.h>

#if !defined(_STRUCT_TIMESPEC)
# if __hpux1000p || defined(hp9000s700) || defined(__hp9000s700)
#  include <sys/timers.h>
#  if !defined(_STRUCT_TIMESPEC)
#   define _STRUCT_TIMESPEC
#  endif
# else
#  define _STRUCT_TIMESPEC
   struct timespec {
       unsigned long	tv_sec;		/* seconds */
       long		tv_nsec;	/* and nanoseconds */
   };
# endif
#endif

#if !defined(_TIMESPEC_T_)
# define _TIMESPEC_T_
  typedef struct timespec timespec_t;
#endif

#else	/* !__hpux */

#if _CMA_OSIMPL_ == _CMA__OS_OSF
#include <standards.h>
#endif

#ifdef _AES_SOURCE
# include <sys/timers.h>
#else
# ifndef _TIMESPEC_T_
# define _TIMESPEC_T_
typedef struct timespec {
    unsigned long	tv_sec;		/* seconds */
    long		tv_nsec;	/* and nanoseconds */
    } timespec_t;
# endif
#endif

#endif	/* __hpux */

/*
 * INTERFACES
 */

#ifdef __cplusplus
}
#endif

#endif  /* CMA_PX */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_PX.H */
/*  *15   19-NOV-1991 12:18:07 BUTENHOF "Remove tests for _POSIX_REENTRANT_FUNCTIONS" */
/*  *14   14-OCT-1991 13:39:45 BUTENHOF "Refine/fix use of config symbols" */
/*  *13   24-SEP-1991 16:27:42 BUTENHOF "Merge CMA5 reverse IBM/HP/Apollo drops" */
/*  *12    8-JUL-1991 15:46:25 SCALES "Fix typo in os-impl symbol" */
/*  *11   10-JUN-1991 19:54:59 SCALES "Convert to stream format for ULTRIX build" */
/*  *10   10-JUN-1991 19:21:16 BUTENHOF "Fix the sccs headers" */
/*  *9    10-JUN-1991 18:22:56 SCALES "Add sccs headers for Ultrix" */
/*  *8    10-JUN-1991 17:54:54 SCALES "Conditionalize sigaction macro for kernel threads" */
/*  *7     5-JUN-1991 18:38:04 BUTENHOF "Include sys/timers.h for timespec" */
/*  *6    29-MAY-1991 17:02:17 BUTENHOF "Change wrapper macros for MP" */
/*  *5    10-MAY-1991 17:52:06 BUTENHOF "Remove prototype" */
/*  *4     6-FEB-1991 01:33:10 BUTENHOF "Drop prototypes to avoid pulling in signal.h, socket.h, etc." */
/*  *3    31-JAN-1991 16:37:42 CURTIN "added _CMA_NOWRAPPERS_ conditional statments" */
/*  *2    17-DEC-1990 14:34:36 CURTIN "split with cma_sigwait.h" */
/*  *1    12-DEC-1990 21:48:42 BUTENHOF "P1003.4a support" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_PX.H */
