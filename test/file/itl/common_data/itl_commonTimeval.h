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
 * itl_commonTimeval.h -- 
 *    The definitions and declarations for the timeval data type.
 * 
 * $Header: /u0/rcs_trees/dce/rcs/test/file/itl/common_data/itl_commonTimeval.h,v 1.1.11.1 1996/10/17 18:25:35 damon Exp $
 */
/*
 * HISTORY
 * $Log: itl_commonTimeval.h,v $
 * Revision 1.1.11.1  1996/10/17  18:25:35  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:52:53  damon]
 *
 * Revision 1.1.6.1  1994/02/04  20:43:36  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:23:43  devsrc]
 * 
 * Revision 1.1.4.1  1994/01/28  20:44:17  annie
 * 	expand OSF copyright
 * 	[1994/01/28  20:42:43  annie]
 * 
 * Revision 1.1.2.2  1993/01/13  21:07:34  shl
 * 	Transarc delta: fred-ot-6350-add-fx-tests 1.3
 * 	  Selected comments:
 * 	    To place the DFS fole exporter tests into the tree.
 * 	    Migrate the fx exerciser to the new fx interface (OT 6054)
 * 	    Fix compilation problems.
 * 	[1993/01/13  13:58:56  shl]
 * 
 * $EndLog$
 */

#ifndef ITL_COMMON_TIMEVAL_H
#define ITL_COMMON_TIMEVAL_H

extern int itlCommon_timevalDataType;

/*
 *  External function declarations.
 */

extern void itlCommon_TimevalInit _TAKES((
					void
					));

extern var_val_t *itlCommon_TimevalCreate _TAKES((
						  IN unsigned long seconds,
						  IN unsigned long microSeconds
						  ));

#endif  /* ITL_COMMON_TIMEVAL_H */
