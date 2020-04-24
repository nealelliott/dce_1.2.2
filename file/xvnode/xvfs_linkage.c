/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xvfs_linkage.c,v $
 * Revision 1.1.136.1  1996/10/02  19:04:10  damon
 * 	New DFS from Transarc
 * 	[1996/10/01  18:59:41  damon]
 *
 * $EndLog$
 */
/* Copyright (C) 1996, 1993 Transarc Corporation - All rights reserved */

/*
 * This file contains the symbol linkage table that we use in the
 * SunOS 5.x kernel to force references to exported symbols in the
 * DFS core module that are not otherwise used internally.
 */

/* $Header: /u0/rcs_trees/dce/rcs/file/xvnode/xvfs_linkage.c,v 1.1.136.1 1996/10/02 19:04:10 damon Exp $ */

#include <dcedfs/xvfs_export.h>

#if defined(_KERNEL) && defined(AFS_SUNOS5_ENV)
void *xvfs_symbol_linkage[] = {
    (void *)xvfs_ConvertDev,
    (void *)xvfs_convert,
    (void *)xvfs_InitFromVFSOps,
    (void *)xvfs_InitFromXOps,
    (void *)xvfs_SetAdminGroupID,
    (void *)xvfs_NullTxvattr
};
#endif /* _KERNEL && AFS_SUNOS5_ENV */
