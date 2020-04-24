/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: cma_vp.h $Revision: 1.1.8.1 $ $Date: 1996/05/10 13:44:44 $
 */
/*
 * HISTORY
 * $Log: cma_vp.h,v $
 * Revision 1.1.8.1  1996/05/10  13:44:44  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:47 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:46 UTC  jss  /main/HPDCE02/1]
 *
 * 	CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:58  devsrc]
 * 	 *
 *
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:19:38  jd]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/25  15:18  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/21  15:46:41  truitt]
 * 	 *
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:45:03  root  1.1.6.3]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:15:14  hopkins]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  19:19:10  root]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:36:18  hopkins]
 *
 * 	Insert copyright notices
 * 	[1992/12/07  19:44:25  alan]
 *
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:08:08  bolinger]
 *
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/09  00:16:15  bolinger]
 *
 * 	Loading drop DCE1_0_2FNL
 * 	[1993/05/04  22:36:31  mgm  1.1.1.11]
 *
 * Revision 1.1.4.2  1994/06/09  13:39:12  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:58  devsrc]
 * 
 * Revision 1.1.4.1  1994/04/01  20:17:54  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:19:38  jd]
 * 
 * Revision 1.1.2.3  1992/12/15  22:32:54  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:44:25  alan]
 * 
 * Revision 1.1.2.2  1992/09/03  14:42:41  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:08:08  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/09  00:16:29  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/09  00:16:15  bolinger]
 * 
 * Revision 1.1  1992/01/19  14:39:43  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *	%W%	(DEC OSF/1)	%G%
 */
/*
 *  Copyright (c) 1991, 1992 by
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
 *  and  should  not  be  construed  as  a commitment by Digital Equipment
 *  Corporation.
 *
 *  Digital assumes no responsibility for the use or  reliability  of  its
 *  software on equipment which is not supplied by Digital.
 */

/*
 *  FACILITY:
 *
 *	DECthreads services
 *
 *  ABSTRACT:
 *
 *	Header file for virtual processor services
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	03 April 1991
 * 
 *  MODIFICATION HISTORY:
 * 
 *	001	Dave Butenhof	25 April 1991
 *		Add "vp_lock" and "vp_unlock" functions to emulate atomic
 *		test-and-set. (later removed!!!)
 *	002	Dave Butenhof	31 May 1991
 *		Add cma__vp_delay() function to do VP-synchronous delay
 *		without relying on timer interrupts.
 *	003	Dave Butenhof	05 September 1991
 *		Add prototype for new function cma__vp_yield() to use the
 *		Mach "sched_yield" syscall.
 *	004	Dave Butenhof	07 February 1992
 *		Add cma__vp_dump() function to list all active and cached VPs
 *		for debugging.
 */

#ifndef CMA_VP
#define CMA_VP

/*
 *  INCLUDE FILES
 */

#include <cma_vp_defs.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  GLOBAL DATA
 */

/*
 * INTERNAL INTERFACES
 */

extern void cma__init_vp  (void);

extern void
cma__reinit_vp (cma_t_integer);

extern cma__t_vp_status cma__vp_create  (cma__t_vpid *);

extern void cma__vp_delete  (cma__t_vpid);

extern cma__t_vpid cma__vp_get_id  (void);

extern cma__t_vp_status cma__vp_get_state  (cma__t_vpid,cma__t_vp_state	*);

extern void cma__vp_dump  (void);

extern cma__t_vp_status cma__vp_interrupt  (cma__t_vpid,cma__t_vp_handler,cma_t_address);

extern cma__t_vp_status cma__vp_resume  (cma__t_vpid);

extern cma__t_vp_status cma__vp_resume_others  (void);

extern cma__t_vp_status cma__vp_set_start  (cma__t_vpid,cma__t_vp_state *);

extern cma__t_vp_status cma__vp_suspend  (cma__t_vpid,cma_t_integer);

extern cma__t_vp_status cma__vp_suspend_others  (void);

extern void cma__vp_yield  (void);

#ifdef __cplusplus
}
#endif

#endif

