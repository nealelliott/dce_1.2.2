/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: exc_handling_vms.h,v $
 * Revision 1.1.6.2  1996/02/18  23:09:18  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:33:35  marty]
 *
 * Revision 1.1.6.1  1995/12/07  21:58:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:04:21  root]
 * 
 * Revision 1.1.4.1  1994/06/09  13:37:12  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:27:13  devsrc]
 * 
 * Revision 1.1.2.2  1992/12/15  22:25:27  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:34:03  alan]
 * 
 * Revision 1.1  1992/01/19  14:39:35  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  Copyright (c) 1990, 1991 by
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
 *	Header file for VAX/VMS exception package host-specific functions
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	19 April 1990
 *
 *  MODIFICATION HISTORY:
 * 
 *	001	Dave Butenhof	25 March 1991
 *		Change name to exc_fetch_fp()
 *	002	Dave Butenhof, Webb Scales	30 July 1991
 *		Change type of exc_fetch_fp() to exc_t_address
 *	003	Dave Butenhof	29 October 1991
 *		Change FP fetcher to cma$exc_fetch_fp() to be compatible with
 *		direct use of cma$rtl entry rather than cma$open_rtl entry.
 */


/*
 *  INCLUDE FILES
 */

/*
 * CONSTANTS AND MACROS
 */

#pragma builtins

/*
 * Return the present value of the frame pointer for the current thread
 *
 * _READ_GPR reads the value of a general processor register
 *  (register 13 is FP)
 */
#define cma$exc_fetch_fp() ((exc_t_address)_READ_GPR (13))

/*
 *  GLOBAL DATA
 */

/*
 * INTERNAL INTERFACES
 */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_EXCEPTION_VMS.H */
/*  *1    12-DEC-1990 21:24:47 BUTENHOF "VMS specific exception defs" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_EXCEPTION_VMS.H */
/*  DEC/CMS REPLACEMENT HISTORY, Element EXC_HANDLING_VMS.H */
/*  *3    31-OCT-1991 12:37:02 BUTENHOF "Change name of fetch_fp reference" */
/*  *2    31-JUL-1991 18:39:34 BUTENHOF "Change type of exc_fetch_fp()" */
/*  *1     1-APR-1991 18:52:51 BUTENHOF "VMS specific exception defs" */
/*  DEC/CMS REPLACEMENT HISTORY, Element EXC_HANDLING_VMS.H */
