/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: cma_message.h $Revision: 1.1.8.1 $ $Date: 1996/05/10 13:39:04 $
 */
/*
 * HISTORY
 * $Log: cma_message.h,v $
 * Revision 1.1.8.1  1996/05/10  13:39:04  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:43 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:43 UTC  jss  /main/HPDCE02/1]
 *
 * 	CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:10  devsrc]
 * 	 *
 *
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:18:00  jd]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  16:20  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/21  13:12:11  truitt]
 * 	 *
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:43:43  root  1.1.6.3]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:12:54  hopkins]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  18:26:45  root]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:34:01  hopkins]
 *
 * 	Insert copyright notices
 * 	[1992/12/07  19:39:44  alan]
 *
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:04:48  bolinger]
 *
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:33:43  bolinger]
 *
 * 	Loading drop DCE1_0_2FNL
 * 	[1993/05/04  22:34:37  mgm  1.1.1.11]
 *
 * Revision 1.1.4.2  1994/06/09  13:38:17  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:10  devsrc]
 * 
 * Revision 1.1.4.1  1994/04/01  20:16:42  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:18:00  jd]
 * 
 * Revision 1.1.2.3  1992/12/15  22:28:56  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:39:44  alan]
 * 
 * Revision 1.1.2.2  1992/09/03  14:38:31  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:04:48  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/08  23:33:57  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:33:43  bolinger]
 * 
 * Revision 1.1  1992/01/19  14:39:13  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *	%W%	(ULTRIX/OSF)	%G%
 */
/*
 *  Copyright (c) 1991 by
 *  Digital Equipment Corporation, Maynard Massachusetts.
 *  All rights reserved.
 *
 * This software is furnished under a licence and may be used and
 * copied only in accordance of the terms of such license and
 * with the inclusion of the above copyright notice. This
 * software or any other copies thereof may be provided or
 * otherwise made available to any other person. No title to and
 * ownership of the software is herby transferred.
 *
 * The information in this software is subject to change without
 * notice and should not be construed as a commitment by DIGITAL
 * Equipment Corporation.
 *
 * DIGITAL assumes no responability for the use or reliability
 * of its software on equipment which is not supplied by DIGITAL.
 */

/*
 * FACILITY:
 *
 *	CMA services
 *
 * ABSTRACT:
 *
 *	Header for interface to the message catalog services.
 *
 * AUTHORS:
 *
 *	Dave Butenhof
 *
 * CREATION DATE:
 *
 *	28 May 1991
 *
 * MODIFICATION HISTORY:
 * 
 *	001	Paul Curtin	12 June 1991
 *		DECthreads integration work.
 *	002	Dave Butenhof	13 June 1991
 *		Add SCCS information, move to COMMON domain.
 *	003	Paul Curtin	20 August 1991
 *		Conditionalized out the inclusion of stdio.h on VMS.
 *	004	Dave Butenhof	20 November 1991
 *		Major rewrite: instead of using the old cma__message_*printf
 *		routines (we only called cma__message_fprintf anyway, and
 *		only from one place in exc_handling.c), replace with a single
 *		function, cma__error_inq_text, which returns the text of any
 *		dce message.
 */
#ifndef CMA_MESSAGE
#define CMA_MESSAGE

#ifdef __cplusplus
extern "C" {
#endif

/*
 * INCLUDE FILES
 */

/*
 * CONSTANTS AND MACROS
 */

/*
 * TYPEDEFS
 */

/*
 * GLOBAL DATA
 */

/*
 * INTERNAL INTERFACES
 */

extern void cma__error_inq_text (unsigned long,	unsigned char *,int *);

#ifdef __cplusplus
}
#endif

#endif
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_MESSAGE.H */
/*  *4    22-NOV-1991 11:56:42 BUTENHOF "Integrate dce message formatting" */
/*  *3    21-AUG-1991 16:43:22 CURTIN "Removed VMS include of stdio.h" */
/*  *2    13-JUN-1991 19:33:16 BUTENHOF "fix history" */
/*  *1    13-JUN-1991 19:29:14 BUTENHOF "Message catalog support" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_MESSAGE.H */
