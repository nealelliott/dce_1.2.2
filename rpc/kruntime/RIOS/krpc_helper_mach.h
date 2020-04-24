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
 * Revision 1.1.6.2  1996/02/18  23:46:34  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:45:15  marty]
 *
 * Revision 1.1.6.1  1995/12/08  00:14:52  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:56:10  root]
 * 
 * Revision 1.1.4.2  1994/02/02  21:48:59  cbrooks
 * 	OT9855 code cleanup breaks KRPC
 * 	[1994/02/02  21:00:13  cbrooks]
 * 
 * Revision 1.1.4.1  1994/01/23  21:37:08  cbrooks
 * 	RPC Code Cleanup - CR 9797
 * 	[1994/01/23  21:05:05  cbrooks]
 * 
 * Revision 1.1.2.6  1993/01/03  22:35:56  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:51:54  bbelch]
 * 
 * Revision 1.1.2.5  1992/12/23  19:38:23  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  14:52:37  zeliff]
 * 
 * Revision 1.1.2.4  1992/12/06  17:15:15  rsarbo
 * 	Define TIME macro to be used instead of the kernel's "time"
 * 	variable; the solution we submitted in the previous delta
 * 	broke the OSF1.1 port
 * 	[1992/12/06  17:14:49  rsarbo]
 * 
 * Revision 1.1.2.3  1992/12/04  19:47:20  delgado
 * 	Delete definition for osi_Time; it is unnecessary and is causing
 * 	compilation errors in dfs.
 * 	[1992/12/04  19:46:26  delgado]
 * 
 * Revision 1.1.2.2  1992/12/03  22:58:58  delgado
 * 	Changes for new krpc helper interface
 * 	[1992/12/03  22:58:22  delgado]
 * 
 * $EndLog$
 */


/*
 * Platform dependent header file for krpc_helper module
 */

#ifndef _KRPC_HELPER_MACH_INCL__
#define _KRPC_HELPER_MACH_INCL__

#ifdef _KERNEL
#include <sys/select.h>

/* define our alignment requirements for rounding up */
#define ALIGNMENT 4

#define ROUNDUP(x,y)  roundup(x,y)

#define OSI_TRANSACTION_ID (++tid) 

extern time_t time;
#define TIME time
#define KRPC_HELPER_MACHINIT()

extern dev_t krpchdd_dev;
#define SELECT_NOTIFY() selnotify((int)krpchdd_dev, 0, POLLIN);
#endif /*  _KERNEL */

#include <fcntl.h>
#define KRPCH_DEV "/dev/krpch"
#define KRPC_OPEN_HELPER() open (KRPCH_DEV, O_RDWR)

#endif /* _KRPC_HELPER_MACH_INCL__ */
