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
 * $Log: cma_config.h,v $
 * Revision 1.2.15.2  1996/02/18  23:09:28  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:33:54  marty]
 *
 * Revision 1.2.15.1  1995/12/07  22:00:26  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:04:53  root]
 * 
 * Revision 1.2.12.2  1994/06/09  13:37:30  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:27:29  devsrc]
 * 
 * Revision 1.2.12.1  1994/04/01  20:15:39  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:16:11  jd]
 * 
 * Revision 1.2.10.3  1993/06/24  20:51:23  hinman
 * 	[hinman@sni] - Keep hardware from being defined as _CMA__I386 for SINIX
 * 	[1993/06/18  16:34:31  hinman]
 * 
 * Revision 1.2.10.2  1993/05/24  20:49:42  cjd
 * 	Submitting 102-dme port to 103i
 * 	[1993/05/24  20:16:57  cjd]
 * 
 * Revision 1.2.8.2  1993/05/11  21:46:54  jd
 * 	Initial 486 port.
 * 	[1993/05/11  21:42:51  jd]
 * 
 * Revision 1.2.2.5  1993/02/01  21:47:14  hinman
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  17:25:14  hinman]
 * 
 * 	[hinman] - Check in merged SNI version
 * 	[1993/01/11  16:18:49  hinman]
 * 
 * Revision 9.5.1.7  92/10/26  12:39:18  bein
 * 	PTC - PYRMIPS now uses reentrant libs.
 * 
 * Revision 9.5.4.2  92/10/26  10:30:37  bein
 * 	PTC - We use reentrant libraries now, so no special case is needed.
 * 
 * Revision 9.5.1.6  92/10/01  13:07:19  raj
 * 	[raj] Fix 1.0.2 merge inconsitencies
 * 
 * Revision 9.5.3.2  92/10/01  13:06:22  raj
 * 	[raj] Fix 1.0.2 merge inconsitencies
 * 
 * Revision 9.12.1.2  92/09/30  10:32:28  hinman
 * 	Auto checkin of OSF rev 1.2.2.3
 * 
 * Revision 1.2.2.3  1992/09/29  20:14:48  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:40:55  sekhar]
 * 
 * Revision 1.2.2.2  1992/09/03  14:35:30  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:02:07  bolinger]
 * 
 * Revision 1.1.2.3  1992/08/14  20:21:05  bolinger
 * 	Hardwire _USER_THREADS_ on for the two reference platforms.
 * 	[1992/08/14  20:00:15  bolinger]
 * 
 * 	Bring forward 1.0 def of _CMA_VOID_ for OSF/1.
 * 	[1992/08/10  15:18:42  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/08  22:53:38  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  22:53:21  bolinger]
 * 
 * Revision 1.2  1992/01/19  22:14:46  devrcs
 * 	Dropping DCE1.0 OSF1_misc port archive
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
 *	Configuration header file to set up control symbols.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	26 January 1990
 *
 *  MODIFIED BY:
 *
 *	Dave Butenhof
 *	Bob Conti
 *	Paul Curtin
 *	Webb Scales
 */

#ifndef CMA_CONFIG
#define CMA_CONFIG

/*
 *  INCLUDE FILES
 */

/*
 * CONSTANTS AND MACROS
 */

