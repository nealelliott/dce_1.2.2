/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 *
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems
 * Burlington, MA, USA
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: exc_handling.h $Revision: 1.2.28.1 $ $Date: 1996/05/10 13:45:31 $
 */
/*
 * HISTORY
 * $Log: exc_handling.h,v $
 * Revision 1.2.28.1  1996/05/10  13:45:31  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:48 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:46 UTC  jss  /main/HPDCE02/1]
 *
 * 	Ajay's performance enhancements
 * 	[1994/05/03  18:22 UTC  jss  /main/HPDCE01/3]
 *
 * 	Define exc_push_ctx() and exc_pop_ctx() as macros for libcma internally.
 * 	[1994/04/29  19:12 UTC  ajayb  /main/HPDCE01/ajayb_threads_perf/1]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/25  19:40  truitt  /main/HPDCE01/2]
 *
 * 	Turn on compiler optimization.
 * 	[1993/11/17  19:28:52  ajayb]
 * 	 *
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/21  15:35:04  truitt]
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:45:10  root  1.1.13.3]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:15:32  hopkins]
 *
 * 	On HP-UX: Set _EXC_BAR_JMP_ to true
 * 	Add exc_{setjmp|longjmp} macros for HP platforms.
 * 	[1993/06/08  16:27:21  hopkins]
 *
 * 	Add new #define, _exc_jmpbuf_type, for cast in TRY macro
 * 	Must be double on HP-UX, int on other platforms.
 * 	This should probably be moved to a machine-specific header file
 * 	as part of code cleanup.
 * 	[1993/04/23  22:10:11  hopkins]
 * 	[1993/06/04  21:24:09  sommerfeld]
 *
 * 	Submitting 102-dme port to 103i
 * 	[1993/05/24  20:18:21  cjd]
 *
 * 	Initial 486 port.
 * 	[1993/05/11  21:45:00  jd]
 *
 * 	Don't force _EXC_PROTO_ on HP platforms
 * 	when __STDC__ isn't set.
 * 	[1993/07/28  19:59:33  hopkins]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  19:23:25  root]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:36:35  hopkins]
 *
 * 	Gamera merge
 * 	[1993/03/26  23:57:29  hopkins]
 *
 * 	Loading drop DCE1_0_2B18
 * 	[1993/03/11  22:29:48  osfrcs  1.1.1.8]
 *
 * 	Cast exc_ctx.jmp to (double *) in TRY
 * 	macro so we don't get compile-time warnings.
 * 	[1992/12/18  17:18:24  hopkins]
 *
 * 	Fixes for 9.0 production compiler; turn
 * 	optimization off and remove cast in TRY
 * 	macro.
 * 	[1992/11/10  19:19:14  hopkins]
 *
 * Revision 1.2.21.2  1994/06/09  13:39:18  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:29:04  devsrc]
 * 
 * Revision 1.2.21.1  1994/04/01  20:18:04  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:19:50  jd]
 * 
 * Revision 1.2.19.1  1993/10/05  21:38:49  ohara
 * 	SNI SVR4 ref port 8596
 * 	[1993/09/15  22:14:05  ohara]
 * 
 * Revision 1.2.16.3  1993/06/10  19:50:56  sommerfeld
 * 	On HP-UX: Set _EXC_BAR_JMP_ to true
 * 		  Add exc_{setjmp|longjmp} macros for HP platforms.
 * 	[1993/06/08  16:27:21  hopkins]
 * 
 * 	   Add new #define, _exc_jmpbuf_type, for cast in TRY macro
 * 	   Must be double on HP-UX, int on other platforms.
 * 	   This should probably be moved to a machine-specific header file
 * 	   as part of code cleanup.
 * 	   [1993/04/23  22:10:11  hopkins]
 * 	[1993/06/04  21:24:09  sommerfeld]
 * 
 * Revision 1.2.16.2  1993/05/24  20:50:48  cjd
 * 	Submitting 102-dme port to 103i
 * 	[1993/05/24  20:18:21  cjd]
 * 
 * Revision 1.2.14.2  1993/05/11  22:02:43  jd
 * 	Initial 486 port.
 * 	[1993/05/11  21:45:00  jd]
 * 
 * Revision 1.2.5.5  1993/02/01  22:32:41  hinman
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  17:34:22  hinman]
 * 
 * 	[hinman] - Check in merged SNI version
 * 	[1993/01/11  16:24:12  hinman]
 * 
 * Revision 9.5.1.10  93/01/08  11:27:43  devsrc
 * 	[raj] Fix copyright  notice
 * 
 * Revision 9.5.1.9  92/10/26  12:41:58  bein
 * 	PTC - fixed garbage in file.
 * 
 * Revision 9.5.4.2  92/10/26  10:28:21  bein
 * 	PTC - fixed last set of changes for PYRMIPS platform.
 * 
 * Revision 9.5.1.8  92/10/06  12:14:35  blurie
 * 	Use setjmp() instead of sigsetjmp() on SVR4, MX300I.
 * 	[92/10/06  12:13:36  blurie]
 * 
 * Revision 9.5.1.7  92/09/30  12:43:34  root
 * 	Acceptance of OSF rev 1.2.5.3
 * 
 * Revision 9.12.1.2  92/09/30  10:39:19  hinman
 * 	Auto checkin of OSF rev 1.2.5.3
 * 
 * Revision 1.2.5.3  1992/09/29  20:16:38  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  21:01:06  sekhar]
 * 
 * Revision 1.2.5.2  1992/09/03  14:43:11  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:08:49  bolinger]
 * 
 * Revision 1.1.2.3  1992/08/14  20:23:59  bolinger
 * 	Bring forward 1.0.1 revision 1.2.3.4 (fix for OT defect 2886).
 * 	[1992/08/10  16:42:15  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/09  00:19:57  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/09  00:19:40  bolinger]
 * 
 * Revision 1.2.3.4  1992/05/11  21:32:15  bolinger
 * 	Correct log.
 * 	[1992/05/11  21:27:01  bolinger]
 * 
 * 	Fix for OT defect 2886: enable use of _setjmp/_longjmp on
 * 	both reference platforms.  This avoids needless (and sometimes
 * 	pernicious) save/restore of process signal mask, and nets
 * 	a nice performance gain into the bargain.
 * 	[1992/05/11  13:13:27  bolinger]
 * 
 * Revision 1.2.3.3  1992/04/24  17:49:48  bolinger
 * 	Fix for OT defect 2142 -- typo in _EXC_HARDWARE_ use.
 * 	[1992/04/24  17:15:35  bolinger]
 * 
 * Revision 1.2.3.2  1992/04/22  15:49:48  keane
 * 	Remove dependency on CMA configuration symbol
 * 	[1992/04/22  13:21:44  keane]
 * 
 * Revision 1.2  1992/01/19  22:14:48  devrcs
 * 	Dropping DCE1.0 OSF1_misc port archive
 * 
 * $EndLog$
 */
/*
 *	%W%	(DEC OSF/1)	%G%
 */
/*
 *  Copyright (c) 1989, 1992 by
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
 *	DECthreads exception services
 *
 *  FILENAME:
 *
 * 	EXC_HANDLING.H
 *
 *  ABSTRACT:
 *
 *	Header file for exception handling in C
 *
 *  AUTHORS:
 *
 *	Eric Roberts
 *	Bob Conti
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	15 March 1989
 *
 *  MODIFIED BY:
 *
 *	Dave Butenhof
 *	Bob Conti
 *	Paul Curtin
 *	Webb Scales
 */




#ifndef EXC_HANDLING
# define EXC_HANDLING	1

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  INCLUDE FILES
 */

