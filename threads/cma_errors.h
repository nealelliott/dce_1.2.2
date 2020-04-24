/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: cma_errors.h $Revision: 1.1.11.1 $ $Date: 1996/05/10 13:37:01 $
 */
/*
 * HISTORY
 * $Log: cma_errors.h,v $
 * Revision 1.1.11.1  1996/05/10  13:37:01  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:41 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:42 UTC  jss  /main/HPDCE02/1]
 *
 * 	CR10892 - fix copyright in file
 * 	[1994/06/09  13:27:58  devsrc]
 * 	 *
 *
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:17:33  jd]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  15:53  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/21  12:42:32  truitt]
 * 	 *
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:43:32  root  1.1.6.3]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:12:19  hopkins]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  18:15:19  root]
 *
 * 	Insert copyright notices
 * 	[1992/12/07  19:38:42  alan]
 *
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:03:24  bolinger]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:33:24  hopkins]
 *
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:21:56  bolinger]
 *
 * 	The bugcheck mechanism no longer raises exceptions; rather, a core
 * 	file is produced.  In addition, a log file of useful state information
 * 	is created before the process terminates.
 * 	[1992/05/11  20:26:50  keane]
 *
 * 	Loading drop DCE1_0_2FNL
 * 	[1993/05/04  22:34:08  mgm  1.1.1.11]
 *
 * Revision 1.1.7.2  1994/06/09  13:38:01  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:27:58  devsrc]
 * 
 * Revision 1.1.7.1  1994/04/01  20:16:22  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:17:33  jd]
 * 
 * Revision 1.1.4.3  1992/12/15  22:28:13  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:38:42  alan]
 * 
 * Revision 1.1.4.2  1992/09/03  14:37:28  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:03:24  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/08  23:22:14  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:21:56  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/14  19:49:26  keane
 * 	The bugcheck mechanism no longer raises exceptions; rather, a core
 * 	file is produced.  In addition, a log file of useful state information
 * 	is created before the process terminates.
 * 	[1992/05/11  20:26:50  keane]
 * 
 * Revision 1.1  1992/01/19  14:39:06  devrcs
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
 *	This module is the interface between CMA services and 
 * 	the platform-specific error reporting mechanism.
 *
 *  AUTHORS:
 *
 *	Bob Conti
 *
 *  CREATION DATE:
 *
 *	6 October 1989
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Webb Scales	17 October 1989
 *		Corrected case of #include filenames
 *	002	Bob Conti	10 December 1989
 *		Move cma__unimplemented here from cma_exception.c
 *	003	Dave Butenhof	14 December 1989
 *		Add cma__report_error to output messages for exceptions.
 *	004	Dave Butenhof & Bob Conti	15 December 1989
 *		Remove cma__report_error to exception package & rename to
 *		cma__exc_report.
 *	004	Dave Butenhof	01 May 1991
 *		Add string argument to cma__bugcheck() (written out before
 *		raising exception).
 *	005	Dave Butenhof	24 March 1992
 *		Enhance cma__bugcheck() prototype to take printf argument
 *		list.
 */

#ifndef CMA_ERRORS
#define CMA_ERRORS

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

/*
 * The cma__bugcheck function will print information to stderr (or sys$error
 * on VMS), and more extensive information to the file cma_dump.log in the
 * current working directory.
 */
extern void cma__bugcheck  (char *,...);

extern void cma__error  (int);

extern void cma__unimplemented (void);

#ifdef __cplusplus
}
#endif

#endif
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_ERRORS.H */
/*  *7    24-MAR-1992 13:46:31 BUTENHOF "Put bugcheck output in file" */
/*  *6    22-NOV-1991 11:56:05 BUTENHOF "Remove unnecessary includes" */
/*  *5    10-JUN-1991 19:53:08 SCALES "Convert to stream format for ULTRIX build" */
/*  *4    10-JUN-1991 19:20:47 BUTENHOF "Fix the sccs headers" */
/*  *3    10-JUN-1991 18:21:47 SCALES "Add sccs headers for Ultrix" */
/*  *2     2-MAY-1991 13:58:18 BUTENHOF "Add string argument to cma__bugcheck()" */
/*  *1    12-DEC-1990 21:45:29 BUTENHOF "Error handling" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_ERRORS.H */