/*
 * Quick reference guide to DECthreads configuration symbols:
 *
 *	_CMA_COMPILER_		(CC, VAXC, DECC, CFRONT, GCC,
 *				DECCPLUS, GCPLUS)
 *			Each compiler has its own quirks, and DECthreads
 *			tries to do as much with each as possible. For
 *			example, it instantiates function prototypes where
 *			possible, even if the compiler is not fully ANSI C
 *			compliant.
 *	_CMA_HARDWARE_		(MIPS, VAX, M68K, HPPA, IBMR2, ALPHA)
 *			The computer on which DECthreads will run.
 *	_CMA_OS_		(UNIX, VMS)
 *			The operating system on which DECthreads will run;
 *			generally, all UNIX-descended systems have certain
 *			similarities which need to be considered as a group.
 *	_CMA_VENDOR_		(APOLLO, DIGITAL, HP, IBM, OSF, SUN)
 *			The company supplying the O/S, since each have
 *			slightly different behavior and requirements.
 *	_CMA_PLATFORM_		(MIPS/UNIX, VAX/VMS, VAX/UNIX, M68K/UNIX,
 *				HPPA/UNIX, RS6000/UNIX, ALPHA/UNIX ALPHA/VMS)
 *			This is a convenience to test both hardware and O/S
 *			variants together.
 *	_CMA_PROTECT_MEMORY_	(0, 1)
 *			This is TRUE (1) if the environment supports
 *			protecting memory pages (DECthreads will set stack
 *			guard pages to no access to trap stack overflows).
 *	_CMA_OSIMPL_		(AIX, OSF, BSD, SYSV)
 *			The closest ancestor of the operating system, since
 *			each family inherits common characteristics
 *			(particularly in signal behaviors).
 *	_CMA_STACK_TRACE_	(0, 1)
 *			This can be set to cause DECthreads to generate trace
 *			messages when stack management operations are called.
 *			It triggers conditional compilation in cma_stack.c.
 *	_CMA_VOID_		(0, 1)
 *			This is TRUE (1) if the compiler supports the use of
 *			"void *" types. Most __STDC__ compilers do (except
 *			that a version of MIPS C had a bug that prevented its
 *			use). If _CMA_VOID_ is FALSE (0), DECthreads will use
 *			"char *" instead.
 *	_CMA_VOLATILE_		("volatile" or null)
 *			DECthreads requires "volatile" storage class in
 *			several places, including exception handling. Since
 *			some compilers do not support "volatile", DECthreads
 *			uses this symbol in place of the "volatile" keyword.
 *			Note that DECthreads runtimes built with compilers
 *			that do not support volatile may show incorrect
 *			behavior under certain circumstances (particularly
 *			during delivery of exceptions). Because these
 *			situations tend to occur only when errors have
 *			already occurred, it is hoped that the absence of
 *			volatile will not prevent normal DECthreads
 *			operation.
 *	_CMA_UPSTACK_		(0, 1)
 *			This specifies the direction of stack growth on the
 *			target platform. If TRUE (1), then a stack "push"
 *			(e.g., for a function call) causes the stack pointer
 *			value to INCREASE. If FALSE (0), a stack "push"
 *			causes the stack pointer value to DECREASE.
 *	_CMA_KTHREADS_		(NONE, MACH)
 *			This specifies the type of kernel threads (if any)
 *			supported by the target. In general, DECthreads will
 *			map user threads onto kernel threads if they are
 *			available.
 *	_CMA_UNIPROCESSOR_	(0, 1)
 *			A value of TRUE (1) means that kernel threads are not
 *			supported and threads are a purely user-mode
 *			abstraction. This allows certain optimizations (for
 *			example, "current thread" can be implemented as a
 *			fetch from a global variable rather than a search for
 *			the stack pointer).
 *	_CMA_MULTIPLEX_		(0, 1)
 *			If TRUE (1) then kernel threads are supported, and
 *			DECthreads additionally will multiplex multiple user
 *			threads on each kernel thread. This balances some of
 *			the advantages and disadvantages of each (user mode
 *			context switching is faster, but use of kernel
 *			threads is more robust since kernel functions
 *			generally block only the calling kernel thread).
 *	_CMA_THREAD_IS_VP_	(0, 1)
 *			A convenience, meaning that kernel threads are being
 *			used, and no multiplexing is being done. It is
 *			computed from _CMA_MULTIPLEX_ and _CMA_KTHREADS_.
 *	_CMA_THREAD_SYNC_IO_	(0, 1)
 *			If TRUE (1) then the system supports "thread
 *			synchronous I/O", and DECthreads does not have to
 *			emulate it. This is automatically set when DECthreads
 *			is not configured for a uniprocessor OR for
 *			multiplexing on kernel threads, but can be overridden
 *			if the O/S supports thread sync. I/O on user-mode
 *			threads.
 *	_CMA_PER_THD_SYNC_SIGS_	(0, 1)
 *			If TRUE (1) then the system supports per-thread
 *			synchronous signals.  Currently, this is set only for
 *			one-to-one thread mapping on OSF/1 based systems.
 *	_CMA_REENTRANT_CLIB_	(0, 1)
 *			If TRUE (1) then the C library functions are thread
 *			reentrant. Currently, this is assumed only if
 *			_CMA_THREAD_IS_VP_ or the O/S is descended from OSF/1
 *			(which provides a libc_r library that allows
 *			DECthreads to make the C functions reentrant even
 *			when it is context switching in user mode).
 *	_CMA_MP_HARDWARE_	(0, 1)
 *			Generally, this is the same as "_CMA_KTHREADS_ !=
 *			_CMA__NONE" (kernel threads are assumed to run on
 *			multiprocessor hardware), but should be overridden if
 *			kernel threads are being used and it isn't desirable
 *			to assume MP hardware (setting this for non-MP
 *			hardware may degrade performance).
 *	_CMA_SPINLOOP_		(0, n)
 *			For MP hardware, DECthreads can be configured to
 *			"spin" on a mutex lock for some time before giving up
 *			and blocking the thread. If set to 0, DECthreads will
 *			not spin. Otherwise, _CMA_SPINLOOP_ determines the
 *			number of times DECthreads will try to acquire the
 *			lock before blocking.
 *	_CMA_NO_POSIX_SIGNAL_	(1 or undefined)
 *			If defined, the target platform doesn't support a
 *			POSIX-compatible sigaction() function; DECthreads
 *			will use sigvec() instead. NOTE: must be tested with
 *			#ifdef.
 *	_CMA_VARARGS_		(1 or undefined)
 *			(Not currently used; probably obsolete)
 *	_CMA_IMPORT_, _CMA_EXPORT_, _CMA_EXPORT_FLAG_
 *			Specifies the keywords used for EXPORTing variables
 *			from DECthreads, or IMPORTing those variables to
 *			client code. For normal UNIX systems, IMPORT is
 *			usually "extern" and EXPORT is usually null. Because
 *			of oddities in the VAX C implementation of extern,
 *			DECthreads uses "globaldef" for EXPORT and
 *			"globalref" for IMPORT. _CMA_EXPORT_FLAG_ is 1 (TRUE)
 *			if _CMA_EXPORT_ has a non-null value, and 0 (FALSE)
 *			otherwise.
 *	_CMA_TRACE_KERNEL_	(n or undefined)
 *			If defined, DECthreads will allocate an array of "n"
 *			elements and trace information relating to the use of
 *			the "kernel critical" lock (the user mode scheduling
 *			lock). This can be examined from the debugger, or
 *			printed by cma__format_karray(). It shows the module,
 *			line number, and thread ID of the last n kernel
 *			lock/unlock operations.
 *	_CMA_TRACE_SEM_		(n or undefined)
 *			If defined, DECthreads will allocate an array of "n"
 *			elements and trace information relating to the use of
 *			internal semaphores (the basic blocking mechanism
 *			used for mutexes and condition variables). This can
 *			be examined from the debugger, or printed by
 *			cma__sem_format_array(). It shows the module, line
 *			number, thread ID, and semaphore opcode of the
 *			last n semaphore operations.
 *	_CMA_NOWRAPPERS_	(1 or undefined)
 *			If defined, DECthreads will not use its I/O and C
 *			library wrapper functions. Generally, this is set to
 *		    	1 for building DECthreads, and undefined for building
 *			client code.
 *	_CMA_VSSCANF_		(0, 1)
 *			If set to 1, the DECthreads stdio wrappers will
 *			include the scanf family. The wrappers cannot be
 *			built without the real "v*scanf" function, which don't
 *			exist on most platforms; but we're prepared if we
 *			ever find a platform that does support them. Note that
 *			we assume that using one symbol implicitly assumes
 *			that if one of the v*scanf family is present, they
 *			all will be.
 */

