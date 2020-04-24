/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* Copyright (C) 1994, 1993 Transarc Corporation - All rights reserved */
/*
 * HISTORY
 * $Log: strategy_mach.h,v $
 * Revision 1.1.26.1  1996/10/02  17:15:51  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:07:03  damon]
 *
 * Revision 1.1.20.2  1994/06/09  13:56:17  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:30:54  annie]
 * 
 * Revision 1.1.20.1  1994/02/04  20:10:13  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:08:35  devsrc]
 * 
 * Revision 1.1.18.1  1993/12/07  17:16:05  jaffe
 * 	New File from Transarc 1.0.3a
 * 	[1993/12/02  20:20:10  jaffe]
 * 
 * $EndLog$
 */

/* $Header: /u0/rcs_trees/dce/rcs/file/episode/anode/RIOS/strategy_mach.h,v 1.1.26.1 1996/10/02 17:15:51 damon Exp $ */

#ifndef TRANSARC_EPISODE_STRATEGY_MACH_H
#define TRANSARC_EPISODE_STRATEGY_MACH_H
/*
 * OS-specific definitions for strategy.
 */
extern void epia_MapBufs(struct buf *bp);
extern void epia_UnmapBufs(struct buf *bp);

#endif /* !TRANSARC_EPISODE_STRATEGY_MACH_H */
