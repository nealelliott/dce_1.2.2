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
 * itl_fxVolsync.h -- 
 *    The definitions and declarations for the afsVolsync data type.
 * 
 * $Header: /u0/rcs_trees/dce/rcs/test/file/itl/fx/itl_fxVolSync.h,v 1.1.11.1 1996/10/17 18:26:55 damon Exp $
 */
/*
 * HISTORY
 * $Log: itl_fxVolSync.h,v $
 * Revision 1.1.11.1  1996/10/17  18:26:55  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:32  damon]
 *
 * Revision 1.1.6.1  1994/02/04  20:44:58  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:24:20  devsrc]
 * 
 * Revision 1.1.4.1  1994/01/28  20:44:42  annie
 * 	expand OSF copyright
 * 	[1994/01/28  20:42:53  annie]
 * 
 * Revision 1.1.2.2  1993/01/13  21:35:33  shl
 * 	Transarc delta: fred-ot-6350-add-fx-tests 1.3
 * 	  Selected comments:
 * 	    To place the DFS fole exporter tests into the tree.
 * 	    Migrate the fx exerciser to the new fx interface (OT 6054)
 * 	    Fix compilation problems.
 * 	[1993/01/13  14:42:41  shl]
 * 
 * $EndLog$
 */

#ifndef ITL_FX_VOL_SYNC_H
#define ITL_FX_VOL_SYNC_H

extern int itlFX_volSyncDataType;

/*
 *  External function declarations.
 */

extern void itlFX_VolSyncInit _TAKES((
					void
					));

#endif  /* ITL_FX_VOL_SYNC_H */
