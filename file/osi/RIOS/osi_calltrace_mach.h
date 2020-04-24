/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: osi_calltrace_mach.h,v $
 * Revision 1.1.9.1  1996/10/02  17:58:59  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:44:16  damon]
 *
 * $EndLog$
 */
/*
 *      Copyright (C) 1992 Transarc Corporation
 *      All rights reserved.
 */

#ifndef _OSI_CALLTRACE_MACH_H
#define _OSI_CALLTRACE_MACH_H

/*
 * Use AIX31 kernel trace facility.
 */
#include <sys/trcmacros.h>
#include <sys/trchkid.h>

/*
 * Use two system events (0xfe, 0xff, which are reserved for temporary
 * NON-EXPORT use by IBM), to delineate function entry/exit events.
 */

#ifdef OSI_CALLTRACE_ENTRY_EVENT 
#undef OSI_CALLTRACE_ENTRY_EVENT 
#endif

#ifdef OSI_CALLTRACE_EXIT_EVENT 
#undef OSI_CALLTRACE_EXIT_EVENT 
#endif

#define OSI_CALLTRACE_ENTRY_EVENT	(0xfe)
#define OSI_CALLTRACE_EXIT_EVENT	(0xff)

/*
 * The function id must be a 16 bit number. We used the reserved data
 * field of the header for the id.
 */
#ifdef OSI_CALLTRACE_ENTRY
#undef OSI_CALLTRACE_ENTRY
#endif

#define OSI_CALLTRACE_ENTRY(id, name) \
    TRCHKL0T((OSI_CALLTRACE_ENTRY_EVENT << 20) | (id))

#ifdef OSI_CALLTRACE_EXIT
#undef OSI_CALLTRACE_EXIT
#endif

#define OSI_CALLTRACE_EXIT(id, name) \
    TRCHKL0T((OSI_CALLTRACE_EXIT_EVENT << 20) | (id))

#endif	/* _OSI_CALLTRACE_MACH_H */

