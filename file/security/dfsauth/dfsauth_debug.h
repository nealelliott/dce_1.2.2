/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dfsauth_debug.h,v $
 * Revision 1.1.9.1  1996/10/02  20:58:50  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:47:16  damon]
 *
 * Revision 1.1.4.1  1994/06/09  14:19:25  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:31:21  annie]
 * 
 * Revision 1.1.2.4  1993/01/21  15:19:04  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  15:56:59  cjd]
 * 
 * Revision 1.1.2.3  1992/11/24  18:31:47  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:23:35  bolinger]
 * 
 * Revision 1.1.2.2  1992/08/31  21:13:19  jaffe
 * 	Transarc delta: bab-ot4321-security-dfsauth-dce-error-trace 1.1
 * 	  Selected comments:
 * 	    dfsauth code that interacts with the DCE security service was
 * 	    not making appropriate use of DFSAUTH_DEBUG_BIT_DCE_ERRORS &
 * 	    DFSAUTH_DEBUG_BIT_DCE_TRACE.  This use was cleaned up to make it
 * 	    easier to set the dfsauth_debug_flag to an appropriate value to
 * 	    get more information out of the dfsauth package.
 * 	    ot 4321
 * 	    See above.
 * 	[1992/08/30  12:23:04  jaffe]
 * 
 * Revision 1.1  1992/01/19  02:52:17  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *	dfsauth_debug.h -- debugging flag and routines for dfsauth package
 *
 * Copyright (C) 1991 Transarc Corporation
 * All rights reserved.
 */

#ifndef TRANSARC_DFSAUTH_DEBUG_H
#define TRANSARC_DFSAUTH_DEBUG_H 1

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/debug.h>

IMPORT long int dfsauth_debug_flag;

#define DFSAUTH_DEBUG_MASK(bitnum)		((1L) << (bitnum - 1))

#define DFSAUTH_DEBUG_BIT_WARNINGS	1
#define DFSAUTH_DEBUG_BIT_PRINCIPAL	2
#define DFSAUTH_DEBUG_BIT_DCE_TRACE	3
#define DFSAUTH_DEBUG_BIT_DCE_ERRORS	4
#define DFSAUTH_DEBUG_BIT_KEYTAB	5
#define DFSAUTH_DEBUG_BIT_TRACE		6
#define DFSAUTH_DEBUG_BIT_PATHS		7

#define DFSAUTH_DEBUG_BIT_FS		32

/*
 * DFSAUTH_DEBUG_BIT_WARNINGS	=> 0x01
 * DFSAUTH_DEBUG_BIT_PRINCIPAL	=> 0x02
 * DFSAUTH_DEBUG_BIT_DCE_TRACE	=> 0x04
 * DFSAUTH_DEBUG_BIT_DCE_ERRORS	=> 0x08
 * DFSAUTH_DEBUG_BIT_KEYTAB	=> 0x10
 * DFSAUTH_DEBUG_BIT_TRACE	=> 0x20
 * DFSAUTH_DEBUG_BIT_PATHS	=> 0x40
 * DFSAUTH_DEBUG_BIT_FS		=> 0x80000000
 */

#define DFSAUTH_DEBUG_DCE_TRACE_ERRORS_MASK	\
  (DFSAUTH_DEBUG_MASK(DFSAUTH_DEBUG_BIT_DCE_TRACE) | \
   DFSAUTH_DEBUG_MASK(DFSAUTH_DEBUG_BIT_DCE_ERRORS))

#define DFSAUTH_DEBUG_DCE_TRACE_ERRORS	\
  (dfsauth_debug_flag & DFSAUTH_DEBUG_DCE_TRACE_ERRORS_MASK)

IMPORT int ParseDebugFlag _TAKES((
				  char *	flagString,
				  long *	flagValP
				));

IMPORT void dfsauth_LogParamError _TAKES((
					  char *	routineNameP,
					  char *	parameterNameP
					));

#endif /* TRANSARC_DFSAUTH_DEBUG_H */
