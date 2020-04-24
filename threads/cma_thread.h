/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: cma_thread.h $Revision: 1.1.8.1 $ $Date: 1996/05/10 13:43:13 $
 */
/*
 * HISTORY
 * $Log: cma_thread.h,v $
 * Revision 1.1.8.1  1996/05/10  13:43:13  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:46 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:45 UTC  jss  /main/HPDCE02/1]
 *
 * 	CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:43  devsrc]
 * 	 *
 *
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:19:08  jd]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/25  14:28  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/21  18:10:44  truitt]
 * 	 *
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:44:37  root  1.1.6.3]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:14:32  hopkins]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  19:03:05  root]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:35:37  hopkins]
 *
 * 	Insert copyright notices
 * 	[1992/12/07  19:42:58  alan]
 *
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:07:08  bolinger]
 *
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/09  00:05:56  bolinger]
 *
 * 	Loading drop DCE1_0_2FNL
 * 	[1993/05/04  22:35:57  mgm  1.1.1.11]
 *
 * Revision 1.1.4.2  1994/06/09  13:38:58  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:43  devsrc]
 * 
 * Revision 1.1.4.1  1994/04/01  20:17:33  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:19:08  jd]
 * 
 * Revision 1.1.2.3  1992/12/15  22:31:53  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:42:58  alan]
 * 
 * Revision 1.1.2.2  1992/09/03  14:41:24  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:07:08  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/09  00:06:09  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/09  00:05:56  bolinger]
 * 
 * Revision 1.1  1992/01/19  14:39:30  devrcs
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
 *	Header file for cma thread services
 *
 *  AUTHORS:
 *
 *	Webb Scales
 *
 *  CREATION DATE:
 *
 *	31 August 1989
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	4 December 1989
 *		Include cma_tcb_defs.h instead of cma_tcb.h
 *	002	Dave Butenhof	12 December 1989
 *		Add cma__thread_start routine.
 *	003	Dave Butenhof	05 April 1991
 *		Add cma__int_make_thread routine.
 *	004	Dave butenhof	02 July 1991
 *		Simplify prototype for cma__thread_base()... it'll get start
 *		routine and argument from the TCB.
 *	005	Dave Butenhof	11 February 1992
 *		Modify prototype for cma__int_make_thread
 */

#ifndef CMA_THREAD
#define CMA_THREAD

/*
 *  INCLUDE FILES
 */

#include <cma.h>
#include <cma_tcb_defs.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * CONSTANTS AND MACROS
 */

/*
 * TYPEDEFS
 */

/*
 *  GLOBAL DATA
 */

/*
 * INTERNAL INTERFACES
 */

extern void cma__cleanup_thread  (cma__t_int_tcb *,cma_t_address,cma_t_exit_status);

extern cma_t_boolean cma__int_make_thread  (cma__t_int_tcb *,cma_t_thread *,cma_t_start_routine,cma_t_address);

extern void cma__thread_base  (cma__t_int_tcb *);

#ifdef __cplusplus
}
#endif

#endif
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_THREAD.H */
/*  *8    13-MAR-1992 14:10:28 BUTENHOF "Remove excess thread_base arguments" */
/*  *7    18-FEB-1992 15:30:55 BUTENHOF "Add prototype" */
/*  *6     2-JUL-1991 16:53:08 BUTENHOF "Change prototype for thread_base" */
/*  *5    10-JUN-1991 19:57:24 SCALES "Convert to stream format for ULTRIX build" */
/*  *4    10-JUN-1991 19:22:06 BUTENHOF "Fix the sccs headers" */
/*  *3    10-JUN-1991 18:24:36 SCALES "Add sccs headers for Ultrix" */
/*  *2    12-APR-1991 23:37:12 BUTENHOF "Add internal function" */
/*  *1    12-DEC-1990 21:54:29 BUTENHOF "Thread management" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_THREAD.H */
