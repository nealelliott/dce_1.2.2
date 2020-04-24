/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: volreg_linkage.c,v $
 * Revision 1.1.131.1  1996/10/02  21:11:04  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:51:17  damon]
 *
 * Revision 1.1.126.2  1994/06/09  14:25:14  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:37:11  annie]
 * 
 * Revision 1.1.126.1  1994/02/04  20:35:47  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:20:27  devsrc]
 * 
 * Revision 1.1.124.1  1993/12/07  17:37:56  jaffe
 * 	New File from Transarc 1.0.3a
 * 	[1993/12/02  21:56:57  jaffe]
 * 
 * $EndLog$
 */
/* Copyright (C) 1993 Transarc Corporation - All rights reserved */

/*
 * This file contains the symbol linkage table that we use in the
 * SunOS 5.x kernel to resolve references to symbols in the DFS core
 * module.  We must list each symbol here for which we defined a macro
 * in volreg.h.
 * NOTE: The symbols must appear in exactly the same order as their
 * declarations in volreg.h!
 */

/* $Header: /u0/rcs_trees/dce/rcs/file/volreg/volreg_linkage.c,v 1.1.131.1 1996/10/02 21:11:04 damon Exp $ */

#include <dcedfs/volreg.h>

#if defined(_KERNEL) && defined(AFS_SUNOS5_ENV)
void *volreg_symbol_linkage[] = {
    (void *)volreg_Init,
    (void *)volreg_Enter,
    (void *)volreg_Delete,
    (void *)volreg_Lookup,
    (void *)volreg_LookupByName
};
#endif /* _KERNEL && AFS_SUNOS5_ENV */