/*
 * NOTE: all configuration symbols are set up such that they can be
 * overridden by a -D switch on the command line if desired (but be sure
 * that you know what you're doing).
 */

/*
 * Name of the platform C compiler
 */
#define _CMA__CC	1
#define _CMA__VAXC	2
#define _CMA__DECC	3
#define _CMA__CFRONT	4
#define _CMA__GCC	5
#define _CMA__DECCPLUS	6
#define _CMA__GCPLUS	7
#define _CMA__SIEMENSC  8

/*
 * Test for C++ compilers before C compilers because Glockenspiel C++ also
 * defines symbols for the VAX C compiler and this could be the case for
 * other C++/C compiler combinations
 */
#ifndef _CMA_COMPILER_
# if defined(__cplusplus)		/* test for other C++ compilers first */
#  if defined(__DECCXX)
#   define _CMA_COMPILER_	_CMA__DECCPLUS
#  else
#   define _CMA_COMPILER_	_CMA__CFRONT
#  endif
# elif defined(__decc) || defined(__DECC)
#  define _CMA_COMPILER_	_CMA__DECC
# elif defined(vaxc) || defined(VAXC) || defined(__vaxc) || defined(__VAXC)
#  define _CMA_COMPILER_	_CMA__VAXC
# elif defined(sinix)
#  define _CMA_COMPILER_	_CMA__SIEMENSC
# elif defined(__GNUC__) || defined(__GNUC) || defined(__gnuc)
#  define _CMA_COMPILER_	_CMA__GCC
# else
#  define _CMA_COMPILER_	_CMA__CC
# endif
#endif

/*
 * Name of the hardware platform
 */
#define	_CMA__MIPS	1
#define	_CMA__VAX	2
#define _CMA__M68K	3
#define _CMA__HPPA	4
#define _CMA__IBMR2     5
#define _CMA__ALPHA	6
#define _CMA__MX300I    7
#define	_CMA__CPLMIPS	8
#define _CMA__I386      9

