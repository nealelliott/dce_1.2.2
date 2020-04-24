/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cma_test.h,v $
 * Revision 1.1.6.2  1996/02/18  23:08:06  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:30:38  marty]
 *
 * Revision 1.1.6.1  1995/12/11  22:57:11  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:34:38  root]
 * 
 * Revision 1.1.4.1  1994/04/01  20:08:28  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:11:27  jd]
 * 
 * Revision 1.1.2.2  1992/12/30  16:27:35  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:04:02  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:19:31  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*
 *  FACILITY:
 *
 *      CMA
 *
 *  ABSTRACT:
 *
 *      This header file defines the interfaces to the CMA testing facility
 *	routines.
 *
 *  AUTHORS:
 *
 *      Webb Scales
 *
 *
 *  CREATION DATE:      31 July 1989
 *
 *  MODIFICATION HISTORY:
 *
 */

#include <dce/cma.h>

/*
 *  T_CMA_PROTOTYPE_his routine is called at the beginning of a test to initialize the CMA
 *  testing system and log the start of a test.
 */_
extern void cma__test  (char *,char *);

/*
 *  This routine may be called at any time during a test to cause the
 *  specified string to be printed to the test log (stdout).
 *
 */
extern void cma__comment  (char *);

/*
 *  This routine may be called at any time during a test to indicate that
 *  the current test was failed and to print the reason for failure into the
 *  test log (stdout).
 */
extern void
 cma__failed  (char *);

/*
 *  This routine must be called at the end of a test to indicate that
 *  the test is complete.  If no failures were encountered during the test,
 *  a "passed" messages is placed in the test log (stdout).
 */
extern void
cma__result ();
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_TEST.H */
/*  *3    21-JUN-1990 08:21:57 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *2     7-JUN-1990 11:33:11 SCALES "Updates to CTS" */
/*  *1    17-MAY-1990 13:56:44 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_TEST.H */