#define _EXC__CC	1
#define _EXC__VAXC	2
#define _EXC__DECC	3
#define _EXC__CFRONT	4
#define _EXC__GCC	5
#define _EXC__DECCPLUS	6
#define _EXC__GCPLUS	7

/*
 * Test for C++ compilers before C compilers because Glockenspiel C++ also
 * defines symbols for the VAX C compiler and this could be the case for
 * other C++/C compiler combinations
 */

#ifndef _EXC_COMPILER_
# if defined(__cplusplus)		/* test for other C++ compilers first */
#  if defined(__DECCXX)
#   define _EXC_COMPILER_	_EXC__DECCPLUS
#  else
#   define _EXC_COMPILER_	_EXC__CFRONT
#  endif
# elif defined(vaxc) || defined(VAXC) || defined(__vaxc) || defined(__VAXC)
#  define _EXC_COMPILER_	_EXC__VAXC
# elif defined(__decc) || defined(__DECC)
#  define _EXC_COMPILER_	_EXC__DECC
# elif defined(__GNUC__)
#  define _EXC_COMPILER_	_EXC__GCC
# else
#  define _EXC_COMPILER_	_EXC__CC
# endif
#endif

/*
 * Name of the hardware platform
 */
#define	_EXC__MIPS	1
#define	_EXC__VAX	2
#define _EXC__M68K	3
#define _EXC__HPPA	4
#define _EXC__IBMR2     5
#define _EXC__ALPHA	6
#define _EXC__MX300I	7
#define _EXC__CPLMIPS	8
#define _EXC__I386      9

#ifndef	_EXC_HARDWARE_
# if defined(vax) || defined (VAX) || defined(__vax) || defined(__VAX)
#  define	_EXC_HARDWARE_	_EXC__VAX
# endif
# if defined(mips) || defined(MIPS) || defined(__mips) || defined(__MIPS)
#  if !defined(SNI_DCOSX)
#  define	_EXC_HARDWARE_	_EXC__MIPS
#  endif    /* !SNI_DCOSX */
# endif
# if defined(m68k) || defined(m68000) || defined(_ISP__M68K) || defined(M68000) || defined(mc68000) 
#  define	_EXC_HARDWARE_	_EXC__M68K
# endif
# if defined(hp9000s300) || defined(__hp9000s300)
#  define	_EXC_HARDWARE_	_EXC__M68K
# endif
# if defined(__hppa)
#  define	_EXC_HARDWARE_	_EXC__HPPA
# endif
# if defined(_IBMR2)
#  define	_EXC_HARDWARE_	_EXC__IBMR2
# endif
# if defined(__ALPHA) || defined(__alpha)
#  define	_EXC_HARDWARE_	_EXC__ALPHA
# endif
# if defined(SNI_MX300I)
#  define _EXC_HARDWARE_ _EXC__MX300I
# endif
# if defined(SNI_DCOSX)
#  define _EXC_HARDWARE_ _EXC__CPLMIPS
# endif
# if !defined(SNI_SVR4)
#  if defined(__i386)
#   define       _EXC_HARDWARE_  _EXC__I386
#  endif
#endif
# ifndef _EXC_HARDWARE_
   #error "_EXC_HARDWARE_ not set"
# endif
#endif

#define _EXC__UNIX	1
#define _EXC__VMS	2
#define _EXC__SVR4  3

#ifndef	_EXC_OS_
# if defined(unix) || defined(__unix) || defined(_AIX) || defined(__OSF__) || defined(__osf__) || defined(SNI_SVR4)
#  define	_EXC_OS_	_EXC__UNIX
#  if defined(SNI_SVR4)
#    define _EXC_UNIX_TYPE _EXC__SVR4
#  endif 
# endif
# if defined(vms) || defined(__vms) || defined(VMS) || defined(__VMS) || defined(__vms__)
#  define	_EXC_OS_	_EXC__VMS
# endif
# ifndef _EXC_OS_
   #error "_EXC_OS_ not set"
# endif
#endif

/*
 * Combined platform (OS + hardware)
 */
#define	_EXC__MIPS_UNIX		1
#define	_EXC__VAX_VMS		2
#define _EXC__VAX_UNIX		3
#define _EXC__M68K_UNIX		4
#define _EXC__HPPA_UNIX		5
#define _EXC__IBMR2_UNIX	6
#define _EXC__ALPHA_UNIX        7
#define _EXC__ALPHA_VMS         8
#define _EXC__SINIX_MX300I 9
#define _EXC__DCOSX_MIPS       10
#define _EXC__I386_UNIX        11

#ifndef	_EXC_PLATFORM_
# if _EXC_OS_ == _EXC__UNIX
#  if _EXC_HARDWARE_ == _EXC__MIPS
#   define _EXC_PLATFORM_	_EXC__MIPS_UNIX
#  endif
#  if _EXC_HARDWARE_ == _EXC__VAX
#   define _EXC_PLATFORM_	_EXC__VAX_UNIX
#  endif
#  if _EXC_HARDWARE_ == _EXC__M68K
#   define _EXC_PLATFORM_	_EXC__M68K_UNIX
#  endif
#  if _EXC_HARDWARE_ == _EXC__HPPA
#   define _EXC_PLATFORM_	_EXC__HPPA_UNIX
#  endif
#  if _EXC_HARDWARE_ == _EXC__IBMR2
#   define _EXC_PLATFORM_	_EXC__IBMR2_UNIX
#  endif
#  if _EXC_HARDWARE_ == _EXC__ALPHA
#   define _EXC_PLATFORM_	_EXC__ALPHA_UNIX
#  endif
#  if (_EXC_HARDWARE_ == _EXC__MX300I) && (_EXC_UNIX_TYPE == _EXC__SVR4)
#   define _EXC_PLATFORM_ _EXC__SINIX_MX300I
#  endif
#  if (_EXC_HARDWARE_ == _EXC__CPLMIPS)
#   define _EXC_PLATFORM_	_EXC__DCOSX_MIPS
#  endif
#  if (_EXC_HARDWARE_ == _EXC__I386)
#   define _EXC_PLATFORM_       _EXC__I386_UNIX
#  endif
# endif
# if _EXC_OS_ == _EXC__VMS
#  if _EXC_HARDWARE_ == _EXC__VAX
#   define _EXC_PLATFORM_	_EXC__VAX_VMS
#  endif
#  if _EXC_HARDWARE_ == _EXC__ALPHA
#   define _EXC_PLATFORM_	_EXC__ALPHA_VMS
#  endif
# endif
# ifndef _EXC_PLATFORM_
   #error "_EXC_PLATFORM_ not set"
# endif
#endif

/*
 * Name of the software vendor (alphabetical order :-) )
 */
#define _EXC__APOLLO	1
#define _EXC__DIGITAL	2
#define _EXC__HP        3
#define _EXC__IBM       4
#define _EXC__OSF	5
#define _EXC__SUN       6
#define _EXC__SNI       7
#define _EXC__PTC       8

