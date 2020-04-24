/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: cma_once.h $Revision: 1.1.8.1 $ $Date: 1996/05/10 13:39:39 $
 */
/*
 * HISTORY
 * $Log: cma_once.h,v $
 * Revision 1.1.8.1  1996/05/10  13:39:39  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:43 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:43 UTC  jss  /main/HPDCE02/1]
 *
 * 	CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:15  devsrc]
 * 	 *
 *
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:18:12  jd]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  16:46  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/21  13:23:01  truitt]
 * 	 *
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:43:50  root  1.1.6.3]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:13:05  hopkins]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  18:31:43  root]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:34:13  hopkins]
 *
 * 	Insert copyright notices
 * 	[1992/12/07  19:40:08  alan]
 *
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:05:13  bolinger]
 *
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:36:53  bolinger]
 *
 * 	Loading drop DCE1_0_2FNL
 * 	[1993/05/04  22:34:48  mgm  1.1.1.11]
 *
 * Revision 1.1.4.2  1994/06/09  13:38:22  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:15  devsrc]
 * 
 * Revision 1.1.4.1  1994/04/01  20:16:52  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:18:12  jd]
 * 
 * Revision 1.1.2.3  1992/12/15  22:29:12  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:40:08  alan]
 * 
 * Revision 1.1.2.2  1992/09/03  14:38:56  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:05:13  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/08  23:37:08  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:36:53  bolinger]
 * 
 * Revision 1.1  1992/01/19  14:39:17  devrcs
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
 *	Concert Multithread Architecture (CMA) services
 *
 *  ABSTRACT:
 *
 *	Header file for cma_once.c
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	19 July 1989
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	12 October 1989
 *		Convert to use internal mutex operations.
 *	002	Dave Butenhof	27 August 1990
 *		Change flag enum to begin at 0 rather than 1, to make static
 *		initialization easier (for languages without type-based init
 *		or macros).
 *	003	Paul Curtin	21 May 1991
 *		Added cma__g_once_mutexes: new queue header
 *	004	Paul Curtin	24 May 1991
 *		Added a prototype for cma__reinit_once.
 */

#ifndef CMA_ONCE
#define CMA_ONCE

/*
 *  INCLUDE FILES
 */

#include <cma_mutex.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * CONSTANTS AND MACROS
 */

/*
 * TYPEDEFS
 */

typedef enum CMA__T_ONCE_FLAG {
    cma__c_once_uninit = 0,
    cma__c_once_initing = 1,
    cma__c_once_inited = 2
    } cma__t_once_flag;

typedef struct CMA__T_ONCE_MUTEX {
    cma__t_queue	queue;
    cma__t_int_mutex	*mutex;
    } cma__t_once_mutex;

typedef struct CMA__T_INT_ONCE {
    cma_t_integer	mbz;
    cma__t_int_mutex	*mutex;
    cma__t_once_flag	flag;
    } cma__t_int_once;

/*
 *  GLOBAL DATA
 */

extern cma__t_queue	    cma__g_once_mutexes;

/*
 * INTERNAL INTERFACES
 */

extern void cma__init_once (void);

extern void cma__reinit_once (cma_t_integer);

#ifdef __cplusplus
}
#endif

#endif
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_ONCE.H */
/*  *5    10-JUN-1991 19:54:43 SCALES "Convert to stream format for ULTRIX build" */
/*  *4    10-JUN-1991 19:21:13 BUTENHOF "Fix the sccs headers" */
/*  *3    10-JUN-1991 18:22:38 SCALES "Add sccs headers for Ultrix" */
/*  *2     5-JUN-1991 16:13:55 CURTIN "fork work" */
/*  *1    12-DEC-1990 21:47:51 BUTENHOF "Client one-time init" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_ONCE.H */
