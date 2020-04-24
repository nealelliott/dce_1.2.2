/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: bossvr_sleep.h,v $
 * Revision 1.1.10.1  1996/10/02  17:04:47  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:05:07  damon]
 *
 * Revision 1.1.4.1  1994/06/09  13:52:35  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:26:24  annie]
 * 
 * Revision 1.1.2.2  1993/01/18  19:57:09  cjd
 * 	Embedded copyright notice
 * 	[1993/01/18  19:33:31  cjd]
 * 
 * Revision 1.1  1992/01/19  02:49:28  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  bossvr_sleep.h -- prototypes for routines that implement a utility simulating
 * kernel-type sleeping in a CMA environment
 *
 *
 *	Copyright (C) 1991, 1990 Transarc Corporation
 *	All rights reserved.
 */


/* basic defines for compatibility SLEEP mode package */
#ifndef _TRANSARC_BOSSVR_SLEEP_INCL__
#define _TRANSARC_BOSSVR_SLEEP_INCL__ 1

#define SLEEP_HASH_CONDS 64	/* must be power of 2 for lock_HashAddr to work */

IMPORT int bossvr_InitSleepers _TAKES((
				     void
				   ));
IMPORT void bossvr_Sleep _TAKES((
			       long	addr
			     ));
IMPORT void bossvr_Wakeup _TAKES((
				long	addr
			      ));

#endif /* TRANSARC_BOSSVR_SLEEP_INCL */
