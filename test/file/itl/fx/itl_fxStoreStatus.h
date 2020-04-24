/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 *
 * (C) Copyright 1992 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * itl_fxStoreStatus.h -- 
 *    The definitions and declarations for the afsStoreStatus data type.
 * 
 * $Header: /u0/rcs_trees/dce/rcs/test/file/itl/fx/itl_fxStoreStatus.h,v 1.1.11.1 1996/10/17 18:26:46 damon Exp $
 */
/*
 * HISTORY
 * $Log: itl_fxStoreStatus.h,v $
 * Revision 1.1.11.1  1996/10/17  18:26:46  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:27  damon]
 *
 * Revision 1.1.6.1  1994/02/04  20:44:50  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:24:17  devsrc]
 * 
 * Revision 1.1.4.1  1994/01/28  20:44:39  annie
 * 	expand OSF copyright
 * 	[1994/01/28  20:42:52  annie]
 * 
 * Revision 1.1.2.2  1993/01/13  21:34:45  shl
 * 	Transarc delta: fred-ot-6350-add-fx-tests 1.3
 * 	  Selected comments:
 * 	    To place the DFS fole exporter tests into the tree.
 * 	    Migrate the fx exerciser to the new fx interface (OT 6054)
 * 	    Fix compilation problems.
 * 	[1993/01/13  14:39:08  shl]
 * 
 * $EndLog$
 */

#ifndef ITL_FX_STORE_STATUS_H
#define ITL_FX_STORE_STATUS_H

extern int itlFX_storeStatusDataType;

/*
 *  External function declarations.
 */

extern void itlFX_StoreStatusInit _TAKES((
					void
					));

extern void itlFX_StoreStatusGet _TAKES((
					 OUT afsStoreStatus *statusP
					 ));

#endif  /* ITL_FX_STORE_STATUS_H */