#ifndef	_CMA_HARDWARE_
# if defined(vax) || defined (VAX) || defined(__vax) || defined(__VAX)
#  define	_CMA_HARDWARE_	_CMA__VAX
# endif
# if defined(mips) || defined(MIPS) || defined(__mips) || defined(__MIPS) || defined(__MIPSEL__) || defined(__mips__)
#  if defined(SNI_DCOSX)
#  define	_CMA_HARDWARE_	_CMA__CPLMIPS
#  else
#  define	_CMA_HARDWARE_	_CMA__MIPS
#  endif
# endif
# if defined(m68k) || defined(m68000) || defined(_ISP__M68K) || defined(M68000) || defined(mc68000) 
#  define	_CMA_HARDWARE_	_CMA__M68K
# endif
# if defined(hp9000s300) || defined(__hp9000s300)
#  define	_CMA_HARDWARE_	_CMA__M68K
# endif
# if defined(__hppa)
#  define	_CMA_HARDWARE_	_CMA__HPPA
# endif
# if defined(_IBMR2)
#  define _CMA_HARDWARE_	_CMA__IBMR2
# endif
# if defined(__ALPHA) || defined(__alpha)
#  define _CMA_HARDWARE_	_CMA__ALPHA
# endif
# if defined(SNI_MX300I)
#  define _CMA_HARDWARE_ _CMA__MX300I
# endif
# if !defined(SNI_SVR4)
#  ifdef __i386
#   define       _CMA_HARDWARE_  _CMA__I386
#  endif
# endif 
# ifndef _CMA_HARDWARE_
   #error "_CMA_HARDWARE_ not set"
# endif
#endif

/*
 * Name of the software platform
 */
#define	_CMA__UNIX	1
#define	_CMA__VMS	2
#define _CMA__SVR4  	3
#define _CMA__BSD	4

#ifndef	_CMA_OS_
# if defined(unix) || defined(__unix) || defined(__unix__) || defined(_AIX) || defined(__OSF__) || defined(__osf__) || defined(SNI_SVR4)
#     define	_CMA_OS_	_CMA__UNIX
#     ifdef SNI_SVR4
#		define _CMA_UNIX_TYPE _CMA__SVR4
#		if defined(SNI_BSD_CMP)
#			define _CMA_BSD_CMP_	1
#		endif /* defined(SNI_BSD_CMP) */
#     else
#		define _CMA_UNIX_TYPE _CMA__BSD
#     endif /* SNI_SVR4 */
# endif 
# if defined(vms) || defined(__vms) || defined(VMS) || defined(__VMS) || defined(__vms__)
#  define	_CMA_OS_	_CMA__VMS
# endif
# ifndef _CMA_OS_
   #error "_CMA_OS_ not set"
# endif
#endif

/*
 * Name of the software vendor
 */
#define _CMA__APOLLO	1
#define _CMA__DIGITAL	2
#define _CMA__HP        3
#define _CMA__IBM       4
#define _CMA__OSF	5
#define _CMA__SUN       6
#define _CMA__SNI	7
#define	_CMA__PTC	8

#ifndef _CMA_VENDOR_
# ifdef apollo
#  define	_CMA_VENDOR_	_CMA__APOLLO
# endif
# if _CMA_OS_ == _CMA__VMS
#  define	_CMA_VENDOR_	_CMA__DIGITAL
# endif
# if defined(ultrix) || defined(__ULTRIX) || defined (__ultrix)
#  define	_CMA_VENDOR_	_CMA__DIGITAL
# endif
# if defined(__osf__) && !defined(__OSF__) && (_CMA_HARDWARE_ == _CMA__MIPS)
#  define	_CMA_VENDOR_	_CMA__DIGITAL
# endif
# if defined(__osf__) && !defined(__OSF__) && (_CMA_HARDWARE_ == _CMA__ALPHA)
#  define	_CMA_VENDOR_	_CMA__DIGITAL
# endif
# if defined(hpux) || defined(__hpux)
#  define	_CMA_VENDOR_	_CMA__HP
# endif
# ifdef _IBMR2
#  define 	_CMA_VENDOR_	_CMA__IBM
# endif
# ifdef sun
#  define	_CMA_VENDOR_	_CMA__SUN
# endif
# ifdef SNI_SINIX
#  define 	_CMA_VENDOR_	_CMA__SNI
# endif
# if defined (__OSF__) && !defined (_CMA_VENDOR_)
#  define	_CMA_VENDOR_	_CMA__OSF
# endif
# ifdef SNI_DCOSX
#  define 	_CMA_VENDOR_	_CMA__PTC
# endif
# ifndef _CMA_VENDOR_
   #error "_CMA_VENDOR_ not set"
# endif
#endif

