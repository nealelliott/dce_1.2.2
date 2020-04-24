/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: cma_context.h $Revision: 1.1.8.1 $ $Date: 1996/05/10 13:34:38 $
 */
/*
 * HISTORY
 * $Log: cma_context.h,v $
 * Revision 1.1.8.1  1996/05/10  13:34:38  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:39 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:41 UTC  jss  /main/HPDCE02/1]
 *
 * 	CR10892 - fix copyright in file
 * 	[1994/06/09  13:27:32  devsrc]
 * 	 *
 *
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:16:23  jd]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  14:58  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/20  19:22:01  truitt]
 * 	 *
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:43:06  root  1.1.6.3]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:11:27  hopkins]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  17:55:53  root]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:32:29  hopkins]
 *
 * 	Insert copyright notices
 * 	[1992/12/07  19:36:54  alan]
 *
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:02:18  bolinger]
 *
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:06:19  bolinger]
 *
 * 	Loading drop DCE1_0_2FNL
 * 	[1993/05/04  22:33:29  mgm  1.1.1.11]
 *
 * Revision 1.1.4.2  1994/06/09  13:37:34  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:27:32  devsrc]
 * 
 * Revision 1.1.4.1  1994/04/01  20:15:44  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:16:23  jd]
 * 
 * Revision 1.1.2.3  1992/12/15  22:26:39  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:36:54  alan]
 * 
 * Revision 1.1.2.2  1992/09/03  14:35:45  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:02:18  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/08  23:06:34  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:06:19  bolinger]
 * 
 * Revision 1.1  1992/01/19  14:38:55  devrcs
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
 *	CMA services
 *
 *  ABSTRACT:
 *
 *	Header file for per-thread context
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	26 July 1989
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	4 December 1989
 *		Include cma_tcb_defs.h instead of cma_tcb.h
 *	002	Paul Curtin	24 May 1991
 *		Added a prototype for cma__reinit_context.
 *	003	Dave Butenhof	12 March 1992
 *		Make most of the variables global so pthread interface can
 *		avoid extra call.
 */

#ifndef CMA_CONTEXT
#define CMA_CONTEXT

/*
 *  INCLUDE FILES
 */

#include <cma.h>
#include <cma_tcb_defs.h>
#include <cma_mutex.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * CONSTANTS AND MACROS
 */

#define cma___c_context_increment 100	/* Realloc arrays this much bigger */

/*
 * TYPEDEFS
 */

typedef cma_t_destructor	*cma__t_destructor_array;

/*
 *  GLOBAL DATA
 */

extern cma_t_natural	cma__g_context_next;	/* Next available key */
extern cma_t_integer	cma__g_context_size;	/* Maximum index of key array */
extern cma__t_int_mutex	*cma__g_context_mutex;	/* Mutex for key creation */

/*
 * INTERNAL INTERFACES
 */

extern void cma__init_context  (void);

extern void cma__reinit_context  (cma_t_integer);

extern void cma__run_down_context  (cma__t_int_tcb *);

#ifdef __cplusplus
}
#endif

#endif
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_CONTEXT.H */
/*  *6    13-MAR-1992 14:08:11 BUTENHOF "Make context mutex global for pthreads" */
/*  *5    10-JUN-1991 19:51:19 SCALES "Convert to stream format for ULTRIX build" */
/*  *4    10-JUN-1991 19:20:16 BUTENHOF "Fix the sccs headers" */
/*  *3    10-JUN-1991 18:18:20 SCALES "Add sccs headers for Ultrix" */
/*  *2    24-MAY-1991 16:45:06 CURTIN "Added a new reinit routine" */
/*  *1    12-DEC-1990 21:43:24 BUTENHOF "Per-thread context" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_CONTEXT.H */
