/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: getasyncinfo.c,v $
 * Revision 1.1.8.2  1996/02/17  23:33:16  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:49:32  marty]
 *
 * Revision 1.1.8.1  1995/12/07  21:50:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:00:45  root]
 * 
 * Revision 1.1.6.1  1994/06/09  13:35:56  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:26:28  devsrc]
 * 
 * Revision 1.1.4.4  1992/12/15  22:23:34  alan
 * 	Insert copyright notices
 * 	[1992/12/07  16:03:07  alan]
 * 
 * Revision 1.1.4.3  1992/12/07  13:21:10  zeliff
 * 	Removed duplicate OSF copyright markers
 * 	[1992/12/06  00:45:08  zeliff]
 * 
 * Revision 1.1.4.2  1992/09/03  14:34:14  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:01:13  bolinger]
 * 
 * Revision 1.1.2.2  1992/08/28  20:59:58  bolinger
 * 	Do the update for real, this time.
 * 	[1992/08/28  20:59:26  bolinger]
 * 
 * Revision 1.1  1992/01/19  14:38:40  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

#include <cma.h>
#include <cma_defs.h>
#include <cma_tcb.h>
#include <cma_stack.h>
#include <signal.h>

void
cma__get_ibm_async_info(stack)
struct sigcontext *stack;
{
	cma__bugcheck("called cma__get_ibm_async_info() -- shouldn't happen");
}
/*  DEC/CMS REPLACEMENT HISTORY, Element GETASYNCINFO.C */
/*  *5    19-JUN-1991 12:16:17 BUTENHOF "[-.rios]" */
/*  *4    12-JUN-1991 13:07:14 BUTENHOF "Upgrade to latest IBM versions" */
/*  *3    28-FEB-1991 13:06:37 BUTENHOF "Fix include file" */
/*  *2    31-JAN-1991 13:00:09 SCALES "Convert to stream format for ULTRIX build" */
/*  *1    15-JAN-1991 22:27:13 BUTENHOF "Support for preemption" */
/*  DEC/CMS REPLACEMENT HISTORY, Element GETASYNCINFO.C */
