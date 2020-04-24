/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dacl_linkage.c,v $
 * Revision 1.1.141.1  1996/10/02  18:15:41  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:46:34  damon]
 *
 * Revision 1.1.136.2  1994/06/09  14:18:40  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:30:41  annie]
 * 
 * Revision 1.1.136.1  1994/02/04  20:28:33  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:17:23  devsrc]
 * 
 * Revision 1.1.134.1  1993/12/07  17:32:40  jaffe
 * 	New File from Transarc 1.0.3a
 * 	[1993/12/02  21:55:50  jaffe]
 * 
 * $EndLog$
 */
/* Copyright (C) 1993 Transarc Corporation - All rights reserved */

/*
 * This file contains the symbol linkage table that we use in the
 * SunOS 5.x kernel to resolve references to symbols in the DFS core
 * module.  We must list each symbol here for which we defined a macro
 * in dacl.p.h and dacl_mgruuids.h.
 * NOTE: The symbols must appear in exactly the same order as their
 * declarations in dacl.p.h and dacl_mgruuids.h!
 */

/* $Header: /u0/rcs_trees/dce/rcs/file/security/dacl/dacl_linkage.c,v 1.1.141.1 1996/10/02 18:15:41 damon Exp $ */

#include <dcedfs/dacl.h>
#include <dcedfs/dacl_mgruuids.h>

#if defined(_KERNEL) && defined(AFS_SUNOS5_ENV)
void *dacl_symbol_linkage[] = {
    (void *)dacl_SetSysAdminGroupID,
    (void *)dacl_GetSysAdminGroupID,
    (void *)dacl_SetLocalCellID,
    (void *)dacl_GetLocalCellID,
    (void *)dacl_epi_CheckAccessAllowedPac,
    (void *)dacl_epi_CheckAccessPac,
    (void *)dacl_AddPermBitsToOnePermset,
    (void *)dacl_ChmodOnePermset,
    (void *)dacl_OnePermsetToPermBits,
    (void *)dacl_epi_FlattenAcl,
    (void *)dacl_FlattenAcl,
    (void *)dacl_FlattenAclWithModeBits,
    (void *)dacl_epi_ValidateBuffer,
    (void *)dacl_ExtractPermBits,
    (void *)dacl_ExtractMinimumPermSet,
    (void *)dacl_ParseAcl,
    (void *)dacl_FreeAclEntries,
    (void *)dacl_PacFromUcred,
    (void *)dacl_PacListFromUcred,
    (void *)dacl_InitEpiAcl,
    (void *)dacl_ChangeRealm,
    (void *)dacl_ChangeUnauthMask,
    (void *)&dacl_localCellID,
    (void *)&episodeAclMgmtUuid
};
#endif /* _KERNEL && AFS_SUNOS5_ENV */