#ifndef _EXC_VENDOR_
# ifdef apollo
#  define	_EXC_VENDOR_	_EXC__APOLLO
# endif
# if _EXC_OS_ == _EXC__VMS
#  define	_EXC_VENDOR_	_EXC__DIGITAL
# endif
# if defined(ultrix) || defined(__ULTRIX) || defined (__ultrix)
#  define	_EXC_VENDOR_	_EXC__DIGITAL
# endif
# if defined(__osf__) && !defined(__OSF__) && (_EXC_HARDWARE_ == _EXC__MIPS)
#  define	_EXC_VENDOR_	_EXC__DIGITAL
# endif
# if defined(__osf__) && !defined(__OSF__) && (_EXC_HARDWARE_ == _EXC__ALPHA)
#  define	_EXC_VENDOR_	_EXC__DIGITAL
# endif
# if defined(hpux) || defined(__hpux)
#  define	_EXC_VENDOR_	_EXC__HP
# endif
# ifdef _IBMR2
#  define 	_EXC_VENDOR_	_EXC__IBM
# endif
# ifdef sun
#  define	_EXC_VENDOR_	_EXC__SUN
# endif
# ifdef SNI_SINIX
#  define   _EXC_VENDOR_    _EXC__SNI
# endif
# ifdef SNI_DCOSX
#  define   _EXC_VENDOR_    _EXC__PTC
# endif
# if defined (__OSF__) && !defined (_EXC_VENDOR_)
#  define	_EXC_VENDOR_	_EXC__OSF
# endif
# ifndef _EXC_VENDOR_
   #error "_EXC_VENDOR_ not set"
# endif
#endif

/*
 * This controls whether ANSI C function prototypes are used for EXC
 * interfaces.
 */
#ifndef	_EXC_PROTO_
# ifdef __STDC__
#  define _EXC_PROTO_		1
# endif
# if _EXC_COMPILER_ == _EXC__DECC
#  define _EXC_PROTO_		1
# endif
# if _EXC_COMPILER_ == _EXC__GCC
#  define _EXC_PROTO_		1
# endif
# if _EXC_COMPILER_ == _EXC__CFRONT
#  define _EXC_PROTO_		1
# endif
# if _EXC_COMPILER_ == _EXC__DECCPLUS
#  define _EXC_PROTO_		1
# endif
# if _EXC_COMPILER_ == _EXC__GCPLUS
#  define _EXC_PROTO_		1
# endif
# if _EXC_COMPILER_ == _EXC__VAXC
#  define _EXC_PROTO_		1
# endif
# if (_EXC_VENDOR_ == _EXC__DIGITAL) && (_EXC_HARDWARE_ == _EXC__MIPS)
#  define _EXC_PROTO_		1
# endif
# if _EXC_VENDOR_ == _EXC__APOLLO
#  define _EXC_PROTO_		1
# endif

/*
 * Not sure why this was even here (since our compiler should
 * define __STDC__ appropriately), but it breaks non-ansi
 * compiles, so comment it out.
 *
 * # if (_EXC_VENDOR_ == _EXC__HP) && (_EXC_HARDWARE_ == _EXC__HPPA)
 * #  define _EXC_PROTO_		1
 * # endif
 */

# if _EXC_HARDWARE_ == _EXC__IBMR2
#  define _EXC_PROTO_		1
# endif
# if _EXC_PLATFORM_ == _EXC__SINIX_MX300I
#  define _EXC_PROTO_       1
# endif
# if _EXC_PLATFORM_ == _EXC__DCOSX_MIPS
#  define _EXC_PROTO_       1
# endif
/* Otherwise, _EXC_PROTO_ is undefined, which means do not use prototypes. */
#endif

/*
 * MIPS C on DEC OSF/1 sets __osf__ but not __OSF__; but gcc on "raw" OSF/1
 * sets __OSF__ but not __osf__. This little ditty provides a bridge.
 */
#if defined (__OSF__) && !defined (__osf__)
# define __osf__
#endif

#define _EXC__OS_AIX	1
#define _EXC__OS_OSF	2
#define _EXC__OS_BSD	3
#define _EXC__OS_SYSV	4
#define _EXC__OS_VMS	5

#ifndef _EXC_OSIMPL_
# if _EXC_OS_ == _EXC__VMS
#  define _EXC_OSIMPL_          _EXC__OS_VMS
# else
#  if defined (__osf__)
#   define _EXC_OSIMPL_         _EXC__OS_OSF
#  else
#   if _EXC_VENDOR_ == _EXC__IBM
#    define _EXC_OSIMPL_        _EXC__OS_AIX
#   else
#    if _EXC_VENDOR_ == _EXC__SUN
#     define _EXC_OSIMPL_       _EXC__OS_SYSV
#    else
#     define _EXC_OSIMPL_       _EXC__OS_BSD
#    endif
#   endif
#  endif
# endif
#endif

#ifndef	_EXC_VOLATILE_
# if _EXC_PLATFORM_ == _EXC__VAX_UNIX
#  if _EXC_COMPILER_ == _EXC__CC
#   define _EXC_VOLATILE_
#   define _EXC_VOLATILE_FLAG_		0
#  endif
# endif
# if _EXC_VENDOR_ == _EXC__SUN
#  define _EXC_VOLATILE_
#  define _EXC_VOLATILE_FLAG_		0
# endif
# if _EXC_COMPILER_ == _EXC__CFRONT
#  define _EXC_VOLATILE_
#  define _EXC_VOLATILE_FLAG_		0
# endif
#ifndef	_EXC_VOLATILE_
#  define _EXC_VOLATILE_  		volatile
#  define _EXC_VOLATILE_FLAG_		1
# endif
#endif

#ifndef _EXC_IMPORT_
# if (_EXC_OS_ == _EXC__VMS) && (_EXC_HARDWARE_ == _EXC__VAX)
#  if _EXC_COMPILER_ == _EXC__DECCPLUS
#   pragma __extern_model __save
#   pragma __extern_model __strict_refdef
#   define _EXC_IMPORT_ extern
#  else
#   define _EXC_IMPORT_ globalref
#  endif
# else
#  define _EXC_IMPORT_ extern
# endif
#endif

#ifdef _EXC_PROTO_
#define _EXC_PROTOTYPE_(arg)	arg
#else
#define _EXC_PROTOTYPE_(arg)	()
#endif

/*
 * NOTE: on U*IX systems, these status codes must be kept unique from
 * "Enums".  We do this arbitrarily by setting some high order bits which
 * happen to be the same as we use on VMS. Apollo systems use different
 * error numbering scheme, and override this.
 */
#if _EXC_VENDOR_ == _EXC__APOLLO
# define exc_facility_c	0x03c0000
# define _EXC_STATUS_(val, sev) ((exc_int_t)(exc_facility_c + (val)))
#endif
#if _EXC_OS_ == _EXC__VMS
# define exc_facility_c	00020100000
# define _EXC_STATUS_(val, sev) \
	((exc_int_t)(exc_facility_c | ((val) << 3) | (sev)))
#endif
#ifndef _EXC_STATUS_
# define _EXC_DCE_PREFIX_	0x177db000
# define exc_facility_c		_EXC_DCE_PREFIX_
# define _EXC_STATUS_(val, sev) \
	((exc_int_t)(_EXC_DCE_PREFIX_ | (val)))
#endif

#ifdef __cplusplus
}
#endif

#include <setjmp.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Define a symbol that specifies whether exception handling should use the
 * standard setjmp() and longjmp() functions, or the alternate _setjmp() and
 * _longjmp().  The latter are faster, as they don't save/restore the signal
 * mask (and therefore require no kernel calls).  However, _setjmp() and
 * _longjmp() are not standard, and therefore may not be available
 * everywhere. Also, there may be some platforms where failing to save signal
 * state could break exception handling. For both reasons, we enable use of
 * the optimized functions only where we know for sure they are both
 * available and appropriate.
 */
#ifndef _EXC_BAR_JMP_
/*
 * The current intent is to enable this feature for all Apollo O.S.'s,
 * DEC's OSF/1 and Ultrix, AIX, and OSF's OSF/1.  Added HP-UX as well.
 */
 /*
 *	On SVR4, we use setjmp() if _EXC_BAR_JMP_ is set, otherwise use
 *	sigsetjmp().
 */
