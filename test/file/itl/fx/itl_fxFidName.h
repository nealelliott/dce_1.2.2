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
 * itl_fxFidName.h -- 
 *    The definitions and declarations for the afsFidName data type.
 * 
 * $Header: /u0/rcs_trees/dce/rcs/test/file/itl/fx/itl_fxFidName.h,v 1.1.11.1 1996/10/17 18:26:30 damon Exp $
 */
/*
 * HISTORY
 * $Log: itl_fxFidName.h,v $
 * Revision 1.1.11.1  1996/10/17  18:26:30  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:19  damon]
 *
 * Revision 1.1.6.1  1994/02/04  20:44:30  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:24:08  devsrc]
 * 
 * Revision 1.1.4.1  1994/01/28  20:44:25  annie
 * 	expand OSF copyright
 * 	[1994/01/28  20:42:46  annie]
 * 
 * Revision 1.1.2.2  1993/01/13  21:11:06  shl
 * 	Transarc delta: fred-ot-6350-add-fx-tests 1.3
 * 	  Selected comments:
 * 	    To place the DFS fole exporter tests into the tree.
 * 	    Migrate the fx exerciser to the new fx interface (OT 6054)
 * 	    Fix compilation problems.
 * 	    Declare the parameter with a valid type.
 * 	[1993/01/13  14:29:08  shl]
 * 
 * $EndLog$
 */

#ifndef ITL_FX_FID_NAME_H
#define ITL_FX_FID_NAME_H

#include <dcedfs/common_data.h>

extern int itlFX_fidNameDataType;

/*
 *  External function declarations.
 */

extern void itlFX_FidNameInit _TAKES((
				      void
				      ));

extern void itlFX_FidNameGet _TAKES((
				     OUT afsFidTaggedName *fidNameP
				     ));

#endif  /* ITL_FX_FID_NAME_H */
