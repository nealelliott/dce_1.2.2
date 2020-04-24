/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: cma_cond_defs.h $Revision: 1.1.8.1 $ $Date: 1996/05/10 13:34:03 $
 */
/*
 * HISTORY
 * $Log: cma_cond_defs.h,v $
 * Revision 1.1.8.1  1996/05/10  13:34:03  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:38 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:41 UTC  jss  /main/HPDCE02/1]
 *
 * 	CR10892 - fix copyright in file
 * 	[1994/06/09  13:27:25  devsrc]
 * 	 *
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  14:30  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/20  19:06:28  truitt]
 * 	 *
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:43:00  root  1.1.6.3]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:11:09  hopkins]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  17:52:06  root]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:32:04  hopkins]
 *
 * 	Insert copyright notices
 * 	[1992/12/07  19:36:22  alan]
 *
 * 	Move file from nosupport/threads_bl10 area into threads area.
 * 	[1992/09/03  15:04:01  bolinger]
 *
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  22:51:13  bolinger]
 *
 * 	Loading drop DCE1_0_2FNL
 * 	[1993/05/04  22:33:18  mgm  1.1.1.5]
 *
 * Revision 1.1.4.1  1994/06/09  13:37:25  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:27:25  devsrc]
 * 
 * Revision 1.1.2.3  1992/12/15  22:26:16  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:36:22  alan]
 * 
 * Revision 1.1.2.2  1992/09/03  15:05:47  bolinger
 * 	Move file from nosupport/threads_bl10 area into threads area.
 * 	[1992/09/03  15:04:01  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/08  22:51:27  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  22:51:13  bolinger]
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
 *	Header file for mutex operations
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	22 November 1991
 *
 *  MODIFICATION HISTORY:
 *
 */

#ifndef CMA_COND_DEFS
#define CMA_COND_DEFS

/*
 *  INCLUDE FILES
 */

#include <cma.h>
#include <cma_attr.h>
#include <cma_defs.h>
#include <cma_semaphore_defs.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * CONSTANTS AND MACROS
 */

/*
 * TYPEDEFS
 */

typedef struct CMA__T_INT_CV {
    cma__t_object	header;		/* Common header (sequence, type) */
    cma__t_int_attr	*attributes;	/* Back link */
    cma__t_atomic_bit	event;		/* Clear when may be waiters */
    cma__t_semaphore	semaphore;	/* Semaphore for condition wait */
    } cma__t_int_cv;

/*
 *  GLOBAL DATA
 */

/*
 * INTERNAL INTERFACES
 */

#ifdef __cplusplus
}
#endif

#endif
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_COND_DEFS.H */
/*  *1    22-NOV-1991 13:27:47 BUTENHOF "Condition variable struct" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_COND_DEFS.H */
