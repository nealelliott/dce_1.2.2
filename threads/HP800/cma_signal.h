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
 * @(#)Module: cma_signal.h $Revision: 1.1.6.1 $ $Date: 1996/05/10 13:28:08 $
 */
/*
 * HISTORY
 * $Log: cma_signal.h,v $
 * Revision 1.1.6.1  1996/05/10  13:28:08  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:50 UTC  bissen
 * 	Merge from branch jss_single_thread
 * 	[1995/01/03  22:36 UTC  jss  /main/HPDCE02/2]
 *
 * 	Changes for single threaded client. Added routine cma__hp_preinit_signal().
 * 	[1995/01/03  19:16 UTC  jss  /main/HPDCE02/jss_single_thread/1]
 *
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:48 UTC  jss  /main/HPDCE02/1]
 *
 * 	CR10892 - fix copyright in file
 * 	[1994/06/09  13:25:32  devsrc]
 * 	 *
 *
 * 	Moved from src/threads to here
 * 	[1994/04/01  17:02:50  jd]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  18:05  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/21  14:38:54  truitt]
 * 	 *
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:44:12  root  1.1.6.3]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:13:46  hopkins]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  18:45:20  root]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:34:50  hopkins]
 *
 * 	Insert copyright notices
 * 	[1992/12/07  19:41:27  alan]
 *
 * 	Fixed a typo. cma_reinitsignal was spelled cma_reinit_sigal in its
 * 	prototype resulting in compiler warnings.
 * 	[1992/10/28  21:30:28  jd]
 *
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:51:20  sekhar]
 *
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:06:05  bolinger]
 *
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:51:39  bolinger]
 *
 * 	Loading drop DCE1_0_2FNL
 * 	[1993/05/04  22:35:18  mgm  1.1.1.11]
 *
 * Revision 1.1.2.2  1994/06/09  13:34:39  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:25:32  devsrc]
 * 
 * Revision 1.1.2.1  1994/04/01  20:13:42  jd
 * 	Moved from src/threads to here
 * 	[1994/04/01  17:02:50  jd]
 * 
 * Revision 1.1.2.5  1992/12/15  22:30:44  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:41:27  alan]
 * 
 * Revision 1.1.2.4  1992/10/28  21:30:57  jd
 * 	Fixed a typo. cma_reinitsignal was spelled cma_reinit_sigal in its
 * 	prototype resulting in compiler warnings.
 * 	[1992/10/28  21:30:28  jd]
 * 
 * Revision 1.1.2.3  1992/09/29  20:15:31  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:51:20  sekhar]
 * 
 * Revision 1.1.2.2  1992/09/03  14:40:02  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:06:05  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/08  23:51:52  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:51:39  bolinger]
 * 
 * Revision 1.1  1992/01/19  14:39:26  devrcs
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
 *	Header file for signal operations
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	23 May 1991
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Paul Curtin	3 June 1991
 *		Added a prototype for new reinit routine.
 *	002	Webb Scales	7 April 1991
 *		Set up for building on VMS.
 */

#ifndef CMA_SIGNAL
#define CMA_SIGNAL

/*
 *  INCLUDE FILES
 */

#include <cma_tcb_defs.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * CONSTANTS AND MACROS
 */

/*
 *  TYPE DEFINITIONS
 */

/*
 *  GLOBAL DATA
 */

/*
 * EXTERNAL INTERFACES
 */

extern void cma__abort_process (cma_t_integer);

extern int
cma_sigaction (int,struct sigaction *,struct sigaction *);

extern void cma__init_signal  (void);

extern void cma__reinit_signal  (cma_t_integer);

extern void cma__preinit_signal  (void);

extern cma_t_boolean

cma__sig_deliver  (int,int,struct sigcontext *);

#ifdef __cplusplus
}
#endif

#endif

