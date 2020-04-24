/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: cma_util.h $Revision: 1.1.64.1 $ $Date: 1996/05/10 13:43:59 $
 */
/*
 * HISTORY
 * $Log: cma_util.h,v $
 * Revision 1.1.64.1  1996/05/10  13:43:59  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:47 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:45 UTC  jss  /main/HPDCE02/1]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/25  14:55  truitt  /main/HPDCE01/1]
 *
 * 	CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:49  devsrc]
 * 	 *
 *
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:19:21  jd]
 *
 * 	SNI SVR4 ref port 8596
 * 	[1993/09/15  22:31:02  ohara]
 *
 * 	Change polarity of ifdef for including cma performance fixes
 * 	[1993/10/29  18:54:23  ajayb]
 * 	 *
 *
 * 	Add proper ifdefs to include DEPT changes.
 * 	[1993/10/19  19:38:52  ajayb]
 *
 * 	Merging DEPT changes
 * 	[1993/09/30  15:08:33  gaz]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/21  16:00:45  truitt]
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:44:49  root  1.1.7.3]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:14:50  hopkins]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  19:11:19  root]
 *
 * 	Insert copyright notices
 * 	[1992/12/07  19:43:37  alan]
 *
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:07:36  bolinger]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:35:54  hopkins]
 *
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/09  00:10:08  bolinger]
 *
 * 	Separate file open logic from init_trace into a new
 * 	cma__int_fopen() that can be used for bugcheck output, too.
 * 	Also add cma__putformat to do formatted write using debug
 * 	output indirection.
 * 	[1992/05/11  20:32:51  keane]
 *
 * 	Loading drop DCE1_0_2FNL
 * 	[1993/05/04  22:36:10  mgm  1.1.1.11]
 *
 * Revision 1.1.60.2  1994/06/09  13:39:04  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:49  devsrc]
 * 
 * Revision 1.1.60.1  1994/04/01  20:17:43  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:19:21  jd]
 * 
 * Revision 1.1.58.1  1993/10/05  21:38:47  ohara
 * 	SNI SVR4 ref port 8596
 * 	[1993/09/15  22:31:02  ohara]
 * 
 * Revision 1.1.4.3  1992/12/15  22:32:23  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:43:37  alan]
 * 
 * Revision 1.1.4.2  1992/09/03  14:42:01  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:07:36  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/09  00:10:21  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/09  00:10:08  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/14  19:45:52  keane
 * 	Separate file open logic from init_trace into a new
 * 	cma__int_fopen() that can be used for bugcheck output, too.
 * 	Also add cma__putformat to do formatted write using debug
 * 	output indirection.
 * 	[1992/05/11  20:32:51  keane]
 * 
 * Revision 1.1  1992/01/19  14:39:38  devrcs
 * 	Initial revision
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
 *	CMA internal services
 *
 *  ABSTRACT:
 *
 *	Header file for CMA internal UTIL operations
 *
 *  AUTHORS:
 *
 *	Paul Curtin
 *
 *  CREATION DATE:
 *
 *	1 August 1990
 *
 *  MODIFICATION HISTORY:
 * 
 *	001	Paul Curtin	14 September 1990
 *		Added cma__ftime function.
 *	002	Bob Conti	6 October 1990
 *		Added cma__putint_5, cma__putint_10, cma__puthex_8, 
 *		and cma__set_eol_routine.
 *	003	Dave Butenhof	14 December 1990
 *		Change cma_attributes.h to cma_attr.h (shorten length)
 *	004	Dave Butenhof	7 March 1991
 *		Remove cma__strtok function, which is no longer used.
 *	005	Dave Butenhof	27 March 1991
 *		Add conditional cma__trace function.
 *	006	Dave Butenhof	12 April 1991
 *		Move cma__trace definition to cma_defs.h (even though the
 *		code is in cma_util.c) to make it more accessible.
 *	007	Dave Butenhof	24 April 1991
 *		Add cma__init_trace to do more sophisticated trace init.
 *	008	Paul Curtin	20 August 1991
 *		Conditionalized out the include of stdio.h on VMS.
 *	009	Dave Butenhof	18 December 1991
 *		Change interface to cma__getenv() -- it's incorrect, since it
 *		returns address of stack buffer; but while I'm fixing it, I
 *		might as well clean it up and make it reentrant.
 *	010	Dave Butenhof	10 March 1992
 *		Now that we're using gettimeofday() instead of ftime() on
 *		UNIX, change cma__ftime() to cma__gettimeval() [which is like
 *		gettimeofday() except it doesn't return the timezone
 *		information that we don't need anyway].
 *	011	Dave Butenhof	23 March 1992
 *		Separate file open logic from init_trace into a new
 *		cma__int_fopen() that can be used for bugcheck output, too.
 *		Also add cma__putformat to do formatted write using debug
 *		output indirection.
 */