/*
 * Combined platform (OS + hardware)
 */
#define	_CMA__MIPS_UNIX		1
#define	_CMA__VAX_VMS		2
#define _CMA__VAX_UNIX		3
#define _CMA__M68K_UNIX		4
#define _CMA__HPPA_UNIX		5
#define _CMA__IBMR2_UNIX	6
#define _CMA__ALPHA_UNIX	7
#define _CMA__ALPHA_VMS		8
#define _CMA__SINIX_MX300I	9
#define _CMA__DCOSX_MIPS	10
#define _CMA__I386_UNIX         11

#ifndef	_CMA_PLATFORM_
# if _CMA_OS_ == _CMA__UNIX
#  if _CMA_HARDWARE_ == _CMA__MIPS
#   define _CMA_PLATFORM_	_CMA__MIPS_UNIX
#  endif
#  if _CMA_HARDWARE_ == _CMA__VAX
#   define _CMA_PLATFORM_	_CMA__VAX_UNIX
#  endif
#  if _CMA_HARDWARE_ == _CMA__M68K
#   define _CMA_PLATFORM_	_CMA__M68K_UNIX
#  endif
#  if _CMA_HARDWARE_ == _CMA__HPPA
#   define _CMA_PLATFORM_	_CMA__HPPA_UNIX
#  endif
#  if _CMA_HARDWARE_ == _CMA__IBMR2
#   define _CMA_PLATFORM_	_CMA__IBMR2_UNIX
#  endif
#  if _CMA_HARDWARE_ == _CMA__ALPHA
#   define _CMA_PLATFORM_	_CMA__ALPHA_UNIX
#  endif
#  if (_CMA_HARDWARE_ == _CMA__MX300I) && (_CMA_UNIX_TYPE == _CMA__SVR4)
#	define _CMA_PLATFORM_ _CMA__SINIX_MX300I
#  endif
#  if (_CMA_HARDWARE_ == _CMA__CPLMIPS)
#	define _CMA_PLATFORM_	_CMA__DCOSX_MIPS
#  endif
# endif
# if _CMA_OS_ == _CMA__VMS
#  if _CMA_HARDWARE_ == _CMA__VAX
#   define _CMA_PLATFORM_	_CMA__VAX_VMS
#  endif
#  if _CMA_HARDWARE_ == _CMA__ALPHA
#   define _CMA_PLATFORM_	_CMA__ALPHA_VMS
#  endif
# endif
#  if _CMA_HARDWARE_ == _CMA__I386
#   define _CMA_PLATFORM_       _CMA__I386_UNIX
#  endif
# ifndef _CMA_PLATFORM_
   #error "_CMA_PLATFORM_ not set"
# endif
#endif

/*
 * Set to 1 if system supports setting memory page protection (see
 * cma_stack.c for use of page protection routines; "cma_vmprot.h" defines
 * interface to generic jacket routines "cma__set_noaccess" and
 * "cma__set_access").
 */
#ifndef _CMA_PROTECT_MEMORY_
# if _CMA_PLATFORM_ == _CMA__VAX_VMS
#  define _CMA_PROTECT_MEMORY_	1
# endif
# if _CMA_PLATFORM_ == _CMA__MIPS_UNIX
#  define _CMA_PROTECT_MEMORY_  1
# endif
# if _CMA_PLATFORM_ == _CMA__VAX_UNIX
#  define _CMA_PROTECT_MEMORY_  1
# endif
# if _CMA_PLATFORM_ == _CMA__SINIX_MX300I
#  define _CMA_PROTECT_MEMORY_  1
# endif
# if _CMA_PLATFORM_ == _CMA__DCOSX_MIPS
#  define _CMA_PROTECT_MEMORY_  1
# endif
# if _CMA_PLATFORM_ == _CMA__I386_UNIX
#  define _CMA_PROTECT_MEMORY_ 1
# endif
#endif

#define _CMA__OS_AIX	1
#define _CMA__OS_OSF	2
#define _CMA__OS_BSD	3
#define _CMA__OS_SYSV	4
#define _CMA__OS_VMS	5

/*
 * MIPS C on DEC OSF/1 sets __osf__ but not __OSF__; but gcc on "raw" OSF/1
 * sets __OSF__ but not __osf__. This little ditty provides a bridge.
 */
#if defined (__OSF__) && !defined (__osf__)
# define __osf__
#endif

#ifndef _CMA_OSIMPL_
# if _CMA_OS_ == _CMA__VMS
#  define _CMA_OSIMPL_		_CMA__OS_VMS
# else
#  if defined (__osf__)
#   define _CMA_OSIMPL_		_CMA__OS_OSF
#  else
#   if _CMA_VENDOR_ == _CMA__IBM
#    define _CMA_OSIMPL_	_CMA__OS_AIX
#   else
#    if _CMA_VENDOR_ == _CMA__SUN
#     define _CMA_OSIMPL_	_CMA__OS_SYSV
#    else
#     define _CMA_OSIMPL_	_CMA__OS_BSD
#    endif
#   endif
#  endif
# endif
#endif