/*
 *	Set _EXC_BAR_JMP_ to 1 on HP platforms.
 */
# if (_EXC_VENDOR_ == _EXC__APOLLO) || ((_EXC_VENDOR_ == _EXC__DIGITAL) && (_EXC_OS_ == _EXC__UNIX)) || _EXC_PLATFORM_ == _EXC__IBMR2_UNIX || _EXC_VENDOR_ == _EXC__OSF || (_EXC_PLATFORM_ == _EXC__SINIX_MX300I) || (_EXC_VENDOR_ == _EXC__HP)
#  define _EXC_BAR_JMP_	1
# endif
# ifndef _EXC_BAR_JMP_
#  define _EXC_BAR_JMP_	0
# endif
#endif

#if _EXC_OS_ == _EXC__VMS
# if _EXC_HARDWARE_ == _EXC__VAX
   typedef int cma__t_jmp_buf[14];
   extern int cma$$save_exc_context (_EXC_VOLATILE_ int *);
# else
#  include <ints.h>
   typedef uint64 cma__t_jmp_buf[(14+8+3)];
   extern int cma$$save_exc_context (_EXC_VOLATILE_ uint64 *);
# endif
# define exc_setjmp(__env)	cma$$save_exc_context((__env))
#else
# if _EXC_UNIX_TYPE == _EXC__SVR4
#  if _EXC_BAR_JMP_
    typedef jmp_buf cma__t_jmp_buf;
#  else
    typedef sigjmp_buf cma__t_jmp_buf;
#  endif
# else
  typedef jmp_buf cma__t_jmp_buf;
# endif
# if _EXC_BAR_JMP_
#  if _EXC_OSIMPL_ == _EXC__OS_OSF
    /*
     * OSF/1 already provides prototypes for _setjmp and _longjmp in
     * /usr/include/setjmp.h; the prototypes here should be compatible, so
     * we'll just cast the volatile jump buffer inside the exc_setjmp macro
     * instead of defining an appropriate prototype as we do elsewhere.
     */
    extern int _setjmp _EXC_PROTOTYPE_ ((jmp_buf));
    extern void _longjmp _EXC_PROTOTYPE_ ((jmp_buf, int));
#   define exc_setjmp(__env)		_setjmp ((int *)(__env))
#   define exc_longjmp(__env,__val)	_longjmp((int *)(__env),(__val))
#  elif (_EXC_UNIX_TYPE == _EXC__SVR4)
#   define exc_setjmp(__env)     	setjmp ((__env))
#   define exc_longjmp(__env,__val) longjmp((__env),(__val))
#  elif (_EXC_VENDOR_ == _EXC__HP)
#   define exc_setjmp(__env)		_setjmp ((__env))
#   define exc_longjmp(__env,__val)	_longjmp((__env),(__val))
#  else
    extern int _setjmp _EXC_PROTOTYPE_ ((_EXC_VOLATILE_ int *));
    extern void _longjmp _EXC_PROTOTYPE_ ((_EXC_VOLATILE_ int *, int));
#   define exc_setjmp(__env)		_setjmp ((__env))
#   define exc_longjmp(__env,__val)	_longjmp((__env),(__val))
#  endif
# else
#  if (_EXC_UNIX_TYPE == _EXC__SVR4)
#   define exc_setjmp(__env)     sigsetjmp ((__env),0)
#   define exc_longjmp(__env,__val)  siglongjmp((__env),(__val))
#  else
#   define exc_setjmp(__env)		setjmp ((__env))
#   define exc_longjmp(__env,__val)	longjmp((__env),(__val))
#  endif
# endif
#endif

typedef char *exc_address_t;

/*
 * Use the most efficient code available to determine the address of the
 * current procedure frame on VAX VMS systems (which we need to integrate
 * well with native VAX VMS condition handling).
 *
 * - VAX C under VAX VMS supports instruction "builtins" to access general
 * registers. Since it requires a "#pragma", which some old cpp versions
 * can't parse, it's hidden in a separate file.
 *
 * - GCC supports an "asm" statement that generates inline assembly code.
 *
 * - Otherwise, declare an extern function (part of DECthreads' assembly
 * code) that will return the value.
 */
#if _EXC_OS_ == _EXC__VMS
# if _EXC_HARDWARE_ == _EXC__VAX
#  if _EXC_COMPILER_ == _EXC__VAXC
#   include <exc_handling_vms.h>
#  elif _EXC_COMPILER_ == _EXC__GCC
#   define cma$exc_fetch_fp() \
    ({ int frameptr; \
    asm volatile ("movl fp, %0" : "=g" (frameptr)); \
    frameptr; })
#  else
   extern exc_address_t cma$exc_fetch_fp (void);
#  endif
# else
   extern exc_address_t cma$exc_fetch_fp (void);
# endif
#endif

/*
 * Define all of the status codes used by DECthreads.
 *
 * For VMS, these must remain in synch with the CMA_MESSAGE.GNM message file.
 *
 * These values cannot be altered after they have shipped in some DECthreads
 * release.
 */

/*
 * EXC facility messages
 */
#define exc_s_exception         _EXC_STATUS_(1, 4)
#define exc_s_exccop            _EXC_STATUS_(2, 4)
#define exc_s_uninitexc         _EXC_STATUS_(3, 4)
#define exc_s_unkstatus		_EXC_STATUS_(128, 4)
#define exc_s_exccoplos		_EXC_STATUS_(129, 4)

/*
 * These should be set to match with underlying system exception codes on
 * platforms where that is appropriate (e.g., ss$_ codes on VMS).
 */
#if _EXC_OS_ == _EXC__VMS
/*
 * A few of these codes are somewhat imaginary, since VMS doesn't support
 * condition codes that very closely approximate the sense of some UNIX
 * signals.  SIGTRAP, SIGIOT, and SIGEMT have no clear parallels, and the
 * values chosen are fairly arbitrary.  For two others, we chose what seemed
 * close equivalents: SIGPIPE becomes "no mailbox", and SIGXFSZ becomes "disk
 * quota exceeded".
 */
