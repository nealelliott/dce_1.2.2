/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: fid.c,v $
 * Revision 1.1.107.1  1996/10/02  17:17:14  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:27:09  damon]
 *
 * Revision 1.1.101.3  1994/07/13  22:21:38  devsrc
 * 	merged with bl-10
 * 	[1994/06/28  20:31:49  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:56:01  mbs]
 * 
 * Revision 1.1.101.2  1994/06/09  13:57:33  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:32:42  annie]
 * 
 * Revision 1.1.101.1  1994/02/04  20:11:16  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:09:11  devsrc]
 * 
 * Revision 1.1.99.1  1993/12/07  17:17:03  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  13:41:03  jaffe]
 * 
 * Revision 1.1.4.4  1993/01/18  22:03:33  cjd
 * 	embedded copyright notice
 * 	[1993/01/18  21:43:08  cjd]
 * 
 * Revision 1.1.4.3  1992/11/24  16:20:01  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:17:58  bolinger]
 * 
 * Revision 1.1.4.2  1992/08/31  18:39:26  jaffe
 * 	Fixed RCSID
 * 	[1992/08/30  13:06:43  jaffe]
 * 
 * Revision 1.1.2.3  1992/05/13  21:54:53  jdp
 * 	Transarc delta: ota-16K-fid-index-map-2680 1.1
 * 	  Files modified:
 * 	    anode: anode_block.c, anode_block.h, fid.c, fixed_anode.c
 * 	    anode: many_files.test, volume_table.c
 * 	  Selected comments:
 * 	    In aggregates with 16K block size the calculation of how many anodes fit
 * 	    in a block was being done separately in fid.c and anode_block.h.
 * 	    Clearly this is a "bad thing".  Worse, however, was that the logic in
 * 	    fid.c was wrong!
 * 	    Change anode_block.c to export an array indexed by [log(block size)-10]
 * 	    of anodes per block.  Have fid.c use this.
 * 	    Use epic_anodesPerBlock instead of incorrect local copy.
 * 	[1992/05/13  19:41:19  jdp]
 * 
 * Revision 1.1.2.2  1992/05/05  20:41:24  jdp
 * 	delta jdp-epimerge-1.0-to-1.0.1b9-into-2.17 1.20
 * 	[1992/05/04  20:20:52  jdp]
 * 
 * Revision 1.1  1992/01/19  02:37:23  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/* fid.c -- provides functions to map between files indexes in fids and
 *     epiv-style volume indexes.  The important feature of the mapping being
 *     that it is sparce and leaves indexes corresponding to every other page
 *     of the volume table unused.  These indexes can then be used for auxilary
 *     containers such as acls.
 *
 *     The kernel of this idea is due to Bob Sidebotham. */

/* Copyright (C) 1992, 1991 Transarc Corporation - All rights reserved. */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/debug.h>

RCSID ( "$Header: /u0/rcs_trees/dce/rcs/file/episode/anode/fid.c,v 1.1.107.1 1996/10/02 17:17:14 damon Exp $" )

#include <anode.h>
#include <anode_block.h>
#include <volume_table.h>		/* get epit_useOldFidIndex */
#include <fid.h>

#if !defined(KERNEL)
#include <pthread.h>
#endif /* !KERNEL */

/* epid_IndexFromFid -- converts the index in a fid to a epiv-style index.
 *
 *     Do the arithmetic on epia-style indexes so that the page breaks appear
 *     at the correct places.  Break the index at the page boundary double the
 *     page number and put it back together.  This maps all fid-style indexes
 *     to epia-style index that live on even numbered volume table pages. */

SHARED u_long epid_IndexFromFid (fid, dev)
  IN epi_volFileId_t *fid;
  IN struct async_device *dev;
{
    u_long i = epid_ToIndex (fid);	/* get fid-style index */
    u_int page;
    u_int offset;
    u_int b = epic_anodesPerBlock[dev->logBlkSize-10];
    u_long j;

    if (epit_useOldFidIndex) return i;

    i++;				/* to epia-style origin */
    page = i / b;			/* volume table page */
    offset = i % b;			/* offset in vt page */

    j = (page << 1)*b + offset;
    j--;				/* to epiv-style */
    return j;
}

/* epid_IndexForFid -- maps an epiv-style index to a fid-style index suitable
 *     for inserting into a fid.  It uses the inverse of the algorithm
 *     described above.
 *
 * RETURN CODES -- If the fid is unmappable, namely it would reside on an odd
 *     numbered volume table page, the function returns a zero.  Zero is
 *     invalid in every style. */

SHARED u_long epid_IndexForFid (i, dev)
  IN u_long i;                          /* epiv-style index */
  IN struct async_device *dev;
{
    u_int page;
    u_int offset;
    u_int b = epic_anodesPerBlock[dev->logBlkSize-10];
    u_long j;

    if (epit_useOldFidIndex) return i;

    i++;				/* to epia-style */
    page = i / b;			/* volume table page */
    if (page&1) return 0;		/* odd pages disallowed for fids */
    offset = i % b;			/* offset in vt page */

    j = (page >> 1)*b + offset;
    j--;				/* to fid-style */
    return j;
}

/* epid_LastIndex -- returns the larges legal fid-style index given the largest
 *     possible epiv-style index (e.g. the value epiv_lastIndex returns). */

EXPORT u_long epid_LastIndex (lastIndex, dev)
  IN u_long lastIndex;			/* epiv-style index */
  IN struct async_device *dev;
{
    u_long i;
    u_int page;
    u_int offset;
    u_int b = epic_anodesPerBlock[dev->logBlkSize-10];
    u_long j;

    if (epit_useOldFidIndex) return lastIndex;

    i = lastIndex+1;			/* to epia-style */
    page = i / b;			/* volume table page */
    if (page&1) {
	/* a non-file index maps to next smaller index for a file */
	offset = b-1;
    }
    else offset = i % b;		/* offset in vt page */

    j = (page >> 1)*b + offset;
    j--;				/* to fid-style */
    return j;
}

#ifndef	KERNEL

/* epid_PrintedFid -- produces a printed representation of a fid given the fid,
 *     the length of a buffer and the buffer itself.  Presently the buffer
 *     length is ignored, but should be at least 24 chars long. */

EXPORT char * epid_PrintedFid (fid, len, buf)
  IN epi_volFileId_t *fid;
  IN int len;
  OUT char *buf;
{
    u_long n;
    char *s = buf;
    u_long base;
    buf[len-1] = 0;

    sprintf (buf, "(%u,%u)", epid_ToIndex(fid), epid_ToUnique(fid));

    afsl_MBZ (buf[len-1]);		/* check for overwrite */
    return buf;
}

#endif	/* !KERNEL */
