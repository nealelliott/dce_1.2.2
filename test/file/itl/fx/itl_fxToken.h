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
 * itl_fxToken.h -- 
 *    Header file for the token revocation code.
 * 
 * $Header: /u0/rcs_trees/dce/rcs/test/file/itl/fx/itl_fxToken.h,v 1.1.11.1 1996/10/17 18:26:51 damon Exp $
 */
/*
 * HISTORY
 * $Log: itl_fxToken.h,v $
 * Revision 1.1.11.1  1996/10/17  18:26:51  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:30  damon]
 *
 * Revision 1.1.6.1  1994/02/04  20:44:53  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:24:19  devsrc]
 * 
 * Revision 1.1.4.1  1994/01/28  20:44:40  annie
 * 	expand OSF copyright
 * 	[1994/01/28  20:42:53  annie]
 * 
 * Revision 1.1.2.2  1993/01/13  21:35:16  shl
 * 	Transarc delta: fred-ot-6350-add-fx-tests 1.3
 * 	  Selected comments:
 * 	    To place the DFS fole exporter tests into the tree.
 * 	    Migrate the fx exerciser to the new fx interface (OT 6054)
 * 	    Fix compilation problems.
 * 	[1993/01/13  14:40:20  shl]
 * 
 * $EndLog$
 */

#ifndef ITL_FX_TOKEN_H
#define ITL_FX_TOKEN_H

#include <dcedfs/afs4int.h>

extern long epochStartTime;
extern struct afsNetData tokenServerAddress;

extern void itlFX_TokenInit _TAKES((
				    void
				    ));


#endif  /* ITL_FX_TOKEN_H */
