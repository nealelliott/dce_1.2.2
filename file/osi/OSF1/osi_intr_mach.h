/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: osi_intr_mach.h,v $
 * Revision 1.1.178.1  1996/10/02  17:58:22  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:43:55  damon]
 *
 * Revision 1.1.173.2  1994/06/09  14:15:36  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:28:06  annie]
 * 
 * Revision 1.1.173.1  1994/02/04  20:24:50  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:15:45  devsrc]
 * 
 * Revision 1.1.171.1  1993/12/07  17:29:58  jaffe
 * 	New File from Transarc 1.0.3a
 * 	[1993/12/02  21:31:21  jaffe]
 * 
 * $EndLog$
 */

/* Copyright (C) 1993 Transarc Corporation - All rights reserved */

/* $Header: /u0/rcs_trees/dce/rcs/file/osi/OSF1/osi_intr_mach.h,v 1.1.178.1 1996/10/02 17:58:22 damon Exp $ */

#ifndef TRANSARC_OSI_INTR_MACH_H
#define	TRANSARC_OSI_INTR_MACH_H

#ifdef _KERNEL
#define osi_intr_exclude(mutex, cookie)	((cookie) = splbio())
#define osi_intr_allow(mutex, cookie)	splx(cookie)
#define osi_intr_start(mutex)
#define osi_intr_end(mutex)
#define osi_intr_wait(cv)		osi_Sleep(cv)
#define osi_intr_wakeup(cv)		osi_Wakeup(cv)
#define osi_splclock() splclock()
#endif /* _KERNEL */

#endif /* !TRANSARC_OSI_INTR_MACH_H */