/*
 * The stack manager module (cma_stack.c) can printf messages which can be
 * useful for debugging (if changes are made to stack management for a
 * platform).
 */
#ifndef _CMA_STACK_TRACE_
# define _CMA_STACK_TRACE_	0	/* DEBUG (report stack management) */
#endif

/*
 * Define whether to use "void *" or "char *" pointers, based on whether the
 * compiler can support them.
 */
#ifndef	_CMA_VOID_
# ifdef __STDC__
#  define _CMA_VOID_		1
# endif
# if _CMA_OSIMPL_ == _CMA__OS_OSF
#  define _CMA_VOID_        1
# endif
# if _CMA_COMPILER_ == _CMA__VAXC 
#  define _CMA_VOID_	        1
# endif
# if _CMA_COMPILER_ == _CMA__DECCPLUS
#  define _CMA_VOID_		1
# endif
# if _CMA_COMPILER_ == _CMA__GCPLUS
#  define _CMA_VOID_		1
# endif
# if _CMA_COMPILER_ == _CMA__CFRONT
#  define _CMA_VOID_		1
# endif
# if _CMA_COMPILER_ == _CMA__DECC
#  define _CMA_VOID_		1
# endif
# if _CMA_VENDOR_ == _CMA__SUN
#  define _CMA_VOID_            1
# endif
# if _CMA_VENDOR_ == _CMA__HP
#  define _CMA_VOID_            1
# endif
# if _CMA_PLATFORM_ == _CMA__SINIX_MX300I
#  define _CMA_VOID_       1
# endif
# if _CMA_PLATFORM_ == _CMA__DCOSX_MIPS
#  define _CMA_VOID_       1
# endif
# ifndef _CMA_VOID_
#  define _CMA_VOID_		0
# endif
#endif

/*
 * Certain structures within CMA (especially in exception handling) should be
 * marked "volatile", however some C compilers do not support "volatile" on
 * structures.  Set this to "volatile" unless using such a deficient
 * compiler, in which case it should be "".
 */
#ifndef	_CMA_VOLATILE_
# if _CMA_PLATFORM_ == _CMA__VAX_UNIX
#  if _CMA_COMPILER_ == _CMA__CC
#   define _CMA_VOLATILE_
#   define _CMA_VOLATILE_FLAG_		0
#  endif
# endif
# if _CMA_VENDOR_ == _CMA__SUN
#  define _CMA_VOLATILE_
#  define _CMA_VOLATILE_FLAG_		0
# endif
# if _CMA_COMPILER_ == _CMA__CFRONT
#  define _CMA_VOLATILE_
#  define _CMA_VOLATILE_FLAG_		0
# endif
#ifndef	_CMA_VOLATILE_
#  define _CMA_VOLATILE_  		volatile
#  define _CMA_VOLATILE_FLAG_		1
# endif
#endif

/*
 * This symbol defines whether stacks grow towards lower addresses or higher
 * addresses (_CMA_UPSTACK_ is defined if the stack grows up).
 */
#ifndef _CMA_UPSTACK_
# if _CMA_HARDWARE_ == _CMA__HPPA
#  define _CMA_UPSTACK_ 1
# endif
#endif

/*
 * If the platform supports kernel threads, then the DECthreads VP layer can
 * provide parallel computation. This symbol defines the variety of kernel
 * threads supported by the platform.
 */
#define _CMA__NONE	0
#define _CMA__MACH	1

#ifndef _CMA_KTHREADS_
# if _CMA_OSIMPL_ == _CMA__OS_OSF
#  define _CMA_KTHREADS_	_CMA__MACH
# else
#  define _CMA_KTHREADS_	_CMA__NONE
# endif
#endif

/*
 * This symbol, if defined, provides for some shortcuts that can be made when
 * only uniprocessor hardware is supported (for example, using a fixed entry
 * for "current thread" instead of search stack clusters).
 *
 * If the builder has specifically defined _CMA_UNIPROCESSOR_ to 1, then
 * build a non-kernel-thread version even if the system supports kernel
 * threads, by setting _CMA_KTHREADS_ to _CMA__NONE.
 *
 * Note that if _USER_THREADS_ is set, then _CMA_UNIPROCESSOR_ is explicitly
 * defined to "TRUE" regardless of the platform. That overrides the default
 * user/kernel threads decision, forcing use of user threads. If kernel
 * threads have been determined to be present, they are turned off.
 */

