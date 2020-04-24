/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: cma_vmprot.h $Revision: 1.1.8.1 $ $Date: 1996/05/10 13:44:32 $
 */
/*
 * HISTORY
 * $Log: cma_vmprot.h,v $
 * Revision 1.1.8.1  1996/05/10  13:44:32  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:47 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:45 UTC  jss  /main/HPDCE02/1]
 *
 * 	CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:55  devsrc]
 * 	 *
 *
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:19:32  jd]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/25  15:17  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/21  15:49:04  truitt]
 * 	 *
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:45:01  root  1.1.6.3]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:15:08  hopkins]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  19:17:42  root]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:36:12  hopkins]
 *
 * 	Insert copyright notices
 * 	[1992/12/07  19:44:13  alan]
 *
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:07:57  bolinger]
 *
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/09  00:14:38  bolinger]
 *
 * 	Loading drop DCE1_0_2FNL
 * 	[1993/05/04  22:36:27  mgm  1.1.1.11]
 *
 * Revision 1.1.4.2  1994/06/09  13:39:10  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:55  devsrc]
 * 
 * Revision 1.1.4.1  1994/04/01  20:17:50  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:19:32  jd]
 * 
 * Revision 1.1.2.3  1992/12/15  22:32:46  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:44:13  alan]
 * 
 * Revision 1.1.2.2  1992/09/03  14:42:28  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:07:57  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/09  00:14:51  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/09  00:14:38  bolinger]
 * 
 * Revision 1.1  1992/01/19  14:39:40  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *	%W%	(ULTRIX/OSF)	%G%
 */
/*
 *  Copyright (c) 1989, 1990 by
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
 *	Header file for VM protection services
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	23 August 1989
 *
 *  MODIFICATION HISTORY:
 *
 */

#ifndef CMA_VMPROT
#define CMA_VMPROT

/*
 *  INCLUDE FILES
 */

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

extern void cma__set_noaccess  (cma_t_address,cma_t_address);

extern void cma__set_writable  (cma_t_address,cma_t_address);

#ifdef __cplusplus
}
#endif

#endif
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_VMPROT.H */
/*  *4    10-JUN-1991 19:58:24 SCALES "Convert to stream format for ULTRIX build" */
/*  *3    10-JUN-1991 19:22:28 BUTENHOF "Fix the sccs headers" */
/*  *2    10-JUN-1991 18:25:20 SCALES "Add sccs headers for Ultrix" */
/*  *1    12-DEC-1990 21:56:01 BUTENHOF "VM management" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_VMPROT.H */
