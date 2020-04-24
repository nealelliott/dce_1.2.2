/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: zlc_linkage.c,v $
 * Revision 1.1.145.1  1996/10/02  19:05:07  damon
 * 	New DFS from Transarc
 * 	[1996/10/01  19:00:14  damon]
 *
 * Revision 1.1.140.2  1994/06/09  14:27:01  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:38:53  annie]
 * 
 * Revision 1.1.140.1  1994/02/04  20:38:03  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:21:28  devsrc]
 * 
 * Revision 1.1.138.1  1993/12/07  17:39:47  jaffe
 * 	New File from Transarc 1.0.3a
 * 	[1993/12/02  21:57:40  jaffe]
 * 
 * $EndLog$
 */
/* Copyright (C) 1995, 1993 Transarc Corporation - All rights reserved */

/*
 * This file contains the symbol linkage table that we use in the
 * SunOS 5.x kernel to resolve references to symbols in the DFS core
 * module.  We must list each symbol here for which we defined a macro
 * in zlc.h.
 * NOTE: The symbols must appear in exactly the same order as their
 * declarations in zlc.h!
 */

/* $Header: /u0/rcs_trees/dce/rcs/file/zlc/zlc_linkage.c,v 1.1.145.1 1996/10/02 19:05:07 damon Exp $ */

#include <dcedfs/zlc.h>

#if defined(_KERNEL) && defined(AFS_SUNOS5_ENV)
void *zlc_symbol_linkage[ZLC_SYMBOL_LINKS] = {
    (void *)zlc_Init,
    (void *)zlc_TryRemove,
    (void *)zlc_CleanVolume,
    (void *)zlc_SetRestartState
};
#endif /* _KERNEL && AFS_SUNOS5_ENV */
