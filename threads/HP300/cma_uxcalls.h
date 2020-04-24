/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cma_uxcalls.h,v $
 * Revision 1.1.6.2  1996/02/18  23:08:23  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:31:28  marty]
 *
 * Revision 1.1.6.1  1995/12/07  21:45:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  20:59:09  root]
 * 
 * Revision 1.1.4.1  1994/06/09  13:34:16  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:25:19  devsrc]
 * 
 * Revision 1.1.2.2  1992/12/15  22:20:49  alan
 * 	Insert copyright notices
 * 	[1992/12/07  15:59:15  alan]
 * 
 * Revision 1.1  1992/01/19  14:40:01  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#ifndef CMA_UXCALLS
#define CMA_UXCALLS

#undef fork
#pragma _HP_SECONDARY_DEF _fork fork
#define cma_fork _fork
#define fork fork_sys

#pragma _HP_SECONDARY_DEF _atfork atfork
#define cma_atfork _atfork

#endif
