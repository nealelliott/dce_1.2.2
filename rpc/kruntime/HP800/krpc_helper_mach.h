/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: krpc_helper_mach.h,v $
 * Revision 1.1.69.2  1996/02/18  23:46:25  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:45:07  marty]
 *
 * Revision 1.1.69.1  1995/12/08  00:14:00  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:55:41  root]
 * 
 * Revision 1.1.67.2  1994/06/10  20:54:09  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:59:54  devsrc]
 * 
 * Revision 1.1.67.1  1994/01/21  22:31:24  cbrooks
 * 	RPC Code Cleanup
 * 	[1994/01/21  20:33:01  cbrooks]
 * 
 * Revision 1.1.4.2  1993/06/10  19:23:28  sommerfeld
 * 	Initial HPUX RP version.
 * 	[1993/06/03  22:04:06  kissel]
 * 
 * 	Initial revision.
 * 	[1993/01/15  21:17:51  toml]
 * 
 * $EndLog$
 */

/*
 * Platform dependent header file for krpc_helper module
 */

#ifndef _KRPC_HELPER_MACH_H
#define _KRPC_HELPER_MACH_H

#ifdef _KERNEL

/* define our alignment requirements for rounding up */
#define ALIGNMENT 4

#define ROUNDUP(x,y)  roundup(x,y)

#define OSI_TRANSACTION_ID (++tid) 

extern struct sel_queue krpch_selq;

#define KRPC_HELPER_MACHINIT()

#define SELECT_NOTIFY() krpc_wakeup()

extern struct timeval time;
#define TIME time.tv_sec

#else  /* _KERNEL */

#include <fcntl.h>
#define KRPCH_DEV "/dev/krpch"
#define KRPC_OPEN_HELPER() open (KRPCH_DEV, O_RDWR)

#endif /* _KERNEL */

#endif /* _KRPC_HELPER_MACH_H*/
