/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: cma_sequence.h $Revision: 1.1.8.1 $ $Date: 1996/05/10 13:41:24 $
 */
/*
 * HISTORY
 * $Log: cma_sequence.h,v $
 * Revision 1.1.8.1  1996/05/10  13:41:24  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:45 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:44 UTC  jss  /main/HPDCE02/1]
 *
 * 	CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:30  devsrc]
 * 	 *
 *
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:18:41  jd]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  17:04  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/21  14:09:58  truitt]
 * 	 *
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:44:06  root  1.1.6.3]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:13:40  hopkins]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  18:42:22  root]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:34:44  hopkins]
 *
 * 	Insert copyright notices
 * 	[1992/12/07  19:41:13  alan]
 *
 * 	Loading drop DCE1_0_2FNL
 *
 * 	[1993/05/04  22:35:12  mgm  1.1.1.11]
 *
 * Revision 1.1.4.2  1994/06/09  13:38:44  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:30  devsrc]
 * 
 * Revision 1.1.4.1  1994/04/01  20:17:12  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:18:41  jd]
 * 
 * Revision 1.1.2.2  1992/12/15  22:30:30  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:41:13  alan]
 * 
 * Revision 1.1  1992/01/19  14:39:25  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  Copyright (c) 1989, 1991 by
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
 *	Header file for sequence generator functions
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	24 July 1989
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	25 August 1989
 *		Add initialization code for convenience.
 *	002	Dave Butenhof	11 October 1989
 *		Convert to use internal mutex operations.
 *	003	Dave Butenhof	1 November 1989
 *		Remove circular header dependency with mutex.
 *	004	Dave Butenhof and Webb Scales	05 June 1991
 *		Conditionalize vacuous (forward) structure defs, since MIPS C
 *		V2.1 doesn't like (or, apparently, need).
 */

#ifndef CMA_SEQUENCE
#define CMA_SEQUENCE

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

#ifndef __STDC__
struct CMA__T_INT_MUTEX;
#endif

typedef struct CMA__T_SEQUENCE {
    struct CMA__T_INT_MUTEX	*mutex;	/* Serialize access to counter */
    cma_t_natural		seq;	/* Sequence number for object */
    } cma__t_sequence;

/*
 *  GLOBAL DATA
 */

/*
 * INTERNAL INTERFACES
 */

extern cma_t_natural cma__assign_sequence (cma__t_sequence *);

extern void cma__init_sequence (cma__t_sequence *);	

#ifdef __cplusplus
}
#endif

#endif
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_SEQUENCE.H */
/*  *5    10-JUN-1991 19:56:01 SCALES "Convert to stream format for ULTRIX build" */
/*  *4    10-JUN-1991 19:21:34 BUTENHOF "Fix the sccs headers" */
/*  *3    10-JUN-1991 18:23:27 SCALES "Add sccs headers for Ultrix" */
/*  *2     5-JUN-1991 17:31:33 BUTENHOF "Conditionalize vacuous defs" */
/*  *1    12-DEC-1990 21:52:41 BUTENHOF "Atomic sequence numbers" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_SEQUENCE.H */