#ifndef	CMA_UTIL
#define CMA_UTIL

/*
 *  INCLUDE FILES
 */

#include <cma.h>
#include <cma_attr.h>
#include <cma_defs.h>

#if _CMA_OS_ == _CMA__VMS
# include <cma_rms.h>
#endif

#if _CMA_VENDOR_ == _CMA__SUN
# include <sys/time.h>
#else
# include <time.h>
#endif

#if _CMA_OS_ == _CMA__UNIX
# include <stdio.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
 * CONSTANTS AND MACROS
 */

#define cma__c_buffer_size  256		    /* Size of output buffer	    */

/*
 * TYPEDEFS
 */

/* 
 * Alternate eol routine
 */
typedef void (*cma__t_eol_routine) (char *);

#if _CMA_OS_ == _CMA__VMS
 typedef struct CMA__T_VMSFILE {
    struct RAB	rab;
    struct FAB	fab;
    } cma__t_vmsfile, 	*cma__t_file;
#elif  ( _CMA_UNIX_TYPE == _CMA__SVR4 )
 typedef int           cma__t_file;
#else
 typedef FILE		*cma__t_file;
#endif

/*
 *  GLOBAL DATA
 */

/*
 * INTERNAL INTERFACES
 */

extern void cma__abort  (void);

extern cma_t_integer cma__atol  (char *);

extern cma_t_integer cma__atoi  (char *);

extern char * cma__getenv  (char *,char *,int);

extern int cma__gettimespec  (struct timespec *);

extern cma__t_file cma__int_fopen  (char *,char *);

#ifndef NDEBUG
extern void cma__init_trace  (char *_env);
#endif

extern char * cma__memcpy  (char *,char *,cma_t_integer);
	
#ifdef _HP_CMA_PERF_
extern char * cma__memset  (char *,cma_t_integer,cma_t_integer);

extern void cma__putformat  (char *,char *,...);

extern void cma__putstring  (char *,char *);

extern void cma__putint  (char *,cma_t_integer);

extern void cma__putint_5  (char *,cma_t_integer);

extern void cma__putint_10  (char *,cma_t_integer);

extern void cma__puthex  (char *,cma_t_integer);

extern void cma__puthex_8  (char *,cma_t_integer);

extern void cma__puteol  (char *);

extern void cma__set_eol_routine  (cma__t_eol_routine,cma__t_eol_routine *);

extern cma_t_integer cma__strlen  (char *);

extern int cma__strncmp  (char *,char *,cma_t_integer);

extern char *cma__gets (char *,char *);
#endif /* _HP_CMA_PERF_ */

#ifdef __cplusplus
}
#endif

#endif
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_UTIL.H */
/*  *13   24-MAR-1992 13:46:52 BUTENHOF "Put bugcheck output in file" */
/*  *12   10-MAR-1992 16:28:15 BUTENHOF "Change ftime to gettimeofday" */
/*  *11   19-DEC-1991 13:08:45 BUTENHOF "Change interface to cma__getenv" */
/*  *10   21-AUG-1991 16:45:50 CURTIN "Removed VMS include of stdio.h" */
/*  *9    10-JUN-1991 19:57:49 SCALES "Convert to stream format for ULTRIX build" */
/*  *8    10-JUN-1991 19:22:18 BUTENHOF "Fix the sccs headers" */
/*  *7    10-JUN-1991 18:25:03 SCALES "Add sccs headers for Ultrix" */
/*  *6     2-MAY-1991 13:59:48 BUTENHOF "Add util init" */
/*  *5    12-APR-1991 23:37:32 BUTENHOF "Move trace to cma_defs.h" */
/*  *4     1-APR-1991 18:09:55 BUTENHOF "Add trace function" */
/*  *3     8-MAR-1991 18:49:21 BUTENHOF "Don't need strtok anymore" */
/*  *2    14-DEC-1990 00:56:08 BUTENHOF "Change module names" */
/*  *1    12-DEC-1990 21:55:22 BUTENHOF "Utilities" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_UTIL.H */
