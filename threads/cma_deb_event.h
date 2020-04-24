/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: cma_deb_event.h $Revision: 1.1.8.1 $ $Date: 1996/05/10 13:35:00 $
 */
/*
 * HISTORY
 * $Log: cma_deb_event.h,v $
 * Revision 1.1.8.1  1996/05/10  13:35:00  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:39 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:42 UTC  jss  /main/HPDCE02/1]
 *
 * 	CR10892 - fix copyright in file
 * 	[1994/06/09  13:27:39  devsrc]
 * 	 *
 *
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:16:49  jd]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  15:11  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/20  19:26:41  truitt]
 * 	 *
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:43:12  root  1.1.6.3]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:11:40  hopkins]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  17:58:32  root]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:32:43  hopkins]
 *
 * 	Insert copyright notices
 * 	[1992/12/07  19:37:23  alan]
 *
 * 	Loading drop DCE1_0_2FNL
 *
 * 	[1993/05/04  22:33:39  mgm  1.1.1.11]
 *
 * Revision 1.1.4.2  1994/06/09  13:37:42  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:27:39  devsrc]
 * 
 * Revision 1.1.4.1  1994/04/01  20:15:55  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:16:49  jd]
 * 
 * Revision 1.1.2.2  1992/12/15  22:27:11  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:37:23  alan]
 * 
 * Revision 1.1  1992/01/19  14:39:00  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  Copyright (c) 1990 by
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
 *	Header file for debugging events
 *
 *  AUTHORS:
 *
 *	Bob Conti
 *
 *  CREATION DATE:
 *
 *	9 September 1990
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Bob Conti	9 September 1990
 *		Create module
 */

#ifndef CMA_DEB_EVENT
#define CMA_DEB_EVENT

/*
 *  INCLUDE FILES
 */
#include <cma.h>
#include <cma_tcb_defs.h>	/* Contains the debug event definitions */

#ifdef __cplusplus
extern "C" {
#endif

/*
 * CONSTANTS AND MACROS
 */

/* Compile time flag to enable the event reporting code */
/* 
 * FIX-ME: This should come from the compile qualifier eventually so
 * that the dispatcher can be compiled two ways: with and without 
 * the event reporting code.
 */
#define _CMA_DEBEVT_	1

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
 * Routines to report that a CMA debugging-event has occurred 
 */
extern void cma__debevt_report  (cma__t_debevt);

extern void cma__debevt_report_2  (cma__t_debevt,cma_t_address,cma_t_address);

/* 
 * Routine to notify debugger that a thread it requested is about to run
 */
extern void cma__debevt_notify  (void);

/* 
 * Routine to enable a CMA debugging-event 
 */
extern void cma__debevt_set  (cma__t_int_tcb *,cma__t_debevt);

/* 
 * Routine to disable a CMA debugging-event 
 */
extern void cma__debevt_clear  (cma__t_int_tcb *,cma__t_debevt);

/* 
 * Routine to test a CMA debugging-event 
 */
extern cma_t_boolean cma__debevt_test     (cma__t_int_tcb *,cma__t_debevt);

#ifdef __cplusplus
}
#endif

#endif


/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_DEB_EVENT.H */
/*  *4    10-JUN-1991 19:52:13 SCALES "Convert to stream format for ULTRIX build" */
/*  *3    10-JUN-1991 19:20:29 BUTENHOF "Fix the sccs headers" */
/*  *2    10-JUN-1991 18:20:10 SCALES "Add sccs headers for Ultrix" */
/*  *1    12-DEC-1990 21:44:00 BUTENHOF "Debug support" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_DEB_EVENT.H */