# define exc_s_illaddr		12	/* ss$_accvio */
# define exc_s_exquota		28	/* ss$_exquota */
# define exc_s_insfmem		292	/* ss$_insfmem */
# define exc_s_nopriv		36	/* ss$_nopriv */
# define exc_s_normal		1	/* ss$_normal */
# define exc_s_illinstr		1084	/* ss$_opcdec */
# define exc_s_resaddr		1100	/* ss$_radrmod */
# define exc_s_privinst		1084	/* ss$_opcdec */
# define exc_s_resoper		1108	/* ss$_roprand */
# define exc_s_SIGTRAP		1044	/* ss$_break */
# define exc_s_SIGIOT		44	/* ss$_abort */
# define exc_s_SIGEMT		1068	/* ss$_compat */
# define exc_s_aritherr		1164	/* ss$_fltovf */
# define exc_s_SIGSYS		20	/* ss$_badparam */
# define exc_s_SIGPIPE		628	/* ss$_nombx */
# define exc_s_excpu		8364	/* ss$_excputim */
# define exc_s_exfilsiz		1004	/* ss$_exdiskquota */
# define exc_s_intovf		1148	/* ss$_intovf */
# define exc_s_intdiv		1156	/* ss$_intdiv */
# define exc_s_fltovf		1164	/* ss$_fltovf */
# define exc_s_fltdiv		1172	/* ss$_fltdiv */
# define exc_s_fltund		1180	/* ss$_fltund */
# define exc_s_decovf		1188	/* ss$_decovf */
# define exc_s_subrng		1196	/* ss$_subrng */
#else
# define exc_s_illaddr		_EXC_STATUS_(5, 4)
# define exc_s_exquota		_EXC_STATUS_(6, 4)
# define exc_s_insfmem		_EXC_STATUS_(7, 4)
# define exc_s_nopriv		_EXC_STATUS_(8, 4)
# define exc_s_normal		_EXC_STATUS_(9, 1)
# define exc_s_illinstr		_EXC_STATUS_(10, 4)
# define exc_s_resaddr		_EXC_STATUS_(11, 4)
# define exc_s_privinst		_EXC_STATUS_(12, 4)
# define exc_s_resoper		_EXC_STATUS_(13, 4)
# define exc_s_SIGTRAP		_EXC_STATUS_(14, 4)
# define exc_s_SIGIOT		_EXC_STATUS_(15, 4)
# define exc_s_SIGEMT		_EXC_STATUS_(16, 4)
# define exc_s_aritherr		_EXC_STATUS_(17, 4)
# define exc_s_SIGSYS		_EXC_STATUS_(18, 4)
# define exc_s_SIGPIPE		_EXC_STATUS_(19, 4)
# define exc_s_excpu		_EXC_STATUS_(20, 4)
# define exc_s_exfilsiz		_EXC_STATUS_(21, 4)
# define exc_s_intovf		_EXC_STATUS_(22, 4)
# define exc_s_intdiv		_EXC_STATUS_(23, 4)
# define exc_s_fltovf		_EXC_STATUS_(24, 4)
# define exc_s_fltdiv		_EXC_STATUS_(25, 4)
# define exc_s_fltund		_EXC_STATUS_(26, 4)
# define exc_s_decovf		_EXC_STATUS_(27, 4)
# define exc_s_subrng		_EXC_STATUS_(28, 4)
# if (_CMA_UNIX_TYPE == _CMA__SVR4)
#  define exc_s_illopc      _EXC_STATUS_(29, 4)
#  define exc_s_illopn      _EXC_STATUS_(30, 4)
#  define exc_s_illadr      _EXC_STATUS_(31, 4)
#  define exc_s_illtrp      _EXC_STATUS_(32, 4)
#  define exc_s_prvopc      _EXC_STATUS_(33, 4)
#  define exc_s_prvreg      _EXC_STATUS_(34, 4)
#  define exc_s_coproc      _EXC_STATUS_(35, 4)
#  define exc_s_badstk      _EXC_STATUS_(36, 4)
#  define exc_s_brkpt       _EXC_STATUS_(37, 4)
#  define exc_s_trace       _EXC_STATUS_(38, 4)
#  define exc_s_fltinv          _EXC_STATUS_(39, 4)
#  define exc_s_maperr          _EXC_STATUS_(40, 4)
#  define exc_s_accerr          _EXC_STATUS_(41, 4)
#  define exc_s_adraln          _EXC_STATUS_(42, 4)
#  define exc_s_adrerr          _EXC_STATUS_(43, 4)
#  define exc_s_objerr      _EXC_STATUS_(44, 4)
#  define exc_s_fltres      _EXC_STATUS_(45, 4)
# endif /* _CMA_UNIX_TYPE == _CMA__SVR4 */
#endif

/*
 * Define alias names
 */
#define exc_s_accvio		exc_s_illaddr
#define exc_s_SIGILL		exc_s_illinstr
#define exc_s_SIGFPE		exc_s_aritherr
#define exc_s_SIGBUS		exc_s_illaddr
#define exc_s_SIGSEGV		exc_s_illaddr
#define exc_s_SIGXCPU		exc_s_excpu
#define exc_s_SIGXFSZ		exc_s_exfilsiz

/*
 * DECthreads facility (CMA) messages
 */
#define cma_s_alerted           _EXC_STATUS_(48, 4)
#define cma_s_assertion         _EXC_STATUS_(49, 4)
#define cma_s_badparam          _EXC_STATUS_(50, 4)
#define cma_s_bugcheck          _EXC_STATUS_(51, 4)
#define cma_s_exit_thread       _EXC_STATUS_(52, 4)
#define cma_s_existence         _EXC_STATUS_(53, 4)
#define cma_s_in_use            _EXC_STATUS_(54, 4)
#define cma_s_use_error         _EXC_STATUS_(55, 4)
#define cma_s_wrongmutex	_EXC_STATUS_(56, 4)
#define cma_s_stackovf          _EXC_STATUS_(57, 4)
#define cma_s_nostackmem        _EXC_STATUS_(58, 4)
#define cma_s_notcmastack       _EXC_STATUS_(59, 4)
#define cma_s_timed_out         _EXC_STATUS_(60, 4)
#define cma_s_unimp             _EXC_STATUS_(61, 4)
#define cma_s_inialrpro         _EXC_STATUS_(62, 4)
#define cma_s_defer_q_full      _EXC_STATUS_(63, 4)
#define cma_s_signal_q_full	_EXC_STATUS_(64, 4)
#define cma_s_alert_nesting	_EXC_STATUS_(65, 4)

/*
 * Synonyms for convenience
 */
#define cma_s_normal		exc_s_normal

/*
 * TYPEDEFS
 */

/*
 * Constants for the kind of an exception object.
 *
 * There are *currently* only two kinds.  In the address-kind, the identity
 * of an exception object is its address; in the value-kind, the
 * identity of an exception object is an integer, typically, 
 * a system-defined-status-value. These coded kinds also
 * serve as sentinels to help detect uninitialized exceptions.
 */
typedef enum EXC_KIND_T {
    exc_kind_address_c	= 0x02130455,  
    exc_kind_status_c	= 0x02130456
    }			exc_kind_t;

/*
 * Internal contents of an exception object.
 */
typedef long int exc_int_t;

typedef struct EXC_EXT_T {
    exc_int_t		sentinel;
    exc_int_t		version;
    exc_address_t	extend;
    unsigned int	*args;
    } exc_ext_t;

typedef struct EXC_KIND_V1ADDR_T {
    exc_kind_t		kind;
    exc_address_t	address;
    exc_int_t		filler[6];
    } exc_kind_v1addr_t;

typedef struct EXC_KIND_V1STATUS_T {
    exc_kind_t		kind;
    exc_int_t		status;
    exc_int_t		filler[6];
    } exc_kind_v1status_t;

typedef struct EXC_KIND_ADDRESS_T {
    exc_kind_t		kind;
    exc_address_t	address;
    exc_ext_t		ext;
    } exc_kind_address_t;

typedef struct EXC_KIND_STATUS_T {
    exc_kind_t		kind;
    exc_int_t		status;
    exc_ext_t		ext;
    } exc_kind_status_t;

typedef union EXC_EXCEPTION_T	{
    exc_kind_t		kind;
    exc_kind_v1status_t	v1status;
    exc_kind_v1addr_t	v1address;
    exc_kind_status_t	status;
    exc_kind_address_t	address;
    } EXCEPTION;

/*
 * Constants for the state of handling in the current TRY clause.
 * 
 * The state is "none" when no exception has been raised, "active" when
 * one has been raised but has not yet been caught by a CATCH clause, and
 * "handled" after the exception has been caught by some CATCH clause.
 */
typedef enum EXC_STATE_T {
    exc_active_c	= 0, /* This must be the 0 state, see pop_ctx */
    exc_none_c		= 1,
    exc_handled_c	= 2,
    exc_popped_c	= 3
    }			exc_state_t;

