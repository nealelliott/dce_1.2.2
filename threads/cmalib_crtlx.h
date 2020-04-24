/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: cmalib_crtlx.h $Revision: 1.1.18.1 $ $Date: 1996/05/10 13:45:05 $
 */
/*
 * HISTORY
 * $Log: cmalib_crtlx.h,v $
 * Revision 1.1.18.1  1996/05/10  13:45:05  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:48 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:46 UTC  jss  /main/HPDCE02/1]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/25  15:21  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/21  15:38:18  truitt]
 * 	 *
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:45:05  root  1.1.6.3]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:15:24  hopkins]
 *
 * 	Submitting 102-dme port to 103i
 * 	[1993/05/24  20:18:03  cjd]
 *
 * 	Initial 486 port.
 * 	[1993/05/11  21:44:42  jd]
 *
 * 	CR10892 - fix copyright in file
 * 	[1994/06/09  13:29:00  devsrc]
 * 	 *
 *
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:19:44  jd]
 *
 * 	Submitting 102-dme port to 103i
 * 	[1993/05/24  20:18:03  cjd]
 *
 * 	Initial 486 port.
 * 	[1993/05/11  21:44:42  jd]
 *
 * 	Insert copyright notices
 * 	[1992/12/07  19:44:35  alan]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  19:22:01  root]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:36:30  hopkins]
 *
 * 	Address defect 3483.  Reconfigure configuration symbols so DEC platforms
 * 	can compile either with _STDC_ or without.
 * 	[1992/05/14  17:37:37  keane]
 *
 * 	Loading drop DCE1_0_2FNL
 * 	[1993/05/04  22:36:33  mgm  1.1.1.11]
 *
 * Revision 1.1.12.2  1994/06/09  13:39:14  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:29:00  devsrc]
 * 
 * Revision 1.1.12.1  1994/04/01  20:17:58  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:19:44  jd]
 * 
 * Revision 1.1.10.2  1993/05/24  20:50:34  cjd
 * 	Submitting 102-dme port to 103i
 * 	[1993/05/24  20:18:03  cjd]
 * 
 * Revision 1.1.8.2  1993/05/11  22:02:35  jd
 * 	Initial 486 port.
 * 	[1993/05/11  21:44:42  jd]
 * 
 * Revision 1.1.6.2  1992/12/15  22:33:11  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:44:35  alan]
 * 
 * Revision 1.1.2.2  1992/05/14  20:50:19  keane
 * 	Address defect 3483.  Reconfigure configuration symbols so DEC platforms
 * 	can compile either with _STDC_ or without.
 * 	[1992/05/14  17:37:37  keane]
 * 
 * Revision 1.1  1992/01/19  14:41:23  devrcs
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
 *	CMALIB services
 *
 *  ABSTRACT:
 *
 *	Header file for C run-time library wrapper routines
 *
 *  AUTHORS:
 *
 *	Webb Scales
 *
 *  CREATION DATE:
 *
 *	29 March 1990
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	30 March 1990
 *		Use "int" instead of "sigset_t" type.
 *	002	Webb Scales	12 April 1990
 *		Return to sigset_t, and supply a typedef for it on VMS.
 *	003	Webb Scales	27 April 1990
 *		And correct the routine prototype for sigwait to use sigset_t.
 *	004	Dave Butenhof	18 May 1990
 *		move sigwait to new cma_px.h (POSIX emulation).
 *	005	Dave Butenhof	29 June 1990
 *		Add typecasts to the VM macros to correct for not having
 *		mutable types ("void *").
 *	006	Paul Curtin	17 September 1990
 *		Added module to CMALIB, from CMA.
 *	007	Webb Scales	17 September 1990
 *		Corrected macro values, moved errno macro to CMA.H
 *	008	Dave Butenhof	11 October 1990
 *		Remove redundant cma_errno prototype.
 *	009	Paul Curtin	24 January 1991
 *		Removed arguments from wrapper macros, changed the
 *		return values on protos, and removed arguments from protos.
 *		Return values now match `man' pages.
 *	010	Paul Curtin	28 January 1991
 *		Conditionalized wrapper macros on _CMA_NOWRAPPERS_
 *	011	Paul Curtin	06 February 1991
 *		More reworking of return types.
 *	012	Dave Butenhof	19 February 1991
 *		Fix prototypes to work with C++ (hopefully without breaking
 *		anyone else).
 *	013	Paul Curtin	06 June 1991
 *		Removed wrapper protos.
 *	014	Dave Butenhof	19 September 1991
 *		Integrate HPUX CMA5 reverse drop: remove wrappers if
 *		_POSIX_REENTRANT_FUNCTIONS is defined.
 *	015	Dave Butenhof	19 November 1991
 *		Remove direct check for _POSIX_REENTRANT_FUNCTIONS; rely on
 *		_CMA_REENTRANT_CLIB_, since we control that. OSF/1 defines
 *		_POSIX_REENTRANT_FUNCTIONS, but OSF DCE reference port isn't
 *		using libc_r.a.
 */


#ifndef CMA_CRTLX
#define CMA_CRTLX

/*
 *  INCLUDE FILES
 */
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Determine the proper types used by stdlib.h for memory management
 * functions
 */