/*
 * Hardwire _USER_THREADS_ on for the reference platforms.
 */
#if _EXC_PLATFORM_ == _EXC__IBMR2_UNIX || _EXC_VENDOR_ == _EXC__OSF
# define _USER_THREADS_	1
#endif

#ifdef _USER_THREADS_
# define _CMA_UNIPROCESSOR_	1
#endif

#ifndef _CMA_UNIPROCESSOR_
# if _CMA_KTHREADS_ == _CMA__NONE
#  define _CMA_UNIPROCESSOR_	1
# else
#  define _CMA_UNIPROCESSOR_	0
# endif
#elif _CMA_UNIPROCESSOR_
# undef _CMA_KTHREADS_
# define _CMA_KTHREADS_		_CMA__NONE
#endif

/*
 * Specify whether this implementation will multiplex on top of VPs
 */
#ifndef _CMA_MULTIPLEX_
# if !_CMA_UNIPROCESSOR_
#  define _CMA_MULTIPLEX_	0	/* No support yet! */
# endif
# ifndef _CMA_MULTIPLEX_
#  define _CMA_MULTIPLEX_	0
# endif
#endif

/*
 * _CMA_THREAD_IS_VP_ is true IFF the platform supports kernel threads and
 * each DECthreads thread is permanently bound to a specific kernel thread
 * throughout its life: in other words, it is a kernel thread platform and
 * threads are not multiplexed in user mode.
 */
#ifndef _CMA_THREAD_IS_VP_
# if _CMA_MULTIPLEX_ || (_CMA_KTHREADS_ == _CMA__NONE)
#  define _CMA_THREAD_IS_VP_	0
# else
#  define _CMA_THREAD_IS_VP_	1
# endif
#endif

/*
 * _CMA_THREAD_SYNC_IO_ is true IFF a blocking I/O function (e.g., read())
 * will block only the DECthreads thread that issued the call. It should
 * generally be false if _CMA_MULTIPLEX_ || _CMA_UNIPROCESSOR_, but can be
 * controlled separately. If _CMA_THREAD_SYNC_IO_ is true, the DECthreads I/O
 * wrapper functions will not be compiled.
 */
#ifndef _CMA_THREAD_SYNC_IO_
# if _CMA_MULTIPLEX_ || _CMA_UNIPROCESSOR_
#  define _CMA_THREAD_SYNC_IO_	0
# else
#  define _CMA_THREAD_SYNC_IO_	1
# endif
#endif

/*
 * _CMA_PER_THD_SYNC_SIGS_ is true IFF the system supports per-thread
 * synchronous signal actions for DECthreads threads. Generally, that means
 * that kernel threads support per-thread sync. signals, and DECthreads is
 * mapped one-to-one on kernel threads (_CMA_THREAD_IS_VP_).
 */
#ifndef _CMA_PER_THD_SYNC_SIGS_
# if _CMA_THREAD_IS_VP_ && (_CMA_OSIMPL_ == _CMA__OS_OSF)
#  define _CMA_PER_THD_SYNC_SIGS_	1
# else
#  define _CMA_PER_THD_SYNC_SIGS_	0
# endif
#endif

/*
 * _CMA_REENTRANT_CLIB_ is true IFF the platform's C library is reentrant.
 * This is generally true on a platform where _CMA_THREAD_IS_VP_ is true, but
 * may also be true for multiplexed threads if the C library is designed
 * properly (for example, the OSF/1 libc_r library allows thread packages to
 * pass it a vector of interlock management functions, rather than assuming
 * that some Mach thread synchronization mechanism is sufficient).
 */
#ifndef _CMA_REENTRANT_CLIB_
# if _CMA_THREAD_IS_VP_ || (defined(_POSIX_REENTRANT_FUNCTIONS) && (_CMA_HARDWARE_ == _CMA__HPPA)) || (_CMA_UNIX_TYPE ==  _CMA__SVR4)
#  define _CMA_REENTRANT_CLIB_  1
# else
#  define _CMA_REENTRANT_CLIB_	0
# endif
#endif

/*
 * _CMA_MP_HARDWARE_ defines whether DECthreads is being built to support
 * actual multiprocessor hardware, not merely kernel threads. In most cases,
 * the important distinction is kernel threads vs. user multiplexing.
 * However, there are some decisions that ought to be based on whether the
 * kernel threads may actually run on different CPUs concurrently; so we
 * might as well have this convenient symbol.
 */
#ifndef _CMA_MP_HARDWARE_
# if _CMA_KTHREADS_ != _CMA__NONE
#  define _CMA_MP_HARDWARE_	1	/* Assume YES for kernel threads */
# else
#  define _CMA_MP_HARDWARE_	0	/* Assume NO if no kernel threads */
# endif
#endif

