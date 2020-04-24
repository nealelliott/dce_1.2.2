/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: tkc_linkage.c,v $
 * Revision 1.1.138.1  1996/10/02  21:01:26  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:48:10  damon]
 *
 * Revision 1.1.133.2  1994/06/09  14:20:36  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:32:02  annie]
 * 
 * Revision 1.1.133.1  1994/02/04  20:30:27  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:18:11  devsrc]
 * 
 * Revision 1.1.131.1  1993/12/07  17:33:57  jaffe
 * 	New File from Transarc 1.0.3a
 * 	[1993/12/02  21:56:06  jaffe]
 * 
 * $EndLog$
 */
/* Copyright (C) 1993 Transarc Corporation - All rights reserved */

/*
 * This file contains the symbol linkage table that we use in the
 * SunOS 5.x kernel to force references to exported symbols in the
 * DFS core module that are not otherwise used internally.
 */

/* $Header: /u0/rcs_trees/dce/rcs/file/tkc/tkc_linkage.c,v 1.1.138.1 1996/10/02 21:01:26 damon Exp $ */

#include <dcedfs/tkc.h>

#if defined(_KERNEL) && defined(AFS_SUNOS5_ENV)
void *tkc_symbol_linkage[] = {
    (void *)tkc_Init,
    (void *)tkc_Put,
    (void *)tkc_FindVcache,
    (void *)tkc_FlushVnode,
    (void *)tkc_GetTokens,
    (void *)tkc_PutTokens,
    (void *)tkc_Get
};
#endif /* _KERNEL && AFS_SUNOS5_ENV */