/*
 * Structure of a context block.
 *
 * A context block is allocated in the current stack frame for each
 * TRY clause.  These context blocks are linked to form a stack of
 * all current TRY blocks in the current thread.  Each context block
 * contains a jump buffer for use by setjmp and longjmp.  
 *
 */
#define exc_excargs_c	40

typedef struct EXC_CONTEXT_T {
    cma__t_jmp_buf	jmp;		/* Jump buffer */
    _EXC_VOLATILE_ struct EXC_CONTEXT_T
			*link;		/* Link to context block stack */
    EXCEPTION		cur_exception;	/* Copy of the current exception */
    exc_state_t		exc_state;	/* State of handling for this TRY */
#if _EXC_OS_ == _EXC__VMS
    exc_address_t	current_frame;	/* Address of current stack frame */
# if _EXC_PLATFORM_ == _EXC__VAX_VMS
    exc_address_t	old_handler;	/* Address of previous handler */
# endif
#endif
    exc_int_t		sentinel;	/* Identify as "new" ctx block */
    exc_int_t		version;	/* Client context version */
    unsigned int	exc_args[exc_excargs_c];
    } exc_context_t;

/*
 *  GLOBAL DATA
 */

#if _EXC_OS_ == _EXC__VMS
/*
 * On VMS, use the VMS calling standard ("$") interface, to avoid pulling in
 * the open interface image (cma$open_rtl) unless the client code uses it.
 */
extern void cma$exc_push_ctx _EXC_PROTOTYPE_ ((	/* Push a context block */
	_EXC_VOLATILE_	exc_context_t *cb));
extern void cma$exc_pop_ctx _EXC_PROTOTYPE_ ((	/* Pop a context block */
	_EXC_VOLATILE_	exc_context_t *cb));
extern void cma$exc_raise _EXC_PROTOTYPE_ ((	/* Raise an exception */
	EXCEPTION *exc));
extern void cma$exc_raise_status _EXC_PROTOTYPE_ ((	/* Raise a status as exception*/
	exc_int_t	status));
extern void cma$exc_report _EXC_PROTOTYPE_ ((	/* Report an exception */
	EXCEPTION *exc));
# define exc_push_ctx		cma$exc_push_ctx
# define exc_pop_ctx		cma$exc_pop_ctx
# define exc_raise		cma$exc_raise
# define exc_raise_status	cma$exc_raise_status
# define exc_report		cma$exc_report
#else
extern void exc_push_ctx _EXC_PROTOTYPE_ ((	/* Push a context block */
	_EXC_VOLATILE_	exc_context_t *cb));
extern void exc_pop_ctx _EXC_PROTOTYPE_ ((	/* Pop a context block */
	_EXC_VOLATILE_	exc_context_t *cb));
extern void exc_raise _EXC_PROTOTYPE_ ((	/* Raise an exception */
	EXCEPTION *exc));
extern void exc_raise_status _EXC_PROTOTYPE_ ((	/* Raise a status as exception*/
	exc_int_t	status));
extern void exc_report _EXC_PROTOTYPE_ ((	/* Report an exception */
	EXCEPTION *exc));
#endif

#if _EXC_OS_ == _EXC__VMS
extern int  cma$exc_handler (/* sargs, margs*/);	/* System condition handler */
#endif

/*
 * Define the exception values that go with the above status codes
 *
 * NOTE: it does not make sense to turn all of the above into 
 * exceptions as some are never raised as exceptions.  Those are:
 *	cma_s_normal	-- never signalled
 *	cma_s_exception	-- internal to the implementation of exceptions
 * 	cma_s_exccop	-- internal to the implementation of exceptions
 *	cma_s_timed_out -- returned as status value from timed condition wait
 */

#if !defined (_EXC_NO_EXCEPTIONS_) && !defined (_CMA_SUPPRESS_EXTERNALS_)
_EXC_IMPORT_ EXCEPTION
    exc_e_uninitexc,
    exc_e_illaddr,
    exc_e_exquota,
    exc_e_insfmem,
    exc_e_nopriv,
    exc_e_illinstr,
    exc_e_resaddr,
    exc_e_privinst,
    exc_e_resoper,
    exc_e_SIGTRAP,
    exc_e_SIGIOT,
    exc_e_SIGEMT,
    exc_e_aritherr,
    exc_e_SIGSYS,
    exc_e_SIGPIPE,
    exc_e_excpu,
    exc_e_exfilsiz,
    exc_e_intovf,
    exc_e_intdiv,
    exc_e_fltovf,
    exc_e_fltdiv,
    exc_e_fltund,
    exc_e_decovf,
    exc_e_subrng,
# if (_CMA_UNIX_TYPE == _CMA__SVR4)
	exc_e_illopc,
	exc_e_illopn,
	exc_e_illadr,
	exc_e_illtrp,
	exc_e_prvopc,
	exc_e_prvreg,
	exc_e_coproc,
	exc_e_badstk,
	exc_e_brkpt,
	exc_e_trace,
	exc_e_objerr,
	exc_e_fltres,
	exc_e_fltinv,
	exc_e_maperr,
	exc_e_accerr,
	exc_e_adraln,
	exc_e_adrerr,
# endif
    cma_e_alerted,
    cma_e_assertion,
    cma_e_badparam,
    cma_e_bugcheck,
    cma_e_exit_thread,
    cma_e_existence,
    cma_e_in_use,
    cma_e_use_error,
    cma_e_wrongmutex,
    cma_e_stackovf,
    cma_e_nostackmem,
    cma_e_notcmastack,
    cma_e_unimp,
    cma_e_inialrpro,
    cma_e_defer_q_full,
    cma_e_signal_q_full,
    cma_e_alert_nesting;

/*
 * Define aliased exceptions
 */
# define exc_e_accvio		exc_e_illaddr
# define exc_e_SIGILL		exc_e_illinstr
# define exc_e_SIGFPE		exc_e_aritherr
# define exc_e_SIGBUS		exc_e_illaddr
# define exc_e_SIGSEGV		exc_e_illaddr
# define exc_e_SIGXCPU		exc_e_excpu
# define exc_e_SIGXFSZ		exc_e_exfilsiz

/*
 * The following are pthread exception names.
 */

# define exc_uninitexc_e	exc_e_uninitexc
# define exc_illaddr_e		exc_e_illaddr
# define exc_exquota_e		exc_e_exquota
# define exc_insfmem_e		exc_e_insfmem
# define exc_nopriv_e		exc_e_nopriv
# define exc_illinstr_e		exc_e_illinstr
# define exc_resaddr_e		exc_e_resaddr
# define exc_privinst_e		exc_e_privinst
# define exc_resoper_e		exc_e_resoper
# define exc_SIGTRAP_e		exc_e_SIGTRAP
# define exc_SIGIOT_e		exc_e_SIGIOT
# define exc_SIGEMT_e		exc_e_SIGEMT
# define exc_aritherr_e		exc_e_aritherr
# define exc_SIGSYS_e		exc_e_SIGSYS
# define exc_SIGPIPE_e		exc_e_SIGPIPE
# define exc_excpu_e		exc_e_excpu
# define exc_exfilsiz_e		exc_e_exfilsiz
# define exc_intovf_e		exc_e_intovf
# define exc_intdiv_e		exc_e_intdiv
# define exc_fltovf_e		exc_e_fltovf
# define exc_fltdiv_e		exc_e_fltdiv
# define exc_fltund_e		exc_e_fltund
# define exc_decovf_e		exc_e_decovf
# define exc_subrng_e		exc_e_subrng
# if (_CMA_UNIX_TYPE == _CMA__SVR4)
#  define	exc_illopc_e	exc_e_illopc
#  define	exc_illopn_e	exc_e_illopn
#  define	exc_illadr_e	exc_e_illadr
#  define	exc_illtrp_e	exc_e_illtrp
#  define	exc_prvopc_e	exc_e_prvopc
#  define	exc_prvreg_e	exc_e_prvreg
#  define	exc_coproc_e	exc_e_coproc
#  define	exc_badstk_e	exc_e_badstk
#  define	exc_brkpt_e		exc_e_brkpt
#  define	exc_trace_e		exc_e_trace
#  define	exc_objerr_e	exc_e_objerr
#  define	exc_fltres_e	exc_e_fltres
#  define   exc_fltinv_e	exc_e_fltinv
#  define   exc_maperr_e    exc_e_maperr
#  define   exc_accerr_e    exc_e_accerr
#  define   exc_adraln_e    exc_e_adraln
#  define   exc_adrerr_e    exc_e_adrerr
# endif