/*
 * _CMA_SPINLOOP_ controls the number of times a thread will spin (in a tight
 * loop) attempting to lock a mutex before it gives up and blocks itself.
 *
 * On a uniprocessor configuration, this symbol should be defined to zero;
 * spinning won't accomplish anything but to waste the rest of the thread's
 * timeslice. Even when kernel threads are supported on uniprocessor
 * hardware, this symbol likely has little value, since the thread may still
 * spin (wasting CPU) until the kernel performs a thread context switch.
 */
#ifndef _CMA_SPINLOOP_
# if _CMA_MP_HARDWARE_
#  define	_CMA_SPINLOOP_		100
# else
#  define	_CMA_SPINLOOP_		0
# endif
#endif

/*
 * Some UNIX vendors don't yet provide POSIX compatible sigaction().
 * In this case, use sigvec() instead.
 */
#ifndef _CMA_NO_POSIX_SIGNAL_
# if _CMA_VENDOR_ == _CMA__SUN
#  define _CMA_NO_POSIX_SIGNAL_         1
# endif
#endif

/*
 * Use ANSI varargs method for functions with variable number of
 * arguments.  Not same as  __STDC__, because some compilers support 
 * varargs without  full __STDC__  compatibility.
 */
#ifndef _CMA_VARARGS_
# ifdef __STDC__
#  define _CMA_VARARGS_		1
# endif
# if _CMA_COMPILER_ == _CMA__VAXC
#  define _CMA_VARARGS_		1
# endif
# if _CMA_PLATFORM_ == _CMA__MIPS_UNIX
#  define _CMA_VARARGS_		1
# endif
# if _CMA_VENDOR_ == _CMA__APOLLO
#  define _CMA_VARARGS_		1
# endif
# if _CMA_VENDOR_ == _CMA__SUN
#  define _CMA_VARARGS_		1
# endif
# if _CMA_VENDOR_ == _CMA__HP
#  define _CMA_VARARGS_		1
# endif
# if _CMA_PLATFORM_ == _CMA__IBMR2_UNIX
#  define _CMA_VARARGS_		1
# endif
# if _CMA_PLATFORM_ == _CMA__SINIX_MX300I
#  define _CMA_VARARGS_		1
# endif
# if _CMA_PLATFORM_ == _CMA__DCOSX_MIPS
#  define _CMA_VARARGS_		1
# endif
# if _CMA_PLATFORM_ == _CMA__I386_UNIX
#  define _CMA_VARARGS_         1
# endif
#endif

/*
 * Does this platform need the rs socket routines which are internal
 * functions for reentrant versions of tcpip.so and resolv.so.
 *
 */
#if (_CMA_REENTRANT_CLIB_ && (_CMA_UNIX_TYPE == _CMA__SVR4))
# define _CMA_RS_SOCKET_ 	1
#endif

/*
 * Define the symbols used to "import" and "export" symbols for the client
 * interface.  Note that these shouldn't be used for symbols shared only
 * between CMA modules; it's for those symbols which are "imported" in the
 * cma.h (or pthread.h) header files, such as cma_c_null,
 * pthread_attr_default, and the exception names.
 *
 * On most platforms (with well-integrated C compilers), "import" should be
 * "extern", and "export" should be "".
 */
#if _CMA_PLATFORM_ == _CMA__VAX_VMS
# if _CMA_COMPILER_ == _CMA__DECCPLUS
#  pragma __extern_model __save		/* restored at the end of cma.h */
#  pragma __extern_model __strict_refdef
#  define _CMA_IMPORT_ extern
#  define _CMA_EXPORT_ extern
#  define _CMA_EXPORT_FLAG_	1
# else
#  define _CMA_IMPORT_ globalref
#  define _CMA_EXPORT_ globaldef
#  define _CMA_EXPORT_FLAG_	1
# endif
#else
# define _CMA_IMPORT_ extern
# define _CMA_EXPORT_
# define _CMA_EXPORT_FLAG_	0
#endif

/*
 * The DECthreads stdio formatting wrappers (printf & scanf family) depend on
 * the existence of the stdarg variety of those functions (vsprintf &
 * vsscanf), since DECthreads must pass on the client's variable argument
 * list. Most systems have vsprintf, however we haven't seen one with
 * vsscanf, although it seems a logical extension. The scanf family wrappers
 * are coded, but can't be built without vsscanf: for a system which does
 * supply this elusive function, turn on the _CMA_VSSCANF_ config symbol and
 * rebuild DECthreads.
 */
#ifndef _CMA_VSSCANF_
# define _CMA_VSSCANF_	0
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

#endif
