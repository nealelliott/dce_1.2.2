/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: badblock.h,v $
 * Revision 1.1.12.1  1996/10/02  17:16:17  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:07:11  damon]
 *
 * Revision 1.1.6.1  1994/06/09  14:42:26  annie
 * 	cr10982- fix copyright
 * 	[1994/06/09  14:42:10  annie]
 * 
 * Revision 1.1.4.2  1993/01/18  20:55:19  cjd
 * 	embedded copyright notice
 * 	[1993/01/18  17:42:16  cjd]
 * 
 * Revision 1.1.2.3  1992/05/13  21:54:09  jdp
 * 	Transarc delta: ota-glenna-comments-2612 1.1
 * 	  Files modified:
 * 	    anode: aggregate.c, anode.c, badblock.h
 * 	  Selected comments:
 * 	    Insert comments suggested by Glenna Mayo's questions.
 * 	    Describe format of bb file.
 * 	[1992/05/13  18:58:53  jdp]
 * 
 * Revision 1.1.2.2  1992/05/05  20:38:29  jdp
 * 	delta jdp-epimerge-1.0-to-1.0.1b9-into-2.17 1.20
 * 	[1992/05/04  20:09:11  jdp]
 * 
 * Revision 1.1  1992/01/19  02:36:52  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/* badblock.h -- description of the contents of the bad-block container. */


/* Copyright (C) 1992, 1990 Transarc Corporation - All rights reserved */

/* $Header: /u0/rcs_trees/dce/rcs/file/episode/anode/badblock.h,v 1.1.12.1 1996/10/02 17:16:17 damon Exp $" */

#ifndef TRANSARC_EPI_BADBLOCK_H
#define TRANSARC_EPI_BADBLOCK_H

/* The bad block container is made up of 1024-byte frames.  In each frame is a
 * header followed by sequence of entries all of which must fit in 1024 bytes.
 * Each entry contains a header specifying a type-code, a length which does not
 * include the entry header, and type-specific information.  Entries are
 * expected to be aligned properly for referencing an int16; the length field
 * must include any necessary padding.  The thisFrag and totFrags fields in the
 * entry header are indended to allow multiple frames to be linked together
 * (perhaps because they span a 1024 byte frame), however, these fields are not
 * used. */

#define	epiz_bbFrameSize    1024
#define	epiz_BBFRAMEMAGIC   (0x76618c6d)

struct	epiz_bbFrame	{
    u_int32	beginStamp;
    u_int32	bbFrameMagic;		/* epiz_BBFRAMEMAGIC */
    char	data[4];
};

struct	epiz_bbEntry	{
    u_int16	type;			/* epiz_bbetXXX */
    u_int16	length;			/* bytes in this fragment, sans hdr */
    u_int16	thisFrag;		/* index (0-origin) of this fragment */
    u_int16	totFrags;		/* total fragments in the chunk */
    char	data[4];		/* the data itself */
};

/* Types for epiz_bbEntry.type */
#define	epiz_bbetNULL		0	/* no meaning--skip it */
#define	epiz_bbetSuperBlocks	1	/* array of daddr_t's giving disk-block
                                         * locs of duplicate superblocks.  */
#define	epiz_bbetBadBlocks	2	/* array of daddr_t's giving disk-block
                                         * locations of known bad blocks */
#endif /* TRANSARC_EPI_BADBLOCK_H */