# define pthread_cancel_e	cma_e_alerted
# define pthread_assertion_e	cma_e_assertion
# define pthread_badparam_e	cma_e_badparam
# define pthread_bugcheck_e	cma_e_bugcheck
# define pthread_exit_thread_e	cma_e_exit_thread
# define pthread_existence_e	cma_e_existence
# define pthread_in_use_e	cma_e_in_use
# define pthread_use_error_e	cma_e_use_error
# define pthread_wrongmutex_e	cma_e_wrongmutex
# define pthread_stackovf_e	cma_e_stackovf
# define pthread_nostackmem_e	cma_e_nostackmem
# define pthread_notstack_e	cma_e_notcmastack
# define pthread_unimp_e	cma_e_unimp
# define pthread_inialrpro_e	cma_e_inialrpro
# define pthread_defer_q_full_e	cma_e_defer_q_full
# define pthread_signal_q_full_e cma_e_signal_q_full

# define exc_accvio_e		exc_e_accvio
# define exc_SIGILL_e		exc_e_SIGILL
# define exc_SIGFPE_e		exc_e_SIGFPE
# define exc_SIGBUS_e		exc_e_SIGBUS
# define exc_SIGSEGV_e		exc_e_SIGSEGV
# define exc_SIGXCPU_e		exc_e_SIGXCPU
# define exc_SIGXFSZ_e		exc_e_SIGXFSZ
#endif

/*
 * CONSTANTS AND MACROS
 */

/*
 * This constant helps to identify a context block or exception created with
 * DECthreads BL9 or later; the new structures include a version field to
 * better manage future changes.
 */
#define exc_newexc_c	0x45586732	/* Identify ctx block with version */

/*
 * Define a version constant to be put into exception structures.
 */
#define exc_v2exc_c	2

/*
 * Define "keyword" to initialize an exception. Note: all exceptions *must*
 * be initialized using this macro.
 */
#define EXCEPTION_INIT(e)   (	\
    (e).address.address = (exc_address_t)&(e),	\
    (e).address.kind = exc_kind_address_c, \
    (e).address.ext.sentinel = exc_newexc_c, \
    (e).address.ext.version = exc_v2exc_c, \
    (e).address.ext.extend = (exc_address_t)0, \
    (e).address.ext.args = (unsigned int *)0)

/*
 * Define "routine" to equivalence an exception to an integer
 * (typically a system-defined status value).
 */
#define exc_set_status(e,s) ( \
    (e)->status.status = (s), \
    (e)->status.kind = exc_kind_status_c)

/*
 * Define "routine" to return the status of an exception. Returns 0 if status
 * kind (and value of status in *s), or -1 if not status kind.
 */
#define exc_get_status(e,s) ( \
    (e)->kind == exc_kind_status_c ? \
	(*(s) = (e)->status.status, 0) : \
	-1)

/*
 * Define "routine" to determine if two exceptions match.
 */
#define exc_matches(e1,e2) \
    ((e1)->kind == (e2)->kind \
    && (e1)->address.address == (e2)->address.address)

/*
 * Define "statement" for clients to use to raise an exception.
 */
#define RAISE(e) exc_raise(&(e))

#if _EXC_PLATFORM_ == _EXC__VAX_VMS
/*
 * For VAX VMS, try to integrate peacefully with native VMS condition
 * handling. Save the previous handler for the frame, and restore it on
 * ENDTRY. The DECthreads condition handler will call the saved handler
 * before resignalling a condition that we don't want to handle, unless
 * it is the DECthreads condition handler (to avoid infinite recursion).
 */
# define exc_establish(_exc_ctx_) ( \
        (_exc_ctx_)->current_frame = ((exc_address_t)cma$exc_fetch_fp()), \
	(_exc_ctx_)->old_handler = \
		*((exc_address_t *)(_exc_ctx_)->current_frame), \
        *(exc_address_t *)(_exc_ctx_)->current_frame = \
		((exc_address_t)cma$exc_handler))

# define exc_unestablish(_exc_ctx_) \
	*(exc_address_t *)(_exc_ctx_)->current_frame = (_exc_ctx_)->old_handler;
#else
# if _EXC_PLATFORM_ == _EXC__ALPHA_VMS
    /*
     * Workaround: early Alpha DEC C baselevels require including the header
     * file vaxcshr.h to direct C RTL calls to the proper DEC C entry points.
     * This header erroneously defines "vaxc$establish" to "decc$establish",
     * which doesn't exist and isn't properly translated into defining a
     * condition handler in the procedure descriptor. So defeat the #define
     * manually.
     */
#  if defined (vaxc$establish)
#   undef vaxc$establish
#  endif
#  define exc_establish(_exc_ctx_) \
	(_exc_ctx_)->current_frame = ((exc_address_t)cma$exc_fetch_fp()); \
	lib$establish (cma$exc_handler);
#  define exc_unestablish(_exc_ctx_)
# else
#  define exc_establish(_exc_ctx_)
#  define exc_unestablish(_exc_ctx_)
# endif
#endif

/*
 * Constants to define versions of the context block:
 */
#define exc_v2ctx_c	2

#ifdef __hpux
#define _exc_jmpbuf_type double
#else
#define _exc_jmpbuf_type int
#endif

/* 
 * Start a new TRY block, which may contain exception handlers
 * 
 *   Allocate a context block on the stack to remember the current
 *   exception. Push it on the context block stack.  Initialize
 *   this context block to indicate that no exception is active. Do a SETJMP
 *   to snapshot this environment (or return to it).  Then, start
 *   a block of statements to be guarded by the TRY clause.
 *   This block will be ended by one of the following: a CATCH, CATCH_ALL,
 *   or the ENDTRY macros.
 */
#if _EXC_PLATFORM_ == _EXC__HPPA_UNIX
/*
 * Properly cast parameter to setjmp to prevent compiler warning messages
 */
#ifndef HP_CMA_EXC_MACROS_OPT
        /* Call exc_push_ctx_ instead of exc_push_ctx
         * from TRY within libcma and save on procedure calls. 
         */
#ifdef _HP_CMA_
#define exc_push_ctx_(cb) \
          {                                                             \
             cma__int_init ();                                       \
             (cb)->link = cma__g_current_thread->exc_stack;          \
             (cma__get_self_tcb ())->exc_stack=(exc_context_t *)(cb); \
             (cb)->exc_state = exc_none_c;                           \
          }