#if _CMA_PLATFORM_ == _CMA__IBMR2_UNIX
# define STDLIB_MEM_PTR_T void
# define STDLIB_MEM_SIZE_T size_t
#endif

#if _CMA_PLATFORM_ == _CMA__MIPS_UNIX
#  if _CMA_OSIMPL_ == _CMA__OS_OSF
#   define STDLIB_MEM_PTR_T void
#   define STDLIB_MEM_SIZE_T size_t
#  else
#   define STDLIB_MEM_PTR_T char
#   define STDLIB_MEM_SIZE_T unsigned
#  endif
#endif

#if _CMA_PLATFORM_ == _CMA__I386_UNIX
#   define STDLIB_MEM_PTR_T void
#   define STDLIB_MEM_SIZE_T size_t
#endif

/*
 * PUT YOUR PLATFORM HERE *
# if _CMA_PLATFORM == _???_
#	define STDLIB_MEM_PTR_T ???
#	define STDLIB_MEM_SIZE_T ???
# endif
 */

#if defined(__STDC__) && !defined(STDLIB_MEM_PTR_T)
# define STDLIB_MEM_PTR_T void
# define STDLIB_MEM_SIZE_T size_t
#endif

/*
 * CONSTANTS AND MACROS
 */

/*
 * Memory Allocation Routine Wrappers
 */

#if !_CMA_REENTRANT_CLIB_

#if defined(CMA_CRTLX_IMPLEMENTATION) || !defined(_CMA_NOWRAPPERS_)

/*
 * TYPEDEFS
 */

/*
 *  GLOBAL DATA
 */

/*
 * INTERFACES
 */

/* Each prototype declared in this file is defined as a macro.  The macro
   is expanded twice:  Once to generate a duplicate prototype for the
   system function, ensuring that the prototypes match, and a second time
   to generate a prototype for the wrapper function, ensuring that its
   interface is identical to the interface of the system function.
*/

extern STDLIB_MEM_PTR_T * calloc (STDLIB_MEM_SIZE_T, STDLIB_MEM_SIZE_T);
extern STDLIB_MEM_PTR_T * cma_lib_calloc (STDLIB_MEM_SIZE_T, STDLIB_MEM_SIZE_T);


extern void cfree (STDLIB_MEM_PTR_T *);
extern void cma_lib_cfree (STDLIB_MEM_PTR_T *);


extern void free (STDLIB_MEM_PTR_T *);
extern void cma_lib_free (STDLIB_MEM_PTR_T *);


extern STDLIB_MEM_PTR_T * malloc (STDLIB_MEM_SIZE_T );
extern STDLIB_MEM_PTR_T * cma_lib_malloc (STDLIB_MEM_SIZE_T );


extern STDLIB_MEM_PTR_T * realloc (STDLIB_MEM_PTR_T *ptr, STDLIB_MEM_SIZE_T size);
extern STDLIB_MEM_PTR_T * cma_lib_realloc (STDLIB_MEM_PTR_T *ptr, STDLIB_MEM_SIZE_T size);


#ifndef CMA_CRTLX_IMPLEMENTATION
/* Redefine the system functions names, so that the wrapper functions will
   be called instead of the system functions. */
# define calloc		cma_lib_calloc
# define cfree		cma_lib_cfree
# define free		cma_lib_free
# define malloc		cma_lib_malloc
# define realloc	cma_lib_realloc
#endif /* CMA_CRTLX_IMPLEMENTATION */

#endif  /* CMA_CRTLX_IMPLEMENTATION || !_CMA_NOWRAPPERS_ */

#endif  /*  !_CMA_REENTRANT_CLIB_ */

#ifdef __cplusplus
}
#endif

#endif  /* CMA_CRTLX */

/*  DEC/CMS REPLACEMENT HISTORY, Element CMALIB_CRTLX.H */
/*  *12   19-NOV-1991 12:17:44 BUTENHOF "Remove tests for _POSIX_REENTRANT_FUNCTIONS" */
/*  *11   14-OCT-1991 13:37:21 BUTENHOF "Use new config symbols" */
/*  *10   24-SEP-1991 16:26:09 BUTENHOF "Merge CMA5 reverse IBM/HP/Apollo drops" */
/*  *9    11-JUN-1991 10:36:38 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *8    10-JUN-1991 19:19:51 BUTENHOF "Fix the sccs headers" */
/*  *7    10-JUN-1991 18:16:20 SCALES "Add sccs headers for Ultrix" */
/*  *6     6-JUN-1991 14:08:40 CURTIN "removed wrapper protos" */
/*  *5    20-FEB-1991 16:07:07 CURTIN "" */
/*  *4    19-FEB-1991 17:23:02 BUTENHOF "Integrate new C++ fixes" */
/*  *3     6-FEB-1991 19:17:10 CURTIN "adjusting routine return types" */
/*  *2    28-JAN-1991 11:18:01 CURTIN "Keeping up with the LIBRARY cms" */
/*  *1    15-JAN-1991 22:03:44 BUTENHOF "HACK: interlocked malloc/free until CMALIB" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMALIB_CRTLX.H */
