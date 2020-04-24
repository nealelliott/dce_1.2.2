/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: krpc_helper_mach.h,v $
 * Revision 1.1.57.2  1996/02/18  23:46:30  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:45:11  marty]
 *
 * Revision 1.1.57.1  1995/12/08  00:14:33  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:55:57  root]
 * 
 * Revision 1.1.55.1  1994/01/21  22:31:44  cbrooks
 * 	RPC Code Cleanup
 * 	[1994/01/21  20:41:22  cbrooks]
 * 
 * Revision 1.1.53.1  1993/09/01  14:38:16  delgado
 * 	DFS 486 port - change ALIGNMENT to use
 * 	MACHINE_ALIGNMENT specified by the OS
 * 	[1993/09/01  14:34:46  delgado]
 * 
 * Revision 1.1.2.6  1993/01/03  22:35:30  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:51:08  bbelch]
 * 
 * Revision 1.1.2.5  1992/12/23  19:37:31  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  14:49:29  zeliff]
 * 
 * Revision 1.1.2.4  1992/12/06  17:37:18  delgado
 * 	Try to submit again...
 * 	[1992/12/06  17:37:00  delgado]
 * 
 * Revision 1.1.4.2  1992/12/06  17:23:16  rsarbo
 * 	Define TIME macro to be used instead of the kernel's "time"
 * 	variable; the solution we submitted in the previous delta
 * 	broke the OSF1.1 port
 * 
 * Revision 1.1.2.3  1992/12/04  16:14:57  delgado
 * 	Get rid of the time declarations and use the ones in kernel.h
 * 	[1992/12/04  16:14:21  delgado]
 * 
 * Revision 1.1.2.2  1992/12/03  22:31:24  delgado
 * 	New interface for kprc helper
 * 	[1992/12/03  22:30:29  delgado]
 * 
 * $EndLog$
 */

/*
 * Platform dependent header file for krpc_helper module
 */

#ifndef _KRPC_HELPER_MACH_H
#define _KRPC_HELPER_MACH_H


#ifdef _KERNEL

#include <sys/types.h>
#include <sys/kernel.h>
#include <sys/select.h>
#include <sys/param.h>


/* define our alignment requirements for rounding up */


#define ALIGNMENT MACHINE_ALIGNMENT

#define ROUNDUP(x,y)  roundup(x,y)

#define OSI_TRANSACTION_ID (++tid) 

extern struct sel_queue krpch_selq;

#define krpc_helper_machinit()     queue_init(&krpch_selq.links)
#define SELECT_NOTIFY()            select_wakeup(&krpch_selq)

#define TIME time.tv_sec

#endif /* _KERNEL */

#ifndef _KERNEL

#include <fcntl.h>
#define KRPCH_DEV "/dev/krpch"
#define KRPC_OPEN_HELPER() open (KRPCH_DEV, O_RDWR)

#endif /* ! _KERNEL */

#endif /* _KRPC_HELPER_MACH_H */
