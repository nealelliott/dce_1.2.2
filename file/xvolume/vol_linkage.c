/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: vol_linkage.c,v $
 * Revision 1.1.138.1  1996/10/02  19:04:45  damon
 * 	New DFS from Transarc
 * 	[1996/10/01  19:00:00  damon]
 *
 * $EndLog$
 */
/* Copyright (C) 1996, 1993 Transarc Corporation - All rights reserved */

/*
 * This file contains the symbol linkage table that we use in the
 * SunOS 5.x kernel to resolve references to symbols in the DFS core
 * module.  We must list each symbol here for which we defined a macro
 * in vol_init.h.
 * NOTE: The symbols must appear in exactly the same order as their
 * declarations in vol_init.h!
 */

/* $Header: /u0/rcs_trees/dce/rcs/file/xvolume/vol_linkage.c,v 1.1.138.1 1996/10/02 19:04:45 damon Exp $ */

#include <dcedfs/vol_init.h>

#if defined(_KERNEL) && defined(AFS_SUNOS5_ENV)
void *vol_symbol_linkage[VOL_SYMBOL_LINKS] = {
    (void *)vol_Init,
    (void *)vol_fsStartVnodeOp,
    (void *)vol_fsStartBusyOp,
    (void *)vol_fsEndVnodeOp,
    (void *)vol_GCDesc,
    (void *)vol_RCZero,
    (void *)vol_VolInactive,
};
#endif /* _KERNEL && AFS_SUNOS5_ENV */
