/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: bossvr_debug.h,v $
 * Revision 1.1.13.1  1996/10/02  17:04:30  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:04:58  damon]
 *
 * Revision 1.1.7.1  1994/06/09  13:52:27  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:26:09  annie]
 * 
 * Revision 1.1.5.6  1993/01/18  19:56:15  cjd
 * 	Embedded copyright notice
 * 	[1993/01/18  19:32:46  cjd]
 * 
 * Revision 1.1.5.5  1992/11/24  15:41:25  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:09:21  bolinger]
 * 
 * Revision 1.1.5.4  1992/11/18  17:37:51  jaffe
 * 	Transarc delta: comer-ot5749-bosserver-truncates-config-file 1.1
 * 	  Selected comments:
 * 	    There was a problem with the bosserver where the BosConfig file could
 * 	    be truncated under mysterious circumstances.  I believe the problem
 * 	    has been found.  It turned out the the bosserver was writing out the
 * 	    complete BosConfig file for every bnode created -- even those being
 * 	    created on reading the file at startup.  Thus, if the bosserver was
 * 	    killed after having processed a subset of the BosConfig file, it would
 * 	    have overwritten the existing file with the subset: truncated file.
 * 	    The obvious solution is to not rewrite the file when processing it on
 * 	    startup.
 * 	    In case there are additional problems, an end record with checksum can
 * 	    be enabled by turning on the 0x20 bit of the bosserver debug flag.
 * 	    Added a bit to the bossvr_debug_flag to enable end record and checksum
 * 	    processing.
 * 	[1992/11/17  19:35:37  jaffe]
 * 
 * Revision 1.1.5.3  1992/09/25  18:02:27  jaffe
 * 	Transarc delta: bab-ot5392-bossvr-conditional-fork 1.1
 * 	  Selected comments:
 * 	    Whether or not the bosserver executes a fork when it is starting should
 * 	    be dependent upon the setting of the bosserver debug flag.  This will
 * 	    make debugging already built bosservers much easier.
 * 	    ot 5392
 * 	    Add symbolic constant debug bit value indicating no fork.
 * 	[1992/09/23  18:34:37  jaffe]
 * 
 * Revision 1.1.5.2  1992/08/31  18:32:31  jaffe
 * 	Transarc delta: bab-ot4423-bosserver-remove-BOSSVR_LOCAL_DEBUG 1.1
 * 	  Selected comments:
 * 	    All uses of BOSSVR_LOCAL_DEBUG are obsolete (and some of them
 * 	    got in the way of debugging).  They were removed.
 * 	    ot 4423
 * 	    See above.
 * 	Transarc delta: jess-ot2869-com_err-usage-cleanup 1.5
 * 	  Selected comments:
 * 	    The use of com_err in dfs is inconsistent and in some cases
 * 	    incorrect.
 * 	    1) there are many sources which include <com_err.h> but don't
 * 	    use any definitions from it.
 * 	    2) there are sources which use com_err, but don't include the
 * 	    <com_err.h>
 * 	    3) there are subdirectories which do not need libcom_err.a for
 * 	    any reason that link with it.
 * 	    This delta is created to clean up the usage of com_err in dfs.
 * 	    Delete line "#include <com_err.h>
 * 	    Add include if com_err.h indef KERNEL
 * 	[1992/08/28  20:49:27  jaffe]
 * 
 * Revision 1.1.3.4  1992/01/25  20:44:08  zeliff
 * 	dfs6.3 merge, part2
 * 	[1992/01/25  20:00:12  zeliff]
 * 
 * $EndLog$
 */
/*
 * bossvr_debug.h -- external declaration used for debugging
 *
 *	Copyright (C) 1991 Transarc Corporation
 *	All rights reserved.
 *
 */

#ifndef TRANSARC_BOSSVR_DEBUG_H
#define TRANSARC_BOSSVR_DEBUG_H

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/debug.h>
#include <dcedfs/compat.h>		/* dfs_dceErrTxt */

/* bit numbers and masks for using the dmprintf macro */
#define BOSSVR_DEBUG_MASK(bit)		(1 << ((bit) - 1))
#define BOSSVR_DEBUG_BIT_BINDING		1
#define BOSSVR_DEBUG_BIT_RPCTRACE		2
#define BOSSVR_DEBUG_BIT_BNODE		3
#define BOSSVR_DEBUG_BIT_DCE_TRACE	4
#define BOSSVR_DEBUG_BIT_NO_FORK	5
#define BOSSVR_DEBUG_BIT_END_RECORD	6
/*
  BOSSVR_DEBUG_BIT_BINDING	=> 0x00000001
  BOSSVR_DEBUG_BIT_RPCTRACE	=> 0x00000002
  BOSSVR_DEBUG_BIT_BNODE	=> 0x00000004
  BOSSVR_DEBUG_BIT_DCE_TRACE	=> 0x00000008
  BOSSVR_DEBUG_BIT_NO_FORK	=> 0x00000010
  BOSSVR_DEBUG_BIT_END_RECORD	=> 0x00000020
*/

extern long bossvr_debug_flag;

/* only use the debugging macros when we will be retaining the tty */
#if defined(AFS_DEBUG)
#define bossvr_dmprintf(flag, bit, str)	dmprintf(flag, bit, str)
#else  /* defined(AFS_DEBUG) */
#define bossvr_dmprintf(flag, bit, str)
#endif /* defined(AFS_DEBUG) */

#endif /* TRANSARC_BOSSVR_DEBUG_H */