#define TRY \
    { \
        _EXC_VOLATILE_ exc_context_t exc_ctx; \
        exc_ctx.sentinel = exc_newexc_c; \
        exc_ctx.version = exc_v2ctx_c; \
        exc_ctx.exc_args[0] = 0; \
        exc_push_ctx_ (&exc_ctx);\
        exc_establish (&exc_ctx);\
        if (!exc_setjmp ((double *)exc_ctx.jmp)) {
/*              { user's block of code goes here }      */
#else /* _HP_CMA_ */
#define TRY \
    { \
	_EXC_VOLATILE_ exc_context_t exc_ctx; \
	exc_ctx.sentinel = exc_newexc_c; \
	exc_ctx.version = exc_v2ctx_c; \
	exc_ctx.exc_args[0] = 0; \
	exc_push_ctx (&exc_ctx);\
	exc_establish (&exc_ctx);\
        if (!exc_setjmp ((double *)exc_ctx.jmp)) {
/*		{ user's block of code goes here } 	*/
#endif /* _HP_CMA_ */
#else  /* HP_CMA_EXC_MACROS_OPT */
#define TRY \
    { \
        _EXC_VOLATILE_ exc_context_t exc_ctx; \
        exc_ctx.sentinel = exc_newexc_c; \
        exc_ctx.version = exc_v2ctx_c; \
        exc_ctx.exc_args[0] = 0; \
        exc_push_ctx (&exc_ctx);\
        exc_establish (&exc_ctx);\
        if (!exc_setjmp ((double *)exc_ctx.jmp)) {
/*              { user's block of code goes here }      */
#endif /* HP_CMA_EXC_MACROS_OPT */
#else
#define TRY \
    { \
	_EXC_VOLATILE_ exc_context_t exc_ctx; \
	exc_ctx.sentinel = exc_newexc_c; \
	exc_ctx.version = exc_v2ctx_c; \
	exc_ctx.exc_args[0] = 0; \
	exc_push_ctx (&exc_ctx);\
	exc_establish (&exc_ctx);\
        if (!exc_setjmp ((int *)exc_ctx.jmp)) {
/*		{ user's block of code goes here } 	*/
#endif

/* 
 * Define an CATCH(e) clause (or exception handler).
 *
 *   First, end the prior block.  Then, check if the current exception
 *   matches what the user is trying to catch with the CATCH clause.
 *   If there is a match, a variable is declared to support lexical
 *   nesting of RERAISE statements, and the state of the current
 *   exception is changed to "handled".
 */
#define CATCH(e) \
            } \
            else if (exc_matches(&exc_ctx.cur_exception, &(e))) { \
		EXCEPTION *THIS_CATCH = (EXCEPTION *)&exc_ctx.cur_exception;\
		exc_ctx.exc_state = exc_handled_c;
/*		{ user's block of code goes here } 	*/

/* 
 * Define an CATCH_ALL clause (or "catchall" handler).
 *
 *   First, end the prior block.  Then, unconditionally,
 *   let execution enter into the catchall code.  As with a normal
 *   catch, a variable is declared to support lexical
 *   nesting of RERAISE statements, and the state of the current
 *   exception is changed to "handled".
 */
#define CATCH_ALL \
            } \
            else { \
		EXCEPTION *THIS_CATCH = (EXCEPTION *)&exc_ctx.cur_exception;\
		exc_ctx.exc_state = exc_handled_c;
/*		{ user's block of code goes here } 	*/

/* 
 * Define a RERAISE statement.
 * 
 *   This "statement" is valid only if lexically nested in
 *   a CATCH or CATCH_ALL clause. Reraise the current lexically visible 
 *   exception.
 */
#define RERAISE exc_raise(THIS_CATCH)

/* 
 * Define a FINALLY clause
 *
 *   This "keyword" starts a FINALLY clause.  It must appear before
 *   an ENDTRY.  A FINALLY clause will be entered after normal exit
 *   of the TRY block, or if an unhandled exception tries to propagate
 *   out of the TRY block.  
 *
 *   Unlike Modula 3's TRY clause, we do not expend overhead trying to
 *   enforce that FINALLY be mutually exclusive with CATCH clauses.  Currently, 
 *   if they are used together, then control will drop into a FINALLY clause 
 *   under the following conditions:
 *	o normal exit from TRY, 
 *	o an exception is raised and no CATCH is present (recommended usage) 
 *	o CATCH's are present but none matches the exception.
 *	o CATCH's are present and one matches the exception, but it
 *	  does not raise any exception.  
 *   That is, FINALLY is always entered after TRY unless a CATCH clause raises 
 *   (or re-raises) an exception.
 *
 *			** WARNING **
 *   You should *avoid* using FINALLY with CATCH clauses, that is, use it 
 *   only as TRY {} FINALLY {} ENDTRY.  Source code that combines CATCHes
 *   with FINALLY in the same TRY clause is considered "unsupported"
 *   -- that is, such code may be broken by a future version of this
 *   package.  
 *
 *   There are several reasons this restriction is necessary:
 *	o FINALLY may be added to C++ and its combination with CATCH
 *	  clauses may have different semantics than implemented by these macros.
 *	o The restriction is consistant with the same restriction in Modula 3
 *	o It allows the use of the 2-phase or "debugging" implementation 
 *	  technique of the SRC exception package for these same macros.
 */
#ifndef HP_CMA_EXC_MACROS_OPT
#ifdef _HP_CMA_
#define exc_pop_ctx_(cb) \
         {                                                              \
                cma__g_current_thread->exc_stack = (exc_context_t *)(cb)->link; \
                if (!(cb)->exc_state) {                                 \
                        exc_raise ((EXCEPTION *)&(cb)->cur_exception);  \
                }                                                       \
                (cb)->exc_state = exc_popped_c;                         \
         }

#define FINALLY   } \
        if (exc_ctx.exc_state == exc_none_c) \
            exc_pop_ctx_ (&exc_ctx);\
        {
/*              { user's block of code goes here }      */
#else /* _HP_CMA_ */
#define FINALLY   } \
        if (exc_ctx.exc_state == exc_none_c) \
            exc_pop_ctx (&exc_ctx);\
        {
/*              { user's block of code goes here }      */
#endif /* _HP_CMA_ */
#else /* HP_CMA_EXC_MACROS_OPT */
#define FINALLY   } \
	if (exc_ctx.exc_state == exc_none_c) \
	    exc_pop_ctx (&exc_ctx);\
	{
/*		{ user's block of code goes here } 	*/
#endif

/* 
 * End the whole TRY clause
 */
#ifndef HP_CMA_EXC_MACROS_OPT
#ifdef _HP_CMA_
#define ENDTRY \
        } \
    exc_unestablish (&exc_ctx); \
    if (exc_ctx.exc_state == exc_none_c \
            || exc_ctx.exc_state == exc_active_c) { \
        exc_pop_ctx_ (&exc_ctx); \
        } \
    }
#else /* _HP_CMA_ */
#define ENDTRY \
        } \
    exc_unestablish (&exc_ctx); \
    if (exc_ctx.exc_state == exc_none_c \
            || exc_ctx.exc_state == exc_active_c) { \
        exc_pop_ctx (&exc_ctx); \
        } \
    }
#endif /* _HP_CMA_ */
#else /* HP_CMA_EXC_MACROS_OPT */
#define ENDTRY \
	} \
    exc_unestablish (&exc_ctx); \
    if (exc_ctx.exc_state == exc_none_c \
	    || exc_ctx.exc_state == exc_active_c) { \
	exc_pop_ctx (&exc_ctx); \
	} \
    }
#endif

#if (_EXC_OS_ == _EXC__VMS) && (_EXC_HARDWARE_ == _EXC__VAX) && (_EXC_COMPILER_ == _EXC__DECCPLUS)
# pragma __extern_model __restore
#endif

#ifdef __cplusplus
}
#endif

#endif
