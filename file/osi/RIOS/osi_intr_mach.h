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
 * Revision 1.1.314.1  1996/10/02  17:59:09  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:44:27  damon]
 *
 * Revision 1.1.309.2  1994/06/09  14:16:19  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:28:36  annie]
 * 
 * Revision 1.1.309.1  1994/02/04  20:25:31  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:16:01  devsrc]
 * 
 * Revision 1.1.307.1  1993/12/07  17:30:22  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  16:06:19  jaffe]
 * 
 * $EndLog$
 */

/* Copyright (C) 1993, 1992 Transarc Corporation - All rights reserved */

/* $Header: /u0/rcs_trees/dce/rcs/file/osi/RIOS/osi_intr_mach.h,v 1.1.314.1 1996/10/02 17:59:09 damon Exp $ */

#ifndef TRANSARC_OSI_INTR_MACH_H
#define	TRANSARC_OSI_INTR_MACH_H

#ifdef KERNEL
#include <sys/m_intr.h>

#define osi_intr_exclude(mutex, cookie) \
    MACRO_BEGIN \
	cookie = i_disable(INTIODONE); \
    MACRO_END

#define osi_intr_allow(mutex, cookie) \
    MACRO_BEGIN \
	i_enable(cookie); \
    MACRO_END

#define osi_intr_start(mutex)
#define osi_intr_end(mutex)

#define osi_intr_wait(cv, mutex)	osi_Sleep(cv)
#define osi_intr_wakeup(cv)		osi_Wakeup(cv)
#endif /* KERNEL */

#endif /* TRANSARC_OSI_INTR_MACH_H */
