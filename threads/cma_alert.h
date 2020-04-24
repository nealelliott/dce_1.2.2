/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: cma_alert.h $Revision: 1.1.8.1 $ $Date: 1996/05/10 13:53:08 $
 */
/*
 * HISTORY
 * $Log: cma_alert.h,v $
 * Revision 1.1.8.1  1996/05/10  13:53:08  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:38 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:41 UTC  jss  /main/HPDCE02/1]
 *
 * 	CR10892 - fix copyright in file
 * 	[1994/06/09  13:27:18  devsrc]
 * 	 *
 *
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:15:41  jd]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  13:41  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/20  15:06:23  truitt]
 * 	 *
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:42:53  root  1.1.6.3]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:10:46  hopkins]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  17:47:05  root]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:31:42  hopkins]
 *
 * 	Insert copyright notices
 * 	[1992/12/07  19:35:46  alan]
 *
 * 	Loading drop DCE1_0_2FNL
 *
 * 	[1993/05/04  22:33:06  mgm  1.1.1.11]
 *
 * Revision 1.1.4.2  1994/06/09  13:37:16  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:27:18  devsrc]
 * 
 * Revision 1.1.4.1  1994/04/01  20:15:21  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:15:41  jd]
 * 
 * Revision 1.1.2.2  1992/12/15  22:25:52  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:35:46  alan]
 * 
 * Revision 1.1  1992/01/19  14:38:47  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
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
 *	Header file for ALERT functions
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	14 September 1989
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	27 October 1989
 *		Make cma___attempt_delivery externally visible.
 *	002	Dave Butenhof	4 December 1989
 *		Include cma_tcb_defs.h instead of cma_tcb.h
 *	003	Dave Butenhof	15 August 1990
 *		Add new function cma__asynch_delivery, used by assembler code
 *		to call cma__attempt_delivery with appropriate TCB (since
 *		the assembler code doesn't have access to the macros
 *		specifying how to get the current TCB).
 *	004	Paul Curtin	 8 May 1991
 *		Added the cma__int_alert_test macro
 */

#ifndef CMA_ALERT
#define CMA_ALERT

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
 * TYPEDEFS
 */

/*
 *  GLOBAL DATA
 */

/*
 * MACROS
 */

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *  cma__int_alert_test - Performs work for cma_alert_test
 *
 *  FORMAL PARAMETERS:
 *
 *	none
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	none
 */

#define cma__int_alert_test() { \
    cma__t_int_tcb      *_tcb_; \
    _tcb_ = cma__get_self_tcb (); \
    cma__attempt_delivery (_tcb_); \
    }


/*
 * INTERNAL INTERFACES
 */

extern void cma__async_delivery (void);

extern void cma__attempt_delivery (cma__t_int_tcb *);

#ifdef __cplusplus
}
#endif

#endif
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_ALERT.H */
/*  *6    10-JUN-1991 19:50:09 SCALES "Convert to stream format for ULTRIX build" */
/*  *5    10-JUN-1991 19:19:54 BUTENHOF "Fix the sccs headers" */
/*  *4    10-JUN-1991 18:16:37 SCALES "Add sccs headers for Ultrix" */
/*  *3    10-MAY-1991 11:09:22 CURTIN "added cma__int_alert_test macro" */
/*  *2    12-FEB-1991 01:28:35 BUTENHOF "New alert control primitives" */
/*  *1    12-DEC-1990 21:40:34 BUTENHOF "alerts" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_ALERT.H */
