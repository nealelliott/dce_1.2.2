/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: cma_debugger.h $Revision: 1.1.8.1 $ $Date: 1996/05/10 13:35:22 $
 */
/*
 * HISTORY
 * $Log: cma_debugger.h,v $
 * Revision 1.1.8.1  1996/05/10  13:35:22  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:40 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:42 UTC  jss  /main/HPDCE02/1]
 *
 * 	CR10892 - fix copyright in file
 * 	[1994/06/09  13:27:43  devsrc]
 * 	 *
 *
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:17:02  jd]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  15:14  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/20  19:31:52  truitt]
 * 	 *
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:43:16  root  1.1.6.3]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:11:51  hopkins]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  18:02:11  root]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:32:55  hopkins]
 *
 * 	Insert copyright notices
 * 	[1992/12/07  19:37:43  alan]
 *
 * 	Loading drop DCE1_0_2FNL
 *
 * 	[1993/05/04  22:33:48  mgm  1.1.1.11]
 *
 * Revision 1.1.4.2  1994/06/09  13:37:46  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:27:43  devsrc]
 * 
 * Revision 1.1.4.1  1994/04/01  20:16:02  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:17:02  jd]
 * 
 * Revision 1.1.2.2  1992/12/15  22:27:36  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:37:43  alan]
 * 
 * Revision 1.1  1992/01/19  14:39:04  devrcs
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
 *	Concert Multithread Architecture (CMA) services
 *
 *  ABSTRACT:
 *
 *	Debug structures
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	9 April 1990
 *
 *  MODIFICATION HISTORY:
 *
 */

#ifndef CMA_DEBUGGER
#define CMA_DEBUGGER

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  INCLUDE FILES
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

#ifdef __cplusplus
}
#endif

/*
 * LOCAL FUNCTIONS
 */

extern void cma__debug_format_thread  (cma__t_int_tcb *,cma_t_integer);

extern void cma__debug_help  (int,char **);

extern void cma__debug_list_atts  (int,char **);

extern void cma__debug_list_cvs  (int,char **);

extern void cma__debug_list_mutexes  (int,char **);

extern void cma__debug_list_threads  (int,char **);

extern cma_t_boolean cma__debug_parse  (char *,cma_t_boolean);

extern void cma__debug_show  (int,char **);

extern void cma__debug_set_thread  (int,char **);

extern cma_t_boolean cma__debug_trylock  (void);

extern void cma__debug_vp  (int,char **);

#endif
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_DEBUGGER.H */
/*  *4    10-JUN-1991 19:51:33 SCALES "Convert to stream format for ULTRIX build" */
/*  *3    10-JUN-1991 19:20:19 BUTENHOF "Fix the sccs headers" */
/*  *2    10-JUN-1991 18:19:08 SCALES "Add sccs headers for Ultrix" */
/*  *1    12-DEC-1990 21:43:48 BUTENHOF "Debug support" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_DEBUGGER.H */
